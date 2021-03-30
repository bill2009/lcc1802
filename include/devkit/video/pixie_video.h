#ifndef _PIXIE_VIDEO_H
#define _PIXIE_VIDEO_H

//1861_video header

#include "stdint.h"
#include "devkit/system/flags.h"

#define PIXIE_SYNC 0x8000

uint16_t pixie_wait_vsync;
uint16_t pixie_video_mem;
uint32_t pixie_sprite_data;

void initvideo();
void vidclr();
#ifndef PIXIE_NO_VSYNC
void flags_pixie_vsync_includer(){
asm("PIXIE_VSYNC equ 1\n");
}
#endif
#if defined PIXIE_SPRITE
#define X_SIZE 64
#define Y_SIZE RES
void flags_pixie_sprite_includer(){
asm("PIXIE_SPRITE equ 1\n");
}
uint8_t drawsprite(uint8_t x, uint8_t y, const uint8_t * spriteshape);
#endif
#if defined PIXIE_MOVE
#define X_SIZE 64
#define Y_SIZE RES
void flags_pixie_sprite_includer(){
asm("PIXIE_MOVE equ 1\n");
}
uint8_t showsprite(uint32_t * spritedata, const uint8_t * spriteshape, uint8_t x, uint8_t y);
uint8_t movesprite(uint32_t * spritedata, uint8_t direction);
uint8_t movexysprite(uint32_t * spritedata, uint8_t x, uint8_t y);
void removesprite(uint32_t * spritedata);
#endif
#if defined PIXIE_PATTERN
#define X_SIZE 64
#define Y_SIZE RES
void flags_pixie_sprite_includer(){
asm("PIXIE_PATTERN equ 1\n");
}
uint8_t showsprite(uint32_t * spritedata, const uint8_t * spriteshape, uint8_t x, uint8_t y);
uint8_t movesprite(uint32_t * spritedata, uint8_t direction);
uint8_t movexysprite(uint32_t * spritedata, uint8_t x, uint8_t y);
void removesprite(uint32_t * spritedata);
#endif
#if defined PIXIE_TILE
#define X_SIZE 16
#define Y_SIZE 8
void flags_pixie_tile_includer(){
asm("PIXIE_TILE equ 1\n");
}
void drawtile(uint8_t x, uint8_t y, const uint8_t * spriteshape);
#endif
#if defined PIXIE_TEXT
void flags_pixie_text_includer(){
asm("PIXIE_TEXT equ 1\n");
}
void vidstrcpyxy(uint8_t x, uint8_t y, char * text);
void vidcharxy(uint8_t x, uint8_t y, uint8_t character);
#endif
#if defined PIXIE_TEXT96
void flags_pixie_text96_includer(){
asm("PIXIE_TEXT96 equ 1\n");
}
#endif
#if defined PIXIE_CHECK_BORDER
void flags_pixie_check_border_includer(){
asm("PIXIE_CHECK_BORDER equ 1\n");
}
#endif

void pixie_video_includer(){
asm(" VIDMEM: equ "xstr(VIDMEM)"\n"
	" include devkit/video/pixie_video.inc\n");
}

#endif // _PIXIE_VIDEO_H
