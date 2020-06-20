#include "starship.h"
#include <nstdlib.h>
#include <olduino.h>
void startpixie(){
	asm(" ldaD R1,DisplayInt\n");
	(void)inp(1);
}
void display(char * what, unsigned int where){//where is encoded as hex yyxx for row and column in pixels
	asm(" cpy2 RF,R12\n"
		" cpy2 RE,R13\n"
		" ccall DrawString\n");
}
void disprc(char * what, unsigned char row, unsigned char col){//row and char are pixel coordinates
	unsigned int where=(row<<8)+col;
	display(what,where);
}
void clear(){
	asm(" ld2z RF\n");
	asm(" ccall FillScreen\n");
}
int main(){
	unsigned char row=0, col=0; //row and column positions for drawing
	int rdir=6, cdir=3; //direction of motion
	//memcpy((void *)0x200, starship, 256);
	//disprc("Hello Bill!",16,16); //pixel coordinates row and column
	startpixie();
	while(1){
		disprc(" ",row,col);
		if (row>58){
			rdir=-6;
		} else if (row<6){
			rdir=6;
		}
		if (col>58){
			cdir=-6;
		} else if (col<6){
			cdir=6;
		}
		row+=rdir;col+=cdir;
		disprc("O",row,col);
		delay(100);
	}
//label: goto label;
	return 0;
}
void includepixie(){
	asm(" INCLUDE bitfuncs.inc\n");
	asm(" include nStdDefs.inc\n");
	asm("System				EQU \"Elf\"\n");
	asm("UseIO				EQU \"TRUE\"\n");
	asm("UseRandom			EQU \"TRUE\"\n");
	asm("RandomSize			EQU 8				; 8 or 16\n");
	asm("UseGraphics			EQU \"TRUE\"\n");
	asm("Resolution			EQU \"64x64\"			; 64x32, 64x64 or 64x128\n");
	asm("BackBuffer			EQU \"OFF\"			; 'OFF', 'COPY' or 'SWAP'\n");
	asm("UseText				EQU \"TRUE\"\n");
	asm("Use96Char			EQU \"TRUE\"			; TRUE = 96 char, FALSE = 64 char\n");
	asm("UseConsole			EQU \"FALSE\"\n");
	asm("UseConversion		EQU \"TRUE\"\n");
	asm(" align 256\n");
	asm(" INCLUDE Graphics1861.asm\n");
	asm(" INCLUDE Conversion.asm\n");
	asm(" align 256\n");
	asm(" INCLUDE Text1861.asm\n");
}
#include <nstdlib.c>
#include <olduino.c>
