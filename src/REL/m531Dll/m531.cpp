#include "dolphin/types.h"

extern "C" {
extern s16 omSysExitReq;

void fn_1_1F68(void);
void fn_1_1F10(void);
void fn_1_1F3C(void);
void fn_1_2104(void **vtablePtr, void *obj);
void fn_1_2124(void *obj);
void fn_1_25C8(void **objPtr);
s32 fn_1_2424(void *obj);
extern void *lbl_1_data_BC[4];
void WipeCreate(s32 a, s32 b, s32 c);
s32 MgTimerValueGet(void *timer);
}

void fn_1_1F68(void)
{
    omSysExitReq = 1;
}

void fn_1_1F10(void)
{
    WipeCreate(1, 5, 0x3C);
}

void fn_1_1F3C(void)
{
    WipeCreate(2, 6, 0x3C);
}

void fn_1_2104(void **vtablePtr, void *obj)
{
    *vtablePtr = lbl_1_data_BC;
    *(void **)((u8 *)obj + 0x5C) = vtablePtr;
    *(void **)((u8 *)obj + 0x14) = fn_1_2124;
}

void fn_1_25C8(void **objPtr)
{
    if (*(s32 *)((u8 *)*objPtr + 0x14) == 0) {
        *(u32 *)((u8 *)*objPtr + 0x14) = 1;
        *(u32 *)*objPtr = 1;
    }
}

s32 fn_1_2424(void *obj)
{
    return MgTimerValueGet(*(void **)obj);
    // NONMATCHING: stack frame size and register allocation differs
}