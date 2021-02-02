#ifndef _JOYSTICK_H
#define _JOYSTICK_H

//joystick header

#include "stdint.h"
#include "devkit/system/flags.h"

#ifdef __COMX__
#define MOVE_UP 0x82
#define MOVE_RIGHT 0x83
#define MOVE_LEFT 0x84
#define MOVE_DOWN 0x85
#define MOVE_FIRE 0x5f
#endif

#ifdef __MICRO__
#define MOVE_UP 0x3b
#define MOVE_RIGHT 0x3c
#define MOVE_LEFT 0x3a
#define MOVE_DOWN 0x3d
#define MOVE_FIRE 0x20
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
#ifdef __ALTAIR__
#define MOVE_UP 0x1
#define MOVE_RIGHT 0x20
#define MOVE_LEFT 0x40    
#define MOVE_DOWN 0x2
#define MOVE_FIRE 0x80
#define MOVE_FIRE2 0x4
#define MOVE_B1 0x8
#define MOVE_B2 0x10
#endif
#ifdef __DESTROYER__
#define MOVE_UP 0x2
#define MOVE_RIGHT 0x8
#define MOVE_LEFT 0x10    
#define MOVE_DOWN 0x4
#define MOVE_FIRE 0x20
#define MOVE_B1 0x2
#define MOVE_B2 0x4
#endif
#ifdef __DRACO__
#define MOVE_UP 0x10
#define MOVE_RIGHT 0x40
#define MOVE_LEFT 0x80    
#define MOVE_DOWN 0x20
#define MOVE_FIRE 0x8
#define MOVE_B1 0x1
#define MOVE_B2 0x2
#define MOVE_TILT 0x4
#endif
#endif

uint8_t get_stick();

void joystick_includer(){
asm(" include devkit/input/joystick.inc\n");
}

#endif // _JOYSTICK_H
