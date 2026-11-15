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

// M532UnkObj / M532UnkObjVtbl: see obj.h.

M532Unk *fn_1_E54C(M532Unk *self, M532HandleMan *handleMan);
M532UnkObj *fn_1_EA08(M532UnkObj *self);
M532UnkObj *fn_1_EAF0(M532UnkObj *self, s16 flag);
// Handles travel as pointer-sized values (they are 32-bit table handles).
void *fn_1_EB7C(M532UnkObj *self);
void *fn_1_EB84(void *handle);

#ifdef __cplusplus
}
#endif

#endif
