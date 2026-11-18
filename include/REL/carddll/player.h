#ifndef _CARDDLL_PLAYER_H
#define _CARDDLL_PLAYER_H

#include "REL/carddll.h"

/* Globals defined in carddll/player.c (bss at 0xBE8..0x1168). */
extern f32 lbl_1_bss_F14[10];
extern s32 lbl_1_bss_F3C[10];

/* Functions of carddll/player.c not listed in carddll.h. */
s32 fn_1_FCD4(CardGame *game);
s32 fn_1_11C48(CardGame *game);
s32 fn_1_12440(s32 arg0);
s32 fn_1_127F8(void);
s32 fn_1_1328C(s32 arg0);
s32 fn_1_13728(CardGame *game);
s32 fn_1_15D38(CardGame *game);
s32 fn_1_1658C(CardGame *game);
s32 fn_1_16DBC(CardGame *game);
void fn_1_17BE8(CardGame *game, s32 arg1);
void fn_1_18054(s32 plNo, s32 dir);
void fn_1_20DE4(s32 plNo, CardGame *game);
s32 fn_1_21CA4(s32 plNo, CardGame *game);
s32 fn_1_227A4(s32 plNo, CardGame *game);
void fn_1_25518(s32 plNo);
s32 fn_1_258E4(CardGame *game);
void fn_1_28CA8(CardGame *game, s32 arg1, s32 arg2);
void fn_1_290BC(s32 plNo);
s32 fn_1_2A260(CardGame *game);
s32 fn_1_2B10C(CardGame *game);
s32 fn_1_2B280(CardGame *game);
s32 fn_1_2E548(CardGame *game);
void fn_1_2FBCC(CardGame *arg0);
void fn_1_2FFAC(CardGame *game);
void fn_1_303E0(CardGame *game);
void fn_1_304D8(CardGame *game, s32 *ids);
void fn_1_305B4(CardGame *game, s32 arg1);

#endif
