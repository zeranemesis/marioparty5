#include "REL/mgtourDll.h"

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

void fn_1_1DE8(void)
{
}

void fn_1_1E1C(void)
{
}

void fn_1_1ED4(void)
{
}

void fn_1_2078(void)
{
}

void fn_1_242C(void)
{
}

void fn_1_25E4(void)
{
}

void fn_1_263C(void)
{
}

void fn_1_28C0(void)
{
}

void fn_1_2CE8(void)
{
}

void fn_1_2E68(void)
{
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
        lbl_1_bss_16818->player2No = lbl_1_bss_1681C->playerNo[lbl_1_bss_1681C->cur3 == 0];
        break;
    case 3:
        lbl_1_bss_16818->player1No = lbl_1_bss_1681C->playerNo[lbl_1_bss_1681C->cur2];
        lbl_1_bss_16818->player2No = lbl_1_bss_1681C->playerNo[lbl_1_bss_1681C->cur3];
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