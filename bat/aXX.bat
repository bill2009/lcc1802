@set cpu=1802
@set speed=4000000
@set codeloc=0
@set stackloc=0x7FFF
@if not "%2"=="" set cpu=%2
@if not "%3"=="" set speed=%3
@if not "%4"=="" set codeloc=%4
@if not "%5"=="" set stackloc=%5
@C:\lcc42\bin\asw -cpu %cpu%   -x -P -i ..\..\include -L -D CPUSPEED=%speed% -D CODELOC=%codeloc% -D STACKLOC=%stackloc% -D DATALOC=0x8000 %1.asm
@rem @c:\lcc42\bin\asw -cpu %cpu% -quiet -P -x -i ..\..\include -D CPUSPEED=%speed% -D CODELOC=%codeloc% -D STACKLOC=%stackloc% %1.oasm
c:\lcc42\bin\p2hex -r $-$ %1.p a | more +2