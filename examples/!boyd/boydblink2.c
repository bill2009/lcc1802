#include "olduino.h"
#define initleds() 	asm(" req\n seq\n dec 2\n out 7\n req\n")
unsigned char text[]={1,8,0,5};
unsigned char banner[]={15,15,15,15,5,0,8,1,15};
void texttext(int n){
	unsigned int i;
	initleds();
	for (i=n; i>0;i--){
		out(7,text[i-1]);
	}
}
void main()
{
	register int start, loc;
	initleds();
	asm(//" req\n seq\n"
		//" dec 2\n out 7\n req\n"
		" ldad r11,_banner\n"
		" ldad r10,8\n"
		" sex 11\n"
		" out 7\n"
		" out 7\n"
		" out 7\n"
		" out 7\n"
		" out 7\n"
		" out 7\n"
		" out 7\n"
		" out 7\n"
		" sex 2\n");
	delay(1000);
	texttext(1);
	delay(1000);
	texttext(2);
	while(1);
	while(1){
		for(start=7;start>=0;start--){ //#digits
			initleds();
			for(loc=start;loc<8;loc++){
				out(7,banner[loc]);
				delay(100);
			}
			delay(1000);
		}
	}
}

#include "olduino.c" //for the delay routine
