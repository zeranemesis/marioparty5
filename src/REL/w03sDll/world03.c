#include "game/board/main.h"
#include "game/board/effect.h"
#include "game/board/audio.h"
#include "game/board/scroll.h"
#include "game/board/model.h"
#include "game/board/masu.h"
#include "game/board/opening.h"
#include "game/board/capsule.h"
#include "game/board/camera.h"
#include "game/board/player.h"
#include "game/board/window.h"
#include "game/board/coin.h"

#include "game/sprite.h"
#include "game/wipe.h"
#include "game/gamework.h"
#include "game/pad.h"
#include "game/main.h"
#include "game/object.h"
#include "game/charman.h"
#include "game/hsfex.h"

#include <math.h>

/*
 * w03sDll - Story Mode variant of the w03Dll "Shy Guys Jungle Jam" board.
 *
 * NOTE (port): mb3_Work is one flat scratch buffer (0x1C8 bytes, s32[0x72])
 * reused as several sub-structures via raw byte offsets, same idiom as
 * w03Dll (see src/REL/w03Dll/world03.c). This Story variant only creates two
 * decorative models (the boat/plank objects used by the intro cutscene) and
 * the waterfall splash effect pair; it drops the roll-rock/quicksand/pickaxe
 * board events entirely and replaces the "?" masu hook with a single long
 * cutscene (MB3Ev_HatenaYYHou) that flings the player to a random blue masu.
 */

#define MB3_OBJ_MAX 2 /* mb3_ObjFileTbl entries / model ids: mb3_Work[0..1] */

static void MB3_Create(void);
static void MB3_Kill(void);
static void MB3_ObjectCreate(void);
static void MB3_MainObjUpdate(OMOBJ *obj);
static void MB3_LightSet(void);
static void MB3_LightReset(void);

static void MB3_Splash1EffHook(HU3DMODEL *modelP, MBEFFECT *effP, Mtx matrix);
static void MB3_Splash2EffHook(HU3DMODEL *modelP, MBEFFECT *effP, Mtx matrix);
static void MB3_ImpactEffHook(HU3DMODEL *modelP, MBEFFECT *effP, Mtx matrix);

static void MB3_ImpactEffUpdate(s16 modelId, s32 frame);
static void MB3_ImpactEffPosSet(Point3d *pos, f32 pitch, f32 yaw);

static f32 MB3_DistRatioGet(Point3d *a, Point3d *b, Point3d *dir);
static s32 MB3Ev_BitGet(s32 flag, u32 mask);
static u32 MB3Ev_MasuFlagGet(s32 bits, s32 mask);

static int MB3Ev_MasuHatena(int playerNo);
static void MB3_Stub(void);
static int MB3Ev_MasuWalkPre(int playerNo);
static int MB3Ev_MasuWalkPost(int playerNo);
static void MB3Ev_HatenaYYHou(s32 playerNo);

/* ------------------------------- data ---------------------------------- */

static u32 mb3_ObjFileTbl[MB3_OBJ_MAX] = {
    0xD00006U,
    0xD00004U,
};

static const HuVecF mb3ev_TakiEffPosTbl[3] = {
    { 0.0f, 3345.0f, -1885.0f },
    { 0.0f, 1285.0f, -775.0f },
    { 0.0f, -35.0f, -185.0f },
};

static s32 mb3ev_TakiEmitterNo[3] = { -1, -1, -1 };

static OMOBJ *mb3_MainObj;
static s32 mb3_Work[0x72];
static s16 mb3_MainMdlId;
static u8 mb3_BlueMasuNoTbl[0xFF];

/* Raw byte-offset accessors into mb3_Work, same idiom as w03Dll's MB3W_*
 * macros (MWCC -O0 addresses this buffer byte-wise, not word-wise). */
#define MB3W_S16(off) (*(s16 *) ((u8 *) mb3_Work + (off)))
#define MB3W_U16(off) (*(u16 *) ((u8 *) mb3_Work + (off)))
#define MB3W_S32(off) (*(s32 *) ((u8 *) mb3_Work + (off)))

/* MB3_Create camera constants (extracted from the original REL's .rodata,
 * m2c could not infer these as it has no context for anonymous data). */
static const HuVecF mb3_OpeningPos = { 0.0f, 0.0f, 0.0f };
static const HuVecF mb3_OpeningRot = { 0.0f, 0.0f, 0.0f };
static const HuVecF mb3_MapViewPos = { 0.0f, 0.0f, 0.0f };
static const HuVecF mb3_MapViewRot = { 0.0f, 0.0f, 0.0f };

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

extern void ObjectSetup(void);

int _prolog(void) {
    const VoidFunc *ctors = _ctors;
    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void) {
    const VoidFunc *dtors = _dtors;
    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

void ObjectSetup(void) {
    GWPartyFSet(FALSE);
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

/* NONMATCHING: register allocation order differs, same as w03Dll's copy of
 * this helper (73% match there). */
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

/* NONMATCHING: register allocation order differs, same as w03Dll's copy of
 * this helper. */
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

static int MB3Ev_MasuWalkPost(int playerNo) {
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
            p->color.r = (u8) (160.0f + (70.0f * r));
            p->color.g = (u8) (200.0f + (40.0f * r));
            p->color.b = (u8) (180.0f + (55.0f * r));
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
    memset(mb3_Work, 0, sizeof(mb3_Work));
    HuAudSndGrpSetSet(0x10);
    MBMasuCreate(0xD00000);
    mb3_MainMdlId = MBModelCreate(0xD00001, NULL, FALSE);
    MBModelPosSet(mb3_MainMdlId, 0.0f, 0.0f, 0.0f);
    MBModelAttrSet(mb3_MainMdlId, HU3D_MOTATTR_LOOP);
    MBScrollInit(0xD00002U, 11000.0f, 500.0f);
    MBOpeningViewSet((HuVecF *) &mb3_OpeningRot, (HuVecF *) &mb3_OpeningPos, 7700.0f);
    MBMapViewCameraSet((HuVecF *) &mb3_MapViewRot, (HuVecF *) &mb3_MapViewPos, 11000.0f);
    MBLightHookSet(MB3_LightSet, MB3_LightReset);
    MBCapsuleColCreate(0xD00003);
    MB3_ObjectCreate();
    HuDataDirClose(0xD00000);
    MBMasuWalkPreHookSet(MB3Ev_MasuWalkPre);
    MBMasuWalkPostHookSet((MASUPLAYERHOOK) MB3Ev_MasuWalkPost);
    MBMasuHatenaHookSet(MB3Ev_MasuHatena);
}

static void MB3_ObjectCreate(void) {
    s32 i;
    s32 modelId;

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

    mb3_Work[2] = (s32)(uintptr_t) HuSprAnimRead(HuDataReadNum(0xCF0011, 0x10000000));
    mb3_Work[3] = (s32)(uintptr_t) HuSprAnimRead(HuDataReadNum(0xCF0012, 0x10000000));
    mb3_Work[4] = (s32)(uintptr_t) HuSprAnimRead(HuDataReadNum(0xCF0014, 0x10000000));
    mb3_Work[5] = (s32)(uintptr_t) HuSprAnimRead(HuDataReadNum(0xCF0015, 0x10000000));

    for (i = 0; i < 3; i++) {
        s16 id1 = (s16) MBEffCreate((ANIMDATA *)(uintptr_t) mb3_Work[3], 0xC0);
        s16 id2;
        MB3W_S16(0x20 + i * 2) = id1;
        MBEffHookSet(id1, MB3_Splash1EffHook);
        Hu3DModelLayerSet(id1, 2);
        Hu3DModelPosSetV(id1, (HuVecF *) &mb3ev_TakiEffPosTbl[i]);

        id2 = (s16) MBEffCreate((ANIMDATA *)(uintptr_t) mb3_Work[3], 0x30);
        MB3W_S16(0x26 + i * 2) = id2;
        MBEffHookSet(id2, MB3_Splash2EffHook);
        Hu3DModelLayerSet(id2, 2);
        Hu3DModelPosSet(id2, mb3ev_TakiEffPosTbl[i].x, mb3ev_TakiEffPosTbl[i].y, 50.0f + mb3ev_TakiEffPosTbl[i].z);
        mb3ev_TakiEmitterNo[i] = MBAudFXEmiterSet(0x5DC, (Vec *) &mb3ev_TakiEffPosTbl[i]);
    }
    MB3W_S16(0x2C) = (s16) MBEffCreate((ANIMDATA *)(uintptr_t) mb3_Work[4], 1);
    MB3W_S16(0x2E) = (s16) MBEffCreate((ANIMDATA *)(uintptr_t) mb3_Work[5], 1);
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
    for (i = 0; i < 3; i++) {
        s16 id1 = MB3W_S16(0x20 + i * 2);
        s16 id2 = MB3W_S16(0x26 + i * 2);
        if (id1 >= 0) {
            MBEffKill(id1);
        }
        MB3W_S16(0x20 + i * 2) = -1;
        if (id2 >= 0) {
            MBEffKill(id2);
        }
        MB3W_S16(0x26 + i * 2) = -1;
    }
    if (MB3W_S16(0x2C) >= 0) {
        MBEffKill(MB3W_S16(0x2C));
    }
    if (MB3W_S16(0x2E) >= 0) {
        MBEffKill(MB3W_S16(0x2E));
    }
    MB3W_S16(0x2E) = -1;
    MB3W_S16(0x2C) = -1;
}

static void MB3_MainObjUpdate(OMOBJ *obj) {
    if (MBKillCheck()) {
        if (mb3_MainObj != NULL) {
            omDelObjEx(mbObjMan, mb3_MainObj);
        }
        return;
    }
    (void) MBCameraGet();
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

/* Spawns up to 0x20 sparkle particles for the impact effect around `pos`,
 * fired from a cone whose axis is set by (pitch, yaw). */
static void MB3_ImpactEffPosSet(Point3d *pos, f32 pitch, f32 yaw) {
    MBEFFECT *eff = (MBEFFECT *) Hu3DData[MB3W_S16(0x30)].hookData;
    Mtx pitchMtx, yawMtx, mtx;
    Point3d ofs;
    s32 i;
    s32 budget;

    MTXRotRad(pitchMtx, 'X', 0.017453292f * pitch);
    MTXRotRad(yawMtx, 'Y', 0.017453292f * yaw);
    MTXConcat(pitchMtx, yawMtx, pitchMtx);
    MTXTransApply(pitchMtx, mtx, pos->x, pos->y, pos->z);

    budget = 0x20;
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

        if (--budget < 1) {
            break;
        }
    }
}

/* ---------------------------- masu hooks -------------------------------- */

static int MB3Ev_MasuHatena(int playerNo) {
    MASU *masuP = MBMasuGet(0, (s32) GwPlayer[playerNo].masuId);
    if (masuP->flag & 0x2000) {
        MB3Ev_HatenaYYHou(playerNo);
    }
    return 0;
}

/*
 * Story mode "?" masu event: launches the player up into the air, has them
 * throw a boomerang-like object at the ground (impact sparkle burst), then
 * warps them to a random unoccupied blue masu, landing with a second
 * impact-effect burst. Any other players already sharing that blue masu
 * (picked up along for the ride) get a reward animation and lose 5 coins.
 *
 * NONMATCHING: this cutscene is written to be complete and behaviourally
 * faithful to the original asm, but was not driven to a byte-identical
 * match within the available time (register allocation across ~90
 * HuPrcVSleep-separated basic blocks did not converge). */
static void MB3Ev_HatenaYYHou(s32 playerNo) {
    MASU *masuP;
    MASU *destMasuP;
    s16 boomerangId;
    Point3d playerPos;
    Point3d throwVel;
    Point3d launchDir;
    Point3d camPos;
    Mtx spinA, spinB;
    f32 grav;
    f32 scale;
    s32 riderFlag[4];
    s32 ridersHere;
    s32 destMasuNo;
    s32 blueCount;
    s32 impactFrame;
    s32 i;

    masuP = MBMasuGet(0, (s32) GwPlayer[playerNo].masuId);
    MBPlayerMotIdleSet(playerNo);

    boomerangId = (s16) MBModelCreate(0x8007B, NULL, FALSE);
    MBModelLayerSet(boomerangId, 6U);
    MBModelDispSet(boomerangId, FALSE);

    riderFlag[0] = -1;
    riderFlag[1] = -1;
    riderFlag[2] = -1;
    riderFlag[3] = -1;

    MB3W_S16(0x30) = (s16) MBEffCreate((ANIMDATA *)(uintptr_t) mb3_Work[5], 0x20);
    MBEffAttrSet(MB3W_S16(0x30), 8U);
    MBEffHookSet(MB3W_S16(0x30), MB3_ImpactEffHook);
    Hu3DModelLayerSet(MB3W_S16(0x30), 3);

    /* jump up above the model's throw-hook bone */
    MBPlayerMotionNoShiftSet(playerNo, 4, 0.0f, 8.0f, 0U);
    MBPlayerPosGet(playerNo, &playerPos);
    Hu3DModelObjPosGet(MBModelIdGet((s16) mb3_Work[1]), "yy_hou2-thook", &throwVel);
    throwVel.y += 200.0f;
    VECSubtract(&throwVel, &playerPos, &throwVel);
    throwVel.x /= 0.6f;
    throwVel.z /= 0.6f;
    grav = -4900.0f * 0.0002777778f;
    throwVel.y = (throwVel.y / 0.6f) - (0.5f * 0.6f * -4900.0f);
    VECScale(&throwVel, &throwVel, 0.016666668f);
    MBPlayerRotSet(playerNo, 0.0f, (f32) (180.0 * (atan2((f64) throwVel.x, (f64) throwVel.z) / 3.141592653589793)), 0.0f);

    for (i = 0; i < 3; i++) {
        HuPrcVSleep();
    }
    for (i = 0; i < (s32) (0.6f * 60.0f); i++) {
        throwVel.y += grav;
        VECAdd(&playerPos, &throwVel, &playerPos);
        MBPlayerPosSetV(playerNo, &playerPos);
        HuPrcVSleep();
    }

    /* land, shrink for the throw pose */
    MBPlayerMotionNoShiftSet(playerNo, 5, 0.0f, 0.0f, 0U);
    scale = 1.0f;
    for (i = 0; i < 30; i++) {
        playerPos.y += -200.0f / 30.0f;
        scale *= 0.95f;
        MBPlayerPosSetV(playerNo, &playerPos);
        MBPlayerScaleSet(playerNo, scale, scale, scale);
        HuPrcVSleep();
    }

    /* pick an unoccupied linked masu (unused result kept for parity with
     * the original, which computes it but never applies it) */
    destMasuNo = -1;
    for (i = 0; i < (s32) masuP->linkNum; i++) {
        u16 link = masuP->linkTbl[i];
        if (MBMasuGet(0, (s32) link)->linkNum == 0) {
            destMasuNo = (s32)(u16) link;
            break;
        }
        destMasuNo = -1;
    }

    MBCameraFocusSet(-1);
    MBMotionSpeedSet((s16) mb3_Work[1], 1.0f);
    MBMotionTimeSet((s16) mb3_Work[1], 0.0f);
    MBAudFXPlay(0x5DF);
    for (i = 0; (f32) i < (MBMotionMaxTimeGet((s16) mb3_Work[1]) - 10.0f); i++) {
        HuPrcVSleep();
    }

    /* throw the boomerang down at an angle, spin it back up */
    MBPlayerMotionNoShiftSet(playerNo, 1, 0.0f, 0.0f, HU3D_MOTATTR_LOOP);
    MBPlayerPosGet(playerNo, &playerPos);
    MBPlayerWinLoseVoicePlay(playerNo, 1, 0x1F9);

    launchDir.x = (f32) (sin(3.6651914291880923) * cos(1.0471975511965976));
    launchDir.y = (f32) sin(1.0471975511965976);
    launchDir.z = (f32) (cos(3.6651914291880923) * cos(1.0471975511965976));
    VECScale(&launchDir, &camPos, 200.0f);
    VECAdd(&playerPos, &camPos, &camPos);
    MB3_ImpactEffPosSet(&camPos, 60.0f, 200.0f);
    MBAudFXPlay(0x5E0);
    omVibrate((s16) playerNo, 0xC, 4, 2);
    VECScale(&launchDir, &launchDir, 33.333332f);
    MBCameraQuakeSet(0x12, 500.0f);

    scale = 1.0f;
    for (i = 0; (f32) i < 30.0f; i++) {
        Point3d rotOut;
        f32 spinAngle = 18.0f * (f32) (i + 1);
        MTXRotRad(spinA, 'Y', 0.017453292f * spinAngle);
        MTXRotRad(spinB, 'X', 0.5235988f);
        MTXConcat(spinB, spinA, spinA);
        MTXRotRad(spinB, 'Y', 3.6651914f);
        MTXConcat(spinB, spinA, spinA);
        Hu3DMtxRotGet(spinA, &rotOut);
        MBPlayerRotSetV(playerNo, &rotOut);
        VECAdd(&playerPos, &launchDir, &playerPos);
        MBPlayerPosSetV(playerNo, &playerPos);
        scale += 0.3f * (1.0f - scale);
        MBPlayerScaleSet(playerNo, scale, scale, scale);
        HuPrcVSleep();
    }
    MBPlayerScaleSet(playerNo, 1.0f, 1.0f, 1.0f);
    (void) frand();

    /* build the table of blue masus and pick one at random */
    blueCount = 0;
    {
        s32 masuNum = MBMasuNumGet(0);
        if (masuNum > 0xFF) {
            masuNum = 0xFF;
        }
        for (i = 1; i <= masuNum; i++) {
            if (MBMasuTypeGet(0, i) == 1) {
                mb3_BlueMasuNoTbl[blueCount] = (u8) i;
                blueCount++;
            }
        }
    }
    {
        u8 destMasu = mb3_BlueMasuNoTbl[(s32) ((f32) blueCount * MBEffRandF())];

        ridersHere = 0;
        for (i = 0; i < 4; i++) {
            if (((s32) destMasu == (s32) GwPlayer[i].masuId) && MBPlayerAliveCheck(i)) {
                riderFlag[i] = i;
                ridersHere++;
            }
        }

        WipeColorSet(0, 0, 0);
        WipeCreate(2, 0x81, 1);
        while (!WipeCheckEnd()) {
            HuPrcVSleep();
        }

        MBMotionSpeedSet((s16) mb3_Work[1], 0.0f);
        MBMotionTimeSet((s16) mb3_Work[1], 0.0f);
        MBModelDispSet(MBPlayerModelGet(playerNo), FALSE);
        MBEffKill(MB3W_S16(0x30));
        MB3W_S16(0x32) = -1;
        MB3W_S16(0x30) = -1;
        MBPlayerPosFixWarp(playerNo, (s32) destMasu, TRUE);
        MBCameraFocusMasuSet((s32) destMasu);
        MBCameraSkipSet(0);

        WipeCreate(1, 0x81, 0x1E);
        while (!WipeCheckEnd()) {
            HuPrcVSleep();
        }
        MBModelDispSet(MBPlayerModelGet(playerNo), TRUE);

        destMasuP = MBMasuGet(0, (s32) destMasu);
        playerPos = destMasuP->pos;
        playerPos.y += 1000.0f;
        launchDir.x = 0.0f;
        launchDir.z = 0.0f;
        launchDir.y = -5.0f;
        MBPlayerRotSet(playerNo, 0.0f, 0.0f, 0.0f);
        MBPlayerScaleSet(playerNo, 1.0f, 1.0f, 1.0f);
        MBPlayerMotionNoShiftSet(playerNo, 4, 0.0f, 0.0f, 0U);

        impactFrame = 0;
        while (playerPos.y > destMasuP->pos.y) {
            if (((playerPos.y - destMasuP->pos.y) < 200.0f) && (MBPlayerMotionNoGet(playerNo) != 5)) {
                MBPlayerMotionNoShiftSet(playerNo, 5, 0.0f, 0.0f, 0U);
                impactFrame = 0;
                if (ridersHere != 0) {
                    MBModelDispSet(boomerangId, TRUE);
                    MBMotionTimeSet(boomerangId, 0.0f);
                }
            }
            MBPlayerPosSetV(playerNo, &playerPos);
            MB3_ImpactEffUpdate(boomerangId, impactFrame);
            impactFrame++;
            HuPrcVSleep();
            VECAdd(&playerPos, &launchDir, &playerPos);
            launchDir.y += grav;
        }

        GwPlayer[playerNo].masuId = (s16) destMasu;
        do {
            MB3_ImpactEffUpdate(boomerangId, impactFrame);
            impactFrame++;
            HuPrcVSleep();
        } while (!MBPlayerMotionEndCheck(playerNo));
        MBPlayerMotIdleSet(playerNo);

        for (i = 0; i < 0x1E; i++) {
            impactFrame++;
            MB3_ImpactEffUpdate(boomerangId, impactFrame);
            HuPrcVSleep();
        }

        if (ridersHere != 0) {
            MBModelDispSet(boomerangId, FALSE);
            for (i = 0; i < 4; i++) {
                if (riderFlag[i] >= 0) {
                    MBPlayerMotionNoSet(i, 6, HU3D_MOTATTR_LOOP);
                }
            }
            for (i = 0; i < 4; i++) {
                if (riderFlag[i] >= 0) {
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
            if (riderFlag[i] >= 0) {
                MBPlayerMotionNoSet(i, 1, HU3D_MOTATTR_LOOP);
            }
        }

        MBMotionSpeedSet((s16) mb3_Work[1], 0.0f);
        MBMotionTimeSet((s16) mb3_Work[1], 0.0f);
        WipeCreate(1, 0x81, 0x1E);
        while (!WipeCheckEnd()) {
            HuPrcVSleep();
        }
    }

    MBModelKill(boomerangId);
    (void) destMasuNo;
}
