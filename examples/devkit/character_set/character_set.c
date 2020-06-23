/*
    character_set example by Marcel van Tongoren 
*/

#include <stdint.h>

#include "devkit/video/vis_video.h"
#include "devkit/video/vis_char.h"
#include "devkit/input/joystick.h"
#include "devkit/system/system.h"

uint16_t loc(uint8_t x, uint8_t y)
{
#if !defined(__CIDELSA__)
	return (uint16_t) 0xF800+x+(uint8_t)y*X_SIZE;
#else
	return (uint16_t) 0xF800+X_SIZE*Y_SIZE-Y_SIZE-x*Y_SIZE+(uint8_t)y;
#endif
}

void main(){
	uint16_t vidmem=0xf800;

	disableinterrupt();
	initvideo();
    setvideobase(vidmem);

	vidmem=TOP_LEFT_CORNER;

    vidclr(0xf800, X_SIZE*Y_SIZE);
	character_set(4);

	textcolor(COLOR_WHITE);
	vidstrcpy(loc(0,0), "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	vidstrcpy(loc(0,1), "!#$%&'()*+,-./0123456789:;<=>?@[]^_");
	textcolor(COLOR_CYAN);
	vidstrcpy(loc(0,3), "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	vidstrcpy(loc(0,4), "!#$%&'()*+,-./0123456789:;<=>?@[]^_");
	textcolor(COLOR_YELLOW);
	vidstrcpy(loc(0,6), "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	vidstrcpy(loc(0,7), "!#$%&'()*+,-./0123456789:;<=>?@[]^_");
	textcolor(COLOR_GREEN);
	vidstrcpy(loc(0,9), "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	vidstrcpy(loc(0,10), "!#$%&'()*+,-./0123456789:;<=>?@[]^_");

	while (get_stick() == 0)
	{
	}

	enableinterrupt();
}
