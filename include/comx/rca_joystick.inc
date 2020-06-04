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
_get_stick:			; get joystick values, 1 = pressed
					; bit	7		6		5		4		3		2		1		0
					;		Left 1	Right 1	Down 1	Up 1	Right 2	Left 2	Up 2	Down 2	
	ldi 0
	phi R15
  IFDEF ALTAIR
	inp 1
	xri 0xff
	ani 0x18
	shl
	shl
	shl
	plo R8
	inp 4
	xri 0xff
	ani 0x03
	shl
	shl
	shl
	shl
	str R2
	glo R8
	or
  ENDIF
  IFDEF DESTROYER
	inp 1
	xri 0xff
	ani 0x18
	shl
	shl
	shl
  ENDIF
  IFDEF DRACO
	inp 4
	xri 0xff
  ENDIF
	plo R15
	cretn

_get_trigger:		; get trigger/fire values, 1 = pressed
					; bit	7		6		5		4		3		2		1		0
					;						Fire					Tilt	Button2 Button1	
	ldi 0
	phi R15
	inp 1
	xri 0xff
	ani 0x20
	plo R15
	cretn
	
_kbhit:				; return 1 if a game key is pressed, 0 if not
	ldi 0
	phi R15
	inp 1
	xri 0xff
  IFDEF ALTAIR
	ani 0x38
  ENDIF
  IFDEF DESTROYER
	ani 0x38
  ENDIF
  IFDEF DRACO
	ani 0x20
  ENDIF
	bnz $$pressed
  IFNDEF DESTROYER
	inp 4	
	xri 0xff
  IFDEF ALTAIR
	ani 0x03
  ENDIF
	ani 0
	bnz $$pressed
  ENDIF
	ldi 0
	lskp
$$pressed
	ldi 1
	plo R15
	cretn

  ENDIF
  
  IFDEF  PECOM 	
	align	32
_getkey:			; get game key (space, up, down, left, right, A, Z, DEL) value if pressed, otherwise return 0
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

	align	32	
_cgetc:				; wait for game key (space, up, down, left, right, A, Z, DEL) press
	sex R3
	out 1
	db  0
	ldi 0
	phi R15
	sex R8			; Set stack to R8 which counts up from 0x7CCA to 0x7CE3 to check all keys
$$cgetc:
	ldi 0x7C
	phi R8
	ldireg r14,keytable
$$checknext:
	lda r14
	bz $$cgetc		; all keys checked - check again
	plo r8
	inp 3			; bit 0 and 1 indicate key press
	ani 3
	bz $$checknext	; check next key
	shr	
	shr				; move bit 1 into DF
	glo R8			; get value 'keyset'
	shlc			; shift left and move DF to b0 so key value is unique
	adi 0x98		; convert to ASCII
	plo R15
	cretn

	align	32
_kbhit:				; return 1 if a game key (space, up, down, left, right, A, Z, DEL) is pressed, 0 if not
	sex R3
	out 1
	db  0
	ldi 0x7C
	phi R8
	ldi 0
	phi R15
	ldireg r14,keytable
	sex R8			; Set stack to R8 which counts up from 0x7CCA to 0x7CE3 to check all keys
$$checknext:
	lda r14
	bz $$exit		; all keys checked - return 0
	plo r8
	inp 3			; bit 0 and 1 indicate key press
	ani 3
	bz $$checknext	; check next key
	ldi 1			; key pressed - return 1
$$exit
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
_getkey:			; get game key value if pressed, otherwise return 0
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
	
	align	64
_cgetc:				; wait for game key press 
	ldi 0x40		; keyboard check start value we should check on bit 0 to 5 (i.e 0x3f down to 0)
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
	br _cgetc 		; no key pressed, check again
$$keypressed:
	ldi 4
	phi R8
$$keydelay:
	dec R8
	ghi R8
	bnz $$keydelay	; delay loop before we check if the key was really pressed
	bn3 _cgetc		; if key not pressed continue checking
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

	align 64
_kbhit:				; return 1 if a game key is pressed, 0 if not
	ldi 0x40		; keyboard check start value we should check on bit 0 to 5 (i.e 0x3f down to 0)
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
	br  $$exit		; all keys checked - return 0
$$keypressed:
	ldi 4
	phi R8
$$keydelay:
	dec R8
	ghi R8
	bnz $$keydelay	; delay loop before we check if the key was really pressed
	bn3 _kbhit		; if key not pressed continue checking
	ldi 1			; key pressed - return 1
$$exit:
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