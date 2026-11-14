#include "REL/m562dll.h"
#include "game/flag.h"
#include "game/frand.h"
#include "game/gamework.h"
#include <math.h>

float Hu3DMotionMaxTimeGet(HU3DMODELID modelId);

/* .rodata */
static const Vec lbl_1_rodata_E20 = { 0.0f, 0.0f, 0.0f };
static const Vec lbl_1_rodata_E2C = { 0.0f, 0.0f, 0.0f };

/* .data */
static Vec lbl_1_data_EA8[5] = {
    { 0.0f, 800.0f, 1400.0f },
    { 0.0f, 468.0f, 810.0f },
    { 0.0f, 1796.0f, 873.0f },
    { 0.0f, 1072.0f, 739.0f },
    { 0.0f, 400.0f, 1280.0f },
};
static Vec lbl_1_data_EE4[5] = {
    { 0.0f, 100.0f, -200.0f },
    { 0.0f, 210.0f, -1111.0f },
    { 0.0f, 483.0f, -1111.0f },
    { 0.0f, 483.0f, -1111.0f },
    { 0.0f, 85.0f, 7.0f },
};
static Vec lbl_1_data_F20 = { 0.0f, 0.0f, 500.0f };
static Vec lbl_1_data_F2C = { 0.0f, 0.0f, 0.0f };
static s32 lbl_1_data_F38 = 1;
static Vec lbl_1_data_F3C = { 0.0f, -3800.0f, -26000.0f };
static Vec lbl_1_data_F48 = { -680.0f, 370.0f, -2100.0f };
static s32 lbl_1_data_F54 = 1;
static Vec lbl_1_data_F58 = { 680.0f, 370.0f, -2100.0f };
static s32 lbl_1_data_F64 = 1;

/* .bss */
static s32 lbl_1_bss_79A0;
static Vec lbl_1_bss_79A4;
static Vec lbl_1_bss_79B0;
static Vec lbl_1_bss_79BC;
static Vec lbl_1_bss_79C8;
static Vec lbl_1_bss_79D4;
static Vec lbl_1_bss_79E0;
static Vec lbl_1_bss_79EC;

static f32 endRand3(void) {
    return 3.0f * ((f32) (frand() % 10000) / 10000.0f);
}

static f32 endClamp01(f32 v) {
    if (v > 1.0f) {
        return 1.0f;
    }
    if (v < 0.0f) {
        return 0.0f;
    }
    return v;
}

static void endCamSet(CAMERA *cam, s32 i, f32 speed) {
    cam->mode = 1;
    cam->posGoal = lbl_1_data_EA8[i];
    cam->tgtGoal = lbl_1_data_EE4[i];
    cam->prevMode = 0;
    cam->speed = speed;
    cam->slerpT = speed;
}

s32 fn_1_29DF8(M562Work *w) {
    M562Player *player = &w->player;
    M562PlayerInput *input = &player->input;
    M562Back *back = &w->back;
    CAMERA *cam = (CAMERA *) &w->camera[0];
    KoopaBoss *koopa = &w->boss;
    Vec dir;
    Vec pos;
    Vec v;
    f32 f;
    f32 g;
    f32 dist;
    f32 maxTime;
    f64 tmp;
    s16 model;

    switch (w->state) {
    case 0:
        fn_1_1D9C(w);
        player->sub = 0;
        player->state = 0;
        Hu3DModelAttrReset(w->stage.modelF, 1);
        cam->mode = 1;
        cam->posGoal = lbl_1_data_EA8[0];
        cam->tgtGoal = lbl_1_data_EE4[0];
        cam->prevMode = 0;
        cam->speed = 0.04f;
        cam->slerpT = 0.04f;
        lbl_1_bss_79EC = koopa->pos;
        lbl_1_data_F38 = 1;
        lbl_1_bss_79A0 = 0;
        lbl_1_bss_79E0 = lbl_1_data_F48;
        v = lbl_1_rodata_E20;
        lbl_1_bss_79D4 = v;
        lbl_1_bss_79C8.x = endRand3();
        lbl_1_bss_79C8.y = endRand3();
        lbl_1_bss_79C8.z = endRand3();
        lbl_1_data_F54 = 1;
        lbl_1_bss_79BC = lbl_1_data_F58;
        v = lbl_1_rodata_E2C;
        lbl_1_bss_79B0 = v;
        lbl_1_bss_79A4.x = endRand3();
        lbl_1_bss_79A4.y = endRand3();
        lbl_1_bss_79A4.z = endRand3();
        lbl_1_data_F64 = 1;
        w->evtTimer = 0;
        w->state++;
        /* fallthrough */
    case 1:
        w->evtTimer++;
        if (w->evtTimer >= 0x5A) {
            endCamSet(cam, 1, 0.02f);
            w->evtTimer = 0;
            w->state++;
        }
        break;
    case 2:
        if (w->seId < 0) {
            w->seId = fn_1_814(0x8B5);
        }
        fn_1_4748(back, 4, (f32) w->evtTimer / 180.0f);
        if ((w->evtTimer % 30) == 0) {
            fn_1_21B68(cam, 1, 0x32, 10.0f, 0.95f, 90.0f);
            omVibrate(player->playerNo, 10, 12, 0);
        }
        w->evtTimer++;
        if (w->evtTimer >= 0xB4) {
            endCamSet(cam, 2, 0.02f);
            fn_1_A6C(0x7C);
            koopa->reqMotion = 0x11;
            koopa->loopMotion = 0;
            w->evtTimer = 0;
            w->state++;
        }
        break;
    case 3:
        maxTime = Hu3DMotionMaxTimeGet(koopa->modelId);
        if (Hu3DMotionTimeGet(koopa->modelId) >= maxTime) {
            Hu3DMotionTimeSet(koopa->modelId, 13.0f);
        }
        f = (f32) w->evtTimer / 300.0f;
        if ((w->evtTimer % 20) == 0) {
            fn_1_21B68(cam, 1, 0x32, 10.0f, 0.95f, 90.0f);
            omVibrate(player->playerNo, 10, 12, 0);
        }
        if (w->evtTimer == 0x3C) {
            pos = koopa->pos;
            pos.y = -100.0f;
            fn_1_2DC08((s16) cam->camMask, &pos, 2);
        }
        koopa->pos.z = (f32) ((f64) koopa->pos.z + 80.0 * sin(M_PI * (f64) (200.0f * (360.0f * f)) / 180.0));
        w->evtTimer++;
        if (w->evtTimer >= 0x12C) {
            lbl_1_data_F2C.x = endRand3();
            lbl_1_data_F2C.y = endRand3();
            lbl_1_data_F2C.z = endRand3();
            koopa->reqMotion = 0xA;
            koopa->loopMotion = 0;
            endCamSet(cam, 3, 0.01f);
            w->evtTimer = 0;
            w->state++;
        }
        break;
    case 4:
        if (lbl_1_data_F38 != 0) {
            maxTime = Hu3DMotionMaxTimeGet(koopa->modelId);
            if (Hu3DMotionTimeGet(koopa->modelId) >= maxTime) {
                Hu3DMotionTimeSet(koopa->modelId, 48.0f);
            }
            dist = lbl_1_data_F3C.z - koopa->pos.z;
            if (dist < 0.0f) {
                dist = -dist;
            }
            f = endClamp01(dist / 22000.0f);
            f = 1.0f - f;
            if (f < 0.3f) {
                if ((w->evtTimer % 15) == 0) {
                    fn_1_21B68(cam, 1, 0x32, 10.0f, 0.95f * (1.0f - f), 90.0f);
                    omVibrate(player->playerNo, 10, 12, 0);
                }
            } else if (f < 0.6f) {
                if ((w->evtTimer % 20) == 0) {
                    fn_1_21B68(cam, 1, 0x32, 10.0f, 0.55f, 90.0f);
                    omVibrate(player->playerNo, 10, 4, 2);
                }
            } else if ((w->evtTimer % 30) == 0) {
                fn_1_21B68(cam, 1, 0x32, 10.0f, 0.25f, 90.0f);
                omVibrate(player->playerNo, 10, 6, 6);
            }
            pos.x = (f32) (2500.0 * sin(M_PI * (f64) (10.0f * (360.0f * f)) / 180.0) *
                           sin(M_PI * (f64) (180.0f * (1.0f - ((1.0f - f) * (1.0f - f)))) / 180.0));
            if (f < 0.5f) {
                tmp = 200.0 * sin(M_PI * (90.0 * (f64) (f / 0.5f)) / 180.0);
            } else {
                tmp = 200.0 + (f64) (lbl_1_data_F3C.y - 200.0f) * sin(M_PI * (f64) (90.0f * ((f - 0.5f) / 0.5f)) / 180.0);
            }
            pos.y = (f32) tmp;
            pos.y = (f32) ((f64) pos.y + 2500.0 * cos(M_PI * (f64) (10.0f * (360.0f * f)) / 180.0) *
                                             sin(M_PI * (f64) (180.0f * (1.0f - ((1.0f - f) * (1.0f - f)))) / 180.0));
            koopa->pos.x = pos.x;
            koopa->pos.y = lbl_1_bss_79EC.y + pos.y;
            koopa->pos.z = koopa->pos.z - (30.0f + 30.0f * f);
            g = 5.5f + -5.0f * f;
            Hu3DModelScaleSet(koopa->modelId, g, g, g);
            if (f > 0.1f) {
                PSVECAdd(&koopa->rot, &lbl_1_data_F2C, &koopa->rot);
            }
            if (dist < 100.0f || koopa->pos.z < lbl_1_data_F3C.z) {
                Hu3DModelAttrSet(koopa->modelId, 1);
                lbl_1_data_F38 = 0;
            }
        }
        if (w->evtTimer == 0xB4) {
            fn_1_814(0x8C7);
        }
        if (w->evtTimer == 0x12C) {
            endCamSet(cam, 4, 0.02f);
            if (w->seId >= 0) {
                fn_1_864(w->seId, 0x1388);
            }
        }
        w->evtTimer++;
        if (lbl_1_data_F38 == 0 && w->evtTimer >= 0x12C) {
            w->evtTimer = 0;
            w->state++;
        }
        break;
    case 5:
        dir.x = 0.0f;
        dir.y = 0.0f;
        dir.z = 1.0f;
        if (fn_1_AD98(player, &dir, 0.9f) != 0) {
            player->reqMotion = 0x18;
            player->loopMotion = 0;
            w->evtTimer = 0;
            w->state++;
        }
        break;
    case 6:
        if (w->seId >= 0) {
            fn_1_83C(w->seId);
            w->seId = -1;
        }
        f = endClamp01((f32) w->evtTimer / 120.0f);
        fn_1_4748(back, 4, 1.0f - f);
        w->evtTimer++;
        if (w->evtTimer >= 0x12C) {
            if (_CheckFlag(FLAG_MG_PRACTICE) == 0) {
                GwPlayer[0].mgCoinBonus = 10;
            }
            player->state = 0;
            player->sub = 0;
            w->evtTimer = 0;
            w->state = 0;
            return 1;
        }
        break;
    }

    if (w->state < 4) {
        PSVECSubtract(&lbl_1_data_F20, &player->pos, &dir);
        dir.y = 0.0f;
        if (PSVECMag(&dir) > 30.0f) {
            PSVECNormalize(&dir, &dir);
            input->stkY = 0;
            input->stkX = 0;
            input->btnDown = 0;
            input->btn = 0;
            input->stkX = (s8) (56.0f * dir.x);
            input->stkY = (s8) (56.0f * -dir.z);
            fn_1_7D58(player);
        } else {
            dir.x = 0.0f;
            dir.y = 0.0f;
            dir.z = -1.0f;
            fn_1_AD98(player, &dir, 0.9f);
        }
    }
    if (w->state >= 2) {
        if (lbl_1_bss_79A0 == 0x1FE) {
            v = lbl_1_data_F48;
            fn_1_2DC08((s16) cam->camMask, &v, 2);
            v.y = 50.0f;
            fn_1_2DC08((s16) cam->camMask, &v, 2);
            fn_1_21B68(cam, 1, 0x32, 10.0f, 0.95f, 90.0f);
            omVibrate(player->playerNo, 10, 12, 0);
            fn_1_814(0x8C4);
        } else if (lbl_1_bss_79A0 == 0x208) {
            v = lbl_1_data_F58;
            fn_1_2DC08((s16) cam->camMask, &v, 2);
            v.y = 50.0f;
            fn_1_2DC08((s16) cam->camMask, &v, 2);
            fn_1_21B68(cam, 1, 0x32, 10.0f, 0.95f, 90.0f);
            omVibrate(player->playerNo, 10, 12, 0);
            fn_1_814(0x8C4);
        } else if (lbl_1_bss_79A0 == 0x23A) {
            Hu3DMotionSpeedSet(back->model12, 0.1f);
        } else if (lbl_1_bss_79A0 == 0x244) {
            Hu3DMotionSpeedSet(back->model14, 0.1f);
        }
        if (lbl_1_bss_79A0 > 0x1FE && lbl_1_data_F54 != 0) {
            model = back->model12;
            dist = lbl_1_data_F3C.z - lbl_1_bss_79E0.z;
            if (dist < 0.0f) {
                dist = -dist;
            }
            f = endClamp01(dist / 23900.0f);
            f = 1.0f - f;
            lbl_1_bss_79E0.x = (f32) ((f64) lbl_1_data_F48.x + 2000.0 * sin(M_PI * (f64) (3.0f * (360.0f * f)) / 180.0) *
                                                                 sin(M_PI * (f64) (180.0f * (1.0f - ((1.0f - f) * (1.0f - f)))) / 180.0));
            if (f < 0.5f) {
                tmp = 200.0 * sin(M_PI * (90.0 * (f64) (f / 0.5f)) / 180.0);
            } else {
                tmp = 200.0 + (f64) (lbl_1_data_F3C.y - 1000.0f) * sin(M_PI * (f64) (90.0f * ((f - 0.5f) / 0.5f)) / 180.0);
            }
            lbl_1_bss_79E0.y = (f32) tmp;
            lbl_1_bss_79E0.y = (f32) ((f64) lbl_1_bss_79E0.y + 2000.0 * cos(M_PI * (f64) (3.0f * (360.0f * f)) / 180.0) *
                                                                   sin(M_PI * (f64) (180.0f * (1.0f - ((1.0f - f) * (1.0f - f)))) / 180.0));
            lbl_1_bss_79E0.y = lbl_1_bss_79E0.y + lbl_1_data_F48.y;
            lbl_1_bss_79E0.z = lbl_1_bss_79E0.z - (5.0f + 200.0f * f);
            lbl_1_bss_79D4.x = lbl_1_bss_79D4.x + lbl_1_bss_79C8.x * (0.3f + 0.7f * f);
            lbl_1_bss_79D4.y = lbl_1_bss_79D4.y + lbl_1_bss_79C8.y * (0.3f + 0.7f * f);
            lbl_1_bss_79D4.z = lbl_1_bss_79D4.z + lbl_1_bss_79C8.z * (0.3f + 0.7f * f);
            Hu3DModelPosSet(model, lbl_1_bss_79E0.x, lbl_1_bss_79E0.y, lbl_1_bss_79E0.z);
            g = 1.0f - f;
            Hu3DModelScaleSet(model, g, g, g);
            Hu3DModelRotSet(model, lbl_1_bss_79D4.x, lbl_1_bss_79D4.y, lbl_1_bss_79D4.z);
            if (dist < 100.0f || lbl_1_bss_79E0.z < lbl_1_data_F3C.z) {
                Hu3DModelAttrSet(model, 1);
                lbl_1_data_F54 = 0;
            }
        }
        if (lbl_1_bss_79A0 > 0x208 && lbl_1_data_F64 != 0) {
            model = back->model14;
            dist = lbl_1_data_F3C.z - lbl_1_bss_79BC.z;
            if (dist < 0.0f) {
                dist = -dist;
            }
            f = endClamp01(dist / 23900.0f);
            f = 1.0f - f;
            lbl_1_bss_79BC.x = (f32) ((f64) lbl_1_data_F58.x + 1500.0 * sin(M_PI * (f64) (4.0f * (360.0f * f)) / 180.0) *
                                                                 sin(M_PI * (f64) (180.0f * (1.0f - ((1.0f - f) * (1.0f - f)))) / 180.0));
            if (f < 0.5f) {
                tmp = 200.0 * sin(M_PI * (90.0 * (f64) (f / 0.5f)) / 180.0);
            } else {
                tmp = 200.0 + (f64) (lbl_1_data_F3C.y - 1000.0f) * sin(M_PI * (f64) (90.0f * ((f - 0.5f) / 0.5f)) / 180.0);
            }
            lbl_1_bss_79BC.y = (f32) tmp;
            lbl_1_bss_79BC.y = (f32) ((f64) lbl_1_bss_79BC.y + 1500.0 * cos(M_PI * (f64) (4.0f * (360.0f * f)) / 180.0) *
                                                                   sin(M_PI * (f64) (180.0f * (1.0f - ((1.0f - f) * (1.0f - f)))) / 180.0));
            lbl_1_bss_79BC.y = lbl_1_bss_79BC.y + lbl_1_data_F58.y;
            lbl_1_bss_79BC.z = lbl_1_bss_79BC.z - (5.0f + 200.0f * f);
            lbl_1_bss_79B0.x = lbl_1_bss_79B0.x + lbl_1_bss_79A4.x * (0.3f + 0.7f * f);
            lbl_1_bss_79B0.y = lbl_1_bss_79B0.y + lbl_1_bss_79A4.y * (0.3f + 0.7f * f);
            lbl_1_bss_79B0.z = lbl_1_bss_79B0.z + lbl_1_bss_79A4.z * (0.3f + 0.7f * f);
            Hu3DModelPosSet(model, lbl_1_bss_79BC.x, lbl_1_bss_79BC.y, lbl_1_bss_79BC.z);
            g = 1.0f - f;
            Hu3DModelScaleSet(model, g, g, g);
            Hu3DModelRotSet(model, lbl_1_bss_79B0.x, lbl_1_bss_79B0.y, lbl_1_bss_79B0.z);
            if (dist < 100.0f || lbl_1_bss_79BC.z < lbl_1_data_F3C.z) {
                Hu3DModelAttrSet(model, 1);
                lbl_1_data_F64 = 0;
            }
        }
        lbl_1_bss_79A0++;
    }
    return 0;
}
