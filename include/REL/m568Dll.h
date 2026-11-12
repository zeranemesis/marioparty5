#ifndef _REL_M568DLL_H
#define _REL_M568DLL_H

// Shared declarations for the m568Dll minigame (C++). game/hu3d.h, humath.h
// and game/object.h are not included from here: they pull in MSL math.h,
// whose inline sqrt emits constants into .rodata that the units don't have.

#include "dolphin/types.h"
#include "dolphin/mtx.h"
#include "dolphin/os.h"

// Intrusive doubly-linked node (self-linked when unlinked).
struct M568Link {
    M568Link *next;  // 0x00
    M568Link *prev;  // 0x04
};

// Model handle wrapper: the first halfword is the Hu3D model id.
struct M568Model {
    s16 id;  // 0x00
};

// Anything with virtual slots at vtable offsets 0x8 and 0xC (the sub-object at
// +0x10 of the singleton behind lbl_1_bss_9F0).
struct M568PosSource {
    virtual void Project(Vec *io, Vec *a, Vec *b);
    virtual Vec *GetPos();
};

struct M568PosOwner {
    u8 pad[0x10];
    M568PosSource src;  // 0x10
};

// Element of the lbl_1_bss_9F4 list (the octtree effects).
struct M568Effect {
    M568Link link;     // 0x00
    Vec pos;           // 0x08
    f32 radius;        // 0x14
    M568Model model;   // 0x18
    Vec cur;           // 0x1C
    u8 played;         // 0x28
    s32 seId;          // 0x2C
};

// Element of the lbl_1_bss_9F8 list (the actors the pad/AI inputs drive); only
// the fields the units touch are named, the gaps hold data this module never
// reads.
struct SceneNode {
    M568Link link;     // 0x00
    u8 inactive;       // 0x08
    u8 pad09[3];
    s32 slot;          // 0x0C
    u8 pad10[0x64];
    Vec pos;           // 0x74
    f32 dirY;          // 0x80
    u8 pad84[0xC];
    Vec vel;           // 0x90
    u8 pad9C[0x1C];
    u8 dead;           // 0xB8
};

struct EffectMgr;

// Singleton game master: the base part (module-wide plumbing, m568.cpp), the
// sub-thread the scene script runs on, and the owned scene set.
struct M568Master;
struct M568MasterVtbl {
    void *rtti[2];
    M568Master *(*dtor)(M568Master *self, s16 flag);
    void (*setup)(M568Master *self, void *objman);
    void (*update)(M568Master *self);
    void (*finish)(M568Master *self);
};

struct M568Master {
    s32 pad0;
    M568MasterVtbl *vtbl;  // 0x04
    s32 slot[4];           // 0x08 registered player numbers, -1 = free
    s32 mesId;             // 0x18
    s32 counter;           // 0x1C
};

struct M568SubThread;
struct M568ThreadVtbl {
    void *rtti[2];
    M568SubThread *(*dtor)(M568SubThread *self, s16 flag);
    s32 (*run)(M568SubThread *self, s32 arg);
};

struct M568Thread {
    OSThread thread;
    u32 *stack;
};

struct M568Sem {
    s32 count;
    OSThreadQueue queue;
};

struct M568SubThread {
    M568ThreadVtbl *vtbl;
    M568Thread th;
    M568Sem sem;
    s32 arg;
};

// The three intrusive lists the scene owns (a_set.h): groups, actors, effects.
struct M568Set {
    M568Link groups;   // 0x00
    M568Link actors;   // 0x08
    M568Link effects;  // 0x10
};

struct GameMaster {
    M568Master base;       // 0x00
    M568SubThread thread;  // 0x20
    M568Set *set;          // 0x358 (the original's smart pointer holder)
};

extern "C" {

extern M568PosOwner *lbl_1_bss_9F0;
extern M568Effect *lbl_1_bss_9F4;
extern SceneNode *lbl_1_bss_9F8;
extern EffectMgr *lbl_1_bss_A10;

void fn_1_7EC(const char *expr, const char *file, s32 line);
void fn_1_6E0(void *mem);
void fn_1_187C(void *link);
void *fn_1_1888(void *link, s16 flag);

}

#endif
