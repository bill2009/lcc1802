
@echo compile %1.c with liveness optimization for olduino

@c:\lcc42\bin\lcc.exe "-target=xr18CX" "-Wf-g,;"  -S %2 %3 %4 %5 %6 %1.c 
@IF ERRORLEVEL 1 GOTO quit
@c:\lcc42\bin\copt c:\lcc42\include\lcc1806.opt -I %1.asm -O %1.oasm
c:\apps\pypy\pypy3 c:\lcc42\examples\liveness\liveness9.py %1
c:\lcc42\bin\copt c:\lcc42\include\liveness.opt -I %1.lasm -O %1.olasm
@c:\lcc42\bin\asw -cpu 1802 -quiet  -P -x -i ..\..\include -L %1.olasm
@c:\lcc42\bin\p2hex -r $-$ %1 a | more +2
@echo VS without liveness analysis
@copy %1.lst %1.ollist
@c:\lcc42\bin\asw -cpu 1802 -quiet  -P -x -i ..\..\include -L %1.oasm
@c:\lcc42\bin\p2hex -r $-$ %1 NUL | more +2
@:quit
