; generated by lcc-xr18NW $Version: 5.0 - XR18NW $ on Sun Jan 28 16:05:51 2018

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
_boydsegments:
	db 0
	db 0
	db 0
	db 0
	db 0
	db 0
	db 0
	db 0
	db 0
	db 0
	db 0
	db 0
	db 0
	db 0
	db 0
	db 0
	db 0
	db 0
	db 0
	db 0
	db 0
	db 0
	db 0
	db 0
	db 0
	db 0
	db 0
	db 0
	db 0
	db 0
	db 0
	db 0
	db 0
	db 0
	db 34
	db 0
	db 0
	db 0
	db 0
	db 32
	db 0
	db 0
	db 0
	db 0
	db 17
	db 4
	db 0
	db 44
	db 123
	db 48
	db 109
	db 117
	db 54
	db 87
	db 95
	db 112
	db 127
	db 119
	db 65
	db 81
	db 0
	db 0
	db 0
	db 0
	db 111
	db 126
	db 31
	db 75
	db 61
	db 79
	db 78
	db 91
	db 62
	db 16
	db 57
	db 94
	db 11
	db 88
	db 28
	db 29
	db 110
	db 103
	db 12
	db 83
	db 15
	db 59
	db 25
	db 35
	db 92
	db 55
	db 105
	db 0
	db 22
	db 0
	db 0
	db 1
	db 6
	db 126
	db 31
	db 75
	db 61
	db 79
	db 78
	db 91
	db 62
	db 16
	db 57
	db 94
	db 11
	db 88
	db 28
	db 29
	db 110
	db 103
	db 12
	db 83
	db 15
	db 59
	db 25
	db 35
	db 92
	db 55
	db 105
	db 0
	db 0
	db 0
	db 0
	db 0
;$$function start$$ _disp1
_disp1:		;framesize=2
;void disp1(unsigned char d){//display a byte as two hex digits
;	asm(" glo 12\n ani 0x0f\n" //prep bottom digit
 glo 12
 ani 0x0f
 dec 2
 str 2
 out 7
 glo 12
 shr
 shr
 shr
 shr
 dec 2
 str 2
 out 7
;}
L1:
	Cretn

;$$function end$$ _disp1
;$$function start$$ _dispmemloc
_dispmemloc:		;framesize=8
	pushr R7
	reserve 4; save room for outgoing arguments
	st2 R12,'O',sp,(8+1); flag1 
;void dispmemloc(unsigned char * loc){
;	initleds(0b11010000); //LEDs in hex decode mode
;	disp1(*(loc+1));
 req
 seq
 dec 2
 ldi 0b11010000
 str 2
 out 7
 req
	ld2 R11,'O',sp,(8+1) ;reg:INDIRP2(addr)
	ld1 R12,'O',R11,(1)
	zExt R12 ;CVUI2: widen unsigned char to signed int (zero extend)
	Ccall _disp1
;	disp1(*loc);
	ld2 R11,'O',sp,(8+1) ;reg:INDIRP2(addr)
	ld1 R12,'O',R11,0
	zExt R12 ;CVUI2: widen unsigned char to signed int (zero extend)
	Ccall _disp1
;	lint=(unsigned int)loc;
	ld2 R11,'O',sp,(8+1) ;reg:INDIRP2(addr)
	cpy2 R7,R11 ;LOADU2*(reg)
;	disp1((unsigned int)loc&0xff);
	ld2 R11,'O',sp,(8+1) ;reg:INDIRP2(addr)
	alu2I R11,R11,255,ani,ani ;removed copy;BANDU2(reg,con)  
	cpy1 R12,R11
	zExt R12 ;CVUI2(reg)*: widen unsigned char to signed int (zero extend)*
	Ccall _disp1
;	disp1(lint>>8);
	cpy2 R11,R7
	shrU2I R11,8
	cpy1 R12,R11
	zExt R12 ;CVUI2(reg)*: widen unsigned char to signed int (zero extend)*
	Ccall _disp1
;}
L3:
	release 4; release room for outgoing arguments
	popr R7
	Cretn

;$$function end$$ _dispmemloc
;$$function start$$ _dispval
_dispval:		;framesize=8
	pushr R7
	reserve 4; save room for outgoing arguments
	st2 R12,'O',sp,(8+1); flag1 
	ldA2 R11,'O',sp,(8+1); reg:addr
	ld2 R10,'O',sp,(8+1) ;reg:INDIRI2(addr)
	str1 R10,R11; ASGNU1(indaddr,reg)		DH
;void dispval(unsigned char v){
;	initleds(0b11010000); //LEDs in hex decode mode
;	disp1(v);
 req
 seq
 dec 2
 ldi 0b11010000
 str 2
 out 7
 req
	ld1 R12,'O',sp,(8+1)
	zExt R12 ;CVUI2: widen unsigned char to signed int (zero extend)
	Ccall _disp1
;	for (i=6;i!=0;i--) out(7,0);
	ldaD R7,6; reg:acon
	lbr L10
L7:
	ldaD R12,7; reg:acon
	ld2z R13
	Ccall _out; CALLI2(ar)
L8:
	decm R7,1	;SUBU2(reg,consm)
L10:
	jnzU2 R7,L7; NE 0 
;}
L5:
	release 4; release room for outgoing arguments
	popr R7
	Cretn

;$$function end$$ _dispval
;$$function start$$ _getsp
_getsp:		;framesize=2
;unsigned int getsp(){//return stack pointer value
;	asm(" cpy2 r15,sp\n"  	//copy stack pointer to return reg
;	return 0;				//not executed
 cpy2 r15,sp
 cretn
	ld2z R15
L11:
	Cretn

;$$function end$$ _getsp
;$$function start$$ _execute
_execute:		;framesize=16
	reserve 2; save room for local variables
	pushr R0
	pushr R1
	pushr R6
	pushr R7
	reserve 4; save room for outgoing arguments
	cpy2 R7,R12; function(2055) 1
;unsigned char * execute(unsigned char * loc){
	lbr L15
L14:
;	while(1){
;		op=*loc; val=*(loc+1);
	ldn1 R6,R7;reg:  INDIRU1(indaddr)
	cpy2 R11,R7
	incm R11,1
	ldn1 R1,R11;reg:  INDIRU1(indaddr)
;		switch (op){
	cpy1 R11,R6
	zExt R11 ;CVUI2(reg)*: widen unsigned char to signed int (zero extend)*
	st2 R11,'O',sp,(12+1); ASGNI2(addr,reg)
	ld2 R11,'O',sp,(12+1) ;reg:INDIRI2(addr)
	jcI2I R11,0,lbnf,L17  ;LT=lbnf i.e. subtract immedB from A and jump if borrow
	jnI2I R11,4,lbnf,L17; GT reverse  the subtraction
	shl2I R11,1
	ld2 R11,'O',R11,(L25) ;reg:INDIRP2(addr)
	jumpv R11; JUMPV(reg)
L25:
	dw L20
	dw L21
	dw L22
	dw L23
	dw L24
L20:
;				mp=(unsigned char *)(4096+val);
	cpy1 R11,R1
	zExt R11 ;CVUI2(reg)*: widen unsigned char to signed int (zero extend)*
	ldA2 R11,'O',R11,(4096); reg:addr
	cpy2 R0,R11 ;LOADP2(reg)
;				dispval(*mp); delay(1000);
	ld1 R12,'O',R0,0
	zExt R12 ;CVUI2: widen unsigned char to signed int (zero extend)
	Ccall _dispval
	ldaD R12,1000; reg:acon
	Ccall _delay
;				break;
	lbr L18
L21:
;				mp=(unsigned char *)(4096+val);
	cpy1 R11,R1
	zExt R11 ;CVUI2(reg)*: widen unsigned char to signed int (zero extend)*
	ldA2 R11,'O',R11,(4096); reg:addr
	cpy2 R0,R11 ;LOADP2(reg)
;				*mp+=1;
	ld1 R11,'O',R0,0
	zExt R11 ;CVUI2: widen unsigned char to signed int (zero extend)
	incm R11,1
	str1 R11,R0; ASGNU1(indaddr,reg)		DH
;				break;
	lbr L18
L22:
;				loc=(unsigned char *)(val+4096-2); //ugh
	cpy1 R11,R1
	zExt R11 ;CVUI2(reg)*: widen unsigned char to signed int (zero extend)*
	ldA2 R11,'O',R11,(4096); reg:addr
	decm R11,2	;SUBI2(reg,consm)
	cpy2 R7,R11 ;LOADP2(reg)
;				break;
	lbr L18
L23:
;				delay(val*4);
	cpy1 R11,R1
	zExt R11 ;CVUI2(reg)*: widen unsigned char to signed int (zero extend)*
	shl2I R11,2
	cpy2 R12,R11 ;LOADU2*(reg)
	Ccall _delay
;				break;
	lbr L18
L24:
;				dispval(getsp());
	Ccall _getsp;CALLU2(ar)*
	cpy1 R11,R15;LOADU1(reg)
	cpy1 R12,R11
	zExt R12 ;CVUI2(reg)*: widen unsigned char to signed int (zero extend)*
	Ccall _dispval
;				delay(250);
	ldaD R12,250; reg:acon
	Ccall _delay
;				break;
	lbr L18
L17:
;				dispval(0x41); delay(250);
	ldaD R12,65; reg:acon
	Ccall _dispval
	ldaD R12,250; reg:acon
	Ccall _delay
;				dispmemloc(loc); delay(5000);
	cpy2 R12,R7 ;LOADP2(reg)
	Ccall _dispmemloc
	ldaD R12,5000; reg:acon
	Ccall _delay
;				break;
L18:
;		loc+=2;
	incm R7,2
;	}
L15:
;	while(1){
	lbr L14
;	return loc;
	cpy2 R15,R7 ;LOADP2(reg)
L13:
	release 4; release room for outgoing arguments
	popr R7
	popr R6
	popr R1
	popr R0
	release 2; release room for local variables 
	Cretn

;$$function end$$ _execute
;$$function start$$ _dispalpha
_dispalpha:		;framesize=8
	pushr R7
	reserve 4; save room for outgoing arguments
	st2 R12,'O',sp,(8+1); flag1 
;void dispalpha(unsigned char data[]){
;	dispval(getsp()); //display
	Ccall _getsp;CALLU2(ar)*
	cpy1 R11,R15;LOADU1(reg)
	cpy1 R12,R11
	zExt R12 ;CVUI2(reg)*: widen unsigned char to signed int (zero extend)*
	Ccall _dispval
;	delay(100);
	ldaD R12,100; reg:acon
	Ccall _delay
;	initleds(0b11110000); //LEDs in no-decode mode
;	for (i=8;i!=0;i--){
 req
 seq
 dec 2
 ldi 0b11110000
 str 2
 out 7
 req
	ldaD R7,8; reg:acon
	lbr L31
L28:
;		out(7,boydsegments[data[i]]);
	ldaD R12,7; reg:acon
	alu2RRS R11,R7,'O',sp,(8+1),add,adc; ADDI2(r,INDIRP2(addr))	DH3.1
	ld1 R11,'O',R11,0
	zExt R11 ;CVUI2: widen unsigned char to signed int (zero extend)
	ld1 R13,'O',R11,(_boydsegments)
	zExt R13 ;CVUI2: widen unsigned char to signed int (zero extend)
	Ccall _out; CALLI2(ar)
;	}
L29:
;	for (i=8;i!=0;i--){
	decm R7,1	;SUBU2(reg,consm)
L31:
	jnzU2 R7,L28; NE 0 
;}
L26:
	release 4; release room for outgoing arguments
	popr R7
	Cretn

;$$function end$$ _dispalpha
;$$function start$$ _strlen
_strlen:		;framesize=4
	pushr R7
;{
;	unsigned int slen = 0 ;
	ld2z R7
	lbr L34
L33:
;	while (*str != 0) {
;      slen++ ;
	incm R7,1
;      str++ ;
	incm R12,1
;   }
L34:
;	while (*str != 0) {
	ldn1 R11,R12;reg:  INDIRU1(indaddr)
	jnzU1 R11,L33; NEI2(CVUI2(reg),con0)
;   return slen;
	cpy2 R15,R7 ;LOADU2*(reg)
L32:
	popr R7
	Cretn

;$$function end$$ _strlen
;$$function start$$ _dispstr
_dispstr:		;framesize=12
	pushr R1
	pushr R6
	pushr R7
	reserve 4; save room for outgoing arguments
	st2 R12,'O',sp,(12+1); flag1 
;void dispstr(unsigned char * str){//display 8 or fewer characters on the boyd LEDs
;	L=min(strlen((char *)str),8);//length to display
	ld2 R12,'O',sp,(12+1) ;reg:INDIRP2(addr)
	Ccall _strlen;CALLU2(ar)*
	cpy2 R11,R15 ;LOADU2*(reg)
	jcI2I R11,8,lbdf,L38; GE is flipped test from LT
	ld2 R12,'O',sp,(12+1) ;reg:INDIRP2(addr)
	Ccall _strlen;CALLU2(ar)*
	cpy2 R11,R15 ;LOADU2*(reg)
	cpy2 R1,R11 ;LOADU2*(reg)
	lbr L39
L38:
	ldaD R1,8; reg:acon
L39:
	cpy2 R6,R1 ;LOADU2*(reg)
;	initleds(0b11110000); //LEDs in no-decode mode
;	if (L<8){
 req
 seq
 dec 2
 ldi 0b11110000
 str 2
 out 7
 req
	jcI2I R6,8,lbdf,L41; GE is flipped test from LT
;		for(i=(L-8); i>0;i--){
	alu2I R7,R6,8,smi,smbi
	lbr L46
L43:
;			out(7,255);
	ldaD R12,7; reg:acon
	ldaD R13,255; reg:acon
	Ccall _out; CALLI2(ar)
;		}
L44:
;		for(i=(L-8); i>0;i--){
	decm R7,1	;SUBU2(reg,consm)
L46:
	jnzU2 R7,L43; NE 0 
;	}
L41:
;	for (i=L;i>0;i--){
	cpy2 R7,R6 ;LOADU2*(reg)
	lbr L50
L47:
;		out(7,boydsegments[str[i-1]]);
	ldaD R12,7; reg:acon
	cpy2 R11,R7	;SUBU2(reg,consm)
	decm R11,1	;SUBU2(reg,consm)
	alu2RRS R11,R11,'O',sp,(12+1),add,adc; ADDI2(r,INDIRP2(addr))	DH3.1
	ld1 R11,'O',R11,0
	zExt R11 ;CVUI2: widen unsigned char to signed int (zero extend)
	ld1 R13,'O',R11,(_boydsegments)
	zExt R13 ;CVUI2: widen unsigned char to signed int (zero extend)
	Ccall _out; CALLI2(ar)
;	}
L48:
;	for (i=L;i>0;i--){
	decm R7,1	;SUBU2(reg,consm)
L50:
	jnzU2 R7,L47; NE 0 
;}
L36:
	release 4; release room for outgoing arguments
	popr R7
	popr R6
	popr R1
	Cretn

;$$function end$$ _dispstr
;$$function start$$ _main
_main:		;framesize=16
	reserve 2; save room for local variables
	pushr R0
	pushr R1
	pushr R6
	pushr R7
	reserve 4; save room for outgoing arguments
;{
;	unsigned char * loc=0;
	ldaD R6,0; reg:acon
;	unsigned char memtype='o'; //displaying o=eeprom,a=ram
	ldaD R1,111; reg:acon
;	dispval(0x42);
	ldaD R12,66; reg:acon
	Ccall _dispval
;	delay(100);
	ldaD R12,100; reg:acon
	Ccall _delay
;	dispstr((unsigned char *)"BARRY");
	ldaD R12,L52; reg:acon
	Ccall _dispstr
;	delay(5000);
	ldaD R12,5000; reg:acon
	Ccall _delay
;	dispstr((unsigned char *)"01234567");
	ldaD R12,L53; reg:acon
	Ccall _dispstr
;	delay(5000);
	ldaD R12,5000; reg:acon
	Ccall _delay
	lbr L55
L54:
;	while(1){
;		dispmemloc(loc);
	cpy2 R12,R6 ;LOADP2(reg)
	Ccall _dispmemloc
;		k=boydscan();
	Ccall _boydscan; CALLI2(ar)
	cpy2 R11,R15 ;LOADU2*(reg)
	cpy1 R7,R11;LOADU1(reg)
;		switch(k){
	cpy1 R11,R7
	zExt R11 ;CVUI2(reg)*: widen unsigned char to signed int (zero extend)*
	st2 R11,'O',sp,(12+1); ASGNI2(addr,reg)
	ld2 R11,'O',sp,(12+1) ;reg:INDIRI2(addr)
	jcI2I R11,16,lbnf,L57  ;LT=lbnf i.e. subtract immedB from A and jump if borrow
	jnI2I R11,20,lbnf,L57; GT reverse  the subtraction
	shl2I R11,1
	ld2 R11,'O',R11,(L67-32) ;reg:INDIRP2(addr)
	jumpv R11; JUMPV(reg)
L67:
	dw L60
	dw L61
	dw L62
	dw L65
	dw L66
L60:
;				loc +=1;
	incm R6,1
;				break;
	lbr L58
L61:
;				loc -=1;
	ldA2 R6,'O',R6,(-1); reg:addr
;				break;
	lbr L58
L62:
;				if (memtype=='o'){
	jneU1I R1,111,L63	; DH 4
;					loc=(unsigned char *)4096;
	ldaD R6,0x1000; reg:acon
;					memtype='a';
	ldaD R1,97; reg:acon
;				}else{
	lbr L58
L63:
;					loc=(unsigned char *)0;
	ldaD R6,0; reg:acon
;					memtype='o';
	ldaD R1,111; reg:acon
;				}
;				break;
	lbr L58
L65:
;				dispmemloc(loc); //makes a blink
	cpy2 R12,R6 ;LOADP2(reg)
	Ccall _dispmemloc
;				k=boydscan(); dispval(k); delay(250);
	Ccall _boydscan; CALLI2(ar)
	cpy2 R11,R15 ;LOADU2*(reg)
	cpy1 R7,R11;LOADU1(reg)
	cpy1 R12,R7
	zExt R12 ;CVUI2(reg)*: widen unsigned char to signed int (zero extend)*
	Ccall _dispval
	ldaD R12,250; reg:acon
	Ccall _delay
;				k2=boydscan(); dispval(k2); delay(250);
	Ccall _boydscan; CALLI2(ar)
	cpy2 R11,R15 ;LOADU2*(reg)
	cpy1 R0,R11;LOADU1(reg)
	cpy1 R12,R0
	zExt R12 ;CVUI2(reg)*: widen unsigned char to signed int (zero extend)*
	Ccall _dispval
	ldaD R12,250; reg:acon
	Ccall _delay
;				*loc=(k<<4)+k2;
	cpy1 R11,R7
	zExt R11 ;CVUI2(reg)*: widen unsigned char to signed int (zero extend)*
	shl2I R11,4
	cpy1 R10,R0
	zExt R10 ;CVUI2(reg)*: widen unsigned char to signed int (zero extend)*
	alu2 R11,R11,R10,add,adc; ADDI2(r,r)
	str1 R11,R6; ASGNU1(indaddr,reg)		DH
;				break;
	lbr L58
L66:
;				dispval(0x45);
	ldaD R12,69; reg:acon
	Ccall _dispval
;				delay(250);
	ldaD R12,250; reg:acon
	Ccall _delay
;				loc=execute(loc);
	cpy2 R12,R6 ;LOADP2(reg)
	Ccall _execute
	cpy2 R6,R15 ;LOADP2(reg)
;				break;
	lbr L58
L57:
;				dispval(k);
	cpy1 R12,R7
	zExt R12 ;CVUI2(reg)*: widen unsigned char to signed int (zero extend)*
	Ccall _dispval
;				delay(250);
	ldaD R12,250; reg:acon
	Ccall _delay
;		}
L58:
;	}
L55:
;	while(1){
	lbr L54
;}
L51:
	release 4; release room for outgoing arguments
	popr R7
	popr R6
	popr R1
	popr R0
	release 2; release room for local variables 
	Cretn

;$$function end$$ _main
;$$function start$$ _delay
_delay:		;framesize=10
	pushr R6
	pushr R7
	reserve 4; save room for outgoing arguments
	cpy2 R7,R12; function(2054) 1
;void delay(unsigned int howlong){
;	for (i=1;i!=howlong;i++){
	ldaD R6,1; reg:acon
	lbr L73
L70:
;		oneMs();
	Ccall _oneMs; CALLI2(ar)
;	}
L71:
;	for (i=1;i!=howlong;i++){
	incm R6,1
L73:
	jneU2 R6,R7,L70; NE
;}
L69:
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
L74:
	Cretn

;$$function end$$ _olduinoincluder
;$$function start$$ _boydinc
_boydinc:		;framesize=2
;void boydinc(){
;	asm(" align 256\n");
;	asm(" include \"boydscan.inc\"\n");
 align 256
 include "boydscan.inc"
;}
L76:
	Cretn

;$$function end$$ _boydinc
L53:
	db 48
	db 49
	db 50
	db 51
	db 52
	db 53
	db 54
	db 55
	db 0
L52:
	db 66
	db 65
	db 82
	db 82
	db 89
	db 0
	include lcc1802epiloNW.inc
	include IO1802.inc