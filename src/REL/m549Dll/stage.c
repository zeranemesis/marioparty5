#include "types.h"

typedef struct {
    u8 pad00[0x4EC];
    s16 unk4EC;
    u8 pad4EE[0x1A];
    s16 unk508;
} MgStageModel;

typedef struct {
    u8 pad00[0x58];
    void *unk58;
    s32 unk5C[3];
    s32 unk68[336];
    s32 unk5A8;
    u8 pad5AC[0x1C4];
    s32 unk770;
    u8 pad774[0x44];
    s32 unk7B8;
} MgStageState;

MgStageState *lbl_1_bss_1608;

extern void Hu3DModelAttrSet(s16 modelId, s32 attr);

void fn_1_12450(s16 idx, s32 val) {
    lbl_1_bss_1608->unk5C[idx] = val;
}

void fn_1_12470(s16 idx, s32 val) {
    lbl_1_bss_1608->unk68[idx] = val;
}

void fn_1_134AC(void) {
    lbl_1_bss_1608->unk5A8 = -1;
}

void fn_1_1448C(void) {
    lbl_1_bss_1608->unk770 = -1;
    Hu3DModelAttrSet(((MgStageModel *)lbl_1_bss_1608->unk58)->unk4EC, 1);
}

void fn_1_14720(void) {
    Hu3DModelAttrSet(((MgStageModel *)lbl_1_bss_1608->unk58)->unk508, 1);
    lbl_1_bss_1608->unk7B8 = -1;
}