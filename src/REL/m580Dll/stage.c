#include "game/data.h"
#include "game/hu3d.h"
#include "game/hsfformat.h"
#include "game/memory.h"
#include "game/mg/seqman.h"

#include "datadir_enum.h"

#include "REL/m580Dll.h"

float Hu3DMotionMaxTimeGet(HU3DMODELID modelId);

void fn_1_11E3C(void *obj);
void fn_1_1229C(void *obj, HSFOBJECT *hsfObj);
void fn_1_122D0(void *obj);
s32 fn_1_D544(void);
s32 fn_1_D7B8(void);

void fn_1_86D4(M580_STAGE *stage);
void fn_1_8710(M580_STAGE *stage);
void fn_1_87A4(M580_STAGE *stage);

static char lbl_1_data_298[5][10] = {
    "wall", "post_R", "post_L", "post_R_C", "post_L_C"
};

static s32 lbl_1_data_2CC[5] = { 0xC, 8, 8, 4, 4 };

static char lbl_1_data_2E0[6][8] = {
    "1p", "2p", "3p", "4p", "k_red", "k_blue"
};

M580_STAGEGLOBAL lbl_1_bss_B0;

void fn_1_83FC(M580_STAGE *stage)
{
    M580_STAGEANIM *anim = stage->anim;
    s16 model;
    s32 i;
    M580_HSFOBJECT *hsfObj;
    M580_STAGEANIMOBJ *obj;

    model = Hu3DModelCreateData(DATANUM(DATA_m580, 0x00));
    stage->mdls->mdl0 = model;
    Hu3DModelAttrSet(model, 0x40000001);
    Hu3DModelLayerSet(model, 6);

    model = Hu3DModelCreateData(DATANUM(DATA_m580, 0x01));
    stage->mdls->mdl1 = model;
    Hu3DModelAttrSet(model, 0x40000001);
    Hu3DModelLayerSet(model, 4);

    model = Hu3DModelCreateData(DATANUM(DATA_m580, 0x02));
    stage->mdls->mdl2 = model;
    Hu3DModelAttrSet(model, 0x40000001);
    Hu3DModelLayerSet(model, 3);

    model = Hu3DModelCreateData(DATANUM(DATA_m580, 0x03));
    stage->mdls->mdl3 = model;
    Hu3DModelAttrSet(model, 0x40000001);
    Hu3DModelLayerSet(model, 2);

    model = Hu3DModelCreateData(DATANUM(DATA_m580, 0x04));
    stage->mdls->mdl4 = model;
    Hu3DModelAttrSet(model, 0x40000001);
    Hu3DModelLayerSet(model, 0);

    model = Hu3DModelCreateData(DATANUM(DATA_m580, 0x05));
    stage->mdls->mdl5 = model;
    Hu3DModelLayerSet(model, 4);

    model = Hu3DModelCreateData(DATANUM(DATA_m580, 0x06));
    stage->mdls->mdl6 = model;
    Hu3DModelLayerSet(model, 1);

    Hu3DZClearLayerSet(1);
    Hu3DBGColorSet(0xB2, 0xB2, 0xB2);
    fn_1_8710(stage);

    model = Hu3DModelCreateData(DATANUM(DATA_m580, 0x10));
    stage->mdls->mdlObj = model;
    Hu3DModelAttrSet(model, 1);

    for (i = 0; i < 5; i++) {
        hsfObj = (M580_HSFOBJECT *) Hu3DModelObjPtrGet(model, lbl_1_data_298[i]);
        if (hsfObj != (M580_HSFOBJECT *) 0) {
            obj = &anim->objs[i];
            obj->state = 1;
            obj->frames = lbl_1_data_2CC[i];
            obj->param = 3;
            obj->index = i;
            obj->kind = 0xA;
            obj->stage = stage;
            obj->unk18 = 0;
            obj->unk1C = 0;
            fn_1_1229C(obj, (HSFOBJECT *) hsfObj);
            fn_1_122D0(obj);
            fn_1_11E3C(obj);
        }
    }

    stage->unk14 = fn_1_86D4;
}

void fn_1_86D4(M580_STAGE *stage)
{
    fn_1_87A4(stage);
    stage->unk4C++;
}

void fn_1_8710(M580_STAGE *stage)
{
    M580_STAGEANIM *anim = stage->anim;
    s16 model;

    anim->mode = 0;
    anim->count = 0;
    anim->prevMode = -1;

    model = Hu3DModelCreateData(DATANUM(DATA_m580, 0x08));
    stage->mdls->mdl7 = model;
    Hu3DModelAttrSet(model, 0x40000001);
    Hu3DModelLayerSet(model, 2);
}

void fn_1_87A4(M580_STAGE *stage)
{
    M580_STAGEANIM *anim = stage->anim;
    s16 model = stage->mdls->mdl7;
    s32 newMode = 0;

    switch (anim->mode) {
        case 0:
            if (MgSeqModeGet() != 5) {
                break;
            }
            anim->mode++;
        case 1:
            if (fn_1_D544() != 0) {
                break;
            }
            anim->mode++;
        case 2:
            if (fn_1_D544() == 0) {
                break;
            }
            anim->mode++;
            anim->count = 0;
        case 3:
            if (MgSeqModeGet() != 5) {
                break;
            }
            newMode = fn_1_D7B8() + 1;
            if (!((f64)(++anim->count) < 180.0)) {
                anim->mode = 0;
            }
            break;
    }

    if (anim->prevMode != newMode) {
        anim->prevMode = newMode;
        switch (newMode) {
            case 0:
                Hu3DMotionStartEndSet(model, 0.0f, 120.0f);
                Hu3DMotionTimeSet(model, 0.0f);
                break;
            case 1:
                Hu3DMotionStartEndSet(model, 120.0f, 180.0f);
                Hu3DMotionTimeSet(model, 120.0f);
                break;
            case 2:
                Hu3DMotionStartEndSet(model, 180.0f, Hu3DMotionMaxTimeGet(model));
                Hu3DMotionTimeSet(model, 180.0f);
                break;
        }
    }
}

s32 fn_1_89A8(s32 index, f32 *pos, f32 *angle)
{
    M580_STAGE *stage = lbl_1_bss_B0.stage;
    s16 model = stage->mdls->mdl5;
    M580_HSFOBJECT *hsfObj;

    hsfObj = (M580_HSFOBJECT *) Hu3DModelObjPtrGet(model, lbl_1_data_2E0[index]);
    if (hsfObj != (M580_HSFOBJECT *) 0) {
        pos[0] = hsfObj->currPos.x;
        pos[1] = hsfObj->currPos.y;
        pos[2] = hsfObj->currPos.z;
        *angle = 90.0 + hsfObj->currRot.y;
    }
    return Hu3DMotionEndCheck(model) == 0;
}
