					IF UseConsole == "TRUE"

; =========================================================================================

; =========================================================================================

ToDecimal:		GLO	 RF
				STR  R2
				
				LDI  lo(TD_lookup)
				PLO  RD
				LDI  hi(TD_lookup)
				PHI  RD

				DEC  RE
TD_loop1:		INC  RE
				LDI  00H
				STR  RE
				LDN  RD
TD_loop2:		SD
				BNF  TD_overflow
				STR  R2
				LDN  RE
				ADI  01H
				STR  RE
				BR   TD_loop2

TD_overflow:	LDA  RD
				SHR
				BNF  TD_loop1
				DEC  RD
				DEC  RD
				SEP  R5

TD_lookup:		db 0064H, 000AH, 0001H 

;------------------------------------------------------------------------------------------

					ENDIF