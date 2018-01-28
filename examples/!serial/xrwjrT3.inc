NAK:	EQU 0x15
SOH:	EQU 0x01
EOT:	EQU 0x04
ACK:	EQU 0x06
CAN:	EQU 0x18
Rrcv:	EQU 8
Rsnd:	EQU 9
blksize:	EQU 128
; XMODEM receiver based on xr.asm by Michael H Riley and serial routines by Josh Bensadon   
; See bottom of file for full acknowledgements and links.
; On entry R12 points to memory where received data will go
; On exit R15 =EOT for success, 'T' for timeout, or ffxx where xx is the first char received if other than SOH or EOT

	align 64
_XR:	ldaD	Rsnd,serout
	ldaD    Rrcv,serinT
	ld2z	R15	;clear the return code


initNAK: ldi    NAK	; need to send NAK to start
	sep     Rsnd
	sep     Rrcv  	; wait for next incoming character
	bnf     ckeot	;continue if no timeout

	ldi     NAK	; resend NAK to start
      	sep     Rsnd
      	sep     Rrcv    ; wait for next incoming character
        bnf     ckeot	;continue if no timeout

	ldi     NAK	; resend NAK to start
      	sep     Rsnd
      	sep     Rrcv    ; wait for next incoming character
        bnf     ckeot	;continue if no timeout

	ldi	'T'	;set return value to 'T'
	plo	R15
	cretn

filelp:    ;receive address is in R12

;begining of block read. returns to filelp or exits to filedn   				
           sep     Rrcv               ; wait for next incoming character(will not time out)

ckeot:    plo	   R15		; save char received
	   xri     EOT          ; check for EOT
           lbz     filedn       ; jump if so
           glo	   R15		;
           xri	   SOH		; check for SOH   
           lbnz	   interrupt	; interrupted transmission - return interrupting character

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
           cretn	           	; and return to caller - R15 contains EOT

interrupt:    
           ldi     CAN                  ; invalid char received - try to cancel the session
           sep     Rsnd
           ldi	   0xff			;make sure R15 isn't 0
           phi     R15
           cretn	           	; and return to caller - R15 contains interrupting character


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
