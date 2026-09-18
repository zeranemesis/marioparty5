#ifndef _MSM_MSMSTREAM_H
#define _MSM_MSMSTREAM_H

#include "msm/msmsys.h"

void msmStreamPeriodicProc(void);
void msmStreamSetOutputMode(s32 outputMode);
void msmStreamAmemFree(void);
s32 msmStreamAmemAlloc(void);
s32 msmStreamInit(char *pdtPath);

#endif
