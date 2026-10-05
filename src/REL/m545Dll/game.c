#include "types.h"

extern void fn_1_40AC(void *);
extern s32 fn_1_70E8(void *);
extern void fn_1_2398(s32, s32);

typedef struct {
    u8 pad[0xF0];
    s32 unkF0;
} UnkStruct_2FE8;

typedef struct {
    u8 pad[0x4C];
    s16 unk4C;
    u8 pad4E[4];
    s16 unk52;
} UnkStruct_BSS_19C;

typedef struct {
    s16 unk0;
    u8 pad2[2];
    f32 unk4;
    u8 pad8[4];
    f32 unkC;
    u8 pad10[8];
    s32 unk18;
    u8 pad1C[4];
    s32 unk20;
} UnkStruct_3538_Elem;

extern UnkStruct_BSS_19C lbl_1_bss_19C;
extern f32 lbl_1_rodata_88;
extern f32 lbl_1_rodata_8C;

s32 fn_1_2FE8(UnkStruct_2FE8 *arg)
{
    s32 ret = 0;
    arg->unkF0 = 0;
    fn_1_40AC(arg);
    return ret;
}

s32 fn_1_3480(void *arg)
{
    s32 ret = 0;
    if (lbl_1_bss_19C.unk52 == 0) {
        fn_1_2398(1, 0x64);
        lbl_1_bss_19C.unk52 = 1;
    }
    fn_1_40AC(arg);
    ret = 1;
    return ret;
}

s32 fn_1_34EC(void *arg)
{
    s32 ret = 0;
    ret = fn_1_70E8(arg);
    fn_1_40AC(arg);
    return ret;
}

s32 fn_1_3538(void *arg)
{
    s32 ret = 0;
    UnkStruct_3538_Elem *elem = (UnkStruct_3538_Elem *)((u8 *)arg + 0x50 + lbl_1_bss_19C.unk4C * 0x44);
    if (*(s32 *)((u8 *)arg + 0xE4) == 0) {
        elem = (UnkStruct_3538_Elem *)((u8 *)arg + 0x50 + lbl_1_bss_19C.unk4C * 0x44);
        elem->unk0 = 4;
        elem->unk4 = lbl_1_rodata_88;
        elem->unk18 = 0;
        elem->unk20 = 0x40000001;
        elem->unkC = lbl_1_rodata_8C;
        *(s32 *)((u8 *)arg + 0xE4) = 1;
    }
    fn_1_40AC(arg);
    return ret;
}

s32 fn_1_3600(void *arg)
{
    s32 ret = 1;
    fn_1_40AC(arg);
    return ret;
}

s32 fn_1_3638(void)
{
    s32 val = 1;
    return val;
}