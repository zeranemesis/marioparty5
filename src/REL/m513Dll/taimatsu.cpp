// m513Dll taimatsu: the torch flame, a particle effect task.
#include "REL/m513Dll.h"
extern "C" {
#include "game/frand.h"
}

extern "C" {
void DCStoreRangeNoSync(void *addr, u32 nBytes);
}

typedef struct M513Torch_s {
    M513Node link;
    void **vtbl;
    s32 flags;
    s16 modelId;
    HU3DPARTICLE *particle;
    f32 time;
    s32 count;
    s32 emit;
} M513Torch;

extern "C" {

void fn_1_6D0C(HU3DMODEL *model, HU3DPARTICLE *particle, Mtx mtx);
M513Torch *fn_1_6F78(M513Torch *self, s16 flag);
void fn_1_700C(M513Torch *self, s32 bit);
void fn_1_7024(M513Torch *self);

char lbl_1_data_288[] = "ms_pointer != 0";
char lbl_1_data_298[] = "singleton.h";
void *lbl_1_data_2A4[5] = { 0, 0, (void *)fn_1_6F78, (void *)fn_1_700C, (void *)fn_1_7024 };

// Restart one flame particle.
#define M513_TORCH_INIT(p)                                                                         \
    (p)->vel.x = 0.0f;                                                                             \
    (p)->vel.y = 0.0f;                                                                             \
    (p)->vel.z = 0.0f;                                                                             \
    (p)->scale = 50.0f;                                                                            \
    (p)->pos.x = 20.0f * (0.000030517578f * ((f32)(u16)frand() - 32768.0f));                            \
    (p)->pos.y = 0.5f * (p)->scale;                                                                \
    (p)->pos.z = 20.0f * (0.000030517578f * ((f32)(u16)frand() - 32768.0f));                            \
    (p)->color.r = 0xE0;                                                                           \
    (p)->color.g = 0x60;                                                                           \
    (p)->color.b = 0x30;                                                                           \
    (p)->color.a = 0;                                                                              \
    (p)->time = 0;                                                                                 \
    (p)->parManId = 0.5f * (7.0f + (15.0f * (0.000015258789f * (f32)(u16)frand())));                    \
    (p)->zRot = 0.017453292f * (360.0f * (0.000015258789f * (f32)(u16)frand()))

M513Torch *fn_1_6D50(M513Torch *self, Vec *pos)
{
    HU3DMODELID id;
    HU3DPARTICLEDATA *p;
    s32 i;

    fn_1_1768(&self->link);
    self->vtbl = (void **)&lbl_1_data_218;
    self->flags = 0;
    fn_1_1834(&self->link, &lbl_1_bss_C);
    self->vtbl = lbl_1_data_2A4;
    self->time = 0.0f;
    self->count = 0;
    self->emit = 1;
    M513_ASSERT(lbl_1_bss_9F0 != 0, lbl_1_data_288, lbl_1_data_298, 0x27);
    id = Hu3DParticleCreate(lbl_1_bss_9F0->subAnim[2], 0x10);
    self->particle = (HU3DPARTICLE *)Hu3DData[id].hookData;
    self->particle->work = self;
    Hu3DParticleHookSet(id, fn_1_6D0C);
    Hu3DParticleColSet(id, 0xE0, 0x60, 0x30);
    Hu3DParticleBlendModeSet(id, 1);
    Hu3DModelLayerSet(id, 5);
    Hu3DModelPosSet(id, pos->x, pos->y, pos->z);
    Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
    self->modelId = id;
    p = self->particle->data;
    for (i = 0; i < (s16)self->particle->maxCnt; i++, p++) {
        p->time = 0;
    }
    return self;
}

M513Torch *fn_1_6F20(M513Torch *self, s16 flag)
{
    if (self != 0) {
        fn_1_1774(&self->link, 0);
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M513Torch *fn_1_6F78(M513Torch *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = lbl_1_data_2A4;
        self->particle->work = 0;
        Hu3DModelKill(self->modelId);
        if (self != 0) {
            self->vtbl = (void **)&lbl_1_data_218;
            if (self != 0) {
                fn_1_1774(&self->link, 0);
            }
        }
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_700C(M513Torch *self, s32 bit)
{
    self->flags &= ~(1 << bit);
}

void fn_1_7024(M513Torch *self)
{
    self->time += 0.016666666666666666;
    if (self->emit < 0x10) {
        self->count++;
        if ((self->count & 3) == 0) {
            self->emit++;
        }
    }
}

void fn_1_7078(void *unused, HU3DPARTICLEDATA *p)
{
    M513_TORCH_INIT(p);
}

// Hook: advance every live flame particle.
void fn_1_72AC(M513Torch *self, Mtx mtx)
{
    HU3DMODEL *model = &Hu3DData[self->modelId];
    HU3DPARTICLE *particle = self->particle;
    HU3DPARTICLEDATA *p = particle->data;
    s32 i;
    f32 prev;

    for (i = 0; i < self->emit; i++, p++) {
        if (p->time <= 2) {
            switch (p->time) {
                case 0:
                    M513_TORCH_INIT(p);
                    p->time++;
                    // fall through
                case 1:
                    p->color.a = p->color.a + p->parManId;
                    if (p->color.a >= 0xE6) {
                        p->color.a = 0xFF;
                        p->time++;
                    }
                    break;
                case 2:
                    p->color.a = p->color.a - p->parManId;
                    if (p->color.a <= 0xF) {
                        p->color.a = 0;
                        p->time = 0;
                    }
                    break;
            }
            p->scale -= 0.16666666666666666;
            if (p->scale < 0.0f) {
                p->scale = 0.0f;
            }
            prev = p->vel.y;
            p->vel.y += 0.027240694850362426;
            if (p->vel.y > 0.5448138888888889) {
                p->vel.y = 0.5448139f;
            }
            p->pos.y = p->pos.y + (0.5f * (prev + p->vel.y));
            p->pos.x = p->pos.x * 0.95f;
            p->pos.z = p->pos.z * 0.95f;
            p->color.r = (u8)(s32)((f32)p->color.r * 0.999f);
            p->color.g = (u8)(s32)((f32)p->color.g * 0.999f);
            p->color.b = (u8)(s32)((f32)p->color.b * 0.999f);
        }
    }
    DCStoreRangeNoSync(particle->data, particle->maxCnt * sizeof(HU3DPARTICLEDATA));
}

}
