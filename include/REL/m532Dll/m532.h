#ifndef _M532DLL_M532_H
#define _M532DLL_M532_H

// Types and entry points of m532.cpp (the module's runtime layer) that other
// units of m532Dll need. Every function is extern "C" under its fn_1_ name.
#ifdef __cplusplus
extern "C" {
#endif

#include "dolphin/types.h"
#include "dolphin/mtx.h"
#include "game/hu3d.h"

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


// Camera pose: eye position, up vector, look-at target.
typedef struct M532Cam_s {
    Vec pos;
    Vec up;
    Vec target;
} M532Cam;

// Stored pose: position plus orientation quaternion.
typedef struct M532Pose_s {
    Vec pos;
    Quaternion rot;
} M532Pose;


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


// A wrapper that owns a Hu3D model / MgTimer handle.
typedef struct M532Model_s {
    s16 id;
} M532Model;


void fn_1_1C10(void *ptr);
void fn_1_1D1C(const char *expr, const char *file, s32 line);
void fn_1_219C(M532Cam *self, M532Pose *pose);
void fn_1_24E8(M532Cam *self, u16 camBit);
void fn_1_2528(M532Cam *self, u16 camBit);
void fn_1_32B0(M532Node *self);
void *fn_1_32BC(M532Node *self, s16 flag);
void fn_1_3328(M532Node *self);
void fn_1_337C(M532Node *self, M532Node *other);
void fn_1_33AC(M532Node *self, M532Node *other);
void fn_1_500C(M532Model *self, s16 layer);
M532Model *fn_1_4C9C(M532Model *self, HU3DMODELHOOK hook);
void fn_1_507C(M532Model *self, void *data);
M532Model *fn_1_50B4(M532Model *self, s16 flag);

// Module-wide list of registered tasks (head node).
extern M532Node lbl_1_bss_24;

// handle.cpp: generation-tagged handle table. 16 bytes per node on GameCube.
#define M532_HANDLE_INDEX(h) ((u32)(h) >> 12)
#define M532_HANDLE_GEN(h) ((u32)(h) & 0xFFF)
typedef struct M532HandleNode_s {
    struct M532HandleNode_s *next;
    struct M532HandleNode_s *prev;
    void *ptr;
    u32 word; /* index << 12 | generation; also the handle value */
} M532HandleNode;

typedef struct M532HandleMan_s {
    M532HandleNode *nodes;
    M532HandleNode *used;
    u32 count;
} M532HandleMan;

M532HandleMan *fn_1_2DAC(M532HandleMan *self, u32 n);
M532HandleMan *fn_1_2FB0(M532HandleMan *self, s16 flag);
s32 fn_1_3020(M532HandleMan *self, void *ptr);
void *fn_1_30B0(M532HandleMan *self, s32 handle);
void *fn_1_3174(M532HandleMan *self, s32 handle);

#ifdef __cplusplus
}
#endif

#endif
