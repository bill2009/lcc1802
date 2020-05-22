#ifndef _RCA_VIS_VIDEO_H
#define _RCA_VIS_VIDEO_H

//rca_vis_video header
void setvideobase(unsigned short int vidmem);
void vidclr(unsigned int vidmem, int vidlen);
void vidchar(unsigned short int vidmem, unsigned char character);
void vidstrcpy(unsigned short int vidmem, char * text);
unsigned char bgcolor(unsigned char color);
#ifdef __TMC600__
void textcolor(unsigned char color);
void setcolor(unsigned int colormem, unsigned char color);
#else
void shapechar(const unsigned char * shapelocation, int number);
void shapecolor(unsigned short int character, unsigned char number, unsigned char color);
void textcolordefinition(unsigned char definition);
void monochrome(unsigned char mono);
#endif
void rca_vis_video_includer(){
asm(" include comx/rca_vis_video.inc\n");
}

#endif // _RCA_VIS_VIDEO_H
