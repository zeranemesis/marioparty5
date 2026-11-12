#include "REL/m502dll.h"

s32 lbl_1_data_B60[8] = { 1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80 };

void fn_1_1990(M502CAMERA *cam, s32 camBit);

void fn_1_1830(M502CAMERA *cam, s32 playerNo, struct M502Player_s *player)
{
    Vec up = { 0.0f, 1.0f, 0.0f };
    Vec posGoal = { 0.0f, 0.0f, 2000.0f };
    Vec pos = { 0.0f, 0.0f, 500.0f };

    memset(cam, 0, sizeof(M502CAMERA));
    cam->player = player;
    cam->camBit1 = lbl_1_data_B60[playerNo];
    cam->camBit2 = lbl_1_data_B60[playerNo + 4];
    fn_1_1990(cam, cam->camBit1);
    fn_1_1990(cam, cam->camBit2);
    cam->up = up;
    cam->posGoal = posGoal;
    cam->pos = pos;
    cam->rate = 0.1f;
}

void fn_1_1990(M502CAMERA *cam, s32 camBit)
{
    Hu3DCameraCreate(camBit);
    Hu3DCameraViewportSet(camBit, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraPerspectiveSet(camBit, 20.0f, 20.0f, 30000.0f, 1.2f);
    Hu3DCameraScissorSet(camBit, 0, 0, 640, 480);
}

void fn_1_1A60(M502CAMERA *cam)
{
    memset(cam, 0, sizeof(M502CAMERA));
}

void fn_1_1A90(M502CAMERA *cam)
{
    Vec dPos;
    Vec dTgt;

    PSVECSubtract(&cam->posGoal, &cam->pos, &dPos);
    PSVECSubtract(&cam->tgtGoal, &cam->tgt, &dTgt);
    PSVECScale(&dPos, &dPos, cam->rate);
    PSVECScale(&dTgt, &dTgt, cam->rate);
    PSVECAdd(&cam->pos, &dPos, &cam->pos);
    PSVECAdd(&cam->tgt, &dTgt, &cam->tgt);
    Hu3DCameraPosSetV(cam->camBit1, &cam->pos, &cam->up, &cam->tgt);
    Hu3DCameraPosSetV(cam->camBit2, &cam->pos, &cam->up, &cam->tgt);
}

void fn_1_1B40(Vec *out, Vec *a, Vec *b, f32 t)
{
    Vec v;
    f32 s;
    f64 e;

    if (t > 1.0f) {
        t = 1.0f;
    }
    e = HuSin(90.0f * t);
    s = (f32)(HuSin(90.0f * t) * e);
    v.x = a->x + s * (b->x - a->x);
    v.y = a->y + s * (b->y - a->y);
    v.z = a->z + s * (b->z - a->z);
    *out = v;
}

void fn_1_1CB4(M502CAMERA *cam, Vec *pos, Vec *up, Vec *tgt)
{
    cam->up = *up;
    cam->posA = *pos;
    cam->pos = cam->posA;
    cam->posGoal = cam->pos;
    cam->tgtA = *tgt;
    cam->tgt = cam->tgtA;
    cam->tgtGoal = cam->tgt;
}

void fn_1_1D60(M502CAMERA *cam, Vec *pos, Vec *up, Vec *tgt, f32 *rate)
{
    cam->up = *up;
    cam->posGoal = *pos;
    cam->tgtGoal = *tgt;
    cam->rate = *rate;
}

void fn_1_1DB4(M502CAMERA *cam, Vec *pos, Vec *up, Vec *tgt)
{
    cam->up = *up;
    cam->posA = *pos;
    cam->tgtA = *tgt;
}

void fn_1_1E00(M502CAMERA *cam, Vec *pos, Vec *up, Vec *tgt)
{
    cam->up = *up;
    cam->posB = *pos;
    cam->tgtB = *tgt;
    cam->t = 0.0f;
}

void fn_1_1E5C(M502CAMERA *cam, f32 t)
{
    cam->t = t;
}

s32 fn_1_1E64(M502CAMERA *cam, Vec *up, f32 time)
{
    Vec p;
    Vec t;
    f32 s;
    f64 e;
    f32 k;

    cam->t += 1.0f / time;
    k = cam->t;
    if (k > 1.0f) {
        k = 1.0f;
    }
    e = HuSin(90.0f * k);
    s = (f32)(HuSin(90.0f * k) * e);
    p.x = cam->posA.x + s * (cam->posB.x - cam->posA.x);
    p.y = cam->posA.y + s * (cam->posB.y - cam->posA.y);
    p.z = cam->posA.z + s * (cam->posB.z - cam->posA.z);
    k = cam->t;
    if (k > 1.0f) {
        k = 1.0f;
    }
    e = HuSin(90.0f * k);
    s = (f32)(HuSin(90.0f * k) * e);
    t.x = cam->tgtA.x + s * (cam->tgtB.x - cam->tgtA.x);
    t.y = cam->tgtA.y + s * (cam->tgtB.y - cam->tgtA.y);
    t.z = cam->tgtA.z + s * (cam->tgtB.z - cam->tgtA.z);
    cam->up = *up;
    cam->pos = p;
    cam->posGoal = cam->pos;
    cam->tgt = t;
    cam->tgtGoal = cam->tgt;
    if (1.0f <= cam->t) {
        return 1;
    }
    return 0;
}

void fn_1_21B4(void)
{
}
