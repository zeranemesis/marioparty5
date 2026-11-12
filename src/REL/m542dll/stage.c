// m542dll/stage.c - decompilation of the m542dll/REL/m542dll/stage unit.

#define _MATH_H /* keeps MWCC from adding its 16 bytes of sqrt literals to .rodata */
#include "types.h"
#include "string.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/object.h"
#include "game/pad.h"
#include "REL/m542dll/m542types.h"

float Hu3DMotionMaxTimeGet(HU3DMODELID modelId);

/* ---- bss ---- */
M542WORK *lbl_1_bss_1B38;

void fn_1_84D0(M542WORK *work, M542ENTRY *ents, s32 *data)
{
    s16 j;
    s16 i;
    s16 t;

    lbl_1_bss_1B38 = work;
    memset(lbl_1_bss_1B38, 0, sizeof(M542WORK));
    lbl_1_bss_1B38->ents = ents;
    lbl_1_bss_1B38->data = data;
    i = 0;
    do {
        for (j = 0; j < 4; j++) {
            if (i == (t = GwPlayerConf[j].grp)) {
                lbl_1_bss_1B38->player[i].playerNo = j;
                i++;
                if (i >= 2) {
                    break;
                }
            }
        }
    } while (i < 2);
    for (j = 0; j < 2; j++) {
        lbl_1_bss_1B38->player[j].charNo = GwPlayerConf[lbl_1_bss_1B38->player[j].playerNo].charNo;
        lbl_1_bss_1B38->player[j].padNo = GwPlayerConf[lbl_1_bss_1B38->player[j].playerNo].padNo;
        if ((t = GwPlayerConf[lbl_1_bss_1B38->player[j].playerNo].type) != 0) {
            lbl_1_bss_1B38->player[j].dif = GwPlayerConf[lbl_1_bss_1B38->player[j].playerNo].dif + 1;
        }
    }
}

void fn_1_871C(s16 arg0)
{
    s16 t;
    s16 model;
    s16 i;

    lbl_1_bss_1B38->f_9c = arg0;
    for (i = 0; i < 6; i++) {
        t = (s16)i;
        model = lbl_1_bss_1B38->ents[t].model = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_bss_1B38->data[i], 0x10000000, HEAP_MODEL));
        Hu3DModelAttrSet(model, 1);
        Hu3DModelCameraSet(model, 1);
        Hu3DModelPosSetV(model, &lbl_1_data_3B0[i]);
        Hu3DModelRotSet(model, 0.0f, 0.0f, 0.0f);
        if (i != 0) {
            if (i == 4) {
                Hu3DMotionSpeedSet(model, 0.5f);
            } else {
                Hu3DMotionSpeedSet(model, 1.0f);
            }
            Hu3DMotionTimeSet(model, 0.0f);
            Hu3DModelAttrSet(model, 0x40000001);
        }
        lbl_1_bss_1B38->ents[t].pos.x = lbl_1_data_3B0[i].x;
        lbl_1_bss_1B38->ents[t].pos.y = lbl_1_data_3B0[i].y;
        lbl_1_bss_1B38->ents[t].pos.z = lbl_1_data_3B0[i].z;
        lbl_1_bss_1B38->ents[t].rot.x = 0.0f;
        lbl_1_bss_1B38->ents[t].rot.y = 0.0f;
        lbl_1_bss_1B38->ents[t].rot.z = 0.0f;
        Hu3DModelLayerSet(model, 0);
        Hu3DModelAttrReset(model, 1);
    }
}

void fn_1_89DC(s32 *arg0, s32 *arg1)
{
    lbl_1_bss_1B38->motData[0] = arg0;
    lbl_1_bss_1B38->motData[1] = arg1;
}

void fn_1_8A00(s16 *arg0, s16 *arg1)
{
    lbl_1_bss_1B38->motId[0] = arg0;
    lbl_1_bss_1B38->motId[1] = arg1;
}

void fn_1_8A24(void)
{
    s32 i;

    for (i = 0; i < 2; i++) {
        (&lbl_1_bss_1B38->ents[0x20])[i].model = CharModelCreate(lbl_1_bss_1B38->player[i].charNo, 2);
        Hu3DModelPosSet((&lbl_1_bss_1B38->ents[0x20])[i].model, (f32)(i * 0x1a4 - 0xd2), 170.0f, 254.0f);
        lbl_1_bss_1B38->ents[i + 0x20].pos.x = (f32)(i * 0x1a4 - 0xd2);
        lbl_1_bss_1B38->ents[i + 0x20].pos.y = 170.0f;
        lbl_1_bss_1B38->ents[i + 0x20].pos.z = 254.0f;
        lbl_1_bss_1B38->ents[i + 0x20].rot.x = 0.0f;
        lbl_1_bss_1B38->ents[i + 0x20].rot.y = 0.0f;
        lbl_1_bss_1B38->ents[i + 0x20].rot.z = 0.0f;
        Hu3DModelAttrSet((&lbl_1_bss_1B38->ents[0x20])[i].model, 0x40000001);
        Hu3DModelCameraSet((&lbl_1_bss_1B38->ents[0x20])[i].model, 1);
        Hu3DModelLayerSet((&lbl_1_bss_1B38->ents[0x20])[i].model, 0);
    }
}

void fn_1_8C94(void)
{
    s32 i;
    s32 a;
    s32 b;
    s32 c;
    s32 d;

    for (i = 0; i < 2; i++) {
        if (lbl_1_bss_1B38->player[i].dif != 0) {
            memset(&lbl_1_bss_1B38->player[i].btn, 0, 0x14);
        } else {
            lbl_1_bss_1B38->player[i].btn = HuPadBtn[lbl_1_bss_1B38->player[i].padNo];
            lbl_1_bss_1B38->player[i].btnDown = HuPadBtnDown[lbl_1_bss_1B38->player[i].padNo];
            lbl_1_bss_1B38->player[i].btnRep = HuPadBtnRep[lbl_1_bss_1B38->player[i].padNo];
            lbl_1_bss_1B38->player[i].stkX = (s32)HuPadStkX[lbl_1_bss_1B38->player[i].padNo];
            if (lbl_1_bss_1B38->player[i].stkX >= 0) {
                a = lbl_1_bss_1B38->player[i].stkX;
            } else {
                a = -lbl_1_bss_1B38->player[i].stkX;
            }
            if (a < 10) {
                b = 0;
            } else {
                b = lbl_1_bss_1B38->player[i].stkX;
            }
            lbl_1_bss_1B38->player[i].stkX = b;
            lbl_1_bss_1B38->player[i].stkY = (s32)HuPadStkY[lbl_1_bss_1B38->player[i].padNo];
            if (lbl_1_bss_1B38->player[i].stkY >= 0) {
                c = lbl_1_bss_1B38->player[i].stkY;
            } else {
                c = -lbl_1_bss_1B38->player[i].stkY;
            }
            if (c < 10) {
                d = 0;
            } else {
                d = lbl_1_bss_1B38->player[i].stkY;
            }
            lbl_1_bss_1B38->player[i].stkY = d;
            lbl_1_bss_1B38->player[i].subStkX = (s32)HuPadSubStkX[lbl_1_bss_1B38->player[i].padNo];
            lbl_1_bss_1B38->player[i].subStkY = (s32)HuPadSubStkY[lbl_1_bss_1B38->player[i].padNo];
            lbl_1_bss_1B38->player[i].trigL = HuPadTrigL[lbl_1_bss_1B38->player[i].padNo];
            lbl_1_bss_1B38->player[i].trigR = HuPadTrigR[lbl_1_bss_1B38->player[i].padNo];
            lbl_1_bss_1B38->player[i].dStk = HuPadDStk[lbl_1_bss_1B38->player[i].padNo];
            lbl_1_bss_1B38->player[i].dStkRep = HuPadDStkRep[lbl_1_bss_1B38->player[i].padNo];
        }
    }
}

void fn_1_9150(void)
{
    s16 i;
    s16 j;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 9; j++) {
            lbl_1_bss_1B38->motId[i][j] = CharMotionCreate(lbl_1_bss_1B38->player[i].charNo, lbl_1_bss_1B38->motData[i][j]);
        }
    }
    for (i = 0; i < 2; i++) {
        lbl_1_bss_1B38->slot[i].motCur = -1;
        lbl_1_bss_1B38->slot[i].speed = 1.0f;
        lbl_1_bss_1B38->slot[i].speedCur = 1.0f;
    }
}

void fn_1_92A4(void)
{
    M542SLOT *slot;
    s16 idx;
    s16 i;

    for (i = 0; i < 2; i++) {
        slot = &lbl_1_bss_1B38->slot[i];
        idx = i + 0x20;

        if (slot->mot != slot->motCur) {
            Hu3DMotionSet(lbl_1_bss_1B38->ents[idx].model, lbl_1_bss_1B38->motId[i][slot->mot]);
            slot->motCur = slot->mot;
            slot->maxTime = Hu3DMotionMaxTimeGet(lbl_1_bss_1B38->ents[idx].model);
        }
        if (slot->speed != slot->speedCur) {
            Hu3DMotionSpeedSet(lbl_1_bss_1B38->ents[idx].model, slot->speed);
            slot->speedCur = slot->speed;
        }
        if (slot->attrSet != slot->attrCur) {
            Hu3DModelAttrSet(lbl_1_bss_1B38->ents[idx].model, slot->attrSet);
            slot->attrCur = slot->attrSet;
        }
        if (slot->attrReset != 0) {
            Hu3DModelAttrReset(lbl_1_bss_1B38->ents[idx].model, slot->attrReset);
            slot->attrCur = slot->attrSet = slot->attrReset = 0;
        }
        if (slot->time != slot->timeCur) {
            Hu3DMotionTimeSet(lbl_1_bss_1B38->ents[idx].model, slot->time);
        }
        slot->time = Hu3DMotionTimeGet(lbl_1_bss_1B38->ents[idx].model);
        slot->timeCur = slot->time;
    }
}

void fn_1_94B4(void)
{
}
