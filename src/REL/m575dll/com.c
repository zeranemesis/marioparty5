// REL/m575dll/com - the COM (computer-player) AI.
//
// The COM player owns a 0x4C-byte control block. Its first word points at the
// per-player work record; that record's first word points back at the game
// system, which lays the two players out at +0x08 and +0xd4.
#include "types.h"
#include "REL/m575dll.h"
#include "game/frand.h"
#include "string.h"
#include "humath.h"

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
    u32 unk98;
    u8 pad9C[4];
    s32 unkA0;
    u8 padA4[0xCC - 0xA4];
} COM_PLAYER;

typedef struct comSystem_s {
    void *unk00;
    u8 pad04[4];
    COM_PLAYER players[2];
} COM_SYSTEM;

// The AI tuning block the game system carries at +0x1a0.
typedef struct comAi_s {
    u8 pad0[0x20];
    f32 unk20;
    u8 pad24[0x2C - 0x24];
    void *unk2C;
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
static const Vec lbl_1_rodata_2F4 = { 0.0f, 1.0f, 0.0f };
static const f32 lbl_1_rodata_300[1] = { 10.0f };
static const f32 lbl_1_rodata_288[1] = { 0.5f };
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
s32 fn_1_7A80(COM *self);
s32 fn_1_6BE4(COM *self);
s32 fn_1_734C(COM *self);
s32 fn_1_8180(COM *self);
s32 fn_1_79A0(COM *self);
f64 fn_1_30CC(f32 arg0, f32 arg1, f32 arg2);

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

// Per-frame COM state machine. Clears the four flag words, then - once the
// control block has been linked to a player - picks a stance ("mode") out of
// 0..3 and lets that stance's update run. The stance timer is refreshed every
// 30..59 frames, or reset to 0 whenever a stance reports that it has finished
// something.
void fn_1_678C(COM *self)
{
    COM_PLAYER *work = self->work;
    COM_SYSTEM *sys = work->system;
    COM_AI *ai = COM_AI_OF(sys);
    COM_PLAYER *other = (work == &sys->players[0]) ? &sys->players[1] : &sys->players[0];

    self->unk3C = 0;
    self->unk40 = 0;
    self->unk44 = 0;
    self->unk48 = 0;

    if (self->unkC != 0) {
        if (self->timer <= 0) {
            self->mode = 0;
            self->timer = (s16)(frand() % 30 + 30);

            if (work->unk94 < lbl_1_rodata_258[0] && other->unk94 > lbl_1_rodata_258[0] && other->unkA0 > 120) {
                self->mode = 3;
                self->timer = (s16)(frand() % 30 + 30);
            }
            else if ((self->unk10 == 0 && ai->unk20 > lbl_1_rodata_25C[0])
                  || (self->unk10 == 1 && ai->unk20 > lbl_1_rodata_260[0] && frand() % 10000 < 9500)
                  || (self->unk10 == 2 && ai->unk20 > lbl_1_rodata_264[0] && frand() % 10000 < 8500)
                  || (self->unk10 == 3 && ai->unk20 > lbl_1_rodata_268[0] && frand() % 10000 < 7500)) {
                self->mode = 1;
                self->timer = (s16)(frand() % 30 + 30);
            }
            else if (self->unk10 == 0
                  || (self->unk10 == 1 && frand() % 10000 < 9000)
                  || (self->unk10 == 2 && frand() % 10000 < 8000)
                  || (self->unk10 == 3 && frand() % 10000 < 3000)) {
                if (work->unk98 != 0 && work->unk94 > lbl_1_rodata_258[0]) {
                    self->mode = 2;
                    self->timer = (s16)(frand() % 30 + 30);
                }
                else if (other->unk94 > lbl_1_rodata_258[0] && other->unkA0 > 120) {
                    self->mode = 3;
                    self->timer = (s16)(frand() % 30 + 30);
                }
            }
        }
        switch (self->mode) {
        case 0:
            if (fn_1_7A80(self) != 0) {
                self->timer = 0;
            }
            break;
        case 1:
            if (fn_1_6BE4(self) != 0) {
                self->timer = 0;
            }
            break;
        case 2:
            if (fn_1_734C(self) != 0) {
                self->timer = 0;
            }
            break;
        case 3:
            if (fn_1_79A0(self) != 0) {
                self->timer = 0;
            }
            break;
        }
        if (self->timer != 0) {
            self->timer--;
        }
    }
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

// Stance 0: swing the COM's aim point around the player. The heading is taken
// from the saved aim point, blended with the heading of the player's own board
// position, and rebuilt as a unit vector scaled by the aim point's length.
// Both headings run through the same ((-180,180]) wrap; every arm re-evaluates
// HuAtan rather than keeping the raw angle in a temporary.
s32 fn_1_7A80(COM *self)
{
    COM_PLAYER *work = self->work;
    COM_SYSTEM *sys = work->system;
    void *a = sys->unk00;
    COM_AI *ai = COM_AI_OF(sys);
    void *b = &ai->unk2C;
    Vec up;
    Vec *upP;
    Vec up2;
    Vec playerPos;
    Vec v;
    f64 ang;
    f64 tmp;
    f64 mix;
    f32 dist;
    f32 aimA;
    f32 aimB;

    up = lbl_1_rodata_2F4;
    upP = &up;
    up2 = *upP;
    playerPos = work->unk88;

    PSVECSubtract(&self->unk20, &playerPos, &v);
    v.y = 0.0f;
    dist = PSVECMag(&v);
    if (dist < lbl_1_rodata_300[0]) {
        f32 r = (f32)(frand() % 360);
        f32 scale;
        f32 hi;
        f32 lo;

        v.x = (f32)HuSin(r);
        v.y = 0.0f;
        v.z = (f32)HuCos(r);
        scale = (f32)(frand() % 10000) / 10000.0f;
        if (scale > 1.0f) {
            hi = 1.0f;
        }
        else {
            if (scale < 0.5f) {
                lo = 0.5f;
            }
            else {
                lo = scale;
            }
            hi = lo;
        }
        scale = hi;
        PSVECScale(&v, &v, 320.0f * scale);
        self->unk20 = v;
    }

    v = self->unk20;
    if (v.x != lbl_1_rodata_27C[0] || v.z != lbl_1_rodata_27C[0]) {
        PSVECNormalize(&v, &v);
    }
    if (HuAtan(v.x, v.z) > 180.0) {
        ang = HuAtan(v.x, v.z) - 360.0;
    }
    else {
        if (HuAtan(v.x, v.z) > -180.0) {
            tmp = HuAtan(v.x, v.z);
        }
        else {
            tmp = 360.0 + HuAtan(v.x, v.z);
        }
        ang = tmp;
    }
    aimA = (f32)ang;

    v = work->unk88;
    v.y = 0.0f;
    if (v.x != lbl_1_rodata_27C[0] || v.z != lbl_1_rodata_27C[0]) {
        PSVECNormalize(&v, &v);
    }
    if (HuAtan(v.x, v.z) > 180.0) {
        ang = HuAtan(v.x, v.z) - 360.0;
    }
    else {
        if (HuAtan(v.x, v.z) > -180.0) {
            tmp = HuAtan(v.x, v.z);
        }
        else {
            tmp = 360.0 + HuAtan(v.x, v.z);
        }
        ang = tmp;
    }
    aimB = (f32)ang;

    mix = fn_1_30CC(aimB, aimA, lbl_1_rodata_288[0]);
    v.x = (f32)HuSin(mix);
    v.y = 0.0f;
    v.z = (f32)HuCos(mix);
    dist = PSVECMag(&self->unk20);
    PSVECScale(&v, &v, dist);
    self->unk14 = v;
    fn_1_8180(self);
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

    if (work->unk94 > lbl_1_rodata_258[0] && work->unk8->unk28 != 1) {
        Vec tmp;
        Vec *p;
        tmp = lbl_1_rodata_2E8;
        p = &tmp;
        self->unk14 = *p;
        fn_1_8180(self);
        return 1;
    }
    if (board->mode == 0) {
        self->unk3C |= 0x100;
        self->unk48 |= 0x100;
    }
    return 0;
}
