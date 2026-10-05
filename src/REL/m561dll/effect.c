#include "game/data.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/sprite.h"
#include "REL/m561dll.h"

s16 lbl_1_bss_2E0[16];
void *lbl_1_bss_300;

M561EffectParam lbl_1_data_5A0 = {
    0,
    { 0x00, 0x00, 0x00, 0x80 },
    { 0x00, 0x00, 0x00, 0x00 },
    { 0.0f, 2.0f, 1.0f },
    { 0.95f, 0.99f, 0.95f },
    0.0f,
    0.0f,
    1.5f,
    -4.0f,
    0.0f,
};

/* M561ParticleData in REL/m561dll.h stops at 0x3C, but the live particle array
   is walked with a 0x44 stride and keeps a 4-byte colour at 0x40, so the real
   shape is redone locally. */
typedef struct M561Cell_s {
    /* 0x00 */ s16 state;
    /* 0x02 */ s16 prev;
    /* 0x04 */ s16 unk04;
    /* 0x06 */ s16 modelId;
    /* 0x08 */ Vec vel;
    /* 0x14 */ u8 pad14[0x14];
    /* 0x28 */ f32 scale;
    /* 0x2C */ f32 inUse;
    /* 0x30 */ u8 pad30[4];
    /* 0x34 */ Vec pos;
    /* 0x40 */ u8 color[4];
} M561Cell;

void fn_1_8AE8(void)
{
    s32 i;

    for (i = 0; i < 16; i++) {
        lbl_1_bss_2E0[i] = -1;
    }
    lbl_1_bss_300 = NULL;
}

void fn_1_8B38(void)
{
    s32 i;

    if (lbl_1_bss_300) {
        HuMemDirectFree(lbl_1_bss_300);
        lbl_1_bss_300 = NULL;
    }

    for (i = 0; i < 16; i++) {
        lbl_1_bss_2E0[i] = -1;
    }
    lbl_1_bss_300 = NULL;
}

// NONMATCHING: 218 of 232 instruction rows match. The 14 remaining rows are the
// two n_1_8F5C call sites (7 each): CodeWarrior evaluates the pointer argument
// &lbl_1_data_5A0 last, so it borrows r4 as the scratch register for the 50.0f /
// 30.0f literal and only then materialises the address; MWCC evaluates arguments
// left to right, commits r4 up front and uses r5 as the scratch instead.
void fn_1_8BCC(s16 arg0, Vec *arg1)
{
    s16 i;
    f32 scale;

    scale = 2.0f;

    lbl_1_data_5A0.colorStart.a = 0x80;

    for (i = 0; i < 8; i++) {
        lbl_1_data_5A0.vel.x =
            (f32)(scale * (3.5 * sin((3.141592653589793 * (270.0f + 45.0f * i)) / 180.0)));
        lbl_1_data_5A0.vel.y = 1.0f;
        lbl_1_data_5A0.vel.z =
            (f32)(scale * (3.5 * cos((3.141592653589793 * (270.0f + 45.0f * i)) / 180.0)));
        fn_1_8F5C(arg0, &lbl_1_data_5A0, arg1->x, arg1->y * scale,
            arg1->z, 50.0f);
    }

    for (i = 0; i < 8; i++) {
        lbl_1_data_5A0.vel.x =
            (f32)(scale * (1.5 * sin((3.141592653589793 * (270.0 + (22.5 + 45.0f * i))) / 180.0)));
        lbl_1_data_5A0.vel.y = 1.0f;
        lbl_1_data_5A0.vel.z =
            (f32)(scale * (1.5 * cos((3.141592653589793 * (270.0 + (22.5 + 45.0f * i))) / 180.0)));
        fn_1_8F5C(arg0, &lbl_1_data_5A0, arg1->x, arg1->y * scale,
            arg1->z, 30.0f);
    }

    lbl_1_data_5A0.colorStart.a = 0xFF;
}

void fn_1_9200(void)
{
    s16 i;
    ANIMDATA *anim;
    M561Hu3DParm *parm;
    s16 j;
    M561Cell *cell;
    void *dataTmp;

    anim = NULL;

    for (i = 0; i < 16; i++) {
        if (Hu3DCamera[i].fov == -1.0f) {
            continue;
        }

        if (lbl_1_bss_2E0[i] != -1) {
            parm = ((M561Hu3DModel *)&Hu3DData[lbl_1_bss_2E0[i]])->parm;
            if (parm->anim) {
                anim = parm->anim;
            }

            continue;
        }

        if (!anim) {
            dataTmp = HuDataSelHeapReadNum(0x74001A, HU_MEMNUM_OVL, HEAP_MODEL);
        anim = HuSprAnimRead(dataTmp);
        }

        lbl_1_bss_2E0[i] = Hu3DParticleCreate(anim, 0x96);
        Hu3DModelLayerSet(lbl_1_bss_2E0[i], 5);
        Hu3DParticleHookSet(lbl_1_bss_2E0[i], fn_1_9688);
        Hu3DModelCameraSet(lbl_1_bss_2E0[i], 1 << i);

        if (!lbl_1_bss_300) {
            lbl_1_bss_300 = HuMemDirectMallocNum(HEAP_HEAP, 0x20D0, HU_MEMNUM_OVL);
        }

        Hu3DParticleBlendModeSet(lbl_1_bss_2E0[i], 0);

        parm = ((M561Hu3DModel *)&Hu3DData[lbl_1_bss_2E0[i]])->parm;
        parm->curIdx = 0;
        parm->slots = (M561EffectSlot *)lbl_1_bss_300;
        parm->unk34 = 1;
        cell = (M561Cell *)parm->data;

        for (j = 0; j < parm->dataCnt; j++, cell++) {
            cell->inUse = 0.0f;
        }
    }
}

// Creates the particle model for every camera that does not have one yet.
// Mirrors fn_1_9200 except that it only fills in missing models.
void fn_1_9464(void)
{
    ANIMDATA *anim;
    M561Hu3DParm *parm;
    s16 i;
    s16 j;
    M561Cell *cell;
    void *dataTmp;

    anim = NULL;

    for (i = 0; i < 16; i++) {
        if (Hu3DCamera[i].fov == -1.0f) {
            continue;
        }

        if (lbl_1_bss_2E0[i] != -1) {
            continue;
        }

        if (!anim) {
            dataTmp = HuDataSelHeapReadNum(0x74001A, HU_MEMNUM_OVL, HEAP_MODEL);
        anim = HuSprAnimRead(dataTmp);
        }

        lbl_1_bss_2E0[i] = Hu3DParticleCreate(anim, 0x96);
        Hu3DModelLayerSet(lbl_1_bss_2E0[i], 5);
        Hu3DParticleHookSet(lbl_1_bss_2E0[i], fn_1_9688);
        Hu3DModelCameraSet(lbl_1_bss_2E0[i], 1 << i);

        if (!lbl_1_bss_300) {
            lbl_1_bss_300 = HuMemDirectMallocNum(HEAP_HEAP, 0x20D0, HU_MEMNUM_OVL);
        }

        Hu3DParticleBlendModeSet(lbl_1_bss_2E0[i], 0);

        parm = ((M561Hu3DModel *)&Hu3DData[lbl_1_bss_2E0[i]])->parm;
        parm->curIdx = 0;
        parm->slots = (M561EffectSlot *)lbl_1_bss_300;
        parm->unk34 = 1;
        cell = (M561Cell *)parm->data;

        for (j = 0; j < parm->dataCnt; j++, cell++) {
            cell->inUse = 0.0f;
        }
    }
}

// NONMATCHING: 124 of 174 instruction rows match. The residue is register
// allocation - arg1 lands in r28 and the loop counter in r27 where the
// original has them the other way round, which also relocates the two `while`
// bodies - plus the `mr r27, r4` parameter copy, which CodeWarrior emits
// after the incoming float spills and MWCC emits before them.
s16 fn_1_8F5C(s16 arg0, M561EffectParam *arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5)
{
    s16 i;
    s16 result;
    s16 bit;
    s16 idx;
    M561Hu3DParm *parm;
    M561Cell *dataP;
    M561Hu3DModel *model;
    M561EffectSlot *slots;
    s16 id;

    fn_1_9200();

    result = -1;

    for (i = 0, bit = 1; i < 16; i++, bit <<= 1) {
        if ((bit & arg0) == 0) {
            continue;
        }

        if (Hu3DCamera[i].fov == -1.0f) {
            continue;
        }

        id = lbl_1_bss_2E0[i];

        if (id == -1) {
            return -1;
        }

        model = (M561Hu3DModel *)&Hu3DData[id];
        parm = model->parm;
        slots = parm->slots;
        dataP = (M561Cell *)&parm->data[parm->curIdx];
        idx = parm->curIdx;

        while ((dataP->inUse != 0.0f) && (idx < parm->dataCnt)) {
            idx++;
            dataP++;
        }

        if (idx >= parm->dataCnt) {
            dataP = (M561Cell *)parm->data;
            idx = 0;

            while ((dataP->inUse != 0.0f) && (idx < parm->dataCnt)) {
                idx++;
                dataP++;
            }
        }

        if (idx != parm->dataCnt) {
            *(M561EffectSlot *)&slots[idx] = *arg1;
            dataP->modelId = arg0;
            dataP->pos.x = arg2;
            dataP->pos.y = arg3;
            dataP->pos.z = arg4;
            dataP->vel = *(Vec *)&arg1->vel;
            dataP->color[0] = ((u8 *)arg1)[4];
            dataP->color[1] = ((u8 *)arg1)[5];
            dataP->color[2] = ((u8 *)arg1)[6];
            dataP->color[3] = ((u8 *)arg1)[7];
            dataP->scale = arg5;
            dataP->inUse = arg5;
            dataP->state = 0;
            dataP->prev = -1;
            parm->curIdx = idx;
            result = idx;
        }
    }

    return result;
}

/* The hook's second argument has the M561Hu3DParm shape (0x1C slots, 0x30
   dataCnt, 0x34 unk34, 0x48 data); the declared HU3DPARTICLE is re-interpreted
   through this alias so no extra local is introduced. */
#define M561PARM ((M561Hu3DParm *)particleP)

// Per-frame particle update installed by fn_1_9200 / fn_1_9464.
// modelP and mtx are unused; the engine calls the hook with them anyway.
// NONMATCHING: 300 of 306 instruction rows match exactly. The remaining six are
// the two `cell->inUse == 0.0f` guards: MWCC materialises the 0.0f literal into
// f1 before the field load while the original loads the field into f1 first.
// Swapping the comparison operands was tried and changes nothing.
void fn_1_9688(HU3DMODEL *modelP, HU3DPARTICLE *particleP, f32 mtx[3][4])
{
    M561Cell *cell;
    M561EffectParam *row;
    s16 i;
    s32 t;
    f64 f;

    row = M561PARM->slots;

    if ((u32)M561PARM->unk34 == 0) {
        cell = (M561Cell *)M561PARM->data;

        for (i = 0; i < M561PARM->dataCnt; i++, cell++) {
            cell->inUse = 0.0f;
        }
    }

    cell = (M561Cell *)M561PARM->data;

    for (i = 0; i < M561PARM->dataCnt; i++, cell++) {
        if (cell->inUse == 0.0f) {
            continue;
        }

        cell->vel.x = cell->vel.x * row[i].speedDecay.x;
        cell->vel.y = cell->vel.y * row[i].speedDecay.y;
        cell->vel.z = cell->vel.z * row[i].speedDecay.z;
        PSVECAdd(&cell->vel, &cell->pos, &cell->pos);
        cell->vel.y = cell->vel.y + row[i].accelY;

        t = (s32)((f32)cell->color[0] +
            row[i].colorStep * (row[i].colorEnd.r - row[i].colorStart.r));

        if ((s16)t < 0) {
            t = 0;
        } else if ((s16)t > 0xFF) {
            t = 0xFF;
        }

        cell->color[0] = (u8)t;

        t = (s32)((f32)cell->color[1] +
            row[i].colorStep * (row[i].colorEnd.g - row[i].colorStart.g));

        if ((s16)t < 0) {
            t = 0;
        } else if ((s16)t > 0xFF) {
            t = 0xFF;
        }

        cell->color[1] = (u8)t;

        t = (s32)((f32)cell->color[2] +
            row[i].colorStep * (row[i].colorEnd.b - row[i].colorStart.b));

        if ((s16)t < 0) {
            t = 0;
        } else if ((s16)t > 0xFF) {
            t = 0xFF;
        }

        cell->color[2] = (u8)t;

        t = (s32)((f32)cell->color[3] + row[i].alphaStep);

        if ((s16)t < 1) {
            cell->inUse = 0.0f;
        }

        cell->color[3] = (u8)t;

        if (cell->inUse != 0.0f) {
            if ((u32)row[i].flags & 1) {
                if ((cell->state + i) & 1) {
                    f = 1.0;
                } else {
                    f = 0.5;
                }

                cell->inUse = (f32)((f64)cell->scale * f);
            } else {
                cell->inUse = cell->scale;
            }

            cell->scale = cell->scale + row[i].scaleDecay;

            if (cell->scale <= 0.01f) {
                cell->inUse = 0.0f;
            }
        }

        cell->state++;
    }

    DCStoreRangeNoSync(M561PARM->data, M561PARM->dataCnt * 0x44);
}
