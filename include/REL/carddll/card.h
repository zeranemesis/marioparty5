#ifndef _CARDDLL_CARD_H
#define _CARDDLL_CARD_H

#include "REL/carddll.h"

/* globals defined in card.c (not referenced outside it; see carddll.h for the shared ones) */
extern s32 lbl_1_bss_320;
extern f32 lbl_1_bss_324;
extern f32 lbl_1_bss_328;
extern s32 lbl_1_bss_32C;
extern s16 lbl_1_bss_330[50];
extern s32 lbl_1_bss_394;
extern s32 lbl_1_bss_398;
extern s32 lbl_1_bss_39C[4];
extern s32 lbl_1_bss_3AC[4];
extern s32 lbl_1_bss_3BC[3];
extern s32 lbl_1_bss_3C8[4];
extern s32 lbl_1_bss_3D8[3];
extern Vec lbl_1_bss_3E4;
extern s32 lbl_1_bss_A54[10][10];

void fn_1_A520(CardGame *game);
void fn_1_B104(s32 size);
void fn_1_BB70(s32 row, s32 col, s32 size);
void fn_1_C614(s32 size);
void fn_1_C864(CardGame *game);
void fn_1_D7BC(CardGame *game);
void fn_1_DC9C(CardGame *game);
void fn_1_E17C(CardGame *game);

#endif
