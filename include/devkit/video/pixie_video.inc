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

    align    32

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

  ENDIF

;------------------------------------------------------------------------------------------

; =========================================================================================
; Initialize 1861 video output
; =========================================================================================

    align    16
					
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

    align    128

_drawsprite:			
					ld2  R15,'O',sp,(7)
					lda  R15
					plo  R11
					LDI  VIDMEM/256			; prepare the pointer to the video buffer					
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

$$ByteLoop:		GLO  R11						; exit if all bytes of the sprite have been drawn
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
					GHI  R14
					ADCI 0
					PHI  R14
					BR   $$ByteLoop
$$Exit			cretn

  ENDIF

;------------------------------------------------------------------------------------------
