#include "game/object.h"
#include "game/sprite.h"
#include "game/gamework.h"
#include "game/gamemes.h"
#include "game/mg/score.h"
#include "datadir_enum.h"
#include "string.h"

typedef struct {
    f32 x;
    f32 y;
} SCORE_POS;

typedef struct {
    s32 score;
    HUSPRGRPID win;
    HUSPRGRPID grp;
    MGSCORE *scorePtr;
} SCORE_PLAYER;

typedef struct {
    s32 state;
    f32 timer;
    SCORE_PLAYER player[GW_PLAYER_MAX];
} SCORE_WORK;

extern const SCORE_POS lbl_1_data_640[GW_PLAYER_MAX];
extern const u32 lbl_1_data_660[15];
extern const f32 lbl_1_rodata_640;
extern const Vec lbl_1_rodata_644;
extern const f64 lbl_1_rodata_650;
extern const f64 lbl_1_rodata_658;
extern const f64 lbl_1_rodata_660;

s32 fn_1_1AE0(void);
void fn_1_2260(s32 se, f32 x);

OMOBJ *lbl_1_bss_C8;

void fn_1_D1B4(OMOBJMAN *objMan);
void fn_1_D23C(OMOBJ *obj);
void fn_1_D240(OMOBJ *obj);
void fn_1_D27C(OMOBJ *obj);
void fn_1_D2A4(OMOBJ *obj);
void fn_1_D4CC(OMOBJ *obj);
void fn_1_D6D4(s32 playerNo, s32 add);
s32 fn_1_D778(s32 playerNo);

void fn_1_D1B4(OMOBJMAN *objMan)
{
    OMOBJ *obj;
    SCORE_WORK *work;

    obj = omAddObj(objMan, 80, 0, 0, fn_1_D240);
    lbl_1_bss_C8 = obj;
    work = HuMemDirectMallocNum(HEAP_HEAP, sizeof(SCORE_WORK), HU_MEMNUM_OVL);
    obj->data = work;
    memset(work, 0, sizeof(SCORE_WORK));
}

void fn_1_D23C(OMOBJ *obj)
{
}

void fn_1_D240(OMOBJ *obj)
{
    fn_1_D2A4(obj);
    obj->objFunc = fn_1_D27C;
}

void fn_1_D27C(OMOBJ *obj)
{
    fn_1_D4CC(obj);
}

void fn_1_D2A4(OMOBJ *obj)
{
    SCORE_WORK *work = obj->data;
    ANIMDATA *anim;
    s32 i;
    s32 j;

    for (i = 0; i < GW_PLAYER_MAX; i++) {
        work->player[i].win = MgScoreWinCreate(80, 40);
        MgScoreWinPosSet(work->player[i].win, lbl_1_data_640[i].x, lbl_1_data_640[i].y);
        MgScoreWinDispSet(work->player[i].win, FALSE);

        work->player[i].scorePtr =
            MgScoreCreate(DATANUM(DATA_mgconst, 0x31), -1, TRUE);
        MgScoreMaxDigitSet(work->player[i].scorePtr, 2);
        MgScoreDispOff(work->player[i].scorePtr);
        MgScorePosSet(work->player[i].scorePtr, 3.0f + lbl_1_data_640[i].x,
                      lbl_1_data_640[i].y);
        MgScoreColorSet(work->player[i].scorePtr, 0xFF, 0xD8, 0x15);

        anim = HuSprAnimRead(HuDataSelHeapReadNum(lbl_1_data_660[GwPlayerConf[i].charNo],
                                                 HU_MEMNUM_OVL, HEAP_MODEL));

        work->player[i].grp = HuSprGrpCreate(1);
        HuSprGrpPosSet(work->player[i].grp, lbl_1_data_640[i].x, lbl_1_data_640[i].y);
        HuSprGrpMemberSet(work->player[i].grp, 0, HuSprCreate(anim, 50, 0));
        HuSprPosSet(work->player[i].grp, 0, lbl_1_rodata_644.x, lbl_1_rodata_644.y);

        for (j = 0; j < 2; j++) {
            HuSprAttrSet(work->player[i].grp, j, HUSPR_ATTR_NOANIM);
            HuSprDispOff(work->player[i].grp, j);
        }
    }
}

// NONMATCHING: the original dispatches on work->state with a switch whose
// compare chain does not come out of an if/else spelling
void fn_1_D4CC(OMOBJ *obj)
{
    SCORE_WORK *work = obj->data;
    HUSPRGRPID grp;
    s32 value;
    s32 i;
    s32 j;

    switch (work->state) {
    case 0:
    case 2:
        if (fn_1_1AE0() != 2) {
            break;
        }
        work->state++;
        work->timer = 0.0f;
        for (i = 0; i < GW_PLAYER_MAX; i++) {
            MgScoreWinDispSet(work->player[i].win, TRUE);
            MgScoreDispOn(work->player[i].scorePtr);
            for (j = 0; j < 2; j++) {
                HuSprDispOn(work->player[i].grp, j);
            }
        }
        /* fall through */
    case 1:
        work->timer = work->timer + 0.1;
        if (work->timer >= 1.0) {
            work->state++;
        }
        for (i = 0; i < GW_PLAYER_MAX; i++) {
            MgScoreWinTPLvlSet(work->player[i].win, 0.7 * work->timer);
            MgScoreTPLvlSet(work->player[i].scorePtr, work->timer);
            for (j = 0; j < 2; j++) {
                HuSprTPLvlSet(work->player[i].grp, j, work->timer);
            }
        }
        break;
    default:
        break;
    }

    for (i = 0; i < GW_PLAYER_MAX; i++) {
        grp = work->player[i].grp;
        value = work->player[i].score;
        if (value > 99) {
            value = 99;
        }
        MgScoreValueSet(work->player[i].scorePtr, value);
    }
}

void fn_1_D6D4(s32 playerNo, s32 add)
{
    SCORE_WORK *work = lbl_1_bss_C8->data;

    work->player[playerNo].score += add;

    if (add == 1) {
        fn_1_2260(0x640, lbl_1_data_640[playerNo].x);
    } else {
        fn_1_2260(0x644, lbl_1_data_640[playerNo].x);
    }
}

s32 fn_1_D778(s32 playerNo)
{
    SCORE_WORK *work = lbl_1_bss_C8->data;

    return work->player[playerNo].score;
}

const SCORE_POS lbl_1_data_640[GW_PLAYER_MAX] = {
    { 53.0f, 108.0f },
    { 143.0f, 108.0f },
    { 397.0f, 108.0f },
    { 544.0f, 108.0f },
};

const u32 lbl_1_data_660[15] = {
    DATANUM(DATA_mgconst, 0x20), DATANUM(DATA_mgconst, 0x21),
    DATANUM(DATA_mgconst, 0x22), DATANUM(DATA_mgconst, 0x23),
    DATANUM(DATA_mgconst, 0x24), DATANUM(DATA_mgconst, 0x26),
    DATANUM(DATA_mgconst, 0x27), DATANUM(DATA_mgconst, 0x28),
    DATANUM(DATA_mgconst, 0x29), DATANUM(DATA_mgconst, 0x2A),
    DATANUM(DATA_mgconst, 0x2B), DATANUM(DATA_mgconst, 0x2C),
    DATANUM(DATA_mgconst, 0x2D), DATANUM(DATA_mgconst, 0x2A),
    DATANUM(DATA_mgconst, 0x2A),
};

const f32 lbl_1_rodata_640 = 3.0f;

const Vec lbl_1_rodata_644 = { 0.0f, 0.0f, 0.0f };

const f64 lbl_1_rodata_650 = 0.1;
const f64 lbl_1_rodata_658 = 1.0;
const f64 lbl_1_rodata_660 = 0.7;