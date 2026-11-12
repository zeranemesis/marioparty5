#ifndef _M536DLL_H
#define _M536DLL_H

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
#define M536_RODATA __declspec(section ".rodata")
#else
#define M536_RODATA
#endif

/* Intrusive list node every m536 work object starts with (see fn_1_1264/1330). */
typedef struct M536Node_s {
    struct M536Node_s *next;
    struct M536Node_s *prev;
} M536_NODE;

/* MWCC pointer-to-member-function triple. */
typedef struct M536PTMF_s {
    s32 thisDelta;
    s32 vOffset;
    void *fn;
} M536_PTMF;

s32 __ptmf_test(M536_PTMF *ptmf);
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
typedef struct M536EffObj_s {
    M536_NODE node;
    s16 mdl;
    s32 unkC;
} M536_EFFOBJ;

f32 fn_1_4D1C(void);
void fn_1_12DC(void *node);
M536_EFFOBJ *fn_1_7B3C(M536_EFFOBJ *self, Vec *pos, f32 rotY);
M536_EFFOBJ *fn_1_8098(M536_EFFOBJ *self, Vec *pos);
s16 *fn_1_2D18(s16 *out, s16 charNo, s16 flag);
void fn_1_2EF4(s16 *modelId, Vec *pos);
void fn_1_2F3C(s16 *modelId, Vec *rot);
int fn_1_234(void *file, const char *fmt, ...);
s32 fn_1_4AE0(void);
s32 fn_1_1F30(void);
void *__register_global_object(void *object, void *destructor, void *regmem);

extern M536_NODE lbl_1_bss_C;

/* Inline list-empty test the original compiled into the destroy-all loops. */
static inline u8 M536ListEmpty(M536_NODE *list)
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
