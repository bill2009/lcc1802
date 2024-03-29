	RELAXED ON
;definitions and prolog functions needed for smc1802 programs (Hi Bill)
;dec 21 packaged version for the christmas compiler (Ho Ho Ho)
;Dec 24 fixed shift macros to use memaddr as a work register and not corrupt the shift count
;jan 6 fixed shift left macro as above
;jan 11 saving as lcc1802epiloNG.inc for the NG compiler
;jan 12 minor correction to rldmi
;jan 14 minor correction to ldi4
;jan 16 adding shri4I
;jan20 correct error in shl2r
;jan21 moved 4 byte macros to bottom and added shrc4
;Jan 28 archived before beginning work on Birthday Compiler
;Feb 7 adding nointerrupts/interrupts to control interruptability
;Feb 13 changing address mode macros
;Feb 14 removing nointerrupts, adding reserve/release for stack frame, ld2z macro
;Mar 3 changing reserve/release to use inc/dec for 8 or less bytes
;Mar 4 adding incm macro for multiple increments
;mar 5 adding jzi2 macro to speed up if processing
;mar 6 adding ldn1, str1 for register indirect addressing
;mar 17 adding decm macro
;mar 28 adding jumpv macro
;may 15 adding jnzu1, jzu1 macros
;june 21 adding demote macro
;Oct 2, 2013 DH version for dhrystone optimization 
;Oct 2, 2013 added str2 macro 2 byte store at addr pointed to by register
;oct 4,	added st2i 2 byte immediate sore, pushf,pushm,pushl sequences, 
;oct 24, added ldaXs for stack pointed addresses, added mvcn1, mvc1 for 1 byte storage to storage moves, jneu1i for single byte immediate compare
;Feb 17 2014 added "inc sp" to restore stack pointer in jeqI1, affected strncmp.
;Feb 18 2017 added org to LCCCODELOC to allow for non-zero origin
;Feb 19 2017 beginning to adapt for 1806
;17-03-06 remove inc/dec from ccall, cretn6, add inc to popr
;17-03-07 add popf,popm,popl for optimization
;17-03-14 removed savemi, rldmi,cretn6
;17-03-27 added jequ1i
;17-10-17 compatability with clean stack discipline
;         changed pushr, removed pushf pushr pushm, removed many dec/inc sp's for work areas
;17-10-19 changed LCCCODELOC to CODELOC
;18-02-08 changed branch to lcc1802Init to lcc1802init to match epilog
;18-02-09 moved labels off macro calls e.g. $$rcomp
;18-02-14 replaced $$nobr temporary label with + in jequ2i, jeqi2
;18-01-11 changed MACEXP OFF to MACEXP_DFT for version 1.42 of the assembler
;18-01-25 changed syntax for SCAL
;20-04-28 specific version for COMX-35/Crosslib
;20-05-01 fixed names of pushf, pushr, pushm.  may indicate a problem somewhere.
;20-05-01 copying R15.1 to D before returns to accommodate COMX call/return
;20-05-07 imlementing Ccall macro for calls to a function pointer
;20-05-08 goobered pushf/m/l to just pushr - should never have changed names.
;20-05-10 changed indirect call to detect * as the signal i.e. ccall *R11  means call the address in R11
;20-05-12 added JNEU1 as single byte unsigned compare
;20-06-02 updated to support DATALOC in combination with a change to the compiler.  macros orgd,orgc, tracking variable CSEG for current segment
;20-06-19 fixed set of CODELOC
;20-07-14 added jcU1 single byte compare - required change to the md file and binary
;20-11-28 added st1i single byte immediate store for afterlife optimization
;21-02-15 cumulative changes since CX version
;         xbxx macros invoke subroutines to fake long branches, chbr macro chooses long or emulated branch based on LCCPX
;	  pagefit nn macro makes sure there are nn bytes left in page, xsnf chooses lsnf or pagefit/br $+4 based on LCCPX
;         some complex compares (jci4,jcf4) changed labels to avoid problems when nesting
;	  target=xr18CX chooses either lcc1802CX or lcc1802PX depending on option -pixie

  IFNDEF CODELOC		;wjr 20-05-29 allow code relocation
      SET CODELOC,0		;wjr 20-05-29 allow code relocation
  ENDIF				;wjr 20-05-29 allow code relocation
      org CODELOC		;wjr 20-05-29 allow code relocation
  set CSEG,1			;wjr-20-06-2 set current segment to code
  
R0:	equ	0
R1:	equ	1
R2:	equ	2
R3:	equ	3
R4:	equ	4
R5:	equ	5
R6:	equ	6
R7:	equ	7
R8:	equ	8
R9:	equ	9
R10:	equ	10
R11:	equ	11
R12:	equ	12
R13:	equ	13
R14:	equ	14
R15:	equ	15
RL0:	equ	1 ;long register pairs are identified by their odd numbered register
 if MOMCPU=$1805
RL4:	equ	5 ;17-10-20 RL4/5 register pair is permitted for 1806 only.
 endif
RL6:	equ	7 
RL8:	equ	9 ;temp 1
RL10:	equ	11;temp 2
RL12:	equ	13 ;return value register for longs
Rp1p2:	equ	13 ;argument register for longs
Rt1:	equ	8  ;1st temp register
Rt2:	equ	9  ;2nd temp register
RCALL:	equ 	4 ;standard call routine
RRET:	equ 	5 ;standard return register
RPC:	equ 	3 ; standard program counter


	listing	off
	IF VERSION<=0x142F ;syntax for turning off macro expansion changed with version 4.1.2
		MACEXP OFF	
	ELSE	
		MACEXP_DFT OFF
	ENDIF
;macro definitions
globss: macro	size	;global blank storage definition
;printf(";\tdb %d dup (0); zerofill global\n", p->type->size);
;printf("\tglobss %d; define global BSS\n", p->type->size);
	IFDEF	NOFILLBSS
	    ds  size	;this reserves storage but doesn't fill
	ELSE
	    db 	size dup (0); zerofill global
	ENDIF
	endm

orgc:	macro		;switching from code to data - rom to ram
     IFDEF  DATALOC
     	IF  CSEG=2	;if we're in a data segment
            set DATALOC,$ ;remember the location counter
	    org CODELOC	;return to the code location counter
	    set CSEG,1	;remember we're doing code
	ENDIF
     ENDIF
	endm
	
orgd:	macro		;switching from data to code - ram to rom
     IFDEF  DATALOC
     	IF CSEG=1	;if we're in a code segment
            set CODELOC,$ ;remember the code location counter
	    org DATALOC
	    set CSEG,2	;remember we're doing data
	ENDIF
     ENDIF
	endm

jumpv:	macro	addrReg	;jump to an address in a register
	pushr	r6	;save the real return address
	cpy2	r6,addrReg	;put the jump address into R6
	Cretn		;and "return" to it
	endm
incm:	macro	reg,count
	rept	count
	inc	reg
	endm
	endm
decm:	macro	reg,count
	rept	count
	dec	reg
	endm
	endm
reserve: macro	framesize	;reserve the stack frame
	if (framesize<9)
	    reserveS framesize	;use dec for small amounts
	else
	    reserveL framesize	;use subtract for large amounts
	endif
	endm
reserveS: macro	framesize
	rept	framesize
	dec sp
	endm
	endm
reserveL: macro framesize
;this calculation looks awkward but we're adjusting the top byte of sp first 
;so if there's an interrupt in mid calculation the sp will be pointing to free memory
	glo sp		;grab low byte of stack pointer
	smi framesize&255 ;claculate new low byte
	plo memAddr	;save it for a moment
	ghi sp		;grab high byte
	smbi (framesize>>8)&255	;calculate the new page
	phi sp		;adjust page first
	glo memAddr	;get new low byte
	plo sp		;finish adjusting the stack pointer
	endm
release: macro	framesize	;release the stack frame
	if (framesize<9)
	    releaseS framesize	;use dec for small amounts
	else
	    releaseL framesize	;use subtract for large amounts
	endif
	endm
releaseS: macro	framesize
	rept	framesize
	inc sp
	endm
	endm
releaseL: macro	framesize	;release the stack frame
;for release we adjust the low byte first 
;so if there's an interrupt in mid calculation the sp will be pointing to free memory
	alu2i sp,sp,framesize,adi,adci
	endm
blkcpy:	macro	tgt,src,len	;move memory length len from memory addressed by src to memory addressed by tgt - 19 bytes, lb@5-12, lb@16-4
	ldiReg	retVal,len	;length to move
$$nxt:	glo	retVal
	lbnz	$$go
	ghi	retVal
	lbz	$$done
$$go:	lda	src		;get byte to move - nb, src reg altered
	str	tgt		;place in dest 
	inc	tgt		;advance target ptr - nb dest reg altered
	dec	retval		;decrease move count
	lbr	$$nxt
$$done:
	endm
	
__BRTYP		set	""
chbr:	macro	BROP,DEST	;chooses lbr type opcodes or xbr macros depending on setting LCCPX
__BRTYP	  set	substr("BROP",1,strlen("BROP"))
  	    IFDEF	LCCPX
	      X{__BRTYP} DEST
  	    ELSE
  	      BROP	DEST
  	    ENDIF
	endm
pagefit: macro bytesneeded	
	if ($#256)>(255-bytesneeded)
	    align 256,0xe2
	endif
	endm
xsnf:	macro	;either long skip or short branch around 2 bytes for pixie
  IFDEF LCCPX
  	pagefit 5
  	bnf	$+4	;branch around following 2 bytes
  ELSE
  	lsnf		;no pixie, no problem
  ENDIF
  	endm
xsdf:	macro	;either long skip or short branch around 2 bytes for pixie
  IFDEF LCCPX
  	pagefit 5
  	bnf	$+4	;branch around following 2 bytes
  ELSE
  	lsnf		;no pixie, no problem
  ENDIF
  	endm
	
jcI2:	macro	reg1,reg2,brop,label	;signed comparison -20 bytes, ls@13,lb@17
	glo	reg2
	str	sp 
	glo	reg1
	sm
	ghi	reg2
	str	sp
	ghi	reg1
	smb          ;that's a standard signed subtraction
	ghi	reg1 ;
	xor           ;sets the top bit if the signs are different
 	shlc          ;the original df is now in bit 0 and df=1 if signs were different
	xsnf	;bypass the flip if signs were the same
	xri	01     ;invert original df if signs were different
	shrc           ;put it back in df
	chbr	brop,label  ;execute 
	endm

jcI4:	macro	reg1,reg2,brop,label	;signed comparison of two longs -28 bytes, ls@21, lbr@25
	dec	sp ;make a work area
	glo	reg2	;lowest order byte
	str	sp 
	glo	reg1
	sm
	ghi	reg2
	str	sp
	ghi	reg1
	smb          ;that's a standard signed subtraction of one reg
	glo	reg2-1	;lowest order byte of the top register
	str	sp 
	glo	reg1-1
	smb
	ghi	reg2-1
	str	sp
	ghi	reg1-1
	smb          ;that's a standard signed subtraction of a double register
	ghi	reg1-1 ;
	xor           ;sets the top bit if the signs are different
	inc	sp ;release the work area
	shlc          ;the original df is now in bit 0 and df=1 if signs were different
	xsnf	;bypass the flip if signs were the same
	xri	01     ;invert original df if signs were different
	shrc           ;put it back in df
	chbr	brop,label  ;execute 
	endm

jcU1:	macro	reg1,reg2,brop,label ;
	glo	reg2
	str	sp
	glo	reg1
	sm
	chbr	brop,label
	endm
jcU2:	macro	reg1,reg2,brop,label ;13 bytes, lbr@10
	dec	sp
	glo	reg2
	str	sp
	glo	reg1
	sm
	ghi	reg2
	str	sp
	ghi	reg1
	smb
	inc	sp
	chbr	brop,label
	endm
jcU4:	macro	reg1,reg2,brop,label	;21 bytes, lbr@18
	dec	sp
	glo	reg2	;start with the low byte of the low order register
	str	sp
	glo	reg1
	sm
	ghi	reg2
	str	sp
	ghi	reg1
	smb		;that's the low order register comparison done
	glo	reg2-1
	str	sp
	glo	reg1-1
	smb
	ghi	reg2-1
	str	sp
	ghi	reg1-1	;finish with the high byte of the high order register
	smb
	inc	sp
	chbr	brop,label
	endm
jcI1I:	macro	reg1,ival,brop,label	;signed comparison - one byte immediate -for nopromo
	glo	reg1
	smi	(ival)#256
	chbr	brop,label  ;execute 
	endm
jcI2I:	macro	reg1,ival,brop,label	;signed comparison - register immediate -17 bytes, ls@10, lbr@14
	glo	reg1
	smi	(ival)#256
	ghi	reg1
	smbi    (ival)>>8; was/256	;that's a standard signed subtraction
	ghi	reg1 ;
	xri	(ival)>>8; was/256	;sets the top bit if the signs are different
	shlc          ;the original df is now in bit 0 and df=1 if signs were different
	xsnf	;bypass the df flip if signs were the same
	xri	01     ;invert original df if signs were different
	shrc           ;put it back in df
	chbr	brop,label  ;execute 
	endm
jnI2I:	macro	reg1,ival,brop,label	;reverse signed comparison - register immediate -17 bytes, ls@10, lbr@14
	glo	reg1
	sdi	(ival)#256	;subtract d FROM immediate value
	ghi	reg1
	sdbi    (ival)>>8; was/256	;that's a standard signed subtraction (of register FROM immediate)
	ghi	reg1 ;
	xri	(ival)>>8; was/256	;sets the top bit if the signs are different
	shlc          ;the original df is now in bit 0 and df=1 if signs were different
	xsnf	;bypass the df flip if signs were the same
	xri	01     ;invert original df if signs were different
	shrc           ;put it back in df
	chbr	brop,label  ;execute 
	endm
jcU2I:	macro	reg1,ival,brop,label ;-9 bytes, lbr@6
	glo	reg1
	smi	(ival)#256
	ghi	reg1
	smbi	(ival)>>8; was/256
	chbr	brop,label
	endm
jnU2I:	macro	reg1,ival,brop,label	;reverse unsigned comparison - 9 byes, lbr@6
	glo	reg1
	sdi	(ival)#256	;subtract d FROM immediate value
	ghi	reg1
	sdbi	(ival)>>8; was/256 ; sfinish subtracting register FROM immediate value
	chbr	brop,label
	endm
jneU2:	macro	reg1,reg2,label	;-18 bytes, lbr@6, lbr@15
	dec	sp
	glo	reg2
	str	sp
	glo	reg1
	sm
	inc	sp
	chbr	lbnz,label
	dec	sp
	ghi	reg2
	str	sp
	ghi	reg1
	smb
	inc	sp
	chbr	lbnz,label
	endm
jneU1:	macro	reg1,reg2,label	;-18 bytes, lbr@6, lbr@15
	dec	sp
	glo	reg2
	str	sp
	glo	reg1
	sm
	inc 	sp
	chbr	lbnz,label
	endm

jneU2I:	macro	reg1,ival,label	;-12 bytes, lbr@3, lbr@9
	glo	reg1
	smi	(ival)#256
	chbr	lbnz,label
	ghi	reg1
	smbi	(ival)>>8; was/256
	chbr	lbnz,label
	endm
jneU2NI: macro	reg1,ival,label	;jump if storage at reg1%!=lval
	lda	reg1
	smi	(ival)>>8; was/256
	chbr	lbnz,label
	ldn	reg1
	smi	(ival)#256
	chbr lbnz,label
	endm


jneU1I:	macro	reg1,immedval,label
	glo	reg1
	smi	immedval
	chbr	lbnz,label
	endm
jnzU1:	macro	reg1,label	;4 bytes, lbr@1
	glo	reg1
	chbr 	lbnz,label
	endm
jnzU2:	macro	reg1,label	;8 bytes, lbr@1, lbr@5
	glo	reg1
	chbr	lbnz,label
	ghi	reg1
	chbr	lbnz,label
	endm
jzU1:	macro	reg1,label	;4 bytes, lbr@1
	glo	reg1
	chbr	lbz,label
	endm
jzU2:	macro	reg1,label	;8 bytes, lbr@1-8, lbr@5
	glo	reg1
	chbr	lbnz,+
	ghi	reg1
	chbr	lbz,label
+
	endm
jeqI4:	macro	reg1,reg2,label	;36 bytes, lbr@6-36, lbr@15-36, lbr@24-36,lbr@33
	glo	reg2	;get the low byte of the low register of the pair for the 2nd operand
	str	sp
	glo	reg1
	sm		;subtract from corresponding byte of the 1st operand
	chbr	lbnz,noEQbr	;bail on test if not equal
	ghi	reg2	;repeat for high order byte of low register
	str	sp
	ghi	reg1
	sm
	chbr	lbnz,noEQbr
	glo	reg2-1	;low order byte of high order register
	str	sp
	glo	reg1-1
	sm
	chbr	lbnz,noEQbr
	ghi	reg2-1	;lhigh order byte of high order register
	str	sp
	ghi	reg1-1
	sm
	chbr	lbz,label	;if we get to here all other bytes have been equal so branch if this one is too
noEQbr:
	endm

jneu4:	macro	reg1,reg2,label	;39 bytes, lbr@6-36, lbr@15-36, lbr@24-36,lbr@33-39, lbr@36
	jeqi4	reg1,reg2,noNEbr
	chbr	lbr,label
noNEbr
	endm

jeqI2:	macro	reg1,reg2,label	;18 bytes, lbr@6, lbr@15
	glo	reg2
	str	sp
	glo	reg1
	sm
	chbr	lbnz,+
	ghi	reg2
	str	sp
	ghi	reg1
	smb
	chbr	lbz,label
+:
	endm
jeqI1:	macro	reg1,reg2,label	;18 bytes, lbr@6, lbr@15
	glo	reg2
	str	sp
	glo	reg1
	sm
	chbr	lbz,label
	endm
jeqU1I:	macro	reg1,ival,label	;12 bytes, lbr@3-12, lbr@9
	glo	reg1
	smi	(ival)#256
	chbr	lbz,label
	endm
jeqU2I:	macro	reg1,ival,label	;12 bytes, lbr@3-12, lbr@9
	glo	reg1
	smi	(ival)#256
	chbr	lbnz,+
	ghi	reg1
	smbi	(ival)>>8; was/256
	chbr	lbz,label
+:
	endm
ld2z:	macro	reg1	;load zero into a 16 bit register
	ldi	0
	plo	reg1
	phi	reg1
	endm
ldAD:	macro	reg1,directaddress	;load an absolute address or a constant into a register
 if MOMCPU=$1805
 	RLDI	reg1,directaddress
 else
	ldi	(directaddress)&255
	plo	reg1
	ldi	(directaddress)>>8; was/256
	phi	reg1
 endif
	endm
ldAX:	macro	reg1,basereg,offset	;load a base+offset address into a register
	glo	basereg
	adi	(offset)#256
	plo	reg1
	ghi	basereg
	adci	(offset)>>8; was/256
	phi	reg1
	endm
ldAXs:	macro	reg1,basereg,offset	;load a stack-pointer+offset address into a register
	if	(offset>1)
		ldaX reg1,basereg,offset
	else
		cpy2 reg1,basereg
		incm reg1,offset
	endif
	endm
ldA2:	macro	reg1,OorD,destBase,destOff ;2 byte load address direct or base+offset
;Oct 13 destoff added to ldAD call
	if	(OorD='O')
		ldAX	reg1,destbase,destoff
	else
		ldAD	reg1,destbase+destoff
	endif
	endm
mvc2:	macro	reg1,OorDdest,destBase,destOff,OorDsrc,srcBase,srcOff ;moves 2 bytes from storage to storage
	ldA2	reg1,OorDdest,destBase,destOff
	ldA2	memaddr,OorDsrc,srcBase,srcOff
	lda	memaddr
	str	reg1
	inc 	reg1
	ldn	memaddr
	str	reg1
	endm

mvcn2:	macro	regdest,regsrc 	;moves 2 bytes from storage to storage where addresses are already in registers
	lda	regsrc
	str	regdest
	inc 	regdest
	ldn	regsrc
	str	regdest
	dec regsrc
	dec regdest
	endm

mvc1:	macro	reg1,OorDdest,destBase,destOff,OorDsrc,srcBase,srcOff ;moves 2 bytes from storage to storage
	ldA2	reg1,OorDdest,destBase,destOff
	ldA2	memaddr,OorDsrc,srcBase,srcOff
	ldn	memaddr
	str	reg1
	endm


mvcn1:	macro	regdest,regsrc 	;moves 1 bytes from storage to storage where addresses are already in registers
	ldn	regsrc
	str	regdest
	endm

cpy2:	macro	tgt,src			;2 byte register copy
	glo	src
	plo	tgt
	ghi	src
	phi	tgt
	endm
cpy1:	macro	tgt,src			;1 byte register copy
	glo	src
	plo	tgt
	endm
zext:	macro	reg			;zero extend(clear) the top byte of register
	ldi	0
	phi	reg
	endm
sext:	macro	reg			;sign extend the byte in d to the register
	shl		;put the sign in df
	ldi	0	;clear bits 0-7
	shlc		;sign to bit 0
	xri	1	;bit 0 is now 1 for positive numbers, 0 for negative
	smi	1	;D is now 0 for +v numbers, FF for -v
	phi	reg	;et voila
	endm
sext4:	macro	lreg	;sign extend from the bottom register to the top of a register pair
	ghi	lreg	;get the top of the bottom of the pair (long regs are addressed by the low member)
	shl		;put the sign in df
	ldi	0	;clear bits 0-7
	shlc		;sign to bit 0
	xri	1	;bit 0 is now 1 for positive numbers, 0 for negative
	smi	1	;D is now 0 for +v numbers, FF for -v
	plo	lreg-1	;low order of top of pair
	phi	lreg-1	;high byte
	endm	
zext4:	macro	lreg	;clear the top of a register pair
	ldi	0	;source a 0
	plo	lreg-1	;low order of top of pair
	phi	lreg-1	;high byte
	endm	
shL2I:	macro	reg,n	;shift register left n bits
	rept	n
	shl2	reg
	endm
	endm
shl2:	macro	reg	;shift register left 1 bir
	glo	reg
	shl
	plo	reg
	ghi	reg
	shlc
	phi	reg
	endm
shlR2R:	macro	dest,src	;shift register left 1 bit transferring to another register
	glo	src
	shl
	plo	dest
	ghi	src
	shlc
	phi	dest
	endm
shL2R:	macro	reg,regn	;shift register left by count in regn
	glo	regn		;the + and - are nameless temporary symbols
	lbz	+
	plo	memaddr
-	shl2	reg
	dec	memaddr
	glo	memaddr
	lbnz	-
+
	endm
shRU2I:	macro	reg,n	;shift register right n bits - unsigned
	rept	n
	shrU2	reg
	endm
	endm
shrU2:	macro	reg	;shift register right once - unsigned
	ghi	reg
	shr
	phi	reg
	glo	reg
	shrc
	plo	reg
	endm
shRU2R:	macro	reg,regn	;shift register right by count in regn - unsigned
	glo	regn		;the + and - are nameless temporary symbols
	lbz	+
	plo	memaddr
-	shrU2	reg
	dec	memaddr
	glo	memaddr
	lbnz	-
+
	endm
shrI2I:	macro	reg,n	;shift register right n bits - signed
	rept	n
	shrI2	reg
	endm
	endm
shrI2:	macro	reg	;shift register right once - signed
	ghi	reg
	shl		;set DF to the sign
	ghi	reg	;get the top byte back
	shrc		;shift one bit extending the sign
	phi	reg
	glo	reg
	shrc
	plo	reg
	endm
shRI2R:	macro	reg,regn	;shift register right by count in regn - signed
	glo	regn		;the + and - are nameless temporary symbols
	plo	memaddr		;save the shift count
	lbz	+
-	shrI2	reg
	dec	memaddr		;decrement the shift count
	glo	memaddr		;get it
	lbnz	-
+
	endm
negI2:	macro	tgt,src			;two byte negation from src to tgt
	glo	src			;(flip all the bits and add 1)
	xri	0xff
	plo	tgt
	ghi	src
	xri	0xff
	phi	tgt
	inc	tgt
	endm
alu2I: macro	tgt,src,imm,op1,op2	;2 byte register/immediate alu operation 
	glo 	src
	op1	(imm)#256
	plo	tgt
	ghi	src
	op2	(imm)>>8; was/256
	phi	tgt
	endm
alu1: macro	tgt,src1,src2,op1,op2	;1 byte register/register alu operation with 9 bit result 
	glo 	src2
	str	sp
	glo	src1
	op1		;calculate the low order byte
	plo	tgt
	ldi	0
	op2	0		;calculate the high byte in case of carry/borrow
	phi	tgt
	endm	
alu2: macro	tgt,src1,src2,op1,op2	;2 byte register/register alu operation 
	glo 	src2
	str	sp
	glo	src1
	op1		;calculate the low order byte
	plo	tgt
	ghi	src2
	str	sp
	ghi	src1
	op2		;calculate the high byte
	phi	tgt
	endm	
alu2RRS: macro 	tgtreg,opreg,OorD,basereg,offset,op1,op2 ;adds register to storage result in register
	lda2	memaddr,OorD,basereg,offset+1
	sex	memaddr
	glo	opreg
	op1
	plo	tgtreg
	dec	memaddr
	ghi	opreg
	op2
	phi	tgtreg
	sex	sp
	endm

st2:	macro	reg,OorD,destBase,destOff ;2 byte store to direct memory address or base+offset
	if	(OorD='O')
		ldAX	memAddr,destbase,destoff
	else
		ldAD	memAddr,destBase
	endif
	ghi	reg
	str	memAddr
	inc	memAddr
	glo	reg
	str	memAddr
	endm
st1I:	macro	immedval,OorD,destBase,destOff ;1 byte store of a literal to direct memory address or base+offset
	if	(OorD='O')
		ldAX	memAddr,destbase,destoff
	else
		ldAD	memAddr,destBase
	endif
	ldi	(immedval) & 255;low byte
	str	memAddr
	endm
st2I:	macro	immedval,OorD,destBase,destOff ;2 byte store of a literal to direct memory address or base+offset
	if	(OorD='O')
		ldAX	memAddr,destbase,destoff
	else
		ldAD	memAddr,destBase
	endif
	ldi	(immedval)>>8; top byte
	str	memAddr
	inc	memAddr
	ldi	(immedval) & 255;low byte
	str	memAddr
	endm
ld2:	macro	reg,OorD,destBase,destOff ;2 byte load from direct memory address or base+offset
	if	(OorD='O')
		ldAX	memAddr,destbase,destoff
	else
		ldAD	memAddr,destBase
	endif
	lda	memAddr
	phi	reg
	ldn	memAddr
	plo	reg
	endm
ld1:	macro	reg,OorD,destBase,destOff ;1 byte load from direct memory address or base+offset
	if	(OorD='O')
		ldAX	memAddr,destbase,destoff
	else
		ldAD	memAddr,destBase
	endif
	ldn	memAddr
	plo	reg
	endm
ldn1:	macro	reg,addrReg ;1 byte load from address pointed to by a register
	ldn	addrReg
	plo	reg
	endm
ldn2s:	macro	reg,addrReg ;2 byte load from address pointed to by a register
	;safe to use for same register
	lda	addrReg	;get 1st byte
	phi	memaddr	;save it
	ldn	addrReg	;second byte
	dec	addrReg	;restore address reg
	plo	reg	;1st byte to dest
	ghi	memaddr	;2nd byte
	phi	reg	;to destination (7 instructions vs 10)
	endm
ldn2:	macro	reg,addrReg ;2 byte load from address pointed to by a register
	;not safe to use for same register
	lda	addrReg	;get 1st byte
	phi	reg	;to destination
	ldn	addrReg	;second byte
	dec	addrReg	;restore address reg
	plo	reg	;2nd byte to dest (5 instructions vs 10)
	endm
demote:	macro	offset	;demotes an int to a char -usually a parameter
	ldAX	memaddr,sp,(offset+1)	;point to low order byte
	ldn	memaddr			;pick up low order byte
	dec	memaddr			;back off 1
	str	memaddr			;store it
	endm
st1:	macro	reg,OorD,destBase,destOff ;1 byte store to direct memory address or base+offset
	if	(OorD='O')
		ldAX	memAddr,destbase,destoff
	else
		ldAD	memAddr,destBase
	endif
	glo	reg
	str	memAddr
	endm
str1:	macro	reg,addrReg ;1 byte store to address pointed to by a register
	glo	reg
	str	addrReg
	endm
str1I:	macro	value,addrReg ;1 byte store immediate value to address pointed to by a register
	ldi	value
	str	addrReg
	endm
str2:	macro	reg,addrReg ;2 byte store to address pointed to by a register
	ghi	reg
	str	addrReg
	glo	reg
	inc	addrReg
	str	addrReg
	dec	addrReg
	endm
str2I:	macro	immedval,addrReg ;2 byte immediate store to address pointed to by a register
	ldi	(immedval)>>8; top byte
	str	addrReg
	inc	addrReg
	ldi	(immedval) & 255;low byte
	str	addrReg
	dec	addrReg
	endm

	
nop1806: macro	;used to inject no-ops to balance timing with an 1802
 if MOMCPU=$1805
	nop	;waste 1.5 instruction times
 endif
	endm
CcallD:	macro	target ;direct call
   IFDEF LCCCX
 	ghi	r15	;#20-05-01 for COMX
   ENDIF
 if MOMCPU=$1805
 	IF VERSION<0x142F ;syntax for SCAL changed with version 4.1.2
 		SCAL	6
		dw	target
 	ELSE	
 		SCAL	6,target
 	ENDIF

 else
	sep	RCALL
	dw	target
 endif
	endm

Cretn:	macro
 if MOMCPU=$1805
 	sret	6
 else
   IFDEF LCCCX
 	ghi	r15	;#20-05-01 for COMX
   ENDIF
	sep	RRET
 endif
	endm

pushr:	macro	reg
 if MOMCPU=$1805
 	RSXD	reg
 else
	glo	reg
	stxd
	ghi	reg
	stxd
 endif
	endm
Ccall  macro   addr    ;call to an address that may be in a register
        if      substr("addr",0,1)="*"
   	  ;address is in a register so we use an indirect call
	  pushr	r6	;save reg 6
	  ldi	$$ret&255 ;return address low byte
	  stxd
	  ldi	($$ret>>8)&255 ;return address high byte
	  stxd
	  cpy2	r6,VAL(substr("addr",1,strlen("addr")-1)) ;put the jump address into R6
	  Cretn		;and "return" to it
$$ret:
        elseif
	  CcallD addr ;use the original call macro
        endif
        endm

pushf:	macro	reg	;push 1st in a sequence of registers
	pushr	reg
	endm
pushm:	macro	reg	;push middle in a sequence of registers
	pushr	reg
	endm
pushl:	macro	reg	;push last in a sequence of registers
	pushr	reg
	endm
popr:	macro	reg	;routine standalone pop - inc's sp before load
 	inc	sp
 	lda	sp
 	phi	reg
 	ldn	sp
 	plo	reg
	endm
popf:	macro	reg	;first pop in a string - uses RLXA, leaves stack 'dirty'
 if MOMCPU=$1805
 	inc 	sp
 	RLXA	reg
 else
 	inc	sp
	lda	sp
	phi	reg
	lda	sp
	plo 	reg
 endif
	endm
popm:	macro	reg	;middle pop - uses RLXA - leaves stack dirty
 if MOMCPU=$1805
 	RLXA	reg
 else
	lda	sp
	phi	reg
	lda	sp
	plo 	reg
 endif
	endm
popl:	macro	reg	;final pop - assumes sp already inc'd - leaves stack 'clean'
 	lda	sp
 	phi	reg
 	ldn	sp
 	plo	reg
 	endm
	
;more natural 1802 macros
ldiReg:	macro	reg,value
 if MOMCPU=$1805
 	RLDI	reg,value
 else
	ldi	(value)&255
	plo	reg
	ldi	(value)>>8; was/256
	phi	reg
 endif
	endm		

inc4:	macro	Lreg	;increment a 4 byte register pair
	alu4i	Lreg,Lreg,1,adi,adci
	endm
	
alu4i:	macro	ltgt,lsrc,immval,op1,op2
	glo	Lsrc	;long regs are equated to the second reg which has the low order word 
	op1	(immval)&255
	plo	Ltgt
	ghi	Lsrc
	op2	((immval)>>8)&255; 
	phi	Ltgt
	glo	Lsrc-1
	op2	((immval)>>16)&255; 
	plo	Ltgt-1
	ghi	Lsrc-1
	op2	((immval)>>24)&255; 
	phi	Ltgt-1
	endm

ldi4	macro	lreg,immval ;4 byte load signed literal
	ldi	(immval)&255
	plo	lreg
	ldi	((immval)>>8)&255; 
	phi	lreg
	ldi	((immval)>>16)&255; 
	plo	lreg-1
	ldi	((immval)>>24)&255; 
	phi	lreg-1
	endm

cpy4:	macro	tgt,src			;4 byte register pair copy
	cpy2	tgt,src	;long reg pairs are addressed as the low order pair
	cpy2	tgt-1,src-1
	endm	
st4:	macro	reg,OorD,destBase,destOff ;4 byte store to direct memory address or base+offset
	if	(OorD='O')
		ldAX	memAddr,destbase,destoff
	else
		ldAD	memAddr,destBase
	endif
	ghi	reg-1	;long register pairs are addressed by their second member
	str	memAddr ;but load and store start with the 1st
	inc	memAddr
	glo	reg-1
	str	memAddr
	inc	memAddr
	ghi	reg
	str	memAddr
	inc	memAddr
	glo	reg
	str	memAddr
	endm
ld4:	macro	reg,OorD,destBase,destOff ;4 byte load from direct memory address or base+offset
	if	(OorD='O')
		ldAX	memAddr,destbase,destoff
	else
		ldAD	memAddr,destBase
	endif
	lda	memAddr
	phi	reg-1	;long register pairs are addressed by their second member
	lda	memAddr ;but load and store start with the 1st
	plo	reg-1
	lda	memAddr
	phi	reg
	ldn	memAddr
	plo	reg
	endm

alu4: macro	tgt,src1,src2,op1,op2	;4 byte register/register alu operation 
	dec	sp ;make a work ares
	glo 	src2	;long register pairs are addressed by their second member
	str	sp	;so arithmetic operations start there
	glo	src1
	op1		;calculate the low order byte
	plo	tgt
	ghi	src2
	str	sp
	ghi	src1
	op2		;calculate the second byte
	phi	tgt
	glo 	src2-1
	str	sp
	glo	src1-1
	op2		;calculate the third byte
	plo	tgt-1
	ghi	src2-1
	str	sp
	ghi	src1-1
	op2		;calculate the high byte
	phi	tgt-1
	inc	sp 	;release the work area
	endm	
shrI4I:	macro	reg,n	;shift register right n bits - signed
	rept	n
	shrI4	reg
	endm
	endm
shRI4R:	macro	reg,regn	;shift long register right by count in regn - signed
	glo	regn		;the + and - are nameless temporary symbols
	plo	memaddr		;save the shift count
	lbz	+
-	shrI4	reg
	dec	memaddr		;decrement the shift count
	glo	memaddr		;get it
	lbnz	-
+
	endm
shrI4:	macro	reg	;shift register right once - signed
	ghi	reg-1	;long reg pairs start at reg-1
	shl		;set DF to the sign
	ghi	reg-1	;get the top byte back
	shrc		;shift one bit extending the sign
	phi	reg-1
	glo	reg-1
	shrc
	plo	reg-1
	ghi	reg	;get the top byte of the low order reg
	shrc		;shift one bit extending the sign
	phi	reg
	glo	reg	;finish with the low byte of the 2nd reg of the pair
	shrc
	plo	reg
	endm
shRC4:	macro	reg	;shift register right once - signed continuing from previous shift
	ghi	reg-1	;long reg pairs start at reg-1
	shrc		;shift one bit extending the sign
	phi	reg-1
	glo	reg-1
	shrc
	plo	reg-1
	ghi	reg	;get the top byte of the low order reg
	shrc		;shift one bit extending the sign
	phi	reg
	glo	reg	;finish with the low byte of the 2nd reg of the pair
	shrc
	plo	reg
	endm
shrU4I:	macro	reg,n	;shift register right n bits - signed
	rept	n
	shrU4	reg
	endm
	endm
shRU4R:	macro	reg,regn	;shift long register right by count in regn - signed
	glo	regn		;the + and - are nameless temporary symbols
	plo	memaddr		;save the shift count
	lbz	+
-	shrU4	reg
	dec	memaddr		;decrement the shift count
	glo	memaddr		;get it
	lbnz	-
+
	endm
shrU4:	macro	reg	;shift long register right once - usigned
	ghi	reg-1	;long reg pairs start at reg-1
	shr		;shift one bit 
	phi	reg-1
	glo	reg-1
	shrc		;continue the shift
	plo	reg-1
	ghi	reg	;get the top byte of the low order reg
	shrc		;continue the shift
	phi	reg
	glo	reg	;finish with the low byte of the 2nd reg of the pair
	shrc
	plo	reg
	endm
shL4:	macro	reg	;shift long register left once
	glo	reg	;start with low byte of second register
	shl		;shift left once
	plo	reg	; save it
	ghi	reg	;high byte of second reg
	shlc		;shift one bit carrying
	phi	reg	;save it
	glo	reg-1	;now the bottom byte of top reg
	shlc
	plo	reg-1
	ghi	reg-1	;finally the top byte of the high order reg
	shlc		;gets the last shift
	phi	reg-1	;and we're done
	endm

shL4R:	macro	reg,regn	;shift register right by count in regn - signed
	glo	regn		;the + and - are nameless temporary symbols
	plo	memaddr		;save the shift count
	lbz	+
-	shL4	reg
	dec	memaddr		;decrement the shift count
	glo	memaddr		;get it
	lbnz	-
+
	endm
shL4I:	macro	reg,n	;shift long register left by constant
	rept	n
		shL4	reg
	endm	;ends the rept
	endm	;ends the macro

shLC4:	macro	reg	;shift long register left once continuing previous shift
	glo	reg	;start with low byte of second register
	shlc		;shift left once continuing carry
	plo	reg	; save it
	ghi	reg	;high byte of second reg
	shlc		;shift one bit carrying
	phi	reg	;save it
	glo	reg-1	;now the bottom byte of top reg
	shlc
	plo	reg-1
	ghi	reg-1	;finally the top byte of the high order reg
	shlc		;gets the last shift
	phi	reg-1	;and we're done
	endm
negI4:	macro	tgt,src			;four byte negation from src to tgt
	alu4I	tgt,src,0xffffffff,xri,xri 	;flip all the bits
	alu4i	tgt,tgt,1,adi,adci		;and add 1
	endm
jcF4:	macro	reg1,reg2,brop,label		;jump if float reg 1< float reg 2
	ghi	reg1-1		;see if first arg is -v
	shl
	chbr	lbnf,CFcmp		;if at least 1 reg positive, just compare
	ghi	reg2-1		;check 2nd reg
	shl
	chbr	lbdf,CFrcmp
CFcmp:				;straight compare
	jcI4	reg1,reg2,brop,label	;as long as one register is +v
	chbr	lbr,CFdone
CFrcmp: 			;reverse compare
	jcI4	reg2,reg1,brop,label	;reverse the order of the operands
CFdone:
	endm
;macros inserted between assembler passes for pixie accommodation.
;they get substituted for long branch instructions so as not to upset 1861 video timing
;a companion code module pxbrcode.inc is required
xbr:	macro	target
	sep	R7
	dw	target
	endm
xbz:	macro	target
	pagefit 6
	bnz	mynz
	sep	R7
	dw	target
mynz:
	endm
xbnz:	macro	target
	pagefit 6
	bz	myz
	sep	R7
	dw	target
myz:
	endm
xbdf:	macro	target
	pagefit 6
	bnf	mynf
	sep	R7
	dw	target
mynf:
	endm
xbnf:	macro	target
	pagefit 6
	bdf	mydf
	sep	R7
	dw	target
mydf:
	endm
	
	listing	on
	lbr	lcc1802init	;21-02-04 init code moved to beginning of code to eliminae long branch
