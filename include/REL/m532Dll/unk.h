#ifndef _M532DLL_UNK_H
#define _M532DLL_UNK_H

// unk.cpp: a singleton task that owns the list of "unk objects" (sprites and
// effects drawn through a Hu3D hook) and the handle table they register in.
#include "REL/m532Dll/m532.h"

#ifdef __cplusplus
extern "C" {
#endif

// The singleton itself (0x14 bytes on GameCube): a task plus the handle table
// it was created with (owned by the game master).
typedef struct M532Unk_s {
    M532Task base;
    M532HandleMan *handleMan;
} M532Unk;

// One object of the list. Its vtable is at +0xC (after the node and the model).
struct M532UnkObj_s;
typedef struct M532UnkObjVtbl_s {
    void *rtti[2];
    struct M532UnkObj_s *(*dtor)(struct M532UnkObj_s *self, s16 flag);
    void (*update)(struct M532UnkObj_s *self);
    void (*draw)(struct M532UnkObj_s *self, Mtx *mtx);
    void (*unused)(struct M532UnkObj_s *self);
} M532UnkObjVtbl;

typedef struct M532UnkObj_s {
    M532Node link;
    M532Model model;
    M532UnkObjVtbl *vtbl;
    s32 handle;
} M532UnkObj;

M532Unk *fn_1_E54C(M532Unk *self, M532HandleMan *handleMan);
M532UnkObj *fn_1_EA08(M532UnkObj *self);
M532UnkObj *fn_1_EAF0(M532UnkObj *self, s16 flag);
s32 fn_1_EB7C(M532UnkObj *self);
void *fn_1_EB84(s32 handle);

#ifdef __cplusplus
}
#endif

#endif
