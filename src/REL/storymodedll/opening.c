#include <dolphin/types.h>

extern s32 lbl_1_data_2CC[10];
extern u8 lbl_1_bss_60[0x3C];

void fn_1_A294(s32 index)
{
    s32 val = lbl_1_data_2CC[index];
    *(s32 *)&lbl_1_bss_60[8] = val;
}