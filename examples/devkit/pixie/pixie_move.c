/*
    Pixie sprite move example by Marcel van Tongoren 
*/

#include <stdint.h>
#include <nstdlib.h>

#define PIXIE_MOVE
#define PIXIE_CHECK_BORDER

#include "devkit/video/pixie_video.h"
#include "devkit/input/joystick.h"

static const uint8_t shape_o[] =
{
	RES/8, 
	0x60, 0x90, 0x90, 0x60, 0x60, 0x90, 0x90, 0x60, 0x60, 0x90, 0x90, 0x60, 0x60, 0x90, 0x90, 0x60
};

static const uint8_t shape_x[] =
{
	RES/8, 
	0x90, 0x60, 0x60, 0x90, 0x90, 0x60, 0x60, 0x90, 0x90, 0x60, 0x60, 0x90, 0x90, 0x60, 0x60, 0x90
};

void main(){
	int x, y, x2, y2, delay, collision1, collision2;
    unsigned char key;
	int lines = RES/8;
	uint32_t sprite_data1;
	uint32_t sprite_data2;

	initvideo();

	x = (int) ((X_SIZE-4)/2);
    y = (int) ((Y_SIZE-lines)/2);                   // Set x and y to middle of screen
	x2 = x+8;
	y2 = y;

	collision1 = showsprite (&sprite_data1, shape_o, x, y);
	collision2 = showsprite (&sprite_data2, shape_x, x2, y2);
	collision1 = 0;
	collision2 = 0;

	while (1) {
		key = get_stick();                  // get direction key value
		if (key != 0)
		{
			collision1 = movesprite(&sprite_data1, key);
			collision2 = movesprite(&sprite_data2, key);
		}

		if (collision1 == 1)
		{
			removesprite (&sprite_data1);
			x = (int) ((X_SIZE-4)/2);
			y = (int) ((Y_SIZE-lines)/2);                   // Set x and y to middle of screen
			collision1 = showsprite (&sprite_data1, shape_o, x, y);
		}

		if (collision2 == 1)
		{
			removesprite (&sprite_data2);
			x2 = (int) ((X_SIZE-4)/2) + 8;
			y2 = (int) ((Y_SIZE-lines)/2);                   // Set x and y to middle of screen
			collision2 = showsprite (&sprite_data2, shape_x, x2, y);
		}
	}
}
