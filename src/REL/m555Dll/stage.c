// m555Dll/REL/m555Dll/stage: control record (bss_2D38) setup, pad polling,
// character model/motion creation and per-frame motion driver.

#include "string.h"
#include "game/charman.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/pad.h"
#include "REL/m555Dll.h"

/* Hu3DMotionMaxTimeGet is deliberately not declared in hu3d.h (see there). */
float Hu3DMotionMaxTimeGet(HU3DMODELID modelId);

/* The unit's own pointer to the live control record (stage.o's only .bss). */
M555_CTRL *lbl_1_bss_3038;

#define C lbl_1_bss_3038

/* Binds the control record and clears it, then picks the two team members. */
void fn_1_1A9A4(M555_CTRL *ctrl, M555_MODELENTRY *mdl, s32 *dataList)
{
    s16 i;
    s16 j;
    s16 conf;

    C = ctrl;
    memset(C, 0, sizeof(M555_CTRL));
    C->mdl = mdl;
    C->dataList = dataList;
    i = 0;
    do {
        for (j = 0; j < 4; j++) {
            conf = GwPlayerConf[j].grp;
            if (i == conf) {
                C->mdl0[i].playerNo = j;
                i++;
                if (i >= 2) {
                    break;
                }
            }
        }
    } while (i < 2);
    for (j = 0; j < 2; j++) {
        C->mdl0[j].charNo = GwPlayerConf[C->mdl0[j].playerNo].charNo;
        C->mdl0[j].padNo = GwPlayerConf[C->mdl0[j].playerNo].padNo;
        conf = GwPlayerConf[C->mdl0[j].playerNo].type;
        if (conf != 0) {
            C->mdl0[j].charSlot = GwPlayerConf[C->mdl0[j].playerNo].dif + 1;
        }
    }
}

void fn_1_1ABF0(s16 value)
{
    C->mode174 = value;
}

void fn_1_1AC04(s16 idx, s32 *data)
{
    C->charData[idx] = data;
}

void fn_1_1AC24(s16 idx, s16 *motion)
{
    C->motion[idx] = motion;
}

/* Creates the two character models and their motion-window records. */
void fn_1_1AC44(void)
{
    s32 i;
    M555_CTRLWIN *w;

    for (i = 0; i < 2; i++) {
        C->mdl[i + 0xA9].mdlId = CharModelCreate(C->mdl0[i].charNo, 2);
        Hu3DModelPosSet(C->mdl[i + 0xA9].mdlId, -150.0f + 300.0f * i, 100.0f, 0.0f);
        CharEffectLayerSet(0);
        CharModelFxFlagSet(C->mdl0[i].charNo, 0);
        w = &C->win[i];
        w->on = 0;
        w->scale = 1.0f;
        w->alpha = 0x40000001;
        w->flags = 0;
        w->value = 0.0f;
    }
    C->mdl[0xA9].pos.x = -200.0f;
    C->mdl[0xA9].pos.y = 0.0f;
    C->mdl[0xA9].pos.z = 0.0f;
    C->mdl[0xA9].rot.x = 0.0f;
    C->mdl[0xA9].rot.y = 180.0f;
    C->mdl[0xA9].rot.z = 0.0f;
    C->mdl[0xAA].pos.x = 200.0f;
    C->mdl[0xAA].pos.y = 0.0f;
    C->mdl[0xAA].pos.z = 0.0f;
    C->mdl[0xAA].rot.x = 0.0f;
    C->mdl[0xAA].rot.y = 180.0f;
    C->mdl[0xAA].rot.z = 0.0f;
}

/* Polls each team member's pad into the control record. */
void fn_1_1AF38(OMOBJ *obj)
{
    s32 i;
    s8 stk;
    s8 dead;

    for (i = 0; i < 2; i++) {

        C->mdl0[i].btn = HuPadBtn[C->mdl0[i].padNo];
        C->mdl0[i].btnDown = HuPadBtnDown[C->mdl0[i].padNo];
        C->mdl0[i].btnRep = HuPadBtnRep[C->mdl0[i].padNo];
        C->mdl0[i].stickX = HuPadStkX[C->mdl0[i].padNo];
        if (C->mdl0[i].stickX >= 0) {
            dead = C->mdl0[i].stickX;
        } else {
            dead = -C->mdl0[i].stickX;
        }
        if (dead < 10) {
            stk = 0;
        } else {
            stk = C->mdl0[i].stickX;
        }
        C->mdl0[i].stickX = stk;
        C->mdl0[i].stickY = (s8)(s32)HuPadStkY[C->mdl0[i].padNo];
        C->mdl0[i].subStickX = (s8)(s32)HuPadSubStkX[C->mdl0[i].padNo];
        C->mdl0[i].subStickY = (s8)(s32)HuPadSubStkY[C->mdl0[i].padNo];
        C->mdl0[i].trigL = HuPadTrigL[C->mdl0[i].padNo];
        /* the original writes these three one record past its stride (into the next entry) */
        ((u8 *)C)[i * 0x1C + 0x1C] = HuPadTrigR[C->mdl0[i].padNo];
        ((u8 *)C)[i * 0x1C + 0x1D] = HuPadDStk[C->mdl0[i].padNo];
        ((u8 *)C)[i * 0x1C + 0x1E] = HuPadDStkRep[C->mdl0[i].padNo];
    }
}

/* Builds the per-character motion ids and resets both motion windows. */
void fn_1_1B2F8(void)
{
    s16 i;
    s16 j;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            C->motion[i][j] = CharMotionCreate(C->mdl0[i].charNo, C->charData[i][j]);
        }
    }
    for (i = 0; i < 2; i++) {
        C->win[i].on = 0;
        C->win[i].prevMot = -1;
        C->win[i].scale = 1.0f;
        C->win[i].value = 0.0f;
        C->win[i].alpha = 0x40000001;
        C->win[i].flags = 0;
    }
}

/* Per-frame driver: pushes the motion window state into the Hu3D model. */
void fn_1_1B4B0(OMOBJ *obj)
{
    s16 i;
    s16 id;
    M555_CTRLWIN *w;

    for (i = 0; i < 2; i++) {
        w = &C->win[i];
        id = i + 0xA9;
        if (w->on != w->prevMot) {
            Hu3DMotionSet(C->mdl[id].mdlId, C->motion[i][w->on]);
            w->prevMot = w->on;
            w->maxTime = Hu3DMotionMaxTimeGet(C->mdl[id].mdlId);
        }
        if (w->scale != w->prevScale) {
            Hu3DMotionSpeedSet(C->mdl[id].mdlId, w->scale);
            w->prevScale = w->scale;
        }
        if ((u32)w->alpha != (u32)w->prevAttr) {
            Hu3DModelAttrSet(C->mdl[id].mdlId, w->alpha);
            w->prevAttr = w->alpha;
        }
        if ((u32)w->flags != 0) {
            Hu3DModelAttrReset(C->mdl[id].mdlId, w->flags);
            w->flags = ~w->flags;
            w->alpha &= w->flags;
            w->prevAttr = w->alpha;
            w->flags = 0;
        }
        if (w->value != w->angle) {
            Hu3DMotionTimeSet(C->mdl[id].mdlId, w->value);
        }
        switch (w->on38) {
            case 0:
                break;
            case 1:
                Hu3DMotionShiftSet(C->mdl[id].mdlId, C->motion[i][w->on3A], w->val40, w->val44, w->val3C);
                w->shiftMax = Hu3DMotionShiftMaxTimeGet(C->mdl[id].mdlId);
                w->on38 = 2;
                break;
            case 2:
                w->shiftTime = Hu3DMotionShiftTimeGet(C->mdl[id].mdlId);
                if ((w->val3C & 0x40000001) == 0 && w->shiftTime >= w->val44 - 1.0f) {
                    w->on38 = 3;
                }
                break;
            case 3:
                w->on38 = 0;
                break;
        }
        w->value = Hu3DMotionTimeGet(C->mdl[id].mdlId);
        w->angle = w->value;
        w->endFlag = Hu3DMotionEndCheck(C->mdl[id].mdlId);
        if (w->endFlag != 0 && w->endAction != 0) {
            w->endFlag = 0;
            w->endAction = 0;
            w->on = w->endMot;
            w->value = w->endTime;
            w->alpha = w->endAttr;
            w->flags = w->endAttrReset;
        }
    }
}

void fn_1_1B868(void)
{
}

void fn_1_1B86C(void)
{
}

void fn_1_1B870(void)
{
}

void fn_1_1B874(void)
{
}

void fn_1_1B878(void)
{
}

void fn_1_1B87C(void)
{
}

void fn_1_1B880(void)
{
}
