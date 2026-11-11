#include "game/data.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/sprite.h"

/* One free-slot record of the shared 0x20D0 block (stride 0x38). */
typedef struct M560EffectParam_s {
    s32 flags;
    GXColor colorStart;
    GXColor colorEnd;
    HuVecF vel;
    HuVecF speedDecay;
    f32 accelY;
    f32 unk28;
    f32 scaleDecay;
    f32 alphaStep;
    f32 colorStep;
} M560EffectParam;

s16 lbl_1_bss_520[16];
void *lbl_1_bss_540;

M560EffectParam lbl_1_data_2F0 = {
    0,
    { 0xA0, 0xA0, 0xA0, 0xFF },
    { 0x00, 0x00, 0x00, 0xFF },
    { 0.0f, 2.0f, 1.0f },
    { 0.95f, 0.99f, 0.95f },
    0.0f,
    0.0f,
    1.5f,
    -4.0f,
    0.0f,
};

s16 fn_1_6B00(s16 arg0, M560EffectParam *arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5);
void fn_1_6DA4(void);
void fn_1_722C(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);

void fn_1_668C(void)
{
    s32 i;

    for (i = 0; i < 16; i++) {
        lbl_1_bss_520[i] = -1;
    }
    lbl_1_bss_540 = NULL;
}

void fn_1_66DC(void)
{
    s32 i;

    if (lbl_1_bss_540) {
        HuMemDirectFree(lbl_1_bss_540);
        lbl_1_bss_540 = NULL;
    }

    for (i = 0; i < 16; i++) {
        lbl_1_bss_520[i] = -1;
    }
    lbl_1_bss_540 = NULL;
}

void fn_1_6770(s16 arg0, Vec *arg1)
{
    s16 i;
    f32 scale;

    scale = 2.0f;

    lbl_1_data_2F0.colorStart.a = 0x80;

    for (i = 0; i < 8; i++) {
        lbl_1_data_2F0.vel.x =
            (f32)(scale * (3.5 * sin((3.141592653589793 * (270.0f + 45.0f * i)) / 180.0)));
        lbl_1_data_2F0.vel.y = 1.0f;
        lbl_1_data_2F0.vel.z =
            (f32)(scale * (3.5 * cos((3.141592653589793 * (270.0f + 45.0f * i)) / 180.0)));
        fn_1_6B00(arg0, &lbl_1_data_2F0, arg1->x, arg1->y * scale,
            arg1->z, 50.0f);
    }

    for (i = 0; i < 8; i++) {
        lbl_1_data_2F0.vel.x =
            (f32)(scale * (1.5 * sin((3.141592653589793 * (270.0 + (22.5 + 45.0f * i))) / 180.0)));
        lbl_1_data_2F0.vel.y = 1.0f;
        lbl_1_data_2F0.vel.z =
            (f32)(scale * (1.5 * cos((3.141592653589793 * (270.0 + (22.5 + 45.0f * i))) / 180.0)));
        fn_1_6B00(arg0, &lbl_1_data_2F0, arg1->x, arg1->y * scale,
            arg1->z, 30.0f);
    }

    lbl_1_data_2F0.colorStart.a = 0xFF;
}

s16 fn_1_6B00(s16 arg0, M560EffectParam *arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5)
{
    s16 i;
    s16 result;
    s16 bit;
    s16 idx;
    HU3DPARTICLE *parm;
    HU3DPARTICLEDATA *dataP;
    HU3DMODEL *model;
    M560EffectParam *slots;
    s16 id;

    fn_1_6DA4();

    result = -1;

    for (i = 0, bit = 1; i < 16; i++, bit <<= 1) {
        if ((bit & arg0) == 0) {
            continue;
        }

        if (Hu3DCamera[i].fov == -1.0f) {
            continue;
        }

        id = lbl_1_bss_520[i];

        if (id == -1) {
            return -1;
        }

        model = &Hu3DData[id];
        parm = model->hookData;
        slots = parm->work;
        dataP = &parm->data[parm->emitCnt];
        idx = parm->emitCnt;

        for (; idx < parm->maxCnt; idx++, dataP++) {
            if (dataP->scale == 0.0f) {
                break;
            }
        }

        if (idx >= parm->maxCnt) {
            dataP = parm->data;
            idx = 0;

            for (; idx < parm->maxCnt; idx++, dataP++) {
                if (dataP->scale == 0.0f) {
                    break;
                }
            }
        }

        if (idx != parm->maxCnt) {
            slots[idx] = *arg1;
            dataP->cameraBit = arg0;
            dataP->pos.x = arg2;
            dataP->pos.y = arg3;
            dataP->pos.z = arg4;
            dataP->vel = arg1->vel;
            dataP->color.r = arg1->colorStart.r;
            dataP->color.g = arg1->colorStart.g;
            dataP->color.b = arg1->colorStart.b;
            dataP->color.a = arg1->colorStart.a;
            dataP->scaleBase = arg5;
            dataP->scale = arg5;
            dataP->time = 0;
            dataP->parManId = -1;
            parm->emitCnt = idx;
            result = idx;
        }
    }

    return result;
}

void fn_1_6DA4(void)
{
    s16 i;
    ANIMDATA *anim;
    HU3DPARTICLE *parm;
    s16 j;
    HU3DPARTICLEDATA *cell;
    void *dataTmp;

    anim = NULL;

    for (i = 0; i < 16; i++) {
        if (Hu3DCamera[i].fov == -1.0f) {
            continue;
        }

        if (lbl_1_bss_520[i] != -1) {
            parm = Hu3DData[lbl_1_bss_520[i]].hookData;
            if (parm->anim) {
                anim = parm->anim;
            }

            continue;
        }

        if (!anim) {
            dataTmp = HuDataSelHeapReadNum(0x73001A, HU_MEMNUM_OVL, HEAP_MODEL);
            anim = HuSprAnimRead(dataTmp);
        }

        lbl_1_bss_520[i] = Hu3DParticleCreate(anim, 0x96);
        Hu3DModelLayerSet(lbl_1_bss_520[i], 5);
        Hu3DParticleHookSet(lbl_1_bss_520[i], fn_1_722C);
        Hu3DModelCameraSet(lbl_1_bss_520[i], 1 << i);

        if (!lbl_1_bss_540) {
            lbl_1_bss_540 = HuMemDirectMallocNum(HEAP_HEAP, 0x20D0, HU_MEMNUM_OVL);
        }

        Hu3DParticleBlendModeSet(lbl_1_bss_520[i], 0);

        parm = Hu3DData[lbl_1_bss_520[i]].hookData;
        parm->emitCnt = 0;
        parm->work = lbl_1_bss_540;
        parm->count = 1;
        cell = parm->data;

        for (j = 0; j < parm->maxCnt; j++, cell++) {
            cell->scale = 0.0f;
        }
    }
}

// Creates the particle model for every camera that does not have one yet.
void fn_1_7008(void)
{
    ANIMDATA *anim;
    HU3DPARTICLE *parm;
    s16 i;
    s16 j;
    HU3DPARTICLEDATA *cell;
    void *dataTmp;

    anim = NULL;

    for (i = 0; i < 16; i++) {
        if (Hu3DCamera[i].fov == -1.0f) {
            continue;
        }

        if (lbl_1_bss_520[i] != -1) {
            continue;
        }

        if (!anim) {
            dataTmp = HuDataSelHeapReadNum(0x73001A, HU_MEMNUM_OVL, HEAP_MODEL);
            anim = HuSprAnimRead(dataTmp);
        }

        lbl_1_bss_520[i] = Hu3DParticleCreate(anim, 0x96);
        Hu3DModelLayerSet(lbl_1_bss_520[i], 5);
        Hu3DParticleHookSet(lbl_1_bss_520[i], fn_1_722C);
        Hu3DModelCameraSet(lbl_1_bss_520[i], 1 << i);

        if (!lbl_1_bss_540) {
            lbl_1_bss_540 = HuMemDirectMallocNum(HEAP_HEAP, 0x20D0, HU_MEMNUM_OVL);
        }

        Hu3DParticleBlendModeSet(lbl_1_bss_520[i], 0);

        parm = Hu3DData[lbl_1_bss_520[i]].hookData;
        parm->emitCnt = 0;
        parm->work = lbl_1_bss_540;
        parm->count = 1;
        cell = parm->data;

        for (j = 0; j < parm->maxCnt; j++, cell++) {
            cell->scale = 0.0f;
        }
    }
}

// Per-frame particle update installed by fn_1_6DA4 / fn_1_7008.
void fn_1_722C(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx)
{
    HU3DPARTICLEDATA *cell;
    M560EffectParam *row;
    s16 i;
    s32 t;
    f64 f;

    row = particleP->work;

    if (particleP->count == 0) {
        cell = particleP->data;

        for (i = 0; i < particleP->maxCnt; i++, cell++) {
            cell->scale = 0.0f;
        }
    }

    cell = particleP->data;

    for (i = 0; i < particleP->maxCnt; i++, cell++) {
        if (cell->scale == 0.0f) {
            continue;
        }

        cell->vel.x = cell->vel.x * row[i].speedDecay.x;
        cell->vel.y = cell->vel.y * row[i].speedDecay.y;
        cell->vel.z = cell->vel.z * row[i].speedDecay.z;
        PSVECAdd(&cell->vel, &cell->pos, &cell->pos);
        cell->vel.y = cell->vel.y + row[i].accelY;

        t = (s32)((f32)cell->color.r +
            row[i].colorStep * (row[i].colorEnd.r - row[i].colorStart.r));

        if ((s16)t < 0) {
            t = 0;
        } else if ((s16)t > 0xFF) {
            t = 0xFF;
        }

        cell->color.r = (u8)t;

        t = (s32)((f32)cell->color.g +
            row[i].colorStep * (row[i].colorEnd.g - row[i].colorStart.g));

        if ((s16)t < 0) {
            t = 0;
        } else if ((s16)t > 0xFF) {
            t = 0xFF;
        }

        cell->color.g = (u8)t;

        t = (s32)((f32)cell->color.b +
            row[i].colorStep * (row[i].colorEnd.b - row[i].colorStart.b));

        if ((s16)t < 0) {
            t = 0;
        } else if ((s16)t > 0xFF) {
            t = 0xFF;
        }

        cell->color.b = (u8)t;

        t = (s32)((f32)cell->color.a + row[i].alphaStep);

        if ((s16)t < 1) {
            cell->scale = 0.0f;
        }

        cell->color.a = (u8)t;

        if (cell->scale != 0.0f) {
            if ((u32)row[i].flags & 1) {
                if ((cell->time + i) & 1) {
                    f = 1.0;
                } else {
                    f = 0.5;
                }

                cell->scale = (f32)((f64)cell->scaleBase * f);
            } else {
                cell->scale = cell->scaleBase;
            }

            cell->scaleBase = cell->scaleBase + row[i].scaleDecay;

            if (cell->scaleBase <= 0.01f) {
                cell->scale = 0.0f;
            }
        }

        cell->time++;
    }

    DCStoreRangeNoSync(particleP->data, particleP->maxCnt * sizeof(HU3DPARTICLEDATA));
}
