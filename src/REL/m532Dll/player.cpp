#include "dolphin/types.h"

extern "C" {
s32 fn_1_11D00(void *self);
}

s32 fn_1_11D00(void *self)
{
    u32 *ptr = (u32 *)self;
    return ptr[0x2C / 4] - ptr[0x28 / 4];
}