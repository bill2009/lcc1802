rem 21-4-13 changed directory for visual studio 19
set BUILDDIR=c:\lcc42\bin
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars32.bat"cl /FeC:\lcc42\bin\copt.exe copt.c
pause