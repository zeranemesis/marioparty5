#include "types.h"

#include "game/wipe.h"

s32 lbl_1_bss_20[2];

typedef struct {
    u8 pad0[6];
    s16 unk6;
} M548_PLAYER;

M548_PLAYER lbl_1_bss_3D0[4];

void fn_1_5DF4(void);
void fn_1_202C(void);

void MgSeqModeNext(void);

void fn_1_A54(void)
{
    fn_1_5DF4();
    fn_1_202C();
}

void fn_1_A78(void)
{
    fn_1_202C();
    MgSeqModeNext();
}

void fn_1_A9C(void)
{
    fn_1_202C();
    if (WipeCheckEnd() == 0) {
        MgSeqModeNext();
    }
}

void fn_1_AD0(void)
{
}

void fn_1_194C(void)
{
    lbl_1_bss_20[0] = lbl_1_bss_20[1] = 0;
}

s32 fn_1_196C(void)
{
    return lbl_1_bss_20[lbl_1_bss_3D0[0].unk6] % 32;
}
