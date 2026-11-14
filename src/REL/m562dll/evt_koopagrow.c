#include "REL/m562dll.h"
#include "game/gamework.h"
#include "game/window.h"
#include "game/wipe.h"
#include <math.h>

float Hu3DMotionMaxTimeGet(HU3DMODELID modelId);

/* .data */
static Vec lbl_1_data_DB0[4] = {
    { 0.0f, 478.0f, 485.0f },
    { 0.0f, 262.0f, 540.0f },
    { 0.0f, 1800.0f, 785.0f },
    { 0.0f, 800.0f, 1400.0f },
};
static Vec lbl_1_data_DE0[4] = {
    { 0.0f, -76.0f, -1280.0f },
    { 0.0f, 1040.0f, -1280.0f },
    { 0.0f, 285.0f, -1026.0f },
    { 0.0f, 100.0f, -200.0f },
};
static Vec lbl_1_data_E10 = { 0.0f, 1000.0f, 1000.0f };
static Vec lbl_1_data_E1C = { 0.0f, -1.0f, -1.0f };
static GXColor lbl_1_data_E28 = { 0, 0, 0, 0 };
static GXColor lbl_1_data_E2C = { 0x80, 0xB0, 0xFF, 0xFF };
static Vec lbl_1_data_E30 = { 0.0f, 1000.0f, -280.0f };
static Vec lbl_1_data_E3C = { 0.0f, 55.0f, -1250.0f };
static Vec lbl_1_data_E48 = { 0.0f, 0.0f, 0.0f };
static Vec lbl_1_data_E54 = { 0.0f, 0.0f, 0.0f };
static s16 lbl_1_data_E60 = -1;
static s32 lbl_1_data_E64 = -1;

/* .bss */
static s16 lbl_1_bss_7990;
static f32 lbl_1_bss_7994;
static s16 lbl_1_bss_7998;
static s16 lbl_1_bss_799A;
static s16 lbl_1_bss_799C;

static void growLightColor(f32 f) {
    Hu3DGLightColorSet(lbl_1_data_E60, (u8) (s32) ((f32) lbl_1_data_E2C.r * f), (u8) (s32) ((f32) lbl_1_data_E2C.g * f),
                       (u8) (s32) ((f32) lbl_1_data_E2C.b * f), (u8) (s32) ((f32) lbl_1_data_E2C.a * f));
}

static f32 growClamp01(f32 v) {
    if (v > 1.0f) {
        return 1.0f;
    }
    if (v < 0.0f) {
        return 0.0f;
    }
    return v;
}

s32 fn_1_280C8(M562Work *w) {
    M562FinPlayer *player = (M562FinPlayer *) &w->player;
    M562Back *back = &w->back;
    CAMERA *cam = (CAMERA *) &w->camera[0];
    KoopaBoss *koopa = &w->boss;
    M562StageField *cells = &w->stage;
    Mtx mtx;
    Vec pos;
    f32 f;
    f32 g;
    f32 maxTime;

    switch (w->state) {
    case 0:
        fn_1_16F0(w, 3);
        back->reflectOn = 0;
        back->rippleOn = 0;
        w->mecha.active = 0;
        lbl_1_bss_7998 = 0;
        lbl_1_data_E64 = -1;
        fn_1_864(lbl_1_bss_0->seId2, 0x3E8);
        lbl_1_bss_0->seId2 = -1;
        HuAudAUXSet(4, 1);
        if (w->seId >= 0) {
            fn_1_864(w->seId, 0xBB8);
        }
        Hu3DModelAttrReset(cells->modelF, 1);
        if (GwSystem.subGameNo >= 3) {
            cam->mode = 1;
            cam->pos = lbl_1_data_DB0[3];
            cam->posGoal = cam->pos;
            cam->tgt = lbl_1_data_DE0[3];
            cam->tgtGoal = cam->tgt;
            cam->prevMode = 0;
            cam->speed = 0.05f;
            cam->slerpT = 0.05f;
            koopa->pos = lbl_1_data_E3C;
            koopa->pos.y = -500.0f;
            koopa->reqMotion = 1;
            koopa->curMotion = 1;
            koopa->loopMotion = 1;
            player->pos = lbl_1_data_E30;
            player->pos.y = 0.0f;
            player->pos.z = 500.0f;
            player->rotGoal = 180.0f;
            player->rotSpeed = 180.0f;
            player->rotY = player->rotGoal;
            player->animNo = 0x1C;
            player->animLoop = 0;
            Hu3DModelAttrSet(cells->modelE, 1);
            Hu3DModelAttrSet(w->triple.model2, 1);
            Hu3DModelAttrSet(cells->modelF, 1);
            fn_1_22694(&w->light, 3, 1.0f);
            fn_1_4748(back, 3, 1.0f);
            w->evtTimer = 0;
            w->state = 0xE;
            break;
        }
        if (w->bgmId < 0) {
            w->bgmId = fn_1_A44(0x57);
            fn_1_9BC(w->bgmId, 0x7F, 0);
        }
        cam->mode = 1;
        cam->pos = lbl_1_data_DB0[0];
        cam->posGoal = cam->pos;
        cam->tgt = lbl_1_data_DE0[0];
        cam->tgtGoal = cam->tgt;
        cam->prevMode = 0;
        cam->speed = 1.0f;
        cam->slerpT = 1.0f;
        koopa->pos = lbl_1_data_E3C;
        koopa->reqMotion = 0xB;
        koopa->loopMotion = 1;
        koopa->state = 0;
        Hu3DModelScaleSet(koopa->modelId, 1.5f, 1.5f, 1.5f);
        Hu3DModelAttrReset(w->triple.model2, 1);
        player->unk4 = 0;
        player->animNo = 3;
        player->animLoop = 0;
        player->pos = lbl_1_data_E30;
        player->rotGoal = 180.0f;
        player->rotSpeed = 180.0f;
        player->rotY = player->rotGoal;
        player->velY = 0.0f;
        fn_1_22694(&w->light, 3, 1.0f);
        fn_1_4748(back, 3, 1.0f);
        lbl_1_data_E60 = -1;
        w->evtTimer = 0;
        w->state++;
        /* fallthrough */
    case 1:
        player->velY -= 1.5f;
        player->pos.y += player->velY;
        if (player->velY < 0.0f) {
            if (player->pos.y <= 0.0f) {
                player->pos.y = 0.0f;
                player->velY = 0.0f;
                player->animNo = 4;
                player->animLoop = 0;
                w->evtTimer = 0;
                w->state++;
            }
        }
        break;
    case 2:
        if (w->evtTimer == 0xC) {
            player->animNo = 0x1C;
            player->animLoop = 0;
        }
        w->evtTimer++;
        if (w->evtTimer >= 0x78) {
            HuWinExOpen(w->winId);
            HuWinMesSet(w->winId, 0x410004);
            koopa->reqMotion = 0x12;
            koopa->loopMotion = 1;
            lbl_1_bss_799A = 2;
            lbl_1_bss_799C = 0;
            w->evtTimer = 0;
            w->state++;
        }
        break;
    case 3:
        if (HuWinStatGet(w->winId) == 2) {
            lbl_1_bss_799A = 2;
        } else if (HuWinStatGet(w->winId) == 1) {
            if (lbl_1_bss_799A == 2) {
                if (lbl_1_bss_799C == 0) {
                    fn_1_814(0x2CA);
                } else if (lbl_1_bss_799C == 2) {
                    fn_1_814(0x2CB);
                }
                lbl_1_bss_799C++;
            }
            lbl_1_bss_799A = 1;
        } else if (HuWinStatGet(w->winId) == 0) {
            lbl_1_bss_799A = 0;
            HuWinExClose(w->winId);
            HuWinHomeClear(w->winId);
            koopa->reqMotion = 0xC;
            koopa->loopMotion = 0;
            w->evtTimer = 0;
            w->state++;
        }
        break;
    case 4:
        if (w->evtTimer > 0x14 && w->evtTimer < 0x78 && (w->evtTimer % 30) == 0) {
            fn_1_814(0x8BB);
        }
        w->evtTimer++;
        if (w->evtTimer >= 0x7D) {
            w->evtTimer = 0;
            w->state++;
        }
        break;
    case 5:
        w->evtTimer++;
        if (w->evtTimer >= 0x78) {
            koopa->reqMotion = 0xD;
            koopa->loopMotion = 0;
            pos = koopa->pos;
            Hu3DModelPosSet(koopa->chargeModel, pos.x, pos.y, pos.z);
            Hu3DMotionSpeedSet(koopa->chargeModel, 9.66f);
            Hu3DModelAttrSet(koopa->chargeModel, 0x40000004);
            Hu3DModelScaleSet(koopa->chargeModel, 1.5f, 1.5f, 1.5f);
            lbl_1_data_E60 = Hu3DGLightCreateV(&lbl_1_data_E10, &lbl_1_data_E1C, &lbl_1_data_E28);
            Hu3DGLightStaticSet(lbl_1_data_E60, 1);
            Hu3DGLightSpotSet(lbl_1_data_E60, GX_SP_COS, 30.0f);
            Hu3DGLightInfinitytSet(lbl_1_data_E60);
            lbl_1_data_E64 = fn_1_814(0x8BA);
            w->evtTimer = 0;
            w->state++;
        }
        break;
    case 6:
        if (w->evtTimer == 0x1E) {
            Hu3DModelAttrReset(koopa->chargeModel, 1);
        }
        if (lbl_1_data_E60 >= 0) {
            if (w->evtTimer < 0x14) {
                f = (f32) w->evtTimer / 20.0f;
                growLightColor(f);
            } else if (w->evtTimer > 0x104) {
                f = 1.0f - ((f32) (w->evtTimer - 0x104) / 40.0f);
                growLightColor(f);
            } else {
                f = (f32) (0.9f + 0.1f * sin(M_PI * (f64) (30.0f * (360.0f * ((f32) w->evtTimer / 300.0f))) / 180.0));
                growLightColor(f);
            }
        }
        f = (f32) w->evtTimer / 300.0f;
        if ((w->evtTimer % 20) == 0) {
            omVibrate(player->charIdx, 10, 6, 6);
            fn_1_21B68(cam, 1, 0x32, 10.0f, 0.75f, 90.0f);
        }
        g = (f32) ((f64) (1.5f + 4.0f * f) + 0.04f * sin(M_PI * (f64) (200.0f * (360.0f * f)) / 180.0));
        Hu3DModelScaleSet(koopa->modelId, g, g, g);
        g = 1.5f + 4.0f * f;
        Hu3DModelScaleSet(koopa->chargeModel, g, g, g);
        maxTime = Hu3DMotionMaxTimeGet(koopa->modelId);
        if (Hu3DMotionTimeGet(koopa->modelId) >= maxTime) {
            Hu3DMotionTimeSet(koopa->modelId, 65.0f);
        }
        if (w->evtTimer == 0x1E) {
            fn_1_814(0x8C7);
        }
        if (w->evtTimer == 0x78) {
            player->unkC = 0;
        } else if (w->evtTimer == 0x122) {
            player->unkC = 1;
        }
        w->evtTimer++;
        if (w->evtTimer == 0x3C) {
            cam->posGoal = lbl_1_data_DB0[1];
            cam->tgtGoal = lbl_1_data_DE0[1];
            cam->prevMode = 0;
            cam->speed = 0.01f;
            cam->slerpT = 0.01f;
        } else if (w->evtTimer >= 0x12C) {
            Hu3DModelAttrSet(koopa->chargeModel, 1);
            Hu3DGLightKill(lbl_1_data_E60);
            lbl_1_data_E60 = -1;
            fn_1_83C(lbl_1_data_E64);
            lbl_1_data_E64 = -1;
            koopa->reqMotion = 0;
            koopa->loopMotion = 0;
            w->evtTimer = 0;
            w->state++;
        }
        break;
    case 7:
        if (w->evtTimer == 0x14) {
            fn_1_814(0x8BC);
        }
        w->evtTimer++;
        if (w->evtTimer >= 0x1E) {
            cam->posGoal = lbl_1_data_DB0[2];
            cam->tgtGoal = lbl_1_data_DE0[2];
            cam->prevMode = 0;
            cam->speed = 0.04f;
            cam->slerpT = 0.04f;
            w->evtTimer = 0;
            w->state++;
        }
        break;
    case 8:
        w->evtTimer++;
        if (w->evtTimer == 0x78) {
            fn_1_21B68(cam, 1, 0x32, 10.0f, 0.95f, 90.0f);
            omVibrate(player->charIdx, 10, 12, 0);
            pos = koopa->pos;
            pos.y = lbl_1_data_E3C.y;
            fn_1_2DC08((s16) cam->camMask, &pos, 2);
            fn_1_814(0x8C0);
        } else if (w->evtTimer == 0x82) {
            pos = koopa->pos;
            pos.y = lbl_1_data_E3C.y;
            fn_1_2DC08((s16) cam->camMask, &pos, 2);
        } else if (w->evtTimer >= 0x8C) {
            koopa->reqMotion = 0xE;
            koopa->loopMotion = 0;
            if (w->bgmId >= 0) {
                fn_1_9F4(w->bgmId, 0x64);
                w->bgmId = -1;
            }
            HuWinExOpen(w->winId);
            HuWinMesSet(w->winId, 0x410005);
            w->evtTimer = 0;
            w->state++;
        }
        break;
    case 9:
        w->evtTimer++;
        maxTime = Hu3DMotionMaxTimeGet(koopa->modelId);
        if (Hu3DMotionTimeGet(koopa->modelId) >= maxTime) {
            Hu3DMotionTimeSet(koopa->modelId, 20.0f);
        }
        if (w->evtTimer >= 0x5A) {
            HuWinExClose(w->winId);
            HuWinHomeClear(w->winId);
            lbl_1_data_E64 = fn_1_814(0x8BE);
            w->evtTimer = 0;
            w->state++;
        }
        break;
    case 10:
        f = (f32) w->evtTimer / 300.0f;
        koopa->pos.y = 55.0f + -555.0f * f;
        koopa->pos.y = (f32) ((f64) koopa->pos.y + 10.0 * sin(M_PI * (f64) (200.0f * (360.0f * f)) / 180.0));
        if ((w->evtTimer % 10) == 0) {
            fn_1_21B68(cam, 1, 0x32, 10.0f, 0.95f, 90.0f);
        }
        if ((w->evtTimer % 30) == 0) {
            pos = koopa->pos;
            pos.y = lbl_1_data_E3C.y;
            fn_1_2DC08((s16) cam->camMask, &pos, 2);
            omVibrate(player->charIdx, 10, 12, 0);
        }
        if (((w->evtTimer + 15) % 30) == 0) {
            omVibrate(player->charIdx, 10, 6, 6);
        }
        switch (w->evtTimer) {
        case 0xA:
            koopa->reqMotion = 0xF;
            koopa->loopMotion = 0;
            break;
        case 0x14:
            Hu3DMotionSpeedSet(cells->modelE, 0.3f);
            fn_1_814(0x8BD);
            break;
        case 0x5A:
            cam->posGoal = lbl_1_data_DB0[3];
            cam->tgtGoal = lbl_1_data_DE0[3];
            cam->prevMode = 0;
            cam->speed = 0.01f;
            cam->slerpT = 0.01f;
            HuWinExOpen(w->winId);
            HuWinMesSet(w->winId, 0x410006);
            koopa->reqMotion = 0xF;
            koopa->loopMotion = 1;
            break;
        case 0xF0:
            HuWinExClose(w->winId);
            HuWinHomeClear(w->winId);
            break;
        }
        if (w->evtTimer < 0xA) {
            maxTime = Hu3DMotionMaxTimeGet(koopa->modelId);
            if (Hu3DMotionTimeGet(koopa->modelId) >= maxTime) {
                Hu3DMotionTimeSet(koopa->modelId, 20.0f);
            }
        } else if (w->evtTimer < 0x5A) {
            Hu3DMotionSpeedSet(koopa->modelId, 0.5f);
        } else if (w->evtTimer < 0x78) {
            Hu3DMotionSpeedSet(koopa->modelId, 0.75f);
        } else {
            Hu3DMotionSpeedSet(koopa->modelId, 1.0f);
        }
        w->evtTimer++;
        if (w->evtTimer >= 0x12C) {
            fn_1_83C(lbl_1_data_E64);
            lbl_1_data_E64 = -1;
            fn_1_814(0x8C9);
            Hu3DModelAttrSet(cells->modelE, 1);
            cam->speed = 0.05f;
            cam->slerpT = 0.05f;
            w->evtTimer = 0;
            w->state++;
        }
        break;
    case 11:
        w->evtTimer++;
        if (w->evtTimer >= 0x3C) {
            HuWinExOpen(w->winId);
            HuWinMesSet(w->winId, 0x410007);
            fn_1_814(0x8C8);
            koopa->reqMotion = 0x10;
            koopa->loopMotion = 1;
            w->evtTimer = 0;
            w->state++;
        }
        break;
    case 12:
        if (HuWinStatGet(w->winId) == 0) {
            HuWinExClose(w->winId);
            HuWinHomeClear(w->winId);
            w->evtTimer = 0;
            w->state++;
        }
        break;
    case 13:
        w->evtTimer++;
        if (w->evtTimer >= 0x1E) {
            Hu3DModelAttrSet(cells->modelF, 1);
            koopa->reqMotion = 1;
            koopa->loopMotion = 0;
            player->unk6 = 0;
            player->unk4 = 0;
            w->evtTimer = 0;
            w->state++;
        }
        break;
    case 14:
        if (w->seId >= 0) {
            fn_1_83C(w->seId);
            w->seId = -1;
        }
        if (GwSystem.subGameNo < 3) {
            GwSystem.subGameNo = 3;
        }
        if (WipeCheckEnd() == 0) {
            w->mecha.active = 1;
            w->evtTimer = 0;
            w->state = 0;
            return 1;
        }
        break;
    }

    if (w->state < 5) {
        PSMTXCopy(fn_1_1010C(koopa), mtx);
        Hu3DMtxTransGet(mtx, &pos);
        lbl_1_data_E48 = pos;
        mtx[0][3] = 0.0f;
        mtx[1][3] = 0.0f;
        mtx[2][3] = 0.0f;
        PSMTXCopy(mtx, Hu3DData[w->triple.model2].mtx);
        lbl_1_bss_7990 = 0;
    } else if (w->state >= 5 && w->state < 6) {
        if (lbl_1_bss_7990 == 0) {
            lbl_1_data_E54.x = -5.0f;
            lbl_1_data_E54.y = 0.0f;
            lbl_1_data_E54.z = -15.0f;
            fn_1_814(0x8C1);
        } else if (lbl_1_bss_7990 < 0x78) {
            lbl_1_bss_7994 += 6.0f;
            lbl_1_data_E54.y -= 1.5f;
            PSVECAdd(&lbl_1_data_E48, &lbl_1_data_E54, &lbl_1_data_E48);
            if (lbl_1_data_E48.z > -2200.0f && lbl_1_data_E48.y < 55.0f) {
                lbl_1_data_E48.y = 55.0f;
                lbl_1_data_E54.y = lbl_1_data_E54.y * -1.0f;
                fn_1_814(0x8B9);
            }
        } else {
            Hu3DModelAttrSet(w->triple.model2, 1);
        }
        lbl_1_bss_7990++;
    }
    Hu3DModelPosSet(w->triple.model2, lbl_1_data_E48.x, lbl_1_data_E48.y, lbl_1_data_E48.z);
    Hu3DModelRotSet(w->triple.model2, lbl_1_bss_7994, lbl_1_bss_7994, 0.0f);
    lbl_1_bss_7998++;
    f = growClamp01((f32) lbl_1_bss_7998 / 60.0f);
    Hu3DMotionSpeedSet(back->model40, (f32) (0.25f + 2.0f * sin(M_PI * (f64) (90.0f + 90.0f * f) / 180.0)));
    if (w->state >= 6) {
        if (player->pos.z < 500.0f) {
            player->animNo = 0x1B;
            player->animLoop = 1;
            player->rotGoal = fn_1_36EC(0.0f, player->rotGoal, 0.9f);
            player->rotY = player->rotGoal;
            player->pos.z += 5.0f;
        } else {
            player->pos.z = 500.0f;
            player->animNo = 0x1C;
            player->animLoop = 0;
            player->rotGoal = 180.0f;
            player->rotSpeed = 180.0f;
            player->rotY = player->rotGoal;
        }
    }
    return 0;
}
