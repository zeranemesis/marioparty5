#ifndef _M532_MAN_REFEREE_H
#define _M532_MAN_REFEREE_H

// Declared by hand (not through humath.h/mtx.h) so no MSL inline sqrtf is pulled in.
#include "dolphin/types.h"

extern "C" {

typedef struct {
    f32 x, y, z;
} M532RefVec3;

// Pointer-to-member slot: MWCC layout on GameCube, plain function pointer on PC.
#ifdef TARGET_PC
typedef struct M532RefPtmf_s {
    void (*fn)(void *self);
} M532RefPtmf;
#define M532_REF_PTMF_INIT(f) { (void (*)(void *))(f) }
#define M532RefPtmfCall(obj, p) ((p)->fn(obj))
#else
typedef struct M532RefPtmf_s {
    s32 delta;
    s32 vidx;
    void *fn;
} M532RefPtmf;
#define M532_REF_PTMF_INIT(f) { 0, -1, (void *)(f) }
void __ptmf_scall(void *obj, M532RefPtmf *p);
#define M532RefPtmfCall(obj, p) __ptmf_scall((obj), (p))
#endif

typedef struct M532RefNode_s {
    struct M532RefNode_s *next;
    struct M532RefNode_s *prev;
} M532RefNode;

// Hu3D model wrapper: owns one model id.
typedef struct M532RefModel_s {
    s16 id;
} M532RefModel;

// std::vector<s16>
typedef struct M532RefVec_s {
    u32 cap;
    u32 size;
    s16 *items;
} M532RefVec;

typedef struct M532RefVtbl_s {
    void *rtti[2];
    void *(*dtor)(void *self, s16 flag);
    void (*event)(void *self, s32 ev);
    void (*update)(void *self);
} M532RefVtbl;

// The referee singleton (man_referee.cpp). Size is sizeof() on PC.
typedef struct M532Referee_s {
    M532RefNode node;           // 0x00
    void *vtbl;                 // 0x08
    u32 flags;                  // 0x0C
    M532RefModel model;         // 0x10
    M532RefPtmf state;          // 0x14
    M532RefModel hookR;         // 0x20
    M532RefModel hookL;         // 0x22
    s32 ballId;                 // 0x24
    M532RefVec3 pos;            // 0x28
    M532RefVec3 target;         // 0x34
    M532RefVec3 vel;            // 0x40
    s32 counter;                // 0x4C
    M532RefVec mot;             // 0x50
    s32 motion;                 // 0x5C
    f32 lastTime;               // 0x60
} M532Referee;

extern M532Referee *lbl_1_bss_A24;

}

#endif
