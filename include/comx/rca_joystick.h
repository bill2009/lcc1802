#ifndef _RCA_JOYSTICK_H
#define _RCA_JOYSTICK_H

//rca_keyboard_encoder header
unsigned char getkey();
unsigned char cgetc();
int kbhit();  

void rca_keyboard_encoder_includer(){
asm(" include comx/rca_joystick.inc\n");
}

#endif // _RCA_JOYSTICK_H
