// m502dll/bat -- the bat swarm: eight flyers driven from a single 0x248-byte
// owner, plus an on-screen Y clamp helper.
#include "types.h"

typedef s16 HU3DMODELID;
typedef s16 HU3DMOTID;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} BVec;

/* One flyer slot, 0x48 bytes, starting at offset 8. */
typedef struct bat_s {
    u8 pad00[8];
} BAT;

typedef struct batObj_s {
    BAT bats[8];
} BATOBJ;

void Hu3DMotionKill(HU3DMOTID motId);
void Hu3DModelKill(HU3DMODELID modelId);
void Hu3DModelAttrSet(HU3DMODELID modelId, u32 attr);
extern void *fn_1_CB10(void *self, u16 idx, void *arg2);
extern void fn_1_CAC8(s32 arg0);
extern s16 lbl_1_bss_5090;

// Releases one flyer's model and its three motions, then wipes its 0x48 bytes.
#pragma dont_inline on
void fn_1_CE54(BAT *self)
{
    Hu3DModelAttrSet(*(s16 *)((u8 *)self + 0x0C), 0x40000002);
    Hu3DMotionKill(*(s16 *)((u8 *)self + 0x12));
    Hu3DMotionKill(*(s16 *)((u8 *)self + 0x10));
    Hu3DMotionKill(*(s16 *)((u8 *)self + 0x0E));
    Hu3DModelKill(*(s16 *)((u8 *)self + 0x0C));
    memset(self, 0, 0x48);
}
#pragma dont_inline reset

// Arms all eight flyers: zeroes their trigger word and sets the delay counter.
void fn_1_CAC8(s32 arg0)
{
    s32 i;

    for (i = 0; i < 8; i++) {
        *(s32 *)((u8 *)arg0 + i * 0x48 + 0x1C) = 0;
        *(s16 *)((u8 *)arg0 + i * 0x48 + 0x0C) = 3;
    }
}

// Initialises the swarm owner: wipes it, stores the creation argument, spawns
// each of the eight flyers and clears the active count.
void fn_1_C468(BATOBJ *self, s32 arg1)
{
    s32 i;

    memset(self, 0, 0x248);
    *(s32 *)self = arg1;
    for (i = 0; i < 8; i++) {
        fn_1_CB10((void *)((u8 *)self + i * 0x48 + 8), (u16)i, self);
    }
    lbl_1_bss_5090 = 0;
}

// NONMATCHING: the original emits a two-sided range test (blt to a shared
// "return 0" then a cror-based ">" check); the early-out shape MWCC builds
// from this C collapses the first branch into a single bge over the shared
// return.
// True when pos->y sits inside the playable band (0 .. 480).
BOOL fn_1_DE94(BVec *pos)
{
    if (pos->y < 0.0f) {
        return 0;
    }
    if (pos->y > 480.0f) {
        return 0;
    }
    return 1;
}

// Tears down each of the eight flyers, then wipes the whole owner.
void fn_1_C4F0(BATOBJ *self)
{
    s32 i;

    for (i = 0; i < 8; i++) {
        fn_1_CE54((void *)((u8 *)self + i * 0x48 + 8));
    }
    memset(self, 0, 0x248);
}