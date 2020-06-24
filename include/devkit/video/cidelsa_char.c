//
// *******************************************************************
// *** This software is copyright 2020 by Marcel van Tongeren      ***
// *** with support from Bill Rowe and Fabrizio Caruso.            ***
// ***                                                             ***
// *** You have permission to use, modify, copy, and distribute    ***
// *** this software so long as this copyright notice is retained. ***
// *** This software may not be used in commercial applications    ***
// *** without express written permission from the author.         ***
// *******************************************************************
//

#include "devkit/video/cidelsa_char.h"
#include "devkit/video/vis_video.h"

#ifdef __CIDELSA__
void character_set(unsigned char number)
{
    number--;
    switch (number&3)
    {
        case 0:
            vis_text_color_mask = 0xff;
            shapechar(shape_space, 0x00, 1);
            shapechar(shape_space, 0x20, 1);
            shapechar(shape_numbers, 0x30, NUMBER_COLOR1+0xa);
            shapechar(shape_alpha, 0x41, ALPHA_COLOR1+0x1a);
        break;

        case 1:
            vis_text_color_mask = 0x7f;
            shapechar(shape_space, 0x00, 1);
            shapechar(shape_space, 0x20, 1);
            shapechar(shape_numbers, 0x30, NUMBER_COLOR1+0xa);
            shapechar(shape_alpha, 0x41, ALPHA_COLOR1+0x1a);
            shapechar(shape_space, 0x80, 1);
            shapechar(shape_space, 0xA0, 1);
            shapechar(shape_numbers, 0xB0, NUMBER_COLOR2+0xa);
            shapechar(shape_alpha, 0xC1, ALPHA_COLOR2+0x1a);
        break;

        case 2:
        case 3:
            vis_text_color_mask = 0x3f;
            shapechar(shape_space, 0x00, 1);
            shapechar(shape_space, 0x20, 1);
            shapechar(shape_alpha, 0x01, ALPHA_COLOR1+0x1a);
            shapechar(shape_numbers, 0x30, NUMBER_COLOR1+0xa);
            shapechar(shape_space, 0x40, 1);
            shapechar(shape_space, 0x60, 1);
            shapechar(shape_alpha, 0x41, ALPHA_COLOR2+0x1a);
            shapechar(shape_numbers, 0x70, NUMBER_COLOR2+0xa);
            shapechar(shape_space, 0x80, 1);
            shapechar(shape_space, 0xA0, 1);
            shapechar(shape_alpha, 0x81, ALPHA_COLOR3+0x1a);
            shapechar(shape_numbers, 0xB0, NUMBER_COLOR3+0xa);
            shapechar(shape_space, 0xC0, 1);
            shapechar(shape_space, 0xE0, 1);
            shapechar(shape_alpha, 0xC1, ALPHA_COLOR4+0x1a);
            shapechar(shape_numbers, 0xF0, NUMBER_COLOR4+0xa);
        break;
    }
}
#endif
