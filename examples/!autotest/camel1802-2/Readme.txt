
README file for CamelForth for the RCA 1802
===========================================

Harold Rabbie hzrabbie@comcast.net	2009-02-12

This is an implementation of Brad Rodriguez's Camel Forth, ported to the
RCA 1802 8-bit processor. The data sheet describing the instruction set 
of this processor is available at http://www.intersil.com/data/fn/fn1441.pdf

Files included in this archive
------------------------------
RCA 1802 Assembler source code files:

camel18.asm		FORTH primitives and inner interpreter
camel18d.asm		CPU and Model Dependencies
camel18h.asm		High level words

PseudoSam1802.zip	Pseudo-SAM 1802 cross assembler (for MS-DOS)

1802sim.c		C source code for an 1802 CPU simulator (for Cygwin or Linux)
makefile		makefile for Cygwin
Readme.txt		The file you're reading now

Building the CamelForth system
------------------------------
Typing "make" in Cygwin cross-assembles the RCA1802 source code and generates 
two files:

CF1802.OBJ	Intel hex object code
CF1802.LST	Assembly listing

Simulating Forth on the RCA 1802
--------------------------------
"make" also builds the RCA 1802 instruction-level simulator, which allows a Linux or
Cygwin host PC to simulate object code running on RCA 1802 hardware. 

The simulator emulates Forth EMIT and ACCEPT using standard output and standard input.

"make run" runs the simulation of the CamelForth system

The Forth word TRACE controls simulator tracing at run-time:
1 TRACE turns on Forth-level tracing
2 TRACE turns on Instruction-level tracing
0 TRACE turns off all tracing

