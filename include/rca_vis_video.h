#ifndef _RCA_VIS_VIDEO_H
#define _RCA_VIS_VIDEO_H

//rca_vis_video header
void setvideobase(unsigned short int vidmem);
void vidchar(unsigned short int vidmem, unsigned char character);
void vidstrcpy(unsigned short int vidmem, char * text);
void vidclr(unsigned int vidmem, int vidlen);
void shapechar(const unsigned char * shapelocation, int number);
void shapecolor(unsigned short int character, unsigned char number, unsigned char color);
unsigned char bgcolor(unsigned char color);
void textcolordefinition(unsigned char definition);
void monochrome(unsigned char mono);
void rca_vis_video_includer(){
asm(" include rca_vis_video.inc\n");
}

#endif // _RCA_VIS_VIDEO_H
