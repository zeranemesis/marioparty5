// m531Dll "polish": the particle and model effects of the cleaning contest
// (dust puffs, sparkles, polish lines), all built on the engine particle
// manager. Each effect class carries its vtable pointer after the base data
// (offset 0xC on the GameCube) and is driven through hand-written vtables.
#define _MATH_H
extern "C" {
#include "dolphin.h"
#include "game/hu3d.h"
#include "game/frand.h"
}

extern "C" {
double sin(double x);
double cos(double x);

void *__dl__FPv(void *ptr);
// The base constructors and destructors live in effect.cpp, so they are reached
// through their mangled symbols rather than through the class definitions.
void __ct__15CEffectParticleFilUs(void *self, int dataNum, u32 maxCnt, u16 camera);
void __dt__15CEffectParticleFv(void *self, s32 flag);
void __ct__12CEffectModelFP9Process_sssUs(void *self, struct Process_s *objMan, s16 layer, s16 prio, u16 camera);
void __dt__12CEffectModelFv(void *self, s32 flag);

// Defined in effect.cpp too: the shared Hu3D hook all particle classes use.
void fn_1_296C(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);
void fn_1_2B3C(void *self);
}

struct CEffectParticle;

struct EffectParticleVt {
    void *m_head[2];
    CEffectParticle *(*m_dtor)(CEffectParticle *self, s16 flag);
    void (*m_update)(CEffectParticle *self);
};

// Base of the particle classes: animation, model id, particle work, vtable.
struct CEffectParticle {
    ANIMDATA *m_anim;
    HU3DMODELID m_model;
    HU3DPARTICLE *m_work;
    const EffectParticleVt *m_vt;
};

// A particle effect with two Vec parameters (position and rotation/area).
struct CParticleFill {
    CEffectParticle m_base;
    Vec m_a; // 0x10
    Vec m_b; // 0x1C
};

struct CEffectModel;

struct EffectModelVt {
    void *m_head[2];
    void (*m_update)(CEffectModel *self);
    void (*m_draw)(CEffectModel *self, Mtx mtx);
    CEffectModel *(*m_dtor)(CEffectModel *self, s16 flag);
};

struct CEffectModel {
    struct Process_s *m_objMan;
    struct omObj_s *m_obj;
    HU3DMODELID m_model;
    const EffectModelVt *m_vt;
};

// A drawn model effect carrying the same two Vec parameters.
struct CModelFill {
    CEffectModel m_base;
    Vec m_a; // 0x10
    Vec m_b; // 0x1C
};

extern "C" {
void *fn_1_5700(CEffectParticle *self, const Vec *pos);
void fn_1_57DC(CEffectParticle *self);
CEffectParticle *fn_1_5CC4(CEffectParticle *self, s16 flag);
void *fn_1_5D28(CEffectParticle *self, s32 dataNum, s32 maxCnt, const Vec *pos, const Vec *rot);
void fn_1_5DFC(CEffectParticle *self);
void *fn_1_6080(CEffectParticle *self, s32 maxCnt, const Vec *pos, const Vec *rot);
void fn_1_6168(CEffectParticle *self);
CEffectParticle *fn_1_6654(CEffectParticle *self, s16 flag);
void *fn_1_66B8(CParticleFill *self, s32 maxCnt, const Vec *pos, const Vec *rot);
void fn_1_6808(CParticleFill *self, HU3DPARTICLEDATA *data);
void fn_1_6A8C(CEffectParticle *self);
void fn_1_7030(CParticleFill *self, const Vec *src);
void fn_1_704C(CParticleFill *self, const Vec *src);
void *fn_1_7068(CModelFill *self, struct Process_s *objMan);
CEffectModel *fn_1_70BC(CEffectModel *self, s16 flag);
void fn_1_7120(CEffectModel *self);
void fn_1_7124(CModelFill *self, Mtx mtx);
CEffectParticle *fn_1_74E0(CEffectParticle *self, s16 flag);
CEffectParticle *fn_1_7544(CEffectParticle *self, s16 flag);

extern EffectModelVt lbl_1_data_168;
extern EffectParticleVt lbl_1_data_17C;
extern EffectParticleVt lbl_1_data_18C;
extern EffectParticleVt lbl_1_data_19C;
extern EffectParticleVt lbl_1_data_1AC;
}

void *fn_1_5700(CEffectParticle *self, const Vec *pos)
{
    s16 id;

    __ct__15CEffectParticleFilUs(self, 0x590002, 8, 1);
    self->m_vt = &lbl_1_data_1AC;
    id = self->m_model;
    Hu3DParticleColSet(id, 0xFF, 0xFF, 0xFF);
    Hu3DParticleTPLvlSet(id, 0.0f);
    Hu3DParticleBlendModeSet(id, 0);
    Hu3DParticleHookSet(id, fn_1_296C);
    Hu3DModelLayerSet(id, 5);
    Hu3DModelPosSet(id, pos->x, pos->y, pos->z);
    return self;
}

void fn_1_57DC(CEffectParticle *self)
{
    HU3DMODEL *modelP;
    HU3DPARTICLE *par;
    HU3DPARTICLEDATA *d;
    s32 i;
    s32 active;
    f32 phase;
    f32 diff;

    modelP = &Hu3DData[self->m_model];
    par = self->m_work;
    if (par->count == 0) {
        d = par->data;
        for (i = 0; i < par->maxCnt; i++, d++) {
            phase = 360.0f * (f32)i;
            d->vel.x = sin(0.017453292f * (phase / (f32)par->maxCnt));
            d->vel.y = 0.01f + (0.000015258789f * (f32)frand());
            phase = 360.0f * (f32)i;
            d->vel.z = cos(0.017453292f * (phase / (f32)par->maxCnt));
            d->scale = 100.0f + (50.0f * (0.000015258789f * (f32)frand()));
            d->pos.x = 0.05f * (100.0f * d->vel.x);
            d->pos.y = 0.0f;
            d->pos.z = 0.05f * (100.0f * d->vel.z);
            d->color.a = 0;
            d->time = 0;
            d->parManId = 0;
            d->zRot = 0.017453292f * (360.0f * (0.000015258789f * (f32)frand()));
        }
    }
    d = par->data;
    active = 0;
    for (i = 0; i < par->maxCnt; i++, d++) {
        if (d->time == 0 || d->color.a != 0) {
            if (d->time == 0) {
                d->color.a = d->color.a + 0x10;
                if (d->color.a >= 0xF0) {
                    d->color.a = 0xF0;
                    d->time = d->time + 1;
                }
            } else if (d->color.a != 0) {
                d->color.a = d->color.a - 8;
            }
            active++;
            {
                s16 t = d->parManId;
                d->parManId = t + 1;
                d->scale = d->scale + (0.1f * (f32)t);
            }
            if ((600.0f - d->scale) < 0.0f) {
                diff = -600.0f - d->scale;
            } else {
                diff = 600.0f - d->scale;
            }
            phase = 0.005f * diff;
            if (d->scale <= 0.0f) {
                d->color.a = 0;
            }
            d->pos.x = d->pos.x + (d->vel.x * phase);
            d->pos.y = d->pos.y + d->vel.y;
            d->pos.z = d->pos.z + (d->vel.z * phase);
        }
    }
    if (active == 0 && self != NULL) {
        self->m_vt->m_dtor(self, 1);
    }
}

CEffectParticle *fn_1_5CC4(CEffectParticle *self, s16 flag)
{
    if (self != NULL) {
        self->m_vt = &lbl_1_data_1AC;
        __dt__15CEffectParticleFv(self, 0);
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

void *fn_1_5D28(CEffectParticle *self, s32 dataNum, s32 maxCnt, const Vec *pos, const Vec *rot)
{
    s16 id;

    __ct__15CEffectParticleFilUs(self, dataNum, maxCnt, 1);
    self->m_vt = &lbl_1_data_19C;
    id = self->m_model;
    Hu3DParticleColSet(id, 0xE0, 0xFF, 0xFF);
    Hu3DParticleBlendModeSet(id, 0);
    Hu3DParticleHookSet(id, fn_1_296C);
    Hu3DModelLayerSet(id, 5);
    Hu3DModelPosSet(id, pos->x, pos->y, pos->z);
    Hu3DModelRotSet(id, rot->x, rot->y, rot->z);
    return self;
}

void fn_1_5DFC(CEffectParticle *self)
{
    HU3DMODEL *modelP;
    HU3DPARTICLE *par;
    HU3DPARTICLEDATA *d;
    s32 i;
    f32 fi;

    modelP = &Hu3DData[self->m_model];
    par = self->m_work;
    if (par->count == 0) {
        i = 0;
        d = par->data;
        for (; i < par->maxCnt; i++, d++) {
            d->vel.x = 0.0f;
            d->vel.y = 0.0f;
            d->vel.z = 10.0f;
            d->scale = 0.0f;
            d->pos.x = 0.0f;
            d->pos.y = 0.0f;
            fi = i;
            d->pos.z = fi * (150.0f / (f32)par->maxCnt);
            d->color.a = 0xFF;
            d->time = 0;
            d->parManId = 0;
            d->zRot = 0.017453292f * (360.0f * (0.000015258789f * (f32)frand()));
        }
    }
    i = 0;
    d = par->data;
    for (; i < par->maxCnt; i++, d++) {
        if (d->pos.z > 150.0f) {
            d->pos.x = 0.0f;
            d->pos.y = 0.0f;
            d->pos.z = 0.0f;
            d->scale = 0.0f;
            d->color.a = 0xFF;
        } else {
            d->pos.x = d->pos.x + d->vel.x;
            d->pos.y = d->pos.y + d->vel.y;
            d->pos.z = d->pos.z + d->vel.z;
            d->scale = d->pos.z / 1.5f;
        }
    }
}

void *fn_1_6080(CEffectParticle *self, s32 maxCnt, const Vec *pos, const Vec *rot)
{
    s16 id;

    __ct__15CEffectParticleFilUs(self, 0x590004, maxCnt, 1);
    self->m_vt = &lbl_1_data_18C;
    id = self->m_model;
    Hu3DParticleColSet(id, 0xFF, 0xFF, 0xFF);
    Hu3DParticleTPLvlSet(id, 0.0f);
    Hu3DParticleBlendModeSet(id, 1);
    Hu3DParticleHookSet(id, fn_1_296C);
    Hu3DModelLayerSet(id, 5);
    Hu3DModelPosSet(id, pos->x, pos->y, pos->z);
    Hu3DModelRotSet(id, rot->x, rot->y, rot->z);
    return self;
}

void fn_1_6168(CEffectParticle *self)
{
    HU3DMODEL *modelP;
    HU3DPARTICLE *par;
    HU3DPARTICLEDATA *d;
    s32 i;
    f32 r;
    f32 a;
    f32 b;
    f32 c;
    f32 f;
    u32 col;

    modelP = &Hu3DData[self->m_model];
    par = self->m_work;
    if (par->count == 0) {
        i = 0;
        d = par->data;
        for (; i < par->maxCnt; i++, d++) {
            d->vel.y = 0.0f;
            r = 0.25f + (0.25f * (0.000015258789f * (f32)frand()));
            d->scale = 100.0f * r;
            d->vel.x = d->scale;
            a = 0.000030517578f * ((f32)frand() - 32768.0f);
            d->pos.x = 150.0f * a;
            b = 0.000030517578f * ((f32)frand() - 32768.0f);
            d->pos.y = 150.0f * b;
            c = 0.000030517578f * ((f32)frand() - 32768.0f);
            d->pos.z = 100.0f * c;
            d->time = 0;
            f = 360.0f * (0.000015258789f * (f32)frand());
            d->zRot = 0.017453292f * f;
            col = frand();
            d->color.r = col;
            d->color.g = col >> 8;
            d->color.b = col >> 16;
            d->color.a = 0;
        }
    }
    par->dataCnt = 0;
    i = 0;
    d = par->data;
    for (; i < par->maxCnt; i++, d++) {
        if (d->time <= 1) {
            switch (d->time) {
                case 0:
                    d->color.a = (u8)(s32)((f32)d->color.a + (4.0f * (0.04f * d->scale)));
                    par->dataCnt = par->dataCnt + 1;
                    if (d->color.a < 0xFA) {
                        break;
                    }
                    d->color.a = 0xFF;
                    d->time = d->time + 1;
                case 1:
                    d->color.a = (u8)(s32)((f32)d->color.a - (4.0f * (0.04f * d->scale)));
                    d->scale = d->vel.x * ((f32)d->color.a / 255.0f);
                    if (d->scale < 0.0f) {
                        d->scale = 0.0f;
                    }
                    par->dataCnt = par->dataCnt + 1;
                    if (d->color.a <= 8) {
                        d->time = d->time + 1;
                    }
                    break;
            }
            d->zRot = d->zRot + (0.017453292f * d->vel.y);
            d->vel.y = d->vel.y + 0.002724069383556748;
            d->pos.y = d->pos.y - d->vel.y;
        }
    }
    if (par->dataCnt == 0 && self != NULL) {
        self->m_vt->m_dtor(self, 1);
    }
}

CEffectParticle *fn_1_6654(CEffectParticle *self, s16 flag)
{
    if (self != NULL) {
        self->m_vt = &lbl_1_data_18C;
        __dt__15CEffectParticleFv(self, 0);
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

void *fn_1_66B8(CParticleFill *self, s32 maxCnt, const Vec *pos, const Vec *rot)
{
    s16 id;

    __ct__15CEffectParticleFilUs(self, 0x590004, maxCnt, 1);
    self->m_base.m_vt = &lbl_1_data_17C;
    id = self->m_base.m_model;
    Hu3DParticleColSet(id, 0xFF, 0xFF, 0xFF);
    Hu3DParticleTPLvlSet(id, 0.0f);
    Hu3DParticleBlendModeSet(id, 1);
    Hu3DParticleHookSet(id, fn_1_296C);
    Hu3DModelLayerSet(id, 5);
    Hu3DModelPosSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
    self->m_a.x = pos->x;
    self->m_a.y = pos->y;
    self->m_a.z = pos->z;
    self->m_b.x = rot->x;
    self->m_b.y = rot->y;
    self->m_b.z = rot->z;
    fn_1_2B3C(&self->m_base);
    return self;
}

// Starts (or restarts) one polish spark around the fill position.
static inline void SparkRespawn(CParticleFill *self, HU3DPARTICLEDATA *d)
{
    s32 r;
    f32 a;
    f32 b;
    f32 c;
    f32 e;
    f32 f;
    u32 col;

    d->time = 0;
    r = frand() & 0xF;
    d->parManId = r + 4;
    a = 50.0f * (0.000030517578f * ((f32)frand() - 32768.0f));
    d->pos.x = self->m_a.x + a;
    b = 50.0f * (0.000030517578f * ((f32)frand() - 32768.0f));
    d->pos.y = 50.0f + b;
    c = 50.0f * (0.000030517578f * ((f32)frand() - 32768.0f));
    d->pos.z = self->m_a.z + c;
    e = 0.2f + (0.2f * (0.000015258789f * (f32)frand()));
    d->scale = 100.0f * e;
    f = 360.0f * (0.000015258789f * (f32)frand());
    d->zRot = 0.017453292f * f;
    col = frand();
    d->color.r = col;
    d->color.g = col >> 8;
    d->color.b = col >> 16;
    d->color.a = 0;
}

void fn_1_6808(CParticleFill *self, HU3DPARTICLEDATA *data)
{
    SparkRespawn(self, data);
}

void fn_1_6A8C(CEffectParticle *self)
{
    HU3DMODEL *modelP;
    HU3DPARTICLE *par;
    HU3DPARTICLEDATA *d;
    s32 i;

    modelP = &Hu3DData[self->m_model];
    par = self->m_work;
    if (par->count == 0) {
        i = 0;
        d = par->data;
        for (; i < par->maxCnt; i++, d++) {
            SparkRespawn((CParticleFill *)self, d);
        }
    }
    i = 0;
    d = par->data;
    for (; i < par->maxCnt; i++, d++) {
        if (d->time == 0) {
            d->color.a = d->color.a + 4;
            if (d->color.a > 0xFA) {
                d->color.a = 0xFF;
                d->time = d->time + 1;
            }
        } else {
            d->color.a = d->color.a - d->parManId;
            if (d->color.a < 0x14) {
                SparkRespawn((CParticleFill *)self, d);
            }
        }
        d->zRot = d->zRot + 0.017453292f;
    }
}

void fn_1_7030(CParticleFill *self, const Vec *src)
{
    self->m_a.x = src->x;
    self->m_a.y = src->y;
    self->m_a.z = src->z;
}

void fn_1_704C(CParticleFill *self, const Vec *src)
{
    self->m_b.x = src->x;
    self->m_b.y = src->y;
    self->m_b.z = src->z;
}

void *fn_1_7068(CModelFill *self, struct Process_s *objMan)
{
    __ct__12CEffectModelFP9Process_sssUs(self, objMan, 4, 5, 1);
    self->m_base.m_vt = &lbl_1_data_168;
    return self;
}

CEffectModel *fn_1_70BC(CEffectModel *self, s16 flag)
{
    if (self != NULL) {
        self->m_vt = &lbl_1_data_168;
        __dt__12CEffectModelFv(self, 0);
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

void fn_1_7120(CEffectModel *self)
{
}

// Draws the polish area as a red outline (five-vertex line strip) on the floor.
void fn_1_7124(CModelFill *self, Mtx mtx)
{
    f32 v0, v1, v2, v3, v4, v5, v6, v7, v8, v9;

    GXLoadPosMtxImm(mtx, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetNumTevStages(1);
    GXSetNumTexGens(0);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT0, GX_DF_CLAMP, GX_AF_SPOT);
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ONE, GX_LO_NOOP);
    GXSetAlphaUpdate(GX_TRUE);
    GXBegin(GX_LINESTRIP, GX_VTXFMT0, 5);
    v0 = self->m_a.x;
    v1 = self->m_a.z;
    v2 = self->m_a.x;
    v3 = self->m_b.z;
    v4 = self->m_b.x;
    v5 = self->m_b.z;
    v6 = self->m_b.x;
    v7 = self->m_a.z;
    v8 = self->m_a.x;
    v9 = self->m_a.z;
#ifdef TARGET_PC
    GXPosition3f32(v0, 0.0f, v1);
    GXColor4u8(0xFF, 0x28, 0x28, 0xFF);
    GXPosition3f32(v2, 0.0f, v3);
    GXColor4u8(0xFF, 0x28, 0x28, 0xFF);
    GXPosition3f32(v4, 0.0f, v5);
    GXColor4u8(0xFF, 0x28, 0x28, 0xFF);
    GXPosition3f32(v6, 0.0f, v7);
    GXColor4u8(0xFF, 0x28, 0x28, 0xFF);
    GXPosition3f32(v8, 0.0f, v9);
    GXColor4u8(0xFF, 0x28, 0x28, 0xFF);
    GXEnd();
#else
    GXWGFifo.f32 = v0;
    GXWGFifo.f32 = 0.0f;
    GXWGFifo.f32 = v1;
    GXWGFifo.u8 = 0xFF;
    GXWGFifo.u8 = 0x28;
    GXWGFifo.u8 = 0x28;
    GXWGFifo.u8 = 0xFF;
    GXWGFifo.f32 = v2;
    GXWGFifo.f32 = 0.0f;
    GXWGFifo.f32 = v3;
    GXWGFifo.u8 = 0xFF;
    GXWGFifo.u8 = 0x28;
    GXWGFifo.u8 = 0x28;
    GXWGFifo.u8 = 0xFF;
    GXWGFifo.f32 = v4;
    GXWGFifo.f32 = 0.0f;
    GXWGFifo.f32 = v5;
    GXWGFifo.u8 = 0xFF;
    GXWGFifo.u8 = 0x28;
    GXWGFifo.u8 = 0x28;
    GXWGFifo.u8 = 0xFF;
    GXWGFifo.f32 = v6;
    GXWGFifo.f32 = 0.0f;
    GXWGFifo.f32 = v7;
    GXWGFifo.u8 = 0xFF;
    GXWGFifo.u8 = 0x28;
    GXWGFifo.u8 = 0x28;
    GXWGFifo.u8 = 0xFF;
    GXWGFifo.f32 = v8;
    GXWGFifo.f32 = 0.0f;
    GXWGFifo.f32 = v9;
    GXWGFifo.u8 = 0xFF;
    GXWGFifo.u8 = 0x28;
    GXWGFifo.u8 = 0x28;
    GXWGFifo.u8 = 0xFF;
#endif
}

CEffectParticle *fn_1_74E0(CEffectParticle *self, s16 flag)
{
    if (self != NULL) {
        self->m_vt = &lbl_1_data_17C;
        __dt__15CEffectParticleFv(self, 0);
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

CEffectParticle *fn_1_7544(CEffectParticle *self, s16 flag)
{
    if (self != NULL) {
        self->m_vt = &lbl_1_data_19C;
        __dt__15CEffectParticleFv(self, 0);
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

// Vtables: two zero header words, then the slots in order.
EffectModelVt lbl_1_data_168 = { { NULL, NULL }, fn_1_7120, (void (*)(CEffectModel *, Mtx))fn_1_7124, fn_1_70BC };
EffectParticleVt lbl_1_data_17C = { { NULL, NULL }, fn_1_74E0, fn_1_6A8C };
EffectParticleVt lbl_1_data_18C = { { NULL, NULL }, fn_1_6654, fn_1_6168 };
EffectParticleVt lbl_1_data_19C = { { NULL, NULL }, fn_1_7544, fn_1_5DFC };
EffectParticleVt lbl_1_data_1AC = { { NULL, NULL }, fn_1_5CC4, fn_1_57DC };
