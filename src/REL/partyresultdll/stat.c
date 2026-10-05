#include "REL/partyresultdll.h"
#include "game/hsfex.h"

typedef struct {
    f32 x, y;
} StatLinePt;

static s16 FingerMdlId;

static void StatFingerPosSet(volatile s16 n)
{
    Vec in;
    Vec out;

    in.x = 27.0f;
    in.y = (f32)(n * 53 + 118);
    in.z = 6500.0f;
    Hu3D2Dto3D(&in, 4, &out);
    Hu3DModelPosSetV(FingerMdlId, &out);
}

static s32 StatLinePosGet(StatLinePt *p, StatLinePt *dir, StatLinePt *b, StatLinePt *c, StatLinePt *out)
{
    volatile s32 ret = 1;
    f32 t, t2, den, ad;

    t = dir->x * (b->y - p->y) - dir->y * (b->x - p->x);
    den = c->x * dir->y - c->y * dir->x;
    if (den < 0.0f) {
        ad = -den;
    } else {
        ad = den;
    }
    if (ad > 0.001f) {
        t = t / (c->x * dir->y - c->y * dir->x);
    } else {
        return 0;
    }
    if (dir->x != 0.0) {
        t2 = (b->x + t * c->x - p->x) / dir->x;
    } else {
        t2 = (b->y + t * c->y - p->y) / dir->y;
    }
    out->x = p->x + t2 * dir->x;
    out->y = p->y + t2 * dir->y;
    return 1;
}

// NONMATCHING: mwcceppc 1.3.2 expands 1.0/sqrt() into a guarded 4-iteration rsqrt plus a
// 1.0/x divide (it pulls in __float_huge/__float_nan); the original is a bare 3-iteration
// frsqrte rsqrt, and none of the 11 C spellings tried drops that guard.
static void StatLineNorm(StatLinePt *src, StatLinePt *dst)
{
    f64 e;
    f32 len, s, d;

    len = src->x * src->x + src->y * src->y;
    if (len > 0.0f) {
        e = 1.0 / sqrt(len);
        d = len * e;
    } else {
        d = len;
    }
    s = d;
    dst->x = src->x / s;
    dst->y = src->y / s;
}

static void StatLineScale(StatLinePt *src, StatLinePt *dst, f32 scale)
{
    dst->x = src->x * scale;
    dst->y = src->y * scale;
}

static void StatLineTrans(StatLinePt *a, StatLinePt *b, StatLinePt *out)
{
    out->x = a->x + b->x;
    out->y = a->y + b->y;
}
