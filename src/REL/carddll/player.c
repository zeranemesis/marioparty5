// carddll/player.c - decompilation of the carddll/player unit (136164 B, 43 fns).
// Real code is being added function by function (smallest first), each verified
// at 100% with tools/fndiff.py before the next one. decomp/player.m2c.c is the
// reference draft (raw mips2c output, not buildable as-is).

#include "types.h"
#include "game/process.h"
#include "game/audio.h"
#include "game/hsfex.h"

/* ---- bss globals (carddll/player) ---- */
static s32 lbl_1_bss_8[3];
static s32 lbl_1_bss_1174;
static s32 lbl_1_bss_1168[3];
static s32 lbl_1_bss_C48[2];
static s32 lbl_1_bss_C44;
static s32 lbl_1_bss_1298[3];
static HUPROCESS *lbl_1_bss_11DC;

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
    PSMTXRotRad(rot, 'x', 0.017453292f * arg0[0]);
    PSMTXConcat(id, rot, id);
    PSMTXRotRad(rot, 'z', 0.017453292f * arg0[2]);
    PSMTXConcat(id, rot, id);
    PSMTXRotRad(rot, 'y', 0.017453292f * arg0[1]);
    PSMTXConcat(id, rot, id);
    Hu3DMtxRotGet(id, arg1);
    return &id;
}
