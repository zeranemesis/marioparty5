#include "game/audio.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/frand.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/object.h"
#include "game/pad.h"

#include "humath.h"

#include "REL/m570Dll.h"

/* motion numbers handled by the player state machine */
#define M570_MOT_IDLE 0
#define M570_MOT_READY 1
#define M570_MOT_SURPRISE 2
#define M570_MOT_WIN 4
#define M570_MOT_HOLD 5
#define M570_MOT_DUST 6
#define M570_MOT_APPROACH 7
#define M570_MOT_RETREAT 8
#define M570_MOT_TURN 9

#define M570_ITEM_HOOK "itemhook_R"

typedef struct {
    f32 endTime;
    u32 attr;
    s32 loop;
} M570MOTPARAM;

typedef struct {
    s32 a;
    s32 b;
    s32 c;
    s32 d;
} M570ITEMATTR;

/* motion data numbers, one entry per player */
static u32 CharMotDataTbl[11] = {
    0x8C0000, 0x8C0001, 0x8C0002, 0x8700CA, 0x8C0083, 0x8700BF,
    0x8C000F, 0x8C0051, 0x8C0006, 0x8C0007, 0
};

/* per-motion shift parameters: { blend end time, motion attr, loop attr flag } */
static M570MOTPARAM MotParamTbl[10] = {
    { 10.0f, HU3D_MOTATTR_LOOP, 0 },
    { 10.0f, HU3D_MOTATTR_LOOP, 0 },
    { 10.0f, HU3D_MOTATTR_LOOP, 0 },
    { 10.0f, HU3D_MOTATTR_LOOP, 1 },
    { 10.0f, HU3D_MOTATTR_LOOP, 1 },
    { 10.0f, HU3D_MOTATTR_LOOP, 1 },
    { 10.0f, 0, 0 },
    { 10.0f, HU3D_MOTATTR_LOOP, 0 },
    { 10.0f, 0, 0 },
    { 10.0f, 0, 0 }
};

/* starting rotations, indexed by player slot */
static HuVecF StartRotTbl[4] = {
    { 0.0f, 90.0f, 0.0f },
    { 0.0f, -90.0f, 0.0f },
    { 0.0f, -90.0f, 0.0f },
    { 0.0f, -90.0f, 0.0f }
};

/* starting positions, indexed by player slot */
static HuVecF StartPosTbl[4] = {
    { -300.0f, 0.0f, 50.0f },
    { 220.0f, 0.0f, -100.0f },
    { 270.0f, 0.0f, 50.0f },
    { 340.0f, 0.0f, 200.0f }
};

/* position the players run to when the round starts */
static HuVecF ReadyPosTbl[4] = {
    { -200.0f, 0.0f, 50.0f },
    { 150.0f, 0.0f, -100.0f },
    { 175.0f, 0.0f, 50.0f },
    { 200.0f, 0.0f, 200.0f }
};

/* position the players walk to on their own turn */
static HuVecF TurnPosTbl[4] = {
    { 0.0f, 0.0f, 300.0f },
    { -200.0f, 0.0f, 300.0f },
    { 0.0f, 0.0f, 300.0f },
    { 200.0f, 0.0f, 300.0f }
};

/* item models held by the four players */
static u32 ItemModelDataTbl[3] = { 0x7D000B, 0x7D000C, 0x7D000D };

/* d-pad / button mask used to pick a held item */
static u16 PadMaskTbl[4] = { 0x200, 0x100, 0x400, 0 };

/* dust emitter positions along the four walls of the board */
static HuVecF DustPosTbl[40] = {
    { -350.0f, -50.0f, -600.0f },
    { -250.0f, -50.0f, -600.0f },
    { -150.0f, -50.0f, -600.0f },
    { 350.0f, -50.0f, -600.0f },
    { 250.0f, -50.0f, -600.0f },
    { 150.0f, -50.0f, -600.0f },
    { -450.0f, -50.0f, -700.0f },
    { -350.0f, -50.0f, -700.0f },
    { -250.0f, -50.0f, -700.0f },
    { -150.0f, -50.0f, -700.0f },
    { 450.0f, -50.0f, -700.0f },
    { 350.0f, -50.0f, -700.0f },
    { 250.0f, -50.0f, -700.0f },
    { 150.0f, -50.0f, -700.0f },
    { -600.0f, -50.0f, -450.0f },
    { -600.0f, -50.0f, -325.0f },
    { -600.0f, -50.0f, -200.0f },
    { -600.0f, -50.0f, -75.0f },
    { -600.0f, -50.0f, 50.0f },
    { -600.0f, -50.0f, 175.0f },
    { -600.0f, -50.0f, 300.0f },
    { -600.0f, -50.0f, 425.0f },
    { -775.0f, -50.0f, -525.0f },
    { -775.0f, -50.0f, -325.0f },
    { -800.0f, -50.0f, -225.0f },
    { -925.0f, -50.0f, -350.0f },
    { -925.0f, -50.0f, -150.0f },
    { 600.0f, -50.0f, -450.0f },
    { 600.0f, -50.0f, -325.0f },
    { 600.0f, -50.0f, -200.0f },
    { 600.0f, -50.0f, -75.0f },
    { 600.0f, -50.0f, 50.0f },
    { 600.0f, -50.0f, 175.0f },
    { 600.0f, -50.0f, 300.0f },
    { 600.0f, -50.0f, 425.0f },
    { 750.0f, -50.0f, -650.0f },
    { 750.0f, -50.0f, -450.0f },
    { 750.0f, -50.0f, -175.0f },
    { 925.0f, -50.0f, -275.0f },
    { 900.0f, -50.0f, -475.0f }
};

static char ItemHookName[] = M570_ITEM_HOOK;

/* motion data numbers of the floating item */
static const u32 ItemMotDataTbl[4] = { 0x7D000F, 0x7D0010, 0x7D0011, 0x7D0012 };

/* motion data numbers of the dust puffs */
static const u32 DustMotDataTbl[2] = { 0x7D0015, 0x7D0016 };

static const f32 kBlendLong = 75.0f;
static const f32 kZero = 0.0f;
static const f32 kStep = 5.0f;
static const f32 kTen = 10.0f;
static const f32 kArrive = 0.1f;
static const f32 kWalk = 3.0f;
static const f32 kOne = 1.0f;
static const f32 kItemZ = -345.0f;
static const M570ITEMATTR ItemShiftAttr = { HU3D_MOTATTR_LOOP, 0, 0, HU3D_MOTATTR_LOOP };
static const f32 kMinX = -500.0f;
static const f32 kSideRot = 90.0f;
static const f32 kMaxX = 500.0f;
static const f32 kSideRotNeg = -90.0f;

static M570DUST *gDustTbl[M570_DUST_MAX];
static OMOBJ *gItemObj;
static M570PLAYER *gCpuPlayer[M570_CPU_MAX];
static M570PLAYER *gPlayer;
static M570PLAYER *gPlayerTbl[M570_PLAYER_MAX];

static void fn_1_4440(M570PLAYER *player, s16 motNo);
static void fn_1_45E4(OMOBJ *obj);
static void fn_1_457C(OMOBJ *obj);
static void fn_1_4AC8(M570PLAYER *player);
static void fn_1_4BB0(M570PLAYER *player);
static void fn_1_4CC8(M570PLAYER *player);
static void fn_1_5820(s16 motNo);
static void fn_1_58EC(OMOBJ *obj);
static void fn_1_558C(OMOBJ *obj);

void fn_1_4210(void);
s32 fn_1_4B64(M570PLAYER *player);
void fn_1_4D2C(void);
void fn_1_4DC4(void);
void fn_1_4EF8(void);
void fn_1_5048(void);
void fn_1_51A8(void);
void fn_1_5354(void);
s32 fn_1_5538(void);
void fn_1_56A0(void);
void fn_1_5A78(void);
void fn_1_5D74(void);
void fn_1_5DB8(void);
void fn_1_5E38(void);
void fn_1_5F24(s32 loop);

/* Hu3DMotionMaxTimeGet is not declared in hu3d.h: the unit calls it both with a
 * float return (fn_1_58EC reads f1) and with an implicit int return that goes
 * straight into frandmod(). */
f32 Hu3DMotionMaxTimeGet(HU3DMODELID modelId);
typedef s32 (*M570MAXTIMEGET)(HU3DMODELID modelId);

void fn_1_4210(void)
{
    OMOBJ *obj;
    M570PLAYER *player;
    s16 cpuCount = 0;
    s16 i;
    s16 j;
    s16 modelId;

    for (i = 0; i < M570_PLAYER_MAX; i++) {
        obj = omAddObjEx(lbl_1_bss_108, i + 0x1000, 4, 10, OM_OBJ_NONE, fn_1_457C);
        obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M570PLAYER), HU_MEMNUM_OVL);
        memset(obj->data, 0, sizeof(M570PLAYER));
        player = obj->data;
        player->obj = obj;
        fn_1_3A74(player, i);
        if (player->grp == 0) {
            player->idx = 0;
            gPlayer = player;
        } else {
            player->idx = cpuCount + 1;
            gCpuPlayer[cpuCount] = player;
            cpuCount++;
        }
        gPlayerTbl[i] = player;
        player->tra = StartPosTbl[player->idx];
        player->rot = StartRotTbl[player->idx];
        player->state = 0;
        player->gotItem = 0;
        player->motNo = -1;
        modelId = CharModelMotListCreate(player->charNo, 2, (unsigned int *)CharMotDataTbl, obj->mtnId);
        obj->mdlId[0] = modelId;
        CharMotionSet(player->charNo, obj->mtnId[0]);
        Hu3DModelLayerSet(modelId, 2);
        Hu3DModelShadowSet(modelId);
        fn_1_4440(player, 0);
        for (j = 0; j < M570_CPU_MAX; j++) {
            modelId = Hu3DModelCreate(HuDataSelHeapReadNum(ItemModelDataTbl[j], HU_MEMNUM_OVL, HEAP_MODEL));
            obj->mdlId[j + 1] = modelId;
            Hu3DModelAttrSet(modelId, 1);
        }
    }
    CharEffectLayerSet(3);
}

static void fn_1_4440(M570PLAYER *player, s16 motNo)
{
    OMOBJ *obj;
    f32 shiftStart;

    if (player->motNo != motNo) {
        obj = player->obj;
        if (player->motNo == 5 && motNo == M570_MOT_WIN) {
            shiftStart = kBlendLong;
        } else {
            shiftStart = kZero;
        }
        CharMotionShiftSet(player->charNo, obj->mtnId[motNo], shiftStart, MotParamTbl[motNo].endTime,
                          MotParamTbl[motNo].attr);
        if (MotParamTbl[motNo].loop != 0) {
            Hu3DMotionAttrSet(obj->mtnId[motNo], 1);
        } else {
            Hu3DMotionAttrReset(obj->mtnId[motNo], 1);
        }
        player->motNo = motNo;
    }
}

static void fn_1_457C(OMOBJ *obj)
{
    M570PLAYER *player = obj->data;

    obj->objFunc = fn_1_45E4;
    omSetRot(obj, player->rot.x, player->rot.y, player->rot.z);
    omSetTra(obj, player->tra.x, player->tra.y, player->tra.z);
}

static void fn_1_45E4(OMOBJ *obj)
{
    M570PLAYER *player = obj->data;

    CharModelVoicePanSet(player->charNo, fn_1_17DC(&player->tra));
    switch (player->state) {
    case M570_MOT_APPROACH:
        {
            HuVecF angle;
            OMOBJ *robj;
            f32 shiftStart;

            if (fn_1_3B40(&player->tra, &player->goal, &angle, kStep, &kStep) != 0) {
                player->state = 0;
                if (player->motNo != M570_MOT_IDLE) {
                    robj = player->obj;
                    if (player->motNo == 5 && M570_MOT_IDLE == M570_MOT_WIN) {
                        shiftStart = kBlendLong;
                    } else {
                        shiftStart = kZero;
                    }
                    CharMotionShiftSet(player->charNo, robj->mtnId[M570_MOT_IDLE], shiftStart,
                                      MotParamTbl[M570_MOT_IDLE].endTime, MotParamTbl[M570_MOT_IDLE].attr);
                    if (MotParamTbl[M570_MOT_IDLE].loop != 0) {
                        Hu3DMotionAttrSet(robj->mtnId[M570_MOT_IDLE], 1);
                    } else {
                        Hu3DMotionAttrReset(robj->mtnId[M570_MOT_IDLE], 1);
                    }
                    player->motNo = M570_MOT_IDLE;
                }
            }
        }
        break;
    case M570_MOT_RETREAT:
        if (fn_1_3B40(&player->tra, &player->goal, &player->rot, kTen, &kTen) != 0) {
            player->state = M570_MOT_TURN;
        }
        break;
    case M570_MOT_TURN:
        {
            OMOBJ *robj;
            f32 shiftStart;

            player->rot.y = fn_1_3C64(&kTen, player->rot.y, kZero, kTen);
            if (player->rot.y <= kArrive) {
                if (player->motNo != M570_MOT_IDLE) {
                    robj = player->obj;
                    if (player->motNo == 5 && M570_MOT_IDLE == M570_MOT_WIN) {
                        shiftStart = kBlendLong;
                    } else {
                        shiftStart = kZero;
                    }
                    CharMotionShiftSet(player->charNo, robj->mtnId[M570_MOT_IDLE], shiftStart,
                                      MotParamTbl[M570_MOT_IDLE].endTime, MotParamTbl[M570_MOT_IDLE].attr);
                    if (MotParamTbl[M570_MOT_IDLE].loop != 0) {
                        Hu3DMotionAttrSet(robj->mtnId[M570_MOT_IDLE], 1);
                    } else {
                        Hu3DMotionAttrReset(robj->mtnId[M570_MOT_IDLE], 1);
                    }
                    player->motNo = M570_MOT_IDLE;
                }
                player->state = 0;
            }
        }
        break;
    case M570_MOT_READY:
        fn_1_4AC8(player);
        break;
    case M570_MOT_SURPRISE:
        if (player->timer++ == 15) {
            fn_1_4BB0(player);
        }
        if (player->timer == 20) {
            fn_1_4CC8(player);
            player->state = 0;
        }
        break;
    case M570_MOT_WIN:
        {
            OMOBJ *robj;
            f32 shiftStart;

            player->timer++;
            if (player->timer == 20) {
                if (player->motNo != M570_MOT_WIN) {
                    robj = player->obj;
                    if (player->motNo == 5 && M570_MOT_WIN == M570_MOT_WIN) {
                        shiftStart = kBlendLong;
                    } else {
                        shiftStart = kZero;
                    }
                    CharMotionShiftSet(player->charNo, robj->mtnId[M570_MOT_WIN], shiftStart,
                                      MotParamTbl[M570_MOT_WIN].endTime, MotParamTbl[M570_MOT_WIN].attr);
                    if (MotParamTbl[M570_MOT_WIN].loop != 0) {
                        Hu3DMotionAttrSet(robj->mtnId[M570_MOT_WIN], 1);
                    } else {
                        Hu3DMotionAttrReset(robj->mtnId[M570_MOT_WIN], 1);
                    }
                    player->motNo = M570_MOT_WIN;
                }
                player->timer = 0;
                player->state = 0;
            }
        }
        break;
    case M570_MOT_HOLD:
        {
            OMOBJ *robj;
            f32 shiftStart;

            if (player->timer == 0) {
                omVibrate(player->objId, 30, 12, 0);
                fn_1_1864(&player->tra, 0x913);
            }
            if (player->grp == 0) {
                player->tra.x -= kWalk;
            } else {
                player->tra.x += kWalk;
            }
            if (player->timer++ == 35) {
                player->gotItem = 1;
                if (player->motNo != M570_MOT_APPROACH) {
                    robj = player->obj;
                    if (player->motNo == 5 && M570_MOT_APPROACH == M570_MOT_WIN) {
                        shiftStart = kBlendLong;
                    } else {
                        shiftStart = kZero;
                    }
                    CharMotionShiftSet(player->charNo, robj->mtnId[M570_MOT_APPROACH], shiftStart,
                                      MotParamTbl[M570_MOT_APPROACH].endTime, MotParamTbl[M570_MOT_APPROACH].attr);
                    if (MotParamTbl[M570_MOT_APPROACH].loop != 0) {
                        Hu3DMotionAttrSet(robj->mtnId[M570_MOT_APPROACH], 1);
                    } else {
                        Hu3DMotionAttrReset(robj->mtnId[M570_MOT_APPROACH], 1);
                    }
                    player->motNo = M570_MOT_APPROACH;
                }
                player->state = M570_MOT_DUST;
            }
        }
        break;
    }
    omSetRot(obj, player->rot.x, player->rot.y, player->rot.z);
    omSetTra(obj, player->tra.x, player->tra.y, player->tra.z);
}

static void fn_1_4AC8(M570PLAYER *player)
{
    s16 i;

    for (i = 0; i < M570_CPU_MAX; i++) {
        if (HuPadBtnDown[player->padNo] & PadMaskTbl[i]) {
            omVibrate(player->objId, 12, 6, 6);
            player->btnNo = i;
            break;
        }
    }
}

s32 fn_1_4B64(M570PLAYER *player)
{
    if (gPlayer == player) {
        return 0;
    }
    if (gPlayer->btnNo == player->btnNo) {
        return 1;
    }
    return 0;
}

static void fn_1_4BB0(M570PLAYER *player)
{
    char *hookName;
    s16 modelId;

    hookName = CharModelItemHookGet(player->charNo, 2, 0);
    modelId = player->obj->mdlId[player->btnNo + 1];
    Hu3DModelHookSet(player->obj->mdlId[0], hookName, modelId);
    Hu3DModelLayerSet(modelId, 3);
    Hu3DModelAttrReset(modelId, 1);
    Hu3DModelRotSet(modelId, kZero, kZero, kZero);
    Hu3DModelPosSet(modelId, kZero, kZero, kZero);
    Hu3DModelScaleSet(modelId, kOne, kOne, kOne);
}

static void fn_1_4CC8(M570PLAYER *player)
{
    char *hookName;
    HuVecF pos;

    hookName = CharModelItemHookGet(player->charNo, 2, 0);
    Hu3DModelObjPosGet(player->obj->mdlId[0], hookName, &pos);
    fn_1_16D4(&pos);
}

void fn_1_4D2C(void)
{
    s16 i;

    for (i = 0; i < M570_PLAYER_MAX; i++) {
        if (gPlayerTbl[i]->gotItem == 0) {
            gPlayerTbl[i]->state = M570_MOT_READY;
            gPlayerTbl[i]->btnNo = frandmod(M570_CPU_MAX);
        }
    }
}

void fn_1_4DC4(void)
{
    s16 i;
    M570PLAYER *player;

    for (i = 0; i < M570_PLAYER_MAX; i++) {
        player = gPlayerTbl[i];
        player->state = M570_MOT_APPROACH;
        player->goal = ReadyPosTbl[player->idx];
        if (player->motNo != M570_MOT_READY) {
            OMOBJ *robj;
            f32 shiftStart;

            robj = player->obj;
            if (player->motNo == 5 && M570_MOT_READY == M570_MOT_WIN) {
                shiftStart = kBlendLong;
            } else {
                shiftStart = kZero;
            }
            CharMotionShiftSet(player->charNo, robj->mtnId[M570_MOT_READY], shiftStart,
                              MotParamTbl[M570_MOT_READY].endTime, MotParamTbl[M570_MOT_READY].attr);
            if (MotParamTbl[M570_MOT_READY].loop != 0) {
                Hu3DMotionAttrSet(robj->mtnId[M570_MOT_READY], 1);
            } else {
                Hu3DMotionAttrReset(robj->mtnId[M570_MOT_READY], 1);
            }
            player->motNo = M570_MOT_READY;
        }
    }
}

void fn_1_4EF8(void)
{
    s16 i;
    M570PLAYER *player;

    for (i = 0; i < M570_PLAYER_MAX; i++) {
        player = gPlayerTbl[i];
        if (player->grp == lbl_1_bss_52) {
            player->state = M570_MOT_RETREAT;
            player->goal = TurnPosTbl[player->idx];
            if (player->motNo != M570_MOT_SURPRISE) {
                OMOBJ *robj;
                f32 shiftStart;

                robj = player->obj;
                if (player->motNo == 5 && M570_MOT_SURPRISE == M570_MOT_WIN) {
                    shiftStart = kBlendLong;
                } else {
                    shiftStart = kZero;
                }
                CharMotionShiftSet(player->charNo, robj->mtnId[M570_MOT_SURPRISE], shiftStart,
                                  MotParamTbl[M570_MOT_SURPRISE].endTime, MotParamTbl[M570_MOT_SURPRISE].attr);
                if (MotParamTbl[M570_MOT_SURPRISE].loop != 0) {
                    Hu3DMotionAttrSet(robj->mtnId[M570_MOT_SURPRISE], 1);
                } else {
                    Hu3DMotionAttrReset(robj->mtnId[M570_MOT_SURPRISE], 1);
                }
                player->motNo = M570_MOT_SURPRISE;
            }
        }
    }
}

void fn_1_5048(void)
{
    s16 i;

    HuAudFXPlay(0x911);
    for (i = 0; i < M570_PLAYER_MAX; i++) {
        if (gPlayerTbl[i]->gotItem == 0) {
            M570PLAYER *player;
            OMOBJ *robj;
            f32 shiftStart;

            gPlayerTbl[i]->state = M570_MOT_SURPRISE;
            gPlayerTbl[i]->timer = 0;
            player = gPlayerTbl[i];
            if (player->motNo != M570_MOT_HOLD) {
                robj = player->obj;
                if (player->motNo == 5 && M570_MOT_HOLD == M570_MOT_WIN) {
                    shiftStart = kBlendLong;
                } else {
                    shiftStart = kZero;
                }
                CharMotionShiftSet(player->charNo, robj->mtnId[M570_MOT_HOLD], shiftStart,
                                  MotParamTbl[M570_MOT_HOLD].endTime, MotParamTbl[M570_MOT_HOLD].attr);
                if (MotParamTbl[M570_MOT_HOLD].loop != 0) {
                    Hu3DMotionAttrSet(robj->mtnId[M570_MOT_HOLD], 1);
                } else {
                    Hu3DMotionAttrReset(robj->mtnId[M570_MOT_HOLD], 1);
                }
                player->motNo = M570_MOT_HOLD;
            }
        }
    }
}

void fn_1_51A8(void)
{
    s16 i;
    M570PLAYER *player;

    HuAudFXPlay(0x912);
    for (i = 0; i < M570_PLAYER_MAX; i++) {
        player = gPlayerTbl[i];
        if (player->gotItem == 0) {
            CharModelHookDustCreate(player->charNo, CharModelItemHookGet(player->charNo, 2, 0));
            if (fn_1_4B64(player)) {
                OMOBJ *robj;
                f32 shiftStart;

                player->gotItem = 1;
                player->state = M570_MOT_HOLD;
                if (player->motNo != M570_MOT_DUST) {
                    robj = player->obj;
                    if (player->motNo == 5 && M570_MOT_DUST == M570_MOT_WIN) {
                        shiftStart = kBlendLong;
                    } else {
                        shiftStart = kZero;
                    }
                    CharMotionShiftSet(player->charNo, robj->mtnId[M570_MOT_DUST], shiftStart,
                                      MotParamTbl[M570_MOT_DUST].endTime, MotParamTbl[M570_MOT_DUST].attr);
                    if (MotParamTbl[M570_MOT_DUST].loop != 0) {
                        Hu3DMotionAttrSet(robj->mtnId[M570_MOT_DUST], 1);
                    } else {
                        Hu3DMotionAttrReset(robj->mtnId[M570_MOT_DUST], 1);
                    }
                    player->motNo = M570_MOT_DUST;
                }
            } else {
                player->state = M570_MOT_WIN;
            }
            player->timer = 0;
        }
    }
}

void fn_1_5354(void)
{
    s16 i;
    M570PLAYER *player;

    for (i = 0; i < M570_PLAYER_MAX; i++) {
        if (lbl_1_bss_52 == gPlayerTbl[i]->grp) {
            player = gPlayerTbl[i];
            if (player->motNo != M570_MOT_RETREAT) {
                OMOBJ *robj;
                f32 shiftStart;

                robj = player->obj;
                if (player->motNo == 5 && M570_MOT_RETREAT == M570_MOT_WIN) {
                    shiftStart = kBlendLong;
                } else {
                    shiftStart = kZero;
                }
                CharMotionShiftSet(player->charNo, robj->mtnId[M570_MOT_RETREAT], shiftStart,
                                  MotParamTbl[M570_MOT_RETREAT].endTime, MotParamTbl[M570_MOT_RETREAT].attr);
                if (MotParamTbl[M570_MOT_RETREAT].loop != 0) {
                    Hu3DMotionAttrSet(robj->mtnId[M570_MOT_RETREAT], 1);
                } else {
                    Hu3DMotionAttrReset(robj->mtnId[M570_MOT_RETREAT], 1);
                }
                player->motNo = M570_MOT_RETREAT;
            }
        } else {
            player = gPlayerTbl[i];
            if (player->motNo != M570_MOT_TURN) {
                OMOBJ *robj;
                f32 shiftStart;

                robj = player->obj;
                if (player->motNo == 5 && M570_MOT_TURN == M570_MOT_WIN) {
                    shiftStart = kBlendLong;
                } else {
                    shiftStart = kZero;
                }
                CharMotionShiftSet(player->charNo, robj->mtnId[M570_MOT_TURN], shiftStart,
                                  MotParamTbl[M570_MOT_TURN].endTime, MotParamTbl[M570_MOT_TURN].attr);
                if (MotParamTbl[M570_MOT_TURN].loop != 0) {
                    Hu3DMotionAttrSet(robj->mtnId[M570_MOT_TURN], 1);
                } else {
                    Hu3DMotionAttrReset(robj->mtnId[M570_MOT_TURN], 1);
                }
                player->motNo = M570_MOT_TURN;
            }
        }
    }
}

s32 fn_1_5538(void)
{
    s16 i;

    for (i = 0; i < M570_CPU_MAX; i++) {
        if (gCpuPlayer[i]->gotItem == 0) {
            return 0;
        }
    }
    return 1;
}

static void fn_1_558C(OMOBJ *obj)
{
    HuVecF pos;

    switch (obj->work[0]) {
    case 1:
        obj->work[1]++;
        if (obj->work[1] == 20) {
            HuAudFXPlay(0x910);
        }
        break;
    case 2:
        obj->work[1]++;
        if (obj->work[1] == obj->work[2] * 15 + 25 && obj->work[2] < 5) {
            HuAudFXPlay(0x910);
            obj->work[2]++;
        }
        if (Hu3DMotionEndCheck(obj->mdlId[0]) && Hu3DMotionShiftIDGet(obj->mdlId[0]) < 0) {
            Hu3DModelObjPosGet(obj->mdlId[0], ItemHookName, &pos);
            CharEffectSmokeCreate(1, &pos);
            Hu3DModelAttrSet(obj->mdlId[1], 1);
            fn_1_5820(0);
        }
        break;
    }
}

void fn_1_56A0(void)
{
    OMOBJ *obj;
    s16 modelId;
    s16 i;

    obj = omAddObjEx(lbl_1_bss_108, 0x1100, 2, 4, OM_OBJ_NONE, fn_1_558C);
    modelId = Hu3DModelCreate(HuDataSelHeapReadNum(0x7D000E, HU_MEMNUM_OVL, HEAP_MODEL));
    obj->mdlId[0] = modelId;
    for (i = 0; i < M570_ITEM_MDL_MAX; i++) {
        obj->mtnId[i] = Hu3DJointMotion(modelId, HuDataSelHeapReadNum(ItemMotDataTbl[i], HU_MEMNUM_OVL, HEAP_MODEL));
    }
    Hu3DModelShadowSet(modelId);
    Hu3DModelAttrSet(modelId, HU3D_MOTATTR_LOOP);
    Hu3DMotionSet(modelId, obj->mtnId[0]);
    modelId = Hu3DModelCreate(HuDataSelHeapReadNum(0x7D0013, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelHookSet(obj->mdlId[0], ItemHookName, modelId);
    obj->mdlId[1] = modelId;
    Hu3DModelShadowSet(modelId);
    omSetTra(obj, kZero, kZero, kItemZ);
    gItemObj = obj;
}

static void fn_1_5820(s16 motNo)
{
    OMOBJ *obj;
    M570ITEMATTR attr = ItemShiftAttr;

    obj = gItemObj;
    obj->work[0] = motNo;
    obj->work[1] = 0;
    Hu3DModelAttrReset(obj->mdlId[0], HU3D_MOTATTR_LOOP);
    Hu3DMotionShiftSet(obj->mdlId[0], obj->mtnId[motNo], kZero, kTen, ((s32 *)&attr)[motNo]);
}

static void fn_1_58EC(OMOBJ *obj)
{
    M570DUST *dust = obj->data;
    s16 modelId = obj->mdlId[0];
    f32 endTime;

    switch (dust->state) {
    case 1:
        dust->wait--;
        if (dust->wait <= 0) {
            Hu3DMotionSet(modelId, obj->mtnId[1]);
            dust->state = 2;
        }
        break;
    case 2:
        endTime = Hu3DMotionMaxTimeGet(modelId) - kOne;
        if (Hu3DMotionTimeGet(modelId) == endTime) {
            dust->loop--;
            if (dust->loop == 0) {
                Hu3DMotionSet(modelId, obj->mtnId[0]);
                Hu3DMotionTimeSet(modelId, kZero);
                dust->state = 0;
            }
        }
        break;
    case 3:
        Hu3DModelAttrSet(modelId, HU3D_MOTATTR_LOOP);
        Hu3DMotionSet(modelId, obj->mtnId[0]);
        dust->state = 0;
        break;
    case 4:
        if (Hu3DMotionEndCheck(modelId)) {
            Hu3DModelAttrSet(modelId, HU3D_MOTATTR_LOOP);
            Hu3DMotionSet(modelId, obj->mtnId[0]);
            dust->state = 0;
        }
        break;
    }
}

void fn_1_5A78(void)
{
    OMOBJ *obj;
    M570DUST *dust;
    s16 modelId;
    s16 i;
    s16 j;
    HSFDATA *hsf;
    HSFMATERIAL *mat;

    for (i = 0; i < M570_DUST_MAX; i++) {
        obj = omAddObjEx(lbl_1_bss_108, i + 0x1200, 1, 2, OM_OBJ_NONE, fn_1_58EC);
        obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M570DUST), HU_MEMNUM_OVL);
        memset(obj->data, 0, sizeof(M570DUST));
        dust = obj->data;
        gDustTbl[i] = dust;
        dust->idx = i;
        dust->obj = obj;
        dust->state = 0;
        modelId = Hu3DModelCreate(HuDataSelHeapReadNum(0x7D0014, HU_MEMNUM_OVL, HEAP_MODEL));
        obj->mdlId[0] = modelId;
        for (j = 0; j < 2; j++) {
            obj->mtnId[j] =
                Hu3DJointMotion(modelId, HuDataSelHeapReadNum(DustMotDataTbl[j], HU_MEMNUM_OVL, HEAP_MODEL));
        }
        Hu3DMotionSet(modelId, obj->mtnId[1]);
        Hu3DModelAttrSet(modelId, HU3D_MOTATTR_LOOP);
        Hu3DMotionTimeSet(modelId, (s32)frandmod(((M570MAXTIMEGET)Hu3DMotionMaxTimeGet)(modelId)));
        omSetTra(obj, DustPosTbl[i].x, DustPosTbl[i].y, DustPosTbl[i].z);
        if (DustPosTbl[i].x < kMinX) {
            omSetRot(obj, kZero, kSideRot, kZero);
        } else if (DustPosTbl[i].x > kMaxX) {
            omSetRot(obj, kZero, kSideRotNeg, kZero);
        } else {
            omSetRot(obj, kZero, kZero, kZero);
        }
        hsf = Hu3DData[modelId].hsf;
        mat = hsf->material;
        for (j = 0; j < hsf->materialNum; j++) {
            mat->color[0] = 0xB2;
            mat->color[1] = 0xB2;
            mat->color[2] = 0xB2;
            mat++;
        }
    }
}

void fn_1_5D74(void)
{
    s16 i;

    for (i = 0; i < M570_DUST_MAX; i++) {
        gDustTbl[i]->state = 3;
    }
}

void fn_1_5DB8(void)
{
    s16 i;

    for (i = 0; i < M570_DUST_MAX; i++) {
        Hu3DModelAttrReset(gDustTbl[i]->obj->mdlId[0], HU3D_MOTATTR_LOOP);
        gDustTbl[i]->state = 4;
    }
}

void fn_1_5E38(void)
{
    s16 i;
    s16 modelId;

    for (i = 0; i < M570_DUST_MAX; i++) {
        gDustTbl[i]->state = 0;
        modelId = gDustTbl[i]->obj->mdlId[0];
        Hu3DMotionSet(modelId, gDustTbl[i]->obj->mtnId[0]);
        Hu3DModelAttrSet(modelId, HU3D_MOTATTR_LOOP);
        Hu3DMotionTimeSet(modelId, (s32)frandmod(((M570MAXTIMEGET)Hu3DMotionMaxTimeGet)(modelId)));
    }
}

void fn_1_5F24(s32 loop)
{
    s16 i;

    for (i = 0; i < M570_DUST_MAX; i++) {
        gDustTbl[i]->state = 1;
        gDustTbl[i]->wait = frandmod(0x3C);
        gDustTbl[i]->loop = loop;
    }
}