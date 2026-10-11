// m562dll/sunbomb ' bomb-rain object.
//
// The owner struct is 0x2A8 bytes: a leading word, then five 0x78-byte bomb
// slots starting at 0x04, then the model/motion id table at 0x25E..0x275.
#include "types.h"

typedef s16 HU3DMODELID;
typedef s16 HU3DMOTID;

void Hu3DModelKill(HU3DMODELID modelId);
void Hu3DMotionKill(HU3DMOTID motId);

void fn_1_1F870(void *self);

/* Three floats copied as a unit ' the target moves them word by word. */
typedef struct sunbombVec_s {
    f32 x;
    f32 y;
    f32 z;
} sunbombVec;

/* One bomb slot: four model ids, a spawn flag and a three-float position. */
typedef struct sunbombSlot_s {
    u8 pad00[0x10];
    /* 0x10 */ s32 spawned;
    u8 pad14[0x10];
    /* 0x24 */ HU3DMODELID model;
    /* 0x26 */ HU3DMODELID model2;
    /* 0x28 */ HU3DMODELID model3;
    /* 0x2A */ HU3DMODELID model4;
    u8 pad2C[0x30];
    /* 0x5C */ sunbombVec pos;
    u8 pad68[0x10];
} SUNBOMBSLOT;

typedef struct sunbomb_s {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ SUNBOMBSLOT slots[5];
    /* 0x25C */ s16 active;
} SUNBOMB;

// Releases the four model handles a slot owns, then wipes the whole 0x78 bytes.
#pragma dont_inline on
void fn_1_1F818(SUNBOMBSLOT *self)
{
    Hu3DModelKill(self->model4);
    Hu3DModelKill(self->model3);
    Hu3DModelKill(self->model2);
    Hu3DModelKill(self->model);
    memset(self, 0, 0x78);
}
#pragma dont_inline reset

// Kills the model/motion id table, tears down every slot, then clears the
// whole owner struct.
void fn_1_1F138(SUNBOMB *self)
{
    s32 i;

    Hu3DModelKill(*(HU3DMODELID *)((u8 *)self + 0x26A));
    Hu3DModelKill(*(HU3DMODELID *)((u8 *)self + 0x268));
    Hu3DMotionKill(*(HU3DMOTID *)((u8 *)self + 0x274));
    Hu3DModelKill(*(HU3DMODELID *)((u8 *)self + 0x266));
    Hu3DMotionKill(*(HU3DMOTID *)((u8 *)self + 0x272));
    Hu3DModelKill(*(HU3DMODELID *)((u8 *)self + 0x264));
    Hu3DMotionKill(*(HU3DMOTID *)((u8 *)self + 0x270));
    Hu3DModelKill(*(HU3DMODELID *)((u8 *)self + 0x262));
    Hu3DMotionKill(*(HU3DMOTID *)((u8 *)self + 0x26E));
    Hu3DModelKill(*(HU3DMODELID *)((u8 *)self + 0x260));
    Hu3DMotionKill(*(HU3DMOTID *)((u8 *)self + 0x26C));
    Hu3DModelKill(*(HU3DMODELID *)((u8 *)self + 0x25E));
    for (i = 0; i < 5; i++) {
        fn_1_1F818(&self->slots[i]);
    }
    memset(self, 0, 0x2A8);
}

// Per-frame reset: copies the zero vector into a stack slot and pushes it into
// the owner, tears down all five slots, then clears the active counter.
void fn_1_1F208(SUNBOMB *self)
{
    s32 i;
    s32 zero[3];

    self->unk00 = self->unk00;
    zero[0] = 0.0f;
    zero[1] = 1.0f;
    zero[2] = 0.0f;
    for (i = 0; i < 5; i++) {
        fn_1_1F870(&self->slots[i]);
    }
    self->active = 0;
}

// NONMATCHING: MWCC emits an extra extsh when storing active+1, and the loop
// preheader branch is one instruction longer than the original's.
// Finds the first unspawned slot, seeds it with the given position, flags it
// spawned, bumps the active count and returns it. Returns NULL if all are taken.
SUNBOMBSLOT *fn_1_1F298(SUNBOMB *self, const sunbombVec *pos)
{
    s32 i;
    SUNBOMBSLOT *slot;
    SUNBOMBSLOT *found;

    found = 0;
    for (i = 0; i < 5; i++) {
        slot = &self->slots[i];
        if (slot->spawned != 0) {
            continue;
        }
        slot->pos = *pos;
        slot->spawned = 1;
        self->active = self->active + 1;
        found = slot;
        break;
    }
    return found;
}