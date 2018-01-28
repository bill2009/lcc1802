#include <olduino.h>
#include <nstdlib.h>
#include <cpu1802spd18ser.h>
void main(){
	unsigned char cin='?';
	asm(" seq\n"); //make sure Q is high to start
	//while(1){
		printf("U");
		delay(10);
		putcser(0x55);
		delay(100);
	//}
	while(1){
		printf("Hello From The Emma Side!\n");
		cin=getcser();
		printf("Thanks for the %cx\n",cin);
		delay(1000);
	}
}
void includeser(){
	asm(" include VELFserial2.inc");
}
#include <olduino.c>
#include <nstdlib.c>
