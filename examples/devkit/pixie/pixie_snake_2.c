/*
    Pixie snake example by Marcel van Tongoren 
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
	0,
	MOVE_UP,
	MOVE_RIGHT,
	MOVE_DOWN,
	MOVE_LEFT
};

#define SNAKE_SIZE 14

uint32_t sprite_data[SNAKE_SIZE];
uint8_t x[SNAKE_SIZE], y[SNAKE_SIZE], startx, starty, direction, collision, front, end, i, delay;
unsigned char key, oldkey;
uint8_t * vidmem;

void move()
{
	collision = movexysprite (&sprite_data[end], x[end], y[end]);
	front = end;
	if (end == 0)  
		end = SNAKE_SIZE - 1;
	else
		end--;
}

void main(){

	initvideo();

	direction = 1;
	key = 0;
	oldkey = 0;

	startx=(int) ((X_SIZE-1)/2);
	starty=(int) ((Y_SIZE-1)/2);

	for (i=0; i<SNAKE_SIZE; i++)
	{
		x[i] = startx;
		y[i] = starty;
		collision = showsprite (&sprite_data[i], shape_o, x[i], y[i]);
		starty++;
	}

	collision = 0;
	front = 0;
	end = SNAKE_SIZE - 1;

	while (1) {
		key = get_stick();                  // get direction key value

		if (key != 0) // && key != oldkey)
		{
			if (key == MOVE_RIGHT)
			{
				direction++;
				if (direction == 5)  direction = 1;
			}
			if (key == MOVE_LEFT)
			{
				direction--;
				if (direction == 0)  direction = 4;
			}
		}
	//	oldkey = key;

		if (direction_table[direction] == MOVE_UP)
		{
			x[end] = x[front];
			y[end] = y[front] - 1;
			move();
		}
		if (direction_table[direction] == MOVE_RIGHT)
		{
			x[end] = x[front] + 1;
			y[end] = y[front];
			move();
		}
 		if (direction_table[direction] == MOVE_LEFT)
		{
			x[end] = x[front] - 1;
			y[end] = y[front];
			move();
		}
 		if (direction_table[direction] == MOVE_DOWN)
		{
			x[end] = x[front];
			y[end] = y[front] + 1;
			move();
		}
		if (collision == 1)
		{
			while (1)
			{
			}
		}

		delay = 100;
		while (delay > 0)
			delay--;

	}
}

