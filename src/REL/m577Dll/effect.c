#define _MATH_H /* suppress MSL math.h's extern inline sqrtf body and its two local statics */
#include "game/hu3d.h"
#include "game/sprite.h"
#include "game/frand.h"

typedef struct EffectHookData_s {
    s16 unk0;
    s16 unk2;
    u8 unk4[0x18];
    struct EffectData_s *owner; /* 0x1C */
    u8 unk20[0x10];
    s16 maxCnt; /* 0x30 */
    u8 unk32[0x16];
    HU3DPARTICLEDATA *data; /* 0x48 */
} EffectHookData;

typedef struct EffectData_s {
    EffectHookData *hook; /* 0x00 */
    ANIMDATA *anim;       /* 0x04 */
    s32 arg;              /* 0x08 */
    HU3DMODELID modelId;  /* 0x0C */
} EffectData;

typedef struct EffectWork_s {
    u32 unk0;
    u32 unk4;
    HuVecF *pos; /* 0x08 */
} EffectWork;

void fn_1_B730(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);

void fn_1_B5D4(EffectData *arg0, u16 arg1, s32 arg2)
{
    ANIMDATA *anim;
    HU3DMODELID modelId;
    HU3DPARTICLEDATA *part;
    s32 count;
    s32 i;

    anim = HuSprAnimRead(HuDataSelHeapReadNum(0x84001F, HU_MEMNUM_OVL, HEAP_MODEL));
    modelId = Hu3DParticleCreate(anim, 0x1F);
    Hu3DModelLayerSet(modelId, 4);
    Hu3DParticleHookSet(modelId, fn_1_B730);
    Hu3DModelCameraSet(modelId, arg1);
    Hu3DParticleBlendModeSet(modelId, 0);
    Hu3DParticleColSet(modelId, 0xFF, 0xFF, 0xFF);
    Hu3DParticleScaleSet(modelId, 0.0f);
    arg0->modelId = modelId;
    arg0->anim = anim;
    arg0->hook = Hu3DData[modelId].hookData;
    arg0->hook->unk2 = 0;
    arg0->hook->owner = arg0;
    part = arg0->hook->data;
    count = arg0->hook->maxCnt;
    for (i = 0; i < count; i++, part++) {
        part->scale = 0.0f;
        part->time = 0;
        part->parManId = 0;
    }
    arg0->arg = arg2;
}

void fn_1_B71C(void)
{
}

void fn_1_B720(EffectData *arg0, s32 arg1)
{
    arg0->hook->unk2 = arg1;
}

void fn_1_B730(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx)
{
    HU3DPARTICLEDATA *part;
    EffectWork *work;
    f32 rate;
    f32 fade;
    s8 sp8;
    s32 count;
    s32 i;

    if (particleP->emitCnt != 0) {
        part = particleP->data;
        count = particleP->maxCnt;
        for (i = 0; i < count; i++, part++) {
            if (part->parManId == 0) {
                work = particleP->work;
                part->parManId = 1;
                part->pos = *work->pos;
                part->pos.x += 100.0f * ((f32)(s32)frandmod(0x2711) / 10000.0f - 0.5f);
                part->pos.y += 30.0f;
                part->pos.z -= 3.0f;
                part->color.r = 0xFF;
                part->color.g = 0xFF;
                part->color.b = 0xFF;
                part->color.a = 0xFF;
                break;
            }
        }
    }
    part = particleP->data;
    count = particleP->maxCnt;
    for (i = 0; i < count; i++, part++) {
        if (part->parManId != 0) {
            part->time++;
            rate = (f32)part->time / 30.0f;
            part->scale = 150.0f * rate;
            part->pos.z += 6.0f;
            fade = 255.0f * (1.0f - rate);
            sp8 = fade;
            part->color.a = fade;
            if (part->time == 0x1E) {
                part->scale = 0.0f;
                part->time = 0;
                part->parManId = 0;
            }
        }
    }
}