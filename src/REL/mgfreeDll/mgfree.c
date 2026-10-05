#include "dolphin/types.h"

#include "game/data.h"
#include "game/process.h"
#include "game/sprite.h"

#include "math.h"
#include "string.h"

#include "datanum/mgconst.h"

extern void *lbl_1_bss_0;
extern void *lbl_1_bss_4;
extern void *lbl_1_bss_974;
extern u32 lbl_1_data_11C[16][2];
extern void *mgDataTbl;

void fn_1_1C8(u32 flags)
{
    u32 *bss0 = lbl_1_bss_0;
    u32 *flagsPtr = (u32 *)((u8 *)bss0 + 0x1CC);
    *flagsPtr |= flags;
}

void fn_1_1E4(u32 flags)
{
    u32 *bss0 = lbl_1_bss_0;
    u32 *flagsPtr = (u32 *)((u8 *)bss0 + 0x1CC);
    *flagsPtr &= ~flags;
}

u32 fn_1_200(u32 mask)
{
    u32 *bss0 = lbl_1_bss_0;
    u32 *flagsPtr = (u32 *)((u8 *)bss0 + 0x1CC);
    return *flagsPtr & mask;
}

u32 fn_1_218(void)
{
    u32 *bss4 = lbl_1_bss_4;
    u32 index = *(u32 *)((u8 *)bss4 + 0x10);
    u32 entry = lbl_1_data_11C[index][1];
    return entry & 0xFFFFFF;
}

u32 fn_1_244(void)
{
    u32 *bss4 = lbl_1_bss_4;
    u32 index = *(u32 *)((u8 *)bss4 + 0x10);
    return lbl_1_data_11C[index][1];
}

u32 fn_1_46AC(s32 idx)
{
    void *bss974 = lbl_1_bss_974;
    void *base = *(void **)bss974;
    u16 val = *(u16 *)((u8 *)base + (idx * 4) + 0x84);
    void *entry = (u8 *)mgDataTbl + (val * 0x50);
    return *(u32 *)((u8 *)entry + 0x8);
}