#define _MATH_H /* the original object has no math.h sqrt literals */
#include "types.h"
#include "game/hu3d.h"
#include "game/object.h"
#include "REL/m568Dll.h"
#include "datadir_enum.h"

extern "C" {
#include "msm.h"
}

/*
 * stage.cpp: the stage singleton (lbl_1_bss_9F0). It is two objects in one:
 * the scene node at +0x00 (vtable +0x08) and, at +0x10, a collision
 * interface with its own vtable (project / get-plane / project-to-plane),
 * whose slots reach the real methods through this-adjusting thunks.
 */

#ifdef __MWERKS__
#define STG_RODATA __declspec(section ".rodata")
#else
#define STG_RODATA
#endif

#define STG_CONTAINER(T, m, p) ((T *)((u8 *)(p) - offsetof(T, m)))
#ifndef offsetof
#define offsetof(T, m) ((size_t)&(((T *)0)->m))
#endif

// GC pointer-to-member-function layout
struct StgPtmf {
    s32 adj;
    s32 idx;
    void (*fn)(void *);
};

// Plane stored as normal + distance.
struct StgPlane {
    Vec n;
    f32 d;
};

// Triangle of the collision table.
struct StgTri {
    u16 v[3];  // vertex indices
    u16 col;   // debug colour index
};

// Header of an octree node (see octtree.cpp): triangle index range.
struct StgOctNode {
    s32 start;
    s32 count;
};

// Closest-hit search state shared by fn_1_E468 / fn_1_E584.
struct StgHit {
    f32 best;
    StgTri *tri;
    Vec *out;
    Vec *a;
    Vec *b;
};

// Archive object: slot 8 looks a named block up.
struct StgArchive {
    void **vtbl;
};

// The four model slots owned by fn_1_98D8's object.
struct StgModelSet {
    M568Model *m0;
    M568Model *m1;
    M568Model *m2;
    M568Model *m3;
};

struct StgHolder {
    StgModelSet *p;
};

struct Stage568 {
    M568Link node;        // 0x00
    void **vtbl;          // 0x08
    s32 flags;            // 0x0C
    void **vtbl2;         // 0x10
    M568Model *m14;       // 0x14
    M568Model *m18;       // 0x18
    M568Model *m1C;       // 0x1C
    StgArchive *archive;  // 0x20
    StgHolder holder;     // 0x24
    StgTri *tris;         // 0x28
    s32 triCount;         // 0x2C
    Vec *verts;           // 0x30
    s32 vertCount;        // 0x34
    StgOctNode *octTable; // 0x38
    u16 *triIdx;          // 0x3C
    s32 frame;            // 0x40
    StgPtmf hook;         // 0x44
    StgPlane plane;       // 0x50
};

extern "C" {

double sin(double x);
s32 __ptmf_test(void *ptmf);
void __ptmf_scall(void *obj);
void PSVECAdd(const Vec *a, const Vec *b, Vec *ab);
f32 PSVECDotProduct(const Vec *a, const Vec *b);
void PSVECNormalize(const Vec *src, Vec *dst);

void *fn_1_618(u32 size);
void *fn_1_1948(void *obj, void *list);
s32 fn_1_2548(void);
M568Model *fn_1_32AC(M568Model *self, s32 dataNum);
void fn_1_370C(M568Model *self, s32 arg);
StgArchive *fn_1_0(StgArchive *self, s32 dataNum);
void fn_1_A8(StgArchive *self, s32 arg);
StgHolder *fn_1_98D8(StgHolder *self);
M568OctEffect *fn_1_F7A0(M568OctEffect *self, s32 type, Vec *pos);
void fn_1_FA10(StgOctNode *nodes, s32 count);
StgOctNode *fn_1_FB30(StgOctNode *root, f32 x, f32 y, f32 z);
void fn_1_F424(StgPlane *plane, Vec *v0, Vec *v1, Vec *v2);
f32 fn_1_F4C4(StgPlane *plane, Vec *p);
void fn_1_F518(Vec *out, Vec *a, Vec *b, f32 d0, f32 d1);
u8 fn_1_F5EC(Vec *v0, Vec *v1, Vec *v2, Vec *p, StgPlane *plane);

extern M568Link lbl_1_bss_C;
extern void *lbl_1_data_23C[4];

static void fn_1_D7DC(Stage568 *self, s32 type, Vec *tbl, s32 count);
static void fn_1_D85C(Stage568 *self);
static void fn_1_DB20(Stage568 *self);
void fn_1_E720(Stage568 *self, Vec *out, Vec *p);
Vec *fn_1_E2FC(Stage568 *self);
s32 fn_1_E304(Stage568 *self, Vec *io, Vec *a, Vec *b);
static u8 fn_1_E468(Stage568 *self, Vec *io, Vec *a, Vec *b, StgOctNode *node);
static void fn_1_E584(StgHit *hit, StgTri *tri, Vec *v0, Vec *v1, Vec *v2);
void fn_1_D68C(void);
void fn_1_D9F0(Stage568 *self);
void fn_1_DDA0(Stage568 *self, s32 bit);
Stage568 *fn_1_EBC0(Stage568 *self, s16 flag);
void fn_1_ED80(void *sub, Vec *out, Vec *p);
Vec *fn_1_ED88(void *sub);
s32 fn_1_ED90(void *sub, Vec *io, Vec *a, Vec *b);
f32 fn_1_DEC0(M568PosOwner *owner);

char lbl_1_data_448[] = "ms_pointer != 0";
char lbl_1_data_458[] = "singleton.h";
f32 lbl_1_data_464[3] = { -9654.0f, 8151.0f, 2774.0f };
GXColor lbl_1_data_470 = { 255, 255, 255, 255 };
StgPtmf lbl_1_data_474 = { 0, -1, (void (*)(void *))fn_1_D68C };
char lbl_1_data_480[] = "ms_pointer == 0";
char lbl_1_data_490[] = "TriangleTable";
char lbl_1_data_49E[] = "TriangleCount";
char lbl_1_data_4AC[] = "VertexTable";
char lbl_1_data_4B8[] = "VertexCount";
char lbl_1_data_4C4[] = "OctTable";
char lbl_1_data_4CD[] = "TriIdxTable";
char lbl_1_data_4D9[] = "OctCount";
f32 lbl_1_data_4E4[3] = { -0.68376344f, 0.5698029f, 0.45584232f };
void *lbl_1_data_4F0[13] = {
    0, 0, (void *)fn_1_EBC0, (void *)fn_1_DDA0, (void *)fn_1_D9F0,
    0, 0, (void *)fn_1_ED90, (void *)fn_1_ED88, (void *)fn_1_ED80,
    (void *)fn_1_E304, (void *)fn_1_E720, (void *)fn_1_E2FC
};
void *lbl_1_data_524[5] = { 0, 0, 0, 0, 0 };

Vec lbl_1_bss_A60;
f32 lbl_1_bss_A6C;
u8 lbl_1_bss_A70;
f32 lbl_1_bss_A74;
u8 lbl_1_bss_A78;

STG_RODATA const f32 lbl_1_rodata_258[3] = { 0.0f, -0.8660254f, -0.5f };
STG_RODATA const f32 lbl_1_rodata_264[4][3] = {
    { 425.0f, 0.0f, -12500.0f }, { -425.0f, 0.0f, -14500.0f }, { 5.0f, 0.0f, -15000.0f }, { 425.0f, 0.0f, -18000.0f }
};
STG_RODATA const f32 lbl_1_rodata_294[7][3] = {
    { 5.0f, 0.0f, -7500.0f }, { -425.0f, 0.0f, -8000.0f }, { 5.0f, 0.0f, -11000.0f }, { -425.0f, 0.0f, -16500.0f },
    { -425.0f, 0.0f, -19000.0f }, { 375.0f, 0.0f, -22500.0f }, { 5.0f, 0.0f, -25000.0f }
};
STG_RODATA const GXColor lbl_1_rodata_368[15] = {
    { 0xB2, 0x00, 0x00, 0xFF }, { 0xB2, 0xB2, 0xB2, 0xFF }, { 0xB2, 0xB2, 0x00, 0xFF }, { 0xB2, 0x4C, 0x00, 0xFF },
    { 0x00, 0xB2, 0x00, 0xFF }, { 0xFF, 0xFF, 0xFF, 0xFF }, { 0x00, 0xB2, 0xB2, 0xFF }, { 0x00, 0x00, 0xB2, 0xFF },
    { 0xB2, 0x00, 0xB2, 0xFF }, { 0xB2, 0xB2, 0xFF, 0xFF }, { 0xFF, 0xFF, 0x00, 0xFF }, { 0x4C, 0x4C, 0x00, 0xFF },
    { 0x4C, 0x00, 0x4C, 0xFF }, { 0x00, 0x4C, 0x00, 0xFF }, { 0x00, 0x00, 0x4C, 0xFF }
};

#ifdef TARGET_PC
typedef void *(*StgLookupFn)(StgArchive *, const char *, s32);
#define STG_LOOKUP(a, name) ((StgLookupFn)(a)->vtbl[2])((a), (name), 0)
#else
typedef void *(*StgLookupFn)(StgArchive *, const char *);
#define STG_LOOKUP(a, name) ((StgLookupFn)(a)->vtbl[2])((a), (name))
#endif

#define STG_CHECK(line) \
    if (lbl_1_bss_9F0 != 0) { \
    } else { \
        fn_1_7EC(lbl_1_data_448, lbl_1_data_458, line); \
    }


void fn_1_CE20(Vec *dest)
{
    dest->x = lbl_1_rodata_258[0];
    dest->y = lbl_1_rodata_258[1];
    dest->z = lbl_1_rodata_258[2];
}

void fn_1_CE54(Vec *out, f32 speed)
{
    f32 scale = 0.0002777777777777778 * (f64)(9.80665f * (100.0f * speed));

    PSVECScale((Vec *)lbl_1_rodata_258, out, scale);
}

void fn_1_CED4(Vec *pos, f32 radius)
{
    f32 base;
    f32 x;
    f32 z;

    STG_CHECK(0x27);
    base = fn_1_DEC0(lbl_1_bss_9F0);
    x = pos->x;
    if (x < -800.0f + radius) {
        pos->x = -800.0f + radius;
    } else if (x > 800.0f - radius) {
        pos->x = 800.0f - radius;
    } else {
        pos->x = x;
    }
    z = pos->z;
    if (z < radius + (400.0f + base)) {
        pos->z = radius + (400.0f + base);
    } else if (z > (2200.0f + base) - radius) {
        pos->z = (2200.0f + base) - radius;
    } else {
        pos->z = z;
    }
}

void fn_1_D0A0(Vec *out, Vec *pos, f32 radius)
{
    f32 base;
    f32 r2;

    STG_CHECK(0x27);
    base = fn_1_DEC0(lbl_1_bss_9F0);
    r2 = 2.0f * radius;
    out->x = -1.0f + ((pos->x - (-800.0f + radius)) / ((1600.0f - r2) * 0.5f));
    out->y = 0.0f;
    out->z = -1.0f + ((pos->z - (radius + (400.0f + base))) / ((1800.0f - r2) * 0.5f));
}

void fn_1_D21C(s16 modelId)
{
    Hu3DModelShadowMapSet(modelId);
    Hu3DModelShadowDispOn(modelId);
    Hu3DModelLayerSet(modelId, 0);
}

Stage568 *fn_1_D260(Stage568 *self)
{
    M568Model *m;
    StgPtmf tmp;

    fn_1_187C(self);
    self->vtbl = lbl_1_data_23C;
    self->flags = 0;
    fn_1_1948(self, &lbl_1_bss_C);
    if (lbl_1_bss_9F0 == 0) {
    } else {
        fn_1_7EC(lbl_1_data_480, lbl_1_data_458, 0x35);
    }
    lbl_1_bss_9F0 = (M568PosOwner *)self;
    self->vtbl2 = lbl_1_data_524;
    self->vtbl = lbl_1_data_4F0;
    self->vtbl2 = &lbl_1_data_4F0[5];
    m = (M568Model *)fn_1_618(sizeof(M568Model));
    if (m != 0) {
        m = fn_1_32AC(m, DATANUM(DATA_m568, 0));
    }
    self->m14 = m;
    m = (M568Model *)fn_1_618(sizeof(M568Model));
    if (m != 0) {
        m = fn_1_32AC(m, DATANUM(DATA_m568, 1));
    }
    self->m18 = m;
    self->m1C = 0;
    self->archive = 0;
    self->holder.p = 0;
    Hu3DLighInit();
    PSVECNormalize((Vec *)lbl_1_data_464, &lbl_1_bss_A60);
    PSVECScale(&lbl_1_bss_A60, &lbl_1_bss_A60, -1.0f);
    Hu3DGLightInfinitytSet(Hu3DGLightCreateV((HuVecF *)lbl_1_data_464, (HuVecF *)&lbl_1_bss_A60, &lbl_1_data_470));
    Hu3DShadowCreate(30.0f, 20.0f, 25000.0f);
    fn_1_D21C(self->m14->id);
    fn_1_D85C(self);
    fn_1_D7DC(self, 0, (Vec *)lbl_1_rodata_294, 7);
    fn_1_D7DC(self, 1, (Vec *)lbl_1_rodata_264, 4);
    self->frame = 0;
    tmp = lbl_1_data_474;
    self->hook = tmp;
    return self;
}

// Frees the model set (inlined member destructors, last member first).
#define STG_FREE_SET(p) \
    if ((p) != 0) { \
        if (&(p)->m3 != 0) { \
            fn_1_370C((p)->m3, 1); \
        } \
        if (&(p)->m2 != 0) { \
            fn_1_370C((p)->m2, 1); \
        } \
        if (&(p)->m1 != 0) { \
            fn_1_370C((p)->m1, 1); \
        } \
        if ((p) != 0) { \
            fn_1_370C((p)->m0, 1); \
        } \
        fn_1_6E0(p); \
    }

StgHolder *fn_1_D4E4(StgHolder *self, s16 flag)
{
    StgModelSet *set;

    if (self != 0) {
        set = self->p;
        STG_FREE_SET(set);
        if (flag > 0) {
            fn_1_6E0(self);
        }
    }
    return self;
}

StgArchive **fn_1_D5A8(StgArchive **self, s16 flag)
{
    if (self != 0) {
        fn_1_A8(*self, 1);
        if (flag > 0) {
            fn_1_6E0(self);
        }
    }
    return self;
}

Stage568 *fn_1_D600(Stage568 *self, s16 flag)
{
    if (self != 0) {
        STG_CHECK(0x3D);
        lbl_1_bss_9F0 = 0;
        if (flag > 0) {
            fn_1_6E0(self);
        }
    }
    return self;
}

void fn_1_D68C(void)
{
}

u8 fn_1_D690(Vec *pos)
{
    Vec to;
    Vec from;
    Stage568 *s;
    u8 hit;
    f64 x1;
    f64 z1;
    f64 x2;
    f64 z2;

    STG_CHECK(0x27);
    s = (Stage568 *)lbl_1_bss_9F0;
    x1 = pos->x;
    z1 = pos->z;
    to.x = x1;
    to.y = 5000.0f;
    to.z = z1;
    x2 = pos->x;
    z2 = pos->z;
    from.x = x2;
    from.y = -5000.0f;
    from.z = z2;
    hit = ((u8(*)(void *, Vec *, Vec *, Vec *))s->vtbl2[2])((void *)&s->vtbl2, pos, &to, &from);
    if (hit == 0) {
        pos->y = 0.0f;
    }
    return hit;
}

static void fn_1_D7DC(Stage568 *self, s32 type, Vec *tbl, s32 count)
{
    M568OctEffect *e;
    s32 i;

    for (i = 0; i < count; i++) {
        e = (M568OctEffect *)fn_1_618(sizeof(M568OctEffect));
        if (e != 0) {
            fn_1_F7A0(e, type, tbl + i);
        }
    }
}

static void fn_1_D85C(Stage568 *self)
{
    StgArchive *a;
    StgArchive *arc;

    a = (StgArchive *)fn_1_618(sizeof(StgArchive));
    if (a != 0) {
        a = fn_1_0(a, DATANUM(DATA_m568, 2));
    }
    if (self->archive != a) {
        fn_1_A8(self->archive, 1);
        self->archive = a;
    }
    arc = self->archive;
    self->tris = (StgTri *)STG_LOOKUP(arc, lbl_1_data_490);
    self->triCount = *(s32 *)STG_LOOKUP(arc, lbl_1_data_49E);
    self->verts = (Vec *)STG_LOOKUP(arc, lbl_1_data_4AC);
    self->vertCount = *(s32 *)STG_LOOKUP(arc, lbl_1_data_4B8);
    self->octTable = (StgOctNode *)STG_LOOKUP(arc, lbl_1_data_4C4);
    self->triIdx = (u16 *)STG_LOOKUP(arc, lbl_1_data_4CD);
    Hu3DShadowTPLvlSet(0.8f);
    fn_1_FA10(self->octTable, *(s32 *)STG_LOOKUP(arc, lbl_1_data_4D9));
}

void fn_1_D9F0(Stage568 *self)
{
    StgHolder *h;
    StgModelSet *old;

    if (__ptmf_test(&self->hook) != 0) {
        __ptmf_scall(self);
    }
    self->frame = self->frame + 1;
    if (self->holder.p == 0 && fn_1_DEC0((M568PosOwner *)self) < -9000.0f) {
        h = (StgHolder *)fn_1_618(sizeof(StgModelSet));
        if (h != 0) {
            h = fn_1_98D8(h);
        }
        if (self->holder.p != (StgModelSet *)h) {
            old = self->holder.p;
            STG_FREE_SET(old);
            self->holder.p = (StgModelSet *)h;
        }
    }
    fn_1_DB20(self);
}

static void fn_1_DB20(Stage568 *self)
{
    Vec pos;
    Vec up;
    Vec target;
    f32 z;

    if (lbl_1_bss_A70 == 0) {
        lbl_1_bss_A6C = 12.8f;
        lbl_1_bss_A70 = 1;
    }
    if (lbl_1_bss_A78 == 0) {
        lbl_1_bss_A74 = 60.0f;
        lbl_1_bss_A78 = 1;
    }
    if (fn_1_2548() != 4) {
        up.x = 0.0f;
        up.y = 1.0f;
        up.z = 0.0f;
        z = (100.0f * lbl_1_bss_A6C) + fn_1_DEC0((M568PosOwner *)self);
        target.x = 0.0f;
        target.y = 0.0f;
        target.z = z;
        PSVECScale((Vec *)lbl_1_data_4E4, &pos, -100.0f * lbl_1_bss_A74);
        PSVECAdd(&target, &pos, &pos);
        Hu3DShadowPosSet((HuVecF *)&pos, (HuVecF *)&up, (HuVecF *)&target);
    } else {
        up.x = 0.0f;
        up.y = 1.0f;
        up.z = 0.0f;
        target.x = 0.0f;
        target.y = 0.0f;
        target.z = 0.0f;
        PSVECScale((Vec *)lbl_1_data_4E4, &pos, -100.0f * lbl_1_bss_A74);
        PSVECAdd(&target, &pos, &pos);
        Hu3DShadowPosSet((HuVecF *)&pos, (HuVecF *)&up, (HuVecF *)&target);
    }
    Hu3DShadowTPLvlSet(0.35f);
    Hu3DShadowColSet(0xF, 0x10, 0xB4);
}

void fn_1_DDA0(Stage568 *self, s32 bit)
{
    M568Model *m;

    self->flags = self->flags & ~(1 << bit);
    switch (bit) {
    case 0:
    case 1:
    case 2:
    case 3:
        break;
    case 4:
        if (self->m14 != 0) {
            fn_1_370C(self->m14, 1);
            self->m14 = 0;
        }
        if (self->m18 != 0) {
            fn_1_370C(self->m18, 1);
            self->m18 = 0;
        }
        m = (M568Model *)fn_1_618(sizeof(M568Model));
        if (m != 0) {
            m = fn_1_32AC(m, DATANUM(DATA_m568, 3));
        }
        if (self->m14 != m) {
            fn_1_370C(self->m14, 1);
            self->m14 = m;
        }
        fn_1_D21C(self->m14->id);
        break;
    }
}

f32 fn_1_DEC0(M568PosOwner *owner)
{
    f32 frames = (f32)((Stage568 *)owner)->frame;
    f32 sec = 0.016666666666666666 * frames;
    f32 t = sec;
    f32 u = t;

    return 6000.0f + (14.5f * (-100.0f * u));
}

void fn_1_DF80(Stage568 *self, StgOctNode *node)
{
    StgTri *tris;
    Vec *verts;
    u16 *idx;
    s32 count;
    s32 i;
    StgTri *t;
    Vec *v0;
    Vec *v1;
    Vec *v2;
    const GXColor *c;

    if (node != 0) {
        verts = self->verts;
        tris = self->tris;
        count = node->count;
        idx = self->triIdx + node->start;
        for (i = 0; i < count; i++) {
            t = tris + idx[i];
            v0 = verts + t->v[0];
            v1 = verts + t->v[1];
            v2 = verts + t->v[2];
            c = &lbl_1_rodata_368[t->col];
            GXBegin(GX_LINESTRIP, GX_VTXFMT0, 4);
            GXPosition3f32(v0->x, v0->y, v0->z);
            GXColor4u8(c->r, c->g, c->b, c->a);
            GXPosition3f32(v1->x, v1->y, v1->z);
            GXColor4u8(c->r, c->g, c->b, c->a);
            GXPosition3f32(v2->x, v2->y, v2->z);
            GXColor4u8(c->r, c->g, c->b, c->a);
            GXPosition3f32(v0->x, v0->y, v0->z);
            GXColor4u8(c->r, c->g, c->b, c->a);
            GXEnd();
        }
    }
}

#pragma dont_inline on
Vec *fn_1_E2FC(Stage568 *self)
{
    return &self->plane.n;
}

s32 fn_1_E304(Stage568 *self, Vec *io, Vec *a, Vec *b)
{
    StgOctNode *n0;
    StgOctNode *n1;
    StgOctNode *n2;

    n0 = fn_1_FB30(self->octTable, a->x, a->y, a->z);
    n1 = fn_1_FB30(self->octTable, (a->x + b->x) * 0.5f, (a->y + b->y) * 0.5f, (a->z + b->z) * 0.5f);
    n2 = fn_1_FB30(self->octTable, b->x, b->y, b->z);
    if (fn_1_E468(self, io, a, b, n0) != 0) {
        return 1;
    }
    if (n1 != n0 && fn_1_E468(self, io, a, b, n1) != 0) {
        return 1;
    }
    if (n2 != n1 && fn_1_E468(self, io, a, b, n2) != 0) {
        return 1;
    }
    return 0;
}

#pragma dont_inline reset
static u8 fn_1_E468(Stage568 *self, Vec *io, Vec *a, Vec *b, StgOctNode *node)
{
    StgHit hit;
    u16 *p;
    u16 *end;
    StgTri *tris;
    Vec *verts;
    StgTri *t;

    if (node == 0) {
        return 0;
    }
    if (node->count == 0) {
        return 0;
    }
    p = self->triIdx + node->start;
    end = p + node->count;
    tris = self->tris;
    verts = self->verts;
    hit.best = 0.0f;
    hit.tri = 0;
    hit.out = io;
    hit.a = a;
    hit.b = b;
    for (; p != end; p++) {
        t = tris + *p;
        fn_1_E584(&hit, t, verts + t->v[0], verts + t->v[1], verts + t->v[2]);
    }
    return hit.tri != 0;
}

static void fn_1_E584(StgHit *hit, StgTri *tri, Vec *v0, Vec *v1, Vec *v2)
{
    StgPlane plane;
    Vec p;
    f32 d0;
    f32 d1;

    fn_1_F424(&plane, v0, v1, v2);
    d0 = fn_1_F4C4(&plane, hit->a);
    d1 = fn_1_F4C4(&plane, hit->b);
    if (!(d0 < d1) && !(d0 < 0.0f) && !(d1 > 0.0f)) {
        fn_1_F518(&p, hit->a, hit->b, d0, d1);
        if (fn_1_F5EC(v0, v1, v2, &p, &plane) != 0) {
            if (!(hit->best <= d1)) {
                hit->best = d1;
                hit->tri = tri;
                hit->out->x = p.x;
                hit->out->y = p.y;
                hit->out->z = p.z;
                STG_CHECK(0x27);
                ((Stage568 *)lbl_1_bss_9F0)->plane = plane;
            }
        }
    }
}

#pragma dont_inline on
void fn_1_E720(Stage568 *self, Vec *out, Vec *p)
{
    Vec t;
    f32 dist;
    f32 dot;
    Vec *r;
    Vec *q;

    dot = PSVECDotProduct(&self->plane.n, p);
    dist = self->plane.d + dot;
    PSVECScale(&self->plane.n, &t, -dist);
    q = &t;
    r = q;
    PSVECAdd(p, r, out);
}

#pragma dont_inline reset
void fn_1_E7C8(Vec *pos, s32 handle)
{
    MSM_SEPARAM param;
    Vec n;
    f32 x;
    f32 z;

    if (handle >= 0 && omSysExitReq == 0) {
        fn_1_D0A0(&n, pos, 0.0f);
        x = n.x;
        z = n.z;
        if (x < -1.0f) {
            x = -1.0f;
        } else if (x > 1.0f) {
            x = 1.0f;
        }
        if (z < -1.0f) {
            z = -1.0f;
        } else if (z > 1.0f) {
            z = 1.0f;
        }
        param.flag = MSM_SEPARAM_VOL | MSM_SEPARAM_PAN;
        param.vol = (s8)(72.0f + (55.0f * ((1.0f + z) * 0.5f)));
        param.pan = (s8)(64.0f + (24.0f * x));
        msmSeSetParam(handle, &param);
    }
}

Stage568 *fn_1_EBC0(Stage568 *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = lbl_1_data_4F0;
        self->vtbl2 = &lbl_1_data_4F0[5];
        if (&self->holder != 0) {
            fn_1_D4E4(&self->holder, 0);
        }
        if (&self->archive != 0) {
            fn_1_A8(self->archive, 1);
        }
        if (&self->m1C != 0) {
            fn_1_370C(self->m1C, 1);
        }
        if (&self->m18 != 0) {
            fn_1_370C(self->m18, 1);
        }
        if (&self->m14 != 0) {
            fn_1_370C(self->m14, 1);
        }
        if (&self->vtbl2 != 0) {
            STG_CHECK(0x3D);
            lbl_1_bss_9F0 = 0;
        }
        if (self != 0) {
            self->vtbl = lbl_1_data_23C;
            if (self != 0) {
                fn_1_1888(self, 0);
            }
        }
        if (flag > 0) {
            fn_1_6E0(self);
        }
    }
    return self;
}

void fn_1_ED80(void *sub, Vec *out, Vec *p)
{
    fn_1_E720(STG_CONTAINER(Stage568, vtbl2, sub), out, p);
}

Vec *fn_1_ED88(void *sub)
{
    return fn_1_E2FC(STG_CONTAINER(Stage568, vtbl2, sub));
}

s32 fn_1_ED90(void *sub, Vec *io, Vec *a, Vec *b)
{
    return fn_1_E304(STG_CONTAINER(Stage568, vtbl2, sub), io, a, b);
}

}
