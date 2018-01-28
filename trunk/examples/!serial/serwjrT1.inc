;bit-bang Serial routines adapted from Josh Bensadon's VELFbios-v3.1.asm
;https://groups.yahoo.com/neo/groups/cosmacelf/files/Member-JoshBensadon/
;Transmit Byte via Q connected to RS232 driver
;call via sep, returns via sep R3
;Byte to send in D
;Destroys r14
;17-09-02 this version times out on the first call only - approx 1.5 sec at 4MHz
;----------------------------------------------------------------------
 	IFNDEF 	LCC1802CPUSPEED
LCC1802CPUSPEED EQU 4000000
 	ENDIF
 	MACEXP ON
;
;bitdelay: MACRO baudrate,cpuspeed,baseline,xreg
;	rept ((cpuspeed/(baudrate*8)-baseline))/3
;	NOP
;	endm
;	rept (((cpuspeed/(baudrate*8)-baseline)#3))>=1
;	sex xreg
;	endm
;	ENDM

__BAUDRATE EQU 	9600
	align 64
serout:			;entry from assembly with char in D
	phi R14		;save char in R14.1
	ldi 9		;9 bits to transmit (1 start + 8 data)
	plo r14
	ghi R14
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
	bitdelay __BAUDRATE,LCC1802CPUSPEED,20,2,{EXPAND}
	br .txcloop	;9.5    loop back to send next bit
.txcret: ghi r14		;7
	bitdelay __BAUDRATE,LCC1802CPUSPEED,16,2
	seq		;11.5 stop bit
	bitdelay __BAUDRATE,LCC1802CPUSPEED,4,2
	sep R3		;return 
	br serout	;reset for next time
;**********************************************************************
;serinT
;Receive Byte via EF3 connected to RS232 receiver
;Receives 8 bits
;call via sep
;Returns with Byte received in D, DF is set if the start bit is never seen
;Destroys r14
;----------------------------------------------------------------------
	align 64
serinT:			;serial input with timeout of 65535*6*16 machine cycles - approx 1.5 sec at 4MHZ
	ldaD R14,0x4000	;R14 is timeout loop counter
.rxcw:	b3 .okgo	;check for start bit after each instruction
	dec 14
	b3 .okgo	;check for start bit after each instruction
	ghi 14
	b3 .okgo	;check for start bit after each instruction
	bnz .rxcw
;here we've had a timeout - set DF and return
	ldi 1
	shr
	sep R3		;return
	br  serinT	;for next time
.serinN:	bn3 .serinN	;serial input without timeout	
.okgo:			;here we know the start bit is present
 	ldi 8		;start bit +7 bits from loop, last bit on returning
	plo r14
	ldi 0
	NOP		;delay to center samples

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
	ori 80h		; for a 1 bit
	adi 0		;clear the DF flag because no timeout
	sep R3		;return
	br  .serinN	;for next time - only timeout on first call
	