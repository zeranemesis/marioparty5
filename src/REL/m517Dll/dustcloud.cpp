// m517Dll dustcloud: two dust puff particle tasks. Each fans its particles out
// in a ring, fades them in and out while they drift up, and deletes itself once
// every particle has faded.
#include "REL/m517Dll.h"
extern "C" {
#include "game/frand.h"
#include <math.h>
}

extern "C" {

void fn_1_1A4D4(HU3DMODEL *model, HU3DPARTICLE *particle, Mtx mtx);
M517DustCloud *fn_1_1A678(M517DustCloud *self, s16 flag);
void fn_1_1A714(M517DustCloud *self, s32 bit);
void fn_1_1A72C(M517DustCloud *self, Mtx mtx);
void fn_1_1AC90(HU3DMODEL *model, HU3DPARTICLE *particle, Mtx mtx);
M517DustCloud *fn_1_1AE48(M517DustCloud *self, s16 flag);
void fn_1_1AEE4(M517DustCloud *self, s32 bit);
void fn_1_1AEFC(M517DustCloud *self, HU3DPARTICLE *particle);
void fn_1_1B220(M517DustCloud *self, Mtx mtx);

void *lbl_1_data_858[5] = { 0, 0, (void *)fn_1_1AE48, (void *)fn_1_1AEE4, (void *)fn_1_1E80 };
void *lbl_1_data_86C[5] = { 0, 0, (void *)fn_1_1A678, (void *)fn_1_1A714, (void *)fn_1_1E80 };

// Start particle i of n: a ring direction, a random lift (lift = gscale * rand) and a random ground offset.
static inline void M517DustInit(HU3DPARTICLEDATA *p, s32 i, s32 n, f32 lift, f32 scale)
{
    p->vel.x = sin(0.017453292f * ((360.0f * (f32)i) / (f32)n));
    p->accel.y = p->vel.y = 0.27240694444444447 * (10.0f + (lift * (0.000015258789f * (f32)(u16)frand())));
    p->vel.z = cos(0.017453292f * ((360.0f * (f32)i) / (f32)n));
    p->scale = scale;
    p->pos.x = 100.0f * (0.01f * ((f32)(s32)(fn_1_6598() & 0x3F) - 31.0f));
    p->pos.y = 0.0f;
    p->pos.z = 100.0f * (0.01f * ((f32)(s32)(fn_1_6598() & 0x3F) - 31.0f));
    p->color.a = 0;
    p->time = 0;
    p->parManId = (fn_1_6598() & 0xF) + 0xF;
    p->zRot = 0.017453292f * (360.0f * (0.000015258789f * (f32)(u16)frand()));
}

// Fade the particles in then out, move them and report whether all are finished.
static inline s32 M517DustStep(HU3DPARTICLE *particle)
{
    HU3DPARTICLEDATA *p;
    s32 i;
    s32 done = 1;

    p = particle->data;
    for (i = 0; i < (s16)particle->maxCnt; i++, p++) {
        if ((s16)p->time != 2) {
            f32 prev;
            f32 lift;

            if ((s16)p->time == 0) {
                p->color.a = p->color.a + p->parManId;
                if (p->color.a >= 0xDC) {
                    p->color.a = 0xDC;
                    p->time++;
                }
            } else if (p->color.a > 0x20) {
                p->color.a = p->color.a - (s32)(0.5f * (f32)p->parManId);
            } else {
                p->time++;
            }
            p->scale = p->scale + 3.3333333333333335;
            prev = p->vel.y;
            p->vel.y = p->vel.y + -0.27240694444444447;
            lift = 0.5f * (prev + p->vel.y);
            p->pos.x = p->pos.x + (0.016666666666666666 * (100.0f * (2.0f * p->vel.x)));
            p->pos.y = p->pos.y + lift;
            p->pos.z = p->pos.z + (0.016666666666666666 * (100.0f * (2.0f * p->vel.z)));
            done = 0;
        }
    }
    return done;
}

// Particle hook: update the owning task.
void fn_1_1A4D4(HU3DMODEL *model, HU3DPARTICLE *particle, Mtx mtx)
{
    MtxPtr sp8 = mtx;

    fn_1_1A72C((M517DustCloud *)particle->work, sp8);
}

M517DustCloud *fn_1_1A518(M517DustCloud *self, Vec *pos, GXColor *col, s32 num)
{
    HU3DMODELID id;

    fn_1_2DA8(&self->task.link);
    self->task.vtbl = &lbl_1_data_68.vtbl;
    self->task.flags = 0;
    fn_1_2E74(&self->task.link, &lbl_1_bss_24);
    self->task.vtbl = (M517TaskVtbl *)lbl_1_data_86C;
    self->anim = HuSprAnimRead(HuDataSelHeapReadNum(0x4B0011, HU_MEMNUM_OVL, HEAP_MODEL));
    HuSprAnimLock(self->anim);
    id = Hu3DParticleCreate(self->anim, (s16)num);
    self->particle = (HU3DPARTICLE *)Hu3DData[id].hookData;
    self->particle->work = self;
    Hu3DParticleHookSet(id, fn_1_1A4D4);
    Hu3DParticleColSet(id, col->r, col->g, col->b);
    Hu3DParticleBlendModeSet(id, 0);
    Hu3DModelLayerSet(id, 5);
    Hu3DModelPosSet(id, pos->x, pos->y, pos->z);
    Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
    self->modelId = id;
    return self;
}

M517DustCloud *fn_1_1A678(M517DustCloud *self, s16 flag)
{
    if (self != 0) {
        self->task.vtbl = (M517TaskVtbl *)lbl_1_data_86C;
        self->particle->work = 0;
        Hu3DModelKill(self->modelId);
        HuSprAnimKill(self->anim);
        if (self != 0) {
            self->task.vtbl = &lbl_1_data_68.vtbl;
            if (self != 0) {
                fn_1_2DB4(&self->task.link, 0);
            }
        }
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

void fn_1_1A714(M517DustCloud *self, s32 bit)
{
    self->task.flags &= ~(1 << bit);
}

void fn_1_1A72C(M517DustCloud *self, Mtx mtx)
{
    HU3DMODEL *model;
    HU3DPARTICLE *particle;
    HU3DPARTICLEDATA *p;
    s32 i;

    model = &Hu3DData[self->modelId];
    particle = self->particle;
    if (particle->count == 0) {
        p = particle->data;
        for (i = 0; i < (s16)particle->maxCnt; i++, p++) {
            M517DustInit(p, i, (s16)particle->maxCnt, 30.0f, 200.0f);
        }
    }
    if (M517DustStep(particle) != 0) {
        if (self != 0) {
            self->task.vtbl->dtor(&self->task, 1);
        }
    }
}

// Particle hook of the six-particle cloud.
void fn_1_1AC90(HU3DMODEL *model, HU3DPARTICLE *particle, Mtx mtx)
{
    MtxPtr sp8 = mtx;

    fn_1_1B220((M517DustCloud *)particle->work, sp8);
}

M517DustCloud *fn_1_1ACD4(M517DustCloud *self, Vec *pos)
{
    HU3DMODELID id;

    fn_1_2DA8(&self->task.link);
    self->task.vtbl = &lbl_1_data_68.vtbl;
    self->task.flags = 0;
    fn_1_2E74(&self->task.link, &lbl_1_bss_24);
    self->task.vtbl = (M517TaskVtbl *)lbl_1_data_858;
    self->anim = HuSprAnimRead(HuDataSelHeapReadNum(0x4B0011, HU_MEMNUM_OVL, HEAP_MODEL));
    HuSprAnimLock(self->anim);
    id = Hu3DParticleCreate(self->anim, 6);
    self->particle = (HU3DPARTICLE *)Hu3DData[id].hookData;
    self->particle->work = self;
    Hu3DParticleHookSet(id, fn_1_1AC90);
    Hu3DParticleColSet(id, 0xF0, 0xE0, 0x80);
    Hu3DParticleTPLvlSet(id, 0.0f);
    Hu3DParticleBlendModeSet(id, 0);
    Hu3DModelLayerSet(id, 5);
    Hu3DModelPosSet(id, pos->x, pos->y, pos->z);
    Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
    self->modelId = id;
    fn_1_1AEFC(self, self->particle);
    return self;
}

M517DustCloud *fn_1_1AE48(M517DustCloud *self, s16 flag)
{
    if (self != 0) {
        self->task.vtbl = (M517TaskVtbl *)lbl_1_data_858;
        self->particle->work = 0;
        Hu3DModelKill(self->modelId);
        HuSprAnimKill(self->anim);
        if (self != 0) {
            self->task.vtbl = &lbl_1_data_68.vtbl;
            if (self != 0) {
                fn_1_2DB4(&self->task.link, 0);
            }
        }
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

void fn_1_1AEE4(M517DustCloud *self, s32 bit)
{
    self->task.flags &= ~(1 << bit);
}

// Start every particle of the six-particle cloud.
void fn_1_1AEFC(M517DustCloud *self, HU3DPARTICLE *particle)
{
    HU3DPARTICLEDATA *p;
    s32 i;

    p = particle->data;
    for (i = 0; i < (s16)particle->maxCnt; i++, p++) {
        M517DustInit(p, i, (s16)particle->maxCnt, 15.0f, 50.0f);
    }
}

void fn_1_1B220(M517DustCloud *self, Mtx mtx)
{
    HU3DMODEL *model;
    HU3DPARTICLE *particle;

    model = &Hu3DData[self->modelId];
    particle = self->particle;
    if (M517DustStep(particle) != 0) {
        if (self != 0) {
            self->task.vtbl->dtor(&self->task, 1);
        }
    }
}

} // extern "C"
