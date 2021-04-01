rmdir c:\users\bill\desktop\release /s /q
mkdir c:\users\bill\desktop\release
mkdir c:\users\bill\desktop\release\bin
mkdir c:\users\bill\desktop\release\bat
mkdir c:\users\bill\desktop\release\include
mkdir c:\users\bill\desktop\release\examples


copy c:\lcc42\bin\*.exe c:\users\bill\desktop\release\bin
copy c:\lcc42\bin\*.msg c:\users\bill\desktop\release\bin
copy c:\lcc42\bat\* c:\users\bill\desktop\release\bat
xcopy c:\lcc42\include\* c:\users\bill\desktop\release\include /S /E
xcopy c:\lcc42\examples\* c:\users\bill\desktop\release\examples /S /E
del c:\users\bill\desktop\release\examples\*.lst /s
del c:\users\bill\desktop\release\examples\*.asm /s
del c:\users\bill\desktop\release\examples\*.n /s
del c:\users\bill\desktop\release\examples\*.i /s
del c:\users\bill\desktop\release\examples\*.hex /s
del c:\users\bill\desktop\release\examples\*.p /s
del c:\users\bill\Desktop\bin\lburg.*


