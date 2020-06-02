#ifndef _RCA_VIS_VIDEO_H
#define _RCA_VIS_VIDEO_H

unsigned char vis_text_color;
unsigned long vis_buffer;
#ifdef __CIDELSA__
unsigned char vis_out3_value;
#endif

//rca_vis_video header

void setvideobase(unsigned short int vidmem);
void vidclr(unsigned int vidmem, int vidlen);
void vidchar(unsigned short int vidmem, unsigned char character);
void vidstrcpy(unsigned short int vidmem, char * text);
unsigned char bgcolor(unsigned char color);
void textcolor(unsigned char color);
#ifdef __TMC600__
void setcolor(unsigned int colormem, unsigned char color);
#else
#ifdef __CIDELSA__
void shapechar(const unsigned char * shapelocation, unsigned short int number, unsigned char color);
#else
void shapechar(const unsigned char * shapelocation, int number);
#endif
void shapecolor(unsigned short int character, unsigned char number, unsigned char color);
void textcolordefinition(unsigned char definition);
void monochrome(unsigned char mono);
#endif
void rca_vis_video_includer(){
asm(" include comx/rca_vis_video.inc\n");
}

#endif // _RCA_VIS_VIDEO_H
