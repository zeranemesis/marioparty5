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
            if (parm->unk44 != NULL) {
                anim = parm->unk44;
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
        parm->unk44 = lbl_1_bss_300;
        parm->unk34 = 1;

        for (j = 0; j < parm->dataCnt; j++) {
            parm->data[j].unk2C = 0.0f;
        }
    }
}
