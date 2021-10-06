;elfos_final.inc builds the 6 byte elfos header
;Thanks to Marcel Tongren for the code pattern in his basic_final.inc
;21-07-10 First version
    
 IFDEF LCCELFOS
$$EOP:		;end of module

    org 0x1FFA	;org back to build elfos header
    dw 0x2000	;module begin address
    dw $$EOP-0x2000
    dw 0x2000
 ENDIF
