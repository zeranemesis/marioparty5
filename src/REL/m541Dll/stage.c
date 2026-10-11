#include "types.h"

/*
 * m541Dll "stage" unit. The stage object is a 0x2E8 byte block created by
 * fn_1_19AB0; a pointer to it is parked in lbl_1_bss_3738.
 */

typedef struct M541Stage_s {
    u8 pad[0x174];
    s16 unk174;
    u8 pad2[2];
} M541_STAGE;

extern M541_STAGE *lbl_1_bss_3738;

void fn_1_19CFC(s16 arg)
{
    lbl_1_bss_3738->unk174 = arg;
}

void fn_1_19D10(s16 idx, s32 val)
{
    *(s32 *)((u8 *)lbl_1_bss_3738 + (idx * 4) + 0x5C) = val;
}

void fn_1_19D30(s16 idx, s32 val)
{
    *(s32 *)((u8 *)lbl_1_bss_3738 + (idx * 4) + 0x68) = val;
}

void fn_1_19D50(void)
{
}

void fn_1_1A6D8(void)
{
}

void fn_1_1A6DC(void)
{
}

void fn_1_1A6E0(void)
{
}

void fn_1_1A6E4(void)
{
}

void fn_1_1A6E8(void)
{
}

void fn_1_1A6EC(void)
{
}

void fn_1_1A6F0(void)
{
}
