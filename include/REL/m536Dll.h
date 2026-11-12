#ifndef _M536DLL_H
#define _M536DLL_H

#include "dolphin.h"
#include "humath.h"
#include "game/hu3d.h"
#include "game/data.h"
#include "game/memory.h"
#include "game/audio.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/process.h"
#include "game/gamework.h"
#include "game/hsfex.h"
#include "game/sprite.h"

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
void fn_1_30F4(s16 *modelId, s16 flag);
void *__register_global_object(void *object, void *destructor, void *regmem);

extern M536_NODE lbl_1_bss_C;

#ifdef __cplusplus
}
#endif

#endif
