/*
    blink the q led
*/
#include "olduino.h"
void mysetqOn(){
	asm("	seq\n");
}
void main()
{
	unsigned char flippy=0;
	while(1){
		setqOn();
		delay(250);
		setqOff();
		delay(250);
		setqOn();
		delay(250);
		setqOff();
		delay(1000);
	}
}

#include "olduino.c" //for the delay routine
