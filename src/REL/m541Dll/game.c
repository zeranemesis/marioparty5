#include "REL/m541Dll.h"
#include "string.h"
#include "math.h"
#include "game/frand.h"
#include "game/charman.h"
#include "game/mg/actman.h"
#include <math.h>

/* ---- part_g1.c ---- */
/* ---- game.s data / bss owned by part G1 ---- */
float lbl_1_data_368[8][4] = {
    { 230.0f, -554.0f, 554.0f, -230.0f },
    { 554.0f, -230.0f, 554.0f, 230.0f },
    { 554.0f, 230.0f, 230.0f, 554.0f },
    { 230.0f, 554.0f, -230.0f, 554.0f },
    { -230.0f, 554.0f, -554.0f, 230.0f },
    { -554.0f, 230.0f, -554.0f, -230.0f },
    { -554.0f, -230.0f, -230.0f, -554.0f },
    { -230.0f, -554.0f, 230.0f, -554.0f },
};

HuVecF lbl_1_data_3E8[8] = {
    { -1.0f, 1.0f, 768.0f },
    { 1.0f, 0.0f, -554.0f },
    { 1.0f, 1.0f, -768.0f },
    { 0.0f, 1.0f, -554.0f },
    { -1.0f, 1.0f, -768.0f },
    { 1.0f, 0.0f, 554.0f },
    { 1.0f, 1.0f, 768.0f },
    { 0.0f, 1.0f, 554.0f },
};

float lbl_1_data_48C[25] = {
    0.0f, 0.0f, 0.0f, 0.0f, 913.0f, 0.0f, -1177.0f, 993.0f, 0.0f, -1038.0f,
    550.0f, 1000.0f, -3250.0f, 593.0f, 312.0f, -1119.0f, -462.0f, 330.0f, -1100.0f, -1000.0f,
    40.0f, -2100.0f, -2500.0f, 0.0f, -3900.0f,
};

s32 lbl_1_bss_36A0;
M541_G1TRI lbl_1_bss_36A4[23];
M541_STAGE *lbl_1_bss_3730;

#define MDL(i) (lbl_1_bss_3730->model[i])
#define TRI lbl_1_bss_36A4

void fn_1_2AD4(M541_STAGE *stage)
{
    s16 idx[3];
    float rotTbl[8];
    MGACTOR_PARAM param;
    s16 i;
    s16 j;

    memset(&lbl_1_bss_2734, 0, sizeof(lbl_1_bss_2734));
    memset(&lbl_1_bss_2614, 0, sizeof(lbl_1_bss_2614));
    memset(&lbl_1_bss_45C, 0, sizeof(lbl_1_bss_45C));
    memset(&lbl_1_bss_2F0, 0, sizeof(lbl_1_bss_2F0));
    memset(&lbl_1_bss_20, 0, sizeof(lbl_1_bss_20));
    memset(&lbl_1_bss_0, 0, sizeof(lbl_1_bss_0));
    lbl_1_bss_3730 = stage;
    for (i = 0; i < 4; i++) {
        lbl_1_bss_2734.playerHook[i] = -1;
    }
    lbl_1_bss_2614[0].tpLvl = 255.0f;
    lbl_1_bss_2614[1].tpLvl = 255.0f;
    for (i = 0; i < 23; i++) {
        TRI[i].a = i * 3 + 36;
        TRI[i].b = i * 3 + 37;
        TRI[i].c = i * 3 + 38;
    }
    lbl_1_bss_2F0.length = 4.0f;
    for (i = 0; i < 10; i++) {
        lbl_1_bss_45C.aiTimer[i] = frandmod(180) + 240;
        lbl_1_bss_45C.aiMode[i] = frandmod(8);
    }
    for (i = 15, j = 0; i < 23; i++, j++) {
        lbl_1_bss_45C.leafPos[i].x = lbl_1_data_368[j][0];
        lbl_1_bss_45C.leafPos[i].y = 0.0f;
        lbl_1_bss_45C.leafPos[i].z = lbl_1_data_368[j][1];
    }
    for (i = 0; i < 15; i++) {
        lbl_1_bss_45C.aiPos[i].x = 10.0f * frandmod(36);
        lbl_1_bss_45C.aiPos[i].y = 50.0f + 10.0f * frandmod(45);
        lbl_1_bss_45C.aiPos[i].z = frandmod(360);
    }
    for (i = 0; i < 6; i++) {
        MDL(i).modelId = Hu3DModelCreateData(lbl_1_bss_3730->dataIds[i]);
        Hu3DModelAttrSet(MDL(i).modelId, 1);
        Hu3DModelCameraSet(MDL(i).modelId, 1);
        if (i == 3 || i == 4) {
            Hu3DModelLayerSet(MDL(i).modelId, 4);
        } else {
            Hu3DModelLayerSet(MDL(i).modelId, 1);
        }
        MDL(i).posX = 0.0f;
        MDL(i).posY = 0.0f;
        MDL(i).posZ = 0.0f;
        MDL(i).rotX = 0.0f;
        MDL(i).rotY = 0.0f;
        MDL(i).rotZ = 0.0f;
        if (i == 0) {
            Hu3DModelShadowMapSet(MDL(i).modelId);
        } else {
            if (i == 4) {
                Hu3DMotionSpeedSet(MDL(i).modelId, 0.5f);
            } else {
                Hu3DMotionSpeedSet(MDL(i).modelId, 1.0f);
            }
            Hu3DMotionTimeSet(MDL(i).modelId, 0.0f);
            Hu3DModelAttrSet(MDL(i).modelId, 0x40000001);
        }
        Hu3DModelAttrReset(MDL(i).modelId, 1);
    }
    MDL(105).modelId = Hu3DModelCreateData(lbl_1_bss_3730->dataIds[11]);
    Hu3DModelAttrSet(MDL(105).modelId, 1);
    Hu3DModelCameraSet(MDL(105).modelId, 1);
    MDL(105).posX = 0.0f;
    MDL(105).posY = 0.0f;
    MDL(105).posZ = 0.0f;
    MDL(105).rotX = 0.0f;
    MDL(105).rotY = 0.0f;
    MDL(105).rotZ = 0.0f;
    idx[0] = 6;
    idx[1] = 7;
    idx[2] = 10;
    for (i = 0; i < 23; i++) {
        MDL(TRI[i].a).modelId = Hu3DModelCreateData(lbl_1_bss_3730->dataIds[idx[0]]);
        Hu3DModelAttrSet(MDL(TRI[i].a).modelId, 1);
        Hu3DModelCameraSet(MDL(TRI[i].a).modelId, 1);
        Hu3DModelLayerSet(MDL(TRI[i].a).modelId, 4);
        MDL(TRI[i].a).posX = 0.0f;
        MDL(TRI[i].a).posY = 200.0f;
        MDL(TRI[i].a).posZ = 0.0f;
        MDL(TRI[i].a).rotX = 0.0f;
        MDL(TRI[i].a).rotY = 0.0f;
        MDL(TRI[i].a).rotZ = 0.0f;
        if (i < 15) {
            Hu3DModelScaleSet(MDL(TRI[i].a).modelId, 0.6f, 0.6f, 0.6f);
        } else {
            Hu3DModelScaleSet(MDL(TRI[i].a).modelId, 0.8f, 0.8f, 0.8f);
        }
        MDL(TRI[i].b).modelId = Hu3DModelCreateData(lbl_1_bss_3730->dataIds[idx[1]]);
        Hu3DModelAttrSet(MDL(TRI[i].b).modelId, 1);
        Hu3DModelCameraSet(MDL(TRI[i].b).modelId, 1);
        Hu3DModelLayerSet(MDL(TRI[i].b).modelId, 4);
        MDL(TRI[i].b).posX = 0.0f;
        MDL(TRI[i].b).posY = 200.0f;
        MDL(TRI[i].b).posZ = 0.0f;
        MDL(TRI[i].b).rotX = 0.0f;
        MDL(TRI[i].b).rotY = 0.0f;
        MDL(TRI[i].b).rotZ = 0.0f;
        if (i < 15) {
            Hu3DModelScaleSet(MDL(TRI[i].b).modelId, 0.6f, 0.6f, 0.6f);
        } else {
            Hu3DModelScaleSet(MDL(TRI[i].b).modelId, 0.8f, 0.8f, 0.8f);
        }
        MDL(TRI[i].c).modelId = Hu3DModelCreateData(lbl_1_bss_3730->dataIds[idx[2]]);
        Hu3DModelAttrSet(MDL(TRI[i].c).modelId, 9);
        Hu3DModelCameraSet(MDL(TRI[i].c).modelId, 1);
        Hu3DModelLayerSet(MDL(TRI[i].c).modelId, 3);
        Hu3DModelShadowSet(MDL(TRI[i].c).modelId);
        Hu3DModelShadowDispOff(MDL(TRI[i].c).modelId);
        Hu3DModelTPLvlSet(MDL(TRI[i].c).modelId, 1.0f);
        MDL(TRI[i].c).posX = 0.0f;
        MDL(TRI[i].c).posY = 0.0f;
        MDL(TRI[i].c).posZ = 0.0f;
        MDL(TRI[i].c).rotX = 0.0f;
        MDL(TRI[i].c).rotY = 0.0f;
        MDL(TRI[i].c).rotZ = 0.0f;
        Hu3DModelAttrReset(MDL(TRI[i].c).modelId, 1);
        if (i < 15) {
            Hu3DModelScaleSet(MDL(TRI[i].c).modelId, 0.6f, 0.6f, 0.6f);
        } else {
            Hu3DModelScaleSet(MDL(TRI[i].c).modelId, 0.8f, 0.8f, 0.8f);
        }
    }
    for (i = 0; i < 1; i++) {
        float randA, randB, rad;

        lbl_1_bss_20[i].unk0 = 1;
        lbl_1_bss_20[i].angle = frandmod(360);
        lbl_1_bss_20[0].angle = 90.0f;
        lbl_1_bss_20[i].vel.x = lbl_1_bss_2F0.length * cos(3.141592653589793 * lbl_1_bss_20[i].angle / 180.0);
        lbl_1_bss_20[i].vel.y = 0.0f;
        lbl_1_bss_20[i].vel.z = lbl_1_bss_2F0.length * sin(3.141592653589793 * lbl_1_bss_20[i].angle / 180.0);
        randA = frandmod(360);
        randB = frandmod(360);
        rad = frandmod(300);
        MDL(TRI[i].a).posX = rad * cos(3.141592653589793 * randA / 180.0);
        MDL(TRI[i].a).posY = 200.0f;
        MDL(TRI[i].a).posZ = rad * sin(3.141592653589793 * randB / 180.0);
        MDL(TRI[0].a).posX = 0.0f;
        MDL(TRI[0].a).posY = 200.0f;
        MDL(TRI[0].a).posZ = 0.0f;
        MDL(TRI[i].b).pos = MDL(TRI[i].a).pos;
        MDL(TRI[i].c).pos = MDL(TRI[i].a).pos;
        Hu3DModelAttrReset(MDL(TRI[i].a).modelId, 1);
        Hu3DModelAttrReset(MDL(TRI[i].b).modelId, 1);
        Hu3DModelAttrSet(MDL(TRI[i].b).modelId, 0x40000001);
        Hu3DModelShadowDispOn(MDL(TRI[i].c).modelId);
        idx[0] = frandmod(90);
        Hu3DMotionTimeSet(MDL(TRI[i].b).modelId, idx[0]);
        Hu3DMotionSpeedSet(MDL(TRI[i].b).modelId, 2.0f);
        lbl_1_bss_20[i].tpLvl = 1.0f;
        lbl_1_bss_20[i].exist = 1;
        lbl_1_bss_2F0.spawnCount++;
    }
    rotTbl[0] = 292.0f;
    rotTbl[1] = 337.0f;
    rotTbl[2] = 22.0f;
    rotTbl[3] = 67.0f;
    rotTbl[4] = 112.0f;
    rotTbl[5] = 157.0f;
    rotTbl[6] = 202.0f;
    rotTbl[7] = 247.0f;
    for (i = 15, j = 0; i < 23; i++, j++) {
        MDL(TRI[i].a).posX = lbl_1_data_368[j][0];
        MDL(TRI[i].a).posY = 200.0f;
        MDL(TRI[i].a).posZ = lbl_1_data_368[j][1];
        MDL(TRI[i].b).posX = lbl_1_data_368[j][0];
        MDL(TRI[i].b).posY = 200.0f;
        MDL(TRI[i].b).posZ = lbl_1_data_368[j][1];
        MDL(TRI[i].c).posX = lbl_1_data_368[j][0];
        MDL(TRI[i].c).posY = 200.0f;
        MDL(TRI[i].c).posZ = lbl_1_data_368[j][1];
        MDL(TRI[i].a).rotX = -20.0f;
        MDL(TRI[i].a).rotY = 270.0f + -rotTbl[j];
        MDL(TRI[i].a).rotZ = 0.0f;
        Hu3DModelAttrReset(MDL(TRI[i].a).modelId, 1);
        Hu3DModelAttrReset(MDL(TRI[i].b).modelId, 1);
        Hu3DModelAttrSet(MDL(TRI[i].b).modelId, 0x40000001);
        Hu3DModelShadowDispOn(MDL(TRI[i].c).modelId);
        idx[0] = frandmod(90);
        Hu3DMotionTimeSet(MDL(TRI[i].b).modelId, idx[0]);
        Hu3DMotionSpeedSet(MDL(TRI[i].b).modelId, 2.0f);
    }
    for (i = 6, j = 0; i <= 15; i++, j++) {
        MDL(i).modelId = Hu3DModelCreateData(lbl_1_bss_3730->dataIds[8]);
        Hu3DModelAttrSet(MDL(i).modelId, 1);
        Hu3DModelCameraSet(MDL(i).modelId, 1);
        Hu3DModelLayerSet(MDL(i).modelId, 3);
        Hu3DModelScaleSet(MDL(i).modelId, 0.66f, 0.66f, 0.66f);
        Hu3DMotionTimeSet(MDL(i).modelId, 0.0f);
        Hu3DMotionSpeedSet(MDL(i).modelId, 0.0f);
        MDL(i).posX = 0.0f;
        MDL(i).posY = 200.0f;
        MDL(i).posZ = 0.0f;
        MDL(i).rotX = 0.0f;
        MDL(i).rotY = 0.0f;
        MDL(i).rotZ = 0.0f;
        lbl_1_bss_2F0.scaleB[j].x = 0.5f;
        lbl_1_bss_2F0.scaleB[j].y = 1.0f;
        lbl_1_bss_2F0.scaleB[j].z = 1.0f;
    }
    for (i = 16, j = 0; i <= 25; i++, j++) {
        MDL(i).modelId = Hu3DModelCreateData(lbl_1_bss_3730->dataIds[9]);
        Hu3DModelAttrSet(MDL(i).modelId, 1);
        Hu3DModelCameraSet(MDL(i).modelId, 1);
        Hu3DModelLayerSet(MDL(i).modelId, 3);
        Hu3DMotionTimeSet(MDL(i).modelId, 0.0f);
        Hu3DMotionSpeedSet(MDL(i).modelId, 0.0f);
        MDL(i).posX = 0.0f;
        MDL(i).posY = 200.0f;
        MDL(i).posZ = 0.0f;
        MDL(i).rotX = 0.0f;
        MDL(i).rotY = 0.0f;
        MDL(i).rotZ = 0.0f;
        lbl_1_bss_2F0.unk20[j].x = 1.0f;
        lbl_1_bss_2F0.unk20[j].y = 1.0f;
        lbl_1_bss_2F0.unk20[j].z = 1.0f;
    }
    for (i = 26, j = 0; i <= 27; i++, j++) {
        MDL(i).modelId = Hu3DModelCreateData(lbl_1_bss_3730->dataIds[8]);
        Hu3DModelAttrSet(MDL(i).modelId, 1);
        Hu3DModelCameraSet(MDL(i).modelId, 1);
        Hu3DModelLayerSet(MDL(i).modelId, 3);
        Hu3DMotionTimeSet(MDL(i).modelId, 0.0f);
        Hu3DMotionSpeedSet(MDL(i).modelId, 0.0f);
        MDL(i).posX = 0.0f;
        MDL(i).posY = 0.0f;
        MDL(i).posZ = 0.0f;
        MDL(i).rotX = 0.0f;
        MDL(i).rotY = 0.0f;
        MDL(i).rotZ = 0.0f;
        lbl_1_bss_2F0.scaleB[j].x = 1.0f;
        lbl_1_bss_2F0.scaleB[j].y = 1.0f;
        lbl_1_bss_2F0.scaleB[j].z = 1.0f;
    }
    for (i = 28, j = 0; i <= 35; i++, j++) {
        MDL(i).modelId = Hu3DModelCreateData(lbl_1_bss_3730->dataIds[9]);
        Hu3DModelAttrSet(MDL(i).modelId, 1);
        Hu3DModelCameraSet(MDL(i).modelId, 1);
        Hu3DModelLayerSet(MDL(i).modelId, 3);
        Hu3DModelScaleSet(MDL(i).modelId, 0.5f, 1.0f, 0.46f);
        Hu3DModelAttrReset(MDL(i).modelId, 1);
        Hu3DModelAttrSet(MDL(i).modelId, 0x40000001);
        idx[0] = frandmod(90);
        Hu3DMotionTimeSet(MDL(i).modelId, idx[0]);
        Hu3DMotionSpeedSet(MDL(i).modelId, 3.0f);
        MDL(i).posX = lbl_1_data_368[j][0];
        MDL(i).posY = 199.0f;
        MDL(i).posZ = lbl_1_data_368[j][1] - 1.0f;
        MDL(i).rotX = 0.0f;
        MDL(i).rotY = 0.0f;
        MDL(i).rotZ = 0.0f;
    }
    MDL(28).rotX = 0.0f;
    MDL(28).rotY = 45.0f;
    MDL(28).rotZ = 0.0f;
    MDL(29).rotX = 0.0f;
    MDL(29).rotY = 0.0f;
    MDL(29).rotZ = 0.0f;
    MDL(30).rotX = 0.0f;
    MDL(30).rotY = 315.0f;
    MDL(30).rotZ = 0.0f;
    MDL(31).rotX = 0.0f;
    MDL(31).rotY = 270.0f;
    MDL(31).rotZ = 0.0f;
    MDL(32).rotX = 0.0f;
    MDL(32).rotY = 225.0f;
    MDL(32).rotZ = 0.0f;
    MDL(33).rotX = 0.0f;
    MDL(33).rotY = 180.0f;
    MDL(33).rotZ = 0.0f;
    MDL(34).rotX = 0.0f;
    MDL(34).rotY = 135.0f;
    MDL(34).rotZ = 0.0f;
    MDL(35).rotX = 0.0f;
    MDL(35).rotY = 90.0f;
    MDL(35).rotZ = 0.0f;
    lbl_1_bss_0.colMdl[0] = MDL(105).modelId;
    MgActorColMapInit(&lbl_1_bss_0.colMdl[0], 1, 10);
    param.height = 150.0f;
    param.radius = 40.0f;
    param.param = 0;
    param.type = 0;
    param.attr = 0x8000;
    param.narrowHook = fn_1_E900;
    param.correctHookParam = 0;
    param.correctHook = (MGACTOR_CORRECT_HOOK)fn_1_E91C; /* G5 declares it with COLBODY args; asm reads actor->param */
    lbl_1_bss_0.player[0] = MgPlayerCreate(lbl_1_bss_3730->player[0].playerNo, &param, 1, 1, -0x3F, NULL);
    lbl_1_bss_0.player[0]->actor->pos.x = -300.0f;
    lbl_1_bss_0.player[0]->actor->pos.y = 140.0f;
    lbl_1_bss_0.player[0]->actor->pos.z = -300.0f;
    lbl_1_bss_0.player[0]->actor->rotY = 45.0f;
    Hu3DModelShadowSet(lbl_1_bss_0.player[0]->actor->mdlId);
    Hu3DModelShadowDispOn(lbl_1_bss_0.player[0]->actor->mdlId);
    MDL(106).modelId = lbl_1_bss_0.player[0]->actor->mdlId;
    param.param = 1;
    lbl_1_bss_0.player[1] = MgPlayerCreate(lbl_1_bss_3730->player[1].playerNo, &param, 1, 1, -0x3F, NULL);
    lbl_1_bss_0.player[1]->actor->pos.x = 300.0f;
    lbl_1_bss_0.player[1]->actor->pos.y = 140.0f;
    lbl_1_bss_0.player[1]->actor->pos.z = 300.0f;
    lbl_1_bss_0.player[1]->actor->rotY = -135.0f;
    Hu3DModelShadowSet(lbl_1_bss_0.player[1]->actor->mdlId);
    Hu3DModelShadowDispOn(lbl_1_bss_0.player[1]->actor->mdlId);
    MDL(107).modelId = lbl_1_bss_0.player[1]->actor->mdlId;
    Hu3DModelLayerSet(lbl_1_bss_0.player[1]->actor->mdlId, 2);
    Hu3DModelLayerSet(lbl_1_bss_0.player[0]->actor->mdlId, 2);
    CharModelStepFxSet(lbl_1_bss_3730->player[0].charNo, 0);
    CharModelStepFxSet(lbl_1_bss_3730->player[1].charNo, 0);
    CharEffectLayerSet(5);
    fn_1_11710(0);
    fn_1_11710(1);
    lbl_1_bss_2614[0].hook = CharModelItemHookGet(lbl_1_bss_0.player[0]->charNo, 1, 4);
    lbl_1_bss_2614[1].hook = CharModelItemHookGet(lbl_1_bss_0.player[1]->charNo, 1, 4);
    fn_1_1A16C();
    fn_1_2280(0);
    fn_1_2280(1);
    fn_1_758C();
    lbl_1_bss_2734.timer = fn_1_1970(0, 60, 0, 292.0f, 64.0f);
}

s32 fn_1_5C98(M541_STAGE *stage)
{
    HU3DMODEL *model;
    s32 ret = 1;

    fn_1_2AD4(stage);
    MgPlayerPadSet(lbl_1_bss_0.player[0], 0, 0, 0, 0);
    MgPlayerPadSet(lbl_1_bss_0.player[1], 0, 0, 0, 0);
    MgActorExec();
    model = &Hu3DData[MDL(106).modelId];
    lbl_1_bss_2614[0].pos = model->pos;
    model = &Hu3DData[MDL(107).modelId];
    lbl_1_bss_2614[1].pos = model->pos;
    return ret;
}

s32 fn_1_5DA4(void)
{
    s32 ret = 0;

    fn_1_DDCC();
    fn_1_758C();
    return ret;
}

s32 fn_1_5DD8(void)
{
    s32 ret = 0;

    lbl_1_bss_2734.mode = 0;
    lbl_1_bss_2734.unk2 = 0;
    lbl_1_bss_2734.unk4 = 0;
    lbl_1_bss_2734.unk6 = 0;
    fn_1_758C();
    return ret;
}

/* ---- part_g2.c ---- */
#define G2_STAGE lbl_1_bss_3730
#define G2_PI 3.141592653589793

/* per-frame step of the two battle fighters (switch on the flow mode) */
s32 fn_1_5E48(void)
{
    HuVecF pos;
    s16 i;
    s16 allDone;
    s16 timerDone;
    s16 charNo;
    s32 ret;
    HU3DMODEL *model0 = &Hu3DData[G2_STAGE->model[0x6A].modelId];
    HU3DMODEL *model1 = &Hu3DData[G2_STAGE->model[0x6B].modelId];

    ret = 0;
    allDone = 1;
    lbl_1_bss_45C.count++;
    switch (lbl_1_bss_2734.mode) {
        case 0:
            fn_1_19E4(lbl_1_bss_2734.timer);
            lbl_1_bss_2734.mode++;
        case 1:
            timerDone = fn_1_1A10(lbl_1_bss_2734.timer);
            fn_1_11B7C(timerDone);
            if (lbl_1_bss_2734.cpuPlayer == 0 && timerDone == 0) {
                for (i = 0; i < 2; i++) {
                    if (lbl_1_bss_2614[i].waitMot != 0) {
                        fn_1_11BF8(i);
                    }
                }
            }
            allDone = allDone * fn_1_7850(0);
            allDone = allDone * fn_1_7850(1);
            if (lbl_1_bss_2734.cpuPlayer != 0 || timerDone != 0) {
                lbl_1_bss_2614[0].anim2 = 1;
                lbl_1_bss_2614[1].anim2 = 1;
                allDone = 1;
            }
            if (allDone != 0 || (timerDone != 0 && lbl_1_bss_2614[0].mode == 0 && lbl_1_bss_2614[1].mode == 0)) {
                if (lbl_1_bss_2734.cpuPlayer == 0) {
                    fn_1_2644(1, 100);
                }
                lbl_1_bss_2734.mode++;
            }
            break;
        case 2:
            if (lbl_1_bss_2734.cpuPlayer == 0) {
                fn_1_1A38(lbl_1_bss_2734.timer);
            }
            if (lbl_1_bss_2734.cpuPlayer != 0) {
                MgSeqWinnerSet(G2_STAGE->player[lbl_1_bss_2734.cpuPlayer & 1].charNo, -1, -1, -1);
                charNo = G2_STAGE->player[lbl_1_bss_2734.cpuPlayer & 1].playerNo;
                if (_CheckFlag(FLAG_MG_PRACTICE) == 0) {
                    GwPlayer[charNo].mgCoin = 10;
                }
            } else {
                MgSeqWinnerSet(-1, -1, -1, -1);
            }
            ret = 1;
            break;
    }
    for (i = 0; i < 2; i++) {
        if (lbl_1_bss_2614[i].waitMot != 0) {
            MgPlayerPadSet(lbl_1_bss_0.player[i], lbl_1_bss_2614[i].padStkX, lbl_1_bss_2614[i].padStkY,
                lbl_1_bss_2614[i].padBtnDown, lbl_1_bss_2614[i].padBtn);
        }
        if (lbl_1_bss_2614[i].anim2 != 0) {
            MgPlayerPadSet(lbl_1_bss_0.player[i], 0, 0, 0, 0);
        }
    }
    fn_1_A884();
    fn_1_92FC(1);
    MgActorPosGet(lbl_1_bss_0.player[0]->actor, &pos);
    lbl_1_bss_2614[0].velX = pos.x;
    lbl_1_bss_2614[0].velY = pos.y;
    lbl_1_bss_2614[0].velZ = pos.z;
    MgActorPosGet(lbl_1_bss_0.player[1]->actor, &pos);
    lbl_1_bss_2614[1].velX = pos.x;
    lbl_1_bss_2614[1].velY = pos.y;
    lbl_1_bss_2614[1].velZ = pos.z;
    lbl_1_bss_2614[0].stunTime = model0->rot.y;
    lbl_1_bss_2614[1].stunTime = model1->rot.y;
    MgActorExec();
    MgActorPosGet(lbl_1_bss_0.player[0]->actor, &pos);
    lbl_1_bss_2614[0].pos.x = pos.x;
    lbl_1_bss_2614[0].pos.y = pos.y;
    lbl_1_bss_2614[0].pos.z = pos.z;
    MgActorPosGet(lbl_1_bss_0.player[1]->actor, &pos);
    lbl_1_bss_2614[1].pos.x = pos.x;
    lbl_1_bss_2614[1].pos.y = pos.y;
    lbl_1_bss_2614[1].pos.z = pos.z;
    fn_1_E930();
    for (i = 0; i < 2; i++) {
        if ((s32)lbl_1_bss_2614[i].pos.x > 700 || (s32)lbl_1_bss_2614[i].pos.x < -700
            || (s32)lbl_1_bss_2614[i].pos.z > 700 || (s32)lbl_1_bss_2614[i].pos.z < -700) {
            /* out of bounds: restore the saved positions (the asm always restores slot 0 and 1 from the same fixed fields) */
            lbl_1_bss_2614[0].pos.x = lbl_1_bss_2614[0].velX;
            lbl_1_bss_2614[0].pos.y = lbl_1_bss_2614[0].velY;
            lbl_1_bss_2614[0].pos.z = lbl_1_bss_2614[0].velZ;
            lbl_1_bss_2614[1].pos.x = lbl_1_bss_2614[1].velX;
            lbl_1_bss_2614[1].pos.y = lbl_1_bss_2614[1].velY;
            lbl_1_bss_2614[1].pos.z = lbl_1_bss_2614[1].velZ;
            Hu3DModelPosSet(G2_STAGE->model[0x6A].modelId, lbl_1_bss_2614[0].pos.x, lbl_1_bss_2614[0].pos.y, lbl_1_bss_2614[0].pos.z);
            Hu3DModelPosSet(G2_STAGE->model[0x6B].modelId, lbl_1_bss_2614[1].pos.x, lbl_1_bss_2614[1].pos.y, lbl_1_bss_2614[1].pos.z);
            Hu3DModelRotSet(G2_STAGE->model[0x6A].modelId, 0.0f, lbl_1_bss_2614[0].stunTime, 0.0f);
            Hu3DModelRotSet(G2_STAGE->model[0x6B].modelId, 0.0f, lbl_1_bss_2614[1].stunTime, 0.0f);
            lbl_1_bss_0.player[0]->actor->rotY = lbl_1_bss_2614[0].stunTime;
            lbl_1_bss_0.player[1]->actor->rotY = lbl_1_bss_2614[1].stunTime;
            lbl_1_bss_2614[0].chasePos.x = 0.0f;
            lbl_1_bss_2614[0].chasePos.z = 0.0f;
            lbl_1_bss_2614[1].chasePos.x = 0.0f;
            lbl_1_bss_2614[1].chasePos.z = 0.0f;
        }
    }
    pos.x = lbl_1_bss_2614[0].pos.x;
    pos.y = lbl_1_bss_2614[0].pos.y;
    pos.z = lbl_1_bss_2614[0].pos.z;
    MgActorPosUpdate(lbl_1_bss_0.player[0]->actor, &pos);
    pos.x = lbl_1_bss_2614[1].pos.x;
    pos.y = lbl_1_bss_2614[1].pos.y;
    pos.z = lbl_1_bss_2614[1].pos.z;
    MgActorPosUpdate(lbl_1_bss_0.player[1]->actor, &pos);
    if (lbl_1_bss_2734.cpuPlayer == 0) {
        fn_1_6EA8();
    }
    fn_1_758C();
    return ret;
}

/* initial step of the first mode */
s32 fn_1_6728(void)
{
    HuVecF pos;
    s32 i;
    s32 ret;
    HU3DMODEL *model0 = &Hu3DData[G2_STAGE->model[0x6A].modelId];
    HU3DMODEL *model1 = &Hu3DData[G2_STAGE->model[0x6B].modelId];

    ret = 0;
    lbl_1_bss_2734.mode = 0;
    lbl_1_bss_2734.unk2 = 0;
    lbl_1_bss_2734.unk4 = 0;
    lbl_1_bss_2734.unk6 = 0;
    fn_1_7850(0);
    fn_1_7850(1);
    MgPlayerPadSet(lbl_1_bss_0.player[0], 0, 0, 0, 0);
    MgPlayerPadSet(lbl_1_bss_0.player[1], 0, 0, 0, 0);
    MgActorPosGet(lbl_1_bss_0.player[0]->actor, &pos);
    lbl_1_bss_2614[0].velX = pos.x;
    lbl_1_bss_2614[0].velY = pos.y;
    lbl_1_bss_2614[0].velZ = pos.z;
    MgActorPosGet(lbl_1_bss_0.player[1]->actor, &pos);
    lbl_1_bss_2614[1].velX = pos.x;
    lbl_1_bss_2614[1].velY = pos.y;
    lbl_1_bss_2614[1].velZ = pos.z;
    lbl_1_bss_2614[0].stunTime = model0->rot.y;
    lbl_1_bss_2614[1].stunTime = model1->rot.y;
    MgActorExec();
    for (i = 0; i < 2; i++) {
        if ((s32)lbl_1_bss_2614[i].pos.x > 700 || (s32)lbl_1_bss_2614[i].pos.x < -700
            || (s32)lbl_1_bss_2614[i].pos.z > 700 || (s32)lbl_1_bss_2614[i].pos.z < -700) {
            lbl_1_bss_2614[0].pos.x = lbl_1_bss_2614[0].velX;
            lbl_1_bss_2614[0].pos.y = lbl_1_bss_2614[0].velY;
            lbl_1_bss_2614[0].pos.z = lbl_1_bss_2614[0].velZ;
            lbl_1_bss_2614[1].pos.x = lbl_1_bss_2614[1].velX;
            lbl_1_bss_2614[1].pos.y = lbl_1_bss_2614[1].velY;
            lbl_1_bss_2614[1].pos.z = lbl_1_bss_2614[1].velZ;
            Hu3DModelPosSet(G2_STAGE->model[0x6A].modelId, lbl_1_bss_2614[0].pos.x, lbl_1_bss_2614[0].pos.y, lbl_1_bss_2614[0].pos.z);
            Hu3DModelPosSet(G2_STAGE->model[0x6B].modelId, lbl_1_bss_2614[1].pos.x, lbl_1_bss_2614[1].pos.y, lbl_1_bss_2614[1].pos.z);
            Hu3DModelRotSet(G2_STAGE->model[0x6A].modelId, 0.0f, lbl_1_bss_2614[0].stunTime, 0.0f);
            Hu3DModelRotSet(G2_STAGE->model[0x6B].modelId, 0.0f, lbl_1_bss_2614[1].stunTime, 0.0f);
            lbl_1_bss_0.player[0]->actor->rotY = lbl_1_bss_2614[0].stunTime;
            lbl_1_bss_0.player[1]->actor->rotY = lbl_1_bss_2614[1].stunTime;
            lbl_1_bss_2614[0].chasePos.x = 0.0f;
            lbl_1_bss_2614[0].chasePos.z = 0.0f;
        }
    }
    pos.x = lbl_1_bss_2614[0].pos.x;
    pos.y = lbl_1_bss_2614[0].pos.y;
    pos.z = lbl_1_bss_2614[0].pos.z;
    MgActorPosUpdate(lbl_1_bss_0.player[0]->actor, &pos);
    pos.x = lbl_1_bss_2614[1].pos.x;
    pos.y = lbl_1_bss_2614[1].pos.y;
    pos.z = lbl_1_bss_2614[1].pos.z;
    MgActorPosUpdate(lbl_1_bss_0.player[1]->actor, &pos);
    fn_1_758C();
    return ret;
}

s32 fn_1_6BF0(void)
{
    s16 ret = 0;
    fn_1_7850(0);
    fn_1_7850(1);
    ret = fn_1_E0A4();
    fn_1_758C();
    return ret;
}

s32 fn_1_6C38(void)
{
    s32 ret = 0;
    M541_MODELWORK *work;

    if (G2_STAGE->unk178 == 0) {
        if (lbl_1_bss_2734.cpuPlayer != 0) {
            work = &G2_STAGE->work[lbl_1_bss_2734.cpuPlayer & 1];
            work->motNo = 6;
            work->speed = 1.0f;
            work->time = 0.0f;
            work->attrReset = 0;
            work->attr = 0x40000001;
        } else {
            work = &G2_STAGE->work[0];
            work->motNo = 7;
            work->speed = 1.0f;
            work->time = 0.0f;
            work->attrReset = 0;
            work->attr = 0x40000001;
            work = &G2_STAGE->work[1];
            work->motNo = 7;
            work->speed = 1.0f;
            work->time = 0.0f;
            work->attrReset = 0;
            work->attr = 0x40000001;
            CharLoseVoicePlay(G2_STAGE->player[0].charNo, G2_STAGE->player[1].charNo, -1, -1);
        }
        G2_STAGE->unk178 = 1;
    }
    fn_1_758C();
    if (lbl_1_bss_2734.cpuPlayer != 0) {
        CharModelVoicePanSet(G2_STAGE->player[lbl_1_bss_2734.cpuPlayer & 1].charNo, 64);
    }
    return ret;
}

s32 fn_1_6E20(void)
{
    s32 ret = 1;
    fn_1_758C();
    return ret;
}

s32 fn_1_6E50(void)
{
    s32 ret = 1;
    fn_1_2AB4();
    MgActorKill(lbl_1_bss_0.player[0]->actor);
    MgActorKill(lbl_1_bss_0.player[1]->actor);
    return ret;
}

/* drives the falling leaves and the leaf scale/position models */
void fn_1_6EA8(void)
{
    s16 i;

    for (i = 0; i < 15; i++) {
        if (lbl_1_bss_20[i].unk0 != 0) {
            if (lbl_1_bss_2F0.length <= 10.0f) {
                lbl_1_bss_20[i].vel.x = lbl_1_bss_2F0.length * cos(G2_PI * lbl_1_bss_20[i].angle / 180.0);
                lbl_1_bss_20[i].vel.y = 0.0f;
                lbl_1_bss_20[i].vel.z = lbl_1_bss_2F0.length * sin(G2_PI * lbl_1_bss_20[i].angle / 180.0);
            }
            if (lbl_1_bss_20[i].hidden != 0) {
                lbl_1_bss_20[i].vel.x = 0.0f;
                lbl_1_bss_20[i].vel.z = 0.0f;
            }
            G2_STAGE->model[lbl_1_bss_36A4[i].a].posX += lbl_1_bss_20[i].vel.x;
            G2_STAGE->model[lbl_1_bss_36A4[i].a].posZ += lbl_1_bss_20[i].vel.z;
            *(HuVecF *)&G2_STAGE->model[lbl_1_bss_36A4[i].b].posX = *(HuVecF *)&G2_STAGE->model[lbl_1_bss_36A4[i].a].posX;
            *(HuVecF *)&G2_STAGE->model[lbl_1_bss_36A4[i].c].posX = *(HuVecF *)&G2_STAGE->model[lbl_1_bss_36A4[i].a].posX;
            G2_STAGE->model[lbl_1_bss_36A4[i].a].rotY = 90.0f + -lbl_1_bss_20[i].angle;
        }
    }
    lbl_1_bss_2F0.length += 0.002f;
    if (lbl_1_bss_2F0.length > 10.0f) {
        lbl_1_bss_2F0.length = 10.0f;
    }
}

/* copies the model table (pos/rot/scale) into the HU3D models and pans the voices */
void fn_1_72D8(void)
{
    s16 i;
    s16 j;
    float pan;
    HU3DMODEL *model0 = &Hu3DData[G2_STAGE->model[0x6A].modelId];
    HU3DMODEL *model1 = &Hu3DData[G2_STAGE->model[0x6B].modelId];

    for (i = 0; i < 0x6A; i++) {
        Hu3DModelPosSet(G2_STAGE->model[i].modelId, G2_STAGE->model[i].posX, G2_STAGE->model[i].posY, G2_STAGE->model[i].posZ);
        Hu3DModelRotSet(G2_STAGE->model[i].modelId, G2_STAGE->model[i].rotX, G2_STAGE->model[i].rotY, G2_STAGE->model[i].rotZ);
    }
    for (i = 0x10, j = 0; i <= 0x19; i++, j++) {
        Hu3DModelScaleSet(G2_STAGE->model[i].modelId, lbl_1_bss_2F0.unk20[j].x, lbl_1_bss_2F0.unk20[j].y, lbl_1_bss_2F0.unk20[j].z);
    }
    pan = 0.055652175f * model0->pos.x;
    CharModelVoicePanSet(G2_STAGE->player[0].charNo, (s32)(64.0f + pan));
    pan = 0.055652175f * model1->pos.x;
    CharModelVoicePanSet(G2_STAGE->player[1].charNo, (s32)(64.0f + pan));
}

/* same as fn_1_72D8, then runs the per-frame hooks of the other units */
void fn_1_758C(void)
{
    s16 i;
    s16 j;
    float pan;
    HU3DMODEL *model0 = &Hu3DData[G2_STAGE->model[0x6A].modelId];
    HU3DMODEL *model1 = &Hu3DData[G2_STAGE->model[0x6B].modelId];

    for (i = 0; i < 0x6A; i++) {
        Hu3DModelPosSet(G2_STAGE->model[i].modelId, G2_STAGE->model[i].posX, G2_STAGE->model[i].posY, G2_STAGE->model[i].posZ);
        Hu3DModelRotSet(G2_STAGE->model[i].modelId, G2_STAGE->model[i].rotX, G2_STAGE->model[i].rotY, G2_STAGE->model[i].rotZ);
    }
    for (i = 0x10, j = 0; i <= 0x19; i++, j++) {
        Hu3DModelScaleSet(G2_STAGE->model[i].modelId, lbl_1_bss_2F0.unk20[j].x, lbl_1_bss_2F0.unk20[j].y, lbl_1_bss_2F0.unk20[j].z);
    }
    pan = 0.055652175f * model0->pos.x;
    CharModelVoicePanSet(G2_STAGE->player[0].charNo, (s32)(64.0f + pan));
    pan = 0.055652175f * model1->pos.x;
    CharModelVoicePanSet(G2_STAGE->player[1].charNo, (s32)(64.0f + pan));
    fn_1_91C(0);
    fn_1_10C4();
    fn_1_14C4();
}

/* fighter state machine for player idx; returns 1 once it reached the final state */
s32 fn_1_7850(s16 idx)
{
    M541_MODELWORK *work = &G2_STAGE->work[idx];
    s32 ret = 0;

    switch (lbl_1_bss_2614[idx].mode) {
        case 0:
            if (lbl_1_bss_2614[idx].stunMode != 0 && lbl_1_bss_2734.cpuPlayer == 0) {
                if (lbl_1_bss_2614[idx].stunMode == 2) {
                    fn_1_1A38(lbl_1_bss_2734.timer);
                    lbl_1_bss_2734.cpuPlayer = (idx ^ 1) | (idx * 2);
                }
                lbl_1_bss_2614[idx].anim2 = 1;
                lbl_1_bss_2614[idx].mode++;
            } else if (lbl_1_bss_2734.cpuPlayer != 0) {
                lbl_1_bss_2614[idx].mode = 6;
            }
            break;
        case 1:
            if (lbl_1_bss_0.player[idx]->mode == 0) {
                lbl_1_bss_2614[idx].mode++;
            }
            break;
        case 2:
            MgPlayerPauseOn(lbl_1_bss_0.player[idx]);
            lbl_1_bss_2614[idx].mode++;
            break;
        case 3:
            if (lbl_1_bss_2614[idx].stunMode == 1) {
                if (fn_1_7DE8(idx) != 0) {
                    lbl_1_bss_2614[idx].mode++;
                }
            } else {
                if (lbl_1_bss_2614[idx].sub == 1) {
                    lbl_1_bss_2614[idx].sub = 0;
                    lbl_1_bss_2614[idx].motion = 1;
                    if (lbl_1_bss_2734.cpuPlayer == 0) {
                        omVibrate(G2_STAGE->player[idx].playerNo, 30, 12, 0);
                    }
                } else if (lbl_1_bss_2614[idx].sub == 2) {
                    work->time = 12.0f;
                    lbl_1_bss_2614[idx].sub = 0;
                    lbl_1_bss_2614[idx].motion = 2;
                    if (lbl_1_bss_2734.cpuPlayer == 0) {
                        omVibrate(G2_STAGE->player[idx].playerNo, 30, 12, 0);
                    }
                } else if (lbl_1_bss_2614[idx].sub == 3) {
                    work->motNo = -1;
                    lbl_1_bss_2614[idx].sub = 0;
                    lbl_1_bss_2614[idx].motion = 0;
                }
                if (fn_1_832C(idx) != 0) {
                    lbl_1_bss_2614[idx].mode++;
                }
                if (lbl_1_bss_2614[idx].stunMode == 2) {
                    lbl_1_bss_2734.cpuPlayer = (idx ^ 1) | (idx * 2);
                }
            }
            break;
        case 4:
            MgPlayerPauseOff(lbl_1_bss_0.player[idx], &lbl_1_bss_2614[idx].pos);
            lbl_1_bss_2614[idx].mode++;
            break;
        case 5:
            if (lbl_1_bss_2734.cpuPlayer != 0) {
                lbl_1_bss_2614[idx].mode++;
            } else {
                lbl_1_bss_2614[idx].anim2 = 0;
                lbl_1_bss_2614[idx].stunMode = 0;
                lbl_1_bss_2614[idx].mode = 0;
            }
            break;
        case 6:
            ret = 1;
            break;
    }
    return ret;
}

/* ---- part_g3.c ---- */
s32 fn_1_7DE8(s16 arg0)
{
    M541_MODELWORK *work = &lbl_1_bss_3730->work[arg0];
    float offZ = -30.0f;
    s32 ret = 0;
    HuVecF pos;

    switch (lbl_1_bss_2614[arg0].sub) {
        case 0:
            if (lbl_1_bss_2734.cpuPlayer == 0) {
                omVibrate(lbl_1_bss_3730->player[arg0].playerNo, 30, 6, 6);
            }
            work->motNo = 5;
            work->speed = 1.0f;
            work->time = 0.0f;
            work->attrReset = 0;
            work->attr = 0x40000001;
            lbl_1_bss_2614[arg0].sub++;
            break;
        case 1:
            if (work->time > 10.0f) {
                Hu3DModelAttrReset(lbl_1_bss_3730->model[arg0 + 0x1A].modelId, 1);
                Hu3DModelAttrSet(lbl_1_bss_3730->model[arg0 + 0x1A].modelId, 0x40000001);
                Hu3DMotionTimeSet(lbl_1_bss_3730->model[arg0 + 0x1A].modelId, 0.0f);
                Hu3DMotionSpeedSet(lbl_1_bss_3730->model[arg0 + 0x1A].modelId, 4.0f);
                Hu3DModelObjPosGet(lbl_1_bss_3730->model[arg0 + 0x6A].modelId, lbl_1_bss_2614[arg0].hook, &pos);
                lbl_1_bss_3730->model[arg0 + 0x1A].posX = pos.x;
                lbl_1_bss_3730->model[arg0 + 0x1A].posY = pos.y;
                lbl_1_bss_3730->model[arg0 + 0x1A].posZ = pos.z + offZ;
                lbl_1_bss_2614[arg0].sub++;
            }
            break;
        case 2:
            Hu3DModelObjPosGet(lbl_1_bss_3730->model[arg0 + 0x6A].modelId, lbl_1_bss_2614[arg0].hook, &pos);
            lbl_1_bss_3730->model[arg0 + 0x1A].posX = pos.x;
            lbl_1_bss_3730->model[arg0 + 0x1A].posY = pos.y;
            lbl_1_bss_3730->model[arg0 + 0x1A].posZ = pos.z + offZ;
            if (work->time >= 80.0f) {
                Hu3DModelAttrReset(lbl_1_bss_3730->model[arg0 + 0x1A].modelId, 0x40000001);
                Hu3DModelAttrSet(lbl_1_bss_3730->model[arg0 + 0x1A].modelId, 1);
                Hu3DMotionTimeSet(lbl_1_bss_3730->model[arg0 + 0x1A].modelId, 0.0f);
                Hu3DMotionSpeedSet(lbl_1_bss_3730->model[arg0 + 0x1A].modelId, 0.0f);
                lbl_1_bss_2614[arg0].sub++;
            }
            break;
        case 3:
            if (work->endCheck != 0) {
                lbl_1_bss_2614[arg0].sub++;
            }
            break;
        case 4:
            work->motNo = 0;
            work->speed = 1.0f;
            work->time = 0.0f;
            work->attrReset = 0x40000001;
            work->attr = 0;
            lbl_1_bss_2614[arg0].sub = 0;
            ret = 1;
            break;
    }
    return ret;
}

s32 fn_1_832C(s16 arg0)
{
    HU3DMODEL *mdl = &Hu3DData[lbl_1_bss_3730->model[arg0 + 0x6A].modelId];
    M541_MODELWORK *work = &lbl_1_bss_3730->work[arg0];
    float offZ = -30.0f;
    s32 ret = 0;
    s16 tmp = 0;
    HuVecF pos;
    HuVecF vec;

    switch (lbl_1_bss_2614[arg0].motion) {
        case 0: {
            s32 y, x;
            float fy, fx;
            omVibrate(lbl_1_bss_3730->player[arg0].playerNo, 30, 12, 0);
            fy = 0.026956521f * mdl->pos.z;
            fx = 0.055652175f * mdl->pos.x;
            y = 111.0f + fy;
            x = 64.0f + fx;
            fn_1_2644(1, 100);
            fn_1_27C8(3, y, x);
            work->motNo = 5;
            work->speed = 1.0f;
            work->time = 0.0f;
            work->attrReset = 0;
            work->attr = 0x40000001;
            lbl_1_bss_2614[arg0].motion++;
            break;
        }
        case 1:
            if (work->time > 10.0f) {
                Hu3DModelAttrReset(lbl_1_bss_3730->model[arg0 + 0x1A].modelId, 1);
                Hu3DModelAttrSet(lbl_1_bss_3730->model[arg0 + 0x1A].modelId, 0x40000001);
                Hu3DMotionTimeSet(lbl_1_bss_3730->model[arg0 + 0x1A].modelId, 0.0f);
                Hu3DMotionSpeedSet(lbl_1_bss_3730->model[arg0 + 0x1A].modelId, 4.0f);
                Hu3DModelObjPosGet(lbl_1_bss_3730->model[arg0 + 0x6A].modelId, lbl_1_bss_2614[arg0].hook, &pos);
                lbl_1_bss_3730->model[arg0 + 0x1A].posX = pos.x;
                lbl_1_bss_3730->model[arg0 + 0x1A].posY = pos.y;
                lbl_1_bss_3730->model[arg0 + 0x1A].posZ = pos.z + offZ;
                lbl_1_bss_2614[arg0].tick = 0;
                lbl_1_bss_2614[arg0].anim = 0;
                lbl_1_bss_2614[arg0].motion++;
            }
            break;
        case 2:
            Hu3DModelObjPosGet(lbl_1_bss_3730->model[arg0 + 0x6A].modelId, lbl_1_bss_2614[arg0].hook, &pos);
            lbl_1_bss_3730->model[arg0 + 0x1A].posX = pos.x;
            lbl_1_bss_3730->model[arg0 + 0x1A].posY = pos.y;
            lbl_1_bss_3730->model[arg0 + 0x1A].posZ = pos.z + offZ;
            if (lbl_1_bss_2614[arg0].tick++ == 0) {
                fn_1_2378(arg0, 0.25f);
            } else if (lbl_1_bss_2614[arg0].tick > 3) {
                if (lbl_1_bss_2614[arg0].tick <= 6) {
                    fn_1_24E8(arg0);
                } else {
                    lbl_1_bss_2614[arg0].anim = 0;
                    lbl_1_bss_2614[arg0].tick = 0;
                }
            }
            if (work->time >= 86.0f) {
                fn_1_24E8(arg0);
                fn_1_2378(arg0, 0.25f);
                work->shiftMode = 1;
                work->shiftMotNo = 2;
                work->shiftSpeed = 0.0f;
                work->shiftTime = 10.0f;
                work->shiftAttr = 0;
                work->attr = 0x40000001;
                Hu3DModelAttrReset(lbl_1_bss_3730->model[arg0 + 0x1A].modelId, 0x40000001);
                Hu3DModelAttrSet(lbl_1_bss_3730->model[arg0 + 0x1A].modelId, 1);
                Hu3DMotionTimeSet(lbl_1_bss_3730->model[arg0 + 0x1A].modelId, 0.0f);
                Hu3DMotionSpeedSet(lbl_1_bss_3730->model[arg0 + 0x1A].modelId, 0.0f);
                lbl_1_bss_2614[arg0].motion++;
            }
            break;
        case 3:
            if (work->shiftMode == 3) {
                tmp = fn_1_B4E0(arg0);
                if (tmp == 0) {
                    work->shiftMode = 1;
                    work->shiftMotNo = 3;
                    work->shiftSpeed = 0.0f;
                    work->shiftTime = 8.0f;
                    work->shiftAttr = 0x40000001;
                    work->attr = 0;
                    lbl_1_bss_2614[arg0].motion = 9;
                } else {
                    vec.x = lbl_1_bss_2734.moveX - lbl_1_bss_2614[arg0].pos.x;
                    vec.z = lbl_1_bss_2734.moveZ - lbl_1_bss_2614[arg0].pos.z;
                    lbl_1_bss_2734.velX = vec.x / 20.0f;
                    lbl_1_bss_2734.velY = 10.0f;
                    lbl_1_bss_2734.velZ = vec.z / 20.0f;
                    lbl_1_bss_2734.angAcc = 1.0f;
                    vec.x = lbl_1_bss_2734.goal.x - lbl_1_bss_2734.moveX;
                    vec.z = lbl_1_bss_2734.goal.z - lbl_1_bss_2734.moveZ;
                    lbl_1_bss_2734.angVelX = vec.x * 0.0625f;
                    lbl_1_bss_2734.angVelY = 8.0f;
                    lbl_1_bss_2734.angVelZ = vec.z * 0.0625f;
                    lbl_1_bss_2734.angCount = 0;
                    lbl_1_bss_2614[arg0].motion++;
                }
            }
            break;
        case 4:
            lbl_1_bss_2614[arg0].pos.x += lbl_1_bss_2734.velX;
            lbl_1_bss_2614[arg0].pos.y += lbl_1_bss_2734.velY;
            lbl_1_bss_2614[arg0].pos.z += lbl_1_bss_2734.velZ;
            lbl_1_bss_2734.velY -= lbl_1_bss_2734.angAcc;
            mdl->pos = lbl_1_bss_2614[arg0].pos;
            MgActorPosUpdate(lbl_1_bss_0.player[arg0 + 1]->actor, &lbl_1_bss_2614[arg0].pos);
            if (++lbl_1_bss_2734.angCount >= 10) {
                lbl_1_bss_2734.velY = 0.0f;
                lbl_1_bss_2734.angCount = 0;
                lbl_1_bss_2614[arg0].motion++;
            }
            break;
        case 5:
            lbl_1_bss_2614[arg0].pos.x += lbl_1_bss_2734.velX;
            lbl_1_bss_2614[arg0].pos.y += lbl_1_bss_2734.velY;
            lbl_1_bss_2614[arg0].pos.z += lbl_1_bss_2734.velZ;
            lbl_1_bss_2734.velY -= lbl_1_bss_2734.angAcc;
            mdl->pos = lbl_1_bss_2614[arg0].pos;
            MgActorPosUpdate(lbl_1_bss_0.player[arg0 + 1]->actor, &lbl_1_bss_2614[arg0].pos);
            if (++lbl_1_bss_2734.angCount >= 10) {
                work->shiftMode = 1;
                work->shiftMotNo = 2;
                work->shiftSpeed = 0.0f;
                work->shiftTime = 10.0f;
                work->shiftAttr = 0;
                work->attr = 0x40000001;
                lbl_1_bss_2734.angCount = 0;
                lbl_1_bss_2614[arg0].motion++;
            }
            break;
        case 6:
            lbl_1_bss_2614[arg0].pos.x += lbl_1_bss_2734.angVelX;
            lbl_1_bss_2614[arg0].pos.y += lbl_1_bss_2734.angVelY;
            lbl_1_bss_2614[arg0].pos.z += lbl_1_bss_2734.angVelZ;
            lbl_1_bss_2734.angVelY -= lbl_1_bss_2734.angAcc;
            mdl->pos = lbl_1_bss_2614[arg0].pos;
            MgActorPosUpdate(lbl_1_bss_0.player[arg0 + 1]->actor, &lbl_1_bss_2614[arg0].pos);
            if (++lbl_1_bss_2734.angCount >= 8) {
                lbl_1_bss_2734.angVelY = 0.0f;
                lbl_1_bss_2734.angCount = 0;
                lbl_1_bss_2614[arg0].motion++;
            }
            break;
        case 7:
            lbl_1_bss_2614[arg0].pos.x += lbl_1_bss_2734.angVelX;
            lbl_1_bss_2614[arg0].pos.y += lbl_1_bss_2734.angVelY;
            lbl_1_bss_2614[arg0].pos.z += lbl_1_bss_2734.angVelZ;
            lbl_1_bss_2734.angVelY -= lbl_1_bss_2734.angAcc;
            mdl->pos = lbl_1_bss_2614[arg0].pos;
            MgActorPosUpdate(lbl_1_bss_0.player[arg0 + 1]->actor, &lbl_1_bss_2614[arg0].pos);
            if (++lbl_1_bss_2734.angCount >= 8) {
                work->shiftMode = 1;
                work->shiftMotNo = 2;
                work->shiftSpeed = 0.0f;
                work->shiftTime = 10.0f;
                work->shiftAttr = 0;
                work->attr = 0x40000001;
                lbl_1_bss_2614[arg0].pos.y = 100.0f;
                mdl->pos.y = 100.0f;
                lbl_1_bss_2734.angCount = 0;
                lbl_1_bss_2614[arg0].motion++;
            }
            break;
        case 8:
            if (work->shiftMode == 3) {
                work->shiftMode = 1;
                work->shiftMotNo = 3;
                work->shiftSpeed = 0.0f;
                work->shiftTime = 8.0f;
                work->shiftAttr = 0x40000001;
                work->attr = 0;
                lbl_1_bss_2614[arg0].motion++;
            }
            break;
        case 9:
            lbl_1_bss_2614[arg0].motion = 0;
            ret = 1;
            break;
    }
    return ret;
}

s32 fn_1_92FC(s16 arg0)
{
    s32 ret = 0;
    float range[11][2] = {
        { 0.0f, 0.0f },     { 400.0f, 650.0f }, { 350.0f, 600.0f }, { 300.0f, 550.0f },
        { 250.0f, 500.0f }, { 200.0f, 450.0f }, { 150.0f, 400.0f }, { 100.0f, 350.0f },
        { 50.0f, 300.0f },  { 0.0f, 250.0f },   { 0.0f, 200.0f },
    };
    s16 found;
    s16 i, j, k;
    s16 rnd;
    float dx, dz, d;

    switch (lbl_1_bss_2F0.mode) {
        case 0:
            if (arg0 != 0) {
                lbl_1_bss_2F0.timer = lbl_1_bss_45C.aiTimer[lbl_1_bss_2F0.count];
            } else {
                lbl_1_bss_2F0.timer = 120;
            }
            lbl_1_bss_2F0.count++;
            lbl_1_bss_2F0.mode++;
            break;
        case 1: {
            s16 old = lbl_1_bss_2F0.timer;
            lbl_1_bss_2F0.timer--;
            if (old < 0) {
                lbl_1_bss_2F0.timer = 0;
                lbl_1_bss_2F0.mode++;
            }
            if (arg0 != 0) {
                lbl_1_bss_45C.cnt217E++;
            }
            break;
        }
        case 2:
            if (arg0 != 0) {
                lbl_1_bss_45C.cnt217C++;
            }
            for (i = 0; i < 10; i++) {
                lbl_1_bss_2F0.nextLeaf[i] = -1;
            }
            if (arg0 != 0) {
                rnd = lbl_1_bss_45C.aiMode[lbl_1_bss_2F0.count];
            } else {
                rnd = frandmod(8);
            }
            lbl_1_bss_2F0.nextLeaf[0] = rnd + 15;
            for (i = 1; i <= lbl_1_bss_2F0.count; i++) {
                for (j = 0; j < 15; j++) {
                    found = 0;
                    for (k = 0; k < 10; k++) {
                        if (lbl_1_bss_2F0.nextLeaf[k] == j) {
                            found = 1;
                        }
                    }
                    if (found == 0 && lbl_1_bss_20[j].unk0 != 0) {
                        dx = lbl_1_bss_3730->model[lbl_1_bss_36A4[j].a].posX
                            - lbl_1_bss_3730->model[lbl_1_bss_36A4[lbl_1_bss_2F0.nextLeaf[i - 1]].a].posX;
                        dz = lbl_1_bss_3730->model[lbl_1_bss_36A4[j].a].posZ
                            - lbl_1_bss_3730->model[lbl_1_bss_36A4[lbl_1_bss_2F0.nextLeaf[i - 1]].a].posZ;
                        d = sqrt(dx * dx + dz * dz);
                        if (range[lbl_1_bss_2F0.count][0] < d && range[lbl_1_bss_2F0.count][1] > d) {
                            lbl_1_bss_2F0.nextLeaf[i] = j;
                            lbl_1_bss_2F0.dist[i - 1] = d;
                            dx = 180.0 * (acos(dx / (0.1f + d)) / 3.141592653589793);
                            dz = 180.0 * (asin(dz / (0.1f + d)) / 3.141592653589793);
                            if (dz < 0.0f) {
                                dx = 360.0f - dx;
                            }
                            lbl_1_bss_2F0.angle[i - 1] = dx;
                            break;
                        }
                    }
                }
                if (lbl_1_bss_2F0.nextLeaf[i] == -1) {
                    found = 1;
                    break;
                }
            }
            if (found == 0) {
                for (j = 15; j < 23; j++) {
                    if (arg0 == 0) {
                        j = (lbl_1_bss_2F0.nextLeaf[0] - 11) % 8 + 15;
                    }
                    if (lbl_1_bss_2F0.nextLeaf[0] != j) {
                        dx = lbl_1_bss_3730->model[lbl_1_bss_36A4[j].a].posX
                            - lbl_1_bss_3730->model[lbl_1_bss_36A4[lbl_1_bss_2F0.nextLeaf[lbl_1_bss_2F0.count]].a].posX;
                        dz = lbl_1_bss_3730->model[lbl_1_bss_36A4[j].a].posZ
                            - lbl_1_bss_3730->model[lbl_1_bss_36A4[lbl_1_bss_2F0.nextLeaf[lbl_1_bss_2F0.count]].a].posZ;
                        d = sqrt(dx * dx + dz * dz);
                        if (range[lbl_1_bss_2F0.count][0] < d && range[lbl_1_bss_2F0.count][1] > d) {
                            lbl_1_bss_2F0.nextLeaf[lbl_1_bss_2F0.count + 1] = j;
                            lbl_1_bss_2F0.dist[lbl_1_bss_2F0.count] = d;
                            dx = 180.0 * (acos(dx / (0.1f + d)) / 3.141592653589793);
                            dz = 180.0 * (asin(dz / (0.1f + d)) / 3.141592653589793);
                            if (dz < 0.0f) {
                                dx = 360.0f - dx;
                            }
                            lbl_1_bss_2F0.angle[lbl_1_bss_2F0.count] = dx;
                            break;
                        }
                    }
                }
                for (i = 0; i <= lbl_1_bss_2F0.count + 1; i++) {
                    if (lbl_1_bss_2F0.nextLeaf[i] == -1) {
                        i = 99;
                        for (j = 0; j < 10; j++) {
                            lbl_1_bss_2F0.nextLeaf[j] = -1;
                        }
                    }
                }
                if (lbl_1_bss_2F0.nextLeaf[0] != -1) {
                    lbl_1_bss_2F0.mode++;
                }
            }
            break;
        case 3:
            for (i = 0; i < lbl_1_bss_2F0.count + 2; i++) {
                *(HuVecF *)&lbl_1_bss_3730->model[i + 6].posX = *(HuVecF *)&(&lbl_1_bss_3730->model[lbl_1_bss_36A4[lbl_1_bss_2F0.nextLeaf[i]].a])->posX;
                lbl_1_bss_3730->model[i + 6].posZ -= 1.0f;
                Hu3DModelAttrReset(lbl_1_bss_3730->model[i + 6].modelId, 1);
                Hu3DModelAttrSet(lbl_1_bss_3730->model[i + 6].modelId, 0x40000001);
                Hu3DMotionTimeSet(lbl_1_bss_3730->model[i + 6].modelId, 0.0f);
                Hu3DMotionSpeedSet(lbl_1_bss_3730->model[i + 6].modelId, 4.0f);
            }
            for (i = 1; i < lbl_1_bss_2F0.count + 1; i++) {
                lbl_1_bss_20[lbl_1_bss_2F0.nextLeaf[i]].hidden = 1;
                lbl_1_bss_20[lbl_1_bss_2F0.nextLeaf[i]].velSave = lbl_1_bss_20[lbl_1_bss_2F0.nextLeaf[i]].vel;
                lbl_1_bss_20[lbl_1_bss_2F0.nextLeaf[i]].vel.x = 0.0f;
                lbl_1_bss_20[lbl_1_bss_2F0.nextLeaf[i]].vel.y = 0.0f;
                lbl_1_bss_20[lbl_1_bss_2F0.nextLeaf[i]].vel.z = 0.0f;
            }
            if (arg0 != 0) {
                lbl_1_bss_2F0.timer = 60;
            } else {
                lbl_1_bss_2F0.timer = 120;
            }
            if (lbl_1_bss_2734.cpuPlayer == 0) {
                omVibrate(lbl_1_bss_3730->player[0].playerNo, lbl_1_bss_2F0.timer / 2, 6, 6);
                omVibrate(lbl_1_bss_3730->player[1].playerNo, lbl_1_bss_2F0.timer / 2, 6, 6);
            }
            fn_1_270C(0);
            lbl_1_bss_2F0.mode++;
            break;
        case 4: {
            s16 old = lbl_1_bss_2F0.timer;
            lbl_1_bss_2F0.timer--;
            if (old < 0) {
                for (i = 0; i < lbl_1_bss_2F0.count + 1; i++) {
                *(HuVecF *)&lbl_1_bss_3730->model[i + 0x10].posX = *(HuVecF *)&(&lbl_1_bss_3730->model[lbl_1_bss_36A4[lbl_1_bss_2F0.nextLeaf[i]].a])->posX;
                    lbl_1_bss_3730->model[i + 0x10].posY -= 1.0f;
                    lbl_1_bss_3730->model[i + 0x10].posZ -= 1.0f;
                    Hu3DModelAttrReset(lbl_1_bss_3730->model[i + 0x10].modelId, 1);
                    Hu3DModelAttrSet(lbl_1_bss_3730->model[i + 0x10].modelId, 0x40000001);
                    Hu3DMotionTimeSet(lbl_1_bss_3730->model[i + 0x10].modelId, 0.0f);
                    Hu3DMotionSpeedSet(lbl_1_bss_3730->model[i + 0x10].modelId, 3.0f);
                    lbl_1_bss_3730->model[i + 0x10].rotY = 90.0f + -lbl_1_bss_2F0.angle[i];
                    lbl_1_bss_2F0.unk20[i].x = 0.7f;
                    lbl_1_bss_2F0.unk20[i].z = lbl_1_bss_2F0.dist[i] / 1000.0f;
                }
                lbl_1_bss_2F0.ready = 1;
                if (arg0 != 0) {
                    lbl_1_bss_2F0.timer = 120;
                } else {
                    lbl_1_bss_2F0.timer = 120;
                }
                if (lbl_1_bss_2734.cpuPlayer == 0) {
                    omVibrate(lbl_1_bss_3730->player[0].playerNo, lbl_1_bss_2F0.timer / 2, 12, 0);
                    omVibrate(lbl_1_bss_3730->player[1].playerNo, lbl_1_bss_2F0.timer / 2, 12, 0);
                }
                fn_1_29EC(0);
                fn_1_270C(2);
                lbl_1_bss_2F0.mode++;
            }
            break;
        }
        case 5: {
            s16 old = lbl_1_bss_2F0.timer;
            lbl_1_bss_2F0.timer--;
            if (old < 0) {
                for (i = 6; i <= 0x19; i++) {
                    Hu3DModelAttrSet(lbl_1_bss_3730->model[i].modelId, 1);
                    Hu3DMotionTimeSet(lbl_1_bss_3730->model[i].modelId, 0.0f);
                    Hu3DMotionSpeedSet(lbl_1_bss_3730->model[i].modelId, 0.0f);
                }
                for (i = 1; i < lbl_1_bss_2F0.count + 1; i++) {
                    lbl_1_bss_20[lbl_1_bss_2F0.nextLeaf[i]].hidden = 0;
                    lbl_1_bss_20[lbl_1_bss_2F0.nextLeaf[i]].vel = lbl_1_bss_20[lbl_1_bss_2F0.nextLeaf[i]].velSave;
                }
                fn_1_29EC(2);
                lbl_1_bss_2F0.ready = 0;
                lbl_1_bss_2F0.mode = 0;
                ret = 1;
            }
            break;
        }
    }
    return ret;
}

/* ---- part_g4.c ---- */
#define G4_PI 3.141592653589793

/* leaf spawn timer: every 300 frames (after 600) one more leaf is placed */
void fn_1_A884(void)
{
    s16 done = 0;

    switch (lbl_1_bss_2F0.spawnMode) {
        case 0:
            if (lbl_1_bss_2F0.spawnTimer >= 600 && (lbl_1_bss_2F0.spawnTimer % 300) == 0 && lbl_1_bss_2F0.spawnCount < 15) {
                lbl_1_bss_2F0.spawnMode++;
            }
            break;
        case 1:
            done = fn_1_A9B8(lbl_1_bss_2F0.spawnCount, 1, 0.0f, 0.0f);
            if (done != 0) {
                lbl_1_bss_2F0.spawnCount++;
                lbl_1_bss_2F0.spawnMode = 0;
            }
            break;
    }
    lbl_1_bss_2F0.spawnTimer++;
}

/* places leaf idx (state 0: pick a position, state 1: fade it in); returns 1 when finished */
s16 fn_1_A9B8(s16 idx, s16 mode, float x, float z)
{
    M541_MODEL *m0 = &lbl_1_bss_3730->model[lbl_1_bss_36A4[idx].a];
    M541_MODEL *m1 = &lbl_1_bss_3730->model[lbl_1_bss_36A4[idx].b];
    M541_MODEL *m2 = &lbl_1_bss_3730->model[lbl_1_bss_36A4[idx].c];
    s16 ret = 0;
    s16 done = 0;
    s16 tries = 0;
    s16 i;
    float radius;
    float ang;
    HuVecF d;
    float dist;
    float rnd;

    switch (lbl_1_bss_20[idx].state) {
        case 0:
            if (mode == 0) {
                    ang = 10.0f * frandmod(0x24);
                    radius = 10.0f * (1.0f + frandmod(8));
                    ang = frandmod(0x168);
                    lbl_1_bss_20[idx].angle = ang;
                    m0->posX = x + radius * cos(G4_PI * ang / 180.0);
                    m0->posY = 0.0f;
                    m0->posZ = z + radius * sin(G4_PI * ang / 180.0);
                } else if (mode == 1) {
                    ang = lbl_1_bss_45C.aiPos[idx].x;
                    radius = lbl_1_bss_45C.aiPos[idx].y;
                    lbl_1_bss_20[idx].angle = lbl_1_bss_45C.aiPos[idx].z;
                    m0->posX = x + radius * cos(G4_PI * ang / 180.0);
                    m0->posY = 0.0f;
                    m0->posZ = z + radius * sin(G4_PI * ang / 180.0);
                } else if (mode == 2) {
                    ang = frandmod(0x168);
                    lbl_1_bss_20[idx].angle = ang;
                    do {
                        ang = frandmod(0x168);
                        radius = 61.0f + frandmod(0x78);
                        m0->posX = radius * cos(G4_PI * ang / 180.0);
                        m0->posY = 0.0f;
                        m0->posZ = radius * sin(G4_PI * ang / 180.0);
                        tries++;
                        done = 0;
                        for (i = 0; i < 15; i++) {
                            if (lbl_1_bss_20[i].exist != 0) {
                                d.x = m0->posX - lbl_1_bss_3730->model[lbl_1_bss_36A4[i].a].posX;
                                d.z = m0->posZ - lbl_1_bss_3730->model[lbl_1_bss_36A4[i].a].posZ;
                                dist = sqrt(d.x * d.x + d.z * d.z);
                                if (dist < 61.0f && tries < 10000) {
                                    done = 1;
                                    break;
                                }
                            }
                        }
                    } while (done != 0);
                }
            m1->pos = m0->pos;
            m2->pos = m0->pos;
            Hu3DModelAttrReset(m0->modelId, 1);
            Hu3DModelAttrReset(m1->modelId, 1);
            Hu3DModelAttrSet(m1->modelId, 0x40000001);
            rnd = frandmod(0x5A);
            Hu3DMotionTimeSet(m1->modelId, rnd);
            Hu3DMotionSpeedSet(m1->modelId, 2.0f);
            lbl_1_bss_3730->model[lbl_1_bss_36A4[idx].a].rotY = 90.0f + -lbl_1_bss_20[idx].angle;
            Hu3DModelTPLvlSet(m0->modelId, lbl_1_bss_20[idx].tpLvl);
            Hu3DModelTPLvlSet(m2->modelId, lbl_1_bss_20[idx].tpLvl);
            Hu3DModelShadowDispOn(m2->modelId);
            lbl_1_bss_20[idx].exist = 1;
            lbl_1_bss_20[idx].state++;
            break;
        case 1:
            done += fn_1_1850(&m0->posY, 90.0f, 0.0f, 200.0f);
            done += fn_1_1850(&m1->posY, 90.0f, 0.0f, 200.0f);
            done += fn_1_1850(&m2->posY, 90.0f, 0.0f, 200.0f);
            lbl_1_bss_20[idx].tpLvl += 0.012f;
            if (lbl_1_bss_20[idx].tpLvl >= 1.0f) {
                lbl_1_bss_20[idx].tpLvl = 1.0f;
            } else {
                done = 1;
            }
            Hu3DModelTPLvlSet(m0->modelId, lbl_1_bss_20[idx].tpLvl);
            Hu3DModelTPLvlSet(m2->modelId, lbl_1_bss_20[idx].tpLvl);
            if (done == 0) {
                lbl_1_bss_20[idx].vel.x = lbl_1_bss_2F0.length * cos(G4_PI * lbl_1_bss_20[idx].angle / 180.0);
                lbl_1_bss_20[idx].vel.y = 0.0f;
                lbl_1_bss_20[idx].vel.z = lbl_1_bss_2F0.length * sin(G4_PI * lbl_1_bss_20[idx].angle / 180.0);
                lbl_1_bss_20[idx].unk0 = 1;
                lbl_1_bss_20[idx].state = 1;
                ret = 1;
            }
            break;
    }
    return ret;
}

/*
 * AI helper: looks for a point on a circle around fighter idx (angles around the
 * direction to the other fighter) that is free of walls and leaves, and stores the
 * move target in lbl_1_bss_2734.  Returns 1 when a target was found.
 */
#define G4_INSIDE(x, z) \
    (-(x) + (z) > -784.0f && (x) < 554.0f && (x) + (z) < 784.0f && (z) < 554.0f \
     && -(x) + (z) < 784.0f && (x) > -554.0f && (x) + (z) > -784.0f && (z) > -554.0f)

s16 fn_1_B4E0(s16 idx)
{
    HU3DMODEL *other = &Hu3DData[lbl_1_bss_3730->model[(idx ^ 1) + 0x6A].modelId];
    HU3DMODEL *me = &Hu3DData[lbl_1_bss_3730->model[idx + 0x6A].modelId];
    s32 ret = 0;
    float minX, maxX, minZ, maxZ;
    float dx, dz, mag, baseAng;
    float ang, ang360, lineA, lineB, lineC;
    float ex, ez, hx, hz, dd;
    float ix, iz, t1, t2;
    float px, pz;
    float sp3C, sp38, flagX, flagZ, xLo, xHi, zLo, zHi, cosA, lenProd, tmp;
    float sp4C, sp48, sp44, sp40;
    float found, spC;
    float cx, cz;
    float dist[20];
    float leafMin[20];
    s16 order[20];
    s16 order2[20];
    HuVecF hit[20];
    s16 i, j, n;
    s16 tmpS;

    minX = other->pos.x - 150.0f;
    maxX = 150.0f + other->pos.x;
    minZ = other->pos.z - 110.0f;
    maxZ = 300.0f + other->pos.z;
    if (me->pos.x >= minX && me->pos.x <= maxX && me->pos.z >= minZ && me->pos.z <= maxZ) {
        dx = me->pos.x - other->pos.x;
        dz = me->pos.z - other->pos.z;
        mag = sqrt(dx * dx + dz * dz);
        spC = mag;
        dx = 180.0 * (acos(dx / (0.1f + spC)) / G4_PI);
        dz = 180.0 * (asin(dz / (0.1f + spC)) / G4_PI);
        if (dz < 0.0f) {
            dx = 360.0f - dx;
        }
        baseAng = dx;
        for (i = 0; i < 20; i++) {
            dist[i] = 2000.0f;
            leafMin[i] = 2000.0f;
            order[i] = i;
            order2[i] = i;
        }
        cx = me->pos.x;
        cz = me->pos.z;
        ang = baseAng - 100.0f;
        n = 0;
        ix = iz = 0.0f;
        for (; ang < 100.0f + baseAng; ang += 10.0f, n++) {
            sp4C = cx + 10.0 * cos(G4_PI * ang / 180.0);
            sp48 = cz + 10.0 * sin(G4_PI * ang / 180.0);
            ez = sp48 - cz;
            ex = cx - sp4C;
            sp3C = -ez * cx - ex * cz;
            for (j = 0; j < lbl_1_bss_45C.simSpawnCount - 1; j++) {
                HuVecF *p0 = &lbl_1_bss_45C.simSpawn[lbl_1_bss_45C.simSpawnIdx[j]];
                HuVecF *p1 = &lbl_1_bss_45C.simSpawn[lbl_1_bss_45C.simSpawnIdx[j + 1]];
                if (p0->x < p1->x) {
                    xLo = p0->x;
                    xHi = p1->x;
                } else {
                    xLo = p1->x;
                    xHi = p0->x;
                }
                if (p0->z < p1->z) {
                    zLo = p0->z;
                    zHi = p1->z;
                } else {
                    zLo = p1->z;
                    zHi = p0->z;
                }
                lineA = lbl_1_data_3E8[j].x;
                lineB = lbl_1_data_3E8[j].y;
                sp38 = lbl_1_data_3E8[j].z;
                lenProd = sqrt(ez * ez + ex * ex) * sqrt(lineA * lineA + lineB * lineB);
                if (lenProd != 0.0f) {
                    cosA = (ez * lineA + ex * lineB) / lenProd;
                } else {
                    cosA = 0.0f;
                }
                if (cosA != 0.0f) {
                    if (ez != 0.0f && ex != 0.0f && lineA != 0.0f && lineB != 0.0f) {
                        t1 = lineA * (-ex / ez) + lineB;
                        t2 = -1.0f * (lineA * (-sp3C / ez)) - sp38;
                        if (t1 != 0.0f) {
                            iz = t2 / t1;
                        } else {
                            iz = 0.0f;
                        }
                        ix = (-lineB * iz - sp38) / lineA;
                    } else if (ez == 0.0f && ex != 0.0f && lineA != 0.0f && lineB != 0.0f) {
                        iz = -sp3C / ex;
                        ix = (-lineB * iz - sp38) / lineA;
                    } else if (ez != 0.0f && ex == 0.0f && lineA != 0.0f && lineB != 0.0f) {
                        ix = -sp3C / ez;
                        iz = (-lineA * ix - sp38) / lineB;
                    } else if (ez != 0.0f && ex != 0.0f && lineA == 0.0f && lineB != 0.0f) {
                        iz = -sp38 / lineB;
                        ix = (-ex * iz - sp3C) / ez;
                    } else if (ez != 0.0f && ex != 0.0f && lineA != 0.0f && lineB == 0.0f) {
                        ix = -sp38 / lineA;
                        iz = (-ez * ix - sp3C) / ex;
                    } else if (ez == 0.0f && ex != 0.0f && lineA != 0.0f && lineB == 0.0f) {
                        iz = -sp3C / ex;
                        ix = -sp38 / lineA;
                    } else if (ez != 0.0f && ex == 0.0f && lineA == 0.0f && lineB != 0.0f) {
                        ix = -sp3C / ez;
                        iz = -sp38 / lineB;
                    }
                    if (ix >= xLo && ix <= xHi && iz >= zLo && iz <= zHi) {
                        hx = ix - cx;
                        hz = iz - cz;
                        flagX = 0.0f;
                        flagZ = 0.0f;
                        ang360 = ang;
                        if (ang < 0.0f) {
                            ang360 = 360.0f + ang;
                        }
                        if ((ang360 >= 0.0f && ang360 < 90.0f) || (ang360 > 270.0f && ang360 <= 360.0f)) {
                            if (hx > 0.0f) {
                                flagX = 1.0f;
                            }
                        } else if (ang360 == 90.0f || ang360 == 270.0f) {
                            flagX = 1.0f;
                        } else if (hx < 0.0f) {
                            flagX = 1.0f;
                        }
                        if (ang360 > 0.0f && ang360 < 180.0f) {
                            if (hz > 0.0f) {
                                flagZ = 1.0f;
                            }
                        } else if (ang360 == 0.0f || ang360 == 180.0f || ang360 == 360.0f) {
                            flagZ = 1.0f;
                        } else if (hz < 0.0f) {
                            flagZ = 1.0f;
                        }
                        if (flagX != 0.0f && flagZ != 0.0f) {
                            dd = sqrt(hx * hx + hz * hz);
                            if (dd < dist[n]) {
                                dist[n] = dd;
                                hit[n].x = ix;
                                hit[n].z = iz;
                            }
                        }
                    }
                } else {
                    dist[n] = 0.0f;
                }
            }
            for (j = 0; j < lbl_1_bss_2F0.spawnCount; j++) {
                ix = sp4C - cx;
                iz = sp48 - cz;
                dd = ix * ix + iz * iz;
                sp38 = -1.0f * (ix * (sp4C - lbl_1_bss_3730->model[lbl_1_bss_36A4[j].a].posX)
                                + iz * (sp48 - lbl_1_bss_3730->model[lbl_1_bss_36A4[j].a].posZ));
                if (dd != 0.0f) {
                    t1 = sp38 / dd;
                } else {
                    t1 = 0.0f;
                }
                sp44 = sp4C + ix * t1;
                sp40 = sp48 + iz * t1;
                hx = sp44 - cx;
                hz = sp40 - cz;
                flagX = 0.0f;
                flagZ = 0.0f;
                ang360 = ang;
                if (ang < 0.0f) {
                    ang360 = 360.0f + ang;
                }
                if ((ang360 >= 0.0f && ang360 < 90.0f) || (ang360 > 270.0f && ang360 <= 360.0f)) {
                    if (hx > 0.0f) {
                        flagX = 1.0f;
                    }
                } else if (ang360 == 90.0f || ang360 == 270.0f) {
                    flagX = 1.0f;
                } else if (hx < 0.0f) {
                    flagX = 1.0f;
                }
                if (ang360 > 0.0f && ang360 < 180.0f) {
                    if (hz > 0.0f) {
                        flagZ = 1.0f;
                    }
                } else if (ang360 == 0.0f || ang360 == 180.0f || ang360 == 360.0f) {
                    flagZ = 1.0f;
                } else if (hz < 0.0f) {
                    flagZ = 1.0f;
                }
                if (flagX != 0.0f && flagZ != 0.0f) {
                    ix = lbl_1_bss_3730->model[lbl_1_bss_36A4[j].a].posX - sp44;
                    iz = lbl_1_bss_3730->model[lbl_1_bss_36A4[j].a].posZ - sp40;
                    spC = sqrt(ix * ix + iz * iz);
                    if (spC < 70.0f) {
                        spC = sqrt(hx * hx + hz * hz);
                        if (spC <= leafMin[n]) {
                            leafMin[n] = spC;
                        }
                    }
                }
            }
        }
        for (i = 0; i < 20; i++) {
            for (j = i; j < 20; j++) {
                if (dist[i] <= dist[j] && dist[j] != 2000.0f) {
                    tmp = dist[i];
                    dist[i] = dist[j];
                    dist[j] = tmp;
                    tmpS = order[i];
                    order[i] = order[j];
                    order[j] = tmpS;
                }
            }
        }
        found = 0.0f;
        for (i = 0; i < 20; i++) {
            if (leafMin[order[i]] >= 150.0f) {
                ang = (baseAng - 100.0f) + 10.0f * order[i];
                px = me->pos.x + 150.0 * cos(G4_PI * ang / 180.0);
                pz = me->pos.z + 150.0 * sin(G4_PI * ang / 180.0);
                if (G4_INSIDE(px, pz)) {
                    if (!(px >= minX && px <= maxX && pz >= minZ && pz <= maxZ)) {
                        break;
                    }
                }
            }
        }
        if (i >= 20) {
            found = 1.0f;
        }
        if (found == 1.0f) {
            for (i = 0; i < 20; i++) {
                ang = (baseAng - 100.0f) + 10.0f * order[i];
                px = me->pos.x + 150.0 * cos(G4_PI * ang / 180.0);
                pz = me->pos.z + 150.0 * sin(G4_PI * ang / 180.0);
                if (G4_INSIDE(px, pz)) {
                    if (!(px >= minX && px <= maxX && pz >= minZ && pz <= maxZ)) {
                        break;
                    }
                }
            }
            found = 0.0f;
            if (i >= 20) {
                found = 1.0f;
            }
        }
        if (found == 1.0f) {
            for (i = 0; i < 20; i++) {
                ang = (baseAng - 100.0f) + 10.0f * order[i];
                px = me->pos.x + 150.0 * cos(G4_PI * ang / 180.0);
                pz = me->pos.z + 150.0 * sin(G4_PI * ang / 180.0);
                if (G4_INSIDE(px, pz)) {
                    break;
                }
            }
            found = 0.0f;
            if (i >= 20) {
                found = 1.0f;
            }
        }
        if (found != 1.0f) {
            lbl_1_bss_2734.moveX = me->pos.x + 100.0 * cos(G4_PI * ang / 180.0);
            lbl_1_bss_2734.moveZ = me->pos.z + 100.0 * sin(G4_PI * ang / 180.0);
            lbl_1_bss_2734.goal.x = lbl_1_bss_2734.moveX + 50.0 * cos(G4_PI * ang / 180.0);
            lbl_1_bss_2734.goal.z = lbl_1_bss_2734.moveZ + 50.0 * sin(G4_PI * ang / 180.0);
            ret = 1;
        } else {
            lbl_1_bss_2734.moveX = 0.0f;
            lbl_1_bss_2734.moveZ = 0.0f;
            lbl_1_bss_2734.goal.x = 0.0f;
            lbl_1_bss_2734.goal.z = 0.0f;
            ret = 0;
        }
        for (i = 0; i < 2; i++) {
            if ((s32)lbl_1_bss_2734.moveX > 700 || (s32)lbl_1_bss_2734.moveX < -700
                || (s32)lbl_1_bss_2734.moveZ > 700 || (s32)lbl_1_bss_2734.moveZ < -700
                || (s32)lbl_1_bss_2734.goal.x > 700 || (s32)lbl_1_bss_2734.goal.x < -700
                || (s32)lbl_1_bss_2734.goal.z > 700 || (s32)lbl_1_bss_2734.goal.z < -700) {
                lbl_1_bss_2734.moveX = 0.0f;
                lbl_1_bss_2734.moveZ = 0.0f;
                lbl_1_bss_2734.goal.x = 0.0f;
                lbl_1_bss_2734.goal.z = 0.0f;
                ret = 0;
            }
        }
    } else {
        ret = 0;
    }
    return ret;
}

/* ---- part_g5.c ---- */
#undef MDL
/* model table entry of leaf k (first model of its triangle) */
#define MDL(k) (lbl_1_bss_3730->model[lbl_1_bss_36A4[k].a])
/* model table entry of the k-th leaf of the board path */
#define MDLN(k) (lbl_1_bss_3730->model[lbl_1_bss_36A4[lbl_1_bss_2F0.nextLeaf[k]].a])

s32 fn_1_DDCC(void)
{
    M541_MODELWORK *workA = &lbl_1_bss_3730->work[0];
    M541_MODELWORK *workB = &lbl_1_bss_3730->work[1];
    s32 ret = 0;
    s16 flag = 1;
    s16 i;
    float tbl[32] = {
        0.0f, -200.0f, 200.0f, -120.0f, -200.0f, 100.0f, 0.0f, 220.0f,
        300.0f, 0.0f, -300.0f, 0.0f, 0.0f, -300.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
    };

    switch (lbl_1_bss_2734.mode) {
        case 0:
            fn_1_2614();
            fn_1_270C(1);
            lbl_1_bss_2734.mode++;
            break;
        case 1:
            fn_1_92FC(0);
            if (lbl_1_bss_2F0.mode == 5) {
                workA->motNo = 4;
                workA->speed = 1.0f;
                workA->time = 0.0f;
                workA->attrReset = 0;
                workA->attr = 0x40000001;
                workB->motNo = 4;
                workB->speed = 1.0f;
                workB->time = 0.0f;
                workB->attrReset = 0;
                workB->attr = 0x40000001;
                workA->nextF = 1;
                workA->nextMotNo = 0;
                workA->nextTime = 1.0f;
                workA->nextAttr = 0x40000001;
                workA->nextAttrReset = 0;
                workB->nextF = 1;
                workB->nextMotNo = 0;
                workB->nextTime = 1.0f;
                workB->nextAttr = 0x40000001;
                workB->nextAttrReset = 0;
                lbl_1_bss_2734.mode++;
            }
            break;
        case 2:
            if ((s16)fn_1_92FC(0) != 0) {
                lbl_1_bss_2F0.count = 0;
                lbl_1_bss_2734.mode++;
            }
            break;
        case 3:
            for (i = 0; i < 7; i++) {
                flag = (s16)(flag * fn_1_A9B8(lbl_1_bss_2F0.spawnCount + i, 2, 0.0f, 0.0f));
            }
            if (flag != 0) {
                lbl_1_bss_2F0.spawnCount += 7;
                lbl_1_bss_2734.mode++;
            }
            break;
        case 4:
        default:
            break;
    }
    return ret;
}

s32 fn_1_E0A4(void)
{
    HU3DMODEL *mdl = &Hu3DData[lbl_1_bss_3730->model[(lbl_1_bss_2734.cpuPlayer & 1) + 0x6A].modelId];
    M541_MODELWORK *work = &lbl_1_bss_3730->work[lbl_1_bss_2734.cpuPlayer & 1];
    s32 ret = 0;
    s32 done = 0;
    s16 count;
    s16 i;
    M541_MODEL *m0;
    M541_MODEL *m1;
    M541_MODEL *m2;
    float rate;

    switch (lbl_1_bss_2734.mode) {
        case 0:
            for (i = 6; i <= 0x23; i++) {
                Hu3DModelAttrSet(lbl_1_bss_3730->model[i].modelId, HU3D_ATTR_DISPOFF);
            }
            for (i = 0xF; i < 0x17; i++) {
                lbl_1_bss_20[i].tpLvl = 1.0f;
                lbl_1_bss_20[i].exist = 1;
            }
            for (i = 0; i < 0x17; i++) {
                if (lbl_1_bss_20[i].exist != 0) {
                    m0 = &MDL(i);
                    lbl_1_bss_20[i].vel.y = 200.0f;
                    lbl_1_bss_20[i].tpLvl = 1.0f;
                }
            }
            fn_1_29EC(0);
            fn_1_29EC(1);
            fn_1_29EC(2);
            lbl_1_bss_2734.mode++;
            break;
        case 1:
            count = 0;
            for (i = 0; i < 0x17; i++) {
                if (lbl_1_bss_20[i].exist != 0) {
                    count++;
                    m0 = &MDL(i);
                    m1 = &lbl_1_bss_3730->model[lbl_1_bss_36A4[i].b];
                    m2 = &lbl_1_bss_3730->model[lbl_1_bss_36A4[i].c];
                    rate = 60.0f;
                    done += fn_1_1850(&m0->posY, rate, lbl_1_bss_20[i].vel.y, 0.0f);
                    done += fn_1_1850(&m1->posY, rate, lbl_1_bss_20[i].vel.y, 0.0f);
                    done += fn_1_1850(&m2->posY, rate, lbl_1_bss_20[i].vel.y, 0.0f);
                    if ((lbl_1_bss_20[i].tpLvl -= 0.02f) <= 0.0f) {
                        lbl_1_bss_20[i].tpLvl = 0.0f;
                    } else {
                        done = 1;
                    }
                    Hu3DModelTPLvlSet(m0->modelId, lbl_1_bss_20[i].tpLvl);
                    Hu3DModelTPLvlSet(m1->modelId, lbl_1_bss_20[i].tpLvl);
                    Hu3DModelTPLvlSet(m2->modelId, lbl_1_bss_20[i].tpLvl);
                    if ((s16)done == 0) {
                        lbl_1_bss_20[i].unk0 = 0;
                        lbl_1_bss_20[i].exist = 0;
                        Hu3DModelAttrSet(m0->modelId, HU3D_ATTR_DISPOFF);
                        Hu3DModelAttrSet(m1->modelId, HU3D_ATTR_DISPOFF);
                        Hu3DModelAttrSet(m2->modelId, HU3D_ATTR_DISPOFF);
                    }
                }
            }
            if (count != 0) {
                if (lbl_1_bss_36A0++ <= 0x50) {
                    break;
                }
            }
            for (i = 6; i <= 0x68; i++) {
                Hu3DModelAttrSet(lbl_1_bss_3730->model[i].modelId, HU3D_ATTR_DISPOFF);
            }
            lbl_1_bss_2734.mode++;
            break;
        case 2:
            if (lbl_1_bss_2734.cpuPlayer != 0) {
                if (mdl->rot.y <= 1.0f && mdl->rot.y >= -1.0f) {
                    lbl_1_bss_2734.mode = 4;
                } else {
                    lbl_1_bss_2734.unk8 = mdl->rot.y;
                    work->shiftMode = 1;
                    work->shiftMotNo = 1;
                    work->shiftSpeed = 0.0f;
                    work->shiftTime = 4.0f;
                    work->shiftAttr = 0;
                    work->attr = 0x40000001;
                    lbl_1_bss_2734.mode = 3;
                }
            } else {
                lbl_1_bss_2734.mode = 5;
            }
            break;
        case 3:
            rate = 30.0f;
            done += fn_1_1850(&mdl->rot.y, rate, lbl_1_bss_2734.unk8, 0.0f);
            if ((s16)done == 0) {
                work->shiftMode = 1;
                work->shiftMotNo = 0;
                work->shiftSpeed = 0.0f;
                work->shiftTime = 4.0f;
                work->shiftAttr = 0;
                work->attr = 0x40000001;
                lbl_1_bss_2734.mode++;
            }
            break;
        case 4:
            rate = 30.0f;
            done += fn_1_1850(&lbl_1_bss_1C->center.x, rate, lbl_1_data_1F8[1].center.x, mdl->pos.x);
            done += fn_1_1850(&lbl_1_bss_1C->center.y, rate, lbl_1_data_1F8[1].center.y, 70.0f + mdl->pos.y);
            done += fn_1_1850(&lbl_1_bss_1C->center.z, rate, lbl_1_data_1F8[1].center.z, mdl->pos.z);
            done += fn_1_1850(&lbl_1_bss_1C->rot.x, rate, lbl_1_data_1F8[1].rot.x, -18.0f);
            done += fn_1_1850(&lbl_1_bss_1C->zoom, rate, lbl_1_data_1F8[1].zoom, 500.0f);
            if ((s16)done == 0) {
                lbl_1_bss_2734.mode++;
            }
            break;
        case 5:
            ret = 1;
            break;
        default:
            break;
    }
    return ret;
}

int fn_1_E900(COL_NARROW_PARAM *a, COL_NARROW_PARAM *b)
{
    switch (b->type) {
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
    }
    return 0;
}

void fn_1_E91C(COLBODY *body, void *user)
{
    s32 temp = body->param.paramB;
}

void fn_1_E930(void)
{
    s16 unused0 = 0;
    s16 unused1 = 0;
    s16 unused2 = 0;
    s16 i;
    s16 j;
    float a = 30.0f;
    float b = 30.000002f;
    float c = 10.0f;
    float tbl[8] = { 90.0f, 180.0f, -90.0f, 0.0f, 90.0f, -180.0f, -90.0f, 0.0f };
    float dx;
    float dz;
    float dist;
    float len2;
    float t0;
    float cx;
    float scale;
    float cz;
    float inside;
    float t;

    for (i = 0; i < 15; i++) {
        if (lbl_1_bss_20[i].unk0 == 0 || lbl_1_bss_20[i].hidden != 0) {
            continue;
        }
        for (j = 0; j < 15; j++) {
            if (lbl_1_bss_20[j].unk0 == 0 || i == j) {
                continue;
            }
            dx = MDL(j).posX - MDL(i).posX;
            dz = MDL(j).posZ - MDL(i).posZ;
            dist = sqrt(dx * dx + dz * dz);
            if (dist < a + b) {
                if (dist <= 1.0f) {
                    dist = 1.0f;
                }
                scale = (10.0f + (a + b)) / dist;
                dx *= scale;
                dz *= scale;
                MDL(i).posX = MDL(j).posX - dx;
                MDL(i).posZ = MDL(j).posZ - dz;
                dist = sqrt((MDL(j).posX - MDL(i).posX) * (MDL(j).posX - MDL(i).posX)
                             + (MDL(j).posZ - MDL(i).posZ) * (MDL(j).posZ - MDL(i).posZ));
                if (dist <= 1.0f) {
                    dist = 1.0f;
                }
                t0 = 0.0f;
                len2 = t0;
                len2 = (MDL(j).posZ - MDL(i).posZ) / dist;
                t0 = (MDL(j).posX - MDL(i).posX) / dist;
                if (0.0f == len2 && 0.0f == t0) {
                    len2 = 1.0f;
                    t0 = 0.0f;
                }
                dx = lbl_1_bss_20[j].vel.x * t0 + lbl_1_bss_20[j].vel.z * len2;
                dz = -lbl_1_bss_20[i].vel.x * len2 + lbl_1_bss_20[i].vel.z * t0;
                cx = lbl_1_bss_20[i].vel.x * t0 + lbl_1_bss_20[i].vel.z * len2;
                cz = -lbl_1_bss_20[j].vel.x * len2 + lbl_1_bss_20[j].vel.z * t0;
                lbl_1_bss_20[i].vel.x = dx * t0 - dz * len2;
                lbl_1_bss_20[i].vel.z = dz * t0 + dx * len2;
                lbl_1_bss_20[j].vel.x = cx * t0 - cz * len2;
                lbl_1_bss_20[j].vel.z = cz * t0 + cx * len2;
                dx = sqrt(lbl_1_bss_20[i].vel.x * lbl_1_bss_20[i].vel.x + lbl_1_bss_20[i].vel.z * lbl_1_bss_20[i].vel.z);
                dz = sqrt(lbl_1_bss_20[j].vel.x * lbl_1_bss_20[j].vel.x + lbl_1_bss_20[j].vel.z * lbl_1_bss_20[j].vel.z);
                if (dx != 0.0f) {
                    dx = lbl_1_bss_2F0.length / dx;
                } else {
                    dx = 0.0f;
                }
                if (dz != 0.0f) {
                    dz = lbl_1_bss_2F0.length / dz;
                } else {
                    dz = 0.0f;
                }
                lbl_1_bss_20[i].vel.x *= dx;
                lbl_1_bss_20[i].vel.z *= dx;
                lbl_1_bss_20[j].vel.x *= dz;
                lbl_1_bss_20[j].vel.z *= dz;

                dx = 180.0 * (acos(lbl_1_bss_20[i].vel.x / (0.1f + lbl_1_bss_2F0.length)) / 3.141592653589793);
                dz = 180.0 * (asin(lbl_1_bss_20[i].vel.z / (0.1f + lbl_1_bss_2F0.length)) / 3.141592653589793);
                if (dz < 0.0f) {
                    dx = 360.0f - dx;
                }
                lbl_1_bss_20[i].angle = dx;
                lbl_1_bss_20[i].vel.x = lbl_1_bss_2F0.length * cos(3.141592653589793 * lbl_1_bss_20[i].angle / 180.0);
                lbl_1_bss_20[i].vel.y = 0.0f;
                lbl_1_bss_20[i].vel.z = lbl_1_bss_2F0.length * sin(3.141592653589793 * lbl_1_bss_20[i].angle / 180.0);
                if (lbl_1_bss_20[j].hidden == 0) {
                    dx = 180.0 * (acos(lbl_1_bss_20[j].vel.x / (0.1f + lbl_1_bss_2F0.length)) / 3.141592653589793);
                    dz = 180.0 * (asin(lbl_1_bss_20[j].vel.z / (0.1f + lbl_1_bss_2F0.length)) / 3.141592653589793);
                    if (dz < 0.0f) {
                        dx = 360.0f - dx;
                    }
                    lbl_1_bss_20[j].angle = dx;
                    lbl_1_bss_20[j].vel.x = lbl_1_bss_2F0.length * cos(3.141592653589793 * lbl_1_bss_20[j].angle / 180.0);
                    lbl_1_bss_20[j].vel.y = 0.0f;
                    lbl_1_bss_20[j].vel.z = lbl_1_bss_2F0.length * sin(3.141592653589793 * lbl_1_bss_20[j].angle / 180.0);
                }
            }
        }

        dx = MDL(i).posX;
        dz = MDL(i).posZ;
        for (j = 0; j < 8; j++) {
            dx = lbl_1_data_368[j][0] - lbl_1_data_368[j][2];
            dz = lbl_1_data_368[j][1] - lbl_1_data_368[j][3];
            len2 = dx * dx + dz * dz;
            t0 = -1.0f * (dx * (lbl_1_data_368[j][0] - MDL(i).posX) + dz * (lbl_1_data_368[j][1] - MDL(i).posZ));
            if (len2 != 0.0f) {
                t = t0 / len2;
            } else {
                t = 0.0f;
            }
            cx = dx * t + lbl_1_data_368[j][0];
            cz = dz * scale + lbl_1_data_368[j][1];
            dx = MDL(i).posX - cx;
            dz = MDL(i).posZ - cz;
            dist = sqrt(dx * dx + dz * dz);
            if (dist < b) {
                if (dist <= 1.0f) {
                    dist = 1.0f;
                }
                scale = b / dist;
                dx *= scale;
                dz *= scale;
                MDL(i).posX = cx + dx;
                MDL(i).posZ = cz + dz;
                lbl_1_bss_20[i].angle = 360.0f - lbl_1_bss_20[i].angle;
                lbl_1_bss_20[i].angle += tbl[j];
                if (lbl_1_bss_20[i].angle >= 360.0f) {
                    lbl_1_bss_20[i].angle -= 360.0f;
                }
                lbl_1_bss_20[i].vel.x = lbl_1_bss_2F0.length * cos(3.141592653589793 * lbl_1_bss_20[i].angle / 180.0);
                lbl_1_bss_20[i].vel.y = 0.0f;
                lbl_1_bss_20[i].vel.z = lbl_1_bss_2F0.length * sin(3.141592653589793 * lbl_1_bss_20[i].angle / 180.0);
            }
        }
    }

    for (i = 0; i < 2; i++) {
        if (lbl_1_bss_2614[i].stunMode == 2) {
            continue;
        }
        for (j = 0; j < 15; j++) {
            if (lbl_1_bss_20[j].unk0 == 0) {
                continue;
            }
            dx = MDL(j).posX - lbl_1_bss_2614[i].pos.x;
            dz = MDL(j).posZ - lbl_1_bss_2614[i].pos.z;
            dist = sqrt(dx * dx + dz * dz);
            if (dist < a + b) {
                if (dist <= 1.0f) {
                    dist = 1.0f;
                }
                scale = (a + b) / dist;
                dx *= scale;
                dz *= scale;
                lbl_1_bss_2614[i].stunMode = 2;
            }
        }
        if (lbl_1_bss_2F0.ready != 0) {
            for (j = 0; j < lbl_1_bss_2F0.count + 1; j++) {
                if (MDLN(j).posX < MDLN(j + 1).posX) {
                    dx = MDLN(j).posX;
                    cx = MDLN(j + 1).posX;
                } else {
                    dx = MDLN(j + 1).posX;
                    cx = MDLN(j).posX;
                }
                if (MDLN(j).posZ < MDLN(j + 1).posZ) {
                    dz = MDLN(j).posZ;
                    cz = MDLN(j + 1).posZ;
                } else {
                    dz = MDLN(j + 1).posZ;
                    cz = MDLN(j).posZ;
                }
                if (a + lbl_1_bss_2614[i].pos.x >= dx
                    && lbl_1_bss_2614[i].pos.x - a <= cx
                    && a + lbl_1_bss_2614[i].pos.z >= dz
                    && lbl_1_bss_2614[i].pos.z - a <= cz) {
                    dx = MDLN(j).posX - MDLN(j + 1).posX;
                    dz = MDLN(j).posZ - MDLN(j + 1).posZ;
                    len2 = dx * dx + dz * dz;
                    t0 = -1.0f * (dx * (MDLN(j).posX - lbl_1_bss_2614[i].pos.x)
                                 + dz * (MDLN(j).posZ - lbl_1_bss_2614[i].pos.z));
                    if (len2 != 0.0f) {
                        t = t0 / len2;
                    } else {
                        t = 0.0f;
                    }
                    cx = dx * t + MDLN(j).posX;
                    cz = dz * t + MDLN(j).posZ;
                    dx = lbl_1_bss_2614[i].pos.x - cx;
                    dz = lbl_1_bss_2614[i].pos.z - cz;
                    dist = sqrt(dx * dx + dz * dz);
                    if (dist < a + c) {
                        lbl_1_bss_2614[i].stunMode = 2;
                    }
                }
            }
        }
        dx = lbl_1_bss_2614[i].pos.x;
        dz = lbl_1_bss_2614[i].pos.z;
        inside = -1.0f;
        if (-dx + dz > -784.0f && dx < 554.0f && dx + dz < 784.0f && dz < 554.0f
            && -dx + dz < 784.0f && dx > -554.0f && dx + dz > -784.0f && dz > -554.0f) {
            inside = 1.0f;
        }
        for (j = 0; j < 8; j++) {
            dx = lbl_1_data_368[j][0] - lbl_1_data_368[j][2];
            dz = lbl_1_data_368[j][1] - lbl_1_data_368[j][3];
            len2 = dx * dx + dz * dz;
            t0 = -1.0f * (dx * (lbl_1_data_368[j][0] - lbl_1_bss_2614[i].pos.x)
                         + dz * (lbl_1_data_368[j][1] - lbl_1_bss_2614[i].pos.z));
            if (len2 != 0.0f) {
                t = t0 / len2;
            } else {
                t = 0.0f;
            }
            cx = dx * t + lbl_1_data_368[j][0];
            cz = dz * scale + lbl_1_data_368[j][1];
            dx = lbl_1_bss_2614[i].pos.x - cx;
            dz = lbl_1_bss_2614[i].pos.z - cz;
            dist = sqrt(dx * dx + dz * dz);
            if (dist < a) {
                if (dist <= 1.0f) {
                    dist = 1.0f;
                }
                scale = (20.0f + a) / dist;
                dx = inside * (dx * scale);
                dz = inside * (dz * scale);
                lbl_1_bss_2614[i].pos.x = cx + dx;
                lbl_1_bss_2614[i].pos.z = cz + dz;
                lbl_1_bss_2614[i].stunMode = 2;
            }
        }
    }

    if (lbl_1_bss_2734.cpuPlayer == 0 && lbl_1_bss_2614[0].stunMode == 2 && lbl_1_bss_2614[1].stunMode == 2) {
        i = frandmod(2);
        lbl_1_bss_2614[i].stunMode = 2;
        lbl_1_bss_2614[i ^ 1].stunMode = 0;
    }
}

/* ---- part_g6.c ---- */
void fn_1_11710(s16 idx)
{
    float speedTbl[4] = { 7.2f, 8.0f, 9.0f, 10.285714f };
    s16 timeTbl[8] = { 120, 60, 540, 180, 1500, 300, 3600, 60 };
    s16 posTbl[8] = { 7, 7, 6, 6, 2, 2, 1, 1 };
    s16 i;
    s16 val;

    if (lbl_1_bss_3730->player[idx].dif != 0) {
        lbl_1_bss_2614[idx].waitMot = 1;
        lbl_1_bss_2614[idx].waitCount = lbl_1_bss_3730->player[idx].dif - 1;
        lbl_1_bss_2614[idx].timerScale = speedTbl[lbl_1_bss_2614[idx].waitCount];
        lbl_1_bss_2614[idx].chaseVel = 11.0f;
        val = timeTbl[lbl_1_bss_2614[idx].waitCount * 2] + frandmod(timeTbl[lbl_1_bss_2614[idx].waitCount * 2 + 1]);
        lbl_1_bss_2614[idx].targetMot = val;
        lbl_1_bss_2614[idx].targetTime = 10;
        val = posTbl[lbl_1_bss_2614[idx].waitCount * 2] + frandmod(posTbl[lbl_1_bss_2614[idx].waitCount * 2 + 1]);
        lbl_1_bss_2614[idx].targetPos = val;
    }
    lbl_1_bss_45C.simSpawnCount = 0;
    for (i = 0; i < 8; i++) {
        lbl_1_bss_45C.simSpawn[lbl_1_bss_45C.simSpawnCount].x = lbl_1_data_368[i][0];
        lbl_1_bss_45C.simSpawn[lbl_1_bss_45C.simSpawnCount].y = 0.0f;
        lbl_1_bss_45C.simSpawn[lbl_1_bss_45C.simSpawnCount].z = lbl_1_data_368[i][1];
        lbl_1_bss_45C.simSpawnIdx[i] = i;
        lbl_1_bss_45C.simSpawnCount++;
    }
    lbl_1_bss_45C.simSpawn[lbl_1_bss_45C.simSpawnCount].x = lbl_1_data_368[0][0];
    lbl_1_bss_45C.simSpawn[lbl_1_bss_45C.simSpawnCount].y = 0.0f;
    lbl_1_bss_45C.simSpawn[lbl_1_bss_45C.simSpawnCount].z = lbl_1_data_368[0][1];
    lbl_1_bss_45C.simSpawnIdx[i] = i;
    lbl_1_bss_45C.simSpawnCount++;
}

void fn_1_11B7C(s16 arg0)
{
    switch (lbl_1_bss_45C.simState) {
        case 0:
            fn_1_12008(29);
            lbl_1_bss_45C.simState++;
        case 1:
            lbl_1_bss_45C.simTimer++;
            fn_1_12830();
            break;
    }
}

s32 fn_1_11BF8(s16 arg0)
{
    s32 ret = 0;

    lbl_1_bss_2614[arg0].padStkX = 0;
    lbl_1_bss_2614[arg0].padStkY = 0;
    lbl_1_bss_2614[arg0].padBtnDown = 0;
    lbl_1_bss_2614[arg0].padBtn = 0;
    switch (lbl_1_bss_2614[arg0].otherMode) {
        case 0:
            lbl_1_bss_2614[arg0].otherMode++;
            lbl_1_bss_2614[arg0].otherSub = arg0 * 5;
        case 1:
            if (lbl_1_bss_2614[arg0].otherSub++ >= lbl_1_bss_2614[arg0].targetTime) {
                fn_1_14EB0(arg0);
                lbl_1_bss_2614[arg0].otherSub = 0;
                if (lbl_1_bss_45C.simTimer > lbl_1_bss_2614[arg0].targetMot) {
                    lbl_1_bss_2614[arg0].targetTime += lbl_1_bss_2614[arg0].targetPos;
                }
            }
            fn_1_11DE8(arg0);
            break;
    }
    return ret;
}

s32 fn_1_11DE8(s16 idx)
{
    s32 ret = 0;
    s16 chased = 0;

    switch (lbl_1_bss_2614[idx].chaseSub) {
        case 0:
            MgPlayerComStkOn(lbl_1_bss_0.player[idx]);
            lbl_1_bss_2614[idx].chaseSub++;
            break;
        case 1:
            if ((s32)lbl_1_bss_2614[idx].chasePos.x > 700 || (s32)lbl_1_bss_2614[idx].chasePos.x < -700
                || (s32)lbl_1_bss_2614[idx].chasePos.z > 700 || (s32)lbl_1_bss_2614[idx].chasePos.z < -700) {
                return 0;
            }
            chased = MgPlayerVecChase(lbl_1_bss_0.player[idx], &lbl_1_bss_2614[idx].chasePos, lbl_1_bss_2614[idx].chaseRadius, lbl_1_bss_2614[idx].chaseVel);
            break;
        case 2:
            MgPlayerComStkOff(lbl_1_bss_0.player[idx]);
            lbl_1_bss_2614[idx].chaseSub = 0;
            ret = 1;
            break;
    }
    return ret;
}

s32 fn_1_12008(s16 steps)
{
    s32 ret = 0;
    s16 i;
    s16 j;
    s16 k;

    lbl_1_bss_45C.simCount = 0;
    for (i = 0; i < 15; i++) {
        if (lbl_1_bss_20[i].unk0 != 0) {
            M541_MODEL *mdl = &lbl_1_bss_3730->model[lbl_1_bss_36A4[i].a];
            lbl_1_bss_45C.leafPos[i].x = mdl->posX;
            lbl_1_bss_45C.leafPos[i].y = mdl->posY;
            lbl_1_bss_45C.leafPos[i].z = mdl->posZ;
            lbl_1_bss_45C.simVel[i] = lbl_1_bss_20[i].vel;
            lbl_1_bss_45C.simAngle[i] = lbl_1_bss_20[i].angle;
            lbl_1_bss_45C.simCount++;
        }
        lbl_1_bss_45C.simHistCount[i] = 0;
    }
    lbl_1_bss_45C.simSpeed = lbl_1_bss_2F0.length;
    for (i = 0; i < lbl_1_bss_45C.simCount; i++) {
        lbl_1_bss_45C.simTrail[i][0] = lbl_1_bss_45C.leafPos[i];
        lbl_1_bss_45C.simHist[i][lbl_1_bss_45C.simHistCount[i]] = 0;
        lbl_1_bss_45C.simHistCount[i]++;
    }
    for (k = 0; k < steps; k++) {
        fn_1_194B4();
        fn_1_187F8();
        for (j = 0; j < lbl_1_bss_45C.simCount; j++) {
            lbl_1_bss_45C.simHit[j] = 0;
        }
        fn_1_12F40();
        for (j = 0; j < lbl_1_bss_45C.simCount; j++) {
            if (lbl_1_bss_45C.simSpeed <= 10.0f) {
                lbl_1_bss_45C.simVel[j].x = lbl_1_bss_45C.simSpeed * cos(3.141592653589793 * lbl_1_bss_45C.simAngle[j] / 180.0);
                lbl_1_bss_45C.simVel[j].y = 0.0f;
                lbl_1_bss_45C.simVel[j].z = lbl_1_bss_45C.simSpeed * sin(3.141592653589793 * lbl_1_bss_45C.simAngle[j] / 180.0);
            }
            if (lbl_1_bss_45C.simFixed[j] != 0) {
                lbl_1_bss_45C.simVel[j].x = 0.0f;
                lbl_1_bss_45C.simVel[j].y = 0.0f;
                lbl_1_bss_45C.simVel[j].z = 0.0f;
            }
            lbl_1_bss_45C.leafPos[j].x += lbl_1_bss_45C.simVel[j].x;
            lbl_1_bss_45C.leafPos[j].y += lbl_1_bss_45C.simVel[j].y;
            lbl_1_bss_45C.leafPos[j].z += lbl_1_bss_45C.simVel[j].z;
            lbl_1_bss_45C.simTrail[j][k] = lbl_1_bss_45C.leafPos[j];
            if (lbl_1_bss_45C.simHit[j] != 0 && k != 0) {
                lbl_1_bss_45C.simHist[j][lbl_1_bss_45C.simHistCount[j]] = k;
                lbl_1_bss_45C.simHistCount[j]++;
            }
        }
        lbl_1_bss_45C.simSpeed += 0.002f;
        if (lbl_1_bss_45C.simSpeed > 10.0f) {
            lbl_1_bss_45C.simSpeed = 10.0f;
        }
    }
    for (i = 0; i < lbl_1_bss_45C.simCount; i++) {
        if (lbl_1_bss_45C.simHit[i] != 0) {
            lbl_1_bss_45C.simFlag[i] = 1;
        } else {
            lbl_1_bss_45C.simHist[i][lbl_1_bss_45C.simHistCount[i]] = steps - 1;
            lbl_1_bss_45C.simHistCount[i]++;
            lbl_1_bss_45C.simFlag[i] = 0;
        }
    }
    for (i = 0; i < lbl_1_bss_45C.simCount; i++) {
        lbl_1_bss_45C.simHead[i] = steps;
        if (lbl_1_bss_45C.simHead[i] >= 30) {
            lbl_1_bss_45C.simHead[i] = 0;
        }
        lbl_1_bss_45C.simTail[i] = lbl_1_bss_45C.simHead[i] + 1;
        if (lbl_1_bss_45C.simTail[i] >= 30) {
            lbl_1_bss_45C.simTail[i] = 0;
        }
    }
    return ret;
}

s32 fn_1_12830(void)
{
    s32 ret = 0;
    s16 i;
    s16 j;

    fn_1_194B4();
    fn_1_187F8();
    for (i = 0; i < lbl_1_bss_45C.simCount; i++) {
        lbl_1_bss_45C.simHit[i] = 0;
    }
    fn_1_12F40();
    for (i = 0; i < lbl_1_bss_45C.simCount; i++) {
        if (lbl_1_bss_45C.simSpeed <= 10.0f) {
            lbl_1_bss_45C.simVel[i].x = lbl_1_bss_45C.simSpeed * cos(3.141592653589793 * lbl_1_bss_45C.simAngle[i] / 180.0);
            lbl_1_bss_45C.simVel[i].y = 0.0f;
            lbl_1_bss_45C.simVel[i].z = lbl_1_bss_45C.simSpeed * sin(3.141592653589793 * lbl_1_bss_45C.simAngle[i] / 180.0);
        }
        if (lbl_1_bss_45C.simFixed[i] != 0) {
            lbl_1_bss_45C.simVel[i].x = 0.0f;
            lbl_1_bss_45C.simVel[i].y = 0.0f;
            lbl_1_bss_45C.simVel[i].z = 0.0f;
        }
        lbl_1_bss_45C.leafPos[i].x += lbl_1_bss_45C.simVel[i].x;
        lbl_1_bss_45C.leafPos[i].y += lbl_1_bss_45C.simVel[i].y;
        lbl_1_bss_45C.leafPos[i].z += lbl_1_bss_45C.simVel[i].z;
        lbl_1_bss_45C.simTrail[i][lbl_1_bss_45C.simHead[i]] = lbl_1_bss_45C.leafPos[i];
        if (lbl_1_bss_45C.simHit[i] != 0) {
            if (lbl_1_bss_45C.simFlag[i] != 0) {
                lbl_1_bss_45C.simHistCount[i]++;
            }
            lbl_1_bss_45C.simHist[i][lbl_1_bss_45C.simHistCount[i] - 1] = lbl_1_bss_45C.simHead[i];
            lbl_1_bss_45C.simFlag[i] = 1;
        } else {
            if (lbl_1_bss_45C.simFlag[i] != 0) {
                lbl_1_bss_45C.simHistCount[i]++;
            }
            lbl_1_bss_45C.simHist[i][lbl_1_bss_45C.simHistCount[i] - 1] = lbl_1_bss_45C.simHead[i];
            lbl_1_bss_45C.simFlag[i] = 0;
        }
    }
    lbl_1_bss_45C.simSpeed += 0.002f;
    if (lbl_1_bss_45C.simSpeed > 10.0f) {
        lbl_1_bss_45C.simSpeed = 10.0f;
    }
    for (i = 0; i < lbl_1_bss_45C.simCount; i++) {
        lbl_1_bss_45C.simHist[i][0] = lbl_1_bss_45C.simTail[i];
        if (lbl_1_bss_45C.simHist[i][0] == lbl_1_bss_45C.simHist[i][1]) {
            for (j = 1; j < lbl_1_bss_45C.simHistCount[i]; j++) {
                lbl_1_bss_45C.simHist[i][j - 1] = lbl_1_bss_45C.simHist[i][j];
            }
            lbl_1_bss_45C.simHistCount[i]--;
        }
        lbl_1_bss_45C.simHead[i]++;
        if (lbl_1_bss_45C.simHead[i] >= 30) {
            lbl_1_bss_45C.simHead[i] = 0;
        }
        lbl_1_bss_45C.simTail[i] = lbl_1_bss_45C.simHead[i] + 1;
        if (lbl_1_bss_45C.simTail[i] >= 30) {
            lbl_1_bss_45C.simTail[i] = 0;
        }
    }
    return ret;
}

/* Re-derive the movement angle (degrees) of leaf i from its velocity. */
void fn_1_12F40(void)
{
    float ang[8] = { 90.0f, 180.0f, -90.0f, 0.0f, 90.0f, -180.0f, -90.0f, 0.0f };
    float radA = 30.0f;
    float radB = 30.000002f;
    s16 i;
    s16 j;
    s16 k;
    float dx, dz, dist, f, nx, nz;
    float a1, a2, b1, b2;
    float mi, mj, si, sj, angA;
    float cx, cz, rx, rz, t, len2;

    for (i = 0; i < lbl_1_bss_45C.simCount; i++) {
        if (lbl_1_bss_45C.simFixed[i] != 0) {
            continue;
        }
        for (j = 0; j < lbl_1_bss_45C.simCount; j++) {
            if (i == j) {
                continue;
            }
            dx = lbl_1_bss_45C.leafPos[j].x - lbl_1_bss_45C.leafPos[i].x;
            dz = lbl_1_bss_45C.leafPos[j].z - lbl_1_bss_45C.leafPos[i].z;
            dist = sqrt(dx * dx + dz * dz);
            if (dist < radA + radB) {
                if (dist <= 1.0f) {
                    dist = 1.0f;
                }
                f = (10.0f + (radA + radB)) / dist;
                lbl_1_bss_45C.leafPos[i].x = lbl_1_bss_45C.leafPos[j].x - dx * f;
                lbl_1_bss_45C.leafPos[i].z = lbl_1_bss_45C.leafPos[j].z - dz * f;
                dist = sqrt(
                    (lbl_1_bss_45C.leafPos[j].x - lbl_1_bss_45C.leafPos[i].x) * (lbl_1_bss_45C.leafPos[j].x - lbl_1_bss_45C.leafPos[i].x)
                    + (lbl_1_bss_45C.leafPos[j].z - lbl_1_bss_45C.leafPos[i].z) * (lbl_1_bss_45C.leafPos[j].z - lbl_1_bss_45C.leafPos[i].z));
                if (dist <= 1.0f) {
                    dist = 1.0f;
                }
                nz = (lbl_1_bss_45C.leafPos[j].z - lbl_1_bss_45C.leafPos[i].z) / dist;
                nx = (lbl_1_bss_45C.leafPos[j].x - lbl_1_bss_45C.leafPos[i].x) / dist;
                if (nz == 0.0f && nx == 0.0f) {
                    nz = 1.0f;
                    nx = 0.0f;
                }
                a1 = lbl_1_bss_45C.simVel[j].x * nx + lbl_1_bss_45C.simVel[j].z * nz;
                a2 = -lbl_1_bss_45C.simVel[i].x * nz + lbl_1_bss_45C.simVel[i].z * nx;
                b1 = lbl_1_bss_45C.simVel[i].x * nx + lbl_1_bss_45C.simVel[i].z * nz;
                b2 = -lbl_1_bss_45C.simVel[j].x * nz + lbl_1_bss_45C.simVel[j].z * nx;
                lbl_1_bss_45C.simVel[i].x = a1 * nx - a2 * nz;
                lbl_1_bss_45C.simVel[i].z = a2 * nx + a1 * nz;
                lbl_1_bss_45C.simVel[j].x = b1 * nx - b2 * nz;
                lbl_1_bss_45C.simVel[j].z = b2 * nx + b1 * nz;
                mi = sqrt(lbl_1_bss_45C.simVel[i].x * lbl_1_bss_45C.simVel[i].x + lbl_1_bss_45C.simVel[i].z * lbl_1_bss_45C.simVel[i].z);
                mj = sqrt(lbl_1_bss_45C.simVel[j].x * lbl_1_bss_45C.simVel[j].x + lbl_1_bss_45C.simVel[j].z * lbl_1_bss_45C.simVel[j].z);
                if (mi != 0.0f) {
                    si = lbl_1_bss_45C.simSpeed / mi;
                } else {
                    si = 0.0f;
                }
                if (mj != 0.0f) {
                    sj = lbl_1_bss_45C.simSpeed / mj;
                } else {
                    sj = 0.0f;
                }
                lbl_1_bss_45C.simVel[i].x *= si;
                lbl_1_bss_45C.simVel[i].z *= si;
                lbl_1_bss_45C.simVel[j].x *= sj;
                lbl_1_bss_45C.simVel[j].z *= sj;
                angA = 180.0 * (acos(lbl_1_bss_45C.simVel[i].x / (0.1f + lbl_1_bss_45C.simSpeed)) / 3.141592653589793);
                if ((float)(180.0 * (asin(lbl_1_bss_45C.simVel[i].z / (0.1f + lbl_1_bss_45C.simSpeed)) / 3.141592653589793)) < 0.0f) {
                    angA = 360.0f - angA;
                }
                lbl_1_bss_45C.simAngle[i] = angA;
                lbl_1_bss_45C.simVel[i].x = lbl_1_bss_45C.simSpeed * cos(3.141592653589793 * lbl_1_bss_45C.simAngle[i] / 180.0);
                lbl_1_bss_45C.simVel[i].y = 0.0f;
                lbl_1_bss_45C.simVel[i].z = lbl_1_bss_45C.simSpeed * sin(3.141592653589793 * lbl_1_bss_45C.simAngle[i] / 180.0);
                if (lbl_1_bss_45C.simFixed[j] == 0) {
                    angA = 180.0 * (acos(lbl_1_bss_45C.simVel[j].x / (0.1f + lbl_1_bss_45C.simSpeed)) / 3.141592653589793);
                    if ((float)(180.0 * (asin(lbl_1_bss_45C.simVel[j].z / (0.1f + lbl_1_bss_45C.simSpeed)) / 3.141592653589793)) < 0.0f) {
                        angA = 360.0f - angA;
                    }
                    lbl_1_bss_45C.simAngle[j] = angA;
                    lbl_1_bss_45C.simVel[j].x = lbl_1_bss_45C.simSpeed * cos(3.141592653589793 * lbl_1_bss_45C.simAngle[j] / 180.0);
                    lbl_1_bss_45C.simVel[j].y = 0.0f;
                    lbl_1_bss_45C.simVel[j].z = lbl_1_bss_45C.simSpeed * sin(3.141592653589793 * lbl_1_bss_45C.simAngle[j] / 180.0);
                    lbl_1_bss_45C.simHit[j] = 1;
                }
                lbl_1_bss_45C.simHit[i] = 1;
            }
        }
        for (k = 0; k < 8; k++) {
            dx = lbl_1_data_368[k][0] - lbl_1_data_368[k][2];
            dz = lbl_1_data_368[k][1] - lbl_1_data_368[k][3];
            len2 = dx * dx + dz * dz;
            if (len2 != 0.0f) {
                t = (-1.0f * (dx * (lbl_1_data_368[k][0] - lbl_1_bss_45C.leafPos[i].x) + dz * (lbl_1_data_368[k][1] - lbl_1_bss_45C.leafPos[i].z))) / len2;
            } else {
                t = 0.0f;
            }
            cx = dx * t + lbl_1_data_368[k][0];
            cz = dz * t + lbl_1_data_368[k][1];
            rx = lbl_1_bss_45C.leafPos[i].x - cx;
            rz = lbl_1_bss_45C.leafPos[i].z - cz;
            dist = sqrt(rx * rx + rz * rz);
            if (dist < radB) {
                if (dist <= 1.0f) {
                    dist = 1.0f;
                }
                f = radB / dist;
                lbl_1_bss_45C.leafPos[i].x = cx + rx * f;
                lbl_1_bss_45C.leafPos[i].z = cz + rz * f;
                lbl_1_bss_45C.simAngle[i] = 360.0f - lbl_1_bss_45C.simAngle[i];
                lbl_1_bss_45C.simAngle[i] += ang[k];
                if (lbl_1_bss_45C.simAngle[i] >= 360.0f) {
                    lbl_1_bss_45C.simAngle[i] -= 360.0f;
                }
                lbl_1_bss_45C.simVel[i].x = lbl_1_bss_45C.simSpeed * cos(3.141592653589793 * lbl_1_bss_45C.simAngle[i] / 180.0);
                lbl_1_bss_45C.simVel[i].y = 0.0f;
                lbl_1_bss_45C.simVel[i].z = lbl_1_bss_45C.simSpeed * sin(3.141592653589793 * lbl_1_bss_45C.simAngle[i] / 180.0);
                lbl_1_bss_45C.simHit[i] = 1;
            }
        }
    }
}

/* ---- part_g7.c ---- */
/* lbl_1_bss_45C holds 17 paths: 15 leaf trails followed by the two spawn paths (simSpawn[0..29], [30..59]).
 * G7PATHS is the flat [17][30] view of that memory that the original code indexes. */
typedef struct G7Paths_s {
    s16 hdr[6];         /* 0x0000 */
    HuVecF trail[17][30]; /* 0x000C */
    s16 hist[17][30];   /* 0x17F4 */
    s16 histCount[17];  /* 0x1BF0 */
} G7PATHS;
#define G7_PATHS (*(G7PATHS *)&lbl_1_bss_45C)
#define G7_TRAIL G7_PATHS.trail
#define G7_HIST G7_PATHS.hist
#define G7_HCNT G7_PATHS.histCount
/* point k of path i */
#define G7_PT(i, k) G7_TRAIL[i][G7_HIST[i][k]]

/* absolute value as the original macro: argument is evaluated twice */
#define G7_ABS(e) ((e) > 0.0f ? (e) : -1.0f * (e))

/* "is the point in front of the ray" flags: fx for the x axis, fz for the z axis (angle in degrees) */
#define G7_SIDE(fx, fz, xp, xm, zp, zm)                                                 \
    if ((ang >= 0.0f && ang < 90.0f) || (ang > 270.0f && ang <= 360.0f)) { \
        if ((xp) > 0.0f) {                                            \
            fx = 1.0f;                                                     \
        }                                                                  \
    } else if (ang == 90.0f || ang == 270.0f) {                            \
        fx = 1.0f;                                                         \
    } else if ((xm) < 0.0f) {                                         \
        fx = 1.0f;                                                         \
    }                                                                      \
    if (ang > 0.0f && ang < 180.0f) {                                      \
        if ((zp) > 0.0f) {                                            \
            fz = 1.0f;                                                     \
        }                                                                  \
    } else if (ang == 0.0f || ang == 180.0f || ang == 360.0f) {            \
        fz = 1.0f;                                                         \
    } else if ((zm) < 0.0f) {                                         \
        fz = 1.0f;                                                         \
    }

s32 fn_1_14714(s16 idx)
{
    float rad = 30.0f;
    float margin = 30.000002f;
    s16 i;
    s16 j;
    s32 ret = 0;
    float minX, maxX, minZ, maxZ;
    float dx, dz, t, u, dist;
    float lenSq;

    for (i = 0; i < lbl_1_bss_45C.simCount; i++) {
        for (j = 0; j < G7_HCNT[i] - 1; j++) {
            if (G7_TRAIL[i][j].x < G7_TRAIL[i][j + 1].x) {
                minX = G7_TRAIL[i][j].x;
                maxX = G7_TRAIL[i][j + 1].x;
            } else {
                minX = G7_TRAIL[i][j + 1].x;
                maxX = G7_TRAIL[i][j].x;
            }
            if (G7_TRAIL[i][j].z < G7_TRAIL[i][j + 1].z) {
                minZ = G7_TRAIL[i][j].z;
                maxZ = G7_TRAIL[i][j + 1].z;
            } else {
                minZ = G7_TRAIL[i][j + 1].z;
                maxZ = G7_TRAIL[i][j].z;
            }
            if (rad + lbl_1_bss_2614[idx].pos.x >= minX && lbl_1_bss_2614[idx].pos.x - rad <= maxX
                && rad + lbl_1_bss_2614[idx].pos.z >= minZ && lbl_1_bss_2614[idx].pos.z - rad <= maxZ) {
                dx = G7_TRAIL[i][j].x - G7_TRAIL[i][j + 1].x;
                dz = G7_TRAIL[i][j].z - G7_TRAIL[i][j + 1].z;
                lenSq = dx * dx + dz * dz;
                if (0.0f == lenSq) {
                    lenSq = 1.0f;
                }
                t = -1.0f
                    * (dx * (G7_TRAIL[i][j].x - lbl_1_bss_2614[idx].pos.x) + dz * (G7_TRAIL[i][j].z - lbl_1_bss_2614[idx].pos.z));
                u = t / lenSq;
                minX = G7_TRAIL[i][j].x + dx * u;
                minZ = G7_TRAIL[i][j].z + dz * u;
                dx = lbl_1_bss_2614[idx].pos.x - minX;
                dz = lbl_1_bss_2614[idx].pos.z - minZ;
                dist = sqrt(dx * dx + dz * dz);
                if (dist < rad + margin) {
                    ret = 1;
                }
            }
        }
    }
    return ret;
}

s32 fn_1_14EB0(s16 idx)
{
    HuVec2f dist[36];
    HuVecF hit[36];
    s16 seg[36][2];
    s16 list[36];
    s16 dead198[36];
    s16 flg[36];
    float scl = 60.0f;
    float c30 = 30.0f;
    float c30b = 30.000002f;
    float lim = 90.0f;
    s32 ret = 0;
    s16 cnt = 0;
    s16 i;
    s16 j;
    s16 n;
    s16 found;
    s16 cand;
    s16 nearFlag;
    float px, pz;
    float ang, step;
    float p2x, p2z, cosA, sinA;
    float a1, b1, c1;
    float lenA, lenB;
    float f31, f30, f29, f28, f22;
    float a2, b2, c2;
    float f19, f18;
    float margin;
    float maxX, maxZ;
    float prod;

    for (i = 0; i < 36; i++) {
        dist[i].x = 2000.0f;
        dist[i].y = 2000.0f;
        dead198[i] = 0;
        flg[i] = 0;
    }
    px = lbl_1_bss_2614[idx].pos.x;
    pz = lbl_1_bss_2614[idx].pos.z;
    ang = 0.0f;
    n = 0;
    step = 10.0f;
    for (; ang < 360.0f; ang += step, n++) {
        p2x = px + 10.0 * HuCos(ang);
        p2z = pz + 10.0 * HuSin(ang);
        cosA = HuCos(ang);
        sinA = HuSin(ang);
        a1 = p2z - pz;
        b1 = px - p2x;
        c1 = (-a1) * px - b1 * pz;
        lenA = sqrt(a1 * a1 + b1 * b1);
        f31 = px - p2x;
        f30 = pz - p2z;
        lenB = sqrt(f31 * f31 + f30 * f30);
        for (i = 0; i < 17; i++) {
            margin = 0.0f;
            if (i == lbl_1_bss_45C.simCount) {
                i = 15;
            } else if (i == 16) {
                if (lbl_1_bss_45C.g7Flag200A == 0) {
                    if (lbl_1_bss_2F0.ready == 0) {
                        break;
                    }
                }
                margin = 1.0f;
            }
            for (j = 0; j < G7_HCNT[i] - 1; j++) {
                found = 0;
                nearFlag = 0;
                if (i != 15) {
                    f31 = G7_PT(i, j).x;
                    f30 = G7_PT(i, j).z;
                    f19 = 0.0f;
                    f18 = 0.0f;
                    f29 = f31 - px;
                    f28 = f30 - pz;
                    G7_SIDE(f19, f18, f29 + lim, f29 - lim, f28 + lim, f28 - lim)
                    f22 = sqrt(f29 * f29 + f28 * f28);
                    if (f22 <= lim) {
                        cnt++;
                        nearFlag = 1;
                    }
                    if (f19 != 0.0f && f18 != 0.0f && f22 > lim) {
                        f22 = G7_ABS(c1 + (a1 * f31 + b1 * f30));
                        if (lenA) {
                            f22 = f22 / lenA;
                        } else {
                            f22 = 0.0f;
                        }
                        if (lim >= f22) {
                            f22 = sqrt((f31 - px) * (f31 - px) + (f30 - pz) * (f30 - pz));
                            if (f22 < dist[n].x) {
                                dist[n].x = f22;
                                if (lenB) {
                                    f22 = f22 / lenB;
                                } else {
                                    f22 = 0.0f;
                                }
                                hit[n].x = f31;
                                hit[n].z = f30;
                                seg[n][0] = i;
                                seg[n][1] = j;
                                found = 1;
                            }
                        }
                    }
                    f31 = G7_PT(i, j + 1).x;
                    f30 = G7_PT(i, j + 1).z;
                    f19 = 0.0f;
                    f18 = 0.0f;
                    f29 = f31 - px;
                    f28 = f30 - pz;
                    G7_SIDE(f19, f18, f29 + lim, f29 - lim, f28 + lim, f28 - lim)
                    f22 = sqrt(f29 * f29 + f28 * f28);
                    if (f22 <= lim) {
                        cnt++;
                        nearFlag = 1;
                    }
                    if (f19 != 0.0f && f18 != 0.0f && f22 > lim) {
                        f22 = G7_ABS(c1 + (a1 * f31 + b1 * f30));
                        if (lenA) {
                            f22 = f22 / lenA;
                        } else {
                            f22 = 0.0f;
                        }
                        if (lim >= f22) {
                            f22 = sqrt((f31 - px) * (f31 - px) + (f30 - pz) * (f30 - pz));
                            if (f22 < dist[n].x) {
                                dist[n].x = f22;
                                if (lenB) {
                                    f22 = f22 / lenB;
                                } else {
                                    f22 = 0.0f;
                                }
                                hit[n].x = f31;
                                hit[n].z = f30;
                                seg[n][0] = i;
                                seg[n][1] = j;
                                found++;
                            }
                        }
                    }
                    if (nearFlag != 0) {
                        f31 = G7_PT(i, j + 1).x - G7_PT(i, j).x;
                        f30 = G7_PT(i, j + 1).z - G7_PT(i, j).z;
                        f31 = f31 * 0.5f;
                        f30 = f30 * 0.5f;
                        f31 = f31 + G7_PT(i, j).x;
                        f30 = f30 + G7_PT(i, j).z;
                        f19 = 0.0f;
                        f18 = 0.0f;
                        f29 = f31 - px;
                        f28 = f30 - pz;
                        G7_SIDE(f19, f18, f29 + lim, f29 - lim, f28 + lim, f28 - lim)
                        f22 = sqrt(f29 * f29 + f28 * f28);
                        if (f19 != 0.0f && f18 != 0.0f && f22 > lim) {
                            f22 = G7_ABS(c1 + (a1 * f31 + b1 * f30));
                            if (lenA) {
                                f22 = f22 / lenA;
                            } else {
                                f22 = 0.0f;
                            }
                            f29 = sqrt((f31 - px) * (f31 - px) + (f30 - pz) * (f30 - pz));
                            if (f29 - 0.5f >= f22) {
                                f22 = f29;
                                if (f22 < dist[n].x) {
                                    dist[n].x = f22;
                                    if (lenB) {
                                        f22 = f22 / lenB;
                                    } else {
                                        f22 = 0.0f;
                                    }
                                    hit[n].x = f31;
                                    hit[n].z = f30;
                                    seg[n][0] = i;
                                    seg[n][1] = j;
                                }
                            }
                        }
                    }
                }
                if (found != 0 || nearFlag != 0) {
                    flg[i] = 1;
                }
                if (G7_PT(i, j).x < G7_PT(i, j + 1).x) {
                    f19 = G7_PT(i, j).x;
                    maxX = G7_PT(i, j + 1).x;
                } else {
                    f19 = G7_PT(i, j + 1).x;
                    maxX = G7_PT(i, j).x;
                }
                if (G7_PT(i, j).z < G7_PT(i, j + 1).z) {
                    f18 = G7_PT(i, j).z;
                    maxZ = G7_PT(i, j + 1).z;
                } else {
                    f18 = G7_PT(i, j + 1).z;
                    maxZ = G7_PT(i, j).z;
                }
                if (i != 15) {
                    f31 = G7_PT(i, j).x;
                    f30 = G7_PT(i, j).z;
                    f29 = G7_PT(i, j + 1).x + margin;
                    f28 = G7_PT(i, j + 1).z;
                    a2 = f28 - f30;
                    b2 = f31 - f29;
                    c2 = (-a2) * f31 - b2 * f30;
                } else {
                    a2 = lbl_1_data_3E8[j].x;
                    b2 = lbl_1_data_3E8[j].y;
                    c2 = lbl_1_data_3E8[j].z;
                }
                prod = sqrt(a1 * a1 + b1 * b1) * sqrt(a2 * a2 + b2 * b2);
                if (prod) {
                    margin = (a1 * a2 + b1 * b2) / prod;
                } else {
                    margin = 0.0f;
                }
                if (margin < 0.984f && margin > -0.984f) {
                    if (a1 != 0.0f && b1 != 0.0f && a2 != 0.0f && b2 != 0.0f) {
                        f29 = a2 * (-b1 / a1);
                        f28 = -1.0f * (a2 * (-c1 / a1));
                        f29 = f29 + b2;
                        f28 = f28 - c2;
                        if (f29 != 0.0f) {
                            f30 = f28 / f29;
                        } else {
                            f30 = 0.0f;
                        }
                        f31 = ((-b2) * f30 - c2) / a2;
                    } else if (a1 == 0.0f && b1 != 0.0f && a2 != 0.0f && b2 != 0.0f) {
                        f30 = (-c1) / b1;
                        f31 = ((-b2) * f30 - c2) / a2;
                    } else if (a1 != 0.0f && b1 == 0.0f && a2 != 0.0f && b2 != 0.0f) {
                        f31 = (-c1) / a1;
                        f30 = ((-a2) * f31 - c2) / b2;
                    } else if (a1 != 0.0f && b1 != 0.0f && a2 == 0.0f && b2 != 0.0f) {
                        f30 = (-c2) / b2;
                        f31 = ((-b1) * f30 - c1) / a1;
                    } else if (a1 != 0.0f && b1 != 0.0f && a2 != 0.0f && b2 == 0.0f) {
                        f31 = (-c2) / a2;
                        f30 = ((-a1) * f31 - c1) / b1;
                    } else if (a1 == 0.0f && b1 != 0.0f && a2 != 0.0f && b2 == 0.0f) {
                        f30 = (-c1) / b1;
                        f31 = (-c2) / a2;
                    } else if (a1 != 0.0f && b1 == 0.0f && a2 == 0.0f && b2 != 0.0f) {
                        f31 = (-c1) / a1;
                        f30 = (-c2) / b2;
                    }
                    if (f31 >= f19 && f31 <= maxX && f30 >= f18 && f30 <= maxZ) {
                        f29 = f31 - px;
                        f28 = f30 - pz;
                        f19 = 0.0f;
                        f18 = 0.0f;
                        G7_SIDE(f19, f18, f29, f29, f28, f28)
                        if (f19 != 0.0f && f18 != 0.0f) {
                            f29 = sqrt(f29 * f29 + f28 * f28);
                            if (f29 < dist[n].x) {
                                dist[n].x = f29;
                                hit[n].x = f31;
                                hit[n].z = f30;
                                seg[n][0] = i;
                                seg[n][1] = j;
                            }
                        }
                    }
                }
            }
        }
    }
    i = 0;
    j = 0;
    found = 0;
    cand = 0;
    f19 = dist[0].x;
    maxX = dist[0].x;
    for (; i < n; i++) {
        if ((s32)hit[i].x < 700 && (s32)hit[i].x > -700 && (s32)hit[i].z < 700 && (s32)hit[i].z > -700
            && 2000.0f != dist[i].x) {
            if (f19 <= dist[i].x) {
                f19 = dist[i].x;
                j = i;
            }
            if (maxX >= dist[i].x) {
                maxX = dist[i].x;
            }
            if (dist[i].x >= 600.0f) {
                list[cand] = i;
                cand++;
            }
        } else {
            dist[i].x = -1.0f;
            found++;
            return 0;
        }
    }
    if (cand != 0) {
        i = frandmod(cand);
        j = list[i];
    }
    if (dist[lbl_1_bss_2614[idx].targetIdx].x >= 350.0f) {
        j = lbl_1_bss_2614[idx].targetIdx;
    }
    lbl_1_bss_2614[idx].targetIdx = j;
    lbl_1_bss_2614[idx].targetPosV.x = hit[j].x;
    lbl_1_bss_2614[idx].targetPosV.z = hit[j].z;
    lbl_1_bss_2614[idx].targetDist = f19;
    lbl_1_bss_2614[idx].chaseMode = seg[j][0];
    px -= lbl_1_bss_2614[idx].targetPosV.x;
    pz -= lbl_1_bss_2614[idx].targetPosV.z;
    if (lbl_1_bss_2614[idx].targetDist) {
        p2x = scl / lbl_1_bss_2614[idx].targetDist;
    } else {
        p2x = 0.0f;
    }
    px *= p2x;
    pz *= p2x;
    if ((s32)(px + lbl_1_bss_2614[idx].targetPosV.x) > 700 || (s32)(px + lbl_1_bss_2614[idx].targetPosV.x) < -700
        || (s32)(pz + lbl_1_bss_2614[idx].targetPosV.z) > 700 || (s32)(pz + lbl_1_bss_2614[idx].targetPosV.z) < -700) {
        return 0;
    }
    lbl_1_bss_2614[idx].chasePos.x = px + lbl_1_bss_2614[idx].targetPosV.x;
    lbl_1_bss_2614[idx].chasePos.y = 200.0f;
    lbl_1_bss_2614[idx].chasePos.z = pz + lbl_1_bss_2614[idx].targetPosV.z;
    if (maxX >= 300.0f) {
        lbl_1_bss_2614[idx].chaseRadius = 0.01f;
    } else {
        lbl_1_bss_2614[idx].chaseRadius = lbl_1_bss_2614[idx].timerScale;
    }
    return ret;
}

/* ---- part_g8.c ---- */
/* Raw views of lbl_1_bss_45C (the header names only part of it). */
#define B45C ((u8 *)&lbl_1_bss_45C)
#define W16(off) (*(s16 *)(B45C + (off)))
#define VEC(off, i) (*(HuVecF *)(B45C + (off) + (i) * 0xC))

#define P_CNT W16(0x6)
#define P_SPEED (*(float *)(B45C + 0x8))
#define P_STATE W16(0x2004)
#define P_TIMER W16(0x2006)
#define P_NUM W16(0x2008)
#define P_ACTIVE W16(0x200A)
#define P_SEL(i) W16(0x202A + (i) * 2)
#define P_TMR(i) W16(0x2052 + (i) * 2)
#define P_LEAFPOS(i) VEC(0x1C8C, i)
#define P_LEAFVEL(i) VEC(0x1DA0, i)
#define P_LEAFVELSAVE(i) VEC(0x1EB4, i)

void fn_1_194B4(void)
{
    s16 i;
    s32 done;

    switch (W16(0x2178)) {
        case 0:
            if (W16(0x2176) >= 600 && (W16(0x2176) % 300) == 0 && P_CNT < 15) {
                W16(0x2178)++;
            }
            break;
        case 1:
            done = fn_1_19738(P_CNT, 0.0f, 0.0f);
            if ((s16)done != 0) {
                for (i = 0; i < 30; i++) {
                    VEC(0xC, P_CNT * 30 + i) = P_LEAFPOS(P_CNT);
                }
                W16(0x17F4 + P_CNT * 0x3C) = 0;
                W16(0x17F6 + P_CNT * 0x3C) = 29;
                W16(0x1BF0 + P_CNT * 2) = 2;
                W16(0x1C4E + P_CNT * 2) = 0;
                W16(0x1C12 + P_CNT * 2) = 29;
                W16(0x1C30 + P_CNT * 2) = 0;
                P_CNT++;
                W16(0x2178) = 0;
            }
            break;
    }
    W16(0x2176)++;
}

s32 fn_1_19738(s16 idx, float baseX, float baseZ)
{
    s32 ret = 0;
    float ang;
    float len;

    switch (W16(0x2066 + idx * 2)) {
        case 0:
            W16(0x2084 + idx * 2) = 1;
            ang = *(float *)(B45C + 0x20A4 + idx * 0xC);
            len = *(float *)(B45C + 0x20A8 + idx * 0xC);
            *(float *)(B45C + 0x1C8C + idx * 0xC) = baseX + len * cos(3.141592653589793 * ang / 180.0);
            *(float *)(B45C + 0x1C90 + idx * 0xC) = 0.0f;
            *(float *)(B45C + 0x1C94 + idx * 0xC) = baseZ + len * sin(3.141592653589793 * ang / 180.0);
            W16(0x2158 + idx * 2) = 0;
            W16(0x2066 + idx * 2)++;
            break;
        case 1:
            W16(0x2158 + idx * 2)++;
            if (W16(0x2158 + idx * 2) >= 90) {
                *(float *)(B45C + 0x1FC8 + idx * 4) = *(float *)(B45C + 0x20AC + idx * 0xC);
                *(float *)(B45C + 0x1DA0 + idx * 0xC) = P_SPEED * cos(3.141592653589793 * *(float *)(B45C + 0x1FC8 + idx * 4) / 180.0);
                *(float *)(B45C + 0x1DA4 + idx * 0xC) = 0.0f;
                *(float *)(B45C + 0x1DA8 + idx * 0xC) = P_SPEED * sin(3.141592653589793 * *(float *)(B45C + 0x1FC8 + idx * 4) / 180.0);
                W16(0x2084 + idx * 2) = 0;
                W16(0x2066 + idx * 2) = 1;
                ret = 1;
            }
            break;
    }
    return ret;
}

/* distance between leaves a and b */
#define LEAFDIST(a, b, out) \
    do { \
        float dx_ = P_LEAFPOS(a).x - P_LEAFPOS(b).x; \
        float dz_ = P_LEAFPOS(a).z - P_LEAFPOS(b).z; \
        out = sqrt(dx_ * dx_ + dz_ * dz_); \
    } while (0)

s32 fn_1_187F8(void)
{
    float range[22] = { 0.0f, 0.0f, 400.0f, 650.0f, 350.0f, 600.0f, 300.0f, 550.0f,
                        250.0f, 500.0f, 200.0f, 450.0f, 150.0f, 400.0f, 100.0f, 350.0f,
                        50.0f, 300.0f, 0.0f, 250.0f, 0.0f, 200.0f };
    s32 ret = 0;
    s16 found;
    s16 k, v, c;
    float d;

    found = 0;
    switch (P_STATE) {
        case 0:
            P_TIMER = P_TMR(P_NUM);
            P_NUM++;
            P_STATE++;
            break;
        case 1:
            if (P_TIMER-- < 0) {
                P_TIMER = 0;
                P_STATE++;
            }
            W16(0x2180)++;
            break;
        case 2:
            W16(0x217A)++;
            for (k = 0; k < 10; k++) {
                P_SEL(k) = -1;
            }
            P_SEL(0) = W16(0x203E + P_NUM * 2) + 15;
            for (k = 1; k <= P_NUM; k++) {
                for (c = 0; c < P_CNT; c++) {
                    found = 0;
                    for (v = 0; v < 10; v++) {
                        if (P_SEL(v) == c) {
                            found = 1;
                        }
                    }
                    if (found == 0) {
                        LEAFDIST(c, P_SEL(k - 1), d);
                        if (range[P_NUM * 2] < d && range[P_NUM * 2 + 1] > d) {
                            P_SEL(k) = c;
                            break;
                        }
                    }
                }
                if (P_SEL(k) == -1) {
                    found = 1;
                    break;
                }
            }
            if (found == 0) {
                for (c = 15; c < 23; c++) {
                    if (P_SEL(0) != c) {
                        LEAFDIST(c, P_SEL(P_NUM), d);
                        if (range[P_NUM * 2] < d && range[P_NUM * 2 + 1] > d) {
                            P_SEL(P_NUM + 1) = c;
                            break;
                        }
                    }
                }
                for (k = 0; k <= P_NUM + 1; k++) {
                    if (P_SEL(k) == -1) {
                        k = 99;
                        for (v = 0; v < 10; v++) {
                            P_SEL(v) = -1;
                        }
                    }
                }
                if (P_SEL(0) != -1) {
                    P_STATE++;
                }
            }
            break;
        case 3:
            for (k = 1; k < P_NUM + 1; k++) {
                W16(0x200C + P_SEL(k) * 2) = 1;
                P_LEAFVELSAVE(P_SEL(k)) = P_LEAFVEL(P_SEL(k));
                P_LEAFVEL(P_SEL(k)).x = 0.0f;
                P_LEAFVEL(P_SEL(k)).y = 0.0f;
                P_LEAFVEL(P_SEL(k)).z = 0.0f;
            }
            P_TIMER = 60;
            P_STATE++;
            break;
        case 4:
            if (P_TIMER-- < 0) {
                for (k = 0; k <= P_NUM + 1; k++) {
                    VEC(0x168C, k) = P_LEAFPOS(P_SEL(k));
                    W16(0x1BB4 + k * 2) = k;
                }
                W16(0x1C10) = P_NUM + 2;
                P_ACTIVE = 1;
                P_TIMER = 120;
                P_STATE++;
            }
            break;
        case 5:
            if (P_TIMER-- < 0) {
                for (k = 1; k < P_NUM + 1; k++) {
                    W16(0x200C + P_SEL(k) * 2) = 0;
                    P_LEAFVEL(P_SEL(k)) = P_LEAFVELSAVE(P_SEL(k));
                }
                P_ACTIVE = 0;
                P_STATE = 0;
                ret = 1;
            }
            break;
    }
    return ret;
}
