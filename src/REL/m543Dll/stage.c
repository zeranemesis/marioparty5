#include "types.h"

typedef struct M543StageWork {
    u8 pad_00[0x58];
    s32 unk_58;
    s32 unk_5C[3];
    s32 unk_68[3];
    u8 pad_74[0xFC];
    s32 unk_170;
    s16 unk_174;
    u8 pad_176[0x172];
} M543StageWork;

M543StageWork *lbl_1_bss_15E8;

void fn_1_FCE0(s16 arg0) {
    lbl_1_bss_15E8->unk_174 = arg0;
}

void fn_1_FCF4(s16 arg0, s32 arg1) {
    lbl_1_bss_15E8->unk_5C[arg0] = arg1;
}

void fn_1_FD14(s16 arg0, s32 arg1) {
    lbl_1_bss_15E8->unk_68[arg0] = arg1;
}

void fn_1_FD34(void) {
}

/* fn_1_FD38, fn_1_10150 and fn_1_10304 are not written yet. */

void fn_1_106BC(void) {
}

void fn_1_106C0(void) {
}

void fn_1_106C4(void) {
}

void fn_1_106C8(void) {
}

void fn_1_106CC(void) {
}

void fn_1_106D0(void) {
}

void fn_1_106D4(void) {
}
