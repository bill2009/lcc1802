/*
    terminal example by Marcel van Tongoren 
*/

#include <stdint.h>
#include <nstdlib.h>

#define PIXIE_SPRITE

#include "devkit/video/pixie_video.h"
#include "devkit/input/joystick.h"

static const uint8_t shape_o[] =
{
	RES/8, 
	0x60, 0x90, 0x90, 0x60, 0x60, 0x90, 0x90, 0x60, 0x60, 0x90, 0x90, 0x60, 0x60, 0x90, 0x90, 0x60
};

void main(){
	int x, y, delay;
    unsigned char key;
	int lines = RES/8;

	initvideo();

	x = (int) ((X_SIZE-4)/2);
    y = (int) ((Y_SIZE-lines)/2);                   // Set x and y to middle of screen

	drawsprite (x, y, shape_o);

	while (1) {
		key = get_stick();                  // get direction key value
        switch (key){
            case MOVE_UP:
				if (y != 0)
				{
					drawsprite (x, y, shape_o);
					y--;
					drawsprite (x, y, shape_o);
				}
            break;
            case MOVE_RIGHT:
				if (x != (X_SIZE - 4))
				{
					drawsprite (x, y, shape_o);
					x++;
					drawsprite (x, y, shape_o);
				}
            break;
            case MOVE_LEFT:
				if (x != 0)
				{
					drawsprite (x, y, shape_o);
					x--;
					drawsprite (x, y, shape_o);
				}
            break;
            case MOVE_DOWN:
 				if (y != (Y_SIZE - lines))
				{
					drawsprite (x, y, shape_o);
	                y++;
					drawsprite (x, y, shape_o);
				}
            break;
            default:
            break;            
        }
		if (key != 0)
		{
			delay = 20;
			while (delay > 0)
				delay--;
		}
	}
}
