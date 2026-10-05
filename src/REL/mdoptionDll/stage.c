#include "game/hu3d.h"
#include "game/sprite.h"

typedef struct Sys_s {
    u8 pad_000[0x3B8];
    float waveEffectSpeed;
    u8 pad_3BC[0x18];
    GXColor waveEffectColor;
    u8 pad_3D8[0xD8];
    int capsuleOrbitState;
    int capsuleOrbitArg;
} SYS;

typedef struct MdSpriteWork_s {
    u8 pad_000[0x28E];
    HUSPRGRPID grpId[32];
} MDSPRITE_WORK;

extern void *sysP;

void MdOptionWaveEffectSpeedSet(float speed)
{
    ((SYS *)sysP)->waveEffectSpeed = speed;
}

void MdOptionCapsuleOrbitHideSet(void)
{
    ((SYS *)sysP)->capsuleOrbitState = 0;
}

void LayerHook(s16 layerNo)
{
    Hu3DZClear();
}

void MdOptionCapsuleOrbitOutSet(int arg)
{
    ((SYS *)sysP)->capsuleOrbitState = 1;
    ((SYS *)sysP)->capsuleOrbitArg = arg;
}

void MdOptionWaveEffectColorSet(GXColor *color)
{
    SYS *sys = *(SYS **)sysP;
    sys->waveEffectColor = *color;
}

HuVec2f *MdSpriteGrpPosGet(MDSPRITE_WORK *sprite, int grpNo)
{
    u16 grpId = sprite->grpId[grpNo];
    return &HuSprGrpData[grpId].pos;
}