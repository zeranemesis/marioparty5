#include "types.h"
#include <string.h>

extern void fn_1_5C48(void);
extern void fn_1_4308(void);
extern void fn_1_7F30(void);
extern s16 fn_1_817C(void);
extern void fn_1_753C(void);
extern void fn_1_3F28(void);
extern u8 lbl_1_bss_828[];
extern s16 lbl_1_bss_D0[];

s32 fn_1_5514(void)
{
    register s32 r31 asm("r31");
    r31 = 1;
    fn_1_5C48();
    return r31;
}

s32 fn_1_5544(void)
{
    s32 r31 = 1;
    fn_1_4308();
    return r31;
}

s32 fn_1_4E08(void)
{
    s32 r31 = 0;
    fn_1_7F30();
    fn_1_5C48();
    return r31;
}

s32 fn_1_511C(void)
{
    s32 r31 = 0;
    r31 = fn_1_817C();
    fn_1_753C();
    fn_1_5C48();
    return r31;
}

void fn_1_86BC(void)
{
    s32 r31;
    for (r31 = 0; r31 < 4; r31++) {
        memset(&lbl_1_bss_828[r31 * 0x2C], 0, 0x2C);
    }
}

s32 fn_1_4E3C(void)
{
    s32 r31 = 1;
    if (lbl_1_bss_D0[0x2E / 2] == 0) {
        fn_1_3F28();
        lbl_1_bss_D0[0x2E / 2] = 1;
    }
    lbl_1_bss_D0[0] = 0;
    lbl_1_bss_D0[0x6 / 2] = 0;
    fn_1_5C48();
    return r31;
}