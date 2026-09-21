#include "REL/m506Dll.h"
#include "game/charman.h"
#include "game/frand.h"
#include "game/pad.h"

static s16 PlayerModeNoTbl[M506_PLAYER_MAX] = { 0, 0, 0, 0 };

/* per-character motion file ids: {idle, ?, jump, land?, missA, missB, victory} */
static const u32 PlayerMotionTbl[13][7] = {
    { 0x8C0000, 0x8C0003, 0x8C0004, 0x8C002B, 0x8C0006, 0x8C0007, 0x8C000D },
    { 0x330000, 0x330003, 0x330004, 0x33002B, 0x330006, 0x330007, 0x33000D },
    { 0xB70000, 0xB70003, 0xB70004, 0xB7002B, 0xB70006, 0xB70031, 0xB7000D },
    { 0xED0000, 0xED0003, 0xED0004, 0xED002B, 0xED0006, 0xED0007, 0xED000D },
    { 0xE60000, 0xE60003, 0xE60004, 0xE6002B, 0xE60006, 0xE60007, 0xE6000D },
    { 0x170000, 0x170003, 0x170004, 0x17002B, 0x170006, 0x170007, 0x17000D },
    { 0xE00000, 0xE00003, 0xE00004, 0xE0002B, 0xE00006, 0xE00007, 0xE0000D },
    { 0x2D0000, 0x2D0003, 0x2D0004, 0x2D002B, 0x2D0006, 0x2D0007, 0x2D000D },
    { 0xC90000, 0xC90003, 0xC90004, 0xC9002B, 0xC90006, 0xC90007, 0xC9000D },
    { 0xB00000, 0xB00003, 0xB00004, 0xB0002B, 0xB00006, 0xB00007, 0xB0000D },
    { 0xB00000, 0xB00003, 0xB00004, 0xB0002B, 0xB00006, 0xB00007, 0xB0000D },
    { 0xB00000, 0xB00003, 0xB00004, 0xB0002B, 0xB00006, 0xB00007, 0xB0000D },
    { 0xB00000, 0xB00003, 0xB00004, 0xB0002B, 0xB00006, 0xB00007, 0xB0000D }
};

typedef struct M506ComMaxDelay_s {
    s16 delayMin;
    s16 delayMax;
    s16 moveChancePct;
    s16 recoverMin;
    s16 recoverMax;
} M506_COM_MAX_DELAY;

static const M506_COM_MAX_DELAY ComMaxDelayTbl[4] = {
    { 0x28, 0x34, 0x19, 8, 0x28 },
    { 0xF, 0x1C, 0x4B, 7, 0xF },
    { 6, 0xF, 0x55, 4, 7 },
    { 4, 9, 0x5A, 2, 6 }
};

static const s32 FallStartFXTbl[M506_PLAYER_MAX] = { 0x66F, 0x670, 0x671, 0x672 };
static const s32 FallEndFXTbl[M506_PLAYER_MAX] = { 0x673, 0x674, 0x675, 0x676 };
static const s32 CharFallFXTbl[13] = {
    0x1F9, 0x1E5, 0x25D, 0x2AD, 0x285, 0x1BD, 0x299, 0x1D1, 0x271, 0x235, 0x249, 0x221, 0x20D
};
static const s32 PlayerPanTbl[M506_PLAYER_MAX] = { 0x28, 0x38, 0x48, 0x58 };

static s32 M506PlayerModeNormalJump(OMOBJ *obj);
static s32 M506PlayerModeUpJump(OMOBJ *obj);
static s32 M506PlayerModeSideJump(OMOBJ *obj);
static s32 M506PlayerModeFall(OMOBJ *obj);

static s32 (*const PlayerModeTbl[4])(OMOBJ *) = {
    M506PlayerModeNormalJump,
    M506PlayerModeUpJump,
    M506PlayerModeSideJump,
    M506PlayerModeFall
};

static const f32 MotSpeedTbl[10] = { 1.0f, 0.75f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };

typedef struct M506MotShift_s {
    s32 motIdx;
    f32 start;
    f32 end;
} M506_MOT_SHIFT;

static const M506_MOT_SHIFT MotShiftTbl[4] = {
    { 0, 0.0f, 3.0f },
    { 3, 0.0f, 5.0f },
    { 1, 0.0f, 8.0f },
    { 2, 0.0f, 12.0f }
};

static const f32 JumpUpSpeedTbl[15] = {
    38.0f, 36.0f, 34.0f, 32.0f, 30.0f, 28.0f, 26.0f, 24.0f, 22.0f, 20.0f, 18.0f, 16.0f, 14.0f, 12.0f, 10.0f
};
static const f32 JumpDownSpeedTbl[8] = { 0.0f, -2.0f, -4.0f, -6.0f, -9.0f, -11.0f, -13.0f, -15.0f };
static const f32 BounceUpSpeedTbl[7] = { 20.0f, 18.0f, 16.0f, 14.0f, 12.0f, 10.0f, 8.0f };
static const f32 BounceDownSpeedTbl[8] = { 0.0f, -8.0f, -10.0f, -12.0f, -14.0f, -16.0f, -18.0f, -20.0f };

static s32 CharFallSeNo[M506_PLAYER_MAX];
static s32 FallEndSeNo[M506_PLAYER_MAX];
static s32 FallStartSeNo[M506_PLAYER_MAX];

static s16 M506ComInputDelayGet(OMOBJ *obj);
static s16 M506ComInputCheck(OMOBJ *obj);
static void M506PlayerMotionSet(M506_PLAYER_WORK *player, OMOBJ *obj, s16 motNo);

void M506PlayerUpdate(OMOBJ *obj);
BOOL M506PlayerInputRead(OMOBJ *obj);
void M506ObjShadowMapSet(int no, int height);
void M506ObjShadowMapReset(int no, int from, int to);

// NONMATCHING: faithful translation, not yet instruction-matched
void M506PlayerCreate(OMOBJ *obj)
{
    M506_PLAYER_WORK *player = obj->data;
    s16 i;

    memset(player, 0, sizeof(M506_PLAYER_WORK));
    player->no = obj->work[0];
    player->charNo = GwPlayerConf[player->no].charNo;
    player->padNo = GwPlayerConf[player->no].padNo;
    player->playerType = GwPlayerConf[player->no].type;
    player->comDif = GwPlayerConf[player->no].dif;
    player->unkC = player->unk10 = 0;
    player->newHeight = player->height = 0;
    player->motId = 0;
    player->curDir = -1;
    player->upJumpF = 0;
    player->comMoveChance = 0;
    player->comMoveChanceMax = ComMaxDelayTbl[player->comDif].recoverMin +
        frandmod(ComMaxDelayTbl[player->comDif].recoverMax - ComMaxDelayTbl[player->comDif].recoverMin);
    player->comMoveChanceMax = (player->comMoveChanceMax * 45) + 15;
    player->pos.x = -64.5f;
    player->pos.y = 0.0f;
    player->pos.z = 179.0f;
    PlayerModeNoTbl[player->no] = 4;
    if (player->no == 0) {
        obj->mdlId[0] = CharModelCreate(player->charNo, 4);
    } else {
        obj->mdlId[0] = CharModelCreate(player->charNo, 8);
    }
    omSetTra(obj, player->pos.x, player->pos.y, player->pos.z);
    Hu3DModelShadowSet(obj->mdlId[0]);
    CharModelStepFxSet(player->charNo, 1);
    for (i = 0; i < 7; i++) {
        obj->mtnId[i] = CharMotionCreate(player->charNo, PlayerMotionTbl[player->charNo][i]);
    }
    CharMotionUpdateSet(player->charNo, 0x8C0004, 0);
    CharMotionVoiceOnSet(player->charNo, 0x2B, 0);
    CharMotionSet(player->charNo, obj->mtnId[0]);
    Hu3DModelAttrSet(obj->mdlId[0], HU3D_MOTATTR_LOOP);
    Hu3DModelLayerSet(obj->mdlId[0], 5);
    Hu3DModelCameraSet(obj->mdlId[0], M506CameraBit[player->no]);
    CharEffectLayerSet(6);
    CharMotionDataClose(player->charNo);
    CharModelVoicePanSet(player->charNo, PlayerPanTbl[player->no]);
    obj->objFunc = M506PlayerUpdate;
}

// NONMATCHING: faithful translation, not yet instruction-matched
void M506PlayerUpdate(OMOBJ *obj)
{
    M506_PLAYER_WORK *player = obj->data;

    switch (M506MainObj->work[3]) {
    case 2:
        if (PlayerModeNoTbl[player->no] == 4) {
            M506PlayerInputRead(obj);
        }
        if (PlayerModeNoTbl[player->no] != 4) {
            PlayerModeTbl[PlayerModeNoTbl[player->no]](obj);
        }
        break;
    case 5:
        break;
    default:
        if (PlayerModeNoTbl[player->no] != 4) {
            PlayerModeTbl[PlayerModeNoTbl[player->no]](obj);
        }
        break;
    }
}

// NONMATCHING: faithful translation, not yet instruction-matched
static void M506PlayerMotionSet(M506_PLAYER_WORK *player, OMOBJ *obj, s16 motNo)
{
    Hu3DModelAttrReset(obj->mdlId[0], HU3D_MOTATTR_LOOP);
    Hu3DMotionSpeedSet(obj->mdlId[0], MotSpeedTbl[motNo]);
    player->motId = motNo;
    if ((u32)motNo <= 10) {
        switch (motNo) {
        case 0:
        case 3:
            Hu3DModelAttrSet(obj->mdlId[0], HU3D_MOTATTR_LOOP);
            /* fallthrough */
        case 2:
        case 4:
        case 5:
            Hu3DMotionSet(obj->mdlId[0], obj->mtnId[motNo]);
            return;
        case 7:
        case 8:
            M506ObjShadowMapReset(player->no, player->newHeight, player->height);
            /* fallthrough */
        case 9:
        case 10:
            Hu3DMotionShiftSet(obj->mdlId[0], obj->mtnId[MotShiftTbl[motNo - 7].motIdx],
                MotShiftTbl[motNo - 7].start, MotShiftTbl[motNo - 7].end, 0);
            break;
        }
    }
}

// NONMATCHING: faithful translation, not yet instruction-matched
BOOL M506PlayerMotionEndCheck(OMOBJ *obj)
{
    M506_PLAYER_WORK *player = obj->data;

    if (player->motId < 7) {
        player->motTime = Hu3DMotionTimeGet(obj->mdlId[0]);
    } else {
        player->motTime = Hu3DMotionShiftTimeGet(obj->mdlId[0]);
    }
    return player->motTime == player->motMaxTime;
}

// NONMATCHING: faithful translation, not yet instruction-matched
BOOL M506PlayerInputRead(OMOBJ *obj)
{
    M506_PLAYER_WORK *player = obj->data;
    M506_WORK *work = M506MainObj->data;
    BOOL inputF = FALSE;
    int i;

    if (player->newHeight < M506_JUMP_MAX) {
        if (player->playerType == 0) {
            if (HuPadBtnDown[player->padNo] & PAD_BUTTON_A) {
                player->curDir = -1;
                inputF = TRUE;
            }
            if ((s8)HuPadStkX[player->padNo] > 0x20 && (HuPadBtnDown[player->padNo] & PAD_BUTTON_A)) {
                player->curDir = 1;
                inputF = TRUE;
            }
            if ((s8)HuPadStkX[player->padNo] < -0x20 && (HuPadBtnDown[player->padNo] & PAD_BUTTON_A)) {
                player->curDir = 0;
                inputF = TRUE;
            }
        } else if (player->playerType == 1) {
            inputF = M506ComInputCheck(obj);
        }
    }
    if (inputF) {
        player->jumpTicks = 0;
        player->jumpF = 0;
        player->fallTime = 0;
        player->nextJumpF = (player->newHeight == 0);
        if (player->curDir != -1 && player->curDir != player->prevDir) {
            if (player->curDir == work->mameSide[player->no][player->newHeight]) {
                PlayerModeNoTbl[player->no] = 2;
                player->newHeight++;
                if (player->newHeight > player->height) {
                    player->height++;
                    player->jumpF = 1;
                }
            } else {
                PlayerModeNoTbl[player->no] = 3;
                player->fallLevel = 1;
                if (player->newHeight != 0) {
                    s16 h = player->newHeight;
                    player->fallLevel++;
                    while (work->mameSide[player->no][h - 1] != player->curDir) {
                        player->fallLevel++;
                        h--;
                        if (h == 0) {
                            break;
                        }
                    }
                    player->prevHeight = player->newHeight;
                    player->newHeight = h;
                } else {
                    player->fallLevel = 1;
                }
            }
            player->prevDir = player->curDir;
        } else if (work->mameSide[player->no][player->newHeight] == player->prevDir) {
            PlayerModeNoTbl[player->no] = 0;
            player->newHeight++;
            if (player->newHeight > player->height) {
                player->height++;
                player->jumpF = 1;
            }
        } else {
            PlayerModeNoTbl[player->no] = 1;
            player->fallLevel = 1;
        }
    }
    return 0;
}

// NONMATCHING: faithful translation, not yet instruction-matched
static s32 M506PlayerModeNormalJump(OMOBJ *obj)
{
    M506_PLAYER_WORK *player = obj->data;
    M506_WORK *work = M506MainObj->data;
    s32 doneF = 0;

    player->jumpTicks++;
    player->modeTimer++;
    M506MameUpdate(player);
    M506YCamera->camera[player->no].pos.y += 10.0f;

    switch (player->motId) {
    case 0:
        M506PlayerMotionSet(player, obj, 9);
        player->motId++;
        player->jumpTimer = 0;
        M506ObjShadowMapSet(player->no, player->newHeight);
        /* fallthrough */
    case 1:
        if (player->newHeight != 1) {
            player->pos.y += JumpUpSpeedTbl[player->jumpTimer];
        } else {
            player->pos.y += 4.8f + JumpUpSpeedTbl[player->jumpTimer];
        }
        player->jumpTimer++;
        omSetTra(obj, player->pos.x, player->pos.y, player->pos.z);
        if (player->modeTimer > 14) {
            M506PlayerMotionSet(player, obj, 2);
            player->motId++;
            player->jumpTimer = 0;
        }
        break;
    case 2:
        M506MameModeEnter(player);
        if (player->jumpTimer < 8) {
            player->pos.y += JumpDownSpeedTbl[player->jumpTimer];
            omSetTra(obj, player->pos.x, player->pos.y, player->pos.z);
            if (player->jumpTimer >= 7) {
                CharModelLandDustCreate(player->charNo, &player->pos);
                omVibrate(player->no, 12, 6, 6);
            }
        }
        player->jumpTimer++;
        if (player->modeTimer > 26) {
            M506PlayerMotionSet(player, obj, 7);
            player->motId++;
        }
        break;
    case 3:
        M506MameModeEnter(player);
        if (player->modeTimer > 29) {
            M506PlayerMotionSet(player, obj, 0);
            Hu3DModelAttrSet(obj->mdlId[0], HU3D_MOTATTR_LOOP);
            Hu3DMotionSet(obj->mdlId[0], obj->mtnId[0]);
            player->motId = 0;
            player->curDir = -1;
            PlayerModeNoTbl[player->no] = 4;
            doneF = 1;
        }
        break;
    }
    M506MameModeJump(player);
    return doneF;
}

// NONMATCHING: faithful translation, not yet instruction-matched
static s32 M506PlayerModeUpJump(OMOBJ *obj)
{
    M506_PLAYER_WORK *player = obj->data;
    s32 doneF = 0;

    player->modeTimer++;
    switch (player->motId) {
    case 0:
        M506PlayerMotionSet(player, obj, 9);
        player->jumpTimer = 0;
        player->motId++;
        /* fallthrough */
    case 1:
        if (player->nextJumpF == 0) {
            player->pos.y += JumpUpSpeedTbl[player->jumpTimer];
        } else {
            player->pos.y += 4.8f + JumpUpSpeedTbl[player->jumpTimer];
        }
        player->jumpTimer++;
        omSetTra(obj, player->pos.x, player->pos.y, player->pos.z);
        if (player->modeTimer > 14) {
            player->jumpTimer = 0;
            player->motId++;
        }
        break;
    case 2:
        if (player->jumpTimer < 8) {
            player->pos.y += JumpDownSpeedTbl[player->jumpTimer];
        } else if (player->nextJumpF == 0) {
            player->pos.y -= 20.0f;
        } else {
            player->pos.y -= 24.8f;
        }
        omSetTra(obj, player->pos.x, player->pos.y, player->pos.z);
        player->jumpTimer++;
        if (player->modeTimer > 29) {
            M506PlayerMotionSet(player, obj, 2);
            player->jumpTimer = 0;
            player->motId++;
        }
        break;
    case 3:
        M506MameModeEnter(player);
        if (player->jumpTimer < 8) {
            if (player->nextJumpF == 0) {
                player->pos.y -= 20.0f;
            } else {
                player->pos.y -= 24.8f;
            }
            omSetTra(obj, player->pos.x, player->pos.y, player->pos.z);
            if (player->jumpTimer >= 7) {
                CharModelLandDustCreate(player->charNo, &player->pos);
                omVibrate(player->no, 12, 6, 6);
            }
        }
        player->jumpTimer++;
        if (player->modeTimer > 41) {
            M506PlayerMotionSet(player, obj, 7);
            player->motId++;
        }
        break;
    case 4:
        M506MameModeEnter(player);
        if (player->modeTimer > 44) {
            M506PlayerMotionSet(player, obj, 0);
            Hu3DModelAttrSet(obj->mdlId[0], HU3D_MOTATTR_LOOP);
            Hu3DMotionSet(obj->mdlId[0], obj->mtnId[0]);
            player->motId = 0;
            player->curDir = -1;
            PlayerModeNoTbl[player->no] = 4;
            doneF = 1;
        }
        break;
    }
    return doneF;
}

// NONMATCHING: faithful translation, not yet instruction-matched
static s32 M506PlayerModeSideJump(OMOBJ *obj)
{
    M506_PLAYER_WORK *player = obj->data;
    s32 doneF = 0;

    M506YCamera->camera[player->no].pos.y += 10.0f;
    player->jumpTicks++;
    player->modeTimer++;
    M506MameUpdate(player);

    switch (player->motId) {
    case 0:
        M506PlayerMotionSet(player, obj, 9);
        player->jumpTimer = 0;
        player->motId++;
        M506ObjShadowMapSet(player->no, player->newHeight);
        /* fallthrough */
    case 1:
        if (player->curDir == 0) {
            player->pos.x -= 5.6f;
        } else {
            player->pos.x += 5.6f;
        }
        if (player->newHeight != 1) {
            player->pos.y += JumpUpSpeedTbl[player->jumpTimer];
        } else {
            player->pos.y += 4.8f + JumpUpSpeedTbl[player->jumpTimer];
        }
        player->jumpTimer++;
        omSetTra(obj, player->pos.x, player->pos.y, player->pos.z);
        if (player->modeTimer > 14) {
            M506PlayerMotionSet(player, obj, 2);
            player->jumpTimer = 0;
            player->motId++;
        }
        break;
    case 2:
        M506MameModeEnter(player);
        if (player->jumpTimer < 8) {
            if (player->curDir == 0) {
                player->pos.x -= 5.6f;
            } else {
                player->pos.x += 5.6f;
            }
            player->pos.y += JumpDownSpeedTbl[player->jumpTimer];
            omSetTra(obj, player->pos.x, player->pos.y, player->pos.z);
            if (player->jumpTimer >= 7) {
                CharModelLandDustCreate(player->charNo, &player->pos);
                omVibrate(player->no, 12, 6, 6);
            }
        }
        player->jumpTimer++;
        if (player->modeTimer > 26) {
            M506PlayerMotionSet(player, obj, 7);
            player->jumpTimer = 0;
            player->motId++;
        }
        break;
    case 3:
        M506MameModeEnter(player);
        if (player->modeTimer > 29) {
            M506PlayerMotionSet(player, obj, 0);
            Hu3DModelAttrSet(obj->mdlId[0], HU3D_MOTATTR_LOOP);
            Hu3DMotionSet(obj->mdlId[0], obj->mtnId[0]);
            player->motId = 0;
            player->curDir = -1;
            PlayerModeNoTbl[player->no] = 4;
            doneF = 1;
        }
        break;
    }
    return doneF;
}

// NONMATCHING: complex multi-case fall state machine; faithful translation but not
// instruction-matched (case boundaries and register allocation differ from the original).
// NONMATCHING: faithful translation, not yet instruction-matched
static s32 M506PlayerModeFall(OMOBJ *obj)
{
    M506_PLAYER_WORK *player = obj->data;
    s32 doneF = 0;

    player->modeTimer++;
    if ((u32)player->motId > 10) {
        return doneF;
    }

    switch (player->motId) {
    case 0:
        M506PlayerMotionSet(player, obj, 9);
        player->jumpTimer = 0;
        player->motId++;
        /* fallthrough */
    case 1:
        M506MameModeMiss(player);
        if (player->curDir == 0) {
            player->pos.x -= 5.6f;
        } else {
            player->pos.x += 5.6f;
        }
        if (player->nextJumpF == 0) {
            player->pos.y += JumpUpSpeedTbl[player->jumpTimer];
        } else {
            player->pos.y += 4.8f + JumpUpSpeedTbl[player->jumpTimer];
        }
        player->jumpTimer++;
        omSetTra(obj, player->pos.x, player->pos.y, player->pos.z);
        if (player->modeTimer > 14) {
            if (player->nextJumpF == 0) {
                M506PlayerMotionSet(player, obj, 8);
                M506ObjShadowMapReset(player->no, player->newHeight, player->height);
                FallStartSeNo[player->no] = HuAudFXPlay(FallStartFXTbl[player->no]);
                HuAudFXPanning(FallStartSeNo[player->no], PlayerPanTbl[player->no]);
                CharFallSeNo[player->no] = HuAudFXPlay(CharFallFXTbl[player->charNo]);
                HuAudFXPanning(CharFallSeNo[player->no], PlayerPanTbl[player->no]);
                player->motId++;
            } else {
                player->motId = 8;
            }
            player->fallTime = 0;
            player->jumpTimer = 0;
        }
        break;
    case 2:
        M506MameModeMiss(player);
        player->fallTime++;
        if (player->newHeight != 0) {
            if (player->jumpTimer < 8) {
                player->pos.y += JumpDownSpeedTbl[player->jumpTimer];
            } else {
                player->pos.y -= 20.0f;
            }
        } else if (player->jumpTimer < 8) {
            player->pos.y += JumpDownSpeedTbl[player->jumpTimer];
        } else if (((player->fallLevel * 15) - player->fallTime) < 15) {
            player->pos.y -= 24.8f;
        } else {
            player->pos.y -= 20.0f;
        }
        if (player->jumpTimer < 8) {
            if (player->curDir == 0) {
                player->pos.x -= 5.6f;
            } else {
                player->pos.x += 5.6f;
            }
        }
        omSetTra(obj, player->pos.x, player->pos.y, player->pos.z);
        if (player->jumpTimer > 7 && player->fallTime <= (((player->fallLevel - 1) * 15) + 8)) {
            M506YCamera->camera[player->no].pos.y -= 20.0f;
        }
        player->jumpTimer++;
        if (player->modeTimer > 19) {
            M506PlayerMotionSet(player, obj, 3);
            Hu3DModelAttrSet(obj->mdlId[0], HU3D_MOTATTR_LOOP);
            Hu3DMotionSet(obj->mdlId[0], obj->mtnId[3]);
            player->motId++;
        }
        break;
    case 3:
        if (player->modeTimer < 31) {
            M506MameModeMiss(player);
        }
        player->fallTime++;
        if (player->newHeight != 0) {
            if (player->jumpTimer < 8) {
                player->pos.y += JumpDownSpeedTbl[player->jumpTimer];
            } else {
                player->pos.y -= 20.0f;
            }
        } else if (player->jumpTimer < 8) {
            player->pos.y += JumpDownSpeedTbl[player->jumpTimer];
        } else if (((player->fallLevel * 15) - player->fallTime) < 15) {
            player->pos.y -= 24.8f;
        } else {
            player->pos.y -= 20.0f;
        }
        if (player->jumpTimer < 8) {
            if (player->curDir == 0) {
                player->pos.x -= 5.6f;
            } else {
                player->pos.x += 5.6f;
            }
        }
        omSetTra(obj, player->pos.x, player->pos.y, player->pos.z);
        if (player->jumpTimer > 7 && player->fallTime <= (((player->fallLevel - 1) * 15) + 8)) {
            M506YCamera->camera[player->no].pos.y -= 20.0f;
        }
        player->jumpTimer++;
        if (player->fallTime >= (player->fallLevel * 15)) {
            player->jumpTimer = 0;
            player->prevFallTime = player->fallTime;
            player->motId++;
        }
        break;
    case 4:
        if (player->modeTimer < 31) {
            M506MameModeMiss(player);
        }
        if (player->jumpTimer < 8) {
            M506MameModeEnter(player);
            if (player->nextJumpF == 0) {
                player->pos.y -= 20.0f;
            } else {
                player->pos.y -= 24.8f;
            }
            omSetTra(obj, player->pos.x, player->pos.y, player->pos.z);
            if (player->jumpTimer >= 7) {
                CharModelLandDustCreate(player->charNo, &player->pos);
                omVibrate(player->no, 12, 4, 2);
            }
        }
        player->jumpTimer++;
        player->fallTime++;
        if (player->fallTime <= (((player->fallLevel - 1) * 15) + 8)) {
            M506YCamera->camera[player->no].pos.y -= 20.0f;
        }
        if (player->jumpTimer >= 9) {
            player->fallTime = 0;
            player->jumpTimer = 0;
            player->motId++;
            FallEndSeNo[player->no] = HuAudFXPlay(FallEndFXTbl[player->no]);
            HuAudFXPanning(FallEndSeNo[player->no], PlayerPanTbl[player->no]);
        }
        break;
    case 5:
        if (player->jumpTimer < 7) {
            M506MameModeEnter(player);
            player->pos.y += BounceUpSpeedTbl[player->jumpTimer];
            omSetTra(obj, player->pos.x, player->pos.y, player->pos.z);
        }
        player->jumpTimer++;
        if (player->jumpTimer >= 7) {
            M506PlayerMotionSet(player, obj, 10);
            player->jumpTimer = 0;
            player->motId++;
        }
        break;
    case 6:
        if (player->jumpTimer < 8) {
            player->pos.y += BounceDownSpeedTbl[player->jumpTimer];
            omSetTra(obj, player->pos.x, player->pos.y, player->pos.z);
        }
        player->jumpTimer++;
        if (player->jumpTimer >= 12) {
            Hu3DModelAttrReset(obj->mdlId[0], HU3D_MOTATTR_LOOP);
            Hu3DMotionSpeedSet(obj->mdlId[0], MotSpeedTbl[2]);
            player->motId = 2;
            Hu3DMotionSet(obj->mdlId[0], obj->mtnId[2]);
            Hu3DMotionTimeSet(obj->mdlId[0], Hu3DMotionMotionMaxTimeGet(obj->mdlId[0]));
            M506PlayerMotionSet(player, obj, 7);
            player->jumpTimer = 1;
            player->motId++;
        }
        break;
    case 7:
        player->jumpTimer++;
        if (player->jumpTimer > 3) {
            M506PlayerMotionSet(player, obj, 0);
            Hu3DModelAttrSet(obj->mdlId[0], HU3D_MOTATTR_LOOP);
            Hu3DMotionSet(obj->mdlId[0], obj->mtnId[0]);
            player->motId = 0;
            player->fallLevel = 0;
            player->curDir = -1;
            PlayerModeNoTbl[player->no] = 4;
            doneF = 1;
        }
        break;
    case 8:
        M506MameModeMiss(player);
        if (player->jumpTimer < 8) {
            if (player->curDir == 0) {
                player->pos.x -= 5.6f;
            } else {
                player->pos.x += 5.6f;
            }
            player->pos.y += JumpDownSpeedTbl[player->jumpTimer];
        } else {
            player->pos.y -= 24.8f;
        }
        omSetTra(obj, player->pos.x, player->pos.y, player->pos.z);
        player->jumpTimer++;
        if (player->modeTimer > 29) {
            M506PlayerMotionSet(player, obj, 2);
            player->jumpTimer = 0;
            player->motId++;
        }
        break;
    case 9:
        M506MameModeEnter(player);
        if (player->jumpTimer < 8) {
            player->pos.y -= 24.8f;
            omSetTra(obj, player->pos.x, player->pos.y, player->pos.z);
            if (player->jumpTimer >= 7) {
                CharModelLandDustCreate(player->charNo, &player->pos);
                omVibrate(player->no, 12, 6, 6);
            }
        }
        player->jumpTimer++;
        if (player->modeTimer > 41) {
            M506PlayerMotionSet(player, obj, 7);
            M506ObjShadowMapReset(player->no, player->newHeight, player->height);
            Hu3DMotionShiftSet(obj->mdlId[0], obj->mtnId[MotShiftTbl[0].motIdx], MotShiftTbl[0].start, MotShiftTbl[0].end, 0);
            player->motId++;
        }
        break;
    case 10:
        M506MameModeEnter(player);
        if (player->modeTimer > 44) {
            M506PlayerMotionSet(player, obj, 0);
            Hu3DModelAttrSet(obj->mdlId[0], HU3D_MOTATTR_LOOP);
            Hu3DMotionSet(obj->mdlId[0], obj->mtnId[0]);
            player->motId = 0;
            player->fallLevel = 0;
            player->curDir = -1;
            PlayerModeNoTbl[player->no] = 4;
            doneF = 1;
        }
        break;
    }
    return doneF;
}

// NONMATCHING: faithful translation, not yet instruction-matched
static s16 M506ComInputDelayGet(OMOBJ *obj)
{
    M506_PLAYER_WORK *player = obj->data;
    M506_WORK *work = M506MainObj->data;
    s16 dir;
    s16 count;
    s16 h;

    if (player->prevDir == work->mameSide[player->no][player->newHeight]) {
        dir = (player->prevDir == 1) ? 0 : 1;
        if (player->newHeight != 0) {
            count = 1;
            h = player->newHeight;
            while (work->mameSide[player->no][h - 1] != dir) {
                count++;
                h--;
                if (h == 0) {
                    break;
                }
            }
        } else {
            count = 1;
        }
    } else {
        count = 1;
    }
    if (count >= 2) {
        return (count * 45) + 15;
    }
    return count * 45;
}

// NONMATCHING: faithful translation, not yet instruction-matched
static s16 M506ComInputCheck(OMOBJ *obj)
{
    M506_PLAYER_WORK *player = obj->data;
    M506_WORK *work = M506MainObj->data;
    BOOL inputF = FALSE;
    s16 dirF = -1;

    switch (player->comMode) {
    case 0:
        player->comDelayMax = ComMaxDelayTbl[player->comDif].delayMin +
            frandmod(ComMaxDelayTbl[player->comDif].delayMax - ComMaxDelayTbl[player->comDif].delayMin);
        if (player->nextJumpF == 1) {
            player->comDelayMax /= 2;
        }
        if (player->jumpF == 0) {
            player->comDelayMax /= 2;
        } else if (player->comRecoverF != 0) {
            player->comDelayMax /= 2;
        }
        player->comMode++;
        /* fallthrough */
    case 1:
        player->comDelay++;
        if (player->comDelayMax <= player->comDelay) {
            if (frandmod(100) <= (u32)ComMaxDelayTbl[player->comDif].moveChancePct) {
                dirF = 1;
            } else {
                s16 delay = M506ComInputDelayGet(obj);
                if ((delay + player->comMoveChance) > player->comMoveChanceMax) {
                    dirF = 1;
                } else {
                    dirF = 0;
                    player->comMoveChance += delay;
                }
            }
            if (player->nextJumpF == 1) {
                dirF = 1;
            }
            if (player->jumpF == 0) {
                dirF = 1;
            } else if (player->comRecoverF != 0) {
                player->comRecoverF = 0;
                dirF = 1;
            }
            player->comMode = 0;
            player->comDelayMax = 0;
            player->comDelay = 0;
        }
        break;
    }

    if (dirF == 1) {
        player->curDir = work->mameSide[player->no][player->newHeight];
        inputF = TRUE;
    } else if (dirF == 0) {
        if (work->mameSide[player->no][player->newHeight] == 0) {
            player->curDir = 1;
        } else if (work->mameSide[player->no][player->newHeight] == 1) {
            player->curDir = 0;
        }
        player->comRecoverF = 1;
        inputF = TRUE;
    }
    return inputF;
}
