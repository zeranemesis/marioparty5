#include "types.h"
#include "game/object.h"
#include "game/audio.h"
#include "game/data.h"
#include "game/esprite.h"
#include "game/gamemes.h"
#include "game/memory.h"
#include "game/mgdata.h"
#include "game/sprite.h"
#include "game/mg/score.h"
#include "game/mg/seqman.h"
#include "humath.h"
#include <string.h>

#include "REL/m580Dll.h"

#ifndef M_PI
#define M_PI 3.141592653589793
#endif

/* one sprite-number object made of up to two digit sprites */
typedef struct M580Digit_s {
    /* 0x00 */ s16 id[2];
    /* 0x04 */ f32 ofs[2];
    /* 0x0C */ f32 x;
    /* 0x10 */ f32 y;
    /* 0x14 */ f32 vx;
    /* 0x18 */ f32 vy;
    /* 0x1C */ f32 sx;
    /* 0x20 */ f32 sy;
    /* 0x24 */ f32 alpha;
    /* 0x28 */ f32 rot;
} M580Digit; /* size 0x2C */

/* "goal scored" banner state, lives at M580ScoreData + 0x3C */
typedef struct M580GoalFx_s {
    /* 0x00 */ s32 state;
    /* 0x04 */ u32 timer;
    /* 0x08 */ u32 timer2;
    /* 0x0C */ s32 prev[2];
    /* 0x14 */ s32 score[2];
    /* 0x1C */ s32 side;
    /* 0x20 */ M580Digit dig[4];
    /* 0xD0 */ s32 spr[3][20][4];
    /* 0x490 */ s32 sprExtra[2];
} M580GoalFx;

typedef struct M580ScoreEntry_s {
    /* 0x00 */ s32 score;
    /* 0x04 */ s16 win;
    /* 0x08 */ MGSCORE *obj;
} M580ScoreEntry;

typedef struct M580ScoreData_s {
    /* 0x00 */ s32 introState;
    /* 0x04 */ f32 introAlpha;
    /* 0x08 */ s32 timerState;
    /* 0x0C */ f32 timerAlpha;
    /* 0x10 */ s32 timeLeft;
    /* 0x14 */ s16 timerWin;
    /* 0x16 */ s16 timerSpr;
    /* 0x18 */ MGSCORE *timerMin;
    /* 0x1C */ MGSCORE *timerSec;
    /* 0x20 */ MGSCORE *timerCsec;
    /* 0x24 */ M580ScoreEntry entry[2];
    /* 0x3C */ M580GoalFx goal;
    u8 pad[8];
} M580ScoreData;

void fn_1_EEC0(OMOBJ *obj);
void fn_1_EF0C(OMOBJ *obj);
void fn_1_EF4C(OMOBJ *obj);
void fn_1_F0B8(OMOBJ *obj);
void fn_1_F254(OMOBJ *obj);
void fn_1_F758(OMOBJ *obj);
void fn_1_108DC(OMOBJ *obj);
void fn_1_10C44(OMOBJ *obj);

f32 lbl_1_data_7E0[2][2] = { { 47.0f, 59.0f }, { 529.0f, 59.0f } };
f32 lbl_1_data_7F0[2] = { 288.0f, 55.0f };
s32 lbl_1_data_7F8[11][4] = {
    { 0x230010 },
    { 0x230011 },
    { 0x230012 },
    { 0x230013 },
    { 0x230014 },
    { 0x230015 },
    { 0x230016 },
    { 0x230017 },
    { 0x230018 },
    { 0x230019 },
    { 0x23004E },
};

OMOBJ *lbl_1_bss_250;

void fn_1_EE34(OMOBJMAN *objMan)
{
    OMOBJ *obj;

    obj = omAddObjEx(objMan, 0x50, 0, 0, OM_GRP_NONE, fn_1_EEC0);
    lbl_1_bss_250 = obj;
    obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M580ScoreData), HU_MEMNUM_OVL);
    memset(obj->data, 0, sizeof(M580ScoreData));
}

void fn_1_EEBC(void)
{
}

void fn_1_EEC0(OMOBJ *obj)
{
    fn_1_EF4C(obj);
    fn_1_F254(obj);
    fn_1_108DC(obj);
    obj->objFunc = fn_1_EF0C;
}

void fn_1_EF0C(OMOBJ *obj)
{
    fn_1_F0B8(obj);
    fn_1_F758(obj);
    fn_1_10C44(obj);
}

void fn_1_EF4C(OMOBJ *obj)
{
    M580ScoreData *data = obj->data;
    s16 win;
    MGSCORE *score;
    s32 i;

    for (i = 0; i < 2; i++) {
        win = MgScoreWinCreate(0x40, 0x28);
        data->entry[i].win = win;
        MgScoreWinPosSet(win, lbl_1_data_7E0[i][0], lbl_1_data_7E0[i][1]);
        MgScoreWinDispSet(win, 0);
        if (i == 0) {
            MgScoreWinColorSet(win, 0xA0, 0x40, 0x40);
        } else {
            MgScoreWinColorSet(win, 0x40, 0x40, 0xA0);
        }
        score = MgScoreCreate(0x960031, -1, 1);
        data->entry[i].obj = score;
        MgScoreMaxDigitSet(score, 2);
        MgScoreDispOff(score);
        MgScorePosSet(score, lbl_1_data_7E0[i][0] - 10.0f, lbl_1_data_7E0[i][1]);
        MgScoreColorSet(score, 0xFF, 0xD8, 0x15);
        MgScorePriSet(score, 0xA);
    }
}

void fn_1_F0B8(OMOBJ *obj)
{
    M580ScoreData *data = obj->data;
    s32 i;
    s32 value;

    switch (data->introState) {
        case 0:
            if (MgSeqModeGet() != 5) {
                break;
            }
            data->introState++;
            data->introAlpha = 0.0f;
            for (i = 0; i < 2; i++) {
                MgScoreWinDispSet(data->entry[i].win, 1);
                MgScoreDispOn(data->entry[i].obj);
            }
        case 1:
            data->introAlpha = data->introAlpha + 0.1;
            if (data->introAlpha >= 1.0f) {
                data->introState++;
            }
            for (i = 0; i < 2; i++) {
                MgScoreWinTPLvlSet(data->entry[i].win, 0.7 * data->introAlpha);
                MgScoreTPLvlSet(data->entry[i].obj, data->introAlpha);
            }
            break;
    }
    for (i = 0; i < 2; i++) {
        value = data->entry[i].score;
        if (value > 99) {
            value = 99;
        }
        MgScoreValueSet(data->entry[i].obj, value);
    }
}

void fn_1_F254(OMOBJ *obj)
{
    M580ScoreData *data = obj->data;
    M580GoalFx *goal = &data->goal;
    s32 g;
    s32 i;
    s32 j;

    for (g = 0; g < 3; g++) {
        for (i = 0; i < 20; i++) {
            for (j = 0; j < 2; j++) {
                if (lbl_1_data_7F8[i % 10][j] != 0) {
                    goal->spr[g][i][j] = espEntry(lbl_1_data_7F8[i % 10][j], 0x31, 0);
                    espAttrSet(goal->spr[g][i][j], 8);
                    espDispOff(goal->spr[g][i][j]);
                } else {
                    goal->spr[g][i][j] = -1;
                }
            }
        }
    }
    for (j = 0; j < 2; j++) {
        if (lbl_1_data_7F8[10][j] != 0) {
            goal->sprExtra[j] = espEntry(lbl_1_data_7F8[10][j], 0x31, 0);
            espAttrSet(goal->sprExtra[j], 8);
            espDispOff(goal->sprExtra[j]);
        } else {
            goal->sprExtra[j] = -1;
        }
    }
    for (i = 0; i < 2; i++) {
        goal->dig[i].id[0] = -1;
        goal->dig[i].id[1] = -1;
    }
    goal->dig[2].id[0] = -1;
    goal->dig[2].id[1] = -1;
    goal->dig[3].id[0] = -1;
    goal->dig[3].id[1] = -1;
    HuDataDirClose(lbl_1_data_7F8[0][0]);
}

void fn_1_F4E8(M580Digit *dig)
{
    s32 i;

    for (i = 0; i < 2; i++) {
        if (dig->id[i] >= 0) {
            espDispOn(dig->id[i]);
            espPosSet(dig->id[i], dig->x + dig->ofs[i] * dig->sx * HuCos(dig->rot), dig->y + dig->ofs[i] * dig->sx * HuSin(dig->rot));
            espZRotSet(dig->id[i], dig->rot);
            espScaleSet(dig->id[i], dig->sx, dig->sy);
            espTPLvlSet(dig->id[i], dig->alpha);
        }
    }
}

void fn_1_F640(s32 (*tbl)[4], M580Digit *dig, s32 num)
{
    s32 ones = num % 10;
    u32 tens = num / 10;

    if (tens == 0) {
        dig->id[0] = tbl[ones][0];
        dig->id[1] = -1;
        dig->ofs[0] = 0.0f;
    } else {
        dig->id[0] = tbl[tens + 10][0];
        dig->id[1] = tbl[ones][0];
        dig->ofs[0] = -23.039999f;
        dig->ofs[1] = 23.039999f;
    }
    dig->y = 0.0f;
    dig->x = 0.0f;
    dig->vy = 0.0f;
    dig->vx = 0.0f;
    dig->sy = 1.5f;
    dig->sx = 1.5f;
    dig->alpha = 1.0f;
    dig->rot = 0.0f;
}

void fn_1_F758(OMOBJ *obj)
{
    M580ScoreData *data = obj->data;
    M580GoalFx *goal = &data->goal;
    M580Digit *mover;
    f32 moverVx = 2.88f;
    f32 alpha;
    f32 vy;
    f32 angle;
    f32 amp;
    f32 u;
    s32 g;
    s32 i;

    if (goal->side == 0) {
        mover = &goal->dig[0];
    } else {
        mover = &goal->dig[1];
        moverVx = 2.88f * -1.0f;
    }
    switch (goal->state) {
        case 1:
            fn_1_F640(goal->spr[0], &goal->dig[0], goal->prev[0]);
            fn_1_F640(goal->spr[1], &goal->dig[1], goal->prev[1]);
            fn_1_F640(goal->spr[2], &goal->dig[2], 0);
            fn_1_F640((s32(*)[4]) & goal->sprExtra[0], &goal->dig[3], goal->score[goal->side]);
            goal->dig[0].x = 172.8f;
            goal->dig[0].y = 360.0f - -48.0f;
            goal->dig[1].x = 403.19998f;
            goal->dig[1].y = 360.0f - -48.0f;
            goal->dig[2].x = 288.0f;
            goal->dig[2].y = 360.0f - -48.0f;
            goal->dig[2].alpha = 0.0f;
            goal->dig[1].alpha = 0.0f;
            goal->dig[0].alpha = 0.0f;
            vy = -48.0f / 19.199999f;
            goal->dig[0].vy = vy;
            goal->dig[1].vy = vy;
            goal->dig[2].vy = vy;
            goal->dig[3].x = 806.39996f;
            goal->dig[3].y = 144.0f;
            goal->dig[3].sx = goal->dig[3].sx + 0.5f;
            goal->dig[3].sy = goal->dig[3].sy - 0.5f;
            goal->dig[3].vx = (403.19998f - goal->dig[3].x) / 24.0f;
            goal->dig[3].vy = (360.0f - goal->dig[3].y) / 24.0f;
            if (goal->side == 0) {
                goal->dig[3].x = -230.40001f;
                goal->dig[3].vx = goal->dig[3].vx * -1.0f;
            }
            goal->state++;
            goal->timer = 0;
            break;
        case 2:
            alpha = goal->dig[0].alpha + 0.055555556f;
            if (alpha > 0.95f) {
                alpha = 1.0f;
            }
            goal->dig[2].alpha = alpha;
            goal->dig[1].alpha = alpha;
            goal->dig[0].alpha = alpha;
            goal->dig[0].y = goal->dig[0].y + goal->dig[0].vy;
            goal->dig[1].y = goal->dig[1].y + goal->dig[1].vy;
            goal->dig[2].y = goal->dig[2].y + goal->dig[2].vy;
            goal->dig[3].x = goal->dig[3].x + goal->dig[3].vx;
            goal->dig[3].y = goal->dig[3].y + goal->dig[3].vy;
            if ((f32)goal->timer >= 19.199999f) {
                mover->vx = moverVx;
                mover->vy = -16.0f;
                goal->state++;
                goal->timer2 = 0;
                HuAudFXPlay(0x964);
            }
            break;
        case 3:
            if ((f32)goal->timer <= 24.0f) {
                goal->dig[3].x = goal->dig[3].x + goal->dig[3].vx;
                goal->dig[3].y = goal->dig[3].y + goal->dig[3].vy;
            }
            if ((f32)goal->timer2 < 24.0f) {
                goal->timer2++;
                angle = 33.75f * (f32)goal->timer2;
                u = 0.041666668f * (f32)goal->timer2;
                amp = 0.5f * (1.0f - u * u);
                if ((f32)goal->timer2 == 24.0f) {
                    amp = 0.0f;
                }
                goal->dig[3].sx = 1.5 + amp * HuCos(angle);
                goal->dig[3].sy = 1.5 + amp * HuSin(angle - 90.0f);
            }
            mover->sx = goal->dig[3].sy;
            mover->sy = goal->dig[3].sx;
            mover->x = mover->x + mover->vx;
            mover->y = mover->y + mover->vy;
            mover->vy = mover->vy + 0.80000013f;
            mover->rot = mover->rot + 8.333334f;
            mover->alpha = mover->alpha - 0.017833335f;
            if (mover->alpha < 0.0f) {
                mover->alpha = 0.0f;
            }
            if ((f32)goal->timer >= 102.0f) {
                goal->state++;
                goal->timer = 0;
            }
            break;
        case 4:
            alpha = goal->dig[3].alpha - 0.055555556f;
            if (alpha < 0.05f) {
                alpha = 0.0f;
                goal->state++;
                goal->timer = 0;
            }
            goal->dig[3].alpha = alpha;
            goal->dig[2].alpha = alpha;
            goal->dig[1].alpha = alpha;
            goal->dig[0].alpha = alpha;
            break;
        default:
            for (i = 0; i < 2; i++) {
                goal->dig[i].id[0] = -1;
                goal->dig[i].id[1] = -1;
            }
            goal->dig[2].id[0] = -1;
            goal->dig[2].id[1] = -1;
            goal->dig[3].id[0] = -1;
            goal->dig[3].id[1] = -1;
            goal->state = 0;
            break;
    }
    for (g = 0; g < 3; g++) {
        s32(*row)[4] = goal->spr[g];

        for (i = 0; i < 20; i++, row++) {
            if ((*row)[0] >= 0) {
                espDispOff((*row)[0]);
            }
            if ((*row)[1] >= 0) {
                espDispOff((*row)[1]);
            }
        }
    }
    if (goal->sprExtra[0] >= 0) {
        espDispOff(goal->sprExtra[0]);
    }
    if (goal->sprExtra[1] >= 0) {
        espDispOff(goal->sprExtra[1]);
    }
    fn_1_F4E8(&goal->dig[0]);
    fn_1_F4E8(&goal->dig[1]);
    fn_1_F4E8(&goal->dig[2]);
    fn_1_F4E8(&goal->dig[3]);
    goal->timer++;
}

void fn_1_10880(s32 score0, s32 score1, s32 side)
{
    M580ScoreData *data = lbl_1_bss_250->data;
    M580GoalFx *goal = &data->goal;

    goal->state = 1;
    goal->prev[0] = goal->score[0];
    goal->prev[1] = goal->score[1];
    goal->score[0] = score0;
    goal->score[1] = score1;
    goal->side = side;
}

void fn_1_108DC(OMOBJ *obj)
{
    M580ScoreData *data = obj->data;
    HUSPRGRPID grp;
    ANIMDATA *anim;
    s32 time;

    switch (mgMatchMaxScore) {
        case 1:
            time = 0xE10;
            break;
        case 2:
            time = 0x2A30;
            break;
        case 3:
            time = 0x4650;
            break;
        default:
            time = 0x2A30;
            break;
    }
    data->timeLeft = time;
    data->timerWin = MgScoreWinCreate(0x88, 0x20);
    MgScoreWinPosSet(data->timerWin, lbl_1_data_7F0[0], lbl_1_data_7F0[1]);
    MgScoreWinDispSet(data->timerWin, 0);
    data->timerMin = MgScoreCreate(0x960030, -1, 1);
    MgScoreMaxDigitSet(data->timerMin, 1);
    MgScoreDispOff(data->timerMin);
    MgScorePosSet(data->timerMin, lbl_1_data_7F0[0] - 48.0f, lbl_1_data_7F0[1]);
    MgScoreColorSet(data->timerMin, 0xFF, 0xFF, 0xFF);
    MgScorePriSet(data->timerMin, 0xA);
    data->timerSec = MgScoreCreate(0x960030, -1, 1);
    MgScoreMaxDigitSet(data->timerSec, 2);
    MgScoreDispOff(data->timerSec);
    MgScorePosSet(data->timerSec, lbl_1_data_7F0[0] - 16.0f, lbl_1_data_7F0[1]);
    MgScoreColorSet(data->timerSec, 0xFF, 0xFF, 0xFF);
    MgScorePriSet(data->timerSec, 0xA);
    data->timerCsec = MgScoreCreate(0x960030, -1, 1);
    MgScoreMaxDigitSet(data->timerCsec, 2);
    MgScoreDispOff(data->timerCsec);
    MgScorePosSet(data->timerCsec, 32.0f + lbl_1_data_7F0[0], lbl_1_data_7F0[1]);
    MgScoreColorSet(data->timerCsec, 0xFF, 0xFF, 0xFF);
    MgScorePriSet(data->timerCsec, 0xA);
    grp = HuSprGrpCreate(2);
    data->timerSpr = grp;
    HuSprGrpPosSet(grp, lbl_1_data_7F0[0], lbl_1_data_7F0[1]);
    anim = data->timerMin->digitAnim;
    HuSprGrpMemberSet(grp, 0, HuSprCreate(anim, 0x32, 0xA));
    HuSprPosSet(grp, 0, -32.0f, 0.0f);
    HuSprColorSet(grp, 0, 0xFF, 0xFF, 0xFF);
    HuSprGrpMemberSet(grp, 1, HuSprCreate(anim, 0x32, 0xB));
    HuSprPosSet(grp, 1, 16.0f, 0.0f);
    HuSprColorSet(grp, 1, 0xFF, 0xFF, 0xFF);
    {
        s16 i;

        for (i = 0; i < 2; i++) {
            HuSprAttrSet(grp, i, 1);
            HuSprAttrSet(grp, i, 4);
        }
    }
}

void fn_1_10C44(OMOBJ *obj)
{
    M580ScoreData *data = obj->data;
    HUSPRGRPID grp;
    s16 i;
    s32 rest;

    switch (data->timerState) {
        case 0:
            if (MgSeqModeGet() != 3) {
                break;
            }
            data->timerState++;
            data->timerAlpha = 0.0f;
            MgScoreWinDispSet(data->timerWin, 1);
            MgScoreDispOn(data->timerMin);
            MgScoreDispOn(data->timerSec);
            MgScoreDispOn(data->timerCsec);
            grp = data->timerSpr;
            for (i = 0; i < 2; i++) {
                HuSprAttrReset(grp, i, 4);
            }
        case 1:
            data->timerAlpha = data->timerAlpha + 0.1;
            if (data->timerAlpha < 1.0f) {
                break;
            }
            data->timerState++;
            data->timerAlpha = 1.0f;
        case 2:
            if (MgSeqModeGet() != 5) {
                break;
            }
            data->timerState++;
        case 3:
            if (--data->timeLeft > 0) {
                break;
            }
            data->timerState++;
        case 4:
            data->timerAlpha = data->timerAlpha - 0.1;
            if (data->timerAlpha <= 0.0f) {
                data->timerState++;
                data->timerAlpha = 0.0f;
                MgScoreWinDispSet(data->timerWin, 0);
                MgScoreDispOff(data->timerMin);
                MgScoreDispOff(data->timerSec);
                MgScoreDispOff(data->timerCsec);
                grp = data->timerSpr;
                for (i = 0; i < 2; i++) {
                    HuSprAttrSet(grp, i, 4);
                }
            }
            break;
    }
    MgScoreWinTPLvlSet(data->timerWin, 0.7 * data->timerAlpha);
    MgScoreTPLvlSet(data->timerMin, data->timerAlpha);
    MgScoreTPLvlSet(data->timerSec, data->timerAlpha);
    MgScoreTPLvlSet(data->timerCsec, data->timerAlpha);
    grp = data->timerSpr;
    for (i = 0; i < 2; i++) {
        HuSprTPLvlSet(grp, i, data->timerAlpha);
    }
    rest = data->timeLeft % 3600;
    MgScoreValueSet(data->timerMin, data->timeLeft / 3600);
    MgScoreValueSet(data->timerSec, rest / 60);
    MgScoreValueSet(data->timerCsec, ((rest % 60) * 100) / 60);
}

void fn_1_10F58(s32 side, s32 add)
{
    M580ScoreData *data;
    M580GoalFx *goal;
    s32 value;

    if (MgSeqModeGet() == 5) {
        data = lbl_1_bss_250->data;
        value = add + data->entry[side].score;
        if (value > 99) {
            value = 99;
        }
        data->entry[side].score = value;
        data = lbl_1_bss_250->data;
        goal = &data->goal;
        goal->state = 1;
        goal->prev[0] = goal->score[0];
        goal->prev[1] = goal->score[1];
        goal->score[0] = data->entry[0].score;
        goal->score[1] = data->entry[1].score;
        goal->side = side;
    }
}

s32 fn_1_1100C(s32 index)
{
    M580ScoreData *data = lbl_1_bss_250->data;

    return data->entry[index].score;
}

s32 fn_1_11044(void)
{
    return 0;
}

s32 fn_1_1104C(void)
{
    M580ScoreData *data = lbl_1_bss_250->data;

    if (MgSeqModeGet() == 6) {
        data->timeLeft = 0;
    }
    return data->timeLeft;
}
