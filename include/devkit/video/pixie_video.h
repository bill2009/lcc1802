#ifndef _PIXIE_VIDEO_H
#define _PIXIE_VIDEO_H

//1861_video header

#include "stdint.h"
#include "devkit/system/flags.h"

void initvideo();
void vidclr();
#if defined PIXIE_SPRITE
#define X_SIZE 64
#define Y_SIZE RES
void flags_pixie_sprite_includer(){
asm("PIXIE_SPRITE equ 1\n");
}
void drawsprite(uint8_t x, uint8_t y, const uint8_t * spritedata);
#endif
#if defined PIXIE_TILE
#define X_SIZE 16
#define Y_SIZE 8
void flags_pixie_tile_includer(){
asm("PIXIE_TILE equ 1\n");
}
void drawtile(uint8_t x, uint8_t y, const uint8_t * spritedata);
#endif

#define xstr(s) str(s)
#define str(s) #s

void pixie_video_includer(){
asm(" VIDMEM: equ "xstr(VIDMEM)"\n"
	" include devkit/video/pixie_video.inc\n");
}

#endif // _PIXIE_VIDEO_H
