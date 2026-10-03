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
static s16 lbl_1_bss_FCA[4];
static s16 lbl_1_bss_FD2[4];
static s16 lbl_1_bss_FDA[4];
static s16 lbl_1_bss_FE2[4];
static s16 lbl_1_bss_FEA[4];
static s16 lbl_1_bss_FF2[4];
static s16 lbl_1_bss_FFA[4];
static s16 lbl_1_bss_FAA[4];
static s16 lbl_1_bss_FA2[4];
static s16 lbl_1_bss_1002[4];
static s16 lbl_1_bss_100A[4][4];
static s16 lbl_1_bss_F9E[2];
static f32 lbl_1_rodata_4AC = 0.017453292f;
static f32 lbl_1_rodata_250 = -50.0f;
static f32 lbl_1_rodata_24C = 0.0f;

/* lbl_1_bss_468: per-player model-id table (2 entries per player, 4 players). */
extern s16 lbl_1_bss_468[8];
static Vec lbl_1_rodata_504 = { 1.0f, 1.0f, 1.0f };
void fn_1_9C74(f32 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void fn_1_3F278(s32 arg0, s16 arg1);
void fn_1_3F6B0(s32 arg0, s16 arg1, Vec *arg2);
void fn_1_3F540(s32 arg0, s16 arg1, s32 arg2);
void fn_1_3F3D8(s32 arg0, s16 arg1, Vec *arg2);
void fn_1_5500(f32 *arg0);

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

/* lbl_1_bss_8C4: per-player state table, defined in the card unit (card.o).
   Rows are 0x28 bytes; entry flags live at offset f_10*0x28 + f_14*4. */
extern s32 lbl_1_bss_8C4[0x190 / 4];
s32 fn_1_2E548(f32 *arg0);
s32 fn_1_2B280(f32 *arg0);
s32 fn_1_3FC50(f32 *arg0);
s32 fn_1_42EDC(f32 *arg0);

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
        lbl_1_bss_8[0] = lbl_1_bss_8[1] = 0;
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
    s32 f_00;
    s32 f_04;
    s32 f_08;
    s32 f_0C;
    s32 f_10;
    s32 f_14;
    s32 f_18;
    s32 f_1C;
    s32 f_20;
    s32 f_24;
    s32 f_28;
    s32 f_2C;
    s32 f_30;
    s32 f_34;
    s32 f_38;
    s32 f_3C;
    s32 f_40;
    s32 f_44;
    s32 f_48;
    s32 f_4C;
    s32 f_50;
    s16 f_54;
    s16 f_56;
    char pad_58[4];
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
   card-record index lives at 0x174 + curPlayerNo*4 (s32); the slot count @ 0x194. */
struct PlayerStruct
{
    char pad170[0x170];
    s32 curPlayerNo;
    s32 pad174[8];
    s32 f_194;
};

/* lbl_1_bss_8C4 row lookup: 0x28-byte rows indexed by f_10, then by f_14. */
#define CARD_FLAGS(r) (*(s32 *) ((char *) &lbl_1_bss_8C4 + (r)->f_10 * 0x28 + (r)->f_14 * 4))

/* fn_1_304D8: like fn_1_303E0 but the record array is passed in (arg1) and the
   sprite display is turned ON. For each of the 3 opponents, take the sprite id from
   the bank table matching the current player number, and if the record sprite index
   is set, bank it and turn its display on. */
void fn_1_304D8(struct PlayerStruct *arg0, register s32 *arg1)
{
    s16 bankId3;
    s16 bankId;
    s16 bankId2;
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
        if (arg1[var_r31] != -1) {
            espBankSet(bankId3, arg1[var_r31]);
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

/* fn_1_30910: kill every per-player sprite ESP id. The per-player ids live in 7
   bss tables (3 single-id tables + one 4-slot table + 4 single-id tables); for
   each of the 4 players kill all 16, then kill the 2 global ids. */
void fn_1_30910(void)
{
    s32 var_r31;

    var_r31 = 0;
    while (var_r31 < 4) {
        espKill(lbl_1_bss_1002[var_r31]);
        espKill(lbl_1_bss_FFA[var_r31]);
        espKill(lbl_1_bss_FF2[var_r31]);
        espKill(lbl_1_bss_100A[var_r31][0]);
        espKill(lbl_1_bss_100A[var_r31][1]);
        espKill(lbl_1_bss_100A[var_r31][2]);
        espKill(lbl_1_bss_100A[var_r31][3]);
        espKill(lbl_1_bss_FEA[var_r31]);
        espKill(lbl_1_bss_FE2[var_r31]);
        espKill(lbl_1_bss_FDA[var_r31]);
        espKill(lbl_1_bss_FD2[var_r31]);
        espKill(lbl_1_bss_FCA[var_r31]);
        espKill(lbl_1_bss_FC2[var_r31]);
        espKill(lbl_1_bss_FBA[var_r31]);
        espKill(lbl_1_bss_FB2[var_r31]);
        espKill(lbl_1_bss_FAA[var_r31]);
        espKill(lbl_1_bss_FA2[var_r31]);
        var_r31 += 1;
    }
    espKill(lbl_1_bss_F9E[0]);
    espKill(lbl_1_bss_F9E[1]);
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

/* fn_1_2B10C: per-player state dispatch. Look up the player's state-table entry
   (row f_10 @ +0x10, col f_14 @ +0x14 of the 0x5C-byte record) and, depending on
   which of the four high flag bits (19..16) is set, run the matching handler;
   return 1 if that handler reports a state change. */
s32 fn_1_2B10C(f32 *arg0)
{
    s32 *rec;
    s32 var_r29;

    var_r29 = 0;
    rec = (s32 *) ((u8 *) arg0 + *(s32 *) ((u8 *) arg0 + 0x174 + *(s32 *) ((u8 *) arg0 + 0x170) * 4) * 0x5C);
    if (*(s32 *) ((char *) &lbl_1_bss_8C4 + rec[4] * 0x28 + rec[5] * 4) & 0x1000) {
        if (fn_1_2E548(arg0) != 0) {
            var_r29 = 1;
        }
    } else if (*(s32 *) ((char *) &lbl_1_bss_8C4 + rec[4] * 0x28 + rec[5] * 4) & 0x2000) {
        if (fn_1_2B280(arg0) != 0) {
            var_r29 = 1;
        }
    } else if (*(s32 *) ((char *) &lbl_1_bss_8C4 + rec[4] * 0x28 + rec[5] * 4) & 0x4000) {
        if (fn_1_3FC50(arg0) != 0) {
            var_r29 = 1;
        }
    } else if (((*(s32 *) ((char *) &lbl_1_bss_8C4 + rec[4] * 0x28 + rec[5] * 4) & 0x8000) != 0) && (fn_1_42EDC(arg0) != 0)) {
        var_r29 = 1;
    }
    return var_r29;
}

/* fn_1_2FBCC: card-hand animation. arg0 is the player struct (f32 *). When the
   card state (0x1D4) is 1, for each of the 4 slots read the direction (f_04),
   the y (f_08) and z (f_0C) fields, adjust y/z by one depending on the direction
   (1: y-1, 2: y+1, 3: z-1, 4: z+1), compute the position with fn_1_9C74,
   offset x by -50 (rodata_250), set the model position (bss_468 model id) and
   reset its rotation to zero (rodata_24C = 0.0f). If the slot flag (f_50) has
   bit 0x2 set, destroy the card (fn_1_3F278(3)) and clear the flag; if bit 0x4
   is set, either destroy it (fn_1_3F278(4)) when this is the current player's
   slot, or scale/spawn it (fn_1_3F6B0/fn_1_3F540/fn_1_3F3D8) otherwise.
   After the loop, run fn_1_5500 and set the state to 0xA. Otherwise, copy the
   direction/y/z fields down into the model fields (f_00/f_10/f_14) and set
   f_20 = f_24 = 1 for each slot, then set the state to -1. */
void fn_1_2FBCC(f32 *arg0)
{
    Vec sp2C;
    Vec sp20;
    Vec sp14;
    Vec sp8;
    Vec *pVec14;
    Vec *pVec8;

    if (*(s32 *) ((u8 *) arg0 + 0x1D4) == 1) {
        s32 var_r30;

        var_r30 = 0;
        while (var_r30 < 4) {
            s32 dir;
            s32 y;
            s32 z;

            dir = *(s32 *) ((u8 *) arg0 + var_r30 * 0x5C + 4);
            y = *(s32 *) ((u8 *) arg0 + var_r30 * 0x5C + 8);
            z = *(s32 *) ((u8 *) arg0 + var_r30 * 0x5C + 0xC);
            if (dir == 1) {
                y -= 1;
            } else if (dir == 2) {
                y += 1;
            } else if (dir == 3) {
                z -= 1;
            } else if (dir == 4) {
                z += 1;
            }
            fn_1_9C74((f32 *) &sp2C, y, z, 1, 1);
            sp2C.x += lbl_1_rodata_250;
            Hu3DModelPosSetV(lbl_1_bss_468[var_r30 * 2], &sp2C);
            Hu3DModelRotSet(lbl_1_bss_468[var_r30 * 2], lbl_1_rodata_24C, lbl_1_rodata_24C, lbl_1_rodata_24C);
            if (*(s32 *) ((u8 *) arg0 + var_r30 * 0x5C + 0x50) & 2) {
                fn_1_3F278(3, *(s16 *) ((u8 *) arg0 + var_r30 * 0x5C + 0x56));
                *(s16 *) ((u8 *) arg0 + var_r30 * 0x5C + 0x56) = -1;
                *(s32 *) ((u8 *) arg0 + var_r30 * 0x5C + 0x50) &= 0xFFFFFFFD;
            }
            if (*(s32 *) ((u8 *) arg0 + var_r30 * 0x5C + 0x50) & 4) {
                if (var_r30 == *(s32 *) ((u8 *) arg0 + *(s32 *) ((u8 *) arg0 + 0x170) * 4 + 0x174)) {
                    fn_1_3F278(4, *(s16 *) ((u8 *) arg0 + var_r30 * 0x5C + 0x56));
                    *(s16 *) ((u8 *) arg0 + var_r30 * 0x5C + 0x56) = -1;
                    *(s32 *) ((u8 *) arg0 + var_r30 * 0x5C + 0x50) &= 0xFFFFFFFB;
                } else {
                    sp20 = lbl_1_rodata_504;
                    sp14 = sp20;
                    pVec14 = &sp14;
                    fn_1_3F6B0(4, *(s16 *) ((u8 *) arg0 + var_r30 * 0x5C + 0x56), pVec14);
                    fn_1_3F540(4, *(s16 *) ((u8 *) arg0 + var_r30 * 0x5C + 0x56), 1);
                    sp8 = sp2C;
                    pVec8 = &sp8;
                    fn_1_3F3D8(4, *(s16 *) ((u8 *) arg0 + var_r30 * 0x5C + 0x56), pVec8);
                }
            }
            var_r30 += 1;
        }
        fn_1_5500(arg0);
        *(s32 *) ((u8 *) arg0 + 0x1D4) = 0xA;
        return;
    }
    {
        s32 var_r30_2;

        var_r30_2 = 0;
        while (var_r30_2 < 4) {
        *(s32 *) ((u8 *) arg0 + var_r30_2 * 0x5C + 0) = *(s32 *) ((u8 *) arg0 + var_r30_2 * 0x5C + 4);
        *(s32 *) ((u8 *) arg0 + var_r30_2 * 0x5C + 0x10) = *(s32 *) ((u8 *) arg0 + var_r30_2 * 0x5C + 8);
        *(s32 *) ((u8 *) arg0 + var_r30_2 * 0x5C + 0x14) = *(s32 *) ((u8 *) arg0 + var_r30_2 * 0x5C + 0xC);
        *(s32 *) ((u8 *) arg0 + var_r30_2 * 0x5C + 0x20) = 1;
        *(s32 *) ((u8 *) arg0 + var_r30_2 * 0x5C + 0x24) = 1;
        var_r30_2 += 1;
    }
    *(s32 *) ((u8 *) arg0 + 0x1D4) = -1;
    }
}

/* CARD_FLAGS: the per-player "card phase" walker. Repeatedly advances the 0x5C-byte
   record's f_10/f_14 cursor and f_00 phase field until none of the five state-table
   flag bits (20..24) apply. arg1 is the current direction, carried across iterations. */
void fn_1_17BE8(struct PlayerStruct *arg0, s32 arg1)
{
    struct PlayerCardRec *rec;
    s32 var_C;
    s32 var_8;

    var_C = 0;
    var_8 = 0;
    rec = (struct PlayerCardRec *) ((u8 *) arg0 + *(s32 *) ((u8 *) arg0 + 0x174 + arg0->curPlayerNo * 4) * 0x5C);
for (;;) {
        if (!((CARD_FLAGS(rec) & 0x1000000) ||
              (rec->f_00 == 1 && (CARD_FLAGS(rec) & 0x8000)) ||
              (rec->f_00 == 2 && (CARD_FLAGS(rec) & 0x4000)) ||
              (rec->f_00 == 3 && (CARD_FLAGS(rec) & 0x2000)) ||
              (rec->f_00 == 4 && (CARD_FLAGS(rec) & 0x1000)))) {
            break;
        }
        if (arg1 == 1) {
            if (rec->f_00 == 1) {
                if (rec->f_14 == 0) {
                    arg1 = 4;
                    rec->f_14 += 1;
                } else {
                    arg1 = 3;
                    rec->f_14 -= 1;
                }
            } else if (rec->f_10 == 0) {
                arg1 = (rec->f_00 == 3) ? 4 : 3;
                rec->f_00 = 1;
            } else {
                rec->f_10 -= 1;
            }
        } else if (arg1 == 2) {
            if (rec->f_00 == 2) {
                if (rec->f_14 == 0) {
                    arg1 = 4;
                    rec->f_14 += 1;
                } else {
                    arg1 = 3;
                    rec->f_14 -= 1;
                }
            } else if (rec->f_10 == arg0->f_194 - 1) {
                arg1 = (rec->f_00 == 3) ? 4 : 3;
                rec->f_00 = 2;
            } else {
                rec->f_10 += 1;
            }
        } else if (arg1 == 3) {
            if (rec->f_00 == 3) {
                if (rec->f_10 == 0) {
                    arg1 = 2;
                    rec->f_10 += 1;
                } else {
                    arg1 = 1;
                    rec->f_10 -= 1;
                }
            } else if (rec->f_14 == 0) {
                arg1 = (rec->f_00 == 1) ? 2 : 1;
                rec->f_00 = 3;
            } else {
                rec->f_14 -= 1;
            }
        } else if (arg1 == 4) {
            if (rec->f_00 == 4) {
                if (rec->f_10 == 0) {
                    arg1 = 2;
                    rec->f_10 += 1;
                } else {
                    arg1 = 1;
                    rec->f_10 -= 1;
                }
            } else if (rec->f_14 == arg0->f_194 - 1) {
                arg1 = (rec->f_00 == 1) ? 2 : 1;
                rec->f_00 = 4;
            } else {
                rec->f_14 += 1;
            }
        } else {
            if (rec->f_00 == 1) {
                if (rec->f_14 == 0) {
                    rec->f_00 = 3;
                } else {
                    rec->f_14 -= 1;
                }
            } else if (rec->f_00 == 2) {
                if (rec->f_14 == arg0->f_194 - 1) {
                    rec->f_00 = 4;
                } else {
                    rec->f_14 += 1;
                }
            } else if (rec->f_00 == 3) {
                if (rec->f_10 == arg0->f_194 - 1) {
                    rec->f_00 = 2;
                } else {
                    rec->f_10 += 1;
                }
            } else if (rec->f_00 == 4) {
                if (rec->f_10 == 0) {
                    rec->f_00 = 1;
                } else {
                    rec->f_10 -= 1;
                }
            }
        }
    }
}
