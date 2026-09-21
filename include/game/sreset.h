#ifndef _GAME_SRESET_H
#define _GAME_SRESET_H

#include "version.h"
#include "dolphin.h"

BOOL HuSoftResetButtonCheck(void);
void HuDvdErrDispInit(GXRenderModeObj *rmode, void *xfb1, void *xfb2);
void HuRestartSystem(void);
BOOL HuSoftResetCountCheck(void);
BOOL HuSoftResetCheck(void);

SHARED_SYM extern BOOL SR_ExecReset;
SHARED_SYM extern BOOL SR_ExecResetMenu;

#endif
