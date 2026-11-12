// REL/m575dll/coin.c - the coins the tube throws out (8 per side), their
// bouncing and the pick-up by the two players.
#include "REL/m575dll.h"
#include "game/hu3d.h"
#include "game/hsfex.h"
#include "game/charman.h"
#include "game/object.h"
#include "game/mg/actman.h"
#include "game/mg/colman.h"
#include "string.h"

s32 fn_1_530(s32 seId);
void fn_1_5B0(s32 seNo, s16 pan);
void fn_1_B2E0(M575CoinItem *self, M575Coin *coin);
void fn_1_B324(M575CoinItem *self);
s32 fn_1_B354(M575CoinItem *self);
void fn_1_B91C(M575CoinItem *self, u16 kind, Vec *pos, Vec *vel, f32 height);
void fn_1_BB6C(M575CoinItem *self);
void fn_1_BC18(M575CoinItem *self, M575Ten *ten);
s32 fn_1_C184(M575CoinItem *self, M575Player *player);

void fn_1_AEF0(M575Coin *self, M575Main *main)
{
    s16 id;
    s32 i;

    memset(self, 0, sizeof(M575Coin));
    self->main = main;
    id = Hu3DModelCreateData(0x82000A);
    self->modelId = id;
    Hu3DModelPosSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrSet(id, 1);
    id = Hu3DModelCreateData(0x82000B);
    self->modelId2 = id;
    Hu3DModelPosSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrSet(id, 1);
    for (i = 0; i < 8; i++) {
        fn_1_B2E0(&self->item[i], self);
    }
}

void fn_1_B074(M575Coin *self)
{
    s32 i;

    for (i = 0; i < 8; i++) {
        fn_1_B324(&self->item[i]);
    }
    Hu3DModelKill(self->modelId2);
    Hu3DModelKill(self->modelId);
    memset(self, 0, sizeof(M575Coin));
}

void fn_1_B0E8(M575Coin *self)
{
    M575Main *side = self->main;
    M575Player *p0 = &side->player[0];
    M575Player *p1 = &side->player[1];
    M575CoinItem *item;
    Vec d;
    f32 dist;
    f32 dist2;
    s32 i;

    p0->target = NULL;
    p0->targetDist = 1000000.0f;
    p1->target = NULL;
    p1->targetDist = 1000000.0f;
    for (i = 0; i < 8; i++) {
        item = &self->item[i];
        if (fn_1_B354(item) != 0) {
            fn_1_BB6C(item);
        }
        if (item->active != 0) {
            PSVECSubtract(p0->pos, item->pos, &d);
            d.y = 0.0f;
            dist = PSVECMag(&d);
            if (dist < p0->targetDist || item->kind == 1) {
                p0->target = item;
                p0->targetDist = dist;
            }
            PSVECSubtract(p1->pos, item->pos, &d);
            d.y = 0.0f;
            dist2 = PSVECMag(&d);
            if (dist2 < p1->targetDist || item->kind == 1) {
                p1->target = item;
                p1->targetDist = dist2;
            }
        }
    }
}

// Starts a coin in the first free slot; returns the slot or NULL when all eight are busy.
M575CoinItem *fn_1_B240(M575Coin *self, u16 kind, Vec *pos, Vec *vel, f32 time)
{
    M575CoinItem *found = NULL;
    s32 i;

    for (i = 0; i < 8; i++) {
        if (self->item[i].active == 0) {
            found = &self->item[i];
            fn_1_B91C(found, kind, pos, vel, time);
            break;
        }
    }
    return found;
}

void fn_1_B2E0(M575CoinItem *self, M575Coin *coin)
{
    memset(self, 0, sizeof(M575CoinItem));
    self->coin = coin;
}

void fn_1_B324(M575CoinItem *self)
{
    memset(self, 0, sizeof(M575CoinItem));
}

// Per-frame coin update; returns 1 when the coin is gone and its slot can be freed.
s32 fn_1_B354(M575CoinItem *self)
{
    M575Main *side = self->coin->main;
    M575Game *game = side->game;
    Vec np;
    f32 absH;
    f32 absV;
    f32 ratio;
    f32 clamped;
    f32 lo;
    f32 inv;
    f32 k;
    f32 absPrev;
    f32 absY;
    f32 ratio2;
    f32 clamped2;
    f32 lo2;

    if (self->active == 0) {
        return 0;
    }
    if (game->mode >= 5) {
        if ((self->timer & 1) != 0) {
            Hu3DModelAttrReset(self->modelId, 1);
        }
        else {
            Hu3DModelAttrSet(self->modelId, 1);
        }
        if (self->timer++ > 0x3C) {
            return 1;
        }
        return 0;
    }
    self->vel.y += -0.8f;
    if (self->pos->y < -3000.0f) {
        return 1;
    }
    if (self->bounced == 0) {
        np = *self->pos;
        if (self->vel.y > 0.0f) {
            absH = (self->height >= 0.0f) ? self->height : -self->height;
            absV = (self->vel.y >= 0.0f) ? self->vel.y : -self->vel.y;
            ratio = absV / absH;
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
            inv = 1.0f - clamped;
            np.x = self->start.x + inv * (0.2f * (self->dest.x - self->start.x));
            np.z = self->start.z + inv * (0.2f * (self->dest.z - self->start.z));
            self->prevY = self->pos->y;
        }
        else {
            if (self->pos->y < 0.0f) {
                k = 1.0f;
            }
            else {
                absPrev = (self->prevY >= 0.0f) ? self->prevY : -self->prevY;
                absY = (self->pos->y >= 0.0f) ? self->pos->y : -self->pos->y;
                ratio2 = absY / absPrev;
                if (ratio2 > 1.0f) {
                    clamped2 = 1.0f;
                }
                else {
                    if (ratio2 < 0.0f) {
                        lo2 = 0.0f;
                    }
                    else {
                        lo2 = ratio2;
                    }
                    clamped2 = lo2;
                }
                k = 1.0f - clamped2;
            }
            np.x = self->start.x + (0.2f + 0.8f * k) * (self->dest.x - self->start.x);
            np.z = self->start.z + (0.2f + 0.8f * k) * (self->dest.z - self->start.z);
        }
        np.y = self->pos->y + self->vel.y;
        PSVECSubtract(&np, self->pos, &self->vel);
        self->pos->x = np.x;
        self->pos->y = np.y;
        self->pos->z = np.z;
    }
    else {
        PSVECAdd(self->pos, &self->vel, self->pos);
    }
    if (self->kind == 0) {
        Hu3DModelRotSet(self->modelId, 0.0f, ((f32)(game->frame % 30) / 30.0f) * 360.0f, 0.0f);
    }
    else {
        Hu3DModelRotSet(self->modelId, 0.0f, 0.0f, 0.0f);
    }
    fn_1_BC18(self, &side->ten);
    if (fn_1_C184(self, &side->player[0]) != 0) {
        return 1;
    }
    if (fn_1_C184(self, &side->player[1]) != 0) {
        return 1;
    }
    return 0;
}

void fn_1_B91C(M575CoinItem *self, u16 kind, Vec *pos, Vec *vel, f32 height)
{
    M575Coin *coin = self->coin;
    M575Camera *camera = &coin->main->camera;
    s16 id;

    if (self->active == 0 && ColMapInitCheck() != 0) {
        self->kind = kind;
        self->height = height;
        if (kind == 1) {
            id = Hu3DModelLink(coin->modelId2);
            self->modelId = id;
            Hu3DModelScaleSet(id, 1.0f, 1.0f, 1.0f);
        }
        else {
            id = Hu3DModelLink(coin->modelId);
            self->modelId = id;
            Hu3DModelScaleSet(id, 1.33f, 1.33f, 1.33f);
        }
        Hu3DModelPosSet(id, pos->x, pos->y, pos->z);
        Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
        Hu3DModelCameraSet(id, camera->camBit);
        Hu3DModelLayerSet(id, 4);
        Hu3DModelAttrReset(id, 1);
        Hu3DModelShadowSet(id);
        self->pos = (Vec *)&Hu3DData[id].pos;
        self->mtx = &Hu3DData[id].mtx[0][0];
        self->pos->x = pos->x;
        self->pos->y = pos->y;
        self->pos->z = pos->z;
        self->start.x = pos->x;
        self->start.y = pos->y;
        self->start.z = pos->z;
        self->vel.x = 0.0f;
        self->vel.y = height;
        self->vel.z = 0.0f;
        self->dest.x = vel->x;
        self->dest.y = 0.0f;
        self->dest.z = vel->z;
        self->state = 0;
        self->timer = 0;
        self->bounced = 0;
        self->active = 1;
    }
}

void fn_1_BB6C(M575CoinItem *self)
{
    Vec zero;

    Hu3DModelKill(self->modelId);
    self->state = 0;
    self->timer = 0;
    self->kind = 0;
    self->modelId = 0;
    self->pos = NULL;
    self->mtx = NULL;
    zero.x = 0.0f;
    zero.y = 0.0f;
    zero.z = 0.0f;
    self->vel = zero;
    self->active = 0;
}

// Bounce of a coin off the tube: the coin is brought into tube space and, if it
// has sunk into the top, it is pushed out and reflected.
void fn_1_BC18(M575CoinItem *self, M575Ten *ten)
{
    M575Main *side = ten->main;
    M575TuboEff *eff = &ten->eff;
    Vec axis;
    Vec local;
    Vec flat;
    Vec n;
    Vec ref;
    Vec cross;
    Vec screen;
    Mtx mtx;
    Mtx rot;
    f32 flatLen;
    f32 pan;
    f32 pan2;
    f32 dot;
    f32 absY;
    f32 y;
    f32 y2;
    s32 id;

    n.x = eff->mtx[1][0];
    n.y = eff->mtx[1][1];
    n.z = eff->mtx[1][2];
    local = *self->pos;
    PSMTXCopy(eff->mtx2, mtx);
    PSMTXMultVec(mtx, &local, &local);
    local.x *= -1.0f;
    local.z *= -1.0f;
    PSVECSubtract(&local, ten->pos, &local);
    self->rel = local;
    flat = local;
    flat.y = 0.0f;
    flatLen = PSVECMag(&flat);
    if (local.y < 115.0f && flatLen < 450.0f && local.y > -115.0f) {
        if (self->vel.y < -16.0f) {
            Hu3D3Dto2D((HuVecF *)self->pos, side->camera.camBit, (HuVecF *)&screen);
            pan = 2.0f * (screen.x / 576.0f - 0.5f);
            if (side->side == 0) {
                pan2 = 48.0f + 16.0f * pan;
            }
            else {
                pan2 = 80.0f + 16.0f * pan;
            }
            if (self->kind == 1) {
                id = 0x14;
            }
            else {
                id = 0x12;
            }
            fn_1_5B0(fn_1_530(id), (s32)pan2);
        }
        PSVECNormalize(&n, &flat);
        PSVECScale(&flat, &flat, 115.0f - local.y);
        PSVECAdd(self->pos, &flat, self->pos);
        ref.x = 0.0f;
        ref.y = 1.0f;
        ref.z = 0.0f;
        axis = ref;
        flat.x = n.x;
        flat.y = 0.0f;
        flat.z = n.z;
        if (flat.x != 0.0f || flat.z != 0.0f) {
            PSVECNormalize(&flat, &flat);
            PSVECCrossProduct(&axis, &flat, &cross);
            PSMTXRotAxisRad(rot, &cross, 0.017453292f * (0.25f * ten->rot));
            PSMTXMultVec(rot, &axis, &n);
        }
        dot = PSVECDotProduct(&n, &self->vel);
        flat.x = 2.0f * dot * n.x;
        flat.y = 2.0f * dot * n.y;
        flat.z = 2.0f * dot * n.z;
        PSVECSubtract(&flat, &self->vel, &flat);
        flat.y *= -0.7f;
        absY = (flat.y >= 0.0f) ? flat.y : -flat.y;
        if (absY < 8.0f) {
            if (flat.y < 0.0f) {
                y = -8.0f;
            }
            else {
                y = 8.0f;
            }
            flat.y = y;
        }
        self->vel.y = flat.y;
        flat.y = 0.0f;
        PSVECScale(&flat, &flat, -0.7f);
        if (PSVECMag(&flat) < 2.0f) {
            PSVECNormalize(&flat, &flat);
            PSVECScale(&flat, &flat, 2.0f);
        }
        self->vel.x = flat.x;
        self->vel.z = flat.z;
        self->bounced = 1;
        PSVECAdd(self->pos, &self->vel, self->pos);
    }
}

// Pick-up test of one player against a coin; returns 1 when it was collected.
s32 fn_1_C184(M575CoinItem *self, M575Player *player)
{
    M575Main *side = player->main;
    M575Score *score = &side->score;
    Vec d;
    Vec screen;
    f32 feetY;
    f32 headY;
    f32 lowY;
    f32 highY;
    f32 dist;
    f32 pan;
    f32 pan2;
    s32 id;
    s32 ret;

    feetY = player->pos->y;
    headY = player->pos->y + player->height;
    lowY = self->pos->y - 50.0f;
    highY = 50.0f + self->pos->y;
    if (side->game->mode >= 5) {
        return 0;
    }
    ret = 0;
    if ((feetY >= lowY && headY < highY) || (feetY <= lowY && headY > highY) || (headY >= lowY && headY < highY)) {
        d.x = player->pos->x - self->pos->x;
        d.y = 0.0f;
        d.z = player->pos->z - self->pos->z;
        dist = PSVECMag(&d);
        if (player->falling == 0 && dist < 100.0f) {
            CharEffectCoinGlowCreate(side->camera.camBit, (HuVecF *)self->pos);
            if (self->kind == 1) {
                score->value += 5;
            }
            else {
                score->value += 1;
            }
            Hu3D3Dto2D((HuVecF *)self->pos, side->camera.camBit, (HuVecF *)&screen);
            pan = 2.0f * (screen.x / 576.0f - 0.5f);
            if (side->side == 0) {
                pan2 = 48.0f + 16.0f * pan;
            }
            else {
                pan2 = 80.0f + 16.0f * pan;
            }
            if (self->kind == 1) {
                id = 0x13;
            }
            else {
                id = 7;
            }
            fn_1_5B0(fn_1_530(id), (s32)pan2);
            omVibrate(player->playerNo, 0xF, 6, 6);
            ret = 1;
        }
    }
    return ret;
}
