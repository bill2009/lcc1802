#ifndef _RAND_H
#define _RAND_H

//rand header

#include "stdint.h"

uint16_t seedvalue;

uint16_t rand();
void rand_includer(){
asm(" include devkit/system/rand.inc\n");
}

#endif // _RAND_H
