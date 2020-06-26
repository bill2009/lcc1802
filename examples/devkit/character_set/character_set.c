/*
    character_set example by Marcel van Tongoren 
*/

#include <stdint.h>

#include "devkit/video/vis_video.h"
#include "devkit/video/vis_char.h"
#include "devkit/input/joystick.h"
#include "devkit/system/system.h"

void main(){
    disableinterrupt();
    initvideo();
    setvideobase(0xf800);

    vidclr(0xf800, X_SIZE*Y_SIZE);
    character_set(4);

    textcolor(COLOR_WHITE);
    vidstrcpyxy(0, 0, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    vidstrcpyxy(0, 1, "!#$%&'()*+,-./0123456789:;<=>?@[]^_");
    textcolor(COLOR_CYAN);
    vidstrcpyxy(0, 3, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    vidstrcpyxy(0, 4, "!#$%&'()*+,-./0123456789:;<=>?@[]^_");
    textcolor(COLOR_YELLOW);
    vidstrcpyxy(0, 6, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    vidstrcpyxy(0, 7, "!#$%&'()*+,-./0123456789:;<=>?@[]^_");
    textcolor(COLOR_GREEN);
    vidstrcpyxy(0, 9, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    vidstrcpyxy(0, 10, "!#$%&'()*+,-./0123456789:;<=>?@[]^_");

    while (get_stick() == 0)
    {
    }

    enableinterrupt();
}
