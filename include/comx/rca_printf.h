#ifndef _RCA_PRINTF_H
#define _RCA_PRINTF_H

//rca_printf header
void putlcccx(unsigned char c);
void gotoxy(unsigned char x, unsigned char y);
void rca_printf_includer(){
asm(" include comx/rca_printf.inc\n");
}

#define putc(c) putlcccx(c)

#endif // __RCA_PRINTF_H
