/*
    blink the q led
*/
#include "olduino.h"
#include "nstdlib.h"
void mysetqOn(){
	asm("	seq\n");
}
void main()
{
	while(1){
		mysetqOn();
		delay(500);
		setqOff();
		delay(500);
	}
}

#include "olduino.c" //for the delay routine

