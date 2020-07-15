#include "password.h"

void password()
{
	int i,j=0,k,t;
	int x,y;
	int password[20];
	int pass[4]={1,2,3,4};

	show_jpeg("land.jpg");
	while(1)
	{
		touch(&x,&y);
		if(j<4&&370<x&&x<430&&360<y&&y<420)
		{
			k= 160+j*80;
			lcd_draw_jpg(k,60,"00.jpg",NULL,0,0);

			i=0;
			password[j]=i;
			printf("password:%d \n",password[j] );

			j++;

			usleep(200000);
			continue;
		}//0
		else if(j<4&&300<x&&x<360&&300<y&&y<355)
		{
			k= 160+j*80;
			lcd_draw_jpg(k,60,"01.jpg",NULL,0,0);

			i=1;
			password[j]=i;
			printf("password:%d \n",password[j] );

			j++;
			usleep(200000);
			continue;
		}//1
		else if(j<4&&370<x&&x<430&&300<y&&y<355)
		{
			k= 160+j*80;
			lcd_draw_jpg(k,60,"02.jpg",NULL,0,0);

			i=2;
			password[j]=i;
			printf("password:%d \n",password[j] );

			j++;
			usleep(200000);
			continue;
		}//2
		else if(j<4&&440<x&&x<500&&300<y&&y<355)
		{
			k= 160+j*80;
			lcd_draw_jpg(k,60,"03.jpg",NULL,0,0);

			i=3;
			password[j]=i;
			printf("password:%d \n",password[j] );

			j++;
			usleep(200000);
			continue;
		}//3
		else if(j<4&&300<x&&x<360&&230<y&&y<290)
		{
			k= 160+j*80;
			lcd_draw_jpg(k,60,"04.jpg",NULL,0,0);

			i=4;
			password[j]=i;
			printf("password:%d \n",password[j] );

			j++;
			usleep(200000);
			continue;
		}//4
		else if(j<4&&370<x&&x<430&&230<y&&y<290)
		{
			k= 160+j*80;
			lcd_draw_jpg(k,60,"05.jpg",NULL,0,0);

			i=5;
			password[j]=i;
			printf("password:%d \n",password[j] );

			j++;
			usleep(200000);
			continue;
		}//5
		else if(j<4&&440<x&&x<500&&230<y&&y<290)
		{
			k= 180+j*80;
			lcd_draw_jpg(k,60,"06.jpg",NULL,0,0);

			i=6;
			password[j]=i;
			printf("password:%d \n",password[j] );

			j++;
			usleep(200000);
			continue;
		}//6
		else if(j<4&&300<x&&x<360&&160<y&&y<220)
		{
			k= 160+j*80;
			lcd_draw_jpg(k,60,"07.jpg",NULL,0,0);

			i=7;
			password[j]=i;
			printf("password:%d \n",password[j] );

			j++;
			usleep(200000);
			continue;
		}//7
		else if(j<4&&370<x&&x<430&&160<y&&y<220)
		{
			k= 160+j*80;
			lcd_draw_jpg(k,60,"08.jpg",NULL,0,0);

			i=8;
			password[j]=i;
			printf("password:%d \n",password[j] );

			j++;
			usleep(200000);
			continue;
		}//8
		else if(j<4&&440<x&&x<500&&160<y&&y<220)
		{
			k= 160+j*80;
			lcd_draw_jpg(k,60,"09.jpg",NULL,0,0);

			i=8;
			password[j]=i;
			printf("password:%d \n",password[j] );

			j++;
			usleep(500000);
			continue;
		}//9
		else if(440<x&&x<500&&360<y&&y<420)
		{
			if(password[0]==pass[0]&&password[1]==pass[1]&&password[2]==pass[2]&&password[3]==pass[3])
			{
				break;
			}
		}//打勾进行匹配
		else if(300<x&&x<360&&360<y&&y<420)
		{
			lcd_draw_jpg(120,60,"blank.jpg",NULL,0,0);
			j=0;
			continue;
		}//打叉消除
	}

}