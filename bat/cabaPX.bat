@set cpu=1802
@set speed=4000000
@set codeloc=0
@set stackloc=0x7EFF
@if not "%2"=="" set cpu=%2
@if not "%3"=="" set speed=%3
@if not "%4"=="" set codeloc=%4
@if not "%5"=="" set stackloc=%5
@echo compile %1.c for cpu %cpu% speed %speed% codeloc %codeloc% stackloc %stackloc%

@c:\lcc42\bin\lcc.exe "-target=xr18CX" "-Wf-volatile" "-Wf-g,;" -DRES=32 -DVIDMEM=0x7F00 -D__VIP__ -S %6 %7 %8 %1.c 
@IF ERRORLEVEL 1 GOTO quit
@c:\lcc42\bin\copt c:\lcc42\include\lcc1806.opt -v -I %1.asm -O %1.oasm
@c:\lcc42\bin\asw -cpu %cpu% -quiet -P -x -i c:\lcc42\include -D CPUSPEED=%speed% -D CODELOC=%codeloc% -D STACKLOC=%stackloc% -D LCCPX=1 %1.oasm
@c:\Python27\python c:\lcc42\bat\brfixPX.py %1.i
@c:\lcc42\bin\asw -cpu %cpu%  -quiet -x -i c:\lcc42\include -L -D CPUSPEED=%speed% -D CODELOC=%codeloc% -D STACKLOC=%stackloc% -D LCCPX=1 %1.basm
@c:\lcc42\bin\p2hex -r $-$ %1 %9 | more +2
@:quit
