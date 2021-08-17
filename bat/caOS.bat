
@echo compile %1.c for ELFOS
@c:\lcc42\bin\lcc.exe "-target=xr18CX" "-Wf-g,;" "-Wf-elfos" -S %2 %3 %4 %5 %6 %1.c 
@IF ERRORLEVEL 1 GOTO quit
@c:\lcc42\bin\copt c:\lcc42\include\lcc1806.opt -I %1.asm -O %1.oasm
@c:\lcc42\bin\asw -cpu 1802 -quiet -D LCCELFOS=1 -D CODELOC=0x2000 -D STACKLOC=0x7FFF  -D LCCNOIO -P -x -i c:\lcc42\include -L %1.oasm
@c:\lcc42\bin\p2hex -r $-$ %1 %1 
@c:\lcc42\bin\p2bin -r $-$ %1 %1
@:quit

@rem c:\lcc42\bin\lcc.exe "-target=xr18CX" "-Wf-g,;"  "-Wf-volatile"  "-Wa -D LCCELFOS" "-Wa -D CODELOC=0x2000"  "-Wa -D STACKLOC=0x7Fff" %2 %3 %4 %5 %6 %1.c 
