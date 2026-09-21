/*
 * SDK functions Aurora declares but does not implement.
 *
 * Each one is either forwarded to the call Aurora does provide, or answers
 * what the console would answer on a machine that has no such hardware: no
 * progressive scan, no digital TV, no interrupts to enable, no threads to
 * schedule. The disc error screen's thread is the only caller of most of the
 * thread ones, and it never runs here: the disc is a file.
 */

#include "dolphin.h"

#include "dolphin/ai.h"
#include "dolphin/ar.h"
#include "dolphin/dvd.h"
#include "dolphin/gx.h"
#include "dolphin/os.h"
#include "dolphin/vi.h"

/* Only read when progressive scan is on, which OSGetProgressiveMode denies. */
GXRenderModeObj GXNtsc480Prog;

/*
 * Aurora's ARAM emulation (extern/aurora/lib/dolphin/AR.cpp) starts handing
 * out offsets at ARAM_STACK_START (0x4000), the same value real hardware
 * reserves for the system before the first usable byte. MusyX and msmSysInit
 * both compare an allocation against this "base address" to make sure
 * nothing else claimed ARAM first, so it has to agree with what ARInit
 * effectively starts from.
 */
u32 ARGetBaseAddress(void)
{
    return 0x4000;
}

void AIInit(u8 *stack)
{
    (void)stack;
}

AIDCallback AIRegisterDMACallback(AIDCallback callback)
{
    (void)callback;
    return NULL;
}

s32 DVDRead(DVDFileInfo *fileInfo, void *addr, s32 length, s32 offset)
{
    return DVDReadPrio(fileInfo, addr, length, offset, 2);
}

void GXFastCallDisplayList(const void *list, u32 nbytes)
{
    GXCallDisplayList(list, nbytes);
}

void GXAbortFrame(void)
{
}

u32 OSGetProgressiveMode(void)
{
    return 0;
}

u32 VIGetDTVStatus(void)
{
    return 0;
}

VIRetraceCallback VISetPreRetraceCallback(VIRetraceCallback cb)
{
    (void)cb;
    return NULL;
}

BOOL OSEnableInterrupts(void)
{
    return FALSE;
}

void OSInitMessageQueue(OSMessageQueue *mq, OSMessage *msgArray, s32 msgCount)
{
    (void)mq;
    (void)msgArray;
    (void)msgCount;
}

BOOL OSCreateThread(OSThread *thread, void *(*func)(void *), void *param, void *stack, u32 stackSize,
    OSPriority priority, u16 attr)
{
    (void)thread;
    (void)func;
    (void)param;
    (void)stack;
    (void)stackSize;
    (void)priority;
    (void)attr;
    return FALSE;
}

void OSSleepThread(OSThreadQueue *queue)
{
    (void)queue;
}

void OSYieldThread(void)
{
}

void AISetStreamPlayState(u32 state)
{
    (void)state;
}

void AISetStreamVolLeft(u8 vol)
{
    (void)vol;
}

void AISetStreamVolRight(u8 vol)
{
    (void)vol;
}
