// m550dll/m550.c - decompilation of the m550dll/m550 unit.
// Functions are written smallest-first, each verified against the original
// with tools/fndiff.py.

/* Suppress MSL's `extern inline sqrtf` body: without this, every object that
   reaches humath.h (through REL/m550dll.h -> game/hu3d.h) carries 16 bytes of
   duplicate `.rodata` (the 0.5/3.0 sqrtf statics). */
#define _MATH_H

#include "types.h"

#include "game/mg/seqman.h"

#include "REL/m550dll.h"

/* lbl_1_bss_14AC: the module-wide game state block, 0xA14 bytes.
   Only the fields the decompiled functions touch are named. */
typedef struct M550State_s {
    /* 0x000 */ u8 unk000[0x100];
    /* 0x100 */ s32 f_100;
    /* 0x104 */ s32 f_104;
    /* 0x108 */ u8 unk108[0xA14 - 0x108];
} M550_STATE;

M550_STATE lbl_1_bss_14AC;

/* The mode handler tables. The field at 0x100 of the state block selects the
   entry, so the dispatchers call tbl[state.f_100](). */
typedef void (*M550_MODEFN)(void);

void fn_1_D64(void);
void fn_1_1C84(void);
void fn_1_1D88(void);
void fn_1_1DCC(void);
void fn_1_1E38(void);
void fn_1_235C(void);
void fn_1_2360(void);
void fn_1_2670(void);
void fn_1_2D98(void);
void fn_1_3088(void);
void fn_1_3174(void);
void fn_1_3324(void);

M550_MODEFN lbl_1_data_224[3] = { fn_1_1C84, fn_1_1D88, fn_1_1DCC };
M550_MODEFN lbl_1_data_230[3] = { fn_1_1E38, fn_1_235C, fn_1_2360 };
M550_MODEFN lbl_1_data_23C[5] = { fn_1_2670, fn_1_2D98, fn_1_3088, fn_1_3174, fn_1_3324 };

void fn_1_6B4(void)
{
    lbl_1_data_224[lbl_1_bss_14AC.f_100]();
}

void fn_1_6F8(s16 arg)
{
    lbl_1_bss_14AC.f_100 = 0;
    lbl_1_bss_14AC.f_104 = 0;
    MgSeqModeDelaySet(arg + 1, -1);
    fn_1_D64();
}

void fn_1_750(void)
{
    lbl_1_data_230[lbl_1_bss_14AC.f_100]();
}

void fn_1_7FC(void)
{
    lbl_1_data_23C[lbl_1_bss_14AC.f_100]();
}

void fn_1_910(void) { }
