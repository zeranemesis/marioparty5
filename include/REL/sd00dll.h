#ifndef _SD00DLL_H
#define _SD00DLL_H

#include "dolphin.h"

typedef struct SdMachineTire_s {
    u8 pad00[0x1C];
    f32 unk1C;
    f32 unk20;
    u8 pad24[0x10];
    s32 unk34;
} SdMachineTire;

extern SdMachineTire sdMachineTireTbl[];
extern char *sdMachineHookTbl[];

/* NOTE: game/hu3d.h is intentionally not included here: it pulls
   humath.h -> MSL math.h whose `extern inline sqrtf` emits extra rodata
   (_half/_three) that the original hook unit does not have. */
void Hu3DModelHookSet(s16 modelId, char *objName, s16 hookMdlId);

f32 SdMachineHookSet(s32 machineNo, s16 modelId, s16 *hookMdlIds, s16 arg3, s16 arg4);

#endif
