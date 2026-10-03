#include "REL/resultDll.h"

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];
extern s32 MBDataDir[];

void ObjectSetup(void);

s32 Result_FastF;
s32 Result_BgReadEndF;

s32 _prolog(void)
{
    const VoidFunc *ctor;
    for(ctor = _ctors; *ctor != 0; ctor++) {
        (*ctor)();
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const VoidFunc *dtor;
    for(dtor = _dtors; *dtor != 0; dtor++) {
        (*dtor)();
    }
}

void Result_BgRead(void)
{
    s32 statId;
    if (_CheckFlag(0x10000) == 0) {
        Result_BgReadEndF = 1;
        HuPrcEnd();
        while (1) {
            HuPrcVSleep();
        }
    }
    statId = HuDataDirReadAsync(MBDataDir[GwSystem.boardNo]);
    if (statId != -1) {
        while (HuDataGetAsyncStat(statId) == 0) {
            HuPrcVSleep();
        }
    }
    Result_BgReadEndF = 1;
    HuPrcEnd();
    while (1) {
        HuPrcVSleep();
    }
}

s16 Result_CharMotEnd(s16 playerNo)
{
    if (Hu3DMotionShiftIDGet(StageCharMdlId[playerNo]) == -1) {
        if (Hu3DMotionEndCheck(StageCharMdlId[playerNo]) != 0) {
            return 1;
        }
    }
    return 0;
}