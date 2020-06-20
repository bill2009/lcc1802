					IF UseIO == "TRUE"

					IF System == "Elf"

HexpadPort			EQU  4
HexDisplayPort		EQU  4
ASCIIPort			EQU  7

					ENDIF

; =========================================================================================
; Waits until a key is pressed on the ASCII keyboard and returns its value
;
; Returns:
; RF.0		Byte read from the ASCII keyboard
; 
; =========================================================================================

					IF System == "Elf"
InputASCII:			B3	 InputASCII
IA_Release:			BN3  IA_Release
					ENDIF

					INP  ASCIIPort
					PLO  RF
					SEP  R5

;------------------------------------------------------------------------------------------


; =========================================================================================
; Waits until the input key is pressed returns the value entered into the hexpad
;
; Returns:
; RF.0		Byte read from the ASCII keyboard
; RF.1		8 bit seed for random number generation.
; 
; =========================================================================================

					IF System == "Elf"
InputHexpad:		GHI  RF
					ADI  0001H
					PHI  RF
					BN4  InputHexpad
IH_Release:			B4   IH_Release
					ENDIF

					INP  HexpadPort
					PLO  RF
					SEP  R5

;------------------------------------------------------------------------------------------


; =========================================================================================
; Outputs a value on the hex display
;
; Parameters:
; RF.0		Value to be shown on the hex display
; 
; =========================================================================================

					IF System == "Elf"

OutputHex:			GLO  RF
					STR  R2
					OUT  HexDisplayPort
					DEC  R2
					SEP  R5

					ENDIF

;------------------------------------------------------------------------------------------

					ENDIF