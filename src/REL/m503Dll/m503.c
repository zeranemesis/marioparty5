#include "game/object.h"

extern OMOBJ *lbl_1_bss_20;

s32 fn_1_1AB0(void)
{
    return *(s32 *)((u8 *)lbl_1_bss_20->data + 0x0);
}

s32 fn_1_1AC8(void)
{
    return *(s32 *)((u8 *)lbl_1_bss_20->data + 0x10);
}

s32 fn_1_1AE0(void)
{
    return *(s32 *)((u8 *)lbl_1_bss_20->data + 0x18);
}

s32 fn_1_1AF8(void)
{
    return *(s32 *)((u8 *)lbl_1_bss_20->data + 0x20);
}

s32 fn_1_1B10(void)
{
    return *(s32 *)((u8 *)lbl_1_bss_20->data + 0x70);
}

void fn_1_1B28(s32 val)
{
    *(s32 *)((u8 *)lbl_1_bss_20->data + 0x70) = val;
}