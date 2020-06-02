#ifndef _RCA_KEYBOARD_ENCODER_H
#define _RCA_KEYBOARD_ENCODER_H

#include <comx/rca_global.h>

//rca_keyboard_encoder header
unsigned char getkey();
unsigned char cgetc();
int kbhit();  

void rca_keyboard_encoder_includer(){
asm(" include comx/rca_keyboard_encoder.inc\n");
}

#endif // _RCA_KEYBOARD_ENCODER_H
