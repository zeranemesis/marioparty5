#include "REL/mgtourDll.h"
<<<<<<< HEAD
#include <math.h>
=======
#include "math.h"
extern void fn_1_C76C(void);

MGT_PAPER lbl_1_bss_0[MGTOUR_PAPER_MAX];
MGT_KAMIWORK *lbl_1_bss_16760;
MGT_FLOW *lbl_1_bss_16818;
MGT_MODEWORK *lbl_1_bss_1681C;
MGT_FLOW lbl_1_bss_16820;
>>>>>>> decomp/s55-mgtourDll-C

HUWINID fn_1_660(void)
{
    return lbl_1_bss_16818->winId;
}

u32 fn_1_7E4(u32 mask)
{
    return lbl_1_bss_16818->stat & mask;
}

u32 fn_1_83C(u32 mask)
{
    return lbl_1_bss_1681C->flag & mask;
}

u32 fn_1_88C(u32 mask)
{
    return lbl_1_bss_16818->flag & mask;
}

int fn_1_96C(void)
{
    return lbl_1_bss_16818->turnNo;
}

int fn_1_984(void)
{
    return lbl_1_bss_16818->charNo;
}

void fn_1_A0(s8* arr1, s8* arr2, s8 len)
{
    s8 i;
    s8 j;
    s8 temp1;
    s8 temp2;

    i = 0;
    if (len <= 1) {
        return;
    }
    while (i < len - 1) {
        j = i + 1;
        while (j < len) {
            if (arr1[i] > arr1[j]) {
                temp1 = arr1[i];
                arr1[i] = arr1[j];
                arr1[j] = temp1;
                if (arr2 != NULL) {
                    temp2 = arr2[i];
                    arr2[i] = arr2[j];
                    arr2[j] = temp2;
                }
            }
            j++;
        }
        i++;
    }
}

void fn_1_140(s8 *arr, int num)
{
    s8 usednum[256];
    int i, j;
    int no;
    for (i = num; i--;) {
        usednum[i] = arr[i];
    }
    for (i = num; i--;) {
        if (i) {
            no = frandmod(i + 1);
        } else {
            no = 0;
        }
        arr[i] = usednum[no];
        for (j = 0; j <= i; j++) {
            if (j != no && j > no) {
                usednum[j - 1] = usednum[j];
            }
        }
    }
}

void fn_1_204(void)
{
}

void fn_1_40C(void)
{
}

void fn_1_448(void)
{
}

void fn_1_49C(void)
{
}

void fn_1_4F0(void)
{
}

void fn_1_5CC(void)
{
}

void fn_1_674(void)
{
}

void fn_1_708(void)
{
}

void fn_1_750(void)
{
}

void fn_1_7AC(u32 mask)
{
    lbl_1_bss_16818->stat |= mask;
}

void fn_1_7C8(u32 mask)
{
    lbl_1_bss_16818->stat &= ~mask;
}

// NONMATCHING: 45.45%. Target keeps the global pointer in r4 across the or/clrlwi; MWCC reloads it (lis/addi/lwz) before the stb. Register allocation not controllable from C.
void fn_1_7FC(u8 mask)
{
    lbl_1_bss_1681C->flag = (u8)(lbl_1_bss_1681C->flag | mask);
}

// NONMATCHING: 45.45%. Target keeps the global pointer in r4 across the andc/clrlwi; MWCC reloads it before the stb. Register allocation not controllable from C.
void fn_1_81C(u8 mask)
{
    lbl_1_bss_1681C->flag = (u8)(lbl_1_bss_1681C->flag & ~mask);
}

void fn_1_854(u32 mask)
{
    lbl_1_bss_16818->flag |= mask;
}

void fn_1_870(u32 mask)
{
    lbl_1_bss_16818->flag &= ~mask;
}

void fn_1_8A4(void)
{
}

// NONMATCHING: 18.75%. Target reloads the global twice (r4 and r5) and uses lbzx/lhax; MWCC collapses to one register and reorders. Not controllable from C.
s16 fn_1_904(s32 arg)
{
    MGT_FLOW *f = lbl_1_bss_16818;
    s8 idx = (s8)*((u8 *)f + arg + 0x22A);
    return *(s16 *)((u8 *)f + idx * 0x30 + 0x16);
}

// NONMATCHING: 18.75%. Same register reallocation as fn_1_904 (target uses r4/r5, MWCC collapses).
s16 fn_1_938(s32 arg)
{
    MGT_FLOW *f = lbl_1_bss_16818;
    s8 idx = (s8)*((u8 *)f + arg + 0x22A);
    return *(s16 *)((u8 *)f + idx * 0x30 + 0x14);
}

// NONMATCHING: 33.33%. Target emits extsb after the lbz; MWCC omits it (char-return codegen). Not controllable from C.
s8 fn_1_99C(void)
{
    return lbl_1_bss_1681C->mgNum;
}

s8 fn_1_9B4(s32 arg)
{
    return (s8)*(s16 *)((u8 *)lbl_1_bss_16818 + arg * 0x30 + 0x14);
}

void fn_1_9D4(void)
{
}

void fn_1_A14(void)
{
}

void fn_1_A7C(void)
{
}

void fn_1_B98(void)
{
}

void fn_1_BEC(void)
{
}

void fn_1_C30(void)
{
}

void fn_1_D5C(void)
{
}

// NONMATCHING: 52.63%. Target keeps the current-process pointer in r7 (5th arg) with no frame; MWCC spills it to r31 + a frame. Register allocation not controllable from C.
void fn_1_DA4(void (*arg)(void))
{
    HUPROCESS *parent = HuPrcCurrentGet();
    HuPrcChildCreate(arg, 0x64, 0x1000, 0, parent);
    HuPrcVSleep();
}

void fn_1_DE4(s16 layerNo)
{
}

void fn_1_13D4(void)
{
}

void fn_1_1880(void)
{
}

void fn_1_18E0(void)
{
}

void fn_1_1938(void)
{
}

void fn_1_1C68(void)
{
}

void fn_1_D29C(s32);
void fn_1_C804(void);
void fn_1_D820(s32, MGT_FLOW *);
void fn_1_CFEC(s32);

void fn_1_1DE8(void)
{
    lbl_1_bss_16818->stat |= 0x80;
    lbl_1_bss_16818->stat &= ~0x100;
}

void fn_1_1E1C(void)
{
    char *path = mgDataTbl[(s8)lbl_1_bss_16818->mgTbl[lbl_1_bss_1681C->order[lbl_1_bss_1681C->cur]].mgIdx].movie[0];

    HuTHPSprCreateVol(path, 1, 0, 0.0f);
    Hu3DModelMatHookSet(lbl_1_bss_16818->mdl[5], HuTHPMatHook);
}

void fn_1_1ED4(void)
{
    s8 usednum[256];
    s8 *arr;
    s8 first;
    int num;
    int i, j;
    int no;

    first = *(s8 *)((u8 *)lbl_1_bss_1681C + lbl_1_bss_16818->mgTblNum);
    lbl_1_bss_1681C->cur = 0;
    num = lbl_1_bss_16818->mgTblNum;
    arr = (s8 *)lbl_1_bss_1681C + 1;
    for (i = num; i--;) {
        usednum[i] = arr[i];
    }
    for (i = num; i--;) {
        if (i) {
            no = frandmod(i + 1);
        } else {
            no = 0;
        }
        arr[i] = usednum[no];
        for (j = 0; j <= i; j++) {
            if (j != no && j > no) {
                usednum[j - 1] = usednum[j];
            }
        }
    }
    if (lbl_1_bss_1681C->order[0] == first) {
        lbl_1_bss_1681C->order[0] = lbl_1_bss_1681C->order[lbl_1_bss_16818->mgTblNum / 2];
        lbl_1_bss_1681C->order[lbl_1_bss_16818->mgTblNum / 2] = first;
    }
}

void fn_1_2078(void)
{
    s8 usednum[256];
    int i, j, k, no;
    OMOVLHIS *his;
    s8 *arr;
    int mdl;

    lbl_1_bss_1681C->flag = 0;
    lbl_1_bss_1681C->cur = 0;
    if (lbl_1_bss_16818->stat & 0x20) {
        lbl_1_bss_16818->stat &= ~0x20;
        HuWinExClose(lbl_1_bss_16818->winId);
        HuWinHomeClear(lbl_1_bss_16818->winId);
    }
    his = omOvlHisGet(0);
    omOvlHisChg(0, his->ovl, 0, his->stat);
    lbl_1_bss_1681C->camPos.z += 2000.0f;
    Hu3DCameraViewportSet(0x10, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(0x10, 0, 0, 640, 480);
    Hu3DCameraPosSetV(0x10, &lbl_1_bss_1681C->camPos, &lbl_1_bss_1681C->camUp, &lbl_1_bss_1681C->camTarget);
    Hu3DCameraPerspectiveSet(0x10, lbl_1_bss_1681C->aspect, 20.0f, 60000.0f, 1.2f);
    Hu3DCameraKill(2);
    fn_1_D29C(0);
    lbl_1_bss_16818->stat &= ~0x40000000;
    for (k = 4; k--;) {
        lbl_1_bss_1681C->playerNo[k] = k;
    }
    arr = lbl_1_bss_1681C->playerNo;
    for (i = 4; i--;) {
        usednum[i] = arr[i];
    }
    for (i = 4; i--;) {
        if (i) {
            no = frandmod(i + 1);
        } else {
            no = 0;
        }
        arr[i] = usednum[no];
        for (j = 0; j <= i; j++) {
            if (j != no && j > no) {
                usednum[j - 1] = usednum[j];
            }
        }
    }
    Hu3DModelLayerSet(lbl_1_bss_16818->mdl[4], 3);
    Hu3DModelAttrSet(lbl_1_bss_16818->mdl[4], 1);
    Hu3DModelCameraSet(lbl_1_bss_16818->mdl[4], 4);
    mdl = lbl_1_bss_16818->mdl[2];
    Hu3DMotionStartEndSet(mdl, 0.0f, 28.0f);
    Hu3DMotionTimeSet(mdl, 0.0f);
    Hu3DMotionShapeTimeSet(mdl, 0.0f);
    Hu3DModelAttrSet(lbl_1_bss_16818->mdl[2], 0x40000002);
    lbl_1_bss_16818->stat &= ~0x4000;
    HuTHPClose();
    HuPrcVSleep();
    while (HuTHPProcCheck() != 0) {
        HuPrcVSleep();
    }
}

void fn_1_242C(void)
{
    int mdl;
    int mdl2;

    mdl = lbl_1_bss_16818->mdl[0];
    Hu3DMotionStartEndSet(mdl, 0.0f, 179.0f);
    Hu3DMotionTimeSet(mdl, 0.0f);
    Hu3DMotionShapeTimeSet(mdl, 0.0f);
    Hu3DModelAttrReset(lbl_1_bss_16818->mdl[0], 0x40000082);
    Hu3DModelAttrReset(lbl_1_bss_16818->mdl[0], 0x40000041);
    Hu3DCameraMotionOn(lbl_1_bss_16818->mdl[0], 0x10);
    while (!Hu3DMotionEndCheck(lbl_1_bss_16818->mdl[0])) {
        HuPrcVSleep();
    }
    Hu3DCameraMotionOff(lbl_1_bss_16818->mdl[0]);
    mdl2 = lbl_1_bss_16818->mdl[0];
    Hu3DMotionStartEndSet(mdl2, 180.0f, 359.0f);
    Hu3DMotionTimeSet(mdl2, 180.0f);
    Hu3DMotionShapeTimeSet(mdl2, 180.0f);
    Hu3DModelAttrReset(lbl_1_bss_16818->mdl[0], 0x40000082);
    Hu3DModelAttrSet(lbl_1_bss_16818->mdl[0], 0x40000041);
    lbl_1_bss_16818->stat |= 0x40;
    HuPrcEnd();
}

void fn_1_25E4(void)
{
    lbl_1_bss_16818->stat &= ~0x40;
    HuPrcChildCreate(fn_1_242C, 0x64, 0x1000, 0, HuPrcCurrentGet());
    HuPrcVSleep();
}

f32 lbl_1_data_1B4[13] = { 0.0f, 0.0f, 1000.0f, 0.0f, -3000.0f, 1000.0f, 0.0f, 0.0f, -8000.0f, -3000.0f, -3000.0f, -3000.0f, -3000.0f };

void fn_1_263C(int camBit)
{
    f32 spacing = 200.0f;
    int i;
    int mdl;

    for (i = 4; i--;) {
        mdl = lbl_1_bss_16818->mdl[i + 7];
        Hu3DModelPosSet(mdl, -(1.5f * spacing) + i * spacing, 0.0f, lbl_1_data_1B4[lbl_1_bss_16818->player[i].charNo]);
        Hu3DModelAttrReset(mdl, 1);
        Hu3DModelCameraSet(mdl, 2);
        Hu3DModelLayerSet(mdl, 0);
        mdl = lbl_1_bss_16818->mdl[i + 12];
        Hu3DModelAttrReset(mdl, 1);
        Hu3DModelLayerSet(mdl, 4);
        Hu3DModelCameraSet(mdl, camBit);
    }
    Hu3DCameraCreate(2);
    Hu3DCameraPerspectiveSet(2, 1.2f, 15000.0f, 40000.0f, 1.2f);
    Hu3DCameraViewportSet(2, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(2, 0, 0, 640, 480);
    Hu3DCameraPosSet(2, 0.0f, 100.0f, 30000.0f, 0.0f, 1.0f, 0.0f, 0.0f, 100.0f, 0.0f);
}

void fn_1_28C0(void)
{
    Vec pos;
    Vec up;
    Vec target;

    lbl_1_bss_16818->stat |= 0x400;
    Hu3DCameraKill(0x10);
    pos.x = pos.y = 0.0f;
    pos.z = lbl_1_bss_1681C->camPos.z;
    up.x = up.z = 0.0f;
    up.y = 1.0f;
    target.x = target.y = target.z = 0.0f;
    Hu3DCameraCreate(2);
    Hu3DCameraViewportSet(4, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(4, 0, 0, 640, 480);
    Hu3DCameraPosSetV(4, &pos, &up, &target);
    Hu3DCameraPerspectiveSet(4, lbl_1_bss_1681C->aspect, 20.0f, 13000.0f, 1.2f);
    fn_1_263C(4);
    Hu3DModelAttrSet(lbl_1_bss_16818->mdl[5], 1);
    Hu3DModelAttrReset(lbl_1_bss_16818->mdl[6], 1);
    Hu3DModelAttrReset(lbl_1_bss_16818->mdl[4], 1);
    fn_1_C804();
    fn_1_D820(1, lbl_1_bss_16818);
    fn_1_CFEC(1);
}

void fn_1_2CE8(void)
{
    int i;
    int mdl;

    lbl_1_bss_16818->stat &= ~0x400;
    for (i = 4; i--;) {
        mdl = lbl_1_bss_16818->mdl[i + 7];
        Hu3DModelCameraSet(mdl, 0x10);
        Hu3DModelLayerSet(mdl, 4);
    }
    Hu3DCameraCreate(0x10);
    Hu3DCameraKill(6);
    Hu3DCameraViewportSet(0x10, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(0x10, 0, 0, 640, 480);
    Hu3DCameraPosSetV(0x10, &lbl_1_bss_1681C->camPos, &lbl_1_bss_1681C->camUp, &lbl_1_bss_1681C->camTarget);
    Hu3DCameraPerspectiveSet(0x10, lbl_1_bss_1681C->aspect, 20.0f, 60000.0f, 1.2f);
    fn_1_CFEC(0);
}

Vec lbl_1_data_7C[5] = {
    { 0.0f, 100.0f, 0.0f },
    { -320.0f, 100.0f, 140.0f },
    { -420.0f, 100.0f, 140.0f },
    { -380.0f, 100.0f, 140.0f },
    { 20.0f, 100.0f, 140.0f },
};
u8 lbl_1_data_120[16] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xA0, 0xA0, 0xFF, 0x00, 0x80, 0xC0, 0xFF, 0x00 };
HU3DPARMANPARAM lbl_1_data_130 = {
    0x3C, 0, 1.0f, 10.0f, 5.0f, { 0.0f, -0.4f, 0.0f }, 15.0f, 1.0f, 70.0f, 0.95f, 2,
    { { 0xFF, 0xFF, 0xFF, 0xFF }, { 0xFF, 0xFF, 0xFF, 0xFF }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
    { { 0xFF, 0xFF, 0xFF, 0 }, { 0xFF, 0xFF, 0xFF, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
};

static inline BOOL MgtAngleSub(f32 *angle, f32 end, f32 speed)
{
    f32 diff = *angle - end;
    f32 change = fmodf(diff, 180.0f);
    f32 wrap = fmodf(diff, 360.0f);

    if (fabsf(wrap) > 180.0f) {
        if (change > 0.0f) {
            change = -(180.0f - fabsf(change));
        } else {
            change = 180.0f - fabsf(change);
        }
    }
    if (fabsf(change) < speed) {
        *angle = end;
        return TRUE;
    }
    if (change > 0.0f) {
        change -= speed;
    } else {
        change += speed;
    }
    *angle = end + change;
    return FALSE;
}

// NONMATCHING: float-literal pool order (sin arg) and callee-saved reg numbering differ; logic complete.
void fn_1_2E68(void)
{
    Vec pos;
    Vec dir;
    Vec vel;
    Vec p;
    MGT_PLAYER *pl;
    s32 timer;
    s32 state;
    s32 i;
    s32 j;
    int id2;
    int id1;
    s32 running;
    s16 id3;
    s32 wait1;
    s32 wait2;
    s32 wait3;
    ANIMDATA *anim;
    f32 vy;
    f32 angle;
    f32 t;
    f32 t2;
    f32 ts;
    s16 id;

#ifdef TARGET_PC
    timer = 0;
    vy = 0.0f;
    anim = NULL;
#endif
    angle = 0.0f;
    state = 0;
    running = 1;
    pl = &lbl_1_bss_16818->player[(s8)*((s8 *)lbl_1_bss_16818 + ((s8)lbl_1_bss_16818->turnNo == 0) + 0x22A)];
    while (running) {
        switch (state) {
        case 0:
            if (pl->state != 5) {
                CharMotionShiftSet(pl->charNo, pl->motId[5], 0.0f, 5.0f, 0x40000001);
                pl->state = 5;
            }
            for (wait1 = 0x78; wait1--;) {
                HuPrcVSleep();
            }
            Hu3DModelAttrReset(lbl_1_bss_16818->mdl[3], 0x40000002);
            timer = 0x19;
            angle = 0.0f;
            state = 0x3E8;
            break;
        case 0x3E8:
            ts = 1.0f - ((f32)timer / 25.0f);
            ts = sin(3.141592653589793 * ts * 0.5);
            ts = sin(3.141592653589793 * ts * 0.5);
            MgtAngleSub(&angle, 45.0f, 3.0f);
            PSVECSubtract(&lbl_1_data_7C[3], &lbl_1_data_7C[2], &dir);
            PSVECScale(&dir, &dir, ts);
            PSVECAdd(&lbl_1_data_7C[2], &dir, &pos);
            Hu3DModelPosSetV(lbl_1_bss_16818->mdlShake, &pos);
            Hu3DModelRotSet(lbl_1_bss_16818->mdlShake, 0.0f, angle, 0.0f);
            if (--timer == 0) {
                state = 1;
            }
            break;
        case 1:
            if ((s8)lbl_1_bss_16818->turnNo != 0) {
                HuAudFXPlayPan(0x4A8, 0x30);
                omVibrate(GwPlayerConf[(s8)lbl_1_bss_16818->player1No].padNo, 0xF, 4, 2);
                id1 = lbl_1_bss_16818->mdl[2];
                Hu3DMotionStartEndSet(id1, 0.0f, 28.0f);
                Hu3DMotionTimeSet(id1, 0.0f);
                Hu3DMotionShapeTimeSet(id1, 0.0f);
            } else {
                HuAudFXPlayPan(0x4A8, 0x50);
                omVibrate(GwPlayerConf[(s8)lbl_1_bss_16818->player2No].padNo, 0xF, 4, 2);
                id2 = lbl_1_bss_16818->mdl[2];
                Hu3DMotionStartEndSet(id2, 29.0f, 58.0f);
                Hu3DMotionTimeSet(id2, 29.0f);
                Hu3DMotionShapeTimeSet(id2, 29.0f);
            }
            Hu3DModelAttrReset(lbl_1_bss_16818->mdl[2], 0x40000002);
            if (pl->state != 7) {
                CharMotionShiftSet(pl->charNo, pl->unk10, 0.0f, 5.0f, 0);
                pl->state = 7;
            }
            for (wait2 = 0x19; wait2--;) {
                HuPrcVSleep();
            }
            state = 2;
            break;
        case 2:
            if (pl->state != 4) {
                CharMotionShiftSet(pl->charNo, pl->motId[4], 0.0f, 5.0f, 0x40000001);
                pl->state = 4;
            }
            Hu3DMotionAttrSet(pl->motId[4], 1);
            state = 3;
            break;
        case 3:
            for (wait3 = 0x1E; wait3--;) {
                HuPrcVSleep();
            }
            timer = 0x1E;
            vy = 750.0f;
            state = 4;
            break;
        case 4:
            vy += 150.0f;
            pl->pos.y -= vy / 100.0f;
            Hu3DModelPosSet(pl->modelId, pl->pos.x, pl->pos.y, pl->pos.z);
            if (--timer == 0) {
                anim = HuSprAnimRead(HuDataSelHeapReadNum(0x9A0022, 0x10000000, HEAP_MODEL));
                lbl_1_data_130.colorStart[0].r = lbl_1_data_120[0];
                lbl_1_data_130.colorStart[0].g = lbl_1_data_120[1];
                lbl_1_data_130.colorStart[0].b = lbl_1_data_120[2];
                lbl_1_data_130.colorStart[0].a = lbl_1_data_120[3];
                lbl_1_data_130.colorStart[1].r = lbl_1_data_120[4];
                lbl_1_data_130.colorStart[1].g = lbl_1_data_120[5];
                lbl_1_data_130.colorStart[1].b = lbl_1_data_120[6];
                lbl_1_data_130.colorStart[1].a = lbl_1_data_120[7];
                lbl_1_data_130.colorEnd[0].r = lbl_1_data_120[8];
                lbl_1_data_130.colorEnd[0].g = lbl_1_data_120[9];
                lbl_1_data_130.colorEnd[0].b = lbl_1_data_120[10];
                lbl_1_data_130.colorEnd[0].a = lbl_1_data_120[11];
                lbl_1_data_130.colorEnd[1].r = lbl_1_data_120[12];
                lbl_1_data_130.colorEnd[1].g = lbl_1_data_120[13];
                lbl_1_data_130.colorEnd[1].b = lbl_1_data_120[14];
                lbl_1_data_130.colorEnd[1].a = lbl_1_data_120[15];
                t = 0.0f;
                for (i = 0x18; i--;) {
                    t += 15.0f;
                    vel.x = cos((3.141592653589793 * t) / 180.0);
                    vel.y = 10.0f;
                    vel.z = sin((3.141592653589793 * t) / 180.0);
                    p.x = pl->pos.x;
                    p.y = pl->pos.y;
                    p.z = pl->pos.z;
                    p.x += 30.0f * vel.x;
                    p.z += 30.0f * vel.z;
                    p.y = -150.0f;
                    PSVECNormalize(&vel, &vel);
                    id3 = Hu3DParManCreate(anim, 0x1E, &lbl_1_data_130);
                    lbl_1_bss_16818->parManId[i] = id3;
                    Hu3DParManAttrSet(id3, 0x864);
                    Hu3DParManLayerSet(id3, 4);
                    Hu3DParManPosSet(id3, p.x, p.y, p.z);
                    Hu3DParManVecSet(id3, vel.x, vel.y, vel.z);
                }
                if ((s8)lbl_1_bss_16818->turnNo != 0) {
                    HuAudFXPlayPan(0x4A9, 0x30);
                    omVibrate(GwPlayerConf[(s8)lbl_1_bss_16818->player1No].padNo, 0x14, 4, 2);
                } else {
                    HuAudFXPlayPan(0x4A9, 0x50);
                    omVibrate(GwPlayerConf[(s8)lbl_1_bss_16818->player2No].padNo, 0x14, 4, 2);
                }
                timer = 0xA;
                state = 5;
            }
            break;
        case 5:
            if (--timer == 0) {
                state = 6;
            }
            t2 = 0.0f;
            for (j = 0x18; j--;) {
                t2 += 15.0f;
                vel.x = cos((3.141592653589793 * t2) / 180.0);
                vel.y = (f32)timer;
                vel.z = sin((3.141592653589793 * t2) / 180.0);
                p.x = pl->pos.x;
                p.y = pl->pos.y;
                p.z = pl->pos.z;
                p.x += 30.0f * vel.x * (1.0f + ((10.0f / (f32)timer) * 0.25f));
                p.z += 30.0f * vel.z * (1.0f + ((10.0f / (f32)timer) * 0.25f));
                p.y = -150.0f;
                PSVECNormalize(&vel, &vel);
                id = lbl_1_bss_16818->parManId[j];
                Hu3DParManPosSet(id, p.x, p.y, p.z);
                Hu3DParManVecSet(id, vel.x, vel.y, vel.z);
            }
            break;
        case 6:
            running = 0;
            break;
        }
        HuPrcVSleep();
    }
    lbl_1_bss_16818->stat |= 0x800;
    HuPrcEnd();
}

void fn_1_3B98(void)
{
    lbl_1_bss_16818->stat &= ~0x800;
    HuPrcChildCreate(fn_1_2E68, 0x64, 0x1000, 0, HuPrcCurrentGet());
    HuPrcVSleep();
}

void fn_1_3BF0(void)
{
    Vec target;
    f32 s;
    f32 angle = 0.0f;

    while (lbl_1_bss_16818->stat & 0x4000) {
        s = sin(3.141592653589793 * angle / 180.0);
        target = lbl_1_bss_16818->camUp;
        target.y += 5.0f * s;
        Hu3DCameraPosSetV(0x10, &lbl_1_bss_16818->camPos, &lbl_1_bss_1681C->camUp, &target);
        angle += 2.3f;
        if (angle >= 360.0f) {
            angle -= 360.0f;
        }
        HuPrcVSleep();
    }
    HuPrcEnd();
}

void fn_1_3D2C(void)
{
    lbl_1_bss_16818->camPos = lbl_1_bss_1681C->camPos;
    lbl_1_bss_16818->camUp = lbl_1_bss_1681C->camTarget;
    lbl_1_bss_16818->stat |= 0x4000;
    HuPrcChildCreate(fn_1_3BF0, 0x64, 0x1000, 0, HuPrcCurrentGet());
    HuPrcVSleep();
}

static inline BOOL MgtAngleSub(float *angle, float end, float speed)
{
    float diff = *angle - end;
    float change = fmodf(diff, 180);
    float wrap = fmodf(diff, 360);
    if (fabsf(wrap) > 180.0f) {
        if (change > 0) {
            change = -(180.0f - fabsf(change));
        } else {
            change = 180.0f - fabsf(change);
        }
    }
    if (fabsf(change) < speed) {
        *angle = end;
        return TRUE;
    }
    if (change > 0) {
        change -= speed;
    } else {
        change += speed;
    }
    *angle = end + change;
    return FALSE;
}

void fn_1_3DE4(void)
{
    Vec dir;
    Vec aimPos;
    MGT_PLAYER *player;
    int i;
    int doneNum;
    float rotEnd;
    for (player = &lbl_1_bss_16818->player[0], i = 0; i < MGTOUR_PLAYER_MAX; i++, player++) {
        switch (lbl_1_bss_16818->mode) {
            case 0:
                player->rotX = 0;
                break;
            case 1:
                aimPos = lbl_1_bss_16818->shakePos;
                aimPos.z += 350;
                VECSubtract(&aimPos, &player->pos, &aimPos);
                aimPos.y = 0;
                VECNormalize(&aimPos, &dir);
                if (dir.z > 1) {
                    dir.z = 1;
                } else if (dir.z < -1) {
                    dir.z = -1;
                }
                rotEnd = acosf(dir.z);
                if (dir.x < 0) {
                    rotEnd = -rotEnd;
                }
                player->rotX = (180 * rotEnd) / M_PI;
                break;
        }
        if (!MgtAngleSub(&player->rotY, player->rotX, 0.01f)) {
            if (player->state != 1) {
                CharMotionShiftSet(player->charNo, player->motId[1], 0, 5, HU3D_MOTATTR_LOOP);
                player->state = 1;
            }
        }
    }
    while (1) {
        doneNum = 0;
        for (player = &lbl_1_bss_16818->player[0], i = MGTOUR_PLAYER_MAX; i--; player++) {
            if (MgtAngleSub(&player->rotY, player->rotX, lbl_1_bss_16818->rotSpeed)) {
                if (player->state != 0) {
                    CharMotionShiftSet(player->charNo, player->motId[0], 0, 5, HU3D_MOTATTR_LOOP);
                    player->state = 0;
                }
                doneNum++;
            }
            Hu3DModelRotSet(player->modelId, 0, player->rotY, 0);
        }
        if (doneNum == 4) {
            break;
        }
        HuPrcVSleep();
    }
    lbl_1_bss_16818->stat |= 0x1000;
    HuPrcEnd();
}

void fn_1_44E0(int mode, float speed)
{
    lbl_1_bss_16818->stat &= ~0x1000;
    lbl_1_bss_16818->mode = mode;
    lbl_1_bss_16818->rotSpeed = speed;
    HuPrcChildCreate(fn_1_3DE4, 0x64, 0x1000, 0, HuPrcCurrentGet());
    HuPrcVSleep();
}

Vec lbl_1_data_28[12] = {
    { -160.0f, 0.0f, 160.0f },
    { 0.0f, 0.0f, 160.0f },
    { 160.0f, 0.0f, 160.0f },
    { 320.0f, 0.0f, 160.0f },
    { -180.0f, 31.0f, 420.0f },
    { 167.0f, 31.0f, 420.0f },
    { 0.0f, -300.0f, -7000.0f },
    { 0.0f, 100.0f, 0.0f },
    { -320.0f, 100.0f, 140.0f },
    { -420.0f, 100.0f, 140.0f },
    { -380.0f, 100.0f, 140.0f },
    { 20.0f, 100.0f, 140.0f }
};

struct MgtParCol {
    u8 c[4][4];
};

struct MgtParCol lbl_1_data_1E8 = {
    { { 0x80, 0x80, 0x80, 0xFF }, { 0x40, 0x40, 0x40, 0xFF }, { 0x00, 0x00, 0x00, 0x00 }, { 0x40, 0x40, 0x40, 0x80 } }
};

HU3DPARMANPARAM lbl_1_data_1F8 = {
    40, 0, 0.3f, 30.0f, 10.0f, { 0.0f, -0.2f, 0.0f }, 2.0f, 1.0f, 40.0f, 0.98f, 2,
    { { 0xFF, 0xFF, 0xFF, 0xFF }, { 0xFF, 0xFF, 0xFF, 0xFF }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
    { { 0xFF, 0xFF, 0xFF, 0x00 }, { 0xFF, 0xFF, 0xFF, 0x00 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
};

static inline void MgtParColSet(HU3DPARMANPARAM *dst, struct MgtParCol *src)
{
    dst->colorStart[0].r = src->c[0][0];
    dst->colorStart[0].g = src->c[0][1];
    dst->colorStart[0].b = src->c[0][2];
    dst->colorStart[0].a = src->c[0][3];
    dst->colorStart[1].r = src->c[1][0];
    dst->colorStart[1].g = src->c[1][1];
    dst->colorStart[1].b = src->c[1][2];
    dst->colorStart[1].a = src->c[1][3];
    dst->colorEnd[0].r = src->c[2][0];
    dst->colorEnd[0].g = src->c[2][1];
    dst->colorEnd[0].b = src->c[2][2];
    dst->colorEnd[0].a = src->c[2][3];
    dst->colorEnd[1].r = src->c[3][0];
    dst->colorEnd[1].g = src->c[3][1];
    dst->colorEnd[1].b = src->c[3][2];
    dst->colorEnd[1].a = src->c[3][3];
}

void fn_1_456C(void)
{
    int angle;
    HU3DPARMANID parManId;
    HU3DMODELID mdlId;
    ANIMDATA *anim;
    s8 charNo;
    Vec basePos;
    Vec pos;
    float s;
    mdlId = lbl_1_bss_16818->mdl[4];
    angle = 0;
    basePos = lbl_1_data_28[lbl_1_bss_16818->turnNo + 4];
    charNo = lbl_1_bss_16818->charNo;
    basePos.y += 25.0f + (40.0f + CharModelHeightGet(charNo));
    anim = HuSprAnimRead(HuDataSelHeapReadNum(0x9A0023, 0x10000000, HEAP_MODEL));
    MgtParColSet(&lbl_1_data_1F8, &lbl_1_data_1E8);
    parManId = Hu3DParManCreate(anim, 100, &lbl_1_data_1F8);
    Hu3DParManAttrSet(parManId, 0x54);
    Hu3DParManLayerSet(parManId, 4);
    Hu3DParticleBlendModeSet(Hu3DParManModelIDGet(parManId), 1);
    while (lbl_1_bss_16818->stat & 0x2000) {
        s = sin(M_PI * angle / 180);
        pos = basePos;
        pos.y += 25.0f * s;
        Hu3DModelPosSetV(mdlId, &pos);
        Hu3DParManPosSet(parManId, pos.x, pos.y, pos.z);
        angle = (angle + 1) % 360;
        HuPrcVSleep();
    }
    Hu3DParManAttrSet(parManId, 1);
    HuPrcEnd();
}

void fn_1_485C(void)
{
    lbl_1_bss_16818->stat |= 0x2000;
    HuPrcChildCreate(fn_1_456C, 0x64, 0x1000, 0, HuPrcCurrentGet());
    HuPrcVSleep();
}

MGT_MGENTRY lbl_1_data_B8[MGTOUR_MGTBL_MAX] = {
    { 0x37, 0x21D, 0 }, { 0x38, 0x21E, 0 }, { 0x39, 0x21F, 0 }, { 0x3A, 0x220, 0 }, { 0x3B, 0x221, 0 },
    { 0x3C, 0x222, 0 }, { 0x3D, 0x223, 0 }, { 0x3E, 0x224, 0 }, { 0x41, 0x227, 0 }, { 0x42, 0x228, 0 },
    { 0x3F, 0x225, 0 }, { 0x40, 0x226, 0 }, { 0x43, 0x229, 0 }, { 0x44, 0x22A, 0 }, { 0x45, 0x22B, 0 },
};

unsigned int lbl_1_data_0[10] = {
    0x008C0000, 0x008C0001, 0x008C0032, 0x008C0031, 0x008C0052, 0x008C0036, 0x00870099, 0x008C001A, 0x008C002E, 0
};

Vec lbl_1_data_114 = { 0, -100, -2000 };

extern void fn_1_C434(HU3DMODELID *mdl, MGT_FLOW *flow);

// remaining diff: stack slot order of shakePos/Vec locals and 2 extra callee-saved regs
void fn_1_48B4(void)
{
    OMOVLHIS *his;
    int i;
    MGT_PLAYER *player;
    s8 mgNo;
    s8 k;
    HU3DLIGHTID lightId;
    Vec shakePos;
    HuVec2f winSize;
    HU3DMODELID shakeMdlId;
    ANIMDATA *anim;

    his = omOvlHisGet(0);
    lbl_1_bss_1681C = (MGT_MODEWORK *)mgModeWork;
    lbl_1_bss_16818 = &lbl_1_bss_16820;
    {
        Vec shadowPos = { 0, 8000, 0 };
        Vec shadowUp = { 0, 0, 1 };
        Vec shadowTarget = { 0, 0, 0 };
        Vec altDown = { 0, -1, 0 };
        Vec tempPos = { 0, 50, 4000 };
        Vec altBack = { 0, 0, -1 };
        lbl_1_bss_16818->objman = omInitObjMan(200, 1000);
        HuDataSelHeapReadNum(DATA_instpic, 0x10000000, HEAP_MODEL);
        Hu3DCameraCreate(HU3D_CAM4);
        Hu3DCameraPerspectiveSet(HU3D_CAM4, 60, 20, 30000, 1.2f);
        Hu3DCameraViewportSet(HU3D_CAM4, 0, 0, 640, 480, 0, 1);
        Hu3DCameraScissorSet(HU3D_CAM4, 0, 0, 640, 480);
        lightId = Hu3DGLightCreate(0, 1000, 1000, 0, -1, -1, 255, 255, 255);
        Hu3DGLightStaticSet(lightId, 1);
        Hu3DGLightPosSetV(lightId, &shadowPos, &altDown);
        lightId = Hu3DGLightCreate(0, 1000, 1000, 0, -1, -1, 128, 128, 128);
        Hu3DGLightStaticSet(lightId, 1);
        Hu3DGLightPosSetV(lightId, &tempPos, &altBack);
    }
    lbl_1_bss_16818->mgTblNum = 0;
    for (i = 0; i < MGTOUR_MGTBL_MAX; i++) {
        for (mgNo = 0; mgDataTbl[mgNo].ovl != lbl_1_data_B8[i].mes1; mgNo++) {
            if (mgDataTbl[mgNo].ovl == (u16)DLL_NONE) {
                OSReport("mgtournament.c : ＤＬＬが見つかりません！！\n");
                abort();
            }
        }
        lbl_1_data_B8[i].mgIdx = mgNo;
        if (GWMgUnlockGet(mgNo + 0x1F5)) {
            MGT_MGENTRY *src = &lbl_1_data_B8[i];
            MGT_MGENTRY *dst = &lbl_1_bss_16818->mgTbl[lbl_1_bss_16818->mgTblNum];
            dst->mes1 = src->mes1;
            dst->mes2 = src->mes2;
            dst->mgIdx = src->mgIdx;
            lbl_1_bss_16818->mgTblNum++;
        }
    }
    Hu3DParManInit();
    lbl_1_bss_16818->winId2 = -1;
    lbl_1_bss_16818->winId = 0;
    lbl_1_bss_16818->flag = 0;
    lbl_1_bss_16818->stat = 0;
    lbl_1_bss_16818->turnNo = -1;
    lbl_1_bss_16818->charNo = -1;
    lbl_1_bss_16818->player1No = -1;
    lbl_1_bss_16818->player2No = -1;
    lbl_1_bss_16818->bgmId = -1;
    lbl_1_bss_16818->animId = -1;
    lbl_1_bss_16818->seId = -1;
    lbl_1_bss_16818->mode = 0;
    lbl_1_bss_16818->rotSpeed = 2;
    HuWinInit(1);
    HuWinMesMaxSizeBetGet(&winSize, 0x120000, 0x12000E);
    lbl_1_bss_16818->winId = HuWinExCreateFrame(-10000, 360, winSize.x, winSize.y, -1, 1);
    HuWinMesPalSet(lbl_1_bss_16818->winId, 7, 0, 0, 0);
    fn_1_13D4();
    if ((lbl_1_bss_16818->stat & 0x200) == 0) {
        Hu3DModelAttrSet(lbl_1_bss_16818->mdl[5], 1);
        Hu3DModelScaleSet(lbl_1_bss_16818->mdl[5], 3.8f, 3.8f, 1);
        Hu3DModelPosSet(lbl_1_bss_16818->mdl[5], lbl_1_data_114.x, lbl_1_data_114.y, lbl_1_data_114.z);
        Hu3DModelAttrSet(lbl_1_bss_16818->mdl[6], 1);
        Hu3DModelScaleSet(lbl_1_bss_16818->mdl[6], 3.8f, 3.8f, 1);
        Hu3DModelPosSet(lbl_1_bss_16818->mdl[6], lbl_1_data_114.x, lbl_1_data_114.y, lbl_1_data_114.z);
        lbl_1_bss_16818->stat |= 0x200;
        HuPrcChildCreate(fn_1_1938, 100, 4096, 0, HuPrcCurrentGet());
    }
    fn_1_C434(lbl_1_bss_16818->mdl, lbl_1_bss_16818);
    lbl_1_bss_16818->texId = HuSprAnimRead(HuDataSelHeapReadNum(DATA_mgtour | 0xF, 0x10000000, HEAP_MODEL));
    Hu3DLayerHookSet(0, fn_1_DE4);
    lbl_1_bss_16818->scrollX = 0;
    lbl_1_bss_16818->scrollY = 0;
    if (his->evtno == 0) {
        _SetFlag(FLAG_INST_NOHISCHG);
        _SetFlag(FLAG_INST_MGMODE);
        for (k = 4; k--;) {
            lbl_1_bss_1681C->playerNo[k] = k;
        }
        fn_1_140(lbl_1_bss_1681C->playerNo, 4);
    } else {
        Hu3DCameraPosSetV(HU3D_CAM4, &lbl_1_bss_1681C->camPos, &lbl_1_bss_1681C->camUp, &lbl_1_bss_1681C->camTarget);
        Hu3DCameraPerspectiveSet(HU3D_CAM4, lbl_1_bss_1681C->aspect, 20, 30000, 1.2f);
    }
    for (player = &lbl_1_bss_16818->player[0], i = 0; i < MGTOUR_PLAYER_MAX; i++, player++) {
        player->playerNo = i;
        player->charNo = GwPlayerConf[player->playerNo].charNo;
        player->charNo2 = GwPlayerConf[player->playerNo].padNo;
        player->modelId = CharModelMotListCreate(player->charNo, 2, lbl_1_data_0, player->motId);
        player->state = -1;
        player->pos = lbl_1_data_28[lbl_1_bss_1681C->playerNo[i]];
        player->rotY = 0;
        player->rotX = 0;
        CharMotionVoiceOnSet(player->charNo, lbl_1_data_0[2], FALSE);
        CharMotionVoiceOnSet(player->charNo, lbl_1_data_0[3], FALSE);
        CharMotionVoiceOnSet(player->charNo, lbl_1_data_0[4], FALSE);
        if (player->state != 0) {
            CharMotionShiftSet(player->charNo, player->motId[0], 0, 5, HU3D_MOTATTR_LOOP);
            player->state = 0;
        }
        Hu3DModelAttrSet(player->modelId, 1);
        Hu3DModelPosSet(player->modelId, player->pos.x, player->pos.y, player->pos.z);
        Hu3DModelLayerSet(player->modelId, 4);
        Hu3DModelCameraSet(player->modelId, HU3D_CAM4);
        lbl_1_bss_16818->mdl[i + 7] = player->modelId;
    }
    shakePos = lbl_1_data_28[7];
    shakeMdlId = Hu3DModelCreate(HuDataSelHeapReadNum(DATA_mgtour, 0x10000000, HEAP_MODEL));
    lbl_1_bss_16818->mdlShake = shakeMdlId;
    lbl_1_bss_16818->shakePos = shakePos;
    Hu3DMotionSet(shakeMdlId, Hu3DJointMotion(shakeMdlId, HuDataSelHeapReadNum(DATA_mgtour | 1, 0x10000000, HEAP_MODEL)));
    Hu3DModelAttrSet(shakeMdlId, HU3D_MOTATTR_LOOP);
    Hu3DModelLayerSet(shakeMdlId, 5);
    Hu3DModelCameraSet(shakeMdlId, HU3D_CAM4);
    Hu3DModelPosSet(shakeMdlId, shakePos.x, shakePos.y, shakePos.z);
    for (i = lbl_1_bss_16818->mgTblNum; i--;) {
        anim = HuSprAnimRead(HuDataSelHeapReadNum(mgDataTbl[lbl_1_bss_16818->mgTbl[i].mgIdx].instPic[0], 0x10000000, HEAP_MODEL));
        HuSprAnimLock(anim);
        lbl_1_bss_16818->thpAnim[i] = anim;
    }
    for (i = MGTOUR_MDL_MAX; i--;) {
        Hu3DCameraMotionOff(lbl_1_bss_16818->mdl[i]);
    }
    lbl_1_bss_16818->seId = HuAudFXPlay(0x39F);
}

void fn_1_5540(void)
{
    OMOVLHIS *his;
    if (lbl_1_bss_16818->bgmId >= 0) {
        HuAudSStreamFadeOut(lbl_1_bss_16818->bgmId, 1000);
        lbl_1_bss_16818->bgmId = -1;
    }
    HuDataDirClose(DATA_mgtour);
    HuDataDirClose(DATA_instpic);
    CharModelDataClose(-1);
    fn_1_C76C();
    HuTHPClose();
    GWMgInstFSet(TRUE);
    his = omOvlHisGet(0);
    omOvlHisChg(0, his->ovl, 0, his->stat);
    HuWinExClose(lbl_1_bss_16818->winId);
    HuWinExKill(lbl_1_bss_16818->winId);
    if (lbl_1_bss_16818->seId >= 0) {
        HuAudFXStop(lbl_1_bss_16818->seId);
        lbl_1_bss_16818->seId = -1;
    }
    WipeCreate(WIPE_MODE_OUT, 6, 60);
    while (WipeCheckEnd()) {
        HuPrcVSleep();
    }
    _ClearFlag(FLAG_INST_NOHISCHG);
    _ClearFlag(FLAG_INST_MGMODE);
    memset(lbl_1_bss_1681C, 0, sizeof(MGT_MODEWORK));
}

float lbl_1_data_1B4[13] = {
    0.0f, 0.0f, 1000.0f, 0.0f, -3000.0f, 1000.0f, 0.0f, 0.0f, -8000.0f, -3000.0f, -3000.0f, -3000.0f, -3000.0f
};

extern void fn_1_C7BC(s32 *playerNo, s32 *charNo);
extern void fn_1_C804(void);
extern void fn_1_D820(s32 arg, MGT_FLOW *flow);
extern void fn_1_CFEC(s32 arg);
extern void fn_1_D61C(s32 val);
extern s32 fn_1_D630(void);
extern void fn_1_FBD0(void);
extern void fn_1_FC34(void);

// remaining diff: register allocation (original keeps movie/seId in stack slots, saves r17-r31) and f9 arg of Hu3DCameraPosSet
void fn_1_56C0(void)
{
    OMOVLHIS *his;
    MGT_PLAYER *player;
    s8 *order;
    s32 playerNoList[4];
    s32 charNoList[4];
    s8 last;
    int num;
    int i;
    int j;
    int no;
    s8 usednum[256];
    float interval;
    float fov;
    float nearZ;
    float farZ;
    char *movie;
    HU3DMODELID mdlId;
    HU3DMODELID mdlId2;
    int seId;
    Vec camPos;
    Vec camUp;
    Vec camTarget;

    his = omOvlHisGet(0);
    omOvlHisChg(0, his->ovl, 1, his->stat);
    fn_1_7C8(0);
    lbl_1_bss_16818->flag = 0;
    lbl_1_bss_16818->stat |= 1;
#ifdef TARGET_PC
    /* order[] only holds 11 entries; the original overruns it into the following fields */
    num = lbl_1_bss_16818->mgTblNum;
    if (num > (int)sizeof(lbl_1_bss_1681C->order)) {
        num = sizeof(lbl_1_bss_1681C->order);
    }
    for (i = num; i--;) {
        lbl_1_bss_1681C->order[i] = i;
    }
    lbl_1_bss_1681C->cur = num;
    last = *(s8 *)(num + (u8 *)lbl_1_bss_1681C);
    lbl_1_bss_1681C->cur = 0;
#else
    for (i = lbl_1_bss_16818->mgTblNum; i--;) {
        lbl_1_bss_1681C->order[i] = i;
    }
    lbl_1_bss_1681C->cur = lbl_1_bss_16818->mgTblNum;
    last = *(s8 *)(lbl_1_bss_16818->mgTblNum + (u8 *)lbl_1_bss_1681C);
    lbl_1_bss_1681C->cur = 0;
    num = lbl_1_bss_16818->mgTblNum;
#endif
    order = lbl_1_bss_1681C->order;
    for (i = num; i--;) {
        usednum[i] = order[i];
    }
    for (i = num; i--;) {
        if (i) {
            no = frandmod(i + 1);
        } else {
            no = 0;
        }
        order[i] = usednum[no];
        for (j = 0; j <= i; j++) {
            if (j != no && j > no) {
                usednum[j - 1] = usednum[j];
            }
        }
    }
    if (lbl_1_bss_1681C->order[0] == last) {
        lbl_1_bss_1681C->order[0] = lbl_1_bss_1681C->order[lbl_1_bss_16818->mgTblNum / 2];
        lbl_1_bss_1681C->order[lbl_1_bss_16818->mgTblNum / 2] = last;
    }
    movie = mgDataTbl[lbl_1_bss_16818->mgTbl[lbl_1_bss_1681C->order[lbl_1_bss_1681C->cur]].mgIdx].movie[0];
    HuTHPSprCreateVol(movie, 1, 0, 0);
    Hu3DModelMatHookSet(lbl_1_bss_16818->mdl[5], HuTHPMatHook);
    for (player = &lbl_1_bss_16818->player[0], i = 0; i < MGTOUR_PLAYER_MAX; i++, player++) {
        player->pos = lbl_1_data_28[i];
        if (player->state != 0) {
            CharMotionShiftSet(player->charNo, player->motId[0], 0, 5, HU3D_MOTATTR_LOOP);
            player->state = 0;
        }
        Hu3DModelAttrReset(player->modelId, 1);
        Hu3DModelPosSet(player->modelId, player->pos.x, player->pos.y, player->pos.z);
        Hu3DModelLayerSet(player->modelId, 4);
        Hu3DModelCameraSet(player->modelId, HU3D_CAM4);
    }
    lbl_1_bss_1681C->mgNum = 0;
    for (i = 4; i--;) {
        playerNoList[i] = lbl_1_bss_1681C->playerNo[i];
        charNoList[i] = lbl_1_bss_16818->player[i].charNo;
    }
    fn_1_C7BC(playerNoList, charNoList);
    for (i = 0; i < MGTOUR_PLAYER_MAX; i++) {
        for (j = 0; j < 10; j++) {
            GwMgDecaScore[i].mgScore[j] = 0;
        }
        GwMgDecaScore[i].charNo = lbl_1_bss_16818->player[i].charNo;
        GwMgDecaScore[i].finalScore = 0;
    }
    lbl_1_bss_16818->shakePos = lbl_1_data_28[8];
    Hu3DModelAttrReset(lbl_1_bss_16818->mdlShake, 1);
    Hu3DModelPosSetV(lbl_1_bss_16818->mdlShake, &lbl_1_bss_16818->shakePos);
    Hu3DModelRotSet(lbl_1_bss_16818->mdlShake, 0, 30, 0);
    if (lbl_1_bss_16818->seId <= 0) {
        lbl_1_bss_16818->seId = HuAudFXPlay(0x39F);
    }
    lbl_1_bss_16818->bgmId = HuAudBGMPlay(0x23);
    lbl_1_bss_16818->stat &= ~0x40;
    HuPrcChildCreate(fn_1_242C, 100, 4096, 0, HuPrcCurrentGet());
    HuPrcVSleep();
    WipeCreate(WIPE_MODE_IN, 5, 60);
    while (WipeCheckEnd()) {
        HuPrcVSleep();
    }
    for (i = 45; i--;) {
        HuPrcVSleep();
    }
    for (i = 4; i--;) {
        omVibrate(GwPlayerConf[i].padNo, 15, 4, 2);
    }
    HuAudFXPlayPan(0x4A3, 0x28);
    HuAudFXPlayPan(0x4A3, 0x58);
    while (!(lbl_1_bss_16818->stat & 0x40)) {
        HuPrcVSleep();
    }
    Hu3DCameraPosGet(HU3D_CAM4, &lbl_1_bss_1681C->camPos, &lbl_1_bss_1681C->camUp, &lbl_1_bss_1681C->camTarget);
    Hu3DCameraPerspectiveGet(HU3D_CAM4, &fov, &nearZ, &farZ);
    lbl_1_bss_1681C->aspect = fov;
    lbl_1_bss_16818->camPos = lbl_1_bss_1681C->camPos;
    lbl_1_bss_16818->camUp = lbl_1_bss_1681C->camTarget;
    lbl_1_bss_16818->stat |= 0x4000;
    HuPrcChildCreate(fn_1_3BF0, 100, 4096, 0, HuPrcCurrentGet());
    HuPrcVSleep();
    lbl_1_bss_16818->stat &= ~0x1000;
    lbl_1_bss_16818->mode = 1;
    lbl_1_bss_16818->rotSpeed = 2;
    HuPrcChildCreate(fn_1_3DE4, 100, 4096, 0, HuPrcCurrentGet());
    HuPrcVSleep();
    while (!(lbl_1_bss_16818->stat & 0x1000)) {
        HuPrcVSleep();
    }
    HuPrcChildCreate(fn_1_FBD0, 100, 4096, 0, HuPrcCurrentGet());
    HuPrcVSleep();
    while (!(lbl_1_bss_16818->flag & 1)) {
        HuPrcVSleep();
    }
    HuPrcChildCreate(fn_1_FC34, 100, 4096, 0, HuPrcCurrentGet());
    HuPrcVSleep();
    while (!(lbl_1_bss_16818->flag & 2)) {
        HuPrcVSleep();
    }
    lbl_1_bss_16818->stat &= ~0x1000;
    lbl_1_bss_16818->mode = 0;
    lbl_1_bss_16818->rotSpeed = 2;
    HuPrcChildCreate(fn_1_3DE4, 100, 4096, 0, HuPrcCurrentGet());
    HuPrcVSleep();
    while (!(lbl_1_bss_16818->stat & 0x1000)) {
        HuPrcVSleep();
    }
    for (i = 30; i--;) {
        HuPrcVSleep();
    }
    if (lbl_1_bss_16818->stat & 0x20) {
        lbl_1_bss_16818->stat &= ~0x20;
        HuWinExClose(lbl_1_bss_16818->winId);
        HuWinHomeClear(lbl_1_bss_16818->winId);
    }
    WipeCreate(WIPE_MODE_OUT, 6, 60);
    while (WipeCheckEnd()) {
        HuPrcVSleep();
    }
    lbl_1_bss_16818->stat |= 0x400;
    Hu3DCameraKill(HU3D_CAM4);
    camPos.y = 0;
    camPos.x = 0;
    camPos.z = lbl_1_bss_1681C->camPos.z;
    camUp.z = 0;
    camUp.x = 0;
    camUp.y = 1;
    camTarget.z = 0;
    camTarget.y = 0;
    camTarget.x = 0;
    Hu3DCameraCreate(HU3D_CAM1);
    Hu3DCameraViewportSet(HU3D_CAM2, 0, 0, 640, 480, 0, 1);
    Hu3DCameraScissorSet(HU3D_CAM2, 0, 0, 640, 480);
    Hu3DCameraPosSetV(HU3D_CAM2, &camPos, &camUp, &camTarget);
    Hu3DCameraPerspectiveSet(HU3D_CAM2, lbl_1_bss_1681C->aspect, 20, 13000, 1.2f);
    interval = 200;
    for (i = 4; i--;) {
        mdlId = lbl_1_bss_16818->mdl[i + 7];
        Hu3DModelPosSet(mdlId, -(1.5f * interval) + (i * interval), 0, lbl_1_data_1B4[lbl_1_bss_16818->player[i].charNo]);
        Hu3DModelAttrReset(mdlId, 1);
        Hu3DModelCameraSet(mdlId, HU3D_CAM1);
        Hu3DModelLayerSet(mdlId, 0);
        mdlId2 = lbl_1_bss_16818->mdl[i + 12];
        Hu3DModelAttrReset(mdlId2, 1);
        Hu3DModelLayerSet(mdlId2, 4);
        Hu3DModelCameraSet(mdlId2, HU3D_CAM2);
    }
    Hu3DCameraCreate(HU3D_CAM1);
    Hu3DCameraPerspectiveSet(HU3D_CAM1, 1.2f, 15000, 40000, 1.2f);
    Hu3DCameraViewportSet(HU3D_CAM1, 0, 0, 640, 480, 0, 1);
    Hu3DCameraScissorSet(HU3D_CAM1, 0, 0, 640, 480);
    Hu3DCameraPosSet(HU3D_CAM1, 0, 100, 30000, 0, 1, 0, 0, 100, 0); /* original passes an unset f9 as target Z */
    Hu3DModelAttrSet(lbl_1_bss_16818->mdl[5], 1);
    Hu3DModelAttrReset(lbl_1_bss_16818->mdl[6], 1);
    Hu3DModelAttrReset(lbl_1_bss_16818->mdl[4], 1);
    fn_1_C804();
    fn_1_D820(1, lbl_1_bss_16818);
    fn_1_CFEC(1);
    seId = HuAudFXPlay(0x4A4);
    fn_1_D61C(2);
    WipeCreate(WIPE_MODE_IN, 5, 60);
    while (WipeCheckEnd()) {
        HuPrcVSleep();
    }
    while (!fn_1_D630()) {
        HuPrcVSleep();
    }
    HuAudFXStop(seId);
    for (i = 30; i--;) {
        HuPrcVSleep();
    }
}

SHARED_SYM extern MGDATA mgDataTbl[];

static Vec lbl_1_data_28[6] = {
    { -160.0f, 0.0f, 160.0f }, { 0.0f, 0.0f, 160.0f }, { 160.0f, 0.0f, 160.0f },
    { 320.0f, 0.0f, 160.0f }, { -180.0f, 31.0f, 420.0f }, { 167.0f, 31.0f, 420.0f }
};

static Vec lbl_1_data_7C[5] = {
    { 0.0f, 100.0f, 0.0f }, { -320.0f, 100.0f, 140.0f }, { -420.0f, 100.0f, 140.0f },
    { -380.0f, 100.0f, 140.0f }, { 20.0f, 100.0f, 140.0f }
};

static f32 lbl_1_data_1B4[13] = {
    0.0f, 0.0f, 1000.0f, 0.0f, -3000.0f, 1000.0f, 0.0f, 0.0f, -8000.0f, -3000.0f, -3000.0f, -3000.0f, -3000.0f
};

static u32 lbl_1_data_248[10][4] = {
    { 0x1F8, 0x200, 0x1FA, 0x1FF }, { 0x1E4, 0x1EC, 0x1E6, 0x1EB }, { 0x25C, 0x264, 0x25E, 0x263 },
    { 0x2AC, 0x2B4, 0x2AE, 0x2B3 }, { 0x284, 0x28C, 0x286, 0x28B }, { 0x1BC, 0x1C4, 0x1BE, 0x1C3 },
    { 0x298, 0x2A0, 0x29A, 0x29F }, { 0x1D0, 0x1D8, 0x1D2, 0x1D7 }, { 0x270, 0x278, 0x272, 0x277 },
    { 0x234, 0x23C, 0x236, 0x23B }
};

void fn_1_10110(void);
void fn_1_C7BC(s32 *a, s32 *b);
void fn_1_C804(void);
void fn_1_CFEC(s32 arg);
void fn_1_D61C(s32 arg);
s32 fn_1_D630(void);
void fn_1_D820(s32 arg, MGT_FLOW *flow);

void fn_1_6538(void)
{
    s32 sp3C[4];
    s32 sp4C[4];
    Vec camPos;
    Vec camUp;
    Vec camTarget;
    MGT_PLAYER *pl;
    s32 i;
    s32 cnt;
    s32 t;
    s32 w;
    s8 j;
#ifdef TARGET_PC
    s8 winner = 0;
#else
    s8 winner;
#endif
    f32 spacing;
    s32 mdl;
    s8 *pno;

    switch (lbl_1_bss_1681C->mgNum) {
    case 0:
        lbl_1_bss_16818->player1No = lbl_1_bss_1681C->playerNo[0];
        lbl_1_bss_16818->player2No = lbl_1_bss_1681C->playerNo[1];
        break;
    case 1:
        lbl_1_bss_16818->player1No = lbl_1_bss_1681C->playerNo[2];
        lbl_1_bss_16818->player2No = lbl_1_bss_1681C->playerNo[3];
        break;
    case 2:
        lbl_1_bss_16818->player1No = lbl_1_bss_1681C->playerNo[lbl_1_bss_1681C->cur2 == 0];
        lbl_1_bss_16818->player2No = lbl_1_bss_1681C->playerNo[2 + (lbl_1_bss_1681C->cur3 == 0)];
        break;
    case 3:
        lbl_1_bss_16818->player1No = lbl_1_bss_1681C->playerNo[lbl_1_bss_1681C->cur2];
        lbl_1_bss_16818->player2No = lbl_1_bss_1681C->playerNo[2 + lbl_1_bss_1681C->cur3];
        break;
    }
    for (i = 0; i < 2; i++) {
        pno = &lbl_1_bss_16818->player1No;
        pl = &lbl_1_bss_16818->player[pno[i]];
        pl->pos = lbl_1_data_28[i + 4];
        if (pl->state != 0) {
            CharMotionShiftSet(pl->charNo, pl->motId[0], 0.0f, 5.0f, 0x40000001);
            pl->state = 0;
        }
        Hu3DModelAttrReset(pl->modelId, 1);
        Hu3DModelPosSet(pl->modelId, pl->pos.x, pl->pos.y, pl->pos.z);
    }
    lbl_1_bss_16818->shakePos = lbl_1_data_7C[2];
    Hu3DModelPosSetV(lbl_1_bss_16818->mdlShake, &lbl_1_bss_16818->shakePos);
    Hu3DModelRotSet(lbl_1_bss_16818->mdlShake, 0.0f, 0.0f, 0.0f);
    Hu3DCameraMotionOff(lbl_1_bss_16818->mdl[0]);
    mdl = lbl_1_bss_16818->mdl[0];
    Hu3DMotionStartEndSet(mdl, 180.0f, 359.0f);
    Hu3DMotionTimeSet(mdl, 180.0f);
    Hu3DMotionShapeTimeSet(mdl, 180.0f);
    Hu3DModelAttrReset(lbl_1_bss_16818->mdl[0], 0x40000082);
    Hu3DModelAttrSet(lbl_1_bss_16818->mdl[0], 0x40000041);
    HuTHPSprCreateVol(mgDataTbl[lbl_1_bss_16818->mgTbl[lbl_1_bss_1681C->order[lbl_1_bss_1681C->cur]].mgIdx].movie[0], 1, 0, 0.0f);
    Hu3DModelMatHookSet(lbl_1_bss_16818->mdl[5], HuTHPMatHook);
    i = 4;
    while (i--) {
        sp3C[i] = lbl_1_bss_1681C->playerNo[i];
        sp4C[i] = lbl_1_bss_16818->player[i].charNo;
    }
    fn_1_C7BC(sp3C, sp4C);
    lbl_1_bss_16818->camPos = lbl_1_bss_1681C->camPos;
    lbl_1_bss_16818->camUp = lbl_1_bss_1681C->camTarget;
    lbl_1_bss_16818->stat |= 0x4000;
    HuPrcChildCreate(fn_1_3BF0, 0x64, 0x1000, 0, HuPrcCurrentGet());
    HuPrcVSleep();
    cnt = 0;
    j = 2;
    while (j--) {
        pno = &lbl_1_bss_16818->player1No;
        OSReport("\t[Player %d : No %d] = Coin %d\n", j, pno[j], GwPlayer[pno[j]].mgCoinBonus);
        if (GwPlayer[pno[j]].mgCoinBonus >= 10) {
            winner = j;
            cnt++;
        }
    }
    if (cnt != 1) {
        lbl_1_bss_16818->bgmId = HuAudBGMPlay(0x23);
        WipeCreate(1, 5, 60);
        WipeWait();
        lbl_1_bss_16818->stat |= 8;
        pl = &lbl_1_bss_16818->player[lbl_1_bss_16818->player1No];
        HuAudFXPlayPan(lbl_1_data_248[pl->charNo][1], (s32)(32.0f * (pl->pos.x / 200.0f)) + 0x40);
        if (pl->state != 3) {
            CharMotionShiftSet(pl->charNo, pl->motId[3], 0.0f, 5.0f, 0);
            pl->state = 3;
        }
        pl = &lbl_1_bss_16818->player[lbl_1_bss_16818->player2No];
        HuAudFXPlayPan(lbl_1_data_248[pl->charNo][1], (s32)(32.0f * (pl->pos.x / 200.0f)) + 0x40);
        if (pl->state != 3) {
            CharMotionShiftSet(pl->charNo, pl->motId[3], 0.0f, 5.0f, 0);
            pl->state = 3;
        }
        t = 180;
        HuPrcChildCreate(fn_1_10110, 0x64, 0x1000, 0, HuPrcCurrentGet());
        HuPrcVSleep();
        if (GwPlayerConf[lbl_1_bss_16818->player1No].type != 0 && GwPlayerConf[lbl_1_bss_16818->player2No].type != 0) {
            lbl_1_bss_16818->keyMask = 0x200;
            HuPrcChildCreate(fn_1_A7C, 0xC8, 0x3000, 0, HuPrcCurrentGet());
        }
        while (!(lbl_1_bss_16818->flag & 0x200)) {
            HuPrcVSleep();
            t--;
        }
        if (t > 0) {
            w = t;
            while (w--) {
                HuPrcVSleep();
            }
        }
        if (lbl_1_bss_16818->stat & 0x20) {
            lbl_1_bss_16818->stat &= ~0x20;
            HuWinExClose(lbl_1_bss_16818->winId);
            HuWinHomeClear(lbl_1_bss_16818->winId);
        }
        WipeCreate(2, 6, 60);
        WipeWait();
        pl = &lbl_1_bss_16818->player[lbl_1_bss_16818->player1No];
        if (pl->state != 0) {
            CharMotionShiftSet(pl->charNo, pl->motId[0], 0.0f, 0.0f, 0x40000001);
            pl->state = 0;
        }
        pl = &lbl_1_bss_16818->player[lbl_1_bss_16818->player2No];
        if (pl->state != 0) {
            CharMotionShiftSet(pl->charNo, pl->motId[0], 0.0f, 0.0f, 0x40000001);
            pl->state = 0;
        }
        lbl_1_bss_16818->stat |= 0x400;
        Hu3DCameraKill(0x10);
        camPos.x = camPos.y = 0.0f;
        camPos.z = lbl_1_bss_1681C->camPos.z;
        camUp.x = camUp.z = 0.0f;
        camUp.y = 1.0f;
        camTarget.x = camTarget.y = camTarget.z = 0.0f;
        Hu3DCameraCreate(2);
        Hu3DCameraViewportSet(4, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
        Hu3DCameraScissorSet(4, 0, 0, 640, 480);
        Hu3DCameraPosSetV(4, &camPos, &camUp, &camTarget);
        Hu3DCameraPerspectiveSet(4, lbl_1_bss_1681C->aspect, 20.0f, 13000.0f, 1.2f);
        spacing = 200.0f;
        i = 4;
        while (i--) {
            mdl = lbl_1_bss_16818->mdl[i + 7];
            Hu3DModelPosSet(mdl, -(1.5f * spacing) + (f32)i * spacing, 0.0f, lbl_1_data_1B4[lbl_1_bss_16818->player[i].charNo]);
            Hu3DModelAttrReset(mdl, 1);
            Hu3DModelCameraSet(mdl, 2);
            Hu3DModelLayerSet(mdl, 0);
            mdl = lbl_1_bss_16818->mdl[i + 12];
            Hu3DModelAttrReset(mdl, 1);
            Hu3DModelLayerSet(mdl, 4);
            Hu3DModelCameraSet(mdl, 4);
        }
        Hu3DCameraCreate(2);
        Hu3DCameraPerspectiveSet(2, 1.2f, 15000.0f, 40000.0f, 1.2f);
        Hu3DCameraViewportSet(2, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
        Hu3DCameraScissorSet(2, 0, 0, 640, 480);
        Hu3DCameraPosSet(2, 0.0f, 100.0f, 30000.0f, 0.0f, 1.0f, 0.0f, 0.0f, 100.0f, 0.0f);
        Hu3DModelAttrSet(lbl_1_bss_16818->mdl[5], 1);
        Hu3DModelAttrReset(lbl_1_bss_16818->mdl[6], 1);
        Hu3DModelAttrReset(lbl_1_bss_16818->mdl[4], 1);
        fn_1_C804();
        fn_1_D820(1, lbl_1_bss_16818);
        fn_1_CFEC(1);
        fn_1_D61C(6);
        while (!fn_1_D630()) {
            HuPrcVSleep();
        }
        WipeCreate(1, 5, 60);
        WipeWait();
        return;
    }
    lbl_1_bss_16818->turnNo = winner;
    WipeCreate(1, 5, 60);
    WipeWait();
}

void fn_1_10178(void);

void fn_1_7450(void)
{
    Vec camTarget;
    Vec camUp;
    Vec camPos;
    MGT_PLAYER *pl;
    s32 mdl;
    s32 k;
    OMOVLHIS *his;
    s32 i;
    s32 t;
    s32 pan;
    f32 spacing;

    HuAudJinglePlay(0x86);
    HuPrcChildCreate(fn_1_10178, 0x64, 0x1000, 0, HuPrcCurrentGet());
    HuPrcVSleep();
    if (GwPlayerConf[lbl_1_bss_16818->player1No].type != 0 && GwPlayerConf[lbl_1_bss_16818->player2No].type != 0) {
        lbl_1_bss_16818->keyMask = 0x400;
        HuPrcChildCreate(fn_1_A7C, 0xC8, 0x3000, 0, HuPrcCurrentGet());
    }
    while (!(lbl_1_bss_16818->flag & 0x400)) {
        HuPrcVSleep();
    }
    pl = &lbl_1_bss_16818->player[(&lbl_1_bss_16818->player1No)[lbl_1_bss_16818->turnNo]];
    pan = (s32)(32.0f * (pl->pos.x / 200.0f)) + 0x40;
    HuAudFXPlayPan(lbl_1_data_248[pl->charNo][0], pan);
    if (pl->state != 2) {
        CharMotionShiftSet(pl->charNo, pl->motId[2], 0.0f, 5.0f, 0);
        pl->state = 2;
    }
    lbl_1_bss_16818->stat &= ~0x800;
    HuPrcChildCreate(fn_1_2E68, 0x64, 0x1000, 0, HuPrcCurrentGet());
    HuPrcVSleep();
    t = 120;
    while (t--) {
        HuPrcVSleep();
    }
    if (lbl_1_bss_16818->stat & 0x20) {
        lbl_1_bss_16818->stat &= ~0x20;
        HuWinExClose(lbl_1_bss_16818->winId);
        HuWinHomeClear(lbl_1_bss_16818->winId);
    }
    while (!(lbl_1_bss_16818->stat & 0x800)) {
        HuPrcVSleep();
    }
    lbl_1_bss_16818->bgmId = HuAudBGMPlay(0x23);
    i = 4;
    while (i--) {
        pl = &lbl_1_bss_16818->player[i];
        if (pl->state != 0) {
            CharMotionShiftSet(pl->charNo, pl->motId[0], 0.0f, 5.0f, 0x40000001);
            pl->state = 0;
        }
    }
    WipeCreate(2, 6, 60);
    WipeWait();
    lbl_1_bss_16818->stat |= 0x400;
    Hu3DCameraKill(0x10);
    camPos.x = camPos.y = 0.0f;
    camPos.z = lbl_1_bss_1681C->camPos.z;
    camUp.x = camUp.z = 0.0f;
    camUp.y = 1.0f;
    camTarget.x = camTarget.y = camTarget.z = 0.0f;
    Hu3DCameraCreate(2);
    Hu3DCameraViewportSet(4, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(4, 0, 0, 640, 480);
    Hu3DCameraPosSetV(4, &camPos, &camUp, &camTarget);
    Hu3DCameraPerspectiveSet(4, lbl_1_bss_1681C->aspect, 20.0f, 13000.0f, 1.2f);
    spacing = 200.0f;
    k = 4;
    while (k--) {
        mdl = lbl_1_bss_16818->mdl[k + 7];
        Hu3DModelPosSet(mdl, -(1.5f * spacing) + (f32)k * spacing, 0.0f, lbl_1_data_1B4[lbl_1_bss_16818->player[k].charNo]);
        Hu3DModelAttrReset(mdl, 1);
        Hu3DModelCameraSet(mdl, 2);
        Hu3DModelLayerSet(mdl, 0);
        mdl = lbl_1_bss_16818->mdl[k + 12];
        Hu3DModelAttrReset(mdl, 1);
        Hu3DModelLayerSet(mdl, 4);
        Hu3DModelCameraSet(mdl, 4);
    }
    Hu3DCameraCreate(2);
    Hu3DCameraPerspectiveSet(2, 1.2f, 15000.0f, 40000.0f, 1.2f);
    Hu3DCameraViewportSet(2, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(2, 0, 0, 640, 480);
    Hu3DCameraPosSet(2, 0.0f, 100.0f, 30000.0f, 0.0f, 1.0f, 0.0f, 0.0f, 100.0f, 0.0f);
    Hu3DModelAttrSet(lbl_1_bss_16818->mdl[5], 1);
    Hu3DModelAttrReset(lbl_1_bss_16818->mdl[6], 1);
    Hu3DModelAttrReset(lbl_1_bss_16818->mdl[4], 1);
    fn_1_C804();
    fn_1_D820(1, lbl_1_bss_16818);
    fn_1_CFEC(1);
    WipeCreate(1, 5, 60);
    WipeWait();
    fn_1_D61C(10);
    while (!fn_1_D630()) {
        HuPrcVSleep();
    }
    (&lbl_1_bss_1681C->cur2)[lbl_1_bss_1681C->mgNum] = lbl_1_bss_16818->turnNo;
    lbl_1_bss_1681C->mgNum += 1;
    if (lbl_1_bss_1681C->mgNum == 4) {
        fn_1_D61C(8);
        while (!fn_1_D630()) {
            HuPrcVSleep();
        }
        lbl_1_bss_1681C->flag |= 1;
        his = omOvlHisGet(0);
        omOvlHisChg(0, his->ovl, 2, his->stat);
    }
}

void fn_1_FC90(void);
void fn_1_FD3C(void);
void fn_1_FDE8(void);
void fn_1_FE94(void);
void fn_1_FF40(void);
void fn_1_FF9C(void);
void fn_1_10018(void);
void fn_1_D418(s32 arg);
void fn_1_C76C(void);

void fn_1_7CB8(void)
{
    s32 i;
    s32 j;
    s32 r;
    OMOVLHIS *his;
    s8 *order;
    s32 mes;
    u8 last;
    s32 t;
    s32 n;
    s8 mgIdx;
    s8 tmp[0x100];

    if (lbl_1_bss_1681C->cur == lbl_1_bss_16818->mgTblNum) {
#ifdef TARGET_PC
        last = lbl_1_bss_1681C->order[(lbl_1_bss_16818->mgTblNum > 0x0B ? 0x0B : lbl_1_bss_16818->mgTblNum) - 1];
#else
        last = *(lbl_1_bss_16818->mgTblNum + (u8 *)lbl_1_bss_1681C);
#endif
        lbl_1_bss_1681C->cur = 0;
        n = lbl_1_bss_16818->mgTblNum;
#ifdef TARGET_PC
        if (n > 0x0B) {
            n = 0x0B;
        }
#endif
        order = lbl_1_bss_1681C->order;
        i = n;
        while (i--) {
            tmp[i] = order[i];
        }
        i = n;
        while (i--) {
            if (i != 0) {
                r = frandmod(i + 1);
            } else {
                r = 0;
            }
            order[i] = tmp[r];
            for (j = 0; j <= i; j++) {
                if (j != r && j > r) {
                    tmp[j - 1] = tmp[j];
                }
            }
        }
        if (lbl_1_bss_1681C->order[0] == (s8)last) {
            lbl_1_bss_1681C->order[0] = lbl_1_bss_1681C->order[lbl_1_bss_16818->mgTblNum / 2];
            lbl_1_bss_1681C->order[lbl_1_bss_16818->mgTblNum / 2] = last;
        }
    }
    mes = *(s16 *)&lbl_1_bss_16818->mgTbl[lbl_1_bss_1681C->order[lbl_1_bss_1681C->cur]].mes2;
    mgIdx = lbl_1_bss_16818->mgTbl[lbl_1_bss_1681C->order[lbl_1_bss_1681C->cur]].mgIdx;
    switch (lbl_1_bss_1681C->mgNum) {
    case 0:
        lbl_1_bss_16818->player1No = lbl_1_bss_1681C->playerNo[0];
        lbl_1_bss_16818->player2No = lbl_1_bss_1681C->playerNo[1];
        GwPlayerConf[lbl_1_bss_1681C->playerNo[0]].grp = 0;
        GwPlayerConf[lbl_1_bss_1681C->playerNo[1]].grp = 1;
        GwPlayerConf[lbl_1_bss_1681C->playerNo[2]].grp = 2;
        GwPlayerConf[lbl_1_bss_1681C->playerNo[3]].grp = 2;
        if (!(lbl_1_bss_16818->stat & 8)) {
            omVibrate(GwPlayerConf[lbl_1_bss_16818->player1No].padNo, 15, 4, 2);
            omVibrate(GwPlayerConf[lbl_1_bss_16818->player2No].padNo, 15, 4, 2);
            HuPrcChildCreate(fn_1_FC90, 0x64, 0x1000, 0, HuPrcCurrentGet());
            HuPrcVSleep();
            if (GwPlayerConf[lbl_1_bss_16818->player1No].type != 0 && GwPlayerConf[lbl_1_bss_16818->player2No].type != 0) {
                lbl_1_bss_16818->keyMask = 4;
                HuPrcChildCreate(fn_1_A7C, 0xC8, 0x3000, 0, HuPrcCurrentGet());
            }
            while (!(lbl_1_bss_16818->flag & 4)) {
                HuPrcVSleep();
            }
        }
        break;
    case 1:
        lbl_1_bss_16818->player1No = lbl_1_bss_1681C->playerNo[2];
        lbl_1_bss_16818->player2No = lbl_1_bss_1681C->playerNo[3];
        GwPlayerConf[lbl_1_bss_1681C->playerNo[0]].grp = 2;
        GwPlayerConf[lbl_1_bss_1681C->playerNo[1]].grp = 2;
        GwPlayerConf[lbl_1_bss_1681C->playerNo[2]].grp = 0;
        GwPlayerConf[lbl_1_bss_1681C->playerNo[3]].grp = 1;
        if (!(lbl_1_bss_16818->stat & 8)) {
            omVibrate(GwPlayerConf[lbl_1_bss_16818->player1No].padNo, 15, 4, 2);
            omVibrate(GwPlayerConf[lbl_1_bss_16818->player2No].padNo, 15, 4, 2);
            HuPrcChildCreate(fn_1_FD3C, 0x64, 0x1000, 0, HuPrcCurrentGet());
            HuPrcVSleep();
            if (GwPlayerConf[lbl_1_bss_16818->player1No].type != 0 && GwPlayerConf[lbl_1_bss_16818->player2No].type != 0) {
                lbl_1_bss_16818->keyMask = 8;
                HuPrcChildCreate(fn_1_A7C, 0xC8, 0x3000, 0, HuPrcCurrentGet());
            }
            while (!(lbl_1_bss_16818->flag & 8)) {
                HuPrcVSleep();
            }
        }
        break;
    case 2:
        lbl_1_bss_16818->player1No = lbl_1_bss_1681C->playerNo[lbl_1_bss_1681C->cur2 == 0];
        lbl_1_bss_16818->player2No = lbl_1_bss_1681C->playerNo[2 + (lbl_1_bss_1681C->cur3 == 0)];
        GwPlayerConf[lbl_1_bss_16818->player1No].grp = 0;
        GwPlayerConf[lbl_1_bss_16818->player2No].grp = 1;
        GwPlayerConf[lbl_1_bss_1681C->playerNo[lbl_1_bss_1681C->cur2]].grp = 2;
        GwPlayerConf[lbl_1_bss_1681C->playerNo[2 + lbl_1_bss_1681C->cur3]].grp = 2;
        if (!(lbl_1_bss_16818->stat & 8)) {
            omVibrate(GwPlayerConf[lbl_1_bss_16818->player1No].padNo, 15, 4, 2);
            omVibrate(GwPlayerConf[lbl_1_bss_16818->player2No].padNo, 15, 4, 2);
            HuPrcChildCreate(fn_1_FDE8, 0x64, 0x1000, 0, HuPrcCurrentGet());
            HuPrcVSleep();
            if (GwPlayerConf[lbl_1_bss_16818->player1No].type != 0 && GwPlayerConf[lbl_1_bss_16818->player2No].type != 0) {
                lbl_1_bss_16818->keyMask = 0x10;
                HuPrcChildCreate(fn_1_A7C, 0xC8, 0x3000, 0, HuPrcCurrentGet());
            }
            while (!(lbl_1_bss_16818->flag & 0x10)) {
                HuPrcVSleep();
            }
        }
        break;
    case 3:
        lbl_1_bss_16818->player1No = lbl_1_bss_1681C->playerNo[lbl_1_bss_1681C->cur2];
        lbl_1_bss_16818->player2No = lbl_1_bss_1681C->playerNo[2 + lbl_1_bss_1681C->cur3];
        GwPlayerConf[lbl_1_bss_16818->player1No].grp = 0;
        GwPlayerConf[lbl_1_bss_16818->player2No].grp = 1;
        GwPlayerConf[lbl_1_bss_1681C->playerNo[lbl_1_bss_1681C->cur2 == 0]].grp = 2;
        GwPlayerConf[lbl_1_bss_1681C->playerNo[2 + (lbl_1_bss_1681C->cur3 == 0)]].grp = 2;
        if (!(lbl_1_bss_16818->stat & 8)) {
            omVibrate(GwPlayerConf[lbl_1_bss_16818->player1No].padNo, 15, 4, 2);
            omVibrate(GwPlayerConf[lbl_1_bss_16818->player2No].padNo, 15, 4, 2);
            HuPrcChildCreate(fn_1_FE94, 0x64, 0x1000, 0, HuPrcCurrentGet());
            HuPrcVSleep();
            if (GwPlayerConf[lbl_1_bss_16818->player1No].type != 0 && GwPlayerConf[lbl_1_bss_16818->player2No].type != 0) {
                lbl_1_bss_16818->keyMask = 0x20;
                HuPrcChildCreate(fn_1_A7C, 0xC8, 0x3000, 0, HuPrcCurrentGet());
            }
            while (!(lbl_1_bss_16818->flag & 0x20)) {
                HuPrcVSleep();
            }
        }
        break;
    }
    if (!(lbl_1_bss_16818->stat & 8)) {
        lbl_1_bss_1681C->cur -= 1;
        fn_1_D61C(6);
        while (!fn_1_D630()) {
            HuPrcVSleep();
        }
        lbl_1_bss_1681C->cur += 1;
        t = 60;
        while (t--) {
            HuPrcVSleep();
        }
    }
    HuPrcChildCreate(fn_1_FF40, 0x64, 0x1000, 0, HuPrcCurrentGet());
    HuPrcVSleep();
    if (GwPlayerConf[lbl_1_bss_16818->player1No].type != 0 && GwPlayerConf[lbl_1_bss_16818->player2No].type != 0) {
        lbl_1_bss_16818->keyMask = 0x40;
        HuPrcChildCreate(fn_1_A7C, 0xC8, 0x3000, 0, HuPrcCurrentGet());
    }
    while (!(lbl_1_bss_16818->flag & 0x40)) {
        HuPrcVSleep();
    }
    if (lbl_1_bss_16818->stat & 0x20) {
        lbl_1_bss_16818->stat &= ~0x20;
        HuWinExClose(lbl_1_bss_16818->winId);
        HuWinHomeClear(lbl_1_bss_16818->winId);
    }
    lbl_1_bss_16818->stat |= 0x80;
    lbl_1_bss_16818->stat &= ~0x100;
    fn_1_D418(0);
    fn_1_D61C(4);
    while (!fn_1_D630()) {
        HuPrcVSleep();
    }
    while (!(lbl_1_bss_16818->stat & 0x100)) {
        HuPrcVSleep();
    }
    HuPrcChildCreate(fn_1_FF9C, 0x64, 0x1000, 0, HuPrcCurrentGet());
    HuPrcVSleep();
    if (GwPlayerConf[lbl_1_bss_16818->player1No].type != 0 && GwPlayerConf[lbl_1_bss_16818->player2No].type != 0) {
        lbl_1_bss_16818->keyMask = 0x80;
        HuPrcChildCreate(fn_1_A7C, 0xC8, 0x3000, 0, HuPrcCurrentGet());
    }
    while (!(lbl_1_bss_16818->flag & 0x80)) {
        HuPrcVSleep();
    }
    HuPrcChildCreate(fn_1_10018, 0x64, 0x1000, 0, HuPrcCurrentGet());
    HuPrcVSleep();
    if (GwPlayerConf[lbl_1_bss_16818->player1No].type != 0 && GwPlayerConf[lbl_1_bss_16818->player2No].type != 0) {
        lbl_1_bss_16818->keyMask = 0x100;
        HuPrcChildCreate(fn_1_A7C, 0xC8, 0x3000, 0, HuPrcCurrentGet());
    }
    while (!(lbl_1_bss_16818->flag & 0x100)) {
        HuPrcVSleep();
    }
    if (lbl_1_bss_16818->stat & 2) {
        if (lbl_1_bss_16818->stat & 0x20) {
            lbl_1_bss_16818->stat &= ~0x20;
            HuWinExClose(lbl_1_bss_16818->winId);
            HuWinHomeClear(lbl_1_bss_16818->winId);
        }
        his = omOvlHisGet(0);
        omOvlHisChg(0, his->ovl, 3, his->stat);
        return;
    }
    lbl_1_bss_16818->mgNo = mgIdx;
    HuDataDirClose(0x9A0000);
    HuDataDirClose(0x270000);
    CharModelDataClose(-1);
    if (!(lbl_1_bss_16818->stat & 4)) {
        HuPrcChildCreate(fn_1_4F0, 0xC8, 0x3000, 0, HuPrcCurrentGet());
    } else {
        lbl_1_bss_16818->loadState = 1;
    }
    if (lbl_1_bss_16818->bgmId >= 0) {
        HuAudSStreamFadeOut(lbl_1_bss_16818->bgmId, 1000);
        lbl_1_bss_16818->bgmId = -1;
    }
    if (lbl_1_bss_16818->seId >= 0) {
        HuAudFXStop(lbl_1_bss_16818->seId);
        lbl_1_bss_16818->seId = -1;
    }
    WipeCreate(2, 6, 60);
    WipeWait();
    lbl_1_bss_1681C->cur += 1;
    HuTHPClose();
    HuPrcVSleep();
    while (lbl_1_bss_16818->loadState == 0) {
        HuPrcVSleep();
    }
    while (HuTHPProcCheck()) {
        HuPrcVSleep();
    }
    GwSystem.mgNo = mes - 0x1F5;
    GWMgUnlockSet(mes);
    GwSystem.mgNo = mes - 0x1F5;
    if (lbl_1_bss_16818->stat & 4) {
        GwSystem.mgInstF = 1;
    } else {
        GwSystem.mgInstF = 0;
    }
    fn_1_C76C();
    omOvlCallEx(DLL_instdll, 1, 0, 0);
    while (1) {
        HuPrcVSleep();
    }
}

void fn_1_9094(void)
{
}

void fn_1_A5D8(void)
{
}

void fn_1_A5DC(void)
{
}

void fn_1_A644(void)
{
}

void fn_1_A6BC(void)
{
}

void fn_1_A910(void)
{
}

void fn_1_A94C(void)
{
}

void fn_1_B318(void)
{
}

void fn_1_B678(void)
{
}

void ObjectSetup(void)
{
}