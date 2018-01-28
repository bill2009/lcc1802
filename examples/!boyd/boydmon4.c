#include "olduino.h"
#define initleds() 	asm(" req\n seq\n dec 2\n out 7\n req\n")
unsigned char ef(){//return 1-4 if EF1-4 is asserted or 0 if none
	asm(" rldi r15,0\n"
		" inc r15\n b1 .ret\n"  //ef1
		" inc r15\n b2 .ret\n"  //ef1
		" inc r15\n b3 .ret\n"  //ef1
		" inc r15\n b4 .ret\n"  //ef1
		" ldi 0\n plo r15\n"	//none
		" cretn\n");
	return 0; //keep the compiler happy
}
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
void disp1(unsigned char d){//display a byte as two hex digits
	asm(" glo 12\n ani 0x0f\n" //prep bottom digit
		" dec 2\n str 2\n out 7\n"
		" glo 12\n shr\n shr\n shr\n shr\n" //prep top digit
		" dec 2\n str 2\n out 7\n"
		);

}


void disp12(){
	initleds();
	out(7,0x12);
	out(7,15);out(7,15);out(7,15);out(7,15);out(7,15);out(7,15);out(7,15);
}
void disp42(){
	initleds();
	out(7,2);
	out(7,4);
	out(7,15);out(7,15);out(7,15);out(7,15);out(7,15);out(7,15);
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
	disp1(loc);
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
