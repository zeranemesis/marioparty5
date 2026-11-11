#include "REL/mgtourDll.h"
#include <math.h>

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

void fn_1_140(void)
{
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

void fn_1_DE4(void)
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

static BOOL MgtAngleSub(f32 *angle, f32 end, f32 speed)
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
}

void fn_1_3BF0(void)
{
}

void fn_1_3D2C(void)
{
}

void fn_1_3DE4(void)
{
}

void fn_1_44E0(void)
{
}

void fn_1_456C(void)
{
}

void fn_1_485C(void)
{
}

void fn_1_48B4(void)
{
}

void fn_1_5540(void)
{
}

void fn_1_56C0(void)
{
}

void fn_1_6538(void)
{
}

void fn_1_7450(void)
{
}

void fn_1_7CB8(void)
{
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