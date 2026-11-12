#include "REL/m575dll.h"
#include "game/hu3d.h"
#include "game/data.h"
#include "game/memory.h"
#include "game/frand.h"
#include "game/mg/actman.h"
#include "string.h"
#include "math.h"
#include "humath.h"

float Hu3DMotionMaxTimeGet(HU3DMODELID modelId);
void Hu3DMotionTimeSet(HU3DMODELID modelId, float time);
s32 fn_1_530(s32 seId);
s32 fn_1_AAAC(M575Tubo *self, M575Player *player);

static const f32 lbl_1_rodata_450[1] = { 0.0f };
static const f32 lbl_1_rodata_454[1] = { 320.0f };
static const f32 lbl_1_rodata_458[1] = { 1.0f };
static const f32 lbl_1_rodata_45C[1] = { 500.0f };

#define TUBO_DATA(n) ((0x82 << 16) + (n))

// NONMATCHING: 83.71%. Every instruction is the one the original object has,
// but the original keeps one more value alive across the prologue (a copy of
// conf, read once for the unk4 test) and so allocates r27..r31 where this
// version allocates a different permutation of the same five registers.
void fn_1_9BC8(M575Tubo *self, M575Main *conf)
{
    M575Camera *tenConf = &conf->camera;
    s16 modelId1;
    s16 modelId2;

    memset(self, 0, sizeof(*self));
    self->main = conf;

    if (conf->side == 0) {
        modelId1 = Hu3DModelCreateData(TUBO_DATA(9));
        self->modelId1 = modelId1;
    }
    else {
        modelId1 = Hu3DModelCreateData(TUBO_DATA(8));
        self->modelId1 = modelId1;
    }

    Hu3DModelPosSet(modelId1, lbl_1_rodata_450[0], lbl_1_rodata_454[0], lbl_1_rodata_450[0]);
    Hu3DModelRotSet(modelId1, lbl_1_rodata_450[0], lbl_1_rodata_450[0], lbl_1_rodata_450[0]);
    Hu3DModelCameraSet(modelId1, tenConf->camBit);
    Hu3DModelLayerSet(modelId1, 0);
    Hu3DModelAttrReset(modelId1, 1);

    self->unkC = (Vec *)&Hu3DData[modelId1].pos;
    self->unk10 = &Hu3DData[modelId1].mtx[0][0];
    Hu3DModelShadowSet(modelId1);

    modelId2 = Hu3DModelCreateData(TUBO_DATA(0xD));
    self->modelId2 = modelId2;

    Hu3DModelPosSet(modelId2, lbl_1_rodata_450[0], lbl_1_rodata_454[0], lbl_1_rodata_450[0]);
    Hu3DModelRotSet(modelId2, lbl_1_rodata_450[0], lbl_1_rodata_450[0], lbl_1_rodata_450[0]);
    Hu3DModelCameraSet(modelId2, tenConf->camBit);
    Hu3DModelLayerSet(modelId2, 0);
    Hu3DModelAttrSet(modelId2, 1);

    self->unk18 = (Vec *)&Hu3DData[modelId2].pos;
    self->unk1C = &Hu3DData[modelId2].mtx[0][0];

    self->motionId = Hu3DJointMotionData(modelId2, TUBO_DATA(0x17));
    Hu3DMotionSet(modelId2, self->motionId);
    Hu3DMotionSpeedSet(modelId2, lbl_1_rodata_458[0]);
    Hu3DModelAttrReset(modelId2, 0x40000001);

    fn_1_9320(&self->eff, 1, (const Vec *)&lbl_1_rodata_45C,
              lbl_1_rodata_45C[0], lbl_1_rodata_458[0], lbl_1_rodata_450[0]);
    PSMTXIdentity((f32 (*)[4])&self->mtx);
}

void fn_1_9E8C(M575Tubo *self)
{
    fn_1_96F8(&self->eff);
    Hu3DMotionKill(self->motionId);
    Hu3DModelKill(self->modelId2);
    Hu3DModelKill(self->modelId1);
    memset(self, 0, sizeof(*self));
}

// Per-frame update: wobble, tilt the tube back upright, push players, and drop
// a coin every time the tube finishes its bounce.
void fn_1_9EE4(M575Tubo *self)
{
    M575Main *side = self->main;
    M575Game *game = side->game;
    M575TuboEff *eff = &self->eff;
    Vec up0 = { 0.0f, 1.0f, 0.0f };
    Vec up;
    Vec zero = { 0.0f, 0.0f, 0.0f };
    Vec dir;
    Vec v;
    Vec axis;
    Vec spawn;
    Mtx mtx;
    Quaternion q0, q1, q2;
    f32 tilt;
    f32 absTilt;
    f32 ratio;
    f32 clamped;
    f32 lo;
    f32 heading;
    f32 amp;
    f32 ampClamped;
    f32 ampLo;
    f64 ang;
    f64 angTmp;
    u16 kind;

    up = up0;
    switch (self->unk4) {
    case 0:
        self->unk24 = 0.0f;
        self->unk28 = 0.0f;
        break;
    case 1:
        self->unk24 = (f32)HuSin(180.0f * ((f32)self->unk6 / 30.0f));
        self->unk24 = 130.0f * self->unk24;
        PSMTXRotRad(mtx, 'Y', 0.34906584f);
        PSMTXConcat(self->mtx, mtx, self->mtx);
        if (self->unk6++ > 0x1E) {
            PSMTXIdentity(self->mtx);
            self->unk4 = 0;
        }
        break;
    }
    self->unk20 = (f32)(game->frame % 120) / 120.0f;
    self->unk20 = (f32)(15.0 * HuSin(360.0f * self->unk20));
    self->unk20 = self->unk20 + 320.0f;
    fn_1_9728(eff);
    eff->angVel = zero;
    PSVECScale(&eff->pos, &eff->pos, 0.7f);
    PSVECScale(&eff->vel, &eff->vel, 0.7f);
    fn_1_AAAC(self, &side->player[0]);
    fn_1_AAAC(self, &side->player[1]);
    dir.x = eff->mtx[1][0];
    dir.y = eff->mtx[1][1];
    dir.z = eff->mtx[1][2];
    PSVECNormalize(&dir, &dir);
    tilt = (f32)(180.0 * ((f64)(f32)acos(PSVECDotProduct(&dir, &up)) / 3.141592653589793));
    if (tilt > 60.0f) {
        v.x = dir.x;
        v.y = 0.0f;
        v.z = dir.z;
        PSVECNormalize(&v, &v);
        PSVECCrossProduct(&up, &v, &axis);
        PSMTXRotAxisRad(mtx, &axis, 1.0471976f);
        PSMTXMultVec(mtx, &up, &v);
        fn_1_2D5C(&q0, &dir, &v);
        C_QUATMtx(&q1, eff->mtx);
        PSQUATMultiply(&q0, &q1, &q2);
        PSMTXQuat(eff->mtx, &q2);
    }
    if (tilt > 3.0f) {
        absTilt = (tilt >= 0.0f) ? tilt : -tilt;
        ratio = (absTilt - 3.0f) / 60.0f;
        if (ratio > 1.0f) {
            clamped = 1.0f;
        }
        else {
            if (ratio < 0.2f) {
                lo = 0.2f;
            }
            else {
                lo = ratio;
            }
            clamped = lo;
        }
        PSVECSubtract(&up, &dir, &v);
        PSVECNormalize(&v, &v);
        PSVECScale(&v, &v, 0.0001f * clamped);
        PSVECScale(&dir, &dir, 100.0f);
        fn_1_9B58(eff, &v, &dir);
    }
    self->unkC->y = self->unk20 + self->unk24;
    PSMTXConcat(eff->mtx, self->mtx, mtx);
    PSMTXCopy(mtx, (MtxPtr)self->unk10);
    if (self->unk28 < tilt) {
        self->unk28 = tilt;
    }
    if (self->unk4 == 1 && self->unk6 == 0xF && game->mode < 5) {
        v.x = eff->mtx[1][0];
        v.y = eff->mtx[1][1];
        v.z = eff->mtx[1][2];
        if (v.x != 0.0f || v.y != 0.0f || v.z != 0.0f) {
            PSVECNormalize(&v, &v);
        }
        else {
            v.y = 1.0f;
        }
        PSVECScale(&v, &spawn, 100.0f);
        PSVECAdd(&spawn, self->unkC, &spawn);
        v.x = eff->mtx[1][0];
        v.y = eff->mtx[1][1];
        v.z = eff->mtx[1][2];
        v.y = 0.0f;
        if (v.x != 0.0f || v.z != 0.0f) {
            PSVECNormalize(&v, &v);
        }
        else {
            v.z = 1.0f;
        }
        if ((f32)HuAtan(v.x, v.z) > 180.0f) {
            ang = HuAtan(v.x, v.z) - 360.0;
        }
        else {
            if ((f32)HuAtan(v.x, v.z) <= -180.0f) {
                angTmp = 360.0 + HuAtan(v.x, v.z);
            }
            else {
                angTmp = HuAtan(v.x, v.z);
            }
            ang = angTmp;
        }
        heading = (f32)ang + (60.0f * ((f32)(frand() % 10000) / 10000.0f) - 30.0f);
        v.x = (f32)HuSin(heading);
        v.y = 0.0f;
        v.z = (f32)HuCos(heading);
        amp = (self->unk28 >= 0.0f) ? self->unk28 : -self->unk28;
        amp = amp / 45.0f;
        if (amp > 1.0f) {
            ampClamped = 1.0f;
        }
        else {
            if (amp < 0.5f) {
                ampLo = 0.5f;
            }
            else {
                ampLo = amp;
            }
            ampClamped = ampLo;
        }
        PSVECScale(&v, &v, 400.0f * ampClamped);
        kind = 0;
        if (self->unk14A < game->firstSide && self->unk148 >= game->coinThresh[self->unk14A]) {
            kind = 1;
            self->unk14A++;
        }
        fn_1_B240(&side->coin, kind, &spawn, &v, 16.0f);
        self->unk148++;
        Hu3DMotionTimeSet(self->modelId2, 0.0f);
        Hu3DModelAttrReset(self->modelId2, 1);
    }
    if (Hu3DMotionTimeGet(self->modelId2) < Hu3DMotionMaxTimeGet(self->modelId2)) {
        v.x = eff->mtx[1][0];
        v.y = eff->mtx[1][1];
        v.z = eff->mtx[1][2];
        if (v.x != 0.0f || v.y != 0.0f || v.z != 0.0f) {
            PSVECNormalize(&v, &v);
        }
        else {
            v.y = 1.0f;
        }
        PSVECScale(&v, &spawn, 100.0f);
        PSVECAdd(&spawn, self->unkC, &spawn);
        PSMTXCopy(eff->mtx, (MtxPtr)self->unk1C);
        *self->unk18 = spawn;
        return;
    }
    Hu3DModelAttrSet(self->modelId2, 1);
}

// Collision of one player with the tube: standing on top lets the player's
// weight tilt it, anything else is pushed outwards.
s32 fn_1_AAAC(M575Tubo *self, M575Player *player)
{
    M575Main *side = player->main;
    Vec d;
    Vec n;
    Vec impulse;
    Vec at;
    f32 feetY;
    f32 headY;
    f32 tubeY;
    f32 topY;
    f32 dist;
    f32 dy;
    f32 absDy;
    f32 dy2;
    f32 absDy2;
    f32 ratio;
    f32 clamped;
    f32 lo;
    s32 ret;

    feetY = player->pos->y;
    headY = player->pos->y + player->height;
    tubeY = self->unkC->y;
    topY = 120.0f + tubeY;
    ret = 0;
    if ((feetY >= tubeY && headY < topY) || (feetY <= tubeY && headY > topY) || (headY >= tubeY && headY < topY)) {
        d.x = player->pos->x - self->unkC->x;
        d.y = 0.0f;
        d.z = player->pos->z - self->unkC->z;
        dist = PSVECMag(&d);
        if (dist < 130.0f && player->mgPlayer->actor->velY > 0.0f) {
            dy = headY - tubeY;
            absDy = (dy >= 0.0f) ? dy : -dy;
            if (absDy < 130.0f - dist) {
                dy2 = headY - tubeY;
                absDy2 = (dy2 >= 0.0f) ? dy2 : -dy2;
                player->pos->y -= absDy2;
                MgActorPosUpdate(player->mgPlayer->actor, (HuVecF *)player->pos);
                if (self->unk4 == 0) {
                    at = d;
                    impulse.x = 0.0f;
                    impulse.y = 0.001f;
                    impulse.z = 0.0f;
                    fn_1_9B58(&self->eff, &impulse, &at);
                    self->unk4 = 1;
                    self->unk6 = 0;
                    player->timer = 0;
                    if (player->mgPlayer->actor->velY > 0.0f) {
                        ratio = player->height / 210.0f;
                        if (ratio > 1.0f) {
                            clamped = 1.0f;
                        }
                        else {
                            if (ratio < 0.0f) {
                                lo = 0.0f;
                            }
                            else {
                                lo = ratio;
                            }
                            clamped = lo;
                        }
                        player->mgPlayer->actor->velY = -player->mgPlayer->actor->velY * (1.0f - clamped);
                        MgPlayerModeSet(player->mgPlayer, 2);
                    }
                    omVibrate(player->playerNo, 0xF, 6, 6);
                    if (side->side == 0) {
                        fn_1_530(0x934);
                    }
                    else if (side->side == 1) {
                        fn_1_530(0x935);
                    }
                }
            }
            else {
                PSVECNormalize(&d, &n);
                PSVECScale(&n, &n, 130.0f - dist);
                PSVECAdd(player->pos, &n, player->pos);
                MgActorPosUpdate(player->mgPlayer->actor, (HuVecF *)player->pos);
            }
            ret = 1;
        }
    }
    return ret;
}
