#include "REL/sd00dll_sys.h"
#include "game/data.h"
#include "game/esprite.h"
#include "game/frand.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/mg/seqman.h"
#include "game/pad.h"
#include "game/sprite.h"
#include "math.h"
#include "string.h"

#define SDDATA(n) (0x00BC0000 + (n))

#define RABBIT_TRAIL_NUM 5

/* rabbit main mode (rabbitMode[0]) */
#define RABBIT_MODE_WAIT 0
#define RABBIT_MODE_MOVE 1
#define RABBIT_MODE_CAUGHT 2
#define RABBIT_MODE_RESPAWN 3
#define RABBIT_MODE_SHRINK 10
#define RABBIT_MODE_DROP 11
#define RABBIT_MODE_APPEAR 12
#define RABBIT_MODE_END 99

s32 rand8(void);

/* rabbitTransform: orientation matrix, 3 terrain probe points (rabbit local
   space) and the Euler rotation derived from the matrix. */
typedef struct RabbitTransform_s {
    Mtx mtx;      // 0x00
    Vec probe[3]; // 0x30
    Vec rot;      // 0x54
} RabbitTransform; // size 0x60

static s16 rabbitEffectMdlId;
static s32 rabbitDispTimer;
static f32 rabbitFadeTimer;
static Vec rabbitRot[RABBIT_TRAIL_NUM];
static SdFlagWork rabbitModelData[RABBIT_TRAIL_NUM];
static Vec rabbitZAxis;
static f32 rabbitSpeed;
static s32 rabbitEffectTimer;
static s32 rabbitEffectFadeTimer[2];
static Vec rabbitScale;
static f32 rabbitPrevMoveSpeed;
static f32 rabbitAlpha;
static f32 rabbitDropOfs;
static f32 rabbitScaleVal;
static f32 rabbitTurnDir;
static f32 rabbitMoveDist;
static f32 rabbitMoveSpeed;
static s32 rabbitShrinkTimer[3];
static s32 rabbitOverridePos;
static s32 rabbitImpactTimer[2];
static s32 randomMoveTimer;
static s32 rabbitIdleTimerMax;
static s32 rabbitIdleTimer;
static s32 rabbitMode[3]; // [0] main mode, [1] unused, [2] move sub mode
static RabbitTransform rabbitTransform;
static SdFlagWork rabbitBaseModel;

static void RabbitModeMove(void);
static s32 RabbitStageEdgeCheck(Vec *pos);
static void RabbitRotUpdate(void);
static s32 RabbitImpactDirGet(void);
static s32 RabbitShrinkCheck(void);
static void RabbitEffectCreate(void);
static void RabbitEffectHook(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);
static void RabbitEffectPosSet(void);
static void RabbitEffectUpdate(s32 mode);

/* turn the rabbit about Y and push the new Euler angles to the model */
static void RabbitTurn(f32 angle)
{
    Mtx rotMtx;

    PSMTXRotRad(rotMtx, 'y', angle);
    PSMTXConcat(rabbitTransform.mtx, rotMtx, rabbitTransform.mtx);
    MathMtxRotGet(rabbitTransform.mtx, &rabbitTransform.rot);
    Hu3DModelRotSetV(rabbitBaseModel.mdlId, &rabbitTransform.rot);
}

void SdRabbitInit(void)
{
    s32 i;
    s32 j;
    f32 turn;
    Vec zero = { 0.0f, 0.0f, 0.0f };
    Vec probe0 = { 62.0f, 0.0f, 69.0f };
    Vec probe1 = { -62.0f, 0.0f, 69.0f };
    Vec probe2 = { 62.0f, 0.0f, -69.0f };

    if (sys->gameType == 2) {
        rabbitMode[0] = 0;
        rabbitMode[1] = 0;
        rabbitMode[2] = 0;
        rabbitMoveSpeed = 0.0f;
        rabbitBaseModel.mdlId = -1;
        for (i = 0; i < 2; i++) {
            rabbitBaseModel.timer[i] = 0;
            for (j = 0; j < 3; j++) {
                rabbitBaseModel.spr[i][j] = -1;
            }
        }
        rabbitBaseModel.pos = zero;
        RabbitEffectCreate();
        PSMTXIdentity(rabbitTransform.mtx);
        rabbitTransform.probe[0] = probe0;
        rabbitTransform.probe[1] = probe1;
        rabbitTransform.probe[2] = probe2;
        randomMoveTimer = 0;
        if (frandmod(2) != 0) {
            turn = 1.0f;
        } else {
            turn = -1.0f;
        }
        rabbitTurnDir = turn;
    }
}

void SdRabbitUpdate(void)
{
    s32 i;
    Vec zero = { 0.0f, 0.0f, 0.0f };
    Vec one = { 1.0f, 1.0f, 1.0f };
    Vec impactPos;
    Mtx rotMtx;

    if (sys->gameType != 2 || MgSeqModeGet() < 5) {
        return;
    }
    switch (rabbitMode[0]) {
        case RABBIT_MODE_WAIT:
            if (rabbitBaseModel.mdlId == -1) {
                rabbitBaseModel.mdlId = Hu3DModelCreate(HuDataSelHeapReadNum(SDDATA(0x69), HU_MEMNUM_OVL, HEAP_MODEL));
                Hu3DModelCameraSet(rabbitBaseModel.mdlId, 3);
                Hu3DModelAttrSet(rabbitBaseModel.mdlId, 1);
                Hu3DModelLayerSet(rabbitBaseModel.mdlId, 3);
                if (rabbitBaseModel.mdlId != -1) {
                    for (i = 0; i < RABBIT_TRAIL_NUM; i++) {
                        rabbitModelData[i].mdlId = Hu3DModelLink(rabbitBaseModel.mdlId);
                        Hu3DModelCameraSet(rabbitModelData[i].mdlId, 3);
                        Hu3DModelAttrSet(rabbitModelData[i].mdlId, 1);
                        rabbitFadeTimer = 1.0f;
                        Hu3DModelTPLvlSet(rabbitModelData[i].mdlId, (rabbitFadeTimer - rabbitFadeTimer / 5.0f * i) * 0.25f);
                        Hu3DModelLayerSet(rabbitModelData[i].mdlId, 3);
                    }
                    RabbitEffectUpdate(-1);
                }
                rabbitDispTimer = 0;
            }
            if (SdFlagPosSet(&rabbitBaseModel)) {
                rabbitMode[0] = RABBIT_MODE_DROP;
                rabbitScaleVal = 1.0f;
                rabbitDropOfs = 2000.0f;
                Hu3DModelPosSet(rabbitBaseModel.mdlId, rabbitBaseModel.pos.x, rabbitBaseModel.pos.y + rabbitDropOfs, rabbitBaseModel.pos.z);
                Hu3DModelAttrReset(rabbitBaseModel.mdlId, 1);
                for (i = RABBIT_TRAIL_NUM - 1; i >= 0; i--) {
                    rabbitModelData[i].pos = rabbitBaseModel.pos;
                    rabbitRot[i] = zero;
                }
            }
            break;
        case RABBIT_MODE_MOVE:
            RabbitModeMove();
            RabbitEffectUpdate(0);
            break;
        case RABBIT_MODE_CAUGHT:
            if (rabbitBaseModel.hitF != 0) {
                rabbitMode[0] = RABBIT_MODE_SHRINK;
                rabbitAlpha = 1.0f;
                rabbitScale = one;
                rabbitShrinkTimer[2] = 0;
                rabbitShrinkTimer[1] = 0;
                rabbitShrinkTimer[0] = 0;
            } else {
                rabbitMode[0] = RABBIT_MODE_RESPAWN;
            }
            break;
        case RABBIT_MODE_RESPAWN:
            if (SdFlagPosSet(&rabbitBaseModel)) {
                if (rabbitBaseModel.hitF != 0) {
                    rabbitMode[0] = RABBIT_MODE_DROP;
                    rabbitDropOfs = 2000.0f;
                    Hu3DModelPosSet(rabbitBaseModel.mdlId, rabbitBaseModel.pos.x, rabbitBaseModel.pos.y + rabbitDropOfs, rabbitBaseModel.pos.z);
                    Hu3DModelAttrReset(rabbitBaseModel.mdlId, 1);
                } else if (rabbitOverridePos == 0) {
                    impactPos = rabbitBaseModel.pos;
                    SdImpactSoundStart(0x56B, &impactPos, 1.0f);
                    rabbitMode[0] = RABBIT_MODE_APPEAR;
                    rabbitEffectTimer = 0;
                }
                rabbitMode[2] = 1;
                for (i = RABBIT_TRAIL_NUM - 1; i >= 0; i--) {
                    rabbitModelData[i].pos = rabbitBaseModel.pos;
                    rabbitRot[i] = zero;
                }
            }
            break;
        case RABBIT_MODE_SHRINK:
            if (RabbitShrinkCheck()) {
                rabbitAlpha -= 0.02f;
                if (rabbitAlpha < 0.0f) {
                    rabbitAlpha = 0.0f;
                }
                Hu3DModelTPLvlSet(rabbitBaseModel.mdlId, rabbitAlpha);
                if (0.0f == rabbitAlpha) {
                    if (rabbitBaseModel.timer[0] == 3 || rabbitBaseModel.timer[1] == 3) {
                        rabbitMode[0] = RABBIT_MODE_END;
                    } else {
                        rabbitMode[0] = RABBIT_MODE_RESPAWN;
                    }
                    rabbitScaleVal = 1.0f;
                    Hu3DModelScaleSet(rabbitBaseModel.mdlId, rabbitScaleVal, rabbitScaleVal, rabbitScaleVal);
                    Hu3DModelAttrSet(rabbitBaseModel.mdlId, 1);
                    Hu3DModelTPLvlSet(rabbitBaseModel.mdlId, 1.0f);
                    rabbitMode[1] = 0;
                    RabbitEffectUpdate(-1);
                }
            }
            break;
        case RABBIT_MODE_DROP:
            Hu3DModelPosSet(rabbitBaseModel.mdlId, rabbitBaseModel.pos.x, rabbitBaseModel.pos.y + rabbitDropOfs, rabbitBaseModel.pos.z);
            rabbitDropOfs -= 10.0f;
            if (rabbitDropOfs < 0.0f) {
                rabbitDropOfs = 0.0f;
            }
            if (0.0f == rabbitDropOfs) {
                rabbitBaseModel.hitF = 0;
                rabbitMode[0] = RABBIT_MODE_MOVE;
            }
            Hu3DModelRotSet(rabbitBaseModel.mdlId, 0.0f, 0.0f, 0.0f);
            break;
        case RABBIT_MODE_APPEAR:
            if (++rabbitEffectTimer == 3) {
                RabbitEffectPosSet();
            }
            rabbitScaleVal += 0.05f;
            if (rabbitScaleVal > 1.0f) {
                rabbitScaleVal = 1.0f;
            }
            Hu3DModelScaleSet(rabbitBaseModel.mdlId, rabbitScaleVal, rabbitScaleVal, rabbitScaleVal);
            if (1.0f == rabbitScaleVal) {
                rabbitMode[0] = RABBIT_MODE_MOVE;
            }
            PSMTXRotRad(rotMtx, 'y', 0.7853982f);
            PSMTXConcat(rabbitTransform.mtx, rotMtx, rabbitTransform.mtx);
            MathMtxRotGet(rabbitTransform.mtx, &rabbitTransform.rot);
            Hu3DModelRotSetV(rabbitBaseModel.mdlId, &rabbitTransform.rot);
            break;
        case RABBIT_MODE_END:
        default:
            break;
    }
    SdFlagSprUpdate(&rabbitBaseModel);
}

void SdRabbitClose(void)
{
    s32 i;
    s32 j;

    if (sys->gameType == 2) {
        if (rabbitBaseModel.mdlId != -1) {
            for (i = 0; i < RABBIT_TRAIL_NUM; i++) {
                Hu3DModelKill(rabbitModelData[i].mdlId);
            }
        }
        if (rabbitBaseModel.mdlId != -1) {
            Hu3DModelKill(rabbitBaseModel.mdlId);
        }
        for (i = 0; i < 2; i++) {
            for (j = 0; j < 3; j++) {
                if (rabbitBaseModel.spr[i][j] != -1) {
                    espKill(rabbitBaseModel.spr[i][j]);
                }
            }
        }
        Hu3DModelKill(rabbitEffectMdlId);
    }
}

static void RabbitModeMove(void)
{
    s32 i;
    s32 moved = 0;
    f32 dist;
    f32 turn;
    Vec zAxis = { 0.0f, 0.0f, 0.0f };
    Vec dir;
    Vec step;
    Vec newPos;
    Vec tmp;

    switch (rabbitMode[2]) {
        case 0:
            rabbitMode[2]++;
            break;
        case 1:
            if (randomMoveTimer == 0) {
                randomMoveTimer = frandmod(12) * 10;
            }
            if (sys->machine[0].bulletType == 7 || sys->machine[1].bulletType == 7) {
                RabbitTurn(0.017453292f * randomMoveTimer);
                randomMoveTimer = 0;
                moved = 1;
            } else {
                RabbitTurn(0.05235988f);
                randomMoveTimer--;
                if (randomMoveTimer <= 0) {
                    randomMoveTimer = 0;
                    moved = 1;
                }
            }
            break;
        case 2:
            if (rabbitMoveDist > 1000.0f) {
                moved = 1;
            }
            break;
        case 3:
            if (rabbitMoveDist > 2000.0f) {
                moved = 1;
            }
            break;
        case 4:
            if (rabbitIdleTimer > rabbitIdleTimerMax) {
                moved = 1;
            }
            break;
        case 5:
            if (++rabbitEffectTimer == 3) {
                RabbitEffectPosSet();
            }
            rabbitScaleVal -= 0.05f;
            if (rabbitScaleVal < 0.0f) {
                rabbitScaleVal = 0.0f;
            }
            Hu3DModelScaleSet(rabbitBaseModel.mdlId, rabbitScaleVal, rabbitScaleVal, rabbitScaleVal);
            if (0.0f == rabbitScaleVal) {
                rabbitImpactTimer[1] = 0;
                rabbitImpactTimer[0] = 0;
                rabbitMode[0] = RABBIT_MODE_CAUGHT;
                rabbitBaseModel.pos.y = -1000.0f;
                Hu3DModelPosSetV(rabbitBaseModel.mdlId, &rabbitBaseModel.pos);
            }
            RabbitTurn(0.7853982f);
            return;
        default:
            break;
    }
    if (moved) {
        rabbitPrevMoveSpeed = rabbitMoveSpeed;
        rabbitMoveSpeed = frandmod(3) * 2 + 3;
        rabbitMoveDist = 0.0f;
        rabbitIdleTimerMax = frandmod(3) * 30 + 60;
        rabbitIdleTimer = 0;
        if (rabbitMode[2] == 1) {
            rabbitMode[2]++;
        } else if (sys->machine[0].bulletType == 7 || sys->machine[1].bulletType == 7) {
            rabbitMode[2] = frandmod(3) + 1;
        } else {
            rabbitMode[2] = frandmod(4) + 1;
            if (rabbitMode[2] == 4) {
                rabbitMoveSpeed = -2.0f;
            }
        }
        if (-2.0f == rabbitMoveSpeed || (rabbitPrevMoveSpeed >= 0.0f && rabbitMoveSpeed - rabbitPrevMoveSpeed >= 2.0f)) {
            tmp = rabbitBaseModel.pos;
            SdImpactSoundStart((rand8() % 2) * 2 + 0x56D, &tmp, 1.0f);
        }
        if (frandmod(2) != 0) {
            turn = 1.0f;
        } else {
            turn = -1.0f;
        }
        rabbitTurnDir = turn;
    } else {
        if (rabbitMode[2] != 4) {
            dist = 100.0f * (2.0f + rabbitMoveSpeed) / 60.0f;
            rabbitMoveDist += dist;
            zAxis.x = rabbitTransform.mtx[0][2];
            zAxis.y = rabbitTransform.mtx[1][2];
            zAxis.z = rabbitTransform.mtx[2][2];
            dir = zAxis;
            PSVECScale(&dir, &step, dist);
            PSVECAdd(&rabbitBaseModel.pos, &step, &newPos);
            rabbitSpeed = dist;
            rabbitZAxis = dir;
            tmp = newPos;
            if (RabbitStageEdgeCheck(&tmp)) {
                rabbitBaseModel.pos = newPos;
                tmp = rabbitBaseModel.pos;
                rabbitBaseModel.pos.y = SdStageDistCalc(&tmp);
                Hu3DModelPosSetV(rabbitBaseModel.mdlId, &rabbitBaseModel.pos);
            } else {
                rabbitMode[2] = 1;
            }
        } else {
            rabbitIdleTimer++;
        }
        if (RabbitImpactDirGet() == -1 && rabbitOverridePos == 0) {
            rabbitMode[2] = 5;
            tmp = rabbitBaseModel.pos;
            SdImpactSoundStart(0x569, &tmp, 1.0f);
            rabbitEffectTimer = 0;
        }
        RabbitRotUpdate();
    }
    if (rabbitDispTimer == 0) {
        for (i = RABBIT_TRAIL_NUM - 1; i >= 0; i--) {
            if (i == 0) {
                rabbitModelData[i].pos = rabbitBaseModel.pos;
                rabbitRot[i] = rabbitTransform.rot;
            } else {
                rabbitModelData[i].pos = rabbitModelData[i - 1].pos;
                rabbitRot[i] = rabbitRot[i - 1];
            }
            Hu3DModelPosSet(rabbitModelData[i].mdlId, rabbitModelData[i].pos.x, rabbitModelData[i].pos.y, rabbitModelData[i].pos.z);
            Hu3DModelRotSetV(rabbitModelData[i].mdlId, &rabbitRot[i]);
            Hu3DModelTPLvlSet(rabbitModelData[i].mdlId, (rabbitFadeTimer - rabbitFadeTimer / 5.0f * i) * 0.25f);
            if (rabbitMode[2] != 5) {
                Hu3DModelAttrReset(rabbitModelData[i].mdlId, 1);
            }
        }
    }
    rabbitDispTimer++;
    if (rabbitDispTimer == 15) {
        rabbitDispTimer = 0;
    }
}

static s32 RabbitStageEdgeCheck(Vec *pos)
{
    s32 ret = 0;
    f32 height;
    Vec tmp;

    tmp = *pos;
    if (SdStageEdgeDistGet(&tmp) >= 200.0f) {
        tmp = *pos;
        height = SdStageDistCalc(&tmp);
        if (SdStageRadiusCalc() < height) {
            tmp = *pos;
            if (!SdStageBaseObjHitCheck(&tmp, 69.0f)) {
                ret = 1;
            }
        }
    }
    return ret;
}

static void RabbitRotUpdate(void)
{
    s32 i;
    Vec pts[3];
    Vec axisZ;
    Vec axisY;
    Vec axisX;
    Mtx trans;
    Vec v;
    Vec tmp;

    for (i = 0; i < 3; i++) {
        PSMTXTrans(trans, rabbitTransform.probe[i].x, rabbitTransform.probe[i].y, rabbitTransform.probe[i].z);
        PSMTXConcat(rabbitTransform.mtx, trans, trans);
        v.x = 0.0f;
        v.y = 0.0f;
        v.z = 0.0f;
        v.x = trans[0][3];
        v.y = trans[1][3];
        v.z = trans[2][3];
        pts[i] = v;
        PSVECAdd(&pts[i], &rabbitBaseModel.pos, &pts[i]);
    }
    for (i = 0; i < 3; i++) {
        tmp = pts[i];
        pts[i].y = SdStageDistCalc(&tmp);
    }
    PSVECSubtract(&pts[0], &pts[2], &axisZ);
    PSVECNormalize(&axisZ, &axisZ);
    PSVECSubtract(&pts[1], &pts[0], &axisX);
    PSVECNormalize(&axisX, &axisX);
    PSVECCrossProduct(&axisX, &axisZ, &axisY);
    PSVECNormalize(&axisY, &axisY);
    PSVECCrossProduct(&axisY, &axisZ, &axisX);
    PSVECNormalize(&axisX, &axisX);
    rabbitTransform.mtx[0][0] = axisX.x;
    rabbitTransform.mtx[1][0] = axisX.y;
    rabbitTransform.mtx[2][0] = axisX.z;
    rabbitTransform.mtx[0][1] = axisY.x;
    rabbitTransform.mtx[1][1] = axisY.y;
    rabbitTransform.mtx[2][1] = axisY.z;
    rabbitTransform.mtx[0][2] = axisZ.x;
    rabbitTransform.mtx[1][2] = axisZ.y;
    rabbitTransform.mtx[2][2] = axisZ.z;
    MathMtxRotGet(rabbitTransform.mtx, &rabbitTransform.rot);
    Hu3DModelRotSetV(rabbitBaseModel.mdlId, &rabbitTransform.rot);
}

static s32 RabbitImpactDirGet(void)
{
    s32 ret = 0;
    s32 i;
    SdMachine *m0 = &sys->machine[0];
    SdMachine *m1 = &sys->machine[1];
    Vec p0 = m0->curPos;
    Vec p1 = m1->curPos;
    Vec dir;
    Vec def = { 1.0f, 1.0f, 1.0f };
    Mtx rotMtx;
    f32 dist;

    PSVECSubtract(&p0, &rabbitBaseModel.pos, &dir);
    dist = PSVECMag(&dir);
    if (dist <= 300.0f + m0->hitRadius) {
        if (0.0f == PSVECMag(&dir)) {
            dir = def;
        }
        PSVECNormalize(&dir, &dir);
        PSMTXRotRad(rotMtx, 'y', 0.017453292f * (120.0f * rabbitTurnDir));
        PSMTXMultVec(rotMtx, &dir, &dir);
        MathPlaneRotMtxGet(&dir, NULL, 0.0f, rabbitTransform.mtx);
        MathMtxRotGet(rabbitTransform.mtx, &rabbitTransform.rot);
        Hu3DModelRotSetV(rabbitBaseModel.mdlId, &rabbitTransform.rot);
        rabbitImpactTimer[0]++;
        rabbitMoveSpeed = 18.0f;
        ret = (rabbitImpactTimer[0] > 30) ? -1 : 1;
    } else {
        rabbitImpactTimer[0] = 0;
        if (HuPadBtn[m0->padNo] & 0x200) {
            if (m0->bulletType == 1 || m0->bulletType == 5 || m0->bulletType == 10) {
                if (dist <= 1200.0f + m0->hitRadius) {
                    ret = (frandmod(3) != 0) ? ret : -1;
                }
            } else {
                if (0.0f == PSVECMag(&dir)) {
                    dir = def;
                }
                PSVECNormalize(&dir, &dir);
                PSMTXRotRad(rotMtx, 'y', 0.017453292f * (90.0f * rabbitTurnDir));
                PSMTXMultVec(rotMtx, &dir, &dir);
                MathPlaneRotMtxGet(&dir, NULL, 0.0f, rabbitTransform.mtx);
                MathMtxRotGet(rabbitTransform.mtx, &rabbitTransform.rot);
                Hu3DModelRotSetV(rabbitBaseModel.mdlId, &rabbitTransform.rot);
                rabbitMoveSpeed = (frandmod(2) != 0) ? 10.0f : 8.0f;
            }
        }
    }

    PSVECSubtract(&p1, &rabbitBaseModel.pos, &dir);
    dist = PSVECMag(&dir);
    if (dist <= 300.0f + m1->hitRadius) {
        if (0.0f == PSVECMag(&dir)) {
            dir = def;
        }
        PSVECNormalize(&dir, &dir);
        PSMTXRotRad(rotMtx, 'y', 0.017453292f * (120.0f * rabbitTurnDir));
        PSMTXMultVec(rotMtx, &dir, &dir);
        MathPlaneRotMtxGet(&dir, NULL, 0.0f, rabbitTransform.mtx);
        MathMtxRotGet(rabbitTransform.mtx, &rabbitTransform.rot);
        Hu3DModelRotSetV(rabbitBaseModel.mdlId, &rabbitTransform.rot);
        rabbitImpactTimer[1]++;
        rabbitMoveSpeed = 18.0f;
        ret = (rabbitImpactTimer[1] > 30) ? -1 : ((ret == 0) ? 1 : ret);
    } else {
        rabbitImpactTimer[1] = 0;
        if (HuPadBtn[m1->padNo] & 0x200) {
            if (m1->bulletType == 1 || m1->bulletType == 5 || m1->bulletType == 10) {
                if (dist <= 1200.0f + m1->hitRadius) {
                    ret = (frandmod(3) != 0) ? ret : -1;
                }
            } else {
                if (0.0f == PSVECMag(&dir)) {
                    dir = def;
                }
                PSVECNormalize(&dir, &dir);
                PSMTXRotRad(rotMtx, 'y', 0.017453292f * (90.0f * rabbitTurnDir));
                PSMTXMultVec(rotMtx, &dir, &dir);
                MathPlaneRotMtxGet(&dir, NULL, 0.0f, rabbitTransform.mtx);
                MathMtxRotGet(rabbitTransform.mtx, &rabbitTransform.rot);
                Hu3DModelRotSetV(rabbitBaseModel.mdlId, &rabbitTransform.rot);
                rabbitMoveSpeed = (frandmod(2) != 0) ? 10.0f : 8.0f;
            }
        }
    }
    if (ret == -1) {
        for (i = 0; i < RABBIT_TRAIL_NUM; i++) {
            Hu3DModelAttrSet(rabbitModelData[i].mdlId, 1);
        }
    }
    return ret;
}

static s32 RabbitShrinkCheck(void)
{
    s32 ret = 0;
    s32 i;

    if (rabbitShrinkTimer[0] != 0) {
        Hu3DModelAttrReset(rabbitBaseModel.mdlId, 1);
    } else {
        Hu3DModelAttrSet(rabbitBaseModel.mdlId, 1);
    }
    if (++rabbitShrinkTimer[1] == 2) {
        rabbitShrinkTimer[0] ^= 1;
        rabbitShrinkTimer[1] = 0;
        rabbitShrinkTimer[2]++;
    }
    if (rabbitShrinkTimer[2] > 11) {
        rabbitScale.x -= 0.03f;
        if (rabbitScale.x < 0.0f) {
            rabbitScale.x = 0.0f;
        }
        rabbitScale.z = rabbitScale.x;
        rabbitScale.y += 0.05f;
        Hu3DModelScaleSetV(rabbitBaseModel.mdlId, &rabbitScale);
        ret = 1;
    }
    for (i = 0; i < RABBIT_TRAIL_NUM; i++) {
        Hu3DModelAttrSet(rabbitModelData[i].mdlId, 1);
    }
    return ret;
}

static void RabbitEffectCreate(void)
{
    rabbitEffectMdlId = Hu3DParticleCreate(HuSprAnimRead(HuDataSelHeapReadNum(SDDATA(0x8B), HU_MEMNUM_OVL, HEAP_MODEL)), 30);
    Hu3DModelPosSet(rabbitEffectMdlId, 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(rabbitEffectMdlId, 0.2f, 0.2f, 0.2f);
    Hu3DModelLayerSet(rabbitEffectMdlId, 5);
    Hu3DModelCameraSet(rabbitEffectMdlId, 3);
    Hu3DParticleTPLvlSet(rabbitEffectMdlId, 0.1f);
    Hu3DParticleScaleSet(rabbitEffectMdlId, 0.0f);
    Hu3DParticleHookSet(rabbitEffectMdlId, RabbitEffectHook);
    RabbitEffectUpdate(1);
    rabbitOverridePos = 0;
    rabbitImpactTimer[1] = 0;
    rabbitImpactTimer[0] = 0;
}

static void RabbitEffectHook(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx)
{
    s32 doneCnt = 0;
    s32 i;
    HU3DPARTICLEDATA *data;
    f32 angle;
    f32 radius;

    if (rabbitOverridePos) {
        data = particleP->data;
        for (i = 0; i < particleP->maxCnt; i++, data++) {
            data->cameraBit = 3;
            if (0.0f == data->scale) {
                data->pos.z = 0.0f;
                data->pos.y = 0.0f;
                data->pos.x = 0.0f;
                angle = (f32)((frand() & 0xFF) * 360 >> 8);
                radius = (f32)(frand() & 0x1F);
                data->vel.x = (f32)(radius * sin(3.141592653589793 * angle / 180.0));
                data->vel.y = 50.0f + (f32)(frand() & 0x1F);
                data->vel.z = (f32)(radius * cos(3.141592653589793 * angle / 180.0));
                data->scale = 300.0f;
            }
        }
        data = particleP->data;
        for (i = 0; i < particleP->maxCnt; i++, data++) {
            if (data->scale != 0.0f) {
                PSVECAdd(&data->vel, &data->pos, &data->pos);
                data->vel.x *= 0.999f;
                data->vel.y -= 3.0f;
                data->vel.z *= 0.999f;
                data->scale -= 3.0f;
                if (data->scale <= 0.0f) {
                    data->scale = 0.0f;
                }
            }
            if (0.0f == data->scale) {
                doneCnt++;
            }
        }
        if (doneCnt == particleP->maxCnt) {
            rabbitOverridePos = 0;
        }
        DCStoreRangeNoSync(particleP->data, particleP->maxCnt * sizeof(HU3DPARTICLEDATA));
    }
}

static void RabbitEffectPosSet(void)
{
    rabbitOverridePos = 1;
    Hu3DModelPosSetV(rabbitEffectMdlId, &rabbitBaseModel.pos);
}

static void RabbitEffectUpdate(s32 mode)
{
    u8 colB[8] = { 0xFF, 0x64, 0x5A, 0x50, 0xBE, 0x6E, 0xB4, 0x64 };
    u8 colG[8] = { 0xFF, 0x78, 0x46, 0x8C, 0xAA, 0x8C, 0xB4, 0x50 };
    u8 colR[8] = { 0xFF, 0x96, 0x46, 0xC8, 0x5A, 0xAA, 0x96, 0x5A };
    s32 inRange = 0;
    s32 idx;
    s32 i;
    u8 r;
    u8 g;
    u8 b;

    if (sys->stageNo >= 1 && sys->stageNo <= 7) {
        inRange = 1;
    }
    if (inRange) {
        idx = sys->stageNo;
    } else {
        idx = 0;
    }
    if (mode == 0) {
        if (idx > 0) {
            if (rabbitEffectFadeTimer[1] <= 60) {
                rabbitEffectFadeTimer[0]++;
                if (rabbitEffectFadeTimer[0] >= 40) {
                    rabbitEffectFadeTimer[0] = 0;
                    rabbitEffectFadeTimer[1]++;
                }
            }
            r = 255 - (255 - colR[idx]) * rabbitEffectFadeTimer[1] / 60;
            g = 255 - (255 - colG[idx]) * rabbitEffectFadeTimer[1] / 60;
            b = 255 - (255 - colB[idx]) * rabbitEffectFadeTimer[1] / 60;
            LightColorSet(rabbitBaseModel.mdlId, 0xFF, r, g, b);
            for (i = 0; i < RABBIT_TRAIL_NUM; i++) {
                LightColorSet(rabbitModelData[i].mdlId, 0xFF, r, g, b);
            }
        }
    } else if (mode == 1) {
        Hu3DParticleColSet(rabbitEffectMdlId, colR[idx], colG[idx], colB[idx]);
    } else if (mode == -1) {
        rabbitEffectFadeTimer[1] = 0;
        rabbitEffectFadeTimer[0] = 0;
        LightColorSet(rabbitBaseModel.mdlId, 0xFF, colR[0], colG[0], colB[0]);
        for (i = 0; i < RABBIT_TRAIL_NUM; i++) {
            LightColorSet(rabbitModelData[i].mdlId, 0xFF, colR[0], colG[0], colB[0]);
        }
    }
}

void SdRabbitPosGet(Vec *pos)
{
    Vec p;

    if (rabbitMode[2] == 5) {
        p = rabbitBaseModel.pos;
        p.y = -1000.0f;
        *pos = p;
    } else {
        *pos = rabbitBaseModel.pos;
    }
}

s32 SdRabbitCheck(void)
{
    s32 ret;

    if (rabbitMode[0] == 1) {
        ret = 1;
    } else {
        ret = 0;
    }
    return ret;
}

void SdRabbitPlayerCheck(s32 player)
{
    if (rabbitMode[0] == 1) {
        rabbitMode[0]++;
        rabbitBaseModel.timer[player]++;
        rabbitBaseModel.hitF = 1;
    }
}

s32 SdRabbitDoneCheck(void)
{
    return rabbitBaseModel.hitF;
}

s32 SdRabbitDispTimerGet(s32 player)
{
    return rabbitBaseModel.timer[player];
}

f32 SdRabbitSpeedGet(void)
{
    return rabbitSpeed;
}

void SdRabbitZAxisGet(Vec *zAxis)
{
    *zAxis = rabbitZAxis;
}

void SdRabbitSprDispOff(void)
{
    s32 i;
    s32 j;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            if (rabbitBaseModel.spr[i][j] != -1) {
                espDispOff(rabbitBaseModel.spr[i][j]);
            }
        }
    }
}
