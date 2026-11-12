// m550dll/m550.c - decompilation of the m550dll/m550 unit.

/* Suppress MSL's `extern inline sqrtf` body: without this, every object that
   reaches humath.h (through REL/m550dll.h -> game/hu3d.h) carries 16 bytes of
   duplicate `.rodata` (the 0.5/3.0 sqrtf statics). */
#define _MATH_H

#include "types.h"

#include "game/mg/seqman.h"
#include "game/mg/timer.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/wipe.h"

#include "REL/m550dll.h"

typedef void (*M550_VOIDFN)(void);
extern const M550_VOIDFN _ctors[];
extern const M550_VOIDFN _dtors[];

void ObjectSetup(void);
void fn_1_198(s16 mode, s16 frameNo);
void fn_1_6B4(s16 mode, s16 frameNo);
void fn_1_6F8(s16 mode, s16 frameNo);
void fn_1_750(s16 mode, s16 frameNo);
void fn_1_794(s16 mode, s16 frameNo);
void fn_1_7FC(s16 mode, s16 frameNo);
void fn_1_840(s16 mode, s16 frameNo);
void fn_1_8A0(s16 mode, s16 frameNo);
void fn_1_910(s16 mode, s16 frameNo);
void fn_1_914(OMOBJ *obj);
void fn_1_D64(void);
void fn_1_131C(void);

void fn_1_1C84(void);
void fn_1_1D88(void);
void fn_1_1DCC(void);
void fn_1_1E38(void);
void fn_1_235C(void);
void fn_1_2360(void);
void fn_1_2364(void);
void fn_1_2618(void);
void fn_1_2670(void);
void fn_1_2D98(void);
void fn_1_3088(void);
void fn_1_3174(void);
void fn_1_3324(void);
void fn_1_4E8C(M550STATE *state, M550MODEL *model, s32 *modelData);
void fn_1_50D8(s16 stageNo);
void fn_1_58F4(M550MOTIONSET *p0, M550MOTIONSET *p1);
void fn_1_5918(s16 *p0, s16 *p1);
void fn_1_593C(void);
void fn_1_5BD8(OMOBJ *obj);
void fn_1_6094(void);
void fn_1_61E8(OMOBJ *obj);
void fn_1_65AC(OMOBJ *obj);

/* lbl_1_bss_0 .. lbl_1_bss_14AC, in the original order. */
MGTIMER *lbl_1_bss_0;
M550CPU lbl_1_bss_4[2];
s16 lbl_1_bss_24[10];
OMCAMERAVIEW lbl_1_bss_38[4];
M550MODEL lbl_1_bss_A8[0xB7];
M550STATE lbl_1_bss_14AC;

s32 lbl_1_data_0[2] = { 0x6C003C, 0x6C003D };

s32 lbl_1_data_8[0x3C] = {
    0x6C0000, 0x6C0001, 0x6C0002, 0x6C0003, 0x6C0004, 0x6C0005, 0x6C0006, 0x6C0007,
    0x6C0008, 0x6C0009, 0x6C000A, 0x6C000B, 0x6C000C, 0x6C000D, 0x6C000E, 0x6C000F,
    0x6C0010, 0x6C0011, 0x6C0012, 0x6C0013, 0x6C0014, 0x6C0015, 0x6C0016, 0x6C0017,
    0x6C0018, 0x6C0019, 0x6C001A, 0x6C001B, 0x6C001C, 0x6C001D, 0x6C001E, 0x6C001F,
    0x6C0020, 0x6C0021, 0x6C0022, 0x6C0023, 0x6C0024, 0x6C0025, 0x6C0026, 0x6C0027,
    0x6C0028, 0x6C0029, 0x6C002A, 0x6C002B, 0x6C002C, 0x6C002D, 0x6C002E, 0x6C002F,
    0x6C0030, 0x6C0031, 0x6C0032, 0x6C0033, 0x6C0034, 0x6C0035, 0x6C0036, 0x6C0037,
    0x6C0038, 0x6C0039, 0x6C003A, 0x6C003B,
};

M550MOTIONSET lbl_1_data_F8[13] = {
    { { 0x8700AC, 0x8700AD, 0x8C0060, 0x8C0080, 0x8C007D } },
    { { 0x2E00AC, 0x2E00AD, 0x330060, 0x330080, 0x33007D } },
    { { 0xB200AC, 0xB200AD, 0xB70060, 0xB70080, 0xB7007D } },
    { { 0xE800AC, 0xE800AD, 0xED0060, 0xED0080, 0xED007D } },
    { { 0xE100AC, 0xE100AD, 0xE60060, 0xE60080, 0xE6007D } },
    { { 0x1200AC, 0x1200AD, 0x170060, 0x170080, 0x17007D } },
    { { 0xDB00AC, 0xDB00AD, 0xE00060, 0xE00080, 0xE0007D } },
    { { 0x2800AC, 0x2800AD, 0x2D0060, 0x2D0080, 0x2D007D } },
    { { 0xC400AC, 0xC400AD, 0xC90060, 0xC90080, 0xC9007D } },
    { { 0x9C00AC, 0x9C00AD, 0xB00060, 0xB00080, 0xB0007D } },
    { { 0x9C00AC, 0x9C00AD, 0xB00060, 0xB00080, 0xB0007D } },
    { { 0x9C00AC, 0x9C00AD, 0xB00060, 0xB00080, 0xB0007D } },
    { { 0x9C00AC, 0x9C00AD, 0xB00060, 0xB00080, 0xB0007D } },
};

MGSEQ_PARAM lbl_1_data_1FC = {
    0,
    0,
    fn_1_198,
    fn_1_6B4,
    fn_1_6F8,
    fn_1_750,
    fn_1_794,
    fn_1_7FC,
    fn_1_840,
    fn_1_8A0,
    fn_1_910,
};

/* The mode handler tables. The mode field selects the entry. */
M550_VOIDFN lbl_1_data_224[3] = { fn_1_1C84, fn_1_1D88, fn_1_1DCC };
M550_VOIDFN lbl_1_data_230[3] = { fn_1_1E38, fn_1_235C, fn_1_2360 };
M550_VOIDFN lbl_1_data_23C[5] = { fn_1_2670, fn_1_2D98, fn_1_3088, fn_1_3174, fn_1_3324 };

int _prolog(void)
{
    const M550_VOIDFN *ctors = _ctors;

    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const M550_VOIDFN *dtors = _dtors;

    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

void ObjectSetup(void)
{
    s16 i;

    for (i = 0; i < 0xB7; i++) {
        lbl_1_bss_A8[i].id = -1;
    }
    fn_1_4E8C(&lbl_1_bss_14AC, lbl_1_bss_A8, lbl_1_data_8);
    lbl_1_bss_14AC.player[0].objman = omInitObjMan(0x32, 0x2000);
    omGameSysInit(lbl_1_bss_14AC.player[0].objman);
    omAddObjEx(lbl_1_bss_14AC.player[0].objman, 0x7FDA, 0, 0, -1, omOutView);
    omAddObjEx(lbl_1_bss_14AC.player[0].objman, 0x7FDA, 0, 0, -1, fn_1_5BD8);
    MgSeqCreate(&lbl_1_data_1FC);
}

void fn_1_198(s16 mode, s16 frameNo)
{
    HuVecF shadowPos;
    HuVecF shadowUp;
    HuVecF shadowTarget;
    s16 light;

    fn_1_50D8(2);
    fn_1_58F4(&lbl_1_data_F8[lbl_1_bss_14AC.player[0].pno], &lbl_1_data_F8[lbl_1_bss_14AC.player[1].pno]);
    fn_1_5918(&lbl_1_bss_24[0], &lbl_1_bss_24[5]);
    fn_1_593C();
    fn_1_6094();
    omAddObjEx(lbl_1_bss_14AC.player[0].objman, 0x7FDA, 0, 0, -1, fn_1_61E8);
    fn_1_131C();
    Center.x = 0.0f;
    Center.y = 1000.0f;
    Center.z = 100.0f;
    CRot.x = -90.0f;
    CRot.y = 0.0f;
    CRot.z = 0.0f;
    CZoom = 4000.0f;
    lbl_1_bss_38[1].center.x = 0.0f;
    lbl_1_bss_38[1].center.y = 1150.0f;
    lbl_1_bss_38[1].center.z = 0.0f;
    lbl_1_bss_38[1].rot.x = -10.0f;
    lbl_1_bss_38[1].rot.y = 0.0f;
    lbl_1_bss_38[1].rot.z = 0.0f;
    lbl_1_bss_38[1].zoom = 700.0f;
    Hu3DCameraCreate(1);
    Hu3DCameraPerspectiveSet(1, 45.0f, 20.0f, 100000.0f, 1.2f);
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(1, 0, 0, 640, 480);
    light = Hu3DGLightCreate(0.0f, 1000.0f, 1000.0f, 0.0f, -1.0f, -1.0f, 255, 255, 255);
    Hu3DGLightInfinitytSet(light);
    Hu3DGLightStaticSet(light, 1);
    omAddObjEx(lbl_1_bss_14AC.player[0].objman, 0x7FDA, 0, 0, -1, fn_1_914);
    Hu3DBGColorSet(0, 0, 0);
    Hu3DShadowCreate(45.0f, 20.0f, 15000.0f);
    shadowPos.x = 0.0f;
    shadowPos.y = 1200.0f;
    shadowPos.z = 600.0f;
    shadowUp.x = 0.0f;
    shadowUp.y = 1.0f;
    shadowUp.z = 0.0f;
    shadowTarget.x = 0.0f;
    shadowTarget.y = 0.0f;
    shadowTarget.z = 0.0f;
    Hu3DShadowTPLvlSet(0.5f);
    Hu3DShadowPosSet(&shadowPos, &shadowUp, &shadowTarget);
    lbl_1_bss_14AC.motion[0].motNo = 0;
    lbl_1_bss_14AC.motion[0].speed = 1.0f;
    lbl_1_bss_14AC.motion[0].time = 0.0f;
    lbl_1_bss_14AC.motion[0].attrSet = 0x40000001;
    lbl_1_bss_14AC.motion[1].motNo = 0;
    lbl_1_bss_14AC.motion[1].speed = 1.0f;
    lbl_1_bss_14AC.motion[1].time = 0.0f;
    lbl_1_bss_14AC.motion[1].attrSet = 0x40000001;
    Hu3DFogClear();
    MgSeqModeDelaySet(mode + 1, -1);
    omAddObjEx(lbl_1_bss_14AC.player[0].objman, 0x7FDA, 0, 0, -1, fn_1_65AC);
    MgSeqModeNext();
}

void fn_1_6B4(s16 mode, s16 frameNo)
{
    lbl_1_data_224[lbl_1_bss_14AC.modes[0]]();
}

void fn_1_6F8(s16 mode, s16 frameNo)
{
    lbl_1_bss_14AC.modes[0] = 0;
    lbl_1_bss_14AC.modes[1] = 0;
    MgSeqModeDelaySet(mode + 1, -1);
    fn_1_D64();
}

void fn_1_750(s16 mode, s16 frameNo)
{
    lbl_1_data_230[lbl_1_bss_14AC.modes[0]]();
}

void fn_1_794(s16 mode, s16 frameNo)
{
    MgSeqModeDelaySet(mode + 1, -1);
    if (lbl_1_bss_14AC.modes[0] != 0) {
        fn_1_2618();
    } else {
        fn_1_2364();
    }
    MgSeqModeDelaySet(mode + 1, -1);
}

void fn_1_7FC(s16 mode, s16 frameNo)
{
    lbl_1_data_23C[lbl_1_bss_14AC.modes[0]]();
}

void fn_1_840(s16 mode, s16 frameNo)
{
    if (lbl_1_bss_14AC.modes[0] != 0) {
        CZoom += 5.0f;
    }
    fn_1_D64();
    MgSeqModeNext();
}

void fn_1_8A0(s16 mode, s16 frameNo)
{
    if (lbl_1_bss_14AC.modes[0] != 0) {
        CZoom += 5.0f;
    }
    if (WipeCheckEnd() == 0) {
        MgSeqModeNext();
    }
    fn_1_D64();
}

void fn_1_910(s16 mode, s16 frameNo) { }

void fn_1_914(OMOBJ *obj)
{
    M550MODEL *model;
    s32 i;

    model = lbl_1_bss_A8;
    for (i = 0; i < 0xB7; i++, model++) {
        if (model->id >= 0) {
            Hu3DModelPosSetV(model->id, &model->pos);
            Hu3DModelRotSetV(model->id, &model->rot);
        }
    }
}

void fn_1_984(s32 idx, f32 alpha)
{
    HU3DMODEL *modelP;
    HSFMATERIAL *material;
    s32 i;
    HSFDATA *hsf;

    modelP = &Hu3DData[lbl_1_bss_14AC.model[idx].id];
    hsf = modelP->hsf;
    material = hsf->material;
    for (i = 0; i < modelP->hsf->materialNum; i++, material++) {
        material->invAlpha = alpha;
    }
}

void fn_1_A00(s32 side, HuVecF *pos, HuVecF *rot)
{
    f32 x, y, z;
    s32 i;
    M550MODEL *model;

    x = pos->x;
    y = pos->y;
    z = pos->z;
    for (i = 0; i < 0x1E; i++) {
        model = &lbl_1_bss_A8[6 + side * 0x1E + i];
        if (model->id != 0) {
            Hu3DModelPosSet(model->id, x, y, z);
            model->pos.x = x;
            model->pos.y = y;
            model->pos.z = z;
            model->rot.x = rot->x;
            model->rot.y = rot->y;
            model->rot.z = rot->z;
        }
    }
    for (i = 0; i < 0xB; i++) {
        model = &lbl_1_bss_A8[0x7E + side * 0xB + i];
        if (model->id != 0) {
            Hu3DModelPosSet(model->id, x, y, z);
            model->pos.x = x;
            model->pos.y = y;
            model->pos.z = z;
            model->rot.x = rot->x;
            model->rot.y = rot->y;
            model->rot.z = rot->z;
        }
    }
}

void fn_1_B5C(s32 side)
{
    Mtx mtx;
    Mtx idMtx;
    HuVecF pos;
    HuVecF rot;
    f32 z;
    f32 y;
    f32 x;
    s32 i;
    M550MODEL *model;

    PSMTXIdentity(idMtx);
    Hu3DModelObjPosGet(lbl_1_bss_14AC.model[side + 2].id, CharModelItemHookGet(lbl_1_bss_14AC.player[side].charNo, 1, 4), &pos);
    Hu3DModelObjMtxGet(lbl_1_bss_14AC.model[side + 2].id, CharModelItemHookGet(lbl_1_bss_14AC.player[side].charNo, 1, 4), mtx);
    PSMTXConcat(mtx, idMtx, mtx);
    Hu3DMtxRotGet(mtx, &rot);
    x = pos.x;
    y = pos.y;
    z = pos.z;
    for (i = 0; i < 0x1E; i++) {
        model = &lbl_1_bss_A8[6 + side * 0x1E + i];
        if (model->id != 0) {
            Hu3DModelPosSet(model->id, x, y, z);
            model->pos.x = x;
            model->pos.y = y;
            model->pos.z = z;
            model->rot.x = rot.x;
            model->rot.y = rot.y;
            model->rot.z = rot.z;
        }
    }
    for (i = 0; i < 0xB; i++) {
        model = &lbl_1_bss_A8[0x7E + side * 0xB + i];
        if (model->id != 0) {
            Hu3DModelPosSet(model->id, x, y, z);
            model->pos.x = x;
            model->pos.y = y;
            model->pos.z = z;
            model->rot.x = rot.x;
            model->rot.y = rot.y;
            model->rot.z = rot.z;
        }
    }
}

void fn_1_D64(void)
{
    Mtx idMtx;
    Mtx mtx;
    HuVecF rot;
    HuVecF pos;
    f32 z;
    f32 y;
    f32 x;
    s32 side;
    s32 i;
    M550MODEL *model;

    for (side = 0; side < 2; side++) {
        PSMTXIdentity(idMtx);
        Hu3DModelObjPosGet(lbl_1_bss_14AC.model[side + 2].id, CharModelItemHookGet(lbl_1_bss_14AC.player[side].charNo, 1, 4), &pos);
        Hu3DModelObjMtxGet(lbl_1_bss_14AC.model[side + 2].id, CharModelItemHookGet(lbl_1_bss_14AC.player[side].charNo, 1, 4), mtx);
        PSMTXConcat(mtx, idMtx, mtx);
        Hu3DMtxRotGet(mtx, &rot);
        x = pos.x;
        y = pos.y;
        z = pos.z;
        for (i = 0; i < 0x1E; i++) {
            model = &lbl_1_bss_A8[6 + side * 0x1E + i];
            if (model->id != 0) {
                Hu3DModelPosSet(model->id, x, y, z);
                model->pos.x = x;
                model->pos.y = y;
                model->pos.z = z;
                model->rot.x = rot.x;
                model->rot.y = rot.y;
                model->rot.z = rot.z;
            }
        }
        for (i = 0; i < 0xB; i++) {
            model = &lbl_1_bss_A8[0x7E + side * 0xB + i];
            if (model->id != 0) {
                Hu3DModelPosSet(model->id, x, y, z);
                model->pos.x = x;
                model->pos.y = y;
                model->pos.z = z;
                model->rot.x = rot.x;
                model->rot.y = rot.y;
                model->rot.z = rot.z;
            }
        }
    }
}

void fn_1_F7C(void)
{
    Mtx mtx;
    Mtx idMtx;
    HuVecF pos;
    HuVecF rot;
    s32 i;

    for (i = 0; i < 2; i++) {
        PSMTXIdentity(idMtx);
        Hu3DModelObjPosGet(lbl_1_bss_14AC.model[i + 2].id, CharModelItemHookGet(lbl_1_bss_14AC.player[i].charNo, 1, 4), &pos);
        Hu3DModelObjMtxGet(lbl_1_bss_14AC.model[i + 2].id, CharModelItemHookGet(lbl_1_bss_14AC.player[i].charNo, 1, 4), mtx);
        PSMTXConcat(mtx, idMtx, mtx);
        Hu3DMtxRotGet(mtx, &rot);
        lbl_1_bss_14AC.fxVec[i * 2] = pos;
        lbl_1_bss_14AC.fxVec[i * 2 + 1] = rot;
    }
}

void fn_1_10BC(s32 side)
{
    HuVecF pos;
    M550MODEL *model;

    Hu3DModelObjPosGet(lbl_1_bss_14AC.model[side + 2].id, CharModelItemHookGet(lbl_1_bss_14AC.player[side].charNo, 1, 4), &pos);
    model = &lbl_1_bss_A8[side + 0x95];
    Hu3DModelAttrReset(model->id, 1);
    Hu3DMotionSpeedSet(model->id, 3.0f);
    pos.z += 80.0f;
    pos.x += (f32)(0x1E - side * 0x3C);
    pos.y += 20.0f;
    model->pos = pos;
    Hu3DModelPosSetV(model->id, &model->pos);
}

void fn_1_11F0(M550MODEL *model)
{
    Hu3DModelAttrSet(model->id, 1);
    Hu3DModelPosSet(model->id, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(model->id, 0.0f, 0.0f, 0.0f);
    model->pos.x = 0.0f;
    model->pos.y = 0.0f;
    model->pos.z = 0.0f;
    model->rot.x = 0.0f;
    model->rot.y = 0.0f;
    model->rot.z = 0.0f;
    Hu3DModelCameraSet(model->id, 1);
    Hu3DMotionTimeSet(model->id, 0.0f);
    Hu3DMotionSpeedSet(model->id, 0.0f);
    Hu3DModelAttrReset(model->id, 1);
}

void fn_1_131C(void)
{
    s32 side;
    s32 i;
    M550MODEL *model;

    for (side = 0; side < 2; side++) {
        for (i = 0; i < 0x1E; i++) {
            model = &lbl_1_bss_A8[6 + side * 0x1E + i];
            model->id = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_8[i + 2], HU_MEMNUM_OVL, HEAP_MODEL));
            fn_1_11F0(model);
        }
        for (i = 0; i < 0x1E; i++) {
            model = &lbl_1_bss_A8[0x42 + side * 0x1E + i];
            model->id = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_8[0x20], HU_MEMNUM_OVL, HEAP_MODEL));
            fn_1_11F0(model);
            Hu3DMotionTimeSet(model->id, 0.0f);
            Hu3DMotionSpeedSet(model->id, 1.0f);
            Hu3DModelAttrSet(model->id, 1);
            Hu3DModelLayerSet(model->id, 3);
        }
        for (i = 0; i < 0xB; i++) {
            model = &lbl_1_bss_A8[0x7E + side * 0xB + i];
            model->id = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_8[i + 0x21], HU_MEMNUM_OVL, HEAP_MODEL));
            fn_1_11F0(model);
            Hu3DMotionTimeSet(model->id, 0.0f);
            Hu3DMotionSpeedSet(model->id, 0.0f);
            Hu3DModelLayerSet(model->id, 2);
        }
        model = &lbl_1_bss_A8[side + 4];
        model->id = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_8[0x2C], HU_MEMNUM_OVL, HEAP_MODEL));
        fn_1_11F0(model);
        Hu3DMotionTimeSet(model->id, 0.0f);
        Hu3DModelLayerSet(model->id, 5);
        Hu3DMotionSpeedSet(model->id, 0.0f);
        Hu3DModelHookSet(lbl_1_bss_A8[side + 2].id, CharModelItemHookGet(lbl_1_bss_14AC.player[side].charNo, 1, 0), model->id);
        model = &lbl_1_bss_A8[side + 0x95];
        model->id = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_8[0x2E], HU_MEMNUM_OVL, HEAP_MODEL));
        fn_1_11F0(model);
        Hu3DMotionTimeSet(model->id, 0.0f);
        Hu3DModelLayerSet(model->id, 4);
        Hu3DMotionSpeedSet(model->id, 0.0f);
        Hu3DModelAttrSet(model->id, 1);
    }
    model = &lbl_1_bss_A8[0x94];
    model->id = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_8[0x2D], HU_MEMNUM_OVL, HEAP_MODEL));
    fn_1_11F0(model);
    Hu3DMotionTimeSet(model->id, 0.0f);
    Hu3DMotionSpeedSet(model->id, 1.0f);
    Hu3DModelLayerSet(model->id, 4);
    Hu3DModelAttrSet(model->id, 1);
}
