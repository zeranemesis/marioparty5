#include "types.h"
#include "game/gamework.h"
#include "game/pad.h"
#include "game/charman.h"
#include "string.h"
#include "REL/m543Dll.h"

f32 Hu3DMotionMaxTimeGet(HU3DMODELID modelId);

M543State *lbl_1_bss_15E8;

void fn_1_FA94(M543State *state, M543StageModel *models, s32 *dataNums)
{
    s16 j;
    s16 i;
    s16 grp;

    lbl_1_bss_15E8 = state;
    memset(lbl_1_bss_15E8, 0, sizeof(M543State));
    lbl_1_bss_15E8->models = models;
    lbl_1_bss_15E8->dataNums = dataNums;
    i = 0;
    do {
        for (j = 0; j < 4; j++) {
            if (i == (grp = GwPlayerConf[j].grp)) {
                lbl_1_bss_15E8->chars[i].playerNo = j;
                i++;
                if (i >= 2) {
                    break;
                }
            }
        }
    } while (i < 2);
    for (j = 0; j < 2; j++) {
        lbl_1_bss_15E8->chars[j].charNo = GwPlayerConf[lbl_1_bss_15E8->chars[j].playerNo].charNo;
        lbl_1_bss_15E8->chars[j].padNo = GwPlayerConf[lbl_1_bss_15E8->chars[j].playerNo].padNo;
        grp = GwPlayerConf[lbl_1_bss_15E8->chars[j].playerNo].type;
        if (grp != 0) {
            lbl_1_bss_15E8->chars[j].cpuLevel = GwPlayerConf[lbl_1_bss_15E8->chars[j].playerNo].dif + 1;
        }
    }
}

void fn_1_FCE0(s16 arg0)
{
    lbl_1_bss_15E8->field_174 = arg0;
}

void fn_1_FCF4(s16 arg0, s32 *arg1)
{
    lbl_1_bss_15E8->motionData[arg0] = arg1;
}

void fn_1_FD14(s16 arg0, s16 *arg1)
{
    lbl_1_bss_15E8->motionIds[arg0] = arg1;
}

void fn_1_FD34(void)
{
}

void fn_1_FD38(void)
{
    s32 i;
    s8 absX;
    s8 newX;

    for (i = 0; i < 2; i++) {
        if (lbl_1_bss_15E8->chars[i].cpuLevel != 0) {
            memset(&lbl_1_bss_15E8->chars[i].btn, 0, 0x14);
        } else {
            lbl_1_bss_15E8->chars[i].btn = HuPadBtn[lbl_1_bss_15E8->chars[i].padNo];
            lbl_1_bss_15E8->chars[i].btnDown = HuPadBtnDown[lbl_1_bss_15E8->chars[i].padNo];
            lbl_1_bss_15E8->chars[i].btnRep = HuPadBtnRep[lbl_1_bss_15E8->chars[i].padNo];
            lbl_1_bss_15E8->chars[i].stkX = (s8)HuPadStkX[lbl_1_bss_15E8->chars[i].padNo];
            if (lbl_1_bss_15E8->chars[i].stkX >= 0) {
                absX = lbl_1_bss_15E8->chars[i].stkX;
            } else {
                absX = -lbl_1_bss_15E8->chars[i].stkX;
            }
            if (absX < 10) {
                newX = 0;
            } else {
                newX = lbl_1_bss_15E8->chars[i].stkX;
            }
            lbl_1_bss_15E8->chars[i].stkX = newX;
            lbl_1_bss_15E8->chars[i].stkY = (s8)HuPadStkY[lbl_1_bss_15E8->chars[i].padNo];
            lbl_1_bss_15E8->chars[i].subStkX = (s8)HuPadSubStkX[lbl_1_bss_15E8->chars[i].padNo];
            lbl_1_bss_15E8->chars[i].subStkY = (s8)HuPadSubStkY[lbl_1_bss_15E8->chars[i].padNo];
            lbl_1_bss_15E8->chars[i].trigL = HuPadTrigL[lbl_1_bss_15E8->chars[i].padNo];
            lbl_1_bss_15E8->chars[i].trigR = HuPadTrigR[lbl_1_bss_15E8->chars[i].padNo];
            lbl_1_bss_15E8->chars[i].dstk = HuPadDStk[lbl_1_bss_15E8->chars[i].padNo];
            lbl_1_bss_15E8->chars[i].dstkRep = HuPadDStkRep[lbl_1_bss_15E8->chars[i].padNo];
        }
    }
}

void fn_1_10150(void)
{
    s16 i;
    s16 j;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 7; j++) {
            lbl_1_bss_15E8->motionIds[i][j] =
                CharMotionCreate(lbl_1_bss_15E8->chars[i].charNo, lbl_1_bss_15E8->motionData[i][j]);
        }
    }
    for (i = 0; i < 2; i++) {
        lbl_1_bss_15E8->slots[i].motion = 0;
        lbl_1_bss_15E8->slots[i].id = 0;
        lbl_1_bss_15E8->slots[i].speed = 0.0f;
        lbl_1_bss_15E8->slots[i].curSpeed = 0.0f;
        lbl_1_bss_15E8->slots[i].attrSet = 0;
        lbl_1_bss_15E8->slots[i].attrCur = 0;
    }
}

void fn_1_10304(OMOBJ *obj)
{
    s16 i;
    s16 idx;
    M543StageSlot *slot;

    for (i = 0; i < 2; i++) {
        slot = &lbl_1_bss_15E8->slots[i];
        idx = i + 0x3C;
        if (slot->motion != slot->id) {
            Hu3DMotionSet(lbl_1_bss_15E8->models[idx].id, lbl_1_bss_15E8->motionIds[i][slot->motion]);
            slot->id = slot->motion;
            slot->maxTime = Hu3DMotionMaxTimeGet(lbl_1_bss_15E8->models[idx].id);
        }
        if (slot->speed != slot->curSpeed) {
            Hu3DMotionSpeedSet(lbl_1_bss_15E8->models[idx].id, slot->speed);
            slot->curSpeed = slot->speed;
        }
        if (slot->attrSet != slot->attrCur) {
            Hu3DModelAttrSet(lbl_1_bss_15E8->models[idx].id, slot->attrSet);
            slot->attrCur = slot->attrSet;
        }
        if (slot->attrReset != 0) {
            Hu3DModelAttrReset(lbl_1_bss_15E8->models[idx].id, slot->attrReset);
            slot->attrReset = ~(slot->attrReset | slot->attrReset);
            slot->attrSet = slot->attrSet & slot->attrReset;
            slot->attrCur = slot->attrSet;
            slot->attrReset = 0;
        }
        if (slot->time != slot->curTime) {
            Hu3DMotionTimeSet(lbl_1_bss_15E8->models[idx].id, slot->time);
        }
        switch (slot->shiftState) {
        case 0:
            break;
        case 1:
            Hu3DMotionShiftSet(lbl_1_bss_15E8->models[idx].id, lbl_1_bss_15E8->motionIds[i][slot->shiftMotion],
                               slot->shiftStart, slot->shiftEnd, slot->shiftAttr);
            slot->shiftMax = Hu3DMotionShiftMaxTimeGet(lbl_1_bss_15E8->models[idx].id);
            slot->shiftState = 2;
            break;
        case 2:
            slot->shiftCur = Hu3DMotionShiftTimeGet(lbl_1_bss_15E8->models[idx].id);
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
        slot->time = Hu3DMotionTimeGet(lbl_1_bss_15E8->models[idx].id);
        slot->curTime = slot->time;
        slot->endFlag = Hu3DMotionEndCheck(lbl_1_bss_15E8->models[idx].id);
        if (slot->endFlag != 0 && slot->endReq != 0) {
            slot->endFlag = 0;
            slot->endReq = 0;
            slot->motion = slot->endMotion;
            slot->time = slot->endTime;
            slot->attrSet = slot->endAttrSet;
            slot->attrReset = slot->endAttrReset;
        }
    }
}

void fn_1_106BC(void)
{
}

void fn_1_106C0(void)
{
}

void fn_1_106C4(void)
{
}

void fn_1_106C8(void)
{
}

void fn_1_106CC(void)
{
}

void fn_1_106D0(void)
{
}

void fn_1_106D4(void)
{
}
