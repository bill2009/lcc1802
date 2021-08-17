
@echo compile %1.c for PIXIE
@rem "-Wf-noimul"
@c:\lcc42\bin\lcc.exe "-target=xr18CX" "-Wf-g,;"   -S %2 %3 %4 %5 %6 %1.c 
@IF ERRORLEVEL 1 GOTO quit
@c:\lcc42\bin\copt c:\lcc42\include\lcc1806.opt -I %1.asm -O %1.oasm
C:\Users\bill_\Downloads\asl-current\asl-current\asl -cpu 1802 -quiet -P -x -i ..\..\include -L %1.oasm
@c:\lcc42\bin\p2hex -r $-$ %1 a | more +2
@:quit
