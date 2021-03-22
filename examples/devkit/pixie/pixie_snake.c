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
#include "devkit/system/rand.h"

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
	int x[31], y[31], x_end, y_end, startx, starty, direction = 0, collision, front, end, i, delay, numberOfItems;
    unsigned char key = 0, oldkey = 0;
	uint32_t sprite_data[31];
	uint8_t * vidmem;

	initvideo();

	while (key == 0)
	{
		key = get_stick(); 
		numberOfItems = rand()&0xf+6;
	}

//	vidmem = (uint8_t *)(VIDMEM);
//	*vidmem = 0xff;

//	for (i=0; i<numberOfItems; i++)
//	{
//		vidmem = (uint8_t *)(VIDMEM + (rand()&0xff));
//		*vidmem = 1;
//	}

/*	vidmem = (uint8_t *)VIDMEM;
	for (i=0; i<8; i++)
		*vidmem++ = 0xff;
	for (i=0; i<(RES-2); i++)
	{
		*vidmem = *vidmem | 0x80;
		vidmem = (uint8_t *)vidmem + 7;
		*vidmem++ = 0x01;
	}
	for (i=0; i<8; i++)
		*vidmem++ = 0xff;*/

	startx=(int) ((X_SIZE-1)/2);
	starty=(int) ((Y_SIZE-1)/2);
	for (i=0; i<size; i++)
	{
		x[i] = startx;
		y[i] = starty;
		collision = showsprite (&sprite_data[i], shape_o, x[i], y[i]);
		starty++;
	}

	collision = 0;
	front = 0;
	end = size - 1;

	while (1) {
		key = get_stick();                  // get direction key value

		if (key != 0 && key != oldkey)
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
		oldkey = key;

		if (direction_table[direction] == MOVE_UP)
		{
			x_end = x[end];
			y_end = y[end];
			x[end] = x[front];
			y[end] = y[front] - 1;
			collision = movexysprite (&sprite_data[end], x[end], y[end]);
			front = end;
			end--;
			if (end == -1)  end = size - 1;
		}
		if (direction_table[direction] == MOVE_RIGHT)
		{
			x_end = x[end];
			y_end = y[end];
			x[end] = x[front] + 1;
			y[end] = y[front];
			collision = movexysprite (&sprite_data[end], x[end], y[end]);
			front = end;
			end--;
			if (end == -1)  end = size - 1;
		}
 		if (direction_table[direction] == MOVE_LEFT)
		{
			x_end = x[end];
			y_end = y[end];
			x[end] = x[front] - 1;
			y[end] = y[front];
			collision = movexysprite (&sprite_data[end], x[end], y[end]);
			front = end;
			end--;
			if (end == -1)  end = size - 1;
		}
 		if (direction_table[direction] == MOVE_DOWN)
		{
			x_end = x[end];
			y_end = y[end];
			x[end] = x[front];
			y[end] = y[front] + 1;
			collision = movexysprite (&sprite_data[end], x[end], y[end]);
			front = end;
			end--;
			if (end == -1)  end = size - 1;
		}
		if (collision == 1 && size < 30)
		{
			collision = showsprite (&sprite_data[front], shape_o, x[front], y[front]);
			for (i=size; i>front; i--)
			{
				sprite_data[i] = sprite_data[i-1];
				x[i] = x[i-1];
				y[i] = y[i-1];
			}
			x[front] = x_end;
			y[front] = y_end;
			collision = showsprite (&sprite_data[front], shape_o, x[front], y[front]);
			front++;
			end++;
			size++;
			collision = 0;
			
		//	while(1)
		//	{
		//	}
		}

		delay = 100;
		while (delay > 0)
			delay--;

	}
}
