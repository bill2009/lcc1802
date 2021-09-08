set BUILDDIR=c:/lcc42/bin
pushd .
call "C:/Program Files (x86)\Microsoft Visual Studio/2019/Community/VC/Auxiliary/Build/vcvars32.bat"
popd
nmake /A -f makefile.nt all
rem nmake /G /G /P -f makefile.nt HOSTFILE=etc/win32.c lcc 

rem   to compile "ops" gcc  -Isrc etc\ops.c
rem to run it ops c=1 s=2 i=2 l=4 h=4 f=4 d=8 x=8 p=2

pause