#include "game/data.h"
#include "game/gamemes.h"
#include "game/gamework.h"
#include "game/mg/score.h"
#include "game/memory.h"
#include "game/object.h"
#include "game/sprite.h"
#include "string.h"

#include "datanum/mgconst.h"

void fn_1_EA08(OMOBJMAN *om);
void fn_1_EA90(OMOBJ *obj);
void fn_1_ECC4(OMOBJ *obj);
void fn_1_EEC0(s32 idx);
void fn_1_EEF8(s32 idx);

typedef struct M576Pos_s {
    f32 x;
    f32 y;
} M576Pos;

typedef struct M576PlayerScore_s {
    s32 score;
    HUSPRGRPID winId;
    HUSPRGRPID sprGrpId;
    MGSCORE *scorePtr;
} M576PlayerScore;

typedef struct M576ScoreWork_s {
    s32 state;
    f32 tpLvl;
    M576PlayerScore player[GW_PLAYER_MAX];
} M576ScoreWork;

M576Pos lbl_1_data_0[GW_PLAYER_MAX] = {
    { 55.0f, 59.0f },
    { 175.0f, 59.0f },
    { 401.0f, 59.0f },
    { 521.0f, 59.0f }
};

s32 lbl_1_data_20[15] = {
    DATANUM(DATA_m576, 0x11),
    DATANUM(DATA_m576, 0x12),
    DATANUM(DATA_m576, 0x13),
    DATANUM(DATA_m576, 0x14),
    DATANUM(DATA_m576, 0x15),
    DATANUM(DATA_m576, 0x16),
    DATANUM(DATA_m576, 0x17),
    DATANUM(DATA_m576, 0x18),
    DATANUM(DATA_m576, 0x19),
    DATANUM(DATA_m576, 0x1A),
    DATANUM(DATA_m576, 0x1B),
    DATANUM(DATA_m576, 0x1C),
    DATANUM(DATA_m576, 0x1D),
    DATANUM(DATA_m576, 0x1A),
    DATANUM(DATA_m576, 0x1A)
};

OMOBJ *lbl_1_bss_0;

void fn_1_EA08(OMOBJMAN *om)
{
    OMOBJ *obj;
    M576ScoreWork *work;

    obj = omAddObjEx(om, 0x14, 0, 0, OM_GRP_NONE, fn_1_EA90);
    lbl_1_bss_0 = obj;
    work = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M576ScoreWork), HU_MEMNUM_OVL);
    obj->data = work;
    memset(work, 0, sizeof(M576ScoreWork));
}

void fn_1_EA90(OMOBJ *obj)
{
    M576ScoreWork *work;
    ANIMDATA *anim;
    HUSPRGRPID winId;
    MGSCORE *scorePtr;
    HUSPRGRPID sprGrpId;
    s32 i;
    s32 j;

    work = obj->data;

    for (i = 0; i < GW_PLAYER_MAX; i++) {
        winId = MgScoreWinCreate(0x50, 0x28);
        work->player[i].winId = winId;
        MgScoreWinPosSet(winId, lbl_1_data_0[i].x, lbl_1_data_0[i].y);
        MgScoreWinDispSet(winId, 0);
        scorePtr = MgScoreCreate(DATANUM(DATA_mgconst, 0x31), -1, 0);
        work->player[i].scorePtr = scorePtr;
        MgScoreMaxDigitSet(scorePtr, 1);
        MgScoreDispOff(scorePtr);
        MgScorePosSet(scorePtr, 14.0f + lbl_1_data_0[i].x, lbl_1_data_0[i].y);
        MgScoreColorSet(scorePtr, 0xFF, 0xD8, 0x15);
        anim = HuSprAnimDataRead(lbl_1_data_20[GwPlayerConf[i].charNo]);
        sprGrpId = HuSprGrpCreate(1);
        work->player[i].sprGrpId = sprGrpId;
        HuSprGrpPosSet(sprGrpId, lbl_1_data_0[i].x, lbl_1_data_0[i].y);
        HuSprGrpMemberSet(sprGrpId, 0, HuSprCreate(anim, 0, 0));
        HuSprPosSet(sprGrpId, 0, -21.0f, 0.0f);
        for (j = 0; j < 1; j++) {
            HuSprAttrSet(sprGrpId, j, HUSPR_ATTR_NOANIM);
            HuSprAttrSet(sprGrpId, j, HUSPR_ATTR_DISPOFF);
        }
    }

    obj->objFunc = fn_1_ECC4;
}

void fn_1_ECC4(OMOBJ *obj)
{
    M576ScoreWork *work;
    HUSPRGRPID sprGrpId;
    s32 score;
    s32 i;
    s32 j;

    work = obj->data;

    switch (work->state) {
    case 0:
        work->state++;
        work->tpLvl = 0.0f;
        for (i = 0; i < GW_PLAYER_MAX; i++) {
            MgScoreWinDispSet(work->player[i].winId, 1);
            MgScoreDispOn(work->player[i].scorePtr);
            for (j = 0; j < 1; j++) {
                HuSprAttrReset(work->player[i].sprGrpId, j, HUSPR_ATTR_DISPOFF);
            }
        }
    case 1:
        work->tpLvl = work->tpLvl + 0.1;
        if (work->tpLvl >= 1.0) {
            work->state++;
        }
        for (i = 0; i < GW_PLAYER_MAX; i++) {
            MgScoreWinTPLvlSet(work->player[i].winId, 0.7 * work->tpLvl);
            MgScoreTPLvlSet(work->player[i].scorePtr, work->tpLvl);
            for (j = 0; j < 1; j++) {
                HuSprTPLvlSet(work->player[i].sprGrpId, j, work->tpLvl);
            }
        }
        break;
    case 3:
        break;
    }

    for (i = 0; i < GW_PLAYER_MAX; i++) {
        sprGrpId = work->player[i].sprGrpId;
        score = work->player[i].score;
        if (score > 0x63) {
            score = 0x63;
        }
        MgScoreValueSet(work->player[i].scorePtr, score);
    }
}

void fn_1_EEC0(s32 idx)
{
    M576ScoreWork *work = lbl_1_bss_0->data;

    work->player[idx].score += 1;
}

void fn_1_EEF8(s32 idx)
{
    M576ScoreWork *work = lbl_1_bss_0->data;

    work->player[idx].score -= 1;
}

