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

  IFDEF COMX 
_vis_cursor_position   equ 0x4193
_vis_video_base        equ 0x4195
_vis_shape_pointer     equ 0xfbff
_vis_out3_value        equ 0xE0
_vis_out3              equ 0x41c0
_vis_out5_value        equ 0x80
_vis_out5              equ 0x41c3
_vis_space             equ 0x20
_vis_char_lines        equ 9
_vis_top_left_corner   equ 0xf800
_vis_bot_right_corner  equ 0xfbc0
_vis_char_on_screen    equ 0x3c0
_x_size                equ 40
_y_size                equ 24
_scroll_value          equ 40
  ENDIF
  
  IFDEF PECOM 
_vis_cursor_position   equ 0x7cc5
_vis_video_base        equ 0x7cc8
_vis_shape_pointer     equ 0xfbff
_vis_out3_value        equ 0xe0
_vis_out3              equ 0x7cc4
_vis_out5_value        equ 0x80
_vis_out5              equ 0x7CC3
_vis_space             equ 0x20
_vis_char_lines        equ 9
_vis_top_left_corner   equ 0xf800
_vis_bot_right_corner  equ 0xfbc0
_vis_char_on_screen    equ 0x3c0
_x_size                equ 40
_y_size                equ 24
_scroll_value          equ 40
  ENDIF

  IFDEF TMC600 
_vis_cursor_position   equ 0x6002
_vis_out3_value        equ 0x80
_vis_out3              equ 0x600b
_vis_out5_value        equ 0x80
_vis_out5              equ 0x600c
_vis_space             equ 0x20
_vis_top_left_corner   equ 0xf800
_vis_bot_right_corner  equ 0xfbc0
_vis_char_on_screen    equ 0x3c0
_x_size                equ 40
_y_size                equ 24
_scroll_value          equ 40
  ENDIF  
  
  IFDEF CIDELSA
_vis_out3_value        equ 0xe0
_vis_out5_value        equ 0xc8
_vis_space             equ 0x20
_vis_char_lines        equ 8
_scroll_value          equ 4

  IFDEF DRACO
_vis_shape_pointer     equ 0xffff
_vis_top_left_corner   equ 0xfc10
_vis_bot_right_corner  equ 0xfc38
_vis_char_on_screen    equ 0x438
_x_size                equ 27
_y_size                equ 40
  ENDIF
  
  IFDEF ALTAIR
_vis_shape_pointer     equ 0xfbff
_vis_top_left_corner   equ 0xfbc0
_vis_bot_right_corner  equ 0xfbe8
_vis_char_on_screen    equ 0x3e8
_x_size                equ 25
_y_size                equ 40
  ENDIF
  
  IFDEF DESTROYER
_vis_shape_pointer     equ 0xfbff
_vis_top_left_corner   equ 0xfbc0
_vis_bot_right_corner  equ 0xfbe8
_vis_char_on_screen    equ 0x3e8
_x_size                equ 25
_y_size                equ 40
  ENDIF
  
  ENDIF
  
  IFDEF MICRO
_vis_out3_value        equ 0xe0
  IF (SYMTYPE(PAL1) <> -1) || (SYMTYPE(PAL2) <> -1)
_vis_out5_value        equ 0x80
  ELSE
_vis_out5_value        equ 0x88
  ENDIF
_vis_space             equ 0x20
  IF (SYMTYPE(PAL1) <> -1) || (SYMTYPE(NTSC2_9) <> -1)  || (SYMTYPE(NTSC1_4_8) <> -1) || (SYMTYPE(NTSC3) <> -1)
_vis_char_lines        equ 8
  ELSE
_vis_char_lines        equ 9
  ENDIF
_vis_shape_pointer     equ 0xfbff
_vis_io_group          equ 0x80
_vis_top_left_corner   equ 0xf800
_vis_bot_right_corner  equ 0xfbc0
_vis_char_on_screen    equ 0x3c0
_x_size                equ 40
_y_size                equ 24
_scroll_value          equ 40
  ENDIF


select_vis macro
  IFDEF  PECOM 
    sex R3
    out 1
    db  2
  ENDIF
  IFDEF  MICRO 
    sex R3
    out 1
    db  _vis_io_group
  ENDIF
  endm

select_vis_sex2 macro
  IFDEF  PECOM 
    sex R3
    out 1
    db  2
    sex R2
  ENDIF
  IFDEF  MICRO 
    sex R3
    out 1
    db  _vis_io_group
    sex R2
  ENDIF
  endm

wait_display macro
  IFDEF CIDELSA
  IFDEF DRACO
    b1 $
  ENDIF
  IFDEF ALTAIR
    bn1 $
  ENDIF
  IFDEF DESTROYER
    bn1 $
  ENDIF
  ELSE
    b1 $			; wait til video is quiet
  ENDIF
  endm

out_vis:    macro    reg, output
  IFDEF TMC600 
    sex R3
    out 7
    db output<<4    ; select VIS register 'reg'
    sex reg
    out 5           ; set new baseaddress
  ELSE
    sex reg
    out output      ; set new baseaddress
  ENDIF
  endm

c_return:    macro
  IFDEF CIDELSA 
    sex R2
  ENDIF
  IFDEF MICRO 
    sex R2
  ENDIF
    cretn
  endm

colortable:
    db 3            ; 0: textcolor(COLOR_BLACK)
    db 1            ; 1: textcolor(COLOR_GREEN)
    db 2            ; 2: textcolor(COLOR_BLUE)
    db 0            ; 3: textcolor(COLOR_CYAN)
    db 3            ; 3: textcolor(COLOR_RED)
    db 3            ; 5: textcolor(COLOR_CYAN)
    db 2            ; 6: textcolor(COLOR_MAGENTA)
    db 2            ; 7: textcolor(COLOR_WHITE)
