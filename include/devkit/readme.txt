1802 DEVKIT===========
********************************************************************** This software is copyright 2020 by Marcel van Tongeren      ****** with support from Bill Rowe and Fabrizio Caruso.            ******                                                             ****** You have permission to use, modify, copy, and distribute    ****** this software so long as this copyright notice is retained. ****** This software may not be used in commercial applications    ****** without express written permission from the author.         **********************************************************************
This devkit should be used with Bill Rowe's LCC1802 (https://sites.google.com/site/lcc1802/) and supports Fabrizio Caruso's CROSS-CHASE (https://github.com/Fabrizio-Caruso/CROSS-CHASE) and cross_lib (https://github.com/Fabrizio-Caruso/CROSS-CHASE/tree/master/src/cross_lib).Content

1. Folder Structure2. Input Routines3. Sound Routines4. System Routines5. Video Routines6. Compiler flags
7. Other header files

1. Folder Structure===================
The devkit folder should reside within lcc42/include/ and consists of:input: for input routines (joystick and keyboard)sound: for sound routinessystem: for system routines (general 1802, basic random etc.)video: for video routines (vis, character definition, printf etc.)2. Input Routines=================unsigned char get_stick()-------------------------Return joystick value currently 'pressed'. If nothing is pressed, 0 is returned.Include file: input/joystick.hDefinitions: MOVE_UP, MOVE_RIGHT, MOVE_LEFT, MOVE_DOWN, MOVE_FIRE Definitions for specific targets: CIDELSA: MOVE_B1, MOVE_B2, CIDELSA/ALTAIR: MOVE_FIRE2, CIDELSA/DRACO: MOVE_TILTTargets implemented: COMX, PECOM, TMC600, CIDELSA, MICROunsigned char cgetc()---------------------Wait for a key on the keyboard to be pressed, return key value.Include file: input/keyboard.hTargets implemented: COMX, PECOM, TMC600, MICROint kbhit()-----------Return 1 if a key is pressed, otherwise return 0.Include file: input/keyboard.hTargets implemented: COMX, PECOM, TMC600, CIDELSA, MICRO3. Sound Routines=================void generatetone(unsigned char tone, unsigned char range, unsigned char volume)--------------------------------------------------------------------------------Generate a tone sound with indicated frequency tone, range and volume.tone: 7 bit valuerange: 3 bit valuevolume: 4 bit valueInclude file: sound/sound.hTargets implemented: COMX, PECOM, TMC600, CIDELSA, MICRO
void generatenoise(unsigned char range, unsigned char volume)-------------------------------------------------------------Generate a noise sound with indicated range and volume.range: 3 bit valuevolume: 4 bit valueInclude file: sound/sound.hTargets implemented: COMX, PECOM, TMC600, CIDELSA, MICRO4. System Routines==================unsigned short int rand()-------------------------Return a random unsigned short value (15 bit, 0-0x7FFF). It is recommended to use this routine instead of a standard crandom routine as this one is optimized for the RCA1802 and as such will be much faster.Note for the TMC600, CIDELSA and MICRO targets this routine will generate a random value in a fixed sequence. Meaning the'random' values will be the same on every clean start. To avoid the same random value being used this routine should becalled a random number of times on start-up, like continuously during a wait for a key press on startup of the program.
Include file: system/rand.hTargets implemented: ALLdisableinterrupt()------------------Disable the RCA1802 interrupt.Recommended to do as early as possible in startup of a program. Disabled interrupt will currently block use of printffeature
Include file: system/system.hTargets implemented: ALLenableinterrupt()-----------------Enable the RCA1802 interrupt.Recommended to do at the end of a program before exiting back to BASIC on the COMX, PECOM and TMC600 targets.Recommended not to be used on other targets unless interrupt handling is defined and R1 contains location of the interrupt routine.Include file: system/system.hTargets implemented: ALL5. Video Routines=================void initvideo()----------------Initialize VIS video output and system settings, should be done at start of a program before using any of the VIS routines.Following will be done:- reset devkit vis character output buffer- set devkit color mask to 0x7f (COMX, PECOM, CIDELSA, MICRO targets), 0x3f (MICRO NTSC5, 6 & 7 targets)- set devkit default text color to CYAN (COMX, PECOM, CIDELSA, MICRO targets)- set VIS out 3 to 0xE0 (COMX, PECOM, CIDELSA, MICRO targets), 0x80 (TMC600 target)- set VIS out 5 to 0x80 (COMX PAL, PECOM, TMC600, MICRO PAL targets), 0xc8 (CIDELSA target), 0x88 (COMX NTSC, MICRO NTSC targets)Include file: video/vis_video.hTargets implemented: COMX, PECOM, TMC600, CIDELSA, MICROvoid setvideobase(unsigned short int vidmem)--------------------------------------------Set VIS video base address to value of vidmem. All targets (except CIDELSA) use the base address value as top left corner of 
the screen. CIDELSA uses base address on top right corner and basically has a standard screen rotated by 90 degrees.On startup of a program base address is recommended be set to 0xF800. This will result in 0xF800 being the top left corner of 
the screen (for COMX PAL, PECOM, TMC600, MICRO targets), 0xFC10 (for CIDELSA/DRACO target), 
0xFBC0 (for CIDELSA/ALTAR, CIDELSA/DESTROYER targets).The video base address can also be used for HW scrolling, i.e. setvideobase(0xF828) would scroll one line (0x28/40 characters) 
compared to setvideobase(0xF800). Not all 16 bits are used see VIS datasheet.Include file: video/vis_video.hDefinitions: TOP_LEFT_CORNERTargets implemented: COMX, PECOM, TMC600, CIDELSA, MICROvoid vidclr(unsigned int vidmem, int vidlen)--------------------------------------------Clear screen from location specified in vidmem (top left corner is 0xf800, see setvideobase) with length of number of 
characters in vidlen.Include file: video/vis_video.hDefinitions: X_SIZE, Y_SIZETargets implemented: COMX, PECOM, TMC600, CIDELSA, MICROvoid vidchar(unsigned int vidmem, unsigned char character)----------------------------------------------------------Output specified character to video on position specified in vidmem (top left corner is 0xf800, see setvideobase). This function does not support text color features as described below for textcolor()/character_set() except for the TMC600target.Include file: video/vis_video.hTargets implemented: COMX, PECOM, TMC600, CIDELSA, MICROvoid vidcharxy(unsigned char x, unsignedchar y, unsigned char character)------------------------------------------------------------------------Output specified character to video on position specified in x, y (top left corner is 0,0). This function does not support text color features as described below for textcolor()/character_set() except for the TMC600target.Include file: video/vis_video.hTargets implemented: COMX, PECOM, TMC600, CIDELSA, MICROvoid vidstrcpy(unsigned int vidmem, char * text)------------------------------------------------Output text string to video on position specified in vidmem (top left corner is 0xf800, see setvideobase). This functionsupports text color setting via textcolor()/character_set() functions described below.Include file: video/vis_video.hTargets implemented: COMX, PECOM, TMC600, CIDELSA, MICROvoid vidstrcpyxy(unsigned char x, unsignedchar y, char * text)--------------------------------------------------------------Output text string to video on position specified in x, y (top left corner is 0,0). This functionsupports text color setting via textcolor()/character_set() functions described below.Include file: video/vis_video.hTargets implemented: COMX, PECOM, TMC600, CIDELSA, MICROvoid textcolor(unsigned char color)-----------------------------------Set textcolor to specified color, for all targets (except TMC600, MICRO/NTSC2, 6, 8 & 9) max 4 colors can be used COLOR_CYAN, COLOR_WHITE, COLOR_GREEN, COLOR_YELLOW. TMC600 also supports COLOR_BLACK, COLOR_BLUE, COLOR_RED, COLOR_MAGENTA,. MICRO/NTSC2, 6, 8 & 9 use 4 colors: COLOR_CYAN, COLOR_GREEN, COLOR_BLACK, COLOR_BLUE.For all targets (except TMC600) it is needed to define the number charactet sets (and as such colors) via character_set(x). See also character_set() definition below. So only after a character_set(4) 4 colors will be available.Include file: video/vis_video.hDefinitions: COLOR_BLACK, COLOR_GREEN, COLOR_BLUE, COLOR_CYAN, COLOR_RED, COLOR_YELLOW, COLOR_MAGENTA, COLOR_WHITETargets implemented: COMX, PECOM, TMC600, CIDELSA, MICROvoid character_set(unsigned char number)----------------------------------------Define the number of character sets as specified in number. Number can be 1, 2 or 4 (3 will result in 4 sets). Each setconsists of 64 or 90 characters (symbols, numbers, capitals and lower case, for CIDELSA only numbers and capitals) and isdefined in a specific color, see textcolor() above. Note that any shapechar() done before will be overwritten by character_set(), so it is recommended to call character_set at start of a program before any shapechar().

Numbers 1 will define 90 characters, i.e. symbols, numbers, capitals and lower case: 
- Character 32-95 and 97-122

Numbers 2 will define 90 characters, i.e. symbols, numbers, capitals and lower case: 
- Color 1: character 32-95 and 97-122 
- Color 2: character 160-223 and 225-250

Number 4 will define 64 characters, i.e. symbols, numbers and capitals:
- Color 1: character 0-63 
- Color 2: character 64-127
- Color 3:  character 128-191
- Color 4:  character 192-255
Include file: video/vis_char.hTargets implemented: COMX, PECOM, CIDELSA, MICROunsigned char bgcolor(unsigned char color)
------------------------------------------

Set background to color specified in color, all targets support 8 colors.

Include file: video/vis_video.hDefinitions: COLOR_BLACK, COLOR_GREEN, COLOR_BLUE, COLOR_CYAN, COLOR_RED, COLOR_YELLOW, COLOR_MAGENTA, COLOR_WHITETargets implemented: COMX, PECOM, TMC600, CIDELSA, MICRO
void setcolor(unsigned int colormem, unsigned char color)
---------------------------------------------------------

Set color for position defined in colormem to specified color (top left corner position is 0).

Include file: video/vis_char.hTargets implemented: TMC600

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

Include file: video/vis_video.hTargets implemented: COMX, PECOM, CIDELSA, MICRO (except NTSC5, 6 & 7)

void shapecolor(unsigned short int character, unsigned char number, unsigned char color)----------------------------------------------------------------------------------------

Change color of charcter, parameters:

character: first character to change color
number: number of characters to change color
color: new character color (0 = BLACK/RED, 1 = BLUE/MAGENTA, 2 = GREEN/YELLOW, 3 = CYAN/WHYTE).

Include file: video/vis_video.hTargets implemented: COMX, PECOM, CIDELSA, MICRO (except NTSC5, 6 & 7)

void textcolordefinition(unsigned char definition)
--------------------------------------------------

Change text color definition, for details see VIS data sheet

  b1	b0	RED	BLUE	GREEN
  0	 0	CB0	CC1	PCB
  0	 1  	CCB0 	PCB	CCB1
  1     0/1 	PCB	CCB0	CCB1

Include file: video/vis_video.hTargets implemented: COMX, PECOM, CIDELSA, MICRO

void monochrome(unsigned char mono)
-----------------------------------

Change color to monochrome when mono = 1, set to color if mono = 0.
Include file: video/vis_video.hTargets implemented: COMX, PECOM, CIDELSA, MICRO
void putlcccx(unsigned char c)
------------------------------

Used to replace putc(c) to use printf feature on VIS targets.
Note, default devkit printf routines will be used, taking some RAM space but these routines are fasted and support the text color
commands described above. To use the ROM print routines in the COMX, PECOM or TMC600 specify compiler flag PRINTF_ROM or add '#define PRINTF_ROM' in your source before the first devkit include file.

Include file: video/printf.hTargets implemented: COMX, PECOM, TMC600, MICRO, CIDELSA

void gotoxy(unsigned char x, unsigned char y)
---------------------------------------------

Change cursor position for putc() and printf() to specified x and y.
Include file: video/printf.hTargets implemented: COMX, PECOM, TMC600, MICRO, CIDELSA


6. Compiler flags
=================

Any build for the RCA1802 using the devkit should specify "-target=xr18CX" for the lcc compiler as well as one of the targets
listed below. 

For the COMX, PECOM and TMC600 the assembler should use -D LCCCX=1. Additionally a CODELOC and for some targets a STACKLOC and/or 
DATALOC should be used as listed below. CODELOC defines where in memory the target code should start, STACKLOC where in RAM the stack pointer should start and DATALOC where in RAM global variables should be placed.

COMX target
-----------

lcc flag: -D__COMX__
asw flas: -D LCCCX=1 -D CODELOC=0x4401

PECOM target
------------

lcc flag: -D__PECOM__
asw flas: -D LCCCX=1 -D CODELOC=0x201

TMC600 target
-------------

lcc flag: -D__TMC600__
asw flas: -D LCCCX=1 -D CODELOC=0x6300

CIDELSA DRACO target
--------------------

lcc flag: -D __CIDELSA__ -D__DRACO__
asw flas 16K target: -D NOFILLBSS=1 -D DATALOC=0x4000 -D CODELOC=0 -D STACKLOC=0x40ff 
asw flas 32K target: -D NOFILLBSS=1 -D DATALOC=0x8000 -D CODELOC=0 -D STACKLOC=0x83ff 

CIDELSA ALTAIR target
---------------------

lcc flag: -D __CIDELSA__ -D__ALTAIR__
asw flas 12K target: -D NOFILLBSS=1 -D DATALOC=0x3000 -D CODELOC=0 -D STACKLOC=0x30ff 
asw flas 24K target: -D NOFILLBSS=1 -D DATALOC=0x6000 -D CODELOC=0 -D STACKLOC=0x60ff 

CIDELSA DESTROYER target
------------------------

lcc flag: -D __CIDELSA__ -D__DESTROYER__
asw flas 8K target: -D NOFILLBSS=1 -D DATALOC=0x2000 -D CODELOC=0 -D STACKLOC=0x20ff 
asw flas 12K target: -D NOFILLBSS=1 -D DATALOC=0x3000 -D CODELOC=0 -D STACKLOC=0x30ff 

MCIRO targets
-------------

Microboard System have different memory configurations. The following example would be used for a 32K ROM at 0-0x7fff (with the 
devkit code) and a 8K RAM at 0x8000-0x9fff. Other memory configurations will need a different CODELOC, DATALOC and STACKLOC. 

asw flas: -D NOFILLBSS=1 -D DATALOC=0x8000 -D CODELOC=0 -D STACKLOC=0x9fff  

For a PAL1 variant the lcc compiler would need:
lcc flag: -D __MICRO__ -D__PAL1__

List flags for MICRO variants

__PAL1__
PAL 1 - 128 6x8 Characters - 1 KB character RAM

__PAL2__
PAL 2 - 64 6x16 Characters - 1 KB character RAM

__ NTSC1_4_8__
1 - 128 6x8 Characters - 1 KB character RAM
4 - 128 6x8 Characters - 1 KB character RAM/ROM
8 - 256 6x8 Characters - 2 KB character RAM/ROM

__NTSC3__
3 - 128 6x16 Characters - 2 KB character RAM

__ NTSC2_9__
2 - 256 6x8 Characters - 2 KB character RAM
9 - 256 6x16 Characters - 4 KB character RAM/ROM

__NTSC5_6_7__
5 - 128 6x8 Characters - 1 KB character ROM
6 - 256 6x8 Characters - 2 KB character ROM
7 - 128 6x16 Characters - 2 KB character ROM


7. Other files
==============

video/comx_char.h & .c: files containing COMX character set shapes used for character_set function (COMX, PECOM, MICRO targets)
video/cidelsa_char.h & .c: files containing CIDELSA character set shapes used for character_set function (CIDELSA target)
video/vis.h & .c: assembler macro routines
system/basic_final.inc: assembler definition file to add BASIC program (COMX, PECOM, TMC600 target)
system/flags.h: include file to copy c compiler flags to the assembler compiler.
 