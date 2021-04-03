@set cpu=1802
@set speed=1760900
@set codeloc=0
@set stackloc=0xbFF
@if not "%2"=="" set cpu=%2
@if not "%3"=="" set speed=%3
@if not "%4"=="" set codeloc=%4
@if not "%5"=="" set stackloc=%5
@echo compile %1.c for cpu %cpu% speed %speed% codeloc %codeloc% stackloc %stackloc%
c:\lcc42\bin\lcc.exe "-target=xr18CX" -v "-Wf-volatile" "-Wf-pixie2" "-Wf-g,;" -DRES=32 -DVIDMEM=0x0F00 -D__VIP__ -S %6 %7 %8 %1.c 
@IF ERRORLEVEL 1 GOTO quit
@c:\lcc42\bin\copt c:\lcc42\include\lcc1806.opt -v -I %1.asm -O %1.oasm
@rem @c:\lcc42\bin\asw -cpu %cpu% -quiet -P -x -i c:\lcc42\include -D CPUSPEED=%speed% -D CODELOC=%codeloc% -D STACKLOC=%stackloc% -D LCCPX=2 %1.oasm
@rem @c:\Python27\python c:\lcc42\bat\brfixPX.py %1.i
@c:\lcc42\bin\asw -cpu %cpu%  -quiet -x -P -i c:\lcc42\include -L -D CPUSPEED=%speed% -D CODELOC=%codeloc% -D STACKLOC=%stackloc% -D LCCPX=2 %1.oasm
@c:\lcc42\bin\p2hex -r $-$ %1 a | more +2
@:quit
exit 0
@echo compile %1.c for VIP + 32 lines

@c:\lcc42\bin\lcc.exe "-target=xr18CX" "-Wf-g,;" "-Wf-volatile" "-Wf-pixie2" -O "-Wp-D nofloats" "-Wa-D LCCNOLONG" -DRES=32 -DVIDMEM=0x0F00 -D__VIP__ -S %2 %3 %4 %5 %6 %1.c 
@IF ERRORLEVEL 1 GOTO quit
@c:\lcc42\bin\copt c:\lcc42\include\lcc1806.opt -I %1.asm -O %1.oasm
@c:\lcc42\bin\asw -cpu 1802 -quiet -D LCCPX=2 -D CODELOC=0 -D STACKLOC=0xEFF -P -x -i c:\lcc42\include -L %1.oasm
@c:\lcc42\bin\p2hex -r $-$ %1 %1_32 | more +2
