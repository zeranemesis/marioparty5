#include "REL/m541Dll.h"
#include "string.h"

/* the asm stores the float return (stfs f1); hu3d.h leaves it undeclared on purpose */
float Hu3DMotionMaxTimeGet(HU3DMODELID modelId);

/* ---- part_s.c ---- */
/*
 * m541Dll "stage" unit (stage.s). The stage object is created by fn_1_19AB0;
 * a pointer to it is parked in lbl_1_bss_3738.
 */

M541_STAGE *lbl_1_bss_3738;

void fn_1_19AB0(M541_STAGE *stage, M541_MODEL *model, s32 *dataIds)
{
    s16 i;
    s16 j;
    s16 conf;

    lbl_1_bss_3738 = stage;
    memset(lbl_1_bss_3738, 0, sizeof(M541_STAGE));
    lbl_1_bss_3738->model = model;
    lbl_1_bss_3738->dataIds = dataIds;
    for (i = 0; i < 2; ) {
        for (j = 0; j < 4; j++) {
            conf = GwPlayerConf[j].grp;
            if (i == conf) {
                lbl_1_bss_3738->player[i].playerNo = j;
                i++;
                if (i >= 2) {
                    break;
                }
            }
        }
    }
    for (j = 0; j < 2; j++) {
        lbl_1_bss_3738->player[j].charNo = GwPlayerConf[lbl_1_bss_3738->player[j].playerNo].charNo;
        lbl_1_bss_3738->player[j].padNo = GwPlayerConf[lbl_1_bss_3738->player[j].playerNo].padNo;
        conf = GwPlayerConf[lbl_1_bss_3738->player[j].playerNo].type;
        if (conf != 0) {
            lbl_1_bss_3738->player[j].dif = GwPlayerConf[lbl_1_bss_3738->player[j].playerNo].dif + 1;
        }
    }
}

void fn_1_19CFC(s16 arg)
{
    lbl_1_bss_3738->unk174 = arg;
}

void fn_1_19D10(s16 idx, s32 *val)
{
    lbl_1_bss_3738->motData[idx] = val;
}

void fn_1_19D30(s16 idx, s16 *val)
{
    lbl_1_bss_3738->motId[idx] = val;
}

void fn_1_19D50(void)
{
}

void fn_1_19D54(void)
{
    s32 i;
    s32 stk;
    s32 stkY;

    for (i = 0; i < 2; i++) {
        if (lbl_1_bss_3738->player[i].dif != 0) {
            memset(&lbl_1_bss_3738->player[i].btn, 0, 0x14);
            continue;
        }
        lbl_1_bss_3738->player[i].btn = HuPadBtn[lbl_1_bss_3738->player[i].padNo];
        lbl_1_bss_3738->player[i].btnDown = HuPadBtnDown[lbl_1_bss_3738->player[i].padNo];
        lbl_1_bss_3738->player[i].btnRep = HuPadBtnRep[lbl_1_bss_3738->player[i].padNo];
        lbl_1_bss_3738->player[i].stkX = HuPadStkX[lbl_1_bss_3738->player[i].padNo];
        if (lbl_1_bss_3738->player[i].stkX >= 0) {
            stk = lbl_1_bss_3738->player[i].stkX;
        } else {
            stk = -lbl_1_bss_3738->player[i].stkX;
        }
        if (stk < 10) {
            stkY = 0;
        } else {
            stkY = lbl_1_bss_3738->player[i].stkX;
        }
        lbl_1_bss_3738->player[i].stkX = stkY;
        lbl_1_bss_3738->player[i].stkY = HuPadStkY[lbl_1_bss_3738->player[i].padNo];
        lbl_1_bss_3738->player[i].subStkX = HuPadSubStkX[lbl_1_bss_3738->player[i].padNo];
        lbl_1_bss_3738->player[i].subStkY = HuPadSubStkY[lbl_1_bss_3738->player[i].padNo];
        lbl_1_bss_3738->player[i].trigL = HuPadTrigL[lbl_1_bss_3738->player[i].padNo];
        lbl_1_bss_3738->player[i].trigR = HuPadTrigR[lbl_1_bss_3738->player[i].padNo];
        lbl_1_bss_3738->player[i].dStk = HuPadDStk[lbl_1_bss_3738->player[i].padNo];
        lbl_1_bss_3738->player[i].dStkRep = HuPadDStkRep[lbl_1_bss_3738->player[i].padNo];
    }
}

void fn_1_1A16C(void)
{
    s16 i;
    s16 j;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 8; j++) {
            lbl_1_bss_3738->motId[i][j] = CharMotionCreate(lbl_1_bss_3738->player[i].charNo, lbl_1_bss_3738->motData[i][j]);
        }
    }
    for (i = 0; i < 2; i++) {
        lbl_1_bss_3738->work[i].motNo = 0;
        lbl_1_bss_3738->work[i].motNoPrev = 0;
        lbl_1_bss_3738->work[i].speed = 0.0f;
        lbl_1_bss_3738->work[i].speedOld = 0.0f;
        lbl_1_bss_3738->work[i].attr = 0;
        lbl_1_bss_3738->work[i].attrOld = 0;
    }
}

void fn_1_1A320(void)
{
    s16 i;

    for (i = 0; i < 2; i++) {
        M541_MODELWORK *w = &lbl_1_bss_3738->work[i];
        s16 mdl = i + 0x6A;
        if (w->motNo != w->motNoPrev) {
            Hu3DMotionSet(lbl_1_bss_3738->model[mdl].modelId, lbl_1_bss_3738->motId[i][w->motNo]);
            w->motNoPrev = w->motNo;
            w->maxTime = Hu3DMotionMaxTimeGet(lbl_1_bss_3738->model[mdl].modelId);
        }
        if (w->speed != w->speedOld) {
            Hu3DMotionSpeedSet(lbl_1_bss_3738->model[mdl].modelId, w->speed);
            w->speedOld = w->speed;
        }
        if (w->attr != w->attrOld) {
            Hu3DModelAttrSet(lbl_1_bss_3738->model[mdl].modelId, w->attr);
            w->attrOld = w->attr;
        }
        if (w->attrReset != 0) {
            Hu3DModelAttrReset(lbl_1_bss_3738->model[mdl].modelId, w->attrReset);
            w->attrReset = ~w->attrReset;
            w->attr &= w->attrReset;
            w->attrOld = w->attr;
            w->attrReset = 0;
        }
        if (w->time != w->timeOld) {
            Hu3DMotionTimeSet(lbl_1_bss_3738->model[mdl].modelId, w->time);
        }
        switch (w->shiftMode) {
        case 1:
            Hu3DMotionShiftSet(lbl_1_bss_3738->model[mdl].modelId, lbl_1_bss_3738->motId[i][w->shiftMotNo], w->shiftSpeed, w->shiftTime, w->shiftAttr);
            w->shiftTimeMax = Hu3DMotionShiftMaxTimeGet(lbl_1_bss_3738->model[mdl].modelId);
            w->shiftMode = 2;
            break;
        case 2:
            w->shiftTimeNow = Hu3DMotionShiftTimeGet(lbl_1_bss_3738->model[mdl].modelId);
            if ((w->shiftAttr & 0x40000001) == 0 && w->shiftTimeNow >= w->shiftTime - 1.0f) {
                w->shiftMode = 3;
            }
            break;
        case 3:
            w->shiftMode = 0;
            break;
        }
        w->time = Hu3DMotionTimeGet(lbl_1_bss_3738->model[mdl].modelId);
        w->timeOld = w->time;
        w->endCheck = Hu3DMotionEndCheck(lbl_1_bss_3738->model[mdl].modelId);
        if (w->endCheck != 0 && w->nextF != 0) {
            w->endCheck = 0;
            w->nextF = 0;
            w->motNo = w->nextMotNo;
            w->time = w->nextTime;
            w->attr = w->nextAttr;
            w->attrReset = w->nextAttrReset;
        }
    }
}

void fn_1_1A6D8(void) {}
void fn_1_1A6DC(void) {}
void fn_1_1A6E0(void) {}
void fn_1_1A6E4(void) {}
void fn_1_1A6E8(void) {}
void fn_1_1A6EC(void) {}
void fn_1_1A6F0(void) {}
