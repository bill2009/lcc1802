;testing output to the boyd LED segments
	relaxed on
	req
	seq
	sex 0
	out 7
	db  0xff
	req
	
	out 7
	db  d
	out 7
	db y
	out 7
	db  o
	out 7
	db  b
	out 7
	db  L
	out 7
	db  L
	out 7
	db  one
	out 7
	db  b

	br  $
A	equ 0b01111110 ;A
b	equ 0b00011111 ;b
C	equ 0b01001011 ;C
d	equ 0b00111101 ;d
i	equ 0b00010000 ;i
L	equ 0b00001011 ;L
o	equ 0b00011101 ;o
y	equ 0b00110111 ;y
zero	equ 0b01111011 ;0
one	equ 0b00110000 ;1
two	equ 0b01101101 ;2
eight	equ 0b01111111 ;8
sp	equ 0

