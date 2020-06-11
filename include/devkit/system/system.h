#ifndef _SYSTEM_H
#define _SYSTEM_H

//system header
void disableinterrupt();
void enableinterrupt();
void system_includer(){
asm(" include devkit/system/system.inc\n");
}

#endif // _SYSTEM_H
