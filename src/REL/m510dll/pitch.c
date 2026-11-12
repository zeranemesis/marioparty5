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
    work->soundId = -1;
}

void fn_1_49A4(M510_PITCH_WORK *work)
{
    Hu3DModelKill(work->modelId2);
    Hu3DMotionKill(work->motionId);
    Hu3DModelKill(work->modelId);
    memset(work, 0, 0x3C);
}

void fn_1_49F4(M510_PITCH_WORK *data)
{
    // NONMATCHING: ~36%. Real body, but MWCC allocates fewer callee-saved FP
    // registers (f30/f31) than the target (f23-f31), yielding a different frame
    // (-0x90 vs -0x100); stack-offset/register instructions differ throughout.
    M510_PITCH_WORK *w = data;
    M510_PLAYER_WORK *self = w->owner;
    Mtx mtx;
    Vec vec;
    f32 ratio;
    f32 clamped;
    s16 old;

    switch (w->state)
    {
    case 0:
        if (w->active != 0)
        {
            w->active = 0;
            w->timer = 0;
            w->state = 1;
        }
        break;

    case 1:
        fn_1_52B0(&self->ballWork, &w->ofs, w->index);
        if (self->playerNo == 0)
        {
            w->soundId = fn_1_5B4(0x69b);
        }
        w->state = 2;
        break;

    case 2:
    {
        ratio = (float)w->timer / (float)lbl_1_data_344[w->index + 5];
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
        w->rot = 90.0f * ratio;
        old = w->timer;
        w->timer = old + 1;
        if (old > lbl_1_data_344[w->index + 5])
        {
            w->timer = 0;
            w->state = 3;
        }
        break;
    }

    case 3:
        w->state = 4;
        if (w->soundId >= 0)
        {
            fn_1_5DC(w->soundId);
            w->soundId = -1;
        }
        /* fall through */

    case 4:
    {
        ratio = (float)w->timer / (float)lbl_1_data_344[w->index + 10];
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
        w->rot = 90.0f + 180.0f * ratio;
        old = w->timer;
        w->timer = old + 1;
        if (old > lbl_1_data_344[w->index + 10])
        {
            w->timer = 0;
            w->state = 5;
        }
        break;
    }

    case 5:
    {
        ratio = (float)w->timer / (float)lbl_1_data_344[w->index + 15];
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
        w->rot = 270.0f + 90.0f * ratio;
        old = w->timer;
        w->timer = old + 1;
        if (old > lbl_1_data_344[w->index + 15])
        {
            w->timer = 0;
            w->state = 0;
        }
        break;
    }
    }

    Hu3DModelPosSet(w->modelId, w->pos.x + lbl_1_data_320.x, w->pos.y + lbl_1_data_320.y, w->pos.z + lbl_1_data_320.z);
    Hu3DModelRotSet(w->modelId2, w->rot, 0.0f, 0.0f);

    if (w->rot > 180.0f)
    {
        w->rot -= 360.0f;
    }
    else if (w->rot < -180.0f)
    {
        w->rot += 360.0f;
    }

    PSMTXRotRad(mtx, 'X', w->rot * 0.017453292f);
    PSMTXMultVec(mtx, &lbl_1_data_338, &vec);
    w->ofs.x = w->pos.x + (vec.x + lbl_1_data_32C.x);
    w->ofs.y = w->pos.y + (vec.y + lbl_1_data_32C.y);
    w->ofs.z = w->pos.z + (vec.z + lbl_1_data_32C.z);
}