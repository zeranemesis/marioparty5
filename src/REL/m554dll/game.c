// m554dll/game.c - decompilation of the m554dll/REL/m554dll/game unit.
// Functions are added one at a time, each verified with tools/fndiff.py.

#include "types.h"
#include "string.h"

typedef struct {
    u8 pad_00[8];
    s16 f_08;
    u8 pad_0a[0x1c - 0x0a];
} Row1C;

typedef struct {
    s16 f_00;
    u8 pad_02[6];
    s32 f_08;
} Row0C;

/* ---- bss owned by m554.o ----
   lbl_1_bss_B1C is used as a word array; the members this unit touches are
   +0x03c, +0x100, +0x10c, +0x110 and +0x120, plus a row array with a stride
   of 0x1c whose s16 member sits at +0x08 of the row. Sizes come from the
   distance to the next bss symbol. */
extern s32 lbl_1_bss_B1C[0x844 / 4];
extern Row0C lbl_1_bss_14[7];
extern Row0C lbl_1_bss_6C[2];

void fn_1_4050(s32 arg0);
s32 fn_1_514C(s32 arg0);
void fn_1_5CCC(s32 arg0);
s32 fn_1_3B58(s32 arg0);

/* fn_1_2804: clear both countdown rows when the sequence is at its last
   step, then leave the mode with 0. */
void fn_1_2804(void)
{
    if (lbl_1_bss_B1C[0x10c / 4] == 2) {
        s32 i;

        for (i = 0; i < 2; i++) {
            fn_1_4050(i);
        }
        lbl_1_bss_B1C[0x100 / 4] = 0;
    }
    fn_1_5CCC(0);
}

/* fn_1_31C8: bump the frame counter and hand it to the display update. */
void fn_1_31C8(void)
{
    lbl_1_bss_B1C[0x110 / 4]++;
    fn_1_514C(lbl_1_bss_B1C[0x110 / 4]);
    fn_1_5CCC(1);
}

/* fn_1_3518: advance the frame counter and roll the page counter over
   once it passes 0x1e. */
void fn_1_3518(void)
{
    s32 unused = lbl_1_bss_B1C[0x3c / 4] + 0x38;

    if (lbl_1_bss_B1C[0x110 / 4]++ >= 0x1e) {
        lbl_1_bss_B1C[0x110 / 4] = 0;
        lbl_1_bss_B1C[0x120 / 4]++;
    }
}

/* fn_1_3AC8: restart one row of the frame table. */
void fn_1_3AC8(s32 arg0)
{
    memset(&lbl_1_bss_6C[arg0], 0, 0x0c);
    lbl_1_bss_6C[arg0].f_00 = ((Row1C *)lbl_1_bss_B1C)[arg0].f_08 - 1;
    lbl_1_bss_6C[arg0].f_08 = fn_1_3B58(arg0);
}

/* fn_1_5A5C: wipe the whole frame table. */
void fn_1_5A5C(void)
{
    s32 i;

    for (i = 0; i < 7; i++) {
        memset(&lbl_1_bss_14[i], 0, 0x0c);
    }
}
