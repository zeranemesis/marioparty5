#include "types.h"
#include "game/object.h"
#include "game/charman.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/pad.h"
#include "game/frand.h"
#include "humath.h"
#include "REL/m503Dll.h"
#include <string.h>

#ifndef M_PI
#define M_PI 3.141592653589793
#endif

double atan2(double, double);
double fmod(double, double);

/* Embedded actor record handed to fn_1_DF30 (same layout as the actor table entries). */
typedef struct {
    u32 flags;
    s32 unk04;
    s32 unk08;
    s32 state;
    OMOBJ *owner;
    void (*fn)(OMOBJ *owner);
    HuVecF *pos;
    f32 rad;
#ifdef TARGET_PC
    s32 mode;
#endif
} M503PlayerActor;

typedef struct {
    s32 no;
    s32 grp;
    s32 slot;
    s32 charNo;
    s32 motion;
    f32 ovTime;
    u32 btnDown;
    u32 btn;
    f32 dir;
    f32 power;
    s32 type;
    s32 level;
    s32 wait;
    s32 prevHold;
    OMOBJ *target;
    s32 targetTime;
    HuVecF dest;
    s32 state;
    s32 timer;
    s32 frame;
    s32 unk58;
    HuVecF pos;
    HuVecF vel;
    f32 rotY;
    s32 stun;
    s32 busy;
    s32 voice;
    s32 holding;
    OMOBJ *item;
    M503PlayerActor actor;
} M503Player;

typedef struct {
    u16 mot;
    s16 overlay;
    f32 shift;
    u32 attr;
} M503MotInfo;

typedef struct {
    f32 chance;
    f32 range;
    f32 dist;
    s32 holdWait;
    s32 wait;
    s32 retarget;
    f32 power;
} M503AiInfo;

extern s32 fn_1_DF30(void *actor);
extern void fn_1_1D98(s32 idx, s32 val0, s32 val1);
extern void fn_1_1DD8(void);
extern s32 fn_1_1CB0(void);
extern s32 fn_1_1AE0(void);
extern s32 fn_1_1AF8(void);
extern s32 fn_1_1B10(void);
extern s32 fn_1_1B40(s32 idx);
extern void fn_1_1B70(s32 idx);
extern void fn_1_1BA0(s32 idx, s32 val);
extern void fn_1_1C80(s32 idx);
extern s32 fn_1_1CE8(void);
extern s32 fn_1_1D00(void);
extern s32 *fn_1_1D18(void);
extern s32 fn_1_1EB0(HuVecF *pos);
extern s32 fn_1_1F44(HuVecF *pos);
extern s32 fn_1_1FD8(s32 seId, HuVecF *pos);
extern f32 fn_1_2B7C(f32 from, f32 to, f32 rate);
extern OMOBJ *fn_1_9D18(HuVecF *pos, f32 yaw, f32 fov, f32 dist);
extern s32 fn_1_A080(OMOBJ *item, HuVecF *pos, HuVecF *dest);
extern void fn_1_A338(s32 idx, HuVecF *dst);
extern s16 fn_1_A368(OMOBJ *item, OMOBJ *holder);
extern void fn_1_A438(OMOBJ *item, HuVecF *pos, s32 mode);
extern f32 fn_1_BFE8(f32 x, f32 z);
extern void fn_1_C424(f32 x, f32 z, f32 scale);

void fn_1_3440(OMOBJMAN *objMan);
void fn_1_34F4(void);
void fn_1_3518(OMOBJ *obj);
void fn_1_37C0(OMOBJ *obj);
void fn_1_3C3C(OMOBJ *obj);
void fn_1_4380(OMOBJ *obj);
void fn_1_443C(OMOBJ *obj);
void fn_1_4658(OMOBJ *obj);
void fn_1_47C8(OMOBJ *obj);
void fn_1_48C8(OMOBJ *obj);
void fn_1_4E94(OMOBJ *obj, u32 motion);
s32 fn_1_5038(OMOBJ *obj);
s32 fn_1_5104(OMOBJ *obj);
s32 fn_1_5164(OMOBJ *obj, u32 motion);
s32 fn_1_575C(OMOBJ *obj, s32 mode);
s32 fn_1_53B8(OMOBJ *obj);
OMOBJ *fn_1_53D4(HuVecF *pos, f32 yaw, f32 fov, f32 maxDist);
s32 fn_1_5708(OMOBJ *obj, HuVecF *pos, f32 *rotY);

OMOBJ *lbl_1_bss_30[4];

u32 lbl_1_data_A8[8] = {
    0x8C0000, 0x8C0001, 0x8C0002, 0x8C0006, 0x8C0007, 0x8C0028, 0x8C0026, 0x8C0027,
};

M503MotInfo lbl_1_data_C8[10] = {
    { 0, -1, 0.2f, 0x40000001 },
    { 1, -1, 0.2f, 0x40000001 },
    { 2, -1, 0.2f, 0x40000001 },
    { 3, -1, 0.2f, 0 },
    { 4, -1, 0.2f, 0 },
    { 0, 5, 0.2f, 0 },
    { 0, 6, 0.2f, 0x40000001 },
    { 1, 6, 0.2f, 0x40000001 },
    { 2, 7, 0.2f, 0x40000001 },
    { 0, 5, 0.2f, 0x40000004 },
};

HuVecF lbl_1_data_140[4] = {
    { -300.0f, 0.0f, 0.0f },
    { -100.0f, 0.0f, 0.0f },
    { 100.0f, 0.0f, 0.0f },
    { 300.0f, 0.0f, 0.0f },
};

M503AiInfo lbl_1_data_170[4] = {
    { 0.3f, 40.0f, 120.0f, 0x3C, 0x3C, 0x1E0, 0.65f },
    { 0.8f, 60.0f, 180.0f, 0x33, 0x33, 0x12C, 0.8f },
    { 0.9f, 70.0f, 200.0f, 0x30, 0x30, 0xF0, 0.85f },
    { 1.0f, 100.0f, 300.0f, 0x2A, 0x2A, 0xB4, 0.9f },
};

void fn_1_3440(OMOBJMAN *objMan)
{
    s32 i;
    OMOBJ *obj;

    omMakeGroupEx(objMan, 0, 4);
    for (i = 0; i < 4; i++) {
        obj = omAddObjEx(objMan, 0x1E, 1, 8, 0, fn_1_3518);
        lbl_1_bss_30[i] = obj;
        obj->work[0] = i;
        obj->work[1] = GwPlayerConf[i].grp;
        obj->work[2] = i;
    }
}

void fn_1_34F4(void)
{
    CharModelKill(CHARNO_NONE);
}

void fn_1_3518(OMOBJ *obj)
{
    M503Player *p;
    s32 no;
    s32 slot;
    s16 charNo;
    HU3DMODELID model;
    s32 i;

    obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M503Player), HU_MEMNUM_OVL);
    p = obj->data;
    memset(p, 0, sizeof(M503Player));
    obj->stat |= 0x100;
    no = obj->work[0];
    p->no = no;
    p->grp = obj->work[1];
    slot = obj->work[2];
    p->slot = slot;
    charNo = GwPlayerConf[no].charNo;
    p->charNo = charNo;
    fn_1_1D98(no, p->grp, p->charNo);
    p->level = GwPlayerConf[no].dif & 3;
    p->type = GwPlayerConf[no].type;
    p->wait = 0;
    model = CharModelCreate(charNo, 8);
    *obj->mdlId = model;
    Hu3DModelLayerSet(model, 3);
    for (i = 0; i < 8; i++) {
        obj->mtnId[i] = CharMotionCreate(charNo, lbl_1_data_A8[i]);
    }
    CharMotionDataClose(charNo);
    p->motion = -1;
    fn_1_4E94(obj, 0);
    CharNpcDustVoiceOffSet(model, obj->mtnId[1], 0);
    CharNpcDustVoiceOffSet(model, obj->mtnId[2], 1);
    CharNpcDustVoiceOffSet(model, obj->mtnId[7], 0);
    CharModelStepFxSet(charNo, 2);
    p->pos.x = obj->trans.x = lbl_1_data_140[slot].x;
    p->pos.y = obj->trans.y = lbl_1_data_140[slot].y;
    p->pos.z = obj->trans.z = lbl_1_data_140[slot].z;
    p->vel.z = 0.0f;
    p->vel.y = 0.0f;
    p->vel.x = 0.0f;
    p->rotY = 0.0f;
    p->actor.flags = 1;
    p->actor.unk04 = 0;
    p->actor.unk08 = no;
    p->actor.state = 0;
    p->actor.owner = obj;
    p->actor.fn = fn_1_47C8;
    p->actor.pos = &p->pos;
    p->actor.rad = 150.0f;
    fn_1_DF30(&p->actor);
    p->state = 0;
    p->timer = 0;
    p->voice = 1;
    obj->objFunc = fn_1_37C0;
}

void fn_1_37C0(OMOBJ *obj)
{
    M503Player *p = obj->data;
    s16 pad;
    f32 stickX;
    f32 stickY;

    p->btnDown = 0;
    p->btn = 0;
    p->dir = 0.0f;
    p->power = 0.0f;
    switch (p->state) {
        case 0:
            p->timer++;
            if (p->timer < 10) {
                break;
            }
            p->actor.rad = 40.0f;
            p->state++;
        case 1:
            p->pos.x = lbl_1_data_140[p->slot].x;
            p->pos.z = lbl_1_data_140[p->slot].z;
            if (fn_1_1AE0() != 2) {
                break;
            }
            p->state++;
        case 2:
            if (fn_1_1AE0() != 2) {
                p->state++;
                p->timer = 0;
                p->stun = 1;
            } else if (p->type != 0) {
                fn_1_48C8(obj);
            } else {
                pad = GwPlayerConf[p->no].padNo;
                stickX = (s8)HuPadStkX[pad];
                stickY = -(s8)HuPadStkY[pad];
                p->btn = HuPadBtn[pad];
                p->btnDown = HuPadBtnDown[pad];
                p->dir = HuAtan(stickX, stickY);
                p->power = 0.017857144f * HuMagPoint2D(stickX, stickY);
                if (p->power > 1.0f) {
                    p->power = 1.0f;
                }
            }
            break;
        case 3:
            p->timer++;
            if (p->timer < 0x14) {
                break;
            }
            p->state++;
            fn_1_4E94(obj, 0);
        case 4:
            if (fn_1_1AE0() == 3 && fn_1_1CB0() != 0) {
                p->state = 0;
                obj->objFunc = fn_1_3C3C;
            }
            break;
    }
    p->vel.z = 0.0f;
    p->vel.y = 0.0f;
    p->vel.x = 0.0f;
    fn_1_4658(obj);
    fn_1_443C(obj);
    p->pos.y = fn_1_BFE8(p->pos.x, p->pos.z);
    PSVECAdd(&p->pos, &p->vel, &p->pos);
    fn_1_4380(obj);
}

void fn_1_3C3C(OMOBJ *obj)
{
    M503Player *p = obj->data;
    HuVecF otherPos;
    OMOBJ *other;
    f32 targetZ;
    f32 dx;
    f32 dz;
    f32 dist;
    f32 diff;
    s32 slotNo;

    p->btnDown = 0;
    p->btn = 0;
    p->dir = 0.0f;
    p->power = 0.0f;
    p->vel.z = 0.0f;
    p->vel.y = 0.0f;
    p->vel.x = 0.0f;
    switch (p->state) {
        case 0:
            fn_1_1B70(p->no);
            p->stun = 0;
            p->busy = 0;
            p->state++;
        case 1:
            fn_1_443C(obj);
            if (fn_1_1AF8() != 0) {
                p->state++;
                p->timer = 0;
            }
            break;
        case 2:
            p->timer++;
            if ((f32)p->timer > 300.0f) {
                p->actor.flags &= ~1;
            }
            if (fn_1_1B10() == 0) {
                targetZ = 0.0f;
                slotNo = fn_1_1CE8();
            } else if (fn_1_1B40(p->no) >= 0) {
                targetZ = 120.0f;
                slotNo = fn_1_1CE8();
            } else {
                targetZ = -200.0f;
                slotNo = fn_1_1D00();
            }
            dx = (f32)(0.0 + (-(f32)(85.0 * (slotNo - 1)) + 170.0 * fn_1_1D18()[p->no])) - p->pos.x;
            dz = targetZ - p->pos.z;
            dist = HuMagPoint2D(dx, dz);
            if (dist > 10.0f) {
                if (dist > 150.0f) {
                    p->power = 1.2f;
                } else if (dist > 50.0f) {
                    p->power = 0.8f;
                } else {
                    p->power = 0.4f;
                }
                p->dir = HuAtan(dx, dz);
                other = fn_1_53D4(&p->pos, p->dir, 30.0f, 100.0f);
                if (other != NULL && other != obj) {
                    fn_1_5708(other, &otherPos, NULL);
                    diff = fmod((f32)(p->dir - HuAtan(otherPos.x - p->pos.x, otherPos.z - p->pos.z)), 360.0);
                    if (diff < 0.0f) {
                        diff += 360.0f;
                    }
                    if (diff < 30.0f) {
                        p->dir = p->dir + 20.0;
                    } else {
                        p->dir = p->dir + -20.0;
                    }
                }
                fn_1_443C(obj);
                fn_1_1BA0(p->no, 0);
            } else {
                fn_1_1BA0(p->no, 1);
                if (fn_1_1BFC(1) != 0) {
                    p->state++;
                    p->timer = 0;
                }
            }
            break;
        case 3:
            p->dir = 0.0f;
            p->power = 0.2f;
            fn_1_443C(obj);
            p->timer++;
            if (p->timer >= 8) {
                p->state++;
                p->timer = 0;
                fn_1_1BA0(p->no, 2);
            }
            break;
        case 4:
            fn_1_443C(obj);
            if (fn_1_1BFC(2) != 0) {
                p->timer++;
                if (p->timer >= 0x14) {
                    p->voice = 0;
                    CharModelVoiceVolSet(p->charNo, 0x7F);
                    if (fn_1_1B10() != 0 && fn_1_1B40(p->no) >= 0) {
                        fn_1_4E94(obj, 3);
                    } else {
                        fn_1_4E94(obj, 4);
                        fn_1_1DD8();
                    }
                    p->state++;
                }
            }
            break;
        case 5:
            if (fn_1_5038(obj) != 0) {
                fn_1_1C80(p->no);
            }
            break;
    }
    p->pos.y = fn_1_BFE8(p->pos.x, p->pos.z);
    PSVECAdd(&p->pos, &p->vel, &p->pos);
    fn_1_4380(obj);
}

void fn_1_4380(OMOBJ *obj)
{
    M503Player *p = obj->data;
    f32 time;

    p->frame++;
    time = p->ovTime;
    p->ovTime = p->ovTime - 1.0;
    CharModelVoicePanSet(p->charNo, fn_1_1EB0(&p->pos));
    if (p->voice != 0) {
        CharModelVoiceVolSet(p->charNo, fn_1_1F44(&p->pos));
    }
    if (p->busy != 0 && fn_1_5038(obj) != 0) {
        p->busy = 0;
    }
}

void fn_1_443C(OMOBJ *obj)
{
    M503Player *p = obj->data;
    f32 power = p->power;
    f32 threshold;
    f32 speed;
    s32 idle = 0;
    s32 walk = 1;
    s32 run = 2;

    if (p->holding != 0) {
        power = power * 0.7;
        idle = 6;
        walk = 7;
        run = 8;
    }
    if (power > 0.05f && p->stun == 0 && p->busy == 0) {
        p->rotY = fn_1_2B7C(p->rotY, p->dir, 0.3f);
        p->vel.x = 0.016666668f * (550.0 * (power * HuSin(p->dir)));
        p->vel.z = 0.016666668f * (550.0 * (power * HuCos(p->dir)));
        if (fn_1_5104(obj) != 0) {
            threshold = 0.5f;
            if (p->motion == run) {
                threshold = 0.45f;
            }
            if (power > threshold) {
                fn_1_4E94(obj, run);
                speed = power;
            } else {
                fn_1_4E94(obj, walk);
                speed = power / threshold;
            }
            Hu3DMotionSpeedSet(*obj->mdlId, speed);
        }
    } else if (p->busy == 0) {
        fn_1_4E94(obj, idle);
    }
}

void fn_1_4658(OMOBJ *obj)
{
    M503Player *p = obj->data;
    OMOBJ *item;
    s16 hook;

    if (p->stun != 0) {
        if (p->holding != 0) {
            fn_1_575C(obj, 0);
            fn_1_4E94(obj, 9);
            p->busy = 1;
        }
        return;
    }
    if (p->busy == 0 && (p->btnDown & 0x100) != 0) {
        if (p->holding != 0) {
            fn_1_575C(obj, 1);
            fn_1_4E94(obj, 9);
            p->busy = 1;
            fn_1_1FD8(0x63F, &p->pos);
            return;
        }
        item = fn_1_9D18(&p->pos, p->rotY, 45.0f, 100.0f);
        if (item != NULL) {
            hook = fn_1_A368(item, obj);
            if (hook >= 0) {
                p->holding = 1;
                p->item = item;
                Hu3DModelHookSet(*obj->mdlId, CharModelItemHookGet(p->charNo, 8, 0), hook);
                fn_1_4E94(obj, 5);
                p->busy = 1;
                fn_1_1FD8(0x63E, &p->pos);
            }
        }
    }
}

void fn_1_47C8(OMOBJ *obj)
{
    M503Player *p = obj->data;
    f64 scale;

    Hu3DModelPosSetV(*obj->mdlId, &p->pos);
    Hu3DModelRotSet(*obj->mdlId, 0.0f, p->rotY, 0.0f);
    switch (p->charNo) {
        case 5:
        case 2:
            scale = 15.0f;
            break;
        case 8:
            scale = 10.0f;
            break;
        default:
            scale = 20.0f;
            break;
    }
    if (p->holding != 0) {
        scale = (f32)(scale + 10.0);
    }
    fn_1_C424(p->pos.x, p->pos.z, scale);
}

void fn_1_48C8(OMOBJ *obj)
{
    M503Player *p = obj->data;
    M503AiInfo *info = &lbl_1_data_170[p->level];
    HuVecF dest;
    s32 changed = 0;
    f32 power = info->power;
    f32 ang;
    f32 rnd;
    f32 dx;
    f32 dz;
    f32 dist;

#ifdef TARGET_PC
    dest.x = dest.y = dest.z = 0.0f;
#endif
    if (p->prevHold != p->holding) {
        p->prevHold = p->holding;
        changed = 1;
    }
    if (p->holding == 0) {
        if (changed != 0) {
            p->wait = info->wait;
        }
        if (p->wait <= 0) {
            if (p->target != NULL && (fn_1_A080(p->target, NULL, NULL) == 0 || p->targetTime > info->retarget)) {
                p->target = NULL;
            }
            if (p->target == NULL) {
                p->target = fn_1_9D18(&p->pos, p->rotY, info->range, info->dist);
                p->targetTime = 0;
            }
            if (p->target != NULL) {
                fn_1_A080(p->target, &dest, NULL);
                p->btnDown |= 0x100;
            } else {
                ang = (f32)(1.2 * (f32)p->frame) + 90.0f * (f32)p->no;
                dest.x = 100.0 * (3.2 * HuSin(ang));
                dest.z = 100.0 * (3.2 * HuCos(ang));
                power = 0.7f;
            }
        }
    } else {
        p->target = NULL;
        if (changed != 0) {
            p->wait = info->holdWait;
            if (1.52587890625e-05 * (f64)(f32)(s32)frand() <= info->chance) {
                fn_1_A080(p->item, NULL, &p->dest);
            } else {
                rnd = 1.52587890625e-05 * (f64)(f32)(s32)frand();
                fn_1_A338((s32)(4.0 * rnd), &p->dest);
            }
        }
        if (p->wait <= 0) {
            dest.x = p->dest.x;
            dest.y = p->dest.y;
            dest.z = p->dest.z;
            dx = dest.x - p->pos.x;
            dz = dest.z - p->pos.z;
            dist = HuMagPoint2D(dx, dz);
            if ((dest.z < 0.0f && dist < 190.0) || (dest.z >= 0.0f && dist < 240.0)) {
                p->btnDown |= 0x100;
            }
        }
    }
    if (p->wait <= 0) {
        p->dir = HuAtan(dest.x - p->pos.x, dest.z - p->pos.z);
        p->power = power;
    } else {
        p->wait--;
    }
    p->targetTime++;
}

void fn_1_4E94(OMOBJ *obj, u32 motion)
{
    M503Player *p = obj->data;
    u32 attr = 0;
    HU3DMODELID model = *obj->mdlId;
    f32 shift;
    u32 motAttr;
    s16 overlay;

    if ((u32)p->motion != motion && motion < 10) {
        if (p->motion < 0) {
            shift = 0.0f;
        } else {
            shift = 60.0f * lbl_1_data_C8[motion].shift;
        }
        p->motion = motion;
        motAttr = lbl_1_data_C8[motion].attr;
        Hu3DModelAttrReset(model, 0x40000A05);
        CharMotionShiftSet(p->charNo, obj->mtnId[lbl_1_data_C8[motion].mot], 0.0f, shift, motAttr);
        overlay = lbl_1_data_C8[motion].overlay;
        if (overlay >= 0) {
            Hu3DMotionOverlaySet(model, obj->mtnId[overlay]);
            p->ovTime = Hu3DMotionOverlayMaxTimeGet(model);
            if ((motAttr & 0x40000001) != 0) {
                attr = 0x40000200;
            }
            if ((motAttr & 0x40000004) != 0) {
                attr = attr | 0x40000000 | 0x800;
            }
            Hu3DModelAttrSet(model, attr);
            return;
        }
        Hu3DMotionOverlayReset(model);
    }
}

s32 fn_1_5038(OMOBJ *obj)
{
    M503Player *p = obj->data;

    if (p->motion == -1) {
        return 1;
    }
    if (lbl_1_data_C8[p->motion].overlay >= 0) {
        if (p->ovTime <= 0.0f) {
            return 1;
        }
        return 0;
    }
    if (CharMotionEndCheck(p->charNo) != 0 && CharMotionShiftIDGet(p->charNo) < 0) {
        return 1;
    }
    return 0;
}

s32 fn_1_5104(OMOBJ *obj)
{
    s32 ret = 0;

    if (CharMotionShiftIDGet(((M503Player *)obj->data)->charNo) < 0) {
        ret = 1;
    }
    return ret;
}

s32 fn_1_5164(OMOBJ *obj, u32 motion)
{
    s32 ret = 0;

    if (fn_1_5038(obj) != 0) {
        fn_1_4E94(obj, motion);
        ret = 1;
    }
    return ret;
}

s32 fn_1_53B8(OMOBJ *obj)
{
    M503Player *p = obj->data;

    return p->no;
}

OMOBJ *fn_1_53D4(HuVecF *pos, f32 yaw, f32 fov, f32 maxDist)
{
    OMOBJ *best = NULL;
    f32 bestDist = 100000.0f;
    OMOBJ *cur;
    M503Player *p;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 ang;
    f32 dist;
    s32 i;

    for (i = 0; i < 4; i++) {
        cur = lbl_1_bss_30[i];
        p = cur->data;
        dx = p->pos.x - pos->x;
        dy = p->pos.y - pos->y;
        dz = p->pos.z - pos->z;
        if (fov < 180.0) {
            ang = fmod((f32)HuAtan(dx, dz) - yaw, 360.0);
            if (ang < 0.0f) {
                ang += 360.0f;
            }
            if (ang > fov && ang < 360.0f - fov) {
                continue;
            }
        }
        dist = HuMagPoint3D(dx, dy, dz);
        if (!(dist > maxDist) && dist < bestDist) {
            bestDist = dist;
            best = cur;
        }
    }
    return best;
}

s32 fn_1_5708(OMOBJ *obj, HuVecF *pos, f32 *rotY)
{
    M503Player *p = obj->data;

    if (pos != NULL) {
        pos->x = p->pos.x;
        pos->y = p->pos.y;
        pos->z = p->pos.z;
    }
    if (rotY != NULL) {
        *rotY = p->rotY;
    }
    return p->holding;
}

s32 fn_1_575C(OMOBJ *obj, s32 mode)
{
    M503Player *p = obj->data;
    HU3DMODELID model = *obj->mdlId;
    char *hook;
    HuVecF pos;

    if (p->holding == 0) {
        return 0;
    }
    hook = CharModelItemHookGet(p->charNo, 8, 0);
    Hu3DModelHookObjReset(model, hook);
    Hu3DModelObjPosGet(model, hook, &pos);
    fn_1_A438(p->item, &pos, mode);
    p->holding = 0;
    return 1;
}
