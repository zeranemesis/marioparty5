#include "types.h"
#include "string.h"

typedef struct ComBase_s ComBase;

typedef struct ComSub_s {
    ComBase *parent;
    u8 pad_04[0x84];
    s16 unk88;
    s16 unk8A;
    u8 pad_8C[0x20];
} ComSub;

struct ComBase_s {
    u8 pad_00[8];
    ComSub subs[4];
};

typedef struct Com_s {
    ComSub *unk0;
    u8 pad_04[0x10];
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s32 unk1C;
    s16 unk20;
    u8 pad_22[4];
    s16 unk26;
    s16 unk28;
    s32 unk2C;
    void *unk30;
    u8 pad_34[0x20];
} Com;

void fn_1_A024(Com *com)
{
    com->unk1C = 0;
    com->unk20 = 4;
    com->unk14 = 0;
    com->unk16 = 0;
    com->unk18 = 0;
}

s32 fn_1_E714(Com *com, void *arg1, void *arg2)
{
    com->unk26 = *(s16 *)((u8 *)arg1 + 0xE);
    com->unk28 = *(s16 *)((u8 *)arg1 + 0x10);
    com->unk18 = 1;
    com->unk2C = 0;
    com->unk30 = arg2;
    return 1;
}

void fn_1_9FF4(Com *com)
{
    memset(com, 0, sizeof(Com));
}

ComSub *fn_1_CDD8(Com *com, void *arg1)
{
    ComSub *current = com->unk0;
    ComBase *base = current->parent;
    s32 i;

    for (i = 0; i < 4; i++) {
        ComSub *sub = &base->subs[i];
        if (sub == current) {
            continue;
        }
        if (*(s16 *)((u8 *)arg1 + 0xE) != sub->unk88) {
            continue;
        }
        if (*(s16 *)((u8 *)arg1 + 0x10) != sub->unk8A) {
            continue;
        }
        return sub;
    }
    return NULL;
}

void fn_1_CAD8(s16 arg0, s16 *arg1, s16 *arg2)
{
    switch (arg0 + 7) {
    case 0:
        *arg1 = 0;
        *arg2 = -1;
        break;
    case 1:
        *arg1 = 1;
        *arg2 = -1;
        break;
    case 2:
        *arg1 = 1;
        *arg2 = 0;
        break;
    case 3:
        *arg1 = 1;
        *arg2 = 1;
        break;
    case 4:
        *arg1 = 0;
        *arg2 = 1;
        break;
    case 5:
        *arg1 = -1;
        *arg2 = 1;
        break;
    case 6:
        *arg1 = -1;
        *arg2 = 0;
        break;
    case 7:
        *arg1 = -1;
        *arg2 = -1;
        break;
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    default:
        *arg1 = 0;
        *arg2 = 0;
        break;
    }
}