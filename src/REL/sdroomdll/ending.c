#include "game/hu3d.h"
#include "game/process.h"
#include "dolphin/gx/GXEnum.h"

extern s16 lbl_1_bss_C40[2];
extern s32 lbl_1_data_1DAC[12];
extern void *lbl_1_bss_18F0;
extern HUPROCESS *lbl_1_bss_C38;
extern void fn_1_247F0(void);

void fn_1_2515C(void)
{
    Hu3DModelKill(lbl_1_bss_C40[0]);
    Hu3DModelKill(lbl_1_bss_C40[1]);
}

s32 fn_1_24D20(s16 arg0)
{
    if (arg0 < 0x20) {
        if (arg0 > 9) {
            return 0x190060;
        }
        return arg0;
    }
    return lbl_1_data_1DAC[arg0 - 0x20];
}

void fn_1_22C44(void)
{
    if (Hu3DCameraNo != 0) {
        return;
    }
    Hu3DFbCopyExec(0, 0, 0x100, 0xC0, GX_TF_RGB5A3, 0, lbl_1_bss_18F0);
    Hu3DZClear();
}

// NONMATCHING: stack frame size and register allocation differ
void fn_1_24758(s16 arg0)
{
    if (lbl_1_bss_C38 != NULL) {
        HuPrcKill(lbl_1_bss_C38);
        lbl_1_bss_C38 = NULL;
    } else {
        lbl_1_bss_C38 = HuPrcChildCreate(fn_1_247F0, 0x3E8, 0x3000, 0, HuPrcCurrentGet());
        *(s32 *)((u8 *)lbl_1_bss_C38 + 0x12C) = arg0;
    }
}

// NONMATCHING: floating-point instruction selection and register allocation differ
float fn_1_22E40(float arg0)
{
    if (arg0 < -2700.0f) {
        return -180.0f;
    }
    if (arg0 > -765.0f) {
        return 0.0f;
    }
    return -180.0f * (1.0f - (arg0 + 2700.0f) / 1935.0f);
}

extern void fn_1_25198(Vec *, s32);
extern void fn_1_24F14(void);

void fn_1_24D6C(void)
{
    Vec sp8;

    sp8.x = 200.0f;
    sp8.y = 500.0f;
    sp8.z = 0.0f;

    while (1) {
        fn_1_25198(&sp8, 1);
        fn_1_24F14();
        HuPrcVSleep();
    }
}