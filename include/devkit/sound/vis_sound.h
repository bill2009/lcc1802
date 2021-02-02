#ifndef _VIS_SOUND_H
#define _VIS_SOUND_H

//vis_sound header

#include "devkit/video/vis.h"

void generatetone(uint8_t tone, uint8_t range, uint8_t volume);
// tone is using max 7 bits, range max 3 bits and volume 4 bits
void generatenoise(uint8_t range, uint8_t volume);
// range is using max 3 bits and volume 4 bits
void vis_sound_includer(){
asm(" include devkit/sound/vis_sound.inc\n");
}

#endif // _VIS_SOUND_H
