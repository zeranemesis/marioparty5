#ifndef M516DLL_TYPES_H
#define M516DLL_TYPES_H

#include "types.h"
#include "dolphin/mtx.h"

/* Collision/hit record registered with the actor manager (actor.c).
   Pointer members are real pointers so the layout is correct on 64-bit hosts;
   on GameCube the offsets are the original ones (size 0x34). */
typedef union {
    f32 f;
    s32 i;
} ACTFIELD;

typedef struct ACTHIT {
    u32 flags;                    /* 0x00 bit 0: active */
    s32 kind;                     /* 0x04 1 = actor, 2 = tile */
    s32 id;                       /* 0x08 */
    s32 disabled;                 /* 0x0C */
    void *arg;                    /* 0x10 owner object */
    void (*callback)(void *arg);  /* 0x14 */
    Vec *pos;                     /* 0x18 */
    Vec *vel;                     /* 0x1C */
    f32 f20;                      /* 0x20 */
    f32 f24;                      /* 0x24 */
    ACTFIELD f28;                 /* 0x28 */
    ACTFIELD f2C;                 /* 0x2C */
    union {
        s32 id;
        void *other;
    } f30;                        /* 0x30 */
} ACTHIT;

/* Tile hit record (stage.c): an ACTHIT followed by touch state. Size 0x40. */
typedef struct ACTHITEX {
    ACTHIT base;
    s32 f34;                      /* 0x34 index of the toucher */
    s32 f38;                      /* 0x38 touched flag */
    void *hitObj;                 /* 0x3C touching object */
} ACTHITEX;

/* Expanding ring drawn by effect.c. Size 0xC. */
typedef struct EFFRING {
    s32 active;
    f32 size;
    Vec *pos;
} EFFRING;

extern ACTHIT *fn_1_8E78(s32 idx);
extern s32 fn_1_8D88(ACTHIT *hit);
extern s32 fn_1_8E00(ACTHIT *hit);
extern void fn_1_796C(EFFRING *ring);

#endif
