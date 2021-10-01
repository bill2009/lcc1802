#!/bin/bash
asl $1 $2 $3 $4 $5 $6 $7

#shell script to invoke the asl assembler which otherwise has problems recognizing its parameters.
# the source form is 
#asl "-cpu 1802", "-i " LCCDIR "include", "-L", /*"-quiet",*/ "", "-o", "$3", "$1", "$2", 0 };