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

void *HuDataSelHeapReadNum(s32 arg0, s32 arg1, s32 arg2);
HU3DMODELID Hu3DModelCreate(s32 arg0);
HU3DMOTID Hu3DJointMotion(HU3DMODELID modelId, s32 arg1);
void Hu3DModelCameraSet(HU3DMOTID modelId, u16 arg1);
void Hu3DModelLayerSet(HU3DMOTID modelId, s32 arg1);
void Hu3DModelAttrSet(HU3DMOTID modelId, u32 arg1);
extern u32 lbl_1_data_B60[];
extern const f32 lbl_1_rodata_4B0;
extern const f32 lbl_1_rodata_580;

// Build one flyer: wipe its slot, load the model and its three joint motions
// from the fixed data numbers, then pin it to the camera bit and layer.
// NONMATCHING: the target materialises every literal >= 0x1000 as lis+addi
// where MWCC emits a single li, and the 5-instruction byte-normalise sequence
// after rand8() is not reproduced here.
#pragma dont_inline on
void fn_1_CB10(BAT *self, u16 arg1, void *arg2)
{
    HU3DMODELID model;

    memset(self, 0, 0x48);
    *(void **)((u8 *)self + 0) = arg2;
    model = Hu3DModelCreate((s32)HuDataSelHeapReadNum(0x3C85, 0x1000, 2));
    *(s16 *)((u8 *)self + 0x0C) = model;
    *(s16 *)((u8 *)self + 0x0E) = Hu3DJointMotion(model, (s32)HuDataSelHeapReadNum(0x3C86, 0x1000, 2));
    *(s16 *)((u8 *)self + 0x10) = Hu3DJointMotion(model, (s32)HuDataSelHeapReadNum(0x3C87, 0x1000, 2));
    *(s16 *)((u8 *)self + 0x12) = Hu3DJointMotion(model, (s32)HuDataSelHeapReadNum(0x3C88, 0x1000, 2));
    Hu3DModelCameraSet(model, (u16)lbl_1_data_B60[*(s32 *)((u8 *)arg2 + 4)]);
    Hu3DModelLayerSet(model, 0);
    Hu3DModelAttrSet(model, 1);
    *(s32 *)((u8 *)self + 0x14) = 0;
}
#pragma dont_inline reset

typedef struct batObj_s {
    BAT bats[8];
} BATOBJ;

void Hu3DMotionKill(HU3DMOTID motId);
void Hu3DModelKill(HU3DMODELID modelId);
void Hu3DModelAttrSet(HU3DMODELID modelId, u32 attr);
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

// True when pos->y sits inside the playable band, i.e. low <= y < high. Both
// bounds come from rodata rather than literals, and the second test compiles
// to fcmpo + cror eq,gt,eq -- only if it is written as ">=" rather than "=="
// or "<=", which is what MWCC otherwise canonicalises.
BOOL fn_1_DE94(BVec *pos)
{
    if (pos->y < lbl_1_rodata_4B0 || pos->y >= lbl_1_rodata_580) {
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