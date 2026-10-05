// carddll/dice.c - decompilation of carddll/REL/carddll/dice (XXXX B, 6 fns).
// Functions are added smallest-first, each verified at 0 differing rows with
// tools/fndiff.py before the next one.

#include "types.h"
#include "game/hu3d.h"

/* ---- externs (declared locally, no shared header for this unit) ---- */
void Hu3DMotionShiftSet(HU3DMODELID modelId, HU3DMOTID motId, float start, float end, u32 attr);
void fn_1_351C4(void *);
s32 fn_1_393C4(void *);

/* lbl_1_bss_C48 lives in player.o, not in this translation unit. */
extern s32 lbl_1_bss_C48[2];

/* ---- bss globals owned by dice.o ---- */
static s16 lbl_1_bss_11B8;
static s16 lbl_1_bss_11BA[2];
static s16 lbl_1_bss_11BE;
static s16 lbl_1_bss_11C0[6];
static f32 lbl_1_bss_11CC[3];
static s32 lbl_1_bss_11D8;
static s32 lbl_1_bss_11DC;
static s16 lbl_1_bss_11E0;
static s16 lbl_1_bss_11E2;
static s16 lbl_1_bss_11E4;
static s16 lbl_1_bss_11E6;
static s16 lbl_1_bss_11E8;
static s32 lbl_1_bss_11EC;

/* ---- rodata globals owned by dice.o ---- */
static f32 lbl_1_rodata_5AC = 0.0f;
static f32 lbl_1_rodata_5B0 = 100.0f;
static f32 lbl_1_rodata_5DC = 10.0f;
static f32 lbl_1_rodata_6BC = -1.0f;

void fn_1_36B10(s32 arg0) {
    Hu3DMotionShiftSet(lbl_1_bss_11BE, lbl_1_bss_11BA[arg0], lbl_1_rodata_5AC, lbl_1_rodata_5DC, 0x40000001);
}

s32 fn_1_34DA4(void *arg0) {
    void *arg0_save = arg0;
    s32 result = 0;
    fn_1_351C4(*(void **)((u8 *)arg0_save + 0x174 + *(s32 *)((u8 *)arg0_save + 0x170) * 4));
    if (fn_1_393C4(arg0_save) != 0) {
        lbl_1_bss_C48[0] = 0;
        lbl_1_bss_C48[1] = 0;
        result = 1;
    }
    return result;
}