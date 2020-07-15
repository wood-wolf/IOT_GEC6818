#include "slave.h"
int w_slave(char *slave_name)
{
	 
	if(access("/tmp/fifo",F_OK))
	{
		int fifo=mkfifo("/tmp/fifo",0644);
		if (fifo<0)
		{
			perror("mkfifo()");
		}
	}
	
	int  fd =open("/tmp/fifo",O_RDWR);
	if(fd<0)
	{
		perror("open()");
	}
	
	write(fd,slave_name,strlen(slave_name));

	close(fd);
	return 0;
}//管道用来控制视频的