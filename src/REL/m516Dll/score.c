#include "game/data.h"
#include "game/gamemes.h"
#include "game/gamework.h"
#include "game/memory.h"
#include "game/mg/score.h"
#include "game/object.h"
#include "game/sprite.h"
#include "string.h"

typedef struct {
    s32 value;
    HUSPRGRPID win;
    HUSPRGRPID grp;
    MGSCORE *score;
} SCOREPLAYER;

typedef struct {
    s32 state;
    f32 timer;
    SCOREPLAYER p[4];
} SCOREDATA;

void fn_1_7A84(OMOBJ *obj);
void fn_1_7AC0(OMOBJ *obj);
void fn_1_7AE8(OMOBJ *obj);
void fn_1_7E04(OMOBJ *obj);
s32 fn_1_1850(void);
s32 fn_1_485C(u32 idx);

static f32 lbl_1_data_2C0[4][2] = {
    71.0f, 59.0f,
    191.0f, 59.0f,
    385.0f, 59.0f,
    505.0f, 59.0f
};

static s32 lbl_1_data_2E0[15] = {
    DATANUM(DATA_mgconst, 0x20), DATANUM(DATA_mgconst, 0x21), DATANUM(DATA_mgconst, 0x22),
    DATANUM(DATA_mgconst, 0x23), DATANUM(DATA_mgconst, 0x24), DATANUM(DATA_mgconst, 0x26),
    DATANUM(DATA_mgconst, 0x27), DATANUM(DATA_mgconst, 0x28), DATANUM(DATA_mgconst, 0x29),
    DATANUM(DATA_mgconst, 0x2A), DATANUM(DATA_mgconst, 0x2B), DATANUM(DATA_mgconst, 0x2C),
    DATANUM(DATA_mgconst, 0x2D), DATANUM(DATA_mgconst, 0x2A), DATANUM(DATA_mgconst, 0x2A)
};

static f32 lbl_1_rodata_530 = 22.0f;
static f32 lbl_1_rodata_534 = -38.0f;
static f32 lbl_1_rodata_538 = 0.0f;
static f32 lbl_1_rodata_53C = -15.0f;
static f32 lbl_1_rodata_540 = 4.0f;
static f64 lbl_1_rodata_548 = 0.1;
extern f64 lbl_1_rodata_550;
extern f64 lbl_1_rodata_558;

static OMOBJ *lbl_1_bss_48;

void fn_1_79F8(OMOBJMAN *objman)
{
    OMOBJ *obj;
    SCOREDATA *data;

    obj = omAddObjEx(objman, 0x3C, 0, 0, OM_GRP_NONE, fn_1_7A84);
    lbl_1_bss_48 = obj;
    data = HuMemDirectMallocNum(HEAP_HEAP, 0x38, 0x1000);
    obj->data = data;
    memset(data, 0, 0x38);
}

void fn_1_7A80(void)
{
}

void fn_1_7A84(OMOBJ *obj)
{
    fn_1_7AE8(obj);
    obj->objFunc = fn_1_7AC0;
}

void fn_1_7AC0(OMOBJ *obj)
{
    fn_1_7E04(obj);
}

void fn_1_7AE8(OMOBJ *obj)
{
    SCOREDATA *data;
    ANIMDATA *animBase;
    ANIMDATA *animNum;
    ANIMDATA *animChar;
    HUSPRGRPID win;
    MGSCORE *score;
    HUSPRGRPID grp;
    s32 i;
    s32 j;

    data = obj->data;
    animBase = HuSprAnimRead(HuDataSelHeapReadNum(DATANUM(DATA_m516, 0x12), 0x1000, HEAP_MODEL));
    animNum = HuSprAnimRead(HuDataSelHeapReadNum(DATANUM(DATA_mgconst, 0x30), 0x1000, HEAP_MODEL));

    for (i = 0; i < 4; i++) {
        win = MgScoreWinCreate(0x70, 0x28);
        data->p[i].win = win;
        MgScoreWinPosSet(win, lbl_1_data_2C0[i][0], lbl_1_data_2C0[i][1]);
        MgScoreWinDispSet(win, 0);
        score = MgScoreCreate(DATANUM(DATA_mgconst, 0x31), -1, 1);
        data->p[i].score = score;
        MgScoreMaxDigitSet(score, 2);
        MgScoreDispOff(score);
        MgScorePosSet(score, lbl_1_rodata_530 + lbl_1_data_2C0[i][0], lbl_1_data_2C0[i][1]);
        MgScoreColorSet(score, 0xFF, 0xD8, 0x15);
        MgScorePriSet(score, 0xA);
        animChar = HuSprAnimRead(HuDataSelHeapReadNum(lbl_1_data_2E0[GwPlayerConf[i].charNo], 0x1000, HEAP_MODEL));
        grp = HuSprGrpCreate(3);
        data->p[i].grp = grp;
        HuSprGrpPosSet(grp, lbl_1_data_2C0[i][0], lbl_1_data_2C0[i][1]);
        HuSprGrpMemberSet(grp, 0, HuSprCreate(animChar, 0x32, 0));
        HuSprPosSet(grp, 0, lbl_1_rodata_534, lbl_1_rodata_538);
        HuSprGrpMemberSet(grp, 1, HuSprCreate(animBase, 0x32, 0));
        HuSprPosSet(grp, 1, lbl_1_rodata_53C, lbl_1_rodata_538);
        HuSprGrpMemberSet(grp, 2, HuSprCreate(animNum, 0x32, 0));
        HuSprPosSet(grp, 2, lbl_1_rodata_540, lbl_1_rodata_538);
        HuSprBankSet(grp, 2, 0xF);
        HuSprColorSet(grp, 2, 0xFF, 0xFF, 0xFF);

        for (j = 0; j < 3; j++) {
            HuSprAttrSet(grp, j, 1);
            HuSprAttrSet(grp, j, 4);
        }
    }
}

void fn_1_7E04(OMOBJ *obj)
{
    SCOREDATA *data;
    HUSPRGRPID grp;
    s32 val;
    s32 i;
    s32 j;

    data = obj->data;

    switch (data->state) {
    case 0:
    case 2:
        if (fn_1_1850() != 2) {
            break;
        }
        data->state++;
        data->timer = 0.0f;
        for (i = 0; i < 4; i++) {
            MgScoreWinDispSet(data->p[i].win, 1);
            MgScoreDispOn(data->p[i].score);
            for (j = 0; j < 3; j++) {
                HuSprAttrReset(data->p[i].grp, j, 4);
            }
        }
        /* fall through */
    case 1:
        data->timer = (f64)data->timer + lbl_1_rodata_548;
        if (data->timer >= lbl_1_rodata_550) {
            data->state++;
        }
        for (i = 0; i < 4; i++) {
            MgScoreWinTPLvlSet(data->p[i].win, lbl_1_rodata_558 * (f64)data->timer);
            MgScoreTPLvlSet(data->p[i].score, data->timer);
            for (j = 0; j < 3; j++) {
                HuSprTPLvlSet(data->p[i].grp, j, data->timer);
            }
        }
        break;
    }

    for (i = 0; i < 4; i++) {
        grp = data->p[i].grp;
        val = data->p[i].value;
        if (val > 0x63) {
            val = 0x63;
        }
        MgScoreValueSet(data->p[i].score, val);
    }
}

void fn_1_800C(u32 player, s32 value)
{
    SCOREDATA *data;
    s32 idx;

    data = lbl_1_bss_48->data;
    if (player != 0U) {
        idx = fn_1_485C(player);
        data->p[idx].value += value;
    }
}

s32 fn_1_8088(s32 idx)
{
    SCOREDATA *data;

    data = lbl_1_bss_48->data;
    return data->p[idx].value;
}