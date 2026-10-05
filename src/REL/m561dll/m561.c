#include "game/audio.h"
#include "game/hu3d.h"
#include "REL/m561dll.h"

/* m561.o defines these two .bss items itself (nm reports B, not U), so they
   are storage here rather than externs. The original block spans 0x0-0x2DC;
   only the two words these five functions touch are spelled out. */
s32 lbl_1_bss_88;
s32 lbl_1_bss_2D4[3];

/* The per-model parameter block reached through the model's 0x120 pointer.
   m561dll.h stops it at 0x4C; fn_1_8690 pokes a byte flag at 0x6A. */
typedef struct M561ParmExt_s {
    u8 pad00[0x6A];
    u8 flag6A;
} M561ParmExt;

void fn_1_5D58(void)
{
    lbl_1_bss_88 = -1;
}

void fn_1_5D6C(void)
{
    lbl_1_bss_88 = HuAudSStreamPlay(0x3F);
}

void fn_1_5D9C(void)
{
    HuAudSStreamFadeOut(lbl_1_bss_88, 0x64);
}

void fn_1_6210(void)
{
    lbl_1_bss_2D4[1] = 0;
    lbl_1_bss_2D4[2] = 0;
}

void fn_1_8690(s16 arg0, u8 arg1)
{
    M561Hu3DModel *model = (M561Hu3DModel *)&Hu3DData[arg0];

    ((M561ParmExt *)model->parm)->flag6A = arg1;
}