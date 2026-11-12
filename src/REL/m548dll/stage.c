#include "types.h"
#include "game/gamework.h"
#include "game/pad.h"
#include "string.h"
#include "REL/m548dll.h"

typedef s16 HU3DMODELID;
typedef s16 HU3DMOTID;

HU3DMODELID CharModelCreate(s16 charNo, s16 model);
HU3DMOTID CharMotionCreate(s16 charNo, u32 dataNum);
HU3DMODELID Hu3DModelCreate(void *data);
void *HuDataSelHeapReadNum(s32 dataNum, u32 tag, s32 heap);
void Hu3DModelPosSet(HU3DMODELID modelId, f32 posX, f32 posY, f32 posZ);
void Hu3DModelPosSetV(HU3DMODELID modelId, f32 *pos);
void Hu3DModelRotSet(HU3DMODELID modelId, f32 rotX, f32 rotY, f32 rotZ);
void Hu3DModelAttrSet(HU3DMODELID modelId, u32 attr);
void Hu3DModelAttrReset(HU3DMODELID modelId, u32 attr);
void Hu3DModelCameraSet(HU3DMODELID modelId, u16 cameraBit);
void Hu3DModelLayerSet(HU3DMODELID modelId, s16 layerNo);
void Hu3DModelShadowMapSet(HU3DMODELID modelId);
void Hu3DMotionSet(HU3DMODELID modelId, HU3DMOTID motId);
void Hu3DMotionSpeedSet(HU3DMODELID modelId, f32 speed);
void Hu3DMotionTimeSet(HU3DMODELID modelId, f32 time);
f32 Hu3DMotionTimeGet(HU3DMODELID modelId);
f32 Hu3DMotionMaxTimeGet(HU3DMODELID modelId);
void Hu3DMtxRotGet(Mtx src, f32 *dst);
s32 rand8(void);
double sin(double x);
double cos(double x);

void fn_1_7104(void);
void fn_1_773C(void);
void fn_1_7C88(void);
void fn_1_856C(void);
void fn_1_8F78(void);

M548State *lbl_1_bss_5F8;

M548Vec3 lbl_1_data_530[12] = {
    { 0.0f, 0.0f, 0.0f },
    { 913.0f, 0.0f, -1177.0f },
    { 993.0f, 0.0f, -1038.0f },
    { 550.0f, 1000.0f, -3250.0f },
    { 593.0f, 312.0f, -1119.0f },
    { -462.0f, 330.0f, -1100.0f },
    { -1000.0f, 40.0f, -2100.0f },
    { -2500.0f, 0.0f, -3900.0f },
    { -1000.0f, 0.0f, -2100.0f },
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f },
};
void (*lbl_1_data_5C0[5])(void) = { fn_1_7104, fn_1_773C, fn_1_7C88, fn_1_856C, fn_1_8F78 };

void fn_1_6104(M548State *stage, M548StageModel *models, s32 *dataNums)
{
    s16 j;
    s16 i;
    s16 grp;

    lbl_1_bss_5F8 = stage;
    memset(lbl_1_bss_5F8, 0, sizeof(M548State));
    lbl_1_bss_5F8->models = models;
    lbl_1_bss_5F8->dataNums = dataNums;
    i = 0;
    do {
        for (j = 0; j < 4; j++) {
            if (i == (grp = GwPlayerConf[j].grp)) {
                lbl_1_bss_5F8->chars[i].playerNo = j;
                i++;
                if (i >= 2) {
                    break;
                }
            }
        }
    } while (i < 2);
    for (j = 0; j < 2; j++) {
        lbl_1_bss_5F8->chars[j].charNo = GwPlayerConf[lbl_1_bss_5F8->chars[j].playerNo].charNo;
        lbl_1_bss_5F8->chars[j].padNo = GwPlayerConf[lbl_1_bss_5F8->chars[j].playerNo].padNo;
        grp = GwPlayerConf[lbl_1_bss_5F8->chars[j].playerNo].type;
        if (grp != 0) {
            lbl_1_bss_5F8->chars[j].cpuLevel = GwPlayerConf[lbl_1_bss_5F8->chars[j].playerNo].dif + 1;
        }
    }
}

void fn_1_6350(s16 arg0)
{
    s16 i;
    HU3DMODELID model;
    s16 idx;

    lbl_1_bss_5F8->field9C = arg0;
    for (i = 0; i < 12; i++) {
        idx = (s16)(i + 0);
        model = lbl_1_bss_5F8->models[idx].id = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_bss_5F8->dataNums[idx], 0x10000000, 2));
        Hu3DModelAttrSet(model, 1);
        Hu3DModelCameraSet(model, 1);
        Hu3DModelPosSetV(model, (f32 *)&lbl_1_data_530[i]);
        Hu3DModelRotSet(model, 0.0f, 0.0f, 0.0f);
        Hu3DMotionSpeedSet(model, 0.0f);
        Hu3DMotionTimeSet(model, 0.0f);
        Hu3DModelAttrSet(model, 0x40000001);
        lbl_1_bss_5F8->models[idx].x = lbl_1_data_530[i].x;
        lbl_1_bss_5F8->models[idx].y = lbl_1_data_530[i].y;
        lbl_1_bss_5F8->models[idx].z = lbl_1_data_530[i].z;
        lbl_1_bss_5F8->models[idx].rotX = 0.0f;
        lbl_1_bss_5F8->models[idx].rotY = 0.0f;
        lbl_1_bss_5F8->models[idx].rotZ = 0.0f;
        Hu3DModelAttrReset(model, 1);
        if (i == 0) {
            Hu3DModelShadowMapSet(model);
        }
    }
}

void fn_1_65E8(u32 *arg0, u32 *arg1)
{
    lbl_1_bss_5F8->motionData[0] = arg0;
    lbl_1_bss_5F8->motionData[1] = arg1;
}

void fn_1_660C(s16 *arg0, s16 *arg1)
{
    lbl_1_bss_5F8->motionIds[0] = arg0;
    lbl_1_bss_5F8->motionIds[1] = arg1;
}

void fn_1_6630(void)
{
    s32 i;

    for (i = 0; i < 2; i++) {
        lbl_1_bss_5F8->modelIds[i][168] = CharModelCreate(lbl_1_bss_5F8->chars[i].charNo, 1);
        Hu3DModelPosSet(lbl_1_bss_5F8->modelIds[i][168], (i * 300) - 150, 100.0f, 0.0f);
        Hu3DModelRotSet(lbl_1_bss_5F8->modelIds[i][168], 0.0f, 90.0f + 180.0f * i, 0.0f);
        if (i != 0) {
            Hu3DModelAttrSet(lbl_1_bss_5F8->modelIds[i][168], 0x40000001);
            Hu3DMotionSpeedSet(lbl_1_bss_5F8->modelIds[i][168], 1.0f);
            Hu3DMotionTimeSet(lbl_1_bss_5F8->modelIds[i][168], 0.0f);
        } else {
            Hu3DModelAttrReset(lbl_1_bss_5F8->modelIds[i][168], 0x40000001);
            Hu3DMotionSpeedSet(lbl_1_bss_5F8->modelIds[i][168], 0.0f);
            Hu3DMotionTimeSet(lbl_1_bss_5F8->modelIds[i][168], 25.0f);
        }
    }
}

void fn_1_6890(void)
{
    s32 i;
    s32 absX;
    s32 newX;
    s32 absY;
    s32 newY;

    for (i = 0; i < 2; i++) {
        if (lbl_1_bss_5F8->chars[i].cpuLevel != 0) {
            memset(&lbl_1_bss_5F8->chars[i].btn, 0, 0x14);
        } else {
            lbl_1_bss_5F8->chars[i].btn = HuPadBtn[lbl_1_bss_5F8->chars[i].padNo];
            lbl_1_bss_5F8->chars[i].btnDown = HuPadBtnDown[lbl_1_bss_5F8->chars[i].padNo];
            lbl_1_bss_5F8->chars[i].btnRep = HuPadBtnRep[lbl_1_bss_5F8->chars[i].padNo];
            lbl_1_bss_5F8->chars[i].stkX = (s8)HuPadStkX[lbl_1_bss_5F8->chars[i].padNo];
            if (lbl_1_bss_5F8->chars[i].stkX >= 0) {
                absX = lbl_1_bss_5F8->chars[i].stkX;
            } else {
                absX = -lbl_1_bss_5F8->chars[i].stkX;
            }
            if (absX < 10) {
                newX = 0;
            } else {
                newX = lbl_1_bss_5F8->chars[i].stkX;
            }
            lbl_1_bss_5F8->chars[i].stkX = newX;
            lbl_1_bss_5F8->chars[i].stkY = (s8)HuPadStkY[lbl_1_bss_5F8->chars[i].padNo];
            if (lbl_1_bss_5F8->chars[i].stkY >= 0) {
                absY = lbl_1_bss_5F8->chars[i].stkY;
            } else {
                absY = -lbl_1_bss_5F8->chars[i].stkY;
            }
            if (absY < 10) {
                newY = 0;
            } else {
                newY = lbl_1_bss_5F8->chars[i].stkY;
            }
            lbl_1_bss_5F8->chars[i].stkY = newY;
            lbl_1_bss_5F8->chars[i].subStkX = (s8)HuPadSubStkX[lbl_1_bss_5F8->chars[i].padNo];
            lbl_1_bss_5F8->chars[i].subStkY = (s8)HuPadSubStkY[lbl_1_bss_5F8->chars[i].padNo];
            lbl_1_bss_5F8->chars[i].trigL = HuPadTrigL[lbl_1_bss_5F8->chars[i].padNo];
            lbl_1_bss_5F8->chars[i].trigR = HuPadTrigR[lbl_1_bss_5F8->chars[i].padNo];
            lbl_1_bss_5F8->chars[i].dstk = HuPadDStk[lbl_1_bss_5F8->chars[i].padNo];
            lbl_1_bss_5F8->chars[i].dstkRep = HuPadDStkRep[lbl_1_bss_5F8->chars[i].padNo];
        }
    }
}

void fn_1_6D4C(void)
{
    s16 i;
    s16 j;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 5; j++) {
            lbl_1_bss_5F8->motionIds[i][j] =
                CharMotionCreate(lbl_1_bss_5F8->chars[i].charNo, lbl_1_bss_5F8->motionData[i][j]);
        }
    }
    for (i = 0; i < 2; i++) {
        lbl_1_bss_5F8->slots[i].id = -1;
        lbl_1_bss_5F8->slots[i].speed = 1.0f;
        lbl_1_bss_5F8->slots[i].curSpeed = 1.0f;
    }
}

void fn_1_6EA0(void)
{
    s16 i;
    s16 idx;
    M548StageSlot *slot;

    for (i = 0; i < 2; i++) {
        slot = &lbl_1_bss_5F8->slots[i];
        idx = i + 12;
        if (slot->motion != slot->id) {
            Hu3DMotionSet(lbl_1_bss_5F8->models[idx].id, lbl_1_bss_5F8->motionIds[i][slot->motion]);
            slot->id = slot->motion;
            slot->maxTime = Hu3DMotionMaxTimeGet(lbl_1_bss_5F8->models[idx].id);
        }
        if (slot->speed != slot->curSpeed) {
            Hu3DMotionSpeedSet(lbl_1_bss_5F8->models[idx].id, slot->speed);
            slot->curSpeed = slot->speed;
        }
        if (slot->attrSet != slot->attrCur) {
            Hu3DModelAttrSet(lbl_1_bss_5F8->models[idx].id, slot->attrSet);
            slot->attrCur = slot->attrSet;
        }
        if (slot->attrReset != 0) {
            Hu3DModelAttrReset(lbl_1_bss_5F8->models[idx].id, slot->attrReset);
            slot->attrCur = slot->attrSet = slot->attrReset = 0;
        }
        if (slot->time != slot->curTime) {
            Hu3DMotionTimeSet(lbl_1_bss_5F8->models[idx].id, slot->time);
        }
        slot->time = Hu3DMotionTimeGet(lbl_1_bss_5F8->models[idx].id);
        slot->curTime = slot->time;
    }
}

void fn_1_70B0(void)
{
    s32 i;

    for (i = 0; i < 5; i++) {
        lbl_1_data_5C0[i]();
    }
}

void fn_1_7104(void)
{
    M548StageModel *model;

    model = &lbl_1_bss_5F8->models[3];
    if (lbl_1_bss_5F8->anim[0].hold == 0) {
        switch (lbl_1_bss_5F8->anim[0].state) {
        case 0:
            lbl_1_bss_5F8->anim[0].timer = rand8() % 120 + 180;
            lbl_1_bss_5F8->anim[0].state = 1;
            lbl_1_bss_5F8->anim[0].f24 = 0.4f;
            Hu3DMotionSpeedSet(model->id, lbl_1_bss_5F8->anim[0].f24);
            model->rotY = -60.0f;
            break;
        case 1:
            if (lbl_1_bss_5F8->anim[0].timer-- < 0) {
                lbl_1_bss_5F8->anim[0].state++;
                lbl_1_bss_5F8->anim[0].timer = 0;
                lbl_1_bss_5F8->anim[0].f20 = model->rotY;
                lbl_1_bss_5F8->anim[0].timer2 = 120;
                lbl_1_bss_5F8->anim[0].f24 = rand8() % 10 + 30;
                if (lbl_1_bss_5F8->anim[0].f20 > -45.0f) {
                    lbl_1_bss_5F8->anim[0].f24 = -lbl_1_bss_5F8->anim[0].f24;
                }
            }
            break;
        case 2:
            lbl_1_bss_5F8->anim[0].timer++;
            Hu3DMotionSpeedSet(model->id, 0.4 + 0.5 * sin(3.141592653589793 * (lbl_1_bss_5F8->anim[0].timer * 90 / 40) / 180.0));
            if (lbl_1_bss_5F8->anim[0].timer == 40) {
                lbl_1_bss_5F8->anim[0].timer = 0;
                lbl_1_bss_5F8->anim[0].state++;
            }
            break;
        case 3:
            lbl_1_bss_5F8->anim[0].timer++;
            Hu3DMotionSpeedSet(model->id, 0.8 + 0.2 * sin(3.141592653589793 * (lbl_1_bss_5F8->anim[0].timer * 180 / lbl_1_bss_5F8->anim[0].timer2) / 180.0));
            model->rotY = lbl_1_bss_5F8->anim[0].f20 + lbl_1_bss_5F8->anim[0].f24 * sin(3.141592653589793 * (lbl_1_bss_5F8->anim[0].timer * 90 / lbl_1_bss_5F8->anim[0].timer2) / 180.0);
            if (lbl_1_bss_5F8->anim[0].timer == lbl_1_bss_5F8->anim[0].timer2) {
                model->rotY = lbl_1_bss_5F8->anim[0].f20 + lbl_1_bss_5F8->anim[0].f24;
                lbl_1_bss_5F8->anim[0].state++;
                lbl_1_bss_5F8->anim[0].timer = 0;
            }
            break;
        case 4:
            lbl_1_bss_5F8->anim[0].timer++;
            Hu3DMotionSpeedSet(model->id, 0.8 - 0.4 * sin(3.141592653589793 * (lbl_1_bss_5F8->anim[0].timer * 90 / 40) / 180.0));
            if (lbl_1_bss_5F8->anim[0].timer == 40) {
                lbl_1_bss_5F8->anim[0].timer = rand8() % 120 + 180;
                lbl_1_bss_5F8->anim[0].state = 1;
            }
            break;
        }
    }
}

void fn_1_773C(void)
{
    M548StageModel *model1;
    M548StageModel *model2;

    model1 = &lbl_1_bss_5F8->models[1];
    model2 = &lbl_1_bss_5F8->models[2];
    if (lbl_1_bss_5F8->anim[1].hold == 0) {
        model1->rotY = 30.0f;
        model2->rotY = 30.0f;
        switch (lbl_1_bss_5F8->anim[1].state) {
        case 0:
            if (lbl_1_bss_5F8->anim[0].state == 2) {
                lbl_1_bss_5F8->anim[1].timer = 0;
                lbl_1_bss_5F8->anim[1].state++;
                lbl_1_bss_5F8->anim[1].f20 = 20.0f;
                lbl_1_bss_5F8->anim[1].f24 = 5.0f;
                lbl_1_bss_5F8->anim[1].f28 = -5.0f;
            }
            break;
        case 1:
            lbl_1_bss_5F8->anim[1].timer++;
            model1->rotY = 30.0 + lbl_1_bss_5F8->anim[1].f20 * sin(3.141592653589793 * ((f32)lbl_1_bss_5F8->anim[1].timer * lbl_1_bss_5F8->anim[1].f24) / 180.0);
            model2->rotY = 30.0 + lbl_1_bss_5F8->anim[1].f20 * sin(3.141592653589793 * ((f32)lbl_1_bss_5F8->anim[1].timer * lbl_1_bss_5F8->anim[1].f28) / 180.0);
            if (lbl_1_bss_5F8->anim[0].state == 4) {
                lbl_1_bss_5F8->anim[1].state++;
                lbl_1_bss_5F8->anim[1].timer2 = 0;
            }
            break;
        case 2:
            lbl_1_bss_5F8->anim[1].timer++;
            lbl_1_bss_5F8->anim[1].timer2++;
            if (lbl_1_bss_5F8->anim[1].timer2 > 40) {
                lbl_1_bss_5F8->anim[1].timer2 = 40;
            }
            lbl_1_bss_5F8->anim[1].f20 = 20.0 - 20.0 * sin(3.141592653589793 * (lbl_1_bss_5F8->anim[1].timer2 * 90 / 40) / 180.0);
            model1->rotY = 30.0 + lbl_1_bss_5F8->anim[1].f20 * sin(3.141592653589793 * ((f32)lbl_1_bss_5F8->anim[1].timer * lbl_1_bss_5F8->anim[1].f24) / 180.0);
            model2->rotY = 30.0 + lbl_1_bss_5F8->anim[1].f20 * sin(3.141592653589793 * ((f32)lbl_1_bss_5F8->anim[1].timer * lbl_1_bss_5F8->anim[1].f28) / 180.0);
            if (lbl_1_bss_5F8->anim[0].state == 1) {
                lbl_1_bss_5F8->anim[1].state = 0;
            }
            break;
        }
    }
}

void fn_1_7C88(void)
{
    Mtx concat;
    Mtx rot;
    M548StageModel *model1;
    M548StageModel *model2;

    model1 = &lbl_1_bss_5F8->models[4];
    model2 = &lbl_1_bss_5F8->models[5];
    if (lbl_1_bss_5F8->anim[2].hold == 0) {
        PSMTXIdentity(concat);
        PSMTXRotRad(rot, 'y', -0.7853982f);
        PSMTXConcat(concat, rot, concat);
        model1->rotY = 30.0f;
        switch (lbl_1_bss_5F8->anim[2].state) {
        case 0:
            model2->rotY = -45.0f;
            if (lbl_1_bss_5F8->anim[0].state == 2) {
                lbl_1_bss_5F8->anim[2].f20 = 10.0f;
                lbl_1_bss_5F8->anim[2].f24 = 5.0f;
                lbl_1_bss_5F8->anim[2].timer = 0;
                lbl_1_bss_5F8->anim[2].state++;
            }
            break;
        case 1:
            lbl_1_bss_5F8->anim[2].timer++;
            lbl_1_bss_5F8->anim[2].f24 = 5.0 + 5.0 * sin(3.141592653589793 * (lbl_1_bss_5F8->anim[2].timer * 90 / 40) / 180.0);
            model1->rotX = lbl_1_bss_5F8->anim[2].f20 * sin(3.141592653589793 * ((f32)lbl_1_bss_5F8->anim[2].timer * lbl_1_bss_5F8->anim[2].f24) / 180.0);
            PSMTXRotRad(rot, 'z', 0.01745329238474369 * (lbl_1_bss_5F8->anim[2].f20 * sin(3.141592653589793 * ((f32)lbl_1_bss_5F8->anim[2].timer * lbl_1_bss_5F8->anim[2].f24) / 180.0)));
            PSMTXConcat(concat, rot, concat);
            Hu3DMtxRotGet(concat, &model2->rotX);
            if (lbl_1_bss_5F8->anim[0].state == 3) {
                lbl_1_bss_5F8->anim[2].f24 = 10.0f;
                lbl_1_bss_5F8->anim[2].state++;
            }
            break;
        case 2:
            lbl_1_bss_5F8->anim[2].timer++;
            model1->rotX = lbl_1_bss_5F8->anim[2].f20 * sin(3.141592653589793 * ((f32)lbl_1_bss_5F8->anim[2].timer * lbl_1_bss_5F8->anim[2].f24) / 180.0);
            PSMTXRotRad(rot, 'z', 0.01745329238474369 * (lbl_1_bss_5F8->anim[2].f20 * sin(3.141592653589793 * ((f32)lbl_1_bss_5F8->anim[2].timer * lbl_1_bss_5F8->anim[2].f24) / 180.0)));
            PSMTXConcat(concat, rot, concat);
            Hu3DMtxRotGet(concat, &model2->rotX);
            if (lbl_1_bss_5F8->anim[0].state == 4) {
                lbl_1_bss_5F8->anim[2].state++;
                lbl_1_bss_5F8->anim[2].timer2 = 0;
            }
            break;
        case 3:
            lbl_1_bss_5F8->anim[2].timer2++;
            lbl_1_bss_5F8->anim[2].timer++;
            if (lbl_1_bss_5F8->anim[2].timer2 > 40) {
                lbl_1_bss_5F8->anim[2].timer2 = 40;
            }
            lbl_1_bss_5F8->anim[2].f20 = 10.0 - 5.0 * sin(3.141592653589793 * (lbl_1_bss_5F8->anim[2].timer2 * 90 / 40) / 180.0);
            model1->rotX = lbl_1_bss_5F8->anim[2].f20 * sin(3.141592653589793 * ((f32)lbl_1_bss_5F8->anim[2].timer * lbl_1_bss_5F8->anim[2].f24) / 180.0);
            PSMTXRotRad(rot, 'z', 0.01745329238474369 * (lbl_1_bss_5F8->anim[2].f20 * sin(3.141592653589793 * ((f32)lbl_1_bss_5F8->anim[2].timer * lbl_1_bss_5F8->anim[2].f24) / 180.0)));
            PSMTXConcat(concat, rot, concat);
            Hu3DMtxRotGet(concat, &model2->rotX);
            for (;;) {
                if (model1->rotX >= 360.0f) {
                    model1->rotX -= 360.0f;
                    continue;
                }
                break;
            }
            if (lbl_1_bss_5F8->anim[0].state == 1 && (model1->rotX >= 359.0f || model1->rotX <= 1.0f)) {
                lbl_1_bss_5F8->anim[2].f20 = 0.0f;
                model1->rotX = 0.0f;
                model2->rotX = 0.0f;
                model2->rotY = -45.0f;
                model2->rotZ = 0.0f;
                lbl_1_bss_5F8->anim[2].state = 0;
            }
            break;
        }
        for (;;) {
            if (model1->rotX >= 360.0f) {
                model1->rotX -= 360.0f;
                continue;
            }
            break;
        }
    }
}

void fn_1_856C(void)
{
    M548StageModel *model1;
    M548StageModel *model2;

    model1 = &lbl_1_bss_5F8->models[6];
    model2 = &lbl_1_bss_5F8->models[8];
    switch (lbl_1_bss_5F8->anim[3].state) {
    case 0:
        if (lbl_1_bss_5F8->anim[0].state == 2) {
            lbl_1_bss_5F8->anim[3].timer = 0;
            if (lbl_1_bss_5F8->anim[3].f20 == 0.0f) {
                lbl_1_bss_5F8->anim[3].state++;
                lbl_1_bss_5F8->anim[3].timer = 0;
                lbl_1_bss_5F8->anim[3].f20 = 15.0f;
                lbl_1_bss_5F8->anim[3].f28 = (rand8() % 16) * 25 - 650;
                model1->rotZ = 0.0f;
                model1->x = -1000.0f;
            } else {
                lbl_1_bss_5F8->anim[3].state = 4;
                lbl_1_bss_5F8->anim[3].f20 = 10.0f;
                lbl_1_bss_5F8->anim[3].f28 = (rand8() % 16) * 25 - 650;
                model1->rotZ = 0.0f;
                model1->x = -1000.0f;
            }
            lbl_1_bss_5F8->anim[3].f24 = 250.0f * lbl_1_bss_5F8->anim[3].f20 / 360.0f;
        }
        break;
    case 1:
        model1->rotZ -= lbl_1_bss_5F8->anim[3].f20;
        model1->x += lbl_1_bss_5F8->anim[3].f24;
        if (model1->x >= lbl_1_bss_5F8->anim[3].f28) {
            lbl_1_bss_5F8->anim[3].state++;
        }
        break;
    case 2:
        lbl_1_bss_5F8->anim[3].timer++;
        lbl_1_bss_5F8->anim[3].f20 = 7.5 + 7.5 * cos(3.141592653589793 * lbl_1_bss_5F8->anim[3].timer / 180.0);
        lbl_1_bss_5F8->anim[3].f24 = 250.0f * lbl_1_bss_5F8->anim[3].f20 / 360.0f;
        model1->rotZ -= lbl_1_bss_5F8->anim[3].f20;
        model1->x += lbl_1_bss_5F8->anim[3].f24;
        if (lbl_1_bss_5F8->anim[3].timer >= 90) {
            lbl_1_bss_5F8->anim[3].state++;
        }
        break;
    case 3:
        lbl_1_bss_5F8->anim[3].timer++;
        if (lbl_1_bss_5F8->anim[3].timer >= 180) {
            lbl_1_bss_5F8->anim[3].timer = 180;
        }
        lbl_1_bss_5F8->anim[3].f20 = 7.5 - 7.5 * cos(3.141592653589793 * lbl_1_bss_5F8->anim[3].timer / 180.0);
        lbl_1_bss_5F8->anim[3].f24 = 250.0f * lbl_1_bss_5F8->anim[3].f20 / 360.0f;
        if (model1->x < 1000.0f) {
            model1->rotZ -= lbl_1_bss_5F8->anim[3].f20;
            model1->x += lbl_1_bss_5F8->anim[3].f24;
        } else if (lbl_1_bss_5F8->anim[0].state == 1) {
            lbl_1_bss_5F8->anim[3].state = 0;
        }
        break;
    case 4:
        lbl_1_bss_5F8->anim[3].timer++;
        if (lbl_1_bss_5F8->anim[3].timer >= 20) {
            model1->rotZ -= lbl_1_bss_5F8->anim[3].f20;
            model1->x += lbl_1_bss_5F8->anim[3].f24;
            if (model1->x >= lbl_1_bss_5F8->anim[3].f28) {
                lbl_1_bss_5F8->anim[3].state++;
                lbl_1_bss_5F8->anim[3].timer = 0;
            }
        }
        break;
    case 5:
        lbl_1_bss_5F8->anim[3].timer++;
        lbl_1_bss_5F8->anim[3].f20 = 10.0 * cos(3.141592653589793 * lbl_1_bss_5F8->anim[3].timer / 180.0);
        lbl_1_bss_5F8->anim[3].f24 = 250.0f * lbl_1_bss_5F8->anim[3].f20 / 360.0f;
        model1->rotZ -= lbl_1_bss_5F8->anim[3].f20;
        model1->x += lbl_1_bss_5F8->anim[3].f24;
        if (lbl_1_bss_5F8->anim[3].timer == 90) {
            lbl_1_bss_5F8->anim[3].state++;
            lbl_1_bss_5F8->anim[3].timer = 0;
        }
        break;
    case 6:
        if (lbl_1_bss_5F8->anim[0].state == 2) {
            lbl_1_bss_5F8->anim[3].state++;
        }
        break;
    case 7:
        lbl_1_bss_5F8->anim[3].timer++;
        if (lbl_1_bss_5F8->anim[3].timer >= 150) {
            lbl_1_bss_5F8->anim[3].timer = 150;
        }
        if (lbl_1_bss_5F8->anim[3].timer >= 60) {
            lbl_1_bss_5F8->anim[3].f20 = 10.0 * sin(3.141592653589793 * (lbl_1_bss_5F8->anim[3].timer - 60) / 180.0);
            lbl_1_bss_5F8->anim[3].f24 = 250.0f * lbl_1_bss_5F8->anim[3].f20 / 360.0f;
            if (model1->x < 1000.0f) {
                if (lbl_1_bss_5F8->anim[3].timer >= 70) {
                    model1->rotZ -= lbl_1_bss_5F8->anim[3].f20;
                }
                model1->x += lbl_1_bss_5F8->anim[3].f24;
            } else if (lbl_1_bss_5F8->anim[0].state == 1) {
                lbl_1_bss_5F8->anim[3].state = 0;
                lbl_1_bss_5F8->anim[3].f20 = 0.0f;
            }
        }
        break;
    }
    model2->x = model1->x;
    model2->y = model1->y - 40.0f;
    model2->z = model1->z;
}

void fn_1_8F78(void)
{
    M548StageModel *model;

    model = &lbl_1_bss_5F8->models[7];
    switch (lbl_1_bss_5F8->anim[4].state) {
    case 0:
        if (lbl_1_bss_5F8->anim[0].state == 2) {
            lbl_1_bss_5F8->anim[4].state++;
            lbl_1_bss_5F8->anim[4].timer = 0;
            Hu3DMotionSpeedSet(model->id, 0.5f);
            Hu3DMotionTimeSet(model->id, 0.0f);
        }
        break;
    case 1:
        lbl_1_bss_5F8->anim[4].timer++;
        model->x = (lbl_1_bss_5F8->anim[4].timer * 4000) / 200 - 2000;
        if (lbl_1_bss_5F8->anim[4].timer == 200) {
            lbl_1_bss_5F8->anim[4].state++;
        }
        break;
    case 2:
        if (lbl_1_bss_5F8->anim[0].state == 1) {
            lbl_1_bss_5F8->anim[4].state = 0;
        }
        break;
    }
}
