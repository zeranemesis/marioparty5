#ifndef _M539DLL_H
#define _M539DLL_H

// Declared here rather than through humath.h/dolphin/mtx.h: in C++ those
// headers pull in MSL math.h, whose inline sqrtf emits its own constants
// into .rodata and would shift every literal of the module. A unit that
// includes the real dolphin headers defines M539_USE_DOLPHIN_HEADERS first.
#include "dolphin/types.h"

extern "C" {

#ifndef M539_USE_DOLPHIN_HEADERS
typedef struct {
    f32 x, y, z;
} Vec, *VecPtr, Point3d, *Point3dPtr;
#endif

// Allocation size for a struct whose type is not visible to the allocating
// unit. shortcut: twice the GameCube size on PC (every field is at most
// pointer wide), replace with sizeof once the types live in one header.
#ifdef TARGET_PC
#define M539_ALLOC_SIZE(n) ((n) * 2)
#else
#define M539_ALLOC_SIZE(n) (n)
#endif

#ifndef M539_USE_DOLPHIN_HEADERS
void PSVECAdd(const Vec *a, const Vec *b, Vec *dst);
void PSVECSubtract(const Vec *a, const Vec *b, Vec *dst);
void PSVECNormalize(Vec *v, Vec *dst);
void PSVECScale(const Vec *src, Vec *dst, f32 scale);
f32 PSVECMag(const Vec *v);
double atan2(double y, double x);
double sin(double x);
double cos(double x);
double fmod(double x, double y);
#endif

// Pointer-to-member-function slot. The GameCube build keeps the MWCC layout
// (this-delta, vtable index, function address); on PC the slot is just the
// function pointer, called directly with the object.
#ifdef TARGET_PC
typedef struct M539Ptmf_s {
    void (*fn)(void *self);
} M539Ptmf;
#define M539_PTMF_INIT(f) { (void (*)(void *))(f) }
#define M539_PTMF_NULL_INIT { 0 }
#define M539_PTMF_SET_FROM(dst, tbl, f) ((dst).fn = (void (*)(void *))(f))
#define M539_PTMF_NULL M539PtmfNull
static const M539Ptmf M539PtmfNull = { 0 };
#define M539PtmfTest(p) ((p)->fn != 0)
#define M539PtmfCall(obj, p) ((p)->fn(obj))
#else
typedef struct M539Ptmf_s {
    s32 delta;
    s32 vidx;
    void *fn;
} M539Ptmf;
#define M539_PTMF_INIT(f) { 0, -1, (void *)(f) }
#define M539_PTMF_NULL_INIT { 0, -1, 0 }
#define M539_PTMF_SET_FROM(dst, tbl, f) ((dst).delta = (tbl).delta, (dst).vidx = (tbl).vidx, (dst).fn = (void *)(f))
#define M539_PTMF_NULL __ptmf_null
extern M539Ptmf __ptmf_null;
s32 __ptmf_test(M539Ptmf *p);
void __ptmf_scall(void *obj, M539Ptmf *p);
#define M539PtmfTest(p) __ptmf_test(p)
#define M539PtmfCall(obj, p) __ptmf_scall((obj), (p))
#endif

typedef struct M539Heiho_s {
    s32 curIdx;
    Vec pos;
    Vec target;
    Vec dir;
    f32 angle;
    f32 heading;
    void *actor;
    Point3d *path;
    s32 pathCount;
} M539Heiho;

// ---- shared player / game-object types (player.cpp, game_master.cpp)

typedef struct M539Vec_s {
    u32 cap;
    u32 size;
    s16 *items;
} M539Vec;

struct M539PlayerSub_s;
typedef struct M539PlayerSubVtbl_s {
    void *rtti[2];
    void *(*dtor)(struct M539PlayerSub_s *self, s16 flag);
} M539PlayerSubVtbl;

// Pad controller owned by a player.
typedef struct M539PlayerSub_s {
    M539PlayerSubVtbl *vtbl;
    u8 pad4[4];
    u8 flag8;
} M539PlayerSub;

typedef struct M539Node_s {
    struct M539Node_s *next;
    struct M539Node_s *prev;
} M539Node;

// Hu3D model wrapper: owns one model id.
typedef struct M539Model_s {
    s16 id;
} M539Model;

typedef struct M539Fade_s {
    s32 frames;
    s32 cur;
    f32 from;
    f32 hold;
    f32 to;
} M539Fade;

// The drum prop (drum.cpp): intrusive link, spin state, three models.
typedef struct M539Drum_s {
    M539Node link;
    f32 angle;
    f32 speed;
    s32 state;
    u8 pad14[0x24 - 0x14];
    M539Model *mdl[3];
    s32 timer;
} M539Drum;

typedef struct M539ScoreSpr_s {
    M539Node link;
    f32 x;
    f32 y;
    f32 z;
    s32 value;
    s32 frame;
    void *score;
    M539Ptmf hook;
} M539ScoreSpr;

// Per-player game object (the 0xC8 byte record the player reports to): the
// seven-segment board above a player (score.cpp).
typedef struct M539Game_s {
    M539Node link;
    s32 player;
    u8 pad0C[0x1C - 0x0C];
    M539Model *board;
    M539Model *roller;
    M539Drum *digits[3][3];
    M539Model *block;
    M539Model *head;
    M539Model *sprs[3];
    Vec headPos;
    Vec lastPos;
    Vec delta;
    f32 rot;
    M539Fade digitFade;
    M539Fade rollFade;
    s32 step;
    s32 rollValue;
    M539ScoreSpr *effect;
    s32 unkB8;
    s32 loopSnd;
    s32 lastDigit;
    s32 tick;
} M539Game;

typedef struct M539Player_s {
    void *node0;
    void *node4;
    M539PlayerSub *sub;
    M539Ptmf hook;
    s32 plyIdx;
    s32 charNo;
    f32 height;
    Vec pos;
    s32 unk30;
    s32 curMot;
    s32 state;
    M539Vec mot;
    M539Model *model;
    Vec vel;
    Vec acc;
    f32 rot;
    u8 flag68;
    s32 timer;
    M539Game *game;
} M539Player;

#ifdef M539_USE_DOLPHIN_HEADERS
typedef struct M539MasterVtbl_s {
    void *rtti[2];
    void *(*dtor)(void *self, s16 flag);
    void (*setup)(void *self, void *objman);
    void (*update)(void *self);
    void (*finish)(void *self);
} M539MasterVtbl;

typedef struct M539Master_s {
    s32 pad0;
    M539MasterVtbl *vtbl;
    s32 slot[4];
    s32 mesId;
    s32 counter;
} M539Master;

typedef struct M539ThreadVtbl_s {
    void *rtti[2];
    void *(*dtor)(void *self, s16 flag);
    s32 (*run)(void *self, s32 arg);
    void (*runMaster)(void *self);
} M539ThreadVtbl;

typedef struct M539Thread_s {
    OSThread thread;
    u32 *stack;
} M539Thread;

typedef struct M539Sem_s {
    s32 count;
    OSThreadQueue queue;
} M539Sem;

typedef struct M539SubThread_s {
    M539ThreadVtbl *vtbl;
    M539Thread th;
    M539Sem sem;
    s32 arg;
} M539SubThread;

typedef struct M539TimerObj_s {
    MGTIMER *timer;
} M539TimerObj;

// The two lists live in one 0x10 byte holder: players first, game objects second.
typedef struct M539Holder_s {
    M539Node players;
    M539Node games;
} M539Holder;

typedef struct M539GM_s {
    M539Master base;
    M539SubThread thread;
    M539TimerObj timer;
    M539Player *players[4];
    M539Holder *holder;
    M539Game *games[4];
} M539GM;
#endif

void fn_1_C8(void *ptr);
void *fn_1_558C(void *self, s32 prio);
void *fn_1_5608(void *self, s16 arg);
void fn_1_56BC(void *self, s32 arg);
void fn_1_574C(void *self, s32 arg);

M539Model *fn_1_72E8(s32 id);
void fn_1_2E08(void *actor, u32 flags);
void fn_1_2EF4(void *actor, Vec *pos);
void fn_1_2F3C(void *actor, Vec *rot);
void fn_1_9EC0(M539Heiho *self);

}

#endif
