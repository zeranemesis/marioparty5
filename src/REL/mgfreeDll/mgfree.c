#include "dolphin/types.h"

#include "game/data.h"
#include "game/process.h"
#include "game/sprite.h"

#include "math.h"
#include "string.h"

#include "datanum/mgconst.h"

extern void *lbl_1_bss_0;
extern void *lbl_1_bss_4;
extern Vec lbl_1_bss_8;
extern void *lbl_1_bss_974;
extern u32 lbl_1_data_11C[16][2];
extern void *mgDataTbl;
extern float lbl_1_rodata_10;
extern float lbl_1_rodata_18;

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
}void fn_1_114(s16 arg0)
{
    WipeCreate(2, 6, arg0);
    while (!WipeCheckEnd()) {
        HuPrcVSleep();
    }
}
void fn_1_128C(void) {}
void fn_1_1568(void) {}
void fn_1_15B8(void) {}
void fn_1_160(s16 arg0, float arg1, float arg2)
{
    Hu3DMotionStartEndSet(arg0, arg1, arg2);
    Hu3DMotionTimeSet(arg0, arg1);
    Hu3DMotionShapeTimeSet(arg0, arg1);
}
void fn_1_1828(void) {}
void fn_1_19F8(void) {}
void fn_1_1B84(void) {}
void fn_1_1F50(void) {}
void fn_1_205C(void) {}
void fn_1_21AC(void) {}
void fn_1_26C(void) {}
float fn_1_2B4(float arg1, float arg2, float arg3)
{
    return arg2 + (arg3 * (arg1 - arg2));
}
void fn_1_2C4(void)
{
    float f29 = lbl_1_rodata_10;
    float f30 = lbl_1_rodata_18;
    s16 model = (s16)(*(s32*)((u8*)*(void**)lbl_1_bss_0 + 0x1A8));
    s16 motion = (s16)(*(s32*)((u8*)*(void**)lbl_1_bss_0 + 0x1B4));
    Hu3DModelPosSetV(model, (Vec*)&lbl_1_bss_8);
    Hu3DMotionShiftSet(motion, 0, 0.0f, 0x4000000, 0);
}
void fn_1_2C40(void)
{
    s16 model = (s16)(*(s32*)((u8*)*(void**)lbl_1_bss_0 + 0x1A8));
    s16 motion = (s16)(*(s32*)((u8*)*(void**)lbl_1_bss_0 + 0x1B4));
    Hu3DModelPosSetV(model, (Vec*)&lbl_1_bss_8);
    Hu3DMotionShiftSet(motion, 0, 0.0f, 0x4000000, 0);
}
void fn_1_37A4(void) {}
void fn_1_3C8C(void) {}
void fn_1_45A4(void) {}
void fn_1_4CC(void)
{
    s16 var = (s16)(*(s32*)((u8*)*(void**)lbl_1_bss_0 + 0x1A0));
    *(s32*)((u8*)*(void**)lbl_1_bss_0 + 0x1CC) &= ~0x7;
    Hu3DMotionShiftSet(var, 0, 0.0f, 0x4000000, 0);
}
void fn_1_684(void)
{
    *(s32*)((u8*)*(void**)lbl_1_bss_0 + 0x1CC) |= 4;
}
void fn_1_708(void)
{
    s16 var = (s16)(*(s32*)((u8*)*(void**)lbl_1_bss_0 + 0x19C));
    *(s32*)((u8*)*(void**)lbl_1_bss_0 + 0x1CC) &= ~0x7;
    Hu3DMotionShiftSet(var, 0, 0.0f, 0x4000000, 0);
}
void fn_1_8C0(void)
{
    *(s32*)((u8*)*(void**)lbl_1_bss_0 + 0x1CC) |= 0x8;
    if (*(s32*)((u8*)*(void**)lbl_1_bss_0 + 0x1D8) != 0) {
        Hu3DMotionTimeSet(*(s16*)((u8*)*(void**)lbl_1_bss_0 + 0x1B4), 0.0f);
    }
}
void fn_1_944(void) {}
void fn_1_A0(s16 arg0)
{
    if (WipeTypeGet() == 0x89) {
        WipeCreate(1, 0, arg0);
    } else {
        WipeCreate(1, 5, arg0);
    }
    while (!WipeCheckEnd()) {
        HuPrcVSleep();
    }
}
