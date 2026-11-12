#include "REL/m569Dll.h"

#include "game/audio.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/frand.h"
#include "game/gamework.h"
#include "game/hsfformat.h"
#include "game/memory.h"
#include "game/sprite.h"

#include "dolphin/mtx.h"

#include "math.h"

#define M569_RAD(x) (3.141592653589793 * (f64)(x) / 180.0)

/* Score display (0x14 bytes), OMOBJ::data of the score objects. */
typedef struct m569Score_s {
    /* 0x00 */ MGSCORE *score;
    /* 0x04 */ s32 idx;
    /* 0x08 */ s32 value;
    /* 0x0C */ s32 state;
    /* 0x10 */ s32 timer;
} M569SCORE;

/* Per-player coin banner (0x1C bytes). */
typedef struct m569Banner_s {
    /* 0x00 */ s16 mdl;
    /* 0x02 */ s16 parId;
    /* 0x04 */ HuVecF pos;
    /* 0x10 */ s32 state;
    /* 0x14 */ s32 coin;
    /* 0x18 */ s16 player;
} M569BANNER;

/* Falling coin (0x44 bytes). */
typedef struct m569Coin_s {
    /* 0x00 */ s32 active;
    /* 0x04 */ s32 shown;
    /* 0x08 */ s32 hold;
    /* 0x0C */ HuVecF pos;
    /* 0x18 */ HuVecF vel;
    /* 0x24 */ HuVecF rot;
    /* 0x30 */ HuVecF rotVel;
    /* 0x3C */ f32 groundY;
    /* 0x40 */ s32 delay;
} M569COIN;

/* Confetti piece (0x3C bytes). */
typedef struct m569Confetti_s {
    /* 0x00 */ s32 active;
    /* 0x04 */ s32 landed;
    /* 0x08 */ HuVecF pos;
    /* 0x14 */ HuVecF vel;
    /* 0x20 */ HuVecF rot;
    /* 0x2C */ HuVecF rotVel;
    /* 0x38 */ GXColor color;
} M569CONFETTI;

/* Confetti model state (0xC bytes). */
typedef struct m569ConfettiMdl_s {
    /* 0x00 */ s16 mdl;
    /* 0x02 */ s16 hook;
    /* 0x04 */ f32 floorY;
    /* 0x08 */ s32 count;
} M569CONFETTIMDL;

/* ---------------------------------------------------------------- data */

static HuVecF lbl_1_data_3C0[4] = {
    { -300.0f, 350.0f, -50.0f },
    { -100.0f, 350.0f, -50.0f },
    { 100.0f, 350.0f, -50.0f },
    { 300.0f, 350.0f, -50.0f }
};
static GXColor lbl_1_data_3F0[4] = {
    { 0xFF, 0xC0, 0xCC, 0xFF },
    { 0xFF, 0xFF, 0x99, 0xFF },
    { 0xB2, 0xFF, 0xFF, 0xFF },
    { 0x99, 0xFF, 0xC0, 0xFF }
};
static HU3DPARMANPARAM lbl_1_data_400 = {
    0x32,
    0,
    20.0f,
    100.0f,
    0.0f,
    { 0.0f, 0.0f, 0.0f },
    1.0f,
    0.0f,
    100.0f,
    0.97f,
    1,
    { { 0xFF, 0xFF, 0xFF, 0xFF }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
    { { 0xFF, 0xFF, 0xFF, 0x00 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }
};
static char lbl_1_data_450[] = "m569_ef2";
static char lbl_1_data_459[] = "coin-coin_1";

s32 lbl_1_bss_168;
static s32 lbl_1_bss_16C;
static s16 lbl_1_bss_170;
static s32 lbl_1_bss_174;
static s16 lbl_1_bss_178;
static M569COIN lbl_1_bss_17C[0x90];
static M569CONFETTIMDL lbl_1_bss_27BC;
static M569CONFETTI lbl_1_bss_27C8[0xC8];
static M569SCORE *lbl_1_bss_56A8[2];
static M569BANNER *lbl_1_bss_56B0[4];

/* ------------------------------------------------------------- functions */

static void fn_1_8978(OMOBJ *obj);
static void fn_1_8D78(OMOBJ *obj);
static void fn_1_8F14(OMOBJ *obj);
static s16 fn_1_8F38(HuVecF *pos);
static void fn_1_91A0(void);
static void fn_1_9290(void);
static void fn_1_9384(HuVecF *origin, s32 num);
static void fn_1_9A20(void);
static void fn_1_9C38(HuVecF *origin, M569PLAYER *player);

void fn_1_87D4(void)
{
    HuVecF screen;
    MGSCORE *score;
    OMOBJ *obj;
    s32 i;

    for (i = 0; i < 2; i++) {
        score = MgScoreCreate(0x960034, -1, 1);
        MgScoreMaxDigitSet(score, 2);
        Hu3D3Dto2D(&lbl_1_data_D8[i + 2], (s16)lbl_1_bss_34.camNo, &screen);
        MgScorePosSet(score, screen.x - 15.0f, screen.y);
        MgScoreColorSet(score, 0xFF, 0xFF, 0xFF);
        HuSprGrpDrawNoSet(score->grpId, 0x40);
        obj = omAddObjEx(lbl_1_bss_D4, 0x2000, 0, 0, -1, fn_1_8978);
        obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M569SCORE), HU_MEMNUM_OVL);
        lbl_1_bss_56A8[i] = (M569SCORE *)obj->data;
        lbl_1_bss_56A8[i]->idx = i;
        lbl_1_bss_56A8[i]->score = score;
        lbl_1_bss_56A8[i]->value = 0;
        lbl_1_bss_56A8[i]->state = 0;
    }
    HuSprExecLayerSet(0x40, 2);
}

static void fn_1_8978(OMOBJ *obj)
{
    M569SCORE *disp;

    disp = (M569SCORE *)obj->data;
    switch (disp->state) {
    case 1:
        MgScoreDispOn(disp->score);
        disp->state = 2;
        disp->timer = 0x5A;
        lbl_1_bss_16C = HuAudFXPlay(0x1D);
        break;
    case 2:
        MgScoreValueSet(disp->score, frandmod(100));
        disp->timer--;
        if (disp->timer == 0) {
            MgScoreValueSet(disp->score, disp->value);
            disp->state = 3;
            HuAudFXStop(lbl_1_bss_16C);
            if (disp->idx == 0) {
                HuAudFXPlay(0x90D);
            } else {
                HuAudFXPlay(0x90E);
            }
        }
        break;
    case 0:
        break;
    case 3:
        break;
    }
}

void fn_1_8A74(s32 idx, s32 value)
{
    lbl_1_bss_56A8[idx]->value = value;
    lbl_1_bss_56A8[idx]->state = 1;
}

s32 fn_1_8AAC(s32 idx)
{
    if (lbl_1_bss_56A8[idx]->state == 3) {
        return 1;
    }
    return 0;
}

void fn_1_8ADC(void)
{
    OMOBJ *obj;
    s16 mdl;
    s32 i;
    M569PLAYER *player;
    s16 id;
    s16 coin;

    for (i = 0; i < 4; i++) {
        obj = omAddObjEx(lbl_1_bss_D4, 0x2000, 1, 0, -1, fn_1_8D78);
        obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M569BANNER), HU_MEMNUM_OVL);
        lbl_1_bss_56B0[i] = (M569BANNER *)obj->data;
        mdl = Hu3DModelCreate(HuDataSelHeapReadNum(0x7C0012, HU_MEMNUM_OVL, HEAP_MODEL));
        obj->mdlId[0] = mdl;
        player = (M569PLAYER *)lbl_1_bss_F8[i]->data;
        lbl_1_bss_56B0[i]->pos = lbl_1_data_3C0[player->slot];
        lbl_1_bss_56B0[i]->pos.y = 700.0f;
        omSetTra(obj, lbl_1_bss_56B0[i]->pos.x, lbl_1_bss_56B0[i]->pos.y, lbl_1_bss_56B0[i]->pos.z);
        Hu3DModelPosSetV(mdl, &lbl_1_bss_56B0[i]->pos);
        Hu3DModelAttrSet(mdl, 0x40000002);
        Hu3DModelLayerSet(mdl, 2);
        lbl_1_bss_56B0[i]->player = player->objId;
        lbl_1_bss_56B0[i]->mdl = mdl;
        lbl_1_bss_56B0[i]->parId = -1;
        lbl_1_bss_56B0[i]->state = 0;
        lbl_1_bss_56B0[i]->coin = player->coin;
        id = player->objId;
        coin = player->coin;
        GwPlayer[id].mgCoin = coin;
    }
    fn_1_9290();
    obj = omAddObjEx(lbl_1_bss_D4, 0x2000, 0, 0, -1, fn_1_8F14);
}

static void fn_1_8D78(OMOBJ *obj)
{
    M569BANNER *banner;
    M569PLAYER *player;

    banner = (M569BANNER *)obj->data;
    switch (banner->state) {
    case 0:
        obj->work[0] = 0;
        banner->pos.y -= 8.0f;
        if (banner->pos.y <= 350.0f) {
            banner->pos.y = 350.0f;
            banner->state = 1;
            HuAudFXPlay(0x90F);
        }
        break;
    case 1:
        if (obj->work[0]++ == 0xA) {
            Hu3DModelAttrReset(obj->mdlId[0], 0x40000002);
            if (banner->parId == -1) {
                banner->parId = fn_1_8F38(&banner->pos);
            }
            if (banner->coin != 0) {
                fn_1_9384(&banner->pos, 0x32);
                player = (M569PLAYER *)lbl_1_bss_F8[banner->player]->data;
                fn_1_9C38(&banner->pos, player);
            }
            banner->state = 2;
            obj->work[0] = 0;
        }
        /* fallthrough */
    case 2:
        obj->work[0] += 1;
        if (obj->work[0] == 0x32 && banner->parId != -1) {
            Hu3DParManKill(banner->parId);
            banner->parId = -1;
        }
        break;
    case 3:
        break;
    }
    omSetTra(obj, banner->pos.x, banner->pos.y, banner->pos.z);
}

static void fn_1_8F14(OMOBJ *obj)
{
    fn_1_91A0();
    fn_1_9A20();
}

static s16 fn_1_8F38(HuVecF *pos)
{
    HU3DPARMANID parId;
    ANIMDATA *anim;
    void *data;

    data = HuDataSelHeapReadNum(0x210002, HU_MEMNUM_OVL, HEAP_MODEL);
    anim = HuSprAnimRead(data);
    parId = Hu3DParManCreate(anim, 0x14, &lbl_1_data_400);
    Hu3DParManRotSet(parId, 0.0f, 0.0f, 0.0f);
    Hu3DParManPosSet(parId, pos->x, pos->y, pos->z);
    Hu3DModelLayerSet(Hu3DParManModelIDGet(parId), 5);
    Hu3DParManTimeLimitSet(parId, 0x32);
    return parId;
}

void fn_1_9014(HU3DMODEL *model, Mtx *mtx)
{
    Mtx rotMtx;
    Mtx mat;
    HSFDATA *hsf;
    HSFMATERIAL *material;
    s32 i;
    s32 j;
    M569CONFETTI *confetti;

    Hu3DModelObjDrawInit();
    hsf = Hu3DData[lbl_1_bss_27BC.mdl].hsf;
    for (i = 0; i < 0xC8; i++) {
        confetti = &lbl_1_bss_27C8[i];
        if (confetti->active != 0) {
            PSMTXRotRad(rotMtx, 'z', 0.017453292f * confetti->rot.z);
            PSMTXRotRad(mat, 'x', 0.017453292f * confetti->rot.x);
            PSMTXConcat(rotMtx, mat, mat);
            PSMTXRotRad(rotMtx, 'y', 0.017453292f * confetti->rot.y);
            PSMTXConcat(rotMtx, mat, mat);
            PSMTXTrans(rotMtx, confetti->pos.x, confetti->pos.y, confetti->pos.z);
            PSMTXConcat(rotMtx, mat, mat);
            PSMTXConcat(*mtx, mat, mat);
            Hu3DModelObjDraw(lbl_1_bss_27BC.mdl, lbl_1_data_450, mat);
            material = hsf->material;
            for (j = 0; j < hsf->materialNum; j++) {
                material->color[0] = confetti->color.r;
                material->color[1] = confetti->color.g;
                material->color[2] = confetti->color.b;
                material++;
            }
        }
    }
}

static void fn_1_91A0(void)
{
    s32 i;
    M569CONFETTI *confetti;

    if (lbl_1_bss_27BC.count != 0) {
        for (i = 0; i < 0xC8; i++) {
            confetti = &lbl_1_bss_27C8[i];
            if (confetti->active != 0 && confetti->landed == 0) {
                PSVECAdd(&confetti->pos, &confetti->vel, &confetti->pos);
                PSVECAdd(&confetti->rot, &confetti->rotVel, &confetti->rot);
                if (confetti->pos.y <= lbl_1_bss_27BC.floorY) {
                    confetti->pos.y = lbl_1_bss_27BC.floorY;
                    confetti->rot.x = -90.0f;
                    confetti->rot.z = 0.0f;
                    confetti->landed = 1;
                }
            }
        }
    }
}

static void fn_1_9290(void)
{
    s32 i;

    lbl_1_bss_27BC.mdl = Hu3DModelCreate(HuDataSelHeapReadNum(0x7C0013, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelAttrSet(lbl_1_bss_27BC.mdl, 1);
    lbl_1_bss_27BC.hook = Hu3DHookFuncCreate(fn_1_9014);
    Hu3DModelLayerSet(lbl_1_bss_27BC.hook, 4);
    lbl_1_bss_27BC.floorY = 0.1f;
    lbl_1_bss_27BC.count = 0;
    for (i = 0; i < 0xC8; i++) {
        lbl_1_bss_27C8[i].active = 0;
        lbl_1_bss_27C8[i].landed = 0;
    }
}

static void fn_1_9384(HuVecF *origin, s32 num)
{
    s32 spawned;
    s32 i;
    M569CONFETTI *confetti;
    f32 yaw;
    f32 dist;
    f32 pitch;

    spawned = 0;
    for (i = 0; i < 0xC8; i++) {
        if (spawned == num) {
            break;
        }
        confetti = &lbl_1_bss_27C8[i];
        if (confetti->active == 0) {
            confetti->active = 1;
            spawned++;
            lbl_1_bss_27BC.count++;
            dist = (f32)(s32)frandmod(0x12C0) / 100.0f;
            yaw = (f32)(s32)frandmod(0x168);
            pitch = (f32)(s32)frandmod(0x168);
            confetti->pos.x = origin->x + dist * (sin(M569_RAD(pitch)) * cos(M569_RAD(yaw)));
            confetti->pos.y = origin->y + dist * -sin(M569_RAD(yaw));
            confetti->pos.z = origin->z + dist * (cos(M569_RAD(pitch)) * cos(M569_RAD(yaw)));
            confetti->vel.x = 0.8f * ((f32)((s32)frandmod(0xC9) - 0x64) / 100.0f);
            confetti->vel.y = -6.5f * ((f32)(s32)frandmod(0x64) / 100.0f);
            if (confetti->vel.y > -2.6000001f) {
                confetti->vel.y = -2.6000001f;
            }
            confetti->vel.z = 0.5f * ((f32)((s32)frandmod(0xC9) - 0x64) / 100.0f);
            confetti->rot.x = confetti->rot.y = confetti->rot.z = 0.0f;
            confetti->rotVel.x = 14.0f * ((f32)((s32)frandmod(0xC9) - 0x64) / 100.0f);
            confetti->rotVel.y = 14.0f * ((f32)((s32)frandmod(0xC9) - 0x64) / 100.0f);
            confetti->rotVel.z = 14.0f * ((f32)((s32)frandmod(0xC9) - 0x64) / 100.0f);
            confetti->color = lbl_1_data_3F0[frandmod(4)];
        }
    }
}

void fn_1_986C(HU3DMODEL *model, Mtx *mtx)
{
    Mtx rotMtx;
    Mtx mat;
    HSFDATA *hsf;
    s32 i;
    M569COIN *coin;

    Hu3DModelObjDrawInit();
    hsf = Hu3DData[lbl_1_bss_178].hsf;
    for (i = 0; i < 0x90; i++) {
        coin = &lbl_1_bss_17C[i];
        if (coin->active != 0 && (coin->delay == 0 || coin->shown != 0)) {
            PSMTXRotRad(rotMtx, 'z', 0.017453292f * coin->rot.z);
            PSMTXRotRad(mat, 'x', 0.017453292f * coin->rot.x);
            PSMTXConcat(rotMtx, mat, mat);
            PSMTXRotRad(rotMtx, 'y', 0.017453292f * coin->rot.y);
            PSMTXConcat(rotMtx, mat, mat);
            PSMTXScale(rotMtx, 0.5f, 0.5f, 0.5f);
            PSMTXConcat(rotMtx, mat, mat);
            PSMTXTrans(rotMtx, coin->pos.x, coin->pos.y, coin->pos.z);
            PSMTXConcat(rotMtx, mat, mat);
            PSMTXConcat(*mtx, mat, mat);
            Hu3DModelObjDraw(lbl_1_bss_178, lbl_1_data_459, mat);
        }
    }
}

static void fn_1_9A20(void)
{
    s32 i;
    M569COIN *coin;

    if (lbl_1_bss_174 != 0) {
        for (i = 0; i < 0x90; i++) {
            coin = &lbl_1_bss_17C[i];
            if (coin->active != 0 && coin->hold == 0) {
                if (coin->delay == 0) {
                    coin->vel.y -= 1.96f;
                    PSVECAdd(&coin->pos, &coin->vel, &coin->pos);
                    PSVECAdd(&coin->rot, &coin->rotVel, &coin->rot);
                    if (coin->pos.y <= coin->groundY) {
                        HuAudFXPlay(7);
                        CharEffectCoinGlowCreate((s16)lbl_1_bss_34.camNo, &coin->pos);
                        coin->active = 0;
                    }
                } else {
                    coin->delay--;
                }
            }
        }
    }
}

void fn_1_9B2C(void)
{
    s32 i;

    lbl_1_bss_178 = Hu3DModelCreate(HuDataSelHeapReadNum(0x990000, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelAttrSet(lbl_1_bss_178, 1);
    Hu3DModelScaleSet(lbl_1_bss_178, 0.5f, 0.5f, 0.5f);
    lbl_1_bss_170 = Hu3DHookFuncCreate(fn_1_986C);
    Hu3DModelLayerSet(lbl_1_bss_170, 4);
    lbl_1_bss_174 = 0;
    for (i = 0; i < 0x90; i++) {
        lbl_1_bss_17C[i].active = 0;
        lbl_1_bss_17C[i].hold = 0;
    }
}

static void fn_1_9C38(HuVecF *origin, M569PLAYER *player)
{
    M569COIN *coin;
    s32 spawned;
    s32 i;
    f32 height;

    height = CharModelHeightGet(player->charNo);
    spawned = 0;
    for (i = 0; i < 0x90; i++) {
        if (spawned == player->coin) {
            break;
        }
        coin = &lbl_1_bss_17C[i];
        if (coin->active == 0) {
            coin->active = 1;
            coin->delay = spawned * 4 + 0xF;
            if (spawned == 0) {
                coin->shown = 1;
            } else {
                coin->shown = 0;
            }
            spawned++;
            lbl_1_bss_174++;
            coin->pos = *origin;
            coin->vel.x = 0.0f;
            coin->vel.y = 0.0f;
            coin->vel.z = 0.0f;
            coin->rot.x = coin->rot.y = coin->rot.z = 0.0f;
            coin->rot.y = (f32)(s32)frandmod(0x168);
            coin->rotVel.x = coin->rotVel.y = coin->rotVel.z = 0.0f;
            coin->rotVel.y = 15.0f;
            coin->groundY = height;
        }
    }
}
