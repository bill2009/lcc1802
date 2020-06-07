#ifndef _RCA_JOYSTICK_H
#define _RCA_JOYSTICK_H

//rca_keyboard_encoder header

unsigned char get_stick();
unsigned char get_trigger();

void rca_joystick_includer(){
asm(" include comx/rca_joystick.inc\n");
}

#endif // _RCA_JOYSTICK_H
