#include "types.h"

#include "game/memory.h"

extern void *lbl_1_data_EA8[4];

void fn_1_89F4(void *arg0, s32 arg1, s32 arg2);

void fn_1_9028(void *ptr)
{
    memset(ptr, 0, 0x18);
}

void fn_1_8904(s32 arg0, s32 arg1)
{
    fn_1_89F4(lbl_1_data_EA8[0], arg0, arg1);
}

void fn_1_8940(s32 arg0, s32 arg1)
{
    fn_1_89F4(lbl_1_data_EA8[1], arg0, arg1);
}

void fn_1_897C(s32 arg0, s32 arg1)
{
    fn_1_89F4(lbl_1_data_EA8[2], arg0, arg1);
}

void fn_1_89B8(s32 arg0, s32 arg1)
{
    fn_1_89F4(lbl_1_data_EA8[3], arg0, arg1);
}

s32 fn_1_6CB8(void *arg0)
{
    f32 val = *(f32 *)((u8 *)arg0 + 0x4);
    s32 result;
    
    if (val < 0.0f) {
        result = 0;
    }
    else {
        val = *(f32 *)((u8 *)arg0 + 0x4);
        if (val >= 480.0f) {
            result = 0;
        }
        else {
            result = 1;
        }
    }
    return result;
}