#include "REL/sd00dll.h"

typedef struct SdSys_s {
    u8 pad000[0xA80];
    f32 speed; //0xA80
} SdSys;

extern SdSys *sys;

void StageObjCol(s32 objNo);
void StageBomheiUpdate(s32 objNo);

void StageDokanUpdate(s32 objNo)
{
    StageObjCol(objNo);
}

void StageBomheiBigUpdate(s32 objNo)
{
    StageBomheiUpdate(objNo);
}

void StageHamonSpeedSet(f32 speed)
{
    sys->speed = speed;
}