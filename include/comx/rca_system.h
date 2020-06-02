#ifndef _RCA_SYSTEM_H
#define _RCA_SYSTEM_H

//rca_system header
void disableinterrupt();
void enableinterrupt();
void rca_system_includer(){
asm(" include comx/rca_system.inc\n");
}

#endif // _RCA_SYSTEM_H
