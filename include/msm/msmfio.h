#ifndef _MSM_MSMFIO_H
#define _MSM_MSMFIO_H

#include "msm.h"

typedef struct MSMFio_s {
    /* 0x0 */ MSM_OPEN open;
    /* 0x4 */ MSM_READ read;
    /* 0x8 */ MSM_CLOSE close;
} MSM_FIO; // Size 0xC

BOOL msmFioClose(DVDFileInfo *fileInfo);
BOOL msmFioRead(DVDFileInfo *fileInfo, void *addr, s32 length, s32 offset);
BOOL msmFioOpen(s32 entrynum, DVDFileInfo *fileInfo);
void msmFioInit(MSM_OPEN open, MSM_READ read, MSM_CLOSE close);

#endif
