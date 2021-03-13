#ifndef _FLAGS_H
#define _FLAGS_H

#define xstr(s) str(s)
#define str(s) #s

//flags header

#if defined PRINTF_ROM
#if defined __COMX__ || defined __TMC600__ || defined __PECOM__
void printf_rom_includer(){
asm("PRINTF_ROM equ 1\n");
}
#endif
#endif

// Machine definitions

#if defined __COMX__
void flags_comx_includer(){
asm("COMX equ 1\n");
}
#endif

#if defined __TMC600__
void flags_tmc600_includer(){
asm("TMC600 equ 1\n");
}
#endif

#if defined __PECOM__
void flags_pecom_includer(){
asm("PECOM equ 1\n");
}
#endif

#if defined __CIDELSA__
void flags_cidelsa_includer(){
asm("CIDELSA equ 1\n");
}
#endif

#if defined __MICRO__
void flags_micro_includer(){
asm("MICRO equ 1\n");
}
#endif

#if defined __ELF2K__
void flags_elf2k_includer(){
asm("ELF2K equ 1\n");
}
#endif

#if defined __VIP__
void flags_vip_includer(){
asm("VIP equ 1\n");
}
#endif

// CIDELSA Variants

#if defined __DRACO__
void flags_draco_includer(){
asm("DRACO equ 1\n");
}
#endif

#if defined __ALTAIR__
void flags_altair_includer(){
asm("ALTAIR equ 1\n");
}
#endif

#if defined __DESTROYER__
void flags_destroyer_includer(){
asm("DESTROYER equ 1\n");
}
#endif

// MICRO Variants

#if PAL==1
void flags_pal1_includer(){
asm("PAL1 equ 1\n");
}
#endif

#if PAL==2
void flags_pal2_includer(){
asm("PAL2 equ 1\n");
}
#endif

#if NTSC==1 || NTSC==4 || NTSC==8 
void flags_ntsc1_includer(){
asm("NTSC1_4_8 equ 1\n");
}
#endif

#if NTSC==2 || NTSC==9
void flags_ntsc2_includer(){
asm("NTSC2_9 equ 1\n");
}
#endif

#if NTSC==3
void flags_ntsc3_includer(){
asm("NTSC3 equ 1\n");
}
#endif

#if NTSC==5 || NTSC==6 || NTSC==7
void flags_ntsc6_includer(){
asm("NTSC5_6_7 equ 1\n");
}
#endif

#ifndef RES
#define RES 32
#endif

#ifndef SPRITE_WIDTH
#define SPRITE_WIDTH 4	// Sprite width to detect reaching right size of screen, default 4
#endif

#if RES==32
#ifndef SPRITE_HEIGHT
#define SPRITE_HEIGHT 4	// Sprite height to detect reaching bottom size of screen, default 4
#endif
void flags_res32_includer(){
asm("RES32 equ 1\n"
	" WIDTH: equ "xstr(SPRITE_WIDTH)"\n"
	" HEIGHT: equ "xstr(SPRITE_HEIGHT)"\n"
	" RES: equ "xstr(RES)"\n");
}
#endif

#if RES==64
#ifndef SPRITE_HEIGHT
#define SPRITE_HEIGHT 8
#endif
void flags_res64_includer(){
asm("RES64 equ 1\n"
	" WIDTH: equ "xstr(SPRITE_WIDTH)"\n"
	" HEIGHT: equ "xstr(SPRITE_HEIGHT)"\n"
	" RES: equ "xstr(RES)"\n");
}
#endif

#if RES==128
#ifndef SPRITE_HEIGHT
#define SPRITE_HEIGHT 16
#endif
void flags_res128_includer(){
asm("RES128 equ 1\n"
	" WIDTH: equ "xstr(SPRITE_WIDTH)"\n"
	" HEIGHT: equ "xstr(SPRITE_HEIGHT)"\n"
	" RES: equ "xstr(RES)"\n");
}
#endif

#endif // _FLAGS_H
