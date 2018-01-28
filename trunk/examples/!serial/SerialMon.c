//VserialNW.c - serial demo program
//17-11-21 adapted to use makeit.bat for setting options
#include <olduino.h>
#include <nstdlib.h>
#define putc(x) putcser(x)
unsigned char getcser();
#define getc() getcser()
void putcser(unsigned char);
#include <dumper.c>
#define EOT 4
#define REGADDR (unsigned int *)0xFF00 //where the registers are stored on reset
#define CODEADDR (unsigned char *)0x8000 //address where code is loaded
void regdump(unsigned int* data){
	unsigned int i;
	for(i=0;i<16;i++){
		if (0==(i%4)) printf("\nR%cx ",i);
		printf("%x ",*data++);
	}
	printf("\n");
}
void dispatch(unsigned char * addr){
	asm(" cpy2 r0,r12\n"	//copy target address to R0
		" sex 0\n"			//x register=0
		" sep 0\n"			//and pass control
	);
}
int gethexit(unsigned char cin){ //returns the corresponding hex value for 0-f, -1 otherwise
	if ((cin>='0') && (cin<='9')){
		return cin-'0';
	}else if ((cin>='a') & (cin<='f')){
		return cin-'a'+10;
	}else{
		return -1;
	}
}

int XR(unsigned char *); //xmodem receiver
int bootload(){
	int ret=XR(CODEADDR);
	if (ret==EOT){
		dispatch(CODEADDR);
	} else if(ret=='T'){//timeout flag
		dispatch(CODEADDR);
	} else{
		printf("Interrupt - starting monitor\n");
	}
	return 0;
}

void main(){
	unsigned char cin='?';
	unsigned char seqop=0x7b, reqop=0x7a, brop=0x30;
	unsigned char * addr=0;
	int ret;
	unsigned int caddr=0;
	int hexval;
	asm(" seq\n"); //make sure Q is high to start
	//bootload();//try the loader
	printf("\nSerial Monitor Here - CODEADDR is %x. Baudrate 9600\n",CODEADDR);
	while(1){
		printf("> ");
		cin=getc();
		hexval=gethexit(cin);
		while(hexval>=0){//accumulate address entries - non hex digit characters will return -1
			printf("%c",cin);
			caddr*=16;caddr+=hexval;
			cin=getc();
			hexval=gethexit(cin);
		}
		switch (cin){
			case 'x': //xmodem receive
				printf("\nCalling XR in 2 sec\n",ret);
				delay(2000);
				ret=XR(addr);
				printf("XR returns %x\n",ret);
				if (ret==EOT){
					dump(addr,256);
				} else if(ret=='T'){//timeout flag
					printf("Timeout - no transfer\n");
				} else{
					printf("Interrupt - no transfer\n");
				}
				break;
			case '\r':
				printf("->%x\n",caddr);
				addr=(unsigned char *)caddr;caddr=0;
				dump(addr,16);
				break;
			case '?':			//display memory at addr
				printf("?\n");
				dump(addr,16);
				break;
			case '!':			//alter memory at addr
				printf("!\n");
				*addr=seqop;*(addr+1)=reqop;*(addr+2)=brop;*(addr+3)=(unsigned char)((unsigned int)addr&255); //poke in a loop
				dump(addr,16);
				break;
			case 'g':			//go to program at addr (rp=0)
				printf("g\n");
				dispatch(addr);
				break;
			case 'r':
				printf("r\n");
				regdump(REGADDR);
				break;
			case '+':
				printf("+\n");
				addr+=16;
				dump(addr,16);
				break;
			case '-':
				printf("-\n");
				addr-=16;
				dump(addr,16);
				break;
			default:
				printf("%c/%cx unrecognized\n",cin,cin);
				break;
		}
	}
}

void includeser2(){
	asm("BAUDRATE EQU 	9600\n");
	asm(" include xrwjrT3.inc\n");
	asm(" include serwjrT3.inc\n");
}

#include <olduino.c>
#include <nstdlib.c>
void saveregs(){asm("SAVEREGS: EQU 1\n");}
