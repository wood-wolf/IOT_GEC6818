#ifndef _TOUCH_H
#define _TOUCH_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <stdio.h>

extern int touch(int *x,int *y);

#endif