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

_initvideo:             ; initialize vis video output and system settings
    ldireg R8, _vis_buffer
    ldi 0
    str R8
  IFNDEF TMC600 
    ldireg R8, _vis_text_color_mask
  IFDEF NTSC5_6_7
    ldi 0x3f
  ELSE
    ldi 0x7f
  ENDIF
    str R8
    ldireg R8, _vis_text_color
    ldi 0
    str R8
  ENDIF
    select_vis
    ldireg R8, _vis_out3
    ldi _vis_out3_value
    str R8
    out_vis R8, 3
    ldireg R9, _vis_out5
  IFDEF COMX
    ldn R9
    plo R8
    ldi 0
    phi R8
  ELSE
    ldireg R8, _vis_out5_value
    glo R8
    str R9
  ENDIF
    out_vis R8, 5
    ldireg R8, _vis_cursor_position
    ldi _vis_top_left_corner >> 8
    str R8
    inc R8
    ldi _vis_top_left_corner & 0xff
    str R8
  IFDEF COMX
  IFDEF PRINTF_ROM
    ldi 0x97
    plo R8
    ldi 0
    str R8
    inc R8
    str R8
    inc R8
    inc R8
    str R8    
  ENDIF
  ENDIF
    ldireg R8, _vis_column_counter
	ldi 0
	str R8    
    inc R8
	str R8    
    c_return

_setvideobase:          ; set video base to 'vidmem', vidmem pointer is R12
    select_vis  
    ldireg R8, _vis_video_base
    ghi R12
    str R8
    inc R8
    glo R12
    str R8
    out_vis R12, 7
    cretn

    align    64
_vidcharxy:             ; write 'text' to position x, y
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
    bnf $$loop40        ; if no need to add result (DF=0) continue *40 loop
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
    bnz $$loop40        ; check marking, if 0 stop, otherwise continue *40 loop
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
    sdbi 0xfc
  ELSE
    sdbi 0xfb
  ENDIF
    stxd                ; M(R2) = 0xFC10/0xFBC0 - M(R2), M(R2) is at this time y*40
    glo R13 
    add 
    plo R12 
    inc R2
    ghi R13 
    adc 
    phi R12             ; R12 = 0xfc10 - R12(y) * 40 + R13(x)
  ELSE
    phi R13             ; R13.1 = 0x28, marking DF=1 on an addition need
$$loop40: 
    glo R13 
    shl 
    plo R13 
    ghi R13 
    shlc 
    phi R13             ; R13 = R13 * 2
    bnf $$loop40        ; if no need to add result (DF=0) continue *40 loop
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
    bnz $$loop40        ; check marking, if 0 stop, otherwise continue *40 loop
    glo R12
    add
    plo R12
    inc R2 
    ghi R12
    adc
    adi 0xf8
    phi R12             ; R12 = vidmem = y + x * 40
  ENDIF
    ld2 R13,'O',sp,(7)  ; move text pointer to R13
_vidchar:               ; write 'character' to 'vidmem' location in video memory
    ldireg r14, _vis_buffer
    select_vis
    ldn R14
    bnz $$flushbuffer
    glo r13
  IF (SYMTYPE(ALTAIR) <> -1) || (SYMTYPE(DESTROYER) <> -1)
    bn1 $$pushtobuffer  ; Display period so buffer data
  ELSE
    b1  $$pushtobuffer  ; Display period so buffer data
  ENDIF
    str R12             ; move the byte
    cretn
$$pushtobuffer:
    ghi r12
    str r14
    inc r14
    glo r12
    str r14
    inc r14
    glo r13
    str r14
    cretn
$$flushbuffer:
    phi r15
    ldi 0
    str r14
    inc r14
    lda r14
    plo r15
    ldn r14
    wait_display
    str r15
    glo r13
    wait_display
    str R12             ; move the byte
    cretn

_vidstrcpyxy:           ; write 'text' to position x, y
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
    sdbi 0xfc
  ELSE
    sdbi 0xfb
  ENDIF
    stxd                ; M(R2) = 0xFC10/0xFBC0 - M(R2), M(R2) is at this time y*40
    glo R13 
    add 
    plo R12 
    inc R2
    ghi R13 
    adc 
    phi R12             ; R12 = 0xfc10 - R12(y) * 40 + R13(x)
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
    adi 0xf8
    phi R12             ; R12 = vidmem = y + x * 40
  ENDIF
    ld2 R13,'O',sp,(7)  ; move text pointer to R13
_vidstrcpy:             ; write 'text' to 'vidmem'
    ldireg r14, _vis_buffer
  IFNDEF TMC600 
    ldireg R8, _vis_text_color_mask
    ldireg R9, _vis_text_color
  ENDIF
    select_vis
    ldn R14
    lbz $$cpy
    phi r15
    ldi 0
    str r14
    inc r14
    lda r14
    plo r15
    ldn r14
    wait_display
    str r15
$$cpy:
    ldn R13             ; pick up input pointer
    lbz $$end
  IFNDEF TMC600 
    smi 0x21
    lbnf $$nocolorchange
    smi 0x5a
    lbdf $$nocolorchange
$$colorchange:          ; if character is between 0x21 and 0x7a adapt color
    lda R13
    sex R8
    and
    sex R9
    add
    skp
  ENDIF
$$nocolorchange:
    lda R13
    wait_display
    str R12             ; move the byte
  IFDEF CIDELSA
    glo R12
    smi 0x28
    plo R12
    ghi R12
    smbi 0
    phi R12
    smi 0xf7
    lbz $$pos_nok
  ELSE
    inc R12             ; ++
  ENDIF
    lbr $$cpy
$$end:
    c_return
$$pos_nok:
    glo R12
    adi 0x29
    plo R12
    ghi R12
    adci 5
    phi R12    
    lbr $$cpy

    align    64
_vidclr:                ; clear screen by writing 0's to video memory
                        ; vidmem pointer is R12, vidlen is R13
    ldireg r14, _vis_buffer
    select_vis
    ldn R14
    bz $$cpy
    phi r15
    ldi 0
    str r14
    inc r14
    lda r14
    plo r15
    ldn r14
    wait_display
    str r15
$$cpy:
    ldi _vis_space      ; source a 0 or 0x20 for clearing the screen
    wait_display
    str R12             ; move the byte
    inc R12             ; ++
    dec R13             ; decrement count
    glo R13             ; check bottom byte
    bnz $$cpy
    ghi R13             ; top byte
    bnz $$cpy
    cretn

  IF (SYMTYPE(TMC600) = -1) && (SYMTYPE(NTSC5_6_7) == -1)
_shapechar:             ; shape 'number' of charcters from 'shapelocation'
                        ; shapelocation pointer is R12, start character R13
                        ; on R2: number of shapes loaded into R15.0, color R15.1 (b7, b6 & b5)
                    
    ld2 R15,'O',sp,(7)
    ldi 0xf4
    phi R8              ; R8 = charmem pointer
    ldireg R9, _vis_shape_pointer    
                        ; R9 = vidmem pointer
    ldireg R10, _vis_out5
    select_vis   
    ldn  R10                      
    ori  1
    plo  R10                        
    ldi  0                    
    phi  R10                        
    sex R10
    out 5                ; set cmem acccess
    sex R9
    out 6                ; VIS R6 = 0xFFFF
    dec r9
    sex R2
$$nextshape:
    ldi 0
    plo R8
    glo R13
    str R9
    inc R13
  IFDEF  PECOM 
    ghi R13
    lbz $$standardlines
    ldi 8
    plo R8
    ldi 0
    str R8
    plo 8
    str R8
    inc R8    
    ghi R13
    lskp
$$standardlines:
  ENDIF
     ldi _vis_char_lines
    plo R14
  IFDEF CIDELSA
    lda R12
    str R2
$$nextline:
    ghi R15
    lbnz $$usecolor1
    ldn R2
$$usecolor1:
    shl
    str R2
    lsdf
    req
    skp
    seq
    ghi R15
    lbnz $$usecolor2
    lbr $$shape
$$usecolor2:
    lda R12
    ani 0x3f
    or
  ELSE
$$nextline:
    ghi R15
    lbz $$dontusecolor
    ani 0xc0
    str 2    
    lda R12
  IFDEF COMX
    ani 0x3f
  ENDIF
    or
    skp
$$dontusecolor:
    lda R12
  ENDIF
$$shape:
    str R8
    inc R8
    glo R8
    dec R14
    glo R14
    lbnz $$nextline     ; number of lines not 8 or 9 zero do next line
  IFDEF MICRO
  IF (SYMTYPE(PAL1) <> -1) || (SYMTYPE(NTSC2_9) <> -1)  || (SYMTYPE(NTSC1_4_8) <> -1) || (SYMTYPE(NTSC3) <> -1)
    inc R12
  ENDIF
  ENDIF
    dec r15
    glo r15
    lbnz $$nextshape
    dec R10
    glo R10
    ani 0xfe
    plo R10
    sex R10
    out 5
    sex R2
    cretn

    align 8
_shapecolor:            ; shape color for 'character' start in R12, 'number' in R13 with 'color' on R2/R15
                        ; This routine will recolor characters starting from the character number given in 
                        ; characters and following numbers for the number of characters as specified in 'number'.
                        ; All characters will be colored as specified in parameter 'color' as in table below.
                        ; Colors specified are for default color definitions, default using 3.
                        ;    Regular     +128       
                        ; 0  black       red        
                        ; 1  blue        magenta     
                        ; 2  green       yellow               
                        ; 3  cyan        white       
                        
    select_vis
    ld2 R15,'O',sp,(7)
    shrc
    shrc
    shrc
    ani  0xc0
    plo  R15            ; put color bits in bit 6 and 7
    sex  R8
    ldireg R8,_vis_out5 ; load current out 5 (low byte) video setting from 0x41C3
    ldn  R8                      
    ori  1
    plo  R8                        
    ldi  0                    
    phi  R8                        
    out  5                        
    ldi  0xff           
    phi  R9                  
    plo  R9               
    sex  R9
    out  6                       
$$nextchar:
    ldireg R9,0xf800    ; R9 is page mem pointer               
    wait_display
    glo  R12            ; load character         
    str  R9             ; store on screen         
    inc  R12                 
    ldi  0xf4           ; R9 is cmem pointer
    phi  R9                      
$$nextline:
    wait_display
    ldi  0x3f                  
    and                 ; clear color bit         
    str  R9                      
    glo  R15            ; get color bits         
    or                            
    str  R9             ; or color bits into cmem          
    inc  R9                        
    glo  R9                        
    xri  09H            ; check on last line       
    lbnz  $$nextline           
    glo  R13            ; check on last character           
    dec  R13                     
    lbnz  $$nextchar  
    dec     R8
    glo  R8
    ani  0xfe            
    plo  R8
    sex  R8                     
    out  5                   
    c_return
  ENDIF

_bgcolor:               ; set background to 'color' in R12.0
                        ; 0: black
                        ; 1: green
                        ; 2: blue
                        ; 3: cyan
                        ; 4: red
                        ; 5: yellow
                        ; 6: magenta
                        ; 7: white
    select_vis_sex2
    ldireg R8, _vis_out3
    ldn R8              ; get latest out 3 value
    ani 7               ; get old background color
    plo R15
    ldi 0
    phi R15             ; return old background in R15
    ldn R8              ; get latest out 3 value
    ani 0xf8            ; clear background color
    str R2              ; store value on stack
    glo R12             ; get new color
    ani 7               ; limit to 3 bits
    or                  ; new color or latest out3 value
    str R8              ; store new value 
    out_vis R8, 3
    c_return

  IFDEF TMC600
_textcolor:             ; set text to 'color' in R12.0
                        ; 0: black
                        ; 1: green
                        ; 2: blue
                        ; 3: cyan
                        ; 4: red
                        ; 5: yellow
                        ; 6: magenta
                        ; 7: white
                        ; bit 3 = 1: blink, 0: no blink
    sex R3
    out 7
    db 0x20             ; select VIS register 2
    sex R2
       glo R12          ; get color
    ani 0xf            
    str R2
    out 5               ; set new color value
    dec R2
    cretn
  ELSE
_textcolor:             ; set text to 'color' in R12.0
                        ; 0: 'low' character color (character 0-127)
                        ; 1: 'high' chatacter color (character 128-255)
                        ; If character_set is defined as 4, 4 colors will be available (0-0x3f, 0x40-0x7f etc.) Only numbers and capitals
    ldireg R8, _vis_text_color_mask
    ldireg R9, _vis_text_color
    ldireg R10, colortable
    glo R12
    str R2
    glo R10
    add
    plo R10
    ghi R10
    adci 0
    phi R10
    ldn R8
    xri 0xff
    str R2
    ldn R10
    shrc
    shrc
    shrc
    and
    str R9
    cretn
  ENDIF
    
  IFDEF TMC600
    align 16
_setcolor:              ; color RAM address in R12
                        ; set text to 'color' in R13.0
                        ; 0: black
                        ; 1: green
                        ; 2: blue
                        ; 3: cyan
                        ; 4: red
                        ; 5: yellow
                        ; 6: magenta
                        ; 7: white
                        ; bit 3 = 1: blink, 0: no blink
    sex R3
    out 7
    db 0x20             ; select VIS register 2
    sex R2
       glo R13          ; get color
    ani 0xf            
    str R2
    out 5               ; set new color value
    dec R2
    ghi R12
    adi 0xf8
    phi R12
    b1 $                ; wait til video is quiet
    ldn R12
    str R12
    cretn
  ENDIF
  
  IFNDEF TMC600
_textcolordefinition:   ; set text color to definition in R12.0
                        ;  b1    b0    RED     BLUE    GREEN
                        ;   0     0    CB0     CB1     PCB
                        ;   0     1    CCB0    PCB     CCB1
                        ;   1    0/1   PCB     CCB0    CCB1
    select_vis_sex2
    ldireg R8, _vis_out3
    ldn R8              ; get latest out 3 value
    ani 0x9f            ; clear text color definition
    str R2              ; store value on stack
    glo R12             ; get new color
    shrc
    shrc
    shrc
    shrc
    ani 0x60            ; limit to 2 bits
    or                  ; new text color definition or latest out3 value
    str R8              ; store new value 
    sex R8
    out 3               ; set new color definition value
    c_return
  ENDIF
  
  IFNDEF TMC600
_monochrome:            ; set video to mono if mono=1, set to color if 0
    select_vis_sex2
    ldireg R8, _vis_out3
    ldn R8              ; get latest out 3 value
    ani 0xf7            ; clear cfc
    str R2              ; store value on stack
    glo R12             ; get new cfc
    ani 1               ; limit to 1 bits
    shl
    shl
    shl
    or                  ; new cfc or latest out3 value
    str R8              ; store new value 
    sex R8
    out 3               ; set new cfc value
    c_return
  ENDIF
