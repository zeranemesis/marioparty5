// m562dll/sunbomb ' bomb-rain object.
//
// The owner struct is 0x2A8 bytes: a link word, then five 0x78-byte bomb slots
// starting at 0x04, then the model/motion id table at 0x25E..0x275.
#include "types.h"

#include "game/hu3d.h"

/* Three floats copied as a unit ' the target moves them word by word. */
typedef struct sunbombVec_s {
    f32 x;
    f32 y;
    f32 z;
} sunbombVec;

/* The camera-bit holder the scene block points at. */
typedef struct sunbombCam_s {
    u8 pad00[4];
    /* 0x04 */ u32 cameraBit;
} SUNBOMBCAM;

/* The sub-block the per-frame update walks from scene + 0x28. */
typedef struct sunbombSub_s {
    u8 pad00[0x14];
    /* 0x14 */ s16 vibId;
    u8 pad16[0x11E];
    /* 0x134 */ void *target;
} SUNBOMBSUB;

/* Only the per-bomb sub-block, the camera-bit holder at 0x1BC and the
   effect list at 0x288C are ever reached out of the scene block. */
typedef struct sunbombScene_s {
    u8 pad000[0x28];
    /* 0x28 */ SUNBOMBSUB sub;
    u8 pad160[0x5C];
    /* 0x1BC */ SUNBOMBCAM cam;
    u8 pad1C0[0x26CC];
    /* 0x288C */ void *fxList;
} SUNBOMBSCENE;

typedef struct sunbombLink_s {
    /* 0x00 */ SUNBOMBSCENE *scene;
} SUNBOMBLINK;

/* One bomb slot: the owning link, the life-cycle state, the frame counter,
   the spawn flag, seven linked model handles and the motion bookkeeping. */
typedef struct sunbomb_s SUNBOMB;

typedef struct sunbombSlot_s {
    /* 0x00 */ SUNBOMB *owner;
    /* 0x04 */ u16 modelCnt;
    u8 pad06[2];
    /* 0x08 */ s16 timer;
    u8 pad0A[2];
    /* 0x0C */ s16 phase;
    u8 pad0E[2];
    /* 0x10 */ s32 spawned;
    u8 pad14[0x10];
    /* 0x24 */ HU3DMODELID model[7];
    u8 pad32[2];
    /* 0x34 */ f32 scale;
    /* 0x38 */ sunbombVec pos;
    /* 0x44 */ sunbombVec vel;
    /* 0x50 */ sunbombVec lastPos;
    /* 0x5C */ sunbombVec spawn;
    /* 0x68 */ f32 fallAcc;
    /* 0x6C */ f32 fallVelX;
    /* 0x70 */ f32 pad70;
    /* 0x74 */ f32 fallVelZ;
} SUNBOMBSLOT;

struct sunbomb_s {
    /* 0x00 */ SUNBOMBLINK *link;
    /* 0x04 */ SUNBOMBSLOT slots[5];
    /* 0x25C */ s16 active;
    /* 0x25E */ HU3DMODELID mdl[7];
    /* 0x26C */ HU3DMOTID mot[5];
    u8 pad276[0x1E];
    /* 0x294 */ s32 unk294;
    /* 0x298 */ sunbombVec attractor;
    /* 0x2A4 */ f32 attractRange;
    /* 0x2A8 */ f32 attractHeight;
};

extern const sunbombVec lbl_1_rodata_8A0;
extern const f32 lbl_1_rodata_8AC;
extern const f32 lbl_1_rodata_8B0;
extern const f32 lbl_1_rodata_8B4;

void fn_1_1F818(SUNBOMBSLOT *self);
void fn_1_1F324(SUNBOMBSLOT *slot, SUNBOMB *owner);
void fn_1_1F870(SUNBOMBSLOT *self);

// Clears the object, links it to its owner, then builds the seven model /
// motion pairs the slots draw with.
void fn_1_1EE3C(SUNBOMB *self, s32 arg)
{
    s32 i;
    SUNBOMBSLOT *slot;
    HU3DMODELID mdl;

    memset(self, 0, 0x2A8);
    self->link = (SUNBOMBLINK *)arg;

    mdl = Hu3DModelCreate(HuDataSelHeapReadNum(0x751B, HU_MEMNUM_OVL, HEAP_MODEL));
    self->mdl[0] = mdl;
    Hu3DModelAttrSet(mdl, 1);
    self->mot[0] = Hu3DJointMotion(
        mdl,
        HuDataSelHeapReadNum(0x7550, HU_MEMNUM_OVL, HEAP_MODEL)
    );
    Hu3DMotionSet(mdl, self->mot[0]);
    Hu3DModelAttrSet(mdl, HU3D_MOTATTR_LOOP);

    mdl = Hu3DModelCreate(HuDataSelHeapReadNum(0x7523, HU_MEMNUM_OVL, HEAP_MODEL));
    self->mdl[1] = mdl;
    Hu3DModelAttrSet(mdl, 1);
    self->mot[1] = Hu3DJointMotion(
        mdl,
        HuDataSelHeapReadNum(0x7558, HU_MEMNUM_OVL, HEAP_MODEL)
    );
    Hu3DMotionSet(mdl, self->mot[1]);
    Hu3DModelAttrSet(mdl, HU3D_MOTATTR_LOOP);

    mdl = Hu3DModelCreate(HuDataSelHeapReadNum(0x7524, HU_MEMNUM_OVL, HEAP_MODEL));
    self->mdl[2] = mdl;
    Hu3DModelAttrSet(mdl, 1);
    self->mot[2] = Hu3DJointMotion(
        mdl,
        HuDataSelHeapReadNum(0x7559, HU_MEMNUM_OVL, HEAP_MODEL)
    );
    Hu3DMotionSet(mdl, self->mot[2]);
    Hu3DModelAttrReset(mdl, HU3D_MOTATTR_LOOP);

    mdl = Hu3DModelCreate(HuDataSelHeapReadNum(0x7525, HU_MEMNUM_OVL, HEAP_MODEL));
    self->mdl[3] = mdl;
    Hu3DModelAttrSet(mdl, 1);
    self->mot[3] = Hu3DJointMotion(
        mdl,
        HuDataSelHeapReadNum(0x755A, HU_MEMNUM_OVL, HEAP_MODEL)
    );
    Hu3DMotionSet(mdl, self->mot[3]);
    Hu3DModelAttrSet(mdl, HU3D_MOTATTR_LOOP);

    mdl = Hu3DModelCreate(HuDataSelHeapReadNum(0x7526, HU_MEMNUM_OVL, HEAP_MODEL));
    self->mdl[4] = mdl;
    Hu3DModelAttrSet(mdl, 1);
    self->mot[4] = Hu3DJointMotion(
        mdl,
        HuDataSelHeapReadNum(0x755B, HU_MEMNUM_OVL, HEAP_MODEL)
    );
    Hu3DMotionSet(mdl, self->mot[4]);
    Hu3DModelAttrSet(mdl, HU3D_MOTATTR_LOOP);

    mdl = Hu3DModelCreate(HuDataSelHeapReadNum(0x7515, HU_MEMNUM_OVL, HEAP_MODEL));
    self->mdl[5] = mdl;
    Hu3DModelAttrSet(mdl, 1);

    mdl = Hu3DModelCreate(HuDataSelHeapReadNum(0x7516, HU_MEMNUM_OVL, HEAP_MODEL));
    self->mdl[6] = mdl;
    Hu3DModelAttrSet(mdl, 1);

    for (i = 0; i < 5; i++) {
        slot = &self->slots[i];
        fn_1_1F324(slot, self);
    }
}

// Kills the model/motion id table, tears down every slot, then clears the
// whole owner struct.
void fn_1_1F138(SUNBOMB *self)
{
    s32 i;
    SUNBOMBSLOT *slot;

    Hu3DModelKill(self->mdl[6]);
    Hu3DModelKill(self->mdl[5]);
    Hu3DMotionKill(self->mot[4]);
    Hu3DModelKill(self->mdl[4]);
    Hu3DMotionKill(self->mot[3]);
    Hu3DModelKill(self->mdl[3]);
    Hu3DMotionKill(self->mot[2]);
    Hu3DModelKill(self->mdl[2]);
    Hu3DMotionKill(self->mot[1]);
    Hu3DModelKill(self->mdl[1]);
    Hu3DMotionKill(self->mot[0]);
    Hu3DModelKill(self->mdl[0]);
    for (i = 0; i < 5; i++) {
        slot = &self->slots[i];
        fn_1_1F818(slot);
    }
    memset(self, 0, 0x2A8);
}

// Per-frame reset: stashes the link word and the neutral vector into a stack
// struct, tears down all five slots, then clears the trailing counter.
void fn_1_1F208(SUNBOMB *self)
{
    s32 i;
    sunbombVec tmp;
    s32 tag;
    SUNBOMBSLOT *slot;

    tag = (s32)self->link;
    tmp = lbl_1_rodata_8A0;
    for (i = 0; i < 5; i++) {
        slot = &self->slots[i];
        fn_1_1F870(slot);
    }
    self->unk294 = 0;
}

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
        self->active++;
        found = slot;
        break;
    }
    return found;
}

// Builds one slot's seven linked models: every one gets a zero rotation, a
// unit scale and the owner's camera bit, then its own layer and attributes.
// The first five also pick up the matching joint motion.
void fn_1_1F324(SUNBOMBSLOT *slot, SUNBOMB *owner)
{
    SUNBOMB *self = owner;
    SUNBOMBLINK *link = self->link;
    SUNBOMBSCENE *scene = link->scene;
    SUNBOMBCAM *cam = &scene->cam;
    HU3DMODELID m;

    memset(slot, 0, 0x78);
    slot->owner = owner;

    m = Hu3DModelLink(self->mdl[0]);
    slot->model[0] = m;
    Hu3DModelRotSet(m, lbl_1_rodata_8AC, lbl_1_rodata_8AC, lbl_1_rodata_8AC);
    Hu3DModelScaleSet(m, lbl_1_rodata_8B0, lbl_1_rodata_8B0, lbl_1_rodata_8B0);
    Hu3DModelCameraSet(m, cam->cameraBit);
    Hu3DModelLayerSet(m, 4);
    Hu3DModelAttrSet(m, 1);
    Hu3DMotionSet(m, self->mot[0]);
    Hu3DModelAttrSet(m, HU3D_MOTATTR_LOOP);

    m = Hu3DModelLink(self->mdl[1]);
    slot->model[1] = m;
    Hu3DModelRotSet(m, lbl_1_rodata_8AC, lbl_1_rodata_8AC, lbl_1_rodata_8AC);
    Hu3DModelScaleSet(m, lbl_1_rodata_8B0, lbl_1_rodata_8B0, lbl_1_rodata_8B0);
    Hu3DModelCameraSet(m, cam->cameraBit);
    Hu3DModelLayerSet(m, 4);
    Hu3DModelAttrSet(m, 1);
    Hu3DMotionSet(m, self->mot[1]);
    Hu3DModelAttrReset(m, HU3D_MOTATTR_LOOP);

    m = Hu3DModelLink(self->mdl[2]);
    slot->model[2] = m;
    Hu3DModelRotSet(m, lbl_1_rodata_8AC, lbl_1_rodata_8AC, lbl_1_rodata_8AC);
    Hu3DModelScaleSet(m, lbl_1_rodata_8B0, lbl_1_rodata_8B0, lbl_1_rodata_8B0);
    Hu3DModelCameraSet(m, cam->cameraBit);
    Hu3DModelLayerSet(m, 4);
    Hu3DModelAttrSet(m, 1);
    Hu3DMotionSet(m, self->mot[2]);
    Hu3DModelAttrReset(m, HU3D_MOTATTR_LOOP);

    m = Hu3DModelLink(self->mdl[3]);
    slot->model[3] = m;
    Hu3DModelRotSet(m, lbl_1_rodata_8AC, lbl_1_rodata_8AC, lbl_1_rodata_8AC);
    Hu3DModelScaleSet(m, lbl_1_rodata_8B0, lbl_1_rodata_8B0, lbl_1_rodata_8B0);
    Hu3DModelCameraSet(m, cam->cameraBit);
    Hu3DModelLayerSet(m, 4);
    Hu3DModelAttrSet(m, 1);
    Hu3DMotionSet(m, self->mot[3]);
    Hu3DModelAttrSet(m, HU3D_MOTATTR_LOOP);

    m = Hu3DModelLink(self->mdl[4]);
    slot->model[4] = m;
    Hu3DModelRotSet(m, lbl_1_rodata_8AC, lbl_1_rodata_8AC, lbl_1_rodata_8AC);
    Hu3DModelScaleSet(m, lbl_1_rodata_8B0, lbl_1_rodata_8B0, lbl_1_rodata_8B0);
    Hu3DModelCameraSet(m, cam->cameraBit);
    Hu3DModelLayerSet(m, 5);
    Hu3DModelAttrSet(m, 1);
    Hu3DMotionSet(m, self->mot[4]);
    Hu3DMotionSpeedSet(m, lbl_1_rodata_8B4);
    Hu3DModelAttrSet(m, HU3D_MOTATTR_LOOP);

    m = Hu3DModelLink(self->mdl[5]);
    slot->model[5] = m;
    Hu3DModelRotSet(m, lbl_1_rodata_8AC, lbl_1_rodata_8AC, lbl_1_rodata_8AC);
    Hu3DModelScaleSet(m, lbl_1_rodata_8B0, lbl_1_rodata_8B0, lbl_1_rodata_8B0);
    Hu3DModelCameraSet(m, cam->cameraBit);
    Hu3DModelLayerSet(m, 0);
    Hu3DModelAttrSet(m, 1);

    m = Hu3DModelLink(self->mdl[6]);
    slot->model[6] = m;
    Hu3DModelRotSet(m, lbl_1_rodata_8AC, lbl_1_rodata_8AC, lbl_1_rodata_8AC);
    Hu3DModelScaleSet(m, lbl_1_rodata_8B0, lbl_1_rodata_8B0, lbl_1_rodata_8B0);
    Hu3DModelCameraSet(m, cam->cameraBit);
    Hu3DModelLayerSet(m, 0);
    Hu3DModelLayerSet(m, 0);
    Hu3DModelAttrSet(m, 1);
}

// Releases the four model handles a slot owns, then wipes the whole 0x78 bytes.
#pragma dont_inline on
void fn_1_1F818(SUNBOMBSLOT *self)
{
    Hu3DModelKill(self->model[3]);
    Hu3DModelKill(self->model[2]);
    Hu3DModelKill(self->model[1]);
    Hu3DModelKill(self->model[0]);
    memset(self, 0, 0x78);
}
#pragma dont_inline reset