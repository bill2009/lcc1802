//boydmonseg - sensitive to segment decoding
#include "olduino.h"
#include <nstdlib.h>
#define initleds(mode) 	asm(" req\n seq\n dec 2\n ldi " #mode "\n str 2\n out 7\n req\n")
#include "boyd.h" //definitions for the boyd calculator
unsigned char boydscan();
void disp1(unsigned char d){//display a byte as two hex digits
	asm(" glo 12\n ani 0x0f\n" //prep bottom digit
		" dec 2\n str 2\n out 7\n"
		" glo 12\n shr\n shr\n shr\n shr\n" //prep top digit
		" dec 2\n str 2\n out 7\n"
		);

}

void dispmemloc(unsigned char * loc){
	register unsigned int lint;
	initleds(0b11010000); //LEDs in hex decode mode
	disp1(*(loc+1));
	disp1(*loc);
	lint=(unsigned int)loc;
	disp1((unsigned int)loc&0xff);
	disp1(lint>>8);
}
void dispval(unsigned char v){
	register unsigned int i;
	initleds(0b11010000); //LEDs in hex decode mode
	disp1(v);
	for (i=6;i!=0;i--) out(7,0);

}
unsigned int getsp(){//return stack pointer value
	asm(" cpy2 r15,sp\n"  	//copy stack pointer to return reg
		" cretn\n");		//return it to the caller;
	return 0;				//not executed
}
unsigned char * execute(unsigned char * loc){
	unsigned char op,val;
	unsigned char * mp;
	while(1){
		//dispval(0x44); delay(250);
		//dispmemloc(loc); delay(3000);
		op=*loc; val=*(loc+1);
		switch (op){
			case 0: //display memory at mem[val];
				//dispval(0x49); delay(250);
				mp=(unsigned char *)(4096+val);
				dispval(*mp); delay(1000);
				//dispval(0x50); delay(250);
				break;
			case 1: //increment location val
				mp=(unsigned char *)(4096+val);
				*mp+=1;
				break;
			case 2: //goto val
				loc=(unsigned char *)(val+4096-2); //ugh
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
	}
	return loc;
}
void dispalpha(unsigned char data[]){
	register unsigned int i;
	dispval(getsp()); //display
	delay(100);
	initleds(0b11110000); //LEDs in no-decode mode
	for (i=8;i!=0;i--){
		out(7,boydsegments[data[i]]);
	}
}
unsigned int strlen(char *str)
{
	unsigned int slen = 0 ;
	while (*str != 0) {
      slen++ ;
      str++ ;
   }
   return slen;
}

void dispstr(unsigned char * str){//display 8 or fewer characters on the boyd LEDs
	register unsigned int i,L;
	L=min(strlen((char *)str),8);//length to display
	initleds(0b11110000); //LEDs in no-decode mode
	if (L<8){
		for(i=(L-8); i>0;i--){
			out(7,255);
		}
	}
	for (i=L;i>0;i--){
		out(7,boydsegments[str[i-1]]);
	}
}



void main()
{
	unsigned char * loc=0;
	unsigned char memtype='o'; //displaying o=eeprom,a=ram
	unsigned char k,k2;
	//asm("LCCNOMATH EQU 1\n");
	dispval(0x42);
/*	delay(100);
	dispval(getsp());
	delay(1000);
	dispalpha((unsigned char *)" ABCDEFGH");
	delay(1000);
	dispalpha((unsigned char *)" IJKLMNOP");
	delay(1000);
	dispalpha((unsigned char *)" QRSTUVWX");
	delay(1000);
	dispalpha((unsigned char *)" BILLROWE");
	delay(1000);
	dispval(0x45);*/
	delay(100);
	dispstr((unsigned char *)"BARRY");
	delay(5000);
	dispstr((unsigned char *)"01234567");
	delay(5000);
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
					loc=(unsigned char *)4096;
					memtype='a';
				}else{
					loc=(unsigned char *)0;
					memtype='o';
				}
				break;
			case 19: //ms
				dispmemloc(loc); //makes a blink
				k=boydscan(); dispval(k); delay(250);
				k2=boydscan(); dispval(k2); delay(250);
				*loc=(k<<4)+k2;
				break;
			case 20: //X for execute
				dispval(0x45);
				delay(250);
				loc=execute(loc);
				break;
			default:
				dispval(k);
				delay(250);
		}
	}
}

#include "olduino.c" //for the delay routine
void boydinc(){
	asm(" align 256\n");
	asm(" include \"boydscan.inc\"\n");
}
