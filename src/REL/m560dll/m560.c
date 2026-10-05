#include "types.h"

/* Stream id of the currently playing BGM stream (lbl_1_bss_24) */
static s32 lbl_1_bss_24;

/* Playback state block (lbl_1_bss_510), cleared two words at a time */
static s32 lbl_1_bss_510[4];

int HuAudSStreamPlay(s16 streamId);
void HuAudSStreamFadeOut(int streamNo, s32 speed);

/*
 * The shared game/hu3d.h Hu3DModel_s does not describe this build: the object
 * indexes Hu3DData with a stride of 0x134, while the header struct is 0x144
 * bytes and its field at 0x120 lands inside the matrix. Offsets below are taken
 * from the code of this unit instead.
 */

/* Pointee of the field at 0x120; no header describes this layout */
typedef struct M560Hu3DAnimData_s {
    /* 0x00 */ s8 pad_00[0x6A];
    /* 0x6A */ u8 unk6A;
} M560_HU3DANIMDATA;

typedef struct M560Hu3DData_s {
    /* 0x000 */ s8 pad_000[0x120];
    /* 0x120 */ M560_HU3DANIMDATA *unk120;
    /* 0x124 */ s8 pad_124[0x10];
} M560_HU3DDATA;

extern M560_HU3DDATA Hu3DData[512];

void fn_1_381C(void)
{
    lbl_1_bss_24 = -1;
}

void fn_1_3830(void)
{
    lbl_1_bss_24 = HuAudSStreamPlay(0x3F);
}

void fn_1_3860(void)
{
    HuAudSStreamFadeOut(lbl_1_bss_24, 0x64);
}

void fn_1_3C8C(void)
{
    lbl_1_bss_510[1] = 0;
    lbl_1_bss_510[2] = 0;
}

void fn_1_6234(s16 modelId, u8 value)
{
    M560_HU3DDATA *modelP = &Hu3DData[modelId];

    modelP->unk120->unk6A = value;
}