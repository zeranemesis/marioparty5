// REL/m575dll/player.c - the per-side player work record (0xCC bytes) and its
// MgPlayer hooks.
#include "REL/m575dll.h"
#include "game/hu3d.h"
#include "game/pad.h"
#include "game/charman.h"
#include "game/hsfex.h"
#include "game/gamework.h"
#include "game/mg/actman.h"
#include "game/mg/colman.h"
#include "string.h"
#include "math.h"

// Other units.
void fn_1_5CD8(MGACTOR *actor, int param);
int fn_1_5FC8(COL_NARROW_PARAM *a, COL_NARROW_PARAM *b);
void fn_1_61CC(MGPLAYER *player);
void MgActorPosGet(MGACTOR *actorP, HuVecF *pos);
extern M575Game *lbl_1_bss_0;

void (*lbl_1_data_58[MGPLAYER_MODE_MAX])(MGPLAYER *) = {
    fn_1_61CC,
};

#define M575_MOTROW(base) \
    { ((base) << 16) | 0x0000, ((base) << 16) | 0x0001, ((base) << 16) | 0x0002, ((base) << 16) | 0x0003, \
      ((base) << 16) | 0x0004, ((base) << 16) | 0x002B, ((base) << 16) | 0x005A, ((base) << 16) | 0x0006, \
      ((base) << 16) | 0x0007 }

// Motion data numbers per character.
u32 lbl_1_data_A0[13][9] = {
    M575_MOTROW(0x008C), M575_MOTROW(0x0033), M575_MOTROW(0x00B7), M575_MOTROW(0x00ED),
    M575_MOTROW(0x00E6), M575_MOTROW(0x0017), M575_MOTROW(0x00E0), M575_MOTROW(0x002D),
    M575_MOTROW(0x00C9), M575_MOTROW(0x00B0), M575_MOTROW(0x008C), M575_MOTROW(0x008C),
    M575_MOTROW(0x008C),
};

M575Player *lbl_1_bss_7810[4];

// Finds the player record the engine hands back as a hook parameter. The
// original stores the record's address in the (32-bit) actor parameter; on PC
// the parameter is the player number and the record comes from the table.
#ifdef TARGET_PC
#define M575_PLAYER_FROM_PARAM(param) (lbl_1_bss_7810[(param)])
#else
#define M575_PLAYER_FROM_PARAM(param) ((M575Player *)(param))
#endif

void fn_1_535C(M575Player *self, s16 slot, s16 playerNo, M575Main *main)
{
    MGACTOR_PARAM param;
    Vec init;
    M575Camera *camera = &main->camera;
    s32 i;
    f32 posX;

    memset(self, 0, sizeof(M575Player));
    self->main = main;
    self->slot = slot;
    self->playerNo = playerNo;
    self->charNo = GwPlayerConf[self->playerNo].charNo;
    self->padOn = 0;
    self->height = CharModelHeightGet(self->charNo);
    self->speed = 150.0f;
    lbl_1_bss_7810[playerNo] = self;
    param.height = 150.0f;
    param.radius = 40.0f;
    param.param = self->playerNo;
    param.type = 0;
    param.attr = 0;
    param.narrowHook = fn_1_5FC8;
    param.correctHook = fn_1_5CD8;
#ifdef TARGET_PC
    param.correctHookParam = playerNo;
#else
    param.correctHookParam = (int)self;
#endif
    self->mgPlayer = MgPlayerCreate(self->playerNo, &param, 4, camera->camBit, 3, NULL);
    self->modelId = self->mgPlayer->actor->mdlId;
    self->pos = (Vec *)&Hu3DData[self->modelId].pos;
    self->rot = (Vec *)&Hu3DData[self->modelId].rot;
    Hu3DModelCameraSet(self->modelId, camera->camBit);
    for (i = 0; i < 9; i++) {
        self->motion[i] = CharMotionCreate(self->charNo, lbl_1_data_A0[self->charNo][i]);
    }
    MgPlayerModeFuncSet(self->mgPlayer, lbl_1_data_58);
    Hu3DModelShadowSet(self->modelId);
    init.x = 0.0f;
    init.y = 0.0f;
    init.z = 0.0f;
    *self->pos = init;
    if ((slot & 1) != 0) {
        posX = 150.0f;
    }
    else {
        posX = -150.0f;
    }
    self->pos->x = posX;
    MgActorPosSet(self->mgPlayer->actor, (HuVecF *)self->pos);
    fn_1_6630(&self->com, self);
}

void fn_1_55EC(M575Player *self)
{
    fn_1_675C(&self->com);
    memset(self, 0, sizeof(M575Player));
}

s32 fn_1_562C(M575Player *self)
{
    M575Main *main = self->main;
    M575Ten *ten = &main->ten;
    M575Game *game = main->game;
    M575Com *com = &self->com;
    M575Jugem *jugem = &main->jugem;
    M575Player *other;
    Vec pos;
    Vec push;
    Vec screen;
    Mtx mtx;
    f32 pan;
    f32 pan2;

    if (self == &main->player[0]) {
        other = &main->player[1];
    }
    else {
        other = &main->player[0];
    }
    if (com->unk8 != 0) {
        fn_1_678C(com);
        MgPlayerPadSet(self->mgPlayer, com->unk40, com->unk44, com->unk3C, com->unk48);
    }
    if (self->padOn == 0) {
        MgPlayerPadSet(self->mgPlayer, 0, 0, 0, 0);
    }
    if (self->mgPlayer->mode == 1) {
        self->unkA8 = 1;
    }
    if (self->unkA8 != 0 && self->unkAC < 0x3C) {
        self->unkAC++;
    }
    PSMTXCopy(ten->eff.mtx2, mtx);
    PSMTXMultVec(mtx, self->pos, &pos);
    pos.x *= -1.0f;
    pos.z *= -1.0f;
    PSVECSubtract(&pos, ten->pos, &pos);
    self->vel = pos;
    pos.y = 0.0f;
    self->speed = PSVECMag(&pos);
    if (self->mgPlayer->mode == 1 || self->mgPlayer->mode == 2) {
        self->unkB4 = 1;
    }
    else {
        self->unkB4 = 0;
        self->unkB8 = 0;
    }
    if (self->unkB4 != 0) {
        if (self->prevVelY >= 0.0f && self->mgPlayer->actor->velY < 0.0f) {
            self->unkB8 = 1;
        }
    }
    else {
        self->unkB8 = 0;
    }
    push = self->push;
    if (push.x != 0.0f || push.y != 0.0f || push.z != 0.0f) {
        MgActorPosGet(self->mgPlayer->actor, (HuVecF *)&pos);
        pos.x += 0.6f * push.x;
        pos.y += 0.6f * push.y;
        pos.z += 0.6f * push.z;
        MgActorPosSet(self->mgPlayer->actor, (HuVecF *)&pos);
        MgActorPosGet(other->mgPlayer->actor, (HuVecF *)&pos);
        pos.x -= 0.4f * push.x;
        pos.y -= 0.4f * push.y;
        pos.z -= 0.4f * push.z;
        MgActorPosSet(other->mgPlayer->actor, (HuVecF *)&pos);
        PSVECScale(&self->push, &self->push, 0.8f);
    }
    if (self->vel.y < -25.0f && self->falling == 0) {
        fn_1_5C88(self, 0);
        Hu3DMotionShiftSet(self->modelId, self->motion[5], 0.0f, 4.0f, 0x40000001);
        self->unk84 = self->mgPlayer->actor->velY / 100.0f;
        if (self->unk84 > 0.0f) {
            pan = 0.0f;
        }
        else {
            pan = self->unk84;
        }
        self->unk84 = pan;
        self->falling = 1;
    }
    if (self->vel.y < -25.0f && (jugem->holding == 0 || jugem->held != self)) {
        Hu3DModelShadowDispOff(self->modelId);
    }
    else {
        Hu3DModelShadowDispOn(self->modelId);
    }
    if (self->falling != 0) {
        fn_1_CFE4(jugem, self);
        if (jugem->holding == 0 || jugem->held != self) {
            self->unk84 -= 1.5f;
            if (self->unk84 < -225.0f) {
                self->unk84 = -225.0f;
            }
            self->pos->y += self->unk84;
        }
        if (self->pos->y < -3000.0f) {
            self->pos->y = -3000.0f;
        }
    }
    self->prevVelY = self->mgPlayer->actor->velY;
    self->timer++;
    if (game->mode >= 4) {
        Hu3D3Dto2D((HuVecF *)self->pos, main->camera.camBit, (HuVecF *)&screen);
        pan = 2.0f * (screen.x / 576.0f - 0.5f);
        if (main->side == 0) {
            pan2 = 48.0f + 16.0f * pan;
        }
        else {
            pan2 = 80.0f + 16.0f * pan;
        }
        CharModelVoicePanSet(self->charNo, pan2);
    }
    else {
        CharModelVoicePanSet(self->charNo, 0x40);
    }
    return 0;
}

void fn_1_5C88(M575Player *self, s32 on)
{
    if (on != 0) {
        MgPlayerPauseOff(self->mgPlayer, (HuVecF *)self->pos);
        return;
    }
    MgPlayerPauseOn(self->mgPlayer);
}

// Hook run on every collision-corrected move of the actor: when the actor stands
// on the tube it pushes the tube.
void fn_1_5CD8(MGACTOR *actor, int param)
{
    M575Player *player = M575_PLAYER_FROM_PARAM(actor->correctHookParam);
    M575Main *main = player->main;
    M575Game *game = main->game;
    M575Ten *ten = &main->ten;
    Vec impulse;
    f32 ratio;
    f32 scale;
    f32 speed;
    f32 lo;
    f32 lo2;
    f32 r1;
    f32 r2;
    f32 r3;
    f32 hi;
    f32 clamped;

    if (game->colMap[actor->colMesh] == ten->modelId2) {
        ratio = ten->rot / 30.0f;
        scale = ratio * (0.5f + 0.5f * ratio);
        if (scale > 1.0f) {
            clamped = 1.0f;
        }
        else {
            if (scale < 0.3f) {
                lo = 0.3f;
            }
            else {
                lo = scale;
            }
            clamped = lo;
        }
        speed = PSVECMag(&player->vel) / 400.0f;
        if (speed > 1.0f) {
            hi = 1.0f;
        }
        else {
            if (speed < 0.0f) {
                lo2 = 0.0f;
            }
            else {
                lo2 = speed;
            }
            hi = lo2;
        }
        if (hi < 0.3f) {
            r1 = 0.1f;
        }
        else {
            if (hi < 0.5f) {
                r2 = 0.2f;
            }
            else {
                if (hi < 0.7f) {
                    r3 = 0.6f;
                }
                else {
                    r3 = 1.0f;
                }
                r2 = r3;
            }
            r1 = r2;
        }
        hi = r1;
        if (game->mode < 5) {
            impulse.x = 0.0f;
            impulse.y = -0.0001f * clamped;
            impulse.z = 0.0f;
            fn_1_9B58(&ten->eff, &impulse, player->pos);
            ten->timer = 4;
        }
    }
}

// Narrow-phase hook between two players: a player landing on the other one is
// bounced off and shoves the other away.
int fn_1_5FC8(COL_NARROW_PARAM *a, COL_NARROW_PARAM *b)
{
    M575Game *game = lbl_1_bss_0;
    M575Player *pa;
    M575Player *pb;
    Vec diff;

    if (a->paramB == 0) {
        pa = lbl_1_bss_7810[0];
    }
    else if (a->paramB == 1) {
        pa = lbl_1_bss_7810[1];
    }
    else if (a->paramB == 2) {
        pa = lbl_1_bss_7810[2];
    }
    else {
        pa = lbl_1_bss_7810[3];
    }
    if (b->paramB == 0) {
        pb = lbl_1_bss_7810[0];
    }
    else if (b->paramB == 1) {
        pb = lbl_1_bss_7810[1];
    }
    else if (b->paramB == 2) {
        pb = lbl_1_bss_7810[2];
    }
    else {
        pb = lbl_1_bss_7810[3];
    }
    if (pa->unkB8 != 0 && pa->mgPlayer->actor->velY > 0.0f) {
        pa->mgPlayer->actor->velY = 0.0f;
        MgPlayerModeSet(pa->mgPlayer, 2);
        pa->mgPlayer->actor->forceB.x = 0.0f;
        pa->mgPlayer->actor->forceB.z = 0.0f;
        pa->mgPlayer->actor->rotY = pa->rot->y;
        PSVECSubtract(pa->pos, pb->pos, &diff);
        diff.y = 0.0f;
        pa->push.x = 0.2f * diff.x;
        pa->push.y = 0.0f;
        pa->push.z = 0.2f * diff.z;
    }
    return 1;
}

// Mode-0 (walk) player hook: jump/punch input and the walk/run motion choice.
void fn_1_61CC(MGPLAYER *mg)
{
    M575Game *game = lbl_1_bss_0;
    M575Player *player;
    M575Ten *ai;
    f32 speed2;

    if (lbl_1_bss_7810[1]->mgPlayer == mg) {
        player = lbl_1_bss_7810[1];
    }
    else if (lbl_1_bss_7810[2]->mgPlayer == mg) {
        player = lbl_1_bss_7810[2];
    }
    else if (lbl_1_bss_7810[3]->mgPlayer == mg) {
        player = lbl_1_bss_7810[3];
    }
    else {
        player = lbl_1_bss_7810[0];
    }
    ai = &player->main->ten;
    if ((mg->actor->colGroundAttr & 0x407F) != 0) {
        if (MgPlayerAttrCheck(mg, 1) == 0 && (HuPadBtnDown[(s8)(u8)mg->padNo] & 0x100) != 0) {
            if ((mg->actionFlag & 2) != 0) {
                MgPlayerModeSet(mg, 1);
            }
        }
        else if (MgPlayerAttrCheck(mg, 1) == 0 && (HuPadBtnDown[(s8)(u8)mg->padNo] & 0x200) != 0) {
            if ((mg->actionFlag & 4) != 0) {
                MgPlayerModeSet(mg, 3);
            }
        }
        else if ((mg->actionFlag & 1) != 0) {
            speed2 = mg->actor->vel.x * mg->actor->vel.x + mg->actor->vel.z * mg->actor->vel.z;
            if (speed2 > 25.0f) {
                if (mg->motNo != 2) {
                    CharMotionShiftSet(mg->charNo, mg->omObj->mtnId[2], 0.0f, 5.0f, 0x40000001);
                    mg->motNo = 2;
                }
                if (ai->rot > 25.0f) {
                    Hu3DMotionSpeedSet(player->modelId, 2.0f);
                    if ((game->frame & 1) != 0) {
                        CharModelCryCreate(player->charNo, 30.0f, 0.8f);
                    }
                }
                else {
                    Hu3DMotionSpeedSet(player->modelId, 1.0f);
                }
            }
            else if (speed2 > 0.001) {
                if (mg->motNo != 1) {
                    CharMotionShiftSet(mg->charNo, mg->omObj->mtnId[1], 0.0f, 5.0f, 0x40000001);
                    mg->motNo = 1;
                }
                if (ai->rot > 25.0f) {
                    Hu3DMotionSpeedSet(player->modelId, 2.0f);
                    if ((game->frame & 1) != 0) {
                        CharModelCryCreate(player->charNo, 30.0f, 0.8f);
                    }
                }
                else {
                    Hu3DMotionSpeedSet(player->modelId, 1.0f);
                }
            }
            else {
                if (mg->motNo != 0) {
                    CharMotionShiftSet(mg->charNo, mg->omObj->mtnId[0], 0.0f, 5.0f, 0x40000001);
                    mg->motNo = 0;
                }
                Hu3DMotionSpeedSet(player->modelId, 1.0f);
            }
        }
    }
    else if ((f32)fabs(mg->actor->velY) > 2000.0f) {
        MgPlayerModeSet(mg, 2);
    }
}
