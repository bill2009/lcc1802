#ifndef _RCA_VIS_SOUND_H
#define _RCA_VIS_SOUND_H

//rca_vis_sound header
void generatetone(unsigned char tone, unsigned char range, unsigned char volume);
void generatenoise(unsigned char range, unsigned char volume);
void rca_vis_sound_includer(){
asm(" include rca_vis_sound.inc\n");
}

#endif // _RCA_VIS_SOUND_H
