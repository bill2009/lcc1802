
@echo compile %1.c for COMX

@c:\lcc42\bin\lcc.exe "-target=xr18CX" "-Wf-g,;"  "-Wf-volatile" -D__COMX__ -S %2 %3 %4 %5 %6 %1.c 
@IF ERRORLEVEL 1 GOTO quit
@c:\lcc42\bin\copt c:\lcc42\include\lcc1806.opt -I %1.asm -O %1.oasm
@c:\lcc42\bin\asw -cpu 1802 -quiet -D LCCCX=1 -D CODELOC=0x4401 -D DATALOC=0 -D STACKLOC=0x7F -P -x -i ..\..\include -L %1.oasm
@c:\lcc42\bin\p2hex -r $-$ %1 a | more +2
@:quit
