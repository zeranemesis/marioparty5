#ifndef _CARDDLL_CARDMAIN_H
#define _CARDDLL_CARDMAIN_H

#include "REL/carddll.h"

/* globals defined in cardmain.c (the ones shared with other units are in carddll.h) */
extern s32 lbl_1_bss_18;
extern s16 lbl_1_bss_48;
extern s16 lbl_1_bss_4A;
extern s32 lbl_1_bss_4C[4];
extern s32 lbl_1_bss_60;
extern f32 lbl_1_bss_64;
extern s32 lbl_1_bss_68;
extern s32 lbl_1_bss_6C;
extern s16 lbl_1_bss_70;
extern s16 lbl_1_bss_72;
extern s32 lbl_1_bss_74[3];
extern f32 lbl_1_bss_80;
extern s32 lbl_1_bss_84;
extern Vec lbl_1_bss_88;
extern s32 lbl_1_bss_94;
extern s32 lbl_1_bss_98[2];
extern f32 lbl_1_bss_A0[2];
extern s16 lbl_1_bss_A8[2];
extern s32 lbl_1_bss_AC;
extern s32 lbl_1_bss_B0;
extern s32 lbl_1_bss_B4;
extern s32 lbl_1_bss_B8;
extern f32 lbl_1_bss_BC;
extern f32 lbl_1_bss_C0;
extern f32 lbl_1_bss_C4;
extern f32 lbl_1_bss_C8;
extern Vec lbl_1_bss_CC;
extern Vec lbl_1_bss_D8;
extern f32 lbl_1_bss_2BC[2];
extern s32 lbl_1_bss_2C4;
extern Vec lbl_1_bss_2C8[4];
extern s16 lbl_1_bss_2F8[4][2];
extern s16 lbl_1_bss_308[6];
extern OMOBJ *lbl_1_bss_318;
extern HUPROCESS *lbl_1_bss_31C;

void ObjectSetup(void);
void fn_1_2E8(OMOBJ *obj);
void fn_1_70C(OMOBJ *obj);
void fn_1_934(OMOBJ *obj);
s32 fn_1_AB4(CardGame *game);
s32 fn_1_CA4(CardGame *game);
void fn_1_41EC(CardGame *game);
s32 fn_1_49E4(CardGame *game);
void fn_1_7DFC(void);
void fn_1_5A00(CardGame *game);
u8 fn_1_62A4(CardGame *game);
u8 fn_1_7098(CardGame *game, s32 arg1, s32 arg2, s32 arg3);
void fn_1_7304(void);
void fn_1_77D0(Vec *pos, CardGame *game);
void fn_1_7E28(void);
void fn_1_7E98(CardPlayer *pl);
void fn_1_850C(u8 *alpha, OMOBJ **obj);
void fn_1_8568(HU3DMODEL *model, Mtx *mtx);
void fn_1_8894(void);
void fn_1_8B84(CardGame *game);
void fn_1_8C78(void);

/* wall/exit bits of the cell the player stands on, selected by the pawn slot (f20, f24) */
#define CARD_WALL(pl) (lbl_1_bss_540[(pl)->row][(pl)->col][(pl)->f20 * 3 + (pl)->f24])

#endif
