# lcc1802
An ANSI C Compiler for the RCA 1802 family of processors
Installation on linux - command line examples use pi as the userid - tested on raspbian
-switch to the root directory: 
    cd /
-make a directory for lcc1802:  
    sudo mkdir lcc1802
-give yourself ownership of it: 
    sudo chown lcc1802 pi
-clone the repository: 
    git clone https://github.com/bill2009/lcc1802.git
-switch to the directory: 
    cd lcc1802
-ls will show you directories examples with sample programs, include with the header files, bat with script and batch files, and lcc42 with the source code
-switch to the source directory: 
    cd lcc42
-make the build directory: 
    mkdir build
-make the project: 
    make BUILDDIR=build all
-add links to the executables to /usr/bin/
    sudo ln -sf /lcc1802/lcc42/build/lcc /usr/bin/lcc
    sudo ln -sf /lcc1802/lcc42/build/rcc /usr/bin/rcc
-make the assembler trampoline executable:
    chmod +x /lcc1802/bat/ash.sh

-at this point the compiler is installed and can be used to compile C to assembler 
    lcc -S tst/8q.c
    cat 8q.s

-you now need the assembler asl: you can download the latest c source from http://john.ccac.rwth-aachen.de:8000/as/download.html
-the build instructions are a bit lengthy but everything worked as advertised

-at this point the compiler and assembler can be used to prepare hex files for execution 
    lcc tst/8q.c
    cat 8q.hex
    more <8q.lst
    
