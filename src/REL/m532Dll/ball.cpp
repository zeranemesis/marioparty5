// m532Dll "ball": the four ball models (singleton lbl_1_bss_C), the ball that sits on the
// rotating ring (M532BallA) and the ball that is thrown and falls (M532BallB).
#define _MATH_H
extern "C" {
#include "dolphin.h"
#include "humath.h"
#include "game/hu3d.h"
#include "game/data.h"
#include "game/memory.h"
#include "game/audio.h"
#include "game/sprite.h"
#include "game/hsfformat.h"
#include "game/hsfex.h"
}
#include "REL/m532Dll/ball.h"

#ifdef TARGET_PC /* MWCC intrinsic */
#define __fabs(x) ((x) < 0 ? -(x) : (x))
#endif

#ifdef __MWERKS__
extern "C" void __construct_array(void *ptr, void *ctor, void *dtor, u32 size, u32 n);
extern "C" void __destroy_arr(void *block, void *dtor, u32 size, u32 n);
#else
// Portable stand-ins for the MWCC runtime helpers.
static void __construct_array(void *ptr, void *ctor, void *dtor, u32 size, u32 n)
{
    u8 *p = (u8 *)ptr;

    (void)dtor;
    while (n-- != 0) {
        ((void *(*)(void *))ctor)(p);
        p += size;
    }
}

static void __destroy_arr(void *block, void *dtor, u32 size, u32 n)
{
    u8 *p = (u8 *)block + size * n;

    while (n-- != 0) {
        p -= size;
        ((void *(*)(void *, s16))dtor)(p, -1);
    }
}
#endif

extern "C" {

double acos(double x);
void GXSetVtxAttrFmtv(GXVtxFmt fmt, GXVtxAttrFmtList *list);
void HuSprTexLoad(ANIMDATA *anim, s16 bmpNo, s16 texMapId, s32 wrapS, s32 wrapT, s32 filter);

// helpers of m532.cpp / unk.cpp / player.cpp / man_map.cpp
void fn_1_1D1C(const char *expr, const char *file, s32 line);
void *fn_1_1B48(u32 size);
void fn_1_1C10(void *ptr);
void fn_1_32B0(M532BallNode *node);
void *fn_1_32BC(M532BallNode *node, s16 flag);
void fn_1_337C(M532BallNode *node, M532BallNode *head);
M532Model *fn_1_4C54(M532Model *self, s32 fileNum);
M532Model *fn_1_4D74(M532Model *self, M532Model *src);
void fn_1_4DC8(M532Model *self, u32 attr);
void fn_1_4E00(M532Model *self, u32 attr);
void fn_1_4EB4(M532Model *self, Vec *pos);
void fn_1_4EFC(M532Model *self, Vec *rot);
void *fn_1_50B4(M532Model *self, s16 flag);
void fn_1_500C(M532Model *self, s16 layer);
HSFOBJECT *fn_1_5110(M532Model *self, const char *name);
void *fn_1_EA08(M532Obj *self);
void *fn_1_EAF0(M532Obj *self, s16 flag);
void *fn_1_EB7C(M532Obj *self);
void fn_1_EEB8(s32 seId, Vec *pos);
f32 fn_1_A59C(void *map);
extern M532BallNode lbl_1_bss_24;

void *fn_1_19C(M532BallHolder *self, s16 flag);
M532BallHolder *fn_1_1F4(M532BallHolder *self);
void *fn_1_220(void *self, s16 flag);
void *fn_1_2AC(M532BallMgr *self, s16 flag);
void *fn_1_318(M532BallNode *self, s16 flag);
void *fn_1_370(M532BallMgr *self, s16 flag);
void *fn_1_86C(M532BallA *self, s16 flag);
void fn_1_998(M532BallA *self);
void fn_1_EF0(M532BallA *self, f32 (*mtx)[4]);
void *fn_1_1428(M532BallB *self, s16 flag);
void fn_1_1788(M532BallB *self);
void fn_1_1AF8(void *self);
void fn_1_1AFC(void *self);
void fn_1_1B00(M532BallMgr *self, s32 bit);
void fn_1_1B18(void *self);
void fn_1_1B1C(M532BallHolder *self, M532Model *model);
u8 fn_1_183C(M532BallB *self);
u8 fn_1_1834(M532BallB *self);
u8 fn_1_197C(M532BallB *self);
u8 fn_1_191C(M532BallB *self);
void fn_1_18A4(M532BallB *self);
u8 fn_1_1A28(M532BallB *self);
void fn_1_9F8(M532BallA *self);
void fn_1_AEC(M532BallA *self);
void fn_1_D30(M532BallA *self);
M532BallB *fn_1_11E4(M532BallB *self, Vec *from, Vec *to, f32 spin, s32 idx);

// ---------------------------------------------------------------- data

static const f32 lbl_1_rodata_5C[3] = { 0.0f, 1.0f, 0.0f };
static const f32 lbl_1_rodata_68[3] = { 0.0f, 40.0f, 0.0f };
static const s32 lbl_1_rodata_10[4] = { 0x5A0005, 0x5A0006, 0x5A0007, 0x5A0008 };
static GXVtxDescList lbl_1_rodata_80[3] = { { GX_VA_POS, GX_DIRECT }, { GX_VA_TEX0, GX_DIRECT }, { GX_VA_NULL, GX_NONE } };
static GXVtxAttrFmtList lbl_1_rodata_98[3] = { { GX_VA_POS, GX_POS_XYZ, GX_F32, 0 }, { GX_VA_TEX0, GX_TEX_ST, GX_F32, 0 }, { GX_VA_NULL, GX_POS_XYZ, GX_U8, 0 } };

char lbl_1_data_0[] = "ms_pointer == 0";
char lbl_1_data_10[] = "singleton.h";
char lbl_1_data_1C[] = "ms_pointer != 0";
char lbl_1_data_2C[] = "star_a";

M532BallPtmf lbl_1_data_34 = M532_BALL_PTMF_INIT(fn_1_183C);
M532BallPtmf lbl_1_data_40 = M532_BALL_PTMF_INIT(fn_1_1834);
M532BallPtmf lbl_1_data_4C = M532_BALL_PTMF_INIT(fn_1_197C);
M532BallPtmf lbl_1_data_58 = M532_BALL_PTMF_INIT(fn_1_191C);
M532BallPtmf lbl_1_data_64 = M532_BALL_PTMF_INIT(fn_1_1834);

M532ObjVtbl lbl_1_data_70 = { { 0, 0 }, (void *(*)(void *, s16))fn_1_1428, (void (*)(void *))fn_1_1788, (void (*)(void *, f32 (*)[4]))fn_1_1AF8, (void (*)(void *))fn_1_1AFC };
M532ObjVtbl lbl_1_data_88 = { { 0, 0 }, (void *(*)(void *, s16))fn_1_86C, (void (*)(void *))fn_1_998, (void (*)(void *, f32 (*)[4]))fn_1_EF0, (void (*)(void *))fn_1_1AFC };
M532BallMgrVtbl lbl_1_data_A0 = { { 0, 0 }, (void *(*)(void *, s16))fn_1_370, (void (*)(void *, s32))fn_1_1B00, (void (*)(void *))fn_1_1B18 };
M532BallMgrVtbl lbl_1_data_B4 = { { 0, 0 }, (void *(*)(void *, s16))fn_1_2AC, 0, (void (*)(void *))fn_1_1B18 };

s32 lbl_1_bss_0;
s32 lbl_1_bss_4;
void *lbl_1_bss_8;
M532BallMgr *lbl_1_bss_C;

// Singleton<BallMgr>::Get / Singleton<ManMap>::Get
static inline M532BallMgr *M532BallMgrGet(void)
{
    if (lbl_1_bss_C == 0) {
        fn_1_1D1C(lbl_1_data_1C, lbl_1_data_10, 0x27);
    }
    return lbl_1_bss_C;
}

static inline void *M532BallMapGet(void)
{
    if (lbl_1_bss_8 == 0) {
        fn_1_1D1C(lbl_1_data_1C, lbl_1_data_10, 0x27);
    }
    return lbl_1_bss_8;
}

// ---------------------------------------------------------------- ball manager

M532BallMgr *fn_1_0(M532BallMgr *self)
{
    s32 i;
    M532Model *model;

    fn_1_32B0(&self->node);
    self->vtbl = &lbl_1_data_B4;
    self->flags = 0;
    fn_1_337C(&self->node, &lbl_1_bss_24);
    if (lbl_1_bss_C != 0) {
        fn_1_1D1C(lbl_1_data_0, lbl_1_data_10, 0x35);
    }
    lbl_1_bss_C = self;
    self->vtbl = &lbl_1_data_A0;
    __construct_array(self->models, (void *)fn_1_1F4, (void *)fn_1_19C, sizeof(M532BallHolder), 4);
    self->motion = -1;
    self->anim = HuSprAnimRead(HuDataSelHeapReadNum(0x5A0009, 0x10000000, HEAP_MODEL));
    for (i = 0; i < 4; i++) {
        model = (M532Model *)fn_1_1B48(sizeof(M532Model));
        if (model != 0) {
            model = fn_1_4C54(model, lbl_1_rodata_10[i]);
        }
        if (self->models[i].model != model) {
            fn_1_50B4(self->models[i].model, 1);
            self->models[i].model = model;
        }
        fn_1_4DC8(self->models[i].model, 1);
    }
    return self;
}

void *fn_1_19C(M532BallHolder *self, s16 flag)
{
    if (self != 0) {
        fn_1_50B4(self->model, 1);
        if (flag > 0) {
            fn_1_1C10(self);
        }
    }
    return self;
}

M532BallHolder *fn_1_1F4(M532BallHolder *self)
{
    fn_1_1B1C(self, (M532Model *)0);
    return self;
}

void *fn_1_220(void *self, s16 flag)
{
    if (self != 0) {
        if (lbl_1_bss_C == 0) {
            fn_1_1D1C(lbl_1_data_1C, lbl_1_data_10, 0x3D);
        }
        lbl_1_bss_C = 0;
        if (flag > 0) {
            fn_1_1C10(self);
        }
    }
    return self;
}

void *fn_1_2AC(M532BallMgr *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = &lbl_1_data_B4;
        if (self != 0) {
            fn_1_32BC(&self->node, 0);
        }
        if (flag > 0) {
            fn_1_1C10(self);
        }
    }
    return self;
}

void *fn_1_318(M532BallNode *self, s16 flag)
{
    if (self != 0) {
        fn_1_32BC(self, 0);
        if (flag > 0) {
            fn_1_1C10(self);
        }
    }
    return self;
}

void *fn_1_370(M532BallMgr *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = &lbl_1_data_A0;
        __destroy_arr(self->models, (void *)fn_1_19C, sizeof(M532BallHolder), 4);
        if (self->models != 0) {
            if (lbl_1_bss_C == 0) {
                fn_1_1D1C(lbl_1_data_1C, lbl_1_data_10, 0x3D);
            }
            lbl_1_bss_C = 0;
        }
        if (self != 0) {
            self->vtbl = &lbl_1_data_B4;
            if (self != 0) {
                fn_1_32BC(&self->node, 0);
            }
        }
        if (flag > 0) {
            fn_1_1C10(self);
        }
    }
    return self;
}

// Model `idx` of the manager, put in its resting motion.
M532Model *fn_1_454(u32 idx)
{
    M532BallMgr *mgr = M532BallMgrGet();
    M532Model *model = mgr->models[idx & 3].model;

    Hu3DMotionSet(model->id, -1);
    fn_1_4DC8(model, 0x40000002);
    return model;
}

s16 fn_1_50C(void)
{
    return M532BallMgrGet()->motion;
}

void fn_1_57C(void)
{
    HuSprTexLoad(M532BallMgrGet()->anim, 0, 0, 0, 0, 1);
}

// ---------------------------------------------------------------- ball on the ring

M532BallA *fn_1_604(M532BallA *self, f32 phase, s32 idx)
{
    M532Model *src;
    Vec *cur;
    f32 x, y, z;

    fn_1_EA08(&self->base);
    self->base.vtbl = &lbl_1_data_88;
    src = fn_1_454(idx);
    fn_1_4D74(&self->model, src);
    self->phase = phase;
    self->idx = idx;
    self->thrown = 0;
    self->rotY = &fn_1_5110(&self->model, lbl_1_data_2C)->mesh.base.rot.y;
    self->spin = (f32)(idx * 45);
    fn_1_500C(&self->base.hook, 4);
    fn_1_500C(&self->model, 4);
    fn_1_4E00(&self->model, 1);
    fn_1_9F8(self);
    x = self->pos.x;
    y = self->pos.y - 200.0f;
    z = self->pos.z;
    self->cur.x = x;
    self->cur.y = y;
    self->cur.z = z;
    cur = &self->cur;
    x = cur->x;
    y = (f32)(40.0 + cur->y);
    z = cur->z;
    self->target.x = x;
    self->target.y = y;
    self->target.z = z;
    self->vel.x = 0.0f;
    self->vel.y = 0.0f;
    self->vel.z = 0.0f;
    return self;
}

void *fn_1_86C(M532BallA *self, s16 flag)
{
    if (self != 0) {
        self->base.vtbl = &lbl_1_data_88;
        fn_1_50B4(&self->model, -1);
        fn_1_EAF0(&self->base, 0);
        if (flag > 0) {
            fn_1_1C10(self);
        }
    }
    return self;
}

// Throw the ball at `to`; returns the effect handle of the new flying ball.
void *fn_1_8DC(M532BallA *self, Vec *to)
{
    M532BallB *ball;

    if (self->thrown != 0) {
        return 0;
    }
    self->thrown = 1;
    fn_1_4DC8(&self->model, 1);
    HuAudFXPlay(0x7AC);
    ball = (M532BallB *)fn_1_1B48(sizeof(M532BallB));
    if (ball != 0) {
        ball = fn_1_11E4(ball, &self->cur, to, self->spin, self->idx);
    }
    return fn_1_EB7C(&ball->base);
}

Vec *fn_1_984(M532BallA *self)
{
    return &self->cur;
}

u8 fn_1_98C(M532BallA *self)
{
    return self->thrown;
}

void fn_1_998(M532BallA *self)
{
    fn_1_AEC(self);
    fn_1_D30(self);
    *self->rotY = self->spin;
    self->spin = (f32)(self->spin + 2.0);
}

// Put the ball on its place of the ring, `phase` degrees (plus the map's angle) round.
void fn_1_9F8(M532BallA *self)
{
    Mtx m;
    f32 ang = self->phase + fn_1_A59C(M532BallMapGet());

    self->pos.x = 0.0f;
    self->pos.y = 1150.0f;
    self->pos.z = -320.0f;
    PSMTXRotRad(m, 'y', 0.017453292f * ang);
    PSMTXMultVec(m, &self->pos, &self->pos);
}

void fn_1_AEC(M532BallA *self)
{
    Mtx m;
    Vec d;
    Vec old;
    f32 ang = self->phase + fn_1_A59C(M532BallMapGet());
    f32 dist;
    s32 k;

    self->pos.x = 0.0f;
    self->pos.y = 1150.0f;
    self->pos.z = -320.0f;
    PSMTXRotRad(m, 'y', 0.017453292f * ang);
    PSMTXMultVec(m, &self->pos, &self->pos);
    old = self->cur;
    PSVECAdd(&self->vel, &self->cur, &self->cur);
    PSVECSubtract(&self->cur, &self->pos, &d);
    dist = PSVECMag(&d);
    if (dist > 200.0f) {
        PSVECScale(&d, &d, 200.0f / dist);
        PSVECAdd(&self->pos, &d, &self->cur);
    }
    fn_1_4EB4(&self->model, &self->cur);
    PSVECSubtract(&self->cur, &old, &self->vel);
    PSVECScale(&self->vel, &self->vel, 0.98f);
    if (self->thrown != 0) {
        k = 1;
    } else {
        k = 2;
    }
    self->vel.y = self->vel.y - 0.27240694f * (f32)k;
}

void fn_1_D30(M532BallA *self)
{
    Mtx m;
    Vec dir;
    Vec axis;
    f32 ang;

    PSVECSubtract(&self->pos, &self->cur, &dir);
    PSVECNormalize(&dir, &dir);
    ang = (f32)acos(PSVECDotProduct(&dir, (Vec *)lbl_1_rodata_5C));
    if (__fabs(ang) <= 0.0) {
        dir.x = 0.0f;
        dir.y = 0.0f;
        dir.z = 0.0f;
        fn_1_4EFC(&self->model, &dir);
        PSVECAdd(&self->cur, (Vec *)lbl_1_rodata_68, &self->target);
    } else {
        PSVECCrossProduct(&dir, (Vec *)lbl_1_rodata_5C, &axis);
        PSMTXRotAxisRad(m, &axis, -ang);
        PSMTXTransApply(m, m, self->cur.x, self->cur.y, self->cur.z);
        PSMTXMultVec(m, (Vec *)lbl_1_rodata_68, &self->target);
        Hu3DMtxRotGet(m, (HuVecF *)&dir);
        fn_1_4EFC(&self->model, &dir);
    }
}

void fn_1_EF0(M532BallA *self, f32 (*mtx)[4])
{
    M532BallMgr *mgr;
    Vec *a;
    Vec *b;

    GXLoadPosMtxImm(mtx, 0);
    GXSetNumChans(0);
    mgr = M532BallMgrGet();
    HuSprTexLoad(mgr->anim, 0, 0, 0, 0, 1);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetCullMode(GX_CULL_NONE);
    GXSetColorUpdate(GX_TRUE);
    GXSetAlphaUpdate(GX_FALSE);
    GXSetZCompLoc(GX_TRUE);
    GXSetZMode(GX_TRUE, GX_LESS, GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0xFF, GX_AOP_OR, GX_ALWAYS, 0xFF);
    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_COPY);
    GXClearVtxDesc();
    GXSetVtxDescv(lbl_1_rodata_80);
    GXSetVtxAttrFmtv(GX_VTXFMT0, lbl_1_rodata_98);
    GXEnableTexOffsets(GX_TEXCOORD0, GX_TRUE, GX_FALSE);
    GXSetLineWidth(0x18, GX_TO_ONE);
    a = &self->pos;
    b = &self->target;
    GXBegin(GX_LINES, GX_VTXFMT0, 2);
    GXWGFifo.f32 = a->x;
    GXWGFifo.f32 = a->y;
    GXWGFifo.f32 = a->z;
    GXWGFifo.f32 = 0.0f;
    GXWGFifo.f32 = 0.0f;
    GXWGFifo.f32 = b->x;
    GXWGFifo.f32 = b->y;
    GXWGFifo.f32 = b->z;
    GXWGFifo.f32 = 0.0f;
    GXWGFifo.f32 = 1.0f;
}

// ---------------------------------------------------------------- thrown ball

M532BallB *fn_1_11E4(M532BallB *self, Vec *from, Vec *to, f32 spin, s32 idx)
{
    M532Model *src;
    Vec d;
    Vec *dir;

    fn_1_EA08(&self->base);
    self->base.vtbl = &lbl_1_data_70;
    self->state = lbl_1_data_34;
    src = fn_1_454(idx);
    fn_1_4D74(&self->model, src);
    self->landed = 0;
    self->pos = *from;
    self->target = *to;
    self->rotY = &fn_1_5110(&self->model, lbl_1_data_2C)->mesh.base.rot.y;
    self->spin = spin;
    fn_1_500C(&self->model, 4);
    fn_1_4E00(&self->model, 1);
    self->vel.x = 0.0f;
    self->vel.y = 0.0f;
    self->vel.z = 0.0f;
    PSVECSubtract(to, from, &self->acc);
    PSVECNormalize(&self->acc, &self->acc);
    dir = &self->acc;
    PSVECScale(dir, dir, 0.27240694f);
    PSVECSubtract(to, from, &d);
    self->dist = PSVECMag(&d);
    return self;
}

void *fn_1_1428(M532BallB *self, s16 flag)
{
    if (self != 0) {
        self->base.vtbl = &lbl_1_data_70;
        fn_1_50B4(&self->model, -1);
        fn_1_EAF0(&self->base, 0);
        if (flag > 0) {
            fn_1_1C10(self);
        }
    }
    return self;
}

u8 fn_1_1498(M532BallB *self)
{
    return self->landed;
}

Vec *fn_1_14A4(M532BallB *self)
{
    return &self->pos;
}

// Position of the ball after `t` seconds of flight.
Vec *fn_1_14AC(M532BallB *self, Vec *out, f32 t)
{
    f32 s;
    f32 s2;

    out->x = self->pos.x;
    out->y = self->pos.y;
    out->z = self->pos.z;
    s = 60.0f * t;
    out->x = out->x + self->vel.x * s;
    out->z = out->z + self->vel.z * s;
    s2 = s * s;
    out->y = out->y + (self->vel.y * s - (0.27240694f * s2) * 0.5f);
    return out;
}

void fn_1_157C(M532BallB *self, Vec *pos)
{
    self->pos.x = pos->x;
    self->pos.y = pos->y;
    self->pos.z = pos->z;
    self->state = lbl_1_data_40;
}

// Throw the ball from where it is to `to`, 54 frames of flight.
void fn_1_15D8(M532BallB *self, Vec *to)
{
    Vec d;
    f32 dz, dx, t, t2;

    self->target.x = to->x;
    self->target.y = to->y;
    self->target.z = to->z;
    dz = to->z - self->pos.z;
    dx = to->x - self->pos.x;
    self->vel.x = dx;
    self->vel.y = 0.0f;
    self->vel.z = dz;
    t = 54.0f;
    PSVECScale(&self->vel, &self->vel, (f32)(1.0 / t));
    t2 = t * t;
    self->vel.y = ((to->y - self->pos.y) + (0.27240694f * t2) * 0.5f) / t;
    PSVECSubtract(to, &self->pos, &d);
    self->dist = PSVECMag(&d);
    self->landed = 0;
    self->state = lbl_1_data_4C;
}

void fn_1_1788(M532BallB *self)
{
    if (M532BallPtmfCall(self, &self->state) == 0) {
        if (self != 0) {
            self->base.vtbl->dtor(self, 1);
        }
    } else {
        fn_1_4EB4(&self->model, &self->pos);
        *self->rotY = self->spin;
        self->spin = (f32)(self->spin + 2.0);
    }
}

u8 fn_1_1834(M532BallB *self)
{
    return 1;
}

u8 fn_1_183C(M532BallB *self)
{
    PSVECAdd(&self->vel, &self->pos, &self->pos);
    PSVECAdd(&self->acc, &self->vel, &self->vel);
    if (fn_1_1A28(self) != 0) {
        fn_1_18A4(self);
    }
    return 1;
}

void fn_1_18A4(M532BallB *self)
{
    fn_1_4E00(&self->model, 0x40000003);
    HuAudFXPlay(0x7AE);
    self->state = lbl_1_data_58;
}

u8 fn_1_191C(M532BallB *self)
{
    return Hu3DMotionEndCheck(self->model.id) == 0;
}

u8 fn_1_197C(M532BallB *self)
{
    PSVECAdd(&self->vel, &self->pos, &self->pos);
    self->vel.y = self->vel.y - 0.27240694f;
    if (fn_1_1A28(self) != 0) {
        fn_1_EEB8(0x7B1, &self->pos);
        self->state = lbl_1_data_64;
    }
    return 1;
}

u8 fn_1_1A28(M532BallB *self)
{
    Vec d;
    f32 dist;

    PSVECSubtract(&self->target, &self->pos, &d);
    dist = PSVECMag(&d);
    if (self->vel.y < 0.0f && self->pos.y <= self->target.y) {
        self->landed = 1;
        self->pos.x = self->target.x;
        self->pos.y = self->target.y;
        self->pos.z = self->target.z;
    }
    self->dist = dist;
    return self->landed;
}

void fn_1_1AF8(void *self)
{
}

void fn_1_1AFC(void *self)
{
}

void fn_1_1B00(M532BallMgr *self, s32 bit)
{
    self->flags &= ~(1 << bit);
}

void fn_1_1B18(void *self)
{
}

void fn_1_1B1C(M532BallHolder *self, M532Model *model)
{
    self->model = model;
}

// static initialiser of this unit (in .ctors on GameCube)
void fn_1_1B24(void)
{
    lbl_1_bss_0 = 300;
    lbl_1_bss_4 = 60;
}

#ifdef __MWERKS__
__declspec(section ".ctors") void (*const lbl_ctor_1B24)(void) = fn_1_1B24;
#else
struct M532BallStaticInit {
    M532BallStaticInit() { fn_1_1B24(); }
};
static M532BallStaticInit lbl_ball_static_init;
#endif

}
