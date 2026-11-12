#include "types.h"
#include "game/object.h"
#include "game/hu3d.h"
#include "game/hsfformat.h"
#include "game/data.h"
#include "game/memory.h"
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

/* puck work block (0x11C bytes on GC) */
typedef struct M580PuckData_s {
    /* 0x00 */ s32 hitPrev;
    /* 0x04 */ s32 hitNow;
    /* 0x08 */ s32 hidden;
    /* 0x0C */ s32 held;
    /* 0x10 */ s32 lastGoal;
    /* 0x14 */ s32 state;
    /* 0x18 */ s32 frame;
    /* 0x1C */ s32 timer;
    /* 0x20 */ HU3DMODELID model;
    /* 0x22 */ HU3DMODELID shadow;
    /* 0x24 */ HuVecF pos;
    /* 0x30 */ HuVecF vel;
    /* 0x3C */ u32 hitId;
    /* 0x40 */ s32 hitTime;
    /* 0x44 */ s32 sudden;
    /* 0x48 */ s32 suddenTime;
    /* 0x4C */ s32 count[4];
    /* 0x5C */ M580_BODY body;
    /* 0x9C */ M580_BODY goal[2];
} M580PuckData;

void fn_1_1229C(M580_BODY *body, HSFOBJECT *obj);
void fn_1_122D0(M580_BODY *body);
s32 fn_1_126C8(M580_BODY *body, HuVecF *pos);
s32 fn_1_11DC4(M580_BODY *body);
void fn_1_10F58(s32 side, s32 mode);
void fn_1_1908(HuVecF *pos);
void fn_1_212C(s32 seId, HuVecF *pos);
void fn_1_EC74(HuVecF *pos, HuVecF *vel);
void fn_1_EDF8(s32 sudden);
s32 fn_1_7E2C(s32 hitId);

void fn_1_C99C(OMOBJ *obj);
void fn_1_CBCC(OMOBJ *obj);
void fn_1_CF94(OMOBJ *obj);
s32 fn_1_D018(M580_BODY *self, M580_BODY *other);
void fn_1_D68C();

char lbl_1_data_778[2][8] = { "goal_L", "goal_R" };
HuVecF lbl_1_data_788 = { 0.0f, 0.0f, 0.0f };
HuVecF lbl_1_data_794 = { 0.0f, 0.0f, 0.0f };

OMOBJ *lbl_1_bss_240;

void fn_1_C8F8(OMOBJMAN *objMan)
{
    OMOBJ *obj;

    obj = omAddObjEx(objMan, 0x28, 0, 0, OM_GRP_NONE, fn_1_C99C);
    lbl_1_bss_240 = obj;
    obj->stat |= 0x100;
    obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M580PuckData), HU_MEMNUM_OVL);
    memset(obj->data, 0, sizeof(M580PuckData));
    obj->work[0] = 0;
}

void fn_1_C998(void)
{
}

void fn_1_C99C(OMOBJ *obj)
{
    M580PuckData *data = lbl_1_bss_240->data;
    M580_BODY *body;
    HU3DMODELID goalModel;
    HSFOBJECT *hsfObj;
    HU3DMODELID model;
    s32 i;

    data->hidden = 1;
    data->pos = lbl_1_data_788;
    data->vel = lbl_1_data_794;
    model = Hu3DModelCreateData(DATANUM(DATA_m580, 0x09));
    data->model = model;
    Hu3DModelLayerSet(model, 5);
    model = Hu3DModelLink(model);
    data->shadow = model;
    Hu3DModelLayerSet(model, 1);
    Hu3DModelScaleSet(model, 1.0f, -1.0f, 1.0f);
    Hu3DModelAttrSet(model, 0x800000);
    body = &data->body;
    body->flags = 0;
    body->mask = 0xA;
    body->kind = 1;
    body->id = 0;
    body->type = 0;
    body->obj = obj;
    body->update = fn_1_CF94;
    body->hit = fn_1_D018;
    body->u.circle.pos = &data->pos;
    body->u.circle.vel = &data->vel;
    body->u.circle.prev = data->pos;
    body->u.circle.radius = 40.0f;
    body->u.circle.weight = 1.0f;
    fn_1_11DC4(body);
    goalModel = Hu3DModelCreateData(DATANUM(DATA_m580, 0x11));
    Hu3DModelAttrSet(goalModel, 1);
    for (i = 0; i < 2; i++) {
        hsfObj = Hu3DModelObjPtrGet(goalModel, lbl_1_data_778[i]);
        if (hsfObj != NULL) {
            M580_BODY *goal = &data->goal[i];

            fn_1_1229C(goal, hsfObj);
            fn_1_122D0(goal);
        }
    }
    obj->objFunc = fn_1_CBCC;
}

void fn_1_CBCC(OMOBJ *obj)
{
    M580PuckData *data = obj->data;
    HU3DMODELID model = data->model;
    s32 i;

    switch (data->state) {
        case 0:
            break;
        case 1:
            if (data->held == 0) {
                data->state++;
                data->timer = 0;
            } else {
                break;
            }
        case 2:
            VECScale(&data->pos, &data->pos, 0.98f);
            data->timer++;
            if (!((f32)data->timer < 30.0) && MgSeqModeGet() == 5) {
                data->state++;
                data->hidden = 0;
                data->body.flags |= 1;
                fn_1_D68C();
            } else {
                break;
            }
        case 3:
            for (i = 0; i < 2; i++) {
                if (fn_1_126C8(&data->goal[i], &data->pos) != 0) {
                    data->state++;
                    data->timer = 0;
                    data->hidden = 1;
                    data->hitId = 0;
                    data->hitTime = 0;
                    data->lastGoal = i ^ 1;
                    fn_1_10F58(data->lastGoal, 1);
                }
            }
            if (MgSeqModeGet() != 5) {
                VECScale(&data->vel, &data->vel, 0.8f);
            }
            break;
        case 4:
            data->hitId = 0;
            data->hitTime = 0;
            VECScale(&data->vel, &data->vel, 0.8f);
            break;
    }
    if (data->held == 0) {
        if (data->hitId != 0) {
            data->hitTime++;
        }
        data->vel.x = data->vel.x * 0.99;
        data->vel.y = data->vel.y - 1.3333334028720856;
        data->vel.z = data->vel.z * 0.99;
        VECAdd(&data->pos, &data->vel, &data->pos);
        if (data->pos.y < 0.0) {
            data->pos.y = 0.0f;
            data->vel.y = 0.5 * data->vel.y;
            if (data->vel.y < 3.333333507180214) {
                data->vel.y = 0.0f;
            }
        } else if (data->hidden == 0 && VECMag(&data->vel) > 50.00000260770321) {
            fn_1_EC74(&data->pos, &data->vel);
        }
    }
    data->suddenTime++;
    if ((f32)data->suddenTime > 180.0) {
        data->sudden = 0;
    }
    if (MgSeqModeGet() == 5) {
        fn_1_EDF8(data->sudden);
    } else {
        fn_1_EDF8(0);
    }
    if (MgSeqModeGet() == 5 && data->hitPrev == 0 && data->hitNow != 0) {
        fn_1_212C(0x963, &data->pos);
    }
    data->hitPrev = data->hitNow;
    data->hitNow = 0;
    data->frame++;
}

void fn_1_CF94(OMOBJ *obj)
{
    M580PuckData *data = obj->data;

    if (data->held == 0) {
        Hu3DModelPosSetV(data->model, &data->pos);
        fn_1_1908(&data->pos);
    }
    Hu3DModelPosSet(data->shadow, data->pos.x, -data->pos.y, data->pos.z);
    data->body.u.circle.prev = data->pos;
}

s32 fn_1_D018(M580_BODY *self, M580_BODY *other)
{
    if (other->kind == 3) {
        M580PuckData *data = self->obj->data;

        data->hitNow = 1;
    }
    return 1;
}

s32 fn_1_D054(HuVecF *center, f32 dir, f32 halfAngle, f32 range)
{
    M580PuckData *data = lbl_1_bss_240->data;
    f32 dx;
    f32 dz;
    f32 angle;
    f32 dist;

    if (data->hidden != 0) {
        return 0;
    }
    dx = data->pos.x - center->x;
    dz = data->pos.z - center->z;
    if (halfAngle < 180.0f) {
        angle = fmod((f32)HuAtan(dx, dz) - dir, 360.0);
        if (angle < 0.0f) {
            angle += 360.0f;
        }
        if (angle > halfAngle && angle < 360.0f - halfAngle) {
            return 0;
        }
    }
    dist = HuMagPoint2D(dx, dz);
    if (dist > range) {
        return 0;
    }
    return 1;
}

s32 fn_1_D30C(HuVecF *pos, HuVecF *vel)
{
    M580PuckData *data = lbl_1_bss_240->data;

    if (pos != NULL) {
        *pos = data->pos;
    }
    if (vel != NULL) {
        *vel = data->vel;
    }
    if (data->hidden != 0) {
        return 0;
    }
    return data->hitId;
}

u32 fn_1_D390(HuVecF *pos, HuVecF *vel, u32 hitId, f32 lerp)
{
    M580PuckData *data = lbl_1_bss_240->data;

    if (data->hidden != 0) {
        data->hitId = 0;
        data->hitTime = 0;
        return 0;
    }
    if (pos != NULL) {
        data->pos.x = data->pos.x + lerp * (pos->x - data->pos.x);
        data->pos.y = data->pos.y + lerp * (pos->y - data->pos.y);
        data->pos.z = data->pos.z + lerp * (pos->z - data->pos.z);
    }
    if (vel != NULL) {
        data->vel = *vel;
        if (pos != NULL && (0.0f != vel->x || 0.0f != vel->z)) {
            fn_1_212C(0x962, pos);
        }
    }
    if (hitId != 0) {
        if (data->hitId != hitId && (data->hitId == 0 || data->hitTime > 5)) {
            data->hitId = hitId;
            data->hitTime = 0;
        }
    } else {
        data->hitId = 0;
        data->hitTime = 0;
    }
    return data->hitId;
}

s32 fn_1_D544(void)
{
    M580PuckData *data = lbl_1_bss_240->data;

    return data->hidden;
}

s16 fn_1_D56C(void)
{
    M580PuckData *data = lbl_1_bss_240->data;

    data->state = 1;
    data->held = 1;
    data->hitId = 0;
    data->body.flags &= ~1;
    data->vel.z = 0.0f;
    data->vel.y = 0.0f;
    data->vel.x = 0.0f;
    Hu3DModelPosSet(data->model, 0.0f, 0.0f, 0.0f);
    return data->model;
}

void fn_1_D60C(HuVecF *pos)
{
    M580PuckData *data = lbl_1_bss_240->data;

    if (data->held != 0) {
        data->pos = *pos;
    }
}

void fn_1_D654(void)
{
    M580PuckData *data = lbl_1_bss_240->data;

    if (data->held != 0) {
        data->held = 0;
    }
}

void fn_1_D68C()
{
    M580PuckData *data = lbl_1_bss_240->data;
    s32 i;

    data->sudden = 1;
    data->suddenTime = 0;
    for (i = 0; i < 4; i++) {
        data->count[i] = 0;
    }
}

s32 fn_1_D6EC(void)
{
    M580PuckData *data = lbl_1_bss_240->data;

    return data->sudden;
}

s32 fn_1_D714(s32 hitId)
{
    M580PuckData *data = lbl_1_bss_240->data;
    s32 team = fn_1_7E2C(hitId);

    if (data->sudden == 0) {
        return 0;
    }
    data->hitId = hitId;
    data->hitTime = 0;
    if (++data->count[team] >= 5) {
        data->sudden = 0;
        return 1;
    }
    return 0;
}

s32 fn_1_D7B8(void)
{
    M580PuckData *data = lbl_1_bss_240->data;

    return data->lastGoal;
}
