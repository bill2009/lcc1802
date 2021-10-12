# LCC1802

An ANSI C Compiler for the RCA 1802 family of processors

## Description

A full C compiler for the 1802 based on the LCC retargetable C compiler by Fraser and Hanson

## Getting Started

### Dependencies

LCC1802 requires the asl cross assembler developed by Alfred Arnold 

### Installing for windows
* The releases contain the windows binaries for the compiler and assembler.  the whole directory should be placed at c:/lcc42/
* the bat c:/lcc42/lcc42/file buildvisualstudio.bat will compile lcc1802 from the source

### Installing for linux

command line examples use pi as the userid - tested on raspbian
* switch to the root directory: 
    cd /<br />
* make a directory for lcc1802:  sudo mkdir lcc1802<br />
* give yourself ownership of it: 
    sudo chown lcc1802 pi<br />
* clone the repository: 
    git clone https://github.com/bill2009/lcc1802.git<br />
* switch to the directory: 
    cd lcc1802<br />
* ls will show you directories examples/ with sample programs, include/ with the header files, bat/ with script and batch files, and lcc42/ with the source code<br />
* switch to the source directory: 
    cd lcc42<br />
* make the build directory: 
    mkdir build<br />
* make the project: 
    make BUILDDIR=build all<br />
* add links to the executables to /usr/bin/:
```
sudo ln -sf /lcc1802/lcc42/build/lcc /usr/bin/lcc
sudo ln -sf /lcc1802/lcc42/build/rcc /usr/bin/rcc
sudo ln -sf /lcc1802/lcc42/build/copt /usr/bin/copt
```    
* at this point the compiler is installed and can be used to compile C to assembler :
```
lcc -S tst/8q.c
cat 8q.s
```
* you now need the assembler asl: you can download the latest c source from http://john.ccac.rwth-aachen.de:8000/as/download.html
  For Raspbian it was just a matter of running make and make install

* at this point the compiler and assembler can be used to prepare hex files for execution: 
```
    lcc tst/8q.c
    cat 8q.hex
    more <8q.lst
```    
* because of the way the assembler parses its parameters, they have to be passed differently in linux. 
  For example, "-Wa-D CODELOC=0x2000" becomes "-Wa-D" "-WaCODELOC=0x2000"
  However, multiple symbols can still be combined as "-Wa-D" "-WaCODELOC=0x2000,STACKLOC=7fff"
## Acknowledgments

* the original lcc is at https://github.com/drh/lcc
* [awesome-readme](https://github.com/matiassingers/awesome-readme)
