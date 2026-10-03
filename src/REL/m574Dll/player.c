#include "REL/m574Dll.h"

typedef struct M574PlayerWork_s {
    s32 no;
    u32 grp;
    s32 playerGrp;
    s32 charNo;
    s32 curMotNo;
    s32 unk14;
    u32 unk18;
    s32 unk1C;
    f32 f20;
    s32 type;
    s32 dif;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 counter;
    s32 unk44;
    Vec pos;
    Vec rot;
    s32 unk60;
    s32 unk64;
    s32 unk68;
    f32 angle;
    s32 unk70;
    s32 flag74;
} M574_PLAYER_WORK;

typedef struct M574MotShift_s {
    u16 motId;
    f32 endRateF;
    u32 attr;
} M574_MOT_SHIFT;

void fn_1_1068(OMOBJ *obj);
void fn_1_1264(OMOBJ *obj);
s32 fn_1_1E4C(OMOBJ *obj);
void fn_1_1D54(OMOBJ *obj, u32 motNo);

void fn_1_9FC(s32 no, s32 grp, s32 charNo);
void fn_1_6478(s32 grp, f32 scale);
f32 fn_1_6568(s32 grp);

static const u32 M574MotionDataNumTbl[5] = {
    0x8C0000, 0x8C0001, 0x8C004C, 0x8C0006, 0x8C0031
};

static const M574_MOT_SHIFT M574MotShiftTbl[5] = {
    { 0, 0.2f, 0x40000001 },
    { 1, 0.1f, 0x00010000 },
    { 2, 0.2f, 0x00020000 },
    { 3, 0.2f, 0x00040000 },
    { 4, 0.2f, 0x00000000 }
};

static const Vec M574PlayerPosTbl[4] = {
    { -1052.0f, 0.0f, 0.0f },
    { -220.0f, 0.0f, 0.0f },
    { 220.0f, 0.0f, 0.0f },
    { 1099.5f, 0.0f, 0.0f }
};

static const Vec M574PlayerPosTbl2[4] = {
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f }
};

static const f32 M574PlayerAngleTbl[4] = { 90.0f, 220.0f, 90.0f, 220.0f };

static const s32 M574PlayerPanTbl[4] = { 0x24, 0x3C, 0x44, 0x5C };

static OMOBJ *M574PlayerObjTbl[M574_PLAYER_MAX];

void fn_1_E98(OMOBJMAN *om)
{
    s32 cnt[2] = { 0, 0 };
    s32 playerIdx[2] = { 0, 2 };
    s32 grpTbl[M574_PLAYER_MAX];
    s32 i;
    s32 grp;
    OMOBJ *obj;
    BOOL grpF;

    grpF = FALSE;
    for (i = 0; i < M574_PLAYER_MAX; i++) {
        grp = GwPlayerConf[i].grp;
        grpTbl[i] = grp;
        if (grp >= 2) {
            grpF = TRUE;
        }
        else {
            cnt[grp]++;
        }
    }

    if (cnt[0] != 2 || cnt[1] != 2) {
        grpF = TRUE;
    }

    if (grpF) {
        for (i = 0; i < M574_PLAYER_MAX; i++) {
            grpTbl[i] = i >> 1;
        }
    }

    omMakeGroupEx(om, 0, M574_PLAYER_MAX);

    for (i = 0; i < M574_PLAYER_MAX; i++) {
        obj = (M574PlayerObjTbl[i] = omAddObjEx(om, 0x28, 1, 5, 0, fn_1_1068));
        obj->work[0] = i;
        grp = grpTbl[i];
        obj->work[1] = grp;
        obj->work[2] = playerIdx[grp]++;
        fn_1_9FC(i, grp, GwPlayerConf[i].charNo);
    }
}

void fn_1_1044(void)
{
    CharModelKill(-1);
}

void fn_1_1068(OMOBJ *obj)
{
    M574_PLAYER_WORK *player;
    s32 i;

    obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M574_PLAYER_WORK), HU_MEMNUM_OVL);
    player = obj->data;
    memset(player, 0, sizeof(M574_PLAYER_WORK));

    obj->stat |= OM_STAT_MODELPAUSE;

    player->no = obj->work[0];
    player->grp = obj->work[1];
    player->playerGrp = obj->work[2];
    player->charNo = GwPlayerConf[player->no].charNo;
    player->type = GwPlayerConf[player->no].type;
    player->dif = GwPlayerConf[player->no].dif & 3;
    player->unk2C = 0;

    obj->mdlId[0] = CharModelCreate(player->charNo, 2);
    Hu3DModelLayerSet(obj->mdlId[0], 6);
    Hu3DModelShadowSet(obj->mdlId[0]);
    CharEffectLayerSet(6);

    for (i = 0; i < 5; i++) {
        obj->mtnId[i] = CharMotionCreate(player->charNo, M574MotionDataNumTbl[i]);
    }

    CharMotionDataClose(player->charNo);

    player->curMotNo = -1;
    fn_1_1D54(obj, 2);

    player->pos.x = M574PlayerPosTbl[player->playerGrp].x;
    player->pos.y = M574PlayerPosTbl[player->playerGrp].y;
    player->pos.z = M574PlayerPosTbl[player->playerGrp].z;
    player->rot.z = 0.0f;
    player->rot.y = 0.0f;
    player->rot.x = 0.0f;
    Hu3DModelScaleSet(obj->mdlId[0], 1.2f, 1.2f, 1.2f);
    player->angle = M574PlayerAngleTbl[player->playerGrp];
    player->unk38 = 0;
    obj->objFunc = fn_1_1264;
}

void fn_1_17A0(OMOBJ *obj)
{
    M574_PLAYER_WORK *player;

    player = obj->data;
    player->counter++;
    CharModelVoicePanSet(player->charNo, M574PlayerPanTbl[player->playerGrp]);

    if (player->flag74 && fn_1_1E4C(obj)) {
        player->flag74 = 0;
    }
}

void fn_1_182C(void)
{
}

void fn_1_1830(OMOBJ *obj)
{
    M574_PLAYER_WORK *player;
    f64 rate;

    player = obj->data;
    if (player->unk18 & 0x100) {
        fn_1_6478(player->playerGrp, 1.0f);
    }
    else if (player->f20 > 0.0) {
        fn_1_6478(player->playerGrp, player->f20);
    }
    else {
        fn_1_6478(player->playerGrp, 0.0f);
    }

    if (player->curMotNo == 2 && CharMotionShiftIDGet(player->charNo) < 0) {
        rate = fn_1_6568(player->playerGrp);
        CharMotionTimeSet(player->charNo, (f32) (30.0 * (1.0 - rate)));
    }
}

void fn_1_1938(OMOBJ *obj)
{
    M574_PLAYER_WORK *player;
    HU3DMODELID model;

    player = obj->data;
    model = obj->mdlId[0];
    Hu3DModelPosSetV(model, &player->pos);
    Hu3DModelRotSet(model, 0.0f, player->angle, 0.0f);
}

OMOBJ *fn_1_1CD4(OMOBJ *obj)
{
    M574_PLAYER_WORK *player;
    OMOBJ *target;
    s32 i;
    u32 grp;

    player = obj->data;
    grp = player->grp;
    for (i = 0; i < M574_PLAYER_MAX; i++) {
        target = M574PlayerObjTbl[i];
        if (target != obj) {
            player = target->data;
            if (player->grp == grp) {
                return target;
            }
        }
    }
    return NULL;
}

void fn_1_1D54(OMOBJ *obj, u32 motNo)
{
    M574_PLAYER_WORK *player;
    f32 endF;

    player = obj->data;
    if (player->curMotNo != motNo && motNo < 5) {
        if (player->curMotNo < 0) {
            endF = 0.0f;
        }
        else {
            endF = 60.0f * M574MotShiftTbl[motNo].endRateF;
        }

        player->curMotNo = motNo;
        CharMotionShiftSet(player->charNo, obj->mtnId[M574MotShiftTbl[motNo].motId],
            0.0f, endF, M574MotShiftTbl[motNo].attr);
    }
}

s32 fn_1_1E4C(OMOBJ *obj)
{
    M574_PLAYER_WORK *player;

    player = obj->data;
    if (CharMotionEndCheck(player->charNo)) {
        if (CharMotionShiftIDGet(player->charNo) < 0) {
            return 1;
        }
    }
    return 0;
}

s32 fn_1_1EB8(OMOBJ *obj)
{
    M574_PLAYER_WORK *player;

    player = obj->data;
    return player->no;
}

s32 fn_1_1ED4(OMOBJ *obj, Vec *pos, f32 *angle)
{
    M574_PLAYER_WORK *player;

    player = obj->data;
    if (pos != NULL) {
        *pos = player->pos;
    }
    if (angle != NULL) {
        *angle = player->angle;
    }
    return 1;
}

s32 fn_1_1F28(u32 grp)
{
    M574_PLAYER_WORK *player;
    s32 i;
    OMOBJ *obj;

    for (i = 0; i < M574_PLAYER_MAX; i++) {
        obj = M574PlayerObjTbl[i];
        player = obj->data;
        if (player->grp == grp && player->type == 0) {
            return 1;
        }
    }
    return 0;
}

void fn_1_1F9C(u32 grp)
{
    M574_PLAYER_WORK *player;
    s32 i;
    OMOBJ *obj;

    for (i = 0; i < M574_PLAYER_MAX; i++) {
        obj = M574PlayerObjTbl[i];
        player = obj->data;
        if (player->grp == grp) {
            omVibrate(player->no, 15, 6, 6);
        }
    }
}