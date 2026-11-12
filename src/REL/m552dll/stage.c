#include "types.h"
#include "string.h"
#include "game/gamework.h"
#include "game/pad.h"
#include "game/charman.h"
#include "game/data.h"
#include "REL/m552dll.h"

f32 Hu3DMotionMaxTimeGet(HU3DMODELID modelId);

/* unit-owned .bss */
M552State *lbl_1_bss_4A28;

/* unit-owned .data */
HuVecF lbl_1_data_568[6] = {
    { 0.0f, -100.0f, 0.0f },
    { 0.0f, -100.0f, 0.0f },
    { 0.0f, -100.0f, 0.0f },
    { 0.0f, -100.0f, 0.0f },
    { 0.0f, -100.0f, 0.0f },
    { 0.0f, -100.0f, 0.0f },
};

void fn_1_B33C(M552State *state, M552Mdl *models, s32 *dataNums)
{
    s16 j;
    s16 i;
    s16 grp;

    lbl_1_bss_4A28 = state;
#ifdef TARGET_PC
    memset(lbl_1_bss_4A28, 0, sizeof(M552State));
#else
    memset(lbl_1_bss_4A28, 0, 0x170);
#endif
    lbl_1_bss_4A28->models = models;
    lbl_1_bss_4A28->dataNums = dataNums;
    i = 0;
    do {
        for (j = 0; j < 4; j++) {
            if (i == (grp = GwPlayerConf[j].grp)) {
                lbl_1_bss_4A28->chars[i].playerNo = j;
                i++;
                if (i >= 2) {
                    break;
                }
            }
        }
    } while (i < 2);
    for (j = 0; j < 2; j++) {
        lbl_1_bss_4A28->chars[j].charNo = GwPlayerConf[lbl_1_bss_4A28->chars[j].playerNo].charNo;
        lbl_1_bss_4A28->chars[j].padNo = GwPlayerConf[lbl_1_bss_4A28->chars[j].playerNo].padNo;
        grp = GwPlayerConf[lbl_1_bss_4A28->chars[j].playerNo].type;
        if (grp != 0) {
            lbl_1_bss_4A28->chars[j].cpuLevel = GwPlayerConf[lbl_1_bss_4A28->chars[j].playerNo].dif + 1;
        }
    }
}

void fn_1_B588(s16 arg0)
{
    s16 i;
    HU3DMODELID model;
    s16 idx;

    lbl_1_bss_4A28->stage = arg0;
    for (i = 0; i < 6; i++) {
        idx = i + 2;
        model = lbl_1_bss_4A28->models[idx].id = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_bss_4A28->dataNums[i], 0x10000000, 2));
        Hu3DModelAttrSet(model, 1);
        Hu3DModelCameraSet(model, 1);
        Hu3DModelPosSetV(model, &lbl_1_data_568[i]);
        Hu3DModelRotSet(model, 0.0f, 0.0f, 0.0f);
        Hu3DMotionSpeedSet(model, 1.0f);
        Hu3DMotionTimeSet(model, 0.0f);
        Hu3DModelAttrSet(model, 0x40000001);
        lbl_1_bss_4A28->models[idx].pos.x = lbl_1_data_568[i].x;
        lbl_1_bss_4A28->models[idx].pos.y = lbl_1_data_568[i].y;
        lbl_1_bss_4A28->models[idx].pos.z = lbl_1_data_568[i].z;
        lbl_1_bss_4A28->models[idx].rot.x = 0.0f;
        lbl_1_bss_4A28->models[idx].rot.y = 0.0f;
        lbl_1_bss_4A28->models[idx].rot.z = 0.0f;
        Hu3DModelAttrReset(model, 1);
    }
}

void fn_1_B810(s32 *arg0, s32 *arg1)
{
    lbl_1_bss_4A28->motionData[0] = arg0;
    lbl_1_bss_4A28->motionData[1] = arg1;
}

void fn_1_B834(s16 *arg0, s16 *arg1)
{
    lbl_1_bss_4A28->motionIds[0] = arg0;
    lbl_1_bss_4A28->motionIds[1] = arg1;
}

void fn_1_B858(void)
{
    s32 i;

    for (i = 0; i < 2; i++) {
        lbl_1_bss_4A28->models[i].id = CharModelCreate(lbl_1_bss_4A28->chars[i].charNo, 1);
        Hu3DModelPosSet(lbl_1_bss_4A28->models[i].id, i * 500 - 250, 0.0f, 0.0f);
        lbl_1_bss_4A28->models[i].pos.x = i * 700 - 350;
        lbl_1_bss_4A28->models[i].pos.y = 0.0f;
        lbl_1_bss_4A28->models[i].pos.z = 500.0f;
        lbl_1_bss_4A28->models[i].rot.x = 0.0f;
        lbl_1_bss_4A28->models[i].rot.y = i * 30 + 165;
        lbl_1_bss_4A28->models[i].rot.z = 0.0f;
        Hu3DModelAttrSet(lbl_1_bss_4A28->models[i].id, 0x40000001);
        Hu3DModelCameraSet(lbl_1_bss_4A28->models[i].id, 1);
        Hu3DModelLayerSet(lbl_1_bss_4A28->models[i].id, 0);
        Hu3DModelAttrReset(lbl_1_bss_4A28->models[i].id, 1);
    }
}

void fn_1_BADC(void)
{
    s32 i;
    s8 absX;
    s8 newX;
    s8 absY;
    s8 newY;

    for (i = 0; i < 2; i++) {
        if (lbl_1_bss_4A28->chars[i].cpuLevel != 0) {
            memset(&lbl_1_bss_4A28->chars[i].btn, 0, 0x14);
        } else {
            lbl_1_bss_4A28->chars[i].btn = HuPadBtn[lbl_1_bss_4A28->chars[i].padNo];
            lbl_1_bss_4A28->chars[i].btnDown = HuPadBtnDown[lbl_1_bss_4A28->chars[i].padNo];
            lbl_1_bss_4A28->chars[i].btnRep = HuPadBtnRep[lbl_1_bss_4A28->chars[i].padNo];
            lbl_1_bss_4A28->chars[i].stkX = HuPadStkX[lbl_1_bss_4A28->chars[i].padNo];
            if (lbl_1_bss_4A28->chars[i].stkX >= 0) {
                absX = lbl_1_bss_4A28->chars[i].stkX;
            } else {
                absX = -lbl_1_bss_4A28->chars[i].stkX;
            }
            if (absX < 10) {
                newX = 0;
            } else {
                newX = lbl_1_bss_4A28->chars[i].stkX;
            }
            lbl_1_bss_4A28->chars[i].stkX = newX;
            lbl_1_bss_4A28->chars[i].stkY = HuPadStkY[lbl_1_bss_4A28->chars[i].padNo];
            if (lbl_1_bss_4A28->chars[i].stkY >= 0) {
                absY = lbl_1_bss_4A28->chars[i].stkY;
            } else {
                absY = -lbl_1_bss_4A28->chars[i].stkY;
            }
            if (absY < 10) {
                newY = 0;
            } else {
                newY = lbl_1_bss_4A28->chars[i].stkY;
            }
            lbl_1_bss_4A28->chars[i].stkY = newY;
            lbl_1_bss_4A28->chars[i].subStkX = HuPadSubStkX[lbl_1_bss_4A28->chars[i].padNo];
            lbl_1_bss_4A28->chars[i].subStkY = HuPadSubStkY[lbl_1_bss_4A28->chars[i].padNo];
            lbl_1_bss_4A28->chars[i].trigL = HuPadTrigL[lbl_1_bss_4A28->chars[i].padNo];
            lbl_1_bss_4A28->chars[i].trigR = HuPadTrigR[lbl_1_bss_4A28->chars[i].padNo];
            lbl_1_bss_4A28->chars[i].dstk = HuPadDStk[lbl_1_bss_4A28->chars[i].padNo];
            lbl_1_bss_4A28->chars[i].dstkRep = HuPadDStkRep[lbl_1_bss_4A28->chars[i].padNo];
        }
    }
}

void fn_1_BF98(void)
{
    s16 i;
    s16 j;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            lbl_1_bss_4A28->motionIds[i][j] =
                CharMotionCreate(lbl_1_bss_4A28->chars[i].charNo, lbl_1_bss_4A28->motionData[i][j]);
        }
    }
    for (i = 0; i < 2; i++) {
        lbl_1_bss_4A28->slots[i].motion = 0;
        lbl_1_bss_4A28->slots[i].curMotion = -1;
        lbl_1_bss_4A28->slots[i].speed = 0.0f;
        lbl_1_bss_4A28->slots[i].curSpeed = 0.0f;
        lbl_1_bss_4A28->slots[i].attrSet = 0;
        lbl_1_bss_4A28->slots[i].attrCur = 0;
    }
}

void fn_1_C14C(void)
{
    s16 i;
    s16 idx;
    M552Slot *slot;

    for (i = 0; i < 2; i++) {
        slot = &lbl_1_bss_4A28->slots[i];
        idx = i;
        if (slot->motion != slot->curMotion) {
            Hu3DMotionSet(lbl_1_bss_4A28->models[idx].id, lbl_1_bss_4A28->motionIds[i][slot->motion]);
            slot->curMotion = slot->motion;
            slot->maxTime = Hu3DMotionMaxTimeGet(lbl_1_bss_4A28->models[idx].id);
        }
        if (slot->speed != slot->curSpeed) {
            Hu3DMotionSpeedSet(lbl_1_bss_4A28->models[idx].id, slot->speed);
            slot->curSpeed = slot->speed;
        }
        if (slot->attrSet != slot->attrCur) {
            Hu3DModelAttrSet(lbl_1_bss_4A28->models[idx].id, slot->attrSet);
            slot->attrCur = slot->attrSet;
        }
        if (slot->attrReset != 0) {
            Hu3DModelAttrReset(lbl_1_bss_4A28->models[idx].id, slot->attrReset);
            slot->attrReset = ~slot->attrReset;
            slot->attrCur = slot->attrSet = slot->attrSet & slot->attrReset;
            slot->attrReset = 0;
        }
        if (slot->time != slot->curTime) {
            Hu3DMotionTimeSet(lbl_1_bss_4A28->models[idx].id, slot->time);
        }
        switch (slot->shiftState) {
        case 0:
            break;
        case 1:
            Hu3DMotionShiftSet(lbl_1_bss_4A28->models[idx].id, lbl_1_bss_4A28->motionIds[i][slot->shiftMotion],
                               slot->shiftStart, slot->shiftEnd, slot->shiftAttr);
            slot->shiftMax = Hu3DMotionShiftMaxTimeGet(lbl_1_bss_4A28->models[idx].id);
            slot->shiftState = 2;
            break;
        case 2:
            slot->shiftCur = Hu3DMotionShiftTimeGet(lbl_1_bss_4A28->models[idx].id);
            if ((slot->shiftAttr & 0x40000001) == 0) {
                if (slot->shiftCur >= slot->shiftEnd - 1.0f) {
                    slot->shiftState = 3;
                }
            }
            break;
        case 3:
            slot->shiftState = 0;
            break;
        }
        slot->time = Hu3DMotionTimeGet(lbl_1_bss_4A28->models[idx].id);
        slot->curTime = slot->time;
        slot->endFlag = Hu3DMotionEndCheck(lbl_1_bss_4A28->models[idx].id);
        if (slot->endFlag != 0 && slot->chain != 0) {
            slot->endFlag = 0;
            slot->chain = 0;
            slot->motion = slot->chainMotion;
            slot->time = slot->chainTime;
            slot->attrSet = slot->chainAttr;
            slot->attrReset = slot->chainReset;
        }
    }
}
