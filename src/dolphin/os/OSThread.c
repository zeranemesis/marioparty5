#include "dolphin/os.h"

#include "dolphin/__start.h"
#include "dolphin/os/OSMutex.h"

#define OS_THREAD_IDLE_PRIORITY 16
#define OS_ACTIVE_THREAD_QUEUE (*(OSThreadQueue*)0x800000DC)
#define OS_LAST_FPU_CONTEXT (*(OSContext**)0x800000D8)

extern void __OSUnlockAllMutex(OSThread* thread);
extern BOOL __OSCheckDeadLock(OSThread* thread);
extern BOOL __OSCheckMutexes(OSThread* thread);
extern OSErrorHandler __OSErrorTable[];
extern u32 __OSFpscrEnableBits;

extern char _stack_addr[];
extern char _stack_end[];

void DefaultSwitchThreadCallback(OSThread* from, OSThread* to);
void __OSThreadInit(void);
BOOL OSIsThreadTerminated(OSThread* thread);
s32 OSDisableScheduler(void);
s32 OSEnableScheduler(void);
OSPriority __OSGetEffectivePriority(OSThread* thread);
void __OSReschedule(void);
BOOL OSCreateThread(OSThread* thread, void* (*func)(void*), void* param, void* stack, u32 stackSize,
                    OSPriority priority, u16 attr);
void OSInitContext(OSContext* context, u32 pc, u32 stack);
void OSClearContext(OSContext* context);
void OSSetCurrentContext(OSContext* context);
OSContext* OSGetCurrentContext(void);
u32 OSSaveContext(OSContext* context);
void OSLoadContext(OSContext* context);
void OSWakeupThread(OSThreadQueue* queue);
void OSClearStack(u8 val);

static OSThreadQueue RunQueue[32];
static OSThread IdleThread;
static OSThread DefaultThread;
static OSContext IdleContext;

static OSSwitchThreadCallback SwitchThreadCallback = DefaultSwitchThreadCallback;

static s32 RunQueueBits;
static s32 RunQueueHint;
static s32 Reschedule;

static void UnsetRun(OSThread* thread);
static OSThread* SetEffectivePriority(OSThread* thread, OSPriority priority);
static void SelectThread(OSPriority hint);
static BOOL CheckThreadQueue(OSThreadQueue* queue);

void DefaultSwitchThreadCallback(OSThread* from, OSThread* to)
{
}

void __OSThreadInit(void)
{
    s32 i;

    DefaultThread.state = OS_THREAD_STATE_RUNNING;
    DefaultThread.attr = OS_THREAD_ATTR_DETACH;
    DefaultThread.priority = OS_THREAD_IDLE_PRIORITY;
    DefaultThread.base = OS_THREAD_IDLE_PRIORITY;
    DefaultThread.val = (void*)-1;
    DefaultThread.suspend = 0;
    DefaultThread.mutex = NULL;

    OSInitThreadQueue(&DefaultThread.queueJoin);
    DefaultThread.queueMutex.head = NULL;
    DefaultThread.queueMutex.tail = NULL;

    *(OSContext**)0x800000D8 = &DefaultThread.context;
    OSClearContext(&DefaultThread.context);
    OSSetCurrentContext(&DefaultThread.context);

    DefaultThread.stackBase = (u8*)_stack_addr;
    DefaultThread.stackEnd = (u32*)_stack_end;
    *DefaultThread.stackEnd = OS_THREAD_STACK_MAGIC;

    SwitchThreadCallback(OSGetCurrentThread(), &DefaultThread);
    *(OSThread**)0x800000E4 = &DefaultThread;
    OSClearStack(0);

    RunQueueBits = 0;
    RunQueueHint = 0;

    for (i = 0; i <= OS_PRIORITY_MAX; i++) {
        OSInitThreadQueue(&RunQueue[i]);
    }

    OSInitThreadQueue(&OS_ACTIVE_THREAD_QUEUE);
    if (OS_ACTIVE_THREAD_QUEUE.head == NULL) {
        OS_ACTIVE_THREAD_QUEUE.head = &DefaultThread;
    } else {
        OS_ACTIVE_THREAD_QUEUE.head->linkActive.prev = &DefaultThread;
    }
    DefaultThread.linkActive.next = OS_ACTIVE_THREAD_QUEUE.head;
    DefaultThread.linkActive.prev = NULL;
    OS_ACTIVE_THREAD_QUEUE.tail = &DefaultThread;

    OSClearContext(&IdleContext);
    Reschedule = 0;
}

void OSInitThreadQueue(OSThreadQueue* queue)
{
    queue->tail = 0;
    queue->head = 0;
}

OSThread* OSGetCurrentThread(void)
{
    return *(OSThread**)0x800000E4;
}

BOOL OSIsThreadTerminated(OSThread* thread)
{
    return thread->state == OS_THREAD_STATE_MORIBUND || thread->state == 0;
}

s32 OSDisableScheduler(void)
{
    s32 level;
    s32 old;

    level = OSDisableInterrupts();
    old = Reschedule;
    Reschedule = old + 1;
    OSRestoreInterrupts(level);

    return old;
}

s32 OSEnableScheduler(void)
{
    s32 level;
    s32 old;

    level = OSDisableInterrupts();
    old = Reschedule;
    Reschedule = old - 1;
    OSRestoreInterrupts(level);

    return old;
}

static void UnsetRun(OSThread* thread)
{
    OSThread* next = thread->link.next;
    OSThread* prev = thread->link.prev;
    OSThreadQueue* queue = thread->queue;

    if (next == NULL) {
        queue->tail = prev;
    } else {
        next->link.prev = prev;
    }
    if (prev == NULL) {
        queue->head = next;
    } else {
        prev->link.next = next;
    }
    if (queue->head == NULL) {
        RunQueueBits &= ~(1 << (OS_PRIORITY_MAX - thread->priority));
    }
    thread->queue = NULL;
}

OSPriority __OSGetEffectivePriority(OSThread* thread)
{
    OSPriority priority;
    OSMutex* mutex;

    priority = thread->base;
    for (mutex = thread->queueMutex.head; mutex; mutex = mutex->link.next) {
        if (mutex->thread && mutex->thread->priority < priority) {
            priority = mutex->thread->priority;
        }
    }
    return priority;
}

static OSThread* SetEffectivePriority(OSThread* thread, OSPriority priority)
{
    OSThread* last;
    OSThread* next;

    switch (thread->state) {
    case OS_THREAD_STATE_READY:
        UnsetRun(thread);
        thread->priority = priority;
        thread->queue = &RunQueue[priority];
        if (thread->queue->tail == NULL) {
            thread->queue->head = thread;
        } else {
            thread->queue->tail->link.next = thread;
        }
        thread->link.prev = thread->queue->tail;
        thread->link.next = NULL;
        thread->queue->tail = thread;
        RunQueueBits |= 1 << (OS_PRIORITY_MAX - priority);
        RunQueueHint = 1;
        break;
    case OS_THREAD_STATE_RUNNING:
        RunQueueHint = 1;
        thread->priority = priority;
        break;
    case OS_THREAD_STATE_WAITING:
        next = thread->link.next;
        last = thread->link.prev;
        if (last == NULL) {
            thread->queue->tail = next;
        } else {
            last->link.next = next;
        }
        if (next == NULL) {
            thread->queue->head = last;
        } else {
            next->link.prev = last;
        }
        thread->priority = priority;

        for (next = thread->queue->head; next != NULL; next = next->link.next) {
            if (next->priority > thread->priority) {
                break;
            }
        }
        if (next == NULL) {
            if (thread->queue->tail == NULL) {
                thread->queue->head = thread;
            } else {
                thread->queue->tail->link.next = thread;
            }
            thread->link.prev = thread->queue->tail;
            thread->link.next = NULL;
            thread->queue->tail = thread;
        } else {
            thread->link.next = next;
            last = next->link.prev;
            next->link.prev = thread;
            thread->link.prev = last;
            if (last == NULL) {
                thread->queue->head = thread;
            } else {
                last->link.next = thread;
            }
        }

        if (thread->mutex != NULL) {
            OSThread* owner;

            for (owner = thread->mutex->thread; owner->suspend <= 0;) {
                if (owner->priority == __OSGetEffectivePriority(owner)) {
                    break;
                }
                owner = SetEffectivePriority(owner, __OSGetEffectivePriority(owner));
            }
            return owner;
        }
        break;
    }
    return 0;
}

static void SelectThread(OSPriority hint)
{
    register u32 bits;
    register s32 index;
    OSThread* next;

    if (Reschedule <= 0) {
        OSThread* current = OSGetCurrentThread();

        if (OSGetCurrentContext() != (OSContext*)current) {
            return;
        }
        if (current != NULL) {
            if (current->state == OS_THREAD_STATE_WAITING) {
                bits = (u32)RunQueueBits;
                asm {
                    cntlzw index, bits
                }
                if (hint == 0 && current->priority > index) {
                    return;
                }
                current->state = OS_THREAD_STATE_READY;
                current->queue = &RunQueue[current->priority];
                if (current->queue->tail == NULL) {
                    current->queue->head = current;
                } else {
                    current->queue->tail->link.next = current;
                }
                current->link.prev = current->queue->tail;
                current->link.next = NULL;
                current->queue->tail = current;
                RunQueueBits |= 1 << (OS_PRIORITY_MAX - current->priority);
                RunQueueHint = 1;
            }
            if (current->context.state & OS_CONTEXT_STATE_FPSAVED) {
                if (!OSSaveContext(&current->context)) {
                    return;
                }
            }
        }

        while (RunQueueBits == 0) {
            SwitchThreadCallback(NULL, OSGetCurrentThread());
            *(OSThread**)0x800000E4 = NULL;
            OSSetCurrentContext(&IdleContext);
            while (RunQueueBits == 0) {
                OSEnableInterrupts();
                while (RunQueueBits == 0) {
                }
                OSDisableInterrupts();
                while (RunQueueBits == 0) {
                    OSEnableInterrupts();
                }
            }
            OSClearContext(&IdleContext);
        }
    }

    RunQueueHint = 0;
    bits = (u32)RunQueueBits;
    asm {
        cntlzw index, bits
    }
    next = RunQueue[index].head;

    if (next->link.prev != NULL) {
        next->link.prev->link.next = NULL;
    } else {
        RunQueue[index].tail = NULL;
    }
    RunQueue[index].head = next->link.next;
    if (RunQueue[index].head == NULL) {
        RunQueueBits &= ~(1 << (index ^ OS_PRIORITY_MAX));
    }

    next->queue = NULL;
    next->state = OS_THREAD_STATE_RUNNING;
    SwitchThreadCallback(OSGetCurrentThread(), next);
    *(OSThread**)0x800000E4 = next;
    OSSetCurrentContext(&next->context);
    OSLoadContext(&next->context);
}

void __OSReschedule(void)
{
    if (RunQueueHint != 0) {
        SelectThread(0);
    }
}

void OSYieldThread(void)
{
    BOOL flag;

    flag = OSDisableInterrupts();
    SelectThread(1);
    OSRestoreInterrupts(flag);
}

BOOL OSCreateThread(OSThread* thread, void* (*func)(void*), void* param, void* stack, u32 stackSize,
                    OSPriority priority, u16 attr)
{
    BOOL flag;

    if (priority < OS_PRIORITY_MIN || priority > OS_PRIORITY_MAX) {
        return FALSE;
    }

    thread->state = OS_THREAD_STATE_READY;
    thread->attr = attr & OS_THREAD_ATTR_DETACH;
    stack = (void*)((u32)stack & ~7);
    thread->base = priority;
    thread->priority = priority;
    thread->suspend = 1;
    thread->val = (void*)-1;
    thread->mutex = NULL;
    thread->queueJoin.tail = NULL;
    thread->queueJoin.head = NULL;
    thread->queueMutex.tail = NULL;
    thread->queueMutex.head = NULL;
    ((u32*)stack)[-2] = 0;
    ((u32*)stack)[-1] = 0;

    OSInitContext(&thread->context, (u32)func, (u32)stack - 8);
    thread->context.lr = (u32)OSExitThread;
    thread->context.gpr[3] = (u32)param;
    thread->stackBase = (u8*)stack;
    thread->stackEnd = (u32*)(stackSize - (u32)stack);
    *thread->stackEnd = OS_THREAD_STACK_MAGIC;
    thread->error = 0;
    thread->specific[0] = NULL;
    thread->specific[1] = NULL;

    flag = OSDisableInterrupts();

    if (__OSErrorTable[OS_ERROR_FPE] != NULL) {
        thread->context.srr1 |= 0x900;
        thread->context.state |= OS_CONTEXT_STATE_FPSAVED;
        thread->context.fpscr = (__OSFpscrEnableBits & 0x1F000000) | 4;

        memset(&thread->context.fpr, 0xFF, sizeof(thread->context.fpr));
        memset(&thread->context.psf, 0xFF, sizeof(thread->context.psf));
    }

    if (OS_ACTIVE_THREAD_QUEUE.head == NULL) {
        OS_ACTIVE_THREAD_QUEUE.head = thread;
    } else {
        OS_ACTIVE_THREAD_QUEUE.head->linkActive.prev = thread;
    }
    thread->linkActive.next = OS_ACTIVE_THREAD_QUEUE.head;
    thread->linkActive.prev = NULL;
    OS_ACTIVE_THREAD_QUEUE.tail = thread;

    OSRestoreInterrupts(flag);

    return TRUE;
}

void OSExitThread(void* val)
{
    OSThread* thread;
    OSThread* next;
    OSThread* prev;
    BOOL flag;

    flag = OSDisableInterrupts();

    thread = OSGetCurrentThread();
    OSClearContext(&thread->context);

    if (!(thread->attr & OS_THREAD_ATTR_DETACH)) {
        prev = thread->linkActive.prev;
        next = thread->linkActive.next;
        if (prev == NULL) {
            OS_ACTIVE_THREAD_QUEUE.tail = next;
        } else {
            prev->linkActive.next = next;
        }
        if (next == NULL) {
            OS_ACTIVE_THREAD_QUEUE.head = prev;
        } else {
            next->linkActive.prev = prev;
        }
        thread->state = 0;
    } else {
        thread->state = OS_THREAD_STATE_MORIBUND;
        thread->val = val;
    }

    __OSUnlockAllMutex(thread);
    OSWakeupThread(&thread->queueJoin);

    RunQueueHint = 1;
    __OSReschedule();

    OSRestoreInterrupts(flag);
}

void OSCancelThread(OSThread* thread)
{
    OSThread* owner;
    OSThread* next;
    OSThread* prev;
    BOOL flag;

    flag = OSDisableInterrupts();

    switch (thread->state) {
    case OS_THREAD_STATE_READY:
        if (thread->suspend <= 0) {
            break;
        }
        UnsetRun(thread);
        break;
    case OS_THREAD_STATE_RUNNING:
        RunQueueHint = 1;
        break;
    case OS_THREAD_STATE_WAITING:
        prev = thread->link.prev;
        next = thread->link.next;
        if (prev == NULL) {
            thread->queue->tail = next;
        } else {
            prev->link.next = next;
        }
        if (next == NULL) {
            thread->queue->head = prev;
        } else {
            next->link.prev = prev;
        }
        thread->queue = NULL;
        if (thread->suspend > 0) {
            break;
        }
        if (thread->mutex != NULL) {
            for (owner = thread->mutex->thread; owner->suspend <= 0;) {
                if (owner->priority == __OSGetEffectivePriority(owner)) {
                    break;
                }
                owner = SetEffectivePriority(owner, __OSGetEffectivePriority(owner));
            }
        }
        break;
    default:
        OSRestoreInterrupts(flag);
        return;
    }

    OSClearContext(&thread->context);

    if (!(thread->attr & OS_THREAD_ATTR_DETACH)) {
        prev = thread->linkActive.prev;
        next = thread->linkActive.next;
        if (prev == NULL) {
            OS_ACTIVE_THREAD_QUEUE.tail = next;
        } else {
            prev->linkActive.next = next;
        }
        if (next == NULL) {
            OS_ACTIVE_THREAD_QUEUE.head = prev;
        } else {
            next->linkActive.prev = prev;
        }
        thread->state = 0;
    } else {
        thread->state = OS_THREAD_STATE_MORIBUND;
    }

    __OSUnlockAllMutex(thread);
    OSWakeupThread(&thread->queueJoin);

    __OSReschedule();

    OSRestoreInterrupts(flag);
}

s32 OSResumeThread(OSThread* thread)
{
    OSThread* owner;
    OSThread* next;
    OSThread* prev;
    s32 count;
    BOOL flag;

    flag = OSDisableInterrupts();

    count = thread->suspend;
    thread->suspend = count - 1;
    if (thread->suspend < 0) {
        thread->suspend = 0;
    } else if (thread->suspend == 0) {
        switch (thread->state) {
        case OS_THREAD_STATE_READY:
            thread->priority = __OSGetEffectivePriority(thread);
            thread->queue = &RunQueue[thread->priority];
            if (thread->queue->tail == NULL) {
                thread->queue->head = thread;
            } else {
                thread->queue->tail->link.next = thread;
            }
            thread->link.prev = thread->queue->tail;
            thread->link.next = NULL;
            thread->queue->tail = thread;
            RunQueueBits |= 1 << (OS_PRIORITY_MAX - thread->priority);
            RunQueueHint = 1;
            break;
        case OS_THREAD_STATE_WAITING:
            prev = thread->link.prev;
            next = thread->link.next;
            if (prev == NULL) {
                thread->queue->tail = next;
            } else {
                prev->link.next = next;
            }
            if (next == NULL) {
                thread->queue->head = prev;
            } else {
                next->link.prev = prev;
            }
            thread->priority = __OSGetEffectivePriority(thread);

            for (next = thread->queue->head; next != NULL; next = next->link.next) {
                if (next->priority > thread->priority) {
                    break;
                }
            }
            if (next == NULL) {
                if (thread->queue->tail == NULL) {
                    thread->queue->head = thread;
                } else {
                    thread->queue->tail->link.next = thread;
                }
                thread->link.prev = thread->queue->tail;
                thread->link.next = NULL;
                thread->queue->tail = thread;
            } else {
                thread->link.next = next;
                prev = next->link.prev;
                next->link.prev = thread;
                thread->link.prev = prev;
                if (prev == NULL) {
                    thread->queue->head = thread;
                } else {
                    prev->link.next = thread;
                }
            }

            if (thread->mutex != NULL) {
                for (owner = thread->mutex->thread; owner->suspend <= 0;) {
                    if (owner->priority == __OSGetEffectivePriority(owner)) {
                        break;
                    }
                    owner = SetEffectivePriority(owner, __OSGetEffectivePriority(owner));
                }
            }
            break;
        }
    }

    __OSReschedule();

    OSRestoreInterrupts(flag);

    return count;
}

s32 OSSuspendThread(OSThread* thread)
{
    OSThread* owner;
    OSThread* next;
    OSThread* prev;
    s32 count;
    BOOL flag;

    flag = OSDisableInterrupts();

    count = thread->suspend;
    thread->suspend = count + 1;

    if (count == 0) {
        switch (thread->state) {
        case OS_THREAD_STATE_READY:
            UnsetRun(thread);
            break;
        case OS_THREAD_STATE_RUNNING:
            RunQueueHint = 1;
            thread->state = OS_THREAD_STATE_READY;
            break;
        case OS_THREAD_STATE_WAITING:
            prev = thread->link.prev;
            next = thread->link.next;
            if (prev == NULL) {
                thread->queue->tail = next;
            } else {
                prev->link.next = next;
            }
            if (next == NULL) {
                thread->queue->head = prev;
            } else {
                next->link.prev = prev;
            }
            thread->priority = OS_PRIORITY_MAX + 1;
            if (thread->queue->tail == NULL) {
                thread->queue->head = thread;
            } else {
                thread->queue->tail->link.next = thread;
            }
            thread->link.prev = thread->queue->tail;
            thread->link.next = NULL;
            thread->queue->tail = thread;

            if (thread->mutex != NULL) {
                for (owner = thread->mutex->thread; owner->suspend <= 0;) {
                    if (owner->priority == __OSGetEffectivePriority(owner)) {
                        break;
                    }
                    owner = SetEffectivePriority(owner, __OSGetEffectivePriority(owner));
                }
            }
            break;
        }
        __OSReschedule();
    }

    OSRestoreInterrupts(flag);

    return count;
}

void OSSleepThread(OSThreadQueue* queue)
{
    OSThread* thread;
    OSThread* next;
    OSThread* prev;
    BOOL flag;

    flag = OSDisableInterrupts();

    thread = OSGetCurrentThread();
    thread->state = OS_THREAD_STATE_WAITING;
    thread->queue = queue;

    for (next = queue->head; next != NULL; next = next->link.next) {
        if (next->priority > thread->priority) {
            break;
        }
    }
    if (next == NULL) {
        if (queue->tail == NULL) {
            queue->head = thread;
        } else {
            queue->tail->link.next = thread;
        }
        thread->link.prev = queue->tail;
        thread->link.next = NULL;
        queue->tail = thread;
    } else {
        thread->link.next = next;
        prev = next->link.prev;
        next->link.prev = thread;
        thread->link.prev = prev;
        if (prev == NULL) {
            queue->head = thread;
        } else {
            prev->link.next = thread;
        }
    }

    RunQueueHint = 1;
    __OSReschedule();

    OSRestoreInterrupts(flag);
}

void OSWakeupThread(OSThreadQueue* queue)
{
    OSThread* thread;
    OSThread* next;
    BOOL flag;

    flag = OSDisableInterrupts();

    while ((thread = queue->head) != NULL) {
        next = thread->link.next;
        if (next == NULL) {
            queue->tail = NULL;
        } else {
            next->link.prev = NULL;
        }
        queue->head = next;

        thread->state = OS_THREAD_STATE_READY;
        if (thread->suspend <= 0) {
            thread->queue = &RunQueue[thread->priority];
            if (thread->queue->tail == NULL) {
                thread->queue->head = thread;
            } else {
                thread->queue->tail->link.next = thread;
            }
            thread->link.prev = thread->queue->tail;
            thread->link.next = NULL;
            thread->queue->tail = thread;
            RunQueueBits |= 1 << (OS_PRIORITY_MAX - thread->priority);
            RunQueueHint = 1;
        }
    }

    __OSReschedule();

    OSRestoreInterrupts(flag);
}

OSPriority OSGetThreadPriority(OSThread* thread)
{
    return thread->base;
}

OSThread* OSSetIdleFunction(OSIdleFunction idleFunction, void* param, void* stack, u32 stackSize)
{
    if (idleFunction != NULL) {
        if (IdleThread.state == 0) {
            OSCreateThread(&IdleThread, (void* (*)(void*))idleFunction, param, stack, stackSize,
                           OS_PRIORITY_IDLE, OS_THREAD_ATTR_DETACH);
            OSResumeThread(&IdleThread);
            return &IdleThread;
        }
    } else if (IdleThread.state != 0) {
        OSCancelThread(&IdleThread);
    }
    return NULL;
}

static BOOL CheckThreadQueue(OSThreadQueue* queue)
{
    OSThread* thread;

    if (queue->head && queue->head->link.prev) {
        return FALSE;
    }
    if (queue->tail && queue->tail->link.next) {
        return FALSE;
    }

    for (thread = queue->head; thread; thread = thread->link.next) {
        if (thread->link.next && (OSThread*)thread->link.next->link.prev != thread) {
            return FALSE;
        }
        if (thread->link.prev && (OSThread*)thread->link.prev->link.next != thread) {
            return FALSE;
        }
    }
    return TRUE;
}

long OSCheckActiveThreads(void)
{
    OSThread* thread;
    OSThread* next;
    OSThreadQueue* queue;
    OSPriority priority;
    long result;
    BOOL flag;

    flag = OSDisableInterrupts();
    result = 0;

    for (priority = OS_PRIORITY_MIN; priority <= OS_PRIORITY_MAX; priority++) {
        queue = &RunQueue[priority];
        if (RunQueueBits & (1 << (OS_PRIORITY_MAX - priority))) {
            if (!(queue->head != NULL && queue->tail != NULL)) {
                OSReport("OSCheckActiveThreads: Failed RunQueue[prio].head != NULL && "
                         "RunQueue[prio].tail != NULL in %d\n",
                         __LINE__);
                OSPanic(__FILE__, __LINE__, "");
            }
        } else if (queue->head != NULL || queue->tail != NULL) {
            OSReport("OSCheckActiveThreads: Failed RunQueue[prio].head == NULL && "
                     "RunQueue[prio].tail == NULL in %d\n",
                     __LINE__);
            OSPanic(__FILE__, __LINE__, "");
        }
        if (!CheckThreadQueue(queue)) {
            OSReport("OSCheckActiveThreads: Failed CheckThreadQueue(&RunQueue[prio]) in %d\n",
                     __LINE__);
            OSPanic(__FILE__, __LINE__, "");
        }
    }

    if (OS_ACTIVE_THREAD_QUEUE.head && OS_ACTIVE_THREAD_QUEUE.head->linkActive.prev) {
        OSReport("OSCheckActiveThreads: Failed __OSActiveThreadQueue.head == NULL || "
                 "__OSActiveThreadQueue.head->linkActive.prev == NULL in %d\n",
                 __LINE__);
        OSPanic(__FILE__, __LINE__, "");
    }
    if (OS_ACTIVE_THREAD_QUEUE.tail && OS_ACTIVE_THREAD_QUEUE.tail->linkActive.next) {
        OSReport("OSCheckActiveThreads: Failed __OSActiveThreadQueue.tail == NULL || "
                 "__OSActiveThreadQueue.tail->linkActive.next == NULL in %d\n",
                 __LINE__);
        OSPanic(__FILE__, __LINE__, "");
    }

    for (thread = OS_ACTIVE_THREAD_QUEUE.head; thread; thread = thread->linkActive.next) {
        if (thread->linkActive.next && (OSThread*)thread->linkActive.next->linkActive.prev != thread) {
            OSReport("OSCheckActiveThreads: Failed thread->linkActive.next == NULL || "
                     "thread == thread->linkActive.next->linkActive.prev in %d\n",
                     __LINE__);
            OSPanic(__FILE__, __LINE__, "");
        }
        if (thread->linkActive.prev && (OSThread*)thread->linkActive.prev->linkActive.next != thread) {
            OSReport("OSCheckActiveThreads: Failed thread->linkActive.prev == NULL || "
                     "thread == thread->linkActive.prev->linkActive.next in %d\n",
                     __LINE__);
            OSPanic(__FILE__, __LINE__, "");
        }
        if (*thread->stackEnd != OS_THREAD_STACK_MAGIC) {
            OSReport("OSCheckActiveThreads: Failed *(thread->stackEnd) == OS_THREAD_STACK_MAGIC in %d\n",
                     __LINE__);
            OSPanic(__FILE__, __LINE__, "");
        }
        if (thread->priority < OS_PRIORITY_MIN || thread->priority > OS_PRIORITY_MAX + 1) {
            OSReport("OSCheckActiveThreads: Failed OS_PRIORITY_MIN <= thread->priority && "
                     "thread->priority <= OS_PRIORITY_MAX+1 in %d\n",
                     __LINE__);
            OSPanic(__FILE__, __LINE__, "");
        }
        if (thread->suspend < 0) {
            OSReport("OSCheckActiveThreads: Failed 0 <= thread->suspend in %d\n", __LINE__);
            OSPanic(__FILE__, __LINE__, "");
        }
        if (!CheckThreadQueue(&thread->queueJoin)) {
            OSReport("OSCheckActiveThreads: Failed CheckThreadQueue(&thread->queueJoin) in %d\n",
                     __LINE__);
            OSPanic(__FILE__, __LINE__, "");
        }

        switch (thread->state) {
        case OS_THREAD_STATE_READY:
            if (thread->suspend <= 0) {
                if (thread->queue != &RunQueue[thread->priority]) {
                    OSReport("OSCheckActiveThreads: Failed thread->queue == "
                             "&RunQueue[thread->priority] in %d\n",
                             __LINE__);
                    OSPanic(__FILE__, __LINE__, "");
                }
                for (next = RunQueue[thread->priority].head; next != NULL; next = next->link.next) {
                    if (next == thread) {
                        break;
                    }
                }
                if (next == NULL) {
                    OSReport("OSCheckActiveThreads: Failed IsMember(&RunQueue[thread->priority], thread) "
                             "in %d\n",
                             __LINE__);
                    OSPanic(__FILE__, __LINE__, "");
                }
                if (thread->priority != __OSGetEffectivePriority(thread)) {
                    OSReport("OSCheckActiveThreads: Failed thread->priority == "
                             "__OSGetEffectivePriority(thread) in %d\n",
                             __LINE__);
                    OSPanic(__FILE__, __LINE__, "");
                }
            }
            break;
        case OS_THREAD_STATE_WAITING:
            if (thread->suspend > 0) {
                OSReport("OSCheckActiveThreads: Failed !IsSuspended(thread->suspend) in %d\n",
                         __LINE__);
                OSPanic(__FILE__, __LINE__, "");
            }
            if (thread->queue != NULL) {
                OSReport("OSCheckActiveThreads: Failed thread->queue == NULL in %d\n", __LINE__);
                OSPanic(__FILE__, __LINE__, "");
            }
            if (thread->priority != __OSGetEffectivePriority(thread)) {
                OSReport("OSCheckActiveThreads: Failed thread->priority == "
                         "__OSGetEffectivePriority(thread) in %d\n",
                         __LINE__);
                OSPanic(__FILE__, __LINE__, "");
            }
            break;
        case OS_THREAD_STATE_RUNNING:
            if (thread->queue == NULL) {
                OSReport("OSCheckActiveThreads: Failed thread->queue != NULL in %d\n", __LINE__);
                OSPanic(__FILE__, __LINE__, "");
            }
            if (!CheckThreadQueue(thread->queue)) {
                OSReport("OSCheckActiveThreads: Failed CheckThreadQueue(thread->queue) in %d\n",
                         __LINE__);
                OSPanic(__FILE__, __LINE__, "");
            }
            for (next = thread->queue->head; next != NULL; next = next->link.next) {
                if (next == thread) {
                    break;
                }
            }
            if (next == NULL) {
                OSReport("OSCheckActiveThreads: Failed IsMember(thread->queue, thread) in %d\n",
                         __LINE__);
                OSPanic(__FILE__, __LINE__, "");
            }
            if (thread->suspend <= 0) {
                if (thread->priority != __OSGetEffectivePriority(thread)) {
                    OSReport("OSCheckActiveThreads: Failed thread->priority == "
                             "__OSGetEffectivePriority(thread) in %d\n",
                             __LINE__);
                    OSPanic(__FILE__, __LINE__, "");
                }
            } else if (thread->priority != OS_PRIORITY_MAX + 1) {
                OSReport("OSCheckActiveThreads: Failed thread->priority == 32 in %d\n", __LINE__);
                OSPanic(__FILE__, __LINE__, "");
            }
            if (__OSCheckDeadLock(thread)) {
                OSReport("OSCheckActiveThreads: Failed !__OSCheckDeadLock(thread) in %d\n", __LINE__);
                OSPanic(__FILE__, __LINE__, "");
            }
            break;
        case OS_THREAD_STATE_MORIBUND:
            if (thread->queueMutex.head != NULL || thread->queueMutex.tail != NULL) {
                OSReport("OSCheckActiveThreads: Failed thread->queueMutex.head == NULL && "
                         "thread->queueMutex.tail == NULL in %d\n",
                         __LINE__);
                OSPanic(__FILE__, __LINE__, "");
            }
            break;
        default:
            OSReport("OSCheckActiveThreads: Failed. unkown thread state (%d) of thread %p\n",
                     thread->state, thread);
            OSPanic(__FILE__, __LINE__, "");
            break;
        }

        if (!__OSCheckMutexes(thread)) {
            OSReport("OSCheckActiveThreads: Failed __OSCheckMutexes(thread) in %d\n", __LINE__);
            OSPanic(__FILE__, __LINE__, "");
        }
    }

    OSRestoreInterrupts(flag);

    return result;
}

void OSClearStack(u8 val)
{
    u32 pattern;
    u32* address;
    u32 words;
    u32 i;

    pattern = val * 0x01010101;

    address = (u32*)OSGetStackPointer();
    address = (u32*)OSGetCurrentThread()->stackEnd;
    address++;

    words = ((u32)OSGetCurrentThread()->stackEnd - ((u32)address - 4) + 4) >> 2;

    for (i = words / 8; i != 0; i--) {
        address[0] = pattern;
        address[1] = pattern;
        address[2] = pattern;
        address[3] = pattern;
        address[4] = pattern;
        address[5] = pattern;
        address[6] = pattern;
        address[7] = pattern;
        address += 8;
    }
    words &= 7;
    if (words != 0) {
        for (i = words; i != 0; i--) {
            *address++ = pattern;
        }
    }
}