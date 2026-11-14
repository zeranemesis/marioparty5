// m562dll/koopa - the Bowser actor of the boss battle: model setup, the
// per-frame state machine (walk / stomp / charge / jump / fall) and the hit
// callbacks against the player.
#include "REL/m562dll.h"
#include "game/frand.h"
#include "game/hsfex.h"
#include "humath.h"
#include <string.h>

/* returns a float in the original (see the note in game/hu3d.h) */
float Hu3DMotionMaxTimeGet(HU3DMODELID modelId);

static void fn_1_B7F0(Koopa *self);
static void fn_1_BAE8(Koopa *self);
static void fn_1_BDA4(Koopa *self);
void fn_1_CE24(Koopa *self);
s32 fn_1_D404(Koopa *self);
s32 fn_1_DCC4(Koopa *self);
s32 fn_1_E558(Koopa *self);

/* .rodata vectors (offsets of the weapon-swing effect, in Bowser's space) */
static const Vec lbl_1_rodata_360 = { -80.0f, 0.0f, 50.0f };
static const Vec lbl_1_rodata_36C = { 80.0f, 0.0f, 50.0f };
static const Vec lbl_1_rodata_378 = { -80.0f, 0.0f, 30.0f };
static const Vec lbl_1_rodata_384 = { 80.0f, 0.0f, 30.0f };

/* .data */
/* the 14 joint-motion ids this koopa cycles through */
u32 lbl_1_data_7F8[14] = {
    0x0075002D, 0x0075002E, 0x0075002F, 0x00750030,
    0x00750031, 0x00750032, 0x00750033, 0x00750034,
    0x00750035, 0x00750036, 0x00750037, 0x00750038,
    0x00750039, 0x0075003A
};

/* spawn position handed to Hu3DModelPosSet */
Vec lbl_1_data_830 = { 0.0f, 2000.0f, -1176.0f };

/* position of the first state-0 frame */
Vec lbl_1_data_83C = { 0.0f, 0.0f, -125.0f };

/* name of the node the fire-ball effect is spawned from */
char lbl_1_data_86C[0x14] = "g000m0-ns_hed";

/* .bss */
s16 lbl_1_bss_7820;
s16 lbl_1_bss_7822;

void fn_1_B2E0(Koopa *self, KpHost *host) {
    HU3DMODELID model;
    s32 i;

    memset(self, 0, sizeof(*self));
    self->host = host;
    self->prevTime = 0.0f;
    model = Hu3DModelCreateData(0x750017);
    self->modelId = model;
    Hu3DModelScaleSet(model, 1.5f, 1.5f, 1.5f);
    Hu3DModelCameraSet(model, (u16)(host->camera[0].camMask | host->camera[1].camMask));
    Hu3DModelLayerSet(model, 4);
    Hu3DModelAttrReset(model, 1);
    Hu3DModelShadowSet(model);
    Hu3DMotionSpeedSet(model, 1.0f);
    self->pos = lbl_1_data_830;
    Hu3DModelPosSet(model, self->pos.x, self->pos.y, self->pos.z);
    Hu3DModelRotSet(model, 0.0f, 0.0f, 0.0f);
    for (i = 0; i < 14; i++) {
        self->motionId[i] = Hu3DJointMotionData(model, lbl_1_data_7F8[i]);
    }
    Hu3DMotionSet(model, self->motionId[0]);
    Hu3DModelAttrSet(model, HU3D_MOTATTR_LOOP);
    Hu3DMotionShiftSet(model, self->motionId[0], 0.0f, 4.0f, HU3D_MOTATTR_LOOP);
    self->curMotion = self->reqMotion = 0;
    self->loopMotion = 1;
    fn_1_2CD6C(KP_HITA(host), 0x24);
    fn_1_2CE3C(KP_HITA(host), fn_1_EEC4);
}

void fn_1_B4F8(Koopa *self) {
    s32 i;

    fn_1_2CDE8(KP_HITA(self->host));
    for (i = 0; i < 14; i++) {
        Hu3DMotionKill(self->motionId[i]);
    }
    Hu3DModelKill(self->modelId);
    memset(self, 0, sizeof(*self));
}

/* per-frame update */
void fn_1_B578(Koopa *self) {
    HU3DMODELID model;
    f32 blend;

    if (self->visibleReq != 0 && self->visible == 0) {
        Hu3DModelAttrReset(self->modelId, 1);
        self->visible = 1;
    } else if (self->visibleReq == 0 && self->visible != 0) {
        Hu3DModelAttrSet(self->modelId, 1);
        self->visible = 0;
    }
    if (self->visible != 0) {
        switch (self->state) {
        case 0:
            break;
        case 1:
            fn_1_B7F0(self);
            break;
        case 2:
            fn_1_BAE8(self);
            break;
        case 3:
            fn_1_BDA4(self);
            break;
        }
        model = self->modelId;
        if (self->curMotion != self->reqMotion && Hu3DMotionShiftIDGet(model) < 0 && self->shiftHold <= 0) {
            if (self->curMotion == 10) {
                blend = 30.0f;
            } else if (self->reqMotion == 9) {
                blend = 8.0f;
            } else if (self->curMotion == 9) {
                blend = 15.0f;
            } else {
                blend = 4.0f;
            }
            self->curMotion = self->reqMotion;
            Hu3DModelAttrReset(model, HU3D_MOTATTR_LOOP);
            Hu3DMotionShiftSet(model, self->motionId[self->curMotion], 0.0f, blend,
                               (self->loopMotion != 0) ? HU3D_MOTATTR_LOOP : 0);
            self->shiftHold = self->pendingDelay;
            self->pendingDelay = 0;
            self->prevTime = Hu3DMotionTimeGet(self->modelId);
        }
        if (self->shiftHold != 0) {
            self->shiftHold--;
        }
    }
}

static void fn_1_B7F0(Koopa *self) {
    KpHost *host = self->host;
    M562Player *player = &host->player;
    HU3DMODELID model;

    if (self->active == 0) {
        model = self->modelId;
        self->pos = lbl_1_data_830;
        Hu3DModelPosSet(model, self->pos.x, self->pos.y, self->pos.z);
        Hu3DModelRotSet(model, 0.0f, 0.0f, 0.0f);
    }
    switch (self->sub) {
    case 10:
        break;
    case 0:
        model = self->modelId;
        Hu3DModelCameraSet(model, (u16)(host->camera[0].camMask | host->camera[1].camMask));
        Hu3DModelAttrReset(model, 1);
        Hu3DModelShadowReset(model);
        Hu3DMotionSpeedSet(model, 1.0f);
        self->pos = lbl_1_data_830;
        Hu3DModelPosSet(model, self->pos.x, self->pos.y, self->pos.z);
        Hu3DModelRotSet(model, 0.0f, 0.0f, 0.0f);
        self->sub = 1;
        self->timerA = 0;
        /* fallthrough */
    case 1:
        self->reqMotion = 0;
        self->loopMotion = 1;
        if (player->state == 4) {
            self->reqMotion = 2;
            self->loopMotion = 0;
            self->sub = 2;
            self->timerA = 0xA0;
        } else if (host->fire.count < lbl_1_bss_7822) {
            self->reqMotion = 9;
            self->loopMotion = 0;
            self->sub = 2;
            self->timerA = 0x14;
        } else if (host->step > 4 && host->winner == 0) {
            fn_1_814(0x8C5);
            self->reqMotion = 10;
            self->loopMotion = 0;
            self->sub = 3;
            self->timerA = 0;
        }
        break;
    case 2:
        if (self->curMotion == 2 && self->timerA == 0x6E) {
            fn_1_814(0x8B3);
        }
        if (--self->timerA <= 0) {
            self->sub = 1;
            self->timerA = 0;
        }
        break;
    case 3: {
        f32 maxTime = Hu3DMotionMaxTimeGet(self->modelId);

        if (Hu3DMotionTimeGet(self->modelId) >= maxTime) {
            Hu3DMotionTimeSet(self->modelId, 40.0f);
        }
        break;
    }
    }
    lbl_1_bss_7822 = host->fire.count;
}

static void fn_1_BAE8(Koopa *self) {
    KpHost *host = self->host;
    M562Player *player = &host->player;
    HU3DMODELID model;

    if (self->active == 0) {
        model = self->modelId;
        self->pos = lbl_1_data_830;
        Hu3DModelPosSet(model, self->pos.x, self->pos.y, self->pos.z);
        Hu3DModelRotSet(model, 0.0f, 0.0f, 0.0f);
    }
    switch (self->sub) {
    case 0:
        model = self->modelId;
        Hu3DModelCameraSet(model, (u16)(host->camera[0].camMask | host->camera[1].camMask));
        Hu3DModelAttrReset(model, 1);
        Hu3DModelShadowReset(model);
        Hu3DMotionSpeedSet(model, 1.0f);
        self->pos = lbl_1_data_830;
        Hu3DModelPosSet(model, self->pos.x, self->pos.y, self->pos.z);
        Hu3DModelRotSet(model, 0.0f, 0.0f, 0.0f);
        self->sub = 1;
        self->timerA = 0;
        /* fallthrough */
    case 1:
        self->reqMotion = 0;
        self->loopMotion = 1;
        if (player->state == 4) {
            self->reqMotion = 2;
            self->loopMotion = 0;
            self->sub = 2;
            self->timerA = 0xA0;
        } else if (host->step > 4 && host->winner == 0) {
            fn_1_814(0x8C5);
            self->reqMotion = 10;
            self->loopMotion = 0;
            self->sub = 3;
            self->timerA = 0;
        }
        break;
    case 10:
        break;
    case 2:
        if (self->curMotion == 2 && self->timerA == 0x6E) {
            fn_1_814(0x8B3);
        }
        if (--self->timerA <= 0) {
            self->sub = 1;
            self->timerA = 0;
        }
        break;
    case 3: {
        f32 maxTime = Hu3DMotionMaxTimeGet(self->modelId);

        if (Hu3DMotionTimeGet(self->modelId) >= maxTime) {
            Hu3DMotionTimeSet(self->modelId, 40.0f);
        }
        break;
    }
    }
}

static void fn_1_BDA4(Koopa *self) {
    KpHost *host = self->host;
    CAMERA *cam = &host->camera[0];
    M562Player *player = &host->player;
    HU3DMODELID model;
    Vec hit;
    Mtx rotMtx;
    Vec local;
    Vec world;
    f32 t;

    if (self->active == 0) {
        if (self->sub >= 2 && self->sub < 7) {
            self->sub = 2;
            self->timer10 = 1;
        }
    } else if (self->sub == 1) {
        self->timerC = 0xB4;
        self->sub = 3;
    }
    switch (self->sub) {
    case 0:
        model = self->modelId;
        Hu3DModelCameraSet(model, (u16)cam->camMask);
        Hu3DModelAttrReset(model, 1);
        Hu3DModelShadowSet(model);
        Hu3DMotionSet(model, self->motionId[0]);
        Hu3DMotionSpeedSet(model, 1.0f);
        self->pos = lbl_1_data_83C;
        Hu3DModelPosSet(model, self->pos.x, self->pos.y, self->pos.z);
        Hu3DModelRotSet(model, 0.0f, 0.0f, 0.0f);
        self->curMotion = 0;
        self->reqMotion = 0;
        self->loopMotion = 1;
        if (host->diff == 0) {
            self->speed = 6.0f;
        } else if (host->diff == 1) {
            self->speed = 5.5f;
        } else if (host->diff == 2) {
            self->speed = 5.0f;
        } else {
            self->speed = 4.5f;
        }
        self->baseSpeed = self->speed;
        self->timerC = 0xB4;
        self->sub = 3;
        break;
    case 2:
        fn_1_CE10(self);
        if (self->cell != NULL && self->cell->state > 2) {
            self->velY = 0.0f;
            self->timerC = 0;
            self->sub = 7;
        }
        if (--self->timer10 <= 0) {
            self->sub = 3;
        }
        break;
    case 3:
        fn_1_CE24(self);
        if (--self->timerC <= 0) {
            s32 roll = frand() % 10000;

            if (roll < 0x7D0) {
                self->timerC = 0;
                self->sub = 4;
                self->step = 0;
                self->timerE = 0;
            } else if (roll < 0xFA0) {
                self->timerC = 0;
                self->sub = 5;
                self->step = 0;
                self->timerE = 0;
            } else {
                self->timerC = 0;
                self->sub = 6;
                self->step = 0;
                self->timerE = 0;
            }
        }
        break;
    case 4:
        if (fn_1_D404(self) != 0) {
            self->timerC = 0xB4;
            self->timer10 = 0x3C;
            self->sub = 2;
        }
        break;
    case 5:
        if (fn_1_DCC4(self) != 0) {
            self->timerC = 0xB4;
            self->timer10 = 0x3C;
            self->sub = 2;
        }
        break;
    case 6:
        if (fn_1_E558(self) != 0) {
            self->timerC = 0xB4;
            self->timer10 = 0x3C;
            self->sub = 2;
            if (self->cell != NULL && self->cell->state > 2) {
                self->velY = 0.0f;
                self->timerC = 0;
                self->sub = 7;
            }
        }
        break;
    case 7:
        self->reqMotion = 3;
        self->loopMotion = 0;
        if (self->timerC == 0x14) {
            fn_1_814(0x2CB);
        }
        if (self->timerC >= 0x14) {
            self->velY -= 1.5f;
            if (self->timerC >= 0x19) {
                f32 fall = (f32)(self->timerC - 0x19) / 20.0f;
                f32 scale;

                fall = KP_CLAMP(fall, 0.0f, 1.0f);
                scale = 0.6f + 0.4f * (1.0f - fall);
                Hu3DModelScaleSet(self->modelId, 1.5f * scale, 1.5f * scale, 1.5f * scale);
            }
        }
        self->pos.y += self->velY;
        Hu3DModelPosSet(self->modelId, self->pos.x, self->pos.y, self->pos.z);
        self->timerC++;
        if (self->pos.y < -150.0f) {
            Hu3DModelShadowReset(self->modelId);
        }
        if (self->pos.y < -5000.0f) {
            Hu3DModelAttrSet(self->modelId, 1);
            self->timerC = 0;
            self->sub = 8;
        }
        break;
    }
    if (self->cell != NULL) {
        if (self->cell->state > 2 && self->sub != 7 && self->sub != 0 && self->sub != 8 && self->sub != 1) {
            self->velY = 0.0f;
            self->timerC = 0;
            self->sub = 7;
        }
        if (self->cell->state > 2) {
            Vec p;
            f32 minX;
            f32 maxX;
            f32 minZ;
            f32 maxZ;

            p = player->pos;
            minX = self->cell->pos.x - 175.0f;
            maxX = 175.0f + self->cell->pos.x;
            minZ = self->cell->pos.z - 175.0f;
            maxZ = 175.0f + self->cell->pos.z;
            if (p.x > minX && p.x < maxX && p.z > minZ && p.z < maxZ) {
                f32 pushX;
                f32 pushZ;
                f32 dMaxX = maxX - p.x;
                f32 dMinX = minX - p.x;
                f32 dMaxZ = maxZ - p.z;
                f32 dMinZ = minZ - p.z;

                if (KP_ABS(dMinX) < KP_ABS(dMaxX)) {
                    pushX = 0.4f * dMinX;
                } else {
                    pushX = 0.4f * dMaxX;
                }
                if (KP_ABS(dMinZ) < KP_ABS(dMaxZ)) {
                    pushZ = 0.4f * dMinZ;
                } else {
                    pushZ = 0.4f * dMaxZ;
                }
                if (KP_ABS(pushX) < KP_ABS(pushZ)) {
                    p.x += pushX;
                } else {
                    p.z += pushZ;
                }
            }
            player->pos = p;
        }
    }
    model = self->modelId;
    self->pos.x = KP_CLAMP(self->pos.x, -525.0f, 525.0f);
    self->pos.z = KP_CLAMP(self->pos.z, -400.0f, 400.0f);
    Hu3DModelPosSet(model, self->pos.x, self->pos.y, self->pos.z);
    Hu3DModelRotSet(model, self->rot.x, self->rot.y, self->rot.z);
    t = Hu3DMotionTimeGet(self->modelId);
    switch (self->curMotion) {
    case 12:
    case 13:
        if (self->prevTime < 10.0f && t >= 10.0f) {
            if (lbl_1_bss_7820 == 0) {
                fn_1_814(0x2C7);
                lbl_1_bss_7820 = 0xF;
            }
            fn_1_21B68(cam, 1, 0x32, 10.0f, 0.75f, 90.0f);
            MTXRotRad(rotMtx, 'Y', 0.017453292f * self->rotY);
            local = lbl_1_rodata_360;
            MTXMultVec(rotMtx, &local, &world);
            VECAdd(&world, &self->pos, &world);
            fn_1_2DC08((s16)cam->camMask, &world, 1);
        } else if (self->prevTime < 60.0f && t >= 60.0f) {
            if (lbl_1_bss_7820 == 0) {
                fn_1_814(0x2C7);
                lbl_1_bss_7820 = 0xF;
            }
            fn_1_21B68(cam, 1, 0x32, 10.0f, 0.75f, 90.0f);
            MTXRotRad(rotMtx, 'Y', 0.017453292f * self->rotY);
            local = lbl_1_rodata_36C;
            MTXMultVec(rotMtx, &local, &world);
            VECAdd(&world, &self->pos, &world);
            fn_1_2DC08((s16)cam->camMask, &world, 1);
        }
        break;
    case 8:
        if (self->prevTime < 1.0f && t >= 1.0f) {
            if (lbl_1_bss_7820 == 0) {
                fn_1_814(0x2C8);
                lbl_1_bss_7820 = 0xA;
            }
            fn_1_21B68(cam, 1, 0x32, 10.0f, 0.75f, 90.0f);
            MTXRotRad(rotMtx, 'Y', 0.017453292f * self->rotY);
            local = lbl_1_rodata_378;
            MTXMultVec(rotMtx, &local, &world);
            VECAdd(&world, &self->pos, &world);
            fn_1_2DC08((s16)cam->camMask, &world, 1);
        } else if (self->prevTime < 16.0f && t >= 16.0f) {
            if (lbl_1_bss_7820 == 0) {
                fn_1_814(0x2C8);
                lbl_1_bss_7820 = 0xA;
            }
            fn_1_21B68(cam, 1, 0x32, 10.0f, 0.75f, 90.0f);
            MTXRotRad(rotMtx, 'Y', 0.017453292f * self->rotY);
            local = lbl_1_rodata_384;
            MTXMultVec(rotMtx, &local, &world);
            VECAdd(&world, &self->pos, &world);
            fn_1_2DC08((s16)cam->camMask, &world, 1);
        }
        break;
    }
    self->prevTime = t;
    if (lbl_1_bss_7820 != 0) {
        lbl_1_bss_7820--;
    }
    hit.x = self->pos.x;
    hit.y = self->pos.y;
    hit.z = self->pos.z;
    hit.y += 200.0f;
    fn_1_2CE8C(KP_HITA(host), 3, 0, &hit, self, 200.0f);
    {
        f32 radius = 0.5f * player->height;

        hit = player->pos;
        hit.y += radius;
        fn_1_2CE8C(KP_HITA(host), 0, 0, &hit, player, radius);
    }
    fn_1_2CE44(KP_HITA(host));
    if (self->sub != 6 && self->sub < 7) {
        if (self->pos.y > 0.0f) {
            self->velY -= 1.5f;
            self->pos.y += self->velY;
            return;
        }
        self->velY = 0.0f;
        self->pos.y = 0.0f;
    }
}

void fn_1_CE10(Koopa *self) {
    self->reqMotion = 0;
    self->loopMotion = 1;
}

/* approach the player along the arena floor */
void fn_1_CE24(Koopa *self) {
    KpHost *host = self->host;
    M562Player *player = &host->player;
    Vec goal;
    Vec dir;
    f32 ramp;
    f32 speed;
    f32 t;

    self->reqMotion = 12;
    self->loopMotion = 1;
    ramp = (self->speed - 4.5f) * 0.25f;
    ramp = KP_CLAMP(ramp, 0.0f, 1.0f);
    Hu3DMotionSpeedSet(self->modelId, (f32)(1.0 + (f64)ramp));
    fn_1_ED90(self, &player->pos, &goal);
    VECSubtract(&goal, &self->pos, &dir);
    dir.y = 0.0f;
    if (dir.x != 0.0f || dir.z != 0.0f) {
        VECNormalize(&dir, &dir);
    }
    self->goalRotY = KP_HEADING(dir.x, dir.z);
    self->rotY = fn_1_36EC(self->goalRotY, self->rotY, 0.98f);
    speed = self->speed;
    t = Hu3DMotionTimeGet(self->modelId);
    if (KP_ABS(t - 15.0f) < 10.0f) {
        speed *= 0.3f + 0.7f * (KP_ABS(t - 15.0f) / 10.0f);
    } else if (KP_ABS(t - 65.0f) < 10.0f) {
        speed *= 0.3f + 0.7f * (KP_ABS(t - 65.0f) / 10.0f);
    }
    dir.x = (f32)((f64)speed * HuSin(self->rotY));
    dir.z = (f32)((f64)speed * HuCos(self->rotY));
    self->pos.x += dir.x;
    self->pos.z += dir.z;
    self->rot.y = self->rotY;
}

/* spit mini mecha-koopas while chasing the player */
s32 fn_1_D404(Koopa *self) {
    KpHost *host = self->host;
    M562Player *player = &host->player;
    Mtx mtx;
    Vec spawn;
    Vec goal;
    Vec dir;
    f32 ramp;
    f32 speed;
    f32 t;
    f32 ang;
    s32 done;

    ramp = (self->speed - 4.5f) * 0.25f;
    ramp = KP_CLAMP(ramp, 0.0f, 1.0f);
    Hu3DMotionSpeedSet(self->modelId, (f32)(1.0 + (f64)ramp));
    done = 0;
    switch (self->step) {
    case 0:
        self->reqMotion = 6;
        self->loopMotion = 0;
        self->step++;
        break;
    case 1:
        if (self->curMotion == 6) {
            f32 maxTime = Hu3DMotionMaxTimeGet(self->modelId);

            if (Hu3DMotionTimeGet(self->modelId) >= maxTime) {
                self->reqMotion = 13;
                self->loopMotion = 1;
                self->timerE = 0xB4;
                self->step++;
            }
        }
        break;
    case 2:
        if (self->timerE % 3 == 0) {
            Hu3DModelObjMtxGet(self->modelId, lbl_1_data_86C, mtx);
            Hu3DMtxTransGet(mtx, &spawn);
            dir.x = mtx[0][0];
            dir.y = 0.0f;
            dir.z = mtx[2][0];
            ang = self->rotY;
            if (dir.x != 0.0f || dir.z != 0.0f) {
                VECNormalize(&dir, &dir);
                ang = KP_HEADING(dir.x, dir.z);
            }
            fn_1_181FC(&host->mecha, &spawn, ang);
        }
        if (--self->timerE <= 0) {
            done = 1;
        }
        break;
    }
    fn_1_ED90(self, &player->pos, &goal);
    VECSubtract(&goal, &self->pos, &dir);
    dir.y = 0.0f;
    if (dir.x != 0.0f || dir.z != 0.0f) {
        VECNormalize(&dir, &dir);
    }
    self->goalRotY = KP_HEADING(dir.x, dir.z);
    if (self->step > 1) {
        speed = self->speed;
        t = Hu3DMotionTimeGet(self->modelId);
        if (KP_ABS(t - 15.0f) < 10.0f) {
            speed *= 0.3f + 0.7f * (KP_ABS(t - 15.0f) / 10.0f);
        } else if (KP_ABS(t - 65.0f) < 10.0f) {
            speed *= 0.3f + 0.7f * (KP_ABS(t - 65.0f) / 10.0f);
        }
        dir.x = (f32)((f64)speed * HuSin(self->rotY));
        dir.z = (f32)((f64)speed * HuCos(self->rotY));
        self->pos.x += dir.x;
        self->pos.z += dir.z;
    }
    self->rotY = fn_1_36EC(self->goalRotY, self->rotY, 0.98f);
    self->rot.y = self->rotY;
    return done;
}

/* charge at the player and slam into the arena wall */
s32 fn_1_DCC4(Koopa *self) {
    KpHost *host = self->host;
    CAMERA *cam = &host->camera[0];
    M562Player *player = &host->player;
    Vec dir;
    Vec fwd;
    s32 done;
    s32 hitWall;

    Hu3DMotionSpeedSet(self->modelId, 1.0f);
    done = 0;
    switch (self->step) {
    case 0:
        fn_1_ED90(self, &player->pos, &self->target);
        VECSubtract(&self->target, &self->pos, &dir);
        dir.y = 0.0f;
        if (dir.x != 0.0f || dir.z != 0.0f) {
            VECNormalize(&dir, &dir);
            fwd.x = (f32)HuSin(self->rotY);
            fwd.y = 0.0f;
            fwd.z = (f32)HuCos(self->rotY);
            if (VECDotProduct(&dir, &fwd) > cos(0.05235987755982988)) {
                self->step = 2;
            } else {
                self->step = 1;
            }
            self->timerE = 0;
        } else {
            done = 1;
        }
        break;
    case 1:
        VECSubtract(&self->target, &self->pos, &dir);
        dir.y = 0.0f;
        if (dir.x != 0.0f || dir.z != 0.0f) {
            VECNormalize(&dir, &dir);
            fwd.x = (f32)HuSin(self->rotY);
            fwd.y = 0.0f;
            fwd.z = (f32)HuCos(self->rotY);
            if (VECDotProduct(&dir, &fwd) > cos(0.05235987755982988)) {
                self->timerE = 0;
                self->step = 2;
            } else {
                self->goalRotY = KP_HEADING(dir.x, dir.z);
                self->reqMotion = 12;
                self->loopMotion = 1;
            }
        } else {
            done = 1;
        }
        break;
    case 2:
        VECSubtract(&self->target, &self->pos, &dir);
        dir.y = 0.0f;
        if (dir.x != 0.0f || dir.z != 0.0f) {
            VECNormalize(&dir, &dir);
        } else {
            dir.x = (f32)HuSin(self->rotY);
            dir.z = (f32)HuCos(self->rotY);
        }
        self->dir = dir;
        self->reqMotion = 8;
        self->loopMotion = 1;
        self->timerE = 0;
        self->step++;
        break;
    case 3:
        self->timerE++;
        VECNormalize(&self->dir, &dir);
        self->goalRotY = KP_HEADING(dir.x, dir.z);
        dir.x = 1.6f * (10.0f * self->dir.x);
        dir.z = 1.6f * (10.0f * self->dir.z);
        self->pos.x += dir.x;
        self->pos.z += dir.z;
        self->reqMotion = 8;
        self->loopMotion = 1;
        break;
    }
    self->rotY = fn_1_36EC(self->goalRotY, self->rotY, 0.9f);
    self->rot.y = self->rotY;
    hitWall = 0;
    if (self->pos.z <= -400.0f) {
        hitWall = 1;
    } else if (self->pos.z >= 400.0f) {
        hitWall = 1;
    }
    if (self->pos.x <= -525.0f) {
        hitWall = 1;
    } else if (self->pos.x >= 525.0f) {
        hitWall = 1;
    }
    if (self->step < 3 || self->timerE < 10) {
        hitWall = 0;
    }
    if (hitWall != 0) {
        fn_1_21B68(cam, 1, 0x32, 40.0f, 0.95f, 90.0f);
        omVibrate(player->playerNo, 0x14, 0xC, 0);
        fn_1_814(0x8A6);
        done = 1;
    }
    return done;
}

/* leap onto the floor tile under the player */
s32 fn_1_E558(Koopa *self) {
    KpHost *host = self->host;
    CAMERA *cam = &host->camera[0];
        M562Player *player = &host->player;
    Vec dir;
    Vec fwd;
    s32 done;

    Hu3DMotionSpeedSet(self->modelId, 1.0f);
    done = 0;
    switch (self->step) {
    case 0:
        self->cell = &host->stage.tile[(s16)((0.99f * (125.0f + (player->pos.x - -500.0f))) / 250.0f) +
                                  (s16)((0.99f * (125.0f + (player->pos.z - -375.0f))) / 250.0f) * 5];
        self->cellPos = self->cell->pos;
        VECSubtract(&self->cellPos, &self->pos, &dir);
        dir.y = 0.0f;
        if (dir.x != 0.0f || dir.z != 0.0f) {
            VECNormalize(&dir, &dir);
            fwd.x = (f32)HuSin(self->rotY);
            fwd.y = 0.0f;
            fwd.z = (f32)HuCos(self->rotY);
            if (VECDotProduct(&dir, &fwd) > cos(0.08726646259971647)) {
                self->step = 2;
            } else {
                self->step = 1;
            }
        } else {
            done = 1;
            break;
        }
        /* fallthrough */
    case 1:
        VECSubtract(&self->cellPos, &self->pos, &dir);
        dir.y = 0.0f;
        if (dir.x != 0.0f || dir.z != 0.0f) {
            VECNormalize(&dir, &dir);
            fwd.x = (f32)HuSin(self->rotY);
            fwd.y = 0.0f;
            fwd.z = (f32)HuCos(self->rotY);
            if (VECDotProduct(&dir, &fwd) > cos(0.08726646259971647)) {
                self->step = 2;
            } else {
                self->goalRotY = KP_HEADING(dir.x, dir.z);
                self->reqMotion = 12;
                self->loopMotion = 1;
            }
        } else {
            done = 1;
        }
        break;
    case 2:
        self->jumpFrom = self->pos;
        self->reqMotion = 3;
        self->loopMotion = 0;
        self->timerE = 0;
        self->step++;
        break;
    case 3:
        if (self->timerE >= 0x14 && self->timerE <= 0x3C) {
            f32 prog = (f32)(self->timerE - 0x14) / 40.0f;

            prog = KP_CLAMP(prog, 0.0f, 1.0f);
            self->pos.y = 500.0f * (f32)HuSin(180.0f * prog);
            self->pos.x = self->jumpFrom.x + prog * (self->cellPos.x - self->jumpFrom.x);
            self->pos.z = self->jumpFrom.z + prog * (self->cellPos.z - self->jumpFrom.z);
        } else if (self->timerE == 0x3D) {
            self->cell->flag = 1;
            host->stage.quakePos = self->cellPos;
            host->stage.quakeTimer = 1;
            fn_1_21B68(cam, 1, 0x32, 40.0f, 0.95f, 90.0f);
            fn_1_2DC08((s16)cam->camMask, &self->pos, 0);
            omVibrate(player->playerNo, 0x14, 0xC, 0);
            fn_1_814(0x8A7);
        }
        self->timerE++;
        if (self->timerE >= 0x50) {
            if (host->diff == 0 || host->diff == 1) {
                self->speed += 0.5f;
                self->speed = KP_CLAMP(self->speed, 0.0f, 8.5f);
            }
            done = 1;
        }
        break;
    }
    self->rotY = fn_1_36EC(self->goalRotY, self->rotY, 0.9f);
    self->rot.y = self->rotY;
    return done;
}

/* keep a point inside the arena (corner tiles of the stage table) */
void fn_1_ED90(Koopa *self, Vec *in, Vec *out) {
    KpHost *host = self->host;
    f32 minX = host->stage.tile[0].pos.x;
    f32 minZ = host->stage.tile[0].pos.z;
    f32 maxX = host->stage.tile[19].pos.x;
    f32 maxZ = host->stage.tile[19].pos.z;

    out->x = (in->x > maxX) ? maxX : ((in->x < minX) ? minX : in->x);
    out->y = in->y;
    out->z = (in->z > maxZ) ? maxZ : ((in->z < minZ) ? minZ : in->z);
}

/* hit callback between two collision entries */
void fn_1_EEC4(EffectEntry *a, EffectEntry *b) {
    M562Player *player;

    if (a->type == 0 && b->type == 3) {
        fn_1_F014(a->user, b->user);
        return;
    }
    if (a->type == 3 && b->type == 0) {
        fn_1_F014(b->user, a->user);
        return;
    }
    if (a->type == 4 && b->type == 0) {
        player = b->user;
        if ((player->state == 1 || player->state == 2) && player->invuln == 0) {
            player->timerA = 0;
            player->sub = 0;
            player->subTimer = 0;
            player->state = 4;
        }
        return;
    }
    if (a->type == 0 && b->type == 4) {
        player = a->user;
        if ((player->state == 1 || player->state == 2) && player->invuln == 0) {
            player->timerA = 0;
            player->sub = 0;
            player->subTimer = 0;
            player->state = 4;
        }
    }
}

/* push the player out of Bowser and hurt it when he is charging */
void fn_1_F014(M562Player *player, Koopa *koopa) {
    CAMERA *cam = &player->owner->camera[0];
    f32 pLow;
    f32 pHigh;
    f32 kLow;
    f32 kHigh;

    pLow = player->pos.y;
    pHigh = pLow + player->height;
    kLow = koopa->pos.y;
    kHigh = 300.0f + kLow;
    if ((pHigh == kHigh && pLow == kLow) || (pHigh <= kHigh && pHigh >= kLow) ||
        (pLow <= kHigh && pLow >= kLow) || (kHigh <= pHigh && kHigh >= pLow) ||
        (kLow <= pHigh && kLow >= pLow)) {
        Vec d;
        f32 dist;

        d.x = player->pos.x - koopa->pos.x;
        d.y = 0.0f;
        d.z = player->pos.z - koopa->pos.z;
        dist = VECMag(&d);
        if (dist < 230.0f) {
            f32 pMid;
            f32 kMid;
            f32 pHalf;
            f32 kHalf;
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
            koopa->goalRotY = KP_HEADING(-d.x, -d.z);
            VECScale(&d, &d, 230.0f - dist);
            pMid = 0.5f * (pLow + pHigh);
            kMid = 0.5f * (kLow + kHigh);
            pHalf = KP_ABS(pMid - pLow);
            kHalf = KP_ABS(kMid - kLow);
            big = (pHalf > kHalf) ? pHalf : kHalf;
            small = (pHalf > kHalf) ? kHalf : pHalf;
            gap = KP_ABS(pMid - kMid);
            if (gap < big) {
                weight = 1.0f;
            } else {
                weight = 1.0f - ((gap - big) / small);
            }
            player->pos.x += 0.9f * d.x * weight;
            player->pos.z += 0.9f * d.z * weight;
            koopa->pos.x -= 0.1f * d.x * weight;
            koopa->pos.z -= 0.1f * d.z * weight;
            if (koopa->sub != 7) {
                if (koopa->sub == 3) {
                    koopa->timer10 = 0x78;
                    koopa->sub = 2;
                } else if (koopa->sub == 4) {
                    koopa->timer10 = 0x78;
                    koopa->sub = 2;
                } else if (koopa->sub == 6) {
                    if (koopa->step >= 3 && koopa->timerE >= 0x14) {
                        if (kLow >= pLow && (player->state == 1 || player->state == 2) && player->invuln == 0) {
                            player->scaleTimer = 0x8C;
                            player->invuln = 0x8C;
                            player->hp--;
                            omVibrate(player->playerNo, 0x1E, 4, 2);
                            CharFXPlay(player->charNo, 0x202);
                        }
                        fn_1_21B68(cam, 1, 0x32, 40.0f, 0.95f, 90.0f);
                        koopa->timerC = 0xB4;
                        koopa->timer10 = 0x3C;
                        koopa->sub = 2;
                    }
                }
                if ((player->state == 1 || player->state == 2) && player->invuln == 0) {
                    if (koopa->sub == 5) {
                        player->timerA = 0;
                        player->sub = 0;
                        player->subTimer = 0;
                        player->state = 6;
                        koopa->timerC = 0xB4;
                        koopa->timer10 = 0x3C;
                        koopa->sub = 2;
                        return;
                    }
                    player->timerA = 0;
                    player->sub = 0;
                    player->subTimer = 0;
                    player->state = 3;
                }
            }
        }
    }
}
