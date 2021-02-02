#ifndef _STRLEN_H
#define _STRLEN_H

//strlen header

#include "stdint.h"

uint16_t strlen1802(uint8_t *str);
void strlen_includer(){
asm(" include devkit/system/strlen.inc\n");
}

#endif // _STRLEN_H
