;
; *******************************************************************
; *** This software is copyright 2020 by Marcel van Tongeren      ***
; *** with support from Bill Rowe and Fabrizio Caruso.			  ***
; ***                                                             ***
; *** You have permission to use, modify, copy, and distribute    ***
; *** this software so long as this copyright notice is retained. ***
; *** This software may not be used in commercial applications    ***
; *** without express written permission from the author.         ***
; *******************************************************************
;  
  IFDEF CIDELSA
_get_stick:			; get joystick values, specified bit = 1: pressed
					; bit	7		6		5		4		3		2		1		0
					;						fire	Left 	Right 	Down 	Up 		
					;
					; Note: Destroyer doesn't have up/down keys.
	ldi 0
	phi R15
  IFDEF ALTAIR
	inp 1
	xri 0xff
	ani 0x38
	plo R8
	inp 4
	xri 0xff
	ani 0x03
	shl
	str R2
	glo R8
	or
  ENDIF
  IFDEF DESTROYER
	inp 1
	xri 0xff
	ani 0x38
  ENDIF
  IFDEF DRACO
	inp 4
	xri 0xff
	shr
	shr
	shr
	plo R8
	inp 1
	xri 0xff
	ani 0x20
	str R2
	glo R8
	or
  ENDIF
	plo R15
	cretn

  ENDIF
  
  IFDEF  PECOM 	
	align	32
_get_stick:			; get joystick (space, up, down, left, right, A, Z, DEL) value if pressed, otherwise return 0
	sex R3
	out 1
	db  0
	ldi 0
	phi R15
	ldireg r14,keytable
	ldi 0x7C
	phi R8
	sex R8			; Set stack to R8 which counts up from 0x7CD4 to 0x7CE3 to check some keys
$$checknext:
	lda r14
	bz $$exit		; all keys checked - return 0
	plo r8
	inp 3			; bit 0 and 1 indicate key press
	ani 3
	bz $$checknext	; check next key
	shr	
	shr				; move bit 1 into DF
	glo R8			; get value 'keyset'
	shlc			; shift left and move DF to b0 so key value is unique
	adi 0x98		; convert to ASCII
$$exit:
	plo R15
	cretn

keytable:
	db 0xd4
	db 0xe1
	db 0xe2
	db 0xe3
	db 0
	
  ENDIF

  IFDEF  TMC600 
	align	64
_get_stick:			; get joystick value if pressed, otherwise return 0
	ldi 0x3f		; keyboard check start value we should check on bit 0 to 5 (i.e 0x3f down to 0)
	plo R15
$$checknext:
	glo R15	
	str R2			; store keyboard latch on stack
	out 3			; test for key press
	dec r2
	b3  $$keypressed
	dec R15
	glo R15
	smi 0x3a
	bnz $$arrowkeys
	ldi 0x30
	plo R15
	br $$checknext ; check next key
$$arrowkeys
	glo R15
	smi 0x2f
	bnz $$checknext ; check next key
	ldi 0
	plo R15
	phi R15
	cretn			; all keys checked - return 0
$$keypressed:
	ldi 4
	phi R8
$$keydelay:
	dec R8
	ghi R8
	bnz $$keydelay	; delay loop before we check if the key was really pressed
	bn3 $$checknext	; if key not pressed continue checking
	ldireg R8,keytable	
	glo R8
	smi 0x30
	add
	plo R8
	ldn R8
	plo R15
	ldi 0
	phi R15
	cretn
	
	align	16
keytable:
	db 0x20
	db 0x08
	db 0x1b
	db 0x20
	db 0x20
	db 0x20
	db 0x20
	db 0x20
	db 0x20
	db 0x20
	db 0x18
	db 0xb
	db 0x9
	db 0xd
	db 0xa
	db 0x8
	
  ENDIF
  
  IFDEF  COMX 
	align	16
_get_stick:			; get key value if pressed, otherwise return 0
	b3 $$keypressed	; Check if a key is pressed for the 'first time'
	bn2 $$nokey		; Branch to nokey if no key was pressed previously
$$keypressed:
	inp 3
	lskp
$$nokey:
	ldi 0
	plo R15
	ldi 0
	phi R15
	cretn
	
  ENDIF
	
  IFDEF  MICRO 
	align	16
_get_stick:			; get key value if pressed, otherwise return 0
	bn3 $$nokey;	; Branch to nokey if no key is pressed 
$$keypressed:
	inp 3
	lskp
$$nokey:
	ldi 0
	plo R15
	ldi 0
	phi R15
	cretn
	
  ENDIF
