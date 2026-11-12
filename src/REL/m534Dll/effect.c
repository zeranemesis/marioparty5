#include "REL/m534Dll.h"

static void fn_1_633C(void);
static void fn_1_64EC(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);
static void fn_1_681C(void);
static void fn_1_69E4(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);

s16 lbl_1_bss_50;
s16 lbl_1_bss_52;

void fn_1_6314(void)
{
    fn_1_633C();
    fn_1_681C();
}

void fn_1_6338(void)
{
}

static void fn_1_633C(void)
{
    HU3DPARTICLEDATA *data;
    HU3DMODELID model;
    HU3DPARTICLE *particle;
    s32 i;
    ANIMDATA *anim;

    anim = HuSprAnimRead(HuDataSelHeapReadNum(0x5C000C, 0x10000000, HEAP_MODEL));
    model = Hu3DParticleCreate(anim, 0x80);
    lbl_1_bss_52 = model;
    particle = Hu3DData[model].hookData;
    Hu3DParticleColSet(model, 0xDA, 0xCB, 0x9E);
    Hu3DParticleBlendModeSet(model, 0);
    Hu3DParticleHookSet(model, fn_1_64EC);
    Hu3DModelLayerSet(model, 6);
    Hu3DModelPosSet(model, 0.0f, 0.0f, 0.0f);
    data = particle->data;
    for (i = 0; i < particle->maxCnt; i++, data++) {
        data->time = -1;
        data->scale = 0.0f;
    }
}

void fn_1_6458(HuVecF *pos)
{
    HU3DPARTICLE *particle;
    HU3DPARTICLEDATA *data;
    s32 i;

    particle = Hu3DData[lbl_1_bss_52].hookData;
    data = particle->data;
    for (i = 0; i < particle->maxCnt; i++, data++) {
        if (data->time < 0) {
            data->time = 0;
            data->pos = *pos;
            return;
        }
    }
}

static void fn_1_64EC(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx)
{
    HU3DPARTICLEDATA *data = particleP->data;
    s32 i;

    for (i = 0; i < particleP->maxCnt; i++, data++) {
        switch (data->time) {
            case 0:
                data->time++;
                data->accel.x = 0.8 * (3.0517578125e-05 * (f32) ((s32) (frand() & 0xFFFF) - 0x8000));
                data->accel.y = 0.1 + 0.2 * (1.52587890625e-05 * (f32) (s32) (frand() & 0xFFFF));
                data->accel.z = 0.8 * (3.0517578125e-05 * (f32) ((s32) (frand() & 0xFFFF) - 0x8000));
                data->scale = 10.0 + 40.0 * (1.52587890625e-05 * (f32) (s32) (frand() & 0xFFFF));
                data->zRot = 0.0f;
                data->color.a = 0xB4;
            case 1:
                data->scale += 8.0f;
                data->zRot += 0.008726646192371845;
                data->color.a -= 2;
                if (data->pos.y < -200.0 || data->scale <= 0.0f || data->color.a > 0xF0) {
                    data->time = -1;
                    data->scale = 0.0f;
                } else {
                    data->pos.x += data->accel.x;
                    data->pos.y += data->accel.y;
                    data->pos.z += data->accel.z;
                    data->accel.x *= 0.9;
                    data->accel.y -= 0.05;
                    data->accel.z *= 0.9;
                }
                break;
        }
    }
}

static void fn_1_681C(void)
{
    HU3DPARTICLEDATA *data;
    HU3DMODELID model;
    HU3DPARTICLE *particle;
    s32 i;
    ANIMDATA *anim;

    anim = HuSprAnimRead(HuDataSelHeapReadNum(0x5C000D, 0x10000000, HEAP_MODEL));
    model = Hu3DParticleCreate(anim, 0x200);
    lbl_1_bss_50 = model;
    particle = Hu3DData[model].hookData;
    Hu3DParticleColSet(model, 0xDA, 0xCB, 0x9E);
    Hu3DParticleBlendModeSet(model, 0);
    Hu3DParticleHookSet(model, fn_1_69E4);
    Hu3DModelLayerSet(model, 0);
    Hu3DModelPosSet(model, 0.0f, 0.0f, 0.0f);
    data = particle->data;
    for (i = 0; i < particle->maxCnt; i++, data++) {
        data->time = -1;
        data->scale = 0.0f;
    }
}

void fn_1_6938(HuVecF *pos, HuVecF *vel)
{
    HU3DPARTICLE *particle;
    HU3DPARTICLEDATA *data;
    s32 i;

    particle = Hu3DData[lbl_1_bss_50].hookData;
    data = particle->data;
    for (i = 0; i < particle->maxCnt; i++, data++) {
        if (data->time < 0) {
            data->time = 0;
            data->pos = *pos;
            data->accel = *vel;
            return;
        }
    }
}

static void fn_1_69E4(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx)
{
    HU3DPARTICLEDATA *data = particleP->data;
    s32 i;

    for (i = 0; i < particleP->maxCnt; i++, data++) {
        switch (data->time) {
            case 0:
                data->time++;
                data->scale = 5.0 + 5.0 * (1.52587890625e-05 * (f32) (s32) (frand() & 0xFFFF));
                data->zRot = 0.0f;
                data->color.a = 0x32;
            case 1:
                data->scale += 0.1;
                if (data->pos.y < -100.0 || data->scale <= 0.0f || data->color.a > 0xF0) {
                    data->time = -1;
                    data->scale = 0.0f;
                } else {
                    data->pos.x += data->accel.x;
                    data->pos.y += data->accel.y;
                    data->pos.z += data->accel.z;
                    data->accel.x *= 0.88;
                    data->accel.y -= 0.15;
                    data->accel.z *= 0.88;
                }
                break;
        }
    }
}
