#ifndef _GAME_INIT_H
#define _GAME_INIT_H

#include "version.h"
#include "dolphin.h"

SHARED_SYM extern GXRenderModeObj *RenderMode;
SHARED_SYM extern OSHeapHandle currentHeapHandle;

SHARED_SYM extern void *DemoFrameBuffer1;
SHARED_SYM extern void *DemoFrameBuffer2;
SHARED_SYM extern void *DemoCurrentBuffer;
SHARED_SYM extern u32 minimumVcount;
SHARED_SYM extern float minimumVcountf;
SHARED_SYM extern u32 worstVcount;

void HuSysInit(GXRenderModeObj *mode);
void HuSysBeforeRender();
void HuSysDoneRender(s32 retraceCount);

#endif