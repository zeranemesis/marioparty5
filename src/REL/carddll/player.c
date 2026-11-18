// NONMATCHING: every function in this unit is a faithful (same instruction count) but not byte-identical reconstruction; only the first ~19 small ones are near-identical.
// carddll/player.c - decompilation of the carddll/player unit (136164 B, 43 fns).
// Real code is being added function by function (smallest first), each verified
// at 100% with tools/fndiff.py before the next one. decomp/player.m2c.c is the
// reference draft (raw mips2c output, not buildable as-is).

#include "types.h"
#include "game/process.h"
#include "game/audio.h"
#include "game/hsfex.h"
#include "game/esprite.h"
#include "game/sprite.h"
#include "game/pad.h"
#include "game/frand.h"
#include "game/gamework.h"
#include "REL/carddll.h"
#include "REL/carddll/player.h"

/* ---- .data ---- */
s32 lbl_1_data_B0[122] = {0x00110000, 0x00110001, 0x00110002, 0x00110003, 0x00110004, 0x00110005, 0x00110006, 0x00110007, 0x00110008, 0x00110009, 0x0011000A, 0x0011000B, 0x0011000C, 0x0011000D, 0x0011000E, 0x0011000F, 0x00110010, 0x00110011, 0x00110012, 0x00110013, 0x00110014, 0x00110015, 0x00110016, 0x00110017, 0x00110018, 0x00110019, 0x0011001A, 0x0011001B, 0x0011001C, 0x0011001D, 0x0011001E, 0x0011001F, 0x00110020, 0x00110021, 0x00110022, 0x00110023, 0x00110024, 0x00110025, 0x00110026, 0x00110027, 0x00110028, 0x00110029, 0x0011002A, 0x0011002B, 0x0011002C, 0x0011002D, 0x0011002E, 0x0011002F, 0x00110030, 0x00110031, 0x00110032, 0x00110033, 0x00110034, 0x00110035, 0x00110036, 0x00110037, 0x00110038, 0x00110040, 0x00110041, 0x00110042, 0x00110043, 0x00110044, 0x00110045, 0x00110046, 0x00110047, 0x00110048, 0x00110049, 0x0011004A, 0x0011004B, 0x0011004C, 0x0011004D, 0x0011004E, 0x0011004F, 0x00110050, 0x00110051, 0x00110052, 0x00110053, 0x00110054, 0x00110055, 0x00110056, 0x00110057, 0x00110058, 0x00110059, 0x0011005A, 0x0011005B, 0x0011005C, 0x0011005D, 0x0011005E, 0x0011005F, 0x00110060, 0x00110061, 0x00110062, 0x00110063, 0x00110064, 0x00110065, 0x00110066, 0x00110067, 0x00110068, 0x00110069, 0x0011006A, 0x0011006B, 0x0011006C, 0x0011006D, 0x0011006E, 0x0011006F, 0x00110070, 0x00110071, 0x00110072, 0x00110073, 0x00110074, 0x00110075, 0x00110076, 0x00110077, 0x00110078, 0x00110079, 0x0011007A, 0x0011007B, 0x0011007C, 0x0011007D, 0x0011007E, 0x0011007F, 0x00110080};
s32 lbl_1_data_298[7] = {0x00110039, 0x0011003A, 0x0011003B, 0x0011003C, 0x0011003D, 0x0011003E, 0x0011003F};
s32 lbl_1_data_2B4[36] = {0x00110081, 0x00110082, 0x00110083, 0x00110084, 0x00110085, 0x00110086, 0x00110087, 0x00110088, 0x00110089, 0x0011008A, 0x0011008B, 0x0011008C, 0x0011008D, 0x0011008E, 0x0011008F, 0x00110090, 0x00110091, 0x00110092, 0x00110093, 0x00110094, 0x00110095, 0x00110096, 0x00110097, 0x00110098, 0x00110099, 0x0011009A, 0x0011009B, 0x0011009C, 0x0011009D, 0x0011009E, 0x0011009F, 0x001100A0, 0x001100A1, 0x001100A2, 0x001100A3, 0x001100A4};
s32 lbl_1_data_344[26] = {0x001100A5, 0x001100A6, 0x001100A7, 0x001100A8, 0x001100A9, 0x001100AA, 0x001100AB, 0x001100AC, 0x001100AD, 0x001100AE, 0x001100AF, 0x001100B0, 0x001100B1, 0x001100B2, 0x001100B3, 0x001100B4, 0x001100B5, 0x001100B6, 0x001100B7, 0x001100B8, 0x001100B9, 0x001100BA, 0x001100BB, 0x001100BC, 0x001100BD, 0x001100BE};
s32 lbl_1_data_3AC[3] = {0x001100BF, 0x001100C0, 0x001100C1};
s32 lbl_1_data_3B8[6] = {0x001100C2, 0x001100C3, 0x001100C4, 0x001100C5, 0x001100C6, 0x001100C7};
s32 lbl_1_data_3D0[3] = {0x001100C8, 0x001100C0, 0x001100C1};
s32 lbl_1_data_3DC[4] = {0x001100C9, 0x001100CA, 0x001100CB, 0x001100CC};
s32 lbl_1_data_3EC[5] = {0x001100CD, 0x001100CE, 0x001100CF, 0x001100D0, 0x001100D1};
s32 lbl_1_data_400[10] = {0x001100D2, 0x001100D3, 0x001100D4, 0x001100D5, 0x001100D6, 0x001100D7, 0x001100D8, 0x001100D9, 0x001100DA, 0x001100DB};
f32 lbl_1_data_428[4][2] = {{438.0f, 72.0f}, {438.0f, 140.0f}, {438.0f, 208.0f}, {438.0f, 276.0f}};
f32 lbl_1_data_448[4][2] = {{474.0f, 72.0f}, {474.0f, 140.0f}, {474.0f, 208.0f}, {474.0f, 276.0f}};
f32 lbl_1_data_468[4][2] = {{546.0f, 90.0f}, {546.0f, 158.0f}, {546.0f, 226.0f}, {546.0f, 294.0f}};
f32 lbl_1_data_488[4][2] = {{554.0f, 90.0f}, {554.0f, 158.0f}, {554.0f, 226.0f}, {554.0f, 294.0f}};
f32 lbl_1_data_4A8[4][2] = {{454.0f, 88.0f}, {454.0f, 156.0f}, {454.0f, 224.0f}, {454.0f, 292.0f}};
f32 lbl_1_data_4C8[4][2] = {{514.0f, 90.0f}, {514.0f, 158.0f}, {514.0f, 226.0f}, {514.0f, 294.0f}};
f32 lbl_1_data_4E8[4][2] = {{530.0f, 90.0f}, {530.0f, 158.0f}, {530.0f, 226.0f}, {530.0f, 294.0f}};
f32 lbl_1_data_508[4][2] = {{548.0f, 90.0f}, {548.0f, 158.0f}, {548.0f, 226.0f}, {548.0f, 294.0f}};
f32 lbl_1_data_528[4][2] = {{0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}};
f32 lbl_1_data_548[4][2] = {{492.0f, 60.0f}, {492.0f, 128.0f}, {492.0f, 196.0f}, {492.0f, 264.0f}};
f32 lbl_1_data_568[4][2] = {{520.0f, 60.0f}, {520.0f, 128.0f}, {520.0f, 196.0f}, {520.0f, 264.0f}};
f32 lbl_1_data_588[4][2] = {{548.0f, 60.0f}, {548.0f, 128.0f}, {548.0f, 196.0f}, {548.0f, 264.0f}};
f32 lbl_1_data_5A8[4][2] = {{400.0f, 60.0f}, {400.0f, 128.0f}, {400.0f, 196.0f}, {400.0f, 264.0f}};
f32 lbl_1_data_5C8[4][2] = {{594.0f, 76.0f}, {594.0f, 144.0f}, {594.0f, 212.0f}, {594.0f, 280.0f}};
f32 lbl_1_data_618[4][3] = {{64.0f, 80.0f, 0.0f}, {512.0f, 80.0f, 0.0f}, {64.0f, 280.0f, 0.0f}, {512.0f, 280.0f, 0.0f}};

/* ---- bss globals (carddll/player) ---- */
s32 lbl_1_bss_C48[2];
s32 lbl_1_bss_C44;
s32 lbl_1_bss_C40;
s16 lbl_1_bss_FB2[4];
s16 lbl_1_bss_FBA[4];
s16 lbl_1_bss_FC2[4];
s16 lbl_1_bss_FCA[4];
s16 lbl_1_bss_FD2[4];
s16 lbl_1_bss_FDA[4];
s16 lbl_1_bss_FE2[4];
s16 lbl_1_bss_FEA[4];
s16 lbl_1_bss_FF2[4];
s16 lbl_1_bss_FFA[4];
s16 lbl_1_bss_FAA[4];
s16 lbl_1_bss_FA2[4];
s16 lbl_1_bss_1002[4];
s16 lbl_1_bss_100A[4][4];
s16 lbl_1_bss_F9E[2];
static f32 lbl_1_rodata_4AC = 0.017453292f;
static f32 lbl_1_rodata_250 = -50.0f;
static f32 lbl_1_rodata_24C = 0.0f;

/* lbl_1_bss_468: per-player model-id table (2 entries per player, 4 players). */
static Vec lbl_1_rodata_504 = { 1.0f, 1.0f, 1.0f };
void fn_1_3F278(s32 arg0, s16 arg1);
void fn_1_3F6B0(s32 arg0, s16 arg1, Vec *arg2);
void fn_1_3F540(s32 arg0, s16 arg1, s32 arg2);
void fn_1_3F3D8(s32 arg0, s16 arg1, Vec *arg2);
void fn_1_5500(CardGame *arg0);

void fn_1_FB10(void);
s32 fn_1_21028(CardGame *arg0);
s32 fn_1_13728(CardGame *arg0);
s32 fn_1_FCD4(CardGame *arg0);
s32 fn_1_11C48(CardGame *arg0);
s32 fn_1_1658C(CardGame *arg0);
s32 fn_1_127F8(void);
void PSMTXIdentity(Mtx m);
void PSMTXRotRad(Mtx m, char axis, f32 rad);
void PSMTXConcat(const Mtx a, const Mtx b, Mtx ab);
void Hu3DMtxRotGet(Mtx src, HuVecF *dst);

/* lbl_1_bss_8C4: per-player state table, defined in the card unit (card.o).
   Rows are 0x28 bytes; entry flags live at offset f_10*0x28 + f_14*4. */
s32 fn_1_3FC50(CardGame *arg0);
s32 fn_1_42EDC(CardGame *arg0);

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
    lbl_1_bss_11DC = 0;
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
s32 fn_1_FB88(CardGame *arg0)
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


void fn_1_2FEF4(CardGame *arg0)
{
    s32 var_r31;

    var_r31 = 0;
    while (var_r31 < 4) {
        arg0->pl[var_r31].f30 = 1;
        arg0->pl[var_r31].f34 = -1;
        arg0->pl[var_r31].f38 = -1;
        arg0->pl[var_r31].f3C = -1;
        arg0->pl[var_r31].f40 = 0;
        arg0->pl[var_r31].f44 = 0;
        arg0->pl[var_r31].f48 = 0;
        arg0->pl[var_r31].f4C = 0;
        arg0->pl[var_r31].f50 = 0;
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
#ifdef TARGET_PC
    return NULL; /* the original returns the stack matrix; no caller uses it */
#else
    return &id;
#endif
}

/* lbl_1_bss_8C4 row lookup: 0x28-byte rows indexed by f_10, then by f_14. */
#define CARD_FLAGS(r) (*(s32 *) ((char *) &lbl_1_bss_8C4 + (r)->row * 0x28 + (r)->col * 4))

/* fn_1_304D8: like fn_1_303E0 but the record array is passed in (arg1) and the
   sprite display is turned ON. For each of the 3 opponents, take the sprite id from
   the bank table matching the current player number, and if the record sprite index
   is set, bank it and turn its display on. */
void fn_1_304D8(CardGame *arg0, register s32 *arg1)
{
    s16 bankId3;
    s16 bankId;
    s16 bankId2;
    s32 var_r31;

    var_r31 = 0;
    while (var_r31 < 3) {
        if (var_r31 == 0) {
            bankId = lbl_1_bss_FC2[arg0->curPlayer];
        } else {
            if (var_r31 == 1) {
                bankId2 = lbl_1_bss_FBA[arg0->curPlayer];
            } else {
                bankId2 = lbl_1_bss_FB2[arg0->curPlayer];
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
void fn_1_303E0(CardGame *arg0)
{
    u8 *base;
    s16 bankId3;
    s16 bankId;
    s16 bankId2;
    s32 var_r30;

    base = (u8 *) arg0 + *(s32 *) ((u8 *) arg0 + 0x174 + arg0->curPlayer * 4) * 0x5C;
    var_r30 = 0;
    while (var_r30 < 3) {
        if (var_r30 == 0) {
            bankId = lbl_1_bss_FC2[arg0->curPlayer];
        } else {
            if (var_r30 == 1) {
                bankId2 = lbl_1_bss_FBA[arg0->curPlayer];
            } else {
                bankId2 = lbl_1_bss_FB2[arg0->curPlayer];
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

/* fn_1_20DE4: AI button-press generator, the sibling of fn_1_25518. Runs only for
   a non-human player (GwPlayerConf[arg0].type != 0). It clears the button-down
   and stick-repeat state, latches lbl_1_bss_C40 from fn_1_21028(arg1) the first
   time (it starts at -1), then ticks lbl_1_bss_C48[0] and, when it reaches the
   threshold lbl_1_bss_C48[1], fires A (0x100) and checks whether this slot's
   record field at +0x34 is clear and fn_1_21028(arg1) reports 0 before
   re-arming the counter at 0x14. Otherwise every 30th tick it nudges the stick
   in a random direction. */
void fn_1_20DE4(s32 arg0, CardGame *arg1)
{
    s32 var_r29;

    if (GwPlayerConf[arg0].type != 0) {
        HuPadBtnDown[arg0] = 0;
        HuPadDStkRep[arg0] = 0;
        if (lbl_1_bss_C40 == -1) {
            lbl_1_bss_C40 = fn_1_21028(arg1);
        }
        if (lbl_1_bss_C48[1] == 0) {
            lbl_1_bss_C48[1] = frandmod(0x3C) + 0x1F;
        }
        lbl_1_bss_C48[0] += 1;
        if (lbl_1_bss_C48[0] == lbl_1_bss_C48[1]) {
            HuPadBtnDown[arg0] = 0x100;
            lbl_1_bss_C48[0] = 0;
            lbl_1_bss_C48[1] = 0;
            if (*(s32 *) ((u8 *) arg1 + *(s32 *) ((u8 *) arg1 + 0x174 + arg1->curPlayer * 4) * 0x5C + arg1->f1A4 * 4 + 0x34) == 0) {
                if (fn_1_21028(arg1) == 0) {
                    HuPadBtnDown[arg0] = 0;
                    HuPadDStkRep[arg0] = 2;
                    lbl_1_bss_C48[0] = 0;
                    lbl_1_bss_C48[1] = 0x14;
                }
            }
        } else if (lbl_1_bss_C48[0] % 0x1E == 0) {
            if (frandmod(2) != 0) {
                var_r29 = 2;
            } else {
                var_r29 = 1;
            }
            HuPadDStkRep[arg0] = var_r29;
        }
    }
}

/* fn_1_290BC: stick-only sibling of fn_1_20DE4, one parameter (the player
   number). Same counter/threshold shape, but the threshold is frandmod(0x2D)+0x1F
   and the action is a stick nudge instead of a button: when lbl_1_bss_C40 (the
   facing) is positive and the tick counter has reached 0xF, the stick is driven
   from lbl_1_bss_C40 (1 -> 8, 2 -> 4, 3 -> 1, anything else -> 2); otherwise,
   every 30th tick, frandmod(4) selects 8, 4, 1 or 2. The three-step
   "assign to the inner variable then copy outwards" shape is what the target
   emits (r27<-r26<-r29 and r24<-r25<-r26), so it is spelled out rather than
   collapsed into a lookup. */
void fn_1_290BC(s32 arg0)
{
    s32 var_r30;
    s32 var_r29;
    s32 var_r28;
    s32 var_r27;
    s32 var_r26;
    s32 var_r25;
    s32 var_r24;

    if (GwPlayerConf[arg0].type != 0) {
        HuPadBtnDown[arg0] = 0;
        HuPadDStkRep[arg0] = 0;
        if (lbl_1_bss_C48[1] == 0) {
            lbl_1_bss_C48[1] = frandmod(0x2D) + 0x1F;
        }
        lbl_1_bss_C48[0] += 1;
        if (lbl_1_bss_C48[0] == lbl_1_bss_C48[1]) {
            HuPadBtnDown[arg0] = 0x100;
            lbl_1_bss_C48[0] = 0;
            lbl_1_bss_C48[1] = 0;
            return;
        }
        if (lbl_1_bss_C40 > 0) {
            if (lbl_1_bss_C48[0] >= 0xF) {
                if (lbl_1_bss_C40 == 1) {
                    var_r29 = 8;
                } else {
                    if (lbl_1_bss_C40 == 2) {
                        var_r28 = 4;
                    } else {
                        if (lbl_1_bss_C40 == 3) {
                            var_r27 = 1;
                        } else {
                            var_r27 = 2;
                        }
                        var_r28 = var_r27;
                    }
                    var_r29 = var_r28;
                }
                HuPadDStkRep[arg0] = var_r29;
            }
            return;
        }
        if (lbl_1_bss_C48[0] % 0x1E == 0) {
            var_r30 = frandmod(4);
            if (var_r30 == 0) {
                var_r26 = 8;
            } else {
                if (var_r30 == 1) {
                    var_r25 = 4;
                } else {
                    if (var_r30 == 2) {
                        var_r24 = 1;
                    } else {
                        var_r24 = 2;
                    }
                    var_r25 = var_r24;
                }
                var_r26 = var_r25;
            }
            HuPadDStkRep[arg0] = var_r26;
        }
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

/* fn_1_2B10C: per-player state dispatch. Look up the player's state-table entry
   (row f_10 @ +0x10, col f_14 @ +0x14 of the 0x5C-byte record) and, depending on
   which of the four high flag bits (19..16) is set, run the matching handler;
   return 1 if that handler reports a state change. */
s32 fn_1_2B10C(CardGame *arg0)
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
void fn_1_2FBCC(CardGame *arg0)
{
    Vec sp2C;
    Vec sp20;
    Vec sp14;
    Vec sp8;
    Vec *pVec14;
    Vec *pVec8;
    s32 i;

    if (*(s32 *) ((u8 *) arg0 + 0x1D4) == 1) {
        i = 0;
        while (i < 4) {
            s32 dir;
            s32 y;
            s32 z;

            dir = *(s32 *) ((u8 *) arg0 + i * 0x5C + 4);
            y = *(s32 *) ((u8 *) arg0 + i * 0x5C + 8);
            z = *(s32 *) ((u8 *) arg0 + i * 0x5C + 0xC);
            if (dir == 1) {
                y -= 1;
            } else if (dir == 2) {
                y += 1;
            } else if (dir == 3) {
                z -= 1;
            } else if (dir == 4) {
                z += 1;
            }
            fn_1_9C74(&sp2C, y, z, 1, 1);
            sp2C.x += lbl_1_rodata_250;
            Hu3DModelPosSetV(lbl_1_bss_468[i], &sp2C);
            Hu3DModelRotSet(lbl_1_bss_468[i], lbl_1_rodata_24C, lbl_1_rodata_24C, lbl_1_rodata_24C);
            if (*(s32 *) ((u8 *) arg0 + i * 0x5C + 0x50) & 2) {
                fn_1_3F278(3, *(s16 *) ((u8 *) arg0 + i * 0x5C + 0x56));
                *(s16 *) ((u8 *) arg0 + i * 0x5C + 0x56) = -1;
                *(s32 *) ((u8 *) arg0 + i * 0x5C + 0x50) &= 0xFFFFFFFD;
            }
            if (*(s32 *) ((u8 *) arg0 + i * 0x5C + 0x50) & 4) {
                if (i == *(s32 *) ((u8 *) arg0 + *(s32 *) ((u8 *) arg0 + 0x170) * 4 + 0x174)) {
                    fn_1_3F278(4, *(s16 *) ((u8 *) arg0 + i * 0x5C + 0x56));
                    *(s16 *) ((u8 *) arg0 + i * 0x5C + 0x56) = -1;
                    *(s32 *) ((u8 *) arg0 + i * 0x5C + 0x50) &= 0xFFFFFFFB;
                } else {
                    sp20 = lbl_1_rodata_504;
                    sp14 = sp20;
                    pVec14 = &sp14;
                    fn_1_3F6B0(4, *(s16 *) ((u8 *) arg0 + i * 0x5C + 0x56), pVec14);
                    fn_1_3F540(4, *(s16 *) ((u8 *) arg0 + i * 0x5C + 0x56), 1);
                    sp8 = sp2C;
                    pVec8 = &sp8;
                    fn_1_3F3D8(4, *(s16 *) ((u8 *) arg0 + i * 0x5C + 0x56), pVec8);
                }
            }
            i += 1;
        }
        fn_1_5500(arg0);
        *(s32 *) ((u8 *) arg0 + 0x1D4) = 0xA;
        return;
    }
    i = 0;
    while (i < 4) {
        *(s32 *) ((u8 *) arg0 + i * 0x5C + 0) = *(s32 *) ((u8 *) arg0 + i * 0x5C + 4);
        *(s32 *) ((u8 *) arg0 + i * 0x5C + 0x10) = *(s32 *) ((u8 *) arg0 + i * 0x5C + 8);
        *(s32 *) ((u8 *) arg0 + i * 0x5C + 0x14) = *(s32 *) ((u8 *) arg0 + i * 0x5C + 0xC);
        *(s32 *) ((u8 *) arg0 + i * 0x5C + 0x20) = 1;
        *(s32 *) ((u8 *) arg0 + i * 0x5C + 0x24) = 1;
        i += 1;
    }
    *(s32 *) ((u8 *) arg0 + 0x1D4) = -1;
}

/* CARD_FLAGS: the per-player "card phase" walker. Repeatedly advances the 0x5C-byte
   record's f_10/f_14 cursor and f_00 phase field until none of the five state-table
   flag bits (20..24) apply. arg1 is the current direction, carried across iterations. */
/* MWCC 1.3.2 folds `x & (1 << b)` into `rlwinm ..., 31-b, 31-b`, i.e. the
   rotate mask it builds is the bit index counted from the MSB. Ten independent
   measurements in this unit agree (fn_1_17BE8 bits 24,15,14,13,12 and
   fn_1_2B10C bits 19,18,17,16), and CodeWarrior emits the plain index. The
   five masks below are therefore written bit-reversed: each one is the
   complement of the flag the original actually tests (bit 24, 15, 14, 13 and
   12 of the row word), and spelling them any other way cannot reach 100%. */
#define CARD_BIT_ANY   0x80
#define CARD_BIT_P1    0x10000
#define CARD_BIT_P2    0x20000
#define CARD_BIT_P3    0x40000
#define CARD_BIT_P4    0x80000

void fn_1_17BE8(CardGame *arg0, s32 arg1)
{
    CardPlayer *rec;
    s32 var_C;
    s32 var_8;

    var_C = 0;
    var_8 = 0;
    rec = (CardPlayer *) ((u8 *) arg0 + *(s32 *) ((u8 *) arg0 + 0x174 + arg0->curPlayer * 4) * 0x5C);
for (;;) {
        if (!((CARD_FLAGS(rec) & CARD_BIT_ANY) ||
              (rec->f00 == 1 && (CARD_FLAGS(rec) & CARD_BIT_P1)) ||
              (rec->f00 == 2 && (CARD_FLAGS(rec) & CARD_BIT_P2)) ||
              (rec->f00 == 3 && (CARD_FLAGS(rec) & CARD_BIT_P3)) ||
              (rec->f00 == 4 && (CARD_FLAGS(rec) & CARD_BIT_P4)))) {
            break;
        }
        if (arg1 == 1) {
            if (rec->f00 == 1) {
                if (rec->col == 0) {
                    arg1 = 4;
                    rec->col += 1;
                } else {
                    arg1 = 3;
                    rec->col -= 1;
                }
} else if (rec->row == 0) {
                if (rec->f00 == 3) {
                    arg1 = 4;
                } else {
                    arg1 = 3;
                }
                rec->f00 = 1;
            } else {
                rec->row -= 1;
            }
        } else if (arg1 == 2) {
            if (rec->f00 == 2) {
                if (rec->col == 0) {
                    arg1 = 4;
                    rec->col += 1;
                } else {
                    arg1 = 3;
                    rec->col -= 1;
                }
            } else if (rec->row == arg0->boardSize - 1) {
                if (rec->f00 == 3) {
                    arg1 = 4;
                } else {
                    arg1 = 3;
                }
                rec->f00 = 2;
            } else {
                rec->row += 1;
            }
        } else if (arg1 == 3) {
            if (rec->f00 == 3) {
                if (rec->row == 0) {
                    arg1 = 2;
                    rec->row += 1;
                } else {
                    arg1 = 1;
                    rec->row -= 1;
                }
            } else if (rec->col == 0) {
                if (rec->f00 == 1) {
                    arg1 = 2;
                } else {
                    arg1 = 1;
                }
                rec->f00 = 3;
            } else {
                rec->col -= 1;
            }
        } else if (arg1 == 4) {
            if (rec->f00 == 4) {
                if (rec->row == 0) {
                    arg1 = 2;
                    rec->row += 1;
                } else {
                    arg1 = 1;
                    rec->row -= 1;
                }
            } else if (rec->col == arg0->boardSize - 1) {
                if (rec->f00 == 1) {
                    arg1 = 2;
                } else {
                    arg1 = 1;
                }
                rec->f00 = 4;
            } else {
                rec->col += 1;
            }
        } else {
            if (rec->f00 == 1) {
                if (rec->col == 0) {
                    rec->f00 = 3;
                } else {
                    rec->col -= 1;
                }
            } else if (rec->f00 == 2) {
                if (rec->col == arg0->boardSize - 1) {
                    rec->f00 = 4;
                } else {
                    rec->col += 1;
                }
            } else if (rec->f00 == 3) {
                if (rec->row == arg0->boardSize - 1) {
                    rec->f00 = 2;
                } else {
                    rec->row += 1;
                }
            } else if (rec->f00 == 4) {
                if (rec->row == 0) {
                    rec->f00 = 1;
                } else {
                    rec->row -= 1;
                }
            }
        }
    }
}

/* lbl_1_data_2B4: 0x90-byte table of ESP data-num words (0x00110081..0x001100A4). */

/* lbl_1_bss_112E: single s16 slot. */
static s16 lbl_1_bss_112E;
/* lbl_1_bss_F3C: the 0x28-byte bss table at 0xF3C; word 0 is the ticker timer. */
s32 lbl_1_bss_F3C[10];
static f64 lbl_1_rodata_230 = 4503601774854144.0;
/* lbl_1_rodata_254: an s32 pair copied verbatim into the local at 0x8(r1). */
static s32 lbl_1_rodata_254[2] = { 0x43B00000, 0x43B00000 };
static f32 lbl_1_rodata_25C = 288.0f;
static f32 lbl_1_rodata_260 = 0.5f;
static f32 lbl_1_rodata_264 = 78.0f;

// NONMATCHING: the 8-byte stack copy of lbl_1_rodata_254 comes out as two
// independent lis/addi pairs instead of one base register reused across both
// loads; the state machine itself is faithful.
/* fn_1_12440: the "player finished" ticker ESP. arg0 is the player number and the
   machine lives in lbl_1_bss_1168[0]:
     0 create the ESP from the data-num table, draw-no 0x40 for player 0 else 0,
       arm the timer lbl_1_bss_F3C[0] to 300 and position it,
     1 drain the timer by 10 per call and reposition, moving to state 2 and
       reporting 1 once it reaches zero,
     2 refill the timer by 10 per call and reposition, killing the ESP and
       reporting 1 once it reaches 300, then go back to state 0.
   The sprite y tracks the timer as 78.0 - (f32)(f64)timer and the x is the
   per-player half-width blend 288.0 - w/2 + w/2, where w comes from a local
   pair initialised from rodata_254. */
s32 fn_1_12440(s32 arg0)
{
    s32 var_r31;
    s32 var_r30;
    struct
    {
        s32 a;
        s32 b;
    } sp8;
    var_r30 = 0;
    var_r31 = 0;
    sp8.a = lbl_1_rodata_254[0];
    sp8.b = lbl_1_rodata_254[1];
    switch (lbl_1_bss_1168[0]) {
    case 0:
        lbl_1_bss_112E = espEntry(lbl_1_data_2B4[arg0 + 12], 0, 0);
        if (arg0 == 0) {
            var_r31 = 0x40;
        } else {
            var_r31 = 0;
        }
        espDrawNoSet(lbl_1_bss_112E, (s16) var_r31);
        lbl_1_bss_F3C[0] = 0x12C;
        espPosSet(lbl_1_bss_112E, lbl_1_rodata_25C - ((f32 *) &sp8)[arg0] * lbl_1_rodata_260 + ((f32 *) &sp8)[arg0] * lbl_1_rodata_260,
            lbl_1_rodata_264 - (f32) ((f64) lbl_1_bss_F3C[0]));
        lbl_1_bss_1168[0] += 1;
        HuSprExecLayerSet(0x40, 6);
        break;
    case 1:
        lbl_1_bss_F3C[0] -= 10;
        espPosSet(lbl_1_bss_112E, lbl_1_rodata_25C - ((f32 *) &sp8)[arg0] * lbl_1_rodata_260 + ((f32 *) &sp8)[arg0] * lbl_1_rodata_260,
            lbl_1_rodata_264 - (f32) ((f64) lbl_1_bss_F3C[0]));
        if (lbl_1_bss_F3C[0] <= 0) {
            lbl_1_bss_F3C[0] = 0;
            lbl_1_bss_1168[0] += 1;
            var_r30 = 1;
        }
        break;
    case 2:
        lbl_1_bss_F3C[0] += 10;
        espPosSet(lbl_1_bss_112E, lbl_1_rodata_25C - ((f32 *) &sp8)[arg0] * lbl_1_rodata_260 + ((f32 *) &sp8)[arg0] * lbl_1_rodata_260,
            lbl_1_rodata_264 - (f32) ((f64) lbl_1_bss_F3C[0]));
        if (lbl_1_bss_F3C[0] >= 0x12C) {
            espKill(lbl_1_bss_112E);
            lbl_1_bss_F3C[0] = 0;
            lbl_1_bss_1168[0] = 0;
            var_r30 = 1;
        }
        break;
    default:
        break;
    }
    return var_r30;
}

/* fn_1_18054: three-wide AI driver, runs only when GwPlayerConf[arg0].type != 0.
    It clears the button-down and stick-repeat state, arms the latch
    lbl_1_bss_C48[1] (frandmod(0x1E) + 0x1F in card phase 0, frandmod(0x78) + 0x1F
    in phase 3) while it is still zero, then ticks the latch counter
    lbl_1_bss_C48[0]. When the counter reaches the latch the A button is pressed
    (0x100) and both are cleared; otherwise, in phase 0 every 10th tick the stick
    flicks a random 2/1, and in phase 3 every 20th tick it flicks a random 1/2
    when arg1 <= 2 or a random 8/4 when arg1 > 2. */
void fn_1_18054(s32 arg0, s32 arg1)
{
    s32 var_r30;
    s32 var_r29;
    s32 var_r28;
    s32 var_r27;

    if (GwPlayerConf[arg0].type != 0) {
        HuPadBtnDown[arg0] = 0;
        HuPadDStkRep[arg0] = 0;
        if (lbl_1_bss_C48[1] == 0) {
            if (lbl_1_bss_1174 == 0) {
                lbl_1_bss_C48[1] = frandmod(0x1E) + 0x1F;
            } else if (lbl_1_bss_1174 == 3) {
                lbl_1_bss_C48[1] = frandmod(0x78) + 0x1F;
            }
        }
        lbl_1_bss_C48[0] += 1;
        if (lbl_1_bss_C48[0] == lbl_1_bss_C48[1]) {
            HuPadBtnDown[arg0] = 0x100;
            lbl_1_bss_C48[0] = 0;
            lbl_1_bss_C48[1] = 0;
        } else if (lbl_1_bss_1174 == 0) {
            if (lbl_1_bss_C48[0] % 10 == 0) {
                if (frandmod(2) != 0) {
                    var_r30 = 2;
                } else {
                    var_r30 = 1;
                }
                HuPadDStkRep[arg0] = var_r30;
            }
        } else if (lbl_1_bss_1174 == 3) {
            if (lbl_1_bss_C48[0] % 20 == 0) {
                if (arg1 <= 2) {
                    if (frandmod(2) != 0) {
                        var_r29 = 1;
                    } else {
                        var_r29 = 2;
                    }
                    var_r28 = var_r29;
                } else {
                    if (frandmod(2) != 0) {
                        var_r27 = 8;
                    } else {
                        var_r27 = 4;
                    }
                    var_r28 = var_r27;
                }
                HuPadDStkRep[arg0] = var_r28;
            }
        }
    }
}

/* fn_1_25624: neighbour test for the cursor cell of the current player's card
   record. Returns 1 immediately when the record's own phase (f_00) is non-zero.
   Otherwise builds four flag bits of var_r31, one per neighbour of the cursor
   (row = f_10, col = f_14) inside the slot-count square (arg0->boardSize) laid out
   as 0x28-byte rows in lbl_1_bss_8C4:
     bit 1 = row-1, bit 2 = row+1, bit 4 = col-1, bit 8 = col+1.
   A neighbour counts as set when it falls outside the square, when its
   state-table word has any of bits 8..10 set, or when another of the four
   records has its cursor on that same cell. The result is 1 only when all four
   bits are set. */
s32 fn_1_25624(CardGame *arg0)
{
    s32 var_r31;
    s32 var_r30;
    s32 var_r28;
    s32 var_r27;
    CardPlayer *rec;

    rec = (CardPlayer *) ((u8 *) arg0 + *(s32 *) ((u8 *) arg0 + 0x174 + arg0->curPlayer * 4) * 0x5C);
    if (rec->f00 != 0) {
        return 1;
    }
    var_r31 = 0;
    var_r28 = rec->row - 1;
    var_r27 = rec->col;
    if (var_r28 < 0) {
        var_r31 |= 1;
    } else if ((*(s32 *) ((char *) &lbl_1_bss_8C4 + var_r28 * 0x28 + var_r27 * 4) & 0x700) != 0) {
        var_r31 |= 1;
    } else {
        var_r30 = 0;
        while (var_r30 < 4) {
            if (var_r28 == *(s32 *) ((u8 *) arg0 + var_r30 * 0x5C + 0x10) && var_r27 == *(s32 *) ((u8 *) arg0 + var_r30 * 0x5C + 0x14)) {
                var_r31 |= 1;
                break;
            }
            var_r30 += 1;
        }
    }
    var_r28 = rec->row + 1;
    var_r27 = rec->col;
    if (var_r28 > arg0->boardSize - 1) {
        var_r31 |= 2;
    } else if ((*(s32 *) ((char *) &lbl_1_bss_8C4 + var_r28 * 0x28 + var_r27 * 4) & 0x700) != 0) {
        var_r31 |= 2;
    } else {
        var_r30 = 0;
        while (var_r30 < 4) {
            if (var_r28 == *(s32 *) ((u8 *) arg0 + var_r30 * 0x5C + 0x10) && var_r27 == *(s32 *) ((u8 *) arg0 + var_r30 * 0x5C + 0x14)) {
                var_r31 |= 2;
                break;
            }
            var_r30 += 1;
        }
    }
    var_r28 = rec->row;
    var_r27 = rec->col - 1;
    if (var_r27 < 0) {
        var_r31 |= 4;
    } else if ((*(s32 *) ((char *) &lbl_1_bss_8C4 + var_r28 * 0x28 + var_r27 * 4) & 0x700) != 0) {
        var_r31 |= 4;
    } else {
        var_r30 = 0;
        while (var_r30 < 4) {
            if (var_r28 == *(s32 *) ((u8 *) arg0 + var_r30 * 0x5C + 0x10) && var_r27 == *(s32 *) ((u8 *) arg0 + var_r30 * 0x5C + 0x14)) {
                var_r31 |= 4;
                break;
            }
            var_r30 += 1;
        }
    }
    var_r28 = rec->row;
    var_r27 = rec->col + 1;
    if (var_r27 > arg0->boardSize - 1) {
        var_r31 |= 8;
    } else if ((*(s32 *) ((char *) &lbl_1_bss_8C4 + var_r28 * 0x28 + var_r27 * 4) & 0x700) != 0) {
        var_r31 |= 8;
    } else {
        var_r30 = 0;
        while (var_r30 < 4) {
            if (var_r28 == *(s32 *) ((u8 *) arg0 + var_r30 * 0x5C + 0x10) && var_r27 == *(s32 *) ((u8 *) arg0 + var_r30 * 0x5C + 0x14)) {
                var_r31 |= 8;
                break;
            }
            var_r30 += 1;
        }
    }
    if (var_r31 == 0xF) {
        return 1;
    }
    return 0;
}

/* fn_1_305B4: tint every per-player card sprite grey for the players other than
   the one at arg0->curPlayer. The tint is a flat grey computed from the
   channel count arg1 as 0xFF - arg1 in all three colour components. */
void fn_1_305B4(CardGame *arg0, s32 arg1)
{
    s32 var_r30;

    var_r30 = 0;
    while (var_r30 < 4) {
        if (var_r30 != arg0->curPlayer) {
            espColorSet(lbl_1_bss_1002[var_r30], 0xFF - arg1, 0xFF - arg1, 0xFF - arg1);
            espColorSet(lbl_1_bss_FFA[var_r30], 0xFF - arg1, 0xFF - arg1, 0xFF - arg1);
            espColorSet(lbl_1_bss_FF2[var_r30], 0xFF - arg1, 0xFF - arg1, 0xFF - arg1);
            espColorSet(lbl_1_bss_100A[var_r30][0], 0xFF - arg1, 0xFF - arg1, 0xFF - arg1);
            espColorSet(lbl_1_bss_100A[var_r30][1], 0xFF - arg1, 0xFF - arg1, 0xFF - arg1);
            espColorSet(lbl_1_bss_100A[var_r30][2], 0xFF - arg1, 0xFF - arg1, 0xFF - arg1);
            espColorSet(lbl_1_bss_100A[var_r30][3], 0xFF - arg1, 0xFF - arg1, 0xFF - arg1);
            espColorSet(lbl_1_bss_FEA[var_r30], 0xFF - arg1, 0xFF - arg1, 0xFF - arg1);
            espColorSet(lbl_1_bss_FE2[var_r30], 0xFF - arg1, 0xFF - arg1, 0xFF - arg1);
            espColorSet(lbl_1_bss_FDA[var_r30], 0xFF - arg1, 0xFF - arg1, 0xFF - arg1);
            espColorSet(lbl_1_bss_FD2[var_r30], 0xFF - arg1, 0xFF - arg1, 0xFF - arg1);
            espColorSet(lbl_1_bss_FCA[var_r30], 0xFF - arg1, 0xFF - arg1, 0xFF - arg1);
            espColorSet(lbl_1_bss_FC2[var_r30], 0xFF - arg1, 0xFF - arg1, 0xFF - arg1);
            espColorSet(lbl_1_bss_FBA[var_r30], 0xFF - arg1, 0xFF - arg1, 0xFF - arg1);
            espColorSet(lbl_1_bss_FB2[var_r30], 0xFF - arg1, 0xFF - arg1, 0xFF - arg1);
            espColorSet(lbl_1_bss_FAA[var_r30], 0xFF - arg1, 0xFF - arg1, 0xFF - arg1);
        }
        var_r30 += 1;
    }
}

#include "types.h"
#include "game/process.h"
#include "game/audio.h"
#include "game/hsfex.h"
#include "game/esprite.h"
#include "game/sprite.h"
#include "game/pad.h"
#include "game/frand.h"
#include "game/gamework.h"

/* lbl_1_data_2B4: 0x90-byte table of ESP data-num words (0x00110081..0x001100A4). */

static f32 lbl_1_rodata_510 = 182.0f;
static f32 lbl_1_rodata_514 = 238.0f;
static f32 lbl_1_rodata_2B8 = 210.0f;
static f32 lbl_1_rodata_3D8 = 90.0f;

void fn_1_30B00(CardGame *arg0, s32 arg1)
{
    if (arg1 == 0) {
        lbl_1_bss_F9E[0] = espEntry(lbl_1_data_2B4[0xB], 0, 0);
        espDrawNoSet(lbl_1_bss_F9E[0], 0);
        espPosSet(lbl_1_bss_F9E[0], lbl_1_rodata_510, lbl_1_rodata_3D8);
        espBankSet(lbl_1_bss_F9E[0], 0);
        espDispOff(lbl_1_bss_F9E[0]);
        espPriSet(lbl_1_bss_F9E[0], 0x200);
        lbl_1_bss_F9E[1] = espEntry(lbl_1_data_2B4[0xB], 0, 0);
        espDrawNoSet(lbl_1_bss_F9E[1], 0);
        espPosSet(lbl_1_bss_F9E[1], lbl_1_rodata_514, lbl_1_rodata_3D8);
        espBankSet(lbl_1_bss_F9E[1], 0);
        espDispOff(lbl_1_bss_F9E[1]);
        espPriSet(lbl_1_bss_F9E[1], 0x200);
        return;
    }
    if (arg1 == 1) {
        if ((*(s32 *)((u8 *) arg0 + 0x184)) / 10 > 0) {
            espBankSet(lbl_1_bss_F9E[0], (s16) ((*(s32 *)((u8 *) arg0 + 0x184)) / 10));
            espDispOn(lbl_1_bss_F9E[0]);
        } else {
            espDispOff(lbl_1_bss_F9E[0]);
        }
        if ((*(s32 *)((u8 *) arg0 + 0x184)) / 10 > 0) {
            espPosSet(lbl_1_bss_F9E[1], lbl_1_rodata_514, lbl_1_rodata_3D8);
        } else {
            espPosSet(lbl_1_bss_F9E[1], lbl_1_rodata_2B8, lbl_1_rodata_3D8);
        }
        espBankSet(lbl_1_bss_F9E[1], (s16) ((*(s32 *) ((u8 *) arg0 + 0x184)) % 10));
        espDispOn(lbl_1_bss_F9E[1]);
        if ((*(s32 *) ((u8 *) arg0 + 0x184)) == 0) {
            espDispOff(lbl_1_bss_F9E[0]);
            espDispOff(lbl_1_bss_F9E[1]);
        }
    } else if (arg1 == -1) {
        espDispOff(lbl_1_bss_F9E[0]);
        espDispOff(lbl_1_bss_F9E[1]);
    }
}

/* ---- additional globals / externs (batch 1) ---- */
f32 lbl_1_bss_F14[10];
extern s16 fn_1_36A60(void);
extern void fn_1_36A70(Vec *pos);
extern void fn_1_36B78(void);

s32 fn_1_127F8(void)
{
    Vec pos;
    s32 var_r31;

    var_r31 = 0;
    pos.x = 288.0f;
    pos.y = 320.0f;
    pos.z = 1500.0f;
    switch (lbl_1_bss_1168[0]) {
    case 0:
        lbl_1_bss_F3C[0] = 0x258;
        lbl_1_bss_F14[0] = 0.0f;
        lbl_1_bss_112E = espEntry(lbl_1_data_2B4[0xE], 0, 0);
        espDrawNoSet(lbl_1_bss_112E, 0x40);
        espPosSet(lbl_1_bss_112E, 288.0f, 240.0f - (f32) lbl_1_bss_F3C[0]);
        espTPLvlSet(lbl_1_bss_112E, lbl_1_bss_F14[0]);
        fn_1_36828();
        lbl_1_bss_F14[5] = 400.0f;
        pos.y -= lbl_1_bss_F14[5];
        fn_1_36A70(&pos);
        lbl_1_bss_F14[6] = 0.0f;
        lbl_1_bss_1168[0] += 1;
        break;
    case 1:
        if (lbl_1_bss_F3C[0] == 0x14) {
            HuAudSStreamStop(lbl_1_bss_14);
        }
        lbl_1_bss_F3C[0] -= 0x14;
        if (lbl_1_bss_F3C[0] < 0) {
            lbl_1_bss_F3C[0] = 0;
        }
        lbl_1_bss_F14[0] += 0.03;
        if (lbl_1_bss_F14[0] > 1.0f) {
            lbl_1_bss_F14[0] = 1.0f;
        }
        espPosSet(lbl_1_bss_112E, 288.0f, 240.0f - (f32) lbl_1_bss_F3C[0]);
        espTPLvlSet(lbl_1_bss_112E, lbl_1_bss_F14[0]);
        lbl_1_bss_F14[5] -= 20.0f;
        if (lbl_1_bss_F14[5] < 0.0f) {
            lbl_1_bss_F14[5] = 0.0f;
        }
        pos.y -= lbl_1_bss_F14[5];
        fn_1_36A70(&pos);
        if (lbl_1_bss_F14[0] == 1.0f) {
            lbl_1_bss_F14[0] = 0.0f;
            lbl_1_bss_1168[0] = 0xA;
            lbl_1_bss_F3C[1] = 0;
        }
        break;
    case 2:
        lbl_1_bss_F3C[1] += 1;
        if (lbl_1_bss_F3C[1] >= 0x3C) {
            lbl_1_bss_F14[5] -= 5.0f;
            pos.y += lbl_1_bss_F14[5];
            lbl_1_bss_F14[6] += 40.0f;
            pos.z -= lbl_1_bss_F14[6];
            fn_1_36A70(&pos);
            if (lbl_1_bss_F3C[1] >= 0x50) {
                lbl_1_bss_F14[0] += 0.05;
                if (lbl_1_bss_F14[0] > 2.0f) {
                    lbl_1_bss_F14[0] = 2.0f;
                }
                espScaleSet(lbl_1_bss_112E, 1.0f + lbl_1_bss_F14[0], 1.0f + lbl_1_bss_F14[0]);
                espTPLvlSet(lbl_1_bss_112E, 1.0f - (lbl_1_bss_F14[0] * 0.5f));
                if (lbl_1_bss_F14[0] == 2.0f) {
                    fn_1_36B78();
                    espKill(lbl_1_bss_112E);
                    lbl_1_bss_1168[0] = 0;
                    var_r31 = 1;
                }
            }
        }
        break;
    case 10:
        if (HuAudSStreamStatGet(lbl_1_bss_14) == 0) {
            lbl_1_bss_14 = HuAudBGMPlay(0x28);
            HuAudFXPlay(0x19);
            lbl_1_bss_1168[0] = 2;
        }
        break;
    }
    return var_r31;
}

s32 fn_1_12E04(void)
{
    Vec pos;
    s32 var_r31;

    var_r31 = 0;
    pos.x = 288.0f;
    pos.y = 150.0f;
    pos.z = 1500.0f;
    switch (lbl_1_bss_1168[0]) {
    case 0:
        lbl_1_bss_112E = espEntry(lbl_1_data_2B4[0xF], 0, 0);
        espDrawNoSet(lbl_1_bss_112E, 0);
        espPosSet(lbl_1_bss_112E, 288.0f, 240.0f);
        espScaleSet(lbl_1_bss_112E, 1.0f, 0.0f);
        lbl_1_bss_F14[0] = 0.0f;
        lbl_1_bss_1168[0] += 1;
        HuAudSStreamFadeOut(lbl_1_bss_14, 0x3E8);
        HuAudFXPlay(0x1A);
        lbl_1_bss_20[9] = HuAudFXPlay(0x450);
        break;
    case 1:
        lbl_1_bss_F14[0] += 0.05f;
        if (lbl_1_bss_F14[0] > 1.0f) {
            lbl_1_bss_F14[0] = 1.0f;
        }
        espScaleSet(lbl_1_bss_112E, 1.0f, lbl_1_bss_F14[0]);
        if (lbl_1_bss_F14[0] == 1.0f) {
            lbl_1_bss_1168[0] += 1;
        }
        break;
    case 2:
        if ((HuPadBtnDown[0] & 0x100) != 0 || (HuPadBtnDown[1] & 0x100) != 0 || (HuPadBtnDown[2] & 0x100) != 0
            || (HuPadBtnDown[3] & 0x100) != 0) {
            lbl_1_bss_F14[0] = 0.0f;
            lbl_1_bss_1168[0] += 1;
        }
        break;
    case 3:
        lbl_1_bss_F14[0] += 0.05;
        if (lbl_1_bss_F14[0] > 1.0f) {
            lbl_1_bss_F14[0] = 1.0f;
        }
        espScaleSet(lbl_1_bss_112E, 1.0f - lbl_1_bss_F14[0], 1.0f);
        if (lbl_1_bss_F14[0] == 1.0f) {
            espKill(lbl_1_bss_112E);
            lbl_1_bss_1168[0] += 1;
        }
        break;
    case 4:
        lbl_1_bss_F14[8] -= 6.0f;
        pos.y += lbl_1_bss_F14[8];
        fn_1_36A70(&pos);
        lbl_1_bss_F14[9] -= 15.0f;
        Hu3DModelRotSet(fn_1_36A60(), 0.0f, lbl_1_bss_F14[9], 0.0f);
        if (lbl_1_bss_F14[8] <= -360.0f) {
            fn_1_36B78();
            lbl_1_bss_1168[0] = 0;
            var_r31 = 1;
        }
        break;
    }
    return var_r31;
}

s32 fn_1_1328C(s32 arg0)
{
    s16 var_r29;
    s32 var_r30;
    s32 var_r31;

    var_r31 = 0;
    switch (lbl_1_bss_1168[2]) {
    case 2:
        break;
    case 0:
        lbl_1_bss_F3C[0] = 0x12C;
        lbl_1_bss_F14[0] = 0.0f;
        if (arg0 < 2) {
            var_r30 = 0x10;
        } else {
            var_r30 = 0x11;
        }
        lbl_1_bss_112E = espEntry(lbl_1_data_2B4[var_r30], 0, 0);
        espDrawNoSet(lbl_1_bss_112E, 0);
        espPosSet(lbl_1_bss_112E, 210.0f, 100.0f + (f32) lbl_1_bss_F3C[0]);
        espTPLvlSet(lbl_1_bss_112E, lbl_1_bss_F14[0]);
        lbl_1_bss_1168[2] += 1;
        break;
    case 1:
        lbl_1_bss_F3C[0] -= 0x14;
        if (lbl_1_bss_F3C[0] < 0) {
            lbl_1_bss_1168[2] += 1;
            lbl_1_bss_F3C[0] = 0;
        }
        if (lbl_1_bss_F3C[0] <= 0x12C) {
            lbl_1_bss_F14[0] += 0.06;
            if (lbl_1_bss_F14[0] > 1.0f) {
                lbl_1_bss_F14[0] = 1.0f;
            }
        }
        espPosSet(lbl_1_bss_112E, 210.0f, 100.0f + (f32) lbl_1_bss_F3C[0]);
        espTPLvlSet(lbl_1_bss_112E, lbl_1_bss_F14[0]);
        if (lbl_1_bss_F3C[0] == 0x118) {
            if (arg0 < 2) {
                var_r29 = 0x7F;
            } else {
                var_r29 = 0x8A;
            }
            lbl_1_bss_8[0] = HuAudJinglePlay(var_r29);
            lbl_1_bss_8[1] = 1;
        }
        break;
    case 3:
        lbl_1_bss_F3C[0] = 0;
        lbl_1_bss_F14[0] = 0.0f;
        lbl_1_bss_1168[2] += 1;
        break;
    case 4:
        lbl_1_bss_F14[0] += 0.05;
        if (lbl_1_bss_F14[0] > 1.0f) {
            lbl_1_bss_F14[0] = 1.0f;
        }
        espScaleSet(lbl_1_bss_112E, 1.0f - (lbl_1_bss_F14[0] * 0.5f), 1.0f - (lbl_1_bss_F14[0] * 0.5f));
        espTPLvlSet(lbl_1_bss_112E, 1.0f - lbl_1_bss_F14[0]);
        if (lbl_1_bss_F14[0] == 1.0f) {
            espKill(lbl_1_bss_112E);
            lbl_1_bss_1168[2] = 0;
            var_r31 = 1;
        }
        break;
    }
    return var_r31;
}

s32 fn_1_182E0(CardGame *arg0)
{
    s32 var_r30;

    var_r30 = 0;
    switch (lbl_1_bss_1168[0]) {
    case 0:
        espDispOn(lbl_1_bss_FA2[arg0->curPlayer]);
        lbl_1_bss_F3C[0] = 0x12C;
        espPosSet(lbl_1_bss_FA2[arg0->curPlayer], 200.0f, 240.0f + (f32) lbl_1_bss_F3C[0]);
        espScaleSet(lbl_1_bss_FA2[arg0->curPlayer], 1.0f, 1.0f);
        espZRotSet(lbl_1_bss_FA2[arg0->curPlayer], 0.0f);
        espTPLvlSet(lbl_1_bss_FA2[arg0->curPlayer], 1.0f);
        lbl_1_bss_1168[0] += 1;
        HuAudFXPlay(0x2FD);
        break;
    case 1:
        lbl_1_bss_F3C[0] -= 0x14;
        if (lbl_1_bss_F3C[0] == 0x14) {
            omVibrate((s16) arg0->order[arg0->curPlayer], 0xF, 0xC, 0);
        }
        if (lbl_1_bss_F3C[0] == 0) {
            lbl_1_bss_1168[0] += 1;
        }
        espPosSet(lbl_1_bss_FA2[arg0->curPlayer], 200.0f, 240.0f + (f32) lbl_1_bss_F3C[0]);
        break;
    case 2:
        if ((HuPadBtnDown[arg0->order[arg0->curPlayer]] & 0x100) != 0) {
            lbl_1_bss_F14[0] = 1.0f;
            lbl_1_bss_F14[1] = 0.0f;
            lbl_1_bss_F14[2] = 1.0f;
            lbl_1_bss_1168[0] += 1;
        }
        break;
    case 3:
        lbl_1_bss_F14[0] -= 0.02;
        if (lbl_1_bss_F14[0] < 0.0f) {
            lbl_1_bss_F14[0] = 0.0f;
        }
        espScaleSet(lbl_1_bss_FA2[arg0->curPlayer], lbl_1_bss_F14[0], lbl_1_bss_F14[0]);
        lbl_1_bss_F14[1] += 10.0f;
        if (lbl_1_bss_F14[1] > 350.0f) {
            lbl_1_bss_F14[1] = 0.0f;
        }
        espZRotSet(lbl_1_bss_FA2[arg0->curPlayer], lbl_1_bss_F14[1]);
        lbl_1_bss_F14[2] -= 0.02;
        if (lbl_1_bss_F14[2] < 0.0f) {
            lbl_1_bss_F14[2] = 0.0f;
            espDispOff(lbl_1_bss_FA2[arg0->curPlayer]);
            lbl_1_bss_1168[0] = 0;
            var_r30 = 1;
        }
        espTPLvlSet(lbl_1_bss_FA2[arg0->curPlayer], lbl_1_bss_F14[2]);
        break;
    }
    return var_r30;
}

void fn_1_2FFAC(CardGame *arg0)
{
    CardPlayer *p[4];
    s16 bank;
    s32 i;
    s32 j;
    s32 k;
    s32 score;

    for (i = 0; i < 4; i++) {
        p[i] = &arg0->pl[arg0->order[i]];
        p[i]->f30 = 1;
    }
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (i != j && p[j]->f40 > p[i]->f40) {
                p[i]->f30 += 1;
            }
        }
    }
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (j == p[i]->f30 - 1) {
                espDispOn(lbl_1_bss_100A[i][j]);
            } else {
                espDispOff(lbl_1_bss_100A[i][j]);
            }
        }
        espBankSet(lbl_1_bss_FAA[i], (s16) (p[i]->f30 - 1));
        for (k = 0; k < 3; k++) {
            if (k == 0) {
                bank = lbl_1_bss_FC2[i];
            } else if (k == 1) {
                bank = lbl_1_bss_FBA[i];
            } else {
                bank = lbl_1_bss_FB2[i];
            }
            if (*(&p[i]->f34 + k) != -1) {
                espBankSet(bank, (s16) *(&p[i]->f34 + k));
                espDispOn(bank);
            } else {
                espDispOff(bank);
            }
        }
        if (p[i]->f40 > 0) {
            espBankSet(lbl_1_bss_FE2[i], 0);
        } else if (p[i]->f40 < 0) {
            espBankSet(lbl_1_bss_FE2[i], 1);
        } else {
            espBankSet(lbl_1_bss_FE2[i], 2);
        }
        if (p[i]->f40 < 10 && p[i]->f40 > -10) {
            espBankSet(lbl_1_bss_FDA[i], 3);
        } else if (p[i]->f40 < 20 && p[i]->f40 > -20) {
            espBankSet(lbl_1_bss_FDA[i], 4);
        } else {
            espBankSet(lbl_1_bss_FDA[i], 5);
        }
        if (p[i]->f40 >= 0) {
            score = p[i]->f40;
        } else {
            score = p[i]->f40 * -1;
        }
        espBankSet(lbl_1_bss_FD2[i], (s16) ((score % 10) + 3));
    }
}

s32 lbl_1_bss_1140[10];

s32 fn_1_11C48(CardGame *arg0)
{
    Vec pos;
    Vec pos2;
    s32 var_r26;
    s32 var_r27;
    s32 i;
    s32 j;
    s32 n;

    var_r26 = 0;
    var_r27 = 0;
    switch (lbl_1_bss_1168[0]) {
    case 0:
        lbl_1_bss_F14[5] -= 6.0f;
        pos.x = 288.0f;
        pos.y = 160.0f;
        pos.z = 1800.0f;
        pos.y += lbl_1_bss_F14[5];
        fn_1_36A70(&pos);
        lbl_1_bss_F14[6] += 15.0f;
        Hu3DModelRotSet(fn_1_36A60(), 0.0f, lbl_1_bss_F14[6], 0.0f);
        if (fn_1_30FA4(0, NULL) != 0) {
            lbl_1_bss_1168[0] += 1;
            fn_1_36B78();
        }
        break;
    case 1:
        n = 0;
        for (i = 0; i < arg0->boardSize; i++) {
            lbl_1_bss_1140[i] = (i + 1) * 0x7D0;
            for (j = 0; j < arg0->boardSize; j++) {
                fn_1_9C74(&pos, j, i, 1, 1);
                Hu3DModelPosSet(lbl_1_bss_478[j][i], -50.0f + pos.x, pos.y + (f32) lbl_1_bss_1140[i], pos.z);
                Hu3DModelAttrReset(lbl_1_bss_478[j][i], 1U);
                if ((lbl_1_bss_8C4[j][i] & 0xF00) != 0) {
                    Hu3DModelAttrSet(lbl_1_bss_478[j][i], 1U);
                    Hu3DModelPosSet(lbl_1_bss_3F4[n], -50.0f + pos.x, pos.y + (f32) lbl_1_bss_1140[i], pos.z);
                    Hu3DModelAttrReset(lbl_1_bss_3F4[n], 1U);
                    n++;
                }
            }
        }
        lbl_1_bss_1168[0] += 1;
        break;
    case 2:
        n = 0;
        for (i = 0; i < arg0->boardSize; i++) {
            if (lbl_1_bss_1140[i] > 0) {
                lbl_1_bss_1140[i] = lbl_1_bss_1140[i] - 0x64;
            } else {
                lbl_1_bss_1140[i] = 0;
            }
            for (j = 0; j < arg0->boardSize; j++) {
                fn_1_9C74(&pos, j, i, 1, 1);
                Hu3DModelPosSet(lbl_1_bss_478[j][i], -50.0f + pos.x, pos.y + (f32) lbl_1_bss_1140[i], pos.z);
                if ((lbl_1_bss_8C4[j][i] & 0xF00) != 0) {
                    Hu3DModelPosSet(lbl_1_bss_3F4[n], -50.0f + pos.x, pos.y + (f32) lbl_1_bss_1140[i], pos.z);
                    n++;
                }
            }
            if (lbl_1_bss_1140[i] == 0) {
                var_r27 += 1;
                if (lbl_1_bss_20[i] == 0) {
                    fn_1_8D04(0x410, lbl_1_bss_478[arg0->boardSize - 1][i], 1);
                    lbl_1_bss_20[i] = 1;
                }
            }
        }
        if (var_r27 == arg0->boardSize) {
            lbl_1_bss_F3C[0] = 0;
            lbl_1_bss_1168[0] += 1;
            for (i = 0; i < arg0->boardSize; i++) {
                for (j = 0; j < arg0->boardSize; j++) {
                    if ((lbl_1_bss_8C4[i][j] & 0xF00) != 0) {
                        fn_1_9C74(&pos, i, j, 1, 1);
                        pos2.x = pos.x + -50.0f;
                        pos2.y = pos.y;
                        pos2.z = pos.z;
                        fn_1_3ED3C(0xF, &pos2);
                    }
                }
                lbl_1_bss_20[i] = 0;
            }
            HuAudFXPlay(0x411);
        }
        break;
    case 3:
        lbl_1_bss_F3C[0] += 1;
        if (lbl_1_bss_F3C[0] >= 0x64) {
            for (i = 0; i < arg0->boardSize; i++) {
                for (j = 0; j < arg0->boardSize; j++) {
                    if ((lbl_1_bss_8C4[i][j] & 0xF00) != 0) {
                        Hu3DModelAttrReset(lbl_1_bss_478[i][j], 1U);
                    }
                }
            }
            for (i = 0; i < lbl_1_bss_3F0; i++) {
                Hu3DModelAttrSet(lbl_1_bss_3F4[i], 1U);
            }
        }
        if (lbl_1_bss_F3C[0] >= 0xC8) {
            for (i = 0; i < lbl_1_bss_3F0; i++) {
                Hu3DModelKill(lbl_1_bss_3F4[i]);
            }
            lbl_1_bss_1168[0] = 0;
            var_r26 = 1;
        }
        break;
    }
    return var_r26;
}

extern s32 fn_1_30FA4(s32 mode, void *arg);

s32 fn_1_1658C(CardGame *arg0)
{
    f32 sz[2];
    f32 sz2[2];
    s32 var_r28;
    s32 var_r29;
    s32 var_r30;

    var_r28 = 0;
    switch (lbl_1_bss_1168[1]) {
    case 0:
        var_r30 = 0;
        sz[0] = 352.0f;
        sz[1] = 352.0f;
        switch (lbl_1_bss_1168[0]) {
        case 0:
            lbl_1_bss_112E = espEntry(lbl_1_data_2B4[0xD], 0, 0);
            espDrawNoSet(lbl_1_bss_112E, 0);
            lbl_1_bss_F3C[0] = 0x12C;
            espPosSet(lbl_1_bss_112E, (288.0f - (sz[1] * 0.5f)) + (sz[1] * 0.5f), 78.0f - (f32) lbl_1_bss_F3C[0]);
            lbl_1_bss_1168[0] += 1;
            HuSprExecLayerSet(0x40, 6);
            break;
        case 1:
            lbl_1_bss_F3C[0] -= 0xA;
            espPosSet(lbl_1_bss_112E, (288.0f - (sz[1] * 0.5f)) + (sz[1] * 0.5f), 78.0f - (f32) lbl_1_bss_F3C[0]);
            if (lbl_1_bss_F3C[0] <= 0) {
                lbl_1_bss_F3C[0] = 0;
                lbl_1_bss_1168[0] += 1;
                var_r30 = 1;
            }
            break;
        case 2:
            lbl_1_bss_F3C[0] += 0xA;
            espPosSet(lbl_1_bss_112E, (288.0f - (sz[1] * 0.5f)) + (sz[1] * 0.5f), 78.0f - (f32) lbl_1_bss_F3C[0]);
            if (lbl_1_bss_F3C[0] >= 0x12C) {
                espKill(lbl_1_bss_112E);
                lbl_1_bss_F3C[0] = 0;
                lbl_1_bss_1168[0] = 0;
                var_r30 = 1;
            }
            break;
        }
        if (var_r30 != 0) {
            lbl_1_bss_1168[1] += 1;
            ((s32 *) lbl_1_bss_12A8)[10] = 0x3C;
            fn_1_4FBC8(arg0, 4);
            ((s32 *) lbl_1_bss_12A8)[14] = 1;
        }
        break;
    case 1:
        if (fn_1_16DBC(arg0) != 0) {
            arg0->curPlayer += 1;
            if (arg0->curPlayer == 4) {
                arg0->curPlayer = 0;
                lbl_1_bss_1168[1] += 1;
            }
            lbl_1_bss_C44 = 1;
        }
        break;
    case 2:
        if (fn_1_30FA4(0, arg0) != 0) {
            lbl_1_bss_1168[1] += 1;
        }
        break;
    case 3:
        var_r29 = 0;
        sz2[0] = 352.0f;
        sz2[1] = 352.0f;
        switch (lbl_1_bss_1168[0]) {
        case 0:
            lbl_1_bss_112E = espEntry(lbl_1_data_2B4[0xD], 0, 0);
            espDrawNoSet(lbl_1_bss_112E, 0);
            lbl_1_bss_F3C[0] = 0x12C;
            espPosSet(lbl_1_bss_112E, (288.0f - (sz2[1] * 0.5f)) + (sz2[1] * 0.5f), 78.0f - (f32) lbl_1_bss_F3C[0]);
            lbl_1_bss_1168[0] += 1;
            HuSprExecLayerSet(0x40, 6);
            break;
        case 1:
            lbl_1_bss_F3C[0] -= 0xA;
            espPosSet(lbl_1_bss_112E, (288.0f - (sz2[1] * 0.5f)) + (sz2[1] * 0.5f), 78.0f - (f32) lbl_1_bss_F3C[0]);
            if (lbl_1_bss_F3C[0] <= 0) {
                lbl_1_bss_F3C[0] = 0;
                lbl_1_bss_1168[0] += 1;
                var_r29 = 1;
            }
            break;
        case 2:
            lbl_1_bss_F3C[0] += 0xA;
            espPosSet(lbl_1_bss_112E, (288.0f - (sz2[1] * 0.5f)) + (sz2[1] * 0.5f), 78.0f - (f32) lbl_1_bss_F3C[0]);
            if (lbl_1_bss_F3C[0] >= 0x12C) {
                espKill(lbl_1_bss_112E);
                lbl_1_bss_F3C[0] = 0;
                lbl_1_bss_1168[0] = 0;
                var_r29 = 1;
            }
            break;
        }
        if (var_r29 != 0) {
            lbl_1_bss_1168[1] += 1;
        }
        break;
    case 4:
        lbl_1_bss_1168[1] = 0;
        var_r28 = 1;
        ((s32 *) lbl_1_bss_12A8)[14] = 0;
        ((s32 *) lbl_1_bss_12A8)[10] = 0x64;
        fn_1_4FBC8(arg0, 5);
        HuAudSStreamFadeOut(lbl_1_bss_14, 0x2EE);
        ((s32 *) lbl_1_bss_12A8)[14] = 1;
        break;
    }
    return var_r28;
}

s16 lbl_1_bss_1080[3];
extern void fn_1_31788(Vec *pos);
extern void fn_1_31710(Vec *pos);

s32 fn_1_16DBC(CardGame *arg0)
{
    Vec pos;
    Vec pos2;
    CardPlayer *rec;
    s32 var_r27;
    s32 var_r28;
    s32 var_r29;

    var_r27 = 0;
    rec = &arg0->pl[arg0->order[arg0->curPlayer]];
    switch (lbl_1_bss_1168[2]) {
    case 0:
        if (fn_1_30FA4(1, arg0) != 0) {
            lbl_1_bss_1168[2] += 1;
            fn_1_17BE8(arg0, 0);
            fn_1_9DA8(&pos, arg0);
            lbl_1_bss_1080[0] = Hu3DModelCreateData(lbl_1_data_B0[0x74]);
            Hu3DModelCameraSet(lbl_1_bss_1080[0], 1U);
            Hu3DModelPosSet(lbl_1_bss_1080[0], -50.0f + pos.x, pos.y, pos.z);
            Hu3DModelLayerSet(lbl_1_bss_1080[0], 4);
            Hu3DModelAttrSet(lbl_1_bss_1080[0], 0x40000001U);
            Hu3DModelAttrSet(lbl_1_bss_1080[0], 1U);
            lbl_1_bss_1080[1] = Hu3DModelCreateData(lbl_1_data_B0[0x75]);
            Hu3DModelCameraSet(lbl_1_bss_1080[1], 1U);
            Hu3DModelPosSet(lbl_1_bss_1080[1], -50.0f + pos.x, pos.y, pos.z);
            Hu3DModelLayerSet(lbl_1_bss_1080[1], 4);
            Hu3DModelAttrSet(lbl_1_bss_1080[1], 0x40000001U);
            Hu3DModelAttrSet(lbl_1_bss_1080[1], 1U);
            lbl_1_bss_F14[0] = 0.0f;
            omVibrate((s16) arg0->order[arg0->curPlayer], 0xF, 0xC, 0);
        }
        break;
    case 1:
        fn_1_17BE8(arg0, 0);
        fn_1_9DA8(&pos, arg0);
        Hu3DModelPosSet(lbl_1_bss_470[arg0->order[arg0->curPlayer]], -50.0f + pos.x, pos.y, pos.z);
        Hu3DModelPosSet(lbl_1_bss_468[arg0->order[arg0->curPlayer]], -50.0f + pos.x, pos.y, pos.z);
        lbl_1_bss_F14[0] += 0.05f;
        if (lbl_1_bss_F14[0] > 1.0f) {
            lbl_1_bss_F14[0] = 1.0f;
            lbl_1_bss_1168[2] += 1;
        }
        Hu3DModelScaleSet(lbl_1_bss_470[arg0->order[arg0->curPlayer]], 1.0f, lbl_1_bss_F14[0], lbl_1_bss_F14[0]);
        break;
    case 2:
        if (lbl_1_bss_C44 != 0) {
            ((s32 *) lbl_1_bss_12A8)[10] = 0x3C;
            ((s32 *) lbl_1_bss_12A8)[2] = GwPlayerConf[arg0->order[arg0->curPlayer]].charNo;
            fn_1_4FBC8(arg0, 0x20);
            lbl_1_bss_C44 = 0;
            if (GwPlayerConf[arg0->order[arg0->curPlayer]].type == 0) {
                fn_1_502E0(0x13);
            }
        }
        fn_1_18054(arg0->order[arg0->curPlayer], rec->f00);
        if ((HuPadDStkRep[arg0->order[arg0->curPlayer]] & 8) != 0) {
            if (rec->f00 == 3 || rec->f00 == 4) {
                if (rec->row == 0) {
                    rec->f00 = 1;
                } else {
                    rec->row -= 1;
                }
                HuAudFXPlay(0);
            }
            fn_1_17BE8(arg0, 1);
        } else if ((HuPadDStkRep[arg0->order[arg0->curPlayer]] & 4) != 0) {
            if (rec->f00 == 3 || rec->f00 == 4) {
                if (rec->row == arg0->boardSize - 1) {
                    rec->f00 = 2;
                } else {
                    rec->row += 1;
                }
                HuAudFXPlay(0);
            }
            fn_1_17BE8(arg0, 2);
        } else if ((HuPadDStkRep[arg0->order[arg0->curPlayer]] & 1) != 0) {
            if (rec->f00 == 1 || rec->f00 == 2) {
                if (rec->col == 0) {
                    rec->f00 = 3;
                } else {
                    rec->col -= 1;
                }
                HuAudFXPlay(0);
            }
            fn_1_17BE8(arg0, 3);
        } else if ((HuPadDStkRep[arg0->order[arg0->curPlayer]] & 2) != 0) {
            if (rec->f00 == 1 || rec->f00 == 2) {
                if (rec->col == arg0->boardSize - 1) {
                    rec->f00 = 4;
                } else {
                    rec->col += 1;
                }
                HuAudFXPlay(0);
            }
            fn_1_17BE8(arg0, 4);
        }
        fn_1_9DA8(&pos, arg0);
        Hu3DModelPosSet(lbl_1_bss_470[arg0->order[arg0->curPlayer]], -50.0f + pos.x, pos.y, pos.z);
        Hu3DModelPosSet(lbl_1_bss_468[arg0->order[arg0->curPlayer]], -50.0f + pos.x, pos.y, pos.z);
        if (rec->f00 == 1 || rec->f00 == 2) {
            Hu3DModelAttrReset(lbl_1_bss_1080[0], 1U);
            Hu3DModelAttrSet(lbl_1_bss_1080[1], 1U);
        } else {
            Hu3DModelAttrReset(lbl_1_bss_1080[1], 1U);
            Hu3DModelAttrSet(lbl_1_bss_1080[0], 1U);
        }
        Hu3DModelPosSet(lbl_1_bss_1080[0], -50.0f + pos.x, pos.y, pos.z);
        Hu3DModelPosSet(lbl_1_bss_1080[1], -50.0f + pos.x, pos.y, pos.z);
        if ((HuPadBtnDown[arg0->order[arg0->curPlayer]] & 0x100) != 0) {
            fn_1_50460();
            Hu3DModelKill(lbl_1_bss_1080[0]);
            Hu3DModelKill(lbl_1_bss_1080[1]);
            rec->f04 = rec->f00;
            rec->f08 = rec->row;
            rec->f0C = rec->col;
            var_r29 = rec->row;
            var_r28 = rec->col;
            switch (rec->f00) {
            case 1:
                var_r29 = rec->row - 1;
                var_r28 = rec->col;
                break;
            case 2:
                var_r29 = rec->row + 1;
                var_r28 = rec->col;
                break;
            case 3:
                var_r29 = rec->row;
                var_r28 = rec->col - 1;
                break;
            case 4:
                var_r29 = rec->row;
                var_r28 = rec->col + 1;
                break;
            }
            fn_1_9C74(&pos, var_r29, var_r28, 1, 1);
            pos.x += -50.0f;
            pos2.x = pos.x;
            pos2.y = pos.y;
            pos2.z = pos.z;
            fn_1_3ED3C(0xF, &pos2);
            Hu3DModelScaleSet(lbl_1_bss_468[arg0->order[arg0->curPlayer]], 0.0f, 0.0f, 0.0f);
            lbl_1_bss_F3C[0] = 0;
            lbl_1_bss_F14[0] = 0.0f;
            lbl_1_bss_1168[2] += 1;
            switch (rec->f00) {
            case 1:
                lbl_1_bss_8C4[rec->row][rec->col] |= 0x10000;
                break;
            case 2:
                lbl_1_bss_8C4[rec->row][rec->col] |= 0x20000;
                break;
            case 3:
                lbl_1_bss_8C4[rec->row][rec->col] |= 0x40000;
                break;
            case 4:
                lbl_1_bss_8C4[rec->row][rec->col] |= 0x80000;
                break;
            }
            fn_1_8D04(0x3F8, lbl_1_bss_468[arg0->order[arg0->curPlayer]], 1);
            HuAudFXPlay(2);
            lbl_1_bss_0[arg0->order[arg0->curPlayer]] = Hu3DLLightCreate(lbl_1_bss_468[arg0->order[arg0->curPlayer]],
                -50.0f + pos.x, 80.0f + pos.y, 100.0f + pos.z, 0.0f, 0.0f, -1.0f, 0x60U, 0x60U, 0x60U);
            Hu3DLLightStaticSet(lbl_1_bss_468[arg0->order[arg0->curPlayer]], lbl_1_bss_0[arg0->order[arg0->curPlayer]], 1);
            Hu3DLLightInfinitytSet(lbl_1_bss_468[arg0->order[arg0->curPlayer]], lbl_1_bss_0[arg0->order[arg0->curPlayer]]);
        }
        fn_1_9C74(&pos, rec->row, rec->col, 1, 1);
        pos.x += 150.0f;
        pos.y += 100.0f;
        fn_1_31788(&pos);
        pos.y += 1500.0f;
        pos.z += 3000.0f;
        fn_1_31710(&pos);
        break;
    case 3:
        if (lbl_1_bss_F3C[0] == 0x62) {
            fn_1_8D04(0x412, lbl_1_bss_468[arg0->order[arg0->curPlayer]], 1);
        }
        lbl_1_bss_F3C[0] += 1;
        if (lbl_1_bss_F3C[0] >= 0x64) {
            lbl_1_bss_F14[0] += 0.05f;
            if (lbl_1_bss_F14[0] > 1.0f) {
                lbl_1_bss_F14[0] = 1.0f;
            }
            Hu3DModelAttrReset(lbl_1_bss_468[arg0->order[arg0->curPlayer]], 1U);
            Hu3DModelScaleSet(lbl_1_bss_468[arg0->order[arg0->curPlayer]], lbl_1_bss_F14[0], lbl_1_bss_F14[0], lbl_1_bss_F14[0]);
        }
        if (lbl_1_bss_F3C[0] >= 0xC8) {
            lbl_1_bss_F3C[0] = 0;
            lbl_1_bss_1168[2] = 0;
            var_r27 = 1;
        }
        break;
    }
    return var_r27;
}

s32 lbl_1_bss_F64[8];
Vec lbl_1_bss_E9C[10];
s16 lbl_1_bss_1116[4];
s16 lbl_1_bss_111E[4];
s16 lbl_1_bss_1126[4];
extern void fn_1_36B10(s32 arg0);

s32 fn_1_15D38(CardGame *arg0)
{
    Vec pos;
    Vec pos2;
    Vec scale;
    s16 temp_r29;
    s32 var_r30;
    s32 idx;

    var_r30 = -1;
    if (lbl_1_bss_C44 != 0) {
        fn_1_36B10(1);
        ((s32 *) lbl_1_bss_12A8)[14] = 1;
        ((s32 *) lbl_1_bss_12A8)[2] = GwPlayerConf[arg0->order[lbl_1_bss_F3C[2]]].charNo;
        fn_1_4FBC8(arg0, lbl_1_bss_F3C[2] + 0x21);
        lbl_1_bss_C44 = 0;
        fn_1_36B10(0);
    }
    lbl_1_bss_F3C[3] += 1;
    if (lbl_1_bss_F3C[3] < 0x1E) {
        return -1;
    }
    if (lbl_1_bss_F14[2] == 0.0f) {
        pos.x = -500.0f + (300.0f * (f32) lbl_1_bss_F64[lbl_1_bss_F3C[2] + 4]);
        pos.y = 170.0f;
        pos.z = 300.0f;
        pos2 = pos;
        temp_r29 = fn_1_3ED3C(1, &pos2);
        lbl_1_bss_E9C[0].x = 3.0f;
        lbl_1_bss_E9C[0].y = 3.0f;
        lbl_1_bss_E9C[0].z = 3.0f;
        scale = lbl_1_bss_E9C[0];
        fn_1_3F6B0(1, temp_r29, &scale);
        lbl_1_bss_F14[6] = 360.0f;
        omVibrate((s16) arg0->order[lbl_1_bss_F3C[2]], 0xF, 0xC, 0);
        fn_1_8D04(0x40F, lbl_1_bss_1126[lbl_1_bss_F64[lbl_1_bss_F3C[2] + 4]], 1);
    }
    lbl_1_bss_F14[1] += 20.0f;
    lbl_1_bss_F14[2] += 0.02f;
    if (lbl_1_bss_F14[2] > 1.0f) {
        lbl_1_bss_F14[2] = 1.0f;
    }
    idx = lbl_1_bss_F64[lbl_1_bss_F3C[2] + 4];
    if (idx != 0 && idx != 1 && idx != 2) {
        idx = 3;
    }
    Hu3DModelRotSet(lbl_1_bss_1126[idx], 0.0f, lbl_1_bss_F14[1], 0.0f);
    Hu3DModelScaleSet(lbl_1_bss_1126[idx], 1.0f - lbl_1_bss_F14[2], 1.0f, 1.0f - lbl_1_bss_F14[2]);
    if (lbl_1_bss_F14[2] >= 0.2f) {
        Hu3DModelAttrSet(lbl_1_bss_1116[arg0->order[lbl_1_bss_F3C[2]]], 1U);
    }
    if (lbl_1_bss_F14[2] > 0.2f) {
        lbl_1_bss_F14[6] -= 18.0f;
        if (lbl_1_bss_F14[6] < 0.0f) {
            lbl_1_bss_F14[6] = 0.0f;
        }
        Hu3DModelRotSet(fn_1_36A60(), 0.0f, -lbl_1_bss_F14[6], 0.0f);
    }
    if (lbl_1_bss_F14[2] == 1.0f) {
        Hu3DModelAttrSet(lbl_1_bss_1126[lbl_1_bss_F64[lbl_1_bss_F3C[2] + 4]], 1U);
        var_r30 = lbl_1_bss_F3C[2];
        lbl_1_bss_F14[0] = 0.0f;
        lbl_1_bss_F14[1] = 0.0f;
        lbl_1_bss_F14[2] = 0.0f;
        lbl_1_bss_F3C[2] += 1;
        lbl_1_bss_F3C[3] = 0;
        arg0->curPlayer += 1;
        if (lbl_1_bss_F3C[2] == 4) {
            lbl_1_bss_F3C[2] = 0;
            lbl_1_bss_1168[1] = 0;
            arg0->curPlayer = 0;
            ((s32 *) lbl_1_bss_12A8)[14] = 0;
            fn_1_36B10(0);
            lbl_1_bss_F14[5] = 0.0f;
            lbl_1_bss_F14[6] = 0.0f;
        }
        lbl_1_bss_C44 = 1;
    }
    return var_r30;
}
s32 fn_1_21028(CardGame *arg0) {
    s32 temp_r27;
    s32 temp_r28;
    s32 var_r26;
    s32 var_r29;
    s32 var_r30;
    s32 var_r31;

    var_r29 = 0;
    var_r31 = arg0->pl[arg0->order[arg0->curPlayer]].row;
    var_r30 = arg0->pl[arg0->order[arg0->curPlayer]].col;
    temp_r28 = arg0->pl[arg0->order[arg0->curPlayer]].f20;
    temp_r27 = arg0->pl[arg0->order[arg0->curPlayer]].f24;
    if ((temp_r28 == 1) && (temp_r27 == 1)) {
        if (var_r31 < 2) {
            if ((lbl_1_bss_8C4[var_r31 + 1][var_r30] & 0xF00) == 0) {
                if ((lbl_1_bss_8C4[var_r31 + 2][var_r30] & 0xF00) != 0) {
                    var_r29 = 2;
                } else {
                    if ((var_r30 > 0) && ((lbl_1_bss_8C4[var_r31 + 1][var_r30 - 1] & 0xF00) != 0)) {
                        var_r29 = 2;
                    }
                    if ((var_r30 < (arg0->boardSize - 1)) && ((lbl_1_bss_8C4[var_r31 + 1][var_r30 + 1] & 0xF00) != 0)) {
                        var_r29 = 2;
                    }
                }
            }
        } else if ((lbl_1_bss_8C4[var_r31 - 1][var_r30] & 0xF00) == 0) {
            if ((lbl_1_bss_8C4[var_r31 - 2][var_r30] & 0xF00) != 0) {
                var_r29 = 1;
            } else {
                if ((var_r30 > 0) && ((lbl_1_bss_8C4[var_r31 - 1][var_r30 - 1] & 0xF00) != 0)) {
                    var_r29 = 1;
                }
                if ((var_r30 < (arg0->boardSize - 1)) && ((lbl_1_bss_8C4[var_r31 - 1][var_r30 + 1] & 0xF00) != 0)) {
                    var_r29 = 1;
                }
            }
        }
        if (var_r31 > (arg0->boardSize - 3)) {
            if ((lbl_1_bss_8C4[var_r31 - 1][var_r30] & 0xF00) == 0) {
                if ((lbl_1_bss_8C4[var_r31 - 2][var_r30] & 0xF00) != 0) {
                    var_r29 = 1;
                } else {
                    if ((var_r30 > 0) && ((lbl_1_bss_8C4[var_r31 - 1][var_r30 - 1] & 0xF00) != 0)) {
                        var_r29 = 1;
                    }
                    if ((var_r30 < (arg0->boardSize - 1)) && ((lbl_1_bss_8C4[var_r31 - 1][var_r30 + 1] & 0xF00) != 0)) {
                        var_r29 = 1;
                    }
                }
            }
        } else if ((lbl_1_bss_8C4[var_r31 + 1][var_r30] & 0xF00) == 0) {
            if ((lbl_1_bss_8C4[var_r31 + 2][var_r30] & 0xF00) != 0) {
                var_r29 = 2;
            } else {
                if ((var_r30 > 0) && ((lbl_1_bss_8C4[var_r31 + 1][var_r30 - 1] & 0xF00) != 0)) {
                    var_r29 = 2;
                }
                if ((var_r30 < (arg0->boardSize - 1)) && ((lbl_1_bss_8C4[var_r31 + 1][var_r30 + 1] & 0xF00) != 0)) {
                    var_r29 = 2;
                }
            }
        }
        if (var_r30 < 2) {
            if ((lbl_1_bss_8C4[var_r31][var_r30 + 1] & 0xF00) == 0) {
                if ((lbl_1_bss_8C4[var_r31][var_r30 + 2] & 0xF00) != 0) {
                    var_r29 = 4;
                } else {
                    if ((var_r31 > 0) && ((lbl_1_bss_8C4[var_r31 - 1][var_r30 + 1] & 0xF00) != 0)) {
                        var_r29 = 4;
                    }
                    if ((var_r31 < (arg0->boardSize - 1)) && ((lbl_1_bss_8C4[var_r31 + 1][var_r30 + 1] & 0xF00) != 0)) {
                        var_r29 = 4;
                    }
                }
            }
        } else if ((lbl_1_bss_8C4[var_r31][var_r30 - 1] & 0xF00) == 0) {
            if ((lbl_1_bss_8C4[var_r31][var_r30 - 2] & 0xF00) != 0) {
                var_r29 = 3;
            } else {
                if ((var_r31 > 0) && ((lbl_1_bss_8C4[var_r31 - 1][var_r30 - 1] & 0xF00) != 0)) {
                    var_r29 = 3;
                }
                if ((var_r31 < (arg0->boardSize - 1)) && ((lbl_1_bss_8C4[var_r31 + 1][var_r30 - 1] & 0xF00) != 0)) {
                    var_r29 = 3;
                }
            }
        }
        if (var_r30 > (arg0->boardSize - 3)) {
            if ((lbl_1_bss_8C4[var_r31][var_r30 - 1] & 0xF00) == 0) {
                if ((lbl_1_bss_8C4[var_r31][var_r30 - 2] & 0xF00) != 0) {
                    var_r29 = 3;
                } else {
                    if ((var_r31 > 0) && ((lbl_1_bss_8C4[var_r31 - 1][var_r30 - 1] & 0xF00) != 0)) {
                        var_r29 = 3;
                    }
                    if ((var_r31 < (arg0->boardSize - 1)) && ((lbl_1_bss_8C4[var_r31 + 1][var_r30 - 1] & 0xF00) != 0)) {
                        var_r29 = 3;
                    }
                }
            }
        } else if ((lbl_1_bss_8C4[var_r31][var_r30 + 1] & 0xF00) == 0) {
            if ((lbl_1_bss_8C4[var_r31][var_r30 + 2] & 0xF00) != 0) {
                var_r29 = 4;
            } else {
                if ((var_r31 > 0) && ((lbl_1_bss_8C4[var_r31 - 1][var_r30 + 1] & 0xF00) != 0)) {
                    var_r29 = 4;
                }
                if ((var_r31 < (arg0->boardSize - 1)) && ((lbl_1_bss_8C4[var_r31 + 1][var_r30 + 1] & 0xF00) != 0)) {
                    var_r29 = 4;
                }
            }
        }
    } else if ((temp_r28 == 0) && (temp_r27 == 1)) {
        if ((var_r31 >= 2) && ((lbl_1_bss_8C4[var_r31 - 1][var_r30] & 0xF00) == 0)) {
            if ((lbl_1_bss_8C4[var_r31 - 2][var_r30] & 0xF00) != 0) {
                var_r29 = 1;
            } else {
                if ((var_r30 > 0) && ((lbl_1_bss_8C4[var_r31 - 1][var_r30 - 1] & 0xF00) != 0)) {
                    var_r29 = 1;
                }
                if ((var_r30 < (arg0->boardSize - 1)) && ((lbl_1_bss_8C4[var_r31 - 1][var_r30 + 1] & 0xF00) != 0)) {
                    var_r29 = 1;
                }
            }
        }
    } else if ((temp_r28 == 2) && (temp_r27 == 1)) {
        if ((var_r31 <= (arg0->boardSize - 3)) && ((lbl_1_bss_8C4[var_r31 + 1][var_r30] & 0xF00) == 0)) {
            if ((lbl_1_bss_8C4[var_r31 + 2][var_r30] & 0xF00) != 0) {
                var_r29 = 2;
            } else {
                if ((var_r30 > 0) && ((lbl_1_bss_8C4[var_r31 + 1][var_r30 - 1] & 0xF00) != 0)) {
                    var_r29 = 2;
                }
                if ((var_r30 < (arg0->boardSize - 1)) && ((lbl_1_bss_8C4[var_r31 + 1][var_r30 + 1] & 0xF00) != 0)) {
                    var_r29 = 2;
                }
            }
        }
    } else if ((temp_r28 == 1) && (temp_r27 == 0)) {
        if ((var_r30 >= 2) && ((lbl_1_bss_8C4[var_r31][var_r30 - 1] & 0xF00) == 0)) {
            if ((lbl_1_bss_8C4[var_r31][var_r30 - 2] & 0xF00) != 0) {
                var_r29 = 3;
            } else {
                if ((var_r31 > 0) && ((lbl_1_bss_8C4[var_r31 - 1][var_r30 - 1] & 0xF00) != 0)) {
                    var_r29 = 3;
                }
                if ((var_r31 < (arg0->boardSize - 1)) && ((lbl_1_bss_8C4[var_r31 + 1][var_r30 - 1] & 0xF00) != 0)) {
                    var_r29 = 3;
                }
            }
        }
    } else if ((temp_r28 == 1) && (temp_r27 == 2) && (var_r30 <= (arg0->boardSize - 3)) && ((lbl_1_bss_8C4[var_r31][var_r30 + 1] & 0xF00) == 0)) {
        if ((lbl_1_bss_8C4[var_r31][var_r30 + 2] & 0xF00) != 0) {
            var_r29 = 4;
        } else {
            if ((var_r31 > 0) && ((lbl_1_bss_8C4[var_r31 - 1][var_r30 + 1] & 0xF00) != 0)) {
                var_r29 = 4;
            }
            if ((var_r31 < (arg0->boardSize - 1)) && ((lbl_1_bss_8C4[var_r31 + 1][var_r30 + 1] & 0xF00) != 0)) {
                var_r29 = 4;
            }
        }
    }
    switch (var_r29) {
    case 1:
        var_r31 -= 1;
        break;
    case 2:
        var_r31 += 1;
        break;
    case 3:
        var_r30 -= 1;
        break;
    case 4:
        var_r30 += 1;
        break;
    }
    if (var_r29 != 0) {
        for (var_r26 = 0; var_r26 < 4; var_r26++) {
            if (var_r31 == arg0->pl[var_r26].row && var_r30 == arg0->pl[var_r26].col) {
                var_r29 = 0;
                break;
            }
        }
    }
    return var_r29;
}

void fn_1_28CA8(CardGame *arg0, s32 arg1, s32 arg2)
{
    Vec pos;
    s32 var_r29;
    s32 flags;

    if ((lbl_1_bss_8C4[arg1][arg2] & 0x800) != 0) {
        lbl_1_bss_8C4[arg1][arg2] &= 0xFFFFF7FF;
        Hu3DModelShadowMapObjReset(lbl_1_bss_478[arg1][arg2]);
        Hu3DModelKill(lbl_1_bss_478[arg1][arg2]);
        flags = lbl_1_bss_8C4[arg1][arg2] & 0xF;
        if (flags == 0xF) {
            var_r29 = 0xD;
        } else if (flags == 0xC) {
            var_r29 = 0xE;
        } else if (flags == 3) {
            var_r29 = 0xF;
        } else if (flags == 0xB) {
            var_r29 = 0x10;
        } else if (flags == 0xD) {
            var_r29 = 0x11;
        } else if (flags == 7) {
            var_r29 = 0x12;
        } else if (flags == 0xE) {
            var_r29 = 0x13;
        } else if (flags == 0xA) {
            var_r29 = 0x14;
        } else if (flags == 9) {
            var_r29 = 0x15;
        } else if (flags == 5) {
            var_r29 = 0x16;
        } else {
            var_r29 = 0x17;
        }
        lbl_1_bss_478[arg1][arg2] = Hu3DModelCreateData(lbl_1_data_B0[var_r29]);
        Hu3DModelCameraSet(lbl_1_bss_478[arg1][arg2], 1U);
        fn_1_9C74(&pos, arg1, arg2, 1, 1);
        Hu3DModelPosSet(lbl_1_bss_478[arg1][arg2], -50.0f + pos.x, pos.y, pos.z);
        Hu3DModelRotSet(lbl_1_bss_478[arg1][arg2], 0.0f, 0.0f, 180.0f);
        Hu3DModelLayerSet(lbl_1_bss_478[arg1][arg2], 2);
        Hu3DModelShadowMapSet(lbl_1_bss_478[arg1][arg2]);
        arg0->f1D4 = 0;
    }
}

Vec lbl_1_bss_C04;
Vec lbl_1_bss_C24;
Vec lbl_1_bss_C30;
extern void fn_1_317F0(Vec *curPos, Vec *dstPos, Vec *curTgt, Vec *dstTgt);
extern s32 fn_1_319D4(f32 frames);

s32 fn_1_1AB2C(CardGame *arg0)
{
    Vec dst;
    Vec move;
    Vec newPos;
    f32 ang;
    f32 var_f31;
    s32 var_r30;

    var_r30 = 0;
    var_f31 = 1.0f;
    switch (lbl_1_bss_1168[2]) {
    case 0:
        lbl_1_bss_C30.x = Hu3DCamera->pos.x;
        lbl_1_bss_C30.y = Hu3DCamera->pos.y;
        lbl_1_bss_C30.z = Hu3DCamera->pos.z;
        lbl_1_bss_C24.x = Hu3DCamera->target.x;
        lbl_1_bss_C24.y = Hu3DCamera->target.y;
        lbl_1_bss_C24.z = Hu3DCamera->target.z;
        dst.x = Hu3DCamera->target.x;
        dst.y = Hu3DCamera->target.y;
        dst.z = Hu3DCamera->target.z;
        dst.y = 3500.0f;
        dst.z = dst.z + 2000.0f;
        lbl_1_bss_C04.x = Hu3DCamera->target.x;
        lbl_1_bss_C04.y = Hu3DCamera->target.y;
        lbl_1_bss_C04.z = Hu3DCamera->target.z;
        fn_1_317F0(&Hu3DCamera->pos, &dst, &Hu3DCamera->target, &Hu3DCamera->target);
        lbl_1_bss_1168[2] += 1;
        break;
    case 1:
        if (fn_1_319D4(30.0f) != 0) {
            fn_1_502E0(0x14);
            lbl_1_bss_1168[2] += 1;
        }
        break;
    case 2:
        if ((HuPadBtn[arg0->order[arg0->curPlayer]] & 0x100) != 0) {
            var_f31 = 2.0f;
        }
        move.x = (f32) HuPadStkX[arg0->order[arg0->curPlayer]];
        move.y = 0.0f;
        move.z = (f32) HuPadStkY[arg0->order[arg0->curPlayer]];
        if (move.x != 0.0f || move.z != 0.0f) {
            ang = (f32) (180.0 * (atan2((f64) move.x, -move.z) / 3.141592653589793));
            move.x = (f32) ((f64) (20.0f * var_f31) * sin((3.141592653589793 * (f64) ang) / 180.0));
            move.z = (f32) ((f64) (20.0f * var_f31) * cos((3.141592653589793 * (f64) ang) / 180.0));
        }
        PSVECAdd(&move, &lbl_1_bss_C04, &lbl_1_bss_C04);
        if (lbl_1_bss_C04.x < (-150.0f * (f32) (arg0->boardSize + 1))) {
            lbl_1_bss_C04.x = -150.0f * (f32) (arg0->boardSize + 1);
        }
        if (lbl_1_bss_C04.x > (150.0f * (f32) (arg0->boardSize + 1))) {
            lbl_1_bss_C04.x = 150.0f * (f32) (arg0->boardSize + 1);
        }
        if (lbl_1_bss_C04.z < (-150.0f * (f32) (arg0->boardSize + 1))) {
            lbl_1_bss_C04.z = -150.0f * (f32) (arg0->boardSize + 1);
        }
        if (lbl_1_bss_C04.z > (150.0f * (f32) (arg0->boardSize + 1))) {
            lbl_1_bss_C04.z = 150.0f * (f32) (arg0->boardSize + 1);
        }
        PSVECSubtract(&lbl_1_bss_C04, &Hu3DCamera->target, &move);
        PSVECScale(&move, &move, 0.12f);
        PSVECAdd(&move, &Hu3DCamera->target, &Hu3DCamera->target);
        newPos.x = 0.0f;
        newPos.y = 3500.0f;
        newPos.z = 0.0f;
        newPos.x = Hu3DCamera->target.x;
        newPos.z = 2000.0f + Hu3DCamera->target.z;
        Hu3DCamera->pos.x = newPos.x;
        Hu3DCamera->pos.y = newPos.y;
        Hu3DCamera->pos.z = newPos.z;
        if ((HuPadBtnDown[arg0->order[arg0->curPlayer]] & 0xA00) != 0) {
            fn_1_50460();
            Hu3DCamera->pos.x = lbl_1_bss_C30.x;
            Hu3DCamera->pos.y = lbl_1_bss_C30.y;
            Hu3DCamera->pos.z = lbl_1_bss_C30.z;
            Hu3DCamera->target.x = lbl_1_bss_C24.x;
            Hu3DCamera->target.y = lbl_1_bss_C24.y;
            Hu3DCamera->target.z = lbl_1_bss_C24.z;
            fn_1_317F0(&Hu3DCamera->pos, &Hu3DCamera->pos, &Hu3DCamera->target, &Hu3DCamera->target);
            lbl_1_bss_1168[2] = 0;
            var_r30 = 1;
            HuAudFXPlay(0x3FC);
            HuAudSStreamVolSet(0, 0x7FU, 0x64U);
        }
        break;
    }
    return var_r30;
}
s32 lbl_1_bss_C10;
s16 lbl_1_bss_C14[4];
s16 lbl_1_bss_C1C[4];

s32 fn_1_198F4(CardGame *arg0)
{
    Vec sp48;
    Vec tgt;
    Vec up;
    Vec campos;
    Vec up2;
    Vec p3d;
    GXColor col;
    s32 var_r28;
    s32 var_r29;
    s32 var_r31;
    s32 var_r31_2;
    s32 var_r31_3;
    s32 var_r31_4;
    s32 var_r31_5;
    s32 var_r31_6;

    var_r28 = 0;
    switch (lbl_1_bss_1168[2]) {            /* irregular */
    case 0:
        lbl_1_bss_C10 = 0;
        for (var_r31 = 0; var_r31 < 4; var_r31++) {
            lbl_1_bss_C1C[var_r31] = espEntry(lbl_1_data_2B4[0], 0, 0);
            espDrawNoSet(lbl_1_bss_C1C[var_r31], 0x40);
            espBankSet(lbl_1_bss_C1C[var_r31], GwPlayerConf[arg0->order[var_r31]].charNo);
            espPosSet(lbl_1_bss_C1C[var_r31], lbl_1_data_618[var_r31][0], lbl_1_data_618[var_r31][1]);
            espPriSet(lbl_1_bss_C1C[var_r31], 0x1C0);
            espDispOff(lbl_1_bss_C1C[var_r31]);
            lbl_1_bss_C14[var_r31] = espEntry(0x1100DE, 0, 0);
            espDrawNoSet(lbl_1_bss_C14[var_r31], 0x40);
            espPosSet(lbl_1_bss_C14[var_r31], lbl_1_data_618[var_r31][0], lbl_1_data_618[var_r31][1]);
            espPriSet(lbl_1_bss_C14[var_r31], 0x1D0);
            espDispOff(lbl_1_bss_C14[var_r31]);
        }
        WipeCreate(2, 0x81, 2);
        lbl_1_bss_1168[2] += 1;
        break;
    case 1:
        if (WipeCheckEnd() == 0) {
            for (var_r31_2 = 0; var_r31_2 < 4; var_r31_2++) {
                espPosSet(lbl_1_bss_1002[var_r31_2], 288.0f + lbl_1_data_448[var_r31_2][0], lbl_1_data_448[var_r31_2][1]);
                espPosSet(lbl_1_bss_100A[var_r31_2][0], 288.0f + lbl_1_data_428[var_r31_2][0], lbl_1_data_428[var_r31_2][1]);
                espPosSet(lbl_1_bss_100A[var_r31_2][1], 288.0f + lbl_1_data_428[var_r31_2][0], lbl_1_data_428[var_r31_2][1]);
                espPosSet(lbl_1_bss_100A[var_r31_2][2], 288.0f + lbl_1_data_428[var_r31_2][0], lbl_1_data_428[var_r31_2][1]);
                espPosSet(lbl_1_bss_100A[var_r31_2][3], 288.0f + lbl_1_data_428[var_r31_2][0], lbl_1_data_428[var_r31_2][1]);
                espPosSet(lbl_1_bss_FEA[var_r31_2], 288.0f + lbl_1_data_4A8[var_r31_2][0], lbl_1_data_4A8[var_r31_2][1]);
                espPosSet(lbl_1_bss_FE2[var_r31_2], 288.0f + lbl_1_data_4C8[var_r31_2][0], lbl_1_data_4C8[var_r31_2][1]);
                espPosSet(lbl_1_bss_FDA[var_r31_2], 288.0f + lbl_1_data_4E8[var_r31_2][0], lbl_1_data_4E8[var_r31_2][1]);
                espPosSet(lbl_1_bss_FD2[var_r31_2], 288.0f + lbl_1_data_508[var_r31_2][0], lbl_1_data_508[var_r31_2][1]);
                espPosSet(lbl_1_bss_FCA[var_r31_2], 288.0f + lbl_1_data_528[var_r31_2][0], lbl_1_data_528[var_r31_2][1]);
                espPosSet(lbl_1_bss_FC2[var_r31_2], 288.0f + lbl_1_data_548[var_r31_2][0], lbl_1_data_548[var_r31_2][1]);
                espPosSet(lbl_1_bss_FBA[var_r31_2], 288.0f + lbl_1_data_568[var_r31_2][0], lbl_1_data_568[var_r31_2][1]);
                espPosSet(lbl_1_bss_FB2[var_r31_2], 288.0f + lbl_1_data_588[var_r31_2][0], lbl_1_data_588[var_r31_2][1]);
                espPosSet(lbl_1_bss_FAA[var_r31_2], 288.0f + lbl_1_data_5A8[var_r31_2][0], lbl_1_data_5A8[var_r31_2][1]);
                espPosSet(lbl_1_bss_FFA[var_r31_2], 288.0f + lbl_1_data_468[var_r31_2][0], lbl_1_data_468[var_r31_2][1]);
                espPosSet(lbl_1_bss_FF2[var_r31_2], 288.0f + lbl_1_data_488[var_r31_2][0], lbl_1_data_468[var_r31_2][1]);
            }
            WipeCreate(1, 0x81, 0xF);
            lbl_1_bss_C30.x = Hu3DCamera->pos.x;
            lbl_1_bss_C30.y = Hu3DCamera->pos.y;
            lbl_1_bss_C30.z = Hu3DCamera->pos.z;
            lbl_1_bss_C24.x = Hu3DCamera->target.x;
            lbl_1_bss_C24.y = Hu3DCamera->target.y;
            lbl_1_bss_C24.z = Hu3DCamera->target.z;
            tgt.x = 0.0f;
            tgt.y = 0.0f;
            tgt.z = 0.0f;
            tgt.z = 300.0f + (30.0f * (f32) (arg0->boardSize - 6));
            Hu3DCamera->target.x = tgt.x;
            Hu3DCamera->target.y = tgt.y;
            Hu3DCamera->target.z = tgt.z;
            up.x = 0.0f;
            up.y = 0.0f;
            up.z = -1.0f;
            Hu3DCamera->up.x = up.x;
            Hu3DCamera->up.y = up.y;
            Hu3DCamera->up.z = up.z;
            campos.x = 0.0f;
            campos.y = 0.0f;
            campos.z = 0.0f;
            campos.y = 11000.0f + (1200.0f * (f32) (arg0->boardSize - 6));
            campos.z = 300.0f + (30.0f * (f32) (arg0->boardSize - 6));
            Hu3DCamera->pos.x = campos.x;
            Hu3DCamera->pos.y = campos.y;
            Hu3DCamera->pos.z = campos.z;
            lbl_1_bss_1168[2] += 1;
        }
        break;
    case 2:
        if (WipeCheckEnd() == 0) {
            for (var_r31_3 = 0; var_r31_3 < 4; var_r31_3++) {
                p3d.x = Hu3DData[lbl_1_bss_468[arg0->order[var_r31_3]]].pos.x;
                p3d.y = Hu3DData[lbl_1_bss_468[arg0->order[var_r31_3]]].pos.y;
                p3d.z = Hu3DData[lbl_1_bss_468[arg0->order[var_r31_3]]].pos.z;
                Hu3D3Dto2D(&p3d, 1, &sp48);
                PSVECSubtract(&sp48, (Vec *) lbl_1_data_618[var_r31_3], &sp48);
                espScaleSet(lbl_1_bss_C14[var_r31_3], 0.5f, PSVECMag(&sp48) / 31.0f);
                espPosSet(lbl_1_bss_C14[var_r31_3], (sp48.x * 0.5f) + lbl_1_data_618[var_r31_3][0], (sp48.y * 0.5f) + lbl_1_data_618[var_r31_3][1]);
                PSVECNormalize(&sp48, &sp48);
                espZRotSet(lbl_1_bss_C14[var_r31_3], 180.0f + (f32) -(180.0 * (atan2((f64) sp48.x, (f64) sp48.y) / 3.141592653589793)));
                var_r29 = arg0->pl[arg0->order[var_r31_3]].charNo;
                if (var_r29 >= 5) {
                    var_r29 += 1;
                }
                if (var_r29 > 7) {
                    var_r29 = 7;
                }
                GWCharColorGet(var_r29, &col);
                espColorSet(lbl_1_bss_C14[var_r31_3], col.r, col.g, col.b);
            }
            fn_1_502E0(0x15);
            lbl_1_bss_1168[2] += 1;
        }
        break;
    case 3:
        if ((HuPadBtnDown[arg0->order[arg0->curPlayer]] & 0x100) != 0) {
            for (var_r31_4 = 0; var_r31_4 < 4; var_r31_4++) {
                if (lbl_1_bss_C10 == 0) {
                    espDispOn(lbl_1_bss_C1C[var_r31_4]);
                    espDispOn(lbl_1_bss_C14[var_r31_4]);
                } else {
                    espDispOff(lbl_1_bss_C1C[var_r31_4]);
                    espDispOff(lbl_1_bss_C14[var_r31_4]);
                }
            }
            lbl_1_bss_C10 ^= 1;
        }
        if ((HuPadBtnDown[arg0->order[arg0->curPlayer]] & 0x220) != 0) {
            for (var_r31_5 = 0; var_r31_5 < 4; var_r31_5++) {
                espKill(lbl_1_bss_C1C[var_r31_5]);
                espKill(lbl_1_bss_C14[var_r31_5]);
            }
            for (var_r31_6 = 0; var_r31_6 < 4; var_r31_6++) {
                espPosSet(lbl_1_bss_1002[var_r31_6], lbl_1_data_448[var_r31_6][0], lbl_1_data_448[var_r31_6][1]);
                espPosSet(lbl_1_bss_100A[var_r31_6][0], lbl_1_data_428[var_r31_6][0], lbl_1_data_428[var_r31_6][1]);
                espPosSet(lbl_1_bss_100A[var_r31_6][1], lbl_1_data_428[var_r31_6][0], lbl_1_data_428[var_r31_6][1]);
                espPosSet(lbl_1_bss_100A[var_r31_6][2], lbl_1_data_428[var_r31_6][0], lbl_1_data_428[var_r31_6][1]);
                espPosSet(lbl_1_bss_100A[var_r31_6][3], lbl_1_data_428[var_r31_6][0], lbl_1_data_428[var_r31_6][1]);
                espPosSet(lbl_1_bss_FEA[var_r31_6], lbl_1_data_4A8[var_r31_6][0], lbl_1_data_4A8[var_r31_6][1]);
                espPosSet(lbl_1_bss_FE2[var_r31_6], lbl_1_data_4C8[var_r31_6][0], lbl_1_data_4C8[var_r31_6][1]);
                espPosSet(lbl_1_bss_FDA[var_r31_6], lbl_1_data_4E8[var_r31_6][0], lbl_1_data_4E8[var_r31_6][1]);
                espPosSet(lbl_1_bss_FD2[var_r31_6], lbl_1_data_508[var_r31_6][0], lbl_1_data_508[var_r31_6][1]);
                espPosSet(lbl_1_bss_FCA[var_r31_6], lbl_1_data_528[var_r31_6][0], lbl_1_data_528[var_r31_6][1]);
                espPosSet(lbl_1_bss_FC2[var_r31_6], lbl_1_data_548[var_r31_6][0], lbl_1_data_548[var_r31_6][1]);
                espPosSet(lbl_1_bss_FBA[var_r31_6], lbl_1_data_568[var_r31_6][0], lbl_1_data_568[var_r31_6][1]);
                espPosSet(lbl_1_bss_FB2[var_r31_6], lbl_1_data_588[var_r31_6][0], lbl_1_data_588[var_r31_6][1]);
                espPosSet(lbl_1_bss_FAA[var_r31_6], lbl_1_data_5A8[var_r31_6][0], lbl_1_data_5A8[var_r31_6][1]);
                espPosSet(lbl_1_bss_FFA[var_r31_6], lbl_1_data_468[var_r31_6][0], lbl_1_data_468[var_r31_6][1]);
                espPosSet(lbl_1_bss_FF2[var_r31_6], lbl_1_data_488[var_r31_6][0], lbl_1_data_468[var_r31_6][1]);
            }
            fn_1_50460();
            Hu3DCamera->pos.x = lbl_1_bss_C30.x;
            Hu3DCamera->pos.y = lbl_1_bss_C30.y;
            Hu3DCamera->pos.z = lbl_1_bss_C30.z;
            Hu3DCamera->target.x = lbl_1_bss_C24.x;
            Hu3DCamera->target.y = lbl_1_bss_C24.y;
            Hu3DCamera->target.z = lbl_1_bss_C24.z;
            up2.x = 0.0f;
            up2.y = 1.0f;
            up2.z = 0.0f;
            Hu3DCamera->up.x = up2.x;
            Hu3DCamera->up.y = up2.y;
            Hu3DCamera->up.z = up2.z;
            lbl_1_bss_1168[2] = 0;
            var_r28 = 1;
            HuAudFXPlay(0x3FC);
            HuAudSStreamVolSet(0, 0x7FU, 0x64U);
        }
        break;
    }
    return var_r28;
}
s16 lbl_1_bss_1086[4][18];
s32 lbl_1_bss_1130;
s32 lbl_1_bss_1134;
extern void fn_1_4FBFC(CardGame *game, s32 id);
extern void fn_1_4FFEC(void);

s32 fn_1_187FC(CardGame *arg0, s32 arg1)
{
    CardPlayer *temp_r27;
    s32 var_r18;
    s32 var_r19;
    s32 var_r22;
    s32 var_r24;
    s32 var_r29;
    s32 var_r30;
    s32 var_r30_2;
    s32 var_r30_3;
    s32 var_r30_4;
    s32 var_r30_5;
    s32 var_r31;
    s32 var_r31_2;
    s32 var_r31_3;
    u8 var_r20;
    u8 var_r21;

    var_r24 = 0;
    switch (lbl_1_bss_1168[0]) {                 /* irregular */
    case 0:
        for (var_r31 = 0; var_r31 < 4; var_r31++) {
            temp_r27 = &arg0->pl[arg0->order[var_r31]];
            var_r29 = 0;
            for (var_r30 = 0; var_r30 < temp_r27->f44; var_r30++) {
                lbl_1_bss_1086[var_r31][var_r29] = espEntry(lbl_1_data_2B4[8], 0, 0);
                espDrawNoSet(lbl_1_bss_1086[var_r31][var_r29], 0);
                espBankSet(lbl_1_bss_1086[var_r31][var_r29], 1);
                espPosSet(lbl_1_bss_1086[var_r31][var_r29], (25.0f * (f32) var_r29) + lbl_1_data_5C8[var_r31][0], lbl_1_data_5C8[var_r31][1]);
                var_r29 += 1;
            }
            for (var_r30_2 = 0; var_r30_2 < temp_r27->f48; var_r30_2++) {
                lbl_1_bss_1086[var_r31][var_r29] = espEntry(lbl_1_data_2B4[8], 0, 0);
                espDrawNoSet(lbl_1_bss_1086[var_r31][var_r29], 0);
                espBankSet(lbl_1_bss_1086[var_r31][var_r29], 2);
                espPosSet(lbl_1_bss_1086[var_r31][var_r29], (25.0f * (f32) var_r29) + lbl_1_data_5C8[var_r31][0], lbl_1_data_5C8[var_r31][1]);
                var_r29 += 1;
            }
            for (var_r30_3 = 0; var_r30_3 < temp_r27->f4C; var_r30_3++) {
                lbl_1_bss_1086[var_r31][var_r29] = espEntry(lbl_1_data_2B4[8], 0, 0);
                espDrawNoSet(lbl_1_bss_1086[var_r31][var_r29], 0);
                espBankSet(lbl_1_bss_1086[var_r31][var_r29], 3);
                espPosSet(lbl_1_bss_1086[var_r31][var_r29], (25.0f * (f32) var_r29) + lbl_1_data_5C8[var_r31][0], lbl_1_data_5C8[var_r31][1]);
                var_r29 += 1;
            }
            lbl_1_bss_F64[var_r31] = var_r29;
        }
        lbl_1_bss_5E = 0;
        Hu3DModelAttrReset(lbl_1_bss_5C, 1U);
        lbl_1_bss_1134 = 0;
        lbl_1_bss_1130 = 0;
        lbl_1_bss_1168[0] += 1;
        break;
    case 1:
        if (lbl_1_bss_1130 == 0) {
            var_r22 = lbl_1_bss_1134 + 0x14;
        } else {
            var_r22 = lbl_1_bss_1134 - 0x14;
        }
        lbl_1_bss_1134 = var_r22;
        for (var_r31_2 = 0; var_r31_2 < 4; var_r31_2++) {
            espPosSet(lbl_1_bss_1002[var_r31_2], lbl_1_data_448[var_r31_2][0] - (f32) lbl_1_bss_1134, lbl_1_data_448[var_r31_2][1]);
            espPosSet(lbl_1_bss_100A[var_r31_2][0], lbl_1_data_428[var_r31_2][0] - (f32) lbl_1_bss_1134, lbl_1_data_428[var_r31_2][1]);
            espPosSet(lbl_1_bss_100A[var_r31_2][1], lbl_1_data_428[var_r31_2][0] - (f32) lbl_1_bss_1134, lbl_1_data_428[var_r31_2][1]);
            espPosSet(lbl_1_bss_100A[var_r31_2][2], lbl_1_data_428[var_r31_2][0] - (f32) lbl_1_bss_1134, lbl_1_data_428[var_r31_2][1]);
            espPosSet(lbl_1_bss_100A[var_r31_2][3], lbl_1_data_428[var_r31_2][0] - (f32) lbl_1_bss_1134, lbl_1_data_428[var_r31_2][1]);
            espPosSet(lbl_1_bss_FEA[var_r31_2], lbl_1_data_4A8[var_r31_2][0] - (f32) lbl_1_bss_1134, lbl_1_data_4A8[var_r31_2][1]);
            espPosSet(lbl_1_bss_FE2[var_r31_2], lbl_1_data_4C8[var_r31_2][0] - (f32) lbl_1_bss_1134, lbl_1_data_4C8[var_r31_2][1]);
            espPosSet(lbl_1_bss_FDA[var_r31_2], lbl_1_data_4E8[var_r31_2][0] - (f32) lbl_1_bss_1134, lbl_1_data_4E8[var_r31_2][1]);
            espPosSet(lbl_1_bss_FD2[var_r31_2], lbl_1_data_508[var_r31_2][0] - (f32) lbl_1_bss_1134, lbl_1_data_508[var_r31_2][1]);
            espPosSet(lbl_1_bss_FCA[var_r31_2], lbl_1_data_528[var_r31_2][0] - (f32) lbl_1_bss_1134, lbl_1_data_528[var_r31_2][1]);
            espPosSet(lbl_1_bss_FC2[var_r31_2], lbl_1_data_548[var_r31_2][0] - (f32) lbl_1_bss_1134, lbl_1_data_548[var_r31_2][1]);
            espPosSet(lbl_1_bss_FBA[var_r31_2], lbl_1_data_568[var_r31_2][0] - (f32) lbl_1_bss_1134, lbl_1_data_568[var_r31_2][1]);
            espPosSet(lbl_1_bss_FB2[var_r31_2], lbl_1_data_588[var_r31_2][0] - (f32) lbl_1_bss_1134, lbl_1_data_588[var_r31_2][1]);
            espPosSet(lbl_1_bss_FAA[var_r31_2], lbl_1_data_5A8[var_r31_2][0] - (f32) lbl_1_bss_1134, lbl_1_data_5A8[var_r31_2][1]);
            espPosSet(lbl_1_bss_FFA[var_r31_2], lbl_1_data_468[var_r31_2][0] - ((f32) lbl_1_bss_1134 * 0.5f), lbl_1_data_468[var_r31_2][1]);
            espScaleSet(lbl_1_bss_FFA[var_r31_2], 1.0f + (2.5f * (f32) (lbl_1_bss_1134 / 20)), 1.0f);
            for (var_r30_4 = 0; var_r30_4 < lbl_1_bss_F64[var_r31_2]; var_r30_4++) {
                espPosSet(lbl_1_bss_1086[var_r31_2][var_r30_4], ((25.0f * (f32) var_r30_4) + lbl_1_data_5C8[var_r31_2][0]) - (f32) lbl_1_bss_1134, lbl_1_data_5C8[var_r31_2][1]);
            }
        }
        if (lbl_1_bss_1130 == 0) {
            if (lbl_1_bss_5E <= 0x8C) {
                var_r21 = lbl_1_bss_5E + 0xA;
            } else {
                var_r21 = 0x96;
            }
            lbl_1_bss_5E = var_r21;
            if (lbl_1_bss_1134 == 0x168) {
                lbl_1_bss_1130 = 1;
                lbl_1_bss_1168[0] += 1;
            }
        } else {
            if (lbl_1_bss_5E >= 0xA) {
                var_r20 = lbl_1_bss_5E - 0xA;
            } else {
                var_r20 = 0;
            }
            lbl_1_bss_5E = var_r20;
            if (lbl_1_bss_1134 == 0) {
                for (var_r31_3 = 0; var_r31_3 < 4; var_r31_3++) {
                    for (var_r30_5 = 0; var_r30_5 < lbl_1_bss_F64[var_r31_3]; var_r30_5++) {
                        espKill(lbl_1_bss_1086[var_r31_3][var_r30_5]);
                    }
                }
                Hu3DModelAttrSet(lbl_1_bss_5C, 1U);
                lbl_1_bss_1130 = 0;
                lbl_1_bss_1168[0] = 0;
                var_r24 = 1;
            }
        }
        if (arg1 != -1) {
            espColorSet(lbl_1_bss_F9E[0], (u8) (0xFF - lbl_1_bss_5E), (u8) (0xFF - lbl_1_bss_5E), (u8) (0xFF - lbl_1_bss_5E));
            espColorSet(lbl_1_bss_F9E[1], (u8) (0xFF - lbl_1_bss_5E), (u8) (0xFF - lbl_1_bss_5E), (u8) (0xFF - lbl_1_bss_5E));
        }
        break;
    case 2:
        fn_1_502E0(0x16);
        if (arg1 == -1) {
            var_r19 = arg0->order[arg0->curPlayer];
        } else {
            var_r19 = arg1;
        }
        if ((HuPadBtnDown[var_r19] & 0x1000) != 0) {
            fn_1_50460();
            lbl_1_bss_1168[0] -= 1;
            HuAudFXPlay(0x3FC);
            HuAudSStreamVolSet(0, 0x7F, 0x1F4);
        } else if ((HuPadBtnDown[var_r19] & 0x400) != 0) {
            HuAudFXPlay(2);
            fn_1_50460();
            ((s32 *) lbl_1_bss_12A8)[14] = 0;
            fn_1_4FBFC(arg0, 0x43);
            lbl_1_bss_1168[0] += 1;
        }
        break;
    case 3:
        if (arg1 == -1) {
            var_r18 = arg0->order[arg0->curPlayer];
        } else {
            var_r18 = arg1;
        }
        winData[((s32 *) lbl_1_bss_12A8)[17]].padMask = (u8) (1 << GwPlayerConf[var_r18].padNo);
        if (HuWinChoiceGet((s16) ((s32 *) lbl_1_bss_12A8)[17], 1) == 0) {
            var_r24 = -1;
            HuAudSStreamFadeOut(lbl_1_bss_14, 0x3E8);
        } else {
            lbl_1_bss_1168[0] -= 1;
        }
        ((s32 *) lbl_1_bss_12A8)[14] = 1;
        fn_1_4FFEC();
        break;
    }
    return var_r24;
}

s32 lbl_1_bss_113C;

s32 fn_1_21CA4(s32 plSel, CardGame *arg0)
{
    Vec pos;
    Vec v;
    Vec scl;
    CardPlayer *rec;
    CardPlayer *tp;
    s16 id;
    s32 var_r19;
    s32 var_r20;
    s32 var_r25;
    s32 var_r26;
    s32 var_r27;
    s32 i;

    var_r27 = 0;
    var_r26 = 0;
    var_r20 = 0;
    var_r19 = 0;
    var_r25 = 0;
    rec = &arg0->pl[arg0->order[arg0->curPlayer]];
    if (plSel != -1) {
        if (plSel == 0) {
            rec->f50 |= 2;
        }
        fn_1_9DA8(&pos, arg0);
    } else {
        for (i = 0; i < 4; i++) {
            if (arg0->f1A8 == arg0->pl[i].charNo) {
                arg0->pl[i].f50 |= 4;
                arg0->f1AC = i;
                var_r27 = arg0->pl[i].row;
                var_r26 = arg0->pl[i].col;
                var_r20 = arg0->pl[i].f20;
                var_r19 = arg0->pl[i].f24;
                if (arg0->pl[i].f00 == 1) {
                    var_r27 -= 1;
                } else if (arg0->pl[i].f00 == 2) {
                    var_r27 += 1;
                } else if (arg0->pl[i].f00 == 3) {
                    var_r26 -= 1;
                } else if (arg0->pl[i].f00 == 4) {
                    var_r26 += 1;
                }
                break;
            }
        }
        fn_1_9C74(&pos, var_r27, var_r26, var_r20, var_r19);
        if ((arg0->pl[arg0->f1AC].f50 & 0x200) != 0) {
            scl.x = 1.0f;
            scl.y = 0.0f;
            scl.z = 1.0f;
            pos.x += -50.0f;
            v = pos;
            arg0->pl[arg0->f1AC].f56 = fn_1_3ED3C(4, &v);
            fn_1_3F6B0(4, arg0->pl[arg0->f1AC].f56, &scl);
            lbl_1_bss_1168[1] = 0;
            ((s32 *) lbl_1_bss_12A8)[10] = 0x3C;
            /* fn_1_502DC is a bare blr in the original: it returns its first argument (the char number). */
            ((s32 *) lbl_1_bss_12A8)[2] = GwPlayerConf[arg0->f1AC].charNo;
            fn_1_4FBC8(arg0, 0xD);
            return 1;
        }
    }
    switch (lbl_1_bss_1168[1]) {
    case 0:
        lbl_1_bss_1080[0] = Hu3DModelCreateData(lbl_1_data_B0[plSel == 0 ? 0x66 : 0x67]);
        Hu3DModelCameraSet(lbl_1_bss_1080[0], 1U);
        Hu3DModelPosSet(lbl_1_bss_1080[0], -50.0f + pos.x, 1000.0f + pos.y, pos.z);
        Hu3DModelScaleSet(lbl_1_bss_1080[0], 1.6f, 1.6f, 1.6f);
        lbl_1_bss_113C = 0;
        lbl_1_bss_F3C[0] = 0;
        lbl_1_bss_F14[0] = 1.6f;
        if (plSel != -1) {
            lbl_1_bss_1168[1] += 1;
        } else {
            lbl_1_bss_1168[1] = 4;
        }
        break;
    case 1:
        lbl_1_bss_113C += 0x14;
        if (lbl_1_bss_113C > 0x258) {
            lbl_1_bss_113C = 0x258;
        }
        Hu3DModelPosSet(lbl_1_bss_1080[0], -50.0f + pos.x, (1000.0f + pos.y) - (f32) lbl_1_bss_113C, pos.z);
        if (lbl_1_bss_20[0] == 0 && fn_1_8D04(0x319, lbl_1_bss_1080[0], 1) != -1) {
            lbl_1_bss_20[0] = 1;
        }
        if (lbl_1_bss_113C == 0x258) {
            if (plSel == 1) {
                if (lbl_1_bss_F3C[0] == 0) {
                    pos.x += -50.0f;
                    pos.y = pos.y + ((1000.0f - (f32) lbl_1_bss_113C) - 40.0f);
                    v = pos;
                    id = fn_1_3ED3C(2, &v);
                    lbl_1_bss_E9C[0].x = 2.0f;
                    lbl_1_bss_E9C[0].y = 2.0f;
                    lbl_1_bss_E9C[0].z = 2.0f;
                    scl = lbl_1_bss_E9C[0];
                    fn_1_3F6B0(2, id, &scl);
                }
                if (lbl_1_bss_F3C[0] == 0xA) {
                    Hu3DModelKill(lbl_1_bss_1080[0]);
                }
            }
            lbl_1_bss_F3C[0] += 1;
        }
        if (lbl_1_bss_F3C[0] > 0xF) {
            if (plSel != 1) {
                lbl_1_bss_1168[1] += 1;
            } else {
                lbl_1_bss_1168[1] = 3;
            }
            lbl_1_bss_20[0] = 0;
        }
        break;
    case 2:
        if (lbl_1_bss_113C == 0x258) {
            lbl_1_bss_20[0] = (plSel == 0) ? 0x31A : 0x31B;
            fn_1_8D04(lbl_1_bss_20[0], lbl_1_bss_1080[0], 1);
            lbl_1_bss_20[0] = 0;
        }
        lbl_1_bss_113C += 0x14;
        lbl_1_bss_F14[0] -= 0.08f;
        if (lbl_1_bss_F14[0] < 0.0f) {
            lbl_1_bss_F14[0] = 0.0f;
        }
        Hu3DModelPosSet(lbl_1_bss_1080[0], -50.0f + pos.x, (1000.0f + pos.y) - (f32) lbl_1_bss_113C, pos.z);
        Hu3DModelScaleSet(lbl_1_bss_1080[0], lbl_1_bss_F14[0], lbl_1_bss_F14[0], lbl_1_bss_F14[0]);
        if (lbl_1_bss_113C == 0x3E8) {
            pos.x += -50.0f;
            if (plSel == 0) {
                if ((rec->f50 & 4) != 0) {
                    fn_1_3F278(4, rec->f56);
                    rec->f56 = -1;
                    rec->f50 &= 0xFFFFFFFB;
                }
                v = pos;
                rec->f56 = fn_1_3ED3C(3, &v);
            } else if (plSel != 0) {
                tp = &arg0->pl[arg0->f1AC];
                if ((tp->f50 & 2) != 0) {
                    fn_1_3F278(3, tp->f56);
                    tp->f56 = -1;
                    tp->f50 &= 0xFFFFFFFD;
                } else if ((tp->f50 & 4) != 0 && tp->f56 != -1) {
                    fn_1_3F278(4, tp->f56);
                    tp->f56 = -1;
                }
                v = pos;
                tp->f56 = fn_1_3ED3C(4, &v);
            }
            lbl_1_bss_1168[1] += 1;
        }
        break;
    case 3:
        if (plSel != 1) {
            Hu3DModelKill(lbl_1_bss_1080[0]);
        }
        lbl_1_bss_1168[1] = 0;
        var_r25 = 1;
        ((s32 *) lbl_1_bss_12A8)[10] = 0x3C;
        if (plSel == 0) {
            fn_1_4FBC8(arg0, (rec->f50 & 0x800) != 0 ? 0x3F : 0xB);
        } else if (plSel == -1) {
            ((s32 *) lbl_1_bss_12A8)[2] = GwPlayerConf[arg0->f1AC].charNo;
            fn_1_4FBC8(arg0, 0xD);
        }
        break;
    case 4:
        if (fn_1_30FA4(9, &pos) != 0) {
            lbl_1_bss_1168[1] = 1;
        }
        break;
    }
    return var_r25;
}

s16 lbl_1_bss_F84;
extern void fn_1_352D0(CardGame *game);
extern s32 fn_1_F2A8(s32 mode, s32 row, s32 col);
extern s32 fn_1_5710(CardGame *game, s32 plNo, s32 flag);
extern s32 fn_1_36C00(CardGame *game);
extern s32 fn_1_3EA44(CardGame *game);
extern s32 fn_1_47AA8(CardGame *game);
extern s32 fn_1_325F4(CardGame *game);
extern s32 fn_1_5248(CardGame *game);

s32 fn_1_2930C(CardGame *arg0)
{
    Vec pos;
    Vec v;
    s32 list[4];
    CardPlayer *rec;
    s32 plNo;
    s32 var_r25;
    s32 var_r27;
    s32 var_r28;
    s32 i;

    var_r28 = 0;
    rec = &arg0->pl[arg0->order[arg0->curPlayer]];
    pos.x = 288.0f;
    pos.y = 150.0f;
    pos.z = 1500.0f;
    if (lbl_1_bss_1168[0] <= 0x2A) {
        switch (lbl_1_bss_1168[0]) {
        case 0:
            if (arg0->f1D4 == 0) {
                fn_1_352D0(arg0);
            }
            if ((lbl_1_bss_8C4[rec->row][rec->col] & 0xF00) != 0) {
                if ((lbl_1_bss_8C4[rec->row][rec->col] & 0x800) != 0) {
                    arg0->f19C = 5;
                    lbl_1_bss_1168[0] = 1;
                } else {
                    if ((lbl_1_bss_8C4[rec->row][rec->col] & 0x100) != 0) {
                        arg0->f19C = 4;
                        HuAudFXPlay(0x447);
                    } else if ((lbl_1_bss_8C4[rec->row][rec->col] & 0x200) != 0) {
                        arg0->f19C = 5;
                        HuAudFXPlay(0x448);
                    } else if ((lbl_1_bss_8C4[rec->row][rec->col] & 0x400) != 0) {
                        arg0->f19C = 6;
                        HuAudFXPlay(0x44A);
                    }
                    lbl_1_bss_1168[0] = 0x14;
                }
                fn_1_30B00(arg0, -1);
            } else {
                lbl_1_bss_1168[0] = 1;
            }
            fn_1_F2A8(0, 0, 0);
            break;
        case 1:
            if (fn_1_F2A8(1, rec->row, rec->col) != 0) {
                if ((lbl_1_bss_8C4[rec->row][rec->col] & 0xF00) != 0) {
                    if ((lbl_1_bss_8C4[rec->row][rec->col] & 0x800) != 0) {
                        lbl_1_bss_1168[0] = 0x28;
                        HuAudSStreamVolSet(0, 0, 0x3E8);
                    } else {
                        lbl_1_bss_1168[0] = 3;
                    }
                } else if (frandmod(0x64) < 0x14U && (rec->f50 & 0x400) == 0 && (rec->f50 & 2) == 0 && (rec->f50 & 4) == 0
                           && arg0->f1A0 == 0) {
                    arg0->f184 += 1;
                    rec->f50 |= 0x400;
                    lbl_1_bss_1168[0] = 0x1E;
                } else {
                    lbl_1_bss_1168[0] = 0;
                    var_r28 = 1;
                }
            }
            break;
        case 2:
            if (fn_1_30FA4(arg0->f19C, arg0) != 0) {
                lbl_1_bss_1168[0] = 1;
                if ((lbl_1_bss_8C4[rec->row][rec->col] & 0x200) != 0) {
                    if ((lbl_1_bss_8C4[rec->row][rec->col] & 0x2000) != 0) {
                        if (frandmod(2) != 0) {
                            var_r27 = 0;
                        } else if ((rec->f50 & 0x1000) != 0) {
                            var_r27 = 0;
                        } else if (fn_1_5710(arg0, arg0->curPlayer, 1) != 0) {
                            var_r27 = 0;
                        } else {
                            var_r27 = 0;
                            for (i = 0; i < 4; i++) {
                                if (i != arg0->curPlayer && fn_1_5710(arg0, i, 0) == 0) {
                                    list[var_r27] = i;
                                    var_r27 += 1;
                                }
                            }
                        }
                        if (var_r27 != 0) {
                            arg0->f1A4 = list[frandmod(var_r27)];
                        } else {
                            arg0->f1A4 = -1;
                            fn_1_F104(rec->row, rec->col);
                        }
                    } else if ((lbl_1_bss_8C4[rec->row][rec->col] & 0x1000) != 0) {
                        for (i = 0; i < 4; i++) {
                            if (i != arg0->order[arg0->curPlayer] && rec->f04 == arg0->pl[i].f00) {
                                lbl_1_bss_8C4[rec->row][rec->col] &= 0xFFFFEFFF;
                                lbl_1_bss_8C4[rec->row][rec->col] |= 0x2000;
                                arg0->f1A4 = -1;
                                fn_1_F1A8(rec->row, rec->col);
                                break;
                            }
                        }
                    }
                }
            }
            break;
        case 3:
            if ((lbl_1_bss_8C4[rec->row][rec->col] & 0x100) != 0) {
                lbl_1_bss_1168[0] = 4;
            } else if ((lbl_1_bss_8C4[rec->row][rec->col] & 0x200) != 0) {
                lbl_1_bss_1168[0] = 5;
            } else if ((lbl_1_bss_8C4[rec->row][rec->col] & 0x400) != 0) {
                lbl_1_bss_1168[0] = 6;
                Hu3DModelShadowMapObjReset(lbl_1_bss_478[rec->row][rec->col]);
            }
            break;
        case 4:
            if (fn_1_2A260(arg0) != 0) {
                arg0->cnt1C8 -= 1;
                lbl_1_bss_1168[0] = 7;
            }
            break;
        case 5:
            if (fn_1_2B10C(arg0) != 0) {
                arg0->cnt1CC -= 1;
                lbl_1_bss_1168[0] = 7;
            }
            break;
        case 6:
            if (fn_1_36C00(arg0) != 0) {
                arg0->cnt1D0 -= 1;
                lbl_1_bss_1168[0] = 7;
            }
            break;
        case 7:
            fn_1_2FFAC(arg0);
            lbl_1_bss_1168[0] = 8;
            break;
        case 8:
            if (fn_1_EB38(rec->row, rec->col) != 0) {
                lbl_1_bss_8C4[rec->row][rec->col] &= 0xFF;
                if (arg0->f1D4 == 0xA) {
                    arg0->f184 = 0;
                    arg0->f18C = 0x133;
                    fn_1_2FBCC(arg0);
                    lbl_1_bss_1168[0] = 0;
                    var_r28 = 1;
                } else {
                    lbl_1_bss_F3C[0] = rec->row;
                    lbl_1_bss_F3C[1] = rec->col;
                    lbl_1_bss_F3C[2] = rec->f20;
                    lbl_1_bss_F3C[3] = rec->f24;
                    if (rec->rotY == 180.0f) {
                        rec->row += 1;
                        if ((lbl_1_bss_8C4[rec->row][rec->col] & 0x40) != 0) {
                            rec->f20 = 1;
                        } else {
                            rec->f20 = 0;
                        }
                    } else if (rec->rotY == 0.0f) {
                        rec->row -= 1;
                        if ((lbl_1_bss_8C4[rec->row][rec->col] & 0x40) != 0) {
                            rec->f20 = 1;
                        } else {
                            rec->f20 = 2;
                        }
                    } else if (rec->rotY == -90.0f) {
                        rec->col += 1;
                        if ((lbl_1_bss_8C4[rec->row][rec->col] & 0x40) != 0) {
                            rec->f24 = 1;
                        } else {
                            rec->f24 = 0;
                        }
                    } else if (rec->rotY == 90.0f) {
                        rec->col -= 1;
                        if ((lbl_1_bss_8C4[rec->row][rec->col] & 0x40) != 0) {
                            rec->f24 = 1;
                        } else {
                            rec->f24 = 2;
                        }
                    }
                    if (arg0->f1D4 == 0x14) {
                        arg0->f184 = 0;
                        arg0->f18C = 0x133;
                        lbl_1_bss_1168[0] = 0;
                        var_r28 = 1;
                    } else if ((rec->f50 & 8) != 0 || (rec->f50 & 0x10) != 0) {
                        arg0->f184 = 0;
                        arg0->f18C = 0x133;
                        if ((rec->f50 & 8) != 0) {
                            fn_1_2B280(arg0);
                        } else if ((rec->f50 & 0x10) != 0) {
                            fn_1_2E548(arg0);
                        }
                        lbl_1_bss_1168[0] = 0xB;
                    } else if (arg0->cnt1D0 == 0) {
                        lbl_1_bss_1168[0] = 0xA;
                        fn_1_36828();
                        lbl_1_bss_F14[8] = 360.0f;
                        pos.y += lbl_1_bss_F14[8];
                        fn_1_36A70(&pos);
                        lbl_1_bss_F14[9] = 900.0f;
                        Hu3DModelRotSet(fn_1_36A60(), 0.0f, lbl_1_bss_F14[9], 0.0f);
                    } else {
                        if (arg0->f19C == 4) {
                            var_r25 = 3;
                        } else {
                            var_r25 = 8;
                        }
                        arg0->f19C = var_r25;
                        lbl_1_bss_1168[0] = 9;
                    }
                }
            }
            break;
        case 9:
            if (fn_1_30FA4(arg0->f19C, arg0) != 0) {
                rec->row = lbl_1_bss_F3C[0];
                rec->col = lbl_1_bss_F3C[1];
                rec->f20 = lbl_1_bss_F3C[2];
                rec->f24 = lbl_1_bss_F3C[3];
                lbl_1_bss_1168[0] = 0;
                var_r28 = 1;
                fn_1_30B00(arg0, 1);
            }
            break;
        case 10:
            if (fn_1_30FA4(0, arg0) != 0) {
                lbl_1_bss_1168[0] = 0;
                var_r28 = 1;
            } else {
                lbl_1_bss_F14[8] -= 6.0f;
                pos.y += lbl_1_bss_F14[8];
                fn_1_36A70(&pos);
                lbl_1_bss_F14[9] -= 15.0f;
                Hu3DModelRotSet(fn_1_36A60(), 0.0f, lbl_1_bss_F14[9], 0.0f);
            }
            break;
        case 11:
            if (fn_1_5248(arg0) != 0) {
                lbl_1_bss_1168[0] = 0;
                var_r28 = 1;
            }
            break;
        case 20:
            if (fn_1_325F4(arg0) != 0) {
                lbl_1_bss_1168[0] = 2;
            }
            break;
        case 30:
            plNo = arg0->order[arg0->curPlayer];
            if (GwPlayerConf[plNo].type != 0) {
                HuPadBtnDown[plNo] = 0;
                HuPadDStkRep[plNo] = 0;
                lbl_1_bss_C48[0] += 1;
                lbl_1_bss_C48[0] %= 0x15E;
                if ((u32) lbl_1_bss_C48[0] > (u32) (frandmod(0x12C) + 0x1F)) {
                    HuPadBtnDown[plNo] = 0x100;
                    lbl_1_bss_C48[0] = 0;
                }
            }
            if (fn_1_3EA44(arg0) != 0) {
                arg0->f184 -= 1;
                lbl_1_bss_C48[0] = 0;
                lbl_1_bss_C48[1] = 0;
                lbl_1_bss_1168[0] = 0;
                var_r28 = 1;
            }
            break;
        case 40:
            if (fn_1_30FA4(arg0->f19C, arg0) != 0) {
                fn_1_9C74(&pos, rec->row, rec->col, 1, 1);
                pos.x += -50.0f;
                v = pos;
                lbl_1_bss_F84 = fn_1_3ED3C(0x19, &v);
                lbl_1_bss_1168[0] += 1;
                fn_1_8D04(0x435, lbl_1_bss_478[rec->row][rec->col], 1);
                HuAudJinglePlay(0x8B);
            }
            break;
        case 41:
            if (fn_1_3F714(0x19, lbl_1_bss_F84) != 0) {
                lbl_1_bss_1168[0] += 1;
            }
            break;
        case 42:
            if (fn_1_47AA8(arg0) != 0) {
                if (arg0->f1D4 == 1) {
                    arg0->f1D4 = -1;
                }
                lbl_1_bss_1168[0] = 8;
            }
            break;
        }
    }
    return var_r28;
}

s16 lbl_1_bss_104E;
s16 lbl_1_bss_1056[21];
Vec lbl_1_bss_E48;
Vec lbl_1_bss_E54;
extern void fn_1_4F144(s32 i, Vec *pos2D, Vec *target, s32 frames, f32 arg4, s32 arg5);
extern s32 fn_1_4F1EC(s32 i, Vec *pos2D);
extern void fn_1_4EDB8(Vec *out, Vec *in, f32 x, f32 y);

s32 fn_1_2A260(CardGame *arg0)
{
    Vec pos;
    Vec v;
    Vec v2;
    Vec v3;
    f32 step;
    s16 temp_r25;
    s32 var_r29;
    CardPlayer *rec;

    var_r29 = 0;
    rec = &arg0->pl[arg0->order[arg0->curPlayer]];
    fn_1_9C74(&pos, rec->row, rec->col, 1, 1);
    switch (lbl_1_bss_1168[1]) {
    case 0:
        if (rec->f3C == -1) {
            if ((lbl_1_bss_8C4[rec->row][rec->col] & 0x1000) != 0) {
                if (rec->f34 == -1) {
                    rec->f34 = 1;
                    lbl_1_bss_F3C[0] = 0;
                } else if (rec->f38 == -1) {
                    rec->f38 = 1;
                    lbl_1_bss_F3C[0] = 1;
                } else {
                    rec->f3C = 1;
                    lbl_1_bss_F3C[0] = 2;
                }
                lbl_1_bss_F3C[1] = 1;
            } else if ((lbl_1_bss_8C4[rec->row][rec->col] & 0x2000) != 0) {
                if (rec->f34 == -1) {
                    rec->f34 = 2;
                    lbl_1_bss_F3C[0] = 0;
                } else if (rec->f38 == -1) {
                    rec->f38 = 2;
                    lbl_1_bss_F3C[0] = 1;
                } else {
                    rec->f3C = 2;
                    lbl_1_bss_F3C[0] = 2;
                }
                lbl_1_bss_F3C[1] = 2;
            } else {
                if (rec->f34 == -1) {
                    rec->f34 = 0;
                    lbl_1_bss_F3C[0] = 0;
                } else if (rec->f38 == -1) {
                    rec->f38 = 0;
                    lbl_1_bss_F3C[0] = 1;
                } else {
                    rec->f3C = 0;
                    lbl_1_bss_F3C[0] = 2;
                }
                lbl_1_bss_F3C[1] = 0;
            }
            lbl_1_bss_1168[1] += 1;
        } else {
            ((s32 *) lbl_1_bss_12A8)[10] = 0x3C;
            fn_1_4FBC8(arg0, 9);
            var_r29 = 1;
        }
        break;
    case 1:
        {
            s32 model;

            if (lbl_1_bss_F3C[1] == 0) {
                model = 0x1A;
            } else if (lbl_1_bss_F3C[1] == 1) {
                model = 0x18;
            } else {
                model = 0x19;
            }
            lbl_1_bss_104E = Hu3DModelCreateData(lbl_1_data_B0[model]);
        }
        Hu3DModelCameraSet(lbl_1_bss_104E, 1U);
        Hu3DModelPosSet(lbl_1_bss_104E, -50.0f + pos.x, 220.0f + pos.y, pos.z);
        Hu3DModelRotSet(lbl_1_bss_104E, 90.0f, 0.0f, 0.0f);
        Hu3DModelAttrSet(lbl_1_bss_104E, 1U);
        Hu3DModelScaleSet(lbl_1_bss_104E, 0.5f, 0.5f, 0.5f);
        Hu3DModelLayerSet(lbl_1_bss_104E, 2);
        lbl_1_bss_1056[0] = Hu3DLLightCreate(lbl_1_bss_104E, -50.0f + pos.x, 220.0f + pos.y, 1000.0f + pos.z, 0.0f, 0.0f, -1.0f,
            0x80U, 0x80U, 0x80U);
        Hu3DLLightStaticSet(lbl_1_bss_104E, lbl_1_bss_1056[0], 1);
        Hu3DLLightInfinitytSet(lbl_1_bss_104E, lbl_1_bss_1056[0]);
        lbl_1_bss_E48.x = -50.0f + pos.x;
        lbl_1_bss_E48.y = 220.0f + pos.y;
        lbl_1_bss_E48.z = pos.z;
        pos.x += -50.0f;
        pos.y += 240.0f;
        pos.z = pos.z + 100.0f;
        v = pos;
        temp_r25 = fn_1_3ED3C(1, &v);
        lbl_1_bss_E9C[0].x = 2.0f;
        lbl_1_bss_E9C[0].y = 2.0f;
        lbl_1_bss_E9C[0].z = 2.0f;
        v2 = lbl_1_bss_E9C[0];
        fn_1_3F6B0(1, temp_r25, &v2);
        lbl_1_bss_F3C[2] = 0;
        lbl_1_bss_F14[0] = 0.0f;
        lbl_1_bss_1168[1] += 1;
        break;
    case 2:
        lbl_1_bss_F3C[2] += 1;
        if (lbl_1_bss_F3C[2] >= 0xA) {
            Hu3DModelAttrReset(lbl_1_bss_104E, 1U);
            if (lbl_1_bss_F14[0] < 1080.0f) {
                step = 108.0f;
            } else if (lbl_1_bss_F14[0] < 1800.0f) {
                step = 72.0f;
            } else if (lbl_1_bss_F14[0] < 2160.0f) {
                step = 36.0f;
            } else {
                step = 18.0f;
            }
            lbl_1_bss_F14[0] += step;
            if (lbl_1_bss_F14[0] >= 2880.0f) {
                lbl_1_bss_F14[0] = 2880.0f;
            }
            Hu3DModelRotSet(lbl_1_bss_104E, 90.0f, lbl_1_bss_F14[0], 0.0f);
        }
        if (lbl_1_bss_F3C[2] == 0xA) {
            fn_1_8D04(0x409, lbl_1_bss_104E, 1);
        }
        if (lbl_1_bss_F14[0] == 2880.0f) {
            lbl_1_bss_F3C[2] = 0;
            lbl_1_bss_1168[1] += 1;
        }
        break;
    case 3:
        lbl_1_bss_F3C[2] += 1;
        if (lbl_1_bss_F3C[2] > 0x1E) {
            Hu3D3Dto2D(&lbl_1_bss_E48, 1, &lbl_1_bss_E54);
            if (lbl_1_bss_F3C[0] == 0) {
                pos.x = lbl_1_data_548[arg0->curPlayer][0];
            } else if (lbl_1_bss_F3C[0] == 1) {
                pos.x = lbl_1_data_568[arg0->curPlayer][0];
            } else {
                pos.x = lbl_1_data_588[arg0->curPlayer][0];
            }
            if (lbl_1_bss_F3C[0] == 0) {
                pos.y = lbl_1_data_548[arg0->curPlayer][1];
            } else if (lbl_1_bss_F3C[0] == 1) {
                pos.y = lbl_1_data_568[arg0->curPlayer][1];
            } else {
                pos.y = lbl_1_data_588[arg0->curPlayer][1];
            }
            pos.z = lbl_1_bss_E48.z;
            v = pos;
            v3 = lbl_1_bss_E54;
            fn_1_4F144(0, &v3, &v, 0x28, 0.0f, 0x21);
            lbl_1_bss_F3C[2] = 0;
            lbl_1_bss_F14[1] = 0.0f;
            Hu3DCamera[1] = Hu3DCamera[0];
            Hu3DModelLayerSet(lbl_1_bss_104E, 7);
            Hu3DModelCameraSet(lbl_1_bss_104E, 2U);
            ((s32 *) lbl_1_bss_12A8)[10] = 0x3C;
            switch (lbl_1_bss_F3C[1]) {
            case 0:
                fn_1_4FBC8(arg0, 8);
                break;
            case 1:
                fn_1_4FBC8(arg0, 6);
                break;
            case 2:
                fn_1_4FBC8(arg0, 7);
                break;
            }
            lbl_1_bss_1168[1] += 1;
        }
        break;
    case 4:
        if (lbl_1_bss_F14[0] == 2880.0f) {
            fn_1_8D04(0x40C, lbl_1_bss_104E, 1);
        }
        fn_1_8D04(0x840C, lbl_1_bss_104E, 1);
        if (lbl_1_bss_F14[0] < 1080.0f) {
            step = 108.0f;
        } else if (lbl_1_bss_F14[0] < 1800.0f) {
            step = 72.0f;
        } else if (lbl_1_bss_F14[0] < 2160.0f) {
            step = 36.0f;
        } else {
            step = 18.0f;
        }
        lbl_1_bss_F14[0] -= step;
        Hu3DModelRotSet(lbl_1_bss_104E, 90.0f, -lbl_1_bss_F14[0], 0.0f);
        if (lbl_1_bss_F14[0] < 2160.0f) {
            lbl_1_bss_F14[1] += 0.0125f;
            if (lbl_1_bss_F14[1] > 0.35f) {
                lbl_1_bss_F14[1] = 0.35f;
            }
            Hu3DModelScaleSet(lbl_1_bss_104E, 0.5f - lbl_1_bss_F14[1], 0.5f - lbl_1_bss_F14[1], 0.5f - lbl_1_bss_F14[1]);
            fn_1_4F1EC(0, &pos);
            v = lbl_1_bss_E48;
            fn_1_4EDB8(&lbl_1_bss_E9C[0], &v, pos.x, pos.y);
            Hu3DModelPosSetV(lbl_1_bss_104E, &lbl_1_bss_E9C[0]);
            lbl_1_bss_F3C[2] += 1;
            if (lbl_1_bss_F3C[2] > 0x28) {
                Hu3DLLightKill(lbl_1_bss_104E, lbl_1_bss_1056[0]);
                Hu3DModelKill(lbl_1_bss_104E);
                lbl_1_bss_1168[1] = 0;
                var_r29 = 1;
            }
            if (lbl_1_bss_F3C[2] == 0x28) {
                fn_1_8D04(0x40A, lbl_1_bss_104E, 1);
            }
        }
        break;
    }
    return var_r29;
}

s32 fn_1_FCD4(CardGame *arg0)
{
    s16 var_r31;
    s32 temp_r31;
    s32 var_r28;
    s32 var_r29;
    s32 var_r31_2;
    s32 var_r31_3;

    var_r29 = 0;
    switch (lbl_1_bss_1168[0]) {                 /* irregular */
    case 0:
        for (var_r31 = 0; var_r31 < 4; var_r31++) {
            lbl_1_bss_1002[var_r31] = espEntry(lbl_1_data_2B4[4], 0, 0);
            espDrawNoSet(lbl_1_bss_1002[var_r31], 0x40);
            espBankSet(lbl_1_bss_1002[var_r31], GwPlayerConf[arg0->order[var_r31]].charNo);
            espTPLvlSet(lbl_1_bss_1002[var_r31], 0.3f);
            espPriSet(lbl_1_bss_1002[var_r31], 0x1F0);
            lbl_1_bss_FFA[var_r31] = espEntry(lbl_1_data_2B4[5], 0, 0);
            espDrawNoSet(lbl_1_bss_FFA[var_r31], 0x40);
            espBankSet(lbl_1_bss_FFA[var_r31], (s16) (GwPlayerConf[arg0->order[var_r31]].charNo * 2));
            espTPLvlSet(lbl_1_bss_FFA[var_r31], 0.3f);
            espPriSet(lbl_1_bss_FFA[var_r31], 0x1F0);
            lbl_1_bss_FF2[var_r31] = espEntry(lbl_1_data_2B4[5], 0, 0);
            espDrawNoSet(lbl_1_bss_FF2[var_r31], 0x40);
            espBankSet(lbl_1_bss_FF2[var_r31], (s16) ((GwPlayerConf[arg0->order[var_r31]].charNo * 2) + 1));
            espTPLvlSet(lbl_1_bss_FF2[var_r31], 0.3f);
            espPriSet(lbl_1_bss_FF2[var_r31], 0x1F0);
            lbl_1_bss_100A[var_r31][0] = espEntry(lbl_1_data_2B4[0], 0, 0);
            espDrawNoSet(lbl_1_bss_100A[var_r31][0], 0x40);
            espBankSet(lbl_1_bss_100A[var_r31][0], GwPlayerConf[arg0->order[var_r31]].charNo);
            espPriSet(lbl_1_bss_100A[var_r31][0], 0x1E0);
            lbl_1_bss_100A[var_r31][1] = espEntry(lbl_1_data_2B4[1], 0, 0);
            espDrawNoSet(lbl_1_bss_100A[var_r31][1], 0x40);
            espBankSet(lbl_1_bss_100A[var_r31][1], GwPlayerConf[arg0->order[var_r31]].charNo);
            espPriSet(lbl_1_bss_100A[var_r31][1], 0x1E0);
            espDispOff(lbl_1_bss_100A[var_r31][1]);
            lbl_1_bss_100A[var_r31][2] = espEntry(lbl_1_data_2B4[2], 0, 0);
            espDrawNoSet(lbl_1_bss_100A[var_r31][2], 0x40);
            espBankSet(lbl_1_bss_100A[var_r31][2], GwPlayerConf[arg0->order[var_r31]].charNo);
            espPriSet(lbl_1_bss_100A[var_r31][2], 0x1E0);
            espDispOff(lbl_1_bss_100A[var_r31][2]);
            lbl_1_bss_100A[var_r31][3] = espEntry(lbl_1_data_2B4[3], 0, 0);
            espDrawNoSet(lbl_1_bss_100A[var_r31][3], 0x40);
            espBankSet(lbl_1_bss_100A[var_r31][3], GwPlayerConf[arg0->order[var_r31]].charNo);
            espPriSet(lbl_1_bss_100A[var_r31][3], 0x1E0);
            espDispOff(lbl_1_bss_100A[var_r31][3]);
            lbl_1_bss_FEA[var_r31] = espEntry(lbl_1_data_2B4[6], 0, 0);
            espDrawNoSet(lbl_1_bss_FEA[var_r31], 0x40);
            espBankSet(lbl_1_bss_FEA[var_r31], 0);
            espPriSet(lbl_1_bss_FEA[var_r31], 0x1D0);
            lbl_1_bss_FE2[var_r31] = espEntry(lbl_1_data_2B4[7], 0, 0);
            espDrawNoSet(lbl_1_bss_FE2[var_r31], 0x40);
            espBankSet(lbl_1_bss_FE2[var_r31], 2);
            espPriSet(lbl_1_bss_FE2[var_r31], 0x1E0);
            lbl_1_bss_FDA[var_r31] = espEntry(lbl_1_data_2B4[7], 0, 0);
            espDrawNoSet(lbl_1_bss_FDA[var_r31], 0x40);
            espBankSet(lbl_1_bss_FDA[var_r31], 3);
            espPriSet(lbl_1_bss_FDA[var_r31], 0x1D0);
            lbl_1_bss_FD2[var_r31] = espEntry(lbl_1_data_2B4[7], 0, 0);
            espDrawNoSet(lbl_1_bss_FD2[var_r31], 0x40);
            espBankSet(lbl_1_bss_FD2[var_r31], 3);
            espPriSet(lbl_1_bss_FD2[var_r31], 0x1C0);
            lbl_1_bss_FCA[var_r31] = espEntry(lbl_1_data_2B4[8], 0, 0);
            espDrawNoSet(lbl_1_bss_FCA[var_r31], 0x40);
            espBankSet(lbl_1_bss_FCA[var_r31], 0);
            espPriSet(lbl_1_bss_FCA[var_r31], 0x1E0);
            lbl_1_bss_FC2[var_r31] = espEntry(lbl_1_data_2B4[9], 0, 0);
            espDrawNoSet(lbl_1_bss_FC2[var_r31], 0x40);
            espBankSet(lbl_1_bss_FC2[var_r31], 0);
            espPriSet(lbl_1_bss_FC2[var_r31], 0x1E0);
            lbl_1_bss_FBA[var_r31] = espEntry(lbl_1_data_2B4[9], 0, 0);
            espDrawNoSet(lbl_1_bss_FBA[var_r31], 0x40);
            espBankSet(lbl_1_bss_FBA[var_r31], 1);
            espPriSet(lbl_1_bss_FBA[var_r31], 0x1D0);
            lbl_1_bss_FB2[var_r31] = espEntry(lbl_1_data_2B4[9], 0, 0);
            espDrawNoSet(lbl_1_bss_FB2[var_r31], 0x40);
            espBankSet(lbl_1_bss_FB2[var_r31], 2);
            espPriSet(lbl_1_bss_FB2[var_r31], 0x1C0);
            lbl_1_bss_FAA[var_r31] = espEntry(lbl_1_data_2B4[0xA], 0, 0);
            espDrawNoSet(lbl_1_bss_FAA[var_r31], 0x40);
            espBankSet(lbl_1_bss_FAA[var_r31], var_r31);
            espPriSet(lbl_1_bss_FC2[var_r31], 0x1D0);
            lbl_1_bss_FA2[var_r31] = espEntry(lbl_1_data_2B4[GwPlayerConf[arg0->order[var_r31]].charNo + 0x1B], 0, 0);
            espDrawNoSet(lbl_1_bss_FA2[var_r31], 0);
            espPosSet(lbl_1_bss_FA2[var_r31], 200.0f, 540.0f);
            espDispOff(lbl_1_bss_FA2[var_r31]);
        }
        HuSprExecLayerSet(0x40, 6);
        fn_1_2FFAC(arg0);
        for (var_r31_2 = 0; var_r31_2 < 4; var_r31_2++) {
            lbl_1_bss_1140[var_r31_2] = 0x12C;
            espPosSet(lbl_1_bss_1002[var_r31_2], lbl_1_data_448[var_r31_2][0] + (f32) lbl_1_bss_1140[var_r31_2], lbl_1_data_448[var_r31_2][1]);
            espPosSet(lbl_1_bss_FFA[var_r31_2], lbl_1_data_468[var_r31_2][0] + (f32) lbl_1_bss_1140[var_r31_2], lbl_1_data_468[var_r31_2][1]);
            espPosSet(lbl_1_bss_FF2[var_r31_2], lbl_1_data_488[var_r31_2][0] + (f32) lbl_1_bss_1140[var_r31_2], lbl_1_data_488[var_r31_2][1]);
            espPosSet(lbl_1_bss_100A[var_r31_2][0], lbl_1_data_428[var_r31_2][0] + (f32) lbl_1_bss_1140[var_r31_2], lbl_1_data_428[var_r31_2][1]);
            espPosSet(lbl_1_bss_100A[var_r31_2][1], lbl_1_data_428[var_r31_2][0] + (f32) lbl_1_bss_1140[var_r31_2], lbl_1_data_428[var_r31_2][1]);
            espPosSet(lbl_1_bss_100A[var_r31_2][2], lbl_1_data_428[var_r31_2][0] + (f32) lbl_1_bss_1140[var_r31_2], lbl_1_data_428[var_r31_2][1]);
            espPosSet(lbl_1_bss_100A[var_r31_2][3], lbl_1_data_428[var_r31_2][0] + (f32) lbl_1_bss_1140[var_r31_2], lbl_1_data_428[var_r31_2][1]);
            espPosSet(lbl_1_bss_FEA[var_r31_2], lbl_1_data_4A8[var_r31_2][0] + (f32) lbl_1_bss_1140[var_r31_2], lbl_1_data_4A8[var_r31_2][1]);
            espPosSet(lbl_1_bss_FE2[var_r31_2], lbl_1_data_4C8[var_r31_2][0] + (f32) lbl_1_bss_1140[var_r31_2], lbl_1_data_4C8[var_r31_2][1]);
            espPosSet(lbl_1_bss_FDA[var_r31_2], lbl_1_data_4E8[var_r31_2][0] + (f32) lbl_1_bss_1140[var_r31_2], lbl_1_data_4E8[var_r31_2][1]);
            espPosSet(lbl_1_bss_FD2[var_r31_2], lbl_1_data_508[var_r31_2][0] + (f32) lbl_1_bss_1140[var_r31_2], lbl_1_data_508[var_r31_2][1]);
            espPosSet(lbl_1_bss_FCA[var_r31_2], lbl_1_data_528[var_r31_2][0] + (f32) lbl_1_bss_1140[var_r31_2], lbl_1_data_528[var_r31_2][1]);
            espPosSet(lbl_1_bss_FC2[var_r31_2], lbl_1_data_548[var_r31_2][0] + (f32) lbl_1_bss_1140[var_r31_2], lbl_1_data_548[var_r31_2][1]);
            espPosSet(lbl_1_bss_FBA[var_r31_2], lbl_1_data_568[var_r31_2][0] + (f32) lbl_1_bss_1140[var_r31_2], lbl_1_data_568[var_r31_2][1]);
            espPosSet(lbl_1_bss_FB2[var_r31_2], lbl_1_data_588[var_r31_2][0] + (f32) lbl_1_bss_1140[var_r31_2], lbl_1_data_588[var_r31_2][1]);
            espPosSet(lbl_1_bss_FAA[var_r31_2], lbl_1_data_5A8[var_r31_2][0] + (f32) lbl_1_bss_1140[var_r31_2], lbl_1_data_5A8[var_r31_2][1]);
            if ((s16) GwPlayerConf[arg0->order[var_r31_2]].type == 0) {
                espDispOff(lbl_1_bss_FEA[var_r31_2]);
            }
        }
        lbl_1_bss_1168[0] += 1;
        break;
    case 1:
        lbl_1_bss_113C = fn_1_15D38(arg0);
        if (lbl_1_bss_113C != -1) {
            lbl_1_bss_1168[0] += 1;
        }
        break;
    case 2:
        temp_r31 = lbl_1_bss_113C;
        if (lbl_1_bss_1140[temp_r31] > 0) {
            var_r28 = lbl_1_bss_1140[temp_r31] - 0x14;
        } else {
            var_r28 = 0;
        }
        lbl_1_bss_1140[temp_r31] = var_r28;
        espPosSet(lbl_1_bss_1002[temp_r31], lbl_1_data_448[temp_r31][0] + (f32) lbl_1_bss_1140[temp_r31], lbl_1_data_448[temp_r31][1]);
        espPosSet(lbl_1_bss_FFA[temp_r31], lbl_1_data_468[temp_r31][0] + (f32) lbl_1_bss_1140[temp_r31], lbl_1_data_468[temp_r31][1]);
        espPosSet(lbl_1_bss_FF2[temp_r31], lbl_1_data_488[temp_r31][0] + (f32) lbl_1_bss_1140[temp_r31], lbl_1_data_488[temp_r31][1]);
        espPosSet(lbl_1_bss_100A[temp_r31][0], lbl_1_data_428[temp_r31][0] + (f32) lbl_1_bss_1140[temp_r31], lbl_1_data_428[temp_r31][1]);
        espPosSet(lbl_1_bss_100A[temp_r31][1], lbl_1_data_428[temp_r31][0] + (f32) lbl_1_bss_1140[temp_r31], lbl_1_data_428[temp_r31][1]);
        espPosSet(lbl_1_bss_100A[temp_r31][2], lbl_1_data_428[temp_r31][0] + (f32) lbl_1_bss_1140[temp_r31], lbl_1_data_428[temp_r31][1]);
        espPosSet(lbl_1_bss_100A[temp_r31][3], lbl_1_data_428[temp_r31][0] + (f32) lbl_1_bss_1140[temp_r31], lbl_1_data_428[temp_r31][1]);
        espPosSet(lbl_1_bss_FEA[temp_r31], lbl_1_data_4A8[temp_r31][0] + (f32) lbl_1_bss_1140[temp_r31], lbl_1_data_4A8[temp_r31][1]);
        espPosSet(lbl_1_bss_FE2[temp_r31], lbl_1_data_4C8[temp_r31][0] + (f32) lbl_1_bss_1140[temp_r31], lbl_1_data_4C8[temp_r31][1]);
        espPosSet(lbl_1_bss_FDA[temp_r31], lbl_1_data_4E8[temp_r31][0] + (f32) lbl_1_bss_1140[temp_r31], lbl_1_data_4E8[temp_r31][1]);
        espPosSet(lbl_1_bss_FD2[temp_r31], lbl_1_data_508[temp_r31][0] + (f32) lbl_1_bss_1140[temp_r31], lbl_1_data_508[temp_r31][1]);
        espPosSet(lbl_1_bss_FCA[temp_r31], lbl_1_data_528[temp_r31][0] + (f32) lbl_1_bss_1140[temp_r31], lbl_1_data_528[temp_r31][1]);
        espPosSet(lbl_1_bss_FC2[temp_r31], lbl_1_data_548[temp_r31][0] + (f32) lbl_1_bss_1140[temp_r31], lbl_1_data_548[temp_r31][1]);
        espPosSet(lbl_1_bss_FBA[temp_r31], lbl_1_data_568[temp_r31][0] + (f32) lbl_1_bss_1140[temp_r31], lbl_1_data_568[temp_r31][1]);
        espPosSet(lbl_1_bss_FB2[temp_r31], lbl_1_data_588[temp_r31][0] + (f32) lbl_1_bss_1140[temp_r31], lbl_1_data_588[temp_r31][1]);
        espPosSet(lbl_1_bss_FAA[temp_r31], lbl_1_data_5A8[temp_r31][0] + (f32) lbl_1_bss_1140[temp_r31], lbl_1_data_5A8[temp_r31][1]);
        if (lbl_1_bss_20[0] == 0) {
                        if ((lbl_1_data_428[temp_r31][0] + (f32) lbl_1_bss_1140[temp_r31]) <= 576.0f) {
                if ((s16) GwPlayerConf[arg0->order[temp_r31]].charNo == 0) {
                    lbl_1_bss_20[1] = 0x1F8;
                }
                if ((s16) GwPlayerConf[arg0->order[temp_r31]].charNo == 1) {
                    lbl_1_bss_20[1] = 0x1E4;
                }
                if ((s16) GwPlayerConf[arg0->order[temp_r31]].charNo == 2) {
                    lbl_1_bss_20[1] = 0x25C;
                }
                if ((s16) GwPlayerConf[arg0->order[temp_r31]].charNo == 3) {
                    lbl_1_bss_20[1] = 0x2AC;
                }
                if ((s16) GwPlayerConf[arg0->order[temp_r31]].charNo == 4) {
                    lbl_1_bss_20[1] = 0x284;
                }
                if ((s16) GwPlayerConf[arg0->order[temp_r31]].charNo == 5) {
                    lbl_1_bss_20[1] = 0x1BC;
                }
                if ((s16) GwPlayerConf[arg0->order[temp_r31]].charNo == 6) {
                    lbl_1_bss_20[1] = 0x298;
                }
                fn_1_8D04(lbl_1_bss_20[1], lbl_1_bss_1126[3], 1);
                lbl_1_bss_20[1] = 0;
                lbl_1_bss_20[0] = 1;
            }
        }
        if (lbl_1_bss_1140[temp_r31] == 0) {
            if (temp_r31 == 3) {
                lbl_1_bss_1168[0] = 0;
                var_r29 = 1;
                for (var_r31_3 = 0; var_r31_3 < 4; var_r31_3++) {
                    Hu3DLLightKill(lbl_1_bss_1116[var_r31_3], lbl_1_bss_0[var_r31_3]);
                    lbl_1_bss_0[var_r31_3] = -1;
                    Hu3DModelShadowMapObjReset(lbl_1_bss_1126[var_r31_3]);
                    Hu3DModelShadowReset(lbl_1_bss_111E[var_r31_3]);
                    Hu3DModelShadowReset(lbl_1_bss_1116[var_r31_3]);
                    Hu3DModelKill(lbl_1_bss_111E[var_r31_3]);
                    Hu3DModelKill(lbl_1_bss_1116[var_r31_3]);
                    Hu3DModelKill(lbl_1_bss_1126[var_r31_3]);
                }
            } else {
                lbl_1_bss_1168[0] -= 1;
            }
            lbl_1_bss_20[0] = 0;
        }
        break;
    }
    return var_r29;
}

s16 lbl_1_bss_F92[2];

s32 fn_1_13728(CardGame *arg0)
{
    Vec posA;
    Vec posB;
    Vec posC;
    Vec cur;
    Vec p2d;
    Vec v;
    Vec v2;
    f32 sz[2];
    f32 sz2[2];
    s32 ids[4];
    s32 keys[4];
    f32 temp_f31;
    f32 var_f31;
    s32 var_r26;
    s32 var_r27;
    s32 var_r28;
    s32 var_r30;
    s32 var_r31;
    s32 var_r31_2;
    s32 var_r31_3;
    s32 var_r31_4;
    s32 var_r31_5;
    s32 var_r31_6;
    s32 var_r31_7;
    s32 tmp;

    var_r26 = 0;
    posA.x = 288.0f;
    posA.y = 240.0f;
    posA.z = 1800.0f;
    posB.x = 68.0f;
    posB.y = 220.0f;
    posB.z = 1800.0f;
    posC.x = 508.0f;
    posC.y = 220.0f;
    posC.z = 1800.0f;
    switch (lbl_1_bss_1168[1]) {                  /* switch 1 */
    case 0:                                         /* switch 1 */
        var_r28 = 0;
        sz[0] = 352.0f;
        sz[1] = 352.0f;
        switch (lbl_1_bss_1168[0]) {        /* switch 2; irregular */
        case 0:                                     /* switch 2 */
            lbl_1_bss_112E = espEntry(lbl_1_data_2B4[0xC], 0, 0);
            espDrawNoSet(lbl_1_bss_112E, 0x40);
            lbl_1_bss_F3C[0] = 0x12C;
            espPosSet(lbl_1_bss_112E, (288.0f - (sz[0] * 0.5f)) + (sz[0] * 0.5f), 78.0f - (f32) lbl_1_bss_F3C[0]);
            lbl_1_bss_1168[0] += 1;
            HuSprExecLayerSet(0x40, 6);
            break;
        case 1:                                     /* switch 2 */
            lbl_1_bss_F3C[0] -= 0xA;
            espPosSet(lbl_1_bss_112E, (288.0f - (sz[0] * 0.5f)) + (sz[0] * 0.5f), 78.0f - (f32) lbl_1_bss_F3C[0]);
            if (lbl_1_bss_F3C[0] <= 0) {
                lbl_1_bss_F3C[0] = 0;
                lbl_1_bss_1168[0] += 1;
                var_r28 = 1;
            }
            break;
        case 2:                                     /* switch 2 */
            lbl_1_bss_F3C[0] += 0xA;
            espPosSet(lbl_1_bss_112E, (288.0f - (sz[0] * 0.5f)) + (sz[0] * 0.5f), 78.0f - (f32) lbl_1_bss_F3C[0]);
            if (lbl_1_bss_F3C[0] >= 0x12C) {
                espKill(lbl_1_bss_112E);
                lbl_1_bss_F3C[0] = 0;
                lbl_1_bss_1168[0] = 0;
                var_r28 = 1;
            }
            break;
        }
        if (var_r28 != 0) {
            lbl_1_bss_1168[1] = (lbl_1_bss_1168[1] + 1);
        }
        break;
    case 1:                                         /* switch 1 */
        for (var_r31 = 0; var_r31 < 4; var_r31++) {
            ids[var_r31] = var_r31;
            keys[var_r31] = frandmod(0x64);
        }
        for (var_r31_2 = 0; var_r31_2 < 3; var_r31_2++) {
            var_r30 = var_r31_2 + 1;
            while (var_r30 < 4) {
                if (keys[var_r31_2] > keys[var_r30]) {
                    tmp = ids[var_r30];
                    ids[var_r30] = ids[var_r31_2];
                    ids[var_r31_2] = tmp;
                    tmp = keys[var_r30];
                    keys[var_r30] = keys[var_r31_2];
                    keys[var_r31_2] = tmp;
                }
                var_r30 += 1;
            }
        }
        for (var_r31_3 = 0; var_r31_3 < 4; var_r31_3++) {
            lbl_1_bss_F64[var_r31_3] = ids[var_r31_3];
            lbl_1_bss_1126[var_r31_3] = Hu3DModelCreateData(lbl_1_data_B0[lbl_1_bss_F64[var_r31_3] + 0x57]);
            Hu3DModelCameraSet(lbl_1_bss_1126[var_r31_3], 1);
            lbl_1_bss_F14[0] = 3000.0f;
            lbl_1_bss_F14[8] = 1800.0f;
            Hu3DModelPosSet(lbl_1_bss_1126[var_r31_3], -500.0f + (300.0f * (f32) var_r31_3) + lbl_1_bss_F14[0], 0.0f, 400.0f);
            Hu3DModelRotSet(lbl_1_bss_1126[var_r31_3], 0.0f, 0.0f, 180.0f);
            Hu3DModelShadowMapSet(lbl_1_bss_1126[var_r31_3]);
            lbl_1_bss_111E[var_r31_3] = Hu3DModelCreateData(lbl_1_data_B0[GwPlayerConf[var_r31_3].charNo + 0x50]);
            Hu3DModelCameraSet(lbl_1_bss_111E[var_r31_3], 1);
            Hu3DModelPosSet(lbl_1_bss_111E[var_r31_3], (-500.0f + (300.0f * (f32) var_r31_3)) - 80.0f, 50.0f, 480.0f);
            Hu3DModelRotSet(lbl_1_bss_111E[var_r31_3], 0.0f, 0.0f, 0.0f);
            Hu3DModelAttrSet(lbl_1_bss_111E[var_r31_3], 1);
            Hu3DModelShadowSet(lbl_1_bss_111E[var_r31_3]);
            lbl_1_bss_1116[var_r31_3] = Hu3DModelCreateData(arg0->pl[var_r31_3].f28 + lbl_1_data_B0[(GwPlayerConf[var_r31_3].charNo * 3) + 0x24]);
            Hu3DModelCameraSet(lbl_1_bss_1116[var_r31_3], 1);
            temp_f31 = lbl_1_bss_F14[0] + (1200.0f * (f32) var_r31_3);
            Hu3DModelPosSet(lbl_1_bss_1116[var_r31_3], -275.0f + (150.0f * (f32) var_r31_3), temp_f31, -200.0f);
            Hu3DModelRotSet(lbl_1_bss_1116[var_r31_3], 0.0f, lbl_1_bss_F14[8] + (360.0f * (f32) var_r31_3), 0.0f);
            Hu3DModelShadowSet(lbl_1_bss_1116[var_r31_3]);
            Hu3DModelShadowDispOff(lbl_1_bss_1116[var_r31_3]);
            lbl_1_bss_0[var_r31_3] = Hu3DLLightCreate(lbl_1_bss_1116[var_r31_3], -275.0f + (150.0f * (f32) var_r31_3), 80.0f + temp_f31, 500.0f, 0.0f, 0.0f, -1.0f, 0x60, 0x60, 0x60);
            Hu3DLLightStaticSet(lbl_1_bss_1116[var_r31_3], lbl_1_bss_0[var_r31_3], 1);
            Hu3DLLightInfinitytSet(lbl_1_bss_1116[var_r31_3], lbl_1_bss_0[var_r31_3]);
        }
        lbl_1_bss_1168[1] = (lbl_1_bss_1168[1] + 1);
        break;
    case 2:                                         /* switch 1 */
        lbl_1_bss_F14[0] -= 100.0f;
        lbl_1_bss_F14[8] -= 60.0f;
        for (var_r31_4 = 0; var_r31_4 < 4; var_r31_4++) {
                        if (lbl_1_bss_F14[0] >= 0.0f) {
                Hu3DModelPosSet(lbl_1_bss_1126[var_r31_4], -500.0f + (300.0f * (f32) var_r31_4) + lbl_1_bss_F14[0], 0.0f, 400.0f);
            }
            var_f31 = lbl_1_bss_F14[0] + (1200.0f * (f32) var_r31_4);
                        if (var_f31 >= 0.0f) {
                Hu3DModelRotSet(lbl_1_bss_1116[var_r31_4], 0.0f, lbl_1_bss_F14[8] + (360.0f * (f32) var_r31_4), 0.0f);
            }
            if (var_f31 < 0.0f) {
                var_f31 = 0.0f;
            }
            Hu3DModelPosSet(lbl_1_bss_1116[var_r31_4], -275.0f + (150.0f * (f32) var_r31_4), var_f31, -200.0f);
                        if (var_f31 <= 300.0f) {
                Hu3DModelShadowDispOn(lbl_1_bss_1116[var_r31_4]);
            }
            if ((s16) lbl_1_bss_0[var_r31_4] != -1) {
                Hu3DLLightPosSet(lbl_1_bss_1116[var_r31_4], lbl_1_bss_0[var_r31_4], -275.0f + (150.0f * (f32) var_r31_4), 80.0f + var_f31, 500.0f, 0.0f, 0.0f, -1.0f);
            }
        }
        cur.x = -500.0f + lbl_1_bss_F14[0];
        cur.y = 0.0f;
        cur.z = 400.0f;
        Hu3D3Dto2D(&cur, 1, &p2d);
        if (lbl_1_bss_20[0] == 0) {
                        if (p2d.x <= 576.0f) {
                fn_1_8D04(0x3FD, lbl_1_bss_1126[0], 1);
                lbl_1_bss_20[0] = 1;
            }
        }
                if (lbl_1_bss_F14[0] <= -4800.0f) {
            lbl_1_bss_F14[0] = 0.0f;
            lbl_1_bss_1168[1] = 0xA;
            lbl_1_bss_F3C[0] = 0;
            lbl_1_bss_F3C[1] = 0;
            lbl_1_bss_F3C[2] = 0;
            lbl_1_bss_F3C[3] = 0;
            fn_1_36828();
            lbl_1_bss_F14[5] = 300.0f;
            posA.y -= lbl_1_bss_F14[5];
            fn_1_36A70(&posA);
            lbl_1_bss_20[0] = 0;
        }
        break;
    case 3:                                         /* switch 1 */
        if (lbl_1_bss_C44 != 0) {
            if ((s16) GwPlayerConf[lbl_1_bss_F3C[1]].type != 0) {
                for (var_r31_5 = 0; var_r31_5 < 0xA; var_r31_5++) {
                    HuWinComKeyWait(0x100, 0x100, 0x100, 0x100, GWComKeyDelayGet());
                }
            }
            ((s32 *) lbl_1_bss_12A8)[10] = 0x3C;
            ((s32 *) lbl_1_bss_12A8)[2] = GwPlayerConf[lbl_1_bss_F3C[1]].charNo;
            fn_1_4FBC8(arg0, 2);
            lbl_1_bss_C44 = 0;
            Hu3DModelAttrReset(lbl_1_bss_111E[lbl_1_bss_F3C[1]], 1);
            fn_1_36B10(0);
        }
        fn_1_18054(lbl_1_bss_F3C[1], lbl_1_bss_F3C[0]);
        if ((s16) GwPlayerConf[lbl_1_bss_F3C[1]].type == 0) {
            fn_1_502E0(0x13);
        }
        if ((HuPadDStkRep[lbl_1_bss_F3C[1]] & 2) != 0) {
            do {
                lbl_1_bss_F3C[0] += 1;
                if (lbl_1_bss_F3C[0] > 3) {
                    lbl_1_bss_F3C[0] = 0;
                }
            } while (lbl_1_bss_F64[lbl_1_bss_F3C[0]] == -1);
            if (lbl_1_bss_F3C[1] < 3) {
                HuAudFXPlay(0);
            }
        } else if ((HuPadDStkRep[lbl_1_bss_F3C[1]] & 1) != 0) {
            do {
                lbl_1_bss_F3C[0] -= 1;
                if (lbl_1_bss_F3C[0] < 0) {
                    lbl_1_bss_F3C[0] = 3;
                }
            } while (lbl_1_bss_F64[lbl_1_bss_F3C[0]] == -1);
            if (lbl_1_bss_F3C[1] < 3) {
                HuAudFXPlay(0);
            }
        }
        for (var_r31_6 = 0; var_r31_6 < 4; var_r31_6++) {
            Hu3DModelPosSet(lbl_1_bss_111E[var_r31_6], (-500.0f + (300.0f * (f32) lbl_1_bss_F3C[0])) - 80.0f, 50.0f, 480.0f);
        }
        if ((HuPadBtnDown[lbl_1_bss_F3C[1]] & 0x100) != 0) {
            fn_1_50460();
            if (lbl_1_bss_F64[lbl_1_bss_F3C[0]] != -1) {
                lbl_1_bss_E9C->x = -275.0f + (150.0f * (f32) lbl_1_bss_F3C[1]);
                lbl_1_bss_E9C->y = 0.0f;
                lbl_1_bss_E9C->z = -200.0f;
                lbl_1_bss_E9C[1].x = -500.0f + (300.0f * (f32) lbl_1_bss_F3C[0]);
                lbl_1_bss_E9C[1].y = 0.0f;
                lbl_1_bss_E9C[1].z = 200.0f;
                lbl_1_bss_F14[8] = 0.0f;
                lbl_1_bss_F14[9] = 1.0f;
                lbl_1_bss_F3C[8] = 0;
                lbl_1_bss_F3C[9] = 0;
                lbl_1_bss_1168[1] = 6;
                HuAudFXPlay(2);
            }
        }
        break;
    case 4:                                         /* switch 1 */
        var_r27 = 0;
        sz2[0] = 352.0f;
        sz2[1] = 352.0f;
        switch (lbl_1_bss_1168[0]) {        /* switch 3; irregular */
        case 0:                                     /* switch 3 */
            lbl_1_bss_112E = espEntry(lbl_1_data_2B4[0xC], 0, 0);
            espDrawNoSet(lbl_1_bss_112E, 0x40);
            lbl_1_bss_F3C[0] = 0x12C;
            espPosSet(lbl_1_bss_112E, (288.0f - (sz2[0] * 0.5f)) + (sz2[0] * 0.5f), 78.0f - (f32) lbl_1_bss_F3C[0]);
            lbl_1_bss_1168[0] += 1;
            HuSprExecLayerSet(0x40, 6);
            break;
        case 1:                                     /* switch 3 */
            lbl_1_bss_F3C[0] -= 0xA;
            espPosSet(lbl_1_bss_112E, (288.0f - (sz2[0] * 0.5f)) + (sz2[0] * 0.5f), 78.0f - (f32) lbl_1_bss_F3C[0]);
            if (lbl_1_bss_F3C[0] <= 0) {
                lbl_1_bss_F3C[0] = 0;
                lbl_1_bss_1168[0] += 1;
                var_r27 = 1;
            }
            break;
        case 2:                                     /* switch 3 */
            lbl_1_bss_F3C[0] += 0xA;
            espPosSet(lbl_1_bss_112E, (288.0f - (sz2[0] * 0.5f)) + (sz2[0] * 0.5f), 78.0f - (f32) lbl_1_bss_F3C[0]);
            if (lbl_1_bss_F3C[0] >= 0x12C) {
                espKill(lbl_1_bss_112E);
                lbl_1_bss_F3C[0] = 0;
                lbl_1_bss_1168[0] = 0;
                var_r27 = 1;
            }
            break;
        }
        if (var_r27 != 0) {
            lbl_1_bss_F14[0] = 0.0f;
            lbl_1_bss_1168[1] = (lbl_1_bss_1168[1] + 1);
            fn_1_8D04(0x3F6, lbl_1_bss_1126[2], 1);
        }
        break;
    case 5:                                         /* switch 1 */
        lbl_1_bss_F14[0] += 20.0f;
                if (lbl_1_bss_F14[0] <= 180.0f) {
            Hu3DModelRotSet(lbl_1_bss_1126[0], 0.0f, 0.0f, 180.0f + lbl_1_bss_F14[0]);
            Hu3DModelRotSet(lbl_1_bss_1126[1], 0.0f, 0.0f, 180.0f - lbl_1_bss_F14[0]);
            Hu3DModelRotSet(lbl_1_bss_1126[2], 0.0f, 0.0f, 180.0f + lbl_1_bss_F14[0]);
            Hu3DModelRotSet(lbl_1_bss_1126[3], 0.0f, 0.0f, 180.0f - lbl_1_bss_F14[0]);
        }
                if (lbl_1_bss_F14[0] >= 1140.0f) {
            lbl_1_bss_F14[0] = 0.0f;
            lbl_1_bss_F14[1] = 0.0f;
            lbl_1_bss_F14[2] = 0.0f;
            lbl_1_bss_1168[1] = 0;
            var_r26 = 1;
            fn_1_36B10(1);
            ((s32 *) lbl_1_bss_12A8)[14] = 0;
            ((s32 *) lbl_1_bss_12A8)[10] = 0x3C;
            fn_1_4FBC8(arg0, 3);
        }
        break;
    case 6:
        if (lbl_1_bss_F3C[9] == 0) {
            if (lbl_1_bss_F14[8] == 0.0f) {
                v = lbl_1_bss_E9C[0];
                lbl_1_bss_F92[0] = fn_1_3ED3C(8, &v);
                fn_1_8D04(0x3FF, lbl_1_bss_1116[lbl_1_bss_F3C[1]], 1);
            }
            lbl_1_bss_F14[8] += 72.0f;
            lbl_1_bss_F14[9] -= 0.05f;
            if (lbl_1_bss_F14[9] <= 0.0f) {
                lbl_1_bss_F14[8] = 0.0f;
                lbl_1_bss_F14[9] = 0.0f;
                fn_1_3F278(8, lbl_1_bss_F92[0]);
                Hu3DModelPosSet(lbl_1_bss_1116[lbl_1_bss_F3C[1]], lbl_1_bss_E9C[1].x, 0.0f, lbl_1_bss_E9C[1].z);
                lbl_1_bss_F3C[9] += 1;
            }
            Hu3DModelRotSet(lbl_1_bss_1116[lbl_1_bss_F3C[1]], 0.0f, lbl_1_bss_F14[8], 0.0f);
            Hu3DModelScaleSet(lbl_1_bss_1116[lbl_1_bss_F3C[1]], lbl_1_bss_F14[9], 1.0f, lbl_1_bss_F14[9]);
        } else if (lbl_1_bss_F3C[9] == 1) {
            if (lbl_1_bss_F14[8] == 0.0f) {
                v = lbl_1_bss_E9C[1];
                lbl_1_bss_F92[0] = fn_1_3ED3C(8, &v);
                Hu3DModelAttrSet(lbl_1_bss_111E[lbl_1_bss_F3C[1]], 1);
                fn_1_8D04(0x400, lbl_1_bss_1116[lbl_1_bss_F3C[1]], 1);
            }
            lbl_1_bss_F14[8] += 72.0f;
            lbl_1_bss_F14[9] += 0.05f;
            if (lbl_1_bss_F14[9] >= 1.0f) {
                lbl_1_bss_F14[8] = 0.0f;
                lbl_1_bss_F14[9] = 1.0f;
                fn_1_3F278(8, lbl_1_bss_F92[0]);
                lbl_1_bss_1168[1] += 1;
            }
            Hu3DModelRotSet(lbl_1_bss_1116[lbl_1_bss_F3C[1]], 0.0f, lbl_1_bss_F14[8], 0.0f);
            Hu3DModelScaleSet(lbl_1_bss_1116[lbl_1_bss_F3C[1]], lbl_1_bss_F14[9], 1.0f, lbl_1_bss_F14[9]);
            if (lbl_1_bss_0[lbl_1_bss_F3C[1]] != -1) {
                Hu3DLLightPosSet(lbl_1_bss_1116[lbl_1_bss_F3C[1]], lbl_1_bss_0[lbl_1_bss_F3C[1]], lbl_1_bss_E9C[1].x, 80.0f,
                    100.0f + lbl_1_bss_E9C[1].z, 0.0f, 0.0f, -1.0f);
            }
        }
        break;
    case 7:
        arg0->order[lbl_1_bss_F64[lbl_1_bss_F3C[0]]] = lbl_1_bss_F3C[1];
        lbl_1_bss_F64[lbl_1_bss_F64[lbl_1_bss_F3C[0]] + 4] = lbl_1_bss_F3C[0];
        lbl_1_bss_F64[lbl_1_bss_F3C[0]] = -1;
        lbl_1_bss_F3C[1] += 1;
        lbl_1_bss_C44 = 1;
        if (lbl_1_bss_F3C[1] == 4) {
            lbl_1_bss_1168[1] = 0xB;
            posA.y -= 80.0f;
            v = posA;
            v2 = posC;
            fn_1_4F144(0, &v2, &v, 0x3C, 80.0f, 0x315);
        } else {
            do {
                lbl_1_bss_F3C[0] += 1;
                if (lbl_1_bss_F3C[0] > 3) {
                    lbl_1_bss_F3C[0] = 0;
                }
            } while (lbl_1_bss_F64[lbl_1_bss_F3C[0]] == -1);
            for (var_r31_7 = 0; var_r31_7 < 4; var_r31_7++) {
                Hu3DModelPosSet(lbl_1_bss_111E[var_r31_7], (-500.0f + (300.0f * (f32) lbl_1_bss_F3C[0])) - 80.0f, 50.0f, 480.0f);
            }
            lbl_1_bss_1168[1] = 0xB;
            if ((lbl_1_bss_F3C[1] % 2) != 0) {
                v = posC;
                v2 = posB;
                fn_1_4F144(0, &v2, &v, 0x3C, 100.0f, 0x315);
            } else {
                v = posB;
                v2 = posC;
                fn_1_4F144(0, &v2, &v, 0x3C, 100.0f, 0x315);
            }
        }
        break;
    case 10:
        lbl_1_bss_F14[5] -= 10.0f;
        if (lbl_1_bss_F14[5] < 0.0f) {
            lbl_1_bss_F14[5] = 0.0f;
        }
        posA.y -= lbl_1_bss_F14[5];
        fn_1_36A70(&posA);
        if (lbl_1_bss_F14[5] == 0.0f) {
            fn_1_36B10(1);
            ((s32 *) lbl_1_bss_12A8)[10] = 0x3C;
            fn_1_4FBC8(arg0, 1);
            fn_1_36B10(0);
            lbl_1_bss_1168[1] = 0xB;
            v = posB;
            v2 = posA;
            fn_1_4F144(0, &v2, &v, 0x3C, 80.0f, 0x315);
            lbl_1_bss_F14[6] = 0.0f;
        }
        break;
    case 11:
        if ((lbl_1_bss_F3C[1] % 2) != 0) {
            lbl_1_bss_F14[6] -= 0.5f;
        } else if (lbl_1_bss_F3C[1] == 0 || lbl_1_bss_F3C[1] == 4) {
            lbl_1_bss_F14[6] += 0.25f;
        } else {
            lbl_1_bss_F14[6] += 0.5f;
        }
        Hu3DModelRotSet(fn_1_36A60(), 0.0f, 0.0f, lbl_1_bss_F14[6]);
        if (fn_1_4F1EC(0, &cur) != 0) {
            if (lbl_1_bss_F3C[1] == 4) {
                lbl_1_bss_1168[1] = 4;
            } else {
                lbl_1_bss_1168[1] = 3;
                fn_1_36B10(1);
            }
        }
        fn_1_36A70(&cur);
        break;
    }
    return var_r26;
}

s16 lbl_1_bss_F8E[2];
Vec lbl_1_bss_E68[3];

s32 fn_1_2E548(CardGame *arg0)
{
    Vec v;
    Vec sp50;
    CardPlayer *rec;
    f32 sp44;
    f32 sp4C;
    f32 var_f30;
    f32 var_f31;
    s32 var_r25;
    s32 var_r26;
    s32 var_r27;
    s32 var_r28;
    s32 var_r28_2;
    s32 var_r29;
    s32 var_r29_2;

    var_r25 = 0;
    rec = &arg0->pl[arg0->order[arg0->curPlayer]];
    fn_1_9C74(&sp50, rec->row, rec->col, 1, 1);
    switch (lbl_1_bss_1168[1]) {                  /* switch 1 */
    case 0:                                         /* switch 1 */
        var_r29 = rec->row;
        var_r28 = rec->col;
        var_r27 = rec->f20;
        var_r26 = rec->f24;
        if (rec->rotY == 180.0f) {
            var_r29 += 1;
            if ((lbl_1_bss_8C4[var_r29][var_r28] & 0x40) != 0) {
                var_r27 = 1;
                lbl_1_bss_F14[8] = 300.0f;
                lbl_1_bss_F14[9] = 201.5625f;
            } else {
                var_r27 = 0;
                lbl_1_bss_F14[8] = 201.5625f;
                lbl_1_bss_F14[9] = 103.125f;
            }
        } else if (rec->rotY == 0.0f) {
            var_r29 -= 1;
            if ((lbl_1_bss_8C4[var_r29][var_r28] & 0x40) != 0) {
                var_r27 = 1;
                lbl_1_bss_F14[8] = 300.0f;
                lbl_1_bss_F14[9] = 201.5625f;
            } else {
                var_r27 = 2;
                lbl_1_bss_F14[8] = 201.5625f;
                lbl_1_bss_F14[9] = 103.125f;
            }
        } else if (rec->rotY == -90.0f) {
            var_r28 += 1;
            if ((lbl_1_bss_8C4[var_r29][var_r28] & 0x40) != 0) {
                var_r26 = 1;
                lbl_1_bss_F14[8] = 300.0f;
                lbl_1_bss_F14[9] = 201.5625f;
            } else {
                var_r26 = 0;
                lbl_1_bss_F14[8] = 201.5625f;
                lbl_1_bss_F14[9] = 103.125f;
            }
        } else if (rec->rotY == 90.0f) {
            var_r28 -= 1;
            if ((lbl_1_bss_8C4[var_r29][var_r28] & 0x40) != 0) {
                var_r26 = 1;
                lbl_1_bss_F14[8] = 300.0f;
                lbl_1_bss_F14[9] = 201.5625f;
            } else {
                var_r26 = 2;
                lbl_1_bss_F14[8] = 201.5625f;
                lbl_1_bss_F14[9] = 103.125f;
            }
        }
        fn_1_9C74(&lbl_1_bss_E68[0], var_r29, var_r28, var_r27, var_r26);
        var_r29_2 = rec->f08;
        var_r28_2 = rec->f0C;
        if (rec->f04 == 1) {
            var_r29_2 -= 1;
        } else if (rec->f04 == 2) {
            var_r29_2 += 1;
        } else if (rec->f04 == 3) {
            var_r28_2 -= 1;
        } else if (rec->f04 == 4) {
            var_r28_2 += 1;
        }
        fn_1_9C74(&lbl_1_bss_E68[1], var_r29_2, var_r28_2, 1, 1);
        lbl_1_bss_F3C[0] = 0;
        lbl_1_bss_1168[1] += 1;
        break;
    case 1:                                         /* switch 1 */
        if (lbl_1_bss_F3C[0] == 0) {
            fn_1_9C74(&lbl_1_bss_E9C[0], rec->row, rec->col, 1, 1);
            lbl_1_bss_E9C[0].x += -50.0f;
            v = lbl_1_bss_E9C[0];
            lbl_1_bss_F8E[0] = fn_1_3ED3C(5, &v);
            fn_1_8D04(0x414, lbl_1_bss_478[rec->row][rec->col], 1);
        }
        lbl_1_bss_F3C[0] += 1;
        if (lbl_1_bss_F3C[0] == 0x64) {
            if ((rec->f50 & 2) != 0) {
                fn_1_3F278(3, rec->f56);
                rec->f56 = -1;
                rec->f50 &= 0xFFFFFFFD;
            }
            if ((rec->f50 & 4) != 0) {
                fn_1_3F278(4, rec->f56);
                rec->f56 = -1;
                rec->f50 &= 0xFFFFFFFB;
            }
            v = lbl_1_bss_E9C[0];
            lbl_1_bss_F8E[1] = fn_1_3ED3C(7, &v);
            lbl_1_bss_F14[0] = 0.0f;
            lbl_1_bss_F14[1] = 0.0f;
            lbl_1_bss_F14[2] = 0.0f;
            lbl_1_bss_1168[1] += 1;
            omVibrate((s16) arg0->order[arg0->curPlayer], 0xB4, 6, 6);
            fn_1_8D04(0x415, lbl_1_bss_478[rec->row][rec->col], 1);
            if ((s16) GwPlayerConf[arg0->order[arg0->curPlayer]].charNo == 0) {
                lbl_1_bss_20[0] = 0x1F9;
            }
            if ((s16) GwPlayerConf[arg0->order[arg0->curPlayer]].charNo == 1) {
                lbl_1_bss_20[0] = 0x1E5;
            }
            if ((s16) GwPlayerConf[arg0->order[arg0->curPlayer]].charNo == 2) {
                lbl_1_bss_20[0] = 0x25D;
            }
            if ((s16) GwPlayerConf[arg0->order[arg0->curPlayer]].charNo == 3) {
                lbl_1_bss_20[0] = 0x2AD;
            }
            if ((s16) GwPlayerConf[arg0->order[arg0->curPlayer]].charNo == 4) {
                lbl_1_bss_20[0] = 0x285;
            }
            if ((s16) GwPlayerConf[arg0->order[arg0->curPlayer]].charNo == 5) {
                lbl_1_bss_20[0] = 0x1BD;
            }
            if ((s16) GwPlayerConf[arg0->order[arg0->curPlayer]].charNo == 6) {
                lbl_1_bss_20[0] = 0x299;
            }
            fn_1_8D04(lbl_1_bss_20[0], lbl_1_bss_478[rec->row][rec->col], 1);
            lbl_1_bss_20[0] = 0;
        }
        break;
    case 2:                                         /* switch 1 */
        sp44 = 0.1f * (f32) (frandmod(0x14) + 1);
        if (frandmod(2) != 0) {
            var_f31 = sp44;
        } else {
            var_f31 = -sp44;
        }
        sp44 = var_f31;
        sp4C = 0.1f * (f32) (frandmod(0x14) + 1);
        if (frandmod(2) != 0) {
            var_f30 = sp4C;
        } else {
            var_f30 = -sp4C;
        }
        sp4C = var_f30;
        if (rec->rotY == 180.0f) {
            lbl_1_bss_F14[0] -= lbl_1_bss_F14[8] / 66.66667f;
            if (lbl_1_bss_F14[0] < -lbl_1_bss_F14[8]) {
                lbl_1_bss_F14[0] = -lbl_1_bss_F14[8];
            }
            Hu3DModelPosSet(lbl_1_bss_468[arg0->order[arg0->curPlayer]], -50.0f + lbl_1_bss_E68[0].x + sp44, lbl_1_bss_E68[0].y, sp4C + (lbl_1_bss_E68[0].z + lbl_1_bss_F14[0]));
            if (lbl_1_bss_F14[0] < -lbl_1_bss_F14[9]) {
                lbl_1_bss_F14[1] += 5.0f;
                lbl_1_bss_F14[2] += 0.015f;
            }
        } else if (rec->rotY == 0.0f) {
            lbl_1_bss_F14[0] += lbl_1_bss_F14[8] / 66.66667f;
            if (lbl_1_bss_F14[0] > lbl_1_bss_F14[8]) {
                lbl_1_bss_F14[0] = lbl_1_bss_F14[8];
            }
            Hu3DModelPosSet(lbl_1_bss_468[arg0->order[arg0->curPlayer]], -50.0f + lbl_1_bss_E68[0].x + sp44, lbl_1_bss_E68[0].y, sp4C + (lbl_1_bss_E68[0].z + lbl_1_bss_F14[0]));
            if (lbl_1_bss_F14[0] > lbl_1_bss_F14[9]) {
                lbl_1_bss_F14[1] += 5.0f;
                lbl_1_bss_F14[2] += 0.015f;
            }
        } else if (rec->rotY == -90.0f) {
            lbl_1_bss_F14[0] -= lbl_1_bss_F14[8] / 66.66667f;
            if (lbl_1_bss_F14[0] < -lbl_1_bss_F14[8]) {
                lbl_1_bss_F14[0] = -lbl_1_bss_F14[8];
            }
            Hu3DModelPosSet(lbl_1_bss_468[arg0->order[arg0->curPlayer]], sp44 + (-50.0f + lbl_1_bss_E68[0].x + lbl_1_bss_F14[0]), lbl_1_bss_E68[0].y, lbl_1_bss_E68[0].z + sp4C);
            if (lbl_1_bss_F14[0] < -lbl_1_bss_F14[9]) {
                lbl_1_bss_F14[1] += 5.0f;
                lbl_1_bss_F14[2] += 0.015f;
            }
        } else if (rec->rotY == 90.0f) {
            lbl_1_bss_F14[0] += lbl_1_bss_F14[8] / 66.66667f;
            if (lbl_1_bss_F14[0] > lbl_1_bss_F14[8]) {
                lbl_1_bss_F14[0] = lbl_1_bss_F14[8];
            }
            Hu3DModelPosSet(lbl_1_bss_468[arg0->order[arg0->curPlayer]], sp44 + (-50.0f + lbl_1_bss_E68[0].x + lbl_1_bss_F14[0]), lbl_1_bss_E68[0].y, lbl_1_bss_E68[0].z + sp4C);
            if (lbl_1_bss_F14[0] > lbl_1_bss_F14[9]) {
                lbl_1_bss_F14[1] += 5.0f;
                lbl_1_bss_F14[2] += 0.015f;
            }
        }
                if (lbl_1_bss_F14[2] >= 1.0f) {
            lbl_1_bss_F14[2] = 1.0f;
            lbl_1_bss_F3C[0] = 0;
            lbl_1_bss_1168[1] += 1;
        }
        Hu3DModelRotSet(lbl_1_bss_468[arg0->order[arg0->curPlayer]], lbl_1_bss_F14[1], rec->rotY + lbl_1_bss_F14[1], lbl_1_bss_F14[1]);
        Hu3DModelScaleSet(lbl_1_bss_468[arg0->order[arg0->curPlayer]], 1.0f - lbl_1_bss_F14[2], 1.0f - lbl_1_bss_F14[2], 1.0f - lbl_1_bss_F14[2]);
        break;
    case 3:                                         /* switch 1 */
        if (lbl_1_bss_F3C[0] == 0) {
            fn_1_3F278(5, lbl_1_bss_F8E[0]);
            fn_1_9C74(&lbl_1_bss_E9C[0], rec->row, rec->col, 1, 1);
            lbl_1_bss_E9C[0].x += -50.0f;
            v = lbl_1_bss_E9C[0];
            lbl_1_bss_F8E[0] = fn_1_3ED3C(6, &v);
            lbl_1_bss_F3C[0] += 1;
            fn_1_8D04(0x418, lbl_1_bss_478[rec->row][rec->col], 1);
        }
        if (fn_1_3F714(6, lbl_1_bss_F8E[0]) != 0) {
            lbl_1_bss_1168[1] += 1;
        }
        break;
    case 4:                                         /* switch 1 */
        if (fn_1_30FA4(0xA, &lbl_1_bss_E68[1]) != 0) {
            lbl_1_bss_F3C[0] = 0;
            lbl_1_bss_1168[1] += 1;
        }
        break;
    case 5:                                         /* switch 1 */
        if (lbl_1_bss_F3C[0] == 0) {
            lbl_1_bss_E9C[0].x = lbl_1_bss_E68[1].x;
            lbl_1_bss_E9C[0].y = lbl_1_bss_E68[1].y;
            lbl_1_bss_E9C[0].z = lbl_1_bss_E68[1].z;
            lbl_1_bss_E9C[0].x += -50.0f;
            v = lbl_1_bss_E9C[0];
            lbl_1_bss_F8E[0] = fn_1_3ED3C(5, &v);
            lbl_1_bss_F3C[0] += 1;
            fn_1_8D04(0x414, lbl_1_bss_470[arg0->order[arg0->curPlayer]], 1);
        }
        if (fn_1_3F714(5, lbl_1_bss_F8E[0]) != 0) {
            lbl_1_bss_F14[0] = 0.0f;
            lbl_1_bss_1168[1] += 1;
            fn_1_8D04(0x412, lbl_1_bss_470[arg0->order[arg0->curPlayer]], 1);
        }
        break;
    case 6:                                         /* switch 1 */
        lbl_1_bss_F14[0] += 0.05f;
        if (lbl_1_bss_F14[0] > 1.0f) {
            lbl_1_bss_F14[0] = 1.0f;
            lbl_1_bss_F3C[0] = 0;
            lbl_1_bss_1168[1] += 1;
        }
        Hu3DModelPosSet(lbl_1_bss_468[arg0->order[arg0->curPlayer]], -50.0f + lbl_1_bss_E68[1].x, lbl_1_bss_E68[1].y, lbl_1_bss_E68[1].z);
        Hu3DModelRotSet(lbl_1_bss_468[arg0->order[arg0->curPlayer]], 0.0f, 0.0f, 0.0f);
        Hu3DModelScaleSet(lbl_1_bss_468[arg0->order[arg0->curPlayer]], lbl_1_bss_F14[0], lbl_1_bss_F14[0], lbl_1_bss_F14[0]);
        break;
    case 7:                                         /* switch 1 */
        if (lbl_1_bss_F3C[0] == 0) {
            fn_1_3F278(5, lbl_1_bss_F8E[0]);
            lbl_1_bss_E9C[0].x = lbl_1_bss_E68[1].x;
            lbl_1_bss_E9C[0].y = lbl_1_bss_E68[1].y;
            lbl_1_bss_E9C[0].z = lbl_1_bss_E68[1].z;
            lbl_1_bss_E9C[0].x += -50.0f;
            v = lbl_1_bss_E9C[0];
            lbl_1_bss_F8E[0] = fn_1_3ED3C(6, &v);
            lbl_1_bss_F3C[0] += 1;
            fn_1_8D04(0x418, lbl_1_bss_470[arg0->order[arg0->curPlayer]], 1);
        }
        if (fn_1_3F714(6, lbl_1_bss_F8E[0]) != 0) {
            fn_1_3F278(6, lbl_1_bss_F8E[0]);
            lbl_1_bss_1168[1] += 1;
            rec->f50 |= 0x10;
            var_r25 = 1;
        }
        break;
    case 8:                                         /* switch 1 */
        rec->f00 = rec->f04;
        rec->row = rec->f08;
        rec->col = rec->f0C;
        rec->f20 = 1;
        rec->f24 = 1;
        ((s32 *) lbl_1_bss_12A8)[10] = 0x3C;
        fn_1_4FBC8(arg0, 0x10);
        lbl_1_bss_1168[1] = 0;
        var_r25 = 1;
        break;
    }
    return var_r25;
}


s16 lbl_1_bss_BE8;
s32 lbl_1_bss_BEC;
s32 lbl_1_bss_BF0;
Vec lbl_1_bss_BF4;
f32 lbl_1_bss_C00;
s32 lbl_1_bss_C50[18];
Vec lbl_1_bss_C98[18];
Vec lbl_1_bss_D70[18];
s32 lbl_1_bss_E8C[3];
s16 lbl_1_bss_102A[18];
s16 lbl_1_bss_1050[3];
extern void fn_1_4149C(s32 plNo);
extern void fn_1_3FD5C(void);

static const char lbl_1_rodata_424[18] = { 'r', 'o', '0', '0', '0', 0, 'r', 'o', '0', '0', '2', 0, 'r', 'o', '0', '0', '1', 0 };
static const s32 cardSeA[7] = { 0x1F6, 0x1E2, 0x25A, 0x2AA, 0x282, 0x1BA, 0x296 };
static const s32 cardSeB[7] = { 0x200, 0x1EC, 0x264, 0x2B4, 0x28C, 0x1C4, 0x2A0 };

s32 fn_1_227A4(s32 plSel, CardGame *arg0)
{
    Vec pos;
    Vec rot;
    Vec out;
    Vec va;
    Vec vb;
    Vec cur2d;
    CardPlayer *rec;
    CardPlayer *other;
    HSFMATERIAL *mat;
    s16 id;
    s32 c0;
    s32 c1;
    s32 c2;
    s32 var_r18;
    s32 var_r22;
    s32 var_r23;
    s32 var_r24;
    s32 var_r25;
    s32 charNo;
    s32 i;
    s32 j;
    s32 doRot;
    f32 var_f31;

    var_r22 = 0;
    rec = &arg0->pl[arg0->order[arg0->curPlayer]];
    if (plSel == 0) {
        pos = lbl_1_bss_BF4;
        var_f31 = lbl_1_bss_C00;
    } else {
        fn_1_9DA8(&pos, arg0);
        pos.x += -50.0f;
        pos.y += 400.0f;
        var_f31 = 0.7f;
    }
    switch (lbl_1_bss_1168[1]) {
    case 0:
        lbl_1_bss_BEC = -1;
        lbl_1_bss_BF0 = -1;
        if (plSel == 0) {
            var_r18 = 0x42;
        } else {
            var_r18 = 0x45;
        }
        lbl_1_bss_1050[0] = Hu3DModelCreateData(lbl_1_data_B0[var_r18]);
        Hu3DModelPosSetV(lbl_1_bss_1050[0], &pos);
        rot.x = 90.0f;
        rot.y = 0.0f;
        rot.z = -4.0f;
        fn_1_25444(&rot.x, &out);
        Hu3DModelRotSetV(lbl_1_bss_1050[0], &out);
        Hu3DModelScaleSet(lbl_1_bss_1050[0], var_f31, 1.0f, var_f31);
        Hu3DModelLayerSet(lbl_1_bss_1050[0], 2);
        lbl_1_bss_1050[1] = Hu3DModelCreateData(lbl_1_data_B0[var_r18 + 1]);
        Hu3DModelPosSetV(lbl_1_bss_1050[1], &pos);
        rot.x = 90.0f;
        rot.y = -10.0f;
        rot.z = -4.0f;
        fn_1_25444(&rot.x, &out);
        Hu3DModelRotSetV(lbl_1_bss_1050[1], &out);
        Hu3DModelScaleSet(lbl_1_bss_1050[1], var_f31, 1.0f, var_f31);
        Hu3DModelLayerSet(lbl_1_bss_1050[1], 2);
        if (plSel == 0) {
            lbl_1_bss_1050[2] = Hu3DModelCreateData(lbl_1_data_B0[0x44]);
            Hu3DModelPosSetV(lbl_1_bss_1050[2], &pos);
            rot.x = 90.0f;
            rot.y = 0.0f;
            rot.z = -4.0f;
            fn_1_25444(&rot.x, &out);
            Hu3DModelRotSetV(lbl_1_bss_1050[2], &out);
            Hu3DModelScaleSet(lbl_1_bss_1050[2], var_f31, 1.0f, var_f31);
            Hu3DModelLayerSet(lbl_1_bss_1050[2], 2);
        }
        Hu3DModelCameraSet(lbl_1_bss_1050[0], 1);
        Hu3DModelCameraSet(lbl_1_bss_1050[1], 1);
        if (plSel == 0) {
            Hu3DModelCameraSet(lbl_1_bss_1050[2], 1);
        }
        var_r23 = 0;
        for (i = 0; i < 4; i++) {
            charNo = GwPlayerConf[i].charNo;
            if (rec->charNo != charNo) {
                Hu3DAnimCreate(HuDataReadNum(lbl_1_data_B0[charNo + 0x47], 0x10000000), lbl_1_bss_1050[1],
                    (char *) &lbl_1_rodata_424[var_r23 * 6]);
                lbl_1_bss_E8C[var_r23] = charNo;
                var_r23 += 1;
            }
        }
        lbl_1_bss_1056[0x12] = Hu3DLLightCreate(lbl_1_bss_1050[0], pos.x, pos.y, pos.z, 0.0f, 0.0f, -1.0f, 0x80, 0x80, 0x80);
        Hu3DLLightStaticSet(lbl_1_bss_1050[0], lbl_1_bss_1056[0x12], 1);
        Hu3DLLightInfinitytSet(lbl_1_bss_1050[0], lbl_1_bss_1056[0x12]);
        lbl_1_bss_1056[0x13] = Hu3DLLightCreate(lbl_1_bss_1050[1], pos.x, pos.y, pos.z, 0.0f, 0.0f, -1.0f, 0x80, 0x80, 0x80);
        Hu3DLLightStaticSet(lbl_1_bss_1050[1], lbl_1_bss_1056[0x13], 1);
        Hu3DLLightInfinitytSet(lbl_1_bss_1050[1], lbl_1_bss_1056[0x13]);
        if (plSel == 0) {
            lbl_1_bss_1056[0x14] = Hu3DLLightCreate(lbl_1_bss_1050[2], pos.x, pos.y, pos.z, 0.0f, 0.0f, -1.0f, 0x80, 0x80, 0x80);
            Hu3DLLightStaticSet(lbl_1_bss_1050[2], lbl_1_bss_1056[0x14], 1);
            Hu3DLLightInfinitytSet(lbl_1_bss_1050[2], lbl_1_bss_1056[0x14]);
        }
        if (GwPlayerConf[arg0->order[arg0->curPlayer]].type == 0) {
            fn_1_502E0(0x12);
        }
        lbl_1_bss_F3C[0] = 0;
        lbl_1_bss_F3C[1] = 0;
        if (plSel == 0) {
            lbl_1_bss_1168[1] = 0x63;
        } else {
            lbl_1_bss_1168[1] += 1;
        }
        HuAudFXPlay(0x2FF);
        break;
    case 1:
        if (lbl_1_bss_F3C[0] == 0) {
            rot.x = 90.0f;
            rot.y = -10.0f;
            rot.z = -4.0f;
            rot.y -= (f32) lbl_1_bss_F3C[1];
            fn_1_25444(&rot.x, &out);
            Hu3DModelRotSetV(lbl_1_bss_1050[1], &out);
            if (lbl_1_bss_F3C[1] < 0x159) {
                lbl_1_bss_F3C[1] = lbl_1_bss_F3C[1] + 0xF;
            } else {
                lbl_1_bss_F3C[1] = 0;
            }
            fn_1_25518(arg0->order[arg0->curPlayer]);
            if ((HuPadBtnDown[arg0->order[arg0->curPlayer]] & 0x100) != 0) {
                lbl_1_bss_F3C[0] = 1;
                lbl_1_bss_F3C[2] = 0xF;
                HuAudFXPlay(0x301);
            }
        } else {
            rot.x = 90.0f;
            rot.y = -10.0f;
            rot.z = -4.0f;
            rot.y -= (f32) lbl_1_bss_F3C[1];
            fn_1_25444(&rot.x, &out);
            Hu3DModelRotSetV(lbl_1_bss_1050[1], &out);
            if (lbl_1_bss_F3C[1] < (0x168 - lbl_1_bss_F3C[2])) {
                lbl_1_bss_F3C[1] = lbl_1_bss_F3C[1] + lbl_1_bss_F3C[2];
            } else {
                lbl_1_bss_F3C[1] = lbl_1_bss_F3C[1] + (lbl_1_bss_F3C[2] - 0x168);
            }
            if (lbl_1_bss_F3C[2] == 0) {
                lbl_1_bss_F3C[0] = -1;
            } else {
                lbl_1_bss_F3C[2] -= 1;
            }
        }
        if (lbl_1_bss_F3C[0] == -1) {
            fn_1_50460();
            if (lbl_1_bss_F3C[1] >= 0x37 && lbl_1_bss_F3C[1] <= 0xAA) {
                arg0->f1A8 = lbl_1_bss_E8C[1];
            } else if (lbl_1_bss_F3C[1] >= 0xAF && lbl_1_bss_F3C[1] <= 0x122) {
                arg0->f1A8 = lbl_1_bss_E8C[2];
            } else {
                arg0->f1A8 = lbl_1_bss_E8C[0];
            }
            lbl_1_bss_F3C[0] = 0;
            if (plSel != 0) {
                lbl_1_bss_1168[1] = 2;
            } else {
                lbl_1_bss_1168[1] = 3;
            }
            mat = Hu3DData[lbl_1_bss_1050[1]].hsf->material;
            for (var_r25 = 0; var_r25 < 3; var_r25++) {
                if (arg0->f1A8 != lbl_1_bss_E8C[var_r25]) {
                    for (j = 0; j < 4; j++) {
                        if (lbl_1_bss_E8C[var_r25] == arg0->pl[j].charNo) {
                            mat->color[0] = 0x50;
                            mat->color[1] = 0x50;
                            mat->color[2] = 0x50;
                        }
                    }
                }
                mat++;
            }
        }
        break;
    case 2:
        lbl_1_bss_F3C[0] += 1;
        if (lbl_1_bss_F3C[0] == 0x3C) {
            va = pos;
            id = fn_1_3ED3C(2, &va);
            lbl_1_bss_E9C[0].x = 2.0f;
            lbl_1_bss_E9C[0].y = 2.0f;
            lbl_1_bss_E9C[0].z = 2.0f;
            vb = lbl_1_bss_E9C[0];
            fn_1_3F6B0(2, id, &vb);
        }
        if (lbl_1_bss_F3C[0] > 0x46) {
            Hu3DAnimModelKill(lbl_1_bss_1050[1]);
            Hu3DModelKill(lbl_1_bss_1050[0]);
            Hu3DModelKill(lbl_1_bss_1050[1]);
            Hu3DLLightKill(lbl_1_bss_1050[0], lbl_1_bss_1056[0x12]);
            Hu3DLLightKill(lbl_1_bss_1050[1], lbl_1_bss_1056[0x13]);
            lbl_1_bss_1168[1] = 0;
            var_r22 = 1;
        }
        break;
    case 3:
        for (i = 0; i < 3 && arg0->f1A8 != arg0->pl[arg0->order[i]].charNo; i++) {
        }
        lbl_1_bss_F3C[0] = i;
        other = &arg0->pl[arg0->order[lbl_1_bss_F3C[0]]];
        c0 = other->f44;
        c1 = other->f48;
        c2 = other->f4C;
        lbl_1_bss_F3C[1] = c0 + (c1 + c2);
        if (lbl_1_bss_F3C[1] <= 1) {
            if (lbl_1_bss_F3C[1] != 0) {
                fn_1_4149C(arg0->order[lbl_1_bss_F3C[0]]);
            }
            lbl_1_bss_F3C[2] = 0;
            lbl_1_bss_102A[0] = -1;
            lbl_1_bss_1168[1] = 8;
        } else {
            for (i = 0; i < lbl_1_bss_F3C[1]; i++) {
                j = frandmod(3);
                if (j == 0) {
                    if (c0 > 0) {
                        lbl_1_bss_C50[i] = 0;
                        c0 -= 1;
                    } else if (c1 > 0) {
                        lbl_1_bss_C50[i] = 1;
                        c1 -= 1;
                    } else {
                        lbl_1_bss_C50[i] = 2;
                        c2 -= 1;
                    }
                } else if (j == 1) {
                    if (c1 > 0) {
                        lbl_1_bss_C50[i] = 1;
                        c1 -= 1;
                    } else if (c2 > 0) {
                        lbl_1_bss_C50[i] = 2;
                        c2 -= 1;
                    } else {
                        lbl_1_bss_C50[i] = 0;
                        c0 -= 1;
                    }
                } else if (c2 > 0) {
                    lbl_1_bss_C50[i] = 2;
                    c2 -= 1;
                } else if (c0 > 0) {
                    lbl_1_bss_C50[i] = 0;
                    c0 -= 1;
                } else {
                    lbl_1_bss_C50[i] = 1;
                    c1 -= 1;
                }
            }
            lbl_1_bss_F14[1] = 360.0f / (f32) lbl_1_bss_F3C[1];
            lbl_1_bss_E9C[0].x = lbl_1_data_528[lbl_1_bss_F3C[0]][0] - 12.0f;
            lbl_1_bss_E9C[0].y = lbl_1_data_528[lbl_1_bss_F3C[0]][1] - 13.0f;
            lbl_1_bss_E9C[0].z = 100.0f + pos.z;
            for (i = 0; i < lbl_1_bss_F3C[1]; i++) {
                if (lbl_1_bss_C50[i] == 0) {
                    j = 0x21;
                } else if (lbl_1_bss_C50[i] == 1) {
                    j = 0x22;
                } else {
                    j = 0x23;
                }
                lbl_1_bss_102A[i] = Hu3DModelCreateData(lbl_1_data_B0[j]);
                Hu3DModelCameraSet(lbl_1_bss_102A[i], 1);
                rot.x = 90.0f;
                rot.y = 0.0f;
                rot.z = -4.0f;
                fn_1_25444(&rot.x, &out);
                Hu3DModelRotSetV(lbl_1_bss_102A[i], &out);
                Hu3DModelScaleSet(lbl_1_bss_102A[i], 0.13f * var_f31, 0.13f * var_f31, 0.13f * var_f31);
                Hu3DModelAttrSet(lbl_1_bss_102A[i], 1);
                Hu3DCamera[1] = Hu3DCamera[0];
                Hu3DModelLayerSet(lbl_1_bss_102A[i], 7);
                Hu3DModelCameraSet(lbl_1_bss_102A[i], 2);
                lbl_1_bss_C98[i].x = pos.x;
                lbl_1_bss_C98[i].y = pos.y;
                lbl_1_bss_C98[i].z = pos.z;
                lbl_1_bss_C98[i].z += 100.0f;
                lbl_1_bss_C98[i].x = (f32) ((f64) lbl_1_bss_C98[i].x
                    + ((f64) (155.0f * var_f31) * sin((3.141592653589793 * (f64) (lbl_1_bss_F14[1] * (f32) i)) / 180.0)));
                lbl_1_bss_C98[i].y = (f32) ((f64) lbl_1_bss_C98[i].y
                    + ((f64) (155.0f * var_f31) * cos((3.141592653589793 * (f64) (lbl_1_bss_F14[1] * (f32) i)) / 180.0)));
                lbl_1_bss_C98[i].x += 7.0f;
                lbl_1_bss_1056[i] = Hu3DLLightCreate(lbl_1_bss_102A[i], lbl_1_bss_C98[i].x, lbl_1_bss_C98[i].y, lbl_1_bss_C98[i].z,
                    0.0f, 0.0f, -1.0f, 0x80, 0x80, 0x80);
                Hu3DLLightStaticSet(lbl_1_bss_102A[i], lbl_1_bss_1056[i], 1);
                Hu3DLLightInfinitytSet(lbl_1_bss_102A[i], lbl_1_bss_1056[i]);
                Hu3D3Dto2D(&lbl_1_bss_C98[i], 1, &lbl_1_bss_D70[i]);
                va = lbl_1_bss_D70[i];
                vb = lbl_1_bss_E9C[0];
                fn_1_4F144(i, &vb, &va, 0x28, 0.0f, 0x21);
            }
            lbl_1_bss_F3C[2] = 0;
            lbl_1_bss_F14[2] = 0.13f * var_f31;
            lbl_1_bss_1168[1] += 1;
        }
        break;
    case 4:
        lbl_1_bss_F14[2] += 0.02f * var_f31;
        if (lbl_1_bss_F14[2] > (0.2f * var_f31)) {
            lbl_1_bss_F14[2] = 0.2f * var_f31;
        }
        for (i = 0; i < lbl_1_bss_F3C[1]; i++) {
            fn_1_4F1EC(i, &cur2d);
            va = lbl_1_bss_C98[i];
            fn_1_4EDB8(&lbl_1_bss_E9C[0], &va, cur2d.x, cur2d.y);
            Hu3DModelPosSetV(lbl_1_bss_102A[i], &lbl_1_bss_E9C[0]);
            Hu3DModelAttrReset(lbl_1_bss_102A[i], 1);
            Hu3DModelScaleSet(lbl_1_bss_102A[i], lbl_1_bss_F14[2], lbl_1_bss_F14[2], lbl_1_bss_F14[2]);
        }
        lbl_1_bss_F3C[2] += 1;
        if (lbl_1_bss_F3C[2] > 0x32) {
            lbl_1_bss_F3C[2] = 1;
            lbl_1_bss_F3C[3] = 0;
            lbl_1_bss_F14[2] = 0.0f;
            lbl_1_bss_1168[1] += 1;
            HuAudFXPlay(0x2FF);
        }
        break;
    case 5:
        doRot = 1;
        lbl_1_bss_F14[2] += lbl_1_bss_F14[1] / 10.0f;
        if (lbl_1_bss_F14[2] > 360.0f) {
            lbl_1_bss_F14[2] = lbl_1_bss_F14[1] / 10.0f;
            lbl_1_bss_F3C[2] = 1;
        }
        if (lbl_1_bss_F14[2] > (lbl_1_bss_F14[1] * (f32) lbl_1_bss_F3C[2])) {
            if (lbl_1_bss_F3C[3] != 0) {
                doRot = 0;
                for (var_r24 = 0; arg0->order[var_r24] != arg0->order[lbl_1_bss_F3C[0]]; var_r24++) {
                }
                other = &arg0->pl[arg0->order[lbl_1_bss_F3C[0]]];
                for (i = 0; i < lbl_1_bss_F3C[1]; i++) {
                    if (i == (lbl_1_bss_F3C[1] - lbl_1_bss_F3C[2])) {
                        lbl_1_bss_E9C[0].x = lbl_1_data_528[arg0->curPlayer][0] - 12.0f;
                        lbl_1_bss_E9C[0].y = lbl_1_data_528[arg0->curPlayer][1] - 13.0f;
                        if (lbl_1_bss_C50[i] == 0) {
                            rec->f44 += 1;
                            other->f44 -= 1;
                        } else if (lbl_1_bss_C50[i] == 1) {
                            rec->f48 += 1;
                            other->f48 -= 1;
                        } else if (lbl_1_bss_C50[i] == 2) {
                            rec->f4C += 1;
                            other->f4C -= 1;
                        }
                        rec->f40 = rec->f44 + ((rec->f48 * 2) + (rec->f4C * -1));
                        other->f40 = other->f44 + ((other->f48 * 2) + (other->f4C * -1));
                        lbl_1_bss_BEC = arg0->order[lbl_1_bss_F3C[0]];
                        lbl_1_bss_BF0 = lbl_1_bss_C50[i];
                    } else {
                        lbl_1_bss_E9C[0].x = lbl_1_data_528[var_r24][0] - 12.0f;
                        lbl_1_bss_E9C[0].y = lbl_1_data_528[var_r24][1] - 13.0f;
                    }
                    lbl_1_bss_E9C[0].z = 100.0f + pos.z;
                    Hu3D3Dto2D(&lbl_1_bss_C98[i], 1, &lbl_1_bss_D70[i]);
                    va = lbl_1_bss_E9C[0];
                    vb = lbl_1_bss_D70[i];
                    fn_1_4F144(i, &vb, &va, 0x28, 0.0f, 0x21);
                }
                lbl_1_bss_F14[2] = 0.0f;
                lbl_1_bss_1168[1] += 1;
            } else {
                lbl_1_bss_F3C[2] += 1;
            }
        }
        if (doRot) {
            for (i = 0; i < lbl_1_bss_F3C[1]; i++) {
                lbl_1_bss_C98[i].x = pos.x;
                lbl_1_bss_C98[i].y = pos.y;
                lbl_1_bss_C98[i].z = pos.z;
                lbl_1_bss_C98[i].z += 100.0f;
                lbl_1_bss_C98[i].x = (f32) ((f64) lbl_1_bss_C98[i].x
                    + ((f64) (155.0f * var_f31)
                        * sin((3.141592653589793 * (f64) (lbl_1_bss_F14[2] + (lbl_1_bss_F14[1] * (f32) i))) / 180.0)));
                lbl_1_bss_C98[i].y = (f32) ((f64) lbl_1_bss_C98[i].y
                    + ((f64) (155.0f * var_f31)
                        * cos((3.141592653589793 * (f64) (lbl_1_bss_F14[2] + (lbl_1_bss_F14[1] * (f32) i))) / 180.0)));
                lbl_1_bss_C98[i].x += 7.0f;
                Hu3DModelPosSetV(lbl_1_bss_102A[i], &lbl_1_bss_C98[i]);
            }
            if (lbl_1_bss_F3C[3] == 0) {
                if (GwPlayerConf[arg0->order[arg0->curPlayer]].type == 0) {
                    fn_1_502E0(0x12);
                }
                fn_1_25518(arg0->order[arg0->curPlayer]);
                if ((HuPadBtnDown[arg0->order[arg0->curPlayer]] & 0x100) != 0) {
                    fn_1_50460();
                    lbl_1_bss_F3C[3] = 1;
                    HuAudFXPlay(0x301);
                }
            }
        }
        break;
    case 6:
        lbl_1_bss_F14[2] += 5.0f;
        if (lbl_1_bss_F14[2] > 180.0f) {
            lbl_1_bss_F14[2] = 180.0f;
            lbl_1_bss_F14[3] = 0.2f * var_f31;
            lbl_1_bss_F3C[2] = 0;
            lbl_1_bss_1168[1] += 1;
        }
        rot.x = 90.0f;
        rot.y = 0.0f;
        rot.z = -4.0f;
        rot.y += lbl_1_bss_F14[2];
        fn_1_25444(&rot.x, &out);
        Hu3DModelRotSetV(lbl_1_bss_1050[2], &out);
        break;
    case 7:
        if (lbl_1_bss_F3C[2] >= 0x1E) {
            lbl_1_bss_F14[3] -= 0.02f * var_f31;
            if (lbl_1_bss_F14[3] < (0.13f * var_f31)) {
                lbl_1_bss_F14[3] = 0.13f * var_f31;
            }
        }
        for (i = 0; i < lbl_1_bss_F3C[1]; i++) {
            fn_1_4F1EC(i, &cur2d);
            va = lbl_1_bss_C98[i];
            fn_1_4EDB8(&lbl_1_bss_E9C[0], &va, cur2d.x, cur2d.y);
            Hu3DModelPosSetV(lbl_1_bss_102A[i], &lbl_1_bss_E9C[0]);
            Hu3DModelScaleSet(lbl_1_bss_102A[i], lbl_1_bss_F14[3], lbl_1_bss_F14[3], lbl_1_bss_F14[3]);
        }
        lbl_1_bss_F3C[2] += 1;
        if (lbl_1_bss_F3C[2] > 0x28) {
            lbl_1_bss_F3C[2] = 0;
            if (lbl_1_bss_BF0 >= 0) {
                charNo = GwPlayerConf[arg0->order[arg0->curPlayer]].charNo;
                if (lbl_1_bss_BF0 != 2) {
                    Hu3DMotionShiftSet(lbl_1_bss_468[arg0->order[arg0->curPlayer]], lbl_1_bss_430[arg0->order[arg0->curPlayer]][0], 0.0f, 10.0f, 0);
                    Hu3DMotionShiftSet(lbl_1_bss_468[lbl_1_bss_BEC], lbl_1_bss_430[lbl_1_bss_BEC][1], 0.0f, 10.0f, 0);
                    if (charNo >= 0 && charNo < 7) {
                        lbl_1_bss_20[1] = cardSeA[charNo];
                    }
                    fn_1_8D04(lbl_1_bss_20[1], lbl_1_bss_468[arg0->order[arg0->curPlayer]], 1);
                    lbl_1_bss_20[1] = 0;
                } else {
                    Hu3DMotionShiftSet(lbl_1_bss_468[arg0->order[arg0->curPlayer]], lbl_1_bss_430[arg0->order[arg0->curPlayer]][1], 0.0f, 10.0f, 0);
                    Hu3DMotionShiftSet(lbl_1_bss_468[lbl_1_bss_BEC], lbl_1_bss_430[lbl_1_bss_BEC][0], 0.0f, 10.0f, 0);
                    if (charNo >= 0 && charNo < 7) {
                        lbl_1_bss_20[1] = cardSeB[charNo];
                    }
                    fn_1_8D04(lbl_1_bss_20[1], lbl_1_bss_468[arg0->order[arg0->curPlayer]], 1);
                    lbl_1_bss_20[1] = 0;
                }
                HuAudFXPlay(8);
            }
            fn_1_2FFAC(arg0);
            lbl_1_bss_1168[1] += 1;
        }
        break;
    case 8:
        lbl_1_bss_F3C[2] += 1;
        if (lbl_1_bss_F3C[2] == 1) {
            for (i = 0; i < lbl_1_bss_F3C[1]; i++) {
                Hu3DModelAttrSet(lbl_1_bss_102A[i], 1);
            }
        }
        if (lbl_1_bss_F3C[2] == 0x14) {
            if (lbl_1_bss_BF0 == 1) {
                HuAudFXPlay(8);
            }
            Hu3DModelAttrReset(lbl_1_bss_BE8, 1);
            fn_1_3FD5C();
        }
        if (lbl_1_bss_F3C[2] > 0x1E) {
            Hu3DAnimModelKill(lbl_1_bss_1050[1]);
            Hu3DModelKill(lbl_1_bss_1050[0]);
            Hu3DModelKill(lbl_1_bss_1050[1]);
            Hu3DModelKill(lbl_1_bss_1050[2]);
            if (lbl_1_bss_102A[0] != -1) {
                for (i = 0; i < lbl_1_bss_F3C[1]; i++) {
                    Hu3DLLightKill(lbl_1_bss_1050[0], lbl_1_bss_1056[i]);
                    Hu3DModelKill(lbl_1_bss_102A[i]);
                }
                var_r22 = 1;
            } else {
                if (lbl_1_bss_F3C[1] != 0) {
                    var_r22 = 1;
                } else {
                    var_r22 = 2;
                }
            }
            Hu3DLLightKill(lbl_1_bss_1050[0], lbl_1_bss_1056[0x12]);
            Hu3DLLightKill(lbl_1_bss_1050[1], lbl_1_bss_1056[0x13]);
            Hu3DLLightKill(lbl_1_bss_1050[2], lbl_1_bss_1056[0x14]);
            lbl_1_bss_1168[1] = 0;
        }
        break;
    case 0x63:
        rot.x = 90.0f;
        rot.y = -10.0f;
        rot.z = -4.0f;
        rot.y -= (f32) lbl_1_bss_F3C[1];
        fn_1_25444(&rot.x, &out);
        Hu3DModelRotSetV(lbl_1_bss_1050[1], &out);
        if (lbl_1_bss_F3C[1] < 0x159) {
            lbl_1_bss_F3C[1] = lbl_1_bss_F3C[1] + 0xF;
        } else {
            lbl_1_bss_F3C[1] = 0;
        }
        if (Hu3DMotionEndCheck(lbl_1_bss_BE8) != 0) {
            Hu3DModelAttrSet(lbl_1_bss_BE8, 1);
            lbl_1_bss_1168[1] = 1;
        }
        break;
    }
    return var_r22;
}


s16 lbl_1_bss_F86[4];
s16 lbl_1_bss_F96[4];
extern void fn_1_AB64(s32 row, s32 col, s32 flags);
extern void fn_1_31AC0(Vec *pos, Vec *target);

/* card model index for the low nibble of a board cell (same table as fn_1_28CA8) */
static s32 cardCellModel(s32 flags)
{
    flags &= 0xF;
    if (flags == 0xF) {
        return 0xD;
    } else if (flags == 0xC) {
        return 0xE;
    } else if (flags == 3) {
        return 0xF;
    } else if (flags == 0xB) {
        return 0x10;
    } else if (flags == 0xD) {
        return 0x11;
    } else if (flags == 7) {
        return 0x12;
    } else if (flags == 0xE) {
        return 0x13;
    } else if (flags == 0xA) {
        return 0x14;
    } else if (flags == 9) {
        return 0x15;
    } else if (flags == 5) {
        return 0x16;
    }
    return 0x17;
}

/* nonzero if cell (r, c) is outside the board, holds a card, or is occupied by a player */
static s32 cardCellBlocked(CardGame *game, s32 outside, s32 r, s32 c)
{
    s32 i;

    if (outside) {
        return 1;
    }
    if ((lbl_1_bss_8C4[r][c] & 0x700) != 0) {
        return 1;
    }
    for (i = 0; i < 4; i++) {
        if (r == game->pl[i].row && c == game->pl[i].col) {
            return 1;
        }
    }
    return 0;
}

/* spawn a replacement card model next to the flipped cell; mask = f1AC bit, dx/dz = offset */
static void cardSpawnNeighbour(CardGame *game, s32 slot, s32 r, s32 c, s32 mask, f32 dx, f32 dz)
{
    Vec v;
    Vec v2;

    lbl_1_bss_F96[slot] = -1;
    if ((lbl_1_bss_8C4[r][c] & 0x40) == 0 && (game->f1AC & mask) != 0) {
        lbl_1_bss_F96[slot] = Hu3DModelCreateData(lbl_1_data_B0[cardCellModel(lbl_1_bss_8C4[r][c])]);
        Hu3DModelCameraSet(lbl_1_bss_F96[slot], 1);
        Hu3DModelRotSet(lbl_1_bss_F96[slot], 0.0f, 0.0f, 0.0f);
        Hu3DModelAttrSet(lbl_1_bss_F96[slot], 1);
        v.x = (-50.0f + lbl_1_bss_E68[0].x) + dx;
        v.y = lbl_1_bss_E68[0].y - 75.0f;
        v.z = lbl_1_bss_E68[0].z + dz;
        Hu3DModelPosSetV(lbl_1_bss_F96[slot], &v);
        v2 = v;
        fn_1_3ED3C(0x11, &v2);
    }
}

s32 fn_1_258E4(CardGame *arg0)
{
    Vec pos;
    Vec v;
    CardPlayer *rec;
    s32 var_r26;
    s32 var_r29;
    s32 var_r30;
    s32 i;
    s32 dir;

    var_r26 = 0;
    rec = &arg0->pl[arg0->order[arg0->curPlayer]];
    var_r30 = rec->row;
    var_r29 = rec->col;
    fn_1_9C74(&pos, var_r30, var_r29, 1, 1);
    switch (lbl_1_bss_1168[1]) {
    case 0:
        pos.x += -50.0f;
        if (fn_1_30FA4(0xB, &pos) != 0) {
            lbl_1_bss_1168[1] += 1;
            ((s32 *) lbl_1_bss_12A8)[10] = 0x3C;
            fn_1_4FBC8(arg0, 0xF);
        }
        break;
    case 1:
        lbl_1_bss_111E[0] = Hu3DModelCreateData(lbl_1_data_B0[GwPlayerConf[arg0->order[arg0->curPlayer]].charNo + 0x50]);
        Hu3DModelCameraSet(lbl_1_bss_111E[0], 1);
        Hu3DModelRotSet(lbl_1_bss_111E[0], 0.0f, 0.0f, 0.0f);
        Hu3DModelShadowSet(lbl_1_bss_111E[0]);
        arg0->f1A8 = 0;
        if (cardCellBlocked(arg0, rec->row - 1 < 0, rec->row - 1, rec->col)) {
            arg0->f1A8 |= 1;
        }
        if (cardCellBlocked(arg0, rec->row + 1 > arg0->boardSize - 1, rec->row + 1, rec->col)) {
            arg0->f1A8 |= 2;
        }
        if (cardCellBlocked(arg0, rec->col - 1 < 0, rec->row, rec->col - 1)) {
            arg0->f1A8 |= 4;
        }
        if (cardCellBlocked(arg0, rec->col + 1 > arg0->boardSize - 1, rec->row, rec->col + 1)) {
            arg0->f1A8 |= 8;
        }
        if ((arg0->f1A8 & 1) == 0) {
            pos.z -= 300.0f;
            arg0->f1AC = 0;
        } else if ((arg0->f1A8 & 2) == 0) {
            pos.z += 300.0f;
            arg0->f1AC = 1;
        } else if ((arg0->f1A8 & 4) == 0) {
            pos.x -= 300.0f;
            arg0->f1AC = 2;
        } else if ((arg0->f1A8 & 8) == 0) {
            pos.x += 300.0f;
            arg0->f1AC = 3;
        }
        Hu3DModelPosSet(lbl_1_bss_111E[0], (-50.0f + pos.x) - 80.0f, 100.0f + pos.y, 100.0f + pos.z);
        fn_1_9C74(&pos, rec->row, rec->col, 1, 1);
        pos.x += -50.0f;
        pos.y = 0.0f;
        lbl_1_bss_F86[0] = -1;
        if ((arg0->f1A8 & 1) == 0) {
            lbl_1_bss_E9C[0] = pos;
            lbl_1_bss_E9C[0].z = lbl_1_bss_E9C[0].z - 300.0f;
            v = lbl_1_bss_E9C[0];
            lbl_1_bss_F86[0] = fn_1_3ED3C(0x12, &v);
        }
        lbl_1_bss_F86[1] = -1;
        if ((arg0->f1A8 & 2) == 0) {
            lbl_1_bss_E9C[0] = pos;
            lbl_1_bss_E9C[0].z = lbl_1_bss_E9C[0].z + 300.0f;
            v = lbl_1_bss_E9C[0];
            lbl_1_bss_F86[1] = fn_1_3ED3C(0x12, &v);
        }
        lbl_1_bss_F86[2] = -1;
        if ((arg0->f1A8 & 4) == 0) {
            lbl_1_bss_E9C[0] = pos;
            lbl_1_bss_E9C[0].x = lbl_1_bss_E9C[0].x - 300.0f;
            v = lbl_1_bss_E9C[0];
            lbl_1_bss_F86[2] = fn_1_3ED3C(0x12, &v);
        }
        lbl_1_bss_F86[3] = -1;
        if ((arg0->f1A8 & 8) == 0) {
            lbl_1_bss_E9C[0] = pos;
            lbl_1_bss_E9C[0].x = lbl_1_bss_E9C[0].x + 300.0f;
            v = lbl_1_bss_E9C[0];
            lbl_1_bss_F86[3] = fn_1_3ED3C(0x12, &v);
        }
        lbl_1_bss_1168[1] += 1;
        break;
    case 2:
        if (GwPlayerConf[arg0->order[arg0->curPlayer]].type == 0) {
            fn_1_502E0(0x13);
        }
        fn_1_290BC(arg0->order[arg0->curPlayer]);
        if ((HuPadDStkRep[arg0->order[arg0->curPlayer]] & 8) != 0) {
            if (arg0->f1AC != 0 && (arg0->f1A8 & 1) == 0) {
                arg0->f1AC = 0;
                HuAudFXPlay(0);
            }
        } else if ((HuPadDStkRep[arg0->order[arg0->curPlayer]] & 4) != 0) {
            if (arg0->f1AC != 1 && (arg0->f1A8 & 2) == 0) {
                arg0->f1AC = 1;
                HuAudFXPlay(0);
            }
        } else if ((HuPadDStkRep[arg0->order[arg0->curPlayer]] & 1) != 0) {
            if (arg0->f1AC != 2 && (arg0->f1A8 & 4) == 0) {
                arg0->f1AC = 2;
                HuAudFXPlay(0);
            }
        } else if ((HuPadDStkRep[arg0->order[arg0->curPlayer]] & 2) != 0 && arg0->f1AC != 3 && (arg0->f1A8 & 8) == 0) {
            arg0->f1AC = 3;
            HuAudFXPlay(0);
        }
        switch (arg0->f1AC) {
        case 0:
            pos.z -= 300.0f;
            break;
        case 1:
            pos.z += 300.0f;
            break;
        case 2:
            pos.x -= 300.0f;
            break;
        case 3:
            pos.x += 300.0f;
            break;
        }
        Hu3DModelPosSet(lbl_1_bss_111E[0], (-50.0f + pos.x) - 80.0f, 100.0f + pos.y, 100.0f + pos.z);
        if ((HuPadBtnDown[arg0->order[arg0->curPlayer]] & 0x100) != 0) {
            fn_1_50460();
            lbl_1_bss_E68[0].x = pos.x;
            lbl_1_bss_E68[0].y = 75.0f + pos.y;
            lbl_1_bss_E68[0].z = pos.z;
            switch (arg0->f1AC) {
            case 0:
                var_r30 = rec->row - 1;
                var_r29 = rec->col;
                break;
            case 1:
                var_r30 = rec->row + 1;
                var_r29 = rec->col;
                break;
            case 2:
                var_r30 = rec->row;
                var_r29 = rec->col - 1;
                break;
            case 3:
                var_r30 = rec->row;
                var_r29 = rec->col + 1;
                break;
            }
            fn_1_28CA8(arg0, var_r30, var_r29);
            arg0->f1AC = 0;
            if (var_r30 - 1 >= 0) {
                fn_1_28CA8(arg0, var_r30 - 1, var_r29);
                lbl_1_bss_8C4[var_r30][var_r29] |= 1;
                lbl_1_bss_8C4[var_r30 - 1][var_r29] |= 2;
                fn_1_AB64(var_r30 - 1, var_r29, lbl_1_bss_8C4[var_r30 - 1][var_r29]);
                if ((lbl_1_bss_8C4[var_r30 - 1][var_r29] & 0x700) == 0) {
                    arg0->f1AC |= 1;
                }
            }
            if (var_r30 + 1 <= arg0->boardSize - 1) {
                fn_1_28CA8(arg0, var_r30 + 1, var_r29);
                lbl_1_bss_8C4[var_r30][var_r29] |= 2;
                lbl_1_bss_8C4[var_r30 + 1][var_r29] |= 1;
                fn_1_AB64(var_r30 + 1, var_r29, lbl_1_bss_8C4[var_r30 + 1][var_r29]);
                if ((lbl_1_bss_8C4[var_r30 + 1][var_r29] & 0x700) == 0) {
                    arg0->f1AC |= 2;
                }
            }
            if (var_r29 - 1 >= 0) {
                fn_1_28CA8(arg0, var_r30, var_r29 - 1);
                lbl_1_bss_8C4[var_r30][var_r29] |= 4;
                lbl_1_bss_8C4[var_r30][var_r29 - 1] |= 8;
                fn_1_AB64(var_r30, var_r29 - 1, lbl_1_bss_8C4[var_r30][var_r29 - 1]);
                if ((lbl_1_bss_8C4[var_r30][var_r29 - 1] & 0x700) == 0) {
                    arg0->f1AC |= 4;
                }
            }
            if (var_r29 + 1 <= arg0->boardSize - 1) {
                fn_1_28CA8(arg0, var_r30, var_r29 + 1);
                lbl_1_bss_8C4[var_r30][var_r29] |= 8;
                lbl_1_bss_8C4[var_r30][var_r29 + 1] |= 4;
                fn_1_AB64(var_r30, var_r29 + 1, lbl_1_bss_8C4[var_r30][var_r29 + 1]);
                if ((lbl_1_bss_8C4[var_r30][var_r29 + 1] & 0x700) == 0) {
                    arg0->f1AC |= 8;
                }
            }
            fn_1_AB64(var_r30, var_r29, lbl_1_bss_8C4[var_r30][var_r29]);
            lbl_1_bss_8C4[var_r30][var_r29] |= 0x50;
            lbl_1_bss_F3C[8] = var_r30;
            lbl_1_bss_F3C[9] = var_r29;
            lbl_1_bss_1168[1] += 1;
            for (i = 0; i < 4; i++) {
                if (lbl_1_bss_F86[i] != -1) {
                    fn_1_3F278(0x12, lbl_1_bss_F86[i]);
                }
            }
            HuAudFXPlay(2);
        }
        break;
    case 3:
        lbl_1_bss_1080[0] = Hu3DModelCreateData(lbl_1_data_B0[0x68]);
        Hu3DModelCameraSet(lbl_1_bss_1080[0], 1);
        Hu3DModelRotSet(lbl_1_bss_1080[0], 0.0f, 0.0f, 0.0f);
        Hu3DModelScaleSet(lbl_1_bss_1080[0], 0.7f, 1.0f, 0.7f);
        lbl_1_bss_E68[0].y += 5000.0f;
        Hu3DModelPosSet(lbl_1_bss_1080[0], -50.0f + lbl_1_bss_E68[0].x, lbl_1_bss_E68[0].y, lbl_1_bss_E68[0].z);
        Hu3DModelShadowSet(lbl_1_bss_1080[0]);
        Hu3DModelShadowDispOff(lbl_1_bss_1080[0]);
        lbl_1_bss_F14[0] = 0.0f;
        lbl_1_bss_1168[1] += 1;
        break;
    case 4:
        lbl_1_bss_F14[0] += 100.0f;
        if (lbl_1_bss_F14[0] == 5000.0f) {
            omVibrate(0, 0x11, 0xC, 0);
            omVibrate(1, 0x11, 0xC, 0);
            omVibrate(2, 0x11, 0xC, 0);
            omVibrate(3, 0x11, 0xC, 0);
        }
        if (lbl_1_bss_F14[0] > 5000.0f) {
            lbl_1_bss_F14[0] = 5000.0f;
            Hu3DModelShadowReset(lbl_1_bss_111E[0]);
            Hu3DModelKill(lbl_1_bss_111E[0]);
            lbl_1_bss_F3C[0] = 0x64;
            lbl_1_bss_1168[1] += 1;
        }
        Hu3DModelPosSet(lbl_1_bss_1080[0], -50.0f + lbl_1_bss_E68[0].x, lbl_1_bss_E68[0].y - lbl_1_bss_F14[0], lbl_1_bss_E68[0].z);
        if (lbl_1_bss_F14[0] >= 4700.0f) {
            Hu3DModelShadowDispOn(lbl_1_bss_1080[0]);
        }
        if (lbl_1_bss_F14[0] >= 4900.0f) {
            Hu3DModelAttrSet(lbl_1_bss_111E[0], 1);
        }
        break;
    case 5:
        if (lbl_1_bss_F3C[0] == 0x64) {
            fn_1_31AC0(&lbl_1_bss_E68[1], &lbl_1_bss_E68[2]);
            fn_1_8D04(0x428, lbl_1_bss_1080[0], 1);
        }
        if (lbl_1_bss_F3C[0] == 0x37) {
            omVibrate(0, 0xF, 4, 2);
            omVibrate(1, 0xF, 4, 2);
            omVibrate(2, 0xF, 4, 2);
            omVibrate(3, 0xF, 4, 2);
        }
        if (lbl_1_bss_F3C[0] == 0xA) {
            omVibrate(0, 0xA, 6, 6);
            omVibrate(1, 0xA, 6, 6);
            omVibrate(2, 0xA, 6, 6);
            omVibrate(3, 0xA, 6, 6);
        }
        pos.x = lbl_1_bss_E68[1].x + (f32) frandmod(lbl_1_bss_F3C[0]);
        pos.y = lbl_1_bss_E68[1].y + (f32) frandmod(lbl_1_bss_F3C[0]);
        pos.z = lbl_1_bss_E68[1].z + (f32) frandmod(lbl_1_bss_F3C[0]);
        fn_1_31710(&pos);
        pos.x = lbl_1_bss_E68[2].x + (f32) frandmod(lbl_1_bss_F3C[0]);
        pos.y = lbl_1_bss_E68[2].y + (f32) frandmod(lbl_1_bss_F3C[0]);
        pos.z = lbl_1_bss_E68[2].z + (f32) frandmod(lbl_1_bss_F3C[0]);
        fn_1_31788(&pos);
        lbl_1_bss_F3C[0] -= 3;
        if (lbl_1_bss_F3C[0] < 3) {
            fn_1_31710(&lbl_1_bss_E68[1]);
            fn_1_31788(&lbl_1_bss_E68[2]);
            lbl_1_bss_F14[0] = 0.7f;
            lbl_1_bss_1168[1] += 1;
        }
        break;
    case 6:
        lbl_1_bss_F14[0] += 0.01f;
        if (lbl_1_bss_F14[0] > 0.99f) {
            lbl_1_bss_F14[0] = 0.99f;
            lbl_1_bss_F3C[0] = 0;
            fn_1_F2A8(0, 0, 0);
            lbl_1_bss_1168[1] += 1;
        }
        Hu3DModelScaleSet(lbl_1_bss_1080[0], lbl_1_bss_F14[0], 1.0f, lbl_1_bss_F14[0]);
        break;
    case 7:
        {
            s32 nr;
            s32 nc;
            s32 mask;

            switch (lbl_1_bss_F3C[0]) {
            case 0:
                nr = lbl_1_bss_F3C[8] - 1;
                nc = lbl_1_bss_F3C[9];
                mask = 1;
                break;
            case 1:
                nr = lbl_1_bss_F3C[8] + 1;
                nc = lbl_1_bss_F3C[9];
                mask = 2;
                break;
            case 2:
                nr = lbl_1_bss_F3C[8];
                nc = lbl_1_bss_F3C[9] - 1;
                mask = 4;
                break;
            default:
                nr = lbl_1_bss_F3C[8];
                nc = lbl_1_bss_F3C[9] + 1;
                mask = 8;
                break;
            }
            if ((lbl_1_bss_8C4[nr][nc] & 0x10) == 0) {
                if ((arg0->f1AC & mask) != 0) {
                    if (fn_1_F2A8(1, nr, nc) != 0) {
                        fn_1_F2A8(0, 0, 0);
                        lbl_1_bss_F3C[0] += 1;
                    }
                } else {
                    fn_1_F2A8(0, 0, 0);
                    lbl_1_bss_F3C[0] += 1;
                }
            } else {
                fn_1_F2A8(0, 0, 0);
                lbl_1_bss_F3C[0] += 1;
            }
        }
        if (lbl_1_bss_F3C[0] == 4) {
            lbl_1_bss_1168[1] += 1;
        }
        break;
    case 8:
        lbl_1_bss_E68[0].y -= 5000.0f;
        lbl_1_bss_1080[1] = Hu3DModelCreateData(lbl_1_data_B0[0x1A]);
        Hu3DModelCameraSet(lbl_1_bss_1080[1], 1);
        Hu3DModelRotSet(lbl_1_bss_1080[1], 0.0f, 0.0f, 0.0f);
        Hu3DModelPosSet(lbl_1_bss_1080[1], -50.0f + lbl_1_bss_E68[0].x, lbl_1_bss_E68[0].y - 75.0f, lbl_1_bss_E68[0].z);
        Hu3DModelAttrSet(lbl_1_bss_1080[1], 1);
        pos.x = -50.0f + lbl_1_bss_E68[0].x;
        pos.y = lbl_1_bss_E68[0].y - 75.0f;
        pos.z = lbl_1_bss_E68[0].z;
        v = pos;
        fn_1_3ED3C(0x11, &v);
        var_r30 = lbl_1_bss_F3C[8];
        var_r29 = lbl_1_bss_F3C[9];
        cardSpawnNeighbour(arg0, 0, var_r30 - 1, var_r29, 1, 0.0f, -300.0f);
        cardSpawnNeighbour(arg0, 1, var_r30 + 1, var_r29, 2, 0.0f, 300.0f);
        cardSpawnNeighbour(arg0, 2, var_r30, var_r29 - 1, 4, -300.0f, 0.0f);
        cardSpawnNeighbour(arg0, 3, var_r30, var_r29 + 1, 8, 300.0f, 0.0f);
        lbl_1_bss_F3C[0] = 0;
        lbl_1_bss_F14[0] = 0.0f;
        lbl_1_bss_1168[1] += 1;
        fn_1_8D04(0x452, lbl_1_bss_478[var_r30][var_r29], 1);
        break;
    case 9:
        var_r30 = lbl_1_bss_F3C[8];
        var_r29 = lbl_1_bss_F3C[9];
        lbl_1_bss_F14[0] += 5.0f;
        if (lbl_1_bss_F14[0] > 200.0f) {
            lbl_1_bss_F14[0] = 200.0f;
            lbl_1_bss_1168[1] += 1;
        }
        Hu3DModelPosSet(lbl_1_bss_1080[0], -50.0f + lbl_1_bss_E68[0].x, lbl_1_bss_E68[0].y - lbl_1_bss_F14[0], lbl_1_bss_E68[0].z);
        lbl_1_bss_F3C[0] += 1;
        if (lbl_1_bss_F3C[0] == 0x14) {
            Hu3DModelAttrSet(lbl_1_bss_478[var_r30][var_r29], 1);
            Hu3DModelAttrReset(lbl_1_bss_1080[1], 1);
            if (lbl_1_bss_F96[0] != -1) {
                Hu3DModelAttrSet(lbl_1_bss_478[var_r30 - 1][var_r29], 1);
                Hu3DModelAttrReset(lbl_1_bss_F96[0], 1);
            }
            if (lbl_1_bss_F96[1] != -1) {
                Hu3DModelAttrSet(lbl_1_bss_478[var_r30 + 1][var_r29], 1);
                Hu3DModelAttrReset(lbl_1_bss_F96[1], 1);
            }
            if (lbl_1_bss_F96[2] != -1) {
                Hu3DModelAttrSet(lbl_1_bss_478[var_r30][var_r29 - 1], 1);
                Hu3DModelAttrReset(lbl_1_bss_F96[2], 1);
            }
            if (lbl_1_bss_F96[3] != -1) {
                Hu3DModelAttrSet(lbl_1_bss_478[var_r30][var_r29 + 1], 1);
                Hu3DModelAttrReset(lbl_1_bss_F96[3], 1);
            }
        }
        break;
    case 10:
        var_r30 = lbl_1_bss_F3C[8];
        var_r29 = lbl_1_bss_F3C[9];
        Hu3DModelShadowReset(lbl_1_bss_1080[0]);
        Hu3DModelKill(lbl_1_bss_1080[0]);
        Hu3DModelShadowMapObjReset(lbl_1_bss_478[var_r30][var_r29]);
        Hu3DModelKill(lbl_1_bss_478[var_r30][var_r29]);
        lbl_1_bss_478[var_r30][var_r29] = lbl_1_bss_1080[1];
        Hu3DModelShadowMapSet(lbl_1_bss_478[var_r30][var_r29]);
        for (dir = 0; dir < 4; dir++) {
            s32 nr = var_r30 + (dir == 0 ? -1 : dir == 1 ? 1 : 0);
            s32 nc = var_r29 + (dir == 2 ? -1 : dir == 3 ? 1 : 0);

            if (lbl_1_bss_F96[dir] != -1) {
                Hu3DModelShadowMapObjReset(lbl_1_bss_478[nr][nc]);
                Hu3DModelKill(lbl_1_bss_478[nr][nc]);
                lbl_1_bss_478[nr][nc] = lbl_1_bss_F96[dir];
                Hu3DModelShadowMapSet(lbl_1_bss_478[nr][nc]);
            }
        }
        lbl_1_bss_1168[1] = 0;
        var_r26 = 1;
        break;
    }
    return var_r26;
}


s32 lbl_1_bss_1138;
f32 lbl_1_bss_E60[2];
static const s32 cardSeC[7] = { 0x1F9, 0x1E5, 0x25D, 0x2AD, 0x285, 0x1BD, 0x299 };

/* the cell in front of rec (according to its facing angle) and its two orientation flags */
static void cardFrontCell(CardPlayer *rec, s32 *r, s32 *c, s32 *a, s32 *b)
{
    *r = rec->row;
    *c = rec->col;
    *a = rec->f20;
    *b = rec->f24;
    if (rec->rotY == 180.0f) {
        *r += 1;
        *a = (lbl_1_bss_8C4[*r][*c] & 0x40) != 0 ? 1 : 0;
    } else if (rec->rotY == 0.0f) {
        *r -= 1;
        *a = (lbl_1_bss_8C4[*r][*c] & 0x40) != 0 ? 1 : 2;
    } else if (rec->rotY == -90.0f) {
        *c += 1;
        *b = (lbl_1_bss_8C4[*r][*c] & 0x40) != 0 ? 1 : 0;
    } else if (rec->rotY == 90.0f) {
        *c -= 1;
        *b = (lbl_1_bss_8C4[*r][*c] & 0x40) != 0 ? 1 : 2;
    }
}

/* effect position: src shifted by -50 in x and dy in y */
static void cardEffPos(Vec *dst, const Vec *src, f32 dy)
{
    dst->x = src->x;
    dst->y = src->y;
    dst->z = src->z;
    dst->x += -50.0f;
    dst->y += dy;
}

static void cardSwapSe(CardGame *game)
{
    s32 charNo = GwPlayerConf[game->order[game->curPlayer]].charNo;

    if (charNo >= 0 && charNo < 7) {
        lbl_1_bss_20[1] = cardSeC[charNo];
    }
    fn_1_8D04(lbl_1_bss_20[1], lbl_1_bss_468[game->order[game->curPlayer]], 1);
    lbl_1_bss_20[1] = 0;
}

s32 fn_1_2B280(CardGame *arg0)
{
    Vec v;
    Vec tmp;
    CardPlayer *rec;
    CardPlayer *other;
    s32 tgt;
    s32 var_r19;
    s32 var_r21;
    s32 r;
    s32 c;
    s32 a;
    s32 b;
    s32 i;
    f32 ft;
    f32 var_f31;

    var_r21 = 0;
    rec = &arg0->pl[arg0->order[arg0->curPlayer]];
    switch (lbl_1_bss_1168[1]) {
    case 0:
        if (fn_1_30FA4(7, arg0) != 0) {
            if (arg0->f1A4 == -1) {
                lbl_1_bss_1168[1] = 0xA;
            } else {
                lbl_1_bss_1168[1] += 1;
            }
        }
        break;
    case 1:
        tgt = arg0->order[arg0->f1A4];
        other = &arg0->pl[tgt];
        r = other->row;
        c = other->col;
        if (other->f00 == 1) {
            r -= 1;
        } else if (other->f00 == 2) {
            r += 1;
        } else if (other->f00 == 3) {
            c -= 1;
        } else if (other->f00 == 4) {
            c += 1;
        }
        fn_1_9C74(&lbl_1_bss_E68[1], r, c, other->f20, other->f24);
        lbl_1_bss_E60[1] = other->rotY;
        cardFrontCell(rec, &r, &c, &a, &b);
        fn_1_9C74(&lbl_1_bss_E68[0], r, c, a, b);
        lbl_1_bss_E60[0] = rec->rotY;
        lbl_1_bss_1138 = 0;
        lbl_1_bss_113C = 0;
        lbl_1_bss_F14[0] = 0.0f;
        lbl_1_bss_F14[1] = 0.0f;
        lbl_1_bss_F14[2] = 0.0f;
        lbl_1_bss_1168[1] = 5;
        lbl_1_bss_F14[3] = 0.0f;
        break;
    case 2:
        tgt = arg0->order[arg0->f1A4];
        other = &arg0->pl[tgt];
        if (lbl_1_bss_1138 == 0) {
            if (lbl_1_bss_F14[0] < 90.0f) {
                var_f31 = 5.0f + lbl_1_bss_F14[0];
            } else {
                var_f31 = 90.0f;
            }
            lbl_1_bss_F14[0] = var_f31;
            lbl_1_bss_F14[1] += lbl_1_bss_F14[0];
            Hu3DModelRotSet(lbl_1_bss_468[arg0->order[arg0->curPlayer]], 0.0f, lbl_1_bss_E60[0] + lbl_1_bss_F14[1], 0.0f);
            Hu3DModelRotSet(lbl_1_bss_468[tgt], 0.0f, lbl_1_bss_E60[1] + lbl_1_bss_F14[1], 0.0f);
            if (lbl_1_bss_F14[1] > 1000.0f) {
                if (lbl_1_bss_113C == 0) {
                    if ((rec->f50 & 2) != 0) {
                        fn_1_3F278(3, rec->f56);
                        rec->f56 = -1;
                        rec->f50 &= 0xFFFFFFFD;
                    }
                    if ((rec->f50 & 4) != 0) {
                        fn_1_3F278(4, rec->f56);
                        rec->f56 = -1;
                        rec->f50 &= 0xFFFFFFFB;
                    }
                    if ((other->f50 & 4) != 0) {
                        fn_1_3F278(4, other->f56);
                        other->f56 = -1;
                    }
                }
                lbl_1_bss_113C += 0x14;
                Hu3DModelPosSet(lbl_1_bss_468[arg0->order[arg0->curPlayer]], -50.0f + lbl_1_bss_E68[0].x,
                    (f32) lbl_1_bss_113C + (lbl_1_bss_E68[0].y + lbl_1_bss_F14[3]), lbl_1_bss_E68[0].z);
                Hu3DModelPosSet(lbl_1_bss_468[tgt], -50.0f + lbl_1_bss_E68[1].x,
                    (f32) lbl_1_bss_113C + (lbl_1_bss_E68[1].y + lbl_1_bss_F14[3]), lbl_1_bss_E68[1].z);
                if (lbl_1_bss_113C == 0x14) {
                    cardEffPos(&lbl_1_bss_E9C[0], &lbl_1_bss_E68[0], (f32) lbl_1_bss_113C);
                    v = lbl_1_bss_E9C[0];
                    lbl_1_bss_F92[0] = fn_1_3ED3C(8, &v);
                    cardEffPos(&lbl_1_bss_E9C[0], &lbl_1_bss_E68[1], (f32) lbl_1_bss_113C);
                    v = lbl_1_bss_E9C[0];
                    lbl_1_bss_F92[1] = fn_1_3ED3C(8, &v);
                    fn_1_8D04(0x3FF, lbl_1_bss_468[arg0->order[arg0->curPlayer]], 1);
                    cardSwapSe(arg0);
                } else {
                    cardEffPos(&lbl_1_bss_E9C[0], &lbl_1_bss_E68[0], (f32) lbl_1_bss_113C);
                    v = lbl_1_bss_E9C[0];
                    fn_1_3F3D8(8, lbl_1_bss_F92[0], &v);
                    cardEffPos(&lbl_1_bss_E9C[0], &lbl_1_bss_E68[1], (f32) lbl_1_bss_113C);
                    v = lbl_1_bss_E9C[0];
                    fn_1_3F3D8(8, lbl_1_bss_F92[1], &v);
                }
                Hu3DModelScaleSet(lbl_1_bss_468[arg0->order[arg0->curPlayer]], 1.0f - lbl_1_bss_F14[2], 1.0f, 1.0f - lbl_1_bss_F14[2]);
                Hu3DModelScaleSet(lbl_1_bss_468[tgt], 1.0f - lbl_1_bss_F14[2], 1.0f, 1.0f - lbl_1_bss_F14[2]);
                lbl_1_bss_F14[2] += 0.05f;
                if (lbl_1_bss_F14[2] > 1.0f) {
                    lbl_1_bss_F14[2] = 1.0f;
                }
                if (lbl_1_bss_113C == 0x3E8) {
                    fn_1_3F278(8, lbl_1_bss_F92[0]);
                    fn_1_3F278(8, lbl_1_bss_F92[1]);
                    lbl_1_bss_1138 = 1;
                    lbl_1_bss_F3C[0] = 0;
                    lbl_1_bss_1168[1] += 1;
                }
            }
        } else {
            lbl_1_bss_113C -= 0x14;
            if (lbl_1_bss_113C < 0) {
                lbl_1_bss_113C = 0;
            }
            Hu3DModelPosSet(lbl_1_bss_468[arg0->order[arg0->curPlayer]], -50.0f + lbl_1_bss_E68[0].x,
                lbl_1_bss_E68[0].y + (f32) lbl_1_bss_113C, lbl_1_bss_E68[0].z);
            Hu3DModelPosSet(lbl_1_bss_468[tgt], -50.0f + lbl_1_bss_E68[1].x, lbl_1_bss_E68[1].y + (f32) lbl_1_bss_113C,
                lbl_1_bss_E68[1].z);
            if (lbl_1_bss_113C <= 0x1F4) {
                if (lbl_1_bss_113C == 0x1F4) {
                    cardEffPos(&lbl_1_bss_E9C[0], &lbl_1_bss_E68[0], (f32) lbl_1_bss_113C);
                    v = lbl_1_bss_E9C[0];
                    lbl_1_bss_F92[0] = fn_1_3ED3C(8, &v);
                    cardEffPos(&lbl_1_bss_E9C[0], &lbl_1_bss_E68[1], (f32) lbl_1_bss_113C);
                    v = lbl_1_bss_E9C[0];
                    lbl_1_bss_F92[1] = fn_1_3ED3C(8, &v);
                    fn_1_8D04(0x400, lbl_1_bss_468[arg0->order[arg0->curPlayer]], 1);
                } else {
                    cardEffPos(&lbl_1_bss_E9C[0], &lbl_1_bss_E68[0], (f32) lbl_1_bss_113C);
                    v = lbl_1_bss_E9C[0];
                    fn_1_3F3D8(8, lbl_1_bss_F92[0], &v);
                    cardEffPos(&lbl_1_bss_E9C[0], &lbl_1_bss_E68[1], (f32) lbl_1_bss_113C);
                    v = lbl_1_bss_E9C[0];
                    fn_1_3F3D8(8, lbl_1_bss_F92[1], &v);
                }
                Hu3DModelScaleSet(lbl_1_bss_468[arg0->order[arg0->curPlayer]], 1.0f - lbl_1_bss_F14[2], 1.0f, 1.0f - lbl_1_bss_F14[2]);
                Hu3DModelScaleSet(lbl_1_bss_468[tgt], 1.0f - lbl_1_bss_F14[2], 1.0f, 1.0f - lbl_1_bss_F14[2]);
                lbl_1_bss_F14[2] -= 0.05f;
                if (lbl_1_bss_F14[2] < 0.0f) {
                    lbl_1_bss_F14[2] = 0.0f;
                }
            }
            if (lbl_1_bss_F14[1] <= 800.0f) {
                if (lbl_1_bss_F14[0] > 0.0f) {
                    ft = lbl_1_bss_F14[0] - 5.0f;
                } else {
                    ft = 0.0f;
                }
                lbl_1_bss_F14[0] = ft;
            }
            lbl_1_bss_F14[1] -= lbl_1_bss_F14[0];
            if (lbl_1_bss_F14[1] <= 0.0f) {
                lbl_1_bss_F14[1] = 0.0f;
                fn_1_3F278(8, lbl_1_bss_F92[0]);
                fn_1_3F278(8, lbl_1_bss_F92[1]);
                if ((other->f50 & 4) != 0) {
                    lbl_1_bss_E9C[0] = lbl_1_bss_E68[1];
                    lbl_1_bss_E9C[0].x += -50.0f;
                    v = lbl_1_bss_E9C[0];
                    other->f56 = fn_1_3ED3C(4, &v);
                }
                lbl_1_bss_1138 = 0;
                rec->f50 |= 8;
                lbl_1_bss_1168[1] = 4;
                var_r21 = 1;
            }
            Hu3DModelRotSet(lbl_1_bss_468[arg0->order[arg0->curPlayer]], 0.0f, lbl_1_bss_E60[0] + lbl_1_bss_F14[1], 0.0f);
            Hu3DModelRotSet(lbl_1_bss_468[tgt], 0.0f, lbl_1_bss_E60[1] + lbl_1_bss_F14[1], 0.0f);
        }
        break;
    case 3:
        lbl_1_bss_F3C[0] += 1;
        if (lbl_1_bss_F3C[0] == 0x1E) {
            fn_1_3F278(8, lbl_1_bss_F92[0]);
            fn_1_3F278(8, lbl_1_bss_F92[1]);
        }
        if (lbl_1_bss_F3C[0] >= 0x23) {
            lbl_1_bss_F3C[0] = 0x23;
            if (fn_1_30FA4(9, &lbl_1_bss_E68[1]) != 0) {
                tmp = lbl_1_bss_E68[1];
                lbl_1_bss_E68[1] = lbl_1_bss_E68[0];
                lbl_1_bss_E68[0] = tmp;
                ft = lbl_1_bss_E60[1];
                lbl_1_bss_E60[1] = lbl_1_bss_E60[0];
                lbl_1_bss_E60[0] = ft;
                lbl_1_bss_1168[1] = 2;
            }
        }
        break;
    case 4:
        tgt = arg0->order[arg0->f1A4];
        other = &arg0->pl[tgt];
        {
            CardPlayer swap;

            swap.f00 = other->f00;
            swap.row = other->row;
            swap.col = other->col;
            swap.f20 = other->f20;
            swap.f24 = other->f24;
            swap.rotY = other->rotY;
            swap.rotY2 = other->rotY2;
            other->f00 = rec->f00;
            other->row = rec->row;
            other->col = rec->col;
            other->f20 = rec->f20;
            other->f24 = rec->f24;
            other->rotY = rec->rotY;
            other->rotY2 = rec->rotY2;
            rec->f00 = swap.f00;
            rec->row = swap.row;
            rec->col = swap.col;
            rec->f20 = swap.f20;
            rec->f24 = swap.f24;
            rec->rotY = swap.rotY;
            rec->rotY2 = swap.rotY2;
        }
        ((s32 *) lbl_1_bss_12A8)[10] = 0x3C;
        ((s32 *) lbl_1_bss_12A8)[2] = GwPlayerConf[arg0->order[arg0->curPlayer]].charNo;
        ((s32 *) lbl_1_bss_12A8)[3] = GwPlayerConf[tgt].charNo;
        fn_1_4FBC8(arg0, 0x11);
        lbl_1_bss_1168[1] = 0;
        var_r21 = 1;
        break;
    case 5:
        if (lbl_1_bss_F14[3] == 0.0f) {
            fn_1_8D04(0x44F, lbl_1_bss_468[arg0->order[arg0->curPlayer]], 1);
        }
        tgt = arg0->order[arg0->f1A4];
        other = &arg0->pl[tgt];
        lbl_1_bss_F14[3] += 1.0f;
        if (lbl_1_bss_F14[3] > 50.0f) {
            lbl_1_bss_F14[3] = 50.0f;
        }
        Hu3DModelPosSet(lbl_1_bss_468[arg0->order[arg0->curPlayer]], -50.0f + lbl_1_bss_E68[0].x,
            lbl_1_bss_E68[0].y + lbl_1_bss_F14[3], lbl_1_bss_E68[0].z);
        Hu3DModelPosSet(lbl_1_bss_468[tgt], -50.0f + lbl_1_bss_E68[1].x, lbl_1_bss_E68[1].y + lbl_1_bss_F14[3],
            lbl_1_bss_E68[1].z);
        lbl_1_bss_F14[1] += 10.0f;
        Hu3DModelRotSet(lbl_1_bss_468[arg0->order[arg0->curPlayer]], 0.0f, lbl_1_bss_E60[0] + lbl_1_bss_F14[1], 0.0f);
        Hu3DModelRotSet(lbl_1_bss_468[tgt], 0.0f, lbl_1_bss_E60[1] + lbl_1_bss_F14[1], 0.0f);
        if (lbl_1_bss_F14[1] >= 720.0f) {
            lbl_1_bss_F14[1] = 0.0f;
            lbl_1_bss_1168[1] = 2;
        }
        cardEffPos(&lbl_1_bss_E9C[8], &lbl_1_bss_E68[0], lbl_1_bss_F14[3]);
        if ((rec->f50 & 2) != 0) {
            fn_1_3F3D8(3, rec->f56, &lbl_1_bss_E9C[8]);
        }
        if ((rec->f50 & 4) != 0) {
            fn_1_3F3D8(4, rec->f56, &lbl_1_bss_E9C[8]);
        }
        cardEffPos(&lbl_1_bss_E9C[9], &lbl_1_bss_E68[1], lbl_1_bss_F14[3]);
        if ((other->f50 & 2) != 0) {
            fn_1_3F3D8(3, other->f56, &lbl_1_bss_E9C[9]);
        }
        if ((other->f50 & 4) != 0) {
            fn_1_3F3D8(4, other->f56, &lbl_1_bss_E9C[9]);
        }
        break;
    case 10:
        lbl_1_bss_1168[1] += 1;
        break;
    case 11:
        var_r19 = 1;
        r = 0;
        c = 0;
        while (var_r19 != 0) {
            r = frandmod(arg0->boardSize);
            c = frandmod(arg0->boardSize);
            if ((lbl_1_bss_8C4[r][c] & 0x10) != 0 && (rec->row != r || rec->col != c)) {
                for (i = 0; i < 4; i++) {
                    CardPlayer *p = &arg0->pl[i];

                    if (r == p->row && c == p->col && p->f20 == 1 && p->f24 == 1) {
                        break;
                    }
                }
                if (i == 4) {
                    var_r19 = 0;
                }
            }
        }
        fn_1_9C74(&lbl_1_bss_E68[1], r, c, 1, 1);
        lbl_1_bss_F3C[8] = r;
        lbl_1_bss_F3C[9] = c;
        cardFrontCell(rec, &r, &c, &a, &b);
        fn_1_9C74(&lbl_1_bss_E68[0], r, c, a, b);
        lbl_1_bss_E60[0] = rec->rotY;
        lbl_1_bss_1138 = 0;
        lbl_1_bss_113C = 0;
        lbl_1_bss_F14[0] = 0.0f;
        lbl_1_bss_F14[1] = 0.0f;
        lbl_1_bss_F14[2] = 0.0f;
        lbl_1_bss_1168[1] = 0xF;
        lbl_1_bss_F14[3] = 0.0f;
        break;
    case 12:
        if (lbl_1_bss_1138 == 0) {
            if (lbl_1_bss_F14[0] < 90.0f) {
                var_f31 = 5.0f + lbl_1_bss_F14[0];
            } else {
                var_f31 = 90.0f;
            }
            lbl_1_bss_F14[0] = var_f31;
            lbl_1_bss_F14[1] += lbl_1_bss_F14[0];
            Hu3DModelRotSet(lbl_1_bss_468[arg0->order[arg0->curPlayer]], 0.0f, lbl_1_bss_E60[0] + lbl_1_bss_F14[1], 0.0f);
            if (lbl_1_bss_F14[1] > 1000.0f) {
                if (lbl_1_bss_113C == 0) {
                    if ((rec->f50 & 2) != 0) {
                        fn_1_3F278(3, rec->f56);
                        rec->f56 = -1;
                        rec->f50 &= 0xFFFFFFFD;
                    }
                    if ((rec->f50 & 4) != 0) {
                        fn_1_3F278(4, rec->f56);
                        rec->f56 = -1;
                        rec->f50 &= 0xFFFFFFFB;
                    }
                }
                lbl_1_bss_113C += 0x14;
                Hu3DModelPosSet(lbl_1_bss_468[arg0->order[arg0->curPlayer]], -50.0f + lbl_1_bss_E68[0].x,
                    (f32) lbl_1_bss_113C + (lbl_1_bss_E68[0].y + lbl_1_bss_F14[3]), lbl_1_bss_E68[0].z);
                if (lbl_1_bss_113C == 0x14) {
                    cardEffPos(&lbl_1_bss_E9C[0], &lbl_1_bss_E68[0], (f32) lbl_1_bss_113C);
                    v = lbl_1_bss_E9C[0];
                    lbl_1_bss_F92[0] = fn_1_3ED3C(8, &v);
                    fn_1_8D04(0x3FF, lbl_1_bss_468[arg0->order[arg0->curPlayer]], 1);
                    cardSwapSe(arg0);
                } else {
                    cardEffPos(&lbl_1_bss_E9C[0], &lbl_1_bss_E68[0], (f32) lbl_1_bss_113C);
                    v = lbl_1_bss_E9C[0];
                    fn_1_3F3D8(8, lbl_1_bss_F92[0], &v);
                }
                Hu3DModelScaleSet(lbl_1_bss_468[arg0->order[arg0->curPlayer]], 1.0f - lbl_1_bss_F14[2], 1.0f, 1.0f - lbl_1_bss_F14[2]);
                lbl_1_bss_F14[2] += 0.05f;
                if (lbl_1_bss_F14[2] > 1.0f) {
                    lbl_1_bss_F14[2] = 1.0f;
                }
                if (lbl_1_bss_113C == 0x3E8) {
                    fn_1_3F278(8, lbl_1_bss_F92[0]);
                    lbl_1_bss_1138 = 1;
                    lbl_1_bss_F3C[0] = 0;
                    lbl_1_bss_1168[1] += 1;
                }
            }
        } else {
            lbl_1_bss_113C -= 0x14;
            if (lbl_1_bss_113C < 0) {
                lbl_1_bss_113C = 0;
            }
            Hu3DModelPosSet(lbl_1_bss_468[arg0->order[arg0->curPlayer]], -50.0f + lbl_1_bss_E68[0].x,
                lbl_1_bss_E68[0].y + (f32) lbl_1_bss_113C, lbl_1_bss_E68[0].z);
            if (lbl_1_bss_113C <= 0x1F4) {
                if (lbl_1_bss_113C == 0x1F4) {
                    cardEffPos(&lbl_1_bss_E9C[0], &lbl_1_bss_E68[0], (f32) lbl_1_bss_113C);
                    v = lbl_1_bss_E9C[0];
                    lbl_1_bss_F92[0] = fn_1_3ED3C(8, &v);
                    fn_1_8D04(0x400, lbl_1_bss_468[arg0->order[arg0->curPlayer]], 1);
                } else {
                    cardEffPos(&lbl_1_bss_E9C[0], &lbl_1_bss_E68[0], (f32) lbl_1_bss_113C);
                    v = lbl_1_bss_E9C[0];
                    fn_1_3F3D8(8, lbl_1_bss_F92[0], &v);
                }
                Hu3DModelScaleSet(lbl_1_bss_468[arg0->order[arg0->curPlayer]], 1.0f - lbl_1_bss_F14[2], 1.0f, 1.0f - lbl_1_bss_F14[2]);
                lbl_1_bss_F14[2] -= 0.05f;
                if (lbl_1_bss_F14[2] < 0.0f) {
                    lbl_1_bss_F14[2] = 0.0f;
                }
            }
            if (lbl_1_bss_F14[1] <= 800.0f) {
                if (lbl_1_bss_F14[0] > 0.0f) {
                    ft = lbl_1_bss_F14[0] - 5.0f;
                } else {
                    ft = 0.0f;
                }
                lbl_1_bss_F14[0] = ft;
            }
            lbl_1_bss_F14[1] -= lbl_1_bss_F14[0];
            if (lbl_1_bss_F14[1] <= 0.0f) {
                lbl_1_bss_F14[1] = 0.0f;
                fn_1_3F278(8, lbl_1_bss_F92[0]);
                lbl_1_bss_1138 = 0;
                rec->f50 |= 8;
                lbl_1_bss_1168[1] = 0xE;
                var_r21 = 1;
            }
            Hu3DModelRotSet(lbl_1_bss_468[arg0->order[arg0->curPlayer]], 0.0f, lbl_1_bss_E60[0] + lbl_1_bss_F14[1], 0.0f);
        }
        break;
    case 13:
        lbl_1_bss_F3C[0] += 1;
        if (lbl_1_bss_F3C[0] == 0x1E) {
            fn_1_3F278(8, lbl_1_bss_F92[0]);
            fn_1_3F278(8, lbl_1_bss_F92[0]);
        }
        if (lbl_1_bss_F3C[0] >= 0x23) {
            lbl_1_bss_F3C[0] = 0x23;
            if (fn_1_30FA4(9, &lbl_1_bss_E68[1]) != 0) {
                lbl_1_bss_E68[0] = lbl_1_bss_E68[1];
                lbl_1_bss_1168[1] = 0xC;
            }
        }
        break;
    case 14:
        rec->row = lbl_1_bss_F3C[8];
        rec->col = lbl_1_bss_F3C[9];
        rec->f20 = 1;
        rec->f24 = 1;
        ((s32 *) lbl_1_bss_12A8)[10] = 0x3C;
        ((s32 *) lbl_1_bss_12A8)[2] = GwPlayerConf[arg0->order[arg0->curPlayer]].charNo;
        fn_1_4FBC8(arg0, 0x3C);
        lbl_1_bss_1168[1] = 0;
        var_r21 = 1;
        break;
    case 15:
        if (lbl_1_bss_F14[3] == 0.0f) {
            fn_1_8D04(0x44F, lbl_1_bss_468[arg0->order[arg0->curPlayer]], 1);
        }
        lbl_1_bss_F14[3] += 1.0f;
        if (lbl_1_bss_F14[3] > 50.0f) {
            lbl_1_bss_F14[3] = 50.0f;
        }
        Hu3DModelPosSet(lbl_1_bss_468[arg0->order[arg0->curPlayer]], -50.0f + lbl_1_bss_E68[0].x,
            lbl_1_bss_E68[0].y + lbl_1_bss_F14[3], lbl_1_bss_E68[0].z);
        lbl_1_bss_F14[1] += 10.0f;
        Hu3DModelRotSet(lbl_1_bss_468[arg0->order[arg0->curPlayer]], 0.0f, lbl_1_bss_E60[0] + lbl_1_bss_F14[1], 0.0f);
        if (lbl_1_bss_F14[1] >= 720.0f) {
            lbl_1_bss_F14[1] = 0.0f;
            lbl_1_bss_1168[1] = 0xC;
        }
        cardEffPos(&lbl_1_bss_E9C[8], &lbl_1_bss_E68[0], lbl_1_bss_F14[3]);
        if ((rec->f50 & 2) != 0) {
            fn_1_3F3D8(3, rec->f56, &lbl_1_bss_E9C[8]);
        }
        if ((rec->f50 & 4) != 0) {
            fn_1_3F3D8(4, rec->f56, &lbl_1_bss_E9C[8]);
        }
        break;
    }
    return var_r21;
}


s32 lbl_1_bss_C3C;
s32 lbl_1_bss_E98;
extern void fn_1_4FFEC(void);

/* x offset of card k when n cards are shown side by side */
static f32 cardPopupDx(s32 n, s32 k)
{
    if (n == 2) {
        return k == 0 ? -80.0f : 80.0f;
    }
    if (n == 3) {
        return k == 0 ? -160.0f : (k == 1 ? 0.0f : 160.0f);
    }
    return 0.0f;
}

static s32 cardSlotType(CardPlayer *p, s32 i)
{
    if (i == 0) {
        return p->f34;
    }
    if (i == 1) {
        return p->f38;
    }
    return p->f3C;
}

static s32 cardSlotModel(s32 type)
{
    if (type == 0) {
        return 0x1A;
    }
    if (type == 1) {
        return 0x18;
    }
    return 0x19;
}

static f32 cardSpinStep(f32 ang)
{
    if (ang < 1080.0f) {
        return 108.0f;
    }
    if (ang < 1800.0f) {
        return 72.0f;
    }
    if (ang < 2160.0f) {
        return 36.0f;
    }
    return 18.0f;
}

/* place the 3D point (sx + dx, 370 + sy, sz) in E68[slot] and start the 2D slide of sprite slot toward the HUD table entry */
static void cardTarget2D(CardGame *game, const Vec *base, s32 slot, f32 dx, s32 table, s32 e54First)
{
    Vec a;
    Vec b;
    f32 hx;
    f32 hy;

    lbl_1_bss_E68[slot].x = (-50.0f + base->x) + dx;
    lbl_1_bss_E68[slot].y = 370.0f + base->y;
    lbl_1_bss_E68[slot].z = base->z;
    Hu3D3Dto2D(&lbl_1_bss_E68[slot], 1, &lbl_1_bss_E54);
    if (table == 0) {
        hx = lbl_1_data_548[game->curPlayer][0];
        hy = lbl_1_data_548[game->curPlayer][1];
    } else if (table == 1) {
        hx = lbl_1_data_568[game->curPlayer][0];
        hy = lbl_1_data_568[game->curPlayer][1];
    } else {
        hx = lbl_1_data_588[game->curPlayer][0];
        hy = lbl_1_data_588[game->curPlayer][1];
    }
    lbl_1_bss_E9C[0].x = hx - 2.8f;
    lbl_1_bss_E9C[0].y = hy - 2.5f;
    lbl_1_bss_E9C[0].z = lbl_1_bss_E68[slot].z;
    a = lbl_1_bss_E54;
    b = lbl_1_bss_E9C[0];
    if (e54First) {
        fn_1_4F144(slot, &a, &b, 0x28, 0.0f, 0x21);
    } else {
        fn_1_4F144(slot, &b, &a, 0x28, 0.0f, 0x21);
    }
}

/* put popup model k at the projected position of E68[slot] */
static void cardPopupPlace(s32 k, s32 slot, Vec *cur2d, f32 *scale)
{
    Vec v;

    fn_1_4F1EC(slot, cur2d);
    v = lbl_1_bss_E68[slot];
    fn_1_4EDB8(&lbl_1_bss_E9C[0], &v, cur2d->x, cur2d->y);
    Hu3DModelPosSetV(lbl_1_bss_1080[k], &lbl_1_bss_E9C[0]);
    if (scale != NULL) {
        Hu3DModelScaleSet(lbl_1_bss_1080[k], *scale, *scale, *scale);
    }
}

static void cardPopupKill(CardPlayer *rec)
{
    Hu3DLLightKill(lbl_1_bss_1080[0], lbl_1_bss_1056[0]);
    Hu3DModelKill(lbl_1_bss_1080[0]);
    if (rec->f38 != -1) {
        Hu3DLLightKill(lbl_1_bss_1080[1], lbl_1_bss_1056[1]);
        Hu3DModelKill(lbl_1_bss_1080[1]);
        if (rec->f3C != -1) {
            Hu3DLLightKill(lbl_1_bss_1080[2], lbl_1_bss_1056[2]);
            Hu3DModelKill(lbl_1_bss_1080[2]);
        }
    }
    Hu3DLLightKill(lbl_1_bss_111E[0], lbl_1_bss_1056[5]);
    Hu3DModelKill(lbl_1_bss_111E[0]);
}

s32 fn_1_1B2F0(CardGame *arg0)
{
    Vec pos;
    Vec cur2d;
    CardPlayer *rec;
    s32 var_r29;
    s32 k;
    s32 j;
    s32 sel;
    s32 n;
    s32 ids[3];
    f32 dx;
    f32 sc;

    var_r29 = 0;
    rec = &arg0->pl[arg0->order[arg0->curPlayer]];
    fn_1_9DA8(&pos, arg0);
    if (rec->f34 == -1) {
        var_r29 = 1;
    } else if ((rec->f50 & 0x20) != 0) {
        var_r29 = 1;
    } else if (lbl_1_bss_1168[0] <= 0xB) {
        switch (lbl_1_bss_1168[0]) {
        case 0:
            if (fn_1_30FA4(7, arg0) != 0) {
                lbl_1_bss_E98 = 0;
                lbl_1_bss_1168[0] = 1;
                lbl_1_bss_C40 = -1;
                lbl_1_bss_C3C = 0;
            }
            break;
        case 1:
            lbl_1_bss_1080[0] = Hu3DModelCreateData(lbl_1_data_B0[cardSlotModel(rec->f34)]);
            Hu3DModelCameraSet(lbl_1_bss_1080[0], 1);
            Hu3DModelPosSet(lbl_1_bss_1080[0], -50.0f + pos.x, 370.0f + pos.y, pos.z);
            Hu3DModelRotSet(lbl_1_bss_1080[0], 90.0f, 0.0f, 0.0f);
            Hu3DModelAttrSet(lbl_1_bss_1080[0], 1);
            Hu3DModelScaleSet(lbl_1_bss_1080[0], 0.15f, 0.15f, 0.15f);
            Hu3DCamera[1] = Hu3DCamera[0];
            Hu3DModelLayerSet(lbl_1_bss_1080[0], 7);
            Hu3DModelCameraSet(lbl_1_bss_1080[0], 2);
            lbl_1_bss_E98 += 1;
            if (rec->f38 != -1) {
                lbl_1_bss_1080[1] = Hu3DModelCreateData(lbl_1_data_B0[cardSlotModel(rec->f38)]);
                Hu3DModelCameraSet(lbl_1_bss_1080[1], 1);
                Hu3DModelPosSet(lbl_1_bss_1080[1], -50.0f + pos.x, 370.0f + pos.y, pos.z);
                Hu3DModelRotSet(lbl_1_bss_1080[1], 90.0f, 0.0f, 0.0f);
                Hu3DModelAttrSet(lbl_1_bss_1080[1], 1);
                Hu3DModelScaleSet(lbl_1_bss_1080[1], 0.15f, 0.15f, 0.15f);
                Hu3DCamera[1] = Hu3DCamera[0];
                Hu3DModelLayerSet(lbl_1_bss_1080[1], 7);
                Hu3DModelCameraSet(lbl_1_bss_1080[1], 2);
                lbl_1_bss_E98 += 1;
                if (rec->f3C != -1) {
                    lbl_1_bss_1080[2] = Hu3DModelCreateData(lbl_1_data_B0[cardSlotModel(rec->f3C)]);
                    Hu3DModelCameraSet(lbl_1_bss_1080[2], 1);
                    Hu3DModelPosSet(lbl_1_bss_1080[2], -50.0f + pos.x, 370.0f + pos.y, pos.z);
                    Hu3DModelRotSet(lbl_1_bss_1080[2], 90.0f, 0.0f, 0.0f);
                    Hu3DModelAttrSet(lbl_1_bss_1080[2], 1);
                    Hu3DModelScaleSet(lbl_1_bss_1080[2], 0.15f, 0.15f, 0.15f);
                    Hu3DCamera[1] = Hu3DCamera[0];
                    Hu3DModelLayerSet(lbl_1_bss_1080[2], 7);
                    Hu3DModelCameraSet(lbl_1_bss_1080[2], 2);
                    lbl_1_bss_E98 += 1;
                }
            }
            n = lbl_1_bss_E98;
            lbl_1_bss_111E[0] = Hu3DModelCreateData(lbl_1_data_B0[GwPlayerConf[arg0->order[arg0->curPlayer]].charNo + 0x50]);
            Hu3DModelCameraSet(lbl_1_bss_111E[0], 1);
            Hu3DModelPosSet(lbl_1_bss_111E[0], (-50.0f + pos.x) - 50.0f, 322.0f + pos.y, 100.0f + pos.z);
            Hu3DModelRotSet(lbl_1_bss_111E[0], 90.0f, 0.0f, 0.0f);
            Hu3DModelAttrSet(lbl_1_bss_111E[0], 1);
            Hu3DModelScaleSet(lbl_1_bss_111E[0], 0.7f, 0.7f, 0.7f);
            Hu3DCamera[1] = Hu3DCamera[0];
            Hu3DModelLayerSet(lbl_1_bss_111E[0], 7);
            Hu3DModelCameraSet(lbl_1_bss_111E[0], 2);
            lbl_1_bss_1056[5] = Hu3DLLightCreate(lbl_1_bss_111E[0], (-50.0f + pos.x) - 50.0f, 370.0f + pos.y, 1000.0f + pos.z, 0.0f,
                0.0f, -1.0f, 0x80, 0x80, 0x80);
            Hu3DLLightStaticSet(lbl_1_bss_111E[0], lbl_1_bss_1056[5], 1);
            Hu3DLLightInfinitytSet(lbl_1_bss_111E[0], lbl_1_bss_1056[5]);
            if (n >= 2) {
                for (k = 0; k < n; k++) {
                    Hu3DModelPosSet(lbl_1_bss_1080[k], (-50.0f + pos.x) + cardPopupDx(n, k), 370.0f + pos.y, pos.z);
                }
                Hu3DModelPosSet(lbl_1_bss_111E[0], ((-50.0f + pos.x) - 50.0f) + cardPopupDx(n, 0), 322.0f + pos.y, 100.0f + pos.z);
            }
            for (k = 0; k < n; k++) {
                lbl_1_bss_1056[k] = Hu3DLLightCreate(lbl_1_bss_1080[k], (-50.0f + pos.x) + cardPopupDx(n, k), 370.0f + pos.y,
                    1000.0f + pos.z, 0.0f, 0.0f, -1.0f, 0x80, 0x80, 0x80);
                Hu3DLLightStaticSet(lbl_1_bss_1080[k], lbl_1_bss_1056[k], 1);
                Hu3DLLightInfinitytSet(lbl_1_bss_1080[k], lbl_1_bss_1056[k]);
            }
            for (k = 0; k < n; k++) {
                cardTarget2D(arg0, &pos, k, cardPopupDx(n, k), k, 0);
            }
            for (k = 0; k < n; k++) {
                Hu3DModelAttrReset(lbl_1_bss_1080[k], 1);
            }
            for (k = 0; k < n; k++) {
                cardPopupPlace(k, k, &cur2d, NULL);
                Hu3DModelRotSet(lbl_1_bss_1080[k], 90.0f, 0.0f, 0.0f);
            }
            lbl_1_bss_F14[0] = 0.0f;
            lbl_1_bss_F14[1] = 0.15f;
            lbl_1_bss_1138 = 0;
            lbl_1_bss_1168[0] = 2;
            lbl_1_bss_5E = 0;
            Hu3DModelAttrReset(lbl_1_bss_5C, 1);
            Hu3DZClearLayerSet(0xF);
            fn_1_303E0(arg0);
            break;
        case 2:
            n = lbl_1_bss_E98;
            sel = (n == 1) ? 0 : 1;
            if (lbl_1_bss_1138 == 0) {
                if (lbl_1_bss_5E <= 0x8C) {
                    lbl_1_bss_5E = lbl_1_bss_5E + 0xA;
                } else {
                    lbl_1_bss_5E = 0x96;
                }
                if ((rec->f50 & 0x1000) == 0) {
                    fn_1_305B4(arg0, lbl_1_bss_5E);
                }
                if (lbl_1_bss_F14[0] == 0.0f) {
                    fn_1_8D04(0x40B, lbl_1_bss_1080[sel], 1);
                }
                fn_1_8D04(0x840B, lbl_1_bss_1080[sel], 1);
                lbl_1_bss_F14[0] += cardSpinStep(lbl_1_bss_F14[0]);
                if (lbl_1_bss_F14[0] >= 2880.0f) {
                    lbl_1_bss_F14[0] = 2880.0f;
                }
                lbl_1_bss_F14[1] += 0.0125f;
                if (lbl_1_bss_F14[1] > 0.5f) {
                    lbl_1_bss_F14[1] = 0.5f;
                }
                for (k = 0; k < n; k++) {
                    cardPopupPlace(k, k, &cur2d, NULL);
                    Hu3DModelRotSet(lbl_1_bss_1080[k], 90.0f, lbl_1_bss_F14[0], 0.0f);
                    Hu3DModelScaleSet(lbl_1_bss_1080[k], lbl_1_bss_F14[1], lbl_1_bss_F14[1], lbl_1_bss_F14[1]);
                }
                if (lbl_1_bss_F14[0] == 2880.0f) {
                    lbl_1_bss_1138 = 1;
                    arg0->f1A4 = 0;
                    Hu3DModelAttrReset(lbl_1_bss_111E[0], 1);
                    lbl_1_bss_1168[0] = 3;
                    ((s32 *) lbl_1_bss_12A8)[0] = 0;
                    ((s32 *) lbl_1_bss_12A8)[1] = 0;
                }
            } else {
                if (lbl_1_bss_F14[0] == 2880.0f) {
                    fn_1_8D04(0x40C, lbl_1_bss_1080[sel], 1);
                }
                fn_1_8D04(0x840C, lbl_1_bss_1080[sel], 1);
                lbl_1_bss_F14[0] -= cardSpinStep(lbl_1_bss_F14[0]);
                for (k = 0; k < n; k++) {
                    Hu3DModelRotSet(lbl_1_bss_1080[k], 90.0f, -lbl_1_bss_F14[0], 0.0f);
                }
                if (lbl_1_bss_F14[0] < 2160.0f) {
                    if (lbl_1_bss_5E >= 0xA) {
                        lbl_1_bss_5E = lbl_1_bss_5E - 0xA;
                    } else {
                        lbl_1_bss_5E = 0;
                    }
                    if ((rec->f50 & 0x1000) == 0) {
                        fn_1_305B4(arg0, lbl_1_bss_5E);
                    }
                    lbl_1_bss_F14[1] += 0.0125f;
                    if (lbl_1_bss_F14[1] > 0.35f) {
                        lbl_1_bss_F14[1] = 0.35f;
                    }
                    sc = 0.5f - lbl_1_bss_F14[1];
                    for (k = 0; k < n; k++) {
                        cardPopupPlace(k, k, &cur2d, &sc);
                    }
                    lbl_1_bss_F3C[0] += 1;
                    if (lbl_1_bss_F3C[0] > 0x28) {
                        cardPopupKill(rec);
                        Hu3DModelAttrSet(lbl_1_bss_5C, 1);
                        Hu3DLayerHookSet(0xF, NULL);
                        fn_1_2FFAC(arg0);
                        lbl_1_bss_1138 = 1;
                        lbl_1_bss_1168[0] = 0;
                        var_r29 = 1;
                    }
                }
            }
            break;
        case 3:
            if (GwPlayerConf[arg0->order[arg0->curPlayer]].type == 0) {
                ((s32 *) lbl_1_bss_12A8)[11] = 1;
                fn_1_502E0(0x13);
            }
            if (lbl_1_bss_C44 != 0) {
                if (cardSlotType(rec, arg0->f1A4) == 0) {
                    ((s32 *) lbl_1_bss_12A8)[11] = 1;
                    fn_1_4FBFC(arg0, 0x1F);
                } else if (cardSlotType(rec, arg0->f1A4) == 1) {
                    ((s32 *) lbl_1_bss_12A8)[11] = 1;
                    fn_1_4FBFC(arg0, 0x1D);
                } else if (cardSlotType(rec, arg0->f1A4) == 2) {
                    ((s32 *) lbl_1_bss_12A8)[11] = 1;
                    fn_1_4FBFC(arg0, 0x1E);
                }
                ((s32 *) lbl_1_bss_12A8)[0] = 1;
                lbl_1_bss_C44 = 0;
            }
            fn_1_20DE4(arg0->order[arg0->curPlayer], arg0);
            if ((HuPadDStkRep[arg0->order[arg0->curPlayer]] & 1) != 0) {
                if (arg0->f1A4 == 0) {
                    if (rec->f3C != -1) {
                        arg0->f1A4 = 2;
                    } else if (rec->f38 != -1) {
                        arg0->f1A4 = 1;
                    }
                } else {
                    arg0->f1A4 -= 1;
                }
                lbl_1_bss_C44 = 1;
                if (lbl_1_bss_E98 > 1) {
                    HuAudFXPlay(0);
                }
            } else if ((HuPadDStkRep[arg0->order[arg0->curPlayer]] & 2) != 0) {
                switch (arg0->f1A4) {
                case 0:
                    if (rec->f38 != -1) {
                        arg0->f1A4 = 1;
                    }
                    break;
                case 1:
                    if (rec->f3C != -1) {
                        arg0->f1A4 = 2;
                    } else {
                        arg0->f1A4 = 0;
                    }
                    break;
                case 2:
                    arg0->f1A4 = 0;
                    break;
                }
                lbl_1_bss_C44 = 1;
                if (lbl_1_bss_E98 > 1) {
                    HuAudFXPlay(0);
                }
            } else if ((HuPadBtnDown[arg0->order[arg0->curPlayer]] & 0x100) != 0) {
                if (cardSlotType(rec, arg0->f1A4) == 0 && fn_1_25624(arg0) != 0) {
                    HuAudFXPlay(4);
                    fn_1_4FBC8(arg0, 0x28);
                    ((s32 *) lbl_1_bss_12A8)[0] = 1;
                    ((s32 *) lbl_1_bss_12A8)[11] = 1;
                    fn_1_4FBFC(arg0, 0x1F);
                    ((s32 *) lbl_1_bss_12A8)[0] = 1;
                } else {
                    fn_1_50460();
                    lbl_1_bss_113C = 0;
                    lbl_1_bss_F14[1] = 0.0f;
                    lbl_1_bss_1168[0] = 4;
                    lbl_1_bss_F3C[0] = 1;
                    switch (arg0->f1A4) {
                    case 0:
                        lbl_1_bss_F3C[8] = 1;
                        lbl_1_bss_F3C[9] = 2;
                        break;
                    case 1:
                        lbl_1_bss_F3C[8] = 0;
                        lbl_1_bss_F3C[9] = 2;
                        break;
                    case 2:
                        lbl_1_bss_F3C[8] = 0;
                        lbl_1_bss_F3C[9] = 1;
                        break;
                    }
                    lbl_1_bss_C44 = 1;
                    ((s32 *) lbl_1_bss_12A8)[0] = 0;
                    fn_1_4FFEC();
                    HuAudFXPlay(2);
                    ((s32 *) lbl_1_bss_12A8)[10] = 0x3C;
                    /* fn_1_502DC is a bare blr in the original: it returns its first argument (the char number). */
                    ((s32 *) lbl_1_bss_12A8)[2] = GwPlayerConf[arg0->order[arg0->curPlayer]].charNo;
                    if (cardSlotType(rec, arg0->f1A4) == 0) {
                        fn_1_4FBC8(arg0, 0xE);
                    } else if (cardSlotType(rec, arg0->f1A4) == 1) {
                        fn_1_4FBC8(arg0, 0xA);
                    } else if (cardSlotType(rec, arg0->f1A4) == 2) {
                        fn_1_4FBC8(arg0, 0xC);
                    }
                    ((s32 *) lbl_1_bss_12A8)[1] = 1;
                    Hu3DModelAttrSet(lbl_1_bss_111E[0], 1);
                    if (cardSlotType(rec, arg0->f1A4) == 0) {
                        lbl_1_bss_C3C = 1;
                    }
                    HuAudSStreamVolSet(0, 0x7F, 0x64);
                }
            } else if ((HuPadBtnDown[arg0->order[arg0->curPlayer]] & 0x200) != 0) {
                HuAudFXPlay(0x3FC);
                fn_1_50460();
                lbl_1_bss_1168[0] = 4;
                Hu3DModelAttrSet(lbl_1_bss_111E[0], 1);
                lbl_1_bss_F3C[0] = 0;
                lbl_1_bss_C44 = 1;
                ((s32 *) lbl_1_bss_12A8)[0] = 0;
                fn_1_4FFEC();
                ((s32 *) lbl_1_bss_12A8)[1] = 1;
                HuAudSStreamVolSet(0, 0x7F, 0x1F4);
            }
            n = lbl_1_bss_E98;
            if (n == 1) {
                Hu3DModelPosSet(lbl_1_bss_111E[0], (-50.0f + pos.x) - 50.0f, 322.0f + pos.y, 100.0f + pos.z);
            } else if (n == 2) {
                Hu3DModelPosSet(lbl_1_bss_111E[0], (((-50.0f + pos.x) - 50.0f) - 80.0f) + (160.0f * (f32) arg0->f1A4), 322.0f + pos.y,
                    100.0f + pos.z);
            } else if (n == 3) {
                Hu3DModelPosSet(lbl_1_bss_111E[0], (((-50.0f + pos.x) - 50.0f) - 160.0f) + (160.0f * (f32) arg0->f1A4), 322.0f + pos.y,
                    100.0f + pos.z);
            }
            break;
        case 4:
            n = lbl_1_bss_E98;
            if (lbl_1_bss_F3C[0] != 0) {
                /* the chosen card is used: slide the remaining ones to the HUD */
                if (n != 1) {
                    j = 0;
                    for (k = 0; k < n; k++) {
                        if (k != arg0->f1A4) {
                            cardTarget2D(arg0, &pos, j, cardPopupDx(n, k), j, 1);
                            j += 1;
                        }
                    }
                }
                lbl_1_bss_113C = 0;
                lbl_1_bss_F3C[1] = 0;
                lbl_1_bss_F14[0] = 2880.0f;
                lbl_1_bss_F14[1] = 0.0f;
                lbl_1_bss_1168[0] = 5;
            } else {
                for (k = 0; k < n; k++) {
                    cardTarget2D(arg0, &pos, k, cardPopupDx(n, k), k, 1);
                }
                lbl_1_bss_F3C[1] = 0;
                lbl_1_bss_F14[0] = 2880.0f;
                lbl_1_bss_F14[1] = 0.0f;
                lbl_1_bss_1168[0] = 2;
            }
            break;
        case 5:
            n = lbl_1_bss_E98;
            sel = (n == 1) ? 0 : 1;
            if (lbl_1_bss_F14[0] == 2880.0f) {
                fn_1_8D04(0x40C, lbl_1_bss_1080[sel], 1);
            }
            fn_1_8D04(0x840C, lbl_1_bss_1080[sel], 1);
            lbl_1_bss_F14[0] -= cardSpinStep(lbl_1_bss_F14[0]);
            for (k = 0; k < n; k++) {
                Hu3DModelRotSet(lbl_1_bss_1080[k], 90.0f, -lbl_1_bss_F14[0], 0.0f);
            }
            if (lbl_1_bss_F14[0] < 2160.0f) {
                if (lbl_1_bss_5E >= 0xA) {
                    lbl_1_bss_5E = lbl_1_bss_5E - 0xA;
                } else {
                    lbl_1_bss_5E = 0;
                }
                if ((rec->f50 & 0x1000) == 0) {
                    fn_1_305B4(arg0, lbl_1_bss_5E);
                }
                lbl_1_bss_113C += 0x1E;
                if (lbl_1_bss_113C > 0xBB8) {
                    lbl_1_bss_113C = 0xBB8;
                }
                dx = cardPopupDx(n, arg0->f1A4);
                lbl_1_bss_F14[2] = (-50.0f + pos.x) + dx;
                Hu3DModelPosSet(lbl_1_bss_1080[arg0->f1A4], lbl_1_bss_F14[2], 370.0f + pos.y + (f32) lbl_1_bss_113C, pos.z);
                lbl_1_bss_F14[1] += 0.0125f;
                if (lbl_1_bss_F14[1] > 0.35f) {
                    lbl_1_bss_F14[1] = 0.35f;
                }
                if (n != 1) {
                    sc = 0.5f - lbl_1_bss_F14[1];
                    if (n == 2) {
                        cardPopupPlace(lbl_1_bss_F3C[8], 0, &cur2d, &sc);
                    } else if (n == 3) {
                        cardPopupPlace(lbl_1_bss_F3C[8], 0, &cur2d, &sc);
                        cardPopupPlace(lbl_1_bss_F3C[9], 1, &cur2d, &sc);
                    }
                }
                lbl_1_bss_F3C[1] += 1;
                if (lbl_1_bss_F3C[1] > 0x28) {
                    cardPopupKill(rec);
                    if (cardSlotType(rec, arg0->f1A4) == 0) {
                        lbl_1_bss_1168[0] = 0xA;
                    } else if (cardSlotType(rec, arg0->f1A4) == 1) {
                        lbl_1_bss_1168[0] = 6;
                    } else if (cardSlotType(rec, arg0->f1A4) == 2) {
                        lbl_1_bss_1168[0] = 7;
                    }
                    Hu3DModelAttrSet(lbl_1_bss_5C, 1);
                    Hu3DLayerHookSet(0xF, NULL);
                    if (lbl_1_bss_F3C[0] != 0) {
                        ids[0] = rec->f34;
                        ids[1] = rec->f38;
                        ids[2] = rec->f3C;
                        switch (arg0->f1A4) {
                        case 0:
                            ids[0] = ids[1];
                            ids[1] = ids[2];
                            ids[2] = -1;
                            break;
                        case 1:
                            ids[1] = ids[2];
                            ids[2] = -1;
                            break;
                        case 2:
                            ids[2] = -1;
                            break;
                        }
                        fn_1_304D8(arg0, ids);
                    }
                }
            }
            break;
        case 6:
            if (fn_1_21CA4(0, arg0) != 0) {
                lbl_1_bss_1168[0] = 0xB;
            }
            break;
        case 7:
            if (fn_1_21CA4(1, arg0) != 0) {
                lbl_1_bss_1168[0] = 8;
            }
            break;
        case 8:
            if (fn_1_227A4(1, arg0) != 0) {
                lbl_1_bss_1168[0] = 9;
            }
            break;
        case 9:
            if (fn_1_21CA4(-1, arg0) != 0) {
                lbl_1_bss_1168[0] = 0xB;
            }
            break;
        case 10:
            if (fn_1_258E4(arg0) != 0) {
                lbl_1_bss_1168[0] = 0xB;
            }
            break;
        case 11:
            switch (arg0->f1A4) {
            case 0:
                rec->f34 = rec->f38;
                rec->f38 = rec->f3C;
                rec->f3C = -1;
                break;
            case 1:
                rec->f38 = rec->f3C;
                rec->f3C = -1;
                break;
            case 2:
                rec->f3C = -1;
                break;
            }
            fn_1_2FFAC(arg0);
            rec->f50 |= 0x20;
            lbl_1_bss_1168[0] = 0;
            var_r29 = 1;
            break;
        }
    }
    return var_r29;
}
