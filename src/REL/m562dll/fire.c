#include "game/process.h"

extern void DCStoreRangeNoSync(void *addr, u32 size);
extern void *memset(void *dest, s32 val, u32 size);

// NONMATCHING: fn_1_152C8 uses r4 for first arg instead of r3, different register allocation
void fn_1_152C8(void *arg0) {
    void *buf = *(void **)((u32)arg0 + 0x48);
    u16 count = *(u16 *)((u32)arg0 + 0x30);
    DCStoreRangeNoSync(buf, count * 0x44);
}

// NONMATCHING: fn_1_15390 uses r4 for first arg instead of r3
void fn_1_15390(void *arg0) {
    *(s32 *)((u32)arg0 + 0x8) = 0;
    *(s32 *)((u32)arg0 + 0x4C) = 0;
}

void fn_1_154FC(void *arg0) {
    void *r31 = *(void **)arg0;
    void *r29 = *(void **)r31;
    volatile u32 dummy = (u32)r29 + 0x160;
    (void)dummy;
    memset(arg0, 0, 0x54);
}

// NONMATCHING: fn_1_154FC register allocation differs (r30/r29 swap)
void fn_1_16CF4(void *arg0) {
    *(s16 *)((u32)arg0 + 0x3E) = 0;
    *(s32 *)((u32)arg0 + 0x40) = 1;
}

s32 fn_1_1776C(void *arg0) {
    void *r30 = *(void **)arg0;
    void *r29 = *(void **)r30;
    u32 dummy = (u32)r29 + 0x28;
    void *ptr;
    (void)dummy;

    *(s16 *)((u32)arg0 + 0x3E) = 0;
    *(s32 *)((u32)arg0 + 0x40) = 1;

    ptr = *(void **)((u32)arg0 + 0x68);
    if (ptr != NULL) {
        *(s32 *)((u32)ptr + 0x8) = 0;
        *(s32 *)((u32)ptr + 0x4C) = 0;
        *(void **)((u32)arg0 + 0x68) = NULL;
        *(s16 *)((u32)arg0 + 0x6E) = 0;
        *(s16 *)((u32)arg0 + 0x6C) = 0;
    }
    return 0;
}

typedef struct FireSlot_s {
    u8 unk0[0x8];
    s32 unk8;
    u8 unkC[0x4];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    f32 unk2C;
    u8 unk30[0x24]; // padding to 0x54
} FireSlot;

typedef struct FireData_s {
    u8 unk0[0x1108];
    FireSlot slots[6];
} FireData;

typedef struct FireArg2_s {
    u8 unk0[0x34];
    f32 unk34;
    u8 unk38[0xC];
    s32 unk44;
    s32 unk48;
    s32 unk4C;
} FireArg2;

void *fn_1_15308(FireData *arg0, FireArg2 *arg1) {
    s32 i;
    FireSlot *slot;

    for (i = 0; i < 6; i++) {
        slot = &arg0->slots[i];
        if (*(s32 *)((u32)slot + 0xC) != 0) {
            continue;
        }
        if (slot->unk8 != 0) {
            continue;
        }
        slot->unk8 = 1;
        *(s32 *)((u32)slot + 0x20) = arg1->unk44;
        *(s32 *)((u32)slot + 0x24) = arg1->unk48;
        *(s32 *)((u32)slot + 0x28) = arg1->unk4C;
        *(f32 *)((u32)slot + 0x2C) = arg1->unk34;
        return slot;
    }
    return NULL;
}