#include <stdio.h>
#include <stdlib.h>

#include "jpeg.h"
#include"madplay.h"
#include "touch.h"
/*
int main(int argc, char const *argv[])
{
	//int system(const char *command);
	char buf[5][20]={"1.mp3","2.mp3","3.mp3"};

	system("ls");
	system("madplay 3.mp3 &");
	show_bmp(buf[0]);
	sprintf()
	return 0;
}
*/
 
 void music()
 {
 	system("madplay 1.mp3 &");
 	show_jpeg("music.jpg");

 	int i=0;
 	// int k=0;
 	int x,y;
 	char buf[20];
 	char mus[3][8]={"1.mp3","2.mp3","3.mp3"};

 	// char buff[5][20]={"1.mp3","2.mp3","3.mp3"};
	// char buf[20];

 	while(1)
 	{	
 		touch(&x,&y);
 		
 	if(x>130&& x<220&&y>320&&y<410)//上一首
	{
		if(i==0)
		{
			i=2;
			sprintf(buf,"madplay %s &",mus[i]);
			printf("%s\n", buf);
			system("killall -9 madplay");
			system(buf);
		}
		else
		{
			--i;
			sprintf(buf,"madplay %s &",mus[i]);
			printf("%s\n", buf);
			system("killall -9 madplay");
			system(buf);
		}//上一首
	}
	else if(x>300 && x<400&&y>320&&y<410)
	{
		system("killall -18 madplay");//播放
	}
	else if(x>450 && x<530&&y>320&&y<410)
	{
		system("killall -19 madplay");//暂停
	}
	else if(x>600 && x<700&&y>320&&y<410)
	{
		if(i==2)
		{
			i=0;
			sprintf(buf,"madplay %s &",mus[i]);
			printf("%s\n", buf);
			system("killall -9 madplay");
			system(buf);
		}
		else
		{
			++i;
			sprintf(buf,"madplay %s &",mus[i]);
			printf("%s\n", buf);
			system("killall -9 madplay");
			system(buf);
		}	//下一首
	}
	else if (x>700&&x<755)
	{
		if (y>80&&y<135)
		{
			sprintf(buf,"madplay %s -a 0 &",mus[i]);
			system(buf);
			// madplay faded.mp3 -a 10；
			//音量++
		}
		if (y>170&&y<225)
		{
			sprintf(buf,"madplay %s -a -50 &",mus[i]);
			system(buf);
			//音量--
		}
	}
	else if(x>0 && x<50&&y>0&&y<50)
	{
		break;
	}

	// if (280<x&&x<420&&300<y&&y<430) //开始播放
	// {
		
	// 	sprintf(buf," madplay %s &",buff[k]);
	// 	printf("%s\n",buf );
	// 	system(buf);

	// }
	// else if (580<x&&x<720&&300<y&&y<430) //下一首
	// {

	// 	if (k<2&&0<=k)
	// 	{
	// 		k+=1;
	// 		sprintf(buf,"madplay %s &",buff[k]);
	// 		printf("%s\n",buf );
	// 		system(buf);
	// 	}
	// 	else
	// 	{
	// 		k=0;
	// 		sprintf(buf,"madplay %s &",buff[k]);
	// 		printf("%s\n",buf );
	// 		system(buf);
	// 	}
	
	// }
	// else if (110<x&&x<240&&300<y&&y<430) //上一首
	// {
	// 	if (k<=2&&0<k)
	// 	{
	// 		k-=1;
	// 		sprintf(buf,"madplay %s &",buff[k]);
	// 		printf("%s\n",buf );
	// 		system(buf);
	// 	}
	// 	else
	// 	{
	// 		k=2;
	// 		sprintf(buf,"madplay %s &",buff[k]);
	// 		printf("%s\n",buf );
	// 		system(buf);
	// 	}
	// }	
	
	// else if (430<x&&x<550&&300<y&&y<430)  //暂停
	// {
	// 	system("killall - 19 madplay");
	// }
	// else if (400<x&&x<800&&240<y&&y<480)  //继续
	// {
	// 	system("killall - 18 madplay");
	// }
	// else if (400<x&&x<800&&240<y&&y<480)  //停止
	// {
	// 	system("killall - 9 madplay");
	// }
		
 	}
 	system("killall -9 madplay");
 }
/* 
int main(int argc, char const *argv[])
{
	//char *a="123456";
	//char s[20];
	char buff[5][20]={"1.mp3","2.mp3","3.mp3"};


//	sprintf(s,">>>%s<<<",a);


	char buf[20];
	sprintf(buf,"madplay %s &",buff[2]);
	printf("%s\n",buf );
	system(buf);




	//"madplay 3.mp3 &"
	//printf("%s\n", s);
	return 0;
}*/