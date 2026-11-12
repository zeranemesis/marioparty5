#include "REL/m510dll.h"
#include "string.h"

// Not declared in game/hu3d.h on purpose (see the note there); this unit needs
// the float return.
float Hu3DMotionMaxTimeGet(HU3DMODELID modelId);

// .data:0x0 (absolute 0x60) - spectator positions
M510_VECF lbl_1_data_60[11] = {
    { 385.0f, 60.0f, -850.0f },
    { 470.0f, 70.0f, -815.0f },
    { 560.0f, 80.0f, -780.0f },
    { 630.0f, 88.0f, -745.0f },
    { 450.0f, 90.0f, -865.0f },
    { 545.0f, 105.0f, -830.0f },
    { 610.0f, 115.0f, -810.0f },
    { 420.0f, 105.0f, -905.0f },
    { 500.0f, 125.0f, -890.0f },
    { 590.0f, 145.0f, -865.0f },
    { 660.0f, 160.0f, -840.0f },
};

// .data:0x84 (absolute 0xE4) - spectator scales
f32 lbl_1_data_E4[11] = { 0.65f, 0.7f, 0.7f, 0.75f, 0.6f, 0.65f, 0.7f, 0.55f, 0.6f, 0.6f, 0.55f };

void fn_1_295C(M510_STAGE *stage, M510_OBJ *obj)
{
    M510_OPEN_CAM *cam = &obj->cam;
    s16 order[11];
    M510_VECF pos;
    M510_VECF base;
    s16 i;
    s32 j;
    s32 k;
    s32 n;
    s16 a;
    s16 b;
    s16 tmp;
    HU3DMODELID modelId;

    memset(stage, 0, sizeof(M510_STAGE));
    stage->obj = obj;

    modelId = Hu3DModelCreate(HuDataSelHeapReadNum(0x440006, HU_MEMNUM_OVL, HEAP_MODEL));
    stage->modelId[0] = modelId;
    Hu3DModelPosSet(modelId, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(modelId, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(modelId, cam->unk04 & 0xFFFF);
    Hu3DModelLayerSet(modelId, 0);
    Hu3DModelAttrReset(modelId, 1);
    Hu3DModelShadowMapSet(modelId);

    modelId = Hu3DModelCreate(HuDataSelHeapReadNum(0x440007, HU_MEMNUM_OVL, HEAP_MODEL));
    stage->modelId[1] = modelId;
    Hu3DModelPosSet(modelId, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(modelId, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(modelId, cam->unk04 & 0xFFFF);
    Hu3DModelLayerSet(modelId, 0);
    Hu3DModelAttrReset(modelId, 1);
    Hu3DModelPosSet(modelId, 380.0f, 0.0f, 0.0f);
    Hu3DModelShadowMapSet(modelId);

    modelId = Hu3DModelCreate(HuDataSelHeapReadNum(0x440005, HU_MEMNUM_OVL, HEAP_MODEL));
    stage->modelId[2] = modelId;
    Hu3DModelRotSet(modelId, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(modelId, cam->unk04 & 0xFFFF);
    Hu3DModelLayerSet(modelId, 0);
    Hu3DModelAttrReset(modelId, 1);
    Hu3DModelPosSet(modelId, 380.0f, 0.0f, 0.0f);
    stage->motionId = Hu3DJointMotion(modelId, HuDataSelHeapReadNum(0x440017, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DMotionSet(modelId, stage->motionId);
    Hu3DModelAttrSet(modelId, 0x40000001);

    for (i = 0; i < 11; i++) {
        order[i] = i;
    }
    for (j = 0; j < 50; j++) {
        a = j % 11;
        b = frand() % 11;
        tmp = order[a];
        order[a] = order[b];
        order[b] = tmp;
    }

    j = 0;
    k = 0;
    while (j < 8) {
        pos.x = 380.0f + lbl_1_data_60[order[k]].x;
        pos.y = lbl_1_data_60[order[k]].y;
        pos.z = lbl_1_data_60[order[k]].z;
        fn_1_30D8(&stage->spec[j], j % 3, &pos, stage, lbl_1_data_E4[order[k]]);
        k++;
        j++;
    }
    n = 10;
    while (j < 16) {
        pos.x = 380.0f + -lbl_1_data_60[order[n]].x;
        pos.y = lbl_1_data_60[order[n]].y;
        pos.z = lbl_1_data_60[order[n]].z;
        fn_1_30D8(&stage->spec[j], j % 3, &pos, stage, lbl_1_data_E4[order[n]]);
        n--;
        j++;
    }

    base.x = 420.0f;
    base.y = 190.0f;
    base.z = -910.0f;
    pos = base;
    fn_1_3598(&stage->digit[0], obj->remain % 10, &pos, stage);

    base.x = 340.0f;
    base.y = 190.0f;
    base.z = -910.0f;
    pos = base;
    fn_1_3598(&stage->digit[1], obj->remain / 10, &pos, stage);
}

void fn_1_2F8C(M510_STAGE *stage)
{
    s32 i;

    fn_1_3834(&stage->digit[1]);
    fn_1_3834(&stage->digit[0]);
    for (i = 0; i < 3; i++) {
        fn_1_33C0(&stage->spec[i]);
    }
    Hu3DMotionKill(stage->motionId);
    Hu3DModelKill(stage->modelId[2]);
    Hu3DModelKill(stage->modelId[1]);
    Hu3DModelKill(stage->modelId[0]);
    memset(stage, 0, sizeof(M510_STAGE));
}

void fn_1_3020(M510_STAGE *stage)
{
    M510_OBJ *obj = stage->obj;
    s32 i;

    for (i = 0; i < 16; i++) {
        fn_1_3418(&stage->spec[i]);
    }
    stage->digit[1].goal = obj->remain / 10;
    stage->digit[0].goal = obj->remain % 10;
    fn_1_38A0(&stage->digit[0]);
    fn_1_38A0(&stage->digit[1]);
}

void fn_1_30D8(M510_SPECTATOR *spec, u16 type, M510_VECF *pos, M510_STAGE *stage, f32 scale)
{
    M510_OPEN_CAM *cam = &stage->obj->cam;
    s16 modelId;

    memset(spec, 0, sizeof(M510_SPECTATOR));
    spec->owner = stage;
    spec->type = type;
    spec->pos.x = pos->x;
    spec->pos.y = pos->y;
    spec->pos.z = pos->z;

    if (type == 0) {
        modelId = Hu3DModelCreate(HuDataSelHeapReadNum(0x440012, HU_MEMNUM_OVL, HEAP_MODEL));
        spec->modelId = modelId;
        spec->motion[0] = Hu3DJointMotion(modelId, HuDataSelHeapReadNum(0x440020, HU_MEMNUM_OVL, HEAP_MODEL));
        spec->motion[1] = Hu3DJointMotion(modelId, HuDataSelHeapReadNum(0x440021, HU_MEMNUM_OVL, HEAP_MODEL));
        spec->motion[2] = Hu3DJointMotion(modelId, HuDataSelHeapReadNum(0x440022, HU_MEMNUM_OVL, HEAP_MODEL));
    } else if (type == 1) {
        modelId = Hu3DModelCreate(HuDataSelHeapReadNum(0x440013, HU_MEMNUM_OVL, HEAP_MODEL));
        spec->modelId = modelId;
        spec->motion[0] = Hu3DJointMotion(modelId, HuDataSelHeapReadNum(0x440023, HU_MEMNUM_OVL, HEAP_MODEL));
        spec->motion[1] = Hu3DJointMotion(modelId, HuDataSelHeapReadNum(0x440024, HU_MEMNUM_OVL, HEAP_MODEL));
        spec->motion[2] = Hu3DJointMotion(modelId, HuDataSelHeapReadNum(0x440025, HU_MEMNUM_OVL, HEAP_MODEL));
    } else {
        modelId = Hu3DModelCreate(HuDataSelHeapReadNum(0x440014, HU_MEMNUM_OVL, HEAP_MODEL));
        spec->modelId = modelId;
        spec->motion[0] = Hu3DJointMotion(modelId, HuDataSelHeapReadNum(0x440026, HU_MEMNUM_OVL, HEAP_MODEL));
        spec->motion[1] = Hu3DJointMotion(modelId, HuDataSelHeapReadNum(0x440027, HU_MEMNUM_OVL, HEAP_MODEL));
        spec->motion[2] = Hu3DJointMotion(modelId, HuDataSelHeapReadNum(0x440028, HU_MEMNUM_OVL, HEAP_MODEL));
    }
    Hu3DModelRotSet(modelId, 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(modelId, scale, scale, scale);
    Hu3DModelCameraSet(modelId, cam->unk04 & 0xFFFF);
    Hu3DModelLayerSet(modelId, 4);
    Hu3DModelAttrReset(modelId, 1);
    Hu3DModelPosSet(modelId, pos->x, pos->y, pos->z);
    Hu3DMotionSet(modelId, spec->motion[0]);
    Hu3DModelAttrSet(modelId, 0x40000001);
}

void fn_1_33C0(M510_SPECTATOR *spec)
{
    Hu3DMotionKill(spec->motion[2]);
    Hu3DMotionKill(spec->motion[1]);
    Hu3DMotionKill(spec->motion[0]);
    Hu3DModelKill(spec->modelId);
    memset(spec, 0, sizeof(M510_SPECTATOR));
}

void fn_1_3418(M510_SPECTATOR *spec)
{
    M510_VECF screen;

    switch (spec->state) {
    case 1:
        Hu3DMotionShiftSet(spec->modelId, spec->motion[2], 0.0f, 4.0f, 0x40000001);
        spec->timer = 0;
        spec->state++;
        Hu3D3Dto2D(&spec->pos, 1, &screen);
        fn_1_634(fn_1_5B4(0x6A1), (s32)(64.0f + (16.0f * (2.0f * ((screen.x / 576.0f) - 0.5f)))));
        break;
    case 2:
        if (spec->timer++ > 60) {
            Hu3DMotionShiftSet(spec->modelId, spec->motion[0], 0.0f, 4.0f, 0x40000001);
            spec->timer = 0;
            spec->state = 0;
        }
        break;
    }
}

void fn_1_3598(M510_DIGIT *digit, s16 value, M510_VECF *pos, M510_STAGE *stage)
{
    M510_OPEN_CAM *cam = &stage->obj->cam;
    s16 modelId;

    memset(digit, 0, sizeof(M510_DIGIT));
    digit->owner = stage;
    digit->goal = value;
    digit->cur = value;

    modelId = Hu3DModelCreate(HuDataSelHeapReadNum(0x440015, HU_MEMNUM_OVL, HEAP_MODEL));
    digit->modelId = modelId;
    digit->motion[0] = Hu3DJointMotion(modelId, HuDataSelHeapReadNum(0x440029, HU_MEMNUM_OVL, HEAP_MODEL));
    digit->motion[1] = Hu3DJointMotion(modelId, HuDataSelHeapReadNum(0x44002A, HU_MEMNUM_OVL, HEAP_MODEL));
    digit->motion[2] = Hu3DJointMotion(modelId, HuDataSelHeapReadNum(0x44002B, HU_MEMNUM_OVL, HEAP_MODEL));
    digit->motion[3] = Hu3DJointMotion(modelId, HuDataSelHeapReadNum(0x44002C, HU_MEMNUM_OVL, HEAP_MODEL));
    digit->motion[4] = Hu3DJointMotion(modelId, HuDataSelHeapReadNum(0x44002D, HU_MEMNUM_OVL, HEAP_MODEL));
    digit->motion[5] = Hu3DJointMotion(modelId, HuDataSelHeapReadNum(0x44002E, HU_MEMNUM_OVL, HEAP_MODEL));
    digit->motion[6] = Hu3DJointMotion(modelId, HuDataSelHeapReadNum(0x44002F, HU_MEMNUM_OVL, HEAP_MODEL));
    digit->motion[7] = Hu3DJointMotion(modelId, HuDataSelHeapReadNum(0x440030, HU_MEMNUM_OVL, HEAP_MODEL));
    digit->motion[8] = Hu3DJointMotion(modelId, HuDataSelHeapReadNum(0x440031, HU_MEMNUM_OVL, HEAP_MODEL));
    digit->motion[9] = Hu3DJointMotion(modelId, HuDataSelHeapReadNum(0x440032, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelRotSet(modelId, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(modelId, cam->unk04 & 0xFFFF);
    Hu3DModelLayerSet(modelId, 4);
    Hu3DModelAttrReset(modelId, 1);
    Hu3DModelPosSet(modelId, pos->x, pos->y, pos->z);
    Hu3DModelAttrReset(modelId, 0x40000001);
    Hu3DModelAttrReset(modelId, 0x40000004);
    Hu3DMotionSet(modelId, digit->motion[value]);
    Hu3DMotionTimeSet(modelId, Hu3DMotionMaxTimeGet(modelId));
}

void fn_1_3834(M510_DIGIT *digit)
{
    s32 i;

    for (i = 0; i < 10; i++) {
        Hu3DMotionKill(digit->motion[i]);
    }
    Hu3DModelKill(digit->modelId);
    memset(digit, 0, sizeof(M510_DIGIT));
}

void fn_1_38A0(M510_DIGIT *digit)
{
    f32 maxTime;
    s16 diff;
    s16 absDiff;
    s16 next;

    if (digit->cur != digit->goal) {
        diff = digit->goal - digit->cur;
        if (diff >= 0) {
            absDiff = diff;
        } else {
            absDiff = -diff;
        }
        if (absDiff > 5) {
            if (diff > 0) {
                next = diff - 10;
            } else {
                next = diff + 10;
            }
            diff = next;
        }
        if (diff > 0) {
            Hu3DModelAttrReset(digit->modelId, 0x40000004);
            maxTime = Hu3DMotionMaxTimeGet(digit->modelId);
            if (Hu3DMotionTimeGet(digit->modelId) >= maxTime) {
                if (digit->cur >= 9) {
                    next = 0;
                } else {
                    next = digit->cur + 1;
                }
                digit->cur = next;
                Hu3DMotionSet(digit->modelId, digit->motion[digit->cur]);
            }
        } else {
            Hu3DModelAttrSet(digit->modelId, 0x40000004);
            if (Hu3DMotionTimeGet(digit->modelId) <= 0.0f) {
                if (digit->cur <= 0) {
                    next = 9;
                } else {
                    next = digit->cur - 1;
                }
                digit->cur = next;
                Hu3DMotionSet(digit->modelId, digit->motion[digit->cur]);
                Hu3DMotionTimeSet(digit->modelId, Hu3DMotionMaxTimeGet(digit->modelId));
                if (digit->cur == digit->goal) {
                    Hu3DModelAttrReset(digit->modelId, 0x40000004);
                }
            }
        }
    }
}
