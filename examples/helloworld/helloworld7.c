/*
   print the string "hello World!"
*/
#include <nstdlib.h>
#include <cpu1802spd4port7.h>
#define putc(x) out(7,x)
void main()
{
	printstr("Hello World!\n");
	asm("x: jnz x\n");
	asm(" org 0xf0\n");
	asm(" jmp past\n");
	asm(" jmp past\n");
	asm(" jmp past\n");
	asm(" jmp past\n");
	asm("past: equ $\n");
	asm(" jnz $+0x100\n");
}
#include <nstdlib.c>
