#ifndef _VIS_CHAR_H
#define _VIS_CHAR_H

#ifdef __CIDELSA__
#include "devkit/video/cidelsa_char.h"
#include <devkit/video/cidelsa_char.c>
#endif

#if defined __COMX__ || defined __PECOM__ || defined __MICRO__

#ifndef __NTSC5_6_7__
#include "devkit/video/comx_char.h"
#include <devkit/video/comx_char.c>
#else
#define character_set(number)
#endif

#endif

#ifdef __TMC600__
#define character_set(number)
#endif

#endif // _VIS_CHAR_H
