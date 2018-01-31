//boydmonseg - sensitive to segment decoding
#include "olduino.h"
#include <nstdlib.h>
#define initleds(mode) 	asm(" req\n seq\n dec 2\n ldi " #mode "\n str 2\n out 7\n req\n")
#include "boyd.h" //definitions for the boyd calculator
#define nofloats
unsigned char boydscan();
void disp1(unsigned char d){//display a byte as two hex digits
	asm(" glo 12\n ani 0x0f\n" //prep bottom digit
		" dec 2\n str 2\n out 7\n"
		" glo 12\n shr\n shr\n shr\n shr\n" //prep top digit
		" dec 2\n str 2\n out 7\n"
		);

}

void dispval(unsigned char v){
	register unsigned int i;
	initleds(0b11010000); //LEDs in hex decode mode
	disp1(v);
	for (i=6;i!=0;i--) out(7,0);

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
char * itoa(int s, char *buffer){ //convert an integer to printable ascii in a buffer supplied by the caller
	unsigned int r,k,n;
	unsigned int flag=0;
	char * bptr; bptr=buffer;
	if (s<0){
		*bptr='-';bptr++;
		n=-s;
	} else{
		n=s;
	}
	k=10000;
	while(k>0){
		for(r=0;k<=n;r++,n-=k); // was r=n/k
		if (flag || r>0||k==1){
			*bptr=('0'+r);bptr++;
			flag='y';
		}
		//n=n-r*k;
		k=k/10;
	}

	*bptr='\0';
	return buffer;
}

void dispstr(char * str){//display 8 or fewer characters on the boyd LEDs
	register unsigned int i,L;
	L=min(strlen((char *)str),8);//length to display
	initleds(0b11110000); //LEDs in no-decode mode
	if (L<8){
		for(i=(8-L); i>0;i--){ //blank trailing positions
			out(7,0);
		}
	}
	for (i=L;i>0;i--){
		out(7,boydsegments[str[i-1]]);
	}
}



void main()
{
	unsigned int acc=0;
	unsigned char k,k2;
	char buf[8]="01234567";
	dispval(0x42);

	delay(100);
	dispstr("BOYDPROG");
	delay(2000);
	while(1){
		dispstr("_"); acc=0;
		k=boydscan();
		while(k<16){
			acc=(acc<<4)+k;
			//stringit(buf,k);
			dispstr(itoa(acc,buf));
			k=boydscan();
		}
	}
}

#include "olduino.c" //for the delay routine
void boydinc(){
	asm(" align 256\n");
	asm(" include \"boydscan.inc\"\n");
}
