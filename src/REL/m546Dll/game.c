#include "types.h"

extern void fn_1_301C(void);
extern void fn_1_4010(void);
extern void fn_1_58B0(void);
extern void fn_1_7190(void);
extern void fn_1_A4EC(void);
extern s16 fn_1_AC58(void);

extern s16 lbl_1_bss_14A10[];
extern s16 lbl_1_bss_14AB8[];

typedef struct {
    u8 pad_00[0x5C];
    s16 unk5C; // 0x5C
} Lbl_1Bss14C40;

extern Lbl_1Bss14C40 lbl_1_bss_14C40;

s32 fn_1_6A90(void) {
    s32 ret = 0;
    fn_1_A4EC();
    fn_1_7190();
    return ret;
}

s32 fn_1_6CD4(void) {
    s32 ret = 0;
    if (lbl_1_bss_14C40.unk5C == 0) {
        lbl_1_bss_14C40.unk5C = 1;
    }
    return ret;
}

s32 fn_1_6D14(void) {
    s32 ret = 0;
    ret = fn_1_AC58();
    fn_1_7190();
    return ret;
}

s32 fn_1_6E94(void) {
    s32 ret = 1;
    fn_1_7190();
    return ret;
}

s32 fn_1_6EC4(void) {
    s32 ret = 1;
    fn_1_58B0();
    if (lbl_1_bss_14AB8[0] != 0) {
        fn_1_301C();
    }
    if (lbl_1_bss_14A10[0] != 0) {
        fn_1_4010();
    }
    return ret;
}