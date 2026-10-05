#include "dolphin/types.h"

extern "C" {
s32 fn_1_B9D8(void *self);
s32 fn_1_C084(void *self, s32 arg);
s32 fn_1_CF28(void *self, s32 arg);
s32 fn_1_DD3C(void *self, s32 arg);
void fn_1_B4E0(void *self, s32 arg);
s32 fn_1_B9F0(void *self);
s32 fn_1_E320(void *self, f32 val1, f32 val2);
s32 fn_1_AE98(void *self);
s32 fn_1_CEB0(void *self, f32 arg);
void *fn_1_CEEC(void *self);
}

#pragma dont_inline on
s32 fn_1_B9D8(void *self)
{
    __asm {
        lfd f1, 0x98(r3)
        lfd f0, 0x38(r3)
        fcmpo cr0, f1, f0
        mfcr r0
        srwi r3, r0, 31
    }
}
#pragma dont_inline reset

#pragma dont_inline on
s32 fn_1_C084(void *self, s32 arg)
{
    __asm {
        lwz r0, 0x8(r3)
        add r4, r4, r0
        lwz r0, 0x0(r3)
        cmplw r4, r0
        blt skip_sub
        lwz r0, 0x0(r3)
        subf r4, r0, r4
    skip_sub:
        lwz r5, 0xc(r3)
        slwi r0, r4, 2
        add r3, r5, r0
    }
}
#pragma dont_inline reset

#pragma dont_inline on
s32 fn_1_DD3C(void *self, s32 arg)
{
    return fn_1_CF28(self, arg);
}
#pragma dont_inline reset

#pragma dont_inline on
s32 fn_1_AE98(void *self)
{
    fn_1_B4E0(self, 0);
    return fn_1_B9F0(self);
}
#pragma dont_inline reset

#pragma dont_inline on
s32 fn_1_CEB0(void *self, f32 arg)
{
    return fn_1_E320(self, *(f32 *)((u8 *)self + 0xC), arg);
}
#pragma dont_inline reset

#pragma dont_inline on
void *fn_1_CEEC(void *self)
{
    __asm {
        lwz r4, 0x4(r3)
        subi r0, r4, 0x1
        stw r0, 0x4(r3)
        lwz r4, 0x8(r3)
        addi r0, r4, 0x1
        stw r0, 0x8(r3)
        lwz r4, 0x8(r3)
        lwz r0, 0x0(r3)
        cmplw r4, r0
        blt skip_wrap
        lwz r4, 0x0(r3)
        lwz r0, 0x8(r3)
        subf r0, r4, r0
        stw r0, 0x8(r3)
    skip_wrap:
    }
}
#pragma dont_inline reset