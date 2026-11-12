#include "REL/m553Dll.h"
#include "game/memory.h"
#include "string.h"
#include "math.h"

extern float Hu3DMotionMaxTimeGet(HU3DMODELID modelId);
extern s32 rand8(void);

s32 lbl_1_data_3B8[2] = { 0x6F0019, 0x6F001A };

void fn_1_ECB8(void);
void fn_1_F070(void);
void fn_1_F2A8(void);
void fn_1_F7D8(void);
void fn_1_102BC(void);
void fn_1_10550(void);

void (*lbl_1_data_3C0[6])(void) = {
    fn_1_ECB8, fn_1_F070, fn_1_F2A8, fn_1_F7D8, fn_1_102BC, fn_1_10550,
};

M553STWORK *lbl_1_bss_1F00;

void fn_1_D9A4(M553STWORK *work, M553STMODEL *model, s32 *data)
{
    s16 i;
    s16 p;

    lbl_1_bss_1F00 = work;
    memset(lbl_1_bss_1F00, 0, sizeof(M553STWORK));
    lbl_1_bss_1F00->model = model;
    lbl_1_bss_1F00->data = data;
    p = 0;
    do {
        for (i = 0; i < 4; i++) {
            if (p == GwPlayerConf[i].grp) {
                lbl_1_bss_1F00->player[p].idx = i;
                p++;
                if (p >= 2) {
                    break;
                }
            }
        }
    } while (p < 2);
    for (i = 0; i < 2; i++) {
        lbl_1_bss_1F00->player[i].charNo = GwPlayerConf[lbl_1_bss_1F00->player[i].idx].charNo;
        lbl_1_bss_1F00->player[i].padNo = GwPlayerConf[lbl_1_bss_1F00->player[i].idx].padNo;
        if (GwPlayerConf[lbl_1_bss_1F00->player[i].idx].type != 0) {
            lbl_1_bss_1F00->player[i].dif = GwPlayerConf[lbl_1_bss_1F00->player[i].idx].dif + 1;
        }
    }
}

void fn_1_DBF0(s32 arg)
{
    s16 mot[2] = { 0, 0 };
    HU3DMODELID base;
    M553STMODEL *m;
    s32 i;
    s32 j;

    for (i = 0; i < 3; i++) {
        m = &lbl_1_bss_1F00->model[0x66 + i * 3];
        for (j = 0; j < 3; j++, m++) {
            if (j == 0) {
                m->id = Hu3DModelCreateData(lbl_1_bss_1F00->data[i + 0xC]);
                base = m->id;
            } else {
                m->id = Hu3DModelLink(base);
            }
            Hu3DModelAttrSet(m->id, 1);
            Hu3DMotionTimeSet(m->id, 50.0f);
            Hu3DMotionSpeedSet(m->id, 0.5f);
            Hu3DModelCameraSet(m->id, 1);
            Hu3DModelLayerSet(m->id, 1);
        }
    }
    for (i = 0; i < 2; i++) {
        m = &lbl_1_bss_1F00->model[0x6F + i];
        m->id = Hu3DModelCreateData(lbl_1_bss_1F00->data[i + 0xF]);
        Hu3DModelAttrSet(m->id, 1);
        Hu3DMotionSpeedSet(m->id, 0.5f);
        Hu3DModelCameraSet(m->id, 1);
        Hu3DMotionTimeSet(m->id, 0.0f);
        Hu3DModelLayerSet(m->id, 7);
        m = &lbl_1_bss_1F00->model[0x71 + i * 2];
        for (j = 0; j < 2; j++, m++) {
            if (j == 0) {
                m->id = Hu3DModelCreateData(lbl_1_bss_1F00->data[i + 0x11]);
                base = m->id;
            } else {
                m->id = Hu3DModelLink(base);
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
        s32 modelNo[2] = { 0x75, 0x76 };
        s32 dataNo[2] = { 0x13, 0x14 };

        m = &lbl_1_bss_1F00->model[modelNo[i]];
        m->id = Hu3DModelCreateData(lbl_1_bss_1F00->data[dataNo[i]]);
        Hu3DModelAttrSet(m->id, 1);
        Hu3DModelCameraSet(m->id, 1);
        if (i < 1) {
            Hu3DModelLayerSet(m->id, 4);
        } else {
            Hu3DModelLayerSet(m->id, 1);
        }
    }
    for (i = 0; i < 10; i++) {
        m = &lbl_1_bss_1F00->model[0x77 + i];
        if (i == 0) {
            m->id = Hu3DModelCreateData(lbl_1_bss_1F00->data[0x15]);
            base = m->id;
        } else {
            m->id = Hu3DModelLink(base);
        }
        Hu3DModelAttrSet(m->id, 1);
        Hu3DModelCameraSet(m->id, 1);
        Hu3DModelLayerSet(m->id, 4);
    }
    for (i = 0; i < 2; i++) {
        m = &lbl_1_bss_1F00->model[0x81 + i * 2];
        for (j = 0; j < 2; j++, m++) {
            Hu3DModelAttrSet(m->id, 1);
            if (j == 0) {
                m->id = Hu3DModelCreateData(lbl_1_bss_1F00->data[i + 0x16]);
                base = m->id;
                mot[i] = Hu3DJointMotionData(m->id, lbl_1_data_3B8[i]);
                Hu3DModelScaleSet(m->id, 2.2f, 2.2f, 2.2f);
            } else {
                m->id = Hu3DModelLink(base);
                Hu3DModelScaleSet(m->id, 1.0f, 1.0f, 1.0f);
            }
            Hu3DMotionSet(m->id, mot[i]);
            Hu3DMotionSpeedSet(m->id, 1.0f);
            Hu3DModelAttrSet(m->id, 0x40000001);
            Hu3DModelCameraSet(m->id, 1);
            Hu3DModelLayerSet(m->id, 1);
        }
    }
    m = &lbl_1_bss_1F00->model[0x85];
    m->id = Hu3DModelCreateData(lbl_1_bss_1F00->data[0x18]);
    Hu3DModelAttrSet(m->id, 1);
    Hu3DMotionTimeSet(m->id, 50.0f);
    Hu3DMotionSpeedSet(m->id, 0.5f);
    Hu3DModelCameraSet(m->id, 1);
    Hu3DModelLayerSet(m->id, 4);
}

void fn_1_E234(s16 index, s32 *data)
{
    lbl_1_bss_1F00->motData[index] = data;
}

void fn_1_E254(s16 index, s16 *data)
{
    lbl_1_bss_1F00->motId[index] = data;
}

void fn_1_E274(void)
{
}

void fn_1_E278(void)
{
    s32 i;
    s8 absX;
    s8 stkX;

    for (i = 0; i < 2; i++) {
        if (lbl_1_bss_1F00->player[i].dif != 0) {
            memset(&lbl_1_bss_1F00->player[i].btn, 0, 0x14);
        } else {
            lbl_1_bss_1F00->player[i].btn = HuPadBtn[lbl_1_bss_1F00->player[i].padNo];
            lbl_1_bss_1F00->player[i].btnDown = HuPadBtnDown[lbl_1_bss_1F00->player[i].padNo];
            lbl_1_bss_1F00->player[i].btnRep = HuPadBtnRep[lbl_1_bss_1F00->player[i].padNo];
            lbl_1_bss_1F00->player[i].stkX = HuPadStkX[lbl_1_bss_1F00->player[i].padNo];
            if (lbl_1_bss_1F00->player[i].stkX >= 0) {
                absX = lbl_1_bss_1F00->player[i].stkX;
            } else {
                absX = -lbl_1_bss_1F00->player[i].stkX;
            }
            if (absX < 10) {
                stkX = 0;
            } else {
                stkX = lbl_1_bss_1F00->player[i].stkX;
            }
            lbl_1_bss_1F00->player[i].stkX = stkX;
            lbl_1_bss_1F00->player[i].stkY = HuPadStkY[lbl_1_bss_1F00->player[i].padNo];
            lbl_1_bss_1F00->player[i].subStkX = HuPadSubStkX[lbl_1_bss_1F00->player[i].padNo];
            lbl_1_bss_1F00->player[i].subStkY = HuPadSubStkY[lbl_1_bss_1F00->player[i].padNo];
            lbl_1_bss_1F00->player[i].trigL = HuPadTrigL[lbl_1_bss_1F00->player[i].padNo];
            lbl_1_bss_1F00->player[i].trigR = HuPadTrigR[lbl_1_bss_1F00->player[i].padNo];
            lbl_1_bss_1F00->player[i].dStk = HuPadDStk[lbl_1_bss_1F00->player[i].padNo];
            lbl_1_bss_1F00->player[i].dStkRep = HuPadDStkRep[lbl_1_bss_1F00->player[i].padNo];
        }
    }
}

void fn_1_E690(void)
{
    s16 i;
    s16 j;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 5; j++) {
            lbl_1_bss_1F00->motId[i][j] = CharMotionCreate(lbl_1_bss_1F00->player[i].charNo, lbl_1_bss_1F00->motData[i][j]);
        }
    }
    for (i = 0; i < 2; i++) {
        lbl_1_bss_1F00->motCtl[i].motNo = 0;
        lbl_1_bss_1F00->motCtl[i].motNoOld = 0;
        lbl_1_bss_1F00->motCtl[i].speed = 0.0f;
        lbl_1_bss_1F00->motCtl[i].speedOld = 0.0f;
        lbl_1_bss_1F00->motCtl[i].attr = 0;
        lbl_1_bss_1F00->motCtl[i].attrOld = 0;
    }
}

void fn_1_E844(OMOBJ *obj)
{
    s16 i;
    s16 modelNo;
    M553STMOTCTL *ctl;

    for (i = 0; i < 2; i++) {
        ctl = &lbl_1_bss_1F00->motCtl[i];
        modelNo = i + 0x86;
        if (ctl->motNo != ctl->motNoOld) {
            Hu3DMotionSet(lbl_1_bss_1F00->model[modelNo].id, lbl_1_bss_1F00->motId[i][ctl->motNo]);
            ctl->motNoOld = ctl->motNo;
            ctl->maxTime = Hu3DMotionMaxTimeGet(lbl_1_bss_1F00->model[modelNo].id);
        }
        if (ctl->speed != ctl->speedOld) {
            Hu3DMotionSpeedSet(lbl_1_bss_1F00->model[modelNo].id, ctl->speed);
            ctl->speedOld = ctl->speed;
        }
        if (ctl->attr != ctl->attrOld) {
            Hu3DModelAttrSet(lbl_1_bss_1F00->model[modelNo].id, ctl->attr);
            ctl->attrOld = ctl->attr;
        }
        if (ctl->attrReset != 0) {
            Hu3DModelAttrReset(lbl_1_bss_1F00->model[modelNo].id, ctl->attrReset);
            ctl->attrReset = ~ctl->attrReset;
            ctl->attr = ctl->attr & ctl->attrReset;
            ctl->attrOld = ctl->attr;
            ctl->attrReset = 0;
        }
        if (ctl->time != ctl->timeOld) {
            Hu3DMotionTimeSet(lbl_1_bss_1F00->model[modelNo].id, ctl->time);
        }
        switch (ctl->shiftMode) {
            case 0:
                break;
            case 1:
                Hu3DMotionShiftSet(lbl_1_bss_1F00->model[modelNo].id, lbl_1_bss_1F00->motId[i][ctl->shiftMotNo], ctl->shiftStart, ctl->shiftEnd, ctl->shiftAttr);
                ctl->shiftMaxTime = Hu3DMotionShiftMaxTimeGet(lbl_1_bss_1F00->model[modelNo].id);
                ctl->shiftMode = 2;
                break;
            case 2:
                ctl->shiftTime = Hu3DMotionShiftTimeGet(lbl_1_bss_1F00->model[modelNo].id);
                if ((ctl->shiftAttr & 0x40000001) == 0) {
                    if (ctl->shiftTime >= (ctl->shiftEnd - 1.0f)) {
                        ctl->shiftMode = 3;
                    }
                }
                break;
            case 3:
                ctl->shiftMode = 0;
                break;
        }
        ctl->time = Hu3DMotionTimeGet(lbl_1_bss_1F00->model[modelNo].id);
        ctl->timeOld = ctl->time;
        ctl->endFlag = Hu3DMotionEndCheck(lbl_1_bss_1F00->model[modelNo].id);
        if (ctl->endFlag != 0 && ctl->endReturn != 0) {
            ctl->endFlag = 0;
            ctl->endReturn = 0;
            ctl->motNo = ctl->retMotNo;
            ctl->time = ctl->retTime;
            ctl->attr = ctl->retAttr;
            ctl->attrReset = ctl->retAttrReset;
        }
    }
}

void fn_1_EBFC(HU3DMODELID modelId, f32 alpha)
{
    HU3DMODEL *model = &Hu3DData[modelId];
    HSFMATERIAL *mat = model->hsf->material;
    s32 i;

    for (i = 0; i < model->hsf->materialNum; i++, mat++) {
        mat->invAlpha = alpha;
    }
}

void fn_1_EC64(OMOBJ *obj)
{
    s32 i;

    for (i = 0; i < 6; i++) {
        lbl_1_data_3C0[i]();
    }
}

void fn_1_ECB8(void)
{
    s32 modelNo[9] = { 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x85 };
    f32 posTbl[9][3] = {
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
    f32 scaleTbl[9] = { 2.0f, 1.5f, 1.5f, 1.0f, 1.0f, 1.2f, 1.8f, 1.5f, 1.5f };
    s32 i;
    s32 j;
    M553STFX *fx;
    M553STMODEL *m;

    for (i = 0; i < 9; i++) {
        fx = &lbl_1_bss_1F00->fx[i];
        m = &lbl_1_bss_1F00->model[modelNo[i]];
        if (fx->active == 0) {
            fx->active = 1;
            fx->t[0] = 0;
            fx->t[1] = 0;
            fx->t[2] = 0;
            m->pos.x = posTbl[i][0];
            m->pos.z = posTbl[i][2];
            m->pos.y = posTbl[i][1];
            fx->f34 = i * 0x28;
            Hu3DModelAttrReset(m->id, 1);
            Hu3DMotionTimeSet(m->id, 50.0f);
            Hu3DMotionSpeedSet(m->id, 0.5f);
            Hu3DModelPosSetV(m->id, &m->pos);
            Hu3DModelScaleSet(m->id, scaleTbl[i], scaleTbl[i], scaleTbl[i]);
        }
        for (j = 0; j < 3; j++) {
            fx->t[j] = fx->t[j] + 1;
            switch (j) {
                case 0:
                    if (fx->t[j] >= 0x10) {
                        fx->t[j] = 0;
                        fx->f2C = fx->f2C + 0.001;
                    }
                    break;
                case 1:
                    if (fx->t[j] >= 0x64) {
                        fx->t[j] = 0;
                        Hu3DMotionTimeSet(m->id, 50.0f);
                    }
                    break;
                case 2:
                    if (fx->t[j] >= 4) {
                        fx->t[j] = 0;
                        fx->f34 = fx->f34 + 1.0f;
                    }
                    break;
            }
        }
        m->pos.y = posTbl[i][1] + (200.0 * sin((3.141592653589793 * fx->f34) / 180.0));
    }
}

void fn_1_F070(void)
{
    s32 modelNo[4] = { 0x71, 0x73, 0x72, 0x74 };
    s32 i;
    M553STFX *fx;
    M553STMODEL *m;

    for (i = 0; i < 4; i++) {
        fx = &lbl_1_bss_1F00->fx[i + 9];
        m = &lbl_1_bss_1F00->model[modelNo[i]];
        if (fx->active == 0) {
            fx->active = 1;
            fx->t[0] = 0;
            m->pos.x = 0.0f;
            m->pos.y = 0.0f;
            m->pos.z = 0.0f;
            m->rot.y = i * 0x1E - 0xF;
            Hu3DModelAttrReset(m->id, 1);
            Hu3DMotionTimeSet(m->id, i * 0x19);
            Hu3DMotionSpeedSet(m->id, 0.5f);
            Hu3DModelPosSetV(m->id, &m->pos);
            Hu3DModelRotSetV(m->id, &m->rot);
            Hu3DModelAttrSet(m->id, 0x40000001);
        }
        fx->t[0] = fx->t[0] + 1;
        m->rot.y = m->rot.y - 0.01;
        if (m->rot.y <= -45.0f) {
            m->rot.y = 75.0f;
        }
    }
    m = lbl_1_bss_1F00->model;
    m->rot.y = m->rot.y - 0.005f;
}

void fn_1_F290(void)
{
    lbl_1_bss_1F00->fx[13].t[0] = -1;
}

void fn_1_F2A8(void)
{
    s32 modelNo = 0x75;
    M553STFX *fx = &lbl_1_bss_1F00->fx[13];
    M553STMODEL *m = &lbl_1_bss_1F00->model[modelNo];
    HU3DMODEL *model;
    HSFMATERIAL *mat;
    s32 i;

    if (fx->t[0] >= -1) {
        if (fx->t[0] == -1) {
            Hu3DModelAttrSet(m->id, 1);
            fx->t[0]--;
            return;
        }
        if (lbl_1_bss_1F00->fx[13].active == 0) {
            lbl_1_bss_1F00->fx[13].active = 1;
            fx->t[0] = 0;
            fx->t[1] = 0;
            fx->t[2] = 0;
            fx->i10 = 0;
            fx->i18 = 0;
            m->pos.x = -1100.0f;
            m->pos.y = 1150.0f;
            m->pos.z = -800.0f;
            m->rot.x = 0.0f;
            m->rot.z = 0.0f;
            Hu3DModelPosSetV(m->id, &m->pos);
            Hu3DModelAttrReset(m->id, 1);
            Hu3DMotionTimeSet(m->id, 0.0f);
            Hu3DMotionSpeedSet(m->id, 1.0f);
            fx->f2C = 1.0f;
            model = &Hu3DData[m->id];
            mat = model->hsf->material;
            for (i = 0; i < model->hsf->materialNum; i++, mat++) {
                mat->invAlpha = fx->f2C;
            }
            return;
        }
        fx->t[0] = (fx->t[0] + 1) % 360;
        m->rot.z = 2.0 * sin((3.141592653589793 * fx->t[0]) / 180.0);
        switch (fx->i18) {
            case 0:
                fx->t[1] = fx->t[1] + 1;
                fx->f2C = 1.0 - (0.4 * sin((3.141592653589793 * ((fx->t[1] * 0x5A) / 16)) / 180.0));
                model = &Hu3DData[m->id];
                mat = model->hsf->material;
                for (i = 0; i < model->hsf->materialNum; i++, mat++) {
                    mat->invAlpha = fx->f2C;
                }
                if (fx->t[1] == 0x10) {
                    fx->t[1] = 0;
                    fx->t[2] = (rand8() % 60) + 0x40;
                    fx->i18 = fx->i18 + 1;
                    return;
                }
                break;
            case 1:
                if (fx->t[1]++ >= fx->t[2]) {
                    model = &Hu3DData[m->id];
                    mat = model->hsf->material;
                    for (i = 0; i < model->hsf->materialNum; i++, mat++) {
                        mat->invAlpha = 1.0f;
                    }
                    fx->i10 = (fx->i10 + 1) % 6;
                    if (fx->i10 == 1 || fx->i10 == 4) {
                        fx->i18 = 0;
                    } else {
                        fx->i18 = fx->i18 + 1;
                    }
                    fx->t[1] = 0;
                    return;
                }
                break;
            case 2:
                if (fx->t[1]++ >= 6) {
                    model = &Hu3DData[m->id];
                    mat = model->hsf->material;
                    for (i = 0; i < model->hsf->materialNum; i++, mat++) {
                        mat->invAlpha = 0.6f;
                    }
                    fx->i18 = 0;
                    fx->t[1] = 0;
                }
                break;
        }
    }
}

void fn_1_F7D8(void)
{
    s32 modelNo[6] = { 0x76, 0x81, 0x83, 0x82, 0x84, 0x77 };
    f32 radius[5] = { 3000.0f, 700.0f, 700.0f, 500.0f, 500.0f };
    f32 angle[5] = { 90.0f, 180.0f, 0.0f, 90.0f, 270.0f };
    f32 speed[5] = { 0.3f, 0.5f, 0.5f, 0.5f, 0.5f };
    f32 base[6] = { 1600.0f, -300.0f, -3900.0f, -1600.0f, -600.0f, -5800.0f };
    s32 i;
    M553STFX *fx;
    M553STMODEL *m;
    M553STMODEL *m2;

    for (i = 0; i < 5; i++) {
        fx = &lbl_1_bss_1F00->fx[i + 0xE];
        m = &lbl_1_bss_1F00->model[modelNo[i]];
        if (fx->active == 0) {
            fx->active = 1;
            fx->t[0] = 0;
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
            fx->f2C = radius[i];
            fx->f30 = angle[i];
            fx->f34 = speed[i];
        }
        fx->f30 = fx->f30 + fx->f34;
        if (i == 0) {
            if (fx->f30 > 270.0f) {
                fx->f30 = 0.0f;
                fx->i1C = 0;
            }
            if (fx->f30 > 0.0f) {
                if (fx->f30 <= 90.0f) {
                    fx->f30 = fx->f30 - (3.0f * fx->f34 * 0.25f);
                }
            }
        }
        if (fx->f34 >= 0.0f) {
            m->rot.y = 90.0f + fx->f30;
        } else {
            m->rot.y = 270.0f + fx->f30;
        }
        if (i < 3) {
            if (i != 0) {
                if (i < 0) {
                } else {
                    m->pos.x = base[0] + (fx->f2C * sin((3.141592653589793 * fx->f30) / 180.0));
                    m->pos.z = base[2] + (fx->f2C * cos((3.141592653589793 * fx->f30) / 180.0));
                    m->pos.y = base[1];
                }
            } else {
                if (fx->i18 == 0) {
                    m->pos.x = fx->f2C * sin((3.141592653589793 * fx->f30) / 180.0);
                    m->pos.z = fx->f2C * cos((3.141592653589793 * fx->f30) / 180.0);
                    m->pos.y = -200.0 + (5000.0 * sin(0.2617993877991494) * sin((3.141592653589793 * (180.0f + fx->f30)) / 180.0));
                    m->rot.x = 15.0 * sin((3.141592653589793 * (90.0f + fx->f30)) / 180.0);
                    if (fx->i1C == 0 && fx->f30 > 180.0f) {
                        fx->i18 = fx->i18 + 1;
                        fx->f3C = m->pos.x + (600.0 * cos((3.141592653589793 * (90.0f + m->rot.x)) / 180.0));
                        fx->f40 = m->pos.y + (600.0 * sin((3.141592653589793 * (90.0f + m->rot.x)) / 180.0));
                        fx->f44 = (90.0f + m->rot.x) - 180.0f;
                        fx->i10 = 0;
                        fx->f34 = 0.0f;
                        fx->i1C = 1;
                    }
                } else {
                    fx->f44 = fx->f44 - 1.5;
                    m->rot.x = m->rot.x - 1.5;
                    m->pos.x = fx->f3C + (600.0 * cos((3.141592653589793 * fx->f44) / 180.0));
                    m->pos.y = fx->f40 + (600.0 * sin((3.141592653589793 * fx->f44) / 180.0));
                    if (fx->i10++ >= 0xEF) {
                        fx->f34 = 0.3f;
                        fx->i18 = 0;
                    }
                }
                fx->t[0] = fx->t[0] + 1;
                if (fx->t[0] >= 0xF) {
                    fx->t[0] = 0;
                    m2 = &lbl_1_bss_1F00->model[0x77 + fx->t[1]];
                    Hu3DModelAttrReset(m2->id, 1);
                    Hu3DMotionTimeSet(m2->id, 0.0f);
                    Hu3DMotionSpeedSet(m2->id, 1.0f);
                    if (fx->i18 == 0) {
                        m2->pos.x = fx->f2C * sin((3.141592653589793 * (fx->f30 - fx->f34)) / 180.0);
                        m2->pos.z = fx->f2C * cos((3.141592653589793 * (fx->f30 - fx->f34)) / 180.0);
                        m2->pos.y = -200.0 + (5000.0 * sin(0.2617993877991494) * sin((3.141592653589793 * (180.0f + (fx->f30 - fx->f34))) / 180.0));
                    } else {
                        m2->pos.x = fx->f3C + (600.0 * cos((3.141592653589793 * (1.5f + fx->f44)) / 180.0));
                        m2->pos.y = fx->f40 + (600.0 * sin((3.141592653589793 * (1.5f + fx->f44)) / 180.0));
                        m2->pos.z = m->pos.z;
                    }
                    Hu3DModelPosSetV(m2->id, &m2->pos);
                    fx->t[1] = (fx->t[1] + 1) % 10;
                }
            }
        } else if (i < 5) {
            m->pos.x = base[3] + (fx->f2C * sin((3.141592653589793 * fx->f30) / 180.0));
            m->pos.z = base[5] + (fx->f2C * cos((3.141592653589793 * fx->f30) / 180.0));
            m->pos.y = base[4];
        }
    }
}

void fn_1_10270(void)
{
    lbl_1_bss_1F00->fx[19].t[0] = -1;
    Hu3DModelAttrSet(lbl_1_bss_1F00->model[0x7C].id, 1);
}

void fn_1_102BC(void)
{
    s32 unused = 0x6F;
    M553STFX *fx = &lbl_1_bss_1F00->fx[19];
    M553STMODEL *model = lbl_1_bss_1F00->model;
    M553STMODEL *m = &model[0x7C];

    if (fx->t[0] >= -1) {
        if (fx->t[0] == -1) {
            Hu3DModelAttrSet(model[0x7C].id, 1);
            fx->t[0]--;
        }
        if (lbl_1_bss_1F00->fx[19].active == 0) {
            if (fx->t[0]++ > 0x320) {
                lbl_1_bss_1F00->fx[19].active = 1;
                fx->t[0] = 0;
                Hu3DModelAttrReset(model[0x7C].id, 1);
                Hu3DMotionTimeSet(model[0x7C].id, 50.0f);
                Hu3DMotionSpeedSet(model[0x7C].id, 0.5f);
                Hu3DModelScaleSet(model[0x7C].id, 2.0f, 2.0f, 2.0f);
                m->pos.x = -2000.0f;
                m->pos.z = 2500.0f;
                m->pos.y = 300.0f;
                Hu3DModelPosSetV(model[0x7C].id, &m->pos);
            }
        }
        if (lbl_1_bss_1F00->fx[19].active == 1) {
            fx->t[0] = fx->t[0] + 1;
            if (fx->t[0] >= 0x63) {
                Hu3DMotionTimeSet(model[0x7C].id, 51.0f);
                fx->t[0] = 0;
            }
            m->pos.x = m->pos.x + 5.0f;
            if (m->pos.x > 4000.0f) {
                lbl_1_bss_1F00->fx[19].active = 2;
                fx->t[0] = 0;
                fx->t[1] = ((rand8() % 5) * 0x1E) + 0x258;
            }
        }
        if (lbl_1_bss_1F00->fx[19].active == 2) {
            if (fx->t[0]++ >= fx->t[1]) {
                m->pos.x = -2000.0f;
                lbl_1_bss_1F00->fx[19].active = 1;
            }
        }
    }
}

void fn_1_10504(void)
{
    Hu3DModelAttrSet(lbl_1_bss_1F00->model[0x7D].id, 1);
    lbl_1_bss_1F00->fx[20].active = -1;
}

void fn_1_10550(void)
{
    s32 modelNo = 0x70;
    M553STFX *fx = &lbl_1_bss_1F00->fx[20];
    M553STMODEL *m = &lbl_1_bss_1F00->model[modelNo];

    if (lbl_1_bss_1F00->fx[20].active >= 0) {
        switch (lbl_1_bss_1F00->fx[20].active) {
            case 0:
                if (fx->t[0]++ > 0x12C) {
                    fx->t[0] = -0xC8;
                    lbl_1_bss_1F00->fx[20].active = 1;
                    Hu3DModelAttrReset(m->id, 1);
                    Hu3DMotionTimeSet(m->id, 0.0f);
                    Hu3DMotionSpeedSet(m->id, 0.25f);
                    m->pos.x = -4356.0f;
                    m->pos.z = -13000.0f;
                    m->pos.y = 0.0f;
                    Hu3DModelPosSetV(m->id, &m->pos);
                    return;
                }
                break;
            case 1:
                fx->t[0] = fx->t[0] + 1;
                if (fx->t[0] >= 0xC8) {
                    fx->t[0] = 0;
                    Hu3DMotionTimeSet(m->id, 51.0f);
                }
                m->pos.x = m->pos.x + (14.85 * sin(0.39269908169872414));
                m->pos.z = m->pos.z + (14.85 * cos(0.39269908169872414));
                if (m->pos.z >= 2000.0f) {
                    lbl_1_bss_1F00->fx[20].active = 2;
                    fx->t[0] = 0;
                    fx->t[1] = ((rand8() % 8) * 0x1E) + 0x12C;
                    return;
                }
                break;
            case 2:
                if (fx->t[0]++ >= fx->t[1]) {
                    lbl_1_bss_1F00->fx[20].active = 1;
                    Hu3DMotionTimeSet(m->id, 0.0f);
                    m->pos.x = ((rand8() % 4) * 0x1F4) - 0x1104;
                    m->pos.z = -13000.0f;
                    fx->t[0] = -0xC8;
                }
                break;
        }
    }
}
