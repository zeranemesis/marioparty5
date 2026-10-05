#include "game/process.h"

typedef struct {
    u8 pad[12];
    u32 field12;
} Struct_E91C;

void *fn_1_E91C(Struct_E91C *arg)
{
    u32 temp = arg->field12;
    return arg;
}

typedef struct {
    u8 pad[8];
    s32 field8;
} Struct_E900;

s32 fn_1_E900(s32 ret_val, Struct_E900 *arg)
{
    if (arg->field8 >= 6) goto ret;
    if (arg->field8 == 0) ret_val = 0;
    goto ret;
ret:
    return ret_val;
}

extern void fn_1_758C(void);

s32 fn_1_6E20(void)
{
    s32 ret = 1;
    fn_1_758C();
    return ret;
}