#include "types.h"

extern const void *_dtors[];

extern s16 lbl_1_bss_0;
extern s32 lbl_1_bss_1954;
extern void (*lbl_1_data_27C[])(void);

extern void MgSeqModeNext(void);
extern void fn_1_737C(void);
extern void Hu3DModelKill(s16 modelId);
extern s32 WipeCheckEnd(void);
extern void fn_1_24F8(void);

void _epilog(void)
{
    const void **dtor;

    for (dtor = _dtors; *dtor != 0; dtor++) {
        ((void (*)(void))*dtor)();
    }
}

void fn_1_7A0(void)
{
    ((void (*)(void))*(u32 *)((u8 *)lbl_1_data_27C + (*(s32 *)((u8 *)&lbl_1_bss_1954 + 0xA0) << 2)))();
}

void fn_1_CC4(void)
{
    MgSeqModeNext();
    fn_1_737C();
}

void fn_1_CE8(void)
{
    fn_1_737C();
    if ((u8)WipeCheckEnd() == 0) {
        MgSeqModeNext();
    }
}

void fn_1_D1C(void)
{
    fn_1_24F8();
}

void fn_1_24F8(void)
{
    Hu3DModelKill(lbl_1_bss_0);
}