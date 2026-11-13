#include "REL/m541Dll.h"

#include "string.h"
#include "math.h"
#include "game/frand.h"
#include "game/charman.h"
#include "game/mg/actman.h"

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
