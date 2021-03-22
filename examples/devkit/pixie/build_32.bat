@echo compile %1.c for VIP + 32 lines

@c:\lcc42\bin\lcc.exe "-target=xr18CX" "-Wf-g,;" "-Wf-volatile" "-Wf-pixie2" -O "-Wp-D nofloats" "-Wa-D LCCNOLONG" -DRES=32 -DVIDMEM=0x3F00 -D__VIP__ -S %2 %3 %4 %5 %6 %1.c 
@IF ERRORLEVEL 1 GOTO quit
@c:\lcc42\bin\copt c:\lcc42\include\lcc1806.opt -I %1.asm -O %1.oasm
@c:\lcc42\bin\asw -cpu 1802 -quiet -D LCCPX=2 -D CODELOC=0 -D STACKLOC=0x3EFF -P -x -i ..\..\..\include -L %1.oasm
@c:\lcc42\bin\p2hex -r $-$ %1 %1_32 | more +2
@:quit

