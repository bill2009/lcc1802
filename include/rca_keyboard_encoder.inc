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
_getkey:			; get key value if pressed, otherwise return 0
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
_getkey:			; get key value if pressed, otherwise return 0
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
	ldi 0
	br  $$exit		; all keys checked - return 0
$$keypressed:
	shr	
	shr				; move bit 1 into DF
	glo R8			; get value 'keyset'
	shlc			; shift left and move DF to b0 so key value is unique
	adi 0x98		; convert to ASCII
$$exit:
	plo R15
	ldi 0
	phi R15
    sex R2
	cretn

	align	32	
_cgetc:				; wait for key press
$$cgetc:
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
	br  $$cgetc		; all keys checked - check again
$$keypressed:
	shr	
	shr				; move bit 1 into DF
	glo R8			; get value 'keyset'
	shlc			; shift left and move DF to b0 so key value is unique
	adi 0x98		; convert to ASCII
	plo R15
	ldi 0
	phi R15
    sex R2
	cretn

	align	32
_kbhit:				; return 1 if a key is pressed, 0 if not
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
    sex R2
	cretn
	
	align	64
_getkey_game:		; get game key (space, up, down, left, right, A, Z, DEL) value if pressed, otherwise return 0
	ldireg R8, 0x7CD4
	sex R8			; Set stack to R8 which counts up from 0x7CCA to 0x7CE3 to check all keys
$$checknext:
	inp 3			; bit 0 and 1 indicate key press
	ani 3
	bnz $$keypressed; bit 0 or 1 are 1 so a key is pressed
	glo R8
	inc R8
	xri 0xd4
	bnz $$arrowkeys
	ldi 0xe1
	plo R8
$$arrowkeys:
	glo R8
	xri 0xe4
	bnz $$checknext	; R8++ if not 0xE4 check next key
	ldi 0
	br  $$exit		; all keys checked - return 0
$$keypressed:
	shr	
	shr				; move bit 1 into DF
	glo R8			; get value 'keyset'
	shlc			; shift left and move DF to b0 so key value is unique
	adi 0x98		; convert to ASCII
$$exit:
	plo R15
	ldi 0
	phi R15
    sex R2
	cretn

	align	32	
_cgetc_game:		; wait for game key (space, up, down, left, right, A, Z, DEL) press
$$cgetc:
	ldireg R8, 0x7CD4
	sex R8			; Set stack to R8 which counts up from 0x7CCA to 0x7CE3 to check all keys
$$checknext:
	inp 3			; bit 0 and 1 indicate key press
	ani 3
	bnz $$keypressed; bit 0 or 1 are 1 so a key is pressed
	glo R8
	inc R8
	xri 0xd4
	bnz $$arrowkeys
	ldi 0xe1
	plo R8
$$arrowkeys:
	glo R8
	xri 0xe4
	bnz $$checknext	; R8++ if not 0xE4 check next key
	br  $$cgetc		; all keys checked - check again
$$keypressed:
	shr	
	shr				; move bit 1 into DF
	glo R8			; get value 'keyset'
	shlc			; shift left and move DF to b0 so key value is unique
	adi 0x98		; convert to ASCII
	plo R15
	ldi 0
	phi R15
    sex R2
	cretn

	align	32
_kbhit_game:		; return 1 if a game key (space, up, down, left, right, A, Z, DEL) is pressed, 0 if not
	ldireg R8, 0x7CD4
	sex R8			; Set stack to R8 which counts up from 0x7CCA to 0x7CE3 to check all keys
$$checknext:
	inp 3			; bit 0 and 1 indicate key press
	ani 3
	bnz $$keypressed; bit 0 or 1 are 1 so a key is pressed
	glo R8
	inc R8
	xri 0xd4
	bnz $$arrowkeys
	ldi 0xe1
	plo R8
$$arrowkeys:
	glo R8
	xri 0xe4
	bnz $$checknext ; R8++ if not 0xE4 check next key
	ldi 0			; all keys checked - return 0
	lskp
$$keypressed:
	ldi 1			; key pressed - return 1
	plo R15
	ldi 0
	phi R15
    sex R2
	cretn

  ENDIF
