#include "REL/m539Dll.h"

/*
 * m539's "scorespr": the little number that flies off a player when their
 * score changes.
 *
 * Two variants exist, picked when the object is made: one pops up, growing
 * and drifting, and one rises while fading out. Both are driven from a
 * pointer-to-member-function hook stored in the object, and a process keeps
 * a list of the live ones.
 */

extern "C" {

typedef struct {
    s32 ptmfA;
    s32 ptmfB;
    void *ptmfFn;
} PTMF;

typedef struct M539ScoreMgr_s {
    s32 unk0;
    s32 unk4;
    void *vtable;
    s32 flags;
} M539ScoreMgr;

typedef struct M539ScoreSpr_s {
    s32 unk0;
    s32 unk4;
    f32 x;
    f32 y;
    f32 z;
    s32 value;
    s32 frame;
    void *score;
    PTMF hook;
} M539ScoreSpr;

typedef struct M539ScoreList_s {
    void *head;
    void *tail;
    void *unk8;
} M539ScoreList;

extern void *lbl_1_data_1FC;
extern s32 lbl_1_bss_C;
extern void *lbl_1_bss_A0C;

char lbl_1_data_540[] = "ms_pointer == 0";
char lbl_1_data_550[] = "singleton.h";
char lbl_1_data_55C[] = "ms_pointer != 0";
s32 lbl_1_data_56C[3] = {0, -1, 0};
s32 lbl_1_data_578[3] = {0, -1, 0};
void *lbl_1_data_584[5];

s32 lbl_1_bss_A58[3];
M539ScoreList lbl_1_bss_A64;

void __ptmf_scall(void *obj, PTMF *ptmf);
void __register_global_object(void *obj, void *dtor, void *reg);

void fn_1_1264(void *self);
void fn_1_1270(void *self, s32 arg);
void fn_1_12DC(void *self);
void fn_1_1330(void *self, s32 *arg);
void fn_1_1360(void *list, void **node);
void fn_1_1D4(const char *msg, const char *file, s32 line);
void fn_1_C8(void *ptr);

void *MgScoreCreate(s32 digitFile, s32 unitFile, s32 dispLeadZero);
void MgScoreKill(void *score);
void MgScoreMaxDigitSet(void *score, s32 maxDigit);
void MgScoreDispOff(void *score);
void MgScoreDispOn(void *score);
void MgScorePosSet(void *score, f32 x, f32 y);
void MgScoreScaleSet(void *score, f32 x, f32 y);
void MgScoreColorSet(void *score, u8 r, u8 g, u8 b);
void MgScoreValueSet(void *score, s32 value);
void MgScoreTPLvlSet(void *score, f32 tpLvl);

M539ScoreSpr *fn_1_F40C(M539ScoreSpr *self, s16 arg);
void fn_1_F480(void);
void fn_1_F55C(void);
void fn_1_F58C(void *self);
void fn_1_F5C4(M539ScoreSpr *self);
void fn_1_F744(M539ScoreSpr *self);
void fn_1_F914(M539ScoreList *list, void (*fn)(void *));
M539ScoreSpr *fn_1_FA04(M539ScoreSpr *self, s16 arg);
M539ScoreSpr *fn_1_F258(M539ScoreSpr *self, Vec *pos, s32 value, s32 kind, s32 arg);

// NONMATCHING: register allocation only.
M539ScoreMgr *fn_1_EFF4(M539ScoreMgr *self)
{
    fn_1_1264(self);
    self->vtable = &lbl_1_data_1FC;
    self->flags = 0;
    fn_1_1330(self, &lbl_1_bss_C);
    if (lbl_1_bss_A0C != NULL) {
        fn_1_1D4(lbl_1_data_540, lbl_1_data_550, 0x35);
    }
    lbl_1_bss_A0C = self;
    self->vtable = lbl_1_data_584;
    return self;
}

// NONMATCHING: register allocation only.
M539ScoreMgr *fn_1_F0C4(M539ScoreMgr *self, s16 arg)
{
    s16 mode = arg;

    if (self != NULL) {
        if (lbl_1_bss_A0C == NULL) {
            fn_1_1D4(lbl_1_data_55C, lbl_1_data_550, 0x3D);
        }
        lbl_1_bss_A0C = NULL;
        if (mode > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

// NONMATCHING: register allocation only.
M539ScoreMgr *fn_1_F150(M539ScoreMgr *self, s16 arg)
{
    s16 mode = arg;

    if (self != NULL) {
        self->vtable = lbl_1_data_584;
        fn_1_F480();
        if (self != NULL) {
            if (lbl_1_bss_A0C == NULL) {
                fn_1_1D4(lbl_1_data_55C, lbl_1_data_550, 0x3D);
            }
            lbl_1_bss_A0C = NULL;
        }
        if (self != NULL) {
            self->vtable = &lbl_1_data_1FC;
            fn_1_1270(self, 0);
        }
        if (mode > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_F220(M539ScoreMgr *self, s32 arg)
{
    self->flags = self->flags & ~(1 << arg);
}

void fn_1_F238(void)
{
    fn_1_F55C();
}

// NONMATCHING: register allocation only.
M539ScoreSpr *fn_1_F258(M539ScoreSpr *self, Vec *pos, s32 value, s32 kind, s32 arg)
{
    s32 v = value;
    s32 *hook;
    s32 digits;

    fn_1_1264(self);
    self->x = pos->x;
    self->y = pos->y;
    self->z = pos->z;
    self->value = v;
    self->frame = 0;
    fn_1_1330(self, (s32 *)&lbl_1_bss_A64);
    self->score = MgScoreCreate(0x960032, -1, 0);
    if (self->score == NULL) {
        return fn_1_F40C(self, 1);
    }
    if (arg == 0) {
        hook = lbl_1_data_56C;
    }
    else {
        hook = lbl_1_data_578;
    }
    self->hook.ptmfA = hook[0];
    self->hook.ptmfB = hook[1];
    self->hook.ptmfFn = (void *)hook[2];
    if (arg == 1) {
        digits = 2;
    }
    else {
        digits = 3;
    }
    MgScoreMaxDigitSet(self->score, digits);
    MgScoreDispOff(self->score);
    MgScorePosSet(self->score, self->x, self->y);
    MgScoreScaleSet(self->score, 1.0f, 1.0f);
    MgScoreColorSet(self->score, 0xFF, 0xD8, 0x15);
    return self;
}

// NONMATCHING: register allocation only.
M539ScoreSpr *fn_1_F3B4(M539ScoreSpr *self, s16 arg)
{
    s16 mode = arg;

    if (self != NULL) {
        fn_1_1270(self, 0);
        if (mode > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

// NONMATCHING: register allocation only.
M539ScoreSpr *fn_1_F40C(M539ScoreSpr *self, s16 arg)
{
    s16 mode = arg;

    if (self != NULL) {
        if (self->score != NULL) {
            MgScoreKill(self->score);
        }
        fn_1_1270(self, 0);
        if (mode > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

// NONMATCHING: register allocation only.
void fn_1_F480(void)
{
    M539ScoreSpr *node;

    for (;;) {
        if (lbl_1_bss_A64.head == &lbl_1_bss_A64 && lbl_1_bss_A64.head == lbl_1_bss_A64.tail) {
            break;
        }
        node = (M539ScoreSpr *)lbl_1_bss_A64.tail;
        if (node != NULL) {
            if (node->score != NULL) {
                MgScoreKill(node->score);
            }
            fn_1_1270(node, 0);
            fn_1_C8(node);
        }
    }
}

void fn_1_F55C(void)
{
    fn_1_F914(&lbl_1_bss_A64, fn_1_F58C);
}

void fn_1_F58C(void *self)
{
    __ptmf_scall(self, &((M539ScoreSpr *)self)->hook);
}

// NONMATCHING: register allocation only.
void fn_1_F5C4(M539ScoreSpr *self)
{
    f32 t = self->frame / 60.0f;
    f32 scale;

    if ((1.0f - (t * t)) < 0.0f) {
    }
    MgScoreDispOn(self->score);
    MgScoreValueSet(self->score, self->value);
    MgScorePosSet(self->score, self->x - (2.0f * t), self->y - t);
    scale = 1.0f + (2.0f * t);
    MgScoreScaleSet(self->score, scale, scale);
    self->frame = self->frame + 1;
    if ((f32)self->frame > 30.0) {
        self->frame = 0x1E;
    }
}

// NONMATCHING: register allocation only.
void fn_1_F744(M539ScoreSpr *self)
{
    f32 t = self->frame / 60.0f;
    f32 tpLvl = 1.0f - (t * t);
    f32 scale;

    if (tpLvl < 0.0f) {
        tpLvl = 0.0f;
    }
    MgScoreDispOn(self->score);
    MgScoreValueSet(self->score, self->value);
    MgScoreTPLvlSet(self->score, tpLvl);
    MgScorePosSet(self->score, self->x - (2.0f * t), self->y + (80.0f * t));
    scale = 1.0f + (2.0f * t);
    MgScoreScaleSet(self->score, scale, scale);
    self->frame = self->frame + 1;
    if ((f32)self->frame >= 60.0f && self != NULL) {
        if (self->score != NULL) {
            MgScoreKill(self->score);
        }
        fn_1_1270(self, 0);
        fn_1_C8(self);
    }
}

// NONMATCHING: register allocation only.
void fn_1_F914(M539ScoreList *list, void (*fn)(void *))
{
    void *spC;
    void *end = list->unk8;

    if (list->head == list && list->head == list->tail) {
        return;
    }
    fn_1_1264(&spC);
    fn_1_1360(list, &spC);
    for (;;) {
        if (spC == &spC && spC == end) {
            break;
        }
        fn_1_12DC(spC);
        fn_1_1330(spC, (s32 *)list);
        fn(spC);
    }
    fn_1_1270(&spC, 0);
}

// NONMATCHING: register allocation only.
M539ScoreSpr *fn_1_FA04(M539ScoreSpr *self, s16 arg)
{
    s16 mode = arg;

    if (self != NULL) {
        fn_1_1270(self, 0);
        if (mode > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_FA5C(void)
{
    fn_1_1264(&lbl_1_bss_A64);
    __register_global_object(&lbl_1_bss_A64, (void *)fn_1_FA04, lbl_1_bss_A58);
}

}
