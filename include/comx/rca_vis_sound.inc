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
_generatetone:			; generate 'tone' in R12, with 'range' in R13 and 'volume' on R2
  IFDEF  PECOM 
	sex R3
	out 1
	db  2
	sex R2
  ENDIF
	ld2 R15,'O',sp,(7)
	ani 0xf				
	lbnz $$toneon		; if volume = 0 turn off tone
	ldi 0
	phi R15
	ldi 0x80
	plo R15				; R15 = 0x0080 = turn off tone
	lbr  $$tone	
$$toneon:
	str R2				; Stack = volume value
	glo R13		
	ani 0x7				; R13 lowest 3 bits is range
	shl
	shl
	shl
	shl					; shift range 4 bits to the left
	or					; D = range<<4 | volume
	plo R15				; R15.0 = range<<4 | volume
	glo R12
	ani 0x7f	
	phi R15				; R15 = tone		
$$tone:
  IFDEF  TMC600 
    sex R3
    out 7
    db 0x40				; select VIS register 4
	sex R15				; tone generation value
    out 5				
  ELSE	
	sex R15				; tone generation value
	out 4
  ENDIF
  IFDEF CIDELSA 
    sex R2
  ENDIF
	cretn

_generatenoise			; generate noise with 'range' in R12 and 'volume' on R13
  IFDEF  PECOM 
	sex R3
	out 1
	db  2
	sex R2
	ldireg R8,0x7CC3	; load current OUT 5 (low byte) video setting from 0x7cc3
  ENDIF  
  IFDEF  COMX 
	ldireg R8,0x41C3	; load current OUT 5 (low byte) video setting from 0x41C3
  ENDIF  
  IFDEF  TMC600 
	ldireg R8,0x600C	; load current OUT 5 (low byte) video setting from 0x600C
  ENDIF  
    glo R13 
	ani 0xf
	lbnz $$noiseon		; if volume = 0 turn off noise
	ldi 0x80
	phi R15
  IFDEF  CIDELSA 
	ldi 0x88
  ELSE
	ldn R8				; get low byte video value	
  ENDIF  
	plo R15				; R15 = 0x80xx = turn off tone
	lbr  $$noise	
$$noiseon:
	str R2				; Stack = volume value
	glo R12
	ani 0x7
	shl
	shl
	shl
	shl					; shift range 4 bits to the left
	or					; D = range<<4 | volume
	phi R15				; R15.1 = range<<4 | volume
  IFDEF  CIDELSA 
	ldi 0x88
  ELSE
	ldn R8				; get low byte video value	
  ENDIF	
	plo R15				; store in R15.0
$$noise:					  
  IFDEF  TMC600 
    sex R3
    out 7
    db 0x50				; select VIS register 5
  ENDIF	
	sex R15				; Noise  generation value
	out 5
  IFDEF CIDELSA 
    sex R2
  ENDIF
	cretn

