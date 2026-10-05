#include "dolphin/types.h"

extern "C" {

void fn_1_924(void *obj)
{
    register f32 *ptr asm("r3") = (f32 *)obj;
    ptr[0x40 / 4] = ptr[0x38 / 4];
    ptr[0x44 / 4] = ptr[0x3C / 4];
}

} // extern "C"