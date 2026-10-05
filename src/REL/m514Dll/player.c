#define _MATH_H
#include "game/charman.h"
#include "game/hu3d.h"

extern HuVecF Center;
extern void *lbl_1_bss_70[4];
extern f32 lbl_1_rodata_E0;
extern f64 lbl_1_rodata_E8;

void fn_1_1EA0(void)
{
    CharModelKill(CHARNO_NONE);
}

f32 fn_1_4238(s32 idx)
{
    void *player = lbl_1_bss_70[idx];
    void *unk5C = *(void **)((u8 *)player + 0x5C);
    return *(f32 *)((u8 *)unk5C + 0x40);
}

// NONMATCHING: register allocation differs (r28/r29/r30/r31 usage)
s32 fn_1_4024(void *player)
{
    void *unk5C = *(void **)((u8 *)player + 0x5C);
    s16 motId = *(s16 *)((u8 *)unk5C + 0xC);
    s32 result = 0;

    if (CharMotionEndCheck(motId) == FALSE) {
        HU3DMOTID shiftId = CharMotionShiftIDGet(motId);
        if ((s16)shiftId < 0) {
            result = 1;
        }
    }

    return result;
}

// NONMATCHING: register allocation differs (stack frame size, r28/r29/r30/r31 usage)
s32 fn_1_3FB8(void *player)
{
    void *unk5C = *(void **)((u8 *)player + 0x5C);
    s16 motId = *(s16 *)((u8 *)unk5C + 0xC);
    s32 result = 0;

    if (CharMotionEndCheck(motId) != FALSE) {
        HU3DMOTID shiftId = CharMotionShiftIDGet(motId);
        if ((s16)shiftId < 0) {
            result = 1;
        }
    }

    return result;
}

// NONMATCHING: register allocation differs (f30/f31 usage, stack frame size)
void fn_1_22C8(void *player)
{
    void *unk5C = *(void **)((u8 *)player + 0x5C);
    f32 val = *(f32 *)((u8 *)unk5C + 0x40);
    f32 centerX = Center.x;
    f32 factor = (f32)lbl_1_rodata_E8;

    val = (val - centerX) * factor + centerX;
    Center.x = val;
}

// NONMATCHING: register allocation differs (r28/r30 usage)
void fn_1_3CF8(void *player)
{
    void *unk5C = *(void **)((u8 *)player + 0x5C);
    HU3DMODELID modelId = *(HU3DMODELID *)((u8 *)player + 0x40);
    s16 rotVal = *(s16 *)((u8 *)unk5C + 0x0);

    Hu3DModelPosSetV(modelId, (HuVecF *)((u8 *)unk5C + 0x40));
    Hu3DModelRotSet(rotVal, lbl_1_rodata_E0, *(f32 *)((u8 *)unk5C + 0x58), lbl_1_rodata_E0);
}