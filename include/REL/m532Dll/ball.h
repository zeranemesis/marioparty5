#ifndef _M532DLL_BALL_H
#define _M532DLL_BALL_H

#include "dolphin.h"
#include "game/animdata.h"
#include "REL/m532Dll/shared_b.h"

// Pointer-to-member slot: MWCC layout on GameCube, plain function pointer on PC.
#ifdef TARGET_PC
typedef struct M532BallPtmf_s {
    u8 (*fn)(void *self);
} M532BallPtmf;
#define M532_BALL_PTMF_INIT(f) { (u8 (*)(void *))(f) }
#define M532BallPtmfCall(obj, p) ((p)->fn(obj))
#else
typedef struct M532BallPtmf_s {
    s32 delta;
    s32 vidx;
    void *fn;
} M532BallPtmf;
#define M532_BALL_PTMF_INIT(f) { 0, -1, (void *)(f) }
u8 __ptmf_scall(void *obj, M532BallPtmf *p);
#define M532BallPtmfCall(obj, p) __ptmf_scall((obj), (p))
#endif

typedef struct M532BallMgrVtbl_s {
    void *rtti[2];
    void *(*dtor)(void *self, s16 flag);
    void (*slot1)(void *self, s32 bit);
    void (*slot2)(void *self);
} M532BallMgrVtbl;

// Owner of a Model; the manager keeps four of them in an array.
typedef struct M532BallHolder_s {
    M532Model *model;
} M532BallHolder;

// Singleton (ms_pointer = lbl_1_bss_C): the four ball models and the line texture.
typedef struct M532BallMgr_s {
    M532BallNode node;
    M532BallMgrVtbl *vtbl;
    s32 flags;
    M532BallHolder models[4];
    s16 motion;
    ANIMDATA *anim;
} M532BallMgr;

// Ball sitting on the rotating ring (fn_1_604).
typedef struct M532BallA_s {
    M532Obj base;
    M532Model model;
    f32 phase;
    s32 idx;
    u8 thrown;
    Vec pos;
    Vec target;
    Vec cur;
    Vec vel;
    f32 *rotY;
    f32 spin;
} M532BallA;

// Ball in flight (fn_1_11E4).
typedef struct M532BallB_s {
    M532Obj base;
    M532BallPtmf state;
    M532Model model;
    u8 landed;
    Vec pos;
    Vec target;
    Vec vel;
    Vec acc;
    f32 *rotY;
    f32 spin;
    f32 dist;
} M532BallB;

#endif
