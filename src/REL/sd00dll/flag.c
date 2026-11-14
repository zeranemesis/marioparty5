#include "REL/sd00dll_sys.h"
#include "game/audio.h"
#include "game/data.h"
#include "game/esprite.h"
#include "game/frand.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/mg/seqman.h"
#include "math.h"
#include "string.h"

#define SDDATA(n) (0x00BC0000 + (n))

#define FLAG_MODE_WAIT 0
#define FLAG_MODE_IDLE 1
#define FLAG_MODE_PICKED 2
#define FLAG_MODE_RESPAWN 3
#define FLAG_MODE_APPEAR 10
#define FLAG_MODE_VANISH 11
#define FLAG_MODE_END 99

typedef struct SdFlagWork_s {
    HU3DMODELID mdlId; // 0x00
    Vec pos;           // 0x04
    s32 pad10;         // 0x10
    s32 timer[2];      // 0x14 (pickup count per machine)
    s16 spr[2][3];     // 0x1C (HUD flag markers per machine)
} SdFlagWork; // size 0x28

static s32 flagRippleTimer;
static HSFBUFFER *flag2Vtx;
static HSFBUFFER *flag1Vtx;
static s32 flagRespawnTimer;
static f32 flagScale;
static f32 flagRotY;
static s32 flagMode[3]; // [0] mode, [1] sprite setup step
static SdFlagWork flagWork;

static s32 FlagNextPosGet(Vec *out);
static s32 FlagPickupCheck(void);
static void FlagRippleInit(void);
static void FlagRippleUpdate(void);
s32 SdFlagPosSet(SdFlagWork *work);
void SdFlagSprUpdate(SdFlagWork *work);

void SdFlagInit(void)
{
    s32 i;
    s32 j;
    Vec zero = { 0.0f, 0.0f, 0.0f };

    if (sys->gameType == 1) {
        flagMode[0] = 0;
        flagMode[1] = 0;
        flagMode[2] = 0;
        flagWork.mdlId = -1;
        for (i = 0; i < 2; i++) {
            flagWork.timer[i] = 0;
            for (j = 0; j < 3; j++) {
                flagWork.spr[i][j] = -1;
            }
        }
        flagWork.pos = zero;
        flagRespawnTimer = 0;
    }
}

void SdFlagUpdate(void)
{
    if (sys->gameType != 1 || MgSeqModeGet() < 5) {
        return;
    }
    switch (flagMode[0]) {
        case FLAG_MODE_WAIT:
            if (flagWork.mdlId == -1) {
                flagWork.mdlId = Hu3DModelCreate(HuDataSelHeapReadNum(SDDATA(0x68), HU_MEMNUM_OVL, HEAP_MODEL));
                Hu3DModelCameraSet(flagWork.mdlId, 3);
                Hu3DModelAttrSet(flagWork.mdlId, 1);
                Hu3DModelAttrSet(flagWork.mdlId, 0x40000001);
                FlagRippleInit();
            }
            if (SdFlagPosSet(&flagWork)) {
                flagMode[0] = FLAG_MODE_APPEAR;
                flagRotY = 0.0f;
                flagScale = 0.0f;
                Hu3DModelScaleSet(flagWork.mdlId, flagScale, flagScale, flagScale);
                Hu3DModelAttrReset(flagWork.mdlId, 1);
            }
            break;
        case FLAG_MODE_IDLE:
            if (FlagPickupCheck()) {
                flagMode[0] = FLAG_MODE_VANISH;
            }
            break;
        case FLAG_MODE_PICKED:
            Hu3DModelAttrSet(flagWork.mdlId, 1);
            if (flagWork.timer[0] == 3 || flagWork.timer[1] == 3) {
                flagMode[0] = FLAG_MODE_END;
            } else {
                flagMode[0]++;
            }
            break;
        case FLAG_MODE_RESPAWN:
            if (SdFlagPosSet(&flagWork)) {
                flagMode[0] = FLAG_MODE_APPEAR;
                Hu3DModelAttrReset(flagWork.mdlId, 1);
                flagRotY = 0.0f;
            }
            break;
        case FLAG_MODE_APPEAR:
            flagScale += 0.05f;
            if (flagScale > 1.0f) {
                flagScale = 1.0f;
            }
            Hu3DModelScaleSet(flagWork.mdlId, flagScale, flagScale, flagScale);
            if (1.0f == flagScale) {
                flagMode[0] = FLAG_MODE_IDLE;
            }
            break;
        case FLAG_MODE_VANISH:
            flagScale -= 0.05f;
            if (flagScale < 0.0f) {
                flagScale = 0.0f;
            }
            Hu3DModelScaleSet(flagWork.mdlId, flagScale, flagScale, flagScale);
            if (0.0f == flagScale) {
                flagMode[0] = FLAG_MODE_PICKED;
            }
            break;
        case FLAG_MODE_END:
        default:
            break;
    }
    SdFlagSprUpdate(&flagWork);
    FlagRippleUpdate();
}

void SdFlagClose(void)
{
    s32 i;
    s32 j;

    if (sys->gameType == 1) {
        if (flagWork.mdlId != -1) {
            Hu3DModelKill(flagWork.mdlId);
        }
        for (i = 0; i < 2; i++) {
            for (j = 0; j < 3; j++) {
                if (flagWork.spr[i][j] != -1) {
                    espKill(flagWork.spr[i][j]);
                }
            }
        }
    }
}

void SdFlagRotUpdate(void)
{
    f32 rot;

    Hu3DModelRotSet(flagWork.mdlId, 0.0f, flagRotY, 0.0f);
    if (flagRotY < 358.0f) {
        rot = 2.0f + flagRotY;
    } else {
        rot = 0.0f;
    }
    flagRotY = rot;
}

s32 SdFlagPosSet(SdFlagWork *work)
{
    s32 ret = 0;
    Vec nextPos;
    Vec tmp;

    nextPos = work->pos;
    if (FlagNextPosGet(&nextPos)) {
        tmp = nextPos;
        nextPos.y = SdStageDistCalc(&tmp);
        if (-10000.0f != nextPos.y && SdStageRadiusCalc() < nextPos.y) {
            work->pos = nextPos;
            Hu3DModelPosSetV(work->mdlId, &work->pos);
            ret = 1;
        }
    }
    return ret;
}

static s32 FlagNextPosGet(Vec *out)
{
    s32 ret = 0;
    SdMachine *m0 = &sys->machine[0];
    SdMachine *m1 = &sys->machine[1];
    Vec p0 = m0->curPos;
    Vec p1 = m1->curPos;
    Vec cand = *out;
    Vec diff;
    Vec tmp;
    f32 midX;
    f32 midZ;
    f32 slope;
    f32 dist;
    f32 minDist;
    f32 maxDist;
    s32 r;

    cand.y = 0.0f;
    p1.y = 0.0f;
    p0.y = 0.0f;
    if (p0.x != p1.x && p0.z != p1.z) {
        midX = (p0.x + p1.x) * 0.5f;
        midZ = (p0.z + p1.z) * 0.5f;
        slope = (p0.z - p1.z) / (p0.x - p1.x);
        cand.x = frandmod(15000);
        cand.z = midZ + -(1.0f / slope) * (cand.x - midX);
        r = frandmod(100);
        if (r < 5) {
            minDist = 500.0f;
            maxDist = 1000.0f;
        } else if (r < 40) {
            minDist = 1100.0f;
            maxDist = 2000.0f;
        } else if (r < 85) {
            minDist = 2100.0f;
            maxDist = 3000.0f;
        } else {
            minDist = 3100.0f;
            maxDist = 50000.0f;
        }
        PSVECSubtract(&p0, &cand, &diff);
        dist = PSVECMag(&diff);
        if (dist >= minDist && dist <= maxDist) {
            tmp = cand;
            if (SdStageEdgeDistGet(&tmp) >= 200.0f) {
                tmp = cand;
                if (!SdStageBaseObjHitCheck(&tmp, 100.0f)) {
                    *out = cand;
                    ret = 1;
                    flagRespawnTimer = 0;
                }
            }
        }
    }
    if (ret == 0) {
        if (++flagRespawnTimer > 300) {
            SdStageRandomPosGet(&cand, -200.0f);
            tmp = cand;
            if (!SdStageBaseObjHitCheck(&tmp, 100.0f)) {
                *out = cand;
                ret = 1;
                flagRespawnTimer = 0;
            }
        }
    }
    return ret;
}

static s32 FlagPickupCheck(void)
{
    s32 ret = 0;
    SdMachine *m0;
    SdMachine *m1;
    Vec p0;
    Vec p1;
    Vec flagPos;
    Vec diff;
    f32 dist;

    if (MgSeqModeGet() > 5) {
        return 0;
    }
    m0 = &sys->machine[0];
    m1 = &sys->machine[1];
    p0 = m0->curPos;
    p1 = m1->curPos;
    flagPos = flagWork.pos;
    flagPos.y += 150.0f;
    PSVECSubtract(&p0, &flagPos, &diff);
    dist = PSVECMag(&diff);
    if (dist <= 100.0f + m0->hitRadius) {
        flagWork.timer[0]++;
        ret = 1;
    }
    PSVECSubtract(&p1, &flagPos, &diff);
    dist = PSVECMag(&diff);
    if (dist <= 100.0f + m1->hitRadius) {
        if (ret == 0) {
            flagWork.timer[1]++;
            ret = 2;
        } else {
            flagWork.timer[0]--;
            ret = -1;
        }
    }
    if (ret == 1) {
        SdVibrate(m0->playerNo, 2, 15);
    }
    if (ret == 2) {
        SdVibrate(m1->playerNo, 2, 15);
    }
    if (ret == 1 || ret == 2) {
        HuAudFXPlayVolPan(ret + 0x564, 0x7F, (s16)((ret - 1) * 32 + 0x30));
        SdPlayerSoundPlay(&sys->machine[ret - 1], 4);
    }
    return ret;
}

void SdFlagSprUpdate(SdFlagWork *work)
{
    s32 i;
    s32 sprNo;
    f32 scale = 0.5f;

    if (sys->gameType == 1) {
        sprNo = SDDATA(0x9F);
    } else {
        sprNo = SDDATA(0xA0);
    }
    switch (flagMode[1]) {
        case 0:
            for (i = 0; i < 3; i++) {
                work->spr[0][i] = espEntry(sprNo, 0, 0);
                espDrawNoSet(work->spr[0][i], 0);
                espPosSet(work->spr[0][i], 16.0f + 32.0f * scale + 69.0f * i * scale, 40.0f + 32.0f * scale);
                espScaleSet(work->spr[0][i], scale, scale);
                espDispOff(work->spr[0][i]);
                work->spr[1][i] = espEntry(sprNo, 0, 0);
                espDrawNoSet(work->spr[1][i], 0);
                espPosSet(work->spr[1][i], 560.0f - 32.0f * scale - 69.0f * i * scale, 40.0f + 32.0f * scale);
                espScaleSet(work->spr[1][i], scale, scale);
                espDispOff(work->spr[1][i]);
            }
            flagMode[1]++;
            break;
        case 1:
            if (sys->stageClosedF == 0 || sys->spr.winnerNo == -1) {
                if (work->timer[0] > 0 && work->timer[0] <= 3) {
                    espDispOn(work->spr[0][work->timer[0] - 1]);
                }
                if (work->timer[1] > 0 && work->timer[1] <= 3) {
                    espDispOn(work->spr[1][work->timer[1] - 1]);
                }
            }
            break;
        default:
            break;
    }
    for (i = 0; i < 3; i++) {
        SdSprScissorUpdate(work->spr[0][i], 0);
        SdSprScissorUpdate(work->spr[1][i], 1);
    }
}

static void FlagRippleInit(void)
{
    char *names[2] = { "flag", "flag1" };
    HSFDATA *hsf;
    HSFBUFFER *buf;
    s32 i;
    s32 j;

    flagRippleTimer = 0;
    flag2Vtx = NULL;
    flag1Vtx = NULL;
    hsf = Hu3DData[flagWork.mdlId].hsf;
    for (i = 0; i < hsf->vertexNum; i++) {
        buf = &hsf->vertex[i];
        for (j = 0; j < 2; j++) {
            if (strlen(names[j]) == strlen(buf->name) && memcmp(names[j], buf->name, strlen(names[j])) == 0) {
                if (j == 0) {
                    flag1Vtx = buf;
                } else {
                    flag2Vtx = buf;
                }
            }
        }
    }
}

static void FlagRippleUpdate(void)
{
    s32 step;
    f32 amp;
    f32 wave;
    f32 ang;
    f32 val;
    f32 dz;
    HSFBUFFER *b1;
    HSFBUFFER *b2;
    s32 i;

    if (flag1Vtx == NULL || flag2Vtx == NULL) {
        return;
    }
    step = 8;
    amp = 0.2f;
    wave = 24.0f;
    flagRippleTimer -= step;
    b1 = flag1Vtx;
    b2 = flag2Vtx;
    for (i = 0; i < b1->count; i++) {
        val = amp * (((Vec *)b1->data)[i].x - 10.0f);
        ang = (f32)flagRippleTimer + (f32)(i / 2) * wave;
        dz = (f32)(val * sin(3.141592653589793 * ang / 180.0));
        ((Vec *)b1->data)[i].z = dz;
        ((Vec *)b2->data)[i].z = dz;
    }
}

void SdFlagPosGet(Vec *pos)
{
    *pos = flagWork.pos;
}

s32 SdFlagCheck(void)
{
    s32 ret;

    if (flagMode[0] == 1) {
        ret = 1;
    } else {
        ret = 0;
    }
    return ret;
}

s32 SdFlagDispTimerGet(s32 no)
{
    return flagWork.timer[no];
}

void SdFlagSprDispOff(void)
{
    s32 i;
    s32 j;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            if (flagWork.spr[i][j] != -1) {
                espDispOff(flagWork.spr[i][j]);
            }
        }
    }
}
