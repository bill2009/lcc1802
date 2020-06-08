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

_setvideobase:		; set video base to 'vidmem', vidmem pointer is R12
	ldireg r14, _vis_buffer
  IFDEF PECOM 
	sex R3
	out 1
	db  2
	ldireg R8, 0x7cc8
  ENDIF
  IFDEF COMX 
	ldireg R8, 0x4195
  ENDIF
  IFDEF TMC600 
	ldireg R8, 0x6002
  ELSE
	ldireg R8, _vis_text_color_mask
	ldireg R9, _vis_text_color
	ldi 0
	str R9
	ldi 0x80
	str R8
  ENDIF  
	ldi 0
	str R14
  IFNDEF CIDELSA 
	ghi R12
	str R8
	inc R8
	glo R12
	str R8
  ENDIF  
  IFDEF TMC600 
    sex R3
    out 7
    db 0x70					; select VIS register 7
    sex 12
    out 5					; set new baseaddress
  ELSE
	sex R12
    out 7					; set new baseaddress
  ENDIF
  IFDEF CIDELSA 
    out 6					; set new baseaddress
    ldireg R12, _vis_out3_value
    ldi 0x80
    str R12
    out 3
    ldireg R12, 0xc8
    out 5
    sex R2
  ENDIF
	cretn

	align	32
_vidchar:			; write 'character' to 'vidmem' location in video memory
	ldireg r14, _vis_buffer
  IFDEF  PECOM 
	sex R3
	out 1
	db  2
  ENDIF
    ldn R14
	bnz $$flushbuffer
	glo r13
  IFDEF CIDELSA
  IFDEF DRACO
	b1  $$pushtobuffer	; Display period so buffer data
  ENDIF
  IFDEF ALTAIR
	bn1 $$pushtobuffer	; Display period so buffer data
  ENDIF
  IFDEF DESTROYER
	bn1 $$pushtobuffer	; Display period so buffer data
  ENDIF
  ELSE
	b1  $$pushtobuffer	; Display period so buffer data
  ENDIF
	str R12				; move the byte
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
	str r15
	glo r13
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
	str R12			; move the byte
	cretn


  IFDEF  PECOM 
	align	64
  ENDIF
  IFDEF  CIDELSA 
	align	64
  ENDIF
  IFDEF  COMX 
	align	32
  ENDIF
  IFDEF  TMC600 
	align	32
  ENDIF
_vidstrcpy:			; write 'text' to 'vidmem'
	ldireg r14, _vis_buffer
  IFDEF  PECOM 
	sex R3
	out 1
	db  2
	sex R2
  ENDIF
    ldn R14
	bz $$cpy
	phi r15
	ldi 0
	str r14
	inc r14
	lda r14
	plo r15
	ldn r14
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
	b1 $
  ENDIF
	str r15
$$cpy:
	ldireg R9, _vis_text_color
	ldn R9
	str 2
	lda R13			; pick up input pointer
	bz $$end
	add
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
	str R12			; move the byte
  IFDEF CIDELSA
    glo R12
    smi 0x28
    plo R12
    ghi R12
    smbi 0
    phi R12
    smi 0xf7
    bnz $$pos_ok
    glo R12
    adi 0x29
    plo R12
    ghi R12
    adci 5
    phi R12    
$$pos_ok:
  ELSE
	inc R12			; ++
  ENDIF
	br $$cpy
$$end:
	cretn

	align	64
_vidclr:			; clear screen by writing 0's to video memory
					; vidmem pointer is R12, vidlen is R13
	ldireg r14, _vis_buffer
  IFDEF  PECOM 
	sex R3
	out 1
	db  2
  ENDIF
    ldn R14
	bz $$cpy
	phi r15
	ldi 0
	str r14
	inc r14
	lda r14
	plo r15
	ldn r14
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
	str r15
$$cpy:
  IFDEF COMX
	ldi 0			; source a 0 for clearing the screen
  ELSE
	ldi 0x20		; source a 0 for clearing the screen
  ENDIF
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
	str R12			; move the byte
	inc R12			; ++
	dec R13			; decrement count
	glo R13			; check bottom byte
	bnz $$cpy
	ghi R13			; top byte
	bnz $$cpy
	cretn

  IFNDEF TMC600
_shapechar:				; shape 'number' of charcters from 'shapelocation'
						; shapelocation pointer is R12, start character R13
						; on R2: number of shapes loaded into R15.0, color R15.1 (b7, b6 & b5)
					
	ld2 R15,'O',sp,(7)
	ldi 0xf4
	phi R8				; R8 = charmem pointer
  IFDEF CIDELSA
  IFDEF DRACO
	ldireg R9, 0xffff	; R9 = vidmem pointer
  ENDIF
  IFDEF ALTAIR
	ldireg R9, 0xfbff	; R9 = vidmem pointer
  ENDIF
  IFDEF DESTROYER
	ldireg R9, 0xfbff	; R9 = vidmem pointer
  ENDIF
	ldireg R10, 0x00c9
  ELSE
	ldireg R9, 0xfbff	; R9 = vidmem pointer
  IFDEF  PECOM 
	sex R3
	out 1
	db  2
	ldireg R10,0x7CC3	; load current out 5 (low byte) video setting from 0x7cc3
  ENDIF    
  IFDEF  COMX 
	ldireg R10,0x41C3	; load current out 5 (low byte) video setting from 0x41C3
  ENDIF    
	ldn  R10                      
	ori  1
	plo  R10                        
	ldi  0                    
	phi  R10                        
  ENDIF
	sex R10
	out 5				; set cmem acccess
	sex R9
	out 6				; VIS R6 = 0xFFFF
	dec r9
	sex R2
$$nextshape:
	ldi 0
	plo R8
	glo R13
	str R9
	inc R13
  IFDEF CIDELSA
	lda R12
	str R2
  ENDIF
$$nextline:
  IFDEF CIDELSA
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
  ENDIF
	lda R12
  IFDEF CIDELSA
	lbr $$shape
$$usecolor2:
	lda R12
	ani 0x3f
	or
$$shape:
  ENDIF
	str R8
	inc R8
	glo R8
  IFDEF CIDELSA
  	smi 8 
  ELSE
  	smi 9 
  ENDIF
	lbnz $$nextline	; number of lines not 8 or 9 zero do next line
	dec r15
	glo r15
	lbnz $$nextshape
	glo R10
	ani 0xfe
	plo R10
	sex R10
	out 5
	sex R2
	cretn

	align 8
_shapecolor:			; shape color for 'character' start in R12, 'number' in R13 with 'color' on R2/R15
						; This routine will recolor characters starting from the character number given in 
						; characters and following numbers for the number of characters as specified in 'number'.
						; All characters will be colored as specified in parameter 'color' as in table below.
						; Colors specified are for default color definitions, in COMX using 3 in PECOM using 0.
						;	COMX					PECOM
						;	Regular		+128		Regular		+128
						; 0	black		red			black		green
						; 1	blue		magenta		red			yellow
						; 2 green		yellow		blue		cyan			
						; 3	cyan		white		magenta		white
						
  IFDEF  PECOM 
	sex R3
	out 1
	db  2
  ENDIF
	ld2 R15,'O',sp,(7)
	shrc
	shrc
	shrc
	ani  0xc0
	plo  R15			; put color bits in bit 6 and 7
	sex  R8
  IFDEF  PECOM 
	ldireg R8,0x7CC3; load current out 5 (low byte) video setting from 0x7cc3
  ENDIF    
  IFDEF  COMX 
	ldireg R8,0x41C3; load current out 5 (low byte) video setting from 0x41C3
  ENDIF    
  IFDEF  CIDELSA
    ldi  0xc8
  ELSE 
	ldn  R8                      
  ENDIF    
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
	ldireg R9,0xf800	; R9 is page mem pointer               
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
	glo  R12			; load character         
	str  R9             ; store on screen         
	inc  R12                 
	ldi  0xf4           ; R9 is cmem pointer
	phi  R9                      
$$nextline:
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
	glo  R8
	ani  0xfe            
	plo  R8
	sex  R8                     
	out  5                   
	cretn
  IFDEF CIDELSA 
    sex R2
  ENDIF
  ENDIF

_bgcolor:			; set background to 'color' in R12.0
					; 0: black
					; 1: green
					; 2: blue
					; 3: cyan
					; 4: red
					; 5: yellow
					; 6: magenta
					; 7: white
  IFDEF  PECOM 
	sex R3
	out 1
	db  2
	sex R2
	ldireg R8, 0x7cc4
  ENDIF
  IFDEF  COMX 
	ldireg R8, 0x41c0
  ENDIF
  IFDEF  TMC600 
	ldireg R8, 0x600b
  ENDIF
  IFDEF  CIDELSA 
	ldireg R8, _vis_out3_value
  ENDIF
	ldn R8			; get latest out 3 value
	ani 7			; get old background color
	plo R15
	ldi 0
	phi R15			; return old background in R15
	ldn R8			; get latest out 3 value
	ani 0xf8		; clear background color
	str R2			; store value on stack
	glo R12			; get new color
	ani 7			; limit to 3 bits
	or				; new color or latest out3 value
	str R8			; store new value 
  IFDEF  TMC600 
    sex R3
    out 7
    db 0x30			; select VIS register 3
    sex R8
    out 5			; set new color valu
  ELSE	
	sex R8
	out 3			; set new color value
  ENDIF
  IFDEF CIDELSA 
    sex R2
  ENDIF
	cretn

  IFDEF TMC600
_textcolor:			; set text to 'color' in R12.0
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
    db 0x20			; select VIS register 2
    sex R2
   	glo R12			; get color
	ani 0xf			
	str R2
	out 5			; set new color value
	dec R2
	cretn
  ELSE
_textcolor:			; set text to 'color' in R12.0
					; 0: 'low' character color
					; 1: 'high' chatacter color
					; For CIDELSA, if character_set is defined as 4, 4 colors will be available
	ldireg R8, _vis_text_color_mask
	ldireg R9, _vis_text_color
	ldn R8
	str R2
  IFDEF CIDELSA
	smi 0xc0
	lbz $$fourcolors
  ENDIF
	glo R12
    shrc
    shrc
    and
    str R9
    cretn
$$fourcolors:
	glo R12
	shrc
	shrc
	shrc
	and
	str R9
	cretn
  ENDIF
	
  IFDEF TMC600
	align 16
_setcolor:			; color RAM address in R12
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
    db 0x20			; select VIS register 2
    sex R2
   	glo R13			; get color
	ani 0xf			
	str R2
	out 5			; set new color value
	dec R2
	ghi R12
	adi 0xf8
	phi R12
	b1 $			; wait til video is quiet
	ldn R12
	str R12
	cretn
  ENDIF
  
  IFNDEF TMC600
_textcolordefinition:	; set text color to definition in R12.0
					;  b1	b0	RED	 BLUE	GREEN
					;   0	 0	CB0	 Cb1	PCB
					;   0	 1  CCB0 PCB	CCb1
					;   1  0/1 PCB	 CCB0	CCb1
  IFDEF  PECOM 
	sex R3
	out 1
	db  2
	sex R2
	ldireg R8, 0x7cc4
  ENDIF
  IFDEF  COMX 
	ldireg R8, 0x41C0
  ENDIF
  IFDEF  CIDELSA 
	ldireg R8, _vis_out3_value
  ENDIF
	ldn R8			; get latest out 3 value
	ani 0x9f		; clear text color definition
	str R2			; store value on stack
	glo R12			; get new color
	shrc
	shrc
	shrc
	shrc
	ani 0x60		; limit to 2 bits
	or				; new text color definition or latest out3 value
	str R8			; store new value 
	sex R8
	out 3			; set new color definition value
  IFDEF CIDELSA 
    sex R2
  ENDIF
	cretn
  ENDIF
  
  IFNDEF TMC600
_monochrome:		; set video to mono if mono=1, set to color if 0
  IFDEF  PECOM 
	sex R3
	out 1
	db  2
	sex R2
	ldireg R8, 0x7cc4
  ENDIF
  IFDEF  COMX 
	ldireg R8, 0x41C0
  ENDIF
  IFDEF  CIDELSA 
	ldireg R8, _vis_out3_value
  ENDIF
	ldn R8			; get latest out 3 value
	ani 0xf7		; clear cfc
	str R2			; store value on stack
	glo R12			; get new cfc
	ani 1			; limit to 1 bits
	shl
	shl
	shl
	or				; new cfc or latest out3 value
	str R8			; store new value 
	sex R8
	out 3			; set new cfc value
  IFDEF CIDELSA 
    sex R2
  ENDIF
	cretn
  ENDIF
