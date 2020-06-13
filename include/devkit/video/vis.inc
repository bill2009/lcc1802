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


  IFDEF COMX 
_vis_video_base		equ	0x4195
_vis_shape_pointer	equ	0xfbff
_vis_out3_value		equ	0xE0
_vis_out3			equ	0x41C0
_vis_out5_value		equ	0x80
_vis_out5			equ	0x41C3
_vis_space			equ	0
_vis_char_lines		equ	9
  ENDIF
  
  IFDEF PECOM 
_vis_video_base		equ 0x7cc8
_vis_shape_pointer	equ 0xfbff
_vis_out3_value		equ	0xe0
_vis_out3			equ	0x7cc4
_vis_out5_value		equ	0x80
_vis_out5			equ 0x7CC3
_vis_space			equ 0x20
_vis_char_lines		equ 9
  ENDIF

  IFDEF TMC600 
_vis_video_base		equ 0x6002
_vis_out3_value		equ	0x80
_vis_out3			equ	0x600b
_vis_out5_value		equ	0x80
_vis_out5			equ 0x600C
_vis_space			equ 0x20
  ENDIF  
  
  IFDEF CIDELSA
_vis_out3_value		equ	0xe0
_vis_out5_value		equ	0xc8
_vis_space			equ 0x20
_vis_char_lines		equ 8

  IFDEF DRACO
_vis_shape_pointer	equ 0xffff
  ENDIF
  
  IFDEF ALTAIR
_vis_shape_pointer	equ 0xfbff
  ENDIF
  
  IFDEF DESTROYER
_vis_shape_pointer	equ 0xfbff
  ENDIF
  
  ENDIF

SELECT_VIS MACRO
  IFDEF  PECOM 
	sex R3
	out 1
	db  2
  ENDIF
  ENDM

SELECT_VIS_SEX2 MACRO
  IFDEF  PECOM 
	sex R3
	out 1
	db  2
	sex R2
  ENDIF
  ENDM

WAIT_DISPLAY MACRO
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
  ENDM

colortable:
	db 1			; 0: textcolor(COLOR_BLACK)
	db 1			; 1: textcolor(COLOR_GREEN)
	db 0			; 2: textcolor(COLOR_BLUE)
	db 0			; 3: textcolor(COLOR_CYAN)
	db 3			; 3: textcolor(COLOR_RED)
	db 3			; 5: textcolor(COLOR_CYAN)
	db 2			; 6: textcolor(COLOR_MAGENTA)
	db 2			; 7: textcolor(COLOR_WHITE)
