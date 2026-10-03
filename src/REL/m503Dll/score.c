#include "game/object.h"
#include "game/sprite.h"
#include "game/gamework.h"
#include "datadir_enum.h"
#include "string.h"

typedef struct MgScore_s MGSCORE;

MGSCORE *MgScoreCreate(int digitFile, int unitFile, BOOL dispLeadZeroF);
void MgScoreValueSet(MGSCORE *score, int value);
void MgScorePosSet(MGSCORE *score, float posX, float posY);
void MgScoreTPLvlSet(MGSCORE *score, float tpLvl);
void MgScoreColorSet(MGSCORE *score, u8 r, u8 g, u8 b);
void MgScoreMaxDigitSet(MGSCORE *score, int maxDigit);
void MgScoreDispOn(MGSCORE *score);
void MgScoreDispOff(MGSCORE *score);

HUSPRGRPID MgScoreWinCreate(s16 sizeX, s16 sizeY);
void MgScoreWinTPLvlSet(HUSPRGRPID gid, float tpLvl);
void MgScoreWinPosSet(HUSPRGRPID gid, float posX, float posY);
void MgScoreWinDispSet(HUSPRGRPID gid, BOOL disp);

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
    f32 v;
} SCORE_F32;

typedef struct {
    s32 state;
    f32 timer;
    SCORE_PLAYER player[GW_PLAYER_MAX];
} SCORE_WORK;

extern const SCORE_POS lbl_1_data_640[GW_PLAYER_MAX];
extern const u32 lbl_1_data_660[15];
extern const f32 lbl_1_rodata_640;
extern const f32 lbl_1_rodata_644;
extern const f32 lbl_1_rodata_648;
extern const f64 lbl_1_rodata_650;
extern const f64 lbl_1_rodata_658;
extern const f64 lbl_1_rodata_660;

u32 fn_1_1AE0(void);
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
    HUSPRGRPID win;
    MGSCORE *score;
    HUSPRGRPID grp;
    s32 i;

    for (i = 0; i < GW_PLAYER_MAX; i++) {
        win = MgScoreWinCreate(80, 40);
        work->player[i].win = win;
        MgScoreWinPosSet(win, lbl_1_data_640[i].x, lbl_1_data_640[i].y);
        MgScoreWinDispSet(win, FALSE);

        score = MgScoreCreate(DATANUM(DATA_mgconst, 0x31), -1, TRUE);
        work->player[i].scorePtr = score;
        MgScoreMaxDigitSet(score, 2);
        MgScoreDispOff(score);
        MgScorePosSet(score, 3.0f + lbl_1_data_640[i].x, lbl_1_data_640[i].y);
        MgScoreColorSet(score, 0xFF, 0xD8, 0x15);

        anim = HuSprAnimRead(HuDataSelHeapReadNum(lbl_1_data_660[GwPlayerConf[i].charNo],
                                                 HU_MEMNUM_OVL, HEAP_MODEL));

        grp = HuSprGrpCreate(1);
        work->player[i].grp = grp;
        HuSprGrpPosSet(grp, lbl_1_data_640[i].x, lbl_1_data_640[i].y);
        HuSprGrpMemberSet(grp, 0, HuSprCreate(anim, 50, 0));
        HuSprPosSet(grp, 0, lbl_1_rodata_644, lbl_1_rodata_648);

        {
            s32 j;

            for (j = 0; j < 1; j++) {
                HuSprAttrSet(grp, j, HUSPR_ATTR_NOANIM);
                HuSprDispOff(grp, j);
            }
        }
    }
}

// NONMATCHING: the original's state dispatch is a switch whose compare chain
// MWCC does not reproduce from a plain case list
void fn_1_D4CC(OMOBJ *obj)
{
    s32 value;
    HUSPRGRPID win;
    HUSPRGRPID grp;
    MGSCORE *score;
    s32 i;
    s32 j;
    SCORE_WORK *work = obj->data;

    switch (work->state) {
    case 0:
    case 2:
        if (fn_1_1AE0() != 2) {
            break;
        }
        work->state++;
        work->timer = 0.0f;
        for (i = 0; i < GW_PLAYER_MAX; i++) {
            win = work->player[i].win;
            MgScoreWinDispSet(win, TRUE);
            score = work->player[i].scorePtr;
            MgScoreDispOn(score);
            grp = work->player[i].grp;
            for (j = 0; j < 1; j++) {
                HuSprDispOn(grp, j);
            }
        }
        /* fall through */
    case 1:
        if ((work->timer += 0.1) >= 1.0) {
            work->state++;
        }
        for (i = 0; i < GW_PLAYER_MAX; i++) {
            win = work->player[i].win;
            MgScoreWinTPLvlSet(win, 0.7 * work->timer);
            score = work->player[i].scorePtr;
            MgScoreTPLvlSet(score, work->timer);
            grp = work->player[i].grp;
            for (j = 0; j < 1; j++) {
                HuSprTPLvlSet(grp, j, work->timer);
            }
        }
        break;
    }

    for (i = 0; i < GW_PLAYER_MAX; i++) {
        grp = work->player[i].grp;
        value = work->player[i].score;
        if (value > 99) {
            value = 99;
        }
        score = work->player[i].scorePtr;
        MgScoreValueSet(score, value);
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
    OMOBJ *obj = lbl_1_bss_C8;
    SCORE_WORK *work = obj->data;

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

const f32 lbl_1_rodata_644 = 0.0f;

const f32 lbl_1_rodata_648 = 0.0f;

const f64 lbl_1_rodata_650 = 0.1;
const f64 lbl_1_rodata_658 = 1.0;
const f64 lbl_1_rodata_660 = 0.7;