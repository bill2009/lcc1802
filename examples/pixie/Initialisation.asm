					INCLUDE "bitfuncs.inc"

; =========================================================================================
; Starting point of the program and initialisation of the CPU registers
;
; R0		Reserved as pointer to the DMA buffer
; R1		Reserved as interrupt vector
; R2		Main stack pointer
; R3		Main program counter
; R4		Program counter for standard call procedure
; R5		Program counter for standard return procedure
; R6		Reserved for temporary values from standard call/return procedures
; R7 - RF	Free to use in the program, not initialized
; =========================================================================================	
					CPU 1802
					ORG 0000H
		
Init:				LDI  lo(DisplayInt)
					PLO  R1
					LDI  hi(DisplayInt)
					PHI  R1
					LDI  lo(StackTop)
					PLO  R2
					LDI  hi(StackTop)
					PHI  R2
					LDI  lo(Start)
					PLO  R3
					LDI  hi(Start)
					PHI  R3
					LDI  lo(StdCall)
					PLO  R4
					LDI  hi(StdCall)
					PHI  R4
					LDI  lo(StdReturn)
					PLO  R5
					LDI  hi(StdReturn)
					PHI  R5
					SEP  R3

;------------------------------------------------------------------------------------------


; =========================================================================================
; Standard Call Procedure
; as described in RCA CDP1802 User Manual, page 61 
; =========================================================================================
STC_Exit:			SEP  R3
StdCall:			SEX  R2
					GHI  R6
					STXD
					GLO  R6
					STXD
					GHI  R3
					PHI  R6
					GLO  R3
					PLO  R6
					LDA  R6
					PHI  R3
					LDA  R6
					PLO  R3
					BR   STC_Exit

;------------------------------------------------------------------------------------------


; =========================================================================================
; Standard Return Procedure
; as described in RCA CDP1802 User Manual, page 61 
; =========================================================================================
STR_Exit:			SEP  R3
StdReturn			GHI  R6
					PHI  R3
					GLO  R6
					PLO  R3
					SEX  R2
					INC  R2
					LDXA
					PLO  R6
					LDX
					PHI  R6
					BR   STR_Exit

;------------------------------------------------------------------------------------------


; =========================================================================================
; Simple delay loop
; 
; Parameters:
; RF.0				Delay time
; =========================================================================================
Delay:				NOP
					DEC  RF
					GLO  RF
					BNZ  Delay
					SEP  R5

;------------------------------------------------------------------------------------------


; =========================================================================================
; Space for the main stack
; =========================================================================================

					ORG 00FFH
StackTop:

;------------------------------------------------------------------------------------------


; =========================================================================================
; Includes
; =========================================================================================

					INCLUDE "Graphics1861.asm"
					;INCLUDE "Console1861.asm"
					INCLUDE "Conversion.asm"
					INCLUDE "Random.asm"
					INCLUDE "Text1861.asm"
					INCLUDE "IO.asm"

;------------------------------------------------------------------------------------------
