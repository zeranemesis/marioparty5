// m550dll/game.c - decompilation of the m550dll/game unit.
// Functions are written smallest-first, each verified against the original
// with tools/fndiff.py.

#include "types.h"

#include "game/object.h"
#include "game/wipe.h"

#include "math.h"

/* lbl_1_bss_14AC: the module-wide game state block (0xA14 bytes).
   Only the fields the decompiled functions touch are named. */
typedef struct GameState_s {
    /* 0x000 */ u8 unk000[0x3C];
    /* 0x03C */ void *unk03C;
    /* 0x040 */ u8 unk040[0xC0];
    /* 0x100 */ s32 f_100;
    /* 0x104 */ s32 f_104;
    /* 0x108 */ u8 unk108[8];
    /* 0x110 */ s32 f_110;
    /* 0x114 */ u8 unk114[4];
    /* 0x118 */ s32 f_118;
    /* 0x11C */ u8 unk11C[0xA14 - 0x11C];
} GAME_STATE;

/* The per-camera slice the frame code pokes at base + 0x1030. */
typedef struct GameCamState_s {
    /* 0x00 */ f32 f_00;
    /* 0x04 */ f32 f_04;
    /* 0x08 */ f32 f_08;
    /* 0x0C */ f32 f_0C;
} GAME_CAMSTATE;

/* lbl_1_bss_14AC lives at bss + 0x14AC but is defined in another object of
   this module, so from here it is only ever an extern. */
extern GAME_STATE lbl_1_bss_14AC;

/* lbl_1_bss_38: the camera-view scratch block; the active view starts at
   offset 0x1C, i.e. entry 1. Also defined elsewhere in the module. */
extern OMCAMERAVIEW lbl_1_bss_38[4];

/* lbl_1_bss_1ECC: 30-wide rows of slot ids, row picked by the argument.
   The original object reserves 0xF4 bytes, four more than the two rows. */
static union {
    s32 rows[2][0x1E];
    u8 raw[0xF4];
} lbl_1_bss_1ECC;

/* lbl_1_data_3B8: last BGM stream id, seeded to -1. */
static s32 lbl_1_data_3B8 = -1;

/* The original .rodata carries 0x98 bytes of constants ahead of these two,
   owned by the other functions in this unit (lbl_1_rodata_78 onward).
   Reserving that span is what lands these at the offsets the original
   relocates against: lbl_1_rodata_110 at 0x98, lbl_1_rodata_118 at 0xA0. */
static const u8 lbl_1_rodata_78[0x88] = { 0 };

/* lbl_1_rodata_110 (5.5) then lbl_1_rodata_118 (pi/8). */
const f64 lbl_1_rodata_110 = 5.5;
const f64 lbl_1_rodata_118 = 0.39269908169872414;

/* MWCC constant-folds a plain `const double` use into an anonymous rodata
   copy, which loses the named @ha/@l relocation the original makes. Reading
   through volatile keeps the named symbol and the load. */
#define RODATA_D(name) (*(volatile f64 *)&(name))

void MgSeqModeNext(void);
s32 HuAudBGMPlay(s16 streamId);
void fn_1_D64(void);
void fn_1_3DD8(s32 arg0);
void fn_1_4C00(void);

void fn_1_1D88(void)
{
    fn_1_D64();
    if ((u8)WipeCheckEnd() == 0) {
        lbl_1_bss_14AC.f_100++;
    }
}

void fn_1_1DCC(void)
{
    if (omCameraViewCheck(1)) {
        lbl_1_bss_14AC.f_100 = 0;
        lbl_1_bss_14AC.f_104 = 0;
        MgSeqModeNext();
        lbl_1_data_3B8 = HuAudBGMPlay(0x4B);
    }
    fn_1_D64();
}

void fn_1_2618(void)
{
    s32 i;

    for (i = 0; i < 2; i++) {
        fn_1_3DD8(i);
    }
    lbl_1_bss_14AC.f_100 = 4;
    fn_1_4C00();
    fn_1_D64();
}

void fn_1_3088(void)
{
    GAME_CAMSTATE *cam;

    cam = (GAME_CAMSTATE *)((u8 *)lbl_1_bss_14AC.unk03C + 0x1030);
    /* Read through pointers: a directly-named const double is folded into an
       anonymous rodata copy by MWCC, losing the named relocation. */
    cam->f_04 += RODATA_D(lbl_1_rodata_110) * sin(RODATA_D(lbl_1_rodata_118));
    cam->f_0C += RODATA_D(lbl_1_rodata_110) * cos(RODATA_D(lbl_1_rodata_118));
    if (lbl_1_bss_14AC.f_110++ > 0x3C) {
        omCameraViewMoveSimple(&lbl_1_bss_38[1], 0xB4);
        lbl_1_bss_14AC.f_100++;
        lbl_1_bss_14AC.f_118 = 0;
    }
    fn_1_D64();
}

s32 fn_1_4990(s32 arg0)
{
    s32 i;

    for (i = 0; i < 0x1E; i++) {
        if (lbl_1_bss_1ECC.rows[arg0][i] == 0) {
            break;
        }
    }
    if (i == 0x1E) {
        return -1;
    }
    return i;
}