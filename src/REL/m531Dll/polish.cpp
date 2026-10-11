#include "dolphin/mtx.h"
#include "dolphin/gx.h"

// game/hu3d.h and game/sprite.h are not included: both pull in MSL math.h,
// whose inline std::sqrtf emits constants into .rodata, and this unit has no
// .rodata section of its own.
extern "C" {
typedef s16 HU3DMODELID;
typedef s16 HU3DMOTID;
typedef struct Process_s OMOBJMAN;
typedef struct omObj_s OMOBJ;
typedef void (*OMOBJFUNC)(OMOBJ *obj);
typedef struct Hu3DModel_s HU3DMODEL;
typedef struct Hu3DParticle_s HU3DPARTICLE;
typedef struct ANIMDATA ANIMDATA;

// sizeof(HU3DMODEL) is 0x134, which is how Hu3DData is indexed here.
struct Hu3DModel_s {
    u8 unk000[0x120];
    void *hookData;
    u8 unk124[0x134 - 0x124];
};
extern HU3DMODEL Hu3DData[];

void *__dl__FPv(void *ptr);
// The base constructor and destructor live in effect.cpp, so they are reached
// through their mangled symbols rather than through the class definitions.
void __ct__15CEffectParticleFilUs(void *self, int dataNum, u32 maxCnt, u16 camera);
void __dt__15CEffectParticleFv(void *self, s32 flag);
void __ct__12CEffectModelFP9Process_sssUs(void *self, OMOBJMAN *objMan, s16 layer,
                                          s16 prio, u16 camera);
void __dt__12CEffectModelFv(void *self, s32 flag);

// Defined in effect.cpp too: the shared Hu3D hook both particle classes use.
void fn_1_296C(HU3DMODEL *modelP, struct Hu3DParticle_s *particleP, Mtx *mtx);

void Hu3DParticleColSet(s16 id, u8 r, u8 g, u8 b);
void Hu3DParticleTPLvlSet(s16 id, f32 level);
void Hu3DParticleBlendModeSet(s16 id, s32 mode);
void Hu3DParticleHookSet(s16 id, void (*hook)(HU3DMODEL *, struct Hu3DParticle_s *, Mtx *));
void Hu3DModelLayerSet(HU3DMODELID modelId, s16 layerNo);
void Hu3DModelPosSet(HU3DMODELID modelId, f32 x, f32 y, f32 z);
void Hu3DModelRotSet(HU3DMODELID modelId, f32 x, f32 y, f32 z);
}

static const f32 kZero = 0.0f;

// Base of the particle classes in this unit: an animation pointer at 0x00, a
// model id at 0x04, then the vtable pointer at 0x0C once a derived class adds
// its own data after the two-word base header.
class CEffectParticle {
public:
    ANIMDATA *m_anim;
    HU3DMODELID m_model;
    u16 pad6;
    u32 pad8;
    void **vptr;
    f32 m_a[3];
    f32 m_b[3];

public:
    virtual ~CEffectParticle();
};

// fn_1_70BC belongs to the model family, not the particle family: it calls
// CEffectModel's destructor.
class CEffectModel {
public:
    OMOBJMAN *m_objMan;
    OMOBJ *m_obj;
    u16 pad8;
    void **vptr;

public:
    virtual ~CEffectModel();
};

// fn_1_7030 and fn_1_704C store three floats into the two 12-byte groups that
// follow the base, at 0x10 and 0x1C.
class CParticleFill : public CEffectParticle {
public:
};

// The three classes below each carry a distinct vtable; only the destructor
// pair differs, and the second virtual is not part of this unit's text.
class CParticleFillA : public CParticleFill {
public:
    virtual void fn_1_7120();
    virtual void fn_1_7124(CParticleFillA *self, f32 mtx[3][4]);
    virtual void fn_1_70BC(s16 flag);
};

class CParticleFillB : public CParticleFill {
public:
    virtual void fn_1_74E0(s16 flag);
    virtual void fn_1_6168();
};

class CParticleFillC : public CParticleFill {
public:
    virtual void fn_1_7544(s16 flag);
    virtual void fn_1_5DFC();
};

extern "C" {
void fn_1_7120(CParticleFillA *self);
void fn_1_6168(CParticleFillB *self);
void fn_1_5DFC(CParticleFillC *self);
CEffectModel *fn_1_70BC(CEffectModel *self, s16 flag);
CParticleFillB *fn_1_74E0(CParticleFillB *self, s16 flag);
CParticleFillC *fn_1_7544(CParticleFillC *self, s16 flag);
CEffectParticle *fn_1_5CC4(CEffectParticle *self, s16 flag);
CEffectParticle *fn_1_6654(CEffectParticle *self, s16 flag);
void fn_1_57DC(CEffectParticle *self);
void fn_1_7124(CParticleFillA *self, f32 mtx[3][4]);
}

static void *kVtblFillA[] = {
    NULL, NULL, (void *)fn_1_70BC, (void *)fn_1_7120, (void *)fn_1_7124
};
static void *kVtblFillB[] = {NULL, NULL, (void *)fn_1_74E0, (void *)fn_1_6168};
static void *kVtblFillC[] = {NULL, NULL, (void *)fn_1_7544, (void *)fn_1_5DFC};
static void *kVtblFreeA[] = {NULL, NULL, (void *)fn_1_5CC4, (void *)fn_1_57DC,
                             (void *)NULL};
static void *kVtblFreeB[] = {NULL, NULL, (void *)fn_1_6654, (void *)fn_1_6168};

// vtable slot 0 holds the destructor proper, so it is the same address as the
// deleting destructor below; that is what the original emits as well.
extern "C" {

void fn_1_7120(CParticleFillA *self)
{
    (void)self;
}

void fn_1_6168(CParticleFillB *self)
{
    (void)self;
}

void fn_1_5DFC(CParticleFillC *self)
{
    (void)self;
}

void fn_1_7030(CParticleFill *self, const f32 *src)
{
    self->m_a[0] = src[0];
    self->m_a[1] = src[1];
    self->m_a[2] = src[2];
}

void fn_1_704C(CParticleFill *self, const f32 *src)
{
    self->m_b[0] = src[0];
    self->m_b[1] = src[1];
    self->m_b[2] = src[2];
}

CEffectModel *fn_1_70BC(CEffectModel *self, s16 flag)
{
    if (self != 0) {
        self->vptr = kVtblFillA;
        __dt__12CEffectModelFv(self, 0);
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

CParticleFillB *fn_1_74E0(CParticleFillB *self, s16 flag)
{
    if (self != 0) {
        self->vptr = kVtblFillB;
        __dt__15CEffectParticleFv(self, 0);
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

CParticleFillC *fn_1_7544(CParticleFillC *self, s16 flag)
{
    if (self != 0) {
        self->vptr = kVtblFillC;
        __dt__15CEffectParticleFv(self, 0);
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

CEffectParticle *fn_1_5CC4(CEffectParticle *self, s16 flag)
{
    if (self != 0) {
        self->vptr = kVtblFreeA;
        __dt__15CEffectParticleFv(self, 0);
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

CEffectParticle *fn_1_6654(CEffectParticle *self, s16 flag)
{
    if (self != 0) {
        self->vptr = kVtblFreeB;
        __dt__15CEffectParticleFv(self, 0);
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

// Each constructor runs the base constructor, stamps its own vtable over the
// base's, then configures the particle: colour, texture level, blend mode, the
// shared hook, the render layer, and the model's position and rotation. These
// are written out one by one rather than sharing a helper, because the helper
// form changes each one's prologue.

// 0x5902, 0x5904 and 0x5905 are data numbers, built as lis+addi.
extern "C" {

CEffectParticle *fn_1_5700(CEffectParticle *self, const f32 *rot)
{
    s16 id;

    __ct__15CEffectParticleFilUs(self, 0x5902, 8, 1);
    self->vptr = kVtblFreeA;
    id = (s16)self->m_model;
    Hu3DParticleColSet(id, 0xFF, 0xFF, 0xFF);
    Hu3DParticleTPLvlSet(id, kZero);
    Hu3DParticleBlendModeSet(id, 0);
    Hu3DParticleHookSet(id, fn_1_296C);
    Hu3DModelLayerSet(id, 5);
    Hu3DModelPosSet(id, rot[0], rot[1], rot[2]);
    return self;
}

// NONMATCHING: the original keeps self in r28 and builds the data number with
// lis+addi; a plain local makes MWCC pick r26 and a plain literal gives li
CEffectParticle *fn_1_6080(CEffectParticle *self, u32 maxCnt, const f32 *pos, const f32 *rot)
{
    int dataNum = 0x5900;
    s16 id;

    dataNum = dataNum + 4;
    __ct__15CEffectParticleFilUs(self, dataNum, maxCnt, 1);
    self->vptr = kVtblFreeB;
    id = (s16)self->m_model;
    Hu3DParticleColSet(id, 0xFF, 0xFF, 0xFF);
    Hu3DParticleTPLvlSet(id, kZero);
    Hu3DParticleBlendModeSet(id, 1);
    Hu3DParticleHookSet(id, fn_1_296C);
    Hu3DModelLayerSet(id, 5);
    Hu3DModelPosSet(id, pos[0], pos[1], pos[2]);
    Hu3DModelRotSet(id, rot[0], rot[1], rot[2]);
    return self;
}

// NONMATCHING: frame setup differs - original uses _savegpr_27 (5 GPRs + 5 FPRs)
// while MWCC allocates different callee-saved registers for this body
CEffectParticle *fn_1_5D28(void *self, u32 maxCnt, const f32 *pos, const f32 *rot)
{
    CEffectParticle *p = (CEffectParticle *)self;

    __ct__15CEffectParticleFilUs(self, 0x5905, maxCnt, 1);
    p->vptr = kVtblFillC;
    Hu3DParticleColSet(p->m_model, 0xE0, 0xFF, 0xFF);
    Hu3DParticleBlendModeSet(p->m_model, 1);
    Hu3DParticleHookSet(p->m_model, fn_1_296C);
    Hu3DModelLayerSet(p->m_model, 5);
    Hu3DModelPosSet(p->m_model, pos[0], pos[1], pos[2]);
    Hu3DModelRotSet(p->m_model, rot[0], rot[1], rot[2]);
    return p;
}

CEffectParticle *fn_1_66B8(void *self, u32 maxCnt, const f32 *pos, const f32 *rot)
{
    CEffectParticle *p = (CEffectParticle *)self;

    __ct__15CEffectParticleFilUs(self, 0x5904, maxCnt, 1);
    p->vptr = kVtblFillB;
    Hu3DParticleColSet(p->m_model, 0xFF, 0xFF, 0xFF);
    Hu3DParticleTPLvlSet(p->m_model, kZero);
    Hu3DParticleBlendModeSet(p->m_model, 1);
    Hu3DParticleHookSet(p->m_model, fn_1_296C);
    Hu3DModelLayerSet(p->m_model, 5);
    Hu3DModelPosSet(p->m_model, pos[0], pos[1], pos[2]);
    Hu3DModelRotSet(p->m_model, rot[0], rot[1], rot[2]);
    return p;
}

CEffectModel *fn_1_7068(void *self, OMOBJMAN *objMan)
{
    __ct__12CEffectModelFP9Process_sssUs(self, objMan, 4, 5, 1);
    ((CEffectModel *)self)->vptr = kVtblFillA;
    return (CEffectModel *)self;
}

} // extern "C"

} // extern "C"
// NONMATCHING: two residual gaps vs the original ' GXSetChanCtrl's enable/light-mask
// and its diffuse/attn pair are swapped, and the f30/f31 load order at the first
// vertex differs. Body is otherwise identical instruction-for-instruction.
// Billboard draw: sets up an alpha-blended GX pipeline with no textures and
// no culling, then emits a five-vertex strip. Vertices 0/2/4 use the quad's
// near edge and 1/3 its far edge; every vertex carries the same tint
// (r=0xFF, g=b=0x28, a=0xFF). Positions go straight into the XFIFO.
#define XFIFO_F ((volatile f32 *)0xCC008000)
#define XFIFO_C ((volatile u8 *)0xCC008000)

static const f32 kBillboardZ = 0.0f;

void fn_1_7124(CParticleFillA *self, f32 mtx[3][4])
{
    GXLoadPosMtxImm(mtx, 0);
    GXClearVtxDesc();
    GXSetVtxDesc((GXAttr)9, (GXAttrType)1);
    GXSetVtxDesc((GXAttr)0xB, (GXAttrType)1);
    GXSetVtxAttrFmt((GXVtxFmt)0, (GXAttr)9, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt((GXVtxFmt)0, (GXAttr)0xB, GX_POS_XYZ, (GXCompType)5, 0);
    GXSetNumTevStages(1);
    GXSetNumTexGens(0);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, (GXChannelID)4);
    GXSetTevOp(GX_TEVSTAGE0, (GXTevMode)4);
    GXSetZMode(GX_FALSE, (GXCompare)7, GX_FALSE);
    GXSetNumChans(1);
    GXSetChanCtrl((GXChannelID)4, GX_FALSE, (GXColorSrc)0, (GXColorSrc)1, 1, (GXDiffuseFn)1, (GXAttnFn)2);
    GXSetZCompLoc(0);
    GXSetAlphaCompare((GXCompare)6, 0x1, GX_AOP_AND, (GXCompare)6, 0x1);
    GXSetBlendMode((GXBlendMode)0, (GXBlendFactor)1, (GXBlendFactor)1, (GXLogicOp)0x5);
    GXSetAlphaUpdate(GX_TRUE);

    f32 *near_x = (f32 *)((u8 *)self + 0x10);
    f32 *far_x = (f32 *)((u8 *)self + 0x1C);
    GXBegin((GXPrimitive)0xB0, (GXVtxFmt)0, 5);

    f32 v0 = near_x[2];
    f32 v1 = near_x[0];
    f32 v2 = far_x[2];
    f32 v3 = far_x[0];
    f32 v4 = near_x[2];
    f32 v5 = near_x[0];
    f32 v6 = far_x[0];
    f32 v7 = far_x[2];
    f32 v8 = near_x[0];
    f32 v9 = near_x[2];
    XFIFO_F[0] = v0; XFIFO_F[0] = kBillboardZ; XFIFO_F[0] = v1;
    XFIFO_C[0] = 0xFF; XFIFO_C[0] = 0x28; XFIFO_C[0] = 0x28; XFIFO_C[0] = 0xFF;
    XFIFO_F[0] = v4; XFIFO_F[0] = kBillboardZ; XFIFO_F[0] = v2;
    XFIFO_C[0] = 0xFF; XFIFO_C[0] = 0x28; XFIFO_C[0] = 0x28; XFIFO_C[0] = 0xFF;
    XFIFO_F[0] = v3; XFIFO_F[0] = kBillboardZ; XFIFO_F[0] = v5;
    XFIFO_C[0] = 0xFF; XFIFO_C[0] = 0x28; XFIFO_C[0] = 0x28; XFIFO_C[0] = 0xFF;
    XFIFO_F[0] = v6; XFIFO_F[0] = kBillboardZ; XFIFO_F[0] = v7;
    XFIFO_C[0] = 0xFF; XFIFO_C[0] = 0x28; XFIFO_C[0] = 0x28; XFIFO_C[0] = 0xFF;
    XFIFO_F[0] = v8; XFIFO_F[0] = kBillboardZ; XFIFO_F[0] = v9;
    XFIFO_C[0] = 0xFF; XFIFO_C[0] = 0x28; XFIFO_C[0] = 0x28; XFIFO_C[0] = 0xFF;
    return;

    GXBegin((GXPrimitive)0xB0, (GXVtxFmt)0, 5);

    XFIFO_F[0] = near_x[2];
    XFIFO_F[0] = kBillboardZ;
    XFIFO_F[0] = near_x[0];
    XFIFO_C[0] = 0xFF;
    XFIFO_C[0] = 0x28;
    XFIFO_C[0] = 0x28;
    XFIFO_C[0] = 0xFF;

    XFIFO_F[0] = near_x[2];
    XFIFO_F[0] = kBillboardZ;
    XFIFO_F[0] = far_x[2];
    XFIFO_C[0] = 0xFF;
    XFIFO_C[0] = 0x28;
    XFIFO_C[0] = 0x28;
    XFIFO_C[0] = 0xFF;

    XFIFO_F[0] = far_x[0];
    XFIFO_F[0] = kBillboardZ;
    XFIFO_F[0] = far_x[2];
    XFIFO_C[0] = 0xFF;
    XFIFO_C[0] = 0x28;
    XFIFO_C[0] = 0x28;
    XFIFO_C[0] = 0xFF;

    XFIFO_F[0] = far_x[0];
    XFIFO_F[0] = kBillboardZ;
    XFIFO_F[0] = near_x[0];
    XFIFO_C[0] = 0xFF;
    XFIFO_C[0] = 0x28;
    XFIFO_C[0] = 0x28;
    XFIFO_C[0] = 0xFF;

    XFIFO_F[0] = near_x[0];
    XFIFO_F[0] = kBillboardZ;
    XFIFO_F[0] = near_x[2];
    XFIFO_C[0] = 0xFF;
    XFIFO_C[0] = 0x28;
    XFIFO_C[0] = 0x28;
    XFIFO_C[0] = 0xFF;
}
