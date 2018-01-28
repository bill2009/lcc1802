NAK:	EQU 0x15;'N'
SOH:	EQU 0x01;'S'
EOT:	EQU 0x04;'T'
ACK:	EQU 0x06;'K'
Rrcv:	EQU 8
Rsnd:	EQU 9
blksize:	EQU 128
trc:	MACRO
	dec 2
	str 2
	out 7
	ENDM
; XMODEM receiver based on xr.asm by Michael H Riley and serial routines by Josh Bensadon   
; See bottom of file for full acknowledgements and links.
; On entry R12 points to memory where received data will go
; On exit R15 has the last byte written
	align 64
_XR:	   ldaD	   Rsnd,serout
	   ldaD    Rrcv,serinT
	ldi '<'
	trc 
	   ldi     NAK                 ; need to send NAK to start
           sep     Rsnd
filelp:    ;receive address is in R12, length goes in R11

;begining of block read. returns to filelp or exits to filedn   				
           sep     Rrcv               ; wait for next incoming character
           bnf     .ckeot		;continue if no timeout
	   ldaD    R15,0		;set return value to 0
  	   ldi 'T'
	   trc
	   cretn

.ckeot:    smi     EOT              ; check for EOT
           lbz     filedn           ; jump if so

	   sep     Rrcv               ; read block number
           sep     Rrcv               ; read inverted block number

           ldi     blksize             ; 128 bytes to receive
           plo     r11

readlp:    sep     Rrcv               ; read data byte
           str     r12                  ; store into output buffer
           inc     r12                  ; point to next position
           dec     r11                  ; decrement block count
           glo     r11                  ; see if done
           bnz     readlp              ; loop back if not
;end of block read
           sep     Rrcv               ; read checksum byte


           ldi     ACK                  ; send an ACK
           sep     Rsnd
           lbr     filelp              ; loop back for more
filedn:    
           ldi     ACK                  ; acknowledge end of transmission
           sep     Rsnd
	   glo	   R12			;copy last address to return register
	   plo     R15
	   ghi     R12
	   phi     R15
	ldi '>'
	trc	   
           cretn	           	; and return to caller


; *******************************************************************
; *** This software is copyright 2005 by Michael H Riley          ***
; *** You have permission to use, modify, copy, and distribute    ***
; *** this software so long as this copyright notice is retained. ***
; *** This software may not be used in commercial applications    ***
; *** without express written permission from the author.         ***
; *******************************************************************
; *********************************************************************
; *** xmodem code based on xr.asm copyright 2005 by Michael H Riley ***
; *** You have permission to use, modify, copy, and distribute      ***
; *** this software so long as this copyright notice is retained.   ***
; *** This software may not be used in commercial applications      ***
; *** without express written permission from the author.           ***
; *********************************************************************
;http://www.elf-emulation.com/software.html
