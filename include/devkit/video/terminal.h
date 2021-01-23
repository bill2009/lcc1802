#ifndef _TERMINAL_H
#define _TERMINAL_H

//terminal header

#include "devkit/system/flags.h"

#if defined __ELF2K__
void putchar_term(unsigned char character);
unsigned char getchar_term();
#endif

void terminal_includer(){
asm(" include devkit/video/terminal.inc\n");
}

#endif // _TERMINAL_H
