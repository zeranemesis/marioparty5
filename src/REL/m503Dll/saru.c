#include "types.h"
#include "game/object.h"
#include "game/hu3d.h"
#include "game/data.h"
#include "game/audio.h"
#include "game/frand.h"
#include "game/memory.h"
#include "humath.h"
#include "REL/m503Dll.h"
#include <string.h>

#ifndef M_PI
#define M_PI 3.141592653589793
#endif

double atan2(double, double);
double fmod(double, double);

/* Work area of one monkey object (0xD8 bytes on GC). */
typedef struct {
    s32 type;
    s32 kind;
    s32 index;
    s32 motion;
    s32 state;
    u32 timer;
    s32 sub;
    s32 subTimer;
    s32 flag20;
    s32 cnt24;
    s32 cnt28;
    s32 unk2C;
    s32 frame;
    s32 seId;
    HuVecF pos;
    HuVecF vel;
    f32 rotY;
    s32 unk54;
    s32 attract;
    s32 slot;
    f32 time60;
    s32 owner;
    s32 inGoal;
    s32 goalIdx;
    s32 goalReady;
    HuVecF goal;
    s32 held;
    OMOBJ *holder;
    s32 airborne;
    HuVecF velTarget;
    HuVecF target;
    f32 targetYaw;
    M503Actor actor;
    M503Ring ring;
} M503Saru;

typedef struct {
    u32 file;
    u16 count;
    s16 grp;
    u32 motCount;
    OMOBJFUNC func;
} M503SaruInfo;

typedef struct {
    f32 shift;
    u32 attr;
} M503SaruMot;

extern s32 fn_1_DF30(void *actor);
extern void fn_1_D6D4(s32 playerNo, s32 add);
extern s32 fn_1_1AE0(void);
extern s32 fn_1_1BFC(s32 mode);
extern s32 fn_1_1D30(void);
extern s32 fn_1_1FD8(s32 seId, HuVecF *pos);
extern void fn_1_210C(s32 seNo, HuVecF *pos);
extern f32 fn_1_2B7C(f32 from, f32 to, f32 rate);
extern s32 fn_1_53B8(OMOBJ *obj);
extern OMOBJ *fn_1_53D4(HuVecF *pos, f32 yaw, f32 fov, f32 maxDist);
extern s32 fn_1_5708(OMOBJ *obj, HuVecF *pos, f32 *rotY);
extern f32 fn_1_BFE8(f32 x, f32 z);
extern void fn_1_C424(f32 x, f32 z, f32 scale);

void fn_1_5810(OMOBJMAN *objMan);
void fn_1_5D4C(void);
void fn_1_5D50(OMOBJ *obj);
void fn_1_5D60(OMOBJ *obj);
void fn_1_5F50(OMOBJ *obj);
void fn_1_5FF0(OMOBJ *obj);
void fn_1_6044(OMOBJ *obj);
void fn_1_60EC(OMOBJ *obj);
s32 fn_1_63C8(OMOBJ *obj);
void fn_1_6458(OMOBJ *obj);
void fn_1_6618(OMOBJ *obj);
void fn_1_6C0C(OMOBJ *obj, s32 state);
s32 fn_1_6F8C(OMOBJ *obj);
s32 fn_1_6F94(OMOBJ *obj);
s32 fn_1_6FC4(OMOBJ *obj);
s32 fn_1_721C(OMOBJ *obj);
s32 fn_1_780C(OMOBJ *obj);
s32 fn_1_7A78(OMOBJ *obj);
s32 fn_1_7C8C(OMOBJ *obj);
s32 fn_1_7D6C(OMOBJ *obj);
s32 fn_1_82B4(OMOBJ *obj);
s32 fn_1_8AD8(OMOBJ *obj);
s32 fn_1_8EA0(OMOBJ *obj);
s32 fn_1_9210(OMOBJ *obj);
s32 fn_1_961C(OMOBJ *obj);
void fn_1_98D0(OMOBJ *obj);
void fn_1_9A04(OMOBJ *obj, s32 motion);
s32 fn_1_9B00(OMOBJ *obj);

M503SaruInfo lbl_1_data_1E0[9] = {
    { 0x3D0005, 1, 0, 2, fn_1_6044 },
    { 0x3D0006, 1, 0, 2, fn_1_6044 },
    { 0x3D0007, 1, 0, 2, fn_1_6044 },
    { 0x3D0008, 1, 0, 2, fn_1_6044 },
    { 0x3D0009, 5, 1, 0xB, fn_1_6458 },
    { 0x3D000A, 5, 1, 0xB, fn_1_6458 },
    { 0x3D000B, 5, 1, 0xB, fn_1_6458 },
    { 0x3D000C, 5, 1, 0xB, fn_1_6458 },
    { 0x3D000D, 1, 2, 0xB, fn_1_6458 },
};

u32 lbl_1_data_270[3][11] = {
    { 0x3D000E, 0x3D000F },
    { 0x3D0010, 0x3D0011, 0x3D0012, 0x3D0013, 0x3D0014, 0x3D0015, 0x3D0016, 0x3D0017, 0x3D0018, 0x3D0019, 0x3D001A },
    { 0x3D001B, 0x3D001C, 0x3D001D, 0x3D001E, 0x3D001F, 0x3D0020, 0x3D0021, 0x3D0022, 0x3D0023, 0x3D0024, 0x3D0025 },
};

M503SaruMot lbl_1_data_2F4[2] = {
    { 0.2f, 0x40000001 },
    { 0.2f, 0x40000001 },
};

M503SaruMot lbl_1_data_304[11] = {
    { 0.1f, 0x40000001 },
    { 0.1f, 0x40000001 },
    { 0.1f, 0x40000001 },
    { 0.1f, 0 },
    { 0.1f, 0x40000001 },
    { 0.1f, 0x40000001 },
    { 0.1f, 0 },
    { 0.1f, 0 },
    { 0.1f, 0 },
    { 0.1f, 0x40000001 },
    { 0.1f, 0 },
};

HuVecF lbl_1_data_35C[4] = {
    { -820.0f, 55.0f, -740.0f },
    { 820.0f, 55.0f, -740.0f },
    { 820.0f, 55.0f, 820.0f },
    { -820.0f, 55.0f, 820.0f },
};

HuVecF lbl_1_data_38C[4] = {
    { -370.0f, 90.0f, -325.0f },
    { 370.0f, 90.0f, -325.0f },
    { 370.0f, 90.0f, 400.0f },
    { -370.0f, 90.0f, 400.0f },
};

f32 lbl_1_data_3BC[4] = { 45.0f, 315.0f, 225.0f, 135.0f };

HuVecF lbl_1_data_3CC[4] = {
    { -510.0f, 265.0f, -380.0f },
    { 510.0f, 265.0f, -380.0f },
    { 520.0f, 265.0f, 390.0f },
    { -520.0f, 265.0f, 390.0f },
};

f32 lbl_1_data_3FC[4] = { 45.0f, 315.0f, 225.0f, 135.0f };

s32 lbl_1_data_40C[5] = { 1, 1, 1, 1, 3 };

HuVecF lbl_1_data_420 = { 0.0f, 0.0f, 0.0f };

HuVecF lbl_1_data_458[4] = {
    { -700.0f, 300.0f, 0.0f },
    { 700.0f, 300.0f, 0.0f },
    { 0.0f, 300.0f, 700.0f },
    { 0.0f, 0.0f, 0.0f },
};

f32 lbl_1_data_488[3] = { 90.0f, 270.0f, 180.0f };
f32 lbl_1_data_494[5] = { 90.0f, 90.0f, 50.0f, 90.0f, 180.0f };
f32 lbl_1_data_4A8[5] = { 320.0f, 320.0f, 200.0f, 320.0f, 350.0f };
s32 lbl_1_data_4BC[5] = { 0xB4, 0xB4, 0xB4, 0x3C, 0x5A };
f32 lbl_1_data_4D0[5] = { 0.2f, 5.0f, 0.2f, 0.2f, -1.0f };
f32 lbl_1_data_4E4[5] = { 5.0000005f, 3.3333335f, 2.5000002f, 3.3333335f, 5.8333335f };
f32 lbl_1_data_4F8[5] = { 10.000001f, 6.666667f, 5.0000005f, 6.666667f, 10.833334f };
f32 lbl_1_data_50C[4] = { 200.0f, 200.0f, 250.0f, 250.0f };

s32 lbl_1_bss_40;
OMOBJ *lbl_1_bss_44[0x16];
OMOBJ *lbl_1_bss_9C[5];
OMOBJ *lbl_1_bss_B0;

void fn_1_5810(OMOBJMAN *objMan)
{
    s16 slotUsed[4];
    s16 kindUsed[5];
    s16 motIds[3][11];
    M503SaruInfo *info;
    M503Saru *w;
    OMOBJ *obj;
    HU3DMODELID first;
    HU3DMODELID model;
    u32 *motTbl;
    s16 *motSlot;
    s32 n9C = 0;
    s32 n44 = 0;
    s32 type;
    s32 count;
    s32 k;
    s32 m;
    s32 j;
    s32 r;

    info = lbl_1_data_1E0;
    lbl_1_bss_B0 = omAddObjEx(objMan, 0x32, 0, 0, OM_GRP_NONE, fn_1_5D50);
    for (j = 0; j < 4; j++) {
        slotUsed[j] = 0;
    }
    for (j = 0; j < 5; j++) {
        kindUsed[j] = 0;
    }
    for (type = 0; type < 3; type++) {
        for (j = 0; j < 0xB; j++) {
            motIds[type][j] = -1;
        }
    }
    for (type = 0; type < 9; type++, info++) {
        count = info->count;
        if (type >= 4 && type <= 7) {
            count++;
        }
        for (k = 0; k < count; k++) {
            obj = omAddObjEx(objMan, 0x28, 1, (u16)info->motCount, 0, info->func);
            obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M503Saru), HU_MEMNUM_OVL);
            w = obj->data;
            memset(w, 0, sizeof(M503Saru));
            obj->stat |= 0x100;
            w->type = type;
            w->index = k;
            w->motion = -1;
            w->seId = -1;
            w->state = 0;
            w->vel.z = 0.0f;
            w->vel.y = 0.0f;
            w->vel.x = 0.0f;
            if (k == 0) {
                first = Hu3DModelCreate(HuDataSelHeapReadNum(info->file, HU_MEMNUM_OVL, HEAP_MODEL));
                model = first;
            } else {
                model = Hu3DModelLink(first);
            }
            *obj->mdlId = model;
            motTbl = lbl_1_data_270[info->grp];
            motSlot = motIds[info->grp];
            for (m = 0; m < (s32)info->motCount; m++) {
                if (*motSlot < 0) {
                    *motSlot = Hu3DJointMotion(model, HuDataSelHeapReadNum(*motTbl, HU_MEMNUM_OVL, HEAP_MODEL));
                }
                obj->mtnId[m] = *motSlot;
                motTbl++;
                motSlot++;
            }
            if (type < 4) {
                w->owner = type;
                lbl_1_bss_9C[n9C++] = obj;
                Hu3DModelLayerSet(model, 7);
                r = (s32)(4.0 * (1.52587890625e-05 * (f64)(f32)(s32)frand()));
                for (j = 0; j < 4; j++) {
                    if (r >= 4) {
                        r = 0;
                    }
                    if (slotUsed[r] == 0) {
                        break;
                    }
                    r++;
                }
#ifdef TARGET_PC
                if (r >= 4) {
                    r = 0;
                }
#endif
                slotUsed[r]++;
                w->slot = r;
            } else {
                if (type == 8) {
                    w->kind = 4;
                } else {
                    w->kind = (s32)(4.0 * (1.52587890625e-05 * (f64)(f32)(s32)frand()));
                    r = (s32)(3.0 * (1.52587890625e-05 * (f64)(f32)(s32)frand()));
                    for (j = 0; j < 4; j++) {
                        if (r >= 4) {
                            r = 0;
                        }
                        if (kindUsed[r] < 5) {
                            break;
                        }
                        r++;
                    }
#ifdef TARGET_PC
                    if (r >= 4) {
                        r = 0;
                    }
#endif
                    kindUsed[r]++;
                    w->kind = r;
                }
                w->owner = type - 4;
                Hu3DModelScaleSet(model, 0.8f, 0.8f, 0.8f);
                Hu3DModelLayerSet(model, 7);
                if ((u32)w->owner < 4U && k == count - 1) {
                    w->index = 0;
                    obj->objFunc = fn_1_5F50;
                } else {
                    lbl_1_bss_44[n44++] = obj;
                }
            }
        }
    }
    lbl_1_bss_9C[n9C] = NULL;
    lbl_1_bss_44[n44] = NULL;
}

void fn_1_5D4C(void)
{
}

void fn_1_5D50(OMOBJ *obj)
{
    obj->objFunc = fn_1_5D60;
}

void fn_1_5D60(OMOBJ *obj)
{
    f32 ang[4];
    OMOBJ *cur;
    M503Saru *w;
    f32 a;
    s32 g;
    s32 i;

    lbl_1_bss_40 = 0;
    for (i = 0; i < 4; i++) {
        ang[i] = lbl_1_data_3BC[i] - 24.0;
    }
    for (i = 0; i < 0x15; i++) {
        cur = lbl_1_bss_44[i];
        if (cur == NULL) {
            break;
        }
        w = cur->data;
        if (w->inGoal == 0) {
            lbl_1_bss_40++;
        } else {
            g = w->goalIdx;
            a = ang[g];
            w->goal.x = lbl_1_data_35C[g].x + 100.0 * (5.0 * HuSin(a));
            w->goal.y = lbl_1_data_35C[g].y;
            w->goal.z = lbl_1_data_35C[g].z + 100.0 * (5.0 * HuCos(a));
            w->goalReady = 1;
            ang[g] = a + 10.0;
        }
    }
}

void fn_1_5F50(OMOBJ *obj)
{
    M503Saru *w = obj->data;

    w->pos.x = obj->trans.x = lbl_1_data_3CC[w->owner].x;
    w->pos.y = obj->trans.y = lbl_1_data_3CC[w->owner].y;
    w->pos.z = obj->trans.z = lbl_1_data_3CC[w->owner].z;
    w->rotY = lbl_1_data_3FC[w->owner];
    obj->objFunc = fn_1_5FF0;
}

void fn_1_5FF0(OMOBJ *obj)
{
    M503Saru *w = obj->data;

    fn_1_9A04(obj, 9);
    fn_1_98D0(obj);
    w->frame++;
}

void fn_1_6044(OMOBJ *obj)
{
    M503Saru *w = obj->data;

    w->pos.x = obj->trans.x = lbl_1_data_38C[w->type].x;
    w->pos.y = obj->trans.y = lbl_1_data_38C[w->type].y;
    w->pos.z = obj->trans.z = lbl_1_data_38C[w->type].z;
    w->rotY = lbl_1_data_3BC[w->type];
    w->state = 0;
    obj->objFunc = fn_1_60EC;
}

void fn_1_60EC(OMOBJ *obj)
{
    M503Saru *w = obj->data;
    HU3DMODELID model = *obj->mdlId;
    f32 time;

    switch (w->state) {
        case 0:
            w->state++;
            fn_1_9A04(obj, 0);
        case 1:
            if (fn_1_1AE0() != 2) {
                break;
            }
            w->state++;
            w->timer = 300.0 + (180.0 * (f64)w->slot + (f64)(frand() & 0x3F));
        case 2:
            if (fn_1_1AE0() != 2) {
                w->state = 4;
            } else {
                w->timer--;
                if ((s32)w->timer <= 0) {
                    if (fn_1_63C8(obj) == 0) {
                        w->timer = 0xB4;
                    } else {
                        fn_1_9A04(obj, 1);
                        fn_1_1FD8(0x642, &w->pos);
                        w->attract = 1;
                        w->time60 = 100.0f;
                        w->state++;
                        w->timer = 0x78;
                    }
                }
            }
            break;
        case 3:
            if (fn_1_1AE0() != 2) {
                w->state = 4;
            } else {
                if (Hu3DMotionShiftIDGet(model) == -1) {
                    time = Hu3DMotionTimeGet(model);
                    if (time < w->time60) {
                        fn_1_1FD8(0x642, &w->pos);
                    }
                    w->time60 = time;
                }
                w->timer--;
                if ((s32)w->timer <= 0) {
                    fn_1_9A04(obj, 0);
                    w->attract = 0;
                    w->state--;
                    w->timer = 480.0 + (f64)frand();
                }
            }
            break;
        case 4:
            fn_1_9A04(obj, 0);
            w->attract = 0;
            w->state++;
            break;
    }
    fn_1_98D0(obj);
    w->frame++;
}

s32 fn_1_63C8(OMOBJ *obj)
{
    M503Saru *me = obj->data;
    OMOBJ *cur;
    M503Saru *w;
    s32 cnt = 0;
    s32 i;

    for (i = 0; i < 0x15; i++) {
        cur = lbl_1_bss_44[i];
        if (cur == NULL) {
            break;
        }
        w = cur->data;
        if (w->inGoal == 0 && (u32)me->type == (u32)w->owner) {
            cnt++;
        }
    }
    return cnt;
}

void fn_1_6458(OMOBJ *obj)
{
    M503Saru *w = obj->data;

    w->pos.x = obj->trans.x = 300.0 * (3.0517578125e-05 * (f64)(f32)((s32)frand() - 0x8000));
    w->pos.y = obj->trans.y = 0.0f;
    w->pos.z = obj->trans.z = 300.0 * (3.0517578125e-05 * (f64)(f32)((s32)frand() - 0x8000));
    w->rotY = 0.0f;
    w->actor.flags = 1;
    w->actor.unk04 = 1;
    w->actor.unk08 = 0;
    w->actor.state = 0;
    w->actor.owner = obj;
    w->actor.fn = fn_1_98D0;
    w->actor.pos = &w->pos;
    w->actor.rad = 30.0f;
    fn_1_DF30(&w->actor);
    w->ring.active = 0;
    w->ring.scale = 50.0f;
    w->ring.pos = &w->pos;
    w->ring.offset = &lbl_1_data_420;
    fn_1_D134(&w->ring);
    w->state = 0;
    obj->objFunc = fn_1_6618;
}

void fn_1_6618(OMOBJ *obj)
{
    M503Saru *w = obj->data;
    M503Saru *owner;
    HU3DMODELID model;
    f32 dx;
    f32 dz;
    f32 dist;
    f32 amb;
    f32 maxTime;

    dx = -470.0 - (f64)w->pos.x;
    dz = 570.0 - (f64)w->pos.z;
    dist = HuMagPoint2D(dx, dz);
    if (dist > 300.0f) {
        amb = 1.0f;
    } else {
        amb = 1.0 - ((300.0 - (f64)dist) / 150.0);
        if (amb < 0.2f) {
            amb = 0.2f;
        }
    }
    Hu3DModelAmbSet(*obj->mdlId, amb, amb, amb);
    if (w->held != 0) {
        fn_1_6C0C(obj, 7);
    }
    if (w->inGoal != 0) {
        fn_1_6C0C(obj, 8);
    }
    if (w->held == 0 && w->state != 8 && w->airborne == 0 && (u32)w->type != 8U) {
        owner = lbl_1_bss_9C[w->owner]->data;
        if (owner->attract != 0) {
            w->actor.flags |= 1;
            fn_1_6C0C(obj, 6);
        }
    }
    if ((u32)w->state <= 0xAU) {
        switch (w->state) {
            case 0:
                fn_1_9A04(obj, 0);
                model = *obj->mdlId;
                maxTime = Hu3DMotionMaxTimeGet(model) - 1.0;
                Hu3DMotionTimeSet(model, 1.52587890625e-05 * (f64)(f32)(s32)frand() * maxTime);
                if ((u32)w->type == 8U) {
                    fn_1_6C0C(obj, 3);
                } else {
                    fn_1_6C0C(obj, 1);
                }
                break;
            case 1:
                if (fn_1_6F8C(obj) != 0) {
                    fn_1_6C0C(obj, 2);
                }
                break;
            case 2:
                fn_1_6F94(obj);
                if (fn_1_1AE0() == 2) {
                    w->timer++;
                    if (w->timer >= (u32)((w->index + 1) * 0xA)) {
                        fn_1_6C0C(obj, 4);
                    }
                }
                break;
            case 3:
                if (fn_1_6FC4(obj) != 0) {
                    fn_1_6C0C(obj, 4);
                }
                break;
            case 4:
                if (fn_1_1AE0() != 2) {
                    fn_1_6C0C(obj, 9);
                } else if (fn_1_721C(obj) != 0) {
                    if ((s32)(frand() & 0x3F) == 0) {
                        fn_1_6C0C(obj, 5);
                    } else if ((s32)(frand() & 0x3F) == 0) {
                        fn_1_6C0C(obj, 0xA);
                    }
                }
                break;
            case 5:
                if (fn_1_780C(obj) != 0) {
                    fn_1_6C0C(obj, 4);
                }
                break;
            case 6:
                if (fn_1_7A78(obj) != 0) {
                    fn_1_6C0C(obj, 4);
                }
                break;
            case 7:
                if (fn_1_7C8C(obj) != 0) {
                    fn_1_6C0C(obj, 4);
                }
                break;
            case 8:
                if (fn_1_7D6C(obj) != 0) {
                    fn_1_6C0C(obj, 4);
                }
                break;
            case 9:
                fn_1_82B4(obj);
                break;
            case 10:
                if (fn_1_8AD8(obj) != 0) {
                    fn_1_6C0C(obj, 4);
                }
                break;
        }
    }
    if (w->held == 0) {
        if (w->airborne == 0 && w->inGoal == 0) {
            w->pos.y = w->pos.y + 0.5 * (f64)(fn_1_BFE8(w->pos.x, w->pos.z) - w->pos.y);
        }
        PSVECAdd(&w->pos, &w->vel, &w->pos);
    }
    w->frame++;
}

void fn_1_6C0C(OMOBJ *obj, s32 state)
{
    M503Saru *w = obj->data;

    if (w->state != state) {
        w->state = state;
        w->timer = 0;
        w->sub = 0;
        w->subTimer = 0;
        w->cnt24 = 0;
        w->cnt28 = 0;
    }
}

void fn_1_6C5C(OMOBJ *obj, f32 targetX, f32 targetZ, f32 rate)
{
    M503Saru *w = obj->data;

    w->vel.x = w->vel.x + rate * (targetX - w->vel.x);
    w->vel.z = w->vel.z + rate * (targetZ - w->vel.z);
}

void fn_1_6CA4(OMOBJ *obj, f32 ang, f32 speed, f32 rate)
{
    M503Saru *w;
    f32 z;

    z = speed * HuCos(ang);
    w = obj->data;
    w->vel.x = w->vel.x + rate * ((f32)(speed * HuSin(ang)) - w->vel.x);
    w->vel.z = w->vel.z + rate * (z - w->vel.z);
}

void fn_1_6DBC(OMOBJ *obj, f32 rate)
{
    M503Saru *w = obj->data;

    w->rotY = fn_1_2B7C(w->rotY, HuAtan(w->vel.x, w->vel.z), rate);
}

void fn_1_6E48(OMOBJ *obj, f32 speed, f32 rate)
{
    M503Saru *w;
    f32 ang;
    f32 z;

    ang = ((M503Saru *)obj->data)->rotY;
    z = (f64)(0.016666668f * speed) * HuCos(ang);
    w = obj->data;
    w->vel.x = w->vel.x + rate * ((f32)((f64)(0.016666668f * speed) * HuSin(ang)) - w->vel.x);
    w->vel.z = w->vel.z + rate * (z - w->vel.z);
}

s32 fn_1_6F8C(OMOBJ *obj)
{
    return 1;
}

s32 fn_1_6F94(OMOBJ *obj)
{
    fn_1_9A04(obj, 0);
    return 0;
}

s32 fn_1_6FC4(OMOBJ *obj)
{
    M503Saru *w = obj->data;
    HU3DMODELID model = *obj->mdlId;
    f32 ang;
    s32 idx;

    switch (w->sub) {
        case 0:
            w->sub++;
            w->vel.z = 0.0f;
            w->vel.y = 0.0f;
            w->vel.x = 0.0f;
            w->airborne = 1;
            w->actor.flags &= ~1;
            Hu3DModelAttrSet(model, 1);
            return 0;
        case 1:
            if (fn_1_1D30() > 0xA) {
                return 0;
            }
            w->sub++;
            w->cnt24 = 0;
            w->cnt28 = 0;
            idx = (s32)(3.0 * (1.52587890625e-05 * (f64)(f32)(s32)frand()));
            ang = lbl_1_data_488[idx];
            w->velTarget.y = 28.333334f;
            w->velTarget.x = 26.666668f * HuSin(ang);
            w->velTarget.z = 26.666668f * HuCos(ang);
            w->pos = lbl_1_data_458[idx];
            Hu3DModelAttrReset(model, 1);
            w->seId = fn_1_1FD8(0x643, &w->pos);
        case 2:
            if (fn_1_961C(obj) != 0) {
                w->actor.flags |= 1;
                return 1;
            }
            return 0;
        default:
            return 0;
    }
}

s32 fn_1_721C(OMOBJ *obj)
{
    M503Saru *w = obj->data;
    OMOBJ *other;
    HuVecF tgt;
    f32 dist0;
    f32 dx;
    f32 dz;
    f32 d;
    f32 speed;
    f32 ang;
    f32 rad;
    f32 base;

    switch (w->sub) {
        case 0:
            if (fn_1_53D4(&w->pos, w->rotY, lbl_1_data_494[w->kind], lbl_1_data_4A8[w->kind]) == NULL
                && fn_1_53D4(&w->pos, w->rotY, 180.0f, 150.0f) == NULL) {
                fn_1_8EA0(obj);
                return 1;
            }
            w->sub++;
            w->cnt24 = 0;
            w->cnt28 = 0;
        case 1:
            dist0 = lbl_1_data_4A8[w->kind];
            other = fn_1_53D4(&w->pos, w->rotY, 180.0f, 30.0 + dist0);
            if (other == NULL) {
                w->sub--;
                return 1;
            }
            d = HuMagPoint2D(w->pos.x, w->pos.z);
            if (d < (f32)(50.0 + dist0)) {
                fn_1_5708(other, &tgt, NULL);
                dx = w->pos.x - tgt.x;
                dz = w->pos.z - tgt.z;
                d = HuMagPoint2D(dx, dz);
                speed = (f32)((30.0 + dist0) - d) / 5.0;
                if ((w->index & 1) != 0) {
                    speed = -speed;
                }
                ang = speed + HuAtan(dx, dz);
                rad = 30.0 + dist0;
            } else {
                base = HuAtan(-w->pos.x, -w->pos.z);
                if ((w->index & 1) != 0) {
                    ang = base + 90.0f;
                } else {
                    ang = base - 90.0f;
                }
                tgt.x = w->pos.x;
                tgt.y = w->pos.y;
                tgt.z = w->pos.z;
                rad = 100.0f;
            }
            w->target.x = (f64)tgt.x + (f64)rad * HuSin(ang);
            w->target.z = (f64)tgt.z + (f64)rad * HuCos(ang);
            w->targetYaw = 0.0f;
            fn_1_9210(obj);
        default:
            return 0;
    }
}

s32 fn_1_780C(OMOBJ *obj)
{
    M503Saru *w = obj->data;
    M503Saru *w2;
    s32 yaw;

    switch (w->sub) {
        case 0:
            w->sub++;
            w->subTimer = 0;
            w->flag20 = frand() & 1;
            fn_1_9A04(obj, 0);
            break;
        case 1:
            w->subTimer++;
            if (!((f32)w->subTimer < 12.0f)) {
                w->sub++;
                if (w->flag20 == 0) {
                    fn_1_9A04(obj, 6);
                } else {
                    fn_1_9A04(obj, 7);
                }
            }
            break;
        case 2:
            if (fn_1_9B00(obj) != 0) {
                w->sub++;
                w->subTimer = 0;
                fn_1_9A04(obj, 0);
            }
            break;
        case 3:
            w->subTimer++;
            if (!((f32)w->subTimer < 12.0f)) {
                return 1;
            }
            break;
    }
    if (w->flag20 == 0) {
        yaw = 0;
    } else {
        yaw = 0xB4;
    }
    w->rotY = fn_1_2B7C(w->rotY, yaw, 0.2f);
    w2 = obj->data;
    w2->vel.x = w2->vel.x + 0.3f * (0.0f - w2->vel.x);
    w2->vel.z = w2->vel.z + 0.3f * (0.0f - w2->vel.z);
    return 0;
}

s32 fn_1_7A78(OMOBJ *obj)
{
    M503Saru *w = obj->data;
    M503Saru *owner;
    M503Saru *w2;

    if ((u32)w->type == 8U) {
        return 1;
    }
    owner = lbl_1_bss_9C[w->owner]->data;
    switch (w->sub) {
        case 0:
            w->sub++;
            fn_1_9A04(obj, 8);
            break;
        case 1:
            if (fn_1_9B00(obj) != 0) {
                w->sub++;
                fn_1_9A04(obj, 9);
            }
            break;
        case 2:
            if (owner->attract == 0) {
                w->sub++;
                fn_1_9A04(obj, 0xA);
            }
            break;
        case 3:
            if (fn_1_9B00(obj) != 0) {
                return 1;
            }
            break;
    }
    w->rotY = fn_1_2B7C(w->rotY, HuAtan(owner->pos.x - w->pos.x, owner->pos.z - w->pos.z), 0.2f);
    w2 = obj->data;
    w2->vel.x = w2->vel.x + 0.3f * (0.0f - w2->vel.x);
    w2->vel.z = w2->vel.z + 0.3f * (0.0f - w2->vel.z);
    return 0;
}

s32 fn_1_7C8C(OMOBJ *obj)
{
    M503Saru *w = obj->data;

    if (w->held == 0) {
        return 1;
    }
    switch (w->sub) {
        case 0:
            w->sub++;
            w->subTimer = 0;
            fn_1_9A04(obj, 4);
            break;
        case 1:
            w->subTimer++;
            if (w->subTimer >= lbl_1_data_4BC[w->kind]) {
                w->sub++;
                w->subTimer = 0;
                fn_1_9A04(obj, 5);
            }
            break;
    }
    return 0;
}

s32 fn_1_7D6C(OMOBJ *obj)
{
    M503Saru *w = obj->data;
    f32 ang;
    f32 speed;

    switch (w->sub) {
        case 0:
            if (w->goalReady == 0) {
                return 0;
            }
            w->sub++;
            w->cnt24 = 0;
            w->cnt28 = 0;
            ang = HuAtan(w->goal.x - w->pos.x, w->goal.z - w->pos.z);
            w->velTarget.y = 40.000004f;
            w->velTarget.x = 10.000001f * HuSin(ang);
            w->velTarget.z = 10.000001f * HuCos(ang);
        case 1:
            if (w->airborne == 0) {
                w->ring.active = 1;
            }
            if (fn_1_961C(obj) != 0) {
                w->sub++;
                w->subTimer = 0;
                w->cnt24 = 0;
                w->cnt28 = 0;
            }
            return 0;
        case 2:
            w->target.x = w->goal.x;
            w->target.y = w->goal.y;
            w->target.z = w->goal.z;
            w->targetYaw = lbl_1_data_3BC[w->goalIdx];
            fn_1_9210(obj);
            if (fn_1_1AE0() == 2 && fn_1_1D30() > 2) {
                w->subTimer++;
                if (!((f32)w->subTimer < 480.0f) && lbl_1_bss_40 < 8
                    && (lbl_1_bss_40 < 3
                        || !((f32)(100.0 * (1.52587890625e-05 * (f64)(f32)(s32)frand())) > lbl_1_data_4D0[w->kind]))) {
                    w->sub++;
                    w->cnt24 = 0;
                    w->cnt28 = 0;
                    w->inGoal = 0;
                    lbl_1_bss_40++;
                    ang = lbl_1_data_3BC[w->goalIdx] + 20.0 * (3.0517578125e-05 * (f64)(f32)((s32)frand() - 0x8000));
                    speed = 0.016666668f * (1000.0 + 400.0 * (1.52587890625e-05 * (f64)(f32)(s32)frand()));
                    w->velTarget.y = 0.016666668f * (2400.0 + 400.0 * (1.52587890625e-05 * (f64)(f32)(s32)frand()));
                    w->velTarget.x = (f64)speed * HuSin(ang);
                    w->velTarget.z = (f64)speed * HuCos(ang);
                    w->seId = fn_1_1FD8(0x641, &w->pos);
                }
            }
            return 0;
        case 3:
            if (w->airborne != 0) {
                w->ring.active = 0;
            }
            if (fn_1_961C(obj) != 0) {
                w->actor.flags |= 1;
                return 1;
            }
            return 0;
        default:
            return 0;
    }
}

s32 fn_1_82B4(OMOBJ *obj)
{
    M503Saru *w = obj->data;
    M503Saru *w2;
    f32 fx;
    f32 fz;
    f32 vx = 0.0f;
    f32 vz = 0.0f;
    f32 dist;

    if (w->sub == 0) {
        w->subTimer++;
        if ((u32)w->subTimer < (u32)(w->index * 0xA)) {
            w2 = obj->data;
            w2->vel.x = w2->vel.x + 0.1f * (0.0f - w2->vel.x);
            w2->vel.z = w2->vel.z + 0.1f * (0.0f - w2->vel.z);
            return 0;
        }
        w->sub++;
        w->target.x = w->pos.x;
        w->target.y = w->pos.y;
        w->target.z = w->pos.z;
        fx = w->pos.x;
        fz = w->pos.z;
        if (fx < 0.0f) {
            if (fx > -250.0f && (w->index & 1) != 0) {
                w->target.x = 800.0f;
            } else {
                w->target.x = -800.0f;
            }
        } else if (fx < 250.0f && (w->index & 1) != 0) {
            w->target.x = -800.0f;
        } else {
            w->target.x = 800.0f;
        }
        if (fz < 0.0f) {
            if (fz < -380.0f) {
                w->target.z = -380.0f;
            }
        } else if (fz > 300.0f) {
            w->target.z = 300.0f;
        }
        w->targetYaw = HuAtan(w->target.x - w->pos.x, w->target.z - w->pos.z);
    }
    fx = w->pos.x;
    fz = w->pos.z;
    dist = HuMagPoint2D(fx, fz);
    if (fn_1_53D4(&w->pos, 0.0f, 180.0f, 120.0f) != NULL || dist < 300.0f) {
        if (fn_1_1BFC(2) == 0) {
            if (fn_1_53D4(&w->pos, w->targetYaw, 45.0f, 120.0f) != NULL) {
                w->targetYaw = w->targetYaw + 20.0;
            }
            vx = 6.6666667f * HuSin(w->targetYaw);
            vz = 6.6666667f * HuCos(w->targetYaw);
        }
    } else {
        if (fx < 0.0f) {
            if (fx > -370.0f) {
                vx = -6.666667f;
            }
        } else if (fx < 370.0f) {
            vx = 6.666667f;
        }
        if (fz < 0.0f) {
            if (fz < -380.0f) {
                vz = 6.666667f;
            }
        } else if (fz > 300.0f) {
            vz = -6.666667f;
        }
        if (0.0 != vx || 0.0 != vz) {
            w->targetYaw = HuAtan(vx, vz);
        }
    }
    w2 = obj->data;
    w2->vel.x = w2->vel.x + 0.1f * (vx - w2->vel.x);
    w2->vel.z = w2->vel.z + 0.1f * (vz - w2->vel.z);
    dist = HuMagPoint2D(w->vel.x, w->vel.z);
    if (dist < 0.16666667f) {
        w->rotY = fn_1_2B7C(w->rotY, 0.0f, 0.08f);
        fn_1_9A04(obj, 0);
    } else {
        w2 = obj->data;
        w2->rotY = fn_1_2B7C(w2->rotY, HuAtan(w2->vel.x, w2->vel.z), 0.08f);
        fn_1_9A04(obj, 1);
    }
    return 0;
}

s32 fn_1_8AD8(OMOBJ *obj)
{
    M503Saru *w = obj->data;
    OMOBJ *other;
    HuVecF opos;
    f32 ang;
    f32 speed;

    switch (w->sub) {
        case 0:
            w->sub++;
            other = fn_1_53D4(&w->pos, w->rotY, 180.0f, 200.0f);
            if (other != NULL) {
                fn_1_5708(other, &opos, NULL);
                ang = 180.0 * (atan2((f64)(w->pos.x - opos.x), (f64)(w->pos.z - opos.z)) / M_PI);
                speed = 0.016666668f * (300.0 + 400.0 * (1.52587890625e-05 * (f64)(f32)(s32)frand()));
                w->velTarget.y = 0.016666668f * (1600.0 + 400.0 * (1.52587890625e-05 * (f64)(f32)(s32)frand()));
            } else {
                ang = 360.0 * (1.52587890625e-05 * (f64)(f32)(s32)frand());
                speed = 0.016666668f * (200.0 + 200.0 * (1.52587890625e-05 * (f64)(f32)(s32)frand()));
                w->velTarget.y = 0.016666668f * (1500.0 + 400.0 * (1.52587890625e-05 * (f64)(f32)(s32)frand()));
            }
            w->velTarget.x = (f64)speed * HuSin(ang);
            w->velTarget.z = (f64)speed * HuCos(ang);
        case 1:
            if (fn_1_961C(obj) != 0) {
                return 1;
            }
        default:
            return 0;
    }
}

s32 fn_1_8EA0(OMOBJ *obj)
{
    M503Saru *w = obj->data;
    M503Saru *w2;
    f32 speed;
    f32 ang;
    f32 z;

    fn_1_9A04(obj, 1);
    speed = HuMagPoint2D(w->vel.x, w->vel.z);
    ang = HuAtan(w->pos.x, w->pos.z) + (80.0f + 5.0f * (f32)w->index);
    z = 1.3333334f * HuCos(ang);
    w2 = obj->data;
    w2->vel.x = w2->vel.x + 0.1f * ((f32)(1.3333334f * HuSin(ang)) - w2->vel.x);
    w2->vel.z = w2->vel.z + 0.1f * (z - w2->vel.z);
    w2 = obj->data;
    w2->rotY = fn_1_2B7C(w2->rotY, HuAtan(w2->vel.x, w2->vel.z), 0.2f);
    return 0;
}

s32 fn_1_9210(OMOBJ *obj)
{
    M503Saru *w = obj->data;
    M503Saru *w2;
    s32 motion = 0;
    s32 result = 0;
    f32 dx;
    f32 dz;
    f32 dist;
    f32 speed;
    f32 ang;
    f32 z;

    dx = w->target.x - w->pos.x;
    dz = w->target.z - w->pos.z;
    dist = HuMagPoint2D(dx, dz);
    if (dist < 10.0f) {
        w->vel.z = 0.0f;
        w->vel.x = 0.0f;
        w->pos.x = (f64)w->pos.x + 0.1 * (f64)dx;
        w->pos.z = (f64)w->pos.z + 0.1 * (f64)dz;
        w->rotY = fn_1_2B7C(w->rotY, w->targetYaw, 0.1f);
        result = 1;
    } else {
        if (dist < 50.0f) {
            motion = 1;
            speed = lbl_1_data_4E4[w->kind];
        } else {
            motion = 2;
            speed = lbl_1_data_4F8[w->kind];
        }
        ang = HuAtan(dx, dz);
        z = (f64)speed * HuCos(ang);
        w2 = obj->data;
        w2->vel.x = w2->vel.x + 0.1f * ((f32)((f64)speed * HuSin(ang)) - w2->vel.x);
        w2->vel.z = w2->vel.z + 0.1f * (z - w2->vel.z);
        w2 = obj->data;
        w2->rotY = fn_1_2B7C(w2->rotY, HuAtan(w2->vel.x, w2->vel.z), 0.1f);
    }
    fn_1_9A04(obj, motion);
    return result;
}

s32 fn_1_961C(OMOBJ *obj)
{
    M503Saru *w = obj->data;
    f32 floorY;

    switch (w->cnt24) {
        case 0:
            w->cnt24++;
            w->cnt28 = 0;
            fn_1_9A04(obj, 0);
        case 1:
            w->rotY = fn_1_2B7C(w->rotY, HuAtan(w->velTarget.x, w->velTarget.z), 0.3f);
            w->cnt28++;
            if (!((f32)w->cnt28 < 6.0f)) {
                w->cnt24++;
                w->airborne = 1;
                w->vel.x = w->velTarget.x;
                w->vel.y = w->velTarget.y;
                w->vel.z = w->velTarget.z;
                fn_1_9A04(obj, 3);
            }
            return 0;
        case 2:
            w->vel.x = (f64)w->vel.x * 0.98;
            w->vel.y = (f64)w->vel.y - 2.0000001043081284;
            w->vel.z = (f64)w->vel.z * 0.98;
            if (!(w->vel.y > 0.0f)) {
                if (w->inGoal != 0) {
                    floorY = lbl_1_data_35C[w->goalIdx].y;
                } else {
                    floorY = fn_1_BFE8(w->pos.x, w->pos.z);
                }
                if (!(w->pos.y > floorY)) {
                    w->pos.y = floorY;
                    w->vel.z = 0.0f;
                    w->vel.y = 0.0f;
                    w->vel.x = 0.0f;
                    w->airborne = 0;
                    w->cnt24++;
                    w->cnt28 = 0;
                    fn_1_9A04(obj, 0);
                }
            }
            return 0;
        case 3:
            w->cnt28++;
            if (!((f32)w->cnt28 < 6.0f)) {
                return 1;
            }
            return 0;
        default:
            return 0;
    }
}

void fn_1_98D0(OMOBJ *obj)
{
    M503Saru *w = obj->data;
    HU3DMODELID model = *obj->mdlId;
    HuVecF pos;

    if (w->held != 0) {
        w->seId = -1;
        return;
    }
    pos.x = w->pos.x;
    pos.y = w->pos.y;
    pos.z = w->pos.z;
    if (w->seId != -1) {
        if (HuAudFXStatusGet(w->seId) == 2) {
            fn_1_210C(w->seId, &pos);
        } else {
            w->seId = -1;
        }
    }
    if ((u32)w->type < 4U) {
    } else {
        pos.y = pos.y + -8.0;
        if (w->inGoal == 0 && w->airborne == 0) {
            fn_1_C424(w->pos.x, w->pos.z, 15.0f);
        }
    }
    Hu3DModelPosSetV(model, &pos);
    Hu3DModelRotSet(model, 0.0f, w->rotY, 0.0f);
}

void fn_1_9A04(OMOBJ *obj, s32 motion)
{
    M503Saru *w = obj->data;
    HU3DMODELID model = *obj->mdlId;
    M503SaruMot *tbl;

    if (w->motion != motion) {
        if ((u32)w->type < 4U) {
            if (motion < 2) {
                tbl = lbl_1_data_2F4;
                goto apply;
            }
        } else if (motion < 0xB) {
            tbl = lbl_1_data_304;
apply:
            if (w->motion < 0) {
                Hu3DMotionSet(model, obj->mtnId[motion]);
                Hu3DModelAttrSet(model, tbl[motion].attr);
            } else {
                Hu3DMotionShiftSet(model, obj->mtnId[motion], 0.0f, 60.0f * tbl[motion].shift, tbl[motion].attr);
            }
            w->motion = motion;
        }
    }
}

s32 fn_1_9B00(OMOBJ *obj)
{
    HU3DMODELID model = *obj->mdlId;

    if (Hu3DMotionEndCheck(model) != 0 && Hu3DMotionShiftIDGet(model) < 0) {
        return 1;
    }
    return 0;
}

s32 fn_1_9B68(OMOBJ *obj)
{
    HU3DMODELID model = *obj->mdlId;

    if (Hu3DMotionShiftIDGet(model) < 0) {
        return 1;
    }
    return 0;
}

s32 fn_1_9BC0(OMOBJ *obj, s32 motion)
{
    s32 ret = 0;

    if (fn_1_9B00(obj) != 0) {
        fn_1_9A04(obj, motion);
        ret = 1;
    }
    return ret;
}

s32 fn_1_9D08(void)
{
    return lbl_1_bss_40;
}

OMOBJ *fn_1_9D18(HuVecF *pos, f32 yaw, f32 fov, f32 maxDist)
{
    OMOBJ *best = NULL;
    f32 bestDist = 100000.0f;
    OMOBJ *cur;
    M503Saru *w;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 ang;
    f32 dist;
    s32 i;

    for (i = 0; i < 0x15; i++) {
        cur = lbl_1_bss_44[i];
        if (cur == NULL) {
            break;
        }
        w = cur->data;
        if (w->inGoal == 0 && w->held == 0 && (w->actor.flags & 1) != 0) {
            dx = w->pos.x - pos->x;
            dy = w->pos.y - pos->y;
            dz = w->pos.z - pos->z;
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
    }
    return best;
}

s32 fn_1_A080(OMOBJ *obj, HuVecF *pos, HuVecF *dest)
{
    M503Saru *w = obj->data;
    f32 best = 100000.0f;
    s32 slot = -1;
    f32 dx;
    f32 dz;
    f32 dist;
    s32 i;

    if (pos != NULL) {
        pos->x = w->pos.x;
        pos->y = w->pos.y;
        pos->z = w->pos.z;
    }
    if (dest != NULL) {
        if ((u32)w->type != 8U) {
            slot = w->owner;
        } else {
            for (i = 0; i < 4; i++) {
                dx = lbl_1_data_38C[i].x - w->pos.x;
                dz = lbl_1_data_38C[i].z - w->pos.z;
                dist = HuMagPoint2D(dx, dz);
                if (dist < best) {
                    best = dist;
                    slot = i;
                }
            }
            if (slot < 0) {
                slot = 0;
            }
        }
        *dest = lbl_1_data_38C[slot];
    }
    if (w->inGoal != 0) {
        return 0;
    }
    if (w->held != 0) {
        return 0;
    }
    if ((w->actor.flags & 1) == 0) {
        return 0;
    }
    return 1;
}

void fn_1_A338(s32 idx, HuVecF *dst)
{
    *dst = lbl_1_data_38C[idx & 3];
}

s16 fn_1_A368(OMOBJ *item, OMOBJ *holder)
{
    M503Saru *w = item->data;
    HU3DMODELID model = *item->mdlId;

    Hu3DModelPosSet(model, 0.0f, 0.0f, -1.6f);
    Hu3DModelRotSet(model, 0.0f, 0.0f, 0.0f);
    w->held = 1;
    w->goalReady = 0;
    w->airborne = 0;
    w->holder = holder;
    w->actor.flags &= ~1;
    return model;
}

void fn_1_A438(OMOBJ *item, HuVecF *pos, s32 mode)
{
    M503Saru *w = item->data;
    f32 dx;
    f32 dz;
    f32 dist;
    s32 idx;
    s32 i;

    w->pos = *pos;
    Hu3DModelPosSetV(*item->mdlId, &w->pos);
    w->vel.z = 0.0f;
    w->vel.y = 0.0f;
    w->vel.x = 0.0f;
    fn_1_5708(w->holder, NULL, &w->rotY);
    w->held = 0;
    w->actor.flags |= 1;
    if (mode != 0) {
        for (i = 0; i < 4; i++) {
            idx = w->owner;
            if (w->type == 8 || idx == i) {
                dx = lbl_1_data_38C[i].x - w->pos.x;
                dz = lbl_1_data_38C[i].z - w->pos.z;
                dist = HuMagPoint2D(dx, dz);
                if (!(dist > lbl_1_data_50C[i])) {
                    w->inGoal = 1;
                    w->goalIdx = i;
                    w->actor.flags &= ~1;
                    fn_1_D6D4(fn_1_53B8(w->holder), lbl_1_data_40C[idx]);
                    return;
                }
            }
        }
    }
}
