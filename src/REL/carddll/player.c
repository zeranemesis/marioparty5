// carddll/player.c - decompilation of the carddll/player unit (136164 B, 43 fns).
// Real code is being added function by function (smallest first), each verified
// at 100% with tools/fndiff.py before the next one. decomp/player.m2c.c is the
// reference draft (raw mips2c output, not buildable as-is).

#include "types.h"
#include "game/process.h"
#include "game/audio.h"
#include "game/hsfex.h"
#include "game/esprite.h"
#include "game/pad.h"
#include "game/frand.h"
#include "game/gamework.h"

/* ---- bss globals (carddll/player) ---- */
static s32 lbl_1_bss_8[3];
static s32 lbl_1_bss_1174;
static s32 lbl_1_bss_1168[3];
static s32 lbl_1_bss_C48[2];
static s32 lbl_1_bss_C44;
static s32 lbl_1_bss_1298[3];
static HUPROCESS *lbl_1_bss_11DC;
static s16 lbl_1_bss_FB2[4];
static s16 lbl_1_bss_FBA[4];
static s16 lbl_1_bss_FC2[4];
static f32 lbl_1_rodata_4AC = 0.017453292f;

void fn_1_FB10(void);
s32 fn_1_13728(f32 *arg0);
s32 fn_1_FCD4(f32 *arg0);
s32 fn_1_11C48(f32 *arg0);
s32 fn_1_1658C(f32 *arg0);
s32 fn_1_127F8(void);
void PSMTXIdentity(Mtx m);
void PSMTXRotRad(Mtx m, char axis, f32 rad);
void PSMTXConcat(const Mtx a, const Mtx b, Mtx ab);
void Hu3DMtxRotGet(Mtx src, HuVecF *dst);

/* fn_1_FA24: reset the player state globals, then spawn the player process. */
void fn_1_FA24(void)
{
    lbl_1_bss_1174 = 0;
    lbl_1_bss_1168[0] = 0;
    lbl_1_bss_1168[1] = 0;
    lbl_1_bss_1168[2] = 0;
    lbl_1_bss_C48[0] = 0;
    lbl_1_bss_C48[1] = 0;
    lbl_1_bss_C44 = 1;
    lbl_1_bss_1298[0] = 0;
    lbl_1_bss_1298[1] = 0;
    lbl_1_bss_1298[2] = 0;
    lbl_1_bss_11DC = NULL;
    HuPrcChildCreate(fn_1_FB10, 0x64, 0x2000, 0, HuPrcCurrentGet());
}

/* fn_1_FB10: player process body - mute the stream when it is idle, then sleep. */
void fn_1_FB10(void)
{
    if (lbl_1_bss_8[1] != 0 && HuAudSStreamStatGet(lbl_1_bss_8[0]) == 0) {
        HuAudSStreamVolSet(0, 0x7F, 0x3E8);
        lbl_1_bss_8[1] = 0;
        lbl_1_bss_8[0] = 0;
    }
    HuPrcVSleep();
}

/* fn_1_FB88: player state machine - advance lbl_1_bss_1174 through states 0..4. */
s32 fn_1_FB88(f32 *arg0)
{
    s32 var_r30;

    var_r30 = 0;
    if (lbl_1_bss_1174 == 0) {
        if (fn_1_13728(arg0) != 0) {
            lbl_1_bss_1174 = 1;
        }
    } else if (lbl_1_bss_1174 == 1) {
        if (fn_1_FCD4(arg0) != 0) {
            lbl_1_bss_1174 = 2;
        }
    } else if (lbl_1_bss_1174 == 2) {
        if (fn_1_11C48(arg0) != 0) {
            lbl_1_bss_1174 = 3;
        }
    } else if (lbl_1_bss_1174 == 3) {
        if (fn_1_1658C(arg0) != 0) {
            lbl_1_bss_1174 = 4;
        }
    } else if (lbl_1_bss_1174 == 4 && fn_1_127F8() != 0) {
        lbl_1_bss_1174 = 0;
        var_r30 = 1;
    }
    return var_r30;
}

/* fn_1_2FEF4: reset the per-player card state (byte 0x30..0x50) for all 4 players.
   The per-player record is 0x5C bytes; the fields reset here live at 0x30..0x50. */
struct PlayerCardRec
{
    char pad0[0x30];
    s32 f_30;
    s32 f_34;
    s32 f_38;
    s32 f_3C;
    s32 f_40;
    s32 f_44;
    s32 f_48;
    s32 f_4C;
    s32 f_50;
    char pad54[0x8];
}; /* size = 0x5C */

void fn_1_2FEF4(struct PlayerCardRec *arg0)
{
    s32 var_r31;

    var_r31 = 0;
    while (var_r31 < 4) {
        arg0[var_r31].f_30 = 1;
        arg0[var_r31].f_34 = -1;
        arg0[var_r31].f_38 = -1;
        arg0[var_r31].f_3C = -1;
        arg0[var_r31].f_40 = 0;
        arg0[var_r31].f_44 = 0;
        arg0[var_r31].f_48 = 0;
        arg0[var_r31].f_4C = 0;
        arg0[var_r31].f_50 = 0;
        var_r31 += 1;
    }
}

/* fn_1_25444: build a rotation matrix from 3 angles in degrees (x, y, z)
   and extract the resulting rotation into arg1 (HuVecF). Returns the matrix. */
Mtx *fn_1_25444(f32 *arg0, HuVecF *arg1)
{
    Mtx id;
    Mtx rot;

    PSMTXIdentity(id);
    PSMTXRotRad(rot, 'x', lbl_1_rodata_4AC * arg0[0]);
    PSMTXConcat(id, rot, id);
    PSMTXRotRad(rot, 'z', lbl_1_rodata_4AC * arg0[2]);
    PSMTXConcat(id, rot, id);
    PSMTXRotRad(rot, 'y', lbl_1_rodata_4AC * arg0[1]);
    PSMTXConcat(id, rot, id);
    Hu3DMtxRotGet(id, arg1);
    return &id;
}

/* Player struct (fields used by this unit so far): curPlayerNo @ 0x170; the per-player
   card-record index lives at 0x174 + curPlayerNo*4 (s32). */
struct PlayerStruct
{
    char pad170[0x170];
    s32 curPlayerNo;
};

/* fn_1_304D8: like fn_1_303E0 but the record array is passed in (arg1) and the
   sprite display is turned ON. For each of the 3 opponents, take the sprite id from
   the bank table matching the current player number, and if the record sprite index
   is set, bank it and turn its display on. */
void fn_1_304D8(struct PlayerStruct *arg0, s32 *arg1)
{
    s16 bankId;
    s16 bankId2;
    s16 bankId3;
    s32 var_r31;

    var_r31 = 0;
    while (var_r31 < 3) {
        if (var_r31 == 0) {
            bankId = lbl_1_bss_FC2[arg0->curPlayerNo];
        } else {
            if (var_r31 == 1) {
                bankId2 = lbl_1_bss_FBA[arg0->curPlayerNo];
            } else {
                bankId2 = lbl_1_bss_FB2[arg0->curPlayerNo];
            }
            bankId = bankId2;
        }
        bankId3 = bankId;
        if (*(s32 *) (arg1 + var_r31) != -1) {
            espBankSet(bankId3, *(s32 *) (arg1 + var_r31));
            espDispOn(bankId3);
        }
        var_r31 += 1;
    }
}

/* fn_1_303E0: per-player sprite-ESP bank setup. For each of the 3 opponents, take the
   sprite id from the bank table matching the current player number, and if the record
   sprite index (f_34) is set, bank it and turn its display off. */
void fn_1_303E0(struct PlayerStruct *arg0)
{
    u8 *base;
    s16 bankId3;
    s16 bankId;
    s16 bankId2;
    s32 var_r30;

    base = (u8 *) arg0 + *(s32 *) ((u8 *) arg0 + 0x174 + arg0->curPlayerNo * 4) * 0x5C;
    var_r30 = 0;
    while (var_r30 < 3) {
        if (var_r30 == 0) {
            bankId = lbl_1_bss_FC2[arg0->curPlayerNo];
        } else {
            if (var_r30 == 1) {
                bankId2 = lbl_1_bss_FBA[arg0->curPlayerNo];
            } else {
                bankId2 = lbl_1_bss_FB2[arg0->curPlayerNo];
            }
            bankId = bankId2;
        }
        bankId3 = bankId;
        if (*(s32 *) (base + var_r30 * 4 + 0x34) != -1) {
            espBankSet(bankId3, *(s32 *) (base + var_r30 * 4 + 0x34));
            espDispOff(bankId3);
        }
        var_r30 += 1;
    }
}

/* fn_1_25518: per-player "random button press" for AI players. arg0 is the player
   number. If the player is not a human (GwPlayerConf[arg0].type != 0), clear its
   button-down and stick-rep state, then increment a global counter (mod 0x15E).
   When the counter exceeds a random threshold (frandmod(0x12C) + 0x1F), fire a
   press (HuPadBtnDown = 0x100) and reset the counter. */
void fn_1_25518(s32 arg0)
{
    if (GwPlayerConf[arg0].type != 0) {
        HuPadBtnDown[arg0] = 0;
        HuPadDStkRep[arg0] = 0;
        lbl_1_bss_C48[0] += 1;
        lbl_1_bss_C48[0] %= 0x15E;
        if ((u32) lbl_1_bss_C48[0] > (u32) (frandmod(0x12C) + 0x1F)) {
            HuPadBtnDown[arg0] = 0x100;
            lbl_1_bss_C48[0] = 0;
        }
    }
}
