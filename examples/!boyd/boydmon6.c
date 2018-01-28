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
	disp1(loc&0xff);
	disp1(loc>>8);
}
void dispval(unsigned char v){
	register unsigned int i;
	initleds();
	disp1(v);
	for (i=6;i!=0;i--) out(7,0);

}
unsigned int getsp(){//return stack pointer value
	asm(" cpy2 r15,sp\n"  	//copy stack pointer to return reg
		" cretn\n");		//return it to the caller;
	return 0;				//not executed
}
void execute(unsigned int loc){
	unsigned char* mem=0;
	unsigned char op,val;
	//while(1){
		dispval(0x44); delay(250);
		dispmemloc(loc); delay(3000);
		op=mem[loc]; val=mem[loc+1];
		switch (op){
			case 0: //display memory at mem[val];
				dispval(0x49); delay(250);
				dispval(mem[val+4096]); delay(1000);
				dispval(0x50); delay(250);
				break;
			case 1: //increment location val
				mem[val+4096]+=1;
				break;
			case 2: //goto val
				loc=val+4096-2; //ugh
				break;
			case 3: //delay val*4 ms
				delay(val*4);
				break;
			case 4: //display stack pointer;
				dispval(getsp());
				delay(250);
				break;
			default:
				dispval(0x41); delay(250);
				dispmemloc(loc); delay(5000);
				break;
		}
		loc+=2;
	//}
}



void main()
{
	unsigned int loc=0;
	unsigned char memtype='o'; //displaying o=eeprom,a=ram
	unsigned char k,k2;
	register unsigned char* mem=0;
	dispval(0x42);
	delay(1000);
	while(1){
		dispmemloc(loc);
		k=boydscan();
		switch(k){
			case 16: //+
				loc +=1;
				break;
			case 17: //-
				loc -=1;
				break;
			case 18:	//rem
				if (memtype=='o'){
					loc=4096;
					memtype='a';
				}else{
					loc=0;
					memtype='o';
				}
				break;
			case 19: //ms
				dispmemloc(loc); //makes a blink
				k=boydscan(); dispval(k); delay(250);
				k2=boydscan(); dispval(k2); delay(250);
				mem[loc]=(k<<4)+k2;
				break;
			case 20: //X for execute
				loc=4096;
				dispval(0x45);
				delay(250);
				execute(4096);
				break;
			default:
				dispval(k);
				delay(250);
		}
	}
}

#include "olduino.c" //for the delay routine
