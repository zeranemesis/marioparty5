// REL/m575dll/stage.c - the stage model (side 0 only) and the 60 falling
// background props attached to it.
#include "REL/m575dll.h"
#include "game/hu3d.h"
#include "game/data.h"
#include "game/memory.h"
#include "game/frand.h"
#include "string.h"

s32 fn_1_530(s32 seId);
void fn_1_4538(M575StageItem *self, s16 idx, M575Stage *stage);
void fn_1_463C(M575StageItem *self);
s32 fn_1_46B4(M575StageItem *self);

void fn_1_3DE0(M575Stage *self, M575Main *main)
{
    M575Camera *camera = &main->camera;
    s16 i;
    s16 id;
    Vec tmp;

    memset(self, 0, sizeof(M575Stage));
    self->main = main;
    if (main->side == 0) {
        id = Hu3DModelCreateData(0x820000);
        self->modelId = id;
        Hu3DModelPosSet(id, 0.0f, 0.0f, 0.0f);
        Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
        Hu3DModelCameraSet(id, 0x11);
        Hu3DModelLayerSet(id, 0);
        Hu3DModelAttrReset(id, 1);
        self->motion1 = Hu3DJointMotionData(id, 0x82000F);
        Hu3DMotionSet(id, self->motion1);
        Hu3DModelAttrSet(id, 0x40000001);
        id = Hu3DModelCreateData(0x820001);
        self->modelId2 = id;
        Hu3DModelAttrSet(id, 1);
        for (i = 0; i < 0x3C; i++) {
            M575StageItem *item = &self->item[i];
            f32 z;
            fn_1_4538(item, i, self);
            tmp.x = 0.0f;
            tmp.y = 0.0f;
            tmp.z = 0.0f;
            item->pos = tmp;
            item->target = item->pos;
            z = 50.0f + (400.0f + 100.0f * (f32)i);
            item->pos.z = z;
            item->target.z = z;
        }
        self->itemOn = 1;
        id = Hu3DModelCreateData(0x820002);
        self->modelId3 = id;
        Hu3DModelPosSet(id, 0.0f, 0.0f, 0.0f);
        Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
        Hu3DModelCameraSet(id, 0x11);
        Hu3DModelLayerSet(id, 0);
        Hu3DModelAttrReset(id, 1);
        self->motion2 = Hu3DJointMotionData(id, 0x820010);
        Hu3DMotionSet(id, self->motion2);
        Hu3DModelAttrSet(id, 0x40000001);
        id = Hu3DModelCreateData(0x820003);
        self->modelId4 = id;
        Hu3DModelPosSet(id, 0.0f, 0.0f, 0.0f);
        Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
        Hu3DModelCameraSet(id, 1);
        Hu3DModelLayerSet(id, 0);
        Hu3DModelAttrReset(id, 1);
        self->motion3 = Hu3DJointMotionData(id, 0x820011);
        Hu3DMotionSet(id, self->motion3);
        Hu3DModelAttrSet(id, 0x40000001);
        id = Hu3DModelCreateData(0x820004);
        self->modelId5 = id;
        Hu3DModelPosSet(id, 0.0f, 0.0f, 0.0f);
        Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
        Hu3DModelCameraSet(id, 0x11);
        Hu3DModelLayerSet(id, 0);
        Hu3DModelAttrReset(id, 1);
        id = Hu3DModelCreateData(0x820005);
        self->modelId6 = id;
        Hu3DModelPosSet(id, 0.0f, 0.0f, 8000.0f);
        Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
        Hu3DModelCameraSet(id, 0x11);
        Hu3DModelLayerSet(id, 0);
        Hu3DModelAttrReset(id, 1);
        self->motion4 = Hu3DJointMotionData(id, 0x820012);
        Hu3DModelAttrReset(id, 0x40000001);
    }
    self->time = 0.0f;
}

void fn_1_4360(M575Stage *self)
{
    s32 i;

    if (self->main->side == 0) {
        for (i = 0; i < 0x3C; i++) {
            fn_1_463C(&self->item[i]);
        }
        Hu3DMotionKill(self->motion4);
        Hu3DMotionKill(self->motion3);
        Hu3DMotionKill(self->motion2);
        Hu3DMotionKill(self->motion1);
        Hu3DModelKill(self->modelId6);
        Hu3DModelKill(self->modelId5);
        Hu3DModelKill(self->modelId4);
        Hu3DModelKill(self->modelId3);
        Hu3DModelKill(self->modelId2);
        Hu3DModelKill(self->modelId);
    }
    memset(self, 0, sizeof(M575Stage));
}

void fn_1_442C(M575Stage *self)
{
    M575Main *main = self->main;
    M575Game *game = main->game;
    f32 time;
    s32 i;

    if (main->side == 0 && self->itemOn != 0) {
        for (i = 0; i < 0x3C; i++) {
            fn_1_46B4(&self->item[i]);
        }
    }
    if (game->mode == 4) {
        time = Hu3DMotionTimeGet(self->modelId3);
        if (self->time < 90.0f) {
            if (time >= 90.0f) {
                if (main->side == 0) {
                    fn_1_530(0x932);
                }
                else if (main->side == 1) {
                    fn_1_530(0x933);
                }
            }
        }
        self->time = time;
    }
}

void fn_1_4538(M575StageItem *self, s16 idx, M575Stage *stage)
{
    M575Main *main = stage->main;
    M575Camera *camera = &main->camera;
    s16 id;

    memset(self, 0, sizeof(M575StageItem));
    if (main->side == 0) {
        self->stage = stage;
        self->idx = idx;
        id = Hu3DModelLink(stage->modelId2);
        self->modelId = id;
        Hu3DModelPosSet(id, 0.0f, 0.0f, 0.0f);
        Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
        Hu3DModelCameraSet(id, camera->camBit);
        Hu3DModelLayerSet(id, 0);
        Hu3DModelAttrReset(id, 1);
        Hu3DModelShadowMapSet(id);
    }
}

void fn_1_463C(M575StageItem *self)
{
    if (self->stage->main->side != 0) {
        memset(self, 0, sizeof(M575StageItem));
        return;
    }
    Hu3DModelKill(self->modelId);
    memset(self, 0, sizeof(M575StageItem));
}

s32 fn_1_46B4(M575StageItem *self)
{
    M575Main *main = self->stage->main;
    M575Player *player = &main->player[0];
    M575Player *player2 = &main->player[1];
    Vec diff;
    Vec effPos;
    f32 ratio;
    f32 clamped;
    f32 lo;
    f32 rnd;
    f32 sign;
    f32 damp;
    f32 damp2;

    ratio = ((player->pos->z - 400.0f) - 50.0f) / 5000.0f;
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
    switch (self->state) {
    case 0:
        if (player->pos->z < (self->pos.z - 50.0f) + 100.0f * (1.0f - clamped)) {
            self->state++;
        }
        break;
    case 1:
        self->velY = -20.0f * (1.0f - clamped) - 0.1f;
        self->state++;
        break;
    case 2:
        self->velY += 1.5f;
        if (self->pos.y >= 0.0f) {
            rnd = 200.0f * ((f32)(frand() % 10000) / 10000.0f);
            effPos.x = self->pos.x - 100.0f + rnd;
            effPos.y = self->pos.y;
            effPos.z = self->pos.z;
            fn_1_E55C(1, &effPos);
            self->state++;
        }
        break;
    case 3:
        self->velY += 1.5f;
        if (self->pos.y > 100.0f) {
            self->rotTarget = 20.0f + 60.0f * ((f32)(frand() % 10000) / 10000.0f);
            if ((frand() & 1) != 0) {
                sign = -1.0f;
            }
            else {
                sign = 1.0f;
            }
            self->rotTarget *= sign;
            self->state++;
        }
        break;
    case 4:
        damp = 0.1f * self->velY;
        if (damp >= 0.0f) {
            damp2 = damp;
        }
        else {
            damp2 = -damp;
        }
        self->velY -= 0.8f + damp2;
        if (self->pos.y < -3000.0f) {
            Hu3DModelAttrSet(self->modelId, 1);
            self->state++;
        }
        break;
    }
    self->target.y += self->velY;
    PSVECSubtract(&self->target, &self->pos, &diff);
    PSVECScale(&diff, &diff, 0.2f);
    PSVECAdd(&self->pos, &diff, &self->pos);
    Hu3DModelPosSet(self->modelId, self->pos.x, self->pos.y, self->pos.z);
    self->rot += 0.05f * (self->rotTarget - self->rot);
    Hu3DModelRotSet(self->modelId, 0.0f, 0.0f, self->rot);
    return 0;
}
