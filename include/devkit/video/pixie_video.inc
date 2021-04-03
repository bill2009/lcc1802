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

  IFDEF PIXIE_MOVE										
  pagefit    20
_videobuf:		db 20 dup 
  ENDIF
  IFDEF PIXIE_PATTERN										
  pagefit    8
_addition_table:	db 1
					db SPRITE_HEIGHT*2+1
					db SPRITE_HEIGHT*4+1 
					db SPRITE_HEIGHT*6+1 
					db SPRITE_HEIGHT*8+1 
					db SPRITE_HEIGHT*10+1 
					db SPRITE_HEIGHT*12+1 
					db SPRITE_HEIGHT*14+1					
  ENDIF
  ENDIF
				
;------------------------------------------------------------------------------------------


; =========================================================================================
; Interrupt and DMA service routine for the CDP1861 to display an effective resolution
; of 64 x 64 pixels, using a display buffer of 512 bytes.
; =========================================================================================

  IFDEF RES64

    pagefit    0x1e

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
					
  IFDEF PIXIE_MOVE										
  pagefit    40
_videobuf:		db 40 dup 
  ENDIF
  IFDEF PIXIE_PATTERN										
  pagefit    8
_addition_table:	db 1
					db SPRITE_HEIGHT*2+1
					db SPRITE_HEIGHT*4+1 
					db SPRITE_HEIGHT*6+1 
					db SPRITE_HEIGHT*8+1 
					db SPRITE_HEIGHT*10+1 
					db SPRITE_HEIGHT*12+1 
					db SPRITE_HEIGHT*14+1					
  ENDIF
  ENDIF

;------------------------------------------------------------------------------------------


; =========================================================================================
; Interrupt and DMA service routine for the CDP1861 to display an effective resolution
; of 64 x 128 pixels, using a display buffer of 1024 bytes.
; =========================================================================================

  IFDEF RES128

  pagefit    10
    
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
					LDI  0
					PLO  R0
					BR   INT_Exit

  IFDEF PIXIE_MOVE										
  pagefit    80    
_videobuf:		db 80 dup 
  ENDIF
  IFDEF PIXIE_PATTERN										
  pagefit    8
_addition_table:	db 1
					db SPRITE_HEIGHT*2+1
					db SPRITE_HEIGHT*4+1 
					db SPRITE_HEIGHT*6+1 
					db SPRITE_HEIGHT*8+1 
					db SPRITE_HEIGHT*10+1 
					db SPRITE_HEIGHT*12+1 
					db SPRITE_HEIGHT*14+1					
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
    Cretn

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

  IFDEF RES32
  pagefit 0x52
  ENDIF
  IFDEF RES64
  pagefit 0x59
  ENDIF
  IFDEF RES128
  pagefit 0x62
  ENDIF

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
  IFDEF PIXIE_VSYNC										
					IDL
  ENDIF
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
$$exit:			    Cretn

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
  pagefit 0x55
  ENDIF
  IFDEF RES64
  pagefit 0x5f
  ENDIF
  IFDEF RES128
  pagefit 0x67
  ENDIF

_drawsprite:		ld2  R15,'O',sp,(7)		
					lda  R15
					plo  R11
_drawsprite2:		LDI  VIDMEM/256					; prepare the pointer to the video buffer					
					PHI  R14						; VIDMEM + Y * 8 + X / 8
					GLO  R13						; result goes to R14
  IFDEF RES32
					ANI  0x1F						; between 0 - 31
					SHL
					SHL
					SHL
					PLO  R14
  ENDIF
  IFDEF RES64
					ANI  0x3F						; between 0 - 63
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
					ANI  0x7F						; between 0 - 127
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
  IFDEF PIXIE_VSYNC										
					IDL
  ENDIF
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
$$exit:			    Cretn

  ENDIF

;------------------------------------------------------------------------------------------

; =========================================================================================
; Parameters:
; R12       sprite data pointer
; R13		Pointer to sprite - number of lines, 8 bits per line
; R8.0 		X coordinate of the sprite
; R9.0		Y coordinate of the sprite
; R15		return value, 1 = collision
;
; Internal:
; R8.1
; R9.1		high byte of temp 16 bit pattern
; R10		buffer
; R11		Size of the sprite in bytes, first byte of pointer to sprite
; R14		Pointer to video memory
; =========================================================================================

  IFDEF PIXIE_MOVE										

  pagefit    0x2e

_showsprite:		glo  R2
					adi  8
					plo  R14
					ghi  R2
					adci 0
					phi  R14
					lda  R14
					plo  R8
					inc  R14
					ldn  R14
					plo  R9
					ldi  _videobuf/256
					phi  R10
					ldi  _videobuf&0xff
					plo  R10	
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
					br   _fill_draw_buffer
	
_removesprite:		lda  R12
					phi  R13
					lda  R12
					plo  R13
					lda  R12
					plo  R8		
					ldn  R12
					plo  R9
					ldi  _videobuf/256
					phi  R10
					ldi  _videobuf&0xff
					plo  R10	
_fill_draw_buffer:	sep  R4
					dw	 _fill_buffer
					ldi  _videobuf&0xff				
					plo  R10	
  IFDEF PIXIE_VSYNC										
					IDL
  ENDIF
					sep  R4
					dw	 _draw_buffer
					Cretn

  pagefit    0x72

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
					xri  64-SPRITE_WIDTH
					bnz  $$removesprite
					br   $$exit
$$up1:				glo  R9
					bnz  $$removesprite
					br   $$exit
$$down1:			glo  R9
					xri  RES-SPRITE_HEIGHT
					bz   $$exit
$$removesprite:
  ENDIF
					dec  R12
					dec  R12
					dec  R12
					ldi  _videobuf/256
					phi  R10
					ldi  _videobuf&0xff
					plo  R10	
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
$$left:				dec  R8
					br   $$nextsprite
$$right:			inc  R8
					br   $$nextsprite
$$up:		   		dec  R9
					br   $$nextsprite
$$down:		  		inc  R9
$$nextsprite:		lda  R12
					phi  R13
					lda  R12
					plo  R13
					glo  R8
					str  R12
					inc  R12		
					glo  R9
					str  R12
					sep  R4
					dw	 _fill_buffer
					ldi  _videobuf&0xff				
					plo  R10	
  IFDEF PIXIE_VSYNC										
					IDL
  ENDIF
					sep  R4
					dw	 _draw_buffer
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
					dec  R12
					dec  R12
					dec  R12
					ldi  _videobuf/256
					phi  R10
					ldi  _videobuf&0xff
					plo  R10	
					sep  R4
					dw	 _fill_buffer
					lda  R12
					phi  R13
					lda  R12
					plo  R13
					ghi  R8
					plo  R8
					str  R12
					inc  R12		
					glo  R2
					adi  8
					plo  R14
					ghi  R2
					adci 0
					phi  R14
					ldn  R14
					plo  R9
					str  R12
					sep  R4
					dw	 _fill_buffer
					ldi  _videobuf&0xff
					plo  R10	
  IFDEF PIXIE_VSYNC										
					IDL
  ENDIF
					sep  R4
					dw	 _draw_buffer
					sep  R4
					dw	 _draw_buffer
					Cretn

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
_fill_buffer2:		phi  R11
					LDI  VIDMEM/256					; prepare the pointer to the video buffer					
					PHI  R14						; VIDMEM + Y * 8 + X / 8
					GLO  R9							; result goes to R14
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
					STR  R10
					INC  R10
					GLO  R14
					STR  R2
					GLO  R8
					ANI  0x3F
					SHR
					SHR
					SHR
					ADD
					STR  R10
					INC  R10
					GLO  R8							; calculate the number of required shifts 
					ANI  0x07						; result to R14.1, replacing the Y coordinate
					PHI  R14						; R14.0 will be used later to count the shifts

$$ByteLoop:			GLO  R11						; exit if all bytes of the sprite have been calculated
					BZ   $$Exit
					
					LDN	 R13						; load the next byte of the sprite into M(R2)
					STR  R2
					LDI  0							; set R9.1 to OOH
					PHI  R9
					DEC  R11						; decrement the sprite's byte counter
					INC  R13						; increment the pointer to the sprite's bytes
					GHI  R14						; prepare the shift counter
					PLO  R14
$$ShiftLoop:		GLO  R14						; exit the loop if all shifts have been performed
					BZ   $$ShiftExit
					DEC  R14						; decrement the shift counter
					LDN  R2							; shift the values on R2
					SHR
					STR  R2
					GHI  R9
					RSHR
					PHI  R9
					BR   $$ShiftLoop
$$ShiftExit:		ldn  R2
					str  R10
					inc  R10
					ghi  R9
					str  R10
					inc  R10
					BR   $$ByteLoop
$$Exit:				Cretn

  IFDEF RES32
  pagefit 0x28
  ENDIF
  IFDEF RES64
  pagefit 0x34
  ENDIF
  IFDEF RES128
  pagefit 0x34
  ENDIF
							
_draw_buffer:		LDI  0
					PLO  R15
					PHI  R15
					LDA  R10
					PHI  R14
					LDA  R10
					PLO  R14
					ghi  R11						; draw the shifted bytes in the video buffer
					plo  R11
$$DrawLoop: 
  IFDEF RES64										; exit if we are about to draw outside the video buffer
					LDI  VIDMEM/256+1			
					STR  R2
					GHI  R14
					SD
					BNF  $$Exit
  ENDIF
  IFDEF RES128										
					LDI  VIDMEM/256+3	
					STR  R2
					GHI  R14
					SD
					BNF  $$Exit
  ENDIF
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
					ldn  R10
					and
					bz   $$no_collision2
					ldi  1
					plo  R15
$$no_collision2:	lda  R10
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
$$Exit:				Cretn

  IFDEF PIXIE_TEXT
_drawsprite2:		glo  R12
					plo  R8
					glo  R13
					plo  R9
					glo  R15
					plo  R13
					ghi  R15
					phi  R13
					ldi  _videobuf/256
					phi  R10
					ldi  _videobuf&0xff
					plo  R10
					glo  R11	
					sep  R4
					dw	 _fill_buffer2
					ldi  _videobuf&0xff
					plo  R10	
  IFDEF PIXIE_VSYNC										
					IDL
  ENDIF
					sep  R4
					dw	 _draw_buffer
					Cretn

  ENDIF ; PIXIE_TEXT

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
					xri  64-SPRITE_WIDTH
					bnz  $$removesprite
					br   $$exit
$$up1:				glo  R9
					bnz  $$removesprite
					br   $$exit
$$down1:			glo  R9
					xri  RES-SPRITE_HEIGHT
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
					glo  R2
					adi  8
					plo  R10
					ghi  R2
					adci 0
					phi  R10
					ldn  R10
					plo  R9
					str  R12
					dec  R12		
					ghi  R8
					plo  R8
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
					Cretn
  ENDIF

;------------------------------------------------------------------------------------------

  IFDEF PIXIE_TEXT

; =========================================================================================
; Draws a zero terminated string at specified screen coordinates. 
;
; Parameters:
; R8		Pointer to the string
; R12.0		X coordinate
; R13.0		Y coordinate
; 
; Internal:
; R9.0		character
; =========================================================================================

  pagefit 0x2a
  
_vidstrcpyxy:	    ld2  R8,'O',sp,(7)
$$drawloop:			LDA  R8							; get character, exit if 0
					PLO  R9
					BZ   $$Exit
					
					GLO  R8							; push R8 onto the stack
					STXD
					GHI  R8
					STXD
					
					GLO  R12						; push R12/R13 onto the stack
					STXD
					GLO  R13
					STXD
					
					SEP  R4							; draw the character
					dw _drawchar2
					
					IRX								; restore R12/R13
					LDXA
					PLO  R13
					
					GLO  R15						; advance the x coordinate by the
					ADD								; width of the character + 1
					ADI  0x1
					PLO  R12
					
					IRX								; restore R8
					LDXA
					PHI  R8
					LDX
					PLO  R8
					BR   $$drawloop					; continue with the next character

$$Exit:				Cretn

;------------------------------------------------------------------------------------------


; =========================================================================================
; Draws a character at specified screen coordinates 
;
; Parameters:
; R12		X coordinate of the character
; R13		Y coordinate of the character
; R9.0		ASCII code of the character (20 - 5F)
; 
; Internal:
; R15		Pointer to the unpacked character's pattern
; R11
; R10		Pointer to the font
; R9.1		Temporary values
; =========================================================================================

  pagefit 0x72

_vidcharxy:			glo  R2
					adi  8
					plo  R10
					ghi  R2
					adci 0
					phi  R10
					lda  R10
					plo  R9
_drawchar2:			LDI  CharacterPattern&0xff		; R15 points to the buffer for the character pattern
					PLO  R15
					LDI  CharacterPattern/256
					PHI  R15
					
					LDI  Font&0xff					; R10 points to the font
					PLO  R10
					LDI  Font/256
					PHI  R10
					
					GLO  R9							; calculate the offset in the font
					SMI  0x20						; (( character code - 20) / 2) * 6
					ANI  0xFE
					PHI  R9
					SHL
					STXD
					IRX					
					GHI  R9
					ADD
					STXD
					IRX
					BNF DC_SkipHighByte
					GHI  R10
					ADI	 0x1
					PHI  R10
					
DC_SkipHighByte:	GLO  R10							; add to the address in R10
					ADD	
					PLO  R10
					BNF  DC_SkipHighByte2
					GHI  R10
					ADI  0x1
					PHI  R10

DC_SkipHighByte2:	LDN  R10							; get the width of the first character pattern
					SHR
					SHR
					SHR
					SHR
					STXD
					IRX

					GLO  R9							; do we need the first or the second pattern?
					ANI  0x1
					PHI  R9
					BNZ  DC_PrepareSecond
					
					LDX								; prepare the mask					
					PLO  R11	
					LDI  0
					PHI  R11
DC_MaskLoop:		GHI  R11
					SHR
					ORI  0x80
					PHI  R11
					DEC  R11
					GLO  R11
					BNZ  DC_MaskLoop

					LDX
					STXD							; keep the width of the first pattern on the stack
					BR   DC_CopyPattern

DC_PrepareSecond:	LDX								; use the width of the first pattern for shifting
					PHI  R11

					LDN  R10						; keep the width of the second character pattern on the stack
					ANI  0x7
					STXD

DC_CopyPattern:		INC  R10
					LDI  0x5						; prepare a loop over the five bytes of the pattern
					PLO  R9
					
DC_ByteLoop:		LDN  R10						; get a byte from the font
					STXD
					IRX
					INC  R10
					
					GHI  R9
					BNZ  DC_ByteShift

					GHI  R11						; mask out the first pattern
					AND
					STXD
					IRX
					BR   DC_ByteWrite

DC_ByteShift:		GHI  R11						; shift the second pattern
					PLO  R11
DC_ShiftLoop:		LDX
					SHL
					STXD
					IRX
					DEC  R11
					GLO  R11
					BNZ  DC_ShiftLoop

DC_ByteWrite:		LDX								; write the byte
					STR  R15
					INC  R15

					DEC  R9							; continue until all bytes of the pattern are done
					GLO  R9
					BNZ  DC_ByteLoop

					LDI  CharacterPattern&0xff		; restore R15 to the beginning of the pattern
					PLO  R15
					LDI  0x5						; set the length of the pattern
					PLO  R11
					SEP  R4							; call sprite routine to draw  
					dw   _drawsprite2

					IRX								; clean up and exit
					LDX
					PLO  R15
					SEP  R5

;------------------------------------------------------------------------------------------


; =========================================================================================
; Buffer for unpacked characters
; =========================================================================================

  pagefit 5
CharacterPattern:	db 5 dup

;------------------------------------------------------------------------------------------


; =========================================================================================
; Font definition
;
; The font has 64 or 96 printable characters, selected by setting PIXIE_TEXT96 or not setting
; it. The font requires 192 bytes memory if 64 characters are used. Full 96 characters
; require 96 additional bytes, a total of 288 bytes.
;
; The characters are encoded in standard ASCII, beginning with 20H and anding at 5FH for
; 64 characters or 7FH for 96 characters. Each character has a height of 5 pixels and a
; variable width of 1 - 5 pixels. 
;
; In order to save memory, the patterns of two characters have been combined into one. Without
; this, the complete 96 character font would have required 480 bytes. The 'left' character
; stands for the lower even ASCII code, the 'right' one for the following
; uneven ASCII code. The DrawCharacter subroutine will either mask the left character or
; shift the right one over to the position of the left one when a character is drawn.
;
; Each line defines two characters of variable width, together no more than 8 pixels. The
; first byte contains the width of each character. The upper four bits hold the width of
; the left character in the pattern, the lower four bits hold the width of the right character.
;
; The following five bytes contain the bit patterns of the characters. Beginning at the left
; (most significant) bit, the pattern of the left character (up to its specified width) is 
; immediately followed by the bits of the right character. Any remaining bits to the right 
; (if both characters together are less than 8 pixels wide) must be set to 0. 
; =========================================================================================

Font:				db 0x11, 0x40, 0x40, 0x40, 0x00, 0x40	; space and !
					db 0x35, 0xAA, 0xBF, 0x0A, 0x1F, 0x0A	; " and #
					db 0x33, 0x74, 0xC4, 0x48, 0x70, 0xD4	; $ and %
					db 0x41, 0x48, 0xA8, 0x40, 0xA0, 0xD0	; & and '
					db 0x22, 0x60, 0x90, 0x90, 0x90, 0x60	; ( and )
					db 0x33, 0x00, 0xA8, 0x5C, 0xA8, 0x00	; * and +
					db 0x22, 0x00, 0x00, 0x30, 0x40, 0x80	; , and -
					db 0x13, 0x10, 0x10, 0x20, 0x40, 0xC0	; . and /
					db 0x33, 0x48, 0xB8, 0xA8, 0xA8, 0x5C	; 0 and 1
					db 0x33, 0xD8, 0x24, 0x48, 0x84, 0xF8	; 2 and 3
					db 0x33, 0x3C, 0xB0, 0xF8, 0x24, 0x38	; 4 and 5
					db 0x33, 0x5C, 0x84, 0xC4, 0xA8, 0x48	; 6 and 7		
					db 0x33, 0x48, 0xB4, 0x4C, 0xA4, 0x48	; 8 and 9
					db 0x12, 0x00, 0x20, 0x80, 0x20, 0xC0	; : and ;		
					db 0x32, 0x20, 0x58, 0x80, 0x58, 0x20	; < and =
					db 0x33, 0x88, 0x54, 0x24, 0x48, 0x88	; > and ?
					db 0x33, 0x48, 0xF4, 0x9C, 0xB4, 0x54	; @ and A
					db 0x33, 0xCC, 0xB0, 0xD0, 0xB0, 0xCC	; B and C
					db 0x33, 0xDC, 0xB0, 0xB8, 0xB0, 0xDC	; D and E
					db 0x33, 0xEC, 0x90, 0xD0, 0x94, 0x8C	; F and G
					db 0x33, 0xBC, 0xA8, 0xE8, 0xA8, 0xBC	; H and I
					db 0x33, 0x34, 0x34, 0x38, 0xB4, 0x54	; J and K
					db 0x35, 0x91, 0x9B, 0x95, 0x91, 0xF1	; L and M
					db 0x43, 0x9E, 0xDA, 0xBA, 0x9A, 0x9E	; N and O
					db 0x34, 0xDE, 0xB2, 0xD2, 0x96, 0x9E	; P and Q
					db 0x33, 0xCC, 0xB0, 0xC8, 0xA4, 0xB8	; R and S
					db 0x33, 0xF4, 0x54, 0x54, 0x54, 0x5C	; T and U
					db 0x35, 0xB1, 0xB1, 0xB1, 0xB5, 0x4A	; V and W
					db 0x33, 0xB4, 0xB4, 0x48, 0xA8, 0xA8	; X and Y
					db 0x32, 0xF8, 0x30, 0x50, 0x90, 0xF8	; Z and [
					db 0x32, 0x98, 0x88, 0x48, 0x28, 0x38	; \ and ]
					db 0x33, 0x40, 0xA0, 0x00, 0x00, 0x1C	; ^ and _

  IFDEF PIXIE_TEXT96

					db 0x23, 0x40, 0x98, 0x28, 0x28, 0x18	; ' and a
					db 0x33, 0x80, 0xCC, 0xB0, 0xB0, 0xCC	; b and c
					db 0x33, 0x20, 0x6C, 0xB4, 0xB8, 0x6C	; d and e
					db 0x23, 0x58, 0xA8, 0xF0, 0x88, 0xB0	; f and g
					db 0x31, 0x90, 0x80, 0xD0, 0xB0, 0xB0	; h and i
					db 0x23, 0x60, 0x28, 0x70, 0x68, 0xA8	; j and k
					db 0x25, 0x80, 0x94, 0xAA, 0xAA, 0x6A	; l and m
					db 0x33, 0x00, 0xC8, 0xB4, 0xB4, 0xA8	; n and o
					db 0x33, 0x00, 0xCC, 0xB4, 0xCC, 0x84	; p and q
					db 0x23, 0x00, 0x58, 0xB0, 0x88, 0xB0	; r and s
					db 0x23, 0x80, 0xE8, 0xA8, 0xA8, 0x58	; t and u
					db 0x35, 0x00, 0xB1, 0xB5, 0xB5, 0x4A	; v and w
					db 0x33, 0x00, 0xB4, 0x4C, 0x44, 0xA8	; x and y
					db 0x33, 0x0C, 0xE8, 0x30, 0x48, 0xEC	; z and {
					db 0x13, 0xE0, 0xA0, 0x90, 0xA0, 0xE0	; | and }
					db 0x44, 0x05, 0x5A, 0xA5, 0x0A, 0x05	; ~ and DEL

  ENDIF

  IF (SYMTYPE(PIXIE_PATTERN) <> -1) || (SYMTYPE(PIXIE_TILE) <> -1)

  IFDEF RES32
  pagefit 0x47
  ENDIF
  IFDEF RES64
  pagefit 0x51
  ENDIF
  IFDEF RES128
  pagefit 0x59
  ENDIF

  align 256
  
_drawsprite2:		LDI  VIDMEM/256					; prepare the pointer to the video buffer					
					PHI  R14						; VIDMEM + Y * 8 + X / 8
					GLO  R13						; result goes to R14
  IFDEF RES32
					ANI  0x1F						; between 0 - 31
					SHL
					SHL
					SHL
					PLO  R14
  ENDIF
  IFDEF RES64
					ANI  0x3F						; between 0 - 63
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
					ANI  0x7F						; between 0 - 127
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
  IFDEF PIXIE_VSYNC										
					IDL
  ENDIF
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
$$exit:			    Cretn

  ENDIF

;------------------------------------------------------------------------------------------

  ENDIF
