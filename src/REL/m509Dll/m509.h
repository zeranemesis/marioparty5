#ifndef M509_H
#define M509_H

#include "game/data.h"
#include "game/hu3d.h"
#include "game/sprite.h"

#ifdef __MWERKS__
#define XX_RODATA __declspec(section ".rodata")
#else
#define XX_RODATA
#endif

/* Header of every m509 object: two words of base data, the vtable at +8 and the flag word at +0xC. */
typedef struct M509Base {
    void *base[2];
    void **vtbl;
    u32 flags;
} M509Base;

/* Pointer to member function in the CodeWarrior layout (adjustment, vtable index or -1, function). */
typedef struct M509Ptmf {
    s32 adj;
    s32 vtblIdx;
    void *func;
} M509Ptmf;

extern "C" {
void fn_1_0(const char *cond, const char *file, s32 line);
void *fn_1_104(s32 size);
void fn_1_21C(void *self);
void fn_1_E48(void *self);
void fn_1_E54(void *self, s32 flag);
void fn_1_F14(void *self, void *base);
extern u8 lbl_1_bss_1C[];
extern u8 lbl_1_data_1C4[];

/* model wrappers (m509.cpp); a model is an s16 handle */
s16 *fn_1_3BBC(s16 *model, s32 dataNum);
void fn_1_3E1C(s16 *model, Vec *pos);
void fn_1_3E64(s16 *model, Vec *rot);
void fn_1_3EAC(s16 *model, Vec *scale);
void fn_1_3F74(s16 *model, s16 layer);
s16 *fn_1_401C(s16 *model, s16 flag);
}

#endif
