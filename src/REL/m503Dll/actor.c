#include "game/object.h"
#include <math.h>
#include "string.h"

typedef struct {
    f32 x;
    s16 hu;
    s16 ht;
    f32 y;
    f32 z;
} ACTOR_VEC;

typedef struct {
    u32 flags;
    u32 unk04;
    u32 unk08;
    s32 state;
    void *owner;
    void (*fn)(void *owner);
    ACTOR_VEC *pos;
    f32 rad;
    s32 mode;
} ACTOR;

typedef struct {
    ACTOR *actor[0x28];
    ACTOR *attract[8];
} ACTOR_TBL;

extern const f32 lbl_1_rodata_668;
extern const f32 lbl_1_rodata_66C;
extern const f64 lbl_1_rodata_680;
extern const f32 lbl_1_rodata_688;
extern const f64 lbl_1_rodata_690;
extern const f64 lbl_1_rodata_698;

OMOBJ *lbl_1_bss_D0;

void fn_1_D7B0(OMOBJMAN *objMan);
void fn_1_D838(OMOBJ *obj);
void fn_1_D83C(OMOBJ *obj);
void fn_1_D84C(OMOBJ *obj);
BOOL fn_1_D9C4(ACTOR *a, ACTOR *b);
BOOL fn_1_DC6C(ACTOR *a, ACTOR *b);
s32 fn_1_DF30(ACTOR *a);
s32 fn_1_DFA8(ACTOR *a);
void fn_1_E020(s32 idx);

void fn_1_D7B0(OMOBJMAN *objMan)
{
    OMOBJ *obj;
    ACTOR_TBL *tbl;

    obj = omAddObjEx(objMan, 0x46, 0, 0, OM_GRP_NONE, fn_1_D83C);
    lbl_1_bss_D0 = obj;
    tbl = HuMemDirectMallocNum(HEAP_HEAP, sizeof(ACTOR_TBL), HU_MEMNUM_OVL);
    obj->data = tbl;
    memset(tbl, 0, sizeof(ACTOR_TBL));
}

void fn_1_D838(OMOBJ *obj)
{
}

void fn_1_D83C(OMOBJ *obj)
{
    obj->objFunc = fn_1_D84C;
}

void fn_1_D84C(OMOBJ *obj)
{
    ACTOR_TBL *tbl;
    ACTOR *a;
    ACTOR *b;
    s32 i;
    s32 j;
    s32 k;

    tbl = obj->data;

    for (i = 0; i < 0x28; i++) {
        a = tbl->actor[i];
        if (a == NULL) {
            continue;
        }
        if ((a->flags & 0x80000000u) == 0) {
            continue;
        }
        if (a->state != 0) {
            continue;
        }
        for (j = i + 1; j < 0x28; j++) {
            b = tbl->actor[j];
            if (b == NULL) {
                continue;
            }
            if ((b->flags & 0x80000000u) == 0) {
                continue;
            }
            if (b->state != 0) {
                continue;
            }
            fn_1_D9C4(a, b);
        }
        for (k = 0; k < 8; k++) {
            b = tbl->attract[k];
            if (b == NULL) {
                continue;
            }
            if ((b->flags & 0x80000000u) == 0) {
                continue;
            }
            if (b->state == 0) {
                fn_1_DC6C(a, b);
            }
        }
    }

    for (i = 0; i < 0x28; i++) {
        a = tbl->actor[i];
        if (a == NULL) {
            continue;
        }
        if (a->fn != NULL) {
            a->fn(a->owner);
        }
    }
}

BOOL fn_1_D9C4(ACTOR *a, ACTOR *b)
{
    ACTOR_VEC *pa;
    ACTOR_VEC *pb;
    f32 dx;
    f32 dy;
    f32 d;
    f32 d2;
    f32 sum;
    f32 mx;
    f32 my;
    f32 sc;

    pa = a->pos;
    pb = b->pos;
    dx = pa->x - pb->x;
    dy = pa->y - pb->y;
    d2 = dx * dx + dy * dy;
    if (d2 > lbl_1_rodata_668) {
        d = sqrtf(dx * dx + dy * dy);
    }
    else {
        d = d2;
    }
    sum = a->rad + b->rad;

    if (sum > d) {
        if (d == 0.0) {
            dx = 0.0f;
            dy = -1.0f;
        }
        else {
            dx = dx / d;
            dy = dy / d;
        }
        mx = pa->x + (pb->x - pa->x) * 0.5;
        my = pa->y + (pb->y - pa->y) * 0.5;
        sc = sum * 0.5;
        pa->x = mx + dx * sc;
        pa->y = my + dy * sc;
        pb->x = mx - dx * sc;
        pb->y = my - dy * sc;
        return 1;
    }
    return 0;
}

BOOL fn_1_DC6C(ACTOR *a, ACTOR *b)
{
    f32 dx;
    f32 dy;
    f32 d;
    f32 d2;
    f32 sum;
    f32 ang;

    dx = a->pos->x - b->pos->x;
    dy = a->pos->y - b->pos->y;
    d2 = dx * dx + dy * dy;
    if (d2 > lbl_1_rodata_668) {
        d = sqrtf(dx * dx + dy * dy);
    }
    else {
        d = d2;
    }
    sum = a->rad + b->rad;

    if (b->mode != 0 ? d > sum : d < sum) {
        ang = (f32)(atan2(dy, dx) / lbl_1_rodata_698 / lbl_1_rodata_690 * lbl_1_rodata_690);
        a->pos->x = b->pos->x + sin(ang * lbl_1_rodata_698 / lbl_1_rodata_690) * sum;
        a->pos->y = b->pos->y + cos(ang * lbl_1_rodata_698 / lbl_1_rodata_690) * sum;
        return 1;
    }
    return 0;
}

s32 fn_1_DF30(ACTOR *a)
{
    OMOBJ *obj;
    ACTOR_TBL *tbl;
    void **p;
    s32 i;

    obj = lbl_1_bss_D0;
    tbl = obj->data;
    p = (void **)tbl->actor;

    for (i = 0; i < 0x28; i++, p++) {
        if (*p == NULL) {
            *p = a;
            return i;
        }
    }
    return -1;
}

s32 fn_1_DFA8(ACTOR *a)
{
    OMOBJ *obj;
    ACTOR_TBL *tbl;
    void **p;
    s32 i;

    obj = lbl_1_bss_D0;
    tbl = obj->data;
    p = (void **)tbl->attract;

    for (i = 0; i < 8; i++, p++) {
        if (*p == NULL) {
            *p = a;
            return i + 0x28;
        }
    }
    return -1;
}

void fn_1_E020(s32 idx)
{
    OMOBJ *obj;
    ACTOR_TBL *tbl;

    obj = lbl_1_bss_D0;
    tbl = obj->data;

    if (idx < 0) {
        return;
    }
    if (idx < 0x28) {
        tbl->actor[idx] = NULL;
    }
    idx -= 0x28;
    if (idx < 8) {
        tbl->attract[idx] = NULL;
    }
}

const f32 lbl_1_rodata_668 = 0.0f;
const f32 lbl_1_rodata_66C = 0.0f;
const f64 lbl_1_rodata_680 = 0.0;
const f32 lbl_1_rodata_688 = -1.0f;
const f64 lbl_1_rodata_690 = 360.0;
const f64 lbl_1_rodata_698 = 3.141592653589793;
