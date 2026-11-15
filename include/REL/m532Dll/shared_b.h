#ifndef _M532DLL_SHARED_B_H
#define _M532DLL_SHARED_B_H

#include "dolphin/types.h"

// Types of units owned by the other agent (m532.cpp, unk.cpp), declared minimally.
// Each section is guarded so several units may declare the same type.

// ---- ball.cpp: model wrapper and object base (m532.cpp / unk.cpp)
#ifndef M532_BALL_SHARED
#define M532_BALL_SHARED
// Intrusive list node (fn_1_32B0 initialises it).
typedef struct M532BallNode_s {
    struct M532BallNode_s *next;
    struct M532BallNode_s *prev;
} M532BallNode;

// Hu3D model wrapper (fn_1_4C54 creates, fn_1_4D74 links, fn_1_50B4 kills).
typedef struct M532Model_s {
    s16 id;
} M532Model;

// Base of the game objects built by fn_1_EA08 (unk.cpp). `hook` is the draw hook model.
typedef struct M532ObjVtbl_s {
    void *rtti[2];
    void *(*dtor)(void *self, s16 flag);
    void (*update)(void *self);
    void (*draw)(void *self, f32 (*mtx)[4]);
    void (*slot3)(void *self);
} M532ObjVtbl;

typedef struct M532Obj_s {
    M532BallNode node;
    M532Model hook;
    M532ObjVtbl *vtbl;
    void *handle;
} M532Obj;
#endif

#endif
