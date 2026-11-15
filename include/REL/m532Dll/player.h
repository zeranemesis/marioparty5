#ifndef _M532DLL_PLAYER_H
#define _M532DLL_PLAYER_H

#include "dolphin/types.h"

// m532Dll player.cpp types. Include after dolphin.h (Vec).

// Pointer-to-member-function slot used for the player state machine. The
// GameCube build keeps the MWCC layout; on PC the slot is the function pointer.
#ifdef TARGET_PC
typedef struct M532Ptmf_s {
    void (*fn)(void *self);
} M532Ptmf;
#define M532_PTMF_INIT(f) { (void (*)(void *))(f) }
#define M532PtmfIs(p, c) ((p)->fn == (c).fn)
#define M532PtmfCall(obj, p) ((p)->fn(obj))
#else
typedef struct M532Ptmf_s {
    s32 delta;
    s32 vidx;
    void *fn;
} M532Ptmf;
#define M532_PTMF_INIT(f) { 0, -1, (void *)(f) }
extern "C" s32 __ptmf_cmpr(M532Ptmf *a, M532Ptmf *b);
extern "C" void __ptmf_scall(void *obj, M532Ptmf *p);
#define M532PtmfIs(p, c) (__ptmf_cmpr((p), (M532Ptmf *)&(c)) == 0)
#define M532PtmfCall(obj, p) __ptmf_scall((obj), (p))
#endif

typedef struct M532Node_s {
    struct M532Node_s *next;
    struct M532Node_s *prev;
} M532Node;

// Hu3D character model wrapper: owns one model id.
typedef struct M532Model_s {
    s16 id;
    s16 pad2;
} M532Model;

// Wrapping angle: [min, max) with span = max - min.
typedef struct M532Range_s {
    f32 min;
    f32 max;
    f32 span;
    f32 cur;
} M532Range;

// std::vector<Vec> / std::vector<s16>: capacity, size, data.
typedef struct M532VecV_s {
    u32 cap;
    u32 size;
    Vec *items;
} M532VecV;

typedef struct M532Vec16_s {
    u32 cap;
    u32 size;
    s16 *items;
} M532Vec16;

// One player on a team list (AA0 / AB4). 0x80 bytes on GameCube.
typedef struct M532Player_s {
    M532Node link;
    M532Model model;
    s32 plyIdx;
    s32 charNo;
    s32 team;
    s32 padNo; // -1 for CPU
    s32 dif;
    s32 side; // 0 = left team list, 1 = right
    s32 target; // ball handle (0 = none)
    s32 t28;
    s32 t2C;
    s32 t30;
    Vec pos;
    M532VecV path;
    M532Range range;
    f32 tgtAng;
    s32 timer;
    M532Ptmf state;
    M532Vec16 motions;
    s32 curMot;
} M532Player;

#ifndef TARGET_PC
typedef char M532PlayerSizeCheck[(sizeof(M532Player) == 0x80) ? 1 : -1];
#endif

#endif
