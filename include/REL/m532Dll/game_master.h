#ifndef _M532DLL_GAME_MASTER_H
#define _M532DLL_GAME_MASTER_H

extern "C" {
#include "dolphin.h"
}

// Allocation size for a struct whose type is not visible to the allocating
// unit. shortcut: twice the GameCube size on PC (every field is at most
// pointer wide), replace with sizeof once the types live in one header.
#ifndef M532_ALLOC_SIZE
#ifdef TARGET_PC
#define M532_ALLOC_SIZE(n) ((n) * 2)
#else
#define M532_ALLOC_SIZE(n) (n)
#endif
#endif

typedef struct M532MasterVtbl_s {
    void *rtti[2];
    void *(*dtor)(void *self, s16 flag);
    void (*setup)(void *self, void *objman);
    void (*update)(void *self);
    void (*finish)(void *self);
} M532MasterVtbl;

typedef struct M532Master_s {
    s32 pad0;
    M532MasterVtbl *vtbl;
    s32 slot[6];
} M532Master;

// Sub-thread vtable (this-adjusted thunks for dtor and the run slot).
typedef struct M532ThreadVtbl_s {
    void *rtti[2];
    void *(*dtor)(void *self, s16 flag);
    void (*run)(void *self);
    void (*unk)(void *self);
    void (*runMaster)(void *self);
} M532ThreadVtbl;

#ifdef TARGET_PC
typedef OSSemaphore M532Sem; /* Aurora's semaphore is larger than the GC one */
#else
typedef struct M532Sem_s {
    s32 count;
    OSThreadQueue queue;
} M532Sem;
#endif

typedef struct M532SubThread_s {
    M532ThreadVtbl *vtbl;
    OSThread thread;
    u32 *stack;
    M532Sem sem;
    s32 arg; // 0x354 on GameCube (written by fn_1_7324)
} M532SubThread;

typedef struct M532TimerObj_s {
    void *timer;
} M532TimerObj;

typedef struct M532Vec_s {
    void *a;
    void *b;
    void *c;
} M532Vec;


typedef struct M532GM_s {
    M532Master base;
    M532SubThread thread;
    M532TimerObj timer;
    M532Vec vec;
} M532GM;

extern "C" {
extern void *lbl_1_bss_A24; /* referee singleton */
extern void *lbl_1_bss_A28; /* score singleton */
}

#endif
