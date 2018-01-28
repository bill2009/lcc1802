;testing output to the boyd calculator printer
 ldi 0 ;
 plo 9
 phi 9
dly: 
 dec 9
 ghi 9
 bnz dly
 seq ;activate the printer
 sex 0 ;inline output data
 nop
 nop
 nop
 nop
 nop
 nop
 nop
 nop
 out 1
 db ' '
 nop
 nop
 nop
 nop
 out 1
 db ' '
 nop
 nop
 nop
 nop
 out 1
 db ' '
 nop
 nop
 nop
 nop
 out 1
 db ' '
 nop
 nop
 nop
 nop
 out 1
 db ' '
 nop
 nop
 nop
 nop
 out 1
 db ' '
 nop
 nop
 nop
 nop
 out 1
 db ' '
 nop
 nop
 nop
 nop
 out 1
 db ' '
 nop
 nop
 nop
 nop
 out 1
 db ' '
 nop
 nop
 nop
 nop
 out 1
 db '0'
 nop
 nop
 nop
 nop
 out 1
 db ' '
 nop
 nop
 nop
 nop
 out 1
 db 'C'
 nop
 nop
 nop
 nop
 out 1
 db ' '
 nop
 nop
 req ;deactivate printer?
l: br l;loop