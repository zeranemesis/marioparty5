#include "types.h"

extern u32 lbl_1_bss_24;

void fn_1_C3C(void)
{
    lbl_1_bss_24 = 2;
}

u32 fn_1_3224(void *arg)
{
    void *ptr = *(void **)((u8 *)arg + 0x5C);
    return *(u32 *)ptr;
}