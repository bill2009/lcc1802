; generated by lcc-xr18NW $Version: 5.0 - XR18NW $ on Fri Apr 07 08:14:26 2017

SP:	equ	2 ;stack pointer
memAddr: equ	14
retAddr: equ	6
retVal:	equ	15
regArg1: equ	12
regArg2: equ	13
	listing off
	include lcc1802proloNW.inc
	listing on
_PIN4:
	db 0
_test:
	db 15
	db 15
	db 15
	db 15
	db 5
	db 0
	db 8
	db 1
;$$function start$$ _main
_main:		;framesize=2
;{
;	asm(" req\n seq\n"
 req
 seq
 dec 2
 out 7
 req
 ldad r11,_test
 ldad r10,8
 sex 11
 out 7
 out 7
 out 7
 out 7
 out 7
 out 7
 out 7
 out 7
 br $
;}
L1:
	Cretn

;$$function end$$ _main
	include lcc1802epiloNW.inc
	include IO1802.inc
