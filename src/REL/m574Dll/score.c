#include "REL/m574Dll.h"
#include "game/audio.h"
#include "game/data.h"
#include "game/mg/score.h"
#include "game/mg/seqman.h"

typedef struct M574ScoreSide_s {
    /* 0x00 */ s32 target;
    /* 0x04 */ s32 shown;
    /* 0x08 */ s32 timer;
    /* 0x0C */ s32 add;
    /* 0x10 */ s16 animId[5];
    /* 0x1A */ u8 pad1A[2];
} M574_SCORE_SIDE;

typedef struct M574ScoreWork_s {
    /* 0x00 */ M574_SCORE_SIDE side[2];
    /* 0x38 */ s32 state;
    /* 0x3C */ s32 counter;
    /* 0x40 */ s32 record;
    /* 0x44 */ s16 winId;
    /* 0x46 */ s16 sprGrp;
    /* 0x48 */ MGSCORE *score;
} M574_SCORE_WORK;

s32 fn_1_1F28(s32 grp);
s32 fn_1_940(u32 grp);

static void fn_1_71BC(OMOBJ *obj);
static void fn_1_7200(OMOBJ *obj);
static void fn_1_7238(OMOBJ *obj);
static void fn_1_7328(OMOBJ *obj);
static void fn_1_7468(OMOBJ *obj);
static void fn_1_75EC(OMOBJ *obj);

static f32 scorePos[2] = { 288.0f, 58.0f };

static s32 scoreAddTbl[13] = {
    0x1F4, 0x3E8, 0x7D0, 0xA, 0x32, 0x64, 0x64, 0x32, 0x32, 0x64, 0xA, 0x14, 0xC8,
};

static char digitNames[11][4] = {
    "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
};

static OMOBJ *scoreObj;

void fn_1_7130(OMOBJMAN *om)
{
    OMOBJ *obj;
    void *data;

    obj = omAddObjEx(om, 0x46, 1, 0, -1, fn_1_71BC);
    scoreObj = obj;
    data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M574_SCORE_WORK), HU_MEMNUM_OVL);
    obj->data = data;
    memset(data, 0, sizeof(M574_SCORE_WORK));
}

void fn_1_71B8(void)
{
}

static void fn_1_71BC(OMOBJ *obj)
{
    fn_1_7238(obj);
    fn_1_7468(obj);
    obj->objFunc = fn_1_7200;
}

static void fn_1_7200(OMOBJ *obj)
{
    fn_1_7328(obj);
    fn_1_75EC(obj);
}

static void fn_1_7238(OMOBJ *obj)
{
    M574_SCORE_WORK *work;
    M574_SCORE_SIDE *side;
    void *anim;
    s16 model;
    s32 i;
    s32 j;

    work = obj->data;
    anim = HuDataSelHeapReadNum(0x81001F, HU_MEMNUM_OVL, HEAP_MODEL);
    model = Hu3DModelCreate(HuDataSelHeapReadNum(0x81001E, HU_MEMNUM_OVL, HEAP_MODEL));
    obj->mdlId[0] = model;
    Hu3DModelLayerSet(model, 4);
    for (i = 0; i < 2; i++) {
        side = &work->side[i];
        for (j = 0; j < 5; j++) {
            side->animId[j] = Hu3DAnimCreate(anim, model, (char *)digitNames + i * 0x14 + j * 4);
            Hu3DAnimAttrSet(side->animId[j], 1);
        }
    }
}

static void fn_1_7328(OMOBJ *obj)
{
    M574_SCORE_WORK *work;
    M574_SCORE_SIDE *side;
    s32 blink;
    s32 val;
    s32 i;
    s32 j;
    s32 div;

    work = obj->data;
    for (i = 0; i < 2; i++) {
        side = &work->side[i];
        if (side->timer != 0) {
            side->timer--;
            blink = (side->timer & 0xF) < 6 ? 0 : 1;
            val = side->add;
        }
        else {
            blink = 0;
            val = side->shown;
            val += (side->target - val > 0x64) ? 0x28 : 0xA;
            if (val > side->target) {
                val = side->target;
            }
            side->shown = val;
        }
        if (val > 99999) {
            val = 99999;
        }
        div = 10000;
        for (j = 0; j < 5; j++) {
            Hu3DAnimBankSet(side->animId[j], (u16)(blink + (val / div) * 2));
            val %= div;
            div /= 10;
        }
    }
}

static void fn_1_7468(OMOBJ *obj)
{
    M574_SCORE_WORK *work;
    MGSCORE *score;
    s16 win;
    s16 grp;
    ANIMDATA *anim;

    work = obj->data;
    work->state = 0;
    work->record = GWMgRecordGet(9);
    win = MgScoreWinCreate(0x78, 0x28);
    work->winId = win;
    MgScoreWinPosSet(win, scorePos[0], scorePos[1]);
    score = MgScoreCreate(0x960030, -1, 1);
    work->score = score;
    MgScoreMaxDigitSet(score, 5);
    MgScorePosSet(score, scorePos[0] - 22.0f, scorePos[1]);
    MgScoreColorSet(score, 0x42, 0xFF, 0x7A);
    MgScoreValueSet(score, work->record);
    grp = HuSprGrpCreate(1);
    work->sprGrp = grp;
    HuSprGrpPosSet(grp, scorePos[0], scorePos[1]);
    anim = HuSprAnimRead(HuDataSelHeapReadNum(0x960045, HU_MEMNUM_OVL, HEAP_MODEL));
    HuSprGrpMemberSet(grp, 0, HuSprCreate(anim, 0x32, 0));
    HuSprPosSet(grp, 0, -42.0f, 0.0f);
}

static void fn_1_75EC(OMOBJ *obj)
{
    M574_SCORE_WORK *work;
    f32 scale;

    work = obj->data;
    scale = 1.0f;
    switch (work->state) {
        case 0:
            break;
        case 1:
            if (MgSeqModeGet() == 7) {
                work->state++;
            }
            break;
        case 2:
            if (MgSeqFrameNoGet() >= 0x5A) {
                work->state++;
                work->counter = 0;
                MgScoreValueSet(work->score, work->record);
            }
            break;
        case 3:
            scale = 1.0 + 0.02 * (f32)++work->counter;
            if (work->counter >= 0xA) {
                work->state++;
                work->counter = 0;
            }
            break;
        case 4:
            scale = 1.2 - 0.04 * (f32)++work->counter;
            if (work->counter >= 5) {
                work->state--;
                work->counter = 0;
            }
            break;
    }
    if (work->state != 0) {
        MgScoreDigitScaleSet(work->score, scale, scale);
        HuSprScaleSet(work->sprGrp, 0, scale, scale);
    }
}

void fn_1_77D4(s32 grp, s32 type)
{
    M574_SCORE_WORK *work;
    s32 val;
    s32 *target;

    if (MgSeqModeGet() == 5) {
        work = scoreObj->data;
        target = &work->side[grp].target;
        val = scoreAddTbl[type];
        val += *target;
        if (val > 99999) {
            val = 99999;
        }
        *target = val;
    }
}

void fn_1_7874(s32 grp, s32 type)
{
    M574_SCORE_WORK *work;
    M574_SCORE_WORK *work2;
    M574_SCORE_SIDE *side;
    s32 val;
    s32 *target;

    if (MgSeqModeGet() == 5) {
        work = scoreObj->data;
        side = &work->side[grp];
        side->add = scoreAddTbl[type];
        side->timer = 0x3C;
        if (MgSeqModeGet() == 5) {
            work2 = scoreObj->data;
            target = &work2->side[grp].target;
            val = scoreAddTbl[type];
        val += *target;
            if (val > 99999) {
                val = 99999;
            }
            *target = val;
        }
    }
}

s32 fn_1_7948(s32 grp)
{
    OMOBJ *obj = scoreObj;
    M574_SCORE_WORK *work = obj->data;

    return work->side[grp].target;
}

s32 fn_1_797C(void)
{
    M574_SCORE_WORK *work;
    s32 best;
    s32 i;

    work = scoreObj->data;
    best = 0;
    for (i = 0; i < 2; i++) {
        if (fn_1_940(i) != -1 && fn_1_1F28(i) != 0 && work->side[i].target > best) {
            best = work->side[i].target;
        }
    }
    if (best > work->record) {
        MgSeqRecordSet(best);
        GWMgRecordSet(9, best);
        work->record = best;
        work->state = 1;
        return 1;
    }
    return 0;
}
