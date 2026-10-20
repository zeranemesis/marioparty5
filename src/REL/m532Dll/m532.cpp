#include "dolphin/types.h"
#include "dolphin/os/OSThread.h"
#include "dolphin/mtx.h"
#include "game/object.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/wipe.h"

extern "C" {
extern void *lbl_1_data_2B8[];
extern void fn_1_6834(void);

extern u32 lbl_1_bss_2C;

extern void MgTimerValueGet(void *timer);
extern void MgTimerModeOffSet(void *timer);

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

void fn_1_1C10(void *ptr)
{
    if (ptr != NULL) {
        HuMemDirectFree(ptr);
    }
}

void fn_1_1C48(void *ptr)
{
    if (ptr != NULL) {
        HuMemDirectFree(ptr);
    }
}

void fn_1_20B4(void *arg0, Vec *arg1)
{
    PSVECAdd(arg1, (Vec *)((u8 *)arg0 + 0x18), (Vec *)((u8 *)arg0 + 0x18));
    PSVECAdd(arg1, (Vec *)arg0, (Vec *)arg0);
}

void fn_1_24E8(void *arg0, u16 cameraBit)
{
    Hu3DCameraPosSetV(cameraBit, (Vec *)arg0, (Vec *)((u8 *)arg0 + 0xC),
                      (Vec *)((u8 *)arg0 + 0x18));
}

void fn_1_2528(void *arg0, u16 cameraBit)
{
    Hu3DCameraPosGet(cameraBit, (Vec *)arg0, (Vec *)((u8 *)arg0 + 0xC),
                     (Vec *)((u8 *)arg0 + 0x18));
}

void fn_1_32B0(void *node)
{
    *(void **)node = node;
    *(void **)((u8 *)node + 4) = node;
}

void fn_1_334C(void *self, void *node)
{
    *(void **)((u8 *)self + 4) = node;
    *(void **)self = *(void **)node;
    *(void **)((u8 *)*(void **)node + 4) = self;
    *(void **)node = self;
}

void fn_1_337C(void *self, void *node)
{
    *(void **)self = node;
    *(void **)((u8 *)self + 4) = *(void **)((u8 *)node + 4);
    *(void **)*(void **)((u8 *)node + 4) = self;
    *(void **)((u8 *)node + 4) = self;
}

u32 fn_1_3EF0(void)
{
    return lbl_1_bss_2C;
}

void fn_1_442C(void)
{
}

void fn_1_4BAC(void)
{
    WipeCreate(1, 5, 0x3C);
}

void fn_1_4BD8(void)
{
    WipeCreate(2, 6, 0x3C);
}

void fn_1_4DC8(void *arg0, u32 attr)
{
    Hu3DModelAttrSet(*(s16 *)arg0, attr);
}

void fn_1_4E00(void *arg0, u32 attr)
{
    Hu3DModelAttrReset(*(s16 *)arg0, attr);
}

void fn_1_51A4(void *arg0)
{
    Hu3DModelShadowSet(*(s16 *)arg0);
}

void fn_1_51D4(void *arg0)
{
    Hu3DModelShadowMapSet(*(s16 *)arg0);
}

void fn_1_5204(void *arg0, char *objName)
{
    Hu3DModelShadowMapObjSet(*(s16 *)arg0, objName);
}

void fn_1_523C(void *arg0)
{
    Hu3DModelShadowMapObjReset(*(s16 *)arg0);
}

void fn_1_6880(void)
{
}

void fn_1_7CC8(void *arg0)
{
    MgTimerValueGet(*(void **)arg0);
}

void fn_1_7E3C(void *arg0)
{
    MgTimerModeOffSet(*(void **)arg0);
}

void fn_1_7E6C(void *arg0)
{
    if (*(s32 *)((u8 *)*(void **)arg0 + 0x14) == 0) {
        *(s32 *)((u8 *)*(void **)arg0 + 0x14) = 1;
        *(s32 *)((u8 *)*(void **)arg0 + 0x0) = 1;
    }
}

}