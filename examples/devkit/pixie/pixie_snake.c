/*
    Pixie sprite move example by Marcel van Tongoren 
*/

#include <stdint.h>
#include <nstdlib.h>

#define PIXIE_PATTERN
#define SPRITE_WIDTH 1
#define SPRITE_HEIGHT 1

#include "devkit/video/pixie_video.h"
#include "devkit/input/joystick.h"

#if RES==32
static const uint8_t shape_o[] =
{
	1, 
	0x80, 0x00, 
	0x40, 0x00, 
	0x20, 0x00, 
	0x10, 0x00, 
	0x08, 0x00, 
	0x04, 0x00, 
	0x02, 0x00, 
	0x01, 0x00,
};
#endif

#if RES==64
static const uint8_t shape_o[] =
{
	1, 
	0x80, 0x00, 
	0x40, 0x00, 
	0x20, 0x00, 
	0x10, 0x00, 
	0x08, 0x00, 
	0x04, 0x00, 
	0x02, 0x00, 
	0x01, 0x00,
};
#endif

#if RES==128
static const uint8_t shape_o[] =
{
	1, 
	0x80, 0x00, 
	0x40, 0x00, 
	0x20, 0x00, 
	0x10, 0x00, 
	0x08, 0x00, 
	0x04, 0x00, 
	0x02, 0x00, 
	0x01, 0x00,
};
#endif

void main(){
	int x[6], y[6], delay, collision, front, end;
    unsigned char key;
	uint32_t sprite_data[6];

	initvideo();

	x[0] = (int) ((X_SIZE-1)/2);
    y[0] = (int) ((Y_SIZE-1)/2);                   // Set x and y to middle of screen
	x[1] = x[0];
	y[1] = y[0]+1;
	x[2] = x[0];
	y[2] = y[0]+2;
	x[3] = x[0];
	y[3] = y[0]+3;
	x[4] = x[0];
	y[4] = y[0]+4;
	x[5] = x[0];
	y[5] = y[0]+5;

	collision = showsprite (&sprite_data[0], shape_o, x[0], y[0]);
	collision = showsprite (&sprite_data[1], shape_o, x[1], y[1]);
	collision = showsprite (&sprite_data[2], shape_o, x[2], y[2]);
	collision = showsprite (&sprite_data[3], shape_o, x[3], y[3]);
	collision = showsprite (&sprite_data[4], shape_o, x[4], y[4]);
	collision = showsprite (&sprite_data[5], shape_o, x[5], y[5]);
	collision = 0;
	front = 0;
	end = 5;

	while (1) {
		key = get_stick();                  // get direction key value

		if (key == MOVE_UP)
		{
			if (y[front] != 0)
			{
				removesprite (&sprite_data[end]);
				x[end] = x[front];
				y[end] = y[front] - 1;
				collision = showsprite (&sprite_data[end], shape_o, x[end], y[end]);
				front = end;
				end--;
				if (end == -1)  end = 5;
			}
		}
		if (key == MOVE_RIGHT)
		{
			if (x[front] != (X_SIZE - 1))
			{
				removesprite (&sprite_data[end]);
				x[end] = x[front] + 1;
				y[end] = y[front];
				collision = showsprite (&sprite_data[end], shape_o, x[end], y[end]);
				front = end;
				end--;
				if (end == -1)  end = 5;
			}
		}
 		if (key == MOVE_LEFT)
		{
			if (x[front] != 0)
			{
				removesprite (&sprite_data[end]);
				x[end] = x[front] - 1;
				y[end] = y[front];
				collision = showsprite (&sprite_data[end], shape_o, x[end], y[end]);
				front = end;
				end--;
				if (end == -1)  end = 5;
			}
		}
 		if (key == MOVE_DOWN)
		{
			if (y[front] != (Y_SIZE - 1))
			{
				removesprite (&sprite_data[end]);
				x[end] = x[front];
				y[end] = y[front] + 1;
				collision = showsprite (&sprite_data[end], shape_o, x[end], y[end]);
				front = end;
				end--;
				if (end == -1)  end = 5;
			}
		}

//		while (collision == 1)
//		{
//		}
	}
}
