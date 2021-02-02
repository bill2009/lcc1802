#ifndef _VIS_VIDEO_H
#define _VIS_VIDEO_H

//vis_video header

#include "devkit/system/flags.h"
#include "devkit/video/vis.h"

uint8_t vis_text_color;
uint8_t vis_text_color_mask;
uint32_t vis_buffer;
#if defined __CIDELSA__ || defined __MICRO__
uint8_t vis_out3;
uint8_t vis_out5;
#endif

void initvideo();
void setvideobase(uint16_t vidmem);
void vidclr(uint16_t vidmem, int vidlen);
void vidchar(uint16_t vidmem, uint8_t character);
void vidcharxy(uint8_t x, uint8_t y, uint8_t character);
void vidstrcpy(uint16_t vidmem, char * text);
void vidstrcpyxy(uint8_t x, uint8_t y, char * text);
uint8_t bgcolor(uint8_t color);
void textcolor(uint8_t color);
#ifdef __TMC600__
void setcolor(uint16_t colormem, uint8_t color);
#define shapechar(shapelocation, lines_character, color_number)
#define shapecolor(character, number, color)
#define textcolordefinition(definition)
#define monochrome(mono)
#else
#define setcolor(colormem, color)
#if NTSC!=5 && NTSC!=6 && NTSC!=7
void shapechar(const uint8_t * shapelocation, uint16_t lines_character, uint16_t color_number);
void shapecolor(uint16_t character, uint8_t number, uint8_t color);
#else
#define shapechar(shapelocation, lines_character, color_number)
#define shapecolor(character, number, color)
#endif
void textcolordefinition(uint8_t definition);
void monochrome(uint8_t mono);
#endif

void vis_video_includer(){
asm(" include devkit/video/vis_video.inc\n");
}

#endif // _VIS_VIDEO_H
