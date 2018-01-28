#include "olduino.h"
#define initleds() 	asm(" req\n seq\n dec 2\n out 7\n req\n")
unsigned char boydscan();
void boydinc(){
	asm(" include \"boydscan.inc\"\n");
}
void disp1(unsigned char d){//display a byte as two hex digits
	asm(" glo 12\n ani 0x0f\n" //prep bottom digit
		" dec 2\n str 2\n out 7\n"
		" glo 12\n shr\n shr\n shr\n shr\n" //prep top digit
		" dec 2\n str 2\n out 7\n"
		);

}

void dispmemloc(unsigned int loc){
	register unsigned char* m=0;
	register unsigned char m1,m2;
	register unsigned int i;
	initleds();
	disp1(m[loc+1]);
	disp1(m[loc]);
	disp1(loc&0x0f);
	disp1(loc>>8);
}
void dispval(unsigned char v){
	register unsigned int i;
	initleds();
	disp1(v);
	for (i=6;i!=0;i--) out(7,0);

}
void disp42(){
	initleds();
	out(7,2);
	out(7,4);
	out(7,15);out(7,15);out(7,15);out(7,15);out(7,15);out(7,15);
}

void main()
{
	unsigned int loc=0;
	unsigned char m='o'; //displaying o=eeprom,a=ram
	unsigned char k;
	disp42();
	delay(1000);
	while(1){
		dispmemloc(loc);
		k=boydscan();
		dispval(k); delay(250);
		if (99==k){
			if (m=='o'){
				loc=4096;
				m='a';
			}else{
				loc=0;
				m='o';
			}
		}else
			loc+=2;
		}
}

#include "olduino.c" //for the delay routine
