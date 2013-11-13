; generated by lcc-xr182b $Version: 5.0 - XR182b $ on Thu May 16 15:21:08 2013

SP:	equ	2 ;stack pointer
memAddr: equ	14
retAddr: equ	6
retVal:	equ	15
regArg1: equ	12
regArg2: equ	13
	listing off
	include lcc1802ProloFL.inc
	listing on
_main: ;copt is peeping you now!
	pushr r7
	reserve 4; save room for outgoing arguments
;{
;	printf("generates 92 solutions. \n15863724 is first\n\n");
	ldaD R12,L2; reg:acon
	Ccall _printf
;	for (i = 14; i >=0; i--)
	ldaD R7,14; reg:acon
L3:
;		up[i] = down[i] = 1;
	ldaD R11,1; reg:acon
	cpy2 R10,R7
	shl2I R10,1
	st2 R11,'O',R10,(_down); ASGNI2(addr,reg)*;
	st2 R11,'O',R10,(_up); ASGNI2(addr,reg)*;
L4:
;	for (i = 14; i >=0; i--)
	decm R7,1
	jcI2I r7,0,lbdf,L3; GE is flipped test from LT
;	for (i = 7; i >=0; i--)
	ldaD R7,7; reg:acon
L7:
;		rows[i] = 1;
	ldaD R11,1; reg:acon
	cpy2 R10,R7
	shl2I R10,1
	st2 R11,'O',R10,(_rows); ASGNI2(addr,reg)*;
L8:
;	for (i = 7; i >=0; i--)
	decm R7,1
	jcI2I r7,0,lbdf,L7; GE is flipped test from LT
;	queens(0);
	ld2z R12
	Ccall _queens; CALLI2(ar)
;	printf("...and...\n84136275 is last\nTook about 25 sec on 1.6mhz elf with Christmas compiler\n");
	ldaD R12,L11; reg:acon
	Ccall _printf
;	printf("Took about 22 sec on 1.6mhz elf with Birthday compiler\n");
	ldaD R12,L12; reg:acon
	Ccall _printf
;	printf("Took about 17 sec on 1.6mhz elf with St Judy's compiler(from run1802 to 92nd solution)\n");
	ldaD R12,L13; reg:acon
	Ccall _printf
;	return 0;
	ld2z R15
L1:
	release 4; release room for outgoing arguments
	popr r7
	Cretn

_queens:
	pushr r6
	pushr r7
	reserve 4; save room for outgoing arguments
	cpy2 r7,r12; function(2053) 1
;{
;	for (r = 0; r < 8; r++){
	ld2z R6
L15:
;		if (rows[r] && up[r-c+7] && down[r+c]) {
	cpy2 R11,R6
	shl2I R11,1
	ld2 R11,'O',R11,(_rows) ;reg:INDIRI2(addr)
	jzU2 r11,L19; EQ 0
	alu2 R11,R6,R7,sm,smb
	shl2I R11,1
	ld2 R11,'O',R11,(_up+14) ;reg:INDIRI2(addr)
	jzU2 r11,L19; EQ 0
	alu2 R11,R6,R7,add,adc; ADDI2(r,r)
	shl2I R11,1
	ld2 R11,'O',R11,(_down) ;reg:INDIRI2(addr)
	jzU2 r11,L19; EQ 0
;			rows[r] = up[r-c+7] = down[r+c] = 0;
	ld2z R11
	alu2 R10,R6,R7,add,adc; ADDI2(r,r)
	shl2I R10,1
	st2 R11,'O',R10,(_down); ASGNI2(addr,reg)*;
	alu2 R10,R6,R7,sm,smb
	shl2I R10,1
	st2 R11,'O',R10,(_up+14); ASGNI2(addr,reg)*;
	cpy2 R10,R6
	shl2I R10,1
	st2 R11,'O',R10,(_rows); ASGNI2(addr,reg)*;
;			x[c] = r;
	cpy2 R11,R7
	shl2I R11,1
	st2 R6,'O',R11,(_x); ASGNI2(addr,reg)*;
;			if (c == 7)
	jneU2I r7,7,L23; NE
;				print();
	Ccall _print; CALLI2(ar)
	lbr L24
L23:
;				queens(c + 1);
	cpy2 R12,R7 ;reg:ADDI2(consm,reg)
	incm R12,1
	Ccall _queens; CALLI2(ar)
L24:
;			rows[r] = up[r-c+7] = down[r+c] = 1;
	ldaD R11,1; reg:acon
	alu2 R10,R6,R7,add,adc; ADDI2(r,r)
	shl2I R10,1
	st2 R11,'O',R10,(_down); ASGNI2(addr,reg)*;
	alu2 R10,R6,R7,sm,smb
	shl2I R10,1
	st2 R11,'O',R10,(_up+14); ASGNI2(addr,reg)*;
	cpy2 R10,R6
	shl2I R10,1
	st2 R11,'O',R10,(_rows); ASGNI2(addr,reg)*;
;		}
L19:
;	}
L16:
;	for (r = 0; r < 8; r++){
	incm R6,1
	jcI2I r6,8,lbnf,L15  ;LT=lbnf i.e. subtract immedB from A and jump if borrow
	ld2z R15
;}
L14:
	release 4; release room for outgoing arguments
	popr r7
	popr r6
	Cretn

_print:
	pushr r7
	reserve 4; save room for outgoing arguments
;{
;	for (k = 0; k < 8; k++)
	ld2z R7
L27:
;		printf("%c", x[k]+'1');
	ldaD R12,L31; reg:acon
	cpy2 R11,R7
	shl2I R11,1
	ld2 R11,'O',R11,(_x) ;reg:INDIRI2(addr)
	ldA2 R13,'O',R11,(49); reg:addr
	Ccall _printf
L28:
;	for (k = 0; k < 8; k++)
	incm R7,1
	jcI2I r7,8,lbnf,L27  ;LT=lbnf i.e. subtract immedB from A and jump if borrow
;	printf("\n");
	ldaD R12,L32; reg:acon
	Ccall _printf
	ld2z R15
;}
L26:
	release 4; release room for outgoing arguments
	popr r7
	Cretn

_strcpy:
	reserve 2
;{
;	char *save = to;
	st2 R12,'O',sp,(-4+4); ASGNP2
;	for (; (*to = *from) != 0; ++from, ++to);
	lbr L37
L34:
L35:
	incm R13,1
	incm R12,1
L37:
	ldn1 R11,R13;reg:  INDIRU1(indaddr)
	str1 R11,R12; ASGNU1(indaddr,reg)
	jnzU1 r11,L34; NEI2(CVUI2(reg),con0)
;	return(save);
	ld2 R15,'O',sp,(-4+4);reg:  INDIRP2(addr)
L33:
	release 2
	Cretn

_strlen:
	pushr r7
;{
;	unsigned int slen = 0 ;
	ld2z R7
	lbr L40
L39:
;	while (*str != 0) {
;      slen++ ;
	incm R7,1
;      str++ ;
	incm R12,1
;   }
L40:
;	while (*str != 0) {
	ldn1 R11,R12;reg:  INDIRU1(indaddr)
	jnzU1 r11,L39; NEI2(CVUI2(reg),con0)
;   return slen;
	cpy2 R15,R7 ;LOADU2(reg)*
L38:
	popr r7
	Cretn

_printstr:
	pushr r7
	reserve 4; save room for outgoing arguments
	cpy2 r7,r12; function(2055) 1
;void printstr(char *ptr){
	lbr L44
L43:
;    while(*ptr) out(5,*ptr++);
	ldaD R12,5; reg:acon
	cpy2 R11,R7 ;LOADP2(reg) opt1
	incm R7,1
	ld1 R13,'O',R11,0
	zExt R13 ;CVUI2: widen unsigned char to signed int (zero extend)
	Ccall _out; CALLI2(ar)
L44:
	ldn1 R11,R7;reg:  INDIRU1(indaddr)
	jnzU1 r11,L43; NEI2(CVUI2(reg),con0)
;}
L42:
	release 4; release room for outgoing arguments
	popr r7
	Cretn

	align 4
_round_nums:
	dd 0x3f000000
	dd 0x3d4ccccd
	dd 0x3ba3d70a
	dd 0x3a03126f
	dd 0x3851b717
	dd 0x36a7c5ac
	dd 0x350637bd
	dd 0x3356bf95
	align 4
_mult_nums:
	dd 0x3f800000
	dd 0x41200000
	dd 0x42c80000
	dd 0x447a0000
	dd 0x461c4000
	dd 0x47c35000
	dd 0x49742400
	dd 0x4b189680
_ftoa:
	reserve 62; save room for local variables
	pushr r0
	pushr r1
	pushr r6
	pushr r7
	reserve 8; save room for outgoing arguments
	cpy4 RL6,RL12; halfbaked&floaty
	ld2 R1,'O',sp,(6+80); reg:INDIRU2(addr)
;{
;   char *output = outbfr ;
	ld2 R0,'O',sp,(4+80);reg:  INDIRP2(addr)
;   if (flt < 0.0) {
	ld4 RL10,'D',(L49),0;INDIRF4(addr)
	jcF4 RL6,RL10,lbdf,L47;GEF4(reg,reg) - reverse test
;      *output++ = '-' ;
	cpy2 R11,R0 ;LOADP2(reg)
	cpy2 R0,R11
	incm R0,1
	ldaD R10,45; reg:acon
	str1 R10,R11; ASGNU1(indaddr,reg)
;      flt *= -1.0 ;
	ld4 RL8,'D',(L50),0;INDIRF4(addr)
	cpy4 RL10,RL6; LOADU4(reg)
	Ccall fp_mul ;MULF4(reg,reg)
	cpy4 RL6,RL8; LOADU4(reg)
;   } else {
	lbr L48
L47:
;      if (use_leading_plus) {
	lbr L51
;         *output++ = '+' ;
	cpy2 R11,R0 ;LOADP2(reg)
	cpy2 R0,R11
	incm R0,1
	ldaD R10,43; reg:acon
	str1 R10,R11; ASGNU1(indaddr,reg)
;      }
L51:
;   }
L48:
;   if (dec_digits < 8) {
	jcI2I r1,8,lbdf,L53; GE is flipped test from LT
;      flt += round_nums[dec_digits] ;
	cpy4 RL8,RL6; LOADU4(reg)
	cpy2 R11,R1
	shl2I R11,2
	ld4 RL10,'O',R11,(_round_nums);INDIRF4(addr)
	Ccall fp_add ;ADDF4(reg,reg)
	cpy4 RL6,RL8; LOADU4(reg)
;   }
L53:
;	mult=mult_nums[dec_digits];
	cpy2 R11,R1
	shl2I R11,2
	ld4 RL10,'O',R11,(_mult_nums);INDIRF4(addr)
	st4 RL10,'O',sp,(-12+80); ASGNF4(addr,reg)
;   wholeNum = flt;
	cpy4 RL8,RL6; LOADU4(reg)
	ccall cvfi4; CVFI4(reg) convert float to long
	st4 RL8,'O',sp,(-8+80)
;   decimalNum = ((flt - wholeNum) * mult);
	cpy4 RL8,RL6; LOADU4(reg)
	st4 RL8,'O',sp,(-64+80); ASGNF4(addr,reg)
	ld4 RL8,'O',sp,(-8+80);reg:  INDIRI4(addr)
	Ccall cvif4; emit2
	cpy4 RL10,RL8; LOADU4(reg)
	ld4 RL8,'O',sp,(-64+80);INDIRF4(addr)
	Ccall fp_sub ;SUBF4(reg,reg)
	ld4 RL10,'O',sp,(-12+80);INDIRF4(addr)
	Ccall fp_mul ;MULF4(reg,reg)
	ccall cvfi4; CVFI4(reg) convert float to long
	st4 RL8,'O',sp,(-16+80)
;   strcpy(output,dubdabx(wholeNum,output,1));
	ld4 Rp1p2,'O',sp,(-8+80);reg:  INDIRI4(addr)
	st2 r0,'O',sp,(4); arg+f**
	ldaD R11,1; reg:acon
	st2 r11,'O',sp,(6); arg+f**
	Ccall _dubdabx
	cpy2 R11,R15 ;LOADP2(reg)
	cpy2 R12,R0 ;LOADP2(reg)
	cpy2 R13,R11 ;LOADP2(reg)
	Ccall _strcpy
;   output+=strlen(output);
	cpy2 R12,R0 ;LOADP2(reg)
	Ccall _strlen;CALLU2(ar)*
	alu2 R0,R15,R0,add,adc
;   if (dec_digits > 0) {
	jzU2 r1,L55; EQ 0
;		*output++ = '.' ;
	cpy2 R11,R0 ;LOADP2(reg)
	cpy2 R0,R11
	incm R0,1
	ldaD R10,46; reg:acon
	str1 R10,R11; ASGNU1(indaddr,reg)
;		strcpy(output,dubdabx(decimalNum,output,dec_digits));
	ld4 Rp1p2,'O',sp,(-16+80);reg:  INDIRI4(addr)
	st2 r0,'O',sp,(4); arg+f**
	cpy2 R11,R1 ;LOADI2(reg)*
	st2 r11,'O',sp,(6); arg+f**
	Ccall _dubdabx
	cpy2 R11,R15 ;LOADP2(reg)
	cpy2 R12,R0 ;LOADP2(reg)
	cpy2 R13,R11 ;LOADP2(reg)
	Ccall _strcpy
;	}
L55:
;   return outbfr;
	ld2 R15,'O',sp,(4+80);reg:  INDIRP2(addr)
L46:
	release 8; release room for outgoing arguments
	popr r7
	popr r6
	popr r1
	popr r0
	release 62; release room for local variables 
	Cretn

_itoa:
	reserve 2; save room for local variables
	pushr r0
	pushr r1
	pushr r6
	pushr r7
	reserve 4; save room for outgoing arguments
	st2 r12,'O',sp,(16); flag1 
	st2 r13,'O',sp,(18); flag1 
;char * itoa(int s, char *buffer){ //convert an integer to printable ascii in a buffer supplied by the caller
;	unsigned int flag=0;
	ld2z R0
;	char * bptr; bptr=buffer;
	ld2 R11,'O',sp,(2+16);reg:  INDIRP2(addr)
	st2 R11,'O',sp,(-4+16); ASGNP2
;	if (s<0){
	ld2 R11,'O',sp,(0+16) ;reg:INDIRI2(addr)
	jcI2I r11,0,lbdf,L58; GE is flipped test from LT
;		*bptr='-';bptr++;
	ld2 R11,'O',sp,(-4+16);reg:  INDIRP2(addr)
	ldaD R10,45; reg:acon
	str1 R10,R11; ASGNU1(indaddr,reg)
	ld2 R11,'O',sp,(-4+16);reg:  INDIRP2(addr)
	incm R11,1
	st2 R11,'O',sp,(-4+16); ASGNP2
;		n=-s;
	ld2 R11,'O',sp,(0+16) ;reg:INDIRI2(addr)
	negI2 R11,R11 ;was alu2I R11,R11,0,sdi,sdbi
	cpy2 R6,R11 ;LOADU2(reg)*
;	} else{
	lbr L59
L58:
;		n=s;
	ld2 R11,'O',sp,(0+16) ;reg:INDIRI2(addr)
	cpy2 R6,R11 ;LOADU2(reg)*
;	}
L59:
;	k=10000;
	ldaD R7,10000; reg:acon
	lbr L61
L60:
;	while(k>0){
;		for(r=0;k<=n;r++,n-=k); // was r=n/k
	ld2z R1
	lbr L66
L63:
L64:
	incm R1,1
	alu2 R6,R6,R7,sm,smb
L66:
	jcU2 r6,r7,lbdf,L63 ;LE is flipped test & operands
;		if (flag || r>0||k==1){
	jnzU2 r0,L70; NE 0
	jnzU2 r1,L70; NE 0
	jneU2I r7,1,L67; NE
L70:
;			*bptr=('0'+r);bptr++;
	ld2 R11,'O',sp,(-4+16);reg:  INDIRP2(addr)
	ldA2 R10,'O',R1,(48); reg:addr
	str1 R10,R11; ASGNU1(indaddr,reg)
	ld2 R11,'O',sp,(-4+16);reg:  INDIRP2(addr)
	incm R11,1
	st2 R11,'O',sp,(-4+16); ASGNP2
;			flag='y';
	ldaD R0,121; reg:acon
;		}
L67:
;		k=k/10;
	cpy2 R12,R7 ;LOADU2(reg)*
	ldaD R13,10; reg:acon
	Ccall _divu2
	cpy2 R7,R15 ;LOADU2(reg)*
;	}
L61:
;	while(k>0){
	jnzU2 r7,L60; NE 0
;	*bptr='\0';
	ld2 R11,'O',sp,(-4+16);reg:  INDIRP2(addr)
	ldaD R10,0; reg:acon
	str1 R10,R11; ASGNU1(indaddr,reg)
;	return buffer;
	ld2 R15,'O',sp,(2+16);reg:  INDIRP2(addr)
L57:
	release 4; release room for outgoing arguments
	popr r7
	popr r6
	popr r1
	popr r0
	release 2; release room for local variables 
	Cretn

_ltoa:
	pushr r1
	pushr r6
	pushr r7
	reserve 8; save room for outgoing arguments
	cpy4 RL6,RL12; halfbaked
;char * ltoa(long s, char *buffer){ //convert a long integer to printable ascii in a buffer supplied by the caller
;	char* bptr=buffer;
	ld2 R1,'O',sp,(4+16);reg:  INDIRP2(addr)
;	if (s<0){
	ldI4 RL10,0 ;loading a long integer constant
	jcI4 RL6,RL10,lbdf,L72; GE is flipped test from LT
;		*bptr++='-';
	cpy2 R11,R1 ;LOADP2(reg)
	cpy2 R1,R11
	incm R1,1
	ldaD R10,45; reg:acon
	str1 R10,R11; ASGNU1(indaddr,reg)
;		s=-s;
	negI4 RL6,RL6 ;was alu4I RL6,RL6,0,sdi,sdbi
;	}
L72:
;	strcpy(bptr,dubdabx(s,bptr,1)); //uses assembler double-dabble routine
	cpy4 Rp1p2,RL6; LOADI4*
	st2 r1,'O',sp,(4); arg+f**
	ldaD R11,1; reg:acon
	st2 r11,'O',sp,(6); arg+f**
	Ccall _dubdabx
	cpy2 R11,R15 ;LOADP2(reg)
	cpy2 R12,R1 ;LOADP2(reg)
	cpy2 R13,R11 ;LOADP2(reg)
	Ccall _strcpy
;	return buffer;
	ld2 R15,'O',sp,(4+16);reg:  INDIRP2(addr)
L71:
	release 8; release room for outgoing arguments
	popr r7
	popr r6
	popr r1
	Cretn

_printint:
	reserve 12
	st2 r12,'O',sp,(14); flag1 
;void printint(int s){ //print an integer
;	itoa(s,buffer);
	ld2 R12,'O',sp,(0+14) ;reg:INDIRI2(addr)
	ldA2 R13,'O',sp,(-10+14); reg:addr
	Ccall _itoa
;	printstr(buffer);
	ldA2 R12,'O',sp,(-10+14); reg:addr
	Ccall _printstr
;}
L74:
	release 12
	Cretn

_printlint:
	reserve 18
	st2 r12,'O',sp,(20); flag1 
	st2 r13,'O',sp,(22); flag1 
;void printlint(long s){ //print a long integer
;	printstr(ltoa(s,buffer));
	ld4 Rp1p2,'O',sp,(0+20);reg:  INDIRI4(addr)
	ldA2 R11,'O',sp,(-14+20); reg:addr
	st2 r11,'O',sp,(4); arg+f**
	Ccall _ltoa
	cpy2 R11,R15 ;LOADP2(reg)
	cpy2 R12,R11 ;LOADP2(reg)
	Ccall _printstr
;}
L75:
	release 18
	Cretn

_printflt:
	reserve 28
	st2 r12,'O',sp,(30); flag1 
	st2 r13,'O',sp,(32); flag1 
;void printflt(float s){ //print a float
;	printstr(ftoa(s,buffer,3));
	ld4 Rp1p2,'O',sp,(0+30);INDIRF4(addr)
	ldA2 R11,'O',sp,(-22+30); reg:addr
	st2 r11,'O',sp,(4); arg+f**
	ldaD R11,3; reg:acon
	st2 r11,'O',sp,(6); arg+f**
	Ccall _ftoa
	cpy2 R11,R15 ;LOADP2(reg)
	cpy2 R12,R11 ;LOADP2(reg)
	Ccall _printstr
;}
L76:
	release 28
	Cretn

_putxn:
	reserve 4
	st2 r12,'O',sp,(6); flag1 
	ld2 R11,'O',sp,(0+6) ;reg:INDIRI2(addr)
	st1 R11,'O',sp,(0+6); ASGNU1
;void putxn(unsigned char x){ //print a nibble as ascii hex
;	if (x<10){
	ld1 R11,'O',sp,(0+6)
	zExt R11 ;CVUI2: widen unsigned char to signed int (zero extend)
	jcI2I r11,10,lbdf,L78; GE is flipped test from LT
;		putc(x+'0');
	ld1 R11,'O',sp,(0+6)
	zExt R11 ;CVUI2: widen unsigned char to signed int (zero extend)
	ldA2 R11,'O',R11,(48); reg:addr
	cpy1 R12,R11
	zExt R12 ;CVUI2(reg)*: widen unsigned char to signed int (zero extend)*
	Ccall _putc
;	} else {
	lbr L79
L78:
;		putc(x+'A'-10);
	ld1 R11,'O',sp,(0+6)
	zExt R11 ;CVUI2: widen unsigned char to signed int (zero extend)
	ldA2 R11,'O',R11,(65); reg:addr
	alu2I R11,R11,10,smi,smbi
	cpy1 R12,R11
	zExt R12 ;CVUI2(reg)*: widen unsigned char to signed int (zero extend)*
	Ccall _putc
;	}
L79:
;}
L77:
	release 4
	Cretn

_putx:
	reserve 4
	st2 r12,'O',sp,(6); flag1 
	ld2 R11,'O',sp,(0+6) ;reg:INDIRI2(addr)
	st1 R11,'O',sp,(0+6); ASGNU1
;void putx(unsigned char x){ //print a unsigned char as ascii hex
;	putxn(x>>4);
	ld1 R11,'O',sp,(0+6)
	zExt R11 ;CVUI2: widen unsigned char to signed int (zero extend)
	shrI2I R11,4
	cpy1 R12,R11
	zExt R12 ;CVUI2(reg)*: widen unsigned char to signed int (zero extend)*
	Ccall _putxn
;	putxn(x & 0x0F);
	ld1 R11,'O',sp,(0+6)
	zExt R11 ;CVUI2: widen unsigned char to signed int (zero extend)
	alu2I R11,R11,15,ani,ani
	;removed ?	cpy2 R11,R11
	cpy1 R12,R11
	zExt R12 ;CVUI2(reg)*: widen unsigned char to signed int (zero extend)*
	Ccall _putxn
;}
L80:
	release 4
	Cretn

_printf:
	reserve 2; save room for local variables
	pushr r0
	pushr r1
	pushr r6
	pushr r7
	reserve 4; save room for outgoing arguments
	st2 r12,'O',sp,(16); flag1 
	st2 r13,'O',sp,(18); flag2
;void printf(char *ptr,...){ //limited implementation of printf
;	int argslot=0;	//used to align longs
	ld2z R1
;	int * this=(int *)&ptr;
	ldA2 R6,'O',sp,(0+16); reg:addr
;	this++; argslot++; //advance argument pointer and slot #
	incm R6,2
	incm R1,1
	lbr L83
L82:
;    while(*ptr) {
;		c=*ptr++;
	ld2 R11,'O',sp,(0+16);reg:  INDIRP2(addr)
	cpy2 R10,R11
	incm R10,1
	st2 R10,'O',sp,(0+16); ASGNP2
	ldn1 R7,R11;reg:  INDIRU1(indaddr)
;		if (c!='%'){
	cpy1 R11,R7
	zExt R11 ;CVUI2(reg)*: widen unsigned char to signed int (zero extend)*
	jeqU2I r11,37,L85;EQI2(reg,con)
;			putc(c);
	cpy1 R12,R7
	zExt R12 ;CVUI2(reg)*: widen unsigned char to signed int (zero extend)*
	Ccall _putc
;		} else{
	lbr L86
L85:
;			c=*ptr++;
	ld2 R11,'O',sp,(0+16);reg:  INDIRP2(addr)
	cpy2 R10,R11
	incm R10,1
	st2 R10,'O',sp,(0+16); ASGNP2
	ldn1 R7,R11;reg:  INDIRU1(indaddr)
;			switch (c){
	cpy1 R0,R7
	zExt R0 ;CVUI2(reg)*: widen unsigned char to signed int (zero extend)*
	jcI2I r0,99,lbnf,L106  ;LT=lbnf i.e. subtract immedB from A and jump if borrow
	jnI2I r0,108,lbnf,L107; GT reverse  the subtraction
	cpy2 R11,R0
	shl2I R11,1
	ld2 R11,'O',R11,(L108-198);reg:  INDIRP2(addr)
	jumpv r11; JUMPV(reg)
L108:
	dw L92
	dw L90
	dw L87
	dw L101
	dw L87
	dw L87
	dw L90
	dw L87
	dw L87
	dw L94
L106:
	jeqU2I r0,88,L93;EQI2(reg,con)
	lbr L87
L107:
	ldaD R11,115; reg:acon
	jeqI2 r0,r11,L91; EQI2(reg,reg)
	jcI2 r0,r11,lbnf,L87; LT=lbnf i.e. subtract B from A and jump if borrow 
L110:
	jeqU2I r0,120,L93;EQI2(reg,con)
	lbr L87
L90:
;					printint(*this++);
	cpy2 R11,R6 ;LOADP2(reg)
	cpy2 R6,R11
	incm R6,2
	ld2 R12,'O',R11,0 ;reg:INDIRI2(addr)
	Ccall _printint
;					argslot+=1; //next argument slot
	incm R1,1
;					break;
	lbr L88
L91:
;					printstr((char*) *this++);
	cpy2 R11,R6 ;LOADP2(reg)
	cpy2 R6,R11
	incm R6,2
	ld2 R11,'O',R11,0 ;reg:INDIRI2(addr)
	cpy2 R12,R11 ;LOADP2(reg)
	Ccall _printstr
;					argslot+=1; //next argument slot
	incm R1,1
;					break;
	lbr L88
L92:
;					putc((unsigned int) *this++);
	cpy2 R11,R6 ;LOADP2(reg)
	cpy2 R6,R11
	incm R6,2
	ld2 R11,'O',R11,0 ;reg:INDIRI2(addr)
	cpy1 R12,R11
	zExt R12 ;CVUI2(reg)*: widen unsigned char to signed int (zero extend)*
	Ccall _putc
;					argslot+=1; //next argument slot
	incm R1,1
;					break;
	lbr L88
L93:
;					putx(((unsigned int) *this)>>8);
	ld2 R11,'O',R6,0 ;reg:INDIRI2(addr)
	shrU2I R11,8
	cpy1 R12,R11
	zExt R12 ;CVUI2(reg)*: widen unsigned char to signed int (zero extend)*
	Ccall _putx
;					putx(((unsigned int) *this++)&255);
	cpy2 R11,R6 ;LOADP2(reg)
	cpy2 R6,R11
	incm R6,2
	ld2 R11,'O',R11,0 ;reg:INDIRI2(addr)
	alu2I R11,R11,255,ani,ani ;removed copy;BANDU2(reg,con)  
	cpy1 R12,R11
	zExt R12 ;CVUI2(reg)*: widen unsigned char to signed int (zero extend)*
	Ccall _putx
;					argslot+=1; //next argument slot
	incm R1,1
;					break;
	lbr L88
L94:
;					if (*ptr){ //as long as there's something there
	ld2 R11,'O',sp,(0+16);reg:  INDIRP2(addr)
	ld1 R11,'O',R11,0
	zExt R11 ;CVUI2: widen unsigned char to signed int (zero extend)
	jzU2 r11,L95; EQ 0
;						xord=*ptr++;
	ld2 R11,'O',sp,(0+16);reg:  INDIRP2(addr)
	cpy2 R10,R11
	incm R10,1
	st2 R10,'O',sp,(0+16); ASGNP2
	ldn1 R11,R11;reg:  INDIRU1(indaddr)
	st1 R11,'O',sp,(-3+16); ASGNU1
;						if (argslot&1) {
	alu2I R11,R1,1,ani,ani
	;removed ?	cpy2 R11,R1
	jzU2 r11,L97; EQ 0
;							this++;
	incm R6,2
;							argslot++;
	incm R1,1
;						}
L97:
;						if(xord=='d'){
	ld1 R11,'O',sp,(-3+16)
	zExt R11 ;CVUI2: widen unsigned char to signed int (zero extend)
	jneU2I r11,100,L99; NE
;							printlint(*(long *)this);//treats "this" as a pointer to long
	ld4 Rp1p2,'O',R6,0;reg:  INDIRI4(addr)
	Ccall _printlint
;							this+=2;				// and advances it 4 bytes
	incm R6,4
;						} else{
	lbr L100
L99:
;							putx(((unsigned int) *this)>>8);
	ld2 R11,'O',R6,0 ;reg:INDIRI2(addr)
	shrU2I R11,8
	cpy1 R12,R11
	zExt R12 ;CVUI2(reg)*: widen unsigned char to signed int (zero extend)*
	Ccall _putx
;							putx(((unsigned int) *this++)&255);
	cpy2 R11,R6 ;LOADP2(reg)
	cpy2 R6,R11
	incm R6,2
	ld2 R11,'O',R11,0 ;reg:INDIRI2(addr)
	alu2I R11,R11,255,ani,ani ;removed copy;BANDU2(reg,con)  
	cpy1 R12,R11
	zExt R12 ;CVUI2(reg)*: widen unsigned char to signed int (zero extend)*
	Ccall _putx
;							putx(((unsigned int) *this)>>8);
	ld2 R11,'O',R6,0 ;reg:INDIRI2(addr)
	shrU2I R11,8
	cpy1 R12,R11
	zExt R12 ;CVUI2(reg)*: widen unsigned char to signed int (zero extend)*
	Ccall _putx
;							putx(((unsigned int) *this++)&255);
	cpy2 R11,R6 ;LOADP2(reg)
	cpy2 R6,R11
	incm R6,2
	ld2 R11,'O',R11,0 ;reg:INDIRI2(addr)
	alu2I R11,R11,255,ani,ani ;removed copy;BANDU2(reg,con)  
	cpy1 R12,R11
	zExt R12 ;CVUI2(reg)*: widen unsigned char to signed int (zero extend)*
	Ccall _putx
;						}
L100:
;						argslot+=2;
	incm R1,2
;						break;
	lbr L88
L95:
L101:
;					if (*ptr){ //as long as there's something there
	ld2 R11,'O',sp,(0+16);reg:  INDIRP2(addr)
	ld1 R11,'O',R11,0
	zExt R11 ;CVUI2: widen unsigned char to signed int (zero extend)
	jzU2 r11,L102; EQ 0
;						if (argslot&1) { //adjust alignment
	alu2I R11,R1,1,ani,ani
	;removed ?	cpy2 R11,R1
	jzU2 r11,L104; EQ 0
;							this++;
	incm R6,2
;							argslot++;
	incm R1,1
;						}
L104:
;						printflt(*(float *)this);//treats "this" as a pointer to float
	ld4 Rp1p2,'O',R6,0;INDIRF4(addr)
	Ccall _printflt
;						this+=2;				// and advances it 4 bytes
	incm R6,4
;						argslot+=2;
	incm R1,2
;						break;
	lbr L88
L102:
L87:
;					putc('%');putc(c);
	ldaD R12,37; reg:acon
	Ccall _putc
	cpy1 R12,R7
	zExt R12 ;CVUI2(reg)*: widen unsigned char to signed int (zero extend)*
	Ccall _putc
;			} //switch
L88:
;		} //%
L86:
;	} //while
L83:
;    while(*ptr) {
	ld2 R11,'O',sp,(0+16);reg:  INDIRP2(addr)
	ldn1 R11,R11;reg:  INDIRU1(indaddr)
	jnzU1 r11,L82; NEI2(CVUI2(reg),con0)
;} //prtf
L81:
	release 4; release room for outgoing arguments
	popr r7
	popr r6
	popr r1
	popr r0
	release 2; release room for local variables 
	Cretn

_exit:
	reserve 4
	st2 r12,'O',sp,(6); flag1 
;void exit(int code){
;	printf("exit %d\n",code);
	ldaD R12,L112; reg:acon
	ld2 R13,'O',sp,(0+6) ;reg:INDIRI2(addr)
	Ccall _printf
L113:
;	while(1);
L114:
	lbr L113
;}
L111:
	release 4
	Cretn

_memcmp:
	pushr r0
	pushr r1
	pushr r6
	pushr r7
	ld2 R7,'O',sp,(4+10); reg:INDIRU2(addr)
;int memcmp(const void *Ptr1, const void *Ptr2, unsigned int Count){
;    int v = 0;
	ld2z R6
;    p1 = (unsigned char *)Ptr1;
	cpy2 R1,R12 ;LOADP2(reg)
;    p2 = (unsigned char *)Ptr2;
	cpy2 R0,R13 ;LOADP2(reg)
	lbr L118
L117:
;    while(Count-- > 0 && v == 0) {
;        v = *(p1++) - *(p2++);
	cpy2 R11,R1 ;LOADP2(reg)
	cpy2 R1,R11
	incm R1,1
	cpy2 R10,R0 ;LOADP2(reg)
	cpy2 R0,R10
	incm R0,1
	ld1 R11,'O',R11,0
	zExt R11 ;CVUI2: widen unsigned char to signed int (zero extend)
	ld1 R10,'O',R10,0
	zExt R10 ;CVUI2: widen unsigned char to signed int (zero extend)
	alu2 R6,R11,R10,sm,smb
;    }
L118:
;    while(Count-- > 0 && v == 0) {
	cpy2 R11,R7 ;LOADU2(reg)*
	cpy2 R7,R11
	decm R7,1
	jzU2 r11,L120; EQ 0
	jzU2 r6,L117; EQ 0
L120:
;    return v;
	cpy2 R15,R6 ;LOADI2(reg)*
L116:
	popr r7
	popr r6
	popr r1
	popr r0
	Cretn

_memcpy:
	pushr r1
	pushr r6
	pushr r7
	ld2 R7,'O',sp,(4+8); reg:INDIRU2(addr)
;void* memcpy(void* dest, const void* src, unsigned int count) {
;        char* dst8 = (char*)dest;
	cpy2 R6,R12 ;LOADP2(reg)
;        char* src8 = (char*)src;
	cpy2 R1,R13 ;LOADP2(reg)
	lbr L123
L122:
;        while (count--) {
;            *dst8++ = *src8++;
	cpy2 R11,R6 ;LOADP2(reg)
	cpy2 R6,R11
	incm R6,1
	cpy2 R10,R1 ;LOADP2(reg)
	cpy2 R1,R10
	incm R1,1
	ldn1 R10,R10;reg:  INDIRU1(indaddr)
	str1 R10,R11; ASGNU1(indaddr,reg)
;        }
L123:
;        while (count--) {
	cpy2 R11,R7 ;LOADU2(reg)*
	cpy2 R7,R11
	decm R7,1
	jnzU2 r11,L122; NE 0
;        return dest;
	cpy2 R15,R12 ;LOADP2(reg)
L121:
	popr r7
	popr r6
	popr r1
	Cretn

_memset:
	pushr r6
	pushr r7
	ld2 R7,'O',sp,(4+6); reg:INDIRU2(addr)
;{
;    unsigned char* p=s;
	cpy2 R6,R12 ;LOADP2(reg)
	lbr L127
L126:
;        *p++ = (unsigned char)c;
	cpy2 R11,R6 ;LOADP2(reg)
	cpy2 R6,R11
	incm R6,1
	cpy2 R10,R13 ;LOADU2(reg)*
	str1 R10,R11; ASGNU1(indaddr,reg)
L127:
;    while(n--)
	cpy2 R11,R7 ;LOADU2(reg)*
	cpy2 R7,R11
	decm R7,1
	jnzU2 r11,L126; NE 0
;    return s;
	cpy2 R15,R12 ;LOADP2(reg)
L125:
	popr r7
	popr r6
	Cretn

_x:
	db 16 dup (0); zerofill global
_rows:
	db 16 dup (0); zerofill global
_down:
	db 30 dup (0); zerofill global
_up:
	db 30 dup (0); zerofill global
L112:
	db 101
	db 120
	db 105
	db 116
	db 32
	db 37
	db 100
	db 10
	db 0
	align 4
L50:
	dd 0xbf800000
	align 4
L49:
	dd 0x0
L32:
	db 10
	db 0
L31:
	db 37
	db 99
	db 0
L13:
	db 84
	db 111
	db 111
	db 107
	db 32
	db 97
	db 98
	db 111
	db 117
	db 116
	db 32
	db 49
	db 55
	db 32
	db 115
	db 101
	db 99
	db 32
	db 111
	db 110
	db 32
	db 49
	db 46
	db 54
	db 109
	db 104
	db 122
	db 32
	db 101
	db 108
	db 102
	db 32
	db 119
	db 105
	db 116
	db 104
	db 32
	db 83
	db 116
	db 32
	db 74
	db 117
	db 100
	db 121
	db 39
	db 115
	db 32
	db 99
	db 111
	db 109
	db 112
	db 105
	db 108
	db 101
	db 114
	db 40
	db 102
	db 114
	db 111
	db 109
	db 32
	db 114
	db 117
	db 110
	db 49
	db 56
	db 48
	db 50
	db 32
	db 116
	db 111
	db 32
	db 57
	db 50
	db 110
	db 100
	db 32
	db 115
	db 111
	db 108
	db 117
	db 116
	db 105
	db 111
	db 110
	db 41
	db 10
	db 0
L12:
	db 84
	db 111
	db 111
	db 107
	db 32
	db 97
	db 98
	db 111
	db 117
	db 116
	db 32
	db 50
	db 50
	db 32
	db 115
	db 101
	db 99
	db 32
	db 111
	db 110
	db 32
	db 49
	db 46
	db 54
	db 109
	db 104
	db 122
	db 32
	db 101
	db 108
	db 102
	db 32
	db 119
	db 105
	db 116
	db 104
	db 32
	db 66
	db 105
	db 114
	db 116
	db 104
	db 100
	db 97
	db 121
	db 32
	db 99
	db 111
	db 109
	db 112
	db 105
	db 108
	db 101
	db 114
	db 10
	db 0
L11:
	db 46
	db 46
	db 46
	db 97
	db 110
	db 100
	db 46
	db 46
	db 46
	db 10
	db 56
	db 52
	db 49
	db 51
	db 54
	db 50
	db 55
	db 53
	db 32
	db 105
	db 115
	db 32
	db 108
	db 97
	db 115
	db 116
	db 10
	db 84
	db 111
	db 111
	db 107
	db 32
	db 97
	db 98
	db 111
	db 117
	db 116
	db 32
	db 50
	db 53
	db 32
	db 115
	db 101
	db 99
	db 32
	db 111
	db 110
	db 32
	db 49
	db 46
	db 54
	db 109
	db 104
	db 122
	db 32
	db 101
	db 108
	db 102
	db 32
	db 119
	db 105
	db 116
	db 104
	db 32
	db 67
	db 104
	db 114
	db 105
	db 115
	db 116
	db 109
	db 97
	db 115
	db 32
	db 99
	db 111
	db 109
	db 112
	db 105
	db 108
	db 101
	db 114
	db 10
	db 0
L2:
	db 103
	db 101
	db 110
	db 101
	db 114
	db 97
	db 116
	db 101
	db 115
	db 32
	db 57
	db 50
	db 32
	db 115
	db 111
	db 108
	db 117
	db 116
	db 105
	db 111
	db 110
	db 115
	db 46
	db 32
	db 10
	db 49
	db 53
	db 56
	db 54
	db 51
	db 55
	db 50
	db 52
	db 32
	db 105
	db 115
	db 32
	db 102
	db 105
	db 114
	db 115
	db 116
	db 10
	db 10
	db 0
	include lcc1802Epilofl.inc
	include LCC1802fp.inc
	include IO1802.inc
