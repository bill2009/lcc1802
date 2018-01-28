#include <olduino.h>
#define	EF3	0
void putcser(char);
unsigned char getcser(){
	//unsigned char c=0,n=8; //R15 is C, R11 is n
	asm(" ldi 0\n plo 15\n ldi 8\n plo 11\n");
	//while(EF3); //wait for start
	asm(" bn3 $\n");
	//delay(.2);  //middle of start bit
	asm(" ldi (LCC1802CPUSPEED/5000/16)/2\n" //.2ms
		".dh smi 1\n"
		" bnz .dh\n");
	//do{
	asm(".nextbit:\n");
		//delay(.4);
		asm(" ldi (LCC1802CPUSPEED/2500/16)/2\n" //.4ms
			".d1 smi 1\n"
			" bnz .d1\n");
		//c=c<<1|EF3; //data bit
		asm(" ldi 0	;assume no bit\n"
			" b3  .nobit\n"
			" ldi 1\n ;set the bit\n"
			".nobit:\n"
			" shr	;move bit/nobit to df\n"
			" glo 15\n shrc\n plo 15\n"); //shift toward lower end
		//n--;
		asm(" dec 11\n");
	//}while(n>0);
	asm(" glo 11\n bnz .nextbit\n");
	//return c;
	asm(" cretn\n");
	return 0; //just to keep the compiler happy
}
void main(){
	unsigned char c;
	asm(" seq\n"); //make sure Q is high to start
	while(1){
		putcser('?');
		c=getcser();
		asm(" seq\n req\n");
		putc(c);
		asm(" seq\n req\n");
		putcser(c);
		delay(1000);
	}
}
void noname(){
	asm(" include IOser1802.inc\n");
}
#include <olduino.c>
