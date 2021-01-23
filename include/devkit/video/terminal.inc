;
; *******************************************************************
; *** This software is copyright 2021 by Marcel van Tongeren      ***
; *** with support from Bill Rowe and Fabrizio Caruso.            ***
; ***                                                             ***
; *** You have permission to use, modify, copy, and distribute    ***
; *** this software so long as this copyright notice is retained. ***
; *** This software may not be used in commercial applications    ***
; *** without express written permission from the author.         ***
; *******************************************************************
; 

  IFDEF ELF2K
_putchar_term_rom_routine   equ 0xF803
_baud_rate_factor equ 0x7FA3
    
_putchar_term:			; output character to terminal, character is R12
	glo R12
    lbr _putchar_term_rom_routine
    
    align    64
_getchar_term:			; receive character from terminal, 
    ldi  0x08
    plo  R15
    ldireg R13, $$delay
    ldireg R12, _baud_rate_factor
    ldn R12
    phi  R15
    shr 
    smi  0x01 
    phi  R14
$$wait_ef:
    b3   $$wait_ef
    sep  R13
    ghi  R15
    phi  R14 
$$loop:
    ghi  R15
    shr  
    bn3  $$branch_back
    ori  0x80 
$$back:
   phi  R15
   sep  R13
   dec  R15 
   nop  
   nop  
   glo  R15   
   bnz  $$loop
   req      
   sep  R13   
   sep  R13 
   ghi  R15 
   shr  
   plo R15
   ldi 0
   phi R15
   cretn
$$branch_back:
   br $$back

$$return:
   sep R3
$$delay:
   ghi R14
   shr
   plo R14
   sex R2
$$wait:
   dec R14
   glo R14
   bz $$return
   br $$wait
	
  ENDIF

