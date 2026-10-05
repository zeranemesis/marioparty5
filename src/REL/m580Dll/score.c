#include "types.h"

u32 MgSeqModeGet(void);

typedef struct M580ScoreEntry_s {
    /* 0x00 */ s32 val0;
    /* 0x04 */ s32 val4;
    /* 0x08 */ s32 val8;
} M580_SCOREENTRY;

typedef struct M580ScoreGoal_s {
    /* 0x00 */ s32 flag;
    /* 0x04 */ u8 pad04[8];
    /* 0x0C */ s32 prev;
    /* 0x10 */ s32 cur;
    /* 0x14 */ s32 arg0;
    /* 0x18 */ s32 arg1;
    /* 0x1C */ s32 arg2;
} M580_SCOREGOAL;

typedef struct M580ScoreData_s {
    /* 0x00 */ u8 pad00[0x10];
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 pad14[0x10];
    /* 0x24 */ M580_SCOREENTRY entries[2];
    /* 0x3C */ M580_SCOREGOAL goal;
} M580_SCOREDATA;

typedef struct M580Score_s M580_SCORE;

struct M580Score_s {
    /* 0x00 */ u8 pad00[0x14];
    /* 0x14 */ void (*unk14)(M580_SCORE *);
    /* 0x18 */ u8 pad18[0x44];
    /* 0x5C */ M580_SCOREDATA *data;
};

void fn_1_EF0C(M580_SCORE *score);
void fn_1_EF4C(M580_SCORE *score);
void fn_1_F254(M580_SCORE *score);
void fn_1_108DC(M580_SCORE *score);
void fn_1_F0B8(M580_SCORE *score);
void fn_1_F758(M580_SCORE *score);
void fn_1_10C44(M580_SCORE *score);

extern M580_SCORE *lbl_1_bss_250;

void fn_1_EEC0(M580_SCORE *score)
{
    fn_1_EF4C(score);
    fn_1_F254(score);
    fn_1_108DC(score);
    score->unk14 = fn_1_EF0C;
}

void fn_1_EF0C(M580_SCORE *score)
{
    fn_1_F0B8(score);
    fn_1_F758(score);
    fn_1_10C44(score);
}

void fn_1_10880(s32 arg0, s32 arg1, s32 arg2)
{
    M580_SCORE *score = lbl_1_bss_250;
    M580_SCOREDATA *data = score->data;
    M580_SCOREGOAL *goal = &data->goal;

    goal->flag = 1;
    goal->prev = goal->arg0;
    goal->cur = goal->arg1;
    goal->arg0 = arg0;
    goal->arg1 = arg1;
    goal->arg2 = arg2;
}

s32 fn_1_1100C(s32 index)
{
    M580_SCORE *score = lbl_1_bss_250;
    M580_SCOREDATA *data = score->data;

    return data->entries[index].val0;
}

s32 fn_1_1104C(void)
{
    M580_SCORE *score = lbl_1_bss_250;
    M580_SCOREDATA *data = score->data;

    if (MgSeqModeGet() == 6) {
        data->unk10 = 0;
    }
    return data->unk10;
}