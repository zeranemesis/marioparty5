/*
 * OS threads and semaphores for the native port, on top of libco coroutines.
 *
 * The GameCube scheduler is preemptive by priority (0 = highest). Everything here
 * runs on one host thread, so preemption happens only at the points where the
 * GameCube kernel would also reschedule: a thread becoming ready (create/resume,
 * semaphore signal, wakeup) or the running thread blocking (sleep, semaphore wait,
 * suspend, exit). A thread of higher priority than the running one takes over at
 * once, as on hardware.
 *
 * The main game thread is not a coroutine of ours: it is whatever coroutine is
 * active when it switches to a thread (the HuPrc process system runs on libco
 * too), and control returns to exactly that coroutine. On hardware the main
 * thread sleeps in VIWaitForRetrace, which is when lower-priority threads run;
 * PortThreadFrame() reproduces that: called from VIWaitForRetrace, it lets every
 * ready thread run until all of them block again.
 *
 * shortcut: a thread that loops forever without blocking hangs the game (on
 * hardware the next vblank interrupt would preempt it); none of the game's threads
 * are expected to do that.
 */
#include <dolphin/os.h>
#include <stdio.h>
#include <string.h>

#include "../../extern/libco/libco.h"

#define PORT_THREAD_MAX 64
#define MAIN_PRIORITY 16

typedef struct PortThread {
    OSThread *thread;
    cothread_t co;
    void *(*func)(void *);
    void *param;
    u32 readySeq; /* FIFO order among threads of equal priority */
    BOOL started; /* the coroutine has been entered once */
} PortThread;

static PortThread sThreads[PORT_THREAD_MAX];
static OSThread sMainThread;
static OSThread *sCurrent;
static cothread_t sMainCo;       /* coroutine the main thread left from */
static cothread_t sDeadCo;       /* coroutine of an exited thread, freed after we left it */
static PortThread *sStarting;    /* thread whose coroutine is starting */
static BOOL sMainRetrace;        /* main thread is "sleeping" in VIWaitForRetrace */
static OSThreadQueue sRetraceQueue; /* threads waiting for the next frame */
static u32 sReadySeq;

static void Init(void)
{
    if (sCurrent != NULL) {
        return;
    }
    memset(&sMainThread, 0, sizeof(sMainThread));
    sMainThread.state = OS_THREAD_STATE_RUNNING;
    sMainThread.priority = sMainThread.base = MAIN_PRIORITY;
    sCurrent = &sMainThread;
}

static PortThread *Find(OSThread *thread)
{
    int i;
    for (i = 0; i < PORT_THREAD_MAX; i++) {
        if (sThreads[i].thread == thread) {
            return &sThreads[i];
        }
    }
    return NULL;
}

static void QueueRemove(OSThread *thread)
{
    OSThreadQueue *queue = thread->queue;
    if (queue == NULL) {
        return;
    }
    if (thread->link.prev != NULL) {
        thread->link.prev->link.next = thread->link.next;
    } else {
        queue->head = thread->link.next;
    }
    if (thread->link.next != NULL) {
        thread->link.next->link.prev = thread->link.prev;
    } else {
        queue->tail = thread->link.prev;
    }
    thread->link.next = thread->link.prev = NULL;
    thread->queue = NULL;
}

static void QueueAppend(OSThreadQueue *queue, OSThread *thread)
{
    thread->queue = queue;
    thread->link.next = NULL;
    thread->link.prev = queue->tail;
    if (queue->tail != NULL) {
        queue->tail->link.next = thread;
    } else {
        queue->head = thread;
    }
    queue->tail = thread;
}

static void MakeReady(OSThread *thread)
{
    PortThread *pt = Find(thread);
    thread->state = OS_THREAD_STATE_READY;
    if (pt != NULL) {
        pt->readySeq = ++sReadySeq;
    }
}

static BOOL Runnable(OSThread *thread)
{
    return thread->state == OS_THREAD_STATE_READY && thread->suspend <= 0;
}

/* best ready thread, main included; NULL if none */
static OSThread *PickReady(void)
{
    OSThread *best = NULL;
    u32 bestSeq = 0;
    int i;
    if (Runnable(&sMainThread)) {
        best = &sMainThread;
        bestSeq = 0;
    }
    for (i = 0; i < PORT_THREAD_MAX; i++) {
        OSThread *t = sThreads[i].thread;
        if (t == NULL || !Runnable(t)) {
            continue;
        }
        if (best == NULL || t->priority < best->priority
            || (t->priority == best->priority && best != &sMainThread && sThreads[i].readySeq < bestSeq)) {
            best = t;
            bestSeq = sThreads[i].readySeq;
        }
    }
    return best;
}

static void SwitchTo(OSThread *next)
{
    OSThread *prev = sCurrent;
    cothread_t target;
    if (next == prev) {
        next->state = OS_THREAD_STATE_RUNNING;
        return;
    }
    if (prev == &sMainThread) {
        sMainCo = co_active();
    }
    if (prev->state == OS_THREAD_STATE_RUNNING) {
        MakeReady(prev);
    }
    sCurrent = next;
    next->state = OS_THREAD_STATE_RUNNING;
    if (next == &sMainThread) {
        target = sMainCo;
    } else {
        PortThread *pt = Find(next);
        target = pt->co;
        if (!pt->started) {
            pt->started = TRUE;
            sStarting = pt; /* read by Trampoline on the first entry */
        }
    }
    co_switch(target);
    /* back on prev's coroutine: free a coroutine that exited while we were away */
    if (sDeadCo != NULL && sDeadCo != co_active()) {
        co_delete(sDeadCo);
        sDeadCo = NULL;
    }
}

/* Run the best ready thread if it should preempt the current one, or if the current one blocked. */
static void Reschedule(void)
{
    OSThread *cur = sCurrent;
    OSThread *best = PickReady();
    BOOL curRuns = cur->state == OS_THREAD_STATE_RUNNING && cur->suspend <= 0;
    if (curRuns) {
        if (best == NULL || best->priority >= cur->priority) {
            return;
        }
        SwitchTo(best);
        return;
    }
    if (best == NULL) {
        /* nothing can run: the main thread was only waiting for the frame end (or a real deadlock) */
        if (!sMainRetrace) {
            puts("PortThread: no runnable thread, resuming the main thread");
        }
        sMainRetrace = FALSE;
        if (sMainThread.queue != NULL) {
            QueueRemove(&sMainThread);
        }
        MakeReady(&sMainThread);
        best = &sMainThread;
    }
    SwitchTo(best);
}

static void Trampoline(void)
{
    PortThread *pt = sStarting;
    void *ret;
    sStarting = NULL;
    ret = pt->func(pt->param);
    OSExitThread(ret);
}

void OSInitThreadQueue(OSThreadQueue *queue)
{
    queue->head = queue->tail = NULL;
}

OSThread *OSGetCurrentThread(void)
{
    Init();
    return sCurrent;
}

BOOL OSCreateThread(OSThread *thread, void *(*func)(void *), void *param, void *stack, u32 stackSize,
    OSPriority priority, u16 attr)
{
    PortThread *pt;
    u32 coSize;
    Init();
    if (priority < OS_PRIORITY_MIN || priority > OS_PRIORITY_MAX) {
        return FALSE;
    }
    pt = Find(thread);
    if (pt == NULL) {
        pt = Find(NULL);
    }
    if (pt == NULL) {
        puts("PortThread: too many threads");
        return FALSE;
    }
    memset(thread, 0, sizeof(*thread));
    thread->state = OS_THREAD_STATE_READY;
    thread->attr = attr;
    thread->suspend = 1;
    thread->priority = thread->base = priority;
    thread->val = (void *)-1;
    thread->stackBase = (u8 *)stack;
    thread->stackEnd = (u8 *)stack - stackSize;
    if (stack != NULL) {
        *(u32 *)thread->stackEnd = OS_THREAD_STACK_MAGIC;
    }
    /* host frames are much larger than PowerPC ones, and the game's stack is not used */
    coSize = stackSize * 8 < 0x40000 ? 0x40000 : stackSize * 8;
    if (pt->co != NULL) {
        co_delete(pt->co);
    }
    pt->thread = thread;
    pt->co = co_create(coSize, Trampoline);
    pt->func = func;
    pt->param = param;
    pt->readySeq = ++sReadySeq;
    pt->started = FALSE;
    return pt->co != NULL;
}

s32 OSResumeThread(OSThread *thread)
{
    s32 prev;
    Init();
    prev = thread->suspend--;
    if (thread->suspend < 0) {
        thread->suspend = 0;
    } else if (thread->suspend == 0 && thread->state == OS_THREAD_STATE_READY) {
        MakeReady(thread);
        Reschedule();
    }
    return prev;
}

s32 OSSuspendThread(OSThread *thread)
{
    s32 prev;
    Init();
    prev = thread->suspend++;
    if (prev == 0 && thread == sCurrent) {
        thread->state = OS_THREAD_STATE_READY;
        Reschedule();
    }
    return prev;
}

void OSSleepThread(OSThreadQueue *queue)
{
    OSThread *cur;
    Init();
    cur = sCurrent;
    cur->state = OS_THREAD_STATE_WAITING;
    QueueAppend(queue, cur);
    Reschedule();
}

void OSWakeupThread(OSThreadQueue *queue)
{
    OSThread *t;
    Init();
    while ((t = queue->head) != NULL) {
        QueueRemove(t);
        MakeReady(t);
    }
    Reschedule();
}

void OSYieldThread(void)
{
    OSThread *cur;
    OSThread *best;
    Init();
    cur = sCurrent;
    MakeReady(cur);
    best = PickReady();
    if (best != NULL && best != cur && best->priority <= cur->priority) {
        SwitchTo(best);
    } else {
        cur->state = OS_THREAD_STATE_RUNNING;
    }
}

void OSExitThread(void *val)
{
    OSThread *cur;
    PortThread *pt;
    Init();
    cur = sCurrent;
    pt = Find(cur);
    if (pt == NULL) {
        puts("PortThread: OSExitThread on the main thread");
        return;
    }
    cur->val = val;
    if (cur->attr & OS_THREAD_ATTR_DETACH) {
        cur->state = 0;
    } else {
        cur->state = OS_THREAD_STATE_MORIBUND;
    }
    /* the coroutine is freed once we are off it */
    sDeadCo = pt->co;
    pt->co = NULL;
    pt->thread = cur->state == 0 ? NULL : cur;
    {
        OSThread *t;
        while ((t = cur->queueJoin.head) != NULL) {
            QueueRemove(t);
            MakeReady(t);
        }
    }
    Reschedule();
    puts("PortThread: exited thread resumed");
}

void OSCancelThread(OSThread *thread)
{
    PortThread *pt;
    Init();
    if (thread == sCurrent) {
        OSExitThread(NULL);
        return;
    }
    pt = Find(thread);
    QueueRemove(thread);
    if (thread->attr & OS_THREAD_ATTR_DETACH) {
        thread->state = 0;
    } else {
        thread->state = OS_THREAD_STATE_MORIBUND;
    }
    if (pt != NULL) {
        if (pt->co != NULL) {
            co_delete(pt->co);
        }
        pt->co = NULL;
        if (thread->state == 0) {
            pt->thread = NULL;
        }
    }
    {
        OSThread *t;
        while ((t = thread->queueJoin.head) != NULL) {
            QueueRemove(t);
            MakeReady(t);
        }
    }
    Reschedule();
}

BOOL OSJoinThread(OSThread *thread, void **val)
{
    PortThread *pt;
    Init();
    if (!(thread->attr & OS_THREAD_ATTR_DETACH) && thread->state != OS_THREAD_STATE_MORIBUND) {
        OSSleepThread(&thread->queueJoin);
    }
    if (thread->state != OS_THREAD_STATE_MORIBUND) {
        return FALSE;
    }
    if (val != NULL) {
        *val = thread->val;
    }
    thread->state = 0;
    pt = Find(thread);
    if (pt != NULL) {
        pt->thread = NULL;
    }
    return TRUE;
}

void OSDetachThread(OSThread *thread)
{
    PortThread *pt;
    thread->attr |= OS_THREAD_ATTR_DETACH;
    if (thread->state == OS_THREAD_STATE_MORIBUND) {
        thread->state = 0;
        pt = Find(thread);
        if (pt != NULL) {
            pt->thread = NULL;
        }
    }
}

BOOL OSIsThreadTerminated(OSThread *thread)
{
    return thread->state == OS_THREAD_STATE_MORIBUND || thread->state == 0;
}

BOOL OSIsThreadSuspended(OSThread *thread)
{
    return thread->suspend > 0;
}

OSPriority OSGetThreadPriority(OSThread *thread)
{
    return thread->base;
}

BOOL OSSetThreadPriority(OSThread *thread, OSPriority priority)
{
    if (priority < OS_PRIORITY_MIN || priority > OS_PRIORITY_MAX) {
        return FALSE;
    }
    thread->priority = thread->base = priority;
    Reschedule();
    return TRUE;
}

s32 OSCheckActiveThreads(void)
{
    s32 n = 1;
    int i;
    for (i = 0; i < PORT_THREAD_MAX; i++) {
        OSThread *t = sThreads[i].thread;
        if (t != NULL && t->state != 0 && t->state != OS_THREAD_STATE_MORIBUND) {
            n++;
        }
    }
    return n;
}

void OSInitSemaphore(OSSemaphore *sem, s32 count)
{
    OSInitThreadQueue(&sem->queue);
    sem->count = count;
}

s32 OSWaitSemaphore(OSSemaphore *sem)
{
    s32 count;
    Init();
    while ((count = sem->count) <= 0) {
        OSSleepThread(&sem->queue);
    }
    sem->count--;
    return count;
}

s32 OSTryWaitSemaphore(OSSemaphore *sem)
{
    s32 count = sem->count;
    if (count > 0) {
        sem->count--;
    }
    return count;
}

s32 OSSignalSemaphore(OSSemaphore *sem)
{
    s32 count = sem->count++;
    OSWakeupThread(&sem->queue);
    return count;
}

s32 OSGetSemaphoreCount(OSSemaphore *sem)
{
    return sem->count;
}

/* VIWaitForRetrace: a thread waits for the next frame; the main thread lets every ready thread run first. */
void PortThreadRetrace(void)
{
    Init();
    if (sCurrent != &sMainThread) {
        OSSleepThread(&sRetraceQueue);
        return;
    }
    {
        OSThread *t;
        while ((t = sRetraceQueue.head) != NULL) {
            QueueRemove(t);
            MakeReady(t);
        }
    }
    if (PickReady() == &sMainThread || PickReady() == NULL) {
        return;
    }
    sMainRetrace = TRUE;
    sMainThread.state = OS_THREAD_STATE_WAITING;
    Reschedule();
    sMainRetrace = FALSE;
}
