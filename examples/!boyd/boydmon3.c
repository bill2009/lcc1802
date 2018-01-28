#include "olduino.h"
#define initleds() 	asm(" req\n seq\n dec 2\n out 7\n req\n")
unsigned char ef1(){
	asm(" rldi 15,1\n"
		" bn1 .ret0\n"
		" cretn ;will return a '1'\n"
		".ret0: ;will drop thru to return 0\n");
	return 0; //if the assembly doesn't return, EF1 is not active
}
unsigned char ef2(){
	asm(" rldi 15,1\n"
		" bn2 .ret0\n"
		" cretn ;will return a '1'\n"
		".ret0: ;will drop thru to return 0\n");
	return 0; //if the assembly doesn't return, EF1 is not active
}
void disp1(unsigned char d){//display a hex digit as two dec digits 00 to 15
	asm(" glo 12\n smi 10\n bdf $$big\n" //df set means no overflow
		" ldi 15\n phi 12\n br $$disp\n"
		"$$big: plo 12\n"
		" ldi 1\n phi 12\n"
		"$$disp:\n"
		" dec 2\n glo 12\n str 2\n out 7\n"
		" dec 2\n ghi 12\n str 2\n out 7\n"
		);

/*
	if (d>9){
		out(7,d-10);
		out(7,1);
	}else{
		out(7,d);
		out(7,15);
	}
*/
}


void disp2(unsigned char xy){//display a byte as 4 decimal digits xx yy
	disp1(xy&0x0f);//bottom digit
	disp1(xy>>4);
}
void disp12(){
	initleds();
	out(7,2);
	out(7,1);
	out(7,15);out(7,15);out(7,15);out(7,15);out(7,15);out(7,15);
}
void disp42(){
	initleds();
	out(7,2);
	out(7,4);
	out(7,15);out(7,15);out(7,15);out(7,15);out(7,15);out(7,15);
}
void dispbl(){
	register int i;
	initleds();
	for (i=8;i>0;i--){
		out(7,15);
	}
}
void dispmemloc(unsigned int loc){
	register unsigned char* m=0;
	register unsigned char m1,m2;
	register unsigned int i;
	initleds();
	m1=m[loc]>>4;
	m2=m[loc]&0x0f;
	out(7,m2); out(7,m1);
	for (i=6;i!=0;i--) out(7,15);

}
void dispm2(unsigned int loc){
	register unsigned int i;
	initleds();
	disp2(loc);
	for (i=6;i!=0;i--) out(7,15);

}
void main()
{
	disp42();
	delay(1000);
	dispmemloc(0);
	delay(1000);
	dispm2(0x25);
	delay(1000);
	while(1){
		out(1,1); //activate top row of keys
		if(ef1()){ //key 12 pressed
			disp12();
		} else if(ef2()){//key 8
			dispmemloc(01);
		}else{
			dispmemloc(02);;
		}
	}
	while(1);
}

#include "olduino.c" //for the delay routine
