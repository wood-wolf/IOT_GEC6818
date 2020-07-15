// Jack.c

#include <sys/types.h>
#include <sys/socket.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "touch.h"

#include "Jack.h"

#define SERIAL "/dev/ttySAC2"


int zigbee() // ./Jack  192.168.1.8  50001 
{
	int x,y;//触摸点

	printf("11111111111111\n");
	// 1，创建一个TCP套接字
	int fd_serial = socket(AF_INET, SOCK_STREAM, 0);

	// 2，定义一个带Rose地址（IP + PORT）的结构体
	struct sockaddr_in srvaddr;
	bzero(&srvaddr, sizeof(srvaddr));

	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(50012);
	inet_pton(AF_INET, "192.168.15.64", &srvaddr.sin_addr);

	// 3，向Rose发起连接请求
	connect(fd_serial, (struct sockaddr *)&srvaddr, sizeof(srvaddr));

	

	// 4，向对方发送数据
	
	// char msg[50];
	int init_fd;
	char init_sbuf[20];
	int srceen_fd = -1;
	

	//1.打开串口
	init_fd = open(SERIAL, O_RDWR|O_NOCTTY);//zigbee的接口描述符
	if (init_fd == -1)
	{
		printf("chuankouerro!\n");
		//return -1;
	}
	struct termios new_cfg, old_cfg;
	bzero(&new_cfg, sizeof(new_cfg));
	bzero(&old_cfg, sizeof(old_cfg));

	//1.保存原有配置
	tcgetattr(init_fd, &old_cfg);
	
	//2.激活选项
	new_cfg.c_cflag |= CLOCAL|CREAD;
	
	//3.设置原始模式和波特率
	cfmakeraw(&new_cfg);
	cfsetispeed(&new_cfg, B115200);
	cfsetospeed(&new_cfg, B115200);
	
	//4.设置字符大小
	new_cfg.c_cflag &= ~CSIZE;
	new_cfg.c_cflag |= CS8;
	
	//5.设置奇偶校验
	new_cfg.c_cflag &=  ~PARENB;//无校验
	
	//6.设置停止位
	new_cfg.c_cflag &= ~CSTOPB;//设置一个停止位
	
	//7.设置最小字符数和等待时间
	new_cfg.c_cc[VTIME] = 0;
	new_cfg.c_cc[VMIN] = 0;
	
	//8.清除串口缓冲
	tcflush(init_fd, TCIOFLUSH);
	
	//9.激活配置
	tcsetattr(init_fd, TCSANOW, &new_cfg);

	while(1)	
	{
		touch(&x,&y);
		if (x>0&&x<120&&y>0&&y<80)
		{
			break;
		}else
		{
		for(int n = 0;n<10;++n)
			{
				bzero(init_sbuf, sizeof(init_sbuf));
				read(init_fd,init_sbuf,sizeof(init_sbuf));
				printf("sbuf = %s\n", init_sbuf);

				write(fd_serial, init_sbuf, strlen(init_sbuf));
				sleep(1);

				//touch(&x,&y);
			}
		}
	}
	printf("20\n");
	
	//close(fd_serial);
	//pause();
	return 0;
}
