#ifndef REL_CARDDLL_H
#define REL_CARDDLL_H

#include "types.h"
#include "dolphin.h"

/* Shared declarations of the Card Party bonus mode module (carddll).
 * Add only: several units include this header. */

/* One of the four participants, 0x5C bytes, array at the start of CardWork. */
typedef struct CardPlayer_s {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 row;
    /* 0x14 */ s32 col;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28[3];
    /* 0x34 */ s32 unk34[3];
    /* 0x40 */ s32 unk40[4];
    /* 0x50 */ s32 flags;
    /* 0x54 */ s32 unk54;
    /* 0x58 */ s16 modelId;
    /* 0x5A */ s16 unk5A;
} CardPlayer; /* size 0x5C */

/* Main work area passed to the bonus mode state functions. Only the part used
 * by dice.c is described; the owner (cardmain.c) allocates the real, larger one. */
typedef struct CardWork_s {
    /* 0x000 */ CardPlayer players[4];
    /* 0x170 */ s32 curPlayer;
    /* 0x174 */ s32 order[4];
    /* 0x184 */ s32 unk184;
    /* 0x188 */ s32 state;
    /* 0x18C */ s32 unk18C[2];
    /* 0x194 */ s32 gridSize;
    /* 0x198 */ s32 unk198[9];
    /* 0x1BC */ s32 unk1BC;
    /* 0x1C0 */ s32 unk1C0[4];
    /* 0x1D0 */ s32 unk1D0;
    /* 0x1D4 */ s32 unk1D4;
} CardWork;

/* Data and bss owned by other units of the module. */
extern s32 lbl_1_data_B0[];       /* model / motion file numbers */
extern s32 lbl_1_data_2B4[];      /* sprite file numbers */
extern f32 lbl_1_data_548[4][2];  /* 2D positions per player */
extern f32 lbl_1_data_568[4][2];
extern f32 lbl_1_data_588[4][2];
extern s32 lbl_1_bss_20;
extern u8 lbl_1_bss_5E;
extern s16 lbl_1_bss_478[10][10]; /* board cell model ids */
extern s32 lbl_1_bss_8C4[10][10]; /* board cell flags */
extern s32 lbl_1_bss_C48[2];
extern s32 lbl_1_bss_12A8[];      /* status.c work, [2] = char of the acting player */

/* Functions of other units used by dice.c. */
void fn_1_2FFAC(CardWork *work);
void fn_1_305B4(CardWork *work, u8 alpha);
s32 fn_1_393C4(CardWork *work);
s32 fn_1_4FBC8(CardWork *work, s32 motion);
void fn_1_8D04(s32 seId, s16 modelId, s32 arg);
void fn_1_9C74(Vec *out, s32 row, s32 col, s32 arg2, s32 arg3);
void fn_1_9DA8(Vec *out, CardWork *work);
s16 fn_1_3ED3C(s32 cameraBit, Vec *pos);
void fn_1_3F278(s32 cameraBit, s16 slot);
void fn_1_3F380(s32 cameraBit, s16 slot, s32 arg);
void fn_1_3F3D8(s32 cameraBit, s16 slot, Vec *pos);
void fn_1_3F594(s32 cameraBit, s32 arg, f32 time);
void fn_1_3F658(s32 cameraBit, s16 slot, s32 arg);
void fn_1_3F6B0(s32 cameraBit, s16 slot, Vec *vec);
s32 fn_1_3F714(s32 cameraBit, s32 arg);

#endif
