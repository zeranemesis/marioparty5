
#include "game/object.h"
#include "game/hu3d.h"
#include "game/memory.h"

void fn_1_3B1C(OMOBJ *obj);
void fn_1_3F04(OMOBJ *obj);
void fn_1_406C(OMOBJ *obj);
void fn_1_4A3C(OMOBJ *obj);
void fn_1_430C(OMOBJ *obj);

typedef struct {
    s32 unk_00[0x40]; // 0x00-0xFF: 32 entries * 2 s32
    s32 unk_100[0x2D]; // 0x100-0x1B3
    s32 state; // 0x1B4
    s32 counter; // 0x1B8
    s32 audioHandle; // 0x1BC
    s32 timer; // 0x1C0
    s32 unk_1C4;
    f32 unk_1C8;
    f32 unk_1CC;
    f32 unk_1D0;
    f32 unk_1D4;
    f32 unk_1D8;
    f32 unk_1DC;
    f32 unk_1E0;
    f32 unk_1E4;
    f32 unk_1E8;
    f32 unk_1EC;
    f32 unk_1F0;
    f32 unk_1F4;
    f32 unk_1F8;
    f32 unk_1FC;
} M534Stage; // size 0x200

extern OMOBJ *lbl_1_bss_48;

BOOL fn_1_49BC(void)
{
    OMOBJ *obj = lbl_1_bss_48;
    M534Stage *stage = obj->data;
    return stage->state == 12;
}

BOOL fn_1_49FC(void)
{
    OMOBJ *obj = lbl_1_bss_48;
    M534Stage *stage = obj->data;
    return stage->state == 8;
}

// NONMATCHING: compiler reloads -710.0f constant for second store
void fn_1_4450(OMOBJ *obj)
{
    M534Stage *stage = obj->data;
    stage->state = 0;
    stage->unk_1C4 = 12;
    stage->unk_1E8 = 0.5f;
    stage->unk_1F0 = -710.0f;
    stage->unk_1EC = -710.0f;
}

void fn_1_4970(void)
{
    s32 i;
    for (i = 0; i < 4; i++) {
        omVibrate(i, 0x28, 0xC, 0);
    }
}

void fn_1_3624(OMOBJ *obj)
{
    fn_1_3B1C(obj);
    fn_1_3F04(obj);
    fn_1_406C(obj);
    fn_1_4A3C(obj);
    fn_1_430C(obj);
}

void fn_1_3AC8(OMOBJ *obj)
{
    s32 *arr = obj->data;
    s32 *ptr = arr;
    s32 i = 0;
    while (i < 32) {
        ptr[0] = -1;
        ptr[1] = -1;
        i++;
        ptr += 2;
    }
}
