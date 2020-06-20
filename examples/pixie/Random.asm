					IF UseRandom == "TRUE"

; =========================================================================================
; Sets a bit seed value for the random number generator
;
; Parameters:
; RF		The seed value (16 bit)
; RF.0		The seed value (8 bit)
;
; Internal:
; RE		Pointer to random state
; =========================================================================================

SetSeed:			LDI  lo(RandomState)			; load the address of the random state
					PLO  RE
					LDI  hi(RandomState)
					PHI  RE
					GLO  RF							; copy the values in RF to the random state
					STR  RE
										
					IF RandomSize == 16
					INC  RE
					GHI  RF
					STR  RE
					ENDIF

					SEP  R5

;------------------------------------------------------------------------------------------


; =========================================================================================
; Generates a 16 bit or 8 bit (pseudo) random number
;
; Parameters:
; RF		16 bit random return value
; RF.0		8 bit random return value
;
; Internal:
; RE		Pointer to random state
; RD.0		Loop counter
; =========================================================================================

GetRandom:			GLO  RE							; save registers RE and RD.0 on the stack
					STXD
					GHI  RE
					STXD
					GLO  RD
					STXD

					LDI  lo(RandomState)			; load the address of the random state
					PLO  RE
					LDI  hi(RandomState)
					PHI  RE

					IF RandomSize == 16
					LDI  10H						; set up the loop counter to shift 16 bits
					PLO  RD
					ELSE
					LDI  08H						; set up the loop counter to shift 8 bits
					PLO  RD
					ENDIF

GRA_ShiftLoop:		GLO  RF							; shift the value in RF
					SHL
					PLO  RF

					IF RandomSize == 16
					GHI  RF							; extend to 16 bits
					RSHL
					PHI  RF
					ENDIF

					LDN  RE							; shift random state
					SHL

					IF RandomSize == 16
					STR  RE							; extend to 16 bits
					INC  RE
					LDN  RE
					RSHL

					ENDIF

					BNF GRA_BitZero
GRA_BitOne:			XRI  0A7H						; XOR over the random state
					STR  RE

					IF RandomSize == 16
					DEC  RE							; extend to 16 bits
					LDN  RE
					XRI  03EH
					STR  RE
					ENDIF

					GLO  RF							; add the bit to RF
					ORI  01H
					PLO  RF
					LBR  GRA_TestLoop
GRA_BitZero:		XRI  035H						; XOR over the random state
					STR  RE

					IF RandomSize == 16
					DEC  RE							; extend to 16 bits
					LDN  RE
					XRI  07AH
					STR  RE	
					ENDIF

GRA_TestLoop:		DEC  RD							; loop until all bits have been shifted
					GLO  RD
					BNZ  GRA_ShiftLoop
					
					IRX							; restore registers RE and RD.0
					LDXA
					PLO  RD
					LDXA
					PHI  RE
					LDX
					PLO  RE
					SEP  R5

;------------------------------------------------------------------------------------------


; =========================================================================================
; Data
; =========================================================================================

RandomState:		db 2 dup (?)

;------------------------------------------------------------------------------------------

					ENDIF