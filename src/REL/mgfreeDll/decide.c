#include "dolphin/types.h"

extern void *lbl_1_bss_974;

typedef struct DecideWork_s {
    s16 unk_00;
    s16 unk_02;
    u8 pad_04[0x80];
    s16 unk_84[0x3E];
    s16 unk_100;
    u8 pad_102[0x2];
    s16 unk_104;
    s8 unk_106;
    u8 unk_107;
    u8 pad_108[0x42];
    s8 unk_149[0x100];
} DecideWork;

u8 fn_1_4D4C(void)
{
    return (*(DecideWork **)&lbl_1_bss_974)->unk_107;
}

s8 fn_1_4D60(s32 idx)
{
    return (*(DecideWork **)&lbl_1_bss_974)->unk_149[idx + 0x149];
}

void fn_1_4D20(s16 val)
{
    DecideWork *work = *(DecideWork **)&lbl_1_bss_974;
    work->unk_00 = val;
    work->unk_02 = 0;
}

s16 fn_1_A4B0(void)
{
    DecideWork *work = *(DecideWork **)&lbl_1_bss_974;
    return work->unk_84[work->unk_100 * 4 + 0x84 / 2];
}

s32 fn_1_A544(void)
{
    DecideWork *work = *(DecideWork **)&lbl_1_bss_974;
    return work->unk_00 == 1;
}

void fn_1_6B80(s32 *out1, s32 *out2, s32 *out3)
{
    DecideWork *work = *(DecideWork **)&lbl_1_bss_974;
    *out1 = work->unk_100;
    *out2 = (s32)(s8)work->unk_106;
    *out3 = work->unk_104;
}