#include "REL/m562dll.h"
#include "game/gamework.h"
#include "game/frand.h"
#include "game/window.h"
#include "game/charman.h"
#include <math.h>

/* camera goals (pos / target pairs, one per shot) */
static Vec lbl_1_data_C30[4] = {
    { 0.0f, 420.0f, 2040.0f },
    { 0.0f, 500.0f, 578.0f },
    { 0.0f, 347.9f, 26.0f },
    { 0.0f, 1000.0f, 1400.0f },
};
static Vec lbl_1_data_C60[4] = {
    { 0.0f, 580.0f, 0.0f },
    { 0.0f, 80.0f, 0.0f },
    { 0.0f, 346.54f, 21.37f },
    { 0.0f, 100.0f, -200.0f },
};
static s32 lbl_1_data_C90 = -1;

static s16 lbl_1_bss_7900;
static s16 lbl_1_bss_7902;
static f32 lbl_1_bss_7904[3];
static f32 lbl_1_bss_7910[3];
static Vec lbl_1_bss_791C;
static s16 lbl_1_bss_7928[4];

/* Opening: Bowser is introduced, speaks, and the fire creatures that will take
   part are shown on the floor three at a time. */
s32 fn_1_2446C(M562Work *w)
{
    M562EvtPlayer *player = (M562EvtPlayer *)&w->player;
    M562Back *back = &w->back;
    CAMERA *camera = (CAMERA *)&w->camera[0];
    Koopa *koopa = (Koopa *)&w->koopa;
    FireMgr *fire = (FireMgr *)&w->fire;
    M562MainTriple *triple = &w->triple;
    s32 done = 0;
    s32 i;
    s16 idx;
    s16 clampIdx;
    s32 k;
    f32 t;
    Vec v;
    Vec dir;
    Fire *f;

    switch (w->state) {
    case 0:
        fn_1_16F0(w, 0);
        lbl_1_bss_0->seId2 = fn_1_814(0x8CA);
        if (GwSystem.subGameNo >= 0) {
            back->reflectOn = 1;
            back->rippleOn = 0;
            player->animNo = 0x1C;
            player->animLoop = 0;
            player->rotGoal = 180.0f;
            player->rotSpeed = 180.0f;
            player->rotY = player->rotGoal;
            done = 1;
        } else {
            player->animNo = 0x1E;
            player->animLoop = 0;
            w->sprite[0].active = 0;
            for (i = 0; i < 0x23; i++) {
                f = &fire->fire[i];
                Hu3DModelAttrSet(f->model, HU3D_ATTR_DISPOFF);
                Hu3DModelAttrSet(f->shadow, HU3D_ATTR_DISPOFF);
            }
            back->reflectOn = 0;
            back->rippleOn = 0;
            Hu3DModelAttrSet(triple->model, HU3D_ATTR_DISPOFF);
            for (i = 0; i < 3; i++) {
                s16 id = Hu3DModelLink(triple->model);
                lbl_1_bss_7928[i] = id;
                Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
                Hu3DModelScaleSet(id, 15.0f, 15.0f, 15.0f);
                Hu3DModelCameraSet(id, (u16)camera->camMask);
                Hu3DModelLayerSet(id, 4);
                Hu3DModelAttrSet(id, HU3D_ATTR_DISPOFF);
                Hu3DMotionSet(id, triple->motion);
                Hu3DModelAttrSet(id, HU3D_MOTATTR_LOOP);
                lbl_1_bss_7910[i] = 360.0f * ((f32)(frand() % 10000) / 10000.0f);
                lbl_1_bss_7904[i] = 30.0f * ((f32)(frand() % 10000) / 10000.0f);
            }
            camera->mode = 1;
            camera->pos = lbl_1_data_C30[0];
            camera->posGoal = camera->pos;
            camera->tgt = lbl_1_data_C60[0];
            camera->tgtGoal = camera->tgt;
            camera->prevMode = 0;
            camera->posGoal = lbl_1_data_C30[1];
            camera->tgtGoal = lbl_1_data_C60[1];
            camera->speed = 0.04f;
            camera->slerpT = 0.03f;
            lbl_1_data_C90 = fn_1_814(0x8B5);
            Hu3DMotionSpeedSet(back->model40, 1.25f);
            w->evtTimer = 0;
            w->state++;
        }
        break;
    case 1:
        Hu3DMotionTimeSet(player->motion, 0.0f);
        if (w->evtTimer++ > 0xB4) {
            w->evtTimer = 0;
            w->state++;
        }
        break;
    case 2:
        if (w->evtTimer == 5) {
            CharFXPlay(player->charNo, 0x9E);
        }
        if (++w->evtTimer >= 0x1E) {
            player->animNo = 0x1A;
            player->animLoop = 0;
            w->evtTimer = 0;
            w->state++;
        }
        break;
    case 3:
        Hu3DMotionSpeedSet(player->motion, 3.0f);
        if (++w->evtTimer >= 0x28) {
            Hu3DMotionSpeedSet(player->motion, 1.0f);
            player->animNo = 1;
            player->animLoop = 1;
            w->evtTimer = 0;
            w->state++;
        }
        break;
    case 4:
        dir.x = 0.0f;
        dir.y = 0.0f;
        dir.z = -1.0f;
        if (fn_1_AD98(player, &dir, 0.9f) != 0) {
            player->rotGoal = 180.0f;
            player->rotSpeed = 180.0f;
            player->animNo = 0;
            player->animLoop = 1;
            w->evtTimer = 0;
            w->state++;
        }
        break;
    case 5:
        if (++w->evtTimer >= 0x1E) {
            camera->prevMode = 0;
            camera->posGoal = lbl_1_data_C30[2];
            camera->tgtGoal = lbl_1_data_C60[2];
            camera->speed = 0.05f;
            camera->slerpT = 0.06f;
            w->evtTimer = 0;
            w->state++;
        }
        break;
    case 6:
        if (w->evtTimer++ > 0x3C) {
            camera->posGoal = lbl_1_data_C30[2];
            camera->tgtGoal = lbl_1_data_C60[2];
            fn_1_814(0x8C2);
            w->evtTimer = 0;
            w->state++;
        }
        break;
    case 7:
        if (w->evtTimer < 0x14) {
            back->reflectOn = 1;
            back->rippleOn = 1;
        } else {
            back->reflectOn = 1;
            back->rippleOn = 0;
        }
        w->evtTimer++;
        if (w->evtTimer > 0x3C) {
            back->rippleOn = 0;
            w->evtTimer = 0;
            w->state++;
        }
        break;
    case 8:
        HuWinExOpen(w->winId);
        HuWinMesSet(w->winId, 0x410000);
        koopa->reqMotion = 5;
        koopa->loopMotion = 1;
        koopa->pendingDelay = 0x1E;
        koopa->sub = 0xA;
        fn_1_814(0x8B3);
        lbl_1_bss_7900 = 0;
        w->evtTimer = 0;
        w->state++;
        break;
    case 9:
        if (HuWinStatGet(w->winId) == 2) {
            lbl_1_bss_7900 = 0;
        } else if (HuWinStatGet(w->winId) == 1) {
            if (++lbl_1_bss_7900 == 5) {
                koopa->curMotion = 0;
                koopa->reqMotion = 5;
                koopa->loopMotion = 1;
            }
        } else if (HuWinStatGet(w->winId) == 0) {
            lbl_1_bss_7900 = 0;
            koopa->reqMotion = 0;
            koopa->loopMotion = 1;
            player->animNo = 0x1A;
            player->animLoop = 1;
            camera->prevMode = 0;
            camera->posGoal = lbl_1_data_C30[3];
            camera->tgtGoal = lbl_1_data_C60[3];
            camera->speed = 0.06f;
            camera->slerpT = 0.06f;
            HuWinExClose(w->winId);
            HuWinHomeClear(w->winId);
            lbl_1_bss_7902 = -1;
            w->evtTimer = 0;
            w->state++;
        }
        break;
    case 10:
        Hu3DMotionSpeedSet(player->motion, 3.0f);
        idx = (f32)w->evtTimer / 120.0f * (f32)((fire->count + 2) / 3);
        if (idx > (fire->count + 2) / 3) {
            clampIdx = (fire->count + 2) / 3;
        } else if (idx < 0) {
            clampIdx = 0;
        } else {
            clampIdx = idx;
        }
        if (clampIdx != lbl_1_bss_7902 && fire->fire[clampIdx * 3].state != 7) {
            fn_1_814(0x8B7);
            fn_1_21B68(camera, 1, 0x32, 30.0f, 0.55f, 90.0f);
            omVibrate(player->charIdx, 10, 12, 0);
        }
        lbl_1_bss_7902 = clampIdx;
        for (i = 0; i < 3; i++) {
            k = i + clampIdx * 3;
            f = &fire->fire[k];
            if (f->state != 7 && k < 0x23) {
                Hu3DModelAttrReset(f->model, HU3D_ATTR_DISPOFF);
                Hu3DModelAttrReset(f->shadow, HU3D_ATTR_DISPOFF);
                v.x = 0.0f;
                v.y = 0.0f;
                v.z = 0.0f;
                v.x = f->pos.x;
                v.z = f->pos.z;
                lbl_1_bss_791C = v;
                Hu3DModelPosSet(lbl_1_bss_7928[i], lbl_1_bss_791C.x, lbl_1_bss_791C.y, lbl_1_bss_791C.z);
                Hu3DModelAttrReset(lbl_1_bss_7928[i], HU3D_ATTR_DISPOFF);
            } else {
                Hu3DModelAttrSet(lbl_1_bss_7928[i], HU3D_ATTR_DISPOFF);
            }
            Hu3DModelRotSet(lbl_1_bss_7928[i], lbl_1_bss_7904[i], lbl_1_bss_7910[i], 0.0f);
            lbl_1_bss_7910[i] += 6.0f;
        }
        if (++w->evtTimer > 0x78) {
            Hu3DMotionSpeedSet(player->motion, 1.0f);
            player->animNo = 0x1C;
            player->animLoop = 0;
            for (i = 0; i < 3; i++) {
                Hu3DModelAttrSet(lbl_1_bss_7928[i], HU3D_ATTR_DISPOFF);
            }
            w->evtTimer = 0;
            w->state++;
        }
        break;
    case 11:
        t = (f32)w->evtTimer / 30.0f;
        Hu3DMotionSpeedSet(back->model40, 0.25 + sin(M_PI * (f64)(90.0f + 90.0f * t) / 180.0));
        if (w->evtTimer++ > 0x1E) {
            for (i = 0; i < 3; i++) {
                Hu3DModelKill(lbl_1_bss_7928[i]);
            }
            fn_1_864(lbl_1_data_C90, 0x3E8);
            lbl_1_data_C90 = -1;
            if (GwSystem.subGameNo < 0) {
                GwSystem.subGameNo = 0;
            }
            w->evtTimer = 0;
            w->sprite[0].active = 1;
            done = 1;
        }
        break;
    }
    return done;
}
