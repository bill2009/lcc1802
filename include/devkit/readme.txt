1802 DEVKIT
*******************************************************************
This devkit should be used with Bill Rowe's LCC1802 (https://sites.google.com/site/lcc1802/) and supports Fabrizio Caruso's 

1. Folder Structure
7. Other header files


The devkit folder should reside within lcc42/include/ and consists of:
void generatenoise(unsigned char range, unsigned char volume)
Include file: system/rand.h

the screen. CIDELSA uses base address on top right corner and basically has a standard screen rotated by 90 degrees.
the screen (for COMX PAL, PECOM, TMC600, MICRO targets), 0xFC10 (for CIDELSA/DRACO target), 
0xFBC0 (for CIDELSA/ALTAR, CIDELSA/DESTROYER targets).
compared to setvideobase(0xF800). Not all 16 bits are used see VIS datasheet.
characters in vidlen.
------------------------------------------

Set background to color specified in color, all targets support 8 colors.

Include file: video/vis_video.h

---------------------------------------------------------

Set color for position defined in colormem to specified color (top left corner position is 0).

Include file: video/vis_char.h

void shapechar(const unsigned char * shapelocation, unsigned short int lines_character, unsigned short int color_number)
------------------------------------------------------------------------------------------------------------------------

Change shape of a character, parameters:

shapelocation: pointer to an array of bytes, 9 per character. For all targets (except CIDELSA) one byte per horizontal 
line where the first 2 bits (bit 6 & 7) of every byte are used to identify one out of 4 colors and the remaining 6 bits 
define the pixels used. For the CIDELSA the first byte is used for the 3rd color bit, one bit per character line and the 
2nd to 9th byte for every vertical line per character.

lines_character: 16 bit value of which the lower byte represent the first character to be changed. The higher byte indicate 
the number of lines per character, 0 = 9 lines and 9 bytes per character in the shapelocation, 7 = 7 lines and 7 bytes per 
character in the shapelocation. In the case of 7 lines the first and last line will be shaped with a 0, this is used for
Shaping characters out of the PECOM ROM.

color_number: 16 bit value of which the lower byte represents the number of characters to be changed and included on the
shapelocation. The higher byte indicate the color, if 0 color from the shapelocation bytes will be used if not 0 this byte
will be used as color definition for the complete charcater (0xC0 = CYAN/WHITE, 0x80 = GREEN/YELLOW, 0x40 = BLUE/MAGENTA, 
0x10 = BLACK/RED).

Include file: video/vis_video.h

void shapecolor(unsigned short int character, unsigned char number, unsigned char color)

Change color of charcter, parameters:

character: first character to change color
number: number of characters to change color
color: new character color (0 = BLACK/RED, 1 = BLUE/MAGENTA, 2 = GREEN/YELLOW, 3 = CYAN/WHYTE).

Include file: video/vis_video.h

void textcolordefinition(unsigned char definition)
--------------------------------------------------

Change text color definition, for details see VIS data sheet

  b1	b0	RED	BLUE	GREEN
  0	 0	CB0	CC1	PCB
  0	 1  	CCB0 	PCB	CCB1
  1     0/1 	PCB	CCB0	CCB1

Include file: video/vis_video.h

void monochrome(unsigned char mono)
-----------------------------------

Change color to monochrome when mono = 1, set to color if mono = 0.


------------------------------

Used to replace putc(c) to use printf feature on VIS targets.

Include file: video/printf.h

void gotoxy(unsigned char x, unsigned char y)
---------------------------------------------

Change cursor position for putc() and printf() to specified x and y.



6. Compiler flags
=================

Flags starting and ending with __ are used for the lcc compiler, without for assembler.

__COMX__ / COMX=1: Build COMX target
__PECOM__ / PECOM=1: Build PECOM target
__TMC600__ / TMC600=1: Build TMC600 target
__CIDELSA__ / CIDELSA=1: Build CIDELSA target, note at CIDELSA variant should also be specified
__MICRO__ / MICRO=1: Build MICROBOARD target, default MICRO variant PAL1/NTSC3 will be build 

CIDELSA variants
__ALTAIR__ / ALTAIR=1
__DESTROYER__ / DESTROYER=1
__DRACO__ / DRACO=1

MICRO variants:

Default, i.e. no flag needed
PAL 2 - 64 6x16 Characters - 1 KB character RAM
NTSC 3 - 128 6x16 Characters - 2 KB character RAM

Flags:
__PAL1__ / PAL1
PAL 1 - 128 6x8 Characters - 1 KB character RAM

__ NTSC1_4_8__ / NTSC1_4_8
1 - 128 6x8 Characters - 1 KB character RAM
4 - 128 6x8 Characters - 1 KB character RAM/ROM
8 - 256 6x8 Characters - 2 KB character RAM/ROM

__ NTSC2_9__ / NTSC2_9
2 - 256 6x8 Characters - 2 KB character RAM
9 - 256 6x16 Characters - 4 KB character RAM/ROM

__NTSC5_6_7__ / NTSC5_6_7
5 - 128 6x8 Characters - 1 KB character ROM
6 - 256 6x8 Characters - 2 KB character ROM
7 - 128 6x16 Characters - 2 KB character ROM


7. Other files
==============

video/comx_char.h & .c: files containing COMX character set shapes used for character_set function (COMX, PECOM, MICRO targets)
video/cidelsa_char.h & .c: files containing CIDELSA character set shapes used for character_set function (CIDELSA target)
video/vis.h & .c: assembler macro routines
system/basic_final.inc: assembler definition file to add BASIC program (COMX, PECOM, TMC600 target)
 