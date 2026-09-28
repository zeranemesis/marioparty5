#include "game/hu3d.h"
#include "game/gamework.h"
#include "game/object.h"
#include "game/wipe.h"
#include "game/frand.h"
#include <math.h>

/* rodata (bit-exact values referenced by the original code) */
static f32 lbl_1_rodata_CD0 = 0.99f;
static f32 lbl_1_rodata_CD4 = 125.0f;
static f32 lbl_1_rodata_CD8 = -500.0f;
static f32 lbl_1_rodata_CDC = 250.0f;
static f32 lbl_1_rodata_CE0 = -375.0f;
static f32 lbl_1_rodata_CE4 = 0.04f;
static f32 lbl_1_rodata_CE8 = 3.0f;
static f32 lbl_1_rodata_CEC = 120.0f;
static f32 lbl_1_rodata_CF0 = 1.0f;
static f32 lbl_1_rodata_CF4 = 0.0f;
static f64 lbl_1_rodata_CF8 = 0.25;
static f64 lbl_1_rodata_D00 = 2.0;
static f64 lbl_1_rodata_D08 = 3.141592653589793;
static f32 lbl_1_rodata_D10 = 180.0f;
static f64 lbl_1_rodata_D18 = 180.0;
static f32 lbl_1_rodata_D20 = 10.0f;
static f32 lbl_1_rodata_D24 = 0.95f;
static f32 lbl_1_rodata_D28 = 90.0f;
static f32 lbl_1_rodata_D2C = 1.5f;
static f32 lbl_1_rodata_D30 = -2000.0f;

/* data */
static f32 lbl_1_data_D98[3] = { 0.0f, 1000.0f, 1400.0f };
static f32 lbl_1_data_DA4[3] = { 0.0f, 100.0f, -200.0f };

/* bss */
static void *lbl_1_bss_7960;
static s16 lbl_1_bss_7964[0x14];
static s16 lbl_1_bss_798C;

extern s32 fn_1_21B68(void *, s32, s32, f32 *, f32, f32, f32);
extern s32 fn_1_814(s32);

s32 fn_1_27950(u8 *arg0) {
    s32 var_r23 = 0;
    f32 var_f30;
    f32 var_f29;
    s16 var_r28;
    s32 var_r28_2;
    s32 temp_r25;
    s32 temp_r0_3;
    s16 temp_r22;
    s16 temp_r0_2;
    u16 temp_r0;
    f32 temp_f31;
    s32 temp_r25_2;
    s32 temp_r25_3;
    s32 temp_r25_4;
    void *temp_r26;
    void *temp_r27;
    void *temp_r29;
    void *temp_r30;

    temp_r29 = (void *) ((u32) arg0 + 0x1BC);
    temp_r25 = (s32) ((u32) arg0 + 0x227C);
    temp_r30 = (void *) ((u32) arg0 + 0x28);
    temp_r27 = (void *) ((u32) arg0 + 0x160);

    temp_r0 = (u16) (*(u16 *) (arg0 + 0x10));
    switch (temp_r0) {
    case 0:
        *(s16 *) ((u32) temp_r27 + 0x24) = 0;
        *(s16 *) ((u32) temp_r27 + 0x28) = 0;
        if ((s16) GwSystem.subGameNo >= 3) {
            *(s32 *) (arg0 + 0x1C) = 0;
            *(s16 *) (arg0 + 0x10) = 4;
            break;
        }
        temp_r25_2 = (s16) (((lbl_1_rodata_CD0 * (lbl_1_rodata_CD4 + (*(f32 *) ((u32) temp_r30 + 0xA0) - lbl_1_rodata_CD8)) / lbl_1_rodata_CDC)));
        temp_r25_3 = (s16) (((lbl_1_rodata_CD0 * (lbl_1_rodata_CD4 + (*(f32 *) ((u32) temp_r30 + 0xA8) - lbl_1_rodata_CE0)) / lbl_1_rodata_CDC)));
        temp_r25_4 = (s32) ((temp_r25_2 + (temp_r25_3 * 5)) * 0x48) + 8;
        lbl_1_bss_7960 = (void *) ((u32) temp_r25 + temp_r25_4);
        var_r28 = 0;
    loop_13:
        if (var_r28 < 0x14) {
            lbl_1_bss_7964[var_r28] = var_r28;
            var_r28 += 1;
            goto loop_13;
        }
        var_r28_2 = 0;
    loop_16:
        if (var_r28_2 < 0x14) {
            temp_r22 = (s16) (frand() % 20);
            temp_r0_2 = (s16) (frand() % 20);
            lbl_1_bss_7964[temp_r22] = lbl_1_bss_7964[temp_r0_2];
            lbl_1_bss_7964[temp_r0_2] = lbl_1_bss_7964[temp_r22];
            var_r28_2 += 1;
            goto loop_16;
        }
        *(s16 *) ((u32) temp_r29 + 0xA) = 1;
        *(s32 *) ((u32) temp_r29 + 0x10) = (s32) lbl_1_data_D98[0];
        *(s32 *) ((u32) temp_r29 + 0x14) = (s32) lbl_1_data_D98[1];
        *(s32 *) ((u32) temp_r29 + 0x18) = (s32) lbl_1_data_D98[2];
        *(s32 *) ((u32) temp_r29 + 0x1C) = (s32) lbl_1_data_DA4[0];
        *(s32 *) ((u32) temp_r29 + 0x20) = (s32) lbl_1_data_DA4[1];
        *(s32 *) ((u32) temp_r29 + 0x24) = (s32) lbl_1_data_DA4[2];
        *(s16 *) ((u32) temp_r29 + 0xC) = 0;
        *(f32 *) ((u32) temp_r29 + 0x7C) = lbl_1_rodata_CE4;
        *(f32 *) ((u32) temp_r29 + 0x80) = lbl_1_rodata_CE4;
        *(s16 *) ((u32) temp_r30 + 0x4) = 0;
        *(s16 *) ((u32) temp_r30 + 0x9A) = 0x1A;
        *(s16 *) ((u32) temp_r30 + 0x9C) = 1;
        Hu3DMotionSpeedSet(*(s32 *) ((u32) temp_r30 + 0x24), lbl_1_rodata_CE8);
        lbl_1_bss_798C = 0;
        *(s32 *) (arg0 + 0x1C) = 0;
        *(s16 *) (arg0 + 0x10) = (u16) (*(s16 *) (arg0 + 0x10) + 1);
        /* fallthrough */
    case 1:
        if (*(s32 *) (arg0 + 0x28AC) < 0) {
            *(s32 *) (arg0 + 0x28AC) = fn_1_814(0x8B5);
        }
        temp_f31 = (f32) (*(s32 *) (arg0 + 0x1C)) / lbl_1_rodata_CEC;
        if (temp_f31 > lbl_1_rodata_CF0) {
            var_f30 = lbl_1_rodata_CF0;
        } else {
            if (temp_f31 < lbl_1_rodata_CF4) {
                var_f29 = lbl_1_rodata_CF4;
            } else {
                var_f29 = temp_f31;
            }
            var_f30 = var_f29;
        }
        Hu3DMotionSpeedSet(*(s32 *) ((u32) temp_r27 + 0x40), (f32) (lbl_1_rodata_CF8 + (lbl_1_rodata_D00 * sin(lbl_1_rodata_D08 * (f64) (lbl_1_rodata_D10 * var_f30) / lbl_1_rodata_D18))));
        Hu3DMotionSpeedSet(*(s32 *) ((u32) temp_r30 + 0x24), lbl_1_rodata_CE8);
        *(s32 *) (arg0 + 0x1C) = (s32) (*(s32 *) (arg0 + 0x1C) + 1);
        if ((*(s32 *) (arg0 + 0x1C) % 10) == 0) {
            fn_1_21B68(temp_r29, 1, 0x32, &lbl_1_rodata_D28, lbl_1_rodata_D20, lbl_1_rodata_D24, lbl_1_rodata_D28);
            omVibrate(*(s32 *) ((u32) temp_r30 + 0x14), 0x14, 0xC, 0);
        }
        if ((*(s32 *) (arg0 + 0x1C) % 5) == 0) {
            if ((s16) lbl_1_bss_798C < 0x14) {
                temp_r26 = (void *) ((u32) temp_r25 + ((lbl_1_bss_7964[lbl_1_bss_798C] * 0x48) + 8));
                if ((temp_r26 != (void *) lbl_1_bss_7960) && ((u32) temp_r26 != (u32) (*(s32 *) ((u32) arg0 + 0x30C) + 0xB0))) {
                    fn_1_814(0x8C3);
                    *(s16 *) ((u32) temp_r26 + 0x4) = 0xA;
                }
                lbl_1_bss_798C += 1;
            } else {
                Hu3DMotionSpeedSet(*(s32 *) ((u32) temp_r30 + 0x24), lbl_1_rodata_CF0);
                *(s32 *) (arg0 + 0x1C) = 0;
                *(s16 *) (arg0 + 0x10) = (u16) (*(s16 *) (arg0 + 0x10) + 1);
            }
        }
        break;
    case 2:
        fn_1_814(0x8C3);
        *(s16 *) ((u32) lbl_1_bss_7960 + 0x4) = 0xA;
        *(f32 *) ((u32) temp_r30 + 0xC4) = 0.0f;
        *(s16 *) ((u32) temp_r30 + 0x4) = 0;
        *(s16 *) ((u32) temp_r30 + 0x9A) = 0x17;
        *(s16 *) ((u32) temp_r30 + 0x9C) = 0;
        Hu3DMotionSpeedSet(*(s32 *) ((u32) temp_r30 + 0x24), lbl_1_rodata_CF0);
        *(s32 *) (arg0 + 0x1C) = 0;
        *(s16 *) (arg0 + 0x10) = (u16) (*(s16 *) (arg0 + 0x10) + 1);
        break;
    case 3:
        *(s32 *) (arg0 + 0x1C) = (s32) (*(s32 *) (arg0 + 0x1C) + 1);
        if (*(s32 *) (arg0 + 0x1C) == 0x1C) {
            *(s16 *) ((u32) temp_r30 + 0x9A) = 0;
            *(s16 *) ((u32) temp_r30 + 0x9C) = 0;
        }
        if (*(s32 *) (arg0 + 0x1C) >= 0x1E) {
            *(f32 *) ((u32) temp_r30 + 0xC4) = (f32) (*(f32 *) ((u32) temp_r30 + 0xC4) - lbl_1_rodata_D2C);
            *(f32 *) ((u32) temp_r30 + 0xA4) = (f32) (*(f32 *) ((u32) temp_r30 + 0xA4) + *(f32 *) ((u32) temp_r30 + 0xC4));
            if (*(f32 *) ((u32) temp_r30 + 0xA4) < lbl_1_rodata_D30) {
                *(s32 *) (arg0 + 0x1C) = 0;
                *(s16 *) (arg0 + 0x10) = (u16) (*(s16 *) (arg0 + 0x10) + 1);
            }
        }
        break;
    case 4:
        temp_r0_3 = (s32) (*(s32 *) (arg0 + 0x1C) + 1);
        *(s32 *) (arg0 + 0x1C) = temp_r0_3;
        if (temp_r0_3 >= 0x1E) {
            WipeCreate(2, 0, 0x3C);
            WipeColorSet(0, 0, 0);
            *(s16 *) ((u32) temp_r30 + 0x6) = 0;
            *(s16 *) ((u32) temp_r30 + 0x4) = 0;
            *(s32 *) (arg0 + 0x1C) = 0;
            *(s16 *) (arg0 + 0x10) = (u16) (*(s16 *) (arg0 + 0x10) + 1);
        }
        break;
    case 5:
        *(s32 *) (arg0 + 0x1C) = (s32) (*(s32 *) (arg0 + 0x1C) + 1);
        if (WipeCheckEnd() == 0) {
            *(s32 *) (arg0 + 0x1C) = 0;
            *(s16 *) (arg0 + 0x10) = 0;
            var_r23 = 1;
        }
        break;
    }
    return var_r23;
}
