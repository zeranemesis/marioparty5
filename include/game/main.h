#ifndef _GAME_MAIN_H
#define _GAME_MAIN_H

#include "version.h"
#include "dolphin.h"

SHARED_SYM extern BOOL NintendoDispF;
SHARED_SYM extern BOOL HuSRDisableF;
SHARED_SYM extern BOOL HuDvdErrWait;
SHARED_SYM extern u32 GlobalCounter;

void HuSysVWaitSet(s16 vWait);
s16 HuSysVWaitGet(s16 prev);

#endif