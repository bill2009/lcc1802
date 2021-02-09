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
					
_initvideo:   
    ldireg R1, DisplayInt
    inp 1
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
					BZ   $$high_nible				; check if position is on low or high nible
					LDA  R15						; load the next byte of the sprite
					ANI  0xF
					STR  R2
					LDN  R14
					ANI  0xF0
					OR
					STR  R14						; OR in thr low nible of the sprite
					BR   $$next_line
$$high_nible:       LDA  R15						; load the next byte of the sprite
					ANI  0xF0
					STR  R2
					LDN  R14
					ANI  0xF
					OR
					STR  R14						; OR in thr high nible of the sprite
$$next_line:		GLO  R14						; advance the video buffer pointer to the next line
					ADI  0x08
					PLO  R14
					GHI  R14
					ADCI 0
					PHI  R14
					DEC  R11						; decrement the sprite's byte counter
					BR   $$byteLoop
$$exit			    cretn

;------------------------------------------------------------------------------------------
