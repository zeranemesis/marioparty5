#ifndef _M532DLL_OBJ_H
#define _M532DLL_OBJ_H

// Basic types shared by every unit of m532Dll: list node, model wrapper and the
// base object built by fn_1_EA08 (unk.cpp). Types only, no prototypes.
#include "dolphin/types.h"

#ifdef __cplusplus
extern "C" {
#endif

// Pointer-to-member-function slot. The GameCube build keeps the MWCC layout
// (this-delta, vtable index, function address); on PC the slot is just the
// function pointer, called directly with the object.
#ifdef TARGET_PC
typedef struct M532Ptmf_s {
    s32 (*fn)(void *self);
} M532Ptmf;
#define M532_PTMF_INIT(f) { (s32 (*)(void *))(f) }
#define M532_PTMF_NULL_INIT { 0 }
#define M532_PTMF_NULL M532PtmfNull
static const M532Ptmf M532PtmfNull = { 0 };
#define M532PtmfTest(p) ((p)->fn != 0)
#define M532PtmfCall(obj, p) ((p)->fn(obj))
#else
typedef struct M532Ptmf_s {
    s32 delta;
    s32 vidx;
    void *fn;
} M532Ptmf;
#define M532_PTMF_INIT(f) { 0, -1, (void *)(f) }
#define M532_PTMF_NULL_INIT { 0, -1, 0 }
#define M532_PTMF_NULL __ptmf_null
extern M532Ptmf __ptmf_null;
s32 __ptmf_test(M532Ptmf *p);
u8 __ptmf_scall(void *obj, M532Ptmf *p);
#define M532PtmfTest(p) __ptmf_test(p)
#define M532PtmfCall(obj, p) __ptmf_scall((obj), (p))
#endif



// Intrusive doubly linked list node (also the base of every registered task).
typedef struct M532Node_s {
    struct M532Node_s *next;
    struct M532Node_s *prev;
} M532Node;

// Task registered on the module-wide list; its vtable lives at +8.
struct M532Task_s;
typedef struct M532TaskVtbl_s {
    void *rtti[2];
    struct M532Task_s *(*dtor)(struct M532Task_s *self, s16 flag);
    void (*onFlag)(struct M532Task_s *self, s32 bit);
    void (*update)(struct M532Task_s *self);
} M532TaskVtbl;

typedef struct M532Task_s {
    M532Node link;
    M532TaskVtbl *vtbl;
    u32 flags;
} M532Task;

// Hu3D model wrapper (fn_1_4C54 creates, fn_1_4D74 links, fn_1_50B4 kills).
typedef struct M532Model_s {
    s16 id;
} M532Model;

// Base of the game objects built by fn_1_EA08 (unk.cpp): list node, draw-hook
// model, vtable at +0xC (+0x14 on PC pointers) and the handle table index.
typedef struct M532UnkObjVtbl_s {
    void *rtti[2];
    void *(*dtor)(void *self, s16 flag);
    void (*update)(void *self);
    void (*draw)(void *self, f32 (*mtx)[4]);
    void (*slot3)(void *self);
} M532UnkObjVtbl;

typedef struct M532UnkObj_s {
    M532Node node;
    M532Model hook;
    M532UnkObjVtbl *vtbl;
    s32 handle;
} M532UnkObj;

#ifdef __cplusplus
}
#endif

#endif
