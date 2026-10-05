#include "dolphin/types.h"

typedef struct Vec {
    f32 x, y, z;
} Vec;

extern "C" {
void fn_1_10DA8(void *);
void fn_1_11FB4(void *, s32);
void fn_1_111C0(void *, s32);
void fn_1_11524(void *, s32);
void fn_1_11864(void *);
void fn_1_114E4(void *, s32);
void PSVECAdd(Vec *, const Vec *, const Vec *);

void fn_1_10178(void *arg0)
{
    fn_1_11FB4(arg0, 0);
}

void fn_1_10860(void *arg0)
{
    fn_1_10DA8(arg0);
}

void fn_1_10BB0(void *arg0, s32 arg1)
{
    fn_1_111C0(arg0, arg1);
    *(u8 *)((u8 *)arg0 + 0x2C) = 0;
}

#pragma dont_inline on
void fn_1_114E4(void *arg0, s32 arg1)
{
    fn_1_11524(arg0, arg1);
    fn_1_11864(arg0);
}
#pragma dont_inline reset

void fn_1_10888(void *arg0, s32 arg1)
{
    void *ptr = *(void **)((u8 *)arg0 + 0x120);
    fn_1_114E4(ptr, arg1);
}

// NONMATCHING: register allocation differs (r30/r31 usage)
void fn_1_11108(void *arg0)
{
    Vec *vec = (Vec *)arg0;
    PSVECAdd((Vec *)((u8 *)arg0 + 0x18), vec, vec);
    vec->y -= 0.8172208f;
}
}