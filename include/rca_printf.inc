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

_putlcccx:				; R12 = character to output 
  IFDEF  PECOM 
	ldireg R13, 0xB102	; value required by 0xaab8 Pecom print routine
 	sex R2
    glo r12
	lbr 0xaab8
  ELSE
	ldireg R13, 0x3302	; value required by 0x2cd3 COMX print routine
    glo r12
    lbr 0x2cd3
  ENDIF
	cretn

	align	32
_gotoxy:				; R12 = x, R13 = y 
  IFDEF  PECOM 
	 ldireg R8, 0x7CC6
  ELSE
	 ldireg R8, 0x419A 
	 sex R8 
	 glo R13 
	 stxd 
  ENDIF
	 ldi 5 
$$loop40: 
	 plo R15 
	 glo R13 
	 shl 
	 plo R13 
	 ghi R13 
	 shlc 
	 phi R13 
	 dec R15 
	 glo R15 
	 bnz $$loop40 
	 sex R2 
	 glo R13 
	 str R2 
	 glo R12 
	 add 
	 plo R12 
	 ghi R13 
	 str R2 
	 ghi R12 
	 adc 
	 phi R12 
	 sex R8 
  IFDEF  COMX 
	 dec R8 
	 glo R12 
	 stxd 
	 ghi R12 
	 stxd 
	 dec R8 
	 dec R8 
  ENDIF
	 glo R12 
	 stxd 
	 ghi R12 
	 adi 0xf8 
	 stxd 
     sex R2
	cretn
