
@echo fabrizio's build

@c:\lcc42\bin\lcc.exe -v -target=xr18CX "-Wf-volatile" -O "-Wp-D nofloats" "-Wa-D LCCNOLONG" "-Wa-D CODELOC=0"  "-Wa-D NOFILLBSS=1" "-Wf-mulcall" "-Wa-D DATALOC=0x8000" "-Wa-D STACKLOC=0x83ff" -savetemps  %1.c 
