#include "game/object.h"
#include "game/hu3d.h"

typedef struct {
    u32 unk00;
    u32 unk04;
    u32 unk08;
    s32 unk0C;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    u32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    f32 unk50;
    u32 unk54;
    u32 unk58;
    void *unk5C;
    void *unk60;
    void *unk64;
    void *unk68;
    u32 unk6C;
    u32 unk70;
    u32 unk74;
    u32 unk78;
    u32 unk7C;
    u32 unk80;
    u32 unk84;
    void *unk88;
} SaruWork;

typedef struct {
    u32 unk00[16];
    HU3DMODELID *mdlId;
    u32 unk44[6];
    SaruWork *work;
} SaruObj;

extern void fn_1_9A04(SaruObj *obj, s32 arg1);
extern void fn_1_98D0(SaruObj *obj);

s32 fn_1_5FF0(SaruObj *obj)
{
    SaruWork *work = obj->work;

    fn_1_9A04(obj, 9);
    fn_1_98D0(obj);
    return work->unk30++;
}

void fn_1_6C0C(SaruObj *obj, s32 arg1)
{
    SaruWork *work = obj->work;

    if (work->unk10 != arg1) {
        work->unk10 = arg1;
        work->unk14 = 0;
        work->unk18 = 0;
        work->unk1C = 0;
        work->unk24 = 0;
        work->unk28 = 0;
    }
}

void fn_1_6C5C(SaruObj *obj, f32 arg1, f32 arg2, f32 arg3)
{
    SaruWork *work = obj->work;

    work->unk44 = work->unk44 + arg3 * (arg1 - work->unk44);
    work->unk4C = work->unk4C + arg3 * (arg2 - work->unk4C);
}

s32 fn_1_6F94(SaruObj *obj)
{
    fn_1_9A04(obj, 0);
    return 0;
}

void fn_1_A338(s32 idx, Vec *dst)
{
    extern const Vec lbl_1_data_38C[4];

    *dst = lbl_1_data_38C[idx & 3];
}

s32 fn_1_9B68(SaruObj *obj)
{
    HU3DMODELID modelId = *obj->mdlId;

    if (Hu3DMotionShiftIDGet(modelId) < 0) {
        return 1;
    }
    return 0;
}