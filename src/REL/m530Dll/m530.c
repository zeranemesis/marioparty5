#include "types.h"

typedef struct m530model_s {
    s32 unk0; //0x00
    u8 pad04[0x98];
    s32 unk9C; //0x9C
} M530_MODEL;

typedef struct m530owner_s {
    u8 pad00[0x5C];
    M530_MODEL *unk5C; //0x5C
} M530_OWNER;

typedef struct m530block_s {
    s32 v[18];
} M530_BLOCK;

void MgSeqModeNext(void);
void fn_1_6740(void);

M530_BLOCK lbl_1_bss_6C;

void fn_1_CE0(void) {
    fn_1_6740();
}

void fn_1_DAC(M530_OWNER *self, volatile s16 id) {
    if (id == 0x78) {
        MgSeqModeNext();
    }
}

void fn_1_23B0(M530_BLOCK *src) {
    lbl_1_bss_6C = *src;
}

s32 fn_1_31C0(M530_OWNER *owner) {
    M530_MODEL *mdl = owner->unk5C;

    if (mdl->unk9C & 0x2) {
        return 1;
    } else {
        return 0;
    }
}

s32 fn_1_3948(M530_OWNER *owner) {
    M530_MODEL *mdl = owner->unk5C;

    return mdl->unk0;
}
