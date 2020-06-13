//
// *******************************************************************
// *** This software is copyright 2020 by Marcel van Tongeren      ***
// *** with support from Bill Rowe and Fabrizio Caruso.			   ***
// ***                                                             ***
// *** You have permission to use, modify, copy, and distribute    ***
// *** this software so long as this copyright notice is retained. ***
// *** This software may not be used in commercial applications    ***
// *** without express written permission from the author.         ***
// *******************************************************************
//

#include "devkit/video/comx_char.h"
#include "devkit/video/vis_video.h"

void character_set(unsigned char number)
{
#if defined __COMX__ || defined __PECOM__
	unsigned char *shape_rom;
#endif
	number--;
	switch (number&3)
	{
		case 0:
			vis_text_color_mask = 0xff;
#ifdef __COMX__
			shape_rom = (unsigned char *)0x700;
			shapechar(shape_rom, 0x00, 1);
			shapechar(shape_rom, 0x20, 1);
			shape_rom = (unsigned char *)0x7b5;
			shapechar(shape_rom, 0x30, NUMBER_COLOR1+0xa);
			shape_rom = (unsigned char *)0x84e;
			shapechar(shape_rom, 0x41, ALPHA_COLOR1+0x1a);
#elif __PECOM__
			shape_rom = (unsigned char *)0x8800;
			shapechar(shape_rom, 0x700, 1);
			shapechar(shape_rom, 0x720, 1);
			shape_rom = (unsigned char *)0x8990;
			shapechar(shape_rom, 0x730, NUMBER_COLOR1+0xa);
			shape_rom = (unsigned char *)0x8a07;
			shapechar(shape_rom, 0x741, ALPHA_COLOR1+0x1a);
#else
			shapechar(shape_space, 0x00, 1);
			shapechar(shape_space, 0x20, 1);
			shapechar(shape_numbers, 0x30, NUMBER_COLOR1+0xa);
			shapechar(shape_alpha, 0x41, ALPHA_COLOR1+0x1a);
#endif
		break;

		case 1:
			vis_text_color_mask = 0x7f;
#ifdef __COMX__
			shape_rom = (unsigned char *)0x700;
			shapechar(shape_rom, 0x00, 1);
			shapechar(shape_rom, 0x20, 1);
			shape_rom = (unsigned char *)0x7b5;
			shapechar(shape_rom, 0x30, NUMBER_COLOR1+0xa);
			shape_rom = (unsigned char *)0x84e;
			shapechar(shape_rom, 0x41, ALPHA_COLOR1+0x1a);
#elif __PECOM__
			shape_rom = (unsigned char *)0x8800;
			shapechar(shape_rom, 0x700, 1);
			shapechar(shape_rom, 0x720, 1);
			shape_rom = (unsigned char *)0x8990;
			shapechar(shape_rom, 0x730, NUMBER_COLOR1+0xa);
			shape_rom = (unsigned char *)0x8a07;
			shapechar(shape_rom, 0x741, ALPHA_COLOR1+0x1a);
#else
			shapechar(shape_space, 0x00, 1);
			shapechar(shape_space, 0x20, 1);
			shapechar(shape_numbers, 0x30, NUMBER_COLOR1+0xa);
			shapechar(shape_alpha, 0x41, NUMBER_COLOR1+0x1a);
//			shapechar(shape_space, 0x80, 1);
//			shapechar(shape_space, 0xA0, 1);
//			shapechar(shape_numbers, 0xB0, 0xa);
//			shapechar(shape_alpha, 0xC1, 0x1a);
#endif
		break;

		case 2:
		case 3:
			vis_text_color_mask = 0x3f;
#ifdef __COMX__
			shape_rom = (unsigned char *)0x700;
			shapechar(shape_rom, 0x00, 1);
			shapechar(shape_rom, 0x20, 1);
			shapechar(shape_rom, 0x40, 1);
			shapechar(shape_rom, 0x60, 1);
			shape_rom = (unsigned char *)0x7b5;
			shapechar(shape_rom, 0x30, NUMBER_COLOR1+0xa);
			shapechar(shape_rom, 0x70, NUMBER_COLOR2+0xa);
			shape_rom = (unsigned char *)0x84e;
			shapechar(shape_rom, 0x01, ALPHA_COLOR1+0x1a);
			shapechar(shape_rom, 0x41, ALPHA_COLOR2+0x1a);
#elif __PECOM__
			shape_rom = (unsigned char *)0x8800;
			shapechar(shape_rom, 0x700, 1);
			shapechar(shape_rom, 0x720, 1);
			shapechar(shape_rom, 0x740, 1);
			shapechar(shape_rom, 0x760, 1);
			shape_rom = (unsigned char *)0x8990;
			shapechar(shape_rom, 0x730, NUMBER_COLOR1+0xa);
			shapechar(shape_rom, 0x770, NUMBER_COLOR2+0xa);
			shape_rom = (unsigned char *)0x8a07;
			shapechar(shape_rom, 0x701, ALPHA_COLOR1+0x1a);
			shapechar(shape_rom, 0x741, ALPHA_COLOR2+0x1a);
#else
			shapechar(shape_space, 0x00, 1);
			shapechar(shape_space, 0x20, 1);
			shapechar(shape_alpha, 0x01, ALPHA_COLOR1+0x1a);
			shapechar(shape_numbers, 0x30, NUMBER_COLOR1+0xa);
			shapechar(shape_space, 0x40, 1);
			shapechar(shape_space, 0x60, 1);
			shapechar(shape_alpha, 0x41, ALPHA_COLOR2+0x1a);
			shapechar(shape_numbers, 0x70, NUMBER_COLOR2+0xa);
//			shapechar(shape_alpha, 0x81, 0x1a);
//			shapechar(shape_space, 0x80, 0x0);
//			shapechar(shape_space, 0xA0, 0x0);
//			shapechar(shape_numbers, 0xB0, 0xa);
//			shapechar(shape_alpha, 0xC1, 0x1a);
//			shapechar(shape_space, 0xC0, 0x0);
//			shapechar(shape_space, 0xE0, 0x0);
//			shapechar(shape_numbers, 0xF0, 0xa);
#endif
		break;
	}
}
