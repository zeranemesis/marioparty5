#include <dolphin/os.h>

typedef struct OSSemaphore {
    volatile s32 count;
    OSThreadQueue queue;
} OSSemaphore;

void OSInitSemaphore(OSSemaphore *sem, s32 count)
{
    BOOL enabled = OSDisableInterrupts();
    OSInitThreadQueue(&sem->queue);
    sem->count = count;
    OSRestoreInterrupts(enabled);
}

s32 OSWaitSemaphore(OSSemaphore *sem)
{
    BOOL enabled;
    s32 count;

    enabled = OSDisableInterrupts();
    while ((count = sem->count) <= 0) {
        OSSleepThread(&sem->queue);
    }
    --sem->count;
    OSRestoreInterrupts(enabled);
    return count;
}

s32 OSSignalSemaphore(OSSemaphore *sem)
{
    BOOL enabled;
    s32 count;

    enabled = OSDisableInterrupts();
    sem->count = (count = sem->count) + 1;
    OSWakeupThread(&sem->queue);
    OSRestoreInterrupts(enabled);
    return count;
}
