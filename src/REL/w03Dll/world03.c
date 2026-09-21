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
static s32 MB3Ev_BitGet(u16 flag, s32 mask);
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
        mag = ((dir->z * (a->z - b->z)) + (dir->x * (a->x - b->x)) + (dir->y * (a->y - b->y))) / mag;
    }
    return mag;
}

static s32 MB3Ev_BitGet(u16 flag, s32 mask) {
    s32 result = 0;
    s32 outBit = 1;
    s32 inBit = 1;
    s32 i;
    for (i = 0; i < 0x20; i++) {
        if (mask & inBit) {
            if (flag & mask & inBit) {
                result |= outBit;
            }
            outBit *= 2;
        }
        inBit *= 2;
    }
    return result;
}

static u32 MB3Ev_MasuFlagGet(s32 bits, s32 mask) {
    s32 inBit = 1;
    s32 outBit = 1;
    u32 result = 0;
    s32 i;
    for (i = 0; i < 0x20; i++) {
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
    return 0;
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

    mb3_Work[0xD] = (s32) HuSprAnimRead(HuDataReadNum(0xCF0011, 0x10000000));
    mb3_Work[0xE] = (s32) HuSprAnimRead(HuDataReadNum(0xCF0012, 0x10000000));
    mb3_Work[0xF] = (s32) HuSprAnimRead(HuDataReadNum(0xCF0014, 0x10000000));
    mb3_Work[0x10] = (s32) HuSprAnimRead(HuDataReadNum(0xCF0015, 0x10000000));

    for (i = 0; i < 4; i++) {
        s16 id = (s16) MBEffCreate((ANIMDATA *) mb3_Work[0xD], 0x40);
        MB3W_S16(0x44 + i * 2) = id;
        MBEffHookSet(id, MB3_TakiEffHook);
        Hu3DModelLayerSet(id, 3);
    }
    for (i = 0; i < 3; i++) {
        s16 id1 = (s16) MBEffCreate((ANIMDATA *) mb3_Work[0xE], 0xC0);
        s16 id2;
        MB3W_S16(0x4C + i * 2) = id1;
        MBEffHookSet(id1, MB3_Splash1EffHook);
        Hu3DModelLayerSet(id1, 2);
        Hu3DModelPosSetV(id1, (HuVecF *) &mb3ev_TakiEffPosTbl[i]);

        id2 = (s16) MBEffCreate((ANIMDATA *) mb3_Work[0xE], 0x30);
        MB3W_S16(0x52 + i * 2) = id2;
        MBEffHookSet(id2, MB3_Splash2EffHook);
        Hu3DModelLayerSet(id2, 2);
        Hu3DModelPosSet(id2, mb3ev_TakiEffPosTbl[i].x, mb3ev_TakiEffPosTbl[i].y, 50.0f + mb3ev_TakiEffPosTbl[i].z);
        MBAudFXEmiterSet(0x5DC, (Vec *) &mb3ev_TakiEffPosTbl[i]);
    }
    MB3W_S16(0x58) = (s16) MBEffCreate((ANIMDATA *) mb3_Work[0xF], 1);
    MB3W_S16(0x5A) = (s16) MBEffCreate((ANIMDATA *) mb3_Work[0x10], 1);
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
        c->modelId = (s16) MBModelCreate(0x80002, NULL, TRUE);
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

static void MB3_ImpactEffPosSet(Point3d *pos, f32 pitch, f32 yaw) {
    /* TODO */
}

static void MB3_RollRockPathUpdate(void) {
    /* TODO */
}

static void MB3_RollRockEffUpdate(f32 *pos) {
    /* TODO */
}

static void MB3_RollRockDustCreate(f32 *pos) {
    /* TODO */
}

static void MB3_TuruhasiEffCreate(MB3_PLAYER_SCRATCH *entry) {
    /* TODO */
}

static void MB3_AsibaPlayerUpdate(void) {
    /* TODO */
}

static void MB3_AsibaPlayerEffCreate(MB3_PLAYER_SCRATCH *entry, s32 kind) {
    /* TODO */
}

static int MB3Ev_MasuHatena(int playerNo) {
    /* TODO */
    return 0;
}

static int MB3Ev_MasuWalkPost(int playerNo) {
    /* TODO */
    return 0;
}

static void MB3Ev_KubiBone(MASU *masuP, s32 idx) {
    /* TODO */
}

static void MB3Ev_YYHou(s32 playerNo) {
    /* TODO */
}

static void MB3Ev_RollRock(s32 playerNo) {
    /* TODO */
}

static void MB3Ev_Turuhasi(s32 playerNo) {
    /* TODO */
}

static void MB3Ev_Asiba(s32 playerNo) {
    /* TODO */
}
