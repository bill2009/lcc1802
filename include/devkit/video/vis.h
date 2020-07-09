#ifndef _VIS_H
#define _VIS_H

//vis header

#include "devkit/system/flags.h"

#if defined __CIDELSA__ || defined __MICRO__
unsigned short int vis_cursor_position;
unsigned short int vis_video_base;
#endif
#if defined __TMC600__
unsigned short int vis_video_base;
#endif

unsigned short int vis_column_counter;

#define COLOR_BLACK 0
#define COLOR_GREEN 1    
#define COLOR_BLUE 2
#define COLOR_CYAN 3     
#define COLOR_RED 4
#define COLOR_YELLOW 5
#define COLOR_MAGENTA 6
#define COLOR_WHITE 7  

#ifdef __CIDELSA__
#define Y_SIZE 40

#ifdef __DRACO__
#define TOP_LEFT_CORNER 0xfc10
#define LAST_POS_PMEM 0xfc37
#define X_SIZE 27
#else
#define TOP_LEFT_CORNER 0xfbc0
#define LAST_POS_PMEM 0xfbe7
#define X_SIZE 25
#endif

#else
#define TOP_LEFT_CORNER 0xf800
#define LAST_POS_PMEM 0xfbbf
#define X_SIZE 40
#define Y_SIZE 24
#endif

void vis_includer(){
asm(" include devkit/video/vis.inc\n");
}

#endif // _VIS_H
