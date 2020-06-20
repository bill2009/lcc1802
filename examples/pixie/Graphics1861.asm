;20-06-19 commented out org 0 for display buffer
					IF UseGraphics == "TRUE"
; =========================================================================================
; Display buffer
; =========================================================================================
					;ORG 0100H
					IF Resolution == "64x32"
DisplayBuffer:		db 256 dup (?)
					ENDIF

					IF Resolution == "64x64"
DisplayBuffer:		db 512 dup (?)
					ENDIF

					IF Resolution == "64x128"
DisplayBuffer:		db 1024 dup (?)
					ENDIF

					IF BackBuffer <> "OFF"

					IF Resolution == "64x32"
DoubleBuffer:		db 256 dup (?)
					ENDIF

					IF Resolution == "64x64"
DoubleBuffer:		db 512 dup (?)
					ENDIF

					IF Resolution == "64x128"
DoubleBuffer:		db 1024 dup (?)
					ENDIF
					ENDIF

Reserved:			db 8 dup (?)

;------------------------------------------------------------------------------------------


; =========================================================================================
; Interrupt and DMA service routine for the CDP1861 to display an effective resolution
; of 64 x 32 pixels, using a display buffer of 256 bytes.
; =========================================================================================

					IF Resolution == "64x32"

INT_Exit:			LDXA
					RET
DisplayInt:			DEC  R2
					SAV
					DEC  R2
					STR  R2
					NOP
					NOP
					NOP

					IF BackBuffer <> "SWAP"
					LDI  hi(DisplayBuffer)
					ENDIF

					IF BackBuffer == "SWAP"
					GHI  R7
					ENDIF

					PHI  R0
					LDI  00H
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

					IF Resolution == "64x64"

INT_Exit:			LDXA
					RET
DisplayInt:			NOP
					DEC  R2
					SAV
					DEC  R2
					STR  R2

					IF BackBuffer <> "SWAP"
					LDI  hi(DisplayBuffer)
					ENDIF

					IF BackBuffer == "SWAP"
					GHI  R7
					ENDIF

					PHI  R0
					LDI  00H
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

					IF Resolution == "64x128"

INT_Exit:			LDXA
					RET
DisplayInt:			NOP
					DEC  R2
					SAV
					DEC  R2
					STR  R2
					SEX  R2					
					SEX  R2
					
					IF BackBuffer <> "SWAP"
					LDI  hi(DisplayBuffer)
					ENDIF

					IF BackBuffer == "SWAP"
					GHI  R7
					ENDIF

					PHI  R0
					LDI  00H
					PLO  R0
					BR   INT_Exit

					ENDIF

;------------------------------------------------------------------------------------------


; =========================================================================================
; Parameters:
; RF		Pointer to the image
;
; Internal:
; RE		Pointer to video buffer
; =========================================================================================

					
CopyImage:			IF BackBuffer == "OFF"
					LDI  hi(DisplayBuffer)			; prepare the pointer to the video buffer
					PHI  RE
					ENDIF

					IF BackBuffer == "COPY"
					LDI  hi(DoubleBuffer)			; prepare the pointer to the back buffer
					PHI  RE
					ENDIF

					IF BackBuffer == "SWAP"
					GLO  R7							; prepare the pointer to the current back buffer
					PHI  RE
					ENDIF

					LDI  00H
					PLO  RE

CI_Loop:			LDA  RF
					STR  RE
					INC  RE
					GLO  RE
					BNZ  CI_Loop
					
					IF Resolution == "64x64"					
					IF BackBuffer == "OFF"
					LDI   hi(DisplayBuffer) + 1
					ENDIF

					IF BackBuffer == "COPY"
					LDI   hi(DoubleBuffer) + 1
					ENDIF

					IF BackBuffer == "SWAP"
					GLO  R7
					ADI	 01H
					ENDIF

					STR  R2
					GHI  RE
					SD
					BDF  CI_Loop				
					ENDIF

					IF Resolution == "64x128"
					IF BackBuffer == "OFF"
					LDI   hi(DisplayBuffer) + 3
					ENDIF

					IF BackBuffer == "COPY"
					LDI   hi(DoubleBuffer) + 3
					ENDIF

					IF BackBuffer == "SWAP"
					GLO  R7
					ADI  03H
					ENDIF

					STR  R2
					GHI  RE
					SD
					BDF  CI_Loop				
					ENDIF
					
					SEP  R5					

;------------------------------------------------------------------------------------------


; =========================================================================================
; Parameters:
; RF		Value for filling
;
; Internal:
; RE		Pointer to video buffer
; =========================================================================================

FillScreen:			IF BackBuffer == "OFF"
					LDI  hi(DisplayBuffer)			; prepare the pointer to the video buffer
					PHI  RE
					ENDIF

					IF BackBuffer == "COPY"
					LDI  hi(DoubleBuffer)			; prepare the pointer to the back buffer
					PHI  RE
					ENDIF

					IF BackBuffer == "SWAP"
					GLO  R7							; prepare the pointer to the current back buffer
					PHI  RE
					ENDIF

					LDI  00H
					PLO  RE

FS_Loop:			GHI  RF
					STR  RE
					INC  RE
					GLO  RE
					BNZ  FS_Loop
					
					IF Resolution == "64x64"
					IF BackBuffer == "OFF"
					LDI   hi(DisplayBuffer) + 1
					ENDIF

					IF BackBuffer == "COPY"
					LDI   hi(DoubleBuffer) + 1
					ENDIF

					IF BackBuffer == "SWAP"
					GLO  R7
					ADI  01H
					ENDIF

					STR  R2
					GHI  RE
					SD
					BDF  FS_Loop				
					ENDIF

					IF Resolution == "64x128"
					IF BackBuffer == "OFF"
					LDI   hi(DisplayBuffer) + 3
					ENDIF

					IF BackBuffer == "COPY"
					LDI   hi(DoubleBuffer) + 3
					ENDIF

					IF BackBuffer == "SWAP"
					GLO  R7
					ADI  03H
					ENDIF

					STR  R2
					GHI  RE
					SD
					BDF  FS_Loop				
					ENDIF
					
					SEP  R5					

;------------------------------------------------------------------------------------------


; =========================================================================================
; Parameters:
; RE.0		X coordinate of the sprite
; RE.1		Y coordinate of the sprite
; RF		Pointer to sprite
; RD		Size of the sprite in bytes
;
; Internal:
; RC		Pointer to video memory
; =========================================================================================

DrawSprite:			IF BackBuffer == "OFF"
					LDI  hi(DisplayBuffer)			; prepare the pointer to the video buffer
					ENDIF

					IF BackBuffer == "COPY"
					LDI  hi(DoubleBuffer)			; prepare the pointer to the back buffer
					ENDIF

					IF BackBuffer == "SWAP"
					GLO  R7							; prepare the pointer to the current back buffer
					ENDIF
					
					PHI  RC							; DisplayBuffer + Y * 8 + X / 8
					GHI  RE							; result goes to RC

					IF Resolution == "64x32"
					ANI  1FH						; between 0 - 31
					ENDIF
				
					IF Resolution == "64x64"
					ANI  3FH						; or 0 - 63
					ENDIF

					IF Resolution == "64x128"
					ANI  7FH						; or 0 - 127
					ENDIF

					SHL
					SHL
					SHL
					PLO  RC
					BNF  DSP_SkipIncrement
					GHI  RC
					ADI  01H
					PHI  RC
					
DSP_SkipIncrement:	GLO  RC
					STR  R2
					GLO  RE
					ANI  3FH
					SHR
					SHR
					SHR
					ADD
					PLO  RC
					GLO  RE							; calculate the number of required shifts 
					ANI  07H						; result to RE.1, replacing the Y coordinate
					PHI  RE							; RE.0 will be used later to count the shifts

DSP_ByteLoop:		GLO  RD							; exit if all bytes of the sprite have been drawn
					BZ   DSP_Exit
					
					IF Resolution == "64x32"		; or if we are about to draw outside the video buffer
					IF BackBuffer == "OFF"
					LDI   hi(DisplayBuffer)
					ENDIF

					IF BackBuffer == "COPY"
					LDI   hi(DoubleBuffer)
					ENDIF

					IF BackBuffer == "SWAP"
					GLO  R7
					ENDIF
					ENDIF

					IF Resolution == "64x64"
					IF BackBuffer == "OFF"
					LDI   hi(DisplayBuffer) + 1
					ENDIF

					IF BackBuffer == "COPY"
					LDI   hi(DoubleBuffer) + 1
					ENDIF

					IF BackBuffer == "SWAP"
					GLO  R7
					ADI  01H
					ENDIF
					ENDIF

					IF Resolution == "64x128"
					IF BackBuffer == "OFF"
					LDI   hi(DisplayBuffer) + 3
					ENDIF

					IF BackBuffer == "COPY"
					LDI   hi(DoubleBuffer) + 3
					ENDIF

					IF BackBuffer == "SWAP"
					GLO  R7
					ADI	 03H
					ENDIF
					ENDIF

					STR  R2
					GHI  RC
					SD
					BNF  DSP_Exit
					LDN	 RF							; load the next byte of the sprite into RB.0
					PLO  RB
					LDI  00H						; set RB.1 to OOH
					PHI  RB
					DEC  RD							; decrement the sprite's byte counter
					INC  RF							; increment the pointer to the sprite's bytes
					GHI  RE							; prepare the shift counter
					PLO  RE
DSP_ShiftLoop:		GLO  RE							; exit the loop if all shifts have been performed
					BZ   DSP_ShiftExit
					DEC  RE							; decrement the shift counter
					GLO  RB							; shift the values in RB
					SHR
					PLO  RB
					GHI  RB
					RSHR
					PHI  RB
					BR   DSP_ShiftLoop
DSP_ShiftExit:		SEX  RC							; store the shifted bytes in the video buffer
					GLO  RB
					XOR
					STR  RC
					INC  RC
					GHI  RB
					XOR
					STR  RC
					SEX  R2
					GLO  RC							; advance the video buffer pointer to the next line
					ADI  07H
					PLO  RC
					GHI  RC
					ADCI 00H
					PHI  RC
					BR   DSP_ByteLoop
DSP_Exit			SEP  R5

;------------------------------------------------------------------------------------------


; =========================================================================================
; Parameters:
; ----
;
; Internal:
; RE		Pointer to video buffer
; RF		Pointer to back buffer
; =========================================================================================
					
CopyBackBuffer:		IF BackBuffer <> "OFF"

					IF BackBuffer == "COPY"					
					LDI  hi(DisplayBuffer)			; prepare the pointer to the video buffer
					PHI  RE
					LDI  lo(DisplayBuffer)
					PLO  RE

					LDI  hi(DoubleBuffer)			; prepare the pointer to the back buffer
					PHI  RF
					LDI  lo(DoubleBuffer)
					PLO  RF
					
CBB_Loop:			LDA  RF
					STR  RE
					INC  RE
					GLO  RE
					BNZ  CBB_Loop
					
					IF Resolution == "64x64"
					LDI   hi(DisplayBuffer) + 1
					ENDIF

					IF Resolution == "64x128"
					LDI   hi(DisplayBuffer) + 3
					ENDIF

					STR  R2
					GHI  RE
					SD
					BDF  CBB_Loop
					ENDIF

					IF BackBuffer == "SWAP"
					GHI  R7
					SMI  hi(DisplayBuffer)
					BZ   CBB_Swap
					LDI  hi(DisplayBuffer)
					PHI  R7
					LDI  hi(DoubleBuffer)
					PLO  R7
					BR   CBB_Exit
CBB_Swap:			LDI  hi(DoubleBuffer)
					PHI  R7
					LDI  hi(DisplayBuffer)
					PLO  R7
					BR   CBB_Exit
					ENDIF

					ENDIF					
CBB_Exit:			SEP  R5		

;------------------------------------------------------------------------------------------

					ENDIF
