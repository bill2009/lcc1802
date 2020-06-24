;
; *******************************************************************
; *** This software is copyright 2020 by Marcel van Tongeren      ***
; *** with support from Bill Rowe and Fabrizio Caruso.            ***
; ***                                                             ***
; *** You have permission to use, modify, copy, and distribute    ***
; *** this software so long as this copyright notice is retained. ***
; *** This software may not be used in commercial applications    ***
; *** without express written permission from the author.         ***
; *******************************************************************
; 

_generatetone:			; generate 'tone' in R12, with 'range' in R13 and 'volume' on R2
    select_vis_sex2
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
    out_vis R15, 4
	c_return

_generatenoise			; generate noise with 'range' in R12 and 'volume' on R13
    select_vis_sex2  
	ldireg R8,_vis_out5	; load current OUT 5 (low byte) video setting from 0x41C3/0x7cc3/0x600C/vis_out5
    glo R13 
	ani 0xf
	lbnz $$noiseon		; if volume = 0 turn off noise
	ldi 0x80
	phi R15
	ldn R8				; get low byte video value	
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
	ldn R8				; get low byte video value	
	plo R15				; store in R15.0
$$noise:					  
    out_vis R15, 5
	c_return

