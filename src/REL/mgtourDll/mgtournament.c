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

void fn_1_6538(void)
{
}

void fn_1_7450(void)
{
}

void fn_1_7CB8(void)
{
}

MGT_PAPER lbl_1_bss_0[MGTOUR_PAPER_MAX];
MGT_KAMIWORK lbl_1_bss_16760;

Vec lbl_1_data_35C[4] = {
    { 255.0f, 191.25f, 204.0f },
    { 255.0f, 255.0f, 153.0f },
    { 178.5f, 255.0f, 255.0f },
    { 153.0f, 255.0f, 191.25f },
};

Vec lbl_1_data_38C[4] = {
    { 1.0f, 1.0f, 1.0f },
    { 1.0f, 1.0f, 1.0f },
    { 1.0f, 1.0f, 1.0f },
    { 1.0f, 1.0f, 1.0f },
};

extern Vec lbl_1_data_28[];
extern f32 lbl_1_data_180[];
extern f32 lbl_1_data_1B4[];
extern s32 lbl_1_data_248[][4];
extern Vec lbl_1_data_7C[];

void fn_1_A6BC(Vec *pos);
void fn_1_A910(void);
void fn_1_B318(void);
void fn_1_B678(HU3DMODEL *modelP, Mtx *mtx);
void fn_1_456C(void);
void fn_1_10288(void);
void fn_1_102E4(void);
void fn_1_CB8C(s32 *order);
void fn_1_CFEC(s32 arg);
void fn_1_D29C(s32 arg);
void fn_1_D820(s32 arg, MGT_FLOW *flow);

void fn_1_9094(void)
{
    s32 order[4];
    s32 tbl[4];
    s32 a, b, c, d;
    Vec camPos;
    Vec camTarget;
    Vec camUp;
    Vec kamiPos[2];
    s8 shuf[4];
    MGT_PLAYER *player;
    MGT_PLAYER *p;
    HU3DMODELID model;
    s32 timer;
    s32 i;
    s32 j;
    u32 r;
    s8 turn;

    if (lbl_1_bss_1681C->mgNum <= 0) {
        a = -1;
    } else {
        a = lbl_1_bss_1681C->cur2;
    }
    if (a == 0) {
        tbl[0] = 0;
        tbl[2] = 1;
    } else {
        tbl[0] = 1;
        tbl[2] = 0;
    }
    if (lbl_1_bss_1681C->mgNum <= 1) {
        b = -1;
    } else {
        b = lbl_1_bss_1681C->cur3;
    }
    if (b == 0) {
        tbl[1] = 2;
        tbl[3] = 3;
    } else {
        tbl[1] = 3;
        tbl[3] = 2;
    }
    if (lbl_1_bss_1681C->mgNum <= 2) {
        c = -1;
    } else {
        c = lbl_1_bss_1681C->cur4;
    }
    if (c == 0) {
        order[2] = tbl[2];
        order[3] = tbl[3];
    } else {
        order[2] = tbl[3];
        order[3] = tbl[2];
    }
    if (lbl_1_bss_1681C->mgNum <= 3) {
        d = -1;
    } else {
        d = lbl_1_bss_1681C->cur5;
    }
    if (d == 0) {
        order[0] = tbl[0];
        order[1] = tbl[1];
    } else {
        order[0] = tbl[1];
        order[1] = tbl[0];
    }
    player = &lbl_1_bss_16818->player[lbl_1_bss_1681C->playerNo[order[0]]];
    turn = lbl_1_bss_16818->turnNo;
    lbl_1_bss_16818->charNo = player->charNo;
    if (lbl_1_bss_16818->bgmId >= 0) {
        HuAudSStreamFadeOut(lbl_1_bss_16818->bgmId, 1000);
        lbl_1_bss_16818->bgmId = -1;
    }
    WipeCreate(2, 6, 60);
    while (WipeCheckEnd() != 0) {
        HuPrcVSleep();
    }
    lbl_1_bss_16818->stat &= ~0x400;
    i = 4;
    while (i--) {
        model = lbl_1_bss_16818->mdl[i + 7];
        Hu3DModelCameraSet(model, 0x10);
        Hu3DModelLayerSet(model, 4);
    }
    Hu3DCameraCreate(0x10);
    Hu3DCameraKill(6);
    Hu3DCameraViewportSet(0x10, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(0x10, 0, 0, 640, 480);
    Hu3DCameraPosSetV(0x10, &lbl_1_bss_1681C->camPos, &lbl_1_bss_1681C->camUp, &lbl_1_bss_1681C->camTarget);
    Hu3DCameraPerspectiveSet(0x10, lbl_1_bss_1681C->aspect, 20.0f, 60000.0f, 1.2f);
    fn_1_CFEC(0);
    camPos = lbl_1_data_28[turn + 4];
    camTarget = lbl_1_data_28[turn + 4];
    camTarget.y = camTarget.y + (25.0f + (lbl_1_data_180[player->charNo] + CharModelHeightGet(player->charNo)));
    camPos.z += 800.0f;
    camPos.y = 300.0f + camTarget.y;
    Hu3DCameraViewportSet(0x10, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(0x10, 0, 0, 640, 480);
    Hu3DCameraPosSetV(0x10, &camPos, &lbl_1_bss_1681C->camUp, &camTarget);
    Hu3DCameraPerspectiveSet(0x10, lbl_1_bss_1681C->aspect, 20.0f, 60000.0f, 1.2f);
    lbl_1_bss_16818->camPos.x = camPos.x;
    lbl_1_bss_16818->camPos.y = camPos.y;
    lbl_1_bss_16818->camPos.z = camPos.z;
    lbl_1_bss_16818->camUp.x = camTarget.x;
    lbl_1_bss_16818->camUp.y = camTarget.y;
    lbl_1_bss_16818->camUp.z = camTarget.z;
    Hu3DModelAttrReset(player->modelId, 1);
    Hu3DModelCameraSet(player->modelId, 0x10);
    Hu3DModelLayerSet(player->modelId, 4);
    Hu3DModelPosSet(player->modelId, player->pos.x, player->pos.y, player->pos.z);
    lbl_1_bss_16818->shakePos.x = lbl_1_data_7C[4].x;
    lbl_1_bss_16818->shakePos.y = lbl_1_data_7C[4].y;
    lbl_1_bss_16818->shakePos.z = lbl_1_data_7C[4].z;
    Hu3DModelPosSetV(lbl_1_bss_16818->mdlShake, &lbl_1_bss_16818->shakePos);
    if (turn == 0) {
        Hu3DModelRotSet(lbl_1_bss_16818->mdlShake, 0.0f, -30.0f, 0.0f);
    } else {
        Hu3DModelRotSet(lbl_1_bss_16818->mdlShake, 0.0f, 30.0f, 0.0f);
    }
    model = lbl_1_bss_16818->mdl[4];
    camUp = lbl_1_data_28[turn + 4];
    camUp.y += 200.0f;
    Hu3DModelCameraSet(model, 0x10);
    Hu3DModelPosSetV(model, &camUp);
    lbl_1_bss_16818->stat |= 0x2000;
    HuPrcChildCreate(fn_1_456C, 100, 0x1000, 0, HuPrcCurrentGet());
    HuPrcVSleep();
    WipeCreate(1, 5, 60);
    while (WipeCheckEnd() != 0) {
        HuPrcVSleep();
    }
    kamiPos[0].x = lbl_1_data_28[turn + 4].x - 400.0f;
    kamiPos[0].y = 430.0f + lbl_1_data_28[turn + 4].y;
    kamiPos[0].z = 80.0f + lbl_1_data_28[turn + 4].z;
    kamiPos[1].x = 400.0f + lbl_1_data_28[turn + 4].x;
    kamiPos[1].y = 430.0f + lbl_1_data_28[turn + 4].y;
    kamiPos[1].z = 80.0f + lbl_1_data_28[turn + 4].z;
    fn_1_A6BC(kamiPos);
    HuAudFXPlayPan(lbl_1_data_248[player->charNo][2], 0x40);
    if (player->state != 2) {
        CharMotionShiftSet(player->charNo, player->motId[2], 0.0f, 5.0f, 0);
        player->state = 2;
    }
    HuAudSStreamPlay(GameMesCreate(4, 3, player->charNo, -1, -1, -1));
    timer = 225;
    while (timer--) {
        HuPrcVSleep();
    }
    HuPrcChildCreate(fn_1_10288, 100, 0x1000, 0, HuPrcCurrentGet());
    HuPrcVSleep();
    while ((lbl_1_bss_16818->flag & 0x1000) == 0) {
        HuPrcVSleep();
    }
    if ((lbl_1_bss_16818->stat & 0x20) != 0) {
        lbl_1_bss_16818->stat &= ~0x20;
        HuWinExClose(lbl_1_bss_16818->winId);
        HuWinHomeClear(lbl_1_bss_16818->winId);
    }
    WipeCreate(2, 6, 60);
    while (WipeCheckEnd() != 0) {
        HuPrcVSleep();
    }
    fn_1_A910();
    lbl_1_bss_16818->stat &= ~0x2000;
    Hu3DCameraViewportSet(0x10, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(0x10, 0, 0, 640, 480);
    Hu3DCameraPosSetV(0x10, &lbl_1_bss_1681C->camPos, &lbl_1_bss_1681C->camUp, &lbl_1_bss_1681C->camTarget);
    Hu3DCameraPerspectiveSet(0x10, lbl_1_bss_1681C->aspect, 20.0f, 60000.0f, 1.2f);
    lbl_1_bss_16818->camPos.x = lbl_1_bss_1681C->camPos.x;
    lbl_1_bss_16818->camPos.y = lbl_1_bss_1681C->camPos.y;
    lbl_1_bss_16818->camPos.z = lbl_1_bss_1681C->camPos.z;
    lbl_1_bss_16818->camUp.x = lbl_1_bss_1681C->camTarget.x;
    lbl_1_bss_16818->camUp.y = lbl_1_bss_1681C->camTarget.y;
    lbl_1_bss_16818->camUp.z = lbl_1_bss_1681C->camTarget.z;
    fn_1_CB8C(order);
    fn_1_D29C(1);
    i = 4;
    while (i--) {
        model = lbl_1_bss_16818->mdl[i + 7];
        Hu3DModelPosSet(model, -(1.5f * 200.0f) + ((f32)i * 200.0f), 0.0f, lbl_1_data_1B4[lbl_1_bss_16818->player[i].charNo]);
        Hu3DModelAttrReset(model, 1);
        Hu3DModelCameraSet(model, 2);
        Hu3DModelLayerSet(model, 0);
        model = lbl_1_bss_16818->mdl[i + 12];
        Hu3DModelAttrReset(model, 1);
        Hu3DModelLayerSet(model, 4);
        Hu3DModelCameraSet(model, 0x10);
    }
    Hu3DCameraCreate(2);
    Hu3DCameraPerspectiveSet(2, 1.2f, 15000.0f, 40000.0f, 1.2f);
    Hu3DCameraViewportSet(2, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(2, 0, 0, 640, 480);
    Hu3DCameraPosSet(2, 0.0f, 100.0f, 30000.0f, 0.0f, 1.0f, 0.0f, 0.0f, 100.0f, 0.0f);
    Hu3DModelAttrSet(lbl_1_bss_16818->mdlShake, 1);
    Hu3DModelCameraSet(lbl_1_bss_16818->mdl[4], 4);
    p = &lbl_1_bss_16818->player[lbl_1_bss_1681C->playerNo[order[0]]];
    p->state = -1;
    if (p->state != 2) {
        CharMotionSet(p->charNo, p->motId[2]);
        p->state = 2;
    }
    Hu3DModelAttrReset(p->modelId, 0x40000001);
    p = &lbl_1_bss_16818->player[lbl_1_bss_1681C->playerNo[order[1]]];
    p->state = -1;
    if (p->state != 0) {
        CharMotionShiftSet(p->charNo, p->motId[0], 0.0f, 0.0f, 0x40000001);
        p->state = 0;
    }
    p = &lbl_1_bss_16818->player[lbl_1_bss_1681C->playerNo[order[2]]];
    p->state = -1;
    if (p->state != 3) {
        CharMotionSet(p->charNo, p->motId[3]);
        p->state = 3;
    }
    Hu3DModelAttrReset(p->modelId, 0x40000001);
    p = &lbl_1_bss_16818->player[lbl_1_bss_1681C->playerNo[order[3]]];
    p->state = -1;
    if (p->state != 3) {
        CharMotionSet(p->charNo, p->motId[3]);
        p->state = 3;
    }
    Hu3DModelAttrReset(p->modelId, 0x40000001);
    HuPrcVSleep();
    HuPrcVSleep();
    HuPrcVSleep();
    HuPrcVSleep();
    HuPrcVSleep();
    HuPrcVSleep();
    fn_1_D820(0, lbl_1_bss_16818);
    lbl_1_bss_16818->bgmId = HuAudBGMPlay(0x48);
    WipeCreate(1, 5, 60);
    while (WipeCheckEnd() != 0) {
        HuPrcVSleep();
    }
    HuPrcChildCreate(fn_1_102E4, 100, 0x1000, 0, HuPrcCurrentGet());
    HuPrcVSleep();
    while ((lbl_1_bss_16818->flag & 0x2000) == 0) {
        HuPrcVSleep();
    }
    if ((lbl_1_bss_16818->stat & 0x10) != 0) {
        OMOVLHIS *his;
        if (lbl_1_bss_16818->bgmId >= 0) {
            HuAudSStreamFadeOut(lbl_1_bss_16818->bgmId, 1000);
            lbl_1_bss_16818->bgmId = -1;
        }
        if (lbl_1_bss_16818->seId >= 0) {
            HuAudFXStop(lbl_1_bss_16818->seId);
            lbl_1_bss_16818->seId = -1;
        }
        WipeCreate(2, 6, 60);
        while (WipeCheckEnd() != 0) {
            HuPrcVSleep();
        }
        lbl_1_bss_1681C->flag = 0;
        lbl_1_bss_1681C->cur = 0;
        if ((lbl_1_bss_16818->stat & 0x20) != 0) {
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
        {
            s8 k = 4;
            while (k--) {
                lbl_1_bss_1681C->playerNo[k] = k;
            }
        }
        i = 4;
        while (i--) {
            shuf[i] = lbl_1_bss_1681C->playerNo[i];
        }
        i = 4;
        while (i--) {
            if (i != 0) {
                r = frandmod(i + 1);
            } else {
                r = 0;
            }
            lbl_1_bss_1681C->playerNo[i] = shuf[r];
            for (j = 0; j <= i; j++) {
                if (j != r && j > r) {
                    shuf[j - 1] = shuf[j];
                }
            }
        }
        Hu3DModelLayerSet(lbl_1_bss_16818->mdl[4], 3);
        Hu3DModelAttrSet(lbl_1_bss_16818->mdl[4], 1);
        Hu3DModelCameraSet(lbl_1_bss_16818->mdl[4], 4);
        model = lbl_1_bss_16818->mdl[2];
        Hu3DMotionStartEndSet(model, 0.0f, 28.0f);
        Hu3DMotionTimeSet(model, 0.0f);
        Hu3DMotionShapeTimeSet(model, 0.0f);
        Hu3DModelAttrSet(lbl_1_bss_16818->mdl[2], 0x40000002);
        lbl_1_bss_16818->stat &= ~0x4000;
        HuTHPClose();
        HuPrcVSleep();
        while (HuTHPProcCheck() != 0) {
            HuPrcVSleep();
        }
        return;
    }
    {
        OMOVLHIS *his = omOvlHisGet(0);
        omOvlHisChg(0, his->ovl, 3, his->stat);
    }
}

void fn_1_A5D8(void)
{
}

void fn_1_A5DC(void)
{
    s32 timer = 120;

    while (lbl_1_bss_16760.active != 0) {
        timer--;
        if (timer == 0) {
            lbl_1_bss_16760.stopF = 0;
        }
        fn_1_B318();
        HuPrcVSleep();
    }
    HuPrcEnd();
}

f32 fn_1_A644(void)
{
    s32 n = frandmod(201) - 100;
    f32 r = n;

    r = r / 100.0f;
    return r;
}

void fn_1_A6BC(Vec *pos)
{
    memset(&lbl_1_bss_16760, 0, sizeof(MGT_KAMIWORK));
    lbl_1_bss_16760.active = 1;
    lbl_1_bss_16760.curIdx = 0;
    lbl_1_bss_16760.stopF = 1;
    lbl_1_bss_16760.time = 1.0f;
    lbl_1_bss_16760.modelId[0] = Hu3DModelCreate(HuDataSelHeapReadNum(0x9A000A, 0x10000000, HEAP_MODEL));
    lbl_1_bss_16760.modelId[1] = Hu3DModelCreate(HuDataSelHeapReadNum(0x9A000B, 0x10000000, HEAP_MODEL));
    lbl_1_bss_16760.modelId[2] = Hu3DModelCreate(HuDataSelHeapReadNum(0x9A000C, 0x10000000, HEAP_MODEL));
    lbl_1_bss_16760.modelId[3] = Hu3DModelCreate(HuDataSelHeapReadNum(0x9A000D, 0x10000000, HEAP_MODEL));
    sprintf(lbl_1_bss_16760.name[0], "%s", "m546-kami01");
    sprintf(lbl_1_bss_16760.name[1], "%s", "m546-kami02");
    sprintf(lbl_1_bss_16760.name[2], "%s", "m546-kami03");
    sprintf(lbl_1_bss_16760.name[3], "%s", "m546-kami04");
    lbl_1_bss_16760.pos[0] = pos[0];
    lbl_1_bss_16760.pos[1] = pos[1];
    lbl_1_bss_16760.timer = 10;
    lbl_1_bss_16760.timerMax = 1;
    lbl_1_bss_16760.hookMdlId = Hu3DHookFuncCreate(fn_1_B678);
    Hu3DModelCameraSet(lbl_1_bss_16760.hookMdlId, 0x10);
    Hu3DModelLayerSet(lbl_1_bss_16760.hookMdlId, 6);
    HuPrcChildCreate(fn_1_A5DC, 100, 0x1000, 0, HuPrcCurrentGet());
}

void fn_1_A910(void)
{
    lbl_1_bss_16760.active = 0;
    Hu3DModelKill(lbl_1_bss_16760.hookMdlId);
}

void fn_1_A94C(void)
{
    s16 i;
    s16 t;
    f32 ang;
    f32 t1, t2, t3, t4, t5;

    for (i = 0; i < MGTOUR_PAPER_MAX; i++) {
        if (lbl_1_bss_0[i].alive == 0) {
            lbl_1_bss_0[i].alive = 1;
            lbl_1_bss_0[i].kind = frandmod(30) + 0x88;
            lbl_1_bss_16760.time += 0.001f;
            lbl_1_bss_0[i].life = lbl_1_bss_16760.time;
            lbl_1_bss_0[i].moving = 1;
            ang = 360.0f * fn_1_A644();
            lbl_1_bss_0[i].posX = lbl_1_bss_16760.pos[lbl_1_bss_16760.curIdx].x + 25.0 * sin(3.141592653589793 * ang / 180.0);
            lbl_1_bss_0[i].posY = lbl_1_bss_16760.pos[lbl_1_bss_16760.curIdx].y;
            lbl_1_bss_0[i].posZ = lbl_1_bss_16760.pos[lbl_1_bss_16760.curIdx].z + 25.0 * cos(3.141592653589793 * ang / 180.0);
            lbl_1_bss_0[i].rotX = 0.0f;
            lbl_1_bss_0[i].rotY = 0.0f;
            lbl_1_bss_0[i].rotZ = 0.0f;
            if (lbl_1_bss_16760.curIdx != 0) {
                lbl_1_bss_0[i].velX = 3.0f * (fn_1_A644() - 4.0f);
            } else {
                lbl_1_bss_0[i].velX = 3.0f * (4.0f + fn_1_A644());
            }
            t1 = fn_1_A644();
            lbl_1_bss_0[i].velY = -1.0f * (6.5f * t1);
            if (lbl_1_bss_0[i].velY > 0.0f) {
                lbl_1_bss_0[i].velY *= -1.0f;
            }
            if (lbl_1_bss_0[i].velY > -2.6000001f) {
                lbl_1_bss_0[i].velY = -2.6000001f;
            }
            t2 = fn_1_A644();
            lbl_1_bss_0[i].velZ = 0.4f * t2;
            t3 = fn_1_A644();
            lbl_1_bss_0[i].rotVelX = 14.0f * t3;
            t4 = fn_1_A644();
            lbl_1_bss_0[i].rotVelY = 14.0f * t4;
            t5 = fn_1_A644();
            lbl_1_bss_0[i].rotVelZ = 14.0f * t5;
            t = frandmod(4);
            lbl_1_bss_0[i].color = t;
            lbl_1_bss_0[i].tint.x = lbl_1_data_35C[t].x;
            lbl_1_bss_0[i].tint.y = lbl_1_data_35C[t].y;
            lbl_1_bss_0[i].tint.z = lbl_1_data_35C[t].z;
            t = frandmod(4);
            lbl_1_bss_0[i].scale.x = lbl_1_data_38C[t].x;
            lbl_1_bss_0[i].scale.y = lbl_1_data_38C[t].y;
            lbl_1_bss_0[i].scale.z = lbl_1_data_38C[t].z;
            lbl_1_bss_0[i].alpha = 1.0f;
            lbl_1_bss_16760.total++;
            lbl_1_bss_16760.curIdx = (lbl_1_bss_16760.curIdx == 0);
            if (lbl_1_bss_16760.count++ >= lbl_1_bss_16760.timerMax) {
                break;
            }
        }
    }
}

void fn_1_B318(void)
{
    s16 i;
    f32 px, pz;

    if (lbl_1_bss_16760.active != 0) {
        for (i = 0; i < MGTOUR_PAPER_MAX; i++) {
            if (lbl_1_bss_0[i].alive != 0) {
                if (lbl_1_bss_0[i].moving != 0) {
                    lbl_1_bss_0[i].posX += lbl_1_bss_0[i].velX;
                    lbl_1_bss_0[i].posY += lbl_1_bss_0[i].velY;
                    lbl_1_bss_0[i].posZ += lbl_1_bss_0[i].velZ;
                    lbl_1_bss_0[i].rotX += lbl_1_bss_0[i].rotVelX;
                    lbl_1_bss_0[i].rotY += lbl_1_bss_0[i].rotVelY;
                    lbl_1_bss_0[i].rotZ += lbl_1_bss_0[i].rotVelZ;
                    if (fabsf(lbl_1_bss_0[i].velX) > 0.001) {
                        lbl_1_bss_0[i].velX -= lbl_1_bss_0[i].velX / 40.0f;
                    }
                }
                px = lbl_1_bss_0[i].posX;
                pz = lbl_1_bss_0[i].posZ;
            }
        }
        if (lbl_1_bss_16760.stopF != 0) {
            if (lbl_1_bss_16760.timer-- <= 0 && lbl_1_bss_16760.total < MGTOUR_PAPER_MAX) {
                lbl_1_bss_16760.timer = frandmod(2) + 1;
                lbl_1_bss_16760.timerMax = 1;
                lbl_1_bss_16760.count = 0;
                fn_1_A94C();
            }
        }
    }
}

void fn_1_B678(HU3DMODEL *modelP, Mtx *mtx)
{
    Mtx outM;
    Mtx rotM;
    s16 i;

    if (lbl_1_bss_16760.active != 0) {
        Hu3DModelObjDrawInit();
        for (i = 0; i < MGTOUR_PAPER_MAX; i++) {
            if (lbl_1_bss_0[i].alive != 0) {
                PSMTXRotRad(rotM, 'z', 0.017453292f * lbl_1_bss_0[i].rotZ);
                PSMTXRotRad(outM, 'x', 0.017453292f * lbl_1_bss_0[i].rotX);
                PSMTXConcat(rotM, outM, outM);
                PSMTXRotRad(rotM, 'y', 0.017453292f * lbl_1_bss_0[i].rotY);
                PSMTXConcat(rotM, outM, outM);
                PSMTXTrans(rotM, lbl_1_bss_0[i].posX, lbl_1_bss_0[i].posY, lbl_1_bss_0[i].posZ);
                PSMTXConcat(rotM, outM, outM);
                PSMTXConcat(*mtx, outM, outM);
                Hu3DModelTPLvlSet(lbl_1_bss_16760.modelId[lbl_1_bss_0[i].color], lbl_1_bss_0[i].alpha);
                Hu3DModelObjDraw(lbl_1_bss_16760.modelId[lbl_1_bss_0[i].color], lbl_1_bss_16760.name[lbl_1_bss_0[i].color], outM);
            }
        }
    }
}

void ObjectSetup(void)
{
}