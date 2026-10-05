#include "REL/m513Dll.h"

extern s32 lbl_1_bss_A00;
extern s32 lbl_1_bss_A04;
extern s32 lbl_1_bss_A08;

extern "C" void fn_1_D34C(void *);
extern "C" void fn_1_D4C8(void *);
extern "C" void fn_1_D56C(void *);
extern "C" void fn_1_22E4(void);
extern "C" void fn_1_D88C(void *, s16);
extern "C" void fn_1_B7F4(void *, s16);

extern "C" void fn_1_D6F4(void)
{
    lbl_1_bss_A00 = 0x78;
    lbl_1_bss_A04 = 0xD2;
    lbl_1_bss_A08 = 0xD2;
}

extern "C" void fn_1_CB44(void *arg0)
{
    fn_1_D34C(arg0);
    fn_1_D4C8(arg0);
    fn_1_D56C(arg0);
    fn_1_22E4();
}

extern "C" void *fn_1_B744(void *arg0, s16 arg1)
{
    if (arg0 != NULL) {
        fn_1_D88C(*(void **)arg0, 1);
        if (arg1 > 0) {
            fn_1_C8(arg0);
        }
    }
    return arg0;
}

extern "C" void *fn_1_B79C(void *arg0, s16 arg1)
{
    if (arg0 != NULL) {
        fn_1_B7F4(*(void **)arg0, 1);
        if (arg1 > 0) {
            fn_1_C8(arg0);
        }
    }
    return arg0;
}

extern "C" void *fn_1_BC34(void *arg0, s16 arg1)
{
    if (arg0 != NULL) {
        fn_1_1774((M513_OBJHEAD *)arg0, 0);
        if (arg1 > 0) {
            fn_1_C8(arg0);
        }
    }
    return arg0;
}

extern "C" void *fn_1_BD4C(void *arg0, s16 arg1)
{
    if (arg0 != NULL) {
        fn_1_1774((M513_OBJHEAD *)arg0, 0);
        if (arg1 > 0) {
            fn_1_C8(arg0);
        }
    }
    return arg0;
}

// NONMATCHING: cmplwi vs li+cmplw (compiler version artifact)