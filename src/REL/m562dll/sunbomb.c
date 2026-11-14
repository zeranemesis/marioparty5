// m562dll/sunbomb - the bomb-rain object: five bomb slots that spawn above
// Bowser's hand, arc to their landing spot, explode and burn out.
#include <humath.h>
#include <math.h>

#include "REL/m562dll.h"
#include "datadir_enum.h"
#include "game/hsfex.h"

typedef KbBombSlot SUNBOMBSLOT;
typedef KbBombMgr SUNBOMB;

/* The two vectors the original keeps in .rodata and copies onto the stack. */
void fn_1_1F324(SUNBOMBSLOT *slot, SUNBOMB *owner);
void fn_1_1F818(SUNBOMBSLOT *self);
void fn_1_1F870(SUNBOMBSLOT *self);

static const Vec kZeroUnitY = {0.0f, 1.0f, 0.0f};
static const Vec kSpawnOffset = {0.0f, 50.0f, 100.0f};

#define CLAMP01(v) ((v) > 1.0f ? 1.0f : ((v) < 0.0f ? 0.0f : (v)))

// Clears the object, links it to its owner, then builds the seven model /
// motion pairs the slots draw with.
void fn_1_1EE3C(KbBombMgr *mgr, KoopaBoss *link)
{
    SUNBOMB *self = (SUNBOMB *)mgr;
    s32 i;
    SUNBOMBSLOT *slot;
    HU3DMODELID mdl;

    memset(self, 0, sizeof(SUNBOMB));
    self->boss = link;

    mdl = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m562, 0x1B), HU_MEMNUM_OVL, HEAP_MODEL));
    self->mdl[0] = mdl;
    Hu3DModelAttrSet(mdl, 1);
    self->mot[0] = Hu3DJointMotion(
        mdl,
        HuDataSelHeapReadNum(DATANUM(DATA_m562, 0x50), HU_MEMNUM_OVL, HEAP_MODEL)
    );
    Hu3DMotionSet(mdl, self->mot[0]);
    Hu3DModelAttrSet(mdl, HU3D_MOTATTR_LOOP);

    mdl = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m562, 0x23), HU_MEMNUM_OVL, HEAP_MODEL));
    self->mdl[1] = mdl;
    Hu3DModelAttrSet(mdl, 1);
    self->mot[1] = Hu3DJointMotion(
        mdl,
        HuDataSelHeapReadNum(DATANUM(DATA_m562, 0x58), HU_MEMNUM_OVL, HEAP_MODEL)
    );
    Hu3DMotionSet(mdl, self->mot[1]);
    Hu3DModelAttrSet(mdl, HU3D_MOTATTR_LOOP);

    mdl = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m562, 0x24), HU_MEMNUM_OVL, HEAP_MODEL));
    self->mdl[2] = mdl;
    Hu3DModelAttrSet(mdl, 1);
    self->mot[2] = Hu3DJointMotion(
        mdl,
        HuDataSelHeapReadNum(DATANUM(DATA_m562, 0x59), HU_MEMNUM_OVL, HEAP_MODEL)
    );
    Hu3DMotionSet(mdl, self->mot[2]);
    Hu3DModelAttrReset(mdl, HU3D_MOTATTR_LOOP);

    mdl = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m562, 0x25), HU_MEMNUM_OVL, HEAP_MODEL));
    self->mdl[3] = mdl;
    Hu3DModelAttrSet(mdl, 1);
    self->mot[3] = Hu3DJointMotion(
        mdl,
        HuDataSelHeapReadNum(DATANUM(DATA_m562, 0x5A), HU_MEMNUM_OVL, HEAP_MODEL)
    );
    Hu3DMotionSet(mdl, self->mot[3]);
    Hu3DModelAttrSet(mdl, HU3D_MOTATTR_LOOP);

    mdl = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m562, 0x26), HU_MEMNUM_OVL, HEAP_MODEL));
    self->mdl[4] = mdl;
    Hu3DModelAttrSet(mdl, 1);
    self->mot[4] = Hu3DJointMotion(
        mdl,
        HuDataSelHeapReadNum(DATANUM(DATA_m562, 0x5B), HU_MEMNUM_OVL, HEAP_MODEL)
    );
    Hu3DMotionSet(mdl, self->mot[4]);
    Hu3DModelAttrSet(mdl, HU3D_MOTATTR_LOOP);

    mdl = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m562, 0x15), HU_MEMNUM_OVL, HEAP_MODEL));
    self->mdl[5] = mdl;
    Hu3DModelAttrSet(mdl, 1);

    mdl = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m562, 0x16), HU_MEMNUM_OVL, HEAP_MODEL));
    self->mdl[6] = mdl;
    Hu3DModelAttrSet(mdl, 1);

    for (i = 0; i < 5; i++) {
        slot = &self->slot[i];
        fn_1_1F324(slot, self);
    }
}

// Kills the model/motion id table, tears down every slot, then clears the
// whole owner struct.
void fn_1_1F138(KbBombMgr *mgr)
{
    SUNBOMB *self = (SUNBOMB *)mgr;
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
        slot = &self->slot[i];
        fn_1_1F818(slot);
    }
    memset(self, 0, sizeof(SUNBOMB));
}

// Per-frame reset: stashes the link word and the neutral vector into a stack
// struct, tears down all five slots, then clears the trailing counter.
void fn_1_1F208(KbBombMgr *mgr)
{
    SUNBOMB *self = (SUNBOMB *)mgr;
    s32 i;
    Vec tmp;
    KoopaBoss *tag;
    SUNBOMBSLOT *slot;

    tag = self->boss;
    tmp = kZeroUnitY;
    for (i = 0; i < 5; i++) {
        slot = &self->slot[i];
        fn_1_1F870(slot);
    }
    self->attract = 0;
}

// Finds the first unspawned slot, seeds it with the given position, flags it
// spawned, bumps the active count and returns it. Returns NULL if all are taken.
KbBombSlot *fn_1_1F298(KbBombMgr *mgr, Vec *pos)
{
    SUNBOMB *self = (SUNBOMB *)mgr;
    s32 i;
    SUNBOMBSLOT *slot;
    SUNBOMBSLOT *found;

    found = 0;
    for (i = 0; i < 5; i++) {
        slot = &self->slot[i];
        if (slot->active != 0) {
            continue;
        }
        slot->spawn = *pos;
        slot->active = 1;
        self->count++;
        found = slot;
        break;
    }
    return (KbBombSlot *)found;
}

// Builds one slot's seven linked models: every one gets a zero rotation, a
// unit scale and the owner's camera bit, then its own layer and attributes.
// The first five also pick up the matching joint motion.
void fn_1_1F324(SUNBOMBSLOT *slot, SUNBOMB *owner)
{
    SUNBOMB *self = owner;
    KoopaBoss *link = self->boss;
    KpHost *scene = link->host;
    CAMERA *cam = &scene->camera[0];
    HU3DMODELID m;

    memset(slot, 0, sizeof(SUNBOMBSLOT));
    slot->mgr = owner;

    m = Hu3DModelLink(self->mdl[0]);
    slot->model[0] = m;
    Hu3DModelRotSet(m, 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(m, 1.0f, 1.0f, 1.0f);
    Hu3DModelCameraSet(m, cam->camMask);
    Hu3DModelLayerSet(m, 4);
    Hu3DModelAttrSet(m, 1);
    Hu3DMotionSet(m, self->mot[0]);
    Hu3DModelAttrSet(m, HU3D_MOTATTR_LOOP);

    m = Hu3DModelLink(self->mdl[1]);
    slot->model[1] = m;
    Hu3DModelRotSet(m, 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(m, 1.0f, 1.0f, 1.0f);
    Hu3DModelCameraSet(m, cam->camMask);
    Hu3DModelLayerSet(m, 4);
    Hu3DModelAttrSet(m, 1);
    Hu3DMotionSet(m, self->mot[1]);
    Hu3DModelAttrReset(m, HU3D_MOTATTR_LOOP);

    m = Hu3DModelLink(self->mdl[2]);
    slot->model[2] = m;
    Hu3DModelRotSet(m, 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(m, 1.0f, 1.0f, 1.0f);
    Hu3DModelCameraSet(m, cam->camMask);
    Hu3DModelLayerSet(m, 4);
    Hu3DModelAttrSet(m, 1);
    Hu3DMotionSet(m, self->mot[2]);
    Hu3DModelAttrReset(m, HU3D_MOTATTR_LOOP);

    m = Hu3DModelLink(self->mdl[3]);
    slot->model[3] = m;
    Hu3DModelRotSet(m, 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(m, 1.0f, 1.0f, 1.0f);
    Hu3DModelCameraSet(m, cam->camMask);
    Hu3DModelLayerSet(m, 4);
    Hu3DModelAttrSet(m, 1);
    Hu3DMotionSet(m, self->mot[3]);
    Hu3DModelAttrSet(m, HU3D_MOTATTR_LOOP);

    m = Hu3DModelLink(self->mdl[4]);
    slot->model[4] = m;
    Hu3DModelRotSet(m, 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(m, 1.0f, 1.0f, 1.0f);
    Hu3DModelCameraSet(m, cam->camMask);
    Hu3DModelLayerSet(m, 5);
    Hu3DModelAttrSet(m, 1);
    Hu3DMotionSet(m, self->mot[4]);
    Hu3DMotionSpeedSet(m, 8.96f);
    Hu3DModelAttrSet(m, HU3D_MOTATTR_LOOP);

    m = Hu3DModelLink(self->mdl[5]);
    slot->model[5] = m;
    Hu3DModelRotSet(m, 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(m, 1.0f, 1.0f, 1.0f);
    Hu3DModelCameraSet(m, cam->camMask);
    Hu3DModelLayerSet(m, 0);
    Hu3DModelAttrSet(m, 1);

    m = Hu3DModelLink(self->mdl[6]);
    slot->model[6] = m;
    Hu3DModelRotSet(m, 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(m, 1.0f, 1.0f, 1.0f);
    Hu3DModelCameraSet(m, cam->camMask);
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
    memset(self, 0, sizeof(SUNBOMBSLOT));
}
#pragma dont_inline reset
// One bomb's per-frame state machine (0 idle, 1 wait for spawn, 2 grow in
// Bowser's hand, 3 arc to the landing spot, 4 land + shockwave, 5 settled,
// 6 explode, 7 burn, 8 shrink away, 9 blink out, 10 free the slot), followed
// by the attractor push, the fall/bounce physics, the arena clamp, and the
// model placement and effect registration.
void fn_1_1F870(SUNBOMBSLOT *self)
{
    Vec sp50;
    Vec sp44;
    Vec off1;
    Vec off;
    SUNBOMB *owner = self->mgr;
    KoopaBoss *link = owner->boss;
    KpHost *scene = link->host;
    M562Player *sub = (M562Player *)&scene->player;
    CAMERA *cam = &scene->camera[0];
    f32 yOffset;
    s32 pushed;
    f32 t;
    f32 s;
    f32 floorY;
    f32 dist;
    f32 dx;
    f32 dz;

    off1 = kSpawnOffset;
    off = off1;

    if (self->active == 0 && self->state > 1) {
        Hu3DModelAttrSet(self->model[0], 1);
        Hu3DModelAttrSet(self->model[1], 1);
        Hu3DModelAttrSet(self->model[2], 1);
        Hu3DModelAttrSet(self->model[3], 1);
        Hu3DModelAttrSet(self->model[4], 1);
        Hu3DModelAttrSet(self->model[5], 1);
        Hu3DModelAttrSet(self->model[6], 1);
        self->state = 0;
    }

    yOffset = 0.0f;
    switch (self->state) {
    case 0:
        self->phase = 0;
        Hu3DModelAttrSet(self->model[0], 1);
        Hu3DModelAttrSet(self->model[1], 1);
        Hu3DModelAttrSet(self->model[2], 1);
        Hu3DModelAttrSet(self->model[3], 1);
        Hu3DModelAttrSet(self->model[4], 1);
        Hu3DModelAttrSet(self->model[5], 1);
        Hu3DModelAttrSet(self->model[6], 1);
        self->mode = 0;
        self->radius = 120.0f;
        self->flag = 0;
        self->state = 1;
        /* fallthrough */
    case 1:
        if (self->active != 0) {
            self->scale = 0.0f;
            Hu3DModelScaleSet(self->model[0], 0.0f, 0.0f, 0.0f);
            Hu3DModelAttrReset(self->model[0], 1);
            Hu3DModelScaleSet(self->model[5], 0.0f, 0.0f, 0.0f);
            Hu3DModelAttrReset(self->model[5], 1);
            Hu3DMtxTransGet(fn_1_1010C(link), &sp44);
            PSVECAdd(&sp44, &off, &self->pos);
            self->mode = 0;
            self->unk8 = 0;
            self->state = 2;
        }
        break;

    case 2:
        Hu3DMtxTransGet(fn_1_1010C(link), &sp44);
        PSVECAdd(&sp44, &off, &self->pos);
        t = self->unk8 / 30.0f;
        t = CLAMP01(t);
        s = 2.0f * t;
        self->scale = s;
        Hu3DModelScaleSet(self->model[0], s, s, s);
        s = s * 1.5f;
        Hu3DModelScaleSet(self->model[5], s, s, s);
        self->unk8++;
        if (self->mode != 0) {
            self->mode = 0;
            self->startPos.x = self->pos.x;
            self->startPos.y = self->pos.y;
            self->startPos.z = self->pos.z;
            self->unk8 = 0;
            self->state = 3;
        }
        break;

    case 3:
        t = self->unk8 / 40.0f;
        if (t < 0.4f) {
            s = HuSin(90.0f * (t / 0.4f));
            self->pos.y = self->startPos.y + 250.0f * s;
        } else {
            s = HuSin(90.0f + 90.0f * ((t - 0.4f) / 0.6f));
            self->pos.y = self->spawn.y + s * (250.0f + (self->startPos.y - self->spawn.y));
        }
        self->pos.x = self->startPos.x + t * (self->spawn.x - self->startPos.x);
        self->pos.z = self->startPos.z + t * (self->spawn.z - self->startPos.z);
        self->unk8++;
        if (self->unk8 >= 40) {
            self->pos.x = self->spawn.x;
            self->pos.y = self->spawn.y;
            self->pos.z = self->spawn.z;
            self->pos.y = 0.0f;
            Hu3DMotionTimeSet(self->model[1], 0.0f);
            Hu3DMotionSpeedSet(self->model[1], 0.5f);
            Hu3DModelAttrReset(self->model[1], 1);
            self->unk8 = 0;
            self->state = 4;
            fn_1_934(0x8AB, &self->pos);
            Hu3DModelScaleSet(self->model[5], 1.0f, 1.0f, 1.0f);
            fn_1_21B68(cam, 1, 0x32, 40.0f, 0.6f, 90.0f);
            omVibrate(sub->playerNo, 10, 12, 0);
        }
        break;

    case 4:
        Hu3DMotionTimeSet(self->model[2], 0.0f);
        if (self->unk8 < 10) {
            t = self->unk8 / 10.0f;
            t = CLAMP01(t);
            s = 2.0f * (0.3f + 0.7f * (1.0f - t));
            self->scale = s;
            Hu3DModelScaleSet(self->model[0], s, s, s);
        } else if (self->unk8 == 10) {
            Hu3DModelAttrSet(self->model[0], 1);
        }
        self->radius = 100.0f + 100.0f * (self->unk8 / 60.0f);
        self->unk8++;
        if (self->unk8 == 10) {
            Hu3DModelScaleSet(self->model[2], 1.0f, 1.0f, 1.0f);
            Hu3DMotionTimeSet(self->model[2], 0.0f);
            Hu3DModelAttrReset(self->model[2], 1);
        } else if (self->unk8 > 10 && self->unk8 < 40) {
            t = (self->unk8 - 10) / 30.0f;
            t = CLAMP01(t);
            yOffset = 300.0 * HuSin(180.0f * t);
        } else if (self->unk8 >= 60) {
            self->phase = 1;
            Hu3DModelAttrSet(self->model[0], 1);
            Hu3DModelAttrSet(self->model[1], 1);
            self->unk8 = 0;
            self->state = 5;
            self->radius = 40.0f;
        }
        break;

    case 6:
        if (self->unk8 == 0) {
            fn_1_934(0x8AE, &self->pos);
        }
        self->phase = 2;
        s = 2.5 * HuSin(180.0f * (self->unk8 / 40.0f));
        Hu3DModelScaleSet(self->model[4], s, s, s);
        Hu3DModelAttrReset(self->model[4], 1);
        if (self->unk8 == 20) {
            Hu3DModelAttrSet(self->model[2], 1);
            Hu3DModelAttrReset(self->model[3], 1);
            Hu3DModelScaleSet(self->model[3], 1.5f, 1.5f, 1.5f);
            Hu3DModelScaleSet(self->model[6], 2.0f, 2.0f, 2.0f);
            Hu3DModelAttrReset(self->model[6], 1);
        }
        self->unk8++;
        if (self->unk8 >= 40) {
            Hu3DModelAttrSet(self->model[5], 1);
            Hu3DModelAttrSet(self->model[4], 1);
            self->unk8 = 0;
            self->state = 7;
        }
        break;

    case 7:
        t = (self->pos.y - 50.0f) / 150.0f;
        t = CLAMP01(t);
        s = 2.0f + 2.0f * t;
        Hu3DModelScaleSet(self->model[6], s, s, s);
        break;

    case 8:
        t = self->unk8 / 10.0f;
        t = CLAMP01(t);
        s = self->scale * (1.0f - t);
        Hu3DModelScaleSet(self->model[0], s, s, s);
        s = s * 1.5f;
        Hu3DModelScaleSet(self->model[5], s, s, s);
        self->unk8++;
        if (self->unk8 >= 10) {
            Hu3DModelAttrSet(self->model[0], 1);
            Hu3DModelAttrSet(self->model[1], 1);
            Hu3DModelAttrSet(self->model[2], 1);
            Hu3DModelAttrSet(self->model[5], 1);
            Hu3DModelAttrSet(self->model[3], 1);
            Hu3DModelAttrSet(self->model[6], 1);
            Hu3DModelAttrSet(self->model[4], 1);
            self->unk8 = 0;
            self->state = 10;
        }
        break;

    case 9:
        switch (self->phase) {
        case 0:
        case 1:
            if (self->unk8 & 1) {
                Hu3DModelAttrSet(self->model[2], 1);
                Hu3DModelAttrSet(self->model[5], 1);
            } else {
                Hu3DModelAttrReset(self->model[2], 1);
                Hu3DModelAttrReset(self->model[5], 1);
            }
            break;
        case 2:
            if (self->unk8 & 1) {
                Hu3DModelAttrSet(self->model[3], 1);
                Hu3DModelAttrSet(self->model[6], 1);
            } else {
                Hu3DModelAttrReset(self->model[3], 1);
                Hu3DModelAttrReset(self->model[6], 1);
            }
            break;
        }
        self->unk8++;
        if (self->unk8 >= 60) {
            Hu3DModelAttrSet(self->model[0], 1);
            Hu3DModelAttrSet(self->model[1], 1);
            Hu3DModelAttrSet(self->model[2], 1);
            Hu3DModelAttrSet(self->model[5], 1);
            Hu3DModelAttrSet(self->model[3], 1);
            Hu3DModelAttrSet(self->model[6], 1);
            Hu3DModelAttrSet(self->model[4], 1);
            self->unk8 = 0;
            self->state = 10;
        }
        break;

    case 10:
        self->unk8 = 0;
        self->active = 0;
        owner->count--;
        break;
    }

    pushed = 0;
    if (owner->attract != 0 && self != sub->target) {
        PSVECSubtract(&self->pos, &owner->attractPos, &sp50);
        sp50.y = 0.0f;
        dist = PSVECMag(&sp50);
        if (dist < owner->attractRange && dist > owner->attractRange - 100.0f) {
            if (sp50.x != 0.0f || sp50.z != 0.0f) {
                PSVECNormalize(&sp50, &sp50);
            } else {
                sp50.z = 1.0f;
            }
            PSVECScale(&sp50, &sp50, owner->attractRange - dist);
            PSVECAdd(&self->pos, &sp50, &sp44);
            if (sp44.z < owner->attractPos.z) {
                sp44.z += 0.1f * (owner->attractPos.z - sp44.z);
            }
            self->pos.x = sp44.x;
            self->pos.y = sp44.y;
            self->pos.z = sp44.z;
            self->flag = 1;
            pushed = 1;
        }
    }

    if (self != sub->target) {
        if (self->state >= 5) {
            if (self->pos.z < -360.0f) {
                floorY = 55.0f;
            } else {
                floorY = 0.0f;
            }
            if (self->pos.y > floorY) {
                self->fallAcc -= 1.5f;
                self->pos.x += self->fallVelX;
                self->pos.z += self->fallVelZ;
                self->pos.y += self->fallAcc;
                if (self->pos.y <= 0.0f) {
                    self->fallVelZ = 0.0f;
                    self->fallVelX = 0.0f;
                    self->pos.y = floorY;
                    self->fallAcc = 0.0f;
                }
            }
        }
        if (self->state >= 4 && pushed == 0) {
            if (self->pos.x > 1125.0f) {
                dx = 1125.0f;
            } else {
                dx = (self->pos.x < -1125.0f) ? -1125.0f : self->pos.x;
            }
            self->pos.x = dx;
            if (self->pos.z > 600.0f) {
                dz = 600.0f;
            } else {
                dz = (self->pos.z < self->pos.z) ? self->pos.z : self->pos.z;
            }
            self->pos.z = dz;
            if (self->pos.z < -360.0f && self->pos.y < 55.0f) {
                f32 dzAbs = -360.0f - self->pos.z;
                f32 dyAbs = 55.0f - self->pos.y;
                dzAbs = (dzAbs >= 0.0f) ? dzAbs : -dzAbs;
                dyAbs = (dyAbs >= 0.0f) ? dyAbs : -dyAbs;
                if (dyAbs < dzAbs) {
                    self->pos.y = 55.0f;
                    if (self->state <= 7) {
                        self->unk8 = 0;
                        self->state = 9;
                    }
                } else {
                    self->pos.z = -360.0f;
                }
            }
        }
    }

    PSVECAdd(&self->pos, &self->vel, &sp44);
    Hu3DModelPosSet(self->model[0], sp44.x, sp44.y, sp44.z);
    Hu3DModelPosSet(self->model[1], sp44.x, sp44.y, sp44.z);
    Hu3DModelPosSet(self->model[2], sp44.x, sp44.y + yOffset, sp44.z);
    Hu3DModelPosSet(self->model[3], sp44.x, sp44.y, sp44.z);
    Hu3DModelPosSet(self->model[4], sp44.x, sp44.y, sp44.z);
    Hu3DModelPosSet(self->model[5], sp44.x, 0.0f, sp44.z);
    Hu3DModelPosSet(self->model[6], sp44.x, 0.0f, sp44.z);
    if (self->active != 0 && self != sub->target && self->state >= 3 && self->state < 9) {
        fn_1_2CE8C(KP_HITB(scene), 5, 0, &self->pos, self, 300.0f);
    }
}
