#include "REL/m562dll.h"
#include "game/gamework.h"
#include "game/window.h"
#include <math.h>

void fn_1_277C8(M562Work *w, f32 t);

/* camera goals: [0] first run, [1] later runs (pos / target pairs) */
static Vec lbl_1_data_D28[2] = {
    { 0.0f, 347.9f, 26.0f },
    { 0.0f, 1000.0f, 1400.0f },
};
static Vec lbl_1_data_D40[2] = {
    { 0.0f, 346.54f, 21.37f },
    { 0.0f, 100.0f, -200.0f },
};
static s32 lbl_1_data_D58 = -1;

static f32 lbl_1_bss_7958;

/* Fight stage: Bowser stomps in, speaks, the backdrop shakes and Bowser is
   dropped in from above. */
s32 fn_1_260B8(M562Work *w)
{
    M562EvtPlayer *player = (M562EvtPlayer *)&w->player;
    M562Back *back = &w->back;
    CAMERA *camera = (CAMERA *)&w->camera[0];
    Koopa *koopa = (Koopa *)&w->koopa;
    M562StageField *field = &w->stage;
    M562EvtInput *input = &player->input;
    s32 done = 0;
    f32 t;
    f32 clamped;
    f32 mag;
    s32 k;
    u32 attr;
    Vec diff;
    Vec goal;
    Vec dir;
    Vec tmp;
    M562StageTile *cell;

    switch (w->state) {
    case 0:
        back->reflectOn = 1;
        back->rippleOn = 0;
        fn_1_16F0(w, 2);
        koopa->visibleReq = 1;
        koopa->state = 2;
        koopa->sub = 0xA;
        koopa->reqMotion = 0;
        koopa->loopMotion = 1;
        koopa->active = 0;
        Hu3DModelCameraSet(koopa->modelId, (u16)(w->camera[0].camMask | w->camera[1].camMask));
        if (GwSystem.subGameNo >= 2) {
            camera->mode = 1;
            camera->posGoal = lbl_1_data_D28[1];
            camera->tgtGoal = lbl_1_data_D40[1];
            camera->prevMode = 0;
            camera->speed = 0.04f;
            camera->slerpT = 0.04f;
            w->evtTimer = 0;
            w->state = 0xA;
            break;
        }
        camera->mode = 1;
        camera->posGoal = lbl_1_data_D28[0];
        camera->tgtGoal = lbl_1_data_D40[0];
        camera->prevMode = 0;
        camera->speed = 0.04f;
        camera->slerpT = 0.03f;
        player->animNo = 1;
        player->animLoop = 1;
        lbl_1_data_D58 = -1;
        w->state++;
        /* fallthrough */
    case 1:
        t = (f32)w->evtTimer / 120.0f;
        fn_1_22694(&w->light, 2, t);
        fn_1_4748(back, 2, t);
        Hu3DMotionSpeedSet(back->model40, 0.25 + 2.0 * sin(M_PI * (f64)(180.0f * t) / 180.0));
        if (w->evtTimer < 0x3C) {
            if (w->seId < 0) {
                w->seId = fn_1_814(0x8B5);
            }
        } else if (w->evtTimer == 0x3C && w->seId >= 0) {
            fn_1_864(w->seId, 0xBB8);
        }
        goal.x = 0.0f;
        goal.y = 0.0f;
        goal.z = 375.0f;
        PSVECSubtract(&goal, &player->pos, &diff);
        diff.y = 0.0f;
        if (PSVECMag(&diff) > 50.0f) {
            PSVECNormalize(&diff, &diff);
            input->btnA = 0;
            input->btnB = 0;
            input->stickX = 0;
            input->stickY = 0;
            input->stickX = 56.0f * diff.x;
            input->stickY = 56.0f * -diff.z;
            fn_1_7D58(player);
        }
        if (w->evtTimer++ > 0x78) {
            goal.x = 0.0f;
            goal.y = 0.0f;
            goal.z = 375.0f;
            player->pos = goal;
            player->rotGoal = 180.0f;
            player->rotSpeed = 180.0f;
            player->rotY = player->rotGoal;
            player->animNo = 0x1C;
            player->animLoop = 0;
            w->evtTimer = 0;
            w->state++;
        }
        break;
    case 2:
        HuWinExOpen(w->winId);
        HuWinMesSet(w->winId, 0x410002);
        fn_1_814(0x8C6);
        koopa->reqMotion = 5;
        koopa->pendingDelay = 0x1E;
        koopa->loopMotion = 1;
        w->evtTimer = 0;
        w->state++;
        break;
    case 3:
        if (HuWinStatGet(w->winId) != 2) {
            if (HuWinStatGet(w->winId) == 1) {
                koopa->curMotion = 0;
                koopa->reqMotion = 5;
                koopa->loopMotion = 1;
            } else if (HuWinStatGet(w->winId) == 0) {
                koopa->curMotion = 0xB;
                koopa->reqMotion = 0xB;
                koopa->loopMotion = 0;
                koopa->shiftHold = 0;
                attr = koopa->loopMotion != 0 ? HU3D_MOTATTR_LOOP : 0;
                Hu3DMotionShiftSet(koopa->modelId, koopa->motionId[koopa->reqMotion], 4.0f, 0.0f, attr);
                HuWinExClose(w->winId);
                HuWinHomeClear(w->winId);
                w->evtTimer = 0;
                w->state++;
            }
        }
        break;
    case 4:
        if (w->evtTimer == 0xA) {
            fn_1_21B68(camera, 1, 0x32, 40.0f, 0.95f, 90.0f);
            omVibrate(player->charIdx, 0x14, 0xC, 0);
            Hu3DMotionSpeedSet(back->model20, 1.0f);
            Hu3DModelAttrReset(back->model20, 1);
            back->bobOn = 0;
        }
        if (w->evtTimer == 0xA) {
            fn_1_814(0x8B8);
        }
        if (w->evtTimer == 0x28) {
            fn_1_814(0x8A3);
        }
        if (w->evtTimer < 0x28) {
            back->reflectOn = 1;
            back->rippleOn = 0;
        } else if (w->evtTimer < 0x50) {
            back->reflectOn = 1;
            back->rippleOn = 1;
        } else {
            back->reflectOn = 0;
            back->rippleOn = 0;
        }
        if (w->evtTimer == 0x50) {
            fn_1_814(0x8BF);
        }
        if (w->evtTimer >= 0x50 && w->evtTimer < 0x64) {
            k = w->evtTimer - 0x50;
            fn_1_277C8(w, (f32)k / 20.0f);
        }
        w->evtTimer++;
        if (w->evtTimer >= 0x78) {
            camera->prevMode = 0;
            camera->posGoal = lbl_1_data_D28[1];
            camera->tgtGoal = lbl_1_data_D40[1];
            camera->speed = 0.06f;
            camera->slerpT = 0.06f;
            koopa->visibleReq = 1;
            koopa->state = 3;
            koopa->sub = 0;
            koopa->active = 0;
            fn_1_22694(&w->light, 2, 1.0f);
            fn_1_B578(koopa);
            tmp.x = 0.0f;
            tmp.y = 1000.0f;
            tmp.z = -125.0f;
            koopa->pos = tmp;
            Hu3DModelPosSet(koopa->modelId, koopa->pos.x, koopa->pos.y, koopa->pos.z);
            Hu3DModelScaleSet(koopa->modelId, 0.0f, 0.0f, 0.0f);
            koopa->sub = 1;
            koopa->reqMotion = 0;
            w->evtTimer = 0;
            w->state++;
        }
        break;
    case 5:
        t = (f32)w->evtTimer / 20.0f;
        koopa->pos.y = 1000.0f;
        Hu3DModelScaleSet(koopa->modelId, 1.5f * t, 1.5f * t, 1.5f * t);
        if (++w->evtTimer >= 0x14) {
            koopa->loopMotion = 0;
            koopa->reqMotion = 3;
            koopa->curMotion = 3;
            Hu3DModelAttrReset(koopa->modelId, HU3D_MOTATTR_LOOP);
            attr = koopa->loopMotion != 0 ? HU3D_MOTATTR_LOOP : 0;
            Hu3DMotionShiftSet(koopa->modelId, koopa->motionId[koopa->curMotion], 0.0f, 4.0f, attr);
            Hu3DMotionTimeSet(koopa->modelId, 40.0f);
            Hu3DModelScaleSet(koopa->modelId, 1.5f, 1.5f, 1.5f);
            w->evtTimer = 0;
            w->state++;
        }
        break;
    case 6:
        t = (f32)w->evtTimer / 20.0f;
        if (t > 1.0f) {
            clamped = 1.0f;
        } else if (t < 0.0f) {
            clamped = 0.0f;
        } else {
            clamped = t;
        }
        koopa->pos.x = 0.0f;
        koopa->pos.y = 1000.0f * (1.0f - clamped);
        koopa->pos.z = -125.0f;
        w->evtTimer++;
        if (w->evtTimer == 20) {
            fn_1_814(0x8A7);
            fn_1_21B68(camera, 1, 0x32, 40.0f, 0.95f, 90.0f);
            fn_1_2DC08((s16)camera->camMask, &koopa->pos, 0);
            omVibrate(player->charIdx, 0x14, 0xC, 0);
            cell = &field->tile[7];
            cell->flag = 1;
            field->quakePos = cell->pos;
            field->quakeTimer = 1;
        } else if (w->evtTimer == 40) {
            koopa->reqMotion = 0;
            koopa->loopMotion = 1;
        } else if (w->evtTimer >= 60) {
            w->evtTimer = 0;
            w->state++;
        }
        break;
    case 7:
        HuWinExOpen(w->winId);
        HuWinMesSet(w->winId, 0x410003);
        fn_1_814(0x2CA);
        koopa->reqMotion = 5;
        koopa->pendingDelay = 0x1E;
        koopa->loopMotion = 1;
        w->evtTimer = 0;
        w->state++;
        break;
    case 8:
        if (HuWinStatGet(w->winId) == 2) {
        } else if (HuWinStatGet(w->winId) == 1) {
        } else if (HuWinStatGet(w->winId) == 0) {
            koopa->curMotion = 0;
            koopa->reqMotion = 0;
            koopa->loopMotion = 1;
            koopa->shiftHold = 0;
            attr = koopa->loopMotion != 0 ? HU3D_MOTATTR_LOOP : 0;
            Hu3DMotionShiftSet(koopa->modelId, koopa->motionId[koopa->reqMotion], 0.0f, 20.0f, attr);
            HuWinExClose(w->winId);
            HuWinHomeClear(w->winId);
            w->evtTimer = 0;
            w->state++;
        }
        break;
    case 9:
        if (w->seId >= 0) {
            fn_1_83C(w->seId);
            w->seId = -1;
        }
        if (GwSystem.subGameNo < 2) {
            GwSystem.subGameNo = 2;
        }
        player->unk6 = 0;
        player->unk4 = 0;
        w->state = 0;
        done = 1;
        break;
    case 10:
        t = (f32)w->evtTimer / 120.0f;
        fn_1_22694(&w->light, 2, t);
        fn_1_4748(back, 2, t);
        Hu3DMotionSpeedSet(back->model40, 0.25 + 2.0 * sin(M_PI * (f64)(180.0f * t) / 180.0));
        if (w->evtTimer < 0x3C) {
            if (w->seId < 0) {
                w->seId = fn_1_814(0x8B5);
            }
        } else if (w->evtTimer == 0x3C && w->seId >= 0) {
            fn_1_864(w->seId, 0xBB8);
        }
        koopa->reqMotion = 0;
        koopa->loopMotion = 1;
        goal.x = 0.0f;
        goal.y = 0.0f;
        goal.z = 375.0f;
        PSVECSubtract(&goal, &player->pos, &diff);
        diff.y = 0.0f;
        mag = PSVECMag(&diff);
        if (mag > 5.0f) {
            PSVECNormalize(&diff, &diff);
            input->btnA = 0;
            input->btnB = 0;
            input->stickX = 0;
            input->stickY = 0;
            input->stickX = 56.0f * diff.x;
            input->stickY = 56.0f * -diff.z;
            fn_1_7D58(player);
        } else {
            player->animNo = 0;
            player->animLoop = 1;
        }
        w->evtTimer++;
        if (w->evtTimer > 0x78) {
            if (mag <= 10.0f) {
                tmp.x = 0.0f;
                tmp.y = 0.0f;
                tmp.z = 375.0f;
                goal = tmp;
                w->evtTimer = 0;
                w->state++;
            }
        }
        break;
    case 11:
        dir.x = 0.0f;
        dir.y = 0.0f;
        dir.z = -1.0f;
        if (fn_1_AD98(player, &dir, 0.9f) != 0) {
            player->animNo = 0x1C;
            player->animLoop = 0;
            koopa->curMotion = 0xB;
            koopa->reqMotion = 0xB;
            koopa->loopMotion = 0;
            koopa->shiftHold = 0;
            attr = koopa->loopMotion != 0 ? HU3D_MOTATTR_LOOP : 0;
            Hu3DMotionShiftSet(koopa->modelId, koopa->motionId[koopa->reqMotion], 4.0f, 0.0f, attr);
            w->evtTimer = 0;
            w->state++;
        }
        break;
    case 12:
        if (w->evtTimer == 0x14) {
            fn_1_21B68(camera, 1, 0x32, 40.0f, 0.95f, 90.0f);
            omVibrate(player->charIdx, 0x14, 0xC, 0);
            Hu3DMotionSpeedSet(back->model20, 1.0f);
            Hu3DModelAttrReset(back->model20, 1);
            back->bobOn = 0;
        }
        if (w->evtTimer == 0xA) {
            fn_1_814(0x8B8);
        }
        if (w->evtTimer == 0x28) {
            fn_1_814(0x8A3);
        }
        if (w->evtTimer < 0x28) {
            back->reflectOn = 1;
            back->rippleOn = 0;
        } else if (w->evtTimer < 0x50) {
            back->reflectOn = 1;
            back->rippleOn = 1;
        } else {
            back->reflectOn = 0;
            back->rippleOn = 0;
        }
        if (w->evtTimer == 0x50) {
            fn_1_814(0x8BF);
        }
        if (w->evtTimer >= 0x50 && w->evtTimer < 0x64) {
            k = w->evtTimer - 0x50;
            fn_1_277C8(w, (f32)k / 20.0f);
        }
        w->evtTimer++;
        if (w->evtTimer >= 0x78) {
            koopa->visibleReq = 1;
            koopa->state = 3;
            koopa->sub = 0;
            koopa->active = 0;
            fn_1_22694(&w->light, 2, 1.0f);
            fn_1_B578(koopa);
            tmp.x = 0.0f;
            tmp.y = 1000.0f;
            tmp.z = -125.0f;
            koopa->pos = tmp;
            Hu3DModelPosSet(koopa->modelId, koopa->pos.x, koopa->pos.y, koopa->pos.z);
            Hu3DModelScaleSet(koopa->modelId, 0.0f, 0.0f, 0.0f);
            koopa->sub = 1;
            koopa->reqMotion = 0;
            w->evtTimer = 0;
            w->state++;
        }
        break;
    case 13:
        t = (f32)w->evtTimer / 20.0f;
        koopa->pos.y = 1000.0f;
        Hu3DModelScaleSet(koopa->modelId, 1.5f * t, 1.5f * t, 1.5f * t);
        if (++w->evtTimer >= 0x14) {
            koopa->loopMotion = 0;
            koopa->reqMotion = 3;
            koopa->curMotion = 3;
            Hu3DModelAttrReset(koopa->modelId, HU3D_MOTATTR_LOOP);
            attr = koopa->loopMotion != 0 ? HU3D_MOTATTR_LOOP : 0;
            Hu3DMotionShiftSet(koopa->modelId, koopa->motionId[koopa->curMotion], 0.0f, 4.0f, attr);
            Hu3DMotionTimeSet(koopa->modelId, 40.0f);
            Hu3DModelScaleSet(koopa->modelId, 1.5f, 1.5f, 1.5f);
            w->evtTimer = 0;
            w->state++;
        }
        break;
    case 14:
        t = (f32)w->evtTimer / 20.0f;
        if (t > 1.0f) {
            clamped = 1.0f;
        } else if (t < 0.0f) {
            clamped = 0.0f;
        } else {
            clamped = t;
        }
        koopa->pos.x = 0.0f;
        koopa->pos.y = 1000.0f * (1.0f - clamped);
        koopa->pos.z = -125.0f;
        w->evtTimer++;
        if (w->evtTimer == 20) {
            fn_1_21B68(camera, 1, 0x32, 40.0f, 0.95f, 90.0f);
            fn_1_2DC08((s16)camera->camMask, &koopa->pos, 0);
            omVibrate(player->charIdx, 0x14, 0xC, 0);
            cell = &field->tile[7];
            cell->flag = 1;
            field->quakePos = cell->pos;
            field->quakeTimer = 1;
            fn_1_814(0x8A7);
        } else if (w->evtTimer == 40) {
            koopa->reqMotion = 0;
            koopa->loopMotion = 1;
        } else if (w->evtTimer >= 60) {
            w->evtTimer = 0;
            w->state = 9;
        }
        break;
    }
    return done;
}

/* Shakes the backdrop while Bowser is dropped in: spins its three models and
   pushes the bob position down, easing off over the last part of the drop. */
void fn_1_277C8(M562Work *w, f32 t)
{
    M562Back *back = &w->back;
    f32 k;

    if (t < 0.7f) {
        k = 1.0f;
    } else {
        k = 1.0f - ((t - 0.7f) / 0.3f);
    }
    back->bobPos.y -= 6.0f * k;
    lbl_1_bss_7958 += 0.5f * k;
    Hu3DModelRotSet(back->model1A, 0.0f, 0.0f, lbl_1_bss_7958);
    Hu3DModelRotSet(back->model1C, 0.0f, 0.0f, lbl_1_bss_7958);
    Hu3DModelRotSet(back->model20, 0.0f, 0.0f, lbl_1_bss_7958);
}
