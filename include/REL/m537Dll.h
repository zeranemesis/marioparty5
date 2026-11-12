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
void *__register_global_object(void *object, void *destructor, void *regmem);

extern M537_NODE lbl_1_bss_C;

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
