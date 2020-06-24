#ifndef _FLAGS_H
#define _FLAGS_H

//flags header

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

#if defined __PAL1__
void flags_pal1_includer(){
asm("PAL1 equ 1\n");
}
#endif

#if defined __PAL2__
void flags_pal2_includer(){
asm("PAL2 equ 1\n");
}
#endif

#if defined __NTSC1_4_8__
void flags_ntsc1_includer(){
asm("NTSC1_4_8 equ 1\n");
}
#endif

#if defined __NTSC2_9__
void flags_ntsc2_includer(){
asm("NTSC2_9 equ 1\n");
}
#endif

#if defined __NTSC3__
void flags_ntsc3_includer(){
asm("NTSC3 equ 1\n");
}
#endif

#if defined __NTSC5_6_7__
void flags_ntsc6_includer(){
asm("NTSC5_6_7 equ 1\n");
}
#endif

#endif // _FLAGS_H
