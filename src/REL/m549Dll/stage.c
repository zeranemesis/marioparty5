#include "REL/m549Dll.h"
#include "game/gamework.h"
#include "game/pad.h"
#include "game/charman.h"
#include "string.h"

float Hu3DMotionMaxTimeGet(HU3DMODELID modelId);
s32 rand8(void);

/* sets invAlpha of every material of a model */
#define MODEL_MAT_ALPHA(modelId, alpha)                                                    \
    do {                                                                                   \
        HU3DMODEL *mdl_ = &Hu3DData[(modelId)];                                            \
        HSFDATA *hsf_ = mdl_->hsf;                                                         \
        HSFMATERIAL *mat_ = hsf_->material;                                                \
        s32 k_;                                                                            \
        for (k_ = 0; k_ < hsf_->materialNum; k_++, mat_++) {                          \
            mat_->invAlpha = (alpha);                                                      \
        }                                                                                  \
    } while (0)

M549State *lbl_1_bss_1608;

s32 lbl_1_data_528[2] = { 0x6F0019, 0x6F001A };
void (*lbl_1_data_530[5])(void) = { fn_1_12ED4, fn_1_1328C, fn_1_139F4, fn_1_144D8, fn_1_1476C };

void fn_1_11BC0(M549State *stage, M549StageModel *models, s32 *dataNums)
{
    s16 j;
    s16 i;

    lbl_1_bss_1608 = stage;
    memset(lbl_1_bss_1608, 0, sizeof(M549State));
    lbl_1_bss_1608->models = models;
    lbl_1_bss_1608->dataNums = dataNums;
    i = 0;
    do {
        for (j = 0; j < 4; j++) {
            if (i == GwPlayerConf[j].grp) {
                lbl_1_bss_1608->chars[i].playerNo = j;
                i++;
                if (i >= 2) {
                    break;
                }
            }
        }
    } while (i < 2);
    for (j = 0; j < 2; j++) {
        lbl_1_bss_1608->chars[j].charNo = GwPlayerConf[lbl_1_bss_1608->chars[j].playerNo].charNo;
        lbl_1_bss_1608->chars[j].padNo = GwPlayerConf[lbl_1_bss_1608->chars[j].playerNo].padNo;
        if (GwPlayerConf[lbl_1_bss_1608->chars[j].playerNo].type != 0) {
            lbl_1_bss_1608->chars[j].cpuLevel = GwPlayerConf[lbl_1_bss_1608->chars[j].playerNo].dif + 1;
        }
    }
}

void fn_1_11E0C(void)
{
    s16 motId[2] = { 0, 0 };
    s32 charIdx[2] = { 0x33, 0x34 };
    s32 charNum[2] = { 0x10, 0x11 };
    M549StageModel *m;
    s16 first;
    s32 i;
    s32 j;

    for (i = 0; i < 3; i++) {
        m = &lbl_1_bss_1608->models[i * 3 + 0x24];
        for (j = 0; j < 3; j++, m++) {
            if (j == 0) {
                m->id = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_bss_1608->dataNums[i + 9], 0x10000000, 2));
                first = m->id;
            } else {
                m->id = Hu3DModelLink(first);
            }
            Hu3DModelAttrSet(m->id, 1);
            Hu3DMotionTimeSet(m->id, 50.0f);
            Hu3DMotionSpeedSet(m->id, 0.5f);
            Hu3DModelCameraSet(m->id, 1);
            Hu3DModelLayerSet(m->id, 2);
        }
    }
    for (i = 0; i < 2; i++) {
        m = &lbl_1_bss_1608->models[i + 0x2D];
        m->id = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_bss_1608->dataNums[i + 12], 0x10000000, 2));
        Hu3DModelAttrSet(m->id, 1);
        Hu3DMotionSpeedSet(m->id, 0.5f);
        Hu3DModelCameraSet(m->id, 2);
        Hu3DMotionTimeSet(m->id, 0.0f);
        Hu3DModelLayerSet(m->id, 7);
        m = &lbl_1_bss_1608->models[i * 2 + 0x2F];
        for (j = 0; j < 2; j++, m++) {
            if (j == 0) {
                m->id = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_bss_1608->dataNums[i + 14], 0x10000000, 2));
                first = m->id;
            } else {
                m->id = Hu3DModelLink(first);
            }
            Hu3DModelAttrSet(m->id, 1);
            Hu3DMotionSpeedSet(m->id, 0.5f);
            Hu3DModelCameraSet(m->id, 1);
            Hu3DMotionTimeSet(m->id, 0.0f);
            Hu3DModelAttrSet(m->id, 0x40000001);
            Hu3DModelLayerSet(m->id, 3);
        }
    }
    for (i = 0; i < 2; i++) {
        m = &lbl_1_bss_1608->models[charIdx[i]];
        m->id = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_bss_1608->dataNums[charNum[i]], 0x10000000, 2));
        Hu3DModelAttrSet(m->id, 1);
        Hu3DModelCameraSet(m->id, 1);
        if (i < 1) {
            Hu3DModelLayerSet(m->id, 4);
        } else {
            Hu3DModelLayerSet(m->id, 2);
        }
    }
    for (i = 0; i < 10; i++) {
        m = &lbl_1_bss_1608->models[i + 0x35];
        if (i == 0) {
            m->id = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_bss_1608->dataNums[18], 0x10000000, 2));
            first = m->id;
        } else {
            m->id = Hu3DModelLink(first);
        }
        Hu3DModelAttrSet(m->id, 1);
        Hu3DModelCameraSet(m->id, 1);
        Hu3DModelLayerSet(m->id, 4);
    }
    for (i = 0; i < 2; i++) {
        m = &lbl_1_bss_1608->models[i * 2 + 0x3F];
        for (j = 0; j < 2; j++, m++) {
            Hu3DModelAttrSet(m->id, 1);
            if (j == 0) {
                m->id = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_bss_1608->dataNums[i + 19], 0x10000000, 2));
                first = m->id;
                motId[i] = Hu3DJointMotion(m->id, HuDataSelHeapReadNum(lbl_1_data_528[i], 0x10000000, 2));
                Hu3DModelScaleSet(m->id, 2.2f, 2.2f, 2.2f);
            } else {
                m->id = Hu3DModelLink(first);
                Hu3DModelScaleSet(m->id, 1.0f, 1.0f, 1.0f);
            }
            Hu3DMotionSet(m->id, motId[i]);
            Hu3DMotionSpeedSet(m->id, 1.0f);
            Hu3DModelAttrSet(m->id, 0x40000001);
            Hu3DModelCameraSet(m->id, 1);
            Hu3DModelLayerSet(m->id, 2);
        }
    }
    m = &lbl_1_bss_1608->models[0x43];
    m->id = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_bss_1608->dataNums[21], 0x10000000, 2));
    Hu3DModelAttrSet(m->id, 1);
    Hu3DMotionTimeSet(m->id, 50.0f);
    Hu3DMotionSpeedSet(m->id, 0.5f);
    Hu3DModelCameraSet(m->id, 1);
    Hu3DModelLayerSet(m->id, 4);
}

void fn_1_12450(s16 idx, s32 *data)
{
    lbl_1_bss_1608->motionData[idx] = data;
}

void fn_1_12470(s16 idx, s16 *ids)
{
    lbl_1_bss_1608->motionIds[idx] = ids;
}

void fn_1_12490(void)
{
}

void fn_1_12494(void)
{
    s32 i;
    s8 absX;
    s8 newX;

    for (i = 0; i < 2; i++) {
        if (lbl_1_bss_1608->chars[i].cpuLevel != 0) {
            memset(&lbl_1_bss_1608->chars[i].btn, 0, 0x14);
        } else {
            lbl_1_bss_1608->chars[i].btn = HuPadBtn[lbl_1_bss_1608->chars[i].padNo];
            lbl_1_bss_1608->chars[i].btnDown = HuPadBtnDown[lbl_1_bss_1608->chars[i].padNo];
            lbl_1_bss_1608->chars[i].btnRep = HuPadBtnRep[lbl_1_bss_1608->chars[i].padNo];
            lbl_1_bss_1608->chars[i].stkX = (s8)HuPadStkX[lbl_1_bss_1608->chars[i].padNo];
            if (lbl_1_bss_1608->chars[i].stkX >= 0) {
                absX = lbl_1_bss_1608->chars[i].stkX;
            } else {
                absX = -lbl_1_bss_1608->chars[i].stkX;
            }
            if (absX < 10) {
                newX = 0;
            } else {
                newX = lbl_1_bss_1608->chars[i].stkX;
            }
            lbl_1_bss_1608->chars[i].stkX = newX;
            lbl_1_bss_1608->chars[i].stkY = (s8)HuPadStkY[lbl_1_bss_1608->chars[i].padNo];
            lbl_1_bss_1608->chars[i].subStkX = (s8)HuPadSubStkX[lbl_1_bss_1608->chars[i].padNo];
            lbl_1_bss_1608->chars[i].subStkY = (s8)HuPadSubStkY[lbl_1_bss_1608->chars[i].padNo];
            lbl_1_bss_1608->chars[i].trigL = HuPadTrigL[lbl_1_bss_1608->chars[i].padNo];
            lbl_1_bss_1608->chars[i].trigR = HuPadTrigR[lbl_1_bss_1608->chars[i].padNo];
            lbl_1_bss_1608->chars[i].dstk = HuPadDStk[lbl_1_bss_1608->chars[i].padNo];
            lbl_1_bss_1608->chars[i].dstkRep = HuPadDStkRep[lbl_1_bss_1608->chars[i].padNo];
        }
    }
}

void fn_1_128AC(void)
{
    s16 i;
    s16 j;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 10; j++) {
            lbl_1_bss_1608->motionIds[i][j] = CharMotionCreate(lbl_1_bss_1608->chars[i].charNo, lbl_1_bss_1608->motionData[i][j]);
        }
    }
    for (i = 0; i < 2; i++) {
        lbl_1_bss_1608->slots[i].motion = 0;
        lbl_1_bss_1608->slots[i].id = 0;
        lbl_1_bss_1608->slots[i].speed = 0.0f;
        lbl_1_bss_1608->slots[i].curSpeed = 0.0f;
        lbl_1_bss_1608->slots[i].attrSet = 0;
        lbl_1_bss_1608->slots[i].attrCur = 0;
    }
}

void fn_1_12A60(void)
{
    s16 i;
    s16 idx;
    M549StageSlot *slot;

    for (i = 0; i < 2; i++) {
        slot = &lbl_1_bss_1608->slots[i];
        idx = i + 0x44;
        if (slot->motion != slot->id) {
            Hu3DMotionSet(lbl_1_bss_1608->models[idx].id, lbl_1_bss_1608->motionIds[i][slot->motion]);
            slot->id = slot->motion;
            slot->maxTime = Hu3DMotionMaxTimeGet(lbl_1_bss_1608->models[idx].id);
        }
        if (slot->speed != slot->curSpeed) {
            Hu3DMotionSpeedSet(lbl_1_bss_1608->models[idx].id, slot->speed);
            slot->curSpeed = slot->speed;
        }
        if (slot->attrSet != slot->attrCur) {
            Hu3DModelAttrSet(lbl_1_bss_1608->models[idx].id, slot->attrSet);
            slot->attrCur = slot->attrSet;
        }
        if (slot->attrReset != 0) {
            Hu3DModelAttrReset(lbl_1_bss_1608->models[idx].id, slot->attrReset);
            slot->attrReset = ~slot->attrReset;
            slot->attrSet = slot->attrSet & slot->attrReset;
            slot->attrCur = slot->attrSet;
            slot->attrReset = 0;
        }
        if (slot->time != slot->curTime) {
            Hu3DMotionTimeSet(lbl_1_bss_1608->models[idx].id, slot->time);
        }
        switch (slot->shiftState) {
        case 0:
            break;
        case 1:
            Hu3DMotionShiftSet(lbl_1_bss_1608->models[idx].id, lbl_1_bss_1608->motionIds[i][slot->shiftMotion],
                               slot->shiftStart, slot->shiftEnd, slot->shiftAttr);
            slot->shiftMax = Hu3DMotionShiftMaxTimeGet(lbl_1_bss_1608->models[idx].id);
            slot->shiftState = 2;
            break;
        case 2:
            slot->shiftCur = Hu3DMotionShiftTimeGet(lbl_1_bss_1608->models[idx].id);
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
        slot->time = Hu3DMotionTimeGet(lbl_1_bss_1608->models[idx].id);
        slot->curTime = slot->time;
        slot->endFlag = Hu3DMotionEndCheck(lbl_1_bss_1608->models[idx].id);
        if (slot->endFlag != 0 && slot->endReq != 0) {
            slot->endFlag = 0;
            slot->endReq = 0;
            slot->motion = slot->retMotion;
            slot->time = slot->retTime;
            slot->attrSet = slot->retAttrSet;
            slot->attrReset = slot->retAttrReset;
        }
    }
}

void fn_1_12E18(s32 modelId, f32 alpha)
{
    HU3DMODEL *mdl = &Hu3DData[modelId];
    HSFDATA *hsf = mdl->hsf;
    HSFMATERIAL *mat = hsf->material;
    s32 i;

    for (i = 0; i < mdl->hsf->materialNum; i++, mat++) {
        mat->invAlpha = alpha;
    }
}

void fn_1_12E80(void)
{
    s32 i;

    for (i = 0; i < 5; i++) {
        lbl_1_data_530[i]();
    }
}

void fn_1_12ED4(void)
{
    s32 idx[9] = { 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x43 };
    HuVecF pos[9] = {
        { -2300.0f, -1000.0f, -3200.0f },
        { 4500.0f, -900.0f, -7700.0f },
        { -4100.0f, -500.0f, -7700.0f },
        { 2300.0f, -1000.0f, -2800.0f },
        { 900.0f, -200.0f, -5700.0f },
        { -2400.0f, -1000.0f, -4700.0f },
        { 1600.0f, -700.0f, -3900.0f },
        { -1600.0f, -1000.0f, -5800.0f },
        { -100.0f, -1100.0f, -11200.0f },
    };
    f32 scale[9] = { 2.0f, 1.5f, 1.5f, 1.0f, 1.0f, 1.2f, 1.8f, 1.5f, 1.5f };
    M549AnimBlock *blk;
    M549StageModel *m;
    s32 i;
    s32 j;

    for (i = 0; i < 9; i++) {
        blk = &lbl_1_bss_1608->anim[i];
        m = &lbl_1_bss_1608->models[idx[i]];
        if (blk->init == 0) {
            blk->init = 1;
            blk->t[0] = 0;
            blk->t[1] = 0;
            blk->t[2] = 0;
            m->x = pos[i].x;
            m->z = pos[i].z;
            m->y = pos[i].y;
            blk->f34 = i * 40;
            Hu3DModelAttrReset(m->id, 1);
            Hu3DMotionTimeSet(m->id, 50.0f);
            Hu3DMotionSpeedSet(m->id, 0.5f);
            Hu3DModelPosSetV(m->id, (HuVecF *)&m->x);
            Hu3DModelScaleSet(m->id, scale[i], scale[i], scale[i]);
        }
        for (j = 0; j < 3; j++) {
            blk->t[j]++;
            switch (j) {
            case 0:
                if (blk->t[j] >= 16) {
                    blk->t[j] = 0;
                    blk->f2C += 0.001;
                }
                break;
            case 1:
                if (blk->t[j] >= 100) {
                    blk->t[j] = 0;
                    Hu3DMotionTimeSet(m->id, 50.0f);
                }
                break;
            case 2:
                if (blk->t[j] >= 4) {
                    blk->t[j] = 0;
                    blk->f34 += 1.0f;
                }
                break;
            }
        }
        m->y = pos[i].y + 200.0 * HuSin(blk->f34);
    }
}

void fn_1_1328C(void)
{
    s32 idx[4] = { 0x2F, 0x31, 0x30, 0x32 };
    M549AnimBlock *blk;
    M549StageModel *m;
    s32 i;

    for (i = 0; i < 4; i++) {
        blk = &lbl_1_bss_1608->anim[i + 9];
        m = &lbl_1_bss_1608->models[idx[i]];
        if (blk->init == 0) {
            blk->init = 1;
            blk->t[0] = 0;
            m->x = 0.0f;
            m->y = 0.0f;
            m->z = 0.0f;
            m->rotY = i * 30 - 15;
            Hu3DModelAttrReset(m->id, 1);
            Hu3DMotionTimeSet(m->id, i * 25);
            Hu3DMotionSpeedSet(m->id, 0.5f);
            Hu3DModelPosSetV(m->id, (HuVecF *)&m->x);
            Hu3DModelRotSetV(m->id, (HuVecF *)&m->rotX);
            Hu3DModelAttrSet(m->id, 0x40000001);
        }
        blk->t[0]++;
        m->rotY -= 0.01;
        if (m->rotY <= -45.0f) {
            m->rotY = 75.0f;
        }
    }
    lbl_1_bss_1608->models[0].rotY -= 0.005f;
}

void fn_1_134AC(void)
{
    lbl_1_bss_1608->anim[13].t[0] = -1;
}

void fn_1_134C4(void)
{
    s32 idx[1] = { 0x33 };
    M549AnimBlock *blk = &lbl_1_bss_1608->anim[13];
    M549StageModel *m = &lbl_1_bss_1608->models[idx[0]];

    if (blk->t[0] >= -1) {
        if (blk->t[0] == -1) {
            Hu3DModelAttrSet(m->id, 1);
            blk->t[0]--;
            return;
        }
        if (blk->init == 0) {
            blk->init = 1;
            blk->t[0] = 0;
            blk->t[1] = 0;
            blk->t[2] = 0;
            blk->s10 = 0;
            blk->state = 0;
            m->x = -1100.0f;
            m->y = 1150.0f;
            m->z = -800.0f;
            m->rotX = 0.0f;
            m->rotZ = 0.0f;
            Hu3DModelPosSetV(m->id, (HuVecF *)&m->x);
            Hu3DModelAttrReset(m->id, 1);
            Hu3DMotionTimeSet(m->id, 0.0f);
            Hu3DMotionSpeedSet(m->id, 1.0f);
            blk->f2C = 1.0f;
            MODEL_MAT_ALPHA(m->id, blk->f2C);
            return;
        }
        blk->t[0] = (blk->t[0] + 1) % 360;
        m->rotZ = 2.0 * HuSin(blk->t[0]);
        switch (blk->state) {
        case 0:
            blk->t[1]++;
            blk->f2C = 1.0 - 0.4 * HuSin(blk->t[1] * 90 / 16);
            MODEL_MAT_ALPHA(m->id, blk->f2C);
            if (blk->t[1] == 16) {
                blk->t[1] = 0;
                blk->t[2] = rand8() % 60 + 64;
                blk->state++;
                return;
            }
            break;
        case 1:
            if (blk->t[1]++ >= blk->t[2]) {
                MODEL_MAT_ALPHA(m->id, 1.0f);
                blk->s10 = (blk->s10 + 1) % 6;
                if (blk->s10 == 1 || blk->s10 == 4) {
                    blk->state = 0;
                } else {
                    blk->state++;
                }
                blk->t[1] = 0;
                return;
            }
            break;
        case 2:
            if (blk->t[1]++ >= 6) {
                MODEL_MAT_ALPHA(m->id, 0.6f);
                blk->state = 0;
                blk->t[1] = 0;
            }
            break;
        }
    }
}

void fn_1_139F4(void)
{
    s32 idx[6] = { 0x34, 0x3F, 0x41, 0x40, 0x42, 0x35 };
    f32 radius[5] = { 3000.0f, 700.0f, 700.0f, 500.0f, 500.0f };
    f32 angle[5] = { 90.0f, 180.0f, 0.0f, 90.0f, 270.0f };
    f32 speed[5] = { 0.3f, 0.5f, 0.5f, 0.5f, 0.5f };
    f32 center[6] = { 1600.0f, -300.0f, -3900.0f, -1600.0f, -600.0f, -5800.0f };
    M549AnimBlock *blk;
    M549StageModel *m;
    M549StageModel *m2;
    s32 i;

    for (i = 0; i < 5; i++) {
        blk = &lbl_1_bss_1608->anim[i + 14];
        m = &lbl_1_bss_1608->models[idx[i]];
        if (blk->init == 0) {
            blk->init = 1;
            blk->t[0] = 0;
            Hu3DModelAttrReset(m->id, 1);
            Hu3DMotionTimeSet(m->id, 0.0f);
            if (i == 0) {
                Hu3DMotionSpeedSet(m->id, 2.0f);
            } else {
                Hu3DMotionSpeedSet(m->id, 1.0f);
            }
            Hu3DModelAttrSet(m->id, 0x40000001);
            if (i >= 3) {
                Hu3DModelScaleSet(m->id, 2.0f, 2.0f, 2.0f);
            }
            blk->f2C = radius[i];
            blk->f30 = angle[i];
            blk->f34 = speed[i];
        }
        blk->f30 += blk->f34;
        if (i == 0) {
            if (blk->f30 > 270.0f) {
                blk->f30 = 0.0f;
                blk->s1C = 0;
            }
            if (blk->f30 > 0.0f) {
                if (blk->f30 <= 90.0f) {
                    blk->f30 -= 3.0f * blk->f34 * 0.25f;
                }
            }
        }
        if (blk->f34 >= 0.0f) {
            m->rotY = 90.0f + blk->f30;
        } else {
            m->rotY = 270.0f + blk->f30;
        }
        if (i < 3) {
            if (i == 0) {
                if (blk->state == 0) {
                    m->x = blk->f2C * HuSin(blk->f30);
                    m->z = blk->f2C * HuCos(blk->f30);
                    m->y = -200.0 + 5000.0 * sin(0.2617993877991494) * HuSin(180.0f + blk->f30);
                    m->rotX = 15.0 * HuSin(90.0f + blk->f30);
                    if (blk->s1C == 0 && blk->f30 > 180.0f) {
                        blk->state++;
                        blk->f3C = m->x + 600.0 * HuCos(90.0f + m->rotX);
                        blk->f40 = m->y + 600.0 * HuSin(90.0f + m->rotX);
                        blk->f44 = (90.0f + m->rotX) - 180.0f;
                        blk->s10 = 0;
                        blk->f34 = 0.0f;
                        blk->s1C = 1;
                    }
                } else {
                    blk->f44 -= 1.5;
                    m->rotX -= 1.5;
                    m->x = blk->f3C + 600.0 * HuCos(blk->f44);
                    m->y = blk->f40 + 600.0 * HuSin(blk->f44);
                    if (blk->s10++ >= 0xEF) {
                        blk->f34 = 0.3f;
                        blk->state = 0;
                    }
                }
                blk->t[0]++;
                if (blk->t[0] >= 15) {
                    blk->t[0] = 0;
                    m2 = &lbl_1_bss_1608->models[idx[5] + blk->t[1]];
                    Hu3DModelAttrReset(m2->id, 1);
                    Hu3DMotionTimeSet(m2->id, 0.0f);
                    Hu3DMotionSpeedSet(m2->id, 1.0f);
                    if (blk->state == 0) {
                        m2->x = blk->f2C * HuSin(blk->f30 - blk->f34);
                        m2->z = blk->f2C * HuCos(blk->f30 - blk->f34);
                        m2->y = -200.0 + 5000.0 * sin(0.2617993877991494) * HuSin(180.0f + (blk->f30 - blk->f34));
                    } else {
                        m2->x = blk->f3C + 600.0 * HuCos(1.5f + blk->f44);
                        m2->y = blk->f40 + 600.0 * HuSin(1.5f + blk->f44);
                        m2->z = m->z;
                    }
                    Hu3DModelPosSetV(m2->id, (HuVecF *)&m2->x);
                    blk->t[1] = (blk->t[1] + 1) % 10;
                }
            } else if (i >= 0) {
                m->x = center[0] + blk->f2C * HuSin(blk->f30);
                m->z = center[2] + blk->f2C * HuCos(blk->f30);
                m->y = center[1];
            }
        } else if (i < 5) {
            m->x = center[3] + blk->f2C * HuSin(blk->f30);
            m->z = center[5] + blk->f2C * HuCos(blk->f30);
            m->y = center[4];
        }
    }
}

void fn_1_1448C(void)
{
    lbl_1_bss_1608->anim[19].t[0] = -1;
    Hu3DModelAttrSet(lbl_1_bss_1608->models[0x2D].id, 1);
}

void fn_1_144D8(void)
{
    s32 idx[1] = { 0x2D };
    M549AnimBlock *blk = &lbl_1_bss_1608->anim[19];
    M549StageModel *m = &lbl_1_bss_1608->models[idx[0]];

    if (blk->t[0] >= -1) {
        if (blk->t[0] == -1) {
            Hu3DModelAttrSet(m->id, 1);
            blk->t[0]--;
        }
        if (blk->init == 0) {
            if (blk->t[0]++ > 800) {
                blk->init = 1;
                blk->t[0] = 0;
                Hu3DModelAttrReset(m->id, 1);
                Hu3DMotionTimeSet(m->id, 50.0f);
                Hu3DMotionSpeedSet(m->id, 0.5f);
                Hu3DModelScaleSet(m->id, 2.0f, 2.0f, 2.0f);
                m->x = -2000.0f;
                m->z = 2800.0f;
                m->y = 800.0f;
                Hu3DModelPosSetV(m->id, (HuVecF *)&m->x);
            }
        }
        if (blk->init == 1) {
            blk->t[0]++;
            if (blk->t[0] >= 99) {
                Hu3DMotionTimeSet(m->id, 51.0f);
                blk->t[0] = 0;
            }
            m->x += 5.0f;
            if (m->x > 4000.0f) {
                blk->init = 2;
                blk->t[0] = 0;
                blk->t[1] = rand8() % 5 * 30 + 600;
            }
        }
        if (blk->init == 2) {
            if (blk->t[0]++ >= blk->t[1]) {
                m->x = -2000.0f;
                blk->init = 1;
            }
        }
    }
}

void fn_1_14720(void)
{
    Hu3DModelAttrSet(lbl_1_bss_1608->models[0x2E].id, 1);
    lbl_1_bss_1608->anim[20].init = -1;
}

void fn_1_1476C(void)
{
    s32 idx[1] = { 0x2E };
    M549AnimBlock *blk = &lbl_1_bss_1608->anim[20];
    M549StageModel *m = &lbl_1_bss_1608->models[idx[0]];

    if (blk->init >= 0) {
        switch (blk->init) {
        case 0:
            if (blk->t[0]++ > 300) {
                blk->t[0] = -200;
                blk->init = 1;
                Hu3DModelAttrReset(m->id, 1);
                Hu3DMotionTimeSet(m->id, 0.0f);
                Hu3DMotionSpeedSet(m->id, 0.25f);
                m->x = -4356.0f;
                m->z = -13000.0f;
                m->y = 0.0f;
                Hu3DModelPosSetV(m->id, (HuVecF *)&m->x);
                return;
            }
            break;
        case 1:
            blk->t[0]++;
            if (blk->t[0] >= 200) {
                blk->t[0] = 0;
                Hu3DMotionTimeSet(m->id, 51.0f);
            }
            m->x = m->x + 14.85 * sin(0.39269908169872414);
            m->z = m->z + 14.85 * cos(0.39269908169872414);
            if (m->z >= 2000.0f) {
                blk->init = 2;
                blk->t[0] = 0;
                blk->t[1] = rand8() % 8 * 30 + 300;
                return;
            }
            break;
        case 2:
            if (blk->t[0]++ >= blk->t[1]) {
                blk->init = 1;
                Hu3DMotionTimeSet(m->id, 0.0f);
                m->x = rand8() % 4 * 500 - 4356;
                m->z = -13000.0f;
                blk->t[0] = -200;
            }
            break;
        }
    }
}
