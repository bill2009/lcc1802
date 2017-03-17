//timer test 1 - 1806 timer counter demo
#include <olduino.h>
#include <nstdlib.h>
#include <cpu1802spd4port7.h>
#include "timer1806.h"
void main()
{
	unsigned int t1,t2;
	int i,d=100;
	printf("Hello Timer Fans\n");
	asm(" seq\n");
	delay(d);
	asm(" req\n");
	initmillis();
	printf("Now we're clocking!\n");
	t1=millis;
	asm(" seq\n");
	delay(d);
	asm(" req\n");
	t2=millis;
	printf("t1=%d, t2=%d\n\n",t1,t2);
	printf("spin delay of %d ms\n",d);
	printf("covered %d timer ms\n",t2-t1);
	printf("done\n");
}
#include <nstdlib.c>
#include <olduino.c>
