#ifndef _RCA_JOYSTICK_H
#define _RCA_JOYSTICK_H

//rca_keyboard_encoder header

#ifdef __COMX__
#define MOVE_UP 0x82
#define MOVE_RIGHT 0x83
#define MOVE_LEFT 0x84
#define MOVE_DOWN 0x85
#define MOVE_FIRE 0x5f
#endif

#ifdef __PECOM__
#define MOVE_UP 0x5e
#define MOVE_RIGHT 0x5d
#define MOVE_LEFT 0x5c
#define MOVE_DOWN 0x5b
#define MOVE_FIRE 0x40
#endif

#ifdef __TMC600__
#define MOVE_UP 0xb
#define MOVE_RIGHT 0x9
#define MOVE_LEFT 0x8
#define MOVE_DOWN 0xa
#define MOVE_FIRE 0x20
#endif

#ifdef __CIDELSA__
#define MOVE_UP 0x2
#define MOVE_RIGHT 0x8
#define MOVE_LEFT 0x10	
#define MOVE_DOWN 0x4
#define MOVE_FIRE 0x20
#endif

unsigned char get_stick();
unsigned char get_trigger();

void rca_joystick_includer(){
asm(" include comx/rca_joystick.inc\n");
}

#endif // _RCA_JOYSTICK_H
