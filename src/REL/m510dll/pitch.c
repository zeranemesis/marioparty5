#include "REL/m510dll.h"
#include "string.h"

// .data:0x0 (absolute 0x320)
HuVecF lbl_1_data_320 = { -50.0f, 0.0f, 0.0f };
// .data:0xC (absolute 0x32C)
HuVecF lbl_1_data_32C = { 0.0f, 75.0f, 0.0f };

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
}