#include "game/data.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/sprite.h"
#include "REL/m561dll.h"

s16 lbl_1_bss_2E0[16];
void *lbl_1_bss_300;

M561EffectParam lbl_1_data_5A0 = {
    0,
    { 0x00, 0x00, 0x00, 0x80 },
    { 0x00, 0x00, 0x00, 0x00 },
    { 0.0f, 2.0f, 1.0f },
    { 0.95f, 0.99f, 0.95f },
    0.0f,
    0.0f,
    1.5f,
    -4.0f,
    0.0f,
};

void fn_1_8AE8(void)
{
    s32 i;

    for (i = 0; i < 16; i++) {
        lbl_1_bss_2E0[i] = -1;
    }
    lbl_1_bss_300 = NULL;
}

void fn_1_8B38(void)
{
    s32 i;

    if (lbl_1_bss_300) {
        HuMemDirectFree(lbl_1_bss_300);
        lbl_1_bss_300 = NULL;
    }

    for (i = 0; i < 16; i++) {
        lbl_1_bss_2E0[i] = -1;
    }
    lbl_1_bss_300 = NULL;
}

void fn_1_8BCC(s16 arg0, void *arg1)
{
    s32 i;
    s32 j;
    Vec *vec = (Vec *)arg1;

    lbl_1_data_5A0.colorStart.a = 0x80;

    for (i = 0; i < 8; i++) {
        lbl_1_data_5A0.vel.x =
            2.0f * 3.5 * (f32)sin((3.141592653589793 * (270.0f + 45.0f * i)) / 180.0);
        lbl_1_data_5A0.vel.y = 1.0f;
        lbl_1_data_5A0.vel.z =
            2.0f * 3.5 * (f32)cos((3.141592653589793 * (270.0f + 45.0f * i)) / 180.0);
        fn_1_8F5C(arg0, &lbl_1_data_5A0, vec->x, vec->y * 2.0f, vec->z, 50.0f);
    }

    for (j = 0; j < 8; j++) {
        lbl_1_data_5A0.vel.x =
            2.0f * 1.5 * (f32)sin((3.141592653589793 * (270.0 + 22.5 + 45.0f * j)) / 180.0);
        lbl_1_data_5A0.vel.y = 1.0f;
        lbl_1_data_5A0.vel.z =
            2.0f * 1.5 * (f32)cos((3.141592653589793 * (270.0 + 22.5 + 45.0f * j)) / 180.0);
        fn_1_8F5C(arg0, &lbl_1_data_5A0, vec->x, vec->y * 2.0f, vec->z, 30.0f);
    }

    lbl_1_data_5A0.colorStart.a = 0xFF;
}
