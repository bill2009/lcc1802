#ifndef _PIXIE_VIDEO_H
#define _PIXIE_VIDEO_H

//1861_video header

#include "stdint.h"
#include "devkit/system/flags.h"

void initvideo();
void drawsprite(uint8_t x, uint8_t y, const uint8_t * spritedata);
void drawtile(uint8_t x, uint8_t y, const uint8_t * spritedata);
/*void vidclr(uint16_t vidmem, int vidlen);
void vidchar(uint16_t vidmem, uint8_t character);
void vidcharxy(uint8_t x, uint8_t y, uint8_t character);
void vidstrcpy(uint16_t vidmem, char * text);
void vidstrcpyxy(uint8_t x, uint8_t y, char * text);
*/
#define xstr(s) str(s)
#define str(s) #s

void pixie_video_includer(){
asm(" VIDMEM: equ "xstr(VIDMEM)"\n"
	" include devkit/video/pixie_video.inc\n");
}

#endif // _PIXIE_VIDEO_H
