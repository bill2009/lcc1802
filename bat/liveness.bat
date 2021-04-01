
c:\apps\pypy\pypy3 c:\lcc42\examples\liveness\liveness9.py %1
c:\lcc42\bin\copt c:\lcc42\include\liveness.opt -v -I %1.lasm -O %1.olasm
rem @rem c:\lcc42\bin\asw -cpu 1802 -quiet  -P -x -i ..\..\include -L %1.olasm
rem @rem c:\lcc42\bin\p2hex -r $-$ %1 a | more +2
rem @:quit
