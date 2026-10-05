#include "REL/mdpartyDll.h"

void MdRingModeSet(int no, int mode)
{
    MDRING *ring = &(*(MDRING_WORK **)((u8 *)sysP + 0x3c8))->ring[no];
    ring->mode = mode;
}

void MdRingMdlSet(int no, int mdlNo)
{
    MDRING *ring = &(*(MDRING_WORK **)((u8 *)sysP + 0x3c8))->ring[no];
    ring->baseMdlNo = mdlNo;
}

HuVec2f *MdSpriteGrpPosGet(MDSPRITE_WORK *sprite, int grpNo)
{
    HUSPRGRP *grp = &HuSprGrpData[*(HUSPRGRPID *)((u8 *)sprite + 0x5c + grpNo * 2)];
    return &grp->pos;
}

HuVec2f *MdSpriteGrpScaleGet(MDSPRITE_WORK *sprite, int grpNo)
{
    HUSPRGRP *grp = &HuSprGrpData[*(HUSPRGRPID *)((u8 *)sprite + 0x5c + grpNo * 2)];
    return &grp->scale;
}

static void LayerPostHook(s16 layerNo)
{
    if (Hu3DCameraNo == 0) {
        Hu3DZClear();
    }
}

static void Layer2Hook(s16 layerNo)
{
    if (Hu3DCameraNo == 1) {
        Hu3DZClear();
    }
}