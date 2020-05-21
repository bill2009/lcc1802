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
  IFDEF  PECOM 
	ldireg R8, 0x7cc8
  ENDIF
  IFDEF  COMX 
	ldireg R8, 0x4195
  ENDIF
  IFDEF  TMC600 
	ldireg R8, 0x6002
  ENDIF
	ghi R12
	str R8
	inc R8
	glo R12
	str R8
  IFDEF  TMC600 
    sex R3
    out 7
    db 0x70					; select VIS register 7
    sex 12
    out 5					; set new baseaddress
  ELSE
	sex R12
    out 7					; set new baseaddress
  ENDIF
    sex R2
	cretn

	align	16
_vidchar:			; write 'character' to 'vidmem' location in video memory
  IFDEF  PECOM 
	sex R3
	out 1
	db  2
    sex R2
  ENDIF
	glo r13
	b1  $			; wait til video is quiet
	str R12			; move the byte
	cretn

	align	16
_vidstrcpy:			; write 'text' to 'vidmem'
  IFDEF  PECOM 
	sex R3
	out 1
	db  2
    sex R2
  ENDIF
$$cpy:
	lda R13			; pick up input pointer
	bz $$end
	b1  $			; wait til video is quiet
	str R12			; move the byte
	inc R12			; ++
	br $$cpy
$$end:
	cretn

	align	16
_vidclr:			; clear screen by writing 0's to video memory
					; vidmem pointer is R12, vidlen is R13
  IFDEF  PECOM 
	sex R3
	out 1
	db  2
  ENDIF
$$cpy:
  IFDEF TMC600
	ldi 0x20		; source a 0 for clearing the screen
  ELSE
	ldi 0			; source a 0 for clearing the screen
  ENDIF
	b1  $			; wait til video is quiet
	str R12			; move the byte
	inc R12			; ++
	dec R13			; decrement count
	glo R13			; check bottom byte
	bnz $$cpy
	ghi R13			; top byte
	bnz $$cpy
 IFDEF  PECOM 
    sex R2
  ENDIF
	cretn

  IFNDEF TMC600
	align	32
_shapechar:			; shape 'number' of charcters from 'shapelocation'
					; shapelocation pointer is R12, number of shapes is R13
  IFDEF  PECOM 
	sex R3
	out 1
	db  2
    sex R2
  ENDIF
	ldi 0xf7
	phi R8
	ldi 0xfb
	phi R9
$$nextshape:
	ldi 0xC0 
	plo R8			; R8 = charmem pointer
	plo r9			; R9 = vidmem pointer
	ldi 9 
	plo R10			; R10.0 = number of lines
	lda 12			; shapechar routine load next charcater
	phi R10			; R10.1 = character
$$nextline:
	ghi R10
	b1  $			; wait til video is quiet
	str R9			; store character in vidmem
	inc R9
	lda R12
	str R8			; store first shape line in charmem
	inc R8
	dec R10
	glo R10
	bnz $$nextline	; number of lines - 1 if not zero do next line
	dec r13
	glo r13
	bnz $$nextshape
	cretn
  ENDIF
  
  IFNDEF TMC600
	align 64
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
  ELSE
	ldireg R8,0x41C3; load current out 5 (low byte) video setting from 0x41C3
  ENDIF    
	ldn  R8                      
	phi  R8                        
	ldi  0x81                    
	plo  R8                        
	out  5                        
	ldi  0xff           
	phi  R9                  
	plo  R9               
	sex  R9
	out  6                       
$$nextchar:
	ldireg R9,0xf800	; R9 is page mem pointer               
	b1   $           
	glo  R12			; load character         
	str  R9             ; store on screen         
	inc  R12                 
	ldi  0xf4           ; R9 is cmem pointer
	phi  R9                      
$$nextline:
	b1   $
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
	ldi  0x80                   
	plo  R8
	sex  R8                     
	out  5                   
	sex  R2     
$$exit:
	cretn
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
	ldireg R8, 0x7cc4
  ENDIF
  IFDEF  COMX 
	ldireg R8, 0x41c0
  ENDIF
  IFDEF  TMC600 
	ldireg R8, 0x600b
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
	sex R2
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
  ENDIF
  
  IFNDEF TMC600
_textcolordefinition:	; set text color to definition in R12.0
					;  b1	b0	RED	 BLUE	GREEN
					;   0	 0	CB0	 Cb1	PCB
					;   0	 1  CCB0 PCB	CCb1
					;   1  0/1 PCB	 CCB0	CCb1
  IFDEF  PECOM 
	ldireg R8, 0x7cc4
  ELSE
	ldireg R8, 0x41C0
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
	sex R2
	cretn
  ENDIF
  
  IFNDEF TMC600
_monochrome:		; set video to mono if mono=1, set to color if 0
  IFDEF  PECOM 
	ldireg R8, 0x7cc4
  ELSE
	ldireg R8, 0x41C0
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
	sex R2
	cretn
  ENDIF
