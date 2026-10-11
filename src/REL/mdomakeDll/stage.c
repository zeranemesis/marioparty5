/* MSL's math.h emits the two unused sqrtf statics that the target's .rodata
 * does not carry; the guard suppresses them without losing the declarations. */
#define _MATH_H
#include "game/hu3d.h"
#include "game/sprite.h"
#include "dolphin/types.h"

typedef struct MdSprite_s {
    ANIMDATA *anim[9];
    s16 sprId[14];
    s16 grpId[1];
} MdSprite;

HuVec2f *MdSpriteGrpPosGet(MdSprite *spr, s32 no)
{
    HUSPRGRP *gp = &HuSprGrpData[spr->grpId[no]];
    return &gp->pos;
}

HuVec2f *MdSpriteGrpScaleGet(MdSprite *spr, s32 no)
{
    HUSPRGRP *gp = &HuSprGrpData[spr->grpId[no]];
    return &gp->scale;
}

HuVec2f *MdSpritePosGet(MdSprite *spr, s32 grpNo, s32 sprNo)
{
    HUSPRGRP *gp = &HuSprGrpData[spr->grpId[grpNo]];
    HUSPRITE *sp = &HuSprData[gp->sprId[sprNo]];
    return &sp->pos;
}

HuVec2f *MdSpriteScaleGet(MdSprite *spr, s32 grpNo, s32 sprNo)
{
    HUSPRGRP *gp = &HuSprGrpData[spr->grpId[grpNo]];
    HUSPRITE *sp = &HuSprData[gp->sprId[sprNo]];
    return &sp->scale;
}

float *MdSpriteAlphaGet(MdSprite *spr, s32 grpNo, s32 sprNo)
{
    HUSPRGRP *gp = &HuSprGrpData[spr->grpId[grpNo]];
    HUSPRITE *sp = &HuSprData[gp->sprId[sprNo]];
    return &sp->a;
}

void Layer2Hook(void)
{
    if (Hu3DCameraNo == 0) {
        Hu3DZClear();
    }
}

void Layer1Hook(void)
{
    if (Hu3DCameraNo == 1) {
        Hu3DZClear();
    }
}
