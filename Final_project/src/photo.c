#include <stdio.h>

#include "jpeg.h"
#include "touch.h"

#include<sys/types.h>
#include<dirent.h>
#include<string.h>
#include<strings.h>

void photo()
{
	int x,y;
	int i = 0;
	char buf [100][20] = {};
	char buff[20];

	int count = 0;


	DIR *dir;//获取一个目录指针
	dir=opendir("./image/");
	struct dirent *fp=NULL;//目录信息结构体指针
	while(1)
	{
		fp=readdir(dir);//读取结构体(linux下的文件和文件夹)
		
		if (fp==NULL)
		{
			break;//读完退出
		}	
	
		
		if(strcmp(fp->d_name,".")==0||strcmp(fp->d_name,"..")==0)//strcmp比较两个字符串如果想等则返回0.
			continue;

		else
		{
			if (fp->d_type==8)
			{
				if(strstr(fp->d_name,".jpg"))
				{
					sprintf(buf[count],"./image/%s",fp->d_name);
					printf("%s\n",fp->d_name );
					// printf("%s\n",buf[count] );
					count++;
				}
			}
		}
	}


	printf("%s\n","1111" );
	// int num = ( sizeof(buf) / sizeof(int) )/ ( sizeof(buf[0]) / sizeof(int) );
	printf("%d\n",count );
	printf("1111111\n");
	// show_jpeg("welcome.jpg");
	printf("%s\n",buf );
	printf("%s\n","????" );
	while(1)
	{
		touch(&x,&y);
		printf("(%d,%d)\n",x,y );
		printf("<%d>\n",i );
		if (x>400)
		{
			if (i >= count)
			{
				i = 0;
				printf("7777\n");
				lcd_draw_jpg(0,0,buf[i],NULL,0,0);
				// show_jpeg(buf[i]);
				printf("888\n");
				usleep(200000);
			}else
			{
				i++;
				lcd_draw_jpg(0,0,buf[i],NULL,0,0);
				//show_jpeg(buf[i]);
				usleep(200000);
			}

		}else if(x>0&&x<=400)
		{
			if (i <= 0)
			{
				i = count;
				lcd_draw_jpg(0,0,buf[i],NULL,0,0);
				//show_jpeg(buf[i]);
				usleep(200000);
			}else
			{
				i--;
				lcd_draw_jpg(0,0,buf[i],NULL,0,0);
				//show_jpeg(buf[i]);
				usleep(200000);
			}
		}

		if (x>0&&x<100&&y>0&&y<100)
		{
			break;
		}

	}
}