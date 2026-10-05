#include "REL/m550dll.h"

extern void *lbl_1_bss_1FC0;

void fn_1_6D8C(void)
{
    void *ptr = lbl_1_bss_1FC0;
    *(s32 *)((u8 *)ptr + 0x428) = -1;
}