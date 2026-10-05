#include "types.h"

extern void HuAudAllStop(void);
extern s32 HuAudJinglePlay(s32);
extern s32 HuAudBGMPlay(s32);
extern s32 MgSeqModeNext(void);
extern s32 fn_1_2FE8(void *);
extern s32 fn_1_3600(void *);
extern s32 fn_1_3638(void);
extern u8 lbl_1_bss_67C;
extern u8 lbl_1_bss_19C;

s32 fn_1_358(void)
{
    return fn_1_2FE8(&lbl_1_bss_67C);
}

s32 fn_1_450(void)
{
    return fn_1_3600(&lbl_1_bss_67C);
}

s32 fn_1_478(void)
{
    s32 ret = fn_1_3638();
    return ret ? MgSeqModeNext() : ret;
}

void fn_1_2338(void)
{
    *(s32 *)((u8 *)&lbl_1_bss_19C + 0x58) = HuAudJinglePlay(0x83);
}

void fn_1_2368(void)
{
    *(s32 *)((u8 *)&lbl_1_bss_19C + 0x54) = HuAudBGMPlay(0x3B);
}

void fn_1_269C(void)
{
    HuAudAllStop();
}