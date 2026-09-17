#ifndef _MSM_MSMMEM_H
#define _MSM_MSMMEM_H

#include "dolphin/types.h"

void msmMemFree(void *ptr);
void *msmMemAlloc(u32 size);
void msmMemInit(void *ptr, u32 size);

#endif
