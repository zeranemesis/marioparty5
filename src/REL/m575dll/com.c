// REL/m575dll/com - the COM (computer-player) AI.
//
// The COM player owns a 0x4C-byte control block. Its first word points at the
// per-player work record; that record's first word points back at the game
// system, which lays the two players out at +0x08 and +0xd4.
#include "types.h"
#include "REL/m575dll.h"
#include "game/frand.h"
#include "string.h"

typedef s16 HU3DMODELID;
typedef s16 HU3DMOTID;

// game/gamework.h pulls in the whole game header set; the five s16 fields are
// all this unit reads out of GwPlayerConf.
typedef struct comGwPlayerConf_s {
    s16 charNo;
    s16 padNo;
    s16 dif;
    s16 grp;
    s16 type;
} COM_GWPLAYERCONF;

extern COM_GWPLAYERCONF GwPlayerConf[4];

// Per-player work record. Only the offsets the COM reads are named.
typedef struct comUnk8_s {
    u8 pad0[0x28];
    s32 unk28;
} COM_UNK8;

typedef struct comPlayer_s {
    struct comSystem_s *system;
    u8 pad4[4];
    COM_UNK8 *unk8;
    u8 padC[0x0E - 0x0C];
    s16 playerNo;
    u8 pad10[0x78 - 0x10];
    Vec *unk78;
    u8 pad7C[0x88 - 0x7C];
    Vec unk88;
    f32 unk94;
    s32 unk98;
    u8 pad9C[0xCC - 0x9C];
} COM_PLAYER;

typedef struct comSystem_s {
    u8 pad00[8];
    COM_PLAYER players[2];
} COM_SYSTEM;

// The AI tuning block the game system carries at +0x1a0.
typedef struct comAi_s {
    u8 pad0[0x20];
    f32 unk20;
} COM_AI;

// The board state block the game system carries at +0xe90.
typedef struct comBoard_s {
    u8 pad0[4];
    u16 mode;
} COM_BOARD;

#define COM_AI_OF(sys) ((COM_AI *)((u8 *)(sys) + 0x1a0))
#define COM_BOARD_OF(sys) ((COM_BOARD *)((u8 *)(sys) + 0xe90))

static const f32 lbl_1_rodata_258[1] = { 50.0f };
static const f32 lbl_1_rodata_25C[1] = { 6.0f };
static const f32 lbl_1_rodata_260[1] = { 6.6666665f };
static const f32 lbl_1_rodata_264[1] = { 7.5f };
static const f32 lbl_1_rodata_268[1] = { 20.0f };
static const f32 lbl_1_rodata_27C[1] = { 0.0f };
static const f32 lbl_1_rodata_284[1] = { 1.0f };
static const f32 lbl_1_rodata_2B4[1] = { 0.2f };
static const Vec lbl_1_rodata_2E8 = { 0.0f, 0.0f, 0.0f };
static const f32 lbl_1_rodata_300[1] = { 10.0f };
static const f32 lbl_1_rodata_310[1] = { 100.0f };
static const f32 lbl_1_rodata_314[1] = { 60.0f };
static const f32 lbl_1_rodata_318[1] = { 30.0f };
static const f32 lbl_1_rodata_31C[1] = { 0.8f };
static const f32 lbl_1_rodata_320[1] = { 56.0f };

// The 0x4C-byte control block. Only the offsets the five functions below reach
// are named; 0x4C is the size both constructors clear.
typedef struct com_s {
    COM_PLAYER *work;
    u16 mode;
    s16 timer;
    s32 unk8;
    s32 unkC;
    u16 unk10;
    u8 pad12[2];
    Vec unk14;
    Vec unk20;
    s32 unk2C;
    s32 unk30;
    f32 unk34;
    f32 unk38;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
} COM;

void fn_1_6630(COM *self, COM_PLAYER *work);
void fn_1_675C(COM *self);
void fn_1_678C(COM *self);
void fn_1_7A80(COM *self);
void fn_1_6BE4(COM *self);
void fn_1_734C(COM *self);
s32 fn_1_8180(COM *self);
s32 fn_1_79A0(COM *self);

// Initialises the control block: links it to the player work record, copies the
// player's difficulty over, maps the pad number onto a skill tier and seeds the
// start position from the player's board position.
void fn_1_6630(COM *self, COM_PLAYER *work)
{
    s32 tier;
    s32 mid;
    s32 low;

    memset(self, 0, sizeof(COM));
    self->work = work;
    self->unk8 = GwPlayerConf[work->playerNo].type;

    if (GwPlayerConf[work->playerNo].dif == 3) {
        tier = 0;
    }
    else {
        if (GwPlayerConf[work->playerNo].dif == 2) {
            mid = 1;
        }
        else {
            if (GwPlayerConf[work->playerNo].dif == 1) {
                low = 2;
            }
            else {
                low = 3;
            }
            mid = low;
        }
        tier = mid;
    }
    self->unk10 = (u16)tier;

    self->unk20 = *work->unk78;
    self->unk14 = self->unk20;
    self->unkC = 0;
}

void fn_1_675C(COM *self)
{
    memset(self, 0, sizeof(COM));
}

// Per-frame aim update. Works out how far the COM has drifted from the player's
// board position and from the opponent, latches the three flag words when it is
// too far away or has just switched targets, then folds the horizontal error
// into an analogue stick deflection (0x34/0x38) and the truncated byte deflections
// (0x40/0x44).
s32 fn_1_8180(COM *self)
{
    COM_PLAYER *work = self->work;
    COM_SYSTEM *sys = work->system;
    COM_PLAYER *other = (work == &sys->players[0]) ? &sys->players[1] : &sys->players[0];
    Vec delta;
    Vec oppDelta;
    Vec playerPos;
    f32 dist;
    f32 oppDist;
    f32 ratio;
    f32 outer;
    f32 inner;

    playerPos = work->unk88;
    PSVECSubtract(&self->unk14, &playerPos, &delta);
    delta.y = 0.0f;
    dist = PSVECMag(&delta);
    if (dist < 10.0f) {
        return 1;
    }
    PSVECSubtract(work->unk78, other->unk78, &oppDelta);
    oppDelta.y = 0.0f;
    oppDist = PSVECMag(&oppDelta);
    if (dist < 100.0f && oppDist < 100.0f) {
        return 1;
    }

    if (self->unk30 != 0) {
        if (work->unk8->unk28 != 1) {
            self->unk30 = 0;
        }
        else if (dist > 60.0f) {
            self->unk48 |= 0x100;
        }
    }
    else if (self->unk2C != 0) {
        if (work->unk8->unk28 == 1) {
            self->unk2C = 0;
            self->unk30 = 1;
        }
        else if (dist > 60.0f) {
            self->unk3C |= 0x100;
            self->unk48 |= 0x100;
        }
        else {
            self->unk2C = 0;
        }
    }

    if (delta.x != lbl_1_rodata_27C[0] || delta.y != lbl_1_rodata_27C[0] || delta.z != lbl_1_rodata_27C[0]) {
        PSVECNormalize(&delta, &delta);
    }

    ratio = dist / 30.0f;
    if (ratio > 1.0f) {
        outer = 1.0f;
    }
    else {
        if (ratio < 0.0f) {
            inner = 0.0f;
        }
        else {
            inner = ratio;
        }
        outer = inner;
    }
    ratio = outer;    delta.x = delta.x * ratio;
    delta.z = delta.z * ratio;

    self->unk34 = (0.8f * self->unk34) + (0.2f * delta.x);
    self->unk38 = (0.8f * self->unk38) + (0.2f * delta.z);
    self->unk40 = (s8)(56.0f * self->unk34);
    self->unk44 = (s8)(56.0f * -self->unk38);
    return 0;
}

// Runs when the COM holds ball kind 4: if it has gone too far past the player
// and the player is not in the recovery window, snap the aim point back to the
// origin and re-run the aim update. Otherwise, while the board is idle, latch
// the two long-stance flags so the next states see a settled COM.
s32 fn_1_79A0(COM *self)
{
    COM_PLAYER *work = self->work;
    COM_SYSTEM *sys = work->system;
    COM_AI *ai = COM_AI_OF(sys);
    COM_BOARD *board = COM_BOARD_OF(sys);
    Vec *src;
    Vec *dst;
    Vec tmp;
    s32 keep;
    s32 keep2;

    keep = 0;
    keep2 = 0;
    if (work->unk94 > lbl_1_rodata_258[0] && work->unk8->unk28 != 1) {
        src = (Vec *)&lbl_1_rodata_2E8;
        dst = (Vec *)((u8 *)self + 0x14);
        tmp = *src;
        *dst = tmp;
        fn_1_8180(self);
        return 1;
    }    if (board->mode == 0) {
        self->unk3C |= 0x100;
        self->unk48 |= 0x100;
    }
    return 0;
}
