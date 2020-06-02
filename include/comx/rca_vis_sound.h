#ifndef _RCA_VIS_SOUND_H
#define _RCA_VIS_SOUND_H

//rca_vis_sound header
void generatetone(unsigned char tone, unsigned char range, unsigned char volume);
// tone is using max 7 bits, range max 3 bits and volume 4 bits
void generatenoise(unsigned char range, unsigned char volume);
// range is using max 3 bits and volume 4 bits
void rca_vis_sound_includer(){
asm(" include comx/rca_vis_sound.inc\n");
}

#endif // _RCA_VIS_SOUND_H
