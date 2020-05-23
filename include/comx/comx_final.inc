;comx_final.inc contains BASIC CALL routine as well as settings for BASIC SYSTEM paramters
;
; *******************************************************************
; *** This software is copyright 2020 by Marcel van Tongeren      ***
; *** with support from Bill Rowe and Fabrizio Caruso.			  ***
; ***                                                             ***
; *** You have permission to use, modify, copy, and distribute    ***
; *** this software so long as this copyright notice is retained. ***
; *** This software may not be used in commercial applications    ***
; *** without express written permission from the author.         ***
; *******************************************************************
; 
;20-05-23 First version
	
  IFDEF LCCCX
	align 256
$$defus:
	dw 0
	dw 0
	dw 0
	dw 0
	dw 0
	dw 0
$$sop
	dw 0x000a
	dw 0x0aa8
	dw 0xd6d0

  IFDEF COMX
	dw 0x3434
	dw 0x3030
	dw 0xc40d
	dw 0xffff
	dw 0x0384
	db 0x0d
$$eop:

	org 0x4281
	dw $$sop
	dw $$eop

	org 0x4292
	dw $$defus+0x26
	dw $$defus+0x26

	org 0x4299	
  ENDIF
  
  IFDEF TMC600
	dw 0x3633
	dw 0x3030
	dw 0xc40d
	dw 0xffff
	dw 0x0384
	db 0x0d
$$eop:

	org 0x6181
	dw $$sop
	dw $$eop

	org 0x6192
	dw $$defus+0x26
	dw $$defus+0x26

	org 0x6199
  ENDIF
  
  IFDEF PECOM
	dw 0x3032
	dw 0x3030
	dw 0xc40d
	dw 0xffff
	dw 0x0384
	db 0x0d
$$eop:

	org 0x81
	dw $$sop
	dw $$eop

	org 0x92
	dw $$defus+0x26
	dw $$defus+0x26

	org 0x6199
  ENDIF
	dw $$defus+0x26
  ENDIF