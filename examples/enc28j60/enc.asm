; generated by lcc-xr18ng $Version: 2.3 - XR18NG - The Birthday Compiler $ on Tue Feb 26 15:06:42 2013

SP:	equ	2 ;stack pointer
memAddr: equ	14
retAddr: equ	6
retVal:	equ	15
regArg1: equ	12
regArg2: equ	13
	listing off
	include lcc1802ProloNG.inc
	listing on
_enableChip:
	reserve 2
	release 2
	Cretn

_disableChip:
	reserve 2
	release 2
	Cretn

_SetBank:
	reserve 2
	st2 r12,'O',sp,(4); flag1 
	release 2
	Cretn

