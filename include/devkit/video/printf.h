#ifndef _PRINTF_H
#define _PRINTF_H

//printf header

#include "devkit/system/flags.h"
#include "devkit/video/vis.h"

void putlcccx(uint8_t c);
void gotoxy(uint8_t x, uint8_t y);
void printf_includer(){
asm(" include devkit/video/printf.inc\n");
}

#define putc(c) putlcccx(c)

#endif // __PRINTF_H
