// m554dll/stage.c - decompilation of the m554dll/REL/m554dll/stage unit.
// Functions are added one at a time, each verified with tools/fndiff.py.

#include "types.h"

extern s32 *lbl_1_bss_1360;

extern void fn_1_78F4(void);
extern void fn_1_7D74(void);
extern void fn_1_7FCC(void);

/* fn_1_7F68: set field at +0x584 to -1 */
void fn_1_7F68(void)
{
    lbl_1_bss_1360[0x584 / 4] = -1;
}

/* fn_1_6C20: store two args at +0x40 and +0x44 */
void fn_1_6C20(s32 arg0, s32 arg1)
{
    lbl_1_bss_1360[0x40 / 4] = arg0;
    lbl_1_bss_1360[0x44 / 4] = arg1;
}

/* fn_1_6C44: store two args at +0x48 and +0x4c */
void fn_1_6C44(s32 arg0, s32 arg1)
{
    lbl_1_bss_1360[0x48 / 4] = arg0;
    lbl_1_bss_1360[0x4c / 4] = arg1;
}

/* fn_1_7880: call three initialization functions */
void fn_1_7880(void)
{
    fn_1_78F4();
    fn_1_7D74();
    fn_1_7FCC();
}

/* fn_1_7F84: clear two rows at stride 0x3c, base 0x374+8*0x3c */
void fn_1_7F84(void)
{
    s32 i;
    u8 *base = (u8 *)lbl_1_bss_1360;
    for (i = 0; i < 2; i++) {
        *(s32 *)(base + 0x374 + (i + 8) * 0x3c) = -1;
    }
}