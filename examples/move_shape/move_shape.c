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
	0xff, 
	0x00, 0xdf, 0xec, 0xfa, 0xfa, 0xec, 0xdf, 0x00,  // 1 - ghost
	0x00, 
	0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6,  // 2 - top line
	0x00, 
	0xc6, 0xc7, 0xc7, 0x00, 0x00, 0x00, 0x00, 0x00,  // 3 - top left corner    
	0x00,   
	0x00, 0x00, 0x00, 0x00, 0x00, 0xc7, 0xc7, 0xc6,  // 4 - top right corner
	0x00,
	0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,  // 5 - left line 
	0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00,  // 6 - right line
	0x00, 
	0xd8, 0xf8, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00,  // 7 - bottom left corner 
	0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xf8, 0xd8,  // 8 - bottom right corner 
	0x00, 
	0xd8, 0xd8, 0xd8, 0xd8, 0xd8, 0xd8, 0xd8, 0xd8,  // 9 - bottom line 
};
#else
static const unsigned char shapes[] =
{
	0x00, 0x8c, 0x9e, 0xad, 0xbf, 0xb3, 0xad, 0xa1, 0x00,  // 1 - ghost
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00,  // 2 - top line
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc7, 0xc7, 0xc6,  // 3 - top left corner
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xf8, 0xd8,  // 4 - top right corner
	0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6,  // 5 - left line
	0xd8, 0xd8, 0xd8, 0xd8, 0xd8, 0xd8, 0xd8, 0xd8, 0xd8,  // 6 - right line
	0xc6, 0xc7, 0xc7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 7 - bottom left corner
	0xd8, 0xf8, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 8 - bottom right corner
	0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 9 - bottom line
};
#endif

void main(){
	int x, y;
    unsigned char key;
	unsigned char highcolor;

#if defined __CIDELSA__ 
	highcolor = 0;
#else
	highcolor = 128;
#endif

    disableinterrupt();                     // Disable 1802 interrupt to speed up COMX code slightly
    initvideo();                            // Initialize VIS video output
    setvideobase(0xf800);                   // Set video base address to 0xF800, needed for vidstrcpy routines

    vidclr(0xf800, X_SIZE*Y_SIZE);          // Clear screen from start (0xF800) with length of number of characters on screen
 
	shapechar(shapes, 1, 9);

	vidcharxy (0, 0, 3);
	vidcharxy (0, Y_SIZE-1, 7);
	for (x=1; x<(X_SIZE-1); x++){
		vidcharxy (x, 0, 2);
		vidcharxy (x, Y_SIZE-1, 9);
	}
	vidcharxy (X_SIZE-1, 0, 4);
	vidcharxy (X_SIZE-1, Y_SIZE-1, 8);
	for (y=1; y<(Y_SIZE-1); y++){
		vidcharxy (0, y, 5);
		vidcharxy (X_SIZE-1, y, 6);
	}

	x = (int) (X_SIZE/2);
    y = (int) (Y_SIZE/2);

	while (1){
        key = get_stick();
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
        vidcharxy(x, y, 1+highcolor);
    }
}
