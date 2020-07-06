/*
    character_set example by Marcel van Tongoren 
*/

#include <stdint.h>
#include <nstdlib.h> // needed when using 'printf'

#include "devkit/video/vis_video.h"
#include "devkit/video/vis_char.h"
#include "devkit/video/printf.h"
#include "devkit/input/joystick.h"
#include "devkit/system/system.h"

void main(){
    int i, y;

    disableinterrupt();                     // Disable 1802 interrupt to speed up COMX code slightly
    initvideo();                            // Initialize VIS video output
    setvideobase(0xf800);                   // Set video base address to 0xF800, needed for vidstrcpy routines

    vidclr(0xf800, X_SIZE*Y_SIZE);          // Clear screen from start (0xF800) with length of number of characters on screen
    character_set(4);                       // Shape 4 character sets in 4 differen colors

    textcolor(COLOR_WHITE);                 // Set text color to white
    vidstrcpyxy(0, 0, "VIDSTRCPY EXAMPLE, 4 COLORS");
                                            // Put specified text on position 0, 0 on screen
    y=2;
    for (i=1; i<8; i+=2)
    {
        textcolor(i);
        vidstrcpyxy(0, y, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
        vidstrcpyxy(0, y+1, "!#$%&'()*+,-./0123456789:;<=>?@[]^_");
        y = y+3;
    }

    while (get_stick() == 0)                // Wait for a key press
    {
    }

    vidclr(0xf800, X_SIZE*Y_SIZE);
    character_set(2);                       // Shape 2 character sets in 2 differen colors

    textcolor(COLOR_WHITE);
    vidstrcpyxy(0, 0, "vidstrcpy Example, lowercase & 2 colors");
                                            // Put specified text on position 0, 0 on screen
    for (i=3; i<8; i+=4)
    {
        textcolor(i);
        vidstrcpyxy(0, i-1, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
        vidstrcpyxy(0, i, "abcdefghijklmnopqrstuvwxyz");
        vidstrcpyxy(0, i+1, "!#$%&'()*+,-./0123456789:;<=>?@[]^_");
    }

    while (get_stick() == 0)                // Wait for a key press
    {
    }

    vidclr(0xf800, X_SIZE*Y_SIZE);
    character_set(4);

    textcolor(COLOR_WHITE);
    gotoxy(0, 0);                           // Move cursor position to 0, 0
    printf("PRINTF EXAMPLE, 4 COLORS");     // 'normal' c printf routine using devkit routines for video output

    gotoxy(0, 2);                           // Move cursor position to 0, 2
    for (i=1; i<8; i+=2)
    {
        textcolor(i);
        printf("ABCDEFGHIJKLMNOPQRSTUVWXYZ\n");
        printf("!#$%&'()*+,-./0123456789:;<=>?@[]^_\n\n");
    }

    while (get_stick() == 0)                // Wait for a key press
    {
    }

    vidclr(0xf800, X_SIZE*Y_SIZE);
    character_set(2);

    textcolor(COLOR_WHITE);
    gotoxy(0, 0);
    printf("printf example, lowercase & 2 colors");

    gotoxy(0, 2);
    for (i=3; i<8; i+=4)
    {
        textcolor(i);
        printf("ABCDEFGHIJKLMNOPQRSTUVWXYZ\n");
        printf("abcdefghijklmnopqrstuvwxyz\n");
        printf("!#$%&'()*+,-./0123456789:;<=>?@[]^_\n\n");
    }

    while (get_stick() == 0)                // Wait for a key press
    {
    }
    
    enableinterrupt();
}

#include <nstdlib.c>  // needed when using 'printf'
