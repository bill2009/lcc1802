/*
   print the string "hello World!"
   output is via port 7 instead of 5
*/
//Jan 29 - trying to define putc(x) as out(7,x)
#include <nstdlib.h>
#define putc(x) out(7,x)
void main()
{
	printstr("Hello World!\n");

	while(1){
		printstr("@@");
		asm("	seq\n");
		out(7,'*');
		asm("	req\n");
		out(7,'!');
	}

}
#include <nstdlib.c>
