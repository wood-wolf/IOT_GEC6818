#include <stdio.h>
#include <stdlib.h>

#include "jpeg.h"//图片显示
#include "touch.h"//触摸，获取坐标
#include "madplay.h"//音乐
#include "photo.h"//相册功能
#include "password.h"//密码锁功能
#include "vedio.h"
#include <sys/types.h>         //socket
#include <sys/socket.h>			//socket
#include "camera.h"

#include "Jack.h"//zigbee

int main(int argc, char const *argv[])
{
	// // 1，创建一个TCP套接字
	// int fd = socket(AF_INET, SOCK_STREAM, 0);

	// // 2，定义一个带Rose地址（IP + PORT）的结构体
	// struct sockaddr_in srvaddr;
	// bzero(&srvaddr, sizeof(srvaddr));

	// srvaddr.sin_family = AF_INET;
	// srvaddr.sin_port = htons(50012);
	// inet_pton(AF_INET, "192.168.15.64", &srvaddr.sin_addr);

	// // 3，向Rose发起连接请求
	// connect(fd, (struct sockaddr *)&srvaddr, sizeof(srvaddr));

	// loop:

	password();//密码锁
	
	show_jpeg("welcome.jpg");
	sleep(3);
	system("madplay welcome.mp3 &");//欢迎
	
	show_jpeg("master.jpg");//主界面	
	while(1)
	{
		int x,y;//触摸坐标
		touch(&x,&y);
		show_jpeg("master.jpg");
		if (x>50&&x<200&&y>200&&y<400)//音乐功能
		{
			// show_jpeg("music.jpg");
			music();
			usleep(500000);
		}

		if (x>260&&x<360&&y>300&&y<400)//相册功能
		{
			// show_jpeg("2.jpg");
			photo();
			usleep(500000);
		}
		if (x>425&&x<555&&y>300&&y<400)//视频功能
		{
			vedio();
			usleep(500000);
		}
		if (x>600&&x<730&&y>300&&y<400)//录像功能
		{
			printf("%s\n","in camera" );
			open_ts();
			show_jpeg("camera.jpg");
			my_camera();
			close_ts();
			printf("%s\n","out camera" );
			usleep(500000);
		}
		if (x>600&&x<720&&y>160&&y<240)//Zigbee
		{
			show_jpeg("zigbee.jpg");
			zigbee();
			usleep(500000);
		}
		if (x>0&&x<50&&y>0&&y<50)
		{
			break;//杀死进程
		}
	}
	system("killall -9 madplay");

	return 0;
}