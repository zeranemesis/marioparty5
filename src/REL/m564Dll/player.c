#include "types.h"

typedef struct M564Model_s {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s32 unk04;
} M564_MODEL;

typedef struct M564Player_s {
    /* 0x00 */ u8 pad0[0x28];
    /* 0x28 */ s32 unk28;
    /* 0x2C */ u8 pad2C[8];
    /* 0x34 */ s16 unk34;
    /* 0x36 */ s16 unk36;
    /* 0x38 */ s16 unk38;
    /* 0x3A */ u8 pad3A[2];
    /* 0x3C */ M564_MODEL *unk3C;
} M564_PLAYER;

typedef struct M564Entry_s {
    /* 0x00 */ u8 pad0[0x2C];
    /* 0x2C */ M564_PLAYER *unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s16 unk38;
    /* 0x3A */ u8 pad3A[2];
    /* 0x3C */ s32 unk3C;
} M564_ENTRY;

typedef struct M564Entry2_s {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s16 unk04;
    /* 0x06 */ u8 pad6[0x14];
    /* 0x1A */ s16 unk1A;
} M564_ENTRY2;

typedef struct M564Idx_s {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s32 unk04;
} M564_IDX;

typedef struct M564Hook_s {
    /* 0x00 */ s16 unk00;
    /* 0x02 */ u8 pad2[0x3A];
    /* 0x3C */ M564_MODEL *unk3C;
} M564_HOOK;

extern M564_ENTRY *lbl_1_bss_100[];
extern M564_ENTRY2 *lbl_1_bss_1E8[];

extern void MgPlayerModeSet(M564_PLAYER *player, s16 mode);
extern void Hu3DModelHookReset(s16 modelId);
extern void Hu3DModelObjPosGet(s16 modelId, void *objName, void *pos);

void fn_1_43E0(M564_PLAYER *player, s16 mode)
{
    player->unk34 = 0;
    player->unk36 = 0;
    MgPlayerModeSet(player, mode);
}

s32 fn_1_4428(M564_IDX *a, M564_IDX *b)
{
    M564_ENTRY *entry = lbl_1_bss_100[a->unk04];
    s16 idx = (s16)(b->unk04 - 4);

    if (idx >= 0 && a->unk04 == lbl_1_bss_1E8[idx]->unk1A) {
        return 0;
    }
    return 1;
}

void fn_1_5DF4(M564_HOOK *hook)
{
    M564_ENTRY *entry = lbl_1_bss_100[hook->unk00];

    Hu3DModelHookReset((s16)hook->unk3C->unk04);
    entry->unk34 = 0;
    entry->unk38 = -1;
}

s32 fn_1_5E5C(s16 idx)
{
    if (idx == -1) {
        return 1;
    }
    if (lbl_1_bss_100[idx]->unk2C->unk28 == 3) {
        return 1;
    }
    return 0;
}

void fn_1_5EA8(s16 idx, void *pos)
{
    M564_ENTRY *entry = lbl_1_bss_100[idx];

    Hu3DModelObjPosGet((s16)entry->unk2C->unk3C->unk04, (void *)entry->unk30, pos);
}