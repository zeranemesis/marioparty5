#include "types.h"

/* ------------------------------------------------------------------ */
/* Data owned by this translation unit (B/D/R in the original game.o)  */
/* ------------------------------------------------------------------ */

/* Handle of the minigame master timer. */
s32 lbl_1_bss_490C;

/* Scroll position table: six floats, read two at a time. */
f32 lbl_1_data_3F8[6] = {0.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.0f};

/* Named float literals referenced by this unit. Declared as one-element
 * arrays so MWCC keeps them as real, named .rodata objects instead of
 * folding them into anonymous entries of the function literal pool. */
const f32 lbl_1_rodata_118[1] = {288.0f};
const f32 lbl_1_rodata_170[1] = {0.0f};
const f32 lbl_1_rodata_180[1] = {64.0f};

/* ------------------------------------------------------------------ */
/* Imported data (U in the original game.o)                            */
/* ------------------------------------------------------------------ */

extern s32 lbl_1_bss_0;
extern u8 lbl_1_bss_36E2[];
extern u8 lbl_1_bss_3700[];
extern u8 lbl_1_bss_4794[];

/* ------------------------------------------------------------------ */
/* Imported functions                                                  */
/* ------------------------------------------------------------------ */

s32 MgTimerCreate(s32 id);
void MgTimerParamSet(s32 timer, s32 frameLimit, s32 a, s32 b);
void MgTimerPosSet(s32 timer, f32 x, f32 y);
void MgTimerModeOnSet(s32 timer, s32 mode);
s32 MgTimerDoneCheck(s32 timer);

void Hu3DTexScrollPosSet(s32 scrId, f32 x, f32 y, f32 z);

void fn_1_3374(s32 idx);
void fn_1_6920(void);
void fn_1_6DE0(void);
void fn_1_88EC(void);
void fn_1_BADC(void);
void fn_1_9238(s32 idx);

/* ------------------------------------------------------------------ */
/* Views onto the two player-table blobs. Each is walked with its own */
/* stride, so each gets its own cast view.                              */
/* ------------------------------------------------------------------ */

/* lbl_1_bss_36E2, stride 0x0E. The s16 at 0x0C is the scroll slot id. */
typedef struct M552Tab14_s {
    s16 v[7];
} M552Tab14;

/* lbl_1_bss_4794, stride 0x1C. The s16 at 0x08 is the "slot busy" flag. */
typedef struct M552Tab1C_s {
    s32 unk00;
    s32 unk04;
    s16 busy;
    s16 unk0A;
    s32 unk0C;
    s32 unk10;
    s32 unk14;
    s32 unk18;
} M552Tab1C;

/* ------------------------------------------------------------------ */

void fn_1_27B4(void)
{
    switch (lbl_1_bss_0) {
    case 0:
        lbl_1_bss_490C = MgTimerCreate(0);
        MgTimerParamSet(lbl_1_bss_490C, 0xE10, 0, 0);
        MgTimerPosSet(lbl_1_bss_490C, lbl_1_rodata_118[0], lbl_1_rodata_180[0]);
        MgTimerModeOnSet(lbl_1_bss_490C, 1);
        break;
    case 1:
    case 2:
        MgTimerModeOnSet(lbl_1_bss_490C, 1);
        break;
    }

    ((s32 *)lbl_1_bss_4794)[64]++;
    fn_1_88EC();
}

void fn_1_2890(void)
{
    s32 i;

    fn_1_BADC();

    for (i = 0; i < 2; i++) {
        if (((M552Tab1C *)lbl_1_bss_4794)[i].busy != 0) {
            fn_1_9238(i);
        }
        fn_1_3374(i);
    }

    fn_1_88EC();

    if (MgTimerDoneCheck(lbl_1_bss_490C)) {
        ((s32 *)lbl_1_bss_4794)[64]++;
    }

    fn_1_6920();
    fn_1_6DE0();
}

void fn_1_2AA0(void)
{
    ((s32 *)lbl_1_bss_4794)[64] = 0;
    ((s32 *)lbl_1_bss_4794)[65] = 0;
}

s32 fn_1_6758(s32 idx, s32 which)
{
    s32 max = 0;
    s8 *p = (s8 *)(lbl_1_bss_3700 + idx * 0x254 + 0xBEC);
    s32 i;

    for (i = 0; i < 4; i++) {
        if (*(p + 0x15C + i) > max) {
            max = *(p + 0x15C + i);
        }
    }

    if (max == *(p + 0x15C + which)) {
        i = *(p + 0x15C + which) * 100 + *(p + 0x15C + which) * 10000 / 35;
    } else {
        i = *(p + 0x15C + which) * 100;
    }
    return i;
}

void fn_1_7800(s32 idx, s32 scrId)
{
    Hu3DTexScrollPosSet(((M552Tab14 *)lbl_1_bss_36E2)[idx].v[6],
                        lbl_1_data_3F8[scrId * 2],
                        lbl_1_data_3F8[scrId * 2 + 1],
                        lbl_1_rodata_170[0]);
}
