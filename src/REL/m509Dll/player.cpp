#include "dolphin/types.h"

extern "C" {

void fn_1_10608(void *self, s32 a, s32 b);
void fn_1_E54(void *self, s32 val);
void fn_1_21C(void *self);

// NONMATCHING: stack frame, register allocation, andc vs nor+and, branch layout
void fn_1_ED2C(void *self, s32 bitIdx)
{
    u32 val = *(u32 *)((u32)self + 0xC);
    val &= ~(1 << bitIdx);
    *(u32 *)((u32)self + 0xC) = val;

    if (bitIdx >= 5) {
        return;
    }
    if (bitIdx < 0) {
        return;
    }
}

void fn_1_FF00(void *self)
{
    fn_1_10608(self, 3, 1);
}

void fn_1_FF30(void *self)
{
    fn_1_10608(self, 4, 1);
}

void *fn_1_F70C(void *self, s16 arg)
{
    if (self == 0) {
        goto exit;
    }
    fn_1_E54(self, 0);
    if (arg <= 0) {
        goto exit;
    }
    fn_1_21C(self);
exit:
    return self;
}

void *fn_1_12D64(void *self, s16 arg)
{
    if (self == 0) {
        goto exit;
    }
    fn_1_E54(self, 0);
    if (arg <= 0) {
        goto exit;
    }
    fn_1_21C(self);
exit:
    return self;
}

}