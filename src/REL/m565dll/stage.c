#include "types.h"

typedef struct MgVec3_s {
    f32 x;
    f32 y;
    f32 z;
} MgVec3;

typedef struct MgStageSlot_s {
    s32 active;
    s16 modelId;
    s16 pad_6;
} MgStageSlot;

typedef struct MgStageMot_s {
    u8 pad_00[4];
    s16 state;
    u8 pad_06[12];
    s16 modelId;
    s16 subModelId;
    u8 pad_16[14];
    f32 posScale;
    f32 speed;
    u8 pad_2C[48];
    void *unk5C;
} MgStageMot;

typedef struct MgStage_s {
    u8 pad_0000[0x3074];
    MgStageSlot slots[10];
    u8 pad_30C4[0x28];
    void *unk30EC;
} MgStage;

extern void Hu3DModelPosSet(s16 modelId, f32 x, f32 y, f32 z);
extern void Hu3DModelAttrSet(s16 modelId, u32 attr);
extern void Hu3DModelAttrReset(s16 modelId, u32 attr);
extern void Hu3DMotionTimeSet(s16 modelId, f32 time);
extern f32 Hu3DMotionTimeGet(s16 modelId);
extern f32 Hu3DMotionMaxTimeGet(s16 modelId);
extern u32 frand(void);
extern void fn_1_E4(void *arg0, void *arg1);
extern void fn_1_5F24(void *arg0, void *arg1, void *arg2);
extern void fn_1_6140(void *arg0, void *arg1, void *arg2);

void fn_1_5EB0(MgStage *stage, void *arg1, s32 arg2, void *arg3)
{
    if (arg2 == 0) {
        fn_1_5F24(stage, arg1, arg3);
    } else {
        fn_1_6140(stage, arg1, arg3);
    }
}

void fn_1_6474(void *arg0, MgStageMot *mot)
{
    Hu3DModelPosSet(mot->modelId, 0.0f, 0.0f, 0.0f);
    mot->posScale = 0.0f;
    mot->speed = 0.1f * ((f32)(frand() % 10000) / 10000.0f);
    mot->state = 0x66;
}

void fn_1_66D4(MgStage *stage, MgStageMot *mot)
{
    Hu3DModelAttrSet(mot->modelId, 1);
    Hu3DModelPosSet(mot->modelId, 0.0f, 0.0f, 0.0f);
    Hu3DModelPosSet(mot->subModelId, 0.0f, -10000.0f, 0.0f);
    fn_1_E4(&stage->unk30EC, &mot->unk5C);
    mot->state = 1;
}

void fn_1_7578(MgStage *stage, MgVec3 *pos)
{
    s32 i;

    for (i = 0; i < 10; i++) {
        MgStageSlot *slot = &stage->slots[i];

        if (slot->active == 0) {
            Hu3DModelPosSet(slot->modelId, pos->x, pos->y, pos->z);
            Hu3DMotionTimeSet(slot->modelId, 0.0f);
            Hu3DModelAttrReset(slot->modelId, 1);
            slot->active = 1;
            break;
        }
    }
}

void fn_1_7624(MgStage *stage)
{
    s32 i;

    for (i = 0; i < 10; i++) {
        MgStageSlot *slot = &stage->slots[i];
        f32 maxTime;

        if (slot->active == 0) {
            continue;
        }
        maxTime = Hu3DMotionMaxTimeGet(slot->modelId);
        if (Hu3DMotionTimeGet(slot->modelId) >= maxTime) {
            Hu3DModelAttrSet(slot->modelId, 1);
            slot->active = 0;
        }
    }
}