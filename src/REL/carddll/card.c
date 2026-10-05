// carddll/card.c - decompilation of carddll/REL/carddll/card (27484 B, 17 fns).
// Functions are added smallest-first, each verified at 0 differing rows with
// tools/fndiff.py before the next one.

#include "types.h"

/* ---- externs (declared locally, no shared header for this unit) ---- */
void Hu3DAnimKill(s32 animId);
s16 Hu3DAnimCreate(void *dataP, s32 modelId, char *bmpName);
void *HuDataReadNum(void *dataNum, s32 num);

/* lbl_1_data_B0 lives in player.o, not in this translation unit. */
extern void *lbl_1_data_B0[];

/* ---- bss globals owned by card.o ---- */
static s16 lbl_1_bss_330[50];
static s32 lbl_1_bss_394;
static s16 lbl_1_bss_478[10][10];

/* ---- data globals owned by card.o ---- */
static char lbl_1_data_9C[8] = "S3_c029";
static char lbl_1_data_A4[8] = "S3_c028";

void fn_1_F24C(void) {
    s32 i;

    for (i = 0; i < lbl_1_bss_394; i++) {
        Hu3DAnimKill(lbl_1_bss_330[i]);
    }
}

void fn_1_F104(s32 arg0, s32 arg1) {
    lbl_1_bss_330[lbl_1_bss_394] =
        Hu3DAnimCreate(HuDataReadNum(lbl_1_data_B0[0x20], 0x10000000),
                       lbl_1_bss_478[arg0][arg1], lbl_1_data_9C);
    lbl_1_bss_394++;
}

void fn_1_F1A8(s32 arg0, s32 arg1) {
    lbl_1_bss_330[lbl_1_bss_394] =
        Hu3DAnimCreate(HuDataReadNum(lbl_1_data_B0[0x20], 0x10000000),
                       lbl_1_bss_478[arg0][arg1], lbl_1_data_A4);
    lbl_1_bss_394++;
}
