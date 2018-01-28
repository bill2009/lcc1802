;**********************************************************************
;bit-bang Serial routines adapted from Josh Bensadon's VELFbios-v3.1.asm
;Transmit Byte via Q connected to RS232 driver
;call via SCRT
;Byte to send in r12.0
;Destroys r14
;----------------------------------------------------------------------
 	IFNDEF 	LCC1802CPUSPEED
LCC1802CPUSPEED EQU 4000000
 	ENDIF
bitdelay: MACRO baudrate,cpuspeed,baseline,xreg
	rept ((cpuspeed/(baudrate*8)-baseline))/3
	NOP
	endm
	rept (((cpuspeed/(baudrate*8)-baseline)#3))>=1
	sex xreg
	endm
	ENDM
__BAUDRATE EQU 	9600
	align 32
_putcser:		;entry from C program with character in reg 12
	ldi 9		;9 bits to transmit (1 start + 8 data)
	plo r14
	glo r12
	shl		;set start bit
	rshr		;DF=0

.txcloop:
	bdf $+5		;10.5   jump to seq to send a 1 bit
	req		;11.5   send a 0 bit
	br $+5		;1      jump +5 to next shift
	seq		;11.5   send a 1 bit
	br $+2		;1      jump +2 to next shift (NOP for timing)
	rshr		;2      shift next bit to DF flag
	phi r14		;3      save D in r14.1
	DEC r14		;4      dec bit count
	glo r14		;5      get bit count
	bz .txcret	;6      if 0 then all 9 bits (start and data) sent
	ghi r14		;7      restore D
	bitdelay __BAUDRATE,LCC1802CPUSPEED,20,2
	br .txcloop	;9.5    loop back to send next bit
.txcret: ghi r14		;7
	bitdelay __BAUDRATE,LCC1802CPUSPEED,16,2
	seq		;11.5 stop bit
	cretn		;return and re-call will cover stop bit time
;**********************************************************************
;rx_char
;Receive Byte via EF2 connected to RS232 receiver
;Receives 8 bits
;call via SCRT
;Returns with Byte received in D and rf.0
;Destroys r14.0
;----------------------------------------------------------------------
	align 64
_getcser:
 	ldi 8		;start bit +7 bits from loop, last bit on returning
	plo r14
	ldi 0
.rxcw:			;wait for start bit
	;bn4 $+4		;loop while high
	;br  RomLoad
	bn3 .rxcw	;each instr takes 9us, we need 104us = 11.5
			;delay 1/2 bit time to center samples
	NOP		;     Don't test for correct start bit
	NOP		;     it will work. if there's too much
	NOP		;     noise on the line, shorten the cable!
.rxcloop:
	bitdelay __BAUDRATE,LCC1802CPUSPEED,20,2
	b3 $+6		;11.5 sample rx input bit
	ori 80h		;1
	br $+4		;2
	phi r14		;1
	phi r14		;2
	shr		;3
	phi r14		;4
	DEC r14		;5
	glo r14		;6
	bz .rxcret	;7
	ghi r14		;8
	br  .rxcloop	;9
.rxcret: ghi r14	;8
	ghi r14		;9
	bitdelay __BAUDRATE,LCC1802CPUSPEED,20,2
	b3 $+4		;11.5 sample last rx input bit
	ori 80h		;1
	plo r15
	cretn
