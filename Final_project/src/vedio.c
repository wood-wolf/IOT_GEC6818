#include <stdio.h>

#include "vedio.h"
int vedio()
{
	
	int x,y;
	show_jpeg("music.jpg");
	system("mplayer -slave -input file=/tmp/fifo -geometry 0:0 -Zoom -x 800 -y 400 Faded3.avi &");
	lcd_draw_jpg(0,400,"vdi.jpg",NULL,0,0);
	while(1)
	{
		
		touch(&x,&y);
		if (y>400)
		{
			if (x>0&&x<200&&y<480&&y>400)
			{
				printf("%d,%d -10\n", x,y);
				w_slave("seek -10 \n");
				sleep(1);
			}else if(x>250&&x<550&&y<480&&y>400)
			{
				//w_slave("seek 0 \n");
				w_slave("pause \n");
				sleep(1);
				//system("mplayer -slave -input file=/tmp/fifo -geometry 0:0 -Zoom -x 800 -y 430 Faded3.avi &");
			}else if (x>600&&x<800&&y<480&&y>400)
			{
				printf("%d,%d 10\n", x,y);
				w_slave("seek 10 \n");
				sleep(1);
			}
			else if(x<670&&x>550&&y<480&&y>400)
			{
				printf("%d,%d\n",x,y );
				w_slave("volume 10\n");
			}
			else if(x<300&&x>180&&y<480&&y>400)
			{
				printf("%d,%d\n",x,y );
				w_slave("volume -10\n");
			}
		}

	}

	return 0;
}