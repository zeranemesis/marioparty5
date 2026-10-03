#include "dolphin/types.h"
#include "datadir_enum.h"
#include "game/memory.h"

/*
 * heiho.cpp builds the four item models an object hangs off its left and
 * right hands. Each model is created through fn_1_618, bound to one of the
 * item HSF files by fn_1_32AC, and the object keeps the four pointers at
 * offsets 0x00..0x0C. The halfword at offset 0 of those model objects is the
 * HU3DMODELID the renderer wants, so every call below reads the id back out
 * of the pointed-at struct rather than keeping it in a local. Each read goes
 * through MODEL_ID(), whose (s16)(s32) cast is what the original's codegen
 * shows.
 *
 * The Hu3D entry points are declared here instead of including game/hu3d.h:
 * that header reaches humath.h, which makes MWCC emit the two unused sqrtf
 * local statics into .rodata, and this unit's target object has no .rodata.
 * For the same reason game/data.h is not included and HuDataSelHeapReadNum is
 * declared locally - that header gives the name C++ linkage in this unit.
 */

typedef s16 HU3DMODELID;
typedef s16 HU3DMOTID;

#define MODEL_ID(p) ((HU3DMODELID)(s32)(p)->m_id)

struct Hu3DModelId {
    HU3DMODELID m_id;  // 0x00
};

struct HeihoModels {
    Hu3DModelId *m_mdl0;  // 0x00
    Hu3DModelId *m_mdl1;  // 0x04
    Hu3DModelId *m_mdl2;  // 0x08
    Hu3DModelId *m_mdl3;  // 0x0C
};

char lbl_1_data_360[] = "itemhook_L";
char lbl_1_data_36B[] = "itemhook_R";
char lbl_1_data_376[18] = "ski-itemhook-h";

extern "C" {

// module-local helpers
void *fn_1_618(s32 id);
void *fn_1_32AC(void *self, int dataNum);
void fn_1_3420(void *self, u32 arg2);
void fn_1_370C(void *self, s32 arg2);
void fn_1_6E0(void *self);

void Hu3DModelShadowSet(HU3DMODELID modelId);
void Hu3DModelShadowDispOn(HU3DMODELID modelId);
void Hu3DModelHookSet(HU3DMODELID modelId, char *objName, HU3DMODELID hookMdlId);
void Hu3DMotionShapeSet(HU3DMODELID modelId, HU3DMOTID motId);
HU3DMOTID Hu3DJointMotion(HU3DMODELID modelId, void *data);
void *HuDataSelHeapReadNum(int dataNum, s32 num, HEAPID heap);

void fn_1_98A0(HU3DMODELID modelId)
{
    Hu3DModelShadowSet(modelId);
    Hu3DModelShadowDispOn(modelId);
}

// NONMATCHING: logic and call sequence are right, but this function is
// register-pressure bound. The target frame is 0xE0 against 0xB0 here: it
// gives every one of the 50 named locals its own stack home and re-loads the
// model pointers from those homes before dereferencing them
// (stw rX,home / lwz rY,home), while this build keeps the pointers in
// callee-saved registers and lets several locals share a slot. The remaining
// difference is allocation only - no instruction in the middle differs.
HeihoModels *fn_1_98D8(HeihoModels *self)
{
    void *motData;
    void *mdl0;
    void *mdl1;
    void *mdl2;
    void *mdl3;
    Hu3DModelId *q0;
    Hu3DModelId *q1;
    Hu3DModelId *q2;
    Hu3DModelId *q3;
    Hu3DModelId *p0;
    Hu3DModelId *p1;
    Hu3DModelId *p2;
    Hu3DModelId *p3;
    Hu3DModelId *p4;
    Hu3DModelId *p5;
    Hu3DModelId *p6;
    Hu3DModelId *p7;
    Hu3DModelId *p8;
    Hu3DModelId *p9;
    Hu3DModelId *p10;
    Hu3DModelId *p11;
    Hu3DModelId *p12;
    Hu3DModelId *p13;
    HU3DMOTID mot;
    s16 id_10c;
    s16 id_11c;
    s16 id_0a, id_0b;
    s16 id_1a, id_1b;
    s16 id_2a, id_2b;
    s16 id_3a, id_3b;
    s16 id_4a, id_4b;
    s16 id_5a, id_5b;
    s16 id_6a, id_6b;
    s16 id_7a, id_7b;
    s16 id_8a, id_8b;
    s16 id_9a, id_9b;
    s16 id_10a, id_10b;
    s16 id_11a, id_11b;

    mdl0 = fn_1_618(2);
    if (mdl0) {
        mdl0 = fn_1_32AC(mdl0, DATANUM(DATA_m568, 0x11));
    }
    self->m_mdl0 = (Hu3DModelId *)mdl0;

    mdl1 = fn_1_618(2);
    if (mdl1) {
        mdl1 = fn_1_32AC(mdl1, DATANUM(DATA_m568, 0x14));
    }
    self->m_mdl1 = (Hu3DModelId *)mdl1;

    mdl2 = fn_1_618(2);
    if (mdl2) {
        mdl2 = fn_1_32AC(mdl2, DATANUM(DATA_m568, 0x14));
    }
    self->m_mdl2 = (Hu3DModelId *)mdl2;

    mdl3 = fn_1_618(2);
    if (mdl3) {
        mdl3 = fn_1_32AC(mdl3, DATANUM(DATA_m568, 0x13));
    }
    self->m_mdl3 = (Hu3DModelId *)mdl3;

    motData = HuDataSelHeapReadNum(DATANUM(DATA_m568, 0x12), HU_MEMNUM_OVL, HEAP_MODEL);

    p0 = self->m_mdl0;
    id_0a = MODEL_ID(p0);
    id_0b = id_0a;
    mot = Hu3DJointMotion(id_0b, motData);

    p1 = self->m_mdl0;
    id_1a = MODEL_ID(p1);
    id_1b = id_1a;
    Hu3DMotionShapeSet(id_1b, mot);

    p2 = self->m_mdl0;
    fn_1_3420(p2, 0x400040);

    p3 = self->m_mdl1;
    id_2a = MODEL_ID(p3);
    id_2b = id_2a;
    p4 = self->m_mdl0;
    id_3a = MODEL_ID(p4);
    id_3b = id_3a;
    Hu3DModelHookSet(id_3b, lbl_1_data_360, id_2b);

    p5 = self->m_mdl2;
    id_4a = MODEL_ID(p5);
    id_4b = id_4a;
    p6 = self->m_mdl0;
    id_5a = MODEL_ID(p6);
    id_5b = id_5a;
    Hu3DModelHookSet(id_5b, lbl_1_data_36B, id_4b);

    p7 = self->m_mdl3;
    id_6a = MODEL_ID(p7);
    id_6b = id_6a;
    p8 = self->m_mdl0;
    id_7a = MODEL_ID(p8);
    id_7b = id_7a;
    Hu3DModelHookSet(id_7b, lbl_1_data_376, id_6b);

    p9 = self->m_mdl3;
    fn_1_3420(p9, 0x400001);

    p10 = self->m_mdl0;
    q0 = p10;
    id_8a = MODEL_ID(q0);
    id_8b = id_8a;
    Hu3DModelShadowSet(id_8b);
    Hu3DModelShadowDispOn(id_8b);

    p11 = self->m_mdl1;
    q1 = p11;
    id_9a = MODEL_ID(q1);
    id_9b = id_9a;
    Hu3DModelShadowSet(id_9b);
    Hu3DModelShadowDispOn(id_9b);

    p12 = self->m_mdl2;
    q2 = p12;
    id_10a = MODEL_ID(q2);
    id_10b = id_10a;
    id_10c = id_10b;
    Hu3DModelShadowSet(id_10c);
    Hu3DModelShadowDispOn(id_10c);

    p13 = self->m_mdl3;
    q3 = p13;
    id_11a = MODEL_ID(q3);
    id_11b = id_11a;
    id_11c = id_11b;
    Hu3DModelShadowSet(id_11c);
    Hu3DModelShadowDispOn(id_11c);

    return self;
}

void *fn_1_9C74(HeihoModels *self, s16 num)
{
    if (self) {
        fn_1_370C(self->m_mdl0, 1);
        if (num > 0) {
            fn_1_6E0(self);
        }
    }
    return self;
}

}