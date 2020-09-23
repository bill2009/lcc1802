#ifndef _STRLEN_H
#define _STRLEN_H

//strlen header

unsigned int strlen1802(char *str);
void strlen_includer(){
asm(" include devkit/system/strlen.inc\n");
}

#endif // _STRLEN_H
