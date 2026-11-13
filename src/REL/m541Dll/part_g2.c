#include "REL/m541Dll.h"

#include <math.h>

#define G2_STAGE lbl_1_bss_3730
#define G2_PI 3.141592653589793

/* per-frame step of the two battle fighters (switch on the flow mode) */
s32 fn_1_5E48(void)
{
    HuVecF pos;
    s16 i;
    s16 allDone;
    s16 timerDone;
    s16 charNo;
    s32 ret;
    HU3DMODEL *model0 = &Hu3DData[G2_STAGE->model[0x6A].modelId];
    HU3DMODEL *model1 = &Hu3DData[G2_STAGE->model[0x6B].modelId];

    ret = 0;
    allDone = 1;
    lbl_1_bss_45C.count++;
    switch (lbl_1_bss_2734.mode) {
        case 0:
            fn_1_19E4(lbl_1_bss_2734.timer);
            lbl_1_bss_2734.mode++;
        case 1:
            timerDone = fn_1_1A10(lbl_1_bss_2734.timer);
            fn_1_11B7C(timerDone);
            if (lbl_1_bss_2734.cpuPlayer == 0 && timerDone == 0) {
                for (i = 0; i < 2; i++) {
                    if (lbl_1_bss_2614[i].waitMot != 0) {
                        fn_1_11BF8(i);
                    }
                }
            }
            allDone = allDone * fn_1_7850(0);
            allDone = allDone * fn_1_7850(1);
            if (lbl_1_bss_2734.cpuPlayer != 0 || timerDone != 0) {
                lbl_1_bss_2614[0].anim2 = 1;
                lbl_1_bss_2614[1].anim2 = 1;
                allDone = 1;
            }
            if (allDone != 0 || (timerDone != 0 && lbl_1_bss_2614[0].mode == 0 && lbl_1_bss_2614[1].mode == 0)) {
                if (lbl_1_bss_2734.cpuPlayer == 0) {
                    fn_1_2644(1, 100);
                }
                lbl_1_bss_2734.mode++;
            }
            break;
        case 2:
            if (lbl_1_bss_2734.cpuPlayer == 0) {
                fn_1_1A38(lbl_1_bss_2734.timer);
            }
            if (lbl_1_bss_2734.cpuPlayer != 0) {
                MgSeqWinnerSet(G2_STAGE->player[lbl_1_bss_2734.cpuPlayer & 1].charNo, -1, -1, -1);
                charNo = G2_STAGE->player[lbl_1_bss_2734.cpuPlayer & 1].playerNo;
                if (_CheckFlag(FLAG_MG_PRACTICE) == 0) {
                    GwPlayer[charNo].mgCoin = 10;
                }
            } else {
                MgSeqWinnerSet(-1, -1, -1, -1);
            }
            ret = 1;
            break;
    }
    for (i = 0; i < 2; i++) {
        if (lbl_1_bss_2614[i].waitMot != 0) {
            MgPlayerPadSet(lbl_1_bss_0.player[i], lbl_1_bss_2614[i].padStkX, lbl_1_bss_2614[i].padStkY,
                lbl_1_bss_2614[i].padBtnDown, lbl_1_bss_2614[i].padBtn);
        }
        if (lbl_1_bss_2614[i].anim2 != 0) {
            MgPlayerPadSet(lbl_1_bss_0.player[i], 0, 0, 0, 0);
        }
    }
    fn_1_A884();
    fn_1_92FC(1);
    MgActorPosGet(lbl_1_bss_0.player[0]->actor, &pos);
    lbl_1_bss_2614[0].velX = pos.x;
    lbl_1_bss_2614[0].velY = pos.y;
    lbl_1_bss_2614[0].velZ = pos.z;
    MgActorPosGet(lbl_1_bss_0.player[1]->actor, &pos);
    lbl_1_bss_2614[1].velX = pos.x;
    lbl_1_bss_2614[1].velY = pos.y;
    lbl_1_bss_2614[1].velZ = pos.z;
    lbl_1_bss_2614[0].stunTime = model0->rot.y;
    lbl_1_bss_2614[1].stunTime = model1->rot.y;
    MgActorExec();
    MgActorPosGet(lbl_1_bss_0.player[0]->actor, &pos);
    lbl_1_bss_2614[0].pos.x = pos.x;
    lbl_1_bss_2614[0].pos.y = pos.y;
    lbl_1_bss_2614[0].pos.z = pos.z;
    MgActorPosGet(lbl_1_bss_0.player[1]->actor, &pos);
    lbl_1_bss_2614[1].pos.x = pos.x;
    lbl_1_bss_2614[1].pos.y = pos.y;
    lbl_1_bss_2614[1].pos.z = pos.z;
    fn_1_E930(lbl_1_bss_2614);
    for (i = 0; i < 2; i++) {
        if ((s32)lbl_1_bss_2614[i].pos.x > 700 || (s32)lbl_1_bss_2614[i].pos.x < -700
            || (s32)lbl_1_bss_2614[i].pos.z > 700 || (s32)lbl_1_bss_2614[i].pos.z < -700) {
            /* out of bounds: restore the saved positions (the asm always restores slot 0 and 1 from the same fixed fields) */
            lbl_1_bss_2614[0].pos.x = lbl_1_bss_2614[0].velX;
            lbl_1_bss_2614[0].pos.y = lbl_1_bss_2614[0].velY;
            lbl_1_bss_2614[0].pos.z = lbl_1_bss_2614[0].velZ;
            lbl_1_bss_2614[1].pos.x = lbl_1_bss_2614[1].velX;
            lbl_1_bss_2614[1].pos.y = lbl_1_bss_2614[1].velY;
            lbl_1_bss_2614[1].pos.z = lbl_1_bss_2614[1].velZ;
            Hu3DModelPosSet(G2_STAGE->model[0x6A].modelId, lbl_1_bss_2614[0].pos.x, lbl_1_bss_2614[0].pos.y, lbl_1_bss_2614[0].pos.z);
            Hu3DModelPosSet(G2_STAGE->model[0x6B].modelId, lbl_1_bss_2614[1].pos.x, lbl_1_bss_2614[1].pos.y, lbl_1_bss_2614[1].pos.z);
            Hu3DModelRotSet(G2_STAGE->model[0x6A].modelId, 0.0f, lbl_1_bss_2614[0].stunTime, 0.0f);
            Hu3DModelRotSet(G2_STAGE->model[0x6B].modelId, 0.0f, lbl_1_bss_2614[1].stunTime, 0.0f);
            lbl_1_bss_0.player[0]->actor->rotY = lbl_1_bss_2614[0].stunTime;
            lbl_1_bss_0.player[1]->actor->rotY = lbl_1_bss_2614[1].stunTime;
            lbl_1_bss_2614[0].chasePos.x = 0.0f;
            lbl_1_bss_2614[0].chasePos.z = 0.0f;
            lbl_1_bss_2614[1].chasePos.x = 0.0f;
            lbl_1_bss_2614[1].chasePos.z = 0.0f;
        }
    }
    pos.x = lbl_1_bss_2614[0].pos.x;
    pos.y = lbl_1_bss_2614[0].pos.y;
    pos.z = lbl_1_bss_2614[0].pos.z;
    MgActorPosUpdate(lbl_1_bss_0.player[0]->actor, &pos);
    pos.x = lbl_1_bss_2614[1].pos.x;
    pos.y = lbl_1_bss_2614[1].pos.y;
    pos.z = lbl_1_bss_2614[1].pos.z;
    MgActorPosUpdate(lbl_1_bss_0.player[1]->actor, &pos);
    if (lbl_1_bss_2734.cpuPlayer == 0) {
        fn_1_6EA8();
    }
    fn_1_758C();
    return ret;
}

/* initial step of the first mode */
s32 fn_1_6728(void)
{
    HuVecF pos;
    s32 i;
    s32 ret;
    HU3DMODEL *model0 = &Hu3DData[G2_STAGE->model[0x6A].modelId];
    HU3DMODEL *model1 = &Hu3DData[G2_STAGE->model[0x6B].modelId];

    ret = 0;
    lbl_1_bss_2734.mode = 0;
    lbl_1_bss_2734.unk2 = 0;
    lbl_1_bss_2734.unk4 = 0;
    lbl_1_bss_2734.unk6 = 0;
    fn_1_7850(0);
    fn_1_7850(1);
    MgPlayerPadSet(lbl_1_bss_0.player[0], 0, 0, 0, 0);
    MgPlayerPadSet(lbl_1_bss_0.player[1], 0, 0, 0, 0);
    MgActorPosGet(lbl_1_bss_0.player[0]->actor, &pos);
    lbl_1_bss_2614[0].velX = pos.x;
    lbl_1_bss_2614[0].velY = pos.y;
    lbl_1_bss_2614[0].velZ = pos.z;
    MgActorPosGet(lbl_1_bss_0.player[1]->actor, &pos);
    lbl_1_bss_2614[1].velX = pos.x;
    lbl_1_bss_2614[1].velY = pos.y;
    lbl_1_bss_2614[1].velZ = pos.z;
    lbl_1_bss_2614[0].stunTime = model0->rot.y;
    lbl_1_bss_2614[1].stunTime = model1->rot.y;
    MgActorExec();
    for (i = 0; i < 2; i++) {
        if ((s32)lbl_1_bss_2614[i].pos.x > 700 || (s32)lbl_1_bss_2614[i].pos.x < -700
            || (s32)lbl_1_bss_2614[i].pos.z > 700 || (s32)lbl_1_bss_2614[i].pos.z < -700) {
            lbl_1_bss_2614[0].pos.x = lbl_1_bss_2614[0].velX;
            lbl_1_bss_2614[0].pos.y = lbl_1_bss_2614[0].velY;
            lbl_1_bss_2614[0].pos.z = lbl_1_bss_2614[0].velZ;
            lbl_1_bss_2614[1].pos.x = lbl_1_bss_2614[1].velX;
            lbl_1_bss_2614[1].pos.y = lbl_1_bss_2614[1].velY;
            lbl_1_bss_2614[1].pos.z = lbl_1_bss_2614[1].velZ;
            Hu3DModelPosSet(G2_STAGE->model[0x6A].modelId, lbl_1_bss_2614[0].pos.x, lbl_1_bss_2614[0].pos.y, lbl_1_bss_2614[0].pos.z);
            Hu3DModelPosSet(G2_STAGE->model[0x6B].modelId, lbl_1_bss_2614[1].pos.x, lbl_1_bss_2614[1].pos.y, lbl_1_bss_2614[1].pos.z);
            Hu3DModelRotSet(G2_STAGE->model[0x6A].modelId, 0.0f, lbl_1_bss_2614[0].stunTime, 0.0f);
            Hu3DModelRotSet(G2_STAGE->model[0x6B].modelId, 0.0f, lbl_1_bss_2614[1].stunTime, 0.0f);
            lbl_1_bss_0.player[0]->actor->rotY = lbl_1_bss_2614[0].stunTime;
            lbl_1_bss_0.player[1]->actor->rotY = lbl_1_bss_2614[1].stunTime;
            lbl_1_bss_2614[0].chasePos.x = 0.0f;
            lbl_1_bss_2614[0].chasePos.z = 0.0f;
        }
    }
    pos.x = lbl_1_bss_2614[0].pos.x;
    pos.y = lbl_1_bss_2614[0].pos.y;
    pos.z = lbl_1_bss_2614[0].pos.z;
    MgActorPosUpdate(lbl_1_bss_0.player[0]->actor, &pos);
    pos.x = lbl_1_bss_2614[1].pos.x;
    pos.y = lbl_1_bss_2614[1].pos.y;
    pos.z = lbl_1_bss_2614[1].pos.z;
    MgActorPosUpdate(lbl_1_bss_0.player[1]->actor, &pos);
    fn_1_758C();
    return ret;
}

s32 fn_1_6BF0(void)
{
    s16 ret = 0;
    fn_1_7850(0);
    fn_1_7850(1);
    ret = fn_1_E0A4();
    fn_1_758C();
    return ret;
}

s32 fn_1_6C38(void)
{
    s32 ret = 0;
    M541_MODELWORK *work;

    if (G2_STAGE->unk178 == 0) {
        if (lbl_1_bss_2734.cpuPlayer != 0) {
            work = &G2_STAGE->work[lbl_1_bss_2734.cpuPlayer & 1];
            work->motNo = 6;
            work->speed = 1.0f;
            work->time = 0.0f;
            work->attrReset = 0;
            work->attr = 0x40000001;
        } else {
            work = &G2_STAGE->work[0];
            work->motNo = 7;
            work->speed = 1.0f;
            work->time = 0.0f;
            work->attrReset = 0;
            work->attr = 0x40000001;
            work = &G2_STAGE->work[1];
            work->motNo = 7;
            work->speed = 1.0f;
            work->time = 0.0f;
            work->attrReset = 0;
            work->attr = 0x40000001;
            CharLoseVoicePlay(G2_STAGE->player[0].charNo, G2_STAGE->player[1].charNo, -1, -1);
        }
        G2_STAGE->unk178 = 1;
    }
    fn_1_758C();
    if (lbl_1_bss_2734.cpuPlayer != 0) {
        CharModelVoicePanSet(G2_STAGE->player[lbl_1_bss_2734.cpuPlayer & 1].charNo, 64);
    }
    return ret;
}

s32 fn_1_6E20(void)
{
    s32 ret = 1;
    fn_1_758C();
    return ret;
}

s32 fn_1_6E50(void)
{
    s32 ret = 1;
    fn_1_2AB4();
    MgActorKill(lbl_1_bss_0.player[0]->actor);
    MgActorKill(lbl_1_bss_0.player[1]->actor);
    return ret;
}

/* drives the falling leaves and the leaf scale/position models */
void fn_1_6EA8(void)
{
    s16 i;

    for (i = 0; i < 15; i++) {
        if (lbl_1_bss_20[i].unk0 != 0) {
            if (lbl_1_bss_2F0.length <= 10.0f) {
                lbl_1_bss_20[i].vel.x = lbl_1_bss_2F0.length * cos(G2_PI * lbl_1_bss_20[i].angle / 180.0);
                lbl_1_bss_20[i].vel.y = 0.0f;
                lbl_1_bss_20[i].vel.z = lbl_1_bss_2F0.length * sin(G2_PI * lbl_1_bss_20[i].angle / 180.0);
            }
            if (lbl_1_bss_20[i].hidden != 0) {
                lbl_1_bss_20[i].vel.x = 0.0f;
                lbl_1_bss_20[i].vel.z = 0.0f;
            }
            G2_STAGE->model[lbl_1_bss_36A4[i].a].posX += lbl_1_bss_20[i].vel.x;
            G2_STAGE->model[lbl_1_bss_36A4[i].a].posZ += lbl_1_bss_20[i].vel.z;
            *(HuVecF *)&G2_STAGE->model[lbl_1_bss_36A4[i].b].posX = *(HuVecF *)&G2_STAGE->model[lbl_1_bss_36A4[i].a].posX;
            *(HuVecF *)&G2_STAGE->model[lbl_1_bss_36A4[i].c].posX = *(HuVecF *)&G2_STAGE->model[lbl_1_bss_36A4[i].a].posX;
            G2_STAGE->model[lbl_1_bss_36A4[i].a].rotY = 90.0f + -lbl_1_bss_20[i].angle;
        }
    }
    lbl_1_bss_2F0.length += 0.002f;
    if (lbl_1_bss_2F0.length > 10.0f) {
        lbl_1_bss_2F0.length = 10.0f;
    }
}

/* copies the model table (pos/rot/scale) into the HU3D models and pans the voices */
void fn_1_72D8(void)
{
    s16 i;
    s16 j;
    float pan;
    HU3DMODEL *model0 = &Hu3DData[G2_STAGE->model[0x6A].modelId];
    HU3DMODEL *model1 = &Hu3DData[G2_STAGE->model[0x6B].modelId];

    for (i = 0; i < 0x6A; i++) {
        Hu3DModelPosSet(G2_STAGE->model[i].modelId, G2_STAGE->model[i].posX, G2_STAGE->model[i].posY, G2_STAGE->model[i].posZ);
        Hu3DModelRotSet(G2_STAGE->model[i].modelId, G2_STAGE->model[i].rotX, G2_STAGE->model[i].rotY, G2_STAGE->model[i].rotZ);
    }
    for (i = 0x10, j = 0; i <= 0x19; i++, j++) {
        Hu3DModelScaleSet(G2_STAGE->model[i].modelId, lbl_1_bss_2F0.unk20[j].x, lbl_1_bss_2F0.unk20[j].y, lbl_1_bss_2F0.unk20[j].z);
    }
    pan = 0.055652175f * model0->pos.x;
    CharModelVoicePanSet(G2_STAGE->player[0].charNo, (s32)(64.0f + pan));
    pan = 0.055652175f * model1->pos.x;
    CharModelVoicePanSet(G2_STAGE->player[1].charNo, (s32)(64.0f + pan));
}

/* same as fn_1_72D8, then runs the per-frame hooks of the other units */
void fn_1_758C(void)
{
    s16 i;
    s16 j;
    float pan;
    HU3DMODEL *model0 = &Hu3DData[G2_STAGE->model[0x6A].modelId];
    HU3DMODEL *model1 = &Hu3DData[G2_STAGE->model[0x6B].modelId];

    for (i = 0; i < 0x6A; i++) {
        Hu3DModelPosSet(G2_STAGE->model[i].modelId, G2_STAGE->model[i].posX, G2_STAGE->model[i].posY, G2_STAGE->model[i].posZ);
        Hu3DModelRotSet(G2_STAGE->model[i].modelId, G2_STAGE->model[i].rotX, G2_STAGE->model[i].rotY, G2_STAGE->model[i].rotZ);
    }
    for (i = 0x10, j = 0; i <= 0x19; i++, j++) {
        Hu3DModelScaleSet(G2_STAGE->model[i].modelId, lbl_1_bss_2F0.unk20[j].x, lbl_1_bss_2F0.unk20[j].y, lbl_1_bss_2F0.unk20[j].z);
    }
    pan = 0.055652175f * model0->pos.x;
    CharModelVoicePanSet(G2_STAGE->player[0].charNo, (s32)(64.0f + pan));
    pan = 0.055652175f * model1->pos.x;
    CharModelVoicePanSet(G2_STAGE->player[1].charNo, (s32)(64.0f + pan));
    fn_1_91C(0);
    fn_1_10C4();
    fn_1_14C4();
}

/* fighter state machine for player idx; returns 1 once it reached the final state */
s32 fn_1_7850(s16 idx)
{
    M541_MODELWORK *work = &G2_STAGE->work[idx];
    s32 ret = 0;

    switch (lbl_1_bss_2614[idx].mode) {
        case 0:
            if (lbl_1_bss_2614[idx].stunMode != 0 && lbl_1_bss_2734.cpuPlayer == 0) {
                if (lbl_1_bss_2614[idx].stunMode == 2) {
                    fn_1_1A38(lbl_1_bss_2734.timer);
                    lbl_1_bss_2734.cpuPlayer = (idx ^ 1) | (idx * 2);
                }
                lbl_1_bss_2614[idx].anim2 = 1;
                lbl_1_bss_2614[idx].mode++;
            } else if (lbl_1_bss_2734.cpuPlayer != 0) {
                lbl_1_bss_2614[idx].mode = 6;
            }
            break;
        case 1:
            if (lbl_1_bss_0.player[idx]->mode == 0) {
                lbl_1_bss_2614[idx].mode++;
            }
            break;
        case 2:
            MgPlayerPauseOn(lbl_1_bss_0.player[idx]);
            lbl_1_bss_2614[idx].mode++;
            break;
        case 3:
            if (lbl_1_bss_2614[idx].stunMode == 1) {
                if (fn_1_7DE8(idx) != 0) {
                    lbl_1_bss_2614[idx].mode++;
                }
            } else {
                if (lbl_1_bss_2614[idx].sub == 1) {
                    lbl_1_bss_2614[idx].sub = 0;
                    lbl_1_bss_2614[idx].motion = 1;
                    if (lbl_1_bss_2734.cpuPlayer == 0) {
                        omVibrate(G2_STAGE->player[idx].playerNo, 30, 12, 0);
                    }
                } else if (lbl_1_bss_2614[idx].sub == 2) {
                    work->time = 12.0f;
                    lbl_1_bss_2614[idx].sub = 0;
                    lbl_1_bss_2614[idx].motion = 2;
                    if (lbl_1_bss_2734.cpuPlayer == 0) {
                        omVibrate(G2_STAGE->player[idx].playerNo, 30, 12, 0);
                    }
                } else if (lbl_1_bss_2614[idx].sub == 3) {
                    work->motNo = -1;
                    lbl_1_bss_2614[idx].sub = 0;
                    lbl_1_bss_2614[idx].motion = 0;
                }
                if (fn_1_832C(idx) != 0) {
                    lbl_1_bss_2614[idx].mode++;
                }
                if (lbl_1_bss_2614[idx].stunMode == 2) {
                    lbl_1_bss_2734.cpuPlayer = (idx ^ 1) | (idx * 2);
                }
            }
            break;
        case 4:
            MgPlayerPauseOff(lbl_1_bss_0.player[idx], &lbl_1_bss_2614[idx].pos);
            lbl_1_bss_2614[idx].mode++;
            break;
        case 5:
            if (lbl_1_bss_2734.cpuPlayer != 0) {
                lbl_1_bss_2614[idx].mode++;
            } else {
                lbl_1_bss_2614[idx].anim2 = 0;
                lbl_1_bss_2614[idx].stunMode = 0;
                lbl_1_bss_2614[idx].mode = 0;
            }
            break;
        case 6:
            ret = 1;
            break;
    }
    return ret;
}
