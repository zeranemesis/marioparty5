#include <math.h>

#include "dolphin.h"

#include "datadir_enum.h"
#include "game/hu3d.h"

/* Three contiguous floats. HuVecF is 16-byte aligned, which would move every
   following field, so the unit uses its own 4-aligned vector type. */
typedef struct {
    f32 x;
    f32 y;
    f32 z;
} M511_VECF;

/* These are plain C in the original: m511.o defines them unmangled
   (m511.cpp is NonMatching, so the module links the original object).
   Left as C++ they mangle and the REL link fails with
   "Failed to find symbol fn_1_1D88__FPv in any module". */
extern "C" void *__dl__FPv(void *self);
extern "C" void *__nw__FUl(u32 size);
extern "C" void __register_global_object(void *obj, void *(*dtor)(void *, s16), void *reg);
f32 fn_1_1A150(u32 self, void *arg1);
extern "C" void fn_1_1D88(void *self);
extern "C" void fn_1_1D94(void *self, s32 arg1);
extern "C" void fn_1_1E54(void *self, void **arg1);
extern "C" void fn_1_372C(void *self, s32 arg1);
void fn_1_384C(void *self, void *arg1);
extern "C" void fn_1_398C(void *self, void *arg1);
extern "C" void fn_1_39D4(void *self, void *arg1, f32 arg2);
extern "C" void fn_1_3B8C(void *self, s32 arg1);
extern "C" void fn_1_CF8(char *cond, char *file, s32 line);

struct Mgr511Stage;
struct Mgr511StageNode;

Mgr511StageNode *fn_1_CE34(Mgr511StageNode *arg0, Mgr511Stage *arg1, M511_VECF *arg2);
void fn_1_D00C(void);
void fn_1_D0E0(void);
void fn_1_D110(Mgr511StageNode *arg0);
void fn_1_D138(Mgr511StageNode *arg0);
void fn_1_D1A8(Mgr511StageNode **arg0, void (*arg1)(Mgr511StageNode *));
void fn_1_D208(Mgr511StageNode *arg0, Mgr511StageNode **arg1, void (*arg2)(Mgr511StageNode *));
void *fn_1_D2C0(void *arg0, s16 arg1);

struct Mgr511StageNode {
    void *unk0;
    void *unk4;
    M511_VECF pos;
    M511_VECF prev;
    s16 modelId;
    u16 pad22;
    u32 unk24;
    u32 mgr;
};

struct Mgr511Stage {
    void *unk0;
    void *unk4;
    void **vtable;
    s32 unkC;
    s16 modelId10;
    s16 modelId12;
    s16 modelId14;
    s16 modelId16;
    s16 modelId18;
    s16 modelId1A;
    s16 modelId1C;
};

struct Mgr511StageList {
    Mgr511StageNode *next;
    Mgr511StageNode *prev;
};

extern "C" {

extern Mgr511Stage *lbl_1_bss_18;
extern u32 lbl_1_bss_1C;
extern void *lbl_1_bss_2C;

extern void *lbl_1_data_40[6];
extern void *lbl_1_data_464[6];
extern GXColor lbl_1_data_430;
extern GXColor lbl_1_data_434;

extern Mgr511StageList lbl_1_bss_A84;
extern Mgr511StageNode *lbl_1_bss_A78;

static const f32 lbl_1_rodata_36C = 5000.0f;
static const f32 lbl_1_rodata_370 = -5000.0f;
static const f32 lbl_1_rodata_374 = -1.25f;

void fn_1_C55C(s16 layerNo)
{
    f32 sp14;
    f32 sp10;
    f32 spC;
    GXColor color;

    Hu3DCameraPerspectiveGet(1, &sp14, &sp10, &spC);
    color.r = lbl_1_data_430.r;
    color.g = lbl_1_data_430.g;
    color.b = lbl_1_data_430.b;
    color.a = lbl_1_data_430.a;
    GXSetFog(GX_FOG_EXP, 3000.0f, 20000.0f, sp10, spC, color);
}

void fn_1_C5EC(s16 layerNo)
{
    f32 sp14;
    f32 sp10;
    f32 spC;
    GXColor color;

    Hu3DCameraPerspectiveGet(1, &sp14, &sp10, &spC);
    color.r = lbl_1_data_434.r;
    color.g = lbl_1_data_434.g;
    color.b = lbl_1_data_434.b;
    color.a = lbl_1_data_434.a;
    GXSetFog(GX_FOG_NONE, 3000.0f, 20000.0f, sp10, spC, color);
}

// NONMATCHING: the two placement loops are folded differently.
Mgr511Stage *fn_1_C67C(Mgr511Stage *arg0, s32 arg_sp0)
{
    s32 i;
    s32 j;
    M511_VECF spVec;
    Mgr511StageNode *node;
    s16 modelId;

    fn_1_1D88(arg0);
    arg0->vtable = &lbl_1_data_40[2];
    arg0->unkC = 0;
    fn_1_1E54(arg0, (void **)lbl_1_bss_2C);
    if (lbl_1_bss_18 == NULL) {
        fn_1_CF8("ms_pointer == 0", "singleton.h", 0x35);
    }
    lbl_1_bss_18 = arg0;
    arg0->vtable = lbl_1_data_40;
    fn_1_372C(&arg0->modelId10, DATANUM(DATA_m511, 0x11));
    fn_1_372C(&arg0->modelId12, DATANUM(DATA_m511, 0x12));
    fn_1_372C(&arg0->modelId14, DATANUM(DATA_m511, 0x13));
    fn_1_372C(&arg0->modelId16, DATANUM(DATA_m511, 0x14));
    fn_1_372C(&arg0->modelId18, DATANUM(DATA_m511, 0x1E));
    fn_1_372C(&arg0->modelId1A, DATANUM(DATA_m511, 0x1D));
    fn_1_372C(&arg0->modelId1C, DATANUM(DATA_m511, 0x15));
    Hu3DLayerHookSet(0, fn_1_C5EC);
    Hu3DLayerHookSet(8, fn_1_C55C);
    modelId = arg0->modelId10;
    Hu3DModelLayerSet(modelId, 0);
    Hu3DModelLightInfoSet(modelId, 1);
    Hu3DModelPosSet(modelId, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrReset(modelId, 0x40000002);
    Hu3DModelAttrSet(modelId, 0x40000001);
    Hu3DModelAttrReset(modelId, 1);
    Hu3DModelCameraSet(modelId, 0xF);
    modelId = arg0->modelId12;
    Hu3DModelLayerSet(modelId, 1);
    Hu3DModelPosSet(modelId, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrReset(modelId, 0x40000002);
    Hu3DModelAttrSet(modelId, 0x40000001);
    Hu3DModelAttrReset(modelId, 1);
    Hu3DModelCameraSet(modelId, 0xF);
    modelId = arg0->modelId14;
    Hu3DModelLayerSet(modelId, 1);
    Hu3DModelPosSet(modelId, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrReset(modelId, 0x40000002);
    Hu3DModelAttrSet(modelId, 0x40000001);
    Hu3DModelAttrReset(modelId, 1);
    Hu3DModelAttrSet(modelId, 0x4000);
    Hu3DModelCameraSet(modelId, 0xF);
    Hu3DModelAttrSet(arg0->modelId16, 1);
    modelId = arg0->modelId18;
    Hu3DModelLayerSet(modelId, 1);
    Hu3DModelPosSet(modelId, 0.0f, 0.0f, 5000.0f);
    Hu3DModelAttrReset(modelId, 1);
    Hu3DModelAttrSet(modelId, 0x4000);
    Hu3DModelCameraSet(modelId, 0xF);
    modelId = arg0->modelId1A;
    Hu3DModelLayerSet(modelId, 1);
    Hu3DModelPosSet(modelId, 0.0f, 0.0f, -5000.0f);
    Hu3DModelAttrReset(modelId, 1);
    Hu3DModelAttrSet(modelId, 0x4000);
    Hu3DModelCameraSet(modelId, 0xF);
    modelId = arg0->modelId1C;
    Hu3DModelLayerSet(modelId, 4);
    Hu3DModelPosSet(modelId, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrReset(modelId, 1);
    Hu3DModelAttrSet(modelId, 0x4000);
    Hu3DModelCameraSet(modelId, 0xF);
    spVec.y = lbl_1_rodata_374;
    for (i = 0; i < 0xB; i++) {
        if (i != 5) {
            spVec.x = (f32)(0x1388 - (i * 0xA * 0x64));
            spVec.z = lbl_1_rodata_370;
            node = (Mgr511StageNode *)__nw__FUl(0x2C);
            if (node != NULL) {
                fn_1_CE34(node, arg0, &spVec);
            }
            spVec.z = lbl_1_rodata_36C;
            node = (Mgr511StageNode *)__nw__FUl(0x2C);
            if (node != NULL) {
                fn_1_CE34(node, arg0, &spVec);
            }
        }
    }
    for (j = 1; j < 0xA; j++) {
        spVec.x = lbl_1_rodata_370;
        spVec.z = (f32)(0x1388 - (j * 0xA * 0x64));
        node = (Mgr511StageNode *)__nw__FUl(0x2C);
        if (node != NULL) {
            fn_1_CE34(node, arg0, &spVec);
        }
        spVec.x = lbl_1_rodata_36C;
        node = (Mgr511StageNode *)__nw__FUl(0x2C);
        if (node != NULL) {
            fn_1_CE34(node, arg0, &spVec);
        }
    }
    return arg0;
}

void *fn_1_CC3C(void *arg0, s16 arg1)
{
    if (arg0 != NULL) {
        if (lbl_1_bss_18 == NULL) {
            fn_1_CF8("ms_pointer != 0", "singleton.h", 0x3D);
        }
        lbl_1_bss_18 = (Mgr511Stage *)NULL;
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

// NONMATCHING: the vtable pointer and teardown order differ.
void *fn_1_CCC8(void *arg0, s16 arg1)
{
    Mgr511Stage *self = (Mgr511Stage *)arg0;

    if (arg0 != NULL) {
        self->vtable = lbl_1_data_464;
        fn_1_D00C();
        fn_1_3B8C(&self->modelId1C, -1);
        fn_1_3B8C(&self->modelId1A, -1);
        fn_1_3B8C(&self->modelId18, -1);
        fn_1_3B8C(&self->modelId16, -1);
        fn_1_3B8C(&self->modelId14, -1);
        fn_1_3B8C(&self->modelId12, -1);
        fn_1_3B8C(&self->modelId10, -1);
        if (lbl_1_bss_18 == NULL) {
            fn_1_CF8("ms_pointer != 0", "singleton.h", 0x3D);
        }
        lbl_1_bss_18 = (Mgr511Stage *)NULL;
        self->vtable = &lbl_1_data_40[2];
        fn_1_1D94(arg0, 0);
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

void fn_1_CDEC(void)
{
    fn_1_D0E0();
}

void fn_1_CE0C(void *arg0, s32 arg1)
{
    s32 *flag = (s32 *)((u8 *)arg0 + 0xC);

    *flag &= ~(1 << arg1);
}

Mgr511StageNode *fn_1_CE34(Mgr511StageNode *arg0, Mgr511Stage *arg1, M511_VECF *arg2)
{
    s16 modelId;

    fn_1_1D88(arg0);
    arg0->pos.x = arg2->x;
    arg0->pos.y = arg2->y;
    arg0->pos.z = arg2->z;
    fn_1_384C(&arg0->modelId, arg1);
    fn_1_1E54(arg0, (void **)&lbl_1_bss_A84);
    modelId = arg0->modelId;
    Hu3DModelScaleSet(modelId, 1.0f, 1.0f, 1.0f);
    Hu3DModelAttrReset(modelId, 1);
    Hu3DModelLayerSet(modelId, 1);
    Hu3DModelAttrSet(modelId, 0x4000);
    Hu3DModelCameraSet(modelId, 0xF);
    if (lbl_1_bss_1C == 0) {
        fn_1_CF8("ms_pointer != 0", "singleton.h", 0x27);
    }
    arg0->mgr = lbl_1_bss_1C;
    arg0->pos.y = fn_1_1A150(arg0->mgr, &arg0->pos);
    arg0->prev.x = 0.0f;
    arg0->prev.y = 0.0f;
    arg0->prev.z = 0.0f;
    fn_1_398C(&arg0->modelId, &arg0->pos);
    fn_1_398C(&arg0->modelId, &arg0->prev);
    return arg0;
}

void *fn_1_CFB4(void *arg0, s16 arg1)
{
    if (arg0 != NULL) {
        fn_1_1D94(arg0, 0);
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

void fn_1_D00C(void)
{
    Mgr511StageNode *node;

    while (lbl_1_bss_A84.next != (Mgr511StageNode *)&lbl_1_bss_A84 || lbl_1_bss_A84.prev != (Mgr511StageNode *)&lbl_1_bss_A84) {
        node = lbl_1_bss_A84.prev;
        if (node != NULL) {
            fn_1_3B8C(&node->modelId, -1);
            fn_1_1D94(node, 0);
            __dl__FPv(node);
        }
    }
}

void fn_1_D0E0(void)
{
    fn_1_D1A8((Mgr511StageNode **)&lbl_1_bss_A84, fn_1_D110);
}

void fn_1_D110(Mgr511StageNode *arg0)
{
    fn_1_D138(arg0);
}

void fn_1_D138(Mgr511StageNode *arg0)
{
    f32 temp_f1;

    temp_f1 = 0.8f * (fn_1_1A150(arg0->mgr, &arg0->pos) - arg0->pos.y);
    arg0->pos.y = (f32)((f64)arg0->pos.y + (f64)temp_f1);
    fn_1_39D4(&arg0->modelId, &arg0->prev, temp_f1);
    fn_1_398C(&arg0->modelId, &arg0->pos);
}

void fn_1_D1A8(Mgr511StageNode **arg0, void (*arg1)(Mgr511StageNode *))
{
    fn_1_D208(*arg0, arg0, arg1);
}

void fn_1_D208(Mgr511StageNode *arg0, Mgr511StageNode **arg1, void (*arg2)(Mgr511StageNode *))
{
    Mgr511StageNode *node = arg0;

    while (node != (Mgr511StageNode *)arg1) {
        arg2(node);
        node = *(Mgr511StageNode **)node;
    }
}

void fn_1_D268(void)
{
    fn_1_1D88(&lbl_1_bss_A84);
    __register_global_object(&lbl_1_bss_A84, fn_1_D2C0, &lbl_1_bss_A78);
}

void *fn_1_D2C0(void *arg0, s16 arg1)
{
    if (arg0 != NULL) {
        fn_1_1D94(arg0, 0);
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

}

/* The vtables this object defines: their entries are
   functions defined above, so the definitions sit here. */

/* lbl_1_data_464 is a vtable stage.o itself defines (6 words at .data+0x2c), and this unit is linked from source, so it has to be a definition rather than an extern declaration: with only the declaration the REL link reported "Failed to find symbol lbl_1_data_464 in any module". 2 leading words are zero and the rest are ADDR32 relocations to functions in this object. The definition is at the end of the unit because its entries are functions defined further down. */
extern "C" void *lbl_1_data_464[6] = {
    0,
    0,
    (void *) fn_1_CCC8,
    (void *) fn_1_CE0C,
    (void *) fn_1_CDEC,
};

