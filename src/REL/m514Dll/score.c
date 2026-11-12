#include "types.h"
#include "game/data.h"
#include "game/flag.h"
#include "game/gamemes.h"
#include "game/gamework.h"
#include "game/memory.h"
#include "game/mg/score.h"
#include "game/mg/seqman.h"
#include "game/object.h"
#include "game/sprite.h"
#include "REL/m514Dll/m514types.h"

#include <string.h>

typedef struct M514ScorePlayer_s {
    s32 score;
    s32 rank;
    HUSPRGRPID win;
    HUSPRGRPID grp;
    MGSCORE *hi;
    MGSCORE *lo;
    f32 x;
    f32 y;
    f32 tx;
    f32 ty;
} M514SCOREPLAYER;

typedef struct M514Score_s {
    s32 state;
    s32 pad4;
    f32 alpha;
    M514SCOREPLAYER player[4];
    s32 recState;
    s32 recTimer;
    s32 record;
    s32 recShown;
    HUSPRGRPID recWin;
    HUSPRGRPID recGrp;
    MGSCORE *recHi;
    MGSCORE *recLo;
} M514SCORE;

void fn_1_8304(OMOBJ *obj);
void fn_1_835C(OMOBJ *obj);
void fn_1_83A8(OMOBJ *obj);
void fn_1_879C(OMOBJ *obj);
void fn_1_8B54(OMOBJ *obj);
void fn_1_8C7C(OMOBJ *obj);
void fn_1_8D58(OMOBJ *obj);
void fn_1_900C(OMOBJ *obj);
void fn_1_9258(OMOBJ *obj);
void fn_1_9300(HUSPRGRPID win, s16 pri);

HuVec2f lbl_1_data_4F8 = { 288.0f, 58.0f };

s32 lbl_1_data_500[15] = {
    0x960020, 0x960021, 0x960022, 0x960023, 0x960024, 0x960026, 0x960027, 0x960028,
    0x960029, 0x96002A, 0x96002B, 0x96002C, 0x96002D, 0x96002A, 0x96002A,
};

HuVec2f lbl_1_data_53C[4] = {
    { 102.0f, 58.0f },
    { 102.0f, 108.0f },
    { 102.0f, 158.0f },
    { 102.0f, 208.0f },
};

OMOBJ *lbl_1_bss_220;

void fn_1_8268(OMOBJMAN *objman)
{
    OMOBJ *obj;
    M514SCORE *w;
    u32 record;

    obj = omAddObjEx(objman, 0x46, 0, 0, -1, fn_1_8304);
    lbl_1_bss_220 = obj;
    w = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M514SCORE), 0x10000000);
    obj->data = w;
    memset(w, 0, sizeof(M514SCORE));
    record = GWMgRecordGet(5);
    w->record = record;
    w->recShown = record;
}

void fn_1_8300(void)
{
}

void fn_1_8304(OMOBJ *obj)
{
    fn_1_83A8(obj);
    if (_CheckFlag(0x30002) == 0) {
        fn_1_8D58(obj);
    }
    obj->objFunc = fn_1_835C;
}

void fn_1_835C(OMOBJ *obj)
{
    fn_1_879C(obj);
    if (_CheckFlag(0x30002) == 0) {
        fn_1_900C(obj);
    }
}

void fn_1_83A8(OMOBJ *obj)
{
    M514SCORE *w = obj->data;
    s32 i;
    s16 m;

    for (i = 0; i < 4; i++) {
        M514SCOREPLAYER *pl = &w->player[i];
        MGSCORE *hi;
        MGSCORE *lo;
        ANIMDATA *animChar;
        ANIMDATA *animUnit;
        HUSPRGRPID grp;
        s32 charNo = GwPlayerConf[i].charNo;

#ifdef TARGET_PC
        if (charNo < 0 || charNo > 14) {
            charNo = 0;
        }
#endif
        pl->rank = i;
        pl->x = lbl_1_data_53C[i].x;
        pl->y = lbl_1_data_53C[i].y;
        pl->tx = lbl_1_data_53C[i].x;
        pl->ty = lbl_1_data_53C[i].y;
        pl->win = MgScoreWinCreate(0xB0, 0x28);
        MgScoreWinPosSet(pl->win, lbl_1_data_53C[i].x, lbl_1_data_53C[i].y);
        MgScoreWinDispSet(pl->win, 0);
        hi = MgScoreCreate(0x960030, -1, 0);
        pl->hi = hi;
        MgScoreMaxDigitSet(hi, 3);
        MgScoreDispOff(hi);
        MgScorePosSet(hi, -48.0f + lbl_1_data_53C[i].x, lbl_1_data_53C[i].y);
        MgScoreColorSet(hi, 0xFF, 0xD8, 0x15);
        lo = MgScoreCreate(0x960030, -1, 1);
        pl->lo = lo;
        MgScoreMaxDigitSet(lo, 2);
        MgScoreDispOff(lo);
        MgScorePosSet(lo, 10.0f + lbl_1_data_53C[i].x, lbl_1_data_53C[i].y);
        MgScoreColorSet(lo, 0xFF, 0xD8, 0x15);
        animChar = HuSprAnimRead(HuDataSelHeapReadNum(lbl_1_data_500[charNo], 0x10000000, HEAP_MODEL));
        animUnit = HuSprAnimRead(HuDataSelHeapReadNum(0x480022, 0x10000000, HEAP_MODEL));
        grp = HuSprGrpCreate(3);
        pl->grp = grp;
        HuSprGrpPosSet(grp, lbl_1_data_53C[i].x, lbl_1_data_53C[i].y);
        HuSprGrpMemberSet(grp, 0, HuSprCreate(animChar, 0x32, 0));
        HuSprPosSet(grp, 0, -70.0f, 0.0f);
        HuSprGrpMemberSet(grp, 1, HuSprCreate(lo->digitAnim, 0x32, 0xC));
        HuSprPosSet(grp, 1, 0.0f, 0.0f);
        HuSprColorSet(grp, 1, 0xFF, 0xD8, 0x15);
        HuSprGrpMemberSet(grp, 2, HuSprCreate(animUnit, 0x32, 2));
        HuSprPosSet(grp, 2, 60.0f, 0.0f);
        HuSprColorSet(grp, 2, 0xFF, 0xD8, 0x15);
        for (m = 0; m < 3; m++) {
            HuSprAttrSet(grp, m, 1);
            HuSprAttrSet(grp, m, 4);
        }
    }
    fn_1_8C7C(obj);
}

void fn_1_879C(OMOBJ *obj)
{
    M514SCORE *w = obj->data;
    s32 i;
    s16 m;

    switch (w->state) {
        case 0:
            if (MgSeqModeGet() != 5) {
                break;
            }
            w->state++;
            w->alpha = 0.0f;
            for (i = 0; i < 4; i++) {
                MgScoreWinDispSet(w->player[i].win, 1);
                MgScoreDispOn(w->player[i].hi);
                MgScoreDispOn(w->player[i].lo);
                for (m = 0; m < 3; m++) {
                    HuSprAttrReset(w->player[i].grp, m, 4);
                }
            }
            /* fallthrough */
        case 1:
            w->alpha = w->alpha + 0.1;
            if (w->alpha >= 1.0) {
                w->alpha = 1.0f;
                w->state++;
            }
            for (i = 0; i < 4; i++) {
                MgScoreWinTPLvlSet(w->player[i].win, (f32)(0.7 * (f64)w->alpha));
                MgScoreTPLvlSet(w->player[i].hi, w->alpha);
                MgScoreTPLvlSet(w->player[i].lo, w->alpha);
                for (m = 0; m < 3; m++) {
                    HuSprTPLvlSet(w->player[i].grp, m, w->alpha);
                }
            }
            break;
    }
    for (i = 0; i < 4; i++) {
        s32 hiVal = w->player[i].score / 100;
        f32 fx;
        f32 fy;

        if (hiVal > 0x3E7) {
            hiVal = 0x3E7;
        }
        MgScoreValueSet(w->player[i].hi, hiVal);
        MgScoreValueSet(w->player[i].lo, w->player[i].score % 100);
        w->player[i].x = w->player[i].x + 0.2 * (w->player[i].tx - w->player[i].x);
        w->player[i].y = w->player[i].y + 0.2 * (w->player[i].ty - w->player[i].y);
        fx = w->player[i].x;
        fy = w->player[i].y;
        MgScoreWinPosSet(w->player[i].win, fx, fy);
        MgScorePosSet(w->player[i].hi, -48.0f + fx, fy);
        MgScorePosSet(w->player[i].lo, 10.0f + fx, fy);
        HuSprGrpPosSet(w->player[i].grp, fx, fy);
    }
}

void fn_1_8B54(OMOBJ *obj)
{
    M514SCORE *w = obj->data;
    s32 order[4];
    s32 i;
    s32 j;

    for (i = 0; i < 4; i++) {
        w->player[i].rank = i;
        order[i] = i;
    }
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            s32 a = order[j];
            s32 b = order[j + 1];
            if (w->player[a].score < w->player[b].score) {
                s32 tmp = w->player[a].rank;
                w->player[a].rank = w->player[b].rank;
                w->player[b].rank = tmp;
                tmp = order[j];
                order[j] = order[j + 1];
                order[j + 1] = tmp;
            }
        }
    }
}

void fn_1_8C7C(OMOBJ *obj)
{
    M514SCORE *w = obj->data;
    s32 i;

    for (i = 0; i < 4; i++) {
        M514SCOREPLAYER *pl = &w->player[i];
        s16 pri = pl->rank * 2 + 0x32;
        HUSPRGRPID grp;

        fn_1_9300(pl->win, pri + 1);
        MgScorePriSet(pl->hi, pri);
        MgScorePriSet(pl->lo, pri);
        grp = pl->grp;
        HuSprPriSet(grp, 0, pri);
        HuSprPriSet(grp, 1, pri);
        HuSprPriSet(grp, 2, pri);
    }
}

void fn_1_8D58(OMOBJ *obj)
{
    M514SCORE *w = obj->data;
    MGSCORE *hi;
    MGSCORE *lo;
    HUSPRGRPID grp;

    w->recState = 0;
    w->recWin = MgScoreWinCreate(0xB0, 0x28);
    MgScoreWinPosSet(w->recWin, lbl_1_data_4F8.x, lbl_1_data_4F8.y);
    hi = MgScoreCreate(0x960030, -1, 0);
    w->recHi = hi;
    MgScoreMaxDigitSet(hi, 3);
    MgScorePosSet(hi, -48.0f + lbl_1_data_4F8.x, lbl_1_data_4F8.y);
    MgScoreColorSet(hi, 0x42, 0xFF, 0x7A);
    lo = MgScoreCreate(0x960030, -1, 1);
    w->recLo = lo;
    MgScoreMaxDigitSet(lo, 2);
    MgScorePosSet(lo, 10.0f + lbl_1_data_4F8.x, lbl_1_data_4F8.y);
    MgScoreColorSet(lo, 0x42, 0xFF, 0x7A);
    grp = HuSprGrpCreate(3);
    w->recGrp = grp;
    HuSprGrpPosSet(grp, lbl_1_data_4F8.x, lbl_1_data_4F8.y);
    HuSprGrpMemberSet(grp, 0, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(0x960045, 0x10000000, HEAP_MODEL)), 0x32, 0));
    HuSprPosSet(grp, 0, -70.0f, 0.0f);
    HuSprGrpMemberSet(grp, 1, HuSprCreate(lo->digitAnim, 0x32, 0xC));
    HuSprPosSet(grp, 1, 0.0f, 0.0f);
    HuSprColorSet(grp, 1, 0x42, 0xFF, 0x7A);
    HuSprGrpMemberSet(grp, 2, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(0x480022, 0x10000000, HEAP_MODEL)), 0x32, 2));
    HuSprPosSet(grp, 2, 60.0f, 0.0f);
    HuSprColorSet(grp, 2, 0x42, 0xFF, 0x7A);
    fn_1_9258(obj);
}

void fn_1_900C(OMOBJ *obj)
{
    M514SCORE *w = obj->data;
    f32 scale = 1.0f;

    switch (w->recState) {
        case 0:
            break;
        case 1:
            if (MgSeqModeGet() == 7) {
                w->recState++;
            }
            break;
        case 2:
            if (MgSeqFrameNoGet() >= 0x5A) {
                w->recState++;
                w->recTimer = 0;
                fn_1_9258(obj);
            }
            break;
        case 3:
            w->recTimer++;
            scale = 1.0 + 0.02 * (f32)w->recTimer;
            if (w->recTimer >= 10) {
                w->recState++;
                w->recTimer = 0;
            }
            break;
        case 4:
            w->recTimer++;
            scale = 1.2 - 0.04 * (f32)w->recTimer;
            if (w->recTimer >= 5) {
                w->recState--;
                w->recTimer = 0;
            }
            break;
    }
    if (w->recState != 0) {
        MgScoreDigitScaleSet(w->recHi, scale, scale);
        MgScoreDigitScaleSet(w->recLo, scale, scale);
        HuSprScaleSet(w->recGrp, 0, scale, scale);
        HuSprScaleSet(w->recGrp, 1, scale, scale);
        HuSprScaleSet(w->recGrp, 2, 1.0 + 0.5 * ((f64)scale - 1.0), scale);
    }
}

void fn_1_9258(OMOBJ *obj)
{
    M514SCORE *w = obj->data;
    s32 hiVal = (u32)w->record / 100;

    if (hiVal > 0x3E7) {
        hiVal = 0x3E7;
    }
    MgScoreValueSet(w->recHi, hiVal);
    MgScoreValueSet(w->recLo, w->record % 100);
    w->recShown = w->record;
}

void fn_1_9300(HUSPRGRPID win, s16 pri)
{
    HuSprPriSet(win, 0, pri);
}

void fn_1_9334(void)
{
    M514SCORE *w = lbl_1_bss_220->data;
    s32 order[4];
    s32 i;
    s32 j;

    for (i = 0; i < 4; i++) {
        w->player[i].rank = i;
        order[i] = i;
    }
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            s32 a = order[j];
            s32 b = order[j + 1];
            if (w->player[a].score < w->player[b].score) {
                s32 tmp = w->player[a].rank;
                w->player[a].rank = w->player[b].rank;
                w->player[b].rank = tmp;
                tmp = order[j];
                order[j] = order[j + 1];
                order[j + 1] = tmp;
            }
        }
    }
    for (i = 0; i < 4; i++) {
        M514SCOREPLAYER *pl = &w->player[i];
        s16 pri = pl->rank * 2 + 0x32;
        HUSPRGRPID grp;

        HuSprPriSet(pl->win, 0, pri + 1);
        MgScorePriSet(pl->hi, pri);
        MgScorePriSet(pl->lo, pri);
        grp = pl->grp;
        HuSprPriSet(grp, 0, pri);
        HuSprPriSet(grp, 1, pri);
        HuSprPriSet(grp, 2, pri);
    }
    for (i = 0; i < 4; i++) {
        s32 rank = w->player[i].rank;
#ifdef TARGET_PC
        if (rank < 0 || rank > 3) {
            rank = 0;
        }
#endif
        w->player[i].tx = lbl_1_data_53C[rank].x;
        w->player[i].ty = lbl_1_data_53C[rank].y;
    }
}

void fn_1_9570(s32 idx, f32 val)
{
    M514SCORE *w = lbl_1_bss_220->data;

#ifdef TARGET_PC
    idx &= 3;
#endif
    w->player[idx].score = (s32)(3.0f * val);
}

s32 fn_1_95BC(s32 idx)
{
    M514SCORE *w = lbl_1_bss_220->data;

#ifdef TARGET_PC
    idx &= 3;
#endif
    return w->player[idx].score;
}

f32 fn_1_95F4(s32 idx)
{
    OMOBJ *obj = lbl_1_bss_220;
    M514SCORE *w = obj->data;
    f32 ret;

#ifdef TARGET_PC
    idx &= 3;
#endif
    ret = -2500.0 + (f64)((f32)w->player[idx].score / 3.0f);
    return ret;
}

f32 fn_1_9688(void)
{
    M514SCORE *w = lbl_1_bss_220->data;
    s32 max = 0;
    s32 best = 0;
    s32 i;
    f32 x;

    for (i = 0; i < 4; i++) {
        if (max < w->player[i].score) {
            max = w->player[i].score;
            best = i;
        }
    }
    x = fn_1_4238(best);
    return x + 0.5 * ((f32)(-2500.0 + (f64)((f32)max / 3.0f)) - x);
}

f32 fn_1_9794(void)
{
    OMOBJ *obj = lbl_1_bss_220;
    M514SCORE *w = obj->data;
    f32 ret;

    ret = -2500.0 + (f64)((f32)w->recShown / 3.0f);
    return ret;
}

s32 fn_1_9820(void)
{
    M514SCORE *w = lbl_1_bss_220->data;
    s32 max = 0;
    s32 i;

    for (i = 0; i < 4; i++) {
        if (fn_1_1704(i) != -1 && fn_1_41C4(i) != 0 && w->player[i].score > max) {
            max = w->player[i].score;
        }
    }
    if (max > w->record) {
        MgSeqRecordSet(max);
        GWMgRecordSet(5, max);
        w->record = max;
        w->recState = 1;
        return 1;
    }
    return 0;
}

void fn_1_98F4(void)
{
    s32 i;

    for (i = 0; i < 4; i++) {
        M514SCORE *w = lbl_1_bss_220->data;
        GwPlayer[i].mgScore = w->player[i].score;
    }
}
