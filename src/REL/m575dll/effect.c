// REL/m575dll/effect.c - the small sprite-particle effect (dust puffs) shared by
// every camera of the game.
#include "REL/m575dll.h"
#include "game/hu3d.h"
#include "game/sprite.h"
#include "game/data.h"
#include "game/memory.h"
#include "game/frand.h"
#include "string.h"
#include "math.h"
#include "humath.h"

// Per-particle template: copied into the particle's work slot when it is emitted
// (0x38 bytes) and read back every frame by the hook.
typedef struct M575EffTpl_s {
    u32 flags;
    GXColor colStart;
    GXColor colEnd;
    Vec vel;
    Vec velMul;
    f32 gravity;
    f32 unk28;
    f32 scaleDelta;
    f32 alphaDelta;
    f32 colStep;
} M575EffTpl;

void fn_1_EE10(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);
s16 fn_1_E6E4(s16 camMask, M575EffTpl *tpl, f32 x, f32 y, f32 z, f32 scale);
void fn_1_E988(void);

M575EffTpl lbl_1_data_3A0 = {
    0,
    { 0xA0, 0xA0, 0xA0, 0xB0 },
    { 0x00, 0x00, 0x00, 0xFF },
    { 0.0f, 2.0f, 1.0f },
    { 0.95f, 0.99f, 0.95f },
    0.0f,
    0.0f,
    1.0f,
    -4.0f,
    0.0f,
};

s16 lbl_1_bss_7B98[0x10];
void *lbl_1_bss_7BB8;

s32 fn_1_E424(M575Game *game)
{
    s32 ret = 0;

    switch (game->phase) {
    case 0:
        game->phase++;
        break;
    case 1:
        game->phase = 0;
        ret = 1;
        break;
    }
    return ret;
}

void fn_1_E478(void)
{
    s32 i;

    for (i = 0; i < 0x10; i++) {
        lbl_1_bss_7B98[i] = -1;
    }
    lbl_1_bss_7BB8 = NULL;
}

void fn_1_E4C8(void)
{
    s32 i;

    if (lbl_1_bss_7BB8 != NULL) {
        HuMemDirectFree(lbl_1_bss_7BB8);
        lbl_1_bss_7BB8 = NULL;
    }
    for (i = 0; i < 0x10; i++) {
        lbl_1_bss_7B98[i] = -1;
    }
    lbl_1_bss_7BB8 = NULL;
}

// Emits one puff at `pos` (y scaled by 3) flying off in a random horizontal direction.
void fn_1_E55C(s16 camMask, Vec *pos)
{
    f32 scale = 3.0f;
    f32 ang;

    ang = ((f32)(frand() % 10000) / 10000.0f) * 360.0f;
    lbl_1_data_3A0.vel.x = (f32)(scale * (4.0 * HuSin(ang)));
    lbl_1_data_3A0.vel.y = (f32)(scale * (4.0 * HuCos(ang)));
    lbl_1_data_3A0.vel.z = 0.0f;
    fn_1_E6E4(camMask, &lbl_1_data_3A0, pos->x, pos->y * scale, pos->z, 300.0f);
}

s16 fn_1_E6E4(s16 camMask, M575EffTpl *tpl, f32 x, f32 y, f32 z, f32 scale)
{
    HU3DPARTICLE *particle;
    HU3DPARTICLEDATA *data;
    M575EffTpl *work;
    s16 result;
    s16 cam;
    s16 bit;
    s16 idx;
    s16 id;

    fn_1_E988();
    result = -1;
    cam = 0;
    bit = 1;
    while (cam < 0x10) {
        if ((bit & camMask) != 0 && Hu3DCamera[cam].fov != -1.0f) {
            id = lbl_1_bss_7B98[cam];
            if (id == -1) {
                return -1;
            }
            particle = (HU3DPARTICLE *)Hu3DData[id].hookData;
            data = &particle->data[particle->emitCnt];
            idx = particle->emitCnt;
            while (idx < particle->maxCnt && data->scale != 0.0f) {
                idx++;
                data++;
            }
            if (idx >= particle->maxCnt) {
                data = particle->data;
                idx = 0;
                while (idx < particle->maxCnt && data->scale != 0.0f) {
                    idx++;
                    data++;
                }
            }
            if (idx != particle->maxCnt) {
                work = (M575EffTpl *)particle->work;
                work[idx] = *tpl;
                data->cameraBit = camMask;
                data->pos.x = x;
                data->pos.y = y;
                data->pos.z = z;
                data->vel.x = tpl->vel.x;
                data->vel.y = tpl->vel.y;
                data->vel.z = tpl->vel.z;
                data->color.r = tpl->colStart.r;
                data->color.g = tpl->colStart.g;
                data->color.b = tpl->colStart.b;
                data->color.a = tpl->colStart.a;
                data->scaleBase = scale;
                data->scale = scale;
                data->time = 0;
                data->parManId = -1;
                particle->emitCnt = idx;
                result = idx;
            }
        }
        cam++;
        bit = bit * 2;
    }
    return result;
}

// Makes sure every active camera owns a particle system.
void fn_1_E988(void)
{
    ANIMDATA *anim = NULL;
    HU3DPARTICLE *particle;
    HU3DPARTICLEDATA *data;
    s16 i;
    s16 j;

    for (i = 0; i < 0x10; i++) {
        if (Hu3DCamera[i].fov != -1.0f) {
            if (lbl_1_bss_7B98[i] != -1) {
                particle = (HU3DPARTICLE *)Hu3DData[lbl_1_bss_7B98[i]].hookData;
                if (particle->anim != NULL) {
                    anim = particle->anim;
                }
            }
            else {
                if (anim == NULL) {
                    anim = HuSprAnimRead(HuDataSelHeapReadNum(0x82001A, HU_MEMNUM_OVL, HEAP_MODEL));
                }
                lbl_1_bss_7B98[i] = Hu3DParticleCreate(anim, 0x96);
                Hu3DModelLayerSet(lbl_1_bss_7B98[i], 4);
                Hu3DParticleHookSet(lbl_1_bss_7B98[i], fn_1_EE10);
                Hu3DModelCameraSet(lbl_1_bss_7B98[i], 1 << i);
                if (lbl_1_bss_7BB8 == NULL) {
                    lbl_1_bss_7BB8 = HuMemDirectMallocNum(HEAP_HEAP, 150 * sizeof(M575EffTpl), HU_MEMNUM_OVL);
                }
                Hu3DParticleBlendModeSet(lbl_1_bss_7B98[i], 0);
                particle = (HU3DPARTICLE *)Hu3DData[lbl_1_bss_7B98[i]].hookData;
                particle->emitCnt = 0;
                particle->work = lbl_1_bss_7BB8;
                particle->count = 1;
                data = particle->data;
                for (j = 0; j < particle->maxCnt; j++) {
                    data->scale = 0.0f;
                    data++;
                }
            }
        }
    }
}

// Creates the particle systems of the cameras that exist so far.
void fn_1_EBEC(void)
{
    ANIMDATA *anim = NULL;
    HU3DPARTICLE *particle;
    HU3DPARTICLEDATA *data;
    s16 i;
    s16 j;

    for (i = 0; i < 0x10; i++) {
        if (Hu3DCamera[i].fov != -1.0f && lbl_1_bss_7B98[i] == -1) {
            if (anim == NULL) {
                anim = HuSprAnimRead(HuDataSelHeapReadNum(0x82001A, HU_MEMNUM_OVL, HEAP_MODEL));
            }
            lbl_1_bss_7B98[i] = Hu3DParticleCreate(anim, 0x96);
            Hu3DModelLayerSet(lbl_1_bss_7B98[i], 4);
            Hu3DParticleHookSet(lbl_1_bss_7B98[i], fn_1_EE10);
            Hu3DModelCameraSet(lbl_1_bss_7B98[i], 1 << i);
            if (lbl_1_bss_7BB8 == NULL) {
                lbl_1_bss_7BB8 = HuMemDirectMallocNum(HEAP_HEAP, 150 * sizeof(M575EffTpl), HU_MEMNUM_OVL);
            }
            Hu3DParticleBlendModeSet(lbl_1_bss_7B98[i], 0);
            particle = (HU3DPARTICLE *)Hu3DData[lbl_1_bss_7B98[i]].hookData;
            particle->emitCnt = 0;
            particle->work = lbl_1_bss_7BB8;
            particle->count = 1;
            data = particle->data;
            for (j = 0; j < particle->maxCnt; j++) {
                data->scale = 0.0f;
                data++;
            }
        }
    }
}

void fn_1_EE10(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx)
{
    M575EffTpl *work = (M575EffTpl *)particleP->work;
    HU3DPARTICLEDATA *data;
    f64 flick;
    s16 i;
    s16 v;
    s16 a;

    if (particleP->count == 0) {
        data = particleP->data;
        for (i = 0; i < particleP->maxCnt; i++) {
            data->scale = 0.0f;
            data++;
        }
    }
    data = particleP->data;
    for (i = 0; i < particleP->maxCnt; i++) {
        if (data->scale != 0.0f) {
            data->vel.x *= work[i].velMul.x;
            data->vel.y *= work[i].velMul.y;
            data->vel.z *= work[i].velMul.z;
            PSVECAdd(&data->vel, &data->pos, &data->pos);
            data->vel.y += work[i].gravity;
            v = (s16)((f32)data->color.r + work[i].colStep * (f32)(work[i].colEnd.r - work[i].colStart.r));
            if (v < 0) {
                v = 0;
            }
            else if (v > 0xFF) {
                v = 0xFF;
            }
            data->color.r = v;
            v = (s16)((f32)data->color.g + work[i].colStep * (f32)(work[i].colEnd.g - work[i].colStart.g));
            if (v < 0) {
                v = 0;
            }
            else if (v > 0xFF) {
                v = 0xFF;
            }
            data->color.g = v;
            v = (s16)((f32)data->color.b + work[i].colStep * (f32)(work[i].colEnd.b - work[i].colStart.b));
            if (v < 0) {
                v = 0;
            }
            else if (v > 0xFF) {
                v = 0xFF;
            }
            data->color.b = v;
            a = (s16)((f32)data->color.a + work[i].alphaDelta);
            if (a < 1) {
                data->scale = 0.0f;
            }
            data->color.a = a;
            if (data->scale != 0.0f) {
                if ((work[i].flags & 1) != 0) {
                    if (((data->time + i) & 1) != 0) {
                        flick = 1.0;
                    }
                    else {
                        flick = 0.5;
                    }
                    data->scale = (f32)((f64)data->scaleBase * flick);
                }
                else {
                    data->scale = data->scaleBase;
                }
                data->scaleBase += work[i].scaleDelta;
                if (data->scaleBase <= 0.01f) {
                    data->scale = 0.0f;
                }
            }
            data->time++;
        }
        data++;
    }
    DCStoreRangeNoSync(particleP->data, particleP->maxCnt * sizeof(HU3DPARTICLEDATA));
}
