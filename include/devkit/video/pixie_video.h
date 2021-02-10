#ifndef _PIXIE_VIDEO_H
#define _PIXIE_VIDEO_H

//1861_video header

#include "stdint.h"
#include "devkit/system/flags.h"

#define X_SIZE 16
#define Y_SIZE 8

void initvideo();
void drawsprite(uint8_t x, uint8_t y, const uint8_t * spritedata);
void drawtile(uint8_t x, uint8_t y, const uint8_t * spritedata);
void vidclr();

#define xstr(s) str(s)
#define str(s) #s

void pixie_video_includer(){
asm(" VIDMEM: equ "xstr(VIDMEM)"\n"
	" include devkit/video/pixie_video.inc\n");
}

#endif // _PIXIE_VIDEO_H
