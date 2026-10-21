#include "game/data.h"
#include "game/memory.h"
#include "game/sprite.h"

/* Minimal view of the Mg timer record; only the fields this unit touches. */
struct MGTIMER {
    s32 unk0;
    u8 pad04[0x10];
    s32 unk14;
    u8 pad18[0x14];
    void *winGrpId;
    s32 winDispF;
    s32 stopF;
    s32 mode;
};

/* These are plain C in the original: m511.o defines them unmangled
   (m511.cpp is NonMatching, so the module links the original object).
   Left as C++ they mangle and the REL link fails with
   "Failed to find symbol fn_1_1D88__FPv in any module". */
extern "C" void *__dl__FPv(void *self);
extern "C" void fn_1_1D88(void *self);
extern "C" void fn_1_1D94(void *self, s32 arg1);
extern "C" void fn_1_1E54(void *self, void **arg1);
extern "C" void fn_1_CF8(char *cond, char *file, s32 line);

/* game/mg/score.h pulls in humath.h, so the Mg* API is declared here instead
   to keep the translation unit's rodata clean. */
extern "C" {

MGTIMER *MgTimerCreate(s32 type);
void MgTimerKill(MGTIMER *timer);
void MgTimerParamSet(MGTIMER *timer, s32 a, s32 b, s32 c);
void MgTimerPosSet(MGTIMER *timer, f32 x, f32 y);
void MgTimerWinColorSet(MGTIMER *timer, u32 r, u32 g, u32 b);
void MgTimerRecordDispOn(MGTIMER *timer);
void MgTimerModeOnSet(MGTIMER *timer, s32 mode);
s32 MgTimerDoneCheck(MGTIMER *timer);
s32 MgTimerValueGet(MGTIMER *timer);

void *MgScoreWinCreate(s32 a, s32 b);
void MgScoreWinColorSet(void *win, u32 r, u32 g, u32 b);
void MgScoreWinTPLvlSet(void *win, f32 tpLvl);
void MgScoreWinDispSet(void *win, s32 disp);
void MgScoreWinPosSet(void *win, f32 x, f32 y);

}

struct Mgr511ScoreBoard;
struct Mgr511Score;

void fn_1_15C24(Mgr511Score *arg0);
void fn_1_166E0(Mgr511Score *arg0, Mgr511ScoreBoard *arg1);
void fn_1_169DC(Mgr511ScoreBoard *arg1);
void fn_1_16B10(Mgr511ScoreBoard *arg1);

struct Mgr511Score {
    void *unk0;
    void **vtable;
    s32 unkC;
    s32 padC; /* 0x0C */
    MGTIMER *timer;
    s32 param;
    u8 hasTimer;
    u8 bigTimer;
};

struct Mgr511ScoreBoard {
    void *win;
    s16 sprGrp;
    s16 pad2;
    s32 unk4;
    s32 index;
    s32 count;
    s32 counter[5];
    s32 param;
    f32 posX;
    f32 posY;
    f32 alpha;
    u8 hasTimer;
    u8 pad35[3];
    MGTIMER *timer;
};

struct Mgr511ScoreRoot {
    void *unk0;
    void *unk4;
    void **vtable;
    s32 unkC;
    u8 active;
    u8 mode;
    u8 animIn;
    u8 pad13;
    Mgr511ScoreBoard board[4];
};

extern "C" {

extern void *lbl_1_bss_2C;

extern void *lbl_1_data_40[6];
extern void *lbl_1_data_62C[6];
extern void *lbl_1_data_640[6];
extern "C" void fn_1_710(void);

extern Mgr511Score *lbl_1_bss_A24;
extern Mgr511ScoreRoot *lbl_1_bss_A28;

static const s32 lbl_1_rodata_85C[5] = {0x450031, 0x450033, 0x450035, 0x450037, 0x450039};
static const s32 lbl_1_rodata_870[5] = {0x450032, 0x450034, 0x450036, 0x450038, 0x45003A};

static const struct {
    f32 x;
    f32 y;
} lbl_1_rodata_8A0[4] = {
    {76.0f, 218.0f},
    {500.0f, 218.0f},
    {76.0f, 424.0f},
    {500.0f, 424.0f},
};

// NONMATCHING: the vtable pointer differs.
Mgr511Score *fn_1_159B4(Mgr511Score *arg0, s32 arg1, u8 arg2)
{
    fn_1_1D88(arg0);
    arg0->vtable = &lbl_1_data_40[2];
    arg0->unkC = 0;
    fn_1_1E54(arg0, (void **)lbl_1_bss_2C);
    if (lbl_1_bss_A28 == NULL) {
        fn_1_CF8("ms_pointer == 0", "singleton.h", 0x35);
    }
    lbl_1_bss_A28 = (Mgr511ScoreRoot *)arg0;
    arg0->vtable = lbl_1_data_640;
    arg0->param = arg1;
    arg0->hasTimer = 0;
    arg0->bigTimer = arg2;
    if (arg0->bigTimer == 0) {
        fn_1_15C24(arg0);
    }
    return arg0;
}

void *fn_1_15AB8(void *arg0, s16 arg1)
{
    if (arg0 != NULL) {
        if (lbl_1_bss_A28 == NULL) {
            fn_1_CF8("ms_pointer != 0", "singleton.h", 0x3D);
        }
        lbl_1_bss_A28 = (Mgr511ScoreRoot *)NULL;
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

// NONMATCHING: the vtable pointer differs.
void *fn_1_15B44(Mgr511Score *arg0, s16 arg1)
{
    if (arg0 != NULL) {
        arg0->vtable = lbl_1_data_640;
        if (arg0->hasTimer != 0) {
            MgTimerKill(arg0->timer);
        }
        if (lbl_1_bss_A28 == NULL) {
            fn_1_CF8("ms_pointer != 0", "singleton.h", 0x3D);
        }
        lbl_1_bss_A28 = (Mgr511ScoreRoot *)NULL;
        arg0->vtable = &lbl_1_data_40[2];
        fn_1_1D94(arg0, 0);
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

void fn_1_15C24(Mgr511Score *arg0)
{
    if (arg0->bigTimer != 0) {
        arg0->timer = MgTimerCreate(2);
        MgTimerParamSet(arg0->timer, 0, 0x1518, arg0->param);
        MgTimerPosSet(arg0->timer, 292.0f, 58.0f);
    } else {
        arg0->timer = MgTimerCreate(1);
        MgTimerParamSet(arg0->timer, 0, 0x4650, arg0->param);
        MgTimerPosSet(arg0->timer, 292.0f, 70.0f);
    }
    MgTimerWinColorSet(arg0->timer, 0, 0, 0);
    MgTimerRecordDispOn(arg0->timer);
    arg0->hasTimer = 1;
}

void fn_1_15D00(Mgr511Score *arg0)
{
    if (arg0->bigTimer != 0) {
        arg0->timer = MgTimerCreate(2);
        MgTimerParamSet(arg0->timer, 0, 0x1518, arg0->param);
        MgTimerPosSet(arg0->timer, 292.0f, 58.0f);
        MgTimerWinColorSet(arg0->timer, 0, 0, 0);
        MgTimerRecordDispOn(arg0->timer);
        arg0->hasTimer = 1;
    }
}

void fn_1_15DE8(Mgr511Score *arg0)
{
    MgTimerModeOnSet(arg0->timer, 0);
}

void fn_1_15E1C(Mgr511Score *arg0)
{
    if (arg0->timer->unk14 == 0) {
        arg0->timer->unk14 = 1;
        arg0->timer->unk0 = 1;
    }
}

s32 fn_1_15E48(Mgr511Score *arg0)
{
    s32 var_r30;

    var_r30 = 0;
    if (MgTimerDoneCheck(arg0->timer) != 0 && arg0->hasTimer != 0) {
        var_r30 = 1;
    }
    return var_r30;
}

void fn_1_15EA0(Mgr511Score *arg0)
{
    MgTimerValueGet(arg0->timer);
}

void fn_1_15ED0(Mgr511Score *arg0, s32 arg1)
{
    s32 *flag = (s32 *)((u8 *)arg0 + 0xC);

    *flag &= ~(1 << arg1);
    switch (arg1) {
    case 1:
        if (arg0->bigTimer != 0) {
            arg0->timer = MgTimerCreate(2);
            MgTimerParamSet(arg0->timer, 0, 0x1518, arg0->param);
            MgTimerPosSet(arg0->timer, 292.0f, 58.0f);
            MgTimerWinColorSet(arg0->timer, 0, 0, 0);
            MgTimerRecordDispOn(arg0->timer);
            arg0->hasTimer = 1;
        }
        return;
    case 2:
        MgTimerModeOnSet(arg0->timer, 0);
        return;
    case 3:
        if (arg0->timer->stopF == 0) {
            arg0->timer->stopF = 1;
            arg0->timer->mode = 1;
        }
        return;
    }
}

// NONMATCHING: the sprite group construction is not reproduced.
Mgr511ScoreRoot *fn_1_16040(Mgr511ScoreRoot *arg0, u8 arg1, s32 arg_sp0)
{
    ANIMDATA *spC[5];
    ANIMDATA *sp20[5];
    Mgr511ScoreBoard *board;
    HUSPRGRPID grpId;
    s32 i;
    s32 j;

    fn_1_1D88(arg0);
    arg0->vtable = &lbl_1_data_40[2];
    arg0->unkC = 0;
    fn_1_1E54(arg0, (void **)lbl_1_bss_2C);
    if (lbl_1_bss_A24 == NULL) {
        fn_1_CF8("ms_pointer == 0", "singleton.h", 0x35);
    }
    lbl_1_bss_A24 = (Mgr511Score *)arg0;
    arg0->vtable = lbl_1_data_62C;
    arg0->active = 0;
    arg0->mode = arg1;
    arg0->animIn = 1;
    for (i = 0; i < 5; i++) {
        spC[i] = HuSprAnimRead(HuDataSelHeapReadNum(lbl_1_rodata_85C[i], HU_MEMNUM_OVL, HEAP_MODEL));
        sp20[i] = HuSprAnimRead(HuDataSelHeapReadNum(lbl_1_rodata_870[i], HU_MEMNUM_OVL, HEAP_MODEL));
    }
    for (i = 0; i < 4; i++) {
        board = &arg0->board[i];
        board->hasTimer = 0;
        board->alpha = 0.0f;
        board->win = MgScoreWinCreate(0x78, 0x20);
        MgScoreWinColorSet(board->win, 0x40, 0x40, 0x40);
        MgScoreWinTPLvlSet(board->win, 0.8f);
        MgScoreWinDispSet(board->win, 0);
        grpId = HuSprGrpCreate(0xA);
        board->sprGrp = (s16)grpId;
        for (j = 0; j < 5; j++) {
            HuSprGrpMemberSet(grpId, (s16)j, HuSprCreate(sp20[j], 0xD, 0));
            HuSprPosSet(grpId, (s16)j, -44.0f + (22.0f * (f32)j), 0.0f);
        }
        for (j = 0; j < 5; j++) {
            HuSprGrpMemberSet(grpId, (s16)(j + 5), HuSprCreate(spC[j], 0xB, 0));
            HuSprPosSet(grpId, (s16)(j + 5), -44.0f + (22.0f * (f32)j), 0.0f);
        }
        for (j = 0; j < 0xA; j++) {
            HuSprAttrSet(grpId, (s16)j, 1);
            HuSprAttrSet(grpId, (s16)j, 4);
        }
    }
    return arg0;
}

void *fn_1_16388(void *arg0, s16 arg1)
{
    if (arg0 != NULL) {
        if (lbl_1_bss_A24 == NULL) {
            fn_1_CF8("ms_pointer != 0", "singleton.h", 0x3D);
        }
        lbl_1_bss_A24 = (Mgr511Score *)NULL;
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

// NONMATCHING: the vtable pointer differs.
void *fn_1_16414(Mgr511ScoreRoot *arg0, s16 arg1)
{
    Mgr511ScoreBoard *board;
    s32 i;

    if (arg0 != NULL) {
        arg0->vtable = lbl_1_data_62C;
        for (i = 0; i < 4; i++) {
            board = &arg0->board[i];
            if (board->hasTimer != 0) {
                MgTimerKill(board->timer);
            }
            HuSprGrpKill(board->sprGrp);
        }
        if (lbl_1_bss_A24 == NULL) {
            fn_1_CF8("ms_pointer != 0", "singleton.h", 0x3D);
        }
        lbl_1_bss_A24 = (Mgr511Score *)NULL;
        arg0->vtable = &lbl_1_data_40[2];
        fn_1_1D94(arg0, 0);
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

void fn_1_1652C(Mgr511ScoreRoot *arg0, s32 arg1, s32 arg2)
{
    Mgr511ScoreBoard *board = &arg0->board[arg1];
    s32 i;

    board->index = arg1;
    board->unk4 = arg2;
    board->count = 0;
    board->posX = lbl_1_rodata_8A0[arg1].x;
    board->posY = lbl_1_rodata_8A0[arg1].y;
    HuSprGrpPosSet(board->sprGrp, board->posX, board->posY);
    MgScoreWinPosSet(board->win, board->posX, board->posY);
    for (i = 0; i < 5; i++) {
        board->counter[i] = 0x12;
    }
}

void fn_1_16604(void *arg0, s32 arg1, s32 arg2, s32 arg3)
{
    *(s32 *)((u8 *)arg0 + (arg1 * 0x3C) + 0x20) = arg2;
    *(void **)((u8 *)arg0 + (arg1 * 0x3C) + 0x38) = (void *)arg3;
}

void fn_1_16620(Mgr511ScoreBoard *arg1)
{
    s32 i;
    s32 temp_r0;

    for (i = 0; i < 5; i++) {
        if (i < arg1->count) {
            temp_r0 = arg1->counter[i] - 1;
            arg1->counter[i] = temp_r0;
            if (temp_r0 < 0) {
                arg1->counter[i] = 0;
            }
        } else {
            temp_r0 = arg1->counter[i] + 1;
            arg1->counter[i] = temp_r0;
            if ((f32)temp_r0 > 18.0f) {
                arg1->counter[i] = 0x12;
            }
        }
    }
}

// NONMATCHING: the float selects are folded differently.
void fn_1_166E0(Mgr511ScoreRoot *arg0, Mgr511ScoreBoard *arg1)
{
    f32 sp8;
    f32 temp_f1;
    f32 temp_f1_2;
    f32 temp_f31;
    s32 i;

    if (arg0->animIn != 0) {
        temp_f1 = (f32)((f64)arg1->alpha + 0.1);
        arg1->alpha = temp_f1;
        if (temp_f1 > 1.0f) {
            arg1->alpha = 1.0f;
        }
    } else {
        temp_f1_2 = (f32)((f64)arg1->alpha - 0.05);
        arg1->alpha = temp_f1_2;
        if (temp_f1_2 < 0.0f) {
            arg1->alpha = 0.0f;
        }
    }
    sp8 = arg1->alpha;
    if (arg1->alpha > 0.0f) {
        MgScoreWinDispSet(arg1->win, 1);
        MgScoreWinTPLvlSet(arg1->win, (f32)(0.8 * (f64)arg1->alpha));
        for (i = 0; i < 5; i++) {
            HuSprAttrReset(arg1->sprGrp, (s16)i, 4);
            HuSprTPLvlSet(arg1->sprGrp, (s16)i, arg1->alpha);
        }
    } else {
        MgScoreWinDispSet(arg1->win, 0);
        for (i = 0; i < 5; i++) {
            HuSprAttrSet(arg1->sprGrp, (s16)i, 4);
        }
    }
    for (i = 0; i < 5; i++) {
        if ((f32)arg1->counter[i] < 18.0f) {
            HuSprAttrReset(arg1->sprGrp, (s16)(i + 5), 4);
            temp_f31 = (f32)(((f64)(f32)((f64)arg1->counter[i] + 18.0)) / 18.0);
            HuSprScaleSet(arg1->sprGrp, (s16)(i + 5), temp_f31, temp_f31);
            HuSprTPLvlSet(arg1->sprGrp, (s16)(i + 5), (f32)(((f64)(f32)(18.0 - (f64)arg1->counter[i])) / 18.0) * arg1->alpha);
        } else {
            HuSprAttrSet(arg1->sprGrp, (s16)(i + 5), 4);
        }
    }
}

void fn_1_169DC(Mgr511ScoreBoard *arg1)
{
    if (arg1->hasTimer == 0) {
        arg1->timer = MgTimerCreate(2);
        MgTimerParamSet(arg1->timer, arg1->param, arg1->param, arg1->param);
        MgTimerModeOnSet(arg1->timer, 2);
        MgTimerRecordDispOn(arg1->timer);
        if (arg1->timer->stopF == 0) {
            arg1->timer->stopF = 1;
            arg1->timer->mode = 1;
        }
        MgTimerPosSet(arg1->timer, 10.0f + lbl_1_rodata_8A0[arg1->index].x, 8.0f + lbl_1_rodata_8A0[arg1->index].y);
        MgScoreWinDispSet(arg1->timer->winGrpId, 0);
        arg1->timer->winDispF = 0;
        arg1->hasTimer = 1;
    }
}

void fn_1_16B10(Mgr511ScoreBoard *arg1)
{
    s16 i;

    for (i = 0; i < 0xA; i++) {
        HuSprAttrSet(arg1->sprGrp, i, 1);
        HuSprAttrSet(arg1->sprGrp, i, 4);
    }
}

// NONMATCHING: the per-board body is not reproduced.
void fn_1_16B74(Mgr511ScoreRoot *arg0)
{
    Mgr511ScoreBoard *board;
    s32 i;
    s32 j;
    s32 temp_r0;

    if (arg0->active != 0) {
        for (i = 0; i < 4; i++) {
            board = &arg0->board[i];
            for (j = 0; j < 5; j++) {
                if (j < board->count) {
                    temp_r0 = board->counter[j] - 1;
                    board->counter[j] = temp_r0;
                    if (temp_r0 < 0) {
                        board->counter[j] = 0;
                    }
                } else {
                    temp_r0 = board->counter[j] + 1;
                    board->counter[j] = temp_r0;
                    if ((f32)temp_r0 > 18.0f) {
                        board->counter[j] = 0x12;
                    }
                }
            }
            if (arg0->mode != 0 && board->count > 5) {
                fn_1_169DC(board);
                fn_1_16B10(board);
            } else {
                fn_1_166E0(arg0, board);
            }
        }
    }
}

void fn_1_16DE4(Mgr511ScoreRoot *arg0, s32 arg1)
{
    s32 *flag = (s32 *)((u8 *)arg0 + 0xC);

    *flag &= ~(1 << arg1);
    switch (arg1) {
    case 2:
        arg0->active = 1;
        return;
    case 3:
        if (arg0->mode == 0) {
            arg0->animIn = 0;
        }
        return;
    }
}

}

/* The vtables this object defines: their entries are
   functions defined above, so the definitions sit here. */

/* lbl_1_data_62C is a vtable score.o itself defines (6 words at .data+0x2c), and this unit is linked from source, so it has to be a definition rather than an extern declaration: with only the declaration the REL link reported "Failed to find symbol lbl_1_data_62C in any module". 2 leading words are zero and the rest are ADDR32 relocations to functions in this object. The definition is at the end of the unit because its entries are functions defined further down. */
extern "C" void *lbl_1_data_62C[6] = {
    0,
    0,
    (void *) fn_1_16414,
    (void *) fn_1_16DE4,
    (void *) fn_1_16B74,
};

/* lbl_1_data_640 is a vtable score.o itself defines (6 words at .data+0x40), and this unit is linked from source, so it has to be a definition rather than an extern declaration: with only the declaration the REL link reported "Failed to find symbol lbl_1_data_640 in any module". 3 leading words are zero and the rest are ADDR32 relocations to functions in this object. The definition is at the end of the unit because its entries are functions defined further down. */
extern "C" void *lbl_1_data_640[6] = {
    0,
    0,
    (void *) fn_1_15B44,
    (void *) fn_1_15ED0,
    (void *) fn_1_710,
    0,
};
