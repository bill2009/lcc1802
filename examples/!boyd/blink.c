#include "olduino.h"
unsigned char test[]={15,15,15,15,5,0,8,1};
void main()
{
	asm(" req\n seq\n"
		" dec 2\n out 7\n req\n"
		" ldad r11,_test\n"
		" ldad r10,8\n"
		" sex 11\n"
		" out 7\n"
		" out 7\n"
		" out 7\n"
		" out 7\n"
		" out 7\n"
		" out 7\n"
		" out 7\n"
		" out 7\n"
		" br $\n");
}

//#include "olduino.c" //for the delay routine
