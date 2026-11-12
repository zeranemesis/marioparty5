#ifndef _REL_M568DLL_H
#define _REL_M568DLL_H

// Shared declarations for the m568Dll minigame (C++). game/hu3d.h, humath.h
// and game/object.h are not included from here: they pull in MSL math.h,
// whose inline sqrt emits constants into .rodata that the units don't have.

#include "dolphin/types.h"
#include "dolphin/mtx.h"

// Intrusive doubly-linked node (self-linked when unlinked).
struct M568Link {
    M568Link *prev;  // 0x00
    M568Link *next;  // 0x04
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

struct SceneNode;
struct EffectMgr;

extern "C" {

extern M568PosOwner *lbl_1_bss_9F0;
extern SceneNode *lbl_1_bss_9F4;
extern SceneNode *lbl_1_bss_9F8;
extern SceneNode *lbl_1_bss_A0C;
extern EffectMgr *lbl_1_bss_A10;

void fn_1_7EC(const char *expr, const char *file, s32 line);
void fn_1_6E0(void *mem);
void fn_1_187C(void *link);
void *fn_1_1888(void *link, s16 flag);

}

#endif
