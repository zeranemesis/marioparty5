#include "types.h"

extern s32 lbl_1_bss_310;
extern s32 lbl_1_bss_324[];

s32 fn_1_7298(void)
{
    return (lbl_1_bss_310 >= 0);
}

void fn_1_BAF4(s32 idx, s32 val)
{
    lbl_1_bss_324[idx] = val;
}

void fn_1_BAD8(void **ptr, s32 val)
{
    ((s16 *)*ptr)[0x34 / 2] = (s16)val;
    ((s16 *)*ptr)[0x36 / 2] = 0;
}

s32 fn_1_6784(void *a, void *b)
{
    return 0;
}