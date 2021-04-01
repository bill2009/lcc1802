
all:	CF1802.OBJ 1802sim.exe

CF1802.OBJ:	camel18h.asm camel18d.asm camel18.asm
	cat camel18.asm camel18d.asm camel18h.asm > CF1802.ASM; cmd /c a18.com CF1802

1802sim.exe:	1802sim.c
	gcc -o 1802sim.exe -Wall -g 1802sim.c

run:	1802sim.exe CF1802.OBJ
	./1802sim.exe CF1802

trace:	1802sim.exe CF1802.OBJ
	./1802sim.exe -f CF1802

clean:
	rm -f CF1802.* 1802sim.exe
