#include "REL/m510dll.h"
#include "string.h"

// .data:0x0 (absolute 0x320)
HuVecF lbl_1_data_320 = { -50.0f, 0.0f, 0.0f };
// .data:0xC (absolute 0x32C)
HuVecF lbl_1_data_32C = { 0.0f, 75.0f, 0.0f };
// .data:0x338 (absolute 0x658)
HuVecF lbl_1_data_338 = { 0.0f, -41.0f, -23.0f };
// .data:0x344 (absolute 0x664)
s16 lbl_1_data_344[] =
{
    0, 0, 0, 0, 0, 60, 20, 30, 40, 0,
    5, 5, 5, 5, 0, 30, 40, 30, 20, 0
};

// Callback work for fn_1_49F4 (0x3C bytes)
typedef struct
{
    void *f00;          // 0x00: M510_PLAYER_WORK *self
    u16 f04;            // 0x04: state
    s16 f06;            // 0x06: frame counter
    s16 f08;            // 0x08: modelId used by Hu3DModelPosSet
    u8 pad0a[2];        // 0x0a
    s16 f0c;            // 0x0c: modelId used by Hu3DModelRotSet
    f32 f10;            // 0x10: position X
    f32 f14;            // 0x14: position Y
    f32 f18;            // 0x18: position Z
    f32 f1c;            // 0x1c: rotation angle
    s16 f20;            // 0x20: table index
    u8 pad22[2];        // 0x22
    s32 f24;            // 0x24: active flag
    f32 f28;            // 0x28: offset position X
    f32 f2c;            // 0x2c: offset position Y
    f32 f30;            // 0x30: offset position Z
    u8 pad34[4];        // 0x34
    s32 f38;            // 0x38: sound id
} M510_49F4_DATA;

void fn_1_47C4(M510_PITCH_WORK *work, M510_PLAYER_WORK *self)
{
    M510_PLAYER_WORK *work2 = self;
    M510_OBJ *obj;
    M510_OPEN_CAM *cam;
    s16 modelId;

    obj = work2->obj;
    cam = &obj->cam;
    memset(work, 0, 0x3C);
    work->owner = self;

    modelId = Hu3DModelCreate(HuDataSelHeapReadNum(0x440000, HU_MEMNUM_OVL, HEAP_MODEL));
    work->modelId = modelId;
    Hu3DModelPosSet(modelId, lbl_1_data_320.x, lbl_1_data_320.y, lbl_1_data_320.z);
    Hu3DModelRotSet(modelId, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(modelId, cam->unk04 & 0xFFFF);
    Hu3DModelLayerSet(modelId, 0);
    Hu3DModelAttrReset(modelId, 1);
    work->motionId = Hu3DJointMotion(modelId, HuDataSelHeapReadNum(0x440016, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DMotionSet(modelId, work->motionId);
    Hu3DModelAttrSet(modelId, 0x40000000 + 1);

    modelId = Hu3DModelCreate(HuDataSelHeapReadNum(0x440001, HU_MEMNUM_OVL, HEAP_MODEL));
    work->modelId2 = modelId;
    Hu3DModelPosSet(modelId, lbl_1_data_32C.x, lbl_1_data_32C.y, lbl_1_data_32C.z);
    Hu3DModelRotSet(modelId, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(modelId, cam->unk04 & 0xFFFF);
    Hu3DModelLayerSet(modelId, 0);
    Hu3DModelAttrReset(modelId, 1);

    work->state = 2;
    work->unk38 = -1;
}

void fn_1_49A4(M510_PITCH_WORK *work)
{
    Hu3DModelKill(work->modelId2);
    Hu3DMotionKill(work->motionId);
    Hu3DModelKill(work->modelId);
    memset(work, 0, 0x3C);
}

void fn_1_49F4(void *data)
{
    // NONMATCHING: ~36%. Real body, but MWCC allocates fewer callee-saved FP
    // registers (f30/f31) than the target (f23-f31), yielding a different frame
    // (-0x90 vs -0x100); stack-offset/register instructions differ throughout.
    M510_49F4_DATA *w = data;
    M510_PLAYER_WORK *self = w->f00;
    Mtx mtx;
    Vec vec;
    f32 ratio;
    f32 clamped;
    s16 old;

    switch (w->f04)
    {
    case 0:
        if (w->f24 != 0)
        {
            w->f24 = 0;
            w->f06 = 0;
            w->f04 = 1;
        }
        break;

    case 1:
        ((void (*)(void *, void *, s16))(void *)fn_1_52B0)((u8 *)self + 0x154, (u8 *)self + 0x140, w->f20);
        if (self->playerNo == 0)
        {
            w->f38 = fn_1_5B4(0x69b);
        }
        w->f04 = 2;
        break;

    case 2:
    {
        ratio = (float)w->f06 / (float)lbl_1_data_344[w->f20 + 5];
        clamped = ratio;
        if (clamped > 1.0f)
        {
            clamped = 1.0f;
        }
        else if (clamped < 0.0f)
        {
            clamped = 0.0f;
        }
        ratio = clamped;
        w->f1c = 90.0f * ratio;
        old = w->f06;
        w->f06 = old + 1;
        if (old > lbl_1_data_344[w->f20 + 5])
        {
            w->f06 = 0;
            w->f04 = 3;
        }
        break;
    }

    case 3:
        w->f04 = 4;
        if (w->f38 >= 0)
        {
            fn_1_5DC(w->f38);
            w->f38 = -1;
        }
        /* fall through */

    case 4:
    {
        ratio = (float)w->f06 / (float)lbl_1_data_344[w->f20 + 10];
        clamped = ratio;
        if (clamped > 1.0f)
        {
            clamped = 1.0f;
        }
        else if (clamped < 0.0f)
        {
            clamped = 0.0f;
        }
        ratio = clamped;
        w->f1c = 90.0f + 180.0f * ratio;
        old = w->f06;
        w->f06 = old + 1;
        if (old > lbl_1_data_344[w->f20 + 10])
        {
            w->f06 = 0;
            w->f04 = 5;
        }
        break;
    }

    case 5:
    {
        ratio = (float)w->f06 / (float)lbl_1_data_344[w->f20 + 15];
        clamped = ratio;
        if (clamped > 1.0f)
        {
            clamped = 1.0f;
        }
        else if (clamped < 0.0f)
        {
            clamped = 0.0f;
        }
        ratio = clamped;
        w->f1c = 270.0f + 90.0f * ratio;
        old = w->f06;
        w->f06 = old + 1;
        if (old > lbl_1_data_344[w->f20 + 15])
        {
            w->f06 = 0;
            w->f04 = 0;
        }
        break;
    }
    }

    Hu3DModelPosSet(w->f08, w->f10 + lbl_1_data_320.x, w->f14 + lbl_1_data_320.y, w->f18 + lbl_1_data_320.z);
    Hu3DModelRotSet(w->f0c, w->f1c, 0.0f, 0.0f);

    if (w->f1c > 180.0f)
    {
        w->f1c -= 360.0f;
    }
    else if (w->f1c < -180.0f)
    {
        w->f1c += 360.0f;
    }

    PSMTXRotRad(mtx, 'X', w->f1c * 0.017453292f);
    PSMTXMultVec(mtx, &lbl_1_data_338, &vec);
    w->f28 = w->f10 + (vec.x + lbl_1_data_32C.x);
    w->f2c = w->f14 + (vec.y + lbl_1_data_32C.y);
    w->f30 = w->f18 + (vec.z + lbl_1_data_32C.z);
}