// m562dll/koopaboss - the fire-breathing boss form of Bowser: model/motion
// setup, the attack state machine (bomb spit / mini mecha / orb) and the
// collision callbacks between player, bombs and the boss.
#include "REL/m562dll.h"
#include "game/frand.h"
#include "game/hsfex.h"
#include "humath.h"
#include <string.h>

/* returns a float in the original (see the note in game/hu3d.h) */
float Hu3DMotionMaxTimeGet(HU3DMODELID modelId);

void fn_1_10520(M562Player *player);
void fn_1_10588(KbBombSlot *slot, EffectEntry *entry);
void fn_1_10784(KbBombSlot *a, KbBombSlot *b);
void fn_1_10918(KbBombSlot *slot, EffectEntry *entry);
void fn_1_10AF0(M562Player *player, KbBombSlot *slot);
void fn_1_10FEC(M562Player *player);
void fn_1_11634(KoopaBoss *self);
s32 fn_1_1183C(KoopaBoss *self);
s32 fn_1_12280(KoopaBoss *self);
s32 fn_1_12A38(KoopaBoss *self);
void fn_1_12BA8(KoopaBoss *self);
void fn_1_13A24(KoopaBoss *self);
s32 fn_1_13C1C(M562Player *player, Vec *center, f32 radius);

/* .rodata vectors (initial velocity of a spat bomb) */
const Vec lbl_1_rodata_4F0 = { 0.0f, 50.0f, 150.0f };
const Vec lbl_1_rodata_4FC = { 0.0f, 50.0f, 150.0f };
const Vec lbl_1_rodata_590 = { 0.0f, 50.0f, 100.0f };
/* offset of the orb above the left hand */
const Vec lbl_1_rodata_59C = { 0.0f, 50.0f, 100.0f };

/* .data */
/* the 19 joint-motion ids of the boss */
u32 lbl_1_data_880[19] = {
    0x0075003D, 0x0075003E, 0x0075003F, 0x00750040, 0x00750041,
    0x00750042, 0x00750043, 0x00750044, 0x00750045, 0x00750046,
    0x00750047, 0x00750048, 0x00750049, 0x0075004A, 0x0075004B,
    0x0075004C, 0x0075004D, 0x0075004E, 0x0075004F
};

/* start position of the boss model */
Vec lbl_1_data_8CC = { 0.0f, -500.0f, -1250.0f };

/* centre of the arena the orb falls into */
Vec lbl_1_data_8D8 = { 0.0f, 0.0f, -100.0f };

/* node names of the boss model */
char lbl_1_data_8E4[] = "g000mSP-itemhook_R";
char lbl_1_data_8F7[] = "g000mSP-itemhook_L";
char lbl_1_data_90A[] = "g000mSP-head";
char lbl_1_data_938[] = "g000mSP-itemhook_M";

/* where the orb leaves the hand */
Vec lbl_1_data_94C = { 0.0f, 0.0f, 0.0f };

/* spot light of the orb */
Vec lbl_1_data_974 = { 0.0f, 1200.0f, 0.0f };
Vec lbl_1_data_980 = { 0.0f, -1.0f, 0.01f };
GXColor lbl_1_data_98C = { 0, 0, 0, 0 };
GXColor lbl_1_data_990 = { 0x80, 0xB0, 0xFF, 0xFF };

/* .bss */
KbBombSlot *lbl_1_bss_7828;
s16 lbl_1_bss_782C;
s16 lbl_1_bss_782E;
KbMiniEntry *lbl_1_bss_7830;
s32 lbl_1_bss_7834;
f32 lbl_1_bss_7838;
s16 lbl_1_bss_783C;

void fn_1_F784(KoopaBoss *self, KpHost *host) {
    CAMERA *cam = &host->camera[0];
    HU3DMODELID model;
    s32 i;

    memset(self, 0, sizeof(*self));
    self->host = host;
    model = Hu3DModelCreateData(0x75001A);
    self->modelId = model;
    Hu3DModelScaleSet(model, 5.5f, 5.5f, 5.5f);
    Hu3DModelCameraSet(model, (u16)cam->camMask);
    Hu3DModelLayerSet(model, 4);
    Hu3DModelAttrSet(model, 1);
    self->visible = 0;
    self->visibleReq = 0;
    self->pos = lbl_1_data_8CC;
    Hu3DModelPosSet(model, self->pos.x, self->pos.y, self->pos.z);
    Hu3DModelRotSet(model, 0.0f, 0.0f, 0.0f);
    for (i = 0; i < 19; i++) {
        self->motionId[i] = Hu3DJointMotionData(model, lbl_1_data_880[i]);
    }
    Hu3DMotionSet(model, self->motionId[1]);
    Hu3DModelAttrSet(model, HU3D_MOTATTR_LOOP);
    Hu3DMotionShiftSet(model, self->motionId[1], 0.0f, 4.0f, HU3D_MOTATTR_LOOP);
    self->reqMotion = 1;
    self->curMotion = 1;
    self->loopMotion = 1;

    model = Hu3DModelCreateData(0x75001C);
    self->orbModel = model;
    Hu3DModelPosSet(model, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(model, 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(model, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(model, (u16)cam->camMask);
    Hu3DModelLayerSet(model, 5);
    Hu3DModelAttrSet(model, 1);
    self->orbMotion = Hu3DJointMotionData(model, 0x750051);
    Hu3DMotionSet(model, self->orbMotion);
    Hu3DModelAttrSet(model, HU3D_MOTATTR_LOOP);

    model = Hu3DModelCreateData(0x75001F);
    self->ringModel = model;
    Hu3DModelPosSet(model, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(model, 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(model, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(model, (u16)cam->camMask);
    Hu3DModelLayerSet(model, 5);
    Hu3DModelAttrSet(model, 1);
    self->ringMotion = Hu3DJointMotionData(model, 0x750054);
    Hu3DMotionSet(model, self->ringMotion);
    Hu3DModelAttrReset(model, HU3D_MOTATTR_LOOP);

    model = Hu3DModelCreateData(0x75001D);
    self->glowModel = model;
    Hu3DModelPosSet(model, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(model, 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(model, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(model, (u16)cam->camMask);
    Hu3DModelLayerSet(model, 5);
    Hu3DModelAttrSet(model, 1);
    self->glowMotion = Hu3DJointMotionData(model, 0x750052);
    Hu3DMotionSet(model, self->glowMotion);
    Hu3DModelAttrSet(model, HU3D_MOTATTR_LOOP);

    model = Hu3DModelCreateData(0x75001E);
    self->hitModel = model;
    Hu3DModelPosSet(model, 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(model, 1.5f, 1.5f, 1.5f);
    Hu3DModelRotSet(model, -30.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(model, (u16)cam->camMask);
    Hu3DModelLayerSet(model, 4);
    Hu3DModelAttrSet(model, 1);
    self->hitMotion = Hu3DJointMotionData(model, 0x750053);
    Hu3DMotionSpeedSet(model, 0.75f);
    Hu3DMotionSet(model, self->hitMotion);
    Hu3DModelAttrReset(model, HU3D_MOTATTR_LOOP);
    self->hitPlaying = 0;

    model = Hu3DModelCreateData(0x750015);
    self->shadowModel = model;
    Hu3DModelRotSet(model, 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(model, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(model, (u16)cam->camMask);
    Hu3DModelLayerSet(model, 0);
    Hu3DModelAttrSet(model, 1);

    model = Hu3DModelCreateData(0x750022);
    self->chargeModel = model;
    Hu3DModelPosSet(model, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(model, 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(model, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(model, (u16)cam->camMask);
    Hu3DModelLayerSet(model, 5);
    Hu3DModelAttrSet(model, 1);
    self->chargeMotion = Hu3DJointMotionData(model, 0x750057);
    Hu3DMotionSet(model, self->chargeMotion);
    Hu3DMotionSpeedSet(model, 9.66f);
    Hu3DModelAttrSet(model, HU3D_MOTATTR_LOOP);

    self->lightId = -1;
    self->lightLevel = 0.0f;
    self->sndHandle = -1;
    if (host->diff == 0) {
        self->interval = 0x3C;
    } else if (host->diff == 1) {
        self->interval = 0x50;
    } else if (host->diff == 2) {
        self->interval = 0x64;
    } else {
        self->interval = 0x78;
    }
    self->life = 5;
    fn_1_1EE3C(&self->bombs, self);
    self->state = 1;
    self->timer = 0x64;
    fn_1_2CD6C(KP_HITB(host), 0x2D);
    fn_1_2CE3C(KP_HITB(host), fn_1_10348);
}

void fn_1_10018(KoopaBoss *self) {
    KpHost *host = self->host;
    s32 i;

    if (self->lightId >= 0) {
        Hu3DGLightKill(self->lightId);
    }
    fn_1_2CDE8(KP_HITB(host));
    fn_1_1F138(&self->bombs);
    Hu3DMotionKill(self->chargeMotion);
    Hu3DModelKill(self->chargeModel);
    Hu3DModelKill(self->shadowModel);
    Hu3DMotionKill(self->hitMotion);
    Hu3DModelKill(self->hitModel);
    Hu3DMotionKill(self->glowMotion);
    Hu3DModelKill(self->glowModel);
    Hu3DMotionKill(self->ringMotion);
    Hu3DModelKill(self->ringModel);
    Hu3DMotionKill(self->orbMotion);
    Hu3DModelKill(self->orbModel);
    for (i = 0; i < 19; i++) {
        Hu3DMotionKill(self->motionId[i]);
    }
    Hu3DModelKill(self->modelId);
    memset(self, 0, sizeof(*self));
}

/* lazily fetched node matrices of the boss model */
MtxPtr fn_1_1010C(KoopaBoss *self) {
    if (self->handRValid == 0) {
        Hu3DModelObjMtxGet(self->modelId, lbl_1_data_8E4, self->handR);
        self->handRValid = 1;
    }
    return self->handR;
}

MtxPtr fn_1_10160(KoopaBoss *self) {
    if (self->handLValid == 0) {
        Hu3DModelObjMtxGet(self->modelId, lbl_1_data_8F7, self->handL);
        self->handLValid = 1;
    }
    return self->handL;
}

MtxPtr fn_1_101B4(KoopaBoss *self) {
    if (self->headValid == 0) {
        Hu3DModelObjMtxGet(self->modelId, lbl_1_data_90A, self->head);
        self->headValid = 1;
    }
    return self->head;
}

/* the boss takes a hit: stagger and drop the attack in progress */
void fn_1_10208(KoopaBoss *self) {
    s32 hurtOrb;
    s32 i;

    if (self->state != 7) {
        self->reqMotion = 9;
        self->loopMotion = 0;
        if (self->sndHandle >= 0) {
            fn_1_83C(self->sndHandle);
            self->sndHandle = -1;
        }
        fn_1_814(0x8B4);
        hurtOrb = 0;
        if (self->orbState != 1 && self->orbState == 2) {
            self->orbTimer = 0;
            self->orbState = 5;
            hurtOrb = 1;
        }
        self->savedState = self->state;
        self->timer = 0x3C;
        if (self->state == 6 && hurtOrb == 0) {
            self->savedState = 1;
            self->timer = self->interval;
        }
        self->state = 7;
        Hu3DModelAttrSet(self->glowModel, 1);
        for (i = 0; i < 5; i++) {
            KbBombSlot *slot = &self->bombs.slot[i];

            if (slot->active != 0 && slot->state <= 2) {
                slot->unk8 = 0;
                slot->state = 8;
            }
        }
        self->life--;
    }
}

/* hit callback between two collision entries of the boss list */
void fn_1_10348(EffectEntry *a, EffectEntry *b) {
    if (a->type == 0 && b->type == 4) {
        fn_1_10520(a->user);
        return;
    }
    if (b->type == 0 && a->type == 4) {
        fn_1_10520(b->user);
        return;
    }
    if (a->type == 0 && b->type == 5) {
        fn_1_10AF0(a->user, b->user);
        return;
    }
    if (b->type == 0 && a->type == 5) {
        fn_1_10AF0(b->user, a->user);
        return;
    }
    if (a->type == 0 && b->type == 6) {
        fn_1_10FEC(a->user);
        return;
    }
    if (b->type == 0 && a->type == 6) {
        fn_1_10FEC(b->user);
        return;
    }
    if (a->type == 5 && b->type == 4) {
        fn_1_10588(a->user, b);
        return;
    }
    if (b->type == 5 && a->type == 4) {
        fn_1_10588(b->user, a);
        return;
    }
    if (a->type == 5 && b->type == 7) {
        fn_1_10918(a->user, b);
        return;
    }
    if (b->type == 5 && a->type == 7) {
        fn_1_10918(b->user, a);
        return;
    }
    if (b->type == 5 && a->type == 5) {
        fn_1_10784(a->user, b->user);
    }
}

/* the player touches a bomb fuse (type 4): knock it down while the boss fights */
void fn_1_10520(M562Player *player) {
    KpHost *host = player->owner;

    if (player->state == 1 || player->state == 2) {
        if (player->invuln == 0 && host->step == 4) {
            player->timerA = 0;
            player->sub = 0;
            player->subTimer = 0;
            player->state = 4;
        }
    }
}

/* a bomb touches a hit ring: wake it up and push it away from the ring */
void fn_1_10588(KbBombSlot *slot, EffectEntry *entry) {
    Vec d;
    f32 push;

    VECSubtract(&slot->pos, &entry->pos, &d);
    d.y = 0.0f;
    if (VECMag(&d) < 100.0f) {
        if (slot->state == 5) {
            slot->state = 6;
        }
        VECSubtract(&slot->pos, &lbl_1_data_8D8, &d);
        d.y = 0.0f;
        push = VECMag(&d) / 800.0f;
        push = KP_CLAMP(push, 0.0f, 1.0f);
        if (d.z < 200.0f) {
            d.z = 200.0f;
        }
        if (d.x == 0.0f && d.z == 0.0f) {
            d.z = 800.0f;
        }
        VECScale(&d, &d, 0.05f * (1.0f - push));
        VECAdd(&slot->pos, &d, &slot->pos);
    }
}

/* two bombs push each other apart */
void fn_1_10784(KbBombSlot *a, KbBombSlot *b) {
    Vec d;
    f32 dist;

    VECSubtract(&a->pos, &b->pos, &d);
    if (VECMag(&d) < 80.0f) {
        d.y = 0.0f;
        dist = VECMag(&d);
        if (d.x != 0.0f || d.z != 0.0f) {
            VECNormalize(&d, &d);
        } else {
            d.z = 1.0f;
        }
        VECScale(&d, &d, 0.5f * (80.0f - dist));
        VECAdd(&a->pos, &d, &a->pos);
        VECScale(&d, &d, -1.0f);
        VECAdd(&b->pos, &d, &b->pos);
        if (a->pos.y < 0.0f) {
            a->pos.y = 0.0f;
        }
        if (b->pos.y < 0.0f) {
            b->pos.y = 0.0f;
        }
    }
}

/* a bomb touches the boss's mouth point: a lit bomb hurts him */
void fn_1_10918(KbBombSlot *slot, EffectEntry *entry) {
    KoopaBoss *boss = slot->mgr->boss;
    KpHost *host = boss->host;
    Vec d;
    f32 dist;

    VECSubtract(&slot->pos, &entry->pos, &d);
    dist = VECMag(&d);
    if (dist < 40.0f + entry->radius) {
        if (d.x != 0.0f || d.z != 0.0f) {
            VECNormalize(&d, &d);
        } else {
            d.z = 1.0f;
        }
        VECScale(&d, &d, (40.0f + entry->radius) - dist);
        VECAdd(&slot->pos, &d, &slot->pos);
        if (slot->phase == 2 && boss->state != 7) {
            fn_1_10208(boss);
            Hu3DMotionTimeSet(boss->hitModel, 0.0f);
            Hu3DModelPosSet(boss->hitModel, slot->pos.x, 100.0f + slot->pos.y, slot->pos.z);
            Hu3DModelAttrReset(boss->hitModel, 1);
            boss->hitPlaying = 1;
            slot->state = 0xA;
            fn_1_814(0x8B2);
            fn_1_21B68(&host->camera[0], 1, 0x32, 80.0f, 0.9f, 90.0f);
            omVibrate(host->player.playerNo, 0xA, 0xC, 0);
        }
    }
}

/* the player and a bomb overlap vertically and horizontally: push them apart,
 * and a bomb that is still falling stuns the player */
void fn_1_10AF0(M562Player *player, KbBombSlot *slot) {
    KpHost *host = player->owner;
    f32 pLow;
    f32 pHigh;
    f32 bLow;
    f32 bHigh;
    f32 dist;
    Vec d;
    s32 hit;

    pLow = player->pos.y;
    pHigh = pLow + player->height;
    bLow = slot->pos.y;
    bHigh = 80.0f + bLow;
    d.x = player->pos.x - slot->pos.x;
    d.y = 0.0f;
    d.z = player->pos.z - slot->pos.z;
    dist = VECMag(&d);
    if (((pHigh == bHigh && pLow == bLow) || (pHigh <= bHigh && pHigh >= bLow) ||
         (pLow <= bHigh && pLow >= bLow) || (bHigh <= pHigh && bHigh >= pLow) ||
         (bLow <= pHigh && bLow >= pLow)) &&
        dist < 100.0f) {
        f32 pMid;
        f32 bMid;
        f32 pHalf;
        f32 bHalf;
        f32 big;
        f32 small;
        f32 gap;
        f32 weight;

        if (d.x != 0.0f || d.z != 0.0f) {
            VECNormalize(&d, &d);
        } else {
            d.x = 0.0f;
            d.z = 1.0f;
        }
        VECScale(&d, &d, 100.0f - dist);
        pMid = 0.5f * (pLow + pHigh);
        bMid = 0.5f * (bLow + bHigh);
        pHalf = KP_ABS(pMid - pLow);
        bHalf = KP_ABS(bMid - bLow);
        big = (pHalf > bHalf) ? pHalf : bHalf;
        small = (pHalf > bHalf) ? bHalf : pHalf;
        gap = KP_ABS(pMid - bMid);
        if (gap < big) {
            weight = 1.0f;
        } else {
            weight = 1.0f - ((gap - big) / small);
        }
        player->pos.x += 0.5f * d.x * weight;
        player->pos.z += 0.5f * d.z * weight;
        slot->pos.x -= 0.5f * d.x * weight;
        slot->pos.z -= 0.5f * d.z * weight;
    }
    hit = 0;
    d.x = player->pos.x - slot->pos.x;
    d.y = player->pos.y - slot->pos.y;
    d.z = player->pos.z - slot->pos.z;
    if (VECMag(&d) < slot->radius && slot->phase == 0) {
        hit = 1;
    }
    if (hit != 0 && (player->state == 1 || player->state == 2) && player->invuln == 0 && host->step == 4) {
        player->timerA = 0;
        player->sub = 0;
        player->subTimer = 0;
        player->state = 4;
    }
}

/* the player touches the orb ring (type 6) */
void fn_1_10FEC(M562Player *player) {
    KpHost *host = player->owner;

    if (player->state == 1 || player->state == 2) {
        if (player->invuln == 0 && host->step == 4) {
            player->timerA = 0;
            player->sub = 0;
            player->subTimer = 0;
            player->state = 5;
        }
    }
}

/* per-frame update */
void fn_1_11054(KoopaBoss *self) {
    KpHost *host = self->host;
    M562Player *player = &host->player;
    HU3DMODELID model;
    f32 radius;
    Vec p;
    Vec fwd;
    Vec out;
    Mtx mtx;
    Mtx rot;

    if (self->visibleReq != 0 && self->visible == 0) {
        Hu3DModelAttrReset(self->modelId, 1);
        self->visible = 1;
    } else if (self->visibleReq == 0 && self->visible != 0) {
        Hu3DModelAttrSet(self->modelId, 1);
        self->visible = 0;
    }
    if (self->visible != 0) {
        if (self->active == 0) {
            if (self->state != 0 && self->state != 7) {
                Hu3DModelAttrSet(self->glowModel, 1);
                self->goalAngle = 0.0f;
                self->reqMotion = 1;
                self->loopMotion = 1;
                self->state = 0;
            }
        } else if (self->state == 0) {
            self->state = 1;
        }
        switch (self->state) {
        case 1:
            fn_1_11634(self);
            if (--self->timer <= 0) {
                self->state = 2;
                self->timer = 0;
                self->sub = 0;
                self->timerA = 0;
            }
            break;
        case 2:
            if (fn_1_1183C(self) != 0) {
                self->state = 3;
                self->timer = self->interval;
                self->sub = 0;
                self->timerA = 0;
            }
            break;
        case 3:
            fn_1_11634(self);
            if (--self->timer <= 0) {
                self->state = 4;
                self->timer = 0;
                self->sub = 0;
                self->timerA = 0;
            }
            break;
        case 4:
            if (fn_1_12280(self) != 0) {
                self->state = 5;
                self->timer = self->interval;
                self->sub = 0;
                self->timerA = 0;
            }
            break;
        case 5:
            fn_1_11634(self);
            if (--self->timer <= 0) {
                self->state = 6;
                self->timer = 0;
                self->sub = 0;
                self->timerA = 0;
            }
            break;
        case 6:
            if (fn_1_12A38(self) != 0) {
                self->state = 1;
                self->timer = self->interval;
                self->sub = 0;
                self->timerA = 0;
            }
            break;
        case 7:
            if (--self->timer <= 0) {
                if (self->active == 0) {
                    self->goalAngle = 0.0f;
                    self->reqMotion = 1;
                    self->loopMotion = 1;
                    self->state = 0;
                } else {
                    self->state = self->savedState;
                }
                self->timer = 0;
                self->sub = 0;
                self->timerA = 0;
            }
            break;
        }
        model = self->modelId;
        if (self->curMotion != self->reqMotion && Hu3DMotionShiftIDGet(model) < 0) {
            self->curMotion = self->reqMotion;
            Hu3DModelAttrReset(model, HU3D_MOTATTR_LOOP);
            Hu3DMotionShiftSet(model, self->motionId[self->curMotion], 0.0f, 4.0f,
                               (self->loopMotion != 0) ? HU3D_MOTATTR_LOOP : 0);
        }
        Hu3DModelPosSet(self->modelId, self->pos.x, self->pos.y, self->pos.z);
        Hu3DModelRotSet(self->modelId, self->rot.x, self->rot.y, self->rot.z);
        if (host->step == 4 || host->step == 5) {
            self->angle = fn_1_36EC(self->goalAngle, self->angle, 0.96f);
            Hu3DMotionForceSet(self->modelId, lbl_1_data_90A, 0x100, self->angle);
        }
        fn_1_1F208(&self->bombs);
        fn_1_12BA8(self);
        fn_1_13A24(self);
        radius = 0.5f * player->height;
        p = player->pos;
        p.y += radius;
        fn_1_2CE8C(KP_HITB(host), 0, 0, &p, player, radius);
        if (self->headValid == 0) {
            Hu3DModelObjMtxGet(self->modelId, lbl_1_data_90A, self->head);
            self->headValid = 1;
        }
        MTXCopy(self->head, mtx);
        fwd.x = mtx[0][2];
        fwd.y = mtx[1][2];
        fwd.z = mtx[2][2];
        VECNormalize(&fwd, &fwd);
        MTXRotRad(rot, 'X', -1.2217305f);
        MTXMultVec(rot, &fwd, &out);
        VECScale(&out, &out, 220.0f);
        Hu3DMtxTransGet(self->head, &p);
        VECAdd(&p, &out, &p);
        fn_1_2CE8C(KP_HITB(host), 7, 0, &p, self, 250.0f);
        fn_1_2CE44(KP_HITB(host));
        if (self->hitPlaying != 0) {
            f32 maxTime = Hu3DMotionMaxTimeGet(self->hitModel);

            if (Hu3DMotionTimeGet(self->hitModel) >= maxTime) {
                Hu3DModelAttrSet(self->hitModel, 1);
                self->hitPlaying = 0;
            }
        }
        self->handRValid = 0;
        self->handLValid = 0;
        self->headValid = 0;
    }
}

/* face the player */
void fn_1_11634(KoopaBoss *self) {
    Vec d;

    self->reqMotion = 1;
    self->loopMotion = 1;
    VECSubtract(&self->host->player.pos, &self->pos, &d);
    d.y = 0.0f;
    if (d.x != 0.0f || d.y != 0.0f) {
        VECNormalize(&d, &d);
    }
    self->goalAngle = KP_HEADING(d.x, d.z);
}

/* spit bombs at the player */
s32 fn_1_1183C(KoopaBoss *self) {
    KpHost *host = self->host;
    M562Player *player = &host->player;
    KbBombMgr *mgr = &self->bombs;
    KbBombSlot *slot;
    Vec d;
    Vec p;
    f32 rnd;
    s32 done;

    VECSubtract(&player->pos, &self->pos, &d);
    d.y = 0.0f;
    if (d.x != 0.0f || d.y != 0.0f) {
        VECNormalize(&d, &d);
    }
    self->goalAngle = KP_HEADING(d.x, d.z);
    done = 0;
    switch (self->sub) {
    case 0:
        lbl_1_bss_782C = (frand() % 3) + 2;
        if ((s32)(lbl_1_bss_782C + mgr->count) > 4) {
            lbl_1_bss_782C -= lbl_1_bss_782C + (mgr->count - 4);
        }
        lbl_1_bss_782C = KP_CLAMP(lbl_1_bss_782C, 0, 4);
        if (lbl_1_bss_782C == 0) {
            done = 1;
            break;
        }
        lbl_1_bss_782E = 0;
        self->timerA = 0;
        self->sub++;
        /* fallthrough */
    case 1:
        self->reqMotion = 2;
        self->loopMotion = 0;
        self->timerA++;
        if (self->timerA == 0x28) {
            slot = fn_1_1F298(mgr, &player->pos);
            if (slot == NULL) {
                return 1;
            }
            slot->vel = lbl_1_rodata_4F0;
            lbl_1_bss_7828 = slot;
            if (self->sndHandle < 0) {
                self->sndHandle = fn_1_814(0x8A9);
            }
            break;
        }
        if (self->timerA >= 0x5A) {
            self->timerA = 0;
            self->sub++;
        }
        break;
    case 2:
        if (player->pos.x <= -341.66666f) {
            self->reqMotion = 4;
            self->loopMotion = 0;
        } else if (player->pos.x >= 341.66666f) {
            self->reqMotion = 5;
            self->loopMotion = 0;
        } else {
            self->reqMotion = 3;
            self->loopMotion = 0;
        }
        self->timerA = 0;
        self->sub++;
        break;
    case 3:
        if (self->timerA == 0x14) {
            fn_1_814(0x8AA);
        }
        if (++self->timerA > 0x1E) {
            slot = lbl_1_bss_7828;
            slot->mode = 1;
            rnd = (f32)(frand() % 10000) / 10000.0f;
            p.x = player->pos.x;
            p.y = player->pos.y;
            p.z = player->pos.z;
            if (host->diff == 0) {
                if ((u32)(frand() % 10000) < 0x1770U) {
                    p.x = (f32)((f64)p.x + (40.0 * ((f64)player->speed * HuSin(player->rotY))));
                    p.z = (f32)((f64)p.z + (40.0 * ((f64)player->speed * HuCos(player->rotY))));
                } else {
                    p.x = (f32)((f64)p.x + ((f64)(rnd - 0.5f) * (100.0 * HuSin(player->rotY))));
                    p.z = (f32)((f64)p.z + ((f64)(rnd - 0.5f) * (100.0 * HuCos(player->rotY))));
                }
            } else {
                p.x = (f32)((f64)p.x + ((f64)(rnd - 0.5f) * (500.0 * HuSin(player->rotY))));
                p.z = (f32)((f64)p.z + ((f64)(rnd - 0.5f) * (500.0 * HuCos(player->rotY))));
            }
            p.x = KP_CLAMP(p.x, -1025.0f, 1025.0f);
            p.z = KP_CLAMP(p.z, -300.0f, 500.0f);
            slot->spawn.x = p.x;
            slot->spawn.y = p.y;
            slot->spawn.z = p.z;
            self->timerA = 0;
            self->sub++;
        }
        break;
    case 4:
        lbl_1_bss_7828->vel.x = lbl_1_bss_7828->vel.x * 0.9f;
        lbl_1_bss_7828->vel.z = lbl_1_bss_7828->vel.z * 0.9f;
        self->timerA++;
        if (lbl_1_bss_782E >= (s32)(lbl_1_bss_782C - 1)) {
            if (self->sndHandle >= 0) {
                fn_1_83C(self->sndHandle);
                self->sndHandle = -1;
            }
            if (++self->timerA >= 0x28) {
                self->reqMotion = 1;
                self->loopMotion = 1;
                self->timerA = 0;
                self->sub = 0;
                done = 1;
            }
            break;
        }
        if (self->timerA == 0x28) {
            slot = fn_1_1F298(mgr, &player->pos);
            if (slot == NULL) {
                return 1;
            }
            slot->vel = lbl_1_rodata_4FC;
            lbl_1_bss_7828 = slot;
            break;
        }
        if (self->timerA >= 0x3C) {
            lbl_1_bss_782E++;
            self->curMotion = 2;
            self->timerA = 0;
            self->sub = 2;
        }
        break;
    }
    return done;
}

/* charge the orb in the hand of the boss and swarm mini mecha-koopas */
s32 fn_1_12280(KoopaBoss *self) {
    KpHost *host = self->host;
    M562Player *player = &host->player;
    KbMiniEntry *mini;
    Mtx mtx;
    Vec p;
    Vec d;
    f32 t;
    f32 s;
    s32 done;

    Hu3DModelObjMtxGet(self->modelId, lbl_1_data_938, mtx);
    Hu3DMtxTransGet(mtx, &p);
    done = 0;
    switch (self->sub) {
    case 0:
        self->reqMotion = 6;
        self->loopMotion = 0;
        Hu3DModelScaleSet(self->glowModel, 0.0f, 0.0f, 0.0f);
        Hu3DModelAttrReset(self->glowModel, 1);
        lbl_1_bss_7834 = 0;
        lbl_1_bss_7830 = NULL;
        lbl_1_bss_7838 = self->goalAngle;
        self->timerA = 0;
        self->sub++;
        /* fallthrough */
    case 1:
        if (self->sndHandle < 0) {
            self->sndHandle = fn_1_814(0x8AC);
        }
        d.x = (f32)(50.0 * HuSin(self->angle));
        d.y = 0.0f;
        d.z = (f32)(50.0 * HuCos(self->angle));
        VECAdd(&p, &d, &p);
        t = (f32)self->timerA / 90.0f;
        Hu3DModelPosSet(self->glowModel, (f32)((f64)p.x + (3.0 * HuSin(24.0f * (360.0f * t)))),
                        (f32)((f64)p.y + (3.0 * HuCos(24.0f * (360.0f * t)))), p.z);
        s = (f32)((f64)(8.0f * t) + (0.5 * HuSin(18.0f * (360.0f * t))));
        Hu3DModelScaleSet(self->glowModel, s, s, s);
        if (++self->timerA >= 0x5A) {
            lbl_1_bss_783C = frand();
            self->reqMotion = 7;
            self->loopMotion = 0;
            self->timerA = 0;
            self->sub++;
        }
        break;
    case 2:
        if (self->sndHandle >= 0) {
            fn_1_83C(self->sndHandle);
            self->sndHandle = -1;
        }
        if (self->timerA % 8 == 0) {
            omVibrate(player->playerNo, 5, 0xC, 0);
        }
        t = (f32)(self->timerA % 90) / 90.0f;
        if ((lbl_1_bss_783C & 1) != 0) {
            self->goalAngle = (f32)((f64)lbl_1_bss_7838 + (45.0 * HuSin(360.0f * t)));
        } else {
            self->goalAngle = (f32)((f64)lbl_1_bss_7838 + (45.0 * HuSin(360.0f * (1.0f - t))));
        }
        if (self->timerA % 2 == 0) {
            mini = fn_1_181FC(&host->mecha, &p, self->angle);
            if (lbl_1_bss_7834 == 0 && mini != NULL) {
                lbl_1_bss_7830 = mini;
                lbl_1_bss_7834 = 1;
            }
        }
        if (++self->timerA >= 0xB4) {
            self->timerA = 0;
            self->sub++;
        }
        break;
    case 3:
        self->reqMotion = 1;
        self->loopMotion = 1;
        self->timerA = 0;
        self->sub = 0;
        done = 1;
        break;
    }
    if (lbl_1_bss_7830 != NULL) {
        if (self->sub > 1) {
            t = (f32)self->timerA * 0.03125f;
            t = KP_CLAMP(t, 0.0f, 1.0f);
            s = 8.0f + (16.0f * t);
            Hu3DModelScaleSet(self->glowModel, s, s, s);
            Hu3DModelPosSet(self->glowModel, lbl_1_bss_7830->pos.x, lbl_1_bss_7830->pos.y, lbl_1_bss_7830->pos.z);
        }
        if (self->timerA >= 0x20) {
            Hu3DModelScaleSet(self->glowModel, 0.0f, 0.0f, 0.0f);
            Hu3DModelAttrSet(self->glowModel, 1);
            lbl_1_bss_7830 = NULL;
        }
    }
    return done;
}

/* wait for the orb to be thrown and finish */
s32 fn_1_12A38(KoopaBoss *self) {
    s32 done;

    done = 0;
    switch (self->sub) {
    case 0:
        self->goalAngle = 0.0f;
        self->reqMotion = 1;
        self->loopMotion = 1;
        self->timerA = 0;
        self->sub++;
        /* fallthrough */
    case 1:
        if (self->orbState == 1) {
            self->reqMotion = 8;
            self->loopMotion = 0;
            self->orbRequest = 1;
            self->timerA = 0;
            self->sub++;
        }
        break;
    case 2:
        if (self->orbState >= 3) {
            self->timerA = 0;
            self->sub++;
        }
        break;
    case 3:
        if (++self->timerA >= 0x78) {
            self->timerA = 0;
            self->sub = 0;
            done = 1;
        }
        break;
    }
    return done;
}

/* the orb: grows in the left hand, is thrown into the arena and bursts */
void fn_1_12BA8(KoopaBoss *self) {
    KpHost *host = self->host;
    M562Player *player = &host->player;
    CAMERA *cam = &host->camera[0];
    KbBombMgr *mgr = &self->bombs;
    Vec offset;
    Vec p;
    Vec q;
    f32 t;
    f32 s;
    f32 r;
    s32 i;

    offset = lbl_1_rodata_59C;
    if ((u32)self->orbState <= 6) {
        switch (self->orbState) {
        case 0:
            self->orbScale = 0.0f;
            Hu3DModelScaleSet(self->orbModel, 0.0f, 0.0f, 0.0f);
            Hu3DModelAttrSet(self->orbModel, 1);
            Hu3DModelScaleSet(self->shadowModel, 0.0f, 0.0f, 0.0f);
            Hu3DModelAttrReset(self->shadowModel, 1);
            Hu3DMotionSpeedSet(self->orbModel, 0.5f);
            self->orbTimer = 0;
            self->orbState = 1;
            return;
        case 1:
            if (self->orbRequest != 0) {
                Hu3DModelAttrReset(self->orbModel, 1);
                Hu3DModelAttrReset(self->shadowModel, 1);
                self->orbTimer = 0;
                self->orbState = 2;
                self->orbRequest = 0;
                return;
            }
            break;
        case 2:
            t = (f32)(self->orbTimer - 0x28) / 60.0f;
            t = KP_CLAMP(t, 0.0f, 1.0f);
            s = 2.0f * t;
            self->orbScale = s;
            Hu3DModelScaleSet(self->orbModel, s, s, s);
            s = s * 1.5f;
            Hu3DModelScaleSet(self->shadowModel, s, s, s);
            if (self->handLValid == 0) {
                Hu3DModelObjMtxGet(self->modelId, lbl_1_data_8F7, self->handL);
                self->handLValid = 1;
            }
            Hu3DMtxTransGet(self->handL, &p);
            VECAdd(&p, &offset, &p);
            Hu3DModelPosSet(self->orbModel, p.x, p.y, p.z);
            Hu3DModelPosSet(self->shadowModel, p.x, 0.0f, p.z);
            self->orbTimer++;
            if (self->orbTimer == 0x28) {
                Hu3DModelScaleSet(self->orbModel, 0.0f, 0.0f, 0.0f);
                Hu3DModelAttrReset(self->orbModel, 1);
                Hu3DMotionSpeedSet(self->orbModel, 0.5f);
                if (self->sndHandle < 0) {
                    self->sndHandle = fn_1_814(0x8AF);
                    return;
                }
            } else if (self->orbTimer >= 0x64) {
                lbl_1_data_94C.x = p.x;
                lbl_1_data_94C.y = p.y;
                lbl_1_data_94C.z = p.z;
                self->orbTimer = 0;
                self->orbState = 3;
                if (self->sndHandle >= 0) {
                    fn_1_83C(self->sndHandle);
                    self->sndHandle = -1;
                }
                fn_1_814(0x8AA);
                return;
            }
            break;
        case 3:
            t = (f32)self->orbTimer / 40.0f;
            if (t < 0.4f) {
                p.y = lbl_1_data_94C.y + (250.0f * (f32)HuSin(90.0f * (t / 0.4f)));
            } else {
                p.y = lbl_1_data_8D8.y + ((f32)HuSin(90.0f + (90.0f * ((t - 0.4f) / 0.6f))) *
                                          (250.0f + (lbl_1_data_94C.y - lbl_1_data_8D8.y)));
            }
            p.x = lbl_1_data_94C.x + (t * (lbl_1_data_8D8.x - lbl_1_data_94C.x));
            p.z = lbl_1_data_94C.z + (t * (lbl_1_data_8D8.z - lbl_1_data_94C.z));
            Hu3DModelPosSet(self->orbModel, p.x, p.y, p.z);
            Hu3DModelPosSet(self->shadowModel, p.x, 0.0f, p.z);
            fn_1_2CE8C(KP_HITB(host), 6, 0, &p, NULL, 120.0f);
            self->orbTimer++;
            if (self->orbTimer >= 0x28) {
                fn_1_934(0x8B0, &p);
                if (self->sndHandle >= 0) {
                    fn_1_83C(self->sndHandle);
                    self->sndHandle = -1;
                }
                self->sndHandle = fn_1_934(0x8B1, &p);
                if (host->step > 4) {
                    self->orbTimer = 0;
                    self->orbState = 5;
                    return;
                }
                Hu3DModelScaleSet(self->ringModel, 1.0f, 1.0f, 1.0f);
                Hu3DModelPosSet(self->ringModel, lbl_1_data_8D8.x, 55.0f + lbl_1_data_8D8.y, lbl_1_data_8D8.z);
                Hu3DMotionSpeedSet(self->ringModel, 4.1666665f);
                Hu3DMotionTimeSet(self->ringModel, 0.0f);
                Hu3DModelAttrReset(self->ringModel, 1);
                fn_1_21B68(cam, 1, 0x32, 40.0f, 0.9f, 90.0f);
                omVibrate(player->playerNo, 0xA, 0xC, 0);
                self->orbTimer = 0;
                self->orbState = 4;
                return;
            }
            break;
        case 4:
            if (self->orbTimer > 0xA) {
                if (self->orbTimer % 4 == 0) {
                    fn_1_21B68(cam, 1, 0x32, 10.0f, 0.7f, 90.0f);
                }
            }
            if (self->orbTimer % 8 == 0) {
                omVibrate(player->playerNo, 5, 0xC, 0);
            }
            if (self->orbTimer < 0xA) {
                t = (f32)self->orbTimer / 10.0f;
                t = KP_CLAMP(t, 0.0f, 1.0f);
                s = 0.3f + (0.7f * (1.0f - t));
                Hu3DModelScaleSet(self->orbModel, s, s, s);
                s = s * 1.5f;
                Hu3DModelScaleSet(self->shadowModel, s, s, s);
            } else if (self->orbTimer == 0xA) {
                Hu3DModelScaleSet(self->orbModel, 0.0f, 0.0f, 0.0f);
                Hu3DModelAttrSet(self->orbModel, 1);
                Hu3DModelScaleSet(self->shadowModel, 0.0f, 0.0f, 0.0f);
                Hu3DModelAttrSet(self->orbModel, 1);
                self->orbScale = 0.0f;
            }
            t = (f32)self->orbTimer / 60.0f;
            t = KP_CLAMP(t, 0.0f, 1.0f);
            r = 1400.0f * t;
            mgr->attract = 1;
            mgr->attractPos.x = lbl_1_data_8D8.x;
            mgr->attractPos.y = lbl_1_data_8D8.y;
            mgr->attractPos.z = lbl_1_data_8D8.z;
            mgr->attractRange = r;
            if (self->sndHandle >= 0) {
                VECSubtract(&cam->pos, &lbl_1_data_94C, &q);
                VECNormalize(&q, &q);
                VECScale(&q, &q, r);
                VECAdd(&q, &lbl_1_data_94C, &q);
                fn_1_964(self->sndHandle, &q);
            }
            if (host->step == 4 && fn_1_13C1C(player, &lbl_1_data_8D8, r) != 0 &&
                (player->state == 1 || player->state == 2) && player->invuln == 0 && host->step == 4) {
                player->timerA = 0;
                player->sub = 0;
                player->subTimer = 0;
                player->state = 5;
            }
            self->lightLevel = (f32)HuSin(180.0f * t);
            self->orbTimer++;
            if (self->orbTimer >= 0x3C) {
                self->lightLevel = 0.0f;
                if (self->sndHandle >= 0) {
                    fn_1_83C(self->sndHandle);
                    self->sndHandle = -1;
                }
                for (i = 0; i < 5; i++) {
                    KbBombSlot *slot = &mgr->slot[i];

                    VECSubtract(&slot->pos, &lbl_1_data_8D8, &q);
                    q.y = 0.0f;
                    r = VECMag(&q);
                    if (slot->flag != 0 && slot->active != 0 && slot != player->target) {
                        slot->state = 0xA;
                        slot->flag = 0;
                    }
                }
                self->orbTimer = 0;
                self->orbState = 6;
                return;
            }
            break;
        case 5:
            t = (f32)self->orbTimer / 10.0f;
            t = KP_CLAMP(t, 0.0f, 1.0f);
            s = self->orbScale * (1.0f - t);
            Hu3DModelScaleSet(self->orbModel, s, s, s);
            s = s * 1.5f;
            Hu3DModelScaleSet(self->shadowModel, s, s, s);
            if (++self->orbTimer >= 0xA) {
                self->orbTimer = 0;
                self->orbState = 6;
                return;
            }
            break;
        case 6:
            Hu3DModelScaleSet(self->ringModel, 0.0f, 0.0f, 0.0f);
            Hu3DModelAttrSet(self->ringModel, 1);
            Hu3DModelScaleSet(self->orbModel, 0.0f, 0.0f, 0.0f);
            Hu3DModelAttrSet(self->orbModel, 1);
            self->orbScale = 0.0f;
            self->orbTimer = 0;
            self->orbState = 1;
            break;
        }
    }
}

/* spot light that follows the orb's burst */
void fn_1_13A24(KoopaBoss *self) {
    GXColor col;
    f32 level;

    if (self->lightLevel > 0.0f) {
        col = lbl_1_data_990;
        if (self->lightId < 0) {
            self->lightId = Hu3DGLightCreateV(&lbl_1_data_974, &lbl_1_data_980, &lbl_1_data_98C);
            Hu3DGLightStaticSet(self->lightId, 1);
            Hu3DGLightSpotSet(self->lightId, GX_SP_COS, 30.0f);
            Hu3DGLightInfinitytSet(self->lightId);
        }
        if (self->lightId >= 0) {
            level = self->lightLevel;
            col.r = (u8)((f32)lbl_1_data_990.r * level);
            col.g = (u8)((f32)lbl_1_data_990.g * level);
            col.b = (u8)((f32)lbl_1_data_990.b * level);
            col.a = 0xFF;
            Hu3DGLightColorSet(self->lightId, col.r, col.g, col.b, col.a);
        }
    } else if (self->lightId >= 0) {
        Hu3DGLightColorSet(self->lightId, 0, 0, 0, 0);
        Hu3DGLightKill(self->lightId);
        self->lightId = -1;
    }
}

/* is the player standing on the orb's shock ring of this radius */
s32 fn_1_13C1C(M562Player *player, Vec *center, f32 radius) {
    Vec d;
    f32 dist;
    s32 hit;

    hit = 0;
    VECSubtract(&player->pos, center, &d);
    d.y = 0.0f;
    dist = VECMag(&d);
    if (dist >= (radius - 70.0f) && dist <= (70.0f + radius) && player->pos.y < 40.0f) {
        hit = 1;
    }
    return hit;
}
