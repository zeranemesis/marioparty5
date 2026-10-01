#include "REL/m539Dll.h"

/*
 * m539's "effect": a puff of smoke that trails a model.
 *
 * The particle object owns a flat array of M539ParticleData. Every entry keeps
 * a unit heading (sin/cos of a share of a turn), a start position and an alpha
 * that ramps up to 220 and then fades back out, so a burst leaves the model
 * and dissolves behind it.
 */

extern "C" {

typedef struct M539ParticleData_s {
    s16 time;
    s16 parManId;
    s16 unk4;
    s16 cameraBit;
    Vec vel;
    Vec accel;
    f32 speedDecay;
    f32 colorIdx;
    f32 scaleBase;
    f32 scale;
    f32 zRot;
    Vec pos;
    struct {
        u8 r, g, b, a;
    } color;
} M539ParticleData; // 0x44 bytes

typedef struct M539Particle_s {
    s16 dataCnt;
    s16 emitCnt;
    Vec pos;
    Vec unk_10;
    void *work;
    s16 animBank;
    s16 animNo;
    f32 animSpeed;
    f32 animTime;
    s16 maxCnt;
    u32 count;
    u32 prevCounter;
    u32 prevCount;
    u32 dlSize;
    void *anim;
    M539ParticleData *data;
} M539Particle;

// Hu3DData has a 0x134 stride and carries the particle's work pointer at 0x120.
typedef struct M539Hu3DData_s {
    u8 unk_000[0x120];
    void *hookData;
    u8 unk_124[0x134 - 0x124];
} M539Hu3DData;

extern M539Hu3DData Hu3DData[];

typedef struct M539Effect_s {
    void *vtable;
    s16 unk4;
    s16 modelId;
    void *anim;
    M539Particle *particle;
    u8 flag;
    u8 unk_15[7];
    s32 unk_18;
    s32 unk_1C;
} M539Effect;

void *HuDataSelHeapReadNum(u32 dataNum, u32 num, s32 heap);
void *HuSprAnimRead(void *data);
void HuSprAnimLock(void *anim);
void HuSprAnimKill(void *anim);
s16 Hu3DParticleCreate(void *anim, s16 maxCnt);
void Hu3DParticleHookSet(s16 modelId, void *hook);
void Hu3DParticleColSet(s16 modelId, u8 r, u8 g, u8 b);
void Hu3DParticleTPLvlSet(s16 modelId, f32 tpLvl);
void Hu3DParticleBlendModeSet(s16 modelId, u8 blendMode);
void Hu3DModelLayerSet(s16 modelId, s16 layerNo);
void Hu3DModelPosSet(s16 modelId, f32 x, f32 y, f32 z);
void Hu3DModelRotSet(s16 modelId, f32 x, f32 y, f32 z);
void Hu3DModelAttrSet(s16 modelId, u32 attr);
void Hu3DModelAttrReset(s16 modelId, u32 attr);
void Hu3DModelKill(s16 modelId);
void DCStoreRangeNoSync(void *start, u32 size);

void fn_1_1264(M539Effect *self);
void fn_1_1270(M539Effect *self, s32 arg);

s32 fn_1_4AE0(const f32 *arg);
f32 fn_1_4FC8(const f32 *arg, f32 min, f32 max);

void fn_1_CE40(M539Effect *self, M539Particle *particleP);
void fn_1_CBAC(M539Effect *self, M539Particle *particleP);

// The two literals the unit takes the address of, for the random helpers.
static const f32 lbl_1_rodata_258 = 0.0f;
static const f32 lbl_1_rodata_264 = 50.0f;

// NONMATCHING: register allocation only.
void fn_1_C8DC(void *modelP, void *particleP, void *mtx)
{
    M539Effect *self = *(M539Effect **)((char *)modelP + 0x1C);

    fn_1_CE40(self, (M539Particle *)particleP);
}

// NONMATCHING: register allocation only.
M539Effect *fn_1_C920(M539Effect *self, Vec *pos)
{
    s16 modelId;

    fn_1_1264(self);
    self->anim = HuSprAnimRead(HuDataSelHeapReadNum(0x61000C, 0x10000000, 2));
    HuSprAnimLock(self->anim);
    modelId = Hu3DParticleCreate(self->anim, 0x10);
    self->particle = (M539Particle *)Hu3DData[modelId].hookData;
    self->particle->work = self;
    Hu3DParticleHookSet(modelId, (void *)fn_1_C8DC);
    Hu3DParticleColSet(modelId, 0xF0, 0xF0, 0xF0);
    Hu3DParticleTPLvlSet(modelId, 0.0f);
    Hu3DParticleBlendModeSet(modelId, 0);
    Hu3DModelLayerSet(modelId, 5);
    Hu3DModelPosSet(modelId, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(modelId, 0.0f, 0.0f, 0.0f);
    self->modelId = modelId;
    self->flag = 1;
    fn_1_CBAC(self, self->particle);
    Hu3DModelAttrSet(modelId, 1);
    Hu3DModelPosSet(self->modelId, pos->x, pos->y, pos->z);
    Hu3DParticleTPLvlSet(self->modelId, 0.0f);
    self->flag = 0;
    Hu3DModelAttrReset(self->modelId, 1);
    return self;
}

// NONMATCHING: register allocation only.
M539Effect *fn_1_CAD8(M539Effect *self, s16 arg)
{
    s16 mode = arg;

    if (self != NULL) {
        fn_1_1270(self, 0);
        if (mode > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

// NONMATCHING: register allocation only.
M539Effect *fn_1_CB30(M539Effect *self, s16 arg)
{
    s16 mode = arg;

    if (self != NULL) {
        self->particle->work = NULL;
        Hu3DModelKill(self->modelId);
        HuSprAnimKill(self->anim);
        fn_1_1270(self, 0);
        if (mode > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

// NONMATCHING: register allocation only - the int-to-float conversions of the
// loop index and of the count go through a different rounding sequence.
void fn_1_CBAC(M539Effect *self, M539Particle *particleP)
{
    M539ParticleData *p;
    s16 i;

    for (p = particleP->data, i = 0; i < particleP->maxCnt; i++, p++) {
        p->vel.x = (f32)sin(0.017453292f * ((360.0f * i) / particleP->maxCnt));
        p->vel.y = (f32)cos(0.017453292f * ((360.0f * i) / particleP->maxCnt));
        p->vel.z = 0.0f;
        p->scale = 50.0f;
        p->pos.x = 100.0f * (0.01f * ((f64)(fn_1_4AE0(&lbl_1_rodata_264) & 0x3F) - 31.0f));
        p->pos.y = 100.0f * (0.01f * ((f64)(fn_1_4AE0(&lbl_1_rodata_264) & 0x3F) - 31.0f));
        p->pos.z = 0.0f;
        p->color.a = 0;
        p->time = 0;
        p->parManId = (fn_1_4AE0(&lbl_1_rodata_264) & 0xF) + 0xF;
        p->zRot = 0.017453292f * fn_1_4FC8(&lbl_1_rodata_258, 0.0f, 360.0f);
    }
}

// NONMATCHING: register allocation only.
void fn_1_CE40(M539Effect *self, M539Particle *particleP)
{
    M539ParticleData *p;
    s16 i;
    u8 alive = 1;

    if (self->flag == 0) {
        for (p = particleP->data, i = 0; i < particleP->maxCnt; i++, p++) {
            if (p->time != 2) {
                if (p->time == 0) {
                    p->color.a = p->color.a + p->parManId;
                    if (p->color.a >= 0xDC) {
                        p->color.a = 0xDC;
                        p->time = p->time + 1;
                    }
                }
                else if (p->color.a > 0x10) {
                    p->color.a = p->color.a - 0x10;
                }
                else {
                    p->color.a = 0;
                    p->time = p->time + 1;
                }
                p->scale = (f32)((f64)p->scale + 2.5);
                p->pos.x = (f32)((f64)p->pos.x + (0.016666666666666666 * (100.0f * (1.5f * p->vel.x))));
                p->pos.y = (f32)((f64)p->pos.y + (0.016666666666666666 * (100.0f * (1.5f * p->vel.y))));
                alive = 0;
            }
        }
        DCStoreRangeNoSync(particleP->data, particleP->maxCnt * sizeof(M539ParticleData));
        self->flag = alive;
        if (self->flag != 0) {
            Hu3DModelAttrSet(self->modelId, 1);
        }
    }
}

}
