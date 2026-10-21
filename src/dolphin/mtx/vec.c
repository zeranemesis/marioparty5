#include <dolphin/types.h>

typedef f32 PSVECFloat;
/* Keep zero first in .sdata2; HalfAngle must load it without constant folding. */
const f32 vec_zero = 0.0f;

/* 12 bytes: C_VECHalfAngle and C_VECReflect keep their three temporaries at
   0x14, 0x20 and 0x2c, which is only true for a three-float struct. The psq_l /
   psq_st pairs below read and write four bytes past z, which is what the
   original does too. */
typedef struct {
    f32 x, y, z;
} PSVECVec;
typedef struct { u32 x, y; } PSVECWordPair;

#define VECTOR PSVECVec


/* The paired-single forms below cannot be produced by MWCC 1.2.5n from plain C:
   -O4,p on scalar float fields emits lfs/fadds/stfs, never psq_l/ps_add. They
   are written as inline asm, the way src/dolphin/mtx/mtxvec.c already does.
   The C bodies are given in the comment above each one. */

asm void PSVECAdd(register VECTOR *a, register VECTOR *b, register VECTOR *out) /* out = a + b */ {
    nofralloc;
    psq_l f2, 0(a), 0, 0
    psq_l f4, 0(b), 0, 0
    ps_add f6, f2, f4
    psq_st f6, 0(out), 0, 0
    psq_l f3, 8(a), 1, 0
    psq_l f5, 8(b), 1, 0
    ps_add f7, f3, f5
    psq_st f7, 8(out), 1, 0
    blr
}

asm void PSVECSubtract(register VECTOR *a, register VECTOR *b, register VECTOR *out) /* out = a - b */ {
    nofralloc;
    psq_l f2, 0(a), 0, 0
    psq_l f4, 0(b), 0, 0
    ps_sub f6, f2, f4
    psq_st f6, 0(out), 0, 0
    psq_l f3, 8(a), 1, 0
    psq_l f5, 8(b), 1, 0
    ps_sub f7, f3, f5
    psq_st f7, 8(out), 1, 0
    blr
}

asm void PSVECScale(register VECTOR *a, PSVECFloat scale, register VECTOR *out) /* out = a * scale */ {
    nofralloc;
    psq_l f0, 0(a), 0, 0
    psq_l f2, 8(a), 1, 0
    ps_muls0 f0, f0, f1
    psq_st f0, 0(out), 0, 0
    ps_muls0 f0, f2, f1
    psq_st f0, 8(out), 1, 0
    blr
}

asm void PSVECNormalize(register VECTOR *v, register VECTOR *out) /* out = v / |v| */ {
    nofralloc;
    lfs f0, 0.5
    lfs f1, 3.0
    psq_l f2, 0(v), 0, 0
    ps_mul f5, f2, f2
    psq_l f3, 8(v), 1, 0
    ps_madd f4, f3, f3, f5
    ps_sum0 f4, f4, f3, f5
    frsqrte f5, f4
    fmuls f6, f5, f5
    fmuls f0, f5, f0
    fnmsubs f6, f6, f4, f1
    fmuls f5, f6, f0
    ps_muls0 f2, f2, f5
    psq_st f2, 0(out), 0, 0
    ps_muls0 f3, f3, f5
    psq_st f3, 8(out), 1, 0
    blr
}

asm PSVECFloat PSVECSquareMag(register VECTOR *v) /* |v|^2 */ {
    nofralloc;
    psq_l f0, 0(v), 0, 0
    ps_mul f0, f0, f0
    lfs f1, 8(v)
    ps_madd f1, f1, f1, f0
    ps_sum0 f1, f1, f0, f0
    blr
}

asm PSVECFloat PSVECMag(register VECTOR *v) /* |v|, zero-guard */ {
    nofralloc;
    lfs f4, 0.5
    psq_l f0, 0(v), 0, 0
    ps_mul f0, f0, f0
    lfs f1, 8(v)
    fsubs f2, f4, f4
    ps_madd f1, f1, f1, f0
    ps_sum0 f1, f1, f0, f0
    fcmpu cr0, f1, f2
    beq lbl_801436EC
    frsqrte f0, f1
    lfs f3, 3.0
    fmuls f2, f0, f0
    fmuls f0, f0, f4
    fnmsubs f2, f2, f1, f3
    fmuls f0, f2, f0
    fmuls f1, f1, f0
lbl_801436EC:
    blr
}

asm PSVECFloat PSVECDotProduct(register VECTOR *a, register VECTOR *b) /* a . b */ {
    nofralloc;
    psq_l f2, 4(a), 0, 0
    psq_l f3, 4(b), 0, 0
    ps_mul f2, f2, f3
    psq_l f5, 0(a), 0, 0
    psq_l f4, 0(b), 0, 0
    ps_madd f3, f5, f4, f2
    ps_sum0 f1, f3, f2, f2
    blr
}

asm void PSVECCrossProduct(register VECTOR *a, register VECTOR *b, register VECTOR *out) /* out = a x b */ {
    nofralloc;
    psq_l f1, 0(b), 0, 0
    lfs f2, 8(a)
    psq_l f0, 0(a), 0, 0
    ps_merge10 f6, f1, f1
    lfs f3, 8(b)
    ps_mul f4, f1, f2
    ps_muls0 f7, f1, f0
    ps_msub f5, f0, f3, f4
    ps_msub f8, f0, f6, f7
    ps_merge11 f9, f5, f5
    ps_merge01 f10, f5, f8
    psq_st f9, 0(out), 1, 0
    ps_neg f10, f10
    psq_st f10, 4(out), 0, 0
    blr
}

void PSVECNormalize(register VECTOR *v, register VECTOR *out);
void PSVECAdd(register VECTOR *a, register VECTOR *b, register VECTOR *out);
PSVECFloat PSVECDotProduct(register VECTOR *a, register VECTOR *b);

void C_VECHalfAngle(register VECTOR *a, register VECTOR *b, register VECTOR *out)
{
    VECTOR v1;
    VECTOR v2;
    VECTOR v3;

    v1.x = -a->x;
    v1.y = -a->y;
    v1.z = -a->z;
    v2.x = -b->x;
    v2.y = -b->y;
    v2.z = -b->z;
    PSVECNormalize(&v1, &v1);
    PSVECNormalize(&v2, &v2);
    PSVECAdd(&v1, &v2, &v3);

    if (PSVECDotProduct(&v3, &v3) > *(volatile f32 *)&vec_zero) {
        PSVECNormalize(&v3, out);
    } else {
        *(PSVECWordPair *)out = *(PSVECWordPair *)&v3;
        ((u32 *)out)[2] = ((u32 *)&v3)[2];
    }
}

void C_VECReflect(register VECTOR *in, register VECTOR *normal, register VECTOR *out)
{
    VECTOR v1;
    VECTOR v2;
    PSVECFloat dot;
    PSVECFloat temp;

    v1.x = -in->x;
    v1.y = -in->y;
    v1.z = -in->z;
    PSVECNormalize(&v1, &v1);
    PSVECNormalize(normal, &v2);
    dot = PSVECDotProduct(&v1, &v2);
    temp = 2.0f * v2.x;
    temp = temp * dot;
    out->x = temp - v1.x;
    temp = 2.0f * v2.y;
    temp = temp * dot;
    out->y = temp - v1.y;
    temp = 2.0f * v2.z;
    dot = temp * dot;
    out->z = dot - v1.z;
    PSVECNormalize(out, out);
}

asm PSVECFloat PSVECSquareDistance(register VECTOR *a, register VECTOR *b) /* |a - b|^2 */ {
    nofralloc;
    psq_l f0, 4(a), 0, 0
    psq_l f1, 4(b), 0, 0
    ps_sub f2, f0, f1
    psq_l f0, 0(a), 0, 0
    psq_l f1, 0(b), 0, 0
    ps_mul f2, f2, f2
    ps_sub f0, f0, f1
    ps_madd f1, f0, f0, f2
    ps_sum0 f1, f1, f2, f2
    blr
}

asm PSVECFloat PSVECDistance(register VECTOR *a, register VECTOR *b) /* |a - b|, zero-guard */ {
    nofralloc;
    psq_l f0, 4(a), 0, 0
    psq_l f1, 4(b), 0, 0
    ps_sub f2, f0, f1
    psq_l f0, 0(a), 0, 0
    psq_l f1, 0(b), 0, 0
    ps_mul f2, f2, f2
    ps_sub f0, f0, f1
    lfs f3, 0.5
    ps_madd f1, f0, f0, f2
    fsubs f0, f3, f3
    ps_sum0 f1, f1, f2, f2
    fcmpu cr0, f0, f1
    beq lbl_80143970
    lfs f4, 3.0
    frsqrte f0, f1
    fmuls f2, f0, f0
    fmuls f0, f0, f3
    fnmsubs f2, f2, f1, f4
    fmuls f0, f2, f0
    fmuls f1, f1, f0
lbl_80143970:
    blr
}
