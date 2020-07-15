#ifndef _JPEG_H
#define _JPEG_H

extern unsigned long file_size_get(const char *pfile_path);
extern int lcd_draw_jpg(unsigned int x,unsigned int y,const char *pjpg_path,char *pjpg_buf,unsigned int jpg_buf_size,unsigned int jpg_half);
extern void show_jpeg(char *pathname);

#endif