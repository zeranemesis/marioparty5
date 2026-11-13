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
