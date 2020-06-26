#ifndef _PRINTF_H
#define _PRINTF_H

//printf header

#include "devkit/system/flags.h"
#include "devkit/video/vis.h"

void putlcccx(unsigned char c);
void gotoxy(unsigned char x, unsigned char y);
void printf_includer(){
asm(" include devkit/video/printf.inc\n");
}

#define putc(c) putlcccx(c)

#endif // __PRINTF_H
