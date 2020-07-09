/*
    move_shape example by Marcel van Tongoren 
*/

#include <stdint.h>

#include "devkit/video/vis_video.h"
#include "devkit/input/joystick.h"
#include "devkit/system/system.h"

#ifdef __CIDELSA__
static const unsigned char shapes[] =
{
	0x00,                                            
	0x00, 0x9f, 0xac, 0xba, 0xba, 0xac, 0x9f, 0x00,  // 1 - ghost
	0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,  // 2 - right line
	0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc7, 0xc7,  // 3 - top right corner
	0x00,   
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xf8,  // 4 - bottom right corner
	0x00,
	0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6,  // 5 - top line
	0x00,
	0xd8, 0xd8, 0xd8, 0xd8, 0xd8, 0xd8, 0xd8, 0xd8,  // 6 - bottum line
	0x00, 
	0xc7, 0xc7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 7 - top left corner
	0x00, 
	0xf8, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 8 - bottom left corner
	0x00, 
	0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00   // 9 - left line
};
#else
#if NTSC!=0 || PAL==1 // use slightly shifted lines on NTSC (Microboard and COMX) systems where characters are 6x8 instead of 6x9
                      // do the same on Microboard PAL1 as it uses a 9th line which is always blank
static const unsigned char shapes[] =
{
	0x00, 0x8c, 0x9e, 0xad, 0xbf, 0xb3, 0xad, 0xa1, 0x00,  // 1 - ghost
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00,  // 2 - top line
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc7, 0xc7, 0x00,  // 3 - top left corner
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xf8, 0x00,  // 4 - top right corner
	0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x00,  // 5 - left line
	0xd8, 0xd8, 0xd8, 0xd8, 0xd8, 0xd8, 0xd8, 0xd8, 0x00,  // 6 - right line
	0xc6, 0xc7, 0xc7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 7 - bottom left corner
	0xd8, 0xf8, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 8 - bottom right corner
	0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00   // 9 - bottom line
};
#else
static const unsigned char shapes[] =
{
	0x00, 0x8c, 0x9e, 0xad, 0xbf, 0xb3, 0xad, 0xa1, 0x00,  // 1 - ghost
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,  // 2 - top line
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc7, 0xc7,  // 3 - top left corner
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xf8,  // 4 - top right corner
	0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6,  // 5 - left line
	0xd8, 0xd8, 0xd8, 0xd8, 0xd8, 0xd8, 0xd8, 0xd8, 0xd8,  // 6 - right line
	0xc7, 0xc7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 7 - bottom left corner
	0xf8, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 8 - bottom right corner
	0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00   // 9 - bottom line
};
#endif
#endif

#if defined __TMC600__ // TMC doesn't have changeable characters so we are using some predefined characters
#define GHOST 242
#define TOP_LINE 176
#define TOP_LEFT 160
#define TOP_RIGHT 144
#define LEFT_LINE 170
#define RIGHT_LINE 149
#define BOT_LEFT 130
#define BOT_RIGHT 129
#define BOT_LINE 131
#else
#if NTSC==5 || NTSC==6 || NTSC==7 // Microboard NTSC 5, 6, 7 don't have changeable characters
#define GHOST 106
#define TOP_LINE 45
#define TOP_LEFT 45
#define TOP_RIGHT 45
#define LEFT_LINE 33
#define RIGHT_LINE 33
#define BOT_LEFT 45
#define BOT_RIGHT 45
#define BOT_LINE 45
#else                             // Folowing characters will be 'shaped' in the code below with 'shapechar'
#define GHOST 1
#define TOP_LINE 2
#define TOP_LEFT 3
#define TOP_RIGHT 4
#define LEFT_LINE 5
#define RIGHT_LINE 6
#define BOT_LEFT 7
#define BOT_RIGHT 8
#define BOT_LINE 9
#endif
#endif

void main(){
	int x, y;
    unsigned char key;
	unsigned short int vidmem;

    disableinterrupt();                     // Disable 1802 interrupt to speed up COMX code slightly
    initvideo();                            // Initialize VIS video output
    setvideobase(0xf800);                   // Set video base address to 0xF800, needed for vidstrcpy routines

    vidclr(0xf800, X_SIZE*Y_SIZE);          // Clear screen from start (0xF800) with length of number of characters on screen
 
	shapechar(shapes, 1, 9);                // Shape 9 characters as defined above in 'shapes'

	vidchar (0xf800, TOP_LEFT);             // Top left corner is always 0xF800 except on CIDELSA where it is TOP RIGHT (90 degress
	vidchar (0xf827, TOP_RIGHT);            // rotated). But shapes will also be 90 degrees rotated which makes this work on all systems.
	for (vidmem=0xf801; vidmem<0xf827; vidmem++){
		vidchar (vidmem, TOP_LINE);
	}
	for (vidmem=LAST_POS_PMEM-38; vidmem<LAST_POS_PMEM; vidmem++){
		vidchar (vidmem, BOT_LINE);
	}
	vidchar (LAST_POS_PMEM-39, BOT_LEFT);
	vidchar (LAST_POS_PMEM, BOT_RIGHT);
	for (vidmem=0xf828; vidmem<LAST_POS_PMEM-39; vidmem+=40){
		vidchar (vidmem, LEFT_LINE);
		vidchar (vidmem+39, RIGHT_LINE);
	}

	x = (int) (X_SIZE/2);
    y = (int) (Y_SIZE/2);                   // Set x and y to middle of screen

#if defined __TMC600__
	setcolor(0xf829, 1);                    // Only on TMC600 set the ghost color on green, vidmem can be anything as long as it is
#endif                                      // not on the border lines.

	while (1){
        key = get_stick();                  // get direction key value
        switch (key){
            case MOVE_UP:
				if (y != 1)
				{
					vidcharxy(x, y,' ');
					y--;
				}
            break;
            case MOVE_RIGHT:
				if (x != (X_SIZE - 2))
				{
					vidcharxy(x, y,' ');
					x++;
				}
            break;
            case MOVE_LEFT:
				if (x != 1)
				{
					vidcharxy(x, y,' ');
					x--;
				}
            break;
            case MOVE_DOWN:
 				if (y != (Y_SIZE - 2))
				{
					vidcharxy(x, y,' ');
	               y++;
				}
            break;
            default:
            break;            
        }
        vidcharxy(x, y, GHOST);             // Output ghost on the screen, we do this on every loop otherwise some might not get
    }                                       // printed due to the fact that vidchar/vidcharxy need to be used regularly for speed
}                                           // optimization
