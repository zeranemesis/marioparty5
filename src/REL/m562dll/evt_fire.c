#include "REL/m562dll.h"
#include "game/gamework.h"
#include "game/window.h"
#include <math.h>

/* camera goals: [0] first run, [1] later runs (pos / target pairs) */
static Vec lbl_1_data_CC8[2] = {
    { 0.0f, 347.9f, 26.0f },
    { 0.0f, 1000.0f, 1400.0f },
};
static Vec lbl_1_data_CE0[2] = {
    { 0.0f, 346.54f, 21.37f },
    { 0.0f, 100.0f, -200.0f },
};

static s16 lbl_1_bss_7930;
static Vec lbl_1_bss_7934;
static Vec lbl_1_bss_7940;
static Vec lbl_1_bss_794C;

/* Fire stage: Bowser walks to the stage centre while the camera pans, speaks,
   and the three fire rings rise up around him. */
s32 fn_1_252B4(M562Work *w)
{
    M562Player *player = &w->player;
    M562Back *back = &w->back;
    CAMERA *camera = (CAMERA *)&w->camera[0];
    Koopa *koopa = (Koopa *)&w->koopa;
    FireRingMgr *ring = &w->fireRing;
    M562PlayerInput *input = &player->input;
    FireRing *e0 = &ring->ring[0];
    FireRing *e1 = &ring->ring[1];
    FireRing *e2 = &ring->ring[2];
    s32 done = 0;
    f32 t;
    f32 mag;
    f32 y;
    Vec diff;
    Vec goal;
    Vec dir;

    switch (w->state) {
    case 0:
        back->reflectOn = 1;
        back->rippleOn = 0;
        fn_1_16F0(w, 1);
        lbl_1_bss_794C = e0->pos;
        lbl_1_bss_7940 = e1->pos;
        lbl_1_bss_7934 = e2->pos;
        goal.x = 0.0f;
        goal.y = 1000.0f;
        goal.z = 0.0f;
        e2->pos = goal;
        e1->pos = e2->pos;
        e0->pos = e1->pos;
        player->reqMotion = 1;
        player->loopMotion = 1;
        if (GwSystem.subGameNo >= 1) {
            camera->mode = 1;
            camera->posGoal = lbl_1_data_CC8[1];
            camera->tgtGoal = lbl_1_data_CE0[1];
            camera->prevMode = 0;
            camera->speed = 0.04f;
            camera->slerpT = 0.04f;
            w->evtTimer = 0;
            w->state = 0xA;
            break;
        }
        camera->mode = 1;
        camera->posGoal = lbl_1_data_CC8[0];
        camera->tgtGoal = lbl_1_data_CE0[0];
        camera->prevMode = 0;
        camera->speed = 0.04f;
        camera->slerpT = 0.03f;
        w->state++;
        /* fallthrough */
    case 1:
        t = (f32)w->evtTimer / 120.0f;
        fn_1_22694(&w->light, 1, t);
        fn_1_4748(back, 1, t);
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
        goal.z = 0.0f;
        PSVECSubtract(&goal, &player->pos, &diff);
        diff.y = 0.0f;
        mag = PSVECMag(&diff);
        if (mag > 40.0f) {
            PSVECNormalize(&diff, &diff);
            input->btn = 0;
            input->btnDown = 0;
            input->stkX = 0;
            input->stkY = 0;
            input->stkX = 56.0f * diff.x;
            input->stkY = 56.0f * -diff.z;
            fn_1_7D58(player);
        } else {
            player->reqMotion = 0;
            player->loopMotion = 1;
        }
        if (w->evtTimer++ > 0x78) {
            if (mag <= 50.0f) {
                goal.x = 0.0f;
                goal.y = 0.0f;
                goal.z = 0.0f;
                player->pos = goal;
                player->reqMotion = 0;
                player->loopMotion = 1;
                player->rotY = 0.0f;
                player->goalRotY = 0.0f;
                player->rot.y = player->rotY;
                w->evtTimer = 0;
                w->state++;
            }
        }
        break;
    case 2:
        HuWinExOpen(w->winId);
        HuWinMesSet(w->winId, 0x410001);
        koopa->reqMotion = 5;
        koopa->pendingDelay = 0x1E;
        koopa->loopMotion = 1;
        koopa->sub = 0xA;
        fn_1_814(0x8C5);
        lbl_1_bss_7930 = 0;
        w->evtTimer = 0;
        w->state++;
        break;
    case 3:
        if (HuWinStatGet(w->winId) == 2) {
            lbl_1_bss_7930 = 0;
        } else if (HuWinStatGet(w->winId) == 1) {
            if (++lbl_1_bss_7930 == 5) {
                koopa->curMotion = 0;
                koopa->reqMotion = 5;
                koopa->loopMotion = 1;
            }
        } else if (HuWinStatGet(w->winId) == 0) {
            lbl_1_bss_7930 = 0;
            koopa->reqMotion = 0;
            koopa->loopMotion = 1;
            player->reqMotion = 0x1A;
            player->loopMotion = 1;
            camera->prevMode = 0;
            camera->posGoal = lbl_1_data_CC8[1];
            camera->tgtGoal = lbl_1_data_CE0[1];
            camera->speed = 0.06f;
            camera->slerpT = 0.06f;
            HuWinExClose(w->winId);
            HuWinHomeClear(w->winId);
            ring->active = 1;
            w->evtTimer = 0;
            w->state++;
        }
        break;
    case 4:
        Hu3DMotionSpeedSet(player->modelId, 3.0f);
        y = 40.0f + 1000.0f * (1.0f - (f32)w->evtTimer / 60.0f);
        e2->pos.y = y;
        e1->pos.y = y;
        e0->pos.y = y;
        if (w->evtTimer == 0x14 && ring->sound < 0) {
            ring->sound = fn_1_814(0x8A2);
        }
        if (++w->evtTimer > 0x3C) {
            fn_1_21B68(camera, 1, 0x32, 30.0f, 0.55f, 90.0f);
            omVibrate(player->playerNo, 10, 12, 0);
            Hu3DMotionSpeedSet(player->modelId, 1.0f);
            w->evtTimer = 0;
            w->state++;
        }
        break;
    case 5:
        t = (f32)w->evtTimer / 60.0f;
        input->stkY = 0;
        input->stkX = 0;
        input->btnDown = 0;
        input->btn = 0;
        if (w->evtTimer >= 10) {
            if (w->evtTimer == 10) {
                input->btnDown = 0x100;
            }
            input->btn = 0x100;
        }
        fn_1_7D58(player);
        e0->pos.x = lbl_1_bss_794C.x * t;
        e0->pos.z = lbl_1_bss_794C.z * t;
        e1->pos.x = lbl_1_bss_7940.x * t;
        e1->pos.z = lbl_1_bss_7940.z * t;
        e2->pos.x = lbl_1_bss_7934.x * t;
        e2->pos.z = lbl_1_bss_7934.z * t;
        if (++w->evtTimer > 0x3C) {
            input->stkY = 0;
            input->stkX = 0;
            input->btnDown = 0;
            input->btn = 0;
            player->state = 0;
            player->sub = 0;
            w->evtTimer = 0;
            w->state++;
        }
        break;
    case 6:
        player->reqMotion = 0x1C;
        player->loopMotion = 0;
        if (++w->evtTimer > 0x1E) {
            w->evtTimer = 0;
            w->state++;
        }
        break;
    case 7:
        if (w->seId >= 0) {
            fn_1_83C(w->seId);
            w->seId = -1;
        }
        if (GwSystem.subGameNo < 1) {
            GwSystem.subGameNo = 1;
        }
        player->sub = 0;
        player->state = 0;
        w->state = 0;
        done = 1;
        break;
    case 10:
        t = (f32)w->evtTimer / 120.0f;
        fn_1_22694(&w->light, 1, t);
        fn_1_4748(back, 1, t);
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
        goal.z = 0.0f;
        PSVECSubtract(&goal, &player->pos, &diff);
        diff.y = 0.0f;
        mag = PSVECMag(&diff);
        if (mag > 5.0f) {
            PSVECNormalize(&diff, &diff);
            input->btn = 0;
            input->btnDown = 0;
            input->stkX = 0;
            input->stkY = 0;
            input->stkX = 56.0f * diff.x;
            input->stkY = 56.0f * -diff.z;
            fn_1_7D58(player);
        } else {
            dir.x = 0.0f;
            dir.y = 0.0f;
            dir.z = 1.0f;
            if (fn_1_AD98(player, &dir, 0.9f) != 0) {
                player->reqMotion = 0;
                player->loopMotion = 1;
            }
        }
        w->evtTimer++;
        if (w->evtTimer > 0x78) {
            if (mag <= 10.0f) {
                goal.x = 0.0f;
                goal.y = 0.0f;
                goal.z = 0.0f;
                player->pos = goal;
                player->rotY = 0.0f;
                player->goalRotY = 0.0f;
                player->rot.y = player->rotY;
                w->evtTimer = 0;
                w->state++;
            }
        }
        break;
    case 11:
        dir.x = 0.0f;
        dir.y = 0.0f;
        dir.z = 1.0f;
        if (fn_1_AD98(player, &dir, 0.9f) != 0) {
            player->reqMotion = 0x1A;
            player->loopMotion = 1;
            ring->active = 1;
            w->evtTimer = 0;
            w->state = 4;
        }
        break;
    }
    return done;
}
