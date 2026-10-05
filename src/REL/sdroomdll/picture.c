#include "types.h"

typedef struct {
    u8 b[0x146];
} PIC_GWSD;

extern PIC_GWSD GwSdCommon;

extern u16 HuPadBtnDown;
extern s16 Hu3DCameraNo;

extern f64 sin(f64);
extern f64 cos(f64);

extern s32 HuWinCreate(f32 x, f32 y, s32 width, s32 height, s32 flag);
extern void HuWinKill(s32 winId);
extern void HuWinMesSet(s32 winId, s32 messNum);
extern void HuWinMesSpeedSet(s32 winId, s32 speed);
extern void HuWinPriSet(s32 winId, s32 pri);
extern void HuWinAttrSet(s32 winId, u32 attr);
extern void HuWinBGTPLvlSet(s32 winId, f32 level);
extern void HuPrcVSleep(void);
extern void Hu3DFbCopyExec(s32 srcX, s32 srcY, s32 width, s32 height, s32 mode, s32 arg, void *fb);
extern void Hu3DZClear(void);
extern void DCFlushRange(void *addr, u32 len);

extern void fn_1_1BDF8(f32 arg);

/* literals used by the picture unit */
static const f64 lbl_1_rodata_518 = 1500.0;
static const f64 lbl_1_rodata_520 = -2.0943951023931953;
static const f32 lbl_1_rodata_528 = 200.0f;
static const f32 lbl_1_rodata_550 = 0.0f;
static const f32 lbl_1_rodata_5C8 = -10000.0f;
static const f64 lbl_1_rodata_620 = 0.0;
static const f32 lbl_1_rodata_650 = 275.0f;

/* module owned globals */
f32 lbl_1_data_1860[7] = {
    433.0f,
    200.0f,
    250.0f,
    0.0f,
    240.0f,
    0.0f,
    840.0f
};

f32 lbl_1_bss_A90;
u32 lbl_1_bss_A94;
u8 lbl_1_bss_A98[6];
s16 lbl_1_bss_A9E;
s32 lbl_1_bss_AA0;
u8 lbl_1_bss_AA4[0x2C];
u16 lbl_1_bss_AD0;
s16 lbl_1_bss_AD2;
s32 lbl_1_bss_AD4[0x47];
u32 lbl_1_bss_BF0[2];

typedef struct {
    u8 pad[8];
    s32 ptr;
} PIC_FBENTRY;

typedef struct {
    u8 pad[0x80];
    s32 ptr;
} PIC_FBSPECIAL;

void fn_1_19324(void)
{
    lbl_1_data_1860[0] = 1500.0 * -sin(lbl_1_rodata_520);
    lbl_1_data_1860[1] = lbl_1_rodata_528;
    lbl_1_data_1860[2] = 1500.0 * -cos(lbl_1_rodata_520);
}

void fn_1_1A850(void)
{
    if (Hu3DCameraNo == 0) {
        if (lbl_1_bss_A9E >= 0) {
            if (lbl_1_bss_A9E >= 0 && lbl_1_bss_A9E < 10) {
                Hu3DFbCopyExec(0, 0, 0x80, 0x80, 5, 0, (void *)((PIC_FBENTRY *)&lbl_1_bss_AD4)[lbl_1_bss_A9E].ptr);
            } else {
                if (lbl_1_rodata_620 != lbl_1_bss_A90) {
                    fn_1_1BDF8(lbl_1_bss_A90);
                }

                Hu3DFbCopyExec(0, 0, 0x1E0, 0x1E0, 5, 0, (void *)((PIC_FBSPECIAL *)&lbl_1_bss_AD4)->ptr);
            }

            Hu3DZClear();
        }
    }
}

/* NONMATCHING: mwceppc 1.3.2 normalizes the s32 win id with clrlwi before the
   halfword store, and materializes 0x200B/0x1905F with li / lis+subi instead
   of CodeWarrior's lis+addi. */
void fn_1_1B640(void)
{
    s32 winId;

    winId = HuWinCreate(lbl_1_rodata_5C8, lbl_1_rodata_650, 0x220, 0x1A, 0);
    lbl_1_bss_AD0 = winId;
    HuWinMesSpeedSet(winId, 0);
    HuWinPriSet(winId, 0x28);
    HuWinAttrSet(winId, 0x800);
    HuWinBGTPLvlSet(winId, lbl_1_rodata_550);
    HuWinMesSet(winId, 0x200B);
    HuWinMesSet(lbl_1_bss_AD2, 0x1905F);

    while ((HuPadBtnDown & 0x200) == 0) {
        HuPrcVSleep();
    }

    HuWinKill(*(s16 *)&lbl_1_bss_AD0);
}

void fn_1_1BCD4(s16 idx)
{
    s16 i;
    u16 *p;
    s32 j;

    for (i = 1; i <= 30; i++) {
        lbl_1_bss_A90 = (f64)i / 30.0;
        HuPrcVSleep();
    }

    p = (u16 *)((PIC_FBENTRY *)&lbl_1_bss_AD4)[idx].ptr;

    for (j = 0; j < 0x4000; j++, p++) {
        *p = 0x8000;
    }

    DCFlushRange((void *)((PIC_FBENTRY *)&lbl_1_bss_AD4)[idx].ptr, 0x8000);
    *(u16 *)((u8 *)&GwSdCommon + idx * 0x1A + 0x5A) = 0xFFFF;
}

typedef struct {
    u16 v[13];
} PIC_FLAGS;

void fn_1_1C13C(void)
{
    s16 i;
    s16 j;
    PIC_FLAGS *p;

    for (i = 8; i >= 0; i--) {
        p = (PIC_FLAGS *)((u8 *)&GwSdCommon + i * 0x1A + 0x42);

        if (p->v[12] != 0xFFFF) {
            continue;
        }

        for (j = i; j < 9; j++) {
            *(PIC_FLAGS *)((u8 *)&GwSdCommon + j * 0x1A + 0x42) =
                *(PIC_FLAGS *)((u8 *)&GwSdCommon + (j + 1) * 0x1A + 0x42);
        }

        *(u16 *)((u8 *)&GwSdCommon + 0x144) = 0xFFFF;
    }
}