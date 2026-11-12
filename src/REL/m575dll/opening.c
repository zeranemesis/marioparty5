// REL/m575dll/opening.c - the opening cut-scene: the players run in, the camera
// flies along a spline, and the screen is split between the two sides.
#include "REL/m575dll.h"
#include "game/hu3d.h"
#include "game/object.h"
#include "string.h"
#include "math.h"
#include "humath.h"

s32 fn_1_530(s32 seId);
void fn_1_558(s32 seNo);
void fn_1_5C88(M575Player *self, s32 on);
s32 fn_1_E050(M575Game *game, M575Player *player);

// Camera spline: points the camera passes through and the segment lengths.
Vec lbl_1_data_310[6] = {
    { 50.0f, 70.0f, -300.0f },
    { 100.0f, 75.0f, -300.0f },
    { 200.0f, 175.0f, -500.0f },
    { 300.0f, 275.0f, -600.0f },
    { 1000.0f, 500.0f, -300.0f },
    { 0.0f, 1370.0f, 1270.0f },
};

// Look-at spline (relative to the first player).
Vec lbl_1_data_358[6] = {
    { 0.0f, 70.0f, 0.0f },
    { 0.0f, 70.0f, 0.0f },
    { 0.0f, 70.0f, 0.0f },
    { 0.0f, 70.0f, 0.0f },
    { 0.0f, 100.0f, 0.0f },
    { 0.0f, 200.0f, 0.0f },
};

M575Path lbl_1_bss_7820;
M575Path lbl_1_bss_79B4;
u8 lbl_1_bss_79B4_pad[0x30];
Vec lbl_1_bss_7B78;
Vec lbl_1_bss_7B84;
f32 lbl_1_bss_7B90;
f32 lbl_1_bss_7B90_pad;

static const s32 splineLen[6] = { 0x28, 0x28, 0x14, 0x3C, 0x3C, 0x28 };

s32 fn_1_D00C(M575Game *game)
{
    M575Main *side0 = &game->side[0];
    M575Player *p0 = &side0->player[0];
    M575Player *p1 = &side0->player[1];
    M575Camera *cam0 = &side0->camera;
    M575Camera *cam1 = &game->side[1].camera;
    M575Ten *ten = &side0->ten;
    M575TuboEff *eff = &ten->eff;
    Vec tmp;
    Vec v;
    Vec up0 = { 0.0f, 1.0f, 0.0f };
    Vec up;
    Vec row;
    Vec flat;
    Vec axis;
    Mtx mtx;
    Quaternion q0, q1, q2;
    s32 ret = 0;
    s32 d0;
    s32 d1;
    s32 i;
    s32 j;
    f32 ratio;
    f32 clamped;
    f32 lo;
    f32 yy;
    f32 rotAbs;
    f32 rotAbs2;
    f32 rotWrap;
    f32 rotWrapLo;
    f32 s;

    switch (game->phase) {
    case 0:
        fn_1_5C88(p0, 0);
        fn_1_5C88(p1, 0);
        Hu3DMotionShiftSet(p0->modelId, p0->motion[2], 0.0f, 4.0f, 0x40000001);
        Hu3DMotionShiftSet(p1->modelId, p1->motion[2], 0.0f, 4.0f, 0x40000001);
        tmp.x = 0.0f;
        tmp.y = 0.0f;
        tmp.z = 5200.0f;
        *p0->pos = tmp;
        tmp.x = 0.0f;
        tmp.y = -40.0f;
        tmp.z = 5400.0f;
        *p1->pos = tmp;
        Hu3DModelRotSet(p0->modelId, 0.0f, 180.0f, 0.0f);
        Hu3DModelRotSet(p1->modelId, 0.0f, 180.0f, 0.0f);
        p1->state = 0;
        p0->state = 0;
        p1->stateTimer = 0;
        p0->stateTimer = 0;
        lbl_1_bss_7B84 = cam0->posTarget;
        lbl_1_bss_7B78 = cam0->tgtTarget;
        fn_1_31E8(&lbl_1_bss_79B4);
        fn_1_31E8(&lbl_1_bss_7820);
        for (i = 0; i < 6; i++) {
            tmp = lbl_1_data_310[i];
            fn_1_3248(&lbl_1_bss_79B4, &tmp, splineLen[i]);
        }
        for (i = 0; i < 6; i++) {
            tmp = lbl_1_data_358[i];
            fn_1_3248(&lbl_1_bss_7820, &tmp, splineLen[i]);
        }
        v = lbl_1_data_310[0];
        cam0->pos.x = v.x;
        cam0->posTarget.x = v.x;
        yy = p0->pos->y + v.y;
        cam0->pos.y = yy;
        cam0->posTarget.y = yy;
        yy = p0->pos->z + v.z;
        cam0->pos.z = yy;
        cam0->posTarget.z = yy;
        v = lbl_1_data_358[0];
        if (v.y < 0.5f * p0->height) {
            lo = 0.5f * p0->height;
        }
        else {
            lo = v.y;
        }
        v.y = lo;
        yy = p0->pos->x + v.x;
        cam0->tgt.x = yy;
        cam0->tgtTarget.x = yy;
        yy = p0->pos->y + v.y;
        cam0->tgt.y = yy;
        cam0->tgtTarget.y = yy;
        yy = p0->pos->z + v.z;
        cam0->tgt.z = yy;
        cam0->tgtTarget.z = yy;
        cam0->shakeOn = 1;
        for (i = 0; i < 0x1E; i++) {
            fn_1_E050(game, p0);
            fn_1_E050(game, p1);
            for (j = 0; j < 0x3C; j++) {
                fn_1_46B4(&side0->stage.item[j]);
            }
        }
        Hu3DMotionSet(side0->stage.modelId6, side0->stage.motion4);
        if (game->unk2610 < 0) {
            game->unk2610 = fn_1_530(0x92F);
        }
        game->wait = 0;
        game->phase++;
        break;
    case 1:
        d0 = fn_1_E050(game, p0);
        d1 = fn_1_E050(game, p1);
        fn_1_33C4(&v, &lbl_1_bss_79B4, game->wait);
        cam0->posTarget.x = v.x;
        cam0->posTarget.y = p0->pos->y + v.y;
        cam0->posTarget.z = p0->pos->z + v.z;
        fn_1_33C4(&v, &lbl_1_bss_7820, game->wait);
        if (v.y < 0.5f * p0->height) {
            lo = 0.5f * p0->height;
        }
        else {
            lo = v.y;
        }
        v.y = lo;
        cam0->tgtTarget.x = v.x;
        cam0->tgtTarget.y = p0->pos->y + v.y;
        cam0->tgtTarget.z = p0->pos->z + v.z;
        game->wait++;
        if (d0 != 0 && d1 != 0) {
            lbl_1_bss_7B90 = 0.0f;
            Hu3DModelAttrSet(side0->stage.modelId4, 1);
            cam0->posTarget = lbl_1_bss_7B84;
            cam0->tgtTarget = lbl_1_bss_7B78;
            game->wait = 0;
            game->phase++;
        }
        break;
    case 2:
        fn_1_E050(game, p0);
        fn_1_E050(game, p1);
        if (game->wait++ > 0x1E) {
            p1->pos->y = 0.0f;
            p0->pos->y = 0.0f;
            Hu3DMotionShiftSet(p0->modelId, p0->motion[1], 0.0f, 4.0f, 0x40000001);
            Hu3DMotionShiftSet(p1->modelId, p1->motion[1], 0.0f, 4.0f, 0x40000001);
            cam0->shakeOn = 0;
            game->wait = 0;
            game->phase++;
        }
        break;
    case 3:
        s = (f32)HuSin(90.0f * lbl_1_bss_7B90);
        fn_1_89D8(cam0, cam1, 0.5f * (s * s));
        lbl_1_bss_7B90 += 0.016666668f;
        if (!(lbl_1_bss_7B90 < 1.0f)) {
            fn_1_89D8(cam0, cam1, 0.5f);
            game->wait = 0;
            game->phase++;
        }
        break;
    case 4:
        if (game->unk2610 >= 0) {
            fn_1_558(game->unk2610);
            game->unk2610 = -1;
        }
        fn_1_3218(&lbl_1_bss_7820);
        fn_1_3218(&lbl_1_bss_79B4);
        cam0->lerp = 0.4f;
        p1->state = 0;
        p0->state = 0;
        p1->stateTimer = 0;
        p0->stateTimer = 0;
        game->phase = 0;
        ret = 1;
        break;
    }
    if (game->phase >= 2) {
        up = up0;
        row.x = eff->mtx[1][0];
        row.y = eff->mtx[1][1];
        row.z = eff->mtx[1][2];
        flat.x = row.x;
        flat.y = 0.0f;
        flat.z = row.z;
        ratio = 0.98f * (ten->rot - 0.5f);
        if (ratio > 90.0f) {
            clamped = 90.0f;
        }
        else {
            if (ratio < 0.0f) {
                lo = 0.0f;
            }
            else {
                lo = ratio;
            }
            clamped = lo;
        }
        PSVECNormalize(&flat, &flat);
        PSVECCrossProduct(&up, &flat, &axis);
        PSMTXRotAxisRad(mtx, &axis, 0.017453292f * clamped);
        PSMTXMultVec(mtx, &up, &flat);
        fn_1_2D5C(&q0, &row, &flat);
        C_QUATMtx(&q1, eff->mtx);
        PSQUATMultiply(&q0, &q1, &q2);
        PSMTXQuat(eff->mtx, &q2);
    }
    if (game->phase >= 3) {
        rotAbs = (p0->rot->y >= 0.0f) ? p0->rot->y : -p0->rot->y;
        if (rotAbs > 3.0f) {
            p0->rot->y = fn_1_30CC(0.0f, p0->rot->y, 0.9f);
            if (p0->rot->y > 180.0f) {
                rotWrap = p0->rot->y - 360.0f;
            }
            else {
                if (p0->rot->y <= -180.0f) {
                    rotWrapLo = 360.0f + p0->rot->y;
                }
                else {
                    rotWrapLo = p0->rot->y;
                }
                rotWrap = rotWrapLo;
            }
            p0->rot->y = rotWrap;
            rotAbs2 = (p0->rot->y >= 0.0f) ? p0->rot->y : -p0->rot->y;
            if (rotAbs2 <= 3.0f) {
                p0->rot->y = 0.0f;
                Hu3DMotionShiftSet(p0->modelId, p0->motion[0], 0.0f, 4.0f, 0x40000001);
            }
        }
        rotAbs = (p1->rot->y >= 0.0f) ? p1->rot->y : -p1->rot->y;
        if (rotAbs > 3.0f) {
            p1->rot->y = fn_1_30CC(0.0f, p1->rot->y, 0.9f);
            if (p1->rot->y > 180.0f) {
                rotWrap = p1->rot->y - 360.0f;
            }
            else {
                if (p1->rot->y <= -180.0f) {
                    rotWrapLo = 360.0f + p1->rot->y;
                }
                else {
                    rotWrapLo = p1->rot->y;
                }
                rotWrap = rotWrapLo;
            }
            p1->rot->y = rotWrap;
            rotAbs2 = (p1->rot->y >= 0.0f) ? p1->rot->y : -p1->rot->y;
            if (rotAbs2 <= 3.0f) {
                p1->rot->y = 0.0f;
                Hu3DMotionShiftSet(p1->modelId, p1->motion[0], 0.0f, 4.0f, 0x40000001);
            }
        }
    }
    if (cam0->shakeOn != 0 && game->frame % 5 == 0) {
        omVibrate(0, 2, 0xC, 0);
        omVibrate(1, 2, 0xC, 0);
        omVibrate(2, 2, 0xC, 0);
        omVibrate(3, 2, 0xC, 0);
    }
    return ret;
}

// Run-in of one player: slides in along z, hops in an arc onto the tube, and
// settles; returns 1 once the player has landed.
s32 fn_1_E050(M575Game *game, M575Player *player)
{
    M575TuboEff *eff = &player->main->ten.eff;
    Vec target;
    Vec delta;
    Vec impulse;
    Vec row;
    Vec scaled;
    f32 side;
    f32 ratio;
    f32 sx;
    s32 ret = 0;

    switch (player->state) {
    case 0:
        player->pos->z -= 20.0f;
        if (player->pos->z <= 700.0f) {
            player->pos->z = 700.0f;
            Hu3DMotionShiftSet(player->modelId, player->motion[3], 0.0f, 4.0f, 0);
            player->stateTimer = 0;
            player->state++;
        }
        break;
    case 1:
        player->pos->z -= 20.0f;
        target.x = 0.0f;
        target.y = 0.0f;
        target.z = 0.0f;
        if ((player->slot & 1) != 0) {
            side = 150.0f;
        }
        else {
            side = -150.0f;
        }
        target.x = side;
        PSVECSubtract(&target, player->pos, &delta);
        delta.y = 0.0f;
        ratio = 1.0f - PSVECMag(&delta) / 700.0f;
        player->pos->x = target.x * ratio;
        player->pos->y = (f32)(300.0 * HuSin(180.0f * ratio));
        if (player->pos->z <= 0.0f) {
            Hu3DMotionShiftSet(player->modelId, player->motion[4], 0.0f, 4.0f, 0);
            impulse.x = 0.0f;
            impulse.y = -0.005f;
            impulse.z = 0.0f;
            fn_1_9B58(eff, &impulse, player->pos);
            player->stateTimer = 0;
            player->state++;
        }
        break;
    case 2:
        if (Hu3DMotionShiftIDGet(player->modelId) < 0) {
            Hu3DMotionShiftSet(player->modelId, player->motion[1], 0.0f, 4.0f, 0x40000001);
            player->stateTimer = 0;
            player->state++;
        }
        break;
    case 3:
        ret = 1;
        break;
    }
    if (player->state >= 2) {
        row.x = eff->mtx[0][0];
        row.y = eff->mtx[0][1];
        row.z = eff->mtx[0][2];
        PSVECScale(&row, &scaled, 150.0f);
        if ((player->slot & 1) != 0) {
            sx = -scaled.y;
        }
        else {
            sx = scaled.y;
        }
        player->pos->y = sx;
    }
    return ret;
}
