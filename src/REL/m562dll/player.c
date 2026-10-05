#include "types.h"

extern void *memset(void *dst, s32 val, u32 len);
extern char *CharModelItemHookGet(s16 charNo, s32 modelId, s32 hookId);
extern void Hu3DModelObjMtxGet(s16 modelId, char *hookName, void *dst);

typedef struct {
    u8 unk0[2];
    s16 unk2;
    u8 pad4[6];
} GwPlayerConfEntry;

extern GwPlayerConfEntry GwPlayerConf[];

typedef struct {
    u8 unk0[4];
    u16 unk4;
    u16 unk6;
} PlayerData;

typedef struct {
    u8 pad0[0x18];
    s32 charNo;
    u8 pad1C[0x8];
    s16 hookId;
    u8 pad26[0xA2];
    u8 mtxC8[0x3C];
    u8 pad104[0x24];
    s32 flag128;
} PlayerWork;

void fn_1_AD68(PlayerData *arg0) {
    if (arg0->unk4 == 2) {
        if (arg0->unk6 < 3) {
            arg0->unk6 = 3;
        }
    } else {
        arg0->unk6 = 3;
    }
}

void fn_1_B0E0(void *arg0) {
    memset(arg0, 0, 0x1C);
}

// NONMATCHING: register allocation uses r30 for hook parameter instead of r4
void *fn_1_7044(PlayerWork *arg0) {
    if (arg0->flag128 == 0) {
        char *hook;
        hook = CharModelItemHookGet((s16)arg0->charNo, 2, 0);
        Hu3DModelObjMtxGet(arg0->hookId, hook, (void *)((u8 *)arg0 + 0xC8));
        arg0->flag128 = 1;
    }
    return (void *)((u8 *)arg0 + 0xC8);
}

// NONMATCHING: register allocation uses r30 for hook parameter instead of r4
void *fn_1_70A8(PlayerWork *arg0) {
    if (*(s32 *)((u8 *)arg0 + 0x12C) == 0) {
        char *hook;
        hook = CharModelItemHookGet((s16)*(u32 *)((u8 *)arg0 + 0x18), 2, 1);
        Hu3DModelObjMtxGet(*(s16 *)((u8 *)arg0 + 0x24), hook, (void *)((u8 *)arg0 + 0xF8));
        *(s32 *)((u8 *)arg0 + 0x12C) = 1;
    }
    return (void *)((u8 *)arg0 + 0xF8);
}

void fn_1_B07C(void *arg0, s32 arg1, s32 arg2) {
    memset(arg0, 0, 0x1C);
    *(s32 *)arg0 = arg2;
    *(s32 *)((u8 *)arg0 + 0x4) = GwPlayerConf[arg1].unk2;
}

void fn_1_710C(void *arg0) {
    if (*(f32 *)((u8 *)arg0 + 0xA4) > 0.0f) {
        *(f32 *)((u8 *)arg0 + 0xC4) = *(f32 *)((u8 *)arg0 + 0xC4) - 1.5f;
        *(f32 *)((u8 *)arg0 + 0xA4) = *(f32 *)((u8 *)arg0 + 0xA4) + *(f32 *)((u8 *)arg0 + 0xC4);
    } else {
        *(f32 *)((u8 *)arg0 + 0xC4) = 0.0f;
        *(f32 *)((u8 *)arg0 + 0xA4) = 0.0f;
    }
}