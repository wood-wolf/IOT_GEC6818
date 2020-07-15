#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include "jpeglib.h"
#include "jconfig.h"

#include "jpeg.h"

#define FB_SIZE 800*480*4


/****************************************************
 *函数名称:file_size_get
 *输入参数:pfile_path	-文件路径
 *返 回 值:-1		-失败
		   其他值	-文件大小
 *说	明:获取文件大小
 ****************************************************/
unsigned long file_size_get(const char *pfile_path)
{
	unsigned long filesize = -1;	
	struct stat statbuff;
	
	if(stat(pfile_path, &statbuff) < 0)
	{
		return filesize;
	}
	else
	{
		filesize = statbuff.st_size;
	}
	
	return filesize;
}

/*

x			: 起点
y			: 起点
pjpg_path	：图片路径
pjpg_buf    ：默认为NULL
jpg_buf_size：默认设为0
jpg_half	: 为1时显示一半，为0时显示全部
*/
int lcd_draw_jpg(unsigned int x,unsigned int y,const char *pjpg_path,char *pjpg_buf,unsigned int jpg_buf_size,unsigned int jpg_half)  
{
	
	char g_color_buf[FB_SIZE]={0};
	
	//初始化LCD
	int g_fb_fd = open("/dev/fb0", O_RDWR);
	
	if(g_fb_fd<0)
	{
			printf("open lcd error\n");
			return -1;
	}

	int *g_pfb_memory  = (int *)mmap(	NULL, 					//映射区的开始地址，设置为NULL时表示由系统决定映射区的起始地址
									FB_SIZE, 				//映射区的长度
									PROT_READ|PROT_WRITE, 	//内容可以被读取和写入
									MAP_SHARED,				//共享内存
									g_fb_fd, 				//有效的文件描述词
									0						//被映射对象内容的起点
								);
	
	/*定义解码对象，错误处理对象*/
	struct 	jpeg_decompress_struct 	cinfo;
	struct 	jpeg_error_mgr 			jerr;	
	
	char *pcolor_buf = g_color_buf;
	char 	*pjpg;
	
	unsigned int 	i=0;
	unsigned int	color =0;
	unsigned int	count =0;
	
	unsigned int 	x_s = x;
	unsigned int 	x_e ;	
	unsigned int 	y_e ;
	
			 int	jpg_fd;
	unsigned int 	jpg_size;
	
	unsigned int 	jpg_width;
	unsigned int 	jpg_height;
	

	if(pjpg_path!=NULL)
	{
		/* 申请jpg资源，权限可读可写 */	
		jpg_fd=open(pjpg_path,O_RDWR);
		
		if(jpg_fd == -1)
		{
		   printf("open %s error\n",pjpg_path);
		   
		   return -1;	
		}	
		
		/* 获取jpg文件的大小 */
		jpg_size=file_size_get(pjpg_path);	

		/* 为jpg文件申请内存空间 */	
		pjpg = malloc(jpg_size);

		/* 读取jpg文件所有内容到内存 */		
		read(jpg_fd,pjpg,jpg_size);
	}
	else
	{
		jpg_size = jpg_buf_size;
		
		pjpg = pjpg_buf;
	}

	/*注册出错处理*/
	cinfo.err = jpeg_std_error(&jerr);

	/*创建解码*/
	jpeg_create_decompress(&cinfo);

	/*直接解码内存数据*/		
	jpeg_mem_src(&cinfo,pjpg,jpg_size);
	
	/*读文件头*/
	jpeg_read_header(&cinfo, TRUE);

	/*开始解码*/
	jpeg_start_decompress(&cinfo);	
	
	
	if(jpg_half)
	{
		x_e	= x_s+(cinfo.output_width/2);
		y_e	= y  +(cinfo.output_height/2);		
		
		/*读解码数据*/
		while(cinfo.output_scanline < cinfo.output_height)
		{		
			pcolor_buf = g_color_buf;
			
			/* 读取jpg一行的rgb值 */
			jpeg_read_scanlines(&cinfo,(JSAMPARRAY)&pcolor_buf,1);			
			
			/* 再读取jpg一行的rgb值 */
			jpeg_read_scanlines(&cinfo,(JSAMPARRAY)&pcolor_buf,1);

			for(i=0; i<(cinfo.output_width/2); i++)
			{
				/* 获取rgb值 */
				color = 		*(pcolor_buf+2);
				color = color | *(pcolor_buf+1)<<8;
				color = color | *(pcolor_buf)<<16;
				
				/* 显示像素点 */
				*(g_pfb_memory+y*800+x)=color;
				
				pcolor_buf +=6;
				
				x++;
			}
			
			/* 换行 */
			y++;					
			
			
			x = x_s;	

			
		}
	}
	else
	{
		x_e	= x_s+cinfo.output_width;
		y_e	= y  +cinfo.output_height;	

		/*读解码数据*/
		while(cinfo.output_scanline < cinfo.output_height )
		{		
			pcolor_buf = g_color_buf;
			
			/* 读取jpg一行的rgb值 */
			jpeg_read_scanlines(&cinfo,(JSAMPARRAY)&pcolor_buf,1);
			
			for(i=0; i<cinfo.output_width; i++)
			{
				/* 获取rgb值 */
				color = 		*(pcolor_buf+2);
				color = color | *(pcolor_buf+1)<<8;
				color = color | *(pcolor_buf)<<16;
				
				/* 显示像素点 */
				*(g_pfb_memory+y*800+x)=color;
				
				pcolor_buf +=3;
				
				x++;
			}
			
			/* 换行 */
			y++;			
			
			x = x_s;
			
		}		
	}
	
	/*解码完成*/
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	if(pjpg_path!=NULL)
	{
		/* 关闭jpg文件 */
		close(jpg_fd);	
		
		/* 释放jpg文件内存空间 */
		free(pjpg);		
	}
	//LCD关闭
	/* 取消内存映射 */
	munmap(g_pfb_memory, FB_SIZE);
	
	/* 关闭LCD设备 */
	close(g_fb_fd);
	return 0;
}

void show_jpeg(char *pathname)
{
	int g_lcd_fd = open("/dev/fb0",O_RDWR);

	if(g_lcd_fd == -1)
	{
		printf("open lcd failure\n");
		
	}
	//映射
	int *g_fd_men = (int *)mmap(NULL,800*480*4,PROT_READ|PROT_WRITE,MAP_SHARED,g_lcd_fd,0);
	if(g_fd_men == NULL)
	{
		printf("mmap g_fd_men failed!\n");
		
	}
	

	//一、为JPEG对象分配空间并初始化
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

	//二、指定解压缩数据源

	FILE * infile;
	if ((infile = fopen(pathname, "rb")) == NULL)
	{
		printf("fopen jpg failed!\n");
		
	}
	jpeg_stdio_src(&cinfo, infile);

	//三、获取文件信息
	jpeg_read_header(&cinfo, TRUE);

	//四、解压
	jpeg_start_decompress(&cinfo);

	//五、取出数据
	int x,i;
	char *buffer;
	int row_stride = cinfo.output_width * cinfo.output_components;
	//开辟一行数据空间
	buffer = malloc(row_stride);

	JSAMPROW a;
	printf("a=%c\n", a);
	 
	//六、逐行扫描
	while (cinfo.output_scanline < cinfo.output_height)
    {

		(void) jpeg_read_scanlines(&cinfo, (JSAMPARRAY)&buffer, 1); // 读取一行数据
		{
			for(i=0,x=0;x<cinfo.output_width;x++)
			{
				*(g_fd_men+(cinfo.output_scanline-1)*800+x) = buffer[i]<<16|buffer[i+1]<<8|buffer[i+2];
				i=i+3;
			}
		}
		 
    }
	//七、解压完毕
	jpeg_finish_decompress(&cinfo);
	//八、释放资源
	jpeg_destroy_decompress(&cinfo);
    fclose(infile);
	
	munmap(g_fd_men,800*480*4);
	close(g_lcd_fd);

}




/*
int main()
{
	show_jpeg("2.jpg");

	lcd_draw_jpg(500,300,"small.jpg",NULL,0,0);
	lcd_draw_jpg(300,300,"small.jpg",NULL,0,0);
}
	
*/