#ifndef _RCA_RAND_H
#define _RCA_RAND_H

unsigned short int seedvalue;

//rca_rand header
unsigned short int rand();
void rca_rand_includer(){
asm(" include rca_rand.inc\n");
}

#endif // _RCA_RAND_H
