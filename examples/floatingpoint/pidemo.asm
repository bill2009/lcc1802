; generated by lcc-xr18fl $Version: 4.0 - XR18FL - St. Judy's Compiler $ on Wed Apr 03 16:00:03 2013

SP:	equ	2 ;stack pointer
memAddr: equ	14
retAddr: equ	6
retVal:	equ	15
regArg1: equ	12
regArg2: equ	13
	listing off
	include lcc1802ProloFL.inc
	listing on
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
	align 4
_denom:
	dd 0x3f800000
	align 4
_PI4:
	dd 0x3f800000
	align 4
_sign:
	dd 0x3f800000
_main:
	reserve 18
;void main(){
;	printf("begin\n");
	ldaD R12,L2; reg:acon
	Ccall _printf
;	for (n=1;n<=20;n++){
	ldaD R11,1; reg:acon
	st2 R11,'D',(_n),0
L3:
;		denom=(denom+2.0);
	ld4 RL8,'D',(_denom),0;INDIRF4(addr)
	ld4 RL10,'D',(L7),0;INDIRF4(addr)
	Ccall fp_add ;ADDF4(reg,reg)
	st4 RL8,'D',(_denom),0; ASGNF4(addr,reg)
;		sign*=-1;
	ld4 RL8,'D',(L8),0;INDIRF4(addr)
	ld4 RL10,'D',(_sign),0;INDIRF4(addr)
	Ccall fp_mul ;MULF4(reg,reg)
	st4 RL8,'D',(_sign),0; ASGNF4(addr,reg)
;		PI4=PI4+sign/denom;
	ld4 RL8,'D',(_PI4),0;INDIRF4(addr)
	st4 RL8,'O',sp,(-8+20); ASGNF4(addr,reg)
	ld4 RL8,'D',(_sign),0;INDIRF4(addr)
	ld4 RL10,'D',(_denom),0;INDIRF4(addr)
	Ccall fp_div ;DIVF4(reg,reg)
	cpy4 RL10,RL8; LOADU4(reg)
	ld4 RL8,'O',sp,(-8+20);INDIRF4(addr)
	Ccall fp_add ;ADDF4(reg,reg)
	st4 RL8,'D',(_PI4),0; ASGNF4(addr,reg)
;		printf("N=%d, denom=%f, PI=%f\n",n,denom,PI4*4);
	ldaD R12,L9; reg:acon
	ld2 R13,'D',(_n),0 ;reg:INDIRI2(addr)
	ld4 RL10,'D',(_denom),0;INDIRF4(addr)
	st4 RL10,'O',sp,(4); arg+f**
	ld4 RL8,'D',(L10),0;INDIRF4(addr)
	ld4 RL10,'D',(_PI4),0;INDIRF4(addr)
	Ccall fp_mul ;MULF4(reg,reg)
	st4 RL8,'O',sp,(8); arg+f**
	Ccall _printf
;	}
L4:
;	for (n=1;n<=20;n++){
	ld2 R11,'D',(_n),0 ;reg:INDIRI2(addr)
	incm R11,1
	st2 R11,'D',(_n),0
	ld2 R11,'D',(_n),0 ;reg:INDIRI2(addr)
	jnI2I r11,20,lbdf,L3 ;LEI2 20 11 L3; LE is flipped test & subtraction
;	printf("end\n");
	ldaD R12,L11; reg:acon
	Ccall _printf
;}
L1:
	release 18
	Cretn

_strcpy:
	reserve 2
;{
;	char *save = to;
	st2 R12,'O',sp,(-4+4); ASGNP2
;	for (; (*to = *from) != 0; ++from, ++to);
	lbr L16
L13:
L14:
	incm R13,1
	incm R12,1
L16:
	ldn1 R11,R13;reg:  INDIRU1(indaddr)
	str1 R11,R12; ASGNU1(indaddr,reg)
	zExt 11 ;CVUI2(reg): widen unsigned char to signed int (zero extend)*
	jnzU2 r11,L13; NE 0
;	return(save);
	ld2 R15,'O',sp,(-4+4);reg:  INDIRP2(addr)
L12:
	release 2
	Cretn

_printstr:
	reserve 0; save room for local variables
	pushr r7
	reserve 4; save room for outgoing arguments
	cpy2 r7,r12; function(2055) 1
;void printstr(char *ptr){
	lbr L19
L18:
;    while(*ptr) out(5,*ptr++);
	ldaD R12,5; reg:acon
	cpy2 R11,R7 ;LOADP2(reg)
	cpy2 R7,R11
	incm R7,1
	ld1 R13,'O',R11,0
	zExt R13 ;CVUI2: widen unsigned char to signed int (zero extend)
	Ccall _out
L19:
	ld1 R11,'O',R7,0
	zExt R11 ;CVUI2: widen unsigned char to signed int (zero extend)
	jnzU2 r11,L18; NE 0
;}
L17:
	release 4; release room for outgoing arguments
	popr r7
	release 0; release room for local variables 
	Cretn

_ftoa:
	reserve 66; save room for local variables
	pushr r0
	pushr r1
	pushr r7
	reserve 4; save room for outgoing arguments
	cpy4 RL0,RL12; halfbaked&floaty
	ld2 R7,'O',sp,(6+78); reg:INDIRU2(addr)
;{
;   char *output = outbfr ;
	ld2 R11,'O',sp,(4+78);reg:  INDIRP2(addr)
	st2 R11,'O',sp,(-54+78); ASGNP2
;   if (flt < 0.0) {
	ld4 RL10,'D',(L24),0;INDIRF4(addr)
	jcF4 RL0,RL10,lbdf,L22;GEF4(reg,reg) - reverse test
;      *output++ = '-' ;
	ld2 R11,'O',sp,(-54+78);reg:  INDIRP2(addr)
	cpy2 R10,R11
	incm R10,1
	st2 R10,'O',sp,(-54+78); ASGNP2
	ldaD R10,45; reg:acon
	str1 R10,R11; ASGNU1(indaddr,reg)
;      flt *= -1.0 ;
	ld4 RL8,'D',(L25),0;INDIRF4(addr)
	cpy4 RL10,RL0; LOADU4(reg)
	Ccall fp_mul ;MULF4(reg,reg)
	cpy4 RL0,RL8; LOADU4(reg)
;   } else {
	lbr L23
L22:
;      if (use_leading_plus) {
	lbr L26
;         *output++ = '+' ;
	ld2 R11,'O',sp,(-54+78);reg:  INDIRP2(addr)
	cpy2 R10,R11
	incm R10,1
	st2 R10,'O',sp,(-54+78); ASGNP2
	ldaD R10,43; reg:acon
	str1 R10,R11; ASGNU1(indaddr,reg)
;      }
L26:
;   }
L23:
;   if (dec_digits < 8) {
	jcI2I r7,8,lbdf,L28; GE is flipped test from LT
;      flt += round_nums[dec_digits] ;
	cpy4 RL8,RL0; LOADU4(reg)
	cpy2 R11,R7
	shl2I R11,2
	ld4 RL10,'O',R11,(_round_nums);INDIRF4(addr)
	Ccall fp_add ;ADDF4(reg,reg)
	cpy4 RL0,RL8; LOADU4(reg)
;   }
L28:
;	mult=mult_nums[dec_digits];
	cpy2 R11,R7
	shl2I R11,2
	ld4 RL10,'O',R11,(_mult_nums);INDIRF4(addr)
	st4 RL10,'O',sp,(-60+78); ASGNF4(addr,reg)
;   wholeNum = flt;
	cpy4 RL8,RL0; LOADU4(reg)
	ccall cvfi4; CVFI4(reg) convert float to long
	st4 RL8,'O',sp,(-8+78)
;   decimalNum = ((flt - wholeNum) * mult);
	cpy4 RL8,RL0; LOADU4(reg)
	st4 RL8,'O',sp,(-64+78); ASGNF4(addr,reg)
	ld4 RL8,'O',sp,(-8+78);reg:  INDIRI4(addr)
	ccall cvif4; CVIF4(reg) convert long to float
	cpy4 RL10,RL8; LOADU4(reg)
	ld4 RL8,'O',sp,(-64+78);INDIRF4(addr)
	Ccall fp_sub ;SUBF4(reg,reg)
	ld4 RL10,'O',sp,(-60+78);INDIRF4(addr)
	Ccall fp_mul ;MULF4(reg,reg)
	ccall cvfi4; CVFI4(reg) convert float to long
	st4 RL8,'O',sp,(-52+78)
;   idx = 0 ;
	ld2z R11
	st2 R11,'O',sp,(-4+78); ASGNU2(addr,reg)*
	lbr L31
L30:
;   while (wholeNum != 0) {
;      tbfr[idx++] = '0' + (wholeNum % 10) ;
	ld2 R11,'O',sp,(-4+78); reg:INDIRU2(addr)
	st2 R11,'O',sp,(-66+78); ASGNU2(addr,reg)*
	cpy2 R10,R11
	incm R10,1
	st2 R10,'O',sp,(-4+78); ASGNU2(addr,reg)*
	ld4 RL8,'O',sp,(-8+78);reg:  INDIRI4(addr)
	ldI4 RL10,10 ;loading a long integer constant
	Ccall _modi4
	cpy4 RL10,RL8; LOADI4*
	ldA2 R9,'O',sp,(-48+78); reg:addr
	ld2 R8,'O',sp,(-66+78); reg:INDIRU2(addr)
	alu2 R9,R8,R9,add,adc
	st2 R9,'O',sp,(-68+78); ASGNP2
	ldI4 RL8,48 ;loading a long integer constant
	alu4 RL10,RL10,RL8,add,adc
	cpy2 R11,RL10 ;LOADU2(reg)*
	ld2 R10,'O',sp,(-68+78);reg:  INDIRP2(addr)
	str1 R11,R10; ASGNU1(indaddr,reg)
;      wholeNum /= 10 ;
	ld4 RL8,'O',sp,(-8+78);reg:  INDIRI4(addr)
	ldI4 RL10,10 ;loading a long integer constant
	Ccall _divi4; DIVI4(reg,reg)
	cpy4 RL10,RL8; LOADI4*
	st4 RL10,'O',sp,(-8+78)
;   }
L31:
;   while (wholeNum != 0) {
	ld4 RL10,'O',sp,(-8+78);reg:  INDIRI4(addr)
	ldI4 RL8,0 ;loading a long integer constant
	jneU4 rL10,rL8,L30; NE
;   if (idx == 0) {
	ld2 R11,'O',sp,(-4+78); reg:INDIRU2(addr)
	jnzU2 r11,L36; NE 0
;      *output++ = '0' ;
	ld2 R11,'O',sp,(-54+78);reg:  INDIRP2(addr)
	cpy2 R10,R11
	incm R10,1
	st2 R10,'O',sp,(-54+78); ASGNP2
	ldaD R10,48; reg:acon
	str1 R10,R11; ASGNU1(indaddr,reg)
;   } else {
	lbr L34
L35:
;      while (idx > 0) {
;         *output++ = tbfr[idx-1] ;  //lint !e771
	ld2 R11,'O',sp,(-54+78);reg:  INDIRP2(addr)
	cpy2 R10,R11
	incm R10,1
	st2 R10,'O',sp,(-54+78); ASGNP2
	ld2 R10,'O',sp,(-4+78); reg:INDIRU2(addr)
	decm R10,1
	ldA2 R9,'O',sp,(-48+78); reg:addr
	alu2 R10,R10,R9,add,adc
	ldn1 R10,R10;reg:  INDIRU1(indaddr)
	str1 R10,R11; ASGNU1(indaddr,reg)
;         idx-- ;
	ld2 R11,'O',sp,(-4+78); reg:INDIRU2(addr)
	decm R11,1
	st2 R11,'O',sp,(-4+78); ASGNU2(addr,reg)*
;      }
L36:
;      while (idx > 0) {
	ld2 R11,'O',sp,(-4+78); reg:INDIRU2(addr)
	jnzU2 r11,L35; NE 0
;   }
L34:
;   if (dec_digits > 0) {
	jeqU2I r7,0,L38;EQU2(reg,con)
;      *output++ = '.' ;
	ld2 R11,'O',sp,(-54+78);reg:  INDIRP2(addr)
	cpy2 R10,R11
	incm R10,1
	st2 R10,'O',sp,(-54+78); ASGNP2
	ldaD R10,46; reg:acon
	str1 R10,R11; ASGNU1(indaddr,reg)
;      idx = 0 ;
	ld2z R11
	st2 R11,'O',sp,(-4+78); ASGNU2(addr,reg)*
	lbr L41
L40:
;      while (decimalNum != 0) {
;         tbfr[idx++] = '0' + (decimalNum % 10) ;
	ld2 R11,'O',sp,(-4+78); reg:INDIRU2(addr)
	st2 R11,'O',sp,(-66+78); ASGNU2(addr,reg)*
	cpy2 R10,R11
	incm R10,1
	st2 R10,'O',sp,(-4+78); ASGNU2(addr,reg)*
	ld4 RL8,'O',sp,(-52+78);reg:  INDIRI4(addr)
	ldI4 RL10,10 ;loading a long integer constant
	Ccall _modi4
	cpy4 RL10,RL8; LOADI4*
	ldA2 R9,'O',sp,(-48+78); reg:addr
	ld2 R8,'O',sp,(-66+78); reg:INDIRU2(addr)
	alu2 R9,R8,R9,add,adc
	st2 R9,'O',sp,(-68+78); ASGNP2
	ldI4 RL8,48 ;loading a long integer constant
	alu4 RL10,RL10,RL8,add,adc
	cpy2 R11,RL10 ;LOADU2(reg)*
	ld2 R10,'O',sp,(-68+78);reg:  INDIRP2(addr)
	str1 R11,R10; ASGNU1(indaddr,reg)
;         decimalNum /= 10 ;
	ld4 RL8,'O',sp,(-52+78);reg:  INDIRI4(addr)
	ldI4 RL10,10 ;loading a long integer constant
	Ccall _divi4; DIVI4(reg,reg)
	cpy4 RL10,RL8; LOADI4*
	st4 RL10,'O',sp,(-52+78)
;      }
L41:
;      while (decimalNum != 0) {
	ld4 RL10,'O',sp,(-52+78);reg:  INDIRI4(addr)
	ldI4 RL8,0 ;loading a long integer constant
	jneU4 rL10,rL8,L40; NE
	lbr L44
L43:
;      while (idx < dec_digits) {
;         tbfr[idx++] = '0' ;
	ld2 R11,'O',sp,(-4+78); reg:INDIRU2(addr)
	cpy2 R10,R11
	incm R10,1
	st2 R10,'O',sp,(-4+78); ASGNU2(addr,reg)*
	ldA2 R10,'O',sp,(-48+78); reg:addr
	alu2 R11,R11,R10,add,adc
	ldaD R10,48; reg:acon
	str1 R10,R11; ASGNU1(indaddr,reg)
;      }
L44:
;      while (idx < dec_digits) {
	ld2 R11,'O',sp,(-4+78); reg:INDIRU2(addr)
	jcU2 r11,r7,lbnf,L43; LT=lbnf i.e. subtract B from A and jump if borrow 
;      if (idx == 0) {
	ld2 R11,'O',sp,(-4+78); reg:INDIRU2(addr)
	jnzU2 r11,L49; NE 0
;         *output++ = '0' ;
	ld2 R11,'O',sp,(-54+78);reg:  INDIRP2(addr)
	cpy2 R10,R11
	incm R10,1
	st2 R10,'O',sp,(-54+78); ASGNP2
	ldaD R10,48; reg:acon
	str1 R10,R11; ASGNU1(indaddr,reg)
;      } else {
	lbr L47
L48:
;         while (idx > 0) {
;            *output++ = tbfr[idx-1] ;
	ld2 R11,'O',sp,(-54+78);reg:  INDIRP2(addr)
	cpy2 R10,R11
	incm R10,1
	st2 R10,'O',sp,(-54+78); ASGNP2
	ld2 R10,'O',sp,(-4+78); reg:INDIRU2(addr)
	decm R10,1
	ldA2 R9,'O',sp,(-48+78); reg:addr
	alu2 R10,R10,R9,add,adc
	ldn1 R10,R10;reg:  INDIRU1(indaddr)
	str1 R10,R11; ASGNU1(indaddr,reg)
;            idx-- ;
	ld2 R11,'O',sp,(-4+78); reg:INDIRU2(addr)
	decm R11,1
	st2 R11,'O',sp,(-4+78); ASGNU2(addr,reg)*
;         }
L49:
;         while (idx > 0) {
	ld2 R11,'O',sp,(-4+78); reg:INDIRU2(addr)
	jnzU2 r11,L48; NE 0
;      }
L47:
;   }
L38:
;   *output = 0 ;
	ld2 R11,'O',sp,(-54+78);reg:  INDIRP2(addr)
	ldaD R10,0; reg:acon
	str1 R10,R11; ASGNU1(indaddr,reg)
;   return outbfr;
	ld2 R15,'O',sp,(4+78);reg:  INDIRP2(addr)
L21:
	release 4; release room for outgoing arguments
	popr r7
	popr r1
	popr r0
	release 66; release room for local variables 
	Cretn

_itoa:
	reserve 4; save room for local variables
	pushr r0
	pushr r1
	pushr r7
	reserve 4; save room for outgoing arguments
	st2 r12,'O',sp,(16); flag1 
	st2 r13,'O',sp,(18); flag1 
;char * itoa(int s, char *buffer){ //convert an integer to printable ascii in a buffer supplied by the caller
;	unsigned int flag=0;
	ld2z R11
	st2 R11,'O',sp,(-4+16); ASGNU2(addr,reg)*
;	char * bptr; bptr=buffer;
	ld2 R11,'O',sp,(2+16);reg:  INDIRP2(addr)
	st2 R11,'O',sp,(-6+16); ASGNP2
;	if (s<0){
	ld2 R11,'O',sp,(0+16) ;reg:INDIRI2(addr)
	jcI2I r11,0,lbdf,L52; GE is flipped test from LT
;		*bptr='-';bptr++;
	ld2 R11,'O',sp,(-6+16);reg:  INDIRP2(addr)
	ldaD R10,45; reg:acon
	str1 R10,R11; ASGNU1(indaddr,reg)
	ld2 R11,'O',sp,(-6+16);reg:  INDIRP2(addr)
	incm R11,1
	st2 R11,'O',sp,(-6+16); ASGNP2
;		n=-s;
	ld2 R11,'O',sp,(0+16) ;reg:INDIRI2(addr)
	negI2 R11,R11 ;was alu2I R11,R11,0,sdi,sdbi
	cpy2 R1,R11 ;LOADU2(reg)*
;	} else{
	lbr L53
L52:
;		n=s;
	ld2 R11,'O',sp,(0+16) ;reg:INDIRI2(addr)
	cpy2 R1,R11 ;LOADU2(reg)*
;	}
L53:
;	k=10000;
	ldaD R7,10000; reg:acon
	lbr L55
L54:
;	while(k>0){
;		for(r=0;k<=n;r++,n-=k); // was r=n/k
	ld2z R0
	lbr L60
L57:
L58:
	incm R0,1
	alu2 R1,R1,R7,sm,smb
L60:
	jcU2 r1,r7,lbdf,L57 ;LE is flipped test & operands
;		if (flag || r>0||k==1){
	ld2 R11,'O',sp,(-4+16); reg:INDIRU2(addr)
	jnzU2 r11,L64; NE 0
	jnzU2 r0,L64; NE 0
	jneU2I r7,1,L61; NE
L64:
;			*bptr=('0'+r);bptr++;
	ld2 R11,'O',sp,(-6+16);reg:  INDIRP2(addr)
	ldA2 R10,'O',R0,(48); reg:addr
	str1 R10,R11; ASGNU1(indaddr,reg)
	ld2 R11,'O',sp,(-6+16);reg:  INDIRP2(addr)
	incm R11,1
	st2 R11,'O',sp,(-6+16); ASGNP2
;			flag='y';
	ldaD R11,121; reg:acon
	st2 R11,'O',sp,(-4+16); ASGNU2(addr,reg)*
;		}
L61:
;		k=k/10;
	cpy2 R12,R7 ;LOADU2(reg)*
	ldaD R13,10; reg:acon
	Ccall _divu2
	cpy2 R11,R15 ;LOADU2(reg)*
	cpy2 R7,R11 ;LOADU2(reg)*
;	}
L55:
;	while(k>0){
	jnzU2 r7,L54; NE 0
;	*bptr='\0';
	ld2 R11,'O',sp,(-6+16);reg:  INDIRP2(addr)
	ldaD R10,0; reg:acon
	str1 R10,R11; ASGNU1(indaddr,reg)
;	return buffer;
	ld2 R15,'O',sp,(2+16);reg:  INDIRP2(addr)
L51:
	release 4; release room for outgoing arguments
	popr r7
	popr r1
	popr r0
	release 4; release room for local variables 
	Cretn

_ltoa:
	reserve 0; save room for local variables
	pushr r0
	pushr r1
	pushr r7
	reserve 6; save room for outgoing arguments
	cpy4 RL0,RL12; halfbaked
;char * ltoa(long s, char *buffer){ //convert a long integer to printable ascii in a buffer supplied by the caller
;	char* bptr=buffer;
	ld2 R7,'O',sp,(4+14);reg:  INDIRP2(addr)
;	if (s<0){
	ldI4 RL10,0 ;loading a long integer constant
	jcI4 RL0,RL10,lbdf,L66; GE is flipped test from LT
;		*bptr++='-';
	cpy2 R11,R7 ;LOADP2(reg)
	cpy2 R7,R11
	incm R7,1
	ldaD R10,45; reg:acon
	str1 R10,R11; ASGNU1(indaddr,reg)
;		s=-s;
	negI4 RL0,RL0 ;was alu4I RL0,RL0,0,sdi,sdbi
;	}
L66:
;	strcpy(bptr,dubdabx(s,bptr)); //uses assembler double-dabble routine
	cpy4 Rp1p2,RL0; LOADI4*
	st2 r7,'O',sp,(4); arg+f**
	Ccall _dubdabx
	cpy2 R11,R15 ;LOADP2(reg)
	cpy2 R12,R7 ;LOADP2(reg)
	cpy2 R13,R11 ;LOADP2(reg)
	Ccall _strcpy
;	return buffer;
	ld2 R15,'O',sp,(4+14);reg:  INDIRP2(addr)
L65:
	release 6; release room for outgoing arguments
	popr r7
	popr r1
	popr r0
	release 0; release room for local variables 
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
L68:
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
L69:
	release 18
	Cretn

_printflt:
	reserve 24
	st2 r12,'O',sp,(26); flag1 
	st2 r13,'O',sp,(28); flag1 
;void printflt(float s){ //print a float
;	printstr(ftoa(s,buffer,6));
	ld4 Rp1p2,'O',sp,(0+26);INDIRF4(addr)
	ldA2 R11,'O',sp,(-18+26); reg:addr
	st2 r11,'O',sp,(4); arg+f**
	ldaD R11,6; reg:acon
	st2 r11,'O',sp,(6); arg+f**
	Ccall _ftoa
	cpy2 R11,R15 ;LOADP2(reg)
	cpy2 R12,R11 ;LOADP2(reg)
	Ccall _printstr
;}
L70:
	release 24
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
	jcI2I r11,10,lbdf,L72; GE is flipped test from LT
;		putc(x+'0');
	ld1 R11,'O',sp,(0+6)
	zExt R11 ;CVUI2: widen unsigned char to signed int (zero extend)
	ldA2 R11,'O',R11,(48); reg:addr
	cpy1 R12,R11
	zExt 12 ;CVUI2(reg): widen unsigned char to signed int (zero extend)*
	Ccall _putc
;	} else {
	lbr L73
L72:
;		putc(x+'A'-10);
	ld1 R11,'O',sp,(0+6)
	zExt R11 ;CVUI2: widen unsigned char to signed int (zero extend)
	ldA2 R11,'O',R11,(65); reg:addr
	alu2I R11,R11,10,smi,smbi
	cpy1 R12,R11
	zExt 12 ;CVUI2(reg): widen unsigned char to signed int (zero extend)*
	Ccall _putc
;	}
L73:
;}
L71:
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
	zExt 12 ;CVUI2(reg): widen unsigned char to signed int (zero extend)*
	Ccall _putxn
;	putxn(x & 0x0F);
	ld1 R11,'O',sp,(0+6)
	zExt R11 ;CVUI2: widen unsigned char to signed int (zero extend)
	alu2I R11,R11,15,ani,ani
	;removed ?	cpy2 R11,R11
	cpy1 R12,R11
	zExt 12 ;CVUI2(reg): widen unsigned char to signed int (zero extend)*
	Ccall _putxn
;}
L74:
	release 4
	Cretn

_printf:
	reserve 4; save room for local variables
	pushr r0
	pushr r1
	pushr r7
	reserve 4; save room for outgoing arguments
	st2 r12,'O',sp,(16); flag1 
	st2 r13,'O',sp,(18); flag2
;void printf(char *ptr,...){ //limited implementation of printf
;	int argslot=0;	//used to align longs
	ld2z R0
;	int * this=(int *)&ptr;
	ldA2 R1,'O',sp,(0+16); reg:addr
;	this++; argslot++; //advance argument pointer and slot #
	incm R1,2
	incm R0,1
	lbr L77
L76:
;    while(*ptr) {
;		c=*ptr++;
	ld2 R11,'O',sp,(0+16);reg:  INDIRP2(addr)
	cpy2 R10,R11
	incm R10,1
	st2 R10,'O',sp,(0+16); ASGNP2
	ldn1 R7,R11;reg:  INDIRU1(indaddr)
;		if (c!='%'){
	cpy1 R11,R7
	zExt 11 ;CVUI2(reg): widen unsigned char to signed int (zero extend)*
	jeqU2I r11,37,L79;EQI2(reg,con)
;			putc(c);
	cpy1 R12,R7
	zExt 12 ;CVUI2(reg): widen unsigned char to signed int (zero extend)*
	Ccall _putc
;		} else{
	lbr L80
L79:
;			c=*ptr++;
	ld2 R11,'O',sp,(0+16);reg:  INDIRP2(addr)
	cpy2 R10,R11
	incm R10,1
	st2 R10,'O',sp,(0+16); ASGNP2
	ldn1 R7,R11;reg:  INDIRU1(indaddr)
;			switch (c){
	cpy1 R11,R7
	zExt 11 ;CVUI2(reg): widen unsigned char to signed int (zero extend)*
	st2 R11,'O',sp,(-5+16)
	ld2 R11,'O',sp,(-5+16) ;reg:INDIRI2(addr)
	jcI2I r11,99,lbnf,L100  ;LT=lbnf i.e. subtract immedB from A and jump if borrow
	jnI2I r11,108,lbnf,L101; GT reverse  the subtraction
	shl2I R11,1
	ld2 R11,'O',R11,(L102-198);reg:  INDIRP2(addr)
	jumpv r11; JUMPV(reg)
L102:
	dw L86
	dw L84
	dw L81
	dw L95
	dw L81
	dw L81
	dw L84
	dw L81
	dw L81
	dw L88
L100:
	ld2 R11,'O',sp,(-5+16) ;reg:INDIRI2(addr)
	jeqU2I r11,88,L87;EQI2(reg,con)
	lbr L81
L101:
	ld2 R11,'O',sp,(-5+16) ;reg:INDIRI2(addr)
	ldaD R10,115; reg:acon
	jeqI2 r11,r10,L85; EQI2(reg,reg)
	jcI2 r11,r10,lbnf,L81; LT=lbnf i.e. subtract B from A and jump if borrow 
L104:
	ld2 R11,'O',sp,(-5+16) ;reg:INDIRI2(addr)
	jeqU2I r11,120,L87;EQI2(reg,con)
	lbr L81
L84:
;					printint(*this++);
	cpy2 R11,R1 ;LOADP2(reg)
	cpy2 R1,R11
	incm R1,2
	ld2 R12,'O',R11,0 ;reg:INDIRI2(addr)
	Ccall _printint
;					argslot+=1; //next argument slot
	incm R0,1
;					break;
	lbr L82
L85:
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
	lbr L82
L86:
;					putc((unsigned int) *this++);
	cpy2 R11,R1 ;LOADP2(reg)
	cpy2 R1,R11
	incm R1,2
	ld2 R11,'O',R11,0 ;reg:INDIRI2(addr)
	cpy1 R12,R11
	zExt 12 ;CVUI2(reg): widen unsigned char to signed int (zero extend)*
	Ccall _putc
;					argslot+=1; //next argument slot
	incm R0,1
;					break;
	lbr L82
L87:
;					putx(((unsigned int) *this)>>8);
	ld2 R11,'O',R1,0 ;reg:INDIRI2(addr)
	shrU2I R11,8
	cpy1 R12,R11
	zExt 12 ;CVUI2(reg): widen unsigned char to signed int (zero extend)*
	Ccall _putx
;					putx(((unsigned int) *this++)&255);
	cpy2 R11,R1 ;LOADP2(reg)
	cpy2 R1,R11
	incm R1,2
	ld2 R11,'O',R11,0 ;reg:INDIRI2(addr)
	alu2I R11,R11,255,ani,ani ;removed copy;BANDU2(reg,con)  
	cpy1 R12,R11
	zExt 12 ;CVUI2(reg): widen unsigned char to signed int (zero extend)*
	Ccall _putx
;					argslot+=1; //next argument slot
	incm R0,1
;					break;
	lbr L82
L88:
;					if (*ptr){ //as long as there's something there
	ld2 R11,'O',sp,(0+16);reg:  INDIRP2(addr)
	ld1 R11,'O',R11,0
	zExt R11 ;CVUI2: widen unsigned char to signed int (zero extend)
	jzU2 r11,L89; EQ 0
;						xord=*ptr++;
	ld2 R11,'O',sp,(0+16);reg:  INDIRP2(addr)
	cpy2 R10,R11
	incm R10,1
	st2 R10,'O',sp,(0+16); ASGNP2
	ldn1 R11,R11;reg:  INDIRU1(indaddr)
	st1 R11,'O',sp,(-3+16); ASGNU1
;						if (argslot&1) {
	alu2I R11,R0,1,ani,ani
	;removed ?	cpy2 R11,R0
	jzU2 r11,L91; EQ 0
;							this++;
	incm R1,2
;							argslot++;
	incm R0,1
;						}
L91:
;						if(xord=='d'){
	ld1 R11,'O',sp,(-3+16)
	zExt R11 ;CVUI2: widen unsigned char to signed int (zero extend)
	jneU2I r11,100,L93; NE
;							printlint(*(long *)this);//treats "this" as a pointer to long
	ld4 Rp1p2,'O',R1,0;reg:  INDIRI4(addr)
	Ccall _printlint
;							this+=2;				// and advances it 4 bytes
	incm R1,4
;						} else{
	lbr L94
L93:
;							putx(((unsigned int) *this)>>8);
	ld2 R11,'O',R1,0 ;reg:INDIRI2(addr)
	shrU2I R11,8
	cpy1 R12,R11
	zExt 12 ;CVUI2(reg): widen unsigned char to signed int (zero extend)*
	Ccall _putx
;							putx(((unsigned int) *this++)&255);
	cpy2 R11,R1 ;LOADP2(reg)
	cpy2 R1,R11
	incm R1,2
	ld2 R11,'O',R11,0 ;reg:INDIRI2(addr)
	alu2I R11,R11,255,ani,ani ;removed copy;BANDU2(reg,con)  
	cpy1 R12,R11
	zExt 12 ;CVUI2(reg): widen unsigned char to signed int (zero extend)*
	Ccall _putx
;							putx(((unsigned int) *this)>>8);
	ld2 R11,'O',R1,0 ;reg:INDIRI2(addr)
	shrU2I R11,8
	cpy1 R12,R11
	zExt 12 ;CVUI2(reg): widen unsigned char to signed int (zero extend)*
	Ccall _putx
;							putx(((unsigned int) *this++)&255);
	cpy2 R11,R1 ;LOADP2(reg)
	cpy2 R1,R11
	incm R1,2
	ld2 R11,'O',R11,0 ;reg:INDIRI2(addr)
	alu2I R11,R11,255,ani,ani ;removed copy;BANDU2(reg,con)  
	cpy1 R12,R11
	zExt 12 ;CVUI2(reg): widen unsigned char to signed int (zero extend)*
	Ccall _putx
;						}
L94:
;						argslot+=2;
	incm R0,2
;						break;
	lbr L82
L89:
L95:
;					if (*ptr){ //as long as there's something there
	ld2 R11,'O',sp,(0+16);reg:  INDIRP2(addr)
	ld1 R11,'O',R11,0
	zExt R11 ;CVUI2: widen unsigned char to signed int (zero extend)
	jzU2 r11,L96; EQ 0
;						if (argslot&1) { //adjust alignment
	alu2I R11,R0,1,ani,ani
	;removed ?	cpy2 R11,R0
	jzU2 r11,L98; EQ 0
;							this++;
	incm R1,2
;							argslot++;
	incm R0,1
;						}
L98:
;						printflt(*(float *)this);//treats "this" as a pointer to float
	ld4 Rp1p2,'O',R1,0;INDIRF4(addr)
	Ccall _printflt
;						this+=2;				// and advances it 4 bytes
	incm R1,4
;						argslot+=2;
	incm R0,2
;						break;
	lbr L82
L96:
L81:
;					putc('%');putc(c);
	ldaD R12,37; reg:acon
	Ccall _putc
	cpy1 R12,R7
	zExt 12 ;CVUI2(reg): widen unsigned char to signed int (zero extend)*
	Ccall _putc
;			} //switch
L82:
;		} //%
L80:
;	} //while
L77:
;    while(*ptr) {
	ld2 R11,'O',sp,(0+16);reg:  INDIRP2(addr)
	ld1 R11,'O',R11,0
	zExt R11 ;CVUI2: widen unsigned char to signed int (zero extend)
	jnzU2 r11,L76; NE 0
;} //prtf
L75:
	release 4; release room for outgoing arguments
	popr r7
	popr r1
	popr r0
	release 4; release room for local variables 
	Cretn

_exit:
	reserve 4
	st2 r12,'O',sp,(6); flag1 
;void exit(int code){
;	printf("exit %d\n",code);
	ldaD R12,L106; reg:acon
	ld2 R13,'O',sp,(0+6) ;reg:INDIRI2(addr)
	Ccall _printf
L107:
;	while(1);
L108:
	lbr L107
;}
L105:
	release 4
	Cretn

_memcmp:
	reserve 2; save room for local variables
	pushr r0
	pushr r1
	pushr r7
	reserve 0; save room for outgoing arguments
	ld2 R7,'O',sp,(4+10); reg:INDIRU2(addr)
;int memcmp(const void *Ptr1, const void *Ptr2, unsigned int Count){
;    int v = 0;
	ld2z R1
;    p1 = (unsigned char *)Ptr1;
	cpy2 R0,R12 ;LOADP2(reg)
;    p2 = (unsigned char *)Ptr2;
	st2 R13,'O',sp,(-4+10); ASGNP2
	lbr L112
L111:
;    while(Count-- > 0 && v == 0) {
;        v = *(p1++) - *(p2++);
	cpy2 R11,R0 ;LOADP2(reg)
	cpy2 R0,R11
	incm R0,1
	ld2 R10,'O',sp,(-4+10);reg:  INDIRP2(addr)
	cpy2 R9,R10
	incm R9,1
	st2 R9,'O',sp,(-4+10); ASGNP2
	ld1 R11,'O',R11,0
	zExt R11 ;CVUI2: widen unsigned char to signed int (zero extend)
	ld1 R10,'O',R10,0
	zExt R10 ;CVUI2: widen unsigned char to signed int (zero extend)
	alu2 R1,R11,R10,sm,smb
;    }
L112:
;    while(Count-- > 0 && v == 0) {
	cpy2 R11,R7 ;LOADU2(reg)*
	cpy2 R7,R11
	decm R7,1
	jeqU2I r11,0,L114;EQU2(reg,con)
	jzU2 r1,L111; EQ 0
L114:
;    return v;
	cpy2 R15,R1 ;LOADI2(reg)
L110:
	release 0; release room for outgoing arguments
	popr r7
	popr r1
	popr r0
	release 2; release room for local variables 
	Cretn

_memcpy:
	reserve 0; save room for local variables
	pushr r0
	pushr r1
	pushr r7
	reserve 0; save room for outgoing arguments
	ld2 R7,'O',sp,(4+8); reg:INDIRU2(addr)
;void* memcpy(void* dest, const void* src, unsigned int count) {
;        char* dst8 = (char*)dest;
	cpy2 R1,R12 ;LOADP2(reg)
;        char* src8 = (char*)src;
	cpy2 R0,R13 ;LOADP2(reg)
	lbr L117
L116:
;        while (count--) {
;            *dst8++ = *src8++;
	cpy2 R11,R1 ;LOADP2(reg)
	cpy2 R1,R11
	incm R1,1
	cpy2 R10,R0 ;LOADP2(reg)
	cpy2 R0,R10
	incm R0,1
	ldn1 R10,R10;reg:  INDIRU1(indaddr)
	str1 R10,R11; ASGNU1(indaddr,reg)
;        }
L117:
;        while (count--) {
	cpy2 R11,R7 ;LOADU2(reg)*
	cpy2 R7,R11
	decm R7,1
	jnzU2 r11,L116; NE 0
;        return dest;
	cpy2 R15,R12 ;LOADP2(reg)
L115:
	release 0; release room for outgoing arguments
	popr r7
	popr r1
	popr r0
	release 0; release room for local variables 
	Cretn

_memset:
	reserve 0; save room for local variables
	pushr r1
	pushr r7
	reserve 0; save room for outgoing arguments
	ld2 R7,'O',sp,(4+6); reg:INDIRU2(addr)
;{
;    unsigned char* p=s;
	cpy2 R1,R12 ;LOADP2(reg)
	lbr L121
L120:
;        *p++ = (unsigned char)c;
	cpy2 R11,R1 ;LOADP2(reg)
	cpy2 R1,R11
	incm R1,1
	cpy2 R10,R13 ;LOADU2(reg)*
	str1 R10,R11; ASGNU1(indaddr,reg)
L121:
;    while(n--)
	cpy2 R11,R7 ;LOADU2(reg)*
	cpy2 R7,R11
	decm R7,1
	jnzU2 r11,L120; NE 0
;    return s;
	cpy2 R15,R12 ;LOADP2(reg)
L119:
	release 0; release room for outgoing arguments
	popr r7
	popr r1
	release 0; release room for local variables 
	Cretn

_n:
	db 2 dup (0); zerofill global
	align 4
_term:
	db 4 dup (0); zerofill global
L106:
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
L25:
	dd 0xbf800000
	align 4
L24:
	dd 0x0
L11:
	db 101
	db 110
	db 100
	db 10
	db 0
	align 4
L10:
	dd 0x40800000
L9:
	db 78
	db 61
	db 37
	db 100
	db 44
	db 32
	db 100
	db 101
	db 110
	db 111
	db 109
	db 61
	db 37
	db 102
	db 44
	db 32
	db 80
	db 73
	db 61
	db 37
	db 102
	db 10
	db 0
	align 4
L8:
	dd 0xbf800000
	align 4
L7:
	dd 0x40000000
L2:
	db 98
	db 101
	db 103
	db 105
	db 110
	db 10
	db 0
	include lcc1802Epilofl.inc
	include LCC1802fp.inc
	include IO1802.inc
