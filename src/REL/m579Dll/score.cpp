#include "dolphin.h"
#include "game/data.h"
#include "game/sprite.h"
#include "REL/m579Dll/m579objs.h"

#ifdef __MWERKS__
#define M579_RODATA __declspec(section ".rodata")
#else
#define M579_RODATA
#endif

extern "C" {
s16 MgScoreWinCreate(s16 sizeX, s16 sizeY);
void MgScoreWinColorSet(s16 gid, u8 r, u8 g, u8 b);
void MgScoreWinTPLvlSet(s16 gid, f32 tpLvl);
void MgScoreWinPosSet(s16 gid, f32 x, f32 y);
void MgScoreWinDispSet(s16 gid, s32 disp);
MGSCORE *MgScoreCreate(s32 digitFile, s32 unitFile, s32 dispLeadZeroF);
void MgScoreValueSet(MGSCORE *score, s32 value);
void MgScorePosSet(MGSCORE *score, f32 x, f32 y);
void MgScoreTPLvlSet(MGSCORE *score, f32 tpLvl);
void MgScoreColorSet(MGSCORE *score, u8 r, u8 g, u8 b);
void MgScoreMaxDigitSet(MGSCORE *score, s32 maxDigit);
void MgScoreDispOn(MGSCORE *score);
void MgScoreDispOff(MGSCORE *score);

extern void *lbl_1_bss_C;
extern void *lbl_1_data_1FC;
extern M579Score *lbl_1_bss_A2C;

M579Score *fn_1_143D4(M579Score *self, s16 flag);
void fn_1_147A4(M579Score *self, s32 bit);
M579Score *fn_1_147E0(M579Score *self, s16 flag);
void fn_1_14684(M579Score *self);

char lbl_1_data_5C0[16] = "ms_pointer == 0";
char lbl_1_data_5D0[12] = "singleton.h";
char lbl_1_data_5DC[16] = "ms_pointer != 0";
s32 lbl_1_data_5EC[15] = { 0x960020, 0x960021, 0x960022, 0x960023, 0x960024,
                           0x960026, 0x960027, 0x960028, 0x960029, 0x96002A,
                           0x96002B, 0x96002C, 0x96002D, 0x96002A, 0x96002A };
void *lbl_1_data_628[6] = { 0, 0, (void *) fn_1_147E0, (void *) fn_1_147A4, (void *) fn_1_14684, 0 };

M579_RODATA const f32 lbl_1_rodata_6F8[4][2] = { { 55.0f, 59.0f }, { 521.0f, 59.0f }, { 55.0f, 422.0f }, { 521.0f, 422.0f } };

M579Score *fn_1_14208(M579Score *self)
{
    s32 i;
    M579ScoreEntry *e;
    MGSCORE *score;

    fn_1_1264(self);
    self->vtable = (void **) &lbl_1_data_1FC;
    self->flags = 0;
    fn_1_1330(self, &lbl_1_bss_C);
    if (lbl_1_bss_A2C == 0) {
    } else {
        fn_1_1D4(lbl_1_data_5C0, lbl_1_data_5D0, 0x35);
    }
    lbl_1_bss_A2C = self;
    self->vtable = lbl_1_data_628;
    self->active = 0;
    for (i = 0; i < 4; i++) {
        e = &self->entry[i];
        e->win = MgScoreWinCreate(0x50, 0x28);
        MgScoreWinColorSet(e->win, 0x40, 0x40, 0x40);
        MgScoreWinTPLvlSet(e->win, 0.8f);
        MgScoreWinDispSet(e->win, 0);
        e->grp = HuSprGrpCreate(1);
        score = MgScoreCreate(0x960031, -1, 1);
        e->score = score;
        MgScoreMaxDigitSet(score, 2);
        MgScoreDispOff(score);
        MgScorePosSet(score, 3.0f + lbl_1_rodata_6F8[i][0], lbl_1_rodata_6F8[i][1]);
        MgScoreColorSet(score, 0xFF, 0xD8, 0x15);
        e->tpLvl = 0.0f;
    }
    return self;
}

M579Score *fn_1_143D4(M579Score *self, s16 flag)
{
    if (self != 0) {
        if (lbl_1_bss_A2C != 0) {
        } else {
            fn_1_1D4(lbl_1_data_5DC, lbl_1_data_5D0, 0x3D);
        }
        lbl_1_bss_A2C = 0;
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_14460(M579Score *self, s32 idx, s32 type)
{
    M579ScoreEntry *e = &self->entry[idx];

    e->type = type;
    e->value = 0;
    e->x = lbl_1_rodata_6F8[idx][0];
    e->y = lbl_1_rodata_6F8[idx][1];
    MgScoreWinPosSet(e->win, e->x, e->y);
    HuSprGrpMemberSet(e->grp, 0, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(lbl_1_data_5EC[type], 0x10000000, HEAP_MODEL)), 0xC, 0));
    HuSprPosSet(e->grp, 0, -21.0f, 0.0f);
    HuSprAttrSet(e->grp, 0, 4);
    HuSprGrpPosSet(e->grp, e->x, e->y);
}

void fn_1_14590(M579Score *self, s32 idx, s32 value)
{
    self->entry[idx].value = value;
}

void fn_1_145A0(M579ScoreEntry *e)
{
    f32 tp;

    e->tpLvl = (f32) ((f64) e->tpLvl + 0.1);
    if (e->tpLvl > 1.0f) {
        e->tpLvl = 1.0f;
    }
    tp = e->tpLvl;
    MgScoreWinDispSet(e->win, 1);
    MgScoreWinTPLvlSet(e->win, (f32) (0.8 * (f64) tp));
    MgScoreValueSet(e->score, e->value);
    MgScoreDispOn(e->score);
    MgScoreTPLvlSet(e->score, tp);
    HuSprAttrReset(e->grp, 0, 4);
    HuSprTPLvlSet(e->grp, 0, tp);
}

void fn_1_14684(M579Score *self)
{
    s32 i;

    if ((u8) self->active != 0) {
        for (i = 0; i < 4; i++) {
            fn_1_145A0(&self->entry[i]);
        }
    }
}

void fn_1_147A4(M579Score *self, s32 bit)
{
    self->flags = self->flags & ~(1 << bit);
    switch (bit) {
    case 0:
    case 1:
        break;
    case 2:
        self->active = 1;
        break;
    }
}

M579Score *fn_1_147E0(M579Score *self, s16 flag)
{
    if (self != 0) {
        self->vtable = lbl_1_data_628;
        if (self != 0) {
            if (lbl_1_bss_A2C != 0) {
            } else {
                fn_1_1D4(lbl_1_data_5DC, lbl_1_data_5D0, 0x3D);
            }
            lbl_1_bss_A2C = 0;
        }
        if (self != 0) {
            self->vtable = (void **) &lbl_1_data_1FC;
            if (self != 0) {
                fn_1_1270(self, 0);
            }
        }
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}
}
