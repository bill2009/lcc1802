#ifndef _RCA_KEYBOARD_ENCODER_H
#define _RCA_KEYBOARD_ENCODER_H
 
//rca_keyboard_encoder header
#ifdef __CIDELSA__
#include "comx/rca_joystick.h"
#define kbhit() get_stick();  
#else
unsigned char cgetc();
int kbhit();  
#endif

void rca_keyboard_encoder_includer(){
asm(" include comx/rca_keyboard_encoder.inc\n");
}

#endif // _RCA_KEYBOARD_ENCODER_H
