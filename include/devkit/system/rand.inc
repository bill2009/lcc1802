;
; *******************************************************************
; *** This software is copyright 2020 by Marcel van Tongeren      ***
; *** with support from Bill Rowe and Fabrizio Caruso.            ***
; ***                                                             ***
; *** You have permission to use, modify, copy, and distribute    ***
; *** this software so long as this copyright notice is retained. ***
; *** This software may not be used in commercial applications    ***
; *** without express written permission from the author.         ***
; *******************************************************************
; 
    align    64
_rand:
    ldireg R12, _seedvalue
    lda R12 
    bnz $$rand1b        ; Do rand if high byte of seed value != 0
    ldn R12
    bnz $$rand1a        ; Do rand if low byte of seed value != 0
  IFDEF PECOM 
$$zerolow:
    glo R0                    
    bz $$zerolow        ; Wait until R0.0 != 0
    phi R15             ; Use R0.0 as low byte of seed value
$$zerohigh:
    ghi R0
    bnz $$rand2         ; If R0.1 != 0 use as high byte of seed value
    br $$zerohigh       ; Wait until R0.1 != 0
  ENDIF
  IFDEF COMX 
$$zerolow:
    glo R0                    
    bz $$zerolow        ; Wait until R0.0 != 0
    phi R15             ; Use R0.0 as low byte of seed value
$$zerohigh:
    ghi R0
    bnz $$rand2         ; If R0.1 != 0 use as high byte of seed value
    br $$zerohigh       ; Wait until R0.1 != 0
  ENDIF
  IFNDEF LCCCX
    ldi 0xAC            
    phi R15
    ldi 0xE1            ; fixed seed value
    br $$rand2          
  ENDIF
  IFDEF TMC600
    ldi 0xAC            
    phi R15
    ldi 0xE1            ; fixed seed value
    br $$rand2          
  ENDIF
$$rand1a:
	dec R12
	lda R12
$$rand1b:
    phi R15             ; R15.1 = high byte of seed value
    ldn R12    
$$rand2:
    plo R15             ; R15.0 = low byte of seed value
    ldi 3
    plo R14
$$loop:
    glo R15
    ani 1
    str R2              ; Stack = bit 0 of seed value, rest is 0
    glo R15             ; D = low byte of seed value
    shr        
    shr        
    xor                 ; D bit 0 = bit 0 XOR bit 2
    plo R9
    ani 1
    str R2              ; Stack = bit 0 XOR bit 2
    glo R9
    shr        
    xor                 ; D bit 0 = bit 0/2 XOR bit 3
    plo R9
    ani 1
    str R2              ; Stack = bit 0/2 XOR bit 3
    glo R9
    shr        
    shr        
    xor                 ; D bit 0 = bit 0/2/3  XOR bit 5
    shr                 ; DF = new input bit
    ghi R15
    shrc                ; R15.1 >> 1, MSB = DF, new input bit, DF = LSB
    phi R15             
    glo R15
    shrc
    plo R15             ; R15.0 >> 1, MSB = DF, DF = LSB
    dec R14
    glo R14
    bnz $$loop
    glo R15
    str R12             ; Store low byte new seed value
    dec R12
    ghi R15				; R15.1 = seed value & 0x7f, allowing only postivie numbers
    str R12             ; Store high byte new seed value, 
    ani 0x7f
    phi R15           
    cretn
