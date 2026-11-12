// m502dll/ending: the epilogue/ending sequence: pad rumble cues and the
// crowd/applause gate that plays them.
#include "REL/m502dll.h"

float Hu3DMotionMaxTimeGet(HU3DMODELID modelId);
void fn_1_C1EC(M502MAIN *m, f32 amt);
void fn_1_C3C4(s32 padNo, s32 tier, s32 playerNo);

static Vec lbl_1_data_1068 = { 0.0f, 4000.0f, 6476.0f };
static Vec lbl_1_data_1074 = { 0.0f, 1.0f, 0.0f };
static Vec lbl_1_data_1080 = { 13.0f, 2029.0f, -1070.0f };
static Vec lbl_1_data_108C = { 0.0f, 6544.0f, 6476.0f };
static Vec lbl_1_data_1098 = { 0.0f, 1.0f, 0.0f };
static Vec lbl_1_data_10A4 = { 13.0f, 1829.0f, -1070.0f };
static Vec lbl_1_data_10B0 = { 156.0f, 5698.0f, 872.0f };
static Vec lbl_1_data_10BC = { 0.0f, 1.0f, 0.0f };
static Vec lbl_1_data_10C8 = { -77.0f, 1388.0f, -800.0f };
static s16 lbl_1_data_10D4 = -1;
static Vec lbl_1_data_10D8 = { 0.0f, 4000.0f, 500.0f };
static Vec lbl_1_data_10E4 = { -0.25f, -1.0f, -0.5f };
static Vec lbl_1_data_10F0 = { 500.0f, 2500.0f, 500.0f };
static Vec lbl_1_data_10FC = { -0.3f, 1.0f, -0.3f };

static f32 lbl_1_bss_5088;
static f32 lbl_1_bss_5084;
static f32 lbl_1_bss_5080;
static f32 lbl_1_bss_507C;
static f32 lbl_1_bss_5078;
static f32 lbl_1_bss_5074;
static s32 lbl_1_bss_5070;
static s32 lbl_1_bss_506C;
static s32 lbl_1_bss_5068;

s32 fn_1_A914(M502MAIN *m)
{
    M502PLRSUB *plr = &m->player[m->firstPlayer].plr;
    M502OPENING *op = &m->opening;
    M502MODELS *mdl = &m->models;
    M502CAMERA *cam = &m->player[0].camera;
    M502STAGE *stg[4];
    M502BLOCK *blk[4];
    s32 p;
    s32 i;
    s32 lvl;
    s32 idx;
    f32 v;
    f32 t;

    switch (m->unkC) {
    case 0:
        lbl_1_data_10D4 = Hu3DLLightCreate(mdl->model[0x78], 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        Hu3DLLightPointSet(mdl->model[0x78], lbl_1_data_10D4, 1000.0f, 1.0f, (GXDistAttnFn)1);
        Hu3DLLightPosSetV(mdl->model[0x78], lbl_1_data_10D4, &lbl_1_data_10F0, &lbl_1_data_10FC);
        Hu3DLLightColorSet(mdl->model[0x78], lbl_1_data_10D4, 0x80, 0x80, 0x80, 0xFF);
        Hu3DLLightStaticSet(mdl->model[0x78], lbl_1_data_10D4, 1);
        Hu3DGLightPosSetV(m->light.id, &lbl_1_data_10D8, &lbl_1_data_10E4);
        fn_1_1CB4(cam, &lbl_1_data_1068, &lbl_1_data_1074, &lbl_1_data_1080);
        plr->obj->trans.x = 0.0f;
        plr->obj->trans.y = 1300.0f;
        plr->obj->trans.z = -300.0f;
        plr->obj->rot.z = 0.0f;
        plr->obj->rot.y = 0.0f;
        plr->obj->rot.x = -45.0f;
        Hu3DMotionShiftSet(*plr->obj->mdlId, plr->obj->mtnId[5], 0.0f, 0.0f, 0x40000001);
        Hu3DMotionTimeSet(*plr->obj->mdlId, 0.0f);
        Hu3DMotionSpeedSet(*plr->obj->mdlId, 0.0f);
        Hu3DModelAttrSet(*m->player[0].plr.obj->mdlId, 1);
        Hu3DModelAttrSet(*m->player[1].plr.obj->mdlId, 1);
        Hu3DModelAttrSet(*m->player[2].plr.obj->mdlId, 1);
        Hu3DModelAttrSet(*m->player[3].plr.obj->mdlId, 1);
        Hu3DModelAttrReset(*plr->obj->mdlId, 1);
        Hu3DModelLayerSet(*plr->obj->mdlId, 5);
        Hu3DModelCameraSet(*plr->obj->mdlId, (u16)lbl_1_data_B60[4]);
        for (p = 0; p < 4; p++) {
            stg[p] = &m->player[p].stage;
            blk[p] = &m->player[p].block;
            for (i = 0; i < 5; i++) {
                Hu3DModelAttrSet(stg[p]->wall[i], 1);
            }
        }
        for (lvl = 0; lvl < 3; lvl++) {
            for (idx = 0; idx < 27; idx++) {
                Hu3DModelAttrSet(blk[0]->cell[lvl][idx].unk1C, 1);
                Hu3DModelAttrSet(blk[1]->cell[lvl][idx].unk1C, 1);
                Hu3DModelAttrSet(blk[2]->cell[lvl][idx].unk1C, 1);
                Hu3DModelAttrSet(blk[3]->cell[lvl][idx].unk1C, 1);
            }
        }
        espPosSet(op->esp[0], 576.0f, 240.0f);
        espPosSet(op->esp[1], 576.0f, 240.0f);
        espPosSet(op->esp[2], 576.0f, 240.0f);
        Hu3DCameraViewportSet(1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
        Hu3DCameraPerspectiveSet(1, 20.0f, 20.0f, 30000.0f, 1.2f);
        Hu3DCameraScissorSet(1, 0, 0, 640, 480);
        Hu3DCameraViewportSet(0x10, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
        Hu3DCameraPerspectiveSet(0x10, 20.0f, 20.0f, 30000.0f, 1.2f);
        Hu3DCameraScissorSet(0x10, 0, 0, 640, 480);
        for (i = 1; i < 4; i++) {
            Hu3DCameraViewportSet(lbl_1_data_B60[i], 0.0f, 0.0f, 0.0f, 480.0f, 0.0f, 1.0f);
            Hu3DCameraPerspectiveSet(lbl_1_data_B60[i], 20.0f, 20.0f, 30000.0f, 0.0f);
            Hu3DCameraScissorSet(lbl_1_data_B60[i], 0, 0, 0, 480);
            Hu3DCameraViewportSet(lbl_1_data_B60[i + 4], 0.0f, 0.0f, 0.0f, 480.0f, 0.0f, 1.0f);
            Hu3DCameraPerspectiveSet(lbl_1_data_B60[i + 4], 20.0f, 20.0f, 30000.0f, 0.0f);
            Hu3DCameraScissorSet(lbl_1_data_B60[i + 4], 0, 0, 0, 480);
        }
        lbl_1_bss_507C = 0.0f;
        fn_1_3380(0.002f);
        Hu3DModelAttrSet(mdl->model[0x77], 0x40000040);
        Hu3DMotionShapeSpeedSet(mdl->model[0x77], lbl_1_bss_507C);
        Hu3DMotionShapeTimeSet(mdl->model[0x77], 0.01f);
        Hu3DModelAttrReset(mdl->model[0x77], 1);
        Hu3DModelCameraSet(mdl->model[0x77], (u16)lbl_1_data_B60[4]);
        Hu3DModelMatHookSet(mdl->model[0x77], (HU3DMATHOOK)fn_1_3390);
        Hu3DModelLayerSet(mdl->model[0x77], 1);
        Hu3DModelLayerSet(mdl->model[0x78], 5);
        Hu3DModelAttrReset(mdl->model[0x78], 1);
        HuAudAUXSet(0, 1);
        HuAudAUXVolSet(-1, -1);
        CharModelVoicePanSet((s16)plr->charNo, 0x40);
        m->timer = 0;
        m->unkC = 1;
        break;
    case 1:
        if (WipeCheckEnd() == 0) {
            if (m->seId >= 0) {
                fn_1_1C0(m->seId);
                m->seId = -1;
            }
            if (m->seId < 0) {
                m->seId = fn_1_198(0x3A1);
            }
            m->unkC = 2;
        }
        break;
    case 2:
        m->timer++;
        if ((f32)m->timer >= 0.0f) {
            m->timer = 0;
            fn_1_1E00(cam, &lbl_1_data_108C, &lbl_1_data_1098, &lbl_1_data_10A4);
            m->unkC = 3;
        }
        break;
    case 3:
        m->timer++;
        v = 0.3f * (f32)m->timer;
        if (v > 20.0f) {
            v = 20.0f;
        }
        if ((f32)m->timer > 30.0f) {
            fn_1_1E64(cam, &lbl_1_data_1098, 60.0f);
        }
        fn_1_C1EC(m, v);
        if ((f32)m->timer > 120.0f) {
            fn_1_1CB4(cam, &lbl_1_data_108C, &lbl_1_data_1098, &lbl_1_data_10A4);
            m->unkC = 4;
            m->timer = 0;
        }
        break;
    case 4:
        m->timer++;
        fn_1_C1EC(m, 60.0f);
        if (m->timer >= 8) {
            m->unkC = 5;
            m->timer = 0;
        }
        break;
    case 5:
        if (m->unk5020 >= 0) {
            fn_1_1C0(m->unk5020);
            m->unk5020 = -1;
        }
        if (m->seId >= 0) {
            fn_1_1C0(m->seId);
            m->seId = -1;
        }
        fn_1_198(0x633);
        if (m->seId < 0) {
            m->seId = fn_1_198(0x634);
        }
        Hu3DModelPosSet(mdl->model[0x72], 0.0f, 0.0f, 0.0f);
        Hu3DModelRotSet(mdl->model[0x72], 0.0f, 0.0f, 0.0f);
        Hu3DModelCameraSet(mdl->model[0x72], (u16)lbl_1_data_B60[4]);
        Hu3DMotionSpeedSet(mdl->model[0x72], 1.0f);
        Hu3DMotionTimeSet(mdl->model[0x72], 0.01f);
        Hu3DModelLayerSet(mdl->model[0x72], 5);
        Hu3DModelAttrReset(mdl->model[0x72], 1);
        fn_1_3380(0.008f);
        lbl_1_bss_507C = 2.0f;
        Hu3DMotionShapeSpeedSet(mdl->model[0x77], lbl_1_bss_507C);
        fn_1_1E00(cam, &lbl_1_data_10B0, &lbl_1_data_10BC, &lbl_1_data_10C8);
        lbl_1_bss_5080 = 1700.0f / Hu3DMotionMaxTimeGet(mdl->model[0x72]);
        m->timer = 0;
        m->unkC = 6;
        break;
    case 6:
        fn_1_1CB4(cam, &lbl_1_data_108C, &lbl_1_data_1098, &lbl_1_data_10A4);
        t = Hu3DData[mdl->model[0x72]].motWork.time;
        if (t >= Hu3DMotionMaxTimeGet(mdl->model[0x72])) {
            lbl_1_bss_5080 = lbl_1_bss_5080 * 0.64f;
            Hu3DModelAttrSet(mdl->model[0x72], 1);
            Hu3DModelPosSet(mdl->model[0x73], 0.0f, 0.0f, 0.0f);
            Hu3DModelRotSet(mdl->model[0x73], 0.0f, 0.0f, 0.0f);
            Hu3DModelCameraSet(mdl->model[0x73], (u16)lbl_1_data_B60[4]);
            Hu3DMotionShapeSpeedSet(mdl->model[0x73], 1.0f);
            Hu3DMotionShapeTimeSet(mdl->model[0x73], 0.01f);
            Hu3DModelAttrSet(mdl->model[0x73], 0x40000041);
            Hu3DModelLayerSet(mdl->model[0x73], 5);
            Hu3DModelAttrReset(mdl->model[0x73], 1);
            Hu3DModelPosSet(mdl->model[0x74], 0.0f, 0.0f, 0.0f);
            Hu3DModelRotSet(mdl->model[0x74], 0.0f, 0.0f, 0.0f);
            Hu3DModelCameraSet(mdl->model[0x74], (u16)lbl_1_data_B60[4]);
            Hu3DMotionShapeSpeedSet(mdl->model[0x74], 1.0f);
            Hu3DMotionShapeTimeSet(mdl->model[0x74], 0.01f);
            Hu3DModelLayerSet(mdl->model[0x74], 5);
            Hu3DModelAttrSet(mdl->model[0x74], 0x40000041);
            Hu3DModelAttrReset(mdl->model[0x74], 1);
            Hu3DModelPosSet(mdl->model[0x75], 0.0f, -200.0f, 0.0f);
            Hu3DModelRotSet(mdl->model[0x75], 0.0f, 0.0f, 0.0f);
            Hu3DModelCameraSet(mdl->model[0x75], (u16)lbl_1_data_B60[4]);
            Hu3DMotionShapeSpeedSet(mdl->model[0x75], 1.0f);
            Hu3DMotionShapeTimeSet(mdl->model[0x75], 0.01f);
            Hu3DModelLayerSet(mdl->model[0x75], 6);
            Hu3DModelAttrSet(mdl->model[0x75], 0x40000041);
            Hu3DModelLayerSet(*plr->obj->mdlId, 7);
            lbl_1_bss_5078 = -125.0f;
            lbl_1_bss_5074 = 40.0f;
            lbl_1_bss_5070 = 0xB4;
            lbl_1_bss_5088 = 180.0f;
            lbl_1_bss_5084 = 0.0f;
            m->timer = 0;
            m->unkC = 7;
        }
        break;
    case 7:
        t = Hu3DData[mdl->model[0x75]].motShapeWork.time;
        if (t >= 1.0f + Hu3DMotionShapeMaxTimeGet(mdl->model[0x75]) * 0.5f) {
            Hu3DModelPosSet(mdl->model[0x76], 0.0f, -200.0f, 0.0f);
            Hu3DModelRotSet(mdl->model[0x76], 0.0f, 0.0f, 0.0f);
            Hu3DModelCameraSet(mdl->model[0x76], (u16)lbl_1_data_B60[4]);
            Hu3DMotionShapeSpeedSet(mdl->model[0x76], 1.0f);
            Hu3DMotionShapeTimeSet(mdl->model[0x76], 0.01f);
            Hu3DModelLayerSet(mdl->model[0x76], 6);
            Hu3DModelAttrSet(mdl->model[0x76], 0x40000041);
            m->timer = 0;
            m->unkC = 8;
        }
        break;
    case 9:
        m->timer++;
        if (m->timer >= 0xD2) {
            m->unkC = 0xA;
            m->timer = 0;
            m->mesId = -1;
        }
        break;
    case 10:
        m->timer++;
        m->mesId = -1;
        WipeCreate(2, 6, 0x3C);
        if (m->seId >= 0) {
            fn_1_1E8(m->seId, 0x3E8);
            m->seId = -1;
        }
        m->unkC = 0xB;
        break;
    case 11:
        if (WipeCheckEnd() == 0) {
            Hu3DLLightKill(mdl->model[0x78], lbl_1_data_10D4);
            return 1;
        }
        break;
    default:
        break;
    }
    if (m->unkC >= 6 && m->unkC <= 9) {
        lbl_1_bss_5078 += 1.8f;
        if (lbl_1_bss_5078 > 0.0f) {
            lbl_1_bss_5078 = 0.0f;
        }
        if (fn_1_1E64(cam, &lbl_1_data_10BC, 60.0f) != 0) {
            fn_1_1CB4(cam, &lbl_1_data_10B0, &lbl_1_data_10BC, &lbl_1_data_10C8);
        }
    }
    if (m->unkC == 6) {
        plr->obj->trans.y = plr->obj->trans.y + lbl_1_bss_5080;
    }
    else if (m->unkC > 6) {
        if (m->unkC < 9) {
            lbl_1_bss_5080 -= 3.5f;
            if (lbl_1_bss_5080 < -70.0f) {
                lbl_1_bss_5080 = -70.0f;
            }
            plr->obj->trans.y = plr->obj->trans.y + lbl_1_bss_5080;
            if (plr->obj->trans.y < 3200.0f && lbl_1_bss_5080 < 0.0f) {
                Hu3DMotionSpeedSet(*plr->obj->mdlId, 1.0f);
                m->mesId = GameMesCreate(4, 3, plr->charNo, -1, -1, -1);
                plr->obj->trans.y = 3200.0f;
                m->timer = 0;
                m->unkC = 9;
                lbl_1_bss_5080 = 0.0f;
                m->unk5020 = CharFXPlay((s16)plr->charNo, 0x1FA);
                m->bgmId = fn_1_340(0x79);
            }
        }
        else {
            lbl_1_bss_5080 = lbl_1_bss_5080 * 0.75f;
            lbl_1_bss_5070++;
            lbl_1_bss_5088 = (360.0f * (f32)lbl_1_bss_5070) / Hu3DMotionMaxTimeGet(mdl->model[0x74]);
            if (lbl_1_bss_5088 < 360.0f) {
                lbl_1_bss_5088 -= 180.0f;
            }
            lbl_1_bss_5084 = lbl_1_bss_5074 * HuSin(100.0f + lbl_1_bss_5088);
            if (((f32)lbl_1_bss_5070 - 180.0f) < 25.0f) {
                lbl_1_bss_5080 = -100.0 * HuSin(180.0f * (((f32)lbl_1_bss_5070 - 180.0f) / 25.0f));
            }
            plr->obj->trans.y = 3200.0f;
            plr->obj->trans.y = plr->obj->trans.y + lbl_1_bss_5084;
            plr->obj->trans.y = plr->obj->trans.y + lbl_1_bss_5080;
        }
    }
    if (m->unkC >= 6) {
        t = Hu3DData[mdl->model[0x74]].motShapeWork.time;
        lbl_1_bss_5088 = 180.0f + (360.0f * t) / Hu3DMotionMaxTimeGet(mdl->model[0x74]);
        if (lbl_1_bss_5088 > 360.0f) {
            lbl_1_bss_5088 -= 360.0f;
        }
        Hu3DData[mdl->model[0x75]].pos.y = lbl_1_bss_5078 + 75.0 * HuCos(lbl_1_bss_5088);
        Hu3DData[mdl->model[0x76]].pos.y = lbl_1_bss_5078 + 75.0 * HuCos(lbl_1_bss_5088);
    }
    if (m->unkC >= 8) {
        t = Hu3DData[mdl->model[0x77]].motShapeWork.time;
        lbl_1_bss_507C = 0.025f * (Hu3DMotionShapeMaxTimeGet(mdl->model[0x77]) - t);
        if (lbl_1_bss_507C > 2.0f) {
            lbl_1_bss_507C = 2.0f;
        }
        Hu3DMotionShapeSpeedSet(mdl->model[0x77], lbl_1_bss_507C);
    }
    return 0;
}

void fn_1_C1EC(M502MAIN *m, f32 amt)
{
    M502CAMERA *cam = &m->player[0].camera;

    if (amt > 0.0f) {
        if (amt < 20.0f) {
            if (lbl_1_bss_506C % 6 == 0) {
                fn_1_C3C4(m->firstPlayer, 0, 2);
            }
        }
        else if (amt < 50.0f) {
            if (lbl_1_bss_506C % 6 == 0) {
                fn_1_C3C4(m->firstPlayer, 1, 4);
            }
        }
        else {
            fn_1_C3C4(m->firstPlayer, 2, 0xC);
        }
    }
    if (lbl_1_bss_5068 < 2) {
        cam->pos.y = cam->pos.y - amt;
        cam->posGoal.y = cam->pos.y;
        cam->tgt.y = cam->tgt.y - amt;
        cam->tgtGoal.y = cam->tgt.y;
    }
    else {
        cam->pos.y = cam->pos.y + amt;
        cam->posGoal.y = cam->pos.y;
        cam->tgt.y = cam->tgt.y + amt;
        cam->tgtGoal.y = cam->tgt.y;
    }
    lbl_1_bss_5068 += 1;
    lbl_1_bss_506C += 1;
    lbl_1_bss_5068 = lbl_1_bss_5068 % 4;
}

// Maps a crowd-intensity tier onto a pad rumble: 0 = the light "cheer" buzz,
// 1 = a shorter pulse, 2 = a long low one. Tier 3 or anything else is silent.
void fn_1_C3C4(s32 padNo, s32 tier, s32 playerNo)
{
    switch (tier) {
    case 0:
        omVibrate((s16)padNo, (s16)playerNo, 6, 6);
        break;
    case 1:
        omVibrate((s16)padNo, (s16)playerNo, 4, 2);
        break;
    case 2:
        omVibrate((s16)padNo, (s16)playerNo, 0xC, 0);
        break;
    }
}
