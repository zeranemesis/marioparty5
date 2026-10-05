#include "dolphin/types.h"

extern "C" {

#pragma dont_inline on
void fn_1_12758(void *self, s32 bit)
{
    ((u32 *)((u8 *)self + 0xC))[0] &= ~(1 << bit);
}
#pragma dont_inline reset

}