#ifndef M579LIB_H
#define M579LIB_H
/* Library object layouts shared by m579.cpp and game_master.cpp (needs dolphin.h, game/mg/timer.h, m579types.h). */
#ifdef TARGET_PC
#include <dolphin/os/OSSemaphore.h> /* the host semaphore is larger than the GC one */
#else
struct OSSemaphore {
    OSThreadQueue queue;
    s32 count;
};
#endif

/* CMinigameCore: the vtable pointer sits at +4 in the MWCC layout */
typedef struct M579MgCore {
    u32 unk0;
    void **vtbl;
    s32 winner[4];
    s32 gameMes;
    s32 delay;
} M579MgCore;

typedef struct M579DThread {
    OSThread thread;
    u32 *stack;
} M579DThread;

typedef struct M579DSubThread {
    void **vtbl;
    M579DThread th;
    OSSemaphore sema;
    s32 arg;
} M579DSubThread;

typedef struct M579DTimer {
    MGTIMER *timer;
} M579DTimer;
#endif
