#include "REL/m510dll.h"
#include "string.h"

// Not declared in game/hu3d.h on purpose (see the note there); this unit needs
// the float return.
float Hu3DMotionMaxTimeGet(HU3DMODELID modelId);

#define MOT_ROW(c) { ((c) << 16) | 0, ((c) << 16) | 1, ((c) << 16) | 2, ((c) << 16) | 0x72, ((c) << 16) | 0x73, ((c) << 16) | 6, ((c) << 16) | 0x31 }

// .data:0x0 (absolute 0x140) - motion data numbers of each character
s32 lbl_1_data_140[13][7] = {
    MOT_ROW(0x8C), MOT_ROW(0x33), MOT_ROW(0xB7), MOT_ROW(0xED), MOT_ROW(0xE6), MOT_ROW(0x17), MOT_ROW(0xE0),
    MOT_ROW(0x2D), MOT_ROW(0xC9), MOT_ROW(0xB0), MOT_ROW(0x8C), MOT_ROW(0x8C), MOT_ROW(0x8C),
};

// .data:0x16C (absolute 0x2AC) - rotation while swinging forwards
f32 lbl_1_data_2AC[16] = {
    -113.176f, -88.14909f, -52.90621f, -16.67476f, 26.3934f, 69.22196f, 101.1403f, 127.9546f,
    151.4606f, 171.4761f, -172.7292f, -161.7649f, -155.779f, -155.779f, -155.779f, -155.779f,
};

// .data:0x1AC (absolute 0x2EC) - rotation while swinging back
f32 lbl_1_data_2EC[12] = {
    -154.6163f, -159.2135f, -172.7292f, 165.2307f, 136.154f, 101.1403f,
    55.91046f, -3.053231f, -52.90621f, -100.0638f, -100.0638f, -100.0638f,
};

void fn_1_3DC8(M510_PLAYER_MODEL_WORK *data, s16 playerNo, M510_PLAYER_WORK *owner)
{
    M510_OBJ *obj = owner->obj;
    M510_OPEN_CAM *cam = &obj->cam;
    OMOBJ *omobj;
    s16 modelId;
    u16 i;
    M510_VECF vec;

    memset(data, 0, sizeof(M510_PLAYER_MODEL_WORK));
    data->owner = owner;
    data->playerNo = playerNo;
    data->grp = GwPlayerConf[data->playerNo].grp;
    data->charNo = GwPlayerConf[data->playerNo].charNo;

    omobj = omAddObjEx(obj->objman, 0x64, 1, 7, -1, NULL);
    data->obj = omobj;
    data->padOn = 0;
    modelId = CharModelCreate(data->charNo, 1);
    data->modelId = modelId;
    omobj->mdlId[0] = modelId;
    Hu3DModelShadowSet(modelId);
    CharModelStepFxSet(data->charNo, 0);
    CharEffectLayerSet(4);
    for (i = 0; i < 7; i++) {
        omobj->mtnId[i] = CharMotionCreate(data->charNo, lbl_1_data_140[data->charNo][i]);
        CharMotionSet(data->charNo, omobj->mtnId[i]);
    }
    Hu3DModelAttrSet(modelId, 1);
    Hu3DModelAttrSet(modelId, 0x40000001);
    Hu3DModelLayerSet(modelId, 3);
    Hu3DMotionSet(modelId, omobj->mtnId[0]);
    Hu3DModelCameraSet(modelId, cam->unk04 & 0xFFFF);
    Hu3DMotionShiftSet(modelId, data->obj->mtnId[3], 0.0f, 4.0f, 0);
    data->motion = 3;
    data->nextMotion = 3;
    data->loopF = 1;
    data->modelPos = &Hu3DData[modelId].pos;
    data->modelRot = &Hu3DData[modelId].rot;
    vec.x = 0.0f;
    vec.y = 0.0f;
    vec.z = 0.0f;
    data->camPos = vec;
    vec.x = 0.0f;
    vec.y = 180.0f;
    vec.z = 0.0f;
    data->camRot = vec;
    omSetTra(omobj, data->camPos.x, data->camPos.y, data->camPos.z);
    omSetRot(omobj, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrReset(modelId, 1);

    modelId = Hu3DModelCreate(HuDataSelHeapReadNum(0x440002, HU_MEMNUM_OVL, HEAP_MODEL));
    data->itemModelId = modelId;
    Hu3DModelPosSet(modelId, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(modelId, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(modelId, cam->unk04 & 0xFFFF);
    Hu3DModelLayerSet(modelId, 3);
    Hu3DModelAttrReset(modelId, 1);
    Hu3DModelShadowSet(modelId);
    Hu3DModelHookSet(data->modelId, CharModelItemHookGet(data->charNo, 1, 1), modelId);

    modelId = Hu3DModelCreate(HuDataSelHeapReadNum(0x44000D, HU_MEMNUM_OVL, HEAP_MODEL));
    data->effModelId = modelId;
    Hu3DModelPosSet(modelId, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(modelId, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(modelId, cam->unk04 & 0xFFFF);
    Hu3DModelLayerSet(modelId, 3);
    Hu3DModelAttrSet(modelId, 1);
    data->effMotionId = Hu3DJointMotion(modelId, HuDataSelHeapReadNum(0x44001E, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DMotionSet(modelId, data->effMotionId);
    Hu3DModelAttrReset(modelId, 0x40000001);
    fn_1_8184(&data->pad, playerNo, data);
}

void fn_1_4284(M510_PLAYER_MODEL_WORK *data)
{
    M510_OBJ *obj = data->owner->obj;

    Hu3DMotionKill(data->effMotionId);
    Hu3DModelKill(data->effModelId);
    Hu3DModelHookReset(data->modelId);
    Hu3DModelKill(data->itemModelId);
    fn_1_81F4(&data->pad);
    if (data->obj != NULL) {
        CharModelKill(data->charNo);
        omDelObjEx(obj->objman, data->obj);
        data->obj = NULL;
    }
    memset(data, 0, sizeof(M510_PLAYER_MODEL_WORK));
}

s32 fn_1_4328(M510_PLAYER_MODEL_WORK *data)
{
    M510_PAD *pad = &data->pad;
    f32 maxTime;
    s16 modelId;
    u32 attr;

    if (data->owner->obj->mode >= 7) {
        return 0;
    }
    pad->stkY = 0;
    pad->stkX = 0;
    pad->btnDown = 0;
    pad->btn = 0;
    if (data->padOn != 0) {
        fn_1_8234(pad);
    }

    switch (data->state) {
    case 0:
        if ((pad->btnDown & 0x100) != 0 || data->flag != 0) {
            data->nextMotion = 4;
            data->motion = 4;
            Hu3DModelAttrReset(data->modelId, 0x40000001);
            Hu3DMotionSet(data->modelId, data->obj->mtnId[4]);
            Hu3DMotionSpeedSet(data->modelId, 1.5f);
            switch (data->playerNo) {
            case 0:
                fn_1_5B4(0x693);
                break;
            case 1:
                fn_1_5B4(0x694);
                break;
            case 2:
                fn_1_5B4(0x695);
                break;
            default:
                fn_1_5B4(0x696);
                break;
            }
            data->loopF = 0;
            data->timer = 0;
            data->state = 1;
            data->flag = 0;
        }
        break;
    case 1:
        fn_1_4770(data);
        data->timer++;
        maxTime = Hu3DMotionMaxTimeGet(data->modelId);
        if (Hu3DMotionTimeGet(data->modelId) >= maxTime) {
            Hu3DMotionSpeedSet(data->modelId, 2.0f);
            Hu3DModelAttrSet(data->modelId, 0x40000004);
            data->timer = 0;
            data->state = 2;
        }
        break;
    case 2:
        fn_1_4770(data);
        data->timer++;
        if (Hu3DMotionTimeGet(data->modelId) <= 0.0f) {
            data->nextMotion = 3;
            Hu3DMotionSpeedSet(data->modelId, 1.0f);
            Hu3DMotionSet(data->modelId, data->obj->mtnId[3]);
            Hu3DModelAttrReset(data->modelId, 0x40000004);
            data->prevRotY = 0.0f;
            data->rotY = 0.0f;
            data->loopF = 1;
            data->timer = 0;
            data->state = 0;
        }
        break;
    case 3:
        if (data->timer-- <= 0) {
            data->timer = 0;
            data->state = 0;
        }
        break;
    }

    switch (data->playerNo) {
    case 0:
        CharModelVoicePanSet(data->charNo, 0x28);
        break;
    case 1:
        CharModelVoicePanSet(data->charNo, 0x38);
        break;
    case 2:
        CharModelVoicePanSet(data->charNo, 0x48);
        break;
    default:
        CharModelVoicePanSet(data->charNo, 0x58);
        break;
    }

    modelId = data->modelId;
    if (data->motion != data->nextMotion && Hu3DMotionShiftIDGet(modelId) < 0) {
        data->motion = data->nextMotion;
        Hu3DModelAttrReset(modelId, 0x40000001);
        if (data->loopF != 0) {
            attr = 0x40000001;
        } else {
            attr = 0;
        }
        Hu3DMotionShiftSet(modelId, data->obj->mtnId[data->motion], 0.0f, 4.0f, attr);
    }
    omSetTra(data->obj, data->camPos.x, data->camPos.y, data->camPos.z);
    omSetRot(data->obj, data->camRot.x, data->camRot.y, data->camRot.z);
    if (data->effOn != 0) {
        maxTime = Hu3DMotionMaxTimeGet(data->effModelId);
        if (Hu3DMotionTimeGet(data->effModelId) >= maxTime) {
            Hu3DModelAttrSet(data->effModelId, 1);
            data->effOn = 0;
        }
    }
    return 0;
}

void fn_1_4770(M510_PLAYER_MODEL_WORK *data)
{
    data->prevRotY = data->rotY;
    if (data->state == 1) {
#ifdef TARGET_PC
        data->rotY = lbl_1_data_2AC[(data->timer < 0) ? 0 : (data->timer > 15) ? 15 : data->timer];
#else
        data->rotY = lbl_1_data_2AC[data->timer];
#endif
        return;
    }
#ifdef TARGET_PC
    data->rotY = lbl_1_data_2EC[(data->timer < 0) ? 0 : (data->timer > 11) ? 11 : data->timer];
#else
    data->rotY = lbl_1_data_2EC[data->timer];
#endif
}
