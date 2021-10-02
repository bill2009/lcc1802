# lcc1802
An ANSI C Compiler for the RCA 1802 family of processors<br />
Installation on linux - command line examples use pi as the userid - tested on raspbian<br />
-switch to the root directory: 
    cd /<br />
-make a directory for lcc1802:  
    sudo mkdir lcc1802<br />
-give yourself ownership of it: 
    sudo chown lcc1802 pi<br />
-clone the repository: 
    git clone https://github.com/bill2009/lcc1802.git<br />
-switch to the directory: 
    cd lcc1802<br />
-ls will show you directories examples with sample programs, include with the header files, bat with script and batch files, and lcc42 with the source code<br />
-switch to the source directory: 
    cd lcc42<br />
-make the build directory: 
    mkdir build<br />
-make the project: 
    make BUILDDIR=build all<br />
-add links to the executables to /usr/bin/:
    sudo ln -sf /lcc1802/lcc42/build/lcc /usr/bin/lcc and sudo ln -sf /lcc1802/lcc42/build/rcc /usr/bin/rcc<br />
-make the assembler trampoline executable:
    chmod +x /lcc1802/ash.sh<br />

-at this point the compiler is installed and can be used to compile C to assembler :
    lcc -S tst/8q.c     then  cat 8q.s<br />

-you now need the assembler asl: you can download the latest c source from http://john.ccac.rwth-aachen.de:8000/as/download.html<br />
-the build instructions are a bit lengthy but everything worked as advertised

-at this point the compiler and assembler can be used to prepare hex files for execution: 
    lcc tst/8q.c<br />
-view the output: cat 8q.hex  and  more <8q.lst
    
