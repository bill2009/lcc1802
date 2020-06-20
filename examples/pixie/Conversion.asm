					IF UseConversion == "TRUE"

; =========================================================================================
; Conversion of a byte to a decimal string
; 
; Parameters:
; RF.1				Byte to convert to a decimal string.
;
; Internal:
; RF.0				Temporary store for digits.
; RE				Pointer to the decimal constants.
; RD				Pointer to the string buffer.
; =========================================================================================

ByteToString:		LDI  hi(BTS_Constants)
					PHI  RE
					LDI  lo(BTS_Constants)
					PLO  RE
					LDI  hi(BTS_Result)
					PHI  RD
					LDI  lo(BTS_Result)
					PLO  RD
					GHI  RF
					STR  R2
BTN_DigitLoop:		LDI  0000H
					PLO  RF
BTN_SubLoop:		LDN  RE
					SD
					BNF  BTS_Overflow
					STR  R2
					INC  RF
					BR   BTN_SubLoop
BTS_Overflow:		GLO  RF
					ADI  0030H
					STR  RD
					INC  RD
					LDA  RE
					SDI  0001H
					BNZ  BTN_DigitLoop
BTS_Exit:			LDI  0000H
					STR  RD
					SEP  R5

BTS_Constants:		db  0064H, 000AH, 0001H, 0000H

BTS_Result:			db 4 dup (?)


;------------------------------------------------------------------------------------------


; =========================================================================================
; Conversion of the lower four bits of a byte to a hexadecimal string
; 
; Parameters:
; RF.1				Byte to convert to a hexadecimal string.
;
; Internal:
; RF.0				Temporary store for digits.
; RE				Pointer to the string buffer.
; =========================================================================================

ByteToHexString:	LDI  hi(BTS_Result)
					PHI  RE
					LDI  lo(BTS_Result)
					PLO  RE
					GHI  RF
					ANI  000FH
					PHI  RF
					SMI  000AH
					BDF  BTHS_Skip
					GHI  RF
					ADI  0030H
					LBR	 BTHS_Exit
BTHS_Skip:			GHI  RF
					ADI  0037H
BTHS_Exit:			STR  RE
					INC  RE
					LDI  0000H
					STR  RE
					SEP  R5

;------------------------------------------------------------------------------------------

					ENDIF