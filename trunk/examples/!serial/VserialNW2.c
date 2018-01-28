//VserialNW.c - serial demo program
//17-11-21 adapted to use makeit.bat for setting options
#include <olduino.h>
#include <nstdlib.h>
#define putc(x) putcser(x)
void putcser(unsigned char);
void main(){
	unsigned char cin='?';
	asm(" seq\n"); //make sure Q is high to start
		printf("UUUUUUUU");
	while(1);
}
void includeser(){
	asm("BAUDRATE EQU 2400\n");
	asm(" include VELFserial3.inc\n");
}
#include <olduino.c>
#include <nstdlib.c>
