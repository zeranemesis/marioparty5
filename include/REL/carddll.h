#ifndef _CARDDLL_H
#define _CARDDLL_H

#include "game/object.h"
#include "game/process.h"
#include "game/memory.h"
#include "game/hu3d.h"
#include "game/data.h"
#include "game/wipe.h"
#include "game/window.h"
#include "game/gamework.h"
#include "game/pad.h"
#include "game/audio.h"
#include "game/frand.h"
#include "game/esprite.h"
#include "game/hsfex.h"

/* Shared declarations for the carddll (Card Party bonus) units.
 * Rules: add only, re-read before editing. */

/* ---- game state (lbl_1_bss_E4 in cardmain.c, 0x1D8 bytes). Every unit receives a pointer to it. ---- */
typedef struct CardPlayer_s {
    /* 0x00 */ s32 f00;
    /* 0x04 */ s32 f04;
    /* 0x08 */ s32 f08;
    /* 0x0C */ s32 f0C;
    /* 0x10 */ s32 row;      /* grid row of the cursor/card */
    /* 0x14 */ s32 col;      /* grid column */
    /* 0x18 */ f32 rotY;
    /* 0x1C */ f32 rotY2;
    /* 0x20 */ s32 f20;
    /* 0x24 */ s32 f24;
    /* 0x28 */ s32 f28;
    /* 0x2C */ s32 charNo;
    /* 0x30 */ s32 f30;
    /* 0x34 */ s32 f34;
    /* 0x38 */ s32 f38;
    /* 0x3C */ s32 f3C;
    /* 0x40 */ s32 f40;
    /* 0x44 */ s32 f44;
    /* 0x48 */ s32 f48;
    /* 0x4C */ s32 f4C;
    /* 0x50 */ s32 f50;
    /* 0x54 */ s16 f54;
    /* 0x56 */ s16 f56;
    /* 0x58 */ s16 f58;
    /* 0x5A */ u8 pad5A[2];
} CardPlayer; /* size 0x5C */

typedef struct CardGame_s {
    /* 0x000 */ CardPlayer pl[4];
    /* 0x170 */ s32 curPlayer;
    /* 0x174 */ s32 order[4];      /* player index per turn slot */
    /* 0x184 */ s32 f184;
    /* 0x188 */ s32 f188;
    /* 0x18C */ s32 f18C;
    /* 0x190 */ s32 f190;
    /* 0x194 */ s32 boardSize;
    /* 0x198 */ s32 f198;
    /* 0x19C */ s32 f19C;
    /* 0x1A0 */ s32 f1A0;
    /* 0x1A4 */ s32 f1A4;
    /* 0x1A8 */ s32 f1A8;
    /* 0x1AC */ s32 f1AC;
    /* 0x1B0 */ s32 f1B0[6];
    /* 0x1C8 */ s32 cnt1C8;
    /* 0x1CC */ s32 cnt1CC;
    /* 0x1D0 */ s32 cnt1D0;
    /* 0x1D4 */ s32 f1D4;
} CardGame; /* size 0x1D8 */

/* ---- globals owned by cardmain.c ---- */
extern CardGame lbl_1_bss_E4;
extern s16 lbl_1_bss_0[4];
extern s32 lbl_1_bss_8[3];
extern s32 lbl_1_bss_14;
extern s32 lbl_1_bss_1C;
extern s32 lbl_1_bss_20[10];
extern s16 lbl_1_bss_5C;
extern u8 lbl_1_bss_5E;
extern s32 lbl_1_bss_314;

/* ---- globals owned by card.c ---- */
extern s32 lbl_1_bss_3F0;
extern s16 lbl_1_bss_3F4[30];
extern s16 lbl_1_bss_430[4][7];
extern s16 lbl_1_bss_468[4];
extern s16 lbl_1_bss_470[4];
extern s16 lbl_1_bss_478[10][10];
extern u8 lbl_1_bss_540[10][10][9];
extern s32 lbl_1_bss_8C4[10][10];

/* ---- globals owned by camera.c ---- */
extern s32 lbl_1_bss_1168[3];
extern s32 lbl_1_bss_1174;
extern s32 lbl_1_bss_C48[2];
extern s32 lbl_1_bss_1298[4];
extern u8 lbl_1_bss_12A8[]; /* status.c window blob; +8 = char no. */
extern s32 lbl_1_bss_11DC;
extern s16 lbl_1_bss_BE8;
extern Vec lbl_1_bss_BF4;
extern f32 lbl_1_bss_C00;
extern Vec lbl_1_bss_E68[3];
extern s32 lbl_1_bss_F3C[10];
extern s16 lbl_1_bss_1050[3];
extern s32 lbl_1_data_2B4[0x24];
extern s32 lbl_1_data_344[26];
extern s32 lbl_1_data_3AC[3];
extern s32 lbl_1_data_3B8[6];
extern s32 lbl_1_data_3D0[3];
extern s32 lbl_1_data_3DC[4];
extern s32 lbl_1_data_3EC[5];
extern s32 lbl_1_data_400[10];
extern f32 lbl_1_data_528[4][2];
extern f32 lbl_1_data_548[4][2];
extern f32 lbl_1_data_568[4][2];
extern f32 lbl_1_data_588[4][2];

/* ---- globals owned by player.c ---- */
extern s32 lbl_1_data_B0[];
extern s32 lbl_1_data_298[];
extern s32 lbl_1_bss_C3C;
extern s16 lbl_1_bss_F9E[2];

/* ---- cross-unit functions ---- */
/* cardmain.c */
s32 fn_1_8D04(s32 arg0, s16 arg1, s32 arg2);
s32 fn_1_8E88(void);
s32 fn_1_5248(CardGame *game);
void fn_1_5500(CardGame *game);
s32 fn_1_5710(CardGame *game, s32 arg1, s32 arg2);
/* card.c */
void fn_1_8EC8(CardGame *game);
void fn_1_9C74(Vec *out, s32 row, s32 col, s32 zSel, s32 xSel);
void fn_1_9DA8(Vec *out, CardGame *game);
void fn_1_F24C(void);
s32 fn_1_EB38(s32 row, s32 col);
void fn_1_F104(s32 row, s32 col);
void fn_1_F1A8(s32 row, s32 col);
void fn_1_AB64(s32 row, s32 col, s32 flags);
s32 fn_1_F2A8(s32 mode, s32 row, s32 col);
/* camera.c */
void fn_1_30E08(void);
s32 fn_1_30FA4(s32 mode, void *arg1);
void fn_1_31B04(Vec *a, Vec *b, Vec *out, f32 t);
void fn_1_320C4(Vec *a, Vec *b, Vec *out, s32 axis, f32 t);
void fn_1_31710(Vec *pos);
void fn_1_31788(Vec *pos);
void fn_1_317F0(Vec *curPos, Vec *dstPos, Vec *curTgt, Vec *dstTgt);
s32 fn_1_318E4(void);
s32 fn_1_319D4(f32 frames);
void fn_1_31AC0(Vec *pos, Vec *target);
/* player.c */
s32 fn_1_12E04(void);
s32 fn_1_182E0(CardGame *game);
s32 fn_1_187FC(CardGame *game, s32 arg);
s32 fn_1_198F4(CardGame *game);
s32 fn_1_1AB2C(CardGame *game);
s32 fn_1_1B2F0(CardGame *game);
s32 fn_1_21028(CardGame *game);
s32 fn_1_25624(CardGame *game);
s32 fn_1_2930C(CardGame *game);
void fn_1_2FEF4(CardGame *game);
void fn_1_30910(void);
void fn_1_30B00(CardGame *game, s32 arg);
void fn_1_FA24(void);
s32 fn_1_FB88(CardGame *game);
/* status.c */
void fn_1_3EAA0(void);
s16 fn_1_3ED3C(s32 arg0, Vec *pos);
void fn_1_3F278(s32 arg0, s16 arg1);
void fn_1_3F3D8(s32 arg0, s16 arg1, Vec *pos);
void fn_1_3F6B0(s32 arg0, s16 arg1, Vec *pos);
void fn_1_3F73C(void);
void fn_1_4FBC8(CardGame *game, s32 arg1);
void fn_1_501B4(s32 arg0);
void fn_1_502E0(s32 arg0);
void fn_1_50460(void);
/* dice.c */
void fn_1_34CC8(CardGame *game);
s32 fn_1_32D70(CardGame *game);
s32 fn_1_345B4(CardGame *game);
void fn_1_358F8(CardGame *game);
void fn_1_3678C(CardGame *game);
void fn_1_36828(void);
void fn_1_36A70(Vec *pos);
void fn_1_36B10(s32 arg0);
void fn_1_392B8(void);
s32 fn_1_393C4(CardGame *game);
void fn_1_4FBFC(CardGame *game, s32 id);
void fn_1_9DA8(Vec *out, CardGame *game);

/* per-unit headers (include guards make the circular include safe: the shared types are above) */
#include "REL/carddll/card.h"
#include "REL/carddll/cardmain.h"
#include "REL/carddll/dice.h"
#include "REL/carddll/player.h"
#include "REL/carddll/status.h"

#endif
