#include "types.h"

void MgSeqModeNext(void);
s32 fn_1_1104C(void);

typedef struct M580UnkB_s {
    /* 0x00 */ void *unk00;
} M580_UNKB;

typedef struct M580UnkA_s {
    /* 0x00 */ u8 pad00[0x5C];
    /* 0x5C */ M580_UNKB *unk5C;
} M580_UNKA;

s16 lbl_1_bss_10[4];
s16 lbl_1_bss_18[4];
s32 lbl_1_bss_20;
s32 lbl_1_bss_24;
M580_UNKA *lbl_1_bss_44;

void fn_1_548(void)
{
    if (fn_1_1104C() == 0) {
        MgSeqModeNext();
    }
}

void *fn_1_18A8(void)
{
    M580_UNKA *a = lbl_1_bss_44;
    M580_UNKB *b = a->unk5C;
    return b->unk00;
}

s16 fn_1_1D40(s32 index)
{
    return lbl_1_bss_10[index];
}

void fn_1_1EC4(u32 index)
{
    /* NONMATCHING: MWCC hoists `li r6,1` ahead of the mask and puts the mask in r0
   (clrlwi r0,r3,30) where the original keeps it in place (clrlwi r3,r3,30).
   Register tie-break; the address form and every other row are identical. */
lbl_1_bss_18[index & 3] = 1;
}

void fn_1_246C(s32 arg)
{
    lbl_1_bss_24 = arg;
    lbl_1_bss_20 = 0x12;
}