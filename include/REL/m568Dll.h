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

struct SceneNode;

extern "C" {

extern void *lbl_1_bss_9F0;
extern SceneNode *lbl_1_bss_9F4;
extern SceneNode *lbl_1_bss_9F8;
extern SceneNode *lbl_1_bss_A0C;

void fn_1_7EC(const char *expr, const char *file, s32 line);
void fn_1_6E0(void *mem);
void fn_1_187C(void *link);
void fn_1_1888(void *link, s32 arg);

}

#endif
