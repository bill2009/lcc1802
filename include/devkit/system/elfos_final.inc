;elfos_final.inc builds the 6 byte elfos header
;Thanks to Marcel Tongren for the code pattern in his basic_final.inc
;21-07-10 First version
;22-01-28 grw - updated to include build information (CODELOC changes to 0x2010) 
    
 IFDEF LCCELFOS
month set SUBSTR(DATE,0,STRSTR(DATE,"/"))
year set SUBSTR(DATE,STRLEN(DATE)-4,4)
month set SUBSTR(DATE,0,STRSTR(DATE,"/"))
chunk set SUBSTR(DATE,STRSTR(DATE,"/")+1,3)
day set SUBSTR(chunk,0,STRSTR(chunk,"/"))  

$$EOP:		;end of module
    org 0x1FFA	;org back to build elfos header
    dw 0x2000	;module begin address
    dw $$EOP-0x2000
    dw 0x2000
    br $$start  ;branch past build info to start of program
    db val(month)+0x80
    db val(day)
    dw val(year)
    dw 1
    db "LCC1802",0
$$start:     
 ENDIF
