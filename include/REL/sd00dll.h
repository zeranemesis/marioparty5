#ifndef _SD00DLL_H
#define _SD00DLL_H

#include "dolphin.h"

typedef struct SdMachineTire_s {
    s32 dataNum[4]; // 0x00 (tire model files)
    s32 stat0;      // 0x10
    s32 stat1;      // 0x14
    s32 stat2;      // 0x18
    f32 unk1C;      // 0x1C (wheel base)
    f32 unk20;      // 0x20
    f32 unk24;      // 0x24
    f32 unk28;      // 0x28 (hook X offset)
    f32 unk2C;      // 0x2C
    f32 unk30;      // 0x30 (hook Z offset)
    s32 unk34;      // 0x34 (low nibble: hook layout 0/1/2)
} SdMachineTire;

extern SdMachineTire sdMachineTireTbl[];
extern char *sdMachineHookTbl[];

/* NOTE: game/hu3d.h is intentionally not included here: it pulls
   humath.h -> MSL math.h whose `extern inline sqrtf` emits extra rodata
   (_half/_three) that the original hook unit does not have. */
void Hu3DModelHookSet(s16 modelId, char *objName, s16 hookMdlId);

f32 SdMachineHookSet(s32 machineNo, s16 modelId, s16 *hookMdlIds, s16 arg3, s16 arg4);

#endif
