#include "olduino.h"
#define initleds() 	asm(" req\n seq\n dec 2\n out 7\n req\n")

unsigned char banner[]={15,15,15,15,5,0,8,1,15};
void main()
{
	initleds();
	out(7,2);
	out(7,4);
	out(7,15);out(7,15);out(7,15);out(7,15);out(7,15);out(7,15);
	while(1);
}

#include "olduino.c" //for the delay routine
