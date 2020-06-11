#ifndef _RAND_H
#define _RAND_H

unsigned short int seedvalue;

//rand header

unsigned short int rand();
void rand_includer(){
asm(" include devkit/system/rand.inc\n");
}

#endif // _RAND_H
