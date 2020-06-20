					IF UseText == "TRUE"

; =========================================================================================
; Draws a zero terminated string at specified screen coordinates. 
;
; Parameters:
; RF		Pointer to the string
; RE.0		X coordinate
; RE.1		Y coordinate
; 
; Internal:
; =========================================================================================

DrawString:			LDN  RF							; get character, exit if 0
					PLO  RC
					BZ   DS_Exit
					INC  RF
					
					GLO  RF							; push RF onto the stack
					STXD
					GHI  RF
					STXD
					
					GLO  RE							; push RE onto the stack
					STXD
					GHI  RE
					STXD
					
					SEP  R4							; draw the character
					dw DrawCharacter
					
					IRX								; restore RE
					LDXA
					PHI  RE
					LDX
					PLO  RE
					
					GLO  RF							; advance the x coordinate by the
					STXD							; width of the character + 1
					IRX
					GLO  RE
					ADD
					ADI  01H
					PLO  RE
					
					IRX								; restore RF
					LDXA
					PHI  RF
					LDX
					PLO  RF
					BR   DrawString					; continue with the next character

DS_Exit:			SEP  R5

;------------------------------------------------------------------------------------------


; =========================================================================================
; Draws a character at specified screen coordinates 
;
; Parameters:
; RE.0		X coordinate of the character
; RE.1		Y coordinate of the character
; RC.0		ASCII code of the character (20 - 5F)
; 
; Internal:
; RF		Pointer to the unpacked character's pattern
; RD		Pointer to the font
; RC.1		Temporary values
; =========================================================================================

DrawCharacter:		LDI  lo(CharacterPattern)		; RF points to the buffer for the character pattern
					PLO  RF
					LDI  hi(CharacterPattern)
					PHI  RF
					
					LDI  lo(Font)					; RD points to the font
					PLO  RD
					LDI  hi(Font)
					PHI  RD
					
					GLO  RC							; calculate the offset in the font
					SMI  020H						; (( character code - 20) / 2) * 6
					ANI  0FEH
					PHI  RC
					SHL
					STXD
					IRX					
					GHI  RC
					ADD
					STXD
					IRX
					LBNF DC_SkipHighByte
					GHI  RD
					ADI	 01H
					PHI  RD
					
DC_SkipHighByte:	GLO  RD							; add to the address in RD
					ADD	
					PLO  RD
					BNF  DC_SkipHighByte2
					GHI  RD
					ADI  01H
					PHI  RD

DC_SkipHighByte2:	LDN  RD							; get the width of the first character pattern
					SHR
					SHR
					SHR
					SHR
					STXD
					IRX

					GLO  RC							; do we need the first or the second pattern?
					ANI  01H
					PHI  RC
					LBNZ  DC_PrepareSecond
					
					LDX								; prepare the mask					
					PLO  RB	
					LDI  00H
					PHI  RB
DC_MaskLoop:		GHI  RB
					SHR
					ORI  80H
					PHI  RB
					DEC  RB
					GLO  RB
					LBNZ  DC_MaskLoop

					LDX
					STXD							; keep the width of the first pattern on the stack
					BR   DC_CopyPattern

DC_PrepareSecond:	LDX								; use the width of the first pattern for shifting
					PHI  RB

					LDN  RD							; keep the width of the second character pattern on the stack
					ANI  07H
					STXD

DC_CopyPattern:		INC  RD
					LDI  05H						; prepare a loop over the five bytes of the pattern
					PLO  RC
					
DC_ByteLoop:		LDN  RD							; get a byte from the font
					STXD
					IRX
					INC  RD
					
					GHI  RC
					BNZ  DC_ByteShift

					GHI  RB							; mask out the first pattern
					AND
					STXD
					IRX
					BR   DC_ByteWrite

DC_ByteShift:		GHI  RB							; shift the second pattern
					PLO  RB
DC_ShiftLoop:		LDX
					SHL
					STXD
					IRX
					DEC  RB
					GLO  RB
					BNZ  DC_ShiftLoop

DC_ByteWrite:		LDX								; write the byte
					STR  RF
					INC  RF

					DEC  RC							; continue until all bytes of the pattern are done
					GLO  RC
					BNZ  DC_ByteLoop

					LDI  lo(CharacterPattern)		; restore RF to the beginning of the pattern
					PLO  RF
					LDI  05H						; set the length of the pattern
					PLO  RD
					SEP  R4							; call sprite routine to draw  
					dw   DrawSprite

					IRX								; clean up and exit
					LDX
					PLO  RF
					SEP  R5

;------------------------------------------------------------------------------------------


; =========================================================================================
; Buffer for unpacked characters
; =========================================================================================

CharacterPattern:	db 5 dup (?)

;------------------------------------------------------------------------------------------


; =========================================================================================
; Font definition
;
; The font has 64 or 96 printable characters, selected by setting Use96Char to "TRUE" or
; "FALSE". The font requires 192 bytes memory if 64 characters are used. Full 96 characters
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

Font:				db  0011H, 0040H, 0040H, 0040H, 0000H, 0040H		; space and !
					db  0035H, 00AAH, 00BFH, 000AH, 001FH, 000AH		; " and #
					db  0033H, 0074H, 00C4H, 0048H, 0070H, 00D4H		; $ and %
					db  0041H, 0048H, 00A8H, 0040H, 00A0H, 00D0H		; & and '
					db  0022H, 0060H, 0090H, 0090H, 0090H, 0060H		; ( and )
					db  0033H, 0000H, 00A8H, 005CH, 00A8H, 0000H		; * and +
					db  0022H, 0000H, 0000H, 0030H, 0040H, 0080H		; , and -
					db  0013H, 0010H, 0010H, 0020H, 0040H, 00C0H		; . and /
					db  0033H, 0048H, 00B8H, 00A8H, 00A8H, 005CH		; 0 and 1
					db  0033H, 00D8H, 0024H, 0048H, 0084H, 00F8H		; 2 and 3
					db  0033H, 003CH, 00B0H, 00F8H, 0024H, 0038H		; 4 and 5
					db  0033H, 005CH, 0084H, 00C4H, 00A8H, 0048H		; 6 and 7		
					db  0033H, 0048H, 00B4H, 004CH, 00A4H, 0048H		; 8 and 9
					db  0012H, 0000H, 0020H, 0080H, 0020H, 00C0H		; : and ;		
					db  0032H, 0020H, 0058H, 0080H, 0058H, 0020H		; < and =
					db  0033H, 0088H, 0054H, 0024H, 0048H, 0088H		; > and ?
					db  0033H, 0048H, 00F4H, 009CH, 00B4H, 0054H		; @ and A
					db  0033H, 00CCH, 00B0H, 00D0H, 00B0H, 00CCH		; B and C
					db  0033H, 00DCH, 00B0H, 00B8H, 00B0H, 00DCH		; D and E
					db  0033H, 00ECH, 0090H, 00D0H, 0094H, 008CH		; F and G
					db  0033H, 00BCH, 00A8H, 00E8H, 00A8H, 00BCH		; H and I
					db  0033H, 0034H, 0034H, 0038H, 00B4H, 0054H		; J and K
					db  0035H, 0091H, 009BH, 0095H, 0091H, 00F1H		; L and M
					db  0043H, 009EH, 00DAH, 00BAH, 009AH, 009EH		; N and O
					db  0034H, 00DEH, 00B2H, 00D2H, 0096H, 009EH		; P and Q
					db  0033H, 00CCH, 00B0H, 00C8H, 00A4H, 00B8H		; R and S
					db  0033H, 00F4H, 0054H, 0054H, 0054H, 005CH		; T and U
					db  0035H, 00B1H, 00B1H, 00B1H, 00B5H, 004AH		; V and W
					db  0033H, 00B4H, 00B4H, 0048H, 00A8H, 00A8H		; X and Y
					db  0032H, 00F8H, 0030H, 0050H, 0090H, 00F8H		; Z and [
					db  0032H, 0098H, 0088H, 0048H, 0028H, 0038H		; \ and ]
					db  0033H, 0040H, 00A0H, 0000H, 0000H, 001CH		; ^ and _

					IF Use96Char == "TRUE"

					db  0023H, 0040H, 0098H, 0028H, 0028H, 0018H		; ' and a
					db  0033H, 0080H, 00CCH, 00B0H, 00B0H, 00CCH		; b and c
					db  0033H, 0020H, 006CH, 00B4H, 00B8H, 006CH		; d and e
					db  0023H, 0058H, 00A8H, 00F0H, 0088H, 00B0H		; f and g
					db  0031H, 0090H, 0080H, 00D0H, 00B0H, 00B0H		; h and i
					db  0023H, 0060H, 0028H, 0070H, 0068H, 00A8H		; j and k
					db  0025H, 0080H, 0094H, 00AAH, 00AAH, 006AH		; l and m
					db  0033H, 0000H, 00C8H, 00B4H, 00B4H, 00A8H		; n and o
					db  0033H, 0000H, 00CCH, 00B4H, 00CCH, 0084H		; p and q
					db  0023H, 0000H, 0058H, 00B0H, 0088H, 00B0H		; r and s
					db  0023H, 0080H, 00E8H, 00A8H, 00A8H, 0058H		; t and u
					db  0035H, 0000H, 00B1H, 00B5H, 00B5H, 004AH		; v and w
					db  0033H, 0000H, 00B4H, 004CH, 0044H, 00A8H		; x and y
					db  0033H, 000CH, 00E8H, 0030H, 0048H, 00ECH		; z and {
					db  0013H, 00E0H, 00A0H, 0090H, 00A0H, 00E0H		; | and }
					db  0044H, 0005H, 005AH, 00A5H, 000AH, 0005H		; ~ and DEL

					ENDIF

;------------------------------------------------------------------------------------------

					ENDIF