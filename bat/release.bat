rmdir c:\Users\bill_\Desktop\release /s /q
mkdir c:\Users\bill_\Desktop\release
mkdir c:\Users\bill_\Desktop\release\bin
mkdir c:\Users\bill_\Desktop\release\bat
mkdir c:\Users\bill_\Desktop\release\include
mkdir c:\Users\bill_\Desktop\release\examples


copy c:\lcc42\bin\*.exe c:\Users\bill_\Desktop\release\bin
copy c:\lcc42\bin\*.msg c:\Users\bill_\Desktop\release\bin
copy c:\lcc42\bat\* c:\Users\bill_\Desktop\release\bat
xcopy c:\lcc42\include\* c:\Users\bill_\Desktop\release\include /S /E
xcopy c:\lcc42\examples\* c:\Users\bill_\Desktop\release\examples /S /E
del c:\Users\bill_\Desktop\release\examples\*.lst /s
del c:\Users\bill_\Desktop\release\examples\*.asm /s
del c:\Users\bill_\Desktop\release\examples\*.n /s
del c:\Users\bill_\Desktop\release\examples\*.i /s
del c:\Users\bill_\Desktop\release\examples\*.hex /s
del c:\Users\bill_\Desktop\release\examples\*.p /s
del c:\Users\bill_\Desktop\bin\lburg.*


