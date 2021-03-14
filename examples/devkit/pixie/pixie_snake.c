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

static const direction_table[] =
{
	MOVE_UP,
	MOVE_RIGHT,
	MOVE_DOWN,
	MOVE_LEFT
};

void main(){
    int size=6;
	int x[6], y[6], startx, starty, direction = 0, collision, front, end, i, delay;
    unsigned char key;
	uint32_t sprite_data[6];

	initvideo();

	startx=(int) ((X_SIZE-1)/2);
	starty=(int) ((X_SIZE-1)/2);
	for (i=0; i<size; i++)
	{
		x[i] = startx;
		y[i] = starty;
		collision = showsprite (&sprite_data[i], shape_o, x[i], y[i]);
		starty++;
	}

	collision = 0;
	front = 0;
	end = 5;

	while (1) {
		key = get_stick();                  // get direction key value

		if (key != 0)
		{
			if (key == MOVE_RIGHT)
			{
				direction++;
				if (direction == 4)  direction = 0;
			}
			if (key == MOVE_LEFT)
			{
				direction--;
				if (direction == -1)  direction = 3;
			}
		}

		if (direction_table[direction] == MOVE_UP)
		{
			x[end] = x[front];
			y[end] = y[front] - 1;
			collision = movexysprite (&sprite_data[end], x[end], y[end]);
			front = end;
			end--;
			if (end == -1)  end = 5;
		}
		if (direction_table[direction] == MOVE_RIGHT)
		{
			x[end] = x[front] + 1;
			y[end] = y[front];
			collision = movexysprite (&sprite_data[end], x[end], y[end]);
			front = end;
			end--;
			if (end == -1)  end = 5;
		}
 		if (direction_table[direction] == MOVE_LEFT)
		{
			x[end] = x[front] - 1;
			y[end] = y[front];
			collision = movexysprite (&sprite_data[end], x[end], y[end]);
			front = end;
			end--;
			if (end == -1)  end = 5;
		}
 		if (direction_table[direction] == MOVE_DOWN)
		{
			x[end] = x[front];
			y[end] = y[front] + 1;
			collision = movexysprite (&sprite_data[end], x[end], y[end]);
			front = end;
			end--;
			if (end == -1)  end = 5;
		}
		if (collision == 1)
		{
			while(1)
			{
			}
		}

		delay = 100;
		while (delay > 0)
			delay--;

	}
}
