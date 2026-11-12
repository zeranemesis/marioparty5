#include "REL/m502dll.h"

static Vec lbl_1_data_FE0 = { 0.0f, -100.0f, -150.0f };
static Vec lbl_1_data_FEC = { 0.0f, 1.0f, 0.0f };
static Vec lbl_1_data_FF8 = { 0.0f, 1400.0f, -150.5f };
static Vec lbl_1_data_1004 = { 0.0f, 300.0f, 3400.5f };
static Vec lbl_1_data_1010 = { 0.0f, 1.0f, 0.0f };
static Vec lbl_1_data_101C = { 0.0f, -70.0f, -150.0f };
static Vec lbl_1_data_1028 = { 0.0f, 300.0f, 4550.0f };
static Vec lbl_1_data_1034 = { 0.0f, 4000.0f, 500.0f };
static Vec lbl_1_data_1040[3] = {
    { 0.0f, -1.0f, -1.0f },
    { 50.0f, 1400.0f, -200.0f },
    { -1.0f, -1.0f, -1.0f },
};

static f32 lbl_1_bss_5050;
static f32 lbl_1_bss_5054;
static f32 lbl_1_bss_5058;
static f32 lbl_1_bss_505C;
static f32 lbl_1_bss_5060;

void fn_1_9468(M502OPENING *o, M502MAIN *m)
{
    memset(o, 0, sizeof(M502OPENING));
    o->main = m;
    o->esp[0] = espEntry(0x3C0090, 12, 0);
    espDispOff(o->esp[0]);
    espDrawNoSet(o->esp[0], 0);
    espPosSet(o->esp[0], 576.0f, 240.0f);
    espAttrSet(o->esp[0], 1);
    o->esp[1] = espEntry(0x3C0090, 12, 0);
    espDispOff(o->esp[1]);
    espDrawNoSet(o->esp[1], 0);
    espPosSet(o->esp[1], 576.0f, 240.0f);
    espAttrSet(o->esp[1], 1);
    o->esp[2] = espEntry(0x3C0090, 12, 0);
    espDispOff(o->esp[2]);
    espDrawNoSet(o->esp[2], 0);
    espPosSet(o->esp[2], 576.0f, 240.0f);
    espAttrSet(o->esp[2], 1);
}

void fn_1_95B4(M502OPENING *o)
{
    espKill(o->esp[0]);
    espKill(o->esp[1]);
    espKill(o->esp[2]);
    memset(o, 0, sizeof(M502OPENING));
}

void fn_1_9604(M502OPENING *o)
{
    espDispOn(o->esp[0]);
    espDispOn(o->esp[1]);
    espDispOn(o->esp[2]);
}

/* intro: drop player 1 in, then fold the split screen into four viewports */
s32 fn_1_9644(M502MAIN *m)
{
    M502PLRSUB *p0;
    M502STAGE *stage;
    M502PLAYER *p;
    Vec dust;
    Vec dustPos;
    f32 v;
    f32 f;
    s32 i;
    s16 j;

    p0 = &m->player[0].plr;
    stage = &m->player[0].stage;
    if (m->unkC != 6) {
        if (m->unkC < 6) {
            switch (m->unkC) {
            case 0:
                Hu3DGLightPosSetV(m->light.id, &lbl_1_data_1034, &lbl_1_data_1040[0]);
                Hu3DModelAttrSet(m->player[1].plr.obj->mdlId[0], 1);
                Hu3DModelAttrSet(m->player[2].plr.obj->mdlId[0], 1);
                Hu3DModelAttrSet(m->player[3].plr.obj->mdlId[0], 1);
                Hu3DCameraViewportSet(1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
                Hu3DCameraPerspectiveSet(1, 20.0f, 20.0f, 30000.0f, 1.2f);
                Hu3DCameraScissorSet(1, 0, 0, 640, 480);
                fn_1_1CB4(&m->player[0].camera, &lbl_1_data_FE0, &lbl_1_data_FEC, &lbl_1_data_FF8);
                Hu3DCameraViewportSet(2, 640.0f, 0.0f, 0.0f, 480.0f, 0.0f, 1.0f);
                Hu3DCameraPerspectiveSet(2, 20.0f, 20.0f, 30000.0f, 0.0f);
                Hu3DCameraScissorSet(2, 640, 0, 0, 480);
                fn_1_1CB4(&m->player[1].camera, &lbl_1_data_1004, &lbl_1_data_1010, &lbl_1_data_101C);
                Hu3DCameraViewportSet(4, 640.0f, 0.0f, 0.0f, 480.0f, 0.0f, 1.0f);
                Hu3DCameraPerspectiveSet(4, 20.0f, 20.0f, 30000.0f, 0.0f);
                Hu3DCameraScissorSet(4, 320, 0, 0, 480);
                fn_1_1CB4(&m->player[2].camera, &lbl_1_data_1004, &lbl_1_data_1010, &lbl_1_data_101C);
                Hu3DCameraViewportSet(8, 640.0f, 0.0f, 0.0f, 480.0f, 0.0f, 1.0f);
                Hu3DCameraPerspectiveSet(8, 20.0f, 20.0f, 30000.0f, 0.0f);
                Hu3DCameraScissorSet(8, 640, 0, 0, 480);
                fn_1_1CB4(&m->player[3].camera, &lbl_1_data_1004, &lbl_1_data_1010, &lbl_1_data_101C);
                p0->obj->trans.y = 16800.0f;
                Hu3DMotionSet(p0->obj->mdlId[0], p0->obj->mtnId[4]);
                Hu3DData[p0->obj->mdlId[0]].motAttr &= ~HU3D_MOTATTR_LOOP;
                Hu3DMotionTimeSet(p0->obj->mdlId[0], 0.0f);
                Hu3DMotionSpeedSet(p0->obj->mdlId[0], 0.0f);
                for (i = 0; i < 4; i++) {
                    m->player[i].plr.obj->trans.z = -150.0f;
                }
                m->unkC = 1;
                break;
            case 1:
                if (WipeCheckEnd() == 0) {
                    lbl_1_bss_5058 = 24.0f;
                    fn_1_198(0x636);
                    m->unkC = 2;
                }
                break;
            default:
                lbl_1_bss_5058 *= 1.04f;
                if (p0->obj->trans.y >= 2400.0f) {
                    if (lbl_1_bss_5058 >= 180.0f) {
                        lbl_1_bss_5058 = 180.0f;
                    }
                } else {
                    if (lbl_1_bss_5058 >= 60.000004f) {
                        lbl_1_bss_5058 = 60.000004f;
                    }
                }
                p0->obj->trans.y -= lbl_1_bss_5058;
                if (m->unkC == 2) {
                    if (p0->obj->trans.y < 3800.0f) {
                        fn_1_1E00(&m->player[0].camera, &lbl_1_data_1004, &lbl_1_data_1010, &lbl_1_data_101C);
                        lbl_1_bss_5054 = 0.0f;
                        m->unkC = 3;
                    }
                } else {
                    if (m->unkC == 3) {
                        if (p0->obj->trans.y < 0.0f) {
                            p0->obj->trans.y = 0.0f;
                            dust.x = 0.0f;
                            dust.y = 0.0f;
                            dust.z = 0.0f;
                            dust.x = p0->obj->trans.x;
                            dust.y = p0->obj->trans.y - 10.0f;
                            dust.z = p0->obj->trans.z;
                            dustPos = dust;
                            CharModelLandDustCreate((s16)p0->charNo, &dustPos);
                            CharFXPlay(p0->charNo, 0x96);
                            Hu3DModelAttrReset(p0->obj->mdlId[0], 0x40000001);
                            Hu3DMotionShiftSet(p0->obj->mdlId[0], p0->obj->mtnId[4], 0.0f, 0.0f, 0);
                            Hu3DMotionSpeedSet(p0->obj->mdlId[0], 1.0f);
                            m->unkC = 4;
                            fn_1_198(0x21);
                        }
                    } else {
                        if (m->unkC == 4) {
                            if (Hu3DMotionEndCheck(p0->obj->mdlId[0]) != 0) {
                                Hu3DMotionShiftSet(p0->obj->mdlId[0], p0->obj->mtnId[0], 0.0f, 16.0f, 0x40000001);
                                m->unkC = 5;
                            }
                        }
                        p0->obj->trans.y = 0.0f;
                    }
                    if (fn_1_1E64(&m->player[0].camera, &lbl_1_data_1010, 70.0f) != 0) {
                        lbl_1_bss_5054 = 0.0f;
                        fn_1_1CB4(&m->player[0].camera, &lbl_1_data_1004, &lbl_1_data_1010, &lbl_1_data_101C);
                        lbl_1_bss_505C = 0.0f;
                        Hu3DModelAttrReset(m->player[1].plr.obj->mdlId[0], 1);
                        Hu3DModelAttrReset(m->player[2].plr.obj->mdlId[0], 1);
                        Hu3DModelAttrReset(m->player[3].plr.obj->mdlId[0], 1);
                        m->unkC = 6;
                    }
                }
                break;
            }
            return 0;
        }
        if (m->unkC < 8) {
            fn_1_1E64(&m->player[0].camera, &lbl_1_data_1010, 30.0f);
            fn_1_1E64(&m->player[1].camera, &lbl_1_data_1010, 30.0f);
            fn_1_1E64(&m->player[2].camera, &lbl_1_data_1010, 30.0f);
            fn_1_1E64(&m->player[3].camera, &lbl_1_data_1010, 30.0f);
            if (m->mesId < 0) {
                m->mesId = GameMesCreate(2, 0);
                if (lbl_1_data_0->versus != 0) {
                    for (i = 0; i < 4; i++) {
                        p = &m->player[i];
                        p->timer = MgTimerCreate(2);
                        MgTimerPosSet(p->timer, 8.0f + (72.0f + 144.0f * (f32)p->no), 64.0f);
                        MgTimerDigitColorSet(p->timer, 0xFF, 0xFF, 0xFF);
                        MgTimerParamSet(p->timer, 0, 0, 0);
                        MgTimerModeOnSet(p->timer, 0);
                    }
                }
                return 0;
            }
            if (m->bgmId < 0) {
                m->bgmId = fn_1_318(0x3C);
            }
            if (GameMesStatGet(m->mesId) == 0) {
                m->state = 2;
                m->unkC = 0;
                m->mesId = -1;
                return 1;
            }
            return 0;
        }
        return 0;
    }
    lbl_1_bss_5060 = (f32)HuSin(90.0f * lbl_1_bss_505C);
    lbl_1_bss_5060 = 1.0f - 0.75f * (lbl_1_bss_5060 * lbl_1_bss_5060);
    lbl_1_bss_505C += 0.016666668f;
    if (lbl_1_bss_505C >= 1.0f) {
        lbl_1_bss_5060 = 0.25f;
        m->unkC = 7;
    }
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, 640.0f * lbl_1_bss_5060, 480.0f, 0.0f, 1.0f);
    Hu3DCameraPerspectiveSet(1, 20.0f, 20.0f, 30000.0f, 1.2f * lbl_1_bss_5060);
    Hu3DCameraScissorSet(1, 0, 0, (u32)(640.0f * lbl_1_bss_5060), 480);
    Hu3DCameraViewportSet(0x10, 0.0f, 0.0f, 640.0f * lbl_1_bss_5060, 480.0f, 0.0f, 1.0f);
    Hu3DCameraPerspectiveSet(0x10, 20.0f, 20.0f, 30000.0f, 1.2f * lbl_1_bss_5060);
    Hu3DCameraScissorSet(0x10, 0, 0, (u32)(640.0f * lbl_1_bss_5060), 480);
    for (j = 0; j < 3; j++) {
        v = (0.5f + 0.25f * (f32)j) - lbl_1_bss_5060;
        if (v < 0.0f) {
            v = 0.0f;
        }
        if (v > 0.25f) {
            v = 0.25f;
        }
        Hu3DCameraViewportSet(lbl_1_data_B60[j + 1], (320.0f + 160.0f * (f32)j) - 640.0f * v, 0.0f, 640.0f * v, 480.0f, 0.0f, 1.0f);
        Hu3DCameraPerspectiveSet(lbl_1_data_B60[j + 1], 20.0f, 20.0f, 30000.0f, 1.2f * v);
        Hu3DCameraScissorSet(lbl_1_data_B60[j + 1], (u32)((320.0f + 160.0f * (f32)j) - 640.0f * v), 0, (u32)(640.0f * v), 480);
        Hu3DCameraViewportSet(lbl_1_data_B60[j + 5], (320.0f + 160.0f * (f32)j) - 640.0f * v, 0.0f, 640.0f * v, 480.0f, 0.0f, 1.0f);
        Hu3DCameraPerspectiveSet(lbl_1_data_B60[j + 5], 20.0f, 20.0f, 30000.0f, 1.2f * v);
        Hu3DCameraScissorSet(lbl_1_data_B60[j + 5], (u32)((320.0f + 160.0f * (f32)j) - 640.0f * v), 0, (u32)(640.0f * v), 480);
        f = lbl_1_bss_5060;
        if (f < 0.25f + 0.25f * (f32)j) {
            f = 0.25f + 0.25f * (f32)j;
        }
        espPosSet(m->opening.esp[j], 576.0f * f, 240.0f);
        fn_1_1E00(&m->player[0].camera, &lbl_1_data_1028, &lbl_1_data_1010, &lbl_1_data_101C);
        fn_1_1E00(&m->player[1].camera, &lbl_1_data_1028, &lbl_1_data_1010, &lbl_1_data_101C);
        fn_1_1E00(&m->player[2].camera, &lbl_1_data_1028, &lbl_1_data_1010, &lbl_1_data_101C);
        fn_1_1E00(&m->player[3].camera, &lbl_1_data_1028, &lbl_1_data_1010, &lbl_1_data_101C);
    }
    return 0;
}
