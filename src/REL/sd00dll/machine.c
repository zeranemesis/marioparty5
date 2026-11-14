#include "REL/sd00dll_sys.h"
#include "game/audio.h"
#include "game/data.h"
#include "game/esprite.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/mg/seqman.h"
#include "math.h"
#include "string.h"

#define SDDATA(n) (0x00BC0000 + (n))

typedef struct SdESPRITE_s {
    s16 memberNo;
    s16 animNo;
} SdESPRITE;

SHARED_SYM extern SdESPRITE esprite[];

typedef struct SdMachineBody_s {
    s32 dataNum;
    s32 stat0;     // 0x04
    s32 stat1;     // 0x08
    s32 stat2;     // 0x0C
    f32 radius;    // 0x10
    f32 height;    // 0x14
    s32 cornerNum; // 0x18
} SdMachineBody;

typedef struct SdMachineGun_s {
    s32 dataNum;    // 0x00
    s32 bulletData; // 0x04 (-1: none)
    s32 unk8;       // 0x08 (stat)
    s32 stat;       // 0x0C
    s32 gunNum;     // 0x10
    s32 bulletNum;  // 0x14
    s32 timerInit;  // 0x18
} SdMachineGun;

typedef struct SdMachineEngine_s {
    s32 unk0;
    s32 stat0;  // 0x04
    s32 stat1;  // 0x08
    s32 stat2;  // 0x0C
    s32 unk10;  // 0x10
} SdMachineEngine;

s32 rand8(void);

static void MapSprCreate(void);
static void MapSprKill(void);
static void ScoreSprPosGet(Vec *out, Vec *in);
static void SdMachinePartcleHook(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);
static void SdImpactSet(s32 snd, Vec *pos, s32 vibrate, f32 vol, f32 range);
HU3DMODELID SdMachineBodyCreate(s32 dataNum, s16 *modelIdOut);
void SdMachineTireCreate(s32 tireType, s16 *modelIdOut);
HU3DMODELID SdMachineGunCreate(s32 dataNum, s16 *modelIdOut);
HU3DMODELID SdMachineCharCreate(s32 dataNum, s16 *modelIdOut);

static s32 effectFileTbl[12] = {
    SDDATA(0x8C), SDDATA(0x8F), SDDATA(0x92), SDDATA(0x8B), SDDATA(0x8C), SDDATA(0x8D),
    SDDATA(0x8E), SDDATA(0x90), SDDATA(0x91), SDDATA(0x93), SDDATA(0x8B), SDDATA(0x94),
};

static s32 randomTireTbl[7] = { 1, 2, 3, 4, 5, 6, 10 };

static s32 randomEngineTbl[9] = { 1, 2, 3, 4, 5, 6, 7, 9, 10 };

static char hookNameC[12] = "itemhook_c";

static s32 engineSeIdTbl[11] = { 0x533, 0x533, 0x533, 0x533, 0x533, 0x535, 0x539, 0x539, 0x533, 0x537, 0x533 };

/* skid sound per tire type (0: none) */
static s32 skidSeIdTbl[11] = { 0x525, 0x525, 0x531, 0x525, 0, 0x527, 0x531, 0, 0x52D, 0, 0x527 };

static s32 gunSeIdTbl[13] = { 0x53B, 0x53D, 0x53F, 0x541, 0x543, 0x545, 0x547,
                              0x549, 0x54B, 0x54D, 0x54F, 0x551, 0x553 };

static s32 impactSeIdTbl[13] = { 0x555, 0x555, 0x559, 0x557, 0x555, 0x555, 0, 0x555, 0x559, 0x557, 0x55B, 0, 0x559 };

/* configPartTbl: indexed [presetNo + 1] (entry 0 is the default config) */
static s32 playerSeIdTbl[12][6] = {
    { 0x1FF, 0x1F9, 0x1F8, 0x200, 0x1F8, 0x1FA },
    { 0x1EB, 0x1E5, 0x1E4, 0x1EC, 0x1E4, 0x1E6 },
    { 0x263, 0x25D, 0x25C, 0x264, 0x25C, 0x25E },
    { 0x2B3, 0x2AD, 0x2AC, 0x2B4, 0x2AC, 0x2AE },
    { 0x28B, 0x285, 0x284, 0x28C, 0x284, 0x286 },
    { 0x1C3, 0x1BD, 0x1BC, 0x1C4, 0x1BC, 0x1BE },
    { 0x29F, 0x299, 0x298, 0x2A0, 0x298, 0x29A },
    { 0x1D7, 0x1D1, 0x1D0, 0x1D8, 0x1D0, 0x1D2 },
    { 0x277, 0x271, 0x270, 0x278, 0x270, 0x272 },
    { 0x23B, 0x235, 0x234, 0x23C, 0x234, 0x236 },
    { 0x2DD, 0x2DD, 0x2DB, 0x2DD, 0x2DB, 0x2DB },
    { 0x2CB, 0x2CA, 0x2C9, 0x2CA, 0x2C9, 0x2C9 },
};
static s32 configPartTbl[13][5] = {
    { 8, 8, 8, 8, 10 },
    { 8, 8, 8, 8, 10 },
    { 2, 4, 9, 6, 11 },
    { 6, 9, 3, 1, 1 },
    { 3, 1, 6, 9, 0 },
    { 9, 7, 5, 3, 2 },
    { 2, 6, 5, 5, 4 },
    { 7, 5, 4, 7, 8 },
    { 4, 1, 2, 4, 3 },
    { 5, 1, 3, 2, 6 },
    { 1, 2, 7, 3, 5 },
    { 0, 0, 0, 0, 7 },
    { 1, 2, 6, 1, 9 },
};

static s32 playerModelTbl[12] = {
    SDDATA(0x12), SDDATA(0x13), SDDATA(0x14), SDDATA(0x15), SDDATA(0x16), SDDATA(0x17),
    SDDATA(0x18), SDDATA(0x19), SDDATA(0x1A), SDDATA(0x1B), SDDATA(0x1C), SDDATA(0x1D),
};

SdMachineTire sdMachineTireTbl[11] = {
    { { SDDATA(0x1E), SDDATA(0x1F), SDDATA(0x1E), SDDATA(0x1F) }, 0, 0, 0, 50.0f, 50.0f, 50.0f, 30.0f, 0.0f, 0.0f, 0 },
    { { SDDATA(0x20), SDDATA(0x21), SDDATA(0x22), SDDATA(0x23) }, 3, 3, -1, 50.0f, 40.0f, 60.0f, 35.0f, 0.0f, 0.0f, 0 },
    { { SDDATA(0x24), SDDATA(0x25), SDDATA(0x24), SDDATA(0x25) }, -3, -3, 0, 56.0f, 56.0f, 56.0f, 50.0f, 0.0f, 80.0f, 1 },
    { { SDDATA(0x26), SDDATA(0x27), SDDATA(0x26), SDDATA(0x27) }, 0, 1, -4, 50.0f, 50.0f, 50.0f, 20.0f, 0.0f, 0.0f, 0 },
    { { SDDATA(0x28), SDDATA(0x29), SDDATA(0x28), SDDATA(0x29) }, 0, -1, -2, 50.0f, 50.0f, 50.0f, 28.0f, 0.0f, 0.0f, 16 },
    { { SDDATA(0x2A), SDDATA(0x2B), SDDATA(0x2A), SDDATA(0x2B) }, 4, 4, -1, 40.0f, 40.0f, 40.0f, 40.0f, 0.0f, 110.0f, 1 },
    { { SDDATA(0x2C), SDDATA(0x2D), SDDATA(0x2C), SDDATA(0x2D) }, -4, -4, -1, 56.0f, 56.0f, 56.0f, 50.0f, 0.0f, 80.0f, 1 },
    { { SDDATA(0x2E), SDDATA(0x2E), SDDATA(0x2E), SDDATA(0x2E) }, -1, -2, 3, 30.0f, 90.0f, 90.0f, 80.0f, 0.0f, 80.0f, 82 },
    { { SDDATA(0x2F), SDDATA(0x30), SDDATA(0x2F), SDDATA(0x30) }, 0, 0, 0, 50.0f, 50.0f, 50.0f, 40.0f, 0.0f, 0.0f, 128 },
    { { SDDATA(0x31), SDDATA(0x31), SDDATA(0x31), SDDATA(0x31) }, -1, -1, 2, 70.0f, 50.0f, 50.0f, 100.0f, 0.0f, 100.0f, 82 },
    { { SDDATA(0x32), SDDATA(0x33), SDDATA(0x32), SDDATA(0x33) }, 0, 1, 1, 50.0f, 50.0f, 50.0f, 28.0f, 0.0f, 0.0f, 32 },
};

static SdMachineBody sdMachineBodyTbl[11] = {
    { SDDATA(0x34), 0, 0, 0, 120.0f, 180.0f, 2 },
    { SDDATA(0x35), 2, -2, 0, 130.0f, 220.0f, 4 },
    { SDDATA(0x36), 0, 4, 0, 125.0f, 180.0f, 1 },
    { SDDATA(0x37), 3, 1, 0, 90.0f, 240.0f, 2 },
    { SDDATA(0x38), 0, 2, 1, 90.0f, 180.0f, 1 },
    { SDDATA(0x39), -3, 1, 3, 120.0f, 170.0f, 2 },
    { SDDATA(0x3A), 4, -4, 0, 90.0f, 160.0f, 2 },
    { SDDATA(0x3B), 2, -1, 1, 90.0f, 200.0f, 2 },
    { SDDATA(0x3C), -4, 3, 4, 80.0f, 180.0f, 2 },
    { SDDATA(0x3D), 2, 2, 2, 90.0f, 185.0f, 4 },
    { SDDATA(0x3E), -1, 1, 2, 80.0f, 190.0f, 2 },
};

static SdMachineGun sdMachineGunTbl[13] = {
    { SDDATA(0x3F), SDDATA(0x4C), 0, -1, 1, 5, 24 },
    { SDDATA(0x40), SDDATA(0x4D), 3, -1, 1, 9, 6 },
    { SDDATA(0x41), SDDATA(0x4E), -2, 2, 1, 3, 1 },
    { SDDATA(0x42), SDDATA(0x4F), -1, 0, 1, 4, 30 },
    { SDDATA(0x43), SDDATA(0x50), 0, 1, 2, 4, 18 },
    { SDDATA(0x44), -1, 0, 2, 1, 2, 1 },
    { SDDATA(0x45), -1, -1, 3, 1, 1, 1 },
    { SDDATA(0x46), SDDATA(0x51), 2, 2, 1, 3, 40 },
    { SDDATA(0x47), SDDATA(0x52), -3, 4, 1, 2, 60 },
    { SDDATA(0x48), SDDATA(0x53), 0, 1, 1, 4, 18 },
    { SDDATA(0x49), SDDATA(0x54), -3, 4, 1, 1, 60 },
    { SDDATA(0x4A), -1, -3, 4, 1, 1, 1 },
    { SDDATA(0x4B), SDDATA(0x55), -2, 2, 1, 4, 30 },
};

static SdMachineEngine sdMachineEngineTbl[11] = {
    { -1, 0, 0, 0, 0 },
    { -1, 3, 3, -1, 0 },
    { -1, -3, -3, 0, 0 },
    { -1, 0, 1, -3, 0 },
    { -1, -1, -2, -2, 0 },
    { -1, 4, 4, -1, 0 },
    { -1, -4, -4, -1, 0 },
    { -1, 1, 1, -4, 0 },
    { -1, 0, 0, 0, 0 },
    { -1, -2, 2, -1, 0 },
    { -1, 1, 2, -3, 1 },
};

static SdBulletParam sdBulletParam[13] = {
    { 1, 5, 5, 2400.0f, 0.0f, 30.0f, 0.0f, 0.0f, 100.0f, 0, 0, 0, 16, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 10, 8, 1800.0f, 0.0f, 30.0f, 0.0f, 0.0f, 100.0f, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0 },
    { 2, 3, 3, 1680.0f, 60.0f, 40.0f, 400.0f, 0.0f, 100.0f, 0, 0, 0, 64, 25, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 4, 5, 180.0f, 0.0f, 30.0f, 200.0f, 90.0f, 100.0f, 0, 0, 0, 64, 25, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 4, 4, 2400.0f, 0.0f, 30.0f, 0.0f, 0.0f, 600.0f, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 2, 6, 6, 3600.0f, 0.0f, 70.0f, 0.0f, 0.0f, 100.0f, 0, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 6, 6, 0.0f, 0.0f, 30.0f, 0.0f, 0.0f, 100.0f, 0, 0, 0, 0, 0, 30, 0, 0, 0, 0, 0, 0 },
    { 0, 3, 15, 3600.0f, 0.0f, 30.0f, 0.0f, 0.0f, 100.0f, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0 },
    { 0, 2, 4, 2880.0f, 0.0f, 30.0f, 400.0f, 0.0f, 100.0f, 0, 0, 0, 64, 25, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 7, 4, 300.0f, 0.0f, 30.0f, 300.0f, 60.0f, 100.0f, 0, 0, 0, 64, 25, 0, 0, 0, 0, 0, 20, 0 },
    { 0, 2, 3, 2880.0f, 0.0f, 30.0f, 600.0f, 0.0f, 100.0f, 0, 0, 0, 64, 25, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 2, 4, 2880.0f, 0.0f, 40.0f, 0.0f, 0.0f, 100.0f, 0, 0, 0, 8, 0, 0, 0, 0, 12, 0, 0, 0 },
    { 1, 5, 5, 110.0f, 60.0f, 40.0f, 400.0f, 0.0f, 100.0f, 0, 0, 0, 64, 25, 0, 0, 0, 0, 0, 0, 0 },
};

static f32 machineYOfsTbl[11] = { -22.0f, -38.0f, -26.0f, -33.0f, 0.0f, 0.0f, 0.0f, -16.0f, -15.0f, -38.0f, -22.0f };


char *sdMachineHookTbl[17] = {
    "itemhook_a",  "itemhook_b",  "itemhook_e", "itemhook_f",  "itemhook_m",  "itemhook_n",
    "itemhook_gf", "itemhook_v",  "itemhook_w", "itemhook_x",  "itemhook_w2", "itemhook_x2",
    "itemhook_z",  "itemhook_t1", "itemhook_t2", "itemhook_t3", "itemhook_t4",
};

static char *sdMachineTireHookTbl[3] = { "chn_tl", "chn_tr", "chn_hover" };

#define STAT_CLAMP(i)                       do {                                        s32 v;                                  if (machine->stat[i] > 8) {                 v = 8;                              } else {                                    if (machine->stat[i] < 1) {                 v = 1;                              } else {                                    v = machine->stat[i];               }                                   }                                       machine->stat[i] = v;               } while (0)

void SdMachineParamSet(SdMachine *machine, s32 tireType, s32 bodyType, s32 engineType, s32 gunType)
{
    machine->stat[0] = sdMachineTireTbl[tireType].stat0 + sdMachineEngineTbl[engineType].stat0 + 5;
    machine->stat[1] = sdMachineTireTbl[tireType].stat1 + sdMachineBodyTbl[bodyType].stat0
                       + sdMachineEngineTbl[engineType].stat1 + sdMachineGunTbl[gunType].unk8 + 5;
    machine->stat[2] = sdMachineTireTbl[tireType].stat2 + sdMachineEngineTbl[engineType].stat2 + 5;
    machine->stat[3] = sdMachineBodyTbl[bodyType].stat2 + sdMachineGunTbl[gunType].stat + 4;
    machine->stat[4] = sdMachineBodyTbl[bodyType].stat1 + 5;
    STAT_CLAMP(0);
    STAT_CLAMP(1);
    STAT_CLAMP(2);
    STAT_CLAMP(3);
    STAT_CLAMP(4);
}

void SdMachineCreate(SdMachine *m)
{
    Mtx bodyMtx;
    Mtx partMtx;
    Vec wheel;
    Vec v;
    SdMachineTire *tireTbl;
    Vec *w;
    SdGun *gun;
    SdBullet *bullet;
    SdMachineCorner *corner;
    s32 tire;
    s32 engine;
    s32 mode;
    s32 i;
    s32 j;
    f32 f;

    SdMachineBodyCreate(m->bodyType, &m->mdlId);
    SdMachineTireCreate(m->kind, m->hookMdlId);
    SdMachineGunCreate(m->bulletType, &m->gunMdlId);
    SdMachineCharCreate(m->charNo, &m->mdl3AE);
    SdMachineHookSet(m->kind, m->mdlId, m->hookMdlId, m->gunMdlId, m->mdl3AE);
    m->target = &sys->machine[m->playerNo ^ 1];
    tire = m->kind;
    if (tire == 8) {
        if (rand8() % 100 < 30) {
            tire = 0;
        } else {
            tire = randomTireTbl[rand8() % 7];
        }
    }
    engine = m->engineType;
    if (engine == 8) {
        if (rand8() % 100 < 25) {
            engine = 0;
        } else {
            engine = randomEngineTbl[rand8() % 9];
        }
    }
    SdMachineParamSet(m, tire, m->bodyType, engine, m->bulletType);
    m->hp = m->stat[4] * 4;
    tireTbl = &sdMachineTireTbl[m->kind];
    m->wheelY0 = tireTbl->unk20;
    m->wheelY1 = tireTbl->unk24;
    m->attr = tireTbl->unk34;
    m->unk39C = sdMachineEngineTbl[m->engineType].unk10;
    if (m->kind == 8) {
        if (tire == 4) {
            m->attr = 0x10;
        }
        if (tire == 10) {
            m->attr = 0x20;
        }
    }
    if (m->engineType == 8 && engine == 10) {
        m->unk39C = 1;
    }
    mode = m->attr & 0xF;
    m->hitRadius = sdMachineBodyTbl[m->bodyType].radius;
    m->wheelBase = tireTbl->unk1C;
    if (mode == 2) {
        m->wheelBase -= machineYOfsTbl[m->bodyType];
    }
    m->hitHeight = sdMachineBodyTbl[m->bodyType].height;
    m->cornerNum = sdMachineBodyTbl[m->bodyType].cornerNum;
    m->gunNum = sdMachineGunTbl[m->bulletType].gunNum;
    m->gunBulletNum = sdMachineGunTbl[m->bulletType].bulletNum;
    m->gunTimerInit = sdMachineGunTbl[m->bulletType].timerInit;
    Hu3DModelCameraSet(m->mdlId, 3);
    Hu3DModelLayerSet(m->mdlId, 1);
    Hu3DModelAttrSet(m->mdlId, 0x40000041);
    if (m->bodyType == 6 || m->bodyType == 4 || m->bodyType == 1 || m->bodyType == 7 || m->bodyType == 0) {
        Hu3DModelAttrReset(m->mdlId, 0x40000082);
        Hu3DMotionSpeedSet(m->mdlId, 0.0f);
        Hu3DMotionShapeSpeedSet(m->mdlId, 0.0f);
    }
    v.x = 10000.0f;
    v.y = 10000.0f;
    v.z = 10000.0f;
    w = m->wheelWorld2;
    w[3] = v;
    w[2] = w[3];
    w[1] = w[2];
    w[0] = w[1];
    m->corner = HuMemDirectMallocNum(HEAP_HEAP, m->cornerNum * sizeof(SdMachineCorner), HU_MEMNUM_OVL);
    memset(m->corner, 0, m->cornerNum * sizeof(SdMachineCorner));
    m->track = HuMemDirectMallocNum(HEAP_HEAP, sizeof(SdMachineTrack), HU_MEMNUM_OVL);
    memset(m->track, 0, sizeof(SdMachineTrack));
    for (i = 0; i < m->cornerNum; i++) {
        corner = &m->corner[i];
        Hu3DModelObjPosGet(m->mdlId, sdMachineHookTbl[i + 8], &corner->pos);
        PSVECSubtract(&corner->pos, &m->curPos, &corner->pos);
    }
    for (i = 0; i < 4; i++) {
        m->tireScale[i] = 0.0f;
        if (mode == 0) {
            m->tireObj[i] = Hu3DModelObjPtrGet(m->hookMdlId[i], sdMachineTireHookTbl[i % 2]);
            Hu3DModelObjMtxGet(m->mdlId, sdMachineHookTbl[i], bodyMtx);
            Hu3DModelObjMtxGet(m->hookMdlId[i], sdMachineTireHookTbl[i % 2], partMtx);
            if (m->kind == 6) {
                Hu3DModelAttrSet(m->hookMdlId[i], 0x40000040);
            }
        } else if (mode == 1) {
            if (i >= 2) {
                continue;
            }
            Hu3DModelAttrSet(m->hookMdlId[i], 0x40000041);
            Hu3DMotionShapeSpeedSet(m->hookMdlId[i], 0.0f);
            Hu3DMotionSpeedSet(m->hookMdlId[i], 0.0f);
            Hu3DModelObjMtxGet(m->mdlId, sdMachineHookTbl[i + 4], bodyMtx);
            if (m->kind != 5) {
                Hu3DModelObjMtxGet(m->hookMdlId[i], sdMachineTireHookTbl[i % 2], partMtx);
            } else {
                PSMTXIdentity(partMtx);
            }
        } else {
            if (i >= 1) {
                continue;
            }
            Hu3DModelAttrSet(m->hookMdlId[i], 0x40000040);
            Hu3DModelObjMtxGet(m->mdlId, sdMachineHookTbl[7], bodyMtx);
            Hu3DModelObjMtxGet(m->hookMdlId[i], sdMachineTireHookTbl[2], partMtx);
        }
        Hu3DModelCameraSet(m->hookMdlId[i], 3);
        Hu3DModelLayerSet(m->hookMdlId[i], 1);
        if (m->kind == 9) {
            Hu3DModelAttrSet(m->hookMdlId[i], 0x40000001);
        }
        PSMTXConcat(bodyMtx, partMtx, bodyMtx);
        wheel.x = 0.0f;
        wheel.y = 0.0f;
        wheel.z = 0.0f;
        wheel.x = bodyMtx[0][3];
        wheel.y = bodyMtx[1][3];
        wheel.z = bodyMtx[2][3];
        w[i] = wheel;
        if (w[i].x < 20.0f && w[i].x > -20.0f) {
            if (i % 2 != 0) {
                w[i].x = -20.0f;
            } else {
                w[i].x = 20.0f;
            }
        }
    }
    switch (mode) {
    case 0:
        w[1].x -= tireTbl->unk28;
        w[0].x = -w[1].x;
        w[2].x = w[0].x;
        w[3].x = w[1].x;
        w[0].y = w[1].y;
        w[2].y = w[3].y;
        break;
    case 1:
        w[0].y = w[1].y;
        w[2].x = w[0].x;
        w[2].y = w[0].y;
        w[2].z = w[0].z;
        w[3].x = w[1].x;
        w[3].y = w[1].y;
        w[3].z = w[1].z;
        w[0].x += tireTbl->unk28;
        w[0].z = tireTbl->unk30;
        w[1].x -= tireTbl->unk28;
        w[1].z = tireTbl->unk30;
        w[2].x += tireTbl->unk28;
        w[2].z = -tireTbl->unk30;
        w[3].x -= tireTbl->unk28;
        w[3].z = -tireTbl->unk30;
        break;
    case 2:
        w[3] = w[0];
        w[2] = w[3];
        w[1] = w[2];
        w[0].x = tireTbl->unk28;
        w[0].z = tireTbl->unk30;
        w[1].x = -tireTbl->unk28;
        w[1].z = tireTbl->unk30;
        w[2].x = tireTbl->unk28;
        w[2].z = -tireTbl->unk30;
        w[3].x = -tireTbl->unk28;
        w[3].z = -tireTbl->unk30;
        break;
    }
    v.x = w[0].x;
    v.y = w[0].y;
    v.z = w[0].z;
    v.y = 0.0f;
    f = PSVECMag(&v);
    if (f > m->hitRadius) {
        m->hitRadius = f;
    }
    Hu3DModelObjMtxGet(m->mdlId, sdMachineHookTbl[6], bodyMtx);
    Hu3DModelAttrSet(m->gunMdlId, 0x40000082);
    Hu3DModelLayerSet(m->gunMdlId, 1);
    Hu3DModelCameraSet(m->gunMdlId, 3);
    if (m->bulletType == 10 || m->bulletType == 4) {
        Hu3DModelAttrReset(m->gunMdlId, 0x40000002);
        Hu3DModelAttrSet(m->gunMdlId, 0x40000001);
    }
    m->gunMdlId2 = -1;
    Hu3DModelCameraSet(m->mdl3AE, 3);
    Hu3DModelLayerSet(m->mdl3AE, 1);
    m->gun = HuMemDirectMallocNum(HEAP_HEAP, m->gunNum * sizeof(SdGun), HU_MEMNUM_OVL);
    memset(m->gun, 0, m->gunNum * sizeof(SdGun));
    for (i = 0; i < m->gunNum; i++) {
        gun = &m->gun[i];
        if (m->bulletType != 11) {
            Hu3DModelObjMtxGet(m->gunMdlId, sdMachineHookTbl[i + 13], partMtx);
        } else {
            PSMTXIdentity(partMtx);
        }
        PSMTXConcat(bodyMtx, partMtx, gun->mtx);
        gun->bullet = HuMemDirectMallocNum(HEAP_HEAP, m->gunBulletNum * sizeof(SdBullet), HU_MEMNUM_OVL);
        memset(gun->bullet, 0, m->gunBulletNum * sizeof(SdBullet));
        for (j = 0; j < m->gunBulletNum; j++) {
            bullet = &gun->bullet[j];
            bullet->mdlId3 = -1;
            bullet->mdlId2 = -1;
            bullet->mdlId = -1;
            bullet->mdlId5 = -1;
            bullet->mdlId4 = -1;
            bullet->machine = m;
            bullet->gun = gun;
            bullet->param = &sdBulletParam[m->bulletType];
            if (m->bulletType == 6 || m->bulletType == 11) {
                continue;
            }
            if (sdMachineGunTbl[m->bulletType].bulletData != -1) {
                if (m->bulletType == 4 || m->bulletType == 1) {
                    if (i == 0 && j == 0) {
                        bullet->mdlId = Hu3DModelCreateData(sdMachineGunTbl[m->bulletType].bulletData);
                    }
                } else {
                    bullet->mdlId = Hu3DModelCreateData(sdMachineGunTbl[m->bulletType].bulletData);
                }
                if (bullet->mdlId != -1) {
                    Hu3DModelCameraSet(bullet->mdlId, 3);
                    Hu3DModelLayerSet(bullet->mdlId, 3);
                    Hu3DModelAttrSet(bullet->mdlId, 1);
                }
                if (m->bulletType == 2 || m->bulletType == 12) {
                    Hu3DModelAttrSet(bullet->mdlId, 0x40000001);
                    bullet->mdlId3 = Hu3DModelCreateData(SDDATA(0x6C));
                    Hu3DModelHookSet(bullet->mdlId, hookNameC, bullet->mdlId3);
                    Hu3DModelAttrSet(bullet->mdlId3, 1);
                    Hu3DModelAttrSet(bullet->mdlId3, 0x40000001);
                    Hu3DModelLayerSet(bullet->mdlId3, 3);
                    Hu3DModelCameraSet(bullet->mdlId3, 3);
                    if (m->bulletType == 2) {
                        Hu3DModelScaleSet(bullet->mdlId3, 1.5f, 1.5f, 1.5f);
                        bullet->mdlId4 = Hu3DJointMotionData(bullet->mdlId, SDDATA(0x56));
                        bullet->mdlId5 = Hu3DJointMotionData(bullet->mdlId, SDDATA(0x57));
                    } else {
                        Hu3DModelScaleSet(bullet->mdlId3, 1.0f, 1.0f, 1.0f);
                        bullet->mdlId4 = Hu3DJointMotionData(bullet->mdlId, SDDATA(0x58));
                        bullet->mdlId5 = Hu3DJointMotionData(bullet->mdlId, SDDATA(0x59));
                    }
                    Hu3DMotionSet(bullet->mdlId, bullet->mdlId4);
                }
            }
            if (m->bulletType != 7) {
                bullet->mdlId2 = Hu3DModelLink(sys->baseShadowMdlId);
                Hu3DModelShadowSet(bullet->mdlId2);
                Hu3DModelCameraSet(bullet->mdlId2, 3);
                Hu3DModelScaleSet(bullet->mdlId2, 0.4f, 0.4f, 0.4f);
                Hu3DModelLayerSet(bullet->mdlId2, 3);
                Hu3DModelAttrSet(bullet->mdlId2, 1);
                Hu3DModelAttrSet(bullet->mdlId2, 8);
            }
        }
    }
    f = (2.0f * m->hitRadius) / 100.0f;
    m->mdl3B4 = Hu3DModelLink(sys->baseShadowMdlId);
    Hu3DModelShadowSet(m->mdl3B4);
    Hu3DModelScaleSet(m->mdl3B4, f, 1.0f, f);
    Hu3DModelLayerSet(m->mdl3B4, 3);
    Hu3DModelCameraSet(m->mdl3B4, 3);
    Hu3DModelAttrReset(m->mdl3B4, 1);
    Hu3DModelAttrSet(m->mdl3B4, 8);
    if (sys->bulletHookMdlId == -1) {
        if (m->bulletType == 4 || m->target->bulletType == 4 || m->bulletType == 1 || m->target->bulletType == 1) {
            sys->bulletHookMdlId = Hu3DHookFuncCreate(SdBulletDraw);
            Hu3DModelCameraSet(sys->bulletHookMdlId, 3);
            Hu3DModelLayerSet(sys->bulletHookMdlId, 3);
        }
    } else if (m->bulletType != 4 && m->target->bulletType != 4 && m->bulletType != 1
               && m->target->bulletType != 1) {
        Hu3DModelKill(sys->bulletHookMdlId);
        sys->bulletHookMdlId = -1;
    }
    Hu3DData[m->mdlId].pos.y = m->wheelBase;
    SdMachineFxStart(m);
}

void SdMachineKill(SdMachine *m)
{
    s32 i;

    SdMachineFxStop(m);
    SdMachineParticleKill();
    SdSprKill();
    if (m->corner != NULL) {
        HuMemDirectFree(m->corner);
    }
    if (m->track != NULL) {
        HuMemDirectFree(m->track);
    }
    m->corner = NULL;
    m->track = NULL;
    Hu3DModelKill(m->mdl3B4);
    Hu3DModelHookReset(m->mdlId);
    Hu3DModelKill(m->mdlId);
    for (i = 0; i < 4; i++) {
        if (m->hookMdlId[i] != -1) {
            Hu3DModelKill(m->hookMdlId[i]);
        }
    }
    if (m->gunMdlId2 != -1) {
        Hu3DModelKill(m->gunMdlId2);
    }
    Hu3DModelKill(m->gunMdlId);
    Hu3DModelKill(m->mdl3AE);
    SdMachineDestroy(m);
}

void SdMachineDestroy(SdMachine *m)
{
    SdBullet *bullet;
    s32 i;
    s32 j;

    for (i = 0; i < m->gunNum; i++) {
        for (j = 0; j < m->gunBulletNum; j++) {
            bullet = &m->gun[i].bullet[j];
            if (bullet->mdlId4 != -1) {
                Hu3DMotionSet(bullet->mdlId, -1);
                Hu3DMotionKill(bullet->mdlId4);
                Hu3DMotionKill(bullet->mdlId5);
                bullet->mdlId5 = -1;
                bullet->mdlId4 = -1;
            }
            if (bullet->mdlId3 != -1) {
                Hu3DModelHookReset(bullet->mdlId);
                Hu3DModelKill(bullet->mdlId3);
            }
            if (bullet->mdlId != -1) {
                Hu3DModelKill(bullet->mdlId);
            }
            if (bullet->mdlId2 != -1) {
                Hu3DModelKill(bullet->mdlId2);
            }
        }
        HuMemDirectFree(m->gun[i].bullet);
    }
    HuMemDirectFree(m->gun);
}

void SdSprCreate(void)
{
    SdSprWork *spr = &sys->spr;
    SdMachine *m;
    s16 *bullet;
    Vec in;
    Vec out;
    Vec dir;
    Vec rot;
    s32 playerNo;
    s32 i;
    s32 j;
    s32 k;

    spr->hideF = 0;
    sys->spr.timer = 0;
    for (k = 0; k < 2; k++) {
        m = &sys->machine[k];
        playerNo = m->playerNo;
        spr->playerSpr[playerNo] = espEntry(SDDATA(0x95), 0, 0);
        espDrawNoSet(spr->playerSpr[playerNo], 0);
        espBankSet(spr->playerSpr[playerNo], playerNo * 2);
        espScaleSet(spr->playerSpr[playerNo], 0.5f, 0.5f);
        espPriSet(spr->playerSpr[playerNo], 0x1D0);
        in = m->curPos;
        ScoreSprPosGet(&out, &in);
        espPosSet(spr->playerSpr[playerNo], out.x, out.y);
        dir.x = 0.0f;
        dir.y = 0.0f;
        dir.z = 0.0f;
        dir.x = m->mtx[0][2];
        dir.y = m->mtx[1][2];
        dir.z = m->mtx[2][2];
        rot = dir;
        espZRotSet(spr->playerSpr[playerNo], (f32) -(180.0 * (atan2((f64) rot.x, (f64) rot.z) / 3.141592653589793)));
        if (sys->gameType == 0) {
            for (i = 0; i < 8; i++) {
                spr->hpIcon[playerNo][i] = espEntry(SDDATA(0x96), 0, 0);
                espDrawNoSet(spr->hpIcon[playerNo][i], 0);
                if (m->hp - i * 4 > 0) {
                    espBankSet(spr->hpIcon[playerNo][i], 0);
                } else {
                    espBankSet(spr->hpIcon[playerNo][i], 3);
                    espDispOff(spr->hpIcon[playerNo][i]);
                }
                espPriSet(spr->hpIcon[playerNo][i], 0x1E0);
                espScaleSet(spr->hpIcon[playerNo][i], 0.6f, 0.6f);
                espPosSet(spr->hpIcon[playerNo][i], 32.0f + (316.0f * (f32) playerNo) + (28.0f * (f32) i), 56.0f);
            }
        }
        spr->bulletSpr[playerNo] = HuMemDirectMallocNum(HEAP_HEAP, m->gunBulletNum * (m->gunNum * sizeof(s16)), HU_MEMNUM_OVL);
        for (i = 0; i < m->gunNum; i++) {
            for (j = 0; j < m->gunBulletNum; j++) {
                bullet = &spr->bulletSpr[playerNo][j + i * m->gunBulletNum];
                *bullet = espEntry(SDDATA(0x95), 0, 0);
                espDrawNoSet(*bullet, 0);
                espBankSet(*bullet, playerNo * 2 + 1);
                espDispOff(*bullet);
                espPriSet(*bullet, 0x1D0);
                espScaleSet(*bullet, 0.25f, 0.25f);
            }
        }
    }
    spr->centerSpr = espEntry(SDDATA(0x9C), 0, 0);
    espDrawNoSet(spr->centerSpr, 0);
    espPriSet(spr->centerSpr, 0x1F0);
    espPosSet(spr->centerSpr, 288.0f, 240.0f);
    if (sys->gameType == 1) {
        spr->flagSpr = espEntry(SDDATA(0x9A), 0, 0);
        espDrawNoSet(spr->flagSpr, 0);
        espScaleSet(spr->flagSpr, 0.5f, 0.5f);
        espPriSet(spr->flagSpr, 0x1D0);
        SdFlagPosGet(&in);
        ScoreSprPosGet(&out, &in);
        espPosSet(spr->flagSpr, out.x, out.y);
        espDispOff(spr->flagSpr);
    }
    if (sys->gameType == 2) {
        spr->rabbitSpr = espEntry(SDDATA(0x9B), 0, 0);
        espDrawNoSet(spr->rabbitSpr, 0);
        espScaleSet(spr->rabbitSpr, 0.5f, 0.5f);
        espPriSet(spr->rabbitSpr, 0x1D0);
        SdRabbitPosGet(&in);
        ScoreSprPosGet(&out, &in);
        espPosSet(spr->rabbitSpr, out.x, out.y);
        espDispOff(spr->rabbitSpr);
    }
    MapSprCreate();
}

static void MapSprCreate(void)
{
    SdSprWork *spr = &sys->spr;

    if (sys->stageNo == 0) {
        spr->mapSpr = espEntry(SDDATA(0x98), 0, 0);
    } else if (sys->stageNo == 7) {
        spr->mapSpr = espEntry(SDDATA(0x99), 0, 0);
    } else {
        spr->mapSpr = espEntry(SDDATA(0x97), 0, 0);
    }
    espDrawNoSet(spr->mapSpr, 0);
    espPriSet(spr->mapSpr, 0x1E0);
    espPosSet(spr->mapSpr, 288.0f, 360.0f);
    espScaleSet(spr->mapSpr, 0.5f, 0.5f);
}

static void MapSprKill(void)
{
    espKill(sys->spr.mapSpr);
}

void SdHpSprCreate(void)
{
    SdSprWork *spr = &sys->spr;
    SdMachine *m;
    Vec in;
    Vec out;
    Vec dir;
    Vec rot;
    s32 playerNo;
    s32 hp;
    s32 i;
    s32 j;
    s32 k;

    espDispOff(spr->mapSpr);
    for (k = 0; k < 2; k++) {
        m = &sys->machine[k];
        playerNo = m->playerNo;
        espDispOff(spr->playerSpr[playerNo]);
        for (j = 0; j < 3; j++) {
            spr->zoomSpr[playerNo][j] = espEntry(SDDATA(0x95), 0, 0);
            espDrawNoSet(spr->zoomSpr[playerNo][j], 0);
            espBankSet(spr->zoomSpr[playerNo][j], playerNo * 2);
            espScaleSet(spr->zoomSpr[playerNo][j], 2.5f, 2.5f);
            espPriSet(spr->zoomSpr[playerNo][j], 0x1D0);
            in = m->curPos;
            ScoreSprPosGet(&out, &in);
            espPosSet(spr->zoomSpr[playerNo][j], out.x, out.y);
            dir.x = 0.0f;
            dir.y = 0.0f;
            dir.z = 0.0f;
            dir.x = m->mtx[0][2];
            dir.y = m->mtx[1][2];
            dir.z = m->mtx[2][2];
            rot = dir;
            espZRotSet(spr->zoomSpr[playerNo][j], (f32) -(180.0 * (atan2((f64) rot.x, (f64) rot.z) / 3.141592653589793)));
            espDispOff(spr->zoomSpr[playerNo][j]);
            espTPLvlSet(spr->zoomSpr[playerNo][j], 0.1f);
            HuSprAttrSet(espGrpIDGet(), esprite[spr->zoomSpr[playerNo][j]].memberNo, 0x80);
        }
        if (sys->gameType == 0) {
            for (i = 0; i < 8; i++) {
                espDispOff(spr->hpIcon[playerNo][i]);
                hp = m->hp - i * 4;
                for (j = 0; j < 3; j++) {
                    spr->zoomHpIcon[playerNo][i][j] = espEntry(SDDATA(0x96), 0, 0);
                    espDrawNoSet(spr->zoomHpIcon[playerNo][i][j], 0);
                    if (hp > 0) {
                        espBankSet(spr->zoomHpIcon[playerNo][i][j], 0);
                    } else {
                        espBankSet(spr->zoomHpIcon[playerNo][i][j], 3);
                        espDispOff(spr->zoomHpIcon[playerNo][i][j]);
                    }
                    espPriSet(spr->zoomHpIcon[playerNo][i][j], 0x1E0);
                    espScaleSet(spr->zoomHpIcon[playerNo][i][j], 2.5f, 2.5f);
                    espPosSet(spr->zoomHpIcon[playerNo][i][j],
                              32.0f + (316.0f * (f32) playerNo) + (28.0f * (f32) i), 56.0f);
                    espDispOff(spr->zoomHpIcon[playerNo][i][j]);
                    HuSprAttrSet(espGrpIDGet(), esprite[spr->zoomHpIcon[playerNo][i][j]].memberNo, 0x80);
                }
            }
        }
    }
}

static void ScoreSprPosGet(Vec *out, Vec *in)
{
    Vec res;
    Vec radius;
    s32 type;

    res.z = 0.0f;
    if (sys->stageNo == 0) {
        res.x = (f32) (288.0 + ((f64) (64.0f * in->x) / 2400.0));
        res.y = (f32) (360.0 + ((f64) (64.0f * in->z) / 2400.0));
    } else if (sys->stageNo == 7) {
        res.x = 288.0f + ((64.0f * in->x) / 3540.0f);
        res.y = 360.0f + ((72.0f * in->z) / 3540.0f);
    } else {
        SdStageSprPosGet(&type, &radius);
        res.x = 288.0f + ((64.0f * in->x) / radius.x);
        res.y = 360.0f + ((64.0f * in->z) / radius.x);
    }
    *out = res;
}

void SdSprKill(void)
{
    SdSprWork *spr = &sys->spr;
    SdMachine *m;
    s32 playerNo;
    s32 i;
    s32 j;
    s32 k;

    if (sys->gameType == 1) {
        espKill(spr->flagSpr);
    }
    if (sys->gameType == 2) {
        espKill(spr->rabbitSpr);
    }
    for (k = 0; k < 2; k++) {
        m = &sys->machine[k];
        playerNo = m->playerNo;
        espKill(spr->playerSpr[playerNo]);
        if (sys->gameType == 0) {
            for (i = 0; i < 8; i++) {
                espKill(spr->hpIcon[playerNo][i]);
            }
        }
        for (i = 0; i < m->gunNum; i++) {
            for (j = 0; j < m->gunBulletNum; j++) {
                espKill(spr->bulletSpr[playerNo][j + i * m->gunBulletNum]);
            }
        }
        HuMemDirectFree(spr->bulletSpr[playerNo]);
    }
    MapSprKill();
    espKill(spr->centerSpr);
}

void SdHpSprKill(void)
{
    SdSprWork *spr = &sys->spr;
    s32 playerNo;
    s32 i;
    s32 j;
    s32 k;

    for (k = 0; k < 2; k++) {
        playerNo = sys->machine[k].playerNo;
        for (j = 0; j < 3; j++) {
            espKill(spr->zoomSpr[playerNo][j]);
            spr->zoomSpr[playerNo][j] = -1;
        }
        if (sys->gameType == 0) {
            for (i = 0; i < 8; i++) {
                for (j = 0; j < 3; j++) {
                    espKill(spr->zoomHpIcon[playerNo][i][j]);
                    spr->zoomHpIcon[playerNo][i][j] = -1;
                }
            }
        }
    }
}

void SdSprScissorUpdate(s16 sprId, s32 cameraNo)
{
    HuSprScissorSet(espGrpIDGet(), esprite[sprId].memberNo, Hu3DCamera[cameraNo].scissorX,
                    Hu3DCamera[cameraNo].scissorY, Hu3DCamera[cameraNo].scissorW, Hu3DCamera[cameraNo].scissorH);
}

void SdSprUpdate(void)
{
    SdSprWork *spr = &sys->spr;
    SdMachine *m;
    SdBullet *bullet;
    s16 *bulletSpr;
    Vec in;
    Vec out;
    Vec dir;
    Vec rot;
    s32 playerNo;
    s32 hp;
    s32 i;
    s32 j;
    s32 k;

    if (MgSeqModeGet() >= 5 && spr->hideF == 0) {
        spr->timer++;
        spr->timer = spr->timer % 600;
        for (k = 0; k < 2; k++) {
            m = &sys->machine[k];
            playerNo = m->playerNo;
            in = m->curPos;
            ScoreSprPosGet(&out, &in);
            espPosSet(spr->playerSpr[playerNo], out.x, out.y);
            dir.x = 0.0f;
            dir.y = 0.0f;
            dir.z = 0.0f;
            dir.x = m->mtx[0][2];
            dir.y = m->mtx[1][2];
            dir.z = m->mtx[2][2];
            rot = dir;
            espZRotSet(spr->playerSpr[playerNo], (f32) -(180.0 * (atan2((f64) rot.x, (f64) rot.z) / 3.141592653589793)));
            if (sys->gameType == 0) {
                for (i = 0; i < 8; i++) {
                    hp = m->hp - i * 4;
                    if (hp > 0) {
                        espDispOn(spr->hpIcon[playerNo][i]);
                        if (hp >= 4) {
                            espBankSet(spr->hpIcon[playerNo][i], 0);
                        } else {
                            espBankSet(spr->hpIcon[playerNo][i], 4 - (hp % 4));
                        }
                    } else {
                        espDispOff(spr->hpIcon[playerNo][i]);
                    }
                }
            }
            for (i = 0; i < m->gunNum; i++) {
                for (j = 0; j < m->gunBulletNum; j++) {
                    bullet = &m->gun[i].bullet[j];
                    bulletSpr = &spr->bulletSpr[playerNo][j + i * m->gunBulletNum];
                    if (bullet->mode == 0) {
                        espDispOff(*bulletSpr);
                    } else if (m->bulletType == 6 || m->bulletType == 11) {
                        espDispOff(*bulletSpr);
                    } else {
                        in = bullet->pos;
                        espDispOn(*bulletSpr);
                        ScoreSprPosGet(&out, &in);
                        espPosSet(*bulletSpr, out.x, out.y);
                    }
                }
            }
        }
        if (sys->gameType == 1) {
            if (SdFlagCheck() != 0) {
                SdFlagPosGet(&in);
                ScoreSprPosGet(&out, &in);
                espPosSet(spr->flagSpr, out.x, out.y);
                espDispOn(spr->flagSpr);
            } else {
                espDispOff(spr->flagSpr);
            }
        }
        if (sys->gameType == 2) {
            if (SdRabbitCheck() != 0) {
                SdRabbitPosGet(&in);
                ScoreSprPosGet(&out, &in);
                espPosSet(spr->rabbitSpr, out.x, out.y);
                espDispOn(spr->rabbitSpr);
                return;
            }
            espDispOff(spr->rabbitSpr);
        }
    }
}

void SdSprHideAll(void)
{
    SdSprWork *spr = &sys->spr;
    s32 i;
    s32 j;

    spr->hideF = 1;
    for (i = 0; i < 2; i++) {
        espDispOff(spr->playerSpr[i]);
    }
    espDispOff(spr->mapSpr);
    if (sys->gameType == 0) {
        for (i = 0; i < 2; i++) {
            for (j = 0; j < 8; j++) {
                if (spr->hpIcon[i][j] != -1) {
                    espDispOff(spr->hpIcon[i][j]);
                }
            }
        }
        return;
    }
    if (sys->gameType == 1) {
        SdFlagSprDispOff();
        return;
    }
    SdRabbitSprDispOff();
}

void SdMachineConfigSet(s32 playerNo)
{
    GWSDMACHINE *config = &GwSdBattle.machine[playerNo];
    SdMachine *m = &sys->machine[playerNo];
    s32 preset;

    if (GwPlayerConf[playerNo].type != 0 && (s8) config->charNo >= 0x20) {
        preset = (s8) config->charNo - 0x20;
        m->bodyType = configPartTbl[preset + 1][0];
        m->kind = configPartTbl[preset + 1][1];
        m->engineType = configPartTbl[preset + 1][2];
        m->bulletType = configPartTbl[preset + 1][3];
        m->charNo = configPartTbl[preset + 1][4];
        m->comPreset = preset;
        return;
    }
    if (GwPlayerConf[playerNo].type != 0) {
        m->comPreset = -1;
    }
    m->bodyType = config->partNo[0];
    m->kind = config->partNo[1];
    m->engineType = config->partNo[2];
    m->bulletType = config->partNo[3];
    m->charNo = (s8) config->charNo;
}

void SdMachineEffectCreate(void)
{
    sys->baseShadowMdlId = Hu3DModelCreateData(SDDATA(0x5A));
    Hu3DModelAttrSet(sys->baseShadowMdlId, 1);
    sys->kinokoMdlId = Hu3DModelCreateData(SDDATA(0x6D));
    Hu3DModelAttrSet(sys->kinokoMdlId, 1);
    Hu3DModelLayerSet(sys->kinokoMdlId, 3);
    Hu3DModelAttrSet(sys->kinokoMdlId, 0x40000001);
    Hu3DMotionAttrSet(Hu3DMotionIDGet(sys->kinokoMdlId), 1);
    sys->kinokoMdlId2 = Hu3DModelCreateData(SDDATA(0x6E));
    Hu3DModelAttrSet(sys->kinokoMdlId2, 1);
    Hu3DModelLayerSet(sys->kinokoMdlId2, 3);
    sys->togezoMdlId = Hu3DModelCreateData(SDDATA(0x6F));
    Hu3DModelAttrSet(sys->togezoMdlId, 1);
    Hu3DModelLayerSet(sys->togezoMdlId, 3);
    Hu3DModelAttrSet(sys->togezoMdlId, 0x40000001);
    Hu3DMotionAttrSet(Hu3DMotionIDGet(sys->togezoMdlId), 1);
    sys->togezoMdlId2 = Hu3DModelCreateData(SDDATA(0x70));
    Hu3DModelAttrSet(sys->togezoMdlId2, 1);
    Hu3DModelLayerSet(sys->togezoMdlId2, 3);
}

void SdMachineParticleCreate(void)
{
    s32 blendTbl[12] = { 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1 };
    SdMachine *m;
    SdMachineCorner *corner;
    SdBullet *bullet;
    SdStageObj *obj;
    HU3DPARTICLE *particle;
    HU3DPARTICLEDATA *data;
    s32 i;
    s32 j;
    s32 k;
    s32 n;

    for (i = 0; i < 12; i++) {
        sys->partAnim[i] = HuSprAnimRead(HuDataSelHeapReadNum(effectFileTbl[i], HU_MEMNUM_OVL, HEAP_MODEL));
        sys->partMdlId[i] = -1;
        sys->partCnt[i] = 0;
    }
    for (i = 0; i < 2; i++) {
        m = &sys->machine[i];
        memset(m->track, 0, sizeof(SdMachineTrack));
        for (j = 0; j < 2; j++) {
            m->track->effIdx[j][0] = sys->partCnt[3];
            sys->partCnt[3] += 0x1E;
            m->track->effIdx[j][1] = sys->partCnt[2];
            sys->partCnt[2] += 0x1E;
            m->track->effIdx[j][2] = sys->partCnt[9];
            sys->partCnt[9] += 0x1E;
            m->track->effIdx[j][3] = sys->partCnt[11];
            sys->partCnt[11] += 0x1E;
        }
        for (j = 0; j < m->gunNum; j++) {
            for (k = 0; k < m->gunBulletNum; k++) {
                bullet = &m->gun[j].bullet[k];
                for (n = 0; n < 12; n++) {
                    bullet->partIdx[n] = sys->partCnt[n];
                    sys->partCnt[n] += bullet->param->partNum[n];
                }
            }
        }
        for (j = 0; j < m->cornerNum; j++) {
            corner = &m->corner[j];
            if (m->bodyType != 1 && m->bodyType != 9 && m->bodyType != 10) {
                corner->effIdx = sys->partCnt[3];
                sys->partCnt[3] += 0x1E;
            } else {
                corner->effIdx = sys->partCnt[10];
                sys->partCnt[10] += 0x1E;
            }
        }
    }
    if (sys->obj != NULL) {
        for (i = 0; i < 8; i++) {
            obj = &sys->obj[i];
            if (obj->mdlId != -1 && (obj->type == 1 || obj->type == 2)) {
                obj->fireIdx = sys->partCnt[3];
                sys->partCnt[3] += 0x30;
                obj->flameIdx = sys->partCnt[4];
                sys->partCnt[4] += 0x19;
            }
        }
    }
    if (sys->stageNo == 7) {
        sys->smokeSlot = sys->partCnt[0];
        sys->partCnt[0] += 0x1FE;
    }
    for (i = 0; i < 12; i++) {
        if (sys->partCnt[i] == 0) {
            sys->partCnt[i] = 1;
        }
        sys->partMdlId[i] = Hu3DParticleCreate(sys->partAnim[i], (s16) sys->partCnt[i]);
        particle = Hu3DData[sys->partMdlId[i]].hookData;
        data = particle->data;
        Hu3DParticleColSet(sys->partMdlId[i], 0xFF, 0xFF, 0xFF);
        Hu3DParticleBlendModeSet(sys->partMdlId[i], (u8) blendTbl[i]);
        Hu3DParticleHookSet(sys->partMdlId[i], SdMachinePartcleHook);
        Hu3DModelLayerSet(sys->partMdlId[i], 3);
        Hu3DModelPosSet(sys->partMdlId[i], 0.0f, 0.0f, 0.0f);
        Hu3DParticleAnimModeSet(sys->partMdlId[i], 0);
        for (j = 0; j < particle->maxCnt; j++) {
            data->time = 0;
            data->scale = 0.0f;
            data->cameraBit = 3;
            data++;
        }
    }
}

void SdMachineParticleKill(void)
{
    s32 i;

    for (i = 0; i < 12; i++) {
        if (sys->partMdlId[i] != -1) {
            Hu3DModelKill(sys->partMdlId[i]);
        }
        sys->partAnim[i] = NULL;
        sys->partMdlId[i] = -1;
        sys->partCnt[i] = 0;
    }
}

static void SdMachinePartcleHook(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx)
{
    DCStoreRangeNoSync(particleP->data, particleP->maxCnt * sizeof(HU3DPARTICLEDATA));
}

void SdMachineFxStart(SdMachine *m)
{
    m->hitSeId = -1;
    m->gunSeId = -1;
    m->skidSeId = -1;
    m->engineSeId = -1;
    m->engineSeId = HuAudFXPlayVolPan(m->playerNo + engineSeIdTbl[m->engineType], 0x7F, (s16) ((m->playerNo << 5) + 0x30));
    HuAudFXPitchSet(m->engineSeId, 0);
    if (skidSeIdTbl[m->kind] != 0) {
        m->skidSeId = HuAudFXPlayVolPan(m->playerNo + skidSeIdTbl[m->kind], 0, (s16) ((m->playerNo << 5) + 0x30));
    }
    if (m->bulletType == 6) {
        m->gunSeId = HuAudFXPlayVolPan(m->playerNo + gunSeIdTbl[m->bulletType], 0, (s16) ((m->playerNo << 5) + 0x30));
    }
    m->hitSeId = HuAudFXPlayVolPan(m->playerNo + 0x55F, 0, (s16) ((m->playerNo << 5) + 0x30));
}

void SdMachineFxStop(SdMachine *m)
{
    if (m->engineSeId != -1) {
        HuAudFXStop(m->engineSeId);
    }
    if (m->skidSeId != -1) {
        HuAudFXStop(m->skidSeId);
    }
    if (m->gunSeId != -1) {
        HuAudFXStop(m->gunSeId);
    }
    if (m->hitSeId != -1) {
        HuAudFXStop(m->hitSeId);
    }
}

void SdMachineFxUpdate(SdMachine *m)
{
    Vec dir;
    Vec zAxis;
    f32 mag;
    f32 dot;
    f32 f;

    if (skidSeIdTbl[m->kind] != 0) {
        if (m->hitF != 0) {
            HuAudFXVolSet(m->skidSeId, 0);
            return;
        }
        if (m->boostType != 1 && m->boostType != 2) {
            HuAudFXVolSet(m->skidSeId, 0);
            return;
        }
        mag = PSVECMag(&m->rotVel);
        if (mag < 1.0f) {
            HuAudFXVolSet(m->skidSeId, 0);
            return;
        }
        PSVECNormalize(&m->rotVel, &dir);
        zAxis.x = 0.0f;
        zAxis.y = 0.0f;
        zAxis.z = 0.0f;
        zAxis.x = m->mtx[0][2];
        zAxis.y = m->mtx[1][2];
        zAxis.z = m->mtx[2][2];
        dot = PSVECDotProduct(&dir, &zAxis);
        if (dot > 0.8f) {
            HuAudFXVolSet(m->skidSeId, 0);
            return;
        }
        if (HuAudFXStatusGet(m->skidSeId) == 0) {
            m->skidSeId = HuAudFXPlayPan(m->playerNo + skidSeIdTbl[m->kind], (s16) ((m->playerNo << 5) + 0x30));
        }
        if (dot < 0.0f) {
            dot = 0.0f;
        }
        if (mag > 10.0f) {
            mag = 10.0f;
        }
        f = 0.75f * (1.0f - dot);
        HuAudFXVolSet(m->skidSeId, (s32) (127.0f * (f + (0.25f * (mag / 10.0f)))));
    }
}

void SdMachineFxGunPlay(SdBullet *bullet)
{
    SdMachine *m = bullet->machine;

    if (m->bulletType != 6) {
        HuAudFXPlayVolPan(m->playerNo + gunSeIdTbl[m->bulletType], 0x7F, (s16) ((m->playerNo << 5) + 0x30));
    }
}

void SdMachineFxImpactPlay(SdBullet *bullet)
{
    SdMachine *m = bullet->machine;
    Vec pos;

    if (impactSeIdTbl[m->bulletType] != 0) {
        if (impactSeIdTbl[m->bulletType] == 0x555) {
            pos = bullet->pos;
            SdImpactStart(impactSeIdTbl[m->bulletType], &pos, 0, 1.0f, 100.0f);
            return;
        }
        pos = bullet->pos;
        SdImpactStart(impactSeIdTbl[m->bulletType], &pos, 1, 1.0f, bullet->param->unk18);
    }
}

void SdImpactStart(s32 snd, Vec *pos, s32 vibrate, f32 vol, f32 range)
{
    Vec localPos = *pos;

    SdImpactSet(snd, &localPos, vibrate, vol, range);
}

void SdImpactSoundStart(s32 snd, Vec *pos, f32 vol)
{
    Vec localPos = *pos;

    SdImpactSet(snd, &localPos, 0, vol, 0.0f);
}

static void SdImpactSet(s32 snd, Vec *pos, s32 vibrate, f32 vol, f32 range)
{
    Mtx camMtx[2];
    Vec scr;
    Vec a;
    Vec b;
    Vec t;
    SdImpact *impact;
    f32 dist;
    f32 pan;
    s32 i;
    s32 c;

    PSVECSubtract(&Hu3DCamera[0].target, &Hu3DCamera[0].pos, &scr);
    MathPlaneRotMtxGet(&scr, &Hu3DCamera[0].up, 0.0f, camMtx[0]);
    PSVECSubtract(&Hu3DCamera[1].target, &Hu3DCamera[1].pos, &scr);
    MathPlaneRotMtxGet(&scr, &Hu3DCamera[1].up, 0.0f, camMtx[1]);
    for (i = 0; i < 30; i++) {
        impact = &sys->impact[i];
        if (impact->active != 0) {
            continue;
        }
        impact->active = 1;
        impact->timer = 0;
        impact->vibrate = vibrate;
        for (c = 0; c < 2; c++) {
            impact->snd[c] = 0;
            SdScreenPosGet(pos, (s16) sdCameraBitTbl[c], &scr);
            scr.x = 2.0f * (scr.x - 144.0f);
            if (scr.x < 0.0f || scr.x > 576.0f || scr.y < 0.0f || scr.y > 480.0f || scr.z > 0.0f) {
                if (range == 0.0f) {
                    continue;
                }
                a.x = 0.0f;
                a.y = 0.0f;
                a.z = 0.0f;
                b = a;
                if (scr.x < 0.0f) {
                    t.x = 0.0f;
                    t.y = 0.0f;
                    t.z = 0.0f;
                    t.x = camMtx[c][0][0];
                    t.y = camMtx[c][1][0];
                    t.z = camMtx[c][2][0];
                    b = t;
                    PSVECScale(&b, &b, -1.0f);
                } else if (scr.x > 576.0f) {
                    t.x = 0.0f;
                    t.y = 0.0f;
                    t.z = 0.0f;
                    t.x = camMtx[c][0][0];
                    t.y = camMtx[c][1][0];
                    t.z = camMtx[c][2][0];
                    b = t;
                }
                if (scr.y > 480.0f || scr.z > 0.0f) {
                    t.x = 0.0f;
                    t.y = 0.0f;
                    t.z = 0.0f;
                    t.x = camMtx[c][0][1];
                    t.y = camMtx[c][1][1];
                    t.z = camMtx[c][2][1];
                    a = t;
                } else if (scr.y < 0.0f) {
                    t.x = 0.0f;
                    t.y = 0.0f;
                    t.z = 0.0f;
                    t.x = camMtx[c][0][1];
                    t.y = camMtx[c][1][1];
                    t.z = camMtx[c][2][1];
                    a = t;
                    PSVECScale(&a, &a, -1.0f);
                }
                PSVECAdd(&b, &a, &b);
                if (PSVECMag(&b) == 0.0f) {
                    b.x = 1.0f;
                    b.y = 1.0f;
                    b.z = 1.0f;
                }
                PSVECNormalize(&b, &b);
                PSVECScale(&b, &b, range);
                PSVECAdd(&b, pos, &b);
                SdScreenPosGet(&b, (s16) sdCameraBitTbl[c], &b);
                b.x = 2.0f * (b.x - 144.0f);
                if (b.x < 0.0f || b.x > 576.0f || b.y < 0.0f || b.y > 480.0f || b.z > 0.0f) {
                    continue;
                }
            }
            scr.x = 0.5f * (scr.x + (576.0f * (f32) c));
            if (scr.x > 576.0f) {
                pan = 576.0f;
            } else if (scr.x < 0.0f) {
                pan = 0.0f;
            } else {
                pan = scr.x;
            }
            scr.x = pan;
            impact->snd[c] = snd + c;
            impact->pan[c] = (s16) ((127.0f * scr.x) / 576.0f);
            PSVECSubtract(&Hu3DCamera[c].pos, pos, &scr);
            dist = PSVECMag(&scr);
            impact->delay[c] = (s16) (dist / 555.55554f);
            impact->vol[c] = (s16) (32.0f + (96.0f - ((63.0f * dist) / 8400.0f)));
            if (impact->vol[c] > 0x7F) {
                impact->vol[c] = 0x7F;
            } else if (impact->vol[c] < 0x20) {
                impact->vol[c] = 0x20;
            }
            impact->vol[c] = (s16) ((f32) impact->vol[c] * vol);
        }
        return;
    }
}

void SdImpactUpdate(void)
{
    SdImpact *impact;
    s32 i;
    s32 c;

    for (i = 0; i < 30; i++) {
        impact = &sys->impact[i];
        if (impact->active != 0) {
            if (impact->snd[0] == 0 && impact->snd[1] == 0) {
                impact->active = 0;
            } else {
                for (c = 0; c < 2; c++) {
                    if (impact->snd[c] != 0 && impact->timer >= impact->delay[c]) {
                        HuAudFXPlayVolPan(impact->snd[c], impact->vol[c], impact->pan[c]);
                        if (impact->vibrate != 0) {
                            sys->camera[c].shake = 5.0f * (1.0f - MathLerp(1, (f32) (0x7F - impact->vol[c]) / 100.0f));
                            SdVibrate(sys->machine[c].playerNo, 0, (s32) (2.0f * sys->camera[c].shake));
                        }
                        impact->snd[c] = 0;
                    }
                }
                impact->timer++;
            }
        }
    }
}

void SdPlayerSoundPlay(SdMachine *m, s32 no)
{
    s16 pan;

    if (no == 5) {
        pan = 0x40;
    } else {
        pan = (m->playerNo << 5) + 0x30;
    }
    HuAudFXPlayPan(playerSeIdTbl[m->charNo][no], pan);
}

HU3DMODELID SdMachineBodyCreate(s32 dataNum, s16 *modelIdOut)
{
    *modelIdOut = Hu3DModelCreateData(sdMachineBodyTbl[dataNum].dataNum);
    return *modelIdOut;
}

void SdMachineTireCreate(s32 tireType, s16 *modelIdOut)
{
    s32 mode = sdMachineTireTbl[tireType].unk34 & 0xF;
    s32 i;

    for (i = 0; i < 4; i++) {
        modelIdOut[i] = -1;
        if (mode == 1 ? i < 2 : (mode != 2 || i < 1)) {
            modelIdOut[i] = Hu3DModelCreateData(sdMachineTireTbl[tireType].dataNum[i]);
        }
    }
}

HU3DMODELID SdMachineGunCreate(s32 dataNum, s16 *modelIdOut)
{
    *modelIdOut = Hu3DModelCreateData(sdMachineGunTbl[dataNum].dataNum);
    return *modelIdOut;
}

HU3DMODELID SdMachineCharCreate(s32 dataNum, s16 *modelIdOut)
{
    *modelIdOut = Hu3DModelCreateData(playerModelTbl[dataNum]);
    return *modelIdOut;
}
