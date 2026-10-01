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

void fn_1_8AE8(void)
{
    s16 i;

    for (i = 0; i < 16; i++) {
        lbl_1_bss_2E0[i] = -1;
    }
    lbl_1_bss_300 = NULL;
}

void fn_1_8B38(void)
{
    s16 i;

    if (lbl_1_bss_300) {
        HuMemDirectFree(lbl_1_bss_300);
        lbl_1_bss_300 = NULL;
    }

    for (i = 0; i < 16; i++) {
        lbl_1_bss_2E0[i] = -1;
    }
    lbl_1_bss_300 = NULL;
}

// NONMATCHING: instruction sequence is exact (234/234 rows) but MWCC assigns
// arg0/arg1 to r28/r31 here instead of r29/r30, shifting every register
// reference; local declaration order did not change the allocation.
void fn_1_8BCC(s16 arg0, Vec *arg1)
{
    s16 i;
    s16 j;
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

    for (j = 0; j < 8; j++) {
        lbl_1_data_5A0.vel.x =
            (f32)(scale * (1.5 * sin((3.141592653589793 * (270.0 + 22.5 + 45.0f * j)) / 180.0)));
        lbl_1_data_5A0.vel.y = 1.0f;
        lbl_1_data_5A0.vel.z =
            (f32)(scale * (1.5 * cos((3.141592653589793 * (270.0 + 22.5 + 45.0f * j)) / 180.0)));
        fn_1_8F5C(arg0, &lbl_1_data_5A0, arg1->x, arg1->y * scale,
            arg1->z, 30.0f);
    }

    lbl_1_data_5A0.colorStart.a = 0xFF;
}

// NONMATCHING: 166/166 instruction rows, all offsets correct; differs only in
// register allocation (r27..r31 vs r28..r32) and in cmplwi-vs-cmplw lowering of
// the NULL tests, which MWCC emits differently for this local ordering.
void fn_1_9200(void)
{
    s16 i;
    ANIMDATA *anim;
    M561Hu3DParm *parm;
    s16 j;

    anim = NULL;

    for (i = 0; i < 16; i++) {
        if (Hu3DCamera[i].fov == -1.0f) {
            continue;
        }

        if (lbl_1_bss_2E0[i] != -1) {
            parm = ((M561Hu3DModel *)&Hu3DData[lbl_1_bss_2E0[i]])->parm;
            if (parm->anim != NULL) {
                anim = parm->anim;
                continue;
            }
        }

        if (anim == NULL) {
            anim = HuSprAnimRead(HuDataSelHeapReadNum(0x8E, 0x1000, 2));
        }

        lbl_1_bss_2E0[i] = Hu3DParticleCreate(anim, 0x96);
        Hu3DModelLayerSet(lbl_1_bss_2E0[i], 5);
        Hu3DParticleHookSet(lbl_1_bss_2E0[i], fn_1_9688);
        Hu3DModelCameraSet(lbl_1_bss_2E0[i], 1 << i);

        if (lbl_1_bss_300 == NULL) {
            lbl_1_bss_300 = HuMemDirectMallocNum(HEAP_HEAP, 0x20D0, 0x1000);
        }

        Hu3DParticleBlendModeSet(lbl_1_bss_2E0[i], 0);

        parm = ((M561Hu3DModel *)&Hu3DData[lbl_1_bss_2E0[i]])->parm;
        parm->slots = (M561EffectSlot *)lbl_1_bss_300;
        parm->unk34 = 1;

        for (j = 0; j < parm->dataCnt; j++) {
            parm->data[j].inUse = 0.0f;
        }
    }
}

// Creates the particle model for every camera that does not have one yet.
// Mirrors fn_1_9200 except that it only fills in missing models.
// NONMATCHING: control flow and all field offsets are recovered, but the
// register allocation and hoisting of the HuDataSelHeapReadNum operands differ.
void fn_1_9464(void)
{
    ANIMDATA *anim;
    M561Hu3DParm *parm;
    s16 i;
    s16 j;

    anim = NULL;

    for (i = 0; i < 16; i++) {
        if (Hu3DCamera[i].fov == -1.0f) {
            continue;
        }

        if (lbl_1_bss_2E0[i] != -1) {
            continue;
        }

        if (anim == NULL) {
            anim = HuSprAnimRead(HuDataSelHeapReadNum(0x8E, 0x1000, 2));
        }

        lbl_1_bss_2E0[i] = Hu3DParticleCreate(anim, 0x96);
        Hu3DModelLayerSet(lbl_1_bss_2E0[i], 5);
        Hu3DParticleHookSet(lbl_1_bss_2E0[i], fn_1_9688);
        Hu3DModelCameraSet(lbl_1_bss_2E0[i], 1 << i);

        if (lbl_1_bss_300 == NULL) {
            lbl_1_bss_300 = HuMemDirectMallocNum(HEAP_HEAP, 0x20D0, 0x1000);
        }

        Hu3DParticleBlendModeSet(lbl_1_bss_2E0[i], 0);

        parm = ((M561Hu3DModel *)&Hu3DData[lbl_1_bss_2E0[i]])->parm;
        parm->curIdx = 0;
        parm->slots = (M561EffectSlot *)lbl_1_bss_300;
        parm->unk34 = 1;

        for (j = 0; j < parm->dataCnt; j++) {
            parm->data[j].inUse = 0.0f;
        }
    }
}

// NONMATCHING: body is faithful and the 0x38/0x44/0x58/0x134 strides and all
// field offsets are recovered, but the fixed-size 0x38 copy from the param
// struct is emitted as a word loop here; register allocation also differs.
s16 fn_1_8F5C(s16 arg0, M561EffectParam *arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5)
{
    s16 i;
    s16 result;
    s16 bit;
    s16 idx;
    M561Hu3DParm *parm;
    M561ParticleData *dataP;
    M561EffectSlot *dst;

    result = -1;
    bit = 1;

    fn_1_9200();

    for (i = 0; i < 16; i++, bit <<= 1) {
        if ((bit & arg0) == 0) {
            continue;
        }

        if (Hu3DCamera[i].fov == -1.0f) {
            continue;
        }

        if (lbl_1_bss_2E0[i] == -1) {
            return -1;
        }

        parm = ((M561Hu3DModel *)&Hu3DData[lbl_1_bss_2E0[i]])->parm;
        idx = parm->curIdx;
        dataP = &parm->data[idx];

        while ((idx < parm->dataCnt) && (dataP->inUse != 0.0f)) {
            idx++;
            dataP++;
        }

        if (idx >= parm->dataCnt) {
            dataP = parm->data;
            idx = 0;

            while ((idx < parm->dataCnt) && (dataP->inUse != 0.0f)) {
                idx++;
                dataP++;
            }
        }

        if (idx != parm->dataCnt) {
            dst = &parm->slots[idx];
            *dst = *arg1;
            dataP->modelId = arg0;
            dataP->posX = arg2;
            dataP->posY = arg3;
            dataP->posZ = arg4;
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
