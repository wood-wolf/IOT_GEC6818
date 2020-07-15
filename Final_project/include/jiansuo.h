#ifndef _JIANSUO_H_
#define _JIANSUO_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <stdio.h>

char buf[100][30];
extern int jiansuo(char *a);

#endif