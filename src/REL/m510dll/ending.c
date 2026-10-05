#define _MATH_H
#include "REL/m510dll.h"

typedef struct EndingHeader_s {
    u8 pad00[0x04];
    s16 modelId1;    // 0x04
    s16 modelId2;    // 0x06
    u8 pad08[0x06];
    s16 modelId3;    // 0x0E
    s16 modelId4;    // 0x10
    s16 modelId5;    // 0x12
    s16 motionId1;   // 0x14
    s16 motionId2;   // 0x16
    u8 pad18[0x08];
} EndingHeader;

typedef struct EndingSlot1_s {
    s16 state;
    u8 pad02[0x02];
    s32 activeF;
    u8 pad08[0x10];
} EndingSlot1;

typedef struct EndingSlot2_s {
    s16 state;
    u8 pad02[0x02];
    s32 activeF;
    u8 pad08[0x04];
    s32 data0;
    s32 data1;
    s32 data2;
    u8 pad14[0x08];
} EndingSlot2;

typedef struct EndingBss_s {
    EndingHeader header;
    EndingSlot1 slots1[16];
    EndingSlot2 slots2[64];
    u32 field_9A0;
    u32 field_9A4;
    u8 pad_9A8[0x9C0 - 0x9A8];
} EndingBss;

extern EndingBss lbl_1_bss_7824;

void fn_1_B8F4(void *data)
{
    s32 i;
    EndingSlot1 *slot;

    for (i = 0; i < 16; i++) {
        slot = &lbl_1_bss_7824.slots1[i];
        if (slot->activeF == 0) {
            slot->state = 0;
            slot->activeF = 1;
            break;
        }
    }
}

void fn_1_BE48(void *data, s32 *src)
{
    s32 i;
    EndingSlot2 *slot;

    for (i = 0; i < 64; i++) {
        slot = &lbl_1_bss_7824.slots2[i];
        if (slot->activeF == 0) {
            slot->state = 0;
            slot->activeF = 1;
            slot->data0 = src[0];
            slot->data1 = src[1];
            slot->data2 = src[2];
            break;
        }
    }
}

// NONMATCHING: register allocation differs in data copy (r3 vs r0 for src[0])

void fn_1_A454(void *data)
{
    Hu3DModelKill(lbl_1_bss_7824.header.modelId5);
    Hu3DModelKill(lbl_1_bss_7824.header.modelId4);
    Hu3DModelKill(lbl_1_bss_7824.header.modelId3);
    Hu3DModelKill(lbl_1_bss_7824.header.modelId2);
    Hu3DModelKill(lbl_1_bss_7824.header.modelId1);
    Hu3DMotionKill(lbl_1_bss_7824.header.motionId2);
    Hu3DMotionKill(lbl_1_bss_7824.header.motionId1);
    memset(&lbl_1_bss_7824, 0, sizeof(EndingBss));
}