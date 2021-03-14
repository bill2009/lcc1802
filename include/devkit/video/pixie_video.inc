;
; *******************************************************************
; *** This software is copyright 2021 by Marcel van Tongeren      ***
; *** with support from Bill Rowe and Fabrizio Caruso.            ***
; ***                                                             ***
; *** All pixie / 1861 routines are based on routines provided    ***
; *** by Richard Dienstknecht                                     ***
; ***                                                             ***
; *** You have permission to use, modify, copy, and distribute    ***
; *** this software so long as this copyright notice is retained. ***
; *** This software may not be used in commercial applications    ***
; *** without express written permission from the author.         ***
; *******************************************************************
; 

; =========================================================================================
; Interrupt and DMA service routine for the CDP1861 to display an effective resolution
; of 64 x 32 pixels, using a display buffer of 256 bytes.
; =========================================================================================

  IFDEF RES32

  pagefit    30

INT_Exit:			LDXA
					RET
DisplayInt:			DEC  R2
					SAV
					DEC  R2
					STR  R2
					NOP
					NOP
					NOP
					LDI  VIDMEM/256
					PHI  R0
					LDI  0
					PLO  R0
INT_Loop:			GLO  R0
					SEX  R2
					SEX  R2
					DEC  R0
					PLO  R0
					SEX  R2
					DEC  R0
					PLO  R0
					SEX  R2
					DEC  R0
					PLO  R0
					BN1  INT_Loop
					BR   INT_Exit

  IFDEF PIXIE_SPRITE_MOVE										
  pagefit    20
_videobuf:		db 20 dup 
  ENDIF
  IFDEF PIXIE_PATTERN										
  pagefit    8
_addition_table:	db 1
					db HEIGHT*2+1
					db HEIGHT*4+1 
					db HEIGHT*6+1 
					db HEIGHT*8+1 
					db HEIGHT*10+1 
					db HEIGHT*12+1 
					db HEIGHT*14+1					
  ENDIF
  ENDIF
				
;------------------------------------------------------------------------------------------


; =========================================================================================
; Interrupt and DMA service routine for the CDP1861 to display an effective resolution
; of 64 x 64 pixels, using a display buffer of 512 bytes.
; =========================================================================================

  IFDEF RES64

    align    32
  
INT_Exit:			LDXA
					RET
DisplayInt:			NOP
					DEC  R2
					SAV
					DEC  R2
					STR  R2
					LDI  VIDMEM/256
					PHI  R0
					LDI  0
					PLO  R0
					NOP
					NOP
					SEX  R2
INT_Loop:			GLO  R0
					SEX  R2
					DEC  R0
					PLO  R0
					SEX  R2
					BN1  INT_Loop
INT_Rest:			GLO  R0
					SEX  R2
					DEC  R0
					PLO  R0
					B1   INT_Rest
					BR   INT_Exit
					
  IFDEF PIXIE_SPRITE_MOVE										
  pagefit    40
_videobuf:		db 40 dup 
  ENDIF
  IFDEF PIXIE_PATTERN										
  pagefit    8
_addition_table:	db 1
					db HEIGHT*2+1
					db HEIGHT*4+1 
					db HEIGHT*6+1 
					db HEIGHT*8+1 
					db HEIGHT*10+1 
					db HEIGHT*12+1 
					db HEIGHT*14+1					
  ENDIF
  ENDIF

;------------------------------------------------------------------------------------------


; =========================================================================================
; Interrupt and DMA service routine for the CDP1861 to display an effective resolution
; of 64 x 128 pixels, using a display buffer of 1024 bytes.
; =========================================================================================

  IFDEF RES128

    align    16
    
INT_Exit:			LDXA
					RET
DisplayInt:			NOP
					DEC  R2
					SAV
					DEC  R2
					STR  R2
					SEX  R2					
					SEX  R2
					LDI  VIDMEM/256
					PHI  R0
					LDI  00H
					PLO  R0
					BR   INT_Exit

  IFDEF PIXIE_SPRITE_MOVE										
  pagefit    80    
_videobuf:		db 80 dup 
  ENDIF
  IFDEF PIXIE_PATTERN										
  pagefit    8
_addition_table:	db 1
					db HEIGHT*2+1
					db HEIGHT*4+1 
					db HEIGHT*6+1 
					db HEIGHT*8+1 
					db HEIGHT*10+1 
					db HEIGHT*12+1 
					db HEIGHT*14+1					
  ENDIF
  ENDIF

;------------------------------------------------------------------------------------------

; =========================================================================================
; Initialize 1861 video output
; =========================================================================================

  pagefit    28
					
_initvideo:   
    ldireg R1, DisplayInt
    inp 1

;------------------------------------------------------------------------------------------

; =========================================================================================
; Clear 1861 video output
; =========================================================================================

_vidclr:                ; clear screen by writing 0's to video memory
	ldi  VIDMEM/256
	phi r14
	ldi 0
	plo r14
	plo r15
  IFDEF RES32
    ldi 1
    phi r15
  ENDIF
  IFDEF RES64										
    ldi 2
    phi r15
  ENDIF
  IFDEF RES128										
    ldi 4
    phi r15
  ENDIF
$$loop:
	ldi 0
	str R14
	inc R14
	dec R15
	glo R15
	bnz $$loop
	ghi R15
	bnz $$loop
    cretn

;------------------------------------------------------------------------------------------

; =========================================================================================
; Parameters:
; R12 		X coordinate of the sprite
; R13		Y coordinate of the sprite
; R15		Pointer to sprite
;
; Internal:
; R11		number of/remaining lines
; R14		Pointer to video memory
; =========================================================================================
  IFDEF PIXIE_TILE										

    align    128

_drawtile:			
					ld2  R15,'O',sp,(7)
  IFDEF RES32										
					ldi  4
  ENDIF
  IFDEF RES64										
					ldi  8
  ENDIF
  IFDEF RES128										
					ldi  16
  ENDIF
					plo  R11
					LDI  VIDMEM/256			; prepare the pointer to the video buffer					
					PHI  R14						; VIDMEM + Y * 8 + X / 8
					GLO  R13						; result goes to R14
					ANI  0x7						; between 0 - 7
  IFDEF RES32										
					SHL  
					SHL  
					SHL  
					SHL  
					SHL  
					PLO  R14
  ENDIF
  IFDEF RES64
					SHL
					SHL  
					SHL  
					SHL  
					SHL  
					SHL  
					PLO  R14
					BNF  $$skipIncrement2
					GHI  R14
					ADI  0x01
					PHI  R14
  ENDIF
  IFDEF RES128
					SHL
					SHL
					SHL  
					SHL  
					SHL  
					SHL  
					PLO  R14
					BNF  $$skipIncrement
					GHI  R14
					ADI  0x02
					PHI  R14
$$skipIncrement:	GLO  R14
					SHL  
					PLO  R14
					BNF  $$skipIncrement2
					GHI  R14
					ADI  0x01
					PHI  R14
  ENDIF
$$skipIncrement2:	GLO  R14
					STR  R2
					GLO  R12
					ANI  0xF
					SHR
					ADD
					PLO  R14

$$byteLoop:			GLO  R11						; exit if all bytes of the sprite have been drawn
					BZ   $$exit
					
  IFDEF RES32										; or if we are about to draw outside the video buffer
					LDI  VIDMEM/256			
  ENDIF
  IFDEF RES64										
					LDI  VIDMEM/256+1			
  ENDIF
  IFDEF RES128										
					LDI  VIDMEM/256+3	
  ENDIF
					STR  R2
					GHI  R14
					SD
					BNF  $$exit
					GLO  R12
					ANI  0x1
					BZ   $$high_nibble				; check if position is on low or high nibble
					LDA  R15						; load the next byte of the sprite
					ANI  0xF
					STR  R2
					LDN  R14
					ANI  0xF0
					OR
					STR  R14						; OR in thr low nibble of the sprite
					BR   $$next_line
$$high_nibble:      LDA  R15						; load the next byte of the sprite
					ANI  0xF0
					STR  R2
					LDN  R14
					ANI  0xF
					OR
					STR  R14						; OR in thr high nibble of the sprite
$$next_line:		GLO  R14						; advance the video buffer pointer to the next line
					ADI  0x08
					PLO  R14
					GHI  R14
					ADCI 0
					PHI  R14
					DEC  R11						; decrement the sprite's byte counter
					BR   $$byteLoop
$$exit			    cretn

  ENDIF

;------------------------------------------------------------------------------------------


; =========================================================================================
; Parameters:
; R12 		X coordinate of the sprite
; R13		Y coordinate of the sprite
; R15		Pointer to sprite - number of lines, 8 bits per line
;
; Internal:
; R11		Size of the sprite in bytes, first byte of pointer to sprite
; R14		Pointer to video memory
; =========================================================================================

  IFDEF PIXIE_SPRITE										

  IFDEF RES32
  pagefit 0x54
  ENDIF
  IFDEF RES64
  pagefit 0x5e
  ENDIF
  IFDEF RES128
  pagefit 0x66
  ENDIF

_drawsprite:			
					ld2  R15,'O',sp,(7)		
					lda  R15
					plo  R11
					LDI  VIDMEM/256					; prepare the pointer to the video buffer					
					PHI  R14						; VIDMEM + Y * 8 + X / 8
					GLO  R13						; result goes to R14
  IFDEF RES32
					ANI  0x1F						; between 0 - 31
  ENDIF
  IFDEF RES64
					ANI  0x3F						; between 0 - 63
  ENDIF
  IFDEF RES128
					ANI  0x7F						; between 0 - 127
  ENDIF
  IFDEF RES32
					SHL
					SHL
					SHL
					PLO  R14
  ENDIF
  IFDEF RES64
					SHL
					SHL
					SHL
					PLO  R14
					BNF  $$SkipIncrement2
					GHI  R14
					ADI  0x01
					PHI  R14
  ENDIF
  IFDEF RES128
					SHL
					SHL
					PLO  R14
					BNF  $$skipIncrement
					GHI  R14
					ADI  0x02
					PHI  R14
$$skipIncrement:	GLO  R14
					SHL  
					PLO  R14
					BNF  $$skipIncrement2
					GHI  R14
					ADI  0x01
					PHI  R14
  ENDIF
					
$$SkipIncrement2:	GLO  R14
					STR  R2
					GLO  R12
					ANI  0x3F
					SHR
					SHR
					SHR
					ADD
					PLO  R14
					GLO  R12						; calculate the number of required shifts 
					ANI  0x07						; result to R12.1, replacing the X coordinate
					PHI  R12						; R12.0 will be used later to count the shifts

$$ByteLoop:			GLO  R11						; exit if all bytes of the sprite have been drawn
					BZ   $$Exit
					
  IFDEF RES32										; or if we are about to draw outside the video buffer
					LDI  VIDMEM/256			
  ENDIF
  IFDEF RES64										
					LDI  VIDMEM/256+1			
  ENDIF
  IFDEF RES128										
					LDI  VIDMEM/256+3	
  ENDIF
					STR  R2
					GHI  R14
					SD
					BNF  $$Exit
					LDN	 R15						; load the next byte of the sprite into R10.0
					PLO  R10
					LDI  0							; set R10.1 to OOH
					PHI  R10
					DEC  R11						; decrement the sprite's byte counter
					INC  R15						; increment the pointer to the sprite's bytes
					GHI  R12						; prepare the shift counter
					PLO  R12
$$ShiftLoop:		GLO  R12						; exit the loop if all shifts have been performed
					BZ   $$ShiftExit
					DEC  R12						; decrement the shift counter
					GLO  R10						; shift the values in R10
					SHR
					PLO  R10
					GHI  R10
					RSHR
					PHI  R10
					BR   $$ShiftLoop
$$ShiftExit:		SEX  R14						; store the shifted bytes in the video buffer
					GLO  R10
					XOR
					STR  R14
					INC  R14
					GHI  R10
					XOR
					STR  R14
					SEX  R2
					GLO  R14						; advance the video buffer pointer to the next line
					ADI  0x07
					PLO  R14
  IFDEF RES64										
					GHI  R14
					ADCI 0
					PHI  R14
  ENDIF
  IFDEF RES128										
					GHI  R14
					ADCI 0
					PHI  R14
  ENDIF
					BR   $$ByteLoop
$$Exit			

  ENDIF

;------------------------------------------------------------------------------------------

; =========================================================================================
; Parameters:
; R12       sprite data pointer
; R13		Pointer to sprite - number of lines, 8 bits per line
; R8 		X coordinate of the sprite
; R15		Y coordinate of the sprite
;
; Internal:
; R9		buffer
; R10		16 bit temp pattern
; R11		Size of the sprite in bytes, first byte of pointer to sprite
; R14		Pointer to video memory
; =========================================================================================

  IFDEF PIXIE_SPRITE_MOVE										

  pagefit    0x2e

_showsprite:		glo  R2
					adi  8
					plo  R10
					ghi  R2
					adci 0
					phi  R10
					lda  R10
					plo  R8
					inc  R10
					ldn  R10
					plo  R15
					ldi  _videobuf/256
					phi  R9
					ldi  _videobuf&0xff
					plo  R9	
					ghi  R13
					str  R12
					inc  R12
					glo  R13
					str  R12
					inc  R12
					glo  R8
					str  R12
					inc  R12
					glo  R15
					str  R12
					br   _fill_draw_buffer
	
_removesprite:		lda  R12
					phi  R13
					lda  R12
					plo  R13
					lda  R12
					plo  R8		
					ldn  R12
					plo  R15
					ldi  _videobuf/256
					phi  R9
					ldi  _videobuf&0xff
					plo  R9	
_fill_draw_buffer:	sep  R4
					dw	 _fill_buffer
					ldi  _videobuf&0xff				
					plo  R9	
  IFDEF PIXIE_VSYNC										
					IDL
  ENDIF
					sep  R4
					dw	 _draw_buffer
					Cretn

  pagefit    0x59
				
_movesprite:		glo  R13
					phi  R8
					lda  R12
					phi  R13
					lda  R12
					plo  R13
					lda  R12
					plo  R8		
					ldn  R12
					plo  R15
					dec  R12
					dec  R12
					dec  R12
					ldi  _videobuf/256
					phi  R9
					ldi  _videobuf&0xff
					plo  R9	
					sep  R4
					dw	 _fill_buffer
					ghi  R8
					smi  3
					bz   $$up
					smi  2
					bz   $$left
					smi  2
					bz   $$right
					smi  2
					bz   $$down
					br   $$exit
$$left:				glo  R8
					bz   $$exit
					dec  R8
					br   $$nextsprite
$$right:			glo  R8
					xri  64-WIDTH
					bz   $$exit
					inc  R8
					br   $$nextsprite
$$up:				glo  R15
					bz   $$exit
		    		dec  R15
					br   $$nextsprite
$$down:				glo  R15
					xri  RES-HEIGHT
					bz   $$exit
		    		inc  R15
$$nextsprite:		lda  R12
					phi  R13
					lda  R12
					plo  R13
					glo  R8
					str  R12
					inc  R12		
					glo  R15
					str  R12
					sep  R4
					dw	 _fill_buffer
					ldi  _videobuf&0xff				
					plo  R9	
  IFDEF PIXIE_VSYNC										
					IDL
  ENDIF
					sep  R4
					dw	 _draw_buffer
					sep  R4
					dw	 _draw_buffer
$$exit:				Cretn

  IFDEF RES32
  pagefit 0x3f
  ENDIF
  IFDEF RES64
  pagefit 0x45
  ENDIF
  IFDEF RES128
  pagefit 0x4d
  ENDIF
 
_fill_buffer:		lda  R13
					plo  R11
					phi  R11
					LDI  VIDMEM/256					; prepare the pointer to the video buffer					
					PHI  R14						; VIDMEM + Y * 8 + X / 8
					GLO  R15						; result goes to R14
  IFDEF RES32
					ANI  0x1F						; between 0 - 31
  ENDIF
  IFDEF RES64
					ANI  0x3F						; between 0 - 63
  ENDIF
  IFDEF RES128
					ANI  0x7F						; between 0 - 127
  ENDIF
  IFDEF RES32
					SHL
					SHL
					SHL
					PLO  R14
  ENDIF
  IFDEF RES64
					SHL
					SHL
					SHL
					PLO  R14
					BNF  $$SkipIncrement2
					GHI  R14
					ADI  0x01
					PHI  R14
  ENDIF
  IFDEF RES128
					SHL
					SHL
					PLO  R14
					BNF  $$skipIncrement
					GHI  R14
					ADI  0x02
					PHI  R14
$$skipIncrement:	GLO  R14
					SHL  
					PLO  R14
					BNF  $$skipIncrement2
					GHI  R14
					ADI  0x01
					PHI  R14
  ENDIF
					
$$SkipIncrement2:	GHI  R14
					STR  R9
					INC  R9
					GLO  R14
					STR  R2
					GLO  R8
					ANI  0x3F
					SHR
					SHR
					SHR
					ADD
					STR  R9
					INC  R9
					GLO  R8							; calculate the number of required shifts 
					ANI  0x07						; result to R15.1, replacing the Y coordinate
					PHI  R15						; R8.0 will be used later to count the shifts

$$ByteLoop:			GLO  R11						; exit if all bytes of the sprite have been calculated
					BZ   $$Exit
					
					LDN	 R13						; load the next byte of the sprite into R10.0
					PLO  R10
					LDI  0							; set R10.1 to OOH
					PHI  R10
					DEC  R11						; decrement the sprite's byte counter
					INC  R13						; increment the pointer to the sprite's bytes
					GHI  R15						; prepare the shift counter
					PLO  R14
$$ShiftLoop:		GLO  R14						; exit the loop if all shifts have been performed
					BZ   $$ShiftExit
					DEC  R14						; decrement the shift counter
					GLO  R10						; shift the values in R10
					SHR
					PLO  R10
					GHI  R10
					RSHR
					PHI  R10
					BR   $$ShiftLoop
$$ShiftExit:		glo  R10
					str  R9
					inc  R9
					ghi  R10
					str  R9
					inc  R9
					BR   $$ByteLoop
$$Exit:				Cretn

  IFDEF RES32
  pagefit 0x34
  ENDIF
  IFDEF RES64
  pagefit 0x38
  ENDIF
  IFDEF RES128
  pagefit 0x38
  ENDIF
								
_draw_buffer:		LDI  0
					PHI  R15
					PLO  R15
					LDA  R9
					PHI  R14
					LDA  R9
					PLO  R14
					ghi  R11						; draw the shifted bytes in the video buffer
					plo  R11
$$DrawLoop: 
  IFDEF RES32										; exit if we are about to draw outside the video buffer
					LDI  VIDMEM/256			
  ENDIF
  IFDEF RES64										
					LDI  VIDMEM/256+1			
  ENDIF
  IFDEF RES128										
					LDI  VIDMEM/256+3	
  ENDIF
					STR  R2
					GHI  R14
					SD
					BNF  $$Exit
					SEX  R14
					ldn  R9
					and
					bz   $$no_collision1
					ldi  1
					plo  R15
$$no_collision1:	lda  R9
					XOR
					STR  R14
					INC  R14
					ldn  R9
					and
					bz   $$no_collision2
					ldi  1
					plo  R15
$$no_collision2:	lda  R9
					XOR
					STR  R14
					GLO  R14						; advance the video buffer pointer to the next line
					ADI  0x07
					PLO  R14
  IFDEF RES64										
					GHI  R14
					ADCI 0
					PHI  R14
  ENDIF
  IFDEF RES128										
					GHI  R14
					ADCI 0
					PHI  R14
  ENDIF
					dec  R11
					glo  R11
					SEX  R2
					Bnz  $$DrawLoop
					Cretn
$$Exit:				ldi  0
					phi  R15

  ENDIF

;------------------------------------------------------------------------------------------

; =========================================================================================
; Parameters:
; R12       sprite data pointer
; R13		Pointer to sprite - number of lines, 8 bits per line
; R8 		X coordinate of the sprite
; R9		Y coordinate of the sprite
;
; Internal:
; R10		16 bit temp pattern
; R11		Size of the sprite in bytes, first byte of pointer to sprite
; R14		Pointer to video memory
; =========================================================================================

  IFDEF PIXIE_PATTERN										
	
_showsprite:		glo  R2
					adi  8
					plo  R10
					ghi  R2
					adci 0
					phi  R10
					lda  R10
					plo  R8
					inc  R10
					ldn  R10
					plo  R9
					ghi  R13
					str  R12
					inc  R12
					glo  R13
					str  R12
					inc  R12
					glo  R8
					str  R12
					inc  R12
					glo  R9
					str  R12
  IFDEF PIXIE_VSYNC										
					IDL
  ENDIF
					sep  R4
					dw	 _draw_buffer
					Cretn
	
_removesprite:		lda  R12
					phi  R13
					lda  R12
					plo  R13
					lda  R12
					plo  R8		
					ldn  R12
					plo  R9
  IFDEF PIXIE_VSYNC										
					IDL
  ENDIF
					sep  R4
					dw	 _draw_buffer
					Cretn

  pagefit    0x5c
		
_movesprite:		glo  R13
					phi  R8
					lda  R12
					phi  R13
					lda  R12
					plo  R13
					lda  R12
					plo  R8		
					ldn  R12
					plo  R9
  IFDEF PIXIE_CHECK_BORDER										
					ghi  R8
					smi  3
					bz   $$up1
					smi  2
					bz   $$left1
					smi  2
					bz   $$right1
					smi  2
					bz   $$down1
					br   $$exit
$$left1:			glo  R8
					bnz  $$removesprite
					br   $$exit
$$right1:			glo  R8
					xri  64-WIDTH
					bnz  $$removesprite
					br   $$exit
$$up1:				glo  R9
					bnz  $$removesprite
					br   $$exit
$$down1:			glo  R9
					xri  RES-HEIGHT
					bz   $$exit
$$removesprite:
  ENDIF
  IFDEF PIXIE_VSYNC										
					IDL
  ENDIF
					sep  R4
					dw	 _draw_buffer
					ghi  R8
					smi  3
					bz   $$up
					smi  2
					bz   $$left
					smi  2
					bz   $$right
					smi  2
					bz   $$down
					br   $$exit
$$left:				dec  R8
					br   $$nextsprite
$$right:			inc  R8
					br   $$nextsprite
$$up:	    		dec  R9
					br   $$nextsprite
$$down:	    		inc  R9
$$nextsprite:		glo  R9
					str  R12
					dec  R12		
					glo  R8
					str  R12
					sep  R4
					dw	 _draw_buffer
$$exit:				Cretn

_movexysprite:		glo  R13
					phi  R8
					lda  R12
					phi  R13
					lda  R12
					plo  R13
					lda  R12
					plo  R8		
					ldn  R12
					plo  R9
  IFDEF PIXIE_VSYNC										
					IDL
  ENDIF
					sep  R4
					dw	 _draw_buffer
					ghi  R8
					plo  R8
					glo  R2
					adi  8
					plo  R10
					ghi  R2
					adci 0
					phi  R10
					ldn  R10
					plo  R9
					glo  R9
					str  R12
					dec  R12		
					glo  R8
					str  R12
					sep  R4
					dw	 _draw_buffer
					Cretn

  IFDEF RES32
  pagefit 0x58
  ENDIF
  IFDEF RES64
  pagefit 0x62
  ENDIF
  IFDEF RES128
  pagefit 0x6A
  ENDIF
 
_draw_buffer:		ldn  R13
					plo  R11
					phi  R11
					LDI  VIDMEM/256					; prepare the pointer to the video buffer					
					PHI  R14						; VIDMEM + Y * 8 + X / 8
					GLO  R9						; result goes to R14
  IFDEF RES32
					ANI  0x1F						; between 0 - 31
  ENDIF
  IFDEF RES64
					ANI  0x3F						; between 0 - 63
  ENDIF
  IFDEF RES128
					ANI  0x7F						; between 0 - 127
  ENDIF
  IFDEF RES32
					SHL
					SHL
					SHL
					PLO  R14
  ENDIF
  IFDEF RES64
					SHL
					SHL
					SHL
					PLO  R14
					BNF  $$SkipIncrement2
					GHI  R14
					ADI  0x01
					PHI  R14
  ENDIF
  IFDEF RES128
					SHL
					SHL
					PLO  R14
					BNF  $$skipIncrement
					GHI  R14
					ADI  0x02
					PHI  R14
$$skipIncrement:	GLO  R14
					SHL  
					PLO  R14
					BNF  $$skipIncrement2
					GHI  R14
					ADI  0x01
					PHI  R14
  ENDIF
					
$$SkipIncrement2:	GLO  R14
					STR  R2
					GLO  R8
					ANI  0x3F
					SHR
					SHR
					SHR
					ADD
					PLO  R14
					ldi  _addition_table&0xff
					str  R2
					GLO  R8							; calculate the number of required shifts 
					ANI  0x07						
					add
					plo  R10
					ldi  _addition_table/256
					phi  R10
					ldn  R10
					str  R2							
					GLO  R13
					ADD
					PLO  R10
					ghi  R13
					ADCI 0
					PHI  R10
					LDI  0
					PHI  R15
					PLO  R15
					ghi  R11						; draw the bytes in the video buffer
					plo  R11
$$DrawLoop: 
  IFDEF RES32										; exit if we are about to draw outside the video buffer
					LDI  VIDMEM/256			
  ENDIF
  IFDEF RES64										
					LDI  VIDMEM/256+1			
  ENDIF
  IFDEF RES128										
					LDI  VIDMEM/256+3	
  ENDIF
					STR  R2
					GHI  R14
					SD
					BNF  $$Exit
					SEX  R14
					ldn  R10
					and
					bz   $$no_collision1
					ldi  1
					plo  R15
$$no_collision1:	lda  R10
					XOR
					STR  R14
					INC  R14
  IFDEF PIXEL
					inc  R10
  ELSE
					ldn  R10
					and
					bz   $$no_collision2
					ldi  1
					plo  R15
$$no_collision2:	lda  R10
					XOR
					STR  R14
  ENDIF
					GLO  R14						; advance the video buffer pointer to the next line
					ADI  0x07
					PLO  R14
  IFDEF RES64										
					GHI  R14
					ADCI 0
					PHI  R14
  ENDIF
  IFDEF RES128										
					GHI  R14
					ADCI 0
					PHI  R14
  ENDIF
					dec  R11
					glo  R11
					SEX  R2
					Bnz  $$DrawLoop
					Cretn
$$Exit:				ldi  0
					phi  R15

  ENDIF

;------------------------------------------------------------------------------------------
