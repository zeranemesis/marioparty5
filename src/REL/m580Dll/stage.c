/* humath.h pulls in libc/math.h, whose `extern inline sqrtf` carries two
   `static const double` locals.  MWCC emits those two constants into .rodata
   of every translation unit that includes it even when sqrtf is never called,
   and the original stage.o has no such pair in its .rodata (the 0.5/3.0 pair
   in m580Dll.rel belongs to the next unit in link order).  Suppressing
   math.h keeps this unit's literal pool to the six constants it uses. */
#define _MATH_H

#include "game/data.h"
#include "game/hu3d.h"
#include "game/hsfformat.h"
#include "game/memory.h"
#include "game/mg/seqman.h"

#include "datadir_enum.h"

#include "REL/m580Dll.h"

#ifdef TARGET_PC
/* the stage object is the engine OMOBJ itself */
typedef OMOBJ M580_STAGEOBJ;
typedef HSFOBJECT STG_HSF;
#define STG_ANIM(s) ((M580_STAGEANIM *)(s)->data)
#define STG_MDL(s, name) ((s)->mdlId[STG_MDLIDX_##name])
#define STG_SETFUNC(s, f) ((s)->objFunc = (f))
#define STG_COUNTER(s) ((s)->work[0])
#define STG_CURR_POS(h) ((h)->mesh.curr.pos)
#define STG_CURR_ROT(h) ((h)->mesh.curr.rot)
#define STG_MDLIDX_mdl0 0
#define STG_MDLIDX_mdl1 1
#define STG_MDLIDX_mdl2 2
#define STG_MDLIDX_mdl3 3
#define STG_MDLIDX_mdl4 4
#define STG_MDLIDX_mdl5 5
#define STG_MDLIDX_mdl6 6
#define STG_MDLIDX_mdl7 7
#define STG_MDLIDX_mdlObj 9
#else
typedef M580_STAGE M580_STAGEOBJ;
typedef M580_HSFOBJECT STG_HSF;
#define STG_ANIM(s) ((s)->anim)
#define STG_MDL(s, name) ((s)->mdls->name)
#define STG_SETFUNC(s, f) ((s)->unk14 = (f))
#define STG_COUNTER(s) ((s)->unk4C)
#define STG_CURR_POS(h) ((h)->currPos)
#define STG_CURR_ROT(h) ((h)->currRot)
#endif

float Hu3DMotionMaxTimeGet(HU3DMODELID modelId);

void fn_1_11E3C(void *obj);
void fn_1_1229C(void *obj, HSFOBJECT *hsfObj);
void fn_1_122D0(void *obj);
s32 fn_1_D544(void);
s32 fn_1_D7B8(void);

void fn_1_86D4(M580_STAGEOBJ *stage);
void fn_1_8710(M580_STAGEOBJ *stage);
void fn_1_87A4(M580_STAGEOBJ *stage);

static char lbl_1_data_298[5][10] = {
    "wall", "post_R", "post_L", "post_R_C", "post_L_C"
};

static s32 lbl_1_data_2CC[5] = { 0xC, 8, 8, 4, 4 };

static char lbl_1_data_2E0[6][8] = {
    "1p", "2p", "3p", "4p", "k_red", "k_blue"
};

M580_STAGEGLOBAL lbl_1_bss_B0;

void fn_1_83FC(M580_STAGEOBJ *stage)
{
    M580_STAGEANIM *anim = STG_ANIM(stage);
    s16 model;
    s32 i;
    STG_HSF *hsfObj;
    M580_STAGEANIMOBJ *obj;

    model = Hu3DModelCreateData(DATANUM(DATA_m580, 0x00));
    STG_MDL(stage, mdl0) = model;
    Hu3DModelAttrSet(model, 0x40000001);
    Hu3DModelLayerSet(model, 6);

    model = Hu3DModelCreateData(DATANUM(DATA_m580, 0x01));
    STG_MDL(stage, mdl1) = model;
    Hu3DModelAttrSet(model, 0x40000001);
    Hu3DModelLayerSet(model, 4);

    model = Hu3DModelCreateData(DATANUM(DATA_m580, 0x02));
    STG_MDL(stage, mdl2) = model;
    Hu3DModelAttrSet(model, 0x40000001);
    Hu3DModelLayerSet(model, 3);

    model = Hu3DModelCreateData(DATANUM(DATA_m580, 0x03));
    STG_MDL(stage, mdl3) = model;
    Hu3DModelAttrSet(model, 0x40000001);
    Hu3DModelLayerSet(model, 2);

    model = Hu3DModelCreateData(DATANUM(DATA_m580, 0x04));
    STG_MDL(stage, mdl4) = model;
    Hu3DModelAttrSet(model, 0x40000001);
    Hu3DModelLayerSet(model, 0);

    model = Hu3DModelCreateData(DATANUM(DATA_m580, 0x05));
    STG_MDL(stage, mdl5) = model;
    Hu3DModelLayerSet(model, 4);

    model = Hu3DModelCreateData(DATANUM(DATA_m580, 0x06));
    STG_MDL(stage, mdl6) = model;
    Hu3DModelLayerSet(model, 1);

    Hu3DZClearLayerSet(1);
    Hu3DBGColorSet(0xB2, 0xB2, 0xB2);
    fn_1_8710(stage);

    model = Hu3DModelCreateData(DATANUM(DATA_m580, 0x10));
    STG_MDL(stage, mdlObj) = model;
    Hu3DModelAttrSet(model, 1);

    for (i = 0; i < 5; i++) {
        hsfObj = (STG_HSF *) Hu3DModelObjPtrGet(model, lbl_1_data_298[i]);
        if (hsfObj != (STG_HSF *) 0) {
            obj = &anim->objs[i];
            obj->state = 1;
            obj->frames = lbl_1_data_2CC[i];
            obj->param = 3;
            obj->index = i;
            obj->kind = 0xA;
            obj->stage = (M580_STAGE *)stage;
            obj->unk18 = 0;
            obj->unk1C = 0;
            fn_1_1229C(obj, (HSFOBJECT *) hsfObj);
            fn_1_122D0(obj);
            fn_1_11E3C(obj);
        }
    }

    STG_SETFUNC(stage, fn_1_86D4);
}

void fn_1_86D4(M580_STAGEOBJ *stage)
{
    fn_1_87A4(stage);
    STG_COUNTER(stage)++;
}

void fn_1_8710(M580_STAGEOBJ *stage)
{
    M580_STAGEANIM *anim = STG_ANIM(stage);
    s16 model;

    anim->mode = 0;
    anim->count = 0;
    anim->prevMode = -1;

    model = Hu3DModelCreateData(DATANUM(DATA_m580, 0x08));
    STG_MDL(stage, mdl7) = model;
    Hu3DModelAttrSet(model, 0x40000001);
    Hu3DModelLayerSet(model, 2);
}

void fn_1_87A4(M580_STAGEOBJ *stage)
{
    M580_STAGEANIM *anim = STG_ANIM(stage);
    s16 model = STG_MDL(stage, mdl7);
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
    M580_STAGEOBJ *stage = lbl_1_bss_B0.stage;
    s16 model = STG_MDL(stage, mdl5);
    STG_HSF *hsfObj;

    hsfObj = (STG_HSF *) Hu3DModelObjPtrGet(model, lbl_1_data_2E0[index]);
    if (hsfObj != (STG_HSF *) 0) {
        pos[0] = STG_CURR_POS(hsfObj).x;
        pos[1] = STG_CURR_POS(hsfObj).y;
        pos[2] = STG_CURR_POS(hsfObj).z;
        *angle = 90.0 + STG_CURR_ROT(hsfObj).y;
    }
    return Hu3DMotionEndCheck(model) == 0;
}
