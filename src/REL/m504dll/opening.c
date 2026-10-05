#define _MATH_H

#include "REL/m504dll.h"

typedef struct OpeningVec_s {
    f32 x;
    f32 y;
    f32 z;
} OPENINGVEC;

typedef struct FPair_s {
    f32 a;
    f32 b;
} FPAIR;

typedef struct OpeningUnit_s {
    u8 pad00[0x24];
    s16 modelId[20];
    s32 live[20];
    HuVecF vec[20];
    u8 pad17C[0x18C - 0x17C];
} OPENINGUNIT;

static OPENINGUNIT lbl_1_bss_7870;

typedef struct OpeningAim_s {
    f32 v00;
    f32 v04;
    f32 v08;
    f32 v0C;
    f32 v10;
    f32 v14;
    f32 v18;
    f32 v1C;
    f32 v20;
    f32 v24;
    f32 v28;
} OPENINGAIM;

extern s32 rand8(void);
extern double atan2();
extern double fmod();
extern double sqrt();

void fn_1_10CEC(f32 x, f32 y)
{
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, x - 1.0f, 20.0f + (y - 1.0f), 0.0f, 1.0f);
    Hu3DCameraScissorSet(1, 0, 0, (u32)(x - 1.0f), (u32)(y - 1.0f));
    Hu3DCameraViewportSet(2, 2.0f + x, 0.0f, 640.0f - x, 20.0f + (y - 1.0f), 0.0f, 1.0f);
    Hu3DCameraScissorSet(2, (u32)(2.0f + x), 0, (u32)(640.0f - x), (u32)(y - 1.0f));
    Hu3DCameraViewportSet(4, 0.0f, -20.0f + (2.0f + y), x - 1.0f, 20.0f + (480.0f - y), 0.0f, 1.0f);
    Hu3DCameraScissorSet(4, 0, (u32)(2.0f + y), (u32)(x - 1.0f), (u32)(480.0f - y));
    Hu3DCameraViewportSet(8, 2.0f + x, -20.0f + (2.0f + y), 640.0f - x, 20.0f + (480.0f - y), 0.0f, 1.0f);
    Hu3DCameraScissorSet(8, (u32)(2.0f + x), (u32)(2.0f + y), (u32)(640.0f - x), (u32)(480.0f - y));
}

void fn_1_11088(void)
{
    s32 i;

    for (i = 0; i < 20; i++) {
        if (lbl_1_bss_7870.live[i] == 0) {
            continue;
        }
        PSVECAdd(&Hu3DData[lbl_1_bss_7870.modelId[i]].pos, &lbl_1_bss_7870.vec[i],
                 &Hu3DData[lbl_1_bss_7870.modelId[i]].pos);
        if (Hu3DMotionEndCheck(lbl_1_bss_7870.modelId[i]) == 0) {
            continue;
        }
        lbl_1_bss_7870.live[i] = 0;
        Hu3DModelAttrSet(lbl_1_bss_7870.modelId[i], 1);
    }
}

void fn_1_11198(f32 *pos, HuVecF *vel)
{
    s32 i;
    s32 rnd;

    for (i = 0; i < 10; i++) {
        if (lbl_1_bss_7870.live[i] != 0) {
            continue;
        }
        lbl_1_bss_7870.live[i] = 1;
        lbl_1_bss_7870.vec[i] = *vel;
        rnd = rand8() % 30;
        Hu3DModelAttrReset(lbl_1_bss_7870.modelId[i], 1);
        /* NONMATCHING: CodeWarrior materialises the 0x4002 attr mask as lis+addi, MWCC folds
           it into one li, which also shifts the 3 branch offsets below. 8 spellings probed. */
        Hu3DModelAttrReset(lbl_1_bss_7870.modelId[i], 0x4000 + HU3D_ATTR_ZWRITE_OFF);
        Hu3DModelPosSet(lbl_1_bss_7870.modelId[i], pos[0] + 10.0f + (f32)rnd, 0.0f, pos[2]);
        Hu3DMotionSpeedSet(lbl_1_bss_7870.modelId[i], 1.0f);
        Hu3DMotionTimeSet(lbl_1_bss_7870.modelId[i], 0.0f);
        break;
    }
}

void fn_1_11360(f32 *pos, HuVecF *vel)
{
    s32 i;
    s32 rnd;

    for (i = 0; i < 10; i++) {
        if (lbl_1_bss_7870.live[i + 10] != 0) {
            continue;
        }
        lbl_1_bss_7870.live[i + 10] = 1;
        lbl_1_bss_7870.vec[i + 10] = *vel;
        rnd = rand8() % 30;
        Hu3DModelAttrReset(lbl_1_bss_7870.modelId[i + 10], 1);
        /* NONMATCHING: same 0x4002 attr-mask lis+addi vs li difference as fn_1_11198. */
        Hu3DModelAttrReset(lbl_1_bss_7870.modelId[i + 10], 0x4000 + HU3D_ATTR_ZWRITE_OFF);
        Hu3DModelPosSet(lbl_1_bss_7870.modelId[i + 10], pos[0] - 10.0f - (f32)rnd, 0.0f, pos[2]);
        Hu3DMotionTimeSet(lbl_1_bss_7870.modelId[i + 10], 0.0f);
        Hu3DMotionSpeedSet(lbl_1_bss_7870.modelId[i + 10], 1.0f);
        break;
    }
}

void fn_1_11548(OPENINGAIM *a, f32 *b)
{
    f64 y;
    f64 t;
    f32 z;
    f32 g;
    f32 s;
    FPAIR tmp;

    b[0] = (f32)(180.0 * (atan2(a->v18, a->v28) / 3.141592653589793));
    if (b[0] < 0.0f) {
        b[0] += 360.0f;
    }
    b[2] = (f32)(180.0 * (atan2(a->v04, a->v00) / 3.141592653589793));
    if (b[2] < 0.0f) {
        b[2] += 360.0f;
    }
    z = -a->v08;
    /* NONMATCHING (2 rows here, 13 in the sqrt block below): CodeWarrior tests "1.0 - z*z" with
       fcmpo + cror eq,gt,eq + bne; MWCC 1.3.2 lowers the same float truthiness test to
       fcmpu + beq. Semantics are identical, the encodings are not. */
    if (1.0 - z * z) {
        t = 1.0 - z * z;
    } else {
        t = -(1.0 - z * z);
    }
    if ((f32)t > 0.0f) {
        /* NONMATCHING: CodeWarrior expands sqrt() inline as frsqrte + two Newton steps of
           y = 0.5*y*(3 - x*y*y); MWCC 1.3.2 at -O0,p never emits frsqrte (probed: sqrt,
           1.0/sqrt(x), sqrtf, float and double spellings all become bl sqrt), so the seed
           is a call here and a frsqrte there. The two Newton steps and the final multiply
           match byte for byte. */
        y = 1.0 / sqrt((f64)(f32)t);
        y = (0.5 * y) * (3.0 - (f64)(f32)t * (y * y));
        y = (0.5 * y) * (3.0 - (f64)(f32)t * (y * y));
        tmp.a = (f32)((f64)(f32)t * y); /* struct member forces the stfs/lfs round trip */
        s = tmp.a;
    } else {
        s = (f32)t;
    }
    g = s;
    if (b[0] > 90.0f && b[0] < 270.0f && b[2] > 90.0f && b[2] < 270.0f) {
        b[0] = (f32)fmod(180.0f + b[0], 360.0);
        b[2] = (f32)fmod(180.0f + b[2], 360.0);
        g = -g;
    }
    b[1] = (f32)(180.0 * (atan2(z, g) / 3.141592653589793));
    if (b[1] < 0.0f) {
        b[1] += 360.0f;
    }
}
