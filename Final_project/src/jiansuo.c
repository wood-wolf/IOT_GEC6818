#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include "jiansuo.h"


int jiansuo(char *a)
{
	DIR *dir;
	dir=opendir("./");//当前路径下

	struct dirent *fp=NULL;

	int i=0;
	
	while(1)
	{	
		fp=readdir(dir);

		if (fp==NULL)
		{
			break;
		}
		
		if(strstr(fp->d_name,a))  //文件类型
		{

			// printf("%s\n",strcpy(*(buf+i),fp->d_name));
			strcpy(*(buf+i),fp->d_name);
			i++;
		}

		if (strcmp(fp->d_name,".")==0||strcmp(fp->d_name,"..")==0)
		{
			continue;
		}
		if(fp->d_name==0)
		{
			break;
		}


	}
	closedir(dir);
	return 0;
}