#include "REL/mgmatchDll.h"
#include "game/gamework.h"
#include "game/flag.h"
#include "game/process.h"
#include "game/hu3d.h"
#include "game/mgdata.h"
#include "game/object.h"
#include "game/window.h"
#include "game/wipe.h"

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

void ObjectSetup(void);
void fn_1_87C(u32 arg0);
u32 fn_1_8B4(u32 arg0);
void fn_1_BC0(void);
extern void fn_1_9FC(void);
extern void omVibrate(s16 arg0, s16 arg1, s16 arg2, s16 arg3);
extern void HuTHPClose(void);
extern void HuAudSStreamFadeOut(s32 arg0, s32 arg1);
extern void HuWinExKill(HUWINID arg0);
extern void fn_1_2E8(void);
extern void HuAudFXPlayPan(s32 arg0, s32 arg1);
extern void fn_1_1C08(void);
extern void fn_1_1EA0(void);
extern void fn_1_1F58(void);
extern void fn_1_2058(void);
extern void fn_1_20BC(void);
extern void fn_1_2170(void);
extern void fn_1_3850(void);
extern void fn_1_3B24(void);
extern void fn_1_3C40(void);
extern void fn_1_4114(void);
extern void fn_1_6504(void);
extern void fn_1_7D74(void);
extern void fn_1_CE5C(void);
extern void fn_1_67C0(void);
extern void fn_1_267C(void);
extern f32 Hu3DMotionMaxTimeGet(s16 arg0);
extern void Hu3DModelAttrReset(s16 arg0, u32 arg1);
extern void Hu3DModelLayerSet(s16 arg0, s16 arg1);
extern void Hu3DModelCameraSet(s16 arg0, u16 arg1);
extern void fn_1_898(u32 arg0);
extern void fn_1_87C(u32 arg0);
extern void HuWinExClose(HUWINID arg0);
extern void HuWinHomeClear(HUWINID arg0);
void fn_1_85C4(void);
void fn_1_9984(void);
void fn_1_A524(void);
void fn_1_B2D8(void);
void fn_1_BE50(void);
void fn_1_112B8(void);
void fn_1_11F2C(void);
void fn_1_C638(void);
extern void abort(void);
/* forward declarations */
int _prolog(void);
void _epilog(void);
void fn_1_A0(s8 *arg0, u8 *arg1, s8 arg2);
void fn_1_140(u8 *arg0, s32 arg1);
void fn_1_204(s32 arg0);
void fn_1_240(s32 arg0, s32 arg1);
void fn_1_294(s32 arg0, s32 arg1);
void fn_1_2E8(void);
void fn_1_3C4(s32 arg0);
s16 fn_1_458(void);
void fn_1_46C(void);
void fn_1_4B4(void);
s32 fn_1_510(void);
s32 fn_1_528(void);
u8 fn_1_588(void);
u8 fn_1_59C(void);
s32 fn_1_5B0(s32 arg0);
void fn_1_5D0(s32 arg0, s32 arg1);
u8 fn_1_624(void);
s32 fn_1_638(void);
s32 fn_1_64C(s32 *place, s32 *tieNum, s32 *score);
void fn_1_87C(u32 arg0);
void fn_1_898(u32 arg0);
u32 fn_1_8B4(u32 arg0);
void fn_1_8CC(u32 arg0);
void fn_1_8EC(u32 arg0);
u32 fn_1_90C(u32 arg0);
void fn_1_924(u32 arg0);
void fn_1_940(u32 arg0);
u32 fn_1_95C(u32 arg0);
void fn_1_974(void *arg0, s32 arg1);
void fn_1_9FC(void);
void fn_1_B18(s32 arg0);
void ObjectSetup(void);
void fn_1_BC0(void);
void fn_1_D40(s32 arg0);
void fn_1_D88(void);
void fn_1_1A24(void *arg0);
s32 fn_1_1A64(s16 camNo, s32 time, s16 mode);
void fn_1_1C08(void);
void fn_1_1DE8(s16 arg0, f32 arg1, s32 arg2);
s32 fn_1_1E74(void);
void fn_1_1EA0(void);
void fn_1_1F00(void);
void fn_1_1F58(void);
void fn_1_2000(void);
void fn_1_2058(void);
void fn_1_2080(void);
void fn_1_20BC(void);
void fn_1_2134(void);
void fn_1_2170(void);
void fn_1_2628(void);
void fn_1_267C(void);
void fn_1_28BC(s32 arg0, s16 arg1);
void fn_1_2994(void);
void fn_1_2F78(void);
void fn_1_317C(s32 arg0);
void fn_1_31B0(void);
s32 fn_1_32C0(void);
void fn_1_32DC(s32 arg0);
void fn_1_33AC(void);
void fn_1_3560(void);
void fn_1_3850(void);
void fn_1_3AD0(void);
void fn_1_3B24(void);
void fn_1_3C04(void);
void fn_1_3C40(void);
void fn_1_4090(void);
void fn_1_4114(void);
void fn_1_64B0(void);
void fn_1_6504(void);
void fn_1_676C(void);
void fn_1_67C0(void);
void fn_1_6B18(void);
void fn_1_6BE8(void);
void fn_1_72E8(void);
void fn_1_74FC(M505_PLAYER *player, Vec *target);
s32 fn_1_7B6C(f32 *cur, f32 target, f32 step);
void fn_1_7D74(void);
void fn_1_8538(s32 arg0, f32 arg1);
void fn_1_85C4(void);
void fn_1_9844(void);
void fn_1_9984(void);
void fn_1_A524(void);
void fn_1_B2D8(void);
void fn_1_BE50(void);
void fn_1_C638(void);
void fn_1_CCE4(f32 *p, s32 n);
void fn_1_CE5C(void);
void fn_1_D130(void);
/* end forward declarations */

M505_FLOW *lbl_1_bss_0;
M505_MODEWORK *lbl_1_bss_4;
M505_FLOW lbl_1_bss_8;

unsigned int lbl_1_data_0[12] = {
    0x8C0000, 0x8C0001, 0x8C0002, 0x8C0032, 0x8C0031, 0x8C0003,
    0x8C0004, 0x8C0017, 0x8C0018, 0x8C005A, 0x8C0052, 0x0,
};

f32 lbl_1_data_30[12] = {
    -160.0f, 0.0f, 140.0f, 0.0f, 0.0f, 140.0f,
    160.0f, 0.0f, 140.0f, 320.0f, 0.0f, 140.0f,
};

f32 lbl_1_data_60[12] = {
    0.0f, 120.0f, 0.0f, -280.0f, 120.0f, 260.0f,
    -800.0f, 120.0f, 260.0f, -280.0f, 120.0f, 50.0f,
};

f32 lbl_1_data_90[6] = {
    0.0f, -630.0f, -550.0f, 0.0f, 580.0f, -550.0f,
};

f32 lbl_1_data_A8[3] = {
    0.0f, 385.0f, 140.0f,
};

f32 lbl_1_data_B4[3] = {
    0.0f, 250.0f, 140.0f,
};

M505_MGENTRY lbl_1_data_C0[43] = {
    { 0xF, 0x1F5, 0, 0 }, { 0x10, 0x1F6, 0, 0 }, { 0x11, 0x1F7, 0, 0 },
    { 0x12, 0x1F8, 0, 0 }, { 0x13, 0x1F9, 0, 0 }, { 0x14, 0x1FA, 0, 0 },
    { 0x15, 0x1FB, 0, 0 }, { 0x16, 0x1FC, 0, 0 }, { 0x17, 0x1FD, 0, 0 },
    { 0x18, 0x1FE, 0, 0 }, { 0x19, 0x1FF, 0, 0 }, { 0x1A, 0x200, 0, 0 },
    { 0x1B, 0x201, 0, 0 }, { 0x1C, 0x202, 0, 0 }, { 0x1D, 0x203, 0, 0 },
    { 0x4A, 0x233, 0, 0 }, { 0x4B, 0x234, 0, 0 }, { 0x4C, 0x235, 0, 0 },
    { 0x4D, 0x236, 0, 0 }, { 0x57, 0x240, 0, 0 }, { 0x59, 0x243, 0, 0 },
    { 0x23, 0x209, 0, 0 }, { 0x28, 0x20E, 0, 0 }, { 0x29, 0x20F, 0, 0 },
    { 0x2A, 0x210, 0, 0 }, { 0x2B, 0x211, 0, 0 }, { 0x2C, 0x212, 0, 0 },
    { 0x2D, 0x213, 0, 0 }, { 0x2E, 0x214, 0, 0 }, { 0x2F, 0x215, 0, 0 },
    { 0x30, 0x216, 0, 0 }, { 0x55, 0x23E, 0, 0 }, { 0x1F, 0x205, 0, 0 },
    { 0x20, 0x206, 0, 0 }, { 0x21, 0x207, 0, 0 }, { 0x22, 0x208, 0, 0 },
    { 0x24, 0x20A, 0, 0 }, { 0x25, 0x20B, 0, 0 }, { 0x27, 0x20D, 0, 0 },
    { 0x4E, 0x237, 0, 0 }, { 0x4F, 0x238, 0, 0 }, { 0x51, 0x23A, 0, 0 },
    { 0x58, 0x241, 0, 0 },
};

f32 lbl_1_data_1C4[3] = {
    -9000.0f, 32387.9023f, 5000.0f,
};

f32 lbl_1_data_1D0[9] = {
    0.100000001f, 0.239999995f, 0.319999993f, 0.479999989f, 0.560000002f, 0.720000029f, 0.839999974f, 0.899999976f, 1.0f,
};

f32 lbl_1_data_1F4[9] = {
    0.600000024f, 0.649999976f, 0.400000006f, 0.200000003f, 0.300000012f, 0.5f, 0.699999988f, 0.800000012f, 1.10000002f,
};

GXColor lbl_1_data_218[4] = {
    { 0x80, 0x80, 0x80, 0xFF }, { 0x40, 0x40, 0x40, 0xFF }, { 0, 0, 0, 0 }, { 0x40, 0x40, 0x40, 0x80 },
};

HU3DPARMANPARAM lbl_1_data_228 = {
    0x28, 0, 0.3f, 30.0f, 10.0f, { 0.0f, -0.2f, 0.0f }, 2.0f, 1.0f, 40.0f, 0.98f, 2,
    { { 0xFF, 0xFF, 0xFF, 0xFF }, { 0xFF, 0xFF, 0xFF, 0xFF }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
    { { 0xFF, 0xFF, 0xFF, 0 }, { 0xFF, 0xFF, 0xFF, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
};

f32 lbl_1_data_278[42] = {
    184.0f, 0.0f, 180.0f, 210.0f, 130.0f, 0.0f,
    0.0f, 180.0f, 230.0f, 0.0f, 0.0f, 0.0f,
    -199.0f, 0.0f, -220.0f, 0.0f, -170.0f, 0.0f,
    -120.0f, -200.0f, -166.0f, 400.0f, 0.0f, 400.0f,
    400.0f, 400.0f, 0.0f, 0.0f, 400.0f, 400.0f,
    0.0f, 0.0f, 0.0f, -400.0f, 0.0f, -400.0f,
    -400.0f, -400.0f, 0.0f, 0.0f, -400.0f, -400.0f,
};

f32 lbl_1_data_320[6] = {
    800.0f, 200.0f, 1000.0f, 9450.36523f, 14472.124f, 4303.53076f,
};

f32 lbl_1_data_338[6] = {
    -7.0f, 3.0f, -10.0f, -30.0f, -20.0f, -10.0f,
};

f32 lbl_1_data_350[16] = {
    2000.0f, 200.0f, 1000.0f, 0.0f, 1300.0f, 280.0f,
    -2000.0f, 400.0f, -500.0f, 1000.0f, -7000.0f, 800.0f,
    -8000.0f, 3000.0f, -8000.0f, 1200.0f,
};

f32 lbl_1_data_390[3] = {
    -280.0f, 120.0f, 600.0f,
};

f32 lbl_1_data_39C[3] = {
    1.0f, 580.0f, -2000.0f,
};

f32 lbl_1_data_3A8[15] = {
    -160.0f, 270.0f, 350.0f,
    160.0f, 270.0f, 350.0f,
    -210.0f, 270.0f, 350.0f,
    0.0f, 270.0f, 350.0f,
    210.0f, 270.0f, 350.0f,
};

f32 lbl_1_data_3E4[13] = {
    100.0f, 110.0f, 140.0f, 140.0f, 115.0f, 130.0f,
    100.0f, 105.0f, 100.0f, 110.0f, 110.0f, 110.0f,
    110.0f,
};

s32 lbl_1_data_418[10][4] = {
    { 504, 512, 506, 505 },
    { 484, 492, 486, 485 },
    { 604, 612, 606, 605 },
    { 684, 692, 686, 685 },
    { 644, 652, 646, 645 },
    { 444, 452, 446, 445 },
    { 664, 672, 666, 665 },
    { 464, 472, 466, 465 },
    { 624, 632, 626, 625 },
    { 564, 572, 566, 565 },
};


int _prolog(void)
{
    const VoidFunc *ctor = _ctors;

    while (*ctor != NULL) {
        (*ctor)();
        ctor++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const VoidFunc *dtor = _dtors;

    while (*dtor != NULL) {
        (*dtor)();
        dtor++;
    }
}

void fn_1_A0(s8 *arg0, u8 *arg1, s8 arg2)
{
    s32 i = 0;
    while (i < arg2 - 1) {
        s32 j = i + 1;
        while (j < arg2) {
            if (arg0[i] > arg0[j]) {
                s8 tmp = arg0[i];
                arg0[i] = arg0[j];
                arg0[j] = tmp;
                if (arg1) {
                    u8 t = arg1[i];
                    arg1[i] = arg1[j];
                    arg1[j] = t;
                }
            }
            j++;
        }
        i++;
    }
}

void fn_1_140(u8 *arg0, s32 arg1)
{
    u8 local[0xF4];
    s32 i = arg1;
    s32 j;
    s32 pick;

    while (i-- != 0) {
        local[i] = arg0[i];
    }
    i = arg1;
    while (i-- != 0) {
        if (i != 0) {
            pick = frandmod(i + 1);
        } else {
            pick = 0;
        }
        arg0[i] = local[pick];
        for (j = 0; j <= i; j++) {
            if (j != pick && j > pick) {
                local[j - 1] = local[j];
            }
        }
    }
}

void fn_1_204(s32 arg0)
{
    while (arg0-- != 0) {
        HuPrcVSleep();
    }
}

void fn_1_240(s32 arg0, s32 arg1)
{
    WipeCreate(1, arg0, arg1);
    while ((u8)WipeCheckEnd() != 0) {
        HuPrcVSleep();
    }
}

void fn_1_294(s32 arg0, s32 arg1)
{
    WipeCreate(2, arg0, arg1);
    while ((u8)WipeCheckEnd() != 0) {
        HuPrcVSleep();
    }
}

static const char lbl_1_data_4B8[] = "Load Time:%d\n";

void fn_1_2E8(void)
{
    // NONMATCHING: the mulhwu/srwi tick conversion uses different registers in MWCC
    // (it also emits an extra li for the 64-bit intermediate).
    u32 start;
    s32 stat;

    lbl_1_bss_0->loadState = 0;
    start = OSGetTick();
    stat = HuDataDirReadAsync(mgDataTbl[lbl_1_bss_0->mgNo].dataDir);
    while (HuDataGetAsyncStat(stat) == 0) {
        HuPrcVSleep();
    }
    lbl_1_bss_0->loadState = 1;
    OSReport(lbl_1_data_4B8,
             (OSGetTick() - start) / (s32)((u64)((u64)(u32)(*(u32 *)0x800000f8 >> 2) * 0x10624DD3u) >> 38));
    HuPrcEnd();
    while (1) {
        HuPrcVSleep();
    }
}

void fn_1_3C4(s32 arg0)
{
    // NONMATCHING: MWCC saves r31 (frame -0x20) although the original keeps every
    // value dead across its calls and needs only frame -0x10.
    lbl_1_bss_0->mgNo = arg0;
    HuDataDirClose(0x980000);
    HuDataDirClose(0x270000);
    CharModelDataClose(-1);
    if (fn_1_8B4(4) == 0) {
        HuPrcChildCreate(fn_1_2E8, 0xc8, 0x3000, 0, HuPrcCurrentGet());
    } else {
        lbl_1_bss_0->loadState = 1;
    }
}

s16 fn_1_458(void)
{
    return lbl_1_bss_0->winId;
}

void fn_1_46C(void)
{
    // NONMATCHING: MWCC reloads lbl_1_bss_0->winId after the call; the original caches
    // the window id in a register before the fn_1_8B4 test.
    if (fn_1_8B4(0x20) == 0) {
        fn_1_87C(0x20);
        HuWinExOpen(lbl_1_bss_0->winId);
    }
}

void fn_1_4B4(void)
{
    // NONMATCHING: the original caches unk6 in r31 before the fn_1_8B4 test;
    // MWCC reloads lbl_1_bss_0->winId after the call.
    if (fn_1_8B4(0x20) != 0) {
        fn_1_898(0x20);
        HuWinExClose(lbl_1_bss_0->winId);
        HuWinHomeClear(lbl_1_bss_0->winId);
    }
}

s32 fn_1_510(void)
{
    return lbl_1_bss_4->maxScore;
}

s32 fn_1_528(void)
{
    // NONMATCHING: the original keeps each array index in a separate register and
    // uses indexed addressing (lbzx); MWCC folds the +1/+0x36 into the displacement.
    return mgDataTbl[((s8 *)lbl_1_bss_4)[((s8 *)lbl_1_bss_4)[((s8 *)lbl_1_bss_4)[0] + 1] * 6 + 0x36]].nameMes;
}

u8 fn_1_588(void)
{
    return lbl_1_bss_0->winner;
}

u8 fn_1_59C(void)
{
    return lbl_1_bss_4->gameType;
}

s32 fn_1_5B0(s32 arg0)
{
    return (s8)lbl_1_bss_0->player[arg0].playerNo;
}

void fn_1_5D0(s32 arg0, s32 arg1)
{
    omVibrate(lbl_1_bss_0->player[arg0].padNo, (s16)arg1, 4, 2);
}

u8 fn_1_624(void)
{
    return lbl_1_bss_4->winner;
}

s32 fn_1_638(void)
{
    return lbl_1_bss_0->unknownE00;
}

s32 fn_1_64C(s32 *place, s32 *tieNum, s32 *score)
{
    s32 tie;
    s32 tieIdx;
    s32 i;
    s32 j;

    tieIdx = 0;
    if ((s8)lbl_1_bss_4->gameType != 0) {
        return 0;
    }
    tie = 0;
    for (i = 0; i < lbl_1_bss_4->teamCount; i++) {
        for (j = i + 1; j < lbl_1_bss_4->teamCount; j++) {
            if (lbl_1_bss_4->score[i] == lbl_1_bss_4->score[j]) {
                tieIdx = i;
                tie++;
            }
        }
    }
    if (tie == 0) {
        return 0;
    }
    if (tieNum == NULL || place == NULL) {
        return 1;
    }
    *tieNum = tie;
    if (*tieNum == 1) {
        *tieNum += 1;
        *place = 1;
        for (i = 0; i < lbl_1_bss_4->teamCount; i++) {
            if (tieIdx != i && lbl_1_bss_4->score[tieIdx] < lbl_1_bss_4->score[i]) {
                *place += 1;
            }
        }
        if (score != NULL) {
            *score = lbl_1_bss_4->score[tieIdx];
        }
    } else {
        *place = 2;
        if (score != NULL) {
            if (lbl_1_bss_4->winner == 0) {
                *score = lbl_1_bss_4->score[1];
            } else {
                *score = lbl_1_bss_4->score[0];
            }
        }
    }
    return 1;
}

void fn_1_87C(u32 arg0)
{
    lbl_1_bss_0->flag2 |= arg0;
}

void fn_1_898(u32 arg0)
{
    lbl_1_bss_0->flag2 &= ~arg0;
}

u32 fn_1_8B4(u32 arg0)
{
    return lbl_1_bss_0->flag2 & arg0;
}

void fn_1_8CC(u32 arg0)
{
    lbl_1_bss_4->flag |= arg0;
}

void fn_1_8EC(u32 arg0)
{
    lbl_1_bss_4->flag &= ~arg0;
}

u32 fn_1_90C(u32 arg0)
{
    return lbl_1_bss_4->flag & arg0;
}

void fn_1_924(u32 arg0)
{
    lbl_1_bss_0->flag |= arg0;
}

void fn_1_940(u32 arg0)
{
    lbl_1_bss_0->flag &= ~arg0;
}

u32 fn_1_95C(u32 arg0)
{
    return lbl_1_bss_0->flag & arg0;
}

void fn_1_974(void *arg0, s32 arg1)
{
    // NONMATCHING: the original mixes a single-precision load for the 24.0f factor
    // with a double-precision divide for the 500.0f divisor (lfd/fdiv); MWCC keeps
    // the whole expression in one precision.
    HuAudFXPlayPan(lbl_1_data_418[*(s16 *)((u8 *)arg0 + 0x1a)][arg1],
                   0x40 + (s32)(24.0f * (*(f32 *)((u8 *)arg0 + 0x28) / 500.0f)));
}

void fn_1_9FC(void)
{
    // NONMATCHING: the original keeps unkE04 in r29 across the polling loop (frame
    // -0x30 with 3 saved registers); MWCC needs only 2 saved registers (-0x20).
    s32 key[4];
    s32 i;
    s32 n;

    HuWinDisablePlayerSet(lbl_1_bss_0->winId, 0xf);
    do {
        i = 4;
        while (i-- != 0) {
            key[i] = -1;
            if (*(s16 *)((u8 *)GwPlayerConf + i * 0xa + 8) != 0) {
                key[i] = 0x100;
            }
        }
        HuWinComKeyWait(key[0], key[1], key[2], key[3], 0xa);
        n = 0x1e;
        while (n-- != 0) {
            HuPrcVSleep();
        }
    } while ((lbl_1_bss_0->flag & lbl_1_bss_0->keyWaitMask) == 0);
    HuWinDisablePlayerReset(lbl_1_bss_0->winId, 0xf);
    HuWinComKeyReset();
    HuPrcEnd();
}

void fn_1_B18(s32 arg0)
{
    lbl_1_bss_0->keyWaitMask = arg0;
    HuPrcChildCreate(fn_1_9FC, 0xc8, 0x3000, 0, HuPrcCurrentGet());
}

void ObjectSetup(void)
{
    GwSystem.subGameNo = 0;
    _SetFlag(0xb);
    HuPrcChildCreate(fn_1_BC0, 0xc8, 0x3000, 0, HuPrcCurrentGet());
}

void fn_1_BC0(void)
{
    s32 cont;
    s32 loop;
    OMOVLHIS *his;

    his = omOvlHisGet(0);
    fn_1_85C4();
    loop = 1;
    while (loop != 0) {
        cont = 1;
        switch (his->evtno) {
            case 0:
                fn_1_9984();
                loop = 1;
                break;
            case 1:
                fn_1_A524();
                if ((lbl_1_bss_0->flag2 & 8) == 0) {
                    fn_1_B2D8();
                    if (lbl_1_bss_4->winner >= 0) {
                        lbl_1_bss_0->flag2 = lbl_1_bss_0->flag2 | 0x200;
                    }
                    if ((lbl_1_bss_0->flag2 & 0x200) != 0) {
                        fn_1_BE50();
                    }
                }
                if ((lbl_1_bss_4->flag & 2) != 0) {
                    cont = 0;
                }
                break;
            case 2:
                fn_1_112B8();
                cont = 0;
                break;
            case 3:
                if ((lbl_1_bss_0->flag2 & 2) != 0) {
                    fn_1_11F2C();
                }
                cont = 0;
                loop = 0;
                break;
        }
        if (cont != 0) {
            fn_1_C638();
        }
    }
    fn_1_9844();
    omOvlReturnEx(1, 1);
    HuPrcEnd();
}

void fn_1_D40(s32 arg0)
{
    OMOVLHIS *his = omOvlHisGet(0);
    omOvlHisChg(0, his->ovl, arg0, his->stat);
}

void fn_1_D88(void)
{
    HU3DMODELID model;
    s32 i;

    lbl_1_bss_0->mdl[0] = Hu3DModelCreateData(0x980009);
    lbl_1_bss_0->mdl[1] = Hu3DModelCreateData(0x98000A);
    lbl_1_bss_0->mdl[3] = Hu3DModelCreateData(0x98000D);
    lbl_1_bss_0->mdl[2] = Hu3DModelCreateData(0x980008);
    lbl_1_bss_0->mdl[4] = Hu3DModelCreateData(0x98000B);
    lbl_1_bss_0->mdl[5] = Hu3DModelCreateData(0x98000C);
    lbl_1_bss_0->mdl[7] = Hu3DModelCreateData(0x98000F);
    lbl_1_bss_0->mdl[8] = Hu3DModelCreateData(0x98000F);
    lbl_1_bss_0->mdl[37] = Hu3DModelCreateData(0x980013);
    lbl_1_bss_0->mdl[54] = Hu3DModelCreateData(0x980019);
    lbl_1_bss_0->mdl[38] = Hu3DModelCreateData(0x980014);
    lbl_1_bss_0->mdl[39] = Hu3DModelCreateData(0x980015);
    lbl_1_bss_0->mdl[40] = Hu3DModelCreateData(0x980015);
    lbl_1_bss_0->mdl[41] = Hu3DModelCreateData(0x980014);
    lbl_1_bss_0->mdl[42] = Hu3DModelCreateData(0x980015);
    lbl_1_bss_0->mdl[43] = Hu3DModelCreateData(0x980014);
    lbl_1_bss_0->mdl[44] = Hu3DModelCreateData(0x980015);
    lbl_1_bss_0->mdl[45] = Hu3DModelCreateData(0x980014);
    lbl_1_bss_0->mdl[46] = Hu3DModelCreateData(0x980015);
    Hu3DModelLayerSet(lbl_1_bss_0->mdl[0], 2);
    Hu3DModelLayerSet(lbl_1_bss_0->mdl[1], 2);
    Hu3DModelLayerSet(lbl_1_bss_0->mdl[3], 3);
    Hu3DModelLayerSet(lbl_1_bss_0->mdl[2], 0);
    Hu3DModelLayerSet(lbl_1_bss_0->mdl[4], 0);
    Hu3DModelLayerSet(lbl_1_bss_0->mdl[5], 3);
    Hu3DModelLayerSet(lbl_1_bss_0->mdl[7], 0);
    Hu3DModelLayerSet(lbl_1_bss_0->mdl[8], 0);
    Hu3DModelLayerSet(lbl_1_bss_0->mdl[37], 0);
    Hu3DModelLayerSet(lbl_1_bss_0->mdl[54], 5);
    Hu3DModelAttrSet(lbl_1_bss_0->mdl[3], 0x40000002);
    Hu3DModelAttrSet(lbl_1_bss_0->mdl[3], 1);
    Hu3DModelAttrSet(lbl_1_bss_0->mdl[2], 0x40000001);
    Hu3DModelAttrSet(lbl_1_bss_0->mdl[4], 0x40000001);
    Hu3DModelAttrSet(lbl_1_bss_0->mdl[5], 0x40000002);
    Hu3DModelAttrSet(lbl_1_bss_0->mdl[7], 1);
    Hu3DModelAttrSet(lbl_1_bss_0->mdl[8], 1);
    Hu3DModelAttrSet(lbl_1_bss_0->mdl[37], 1);
    Hu3DModelAttrSet(lbl_1_bss_0->mdl[54], 1);
    Hu3DModelCameraSet(lbl_1_bss_0->mdl[0], 0x10);
    Hu3DModelCameraSet(lbl_1_bss_0->mdl[1], 0x10);
    Hu3DModelCameraSet(lbl_1_bss_0->mdl[3], 0x10);
    Hu3DModelCameraSet(lbl_1_bss_0->mdl[2], 0x10);
    Hu3DModelCameraSet(lbl_1_bss_0->mdl[4], 0x10);
    Hu3DModelCameraSet(lbl_1_bss_0->mdl[5], 0x10);
    Hu3DModelCameraSet(lbl_1_bss_0->mdl[7], 0x10);
    Hu3DModelCameraSet(lbl_1_bss_0->mdl[8], 0x10);
    Hu3DModelCameraSet(lbl_1_bss_0->mdl[37], 0x10);
    Hu3DModelCameraSet(lbl_1_bss_0->mdl[54], 0x10);
    i = 9;
    while (i-- != 0) {
        Hu3DModelLayerSet(lbl_1_bss_0->mdl[i + 0x26], 0);
        Hu3DModelAttrSet(lbl_1_bss_0->mdl[i + 0x26], 1);
        Hu3DModelCameraSet(lbl_1_bss_0->mdl[i + 0x26], 0x10);
    }
    i = 3;
    while (i-- != 0) {
        model = Hu3DModelCreateData(0x98001A);
        lbl_1_bss_0->mdl[i + 0x37] = model;
        Hu3DModelLayerSet(model, 5);
        Hu3DModelAttrSet(model, 1);
        Hu3DModelAttrSet(model, 0x40000082);
        Hu3DModelCameraSet(model, 0x10);
    }
    lbl_1_bss_0->mdl[0x3A] = Hu3DModelCreateData(0x98001B);
    lbl_1_bss_0->mdl[0x3B] = Hu3DModelCreateData(0x98001C);
    lbl_1_bss_0->mdl[0x3C] = Hu3DModelCreateData(0x98001D);
    i = 3;
    while (i-- != 0) {
        model = lbl_1_bss_0->mdl[i + 0x3A];
        Hu3DModelLayerSet(model, 5);
        Hu3DModelAttrSet(model, 1);
        Hu3DModelCameraSet(model, 0x10);
    }
    i = 7;
    while (i-- != 0) {
        model = Hu3DModelCreateData(0x980010);
        lbl_1_bss_0->mdl[i + 9] = model;
        lbl_1_bss_0->item[i].modelId = model;
        Hu3DModelLayerSet(model, 4);
        Hu3DModelAttrSet(model, 0x40000002);
        Hu3DModelAttrSet(model, 1);
        Hu3DModelCameraSet(model, 0x10);
    }
    i = 10;
    while (i-- != 0) {
        model = Hu3DModelCreateData(0x980011);
        lbl_1_bss_0->mdl[i + 0x10] = model;
        Hu3DModelLayerSet(model, 4);
        Hu3DModelAttrSet(model, 0x40000003);
        Hu3DModelAttrSet(model, 1);
        Hu3DModelCameraSet(model, 0x10);
    }
    model = Hu3DModelCreateData(0x98000E);
    lbl_1_bss_0->mdlItem = model;
    Hu3DModelLayerSet(model, 4);
    Hu3DModelAttrSet(model, 1);
    Hu3DModelAttrSet(model, 0x40000003);
    Hu3DReflectNoSet(2);
    Hu3DModelReflectTypeSet(model, -1);
    Hu3DModelCameraSet(model, 0x10);
    i = 7;
    while (i-- != 0) {
        model = Hu3DModelCreateData(0x980002);
        lbl_1_bss_0->parMan[i].id = model;
        lbl_1_bss_0->mdl[i + 0x2F] = model;
        lbl_1_bss_0->parMan[i].motA = Hu3DJointMotion(model, HuDataSelHeapReadNum(0x980004, HU_MEMNUM_OVL, HEAP_MODEL));
        lbl_1_bss_0->parMan[i].motB = Hu3DJointMotion(model, HuDataSelHeapReadNum(0x980007, HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DMotionSet(model, lbl_1_bss_0->parMan[i].motA);
        Hu3DModelAttrSet(model, 0x40000001);
        Hu3DModelAttrSet(model, 1);
        Hu3DModelLayerSet(model, 4);
        Hu3DModelCameraSet(model, 0x10);
    }
    i = 4;
    while (i-- != 0) {
        model = Hu3DModelCreateData(0x980017);
        lbl_1_bss_0->mdl[i + 0x1E] = model;
        Hu3DModelLayerSet(model, 5);
        Hu3DModelAttrSet(model, 0x40000001);
        Hu3DModelAttrSet(model, 1);
        Hu3DModelCameraSet(model, 0x10);
    }
    i = 3;
    while (i-- != 0) {
        model = Hu3DModelCreateData(0x980012);
        lbl_1_bss_0->mdl[i + 0x22] = model;
        Hu3DModelLayerSet(model, 5);
        Hu3DModelAttrSet(model, 0x40000002);
        Hu3DModelAttrSet(model, 1);
        Hu3DModelCameraSet(model, 0x10);
    }
    lbl_1_bss_0->mdlCam0 = Hu3DModelCameraCreate(Hu3DMotionCreateData(0x98001E), 0x10);
    lbl_1_bss_0->mdlCam1 = Hu3DModelCameraCreate(Hu3DMotionCreateData(0x98001F), 0x10);
    Hu3DModelAttrSet(lbl_1_bss_0->mdlCam0, 0x40000002);
    Hu3DModelAttrSet(lbl_1_bss_0->mdlCam1, 0x40000002);
    Hu3DCameraMotionOff(lbl_1_bss_0->mdlCam0);
    Hu3DCameraMotionOff(lbl_1_bss_0->mdlCam1);
}

void fn_1_1A24(void *arg0)
{
    HuPrcChildCreate(arg0, 0x64, 0x1000, 0, HuPrcCurrentGet());
    HuPrcVSleep();
}

s32 fn_1_1A64(s16 camNo, s32 time, s16 mode)
{
    HSFMOTION *motion;

    if (lbl_1_bss_0->camNo == camNo) {
        return 0;
    }
    lbl_1_bss_0->camMotionNo = camNo;
    if (time != 0) {
        CamMotionEx2(lbl_1_bss_0->mdl[camNo], 0x10, (f32)time, mode);
    } else {
        motion = Hu3DMotion[Hu3DData[lbl_1_bss_0->mdl[camNo]].motId].hsf->motion;
        if (motion->maxTime < 9.0f) {
            CamMotionEx2(lbl_1_bss_0->mdl[camNo], 0x10, motion->maxTime, mode);
        } else {
            CamMotionEx2(lbl_1_bss_0->mdl[camNo], 0x10, 1.0f, mode);
        }
    }
    lbl_1_bss_0->camMotionNo = -1;
    lbl_1_bss_0->camNo = camNo;
    return 1;
}

void fn_1_1C08(void)
{
    HSFMOTION *motion;
    s16 mode;
    s16 camNo;
    s32 time;

    mode = lbl_1_bss_0->timerType;
    time = (s32)lbl_1_bss_0->camMoveRate;
    camNo = lbl_1_bss_0->camMotionNo;
    if (lbl_1_bss_0->camNo != camNo) {
        lbl_1_bss_0->camMotionNo = camNo;
        if (time != 0) {
            CamMotionEx2(lbl_1_bss_0->mdl[camNo], 0x10, (f32)time, mode);
        } else {
            motion = Hu3DMotion[Hu3DData[lbl_1_bss_0->mdl[camNo]].motId].hsf->motion;
            if (motion->maxTime < 9.0f) {
                CamMotionEx2(lbl_1_bss_0->mdl[camNo], 0x10, motion->maxTime, mode);
            } else {
                CamMotionEx2(lbl_1_bss_0->mdl[camNo], 0x10, 1.0f, mode);
            }
        }
        lbl_1_bss_0->camMotionNo = -1;
        lbl_1_bss_0->camNo = camNo;
    }
    lbl_1_bss_0->camMotionNo = -1;
    HuPrcEnd();
}

void fn_1_1DE8(s16 arg0, f32 arg1, s32 arg2)
{
    lbl_1_bss_0->camMotionNo = arg0;
    lbl_1_bss_0->camMoveRate = arg1;
    lbl_1_bss_0->timerType = (s16)arg2;
    HuPrcChildCreate(fn_1_1C08, 0x64, 0x1000, 0, HuPrcCurrentGet());
    HuPrcVSleep();
}

s32 fn_1_1E74(void)
{
    return lbl_1_bss_0->camMotionNo == -1;
}

void fn_1_1EA0(void)
{
    CamMotionExPath(0x10, lbl_1_bss_0->camPath, lbl_1_bss_0->camPathNum);
    lbl_1_bss_0->flag2 |= 0x100;
    HuPrcEnd();
}

void fn_1_1F00(void)
{
    // NONMATCHING: MWCC emits clrlwi r0,9 instead of the original's
    // rlwinm r0,0,24,22 for this 23-bit mask.
    lbl_1_bss_0->flag2 &= 0x7FFFFF;
    HuPrcChildCreate(fn_1_1EA0, 0x64, 0x1000, 0, HuPrcCurrentGet());
    HuPrcVSleep();
}

void fn_1_1F58(void)
{
    CAMMOTIONWORK *tail = &lbl_1_bss_0->camPath[lbl_1_bss_0->camPathNum - 1];
    CamMotionExPath(0x10, lbl_1_bss_0->camPath, lbl_1_bss_0->camPathNum);
    Hu3DCameraPosSetV(0x10, &tail->pos, &tail->up, &tail->target);
    lbl_1_bss_0->flag2 |= 0x100;
    HuPrcEnd();
}

void fn_1_2000(void)
{
    lbl_1_bss_0->flag2 &= ~0x100;
    HuPrcChildCreate(fn_1_1F58, 0x64, 0x1000, 0, HuPrcCurrentGet());
    HuPrcVSleep();
}

void fn_1_2058(void)
{
    HuAudJinglePlay(0x85);
    HuPrcEnd();
}

void fn_1_2080(void)
{
    HuPrcChildCreate(fn_1_2058, 0x64, 0x1000, 0, HuPrcCurrentGet());
}

void fn_1_20BC(void)
{
    s32 i;

    HuAudJinglePlay(0x86);
    i = 0xd2;
    while (i-- != 0) {
        HuPrcVSleep();
    }
    if (lbl_1_bss_0->bgmId < 0) {
        lbl_1_bss_0->bgmId = HuAudBGMPlay(0x23);
    }
    HuPrcEnd();
}

void fn_1_2134(void)
{
    HuPrcChildCreate(fn_1_20BC, 0x64, 0x1000, 0, HuPrcCurrentGet());
}

void fn_1_2170(void)
{
    s32 timer[4];
    s32 state[4];
    f32 y[4];
    f32 speed;
    s32 se;
    s32 done;
    s32 i;
    M505_PLAYER *p;

    se = 0;
    speed = 10.0f * 140.0f;
    i = 4;
    while (i-- != 0) {
        y[i] = (f32)(frandmod(0x3E8) + 0x1D4C);
        state[i] = 0;
        p = &lbl_1_bss_0->player[i];
        p->unk24 = 0.0f;
        p->unk20 = 0.0f;
        Hu3DModelPosSet(p->modelId, p->pos.x, y[i], p->pos.z);
        Hu3DModelAttrReset(p->modelId, 1);
        if (p->motion != 5) {
            CharMotionShiftSet(p->charNo, p->mot5, 0.0f, 15.0f, 0);
            p->motion = 5;
        }
    }
    do {
        done = 0;
        i = 4;
        while (i-- != 0) {
            y[i] -= speed / 100.0f;
            p = &lbl_1_bss_0->player[i];
            switch (state[i]) {
                case 0:
                    if (y[i] > p->pos.y) {
                        Hu3DModelPosSet(p->modelId, p->pos.x, y[i], p->pos.z);
                    } else {
                        state[i] = 1;
                        Hu3DModelPosSet(p->modelId, p->pos.x, p->pos.y, p->pos.z);
                        if (p->motion != 6) {
                            CharMotionShiftSet(p->charNo, p->mot6, 0.0f, 15.0f, 0);
                            p->motion = 6;
                        }
                        Hu3DModelAttrReset(p->modelId, 0x40000002);
                        omVibrate(p->playerNo, 0x12, 4, 2);
                    }
                    break;
                case 1:
                    if (Hu3DMotionEndCheck(p->modelId) != 0) {
                        if (se == 0) {
                            HuAudFXPlayPan(0x4FF, 0x40);
                            se = 1;
                        }
                        if (p->motion != 7) {
                            CharMotionShiftSet(p->charNo, p->mot7, 0.0f, 15.0f, 0x40000001);
                            p->motion = 7;
                        }
                        Hu3DModelAttrReset(p->modelId, 0x40000002);
                        timer[i] = 0;
                        state[i] = 2;
                    }
                    break;
                case 2:
                    timer[i] += 1;
                    if (timer[i] == 0x78) {
                        if (p->motion != 8) {
                            CharMotionShiftSet(p->charNo, p->mot8, 0.0f, 15.0f, 0);
                            p->motion = 8;
                        }
                        Hu3DModelAttrReset(p->modelId, 0x40000002);
                        state[i] = 3;
                    }
                    break;
                case 3:
                    if (Hu3DMotionEndCheck(p->modelId) != 0) {
                        if (p->motion != 0) {
                            CharMotionShiftSet(p->charNo, p->mot0, 0.0f, 15.0f, 0x40000001);
                            p->motion = 0;
                        }
                        Hu3DModelAttrReset(p->modelId, 0x40000002);
                        state[i] = 4;
                    }
                    break;
                case 4:
                    done += 1;
                    break;
            }
        }
        if (done != 4) {
            if (speed != 60.0f * 140.0f) {
                speed += 140.0f;
            }
            HuPrcVSleep();
        }
    } while (done != 4);
    lbl_1_bss_0->flag2 = lbl_1_bss_0->flag2 | 0x40;
    HuPrcEnd();
}

void fn_1_2628(void)
{
    lbl_1_bss_0->flag2 &= ~0x40;
    HuPrcChildCreate(fn_1_2170, 0x64, 0x1000, 0, HuPrcCurrentGet());
}

void fn_1_267C(void)
{
    Vec diff;
    Vec step;
    f32 f;
    s32 i;

    i = 0;
    while (lbl_1_bss_0->camMoveSpeed != i) {
        f = (f32)sin(3.141592653589793 * (f64)(f32)sin(3.141592653589793 * (f64)((f32)i / (f32)lbl_1_bss_0->camMoveSpeed) * 0.5) * 0.5);
        PSVECSubtract(&lbl_1_bss_0->posFrom, &lbl_1_bss_0->posTo, &diff);
        PSVECScale(&diff, &step, f);
        PSVECAdd(&lbl_1_bss_0->posTo, &step, &lbl_1_bss_0->pos);
        Hu3DModelPosSet(lbl_1_bss_0->mdlPodium, lbl_1_bss_0->pos.x, lbl_1_bss_0->pos.y, lbl_1_bss_0->pos.z);
        HuPrcVSleep();
        i++;
    }
    lbl_1_bss_0->pos = lbl_1_bss_0->posFrom;
    Hu3DModelPosSet(lbl_1_bss_0->mdlPodium, lbl_1_bss_0->pos.x, lbl_1_bss_0->pos.y, lbl_1_bss_0->pos.z);
    lbl_1_bss_0->flag2 = lbl_1_bss_0->flag2 | 0x80;
    HuPrcEnd();
}

void fn_1_28BC(s32 arg0, s16 arg1)
{
    // NONMATCHING: 17.57%. MWCC keeps arg0 in a callee-saved register (r31) and reorders the global copies,
    // while the original stores arg0 on the stack and keeps the table pointer in r5.
    const f32 *tbl = &lbl_1_data_60[arg0 * 3];

    lbl_1_bss_0->posTo = lbl_1_bss_0->pos;
    lbl_1_bss_0->posFrom.x = tbl[0];
    lbl_1_bss_0->posFrom.y = tbl[1];
    lbl_1_bss_0->posFrom.z = tbl[2];
    lbl_1_bss_0->camMoveSpeed = arg1;
    lbl_1_bss_0->flag2 &= ~0x80;
    HuPrcChildCreate(fn_1_267C, 0x64, 0x1000, 0, HuPrcCurrentGet());
}

void fn_1_2994(void)
{
    u8 tmp[21];
    u8 order[21];
    Vec p;
    Vec q;
    f32 dy;
    f32 f;
    s32 timer;
    s32 hide;
    s32 count;
    s32 cur;
    s16 pick;
    s32 i;
    s32 j;
    s8 n;
    u8 num;

    cur = -1;
    dy = 0.0f;
    hide = 0;
    timer = 0;
    i = lbl_1_bss_4->mgCount;
    while (i-- != 0) {
        order[i] = i;
    }
    num = lbl_1_bss_4->mgCount;
    n = num;
    while (n-- != 0) {
        tmp[n] = order[n];
    }
    n = num;
    while (n-- != 0) {
        if (n != 0) {
            pick = frandmod(n + 1);
        } else {
            pick = 0;
        }
        order[n] = tmp[pick];
        for (j = 0; j <= n; j++) {
            if (j != pick && j > pick) {
                tmp[j - 1] = tmp[j];
            }
        }
    }
    if (lbl_1_bss_0->animId >= 0) {
        Hu3DAnimKill(lbl_1_bss_0->animId);
    }
    lbl_1_bss_0->animId = Hu3DAnimCreate(lbl_1_bss_0->mgAnim[0], lbl_1_bss_0->mdl[8], "S3TC_mg_movie");
    while (TRUE) {
        if ((lbl_1_bss_0->flag2 & 0x800) != 0) {
            lbl_1_bss_0->flag2 = lbl_1_bss_0->flag2 & ~0x800;
            if (lbl_1_bss_4->mgCount >= 10) {
                cur = frandmod(lbl_1_bss_4->mgCount) + 0x1E;
            } else if (lbl_1_bss_4->mgCount >= 7) {
                cur = frandmod(lbl_1_bss_4->mgCount) + 0x14;
            } else if (lbl_1_bss_4->mgCount >= 4) {
                cur = frandmod(lbl_1_bss_4->mgCount) + 0xF;
            } else {
                cur = frandmod(lbl_1_bss_4->mgCount) + 0xA;
            }
            hide = 0;
        }
        if (cur >= 0) {
            hide += 1;
            if (hide >= 3) {
                if (cur == 0) {
                    Hu3DModelAttrReset(lbl_1_bss_0->mdl[7], 1);
                    Hu3DModelAttrSet(lbl_1_bss_0->mdl[8], 1);
                    HuAudFXPlay(0x495);
                    lbl_1_bss_0->flag2 = lbl_1_bss_0->flag2 | 0x1000;
                } else {
                    HuAudFXPlay(0x494);
                    if (lbl_1_bss_0->animId >= 0) {
                        Hu3DAnimKill(lbl_1_bss_0->animId);
                    }
                    Hu3DAnimCreate(lbl_1_bss_0->mgAnim[(s8)order[cur % lbl_1_bss_4->mgCount]], lbl_1_bss_0->mdl[8],
                        "S3TC_mg_movie");
                    hide = 0;
                }
                cur -= 1;
            }
        }
        switch (lbl_1_bss_0->itemAnimState) {
            case 0:
                break;
            case 1:
                Hu3DModelAttrReset(lbl_1_bss_0->mdl[8], 1);
                HuAudFXPlay(0x496);
                dy = lbl_1_data_90[4] - lbl_1_data_90[1];
                q.x = lbl_1_data_90[0];
                q.y = lbl_1_data_90[1];
                q.z = lbl_1_data_90[2];
                count = 0;
                timer = 0;
                lbl_1_bss_0->itemAnimState = 3;
                break;
            case 2:
                dy = lbl_1_data_90[1] - lbl_1_data_90[4];
                q.x = lbl_1_data_90[3];
                q.y = lbl_1_data_90[4];
                q.z = lbl_1_data_90[5];
                count = 1;
                timer = 0;
                lbl_1_bss_0->itemAnimState = 3;
                break;
            case 3:
                f = (f32)sin(3.141592653589793 * (f64)(f32)sin(3.141592653589793 * (f64)((f32)timer / 60.0f) * 0.5) * 0.5);
                p.x = lbl_1_data_90[0];
                p.y = lbl_1_data_90[1];
                p.z = lbl_1_data_90[2];
                p.y += dy * f;
                Hu3DModelPosSet(lbl_1_bss_0->mdl[7], p.x, p.y, p.z);
                Hu3DModelPosSet(lbl_1_bss_0->mdl[8], p.x, p.y, p.z);
                timer += 1;
                if (timer > 0x3C) {
                    if (count != 0) {
                        Hu3DModelAttrSet(lbl_1_bss_0->mdl[7], 1);
                        Hu3DModelAttrSet(lbl_1_bss_0->mdl[8], 1);
                    }
                    lbl_1_bss_0->itemAnimState = 0;
                }
                break;
        }
        HuPrcVSleep();
    }
}

void fn_1_2F78(void)
{
    if ((lbl_1_bss_0->flag2 & 0x400) == 0) {
        Hu3DModelAttrSet(lbl_1_bss_0->mdl[7], 1);
        Hu3DModelScaleSet(lbl_1_bss_0->mdl[7], 3.8f, 3.8f, 3.0f);
        Hu3DModelRotSet(lbl_1_bss_0->mdl[7], 10.0f, 0.0f, 0.0f);
        Hu3DModelPosSet(lbl_1_bss_0->mdl[7], lbl_1_data_90[0], lbl_1_data_90[1], lbl_1_data_90[2]);
        Hu3DModelAttrSet(lbl_1_bss_0->mdl[8], 1);
        Hu3DModelScaleSet(lbl_1_bss_0->mdl[8], 3.8f, 3.8f, 3.0f);
        Hu3DModelRotSet(lbl_1_bss_0->mdl[8], 10.0f, 0.0f, 0.0f);
        Hu3DModelPosSet(lbl_1_bss_0->mdl[8], lbl_1_data_90[0], lbl_1_data_90[1], lbl_1_data_90[2]);
        lbl_1_bss_0->itemAnimState = 0;
        lbl_1_bss_0->flag2 = lbl_1_bss_0->flag2 | 0x400;
        HuPrcChildCreate(fn_1_2994, 0x64, 0x1000, 0, HuPrcCurrentGet());
    }
}

void fn_1_317C(s32 arg0)
{
    (void)arg0;
    lbl_1_bss_0->flag2 |= 0x800;
    lbl_1_bss_0->flag2 &= ~0x1000;
}

void fn_1_31B0(void)
{
    char *movie;
    s8 idx;

    idx = lbl_1_bss_4->mgTbl[lbl_1_bss_4->order[lbl_1_bss_4->entryNo]].mgIdx;
    if (idx + 0x1F5 == 0x207) {
        movie = mgDataTbl[idx].movie[lbl_1_bss_0->movieNo];
    } else {
        movie = mgDataTbl[idx].movie[0];
    }
    HuTHPSprCreateVol(movie, 1, 0, 0.0f);
    Hu3DModelMatHookSet(lbl_1_bss_0->mdl[7], HuTHPMatHook);
    OSReport("Movie = %s\n", movie);
}

s32 fn_1_32C0(void)
{
    return lbl_1_bss_0->itemAnimState == 0;
}

void fn_1_32DC(s32 arg0)
{
    if (lbl_1_bss_0->flag2 & 0x400) {
        lbl_1_bss_0->itemAnimState = arg0;
        while (lbl_1_bss_0->itemAnimState != 0) {
            HuPrcVSleep();
        }
        if (arg0 == 1) {
            lbl_1_bss_0->flag2 |= 0x800;
            lbl_1_bss_0->flag2 &= ~0x1000;
            while ((lbl_1_bss_0->flag2 & 0x1000) == 0) {
                HuPrcVSleep();
            }
        }
    }
}

void fn_1_33AC(void)
{
    u8 tmp[21];
    u8 last;
    u8 num;
    s16 pick;
    s32 j;
    s8 n;

    last = lbl_1_bss_4->order[lbl_1_bss_4->mgCount - 1];
    lbl_1_bss_4->entryNo = 0;
    num = lbl_1_bss_4->mgCount;
    n = num;
    while (n-- != 0) {
        tmp[n] = lbl_1_bss_4->order[n];
    }
    n = num;
    while (n-- != 0) {
        if (n != 0) {
            pick = frandmod(n + 1);
        } else {
            pick = 0;
        }
        lbl_1_bss_4->order[n] = tmp[pick];
        for (j = 0; j <= n; j++) {
            if (j != pick && j > pick) {
                tmp[j - 1] = tmp[j];
            }
        }
    }
    if (lbl_1_bss_4->order[0] == (s8)last) {
        lbl_1_bss_4->order[0] = lbl_1_bss_4->order[(s8)lbl_1_bss_4->mgCount / 2];
        lbl_1_bss_4->order[(s8)lbl_1_bss_4->mgCount / 2] = last;
    }
}

void fn_1_3560(void)
{
    OMOVLHIS *his;
    M505_PLAYER *p;
    s32 i;
    s8 n;

    n = lbl_1_bss_4->teamCount;
    while (n-- != 0) {
        lbl_1_bss_4->score[n] = 0;
    }
    lbl_1_bss_4->flag = 0;
    lbl_1_bss_4->entryNo = 0;
    lbl_1_bss_4->winner = -1;
    if ((lbl_1_bss_0->flag2 & 0x20) != 0) {
        lbl_1_bss_0->flag2 = lbl_1_bss_0->flag2 & ~0x20;
        HuWinExClose(lbl_1_bss_0->winId);
        HuWinHomeClear(lbl_1_bss_0->winId);
    }
    p = lbl_1_bss_0->player;
    lbl_1_bss_0->pos.x = lbl_1_data_60[0];
    lbl_1_bss_0->pos.y = lbl_1_data_60[1];
    lbl_1_bss_0->pos.z = lbl_1_data_60[2];
    Hu3DModelPosSet(lbl_1_bss_0->mdlPodium, lbl_1_bss_0->pos.x, lbl_1_bss_0->pos.y, lbl_1_bss_0->pos.z);
    Hu3DModelRotSet(lbl_1_bss_0->mdlPodium, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrReset(lbl_1_bss_0->mdlPodium, 1);
    for (i = 0; i < 4; i++) {
        p->pos.x = lbl_1_data_30[i * 3];
        p->pos.y = lbl_1_data_30[i * 3 + 1];
        p->pos.z = lbl_1_data_30[i * 3 + 2];
        if (p->motion != 0) {
            CharMotionShiftSet(p->charNo, p->mot0, 0.0f, 15.0f, 0x40000001);
            p->motion = 0;
        }
        Hu3DModelAttrSet(p->modelId, 1);
        Hu3DModelPosSet(p->modelId, p->pos.x, p->pos.y, p->pos.z);
        p++;
    }
    i = 4;
    while (i-- != 0) {
        Hu3DModelAttrSet(lbl_1_bss_0->mdl[i + 0x1E], 1);
    }
    his = omOvlHisGet(0);
    omOvlHisChg(0, his->ovl, 0, his->stat);
    HuTHPClose();
    HuPrcVSleep();
    while (HuTHPProcCheck() != 0) {
        HuPrcVSleep();
    }
}

void fn_1_3850(void)
{
    s32 idx[4];
    s32 score;
    s32 place;
    s32 count;
    s32 timer;
    s32 state;
    s32 done;
    s8 n;
    s32 i;
    M505_PLAYER *p;

    state = 0;
    done = 0;
    while (done == 0) {
        switch (state) {
            case 0:
                fn_1_64C(&place, &count, &score);
                count = 0;
                n = lbl_1_bss_4->teamCount;
                while (n-- != 0) {
                    if (lbl_1_bss_4->score[n] == score) {
                        idx[count] = n;
                        count += 1;
                    }
                }
                i = count;
                while (i-- != 0) {
                    p = &lbl_1_bss_0->player[idx[i]];
                    if (p->motion != 1) {
                        CharMotionShiftSet(p->charNo, p->mot1, 0.0f, 15.0f, 0x40000001);
                        p->motion = 1;
                    }
                }
                timer = 0x14;
                state = 1;
                break;
            case 1:
                i = count;
                while (i-- != 0) {
                    p = &lbl_1_bss_0->player[idx[i]];
                    p->pos.z = p->pos.z + 4.0f;
                    Hu3DModelPosSet(p->modelId, p->pos.x, p->pos.y, p->pos.z);
                }
                timer -= 1;
                if (timer == 0) {
                    state = 2;
                }
                break;
            case 2:
                i = count;
                while (i-- != 0) {
                    p = &lbl_1_bss_0->player[idx[i]];
                    if (p->motion != 0) {
                        CharMotionShiftSet(p->charNo, p->mot0, 0.0f, 15.0f, 0x40000001);
                        p->motion = 0;
                    }
                }
                done = 1;
                break;
        }
        HuPrcVSleep();
    }
    lbl_1_bss_0->flag2 = lbl_1_bss_0->flag2 | 0x2000;
    HuPrcEnd();
}

void fn_1_3AD0(void)
{
    lbl_1_bss_0->flag2 &= ~0x2000;
    HuPrcChildCreate(fn_1_3850, 0x64, 0x1000, 0, HuPrcCurrentGet());
}

void fn_1_3B24(void)
{
    f32 rot;

    rot = (f32)frandmod(0x8CA0) / 100.0f;
    Hu3DModelRotSet(lbl_1_bss_0->mdl[2], 0.0f, rot, 0.0f);
    while (TRUE) {
        rot = (f32)fmod((f64)(rot + 0.06f), 360.0);
        Hu3DModelRotSet(lbl_1_bss_0->mdl[2], 0.0f, rot, 0.0f);
        HuPrcVSleep();
    }
}

void fn_1_3C04(void)
{
    HuPrcChildCreate(fn_1_3B24, 0x64, 0x1000, 0, HuPrcCurrentGet());
}

void fn_1_3C40(void)
{
    Vec p;
    f32 f;
    f32 g;
    s32 i;
    s32 id;

    i = 4;
    while (i-- != 0) {
        Hu3DParManAttrSet((s16)lbl_1_bss_0->parManId[i], 1);
        lbl_1_bss_0->wheelAngle[i] = (s16)(-30.0f * (f32)i);
    }
    while ((lbl_1_bss_0->flag2 & 0x10000) != 0) {
        i = 4;
        while (i-- != 0) {
            id = lbl_1_bss_0->parManId[i];
            f = (f32)lbl_1_bss_0->wheelAngle[i] / (f32)lbl_1_bss_0->parManMaxTime;
            g = (f32)(2.0 * (3.141592653589793 * (f64)(3.0f * f)));
            p.x = (f32)(150.0 * ((f64)f * cos((f64)g)));
            p.y = lbl_1_data_B4[1] + (300.0f * f);
            p.z = (f32)(150.0 * ((f64)f * sin((f64)g)));
            Hu3DParManPosSet((s16)id, p.x, p.y, p.z);
            if (lbl_1_bss_0->wheelAngle[i] > 0) {
                lbl_1_bss_0->wheelAngle[i] = lbl_1_bss_0->wheelAngle[i] % lbl_1_bss_0->parManMaxTime;
                if (lbl_1_bss_0->wheelAngle[i] == 0) {
                    lbl_1_bss_0->wheelAngle[i] = 0xA;
                }
            } else if (lbl_1_bss_0->wheelAngle[i] == 0) {
                Hu3DParManAttrReset((s16)lbl_1_bss_0->parManId[i], 1);
                lbl_1_bss_0->wheelAngle[i] = 0xA;
            }
            lbl_1_bss_0->wheelAngle[i] = lbl_1_bss_0->wheelAngle[i] + 1;
        }
        HuPrcVSleep();
    }
    i = 4;
    while (i-- != 0) {
        Hu3DParManAttrSet((s16)lbl_1_bss_0->parManId[i], 1);
    }
    Hu3DParManAttrReset((s16)lbl_1_bss_0->parManId[0], 1);
    while ((lbl_1_bss_0->flag2 & 0x80000) != 0) {
        p = lbl_1_bss_0->itemPos2;
        Hu3DParManPosSet((s16)lbl_1_bss_0->parManId[0], p.x, p.y, p.z);
        HuPrcVSleep();
    }
    Hu3DParManAttrSet((s16)lbl_1_bss_0->parManId[0], 1);
    HuPrcEnd();
}

void fn_1_4090(void)
{
    lbl_1_bss_0->parManMaxTime = (s16)Hu3DMotionMaxTimeGet(lbl_1_bss_0->mdl[16]);
    lbl_1_bss_0->flag2 |= 0x00090000;
    HuPrcChildCreate(fn_1_3C40, 0x64, 0x1000, 0, HuPrcCurrentGet());
}

void fn_1_4114(void)
{
    Vec d;
    Vec p;
    Vec q;
    Vec t;
    f32 ang[3];
    f32 f;
    f32 f24;
    f32 f31;
    f32 w;
    f32 ph;
    f32 k;
    f32 se25;
    HU3DMODELID m;
    ANIMDATA *anim;
    s16 grp;
    s32 flag;
    s32 seId;
    s32 seId2;
    s32 state;
    s32 timer;
    s32 timer2;
    s32 idx;
    s32 wave;
    s32 cnt;
    s32 cnt2;
    s32 i;
    s32 id;
    s8 n;
    M505_ITEM *it;
    M505_PLAYER *pl;
    f32 h;
    f32 h2;

#ifdef TARGET_PC
    seId = 0;
    timer2 = 0;
    idx = 0;
    wave = 0;
    cnt2 = 0;
    grp = 0;
    se25 = 0.0f;
    ang[0] = ang[1] = ang[2] = 0.0f;
    p.x = p.y = p.z = 0.0f;
#endif
    flag = 0;
    seId2 = -1;
    state = 0;
    timer = 0;
    do {
        cnt = 0;
        n = lbl_1_bss_4->maxScore;
        while (n-- != 0) {
            it = &lbl_1_bss_0->item[n];
            switch (it->state) {
                case 0:
                    f = ((f32)frandmod(0x8C) / 120.0f) - 0.5833333f;
                    it->radX = 1.0f + f;
                    it->radY = 1.0f + (((f32)frandmod(0x8C) / 120.0f) - 0.5833333f);
                    it->state = 1;
                    break;
                case 1:
                    q.x = lbl_1_data_A8[0];
                    q.y = lbl_1_data_A8[1];
                    q.z = lbl_1_data_A8[2];
                    q.y = (20.0f + q.y) - (20.0f * it->radY);
                    PSVECSubtract(&q, &it->pos, &d);
                    if (PSVECSquareMag(&d) < 900.0f) {
                        it->timer = 0x5A;
                        it->state = 2;
                    } else {
                        PSVECNormalize(&d, &d);
                        PSVECScale(&d, &d, 4.0f);
                        PSVECAdd(&it->pos, &d, &it->pos);
                        Hu3DModelPosSetV(it->modelId, &it->pos);
                    }
                    break;
                case 2:
                    f24 = (f32)(2.0 * (3.141592653589793 * (f64)((f32)it->timer / 120.0f)));
                    f31 = (f32)((f64)it->radX * (20.0 * cos((f64)f24)));
                    w = (f32)((f64)it->radY * (20.0 * sin((f64)f24)));
                    t = it->pos;
                    t.x += f31;
                    t.y += w + (20.0f * it->radY);
                    Hu3DModelPosSetV(it->modelId, &t);
                    it->timer = it->timer + 1;
                    it->timer = it->timer % 120;
                    cnt += 1;
                    break;
            }
            if (state == 0) {
                t = it->scale;
                k = 0.3f * ((f32)timer / 60.0f);
                t.x += k;
                t.y += k;
                t.z += k;
                Hu3DModelScaleSetV(it->modelId, &t);
            }
        }
        if (timer < 0x3C) {
            timer += 1;
        } else if (state == 0 && cnt == lbl_1_bss_4->maxScore) {
            state = 1;
        }
        if (flag != 0) {
            w = (f32)(15.0 * sin((f64)(f32)(2.0 * (3.141592653589793 * (f64)((f32)wave / 300.0f)))));
            t = lbl_1_bss_0->itemPos0;
            t.y += w - 15.0f;
            lbl_1_bss_0->itemPos2 = t;
            Hu3DModelPosSetV(lbl_1_bss_0->mdlItem, &t);
            wave = (wave + 1) % 300;
        }
        switch (state) {
            case 0x1:
                seId = HuAudFXPlay(0x492);
                m = lbl_1_bss_0->mdl[16];
                Hu3DModelAttrReset(m, 0x40000002);
                Hu3DModelAttrReset(m, 1);
                Hu3DModelPosSetV(m, (Vec *)lbl_1_data_B4);
                Hu3DModelScaleSet(m, 2.0f, 1.5f, 2.0f);
                Hu3DParManAttrReset((s16)lbl_1_bss_0->parManId[0], 1);
                lbl_1_bss_0->wheelAngle[0] = 0;
                cnt2 = 0;
                idx = 1;
                state = 2;
                break;
            case 0x2:
                cnt2 += 1;
                if (cnt2 > 0xA) {
                    m = lbl_1_bss_0->mdl[idx + 0x10];
                    Hu3DModelAttrReset(m, 0x40000002);
                    Hu3DModelAttrReset(m, 1);
                    Hu3DModelPosSetV(m, (Vec *)lbl_1_data_B4);
                    Hu3DModelScaleSet(m, 2.0f, 1.5f, 2.0f);
                    if (idx % 2 != 0) {
                        Hu3DModelRotSet(m, 0.0f, 180.0f, 0.0f);
                    }
                    cnt2 = 0;
                    idx += 1;
                    if (idx >= 0xA) {
                        state = 3;
                    }
                }
                break;
            case 0x3:
                cnt2 += 1;
                if (cnt2 > 0x3C) {
                    m = lbl_1_bss_0->mdlItem;
                    Hu3DModelAttrReset(m, 0x40000002);
                    Hu3DModelAttrReset(m, 1);
                    Hu3DModelScaleSet(m, 1.0f, 1.0f, 1.0f);
                    Hu3DModelTPLvlSet(m, 0.0f);
                    lbl_1_bss_0->itemPos0.x = lbl_1_data_A8[0];
                    lbl_1_bss_0->itemPos0.y = lbl_1_data_A8[1];
                    lbl_1_bss_0->itemPos0.z = lbl_1_data_A8[2];
                    flag = 1;
                    wave = 0;
                    cnt2 = 0;
                    state = 6;
                    anim = HuSprAnimRead(HuDataSelHeapReadNum(0x98004D, HU_MEMNUM_OVL, HEAP_MODEL));
                    grp = HuSprGrpCreate(3);
                    i = 3;
                    while (i-- != 0) {
                        ang[i] = (f32)frandmod(0x168);
                        HuSprGrpMemberSet(grp, i, HuSprCreate(anim, 0x64, 0));
                        HuSprScaleSet(grp, i, 0.0f, 0.0f);
                        HuSprZRotSet(grp, i, ang[i]);
                        HuSprAttrSet(grp, i, 0x80);
                    }
                    HuSprGrpPosSet(grp, 290.0f, 120.0f);
                    HuSprGrpDrawNoSet(grp, 0);
                }
                break;
            case 0x6:
                f31 = ((f32)cnt2 - 22.5f) / 5.0f;
                ph = ((f32)cnt2 - 45.0f) / 60.0f;
                f24 = 1.0f - ((f32)cnt2 / 60.0f);
                if (f24 >= 0.0f && f31 <= 1.0f) {
                    n = lbl_1_bss_4->maxScore;
                    while (n-- != 0) {
                        Hu3DModelTPLvlSet(lbl_1_bss_0->item[n].modelId, f24);
                    }
                }
                if (0.0f <= f31 && f31 <= 2.0f) {
                    if (seId2 < 0) {
                        seId2 = HuAudFXPlay(0x493);
                    }
                    if (f31 >= 1.0f) {
                        n = lbl_1_bss_4->maxScore;
                        while (n-- != 0) {
                            Hu3DModelTPLvlSet(lbl_1_bss_0->item[n].modelId, 0.0f);
                        }
                        Hu3DModelTPLvlSet(lbl_1_bss_0->mdlItem, 1.0f);
                        f31 = 2.0f - f31;
                    }
                    f = f31 * 5.0f;
                    i = 3;
                    while (i-- != 0) {
                        if ((i & 1) != 0) {
                            ang[i] += 10.0f;
                        } else {
                            ang[i] -= 10.0f;
                        }
                        ang[i] = (f32)fmod((f64)ang[i], 360.0);
                        HuSprScaleSet(grp, i, f, f);
                        HuSprZRotSet(grp, i, ang[i]);
                    }
                } else {
                    i = 3;
                    while (i-- != 0) {
                        HuSprScaleSet(grp, i, 0.0f, 0.0f);
                    }
                }
                cnt2 += 1;
                if (cnt2 > 0x2D) {
                    n = lbl_1_bss_4->maxScore;
                    while (n-- != 0) {
                        Hu3DModelTPLvlSet(lbl_1_bss_0->item[n].modelId, 0.0f);
                    }
                    Hu3DModelTPLvlSet(lbl_1_bss_0->mdlItem, 1.0f);
                    HuAudFXStop(seId);
                    cnt2 = 0;
                    state = 7;
                }
                break;
            case 0x7:
                i = 0xA;
                while (i-- != 0) {
                    Hu3DModelTPLvlSet(lbl_1_bss_0->mdl[i + 0x10], 1.0f - ((f32)cnt2 / 120.0f));
                }
                cnt2 += 1;
                if (cnt2 > 0x78) {
                    i = 0xA;
                    while (i-- != 0) {
                        Hu3DModelAttrSet(lbl_1_bss_0->mdl[i + 0x10], 1);
                    }
                    lbl_1_bss_0->flag2 = lbl_1_bss_0->flag2 & ~0x10000;
                    HuAudFXStop(lbl_1_bss_0->seId);
                    lbl_1_bss_0->itemPos1 = lbl_1_bss_0->itemPos0;
                    timer2 = 0;
                    state = 0x64;
                }
                break;
            case 0x64:
                switch ((s8)lbl_1_bss_4->gameType) {
                    case 0:
                        p.x = lbl_1_data_30[lbl_1_bss_4->winner * 3];
                        p.y = lbl_1_data_30[lbl_1_bss_4->winner * 3 + 1];
                        p.z = lbl_1_data_30[lbl_1_bss_4->winner * 3 + 2];
                        p.y += 40.0f + CharModelHeightGet(lbl_1_bss_0->player[lbl_1_bss_4->winner].charNo);
                        break;
                    case 2:
                        if (lbl_1_bss_4->winner == 0) {
                            PSVECAdd((Vec *)&lbl_1_data_30[0], (Vec *)&lbl_1_data_30[3], &p);
                            PSVECScale(&p, &p, 0.5f);
                            h = CharModelHeightGet(lbl_1_bss_0->player[0].charNo);
                            if (h < CharModelHeightGet(lbl_1_bss_0->player[1].charNo)) {
                                h = CharModelHeightGet(lbl_1_bss_0->player[1].charNo);
                            }
                            p.y += 40.0f + h;
                        } else {
                            PSVECAdd((Vec *)&lbl_1_data_30[6], (Vec *)&lbl_1_data_30[9], &p);
                            PSVECScale(&p, &p, 0.5f);
                            h2 = CharModelHeightGet(lbl_1_bss_0->player[2].charNo);
                            if (h2 < CharModelHeightGet(lbl_1_bss_0->player[3].charNo)) {
                                h2 = CharModelHeightGet(lbl_1_bss_0->player[3].charNo);
                            }
                            p.y += 40.0f + h2;
                        }
                        break;
                    case 1:
                        if (lbl_1_bss_4->winner == 0) {
                            p.x = lbl_1_data_30[0];
                            p.y = lbl_1_data_30[1];
                            p.z = lbl_1_data_30[2];
                            p.y += 40.0f + CharModelHeightGet(lbl_1_bss_0->player[0].charNo);
                        } else {
                            p.x = lbl_1_data_30[6];
                            p.y = lbl_1_data_30[7];
                            p.z = lbl_1_data_30[8];
                            p.y += 240.0f;
                        }
                        break;
                }
                p.z -= 30.0f;
                f = (f32)sin(3.141592653589793 * (f64)(f32)sin(3.141592653589793 * (f64)((f32)timer2 / 60.0f) * 0.5) * 0.5);
                PSVECSubtract(&p, &lbl_1_bss_0->itemPos1, &d);
                PSVECScale(&d, &d, f);
                PSVECAdd(&lbl_1_bss_0->itemPos1, &d, &lbl_1_bss_0->itemPos0);
                timer2 += 1;
                if ((f32)timer2 > 60.0f) {
                    lbl_1_bss_0->camPath[0].time = 0.0f;
                    lbl_1_bss_0->camPath[0].fov = lbl_1_bss_4->fov;
                    lbl_1_bss_0->camPath[0].pos = lbl_1_bss_4->camPos;
                    lbl_1_bss_0->camPath[0].up = lbl_1_bss_4->camUp;
                    lbl_1_bss_0->camPath[0].target = lbl_1_bss_4->camTarget;
                    lbl_1_bss_0->camPath[1].time = 30.0f;
                    lbl_1_bss_0->camPath[1].fov = 40.0f;
                    lbl_1_bss_0->camPath[1].up = lbl_1_bss_4->camUp;
                    lbl_1_bss_0->camPath[1].pos.x = p.x;
                    lbl_1_bss_0->camPath[1].pos.y = 100.0f + p.y;
                    lbl_1_bss_0->camPath[1].pos.z = 500.0f + p.z;
                    lbl_1_bss_0->camPath[1].target.x = p.x;
                    lbl_1_bss_0->camPath[1].target.y = p.y - 40.0f;
                    lbl_1_bss_0->camPath[1].target.z = p.z;
                    switch ((s8)lbl_1_bss_4->gameType) {
                        case 0:
                            id = lbl_1_bss_4->winner;
                            omVibrate(lbl_1_bss_0->player[id].padNo, 0x14, 4, 2);
                            break;
                        case 2:
                            if (lbl_1_bss_4->winner == 0) {
                                omVibrate(lbl_1_bss_0->player[0].padNo, 0xF, 4, 2);
                                omVibrate(lbl_1_bss_0->player[1].padNo, 0xF, 4, 2);
                            } else {
                                omVibrate(lbl_1_bss_0->player[2].padNo, 0xF, 4, 2);
                                omVibrate(lbl_1_bss_0->player[3].padNo, 0xF, 4, 2);
                            }
                            break;
                        case 1:
                            if (lbl_1_bss_4->winner == 0) {
                                omVibrate(lbl_1_bss_0->player[0].padNo, 0xF, 4, 2);
                            } else {
                                omVibrate(lbl_1_bss_0->player[1].padNo, 0xF, 4, 2);
                                omVibrate(lbl_1_bss_0->player[2].padNo, 0xF, 4, 2);
                                omVibrate(lbl_1_bss_0->player[3].padNo, 0xF, 4, 2);
                            }
                            if (lbl_1_bss_4->winner != 0) {
                                lbl_1_bss_0->camPath[1].target.y = lbl_1_bss_0->camPath[1].target.y - 30.0f;
                            }
                            break;
                    }
                    lbl_1_bss_0->camPathNum = 2;
                    se25 = 0.0f;
                    lbl_1_bss_0->flag2 = lbl_1_bss_0->flag2 & ~0x100;
                    HuPrcChildCreate(fn_1_1EA0, 0x64, 0x1000, 0, HuPrcCurrentGet());
                    HuPrcVSleep();
                    HuAudSStreamFadeOut(lbl_1_bss_0->bgmId, 0x3E8);
                    lbl_1_bss_0->bgmId = -1;
                    state = 0x65;
                }
                break;
            case 0x65:
                se25 += 1.0f;
                lbl_1_data_228.scaleBase = 40.0f - (15.0f * (se25 / 30.0f));
                OSReport("%3.2f\n", lbl_1_data_228.scaleBase);
                if ((lbl_1_bss_0->flag2 & 0x100) != 0) {
                    state = 0x66;
                }
                break;
            case 0x66:
                switch ((s8)lbl_1_bss_4->gameType) {
                    case 0:
                        GameMesCreate(4, 3, lbl_1_bss_0->player[lbl_1_bss_4->winner].charNo, -1, -1, -1);
                        i = 4;
                        while (i-- != 0) {
                            pl = &lbl_1_bss_0->player[i];
                            if (i == lbl_1_bss_4->winner) {
                                fn_1_974(pl, 2);
                                if (pl->motion != 3) {
                                    CharMotionShiftSet(pl->charNo, pl->mot3, 0.0f, 15.0f, 0);
                                    pl->motion = 3;
                                }
                            } else if (pl->motion != 4) {
                                CharMotionShiftSet(pl->charNo, pl->mot4, 0.0f, 15.0f, 0);
                                pl->motion = 4;
                            }
                        }
                        break;
                    case 2:
                        pl = &lbl_1_bss_0->player[0];
                        if (lbl_1_bss_4->winner == 0) {
                            if (pl->motion != 3) {
                                CharMotionShiftSet(pl->charNo, pl->mot3, 0.0f, 15.0f, 0);
                                pl->motion = 3;
                            }
                            fn_1_974(pl, 2);
                            pl++;
                            if (pl->motion != 3) {
                                CharMotionShiftSet(pl->charNo, pl->mot3, 0.0f, 15.0f, 0);
                                pl->motion = 3;
                            }
                            fn_1_974(pl, 2);
                            pl++;
                            if (pl->motion != 4) {
                                CharMotionShiftSet(pl->charNo, pl->mot4, 0.0f, 15.0f, 0);
                                pl->motion = 4;
                            }
                            pl++;
                            if (pl->motion != 4) {
                                CharMotionShiftSet(pl->charNo, pl->mot4, 0.0f, 15.0f, 0);
                                pl->motion = 4;
                            }
                            GameMesCreate(4, 3, lbl_1_bss_0->player[0].charNo, lbl_1_bss_0->player[1].charNo, -1, -1);
                        } else {
                            if (pl->motion != 4) {
                                CharMotionShiftSet(pl->charNo, pl->mot4, 0.0f, 15.0f, 0);
                                pl->motion = 4;
                            }
                            pl++;
                            if (pl->motion != 4) {
                                CharMotionShiftSet(pl->charNo, pl->mot4, 0.0f, 15.0f, 0);
                                pl->motion = 4;
                            }
                            pl++;
                            if (pl->motion != 3) {
                                CharMotionShiftSet(pl->charNo, pl->mot3, 0.0f, 15.0f, 0);
                                pl->motion = 3;
                            }
                            fn_1_974(pl, 2);
                            pl++;
                            if (pl->motion != 3) {
                                CharMotionShiftSet(pl->charNo, pl->mot3, 0.0f, 15.0f, 0);
                                pl->motion = 3;
                            }
                            fn_1_974(pl, 2);
                            GameMesCreate(4, 3, lbl_1_bss_0->player[2].charNo, lbl_1_bss_0->player[3].charNo, -1, -1);
                        }
                        break;
                    case 1:
                        pl = &lbl_1_bss_0->player[0];
                        if (lbl_1_bss_4->winner == 0) {
                            if (pl->motion != 3) {
                                CharMotionShiftSet(pl->charNo, pl->mot3, 0.0f, 15.0f, 0);
                                pl->motion = 3;
                            }
                            fn_1_974(pl, 2);
                            pl++;
                            if (pl->motion != 4) {
                                CharMotionShiftSet(pl->charNo, pl->mot4, 0.0f, 15.0f, 0);
                                pl->motion = 4;
                            }
                            pl++;
                            if (pl->motion != 4) {
                                CharMotionShiftSet(pl->charNo, pl->mot4, 0.0f, 15.0f, 0);
                                pl->motion = 4;
                            }
                            pl++;
                            if (pl->motion != 4) {
                                CharMotionShiftSet(pl->charNo, pl->mot4, 0.0f, 15.0f, 0);
                                pl->motion = 4;
                            }
                            GameMesCreate(4, 3, lbl_1_bss_0->player[0].charNo, -1, -1, -1);
                        } else {
                            if (pl->motion != 4) {
                                CharMotionShiftSet(pl->charNo, pl->mot4, 0.0f, 15.0f, 0);
                                pl->motion = 4;
                            }
                            pl++;
                            if (pl->motion != 3) {
                                CharMotionShiftSet(pl->charNo, pl->mot3, 0.0f, 15.0f, 0);
                                pl->motion = 3;
                            }
                            fn_1_974(pl, 2);
                            pl++;
                            if (pl->motion != 3) {
                                CharMotionShiftSet(pl->charNo, pl->mot3, 0.0f, 15.0f, 0);
                                pl->motion = 3;
                            }
                            fn_1_974(pl, 2);
                            pl++;
                            if (pl->motion != 3) {
                                CharMotionShiftSet(pl->charNo, pl->mot3, 0.0f, 15.0f, 0);
                                pl->motion = 3;
                            }
                            fn_1_974(pl, 2);
                            GameMesCreate(4, 3, lbl_1_bss_0->player[1].charNo, lbl_1_bss_0->player[2].charNo,
                                lbl_1_bss_0->player[3].charNo, -1);
                        }
                        break;
                }
                HuAudJinglePlay(0x81);
                cnt2 = 0;
                state = 0x3E8;
                break;
            case 0x3E8:
                cnt2 += 1;
                if (cnt2 > 0xF0) {
                    lbl_1_bss_0->flag2 = lbl_1_bss_0->flag2 | 0x4000;
                    state = 0x2710;
                }
                break;
        }
        HuPrcVSleep();
    } while (state != 0x2710);
    HuPrcEnd();
}

void fn_1_64B0(void)
{
    lbl_1_bss_0->flag2 &= ~0x4000;
    HuPrcChildCreate(fn_1_4114, 0x64, 0x1500, 0, HuPrcCurrentGet());
}

void fn_1_6504(void)
{
    Vec a;
    Vec b;
    Vec c;
    f32 dx;
    f32 dy;
    f32 k;
    HU3DMODELID m;
    s32 i;

    Hu3DModelAttrReset(lbl_1_bss_0->mdl[37], 1);
    i = 9;
    while (i-- != 0) {
        Hu3DModelAttrReset(lbl_1_bss_0->mdl[i + 0x26], 1);
    }
    while ((lbl_1_bss_0->flag2 & 0x8000) != 0) {
        Hu3D3Dto2D((Vec *)lbl_1_data_1C4, 0x10, &a);
        b.x = a.x;
        b.y = a.y;
        b.z = a.z;
        b.z = lbl_1_bss_4->camPos.z;
        Hu3D2Dto3D(&b, 0x10, &b);
        Hu3DModelPosSetV(lbl_1_bss_0->mdl[37], &b);
        dx = 288.0f - a.x;
        dy = 240.0f - a.y;
        i = 9;
        while (i-- != 0) {
            c.x = a.x;
            c.y = a.y;
            c.z = a.z;
            c.x += 2.0f * (dx * lbl_1_data_1D0[i]);
            c.y = c.y + (2.0f * (dy * lbl_1_data_1D0[i]));
            c.z = lbl_1_bss_4->camPos.z - lbl_1_data_1D0[i];
            Hu3D2Dto3D(&c, 0x10, &c);
            m = lbl_1_bss_0->mdl[i + 0x26];
            Hu3DModelPosSetV(m, &c);
            k = lbl_1_data_1F4[i];
            c.z = k;
            c.y = k;
            c.x = k;
            Hu3DModelScaleSetV(m, &c);
        }
        HuPrcVSleep();
    }
    HuPrcEnd();
}

void fn_1_676C(void)
{
    lbl_1_bss_0->flag2 |= 0x8000;
    HuPrcChildCreate(fn_1_6504, 0x64, 0x1500, 0, HuPrcCurrentGet());
}

void fn_1_67C0(void)
{
    HU3DMODELID m;
    s32 timer;
    s32 n;
    s32 i;

    timer = 0;
    n = 0xF;
    while (n-- != 0) {
        HuPrcVSleep();
    }
    lbl_1_bss_0->flag2 = lbl_1_bss_0->flag2 & ~0x100000;
    HuPrcVSleep();
    i = 7;
    while (i-- != 0) {
        Hu3DModelAttrReset(lbl_1_bss_0->parMan[i].id, 1);
        lbl_1_bss_0->parMan[i].offset.x = lbl_1_data_320[3];
        lbl_1_bss_0->parMan[i].offset.y = lbl_1_data_320[4];
        lbl_1_bss_0->parMan[i].offset.z = lbl_1_data_320[5];
        PSVECAdd(&lbl_1_bss_0->parMan[i].offset, (Vec *)&lbl_1_data_278[(i + 7) * 3], &lbl_1_bss_0->parMan[i].offset);
        lbl_1_bss_0->parMan[i].pos = lbl_1_bss_0->parMan[i].offset;
        Hu3DModelPosSet(lbl_1_bss_0->parMan[i].id, lbl_1_bss_0->parMan[i].pos.x, lbl_1_bss_0->parMan[i].pos.y,
            lbl_1_bss_0->parMan[i].pos.z);
    }
    while ((lbl_1_bss_0->flag2 & 0x40000) == 0) {
        i = 7;
        while (i-- != 0) {
            m = lbl_1_bss_0->parMan[i].id;
            PSVECAdd(&lbl_1_bss_0->parMan[i].pos, (Vec *)&lbl_1_data_338[3], &lbl_1_bss_0->parMan[i].pos);
            Hu3DModelPosSet(m, lbl_1_bss_0->parMan[i].pos.x, lbl_1_bss_0->parMan[i].pos.y, lbl_1_bss_0->parMan[i].pos.z);
        }
        timer += 1;
        if ((f32)timer >= 1200.0f) {
            timer = 0;
            i = 7;
            while (i-- != 0) {
                lbl_1_bss_0->parMan[i].pos = lbl_1_bss_0->parMan[i].offset;
            }
        }
        HuPrcVSleep();
    }
    HuPrcEnd();
}

void fn_1_6B18(void)
{
    s32 i = 7;

    while (i-- != 0) {
        Hu3DModelAttrReset(lbl_1_bss_0->parMan[i].id, 1);
        Hu3DModelLayerSet(lbl_1_bss_0->parMan[i].id, 4);
        Hu3DModelCameraSet(lbl_1_bss_0->parMan[i].id, 0x10);
    }
    lbl_1_bss_0->flag2 &= ~0x40000;
    HuPrcChildCreate(fn_1_67C0, 0x64, 0x1000, 0, HuPrcCurrentGet());
}

void fn_1_6BE8(void)
{
    f32 wob[7];
    f32 *in[3];
    f32 *out[3];
    f32 tt[4];
    f32 kv[4];
    Vec v;
    Vec prev;
    Vec dir;
    f32 *key;
    f32 yaw;
    f32 t;
    f32 w;
    HU3DMODELID m;
    s16 wait;
    s16 k;
    s16 ax;
    s16 i;
    s32 n;

    wait = frandmod(0x4B0);
    while (wait-- != 0) {
        HuPrcVSleep();
    }
    i = 7;
    while (i-- != 0) {
        wob[i] = (f32)frandmod(0x168);
    }
    while (TRUE) {
        prev.x = 0.0f;
        prev.y = 0.0f;
        prev.z = 0.0f;
        out[0] = &v.x;
        out[1] = &v.y;
        out[2] = &v.z;
        i = 7;
        while (i-- != 0) {
            Hu3DModelAttrReset(lbl_1_bss_0->parMan[i].id, 1);
        }
        t = 0.0f;
        while (t <= 1200.0f) {
            if ((lbl_1_bss_0->flag2 & 0x100000) == 0) {
                break;
            }
            key = lbl_1_data_350;
            k = 0;
            while (k < 4) {
#ifdef TARGET_PC
                if (k < 3 && key[3] <= t && t < key[7]) {
                    break;
                }
#else
                if (key[3] <= t && t < key[7]) {
                    break;
                }
#endif
                k += 1;
                key += 0x10 / sizeof(f32);
            }
            if (k == 4) {
                break;
            }
            in[0] = &key[0];
            in[1] = &key[1];
            in[2] = &key[2];
            if (k == 0) {
                tt[0] = -1.0f;
            } else {
                tt[0] = key[-1];
            }
            tt[1] = key[3];
            if (k >= 3) {
                tt[2] = 1.0f + tt[1];
            } else {
                tt[2] = key[7];
            }
            if (k >= 2) {
                tt[3] = 1.0f + tt[2];
            } else {
                tt[3] = key[0xB];
            }
            ax = 3;
            while (ax-- != 0) {
                if (k == 0) {
                    kv[0] = in[ax][0];
                } else {
                    kv[0] = in[ax][-4];
                }
                kv[1] = in[ax][0];
                if (k >= 3) {
                    kv[2] = kv[1];
                } else {
                    kv[2] = in[ax][4];
                }
                if (k >= 2) {
                    kv[3] = kv[2];
                } else {
                    kv[3] = in[ax][8];
                }
                *out[ax] = InterpolateBMLine(kv, tt, t);
            }
            PSVECSubtract(&v, &prev, &dir);
            PSVECNormalize(&dir, &dir);
            prev.x = v.x;
            prev.y = v.y;
            prev.z = v.z;
            if (dir.z > 1.0f) {
                dir.z = 1.0f;
            } else if (dir.z < -1.0f) {
                dir.z = -1.0f;
            }
            yaw = (f32)acos((f64)dir.z);
            if (dir.x < 0.0f) {
                yaw = -yaw;
            }
            i = 7;
            while (i-- != 0) {
                m = lbl_1_bss_0->parMan[i].id;
                w = (f32)(50.0 * sin((3.141592653589793 * (f64)wob[i]) / 180.0));
                PSVECAdd(&v, (Vec *)&lbl_1_data_278[i * 3], &lbl_1_bss_0->parMan[i].pos);
                Hu3DModelPosSet(m, lbl_1_bss_0->parMan[i].pos.x, lbl_1_bss_0->parMan[i].pos.y + w,
                    lbl_1_bss_0->parMan[i].pos.z);
                Hu3DModelRotSet(m, 0.0f, (f32)((f64)(180.0f * yaw) / 3.141592653589793), 0.0f);
                wob[i] += 3.0f;
                wob[i] = (f32)fmod((f64)wob[i], 360.0);
            }
            HuPrcVSleep();
            t += 1.0f;
        }
        i = 7;
        while (i-- != 0) {
            Hu3DModelAttrSet(lbl_1_bss_0->parMan[i].id, 1);
        }
        if ((lbl_1_bss_0->flag2 & 0x100000) != 0) {
            n = 0x4B0;
            while (n-- != 0) {
                HuPrcVSleep();
            }
            continue;
        }
        HuPrcEnd();
        break;
    }
}

void fn_1_72E8(void)
{
    s32 i;

    if ((lbl_1_bss_0->flag2 & 0x100000) == 0) {
        i = 7;
        while (i-- != 0) {
            Hu3DModelAttrReset(lbl_1_bss_0->parMan[i].id, 1);
            lbl_1_bss_0->parMan[i].offset.x = lbl_1_data_320[0];
            lbl_1_bss_0->parMan[i].offset.y = lbl_1_data_320[1];
            lbl_1_bss_0->parMan[i].offset.z = lbl_1_data_320[2];
            PSVECAdd(&lbl_1_bss_0->parMan[i].offset, (Vec *)&lbl_1_data_278[i * 3], &lbl_1_bss_0->parMan[i].offset);
            lbl_1_bss_0->parMan[i].pos = lbl_1_bss_0->parMan[i].offset;
            Hu3DModelPosSet(lbl_1_bss_0->parMan[i].id, lbl_1_bss_0->parMan[i].pos.x, lbl_1_bss_0->parMan[i].pos.y,
                lbl_1_bss_0->parMan[i].pos.z);
            Hu3DModelLayerSet(lbl_1_bss_0->parMan[i].id, 0);
            Hu3DModelCameraSet(lbl_1_bss_0->parMan[i].id, 0x10);
        }
        lbl_1_bss_0->flag2 = lbl_1_bss_0->flag2 | 0x100000;
        HuPrcChildCreate(fn_1_6BE8, 0x64, 0x1000, 0, HuPrcCurrentGet());
    }
}

void fn_1_74FC(M505_PLAYER *player, Vec *target)
{
    f32 knot[4][4];
    f32 tt[4];
    f32 kv[4];
    f32 *in[3];
    f32 *out[3];
    Vec v;
    Vec prev;
    Vec dir;
    f32 *key;
    f32 yaw;
    f32 t;
    s16 k;
    s16 ax;
    s16 i;

    target->y = 0.0f;
    knot[0][0] = player->pos.x;
    knot[0][1] = player->pos.y;
    knot[0][2] = player->pos.z;
    knot[1][0] = player->pos.x;
    knot[1][1] = player->pos.y;
    knot[1][2] = player->pos.z;
    knot[1][2] += 30.0f;
    knot[2][0] = target->x;
    knot[2][1] = target->y;
    knot[2][2] = target->z;
    knot[2][2] -= 30.0f;
    knot[3][0] = target->x;
    knot[3][1] = target->y;
    knot[3][2] = target->z;
    knot[3][2] += 30.0f;
    knot[0][3] = 0.0f;
    i = 1;
    while (i < 4) {
        PSVECSubtract((Vec *)knot[i], (Vec *)knot[i - 1], &v);
        knot[i][3] = knot[i - 1][3] + (PSVECMag(&v) / 6.0f);
        i += 1;
    }
    if (player->motion != 2) {
        CharMotionShiftSet(player->charNo, player->mot2, 0.0f, 15.0f, 0x40000001);
        player->motion = 2;
    }
    prev.x = player->pos.x;
    prev.y = player->pos.y;
    prev.z = player->pos.z;
    prev.z -= 10.0f;
    out[0] = &v.x;
    out[1] = &v.y;
    out[2] = &v.z;
    t = 0.0f;
    while (TRUE) {
        key = &knot[0][0];
        k = 0;
        while (k < 4) {
#ifdef TARGET_PC
            if (k < 3 && key[3] <= t && t < key[7]) {
                break;
            }
#else
            if (key[3] <= t && t < key[7]) {
                break;
            }
#endif
            k += 1;
            key += 0x10 / sizeof(f32);
        }
        if (k == 4) {
            break;
        }
        in[0] = &key[0];
        in[1] = &key[1];
        in[2] = &key[2];
        if (k == 0) {
            tt[0] = -1.0f;
        } else {
            tt[0] = key[-1];
        }
        tt[1] = key[3];
        if (k >= 3) {
            tt[2] = 1.0f + tt[1];
        } else {
            tt[2] = key[7];
        }
        if (k >= 2) {
            tt[3] = 1.0f + tt[2];
        } else {
            tt[3] = key[0xB];
        }
        ax = 3;
        while (ax-- != 0) {
            if (k == 0) {
                kv[0] = in[ax][0];
            } else {
                kv[0] = in[ax][-4];
            }
            kv[1] = in[ax][0];
            if (k >= 3) {
                kv[2] = kv[1];
            } else {
                kv[2] = in[ax][4];
            }
            if (k >= 2) {
                kv[3] = kv[2];
            } else {
                kv[3] = in[ax][8];
            }
            *out[ax] = InterpolateBMLine(kv, tt, t);
        }
        PSVECSubtract(&v, &prev, &dir);
        PSVECNormalize(&dir, &dir);
        prev.x = v.x;
        prev.y = v.y;
        prev.z = v.z;
        if (dir.z > 1.0f) {
            dir.z = 1.0f;
        } else if (dir.z < -1.0f) {
            dir.z = -1.0f;
        }
        yaw = (f32)acos((f64)dir.z);
        if (dir.x < 0.0f) {
            yaw = -yaw;
        }
        player->pos.x = v.x;
        player->pos.y = v.y;
        player->pos.z = v.z;
        Hu3DModelPosSet(player->modelId, player->pos.x, player->pos.y, player->pos.z);
        Hu3DModelRotSet(player->modelId, 0.0f, (f32)((f64)(180.0f * yaw) / 3.141592653589793), 0.0f);
        HuPrcVSleep();
        t += 1.0f;
    }
    player->pos.x = knot[3][0];
    player->pos.y = knot[3][1];
    player->pos.z = knot[3][2];
    Hu3DModelPosSet(player->modelId, player->pos.x, player->pos.y, player->pos.z);
    Hu3DModelRotSet(player->modelId, 0.0f, 0.0f, 0.0f);
    if (player->motion != 0) {
        CharMotionShiftSet(player->charNo, player->mot0, 0.0f, 15.0f, 0x40000001);
        player->motion = 0;
    }
}

s32 fn_1_7B6C(f32 *cur, f32 target, f32 step)
{
    f32 diff;
    f32 absd;
    f32 d;

    diff = *cur - target;
    d = (f32)fmod((f64)diff, 180.0);
    if ((f32)fabs((f32)fmod((f64)diff, 360.0)) > 180.0f) {
        if (d > 0.0f) {
            d = -(180.0f - (f32)fabs(d));
        } else {
            d = 180.0f - (f32)fabs(d);
        }
    }
    absd = (f32)fabs(d);
    if (absd < step) {
        *cur = target;
        return 1;
    }
    if (d > 0.0f) {
        d = d - step;
    } else {
        d = d + step;
    }
    *cur = target + d;
    return 0;
}

void fn_1_7D74(void)
{
    Vec dir;
    Vec norm;
    f32 a;
    s32 done;
    s32 reached;
    s32 i;
    M505_PLAYER *p;

    p = lbl_1_bss_0->player;
    for (i = 0; i < 4; i++) {
        switch (lbl_1_bss_0->camMotionType) {
            case 0:
                p->unk20 = 0.0f;
                break;
            case 1:
                PSVECSubtract((Vec *)lbl_1_data_390, &p->pos, &dir);
                dir.y = 0.0f;
                PSVECNormalize(&dir, &norm);
                if (norm.z > 1.0f) {
                    norm.z = 1.0f;
                } else if (norm.z < -1.0f) {
                    norm.z = -1.0f;
                }
                a = (f32)acos((f64)norm.z);
                if (norm.x < 0.0f) {
                    a = -a;
                }
                p->unk20 = (f32)((f64)(180.0f * a) / 3.141592653589793);
                break;
            case 2:
                PSVECSubtract((Vec *)lbl_1_data_39C, &p->pos, &dir);
                dir.y = 0.0f;
                PSVECNormalize(&dir, &norm);
                if (norm.z > 1.0f) {
                    norm.z = 1.0f;
                } else if (norm.z < -1.0f) {
                    norm.z = -1.0f;
                }
                a = (f32)acos((f64)norm.z);
                if (norm.x < 0.0f) {
                    a = -a;
                }
                p->unk20 = (f32)((f64)(180.0f * a) / 3.141592653589793);
                break;
        }
        reached = fn_1_7B6C(&p->unk24, p->unk20, 0.01f);
        if (reached == 0 && p->motion != 1) {
            CharMotionShiftSet(p->charNo, p->mot1, 0.0f, 15.0f, 0x40000001);
            p->motion = 1;
        }
        p++;
    }
    do {
        done = 0;
        p = lbl_1_bss_0->player;
        i = 4;
        while (i-- != 0) {
            reached = fn_1_7B6C(&p->unk24, p->unk20, lbl_1_bss_0->rotSpeed);
            if (reached != 0) {
                if (p->motion != 0) {
                    CharMotionShiftSet(p->charNo, p->mot0, 0.0f, 15.0f, 0x40000001);
                    p->motion = 0;
                }
                done += 1;
            }
            Hu3DModelRotSet(p->modelId, 0.0f, p->unk24, 0.0f);
            p++;
        }
        if (done != 4) {
            HuPrcVSleep();
        }
    } while (done != 4);
    lbl_1_bss_0->flag2 = lbl_1_bss_0->flag2 | 0x200000;
    HuPrcEnd();
}

void fn_1_8538(s32 arg0, f32 arg1)
{
    lbl_1_bss_0->flag2 &= ~0x200000;
    lbl_1_bss_0->camMotionType = (s16)arg0;
    lbl_1_bss_0->rotSpeed = arg1;
    HuPrcChildCreate(fn_1_7D74, 0x64, 0x1000, 0, HuPrcCurrentGet());
    HuPrcVSleep();
}

void fn_1_85C4(void)
{
    Vec camPos = { 0.0f, 2000.0f, 0.0f };
    Vec camUp = { 0.0f, 0.0f, 1.0f };
    Vec camTarget = { 0.0f, 0.0f, 0.0f };
    Vec lightDir = { 0.0f, -1.0f, 0.0f };
    Vec spotPos = { 0.0f, 50.0f, 700.0f };
    Vec spotDir = { 0.0f, 0.0f, -1.0f };
    Vec podiumPos;
    HuVec2f size;
    s8 key[4];
    u8 order[4];
    ANIMDATA *anim;
    ANIMDATA *parAnim;
    OMOVLHIS *his;
    M505_PLAYER *p;
    HU3DLIGHTID light;
    HU3DMODELID model;
    s32 i;
    s32 j;
    s32 id;
    s8 idx;
    s8 n;
    s8 teams;

    his = omOvlHisGet(0);
    lbl_1_bss_4 = (M505_MODEWORK *)mgModeWork;
    lbl_1_bss_0 = &lbl_1_bss_8;
    lbl_1_bss_0->objman = omInitObjMan(0xC8, 0x3E8);
    HuDataSelHeapReadNum(0x270000, HU_MEMNUM_OVL, HEAP_MODEL);
    Hu3DCameraCreate(0x10);
    Hu3DCameraPerspectiveSet(0x10, 60.0f, 20.0f, 60000.0f, 1.2f);
    Hu3DCameraViewportSet(0x10, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(0x10, 0, 0, 0x280, 0x1E0);
    light = Hu3DGLightCreate(0.0f, 1000.0f, 1000.0f, 0.0f, -1.0f, -1.0f, 0xFF, 0xFF, 0xFF);
    Hu3DGLightStaticSet(light, 1);
    Hu3DGLightPosSetV(light, &camPos, &lightDir);
    light = Hu3DGLightCreate(0.0f, 1000.0f, 1000.0f, 0.0f, -1.0f, -1.0f, 0x80, 0x80, 0x80);
    lbl_1_bss_0->glightId = light;
    Hu3DGLightStaticSet(light, 1);
    Hu3DGLightPosSetV(light, &spotPos, &spotDir);
    Hu3DGLightSpotSet(light, GX_SP_FLAT, 150.0f);
    Hu3DShadowCreate(15.0f, 20.0f, 8000.0f);
    Hu3DShadowTPLvlSet(0.3f);
    Hu3DShadowPosSet(&camPos, &camUp, &camTarget);
    Hu3DShadowSizeSet(0xF0);
    Hu3DParManInit();
    for (i = 0; i < 0x2B; i++) {
        idx = 0;
        while ((u16)mgDataTbl[idx].ovl != (u16)lbl_1_data_C0[i].ovlNo) {
            if ((u16)mgDataTbl[idx].ovl == 0xFFFF) {
                OSReport("mgmatch.c : \x82"
                         "c\x82k\x82k\x82\xaa\x8c\xa9\x82\xc2\x82\xa9\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x81I\x81I\n");
                abort();
            }
            idx += 1;
        }
        lbl_1_data_C0[i].mgIdx = idx;
    }
    for (i = 0; i < 0x2B; i++) {
        lbl_1_data_C0[i].is3D = (u8)mgDataTbl[(s8)(u8)lbl_1_data_C0[i].mgIdx].type == 3;
    }
    lbl_1_bss_0->camNo = -1;
    lbl_1_bss_0->winId = 0;
    lbl_1_bss_0->winner = -1;
    lbl_1_bss_0->flag = 0;
    lbl_1_bss_0->flag2 = 0;
    lbl_1_bss_0->camMotionNo = -1;
    lbl_1_bss_0->camMoveRate = 0.0f;
    lbl_1_bss_0->timerType = 0;
    lbl_1_bss_0->animId = -1;
    lbl_1_bss_0->bgmId = -1;
    lbl_1_bss_0->itemAnimState = 0;
    HuWinInit(1);
    HuWinMesMaxSizeBetGet(&size, 0xF0000, 0xF0012);
    lbl_1_bss_0->winId = HuWinExCreate(-10000.0f, 360.0f, (s16)size.x, (s16)size.y, -1);
    fn_1_D88();
    parAnim = HuSprAnimRead(HuDataSelHeapReadNum(0x98004A, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_data_228.colorStart[0].r = lbl_1_data_218[0].r;
    lbl_1_data_228.colorStart[0].g = lbl_1_data_218[0].g;
    lbl_1_data_228.colorStart[0].b = lbl_1_data_218[0].b;
    lbl_1_data_228.colorStart[0].a = lbl_1_data_218[0].a;
    lbl_1_data_228.colorStart[1].r = lbl_1_data_218[1].r;
    lbl_1_data_228.colorStart[1].g = lbl_1_data_218[1].g;
    lbl_1_data_228.colorStart[1].b = lbl_1_data_218[1].b;
    lbl_1_data_228.colorStart[1].a = lbl_1_data_218[1].a;
    lbl_1_data_228.colorEnd[0].r = lbl_1_data_218[2].r;
    lbl_1_data_228.colorEnd[0].g = lbl_1_data_218[2].g;
    lbl_1_data_228.colorEnd[0].b = lbl_1_data_218[2].b;
    lbl_1_data_228.colorEnd[0].a = lbl_1_data_218[2].a;
    lbl_1_data_228.colorEnd[1].r = lbl_1_data_218[3].r;
    lbl_1_data_228.colorEnd[1].g = lbl_1_data_218[3].g;
    lbl_1_data_228.colorEnd[1].b = lbl_1_data_218[3].b;
    lbl_1_data_228.colorEnd[1].a = lbl_1_data_218[3].a;
    i = 4;
    while (i-- != 0) {
        id = Hu3DParManCreate(parAnim, 0x64, &lbl_1_data_228);
        lbl_1_bss_0->parManId[i] = id;
        Hu3DParManAttrSet(id, 0x55);
        Hu3DParManLayerSet(id, 4);
        Hu3DParticleBlendModeSet(Hu3DParManModelIDGet(id), 1);
    }
    fn_1_2F78();
    if (his->evtno == 0) {
        _SetFlag(0x30003);
        _SetFlag(0x30001);
        lbl_1_bss_4->gameType = mgMatchGameType;
        lbl_1_bss_4->maxScore = mgMatchMaxScore;
        switch (mgMatchGameType) {
            case 0:
                lbl_1_bss_4->mgCount = 0;
                for (i = 0; i < 0x15; i++) {
                    if (GWMgUnlockGet(lbl_1_data_C0[i].mgNo)) {
                        lbl_1_bss_4->mgTbl[(s8)lbl_1_bss_4->mgCount] = lbl_1_data_C0[i];
                        lbl_1_bss_4->mgCount = lbl_1_bss_4->mgCount + 1;
                    }
                }
                for (i = 0; i < 4; i++) {
                    GwPlayerConf[i].grp = i;
                }
                break;
            case 2:
                lbl_1_bss_4->mgCount = 0;
                for (i = 0x15; i < 0x20; i++) {
                    if (GWMgUnlockGet(lbl_1_data_C0[i].mgNo)) {
                        lbl_1_bss_4->mgTbl[(s8)lbl_1_bss_4->mgCount] = lbl_1_data_C0[i];
                        lbl_1_bss_4->mgCount = lbl_1_bss_4->mgCount + 1;
                    }
                }
                break;
            case 1:
                lbl_1_bss_4->mgCount = 0;
                for (i = 0x20; i < 0x2B; i++) {
                    if (GWMgUnlockGet(lbl_1_data_C0[i].mgNo)) {
                        lbl_1_bss_4->mgTbl[(s8)lbl_1_bss_4->mgCount] = lbl_1_data_C0[i];
                        lbl_1_bss_4->mgCount = lbl_1_bss_4->mgCount + 1;
                    }
                }
                break;
        }
        if ((s8)(u8)lbl_1_bss_4->gameType == 0) {
            teams = 4;
        } else {
            teams = 2;
        }
        lbl_1_bss_4->teamCount = teams;
        n = (s8)(u8)lbl_1_bss_4->teamCount;
        while (n-- != 0) {
            lbl_1_bss_4->score[n] = 0;
        }
    } else {
        Hu3DCameraPosSetV(0x10, &lbl_1_bss_4->camPos, &lbl_1_bss_4->camUp, &lbl_1_bss_4->camTarget);
        Hu3DCameraPerspectiveSet(0x10, lbl_1_bss_4->fov, 20.0f, 60000.0f, 1.2f);
    }
    p = lbl_1_bss_0->player;
    for (i = 0; i < 4; i++) {
        order[i] = i;
        key[i] = GwPlayerConf[i].grp;
    }
    for (i = 0; i < 3; i++) {
        for (j = i + 1; j < 4; j++) {
            if (key[i] > key[j]) {
                n = key[i];
                key[i] = key[j];
                key[j] = n;
                n = order[i];
                order[i] = order[j];
                order[j] = n;
            }
        }
    }
    for (i = 0; i < 4; i++) {
        p->playerNo = (s8)order[i];
        p->unk1E = key[i];
        p->charNo = GwPlayerConf[(s8)order[i]].charNo;
        p->padNo = GwPlayerConf[(s8)order[i]].padNo;
        p->modelId = CharModelMotListCreate(p->charNo, 2, lbl_1_data_0, &p->mot0);
        p->motion = -1;
        p->pos.x = lbl_1_data_30[i * 3];
        p->pos.y = lbl_1_data_30[i * 3 + 1];
        p->pos.z = lbl_1_data_30[i * 3 + 2];
        CharMotionVoiceOnSet(p->charNo, (s16)lbl_1_data_0[3], 0);
        CharMotionVoiceOnSet(p->charNo, (s16)lbl_1_data_0[4], 0);
        CharMotionVoiceOnSet(p->charNo, (s16)lbl_1_data_0[5], 0);
        if (p->motion != 0) {
            CharMotionShiftSet(p->charNo, p->mot0, 0.0f, 15.0f, 0x40000001);
            p->motion = 0;
        }
        Hu3DModelAttrSet(p->modelId, 1);
        Hu3DModelPosSet(p->modelId, p->pos.x, p->pos.y, p->pos.z);
        Hu3DModelLayerSet(p->modelId, 4);
        Hu3DModelCameraSet(p->modelId, 0x10);
        lbl_1_bss_0->mdl[i + 0x1A] = p->modelId;
        p++;
    }
    CharEffectLayerSet(5);
    podiumPos.x = lbl_1_data_60[0];
    podiumPos.y = lbl_1_data_60[1];
    podiumPos.z = lbl_1_data_60[2];
    model = Hu3DModelCreate(HuDataSelHeapReadNum(0x980000, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_0->mdlPodium = model;
    lbl_1_bss_0->pos = podiumPos;
    Hu3DMotionSet(model, Hu3DJointMotion(model, HuDataSelHeapReadNum(0x980001, HU_MEMNUM_OVL, HEAP_MODEL)));
    Hu3DModelAttrSet(model, 0x40000001);
    Hu3DModelLayerSet(model, 3);
    Hu3DModelCameraSet(model, 0x10);
    Hu3DModelPosSet(model, podiumPos.x, podiumPos.y, podiumPos.z);
    fn_1_12404((s8)(u8)lbl_1_bss_4->gameType, (s8)(u8)lbl_1_bss_4->maxScore, lbl_1_bss_0->player, lbl_1_bss_4->score,
        lbl_1_bss_0->mdl);
    HuPrcChildCreate(fn_1_3B24, 0x64, 0x1000, 0, HuPrcCurrentGet());
    n = (s8)lbl_1_bss_4->mgCount;
    while (n-- != 0) {
        idx = lbl_1_bss_4->mgTbl[n].mgIdx;
        if (idx + 0x1F5 == 0x207) {
            lbl_1_bss_0->movieNo = frandmod(3);
            anim = HuSprAnimRead(HuDataSelHeapReadNum(mgDataTbl[idx].instPic[lbl_1_bss_0->movieNo], HU_MEMNUM_OVL, HEAP_MODEL));
        } else {
            anim = HuSprAnimRead(HuDataSelHeapReadNum(mgDataTbl[idx].instPic[0], HU_MEMNUM_OVL, HEAP_MODEL));
        }
        HuSprAnimLock(anim);
        lbl_1_bss_0->mgAnim[n] = anim;
    }
    Hu3DModelShadowMapSet(lbl_1_bss_0->mdl[0]);
    for (i = 0; i < 4; i++) {
        Hu3DModelShadowSet(lbl_1_bss_0->player[i].modelId);
    }
    for (i = 0; i < 3; i++) {
        Hu3DModelShadowSet(lbl_1_bss_0->parMan[i].id);
    }
    Hu3DModelShadowSet(lbl_1_bss_0->mdlPodium);
}

void fn_1_9844(void)
{
    OMOVLHIS *his;

    HuDataDirClose(0x980000);
    HuDataDirClose(0x270000);
    CharModelDataClose(-1);
    HuPrcVSleep();
    HuTHPClose();
    if (lbl_1_bss_0->bgmId >= 0) {
        HuAudSStreamFadeOut(lbl_1_bss_0->bgmId, 0x3e8);
        lbl_1_bss_0->bgmId = -1;
    }
    GwSystem.mgInstF = 1;
    his = omOvlHisGet(0);
    omOvlHisChg(0, his->ovl, 0, his->stat);
    HuWinExClose(lbl_1_bss_0->winId);
    HuWinExKill(lbl_1_bss_0->winId);
    WipeCreate(2, 6, 0x3c);
    while ((u8)WipeCheckEnd() != 0) {
        HuPrcVSleep();
    }
    _ClearFlag(0x30003);
    _ClearFlag(0x30001);
    memset(lbl_1_bss_4, 0, 0xd8);
}

void fn_1_9984(void)
{
    Vec cam;
    Vec target;
    CAMMOTIONWORK *buf;
    CAMMOTIONWORK *tail;
    OMOVLHIS *his;
    M505_PLAYER *p;
    f32 fov;
    int max;
    s32 n;
    s32 i;

    his = omOvlHisGet(0);
    omOvlHisChg(0, his->ovl, 1, his->stat);
    lbl_1_bss_4->winner = -1;
    lbl_1_bss_0->flag2 = lbl_1_bss_0->flag2 & ~0x1F;
    lbl_1_bss_0->flag = 0;
    lbl_1_bss_0->flag2 = lbl_1_bss_0->flag2 | 1;
    n = (s8)lbl_1_bss_4->mgCount;
    while (n-- != 0) {
        lbl_1_bss_4->order[n] = n;
    }
    lbl_1_bss_4->entryNo = lbl_1_bss_4->mgCount;
    fn_1_33AC();
    fn_1_31B0();
    fn_1_12934();
    fn_1_15BC8();
    Hu3DModelAttrSet(lbl_1_bss_0->mdl[5], 0x40000002);
    Hu3DMotionTimeSet(lbl_1_bss_0->mdl[5], 0.0f);
    Hu3DModelAttrReset(lbl_1_bss_0->mdl[3], 0x40000002);
    Hu3DModelAttrReset(lbl_1_bss_0->mdl[3], 1);
    Hu3DMotionTimeSet(lbl_1_bss_0->mdl[3], 0.0f);
    p = lbl_1_bss_0->player;
    for (i = 0; i < 4; i++) {
        if (p->motion != 5) {
            CharMotionShiftSet(p->charNo, p->mot5, 0.0f, 15.0f, 0);
            p->motion = 5;
        }
        Hu3DModelAttrSet(p->modelId, 1);
        Hu3DModelPosSet(p->modelId, p->pos.x, p->pos.y, p->pos.z);
        Hu3DModelLayerSet(p->modelId, 4);
        Hu3DModelCameraSet(p->modelId, 0x10);
        p++;
    }
    HuPrcChildCreate(fn_1_2058, 0x64, 0x1000, 0, HuPrcCurrentGet());
    buf = lbl_1_bss_0->camPath;
    max = 0x32;
    lbl_1_bss_0->camPathNum = CamMotionExPathGet(lbl_1_bss_0->mdlCam0, 6, 240.0f, 0, &buf, &max);
    i = lbl_1_bss_0->camPathNum;
    while (i-- != 0) {
        buf[i].pos.y = buf[i].pos.y - 100.0f;
    }
    lbl_1_bss_0->flag2 = lbl_1_bss_0->flag2 & ~0x100;
    HuPrcChildCreate(fn_1_1EA0, 0x64, 0x1000, 0, HuPrcCurrentGet());
    HuPrcVSleep();
    WipeCreate(1, 5, 0x3C);
    while (WipeCheckEnd() != 0) {
        HuPrcVSleep();
    }
    n = 0xAA;
    while (n-- != 0) {
        HuPrcVSleep();
    }
    Hu3DModelAttrReset(lbl_1_bss_0->mdl[5], 0x40000002);
    while ((lbl_1_bss_0->flag2 & 0x100) == 0) {
        HuPrcVSleep();
    }
    tail = &lbl_1_bss_0->camPath[lbl_1_bss_0->camPathNum - 1];
    Hu3DCameraPosGet(0x10, &lbl_1_bss_4->camPos, &lbl_1_bss_4->camUp, &lbl_1_bss_4->camTarget);
    lbl_1_bss_4->fov = tail->fov;
    cam.x = lbl_1_bss_4->camPos.x;
    cam.y = lbl_1_bss_4->camPos.y;
    cam.z = lbl_1_bss_4->camPos.z;
    target.x = lbl_1_bss_4->camTarget.x;
    target.y = lbl_1_bss_4->camTarget.y;
    target.z = lbl_1_bss_4->camTarget.z;
    fov = lbl_1_bss_4->fov;
    n = 0x3C;
    while (n-- != 0) {
        fov -= 0.2f;
        cam.y += 2.0f;
        target.z += 2.0f;
        Hu3DCameraPosSetV(0x10, &cam, &lbl_1_bss_4->camUp, &target);
        Hu3DCameraPerspectiveSet(0x10, fov, 20.0f, 60000.0f, 1.2f);
        HuPrcVSleep();
    }
    lbl_1_bss_0->bgmId = HuAudBGMPlay(0x23);
    HuPrcChildCreate(fn_1_15F24, 0x64, 0x1000, 0, HuPrcCurrentGet());
    HuPrcVSleep();
    while ((lbl_1_bss_0->flag & 1) == 0) {
        HuPrcVSleep();
    }
    if ((lbl_1_bss_0->flag2 & 0x20) != 0) {
        lbl_1_bss_0->flag2 = lbl_1_bss_0->flag2 & ~0x20;
        HuWinExClose(lbl_1_bss_0->winId);
        HuWinHomeClear(lbl_1_bss_0->winId);
    }
    lbl_1_bss_0->posTo = lbl_1_bss_0->pos;
    lbl_1_bss_0->posFrom.x = lbl_1_data_60[3];
    lbl_1_bss_0->posFrom.y = lbl_1_data_60[4];
    lbl_1_bss_0->posFrom.z = lbl_1_data_60[5];
    lbl_1_bss_0->camMoveSpeed = 0x5A;
    lbl_1_bss_0->flag2 = lbl_1_bss_0->flag2 & ~0x80;
    HuPrcChildCreate(fn_1_267C, 0x64, 0x1000, 0, HuPrcCurrentGet());
    i = 0;
    while ((f32)i < 60.0f) {
        fov += 0.2f;
        cam.y -= 2.0f;
        target.z -= 2.0f;
        Hu3DCameraPosSetV(0x10, &cam, &lbl_1_bss_4->camUp, &target);
        Hu3DCameraPerspectiveSet(0x10, fov, 20.0f, 60000.0f, 1.2f);
        Hu3DModelRotSet(lbl_1_bss_0->mdlPodium, 0.0f, 45.0f * ((f32)i / 60.0f), 0.0f);
        HuPrcVSleep();
        i += 1;
    }
    Hu3DModelRotSet(lbl_1_bss_0->mdlPodium, 0.0f, 45.0f, 0.0f);
    n = 0x1E;
    while (n-- != 0) {
        HuPrcVSleep();
    }
    lbl_1_bss_0->flag2 = lbl_1_bss_0->flag2 & ~0x40;
    HuPrcChildCreate(fn_1_2170, 0x64, 0x1000, 0, HuPrcCurrentGet());
    while ((lbl_1_bss_0->flag2 & 0x40) == 0 || (lbl_1_bss_0->flag2 & 0x80) == 0) {
        HuPrcVSleep();
    }
    lbl_1_bss_0->flag2 = lbl_1_bss_0->flag2 & ~0x200000;
    lbl_1_bss_0->camMotionType = 1;
    lbl_1_bss_0->rotSpeed = 2.0f;
    HuPrcChildCreate(fn_1_7D74, 0x64, 0x1000, 0, HuPrcCurrentGet());
    HuPrcVSleep();
}

void fn_1_A524(void)
{
    Vec pv;
    M505_PLAYER *p;
    s8 *movie;
    s8 win;
    s32 cnt;
    s32 i;
    s32 n;
    s8 bonus;
    s16 b16;
    s16 c16;

#ifdef TARGET_PC
    win = 0;
#endif
    p = lbl_1_bss_0->player;
    pv.x = lbl_1_data_60[3];
    pv.y = lbl_1_data_60[4];
    pv.z = lbl_1_data_60[5];
    for (i = 0; i < 4; i++) {
        p->motion = -1;
        if (p->motion != 0) {
            CharMotionSet(p->charNo, p->mot0);
            p->motion = 0;
        }
        Hu3DModelAttrSet(p->modelId, 0x40000001);
        Hu3DModelAttrReset(p->modelId, 1);
        Hu3DModelPosSet(p->modelId, lbl_1_data_30[i * 3], lbl_1_data_30[i * 3 + 1], lbl_1_data_30[i * 3 + 2]);
        p++;
    }
    lbl_1_bss_0->pos = pv;
    Hu3DModelPosSet(lbl_1_bss_0->mdlPodium, pv.x, pv.y, pv.z);
    Hu3DModelRotSet(lbl_1_bss_0->mdlPodium, 0.0f, 45.0f, 0.0f);
    if (lbl_1_bss_4->entryNo == (s8)lbl_1_bss_4->mgCount) {
        fn_1_33AC();
    }
    fn_1_31B0();
    Hu3DMotionTimeSet(lbl_1_bss_0->mdl[0], 240.0f);
    Hu3DMotionTimeSet(lbl_1_bss_0->mdl[1], 240.0f);
    Hu3DMotionTimeSet(lbl_1_bss_0->mdl[5], Hu3DMotionMaxTimeGet(lbl_1_bss_0->mdl[5]));
    cnt = 0;
    switch ((s8)lbl_1_bss_4->gameType) {
        case 0:
            if ((lbl_1_bss_4->flag & 1) != 0) {
                OSReport("\x83o\x83g\x83\x8b\x83Q\x81[\x83\x80\x81I\x81I\n");
                n = 4;
                while (n-- != 0) {
                    b16 = GwPlayer[n].mgCoinBonus;
                    OSReport("\t Player %d = Rank %d\n", n, b16);
                    c16 = GwPlayer[n].mgCoinBonus;
                    if (c16 == 0) {
                        win = n;
                        cnt += 1;
                    }
                }
            } else {
                OSReport("VS 4 \x83Q\x81[\x83\x80\x81I\x81I\n");
                n = 4;
                while (n-- != 0) {
                    b16 = GwPlayer[n].mgCoinBonus;
                    OSReport("\t Player %d = Bounus Coin %d\n", n, b16);
                    c16 = GwPlayer[n].mgCoin;
                    OSReport("\t          = Coin %d\n", c16);
                    b16 = GwPlayer[n].mgCoinBonus;
                    if (b16 >= 10 || (c16 = GwPlayer[n].mgCoin, !(c16 < 10))) {
                        win = n;
                        cnt += 1;
                    }
                }
            }
            break;
        case 2:
            p = lbl_1_bss_0->player;
            OSReport("2 VS 2 \x83Q\x81[\x83\x80\x81I\x81I\n");
            for (i = 0; i < 4; i++) {
                b16 = GwPlayer[p->playerNo].mgCoinBonus;
                OSReport("\t Team %d = Coin %d\n", i, b16);
                c16 = GwPlayer[p->playerNo].mgCoin;
                OSReport("\t        = Coin %d\n", c16);
                b16 = GwPlayer[p->playerNo].mgCoinBonus;
                if (b16 >= 10 || (c16 = GwPlayer[p->playerNo].mgCoin, !(c16 < 10))) {
                    switch (i) {
                        case 0:
                        case 1:
                            win = 0;
                            break;
                        case 2:
                        case 3:
                            win = 1;
                            break;
                    }
                    cnt += 1;
                }
                p++;
            }
            if (cnt == 2) {
                cnt = 1;
            }
            break;
        case 1:
            p = lbl_1_bss_0->player;
            OSReport("1 VS 3 \x83Q\x81[\x83\x80\x81I\x81I\n");
            for (i = 0; i < 4; i++) {
                b16 = GwPlayer[p->playerNo].mgCoinBonus;
                OSReport("\t Team %d = Coin %d\n", i, b16);
                c16 = GwPlayer[p->playerNo].mgCoin;
                OSReport("\t        = Coin %d\n", c16);
                b16 = GwPlayer[p->playerNo].mgCoinBonus;
                if (b16 >= 10 || (c16 = GwPlayer[p->playerNo].mgCoin, !(c16 < 10))) {
                    switch (i) {
                        case 0:
                            win = 0;
                            cnt = 3;
                            break;
                        case 1:
                        case 2:
                        case 3:
                            win = 1;
                            cnt += 1;
                            break;
                    }
                }
                p++;
            }
            if (cnt == 3) {
                cnt = 1;
            }
            break;
    }
    if (cnt != 1) {
        if (lbl_1_bss_0->bgmId < 0) {
            lbl_1_bss_0->bgmId = HuAudBGMPlay(0x23);
        }
        WipeCreate(1, 5, 0x3C);
        while (WipeCheckEnd() != 0) {
            HuPrcVSleep();
        }
        lbl_1_bss_0->flag2 = lbl_1_bss_0->flag2 | 8;
        lbl_1_bss_0->winner = -1;
        i = 4;
        while (i-- != 0) {
            p = &lbl_1_bss_0->player[i];
            fn_1_974(p, 1);
            if (p->motion != 4) {
                CharMotionShiftSet(p->charNo, p->mot4, 0.0f, 15.0f, 0);
                p->motion = 4;
            }
        }
        cnt = 0xB4;
        HuPrcChildCreate(fn_1_161E8, 0x64, 0x1000, 0, HuPrcCurrentGet());
        HuPrcVSleep();
        while ((lbl_1_bss_0->flag & 0x20) == 0 || cnt >= 0) {
            cnt -= 1;
            HuPrcVSleep();
        }
        i = 4;
        while (i-- != 0) {
            p = &lbl_1_bss_0->player[i];
            if (p->motion != 0) {
                CharMotionShiftSet(p->charNo, p->mot0, 0.0f, 15.0f, 0x40000001);
                p->motion = 0;
            }
        }
    } else {
        WipeCreate(1, 5, 0x3C);
        while (WipeCheckEnd() != 0) {
            HuPrcVSleep();
        }
        lbl_1_bss_0->winner = win;
    }
    if ((cnt != 1 || lbl_1_bss_4->score[win] + 1 < lbl_1_bss_4->maxScore) && (lbl_1_bss_4->flag & 4) == 0
        && (lbl_1_bss_0->flag2 & 0x100000) == 0) {
        i = 7;
        while (i-- != 0) {
            Hu3DModelAttrReset(lbl_1_bss_0->parMan[i].id, 1);
            lbl_1_bss_0->parMan[i].offset.x = lbl_1_data_320[0];
            lbl_1_bss_0->parMan[i].offset.y = lbl_1_data_320[1];
            lbl_1_bss_0->parMan[i].offset.z = lbl_1_data_320[2];
            PSVECAdd(&lbl_1_bss_0->parMan[i].offset, (Vec *)&lbl_1_data_278[i * 3], &lbl_1_bss_0->parMan[i].offset);
            lbl_1_bss_0->parMan[i].pos = lbl_1_bss_0->parMan[i].offset;
            Hu3DModelPosSet(lbl_1_bss_0->parMan[i].id, lbl_1_bss_0->parMan[i].pos.x, lbl_1_bss_0->parMan[i].pos.y,
                lbl_1_bss_0->parMan[i].pos.z);
            Hu3DModelLayerSet(lbl_1_bss_0->parMan[i].id, 0);
            Hu3DModelCameraSet(lbl_1_bss_0->parMan[i].id, 0x10);
        }
        lbl_1_bss_0->flag2 = lbl_1_bss_0->flag2 | 0x100000;
        HuPrcChildCreate(fn_1_6BE8, 0x64, 0x1000, 0, HuPrcCurrentGet());
    }
}

void fn_1_B2D8(void)
{
    M505_PLAYER *p;
    s32 cnt;
    s32 i;
    s32 n;

    cnt = 1;
    HuPrcChildCreate(fn_1_20BC, 0x64, 0x1000, 0, HuPrcCurrentGet());
    p = lbl_1_bss_0->player;
    switch ((s8)lbl_1_bss_4->gameType) {
        case 0:
            for (i = 0; i < 4; i++) {
                if (lbl_1_bss_0->winner == p->playerNo) {
                    fn_1_974(p, 0);
                    if (p->motion != 3) {
                        CharMotionShiftSet(p->charNo, p->mot3, 0.0f, 15.0f, 0);
                        p->motion = 3;
                    }
                } else if (p->motion != 4) {
                    CharMotionShiftSet(p->charNo, p->mot4, 0.0f, 15.0f, 0);
                    p->motion = 4;
                }
                p++;
            }
            break;
        case 2:
            if (lbl_1_bss_0->winner == 0) {
                if (p->motion != 3) {
                    CharMotionShiftSet(p->charNo, p->mot3, 0.0f, 15.0f, 0);
                    p->motion = 3;
                }
                fn_1_974(p, 0);
                p++;
                if (p->motion != 3) {
                    CharMotionShiftSet(p->charNo, p->mot3, 0.0f, 15.0f, 0);
                    p->motion = 3;
                }
                fn_1_974(p, 0);
                p++;
                if (p->motion != 4) {
                    CharMotionShiftSet(p->charNo, p->mot4, 0.0f, 15.0f, 0);
                    p->motion = 4;
                }
                p++;
                if (p->motion != 4) {
                    CharMotionShiftSet(p->charNo, p->mot4, 0.0f, 15.0f, 0);
                    p->motion = 4;
                }
            } else {
                if (p->motion != 4) {
                    CharMotionShiftSet(p->charNo, p->mot4, 0.0f, 15.0f, 0);
                    p->motion = 4;
                }
                p++;
                if (p->motion != 4) {
                    CharMotionShiftSet(p->charNo, p->mot4, 0.0f, 15.0f, 0);
                    p->motion = 4;
                }
                p++;
                if (p->motion != 3) {
                    CharMotionShiftSet(p->charNo, p->mot3, 0.0f, 15.0f, 0);
                    p->motion = 3;
                }
                fn_1_974(p, 0);
                p++;
                if (p->motion != 3) {
                    CharMotionShiftSet(p->charNo, p->mot3, 0.0f, 15.0f, 0);
                    p->motion = 3;
                }
                fn_1_974(p, 0);
            }
            break;
        case 1:
            if (lbl_1_bss_0->winner == 0) {
                if (p->motion != 3) {
                    CharMotionShiftSet(p->charNo, p->mot3, 0.0f, 15.0f, 0);
                    p->motion = 3;
                }
                fn_1_974(p, 0);
                p++;
                if (p->motion != 4) {
                    CharMotionShiftSet(p->charNo, p->mot4, 0.0f, 15.0f, 0);
                    p->motion = 4;
                }
                p++;
                if (p->motion != 4) {
                    CharMotionShiftSet(p->charNo, p->mot4, 0.0f, 15.0f, 0);
                    p->motion = 4;
                }
                p++;
                if (p->motion != 4) {
                    CharMotionShiftSet(p->charNo, p->mot4, 0.0f, 15.0f, 0);
                    p->motion = 4;
                }
            } else {
                if (p->motion != 4) {
                    CharMotionShiftSet(p->charNo, p->mot4, 0.0f, 15.0f, 0);
                    p->motion = 4;
                }
                p++;
                if (p->motion != 3) {
                    CharMotionShiftSet(p->charNo, p->mot3, 0.0f, 15.0f, 0);
                    p->motion = 3;
                }
                fn_1_974(p, 0);
                p++;
                if (p->motion != 3) {
                    CharMotionShiftSet(p->charNo, p->mot3, 0.0f, 15.0f, 0);
                    p->motion = 3;
                }
                fn_1_974(p, 0);
                p++;
                if (p->motion != 3) {
                    CharMotionShiftSet(p->charNo, p->mot3, 0.0f, 15.0f, 0);
                    p->motion = 3;
                }
                fn_1_974(p, 0);
            }
            break;
    }
    HuPrcChildCreate(fn_1_16250, 0x64, 0x1000, 0, HuPrcCurrentGet());
    HuPrcVSleep();
    while ((lbl_1_bss_0->flag & 0x40) == 0) {
        HuPrcVSleep();
    }
    fn_1_12AE4(1);
    while (fn_1_12B08() == 0) {
        HuPrcVSleep();
    }
    n = 0x1E;
    while (n-- != 0) {
        HuPrcVSleep();
    }
    fn_1_12AE4(4);
    while (fn_1_12B08() == 0) {
        HuPrcVSleep();
    }
    n = 0x3C;
    while (n-- != 0) {
        HuPrcVSleep();
    }
    if ((lbl_1_bss_0->flag2 & 0x20) != 0) {
        lbl_1_bss_0->flag2 = lbl_1_bss_0->flag2 & ~0x20;
        HuWinExClose(lbl_1_bss_0->winId);
        HuWinHomeClear(lbl_1_bss_0->winId);
    }
    HuPrcChildCreate(fn_1_16D34, 0x64, 0x1000, 0, HuPrcCurrentGet());
    HuPrcVSleep();
    while ((lbl_1_bss_0->flag & 0x1000) == 0) {
        HuPrcVSleep();
    }
    lbl_1_bss_4->score[lbl_1_bss_0->winner] = lbl_1_bss_4->score[lbl_1_bss_0->winner] + 1;
    if (lbl_1_bss_4->score[lbl_1_bss_0->winner] == lbl_1_bss_4->maxScore
        && ((lbl_1_bss_4->winner = lbl_1_bss_0->winner, fn_1_64C(NULL, NULL, NULL) == 0)
            || (lbl_1_bss_4->flag & 4) != 0)) {
        cnt = 0;
    }
    if (cnt != 0) {
        fn_1_12AE4(2);
    }
    i = 4;
    while (i-- != 0) {
        p = &lbl_1_bss_0->player[i];
        if (p->motion != 0) {
            CharMotionShiftSet(p->charNo, p->mot0, 0.0f, 15.0f, 0x40000001);
            p->motion = 0;
        }
    }
    n = 0x5A;
    while (n-- != 0) {
        HuPrcVSleep();
    }
}

void fn_1_BE50(void)
{
    Vec pos;
    Vec scale;
    s8 key[4];
    u8 idx[4];
    OMOVLHIS *his;
    HU3DMODELID m;
    s32 i;
    s32 j;
    s32 n;
    s8 k;
    s8 c;
    s8 t;

    if (fn_1_64C(NULL, NULL, NULL) != 0 && (lbl_1_bss_4->flag & 4) == 0) {
        fn_1_D184();
    } else {
        for (k = 0; k < lbl_1_bss_4->teamCount; k++) {
            idx[k] = k;
            key[k] = lbl_1_bss_4->score[k];
        }
        c = lbl_1_bss_4->teamCount;
        for (i = 0; i < c - 1; i++) {
            for (j = i + 1; j < c; j++) {
                if (key[i] > key[j]) {
                    t = key[i];
                    key[i] = key[j];
                    key[j] = t;
                    t = idx[i];
                    idx[i] = idx[j];
                    idx[j] = t;
                }
            }
        }
        for (k = 0; k < lbl_1_bss_4->teamCount; k++) {
            OSReport("Player %d  Rank = %d [score : %d]\n", (s8)idx[k], k, key[k]);
        }
        for (k = 0; k < lbl_1_bss_4->teamCount; k++) {
            lbl_1_bss_4->rank[k] = (s8)idx[lbl_1_bss_4->teamCount - 1 - k];
        }
    }
    for (k = 0; k < lbl_1_bss_4->teamCount; k++) {
        OSReport("Rank : %d Player %d [score : %d]\n", k, lbl_1_bss_4->rank[k], lbl_1_bss_4->score[lbl_1_bss_4->rank[k]]);
    }
    if (lbl_1_bss_0->bgmId < 0) {
        lbl_1_bss_0->bgmId = HuAudBGMPlay(0x23);
    }
    HuPrcChildCreate(fn_1_169E0, 0x64, 0x1000, 0, HuPrcCurrentGet());
    HuPrcVSleep();
    while ((lbl_1_bss_0->flag & 0x200) == 0) {
        HuPrcVSleep();
    }
    lbl_1_bss_0->flag2 = lbl_1_bss_0->flag2 & ~0x200000;
    lbl_1_bss_0->camMotionType = 2;
    lbl_1_bss_0->rotSpeed = 2.0f;
    HuPrcChildCreate(fn_1_7D74, 0x64, 0x1000, 0, HuPrcCurrentGet());
    HuPrcVSleep();
    if (fn_1_12B4C() == 0) {
        fn_1_12AE4(1);
        while (fn_1_12B08() == 0) {
            HuPrcVSleep();
        }
    }
    while ((lbl_1_bss_0->flag2 & 0x200000) == 0) {
        HuPrcVSleep();
    }
    HuPrcChildCreate(fn_1_16574, 0x64, 0x1000, 0, HuPrcCurrentGet());
    HuPrcVSleep();
    while ((lbl_1_bss_0->flag & 0x80) == 0) {
        HuPrcVSleep();
    }
    if ((lbl_1_bss_0->flag2 & 0x20) != 0) {
        lbl_1_bss_0->flag2 = lbl_1_bss_0->flag2 & ~0x20;
        HuWinExClose(lbl_1_bss_0->winId);
        HuWinHomeClear(lbl_1_bss_0->winId);
    }
    lbl_1_bss_0->flag2 = lbl_1_bss_0->flag2 & ~0x200000;
    lbl_1_bss_0->camMotionType = 0;
    lbl_1_bss_0->rotSpeed = 2.0f;
    HuPrcChildCreate(fn_1_7D74, 0x64, 0x1000, 0, HuPrcCurrentGet());
    HuPrcVSleep();
    while ((lbl_1_bss_0->flag2 & 0x200000) == 0) {
        HuPrcVSleep();
    }
    n = 0x3C;
    while (n-- != 0) {
        HuPrcVSleep();
    }
    k = lbl_1_bss_4->score[lbl_1_bss_4->winner];
    while (k-- != 0) {
        fn_1_138B8(&pos, &scale, lbl_1_bss_4->winner, k, (s8)lbl_1_bss_4->gameType, 40.0f,
            lbl_1_bss_4->camPos.z - lbl_1_data_30[2]);
        lbl_1_bss_0->item[k].pos = pos;
        lbl_1_bss_0->item[k].state = 0;
        lbl_1_bss_0->item[k].scale = scale;
        m = lbl_1_bss_0->item[k].modelId;
        Hu3DModelPosSetV(m, &pos);
        Hu3DModelScaleSetV(m, &scale);
        Hu3DModelAttrReset(m, 1);
        Hu3DModelTPLvlSet(m, 0.0f);
    }
    lbl_1_bss_0->seId = HuAudFXPlay(0x491);
    fn_1_12AE4(5);
    while (fn_1_12B08() == 0) {
        HuPrcVSleep();
    }
    n = 0x1E;
    while (n-- != 0) {
        HuPrcVSleep();
    }
    fn_1_12AE4(2);
    while (fn_1_12B08() == 0) {
        HuPrcVSleep();
    }
    lbl_1_bss_0->parManMaxTime = (s16)Hu3DMotionMaxTimeGet(lbl_1_bss_0->mdl[16]);
    lbl_1_bss_0->flag2 = lbl_1_bss_0->flag2 | 0x90000;
    HuPrcChildCreate(fn_1_3C40, 0x64, 0x1000, 0, HuPrcCurrentGet());
    lbl_1_bss_0->flag2 = lbl_1_bss_0->flag2 & ~0x4000;
    HuPrcChildCreate(fn_1_4114, 0x64, 0x1500, 0, HuPrcCurrentGet());
    while ((lbl_1_bss_0->flag2 & 0x4000) == 0) {
        HuPrcVSleep();
    }
    lbl_1_bss_4->flag = lbl_1_bss_4->flag | 2;
    his = omOvlHisGet(0);
    omOvlHisChg(0, his->ovl, 2, his->stat);
}

void fn_1_C638(void)
{
    OMOVLHIS *his;
    s8 mgIdx;
    s32 mgNo;
    s32 n;

    if ((lbl_1_bss_0->flag2 & 1) != 0) {
        HuPrcChildCreate(fn_1_15F88, 0x64, 0x1000, 0, HuPrcCurrentGet());
        HuPrcVSleep();
        while ((lbl_1_bss_0->flag & 2) == 0) {
            HuPrcVSleep();
        }
    } else {
        HuPrcChildCreate(fn_1_16018, 0x64, 0x1000, 0, HuPrcCurrentGet());
        HuPrcVSleep();
        while ((lbl_1_bss_0->flag & 4) == 0) {
            HuPrcVSleep();
        }
    }
    lbl_1_bss_0->flag2 = lbl_1_bss_0->flag2 & ~0x200000;
    lbl_1_bss_0->camMotionType = 2;
    lbl_1_bss_0->rotSpeed = 2.0f;
    HuPrcChildCreate(fn_1_7D74, 0x64, 0x1000, 0, HuPrcCurrentGet());
    HuPrcVSleep();
    if ((lbl_1_bss_0->flag2 & 0x400) != 0) {
        lbl_1_bss_0->itemAnimState = 1;
        while (lbl_1_bss_0->itemAnimState != 0) {
            HuPrcVSleep();
        }
        lbl_1_bss_0->flag2 = lbl_1_bss_0->flag2 | 0x800;
        lbl_1_bss_0->flag2 = lbl_1_bss_0->flag2 & ~0x1000;
        while ((lbl_1_bss_0->flag2 & 0x1000) == 0) {
            HuPrcVSleep();
        }
    }
    while (lbl_1_bss_0->itemAnimState != 0) {
        HuPrcVSleep();
    }
    while ((lbl_1_bss_0->flag2 & 0x1000) == 0) {
        HuPrcVSleep();
    }
    HuPrcChildCreate(fn_1_16080, 0x64, 0x1000, 0, HuPrcCurrentGet());
    HuPrcVSleep();
    while ((lbl_1_bss_0->flag & 8) == 0) {
        HuPrcVSleep();
    }
    HuPrcChildCreate(fn_1_160F0, 0x64, 0x1000, 0, HuPrcCurrentGet());
    HuPrcVSleep();
    while ((lbl_1_bss_0->flag & 0x10) == 0) {
        HuPrcVSleep();
    }
    mgNo = lbl_1_bss_4->mgTbl[lbl_1_bss_4->order[lbl_1_bss_4->entryNo]].mgNo;
    mgIdx = lbl_1_bss_4->mgTbl[lbl_1_bss_4->order[lbl_1_bss_4->entryNo]].mgIdx;
    if (mgIdx + 0x1F5 == 0x207) {
        GwSystem.subGameNo = lbl_1_bss_0->movieNo;
    }
    lbl_1_bss_4->entryNo = lbl_1_bss_4->entryNo + 1;
    if ((lbl_1_bss_0->flag2 & 2) != 0) {
        lbl_1_bss_0->flag2 = lbl_1_bss_0->flag2 & ~0x200000;
        lbl_1_bss_0->camMotionType = 0;
        lbl_1_bss_0->rotSpeed = 2.0f;
        HuPrcChildCreate(fn_1_7D74, 0x64, 0x1000, 0, HuPrcCurrentGet());
        HuPrcVSleep();
        HuPrcChildCreate(fn_1_16AC4, 0x64, 0x1000, 0, HuPrcCurrentGet());
        HuPrcVSleep();
        while ((lbl_1_bss_0->flag & 0x800) == 0) {
            HuPrcVSleep();
        }
        if ((lbl_1_bss_0->flag2 & 0x20) != 0) {
            lbl_1_bss_0->flag2 = lbl_1_bss_0->flag2 & ~0x20;
            HuWinExClose(lbl_1_bss_0->winId);
            HuWinHomeClear(lbl_1_bss_0->winId);
        }
        n = 0x1E;
        while (n-- != 0) {
            HuPrcVSleep();
        }
        his = omOvlHisGet(0);
        omOvlHisChg(0, his->ovl, 3, his->stat);
        return;
    }
    HuAudSStreamFadeOut(lbl_1_bss_0->bgmId, 0x3E8);
    lbl_1_bss_0->bgmId = -1;
    lbl_1_bss_0->mgNo = mgIdx;
    HuDataDirClose(0x980000);
    HuDataDirClose(0x270000);
    CharModelDataClose(-1);
    if ((lbl_1_bss_0->flag2 & 4) == 0) {
        HuPrcChildCreate(fn_1_2E8, 0xC8, 0x3000, 0, HuPrcCurrentGet());
    } else {
        lbl_1_bss_0->loadState = 1;
    }
    WipeCreate(2, 6, 0x3C);
    while (WipeCheckEnd() != 0) {
        HuPrcVSleep();
    }
    HuTHPClose();
    HuPrcVSleep();
    while (lbl_1_bss_0->loadState == 0) {
        HuPrcVSleep();
    }
    while (HuTHPProcCheck() != 0) {
        HuPrcVSleep();
    }
    GwSystem.mgNo = mgNo - 0x1F5;
    GWMgUnlockSet(mgNo);
    GwSystem.mgNo = mgNo - 0x1F5;
    if ((lbl_1_bss_0->flag2 & 4) != 0) {
        GwSystem.mgInstF = 1;
    } else {
        GwSystem.mgInstF = 0;
    }
    omOvlCallEx(DLL_instdll, 1, 0, 0);
    while (TRUE) {
        HuPrcVSleep();
    }
}

void fn_1_CCE4(f32 *p, s32 n)
{
    f32 a;
    f32 b;

    a = (f32)frandmod(n * 0x14) / 10.0f;
    b = a - (f32)n;
    p[0] += b;
    a = (f32)frandmod(n * 0x14) / 10.0f;
    b = a - (f32)n;
    p[1] = p[1] + b;
    p[2] = p[2] + (((f32)frandmod(n * 0x14) / 10.0f) - (f32)n);
}

void fn_1_CE5C(void)
{
    f32 ang[3];
    Vec v;
    s32 i;

    i = 3;
    while (i-- != 0) {
        ang[i] = (f32)frandmod(0x168);
    }
    while ((lbl_1_bss_0->flag2 & 0x800000) != 0) {
        i = 3;
        while (i-- != 0) {
            ang[i] += 1.0f;
            if (ang[i] >= 360.0f) {
                ang[i] -= 360.0f;
            }
            v = lbl_1_bss_0->parMan[i].pos;
            v.y = (f32)((f64)v.y + (40.0 * sin((3.141592653589793 * (f64)ang[i]) / 180.0)));
            Hu3DModelPosSetV(lbl_1_bss_0->parMan[i].id, &v);
        }
        HuPrcVSleep();
    }
    i = 3;
    while (i-- != 0) {
        ang[i] += 1.0f;
        if (ang[i] >= 360.0f) {
            ang[i] -= 360.0f;
        }
        v = lbl_1_bss_0->parMan[i].pos;
        v.y = (f32)((f64)v.y + (40.0 * sin((3.141592653589793 * (f64)ang[i]) / 180.0)));
        lbl_1_bss_0->parMan[i].pos = v;
    }
    HuPrcEnd();
}

void fn_1_D130(void)
{
    lbl_1_bss_0->flag2 |= 0x00800000;
    HuPrcChildCreate(fn_1_CE5C, 0x63, 0x1000, 0, HuPrcCurrentGet());
}

void fn_1_112B8(void)
{
    s32 ord[4];
    s32 chars[4];
    OMOVLHIS *his;
    M505_PLAYER *p;
    s32 i;

    WipeCreate(2, 6, 0x3C);
    while (WipeCheckEnd() != 0) {
        HuPrcVSleep();
    }
    i = 4;
    while (i-- != 0) {
        chars[i] = lbl_1_bss_0->player[i].charNo;
    }
    lbl_1_bss_0->flag2 = lbl_1_bss_0->flag2 & ~0x80000;
    Hu3DCameraPosSetV(0x10, &lbl_1_bss_4->camPos, &lbl_1_bss_4->camUp, &lbl_1_bss_4->camTarget);
    Hu3DCameraPerspectiveSet(0x10, lbl_1_bss_4->fov, 20.0f, 60000.0f, 1.2f);
    Hu3DModelAttrSet(lbl_1_bss_0->mdlItem, 1);
    Hu3DModelAttrSet(lbl_1_bss_0->mdlPodium, 1);
    p = lbl_1_bss_0->player;
    for (i = 0; i < 4; i++) {
        if (p->motion != 0) {
            CharMotionShiftSet(p->charNo, p->mot0, 0.0f, 15.0f, 0x40000001);
            p->motion = 0;
        }
        p++;
    }
    switch ((s8)lbl_1_bss_4->gameType) {
        case 0:
            fn_1_13C1C((s8)lbl_1_bss_4->winner, (s8)lbl_1_bss_4->gameType, (s8)lbl_1_bss_4->maxScore, lbl_1_bss_4->rank, chars);
            break;
        case 2:
            if ((s8)lbl_1_bss_4->winner == 0) {
                ord[0] = 0;
                ord[1] = 1;
                ord[2] = 2;
                ord[3] = 3;
                fn_1_13C1C(0, (s8)lbl_1_bss_4->gameType, (s8)lbl_1_bss_4->maxScore, ord, chars);
            } else {
                ord[0] = 2;
                ord[1] = 3;
                ord[2] = 0;
                ord[3] = 1;
                fn_1_13C1C(1, (s8)lbl_1_bss_4->gameType, (s8)lbl_1_bss_4->maxScore, ord, chars);
            }
            break;
        case 1:
            if ((s8)lbl_1_bss_4->winner == 0) {
                ord[0] = 0;
                ord[1] = 1;
                ord[2] = 2;
                ord[3] = 3;
                fn_1_13C1C(0, (s8)lbl_1_bss_4->gameType, (s8)lbl_1_bss_4->maxScore, ord, chars);
            } else {
                ord[0] = 1;
                ord[1] = 2;
                ord[2] = 3;
                ord[3] = 0;
                fn_1_13C1C(1, (s8)lbl_1_bss_4->gameType, (s8)lbl_1_bss_4->maxScore, ord, chars);
            }
            break;
    }
    p = lbl_1_bss_0->player;
    for (i = 0; i < 4; i++) {
        Hu3DModelAttrSet(p->modelId, 1);
        p++;
    }
    if ((lbl_1_bss_0->flag2 & 0x20) != 0) {
        lbl_1_bss_0->flag2 = lbl_1_bss_0->flag2 & ~0x20;
        HuWinExClose(lbl_1_bss_0->winId);
        HuWinHomeClear(lbl_1_bss_0->winId);
    }
    if (lbl_1_bss_0->bgmId < 0) {
        lbl_1_bss_0->bgmId = HuAudBGMPlay(0x48);
    }
    WipeCreate(1, 5, 0x3C);
    while (WipeCheckEnd() != 0) {
        HuPrcVSleep();
    }
    p = lbl_1_bss_0->player;
    switch ((s8)lbl_1_bss_4->gameType) {
        case 0:
            for (i = 0; i < 4; i++) {
                OSReport("[No : %d] Rank %d\n", i, lbl_1_bss_4->rank[i]);
                p = &lbl_1_bss_0->player[lbl_1_bss_4->rank[i]];
                switch (i) {
                    case 0:
                        if (p->motion != 3) {
                            CharMotionShiftSet(p->charNo, p->mot3, 0.0f, 15.0f, 0);
                            p->motion = 3;
                        }
                        break;
                    case 1:
                        if (p->motion != 0) {
                            CharMotionShiftSet(p->charNo, p->mot0, 0.0f, 15.0f, 0);
                            p->motion = 0;
                        }
                        break;
                    case 2:
                        if (p->motion != 4) {
                            CharMotionShiftSet(p->charNo, p->mot4, 0.0f, 15.0f, 0);
                            p->motion = 4;
                        }
                        break;
                    case 3:
                        if (p->motion != 4) {
                            CharMotionShiftSet(p->charNo, p->mot4, 0.0f, 15.0f, 0);
                            p->motion = 4;
                        }
                        break;
                }
            }
            break;
        case 2:
            if ((s8)lbl_1_bss_4->winner == 0) {
                if (p->motion != 3) {
                    CharMotionShiftSet(p->charNo, p->mot3, 0.0f, 15.0f, 0);
                    p->motion = 3;
                }
                p++;
                if (p->motion != 3) {
                    CharMotionShiftSet(p->charNo, p->mot3, 0.0f, 15.0f, 0);
                    p->motion = 3;
                }
                p++;
                if (p->motion != 4) {
                    CharMotionShiftSet(p->charNo, p->mot4, 0.0f, 15.0f, 0);
                    p->motion = 4;
                }
                p++;
                if (p->motion != 4) {
                    CharMotionShiftSet(p->charNo, p->mot4, 0.0f, 15.0f, 0);
                    p->motion = 4;
                }
            } else {
                if (p->motion != 4) {
                    CharMotionShiftSet(p->charNo, p->mot4, 0.0f, 15.0f, 0);
                    p->motion = 4;
                }
                p++;
                if (p->motion != 4) {
                    CharMotionShiftSet(p->charNo, p->mot4, 0.0f, 15.0f, 0);
                    p->motion = 4;
                }
                p++;
                if (p->motion != 3) {
                    CharMotionShiftSet(p->charNo, p->mot3, 0.0f, 15.0f, 0);
                    p->motion = 3;
                }
                p++;
                if (p->motion != 3) {
                    CharMotionShiftSet(p->charNo, p->mot3, 0.0f, 15.0f, 0);
                    p->motion = 3;
                }
            }
            break;
        case 1:
            if ((s8)lbl_1_bss_4->winner == 0) {
                if (p->motion != 3) {
                    CharMotionShiftSet(p->charNo, p->mot3, 0.0f, 15.0f, 0);
                    p->motion = 3;
                }
                p++;
                if (p->motion != 4) {
                    CharMotionShiftSet(p->charNo, p->mot4, 0.0f, 15.0f, 0);
                    p->motion = 4;
                }
                p++;
                if (p->motion != 4) {
                    CharMotionShiftSet(p->charNo, p->mot4, 0.0f, 15.0f, 0);
                    p->motion = 4;
                }
                p++;
                if (p->motion != 4) {
                    CharMotionShiftSet(p->charNo, p->mot4, 0.0f, 15.0f, 0);
                    p->motion = 4;
                }
            } else {
                if (p->motion != 4) {
                    CharMotionShiftSet(p->charNo, p->mot4, 0.0f, 15.0f, 0);
                    p->motion = 4;
                }
                p++;
                if (p->motion != 3) {
                    CharMotionShiftSet(p->charNo, p->mot3, 0.0f, 15.0f, 0);
                    p->motion = 3;
                }
                p++;
                if (p->motion != 3) {
                    CharMotionShiftSet(p->charNo, p->mot3, 0.0f, 15.0f, 0);
                    p->motion = 3;
                }
                p++;
                if (p->motion != 3) {
                    CharMotionShiftSet(p->charNo, p->mot3, 0.0f, 15.0f, 0);
                    p->motion = 3;
                }
            }
            break;
    }
    while ((lbl_1_bss_0->flag2 & 0x20000) == 0) {
        HuPrcVSleep();
    }
    HuPrcChildCreate(fn_1_16A48, 0x64, 0x1000, 0, HuPrcCurrentGet());
    HuPrcVSleep();
    while ((lbl_1_bss_0->flag & 0x400) == 0) {
        HuPrcVSleep();
    }
    if ((lbl_1_bss_0->flag2 & 0x10) != 0) {
        HuAudSStreamFadeOut(lbl_1_bss_0->bgmId, 0x3E8);
        lbl_1_bss_0->bgmId = -1;
        WipeCreate(2, 6, 0x3C);
        while (WipeCheckEnd() != 0) {
            HuPrcVSleep();
        }
        fn_1_3560();
    } else {
        his = omOvlHisGet(0);
        omOvlHisChg(0, his->ovl, 3, his->stat);
    }
    lbl_1_bss_0->flag2 = lbl_1_bss_0->flag2 & ~0x400000;
}

void fn_1_11F2C(void)
{
    Vec v;
    CAMMOTIONWORK *buf;
    f32 f;
    int max;
    s32 i;

    buf = &lbl_1_bss_0->camPath[1];
    max = 0x32;
    lbl_1_bss_0->camPathNum = CamMotionExPathGet(lbl_1_bss_0->mdlCam1, 6, 120.0f, 0, &buf, &max);
    lbl_1_bss_0->camPath[0].time = 0.0f;
    lbl_1_bss_0->camPath[0].pos = lbl_1_bss_4->camPos;
    lbl_1_bss_0->camPath[0].up = lbl_1_bss_4->camUp;
    lbl_1_bss_0->camPath[0].target = lbl_1_bss_4->camTarget;
    lbl_1_bss_0->camPath[0].fov = lbl_1_bss_4->fov;
    lbl_1_bss_0->camPath[1].time = 1.0f;
    lbl_1_bss_0->camPath[2].time = 120.0f;
    lbl_1_bss_0->camPathNum = lbl_1_bss_0->camPathNum + 1;
    lbl_1_data_1C4[0] = lbl_1_bss_0->camPath[2].target.x - 9000.0f;
    lbl_1_data_1C4[1] = 18000.0f + lbl_1_bss_0->camPath[2].target.y;
    lbl_1_data_1C4[2] = 5000.0f + lbl_1_bss_0->camPath[2].target.z;
    lbl_1_data_60[6] = 0.5f * lbl_1_bss_0->camPath[2].target.x;
    lbl_1_data_60[7] = 0.5f * lbl_1_bss_0->camPath[2].target.y;
    lbl_1_data_60[8] = 0.5f * lbl_1_bss_0->camPath[2].target.z;
    HuAudFXPlay(0x49D);
    lbl_1_bss_0->flag2 = lbl_1_bss_0->flag2 & ~0x100;
    HuPrcChildCreate(fn_1_1EA0, 0x64, 0x1000, 0, HuPrcCurrentGet());
    HuPrcVSleep();
    lbl_1_bss_0->flag2 = lbl_1_bss_0->flag2 | 0x8000;
    HuPrcChildCreate(fn_1_6504, 0x64, 0x1500, 0, HuPrcCurrentGet());
    lbl_1_bss_0->posTo = lbl_1_bss_0->pos;
    lbl_1_bss_0->posFrom.x = lbl_1_data_60[6];
    lbl_1_bss_0->posFrom.y = lbl_1_data_60[7];
    lbl_1_bss_0->posFrom.z = lbl_1_data_60[8];
    lbl_1_bss_0->camMoveSpeed = 0xB4;
    lbl_1_bss_0->flag2 = lbl_1_bss_0->flag2 & ~0x80;
    HuPrcChildCreate(fn_1_267C, 0x64, 0x1000, 0, HuPrcCurrentGet());
    i = 0x1E;
    while (i-- != 0) {
        HuPrcVSleep();
    }
    i = 0x1E;
    while (i-- != 0) {
        f = (f32)i / 30.0f;
        Hu3DModelTPLvlSet(lbl_1_bss_0->mdlPodium, f);
        Hu3DModelScaleSet(lbl_1_bss_0->mdlPodium, f, f, f);
        HuPrcVSleep();
    }
    v.x = 600.0f;
    v.y = 100.0f;
    v.z = 14000.0f;
    Hu3D2Dto3D(&v, 0x10, &v);
    OSReport("%3.3f, %3.3f, %3.3f\n", v.x, v.y, v.z);
}
