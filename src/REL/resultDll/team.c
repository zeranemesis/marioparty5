#include "REL/resultDll.h"

void Result_TeamSetTPLvl(s16 level, float scale)
{
    if (level != 0) {
        Hu3DModelScaleSet(StageToueiMdlId[3], scale, scale, scale);
        Hu3DModelScaleSet(StageToueiMdlId[2], scale, scale, scale);
    } else {
        Hu3DModelScaleSet(StageToueiMdlId[1], scale, scale, scale);
        Hu3DModelScaleSet(StageToueiMdlId[0], scale, scale, scale);
    }
}