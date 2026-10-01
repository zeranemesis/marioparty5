#include "REL/m575dll.h"
#include "string.h"

// Declared locally: the real headers (game/gamemes.h, game/mg/score.h,
// game/esprite.h) all pull in humath.h -> math.h, whose unused extern inline
// sqrtf would add _half/_three to this unit's .rodata. The original object has
// no such literals.
extern s16 MgScoreWinCreate(s16 sizeX, s16 sizeY);
extern void MgScoreWinColorSet(s16 gid, u8 r, u8 g, u8 b);
extern void MgScoreWinPosSet(s16 gid, float posX, float posY);
extern void MgScoreWinDispSet(s16 gid, BOOL disp);
extern struct MgScore_s *MgScoreCreate(int digitFile, int unitFile, BOOL dispLeadZeroF);
extern void MgScoreKill(struct MgScore_s *score);
extern void MgScoreValueSet(struct MgScore_s *score, int value);
extern void MgScorePosSet(struct MgScore_s *score, float posX, float posY);
extern void MgScoreMaxDigitSet(struct MgScore_s *score, int maxDigit);
extern void MgScoreDispOn(struct MgScore_s *score);
extern void MgScoreDispOff(struct MgScore_s *score);
extern s16 espEntry(unsigned int dataNum, s16 prio, s16 bank);
extern void espKill(s16 espId);
extern void espDispOn(s16 espId);
extern void espDispOff(s16 espId);
extern void espDrawNoSet(s16 espId, s16 drawNo);
extern void espPosSet(s16 espId, float posX, float posY);

static const f32 lbl_1_rodata_C8[1] = { 70.0f };
static const f32 lbl_1_rodata_CC[1] = { 60.0f };
static const f32 lbl_1_rodata_D0[1] = { 84.0f };
static const f32 lbl_1_rodata_D4[1] = { 38.0f };
static const f32 lbl_1_rodata_D8[1] = { 62.0f };
static const f32 lbl_1_rodata_DC[1] = { 502.0f };
static const f32 lbl_1_rodata_E0[1] = { 516.0f };
static const f32 lbl_1_rodata_E4[1] = { 470.0f };
static const f32 lbl_1_rodata_E8[2] = { 494.0f, 0.0f };

// NONMATCHING: one instruction differs - the original keeps the esp id in a
// register for the last espDispOff (mr r3, r29) while using the local there
// costs the register allocator its r29 slot (see the esp note above).
void fn_1_3A7C(M575Score *self, u16 side, s32 arg2)
{
    struct MgScore_s *score;
    s16 esp;

    memset(self, 0, sizeof(M575Score));
    self->unk0 = arg2;
    self->side = side;
    self->value = 0;
    self->winId = MgScoreWinCreate(0x70, 0x28);
    MgScoreWinDispSet(self->winId, FALSE);
    score = MgScoreCreate(0x960031, -1, TRUE);
    self->score = score;
    MgScoreMaxDigitSet(score, 2);
    MgScoreValueSet(score, 0);
    MgScoreDispOff(score);
    esp = espEntry(0x820019, 0, 0);
    self->espA = esp;
    espDrawNoSet(esp, 0);
    espDispOff(esp);
    esp = espEntry(0x960046, 0, 0);
    self->espB = esp;
    espDrawNoSet(esp, 0);
    espDispOff(self->espB);

    if (side == 0) {
        MgScoreWinPosSet(self->winId, lbl_1_rodata_C8[0], lbl_1_rodata_CC[0]);
        MgScoreWinColorSet(self->winId, 0xFA, 0, 0x1E);
        MgScorePosSet(score, lbl_1_rodata_D0[0], lbl_1_rodata_CC[0]);
        espPosSet(self->espA, lbl_1_rodata_D4[0], lbl_1_rodata_CC[0]);
        espPosSet(self->espB, lbl_1_rodata_D8[0], lbl_1_rodata_CC[0]);
    }
    else {
        MgScoreWinPosSet(self->winId, lbl_1_rodata_DC[0], lbl_1_rodata_CC[0]);
        MgScoreWinColorSet(self->winId, 0, 0x32, 0xFA);
        MgScorePosSet(score, lbl_1_rodata_E0[0], lbl_1_rodata_CC[0]);
        espPosSet(self->espA, lbl_1_rodata_E4[0], lbl_1_rodata_CC[0]);
        espPosSet(self->espB, lbl_1_rodata_E8[0], lbl_1_rodata_CC[0]);
    }
}

void fn_1_3CD8(M575Score *self)
{
    espKill(self->espB);
    espKill(self->espA);
    MgScoreKill(self->score);
    memset(self, 0, sizeof(M575Score));
}

void fn_1_3D28(M575Score *self)
{
    MgScoreValueSet(self->score, self->value);
}

void fn_1_3D5C(M575Score *self, s32 disp)
{
    if (disp != 0) {
        MgScoreWinDispSet(self->winId, TRUE);
        MgScoreDispOn(self->score);
        espDispOn(self->espA);
        espDispOn(self->espB);
    }
    else {
        MgScoreWinDispSet(self->winId, FALSE);
        MgScoreDispOff(self->score);
        espDispOff(self->espA);
        espDispOff(self->espB);
    }
}
