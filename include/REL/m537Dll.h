#ifndef _M537DLL_H
#define _M537DLL_H

#include "dolphin.h"
#include "humath.h"
#ifdef __cplusplus
extern "C" {
#endif
#include "game/hu3d.h"
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
extern "C" {
#endif
#include "game/data.h"
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
extern "C" {
#endif
#include "game/memory.h"
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
extern "C" {
#endif
#include "game/audio.h"
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
extern "C" {
#endif
#include "game/object.h"
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
extern "C" {
#endif
#include "game/pad.h"
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
extern "C" {
#endif
#include "game/process.h"
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
extern "C" {
#endif
#include "game/gamework.h"
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
extern "C" {
#endif
#include "game/hsfex.h"
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
extern "C" {
#endif
#include "game/charman.h"
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
extern "C" {
#endif
#include "game/sprite.h"
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __MWERKS__
#define M537_RODATA __declspec(section ".rodata")
#else
#define M537_RODATA
#endif

/* Intrusive list node every m537 work object starts with (see fn_1_1264/1330). */
typedef struct M537Node_s {
    struct M537Node_s *next;
    struct M537Node_s *prev;
} M537_NODE;

/* MWCC pointer-to-member-function triple. */
typedef struct M537PTMF_s {
    s32 thisDelta;
    s32 vOffset;
    void *fn;
} M537_PTMF;

s32 __ptmf_test(M537_PTMF *ptmf);
void __ptmf_scall(void *self);

void fn_1_1D4(const char *cond, const char *file, s32 line);
void fn_1_C8(void *ptr);
void fn_1_1264(void *node);
void *fn_1_1270(void *node, s32 flag);
void fn_1_1330(void *node, void *head);
void fn_1_C08(Vec *pos, s32 mode);
void *fn_1_0(u32 size);
s16 *fn_1_2C94(s16 *out, int dataNum);
s16 *fn_1_2DB4(s16 *out, s16 *modelId);
s16 *fn_1_30F4(s16 *modelId, s16 freeIt);
/* Effect object shared by killereff.cpp and killer.cpp (0x10 bytes). */
typedef struct M537EffObj_s {
    M537_NODE node;
    s16 mdl;
    s32 unkC;
} M537_EFFOBJ;

f32 fn_1_4D1C(void);
void fn_1_12DC(void *node);
M537_EFFOBJ *fn_1_7B3C(M537_EFFOBJ *self, Vec *pos, f32 rotY);
M537_EFFOBJ *fn_1_8098(M537_EFFOBJ *self, Vec *pos);
s16 *fn_1_2D18(s16 *out, s16 charNo, s16 flag);
void fn_1_2EF4(s16 *modelId, Vec *pos);
void fn_1_2F3C(s16 *modelId, Vec *rot);
int fn_1_234(void *file, const char *fmt, ...);
s32 fn_1_4AE0(void);
s32 fn_1_1F30(void);
void fn_1_2E08(s16 *modelId, u32 attr);
void fn_1_2E40(s16 *modelId, u32 attr);
void fn_1_304C(s16 *modelId, s16 layerNo);
void fn_1_31E4(s16 *modelId);
void fn_1_3214(s16 *modelId);
void *__register_global_object(void *object, void *destructor, void *regmem);

extern M537_NODE lbl_1_bss_C;

/* Circular value range used by the dorrie rotation (fn_1_6C20 and friends). */
typedef struct M537Cycle_s {
    f64 lo;     /* 0x00 */
    f64 hi;     /* 0x08 */
    f64 range;  /* 0x10 */
    f64 value;  /* 0x18 */
} M537_CYCLE;

/* std::vector<s16>: capacity, size, data (fn_1_6D0C inserts). */
typedef struct M537VecS16_s {
    u32 cap;
    u32 size;
    s16 *data;
} M537_VECS16;

/* One dorrie: model id (CModel at 0), rotation cycle, tween state, motion list. */
typedef struct M537Dorrie {
    s16 mdl;            /* 0x00 */
    M537_CYCLE cyc;     /* 0x08 */
    f64 from;           /* 0x28 */
    f64 to;             /* 0x30 */
    f64 t;              /* 0x38 */
    f64 dur;            /* 0x40 */
    u8 turn;            /* 0x48 */
    s32 count;          /* 0x4C */
    M537_VECS16 mot;    /* 0x50 */
    s32 timer;          /* 0x5C */
    s16 *shadow;        /* 0x60 */
} M537Dorrie;

/* Lens flare object: list node, vtable, flags, then the four flare sprites. */
typedef struct M537Effect {
    M537_NODE node;     /* 0x00 */
    void **vptr;        /* 0x08 */
    s32 flag;           /* 0x0C */
    ANIMDATA *anim[4];  /* 0x10 */
} M537Effect;

/* Holder of a dorrie owned by the stage (4 bytes on GameCube). */
typedef struct M537DorriePtr {
    M537Dorrie *p;
} M537DorriePtr;

/* Holder of a model owned by the stage. */
typedef struct M537ModelPtr {
    s16 *p;
} M537ModelPtr;

struct M537Camera;
struct M537Stage;
struct M537Water;
extern struct M537Camera *lbl_1_bss_A10;
extern struct M537Stage *lbl_1_bss_A14;

/* Inline list-empty test the original compiled into the destroy-all loops. */
static inline u8 M537ListEmpty(M537_NODE *list)
{
    u8 r = 0;

    if (list->next == list && list->next == list->prev) {
        r = 1;
    }
    return r;
}

#ifdef __cplusplus
}
#endif

#endif
