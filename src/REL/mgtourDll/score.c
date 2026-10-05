#include "types.h"

typedef void ANIMDATA;

typedef struct MGTOUR_WORK {
    /* 0x00 */ u8 pad_00[0xC];
    /* 0x0C */ s16 field_0C;
    /* 0x0E */ u8 pad_0E[0x1E];
    /* 0x2C */ s16 score_2C[4];
    /* 0x34 */ ANIMDATA *anim_34[4];
    /* 0x44 */ u8 pad_44[4];
} MGTOUR_WORK;

MGTOUR_WORK lbl_1_bss_177A0;

void HuSprAnimKill(ANIMDATA *anim);

void fn_1_BB14(s8 *str, s32 len)
{
    s8 tmp[0x10];
    s32 i;

    i = len;
    while (i--) {
        tmp[i] = str[i];
    }
    for (i = 1; i < len; i++) {
        str[i - 1] = tmp[i];
    }
    str[len - 1] = tmp[0];
}

void fn_1_C76C(void)
{
    s32 i;

    i = 4;
    while (i--) {
        HuSprAnimKill(lbl_1_bss_177A0.anim_34[i]);
    }
}

void fn_1_C7BC(s32 *param)
{
    s32 i;

    i = 4;
    while (i--) {
        lbl_1_bss_177A0.score_2C[i] = (s16)param[i];
    }
}

void fn_1_D61C(s32 val)
{
    lbl_1_bss_177A0.field_0C = (s16)val;
}

s32 fn_1_D630(void)
{
    return lbl_1_bss_177A0.field_0C == 1;
}
