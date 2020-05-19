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
	align	32
_rand:
  IFNDEF LCCCX
	br $$rand0
$$seed:
	db 0xac				; Initial eed value for none LCCCX machines
	db 0xe1
$$rand0:
	ldireg R8, $$seed	; Pointer to Seed value for none LCCCX machines
  ENDIF
  IFDEF  PECOM 
	ldireg R8, 0x2be	; Pointer to COMX PECOM RND seed value
  ELSE
	ldireg R8, 0x42be	; Pointer to COMX BASIC RND seed value
  ENDIF
	lda R8	
	bnz $$rand1			; Do rand if high byte of seed value != 0
  IFDEF LCCCX
$$zerolow:
	glo R0					
	bz $$zerolow		; Wait until R0.0 != 0
	phi R15				; Use R0.0 as low byte of seed value
$$zerohigh:
	ghi R0
	bnz $$rand2			; If R0.1 != 0 use as high byte of seed value
	br $$zerohigh		; Wait until R0.1 != 0
  ELSE
	ldi 0xAC			
	phi R15
	ldi 0xE1			; fixed seed value
	lskp
  ENDIF
$$rand1:
	phi R15				; R15.1 = high byte of seed value
	ldn 8	
$$rand2:
	plo R15				; R15.0 = low byte of seed value
	ani 1
	str R2				; Stack = bit 0 of seed value, rest is 0
	glo R15				; D = low byte of seed value
	shr	    
	shr		
	xor					; D bit 0 = bit 0 XOR bit 2
	plo R9
	ani 1
	str R2				; Stack = bit 0 XOR bit 2
	glo R9
	shr		
	xor					; D bit 0 = bit 0/2 XOR bit 3
	plo R9
	ani 1
	str R2				; Stack = bit 0/2 XOR bit 3
	glo R9
	shr		
	shr		
	xor					; D bit 0 = bit 0/2/3  XOR bit 5
	shr					; DF = new input bit
	ghi R15
	shrc
	dec R8
	str R8				; Store high byte new seed value, 
						; R15.1 >> 1, MSB = DF, new input bit, DF = LSB
	ani 0x7f
	phi R15				; R15.1 = seed value & 0x7f, allowing only postivie numbers
	glo R15
	shrc
	plo R15				; R15.0 >> 1, MSB = DF, DF = LSB
	inc R8
	str R8				; Store low byte new seed value
	cretn
