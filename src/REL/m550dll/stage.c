// m550dll/stage.c - decompilation of the m550dll/stage unit.

/* Suppress MSL's `extern inline sqrtf` body (see m550.c). */
#define _MATH_H

#include "types.h"

#include "game/charman.h"
#include "game/data.h"
#include "game/pad.h"

#include "REL/m550dll.h"

#include "string.h"

double sin(double x);
double cos(double x);
int rand8(void);
float Hu3DMotionMaxTimeGet(HU3DMODELID modelId);

/* Defined in m550.c. */
extern s32 lbl_1_data_0[2];
void fn_1_984(s32 idx, f32 alpha);

/* Defined in game.c, where the original object keeps them. */
extern const M550IDX3 lbl_1_rodata_158;
extern const M550IDX3 lbl_1_rodata_164;

void fn_1_6600(void);
void fn_1_6B6C(void);
void fn_1_6DA4(void);
void fn_1_719C(void);
void fn_1_7C4C(void);
void fn_1_7EE0(void);
void fn_1_8350(void);

HuVecF lbl_1_data_3D8[6] = {
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f },
};

s16 lbl_1_data_420[2] = { 0, 0 };

void (*lbl_1_data_424[7])(void) = { fn_1_6600, fn_1_6B6C, fn_1_6DA4, fn_1_719C, fn_1_7C4C, fn_1_7EE0, fn_1_8350 };

M550STATE *lbl_1_bss_1FC0;

void fn_1_4E8C(M550STATE *state, M550MODEL *model, M550MODELIDS *modelData)
{
    s16 i;
    s16 j;

    lbl_1_bss_1FC0 = state;
    memset(lbl_1_bss_1FC0, 0, sizeof(M550STATE));
    lbl_1_bss_1FC0->model = model;
    lbl_1_bss_1FC0->modelData = modelData;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 4; j++) {
            if (i == GwPlayerConf[j].grp) {
                lbl_1_bss_1FC0->player[i].pno = j;
                break;
            }
        }
    }
    for (i = 0; i < 2; i++) {
        lbl_1_bss_1FC0->player[i].charNo = GwPlayerConf[lbl_1_bss_1FC0->player[i].pno].charNo;
        lbl_1_bss_1FC0->player[i].padNo = GwPlayerConf[lbl_1_bss_1FC0->player[i].pno].padNo;
        if (GwPlayerConf[lbl_1_bss_1FC0->player[i].pno].type != 0) {
            lbl_1_bss_1FC0->player[i].cpuLevel = GwPlayerConf[lbl_1_bss_1FC0->player[i].pno].dif + 1;
        }
    }
}

void fn_1_50D8(s16 stageNo)
{
    s16 baseId;
    M550MODEL *p;
    s32 i;
    s32 j;
    s16 k;

    lbl_1_bss_1FC0->stageNo = stageNo;
    for (k = 0; k < 2; k++) {
        baseId = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_bss_1FC0->modelData->id[k], HU_MEMNUM_OVL, HEAP_MODEL));
        lbl_1_bss_1FC0->model[k].id = baseId;
        Hu3DModelAttrSet(baseId, 1);
        Hu3DModelCameraSet(baseId, 1);
        Hu3DModelPosSetV(baseId, &lbl_1_data_3D8[k]);
        Hu3DModelRotSet(baseId, 0.0f, -22.5f, 0.0f);
        lbl_1_bss_1FC0->model[k].pos.x = lbl_1_data_3D8[k].x;
        lbl_1_bss_1FC0->model[k].pos.y = lbl_1_data_3D8[k].y;
        lbl_1_bss_1FC0->model[k].pos.z = lbl_1_data_3D8[k].z;
        lbl_1_bss_1FC0->model[k].rot.x = 0.0f;
        lbl_1_bss_1FC0->model[k].rot.y = 22.5f;
        lbl_1_bss_1FC0->model[k].rot.z = 0.0f;
        Hu3DMotionTimeSet(baseId, 0.0f);
        Hu3DMotionSpeedSet(baseId, 0.25f);
        Hu3DModelAttrSet(baseId, 0x40000001);
        Hu3DModelLayerSet(baseId, 1);
        Hu3DModelAttrReset(baseId, 1);
    }
    for (i = 0; i < 3; i++) {
        p = &lbl_1_bss_1FC0->model[0x97 + i * 3];
        for (j = 0; j < 3; j++) {
            if (j == 0) {
                baseId = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_bss_1FC0->modelData->id[i + 0x2F], HU_MEMNUM_OVL, HEAP_MODEL));
                p->id = baseId;
            } else {
                p->id = Hu3DModelLink(baseId);
            }
            Hu3DModelAttrSet(p->id, 1);
            Hu3DMotionTimeSet(p->id, 50.0f);
            Hu3DMotionSpeedSet(p->id, 0.5f);
            Hu3DModelCameraSet(p->id, 1);
            Hu3DModelLayerSet(p->id, 1);
            p++;
        }
    }
    for (i = 0; i < 2; i++) {
        p = &lbl_1_bss_1FC0->model[0xA1 + i];
        p->id = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_bss_1FC0->modelData->cc[i], HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelAttrSet(p->id, 1);
        Hu3DMotionSpeedSet(p->id, 0.5f);
        Hu3DModelCameraSet(p->id, 1);
        Hu3DMotionTimeSet(p->id, 0.0f);
        Hu3DModelLayerSet(p->id, 7);
        p = &lbl_1_bss_1FC0->model[0xA3 + i * 2];
        for (j = 0; j < 2; j++) {
            if (j == 0) {
                baseId = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_bss_1FC0->modelData->d4[i], HU_MEMNUM_OVL, HEAP_MODEL));
                p->id = baseId;
            } else {
                p->id = Hu3DModelLink(baseId);
            }
            Hu3DModelAttrSet(p->id, 1);
            Hu3DMotionSpeedSet(p->id, 0.5f);
            Hu3DModelCameraSet(p->id, 1);
            Hu3DMotionTimeSet(p->id, 0.0f);
            Hu3DModelAttrSet(p->id, 0x40000001);
            Hu3DModelLayerSet(p->id, 3);
            p++;
        }
    }
    for (i = 0; i < 3; i++) {
        M550IDX3 modelIdx = lbl_1_rodata_158;
        M550IDX3 dataIdx = lbl_1_rodata_164;

        p = &lbl_1_bss_1FC0->model[modelIdx.v[i]];
        p->id = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_bss_1FC0->modelData->id[dataIdx.v[i]], HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelAttrSet(p->id, 1);
        Hu3DModelCameraSet(p->id, 1);
        if (i < 2) {
            Hu3DModelLayerSet(p->id, 4);
        } else {
            Hu3DModelLayerSet(p->id, 1);
        }
    }
    for (i = 0; i < 10; i++) {
        p = &lbl_1_bss_1FC0->model[0xA9 + i];
        if (i == 0) {
            baseId = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_bss_1FC0->modelData->e4, HU_MEMNUM_OVL, HEAP_MODEL));
            p->id = baseId;
        } else {
            p->id = Hu3DModelLink(baseId);
        }
        Hu3DModelAttrSet(p->id, 1);
        Hu3DModelCameraSet(p->id, 1);
        Hu3DModelLayerSet(p->id, 4);
    }
    for (i = 0; i < 2; i++) {
        p = &lbl_1_bss_1FC0->model[0xB3 + i * 2];
        for (j = 0; j < 2; j++) {
#ifdef TARGET_PC
            /* the slot still holds -1 here: the GC code pokes Hu3DData[-1] */
            if (p->id >= 0) {
                Hu3DModelAttrSet(p->id, 1);
            }
#else
            Hu3DModelAttrSet(p->id, 1);
#endif
            if (j == 0) {
                baseId = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_bss_1FC0->modelData->e8[i], HU_MEMNUM_OVL, HEAP_MODEL));
                p->id = baseId;
                lbl_1_data_420[i] = Hu3DJointMotion(p->id, HuDataSelHeapReadNum(lbl_1_data_0[i], HU_MEMNUM_OVL, HEAP_MODEL));
                Hu3DModelScaleSet(p->id, 2.2f, 2.2f, 2.2f);
            } else {
                p->id = Hu3DModelLink(baseId);
                Hu3DModelScaleSet(p->id, 1.0f, 1.0f, 1.0f);
            }
            Hu3DMotionSet(p->id, lbl_1_data_420[i]);
            Hu3DMotionSpeedSet(p->id, 1.0f);
            Hu3DModelAttrSet(p->id, 0x40000001);
            Hu3DModelCameraSet(p->id, 1);
            Hu3DModelLayerSet(p->id, 1);
            p++;
        }
    }
}

void fn_1_58F4(M550MOTIONSET *p0, M550MOTIONSET *p1)
{
    lbl_1_bss_1FC0->motionSet[0] = p0;
    lbl_1_bss_1FC0->motionSet[1] = p1;
}

void fn_1_5918(s16 *p0, s16 *p1)
{
    lbl_1_bss_1FC0->motionId[0] = p0;
    lbl_1_bss_1FC0->motionId[1] = p1;
}

void fn_1_593C(void)
{
    s32 i;

    for (i = 0; i < 2; i++) {
        lbl_1_bss_1FC0->model[i + 2].id = CharModelCreate(lbl_1_bss_1FC0->player[i].charNo, 1);
        Hu3DModelPosSet(lbl_1_bss_1FC0->model[i + 2].id, (f32)(i * 0x1A4 - 0xD2), 1000.0f, 0.0f);
        lbl_1_bss_1FC0->model[i + 2].pos.x = (f32)(i * 0x1A4 - 0xD2);
        lbl_1_bss_1FC0->model[i + 2].pos.y = 1000.0f;
        lbl_1_bss_1FC0->model[i + 2].pos.z = 0.0f;
        lbl_1_bss_1FC0->model[i + 2].rot.x = 0.0f;
        lbl_1_bss_1FC0->model[i + 2].rot.y = 90.0f + (f32)(i * 0xB4);
        lbl_1_bss_1FC0->model[i + 2].rot.z = 0.0f;
        Hu3DModelAttrSet(lbl_1_bss_1FC0->model[i + 2].id, 0x40000001);
        Hu3DModelCameraSet(lbl_1_bss_1FC0->model[i + 2].id, 1);
        Hu3DModelLayerSet(lbl_1_bss_1FC0->model[i + 2].id, 4);
    }
}

void fn_1_5BD8(OMOBJ *obj)
{
    s32 i;
    s8 absX;
    s8 stickX;
    s8 absY;
    s8 stickY;

    for (i = 0; i < 2; i++) {
        if (lbl_1_bss_1FC0->player[i].cpuLevel != 0) {
            memset(&lbl_1_bss_1FC0->player[i].btn, 0, 0x14);
        } else {
            lbl_1_bss_1FC0->player[i].btn = HuPadBtn[lbl_1_bss_1FC0->player[i].padNo];
            lbl_1_bss_1FC0->player[i].btnDown = HuPadBtnDown[lbl_1_bss_1FC0->player[i].padNo];
            lbl_1_bss_1FC0->player[i].btnRep = HuPadBtnRep[lbl_1_bss_1FC0->player[i].padNo];
            lbl_1_bss_1FC0->player[i].stkX = HuPadStkX[lbl_1_bss_1FC0->player[i].padNo];
            if (lbl_1_bss_1FC0->player[i].stkX >= 0) {
                absX = lbl_1_bss_1FC0->player[i].stkX;
            } else {
                absX = -lbl_1_bss_1FC0->player[i].stkX;
            }
            if (absX < 10) {
                stickX = 0;
            } else {
                stickX = lbl_1_bss_1FC0->player[i].stkX;
            }
            lbl_1_bss_1FC0->player[i].stkX = stickX;
            lbl_1_bss_1FC0->player[i].stkY = HuPadStkY[lbl_1_bss_1FC0->player[i].padNo];
            if (lbl_1_bss_1FC0->player[i].stkY >= 0) {
                absY = lbl_1_bss_1FC0->player[i].stkY;
            } else {
                absY = -lbl_1_bss_1FC0->player[i].stkY;
            }
            if (absY < 10) {
                stickY = 0;
            } else {
                stickY = lbl_1_bss_1FC0->player[i].stkY;
            }
            lbl_1_bss_1FC0->player[i].stkY = stickY;
            lbl_1_bss_1FC0->player[i].subStkX = HuPadSubStkX[lbl_1_bss_1FC0->player[i].padNo];
            lbl_1_bss_1FC0->player[i].subStkY = HuPadSubStkY[lbl_1_bss_1FC0->player[i].padNo];
            lbl_1_bss_1FC0->player[i].trigL = HuPadTrigL[lbl_1_bss_1FC0->player[i].padNo];
            lbl_1_bss_1FC0->player[i].trigR = HuPadTrigR[lbl_1_bss_1FC0->player[i].padNo];
            M550_DSTK(&lbl_1_bss_1FC0->player[i]) = HuPadDStk[lbl_1_bss_1FC0->player[i].padNo];
            M550_DSTKREP(&lbl_1_bss_1FC0->player[i]) = HuPadDStkRep[lbl_1_bss_1FC0->player[i].padNo];
        }
    }
}

void fn_1_6094(void)
{
    s16 i;
    s16 j;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 5; j++) {
            lbl_1_bss_1FC0->motionId[i][j] = CharMotionCreate(lbl_1_bss_1FC0->player[i].charNo, lbl_1_bss_1FC0->motionSet[i]->id[j]);
        }
    }
    for (i = 0; i < 2; i++) {
        lbl_1_bss_1FC0->motion[i].prevMotNo = -1;
        lbl_1_bss_1FC0->motion[i].speed = 1.0f;
        lbl_1_bss_1FC0->motion[i].prevSpeed = 1.0f;
    }
}

void fn_1_61E8(OMOBJ *obj)
{
    s16 i;
    M550MOTION *m;
    s16 modelIdx;

    for (i = 0; i < 2; i++) {
        m = &lbl_1_bss_1FC0->motion[i];
        modelIdx = i + 2;
        if (m->motNo != m->prevMotNo) {
            Hu3DMotionSet(lbl_1_bss_1FC0->model[modelIdx].id, lbl_1_bss_1FC0->motionId[i][m->motNo]);
            m->prevMotNo = m->motNo;
            m->maxTime = Hu3DMotionMaxTimeGet(lbl_1_bss_1FC0->model[modelIdx].id);
        }
        if (m->speed != m->prevSpeed) {
            Hu3DMotionSpeedSet(lbl_1_bss_1FC0->model[modelIdx].id, m->speed);
            m->prevSpeed = m->speed;
        }
        if (m->attrSet != m->prevAttrSet) {
            Hu3DModelAttrSet(lbl_1_bss_1FC0->model[modelIdx].id, m->attrSet);
            m->prevAttrSet = m->attrSet;
        }
        if (m->attrReset != 0) {
            Hu3DModelAttrReset(lbl_1_bss_1FC0->model[modelIdx].id, m->attrReset);
            m->attrReset = ~(m->attrReset | m->attrReset);
            m->prevAttrSet = m->attrSet &= m->attrReset;
            m->attrReset = 0;
        }
        if (m->time != m->prevTime) {
            Hu3DMotionTimeSet(lbl_1_bss_1FC0->model[modelIdx].id, m->time);
        }
        switch (m->shiftState) {
            case 0:
                break;
            case 1:
                Hu3DMotionShiftSet(lbl_1_bss_1FC0->model[modelIdx].id, lbl_1_bss_1FC0->motionId[i][m->shiftMotNo], m->shiftStart, m->shiftEnd, m->shiftAttr);
                m->shiftMaxTime = Hu3DMotionShiftMaxTimeGet(lbl_1_bss_1FC0->model[modelIdx].id);
                m->shiftState = 2;
                break;
            case 2:
                m->shiftStart -= 1.0f;
                if (m->shiftStart <= 0.0f) {
                    m->shiftState = 3;
                }
                break;
            case 3:
                m->maxTime = Hu3DMotionMaxTimeGet(lbl_1_bss_1FC0->model[modelIdx].id);
                m->prevTime = m->time;
                m->shiftState = 0;
                m->prevMotNo = m->motNo = m->shiftMotNo;
                break;
        }
        m->time = Hu3DMotionTimeGet(lbl_1_bss_1FC0->model[modelIdx].id);
        m->prevTime = m->time;
        m->endF = Hu3DMotionEndCheck(lbl_1_bss_1FC0->model[modelIdx].id);
        if (m->endF != 0 && m->chainF != 0) {
            m->endF = 0;
            m->chainF = 0;
            m->motNo = m->chainMotNo;
            m->time = m->chainTime;
            m->attrSet = m->chainAttrSet;
            m->attrReset = m->chainAttrReset;
        }
    }
}

void fn_1_65AC(OMOBJ *obj)
{
    s32 i;

    for (i = 0; i < 7; i++) {
        lbl_1_data_424[i]();
    }
}

void fn_1_6600(void)
{
    s32 modelIdx[5] = { 0x97, 0x9A, 0x9D, 0x98, 0x9E };
    f32 angle[5] = { 152.0f, 191.0f, 207.0f, 176.0f, 170.0f };
    f32 radius[5] = { 5000.0f, 3200.0f, 5500.0f, 7000.0f, 6800.0f };
    f32 height[5] = { -800.0f, -900.0f, -800.0f, -300.0f, -400.0f };
    s32 i;
    s32 j;
    M550FX *fx;
    M550MODEL *model;

    for (i = 0; i < 5; i++) {
        fx = &lbl_1_bss_1FC0->fx[i];
        model = &lbl_1_bss_1FC0->model[modelIdx[i]];
        if (fx->state == 0) {
            fx->state = 1;
            fx->cnt[0] = 0;
            fx->cnt[1] = 0;
            fx->cnt[2] = 0;
            fx->f2C = angle[i];
            fx->f30 = radius[i];
            fx->f34 = (f32)(i * 0x2D);
            fx->f38 = height[i];
            model->pos.x = fx->f30 * sin(3.141592653589793 * fx->f2C / 180.0);
            model->pos.z = fx->f30 * cos(3.141592653589793 * fx->f2C / 180.0);
            model->pos.y = (1000.0f - (f32)(i * 200)) + 200.0 * sin(3.141592653589793 * fx->f34 / 180.0);
            Hu3DModelAttrReset(model->id, 1);
            Hu3DMotionTimeSet(model->id, 50.0f);
            Hu3DMotionSpeedSet(model->id, 0.5f);
            Hu3DModelPosSetV(model->id, &model->pos);
            if (i == 3) {
                Hu3DModelScaleSet(model->id, 0.75f, 0.75f, 0.75f);
            } else if (i == 4) {
                Hu3DModelScaleSet(model->id, 1.8f, 1.8f, 1.8f);
            } else {
                Hu3DModelScaleSet(model->id, 0.9f, 0.9f, 0.9f);
            }
        }
        for (j = 0; j < 3; j++) {
            fx->cnt[j]++;
            switch (j) {
                case 0:
                    if (fx->cnt[j] >= 0x10) {
                        fx->cnt[j] = 0;
                        fx->f2C += 0.001;
                    }
                    break;
                case 1:
                    if (fx->cnt[j] >= 0x64) {
                        fx->cnt[j] = 0;
                        Hu3DMotionTimeSet(model->id, 50.0f);
                    }
                    break;
                case 2:
                    if (fx->cnt[j] >= 4) {
                        fx->cnt[j] = 0;
                        fx->f34 += 1.0f;
                    }
                    break;
            }
        }
        model->pos.x = fx->f30 * sin(3.141592653589793 * fx->f2C / 180.0);
        model->pos.z = fx->f30 * cos(3.141592653589793 * fx->f2C / 180.0);
        model->pos.y = (1000.0f + fx->f38) + 200.0 * sin(3.141592653589793 * fx->f34 / 180.0);
    }
}

void fn_1_6B6C(void)
{
    s32 modelIdx[4] = { 0xA3, 0xA5, 0xA4, 0xA6 };
    s32 i;
    M550FX *fx;
    M550MODEL *model;
    M550MODEL *first;

    for (i = 0; i < 4; i++) {
        fx = &lbl_1_bss_1FC0->fx[i + 5];
        model = &lbl_1_bss_1FC0->model[modelIdx[i]];
        if (fx->state == 0) {
            fx->state = 1;
            fx->cnt[0] = 0;
            model->pos.x = 0.0f;
            model->pos.y = 0.0f;
            model->pos.z = 0.0f;
            model->rot.y = (f32)(i * 0x1E - 0xF);
            Hu3DModelAttrReset(model->id, 1);
            Hu3DMotionTimeSet(model->id, (f32)(i * 0x19));
            Hu3DMotionSpeedSet(model->id, 0.5f);
            Hu3DModelPosSetV(model->id, &model->pos);
            Hu3DModelRotSetV(model->id, &model->rot);
            Hu3DModelAttrSet(model->id, 0x40000001);
        }
        fx->cnt[0]++;
        model->rot.y -= 0.01;
        if (model->rot.y <= -45.0f) {
            model->rot.y = 75.0f;
        }
    }
    first = lbl_1_bss_1FC0->model;
    first->rot.y -= 0.005f;
}

void fn_1_6D8C(void)
{
    lbl_1_bss_1FC0->fx[9].cnt[0] = -1;
}

void fn_1_6DA4(void)
{
    s32 idx[1] = { 0xA7 };
    M550FX *fx;
    M550MODEL *model;

    fx = &lbl_1_bss_1FC0->fx[9];
    model = &lbl_1_bss_1FC0->model[idx[0]];
    if (fx->cnt[0] >= -1) {
        if (fx->cnt[0] == -1) {
            Hu3DModelAttrSet(model->id, 1);
            fx->cnt[0]--;
            return;
        }
        if (lbl_1_bss_1FC0->fx[9].state == 0) {
            lbl_1_bss_1FC0->fx[9].state = 1;
            fx->cnt[0] = 0;
            fx->cnt[1] = 0;
            fx->cnt[2] = 0;
            fx->work10 = 0;
            fx->work18 = 0;
            model->pos.x = -1100.0f;
            model->pos.y = 1850.0f;
            model->pos.z = -2800.0f;
            model->rot.x = 0.0f;
            model->rot.z = 0.0f;
            Hu3DModelPosSetV(model->id, &model->pos);
            Hu3DModelAttrReset(model->id, 1);
            Hu3DMotionTimeSet(model->id, 0.0f);
            Hu3DMotionSpeedSet(model->id, 1.0f);
            fx->f2C = 1.0f;
            fn_1_984(0xA7, fx->f2C);
            return;
        }
        fx->cnt[0] = (fx->cnt[0] + 1) % 360;
        model->rot.z = 2.0 * sin(3.141592653589793 * fx->cnt[0] / 180.0);
        switch (fx->work18) {
            case 0:
                fx->cnt[1]++;
                fx->f2C = 1.0 - 0.4 * sin(3.141592653589793 * ((fx->cnt[1] * 0x5A) / 16) / 180.0);
                fn_1_984(0xA7, fx->f2C);
                if (fx->cnt[1] == 0x10) {
                    fx->cnt[1] = 0;
                    fx->cnt[2] = (rand8() % 60) + 0x40;
                    fx->work18++;
                    return;
                }
                break;
            case 1:
                if (fx->cnt[1]++ >= fx->cnt[2]) {
                    fn_1_984(0xA7, 1.0f);
                    fx->work10 = (fx->work10 + 1) % 6;
                    if (fx->work10 == 1 || fx->work10 == 4) {
                        fx->work18 = 0;
                    } else {
                        fx->work18++;
                    }
                    fx->cnt[1] = 0;
                    return;
                }
                break;
            case 2:
                if (fx->cnt[1]++ >= 6) {
                    fn_1_984(0xA7, 0.6f);
                    fx->work18 = 0;
                    fx->cnt[1] = 0;
                }
                break;
        }
    }
}

void fn_1_719C(void)
{
    s32 modelIdx[6] = { 0xA8, 0xB3, 0xB5, 0xB4, 0xB6, 0xA9 };
    f32 radius[5] = { 2900.0f, 700.0f, 700.0f, 300.0f, 300.0f };
    f32 angle[5] = { 90.0f, 180.0f, 0.0f, 90.0f, 270.0f };
    f32 step[5] = { 0.3f, 0.5f, 0.5f, 0.5f, 0.5f };
    f32 pos[6] = { 1000.0f, 1200.0f, -6600.0f, -550.0f, 550.0f, -3120.0f };
    s32 i;
    M550FX *fx;
    M550MODEL *model;
    M550MODEL *trail;
    f64 sinVal;

    for (i = 0; i < 5; i++) {
        fx = &lbl_1_bss_1FC0->fx[i + 0xA];
        model = &lbl_1_bss_1FC0->model[modelIdx[i]];
        if (fx->state == 0) {
            fx->state = 1;
            fx->cnt[0] = 0;
            Hu3DModelAttrReset(model->id, 1);
            Hu3DMotionTimeSet(model->id, 0.0f);
            if (i == 0) {
                Hu3DMotionSpeedSet(model->id, 4.0f);
            } else {
                Hu3DMotionSpeedSet(model->id, 1.0f);
            }
            Hu3DModelAttrSet(model->id, 0x40000001);
            fx->f2C = radius[i];
            fx->f30 = angle[i];
            fx->f34 = step[i];
        }
        fx->f30 += fx->f34;
        if (i == 0) {
            if (fx->f30 > 270.0f) {
                fx->f30 = 0.0f;
                fx->work1C = 0;
            }
            if (fx->f30 > 0.0f) {
                if (fx->f30 <= 90.0f) {
                    fx->f30 -= 3.0f * fx->f34 * 0.25f;
                }
            }
        }
        if (fx->f34 >= 0.0f) {
            model->rot.y = 90.0f + fx->f30;
        } else {
            model->rot.y = 270.0f + fx->f30;
        }
        if (i < 3) {
            if (i != 0) {
                if (i >= 0) {
                    model->pos.x = pos[0] + fx->f2C * sin(3.141592653589793 * fx->f30 / 180.0);
                    model->pos.z = pos[2] + fx->f2C * cos(3.141592653589793 * fx->f30 / 180.0);
                    model->pos.y = pos[1];
                }
            } else {
                if (fx->work18 == 0) {
                    model->pos.x = fx->f2C * sin(3.141592653589793 * fx->f30 / 180.0);
                    model->pos.z = fx->f2C * cos(3.141592653589793 * fx->f30 / 180.0);
                    sinVal = sin(3.141592653589793 * (180.0f + fx->f30) / 180.0);
                    model->pos.y = 400.0 + 5000.0 * sin(0.20943951023931953) * sinVal;
                    model->rot.x = 12.0 * sin(3.141592653589793 * (90.0f + fx->f30) / 180.0);
                    if (fx->work1C == 0 && fx->f30 > 180.0f) {
                        fx->work18++;
                        fx->f3C = model->pos.x + 600.0 * cos(3.141592653589793 * (90.0f + model->rot.x) / 180.0);
                        fx->f40 = model->pos.y + 600.0 * sin(3.141592653589793 * (90.0f + model->rot.x) / 180.0);
                        fx->f44 = (90.0f + model->rot.x) - 180.0f;
                        fx->work10 = 0;
                        fx->f34 = 0.0f;
                        fx->work1C = 1;
                    }
                } else {
                    fx->f44 -= 1.5;
                    model->rot.x -= 1.5;
                    model->pos.x = fx->f3C + 600.0 * cos(3.141592653589793 * fx->f44 / 180.0);
                    model->pos.y = fx->f40 + 600.0 * sin(3.141592653589793 * fx->f44 / 180.0);
                    if (fx->work10++ >= 0xEF) {
                        fx->f34 = 0.3f;
                        fx->work18 = 0;
                    }
                }
                fx->cnt[0]++;
                if (fx->cnt[0] >= 0xF) {
                    fx->cnt[0] = 0;
                    trail = &lbl_1_bss_1FC0->model[modelIdx[5] + fx->cnt[1]];
                    Hu3DModelAttrReset(trail->id, 1);
                    Hu3DMotionTimeSet(trail->id, 0.0f);
                    Hu3DMotionSpeedSet(trail->id, 1.0f);
                    if (fx->work18 == 0) {
                        trail->pos.x = fx->f2C * sin(3.141592653589793 * (fx->f30 - fx->f34) / 180.0);
                        trail->pos.z = fx->f2C * cos(3.141592653589793 * (fx->f30 - fx->f34) / 180.0);
                        sinVal = sin(3.141592653589793 * (180.0f + (fx->f30 - fx->f34)) / 180.0);
                        trail->pos.y = 400.0 + 5000.0 * sin(0.20943951023931953) * sinVal;
                    } else {
                        trail->pos.x = fx->f3C + 600.0 * cos(3.141592653589793 * (1.5f + fx->f44) / 180.0);
                        trail->pos.y = fx->f40 + 600.0 * sin(3.141592653589793 * (1.5f + fx->f44) / 180.0);
                        trail->pos.z = model->pos.z;
                    }
                    Hu3DModelPosSetV(trail->id, &trail->pos);
                    fx->cnt[1] = (fx->cnt[1] + 1) % 10;
                }
            }
        } else if (i < 5) {
            model->pos.x = pos[3] + fx->f2C * sin(3.141592653589793 * fx->f30 / 180.0);
            model->pos.z = pos[5] + fx->f2C * cos(3.141592653589793 * fx->f30 / 180.0);
            model->pos.y = pos[4];
        }
    }
}

void fn_1_7C00(void)
{
    lbl_1_bss_1FC0->fx[15].cnt[0] = -1;
    Hu3DModelAttrSet(lbl_1_bss_1FC0->model[0xA1].id, 1);
}

void fn_1_7C4C(void)
{
    s32 idx[1] = { 0xA1 };
    M550FX *fx;
    M550MODEL *model;

    fx = &lbl_1_bss_1FC0->fx[15];
    model = &lbl_1_bss_1FC0->model[idx[0]];
    if (fx->cnt[0] >= -1) {
        if (fx->cnt[0] == -1) {
            Hu3DModelAttrSet(model->id, 1);
            fx->cnt[0]--;
        }
        if (lbl_1_bss_1FC0->fx[15].state == 0) {
            if (fx->cnt[0]++ > 0x320) {
                lbl_1_bss_1FC0->fx[15].state = 1;
                fx->cnt[0] = 0;
                Hu3DModelAttrReset(model->id, 1);
                Hu3DMotionTimeSet(model->id, 50.0f);
                Hu3DMotionSpeedSet(model->id, 0.5f);
                Hu3DModelScaleSet(model->id, 2.0f, 2.0f, 2.0f);
                model->pos.x = -2000.0f;
                model->pos.z = 2000.0f;
                model->pos.y = 1200.0f;
                Hu3DModelPosSetV(model->id, &model->pos);
            }
        }
        if (lbl_1_bss_1FC0->fx[15].state == 1) {
            fx->cnt[0]++;
            if (fx->cnt[0] >= 0x63) {
                Hu3DMotionTimeSet(model->id, 51.0f);
                fx->cnt[0] = 0;
            }
            model->pos.x += 5.0f;
            if (model->pos.x > 4000.0f) {
                lbl_1_bss_1FC0->fx[15].state = 2;
                fx->cnt[0] = 0;
                fx->cnt[1] = (rand8() % 5) * 0x1E + 0x258;
            }
        }
        if (lbl_1_bss_1FC0->fx[15].state == 2) {
            if (fx->cnt[0]++ >= fx->cnt[1]) {
                model->pos.x = -2000.0f;
                lbl_1_bss_1FC0->fx[15].state = 1;
            }
        }
    }
}

void fn_1_7E94(void)
{
    Hu3DModelAttrSet(lbl_1_bss_1FC0->model[0xA2].id, 1);
    lbl_1_bss_1FC0->fx[16].state = -1;
}

void fn_1_7EE0(void)
{
    s32 idx[1] = { 0xA2 };
    M550FX *fx;
    M550MODEL *model;

    fx = &lbl_1_bss_1FC0->fx[16];
    model = &lbl_1_bss_1FC0->model[idx[0]];
    if (lbl_1_bss_1FC0->fx[16].state >= 0) {
        switch (lbl_1_bss_1FC0->fx[16].state) {
            case 0:
                if (fx->cnt[0]++ > 0x12C) {
                    fx->cnt[0] = -0xC8;
                    lbl_1_bss_1FC0->fx[16].state = 1;
                    Hu3DModelAttrReset(model->id, 1);
                    Hu3DMotionTimeSet(model->id, 0.0f);
                    Hu3DMotionSpeedSet(model->id, 0.25f);
                    model->pos.x = -4356.0f;
                    model->pos.z = -13000.0f;
                    model->pos.y = 0.0f;
                    Hu3DModelPosSetV(model->id, &model->pos);
                    return;
                }
                break;
            case 1:
                fx->cnt[0]++;
                if (fx->cnt[0] >= 0xC8) {
                    fx->cnt[0] = 0;
                    Hu3DMotionTimeSet(model->id, 51.0f);
                }
                model->pos.x += 14.85 * sin(0.39269908169872414);
                model->pos.z += 14.85 * cos(0.39269908169872414);
                if (model->pos.z >= 2000.0f) {
                    lbl_1_bss_1FC0->fx[16].state = 2;
                    fx->cnt[0] = 0;
                    fx->cnt[1] = (rand8() % 8) * 0x1E + 0x12C;
                    return;
                }
                break;
            case 2:
                if (fx->cnt[0]++ >= fx->cnt[1]) {
                    lbl_1_bss_1FC0->fx[16].state = 1;
                    Hu3DMotionTimeSet(model->id, 0.0f);
                    model->pos.x = (rand8() % 4) * 0x1F4 - 0x1104;
                    model->pos.z = -13000.0f;
                    fx->cnt[0] = -0xC8;
                }
                break;
        }
    }
}

void fn_1_8190(void)
{
    HuVecF pos[4] = {
        { -700.0f, -1600.0f, 1100.0f },
        { 1100.0f, -1100.0f, -500.0f },
        { 800.0f, -2700.0f, 1200.0f },
        { -800.0f, -500.0f, -600.0f },
    };
    f32 scale[4] = { 1.0f, 1.2f, 0.7f, 1.5f };
    s32 modelIdx[4] = { 0x99, 0x9B, 0x9C, 0x9F };
    s32 i;
    M550MODEL *model;

    for (i = 0; i < 4; i++) {
        model = &lbl_1_bss_1FC0->model[modelIdx[i]];
        Hu3DModelAttrReset(model->id, 1);
        Hu3DMotionSpeedSet(model->id, 1.0f);
        Hu3DMotionTimeSet(model->id, 50.0f);
        model->pos.x = pos[i].x;
        model->pos.y = pos[i].y;
        model->pos.z = pos[i].z;
        Hu3DModelPosSetV(model->id, &model->pos);
        Hu3DModelScaleSet(model->id, scale[i], scale[i], scale[i]);
    }
}

void fn_1_8350(void)
{
    s32 modelIdx[4] = { 0x99, 0x9B, 0x9C, 0x9F };
    s32 i;
    s16 *modelId;

    for (i = 0; i < 4; i++) {
        modelId = &lbl_1_bss_1FC0->model[modelIdx[i]].id;
        if (Hu3DMotionTimeGet(*modelId) >= 99.0f) {
            Hu3DMotionTimeSet(*modelId, 50.0f);
        }
    }
}
