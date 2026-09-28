#include "REL/sd00dll.h"

// NONMATCHING: instruction-identical (fndiff 100%); our 0.0f literal is
// emitted as 4-byte rodata but the original has an 8-byte double-zero
// (lbl_1_rodata_6A0), so the unit cannot be marked Matching.
f32 SdMachineHookSet(s32 machineNo, s16 modelId, s16 *hookMdlIds, s16 arg3, s16 arg4)
{
    f32 ret = 0.0;
    s32 mode;
    s32 tmp;
    s32 i;

    if (modelId == -1) {
        return ret;
    }
    tmp = sdMachineTireTbl[machineNo].unk34;
    mode = tmp & 0xF;
    for (i = 0; i < 4; i++) {
        if (hookMdlIds[i] == -1) {
            continue;
        }
        if (mode == 0) {
            Hu3DModelHookSet(modelId, sdMachineHookTbl[i], hookMdlIds[i]);
            ret = sdMachineTireTbl[machineNo].unk1C;
        } else if (mode == 1) {
            if (i < 2) {
                Hu3DModelHookSet(modelId, sdMachineHookTbl[i + 4], hookMdlIds[i]);
                ret = sdMachineTireTbl[machineNo].unk1C;
            }
        } else {
            if (i < 1) {
                Hu3DModelHookSet(modelId, sdMachineHookTbl[7], hookMdlIds[i]);
                ret = sdMachineTireTbl[machineNo].unk1C + sdMachineTireTbl[machineNo].unk20;
            }
        }
    }
    if (arg3 != -1) {
        Hu3DModelHookSet(modelId, sdMachineHookTbl[6], arg3);
    }
    if (arg4 != -1) {
        Hu3DModelHookSet(modelId, sdMachineHookTbl[12], arg4);
    }
    return ret;
}
