
@echo compile %1.c for symbolic target

@c:\lcc42\bin\lcc.exe "-target=symbolic"  "-Wf-g,;"  "-Wf-volatile" -S "-D __CIDELSA__"  %2 %3 %4 %5 %6 %1.c 
copy %1.asm %1.symbolic

