; generated by lcc-xr18NW $Version: 5.0 - XR18NW $ on Thu Nov 23 08:43:24 2017

SP:	equ	2 ;stack pointer
memAddr: equ	14
retAddr: equ	6
retVal:	equ	15
regArg1: equ	12
regArg2: equ	13
	listing off
	include lcc1802proloNW.inc
	listing on
_PIN4:
	db 0
;$$function start$$ _main
_main:		;framesize=8
	reserve 6
;void main(){
;	unsigned char cin='?';
	ldA2 R11,'O',sp,(5+1); reg:addr
	str1I 63,R11; ASGNU1(indaddr,acon)	DH
;	asm(" seq\n"); //make sure Q is high to start
;		printf("UUUUUUUU");
 seq
	ldaD R12,L3; reg:acon
	Ccall _printf
L4:
;	while(1);
L5:
	lbr L4
;}
L1:
	release 6
	Cretn

;$$function end$$ _main
;$$function start$$ _includeser
_includeser:		;framesize=2
;void includeser(){
;	asm("BAUDRATE EQU 2400\n");
;	asm(" include VELFserial3.inc\n");
BAUDRATE EQU 2400
 include VELFserial3.inc
;}
L7:
	Cretn

;$$function end$$ _includeser
;$$function start$$ _delay
_delay:		;framesize=10
	pushr R6
	pushr R7
	reserve 4; save room for outgoing arguments
	cpy2 R7,R12; function(2054) 1
;void delay(unsigned int howlong){
;	for (i=1;i!=howlong;i++){
	ldaD R6,1; reg:acon
	lbr L14
L11:
;		oneMs();
	Ccall _oneMs; CALLI2(ar)
;	}
L12:
;	for (i=1;i!=howlong;i++){
	incm R6,1
L14:
	jneU2 R6,R7,L11; NE
;}
L10:
	release 4; release room for outgoing arguments
	popr R7
	popr R6
	Cretn

;$$function end$$ _delay
;$$function start$$ _olduinoincluder
_olduinoincluder:		;framesize=2
;void olduinoincluder(){
;	asm("\tinclude olduino.inc\n");
	include olduino.inc
;}
L15:
	Cretn

;$$function end$$ _olduinoincluder
;$$function start$$ _strncmp
_strncmp:		;framesize=6
	pushr R6
	pushr R7
	ld2 R7,'O',sp,(10+1) ;reg:INDIRU2(addr)
;{
;    for ( ; n > 0; s1++, s2++, --n)
	lbr L21
L18:
;	if (*s1 != *s2)
	ld1 R11,'O',R12,0
	zExt R11 ;CVUI2: widen unsigned char to signed int (zero extend)
	ld1 R10,'O',R13,0
	zExt R10 ;CVUI2: widen unsigned char to signed int (zero extend)
	jeqI2 R11,R10,L22; EQI2(reg,reg)
;	    return ((*(unsigned char *)s1 < *(unsigned char *)s2) ? -1 : +1);
	ld1 R11,'O',R12,0
	zExt R11 ;CVUI2: widen unsigned char to signed int (zero extend)
	ld1 R10,'O',R13,0
	zExt R10 ;CVUI2: widen unsigned char to signed int (zero extend)
	jcI2 R11,R10,lbdf,L25; GE is flipped test from LT
	ldaD R6,-1; reg:acon
	lbr L26
L25:
	ldaD R6,1; reg:acon
L26:
	cpy2 R15,R6 ;LOADI2(reg)
	lbr L17
L22:
;	else if (*s1 == '\0')
	ldn1 R11,R12;reg:  INDIRU1(indaddr)
	jnzU1 R11,L27; NEI2(CVUI2(reg),con0)
;	    return 0;
	ld2z R15
	lbr L17
L27:
L19:
;    for ( ; n > 0; s1++, s2++, --n)
	incm R12,1
	incm R13,1
	decm R7,1	;SUBU2(reg,consm)
L21:
	jnzU2 R7,L18; NE 0 
;    return 0;
	ld2z R15
L17:
	popr R7
	popr R6
	Cretn

;$$function end$$ _strncmp
;$$function start$$ _strlen
_strlen:		;framesize=4
	pushr R7
;{
;	unsigned int slen = 0 ;
	ld2z R7
	lbr L31
L30:
;	while (*str != 0) {
;      slen++ ;
	incm R7,1
;      str++ ;
	incm R12,1
;   }
L31:
;	while (*str != 0) {
	ldn1 R11,R12;reg:  INDIRU1(indaddr)
	jnzU1 R11,L30; NEI2(CVUI2(reg),con0)
;   return slen;
	cpy2 R15,R7 ;LOADU2*(reg)
L29:
	popr R7
	Cretn

;$$function end$$ _strlen
;$$function start$$ _printstr
_printstr:		;framesize=8
	pushr R7
	reserve 4; save room for outgoing arguments
	cpy2 R7,R12; function(2055) 1
;void printstr(char *ptr){
	lbr L35
L34:
;    while(*ptr){
;		putc(*ptr++); //jan 29
	cpy2 R11,R7 ;LOADP2(reg)
	cpy2 R7,R11
	incm R7,1
	ld1 R12,'O',R11,0
	zExt R12 ;CVUI2: widen unsigned char to signed int (zero extend)
	Ccall _putcser
;		asm(" nop1806\n nop1806\n nop1806\n"); //17-03-09
 nop1806
 nop1806
 nop1806
;	}
L35:
;    while(*ptr){
	ldn1 R11,R7;reg:  INDIRU1(indaddr)
	jnzU1 R11,L34; NEI2(CVUI2(reg),con0)
;}
L33:
	release 4; release room for outgoing arguments
	popr R7
	Cretn

;$$function end$$ _printstr
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
;$$function start$$ _ftoa
_ftoa:		;framesize=80
	reserve 62; save room for local variables
	pushr R0
	pushr R1
	pushr R6
	pushr R7
	reserve 8; save room for outgoing arguments
	cpy4 RL6,RL12; halfbaked&floaty
	ld2 R1,'O',sp,(86+1) ;reg:INDIRU2(addr)
;{
;   char *output = outbfr ;
	ld2 R0,'O',sp,(84+1) ;reg:INDIRP2(addr)
;   if (flt < 0.0) {
	ld4 RL10,'D',(L41),0;INDIRF4(addr)
	jcF4 RL6,RL10,lbdf,L39;GEF4(reg,reg) - reverse test
;      *output++ = '-' ;
	cpy2 R11,R0 ;LOADP2(reg)
	cpy2 R0,R11
	incm R0,1
	str1I 45,R11; ASGNU1(indaddr,acon)	DH
;      flt *= -1.0 ;
	ld4 RL8,'D',(L42),0;INDIRF4(addr)
	cpy4 RL10,RL6; LOADU4(reg)
	Ccall fp_mul ;MULF4(reg,reg)
	cpy4 RL6,RL8; LOADU4(reg)
;   } else {
	lbr L40
L39:
;      if (use_leading_plus) {
	lbr L43
;         *output++ = '+' ;
	cpy2 R11,R0 ;LOADP2(reg)
	cpy2 R0,R11
	incm R0,1
	str1I 43,R11; ASGNU1(indaddr,acon)	DH
;      }
L43:
;   }
L40:
;   if (dec_digits < 8) {
	jcI2I R1,8,lbdf,L45; GE is flipped test from LT
;      flt += round_nums[dec_digits] ;
	cpy4 RL8,RL6; LOADU4(reg)
	cpy2 R11,R1
	shl2I R11,2
	ld4 RL10,'O',R11,(_round_nums);INDIRF4(addr)
	Ccall fp_add ;ADDF4(reg,reg)
	cpy4 RL6,RL8; LOADU4(reg)
;   }
L45:
;	mult=mult_nums[dec_digits];
	cpy2 R11,R1
	shl2I R11,2
	ld4 RL10,'O',R11,(_mult_nums);INDIRF4(addr)
	st4 RL10,'O',sp,(68+1); ASGNF4(addr,reg)
;   wholeNum = flt;
	cpy4 RL8,RL6; LOADU4(reg)
	ccall cvfi4; CVFI4(reg) convert float to long
	st4 RL8,'O',sp,(72+1)
;   decimalNum = ((flt - wholeNum) * mult);
	cpy4 RL8,RL6; LOADU4(reg)
	st4 RL8,'O',sp,(16+1); ASGNF4(addr,reg)
	ld4 RL8,'O',sp,(72+1);reg:  INDIRI4(addr)
	Ccall cvif4; emit2
	cpy4 RL10,RL8; LOADU4(reg)
	ld4 RL8,'O',sp,(16+1);INDIRF4(addr)
	Ccall fp_sub ;SUBF4(reg,reg)
	ld4 RL10,'O',sp,(68+1);INDIRF4(addr)
	Ccall fp_mul ;MULF4(reg,reg)
	ccall cvfi4; CVFI4(reg) convert float to long
	st4 RL8,'O',sp,(64+1)
;   strcpy(output,dubdabx(wholeNum,output,1));
	ld4 Rp1p2,'O',sp,(72+1);reg:  INDIRI4(addr)
	st2 R0,'O',sp,(4+1); arg+f**
	ldaD R11,1; reg:acon
	st2 R11,'O',sp,(6+1); arg+f**
	Ccall _dubdabx
	cpy2 R11,R15 ;LOADP2(reg)
	cpy2 R12,R0 ;LOADP2(reg)
	cpy2 R13,R11 ;LOADP2(reg)
	Ccall _strcpy
;   output+=strlen(output);
	cpy2 R12,R0 ;LOADP2(reg)
	Ccall _strlen;CALLU2(ar)*
	alu2 R0,R15,R0,add,adc	;ADDP2(reg,reg)
;   if (dec_digits > 0) {
	jzU2 R1,L47; EQ 0
;		*output++ = '.' ;
	cpy2 R11,R0 ;LOADP2(reg)
	cpy2 R0,R11
	incm R0,1
	str1I 46,R11; ASGNU1(indaddr,acon)	DH
;		strcpy(output,dubdabx(decimalNum,output,dec_digits));
	ld4 Rp1p2,'O',sp,(64+1);reg:  INDIRI4(addr)
	st2 R0,'O',sp,(4+1); arg+f**
	cpy2 R11,R1 ;LOADI2(reg)
	st2 R11,'O',sp,(6+1); arg+f**
	Ccall _dubdabx
	cpy2 R11,R15 ;LOADP2(reg)
	cpy2 R12,R0 ;LOADP2(reg)
	cpy2 R13,R11 ;LOADP2(reg)
	Ccall _strcpy
;	}
L47:
;   return outbfr;
	ld2 R15,'O',sp,(84+1) ;reg:INDIRP2(addr)
L38:
	release 8; release room for outgoing arguments
	popr R7
	popr R6
	popr R1
	popr R0
	release 62; release room for local variables 
	Cretn

;$$function end$$ _ftoa
;$$function start$$ _itoa
_itoa:		;framesize=16
	reserve 2; save room for local variables
	pushr R0
	pushr R1
	pushr R6
	pushr R7
	reserve 4; save room for outgoing arguments
	st2 R12,'O',sp,(16+1); flag1 
	st2 R13,'O',sp,(18+1); flag1 
;char * itoa(int s, char *buffer){ //convert an integer to printable ascii in a buffer supplied by the caller
;	unsigned int flag=0;
	ld2z R0
;	char * bptr; bptr=buffer;
	ld2 R11,'O',sp,(18+1) ;reg:INDIRP2(addr)
	st2 R11,'O',sp,(12+1); ASGNP2(addr,reg)
;	if (s<0){
	ld2 R11,'O',sp,(16+1) ;reg:INDIRI2(addr)
	jcI2I R11,0,lbdf,L50; GE is flipped test from LT
;		*bptr='-';bptr++;
	ld2 R11,'O',sp,(12+1) ;reg:INDIRP2(addr)
	str1I 45,R11; ASGNU1(indaddr,acon)	DH
	ld2 R11,'O',sp,(12+1) ;reg:INDIRP2(addr)
	incm R11,1
	st2 R11,'O',sp,(12+1); ASGNP2(addr,reg)
;		n=-s;
	ld2 R11,'O',sp,(16+1) ;reg:INDIRI2(addr)
	negI2 R11,R11 ;was alu2I R11,R11,0,sdi,sdbi
	cpy2 R6,R11 ;LOADU2*(reg)
;	} else{
	lbr L51
L50:
;		n=s;
	ld2 R11,'O',sp,(16+1) ;reg:INDIRI2(addr)
	cpy2 R6,R11 ;LOADU2*(reg)
;	}
L51:
;	k=10000;
	ldaD R7,10000; reg:acon
	lbr L53
L52:
;	while(k>0){
;		for(r=0;k<=n;r++,n-=k); // was r=n/k
	ld2z R1
	lbr L58
L55:
L56:
	incm R1,1
	alu2 R6,R6,R7,sm,smb
L58:
	jcU2 R6,R7,lbdf,L55 ;LE is flipped test & operands
;		if (flag || r>0||k==1){
	jnzU2 R0,L62; NE 0 
	jnzU2 R1,L62; NE 0 
	jneU2I R7,1,L59; NE
L62:
;			*bptr=('0'+r);bptr++;
	ld2 R11,'O',sp,(12+1) ;reg:INDIRP2(addr)
	ldA2 R10,'O',R1,(48); reg:addr
	str1 R10,R11; ASGNU1(indaddr,reg)		DH
	ld2 R11,'O',sp,(12+1) ;reg:INDIRP2(addr)
	incm R11,1
	st2 R11,'O',sp,(12+1); ASGNP2(addr,reg)
;			flag='y';
	ldaD R0,121; reg:acon
;		}
L59:
;		k=k/10;
	cpy2 R12,R7 ;LOADU2*(reg)
	ldaD R13,10; reg:acon
	Ccall _divu2
	cpy2 R7,R15 ;LOADU2*(reg)
;	}
L53:
;	while(k>0){
	jnzU2 R7,L52; NE 0 
;	*bptr='\0';
	ld2 R11,'O',sp,(12+1) ;reg:INDIRP2(addr)
	str1I 0,R11; ASGNU1(indaddr,acon)	DH
;	return buffer;
	ld2 R15,'O',sp,(18+1) ;reg:INDIRP2(addr)
L49:
	release 4; release room for outgoing arguments
	popr R7
	popr R6
	popr R1
	popr R0
	release 2; release room for local variables 
	Cretn

;$$function end$$ _itoa
;$$function start$$ _ltoa
_ltoa:		;framesize=16
	pushr R1
	pushr R6
	pushr R7
	reserve 8; save room for outgoing arguments
	cpy4 RL6,RL12; halfbaked
;char * ltoa(long s, char *buffer){ //convert a long integer to printable ascii in a buffer supplied by the caller
;	char* bptr=buffer;
	ld2 R1,'O',sp,(20+1) ;reg:INDIRP2(addr)
;	if (s<0){
	ldI4 RL10,0 ;loading a long integer constant
	jcI4 RL6,RL10,lbdf,L64; GE is flipped test from LT
;		*bptr++='-';
	cpy2 R11,R1 ;LOADP2(reg)
	cpy2 R1,R11
	incm R1,1
	str1I 45,R11; ASGNU1(indaddr,acon)	DH
;		s=-s;
	negI4 RL6,RL6 ;was alu4I RL6,RL6,0,sdi,sdbi
;	}
L64:
;	strcpy(bptr,dubdabx(s,bptr,1)); //uses assembler double-dabble routine
	cpy4 Rp1p2,RL6; LOADI4*
	st2 R1,'O',sp,(4+1); arg+f**
	ldaD R11,1; reg:acon
	st2 R11,'O',sp,(6+1); arg+f**
	Ccall _dubdabx
	cpy2 R11,R15 ;LOADP2(reg)
	cpy2 R12,R1 ;LOADP2(reg)
	cpy2 R13,R11 ;LOADP2(reg)
	Ccall _strcpy
;	return buffer;
	ld2 R15,'O',sp,(20+1) ;reg:INDIRP2(addr)
L63:
	release 8; release room for outgoing arguments
	popr R7
	popr R6
	popr R1
	Cretn

;$$function end$$ _ltoa
;$$function start$$ _printint
_printint:		;framesize=14
	reserve 12
	st2 R12,'O',sp,(14+1); flag1 
;void printint(int s){ //print an integer
;	itoa(s,buffer);
	ld2 R12,'O',sp,(14+1) ;reg:INDIRI2(addr)
	ldA2 R13,'O',sp,(4+1); reg:addr
	Ccall _itoa
;	printstr(buffer);
	ldA2 R12,'O',sp,(4+1); reg:addr
	Ccall _printstr
;}
L66:
	release 12
	Cretn

;$$function end$$ _printint
;$$function start$$ _printlint
_printlint:		;framesize=20
	reserve 18
	st2 R12,'O',sp,(20+1); flag1 
	st2 R13,'O',sp,(22+1); flag1 
;void printlint(long s){ //print a long integer
;	printstr(ltoa(s,buffer));
	ld4 Rp1p2,'O',sp,(20+1);reg:  INDIRI4(addr)
	ldA2 R11,'O',sp,(6+1); reg:addr
	st2 R11,'O',sp,(4+1); arg+f**
	Ccall _ltoa
	cpy2 R11,R15 ;LOADP2(reg)
	cpy2 R12,R11 ;LOADP2(reg)
	Ccall _printstr
;}
L67:
	release 18
	Cretn

;$$function end$$ _printlint
;$$function start$$ _printflt
_printflt:		;framesize=30
	reserve 28
	st2 R12,'O',sp,(30+1); flag1 
	st2 R13,'O',sp,(32+1); flag1 
;void printflt(float s){ //print a float
;	printstr(ftoa(s,buffer,3));
	ld4 Rp1p2,'O',sp,(30+1);INDIRF4(addr)
	ldA2 R11,'O',sp,(8+1); reg:addr
	st2 R11,'O',sp,(4+1); arg+f**
	ldaD R11,3; reg:acon
	st2 R11,'O',sp,(6+1); arg+f**
	Ccall _ftoa
	cpy2 R11,R15 ;LOADP2(reg)
	cpy2 R12,R11 ;LOADP2(reg)
	Ccall _printstr
;}
L68:
	release 28
	Cretn

;$$function end$$ _printflt
;$$function start$$ _putxn
_putxn:		;framesize=6
	reserve 4
	st2 R12,'O',sp,(6+1); flag1 
	ldA2 R11,'O',sp,(6+1); reg:addr
	ld2 R10,'O',sp,(6+1) ;reg:INDIRI2(addr)
	str1 R10,R11; ASGNU1(indaddr,reg)		DH
;void putxn(unsigned char x){ //print a nibble as ascii hex
;	if (x<10){
	ld1 R11,'O',sp,(6+1)
	zExt R11 ;CVUI2: widen unsigned char to signed int (zero extend)
	jcI2I R11,10,lbdf,L70; GE is flipped test from LT
;		putc(x+'0');
	ld1 R11,'O',sp,(6+1)
	zExt R11 ;CVUI2: widen unsigned char to signed int (zero extend)
	ldA2 R11,'O',R11,(48); reg:addr
	cpy1 R12,R11
	zExt R12 ;CVUI2(reg)*: widen unsigned char to signed int (zero extend)*
	Ccall _putcser
;	} else {
	lbr L71
L70:
;		putc(x+'A'-10);
	ld1 R11,'O',sp,(6+1)
	zExt R11 ;CVUI2: widen unsigned char to signed int (zero extend)
	ldA2 R11,'O',R11,(65); reg:addr
	alu2I R11,R11,10,smi,smbi
	cpy1 R12,R11
	zExt R12 ;CVUI2(reg)*: widen unsigned char to signed int (zero extend)*
	Ccall _putcser
;	}
L71:
;}
L69:
	release 4
	Cretn

;$$function end$$ _putxn
;$$function start$$ _putx
_putx:		;framesize=6
	reserve 4
	st2 R12,'O',sp,(6+1); flag1 
	ldA2 R11,'O',sp,(6+1); reg:addr
	ld2 R10,'O',sp,(6+1) ;reg:INDIRI2(addr)
	str1 R10,R11; ASGNU1(indaddr,reg)		DH
;void putx(unsigned char x){ //print a unsigned char as ascii hex
;	putxn(x>>4);
	ld1 R11,'O',sp,(6+1)
	zExt R11 ;CVUI2: widen unsigned char to signed int (zero extend)
	shrI2I R11,4
	cpy1 R12,R11
	zExt R12 ;CVUI2(reg)*: widen unsigned char to signed int (zero extend)*
	Ccall _putxn
;	putxn(x & 0x0F);
	ld1 R11,'O',sp,(6+1)
	zExt R11 ;CVUI2: widen unsigned char to signed int (zero extend)
	alu2I R11,R11,15,ani,ani
	;removed ?	cpy2 R11,R11
	cpy1 R12,R11
	zExt R12 ;CVUI2(reg)*: widen unsigned char to signed int (zero extend)*
	Ccall _putxn
;}
L72:
	release 4
	Cretn

;$$function end$$ _putx
;$$function start$$ _printf
_printf:		;framesize=18
	reserve 4; save room for local variables
	pushr R0
	pushr R1
	pushr R6
	pushr R7
	reserve 4; save room for outgoing arguments
	st2 R12,'O',sp,(18+1); flag1 
	st2 R13,'O',sp,(20+1); flag2
;void printf(char *pptr,...){ //limited implementation of printf
;	register char* ptr=pptr; //try to save on loads/spills
	ld2 R7,'O',sp,(18+1) ;reg:INDIRP2(addr)
;	int argslot=0;	//used to align longs
	ld2z R0
;	int * this=(int *)&pptr;
	ldA2 R1,'O',sp,(18+1); reg:addr
;	this++; argslot++; //advance argument pointer and slot #
	incm R1,2
	incm R0,1
	lbr L75
L74:
;    while(*ptr) {
;		c=*ptr; ptr++;
	ldn1 R6,R7;reg:  INDIRU1(indaddr)
	incm R7,1
;		if (c!='%'){
	jeqU1I R6,37,L77;EQI2(CVUI2(reg),con8bit)**
;			putc(c);
	cpy1 R12,R6
	zExt R12 ;CVUI2(reg)*: widen unsigned char to signed int (zero extend)*
	Ccall _putcser
;			asm(" nop1806\n nop1806\n nop1806\n"); //17-03-13
 nop1806
 nop1806
 nop1806
;		} else{
	lbr L78
L77:
;			c=*ptr;ptr++;
	ldn1 R6,R7;reg:  INDIRU1(indaddr)
	incm R7,1
;			switch (c){
	cpy1 R11,R6
	zExt R11 ;CVUI2(reg)*: widen unsigned char to signed int (zero extend)*
	st2 R11,'O',sp,(13+1); ASGNI2(addr,reg)
	ld2 R11,'O',sp,(13+1) ;reg:INDIRI2(addr)
	jcI2I R11,99,lbnf,L101  ;LT=lbnf i.e. subtract immedB from A and jump if borrow
	jnI2I R11,108,lbnf,L102; GT reverse  the subtraction
	shl2I R11,1
	ld2 R11,'O',R11,(L103-198) ;reg:INDIRP2(addr)
	jumpv R11; JUMPV(reg)
L103:
	dw L85
	dw L83
	dw L80
	dw L96
	dw L80
	dw L80
	dw L83
	dw L80
	dw L80
	dw L89
L101:
	ld2 R11,'O',sp,(13+1) ;reg:INDIRI2(addr)
	jeqU2I R11,88,L88;EQI2(reg,con)
	lbr L80
L102:
	ld2 R11,'O',sp,(13+1) ;reg:INDIRI2(addr)
	ldaD R10,115; reg:acon
	jeqI2 R11,R10,L84; EQI2(reg,reg)
	jcI2 R11,R10,lbnf,L80; LT=lbnf i.e. subtract B from A and jump if borrow 
L105:
	ld2 R11,'O',sp,(13+1) ;reg:INDIRI2(addr)
	jeqU2I R11,120,L88;EQI2(reg,con)
	lbr L80
L83:
;					printint(*this++);
	cpy2 R11,R1 ;LOADP2(reg)
	cpy2 R1,R11
	incm R1,2
	ld2 R12,'O',R11,0 ;reg:INDIRI2(addr)
	Ccall _printint
;					argslot+=1; //next argument slot
	incm R0,1
;					break;
	lbr L81
L84:
;					printstr((char*) *this++);
	cpy2 R11,R1 ;LOADP2(reg)
	cpy2 R1,R11
	incm R1,2
	ld2 R11,'O',R11,0 ;reg:INDIRI2(addr)
	cpy2 R12,R11 ;LOADP2(reg)
	Ccall _printstr
;					argslot+=1; //next argument slot
	incm R0,1
;					break;
	lbr L81
L85:
;					if (*ptr=='x'){ //if there's an x
	ldn1 R11,R7;reg:  INDIRU1(indaddr)
	jneU1I R11,120,L86	; DH 4
;						ptr++; //skip over the x
	incm R7,1
;						putx(((unsigned int) *this++)&255); //print 1 byte as hex
	cpy2 R11,R1 ;LOADP2(reg)
	cpy2 R1,R11
	incm R1,2
	ld2 R11,'O',R11,0 ;reg:INDIRI2(addr)
	alu2I R11,R11,255,ani,ani ;removed copy;BANDU2(reg,con)  
	cpy1 R12,R11
	zExt R12 ;CVUI2(reg)*: widen unsigned char to signed int (zero extend)*
	Ccall _putx
;					} else{
	lbr L87
L86:
;						putc((unsigned int) *this++);		//print as char
	cpy2 R11,R1 ;LOADP2(reg)
	cpy2 R1,R11
	incm R1,2
	ld2 R11,'O',R11,0 ;reg:INDIRI2(addr)
	cpy1 R12,R11
	zExt R12 ;CVUI2(reg)*: widen unsigned char to signed int (zero extend)*
	Ccall _putcser
;					}
L87:
;					argslot+=1; //next argument slot
	incm R0,1
;					break;
	lbr L81
L88:
;					putx(((unsigned int) *this)>>8);
	ld2 R11,'O',R1,0 ;reg:INDIRI2(addr)
	shrU2I R11,8
	cpy1 R12,R11
	zExt R12 ;CVUI2(reg)*: widen unsigned char to signed int (zero extend)*
	Ccall _putx
;					putx(((unsigned int) *this++)&255);
	cpy2 R11,R1 ;LOADP2(reg)
	cpy2 R1,R11
	incm R1,2
	ld2 R11,'O',R11,0 ;reg:INDIRI2(addr)
	alu2I R11,R11,255,ani,ani ;removed copy;BANDU2(reg,con)  
	cpy1 R12,R11
	zExt R12 ;CVUI2(reg)*: widen unsigned char to signed int (zero extend)*
	Ccall _putx
;					argslot+=1; //next argument slot
	incm R0,1
;					break;
	lbr L81
L89:
;					if (*ptr){ //as long as there's something there
	ld1 R11,'O',R7,0
	zExt R11 ;CVUI2: widen unsigned char to signed int (zero extend)
	jzU2 R11,L90; EQ 0
;						xord=*ptr++;
	cpy2 R11,R7 ;LOADP2(reg)
	cpy2 R7,R11
	incm R7,1
	ldA2 R10,'O',sp,(15+1); reg:addr
	ldn1 R11,R11;reg:  INDIRU1(indaddr)
	str1 R11,R10; ASGNU1(indaddr,reg)		DH
;						if (argslot&1) {
	alu2I R11,R0,1,ani,ani
	;removed ?	cpy2 R11,R0
	jzU2 R11,L92; EQ 0
;							this++;
	incm R1,2
;							argslot++;
	incm R0,1
;						}
L92:
;						if(xord=='d'){
	ld1 R11,'O',sp,(15+1)
	jneU1I R11,100,L94	; DH 4
;							printlint(*(long *)this);//treats "this" as a pointer to long
	ld4 Rp1p2,'O',R1,0;reg:  INDIRI4(addr)
	Ccall _printlint
;							this+=2;				// and advances it 4 bytes
	incm R1,4
;						} else{
	lbr L95
L94:
;							putx(((unsigned int) *this)>>8);
	ld2 R11,'O',R1,0 ;reg:INDIRI2(addr)
	shrU2I R11,8
	cpy1 R12,R11
	zExt R12 ;CVUI2(reg)*: widen unsigned char to signed int (zero extend)*
	Ccall _putx
;							putx(((unsigned int) *this++)&255);
	cpy2 R11,R1 ;LOADP2(reg)
	cpy2 R1,R11
	incm R1,2
	ld2 R11,'O',R11,0 ;reg:INDIRI2(addr)
	alu2I R11,R11,255,ani,ani ;removed copy;BANDU2(reg,con)  
	cpy1 R12,R11
	zExt R12 ;CVUI2(reg)*: widen unsigned char to signed int (zero extend)*
	Ccall _putx
;							putx(((unsigned int) *this)>>8);
	ld2 R11,'O',R1,0 ;reg:INDIRI2(addr)
	shrU2I R11,8
	cpy1 R12,R11
	zExt R12 ;CVUI2(reg)*: widen unsigned char to signed int (zero extend)*
	Ccall _putx
;							putx(((unsigned int) *this++)&255);
	cpy2 R11,R1 ;LOADP2(reg)
	cpy2 R1,R11
	incm R1,2
	ld2 R11,'O',R11,0 ;reg:INDIRI2(addr)
	alu2I R11,R11,255,ani,ani ;removed copy;BANDU2(reg,con)  
	cpy1 R12,R11
	zExt R12 ;CVUI2(reg)*: widen unsigned char to signed int (zero extend)*
	Ccall _putx
;						}
L95:
;						argslot+=2;
	incm R0,2
;						break;
	lbr L81
L90:
L96:
;					if (*ptr){ //as long as there's something there
	ld1 R11,'O',R7,0
	zExt R11 ;CVUI2: widen unsigned char to signed int (zero extend)
	jzU2 R11,L97; EQ 0
;						if (argslot&1) { //adjust alignment
	alu2I R11,R0,1,ani,ani
	;removed ?	cpy2 R11,R0
	jzU2 R11,L99; EQ 0
;							this++;
	incm R1,2
;							argslot++;
	incm R0,1
;						}
L99:
;						printflt(*(float *)this);//treats "this" as a pointer to float
	ld4 Rp1p2,'O',R1,0;INDIRF4(addr)
	Ccall _printflt
;						this+=2;				// and advances it 4 bytes
	incm R1,4
;						argslot+=2;
	incm R0,2
;						break;
	lbr L81
L97:
L80:
;					putc('%');putc(c);
	ldaD R12,37; reg:acon
	Ccall _putcser
	cpy1 R12,R6
	zExt R12 ;CVUI2(reg)*: widen unsigned char to signed int (zero extend)*
	Ccall _putcser
;			} //switch
L81:
;		} //%
L78:
;	} //while
L75:
;    while(*ptr) {
	ldn1 R11,R7;reg:  INDIRU1(indaddr)
	jnzU1 R11,L74; NEI2(CVUI2(reg),con0)
;} //prtf
L73:
	release 4; release room for outgoing arguments
	popr R7
	popr R6
	popr R1
	popr R0
	release 4; release room for local variables 
	Cretn

;$$function end$$ _printf
;$$function start$$ _exit
_exit:		;framesize=6
	reserve 4
	st2 R12,'O',sp,(6+1); flag1 
;void exit(int code){
;	printf("exit %d\n",code);
	ldaD R12,L107; reg:acon
	ld2 R13,'O',sp,(6+1) ;reg:INDIRI2(addr)
	Ccall _printf
L108:
;	while(1);
L109:
	lbr L108
;}
L106:
	release 4
	Cretn

;$$function end$$ _exit
;$$function start$$ _memcmp
_memcmp:		;framesize=10
	pushr R0
	pushr R1
	pushr R6
	pushr R7
	ld2 R7,'O',sp,(14+1) ;reg:INDIRU2(addr)
;int memcmp(const void *Ptr1, const void *Ptr2, unsigned int Count){
;    int v = 0;
	ld2z R6
;    p1 = (unsigned char *)Ptr1;
	cpy2 R1,R12 ;LOADP2(reg)
;    p2 = (unsigned char *)Ptr2;
	cpy2 R0,R13 ;LOADP2(reg)
	lbr L113
L112:
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
L113:
;    while(Count-- > 0 && v == 0) {
	cpy2 R11,R7 ;LOADU2*(reg)
	cpy2 R7,R11	;SUBU2(reg,consm)
	decm R7,1	;SUBU2(reg,consm)
	jzU2 R11,L115; EQ 0
	jzU2 R6,L112; EQ 0
L115:
;    return v;
	cpy2 R15,R6 ;LOADI2(reg)
L111:
	popr R7
	popr R6
	popr R1
	popr R0
	Cretn

;$$function end$$ _memcmp
;$$function start$$ _memcpy
_memcpy:		;framesize=8
	pushr R1
	pushr R6
	pushr R7
	ld2 R7,'O',sp,(12+1) ;reg:INDIRU2(addr)
;void* memcpy(void* dest, const void* src, unsigned int count) {
;        char* dst8 = (char*)dest;
	cpy2 R6,R12 ;LOADP2(reg)
;        char* src8 = (char*)src;
	cpy2 R1,R13 ;LOADP2(reg)
	lbr L118
L117:
;        while (count--) {
;            *dst8++ = *src8++;
	cpy2 R11,R6 ;LOADP2(reg)
	cpy2 R6,R11
	incm R6,1
	cpy2 R10,R1 ;LOADP2(reg)
	cpy2 R1,R10
	incm R1,1
	ldn1 R10,R10;reg:  INDIRU1(indaddr)
	str1 R10,R11; ASGNU1(indaddr,reg)		DH
;        }
L118:
;        while (count--) {
	cpy2 R11,R7 ;LOADU2*(reg)
	cpy2 R7,R11	;SUBU2(reg,consm)
	decm R7,1	;SUBU2(reg,consm)
	jnzU2 R11,L117; NE 0 
;        return dest;
	cpy2 R15,R12 ;LOADP2(reg)
L116:
	popr R7
	popr R6
	popr R1
	Cretn

;$$function end$$ _memcpy
;$$function start$$ _memset
_memset:		;framesize=6
	pushr R6
	pushr R7
	ld2 R7,'O',sp,(10+1) ;reg:INDIRU2(addr)
;{
;    unsigned char* p=s;
	cpy2 R6,R12 ;LOADP2(reg)
	lbr L122
L121:
;        *p++ = (unsigned char)c;
	cpy2 R11,R6 ;LOADP2(reg)
	cpy2 R6,R11
	incm R6,1
	cpy2 R10,R13 ;LOADU2*(reg)
	str1 R10,R11; ASGNU1(indaddr,reg)		DH
L122:
;    while(n--)
	cpy2 R11,R7 ;LOADU2*(reg)
	cpy2 R7,R11	;SUBU2(reg,consm)
	decm R7,1	;SUBU2(reg,consm)
	jnzU2 R11,L121; NE 0 
;    return s;
	cpy2 R15,R12 ;LOADP2(reg)
L120:
	popr R7
	popr R6
	Cretn

;$$function end$$ _memset
;$$function start$$ _nstdlibincluder
_nstdlibincluder:		;framesize=2
;void nstdlibincluder(){
;	asm("\tinclude nstdlib.inc\n"); //strcpy, strcmp
	include nstdlib.inc
;}
L124:
	Cretn

;$$function end$$ _nstdlibincluder
L107:
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
L42:
	dd 0xbf800000
	align 4
L41:
	dd 0x0
L3:
	db 85
	db 85
	db 85
	db 85
	db 85
	db 85
	db 85
	db 85
	db 0
	include lcc1802epiloNW.inc
	include LCC1802fp.inc
	include IO1802.inc
