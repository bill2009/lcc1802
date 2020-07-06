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
            shape_rom = (unsigned char *)0x725;
            shapechar(shape_rom, 0x20, CHAR_COLOR1+32);
            shape_rom = (unsigned char *)0x845;
            shapechar(shape_rom, 0x40, CHAR_COLOR1+32);
            shapechar(shapes_comx_lowercase, 0x61, CHAR_COLOR1+26);
#elif __PECOM__
            shape_rom = (unsigned char *)0x8920;
            shapechar(shape_rom, 0x720, CHAR_COLOR1+32);
            shape_rom = (unsigned char *)0x8a00;
            shapechar(shape_rom, 0x740, CHAR_COLOR1+32);
#else
            shapechar(shapes_comx_32, 0x20, CHAR_COLOR1+32);
            shapechar(shapes_comx_0, 0x40, CHAR_COLOR1+32);
            shapechar(shapes_comx_lowercase, 0x61, CHAR_COLOR1+26);
#endif
        break;

        case 1:
            vis_text_color_mask = 0x7f;
#ifdef __COMX__
            shape_rom = (unsigned char *)0x725;
            shapechar(shape_rom, 0x20, CHAR_COLOR1+32);
            shape_rom = (unsigned char *)0x845;
            shapechar(shape_rom, 0x40, CHAR_COLOR1+32);
            shapechar(shapes_comx_lowercase, 0x61, CHAR_COLOR1+26);
#elif __PECOM__
            shape_rom = (unsigned char *)0x8920;
            shapechar(shape_rom, 0x720, CHAR_COLOR1+32);
            shape_rom = (unsigned char *)0x8a00;
            shapechar(shape_rom, 0x740, CHAR_COLOR1+32);
#else
            shapechar(shapes_comx_32, 0x20, CHAR_COLOR1+32);
            shapechar(shapes_comx_0, 0x40, CHAR_COLOR1+32);
            shapechar(shapes_comx_lowercase, 0x61, CHAR_COLOR1+26);
#ifdef __NTSC2_9__
            shapechar(shapes_comx_32, 0xA0, CHAR_COLOR2+32);
            shapechar(shapes_comx_0, 0xC0, CHAR_COLOR2+32);
            shapechar(shapes_comx_lowercase, 0xe1, CHAR_COLOR2+26);
#endif
#endif
        break;

        case 2:
        case 3:
            vis_text_color_mask = 0x3f;
#ifdef __COMX__
            shape_rom = (unsigned char *)0x725;
            shapechar(shape_rom, 0x20, CHAR_COLOR1+32);
            shapechar(shape_rom, 0x60, CHAR_COLOR2+32);
            shape_rom = (unsigned char *)0x845;
            shapechar(shape_rom, 0x00, CHAR_COLOR1+32);
            shapechar(shape_rom, 0x40, CHAR_COLOR2+32);
#elif __PECOM__
            shape_rom = (unsigned char *)0x8920;
            shapechar(shape_rom, 0x720, CHAR_COLOR1+32);
            shapechar(shape_rom, 0x760, CHAR_COLOR2+32);
            shape_rom = (unsigned char *)0x8a00;
            shapechar(shape_rom, 0x700, CHAR_COLOR1+32);
            shapechar(shape_rom, 0x740, CHAR_COLOR2+32);
#else
            shapechar(shapes_comx_32, 0x20, CHAR_COLOR1+32);
            shapechar(shapes_comx_32, 0x60, CHAR_COLOR2+32);
            shapechar(shapes_comx_0, 0x00, CHAR_COLOR1+32);
            shapechar(shapes_comx_0, 0x40, CHAR_COLOR2+32);
#ifdef __NTSC2_9__
            shapechar(shapes_comx_32, 0xA0, CHAR_COLOR3+32);
            shapechar(shapes_comx_32, 0xE0, CHAR_COLOR4+32);
            shapechar(shapes_comx_0, 0x80, CHAR_COLOR3+32);
            shapechar(shapes_comx_0, 0xC0, CHAR_COLOR4+32);
#endif
#endif
        break;
    }

#if defined PRINTF_ROM && defined __COMX__
    shape_rom = (unsigned char *)0x700;
	shapechar(shape_rom, 0, 1);
#endif
}
