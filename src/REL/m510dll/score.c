#define _MATH_H
#include "REL/m510dll.h"
#include "string.h"

// .data:0x0 (absolute 0x4D8) - 4 x HuVecF
HuVecF lbl_1_data_4D8[4] = {
    { 72.0f, 420.0f, 0.0f },
    { 216.0f, 420.0f, 0.0f },
    { 360.0f, 420.0f, 0.0f },
    { 504.0f, 420.0f, 0.0f },
};

// NONMATCHING: 40 of the 412 code rows differ and every one of them is the
// r29/r30 pair. The instruction stream is otherwise identical, but this
// compiler build orders the two block locals (id, score) where the original
// has (score, id), so `score` lands in r29 and `id` in r30 instead of the
// reverse. Declaration order, the local names, the assignment spelling
// (`x = f()` vs `y = x = f()`), block scope and `s16` vs `s32` were all
// tried: none of them changes the order, and giving the second esp id its own
// local does fix score/id but pushes `type`/`playerNo` one register down.
void fn_1_CE04(M510_SCORE_WORK *work, u16 type, s16 playerNo)
{
    s16 id;
    MGSCORE *score;
    HuVecF pos;

    memset(work, 0, sizeof(M510_SCORE_WORK));
    work->type = type;
    work->value = 0;
    work->playerNo = playerNo;

    if (type == 0) {
        work->winId = MgScoreWinCreate(0x70, 0x28);
        MgScoreWinDispSet(work->winId, FALSE);
        score = MgScoreCreate(0x960031, -1, TRUE);
        work->score = score;
        MgScoreMaxDigitSet(score, 2);
        MgScoreValueSet(score, 0);
        MgScoreDispOff(score);
        id = espEntry(0x440033, 0, 0);
        work->espId0 = id;
        espDrawNoSet(id, 0);
        espDispOff(id);
        id = espEntry(0x960046, 0, 0);
        work->espId1 = id;
        espDrawNoSet(id, 0);
        espDispOff(id);
        pos = lbl_1_data_4D8[playerNo];
        MgScoreWinPosSet(work->winId, pos.x, pos.y);
        MgScorePosSet(score, 14.0f + pos.x, pos.y);
        espPosSet(work->espId0, -32.0f + pos.x, pos.y);
        espPosSet(work->espId1, -8.0f + pos.x, pos.y);
    }
    else if (type == 1) {
        id = MgScoreWinCreate(0x48, 0x28);
        work->winId = id;
        MgScoreWinTPLvlSet(id, 0.7f);
        MgScoreWinPosSet(id, 524.0f, 60.0f);
        MgScoreWinDispSet(work->winId, FALSE);
        id = espEntry(0x960045, 0, 0);
        work->espId2 = id;
        espDrawNoSet(id, 0);
        espPosSet(id, 505.0f, 60.0f);
        espDispOff(id);
        score = MgScoreCreate(0x960030, -1, TRUE);
        work->score = score;
        MgScoreValueSet(score, 0);
        MgScorePosSet(score, 526.0f, 60.0f);
        MgScoreMaxDigitSet(score, 2);
        MgScoreColorSet(score, 0x42, 0xFF, 0x7A);
        MgScoreDispOff(score);
        work->dispFlag = 0;
        work->scale = 1.0f;
    }
    else {
        id = MgScoreWinCreate(0x40, 0x30);
        work->winId = id;
        MgScoreWinTPLvlSet(id, 0.7f);
        MgScoreWinPosSet(id, 288.0f, 64.0f);
        MgScoreWinDispSet(work->winId, FALSE);
        score = MgScoreCreate(0x960031, -1, TRUE);
        work->score = score;
        MgScoreValueSet(score, 0);
        MgScorePosSet(score, 278.0f, 64.0f);
        MgScoreMaxDigitSet(score, 2);
        MgScoreDispOff(score);
    }
}

void fn_1_D1AC(M510_SCORE_WORK *work)
{
    if (work->type == 0) {
        espKill(work->espId1);
        espKill(work->espId0);
        MgScoreKill(work->score);
    }
    else if (work->type == 1) {
        espKill(work->espId2);
        MgScoreKill(work->score);
    }
    else {
        MgScoreKill(work->score);
    }
    memset(work, 0, sizeof(M510_SCORE_WORK));
}

void fn_1_D234(M510_SCORE_WORK *work)
{
    MgScoreValueSet(work->score, work->value);

    if (work->dispFlag != 0 && work->type == 1) {
        if (work->dirFlag == 0) {
            if ((work->scale += 0.02f) >= 1.2f) {
                work->scale = 1.2f;
                work->dirFlag = 1;
            }
        }
        else {
            if ((work->scale -= 0.04f) <= 1.0f) {
                work->scale = 1.0f;
                work->dirFlag = 0;
            }
        }
    }
    else {
        work->scale = 1.0f;
    }

    MgScoreDigitScaleSet(work->score, work->scale, work->scale);
    espScaleSet(work->espId2, work->scale, work->scale);
}

void fn_1_D354(M510_SCORE_WORK *work, s32 disp)
{
    if (work->type == 0) {
        if (disp) {
            MgScoreWinDispSet(work->winId, TRUE);
            MgScoreDispOn(work->score);
            espDispOn(work->espId0);
            espDispOn(work->espId1);
        }
        else {
            MgScoreWinDispSet(work->winId, FALSE);
            MgScoreDispOff(work->score);
            espDispOff(work->espId0);
            espDispOff(work->espId1);
        }
    }
    else if (work->type == 1) {
        if (disp) {
            MgScoreWinDispSet(work->winId, TRUE);
            MgScoreDispOn(work->score);
            espDispOn(work->espId2);
        }
        else {
            MgScoreWinDispSet(work->winId, FALSE);
            MgScoreDispOff(work->score);
            espDispOff(work->espId2);
        }
    }
    else {
        if (disp) {
            MgScoreWinDispSet(work->winId, TRUE);
            MgScoreDispOn(work->score);
        }
        else {
            MgScoreWinDispSet(work->winId, FALSE);
            MgScoreDispOff(work->score);
        }
    }
}
