#include "types.h"
#include "game/audio.h"
#include "game/frand.h"

s32 lbl_1_bss_104;
s32 *lbl_1_bss_108[18];

void fn_1_6D9C(s32 *arg0)
{
    if (arg0[0] == 1) {
        arg0[0] = 2;
    }
}

void fn_1_735C(void)
{
    s32 i;

    for (i = 0; i < 17; i++) {
        if (lbl_1_bss_108[i][0] == 0) {
            lbl_1_bss_108[i][0] = 1;
            lbl_1_bss_108[i][1] += frandmod(0x5A) * 8;
            lbl_1_bss_108[i][2] = frandmod(2);
        }
    }
}

void fn_1_741C(void)
{
    s32 i;

    for (i = 0; i < 17; i++) {
        if (lbl_1_bss_108[i][0] == 0) {
            lbl_1_bss_108[i][0] = 2;
            lbl_1_bss_108[i][2] = frandmod(2);
        }
    }
}

void fn_1_74B0(void)
{
    lbl_1_bss_104 = HuAudFXPlay(0x39D);
}
