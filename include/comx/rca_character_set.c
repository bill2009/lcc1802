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

#include "comx\rca_character_set.h"
#include "comx\rca_vis_video.h"

#ifdef __CIDELSA__
void character_set(unsigned char number)
{
	number--;
	switch (number&3)
	{
		case 0:
			vis_text_color_mask = 0;
			shapechar(shape_space, 0x0001, 0x0);
			shapechar(shape_space, 0x2001, 0x0);
			shapechar(shape_numbers, 0x300A, RCA_NUMBER_COLOR1);
			shapechar(shape_alpha, 0x411A, RCA_ALPHA_COLOR1);
		break;

		case 1:
			vis_text_color_mask = 0x80;
			shapechar(shape_space, 0x0001, 0x0);
			shapechar(shape_space, 0x2001, 0x0);
			shapechar(shape_numbers, 0x300A, RCA_NUMBER_COLOR1);
			shapechar(shape_alpha, 0x411A, RCA_ALPHA_COLOR1);
			shapechar(shape_space, 0x8001, 0x0);
			shapechar(shape_space, 0xA001, 0x0);
			shapechar(shape_numbers, 0xB00A, RCA_NUMBER_COLOR2);
			shapechar(shape_alpha, 0xC11A, RCA_ALPHA_COLOR2);
		break;

		case 2:
		case 3:
			vis_text_color_mask = 0xC0;
			shapechar(shape_space, 0x0001, 0x0);
			shapechar(shape_space, 0x2001, 0x0);
			shapechar(shape_numbers, 0x300A, RCA_NUMBER_COLOR1);
			shapechar(shape_alpha, 0x411A, RCA_ALPHA_COLOR1);
			shapechar(shape_space, 0x4001, 0x0);
			shapechar(shape_space, 0x6001, 0x0);
			shapechar(shape_numbers, 0x700A, RCA_NUMBER_COLOR2);
			shapechar(shape_alpha, 0x811A, RCA_ALPHA_COLOR2);
			shapechar(shape_space, 0x8001, 0x0);
			shapechar(shape_space, 0xA001, 0x0);
			shapechar(shape_numbers, 0xB00A, RCA_NUMBER_COLOR3);
			shapechar(shape_alpha, 0xC11A, RCA_ALPHA_COLOR3);
			shapechar(shape_space, 0xC001, 0x0);
			shapechar(shape_space, 0xE001, 0x0);
			shapechar(shape_numbers, 0xF00A, RCA_NUMBER_COLOR4);
			shapechar(shape_alpha, 0x011A, RCA_ALPHA_COLOR4);
		break;
	}
}
#endif
