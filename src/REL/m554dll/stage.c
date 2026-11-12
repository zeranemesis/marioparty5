// m554dll/stage.c - decompilation of the m554dll/REL/m554dll/stage unit.

#define _MATH_H /* keeps MWCC from adding its 16 bytes of sqrt literals to .rodata */
#include "types.h"
#include "string.h"
#include "game/audio.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/object.h"
#include "game/pad.h"
#include "REL/m554dll/m554types.h"

double sin(double x);
double cos(double x);
s32 rand8(void);
float Hu3DMotionMaxTimeGet(HU3DMODELID modelId);

void fn_1_78F4(void);
void fn_1_7D74(void);
void fn_1_7FCC(void);

/* ---- data ---- */
HuVecF lbl_1_data_258[6] = {
    { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f },
};
s32 lbl_1_data_2A0[2] = { 0x700015, 0 };
s16 lbl_1_data_2A8[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

/* ---- bss ---- */
M554WORK *lbl_1_bss_1360;

void fn_1_62F0(M554WORK *work, M554ENTRY *ents, s32 *data)
{
    s16 j;
    s16 i;
    s16 t;

    lbl_1_bss_1360 = work;
    memset(lbl_1_bss_1360, 0, sizeof(M554WORK));
    lbl_1_bss_1360->ents = ents;
    lbl_1_bss_1360->data = data;
    i = 0;
    do {
        for (j = 0; j < 4; j++) {
            if (i == (t = GwPlayerConf[j].grp)) {
                lbl_1_bss_1360->player[i].playerNo = j;
                i++;
                if (i >= 2) {
                    break;
                }
            }
        }
    } while (i < 2);
    for (j = 0; j < 2; j++) {
        lbl_1_bss_1360->player[j].charNo = GwPlayerConf[lbl_1_bss_1360->player[j].playerNo].charNo;
        lbl_1_bss_1360->player[j].padNo = GwPlayerConf[lbl_1_bss_1360->player[j].playerNo].padNo;
        if ((t = GwPlayerConf[lbl_1_bss_1360->player[j].playerNo].type) != 0) {
            lbl_1_bss_1360->player[j].dif = GwPlayerConf[lbl_1_bss_1360->player[j].playerNo].dif + 1;
        }
    }
}

void fn_1_653C(s16 arg0)
{
    M554ENTRY *e;
    s16 model;
    s32 i;
    s16 k;
    s16 t;
    s32 j;

    lbl_1_bss_1360->f_fc = arg0;
    for (k = 0; k < 2; k++) {
        t = (s16)k;
        model = lbl_1_bss_1360->ents[t].model = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_bss_1360->data[k], 0x10000000, HEAP_MODEL));
        Hu3DModelAttrSet(model, 1);
        Hu3DModelCameraSet(model, 1);
        Hu3DModelPosSetV(model, &lbl_1_data_258[k]);
        lbl_1_bss_1360->ents[t].pos.x = lbl_1_data_258[k].x;
        lbl_1_bss_1360->ents[t].pos.y = lbl_1_data_258[k].y;
        lbl_1_bss_1360->ents[t].pos.z = lbl_1_data_258[k].z;
        if (k != 0) {
            Hu3DModelRotSet(model, 0.0f, -180.0f, 0.0f);
            lbl_1_bss_1360->ents[t].rot.x = 0.0f;
            lbl_1_bss_1360->ents[t].rot.y = -180.0f;
            lbl_1_bss_1360->ents[t].rot.z = 0.0f;
            Hu3DMotionTimeSet(model, 0.0f);
            Hu3DMotionSpeedSet(model, 1.0f);
            Hu3DModelAttrSet(model, 0x40000001);
        } else {
            Hu3DModelRotSet(model, 0.0f, 0.0f, 0.0f);
            lbl_1_bss_1360->ents[t].rot.x = 0.0f;
            lbl_1_bss_1360->ents[t].rot.y = 0.0f;
            lbl_1_bss_1360->ents[t].rot.z = 0.0f;
        }
        Hu3DModelLayerSet(model, 0);
        Hu3DModelAttrReset(model, 1);
    }
    for (i = 0; i < 2; i++) {
        e = &lbl_1_bss_1360->ents[i + 0x54];
        e->model = Hu3DModelCreate(HuDataSelHeapReadNum((&lbl_1_bss_1360->data[0xf])[i], 0x10000000, HEAP_MODEL));
        Hu3DModelAttrSet(e->model, 1);
        Hu3DMotionSpeedSet(e->model, 0.5f);
        Hu3DModelCameraSet(e->model, 1);
        Hu3DMotionTimeSet(e->model, 0.0f);
        if (i != 0) {
            Hu3DModelLayerSet(e->model, 1);
        } else {
            Hu3DModelLayerSet(e->model, 7);
        }
        e = &lbl_1_bss_1360->ents[i * 2 + 0x56];
        for (j = 0; j < 2; j++, e++) {
            if (j == 0) {
                model = e->model = Hu3DModelCreate(HuDataSelHeapReadNum((&lbl_1_bss_1360->data[0x11])[i], 0x10000000, HEAP_MODEL));
            } else {
                e->model = Hu3DModelLink(model);
            }
            Hu3DModelAttrSet(e->model, 1);
            Hu3DMotionSpeedSet(e->model, 0.5f);
            Hu3DModelCameraSet(e->model, 1);
            Hu3DMotionTimeSet(e->model, 0.0f);
            Hu3DModelAttrSet(e->model, 0x40000001);
            Hu3DModelLayerSet(e->model, 1);
        }
    }
    for (i = 0; i < 4; i++) {
        e = &lbl_1_bss_1360->ents[i + 0x50];
        if (i == 0) {
            model = e->model = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_bss_1360->data[0xe], 0x10000000, HEAP_MODEL));
        } else {
            e->model = Hu3DModelLink(model);
        }
        Hu3DModelAttrSet(e->model, 1);
        Hu3DModelCameraSet(e->model, 1);
        Hu3DModelLayerSet(e->model, 2);
        Hu3DMotionTimeSet(e->model, 50.0f);
        Hu3DMotionSpeedSet(e->model, 0.5f);
    }
    e = &lbl_1_bss_1360->ents[0x5a];
    for (j = 0; j < 2; j++, e++) {
        if (j == 0) {
            model = e->model = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_bss_1360->data[0x13], 0x10000000, HEAP_MODEL));
            lbl_1_data_2A8[i] = Hu3DJointMotion(e->model, HuDataSelHeapReadNum(lbl_1_data_2A0[0], 0x10000000, HEAP_MODEL));
        } else {
            e->model = Hu3DModelLink(model);
        }
        Hu3DModelScaleSet(e->model, 1.0f, 1.0f, 1.0f);
        Hu3DMotionSet(e->model, lbl_1_data_2A8[i]);
        Hu3DMotionSpeedSet(e->model, 1.0f);
        Hu3DModelAttrSet(e->model, 0x40000001);
        Hu3DModelCameraSet(e->model, 1);
        Hu3DModelLayerSet(e->model, 1);
        Hu3DModelAttrSet(e->model, 1);
    }
}

void fn_1_6C20(s32 *arg0, s32 *arg1)
{
    lbl_1_bss_1360->motData[0] = arg0;
    lbl_1_bss_1360->motData[1] = arg1;
}

void fn_1_6C44(s16 *arg0, s16 *arg1)
{
    lbl_1_bss_1360->motId[0] = arg0;
    lbl_1_bss_1360->motId[1] = arg1;
}

void fn_1_6C68(void)
{
    s32 i;

    for (i = 0; i < 2; i++) {
        (&lbl_1_bss_1360->ents[0x4e])[i].model = CharModelCreate(lbl_1_bss_1360->player[i].charNo, 1);
        lbl_1_bss_1360->ents[i + 0x4e].pos.x = 0.0f;
        lbl_1_bss_1360->ents[i + 0x4e].pos.y = 0.0f;
        lbl_1_bss_1360->ents[i + 0x4e].pos.z = 100.0f - (f32)(i * 0xc8);
        lbl_1_bss_1360->ents[i + 0x4e].rot.x = 0.0f;
        lbl_1_bss_1360->ents[i + 0x4e].rot.y = -90.0f;
        lbl_1_bss_1360->ents[i + 0x4e].rot.z = 0.0f;
        Hu3DModelAttrSet((&lbl_1_bss_1360->ents[0x4e])[i].model, 0x40000001);
        Hu3DModelCameraSet((&lbl_1_bss_1360->ents[0x4e])[i].model, 1);
        Hu3DModelLayerSet((&lbl_1_bss_1360->ents[0x4e])[i].model, 0);
    }
}

void fn_1_6E80(void)
{
    s32 i;
    s32 a;
    s32 b;
    s32 c;
    s32 d;

    for (i = 0; i < 2; i++) {
        if (lbl_1_bss_1360->player[i].dif != 0) {
            memset(&lbl_1_bss_1360->player[i].btn, 0, 0x14);
        } else {
            lbl_1_bss_1360->player[i].btn = HuPadBtn[lbl_1_bss_1360->player[i].padNo];
            lbl_1_bss_1360->player[i].btnDown = HuPadBtnDown[lbl_1_bss_1360->player[i].padNo];
            lbl_1_bss_1360->player[i].btnRep = HuPadBtnRep[lbl_1_bss_1360->player[i].padNo];
            lbl_1_bss_1360->player[i].stkX = (s32)HuPadStkX[lbl_1_bss_1360->player[i].padNo];
            if (lbl_1_bss_1360->player[i].stkX >= 0) {
                a = lbl_1_bss_1360->player[i].stkX;
            } else {
                a = -lbl_1_bss_1360->player[i].stkX;
            }
            if (a < 10) {
                b = 0;
            } else {
                b = lbl_1_bss_1360->player[i].stkX;
            }
            lbl_1_bss_1360->player[i].stkX = b;
            lbl_1_bss_1360->player[i].stkY = (s32)HuPadStkY[lbl_1_bss_1360->player[i].padNo];
            if (lbl_1_bss_1360->player[i].stkY >= 0) {
                c = lbl_1_bss_1360->player[i].stkY;
            } else {
                c = -lbl_1_bss_1360->player[i].stkY;
            }
            if (c < 10) {
                d = 0;
            } else {
                d = lbl_1_bss_1360->player[i].stkY;
            }
            lbl_1_bss_1360->player[i].stkY = d;
            lbl_1_bss_1360->player[i].subStkX = (s32)HuPadSubStkX[lbl_1_bss_1360->player[i].padNo];
            lbl_1_bss_1360->player[i].subStkY = (s32)HuPadSubStkY[lbl_1_bss_1360->player[i].padNo];
            lbl_1_bss_1360->player[i].trigL = HuPadTrigL[lbl_1_bss_1360->player[i].padNo];
            lbl_1_bss_1360->player[i].trigR = HuPadTrigR[lbl_1_bss_1360->player[i].padNo];
            lbl_1_bss_1360->player[i].dStk = HuPadDStk[lbl_1_bss_1360->player[i].padNo];
            lbl_1_bss_1360->player[i].dStkRep = HuPadDStkRep[lbl_1_bss_1360->player[i].padNo];
        }
    }
}

void fn_1_733C(void)
{
    s16 i;
    s16 j;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 5; j++) {
            lbl_1_bss_1360->motId[i][j] = CharMotionCreate(lbl_1_bss_1360->player[i].charNo, lbl_1_bss_1360->motData[i][j]);
        }
    }
    for (i = 0; i < 2; i++) {
        lbl_1_bss_1360->slot[i].motCur = -1;
        lbl_1_bss_1360->slot[i].speed = 1.0f;
        lbl_1_bss_1360->slot[i].speedCur = 1.0f;
    }
}

void fn_1_7490(void)
{
    s16 i;
    s16 idx;
    M554SLOT *slot;

    for (i = 0; i < 2; i++) {
        slot = &lbl_1_bss_1360->slot[i];
        idx = i + 0x4e;
        if (slot->mot != slot->motCur) {
            Hu3DMotionSet(lbl_1_bss_1360->ents[idx].model, lbl_1_bss_1360->motId[i][slot->mot]);
            slot->motCur = slot->mot;
            slot->maxTime = Hu3DMotionMaxTimeGet(lbl_1_bss_1360->ents[idx].model);
        }
        if (slot->speed != slot->speedCur) {
            Hu3DMotionSpeedSet(lbl_1_bss_1360->ents[idx].model, slot->speed);
            slot->speedCur = slot->speed;
        }
        if (slot->attrSet != slot->attrCur) {
            Hu3DModelAttrSet(lbl_1_bss_1360->ents[idx].model, slot->attrSet);
            slot->attrCur = slot->attrSet;
        }
        if (slot->attrReset != 0) {
            Hu3DModelAttrReset(lbl_1_bss_1360->ents[idx].model, slot->attrReset);
            slot->attrReset = ~slot->attrReset;
            slot->attrCur = slot->attrSet = slot->attrSet & slot->attrReset;
            slot->attrReset = 0;
        }
        if (slot->time != slot->timeCur) {
            Hu3DMotionTimeSet(lbl_1_bss_1360->ents[idx].model, slot->time);
        }
        switch (slot->shiftState) {
        case 0:
            break;
        case 1:
            Hu3DMotionShiftSet(lbl_1_bss_1360->ents[idx].model, lbl_1_bss_1360->motId[i][slot->shiftMot], slot->shiftStart, slot->shiftEnd, slot->shiftAttr);
            slot->shiftMax = Hu3DMotionShiftMaxTimeGet(lbl_1_bss_1360->ents[idx].model);
            slot->shiftState = 2;
            break;
        case 2:
            slot->shiftTime = Hu3DMotionShiftTimeGet(lbl_1_bss_1360->ents[idx].model);
            if ((slot->shiftAttr & 0x40000001) == 0) {
                if (slot->shiftTime >= slot->shiftEnd - 1.0f) {
                    slot->shiftState = 3;
                }
            }
            break;
        case 3:
            slot->maxTime = Hu3DMotionMaxTimeGet(lbl_1_bss_1360->ents[idx].model);
            slot->timeCur = slot->time;
            slot->shiftState = 0;
            slot->mot = slot->motCur = slot->shiftMot;
            break;
        }
        slot->time = Hu3DMotionTimeGet(lbl_1_bss_1360->ents[idx].model);
        slot->timeCur = slot->time;
        slot->endF = Hu3DMotionEndCheck(lbl_1_bss_1360->ents[idx].model);
        if (slot->endF != 0 && slot->f_28 != 0) {
            slot->endF = 0;
            slot->f_28 = 0;
            slot->mot = slot->f_2a;
            slot->time = slot->f_2c;
            slot->attrSet = slot->f_30;
            slot->attrReset = slot->f_34;
        }
    }
}

void fn_1_7880(void)
{
    fn_1_78F4();
    fn_1_7D74();
    fn_1_7FCC();
}

void fn_1_78A8(void)
{
    s32 i;
    M554OBJ *o;

    o = &lbl_1_bss_1360->obj[0];
    for (i = 0; i < 4; i++, o++) {
        o->flag = 1;
    }
}

void fn_1_78F4(void)
{
    f32 h[5] = { 800.0f, 400.0f, 500.0f, 350.0f, 250.0f };
    f32 sc[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    s32 i;
    s32 j;
    M554OBJ *o;
    M554ENTRY *e;

    for (i = 0; i < 4; i++) {
        o = &lbl_1_bss_1360->obj[i];
        e = &lbl_1_bss_1360->ents[i + 0x50];
        if (o->state == 0) {
            o->state = 1;
            o->timer[0] = 0;
            o->timer[1] = 0;
            o->timer[2] = 0;
            o->f_2c = ((i * 0x23) + 0x82) - (rand8() % 20);
            o->f_30 = (i * 0x190) + 0x9c4 + (i * 5);
            o->f_34 = 0.125 + 0.0165 * (f64)i;
            o->f_38 = h[i];
            Hu3DModelScaleSet(e->model, sc[i], sc[i], 1.0f);
            e->pos.x = o->f_30 * sin(3.141592653589793 * o->f_2c / 180.0);
            e->pos.z = o->f_30 * cos(3.141592653589793 * o->f_2c / 180.0);
            e->pos.y = 100.0f + o->f_38;
            Hu3DModelAttrReset(e->model, 1);
            Hu3DMotionTimeSet(e->model, 50.0f);
            Hu3DMotionSpeedSet(e->model, 0.5f);
            Hu3DModelPosSetV(e->model, &e->pos);
        }
        for (j = 0; j < 3; j++) {
            o->timer[j] += 1;
            switch (j) {
            case 0:
                o->f_2c += o->f_34;
                if (o->f_2c >= 240.0f) {
                    o->f_2c = 100.0f;
                }
                break;
            case 1:
                if (o->timer[j] >= 0x64) {
                    o->timer[j] = 0;
                    Hu3DMotionTimeSet(e->model, 50.0f);
                }
                break;
            case 2:
#ifdef TARGET_PC
                if (o->flag != 0) { /* timer[j + 1] is the flag field; avoid indexing past timer[] */
#else
                if (o->timer[j + 1] != 0) {
#endif
                    o->f_30 += 20.0f;
                }
                break;
            }
        }
        e->pos.x = o->f_30 * sin(3.141592653589793 * o->f_2c / 180.0);
        e->pos.z = o->f_30 * cos(3.141592653589793 * o->f_2c / 180.0);
        e->pos.y = 100.0f + o->f_38;
        e->rot.y = 180.0f + o->f_2c;
    }
}

void fn_1_7D74(void)
{
    s32 idx[4] = { 0x56, 0x58, 0x57, 0x59 };
    s32 i;
    M554OBJ *o;
    M554ENTRY *e;

    for (i = 0; i < 4; i++) {
        o = &lbl_1_bss_1360->obj[i + 4];
        e = &lbl_1_bss_1360->ents[idx[i]];
        if (o->state == 0) {
            o->state = 1;
            o->timer[0] = 0;
            e->pos.x = 0.0f;
            e->pos.y = 0.0f;
            e->pos.z = 0.0f;
            e->rot.y = (i * 0x1e) - 0xf;
            Hu3DModelAttrReset(e->model, 1);
            Hu3DMotionTimeSet(e->model, (f32)(i * 0x19));
            Hu3DMotionSpeedSet(e->model, 0.5f);
            Hu3DModelPosSetV(e->model, &e->pos);
            Hu3DModelRotSetV(e->model, &e->rot);
            Hu3DModelAttrSet(e->model, 0x40000001);
        }
        o->timer[0] = o->timer[0] + 1;
        e->rot.y = e->rot.y - 0.01;
    }
    e = lbl_1_bss_1360->ents;
    e->rot.y = e->rot.y - 0.005f;
}

void fn_1_7F68(void)
{
    lbl_1_bss_1360->obj[9].timer[0] = -1; /* GC offset 0x584 of the work area */
}

void fn_1_7F80(void)
{
}

void fn_1_7F84(void)
{
    s32 i;

    for (i = 0; i < 2; i++) {
        lbl_1_bss_1360->obj[i + 8].flag = -1;
    }
}

void fn_1_7FCC(void)
{
    s32 idx[2] = { 0x5a, 0x5b };
    s32 i;
    M554OBJ *o;
    M554ENTRY *e;

    for (i = 0; i < 2; i++) {
        o = &lbl_1_bss_1360->obj[i + 8];
        e = &lbl_1_bss_1360->ents[idx[i]];
        switch (o->state) {
        case 0:
            o->state = 1;
            o->timer[0] = 0;
            o->flag = 0;
            Hu3DModelAttrReset(e->model, 1);
            Hu3DMotionTimeSet(e->model, 0.0f);
            Hu3DMotionSpeedSet(e->model, 1.0f);
            Hu3DModelAttrSet(e->model, 0x40000001);
            o->f_2c = 70.0f - 0.3f * (f32)(rand8() % 10);
            e->pos.y = 1352.0f;
            if (i == 0) {
                o->timer[0] = 0x23a;
                o->f_2c = 80.0f;
            } else {
                o->timer[0] = 0xc8;
                o->f_2c = -(0.3f * (f32)(rand8() % 10));
            }
            o->f_34 = 1.1 + 0.1 * (f64)(rand8() % 40);
            o->f_30 = 1770.0f;
            o->f_38 = o->f_30 - 270.0f;
            e->pos.x = o->f_30 * sin(3.141592653589793 * o->f_2c / 180.0);
            e->pos.z = -600.0 + o->f_30 * cos(3.141592653589793 * o->f_2c / 180.0);
            break;
        case 1:
            if (o->timer[0]++ > 0x258) {
                if (o->flag == 0) {
                    o->state = o->state + 1;
                }
                o->timer[0] = 0x30;
            }
            e->pos.y = 1352.0f;
            e->pos.x = o->f_30 * sin(3.141592653589793 * o->f_2c / 180.0);
            e->pos.z = -600.0 + o->f_30 * cos(3.141592653589793 * o->f_2c / 180.0);
            break;
        case 2:
            o->timer[0] = o->timer[0] - 1;
            e->pos.y -= 24.0f;
            o->f_30 -= o->f_38 / 48.0f;
            o->f_2c -= o->f_34;
            e->pos.x = o->f_30 * sin(3.141592653589793 * o->f_2c / 180.0);
            e->pos.z = -600.0 + o->f_30 * cos(3.141592653589793 * o->f_2c / 180.0);
            e->rot.y -= 15.0f;
            if (o->timer[0] == 0) {
                o->state = o->state + 1;
                HuAudFXPlay(0x891);
            }
            break;
        case 3:
            o->timer[0] = o->timer[0] + 1;
            if (e->pos.y >= 520.0f) {
                e->pos.y += 23.0f;
                o->f_2c -= 10.0;
                o->f_30 += 15.2f;
            } else {
                e->pos.y += 3.5;
                o->f_2c -= 6.0;
                o->f_30 += 1.3f;
            }
            e->pos.x = o->f_30 * sin(3.141592653589793 * o->f_2c / 180.0);
            e->pos.z = -600.0 + o->f_30 * cos(3.141592653589793 * o->f_2c / 180.0);
            e->rot.y -= 25.0f;
            if (o->timer[0] >= 0x96) {
                o->timer[0] = rand8() - 0x190;
                if (o->flag == 0) {
                    o->state = 1;
                } else {
                    o->state = 4;
                }
                if (i != 0) {
                    o->f_2c = 0.5 * (f64)(rand8() % 40);
                } else {
                    o->f_2c = 55.0 + 0.5 * (f64)(rand8() % 35);
                }
                o->f_34 = 1.1 + 0.1 * (f64)(rand8() % 40);
                o->f_30 = 1570.0f;
                o->f_30 += (f32)((rand8() % 9) * 0x32);
                o->f_38 = o->f_30 - 270.0f;
            }
            break;
        }
    }
}
