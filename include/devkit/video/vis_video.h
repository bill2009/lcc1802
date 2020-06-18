#ifndef _VIS_VIDEO_H
#define _VIS_VIDEO_H

//vis_video header

#define COLOR_BLACK 0
#define COLOR_GREEN 1	
#define COLOR_BLUE 2
#define COLOR_CYAN 3     
#define COLOR_RED 4
#define COLOR_YELLOW 5
#define COLOR_MAGENTA 6
#define COLOR_WHITE 7  

#define CHAR_SET_0 3
#define CHAR_SET_1 1
#define CHAR_SET_2 2
#define CHAR_SET_3 0

#include "devkit/video/vis.h"

unsigned char vis_text_color;
unsigned char vis_text_color_mask;
unsigned long vis_buffer;
#if defined __CIDELSA__ || defined __MICRO__
unsigned char vis_out3;
unsigned char vis_out5;
#endif

void initvideo();
void setvideobase(unsigned short int vidmem);
void vidclr(unsigned int vidmem, int vidlen);
void vidchar(unsigned short int vidmem, unsigned char character);
void vidstrcpy(unsigned short int vidmem, char * text);
unsigned char bgcolor(unsigned char color);
void textcolor(unsigned char color);
#ifdef __TMC600__
void setcolor(unsigned int colormem, unsigned char color);
#else
#ifndef __NTSC5_6_7__
void shapechar(const unsigned char * shapelocation, unsigned short int lines_character, unsigned short int color_number);
void shapecolor(unsigned short int character, unsigned char number, unsigned char color);
#endif
void textcolordefinition(unsigned char definition);
void monochrome(unsigned char mono);
#endif
void vis_video_includer(){
asm(" include devkit/video/vis_video.inc\n");
}

#endif // _VIS_VIDEO_H
