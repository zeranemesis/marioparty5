#include "game/process.h"
#include "game/memory.h"
#include "dolphin/os.h"

#define FAKE_RETADDR 0xA5A5A5A5


#ifdef TARGET_PC
/*
 * The scheduler runs on the caller's context (processthread). A process gives
 * control back with co_switch after setting threadret, which plays the role of
 * the value gclongjmp passed to the scheduler: 1 = next process, 2 = the
 * current process ended and its heap must be freed.
 */
static cothread_t processthread;
static s32 threadret;
#else
static jmp_buf processjmpbuf;
#endif
static HUPROCESS *processtop;
static HUPROCESS *processcur;
static u16 processcnt;
u32 procfunc;

void HuPrcInit(void)
{
    processcnt = 0;
    processtop = NULL;
}

static void LinkProcess(HUPROCESS **root, HUPROCESS *process) {
    HUPROCESS *src_process = *root;

    if (src_process && (src_process->prio >= process->prio)) {
        while (src_process->next && src_process->next->prio >= process->prio) {
            src_process = src_process->next;
        }

        process->next = src_process->next;
        process->prev = src_process;
        src_process->next = process;
        if (process->next) {
            process->next->prev = process;
        }
    } else {
        process->next = (*root);
        process->prev = NULL;
        *root = process;
        if (src_process) {
            src_process->prev = process;
        }
    }
}
static void UnlinkProcess(HUPROCESS **root, HUPROCESS *process) {
    if (process->next) {
        process->next->prev = process->prev;
    }
    if (process->prev) {
        process->prev->next = process->next;
    } else {
        *root = process->next;
    }
}

HUPROCESS *HuPrcCreate(void (*func)(void), u16 prio, u32 stackSize, s32 heapSize)
{
    HUPROCESS *process;
    s32 allocSize;
    void *heap;
    if(stackSize == 0) {
        stackSize = 2048;
    }
#ifdef TARGET_PC
    /* the stack is owned by libco; host frames are several times larger than on PowerPC */
    allocSize = HuMemMemoryAllocSizeGet(sizeof(HUPROCESS))
                    +HuMemMemoryAllocSizeGet(heapSize);
#else
    allocSize = HuMemMemoryAllocSizeGet(sizeof(HUPROCESS))
                    +HuMemMemoryAllocSizeGet(stackSize)
                    +HuMemMemoryAllocSizeGet(heapSize);
#endif
    if(!(heap = HuMemDirectMalloc(HEAP_HEAP, allocSize))) {
        OSReport("process> malloc error size %d\n", allocSize);
        return NULL;
    }
    HuMemHeapInit(heap, allocSize);
    process = HuMemMemoryAlloc(heap, sizeof(HUPROCESS), FAKE_RETADDR);
    process->heap = heap;
    process->exec = HUPRC_EXEC_NORMAL;
    process->stat = 0;
    process->prio = prio;
    process->sleep = 0;
#ifdef TARGET_PC
    process->thread = co_create(stackSize * 8 < 0x10000 ? 0x10000 : stackSize * 8, func);
#else
    process->spBase = ((u32)HuMemMemoryAlloc(heap, stackSize, FAKE_RETADDR))+stackSize-8;
    gcsetjmp(&process->jump);
    process->jump.lr = (u32)func;
    process->jump.sp = process->spBase;
#endif
    process->destructor = NULL;
    process->property = NULL;
    LinkProcess(&processtop, process);
    process->child = NULL;
    process->parent = NULL;
    processcnt++;
    return process;
}

void HuPrcChildLink(HUPROCESS *parent, HUPROCESS *child)
{
    HuPrcChildUnlink(child);
    if(parent->child) {
        parent->child->firstChild = child;
    }
    child->nextChild = parent->child;
    child->firstChild = NULL;
    parent->child = child;
    child->parent = parent;
}

void HuPrcChildUnlink(HUPROCESS *process)
{
    if(process->parent) {
        if(process->nextChild) {
            process->nextChild->firstChild = process->firstChild;
        }
        if(process->firstChild) {
            process->firstChild->nextChild = process->nextChild;
        } else {
            process->parent->child = process->nextChild;
        }
        process->parent = NULL;
    }
}

HUPROCESS *HuPrcChildCreate(void (*func)(void), u16 prio, u32 stackSize, s32 heapSize, HUPROCESS *parent)
{
    HUPROCESS *child = HuPrcCreate(func, prio, stackSize, heapSize);
    HuPrcChildLink(parent, child);
    return child;
}

void HuPrcChildWatch()
{
    HUPROCESS *curr = HuPrcCurrentGet();
    if(curr->child) {
        curr->exec = HUPRC_EXEC_CHILDWATCH;
#ifdef TARGET_PC
        threadret = 1;
        co_switch(processthread);
#else
        if(!gcsetjmp(&curr->jump)) {
            gclongjmp(&processjmpbuf, 1);
        }
#endif
    }
}

HUPROCESS *HuPrcCurrentGet()
{
    return processcur;
}

static s32 SetKillStatusProcess(HUPROCESS *process)
{
    if(process->exec != HUPRC_EXEC_KILLED) {
        HuPrcWakeup(process);
        process->exec = HUPRC_EXEC_KILLED;
        return 0;
    } else {
        return -1;
    }
}

s32 HuPrcKill(HUPROCESS *process)
{
    if(process == NULL) {
        process = HuPrcCurrentGet();
    }
    HuPrcChildKill(process);
    HuPrcChildUnlink(process);
    return SetKillStatusProcess(process);
}

void HuPrcChildKill(HUPROCESS *process)
{
    HUPROCESS *child = process->child;
    while(child) {
        if(child->child) {
            HuPrcChildKill(child);
        }
        SetKillStatusProcess(child);
        child = child->nextChild;
    }
    process->child = NULL;
}

static void gcTerminateProcess(HUPROCESS *process)
{
    if(process->destructor) {
        process->destructor();
    }
    UnlinkProcess(&processtop, process);
    processcnt--;
#ifdef TARGET_PC
    threadret = 2;
    co_switch(processthread);
#else
    gclongjmp(&processjmpbuf, 2);
#endif
}

void HuPrcEnd()
{
    HUPROCESS *process = HuPrcCurrentGet();
    HuPrcChildKill(process);
    HuPrcChildUnlink(process);
    gcTerminateProcess(process);
}

void HuPrcSleep(s32 time)
{
    HUPROCESS *process = HuPrcCurrentGet();
    if(time != 0 && process->exec != HUPRC_EXEC_KILLED) {
        process->exec = HUPRC_EXEC_SLEEP;
        process->sleep = time;
    }
#ifdef TARGET_PC
    threadret = 1;
    co_switch(processthread);
#else
    if(!gcsetjmp(&process->jump)) {
        gclongjmp(&processjmpbuf, 1);
    }
#endif
}

void HuPrcVSleep()
{
    HuPrcSleep(0);
}

void HuPrcWakeup(HUPROCESS *process)
{
    process->sleep = 0;
}

void HuPrcDestructorSet2(HUPROCESS *process, void (*func)(void))
{
    process->destructor = func;
}

void HuPrcDestructorSet(void (*func)(void))
{
    HUPROCESS *process = HuPrcCurrentGet();
    process->destructor = func;
}

void HuPrcCall(s32 tick)
{
    HUPROCESS *process;
    s32 ret;
    processcur = processtop;
#ifdef TARGET_PC
    processthread = co_active();
    ret = 0;
#else
    ret = gcsetjmp(&processjmpbuf);
#endif
    while(1) {
        switch(ret) {
            case 2:
#ifdef TARGET_PC
            {
                /* the process header lives in the heap being freed */
                HUPROCESS *next = processcur->next;
                co_delete(processcur->thread);
                HuMemDirectFree(processcur->heap);
                processcur = next;
                break;
            }
#else
                HuMemDirectFree(processcur->heap);
#endif
            case 1:
                if(((u8 *)(processcur->heap))[4] != 165) {
#ifdef TARGET_PC
                    printf("stack overlap error.(process pointer %p)\n", (void *)processcur);
#else
                    printf("stack overlap error.(process pointer %x)\n", processcur);
#endif
                    while(1);
                } else {
                    processcur = processcur->next;
                }
                break;
        }
        process = processcur;
        if(!process) {
            return;
        }
#ifndef TARGET_PC
        procfunc = process->jump.lr;
#endif
        if((process->stat & (HU_PRC_STAT_PAUSE|HU_PRC_STAT_UPAUSE)) && process->exec != HUPRC_EXEC_KILLED) {
            ret = 1;
            continue;
        }
        switch(process->exec) {
            case HUPRC_EXEC_SLEEP:
                if(process->sleep > 0) {
                    process->sleep -= tick;
                    if(process->sleep <= 0) {
                        process->sleep = 0;
                        process->exec = HUPRC_EXEC_NORMAL;
                    }
                }
                ret = 1;
                break;
                
            case HUPRC_EXEC_CHILDWATCH:
                if(process->child) {
                    ret = 1;
                } else {
                    process->exec = HUPRC_EXEC_NORMAL;
                    ret = 0;
                }
                break;
                
            case HUPRC_EXEC_KILLED:
#ifdef TARGET_PC
                /* HuPrcEnd switches to processthread, which is already running */
                HuPrcEnd();
                ret = threadret;
                continue;
#else
                process->jump.lr = (u32)HuPrcEnd;
#endif
            case HUPRC_EXEC_NORMAL:
#ifdef TARGET_PC
                co_switch(process->thread);
                ret = threadret;
                continue;
#else
                gclongjmp(&process->jump, 1);
#endif
                break;
        }
    }
}

void *HuPrcMemAlloc(s32 size)
{
    HUPROCESS *process = HuPrcCurrentGet();
    return HuMemMemoryAlloc(process->heap, size, FAKE_RETADDR);
}

void HuPrcMemFree(void *ptr)
{
    HuMemMemoryFree(ptr, FAKE_RETADDR);
}

void HuPrcSetStat(HUPROCESS *process, u16 value)
{
    process->stat |= value;
}

void HuPrcResetStat(HUPROCESS *process, u16 value)
{
    process->stat &= ~value;
}

void HuPrcAllPause(s32 flag)
{
    HUPROCESS *process = processtop;
    if(flag) {
        while(process != NULL) {
            if(!(process->stat & HU_PRC_STAT_PAUSE_ON)) {
                HuPrcSetStat(process, HU_PRC_STAT_PAUSE);
            }
            
            process = process->next;
        }
    } else {
        while(process != NULL) {
            if(process->stat & HU_PRC_STAT_PAUSE) {
                HuPrcResetStat(process, HU_PRC_STAT_PAUSE);
            }
            
            process = process->next;
        }
    }
}

void HuPrcAllUPause(s32 flag)
{
    HUPROCESS *process = processtop;
    if(flag) {
        while(process != NULL) {
            if(!(process->stat & HU_PRC_STAT_UPAUSE_ON)) {
                HuPrcSetStat(process, HU_PRC_STAT_UPAUSE);
            }
            
            process = process->next;
        }
    } else {
        while(process != NULL) {
            if(process->stat & HU_PRC_STAT_UPAUSE) {
                HuPrcResetStat(process, HU_PRC_STAT_UPAUSE);
            }
            
            process = process->next;
        }
    }
}
