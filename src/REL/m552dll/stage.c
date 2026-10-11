#include "types.h"
#include "game/gamework.h"




/* ------------------------------------------------------------------ */
/* Data owned by this translation unit (B/D/R in the original stage.o)  */
/* ------------------------------------------------------------------ */

/* One entry in the stage's character table, stride 0x1C. The s16 at 0x06
 * is the character number that is handed to CharMotionCreate. */
typedef struct M552StageEntry_s {
    /* 0x00 */ u8 unk00[0x04];
    /* 0x04 */ s16 playerNo;
    /* 0x06 */ s16 charNo;
    /* 0x08 */ s16 unk08;
    /* 0x0A */ s16 padNo;
    /* 0x0C */ u8 unk0C[0x10];
} M552StageEntry;

/* One per-player stage slot, stride 0x54. */
typedef struct M552StageSlot_s {
    /* 0x00 */ s16 unk00;
    /* 0x02 */ s16 unk02;
    /* 0x04 */ f32 unk04;
    /* 0x08 */ f32 unk08;
    /* 0x0C */ u8 unk0C[0x0C];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ u8 unk20[0x34];
} M552StageSlot;

/* The stage work structure. */
typedef struct M552StageWork_s {
    /* 0x00 */ M552StageEntry entry[2];
    /* 0x38 */ u8 unk38[0x04];
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ u32 *motionData[2];
    /* 0x48 */ s16 *motionId[2];
    /* 0x50 */ M552StageSlot slot[2];
    /* 0xF8 */ s32 unkF8;
    /* 0xFC */ u8 unkFC[0x74];
} M552StageWork;

M552StageWork *lbl_1_bss_4A28;

/* Named float literals referenced by this unit. Declared as one-element
 * arrays so MWCC keeps them as real, named .rodata objects instead of
 * folding them into anonymous entries of the function literal pool. */
const f32 lbl_1_rodata_368[1] = {0.0f};
const f32 lbl_1_rodata_36C[1] = {1.0f};
const f32 lbl_1_rodata_370[1] = {500.0f};
const f64 lbl_1_rodata_378[1] = {4503601774854144.0};

/* Board start positions, {x, y, z} each; y sits below the board. */
f32 lbl_1_data_568[18] = {
    0.0f, -100.0f, 0.0f,
    0.0f, -100.0f, 0.0f,
    0.0f, -100.0f, 0.0f,
    0.0f, -100.0f, 0.0f,
    0.0f, -100.0f, 0.0f,
    0.0f, -100.0f, 0.0f,
};

/* ------------------------------------------------------------------ */
/* Imported functions                                                  */
/* ------------------------------------------------------------------ */

s16 CharMotionCreate(s16 charNo, u32 dataNum);

void *memset(void *dst, int c, unsigned int n);

extern GWPLAYERCONF GwPlayerConf[GW_PLAYER_MAX];

/* ------------------------------------------------------------------ */

// NONMATCHING: the inner-loop counters land in r29/r30/swapped pair
//   under MWCC 1.3.2; the original's CodeWarrior build keeps the other one.
void fn_1_B33C(M552StageWork *work, s32 arg1, s32 arg2)
{
    s16 i;
    s16 j;
    s16 grp;

    lbl_1_bss_4A28 = work;
    memset(lbl_1_bss_4A28, 0, 0x170);
    lbl_1_bss_4A28->unk3C = arg1;
    lbl_1_bss_4A28->unkF8 = arg2;

    for (i = 0; i < 2;) {
        for (j = 0; j < GW_PLAYER_MAX; j++) {
            grp = GwPlayerConf[j].grp;
            if (i == grp) {
                lbl_1_bss_4A28->entry[i].playerNo = j;
                if (++i >= 2) {
                    break;
                }
            }
        }
    }

    for (j = 0; j < 2; j++) {
        lbl_1_bss_4A28->entry[j].charNo = GwPlayerConf[lbl_1_bss_4A28->entry[j].playerNo].charNo;
        lbl_1_bss_4A28->entry[j].padNo = GwPlayerConf[lbl_1_bss_4A28->entry[j].playerNo].padNo;
        grp = GwPlayerConf[lbl_1_bss_4A28->entry[j].playerNo].type;
        if (grp != 0) {
            lbl_1_bss_4A28->entry[j].unk08 = GwPlayerConf[lbl_1_bss_4A28->entry[j].playerNo].dif + 1;
        }
    }
}

void fn_1_B810(u32 *motionTable0, u32 *motionTable1)
{
    lbl_1_bss_4A28->motionData[0] = motionTable0;
    lbl_1_bss_4A28->motionData[1] = motionTable1;
}

void fn_1_B834(s16 *motionId0, s16 *motionId1)
{
    lbl_1_bss_4A28->motionId[0] = motionId0;
    lbl_1_bss_4A28->motionId[1] = motionId1;
}

void fn_1_BF98(void)
{
    s16 j;
    s16 i;
    s16 grp;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            lbl_1_bss_4A28->motionId[i][j] = CharMotionCreate(lbl_1_bss_4A28->entry[i].charNo,
                                                               lbl_1_bss_4A28->motionData[i][j]);
        }
    }

    for (i = 0; i < 2; i++) {
        lbl_1_bss_4A28->slot[i].unk00 = 0;
        lbl_1_bss_4A28->slot[i].unk02 = -1;
        lbl_1_bss_4A28->slot[i].unk04 = lbl_1_rodata_368[0];
        lbl_1_bss_4A28->slot[i].unk08 = lbl_1_rodata_368[0];
        lbl_1_bss_4A28->slot[i].unk18 = 0;
        lbl_1_bss_4A28->slot[i].unk1C = 0;
    }
}
