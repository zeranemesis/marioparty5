// rescardDll/stage.c - decompilation of the rescardDll/stage unit

#include "types.h"
#include "game/sprite.h"
#include "game/hu3d.h"

/* Forward declaration for the structure containing sprite group IDs at offset 0x38 */
typedef struct MdStage_s {
    char pad[0x38];
    HUSPRGRPID grpId[16]; // size unknown, at least enough for the indices used
} MdStage;

HuVec2f *MdSpriteGrpPosGet(MdStage *stage, s32 memberNo)
{
    HUSPRGRP *grp = &HuSprGrpData[stage->grpId[memberNo]];
    return &grp->pos;
}

HuVec2f *MdSpriteGrpScaleGet(MdStage *stage, s32 memberNo)
{
    HUSPRGRP *grp = &HuSprGrpData[stage->grpId[memberNo]];
    return &grp->scale;
}