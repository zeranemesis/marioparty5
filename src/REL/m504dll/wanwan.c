#define _MATH_H

#include "REL/m504dll.h"
#include "string.h"

typedef struct WanwanModel_s {
    u8 pad0[0xC];
    s16 modelId;
    s16 mtnIds[10];
    u8 pad22[0x28 - 0x22];
    s16 mdlA;
    s16 mtnA;
    u8 pad2C[0x30 - 0x2C];
    s16 mdlB;
    s16 mtnB;
} WANWANMODEL;

typedef struct WanwanGrp_s {
    u8 pad0[0x60];
    Vec pos;
} WANWANGRP;

typedef struct WanwanWork_s {
    u8 pad0[0x8];
    WANWANGRP grp;
} WANWANWORK;

typedef struct Wanwan_s {
    WANWANWORK *work;
    u16 kind;
    u16 state6;
    u16 id8;
    u16 idA;
    s16 modelId;
    u8 padE[0x18 - 0xE];
    s16 mtnId;
    u8 pad1A[0x38 - 0x1A];
    Vec dir;
    f32 f44;
    f32 facing;
    f32 f4C;
    u8 pad50[0x78 - 0x50];
    f32 facing2;
    s32 state7C;
} WANWAN;

Vec lbl_1_data_4C4[8] = {
    { 0.0f, 0.0f, -1.0f },
    { 1.0f, 0.0f, -1.0f },
    { 1.0f, 0.0f, 0.0f },
    { 1.0f, 0.0f, 1.0f },
    { 0.0f, 0.0f, 1.0f },
    { -1.0f, 0.0f, 1.0f },
    { -1.0f, 0.0f, 0.0f },
    { -1.0f, 0.0f, -1.0f }
};

extern s32 fn_1_4030(void *a, Vec *b, f32 c, s16 *d, s32 e, s16 *f, s16 *g);

extern double atan2();
extern double sin();
extern double cos();
extern double acos();

#define WANWAN_HEADING (180.0 * (atan2(dir.x, dir.z) / 3.141592653589793))
#define WANWAN_HEADING_V (180.0 * (atan2(v.x, v.z) / 3.141592653589793))

void fn_1_9C10(WANWANMODEL *self)
{
    s32 i;

    Hu3DMotionKill(self->mtnB);
    Hu3DMotionKill(self->mtnA);
    Hu3DModelKill(self->mdlB);
    Hu3DModelKill(self->mdlA);
    /* NONMATCHING: CodeWarrior materialises the 0x4002 attr mask as lis+addi, MWCC folds it into one li. */
    Hu3DModelAttrSet(self->modelId, 0x4000 + HU3D_ATTR_ZWRITE_OFF);

    for (i = 0; i < 10; i++) {
        Hu3DMotionKill(self->mtnIds[i]);
    }

    Hu3DModelKill(self->modelId);
    memset(self, 0, 0x80);
}

s32 fn_1_AEE0(WANWAN *self, f32 *outAng)
{
    Vec dir;
    Vec face;
    register WANWANGRP *grp;
    f64 angHi;
    f64 angLo;
    f64 dot;
    f32 deg;
    f32 theta;

    grp = (WANWANGRP *)((u8 *)self->work + 8);
    PSVECSubtract(&grp->pos, &self->dir, &dir);
    dir.y = 0.0f;
    PSVECNormalize(&dir, &dir);

    if (WANWAN_HEADING > 180.0) {
        angHi = WANWAN_HEADING - 360.0;
    } else {
        if (WANWAN_HEADING <= -180.0) {
            angLo = 360.0 + WANWAN_HEADING;
        } else {
            angLo = WANWAN_HEADING;
        }
        angHi = angLo;
    }
    *outAng = (f32)angHi;

    face.x = (f32)sin(3.141592653589793 * self->facing / 180.0);
    face.y = 0.0f;
    face.z = (f32)cos(3.141592653589793 * self->facing / 180.0);

    dot = PSVECDotProduct(&dir, &face);
    theta = (f32)acos(dot);
    deg = (f32)(180.0 * (theta / 3.141592653589793));

    if (deg < 24.0f) {
        return 1;
    }
    return 0;
}

void fn_1_B1AC(WANWAN *self)
{
    Vec v;
    f64 angHi;
    f64 angLo;
    register WANWANWORK *work;
    register WANWANGRP *grp;

    work = self->work;
    grp = (WANWANGRP *)((u8 *)work + 8);

    if (self->state7C != 0 && (self->kind == 5 || self->kind == 6)) {
        self->f44 = self->f4C = 0.0f;
        self->facing = self->facing2;
        self->dir.y = 0.0f;
        self->id8 = (u16)(rand8() % 3);
self->idA = 0;
        /* NONMATCHING: same 0x4001 attr-mask lis+addi vs li difference as fn_1_9C10 (and its 2 branch offsets). */
    Hu3DModelAttrReset(self->modelId, 0x4000 + HU3D_ATTR_DISPOFF);
        Hu3DMotionShiftSet(self->modelId, self->mtnId, 0.0f, 8.0f, 0);

        PSVECSubtract(&grp->pos, &self->dir, &v);
        v.y = 0.0f;
        PSVECNormalize(&v, &v);

        if (WANWAN_HEADING_V > 180.0) {
            angHi = WANWAN_HEADING_V - 360.0;
        } else {
            if (WANWAN_HEADING_V <= -180.0) {
                angLo = 360.0 + WANWAN_HEADING_V;
            } else {
                angLo = WANWAN_HEADING_V;
            }
            angHi = angLo;
        }

        self->facing2 = (f32)angHi;
        self->state6 = 0;
        self->kind = 7;
    }
}

s32 fn_1_B43C(WANWAN *self, Vec *from, f32 range, Vec *dst, s32 flag)
{
    Vec v;
    f32 mag;
    f32 scale;

    PSVECSubtract(from, &self->dir, &v);

    if (flag == 0) {
        v.y = 0.0f;
    }

    mag = PSVECMag(&v);

    if (mag < 100.0f + range) {
        PSVECNormalize(&v, &v);
        scale = 100.0f + range - mag;
        PSVECScale(&v, dst, scale);
        return 1;
    }
    return 0;
}

s32 fn_1_B52C(WANWAN *self, Vec *outPos)
{
    Vec pts[8];
    s32 ids[8];
    Vec v;
    Vec face;
    s16 outv[8];
    f32 radius;
    f32 ang;
    f32 bestAng;
    f32 dot;
    f32 theta;
    s32 i;
    s32 n;
    s32 ring;
    s32 bestIdx;
    register WANWANWORK *work;
    register WANWANGRP *grp;
    register u8 *p15c;

    work = self->work;
    grp = (WANWANGRP *)((u8 *)work + 8);
    p15c = (u8 *)work + 0x15C;
    radius = 250.0f;
    n = ring = 0;

    while (n == 0) {
        n = 0;

        for (i = 0; i < 8; i++) {
            PSVECNormalize(&lbl_1_data_4C4[i], &v);
            PSVECScale(&v, &v, radius);
            PSVECAdd(&v, &self->dir, &v);

            if (fn_1_4030(p15c, &v, 50.0f, &outv[2], 0, &outv[1], &outv[0]) == 0) {
                ids[n] = i;
                pts[n] = v;
                n++;
            }
        }

        radius += 100.0f;

        if (ring++ > 4) {
            break;
        }
    }

    if (n == 0) {
        return 0;
    }

    bestAng = 0.0f;
    bestIdx = 0;

    for (i = 0; i < n; i++) {
        PSVECSubtract(&pts[i], &self->dir, &v);
        PSVECNormalize(&v, &v);

        if (self->kind != 2) {
            face.x = (f32)sin(3.141592653589793 * self->facing / 180.0);
            face.y = 0.0f;
            face.z = (f32)cos(3.141592653589793 * self->facing / 180.0);
        } else {
            PSVECSubtract(&self->dir, &grp->pos, &face);
            face.y = 0.0f;
            PSVECNormalize(&face, &face);
        }

        dot = PSVECDotProduct(&v, &face);
        theta = (f32)acos(dot);
        ang = (f32)(180.0 * (theta / 3.141592653589793));

        if (ang > bestAng) {
            bestAng = ang;
            bestIdx = i;
        }
    }

    *outPos = pts[bestIdx];
    return 1;
}
