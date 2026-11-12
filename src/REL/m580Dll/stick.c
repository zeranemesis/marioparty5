#include "types.h"
#include "game/object.h"
#include "game/hu3d.h"
#include "game/hsfformat.h"
#include "game/data.h"
#include "game/memory.h"
#include "game/audio.h"
#include "game/frand.h"
#include "game/mg/seqman.h"
#include "humath.h"
#include "datadir_enum.h"
#include <string.h>

#include "REL/m580Dll.h"

#ifndef M_PI
#define M_PI 3.141592653589793
#endif

double atan2(double, double);
double fmod(double, double);
double sin(double);
double cos(double);
double Hu3DMotionMaxTimeGet(HU3DMODELID modelId);

#define M580_STICK_MOTNUM 6

static inline f32 M580_WrapDeg(f32 deg)
{
    return fmod(deg, 360.0);
}

/* one motion of an actor: data file, blend time (seconds) and motion attributes;
   dataNum -1 = unused slot, -2 = end of list */
typedef struct M580StickMotion_s {
    /* 0x00 */ s32 dataNum;
    /* 0x04 */ f32 shift;
    /* 0x08 */ u32 attr;
} M580_STICKMOTION;

/* actor table entry (0x24 bytes on GC), terminated by count == -1 */
typedef struct M580StickActor_s {
    /* 0x00 */ s32 count;
    /* 0x04 */ s32 shared; /* model is created once per data file and linked afterwards */
    /* 0x08 */ s32 kind;
    /* 0x0C */ s32 baseIndex;
    /* 0x10 */ s32 modelNum;
    /* 0x14 */ M580_STICKMOTION *motions;
    /* 0x18 */ HuVecF *initPos;
    /* 0x1C */ f32 initRot;
    /* 0x20 */ OMOBJFUNC func;
} M580_STICKACTOR;

/* work block of the director object created by fn_1_8A64 (0xC bytes on GC) */
typedef struct M580StickData_s {
    /* 0x00 */ s32 state;
    /* 0x04 */ s32 timer;
    /* 0x08 */ HU3DMODELID modelId;
} M580_STICKDATA;

/* work block of one actor object (0xC8 bytes on GC) */
typedef struct M580StickWork_s {
    /* 0x00 */ M580_STICKACTOR *actor;
    /* 0x04 */ s32 index;
    /* 0x08 */ s32 state;
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ s32 timer;
    /* 0x14 */ s32 wait;
    /* 0x18 */ s32 motion;
    /* 0x1C */ s32 frame;
    /* 0x20 */ s32 busy;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ HuVecF pos;
    /* 0x34 */ HuVecF vel;
    /* 0x40 */ HuVecF rot;
    /* 0x4C */ f32 targetRot;
    /* 0x50 */ f32 alpha;
    /* 0x54 */ s32 idle;
    /* 0x58 */ u8 pad58[0x10];
    /* 0x68 */ M580_SHADOW shadow;
    /* 0x88 */ M580_BODY body;
} M580_STICKWORK;

OMOBJ *lbl_1_bss_238[2];
s32 lbl_1_bss_1B8[32];
s32 lbl_1_bss_138[32];
s16 lbl_1_bss_F8[32];
s16 lbl_1_bss_B8[32];

#define MOT(num, shift, attr) { DATANUM(DATA_m580, num), shift, attr }

M580_STICKMOTION lbl_1_data_310[6] = {
    MOT(0x1F, 0.2f, HU3D_MOTATTR_LOOP), { -2, 0.0f, 0 }, { 0 }, { 0 }, { 0 }, { 0 },
};
M580_STICKMOTION lbl_1_data_358[6] = {
    MOT(0x16, 0.2f, HU3D_MOTATTR_LOOP), { -1, 0.0f, 0 }, MOT(0x17, 0.2f, HU3D_MOTATTR_LOOP),
    MOT(0x18, 0.2f, 0), MOT(0x19, 0.2f, 0), MOT(0x1A, 0.2f, 0),
};
M580_STICKMOTION lbl_1_data_3A0[6] = {
    MOT(0x13, 0.2f, HU3D_MOTATTR_LOOP), MOT(0x14, 0.2f, HU3D_MOTATTR_LOOP), { -2, 0.0f, 0 },
    { 0 }, { 0 }, { 0 },
};
M580_STICKMOTION lbl_1_data_3E8[6] = {
    MOT(0x1C, 0.2f, HU3D_MOTATTR_LOOP), MOT(0x1D, 0.2f, HU3D_MOTATTR_LOOP), { -2, 0.0f, 0 },
    { 0 }, { 0 }, { 0 },
};
#undef MOT

HuVecF lbl_1_data_430 = { 0.0f, 0.0f, 0.0f };
HuVecF lbl_1_data_43C[2] = { { 0.0f, 1000.0f, -3000.0f }, { 0.0f, 100.0f, -70.0f } };
HuVecF lbl_1_data_454 = { -920.0f, 0.0f, 0.0f };
HuVecF lbl_1_data_460 = { 920.0f, 0.0f, 0.0f };

void fn_1_9748(OMOBJ *obj);
void fn_1_9F44(OMOBJ *obj);
void fn_1_BAC0(OMOBJ *obj);

M580_STICKACTOR lbl_1_data_46C[6] = {
    { 1, 0, 0, 0, DATANUM(DATA_m580, 0x1E), lbl_1_data_310, lbl_1_data_43C, 0.0f, fn_1_9748 },
    { 1, 0, 1, 0, DATANUM(DATA_m580, 0x15), lbl_1_data_358, &lbl_1_data_454, 90.0f, fn_1_9F44 },
    { 1, 0, 1, 1, DATANUM(DATA_m580, 0x15), lbl_1_data_358, &lbl_1_data_460, 270.0f, fn_1_9F44 },
    { 40, 1, 2, 0, DATANUM(DATA_m580, 0x12), lbl_1_data_3A0, &lbl_1_data_430, 0.0f, fn_1_BAC0 },
    { 5, 0, 3, 0, DATANUM(DATA_m580, 0x1B), lbl_1_data_3E8, &lbl_1_data_430, 0.0f, fn_1_BAC0 },
    { -1 },
};

char lbl_1_data_544[50][8] = {
    "kuri1", "kuri2", "kuri3", "kuri4", "kuri5", "kuri6", "kuri7", "kuri8", "kuri9", "kuri10",
    "kuri11", "kuri12", "kuri13", "kuri14", "kuri15", "kuri16", "kuri17", "kuri18", "kuri19", "kuri20",
    "kuri21", "kuri22", "kuri23", "kuri24", "kuri25", "kuri26", "kuri27", "kuri28", "kuri29", "kuri30",
    "kuri31", "kuri32", "kuri33", "kuri34", "kuri35", "kuri36", "kuri37", "kuri38", "kuri39", "kuri40",
    "kuri41", "kuri42", "kuri43", "kuri44", "kuri45", "kuri46", "kuri47", "kuri48", "kuri49", "kuri50",
};
char lbl_1_data_6D4[6][8] = { "kame1", "kame2", "kame3", "kame4", "kame5", "kame6" };
char lbl_1_data_704[16] = "itemhook_sao";
HuVecF lbl_1_data_738 = { 0.0f, -10.0f, 0.0f };
s32 lbl_1_data_744[2] = { DATANUM(DATA_m580, 0x0B), DATANUM(DATA_m580, 0x0C) };
char lbl_1_data_74C[12] = "itemhook_L";

void fn_1_8B98(OMOBJ *obj);
void fn_1_8B64(OMOBJ *obj);
HU3DMODELID fn_1_8CA4(s32 dataNum);
HU3DMOTID fn_1_8D90(HU3DMODELID model, s32 dataNum);
void fn_1_8E80(OMOBJMAN *objMan, M580_STICKACTOR *actor);
void fn_1_8C08(void);
s32 fn_1_943C(OMOBJ *obj);
void fn_1_92EC(OMOBJ *obj, s32 motion);

s32 fn_1_D544(void);
OMOBJ *fn_1_D30C(HuVecF *pos, HuVecF *vel);
OMOBJ *fn_1_D390(HuVecF *pos, HuVecF *vel, OMOBJ *hitId, f32 lerp);
s32 fn_1_D054(HuVecF *center, f32 dir, f32 halfAngle, f32 range);
HU3DMODELID fn_1_D56C(void);
void fn_1_D60C(HuVecF *pos);
void fn_1_D654(void);
s32 fn_1_1D04(void);
u32 fn_1_1D58(u32 team);
f32 fn_1_2634(f32 from, f32 to, f32 t);
s32 fn_1_89A8(s32 index, f32 *pos, f32 *angle);
void fn_1_ED34(M580_SHADOW *shadow);
s32 fn_1_11DC4(M580_BODY *body);

void fn_1_8A64(OMOBJMAN *objMan)
{
    OMOBJ *obj;
    M580_STICKDATA *data;
    M580_STICKACTOR *actor;

    obj = omAddObjEx(objMan, 0x3C, 0, 0, OM_GRP_NONE, fn_1_8B64);
    lbl_1_bss_238[0] = obj;
    obj->stat |= 0x100;
    data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M580_STICKDATA), HU_MEMNUM_OVL);
    obj->data = data;
    memset(data, 0, sizeof(M580_STICKDATA));
    fn_1_8C08();
    data->modelId = Hu3DModelCreateData(DATANUM(DATA_m580, 0x07));
    Hu3DModelLayerSet(data->modelId, 5);
    for (actor = lbl_1_data_46C; actor->count != -1; actor++) {
        fn_1_8E80(objMan, actor);
    }
}

void fn_1_8B60(void)
{
}

void fn_1_8B64(OMOBJ *obj)
{
    M580_STICKDATA *data = obj->data;

    data->state = 0;
    data->timer = 0;
    obj->objFunc = fn_1_8B98;
}

void fn_1_8B98(OMOBJ *obj)
{
    M580_STICKDATA *data = obj->data;

    switch (data->state) {
    case 0:
        if (fn_1_D544() != 0) {
            break;
        }
        data->state = data->state + 1;
    case 1:
        data->timer = fn_1_D544();
        break;
    }
}

void fn_1_8C08(void)
{
    s32 i;

    for (i = 0; i < 32; i++) {
        lbl_1_bss_1B8[i] = -1;
        lbl_1_bss_F8[i] = -1;
    }

    for (i = 0; i < 32; i++) {
        lbl_1_bss_138[i] = -1;
        lbl_1_bss_B8[i] = -1;
    }
}

HU3DMODELID fn_1_8CA4(s32 dataNum)
{
    HU3DMODELID model;
    s32 id;
    s32 i;

    for (i = 0; i < 32; i++) {
        id = lbl_1_bss_1B8[i];
        if (id == -1) {
            break;
        }
        if (id == dataNum) {
            return Hu3DModelLink(lbl_1_bss_F8[i]);
        }
    }
    if (i < 32) {
        model = Hu3DModelCreateData(dataNum);
        lbl_1_bss_1B8[i] = dataNum;
        lbl_1_bss_F8[i] = model;
        return model;
    }
    return -1;
}

HU3DMOTID fn_1_8D90(HU3DMODELID model, s32 dataNum)
{
    HU3DMOTID motion;
    s32 id;
    s32 i;

    for (i = 0; i < 32; i++) {
        id = lbl_1_bss_138[i];
        if (id == -1) {
            break;
        }
        if (id == dataNum) {
            return lbl_1_bss_B8[i];
        }
    }
    if (i < 32) {
        motion = Hu3DJointMotionData(model, dataNum);
        lbl_1_bss_138[i] = dataNum;
        lbl_1_bss_B8[i] = motion;
        return motion;
    }
    return -1;
}

void fn_1_8E80(OMOBJMAN *objMan, M580_STICKACTOR *actor)
{
    OMOBJ *obj;
    M580_STICKWORK *work;
    M580_STICKMOTION *motion;
    HU3DMODELID model;
    s32 n;
    s32 i;

    for (n = 0; n < actor->count; n++) {
        obj = omAddObjEx(objMan, 0x3C, 6, 6, 0, actor->func);
        work = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M580_STICKWORK), HU_MEMNUM_OVL);
        obj->data = work;
        memset(work, 0, sizeof(M580_STICKWORK));
        obj->stat |= 0x100;
        work->actor = actor;
        work->index = actor->baseIndex + n;
        work->motion = -1;
        for (i = 0; i < M580_STICK_MOTNUM; i++) {
            obj->mdlId[i] = -1;
        }
        for (i = 0; i < M580_STICK_MOTNUM; i++) {
            obj->mtnId[i] = -1;
        }
        if (actor->shared != 0) {
            model = fn_1_8CA4(actor->modelNum);
        } else {
            model = Hu3DModelCreateData(actor->modelNum);
        }
        obj->mdlId[0] = model;
        motion = actor->motions;
        for (i = 0; i < M580_STICK_MOTNUM; i++, motion++) {
            if (motion->dataNum == -2) {
                break;
            }
            if (motion->dataNum != -1) {
                obj->mtnId[i] = fn_1_8D90(model, motion->dataNum);
            }
        }
        work->pos = *actor->initPos;
        work->rot.y = actor->initRot;
        Hu3DModelLayerSet(model, 5);
    }
}

void fn_1_9194(OMOBJ *obj)
{
    M580_STICKWORK *work = obj->data;
    HU3DMODELID model = obj->mdlId[0];

    work->rot.y = M580_WrapDeg(work->rot.y);
    work->rot.z = 0.0f;
    work->rot.x = 0.0f;
    Hu3DModelPosSetV(model, &work->pos);
    Hu3DModelRotSetV(model, &work->rot);
    model = obj->mdlId[2];
    if (model == -1) {
        model = obj->mdlId[1];
    }
    if (model != -1) {
        Hu3DModelPosSet(model, work->pos.x, -work->pos.y, work->pos.z);
        Hu3DModelRotSetV(model, &work->rot);
        work->body.u.circle.prev = work->pos;
    }
    if (work->busy != 0 && fn_1_943C(obj) != 0) {
        work->busy = 0;
    }
    work->frame++;
}

void fn_1_92EC(OMOBJ *obj, s32 motion)
{
    M580_STICKWORK *work = obj->data;
    M580_STICKMOTION *entry;
    HU3DMOTID mot;
    HU3DMODELID model;

    if (work->motion != motion && motion < M580_STICK_MOTNUM) {
        mot = obj->mtnId[motion];
        work->frame = 0;
        if (mot != -1) {
            entry = &work->actor->motions[motion];
            model = obj->mdlId[0];
            if (work->motion < 0) {
                Hu3DMotionSet(model, mot);
                Hu3DModelAttrSet(model, entry->attr);
                model = obj->mdlId[1];
                if (model != -1) {
                    Hu3DMotionSet(model, mot);
                    Hu3DModelAttrSet(model, entry->attr);
                }
            } else {
                Hu3DMotionShiftSet(model, mot, 0.0f, 60.0f * entry->shift, entry->attr);
                model = obj->mdlId[1];
                if (model != -1) {
                    Hu3DMotionShiftSet(model, mot, 0.0f, 60.0f * entry->shift, entry->attr);
                }
            }
            work->motion = motion;
        }
    }
}

s32 fn_1_943C(OMOBJ *obj)
{
    HU3DMODELID model = obj->mdlId[0];

    if (Hu3DMotionEndCheck(model) != 0) {
        if (Hu3DMotionShiftIDGet(model) < 0) {
            return 1;
        }
    }
    return 0;
}

void fn_1_94A4(char *name, HuVecF *pos, f32 *scale)
{
    M580_STICKDATA *data = lbl_1_bss_238[0]->data;
    HSFOBJECT *hsfObj = Hu3DModelObjPtrGet(data->modelId, name);

    if (hsfObj != NULL) {
        pos->x = hsfObj->mesh.base.pos.x;
        pos->y = hsfObj->mesh.base.pos.y;
        pos->z = hsfObj->mesh.base.pos.z;
        scale[0] = hsfObj->mesh.base.rot.y;
    }
}

f32 fn_1_9528(OMOBJ *obj, HuVecF *target)
{
    M580_STICKWORK *work = obj->data;

    f32 dx = target->x - work->pos.x;
    f32 dz = target->z - work->pos.z;

    return 180.0 * (atan2(dx, dz) / M_PI);
}

f32 fn_1_95D0(OMOBJ *obj, HuVecF *target)
{
    M580_STICKWORK *work = obj->data;
    f32 dx = target->x - work->pos.x;
    f32 dz = target->z - work->pos.z;

    return HuMagPoint2D(dx, dz);
}

void fn_1_9970(OMOBJ *obj);

void fn_1_9748(OMOBJ *obj)
{
    M580_STICKWORK *work = obj->data;
    HU3DMODELID model;

    model = fn_1_8CA4(DATANUM(DATA_m580, 0x20));
    obj->mdlId[2] = model;
    Hu3DModelLayerSet(model, 1);
    fn_1_92EC(obj, 0);
    obj->objFunc = fn_1_9970;
}

void fn_1_9970(OMOBJ *obj)
{
    M580_STICKWORK *work = obj->data;
    HU3DMODELID model = obj->mdlId[0];
    HU3DMODELID shadow = obj->mdlId[2];
    HuVecF pos;
    HuVecF offset;
    f32 s;

    switch (work->state) {
    case 0:
        work->state++;
        work->timer = 0;
        work->alpha = 0.0f;
        Hu3DModelHookSet(model, lbl_1_data_704, fn_1_D56C());
    case 1:
        if (++work->timer < 210.0) {
            break;
        }
        work->state++;
    case 2:
        if ((work->alpha += 1.0 / 75.0) < 1.0) {
            break;
        }
        work->alpha = 1.0f;
        work->state++;
        work->timer = 0;
    case 3:
        if (++work->timer < 60.0) {
            break;
        }
        work->state++;
        Hu3DModelHookObjReset(model, lbl_1_data_704);
        fn_1_D654();
        HuAudFXPlay(0x960);
    case 4:
        if (++work->timer < 30.0) {
            break;
        }
        work->state++;
    case 5: {
        s32 mode = MgSeqModeGet();

        if (mode != 5 && mode != 6) {
            break;
        }
        work->state++;
    }
    case 6:
        if ((work->alpha -= (f64)(1.0f / 60.0f)) > 0.0) {
            break;
        }
        work->alpha = 0.0f;
        work->state++;
    case 7:
        if (fn_1_D544() == 0) {
            break;
        }
        work->state++;
        work->timer = 0;
    case 8:
        if (fn_1_1D04() < 2) {
            break;
        }
        if (++work->timer < 60.0) {
            break;
        }
        work->state = 2;
        Hu3DModelHookSet(model, lbl_1_data_704, fn_1_D56C());
        break;
    }
    s = sin(M_PI * (90.0 * work->alpha) / 180.0);
    if (0.0 == s) {
        Hu3DModelAttrSet(model, 1);
        Hu3DModelAttrSet(shadow, 1);
    } else {
        Hu3DModelAttrReset(model, 1);
        Hu3DModelAttrReset(shadow, 1);
    }
    VECSubtract(&work->actor->initPos[1], &work->actor->initPos[0], &offset);
    VECScale(&offset, &offset, s);
    VECAdd(&work->actor->initPos[0], &offset, &work->pos);
    work->targetRot = M580_WrapDeg(1.0 + work->targetRot);
    if (work->targetRot < 0.0) {
        work->targetRot = work->targetRot + 360.0;
    }
    pos.x = work->pos.x + 100.0 * (0.3 * sin(M_PI * work->targetRot / 180.0));
    pos.y = work->pos.y + 100.0 * (0.2 * sin(M_PI * work->targetRot / 180.0));
    pos.z = work->pos.z + 100.0 * (0.2 * cos(M_PI * work->targetRot / 180.0));
    Hu3DModelPosSetV(model, &pos);
    Hu3DModelRotSetV(model, &work->rot);
    Hu3DModelPosSet(shadow, pos.x, -pos.y, pos.z);
    Hu3DModelRotSetV(shadow, &work->rot);
    Hu3DModelObjPosGet(model, lbl_1_data_704, &pos);
    fn_1_D60C(&pos);
}
