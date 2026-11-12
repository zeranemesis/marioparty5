#include "game/audio.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/frand.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/wipe.h"
#include "game/mg/seqman.h"

#include "humath.h"
#include "math.h"
#include <string.h>

#include "REL/m523Dll.h"

#ifdef __MWERKS__
#define M523_RODATA __declspec(section ".rodata")
#else
#define M523_RODATA
#endif

typedef struct M523MotInfo_s {
    s16 motA; // motion index when teamNo == 0
    s16 motB; // motion index otherwise
    f32 shiftTime;
    u32 attr;
} M523MotInfo;

extern OMOBJMAN *lbl_1_bss_E4;
extern s32 lbl_1_bss_14;
extern s32 lbl_1_bss_18;
extern s32 lbl_1_bss_1C;
extern Point3d lbl_1_data_1E4[3];

extern void fn_1_3178(M523Player *player, s16 index);
extern s32 fn_1_3244(Point3d *pos, Point3d *target, Point3d *rot, f32 speed);
extern f32 fn_1_3368(f32 ang0, f32 ang1, f32 max);
extern s32 fn_1_34D8(Point3d *pos, Point3d *target, Point3d *center, f32 dist);
extern f32 fn_1_C88(Point3d *from, Point3d *to);
extern f32 fn_1_CEC(f32 ang0, f32 ang1);
extern f32 fn_1_D58(Point3d *pt, Point3d *a, Point3d *b, Point3d *out);
extern void fn_1_E58(Point3d *pos, s16 *outY, s16 *outX);
extern s32 fn_1_F34(Point3d *pos, s16 seId);

void fn_1_3F68(OMOBJ *obj, s16 motNo);
static void fn_1_4900(OMOBJ *obj);
void fn_1_4CCC(OMOBJ *obj);
static void fn_1_4ED4(OMOBJ *obj);
static void fn_1_54A8(OMOBJ *obj);
static void fn_1_58A8(OMOBJ *obj);
static void fn_1_595C(M523Player *player);
static void fn_1_5AFC(M523Player *player);
static void fn_1_5CB8(OMOBJ *obj);
void fn_1_6294(OMOBJ *obj);
static void fn_1_6800(OMOBJ *obj);
static void fn_1_6DA4(OMOBJ *obj);
s32 fn_1_90EC(M523Player *player);
static s32 fn_1_73D4(M523Player *player);
static void fn_1_7488(void);
static void fn_1_76E4(OMOBJ *obj);
static void fn_1_7898(OMOBJ *obj);
static f32 fn_1_85A8(M523Player *player);
static void fn_1_8874(M523Player *player);
static void fn_1_8A80(M523Player *player, s16 hitNo);
static void fn_1_8B60(M523Player *player, s32 mode);
static s16 fn_1_8D14(M523Player *player);
static f32 fn_1_8DF0(M523Player *player);
static s16 fn_1_9218(void);
static void fn_1_92F4(M523Player *player);
static void fn_1_9440(M523Player *player);
static void fn_1_9574(void);
static M523Cyl *fn_1_9798(Point3d *pos);
void fn_1_992C(void);
static void fn_1_9978(HU3DMODEL *modelP, Mtx *mtx);

unsigned int lbl_1_data_208[9] = { 0x008C0000, 0x008C0001, 0x008C0002, 0x008C001B, 0x008C0019, 0x008C002B, 0x008C0051, 0x008C0006, 0 };

unsigned int lbl_1_data_22C[7] = { 0x008C004D, 0x008C0017, 0x008C002F, 0x008C0000, 0x008C003F, 0, 0x00080005 };

Point3d lbl_1_data_248[4] = {
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, -300.0f },
    { -350.0f, 0.0f, 200.0f },
    { 350.0f, 0.0f, 200.0f },
};

Point3d lbl_1_data_278[4] = {
    { 0.0f, 0.0f, 150.0f },
    { -350.0f, 0.0f, -300.0f },
    { 0.0f, 0.0f, -300.0f },
    { 350.0f, 0.0f, -300.0f },
};

Point3d lbl_1_data_2A8[3] = {
    { 0.0f, 0.0f, 200.0f },
    { -350.0f, 0.0f, -250.0f },
    { 350.0f, 0.0f, -250.0f },
};

M523MotInfo lbl_1_data_2CC[11] = {
    { 0, 0, 10.0f, 0x40000001 },
    { 1, -1, 10.0f, 0x40000001 },
    { 2, -1, 10.0f, 0x40000001 },
    { 3, -1, 10.0f, 0 },
    { 4, -1, 10.0f, 0x40000001 },
    { 5, -1, 10.0f, 0x40000001 },
    { 6, -1, 10.0f, 0x40000001 },
    { -1, 1, 10.0f, 0x40000001 },
    { 7, 2, 10.0f, 0 },
    { -1, 3, 10.0f, 0x40000001 },
    { -1, 4, 10.0f, 0 },
};

char lbl_1_data_350[16] = "itemhook_ufo";

char lbl_1_data_394[6] = "cyl10";

M523Cyl lbl_1_bss_E8;
M523Cyl lbl_1_bss_10C;
s32 lbl_1_bss_130[3];
M523Player *lbl_1_bss_13C;
M523Player *lbl_1_bss_140[4];
Point3d lbl_1_bss_150[10];

M523_RODATA const s16 lbl_1_rodata_F0[14] = { 1, 1, 0, 1, 1, 0, 0, 2, 2, 2, 2, 2, 2, 0 };
M523_RODATA const u32 lbl_1_rodata_10C[3] = { 0x00510006, 0x00510007, 0x00510008 };
M523_RODATA const u32 lbl_1_rodata_118[3] = { 0x00510009, 0x0051000A, 0x0051000B };
M523_RODATA const u32 lbl_1_rodata_124[3] = { 0x0051000C, 0x0051000D, 0x0051000E };
M523_RODATA const u32 lbl_1_rodata_130[3] = { 0x0051000F, 0x00510010, 0x00510011 };
M523_RODATA const u32 lbl_1_rodata_13C[3] = { 0x00510015, 0x00510016, 0x00510017 };

OMOBJ *fn_1_3914(s16 grp)
{
    OMOBJ *obj;

    if (grp == 0) {
        obj = omAddObjEx(lbl_1_bss_E4, grp + 0x1000, 3, 5, -1, fn_1_4900);
    } else {
        obj = omAddObjEx(lbl_1_bss_E4, grp + 0x1000, 3, 8, -1, fn_1_5CB8);
    }
    obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M523Player), HU_MEMNUM_OVL);
    memset(obj->data, 0, sizeof(M523Player));
    return obj;
}

void fn_1_39DC(void)
{
    s16 cpuNo;
    s16 i;
    OMOBJ *obj;
    M523Player *player;
    HU3DMODELID model;
    s32 j;

    cpuNo = 1;
    for (i = 0; i < 4; i++) {
        obj = fn_1_3914(GwPlayerConf[i].grp);
        player = (M523Player *)obj->data;
        lbl_1_bss_140[i] = player;
        fn_1_3178(player, i);
        player->obj = obj;
        player->hitNo = -1;
        player->cyl = NULL;
        player->hitTimer = 0;
        player->shockTimer = 0;
        player->shockF = 0;
        player->shockSe = -1;
        player->doneF = 0;
        player->aiTimer = 0;
        player->velSave.z = 0.0f;
        player->velSave.y = 0.0f;
        player->velSave.x = 0.0f;
        player->vel.z = 0.0f;
        player->vel.y = 0.0f;
        player->vel.x = 0.0f;
        player->aiTarget.z = 0.0f;
        player->aiTarget.y = 0.0f;
        player->aiTarget.x = 0.0f;
        if (player->grp == 0) {
            lbl_1_bss_13C = player;
            player->teamNo = 0;
            player->radius = 50.0f;
            player->pos.x = lbl_1_data_248[1].x;
            player->pos.y = lbl_1_data_248[1].y;
            player->pos.z = lbl_1_data_248[1].z;
            player->state = 14;
            player->aiState = 0;
            player->aiTimer = 0;
            model = CharModelMotListCreate(player->charNo, 2, lbl_1_data_208, obj->mtnId);
            obj->mdlId[0] = model;
            CharMotionSet(player->charNo, obj->mtnId[0]);
            Hu3DModelLayerSet(model, 4);
            Hu3DModelShadowSet(model);
        } else {
            omSetStatBit(obj, 0x100);
            player->teamNo = cpuNo;
            cpuNo++;
            player->radius = 150.0f;
            fn_1_5AFC(player);
            player->spawnPos.x = player->pos.x;
            player->spawnPos.y = player->pos.y;
            player->spawnPos.z = player->pos.z;
            player->state = 19;
            player->aiState = 0;
            player->aiTimer = 30;
            model = CharModelMotListCreate(player->charNo, 2, lbl_1_data_22C, obj->mtnId);
            obj->mdlId[0] = model;
            CharMotionSet(player->charNo, obj->mtnId[0]);
            Hu3DModelLayerSet(model, 4);
            Hu3DModelShadowSet(model);
            CharMotionVoiceOnSet(player->charNo, lbl_1_data_22C[4], 0);
            j = lbl_1_rodata_F0[player->charNo];
            model = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_rodata_10C[j], HU_MEMNUM_OVL, HEAP_MODEL));
            Hu3DModelAttrSet(model, 0x40000001);
            obj->mdlId[1] = model;
            Hu3DModelLayerSet(model, 4);
            Hu3DModelShadowSet(model);
            Hu3DModelHookSet(model, lbl_1_data_350, obj->mdlId[0]);
            player->fuseModel = model;
            player->fuseMot = Hu3DMotionIDGet(model);
            player->fuseMotWin = Hu3DJointMotion(model, HuDataSelHeapReadNum(lbl_1_rodata_118[j], HU_MEMNUM_OVL, HEAP_MODEL));
            player->fuseMotLose = Hu3DJointMotion(model, HuDataSelHeapReadNum(lbl_1_rodata_124[j], HU_MEMNUM_OVL, HEAP_MODEL));
            player->fuseMotHit = Hu3DJointMotion(model, HuDataSelHeapReadNum(lbl_1_rodata_130[j], HU_MEMNUM_OVL, HEAP_MODEL));
            Hu3DModelPosSetV(obj->mdlId[1], &player->pos);
            Hu3DModelRotSetV(obj->mdlId[1], &player->rot);
            model = Hu3DModelCreate(HuDataSelHeapReadNum(0x510012, HU_MEMNUM_OVL, HEAP_MODEL));
            Hu3DModelAttrSet(model, 0x40000002);
            obj->mdlId[2] = model;
            Hu3DModelLayerSet(model, 4);
            Hu3DModelPosSetV(obj->mdlId[2], &player->pos);
            Hu3DModelRotSetV(obj->mdlId[2], &player->rot);
        }
        player->motNo = -1;
        fn_1_3F68(obj, 0);
    }
    CharEffectLayerSet(5);
    fn_1_9574();
    for (j = 0; j < 3; j++) {
        lbl_1_bss_130[j] = 0;
    }
    for (j = 0; j < 10; j++) {
        lbl_1_bss_150[j].x = 0.0f;
        lbl_1_bss_150[j].y = 0.0f;
        lbl_1_bss_150[j].z = 0.0f;
    }
}

void fn_1_3F68(OMOBJ *obj, s16 motNo)
{
    M523Player *player;
    s16 mot;

    player = (M523Player *)obj->data;
    if (player->motNo != motNo) {
        if (player->teamNo == 0) {
            mot = lbl_1_data_2CC[motNo].motA;
        } else {
            mot = lbl_1_data_2CC[motNo].motB;
        }
#ifdef TARGET_PC
        if (mot < 0) {
            return;
        }
#endif
        CharMotionShiftSet(player->charNo, obj->mtnId[mot], 0.0f, lbl_1_data_2CC[motNo].shiftTime, lbl_1_data_2CC[motNo].attr);
        player->motNo = motNo;
    }
}

void fn_1_405C(void)
{
    s32 i;

    for (i = 0; i < 4; i++) {
        lbl_1_bss_140[i]->state = 1;
        lbl_1_bss_140[i]->doneF = 0;
    }
}

void fn_1_40BC(OMOBJ *obj)
{
    M523Player *player = (M523Player *)obj->data;

    if (player->teamNo == 0) {
        fn_1_4CCC(obj);
    } else {
        fn_1_6294(obj);
    }
}

void fn_1_4110(M523Player *player)
{
    Point3d tmp;
    f32 radius;

    if (player->teamNo == 0) {
        radius = player->radius;
    } else {
        radius = player->radius - 50.0f;
    }
    PSVECAdd(&player->pos, &player->vel, &tmp);
    tmp.y = 0.0f;
    if (PSVECMag(&tmp) > (800.0f - radius)) {
        PSVECNormalize(&tmp, &tmp);
        PSVECScale(&tmp, &tmp, 800.0f - radius);
        PSVECSubtract(&tmp, &player->pos, &player->vel);
        player->vel.y = 0.0f;
    }
}

void fn_1_41F8(Point3d *pos, f32 radius)
{
    if (PSVECMag(pos) > (800.0f - radius)) {
        PSVECNormalize(pos, pos);
        PSVECScale(pos, pos, 800.0f - radius);
    }
}

void fn_1_427C(M523Player *player)
{
    Point3d diff;
    M523Player *other;
    s32 i;

    PSVECSubtract(&player->prevPos, &player->pos, &diff);
    if (!(PSVECMag(&diff) < 0.01f)) {
        for (i = 0; i < 4; i++) {
            other = lbl_1_bss_140[i];
            if (player != other && other != lbl_1_bss_13C && other->state != 6) {
                PSVECSubtract(&player->pos, &other->pos, &diff);
                if (!(PSVECMag(&diff) >= (player->radius + other->radius))) {
                    PSVECNormalize(&diff, &diff);
                    PSVECScale(&diff, &diff, player->radius + other->radius);
                    PSVECAdd(&other->pos, &diff, &player->pos);
                    player->pos.y = 0.0f;
                }
            }
        }
    }
}

f32 fn_1_43A4(M523Player *player)
{
    f32 mag;

    player->stkX = HuPadStkX[player->padNo];
    player->stkY = -HuPadStkY[player->padNo];
    mag = (f32)((player->stkX * player->stkX) + (player->stkY * player->stkY));
    if (mag > 0.0f) {
        mag = sqrtf(mag);
    }
    return mag;
}

void fn_1_4534(OMOBJ *obj)
{
    if (Hu3DMotionEndCheck(obj->mdlId[0])) {
        Hu3DModelKill(obj->mdlId[0]);
        omDelObjEx(lbl_1_bss_E4, obj);
    }
}

void fn_1_4590(Point3d *pos)
{
    HU3DMODELID model;
    OMOBJ *obj;

    model = Hu3DModelCreate(HuDataSelHeapReadNum(0x510014, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelLayerSet(model, 5);
    obj = omAddObjEx(lbl_1_bss_E4, 0x1400, 1, 0, -1, fn_1_4534);
    obj->mdlId[0] = model;
    pos->y = 530.0f;
    omSetTra(obj, pos->x, pos->y, pos->z);
    fn_1_F34(pos, 0x74D);
}

void fn_1_4654(OMOBJ *obj)
{
    M523Player *player;

    if (Hu3DMotionEndCheck(obj->mdlId[0])) {
        lbl_1_bss_130[obj->work[1]] = 0;
        Hu3DModelKill(obj->mdlId[0]);
        omDelObjEx(lbl_1_bss_E4, obj);
    }
    player = lbl_1_bss_140[obj->work[0]];
    omSetTra(obj, player->pos.x, player->pos.y, player->pos.z);
}

void fn_1_4700(M523Player *player)
{
    OMOBJ *obj;
    HU3DMODELID model;
    s16 freeIdx[3];
    s16 num;
    s16 slot;
    s16 i;

    num = 0;
    for (i = 0; i < 3; i++) {
        if (lbl_1_bss_130[i] == 0) {
            freeIdx[num++] = i;
        }
    }
    if (num == 0) {
        slot = 0;
    } else {
        slot = freeIdx[frandmod(num)];
    }
    lbl_1_bss_130[slot] = 1;
    model = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_rodata_13C[slot], HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelLayerSet(model, 5);
    obj = omAddObjEx(lbl_1_bss_E4, 0x1400, 1, 0, -1, fn_1_4654);
    obj->mdlId[0] = model;
    obj->work[0] = player->index;
    obj->work[1] = slot;
    omSetTra(obj, player->pos.x, player->pos.y, player->pos.z);
}

void fn_1_4858(void)
{
    s32 i;

    if (lbl_1_bss_1C == 0) {
        for (i = 0; i < 4; i++) {
            lbl_1_bss_140[i]->dest.x = lbl_1_data_278[lbl_1_bss_140[i]->teamNo].x;
            lbl_1_bss_140[i]->dest.y = lbl_1_data_278[lbl_1_bss_140[i]->teamNo].y;
            lbl_1_bss_140[i]->dest.z = lbl_1_data_278[lbl_1_bss_140[i]->teamNo].z;
        }
    } else {
        fn_1_7488();
    }
}

static void fn_1_4900(OMOBJ *obj)
{
    Point3d diff;
    M523Player *player;

    player = (M523Player *)obj->data;
    switch (player->state) {
        case 14:
            obj->work[0]++;
            if (lbl_1_bss_14 == 1) {
                fn_1_3F68(obj, 4);
                player->state = 15;
            }
            break;
        case 15:
            CharMotionSpeedSet(player->charNo, 3.0f);
            if (lbl_1_bss_14 == 2) {
                fn_1_3F68(obj, 3);
                player->state = 16;
                obj->work[0] = 0;
            }
            break;
        case 16:
            if (obj->work[0]++ >= 25) {
                fn_1_3F68(obj, 2);
                player->dest.x = lbl_1_data_248[player->teamNo].x;
                player->dest.y = lbl_1_data_248[player->teamNo].y;
                player->dest.z = lbl_1_data_248[player->teamNo].z;
                player->state = 17;
                lbl_1_bss_14 = 3;
            }
            break;
        case 17:
            PSVECSubtract(&player->dest, &player->pos, &diff);
            if (PSVECMag(&diff) < 15.0f) {
                player->pos.x = player->dest.x;
                player->pos.y = player->dest.y;
                player->pos.z = player->dest.z;
                player->state = 18;
                fn_1_3F68(obj, 6);
                obj->objFunc = fn_1_4CCC;
            } else {
                PSVECNormalize(&diff, &diff);
                PSVECScale(&diff, &diff, 15.0f);
                PSVECAdd(&player->pos, &diff, &player->pos);
            }
            break;
    }
    omSetTra(obj, player->pos.x, player->pos.y, player->pos.z);
}

void fn_1_4CCC(OMOBJ *obj)
{
    s16 pan;
    s16 vol;
    Point3d rot;
    Point3d target;
    M523Player *player;
    M523Player *hit;

    player = (M523Player *)obj->data;
    fn_1_E58(&player->pos, &vol, &pan);
    CharModelVoicePanSet(player->charNo, pan);
    CharModelVoiceVolSet(player->charNo, vol);
    player->vel.z = 0.0f;
    player->vel.x = 0.0f;
    switch (player->state) {
        case 1:
            if (lbl_1_bss_18 == 2) {
                fn_1_3F68(obj, 2);
                obj->objFunc = fn_1_54A8;
                return;
            }
            fn_1_4ED4(obj);
        default:
            omSetRot(obj, player->rot.x, player->rot.y, player->rot.z);
            omSetTra(obj, player->pos.x, player->pos.y, player->pos.z);
            return;
        case 2:
            hit = lbl_1_bss_140[player->hitNo];
            target.x = hit->pos.x;
            target.y = hit->pos.y;
            target.z = hit->pos.z;
            target.y = 200.0f;
            player->rot.y += 4.0f;
            if (fn_1_3244(&player->pos, &target, &rot, 3.0f) != 0) {
                player->doneF = 1;
                player->state = 7;
                obj->objFunc = fn_1_58A8;
            }
            omSetRot(obj, player->rot.x, player->rot.y, player->rot.z);
            omSetTra(obj, player->pos.x, player->pos.y, player->pos.z);
            return;
    }
}

static void fn_1_4ED4(OMOBJ *obj)
{
    Point3d tmp;
    f32 mag;
    f32 velMag;
    M523Player *player;

    player = (M523Player *)obj->data;
    if (player->type != 0) {
        mag = fn_1_85A8(player);
    } else {
        mag = fn_1_43A4(player);
    }
    if (mag >= 10.0f) {
        player->vel.x = 0.26785713f * player->stkX;
        player->vel.z = 0.26785713f * player->stkY;
    }
    velMag = PSVECMag(&player->vel);
    if (velMag < 0.1f) {
        fn_1_3F68(obj, 4);
    } else if (velMag < 7.5f) {
        fn_1_3F68(obj, 1);
    } else {
        fn_1_3F68(obj, 2);
    }
    if (velMag >= 0.1f) {
        player->rot.y = 180.0 * (atan2(player->vel.x, player->vel.z) / 3.141592653589793);
        fn_1_595C(player);
        fn_1_4110(player);
        PSVECAdd(&player->pos, &player->vel, &player->pos);
    }
    {
        s32 i;

        for (i = 1; i < 10; i++) {
            lbl_1_bss_150[i - 1] = lbl_1_bss_150[i];
        }
    }
    lbl_1_bss_150[9].x = player->vel.x;
    lbl_1_bss_150[9].y = player->vel.y;
    lbl_1_bss_150[9].z = player->vel.z;
}

static void fn_1_54A8(OMOBJ *obj)
{
    Point3d tmp;
    Point3d diff;
    M523Player *player;

    player = (M523Player *)obj->data;
    player->vel.z = 0.0f;
    player->vel.y = 0.0f;
    player->vel.x = 0.0f;
    switch (player->state) {
        case 1:
            PSVECSubtract(&lbl_1_data_278[0], &player->pos, &diff);
            diff.y = 0.0f;
            if (PSVECMag(&diff) < 15.0f) {
                fn_1_3F68(obj, 0);
                player->state = 4;
                player->vel.x = diff.x;
                player->vel.y = diff.y;
                player->vel.z = diff.z;
            } else {
                PSVECNormalize(&diff, &diff);
                player->vel.x = 15.0f * diff.x;
                player->vel.z = 15.0f * diff.z;
            }
            player->rot.y = 180.0 * (atan2(player->vel.x, player->vel.z) / 3.141592653589793);
            fn_1_595C(player);
            fn_1_4110(player);
            PSVECAdd(&player->pos, &player->vel, &player->pos);
            break;
        case 4:
            player->rot.y = fn_1_3368(player->rot.y, 0.0f, 15.0f);
            if (fabs(player->rot.y) <= 0.0) {
                player->doneF = 1;
                player->state = 5;
            }
            break;
        case 5:
            if (lbl_1_bss_18 == 4) {
                player->state = 6;
                fn_1_3F68(obj, 8);
            }
            break;
    }
    omSetRot(obj, player->rot.x, player->rot.y, player->rot.z);
    omSetTra(obj, player->pos.x, player->pos.y, player->pos.z);
}

static void fn_1_58A8(OMOBJ *obj)
{
    M523Player *player;

    player = (M523Player *)obj->data;
    player->pos = lbl_1_bss_140[player->hitNo]->pos;
    player->pos.y = 200.0f;
    player->rot.y += 4.0f;
    omSetRot(obj, player->rot.x, player->rot.y, player->rot.z);
    omSetTra(obj, player->pos.x, player->pos.y, player->pos.z);
}

static void fn_1_595C(M523Player *player)
{
    Point3d tmp;
    s32 i;

    for (i = 0; i < 3; i++) {
        PSVECAdd(&player->pos, &player->vel, &tmp);
        fn_1_34D8(&player->pos, &tmp, &lbl_1_data_1E4[i], 50.0f + player->radius);
        PSVECSubtract(&tmp, &player->pos, &player->vel);
    }
}

void fn_1_59F0(void)
{
    omVibrate(lbl_1_bss_13C->index, 0x3C, 0xC, 0);
    fn_1_3F68(lbl_1_bss_13C->obj, 5);
    lbl_1_bss_13C->state = 2;
    fn_1_F34(&lbl_1_bss_13C->pos, 0x74E);
}

static void fn_1_5AFC(M523Player *player)
{
    f32 x;
    f32 z;

    x = lbl_1_data_248[1].x + (300.0 * sin(2.9670597283903604));
    z = lbl_1_data_248[1].z + (300.0 * cos(2.9670597283903604));
    player->dest.x = x;
    player->dest.z = z;
    player->dest.y = 0.0f;
    x = x + ((1500.0f + (628.31854f * (f32)(player->teamNo - 1))) * sin(4.537856055185257));
    z = z + ((1500.0f + (628.31854f * (f32)(player->teamNo - 1))) * cos(4.537856055185257));
    player->pos.x = x;
    player->pos.z = z;
    player->pos.y = 0.0f;
}

static void fn_1_5CB8(OMOBJ *obj)
{
    Point3d diff;
    f32 frame;
    M523Player *player;

    player = (M523Player *)obj->data;
    switch (player->state) {
        case 19:
            PSVECSubtract(&player->dest, &player->pos, &diff);
            if (PSVECMag(&diff) < 30.0f) {
                obj->work[0] = 300;
                obj->work[1] = 0;
                player->pos.x = player->dest.x;
                player->pos.y = player->dest.y;
                player->pos.z = player->dest.z;
                player->state = 20;
                player->rot.y = 170.0f;
                lbl_1_bss_14 = 1;
            } else {
                PSVECNormalize(&diff, &diff);
                PSVECScale(&diff, &diff, 30.0f);
                PSVECAdd(&player->pos, &diff, &player->pos);
            }
            break;
        case 20:
            player->rot.y -= 5.729578f;
            player->rot.x += 5.729578f;
            if (player->rot.x >= (495.0f - (65.0f * (f32)(player->teamNo - 2)))) {
                if (player->teamNo >= 2) {
                    player->state = 21;
                } else if (obj->work[0] > 5) {
                    obj->work[0] -= 5;
                    if (obj->work[0] < 110 && obj->work[1] == 0) {
                        lbl_1_bss_14 = 2;
                        obj->work[1] = 1;
                    }
                } else {
                    obj->work[0] = 0;
                    player->state = 22;
                }
            }
            frame = obj->work[0];
            if (frame == 0.0f) {
                frame = 0.0f;
            }
            player->pos.x = lbl_1_data_248[1].x + (frame * sin((3.141592653589793 * player->rot.y) / 180.0));
            player->pos.z = lbl_1_data_248[1].z + (frame * cos((3.141592653589793 * player->rot.y) / 180.0));
            break;
        case 21:
            PSVECSubtract(&lbl_1_data_248[player->teamNo], &player->pos, &diff);
            if (PSVECMag(&diff) < 30.0f) {
                player->pos.x = lbl_1_data_248[player->teamNo].x;
                player->pos.y = lbl_1_data_248[player->teamNo].y;
                player->pos.z = lbl_1_data_248[player->teamNo].z;
                player->state = 25;
            } else {
                PSVECNormalize(&diff, &diff);
                PSVECScale(&diff, &diff, 30.0f);
                PSVECAdd(&player->pos, &diff, &player->pos);
            }
            break;
        case 22:
            switch (obj->work[0]) {
                case 0:
                    omVibrate(player->index, 0x5A, 6, 6);
                    Hu3DMotionTimeSet(obj->mdlId[2], 0.0f);
                    Hu3DModelAttrReset(obj->mdlId[2], 0x40000002);
                    player->shockSe = fn_1_F34(&player->pos, 0x74C);
                    break;
                case 14:
                    fn_1_9798(&player->pos);
                    Hu3DModelAttrSet(obj->mdlId[2], 0x40000002);
                    break;
                case 74:
                    Hu3DModelAttrReset(obj->mdlId[2], 0x40000002);
                    break;
                case 89:
                    player->state = 25;
                    Hu3DMotionTimeSet(obj->mdlId[2], 0.0f);
                    Hu3DModelAttrSet(obj->mdlId[2], 0x40000002);
                    HuAudFXStop(player->shockSe);
                    break;
            }
            obj->work[0]++;
            break;
        case 25:
            obj->objFunc = fn_1_6294;
            player->rot.y = 0.0f;
            player->rot.x = 0.0f;
            Hu3DModelRotSetV(player->obj->mdlId[1], &player->rot);
            Hu3DModelRotSetV(player->obj->mdlId[2], &player->rot);
            break;
    }
    Hu3DModelPosSetV(player->obj->mdlId[1], &player->pos);
    Hu3DModelPosSetV(player->obj->mdlId[2], &player->pos);
}

static void fn_1_61E0(M523Player *player)
{
    Hu3DMotionTimeSet(player->obj->mdlId[2], 0.0f);
    Hu3DModelAttrSet(player->obj->mdlId[2], 0x40000002);
    player->shockF = 0;
    player->shockTimer = 0;
    if (player->shockSe != -1) {
        HuAudFXStop(player->shockSe);
        player->shockSe = -1;
    }
    HuPadRumbleStop(player->padNo);
    if (player->cyl != NULL) {
        player->cyl->killF = 1;
        player->cyl = NULL;
    }
}

void fn_1_6294(OMOBJ *obj)
{
    s16 pan;
    s16 vol;
    M523Player *player;

    player = (M523Player *)obj->data;
    fn_1_E58(&player->pos, &vol, &pan);
    CharModelVoicePanSet(player->charNo, pan);
    CharModelVoiceVolSet(player->charNo, vol);
    player->velSave.x = player->vel.x;
    player->velSave.y = player->vel.y;
    player->velSave.z = player->vel.z;
    player->vel.z = 0.0f;
    player->vel.y = 0.0f;
    player->vel.x = 0.0f;
    if (lbl_1_bss_18 == 2 && player->shockF != 0 && player->state != 2) {
        fn_1_61E0(player);
        player->state = 3;
        obj->objFunc = fn_1_6800;
        return;
    }
    switch (player->state) {
        case 1:
            if (lbl_1_bss_18 == 2) {
                player->state = 3;
                obj->objFunc = fn_1_6800;
                return;
            }
            fn_1_6DA4(obj);
            break;
        case 2:
            if (lbl_1_bss_18 == 2) {
                if (lbl_1_bss_13C->state == 7) {
                    player->state = 3;
                    obj->objFunc = fn_1_6800;
                }
                return;
            }
            break;
        case 8:
            omVibrate(player->index, 0x5A, 6, 6);
            Hu3DMotionTimeSet(obj->mdlId[2], 0.0f);
            Hu3DModelAttrReset(obj->mdlId[2], 0x40000002);
            player->state = 9;
            player->shockF = 1;
            player->shockSe = fn_1_F34(&player->pos, 0x74C);
            break;
        case 9:
            if (Hu3DMotionTimeGet(obj->mdlId[2]) >= 14.0f) {
                Hu3DModelAttrSet(obj->mdlId[2], 0x40000002);
                player->cyl = fn_1_9798(&player->pos);
                player->state = 10;
                player->shockTimer = 0;
            }
            break;
        case 10:
            player->shockTimer++;
            if (player->shockTimer == 60) {
                player->cyl = NULL;
                Hu3DModelAttrReset(obj->mdlId[2], 0x40000002);
                player->state = 11;
            }
            if (fn_1_73D4(player) != 0) {
                player->shockTimer = 0;
                fn_1_59F0();
                player->state = 2;
                lbl_1_bss_13C->hitNo = player->index;
            }
            break;
        case 11:
            if (Hu3DMotionEndCheck(obj->mdlId[2])) {
                player->state = 1;
                Hu3DMotionTimeSet(obj->mdlId[2], 0.0f);
                Hu3DModelAttrSet(obj->mdlId[2], 0x40000002);
                player->shockTimer = 0;
                HuPadRumbleStop(player->padNo);
                player->shockF = 0;
                if (player->shockSe != -1) {
                    HuAudFXStop(player->shockSe);
                    player->shockSe = -1;
                }
            }
            break;
        case 12:
            if (player->shockF != 0 && lbl_1_bss_13C->hitNo == -1) {
                Hu3DMotionTimeSet(obj->mdlId[2], 0.0f);
                Hu3DModelAttrSet(obj->mdlId[2], 0x40000002);
                player->shockTimer = 0;
                player->shockF = 0;
                if (player->shockSe != -1) {
                    HuAudFXStop(player->shockSe);
                    player->shockSe = -1;
                }
            }
            player->hitTimer--;
            if (player->hitTimer == 0) {
                player->state = 1;
                Hu3DMotionSet(player->fuseModel, player->fuseMot);
            }
            break;
    }
    PSVECScale(&player->vel, &player->vel, 0.2f);
    PSVECScale(&player->velSave, &player->velSave, 0.8f);
    PSVECAdd(&player->vel, &player->velSave, &player->vel);
}

static void fn_1_6800(OMOBJ *obj)
{
    Point3d diff;
    Point3d tmp;
    Point3d push;
    f32 mag;
    s32 i;
    M523Player *player;
    M523Player *other;

    player = (M523Player *)obj->data;
    player->stkY = 0;
    player->stkX = 0;
    player->vel.z = 0.0f;
    player->vel.y = 0.0f;
    player->vel.x = 0.0f;
    if (player->doneF != 0) {
        player->state = 5;
        if (lbl_1_bss_1C == 0) {
            obj->work[0] = (3 - player->teamNo) * 20;
            obj->objFunc = fn_1_7898;
        } else {
            obj->objFunc = fn_1_76E4;
        }
        return;
    }
    PSVECSubtract(&player->dest, &player->pos, &diff);
    diff.y = 0.0f;
    if (PSVECMag(&diff) < 15.0f) {
        player->doneF = 1;
        player->vel.x = diff.x;
        player->vel.y = diff.y;
        player->vel.z = diff.z;
    } else {
        PSVECNormalize(&diff, &diff);
        player->stkX = 56.0f * diff.x;
        player->stkY = 56.0f * diff.z;
        mag = (f32)((player->stkX * player->stkX) + (player->stkY * player->stkY));
        if (mag > 0.0f) {
            mag = sqrtf(mag);
        }
        player->vel.x = 0.26785713f * player->stkX;
        player->vel.z = 0.26785713f * player->stkY;
        player->vel.y = 0.0f;
    }
    fn_1_4110(player);
    for (i = 0; i < 4; i++) {
        lbl_1_bss_140[i]->prevPos.x = lbl_1_bss_140[i]->pos.x;
        lbl_1_bss_140[i]->prevPos.y = lbl_1_bss_140[i]->pos.y;
        lbl_1_bss_140[i]->prevPos.z = lbl_1_bss_140[i]->pos.z;
    }
    PSVECAdd(&player->pos, &player->vel, &player->pos);
    fn_1_427C(player);
    Hu3DModelPosSetV(player->obj->mdlId[1], &player->pos);
    Hu3DModelRotSetV(player->obj->mdlId[1], &player->rot);
    Hu3DModelPosSetV(player->obj->mdlId[2], &player->pos);
    Hu3DModelRotSetV(player->obj->mdlId[2], &player->rot);
}

static s32 fn_1_7358(M523Player *player);

static void fn_1_6DA4(OMOBJ *obj)
{
    f32 mag;
    M523Player *player;

    player = (M523Player *)obj->data;
    if (fn_1_7358(player) != 0) {
        player->shockTimer = 0;
        player->state = 8;
        return;
    }
    if (player->type != 0) {
        mag = fn_1_8DF0(player);
    } else {
        mag = fn_1_43A4(player);
    }
    if (mag > 30.0f) {
        player->stkX = (f32)(player->stkX * 30) / mag;
        player->stkY = (f32)(player->stkY * 30) / mag;
        mag = 30.0f;
    }
    if (mag >= 15.0f) {
        player->vel.x = 1.6666666f * player->stkX;
        player->vel.z = 1.6666666f * player->stkY;
        return;
    }
    if (mag > 0.0f) {
        player->vel.x = 0.16666667f * player->stkX;
        player->vel.z = 0.16666667f * player->stkY;
    }
}

void fn_1_7198(M523Player *player)
{
    player->hitTimer = 40;
    Hu3DMotionSet(player->fuseModel, player->fuseMotHit);
    omVibrate(player->index, 12, 12, 0);
    player->aiState = 0;
    if (player->state != 12) {
        player->state = 12;
        if (player->cyl != NULL) {
            player->cyl->killF = 1;
            player->cyl = NULL;
        }
        fn_1_4700(player);
    }
}

static s32 fn_1_7358(M523Player *player)
{
    s32 result;

    result = 0;
    if (player->type != 0) {
        result = fn_1_90EC(player);
    } else if (HuPadBtnDown[player->padNo] & 0x100) {
        result = 1;
    }
    return result;
}

static s32 fn_1_73D4(M523Player *player)
{
    Point3d diff;

    if (lbl_1_bss_13C->state != 1) {
        return 0;
    }
    if (lbl_1_bss_18 == 2) {
        return 0;
    }
    PSVECSubtract(&lbl_1_bss_13C->pos, &player->pos, &diff);
    diff.y = 0.0f;
    if (PSVECMag(&diff) < 100.0f) {
        return 1;
    }
    return 0;
}

static void fn_1_7488(void)
{
    Point3d diff0;
    Point3d diff1;
    s16 idx[3];
    s16 num;
    f32 dist1;
    s32 i;
#ifdef TARGET_PC
    M523Player *others[4] = { NULL, NULL, NULL, NULL };
#else
    M523Player *others[2];
#endif

    for (i = 0; i < 3; i++) {
        idx[i] = -1;
    }
    idx[0] = lbl_1_bss_13C->hitNo;
    num = 0;
    for (i = 0; i < 4; i++) {
        if (lbl_1_bss_140[i]->teamNo != 0 && i != lbl_1_bss_13C->hitNo) {
            others[num++] = lbl_1_bss_140[i];
        }
    }
#ifdef TARGET_PC
    if (others[1] == NULL) {
        return;
    }
#endif
    PSVECSubtract(&others[0]->pos, &lbl_1_data_2A8[1], &diff0);
    PSVECSubtract(&others[1]->pos, &lbl_1_data_2A8[1], &diff1);
    dist1 = PSVECMag(&diff1);
    if (PSVECMag(&diff0) < dist1) {
        idx[1] = others[0]->index;
        idx[2] = others[1]->index;
    } else {
        idx[1] = others[1]->index;
        idx[2] = others[0]->index;
    }
    for (i = 0; i < 3; i++) {
#ifdef TARGET_PC
        if (idx[i] < 0) {
            continue;
        }
#endif
        lbl_1_bss_140[idx[i]]->dest.x = lbl_1_data_2A8[i].x;
        lbl_1_bss_140[idx[i]]->dest.y = lbl_1_data_2A8[i].y;
        lbl_1_bss_140[idx[i]]->dest.z = lbl_1_data_2A8[i].z;
    }
}

void fn_1_7654(void)
{
    M523Player *player;
    s32 i;

    for (i = 0; i < 4; i++) {
        player = lbl_1_bss_140[i];
        if (player->teamNo != 0) {
            lbl_1_bss_140[i]->dest.x = lbl_1_data_278[player->teamNo].x;
            lbl_1_bss_140[i]->dest.y = lbl_1_data_278[player->teamNo].y;
            lbl_1_bss_140[i]->dest.z = lbl_1_data_278[player->teamNo].z;
        }
    }
}

static void fn_1_76E4(OMOBJ *obj)
{
    M523Player *player;

    player = (M523Player *)obj->data;
    if (lbl_1_bss_18 == 4) {
        switch (player->state) {
            case 5:
                fn_1_3F68(obj, 9);
                Hu3DMotionSet(player->fuseModel, player->fuseMotLose);
                Hu3DModelAttrReset(player->fuseModel, 0x40000001);
                player->state = 6;
                fn_1_F34(&player->pos, 0x74F);
                return;
            case 6:
                if (Hu3DMotionEndCheck(player->fuseModel)) {
                    fn_1_3F68(obj, 8);
                    player->state = 0;
                }
                break;
        }
    }
}

static void fn_1_7898(OMOBJ *obj)
{
    Point3d tmp;
    M523Player *player;

    player = (M523Player *)obj->data;
    if (lbl_1_bss_18 == 4 && (obj->work[0] == 0 || (obj->work[0] -= 1, obj->work[0] == 0))) {
        switch (player->state) {
            case 5:
                fn_1_3F68(obj, 9);
                Hu3DMotionSet(player->fuseModel, player->fuseMotWin);
                Hu3DModelAttrReset(player->fuseModel, 0x40000001);
                player->state = 13;
                omVibrate(player->index, 0x3C, 0xC, 0);
                fn_1_F34(&player->pos, 0x750);
                obj->work[1] = 0;
                return;
            case 13:
                if (Hu3DMotionTimeGet(player->fuseModel) >= 49.0f) {
                    Hu3DModelObjPosGet(player->fuseModel, lbl_1_data_350, &player->pos);
                    omResetStatBit(obj, 0x100);
                    Hu3DModelHookReset(player->fuseModel);
                    player->state = 6;
                    player->vel.z = 0.0f;
                    player->vel.y = 0.0f;
                    player->vel.x = 0.0f;
                    fn_1_3F68(obj, 10);
                    omSetTra(obj, player->pos.x, player->pos.y, player->pos.z);
                    obj->work[1] = 0;
                    return;
                }
                break;
            case 6:
                obj->work[1]++;
                player->vel.y = 1000.0 * sin((3.141592653589793 * (f32)(2.8f * (f32)obj->work[1])) / 180.0);
                player->vel.z += 3.5f;
                PSVECAdd(&player->pos, &player->vel, &tmp);
                if (tmp.y < 0.0f) {
                    fn_1_3F68(obj, 7);
                    player->vel.z = 0.0f;
                    player->vel.y = 0.0f;
                    player->vel.x = 0.0f;
                    tmp.y = 0.0f;
                    player->state = 0;
                }
                omSetTra(obj, tmp.x, tmp.y, tmp.z);
                break;
        }
    }
}

void fn_1_7C84(OMOBJ *obj)
{
    s32 hit[4][4];
    Point3d diff;
    Point3d relVel;
    s32 i;
    s32 j;
    M523Player *player;
    M523Player *other;

    if (lbl_1_bss_18 == 2) {
        omDelObjEx(lbl_1_bss_E4, obj);
        return;
    }
    if (lbl_1_bss_13C->state != 2) {
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                hit[i][j] = 0;
            }
        }
        for (i = 0; i < 4; i++) {
            player = lbl_1_bss_140[i];
            if (player != lbl_1_bss_13C) {
                fn_1_4110(player);
                PSVECAdd(&player->pos, &player->vel, &player->pos);
                for (j = 0; j < 4; j++) {
                    other = lbl_1_bss_140[j];
                    if (player != other && other != lbl_1_bss_13C) {
                        PSVECSubtract(&player->pos, &other->pos, &diff);
                        if (!(PSVECMag(&diff) > (player->radius + other->radius))) {
                            PSVECSubtract(&player->vel, &other->vel, &relVel);
                            PSVECNormalize(&diff, &diff);
                            {
                                f32 dot;

                                dot = PSVECDotProduct(&relVel, &diff);
                                player->vel.x = player->vel.x + (((f64)-1.8f * (diff.x * dot)) * 0.5);
                                player->vel.y = player->vel.y + (((f64)-1.8f * (diff.y * dot)) * 0.5);
                                player->vel.z = player->vel.z + (((f64)-1.8f * (diff.z * dot)) * 0.5);
                                other->vel.x = other->vel.x + (((f64)1.8f * (diff.x * dot)) * 0.5);
                                other->vel.y = other->vel.y + (((f64)1.8f * (diff.y * dot)) * 0.5);
                                other->vel.z = other->vel.z + (((f64)1.8f * (diff.z * dot)) * 0.5);
                            }
                            hit[i][j] = 1;
                            fn_1_7198(lbl_1_bss_140[i]);
                            fn_1_7198(lbl_1_bss_140[j]);
                            PSVECScale(&diff, &diff, 1.0f + (player->radius + other->radius));
                            PSVECAdd(&other->pos, &diff, &player->pos);
                        }
                    }
                }
            }
        }
        for (i = 0; i < 4; i++) {
            if (lbl_1_bss_140[i]->teamNo != 0) {
                player = lbl_1_bss_140[i];
                Hu3DModelPosSetV(player->obj->mdlId[1], &player->pos);
                Hu3DModelRotSetV(player->obj->mdlId[1], &player->rot);
                Hu3DModelPosSetV(player->obj->mdlId[2], &player->pos);
                Hu3DModelRotSetV(player->obj->mdlId[2], &player->rot);
            }
        }
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                if (hit[i][j] == 1) {
                    PSVECAdd(&lbl_1_bss_140[i]->pos, &lbl_1_bss_140[j]->pos, &diff);
                    PSVECScale(&diff, &diff, 0.5f);
                    fn_1_4590(&diff);
                }
            }
        }
    }
}

static f32 fn_1_85A8(M523Player *player)
{
    s32 chance[4] = { 0, 0, 0x23, 0x19 };
    Point3d diff;
    f32 mag;
    s16 hitNo;

    player->stkY = 0;
    player->stkX = 0;
    PSVECSubtract(&player->aiTarget, &player->pos, &diff);
    diff.y = 0.0f;
    if (PSVECMag(&diff) < 15.0f) {
        player->aiState = 0;
    }
    switch (player->aiState) {
        case 0:
            fn_1_8874(player);
            player->aiState = 1;
            player->aiTimer = 100;
        case 1:
            if (player->dif >= 2 && (hitNo = fn_1_8D14(player), hitNo != -1) && frandmod(100) <= chance[player->dif]) {
                if (player->dif <= 2) {
                    player->aiState = 0;
                } else {
                    fn_1_8A80(player, hitNo);
                }
            } else {
                player->aiTimer--;
                if (player->aiTimer == 0) {
                    player->aiState = 0;
                }
                fn_1_8B60(player, 1);
            }
            break;
    }
    mag = (f32)((player->stkX * player->stkX) + (player->stkY * player->stkY));
    if (mag > 0.0f) {
        mag = sqrtf(mag);
    }
    return mag;
}

static void fn_1_8874(M523Player *player)
{
    Point3d diff;
    f32 ang;
    f32 dist;
    s32 retry;
    s32 overlap;
    s32 i;

    retry = 0;
    do {
        overlap = 0;
        retry++;
        if (player->dif == 0) {
            dist = 800.0f - player->radius;
        } else {
            dist = frandmod((s16)(800.0f - player->radius));
        }
        ang = frandmod(0x168);
        player->aiTarget.x = dist * cos((3.141592653589793 * ang) / 180.0);
        player->aiTarget.z = dist * sin((3.141592653589793 * ang) / 180.0);
        player->aiTarget.y = 0.0f;
        for (i = 0; i < 3; i++) {
            PSVECSubtract(&player->aiTarget, &lbl_1_data_1E4[i], &diff);
            if (PSVECMag(&diff) < (50.0f + player->radius)) {
                overlap = 1;
            }
        }
        PSVECSubtract(&player->pos, &player->aiTarget, &diff);
        if (PSVECMag(&diff) < 300.0f && retry <= 10) {
            overlap = 1;
        }
    } while (overlap != 0);
}

static void fn_1_8A80(M523Player *player, s16 hitNo)
{
    Point3d diff;
    f32 radius;

    PSVECSubtract(&player->pos, &lbl_1_bss_140[hitNo]->pos, &diff);
    PSVECScale(&diff, &diff, 200.0f);
    PSVECAdd(&player->pos, &diff, &player->aiTarget);
    radius = player->radius;
    if (PSVECMag(&player->aiTarget) > (800.0f - radius)) {
        PSVECNormalize(&player->aiTarget, &player->aiTarget);
        PSVECScale(&player->aiTarget, &player->aiTarget, 800.0f - radius);
    }
}

static void fn_1_8B60(M523Player *player, s32 mode)
{
    Point3d dir;
    f32 base;
    f32 diff;
    f32 ang;

#ifdef TARGET_PC
    dir.x = 0.0f;
    dir.y = 0.0f;
    dir.z = 0.0f;
#endif
    if (mode != 0) {
        base = player->rot.y;
        diff = fn_1_CEC(base, fn_1_C88(&player->pos, &player->aiTarget));
        if (fabs(diff) < 15.0f) {
            ang = base + diff;
        } else if (diff < 0.0f) {
            ang = base - 15.0f;
        } else {
            ang = base + 15.0f;
        }
        dir.x = sin((3.141592653589793 * ang) / 180.0);
        dir.z = cos((3.141592653589793 * ang) / 180.0);
        dir.y = 0.0f;
    } else {
        PSVECNormalize(&dir, &dir);
    }
    player->stkX = 56.0f * dir.x;
    player->stkY = 56.0f * dir.z;
}

static s16 fn_1_8D14(M523Player *player)
{
    Point3d out;
    s16 result;
    s16 i;
    M523Player *other;
    f32 dist;

    result = -1;
    for (i = 0; i < 4; i++) {
        other = lbl_1_bss_140[i];
        if (other != player && other->shockF != 0 && (dist = fn_1_D58(&other->pos, &player->pos, &player->aiTarget, &out), !(dist < 0.0f)) && dist < 100.0f) {
            result = i;
            break;
        }
    }
    return result;
}

static f32 fn_1_8DF0(M523Player *player)
{
    Point3d diff;
    f32 mag;

    switch (player->aiState) {
        case 0:
            if (player->aiTimer == 0) {
                fn_1_92F4(player);
                player->aiState = 1;
                break;
            }
            player->aiTimer--;
            return 0.0f;
        case 1:
            if (player->index == fn_1_9218()) {
                fn_1_9440(player);
                player->aiState = 2;
            }
            break;
        case 2:
            fn_1_9440(player);
            break;
    }
    PSVECSubtract(&player->aiTarget, &player->pos, &diff);
    diff.y = 0.0f;
    if (PSVECMag(&diff) < 50.0f) {
        player->aiState = 0;
        if (player->dif == 0) {
            player->aiTimer = frandmod(30) + 45;
        } else if (player->dif == 1) {
            player->aiTimer = frandmod(30) + 15;
        } else {
            player->aiTimer = 0;
        }
        return 0.0f;
    }
    PSVECNormalize(&diff, &diff);
    player->stkX = 56.0f * diff.x;
    player->stkY = 56.0f * diff.z;
    mag = (f32)((player->stkX * player->stkX) + (player->stkY * player->stkY));
    if (mag > 0.0f) {
        mag = sqrtf(mag);
    }
    return mag;
}

s32 fn_1_90EC(M523Player *player)
{
    s32 chance[4] = { 10, 15, 20, 20 };
    Point3d diff;

    PSVECSubtract(&lbl_1_bss_13C->pos, &player->pos, &diff);
    diff.y = 0.0f;
    if (PSVECMag(&diff) < 100.0f) {
        if (frandmod(100) < chance[player->dif]) {
            if (player->dif <= 2) {
                return 1;
            }
            PSVECSubtract(&player->aiTarget, &player->pos, &diff);
            if (PSVECMag(&diff) < 50.0f) {
                return 1;
            }
        } else if (frandmod(100) < chance[player->dif]) {
            return 1;
        }
    }
    return 0;
}

static s16 fn_1_9218(void)
{
    Point3d diff;
    f32 best;
    s16 result;
    s16 i;

    best = 9999.9f;
    result = -1;
    for (i = 0; i < 4; i++) {
        if (lbl_1_bss_140[i] != lbl_1_bss_13C) {
            PSVECSubtract(&lbl_1_bss_13C->pos, &lbl_1_bss_140[i]->pos, &diff);
            if (PSVECMag(&diff) < best) {
                best = PSVECMag(&diff);
                result = i;
            }
        }
    }
    return result;
}

static void fn_1_92F4(M523Player *player)
{
    Point3d diff;
    f32 range;
    f32 dist;
    f32 ang;
    s32 retry;

    do {
        retry = 0;
        range = 800.0f - player->radius;
        dist = range - 5.0f;
        ang = frandmod(0x168);
        player->aiTarget.x = dist * cos((3.141592653589793 * ang) / 180.0);
        player->aiTarget.z = dist * sin((3.141592653589793 * ang) / 180.0);
        player->aiTarget.y = 0.0f;
        PSVECSubtract(&player->pos, &player->aiTarget, &diff);
        if (PSVECMag(&diff) < 200.0f) {
            retry = 1;
        }
    } while (retry != 0);
}

static void fn_1_9440(M523Player *player)
{
    Point3d sum;
    f32 radius;
    s32 i;

    if (player->dif < 2) {
        player->aiTarget.x = lbl_1_bss_13C->pos.x;
        player->aiTarget.y = lbl_1_bss_13C->pos.y;
        player->aiTarget.z = lbl_1_bss_13C->pos.z;
    } else {
        sum.z = 0.0f;
        sum.y = 0.0f;
        sum.x = 0.0f;
        for (i = 0; i < 10; i++) {
            PSVECAdd(&sum, &lbl_1_bss_150[i], &sum);
        }
        PSVECAdd(&sum, &lbl_1_bss_13C->pos, &player->aiTarget);
    }
    radius = player->radius - 50.0f;
    if (PSVECMag(&player->aiTarget) > (800.0f - radius)) {
        PSVECNormalize(&player->aiTarget, &player->aiTarget);
        PSVECScale(&player->aiTarget, &player->aiTarget, 800.0f - radius);
    }
}

static void fn_1_9574(void)
{
    lbl_1_bss_10C.next = &lbl_1_bss_E8;
    lbl_1_bss_10C.prev = NULL;
    lbl_1_bss_10C.index = 0;
    lbl_1_bss_10C.model = -1;
    lbl_1_bss_E8.next = NULL;
    lbl_1_bss_E8.prev = &lbl_1_bss_10C;
    lbl_1_bss_E8.model = -1;
    Hu3DModelLayerSet(Hu3DHookFuncCreate(fn_1_9978), 1);
}

void fn_1_98E8(M523Cyl *cyl);

static void fn_1_962C(OMOBJ *obj)
{
    M523Cyl *cyl;

    cyl = (M523Cyl *)obj->data;
    switch (cyl->state) {
        case 0:
            if (cyl->killF != 0 || Hu3DMotionEndCheck(obj->mdlId[0])) {
                Hu3DModelAttrSet(obj->mdlId[0], 0x40000002);
                cyl->state = 1;
                cyl->timer = 180;
                return;
            }
            return;
        case 1:
            cyl->timer--;
            if (cyl->timer <= 0) {
                cyl->state = 2;
                return;
            }
            break;
        case 2:
            cyl->timer++;
            Hu3DModelTPLvlSet(obj->mdlId[0], 1.0f - (0.02f * cyl->timer));
            if (cyl->timer >= 50) {
                cyl->state = 3;
                return;
            }
            break;
        case 3:
            fn_1_98E8(cyl);
            Hu3DModelKill(obj->mdlId[0]);
            omDelObjEx(lbl_1_bss_E4, obj);
            break;
    }
}

static M523Cyl *fn_1_9798(Point3d *pos)
{
    OMOBJ *obj;
    M523Cyl *cyl;
    HU3DMODELID model;

    obj = omAddObjEx(lbl_1_bss_E4, 0x1200, 1, 0, -1, fn_1_962C);
    obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M523Cyl), HU_MEMNUM_OVL);
    cyl = (M523Cyl *)obj->data;
    cyl->state = 0;
    cyl->killF = 0;
    lbl_1_bss_E8.prev->next = cyl;
    cyl->prev = lbl_1_bss_E8.prev;
    cyl->next = &lbl_1_bss_E8;
    lbl_1_bss_E8.prev = cyl;
    cyl->index = cyl->prev->index + 1;
    model = Hu3DModelCreate(HuDataSelHeapReadNum(0x510013, HU_MEMNUM_OVL, HEAP_MODEL));
    cyl->model = model;
    obj->mdlId[0] = model;
    Hu3DModelLayerSet(model, 1);
    Hu3DModelAttrSet(model, 0x400000);
    omSetTra(obj, 5000.0f, 0.0f, 0.0f);
    cyl->pos.x = pos->x;
    cyl->pos.y = pos->y;
    cyl->pos.z = pos->z;
    cyl->pos.y = 0.0f;
    return cyl;
}

void fn_1_98E8(M523Cyl *cyl)
{
    cyl->next->prev = cyl->prev;
    cyl->prev->next = cyl->next;
    fn_1_992C();
}

void fn_1_992C(void)
{
    M523Cyl *node;
    s16 idx;

    idx = 0;
    node = &lbl_1_bss_10C;
    while (node->next != NULL) {
        node->index = idx++;
        node = node->next;
    }
}

static void fn_1_9978(HU3DMODEL *modelP, Mtx *mtx)
{
    HU3DMODEL *model;
    M523Cyl *node;
    Mtx m;

    Hu3DModelObjDrawInit();
    node = &lbl_1_bss_10C;
    while (node->next != NULL) {
        if (node->model != -1) {
            model = &Hu3DData[node->model];
            PSMTXTrans(m, node->pos.x, node->pos.y, node->pos.z);
            PSMTXConcat(*mtx, m, m);
            Hu3DModelObjDraw(node->model, lbl_1_data_394, m);
        }
        node = node->next;
    }
}
