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
 
_putlcccx:              ; R12 = character to output 
  IFDEF  PECOM 
    ldireg R13, 0xB102  ; value required by 0xaab8 Pecom print routine
     sex R2
    glo r12
    lbr 0xaab8
  ENDIF
  IFDEF  COMX 
    ldireg R13, 0x3302  ; value required by 0x2cd3 COMX print routine
    glo r12
    lbr 0x2cd3
  ENDIF
  IFDEF TMC600 
    ldireg R13, 0x2703  ; value required by 0x1fd4 TMC600 print routine
    glo r12
    lbr 0x1fd4
  ENDIF
    cretn

  IFDEF TMC600 
    align 64
  ELSE
    align 32
  ENDIF
_gotoxy:                ; R12 = x, R13 = y 
  IFDEF  PECOM 
    sex R3
    out 1
    db  2
    ldireg R8, 0x7CC5   ; R8 = pointer to current cursor position
    lda R8            
    phi R15
    ldn 8
    plo R15             ; R15 = current cursor position
    ldi 0x20
    b1 $
    str R15             ; Put a space on current cursor position
  ENDIF
  IFDEF  COMX 
    ldireg R8, 0x419A   ; R8 = pointer to column position
    sex R8 
    glo R13 
    stxd                ; Store new column position
  ENDIF
  IFDEF  TMC600 
    ldireg R8, 0x6001   ; R8 = pointer to column position
    sex R8 
    glo R13 
    sdi 0x28
    str 8               ; Store new column position
    inc R8
    lda R8            
    phi R15
    ldn 8
    plo R15             ; R15 = current cursor position
    ldi 0x20
    b1 $
    str R15             ; Put a space on current cursor position
  ENDIF
    sex R2
    ldi 0
    stxd
    str R2              ; M(R2) = result value = 0
    ldi 0x28                 
  IFDEF CIDELSA
    phi R12             ; R12.1 = 0x28, marking DF=1 on an addition need
$$loop40: 
    glo R12 
    shl 
    plo R12 
    ghi R12 
    shlc 
    phi R12             ; R12 = R12 * 2
    lbnf $$loop40       ; if no need to add result (DF=0) continue *40 loop
    glo R12 
    add 
    str R2
    inc R2 
    ghi R12 
    ani 0x3f            ; filter out addition marking
    adc 
    stxd                ; store addition result on M(R2)
    ghi R12
    ani 0xc0            
    lbnz $$loop40       ; check marking, if 0 stop, otherwise continue *40 loop
    ldn R2
  IFDEF DRACO
    sdi 0x10
  ELSE
    sdi 0xc0
  ENDIF
    str R2 
    inc R2
    ldn R2
  IFDEF DRACO  
    sdbi 0x3c
  ELSE
    sdbi 0x3b
  ENDIF
    stxd                ; M(R2) = 0x3C10/0x3BC0 - M(R2), M(R2) is at this time y*40
    glo R13 
    add 
    plo R12 
    inc R2
    ghi R13 
    adc 
    phi R12                ; R12 = 0xfc10 - R12(y) * 40 + R13(x)
  ELSE
    phi R13             ; R13.1 = 0x28, marking DF=1 on an addition need
$$loop40: 
    glo R13 
    shl 
    plo R13 
    ghi R13 
    shlc 
    phi R13             ; R13 = R13 * 2
    lbnf $$loop40       ; if no need to add result (DF=0) continue *40 loop
    glo R13 
    add 
    str R2
    inc R2 
    ghi R13 
    ani 0x3f            ; filter out addition marking
    adc 
    stxd                ; store addition result on M(R2)
    ghi R13
    ani 0xc0            
    lbnz $$loop40       ; check marking, if 0 stop, otherwise continue *40 loop
    glo R12
    add
    plo R12
    inc R2 
    ghi R12
    adc
    phi R12            ; R12 = y + x * 40
  ENDIF
    sex R8 
  IFDEF  COMX 
    dec R8                
    glo R12 
    stxd 
    ghi R12 
    stxd                ; On COMX store new cursor position 0x4197/0x4198 in range 0 - 0x3BF
    dec R8 
    dec R8 
  ENDIF
    glo R12 
    stxd 
    ghi R12 
    adi 0xf8 
    stxd                ; Store new cursor postion in range 0xF800 - 0xFBBF
    sex R2
    cretn
