#include "REL/m575dll.h"

static const Vec lbl_1_rodata_3D8 = {0.0f, 0.0f, 0.0f};
static const Vec lbl_1_rodata_3E4 = {0.0f, 0.0f, 0.0f};
static const Vec lbl_1_rodata_3F0 = {0.0f, 0.0f, 0.0f};
static const Vec lbl_1_rodata_3FC = {0.0f, 0.0f, 0.0f};
static const Vec lbl_1_rodata_408 = {0.0f, 0.0f, 0.0f};
static const f32 lbl_1_rodata_414[1] = {0.25f};
static const f32 lbl_1_rodata_418[1] = {12.0f};
static const f32 lbl_1_rodata_41C[1] = {0.5f};
static const f32 lbl_1_rodata_420[1] = {1.0f};
static const f32 lbl_1_rodata_424[1] = {-1.0f};
static const Vec lbl_1_rodata_428 = {0.0f, 0.0f, 0.0f};
static const Vec lbl_1_rodata_434 = {0.0f, 0.0f, 0.0f};
static const f32 lbl_1_rodata_440[1] = {16.666666f};
static const f32 lbl_1_rodata_444[1] = {0.0f};
static const f32 lbl_1_rodata_448[2] = {0.9999f, 0.0f};

static inline Vec vecConst(const Vec *p)
{
    return *p;
}

void fn_1_9320(M575TuboEff *self, u16 arg1, const Vec *arg2, f32 height, f32 radius, f32 speed)
{
    memset(self, 0, sizeof(*self));
    self->height = height;
    self->unk4 = arg1;

    switch ((s32)arg1) {
    case 0:
        self->len0 = self->height * (radius * radius) / 4.0f +
                     (self->height * (speed * speed)) / 12.0f;
        self->len1 = self->height * (radius * radius) / 2.0f;
        self->len2 = self->height * (radius * radius) / 4.0f +
                     (self->height * (speed * speed)) / 12.0f;
        break;
    default: {
        volatile f32 r[3];

        r[2] = radius;
        r[1] = radius;
        r[0] = radius;
        self->len0 = r[1] * (r[2] * (self->height * r[0])) * (r[1] * r[1] + r[0] * r[0]) / lbl_1_rodata_418[0];
        self->len1 = r[1] * (r[2] * (self->height * r[0])) * (r[0] * r[0] + r[2] * r[2]) / lbl_1_rodata_418[0];
        self->len2 = r[1] * (r[2] * (self->height * r[0])) * (r[1] * r[1] + r[2] * r[2]) / lbl_1_rodata_418[0];
        break;
    }
    }

    self->unkE4 = radius;
    self->unkE8 = speed;

    PSMTXIdentity((f32 (*)[4])&self->scaleMtx);
    self->scaleMtx[0][0] = lbl_1_rodata_420[0] / self->len0;
    self->scaleMtx[1][1] = lbl_1_rodata_420[0] / self->len1;
    self->scaleMtx[2][2] = lbl_1_rodata_420[0] / self->len2;

    PSMTXIdentity((f32 (*)[4])&self->mtx);
    self->mtx[0][0] = lbl_1_rodata_424[0];
    self->mtx[2][2] = lbl_1_rodata_424[0];

    self->pos = vecConst(&lbl_1_rodata_3D8);
    self->vel = vecConst(&lbl_1_rodata_3E4);
    self->angPos = vecConst(&lbl_1_rodata_3F0);
    self->angAcc = vecConst(&lbl_1_rodata_3FC);
    self->angVel = vecConst(&lbl_1_rodata_408);
}

void fn_1_96F8(M575TuboEff *self)
{
    memset(self, 0, sizeof(*self));
}

void fn_1_9728(M575TuboEff *self)
{
    Vec v2;
    Vec v1;
    Mtx m4;
    Mtx m3;
    Mtx m2;
    Mtx m1;

    PSMTXTranspose((const f32 (*)[4])&self->mtx, (f32 (*)[4])&m1);
    PSMTXConcat((const f32 (*)[4])&self->mtx, (const f32 (*)[4])&self->scaleMtx, (f32 (*)[4])&m4);
    PSMTXConcat((const f32 (*)[4])&m4, (const f32 (*)[4])&m1, (f32 (*)[4])&m4);
    PSMTXMultVec((const f32 (*)[4])&m4, &self->vel, &v1);
    PSVECScale(&self->pos, &v2, lbl_1_rodata_420[0] / self->height);

    self->angVel.x = self->angVel.x + lbl_1_rodata_440[0] * v2.x;
    self->angVel.y = self->angVel.y + lbl_1_rodata_440[0] * v2.y;
    self->angVel.z = self->angVel.z + lbl_1_rodata_440[0] * v2.z;

    self->pos.x = self->pos.x + lbl_1_rodata_440[0] * self->angPos.x;
    self->pos.y = self->pos.y + lbl_1_rodata_440[0] * self->angPos.y;
    self->pos.z = self->pos.z + lbl_1_rodata_440[0] * self->angPos.z;

    self->vel.x = self->vel.x + lbl_1_rodata_440[0] * self->angAcc.x;
    self->vel.y = self->vel.y + lbl_1_rodata_440[0] * self->angAcc.y;
    self->vel.z = self->vel.z + lbl_1_rodata_440[0] * self->angAcc.z;

    PSMTXIdentity((f32 (*)[4])&m3);
    m3[0][0] = lbl_1_rodata_444[0];
    m3[0][1] = -v1.z;
    m3[0][2] = v1.y;
    m3[1][0] = v1.z;
    m3[1][1] = lbl_1_rodata_444[0];
    m3[1][2] = -v1.x;
    m3[2][0] = -v1.y;
    m3[2][1] = v1.x;
    m3[2][2] = lbl_1_rodata_444[0];

    PSMTXConcat((const f32 (*)[4])&m3, (const f32 (*)[4])&self->mtx, (f32 (*)[4])&m2);

    self->mtx[0][0] = self->mtx[0][0] + lbl_1_rodata_440[0] * m2[0][0];
    self->mtx[0][1] = self->mtx[0][1] + lbl_1_rodata_440[0] * m2[0][1];
    self->mtx[0][2] = self->mtx[0][2] + lbl_1_rodata_440[0] * m2[0][2];
    self->mtx[1][0] = self->mtx[1][0] + lbl_1_rodata_440[0] * m2[1][0];
    self->mtx[1][1] = self->mtx[1][1] + lbl_1_rodata_440[0] * m2[1][1];
    self->mtx[1][2] = self->mtx[1][2] + lbl_1_rodata_440[0] * m2[1][2];
    self->mtx[2][0] = self->mtx[2][0] + lbl_1_rodata_440[0] * m2[2][0];
    self->mtx[2][1] = self->mtx[2][1] + lbl_1_rodata_440[0] * m2[2][1];
    self->mtx[2][2] = self->mtx[2][2] + lbl_1_rodata_440[0] * m2[2][2];

    PSVECNormalize((const Vec *)&self->mtx[0][0], (Vec *)&self->mtx[0][0]);
    PSVECNormalize((const Vec *)&self->mtx[1][0], (Vec *)&self->mtx[1][0]);
    PSVECNormalize((const Vec *)&self->mtx[2][0], (Vec *)&self->mtx[2][0]);
    PSMTXInverse((const f32 (*)[4])&self->mtx, (f32 (*)[4])&self->mtx2);

    self->angPos = vecConst(&lbl_1_rodata_428);
    self->angAcc = vecConst(&lbl_1_rodata_434);

    PSVECScale(&self->pos, &self->pos, lbl_1_rodata_448[0]);
    PSVECScale(&self->vel, &self->vel, lbl_1_rodata_448[0]);
}

void fn_1_9B58(M575TuboEff *self, const Vec *arg1, const Vec *arg2)
{
    Vec t2;
    Vec t1;

    PSVECSubtract(&self->angVel, arg2, &t1);
    PSVECAdd(&self->angPos, &t1, &self->angPos);
    PSVECCrossProduct(arg1, &t1, &t2);
    PSVECAdd(&self->angAcc, &t2, &self->angAcc);
}
