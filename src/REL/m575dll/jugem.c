// REL/m575dll/jugem.c - the cloud-riding helper that fishes up players who fell
// off the stage and drops them back on the tube.
#include "REL/m575dll.h"
#include "game/hu3d.h"
#include "game/data.h"
#include "game/memory.h"
#include "game/frand.h"
#include "game/mg/actman.h"
#include "string.h"
#include "math.h"
#include "humath.h"

void fn_1_5C88(M575Player *self, s32 on);
static void fn_1_CD90(M575Jugem *self, s32 grab);

char lbl_1_data_2E0[16] = "itemhook_sao";

void fn_1_C464(M575Jugem *self, M575Main *main)
{
    s16 id;

    memset(self, 0, sizeof(M575Jugem));
    self->main = main;
    id = Hu3DModelCreateData(0x82000C);
    self->modelId = id;
    Hu3DModelPosSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(id, main->camera.camBit);
    Hu3DModelLayerSet(id, 3);
    Hu3DModelAttrSet(id, 1);
    Hu3DModelShadowSet(id);
    self->motion = Hu3DJointMotionData(id, 0x820013);
    self->motion2 = Hu3DJointMotionData(id, 0x820014);
    Hu3DMotionSet(id, self->motion);
    Hu3DMotionSpeedSet(id, 1.0f);
    Hu3DModelAttrSet(id, 0x40000001);
}

void fn_1_C5E0(M575Jugem *self)
{
    Hu3DMotionKill(self->motion2);
    Hu3DMotionKill(self->motion);
    Hu3DModelKill(self->modelId);
    memset(self, 0, sizeof(M575Jugem));
}

void fn_1_C630(M575Jugem *self)
{
    M575Player *held = self->held;
    M575Main *side = self->main;
    M575Ten *ten = &side->ten;
    M575Player *other;
    Vec d;
    Vec p;
    Vec r;
    Vec rr;
    Vec unused;
    Mtx mtx;
    f32 ang;
    f32 len;
    f32 c;
    f64 cosA;
    f64 cosB;

    if (self->pos.y < -400.0f) {
        Hu3DModelShadowDispOff(self->modelId);
    }
    else {
        Hu3DModelShadowDispOn(self->modelId);
    }
    switch (self->state) {
    case 1:
        if (held == NULL) {
            self->state = 0;
        }
        else {
            c = held->pos->x;
            self->target.x = c;
            self->pos.x = c;
            self->target.y = 2000.0f;
            self->pos.y = 2000.0f;
            self->target.z = -600.0f;
            self->pos.z = -600.0f;
            Hu3DModelPosSet(self->modelId, self->pos.x, self->pos.y, self->pos.z);
            Hu3DModelAttrReset(self->modelId, 1);
            Hu3DModelObjPosGet(self->modelId, lbl_1_data_2E0, &d);
            PSVECSubtract(&d, &self->pos, &self->offset);
            self->offset.y -= 120.0f;
            self->state++;
        }
        break;
    case 2:
        self->target.y -= 80.0f;
        if (self->pos.y < -3000.0f) {
            self->timer = 0;
            self->state++;
        }
        break;
    case 3:
        fn_1_CD90(self, 1);
        self->timer = 0;
        self->state++;
        break;
    case 4:
        if (held == NULL) {
            self->state = 0;
        }
        else {
            self->target.y += 70.0f;
            if (&side->player[0] == held) {
                other = &side->player[1];
            }
            else {
                other = &side->player[0];
            }
            if (self->pos.y > 100.0f) {
                if (other->falling != 0 || (other->vel.x == 0.0f && other->vel.z == 0.0f)) {
                    ang = 360.0f * ((f32)(frand() % 10000) / 10000.0f);
                    unused.x = 0.0f;
                    unused.y = 0.0f;
                    unused.z = -1.0f;
                    r = unused;
                    PSMTXRotRad(mtx, 'Y', 0.017453292f * ang);
                    PSMTXMultVec(mtx, &r, &r);
                    PSVECScale(&r, &r, 150.0f);
                    self->dest.x = r.x;
                    self->dest.y = 400.0f;
                    self->dest.z = r.z;
                }
                else {
                    rr.x = other->vel.x;
                    rr.y = other->vel.y;
                    rr.z = other->vel.z;
                    rr.y = 0.0f;
                    PSVECScale(&rr, &rr, -1.0f);
                    len = PSVECMag(&rr);
                    if (len < 50.0f) {
                        PSVECScale(&rr, &rr, 50.0f / len);
                    }
                    else if (len > 150.0f) {
                        PSVECScale(&rr, &rr, 150.0f / len);
                    }
                    self->dest.x = rr.x;
                    self->dest.y = 400.0f;
                    self->dest.z = rr.z;
                }
                self->timer = 0;
                self->state++;
            }
        }
        break;
    case 5:
        cosA = HuCos(ten->rot);
        cosA = (cosA >= 0.0) ? cosA : -cosA;
        d.x = (f32)((f64)self->dest.x * cosA);
        d.y = self->dest.y;
        cosB = HuCos(ten->rot);
        cosB = (cosB >= 0.0) ? cosB : -cosB;
        d.z = (f32)((f64)self->dest.z * cosB);
        PSVECAdd(&self->target, &self->offset, &p);
        PSVECSubtract(&d, &p, &r);
        PSVECNormalize(&r, &r);
        PSVECScale(&r, &r, 20.0f);
        PSVECAdd(&self->target, &r, &self->target);
        PSVECAdd(&self->pos, &self->offset, &p);
        PSVECSubtract(&d, &p, &r);
        if (PSVECMag(&r) < 40.0f) {
            fn_1_CD90(self, 0);
            self->held = NULL;
            self->timer = 0;
            self->state++;
        }
        break;
    case 6:
        self->target.y += 40.0f;
        if (self->pos.z > -600.0f) {
            self->target.z -= 20.0f;
        }
        if (self->pos.y > 2000.0f) {
            Hu3DModelAttrSet(self->modelId, 1);
            self->timer = 0;
            self->state = 0;
        }
        break;
    }
    PSVECSubtract(&self->target, &self->pos, &r);
    PSVECScale(&r, &r, 0.1f);
    PSVECAdd(&self->pos, &r, &self->pos);
    Hu3DModelPosSet(self->modelId, self->pos.x, self->pos.y, self->pos.z);
}

// grab != 0: hook the fallen player onto the cloud; otherwise let go and put the
// player back at the hook's position.
static void fn_1_CD90(M575Jugem *self, s32 grab)
{
    M575Player *held = self->held;
    Vec at;

    if (held != NULL) {
        if (grab != 0) {
            fn_1_5C88(held, 0);
            Hu3DMotionShiftSet(held->modelId, held->motion[6], 0.0f, 4.0f, 0x40000001);
            Hu3DMotionShiftSet(self->modelId, self->motion2, 0.0f, 4.0f, 0);
            self->holding = 1;
            Hu3DModelHookSet(self->modelId, lbl_1_data_2E0, held->modelId);
            Hu3DModelPosSet(held->modelId, 0.0f, -120.0f, 0.0f);
            Hu3DModelRotSet(held->modelId, 0.0f, 0.0f, 0.0f);
            Hu3DModelScaleSet(held->modelId, 1.0f, 1.0f, 1.0f);
            Hu3DModelAttrReset(held->modelId, 1);
            return;
        }
        Hu3DModelObjPosGet(self->modelId, lbl_1_data_2E0, &at);
        PSVECSubtract(&at, &self->pos, &self->offset);
        self->offset.y -= 120.0f;
        PSVECAdd(&self->pos, &self->offset, held->pos);
        MgActorPosUpdate(held->mgPlayer->actor, (HuVecF *)held->pos);
        Hu3DModelHookReset(self->modelId);
        fn_1_5C88(held, 1);
        MgPlayerModeLandSet(held->mgPlayer, 0);
        Hu3DMotionShiftSet(self->modelId, self->motion, 0.0f, 4.0f, 0x40000001);
        held->mgPlayer->actor->velY = 0.0f;
        held->mgPlayer->actor->rotY = held->rot->y;
        MgPlayerModeSet(held->mgPlayer, 2);
        held->falling = 0;
        self->holding = 0;
        self->held = NULL;
    }
}

// Called when a player starts to fall: remembers whom to fish up and wakes the cloud.
void fn_1_CFE4(M575Jugem *self, M575Player *player)
{
    if (self->held == NULL) {
        self->held = player;
    }
    if (self->state == 0) {
        self->state = 1;
    }
}
