#include "game/board/main.h"
#include "game/board/effect.h"
#include "game/board/audio.h"
#include "game/board/scroll.h"
#include "game/board/model.h"
#include "game/board/masu.h"
#include "game/board/opening.h"
#include "game/board/capsule.h"
#include "game/board/branch.h"
#include "game/board/camera.h"
#include "game/board/player.h"
#include "game/board/window.h"
#include "game/board/coin.h"
#include "game/board/star.h"
#include "game/board/com.h"

#include "game/sprite.h"
#include "game/wipe.h"
#include "game/gamemes.h"
#include "game/pad.h"
#include "game/main.h"
#include "game/object.h"
#include "game/charman.h"
#include "game/hsfex.h"

#include <math.h>

/*
 * w03Dll - "Shy Guys Jungle Jam" board.
 *
 * NOTE (port): mb3_Work is one flat scratch buffer (0x518 bytes) reused as
 * several different sub-structures, addressed with raw byte offsets in the
 * original code (typical of MWCC -O0 code that never got a single named
 * struct). Several regions are shared/reinterpreted between unrelated
 * gameplay events (the roll-rock event, the pickaxe/coin-dig event and the
 * quicksand event all reuse the same per-player 0x64-byte scratch slots
 * with different meanings) - these are modelled below as a union of three
 * separate views, per event, rather than one struct with made-up shared
 * field names.
 */

/* ---- shared object work ---------------------------------------------- */

#define MB3_OBJ_MAX 9   /* mb3_ObjFileTbl entries / model ids: mb3_Work[0..8] */
#define MB3_BREAKABLE_MAX 0x14 /* footstep/impact debris + Asiba/RollRock player slots */

/* mb3_Work[0x22], 0x14 entries, stride 0x18: footstep/impact debris props
 * (the wobbling platform bits that fly up when KubiBone/RollRock hit). */
typedef struct MB3Debris_s {
    HuVecF vel;      /* 0x00 */
    u16 count;       /* 0x0C, frames remaining, 0 = free slot */
    s16 modelId;     /* 0x0E */
    s32 seId0;       /* 0x10 */
    s32 seId1;       /* 0x14 */
} MB3DEBRIS; /* size 0x18 */

/* mb3_Work[0x9A], 4 entries (one per player), stride 0x64: reused scratch
 * for whichever of the three "fall in/get carried" board events is active.
 * Only one of these events can run at a time, so overlaying them is safe. */

/* MB3Ev_RollRock / MB3_RollRockPathUpdate view */
typedef struct MB3RollRockPlayer_s {
    u8 pad0[0x8];       /* 0x00, unused by this view */
    s32 active;         /* 0x08, gate: 0 = not on the rock path, 1 = riding, 2 = stepping off */
    s32 orderNo;        /* 0x0C, arrival order (mb3_Work[0x1C] counter) */
    s32 state;          /* 0x10, state-machine step */
    s32 masuId;         /* 0x14, masu currently being approached on the rock path */
    s32 shookOnce;       /* 0x18 */
    f32 timer;          /* 0x1C */
    f32 gravity;         /* 0x20, precomputed jump-arc gravity term */
    f32 targetAngle;      /* 0x24 */
    u8 pad28[0xC];       /* 0x28, unused by this view */
    HuVecF pos;          /* 0x34 */
    HuVecF rot;           /* 0x40 (rot.y is the MBAngleMoveTo destination) */
    HuVecF vel;            /* 0x4C (vel.y accumulates gravity) */
    u8 pad58[0x8];         /* 0x58, unused by this view */
} MB3ROLLROCK_PLAYER; /* size 0x64 */

/* MB3Ev_Asiba / MB3_AsibaPlayerUpdate / MB3_AsibaPlayerEffCreate view */
typedef struct MB3AsibaPlayer_s {
    s32 active;          /* 0x00, gate */
    s32 areaNo;          /* 0x04, index into mb3ev_AsibaPosTbl/mb3ev_AsibaOfsTbl */
    u8 pad8[0x8];        /* 0x08, unused by this view */
    s32 state;           /* 0x10 */
    s32 masuId;          /* 0x14 */
    u8 pad18[0x4];       /* 0x18, unused by this view */
    f32 timer;           /* 0x1C */
    f32 gravity;         /* 0x20, precomputed sink/jump gravity term */
    u8 pad24[4];         /* 0x24, unused */
    s16 motionShiftId;   /* 0x28 */
    s16 motionRiseId;    /* 0x2A */
    s32 taki;            /* 0x30, taki (waterfall) splash-model index for this player */
    HuVecF pos;           /* 0x34 */
    f32 tiltAngle;        /* 0x40, X tilt used for the sink/rise lean */
    f32 yawAngle;          /* 0x44 */
    HuVecF vel;             /* 0x4C */
    f32 sinkDepth;           /* 0x50 */
    f32 wobble;              /* 0x54 */
} MB3ASIBA_PLAYER; /* size 0x64 */

/* MB3Ev_Turuhasi view - only element [0] of the array is used (single
 * digging player at a time). */
typedef struct MB3TuruhasiPlayer_s {
    u8 pad0[0x28];       /* 0x00, unused by this view */
    s16 motionA;          /* 0x28, MBPlayerMotionCreate() digging motion */
    s16 motionB;          /* 0x2A, MBPlayerMotionCreate() swing motion */
    u8 pad2C[4];          /* 0x2C, unused */
    s16 effModelId;       /* 0x30, sparkle effect model id (see MB3_TuruhasiEffCreate) */
    u8 pad32[2];           /* 0x32, unused */
    HuVecF pos;            /* 0x34, saved MBPlayerPosGet() */
    HuVecF rot;             /* 0x40, saved MBPlayerRotGet() */
    u8 pad4C[0x18];         /* 0x4C, unused by this view */
} MB3TURUHASI_PLAYER; /* size 0x64 */

typedef union MB3PlayerScratch_u {
    MB3ROLLROCK_PLAYER rollRock;
    MB3ASIBA_PLAYER asiba;
    MB3TURUHASI_PLAYER turuhasi;
} MB3_PLAYER_SCRATCH; /* size 0x64 */

/* mb3_Work[0xFE], 8 entries, stride 0x24: coin pick-up sparkle particles
 * used by the Turuhasi (pickaxe) event's coin reward animation. */
typedef struct MB3CoinSparkle_s {
    s16 life;      /* 0x00, frames remaining, 0 = free slot */
    s16 modelId;   /* 0x02 */
    HuVecF pos;    /* 0x04 */
    HuVecF vel;    /* 0x10 */
    f32 rotY;      /* 0x1C */
    f32 scale;     /* 0x20 */
} MB3COINSPARKLE; /* size 0x24 */

/* -------------------------------------------------------------------- */

static void MB3_Create(void);
static void MB3_Kill(void);
static void MB3_ObjectCreate(void);
static void MB3_MainObjUpdate(OMOBJ *obj);
static void MB3_LightSet(void);
static void MB3_LightReset(void);

static void MB3_Splash1EffHook(HU3DMODEL *modelP, MBEFFECT *effP, Mtx matrix);
static void MB3_Splash2EffHook(HU3DMODEL *modelP, MBEFFECT *effP, Mtx matrix);
static void MB3_TakiEffHook(HU3DMODEL *modelP, MBEFFECT *effP, Mtx matrix);
static void MB3_ImpactEffHook(HU3DMODEL *modelP, MBEFFECT *effP, Mtx matrix);
static void MB3_RollRockEffHook(HU3DMODEL *modelP, MBEFFECT *effP, Mtx matrix);
static void MB3_TuruhasiEffHook(HU3DMODEL *modelP, MBEFFECT *effP, Mtx matrix);

static void MB3_ImpactEffUpdate(s16 modelId, s32 frame);
static void MB3_ImpactEffPosSet(Point3d *pos, f32 pitch, f32 yaw);

static void MB3_RollRockPathUpdate(void);
static void MB3_RollRockEffUpdate(f32 *pos);
static void MB3_RollRockDustCreate(f32 *pos);
static void MB3_TuruhasiEffCreate(MB3_PLAYER_SCRATCH *entry);
static void MB3_AsibaPlayerUpdate(void);
static f32 MB3_AsibaMinYGet(s32 areaNo, Point3d *pos);
static void MB3_AsibaPlayerEffCreate(MB3_PLAYER_SCRATCH *entry, s32 kind);

static void MB3_CoinEffInit(void);
static void MB3_CoinEffClose(void);
static void MB3_CoinEffCreate(f32 *pos);
static void MB3_CoinEffUpdate(void);

static f32 MB3_DistRatioGet(Point3d *a, Point3d *b, Point3d *dir);
static s32 MB3Ev_BitGet(s32 flag, u32 mask);
static u32 MB3Ev_MasuFlagGet(s32 bits, s32 mask);

static int MB3Ev_MasuHatena(int playerNo);
static void MB3_Stub(void);
static int MB3Ev_MasuWalkPre(int playerNo);
static int MB3Ev_MasuWalkPost(int playerNo);
static void MB3Ev_KubiBone(MASU *masuP, s32 idx);
static void MB3Ev_YYHou(s32 playerNo);
static void MB3Ev_RollRock(s32 playerNo);
static void MB3Ev_Turuhasi(s32 playerNo);
static void MB3Ev_Asiba(s32 playerNo);

/* ------------------------------- data ---------------------------------- */

static const HuVecF mb3ev_AsibaPosTbl[8] = {
    { 3443.403f, -1884.013f, 0.0f },
    { 3300.175f, -969.244f, 0.0f },
    { 3142.127f, -829.8f, 0.0f },
    { 1363.629f, -754.184f, 0.0f },
    { 1344.007f, -325.654f, 0.0f },
    { 1235.695f, -239.724f, 0.0f },
    { 1.0f, -154.724f, 0.0f },
    { 1.0f, 345.276f, 0.0f },
};

static u32 mb3_ObjFileTbl[MB3_OBJ_MAX] = {
    0xCF000AU,
    0xCF0004U,
    0xCF000EU,
    0xCF0009U,
    0U,
    0xCF000CU,
    0xCF000DU,
    0xCF0005U,
    0xCF000BU,
};

static const HuVecF mb3ev_TakiEffPosTbl[3] = {
    { 0.0f, 3345.0f, -1885.0f },
    { 0.0f, 1285.0f, -775.0f },
    { 0.0f, -35.0f, -185.0f },
};

static const f32 mb3ev_KubiBoneAngleTbl[4] = { 0.0f, 10.0f, 60.0f, 0.0f };
static const f32 mb3ev_TuruhasiRadiusTbl[4] = { 180.0f, 90.0f, 270.0f, 0.0f };

/* per-area offset applied to the quicksand target position, one entry per
 * mb3ev_AsibaPosTbl segment. */
static const HuVecF mb3ev_AsibaOfsTbl[4];

static const s32 mb3ev_AsibaYureTimeTbl[4] = { 0x30, 0x33, 0x36, 0x39 };

static OMOBJ *mb3_MainObj;
static s32 mb3_Work[0x146];
static s16 mb3_MainMdlId;

/* Raw byte-offset accessors into mb3_Work's header region (object/model ids,
 * motion & effect handles): kept as literal offsets rather than a made-up
 * struct so the mapping stays a mechanical, verifiable match to the
 * original asm (MWCC -O0 addresses this buffer byte-wise, not word-wise). */
#define MB3W_S16(off) (*(s16 *) ((u8 *) mb3_Work + (off)))
#define MB3W_U16(off) (*(u16 *) ((u8 *) mb3_Work + (off)))
#define MB3W_S32(off) (*(s32 *) ((u8 *) mb3_Work + (off)))
#define MB3W_F32(off) (*(f32 *) ((u8 *) mb3_Work + (off)))

#define MB3_DEBRIS ((MB3DEBRIS *) ((u8 *) mb3_Work + 0x88))          /* [0x14] */
#define MB3_PLAYERSCR ((MB3_PLAYER_SCRATCH *) ((u8 *) mb3_Work + 0x268)) /* [4] */
#define MB3_COIN ((MB3COINSPARKLE *) ((u8 *) mb3_Work + 0x3F8))      /* [8] */


/* MB3_Create camera constants (extracted from the original REL's .rodata,
 * m2c could not infer these as it has no context for anonymous data). */
static const HuVecF mb3_OpeningPos = { 3300.175048828125f, -969.2440185546875f, 0.0f };
static const HuVecF mb3_OpeningRot = { 3142.126953125f, -829.7999877929688f, 0.0f };
static const HuVecF mb3_MapViewPos = { 1363.6290283203125f, -754.1840209960938f, 0.0f };
static const HuVecF mb3_MapViewRot = { 1344.0069580078125f, -325.65399169921875f, 0.0f };

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

extern void ObjectSetup(void);

int _prolog(void) {
    const VoidFunc* ctors = _ctors;
    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void) {
    const VoidFunc* dtors = _dtors;
    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

void ObjectSetup(void) {
    GWPartyFSet(TRUE);
    MBObjectSetup(2, MB3_Create, MB3_Kill);
}

/* ----------------------------- leaf helpers ----------------------------- */

static f32 MB3_DistRatioGet(Point3d *a, Point3d *b, Point3d *dir) {
    f32 mag = VECMag(dir);
    if (mag > 0.0f) {
        mag = (dir->x * (a->x - b->x) + dir->y * (a->y - b->y) + dir->z * (a->z - b->z)) / mag;
    }
    return mag;
}

/* NONMATCHING: register allocation order differs (73% match) */
static s32 MB3Ev_BitGet(s32 flag, u32 mask) {
    s32 result;
    u32 inBit;
    u32 outBit;
    s32 i;

    flag = flag & mask;
    result = 0;
    inBit = 1;
    outBit = inBit;
    i = 0;
    for (; i < 0x20; i++) {
        if (mask & inBit) {
            if (flag & inBit) {
                result |= outBit;
            }
            outBit *= 2;
        }
        inBit *= 2;
    }
    return result;
}

/* NONMATCHING: register allocation order differs */
static u32 MB3Ev_MasuFlagGet(s32 bits, s32 mask) {
    u32 result;
    s32 inBit;
    s32 outBit;
    s32 i;

    inBit = 1;
    outBit = inBit;
    result = 0;
    i = 0;
    for (; i < 0x20; i++) {
        if (mask & outBit) {
            if (bits & inBit) {
                result |= outBit;
            }
            inBit *= 2;
        }
        outBit *= 2;
    }
    return result;
}

static f32 MB3_AsibaMinYGet(s32 areaNo, Point3d *pos) {
    Point3d seg;
    VECSubtract((Point3d *) &mb3ev_AsibaPosTbl[areaNo + 1], (Point3d *) &mb3ev_AsibaPosTbl[areaNo], &seg);
    if (fabsf(seg.z) < 0.1f) {
        return mb3ev_AsibaPosTbl[areaNo].y;
    }
    return (seg.y * ((pos->z - mb3ev_AsibaPosTbl[areaNo].z) / seg.z)) + mb3ev_AsibaPosTbl[areaNo].y;
}

static void MB3_LightSet(void) {
    Hu3DShadowCreate(45.0f, 1000.0f, 250000.0f);
    Hu3DShadowTPLvlSet(0.3f);
    Hu3DBGColorSet(0, 0, 0);
    Hu3DModelLightInfoSet(MBModelIdGet(mb3_MainMdlId), TRUE);
}

static void MB3_LightReset(void) {
}

static void MB3_Stub(void) {
}

static int MB3Ev_MasuWalkPre(int playerNo) {
}

/* ------------------------------ effect hooks ---------------------------- */

static void MB3_Splash1EffHook(HU3DMODEL *modelP, MBEFFECT *effP, Mtx matrix) {
    MBEFFECTDATA *p;
    s32 i;

    if (effP->count == 0) {
        p = effP->data;
        for (i = 0; i < effP->num; i++, p++) {
            p->scale = 0.0f;
            p->color.a = 0;
            p->time = (s16) -(i >> 3);
        }
        effP->count = 1;
    }
    p = effP->data;
    for (i = 0; i < effP->num; i++, p++) {
        if (p->time < 0) {
            p->time++;
            continue;
        }
        if (p->time == 0) {
            f32 r;
            p->time++;
            p->rot.z = 360.0f * MBEffRandF();
            r = MBEffRandF();
            p->color.r = (u8) (0.98f * (160.0f + (70.0f * r)));
            p->color.g = (u8) (0.98f * (200.0f + (40.0f * r)));
            p->color.b = (u8) (0.98f * (180.0f + (55.0f * r)));
            p->vel.x = 1.6666666f * (0.8f * MBEffSRandF());
            p->vel.z = 1.6666666f + (1.6666666f * (2.0f * MBEffRandF()));
            p->vel.y = 6.6666665f + (1.6666666f * (4.0f * MBEffRandF()));
            p->parManId = (s16) (effP->num >> 3);
            p->pos.x = 250.0f * MBEffSRandF();
            p->pos.z = 100.0f * (0.3f * MBEffSRandF());
            p->pos.y = 100.0f * (0.5f * MBEffRandF());
            p->color.a = (u8) (180.0f + (70.0f * MBEffRandF()));
            p->scale = 60.0f + (40.0f * MBEffRandF());
        }
        VECAdd(&p->pos, &p->vel, &p->pos);
        p->vel.y += -0.5444445f;
        p->parManId -= 1;
        if (p->parManId <= 0) {
            p->time = 0;
            p->scale = 0.0f;
            p->color.a = 0;
        } else if (p->parManId < 0xA) {
            if (p->color.a >= 0x14) {
                p->color.a -= 0x14;
            }
            p->scale += 5.0f;
        }
    }
}

static void MB3_Splash2EffHook(HU3DMODEL *modelP, MBEFFECT *effP, Mtx matrix) {
    MBEFFECTDATA *p;
    s32 i;

    if (effP->count == 0) {
        p = effP->data;
        for (i = 0; i < effP->num; i++, p++) {
            p->scale = 0.0f;
            p->color.a = 0;
            p->time = (s16) -(i >> 1);
        }
        effP->count = 1;
        effP->blendMode = 1;
    }
    p = effP->data;
    for (i = 0; i < effP->num; i++, p++) {
        if (p->time < 0) {
            p->time++;
            continue;
        }
        if (p->time == 0) {
            p->time++;
            p->rot.z = 360.0f * MBEffRandF();
            p->color.b = 0xFF;
            p->color.g = 0xFF;
            p->color.r = 0xFF;
            p->vel.x = 1.6666666f * (0.8f * MBEffSRandF());
            p->vel.z = 1.6666666f + (1.6666666f * (4.0f * MBEffRandF()));
            p->vel.y = 4.1666665f + (1.6666666f * (2.5f * MBEffRandF()));
            p->parManId = (s16) (effP->num >> 1);
            p->pos.x = 250.0f * MBEffSRandF();
            p->pos.z = (100.0f * (0.3f * MBEffSRandF())) - 50.0f;
            p->pos.y = 50.0f + (100.0f * (1.5f * MBEffRandF()));
            p->color.a = (u8) (20.0f + (20.0f * MBEffRandF()));
            p->scale = 120.0f + (50.0f * MBEffRandF());
        }
        VECAdd(&p->pos, &p->vel, &p->pos);
        p->vel.y += -0.5444445f;
        p->parManId -= 1;
        if (p->parManId <= 0) {
            p->time = 0;
            p->scale = 0.0f;
            p->color.a = 0;
        } else if (p->parManId < 0xA) {
            if (p->color.a >= 0xA) {
                p->color.a -= 0xA;
            }
            p->scale += 5.0f;
        }
    }
}

static void MB3_TakiEffHook(HU3DMODEL *modelP, MBEFFECT *effP, Mtx matrix) {
    MBEFFECTDATA *p;
    s32 i;

    if (effP->count == 0) {
        p = effP->data;
        for (i = 0; i < effP->num; i++, p++) {
            p->scale = 0.0f;
            p->color.a = 0;
        }
        effP->count = 1;
    }
    p = effP->data;
    for (i = 0; i < effP->num; i++, p++) {
        if (p->time != 0) {
            VECAdd(&p->pos, &p->vel, &p->pos);
            p->vel.y += -0.5444445f;
            p->parManId -= 1;
            if (p->parManId <= 0) {
                p->time = 0;
                p->scale = 0.0f;
                p->color.a = 0;
            } else if (p->parManId < 0xA) {
                if (p->color.a >= 0x14) {
                    p->color.a -= 0x14;
                }
                p->scale += 5.0f;
            }
        }
    }
}

static void MB3_TuruhasiEffHook(HU3DMODEL *modelP, MBEFFECT *effP, Mtx matrix) {
    MBEFFECTDATA *p;
    s32 i;

    if (effP->count == 0) {
        p = effP->data;
        for (i = 0; i < effP->num; i++, p++) {
            p->scale = 0.0f;
            p->color.a = 0;
        }
        effP->count = 1;
    }
    p = effP->data;
    for (i = 0; i < effP->num; i++, p++) {
        if (p->time != 0) {
            VECAdd(&p->pos, &p->vel, &p->pos);
            p->vel.y += -0.5444445f;
            p->parManId -= 1;
            if (p->parManId <= 0) {
                p->time = 0;
                p->scale = 0.0f;
                p->color.a = 0;
            } else if (p->parManId < 0xA && p->color.a >= 0x14) {
                p->color.a -= 0x14;
            }
        }
    }
}

static void MB3_RollRockEffHook(HU3DMODEL *modelP, MBEFFECT *effP, Mtx matrix) {
    MBEFFECTDATA *p;
    s32 i;

    p = effP->data;
    for (i = 0; i < effP->num; i++, p++) {
        if (p->time != 0) {
            VECAdd(&p->pos, &p->vel, &p->pos);
            p->vel.x *= 0.94f;
            p->vel.z *= 0.94f;
            p->scale += 6.0f;
            p->color.a = (u8) ((f32) p->color.a * 0.99f);
            if (p->time < 24) {
                p->color.a = (u8) (0.9f * (f32) p->color.a);
            }
            if (--p->time == 0) {
                p->time = 0;
                p->scale = 0.0f;
                p->color.a = 0;
            }
        }
    }
}

static void MB3_ImpactEffHook(HU3DMODEL *modelP, MBEFFECT *effP, Mtx matrix) {
    MBEFFECTDATA *p;
    s32 i;

    p = effP->data;
    for (i = 0; i < effP->num; i++, p++) {
        if (p->time != 0) {
            VECAdd(&p->pos, &p->vel, &p->pos);
            p->vel.x *= 0.9f;
            p->vel.y *= 0.9f;
            p->vel.z *= 0.9f;
            p->scale += 6.0f;
            p->color.a = (u8) ((f32) p->color.a * 0.98f);
            if (p->time < 24) {
                p->color.a = (u8) (0.9f * (f32) p->color.a);
            }
            if (--p->time == 0) {
                p->time = 0;
                p->scale = 0.0f;
                p->color.a = 0;
            }
        }
    }
}

/* ------------------------- setup / teardown ----------------------------- */

static void MB3_Create(void) {
    int boardNo = MBBoardNoGet();

    HuAudSndGrpSetSet(0x10);
    memset(mb3_Work, 0, sizeof(mb3_Work));
    MBMasuCreate(0xCF0000);
    mb3_MainMdlId = MBModelCreate(0xCF0001, NULL, FALSE);
    MBModelPosSet(mb3_MainMdlId, 0.0f, 0.0f, 0.0f);
    MBModelAttrSet(mb3_MainMdlId, HU3D_MOTATTR_LOOP);
    MBScrollInit(0xCF0002U, 20000.0f, -1700.0f);
    MBOpeningViewSet((HuVecF *) &mb3_OpeningRot, (HuVecF *) &mb3_OpeningPos, 20000.0f);
    MBMapViewCameraSet((HuVecF *) &mb3_MapViewRot, (HuVecF *) &mb3_MapViewPos, 20000.0f);
    MBLightHookSet(MB3_LightSet, MB3_LightReset);
    MBCapsuleColCreate(0xCF0003);
    MBBranchFlagSet(0x40U);
    MBBranchFlagSet(2U);
    MB3_ObjectCreate();
    HuDataDirClose(0xCF0000);
    MBMasuWalkPreHookSet(MB3Ev_MasuWalkPre);
    MBMasuWalkPostHookSet((MASUPLAYERHOOK) MB3Ev_MasuWalkPost);
    MBMasuHatenaHookSet(MB3Ev_MasuHatena);
    mb3_MainObj = omAddObjEx(mbObjMan, -0x8000, 0, 0, -1, MB3_MainObjUpdate);
}

static void MB3_ObjectCreate(void) {
    s32 i;
    MASU *masuP, *linkMasuP;
    s32 modelId;
    u16 linkMasu;

    for (i = 0; i < MB3_OBJ_MAX; i++) {
        mb3_Work[i] = -1;
        if (mb3_ObjFileTbl[i] != 0U) {
            modelId = MBModelCreate((s32) mb3_ObjFileTbl[i], NULL, FALSE);
            mb3_Work[i] = modelId;
            MBModelPosSet((s16) modelId, 0.0f, 0.0f, 0.0f);
            MBMotionSpeedSet((s16) modelId, 0.0f);
        }
    }
    MBModelAttrSet((s16) mb3_Work[0], HU3D_MOTATTR_LOOP);
    MBMotionSpeedSet((s16) mb3_Work[0], 1.0f);
    MBModelDispSet((s16) mb3_Work[8], FALSE);

    for (i = 0; i < MB3_BREAKABLE_MAX; i++) {
        MB3_DEBRIS[i].modelId = -1;
        modelId = MBModelCreate(0xCF0006, NULL, TRUE);
        MB3_DEBRIS[i].modelId = (s16) modelId;
        MBModelPosSet((s16) modelId, 0.0f, 0.0f, 0.0f);
        MBModelAttrSet((s16) modelId, HU3D_MOTATTR_LOOP);
        MBModelDispSet((s16) modelId, FALSE);
    }

    /* place the boulder start marker on a rock-track masu, if this board has one */
    if (mb3_Work[2] > 0) {
        s32 startMasuId = MBMasuFlagFind(0, 0x800U, 0x800U);
        s32 i2;
        u16 chosen = 0xFFFF;

        linkMasuP = MBMasuGet(0, startMasuId);
        for (i2 = 0; i2 < (s32) linkMasuP->linkNum; i2++) {
            chosen = linkMasuP->linkTbl[i2];
            if (MBMasuGet(0, (s32) chosen)->linkNum != 0) {
                chosen = 0xFFFF;
                continue;
            }
            break;
        }
        if ((s16) chosen < 0) {
            chosen = (u16) startMasuId;
        }
        mb3_Work[9] = (s32) chosen;
        masuP = MBMasuGet(0, (s32) chosen);
        MBModelDispSet((s16) mb3_Work[2], FALSE);
        MBModelPosSetV((s16) mb3_Work[2], &masuP->pos);
        MBModelRotSet((s16) mb3_Work[2], 0.0f, (f32) (180.0 * (atan2((f64) (linkMasuP->pos.x - masuP->pos.x), (f64) (linkMasuP->pos.z - masuP->pos.z)) / 3.141592653589793)), 0.0f);
        MB3W_S16(0x60) = (s16) MBMotionCreate((s16) mb3_Work[2], 0xCF000F);
        MB3W_S16(0x62) = (s16) MBMotionCreate((s16) mb3_Work[2], 0xCF0010);
        MBMotionNoSet((s16) mb3_Work[2], MB3W_S16(0x60), 0U);
        MBMotionSpeedSet((s16) mb3_Work[2], 0.0f);
    }

    /* find the two waterfall (taki) masus, if this board has a waterfall model */
    if (mb3_Work[5] > 0) {
        s32 masuNum = MBMasuNumGet(0);
        s32 i3;
        for (i3 = 1; i3 <= masuNum; i3++) {
            MASU *m = MBMasuGet(0, i3);
            if (m->flag & 0x4000) {
                if (m->pos.y > 2000.0f) {
                    mb3_Work[0xB] = i3;
                } else {
                    mb3_Work[0xC] = i3;
                }
            }
        }
    }

    mb3_Work[0xD] = (s32)(uintptr_t) HuSprAnimRead(HuDataReadNum(0xCF0011, 0x10000000));
    mb3_Work[0xE] = (s32)(uintptr_t) HuSprAnimRead(HuDataReadNum(0xCF0012, 0x10000000));
    mb3_Work[0xF] = (s32)(uintptr_t) HuSprAnimRead(HuDataReadNum(0xCF0014, 0x10000000));
    mb3_Work[0x10] = (s32)(uintptr_t) HuSprAnimRead(HuDataReadNum(0xCF0015, 0x10000000));

    for (i = 0; i < 4; i++) {
        s16 id = (s16) MBEffCreate((ANIMDATA *)(uintptr_t) mb3_Work[0xD], 0x40);
        MB3W_S16(0x44 + i * 2) = id;
        MBEffHookSet(id, MB3_TakiEffHook);
        Hu3DModelLayerSet(id, 3);
    }
    for (i = 0; i < 3; i++) {
        s16 id1 = (s16) MBEffCreate((ANIMDATA *)(uintptr_t) mb3_Work[0xE], 0xC0);
        s16 id2;
        MB3W_S16(0x4C + i * 2) = id1;
        MBEffHookSet(id1, MB3_Splash1EffHook);
        Hu3DModelLayerSet(id1, 2);
        Hu3DModelPosSetV(id1, (HuVecF *) &mb3ev_TakiEffPosTbl[i]);

        id2 = (s16) MBEffCreate((ANIMDATA *)(uintptr_t) mb3_Work[0xE], 0x30);
        MB3W_S16(0x52 + i * 2) = id2;
        MBEffHookSet(id2, MB3_Splash2EffHook);
        Hu3DModelLayerSet(id2, 2);
        Hu3DModelPosSet(id2, mb3ev_TakiEffPosTbl[i].x, mb3ev_TakiEffPosTbl[i].y, 50.0f + mb3ev_TakiEffPosTbl[i].z);
        MBAudFXEmiterSet(0x5DC, (Vec *) &mb3ev_TakiEffPosTbl[i]);
    }
    MB3W_S16(0x58) = (s16) MBEffCreate((ANIMDATA *)(uintptr_t) mb3_Work[0xF], 1);
    MB3W_S16(0x5A) = (s16) MBEffCreate((ANIMDATA *)(uintptr_t) mb3_Work[0x10], 1);
}

static void MB3_Kill(void) {
    s32 i;

    if (mb3_MainMdlId >= 0) {
        MBModelKill(mb3_MainMdlId);
        mb3_MainMdlId = -1;
    }
    MBCameraModelViewSet(-1, NULL, NULL, -1.0f, 45.0f, 0x3F);
    for (i = 0; i < MB3_OBJ_MAX; i++) {
        if (mb3_Work[i] >= 0) {
            MBModelKill((s16) mb3_Work[i]);
        }
        mb3_Work[i] = -1;
    }
    for (i = 0; i < MB3_BREAKABLE_MAX; i++) {
        if (MB3_DEBRIS[i].modelId >= 0) {
            MBModelKill(MB3_DEBRIS[i].modelId);
        }
        MB3_DEBRIS[i].modelId = -1;
    }
    for (i = 0; i < 4; i++) {
        s16 id = MB3W_S16(0x44 + i * 2);
        if (id >= 0) {
            MBEffKill(id);
        }
        MB3W_S16(0x44 + i * 2) = -1;
    }
    for (i = 0; i < 3; i++) {
        s16 id1 = MB3W_S16(0x4C + i * 2);
        s16 id2 = MB3W_S16(0x52 + i * 2);
        if (id1 >= 0) {
            MBEffKill(id1);
        }
        MB3W_S16(0x4C + i * 2) = -1;
        if (id2 >= 0) {
            MBEffKill(id2);
        }
        MB3W_S16(0x52 + i * 2) = -1;
    }
    if (MB3W_S16(0x58) >= 0) {
        MBEffKill(MB3W_S16(0x58));
    }
    if (MB3W_S16(0x5A) >= 0) {
        MBEffKill(MB3W_S16(0x5A));
    }
    MB3W_S16(0x5A) = -1;
    MB3W_S16(0x58) = -1;
}

static void MB3_MainObjUpdate(OMOBJ *obj) {
    Point3d pos;
    s32 i;
    MB3DEBRIS *d;

    if (MBKillCheck()) {
        if (mb3_MainObj != NULL) {
            omDelObjEx(mbObjMan, mb3_MainObj);
        }
        return;
    }
    (void) MBCameraGet();

    /* the four taki (waterfall) splash pairs each carry a countdown at
     * mb3_Work word 0x1E + i*6 in the original (part of the packed model-id
     * region described in MB3_ObjectCreate). */
    for (i = 0; i < 4; i++) {
        s32 *cnt = &mb3_Work[0x1E + i * 6];
        if (*cnt != 0) {
            *cnt -= 1;
        }
    }

    d = MB3_DEBRIS;
    for (i = 0; i < MB3_BREAKABLE_MAX; i++, d++) {
        if (d->count != 0) {
            s16 modelId = d->modelId;
            d->count -= 1;
            if (d->count == 0) {
                MBModelDispSet(modelId, FALSE);
                d->seId1 = -1;
                d->seId0 = -1;
            } else {
                if (d->vel.y < 13.333333f) {
                    d->vel.y += 0.1388889f;
                }
                MBModelPosGet(modelId, &pos);
                VECAdd(&pos, (Point3d *) &d->vel, &pos);
                MBModelPosSetV(modelId, &pos);
                if (d->seId0 >= 0) {
                    HuAudFXEmiterUpDate(d->seId0, &pos);
                }
                if (d->seId1 >= 0) {
                    HuAudFXEmiterUpDate(d->seId1, &pos);
                }
            }
        }
    }

    if (mb3_Work[0x1D] != 0) {
        Mtx mtx;
        Hu3DModelObjMtxGet(MBModelIdGet((s16) mb3_Work[3]), "yybone3-bnhook", mtx);
        MBMasuMtxSet(MBMasuFlagMatchFind(0, 0x80U), mtx);
        if (MBMotionEndCheck((s16) mb3_Work[3])) {
            mb3_Work[0x1D] = 0;
        }
    }
}

static void MB3_ImpactEffUpdate(s16 modelId, s32 frame) {
    Mtx camInv;
    Point3d ofs;
    Point3d pos;
    f32 scale;

    ofs.x = 120.0f;
    ofs.y = 0.0f;
    ofs.z = -60.0f;
    MTXInverse(Hu3DCameraMtx, camInv);
    MTXMultVec(camInv, &ofs, &pos);
    camInv[0][3] = 0.0f;
    camInv[1][3] = 0.0f;
    camInv[2][3] = 0.0f;
    scale = ((1.0f + (5.0f * ((f32) frame / 30.0f))) + (0.5f * (f32) sin((3.141592653589793 * (f64) (16.0f * (360.0f * ((f32) frame / 30.0f)))) / 180.0))) * 0.4f;
    MBModelPosSetV(modelId, &pos);
    MBModelMtxSet(modelId, &camInv);
    MBModelScaleSet(modelId, scale, scale, scale);
}

static void MB3_CoinEffInit(void) {
    s32 i;
    MB3COINSPARKLE *c = MB3_COIN;
    memset(c, 0, 8 * sizeof(MB3COINSPARKLE));
    for (i = 0; i < 8; i++, c++) {
        c->modelId = MBModelCreate(0x80002, NULL, TRUE);
        MBModelDispSet(c->modelId, FALSE);
        c->scale = 1.0f;
    }
}

static void MB3_CoinEffClose(void) {
    s32 i;
    MB3COINSPARKLE *c = MB3_COIN;
    for (i = 0; i < 8; i++, c++) {
        if (c->modelId != 0) {
            MBModelKill(c->modelId);
        }
        c->modelId = 0;
    }
}

static void MB3_CoinEffCreate(f32 *pos) {
    s32 i;
    MB3COINSPARKLE *c = MB3_COIN;
    f32 ang, spd;

    for (i = 0; i < 8; i++, c++) {
        if (c->life != 0) {
            continue;
        }
        c->life = 0x3C;
        MBModelDispSet(c->modelId, TRUE);
        c->pos.x = pos[0];
        c->pos.y = pos[1];
        c->pos.z = pos[2];
        ang = 360.0f * MBEffRandF();
        spd = 2.1666665f + (1.6666666f * (1.3f * MBEffRandF()));
        c->vel.x = spd * (f32) sin((3.141592653589793 * (f64) ang) / 180.0);
        c->vel.z = spd * (f32) cos((3.141592653589793 * (f64) ang) / 180.0);
        c->vel.y = 21.666666f;
        c->rotY = 360.0f * MBEffRandF();
        c->scale = 0.2f;
        return;
    }
}

static void MB3_CoinEffUpdate(void) {
    s32 i;
    MB3COINSPARKLE *c = MB3_COIN;
    for (i = 0; i < 8; i++, c++) {
        if (c->life != 0) {
            VECAdd(&c->pos, &c->vel, &c->pos);
            c->vel.y += -0.5444445f;
            c->rotY += 6.0000005f;
            c->scale += 0.1f * (1.0f - c->scale);
            MBModelPosSetV(c->modelId, &c->pos);
            MBModelRotSet(c->modelId, 0.0f, c->rotY, 0.0f);
            MBModelScaleSet(c->modelId, c->scale, c->scale, c->scale);
            c->life -= 1;
            if (c->life == 0) {
                MBModelDispSet(c->modelId, FALSE);
            }
        }
    }
}

/* --------------------------- TODO: not yet ported ------------------------ */

/* NONMATCHING: rewritten as for+continue/break instead of the original
 * fused "occupied-check || (spawn, budget--, budget>0)" condition; spawns
 * up to one new particle into the first free slot it finds. */
static void MB3_ImpactEffPosSet(Point3d *pos, f32 pitch, f32 yaw) {
    MBEFFECT *eff = (MBEFFECT *) Hu3DData[MB3W_S16(0x5C)].hookData;
    Mtx pitchMtx, yawMtx, mtx;
    Point3d ofs;
    s32 i;

    MTXRotRad(pitchMtx, 'X', 0.017453292f * pitch);
    MTXRotRad(yawMtx, 'Y', 0.017453292f * yaw);
    MTXConcat(pitchMtx, yawMtx, pitchMtx);
    MTXTransApply(pitchMtx, mtx, pos->x, pos->y, pos->z);

    for (i = 0; i < eff->num; i++) {
        MBEFFECTDATA *p = &eff->data[i];
        f32 ang, spd;

        if (p->time != 0) {
            continue;
        }
        p->time = (s16) (18.0f + (0.2f * (60.0f * MBEffRandF())));
        p->color.r = (u8) (150.0f + (100.0f * MBEffRandF()));
        p->color.g = 0xFF;
        p->color.b = 0xFF;
        p->color.a = (u8) (150.0f + (100.0f * MBEffRandF()));
        p->scale = 20.0f + (40.0f * MBEffRandF());

        ang = 360.0f * MBEffSRandF();
        spd = 20.0f;
        ofs.x = spd * (f32) sin((3.141592653589793 * (f64) ang) / 180.0);
        ofs.y = 0.0f;
        ofs.z = spd * (f32) cos((3.141592653589793 * (f64) ang) / 180.0);
        MTXMultVec(mtx, &ofs, &p->pos);

        spd = 13.333333f + (1.6666666f * (7.0f * MBEffRandF()));
        ofs.x = spd * (f32) sin((3.141592653589793 * (f64) ang) / 180.0);
        ofs.z = spd * (f32) cos((3.141592653589793 * (f64) ang) / 180.0);
        MTXMultVecSR(mtx, &ofs, &p->vel);
        break;
    }
}

/* Drives the four rock-track riders' per-frame state machine (see
 * MB3ROLLROCK_PLAYER above). NONMATCHING: several of the original's fused
 * fall-through switch cases (states 1-3, 4-5) are unwound into separate
 * blocks below for clarity; semantics preserved. */
static void MB3_RollRockPathUpdate(void) {
    Point3d rockPos, rockVel, approach, dir;
    f32 rockYaw;
    s32 rockLinked;
    s32 masuId = mb3_Work[0xA];
    MASU *masuP;
    f32 remaining = 450.0f;
    s32 p;

    rockPos.x = MB3W_F32(0x64);
    rockPos.y = MB3W_F32(0x68);
    rockPos.z = MB3W_F32(0x6C);

    for (;;) {
        masuP = MBMasuGet(0, masuId);
        VECSubtract(&masuP->pos, &rockPos, &approach);
        approach.y = 0.0001f;
        {
            f32 segLen = VECMag(&approach);
            remaining -= segLen;
            if (remaining <= 0.0f) {
                dir = approach;
                VECScale(&dir, &dir, (remaining + segLen) / segLen);
                break;
            }
            rockPos = masuP->pos;
            masuId = (s32) masuP->linkTbl[0];
        }
    }
    rockYaw = (f32) (180.0 * (atan2((f64) dir.x, (f64) dir.z) / 3.141592653589793));
    VECAdd(&rockPos, &dir, &rockPos);
    VECNormalize(&dir, &rockVel);
    rockLinked = (masuP->flag & 0x40) == 0;

    MB3W_F32(0x64) = rockPos.x;
    MB3W_F32(0x68) = rockPos.y;
    MB3W_F32(0x6C) = rockPos.z;

    for (p = 0; p < 4; p++) {
        MB3ROLLROCK_PLAYER *e = &MB3_PLAYERSCR[p].rollRock;
        Point3d ridePos;

        if (e->active == 0) {
            continue;
        }
        masuP = MBMasuGet(0, e->masuId);
        switch (e->state) {
        case 0:
            MBPlayerPosGet(p, &e->pos);
            MBPlayerRotGet(p, &e->rot);
            e->orderNo = -1;
            e->state++;
            /* the original falls through to case 1 in the same tick */
        case 1:
            MBAngleMoveTo(&e->rot.y, 0.0f, 0.1f);
            VECSubtract(&rockPos, &masuP->pos, &approach);
            approach.y = 0.01f;
            if (VECMag(&approach) <= 600.0f) {
                e->orderNo = mb3_Work[0x1C]++;
                MBPlayerMotionNoShiftSet(p, 9, 0.0f, 0.0f, 0U);
                e->timer = 36.0f;
                e->state++;
            } else {
                break;
            }
            /* fallthrough */
        case 2:
            MBAngleMoveTo(&e->rot.y, (f32) (180.0 * (atan2((f64) (rockPos.x - e->pos.x), (f64) (rockPos.z - e->pos.z)) / 3.141592653589793)), 0.1f);
            e->timer -= 1.0f;
            if (e->timer <= 0.0f) {
                MBPlayerMotionNoShiftSet(p, 3, 0.0f, 8.0f, 0x40000001U);
                e->state++;
            } else {
                break;
            }
            /* fallthrough */
        case 3:
            VECScale(&rockVel, &approach, 14.166666f);
            VECAdd(&e->pos, &approach, &e->pos);
            {
                f32 a = rockYaw + mb3ev_TuruhasiRadiusTbl[e->orderNo];
                ridePos.x = (f32) (100.0 * (1.2000000476837158 * sin((3.141592653589793 * (f64) a) / 180.0)));
                ridePos.y = 0.0f;
                ridePos.z = (f32) (100.0 * (1.2000000476837158 * cos((3.141592653589793 * (f64) a) / 180.0)));
            }
            VECAdd(&rockPos, &ridePos, &approach);
            VECSubtract(&approach, &e->pos, &approach);
            VECScale(&approach, &approach, 0.08f);
            VECAdd(&approach, &e->pos, &e->pos);
            MBAngleMoveTo(&e->rot.y, rockYaw, 0.15f);
            if (rockLinked) {
                VECAdd(&masuP->pos, &ridePos, &approach);
                VECSubtract(&approach, &e->pos, &approach);
                approach.x /= 0.8f;
                approach.z /= 0.8f;
                approach.y = (approach.y / 0.8f) - (0.5f * 0.8f * -2450.0f);
                VECScale(&approach, &e->vel, 0.016666668f);
                e->timer = 60.0f * 0.8f;
                e->gravity = 0.016666668f * (0.016666668f * -2450.0f);
                e->targetAngle = rockYaw;
                MBPlayerMotionNoShiftSet(p, 4, 0.0f, 8.0f, 0U);
                e->shookOnce = 0;
                e->masuId = (s32) masuP->linkTbl[0];
                e->state++;
            }
            break;
        case 4:
            e->vel.y += e->gravity;
            VECAdd(&e->pos, &e->vel, &e->pos);
            if (e->shookOnce == 0 && e->timer == 5.0f) {
                MBPlayerMotionNoShiftSet(p, 5, 0.0f, 0.0f, 0U);
                e->shookOnce = 1;
            }
            MBAngleMoveTo(&e->rot.y, e->targetAngle, 0.15f);
            e->timer -= 1.0f;
            if (e->timer <= 0.0f) {
                e->active = 2;
                e->timer = 12.0f;
                e->state++;
            }
            break;
        case 5:
            e->timer -= 1.0f;
            if (e->timer <= 0.0f) {
                MBPlayerMotionNoShiftSet(p, 1, 0.0f, 8.0f, 0x40000001U);
                e->state++;
            }
            break;
        case 6:
            MBAngleMoveTo(&e->rot.y, 0.0f, 0.15f);
            e->active = 2;
            break;
        case -1:
            GwPlayer[p].masuId = (s16) e->masuId;
            MBPlayerPosReset(p);
            e->vel.x = 0.0f;
            e->vel.y = 0.0f;
            e->vel.z = 0.0f;
            MBPlayerMotionNoShiftSet(p, 1, 0.0f, 0.0f, 0x40000001U);
            break;
        }
        if (e->state >= 0) {
            MBPlayerPosSetV(p, &e->pos);
        }
        MBPlayerRotSetV(p, &e->rot);
    }
}

/* NONMATCHING: spawns up to one particle per call, same reasoning as
 * MB3_ImpactEffPosSet above. */
static void MB3_RollRockEffUpdate(f32 *pos) {
    MBEFFECT *eff = (MBEFFECT *) Hu3DData[MB3W_S16(0x5C)].hookData;
    s32 i;

    for (i = 0; i < eff->num; i++) {
        MBEFFECTDATA *p = &eff->data[i];
        f32 ang, spd;

        if (p->time != 0) {
            continue;
        }
        p->time++;
        p->rot.z = 360.0f * MBEffRandF();
        (void) MBEffRandF();
        p->color.r = 0xE6;
        p->color.g = 0xFF;
        p->color.b = 0xFF;
        p->pos.x = pos[0] + (100.0f * (0.2f * MBEffSRandF()));
        p->pos.y = pos[1] - 150.0f;
        p->pos.z = pos[2] + (100.0f * (0.2f * MBEffSRandF()));
        ang = 360.0f * MBEffRandF();
        spd = 13.333333f + (1.6666666f * (5.0f * MBEffRandF()));
        p->vel.x = spd * (f32) sin((3.141592653589793 * (f64) ang) / 180.0);
        p->vel.z = spd * (f32) cos((3.141592653589793 * (f64) ang) / 180.0);
        p->vel.y = 3.3333333f + (1.6666666f * (2.0f * MBEffRandF()));
        p->parManId = (s16) (18.0f + (0.2f * (60.0f * MBEffRandF())));
        p->color.a = (u8) (150.0f + (100.0f * MBEffRandF()));
        p->scale = 20.0f + (40.0f * MBEffRandF());
        break;
    }
}

/* NONMATCHING: same one-per-call spawn pattern. */
static void MB3_RollRockDustCreate(f32 *pos) {
    MBEFFECT *eff = (MBEFFECT *) Hu3DData[MB3W_S16(0x5E)].hookData;
    s32 i;

    for (i = 0; i < eff->num; i++) {
        MBEFFECTDATA *p = &eff->data[i];
        f32 ang, spd, r;

        if (p->time != 0) {
            continue;
        }
        p->time = (s16) (60.0f * (0.7f + (0.5f * MBEffRandF())));
        ang = 360.0f * MBEffSRandF();
        p->pos.x = pos[0] + (100.0f * MBEffSRandF());
        p->pos.y = pos[1] - 120.00001f;
        p->pos.z = pos[2] + (100.0f * MBEffSRandF());
        spd = 1.6666666f * (10.0f * MBEffRandF());
        p->vel.x = spd * (f32) sin((3.141592653589793 * (f64) ang) / 180.0);
        p->vel.z = spd * (f32) cos((3.141592653589793 * (f64) ang) / 180.0);
        p->vel.y = spd * (0.1f + (0.2f * MBEffRandF()));
        p->rot.z = 360.0f * MBEffRandF();
        p->scale = 90.0f + (100.0f * MBEffRandF());
        p->color.a = (u8) (100.0f + (80.0f * MBEffRandF()));
        r = MBEffRandF();
        p->color.r = (u8) (70.0f + (70.0f * r));
        p->color.g = (u8) (64.0f + (68.0f * r));
        p->color.b = (u8) (54.0f + (60.0f * r));
        break;
    }
}

/* Spawns the pickaxe/coin-dig sparkle burst; entry is always the single
 * (index-0) Turuhasi scratch slot. NONMATCHING: spawns up to 3 particles
 * per call instead of mirroring the fused loop condition. */
static void MB3_TuruhasiEffCreate(MB3_PLAYER_SCRATCH *entry) {
    MBEFFECT *eff = (MBEFFECT *) Hu3DData[entry->turuhasi.effModelId].hookData;
    s32 budget = 3;
    s32 i;

    for (i = 0; i < eff->num && budget > 0; i++) {
        MBEFFECTDATA *p = &eff->data[i];
        f32 ang, spd;

        if (p->time != 0) {
            continue;
        }
        p->time++;
        p->rot.z = 360.0f * MBEffRandF();
        (void) MBEffRandF();
        p->color.r = 0xD7;
        p->color.g = 0xFF;
        p->color.b = 0xCD;
        p->pos.x = (f32) (100.0 * sin((3.141592653589793 * (f64) entry->turuhasi.rot.y) / 180.0));
        p->pos.y = 0.0f;
        p->pos.z = (f32) (100.0 * cos((3.141592653589793 * (f64) entry->turuhasi.rot.y) / 180.0));
        ang = 360.0f * MBEffRandF();
        spd = 2.5f + (1.6666666f * (1.5f * MBEffRandF()));
        p->vel.x = spd * (f32) sin((3.141592653589793 * (f64) ang) / 180.0);
        p->vel.z = spd * (f32) cos((3.141592653589793 * (f64) ang) / 180.0);
        p->vel.y = 6.6666665f + (1.6666666f * (5.0f * MBEffRandF()));
        p->parManId = (s16) (30.0f + (0.3f * (60.0f * MBEffRandF())));
        p->color.a = (u8) (128.0f + (100.0f * MBEffRandF()));
        p->scale = 5.0f + (40.0f * MBEffRandF());
        budget--;
    }
}

/* Drives the four quicksand riders' per-frame state machine (see
 * MB3ASIBA_PLAYER above). NONMATCHING: same fallthrough-unwinding as
 * MB3_RollRockPathUpdate. */
static void MB3_AsibaPlayerUpdate(void) {
    s32 p;

    for (p = 0; p < 4; p++) {
        MB3ASIBA_PLAYER *e = &MB3_PLAYERSCR[p].asiba;
        Point3d target, outRot;

        if (e->active == 0) {
            continue;
        }
        switch (e->state) {
        case 0: {
            Point3d savedRot;
            MBPlayerPosGet(p, &e->pos);
            MBPlayerRotGet(p, &savedRot);
            e->motionShiftId = (s16) MBPlayerMotionCreate(p, 0x8C003F);
            e->motionRiseId = (s16) MBPlayerMotionCreate(p, 0x8C001D);
            e->taki = MB3W_S16(0x44 + p * 2);
            VECSubtract(&e->pos, &MBMasuGet(0, e->masuId)->pos, &target);
            target.y = 0.1f;
            if (VECMag(&target) < 30.000002f) {
                e->areaNo = 1;
            } else if (target.z == 0.0f) {
                e->areaNo = (target.x < 0.0f) ? 1 : 0;
            } else {
                e->areaNo = (target.x < 0.0f) ? 3 : 2;
            }
            VECSubtract(&e->pos, &MBMasuGet(0, e->masuId)->pos, &e->vel);
            e->timer = (f32) mb3ev_AsibaYureTimeTbl[e->areaNo];
            e->state++;
            break;
        }
        case 1: {
            MASU *masuP = MBMasuGet(0, e->masuId);
            MTXMultVec(masuP->matrix, &e->vel, &e->pos);
            e->timer -= 1.0f;
            if (e->timer == 0.0f) {
                VECAdd(&masuP->pos, (Point3d *) &mb3ev_AsibaOfsTbl[e->areaNo], &target);
                target.y = MB3_AsibaMinYGet(e->areaNo, &target);
                VECSubtract(&target, &e->pos, &target);
                target.x /= 0.6f;
                target.z /= 0.6f;
                target.y = (target.y / 0.6f) - (0.5f * 0.6f * -1960.0f);
                VECScale(&target, &e->vel, 0.016666668f);
                e->timer = 60.0f * 0.6f;
                e->gravity = 0.016666668f * (0.016666668f * -1960.0f);
                MBPlayerMotionNoShiftSet(p, e->motionShiftId, 0.0f, 8.0f, 0U);
                e->state++;
            }
            break;
        }
        case 2:
            e->sinkDepth += e->gravity;
            VECAdd(&e->pos, &e->vel, &e->pos);
            if (!((70.0f + e->tiltAngle) > MB3_AsibaMinYGet(e->areaNo, &e->pos))) {
                MBPlayerMotionNoShiftSet(p, e->motionRiseId, 0.0f, 8.0f, 0x40000001U);
                MB3_AsibaPlayerEffCreate((MB3_PLAYER_SCRATCH *) e, 0);
                MBAudFXPlay(0x5DD);
                e->state++;
            }
            break;
        case 3:
            e->vel.x *= 0.5f;
            e->vel.z *= 0.5f;
            e->sinkDepth += -2.0f * e->gravity;
            if (e->sinkDepth < 0.0f) {
                e->sinkDepth *= 0.6f;
            }
            e->vel.y *= 0.95f;
            VECAdd(&e->pos, &e->vel, &e->pos);
            if (!(e->sinkDepth < 0.0f) && !(e->tiltAngle < MB3_AsibaMinYGet(e->areaNo, &e->pos))) {
                e->vel.x = 0.0f;
                e->sinkDepth = 0.0f;
                e->state = (e->areaNo >= 6) ? 7 : e->state + 1;
            }
            break;
        case 4:
            e->vel.z += 0.2f * (6.6666665f - e->vel.z);
            VECAdd(&e->pos, &e->vel, &e->pos);
            e->tiltAngle = MB3_AsibaMinYGet(e->areaNo, &e->pos);
            e->yawAngle += 3.0000002f;
            e->vel.x *= 0.95f;
            MB3_AsibaPlayerEffCreate((MB3_PLAYER_SCRATCH *) e, 1);
            if (!(e->pos.z < mb3ev_AsibaPosTbl[e->areaNo + 1].z)) {
                MBPlayerMotionNoShiftSet(p, e->motionShiftId, 0.0f, 16.0f, 0U);
                e->state++;
            }
            break;
        case 5:
            e->vel.z *= 0.95f;
            e->sinkDepth += e->gravity;
            VECAdd(&e->pos, &e->vel, &e->pos);
            e->yawAngle += 3.0000002f;
            e->vel.x -= 1.0f;
            if (!(e->tiltAngle > mb3ev_AsibaPosTbl[e->areaNo + 2].y)) {
                e->state++;
            }
            break;
        case 6:
            e->vel.z *= 0.95f;
            e->sinkDepth += e->gravity;
            VECAdd(&e->pos, &e->vel, &e->pos);
            e->yawAngle += 3.0000002f;
            e->vel.x += 1.5000001f;
            if (!((70.0f + e->tiltAngle) > MB3_AsibaMinYGet(e->areaNo + 3, &e->pos))) {
                MBPlayerMotionNoShiftSet(p, e->motionRiseId, 0.0f, 8.0f, 0x40000001U);
                e->state = 2;
                e->areaNo += 3;
                MB3_AsibaPlayerEffCreate((MB3_PLAYER_SCRATCH *) e, 0);
                MBAudFXPlay(0x5DD);
            }
            break;
        case 7:
            e->vel.z += 0.2f * (1.6666666f - e->vel.z);
            VECAdd(&e->pos, &e->vel, &e->pos);
            e->tiltAngle = MB3_AsibaMinYGet(e->areaNo, &e->pos);
            e->vel.x *= 0.95f;
            e->active = 2;
            break;
        case -1:
            MBPlayerMotionNoShiftSet(p, 1, 0.0f, 0.0f, 0x40000001U);
            MBPlayerMotionKill(p, e->motionShiftId);
            MBPlayerMotionKill(p, e->motionRiseId);
            e->vel.y = 0.0f;
            e->yawAngle = 0.0f;
            e->vel.x = 0.0f;
            break;
        }
        if (e->state >= 0) {
            MBPlayerPosSetV(p, &e->pos);
        }
        {
            Mtx yawMtx, tiltMtx;
            MTXRotRad(yawMtx, 'Y', 0.017453292f * e->yawAngle);
            MTXRotRad(tiltMtx, 'X', 0.017453292f * e->vel.x);
            MTXConcat(tiltMtx, yawMtx, yawMtx);
            Hu3DMtxRotGet(yawMtx, &outRot);
            MBPlayerRotSetV(p, &outRot);
        }
    }
}

/* Spawns the quicksand splash particles for one player's sink/rise motion.
 * kind 0 = a small "plop" burst (budget 0x3C), kind 1 = a continuous
 * trailing droplet (budget 2). NONMATCHING: budget-limited loop instead of
 * the original's fused condition. */
static void MB3_AsibaPlayerEffCreate(MB3_PLAYER_SCRATCH *entry, s32 kind) {
    MB3ASIBA_PLAYER *e = &entry->asiba;
    MBEFFECT *eff = (MBEFFECT *) Hu3DData[e->taki].hookData;
    s32 budget = (kind == 0) ? 0x3C : 2;
    s32 i;

    for (i = 0; i < eff->num && budget > 0; i++) {
        MBEFFECTDATA *p = &eff->data[i];
        f32 r;

        if (p->time != 0) {
            continue;
        }
        p->time++;
        p->rot.z = 360.0f * MBEffRandF();
        r = MBEffRandF();
        p->color.r = (u8) (190.0f + (60.0f * r));
        p->color.g = (u8) (220.0f + (35.0f * r));
        p->color.b = (u8) (205.0f + (43.0f * r));

        if (kind == 0) {
            f32 ang = 360.0f * MBEffRandF();
            f32 dist = 50.0f + (100.0f * (0.3f * MBEffRandF()));
            p->vel.x = 0.0f;
            p->vel.z = 0.0f;
            p->vel.y = 6.6666665f + (1.6666666f * (4.0f * MBEffRandF()));
            p->parManId = (s16) (36.0f + (0.3f * (60.0f * MBEffRandF())));
            p->pos.x = e->pos.x + (dist * (f32) sin((3.141592653589793 * (f64) ang) / 180.0));
            p->pos.z = e->pos.z + (dist * (f32) cos((3.141592653589793 * (f64) ang) / 180.0));
            p->pos.y = MB3_AsibaMinYGet(e->areaNo, &p->pos) - (100.0f * MBEffRandF());
            p->color.a = (u8) (180.0f + (70.0f * MBEffRandF()));
            p->scale = 40.0f + (50.0f * MBEffRandF());
        } else {
            f32 ang2 = 180.0f + (45.0f * MBEffSRandF());
            p->vel.x = 1.6666666f * (0.3f * MBEffSRandF());
            p->vel.z = 1.6666666f * (2.0f * MBEffRandF());
            p->vel.y = 3.3333333f + (1.6666666f * (2.0f * MBEffRandF()));
            p->parManId = 0x12;
            p->pos.x = e->pos.x + (100.0f * (0.8f * (f32) sin((3.141592653589793 * (f64) ang2) / 180.0)));
            p->pos.z = 30.000002f + (e->pos.z + (100.0f * (0.3f * (f32) cos((3.141592653589793 * (f64) ang2) / 180.0))));
            p->pos.y = MB3_AsibaMinYGet(e->areaNo, &p->pos);
            p->color.a = (u8) (180.0f + (70.0f * MBEffRandF()));
            p->scale = 20.0f + (40.0f * MBEffRandF());
        }
        budget--;
    }
}

static int MB3Ev_MasuHatena(int playerNo) {
    u32 flag = MBMasuGet(0, GwPlayer[playerNo].masuId)->flag;

    if (flag & 0x800) {
        MB3Ev_Turuhasi(playerNo);
    }
    if ((flag & 0x1018) != 0 && MB3Ev_BitGet((u16) flag, 0x1018) == 1) {
        MB3Ev_RollRock(playerNo);
    }
    if (flag & 0x2000) {
        MB3Ev_YYHou(playerNo);
    }
    if (flag & 0x4000) {
        MB3Ev_Asiba(playerNo);
    }
    return 0;
}

static int MB3Ev_MasuWalkPost(int playerNo) {
    MASU *masuP = MBMasuGet(0, GwPlayer[playerNo].masuId);
    u32 flag = masuP->flag;

    if ((flag & 0x80) != 0 && mb3_Work[0x1D] == 0) {
        s32 modelId = mb3_Work[3];
        if (modelId >= 0) {
            MBMotionTimeSet((s16) modelId, 0.0f);
            MBMotionSpeedSet((s16) modelId, 1.0f);
        }
        mb3_Work[0x1D] = 1;
        MBAudFXPlay(0x5E3);
    }
    if (flag & 0x8020) {
        MB3Ev_KubiBone(masuP, MB3Ev_BitGet((u16) flag, 0x8020));
    }
    return 0;
}

/* KubiBone: the "bone platform" wobbles and spits ten debris chunks up in
 * a cone around the masu when a player walks past it. idx selects which of
 * the four cone-angle presets (mb3ev_KubiBoneAngleTbl) and which debris
 * "burst" the platform this is: mb3_Work[idx].unk78 (word 0x1E+idx*6, same
 * countdown region MB3_MainObjUpdate ticks down) gates re-triggering. */
static void MB3Ev_KubiBone(MASU *masuP, s32 idx) {
    Point3d basePos;
    f32 coneAngle;
    s32 remaining;
    s32 i;
    MB3DEBRIS *d;

    if (mb3_Work[0x1E + idx * 6] != 0) {
        return;
    }
    mb3_Work[0x1E + idx * 6] = 0xB4;
    basePos = masuP->pos;
    basePos.y += 300.0f;
    coneAngle = mb3ev_KubiBoneAngleTbl[idx];
    remaining = 0xA;
    d = MB3_DEBRIS;
    for (i = 0; i < MB3_BREAKABLE_MAX; i++, d++) {
        Mtx coneMtx, wobbleMtx;
        Point3d rot;
        f32 angle, speed;

        if (d->count != 0) {
            continue;
        }
        d->count = 0xB4;
        angle = coneAngle + (20.0f * MBEffSRandF());
        speed = 6.6666665f + (1.6666666f * (3.0f * MBEffRandF()));
        d->vel.x = speed * (f32) sin((3.141592653589793 * (f64) angle) / 180.0);
        d->vel.z = speed * (f32) cos((3.141592653589793 * (f64) angle) / 180.0);
        d->vel.y = (1.6666666f * (-2.0f * MBEffRandF())) - 1.6666666f;
        MTXRotRad(coneMtx, 'Y', 0.017453292f * angle);
        MTXRotRad(wobbleMtx, 'Z', 0.017453292f * (45.0f * MBEffSRandF()));
        MTXConcat(wobbleMtx, coneMtx, coneMtx);
        Hu3DMtxRotGet(coneMtx, &rot);
        MBModelDispSet(d->modelId, TRUE);
        MBModelPosSet(d->modelId,
            basePos.x + (30.0f * MBEffSRandF()),
            basePos.y - (30.0f * MBEffRandF()),
            basePos.z + (30.0f * MBEffSRandF()));
        MBModelRotSetV(d->modelId, &rot);
        MBMotionTimeSet(d->modelId, 6.0f * MBEffRandF());
        d->seId1 = -1;
        d->seId0 = -1;
        if (i == 0) {
            d->seId0 = HuAudFXEmiterPlay(0x5DE, &basePos);
            d->seId1 = HuAudFXEmiterPlay(0x5E4, &basePos);
        }
        remaining -= 1;
        if (remaining <= 0) {
            break;
        }
    }
}

/* Fireball catapult event: the player is launched, does a spin, crashes
 * down on a random hatena masu and (if another player is standing there)
 * knocks coins off them. NONMATCHING: HuPrcVSleep() wait-loops rewritten
 * as plain for/while loops; PI literal written out; the two float
 * arguments to MB3_ImpactEffPosSet at the crash point could not be
 * resolved with confidence from the m2c draft (see decomp report) and are
 * best-effort (60.0f pitch, 200.0f yaw). */
static void MB3Ev_YYHou(s32 playerNo) {
    MASU *masuP = MBMasuGet(0, GwPlayer[playerNo].masuId);
    s16 fireball;
    s32 rescued[4] = { -1, -1, -1, -1 };
    s16 mainMdl = (s16) mb3_Work[1];
    Point3d pos, vel, hookPos, dir, impactPos;
    f32 gravity = -4900.0f * 0.0002777778f;
    f32 scale;
    s32 i, t;
    s32 targetMasu, rescueCount;
    MASU *landMasu;
    Point3d fallVel;
    s32 frame;

    MBPlayerMotIdleSet(playerNo);
    fireball = (s16) MBModelCreate(0x8007B, NULL, FALSE);
    MBModelLayerSet(fireball, 6U);
    MBModelDispSet(fireball, FALSE);

    MB3W_S16(0x5C) = (s16) MBEffCreate((ANIMDATA *)(uintptr_t) mb3_Work[0x10], 0x20);
    MBEffAttrSet(MB3W_S16(0x5C), 8U);
    MBEffHookSet(MB3W_S16(0x5C), MB3_ImpactEffHook);
    Hu3DModelLayerSet(MB3W_S16(0x5C), 3);

    MBPlayerMotionNoShiftSet(playerNo, 4, 0.0f, 8.0f, 0U);
    MBPlayerPosGet(playerNo, &pos);
    Hu3DModelObjPosGet(MBModelIdGet(mainMdl), "yy_hou2-thook", &hookPos);
    hookPos.y += 200.0f;
    VECSubtract(&hookPos, &pos, &vel);
    vel.x /= 0.6f;
    vel.z /= 0.6f;
    vel.y = (vel.y / 0.6f) - (0.5f * 0.6f * -4900.0f);
    VECScale(&vel, &vel, 0.016666668f);
    MBPlayerRotSet(playerNo, 0.0f, (f32) (180.0 * (atan2((f64) vel.x, (f64) vel.z) / 3.141592653589793)), 0.0f);

    for (i = 0; i < 3; i++) {
        HuPrcVSleep();
    }
    for (i = 0; i < (s32) (0.6f * 60.0f); i++) {
        vel.y += gravity;
        VECAdd(&pos, &vel, &pos);
        MBPlayerPosSetV(playerNo, &pos);
        HuPrcVSleep();
    }
    MBPlayerMotionNoShiftSet(playerNo, 5, 0.0f, 0.0f, 0U);
    scale = 1.0f;
    for (i = 0; i < 30; i++) {
        pos.y += -200.0f / 30.0f;
        scale *= 0.95f;
        MBPlayerPosSetV(playerNo, &pos);
        MBPlayerScaleSet(playerNo, scale, scale, scale);
        HuPrcVSleep();
    }

    MBCameraFocusSet(-1);
    MBMotionSpeedSet(mainMdl, 1.0f);
    MBMotionTimeSet(mainMdl, 0.0f);
    MBAudFXPlay(0x5DF);
    for (t = 0; (f32) t < MBMotionMaxTimeGet(mainMdl) - 10.0f; t++) {
        HuPrcVSleep();
    }
    MBPlayerMotionNoShiftSet(playerNo, 1, 0.0f, 0.0f, 0x40000001U);
    MBPlayerPosGet(playerNo, &pos);
    MBPlayerWinLoseVoicePlay(playerNo, 1, 0x1F9);
    scale = 0.0f;
    dir.x = (f32) (sin(3.6651914291880923) * cos(1.0471975511965976));
    dir.y = (f32) sin(1.0471975511965976);
    dir.z = (f32) (cos(3.6651914291880923) * cos(1.0471975511965976));
    VECScale(&dir, &vel, 200.0f);
    VECAdd(&pos, &vel, &impactPos);
    MB3_ImpactEffPosSet(&impactPos, 60.0f, 200.0f);
    MBAudFXPlay(0x5E0);
    omVibrate((s16) playerNo, 0xC, 4, 2);
    VECScale(&dir, &dir, 33.333332f);
    MBCameraQuakeSet(0x12, 500.0f);
    {
        f32 spinAngle = 0.0f;
        for (i = 0; (f32) i < 30.0f; i++) {
            Mtx a, b;
            Point3d outRot;

            spinAngle += 18.0f;
            MTXRotRad(a, 'Y', 0.017453292f * spinAngle);
            MTXRotRad(b, 'X', 0.5235988f);
            MTXConcat(b, a, a);
            MTXRotRad(b, 'Y', 3.6651914f);
            MTXConcat(b, a, a);
            Hu3DMtxRotGet(a, &outRot);
            MBPlayerRotSetV(playerNo, &outRot);
            VECAdd(&pos, &dir, &pos);
            MBPlayerPosSetV(playerNo, &pos);
            scale += 0.3f * (1.0f - scale);
            MBPlayerScaleSet(playerNo, scale, scale, scale);
            HuPrcVSleep();
        }
    }
    MBPlayerScaleSet(playerNo, 1.0f, 1.0f, 1.0f);
    {
        f32 r = MBEffRandF();
        targetMasu = MBMasuFlagFind(0, MB3Ev_MasuFlagGet((s32) (3.0f * r) + 1, 0x300), 0x300U);
    }
    rescueCount = 0;
    for (i = 0; i < 4; i++) {
        if (targetMasu == GwPlayer[i].masuId) {
            rescued[i] = i;
            rescueCount++;
        }
    }
    WipeColorSet(0, 0, 0);
    WipeCreate(2, 0x81, 1);
    while (!WipeCheckEnd()) {
        HuPrcVSleep();
    }
    MBEffKill(MB3W_S16(0x5C));
    MB3W_S16(0x5E) = -1;
    MB3W_S16(0x5C) = -1;
    MBPlayerPosFixWarp(playerNo, targetMasu, TRUE);
    MBCameraFocusMasuSet(targetMasu);
    MBCameraSkipSet(0);
    WipeCreate(1, 0x81, 0x1E);
    while (!WipeCheckEnd()) {
        HuPrcVSleep();
    }

    landMasu = MBMasuGet(0, targetMasu);
    pos = landMasu->pos;
    pos.y += 1000.0f;
    fallVel.x = 0.0f;
    fallVel.y = -5.0f;
    fallVel.z = 0.0f;
    MBPlayerRotSet(playerNo, 0.0f, 0.0f, 0.0f);
    MBPlayerScaleSet(playerNo, 1.0f, 1.0f, 1.0f);
    MBPlayerMotionNoShiftSet(playerNo, 4, 0.0f, 0.0f, 0U);
    frame = 0;
    while (pos.y > landMasu->pos.y) {
        if (((pos.y - landMasu->pos.y) < 200.0f) && MBPlayerMotionNoGet(playerNo) != 5) {
            MBPlayerMotionNoShiftSet(playerNo, 5, 0.0f, 0.0f, 0U);
            frame = 0;
            if (rescueCount != 0) {
                MBModelDispSet(fireball, TRUE);
                MBMotionTimeSet(fireball, 0.0f);
            }
        }
        MBPlayerPosSetV(playerNo, &pos);
        MB3_ImpactEffUpdate(fireball, frame);
        frame++;
        HuPrcVSleep();
        VECAdd(&pos, &fallVel, &pos);
        fallVel.y += gravity;
    }
    GwPlayer[playerNo].masuId = (s16) targetMasu;
    do {
        MB3_ImpactEffUpdate(fireball, frame);
        frame++;
        HuPrcVSleep();
    } while (!MBPlayerMotionEndCheck(playerNo));
    MBPlayerMotIdleSet(playerNo);
    for (i = 0; i < 0x1E; i++) {
        frame++;
        MB3_ImpactEffUpdate(fireball, frame);
        HuPrcVSleep();
    }
    if (rescueCount != 0) {
        MBModelDispSet(fireball, FALSE);
        for (i = 0; i < 4; i++) {
            if (rescued[i] >= 0) {
                MBPlayerMotionNoSet(i, 6, 0x40000001U);
            }
        }
        for (i = 0; i < 4; i++) {
            if (rescued[i] >= 0) {
                MBCoinAddDispExec(i, -5, TRUE);
            }
        }
    }
    WipeColorSet(0, 0, 0);
    WipeCreate(2, 0x81, 1);
    while (!WipeCheckEnd()) {
        HuPrcVSleep();
    }
    for (i = 0; i < 4; i++) {
        if (rescued[i] >= 0) {
            MBPlayerMotionNoSet(i, 1, 0x40000001U);
        }
    }
    MBMotionSpeedSet(mainMdl, 0.0f);
    MBMotionTimeSet(mainMdl, 0.0f);
    WipeCreate(1, 0x81, 0x1E);
    while (!WipeCheckEnd()) {
        HuPrcVSleep();
    }
    MBModelKill(fireball);
}

/* Boulder-riding event: finds the longest run of linked "rock track" masus,
 * puts the boulder in motion and lets every player standing on the track
 * ride it until it stops (or a player is thrown off by the pickaxe hazard
 * near the end). NONMATCHING: HuPrcVSleep() loops rewritten, minor state
 * machine restructuring. */
static void MB3Ev_RollRock(s32 playerNo) {
    s32 i, j, k;
    s32 bestMasu = 0, best = 0;
    s32 masuNum;
    s16 rockModel;
    f32 savedZoom;
    Point3d rockPos;
    s32 state = 0;
    s32 sndId = -1;
    s32 timer = 0;
    s32 allDone;

    MBMasuGet(0, GwPlayer[playerNo].masuId);
    MBPlayerMotIdleSet(playerNo);
    memset(&mb3_Work[0x9A], 0, 0x190U);

    for (i = 0; i < 4; i++) {
        MASU *cur = MBMasuGet(0, GwPlayer[i].masuId);
        for (j = 0; j < (s32) cur->linkNum; j++) {
            u16 linkId = cur->linkTbl[j];
            if (MBMasuGet(0, linkId)->flag & 0x40) {
                MB3ROLLROCK_PLAYER *e = &MB3_PLAYERSCR[i].rollRock;
                e->active = 1;
                e->masuId = linkId;
                break;
            }
        }
    }
    mb3_Work[0x1C] = 0;
    masuNum = MBMasuNumGet(0);
    for (i = 1; i <= masuNum; i++) {
        s32 chainLen = 0;
        MASU *cur = MBMasuGet(0, i);
        while ((cur->flag & 0x40) && cur->linkNum != 0) {
            MASU *next = NULL;
            for (k = 0; k < (s32) cur->linkNum; k++) {
                MASU *cand = MBMasuGet(0, cur->linkTbl[k]);
                if (cand->flag & 0x40) {
                    next = cand;
                    break;
                }
            }
            if (next == NULL) {
                break;
            }
            cur = next;
            chainLen++;
        }
        if (best < chainLen) {
            bestMasu = i;
            best = chainLen;
        }
    }
    mb3_Work[0xA] = bestMasu;
    MB3_RollRockPathUpdate();
    rockModel = (s16) mb3_Work[7];
    MBMotionSpeedSet(rockModel, 0.0f);
    MBMotionTimeSet(rockModel, 0.0f);
    WipeColorSet(0, 0, 0);
    WipeCreate(2, 0x81, 1);
    while (!WipeCheckEnd()) {
        HuPrcVSleep();
    }
    savedZoom = MBCameraZoomGet();
    MB3W_S16(0x5C) = (s16) MBEffCreate((ANIMDATA *)(uintptr_t) mb3_Work[0xF], 0x20);
    MBEffAttrSet(MB3W_S16(0x5C), 8U);
    MBEffHookSet(MB3W_S16(0x5C), MB3_TuruhasiEffHook);
    Hu3DModelLayerSet(MB3W_S16(0x5C), 3);
    MB3W_S16(0x5E) = (s16) MBEffCreate((ANIMDATA *)(uintptr_t) mb3_Work[0x10], 0x40);
    MBEffAttrSet(MB3W_S16(0x5E), 8U);
    MBEffHookSet(MB3W_S16(0x5E), MB3_RollRockEffHook);
    Hu3DModelLayerSet(MB3W_S16(0x5E), 3);
    MBCameraFocusSet(-1);
    Hu3DModelObjPosGet(MBModelIdGet(rockModel), "trap-iwa1", &rockPos);
    MBCameraPosTargetSetV(&rockPos);
    MBCameraZoomSet(3000.0f);
    MBCameraSkipSet(0);
    MBStarDispSetAll(FALSE);
    WipeCreate(1, 0x81, 0x1E);
    MBCameraQuakeSet(0x3C, 50.0f);
    while (!WipeCheckEnd()) {
        MBCameraPosTargetSetV(&rockPos);
        HuPrcVSleep();
    }
    MBMotionSpeedSet(rockModel, 1.2f);
    do {
        MASU *curMasu = MBMasuGet(0, mb3_Work[0xA]);
        Point3d delta;

        switch (state) {
        case 0:
            if (MB3Ev_BitGet((s32) curMasu->flag, 0x1018) == 2) {
                state++;
            }
            break;
        case 1:
            WipeColorSet(0, 0, 0);
            WipeCreate(2, 0, 0x1E);
            state++;
            break;
        case 2:
            if (!WipeCheckEnd()) {
                MBAudFXPlay(0x5E5);
                MBMotionSpeedSet(rockModel, 6.0f);
                timer = 0x19;
                state++;
            }
            break;
        case 3:
            timer -= 1;
            if (timer < 0) {
                MBMotionSpeedSet(rockModel, 1.2f);
                WipeCreate(1, 0, 0x1E);
                MB3Ev_KubiBone(MBMasuGet(0, MBMasuFlagFind(0, MB3Ev_MasuFlagGet(2, 0x8020), 0x8020U)), 2);
                state++;
            }
            break;
        }
        Hu3DModelObjPosGet(MBModelIdGet(rockModel), "trap-iwa1", &rockPos);
        MB3W_F32(0x64) = rockPos.x;
        MB3W_F32(0x68) = rockPos.y;
        MB3W_F32(0x6C) = rockPos.z;
        MBCameraPosTargetSetV(&rockPos);
        VECSubtract(&rockPos, &curMasu->pos, &delta);
        delta.y = 0.1f;
        if (VECMag(&delta) < 240.00002f) {
            for (k = 0; k < (s32) curMasu->linkNum; k++) {
                if (MBMasuGet(0, curMasu->linkTbl[k])->flag & 0x40) {
                    mb3_Work[0xA] = curMasu->linkTbl[k];
                    break;
                }
            }
        }
        if (MBMotionTimeGet(rockModel) > 10.0f && MBMotionTimeGet(rockModel) < 800.0f) {
            MB3_RollRockEffUpdate(&MB3W_F32(0x64));
            MB3_RollRockDustCreate(&MB3W_F32(0x64));
        }
        if (MBMotionTimeGet(rockModel) > 10.0f) {
            if (MBMotionTimeGet(rockModel) < 820.0f) {
                if (sndId < 0) {
                    sndId = HuAudFXPlay(0x5E2);
                }
            } else if (sndId >= 0) {
                HuAudFXStop(sndId);
                sndId = -1;
                OSReport("GOROGORO Stop!\n");
            }
        }
        MB3_RollRockPathUpdate();
        HuPrcVSleep();
        allDone = 1;
        for (i = 0; i < 4; i++) {
            if (MB3_PLAYERSCR[i].rollRock.active == 1) {
                allDone = 0;
            }
        }
    } while (!allDone);
    for (i = 0; i < 0x3C; i++) {
        MB3_RollRockPathUpdate();
        HuPrcVSleep();
    }
    WipeColorSet(0, 0, 0);
    WipeCreate(2, 0x81, 1);
    while (!WipeCheckEnd()) {
        HuPrcVSleep();
    }
    MBStarDispSetAll(TRUE);
    MBEffKill(MB3W_S16(0x5C));
    MBEffKill(MB3W_S16(0x5E));
    MB3W_S16(0x5E) = -1;
    MB3W_S16(0x5C) = -1;
    for (i = 0; i < 4; i++) {
        MB3_PLAYERSCR[i].rollRock.state = -1;
    }
    MB3_RollRockPathUpdate();
    {
        s32 finalMasu = GwPlayer[playerNo].masuId;
        MBPlayerPosFixCurr(finalMasu, TRUE);
        MBMotionSpeedSet(rockModel, 0.0f);
        MBMotionTimeSet(rockModel, 0.0f);
        MBCameraFocusSet(-1);
        MBCameraZoomSet(savedZoom);
        MBCameraFocusMasuSet(finalMasu);
    }
    MBCameraSkipSet(0);
    WipeCreate(1, 0x81, 0x1E);
    while (!WipeCheckEnd()) {
        HuPrcVSleep();
    }
    for (i = 0; i < 0x3C; i++) {
        HuPrcVSleep();
    }
}

/* Pickaxe/coin-dig event: either a cheap "lose 5 coins" animation (below
 * 5 coins) or the full dig minigame (button-mash rhythm game, rewarding a
 * random coin count via MB3_CoinEffCreate). NONMATCHING: HuPrcVSleep()
 * loops rewritten, button-mash inner loop simplified from the fused
 * fallthrough condition seen in the m2c draft. */
static void MB3Ev_Turuhasi(s32 playerNo) {
    s16 turuhasiMdl = GwPlayer[playerNo].masuId;
    MASU *destMasuP = MBMasuGet(0, turuhasiMdl);
    MASU *startMasuP = MBMasuGet(0, mb3_Work[9]);
    s16 hutaMdl = (s16) mb3_Work[2];
    Point3d pos;
    s32 i;

    MBPlayerMotIdleSet(playerNo);

    if (MBPlayerCoinGet(playerNo) < 5) {
        pos = startMasuP->pos;
        pos.y -= 100.0f;
        MBMotionNoSet(hutaMdl, (s32) MB3W_S16(0x60), 0U);
        MBMotionTimeSet(hutaMdl, 0.0f);
        MBMotionSpeedSet(hutaMdl, 1.0f);
        MBModelPosSetV(hutaMdl, &pos);
        MBModelDispSet(hutaMdl, TRUE);
        MBAudFXPlay(0x5E7);
        HuPrcVSleep();
        for (i = 0; i < 0x14; i++) {
            f32 t = 1.0f - (0.05263158f * (f32) i);
            MBModelPosSet(hutaMdl, pos.x, pos.y + (100.0f * (1.0f - (t * t))), pos.z);
            HuPrcVSleep();
        }
        MBMotionNoSet(hutaMdl, (s32) MB3W_S16(0x62), 0x40000001U);
        MBWinCreate(2, 0x2F0002U, 0x13);
        MBTopWinWait();
        MBTopWinKill();
        MBMotionNoSet(hutaMdl, (s32) MB3W_S16(0x60), 0U);
        MBMotionTimeSet(hutaMdl, MBMotionMaxTimeGet(hutaMdl));
        MBMotionSpeedSet(hutaMdl, -2.0f);
        for (i = 0; i < 0xA; i++) {
            f32 t = 0.11111111f * (f32) i;
            MBModelPosSet(hutaMdl, pos.x, pos.y + (100.0f * (1.0f - (t * t))), pos.z);
            HuPrcVSleep();
        }
        MBModelDispSet(hutaMdl, FALSE);
        return;
    }

    WipeColorSet(0, 0, 0);
    WipeCreate(2, 0x81, 1);
    while (!WipeCheckEnd()) {
        HuPrcVSleep();
    }
    for (i = 0; i < 4; i++) {
        if (i != playerNo) {
            MBModelDispSet(MBPlayerModelGet(i), FALSE);
        }
    }
    MBPlayerRotSet(playerNo, 0.0f, (f32) (180.0 * (atan2((f64) (startMasuP->pos.x - destMasuP->pos.x), (f64) (startMasuP->pos.z - destMasuP->pos.z)) / 3.141592653589793)), 0.0f);
    MBCameraFocusMasuSet(mb3_Work[9]);
    {
        f32 savedZoom = MBCameraZoomGet();
        s32 rounds = 0, mashed = 0, releasing = 1, resetDelay = 0, rhythmDown = 0;
        s32 winCoinId = -1;
        Point3d camRot;

        MBCameraZoomSet(2000.0f);
        MBCameraSkipSet(0);
        WipeCreate(1, 0x81, 0x1E);
        while (!WipeCheckEnd()) {
            HuPrcVSleep();
        }

        pos = startMasuP->pos;
        pos.y -= 100.0f;
        MBMotionNoSet(hutaMdl, (s32) MB3W_S16(0x60), 0U);
        MBMotionTimeSet(hutaMdl, 0.0f);
        MBMotionSpeedSet(hutaMdl, 1.0f);
        MBModelPosSetV(hutaMdl, &pos);
        MBModelDispSet(hutaMdl, TRUE);
        MBAudFXPlay(0x5E7);
        HuPrcVSleep();
        for (i = 0; i < 0x14; i++) {
            f32 t = 1.0f - (0.05263158f * (f32) i);
            MBModelPosSet(hutaMdl, pos.x, pos.y + (100.0f * (1.0f - (t * t))), pos.z);
            HuPrcVSleep();
        }
        MBMotionNoSet(hutaMdl, (s32) MB3W_S16(0x62), 0x40000001U);
        MBWinCreateChoice(0, 0x2F0000U, 0x13, 0);
        if (GwPlayer[playerNo].comF) {
            MBComChoiceSetUp();
        }
        MBTopWinWait();
        {
            s32 giveUp = (MBWinLastChoiceGet() == 0);
            MBTopWinKill();
            if (giveUp) {
                MB3_PLAYER_SCRATCH *turu0 = MB3_PLAYERSCR;
                s8 charHook;

                memset(&mb3_Work[0x9A], 0, 0x190U);
                MBCoinAddDispExec(playerNo, -5, TRUE);
                WipeColorSet(0, 0, 0);
                WipeCreate(2, 0x81, 1);
                while (!WipeCheckEnd()) {
                    HuPrcVSleep();
                }
                for (i = 0; i < 4; i++) {
                    if (i != playerNo) {
                        MBModelDispSet(MBPlayerModelGet(i), FALSE);
                    }
                }
                MBPlayerRotSet(playerNo, 0.0f, (f32) (180.0 * (atan2((f64) (startMasuP->pos.x - destMasuP->pos.x), (f64) (startMasuP->pos.z - destMasuP->pos.z)) / 3.141592653589793)), 0.0f);
                pos.x = -826.0f;
                pos.y = 4162.0f;
                pos.z = -2260.0f;
                MBMotionNoSet(hutaMdl, (s32) MB3W_S16(0x60), 0U);
                MBMotionTimeSet(hutaMdl, 0.0f);
                MBMotionSpeedSet(hutaMdl, 0.0f);
                MBModelPosSetV(hutaMdl, &pos);
                MBPlayerPosSet(playerNo, 200.0f + destMasuP->pos.x, destMasuP->pos.y, 600.0f + destMasuP->pos.z);
                MBPlayerRotSet(playerNo, 0.0f, 0.0f, 0.0f);
                MBPlayerPosGet(playerNo, &turu0->turuhasi.pos);
                MBPlayerRotGet(playerNo, &turu0->turuhasi.rot);
                turu0->turuhasi.motionA = (s16) MBPlayerMotionCreate(playerNo, 0x8C0044);
                turu0->turuhasi.motionB = (s16) MBPlayerMotionCreate(playerNo, 0x8C0045);
                MBPlayerMotionNoShiftSet(playerNo, turu0->turuhasi.motionA, 0.0f, 0.0f, 0x40000001U);
                MBModelRotSet((s16) mb3_Work[8], 90.0f, 0.0f, 0.0f);
                MBModelPosSet((s16) mb3_Work[8], 0.0f, 0.0f, 10.0f);
                MBModelDispSet((s16) mb3_Work[8], TRUE);
                charHook = GwPlayer[playerNo].charNo;
                MBModelHookSet(MBPlayerModelGet(playerNo), CharModelItemHookGet(charHook, 2, 0), (s16) mb3_Work[8]);
                turu0->turuhasi.effModelId = (s16) MBEffCreate((ANIMDATA *)(uintptr_t) mb3_Work[0xF], 0x40);
                MBEffAttrSet(turu0->turuhasi.effModelId, 8U);
                MBEffHookSet(turu0->turuhasi.effModelId, MB3_TuruhasiEffHook);
                Hu3DModelLayerSet(turu0->turuhasi.effModelId, 3);
                Hu3DModelPosSetV(turu0->turuhasi.effModelId, &turu0->turuhasi.pos);
                MB3_CoinEffInit();
                MBCameraFocusSet(-1);
                MBCameraPosTargetSet(150.0f + destMasuP->pos.x, destMasuP->pos.y, 500.0f + destMasuP->pos.z);
                MBCameraZoomSet(2500.0f);
                MBCameraRotGet(&camRot);
                MBCameraRotSet(camRot.x, -20.0f, camRot.z);
                MBCameraSkipSet(0);
                WipeCreate(1, 0x81, 0x1E);
                while (!WipeCheckEnd()) {
                    HuPrcVSleep();
                }
                MBMotionSpeedSet(hutaMdl, 1.0f);
                MBAudFXPlay(0x5E7);
                for (i = 0; i < 0x14; i++) {
                    f32 t = 1.0f - (0.05263158f * (f32) i);
                    MBModelPosSet(hutaMdl, pos.x, pos.y + (100.0f * (1.0f - (t * t))), pos.z);
                    HuPrcVSleep();
                }
                winCoinId = GameMesCreate(1, 5, -1, -1);
                MBMotionNoSet(hutaMdl, (s32) turu0->turuhasi.motionB, 0x40000001U);
                GameMesDispSet(winCoinId, 1, 5);
                MBWinCreate(2, 0x2F0001U, 0x13);
                MBTopWinWait();
                MBTopWinKill();
                MBPlayerMotionNoSet(playerNo, turu0->turuhasi.motionB, 0U);
                MBPlayerMotionTimeSet(playerNo, 0.0f);
                MBPlayerMotionSpeedSet(playerNo, 0.0f);

                for (i = 0; i < 0x168; i++) {
                    u16 keys = HuPadBtnDown[(u8) GwPlayer[playerNo].padNo];
                    if (GwPlayer[playerNo].comF && (i & 3) == 0 && MBEffRandF() < 0.6f) {
                        keys |= 0x100;
                    }
                    if (i >= 0x12C) {
                        keys = 0;
                    }
                    if (keys & 0x100) {
                        mashed = 1;
                        mb3_Work[0xA0] += 1;
                    }
                    if (mb3_Work[0xA0] >= 3) {
                        mb3_Work[0xA0] = 0;
                        rounds++;
                        resetDelay = 1;
                    }
                    if (mashed) {
                        if (releasing == 0) {
                            MBPlayerMotionTimeSet(playerNo, 0.0f);
                            MBPlayerMotionSpeedSet(playerNo, 2.0f);
                            releasing = 1;
                        } else {
                            MBPlayerMotionTimeSet(playerNo, 5.0f);
                            MBPlayerMotionSpeedSet(playerNo, 1.0f);
                            mashed = 0;
                            releasing = 0;
                        }
                    }
                    if (MBPlayerMotionEndCheck(playerNo)) {
                        MBPlayerMotionTimeSet(playerNo, 0.0f);
                        MBPlayerMotionSpeedSet(playerNo, 0.0f);
                        releasing = 1;
                    }
                    if (rhythmDown == 0) {
                        if (MBPlayerMotionTimeGet(playerNo) == 5.0f) {
                            rhythmDown = 1;
                            MBAudFXPlay(0x5E1);
                            MB3_TuruhasiEffCreate(turu0);
                            if (resetDelay) {
                                f32 coinPos[3];
                                coinPos[0] = MB3W_F32(0x29C);
                                coinPos[1] = MB3W_F32(0x2A0);
                                coinPos[2] = MB3W_F32(0x2A4) + 100.0f;
                                MB3_CoinEffCreate(coinPos);
                                resetDelay = 0;
                            }
                        }
                    } else if (releasing) {
                        rhythmDown = 0;
                    }
                    MB3_CoinEffUpdate();
                    if (winCoinId >= 0) {
                        s32 remain = 0x12B - i;
                        if (remain >= 0) {
                            GameMesDispSet(winCoinId, 1, (s16) (remain / 60));
                        } else {
                            GameMesDispSet(winCoinId, 2, -1);
                            winCoinId = -1;
                        }
                    }
                    HuPrcVSleep();
                }
                MBCoinAddDispExec(playerNo, rounds, TRUE);
                MBMotionNoSet(hutaMdl, (s32) MB3W_S16(0x60), 0U);
                MBMotionTimeSet(hutaMdl, MBMotionMaxTimeGet(hutaMdl));
                MBMotionSpeedSet(hutaMdl, -2.0f);
                for (i = 0; i < 0xA; i++) {
                    f32 t = 0.11111111f * (f32) i;
                    MBModelPosSet(hutaMdl, pos.x, pos.y + (100.0f * (1.0f - (t * t))), pos.z);
                    HuPrcVSleep();
                }
                WipeColorSet(0, 0, 0);
                WipeCreate(2, 0x81, 1);
                while (!WipeCheckEnd()) {
                    HuPrcVSleep();
                }
                MBCameraRotSetV(&camRot);
                MBModelHookReset(MBPlayerModelGet(playerNo));
                MBModelDispSet((s16) mb3_Work[8], FALSE);
                MBPlayerMotionKill(playerNo, turu0->turuhasi.motionA);
                MBPlayerMotionKill(playerNo, turu0->turuhasi.motionB);
                MBPlayerMotionNoShiftSet(playerNo, 1, 0.0f, 0.0f, 0x40000001U);
                MB3_CoinEffClose();
                MBEffKill(turu0->turuhasi.effModelId);
            } else {
                MBMotionNoSet(hutaMdl, (s32) MB3W_S16(0x60), 0U);
                MBMotionTimeSet(hutaMdl, MBMotionMaxTimeGet(hutaMdl));
                MBMotionSpeedSet(hutaMdl, -2.0f);
                for (i = 0; i < 0xA; i++) {
                    f32 t = 0.11111111f * (f32) i;
                    MBModelPosSet(hutaMdl, pos.x, pos.y + (100.0f * (1.0f - (t * t))), pos.z);
                    HuPrcVSleep();
                }
                WipeColorSet(0, 0, 0);
                WipeCreate(2, 0x81, 1);
                while (!WipeCheckEnd()) {
                    HuPrcVSleep();
                }
            }
        }
        MBModelDispSet(hutaMdl, FALSE);
        for (i = 0; i < 4; i++) {
            MBModelDispSet(MBPlayerModelGet(i), TRUE);
        }
        MBPlayerPosReset(playerNo);
        MBPlayerRotSet(playerNo, 0.0f, 0.0f, 0.0f);
        MBPlayerMotionNoShiftSet(playerNo, 1, 0.0f, 0.0f, 0x40000001U);
        MBModelPosSetV((s16) mb3_Work[2], &startMasuP->pos);
        MBCameraFocusMasuSet(turuhasiMdl);
        MBCameraZoomSet(savedZoom);
        MBCameraSkipSet(0);
        WipeCreate(1, 0x81, 0x1E);
        while (!WipeCheckEnd()) {
            HuPrcVSleep();
        }
    }
}

/* Quicksand event: the player (and anyone else on the same masu) sinks
 * into and gets carried through the swamp, driven each frame by
 * MB3_AsibaPlayerUpdate. NONMATCHING: HuPrcVSleep() loops rewritten. */
static void MB3Ev_Asiba(s32 playerNo) {
    s16 masuId = GwPlayer[playerNo].masuId;
    MASU *masuP = MBMasuGet(0, masuId);
    s32 highSide = 0;
    s32 takiModel = mb3_Work[6];
    s32 i, targetMasu;
    s32 anyActive;

    if (masuP->pos.y > 2000.0f) {
        highSide = 1;
        takiModel = mb3_Work[5];
    }
    MBMotionSpeedSet((s16) takiModel, 0.0f);
    MBMotionTimeSet((s16) takiModel, 0.0f);
    memset(&mb3_Work[0x9A], 0, 0x190U);

    for (i = 0; i < 4; i++) {
        if (masuId == GwPlayer[i].masuId) {
            MB3ASIBA_PLAYER *e = &MB3_PLAYERSCR[i].asiba;
            e->active = 1;
            e->masuId = masuId;
            if (!highSide) {
                e->areaNo = 3;
            }
            MBPlayerMotionNoShiftSet(i, 9, 0.0f, 0.0f, 0U);
        } else if (mb3_Work[0xB] == GwPlayer[i].masuId || mb3_Work[0xC] == GwPlayer[i].masuId) {
            MBModelDispSet(MBPlayerModelGet(i), FALSE);
        }
    }
    MB3_AsibaPlayerUpdate();
    HuPrcVSleep();
    MBPlayerPosFixFlagSet(FALSE);
    MBMotionSpeedSet((s16) mb3_Work[5], 1.0f);
    MBMotionSpeedSet((s16) mb3_Work[6], 1.0f);
    MBAudFXPlay(0x5E6);
    do {
        Mtx mtx;
        Hu3DModelObjMtxGet(MBModelIdGet((s16) mb3_Work[5]), "takispl-tkhook", mtx);
        MBMasuMtxSet(mb3_Work[0xB], mtx);
        Hu3DModelObjMtxGet(MBModelIdGet((s16) mb3_Work[6]), "takispl-tkhook", mtx);
        MBMasuMtxSet(mb3_Work[0xC], mtx);
        MB3_AsibaPlayerUpdate();
        HuPrcVSleep();
        anyActive = 0;
        for (i = 0; i < 4; i++) {
            if (MB3_PLAYERSCR[i].asiba.active == 1) {
                anyActive = 1;
            }
        }
    } while (anyActive);
    MBPlayerRotSet(playerNo, 0.0f, 0.0f, 0.0f);
    MBMotionSpeedSet((s16) mb3_Work[5], 0.0f);
    MBMotionTimeSet((s16) mb3_Work[5], 0.0f);
    MBMotionSpeedSet((s16) mb3_Work[6], 0.0f);
    MBMotionTimeSet((s16) mb3_Work[6], 0.0f);
    for (i = 0; i < 0x3C; i++) {
        MB3_AsibaPlayerUpdate();
        HuPrcVSleep();
    }
    WipeColorSet(0, 0, 0);
    WipeCreate(2, 0x81, 1);
    while (!WipeCheckEnd()) {
        HuPrcVSleep();
    }
    {
        Mtx mtx;
        Hu3DModelObjMtxGet(MBModelIdGet((s16) mb3_Work[5]), "takispl-tkhook", mtx);
        MBMasuMtxSet(mb3_Work[0xB], mtx);
        Hu3DModelObjMtxGet(MBModelIdGet((s16) mb3_Work[6]), "takispl-tkhook", mtx);
        MBMasuMtxSet(mb3_Work[0xC], mtx);
    }
    targetMasu = MBMasuFlagFind(0, 4U, 4U);
    for (i = 0; i < 4; i++) {
        MBModelDispSet(MBPlayerModelGet(i), TRUE);
        MB3_PLAYERSCR[i].asiba.state = -1;
        if (MB3_PLAYERSCR[i].asiba.active != 0) {
            GwPlayer[i].masuId = (s16) targetMasu;
            MBPlayerPosReset(i);
        }
    }
    MB3_AsibaPlayerUpdate();
    MBPlayerPosFixFlagSet(TRUE);
    MBPlayerPosFixCurr(masuId, TRUE);
    MBCameraSkipSet(0);
    MBCameraFocusMasuSet(masuId);
    WipeCreate(1, 0x81, 0x1E);
    while (!WipeCheckEnd()) {
        HuPrcVSleep();
    }
    for (i = 0; i < 0x3C; i++) {
        HuPrcVSleep();
    }
}
