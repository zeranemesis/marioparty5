#include "types.h"
#include "string.h"
#include "game/gamework.h"
#include "game/pad.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/hsfex.h"
#include "REL/m544Dll.h"

double sin(double x);
double cos(double x);
s32 rand8(void);
f32 Hu3DMotionMaxTimeGet(HU3DMODELID modelId);
f32 Hu3DMotionTimeGet(HU3DMODELID modelId);
f32 Hu3DMotionShiftMaxTimeGet(HU3DMODELID modelId);
f32 Hu3DMotionShiftTimeGet(HU3DMODELID modelId);

/* m544.c / game.c */
extern M544Game lbl_1_bss_D0;
void fn_1_4098(s16 arg0, s16 arg1, s16 arg2);
void fn_1_4120(s16 arg0, s16 arg1);
void fn_1_425C(s16 arg0);

/* unit-owned .bss */
M544State *lbl_1_bss_918;

/* unit-owned .data */
HuVecF lbl_1_data_418[12] = {
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

void fn_1_9EF8(M544State *state, M544Mdl *models, s32 *dataNums)
{
    s16 j;
    s16 i;
    s16 grp;

    lbl_1_bss_918 = state;
#ifdef TARGET_PC
    memset(lbl_1_bss_918, 0, sizeof(M544State));
#else
    memset(lbl_1_bss_918, 0, 0x2E8);
#endif
    lbl_1_bss_918->models = models;
    lbl_1_bss_918->dataNums = dataNums;
    i = 0;
    do {
        for (j = 0; j < 4; j++) {
            if (i == (grp = GwPlayerConf[j].grp)) {
                lbl_1_bss_918->chars[i].playerNo = j;
                i++;
                if (i >= 2) {
                    break;
                }
            }
        }
    } while (i < 2);
    for (j = 0; j < 2; j++) {
        lbl_1_bss_918->chars[j].charNo = GwPlayerConf[lbl_1_bss_918->chars[j].playerNo].charNo;
        lbl_1_bss_918->chars[j].padNo = GwPlayerConf[lbl_1_bss_918->chars[j].playerNo].padNo;
        grp = GwPlayerConf[lbl_1_bss_918->chars[j].playerNo].type;
        if (grp != 0) {
            lbl_1_bss_918->chars[j].cpuLevel = GwPlayerConf[lbl_1_bss_918->chars[j].playerNo].dif + 1;
        }
    }
}

void fn_1_A144(s16 arg0)
{
    s16 i;
    HU3DMODELID model;
    s16 idx;

    lbl_1_bss_918->stage = arg0;
    for (i = 0; i < 12; i++) {
        idx = i + 0x14;
        model = lbl_1_bss_918->models[idx].id = Hu3DModelCreate(HuDataSelHeapReadNum((&lbl_1_bss_918->dataNums[13])[i], 0x10000000, 2));
        Hu3DModelAttrSet(model, 1);
        Hu3DModelCameraSet(model, 1);
        Hu3DModelLayerSet(model, 0);
        Hu3DModelPosSetV(model, &lbl_1_data_418[i]);
        Hu3DModelRotSet(model, 0.0f, 0.0f, 0.0f);
        Hu3DMotionSpeedSet(model, 0.0f);
        Hu3DMotionTimeSet(model, 0.0f);
        Hu3DModelAttrSet(model, 0x40000001);
        lbl_1_bss_918->models[idx].pos.x = lbl_1_data_418[i].x;
        lbl_1_bss_918->models[idx].pos.y = lbl_1_data_418[i].y;
        lbl_1_bss_918->models[idx].pos.z = lbl_1_data_418[i].z;
        lbl_1_bss_918->models[idx].rot.x = 0.0f;
        lbl_1_bss_918->models[idx].rot.y = 0.0f;
        lbl_1_bss_918->models[idx].rot.z = 0.0f;
        Hu3DModelAttrReset(model, 1);
        if (i == 0) {
            Hu3DModelShadowMapSet(model);
        }
    }
}

void fn_1_A3F0(s16 charNo, s32 *motData)
{
    lbl_1_bss_918->motionData[charNo] = motData;
}

void fn_1_A410(s16 charNo, s16 *motId)
{
    lbl_1_bss_918->motionIds[charNo] = motId;
}

void fn_1_A430(void)
{
    s32 i;
    M544Slot *slot;

    for (i = 0; i < 2; i++) {
        (&lbl_1_bss_918->models[0x20])[i].id = CharModelCreate(lbl_1_bss_918->chars[i].charNo, 1);
        Hu3DModelPosSet((&lbl_1_bss_918->models[0x20])[i].id, -150.0f + 300.0f * i, 100.0f, 0.0f);
        CharModelFxFlagSet(lbl_1_bss_918->chars[i].charNo, 1);
        Hu3DModelLayerSet(lbl_1_bss_918->models[i].id, 0);
        Hu3DModelShadowSet((&lbl_1_bss_918->models[0x20])[i].id);
        CharModelStepFxSet(lbl_1_bss_918->chars[i].charNo, 1);
        CharModelFxFlagSet(lbl_1_bss_918->chars[i].charNo, 0);
        slot = &lbl_1_bss_918->slots[i];
        slot->motion = 4;
        slot->speed = 1.0f;
        slot->attrSet = 0x40000001;
        slot->attrReset = 0;
        slot->time = 0.0f;
    }
    lbl_1_bss_918->models[0x22].id = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_bss_918->dataNums[25], 0x10000000, 2));
    Hu3DModelAttrSet(lbl_1_bss_918->models[0x22].id, 1);
    Hu3DModelCameraSet(lbl_1_bss_918->models[0x22].id, 1);
    Hu3DModelLayerSet(lbl_1_bss_918->models[0x22].id, 2);
    Hu3DModelShadowSet(lbl_1_bss_918->models[0x22].id);
    Hu3DModelAmbSet(lbl_1_bss_918->models[0x22].id, 1.0f, 1.0f, 1.0f);
    Hu3DModelAttrReset(lbl_1_bss_918->models[0x22].id, 1);
    slot = &lbl_1_bss_918->slots[2];
    slot->motion = 0;
    slot->speed = 1.0f;
    slot->attrSet = 0x40000001;
    slot->attrReset = 0;
    slot->time = 0.0f;
    lbl_1_bss_918->models[0x20].pos.x = -300.0f;
    lbl_1_bss_918->models[0x20].pos.y = 100.0f;
    lbl_1_bss_918->models[0x20].pos.z = -240.0f;
    lbl_1_bss_918->models[0x20].rot.x = 0.0f;
    lbl_1_bss_918->models[0x20].rot.y = 90.0f;
    lbl_1_bss_918->models[0x20].rot.z = 0.0f;
    lbl_1_bss_918->models[0x21].pos.x = 300.0f;
    lbl_1_bss_918->models[0x21].pos.y = 100.0f;
    lbl_1_bss_918->models[0x21].pos.z = -240.0f;
    lbl_1_bss_918->models[0x21].rot.x = 0.0f;
    lbl_1_bss_918->models[0x21].rot.y = -90.0f;
    lbl_1_bss_918->models[0x21].rot.z = 0.0f;
    lbl_1_bss_918->models[0x22].pos.x = -0.0f;
    lbl_1_bss_918->models[0x22].pos.y = 100.0f;
    lbl_1_bss_918->models[0x22].pos.z = -400.0f;
    lbl_1_bss_918->models[0x22].rot.x = 0.0f;
    lbl_1_bss_918->models[0x22].rot.y = 0.0f;
    lbl_1_bss_918->models[0x22].rot.z = 0.0f;
}

/* differs: s8 sign-extension of the pad stick reads (orig keeps extra extsb) */
void fn_1_A9A4(void)
{
    s32 i;
    s8 absX;
    s8 newX;

    for (i = 0; i < 2; i++) {
        if (lbl_1_bss_918->chars[i].cpuLevel != 0) {
            memset(&lbl_1_bss_918->chars[i].btn, 0, 0x14);
        } else {
            lbl_1_bss_918->chars[i].btn = HuPadBtn[lbl_1_bss_918->chars[i].padNo];
            lbl_1_bss_918->chars[i].btnDown = HuPadBtnDown[lbl_1_bss_918->chars[i].padNo];
            lbl_1_bss_918->chars[i].btnRep = HuPadBtnRep[lbl_1_bss_918->chars[i].padNo];
            lbl_1_bss_918->chars[i].stkX = HuPadStkX[lbl_1_bss_918->chars[i].padNo];
            if (lbl_1_bss_918->chars[i].stkX >= 0) {
                absX = lbl_1_bss_918->chars[i].stkX;
            } else {
                absX = -lbl_1_bss_918->chars[i].stkX;
            }
            if (absX < 10) {
                newX = 0;
            } else {
                newX = lbl_1_bss_918->chars[i].stkX;
            }
            lbl_1_bss_918->chars[i].stkX = newX;
            lbl_1_bss_918->chars[i].stkY = HuPadStkY[lbl_1_bss_918->chars[i].padNo];
            lbl_1_bss_918->chars[i].subStkX = HuPadSubStkX[lbl_1_bss_918->chars[i].padNo];
            lbl_1_bss_918->chars[i].subStkY = HuPadSubStkY[lbl_1_bss_918->chars[i].padNo];
            lbl_1_bss_918->chars[i].trigL = HuPadTrigL[lbl_1_bss_918->chars[i].padNo];
            lbl_1_bss_918->chars[i].trigR = HuPadTrigR[lbl_1_bss_918->chars[i].padNo];
            lbl_1_bss_918->chars[i].dstk = HuPadDStk[lbl_1_bss_918->chars[i].padNo];
            lbl_1_bss_918->chars[i].dstkRep = HuPadDStkRep[lbl_1_bss_918->chars[i].padNo];
        }
    }
}

void fn_1_ADBC(void)
{
    s16 i;
    s16 j;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 7; j++) {
            lbl_1_bss_918->motionIds[i][j] =
                CharMotionCreate(lbl_1_bss_918->chars[i].charNo, lbl_1_bss_918->motionData[i][j]);
        }
    }
    for (i = 0; i < 4; i++) {
        lbl_1_bss_918->motionIds[2][i] =
            Hu3DJointMotion(lbl_1_bss_918->models[0x22].id,
                            HuDataSelHeapReadNum(lbl_1_bss_918->motionData[2][i], 0x10000000, 2));
    }
    for (i = 0; i < 3; i++) {
        lbl_1_bss_918->slots[i].curMotion = -1;
        lbl_1_bss_918->slots[i].curSpeed = -1.0f;
    }
}

void fn_1_AF60(void)
{
    s16 i;
    s16 idx;
    M544Slot *slot;

    for (i = 0; i < 3; i++) {
        slot = &lbl_1_bss_918->slots[i];
        idx = i + 0x20;
        if (slot->motion != slot->curMotion) {
            Hu3DMotionSet(lbl_1_bss_918->models[idx].id, lbl_1_bss_918->motionIds[i][slot->motion]);
            slot->curMotion = slot->motion;
            slot->maxTime = Hu3DMotionMaxTimeGet(lbl_1_bss_918->models[idx].id);
        }
        if (slot->speed != slot->curSpeed) {
            Hu3DMotionSpeedSet(lbl_1_bss_918->models[idx].id, slot->speed);
            slot->curSpeed = slot->speed;
        }
        if (slot->attrSet != slot->attrCur) {
            Hu3DModelAttrSet(lbl_1_bss_918->models[idx].id, slot->attrSet);
            slot->attrCur = slot->attrSet;
        }
        if (slot->attrReset != 0) {
            Hu3DModelAttrReset(lbl_1_bss_918->models[idx].id, slot->attrReset);
            slot->attrReset = ~slot->attrReset;
            slot->attrCur = slot->attrSet = slot->attrSet & slot->attrReset;
            slot->attrReset = 0;
        }
        if (slot->time != slot->curTime) {
            Hu3DMotionTimeSet(lbl_1_bss_918->models[idx].id, slot->time);
        }
        switch (slot->shiftState) {
        case 0:
            break;
        case 1:
            Hu3DMotionShiftSet(lbl_1_bss_918->models[idx].id, lbl_1_bss_918->motionIds[i][slot->shiftMotion],
                               slot->shiftStart, slot->shiftEnd, slot->shiftAttr);
            slot->shiftMax = Hu3DMotionShiftMaxTimeGet(lbl_1_bss_918->models[idx].id);
            slot->shiftState = 2;
            break;
        case 2:
            slot->shiftCur = Hu3DMotionShiftTimeGet(lbl_1_bss_918->models[idx].id);
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
        slot->time = Hu3DMotionTimeGet(lbl_1_bss_918->models[idx].id);
        slot->curTime = slot->time;
        slot->endFlag = Hu3DMotionEndCheck(lbl_1_bss_918->models[idx].id);
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

void fn_1_B384(void);
void fn_1_B990(void);
void fn_1_BEDC(void);
void fn_1_C7C0(void);
void fn_1_D1CC(void);

void (*lbl_1_data_4A8[5])(void) = {
    fn_1_B384,
    fn_1_B990,
    fn_1_BEDC,
    fn_1_C7C0,
    fn_1_D1CC,
};

void fn_1_B318(void)
{
    s32 i;

    for (i = 0; i < 5; i++) {
        lbl_1_data_4A8[i]();
    }
}

void fn_1_B36C(void)
{
    lbl_1_bss_918->anim[0].timer = 0;
}

void fn_1_B384(void)
{
    M544Mdl *p = &lbl_1_bss_918->models[0x17];

    if (lbl_1_bss_918->anim[0].hold == 0) {
        switch (lbl_1_bss_918->anim[0].state) {
        case 0:
            lbl_1_bss_918->anim[0].timer = 10000;
            lbl_1_bss_918->anim[0].state = 1;
            lbl_1_bss_918->anim[0].f24 = 0.4f;
            Hu3DMotionSpeedSet(p->id, lbl_1_bss_918->anim[0].f24);
            p->rot.y = -60.0f;
            return;
        case 1:
            if (lbl_1_bss_918->anim[0].timer-- < 0) {
                lbl_1_bss_918->anim[0].state++;
                lbl_1_bss_918->anim[0].timer = 0;
                lbl_1_bss_918->anim[0].f20 = p->rot.y;
                lbl_1_bss_918->anim[0].timer2 = 0x78;
                lbl_1_bss_918->anim[0].f24 = (rand8() % 10) + 0x1E;
                if (lbl_1_bss_918->anim[0].f20 > -45.0f) {
                    lbl_1_bss_918->anim[0].f24 = -lbl_1_bss_918->anim[0].f24;
                    return;
                }
            }
            break;
        case 2:
            lbl_1_bss_918->anim[0].timer++;
            Hu3DMotionSpeedSet(p->id,
                               0.4 + 0.5 * sin(3.141592653589793 * (f64)((lbl_1_bss_918->anim[0].timer * 0x5A) / 40) / 180.0));
            if (lbl_1_bss_918->anim[0].timer == 0x28) {
                lbl_1_bss_918->anim[0].timer = 0;
                lbl_1_bss_918->anim[0].state++;
                return;
            }
            break;
        case 3:
            lbl_1_bss_918->anim[0].timer++;
            Hu3DMotionSpeedSet(p->id,
                               0.8 + 0.2 * sin(3.141592653589793 * (f64)((lbl_1_bss_918->anim[0].timer * 0xB4) / lbl_1_bss_918->anim[0].timer2) / 180.0));
            p->rot.y = lbl_1_bss_918->anim[0].f20
                + lbl_1_bss_918->anim[0].f24 * sin(3.141592653589793 * (f64)((lbl_1_bss_918->anim[0].timer * 0x5A) / lbl_1_bss_918->anim[0].timer2) / 180.0);
            if (lbl_1_bss_918->anim[0].timer == lbl_1_bss_918->anim[0].timer2) {
                p->rot.y = lbl_1_bss_918->anim[0].f20 + lbl_1_bss_918->anim[0].f24;
                lbl_1_bss_918->anim[0].state++;
                lbl_1_bss_918->anim[0].timer = 0;
                return;
            }
            break;
        case 4:
            lbl_1_bss_918->anim[0].timer++;
            Hu3DMotionSpeedSet(p->id,
                               0.8 - 0.4 * sin(3.141592653589793 * (f64)((lbl_1_bss_918->anim[0].timer * 0x5A) / 40) / 180.0));
            if (lbl_1_bss_918->anim[0].timer == 0x28) {
                lbl_1_bss_918->anim[0].timer = (rand8() % 120) + 0xB4;
                lbl_1_bss_918->anim[0].state = 1;
            }
            break;
        }
    }
}

void fn_1_B990(void)
{
    M544Mdl *p1 = &lbl_1_bss_918->models[0x15];
    M544Mdl *p2 = &lbl_1_bss_918->models[0x16];

    if (lbl_1_bss_918->anim[1].hold == 0) {
        p1->rot.y = 30.0f;
        p2->rot.y = 30.0f;
        switch (lbl_1_bss_918->anim[1].state) {
        case 0:
            if (lbl_1_bss_918->anim[0].state == 2) {
                lbl_1_bss_918->anim[1].timer = 0;
                lbl_1_bss_918->anim[1].state++;
                lbl_1_bss_918->anim[1].f20 = 20.0f;
                lbl_1_bss_918->anim[1].f24 = 5.0f;
                lbl_1_bss_918->anim[1].f28 = -5.0f;
                return;
            }
            break;
        case 1:
            lbl_1_bss_918->anim[1].timer++;
            p1->rot.y = 30.0 + lbl_1_bss_918->anim[1].f20
                * sin(3.141592653589793 * (f64)((f32)lbl_1_bss_918->anim[1].timer * lbl_1_bss_918->anim[1].f24) / 180.0);
            p2->rot.y = 30.0 + lbl_1_bss_918->anim[1].f20
                * sin(3.141592653589793 * (f64)((f32)lbl_1_bss_918->anim[1].timer * lbl_1_bss_918->anim[1].f28) / 180.0);
            if (lbl_1_bss_918->anim[0].state == 4) {
                lbl_1_bss_918->anim[1].state++;
                lbl_1_bss_918->anim[1].timer2 = 0;
                return;
            }
            break;
        case 2:
            lbl_1_bss_918->anim[1].timer++;
            lbl_1_bss_918->anim[1].timer2++;
            if (lbl_1_bss_918->anim[1].timer2 > 0x28) {
                lbl_1_bss_918->anim[1].timer2 = 0x28;
            }
            lbl_1_bss_918->anim[1].f20 = 20.0 - 20.0 * sin(3.141592653589793 * (f64)((lbl_1_bss_918->anim[1].timer2 * 0x5A) / 40) / 180.0);
            p1->rot.y = 30.0 + lbl_1_bss_918->anim[1].f20
                * sin(3.141592653589793 * (f64)((f32)lbl_1_bss_918->anim[1].timer * lbl_1_bss_918->anim[1].f24) / 180.0);
            p2->rot.y = 30.0 + lbl_1_bss_918->anim[1].f20
                * sin(3.141592653589793 * (f64)((f32)lbl_1_bss_918->anim[1].timer * lbl_1_bss_918->anim[1].f28) / 180.0);
            if (lbl_1_bss_918->anim[0].state == 1) {
                lbl_1_bss_918->anim[1].state = 0;
            }
            break;
        }
    }
}

void fn_1_BEDC(void)
{
    Mtx sp38;
    Mtx sp8;
    M544Mdl *p1 = &lbl_1_bss_918->models[0x18];
    M544Mdl *p2 = &lbl_1_bss_918->models[0x19];

    if (lbl_1_bss_918->anim[2].hold == 0) {
        PSMTXIdentity(sp38);
        PSMTXRotRad(sp8, 'y', -0.7853982f);
        PSMTXConcat(sp38, sp8, sp38);
        p1->rot.y = 30.0f;
        switch (lbl_1_bss_918->anim[2].state) {
        case 0:
            p2->rot.y = -45.0f;
            if (lbl_1_bss_918->anim[0].state == 2) {
                lbl_1_bss_918->anim[2].f20 = 10.0f;
                lbl_1_bss_918->anim[2].f24 = 5.0f;
                lbl_1_bss_918->anim[2].timer = 0;
                lbl_1_bss_918->anim[2].state++;
            }
            break;
        case 1:
            lbl_1_bss_918->anim[2].timer++;
            lbl_1_bss_918->anim[2].f24 = 5.0 + 5.0 * sin(3.141592653589793 * (f64)((lbl_1_bss_918->anim[2].timer * 0x5A) / 40) / 180.0);
            p1->rot.x = lbl_1_bss_918->anim[2].f20
                * sin(3.141592653589793 * (f64)((f32)lbl_1_bss_918->anim[2].timer * lbl_1_bss_918->anim[2].f24) / 180.0);
            PSMTXRotRad(sp8, 'z', 0.01745329238474369 * (lbl_1_bss_918->anim[2].f20
                * sin(3.141592653589793 * (f64)((f32)lbl_1_bss_918->anim[2].timer * lbl_1_bss_918->anim[2].f24) / 180.0)));
            PSMTXConcat(sp38, sp8, sp38);
            Hu3DMtxRotGet(sp38, (HuVecF *)&p2->rot);
            if (lbl_1_bss_918->anim[0].state == 3) {
                lbl_1_bss_918->anim[2].f24 = 10.0f;
                lbl_1_bss_918->anim[2].state++;
            }
            break;
        case 2:
            lbl_1_bss_918->anim[2].timer++;
            p1->rot.x = lbl_1_bss_918->anim[2].f20
                * sin(3.141592653589793 * (f64)((f32)lbl_1_bss_918->anim[2].timer * lbl_1_bss_918->anim[2].f24) / 180.0);
            PSMTXRotRad(sp8, 'z', 0.01745329238474369 * (lbl_1_bss_918->anim[2].f20
                * sin(3.141592653589793 * (f64)((f32)lbl_1_bss_918->anim[2].timer * lbl_1_bss_918->anim[2].f24) / 180.0)));
            PSMTXConcat(sp38, sp8, sp38);
            Hu3DMtxRotGet(sp38, (HuVecF *)&p2->rot);
            if (lbl_1_bss_918->anim[0].state == 4) {
                lbl_1_bss_918->anim[2].state++;
                lbl_1_bss_918->anim[2].timer2 = 0;
            }
            break;
        case 3:
            lbl_1_bss_918->anim[2].timer2++;
            lbl_1_bss_918->anim[2].timer++;
            if (lbl_1_bss_918->anim[2].timer2 > 0x28) {
                lbl_1_bss_918->anim[2].timer2 = 0x28;
            }
            lbl_1_bss_918->anim[2].f20 = 10.0 - 5.0 * sin(3.141592653589793 * (f64)((lbl_1_bss_918->anim[2].timer2 * 0x5A) / 40) / 180.0);
            p1->rot.x = lbl_1_bss_918->anim[2].f20
                * sin(3.141592653589793 * (f64)((f32)lbl_1_bss_918->anim[2].timer * lbl_1_bss_918->anim[2].f24) / 180.0);
            PSMTXRotRad(sp8, 'z', 0.01745329238474369 * (lbl_1_bss_918->anim[2].f20
                * sin(3.141592653589793 * (f64)((f32)lbl_1_bss_918->anim[2].timer * lbl_1_bss_918->anim[2].f24) / 180.0)));
            PSMTXConcat(sp38, sp8, sp38);
            Hu3DMtxRotGet(sp38, (HuVecF *)&p2->rot);
            while (p1->rot.x >= 360.0f) {
                p1->rot.x -= 360.0f;
            }
            if (lbl_1_bss_918->anim[0].state == 1 && (p1->rot.x >= 359.0f || p1->rot.x <= 1.0f)) {
                lbl_1_bss_918->anim[2].f20 = 0.0f;
                p1->rot.x = 0.0f;
                p2->rot.x = 0.0f;
                p2->rot.y = -45.0f;
                p2->rot.z = 0.0f;
                lbl_1_bss_918->anim[2].state = 0;
            }
            break;
        }
        while (p1->rot.x >= 360.0f) {
            p1->rot.x -= 360.0f;
        }
    }
}

void fn_1_C7C0(void)
{
    M544Mdl *p1 = &lbl_1_bss_918->models[0x1A];
    M544Mdl *p2 = &lbl_1_bss_918->models[0x1C];

    switch ((u32)lbl_1_bss_918->anim[3].state) {
    case 0:
        if (lbl_1_bss_918->anim[0].state == 2) {
            lbl_1_bss_918->anim[3].timer = 0;
            if (lbl_1_bss_918->anim[3].f20 == 0.0f) {
                lbl_1_bss_918->anim[3].state++;
                lbl_1_bss_918->anim[3].timer = 0;
                lbl_1_bss_918->anim[3].f20 = 15.0f;
                lbl_1_bss_918->anim[3].f28 = (rand8() % 16) * 25 - 0x28A;
                p1->rot.z = 0.0f;
                p1->pos.x = -1000.0f;
            } else {
                lbl_1_bss_918->anim[3].state = 4;
                lbl_1_bss_918->anim[3].f20 = 10.0f;
                lbl_1_bss_918->anim[3].f28 = (rand8() % 16) * 25 - 0x28A;
                p1->rot.z = 0.0f;
                p1->pos.x = -1000.0f;
            }
            lbl_1_bss_918->anim[3].f24 = (250.0f * lbl_1_bss_918->anim[3].f20) / 360.0f;
        }
        break;
    case 1:
        p1->rot.z -= lbl_1_bss_918->anim[3].f20;
        p1->pos.x += lbl_1_bss_918->anim[3].f24;
        if (p1->pos.x >= lbl_1_bss_918->anim[3].f28) {
            lbl_1_bss_918->anim[3].state++;
        }
        break;
    case 2:
        lbl_1_bss_918->anim[3].timer++;
        lbl_1_bss_918->anim[3].f20 = 7.5 + 7.5 * cos(3.141592653589793 * (f64)lbl_1_bss_918->anim[3].timer / 180.0);
        lbl_1_bss_918->anim[3].f24 = (250.0f * lbl_1_bss_918->anim[3].f20) / 360.0f;
        p1->rot.z -= lbl_1_bss_918->anim[3].f20;
        p1->pos.x += lbl_1_bss_918->anim[3].f24;
        if (lbl_1_bss_918->anim[3].timer >= 0x5A) {
            lbl_1_bss_918->anim[3].state++;
        }
        break;
    case 3:
        lbl_1_bss_918->anim[3].timer++;
        if (lbl_1_bss_918->anim[3].timer >= 0xB4) {
            lbl_1_bss_918->anim[3].timer = 0xB4;
        }
        lbl_1_bss_918->anim[3].f20 = 7.5 - 7.5 * cos(3.141592653589793 * (f64)lbl_1_bss_918->anim[3].timer / 180.0);
        lbl_1_bss_918->anim[3].f24 = (250.0f * lbl_1_bss_918->anim[3].f20) / 360.0f;
        if (p1->pos.x < 1000.0f) {
            p1->rot.z -= lbl_1_bss_918->anim[3].f20;
            p1->pos.x += lbl_1_bss_918->anim[3].f24;
        } else if (lbl_1_bss_918->anim[0].state == 1) {
            lbl_1_bss_918->anim[3].state = 0;
        }
        break;
    case 4:
        lbl_1_bss_918->anim[3].timer++;
        if (lbl_1_bss_918->anim[3].timer >= 0x14) {
            p1->rot.z -= lbl_1_bss_918->anim[3].f20;
            p1->pos.x += lbl_1_bss_918->anim[3].f24;
            if (p1->pos.x >= lbl_1_bss_918->anim[3].f28) {
                lbl_1_bss_918->anim[3].state++;
                lbl_1_bss_918->anim[3].timer = 0;
            }
        }
        break;
    case 5:
        lbl_1_bss_918->anim[3].timer++;
        lbl_1_bss_918->anim[3].f20 = 10.0 * cos(3.141592653589793 * (f64)lbl_1_bss_918->anim[3].timer / 180.0);
        lbl_1_bss_918->anim[3].f24 = (250.0f * lbl_1_bss_918->anim[3].f20) / 360.0f;
        p1->rot.z -= lbl_1_bss_918->anim[3].f20;
        p1->pos.x += lbl_1_bss_918->anim[3].f24;
        if (lbl_1_bss_918->anim[3].timer == 0x5A) {
            lbl_1_bss_918->anim[3].state++;
            lbl_1_bss_918->anim[3].timer = 0;
        }
        break;
    case 6:
        if (lbl_1_bss_918->anim[0].state == 2) {
            lbl_1_bss_918->anim[3].state++;
        }
        break;
    case 7:
        lbl_1_bss_918->anim[3].timer++;
        if (lbl_1_bss_918->anim[3].timer >= 0x96) {
            lbl_1_bss_918->anim[3].timer = 0x96;
        }
        if (lbl_1_bss_918->anim[3].timer >= 0x3C) {
            lbl_1_bss_918->anim[3].f20 = 10.0 * sin(3.141592653589793 * (f64)(lbl_1_bss_918->anim[3].timer - 0x3C) / 180.0);
            lbl_1_bss_918->anim[3].f24 = (250.0f * lbl_1_bss_918->anim[3].f20) / 360.0f;
            if (p1->pos.x < 1000.0f) {
                if (lbl_1_bss_918->anim[3].timer >= 0x46) {
                    p1->rot.z -= lbl_1_bss_918->anim[3].f20;
                }
                p1->pos.x += lbl_1_bss_918->anim[3].f24;
            } else if (lbl_1_bss_918->anim[0].state == 1) {
                lbl_1_bss_918->anim[3].state = 0;
                lbl_1_bss_918->anim[3].f20 = 0.0f;
            }
        }
        break;
    }
    p2->pos.x = p1->pos.x;
    p2->pos.y = p1->pos.y - 40.0f;
    p2->pos.z = p1->pos.z;
}

void fn_1_D1CC(void)
{
    M544Mdl *p = &lbl_1_bss_918->models[0x1B];

    switch (lbl_1_bss_918->anim[4].state) {
    case 0:
        if (lbl_1_bss_918->anim[0].state == 2) {
            lbl_1_bss_918->anim[4].state++;
            lbl_1_bss_918->anim[4].timer = 0;
            Hu3DMotionSpeedSet(p->id, 0.5f);
            Hu3DMotionTimeSet(p->id, 0.0f);
            switch (lbl_1_bss_D0.flag38) {
            case 0:
                fn_1_4098(0, 0x7F, 0x24);
                return;
            case 1:
                fn_1_425C(0);
                lbl_1_bss_D0.flag38++;
                return;
            }
        } else {
            return;
        }
        break;
    case 1:
        lbl_1_bss_918->anim[4].timer++;
        if (lbl_1_bss_D0.flag38 != 1) {
            fn_1_4120(0, ((lbl_1_bss_918->anim[4].timer << 6) / 200) + 0x20);
        }
        p->pos.x = ((lbl_1_bss_918->anim[4].timer * 0xFA0) / 200) - 0x7D0;
        if (lbl_1_bss_918->anim[4].timer == 0xC8) {
            lbl_1_bss_918->anim[4].state++;
            return;
        }
        break;
    case 2:
        if (lbl_1_bss_918->anim[0].state == 1) {
            lbl_1_bss_918->anim[4].state = 0;
        }
        break;
    }
}
