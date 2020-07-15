#include <stdio.h>   	//printf scanf
#include <fcntl.h>		//open write read lseek close  	 
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <pthread.h>

#include "camera.h"
#include "api_v4l2.h"
#include "jpeg.h"
#include "yuyv.h"




static unsigned char g_color_buf[FB_SIZE]={0};

int ts_x, ts_y;
int flag = 1;

int  lcd_fd;
int *mmap_fd;
struct jpg_data video_buf;//定义结构体变量

//初始化LCD
int lcd_open(void)
{
	lcd_fd = open("/dev/fb0", O_RDWR);
	
	if(lcd_fd<0)
	{
		printf("open lcd error\n");
		return -1;
	}
	return 0;
}

int mmap_lcd(void)
{
	mmap_fd  = (int *)mmap(	NULL, 					//映射区的开始地址，设置为NULL时表示由系统决定映射区的起始地址
									FB_SIZE, 				//映射区的长度
									PROT_READ|PROT_WRITE, 	//内容可以被读取和写入
									MAP_SHARED,				//共享内存
									lcd_fd, 				//有效的文件描述词
									0						//被映射对象内容的起点
	);
	return lcd_fd;

}

//LCD画点
void lcd_draw_point(unsigned int x,unsigned int y, unsigned int color)
{
	*(mmap_fd+y*800+x)=color;
}

//显示摄像头捕捉
int show_video_data(unsigned int x,unsigned int y,char *pjpg_buf,unsigned int jpg_buf_size)  
{
	/*定义解码对象，错误处理对象*/
	struct 	jpeg_decompress_struct 	cinfo;
	struct 	jpeg_error_mgr 			jerr;	
	
	unsigned char 	*pcolor_buf = g_color_buf;
	char 	*pjpg;
	
	unsigned int 	i=0;
	unsigned int	color =0;
	//unsigned int	count =0;
	
	unsigned int 	x_s = x;
	unsigned int 	x_e ;	
	unsigned int 	y_e ;
	
	pjpg = pjpg_buf;

	/*注册出错处理*/
	cinfo.err = jpeg_std_error(&jerr);

	/*创建解码*/
	jpeg_create_decompress(&cinfo);

	/*直接解码内存数据*/		
	jpeg_mem_src(&cinfo,pjpg,jpg_buf_size);
	
	/*读文件头*/
	jpeg_read_header(&cinfo, TRUE);

	/*开始解码*/
	jpeg_start_decompress(&cinfo);	
	
	x_e	= x_s+cinfo.output_width;
	y_e	= y  +cinfo.output_height;	

	/*读解码数据*/
	while(cinfo.output_scanline < cinfo.output_height )
	{		
		pcolor_buf = g_color_buf;
			
		/* 读取jpg一行的rgb值 */
		jpeg_read_scanlines(&cinfo,&pcolor_buf,1);
		
		for(i=0; i<cinfo.output_width; i++)
		{
			/* 获取rgb值 */
			color = 		*(pcolor_buf+2);
			color = color | *(pcolor_buf+1)<<8;
			color = color | *(pcolor_buf)<<16;
			
			/* 显示像素点 */
			lcd_draw_point(x,y,color);
			
			pcolor_buf +=3;
			
			x++;
		}
		
		/* 换行 */
		y++;			
		
		x = x_s;
		
	}		
			
	/*解码完成*/
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	return 0;
}



//LCD关闭
void lcd_close(void)
{
	
	/* 取消内存映射 */
	munmap(mmap_fd, FB_SIZE);
	
	/* 关闭LCD设备 */
	close(lcd_fd);
}

//获取jpg文件的大小
// unsigned long file_size_get(const char *pfile_path)
// {
// 	unsigned long filesize = -1;	
// 	struct stat statbuff;
	
// 	if(stat(pfile_path, &statbuff) < 0)
// 	{
// 		return filesize;
// 	}
// 	else
// 	{
// 		filesize = statbuff.st_size;
// 	}
	
// 	return filesize;
// }
//显示正常jpg图片
int lcd_draw_jpg1(unsigned int x,unsigned int y,const char *pjpg_path)  
{
	/*定义解码对象，错误处理对象*/
	struct 	jpeg_decompress_struct 	cinfo;
	struct 	jpeg_error_mgr 			jerr;	
	
	unsigned char 	*pcolor_buf = g_color_buf;
	char 	*pjpg;
	
	unsigned int 	i=0;
	unsigned int	color =0;
	//unsigned int	count =0;
	
	unsigned int 	x_s = x;
	unsigned int 	x_e ;	
	unsigned int 	y_e ;
	unsigned int	y_n	= y;
	unsigned int	x_n	= x;
	
			 int	jpg_fd;
	unsigned int 	jpg_size;

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
		if(jpg_size<3000)
			return -1;
		
		/* 为jpg文件申请内存空间 */	
		pjpg = malloc(jpg_size);

		/* 读取jpg文件所有内容到内存 */		
		read(jpg_fd,pjpg,jpg_size);
	}
	else
		return -1;

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
	
	
	x_e	= x_s +cinfo.output_width;
	y_e	= y  +cinfo.output_height;	

	/*读解码数据*/
	while(cinfo.output_scanline < cinfo.output_height )
	{		
		pcolor_buf = g_color_buf;
		
		/* 读取jpg一行的rgb值 */
		jpeg_read_scanlines(&cinfo,&pcolor_buf,1);
		
		for(i=0; i<cinfo.output_width; i++)
		{	
			/* 获取rgb值 */
			color = 		*(pcolor_buf+2);
			color = color | *(pcolor_buf+1)<<8;
			color = color | *(pcolor_buf)<<16;	
			
			/* 显示像素点 */
			lcd_draw_point(x_n,y_n,color);
			
			pcolor_buf +=3;
			
			x_n++;
		}
		
		/* 换行 */
		y_n++;			
		
		x_n = x_s;		
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

	return 0;
}





// static unsigned char g_color_buf[FB_SIZE]={0};

// int ts_x, ts_y;
// int flag = 1;

// int  lcd_fd;
// int *mmap_fd;
// struct jpg_data video_buf;//定义结构体变量


void *start_routine(void *arg)
{
	while(flag)
	{
		read_ts(&ts_x, &ts_y);
		printf("==(x,y):(%d, %d)\n", ts_x, ts_y);
	}	
}

int my_camera(void)
{
	printf("%s\n","111111111111111111" );

	int fd_jpg = -1;
	int cnt = 0,cut=0;
	char path[21] = {};
	char took[][20] = {};
	lcd_open();//打开LCD屏幕	
	mmap_lcd();//创建映射关系，将DDR3中存储的摄像头采集的数据映射到LCD屏幕上显示。
	
	pthread_t ts_thread;
	pthread_create(&ts_thread, NULL, start_routine, NULL);
	
	linux_v4l2_yuyv_init("/dev/video7");//初始化摄像头
	linux_v4l2_start_yuyv_capturing();//开启摄像头捕捉
	
	// int x,y;
	while(1) 
	{
		// touch(&x,&y)
		if(ts_x>660 && ts_x<790 && ts_y>10 && ts_y<60)//实时监控
		{
			printf("%s\n","show_video" );
			linux_v4l2_get_yuyv_data(&video_buf);//获取摄像头捕捉的画面	
			show_video_data(0, 0, video_buf.jpg_data, video_buf.jpg_size);//显示摄像头捕捉的画面
		}
		
		if(640<ts_x&&ts_x<790 && 70<ts_y&&ts_y<120) /*录像*/
		{
			printf("%s\n","lu_xiang" );	
			linux_v4l2_get_yuyv_data(&video_buf);//获取摄像头捕捉的画面
			show_video_data(0, 0, video_buf.jpg_data, video_buf.jpg_size);//显示摄像头捕捉的画面

			//printf("%s\n","6666666" );

			sprintf(path, "image/video/%d.jpg", cnt);

			//printf("%s\n","77777777" );

			fd_jpg = open(path, O_RDWR|O_CREAT|O_TRUNC);

			//printf("%s\n","wuwuwuuw" );

			if(fd_jpg == -1)
			{
				printf("creat err\n");
				printf("video err\n");
			}
			cnt++;
			//printf("%s\n","hahahahhahahahahah" );
			write(fd_jpg, video_buf.jpg_data, video_buf.jpg_size);
			close(fd_jpg);
			

			
			usleep(200000);
		}


			/*for (; cnt<21; cnt++)
			{
				sprintf(path[cnt], "image/video/0%d.jpg", cnt);
				
				fd_jpg = open(path[cnt], O_RDWR|O_CREAT|O_TRUNC);
				
				write(fd_jpg, video_buf.jpg_data, video_buf.jpg_size);
				close(fd_jpg);
				usleep(200000);
			}*/
			// cnt=0;


		

		if(640<ts_x&&ts_x<790 && 200<ts_y&&ts_y<260)  /*播放录制的图像,回放*/
		{
			printf("%s\n","hui_fang" );
			for (int i = 0; i < cnt; ++i)
			{
				sprintf(path, "image/video/%d.jpg", i);
				lcd_draw_jpg1(0,0,path);
			}
			ts_x=0;
			ts_y=0;
			continue;
		}

		if(640<ts_x&&ts_x<790 && 130<ts_y&&ts_y<185) /*抓拍*/
		{
			printf("%s\n", "zhua_pai");
			if (cut < 21)
			{
			linux_v4l2_get_yuyv_data(&video_buf);//获取摄像头捕捉的画面
			// show_video_data(0, 0, video_buf.jpg_data, video_buf.jpg_size);//显示摄像头捕捉的画面
		/*	sleep(1);*/
			sprintf(took[cut], "image/%d.jpg", cut);
			
			fd_jpg = open(took[cut], O_RDWR|O_CREAT|O_TRUNC);
			if(fd_jpg == -1)
			{
				printf("creat err\n");
				printf("video err\n");
			}
			write(fd_jpg, video_buf.jpg_data, video_buf.jpg_size);
			close(fd_jpg);
			cut++;
			}/*else
			{
				cut = 0;
			}*/
		}


		

			
		
		
		
		if(ts_x>640 && ts_x<790 && ts_y>400 && ts_y<460)//退出监控
		{
			printf("\n======quit_jiankong======\n");
			flag = 0;
			break;
		}
	}
	
	//关闭摄像头
	linux_v4l2_yuyv_quit();
	lcd_close();
	return 0;
}
