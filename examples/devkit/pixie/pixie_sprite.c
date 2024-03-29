/*
    terminal example by Marcel van Tongoren 
*/

#include <stdint.h>
#include <nstdlib.h>

#define PIXIE_SPRITE 1

#include "devkit/video/pixie_video.h"
#include "devkit/input/joystick.h"

static const uint8_t shape_o[] =
{
	RES/8, 
	0x60, 0x90, 0x90, 0x60, 0x60, 0x90, 0x90, 0x60, 0x60, 0x90, 0x90, 0x60, 0x60, 0x90, 0x90, 0x60
};

uint8_t x, y, delay, collision;
unsigned char key;

void middle(){
	x = (uint8_t) ((X_SIZE-4)/2);
    y = (uint8_t) ((Y_SIZE-RES/8)/2);                   // Set x and y to middle of screen
}

void main(){

	initvideo();

	middle();

	collision = drawsprite (x, y, shape_o);
	collision = 0;

	while (1) {
		key = get_stick();                  // get direction key value

		switch (key){
            case MOVE_UP:
				if (y != 0)
				{
					collision = drawsprite (x, y, shape_o);
					y--;
					collision = drawsprite (x, y, shape_o);
				}
            break;
            case MOVE_RIGHT:
				if (x != (X_SIZE - 4))
				{
					collision = drawsprite (x, y, shape_o);
					x++;
					collision = drawsprite (x, y, shape_o);
				}
            break;
            case MOVE_LEFT:
				if (x != 0)
				{
					collision = drawsprite (x, y, shape_o);
					x--;
					collision = drawsprite (x, y, shape_o);
				}
            break;
            case MOVE_DOWN:
 				if (y != (Y_SIZE - RES/8))
				{
					collision = drawsprite (x, y, shape_o);
	                y++;
					collision = drawsprite (x, y, shape_o);
				}
            break;
            default:
            break;            
        }
		if (collision == 1)
		{
			collision = drawsprite (x, y, shape_o);
			middle();
			collision = drawsprite (x, y, shape_o);
		}
	}
}
