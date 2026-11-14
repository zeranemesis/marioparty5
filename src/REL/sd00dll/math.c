#include "REL/sd00dll_sys.h"
#include "game/hsfex.h"
#include <math.h>

static void MathVecInterpolate2(Vec *dst, Vec *from, Vec *to, Vec *up, f32 t, f32 s);

void MathMtxRotGet(Mtx mtx, Vec *rot) {
    Hu3DMtxRotGet(mtx, rot);
    if (rot->y == -90.0f) {
        f32 tmp = -rot->z;
        rot->z = -rot->x;
        rot->x = tmp;
    }
}

void MathMtxRotSet(Vec *rot, Vec *out, Mtx *mtx) {
    Mtx local;
    Mtx tmp;
    Mtx *target;

    if (mtx != NULL) {
        target = mtx;
    } else {
        target = &local;
    }
    PSMTXIdentity(*target);

    PSMTXRotRad(tmp, 'y', 0.017453292f * rot->y);
    PSMTXConcat(*target, tmp, *target);
    PSMTXRotRad(tmp, 'x', 0.017453292f * rot->x);
    PSMTXConcat(*target, tmp, *target);
    PSMTXRotRad(tmp, 'z', 0.017453292f * rot->z);
    PSMTXConcat(*target, tmp, *target);

    Hu3DMtxRotGet(*target, out);
    if (out->y == -90.0f) {
        f32 t = -out->z;
        out->z = -out->x;
        out->x = t;
    }
}

void MathPlaneRotMtxGet(Vec *normal, Vec *up, f32 angle, Mtx out) {
    Vec n;
    Vec u;
    Vec a;
    Vec b;
    Mtx rot;
    Vec axisY = {0.0f, 1.0f, 0.0f};
    Vec axisZ = {0.0f, 0.0f, 1.0f};
    f32 k;

    PSVECNormalize(normal, &n);
    if (up != NULL) {
        PSVECNormalize(up, &u);
    } else {
        if (n.y > 0.0f) {
            k = n.y;
        } else {
            k = -n.y;
        }
        if (k < 0.98f) {
            u = axisY;
        } else {
            u = axisZ;
        }
    }

    PSVECCrossProduct(&u, &n, &a);
    PSVECNormalize(&a, &a);
    PSVECCrossProduct(&n, &a, &b);
    PSVECNormalize(&b, &b);

    out[0][0] = a.x;
    out[1][0] = a.y;
    out[2][0] = a.z;
    out[0][1] = b.x;
    out[1][1] = b.y;
    out[2][1] = b.z;
    out[0][2] = n.x;
    out[1][2] = n.y;
    out[2][2] = n.z;

    if (up == NULL) {
        PSMTXRotRad(rot, 'z', 0.017453292f * angle);
        PSMTXConcat(out, rot, out);
    }
}

void MathVecInterpolate(Vec *dst, Vec *a, Vec *b, f32 t, f32 s) {
    Vec up = {0.0f, 1.0f, 0.0f};
    Vec from = *a;
    Vec to = *b;

    MathVecInterpolate2(dst, &from, &to, &up, t, s);
}

static void MathVecInterpolate2(Vec *dst, Vec *from, Vec *to, Vec *up, f32 t, f32 s) {
    Vec tmp;
    f32 len;
    f32 k;
    f32 r;
    f32 f28;

    PSVECSubtract(to, from, &tmp);
    len = PSVECMag(&tmp);
    if (len > 0.0f) {
        PSVECNormalize(&tmp, &tmp);
        PSVECScale(&tmp, &tmp, len * s);
        PSVECAdd(&tmp, from, &tmp);
    } else {
        tmp = *from;
    }

    k = 0.5f - s;
    if (k < 0.0f) {
        k = -k;
    }
    if (t > 0.0f) {
        r = sqrtf(t);
    } else {
        r = t;
    }
    f28 = 2.0f * (k * r);
    f28 = t - f28 * f28;

    PSVECNormalize(up, up);
    PSVECScale(up, up, f28);
    PSVECAdd(&tmp, up, &tmp);
    *dst = tmp;
}

void LightColorSet(s16 modelIdx, s32 lightIdx, u8 r, u8 g, u8 b) {
    HSFMATERIAL *mat;
    s16 count;
    s32 i;

    if (modelIdx == -1) {
        return;
    }
    mat = Hu3DData[modelIdx].hsf->material;
    count = Hu3DData[modelIdx].hsf->materialNum;

    if (lightIdx == 0xff) {
        for (i = 0; i < count; i++) {
            mat[i].color[0] = r;
            mat[i].color[1] = g;
            mat[i].color[2] = b;
        }
    } else if (count >= lightIdx) {
        mat[lightIdx].color[0] = r;
        mat[lightIdx].color[1] = g;
        mat[lightIdx].color[2] = b;
    }
}

f32 MathLerp(s32 mode, f32 t) {
    f32 f31;
    f32 res = 0.0f;

    if (t > 1.0f) {
        f31 = 1.0f;
    } else if (t < 0.0f) {
        f31 = 0.0f;
    } else {
        f31 = t;
    }

    switch (mode) {
    case 0:
        res = 1.0 - cos(3.141592653589793 * (90.0f * f31) / 180.0);
        break;
    case 1:
        res = sin(3.141592653589793 * (90.0f * f31) / 180.0);
        break;
    case 2:
        res = (1.0 - cos(3.141592653589793 * (180.0f * f31) / 180.0)) * 0.5;
        break;
    case 3:
        if (f31 < 0.5) {
            res = sin(3.141592653589793 * (90.0f * f31) / 180.0) * 0.5f;
        } else {
            res = (2.0 - cos(3.141592653589793 * (90.0f * f31) / 180.0)) * 0.5f;
        }
        break;
    case 4:
        res = f31;
        break;
    case 5:
        res = sin(3.141592653589793 * (180.0f * f31) / 180.0);
        break;
    }
    return res;
}

void MathRotVecMulti(Vec *rot, s32 count, Vec *src, Vec *dst) {
    Mtx target;
    Mtx tmp;
    s32 i;

    PSMTXIdentity(target);
    PSMTXRotRad(tmp, 'y', 0.017453292f * rot->y);
    PSMTXConcat(target, tmp, target);
    PSMTXRotRad(tmp, 'x', 0.017453292f * rot->x);
    PSMTXConcat(target, tmp, target);
    PSMTXRotRad(tmp, 'z', 0.017453292f * rot->z);
    PSMTXConcat(target, tmp, target);

    for (i = 0; i < count; i++) {
        PSMTXMultVec(target, src + i, dst + i);
    }
}

void MathMultVecMulti(Mtx m, s32 count, Vec *src, Vec *dst) {
    s32 i;

    for (i = 0; i < count; i++) {
        PSMTXMultVec(m, src + i, dst + i);
    }
}
