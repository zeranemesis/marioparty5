#include "dolphin/types.h"
#include "dolphin/os/OSThread.h"
#include "game/object.h"
#include "game/hu3d.h"

extern "C" {
extern void *lbl_1_data_2B8[];
extern void fn_1_6834(void);

void fn_1_4C04(void)
{
    omSysExitReq = TRUE;
}

void fn_1_507C(s16 *modelIdPtr, void *hookData)
{
    Hu3DData[*modelIdPtr].hookData = hookData;
}

Mtx *fn_1_5098(s16 *modelIdPtr)
{
    return &Hu3DData[*modelIdPtr].mtx;
}

void fn_1_6814(void *obj, void *parent)
{
    *(void **)obj = lbl_1_data_2B8;
    *(void **)((u8 *)parent + 0x5C) = obj;
    *(void **)((u8 *)parent + 0x14) = fn_1_6834;
}

void fn_1_3328(void *node)
{
    *(void **)((u8 *)*(void **)node + 4) = *(void **)((u8 *)node + 4);
    *(void **)*(void **)((u8 *)node + 4) = *(void **)node;
    *(void **)node = node;
    *(void **)((u8 *)node + 4) = node;
}

s32 fn_1_7300(void)
{
    return OSGetThreadPriority(OSGetCurrentThread());
}

}