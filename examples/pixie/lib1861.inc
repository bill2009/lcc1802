System				EQU "Elf"

UseIO				EQU "TRUE"

UseRandom			EQU "TRUE"
RandomSize			EQU 8				; 8 or 16

UseGraphics			EQU "TRUE"
Resolution			EQU "64x64"			; "64x32", "64x64" or "64x128"
BackBuffer			EQU "OFF"			; 'OFF', 'COPY' or 'SWAP'

UseText				EQU "TRUE"
Use96Char			EQU "TRUE"			; TRUE = 96 char, FALSE = 64 char

UseConsole			EQU "FALSE"

UseConversion		EQU "TRUE"

;					INCLUDE "StdDefs.asm"
					INCLUDE "WRInitialisation.inc"


; =========================================================================================
; Main
; =========================================================================================

Start:				SEX  R2								; start CDP1861
					LDI  00H							; clear screen
					PHI  RF
					SEP  R4
					dw   FillScreen
					SEP  R4
					dw   CopyBackBuffer
					SEP  R4
					dw   CopyBackBuffer
					INP  1

MainLoop:			SEP  R4								; let's demonstrate the text functions
					dw   Text
					SEP  R4								; let's draw some sprites
					dw   BigSprites
					BR   MainLoop

;------------------------------------------------------------------------------------------


; =========================================================================================
; Let's show off the 96 character font: 
; =========================================================================================

Text:				LDI  00H							; clear screen
					PHI  RF
					SEP  R4
					dw   FillScreen

					LDI  hi(FontTitle)
					PHI  RF
					LDI  lo(FontTitle)
					PLO  RF
					LDI  08H
					PLO  RE
					LDI  00H
					PHI  RE
					SEP  R4
					dw   DrawString

					LDI  hi(Font1)
					PHI  RF
					LDI  lo(Font1)
					PLO  RF
					LDI  00H
					PLO  RE
					LDI  12H
					PHI  RE
					SEP  R4
					dw   DrawString

					LDI  hi(Font2)
					PHI  RF
					LDI  lo(Font2)
					PLO  RF
					LDI  00H
					PLO  RE
					LDI  18H
					PHI  RE
					SEP  R4
					dw   DrawString

					LDI  hi(Font3)
					PHI  RF
					LDI  lo(Font3)
					PLO  RF
					LDI  00H
					PLO  RE
					LDI  1EH
					PHI  RE
					SEP  R4
					dw   DrawString

					LDI  hi(Font4)
					PHI  RF
					LDI  lo(Font4)
					PLO  RF
					LDI  00H
					PLO  RE
					LDI  24H
					PHI  RE
					SEP  R4
					dw   DrawString

					LDI  hi(Font5)
					PHI  RF
					LDI  lo(Font5)
					PLO  RF
					LDI  00H
					PLO  RE
					LDI  2AH
					PHI  RE
					SEP  R4
					dw   DrawString

					LDI  hi(Font6)
					PHI  RF
					LDI  lo(Font6)
					PLO  RF
					LDI  00H
					PLO  RE
					LDI  30H
					PHI  RE
					SEP  R4
					dw   DrawString

					LDI  hi(PressI)
					PHI  RF
					LDI  lo(PressI)
					PLO  RF
					LDI  0FH
					PLO  RE
					LDI  3AH
					PHI  RE
					SEP  R4
					dw   DrawString

					SEP  R4
					dw   CopyBackBuffer

					SEP  R4								; wait for input from the hex keyboard
					dw   InputHexpad
					SEP  R5

;------------------------------------------------------------------------------------------


; =========================================================================================
; Let's draw some big sprites
; =========================================================================================

BigSprites:			LDI  00H							; clear screen
					PHI  RF
					SEP  R4
					dw   FillScreen

					LDI  0008H							; the Klingon ship is actually made up of four sprites
					PLO  RE								; drawn next to each other
					LDI  0007H
					PHI  RE
					LDI  hi(Klingon_0)
					PHI  RF
					LDI  lo(Klingon_0)
					PLO  RF
					LDI  000FH
					PLO	 RD
					SEP  R4
					dw   DrawSprite

					LDI  0010H
					PLO  RE
					LDI  0000H
					PHI  RE
					LDI  hi(Klingon_1)
					PHI  RF
					LDI  lo(Klingon_1)
					PLO  RF
					LDI  000EH
					PLO	 RD
					SEP  R4
					dw   DrawSprite

					LDI  0018H
					PLO  RE
					LDI  0003H
					PHI  RE
					LDI  hi(Klingon_2)
					PHI  RF
					LDI  lo(Klingon_2)
					PLO  RF
					LDI  000BH
					PLO	 RD
					SEP  R4
					dw   DrawSprite

					LDI  0020H
					PLO  RE
					LDI  000CH
					PHI  RE
					LDI  hi(Klingon_3)
					PHI  RF
					LDI  lo(Klingon_3)
					PLO  RF
					LDI  000AH
					PLO	 RD
					SEP  R4
					dw   DrawSprite

					LDI  0020H							; same with the Romulans
					PLO  RE
					LDI  0020H
					PHI  RE
					LDI  hi(Romulan_0)
					PHI  RF
					LDI  lo(Romulan_0)
					PLO  RF
					LDI  000BH
					PLO	 RD
					SEP  R4
					dw   DrawSprite

					LDI  0028H
					PLO  RE
					LDI  0025H
					PHI  RE
					LDI  hi(Romulan_1)
					PHI  RF
					LDI  lo(Romulan_1)
					PLO  RF
					LDI  0008H
					PLO	 RD
					SEP  R4
					dw   DrawSprite

					LDI  0030H
					PLO  RE
					LDI  0026H
					PHI  RE
					LDI  hi(Romulan_2)
					PHI  RF
					LDI  lo(Romulan_2)
					PLO  RF
					LDI  0006H
					PLO	 RD
					SEP  R4
					dw   DrawSprite

					LDI  0038H
					PLO  RE
					LDI  0020H
					PHI  RE
					LDI  hi(Romulan_3)
					PHI  RF
					LDI  lo(Romulan_3)
					PLO  RF
					LDI  0008H
					PLO	 RD
					SEP  R4
					dw   DrawSprite

					LDI  hi(Title)
					PHI  RF
					LDI  lo(Title)
					PLO  RF
					LDI  0DH
					PLO  RE
					LDI  32H
					PHI  RE
					SEP  R4
					dw   DrawString

					LDI  hi(PressI)
					PHI  RF
					LDI  lo(PressI)
					PLO  RF
					LDI  0FH
					PLO  RE
					LDI  38H
					PHI  RE
					SEP  R4
					dw   DrawString

					SEP  R4
					dw   CopyBackBuffer

					SEP  R4								; wait for input from the hex keyboard
					dw   InputHexpad
					SEP  R5

;------------------------------------------------------------------------------------------


; =========================================================================================
; Strings
; =========================================================================================

FontTitle:			db	"96 char. font\0"
Font1:				db  " !\"#$%&'()*+,-.\\\0"
Font2:				db  "0123456789:;<=>?\0"
Font3:				db  "@ABCDEFGHIJKLMN\0"
Font4:				db  "OPQRSTUVWXYZ[/]\0"
Font5:				db  "^_`abcdefghijklmn\0"
Font6:				db  "opqrstuvwxyz{|}~\0"

Title:				db "Big Sprites!\0"

PressI				db "(Press I)\0"

;------------------------------------------------------------------------------------------


; =========================================================================================
; Graphics
; =========================================================================================

Klingon_0			db 0001H, 0003H, 0007H, 000FH, 000FH, 001FH, 0038H, 0070H			; offset 7
					db 00E0H, 00C0H, 00C0H, 00C0H, 0060H, 0060H, 0020H

Klingon_1			db 0006H, 0006H, 000FH, 007FH, 007FH, 007FH, 001FH, 00DFH			; offset 0
					db 00D9H, 00C9H, 00E6H, 00F0H, 00FFH, 001FH

Klingon_2			db 00E0H, 00E0H, 00E0H, 0080H, 00B8H, 00BCH, 003EH, 007FH			; offset 3
					db 00FFH, 00FFH, 0081H

Klingon_3			db 0080H, 00C0H, 00E0H, 0070H, 0030H, 0030H, 0030H, 0030H			; offset 12
					db 0060H, 0040H

;------------------------------------------------------------------------------------------

Romulan_0			db 0060H, 00F0H, 00F0H, 00F0H, 0060H, 0070H, 0038H, 001CH			; offset 0
					db 000FH, 0007H, 0003H

Romulan_1			db 0006H, 000FH, 007FH, 00FFH, 00FFH, 00F9H, 0039H, 000FH			; offset 5

Romulan_2			db 0001H, 00E3H, 00FFH, 00FEH, 00FCH, 00C0H							; offset 6

Romulan_3			db 0060H, 00F0H, 00F0H, 00F0H, 0060H, 00E0H, 00C0H, 0080H			; offset 0

;------------------------------------------------------------------------------------------

