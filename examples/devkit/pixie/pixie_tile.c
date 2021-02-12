/*
    terminal example by Marcel van Tongoren 
*/

#include <stdint.h>
#include <nstdlib.h>

#define PIXIE_TILE

#include "devkit/video/pixie_video.h"
#include "devkit/input/joystick.h"

static const uint8_t shape_o[] =
{
	0x66, 0x99, 0x99, 0x66, 0x66, 0x99, 0x99, 0x66, 0x66, 0x99, 0x99, 0x66, 0x66, 0x99, 0x99, 0x66
};

static const uint8_t shape_space[] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void main(){
	int x, y, delay;
    unsigned char key;

	initvideo();

	x = (int) (X_SIZE/2);
    y = (int) (Y_SIZE/2);                   // Set x and y to middle of screen

	drawtile (x, y, shape_o);

	while (1) {
		key = get_stick();                  // get direction key value
        switch (key){
            case MOVE_UP:
				if (y != 0)
				{
					drawtile (x, y, shape_space);
					y--;
				}
            break;
            case MOVE_RIGHT:
				if (x != (X_SIZE - 1))
				{
					drawtile (x, y, shape_space);
					x++;
				}
            break;
            case MOVE_LEFT:
				if (x != 0)
				{
					drawtile (x, y, shape_space);
					x--;
				}
            break;
            case MOVE_DOWN:
 				if (y != (Y_SIZE - 1))
				{
					drawtile (x, y, shape_space);
	                y++;
				}
            break;
            default:
            break;            
        }
		drawtile (x, y, shape_o);
		if (key != 0)
		{
			delay = 500;
			while (delay > 0)
				delay--;
		}
	}
}
