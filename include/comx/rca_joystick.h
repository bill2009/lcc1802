#ifndef _RCA_JOYSTICK_H
#define _RCA_JOYSTICK_H

//rca_keyboard_encoder header

#ifdef __COMX__
#define JOY_UP 0x82
#define JOY_RIGHT 0x83
#define JOY_LEFT 0x84
#define JOY_DOWN 0x85
#define JOY_FIRE 0x5f
#endif

#ifdef __PECOM__
#define JOY_UP 0x5e
#define JOY_RIGHT 0x5d
#define JOY_LEFT 0x5c
#define JOY_DOWN 0x5b
#define JOY_FIRE 0x40
#endif

#ifdef __TMC600__
#define JOY_UP 0xb
#define JOY_RIGHT 0x9
#define JOY_LEFT 0x8
#define JOY_DOWN 0xa
#define JOY_FIRE 0x20
#endif

#ifdef __CIDELSA__
#define JOY_UP 0x8
#define JOY_RIGHT 0x4
#define JOY_LEFT 0x2	
#define JOY_DOWN 0x10
#define JOY_FIRE 0x20
#endif

unsigned char get_stick();
unsigned char get_trigger();

void rca_joystick_includer(){
asm(" include comx/rca_joystick.inc\n");
}

#endif // _RCA_JOYSTICK_H
