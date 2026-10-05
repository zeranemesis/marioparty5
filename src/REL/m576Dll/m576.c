#include "types.h"
#include "dolphin/mtx.h"
#include "game/mg/seqman.h"
#include "game/process.h"
#include "game/mg/actman.h"

extern void *lbl_1_bss_2FC[];
extern u32 lbl_1_bss_254[];
extern Vec lbl_1_data_DC[];
extern void fn_1_AC4(void);

s8 fn_1_424(s32 idx)
{
    void *ptr = lbl_1_bss_2FC[idx];
    return ((s8 *)ptr)[0x12];
}

u32 fn_1_6764(void)
{
    return MgSeqModeNext();
}

void fn_1_6734(s16 arg)
{
    if (arg >= 0x3C) {
        MgSeqModeNext();
    }
}

void fn_1_C24(void)
{
    HUPROCESS *current = HuPrcCurrentGet();
    HuPrcChildCreate(fn_1_AC4, 0xC8, 0x1000, 0, current);
}

u32 fn_1_174(s32 idx, void *dest)
{
    if (dest != NULL) {
        Vec *src = &lbl_1_data_DC[idx * 3];
        *(Vec *)dest = *src;
    }
    return lbl_1_bss_254[idx];
}

void fn_1_9A4(s32 idx, HuVecF *pos)
{
    void *ptr = lbl_1_bss_2FC[idx];
    MGACTOR *actor = *(MGACTOR **)((u8 *)ptr + 0x3C);
    MgActorPosGet(actor, pos);
}