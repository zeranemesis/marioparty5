#define _MATH_H

#include "types.h"
#include "game/object.h"
#include "game/hu3d.h"

extern void *memset(void *dst, int c, u32 n);

typedef struct {
    s32 state;
    u8 pad04[0x10];
    Vec pos;
    u8 pad20[0x6c];
} MGSTAGESLOT;

typedef struct {
    u8 pad000[0xa8];
    MGSTAGESLOT slotsA[66];
    u8 pad24c0[0x30];
    MGSTAGESLOT slotsB[12];
    u8 pad2b80[0x578];
    f32 waterX;
    f32 waterY;
} MGSTAGEWORK;

typedef struct {
    s32 unk00;
    u8 pad04[0x48];
    s32 unk4c;
    u8 pad50[0x0c];
    u8 *work;
} MGSTAGE;

extern MGSTAGE *lbl_1_bss_38;

f32 lbl_1_rodata_250 = 0.0f;
f32 lbl_1_rodata_3A0 = -100.0f;
f32 lbl_1_rodata_4A0 = 10.0f;
double lbl_1_rodata_500 = 0.0005;

void fn_1_4960(OMOBJ *obj);

void fn_1_78B8(Vec *vec, f32 v10, f32 *p10);

void fn_1_48CC(OMOBJMAN *objman);
void fn_1_5EDC(Vec *pos);
void fn_1_6814(Vec *pos);
void fn_1_6CF4(MGSTAGE *stage);
s32 fn_1_6D78(void);

void fn_1_48CC(OMOBJMAN *objman)
{
    MGSTAGE *stage;
    u8 *work;

    stage = (MGSTAGE *)omAddObjEx(objman, 0x14, 0x86, 0, -1, fn_1_4960);
    lbl_1_bss_38 = stage;
    work = (u8 *)HuMemDirectMallocNum(0, 0x3100, 0x10000000);
    stage->work = work;
    memset(work, 0, 0x3100);
    stage->unk4c = 0;
}

void fn_1_5EDC(Vec *pos)
{
    MGSTAGESLOT *slot;
    s32 i;
    MGSTAGE *stage = lbl_1_bss_38;
    MGSTAGEWORK *work;

    work = (MGSTAGEWORK *)stage->work;
    for (i = 0; i < 0xc; i++) {
        slot = &work->slotsB[i];
        if (slot->state == 0) {
            slot->state++;
            slot->pos = *pos;
            break;
        }
    }
}

void fn_1_6814(Vec *pos)
{
    Vec v;

    v.x = pos->x;
    v.y = lbl_1_rodata_3A0;
    v.z = pos->z;
    fn_1_78B8(&v, lbl_1_rodata_4A0, &lbl_1_rodata_4A0);
}

void fn_1_6CF4(MGSTAGE *stage)
{
    MGSTAGEWORK *work = (MGSTAGEWORK *)stage->work;

    work->waterX = work->waterX + lbl_1_rodata_500;
    work->waterY = work->waterY + lbl_1_rodata_500;
    Hu3DWaterTexPosSet(work->waterX, work->waterY, lbl_1_rodata_250);
}

s32 fn_1_6D78(void)
{
    s32 i;
    s32 count;
    MGSTAGE *stage = lbl_1_bss_38;
    MGSTAGESLOT *slot;
    MGSTAGEWORK *work;

    work = (MGSTAGEWORK *)stage->work;
    count = 0;
    for (i = 0; i < 0x42; i++) {
        slot = &work->slotsA[i];
        if (slot->state != 2) {
            count++;
        }
    }
    return count;
}