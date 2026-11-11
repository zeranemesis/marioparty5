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
M505_FLOW *lbl_1_bss_0;
M505_MODEWORK *lbl_1_bss_4;
u8 lbl_1_bss_8[0xE30];

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
    // NONMATCHING: MWCC emits subfic/neg/or instead of the original's
    // extsh/subf/cntlzw sequence for this s16==-1 test.
    return (-1 - lbl_1_bss_0->camMotionNo) != 0;
}

void fn_1_1EA0(void)
{
    CamMotionExPath(0x10, lbl_1_bss_0->camPath, lbl_1_bss_0->camPathNum);
    lbl_1_bss_0->flag2 |= 0x100;
    HuPrcEnd();
}

s32 lbl_1_data_418[10][4] = {
    { 0x1f8, 0x200, 0x1fa, 0x1f9 },
    { 0x1e4, 0x1ec, 0x1e6, 0x1e5 },
    { 0x25c, 0x264, 0x25e, 0x25d },
    { 0x2ac, 0x2b4, 0x2ae, 0x2ad },
    { 0x284, 0x28c, 0x286, 0x285 },
    { 0x1bc, 0x1c4, 0x1be, 0x1bd },
    { 0x298, 0x2a0, 0x29a, 0x299 },
    { 0x1d0, 0x1d8, 0x1d2, 0x1d1 },
    { 0x270, 0x278, 0x272, 0x271 },
    { 0x234, 0x23c, 0x236, 0x235 },
};

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

static const f32 lbl_1_data_60[][3] = {
    { 0.0f, 120.0f, 0.0f },
    { -280.0f, 120.0f, 260.0f },
    { -800.0f, 120.0f, 260.0f },
    { -280.0f, 120.0f, 50.0f },
};

void fn_1_28BC(s32 arg0, s16 arg1)
{
    // NONMATCHING: 17.57%. MWCC keeps arg0 in a callee-saved register (r31) and reorders the global copies,
    // while the original stores arg0 on the stack and keeps the table pointer in r5.
    const f32 *tbl = lbl_1_data_60[arg0];

    lbl_1_bss_0->posTo = lbl_1_bss_0->pos;
    lbl_1_bss_0->posFrom.x = tbl[0];
    lbl_1_bss_0->posFrom.y = tbl[1];
    lbl_1_bss_0->posFrom.z = tbl[2];
    lbl_1_bss_0->camMoveSpeed = arg1;
    lbl_1_bss_0->flag2 &= ~0x80;
    HuPrcChildCreate(fn_1_267C, 0x64, 0x1000, 0, HuPrcCurrentGet());
}
