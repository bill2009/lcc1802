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
  IF (SYMTYPE(MICRO) <> -1) || (SYMTYPE(CIDELSA) <> -1)
    select_vis
  IFNDEF TMC600 
    ldireg R8, _vis_text_color_mask
    ldireg R9, _vis_text_color
  ENDIF
    ldireg R10, _vis_cursor_position
    ldireg R11, _vis_column_counter
    lda R10
    phi R15           
    ldn R10
    plo R15             ; R15 = cursor position
    lda R11
    plo R14             ; R14.0 = column
    ldn R11
    plo R13             ; R13.0 = line
    glo r12
    smi 0x8             ; check on 0x8, backspace
    lbz $$backspace    
    smi 0x1             ; check on 0x9, tab
    lbz $$tab
    smi 0x1             ; check on 0xa, new line and carriage return
    lbz $$nlcr
    smi 0x1             ; check on 0xb, new line
    lbz $$nl
    smi 0x2             ; check on oxd, carriage return
    lbz $$cr
    glo r12
  IFNDEF TMC600 
    smi 0x21
    lbnf $$nocolorchange
    smi 0x3F
    lbdf $$nocolorchange
$$colorchange:          ; is character is between 0x21 and 0x60 adapt color
    glo r12
    sex R8
    and                 ; AND character with color mask, i.e. for 4 colors change to 0-0x3f range
    sex R9
    add                 ; ADD color code 0, 0x40, 0x80 or 0xc0
    skp
  ENDIF
$$nocolorchange:
    glo r12
    wait_display
    str R15             ; move the byte
    inc R15             ; cursor positon + 1
    inc R14             ; column position + 1
$$checkendofline:
    glo R14        
    smi _x_size         ; if column position is _x_size, it is the end of line
    lbnz $$saveposition ; if not end of line we are done, just save the new position in the saveposition routine
    inc R13             ; new line, so linecounter + 1
$$continue_cr
    ldi 0               
    plo R14             ; new line, so set column counter to 0
$$continue:
    glo R15
    smi _vis_bot_right_corner & 0xff
    ghi R15
    smbi _vis_bot_right_corner >> 8
    lbnf $$checkendpage ; if new position >= bottum_right_corner we need to wrap around to the top_left_corner value
    ldireg R15, _vis_top_left_corner
                        ; wrap R15 around to the top_left_corner value
$$checkendpage:
    glo R13 
    smi _y_size         ; if line counter is _y_size, it is the end of the page
    lbnz $$saveposition ; if not end of page we are done, just save the new position in the saveposition routine
    ldireg R8, _vis_video_base + 1
                        ; end of page so we need to scroll, first load pointer to low byte of the video base
    ldn R8
    adi _scroll_value
    plo R9
    dec R8
    ldn R8
	adci 0
    phi R9               ; R9 = video base + scroll_value (40 characters on all computer except CIDELSA 4)
    glo R9
    smi _vis_bot_right_corner & 0xff
    ghi R9
    smbi _vis_bot_right_corner >> 8
    lbnz $$storenewbase  ; if new video base = bottum_right_corner we need to wrap around to the top_left_corner value
    ldireg R9, _vis_top_left_corner
                        ; wrap R9 around to the top_left_corner value
$$storenewbase:
    ghi R9
    str R8
    inc R8
    glo R9
    str R8              ; store new video base on video base pointer
    glo R14
    sex R2
    str R2
    glo R15
    sm
    plo R8
    ghi R15
    smbi 0
    phi R8              ; R8 = cursor position - column counter to set position to start cleaning the bottom line after scrolling
    ldi _x_size
    plo R12             ; R12.0 = counter to print _x_size spaces
$$clearline:
    ldi _vis_space
    wait_display
    str R8              ; move the byte
    inc R8
	dec R12
	glo R12
	bnz $$clearline
    out_vis R9, 7       ; output new video base value to VIS
    dec R13
$$saveposition:
    sex R11
    glo R13             ; get line counter
    stxd                ; store 
    glo R14             ; get new column counter
    str R11             ; store 
    sex R10
    glo R15             ; get new cursor position - low
    stxd                ; store 
    ghi R15             ; get new cursor position - high
    str R10             ; store
$$return:
    c_return
$$nlcr:
    glo R14
    sex R2
    str R2
    ldi 0x28
    sm
    str R2
    glo R15
    add
    plo R15
    ghi R15
    adci 0
    phi R15
    inc R13
    lbr $$continue_cr
$$cr:
    glo R14
    sex R2
    str R2
    glo R15
    sm
    plo R15
    ghi R15
    smbi 0
    phi R15
    lbr $$continue_cr
$$nl:
    glo R15
    adi 0x28
    plo R15
    ghi R15
    adci 0
    phi R15
    inc R13
    lbr $$continue
$$backspace:
    glo R14
    lbz $$return
    dec R15
    dec R14
    lbr $$saveposition
$$tab:
    inc R14
    inc R15
    glo R14
    ani 0x7
    lbnz $$tab
    lbr $$checkendofline   
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
  IFDEF MICRO
    ldireg R8, _vis_column_counter
                        ; R8 = pointer to column position
    glo R12 
    str R8              ; Store new column position
    inc R8
    glo R13 
    str R8              ; Store new line position
    ldireg R8, _vis_cursor_position + 1
                        ; R8 = pointer to cursor position low
  ENDIF
  IFDEF CIDELSA
    ldireg R8, _vis_column_counter
                        ; R8 = pointer to column position
    glo R13 
    str R8              ; Store new column position
    inc R8
    glo R12 
    str R8              ; Store new line position
    ldireg R8, _vis_cursor_position + 1
                        ; R8 = pointer to cursor position low
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
    glo R12
    smi _vis_char_on_screen & 0xff
    plo R9
    ghi R12
    smbi _vis_char_on_screen >> 8
    lbnf $$notoutofrange
    phi R12
    glo R9
    plo R12
$$notoutofrange:
  IFDEF  COMX 
    sex R8 
    dec R8                
    glo R12 
    stxd 
    ghi R12 
    stxd                ; On COMX store new cursor position 0x4197/0x4198 in range 0 - 0x3BF
    dec R8 
    dec R8 
  ENDIF
    ldireg R9, _vis_video_base + 1
    sex R9
    glo R12 
    add
    str R8
    dec R8
    dec R9 
    ghi R12 
    adc 
    str R8              ; Store new cursor postion in range 0xF800 - 0xFBBF
    sex R2
    cretn
