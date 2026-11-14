#include "REL/m562dll.h"
#include "game/gamework.h"
#include "game/wipe.h"
#include "game/charman.h"
#include <math.h>

s32 fn_1_2C648(M562Work *w);
s32 fn_1_2CB80(Vec *p, Vec *a, Vec *b, f32 *dist);

static Vec lbl_1_data_F88 = { 0.0f, 0.0f, 0.0f };
static Vec lbl_1_data_F94 = { 0.0f, 0.0f, 0.0f };

/* Result scene: the camera settles on the winner, the loser is sent off, then
   the wipe closes the battle. */
s32 fn_1_2C118(M562Work *w)
{
    M562EvtPlayer *player = (M562EvtPlayer *)&w->player;
    CAMERA *camera = (CAMERA *)&w->camera[0];
    s32 done = 0;
    s32 ok;
    f32 t;
    f32 clamped;
    f32 mag;
    Vec d;

    switch (w->state) {
    case 0:
        if (w->bgmId >= 0) {
            fn_1_9F4(w->bgmId, 0x64);
            w->bgmId = -1;
        }
        if (lbl_1_bss_0->seId2 >= 0) {
            fn_1_864(lbl_1_bss_0->seId2, 0x3E8);
        }
        fn_1_1D9C(w);
        w->evtTimer = 0;
        w->state++;
        /* fallthrough */
    case 1:
        lbl_1_data_F88 = camera->pos;
        lbl_1_data_F94 = camera->tgt;
        camera->mode = 0;
        camera->speed = 0.2f;
        w->evtTimer = 0;
        w->state++;
        break;
    case 2:
        t = (f32)w->evtTimer / 60.0f;
        if (t > 1.0f) {
            clamped = 1.0f;
        } else if (t < 0.0f) {
            clamped = 0.0f;
        } else {
            clamped = t;
        }
        camera->tgtGoal.x = lbl_1_data_F94.x + clamped * (player->pos.x - lbl_1_data_F94.x);
        camera->tgtGoal.y = lbl_1_data_F94.y + clamped * (player->pos.y - lbl_1_data_F94.y);
        camera->tgtGoal.z = lbl_1_data_F94.z + clamped * (player->pos.z - lbl_1_data_F94.z);
        PSVECSubtract(&camera->tgtGoal, &camera->posGoal, &d);
        mag = PSVECMag(&d);
        if (mag > 1000.0f) {
            PSVECNormalize(&d, &d);
            PSVECScale(&d, &d, 0.04f * (mag - 1000.0f));
            PSVECAdd(&camera->posGoal, &d, &camera->posGoal);
        }
        d.x = 0.0f;
        d.y = 0.0f;
        d.z = 1.0f;
        if (fn_1_AD98(player, &d, 0.9f) != 0) {
            player->animNo = 0;
            player->animLoop = 1;
        }
        ok = 1;
        if (w->mode == 3) {
            ok = fn_1_2C648(w);
        }
        w->evtTimer++;
        if ((w->evtTimer >= 0x3C && ok != 0) || w->evtTimer >= 0xB4) {
            fn_1_A6C(0x7A);
            if (w->winner == 2) {
                CharLoseVoicePlay(player->charNo, -1, -1, -1);
                player->animNo = 0x19;
                player->animLoop = 0;
                w->evtTimer = 0;
                w->state = 0xA;
            } else {
                w->evtTimer = 0;
                w->state++;
            }
        }
        break;
    case 3:
        w->evtTimer = 0;
        w->state++;
        break;
    case 4:
        player->animNo = 0x1D;
        player->animLoop = 0;
        if (++w->evtTimer >= 0x78) {
            WipeCreate(2, 0x89, 0x3C);
            fn_1_814(0x8B6);
            w->evtTimer = 0;
            w->state = 0x14;
        }
        break;
    case 10:
        player->animNo = 0x19;
        player->animLoop = 0;
        if (++w->evtTimer >= 0xB4) {
            WipeCreate(2, 0x89, 0x3C);
            fn_1_814(0x8B6);
            w->evtTimer = 0;
            w->state = 0x14;
        }
        break;
    case 20:
        if (WipeCheckEnd() == 0) {
            w->evtTimer = 0;
            w->state++;
        }
        break;
    case 21:
        if (++w->evtTimer >= 0x5A) {
            w->evtTimer = 0;
            w->state = 0;
            done = 1;
        }
        break;
    }
    return done;
}

/* Bowser walks away from the camera when the player stands in his way:
   returns 1 when nothing is in the way (or he is done moving), else 0. */
s32 fn_1_2C648(M562Work *w)
{
    Koopa *koopa = (Koopa *)&w->koopa;
    M562EvtPlayer *player = (M562EvtPlayer *)&w->player;
    CAMERA *camera = (CAMERA *)&w->camera[0];
    s32 ret = 1;
    Vec playerPos;
    Vec koopaPos;
    Vec camPos;
    Vec dPlayer;
    Vec dKoopa;
    Vec dir;
    f32 distP;
    f32 distK;
    f32 near;
    f32 ang;
    f32 t;
    f32 clamped;

    playerPos = player->pos;
    koopaPos = koopa->pos;
    camPos = camera->posGoal;
    koopaPos.y = 0.0f;
    camPos.y = 0.0f;
    playerPos.y = 0.0f;
    PSVECSubtract(&camPos, &playerPos, &dPlayer);
    distP = PSVECMag(&dPlayer);
    PSVECSubtract(&camPos, &koopaPos, &dKoopa);
    distK = PSVECMag(&dKoopa);
    koopa->reqMotion = 0;
    koopa->loopMotion = 1;
    if (distK < 150.0f + distP) {
        near = 0.0f;
        fn_1_2CB80(&koopaPos, &camPos, &playerPos, &near);
        if (near < 200.0f) {
            koopa->reqMotion = 1;
            koopa->loopMotion = 1;
            PSVECSubtract(&koopaPos, &playerPos, &dir);
            if (dir.x != 0.0f || dir.z != 0.0f) {
                PSVECNormalize(&dir, &dir);
            } else {
                dir.z = 1.0f;
            }
            if ((f32)(180.0 * (atan2(dir.x, dir.z) / M_PI)) > 180.0f) {
                ang = (180.0 * (atan2(dir.x, dir.z) / M_PI)) - 360.0;
            } else if ((f32)(180.0 * (atan2(dir.x, dir.z) / M_PI)) <= -180.0f) {
                ang = 360.0 + (180.0 * (atan2(dir.x, dir.z) / M_PI));
            } else {
                ang = 180.0 * (atan2(dir.x, dir.z) / M_PI);
            }
            koopa->goalRotY = ang;
            if (koopa->pos.z <= -400.0f || koopa->pos.z >= 400.0f) {
                if (koopaPos.x > 0.0f) {
                    koopa->goalRotY = -90.0f;
                } else {
                    koopa->goalRotY = 90.0f;
                }
            }
            koopa->speed = 8.5f;
            t = (koopa->speed - 4.5f) * 0.25f;
            if (t > 1.0f) {
                clamped = 1.0f;
            } else if (t < 0.0f) {
                clamped = 0.0f;
            } else {
                clamped = t;
            }
            Hu3DMotionSpeedSet(koopa->modelId, 1.0 + (f64)clamped);
            koopa->rotY = fn_1_36EC(koopa->goalRotY, koopa->rotY, 0.95f);
            dir.x = koopa->speed * sin(M_PI * (f64)koopa->rotY / 180.0);
            dir.z = koopa->speed * cos(M_PI * (f64)koopa->rotY / 180.0);
            koopa->pos.x += dir.x;
            koopa->pos.z += dir.z;
            koopa->rot.y = koopa->rotY;
            ret = 0;
        }
    }
    return ret;
}

/* Distance from p to the segment a..b; returns 1 when the closest point lies
   inside the segment. */
s32 fn_1_2CB80(Vec *p, Vec *a, Vec *b, f32 *dist)
{
    Vec d;
    Vec closest;
    Vec v;
    f32 dot;
    f32 t;
    f32 clamped;
    s32 inside;

    PSVECSubtract(b, a, &d);
    dot = PSVECDotProduct(a, &d);
    t = (-dot + PSVECDotProduct(p, &d)) / (d.x * d.x + d.y * d.y + d.z * d.z);
    inside = 0;
    if (t >= 0.0f && t <= 1.0f) {
        inside = 1;
    }
    if (t > 1.0f) {
        clamped = 1.0f;
    } else if (t < 0.0f) {
        clamped = 0.0f;
    } else {
        clamped = t;
    }
    closest.x = a->x + clamped * d.x;
    closest.y = a->y + clamped * d.y;
    closest.z = a->z + clamped * d.z;
    PSVECSubtract(&closest, p, &v);
    *dist = PSVECMag(&v);
    return inside;
}
