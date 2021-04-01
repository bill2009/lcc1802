@call lcc  %1
@camel1802-2\1802simwjr a >%1.cmp
@diff %1.cmp baseline/%1.cmp
@if ERRORLEVEL 1 goto quit
@del a.hex %1.lst %1.i %1.asm %1.oasm %1.p %1.cmp
:quit