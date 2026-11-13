#include "REL/m549Dll.h"
#include "game/printfunc.h"
#include "math.h"
#include "string.h"


/* ---- G2: lbl_1_data_440..49C, lbl_1_bss_15E0/15F4 ---- */
s16 fn_1_6048(s16 player);
s16 fn_1_67E0(s16 player);
s16 fn_1_71B4(s16 player);

unsigned int lbl_1_data_440[17] = {
    0x008700A7, 0x008C007A, 0x008C007B, 0x008C0003, 0x008C0004, 0x008C0005,
    0x008C0050, 0x008C0008, 0x008C000A, 0x008C0009, 0x008C000F, 0x008C0010,
    0x008C0012, 0x008C0013, 0x008C0017, 0x008C0018, 0x00000000
};
f32 lbl_1_data_484[5] = { 1.0f, 0.9f, 0.8f, 0.6f, 0.4f };
f32 lbl_1_data_498 = 525.0f;
s16 (*lbl_1_data_49C[3])(s16) = { fn_1_6048, fn_1_67E0, fn_1_71B4 };

s32 lbl_1_bss_15E0[5];
M549State *lbl_1_bss_15F4;

/* ---- game.c part G1: fn_1_3B74 .. fn_1_71B4 ---- */

void fn_1_3B74(M549State *state)
{
    MGACTOR_PARAM param;
    s16 i;
    s16 j;

    memset(&lbl_1_bss_580, 0, sizeof(lbl_1_bss_580));
    memset(lbl_1_bss_3B0, 0, sizeof(lbl_1_bss_3B0));
    memset(&lbl_1_bss_0, 0, sizeof(lbl_1_bss_0));
    memset(lbl_1_bss_158, 0, sizeof(lbl_1_bss_158));
    memset(lbl_1_bss_18, 0, sizeof(lbl_1_bss_18));
    lbl_1_bss_15F4 = state;
    for (i = 0; i < 11; i++) {
        lbl_1_bss_580.sfxFlag[i] = -1;
    }
    lbl_1_bss_3B0[0].s62 = 100;
    lbl_1_bss_3B0[1].s62 = 100;
    for (i = 0, j = 0; i <= 2; i++, j++) {
        lbl_1_bss_15F4->models[i].id = Hu3DModelCreateData(lbl_1_bss_15F4->dataNums[j]);
        Hu3DModelAttrSet(lbl_1_bss_15F4->models[i].id, 1);
        Hu3DModelCameraSet(lbl_1_bss_15F4->models[i].id, 1);
        Hu3DModelLayerSet(lbl_1_bss_15F4->models[i].id, 2);
        lbl_1_bss_15F4->models[i].x = 0.0f;
        lbl_1_bss_15F4->models[i].y = 0.0f;
        lbl_1_bss_15F4->models[i].z = 0.0f;
        lbl_1_bss_15F4->models[i].rotX = 0.0f;
        lbl_1_bss_15F4->models[i].rotY = 0.0f;
        lbl_1_bss_15F4->models[i].rotZ = 0.0f;
        Hu3DModelAttrReset(lbl_1_bss_15F4->models[i].id, 1);
        if (i == 1) {
            lbl_1_bss_15F4->models[i].rotX = 0.0f;
            lbl_1_bss_15F4->models[i].rotY = 22.5f;
            lbl_1_bss_15F4->models[i].rotZ = 0.0f;
            Hu3DModelAttrSet(lbl_1_bss_15F4->models[i].id, HU3D_MOTATTR_LOOP);
            Hu3DMotionSpeedSet(lbl_1_bss_15F4->models[i].id, 0.25f);
            Hu3DMotionTimeSet(lbl_1_bss_15F4->models[i].id, 0.0f);
        }
        if (i == 2) {
            Hu3DModelShadowMapSet(lbl_1_bss_15F4->models[i].id);
            Hu3DModelAttrSet(lbl_1_bss_15F4->models[i].id, HU3D_MOTATTR_LOOP);
            Hu3DMotionSpeedSet(lbl_1_bss_15F4->models[i].id, 0.25f);
            Hu3DMotionTimeSet(lbl_1_bss_15F4->models[i].id, 0.0f);
            Hu3DModelScaleSet(lbl_1_bss_15F4->models[i].id, 0.85f, 0.85f, 0.85f);
            Hu3DModelLayerSet(lbl_1_bss_15F4->models[i].id, 0);
        }
    }
    for (i = 3, j = 3; i <= 4; i++, j++) {
        lbl_1_bss_15F4->models[i].id = Hu3DModelCreateData(lbl_1_bss_15F4->dataNums[j]);
        Hu3DModelAttrSet(lbl_1_bss_15F4->models[i].id, 1);
        Hu3DModelCameraSet(lbl_1_bss_15F4->models[i].id, 1);
        Hu3DModelLayerSet(lbl_1_bss_15F4->models[i].id, 0);
        lbl_1_bss_15F4->models[i].x = 0.0f;
        lbl_1_bss_15F4->models[i].y = 0.0f;
        lbl_1_bss_15F4->models[i].z = 0.0f;
        lbl_1_bss_15F4->models[i].rotX = 0.0f;
        lbl_1_bss_15F4->models[i].rotY = 0.0f;
        lbl_1_bss_15F4->models[i].rotZ = 0.0f;
        Hu3DModelAttrReset(lbl_1_bss_15F4->models[i].id, 1);
        Hu3DModelAttrReset(lbl_1_bss_15F4->models[i].id, HU3D_MOTATTR_LOOP);
        Hu3DMotionTimeSet(lbl_1_bss_15F4->models[i].id, 0.0f);
        Hu3DMotionSpeedSet(lbl_1_bss_15F4->models[i].id, 0.0f);
    }
    for (i = 5; i <= 14; i++) {
        lbl_1_bss_15F4->models[i].id = Hu3DModelCreateData(lbl_1_bss_15F4->dataNums[5]);
        Hu3DModelAttrSet(lbl_1_bss_15F4->models[i].id, 1);
        Hu3DModelCameraSet(lbl_1_bss_15F4->models[i].id, 2);
        Hu3DModelLayerSet(lbl_1_bss_15F4->models[i].id, 1);
        lbl_1_bss_15F4->models[i].x = 0.0f;
        lbl_1_bss_15F4->models[i].y = 0.0f;
        lbl_1_bss_15F4->models[i].z = 0.0f;
        lbl_1_bss_15F4->models[i].rotX = 0.0f;
        lbl_1_bss_15F4->models[i].rotY = 0.0f;
        lbl_1_bss_15F4->models[i].rotZ = 0.0f;
    }
    for (i = 26; i <= 35; i++) {
        lbl_1_bss_15F4->models[i].id = Hu3DModelCreateData(lbl_1_bss_15F4->dataNums[8]);
        Hu3DModelAttrSet(lbl_1_bss_15F4->models[i].id, 1);
        Hu3DModelCameraSet(lbl_1_bss_15F4->models[i].id, 1);
        Hu3DModelLayerSet(lbl_1_bss_15F4->models[i].id, 1);
        lbl_1_bss_15F4->models[i].x = 0.0f;
        lbl_1_bss_15F4->models[i].y = 0.0f;
        lbl_1_bss_15F4->models[i].z = 0.0f;
        lbl_1_bss_15F4->models[i].rotX = 0.0f;
        lbl_1_bss_15F4->models[i].rotY = 0.0f;
        lbl_1_bss_15F4->models[i].rotZ = 0.0f;
    }
    for (i = 15; i <= 24; i++) {
        lbl_1_bss_15F4->models[i].id = Hu3DModelCreateData(lbl_1_bss_15F4->dataNums[6]);
        Hu3DModelAttrSet(lbl_1_bss_15F4->models[i].id, 1);
        Hu3DModelCameraSet(lbl_1_bss_15F4->models[i].id, 1);
        Hu3DModelLayerSet(lbl_1_bss_15F4->models[i].id, 3);
        lbl_1_bss_15F4->models[i].x = 0.0f;
        lbl_1_bss_15F4->models[i].y = 0.0f;
        lbl_1_bss_15F4->models[i].z = 0.0f;
        lbl_1_bss_15F4->models[i].rotX = 0.0f;
        lbl_1_bss_15F4->models[i].rotY = 0.0f;
        lbl_1_bss_15F4->models[i].rotZ = 0.0f;
        Hu3DModelScaleSet(lbl_1_bss_15F4->models[i].id, 1.0f, 1.0f, 1.0f);
        Hu3DModelAttrReset(lbl_1_bss_15F4->models[i].id, HU3D_MOTATTR_LOOP);
        Hu3DMotionTimeSet(lbl_1_bss_15F4->models[i].id, 0.0f);
        Hu3DMotionSpeedSet(lbl_1_bss_15F4->models[i].id, 0.0f);
    }
    lbl_1_bss_15F4->models[25].id = Hu3DModelCreateData(lbl_1_bss_15F4->dataNums[7]);
    Hu3DModelAttrSet(lbl_1_bss_15F4->models[25].id, 1);
    Hu3DModelCameraSet(lbl_1_bss_15F4->models[25].id, 1);
    lbl_1_bss_15F4->models[25].x = 0.0f;
    lbl_1_bss_15F4->models[25].y = -15.0f;
    lbl_1_bss_15F4->models[25].z = 0.0f;
    lbl_1_bss_15F4->models[25].rotX = 0.0f;
    lbl_1_bss_15F4->models[25].rotY = 0.0f;
    lbl_1_bss_15F4->models[25].rotZ = 0.0f;
    lbl_1_bss_0.unk0 = lbl_1_bss_15F4->models[25].id;
    MgActorColMapInit(&lbl_1_bss_0.unk0, 1, 10);
    param.height = 150.0f;
    param.radius = 45.0f;
    param.param = 0;
    param.type = 0;
    param.attr = 0x8000;
    param.narrowHook = fn_1_AD78;
    param.correctHookParam = 0;
    param.correctHook = fn_1_ADC8;
    lbl_1_bss_0.player[0] = MgPlayerCreate(lbl_1_bss_15F4->chars[0].playerNo, &param, 1, 1, -0x3F, lbl_1_data_440);
    lbl_1_bss_0.player[0]->actor->pos.x = -400.0f;
    lbl_1_bss_0.player[0]->actor->pos.y = 120.0f;
    lbl_1_bss_0.player[0]->actor->pos.z = 0.0f;
    lbl_1_bss_0.player[0]->actor->rotY = 90.0f;
    Hu3DModelLayerSet(lbl_1_bss_0.player[0]->actor->mdlId, 0);
    Hu3DModelShadowSet(lbl_1_bss_0.player[0]->actor->mdlId);
    Hu3DModelShadowDispOn(lbl_1_bss_0.player[0]->actor->mdlId);
    lbl_1_bss_15F4->models[68].id = lbl_1_bss_0.player[0]->actor->mdlId;
    param.param = 1;
    lbl_1_bss_0.player[1] = MgPlayerCreate(lbl_1_bss_15F4->chars[1].playerNo, &param, 1, 1, -0x3F, lbl_1_data_440);
    lbl_1_bss_0.player[1]->actor->pos.x = 400.0f;
    lbl_1_bss_0.player[1]->actor->pos.y = 120.0f;
    lbl_1_bss_0.player[1]->actor->pos.z = 0.0f;
    lbl_1_bss_0.player[1]->actor->rotY = -90.0f;
    Hu3DModelLayerSet(lbl_1_bss_0.player[1]->actor->mdlId, 0);
    Hu3DModelShadowSet(lbl_1_bss_0.player[1]->actor->mdlId);
    Hu3DModelShadowDispOn(lbl_1_bss_0.player[1]->actor->mdlId);
    lbl_1_bss_15F4->models[69].id = lbl_1_bss_0.player[1]->actor->mdlId;
    CharModelStepFxSet(lbl_1_bss_15F4->chars[0].charNo, 2);
    CharModelStepFxSet(lbl_1_bss_15F4->chars[1].charNo, 2);
    CharEffectLayerSet(0);
    lbl_1_bss_3B0[0].hook = CharModelItemHookGet(lbl_1_bss_15F4->chars[0].charNo, 1, 0);
    Hu3DModelHookSet(lbl_1_bss_15F4->models[68].id, lbl_1_bss_3B0[0].hook, lbl_1_bss_15F4->models[3].id);
    lbl_1_bss_3B0[1].hook = CharModelItemHookGet(lbl_1_bss_15F4->chars[1].charNo, 1, 0);
    Hu3DModelHookSet(lbl_1_bss_15F4->models[69].id, lbl_1_bss_3B0[1].hook, lbl_1_bss_15F4->models[4].id);
    fn_1_BF34(0);
    fn_1_BF34(1);
    fn_1_128AC();
    fn_1_23FC();
    fn_1_5CD4();
    lbl_1_bss_580.mgTimer = fn_1_1AEC(0, 60, 0, 292.0f, 64.0f);
}

s32 fn_1_4EE0(M549State *state)
{
    HU3DMODEL *modelP;
    s32 ret = 1;

    fn_1_3B74(state);
    MgPlayerPadSet(lbl_1_bss_0.player[0], 0, 0, 0, 0);
    MgPlayerPadSet(lbl_1_bss_0.player[1], 0, 0, 0, 0);
    MgActorExec();
    modelP = &Hu3DData[lbl_1_bss_15F4->models[68].id];
    lbl_1_bss_3B0[0].pos = modelP->pos;
    modelP = &Hu3DData[lbl_1_bss_15F4->models[69].id];
    lbl_1_bss_3B0[1].pos = modelP->pos;
    fn_1_115A8(state);
    return ret;
}

s32 fn_1_4FFC(void)
{
    s32 ret = 0;
    s32 i;

    for (i = 0; i < 2; i++) {
        lbl_1_bss_15F4->chars[i].btn = 0;
        lbl_1_bss_15F4->chars[i].btnDown = 0;
    }
    ret = (s16)fn_1_A18C();
    fn_1_762C(0);
    fn_1_762C(1);
    fn_1_8570();
    fn_1_98F0();
    fn_1_ADDC();
    fn_1_5CD4();
    return ret;
}

s32 fn_1_50A4(void)
{
    s32 ret = 0;
    s32 i;

    lbl_1_bss_580.mode = 0;
    lbl_1_bss_580.s02 = 0;
    lbl_1_bss_580.s04 = 0;
    lbl_1_bss_580.timer = 0;
    for (i = 0; i < 2; i++) {
        lbl_1_bss_15F4->chars[i].btn = 0;
        lbl_1_bss_15F4->chars[i].btnDown = 0;
    }
    fn_1_762C(0);
    fn_1_762C(1);
    fn_1_8570();
    fn_1_98F0();
    fn_1_ADDC();
    fn_1_5CD4();
    return ret;
}

s32 fn_1_5184(void)
{
    HuVecF pos;
    s16 unused;
    s16 i;
    s32 playerNo;
    s32 ret;

    ret = 0;
    unused = 0;
    switch (lbl_1_bss_580.mode) {
        case 0:
            fn_1_1B60(lbl_1_bss_580.mgTimer);
            lbl_1_bss_580.mode++;
        case 1:
            for (i = 0; i < 2; i++) {
                if (lbl_1_bss_580.s20 == 0 && lbl_1_bss_3B0[i].s6E != 0) {
                    fn_1_C360(i);
                    MgPlayerPadSet(lbl_1_bss_0.player[i], lbl_1_bss_3B0[i].btnC, lbl_1_bss_3B0[i].btnD,
                        lbl_1_bss_3B0[i].btnB, lbl_1_bss_3B0[i].btnA);
                    lbl_1_bss_15F4->chars[i].btn = lbl_1_bss_3B0[i].btnA;
                    lbl_1_bss_15F4->chars[i].btnDown = lbl_1_bss_3B0[i].btnB;
                }
                fn_1_762C(i);
            }
            if (lbl_1_bss_580.s20 != 0) {
                lbl_1_bss_580.mode++;
            }
            break;
        case 2:
            fn_1_1BB4(lbl_1_bss_580.mgTimer);
            if (lbl_1_bss_580.s24 != 0) {
                MgSeqWinnerSet(lbl_1_bss_15F4->chars[lbl_1_bss_580.s24 & 1].charNo, -1, -1, -1);
                playerNo = lbl_1_bss_15F4->chars[lbl_1_bss_580.s24 & 1].playerNo;
                if (_CheckFlag(FLAG_MG_PRACTICE) == 0) {
                    GwPlayer[playerNo].mgCoinBonus = 10;
                }
            } else {
                MgSeqWinnerSet(-1, -1, -1, -1);
            }
            fn_1_35DC(1, 100);
            ret = 1;
            break;
    }
    for (i = 0; i < 2; i++) {
        if (lbl_1_bss_3B0[i].s28 != 0) {
            MgPlayerPadSet(lbl_1_bss_0.player[i], 0, 0, 0, 0);
        }
    }
    MgActorPosGet(lbl_1_bss_0.player[0]->actor, &pos);
    lbl_1_bss_3B0[0].prevPos.x = pos.x;
    lbl_1_bss_3B0[0].prevPos.y = pos.y;
    lbl_1_bss_3B0[0].prevPos.z = pos.z;
    MgActorPosGet(lbl_1_bss_0.player[1]->actor, &pos);
    lbl_1_bss_3B0[1].prevPos.x = pos.x;
    lbl_1_bss_3B0[1].prevPos.y = pos.y;
    lbl_1_bss_3B0[1].prevPos.z = pos.z;
    MgActorExec();
    MgActorPosGet(lbl_1_bss_0.player[0]->actor, &pos);
    lbl_1_bss_3B0[0].pos.x = pos.x;
    lbl_1_bss_3B0[0].pos.y = pos.y;
    lbl_1_bss_3B0[0].pos.z = pos.z;
    MgActorPosGet(lbl_1_bss_0.player[1]->actor, &pos);
    lbl_1_bss_3B0[1].pos.x = pos.x;
    lbl_1_bss_3B0[1].pos.y = pos.y;
    lbl_1_bss_3B0[1].pos.z = pos.z;
    fn_1_8570();
    fn_1_98F0();
    fn_1_ADDC();
    pos.x = lbl_1_bss_3B0[0].pos.x;
    pos.y = lbl_1_bss_3B0[0].pos.y;
    pos.z = lbl_1_bss_3B0[0].pos.z;
    MgActorPosUpdate(lbl_1_bss_0.player[0]->actor, &pos);
    pos.x = lbl_1_bss_3B0[1].pos.x;
    pos.y = lbl_1_bss_3B0[1].pos.y;
    pos.z = lbl_1_bss_3B0[1].pos.z;
    MgActorPosUpdate(lbl_1_bss_0.player[1]->actor, &pos);
    fn_1_9DBC();
    fn_1_5CD4();
    return ret;
}

s32 fn_1_569C(void)
{
    s32 ret = 0;

    lbl_1_bss_580.mode = 0;
    lbl_1_bss_580.s02 = 0;
    lbl_1_bss_580.s04 = 0;
    lbl_1_bss_580.timer = 0;
    lbl_1_bss_3B0[0].sub = 0;
    lbl_1_bss_3B0[1].sub = 0;
    fn_1_762C(0);
    fn_1_762C(1);
    fn_1_8570();
    fn_1_98F0();
    MgPlayerPadSet(lbl_1_bss_0.player[0], 0, 0, 0, 0);
    MgPlayerPadSet(lbl_1_bss_0.player[1], 0, 0, 0, 0);
    MgActorExec();
    fn_1_5CD4();
    return ret;
}

s32 fn_1_5788(void)
{
    s32 ret = 0;

    ret = (s16)fn_1_A898();
    fn_1_762C(0);
    fn_1_762C(1);
    fn_1_8570();
    fn_1_98F0();
    fn_1_5CD4();
    return ret;
}

s32 fn_1_57D8(void)
{
    M549StageSlot *slot;
    s32 ret = 0;

    if (lbl_1_bss_15F4->doneFlag == 0) {
        if (lbl_1_bss_580.s24 != 0) {
            slot = &lbl_1_bss_15F4->slots[lbl_1_bss_580.s24 & 1];
            slot->motion = 3;
            slot->speed = 1.0f;
            slot->time = 0.0f;
            slot->attrSet = 0;
            slot->attrReset = HU3D_MOTATTR_LOOP;
        } else {
            slot = &lbl_1_bss_15F4->slots[0];
            slot->motion = 4;
            slot->speed = 1.0f;
            slot->time = 0.0f;
            slot->attrSet = 0;
            slot->attrReset = HU3D_MOTATTR_LOOP;
            slot = &lbl_1_bss_15F4->slots[1];
            slot->motion = 4;
            slot->speed = 1.0f;
            slot->time = 0.0f;
            slot->attrSet = 0;
            slot->attrReset = HU3D_MOTATTR_LOOP;
            if (lbl_1_bss_3B0[0].s14 == 0 && lbl_1_bss_3B0[1].s14 == 0) {
                CharLoseVoicePlay(lbl_1_bss_15F4->chars[0].charNo, lbl_1_bss_15F4->chars[1].charNo, -1, -1);
            }
        }
        lbl_1_bss_15F4->doneFlag = 1;
    }
    fn_1_762C(0);
    fn_1_762C(1);
    fn_1_8570();
    fn_1_98F0();
    fn_1_5CD4();
    if (lbl_1_bss_580.s24 != 0) {
        CharModelVoicePanSet(lbl_1_bss_15F4->chars[lbl_1_bss_580.s24 & 1].charNo, 64);
    }
    return ret;
}

s32 fn_1_5A00(void)
{
    s32 ret = 1;

    fn_1_762C(0);
    fn_1_762C(1);
    fn_1_5CD4();
    return ret;
}

s32 fn_1_5A40(void)
{
    s32 ret = 1;

    fn_1_3B54();
    MgActorKill(lbl_1_bss_0.player[0]->actor);
    MgActorKill(lbl_1_bss_0.player[1]->actor);
    return ret;
}

void fn_1_5A98(void)
{
    HU3DMODEL *model0;
    HU3DMODEL *model1;
    s16 i;
    s32 pan;
    f32 panF;

    model0 = &Hu3DData[lbl_1_bss_15F4->models[68].id];
    model1 = &Hu3DData[lbl_1_bss_15F4->models[69].id];
    for (i = 0; i < 68; i++) {
        Hu3DModelPosSet(lbl_1_bss_15F4->models[i].id, lbl_1_bss_15F4->models[i].x, lbl_1_bss_15F4->models[i].y,
            lbl_1_bss_15F4->models[i].z);
        Hu3DModelRotSet(lbl_1_bss_15F4->models[i].id, lbl_1_bss_15F4->models[i].rotX,
            lbl_1_bss_15F4->models[i].rotY, lbl_1_bss_15F4->models[i].rotZ);
    }
    panF = 0.053333335f * model0->pos.x;
    pan = 64.0f + panF;
    CharModelVoicePanSet(lbl_1_bss_15F4->chars[0].charNo, pan);
    panF = 0.053333335f * model1->pos.x;
    pan = 64.0f + panF;
    CharModelVoicePanSet(lbl_1_bss_15F4->chars[1].charNo, pan);
}

void fn_1_5CD4(void)
{
    HU3DMODEL *model0;
    HU3DMODEL *model1;
    s16 i;
    s32 pan;
    f32 panF;

    fontcolor = 0;
    Hu3DModelCameraSet(lbl_1_bss_15F4->models[51].id, 2);
    Hu3DModelAttrSet(lbl_1_bss_15F4->models[51].id, HU3D_ATTR_ZCMP_OFF);
    Hu3DModelLayerSet(lbl_1_bss_15F4->models[51].id, 7);
    lbl_1_bss_15F4->models[51].x = -1100.0f + lbl_1_bss_14->pos.x;
    lbl_1_bss_15F4->models[51].y = 1150.0f + (lbl_1_bss_14->pos.y - 953.0f);
    lbl_1_bss_15F4->models[51].z = -800.0f + (lbl_1_bss_14->pos.z - 1556.0f);
    model0 = &Hu3DData[lbl_1_bss_15F4->models[68].id];
    model1 = &Hu3DData[lbl_1_bss_15F4->models[69].id];
    for (i = 0; i < 68; i++) {
        Hu3DModelPosSet(lbl_1_bss_15F4->models[i].id, lbl_1_bss_15F4->models[i].x, lbl_1_bss_15F4->models[i].y,
            lbl_1_bss_15F4->models[i].z);
        Hu3DModelRotSet(lbl_1_bss_15F4->models[i].id, lbl_1_bss_15F4->models[i].rotX,
            lbl_1_bss_15F4->models[i].rotY, lbl_1_bss_15F4->models[i].rotZ);
    }
    panF = 0.053333335f * model0->pos.x;
    pan = 64.0f + panF;
    CharModelVoicePanSet(lbl_1_bss_15F4->chars[0].charNo, pan);
    panF = 0.053333335f * model1->pos.x;
    pan = 64.0f + panF;
    CharModelVoicePanSet(lbl_1_bss_15F4->chars[1].charNo, pan);
    fn_1_97C(0);
    fn_1_97C(1);
    fn_1_1124();
    fn_1_1524();
}

s16 fn_1_6048(s16 player)
{
    M549StageSlot *slot;
    s16 modelNo;
    s16 lvl;
    s16 pitch;
    s16 ret;
    s32 pan;
    f32 time;
    f32 panF;

    slot = &lbl_1_bss_15F4->slots[player];
    ret = 0;
    modelNo = player + 3;
    switch (lbl_1_bss_3B0[player].state) {
        case 0:
            if (lbl_1_bss_580.s22 == 0) {
                time = Hu3DMotionTimeGet(lbl_1_bss_15F4->models[modelNo].id);
                if (time == 0.0f
                    && (lbl_1_bss_15F4->chars[player].btnDown & 0x100) != 0 && lbl_1_bss_3B0[player].s12 == 0) {
                    Hu3DModelAttrReset(lbl_1_bss_15F4->models[modelNo].id, HU3D_MOTATTR_REV);
                    Hu3DMotionTimeSet(lbl_1_bss_15F4->models[modelNo].id, 0.0f);
                    Hu3DMotionSpeedSet(lbl_1_bss_15F4->models[modelNo].id, 0.24166666f);
                    panF = 0.026666667f * lbl_1_bss_15F4->models[player + 68].x;
                    pan = 64.0f + panF;
                    fn_1_3754(player, 0x7F, pan);
                    fn_1_38A4(player, -0x1F40);
                    lbl_1_bss_3B0[player].s5C = 1;
                    lbl_1_bss_3B0[player].state++;
                }
            }
            break;
        case 1:
            if (lbl_1_bss_580.s22 == 0) {
                if ((lbl_1_bss_15F4->chars[player].btn & 0x100) != 0) {
                    if (++lbl_1_bss_3B0[player].s5C >= 0x78) {
                        Hu3DMotionTimeSet(lbl_1_bss_15F4->models[modelNo].id, 29.0f);
                        Hu3DMotionSpeedSet(lbl_1_bss_15F4->models[modelNo].id, 0.0f);
                        lbl_1_bss_3B0[player].s5C = 0x78;
                    } else {
                        Hu3DMotionSpeedSet(lbl_1_bss_15F4->models[modelNo].id, 0.24166666f);
                    }
                    panF = 0.026666667f * lbl_1_bss_3B0[player].pos.x;
                    pan = 64.0f + panF;
                    fn_1_3814(player, pan);
                    pitch = lbl_1_bss_3B0[player].s5C * 0x85 - 0x1F40;
                    fn_1_38A4(player, pitch);
                } else {
                    Hu3DModelAttrSet(lbl_1_bss_15F4->models[modelNo].id, HU3D_MOTATTR_REV);
                    Hu3DMotionSpeedSet(lbl_1_bss_15F4->models[modelNo].id, 1.5f);
                    slot->motion = 6;
                    slot->speed = 1.0f;
                    slot->time = 0.0f;
                    slot->attrSet = 0;
                    slot->attrReset = HU3D_MOTATTR_LOOP;
                    fn_1_7CC0(player);
                    lvl = lbl_1_bss_3B0[player].s5C / 30;
                    switch (lvl) {
                        case 0:
                        case 1:
                            lvl = 0;
                            break;
                        case 2:
                        case 3:
                            lvl = 1;
                            break;
                        case 4:
                            lvl = 2;
                            break;
                    }
                    lvl += 2;
                    panF = 0.026666667f * lbl_1_bss_3B0[player].pos.x;
                    pan = 64.0f + panF;
                    fn_1_3754(lvl, 0x7F, pan);
                    fn_1_39C4(player);
                    lbl_1_bss_3B0[player].s12 = 10;
                    lbl_1_bss_3B0[player].s5C = 0;
                    lbl_1_bss_3B0[player].s28 = 1;
                    lbl_1_bss_3B0[player].state++;
                }
            } else {
                Hu3DModelAttrSet(lbl_1_bss_15F4->models[modelNo].id, HU3D_MOTATTR_REV);
                Hu3DMotionSpeedSet(lbl_1_bss_15F4->models[modelNo].id, 1.5f);
                fn_1_39C4(player);
                lbl_1_bss_3B0[player].state = 3;
            }
            break;
        case 2:
            if (slot->endFlag != 0 || slot->time >= 20.0f) {
                Hu3DModelAttrReset(lbl_1_bss_15F4->models[modelNo].id, HU3D_MOTATTR_REV);
                Hu3DMotionSpeedSet(lbl_1_bss_15F4->models[modelNo].id, 0.0f);
                slot->motion = 5;
                slot->speed = 1.0f;
                slot->time = 0.0f;
                slot->attrSet = HU3D_MOTATTR_LOOP;
                slot->attrReset = 0;
                lbl_1_bss_3B0[player].state++;
            }
            break;
        case 3:
            if (lbl_1_bss_580.s22 == 0) {
                lbl_1_bss_3B0[player].s28 = 0;
            }
            lbl_1_bss_3B0[player].state = 0;
            break;
    }
    ret = 0;
    return ret;
}

s16 fn_1_67E0(s16 player)
{
    M549StageSlot *slot;
    s32 pan;
    f32 panF;
    f32 speed;
    f32 diff;
    f32 angle;
    f32 rotY;
    s16 ret;
    s16 modelNo;
    s16 motion;
    s16 vol;

    slot = &lbl_1_bss_15F4->slots[player];
    ret = 0;
    modelNo = player + 3;
    switch (lbl_1_bss_3B0[player].state2) {
        case 0:
            speed = 2.5f + 2.5f * lbl_1_bss_3B0[player].s68;
            lbl_1_bss_3B0[player].f44 = speed * sin((3.141592653589793 * lbl_1_bss_3B0[player].f64) / 180.0);
            lbl_1_bss_3B0[player].f4C = speed * cos((3.141592653589793 * lbl_1_bss_3B0[player].f64) / 180.0);
            angle = lbl_1_bss_3B0[player].f64;
            rotY = lbl_1_bss_0.player[player]->actor->rotY;
            diff = angle - rotY;
            if (diff >= -90.0f && diff <= 90.0f) {
                motion = 8;
            } else {
                motion = 7;
            }
            slot->shiftState = 1;
            slot->shiftMotion = motion;
            slot->shiftStart = 0.0f;
            slot->shiftEnd = 4.0f;
            slot->shiftAttr = 0;
            slot->attrReset = HU3D_MOTATTR_LOOP;
            Hu3DMotionSpeedSet(lbl_1_bss_15F4->models[modelNo].id, 0.0f);
            if (lbl_1_bss_3B0[player].s6A != 0) {
                switch (lbl_1_bss_3B0[player].s68) {
                    case 0:
                        omVibrate(lbl_1_bss_15F4->chars[player].playerNo, 20, 6, 6);
                        break;
                    case 1:
                        omVibrate(lbl_1_bss_15F4->chars[player].playerNo, 30, 6, 6);
                        break;
                    case 2:
                        omVibrate(lbl_1_bss_15F4->chars[player].playerNo, 20, 4, 2);
                        break;
                    case 3:
                        omVibrate(lbl_1_bss_15F4->chars[player].playerNo, 30, 4, 2);
                        break;
                    case 4:
                        omVibrate(lbl_1_bss_15F4->chars[player].playerNo, 30, 12, 0);
                        break;
                }
            }
            switch (lbl_1_bss_3B0[player].s68) {
                case 0:
                    vol = 0x54;
                    break;
                case 1:
                    vol = 0x60;
                    break;
                case 2:
                    vol = 0x6C;
                    break;
                case 3:
                    vol = 0x78;
                    break;
                case 4:
                    vol = 0x7F;
                    break;
            }
            panF = 0.026666667f * lbl_1_bss_3B0[player].pos.x;
            pan = 64.0f + panF;
            fn_1_3754(8, vol, pan);
            lbl_1_bss_3B0[player].s0A = 0;
            lbl_1_bss_3B0[player].state2++;
        case 1:
            if (slot->shiftState == 3) {
                slot->speed = 1.5f;
                lbl_1_bss_3B0[player].state2++;
            }
            break;
        case 2:
            lbl_1_bss_3B0[player].pos.x += lbl_1_bss_3B0[player].f44;
            lbl_1_bss_3B0[player].pos.z += lbl_1_bss_3B0[player].f4C;
            MgActorPosUpdate(lbl_1_bss_0.player[player]->actor, &lbl_1_bss_3B0[player].pos);
            if (slot->endFlag != 0) {
                lbl_1_bss_3B0[player].moveSave = lbl_1_bss_3B0[player].move;
                lbl_1_bss_3B0[player].f44 = 0.0f;
                lbl_1_bss_3B0[player].f4C = 0.0f;
                slot->motion = 9;
                slot->speed = 1.0f;
                slot->time = 0.0f;
                slot->attrSet = 0;
                slot->attrReset = HU3D_MOTATTR_LOOP;
                lbl_1_bss_3B0[player].state2++;
            }
            break;
        case 3:
            if (slot->endFlag != 0) {
                slot->motion = 5;
                slot->speed = 1.0f;
                slot->time = 0.0f;
                slot->attrSet = HU3D_MOTATTR_LOOP;
                slot->attrReset = 0;
                lbl_1_bss_3B0[player].state2++;
            }
            break;
        case 4:
            lbl_1_bss_3B0[player].state2 = 0;
            ret = 1;
            break;
    }
    if (ret == 0) {
        if ((Hu3DModelAttrGet(lbl_1_bss_15F4->models[player + 68].id) & HU3D_ATTR_DISPOFF) != 0) {
            Hu3DModelAttrReset(lbl_1_bss_15F4->models[player + 68].id, HU3D_ATTR_DISPOFF);
        } else {
            Hu3DModelAttrSet(lbl_1_bss_15F4->models[player + 68].id, HU3D_ATTR_DISPOFF);
        }
    }
    if (lbl_1_bss_3B0[player].state == 1) {
        if ((lbl_1_bss_15F4->chars[player].btn & 0x100) != 0) {
            panF = 0.026666667f * lbl_1_bss_3B0[player].pos.x;
            pan = 64.0f + panF;
            fn_1_3814(player, pan);
        } else {
            fn_1_39C4(player);
            Hu3DModelAttrSet(lbl_1_bss_15F4->models[modelNo].id, HU3D_MOTATTR_REV);
            Hu3DMotionSpeedSet(lbl_1_bss_15F4->models[modelNo].id, 1.5f);
            lbl_1_bss_3B0[player].s5C = 0;
            lbl_1_bss_3B0[player].state = 0;
        }
    } else if (lbl_1_bss_3B0[player].state == 0) {
        Hu3DModelAttrSet(lbl_1_bss_15F4->models[modelNo].id, HU3D_MOTATTR_REV);
        Hu3DMotionSpeedSet(lbl_1_bss_15F4->models[modelNo].id, 1.5f);
    }
    return ret;
}

s16 fn_1_71B4(s16 player)
{
    HU3DMODEL *modelP;
    M549StageSlot *slot;
    s16 pan;
    s16 ret;
    f32 panF;

    modelP = &Hu3DData[lbl_1_bss_15F4->models[player + 68].id];
    slot = &lbl_1_bss_15F4->slots[player];
    ret = 0;
    switch (lbl_1_bss_3B0[player].state3) {
        case 0:
            panF = 0.026666667f * lbl_1_bss_3B0[player].pos.x;
            pan = 64.0f + panF;
            if (pan > 0x50) {
                pan = 0x50;
            } else if (pan < 0x30) {
                pan = 0x30;
            }
            fn_1_3754(9, 0x7F, pan);
            if (lbl_1_bss_3B0[player].s5C != 0) {
                fn_1_39C4(player);
            }
            lbl_1_bss_3B0[player].state3++;
        case 1:
            Hu3DModelAttrReset(lbl_1_bss_15F4->models[player + 68].id, 1);
            modelP->pos.x += lbl_1_bss_3B0[player].f44;
            modelP->pos.z += lbl_1_bss_3B0[player].f4C;
            MgPlayerPauseOn(lbl_1_bss_0.player[player]);
            lbl_1_bss_3B0[player].f48 = 0.0f;
            lbl_1_bss_3B0[player].state3++;
            break;
        case 2:
            modelP->pos.x += lbl_1_bss_3B0[player].f44;
            modelP->pos.z += lbl_1_bss_3B0[player].f4C;
            slot->shiftState = 1;
            slot->shiftMotion = 2;
            slot->shiftStart = 0.0f;
            slot->shiftEnd = 16.0f;
            slot->shiftAttr = HU3D_MOTATTR_LOOP;
            slot->attrReset = 0;
            lbl_1_bss_3B0[player].state3++;
            break;
        case 3:
            modelP->pos.x += lbl_1_bss_3B0[player].f44;
            modelP->pos.z += lbl_1_bss_3B0[player].f4C;
            if (lbl_1_bss_3B0[player].f48 >= 20.0f && lbl_1_bss_3B0[player].f48 <= 30.0f) {
                Hu3DModelShadowDispOff(lbl_1_bss_0.player[player]->actor->mdlId);
            }
            lbl_1_bss_3B0[player].f48 += 0.5f;
            modelP->pos.y -= lbl_1_bss_3B0[player].f48;
            if (modelP->pos.y <= -4000.0f) {
                lbl_1_bss_3B0[player].state3++;
            }
            break;
        case 4:
            break;
    }
    return ret;
}

/* game.c part G2: fn_1_762C .. fn_1_A18C */
#include "math.h"

/* model table entry position as a vector (x, y, z are contiguous) */
#define M549G2_MDLPOS(idx) ((HuVecF *)&lbl_1_bss_15F4->models[idx].x)

s32 fn_1_762C(s16 arg0)
{
    s32 ret;
    s16 done;
    s8 stkX;
    s8 stkY;
    f32 x;
    f32 y;

    ret = 0;
    done = 0;
    if (lbl_1_bss_3B0[arg0].s62 > 20 && (lbl_1_bss_3B0[arg0].s6A != 0 || lbl_1_bss_3B0[arg0].s6C != 0)) {
        lbl_1_bss_3B0[arg0].s28 = 1;
        if (lbl_1_bss_3B0[arg0].state != 1) {
            lbl_1_bss_3B0[arg0].state = 0;
        }
        lbl_1_bss_3B0[arg0].mode = 1;
    }
    done = lbl_1_data_49C[lbl_1_bss_3B0[arg0].mode](arg0);
    if (done != 0 && (lbl_1_bss_3B0[arg0].s6A != 0 || lbl_1_bss_3B0[arg0].s6C != 0)) {
        if (lbl_1_bss_580.s22 == 0) {
            lbl_1_bss_3B0[arg0].s28 = 0;
        }
        lbl_1_bss_3B0[arg0].s62 = 0;
        lbl_1_bss_3B0[arg0].mode = 0;
    }
    if (++lbl_1_bss_3B0[arg0].s62 <= 20 && lbl_1_bss_3B0[arg0].mode == 0) {
        if (Hu3DModelAttrGet(lbl_1_bss_15F4->models[arg0 + 0x44].id) & 1) {
            Hu3DModelAttrReset(lbl_1_bss_15F4->models[arg0 + 0x44].id, 1);
        } else {
            Hu3DModelAttrSet(lbl_1_bss_15F4->models[arg0 + 0x44].id, 1);
        }
    } else if (lbl_1_bss_3B0[arg0].mode == 0 && (lbl_1_bss_3B0[arg0].s6A != 0 || lbl_1_bss_3B0[arg0].s6C != 0)) {
        lbl_1_bss_3B0[arg0].s6A = 0;
        lbl_1_bss_3B0[arg0].s6C = 0;
        Hu3DModelAttrReset(lbl_1_bss_15F4->models[arg0 + 0x44].id, 1);
    } else {
        lbl_1_bss_3B0[arg0].s62 = 0x78;
    }
    if (lbl_1_bss_3B0[arg0].s12-- <= 0) {
        lbl_1_bss_3B0[arg0].s12 = 0;
    }
    lbl_1_bss_3B0[arg0].s60 = lbl_1_bss_3B0[arg0].s5C / 30;
    if (lbl_1_bss_3B0[arg0].s6E == 0) {
        stkX = HuPadStkX[lbl_1_bss_0.player[arg0]->padNo];
        stkY = HuPadStkY[lbl_1_bss_0.player[arg0]->padNo];
        x = stkX * lbl_1_data_484[lbl_1_bss_3B0[arg0].s60];
        y = stkY * lbl_1_data_484[lbl_1_bss_3B0[arg0].s60];
        lbl_1_bss_3B0[arg0].velX = x;
        lbl_1_bss_3B0[arg0].velZ = y;
        HuPadStkX[lbl_1_bss_0.player[arg0]->padNo] = (s8)x;
        HuPadStkY[lbl_1_bss_0.player[arg0]->padNo] = (s8)y;
    }
    if (lbl_1_bss_3B0[arg0].mode == 2) {
        fn_1_99E8(arg0);
    } else if (lbl_1_bss_580.s20 != 0) {
        fn_1_99E8(arg0);
    }
    return ret;
}

void fn_1_7CC0(s16 arg0)
{
    HuVecF vec;
    f32 scale;
    s16 i;
    s16 shadow;
    s16 body;

    for (i = 0; i < 10; i++) {
        if (lbl_1_bss_158[i].active == 0) {
            body = i + 5;
            lbl_1_bss_158[i].active = 1;
            lbl_1_bss_158[i].s04 = 0;
            lbl_1_bss_158[i].s02 = 0;
            lbl_1_bss_158[i].s06 = 0;
            lbl_1_bss_158[i].side = arg0;
            lbl_1_bss_158[i].s0A = 0;
            lbl_1_bss_158[i].angle = lbl_1_bss_0.player[arg0]->actor->rotY;
            lbl_1_bss_158[i].level = lbl_1_bss_3B0[arg0].s5C / 30;
            lbl_1_bss_158[i].s12 = lbl_1_bss_3B0[arg0].s5C;
            lbl_1_bss_158[i].radius = 50.0f + 10.0f * lbl_1_bss_158[i].level;
            lbl_1_bss_158[i].size = 12.5f;
            lbl_1_bss_158[i].f1C = 0.0f;
            lbl_1_bss_158[i].f20 = 0.05f + 0.05f * lbl_1_bss_158[i].level;
            lbl_1_bss_158[i].dir.x = (15.0f + 5.0f * lbl_1_bss_158[i].level) * sin(M_PI * lbl_1_bss_158[i].angle / 180.0);
            lbl_1_bss_158[i].dir.z = (15.0f + 5.0f * lbl_1_bss_158[i].level) * cos(M_PI * lbl_1_bss_158[i].angle / 180.0);
            Hu3DModelObjPosGet(lbl_1_bss_15F4->models[arg0 + 0x44].id, lbl_1_bss_3B0[arg0].hook, &vec);
            lbl_1_bss_158[i].pos.x = vec.x + 40.0 * sin(M_PI * lbl_1_bss_158[i].angle / 180.0);
            lbl_1_bss_158[i].pos.y = 20.0f + vec.y;
            lbl_1_bss_158[i].pos.z = vec.z + 40.0 * cos(M_PI * lbl_1_bss_158[i].angle / 180.0);
            *M549G2_MDLPOS(body) = lbl_1_bss_158[i].pos;
            lbl_1_bss_15F4->models[body].rotY = 90.0f + lbl_1_bss_158[i].angle;
            scale = lbl_1_bss_158[i].size / 50.0f;
            Hu3DModelScaleSet(lbl_1_bss_15F4->models[body].id, scale, scale, scale);
            Hu3DModelAttrReset(lbl_1_bss_15F4->models[body].id, 1);
            shadow = i + 0x1A;
            *M549G2_MDLPOS(shadow) = lbl_1_bss_158[i].pos;
            lbl_1_bss_15F4->models[shadow].y = 130.0f;
            lbl_1_bss_15F4->models[shadow].rotY = lbl_1_bss_158[i].angle;
            Hu3DMotionTimeSet(lbl_1_bss_15F4->models[shadow].id, 0.0f);
            Hu3DMotionSpeedSet(lbl_1_bss_15F4->models[shadow].id, 1.0f + 0.25f * lbl_1_bss_158[i].level);
            Hu3DModelAttrReset(lbl_1_bss_15F4->models[shadow].id, 1);
            Hu3DModelScaleSet(lbl_1_bss_15F4->models[shadow].id, 1.0f, 1.0f, lbl_1_bss_158[i].f1C);
            return;
        }
    }
}

void fn_1_8570(void)
{
    f32 dist;
    f32 scale;
    f32 time;
    s16 i;
    s16 shadow;
    s16 body;

    for (i = 0; i < 10; i++) {
        if (lbl_1_bss_158[i].active != 0) {
            shadow = i + 0x1A;
            body = i + 5;
            dist = sqrt(lbl_1_bss_158[i].pos.x * lbl_1_bss_158[i].pos.x + lbl_1_bss_158[i].pos.z * lbl_1_bss_158[i].pos.z);
            if (dist >= 645.0f && lbl_1_bss_158[i].s06 == 0) {
                lbl_1_bss_158[i].s06 = 1;
                lbl_1_bss_158[i].s04 = 2;
            }
            if (lbl_1_bss_158[i].s0A != 0 && lbl_1_bss_158[i].s06 == 0) {
                Hu3DModelAttrSet(lbl_1_bss_15F4->models[body].id, 1);
                lbl_1_bss_158[i].s06 = 1;
                lbl_1_bss_158[i].s02 = 2;
                lbl_1_bss_158[i].s04 = 4;
            }
            switch (lbl_1_bss_158[i].s02) {
                case 0:
                    lbl_1_bss_158[i].pos.x += lbl_1_bss_158[i].dir.x;
                    lbl_1_bss_158[i].pos.z += lbl_1_bss_158[i].dir.z;
                    lbl_1_bss_158[i].size += 10.0f;
                    if (lbl_1_bss_158[i].size >= lbl_1_bss_158[i].radius) {
                        lbl_1_bss_158[i].size = lbl_1_bss_158[i].radius;
                        lbl_1_bss_158[i].s02++;
                    }
                    scale = lbl_1_bss_158[i].size / 50.0f;
                    Hu3DModelScaleSet(lbl_1_bss_15F4->models[body].id, scale, scale, scale);
                    break;
                case 1:
                    lbl_1_bss_158[i].pos.x += lbl_1_bss_158[i].dir.x;
                    lbl_1_bss_158[i].pos.z += lbl_1_bss_158[i].dir.z;
                    if (dist >= 695.0f) {
                        scale = lbl_1_bss_158[i].size / 50.0f;
                        Hu3DModelScaleSet(lbl_1_bss_15F4->models[body].id, scale, scale, scale);
                        lbl_1_bss_158[i].size -= 1.5f;
                        if (lbl_1_bss_158[i].size <= 0.0f) {
                            Hu3DModelAttrSet(lbl_1_bss_15F4->models[body].id, 1);
                            lbl_1_bss_158[i].active = 0;
                            lbl_1_bss_158[i].size = 0.0f;
                        }
                    }
                    break;
            }
            *M549G2_MDLPOS(body) = lbl_1_bss_158[i].pos;
            switch (lbl_1_bss_158[i].s04) {
                case 0:
                    lbl_1_bss_15F4->models[shadow].x = lbl_1_bss_15F4->models[body].x;
                    lbl_1_bss_15F4->models[shadow].z = lbl_1_bss_15F4->models[body].z;
                    lbl_1_bss_158[i].f1C += lbl_1_bss_158[i].f20;
                    if (lbl_1_bss_158[i].f1C >= 1.0f) {
                        lbl_1_bss_158[i].f1C = 1.0f;
                    }
                    Hu3DModelScaleSet(lbl_1_bss_15F4->models[shadow].id, 1.0f, 1.0f, lbl_1_bss_158[i].f1C);
                    time = Hu3DMotionTimeGet(lbl_1_bss_15F4->models[shadow].id);
                    if (time >= 30.0f) {
                        lbl_1_bss_158[i].s04++;
                    }
                    break;
                case 1:
                    lbl_1_bss_15F4->models[shadow].x = lbl_1_bss_15F4->models[body].x;
                    lbl_1_bss_15F4->models[shadow].z = lbl_1_bss_15F4->models[body].z;
                    lbl_1_bss_158[i].f1C += lbl_1_bss_158[i].f20;
                    if (lbl_1_bss_158[i].f1C >= 1.0f) {
                        lbl_1_bss_158[i].f1C = 1.0f;
                    }
                    Hu3DModelScaleSet(lbl_1_bss_15F4->models[shadow].id, 1.0f, 1.0f, lbl_1_bss_158[i].f1C);
                    time = Hu3DMotionTimeGet(lbl_1_bss_15F4->models[shadow].id);
                    if (time >= 60.0f) {
                        Hu3DMotionTimeSet(lbl_1_bss_15F4->models[shadow].id, 30.0f);
                    }
                    break;
                case 2:
                    Hu3DMotionTimeSet(lbl_1_bss_15F4->models[shadow].id, 60.0f);
                    lbl_1_bss_158[i].s04++;
                case 3:
                    lbl_1_bss_15F4->models[shadow].x = lbl_1_bss_15F4->models[body].x;
                    lbl_1_bss_15F4->models[shadow].z = lbl_1_bss_15F4->models[body].z;
                    time = Hu3DMotionTimeGet(lbl_1_bss_15F4->models[shadow].id);
                    if (time >= 87.0f) {
                        Hu3DModelAttrSet(lbl_1_bss_15F4->models[shadow].id, 1);
                        lbl_1_bss_158[i].s04 = 6;
                    }
                    break;
                case 4:
                    Hu3DMotionTimeSet(lbl_1_bss_15F4->models[shadow].id, 60.0f);
                    lbl_1_bss_158[i].s04++;
                case 5:
                    lbl_1_bss_158[i].f1C -= lbl_1_bss_158[i].f20;
                    if (lbl_1_bss_158[i].f1C <= 0.0f) {
                        lbl_1_bss_158[i].f1C = 0.0f;
                    }
                    Hu3DModelScaleSet(lbl_1_bss_15F4->models[shadow].id, 1.0f, 1.0f, lbl_1_bss_158[i].f1C);
                    time = Hu3DMotionTimeGet(lbl_1_bss_15F4->models[shadow].id);
                    if (time >= 87.0f || lbl_1_bss_158[i].f1C == 0.0f) {
                        Hu3DModelAttrSet(lbl_1_bss_15F4->models[shadow].id, 1);
                        lbl_1_bss_158[i].active = 0;
                        lbl_1_bss_158[i].s04 = 6;
                    }
                    break;
                case 6:
                    break;
            }
        }
    }
}

void fn_1_9324(HuVecF *arg0, s16 arg1)
{
    s16 i;
    s16 j;
    s16 model;
    s16 sc;
    s16 pan;
    s32 sfx;

    sfx = 0;
    for (i = 0; i < 10; i++) {
        if (lbl_1_bss_18[i].active == 0) {
            model = i + 0xF;
            lbl_1_bss_18[i].active = 1;
            lbl_1_bss_18[i].s04 = 0;
            lbl_1_bss_18[i].s02 = 0;
            lbl_1_bss_18[i].s06 = 30;
            lbl_1_bss_18[i].s08 = 0;
            lbl_1_bss_18[i].type = arg1;
            lbl_1_bss_18[i].fx0C = 100.0f + 50.0f * lbl_1_bss_18[i].type;
            lbl_1_bss_18[i].fx10 = 100.0f + 50.0f * lbl_1_bss_18[i].type;
            lbl_1_bss_18[i].pos = *arg0;
            lbl_1_bss_18[i].pos.y = 150.0f;
            *M549G2_MDLPOS(model) = lbl_1_bss_18[i].pos;
            sc = lbl_1_bss_18[i].fx10 / 50.0f;
            Hu3DModelAttrReset(lbl_1_bss_15F4->models[model].id, 1);
            Hu3DModelScaleSet(lbl_1_bss_15F4->models[model].id, sc, sc, sc);
            Hu3DMotionTimeSet(lbl_1_bss_15F4->models[model].id, 0.0f);
            Hu3DMotionSpeedSet(lbl_1_bss_15F4->models[model].id, 1.0f);
            for (j = 0; j < 2; j++) {
                switch (lbl_1_bss_18[i].type) {
                    case 0:
                        omVibrate(lbl_1_bss_15F4->chars[j].playerNo, 20, 6, 6);
                        break;
                    case 1:
                        omVibrate(lbl_1_bss_15F4->chars[j].playerNo, 30, 6, 6);
                        break;
                    case 2:
                        omVibrate(lbl_1_bss_15F4->chars[j].playerNo, 20, 4, 2);
                        break;
                    case 3:
                        omVibrate(lbl_1_bss_15F4->chars[j].playerNo, 30, 4, 2);
                        break;
                    case 4:
                        omVibrate(lbl_1_bss_15F4->chars[j].playerNo, 30, 12, 0);
                        break;
                }
            }
            switch (arg1) {
                case 0:
                case 1:
                    sfx = 0;
                    break;
                case 2:
                case 3:
                    sfx = 1;
                    break;
                case 4:
                    sfx = 2;
                    break;
            }
            sfx += 5;
            pan = 64.0f + 0.026666667f * lbl_1_bss_18[i].pos.x;
            fn_1_3754(sfx, 0x7F, pan);
            return;
        }
    }
}

void fn_1_98F0(void)
{
    s16 i;
    s16 model;

    for (i = 0; i < 10; i++) {
        if (lbl_1_bss_18[i].active != 0) {
            model = i + 0xF;
            if (lbl_1_bss_18[i].s02 != 0) {
            } else if (Hu3DMotionEndCheck(lbl_1_bss_15F4->models[model].id) != 0) {
                lbl_1_bss_18[i].active = 0;
                Hu3DModelAttrSet(lbl_1_bss_15F4->models[model].id, 1);
            }
        }
    }
}

s32 fn_1_99E8(s16 arg0)
{
    M549StageSlot *slot;
    s32 done;

    slot = &lbl_1_bss_15F4->slots[arg0];
    done = 0;
    switch (lbl_1_bss_3B0[arg0].s06) {
        case 0:
            Hu3DModelObjPosGet(lbl_1_bss_15F4->models[arg0 + 0x44].id, lbl_1_bss_3B0[arg0].hook, M549G2_MDLPOS(arg0 + 3));
            lbl_1_bss_15F4->models[arg0 + 3].rotY = 90.0f - 180.0f * arg0;
            Hu3DModelHookObjReset(lbl_1_bss_15F4->models[arg0 + 0x44].id, lbl_1_bss_3B0[arg0].hook);
            if (lbl_1_bss_3B0[arg0].mode == 0) {
                slot->shiftState = 1;
                slot->shiftMotion = 0;
                slot->shiftStart = 0.0f;
                slot->shiftEnd = 16.0f;
                slot->shiftAttr = 0x40000001;
                slot->attrReset = 0;
            }
            lbl_1_bss_3B0[arg0].scale = 1.0f;
            lbl_1_bss_3B0[arg0].s06++;
            break;
        case 1:
            lbl_1_bss_3B0[arg0].scale -= 0.035f;
            lbl_1_bss_15F4->models[arg0 + 3].y += 10.0f;
            lbl_1_bss_15F4->models[arg0 + 3].rotX += 20.0f;
            Hu3DModelScaleSet(lbl_1_bss_15F4->models[arg0 + 3].id, lbl_1_bss_3B0[arg0].scale, lbl_1_bss_3B0[arg0].scale, lbl_1_bss_3B0[arg0].scale);
            if (lbl_1_bss_3B0[arg0].scale <= 0.1f) {
                Hu3DModelAttrSet(lbl_1_bss_15F4->models[arg0 + 3].id, 1);
                lbl_1_bss_3B0[arg0].s06++;
            }
            break;
        case 2:
            done = 1;
            break;
    }
    return done;
}

void fn_1_9DBC(void)
{
    s16 cnt;
    s16 i;

    cnt = 0;
    if (fn_1_1B8C(lbl_1_bss_580.mgTimer) != 0) {
        lbl_1_bss_580.s22 = 1;
    }
    for (i = 0; i < 2; i++) {
        if (lbl_1_bss_3B0[i].s14 != 0) {
            lbl_1_bss_3B0[i].s6A = 0;
            lbl_1_bss_3B0[i].s6C = 0;
            lbl_1_bss_3B0[i].mode = 2;
            lbl_1_bss_580.s22 = 1;
            lbl_1_bss_580.s24 = (i ^ 1) | (i * 2);
        }
        if (lbl_1_bss_580.s22 != 0) {
            lbl_1_bss_3B0[i].s28 = 1;
            if (lbl_1_bss_3B0[i].mode != 2) {
                if (lbl_1_bss_3B0[i].mode == 0 && lbl_1_bss_3B0[i].state == 0) {
                    cnt++;
                }
            } else {
                cnt++;
            }
        }
    }
    if (cnt >= 2) {
        lbl_1_bss_580.s20 = 1;
        if ((lbl_1_bss_3B0[0].s14 != 0 && lbl_1_bss_3B0[1].s14 != 0) || (lbl_1_bss_3B0[0].s14 == 0 && lbl_1_bss_3B0[1].s14 == 0)) {
            lbl_1_bss_580.s24 = 0;
        }
    }
}

s32 fn_1_9FF4(s16 arg0, s16 arg1)
{
    s32 ret;

    ret = 0;
    switch (lbl_1_bss_3B0[arg0].sub) {
        case 0:
            lbl_1_bss_15F4->chars[arg0].btnDown |= 0x100;
            lbl_1_bss_3B0[arg0].sub++;
        case 1:
            if (lbl_1_bss_3B0[arg0].state == 1) {
                lbl_1_bss_15F4->chars[arg0].btn |= 0x100;
                if (lbl_1_bss_3B0[arg0].s5C >= arg1) {
                    lbl_1_bss_3B0[arg0].sub++;
                }
            } else {
                lbl_1_bss_3B0[arg0].sub = 0;
            }
            break;
        case 2:
            if (lbl_1_bss_3B0[arg0].state != 2) {
                lbl_1_bss_3B0[arg0].sub++;
            }
            break;
        case 3:
            ret = 1;
            break;
    }
    return ret;
}

s32 fn_1_A18C(void)
{
    s32 ret;
    s32 a;
    s32 b;
    f32 steps;

    ret = 0;
    a = 0;
    switch (lbl_1_bss_580.mode) {
        case 0:
            lbl_1_bss_15F4->chars[0].btn = 0;
            lbl_1_bss_15F4->chars[0].btnDown = 0;
            fn_1_36A4(10);
            lbl_1_bss_3B0[0].sub = 0;
            lbl_1_bss_3B0[1].sub = 0;
            lbl_1_bss_580.timer = 0;
            lbl_1_bss_580.mode++;
            break;
        case 1:
            if (lbl_1_bss_580.timer++ >= 90) {
                lbl_1_bss_580.timer = 0;
                lbl_1_bss_580.mode++;
            }
            break;
        case 2:
            switch (lbl_1_bss_3B0[0].sub) {
                case 0:
                    lbl_1_bss_15F4->chars[0].btnDown |= 0x100;
                    lbl_1_bss_3B0[0].sub++;
                case 1:
                    if (lbl_1_bss_3B0[0].state == 1) {
                        lbl_1_bss_15F4->chars[0].btn |= 0x100;
                        if (lbl_1_bss_3B0[0].s5C >= 180) {
                            lbl_1_bss_3B0[0].sub++;
                        }
                    } else {
                        lbl_1_bss_3B0[0].sub = 0;
                    }
                    break;
                case 2:
                    if (lbl_1_bss_3B0[0].state != 2) {
                        lbl_1_bss_3B0[0].sub++;
                    }
                    break;
            }
            lbl_1_bss_580.timer++;
            steps = 50.0f;
            a = fn_1_18B0(&lbl_1_bss_14[0].zoom, steps, lbl_1_data_288[0].zoom, lbl_1_data_288[2].zoom);
            a = fn_1_18B0(&lbl_1_bss_14[1].zoom, steps, lbl_1_data_288[1].zoom, lbl_1_data_288[2].zoom);
            if (a == 0) {
                lbl_1_bss_580.mode++;
            }
            break;
        case 3:
            switch (lbl_1_bss_3B0[0].sub) {
                case 0:
                    lbl_1_bss_15F4->chars[0].btnDown |= 0x100;
                    lbl_1_bss_3B0[0].sub++;
                case 1:
                    if (lbl_1_bss_3B0[0].state == 1) {
                        lbl_1_bss_15F4->chars[0].btn |= 0x100;
                        if (lbl_1_bss_3B0[0].s5C >= 180) {
                            lbl_1_bss_3B0[0].sub++;
                        }
                    } else {
                        lbl_1_bss_3B0[0].sub = 0;
                    }
                    break;
                case 2:
                    if (lbl_1_bss_3B0[0].state != 2) {
                        lbl_1_bss_3B0[0].sub++;
                    }
                    break;
            }
            if (lbl_1_bss_580.timer++ >= 120) {
                lbl_1_bss_580.timer = 0;
                lbl_1_bss_580.mode++;
            }
            break;
        case 4:
            if (lbl_1_bss_580.timer++ <= 11) {
                switch (lbl_1_bss_3B0[0].sub) {
                    case 0:
                        lbl_1_bss_15F4->chars[0].btnDown |= 0x100;
                        lbl_1_bss_3B0[0].sub++;
                    case 1:
                        if (lbl_1_bss_3B0[0].state == 1) {
                            lbl_1_bss_15F4->chars[0].btn |= 0x100;
                            if (lbl_1_bss_3B0[0].s5C >= 180) {
                                lbl_1_bss_3B0[0].sub++;
                            }
                        } else {
                            lbl_1_bss_3B0[0].sub = 0;
                        }
                        break;
                    case 2:
                        if (lbl_1_bss_3B0[0].state != 2) {
                            lbl_1_bss_3B0[0].sub++;
                        }
                        break;
                }
            } else {
                a = 1;
            }
            b = 0;
            switch (lbl_1_bss_3B0[1].sub) {
                case 0:
                    lbl_1_bss_15F4->chars[1].btnDown |= 0x100;
                    lbl_1_bss_3B0[1].sub++;
                case 1:
                    if (lbl_1_bss_3B0[1].state == 1) {
                        lbl_1_bss_15F4->chars[1].btn |= 0x100;
                        if (lbl_1_bss_3B0[1].s5C >= 0) {
                            lbl_1_bss_3B0[1].sub++;
                        }
                    } else {
                        lbl_1_bss_3B0[1].sub = 0;
                    }
                    break;
                case 2:
                    if (lbl_1_bss_3B0[1].state != 2) {
                        lbl_1_bss_3B0[1].sub++;
                    }
                    break;
                case 3:
                    b = 1;
                    break;
            }
            if ((s16)(a + b) >= 2) {
                lbl_1_bss_580.timer = 0;
                lbl_1_bss_580.mode++;
            }
            break;
        case 5:
            if (lbl_1_bss_580.timer++ >= 40) {
                lbl_1_bss_580.timer = 0;
                lbl_1_bss_580.mode++;
            }
            break;
        case 6:
            fn_1_35AC();
            ret = 1;
            break;
    }
    return ret;
}

/* ---- game.c part G3: fn_1_A898 .. fn_1_CDD8 ---- */

s32 fn_1_A898(void)
{
    HU3DMODEL *mdl;
    M549StageSlot *slot;
    s32 done;
    s32 ret;

    mdl = &Hu3DData[lbl_1_bss_15F4->models[(lbl_1_bss_580.s24 & 1) + 0x44].id];
    slot = &lbl_1_bss_15F4->slots[lbl_1_bss_580.s24 & 1];
    done = 0;
    ret = 0;
    switch (lbl_1_bss_580.mode) {
        case 0:
            if (lbl_1_bss_580.timer++ >= 30) {
                lbl_1_bss_580.timer = 0;
                if (lbl_1_bss_580.s24 != 0) {
                    lbl_1_bss_580.f08 = lbl_1_bss_0.player[lbl_1_bss_580.s24 & 1]->actor->rotY;
                    if (lbl_1_bss_580.f08 <= 1.0f && lbl_1_bss_580.f08 >= -1.0f) {
                        lbl_1_bss_580.mode = 2;
                        break;
                    }
                    slot->shiftState = 1;
                    slot->shiftMotion = 1;
                    slot->shiftStart = 0.0f;
                    slot->shiftEnd = 4.0f;
                    slot->shiftAttr = 0x40000001;
                    slot->attrReset = 0;
                }
                lbl_1_bss_580.mode++;
            }
            break;
        case 1:
            if (lbl_1_bss_580.s24 != 0) {
                ret = fn_1_18B0(&mdl->rot.y, 30.0f, lbl_1_bss_580.f08, 0.0f);
                if ((s16)ret == 0) {
                    slot->shiftState = 1;
                    slot->shiftMotion = 0;
                    slot->shiftStart = 0.0f;
                    slot->shiftEnd = 8.0f;
                    slot->shiftAttr = 0x40000001;
                    slot->attrReset = 0;
                    lbl_1_bss_580.mode++;
                }
            } else {
                lbl_1_bss_580.mode++;
            }
            break;
        case 2:
            if (lbl_1_bss_580.s24 != 0) {
                f32 time = 30.0f;

                ret += fn_1_18B0(&lbl_1_bss_14[0].center.x, time, lbl_1_data_288[2].center.x, mdl->pos.x);
                ret += fn_1_18B0(&lbl_1_bss_14[0].center.y, time, lbl_1_data_288[2].center.y, 70.0f + mdl->pos.y);
                ret += fn_1_18B0(&lbl_1_bss_14[0].center.z, time, lbl_1_data_288[2].center.z, mdl->pos.z);
                ret += fn_1_18B0(&lbl_1_bss_14[0].zoom, time, lbl_1_data_288[2].zoom, 500.0f);
                ret += fn_1_18B0(&lbl_1_bss_14[1].center.x, time, lbl_1_data_288[2].center.x, mdl->pos.x);
                ret += fn_1_18B0(&lbl_1_bss_14[1].center.y, time, lbl_1_data_288[2].center.y, 70.0f + mdl->pos.y);
                ret += fn_1_18B0(&lbl_1_bss_14[1].center.z, time, lbl_1_data_288[2].center.z, mdl->pos.z);
                ret += fn_1_18B0(&lbl_1_bss_14[1].zoom, time, lbl_1_data_288[2].zoom, 500.0f);
            }
            if ((s16)ret == 0) {
                lbl_1_bss_580.mode++;
            }
            break;
        case 3:
            done = 1;
            break;
    }
    return done;
}

int fn_1_AD78(COL_NARROW_PARAM *a, COL_NARROW_PARAM *b)
{
    s16 unused = 0;

    switch (b->type) {
        case 0:
            lbl_1_bss_3B0[b->paramB].s7A++;
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            break;
    }
    return 0;
}

void fn_1_ADC8(MGACTOR *actor, int param)
{
    int unused = actor->param;
}

void fn_1_ADDC(void)
{
    f32 dx;
    f32 dz;
    f32 dist;
    f32 unusedF = 45.0f;
    s16 unused0 = 0;
    s16 unused1 = 0;
    s16 unused2 = 0;
    s16 unused3 = 0;
    s16 unused4 = 0;
    s16 i;
    s16 j;
    s16 diff;
    s32 count;
    HuVecF *hitP;
    HuVecF pos;
    HuVecF hitPos;

    for (i = 0; i < 10; i++) {
        if (lbl_1_bss_158[i].active != 0 && lbl_1_bss_158[i].s0A == 0) {
            for (j = 0; j < 10; j++) {
                if (lbl_1_bss_158[j].active != 0 && lbl_1_bss_158[j].s0A == 0 && i != j) {
                    dx = lbl_1_bss_158[j].pos.x - lbl_1_bss_158[i].pos.x;
                    dz = lbl_1_bss_158[j].pos.z - lbl_1_bss_158[i].pos.z;
                    dist = sqrt((dx * dx) + (dz * dz));
                    if (dist <= lbl_1_bss_158[i].radius + lbl_1_bss_158[j].radius) {
                        lbl_1_bss_158[i].s0A = 1;
                        lbl_1_bss_158[j].s0A = 1;
                        dx *= 0.5f;
                        dz *= 0.5f;
                        pos.x = dx + lbl_1_bss_158[i].pos.x;
                        pos.y = 0.0f;
                        pos.z = dz + lbl_1_bss_158[i].pos.z;
                        diff = lbl_1_bss_158[j].level - lbl_1_bss_158[i].level;
                        if (diff > 0) {
                            count = diff;
                        } else {
                            count = diff * -1;
                        }
                        diff = count;
                        hitPos = pos;
                        hitP = &hitPos;
                        fn_1_9324(hitP, diff);
                    }
                }
            }
        }
    }
    for (i = 0; i < 2; i++) {
        dx = -lbl_1_bss_3B0[i].pos.x;
        dz = -lbl_1_bss_3B0[i].pos.z;
        dist = sqrt((dx * dx) + (dz * dz));
        if (dist > lbl_1_data_498 && lbl_1_bss_3B0[i].s14 == 0) {
            lbl_1_bss_3B0[i].f44 = lbl_1_bss_3B0[i].pos.x;
            lbl_1_bss_3B0[i].f4C = lbl_1_bss_3B0[i].pos.z;
            dist = sqrt((lbl_1_bss_3B0[i].f44 * lbl_1_bss_3B0[i].f44) + (lbl_1_bss_3B0[i].f4C * lbl_1_bss_3B0[i].f4C));
            lbl_1_bss_3B0[i].f44 = 10.0f * (lbl_1_bss_3B0[i].f44 / dist);
            lbl_1_bss_3B0[i].f4C = 10.0f * (lbl_1_bss_3B0[i].f4C / dist);
            lbl_1_bss_3B0[i].s14 = 1;
        }
        if (lbl_1_bss_580.s22 == 0) {
            for (j = 0; j < 10; j++) {
                if (lbl_1_bss_158[j].active != 0 && lbl_1_bss_158[j].s0A == 0 && i != lbl_1_bss_158[j].side) {
                    dx = lbl_1_bss_158[j].pos.x - lbl_1_bss_3B0[i].pos.x;
                    dz = lbl_1_bss_158[j].pos.z - lbl_1_bss_3B0[i].pos.z;
                    dist = sqrt((dx * dx) + (dz * dz));
                    if (dist <= 45.0f + lbl_1_bss_158[j].radius) {
                        lbl_1_bss_158[j].s0A = 1;
                        if (lbl_1_bss_3B0[i].s6A == 0 && lbl_1_bss_3B0[i].s6C == 0) {
                            lbl_1_bss_3B0[i].f64 = lbl_1_bss_158[j].angle;
                            lbl_1_bss_3B0[i].s68 = lbl_1_bss_158[j].level;
                            lbl_1_bss_3B0[i].s6A = 1;
                        }
                    }
                }
            }
            if (lbl_1_bss_3B0[i].s6A == 0 && lbl_1_bss_3B0[i].s6C == 0) {
                for (j = 0; j < 10; j++) {
                    if (lbl_1_bss_18[j].active != 0) {
                        dx = lbl_1_bss_18[j].pos.x - lbl_1_bss_3B0[i].pos.x;
                        dz = lbl_1_bss_18[j].pos.z - lbl_1_bss_3B0[i].pos.z;
                        dist = sqrt((dx * dx) + (dz * dz));
                        if (dist <= lbl_1_bss_18[j].fx0C) {
                            dx = 180.0 * (acos(dx / (0.1f + dist)) / 3.141592653589793);
                            dz = -(180.0 * (asin(dz / (0.1f + dist)) / 3.141592653589793));
                            if (dz < 0.0f) {
                                dx = 360.0f - dx;
                            }
                            lbl_1_bss_3B0[i].f64 = dx - 90.0f;
                            lbl_1_bss_3B0[i].s68 = lbl_1_bss_18[j].type;
                            lbl_1_bss_3B0[i].s6C = 1;
                        }
                    }
                }
            }
        }
    }
}

void fn_1_BF34(s16 player)
{
    f32 speedTbl[4] = { 6.2222223f, 7.0f, 8.0f, 10.0f };
    s16 tblA[4] = { 10, 8, 5, 1 };
    s16 tblB[4] = { 5, 7, 8, 10 };
    s16 tblC[4] = { 20, 30, 70, 100 };
    s16 tblD[4] = { 20, 10, 3, 0 };
    s16 tblE[4] = { 0, 10, 40, 60 };
    s16 tblF[4] = { 20, 50, 80, 100 };
    s16 tblG[4] = { 20, 5, 0, 0 };

    if (lbl_1_bss_15F4->chars[player].cpuLevel != 0) {
        lbl_1_bss_3B0[player].s6E = 1;
        lbl_1_bss_3B0[player].s70 = lbl_1_bss_15F4->chars[player].cpuLevel - 1;
        lbl_1_bss_3B0[player].fC4 = speedTbl[lbl_1_bss_3B0[player].s70];
        lbl_1_bss_3B0[player].chaseSpeed = speedTbl[lbl_1_bss_3B0[player].s70];
        lbl_1_bss_3B0[player].chaseRadius = 10.0f;
        lbl_1_bss_3B0[player].sC8 = tblA[lbl_1_bss_3B0[player].s70];
        lbl_1_bss_3B0[player].sCA = tblB[lbl_1_bss_3B0[player].s70];
        lbl_1_bss_3B0[player].sCC = tblC[lbl_1_bss_3B0[player].s70];
        lbl_1_bss_3B0[player].sCE = tblD[lbl_1_bss_3B0[player].s70];
        lbl_1_bss_3B0[player].sD0 = tblE[lbl_1_bss_3B0[player].s70];
        lbl_1_bss_3B0[player].sD2 = tblF[lbl_1_bss_3B0[player].s70];
        lbl_1_bss_3B0[player].sD4 = tblG[lbl_1_bss_3B0[player].s70];
        fn_1_C418(player);
    }
}

s32 fn_1_C360(s16 player)
{
    s32 ret = 0;
    s16 unused = 0;

    lbl_1_bss_3B0[player].btnC = 0;
    lbl_1_bss_3B0[player].btnD = 0;
    lbl_1_bss_3B0[player].btnB = 0;
    lbl_1_bss_3B0[player].btnA = 0;
    fn_1_FDB0(player);
    return ret;
}

s32 fn_1_C418(s16 player)
{
    s32 ret = 0;
    s16 chased = 0;

    if (lbl_1_bss_3B0[player].s6A == 0 && lbl_1_bss_3B0[player].s6C == 0) {
        switch (lbl_1_bss_3B0[player].s7C) {
            case 0:
                MgPlayerComStkOn(lbl_1_bss_0.player[player]);
                lbl_1_bss_3B0[player].s7C++;
                break;
            case 1:
                if (lbl_1_bss_3B0[player].s28 == 0) {
                    lbl_1_bss_3B0[player].chaseSpeed = lbl_1_bss_3B0[player].fC4 * lbl_1_data_484[lbl_1_bss_3B0[player].s60];
                    chased = MgPlayerVecChase(lbl_1_bss_0.player[player], &lbl_1_bss_3B0[player].target, lbl_1_bss_3B0[player].chaseSpeed, lbl_1_bss_3B0[player].chaseRadius);
                    if (chased != 0) {
                        ret = 1;
                    }
                }
                break;
            case 2:
                MgPlayerComStkOff(lbl_1_bss_0.player[player]);
                lbl_1_bss_3B0[player].s7C = 0;
                ret = 1;
                break;
        }
    }
    return ret;
}

void fn_1_C64C(s16 player)
{
    f32 dist;
    HuVecF dir;
    HuVecF aim;

    aim.x = (lbl_1_bss_3B0[player ^ 1].velX / 5.6f) * lbl_1_bss_3B0[player].fC0;
    aim.z = -((lbl_1_bss_3B0[player ^ 1].velZ / 5.6f) * lbl_1_bss_3B0[player].fC0);
    aim.x += lbl_1_bss_3B0[player ^ 1].pos.x;
    aim.z += lbl_1_bss_3B0[player ^ 1].pos.z;
    dir.x = aim.x - lbl_1_bss_3B0[player].pos.x;
    dir.z = aim.z - lbl_1_bss_3B0[player].pos.z;
    dist = sqrt((dir.x * dir.x) + (dir.z * dir.z));
    if (dist != 0.0f) {
        dir.x = lbl_1_bss_3B0[player].pos.x + (20.0f * (dir.x / dist));
        dir.z = lbl_1_bss_3B0[player].pos.z + (20.0f * (dir.z / dist));
    } else {
        dir.x = lbl_1_bss_3B0[player].pos.x;
        dir.z = lbl_1_bss_3B0[player].pos.z;
    }
    lbl_1_bss_3B0[player].target.x = dir.x;
    lbl_1_bss_3B0[player].target.y = lbl_1_bss_3B0[player].pos.y;
    lbl_1_bss_3B0[player].target.z = dir.z;
}

void fn_1_CA38(s16 player, s16 objNo)
{
    f32 dist;
    HuVecF dir;
    s16 i;

    dir.x = lbl_1_bss_158[objNo].pos.x;
    dir.z = lbl_1_bss_158[objNo].pos.z;
    for (i = 0; i < 5; i++) {
        dir.x += lbl_1_bss_158[objNo].dir.x;
        dir.z += lbl_1_bss_158[objNo].dir.z;
    }
    dir.x -= lbl_1_bss_3B0[player].pos.x;
    dir.z -= lbl_1_bss_3B0[player].pos.z;
    dist = sqrt((dir.x * dir.x) + (dir.z * dir.z));
    if (dist != 0.0f) {
        dir.x = lbl_1_bss_3B0[player].pos.x + (20.0f * (dir.x / dist));
        dir.z = lbl_1_bss_3B0[player].pos.z + (20.0f * (dir.z / dist));
    } else {
        dir.x = lbl_1_bss_3B0[player].pos.x;
        dir.z = lbl_1_bss_3B0[player].pos.z;
    }
    lbl_1_bss_3B0[player].target.x = dir.x;
    lbl_1_bss_3B0[player].target.y = lbl_1_bss_3B0[player].pos.y;
    lbl_1_bss_3B0[player].target.z = dir.z;
}

s32 fn_1_CDD8(s16 player)
{
    f32 ang;
    f32 radius;
    s32 ret = 0;
    HuVecF pos;

    pos = lbl_1_bss_3B0[player].pos;
    do {
        ang = frandmod(360);
        radius = frandmod(395);
        pos.x = radius * cos((3.141592653589793 * ang) / 180.0);
        pos.z = radius * sin((3.141592653589793 * ang) / 180.0);
    } while (!((f32)sqrt((pos.x * pos.x) + (pos.z * pos.z)) < lbl_1_data_498));
    lbl_1_bss_3B0[player].target = pos;
    ret = 1;
    return ret;
}

/* ---- game.c part G4: fn_1_D0C4 .. fn_1_FDB0 (COM player AI helpers) ---- */

/* chase target selection: follow the other character's predicted position */
s32 fn_1_D0C4(s16 no, s16 limit) {
    HuVecF a;
    HuVecF b;
    f32 len;
    s32 ret = 0;

    switch (lbl_1_bss_3B0[no].sA8) {
        case 0:
            lbl_1_bss_3B0[no].btnB |= 0x100;
            lbl_1_bss_3B0[no].sAA = 0;
            lbl_1_bss_3B0[no].sA8++;
        case 1:
            if (lbl_1_bss_3B0[no].state == 1) {
                lbl_1_bss_3B0[no].btnA |= 0x100;
                if (lbl_1_bss_3B0[no].s5C >= limit) {
                    a.x = (lbl_1_bss_3B0[no ^ 1].velX / 5.6f) * lbl_1_bss_3B0[no].fC0;
                    a.z = -((lbl_1_bss_3B0[no ^ 1].velZ / 5.6f) * lbl_1_bss_3B0[no].fC0);
                    a.x += lbl_1_bss_3B0[no ^ 1].pos.x;
                    a.z += lbl_1_bss_3B0[no ^ 1].pos.z;
                    b.x = a.x - lbl_1_bss_3B0[no].pos.x;
                    b.z = a.z - lbl_1_bss_3B0[no].pos.z;
                    len = M549_Mag2D(b.x, b.z);
                    if (len != 0.0f) {
                        b.x = lbl_1_bss_3B0[no].pos.x + (20.0f * (b.x / len));
                        b.z = lbl_1_bss_3B0[no].pos.z + (20.0f * (b.z / len));
                    } else {
                        b.x = lbl_1_bss_3B0[no].pos.x;
                        b.z = lbl_1_bss_3B0[no].pos.z;
                    }
                    lbl_1_bss_3B0[no].target.x = b.x;
                    lbl_1_bss_3B0[no].target.y = lbl_1_bss_3B0[no].pos.y;
                    lbl_1_bss_3B0[no].target.z = b.z;
                    lbl_1_bss_3B0[no].targetSave = lbl_1_bss_3B0[no].target;
                    lbl_1_bss_3B0[no].s82 = lbl_1_bss_3B0[no].s80;
                    lbl_1_bss_3B0[no].sA8++;
                }
            } else {
                lbl_1_bss_3B0[no].sA8 = 0;
            }
            break;
        case 2:
            lbl_1_bss_3B0[no].s80 = 0;
            if (lbl_1_bss_3B0[no].s6A == 0 && lbl_1_bss_3B0[no].s6C == 0) {
                switch (lbl_1_bss_3B0[no].s7C) {
                    case 0:
                        MgPlayerComStkOn(lbl_1_bss_0.player[no]);
                        lbl_1_bss_3B0[no].s7C++;
                        break;
                    case 1:
                        if (lbl_1_bss_3B0[no].s28 == 0) {
                            lbl_1_bss_3B0[no].chaseSpeed = lbl_1_bss_3B0[no].fC4 * lbl_1_data_484[lbl_1_bss_3B0[no].s60];
                            if (MgPlayerVecChase(lbl_1_bss_0.player[no], &lbl_1_bss_3B0[no].target,
                                                 lbl_1_bss_3B0[no].chaseSpeed, lbl_1_bss_3B0[no].chaseRadius)) {
                            }
                        }
                        break;
                    case 2:
                        MgPlayerComStkOff(lbl_1_bss_0.player[no]);
                        lbl_1_bss_3B0[no].s7C = 0;
                        break;
                }
            }
            if (lbl_1_bss_3B0[no].sAA++ >= 5) {
                lbl_1_bss_3B0[no].target = lbl_1_bss_3B0[no].targetSave;
                lbl_1_bss_3B0[no].s80 = lbl_1_bss_3B0[no].s82;
                lbl_1_bss_3B0[no].sA8++;
            } else {
                lbl_1_bss_3B0[no].btnA |= 0x100;
            }
            break;
        case 3:
            if (lbl_1_bss_3B0[no].state != 2) {
                lbl_1_bss_3B0[no].sA8 = 0;
                ret = 1;
            }
            break;
    }
    return ret;
}

s32 fn_1_D9E0(s16 no) {
    HuVecF a;
    HuVecF b;
    f32 len;
    s32 ret = 0;

    switch (lbl_1_bss_3B0[no].sA8) {
        case 0:
            lbl_1_bss_3B0[no].btnB |= 0x100;
            lbl_1_bss_3B0[no].sAA = 0;
            lbl_1_bss_3B0[no].sA8++;
        case 1:
            if (lbl_1_bss_3B0[no].state == 1) {
                lbl_1_bss_3B0[no].btnA |= 0x100;
                a.x = (lbl_1_bss_3B0[no ^ 1].velX / 5.6f) * lbl_1_bss_3B0[no].fC0;
                a.z = -((lbl_1_bss_3B0[no ^ 1].velZ / 5.6f) * lbl_1_bss_3B0[no].fC0);
                a.x += lbl_1_bss_3B0[no ^ 1].pos.x;
                a.z += lbl_1_bss_3B0[no ^ 1].pos.z;
                b.x = a.x - lbl_1_bss_3B0[no].pos.x;
                b.z = a.z - lbl_1_bss_3B0[no].pos.z;
                len = M549_Mag2D(b.x, b.z);
                if (len != 0.0f) {
                    b.x = lbl_1_bss_3B0[no].pos.x + (20.0f * (b.x / len));
                    b.z = lbl_1_bss_3B0[no].pos.z + (20.0f * (b.z / len));
                } else {
                    b.x = lbl_1_bss_3B0[no].pos.x;
                    b.z = lbl_1_bss_3B0[no].pos.z;
                }
                lbl_1_bss_3B0[no].target.x = b.x;
                lbl_1_bss_3B0[no].target.y = lbl_1_bss_3B0[no].pos.y;
                lbl_1_bss_3B0[no].target.z = b.z;
                if (lbl_1_bss_3B0[no].s6A == 0 && lbl_1_bss_3B0[no].s6C == 0) {
                    switch (lbl_1_bss_3B0[no].s7C) {
                        case 0:
                            MgPlayerComStkOn(lbl_1_bss_0.player[no]);
                            lbl_1_bss_3B0[no].s7C++;
                            break;
                        case 1:
                            if (lbl_1_bss_3B0[no].s28 == 0) {
                                lbl_1_bss_3B0[no].chaseSpeed = lbl_1_bss_3B0[no].fC4 * lbl_1_data_484[lbl_1_bss_3B0[no].s60];
                                if (MgPlayerVecChase(lbl_1_bss_0.player[no], &lbl_1_bss_3B0[no].target,
                                                     lbl_1_bss_3B0[no].chaseSpeed, lbl_1_bss_3B0[no].chaseRadius)) {
                                }
                            }
                            break;
                        case 2:
                            MgPlayerComStkOff(lbl_1_bss_0.player[no]);
                            lbl_1_bss_3B0[no].s7C = 0;
                            break;
                    }
                    lbl_1_bss_3B0[no].sA8++;
                } else {
                    lbl_1_bss_3B0[no].btnB |= 0x100;
                }
            }
            break;
        case 2:
            if (lbl_1_bss_3B0[no].state != 2) {
                lbl_1_bss_3B0[no].sA8 = 0;
                ret = 1;
            }
            break;
    }
    return ret;
}

s32 fn_1_E160(s16 no, s16 objNo) {
    HuVecF v;
    f32 len;
    s16 i;
    s32 ret = 0;

    switch (lbl_1_bss_3B0[no].sA8) {
        case 0:
            lbl_1_bss_3B0[no].btnB |= 0x100;
            lbl_1_bss_3B0[no].sAA = 0;
            lbl_1_bss_3B0[no].sA8++;
        case 1:
            if (lbl_1_bss_3B0[no].state == 1) {
                lbl_1_bss_3B0[no].btnA |= 0x100;
                v.x = lbl_1_bss_158[objNo].pos.x;
                v.z = lbl_1_bss_158[objNo].pos.z;
                for (i = 0; i < 5; i++) {
                    v.x += lbl_1_bss_158[objNo].dir.x;
                    v.z += lbl_1_bss_158[objNo].dir.z;
                }
                v.x = v.x - lbl_1_bss_3B0[no].pos.x;
                v.z = v.z - lbl_1_bss_3B0[no].pos.z;
                len = M549_Mag2D(v.x, v.z);
                if (len != 0.0f) {
                    v.x = lbl_1_bss_3B0[no].pos.x + (20.0f * (v.x / len));
                    v.z = lbl_1_bss_3B0[no].pos.z + (20.0f * (v.z / len));
                } else {
                    v.x = lbl_1_bss_3B0[no].pos.x;
                    v.z = lbl_1_bss_3B0[no].pos.z;
                }
                lbl_1_bss_3B0[no].target.x = v.x;
                lbl_1_bss_3B0[no].target.y = lbl_1_bss_3B0[no].pos.y;
                lbl_1_bss_3B0[no].target.z = v.z;
                if (lbl_1_bss_3B0[no].s6A == 0 && lbl_1_bss_3B0[no].s6C == 0) {
                    switch (lbl_1_bss_3B0[no].s7C) {
                        case 0:
                            MgPlayerComStkOn(lbl_1_bss_0.player[no]);
                            lbl_1_bss_3B0[no].s7C++;
                            break;
                        case 1:
                            if (lbl_1_bss_3B0[no].s28 == 0) {
                                lbl_1_bss_3B0[no].chaseSpeed = lbl_1_bss_3B0[no].fC4 * lbl_1_data_484[lbl_1_bss_3B0[no].s60];
                                if (MgPlayerVecChase(lbl_1_bss_0.player[no], &lbl_1_bss_3B0[no].target,
                                                     lbl_1_bss_3B0[no].chaseSpeed, lbl_1_bss_3B0[no].chaseRadius)) {
                                }
                            }
                            break;
                        case 2:
                            MgPlayerComStkOff(lbl_1_bss_0.player[no]);
                            lbl_1_bss_3B0[no].s7C = 0;
                            break;
                    }
                    lbl_1_bss_3B0[no].sA8++;
                } else {
                    lbl_1_bss_3B0[no].btnB |= 0x100;
                }
            }
            break;
        case 2:
            if (lbl_1_bss_3B0[no].state != 2) {
                lbl_1_bss_3B0[no].sA8 = 0;
                ret = 1;
            }
            break;
    }
    return ret;
}

void fn_1_E880(s16 no) {
    if (lbl_1_bss_3B0[no].sAC != 0) {
        return;
    }
    if (lbl_1_bss_3B0[no].state == 1) {
        lbl_1_bss_3B0[no].btnA |= 0x100;
    } else {
        lbl_1_bss_3B0[no].btnB |= 0x100;
    }
}

/* pick a point on the circle around the character that stays inside the arena */
s32 fn_1_E90C(s16 no, s16 objNo) {
    HuVecF v;
    HuVecF w;
    f32 lenA;
    f32 lenB;
    f32 ang;
    s32 ret;

    v.x = lbl_1_bss_158[objNo].pos.x - lbl_1_bss_3B0[no].pos.x;
    v.z = lbl_1_bss_158[objNo].pos.z - lbl_1_bss_3B0[no].pos.z;
    if (M549_Mag2D(v.x, v.z) >= 130.0f) {
        ang = lbl_1_bss_3B0[no].fBC;
        v.x = (f32)(lbl_1_bss_3B0[no].pos.x + (200.0 * HuSin(ang)));
        v.z = (f32)(lbl_1_bss_3B0[no].pos.z + (200.0 * HuCos(ang)));
        lenA = M549_Mag2D(v.x, v.z);
        ang = 180.0f + lbl_1_bss_3B0[no].fBC;
        w.x = (f32)(lbl_1_bss_3B0[no].pos.x + (200.0 * HuSin(ang)));
        w.z = (f32)(lbl_1_bss_3B0[no].pos.z + (200.0 * HuCos(ang)));
        lenB = M549_Mag2D(w.x, w.z);
        if (lenB < lbl_1_data_498) {
            lbl_1_bss_3B0[no].target.x = w.x;
            lbl_1_bss_3B0[no].target.z = w.z;
            ret = 1;
        } else if (lenA < lbl_1_data_498) {
            lbl_1_bss_3B0[no].target.x = v.x;
            lbl_1_bss_3B0[no].target.z = v.z;
            ret = 1;
        } else {
            ret = -1;
        }
    } else {
        ret = -1;
    }
    return ret;
}

/* distance to the opponent / to the arena centre */
void fn_1_F0C8(s16 no) {
    HuVecF d;
    s16 other = no ^ 1;

    d.x = lbl_1_bss_3B0[other].pos.x - lbl_1_bss_3B0[no].pos.x;
    d.z = lbl_1_bss_3B0[other].pos.z - lbl_1_bss_3B0[no].pos.z;
    lbl_1_bss_3B0[no].fB4 = M549_Mag2D(d.x, d.z);
    d.x = lbl_1_bss_3B0[no].pos.x;
    d.z = lbl_1_bss_3B0[no].pos.z;
    lbl_1_bss_3B0[no].fB8 = M549_Mag2D(d.x, d.z);
}

/* does the object's path (segment of length 1190 along dir) pass close to the character? */
s32 fn_1_F4A0(s16 no, s16 objNo) {
    HuVecF p0;
    HuVecF p1;
    f32 maxX;
    f32 minX;
    f32 maxZ;
    f32 minZ;
    f32 dx;
    f32 dz;
    f32 t;
    f32 num;
    f32 len;
    f32 den;
    f32 ang;
    f32 sinTest;
    f32 cz;
    s32 ret = 0;

    p0 = lbl_1_bss_158[objNo].pos;
    len = M549_Mag2D(lbl_1_bss_158[objNo].dir.x, lbl_1_bss_158[objNo].dir.z);
    if (len != 0.0f) {
        p1.x = p0.x + ((1190.0f / len) * lbl_1_bss_158[objNo].dir.x);
        p1.z = p0.z + ((1190.0f / len) * lbl_1_bss_158[objNo].dir.z);
    } else {
        p1.x = p0.x;
        p1.z = p0.z;
    }
    if (p0.x > p1.x) {
        maxX = p0.x;
        minX = p1.x;
    } else {
        maxX = p1.x;
        minX = p0.x;
    }
    if (p0.z > p1.z) {
        maxZ = p0.z;
        minZ = p1.z;
    } else {
        maxZ = p1.z;
        minZ = p0.z;
    }
    maxX = maxX + (45.0f + lbl_1_bss_158[objNo].radius);
    minX -= 45.0f + lbl_1_bss_158[objNo].radius;
    maxZ += 45.0f + lbl_1_bss_158[objNo].radius;
    minZ -= 45.0f + lbl_1_bss_158[objNo].radius;
    if (lbl_1_bss_3B0[no].pos.x <= maxX && lbl_1_bss_3B0[no].pos.x >= minX
        && lbl_1_bss_3B0[no].pos.z <= maxZ && lbl_1_bss_3B0[no].pos.z >= minZ) {
        dx = p0.x - p1.x;
        dz = p0.z - p1.z;
        den = (dx * dx) + (dz * dz);
        num = -1.0f * ((dx * (p0.x - lbl_1_bss_3B0[no].pos.x)) + (dz * (p0.z - lbl_1_bss_3B0[no].pos.z)));
        if (den != 0.0f) {
            t = num / den;
        } else {
            t = 0.0f;
        }
        cz = p0.z + (dz * t);
        dx = lbl_1_bss_3B0[no].pos.x - (p0.x + (dx * t));
        dz = lbl_1_bss_3B0[no].pos.z - cz;
        len = M549_Mag2D(dx, dz);
        if (len < 45.0f + lbl_1_bss_158[objNo].radius) {
            ang = (f32)(180.0 * (acos(dx / (0.1f + len)) / M_PI));
            sinTest = (f32) - (180.0 * (asin(dz / (0.1f + len)) / M_PI));
            if (sinTest <= 0.0f) {
                ang = 360.0f - ang;
            }
            lbl_1_bss_3B0[no].fBC = ang - 90.0f;
            ret = 1;
        }
    }
    return ret;
}

/* COM player AI: decide and run the current action */
s32 fn_1_FDB0(s16 no) {
    HuVecF d;
    HuVecF v;
    HuVecF w;
    f32 rot;
    f32 dist;
    s16 i;
    s16 other;
    s16 range;
    s16 rnd;
    s16 done;
    s32 ret = 0;

    other = no ^ 1;
    d.x = lbl_1_bss_3B0[other].pos.x - lbl_1_bss_3B0[no].pos.x;
    d.z = lbl_1_bss_3B0[other].pos.z - lbl_1_bss_3B0[no].pos.z;
    lbl_1_bss_3B0[no].fB4 = M549_Mag2D(d.x, d.z);
    d.x = lbl_1_bss_3B0[no].pos.x;
    d.z = lbl_1_bss_3B0[no].pos.z;
    lbl_1_bss_3B0[no].fB8 = M549_Mag2D(d.x, d.z);

    if (lbl_1_bss_3B0[no].state2 == 0) {
        for (i = 0; i < 10; i++) {
            if (lbl_1_bss_158[i].active != 0 && (no ^ 1) == lbl_1_bss_158[i].side && fn_1_F4A0(no, i) != 0
                && lbl_1_bss_3B0[no].sCC > (rnd = frandmod(100)) && fn_1_E90C(no, i) == 1) {
                lbl_1_bss_3B0[no].s80 = 1;
                lbl_1_bss_3B0[no].s84 = 0;
            }
        }
    }

    switch (lbl_1_bss_3B0[no].sB2) {
        case 0:
            if (lbl_1_bss_3B0[no].state2 == 0) {
                lbl_1_bss_3B0[no].s7A = 0;
                if (lbl_1_bss_3B0[no].fB4 <= 120.0f) {
                    if ((rnd = frandmod(2)) != 0) {
                        if (lbl_1_bss_3B0[(s16)(no ^ 1)].state2 == 1) {
                            lbl_1_bss_3B0[no].sB0 = 30;
                            lbl_1_bss_3B0[no].s80 = 0;
                            lbl_1_bss_3B0[no].sB2 = 7;
                        } else {
                            lbl_1_bss_3B0[no].s80 = 0;
                            lbl_1_bss_3B0[no].sB2 = 5;
                        }
                    }
                } else if (lbl_1_bss_3B0[no].sD2 > (rnd = frandmod(100)) && lbl_1_bss_3B0[no].fB8 >= 400.0f) {
                    lbl_1_bss_3B0[no].s80 = 1;
                    lbl_1_bss_3B0[no].s78 = 0;
                    lbl_1_bss_3B0[no].target.x = 0.0f;
                    lbl_1_bss_3B0[no].target.z = 0.0f;
                    lbl_1_bss_3B0[no].sB2 = 8;
                } else {
                    if ((rnd = frandmod(2)) != 0) {
                        if (lbl_1_bss_3B0[no].sCA > (rnd = frandmod(100))) {
                            range = 0x79 - lbl_1_bss_3B0[no].sCE;
                            if (range > 0) {
                                rnd = lbl_1_bss_3B0[no].sD0 + frandmod(range);
                            } else {
                                rnd = lbl_1_bss_3B0[no].sD0;
                            }
                            if (rnd >= 0x78) {
                                rnd = 0x78;
                            }
                            lbl_1_bss_3B0[no].sB0 = rnd;
                            lbl_1_bss_3B0[no].s80 = 0;
                            lbl_1_bss_3B0[no].sB2 = 7;
                            break;
                        }
                    } else if (lbl_1_bss_3B0[no].sCA > (rnd = frandmod(100))) {
                        range = 0x79 - lbl_1_bss_3B0[no].sCE;
                        if (range > 0) {
                            rnd = lbl_1_bss_3B0[no].sD0 + frandmod(range);
                        } else {
                            rnd = lbl_1_bss_3B0[no].sD0;
                        }
                        if (rnd >= 0x78) {
                            rnd = 0x78;
                        }
                        lbl_1_bss_3B0[no].sB0 = rnd;
                        done = 0;
                        v = lbl_1_bss_3B0[no].pos;
                        do {
                            rot = (f32)frandmod(360);
                            dist = (f32)frandmod(395);
                            v.x = (f32)(dist * HuCos(rot));
                            v.z = (f32)(dist * HuSin(rot));
                        } while (!(M549_Mag2D(v.x, v.z) < lbl_1_data_498));
                        lbl_1_bss_3B0[no].target = v;
                        done = 1;
                        lbl_1_bss_3B0[no].s80 = 1;
                        lbl_1_bss_3B0[no].s84 = 0;
                        lbl_1_bss_3B0[no].sB2 = 10;
                        break;
                    }
                    if (lbl_1_bss_3B0[no].sC8 > (rnd = frandmod(100))) {
                        done = 0;
                        w = lbl_1_bss_3B0[no].pos;
                        do {
                            rot = (f32)frandmod(360);
                            dist = (f32)frandmod(395);
                            w.x = (f32)(dist * HuCos(rot));
                            w.z = (f32)(dist * HuSin(rot));
                        } while (!(M549_Mag2D(w.x, w.z) < lbl_1_data_498));
                        lbl_1_bss_3B0[no].target = w;
                        done = 1;
                        lbl_1_bss_3B0[no].s80 = 1;
                        lbl_1_bss_3B0[no].s84 = 0;
                        lbl_1_bss_3B0[no].sB2 = 6;
                    } else if (lbl_1_bss_3B0[no].sD4 > (rnd = frandmod(100))) {
                        lbl_1_bss_3B0[no].sB2 = 8;
                    }
                }
            }
            break;
        case 5:
            lbl_1_bss_3B0[no].fC0 = 0.0f;
            if (fn_1_D9E0(no) != 0) {
                lbl_1_bss_3B0[no].sB2 = 0;
            }
            break;
        case 6:
            if (lbl_1_bss_3B0[no].s84 != 0 || lbl_1_bss_3B0[no].s6A != 0 || lbl_1_bss_3B0[no].s6C != 0
                || lbl_1_bss_3B0[no].s7A > 0x3C) {
                lbl_1_bss_3B0[no].s80 = 0;
                lbl_1_bss_3B0[no].sB2 = 0;
            }
            break;
        case 7:
            if (lbl_1_bss_3B0[no].fB4 <= 150.0f) {
                lbl_1_bss_3B0[no].fC0 = 0.0f;
                lbl_1_bss_3B0[no].sB0 = 0;
            } else if (lbl_1_bss_3B0[no].fB8 >= 500.0f) {
                lbl_1_bss_3B0[no].fC0 = 20.0f;
                lbl_1_bss_3B0[no].sB0 = 0;
            } else {
                lbl_1_bss_3B0[no].fC0 = 20.0f;
            }
            if (fn_1_D0C4(no, lbl_1_bss_3B0[no].sB0) != 0) {
                lbl_1_bss_3B0[no].sB2 = 0;
            }
            break;
        case 8:
            if (lbl_1_bss_3B0[no].s78++ > 0x28) {
                lbl_1_bss_3B0[no].s78 = 0;
                lbl_1_bss_3B0[no].sB2 = 0;
            }
            break;
        case 9:
            if (lbl_1_bss_3B0[no].s84 != 0 || lbl_1_bss_3B0[no].s6A == 0 || lbl_1_bss_3B0[no].s6C == 0) {
                lbl_1_bss_3B0[no].s78 = 0;
                lbl_1_bss_3B0[no].sB2 = 0;
            }
            break;
        case 10:
            if (lbl_1_bss_3B0[no].fB4 <= 150.0f) {
                lbl_1_bss_3B0[no].fC0 = 0.0f;
                lbl_1_bss_3B0[no].sB0 = 0;
            } else if (lbl_1_bss_3B0[no].fB8 >= 500.0f) {
                lbl_1_bss_3B0[no].fC0 = 20.0f;
                lbl_1_bss_3B0[no].sB0 = 0;
            } else {
                lbl_1_bss_3B0[no].fC0 = 20.0f;
            }
            if (fn_1_D0C4(no, lbl_1_bss_3B0[no].sB0) != 0) {
                lbl_1_bss_3B0[no].s80 = 0;
                lbl_1_bss_3B0[no].sB2 = 0;
            }
            break;
    }

    if (lbl_1_bss_3B0[no].s80 != 0) {
        done = 0;
        if (lbl_1_bss_3B0[no].s6A == 0 && lbl_1_bss_3B0[no].s6C == 0) {
            switch (lbl_1_bss_3B0[no].s7C) {
                case 0:
                    MgPlayerComStkOn(lbl_1_bss_0.player[no]);
                    lbl_1_bss_3B0[no].s7C++;
                    break;
                case 1:
                    if (lbl_1_bss_3B0[no].s28 == 0) {
                        lbl_1_bss_3B0[no].chaseSpeed = lbl_1_bss_3B0[no].fC4 * lbl_1_data_484[lbl_1_bss_3B0[no].s60];
                        if (MgPlayerVecChase(lbl_1_bss_0.player[no], &lbl_1_bss_3B0[no].target,
                                             lbl_1_bss_3B0[no].chaseSpeed, lbl_1_bss_3B0[no].chaseRadius)) {
                            done = 1;
                        }
                    }
                    break;
                case 2:
                    MgPlayerComStkOff(lbl_1_bss_0.player[no]);
                    lbl_1_bss_3B0[no].s7C = 0;
                    done = 1;
                    break;
            }
        }
        lbl_1_bss_3B0[no].s84 = done;
    }
    if (lbl_1_bss_3B0[no].sAE != 0) {
        if (lbl_1_bss_3B0[no].sAC != 0) {
        } else if (lbl_1_bss_3B0[no].state == 1) {
            lbl_1_bss_3B0[no].btnA |= 0x100;
        } else {
            lbl_1_bss_3B0[no].btnB |= 0x100;
        }
    }
    return ret;
}

