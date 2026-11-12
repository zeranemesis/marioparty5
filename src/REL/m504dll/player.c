#define _MATH_H

#include "types.h"
#include "string.h"

#include "REL/m504dll.h"
#include "game/charman.h"
#include "game/data.h"

#ifdef __MWERKS__
#define PL_RODATA __declspec(section ".rodata")
#else
#define PL_RODATA
#endif

#define PL_ABS(x) (((x) >= 0.0f) ? (x) : -(x))

/* Wrap an angle into (-180, 180]. */
#define PL_WRAP(dst, v)             \
    do {                            \
        f32 _w = (v);               \
        if (_w > 180.0f) {          \
            dst = _w - 360.0f;      \
        } else if (_w <= -180.0f) { \
            dst = 360.0f + _w;      \
        } else {                    \
            dst = _w;               \
        }                           \
    } while (0)

/* Inline sqrt of CodeWarrior: frsqrte seed + Newton steps; MWCC calls sqrt for the seed. */
#define PL_SQRT(out, x)                                                                \
    do {                                                                               \
        f64 _y;                                                                        \
        if ((x) > 0.0f) {                                                              \
            _y = 1.0 / sqrt((f64)(x));                                                 \
            _y = (0.5 * _y) * (3.0 - (f64)(x) * (_y * _y));                            \
            _y = (0.5 * _y) * (3.0 - (f64)(x) * (_y * _y));                            \
            tmp.a = (f32)((f64)(x) * ((0.5 * _y) * (3.0 - (f64)(x) * (_y * _y))));     \
            out = tmp.a;                                                               \
        } else {                                                                       \
            out = (x);                                                                 \
        }                                                                              \
    } while (0)

extern double atan2();
extern double sin();
extern double cos();
extern double acos();
extern double fmod();
extern double sqrt();
extern float Hu3DMotionMaxTimeGet(HU3DMODELID modelId);

extern void fn_1_5EC(s32 seId);
extern s32 fn_1_4030(M504STAGE *stage, Vec *pos, Vec *out, s32 flag, s16 *wallIdx, s16 *cornerIdx, f32 range);
extern void fn_1_B1AC(M504WANWAN *wanwan);
extern s32 fn_1_B43C(M504WANWAN *wanwan, Vec *from, f32 range, Vec *dst, s32 flag);
extern s32 fn_1_B52C(M504WANWAN *wanwan, Vec *outPos);
extern void fn_1_B828(INPUTMAIN *main, s32 padNo, PADOWNER *playerConf);
extern void fn_1_B898(INPUTMAIN *main);
extern void fn_1_B8D8(INPUTMAIN *main);

void fn_1_5F58(M504PLAYER *player);
void fn_1_613C(M504PLAYER *player);
f32 fn_1_68E8(f32 cur, f32 from, f32 mul);
void fn_1_6A04(M504PLAYER *player);
void fn_1_6AA0(M504PLAYER *player);
void fn_1_7710(M504PLAYER *player);
void fn_1_85AC(M504PLAYER *player);
void fn_1_8A40(M504PLAYER *player);
void fn_1_91B0(M504PLAYER *player);
void fn_1_9334(M504PLAYER *player);

char lbl_1_bss_7810[0x10];

/* Stage object table (0x20 per entry) copied into the stage by fn_1_255C. */
typedef struct M504StageDef_s {
    s16 mdlIdx;
    u16 type;
    f32 x0;
    f32 y0;
    f32 z0;
    f32 x1;
    f32 y1;
    f32 z1;
    s32 hidden;
} M504STAGEDEF;

typedef struct M504StageTbl_s {
    M504STAGEDEF def[10];
    s16 tail[14];
} M504STAGETBL;

M504STAGETBL lbl_1_data_A8 = {
    {
        { 17, 0, -425.0f, 0.0f, 1701.0f, -350.0f, 0.0f, 2075.0f, 0 },
        { -1, 1, 320.0f, 0.0f, 2350.0f, 60.0f, 0.0f, 0.0f, 0 },
        { 15, 0, 25.0f, 0.0f, 400.0f, 250.0f, 0.0f, 600.0f, 0 },
        { 16, 0, -300.0f, 0.0f, 1050.0f, 25.0f, 0.0f, 1450.0f, 0 },
        { -1, 0, 0.0f, 0.0f, 1750.0f, 250.0f, 0.0f, 2150.0f, 0 },
        { 18, 1, -50.0f, 0.0f, 2800.0f, 194.0f, 0.0f, 0.0f, 0 },
        { -1, 0, -400.0f, 0.0f, 1050.0f, -200.0f, 0.0f, 1450.0f, 1 },
        { -1, 0, 250.0f, 0.0f, 400.0f, 400.0f, 0.0f, 600.0f, 1 },
        { -1, 0, 250.0f, 0.0f, 1750.0f, 400.0f, 0.0f, 2150.0f, 1 },
        { -1, 0, -400.0f, 0.0f, 2719.0f, -244.0f, 0.0f, 2880.0f, 1 },
    },
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0 },
};

u32 lbl_1_data_204[13][12] = {
    { 0x8C0000, 0x8C0001, 0x8C0002, 0x8C002B, 0x8C0017, 0x8C0018, 0x8C001A, 0x87005F, 0x87005C, 0x8C002F, 0x8C0031, 0x8C002C },
    { 0x330000, 0x330001, 0x330002, 0x33002B, 0x330017, 0x330018, 0x33001A, 0x2E005F, 0x2E005C, 0x33002F, 0x330031, 0x33002C },
    { 0xB70000, 0xB70001, 0xB70002, 0xB7002B, 0xB70017, 0xB70018, 0xB7001A, 0xB2005F, 0xB2005C, 0xB7002F, 0xB70031, 0xB7002C },
    { 0xED0000, 0xED0001, 0xED0002, 0xED002B, 0xED0017, 0xED0018, 0xED001A, 0xE8005F, 0xE8005C, 0xED002F, 0xED0031, 0xED002C },
    { 0xE60000, 0xE60001, 0xE60002, 0xE6002B, 0xE60017, 0xE60018, 0xE6001A, 0xE1005F, 0xE1005C, 0xE6002F, 0xE60031, 0xE6002C },
    { 0x170000, 0x170001, 0x170002, 0x17002B, 0x170017, 0x170018, 0x17001A, 0x12005F, 0x12005C, 0x17002F, 0x170031, 0x17002C },
    { 0xE00000, 0xE00001, 0xE00002, 0xE0002B, 0xE00017, 0xE00018, 0xE0001A, 0xDB005F, 0xDB005C, 0xE0002F, 0xE00031, 0xE0002C },
    { 0x2D0000, 0x2D0001, 0x2D0002, 0x2D002B, 0x2D0017, 0x2D0018, 0x2D001A, 0x28005F, 0x28005C, 0x2D002F, 0x2D0031, 0x2D002C },
    { 0xC90000, 0xC90001, 0xC90002, 0xC9002B, 0xC90017, 0xC90018, 0xC9001A, 0xC4005F, 0xC4005C, 0xC9002F, 0xC90031, 0xC9002C },
    { 0xB00000, 0xB00001, 0xB00002, 0xB0002B, 0xB00017, 0xB00018, 0xB0001A, 0x9C005F, 0x9C005C, 0xB0002F, 0xB00031, 0xB0002C },
    { 0x8C0000, 0x8C0001, 0x8C0002, 0x8C002B, 0x8C0017, 0x8C0018, 0x8C001A, 0x87005F, 0x87005C, 0x8C002F, 0x8C0031, 0x8C002C },
    { 0x8C0000, 0x8C0001, 0x8C0002, 0x8C002B, 0x8C0017, 0x8C0018, 0x8C001A, 0x87005F, 0x87005C, 0x8C002F, 0x8C0031, 0x8C002C },
    { 0x8C0000, 0x8C0001, 0x8C0002, 0x8C002B, 0x8C0017, 0x8C0018, 0x8C001A, 0x87005F, 0x87005C, 0x8C002F, 0x8C0031, 0x8C002C },
};
Vec lbl_1_data_474 = { 800.0f, 4000.0f, 1500.0f };
Vec lbl_1_data_480 = { 0.0f, 1.0f, 0.0f };
Vec lbl_1_data_48C = { 0.0f, 0.0f, 0.0f };

PL_RODATA const Vec lbl_1_rodata_E8 = { 220.0f, 0.0f, 250.0f };
PL_RODATA const Vec lbl_1_rodata_1A0 = { 0.0f, 0.0f, 0.0f };
PL_RODATA const Vec lbl_1_rodata_1D0 = { 0.0f, 0.0f, 0.0f };
PL_RODATA const Vec lbl_1_rodata_1DC = { 0.0f, 0.0f, 0.0f };

void fn_1_5934(M504PLAYER *player, s32 slotNo, M504SLOT *slot)
{
    OMOBJMAN *proc;
    OMOBJ *obj;
    s32 i;
    s16 mdl;
    Vec start;

    proc = slot->work->proc;
    memset(player, 0, sizeof(M504PLAYER));
    player->slot = slot;
    player->playerNo = slotNo;
    player->grp = GwPlayerConf[player->playerNo].grp;
    player->charNo = GwPlayerConf[player->playerNo].charNo;
    obj = omAddObjEx(proc, 100, 1, 12, -1, NULL);
    player->obj = obj;
    player->active = 0;
    obj->mdlId[0] = CharModelCreate(player->charNo, 8);
    Hu3DModelShadowSet(obj->mdlId[0]);
    CharModelStepFxSet(player->charNo, 0);
    CharEffectLayerSet(4);
    for (i = 0; i < 12; i++) {
        obj->mtnId[i] = CharMotionCreate(player->charNo, lbl_1_data_204[player->charNo][i]);
        CharMotionSet(player->charNo, obj->mtnId[i]);
    }
    Hu3DMotionShiftSet(player->obj->mdlId[0], player->obj->mtnId[0], 0.0f, 4.0f, 0);
    player->mot = 0;
    player->motNext = 0;
    player->loopF = 1;
    Hu3DModelAttrSet(obj->mdlId[0], 1);
    Hu3DModelAttrSet(obj->mdlId[0], 0x40000001);
    Hu3DModelLayerSet(obj->mdlId[0], 3);
    Hu3DMotionSet(obj->mdlId[0], obj->mtnId[0]);
    Hu3DModelCameraSet(obj->mdlId[0], (u16)lbl_1_data_45E0[slotNo]);
    start = lbl_1_rodata_E8;
    player->pos = start;
    omSetTra(obj, player->pos.x, player->pos.y, player->pos.z);
    omSetRot(obj, 0.0f, -120.0f, 0.0f);
    player->angTarget = -120.0f;
    player->angDisp = -120.0f;
    Hu3DModelAttrReset(obj->mdlId[0], 1);
    mdl = Hu3DModelCreate(HuDataSelHeapReadNum(0x3E001E, 0x10000000, HEAP_MODEL));
    player->mdlC8 = mdl;
    Hu3DModelPosSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(mdl, 2.0f, 2.0f, 2.0f);
    Hu3DModelCameraSet(mdl, (u16)lbl_1_data_45E0[slotNo]);
    Hu3DModelLayerSet(mdl, 5);
    Hu3DModelAttrSet(mdl, 1);
    player->mtnCA = Hu3DJointMotion(mdl, HuDataSelHeapReadNum(0x3E0039, 0x10000000, HEAP_MODEL));
    Hu3DMotionSet(mdl, player->mtnCA);
    Hu3DMotionSpeedSet(mdl, 1.0f);
    Hu3DModelAttrSet(mdl, 0);
    Hu3DModelAttrReset(mdl, 0x400000);
    mdl = Hu3DModelCreate(HuDataSelHeapReadNum(0x3E001D, 0x10000000, HEAP_MODEL));
    player->mdlCC = mdl;
    Hu3DModelPosSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(mdl, (u16)lbl_1_data_45E0[slotNo]);
    Hu3DModelLayerSet(mdl, 5);
    Hu3DModelAttrSet(mdl, 1);
    Hu3DModelHookSet(obj->mdlId[0], CharModelItemHookGet(player->charNo, 8, 0), mdl);
    fn_1_B828(&player->input, slotNo, (PADOWNER *)player);
}

void fn_1_5E1C(M504PLAYER *player)
{
    OMOBJMAN *proc;

    proc = player->slot->work->proc;

    fn_1_B898(&player->input);
    Hu3DModelHookReset(player->obj->mdlId[0]);
    Hu3DModelKill(player->mdlCC);
    Hu3DMotionKill(player->mtnCA);
    Hu3DModelKill(player->mdlC8);

    if (player->obj != NULL) {
        CharModelKill(player->charNo);
        omDelObjEx(proc, player->obj);
        player->obj = NULL;
    }
    memset(player, 0, sizeof(M504PLAYER));
}

s32 fn_1_5EC8(M504PLAYER *player)
{
    M504WORK *work;

    work = player->slot->work;

    if (player->active != 0) {
        fn_1_B8D8(&player->input);
    } else {
        player->input.btn = player->input.btnDown = player->input.stkX = player->input.stkY = 0;
    }

    player->unk4 = 0;

    if (work->unk1218 == 0) {
        fn_1_5F58(player);
        fn_1_9334(player);
    }
    return 0;
}

void fn_1_5F58(M504PLAYER *player)
{
    M504SLOT *slot;
    M504WORK *work;
    OMOBJ *obj;
    HU3DMODELID mdl;
    s16 pan;

    slot = player->slot;
    work = slot->work;
    obj = player->obj;
    switch (player->mode) {
        case 0:
            fn_1_613C(player);
            break;
        case 1:
            fn_1_6AA0(player);
            break;
        case 2:
            fn_1_7710(player);
            break;
        case 3:
            fn_1_8A40(player);
            break;
        case 4:
            fn_1_85AC(player);
            break;
    }
    if (work->unk1218 == 0) {
        if (slot->unk46C != 0) {
            pan = 0x30;
        } else {
            pan = 0x50;
        }
        CharModelVoicePanSet(player->charNo, pan);
    } else {
        CharModelVoicePanSet(player->charNo, 0x40);
    }
    mdl = obj->mdlId[0];
    if (player->mot != player->motNext && Hu3DMotionShiftIDGet(mdl) < 0) {
        s32 attr;

        player->mot = player->motNext;
        Hu3DModelAttrReset(mdl, 0x40000001);
        if (player->loopF != 0) {
            attr = 0x40000001;
        } else {
            attr = 0;
        }
        Hu3DMotionShiftSet(mdl, obj->mtnId[player->mot], 0.0f, 4.0f, attr);
        if (player->mot != 7 && player->mot != 8) {
            Hu3DModelAttrSet(player->mdlCC, 1);
        }
    }
    omSetTra(player->obj, player->pos.x, player->pos.y, player->pos.z);
    omSetRot(player->obj, 0.0f, player->angDisp, 0.0f);
}

void fn_1_613C(M504PLAYER *player)
{
    M504SLOT *slot;
    M504WORK *work;
    M504WANWAN *wanwan;
    INPUTMAIN *input;
    f32 stkLen;
    f32 a;
    f32 b;
    f32 c;
    f32 mx;
    f32 mz;
    Vec hit;
    s16 wallIdx;
    s16 cornerIdx;
    struct {
        f32 a;
    } tmp;

    work = player->slot->work;
    slot = player->slot;
    wanwan = &slot->wanwan;
    input = &player->input;
    if (lbl_1_bss_0->unk122C != 0 && player->doneD0 != 0 && wanwan->state == 6 && wanwan->active == 0) {
        player->speed = 0.0f;
        a = PL_ABS(player->angDisp);
        if (a > 3.0f) {
            PL_WRAP(b, player->angDisp);
            player->angDisp = fn_1_68E8(0.0f, b, 0.9f);
            PL_WRAP(c, player->angDisp);
            player->angDisp = c;
            player->motNext = 1;
            player->loopF = 1;
        } else {
            player->angDisp = 0.0f;
            player->motNext = 0;
            player->loopF = 1;
        }
        return;
    }
    stkLen = ((f32)input->stkX * (f32)input->stkX) + ((f32)input->stkY * (f32)input->stkY);
    PL_SQRT(a, stkLen);
    player->speed = 14.0f * (a / 56.0f);
    if (player->speed >= 0.001f) {
        if (player->speed > 6.0f) {
            player->motNext = 2;
            player->loopF = 1;
        } else {
            player->motNext = 1;
            player->loopF = 1;
        }
        player->angTarget = 180.0 * (atan2((f64)input->stkX, (f64)-input->stkY) / 3.141592653589793);
    } else {
        player->speed = 0.0f;
        player->motNext = 0;
        player->loopF = 1;
    }
    if (player->speed < 4.2000003f && player->active != 0) {
        player->unk90++;
        if (player->unk90 > 15) {
            player->mode = 1;
            player->timer = 0;
            player->step = 0;
        }
    } else {
        player->timer = 0;
        player->unk90 = 0;
        player->unk94 = 0;
    }
    player->angMove = fn_1_68E8(player->angTarget, player->angMove, 0.5f);
    player->angDisp = fn_1_68E8(player->angTarget, player->angDisp, 0.2f);
    mx = player->speed * sin(3.141592653589793 * player->angMove / 180.0);
    mz = player->speed * cos(3.141592653589793 * player->angMove / 180.0);
    player->pos.x = player->pos.x + mx;
    player->pos.z = player->pos.z + mz;
    if (fn_1_B43C(wanwan, &player->pos, 50.0f, &hit, 0) != 0) {
        player->mode = 3;
        player->step = 0;
        player->pos.x = player->pos.x + hit.x;
        player->pos.z = player->pos.z + hit.z;
        omVibrate(slot->slotNo, 30, 12, 0);
        fn_1_6A04(player);
        Hu3DModelAttrSet(player->mdlCC, 1);
        if (player->active != 0) {
            if (slot->unk46C != 0) {
                fn_1_5EC(0x64B);
            } else {
                fn_1_5EC(0x64C);
            }
        }
        return;
    }
    if (fn_1_4030(&slot->stage, &player->pos, &hit, 0, &wallIdx, &cornerIdx, 50.0f) != 0) {
        player->pos.x = player->pos.x + hit.x;
        player->pos.z = player->pos.z + hit.z;
    }
    if ((input->btnDown & 0x100) != 0) {
        player->mode = 2;
        player->step = 0;
    }
    if (player->unk8C != 0) {
        player->mode = 4;
        player->timer = 0;
        player->step = 0;
    }
}

f32 fn_1_68E8(f32 cur, f32 from, f32 mul)
{
    register f32 ang;
    register f32 wrap;
    register f32 res;

    wrap = fmod(from - cur, 360);
    ang = wrap;
    if (ang < 0.0f) {
        ang += 360.0f;
    }
    if (ang > 180.0f) {
        ang -= 360.0f;
    }
    res = fmod(cur + mul * ang, 360);
    ang = res;
    if (ang < 0.0f) {
        ang += 360.0f;
    }
    return ang;
}

void fn_1_6A04(M504PLAYER *player)
{
    M504SLOT *slot;
    M504WANWAN *wanwan;

    slot = player->slot;
    wanwan = &slot->wanwan;

    Hu3DModelPosSet(wanwan->mdl30, player->pos.x, 200.0f + player->pos.y, player->pos.z);
    Hu3DMotionTimeSet(wanwan->mdl30, 0.0f);
    Hu3DModelAttrReset(wanwan->mdl30, 1);
    wanwan->on34 = 1;
}

void fn_1_6AA0(M504PLAYER *player)
{
    M504SLOT *slot;
    M504WORK *work;
    M504WANWAN *wanwan;
    INPUTMAIN *input;
    OMOBJ *obj;
    Vec facing;
    Vec toDog;
    Vec hit;
    s16 wallIdx;
    s16 cornerIdx;
    f32 stkLen;
    f32 spd;
    f32 aim;
    f32 dev;
    f32 dot;
    f32 t;
    struct {
        f32 a;
    } tmp;

    work = player->slot->work;
    slot = player->slot;
    wanwan = &slot->wanwan;
    input = &player->input;
    obj = player->obj;
    stkLen = ((f32)input->stkX * (f32)input->stkX) + ((f32)input->stkY * (f32)input->stkY);
    PL_SQRT(spd, stkLen);
    player->speed = 14.0f * (spd / 56.0f);
    if (player->speed > 4.2000003f) {
        player->mode = 0;
        player->timer = 0;
        player->step = 0;
        return;
    }
    facing.x = sin(3.141592653589793 * player->angMove / 180.0);
    facing.y = 0.0f;
    facing.z = cos(3.141592653589793 * player->angMove / 180.0);
    PSVECSubtract(&wanwan->pos, &player->pos, &toDog);
    toDog.y = 0.0f;
    PSVECNormalize(&toDog, &toDog);
    dot = PSVECDotProduct(&facing, &toDog);
    dev = 180.0 * ((f64)(f32)acos((f64)dot) / 3.141592653589793);
    t = 180.0 * (atan2((f64)toDog.x, (f64)toDog.z) / 3.141592653589793);
    PL_WRAP(aim, t);
    switch (player->step) {
        case 0:
            player->unk98 = 0;
            player->unk94 = 0;
            player->step++;
            break;
        case 1:
            if (PL_ABS(dev) > 10.0f) {
                player->motNext = 1;
                player->loopF = 1;
                player->angTarget = fn_1_68E8(aim, player->angTarget, 0.5f);
            } else {
                player->angTarget = aim;
                player->step++;
            }
            break;
        case 2:
            if (PL_ABS(dev) > 10.0f) {
                player->angTarget = fn_1_68E8(aim, player->angTarget, 0.5f);
            }
            player->motNext = 7;
            player->loopF = 0;
            player->timer = 0;
            player->step++;
            break;
        case 3:
            if (PL_ABS(dev) > 10.0f) {
                player->angTarget = fn_1_68E8(aim, player->angTarget, 0.5f);
            }
            if (player->timer == 4) {
                Hu3DModelAttrReset(player->mdlCC, 1);
            }
            if (player->timer >= 24) {
                player->unk94 = 1;
            }
            player->timer++;
            break;
    }
    player->angMove = fn_1_68E8(player->angTarget, player->angMove, 0.5f);
    player->angDisp = fn_1_68E8(player->angTarget, player->angDisp, 0.2f);
    if (fn_1_B43C(wanwan, &player->pos, 50.0f, &hit, 0) != 0) {
        player->mode = 3;
        player->step = 0;
        player->pos.x = player->pos.x + hit.x;
        player->pos.z = player->pos.z + hit.z;
        omVibrate(slot->slotNo, 30, 12, 0);
        fn_1_6A04(player);
        Hu3DModelAttrSet(player->mdlCC, 1);
        if (player->active != 0) {
            if (slot->unk46C != 0) {
                fn_1_5EC(0x64B);
            } else {
                fn_1_5EC(0x64C);
            }
        }
        return;
    }
    if (fn_1_4030(&slot->stage, &player->pos, &hit, 0, &wallIdx, &cornerIdx, 50.0f) != 0) {
        player->pos.x = player->pos.x + hit.x;
        player->pos.z = player->pos.z + hit.z;
    }
    if ((input->btnDown & 0x100) != 0) {
        player->unk98 = 1;
    }
    if (player->unk98 != 0 && player->unk94 != 0) {
        player->unk98 = 0;
        player->mode = 2;
        player->step = 0;
    }
    if (player->unk8C != 0) {
        player->mode = 4;
        player->timer = 0;
        player->step = 0;
    }
    if (player->active == 0) {
        player->mode = 0;
        player->timer = 0;
        player->step = 0;
        player->unk90 = 0;
    }
}

void fn_1_7710(M504PLAYER *player)
{
    M504SLOT *slot;
    M504WANWAN *wanwan;
    OMOBJ *obj;
    Vec facing;
    Vec toDog;
    Vec hit;
    s16 wallIdx;
    s16 cornerIdx;
    f32 aim;
    f32 dev;
    f32 dot;
    f32 t;
    f32 timeNow;
    f32 timeMax;
    u16 c;

    slot = player->slot;
    wanwan = &slot->wanwan;
    obj = player->obj;
    facing.x = sin(3.141592653589793 * player->angMove / 180.0);
    facing.y = 0.0f;
    facing.z = cos(3.141592653589793 * player->angMove / 180.0);
    PSVECSubtract(&wanwan->pos, &player->pos, &toDog);
    toDog.y = 0.0f;
    PSVECNormalize(&toDog, &toDog);
    dot = PSVECDotProduct(&facing, &toDog);
    dev = 180.0 * ((f64)(f32)acos((f64)dot) / 3.141592653589793);
    t = 180.0 * (atan2((f64)toDog.x, (f64)toDog.z) / 3.141592653589793);
    PL_WRAP(aim, t);
    switch (player->step) {
        case 0:
            if (player->unk94 != 0) {
                player->step = 3;
                player->mot = 7;
                player->motNext = 8;
                player->loopF = 0;
                player->timer = 0;
                Hu3DModelAttrReset(player->mdlCC, 1);
            } else if (PL_ABS(dev) > 10.0f) {
                player->motNext = 1;
                player->loopF = 1;
                player->angTarget = fn_1_68E8(aim, player->angTarget, 0.5f);
            } else {
                player->angTarget = aim;
                player->step++;
            }
            break;
        case 1:
            if (PL_ABS(dev) > 10.0f) {
                player->angTarget = fn_1_68E8(aim, player->angTarget, 0.5f);
            }
            player->motNext = 7;
            player->loopF = 0;
            player->timer = 0;
            player->step++;
            break;
        case 2:
            if (PL_ABS(dev) > 10.0f) {
                player->angTarget = fn_1_68E8(aim, player->angTarget, 0.5f);
            }
            c = player->timer;
            player->timer = c + 1;
            if (c >= 24) {
                player->motNext = 8;
                player->loopF = 0;
                player->timer = 0;
                player->step++;
            }
            if (player->timer == 4) {
                Hu3DModelAttrReset(player->mdlCC, 1);
            }
            break;
        case 3:
            if (PL_ABS(dev) > 10.0f) {
                player->angTarget = fn_1_68E8(aim, player->angTarget, 0.5f);
            }
            c = player->timer;
            player->timer = c + 1;
            if (c >= 30) {
                omVibrate(slot->slotNo, 15, 6, 6);
                Hu3DModelPosSet(player->mdlC8, player->pos.x, 90.0f, player->pos.z);
                Hu3DModelRotSet(player->mdlC8, 0.0f, player->angTarget - 180.0f, 0.0f);
                Hu3DMotionTimeSet(player->mdlC8, 0.0f);
                Hu3DMotionSpeedSet(player->mdlC8, 2.0f);
                Hu3DModelAttrReset(player->mdlC8, 1);
                player->step++;
                if (player->active != 0) {
                    if (slot->unk46C != 0) {
                        fn_1_5EC(0x65C);
                    } else {
                        fn_1_5EC(0x65D);
                    }
                }
            }
            break;
        case 4:
            if (PL_ABS(dev) > 10.0f) {
                player->angTarget = fn_1_68E8(aim, player->angTarget, 0.5f);
                Hu3DModelRotSet(player->mdlC8, 0.0f, player->angTarget - 180.0f, 0.0f);
            }
            timeNow = Hu3DMotionTimeGet(player->mdlC8);
            timeMax = Hu3DMotionMaxTimeGet(player->mdlC8);
            if (timeNow >= 0.5f * timeMax && (wanwan->state == 5 || wanwan->state == 6)) {
                fn_1_B1AC(wanwan);
            }
            c = player->timer;
            player->timer = c + 1;
            if (c >= 60) {
                Hu3DModelAttrSet(player->mdlC8, 1);
                player->timer = 30;
                player->mode = 1;
                player->step = 3;
            }
            break;
    }
    player->angMove = fn_1_68E8(player->angTarget, player->angMove, 0.5f);
    player->angDisp = fn_1_68E8(player->angTarget, player->angDisp, 0.2f);
    if (fn_1_B43C(wanwan, &player->pos, 50.0f, &hit, 0) != 0) {
        Hu3DModelAttrSet(player->mdlCC, 1);
        player->mode = 3;
        player->step = 0;
        player->pos.x = player->pos.x + hit.x;
        player->pos.z = player->pos.z + hit.z;
        omVibrate(slot->slotNo, 40, 12, 0);
        fn_1_6A04(player);
        if (player->active != 0) {
            if (slot->unk46C != 0) {
                fn_1_5EC(0x64B);
            } else {
                fn_1_5EC(0x64C);
            }
        }
        return;
    }
    if (fn_1_4030(&slot->stage, &player->pos, &hit, 0, &wallIdx, &cornerIdx, 50.0f) != 0) {
        player->pos.x = player->pos.x + hit.x;
        player->pos.z = player->pos.z + hit.z;
    }
    if (player->active == 0) {
        player->mode = 0;
        player->timer = 0;
        player->step = 0;
        player->unk90 = 0;
    }
}

void fn_1_85AC(M504PLAYER *player)
{
    M504SLOT *slot;
    M504WANWAN *wanwan;
    Vec hit;
    s16 wallIdx;
    s16 cornerIdx;

    slot = player->slot;
    wanwan = &slot->wanwan;
    switch (player->step) {
        case 1:
            break;
        case 0:
            Hu3DModelAttrSet(player->mdlCC, 1);
            player->motNext = 10;
            player->loopF = 0;
            player->timer = 0;
            player->step++;
            break;
    }
    player->angMove = fn_1_68E8(player->angTarget, player->angMove, 0.5f);
    player->angDisp = fn_1_68E8(player->angTarget, player->angDisp, 0.2f);
    if (fn_1_B43C(wanwan, &player->pos, 50.0f, &hit, 0) != 0) {
        Hu3DModelAttrSet(player->mdlCC, 1);
        player->mode = 3;
        player->step = 0;
        player->pos.x = player->pos.x + hit.x;
        player->pos.z = player->pos.z + hit.z;
        omVibrate(slot->slotNo, 40, 12, 0);
        fn_1_6A04(player);
        if (player->active != 0) {
            if (slot->unk46C != 0) {
                fn_1_5EC(0x64B);
            } else {
                fn_1_5EC(0x64C);
            }
        }
        return;
    }
    if (fn_1_4030(&slot->stage, &player->pos, &hit, 0, &wallIdx, &cornerIdx, 50.0f) != 0) {
        player->pos.x = player->pos.x + hit.x;
        player->pos.z = player->pos.z + hit.z;
    }
}

void fn_1_8A40(M504PLAYER *player)
{
    M504SLOT *slot;
    M504WANWAN *wanwan;
    Vec hit;
    Vec p;
    s16 wallIdx;
    s16 cornerIdx;
    f32 t;
    f32 y;
    u16 c;

    slot = player->slot;
    wanwan = &slot->wanwan;
    switch (player->step) {
        case 0:
            Hu3DModelAttrSet(player->mdlCC, 1);
            player->motNext = 3;
            player->loopF = 0;
            player->unk9E = 0;
            if (player->active != 0) {
                if (slot->unk46C != 0) {
                    fn_1_5EC(0x64D);
                } else {
                    fn_1_5EC(0x64E);
                }
            }
            player->step++;
            /* fallthrough */
        case 1:
            player->pos.y = 1000.0f * (f32)sin(3.141592653589793 * (180.0f * ((f32)player->unk9E / 60.0f)) / 180.0);
            player->unk9E++;
            if (player->pos.y <= 0.0f && player->unk9E >= 30) {
                player->pos.y = 0.0f;
                player->motNext = 4;
                player->loopF = 1;
                player->timer = 60;
                player->step++;
                return;
            }
            if (player->unk9E >= 30 && fn_1_B43C(wanwan, &player->pos, 60.000004f, &hit, 1) != 0) {
                fn_1_6A04(player);
                fn_1_91B0(player);
                omVibrate(slot->slotNo, 20, 12, 0);
                if (player->active != 0) {
                    if (slot->unk46C != 0) {
                        fn_1_5EC(0x64F);
                        return;
                    }
                    fn_1_5EC(0x650);
                    return;
                }
            }
            return;
        case 2:
            if (fn_1_B43C(wanwan, &player->pos, 75.0f, &hit, 1) != 0) {
                player->pos.x = player->pos.x + hit.x;
                player->pos.z = player->pos.z + hit.z;
            }
            if (fn_1_4030(&slot->stage, &player->pos, &hit, 0, &wallIdx, &cornerIdx, 50.0f) != 0) {
                player->pos.x = player->pos.x + hit.x;
                player->pos.z = player->pos.z + hit.z;
            }
            player->timer--;
            if (player->timer == 0) {
                player->motNext = 5;
                player->loopF = 0;
                player->timer = 0;
                player->step++;
                return;
            }
            break;
        case 3:
            c = player->timer;
            player->timer = c + 1;
            if (c >= 30) {
                player->motNext = 0;
                player->loopF = 1;
                player->mode = 0;
                player->timer = 0;
                player->unk90 = 0;
                player->step = 0;
                return;
            }
            break;
        case 4:
            player->vel.y = player->vel.y + -2.4333334f;
            switch (player->unk9C) {
                case 0:
                    t = (f32)player->unk9E / 15.0f;
                    y = player->posA0.y + ((f32)sin(3.141592653589793 * (90.0f * t) / 180.0) * (player->posAC.y - player->posA0.y));
                    PSVECSubtract(&player->posAC, &player->posA0, &p);
                    p.x = p.x * t;
                    p.y = y;
                    p.z *= t;
                    p.x = p.x + player->posA0.x;
                    p.z += player->posA0.z;
                    player->pos.x = p.x;
                    player->pos.y = p.y;
                    player->pos.z = p.z;
                    if (player->unk9E >= 15) {
                        player->unk9C++;
                    }
                    break;
                case 1:
                    t = (f32)(player->unk9E - 15) / 15.0f;
                    y = player->posB8.y + ((f32)sin(3.141592653589793 * (90.0f + (90.0f * t)) / 180.0) * (player->posAC.y - player->posB8.y));
                    PSVECSubtract(&player->posB8, &player->posAC, &p);
                    p.x *= t;
                    p.y = y;
                    p.z *= t;
                    p.x += player->posAC.x;
                    p.z += player->posAC.z;
                    player->pos.x = p.x;
                    player->pos.y = p.y;
                    player->pos.z = p.z;
                    if (player->unk9E >= 30) {
                        player->unk9C++;
                    }
                    break;
                case 2:
                    player->pos.y = player->pos.y + player->vel.y;
                    break;
            }
            player->unk9E++;
            if (player->pos.y <= 0.0f) {
                player->pos.y = 0.0f;
                player->motNext = 4;
                player->loopF = 1;
                player->timer = 60;
                player->step = 2;
                return;
            }
            if (player->unk9C != 0 && fn_1_B43C(wanwan, &player->pos, 60.000004f, &hit, 1) != 0) {
                player->pos.x = player->pos.x + hit.x;
                player->pos.z = player->pos.z + hit.z;
            }
            if (fn_1_4030(&slot->stage, &player->pos, &hit, 0, &wallIdx, &cornerIdx, 50.0f) != 0) {
                player->pos.x = player->pos.x + hit.x;
                player->pos.z = player->pos.z + hit.z;
            }
            break;
    }
}

void fn_1_91B0(M504PLAYER *player)
{
    M504SLOT *slot;
    M504WANWAN *wanwan;
    Vec mid;
    Vec modelPos;
    Vec zero;

    slot = player->slot;
    wanwan = &slot->wanwan;

    if (fn_1_B52C(wanwan, &modelPos) == 0) {
        player->vel.x = 0.0f;
        player->vel.y = 40.0f;
        player->vel.z = 0.0f;
        player->step = 1;
    } else {
        player->unk9C = 0;
        player->unk9E = 0;

        player->posA0 = player->pos;
        player->posB8 = modelPos;

        PSVECSubtract(&player->posB8, &player->posA0, &mid);
        PSVECScale(&mid, &mid, 0.5f);
        PSVECAdd(&mid, &player->posA0, &mid);

        mid.y += 600.0f;
        player->posAC = mid;

        zero = lbl_1_rodata_1A0;
        player->vel = zero;
        player->step = 4;
    }
}

void fn_1_9334(M504PLAYER *player)
{
    M504SLOT *slot;
    M504STAGE *stage;
    M504WANWAN *wanwan;
    M504CAMERA *cam;
    f32 z;
    f32 dz;
    f32 base;
    f32 q;
    f32 t;
    f32 camOff;
    f32 camBack;
    f32 lift;

    slot = player->slot;
    cam = &slot->cam;
    stage = &slot->stage;
    wanwan = &slot->wanwan;
    z = player->pos.z;
    dz = PL_ABS(player->pos.z - wanwan->pos.z);
    base = (dz < 300.0f) ? 0.0f : dz - 300.0f;
    q = base / 1200.0f;
    t = (q > 1.0f) ? 1.0f : ((q < 0.0f) ? 0.0f : q);
    if (player->pos.z > wanwan->pos.z) {
        camBack = 1000.0f + (1000.0f * t);
        camOff = -200.0f * t;
    } else {
        camBack = 1000.0f;
        camOff = 200.0f * t;
    }
    if (player->mode == 3) {
        lift = 0.5f * player->pos.y;
    } else {
        lift = 0.0f;
    }
    cam->lookTgt.z = z + camOff;
    cam->lookTgt.y = 300.0f + lift;
    cam->posTgt.z = z + camBack;
    cam->posTgt.y = 1800.0f + lift;
    cam->smooth = 0.1f;
    lbl_1_data_474.z = z + (500.0f + camBack);
    lbl_1_data_48C.z = z;
    Hu3DShadowMultiPosSet(&lbl_1_data_474, &lbl_1_data_480, &lbl_1_data_48C, (s16)lbl_1_data_45E0[slot->slotNo]);
}
