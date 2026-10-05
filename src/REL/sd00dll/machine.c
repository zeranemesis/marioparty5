#include "REL/sd00dll.h"
#include "game/esprite.h"
#include "game/data.h"
#include "game/hu3d.h"
#include "game/memory.h"

typedef struct MapSprWork_s {
    u8 pad00[0x24];
    s16 sprId;
} MapSprWork;

typedef struct MDPARTICLE_PARAM_s {
    u8 pad00[0x30];
    s16 unk30;
    u8 pad32[0x14];
    void *unk48;
} MDPARTICLE_PARAM;

extern void *sys;
extern void DCStoreRangeNoSync(void *address, u32 nBytes);
extern void SdImpactSet(int arg0, void *pos, float f1, int arg3, float f2);
typedef struct SdMachineBodyEntry_s {
    int dataNum;
    u8 pad04[0x18];
} SdMachineBodyEntry;

extern SdMachineBodyEntry sdMachineBodyTbl[];
extern SdMachineBodyEntry sdMachineGunTbl[];
extern int playerModelTbl[];

void MapSprKill(void)
{
    MapSprWork *work = (MapSprWork *)((u8 *)sys + 0xAA0);
    espKill(work->sprId);
}

// NONMATCHING: stack frame size and register spilling differs (original uses 0x20 frame with r0 spill at 0x8(r1), compiler uses 0x10 frame)
void SdMachinePartcleHook(void *particle, MDPARTICLE_PARAM *param)
{
    DCStoreRangeNoSync(param->unk48, param->unk30 * 0x44);
}

// NONMATCHING: register allocation differs (original uses r31 for output ptr, compiler uses r29/r30)
HU3DMODELID SdMachineBodyCreate(int dataNum, s16 *modelIdOut)
{
    void *data = HuDataSelHeapReadNum(sdMachineBodyTbl[dataNum].dataNum, HU_MEMNUM_OVL, HEAP_MODEL);
    HU3DMODELID modelId = Hu3DModelCreate(data);
    *modelIdOut = modelId;
    return modelId;
}

// NONMATCHING: register allocation differs (original uses r31 for output ptr, compiler uses r29/r30)
HU3DMODELID SdMachineGunCreate(int dataNum, s16 *modelIdOut)
{
    void *data = HuDataSelHeapReadNum(sdMachineGunTbl[dataNum].dataNum, HU_MEMNUM_OVL, HEAP_MODEL);
    HU3DMODELID modelId = Hu3DModelCreate(data);
    *modelIdOut = modelId;
    return modelId;
}

// NONMATCHING: register allocation differs (original uses r31 for output ptr, compiler uses r29/r30)
HU3DMODELID SdMachineCharCreate(int dataNum, s16 *modelIdOut)
{
    void *data = HuDataSelHeapReadNum(playerModelTbl[dataNum], HU_MEMNUM_OVL, HEAP_MODEL);
    HU3DMODELID modelId = Hu3DModelCreate(data);
    *modelIdOut = modelId;
    return modelId;
}

// NONMATCHING: register allocation differs (original uses r30 for pos ptr and saves it, compiler uses r31)
void SdImpactStart(int arg0, int *pos, float f1, int arg3, float f2)
{
    int localPos[3];
    localPos[0] = pos[0];
    localPos[1] = pos[1];
    localPos[2] = pos[2];
    SdImpactSet(arg0, localPos, f1, arg3, f2);
}