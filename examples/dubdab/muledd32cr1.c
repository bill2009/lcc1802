#include "olduino.h"
#include "nstdlib.h"
#include <cpu1802spd4port7.h>
char * dd32cr1(long int);
void inc(){
	asm("\tinclude dd32cr1.inc\n");
}
void main(){
	char *bcd;long binval=12345;
	delay(3000);
	printf("\rBCD Mule Saturday\r\n");
	printf("%ld ",binval);
	printf("%lx ",binval);
	printf("%lx ",binval);
	bcd=dd32cr1(binval);
	printf("%x ",bcd);
	printf("%cx %cx %cx %cx %cx %cx %cx %cx %cx %cx",bcd[0],bcd[1],bcd[2],bcd[3],bcd[4],bcd[5],bcd[6],bcd[7],bcd[8],bcd[9]);
}
#include "nstdlib.c"
#include "olduino.c"
