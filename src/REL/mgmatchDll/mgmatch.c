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
M505_FLOW *lbl_1_bss_0;
M505_MODEWORK *lbl_1_bss_4;
u8 lbl_1_bss_8[0xE30];

u32 lbl_1_data_0[12] = {
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

void ObjectSetup(void)
{
    GwSystem.subGameNo = 0;
    _SetFlag(0xb);
    HuPrcChildCreate(fn_1_BC0, 0xc8, 0x3000, 0, HuPrcCurrentGet());
}

s16 fn_1_458(void)
{
    return lbl_1_bss_0->winId;
}

u8 fn_1_588(void)
{
    return lbl_1_bss_0->winner;
}

u8 fn_1_624(void)
{
    return lbl_1_bss_4->winner;
}

s32 fn_1_638(void)
{
    return lbl_1_bss_0->unknownE00;
}

u32 fn_1_8B4(u32 arg0)
{
    return lbl_1_bss_0->flag2 & arg0;
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

s32 fn_1_528(void)
{
    // NONMATCHING: the original keeps each array index in a separate register and
    // uses indexed addressing (lbzx); MWCC folds the +1/+0x36 into the displacement.
    return mgDataTbl[((s8 *)lbl_1_bss_4)[((s8 *)lbl_1_bss_4)[((s8 *)lbl_1_bss_4)[0] + 1] * 6 + 0x36]].nameMes;
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

void fn_1_1F58(void)
{
    CAMMOTIONWORK *tail = &lbl_1_bss_0->camPath[lbl_1_bss_0->camPathNum - 1];
    CamMotionExPath(0x10, lbl_1_bss_0->camPath, lbl_1_bss_0->camPathNum);
    Hu3DCameraPosSetV(0x10, &tail->pos, &tail->up, &tail->target);
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

void fn_1_974(void *arg0, s32 arg1)
{
    // NONMATCHING: the original mixes a single-precision load for the 24.0f factor
    // with a double-precision divide for the 500.0f divisor (lfd/fdiv); MWCC keeps
    // the whole expression in one precision.
    HuAudFXPlayPan(lbl_1_data_418[*(s16 *)((u8 *)arg0 + 0x1a)][arg1],
                   0x40 + (s32)(24.0f * (*(f32 *)((u8 *)arg0 + 0x28) / 500.0f)));
}

void fn_1_1DE8(s16 arg0, f32 arg1, s32 arg2)
{
    lbl_1_bss_0->camMotionNo = arg0;
    lbl_1_bss_0->camMoveRate = arg1;
    lbl_1_bss_0->timerType = (s16)arg2;
    HuPrcChildCreate(fn_1_1C08, 0x64, 0x1000, 0, HuPrcCurrentGet());
    HuPrcVSleep();
}

void fn_1_5D0(s32 arg0, s32 arg1)
{
    omVibrate(lbl_1_bss_0->player[arg0].padNo, (s16)arg1, 4, 2);
}

void fn_1_B18(s32 arg0)
{
    lbl_1_bss_0->keyWaitMask = arg0;
    HuPrcChildCreate(fn_1_9FC, 0xc8, 0x3000, 0, HuPrcCurrentGet());
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

void fn_1_1A24(void *arg0)
{
    HuPrcChildCreate(arg0, 0x64, 0x1000, 0, HuPrcCurrentGet());
    HuPrcVSleep();
}

u32 fn_1_90C(u32 arg0)
{
    return lbl_1_bss_4->flag & arg0;
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

void fn_1_D40(s32 arg0)
{
    OMOVLHIS *his = omOvlHisGet(0);
    omOvlHisChg(0, his->ovl, arg0, his->stat);
}

u32 fn_1_95C(u32 arg0)
{
    return lbl_1_bss_0->flag & arg0;
}

s32 fn_1_5B0(s32 arg0)
{
    return (s8)lbl_1_bss_0->player[arg0].playerNo;
}

void fn_1_8CC(u32 arg0)
{
    lbl_1_bss_4->flag |= arg0;
}

void fn_1_8EC(u32 arg0)
{
    lbl_1_bss_4->flag &= ~arg0;
}

void fn_1_87C(u32 arg0)
{
    lbl_1_bss_0->flag2 |= arg0;
}

void fn_1_898(u32 arg0)
{
    lbl_1_bss_0->flag2 &= ~arg0;
}

void fn_1_924(u32 arg0)
{
    lbl_1_bss_0->flag |= arg0;
}

void fn_1_940(u32 arg0)
{
    lbl_1_bss_0->flag &= ~arg0;
}

void fn_1_204(s32 arg0)
{
    while (arg0-- != 0) {
        HuPrcVSleep();
    }
}

s32 fn_1_510(void)
{
    return lbl_1_bss_4->maxScore;
}

u8 fn_1_59C(void)
{
    return lbl_1_bss_4->gameType;
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

void fn_1_2628(void)
{
    lbl_1_bss_0->flag2 &= ~0x40;
    HuPrcChildCreate(fn_1_2170, 0x64, 0x1000, 0, HuPrcCurrentGet());
}

void fn_1_317C(s32 arg0)
{
    (void)arg0;
    lbl_1_bss_0->flag2 |= 0x800;
    lbl_1_bss_0->flag2 &= ~0x1000;
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

void fn_1_3AD0(void)
{
    lbl_1_bss_0->flag2 &= ~0x2000;
    HuPrcChildCreate(fn_1_3850, 0x64, 0x1000, 0, HuPrcCurrentGet());
}

void fn_1_3C04(void)
{
    HuPrcChildCreate(fn_1_3B24, 0x64, 0x1000, 0, HuPrcCurrentGet());
}

void fn_1_4090(void)
{
    lbl_1_bss_0->parManMaxTime = (s16)Hu3DMotionMaxTimeGet(lbl_1_bss_0->mdl[16]);
    lbl_1_bss_0->flag2 |= 0x00090000;
    HuPrcChildCreate(fn_1_3C40, 0x64, 0x1000, 0, HuPrcCurrentGet());
}

void fn_1_64B0(void)
{
    lbl_1_bss_0->flag2 &= ~0x4000;
    HuPrcChildCreate(fn_1_4114, 0x64, 0x1500, 0, HuPrcCurrentGet());
}

void fn_1_676C(void)
{
    lbl_1_bss_0->flag2 |= 0x8000;
    HuPrcChildCreate(fn_1_6504, 0x64, 0x1500, 0, HuPrcCurrentGet());
}

void fn_1_8538(s32 arg0, f32 arg1)
{
    lbl_1_bss_0->flag2 &= ~0x200000;
    lbl_1_bss_0->camMotionType = (s16)arg0;
    lbl_1_bss_0->rotSpeed = arg1;
    HuPrcChildCreate(fn_1_7D74, 0x64, 0x1000, 0, HuPrcCurrentGet());
    HuPrcVSleep();
}

void fn_1_D130(void)
{
    lbl_1_bss_0->flag2 |= 0x00800000;
    HuPrcChildCreate(fn_1_CE5C, 0x63, 0x1000, 0, HuPrcCurrentGet());
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

static const f64 lbl_1_rodata_40 = 4503599627370496.0;
static const f32 lbl_1_rodata_2C = 10.0f;
static const f64 lbl_1_rodata_20 = 4503601774854144.0;

void fn_1_CCE4(f32 *arg0, s32 arg1)
{
    // NONMATCHING: 31.00%. MWCC swaps r30/r31 (arg0 vs arg1) and uses double fsub+frsp instead of single fsubs;
    // the original keeps arg0 in r30 and arg1 in r31 and uses single-precision conversions.
    arg0[0] = arg0[0] + (f32)(f64)(u32)frandmod(arg1 * 0x14) / lbl_1_rodata_2C - (f32)(f64)arg1;
    arg0[1] = arg0[1] + (f32)(f64)(u32)frandmod(arg1 * 0x14) / lbl_1_rodata_2C - (f32)(f64)arg1;
    arg0[2] = arg0[2] + (f32)(f64)(u32)frandmod(arg1 * 0x14) / lbl_1_rodata_2C - (f32)(f64)arg1;
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
