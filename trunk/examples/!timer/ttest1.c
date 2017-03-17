//timer test 1 - 1806 timer counter demo
#include <olduino.h>
#include <nstdlib.h>
#include <cpu1802spd4port7.h>
void LDC(unsigned char c){
	asm(" glo r12 ; pick up the value\n"
		" LDC ;		set the timer\n");
}
unsigned char GEC(){
	asm(" GEC ;		get the value\n"
		" plo r15\n ldi 0\n phi r15 \n"
		" cretn ;	this is the actual return\n");
	return 42;//just to keep the compiler happy
}
void main()
{
	unsigned char t1,t2;
	int i,d=16;
	printf("Hello Timer Fans\n");
	asm(" CID; disable timer interrupts\n");
	LDC(255);//load the timer
	asm(" STM; start the timer\n");
	asm(" GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n");
	asm(" GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n");
	asm(" GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n");
	asm(" GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n");
	asm(" GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n GEC\n");
	t1=GEC();
	delay(16);
	t2=GEC();
	printf("t1=%d, t2=%d\n\n",t1,t2);
	printf("spin delay of %d ms\n",d);
	printf("covered about %f timer ms\n",(float)(t1-t2)/(15.625));
	printf("done\n");
}
#include <nstdlib.c>
#include <olduino.c>
