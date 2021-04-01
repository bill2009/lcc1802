@set cpu=1802
@set speed=4000000
@set codeloc=0
@set stackloc=0x7FFF
@set target=xr18CX
@if not "%2"=="" set cpu=%2
@if not "%3"=="" set speed=%3
@if not "%4"=="" set codeloc=%4
@if not "%5"=="" set stackloc=%5
@if not "%6"=="" set target=%6

@echo compile %1.c for cpu %cpu% speed %speed% codeloc %codeloc% stackloc %stackloc% target %target%

c:\lcc42\bin\lcc.exe "-target=%target%" "-Wf-g,;" -S %7 %1.c 
IF ERRORLEVEL 1 GOTO quit
c:\lcc42\bin\copt c:\lcc42\include\lcc1806.opt -v -I %1.asm -O %1.oasm
c:\lcc42\bin\asw -cpu %cpu%  -x -i ..\..\include -L -D CPUSPEED=%speed% -D CODELOC=%codeloc% -D STACKLOC=%stackloc% %1.oasm
c:\lcc42\bin\p2hex -r $-$ %1 a | more +2
:quit
