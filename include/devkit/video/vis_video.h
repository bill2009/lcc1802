#ifndef _VIS_VIDEO_H
#define _VIS_VIDEO_H

//vis_video header

#include "devkit/system/flags.h"

unsigned char vis_text_color;
unsigned char vis_text_color_mask;
unsigned long vis_buffer;
#if defined __CIDELSA__ || defined __MICRO__
unsigned char vis_out3;
unsigned char vis_out5;
#endif

#include "devkit/video/vis.h"

void initvideo();
void setvideobase(unsigned short int vidmem);
void vidclr(unsigned int vidmem, int vidlen);
void vidchar(unsigned short int vidmem, unsigned char character);
void vidcharxy(unsigned char x, unsigned char y, unsigned char character);
void vidstrcpy(unsigned short int vidmem, char * text);
void vidstrcpyxy(unsigned char x, unsigned char y, char * text);
unsigned char bgcolor(unsigned char color);
void textcolor(unsigned char color);
#ifdef __TMC600__
void setcolor(unsigned int colormem, unsigned char color);
#define shapechar(shapelocation, lines_character, color_number)
#define shapecolor(character, number, color)
#define textcolordefinition(definition)
#define monochrome(mono)
#else
#define setcolor(colormem, color)
#if NTSC!=5 && NTSC!=6 && NTSC!=7
void shapechar(const unsigned char * shapelocation, unsigned short int lines_character, unsigned short int color_number);
void shapecolor(unsigned short int character, unsigned char number, unsigned char color);
#else
#define shapechar(shapelocation, lines_character, color_number)
#define shapecolor(character, number, color)
#endif
void textcolordefinition(unsigned char definition);
void monochrome(unsigned char mono);
#endif

void vis_video_includer(){
asm(" include devkit/video/vis_video.inc\n");
}

#endif // _VIS_VIDEO_H
