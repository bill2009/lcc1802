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
  IFDEF  COMX 
	
	align	16
_cgetc:				; wait for key press
$$loop:
	b3 $$keypressed	; Check if a key is pressed for the 'first time'
	bn2 $$loop		;  Loop if no key was pressed previously
$$keypressed:
	inp 3
	plo R15
	ldi 0
	phi R15
	cretn

	align	16
_kbhit:				; return 1 if a key is pressed, 0 if not
    b3 $$keypressed	; Check if a key is pressed for the 'first time'
    bn2 $$nokey		; Branch to nokey if no key was pressed previously
$$keypressed:
	ldi 1
	lskp
$$nokey:
	ldi 0
	plo R15
	ldi 0
	phi R15
	cretn

  ENDIF
  
  IFDEF  PECOM 
  
	align	32	
_cgetc:				; wait for key press
	sex R3
	out 1
	db  0
	ldireg R8, 0x7CCA
	sex R8			; Set stack to R8 which counts up from 0x7CCA to 0x7CE3 to check all keys
$$checknext:
	inp 3			; bit 0 and 1 indicate key press
	ani 3
	bnz $$keypressed; bit 0 or 1 are 1 so a key is pressed
	glo R8
	inc R8
	xri 0xe3
	bnz $$checknext	; R8++ if not 0xE3 check next key
	br  _cgetc		; all keys checked - check again
$$keypressed:
	shr	
	shr				; move bit 1 into DF
	glo R8			; get value 'keyset'
	shlc			; shift left and move DF to b0 so key value is unique
	adi 0x98		; convert to ASCII
	plo R15
	ldi 0
	phi R15
	cretn

	align	32
_kbhit:				; return 1 if a key is pressed, 0 if not
	sex R3
	out 1
	db  0
	ldireg R8, 0x7CCA
	sex R8			; Set stack to R8 which counts up from 0x7CCA to 0x7CE3 to check all keys
$$checknext:
	inp 3			; bit 0 and 1 indicate key press
	ani 3
	bnz $$keypressed; bit 0 or 1 are 1 so a key is pressed
	glo R8
	inc R8
	xri 0xe3
	bnz $$checknext ; R8++ if not 0xE3 check next key
	ldi 0			; all keys checked - return 0
	lskp
$$keypressed:
	ldi 1			; key pressed - return 1
	plo R15
	ldi 0
	phi R15
	cretn
	
  ENDIF

  IFDEF  TMC600 
	
	align	32
_cgetc:				; wait for key press 
	ldi 0x40		; keyboard check start value we should check on bit 0 to 5 (i.e 0x3f down to 0)
$$checknext:
	smi 1	
	str R2			; store keyboard latch on stack
	out 3			; test for key press
	dec r2
	b3  $$keypressed
	bnz $$checknext ; check next key
	br _cgetc		; no key pressed, check again
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
	add
	plo R8
	ldn R8
	plo R15
	ldi 0
	phi R15
	cretn

	align 32
_kbhit:				; return 1 if a key is pressed, 0 if not
	ldi 0x40		; keyboard check start value we should check on bit 0 to 5 (i.e 0x3f down to 0)
$$checknext:
	smi 1	
	str R2			; store keyboard latch on stack
	out 3			; test for key press
	dec r2
	b3  $$keypressed
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
		
	align	64
keytable:
	db 0x30
	db 0x31
	db 0x32
	db 0x33
	db 0x34
	db 0x35
	db 0x36
	db 0x37
	db 0x38
	db 0x39
	db 0x3a
	db 0x3b
	db 0x3c
	db 0x3d
	db 0x3e
	db 0x3f
	db 0x40
	db 0x41
	db 0x42
	db 0x43
	db 0x44
	db 0x45
	db 0x46
	db 0x47
	db 0x48
	db 0x49
	db 0x4a
	db 0x4b
	db 0x4c
	db 0x4d
	db 0x4e
	db 0x4f
	db 0x50
	db 0x51
	db 0x52
	db 0x53
	db 0x54
	db 0x55
	db 0x56
	db 0x57
	db 0x58
	db 0x59
	db 0x5a
	db 0x5b
	db 0x5c
	db 0x5d
	db 0x5e
	db 0x20
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
  