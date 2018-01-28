#include <olduino.h>
#include <nstdlib.h>
#include <cpu1802spd18ser.h>
void main(){
	unsigned char cin='?';
	asm(" seq\n"); //make sure Q is high to start
	while(1){
		//putceagleser(0x55);
		//putceagleser(0x0a);
		printf("Hello From The Emma Side!\n");
		cin=getcser();
		printf("Thanks for the %cx\n",cin);
		delay(1000);
	}
}
void eagleser(){
	asm(" include eagleserial.inc");
}
#include <olduino.c>
#include <nstdlib.c>
