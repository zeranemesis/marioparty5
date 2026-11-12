#ifndef _GAME_FAULT_H
#define _GAME_FAULT_H
#ifdef __cplusplus
extern "C" {
#endif

#include "dolphin.h"

void HuFaultInitXfbDirectDraw(GXRenderModeObj *mode);
void HuFaultSetXfbAddress(s16 index, void* value);

#ifdef __cplusplus
}
#endif
#endif
