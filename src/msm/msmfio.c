#include "msm/msmfio.h"
#ifdef TARGET_PC
#include "port/dvd_timing.h"
#endif

static MSM_FIO fio;

BOOL msmFioClose(DVDFileInfo *fileInfo)
{
    return fio.close(fileInfo);
}

BOOL msmFioRead(DVDFileInfo *fileInfo, void *addr, s32 length, s32 offset)
{
    return fio.read(fileInfo, addr, length, offset, 2);
}

BOOL msmFioOpen(s32 entrynum, DVDFileInfo *fileInfo)
{
    return fio.open(entrynum, fileInfo);
}

#ifdef TARGET_PC
/* DVDReadPrio blocks on the console: with the GameCube loading speed the caller waits for the drive */
static s32 msmFioReadTimed(DVDFileInfo *fileInfo, void *addr, s32 length, s32 offset, s32 prio)
{
    s32 result = DVDReadPrio(fileInfo, addr, length, offset, prio);
    if(Mp5Native_DvdTimingOn()) {
        Mp5Native_DvdTimingSleepUntil(Mp5Native_DvdTimingRead(fileInfo->startAddr+offset, length));
    }
    return result;
}
#endif

void msmFioInit(MSM_OPEN open, MSM_READ read, MSM_CLOSE close)
{
    if (open) {
        fio.open = open;
    } else {
        fio.open = DVDFastOpen;
    }
    if (read) {
        fio.read = read;
    } else {
#ifdef TARGET_PC
        fio.read = msmFioReadTimed;
#else
        fio.read = DVDReadPrio;
#endif
    }
    if (close) {
        fio.close = close;
    } else {
        fio.close = DVDClose;
    }
}
