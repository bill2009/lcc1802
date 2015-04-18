#include "olduino.h"
#include "nstdlib.h"
#include <cpu1802spd4port7.h>
char * dd32cr1(long int);
char * ultoa(unsigned long int,char *);
char * xltoa(unsigned long binval, char* bcd){
	//if (binval>9){
		asm("\tinclude ddgt32.inc\n"); //ends with a return
	//}else{
	//	bcd[0]=binval|'0';
	//	bcd[1]=0;
	//}
	return bcd;
}
void main(){
	char bcdbuf[12]={10,10,10,10,10,10,10,10,10,10,10,10};
	char *bcd;unsigned long binval=21; unsigned int bvali=12345;
	int i;

	delay(3000);
	printf("\rBCD Mule Thursday\r\n");
/*
	printf("16 bit %d\r\n",bvali);
	printf(" 32 bit %ld ",binval);
	printf("%lx ",binval);
	bcd=ultoa(binval,bcdbuf);
	printf("buffer is at %x\r\n",bcdbuf);
	printf("%x:",bcd);
	printf("%cx %cx %cx %cx %cx %cx %cx %cx %cx %cx %cx %cx",bcd[0],bcd[1],bcd[2],bcd[3],bcd[4],bcd[5],bcd[6],bcd[7],bcd[8],bcd[9],bcd[10],bcd[11]);
	printf("\n\rresult %s\n\r",bcd);
*/
	for (i=10;i>=-10;i--){
		binval=i;
		printf("%d %lx=%ld=%s\n\r",i,binval,binval,xltoa(binval,bcdbuf));
		binval--;
	}
}
#include "nstdlib.c"
#include "olduino.c"
