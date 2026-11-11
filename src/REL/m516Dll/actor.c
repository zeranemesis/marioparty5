#include "types.h"
#include "game/object.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "math.h"
#include "string.h"
#include "REL/m516Dll/m516types.h"

extern u32 fn_1_1838(void);

typedef struct {
    ACTHIT *dynamic[4];
    ACTHIT *fixed[0x43];
} ACTWORK;

OMOBJ *lbl_1_bss_50;

void fn_1_814C(OMOBJ *obj);
void fn_1_815C(OMOBJ *obj);
s32 fn_1_8314(ACTHIT *a, ACTHIT *b);
s32 fn_1_83FC(ACTHIT *a, ACTHIT *b, f32 offset);
s32 fn_1_86E8(ACTHIT *a, ACTHIT *b);
s32 fn_1_87DC(ACTHIT *a, ACTHIT *b, f32 offset);

void fn_1_80C0(OMOBJMAN *objman)
{
    OMOBJ *obj;
    void *work;

    obj = omAddObjEx(objman, 0x28, 0, 0, -1, fn_1_814C);
    lbl_1_bss_50 = obj;
    work = HuMemDirectMallocNum(HEAP_HEAP, sizeof(ACTWORK), 0x10000000);
    obj->data = work;
    memset(work, 0, sizeof(ACTWORK));
}

void fn_1_8148(void)
{
}

void fn_1_814C(OMOBJ *obj)
{
    obj->objFunc = fn_1_815C;
}

// Remaining difference: extra local copies of the hit pointer and register numbering in the original.
void fn_1_815C(OMOBJ *obj)
{
    ACTWORK *work = (ACTWORK *)obj->data;
    ACTHIT *a;
    ACTHIT *b;
    s32 i;
    s32 j;

    for (i = 0; i < 4; i++) {
        a = work->dynamic[i];
        if (a != NULL && a->disabled == 0) {
            a->f28.i = 0;
            a->f2C.i = 0;
        }
    }
    for (i = 0; i < 4; i++) {
        a = work->dynamic[i];
        if (a != NULL && (a->flags & 1) != 0 && a->disabled == 0) {
            for (j = i + 1; j < 4; j++) {
                b = work->dynamic[j];
                if (b != NULL && (b->flags & 1) != 0 && b->disabled == 0) {
                    fn_1_8314(a, b);
                }
            }
            for (j = 0; j < 0x43; j++) {
                b = work->fixed[j];
                if (b != NULL && (b->flags & 1) != 0 && b->disabled == 0) {
                    fn_1_86E8(a, b);
                }
            }
        }
    }
    for (i = 0; i < 4; i++) {
        a = work->dynamic[i];
        if (a != NULL && a->callback != NULL) {
            a->callback(a->arg);
        }
    }
}

s32 fn_1_8314(ACTHIT *a, ACTHIT *b)
{
    if (fn_1_83FC(a, b, 0.0f) != 0) {
        return 1;
    }
    if (b->pos->x < -1000.0 && fn_1_83FC(a, b, 2200.0f) != 0) {
        return 1;
    }
    if (b->pos->x > 1000.0 && fn_1_83FC(a, b, -2200.0f) != 0) {
        return 1;
    }
    return 0;
}

// Remaining difference: minor register/operand order.
s32 fn_1_83FC(ACTHIT *a, ACTHIT *b, f32 offset)
{
    Vec *ap;
    Vec bp;
    f32 mid;
    f32 sign;
    s32 dir;

    ap = a->pos;
    bp.x = b->pos->x + offset;
    bp.y = b->pos->y;
    bp.z = b->pos->z;
    if (!((ap->x - a->f24) > (bp.x + b->f24)) && !((ap->x + a->f24) < (bp.x - b->f24)) && !(ap->y > (bp.y + b->f20)) && !((ap->y + a->f20) < bp.y)) {
        mid = (f32)(ap->x + (0.5 * (bp.x - ap->x)));
        if (ap->x > bp.x) {
            dir = 1;
        } else {
            dir = -1;
        }
        sign = (f32)dir;
        if (fabs(ap->y - bp.y) < (f32)(0.25 * (a->f20 + b->f20))) {
            ap->x = mid + (sign * a->f24);
            b->pos->x = b->pos->x + ((mid - (sign * b->f24)) - bp.x);
        }
        if (fabs(ap->x - bp.x) < (f32)(0.5 * (a->f24 + b->f24))) {
            if (a->vel->y < 0.0f && ap->y > bp.y) {
                a->f28.i = 1;
                a->f30.other = b->arg;
                b->f2C.i = 1;
                b->f30.other = a->arg;
            }
            if (b->vel->y < 0.0f && bp.y > ap->y) {
                b->f28.i = 1;
                b->f30.other = a->arg;
                a->f2C.i = 1;
                a->f30.other = b->arg;
            }
        }
        return 1;
    }
    return 0;
}

s32 fn_1_86E8(ACTHIT *a, ACTHIT *b)
{
    if (fn_1_87DC(a, b, 0.0f) != 0) {
        return 1;
    }
    if ((u32)b->kind == 2) {
        if (b->pos->x < -1000.0 && fn_1_87DC(a, b, 2200.0f) != 0) {
            return 1;
        }
        if (b->pos->x > 1000.0 && fn_1_87DC(a, b, -2200.0f) != 0) {
            return 1;
        }
    }
    return 0;
}

s32 fn_1_87DC(ACTHIT *a, ACTHIT *b, f32 offset)
{
    Vec *ap;
    Vec bp;
    f32 half;

    ap = a->pos;
    half = (f32)(0.7 * a->f24);
    bp.x = b->pos->x + offset;
    bp.y = b->pos->y;
    bp.z = b->pos->z;
    if (!(ap->y >= (bp.y + b->f24)) && !((ap->y + a->f20) <= (bp.y + b->f2C.f)) && !((ap->x - half) >= (bp.x + b->f28.f)) && !((ap->x + half) <= (bp.x + b->f20))) {
        if (a->vel->y < 0.0f && ((bp.y + b->f24) - ap->y) < (50.0 - a->vel->y)) {
            ap->y = bp.y + b->f24;
            return 1;
        }
        if (a->vel->y > 0.0f && ((ap->y + a->f20) - (bp.y + b->f2C.f)) < (50.0 + a->vel->y)) {
            ap->y = (bp.y + b->f2C.f) - a->f20;
            return 1;
        }
        if (ap->x > bp.x) {
            ap->x = half + (bp.x + b->f28.f);
        } else {
            ap->x = (bp.x + b->f20) - half;
        }
        return 1;
    }
    return 0;
}

// Remaining difference: original keeps extra float copies (f23-f28) in registers.
s32 fn_1_89EC(Vec *pos, f32 radius)
{
    ACTWORK *work = (ACTWORK *)lbl_1_bss_50->data;
    Vec dir = { 0.0f, -1.0f, 0.0f };
    ACTHITEX hit;
    ACTHIT *b;
    f32 bestDist = 100000.0f;
    s32 bestIdx = -1;
    s32 bestId = 0;
    f32 x = pos->x;
    f32 y = pos->y;
    f32 dist;
    s32 id;
    s32 i;

    hit.base.pos = pos;
    hit.base.vel = &dir;
    hit.base.f24 = radius;
    hit.base.f20 = 1.0f;
    for (i = 0; i < 0x43; i++) {
        b = work->fixed[i];
        if (b != NULL && (b->flags & 1) != 0 && b->disabled == 0 && fn_1_86E8(&hit.base, b) != 0) {
            dist = (f32)fabs(pos->x - b->pos->x);
            id = b->f30.id;
            if (id == bestId) {
                if (!(dist > bestDist)) {
                    goto take;
                }
            } else if (id != 0) {
            take:
                bestDist = dist;
                bestId = id;
                bestIdx = i + 4;
            }
            pos->x = x;
            pos->y = y;
        }
    }
    pos->x = pos->x;
    pos->y = pos->y;
    return bestIdx;
}

// Remaining difference: original copies the hit pointer into an extra register in the loop.
void fn_1_8BF0(Vec *pos, Vec *out)
{
    ACTHIT *b;
    f32 limit;
    f32 top;
    f32 left;
    s32 i;
    OMOBJ *eff = lbl_1_bss_50;
    ACTWORK *work = (ACTWORK *)eff->data;

    limit = (f32)(10.0 + pos->y);
    out->x = pos->x;
    out->y = 2.0f;
    out->z = pos->z;
    if (fn_1_1838() != 3) {
        out->y = 2.0 + pos->y;
        return;
    }
    for (i = 0; i < 0x43; i++) {
        b = work->fixed[i];
        if (b != NULL && (b->flags & 1) != 0 && b->disabled == 0) {
            top = b->pos->y + b->f24;
            if (!(top > limit)) {
                left = b->pos->x;
                if (!(pos->x < (b->pos->x + b->f20)) && !(pos->x > (b->pos->x + b->f28.f))) {
                    top = (f32)(top + 2.0);
                    if (top > out->y) {
                        out->y = top;
                    }
                }
            }
        }
    }
}

s32 fn_1_8D88(ACTHIT *hit)
{
    ACTHIT **slot;
    s32 i;
    OMOBJ *eff = lbl_1_bss_50;
    ACTWORK *work = (ACTWORK *)eff->data;

    slot = work->dynamic;
    for (i = 0; i < 4; i++, slot++) {
        if (*slot == NULL) {
            *slot = hit;
            return i;
        }
    }
    return -1;
}

s32 fn_1_8E00(ACTHIT *hit)
{
    ACTHIT **slot;
    s32 i;
    OMOBJ *eff = lbl_1_bss_50;
    ACTWORK *work = (ACTWORK *)eff->data;

    slot = work->fixed;
    for (i = 0; i < 0x43; i++, slot++) {
        if (*slot == NULL) {
            *slot = hit;
            return i + 4;
        }
    }
    return -1;
}

ACTHIT *fn_1_8E78(s32 idx)
{
    ACTWORK *work;
    OMOBJ *eff = lbl_1_bss_50;

    work = (ACTWORK *)eff->data;
    if (idx < 0) {
        return NULL;
    }
    if (idx < 4) {
        return work->dynamic[idx];
    }
    idx -= 4;
    if (idx < 0x43) {
        return work->fixed[idx];
    }
    return NULL;
}

void fn_1_8EE8(s32 idx)
{
    ACTWORK *work;
    OMOBJ *eff = lbl_1_bss_50;

    work = (ACTWORK *)eff->data;
    if (idx >= 0) {
        if (idx < 4) {
            work->dynamic[idx] = NULL;
        }
        idx -= 4;
        if (idx < 0x43) {
            work->fixed[idx] = NULL;
        }
    }
}

/* Literals other units of the module (score, camera) reference by name. */
#ifdef __MWERKS__
#define M516_RODATA __declspec(section ".rodata")
#else
#define M516_RODATA
#endif
M516_RODATA const double lbl_1_rodata_550 = 1.0;
M516_RODATA const double lbl_1_rodata_558 = 0.7;
