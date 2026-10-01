#include "REL/m510dll.h"
#include "string.h"

// Not declared in game/hu3d.h on purpose (see the note there); this unit needs
// the float return.
float Hu3DMotionMaxTimeGet(HU3DMODELID modelId);

// .data:0x0 (absolute 0x110) - 4 x HuVecF
HuVecF lbl_1_data_110[4] = {
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 300.0f },
    { 0.0f, 0.0f, 600.0f },
    { 0.0f, 0.0f, 850.0f },
};

void fn_1_3A64(M510_PLAYER_WORK *work, s16 playerNo, M510_OBJ *obj)
{
    HuVecF pos;
    M510_OBJ *self = obj;
    M510_OPEN_CAM *cam = &self->cam;
        s16 modelId;

    memset(work, 0, sizeof(M510_PLAYER_WORK));
    work->obj = obj;
    work->playerNo = playerNo;

    fn_1_47C4(&work->pitch, work);

    work->pitch.pos = lbl_1_data_110[playerNo];
    work->pitch.pos.z += -250.0f;

    fn_1_509C(&work->ballPathWork, work);
    fn_1_3DC8(&work->model, playerNo, work);
    fn_1_CE04(&work->score, 0, playerNo);

    pos = lbl_1_data_110[playerNo];
    pos.x -= 130.0f;
    pos.z = 1300.0f;
    work->model.camPos = pos;

    CharEffectLayerSet(4);

    modelId = Hu3DModelCreate(HuDataSelHeapReadNum(0x44000E, HU_MEMNUM_OVL, HEAP_MODEL));
    work->modelId = modelId;
    Hu3DModelPosSet(modelId, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(modelId, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(modelId, cam->unk04 & 0xFFFF);
    Hu3DModelLayerSet(modelId, 4);
    Hu3DModelAttrSet(modelId, 1);
    Hu3DModelScaleSet(modelId, 8.0f, 8.0f, 8.0f);

    work->motionId = Hu3DJointMotion(modelId, HuDataSelHeapReadNum(0x44001F, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DMotionSet(modelId, work->motionId);
    Hu3DMotionSpeedSet(modelId, 2.5f);
    Hu3DModelAttrReset(modelId, 0x40000001);
}

void fn_1_3CCC(M510_PLAYER_WORK *work)
{
    Hu3DMotionKill(work->motionId);
    Hu3DModelKill(work->modelId);
    fn_1_D1AC(&work->score);
    fn_1_4284(&work->model);
    fn_1_51B4(&work->ballPathWork);
    fn_1_49A4(&work->pitch);
    memset(work, 0, sizeof(M510_PLAYER_WORK));
}

s32 fn_1_3D34(M510_PLAYER_WORK *work)
{
    float maxTime;

    fn_1_49F4(&work->pitch);
    fn_1_4328(&work->model);
    fn_1_D234(&work->score);
    fn_1_5230(&work->ballPathWork);

    if (work->flag != 0) {
        maxTime = Hu3DMotionMaxTimeGet(work->modelId);
        if (Hu3DMotionTimeGet(work->modelId) >= maxTime) {
            Hu3DModelAttrSet(work->modelId, 1);
            work->flag = 0;
        }
    }
    return 0;
}
