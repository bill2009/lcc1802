//VserialNW.c - serial demo program
//17-11-21 adapted to use makeit.bat and XR18NW protocols for setting options
#include <olduino.h>
#include <nstdlib.h>
#define putc(x) putcser(x)
void putcser(unsigned char);
unsigned char getcser();
#define __CPUSPEED 4000000
#define __CPUTYPE  1802
#define __CPUTYPE1802
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
