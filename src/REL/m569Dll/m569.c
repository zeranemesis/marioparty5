#include "game/object.h"
#include "game/wipe.h"

extern void *lbl_1_bss_88;
extern void *lbl_1_bss_D4;

// NONMATCHING: fn_1_1858 - register allocation differs
void fn_1_1858(void *src)
{
    u32 *dst = (u32 *)&lbl_1_bss_88;
    u32 *s = (u32 *)((u8 *)src - 4);
    u32 *d = (u32 *)((u8 *)dst - 4);
    int ctr = 9;

    while (ctr--) {
        u32 val1 = s[1];
        u32 val2 = s[2];
        s += 2;
        d[1] = val1;
        d[2] = val2;
        d += 2;
    }
}

// NONMATCHING: fn_1_2668 - stack frame and register allocation differs
s32 fn_1_2668(void *arg0)
{
    u32 *work = *(u32 **)((u8 *)arg0 + 0x5C);
    u32 val = *(u32 *)((u8 *)work + 0x9C);
    u32 bit = (val >> 1) & 1;

    if (bit != 0) {
        return 1;
    }
    return 0;
}

// NONMATCHING: fn_1_269C - stack frame size differs
void fn_1_269C(void *arg0)
{
    OMOBJMAN *objman = (OMOBJMAN *)lbl_1_bss_D4;
    omDelObjEx(objman, arg0);
}

void fn_1_2C18(void)
{
    if (WipeCheckEnd() == 0) {
        omOvlReturnEx(1, 1);
    }
}

u32 fn_1_2DF0(void *arg0)
{
    u32 *work = *(u32 **)((u8 *)arg0 + 0x5C);
    return *work;
}

void fn_1_2E58(void *arg0, u32 arg1, u32 arg2, u32 arg3, u32 arg4)
{
    u32 *work = *(u32 **)((u8 *)arg0 + 0x5C);
    *(u32 *)((u8 *)work + 0x14) = arg1;
    *(u32 *)((u8 *)work + 0x18) = arg2 * 0x3C;
    *(u32 *)((u8 *)work + 0x1C) = arg3 * 0x3C;
    *(u32 *)((u8 *)work + 0x20) = arg4 * 0x3C;
}