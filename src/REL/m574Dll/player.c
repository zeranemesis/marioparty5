#include "REL/m574Dll.h"
#include "game/frand.h"
#include "game/mg/seqman.h"

typedef struct M574PlayerWork_s {
    s32 no;
    u32 grp;
    s32 playerGrp;
    s32 charNo;
    s32 curMotNo;
    s32 unk14;
    u32 unk18;
    f32 unk1C;
    f32 f20;
    s32 type;
    s32 dif;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    u32 mode;
    s32 timer;
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

typedef struct M574WalkEntry_s {
    f32 len;
    s32 v1;
    s32 v2;
    s32 v3;
    s32 v4;
    f32 rate;
} M574_WALK_ENTRY;

typedef struct M574WalkTbl_s {
    M574_WALK_ENTRY ent[M574_PLAYER_MAX];
    s32 pad;
} M574_WALK_TBL;

void fn_1_1068(OMOBJ *obj);
void fn_1_1264(OMOBJ *obj);
void fn_1_1588(OMOBJ *obj);
void fn_1_17A0(OMOBJ *obj);
void fn_1_182C(OMOBJ *obj);
void fn_1_1830(OMOBJ *obj);
void fn_1_1938(OMOBJ *obj);
void fn_1_19AC(OMOBJ *obj);
s32 fn_1_1E4C(OMOBJ *obj);
void fn_1_1D54(OMOBJ *obj, u32 motNo);

void fn_1_9FC(s32 no, s32 grp, s32 charNo);
s32 fn_1_928(s32 no);
void fn_1_AAC(s32 no);
f32 fn_1_CC8(f32 cur, f32 target, f32 speed);
s32 fn_1_6D7C(s32 grp, const Vec *pos, f32 ofs, f32 speed, f32 len);
void fn_1_70B0(s32 handle, s32 idx, Vec *out);
void fn_1_6478(s32 grp, f32 scale);
f32 fn_1_6568(s32 grp);

static u32 M574MotionDataNumTbl[5] = {
    0x8C0000, 0x8C0001, 0x8C004C, 0x8C0006, 0x8C0031
};

static M574_MOT_SHIFT M574MotShiftTbl[5] = {
    { 0, 0.2f, 0x40000001 },
    { 1, 0.1f, 0x40000001 },
    { 2, 0.2f, 0 },
    { 3, 0.2f, 0 },
    { 4, 0.2f, 0x00000000 }
};

static Vec M574PlayerPosTbl[4] = {
    { -1100.0f, 0.0f, 0.0f },
    { -220.0f, 0.0f, 0.0f },
    { 220.0f, 0.0f, 0.0f },
    { 1100.0f, 0.0f, 0.0f }
};

static Vec M574PlayerPosTbl2[4] = {
    { -900.0f, 50.0f, 0.0f },
    { -420.0f, 50.0f, 0.0f },
    { 420.0f, 50.0f, 0.0f },
    { 900.0f, 50.0f, 0.0f }
};

static f32 M574PlayerAngleTbl[4] = { 90.0f, 270.0f, 90.0f, 270.0f };

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
    HU3DMODELID model;
    s32 no;
    s32 charNo;
    s32 playerGrp;
    HU3DMOTID mtnId;

    obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M574_PLAYER_WORK), HU_MEMNUM_OVL);
    player = obj->data;
    memset(player, 0, sizeof(M574_PLAYER_WORK));

    obj->stat |= OM_STAT_MODELPAUSE;

    no = obj->work[0];
    player->no = no;
    player->grp = obj->work[1];
    playerGrp = obj->work[2];
    player->playerGrp = playerGrp;
    charNo = GwPlayerConf[no].charNo;
    player->charNo = charNo;
    player->dif = GwPlayerConf[no].dif & 3;
    player->type = GwPlayerConf[no].type;
    player->unk2C = 0;

    model = CharModelCreate(charNo, 2);
    obj->mdlId[0] = model;
    Hu3DModelLayerSet(model, 6);
    Hu3DModelShadowSet(model);
    CharEffectLayerSet(6);

    for (i = 0; i < 5; i++) {
        mtnId = CharMotionCreate(charNo, M574MotionDataNumTbl[i]);
        obj->mtnId[i] = mtnId;
    }

    CharMotionDataClose(charNo);

    player->curMotNo = -1;
    fn_1_1D54(obj, 2);

    player->pos = M574PlayerPosTbl[playerGrp];
    player->rot.x = player->rot.y = player->rot.z = 0.0f;
    Hu3DModelScaleSet(model, 1.2f, 1.2f, 1.2f);
    player->angle = M574PlayerAngleTbl[playerGrp];
    player->mode = 0;
    obj->objFunc = fn_1_1264;
}

void fn_1_1264(OMOBJ *obj)
{
    M574_PLAYER_WORK *player;
    s32 padNo;
    f32 f8;
    f32 fc;

    player = obj->data;
    player->unk14 = 0;
    player->unk18 = 0;
    player->unk1C = 0.0f;
    player->f20 = 0.0f;

    switch (player->mode) {
    case 0:
        player->mode++;
        player->unk44 = 0;
    case 1:
        if ((f64) (player->unk44 += 1) < 144.0) {
            break;
        }
        player->mode++;
        player->unk44 = 0;
        player->timer = 2;
    case 2:
        player->unk18 = 0x100;
        if ((f64) (player->unk44 += 1) < 12.0) {
            break;
        }
        player->mode++;
        player->unk44 = 0;
    case 3:
        if ((f64) (player->unk44 += 1) < 12.0) {
            break;
        }
        if (--player->timer > 0) {
            player->mode--;
            player->unk44 = 0;
            break;
        }
        player->mode++;
    case 4:
        if (MgSeqModeGet() != 5) {
            break;
        }
        player->mode++;
    case 5:
        if (MgSeqModeGet() != 5) {
            player->mode++;
            player->timer = 0;
            player->unk70 = 1;
            break;
        }
        if (player->type != 0) {
            fn_1_19AC(obj);
            break;
        }
        padNo = GwPlayerConf[player->no].padNo;
        f8 = 0.0f;
        fc = 0.0f;
        player->unk18 = HuPadBtn[padNo];
        player->unk14 = HuPadBtnDown[padNo];
        break;
    case 6:
        if (++player->timer < 10) {
            break;
        }
        player->mode++;
        fn_1_1D54(obj, 0);
    case 7:
        if (MgSeqModeGet() != 7) {
            break;
        }
        player->mode = 0;
        obj->objFunc = fn_1_1588;
        break;
    }

    fn_1_1830(obj);
    fn_1_182C(obj);
    PSVECAdd(&player->pos, &player->rot, &player->pos);
    fn_1_17A0(obj);
    fn_1_1938(obj);
}

void fn_1_1588(OMOBJ *obj)
{
    M574_PLAYER_WORK *player;

    player = obj->data;
    player->unk14 = 0;
    player->unk18 = 0;
    player->unk1C = 0.0f;
    player->f20 = 0.0f;
    player->rot.x = player->rot.y = player->rot.z = 0.0f;

    switch (player->mode) {
    case 0:
        player->unk70 = 0;
        player->flag74 = 0;
        player->mode++;
    case 1:
        fn_1_182C(obj);
        if (MgSeqModeGet() == 7) {
            break;
        }
        player->mode++;
        player->timer = 0;
        fn_1_1D54(obj, 1);
        break;
    case 2:
        fn_1_182C(obj);
        player->angle = fn_1_CC8(player->angle, 0.0f, 0.05f);
        if (++player->timer < 20) {
            break;
        }
        player->mode++;
        player->timer = 0;
        fn_1_1D54(obj, 0);
        break;
    case 3:
        fn_1_182C(obj);
        player->angle = fn_1_CC8(player->angle, 0.0f, 0.05f);
        if (++player->timer < 20) {
            break;
        }
        if (fn_1_928(player->no) >= 0) {
            fn_1_1D54(obj, 3);
        }
        else {
            fn_1_1D54(obj, 4);
            fn_1_AAC(player->no);
        }
        player->mode++;
        break;
    case 4:
        break;
    case 5:
        if (fn_1_1E4C(obj)) {
            player->mode++;
        }
        break;
    case 6:
        break;
    }

    PSVECAdd(&player->pos, &player->rot, &player->pos);
    fn_1_17A0(obj);
    fn_1_1938(obj);
}

static s32 M574PlayerPanTbl[4] = { 0x24, 0x3C, 0x44, 0x5C };

static M574_WALK_TBL M574WalkTblData = {
    {
        { 400.0f, 18, 16, 9, 9, 0.7f },
        { 350.0f, 19, 15, 8, 8, 0.75f },
        { 320.0f, 21, 14, 7, 7, 0.8f },
        { 300.0f, 24, 13, 6, 6, 0.85f }
    },
    0
};

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

void fn_1_182C(OMOBJ *obj)
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

// NONMATCHING: the original's (f32) frand conversion keeps the s16 xoris but emits no extsh after the 16-bit mask
void fn_1_19AC(OMOBJ *obj)
{
    M574_PLAYER_WORK *player;
    M574_WALK_ENTRY *entry;
    s32 ret;
    Vec *target;
    Vec pos;

    player = obj->data;
    entry = (M574_WALK_ENTRY *) &M574WalkTblData.ent[player->dif];
    target = (Vec *) &M574PlayerPosTbl2[player->playerGrp];

    switch (player->unk2C) {
    case 0:
        if (0.0000152587890625 * (f32) ((s16) (u16) frand()) > entry->rate) {
            break;
        }
        ret = fn_1_6D7C(player->grp, target, 0.0f, 120.0f, entry->len);
        if (ret < 0) {
            break;
        }
        fn_1_70B0(ret, 0, &pos);
        if (PSVECMag(&pos) > 33.33333507180214) {
            player->unk34 = 1;
        }
        else {
            player->unk34 = 0;
        }
        player->unk2C++;
        if (player->unk34) {
            player->unk30 = entry->v3;
        }
        else {
            player->unk30 = entry->v1;
        }
if (0.0000152587890625 * (f32) ((s16) (u16) frand()) > entry->rate) {
            player->unk30 += (s32) (12.0 * (0.0000152587890625 * (f32) ((s16) (u16) frand())));
        }
    case 1:
        player->unk18 = 0x100;
        if (--player->unk30 > 0) {
            break;
        }
        player->unk2C++;
        if (player->unk34) {
            player->unk30 = entry->v4;
        }
        else {
            player->unk30 = entry->v2;
        }
if (0.0000152587890625 * (f32) ((s16) (u16) frand()) > entry->rate) {
            player->unk30 += (s32) (15.0 * (0.0000152587890625 * (f32) ((s16) (u16) frand())));
        }
    case 2:
        if (--player->unk30 > 0) {
            break;
        }
        player->unk2C = 0;
        break;
    }
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