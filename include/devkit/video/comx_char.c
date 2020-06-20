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
	number = number &3;
	switch (number)
	{
		case 0:
			vis_text_color_mask = 0xff;
#ifdef __COMX__
			shape_rom = (unsigned char *)0x700;
			shapechar(shape_rom, 0x00, 1);
			shapechar(shape_rom, 0x20, 1);
			shape_rom = (unsigned char *)0x7b5;
			shapechar(shape_rom, 0x30, CHAR_COLOR1+0xa);
			shape_rom = (unsigned char *)0x84e;
			shapechar(shape_rom, 0x41, CHAR_COLOR1+0x1a);
#elif __PECOM__
			shape_rom = (unsigned char *)0x8800;
			shapechar(shape_rom, 0x700, 1);
			shapechar(shape_rom, 0x720, 1);
			shape_rom = (unsigned char *)0x8990;
			shapechar(shape_rom, 0x730, CHAR_COLOR1+0xa);
			shape_rom = (unsigned char *)0x8a07;
			shapechar(shape_rom, 0x741, CHAR_COLOR1+0x1a);
#else
			shapechar(shape_at, 0x00, CHAR_COLOR1+1);
			shapechar(shape_space, 0x20, 1);
			shapechar(shape_sym1, 0x21, CHAR_COLOR1+15);
			shapechar(shape_numbers, 0x30, CHAR_COLOR1+0xa);
			shapechar(shape_sym2, 0x3a, CHAR_COLOR1+6);
			shapechar(shape_alpha, 0x41, CHAR_COLOR1+0x1a);
			shapechar(shape_sym3, 0x5b, CHAR_COLOR1+5);
#endif
		break;

		case 1:
			vis_text_color_mask = 0x7f;
#ifdef __COMX__
			shape_rom = (unsigned char *)0x700;
			shapechar(shape_rom, 0x00, 1);
			shapechar(shape_rom, 0x20, 1);
			shape_rom = (unsigned char *)0x7b5;
			shapechar(shape_rom, 0x30, CHAR_COLOR1+0xa);
			shape_rom = (unsigned char *)0x84e;
			shapechar(shape_rom, 0x41, CHAR_COLOR1+0x1a);
#elif __PECOM__
			shape_rom = (unsigned char *)0x8800;
			shapechar(shape_rom, 0x700, 1);
			shapechar(shape_rom, 0x720, 1);
			shape_rom = (unsigned char *)0x8990;
			shapechar(shape_rom, 0x730, CHAR_COLOR1+0xa);
			shape_rom = (unsigned char *)0x8a07;
			shapechar(shape_rom, 0x741, CHAR_COLOR1+0x1a);
#else
			shapechar(shape_at, 0x00, CHAR_COLOR1+1);
			shapechar(shape_space, 0x20, 1);
			shapechar(shape_sym1, 0x21, CHAR_COLOR1+15);
			shapechar(shape_numbers, 0x30, CHAR_COLOR1+0xa);
			shapechar(shape_sym2, 0x3a, CHAR_COLOR1+6);
			shapechar(shape_alpha, 0x41, CHAR_COLOR1+0x1a);
			shapechar(shape_sym3, 0x5b, CHAR_COLOR1+5);
#ifdef __NTSC2_9__
			shapechar(shape_at, 0x80, CHAR_COLOR2+1);
			shapechar(shape_space, 0xA0, 1);
			shapechar(shape_sym1, 0xA1, CHAR_COLOR2+15);
			shapechar(shape_numbers, 0xB0, CHAR_COLOR2+0xa);
			shapechar(shape_sym2, 0xBA, CHAR_COLOR2+6);
			shapechar(shape_alpha, 0xC1, CHAR_COLOR2+0x1a);
			shapechar(shape_sym3, 0xDB, CHAR_COLOR2+5);
#endif
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
			shapechar(shape_rom, 0x30, CHAR_COLOR1+0xa);
			shapechar(shape_rom, 0x70, CHAR_COLOR2+0xa);
			shape_rom = (unsigned char *)0x84e;
			shapechar(shape_rom, 0x01, CHAR_COLOR1+0x1a);
			shapechar(shape_rom, 0x41, CHAR_COLOR2+0x1a);
#elif __PECOM__
			shape_rom = (unsigned char *)0x8800;
			shapechar(shape_rom, 0x700, 1);
			shapechar(shape_rom, 0x720, 1);
			shapechar(shape_rom, 0x740, 1);
			shapechar(shape_rom, 0x760, 1);
			shape_rom = (unsigned char *)0x8990;
			shapechar(shape_rom, 0x730, CHAR_COLOR1+0xa);
			shapechar(shape_rom, 0x770, CHAR_COLOR2+0xa);
			shape_rom = (unsigned char *)0x8a07;
			shapechar(shape_rom, 0x701, CHAR_COLOR1+0x1a);
			shapechar(shape_rom, 0x741, CHAR_COLOR2+0x1a);
#else
			shapechar(shape_at, 0x00, CHAR_COLOR1+1);
			shapechar(shape_space, 0x20, 1);
			shapechar(shape_alpha, 0x01, CHAR_COLOR1+0x1a);
			shapechar(shape_sym3, 0x1B, CHAR_COLOR1+5);
			shapechar(shape_sym1, 0x21, CHAR_COLOR1+15);
			shapechar(shape_numbers, 0x30, CHAR_COLOR1+0xa);
			shapechar(shape_sym2, 0x3a, CHAR_COLOR1+6);
			shapechar(shape_at, 0x40, CHAR_COLOR2+1);
			shapechar(shape_space, 0x60, 1);
			shapechar(shape_alpha, 0x41, CHAR_COLOR2+0x1a);
			shapechar(shape_sym3, 0x5B, CHAR_COLOR2+5);
			shapechar(shape_sym1, 0x61, CHAR_COLOR2+15);
			shapechar(shape_numbers, 0x70, CHAR_COLOR2+0xa);
			shapechar(shape_sym2, 0x7a, CHAR_COLOR2+6);
#ifdef __NTSC2_9__
			shapechar(shape_at, 0x80, CHAR_COLOR3+1);
			shapechar(shape_space, 0xA0, 1);
			shapechar(shape_alpha, 0x81, CHAR_COLOR3+0x1a);
			shapechar(shape_sym3, 0x9B, CHAR_COLOR3+5);
			shapechar(shape_sym1, 0xA1, CHAR_COLOR3+15);
			shapechar(shape_numbers, 0xB0, CHAR_COLOR3+0xa);
			shapechar(shape_sym2, 0xBA, CHAR_COLOR3+6);
			shapechar(shape_at, 0xC0, CHAR_COLOR4+1);
			shapechar(shape_space, 0xE0, 1);
			shapechar(shape_alpha, 0xC1, CHAR_COLOR4+0x1a);
			shapechar(shape_sym3, 0xDB, CHAR_COLOR4+5);
			shapechar(shape_sym1, 0xE1, CHAR_COLOR4+15);
			shapechar(shape_numbers, 0xF0, CHAR_COLOR4+0xa);
			shapechar(shape_sym2, 0xFA, CHAR_COLOR4+6);
#endif
#endif
		break;
	}
}
