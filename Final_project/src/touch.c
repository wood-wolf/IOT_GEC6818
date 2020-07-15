#include "touch.h"

int touch(int *x,int *y)
{
	//创建结构体
	struct input_event buf;
	int a,b;
	//打开设备文件
	int cd;
	cd=open("/dev/input/event0",O_RDONLY);
	
	while(1)
	{
		
		//读取结构体
		read(cd,&buf,sizeof(buf));
		if(buf.type==EV_ABS && buf.code==ABS_X)
		{
			a=buf.value;
		}
		if(buf.type==EV_ABS && buf.code==ABS_Y)
		{
			b=buf.value;
			
		}
		if(buf.value==0)
		{
			*x=a;
			*y=b;
			break;
		}
		
	
	}
	close(cd);
	
}