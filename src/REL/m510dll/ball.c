#define _MATH_H
#include "REL/m510dll.h"
#include "string.h"

extern double sin(double);
extern double cos(double);
extern double atan2(double, double);

#define M_PI_D 3.141592653589793

// .data:0x0 (absolute 0x370)
f32 lbl_1_data_370[13] = {
    80.0f, 90.0f, 90.0f, 80.0f, 90.0f, 100.0f, 90.0f, 60.0f, 70.0f, 50.0f, 50.0f, 50.0f, 50.0f,
};

// .data:0x34 (absolute 0x3A4)
s16 lbl_1_data_3A4[4] = { 30, 70, 35, 23 };

// .data:0x3C (absolute 0x3AC)
f32 lbl_1_data_3AC[4] = { 300.0f, 300.0f, 250.0f, 200.0f };

// .data:0x4C (absolute 0x3BC)
s16 lbl_1_data_3BC[4] = { 20, 30, 25, 20 };

#define M510_DATA_BALL_SHADOW 0x440003
#define M510_DATA_BALL_ITEM   0x440004
#define M510_DATA_BALL_MOTION 0x440018

static inline f32 clamp01(f32 x)
{
    if (x > 1.0f) {
        return 1.0f;
    }
    if (x < 0.0f) {
        return 0.0f;
    }
    return x;
}

// Wraps an angle in degrees into (-180, 180]; the original is a macro, so the
// argument is evaluated up to three times.
#define WRAP180(a) (((f32)(a) > 180.0f) ? ((a) - 360.0f) : (((f32)(a) <= -180.0f) ? ((a) + 360.0f) : (a)))
#define ABSF(x) (((x) >= 0.0f) ? (x) : -(x))
#define ANGLE_OF(v) (180.0 * (atan2((f64)(v).x, (f64)(v).z) / M_PI_D))

void fn_1_509C(M510_BALL_WORK *work, M510_PLAYER_WORK *owner)
{
    HU3DMODELID modelId;
    s32 i;

    memset(work, 0, sizeof(M510_BALL_WORK));
    work->owner = owner;

    modelId = Hu3DModelCreate(HuDataSelHeapReadNum(M510_DATA_BALL_SHADOW, HU_MEMNUM_OVL, HEAP_MODEL));
    work->modelId0 = modelId;
    Hu3DModelAttrSet(modelId, HU3D_ATTR_DISPOFF);

    modelId = Hu3DModelCreate(HuDataSelHeapReadNum(M510_DATA_BALL_ITEM, HU_MEMNUM_OVL, HEAP_MODEL));
    work->modelId1 = modelId;
    Hu3DModelAttrSet(modelId, HU3D_ATTR_DISPOFF);

    work->motionId = Hu3DJointMotion(modelId, HuDataSelHeapReadNum(M510_DATA_BALL_MOTION, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DMotionSet(modelId, work->motionId);
    Hu3DModelAttrReset(modelId, 0x40000001);

    for (i = 0; i < 3; i++) {
        fn_1_53E0(&work->balls[i], i, work);
    }
}

void fn_1_51B4(M510_BALL_WORK *work)
{
    s32 i;

    for (i = 0; i < 3; i++) {
        fn_1_557C(&work->balls[i]);
    }

    Hu3DMotionKill(work->motionId);
    Hu3DModelKill(work->modelId1);
    Hu3DModelKill(work->modelId0);
    memset(work, 0, sizeof(M510_BALL_WORK));
}

void fn_1_5230(M510_BALL_WORK *work)
{
    s32 i;
    M510_BALL *ball;

    work->liveCount = 0;

    for (i = 0; i < 3; i++) {
        ball = &work->balls[i];
        fn_1_55C4(ball);

        if (ball->active != 0) {
            work->liveCount += 1;
        }
    }
}

void fn_1_52B0(M510_BALL_WORK *work, M510_VECF *pos, s16 type)
{
    s32 i;
    M510_BALL *ball;

    for (i = 0; i < 3; i++) {
        ball = &work->balls[i];
        if (ball->active == 0) {
            fn_1_8060(work, ball, type, 1);
            Hu3DModelPosSet(ball->modelId0, pos->x, pos->y, pos->z);
            Hu3DModelPosSet(ball->modelId1, pos->x, pos->y, pos->z);
            Hu3DModelScaleSet(ball->modelId0, 0.0f, 0.0f, 0.0f);
            Hu3DModelScaleSet(ball->modelId1, 0.0f, 0.0f, 0.0f);
            ball->pos = *pos;
            return;
        }
    }
}

void fn_1_53E0(M510_BALL *ball, s16 index, M510_BALL_WORK *work)
{
    M510_OPEN_CAM *cam = &work->owner->obj->cam;
    HU3DMODELID modelId;

    memset(ball, 0, sizeof(M510_BALL));
    ball->work = work;
    ball->active = 0;
    ball->index = index;

    modelId = Hu3DModelLink(work->modelId0);
    ball->modelId0 = modelId;
    Hu3DModelPosSet(modelId, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(modelId, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(modelId, cam->unk04 & 0xFFFF);
    Hu3DModelLayerSet(modelId, 4);
    Hu3DModelShadowSet(modelId);
    Hu3DModelAttrSet(modelId, 1);

    modelId = Hu3DModelLink(work->modelId1);
    ball->modelId1 = modelId;
    Hu3DModelPosSet(modelId, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(modelId, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(modelId, cam->unk04 & 0xFFFF);
    Hu3DModelLayerSet(modelId, 4);
    Hu3DModelShadowSet(modelId);
    Hu3DModelAttrSet(modelId, 1);
}

void fn_1_557C(M510_BALL *ball)
{
    Hu3DModelKill(ball->modelId1);
    Hu3DModelKill(ball->modelId0);
    memset(ball, 0, sizeof(M510_BALL));
}

void fn_1_55C4(M510_BALL *ball)
{
    M510_BALL_WORK *work = ball->work;
    M510_PLAYER_WORK *player = work->owner;
    M510_OBJ *obj = player->obj;
    M510_PLAYER_MODEL_WORK *model = &player->model;
    M510_SCORE_WORK *score = &player->score;
    M510_PITCH_WORK *pitch = &player->pitch;
    M510_PATH *path = &ball->path;
    M510_VECF pos;
    M510_VECF hit;
    f32 scale;
    f32 t;
    M510_SPECTATOR *spec;

    if (ball->active == 0) {
        ball->state = 0;
        return;
    }

    switch (ball->state) {
    case 0:
        ball->target = NULL;
        ball->timer = 0;
        ball->state = 1;
        break;
    case 1:
        ball->pos = pitch->ofs;
        if (ball->timer <= 15) {
            scale = (f32)ball->timer / 15.0f;
            Hu3DModelScaleSet(ball->modelId0, 1.8f * scale, 1.8f * scale, 1.8f * scale);
            Hu3DModelScaleSet(ball->modelId1, 1.8f * scale, 1.8f * scale, 1.8f * scale);
            ball->timer++;
        }
        if (pitch->state == 4) {
            fn_1_5E40(ball);
            Hu3DModelScaleSet(ball->modelId0, 1.8f, 1.8f, 1.8f);
            Hu3DModelScaleSet(ball->modelId1, 1.8f, 1.8f, 1.8f);
            if (player->playerNo == 0) {
                if (ball->type == 0) {
                    fn_1_5B4(0x69E);
                } else if (ball->type == 3) {
                    fn_1_5B4(0x69D);
                } else {
                    fn_1_5B4(0x69C);
                }
            }
            ball->timer = 0;
            ball->state = 2;
            if (ball->type == 0) {
                Hu3DModelAttrSet(ball->modelId0, 1);
                Hu3DModelAttrReset(ball->modelId1, 1);
                Hu3DMotionTimeSet(ball->modelId1, 0.0f);
            }
        }
        break;
    case 2:
        ball->step += ball->speed;
        if (ball->step > path->sum[path->count - 1]) {
            fn_1_8060(work, ball, 0, 0);
        } else {
            fn_1_C70C(&pos, path, ball->step, 0.5f);
            ball->pos = pos;
            if (ball->type != 0) {
                Hu3DModelRotSet(ball->modelId0, 2.0f * (360.0f * ((f32)(obj->frame % 60) / 60.0f)), 0.0f, 0.0f);
            }
        }
        if (ball->active != 0) {
            if (fn_1_7038(ball, &hit) != 0) {
                fn_1_64B0(ball, &hit);
                omVibrate(model->playerNo, 20, 6, 6);
                if (obj->mode == 4) {
                    score->value++;
                }
                ball->timer = 0;
                ball->state = 3;
                switch (model->playerNo) {
                case 0:
                    fn_1_5B4(0x697);
                    break;
                case 1:
                    fn_1_5B4(0x698);
                    break;
                case 2:
                    fn_1_5B4(0x699);
                    break;
                default:
                    fn_1_5B4(0x69A);
                    break;
                }
                if (ball->type == 0) {
                    Hu3DModelAttrReset(ball->modelId0, 1);
                    Hu3DModelAttrSet(ball->modelId1, 1);
                }
            }
        }
        break;
    case 3:
        ball->step += ball->speed;
        if (ball->step > path->sum[path->count - 1]) {
            fn_1_8060(work, ball, 0, 0);
        } else {
            fn_1_C70C(&pos, path, ball->step, 0.5f);
            ball->pos = pos;
            if (ball->pos.z < -400.0f) {
                t = clamp01(ABSF(-400.0f - ball->pos.z) / 600.0f);
                scale = 0.6f + 0.4f * (1.0f - t);
                Hu3DModelScaleSet(ball->modelId0, 1.8f * scale, 1.8f * scale, 1.8f * scale);
                Hu3DModelScaleSet(ball->modelId1, 1.8f * scale, 1.8f * scale, 1.8f * scale);
            }
            spec = ball->target;
            if (spec != NULL && ball->pos.z < spec->pos.z && spec->state == 0) {
                spec->state = 1;
                Hu3DModelPosSet(player->modelId, spec->pos.x, 100.0f + spec->pos.y, 100.0f + spec->pos.z);
                Hu3DModelAttrReset(player->modelId, 1);
                Hu3DMotionTimeSet(player->modelId, 0.0f);
                player->flag = 1;
            }
        }
        break;
    }

    if (ball->active != 0) {
        pos = ball->pos;
        if (ball->state == 2) {
            t = clamp01(ball->pos.z / 1300.0f);
            pos.y += t * (lbl_1_data_370[model->charNo] - 80.0f);
        }
        Hu3DModelPosSet(ball->modelId0, pos.x, pos.y, pos.z);
        Hu3DModelPosSet(ball->modelId1, pos.x, pos.y, pos.z);
        ball->prevPos = ball->pos;
    }
}

void fn_1_5E40(M510_BALL *ball)
{
    M510_PITCH_WORK *pitch = &ball->work->owner->pitch;
    M510_PATH *path = &ball->path;
    M510_VECF pt;
    M510_VECF v;
    f32 total;
    f32 z;
    f32 t;
    f32 t2;
    f32 s;
    s32 i;

    fn_1_C5F0(path);
    for (i = 0; i < 24; i++) {
        pt.x = 0.0f;
        pt.y = 0.0f;
        pt.z = 0.0f;
        z = -250.0f + 2200.0f * ((f32)i / 23.0f);
        pt.z = z;
        if (z < 300.0f) {
            t = (z < 300.0f) ? (z - -250.0f) / 550.0f : 1.0f;
            s = 1.0f - t;
            pt.x = pitch->pos.x - (-pitch->f34 * (1.0f - s * s));
            pt.y = pitch->ofs.y + (((62.0f + lbl_1_data_3AC[ball->type]) - pitch->ofs.y) * (1.0f - s * s));
        } else if (z < 1250.0f) {
            t = (z < 1250.0f) ? (z - 300.0f) / 950.0f : 1.0f;
            pt.x = (f32)((f64)pitch->pos.x + ((f64)pitch->f34 * sin(M_PI_D * (f64)(90.0f + 90.0f * t) / 180.0)));
            pt.y = (f32)((f64)(30.0f * t) + (62.0f + (f64)lbl_1_data_3AC[ball->type] * sin(M_PI_D * (f64)(90.0f + 90.0f * t) / 180.0)));
        } else {
            t = (z < 1700.0f) ? (z - 1250.0f) / 450.0f : 1.0f;
            pt.x = pitch->pos.x - (0.5f * (pitch->f34 * t));
            t2 = (z < 1700.0f) ? (z - 1250.0f) / 450.0f : 1.0f;
            pt.y = (0.1f * 62.0f) + ((30.0f * (1.0f - t2)) + (0.9f * (62.0f * (1.0f - t2))));
        }
        v = pt;
        fn_1_C650(path, &v, 0);
    }

    total = 0.0f;
    for (i = 1; i < path->count; i++) {
        PSVECSubtract(&path->pts[i], &path->pts[i - 1], &v);
        total += PSVECMag(&v);
    }
    for (i = 1; i < path->count; i++) {
        PSVECSubtract(&path->pts[i], &path->pts[i - 1], &v);
        path->seg[i - 1] = (s16)(1000.0f * (PSVECMag(&v) / total));
    }
    for (i = 0; i < path->count; i++) {
        if (i == 0) {
            path->sum[0] = path->seg[0];
        } else {
            path->sum[i] = path->seg[i] + path->sum[i - 1];
        }
    }
    ball->speed = path->sum[path->count - 1] / lbl_1_data_3A4[ball->type];
    ball->step = 0;
}

void fn_1_64B0(M510_BALL *ball, M510_VECF *target)
{
    M510_PLAYER_WORK *player = ball->work->owner;
    M510_OBJ *obj = player->obj;
    M510_STAGE *stage = &obj->stage;
    M510_PLAYER_MODEL_WORK *model = &player->model;
    M510_PITCH_WORK *pitch = &player->pitch;
    M510_PATH *path = &ball->path;
    M510_SPECTATOR *spec;
    M510_VECF end;
    M510_VECF pt;
    M510_VECF v;
    f32 dir;
    f32 step;
    f32 drift;
    f32 total;
    f32 z;
    f32 t;
    f32 len;
    s32 i;

    dir = ((f32)(ball->hitTime - 7) / 3.0f) * -1.0f;
    if (dir > 1.0f) {
        dir = 1.0f;
    } else if (dir < -1.0f) {
        dir = -1.0f;
    }
    dir = dir * (0.8f + 0.2f * ((f32)(frand() % 10000) / 10000.0f));
    step = 20.833334f * dir;
    spec = NULL;
    if ((frand() & 1) != 0 && obj->mode == 4) {
        if (model->camPos.x < 380.0f && dir < 0.0f) {
            spec = &stage->spec[(frand() & 7) + 8];
        } else if (model->camPos.x > 380.0f && dir > 0.0f) {
            spec = &stage->spec[frand() & 7];
        }
    }

    end.x = 0.0f;
    end.y = 0.0f;
    end.z = 0.0f;
    drift = 0.0f;
    fn_1_C5F0(path);
    if (spec == NULL) {
        for (i = 0; i < 24; i++) {
            pt.x = 0.0f;
            pt.y = 0.0f;
            pt.z = 0.0f;
            t = (f32)i / 23.0f;
            pt.x = pitch->pos.x + drift;
            pt.y = target->y + 50.0f * t;
            z = target->z - 2500.0f * t;
            pt.z = z;
            drift += step;
            if (z > -200.0f) {
                t = clamp01(1.0f - ((z - -200.0f) / (target->z - -200.0f)));
                pt.y = (f32)((f64)pt.y + ((f64)lbl_1_data_3AC[ball->type] * sin(M_PI_D * (f64)(90.0f * t) / 180.0)));
            } else {
                t = clamp01(1.0f - ((z - (target->z - 2500.0f)) / (-200.0f - (target->z - 2500.0f))));
                pt.y = (f32)((f64)pt.y + ((f64)lbl_1_data_3AC[ball->type] * sin(M_PI_D * (f64)(90.0f + 60.0f * t) / 180.0)));
            }
            v = pt;
            fn_1_C650(path, &v, 0);
        }
    } else {
        end = spec->pos;
        end.y += 40.0f;
        end.z += 80.0f;
        for (i = 0; i < 24; i++) {
            pt.x = 0.0f;
            pt.y = 0.0f;
            pt.z = 0.0f;
            z = target->z - 2500.0f * ((f32)i / 23.0f);
            if (z > end.z) {
                t = clamp01(1.0f - ((z - end.z) / (target->z - end.z)));
                pt.x = pitch->pos.x + t * (end.x - pitch->pos.x);
                pt.y = (f32)((f64)(target->y + t * (end.y - target->y)) + (150.0 * sin(M_PI_D * (f64)(180.0f * t) / 180.0)));
                pt.z = z;
            } else {
                t = clamp01(1.0f - ((z - (target->z - 2500.0f)) / (end.z - (target->z - 2500.0f))));
                pt.x = pitch->pos.x + (1.0f + t) * (end.x - pitch->pos.x);
                pt.y = (f32)((f64)end.y + ((f64)(300.0f * t) + (100.0 * cos(M_PI_D * (f64)(90.0f * t) / 180.0))));
                pt.z = z;
            }
            v = pt;
            fn_1_C650(path, &v, 0);
        }
    }

    total = 0.0f;
    for (i = 1; i < path->count; i++) {
        PSVECSubtract(&path->pts[i], &path->pts[i - 1], &v);
        total += PSVECMag(&v);
    }
    for (i = 1; i < path->count; i++) {
        PSVECSubtract(&path->pts[i], &path->pts[i - 1], &v);
        len = PSVECMag(&v);
        if (spec == NULL) {
            path->seg[i - 1] = (s16)(1000.0f * (len / total));
        } else {
            path->seg[i - 1] = (s16)(1000.0f * (len / total));
        }
    }
    for (i = 0; i < path->count; i++) {
        if (i == 0) {
            path->sum[0] = path->seg[0];
        } else {
            path->sum[i] = path->seg[i] + path->sum[i - 1];
        }
    }
    ball->speed = path->sum[path->count - 1] / lbl_1_data_3BC[ball->type];
    ball->step = 0;
    ball->target = spec;
}

s32 fn_1_7038(M510_BALL *ball, M510_VECF *out)
{
    M510_PLAYER_WORK *player = ball->work->owner;
    M510_PLAYER_MODEL_WORK *model = &player->model;
    M510_VECF zero = { 0.0f, 0.0f, 0.0f };
    M510_VECF cur;
    M510_VECF prev;
    M510_VECF dir;
    M510_VECF d;
    M510_VECF ref;
    M510_VECF closest;
    Mtx mtx;
    f32 len;
    f32 dot;
    f32 angPos;
    f32 angPrev;
    f32 rotPrev;
    f32 rotCur;
    f32 midRot;
    f32 midAng;
    f32 limit;
    f32 diff;

    *out = zero;
    if (model->state != 1) {
        return 0;
    }
    if (ball->type != 0) {
        if (model->timer < 2 || model->timer > 11) {
            return 0;
        }
    } else {
        if (model->timer < 6 || model->timer > 8) {
            return 0;
        }
    }
    if (!((ball->pos.z > 1150.0f && ball->pos.z <= 1450.0f)
          || (ball->prevPos.z > 1150.0f && ball->prevPos.z <= 1450.0f)
          || (ball->prevPos.z <= 1150.0f && ball->pos.z > 1450.0f))) {
        return 0;
    }

    prev = ball->prevPos;
    cur = ball->pos;
    PSVECSubtract(&cur, &prev, &d);
    d.y = 0.0f;
    len = PSVECMag(&d);
    PSVECNormalize(&d, &dir);

    ref = model->camPos;
    ref.y = 0.0f;
    PSVECSubtract(&ref, &prev, &d);
    dot = PSVECDotProduct(&d, &dir);
    if (dot <= 0.0f) {
        closest = prev;
    } else if (dot > len) {
        closest = cur;
    } else {
        PSVECScale(&dir, &d, dot);
        PSVECAdd(&prev, &d, &closest);
    }

    PSVECSubtract(&closest, &model->camPos, &d);
    d.y = 0.0f;
    if (PSVECMag(&d) > 146.0f) {
        return 0;
    }

    PSVECSubtract(&cur, &model->camPos, &d);
    d.y = 0.0f;
    if (d.x != 0.0f || d.z != 0.0f) {
        PSVECNormalize(&d, &d);
    }
    angPos = WRAP180(ANGLE_OF(d));

    PSVECSubtract(&prev, &model->camPos, &d);
    d.y = 0.0f;
    if (d.x != 0.0f || d.z != 0.0f) {
        PSVECNormalize(&d, &d);
    }
    angPrev = WRAP180(ANGLE_OF(d));

    rotPrev = model->prevRotY;
    rotCur = model->rotY;
    midRot = WRAP180(rotPrev + 0.5f * (rotCur - rotPrev));
    midAng = WRAP180(angPos + 0.5f * (angPrev - angPos));
    limit = ABSF(WRAP180(0.5f * (angPrev - angPos)) + WRAP180(0.5f * (rotCur - rotPrev)));
    diff = ABSF(WRAP180(midRot - midAng));
    if (diff < limit) {
        PSMTXRotRad(mtx, 0x59, 0.017453292f * (rotCur - angPos));
        PSVECSubtract(&cur, &model->camPos, &d);
        PSMTXMultVec(mtx, &d, &d);
        PSVECAdd(&d, &model->camPos, out);
        Hu3DModelPosSet(model->effModelId, player->pitch.pos.x, out->y, out->z);
        Hu3DModelAttrReset(model->effModelId, 1);
        Hu3DMotionTimeSet(model->effModelId, 0.0f);
        model->effOn = 1;
        ball->hitTime = model->timer;
        return 1;
    }
    return 0;
}

void fn_1_8060(M510_BALL_WORK *work, M510_BALL *ball, s16 type, s32 on)
{
    M510_OPEN_CAM *cam = &work->owner->obj->cam;

    if (ball->active == 0 && on != 0) {
        Hu3DModelScaleSet(ball->modelId0, 1.8f, 1.8f, 1.8f);
        Hu3DModelScaleSet(ball->modelId1, 1.8f, 1.8f, 1.8f);
        Hu3DModelAttrReset(ball->modelId0, 1);
        ball->type = type;
        ball->active = 1;
        return;
    }
    if (ball->active != 0 && on == 0) {
        fn_1_C5F0(&ball->path);
        Hu3DModelAttrSet(ball->modelId0, 1);
        Hu3DModelAttrSet(ball->modelId1, 1);
        ball->active = 0;
        ball->timer = 0;
        ball->state = 0;
    }
}
