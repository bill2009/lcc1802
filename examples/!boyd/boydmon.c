#include "olduino.h"
#define initleds() 	asm(" req\n seq\n dec 2\n out 7\n req\n")
unsigned char ef1(){
	asm(" rldi 15,1\n"
		" bn1 .ret0\n"
		" cretn ;will return a '1'\n"
		".ret0: ;will drop thru to return 0\n");
	return 0; //if the assembly doesn't return, EF1 is not active
}
void disp12(){
	initleds();
	out(7,2);
	out(7,1);
	out(7,15);out(7,15);out(7,15);out(7,15);out(7,15);out(7,15);
}
void dispbl(){
	initleds();
	out(7,15);
	out(7,15);
	out(7,15);out(7,15);out(7,15);out(7,15);out(7,15);out(7,15);
}
void main()
{
	while(1){
		out(1,1); //activate top row of keys
		if(ef1()){ //key 12 pressed
			disp12();
		}else{
			dispbl();
		}
		delay(1000);
	}
	while(1);
}

#include "olduino.c" //for the delay routine
