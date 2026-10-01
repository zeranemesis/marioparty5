#ifndef _REL_M570DLL_H
#define _REL_M570DLL_H

#include "types.h"
#include "game/object.h"
#include "game/process.h"

#define M570_PLAYER_MAX 4
#define M570_CPU_MAX 3
#define M570_DUST_MAX 0x28
#define M570_ITEM_MDL_MAX 4

/* Per-player state block (0x54 bytes, allocated per OMOBJ).
 * objId/charNo/padNo/grp/unk8 come from GwPlayerConf. */
typedef struct m570Player_s {
    /* 0x00 */ s16 objId;
    /* 0x02 */ s16 charNo;
    /* 0x04 */ s16 padNo;
    /* 0x06 */ s16 grp;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ HuVecF tra;
    /* 0x1C */ HuVecF rot;
    /* 0x28 */ s32 state;
    /* 0x2C */ OMOBJ *obj;
    /* 0x30 */ s16 unk30;
    /* 0x32 */ s16 unk32;
    /* 0x34 */ s16 idx;
    /* 0x36 */ s16 motNo;
    /* 0x38 */ HuVecF goal;
    /* 0x44 */ s32 timer;
    /* 0x48 */ s32 gotItem;
    /* 0x4C */ s16 btnNo;
    /* 0x4E */ s16 unk4E;
    /* 0x50 */ s32 unk50;
} M570PLAYER;

/* Dust puffs (0x14 bytes, allocated per OMOBJ) */
typedef struct m570Dust_s {
    /* 0x00 */ s16 idx;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ OMOBJ *obj;
    /* 0x08 */ s32 wait;
    /* 0x0C */ s32 loop;
    /* 0x10 */ s32 state;
} M570DUST;

extern HUPROCESS *lbl_1_bss_108;
extern s16 lbl_1_bss_52;

/* m570.c */
s32 fn_1_17DC(HuVecF *pos);
void fn_1_16D4(HuVecF *pos);
void fn_1_1864(HuVecF *pos, s16 seId);
void fn_1_3A74(M570PLAYER *player, s16 idx);
s32 fn_1_3B40(HuVecF *pos, HuVecF *goal, HuVecF *angle, f32 step, const f32 *tbl);
f32 fn_1_3C64(const f32 *tbl, f32 val, f32 min, f32 max);

#endif