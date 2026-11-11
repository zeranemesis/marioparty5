#include "dolphin/types.h"
#include "game/process.h"

extern void *lbl_1_bss_974;

void fn_1_7FA0(void);
void fn_1_82B4(void);
void fn_1_8678(void);
void fn_1_8944(void);
void fn_1_8C10(void);
void fn_1_8EDC(void);

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
    u8 pad_108[0x41];
    s8 unk_149[0x100];
} DecideWork;

u8 fn_1_4D4C(void)
{
    return (*(DecideWork **)&lbl_1_bss_974)->unk_107;
}

s32 fn_1_4D60(s32 idx)
{
    return (*(DecideWork **)&lbl_1_bss_974)->unk_149[idx];
}

void fn_1_4D20(s32 val)
{
    (*(DecideWork **)&lbl_1_bss_974)->unk_00 = (s16)val;
    (*(DecideWork **)&lbl_1_bss_974)->unk_02 = 0;
}

s16 fn_1_A4B0(void)
{
    return *(s16 *)((char *)(*(DecideWork **)&lbl_1_bss_974) + (*(DecideWork **)&lbl_1_bss_974)->unk_100 * 4 + 0x84);
}

s32 fn_1_A544(void)
{
    return (*(DecideWork **)&lbl_1_bss_974)->unk_00 == 1;
}

void fn_1_6B80(s32 *out1, s32 *out2, s32 *out3)
{
    *out1 = (*(DecideWork **)&lbl_1_bss_974)->unk_100;
    *out2 = (s32)(s8)(*(DecideWork **)&lbl_1_bss_974)->unk_106;
    *out3 = (*(DecideWork **)&lbl_1_bss_974)->unk_104;
}

s32 fn_1_A4DC(void)
{
    if (*(s8 *)((char *)(*(DecideWork **)&lbl_1_bss_974) + (*(DecideWork **)&lbl_1_bss_974)->unk_100 * 4 + 0x86) == 0) {
        return -1;
    }
    return *(s16 *)((char *)(*(DecideWork **)&lbl_1_bss_974) + (*(DecideWork **)&lbl_1_bss_974)->unk_100 * 4 + 0x84);
}

void fn_1_823C(void)
{
    *(s32 *)((char *)(*(DecideWork **)&lbl_1_bss_974) + 0x108) = 8;
    if (*(void **)((char *)(*(DecideWork **)&lbl_1_bss_974) + 0x118) == 0) {
        *(void **)((char *)(*(DecideWork **)&lbl_1_bss_974) + 0x118) = HuPrcChildCreate(fn_1_7FA0, 0xC8, 0x1000, 0, HuPrcCurrentGet());
    }
}

void fn_1_8600(void)
{
    *(s32 *)((char *)(*(DecideWork **)&lbl_1_bss_974) + 0x10C) = 8;
    if (*(void **)((char *)(*(DecideWork **)&lbl_1_bss_974) + 0x11C) == 0) {
        *(void **)((char *)(*(DecideWork **)&lbl_1_bss_974) + 0x11C) = HuPrcChildCreate(fn_1_82B4, 0xC8, 0x1000, 0, HuPrcCurrentGet());
    }
}

void fn_1_88CC(void)
{
    *(s32 *)((char *)(*(DecideWork **)&lbl_1_bss_974) + 0x110) = 8;
    if (*(void **)((char *)(*(DecideWork **)&lbl_1_bss_974) + 0x120) == 0) {
        *(void **)((char *)(*(DecideWork **)&lbl_1_bss_974) + 0x120) = HuPrcChildCreate(fn_1_8678, 0xC8, 0x1000, 0, HuPrcCurrentGet());
    }
}

void fn_1_8B98(void)
{
    *(s32 *)((char *)(*(DecideWork **)&lbl_1_bss_974) + 0x114) = 8;
    if (*(void **)((char *)(*(DecideWork **)&lbl_1_bss_974) + 0x124) == 0) {
        *(void **)((char *)(*(DecideWork **)&lbl_1_bss_974) + 0x124) = HuPrcChildCreate(fn_1_8944, 0xC8, 0x1000, 0, HuPrcCurrentGet());
    }
}

void fn_1_8E64(void)
{
    *(s32 *)((char *)(*(DecideWork **)&lbl_1_bss_974) + 0x110) = 8;
    if (*(void **)((char *)(*(DecideWork **)&lbl_1_bss_974) + 0x120) == 0) {
        *(void **)((char *)(*(DecideWork **)&lbl_1_bss_974) + 0x120) = HuPrcChildCreate(fn_1_8C10, 0xC8, 0x1000, 0, HuPrcCurrentGet());
    }
}

void fn_1_9130(void)
{
    *(s32 *)((char *)(*(DecideWork **)&lbl_1_bss_974) + 0x114) = 8;
    if (*(void **)((char *)(*(DecideWork **)&lbl_1_bss_974) + 0x124) == 0) {
        *(void **)((char *)(*(DecideWork **)&lbl_1_bss_974) + 0x124) = HuPrcChildCreate(fn_1_8EDC, 0xC8, 0x1000, 0, HuPrcCurrentGet());
    }
}