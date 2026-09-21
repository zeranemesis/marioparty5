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
 * w07Dll - "Koopa's Tycoon Town" board.
 *
 * NOTE (port): like w03Dll, mb7_Work is one flat scratch buffer (0x338
 * bytes = 0xCE 32-bit words) reused as several unrelated sub-structures,
 * addressed by raw word/byte offset in the original -O0 code. m2c's draft
 * mistypes it as `ANIMDATA *mb7_Work[0xCE]` (an unrelated struct that just
 * happens to share a size) - that typing is wrong and is not used here.
 * The layout below is being reconstructed from
 * config/GP5E01_00/rels/w07Dll/symbols.txt (mb7_Work = .bss:0x4 size:0x338)
 * and build/GP5E01_00/w07Dll/asm/REL/w07Dll/world07.s, word offset by word
 * offset, as each function that touches it is decompiled - see the
 * MB7W_* macros below, extended incrementally.
 */

#define MB7_OBJ_MAX 0xB   /* mb7_ObjFileTbl entries / model ids: mb7_Work[0..0xA] */
#define MB7_BACKOBJ_MAX 0x18 /* mb7_BackObjFileTbl / mb7_BackObjPosTbl / mb7_BackObjRotYTbl entries */

/* mb7_Work as a flat 32-bit-word scratch buffer, exactly like w03Dll's
 * mb3_Work: word[0..0xA] hold the 11 "front" object model ids (created
 * from mb7_ObjFileTbl), word[0xB..0x22] hold the 24 "back" object model
 * ids (mb7_BackObjFileTbl/Pos/RotY), and u16 word[0x94/2..0x9C/2] hold 4
 * dust effect ids - confirmed against
 * build/GP5E01_00/w07Dll/asm/REL/w07Dll/world07.s (MB7_ObjectCreate uses
 * `slwi r4, r31, 2` / `stw ..., 0x0(r3)` for the first two loops, i.e.
 * word-indexed s32 stores, and `slwi r4, r31, 1` / `sth ..., 0x94(r3)`
 * for the effect ids, i.e. halfword-indexed at byte offset 0x94). Layout
 * of the remaining region (Koopa walk/fire/cube, Dokan, Hou state) is
 * documented next to the functions that use it as they are decompiled. */
static s32 mb7_Work[0xCE];
#define MB7W_S16(off) (*(s16 *) ((off) + (u8 *) mb7_Work))
#define MB7W_U16(off) (*(u16 *) ((off) + (u8 *) mb7_Work))
#define MB7W_S32(off) (*(s32 *) ((off) + (u8 *) mb7_Work))
#define MB7W_F32(off) (*(f32 *) ((off) + (u8 *) mb7_Work))

static OMOBJ *mb7_MainObj;
static u8 *mb7_SaveWorkP;
static s16 mb7_MainMdlId;

static u32 mb7_ObjFileTbl[MB7_OBJ_MAX] = {
    0xD70004U, 0xD70006U, 0xD70007U, 0xD70008U, 0xD7000BU,
    0xD7000CU, 0xD7000DU, 0xD7000EU, 0xD70010U, 0xD70011U, 0xD70012U,
};

static s32 mb7_BackObjFileTbl[MB7_BACKOBJ_MAX] = {
    0xD70005, 0xD70005, 0xD70005, 0xD70005, 0xD70005, 0xD70005,
    0xD70009, 0xD70009, 0xD70009, 0xD70009,
    0xD7000A, 0xD7000A, 0xD7000A, 0xD7000A, 0xD7000A, 0xD7000A,
    0xD7000F, 0xD7000F, 0xD7000F, 0xD7000F, 0xD7000F, 0xD7000F, 0xD7000F, 0xD7000F,
};

static const HuVecF mb7_BackObjPosTbl[MB7_BACKOBJ_MAX] = {
    { -685.0f, 685.0f, -4260.0f }, { 685.0f, 685.0f, -4260.0f },
    { -945.0f, 400.0f, -1120.0f }, { 945.0f, 400.0f, -1120.0f },
    { -615.0f, -315.0f, 645.0f }, { 615.0f, -315.0f, 645.0f },
    { -715.0f, -100.0f, -1600.0f }, { 715.0f, -100.0f, -1600.0f },
    { -715.0f, -100.0f, -2435.0f }, { 715.0f, -100.0f, -2435.0f },
    { -2270.0f, 1465.0f, -5835.0f }, { 2270.0f, 1465.0f, -5835.0f },
    { -3530.0f, 500.0f, -2660.0f }, { 3530.0f, 500.0f, -2660.0f },
    { -2895.0f, 500.0f, -2030.0f }, { 2895.0f, 500.0f, -2030.0f },
    { -615.0f, 1400.0f, -4950.0f }, { 615.0f, 1400.0f, -4950.0f },
    { -3025.0f, 500.0f, -3175.0f }, { 3025.0f, 500.0f, -3175.0f },
    { -2385.0f, 500.0f, -2545.0f }, { 2385.0f, 500.0f, -2545.0f },
    { -1245.0f, -400.0f, -300.0f }, { 1245.0f, -400.0f, -300.0f },
};

static f32 mb7_BackObjRotYTbl[MB7_BACKOBJ_MAX] = {
    -60.0f, 60.0f, -45.0f, 45.0f, 30.0f, -30.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    -150.0f, 150.0f, -45.0f, 45.0f, -45.0f, 45.0f, 0.0f, 0.0f,
    -45.0f, 45.0f, -45.0f, 45.0f, 45.0f, -45.0f,
};

static s32 mb7ev_KoopaMotFileTbl[9] = {
    0x100001, 0x100002, 0x100003, 0x100004, 0x100005,
    0x100007, 0x100006, 0x10000D, 0x100010,
};

/* MB7_Create camera constants (extracted from the original REL's .rodata,
 * m2c could not infer these as it has no context for anonymous data). */
static const HuVecF mb7_OpeningPos = { 0.0f, -1000.0f, -4500.0f };
static const HuVecF mb7_OpeningRot = { -33.0f, 0.0f, 0.0f };
static const HuVecF mb7_MapViewPos = { 0.0f, -2500.0f, -78.0f };
static const HuVecF mb7_MapViewRot = { 0.0f, 0.0f, 0.0f };

/* forward declarations - definitions land in later commits as each
 * function is decompiled, in increasing order of risk (boilerplate ->
 * leaf helpers -> effect hooks -> setup/teardown -> masu event handlers
 * -> Koopa/Dokan/Hou state machines). */
static void MB7_Create(void);
static void MB7_Kill(void);
static void MB7_ObjectCreate(void);
static void MB7_MainObjUpdate(OMOBJ *obj);
static void MB7_LightSet(void);
static int MB7_MasuPathCheck(s16 id, u32 flag, s16 *linkTbl, BOOL endF);
static void MB7Ev_MasuRedInit(void);
static void MB7Ev_MasuRedClose(void);
static void MB7Ev_RedResetHook(void);
static int MB7Ev_RedResetWin(void);
static int MB7Ev_MasuHatena(int playerNo);
static int MB7Ev_MasuWalkPost(int playerNo);
static void MB7Ev_KoopaInit(u16 arg0);
static void MB7Ev_KoopaMotionCreate(s32 motionIdx);
static void MB7Ev_KoopaCreate(void);
static void MB7Ev_KoopaDataClose(void);
static void MB7Ev_KoopaMotionNoSet(s32 motionIdx, s32 loop);
static void MB7Ev_KoopaMotionShiftSet(s32 motionIdx, s32 loop);
static int MB7Ev_KoopaMotionCheck(void);
static void MB7Ev_KoopaClose(void);
static void MB7Ev_KoopaWalkFxPlay(void);
static void MB7_EffDustHook(HU3DMODEL *modelP, MBEFFECT *effP, Mtx matrix);
static s16 MB7Ev_KoopaCubeDustCreate(Point3d *pos, s32 mode);
static void MB7Ev_KoopaCubeDustEffHook(HU3DMODEL *modelP, MBEFFECT *effP, Mtx matrix);
static void MB7Ev_KoopaFire1Hook(HU3DMODEL *modelP, MBEFFECT *effP, Mtx matrix);
static void MB7Ev_KoopaFire2Hook(HU3DMODEL *modelP, MBEFFECT *effP, Mtx matrix);
static void MB7Ev_KoopaFire3Hook(HU3DMODEL *modelP, MBEFFECT *effP, Mtx matrix);
static void MB7Ev_KoopaFire4Hook(HU3DMODEL *modelP, MBEFFECT *effP, Mtx matrix);
static void MB7Ev_KoopaWalkCoinLose(void);
static void MB7Ev_KoopaWalkDustCreate(s32 dispF);
static void MB7Ev_KoopaAsibaUpdate(void);
static void MB7Ev_KoopaWalkPlayerUpdate(void);
static void MB7Ev_KoopaWalk(s32 playerNo);
static void MB7Ev_Dokan(s32 playerNo);
static void MB7Ev_DokanIn(s32 playerNo);
static void MB7Ev_KoopaCube(s32 playerNo);
static void MB7Ev_Hou(s32 playerNo);

/* mb7_Work[0x4F..0x5B] ("fire breath" sub-state, byte 0x13C..0x16C - the
 * region MB7Ev_KoopaFire1Hook reads via mb7_Work[0x53]/[0x55]/[0x56] and
 * via the pointer arithmetic m2c mistranslates as `&mb7_Work[0x4F] +
 * 0x40`, which is really `(u8 *) mb7_Work + 0x13C + 0x40` = word 0x5F).
 * Mapped by reading MB7Ev_Hou's own setup of this region (not yet fully
 * decompiled, but confirmed instruction-for-instruction for this part):
 *   +0x4F (word, byte 0x13C): fire-breath phase index (0,1,2,4,...) -
 *     a small per-frame state machine cycling the koopa's fire breath;
 *     phase 0 resets +0x53/+0x54/+0x55 to 0.0f, +0x56 to 0, plays SFX
 *     0x61D, then advances to phase 1.
 *   +0x53 (f32, byte 0x14C): current fire-breath angle (used with
 *     sin/cos in MB7Ev_KoopaFire1Hook to build the particle scatter
 *     direction around the breath axis).
 *   +0x54 (f32, byte 0x150): angle delta/step for this phase.
 *   +0x55 (f32, byte 0x154): breath sweep magnitude - scales the
 *     PSVECScale radius (`250.0f * mb7_Work[0x55]`) in Fire1Hook.
 *   +0x56 (s32, byte 0x158): active/spawned-this-frame flag - tested
 *     `!= NULL` in Fire1Hook to gate per-particle (re)spawn.
 *   +0x59..0x5B (Point3d, byte 0x164..0x170): "kupa_pos" bone world
 *     position on the koopa model (Hu3DModelObjPosGet(koopaModelId,
 *     "kupa_dream_all-kupa_pos", ...), string at lbl_1_data_340).
 *   +0x5F..0x61 (Point3d, byte 0x17C..0x188): "fire_pos" bone world
 *     position (Hu3DModelObjPosGet(koopaModelId,
 *     "kupa_dream_all-fire_pos", ...), string at lbl_1_data_358) - the
 *     actual fire particle emission origin, added to each particle's
 *     spawn position in Fire1Hook via PSVECAdd.
 * The rest of MB7Ev_Hou (the full cutscene state machine around this
 * setup) is not yet decompiled. */

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
    MBObjectSetup(6, MB7_Create, MB7_Kill);
}

/* ----------------------------- leaf helpers ----------------------------- */

static f32 MB7_DistRatioGet(Point3d *a, Point3d *b, Point3d *dir) {
    f32 mag = VECMag(dir);
    if (mag > 0.0f) {
        mag = ((dir->x * (a->x - b->x)) + (dir->y * (a->y - b->y)) + (dir->z * (a->z - b->z))) / mag;
    }
    return mag;
}

/* NONMATCHING: extra callee-save float temporary (f30/f31 split) not
 * reproduced; logic is otherwise identical instruction-for-instruction */
static f32 MB7_AngleDiff(f32 from, f32 to) {
    f32 mod = (f32) fmod((f64) (to - from), 360.0);
    f32 diff = mod;
    if (diff < 0.0f) {
        diff += 360.0f;
    }
    if (diff > 180.0f) {
        diff -= 360.0f;
    }
    return diff;
}

/* NONMATCHING: register allocation order differs (73% match) */
static s32 MB7Ev_BitGet(s32 flag, u32 mask) {
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

/* NONMATCHING: register allocation order differs (65.52% match) */
static u32 MB7Ev_MasuFlagGet(s32 bits, s32 mask) {
    u32 result;
    s32 inBit;
    s32 outBit;
    s32 i;

    result = 0;
    inBit = 1;
    outBit = 1;
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

static void MB7_LightReset(void) {
}

static void MB7_MasuStub(void) {
}

static int MB7Ev_MasuWalkPre(int playerNo) {
}

/* -------------------------- setup / teardown ---------------------------- */

/* NONMATCHING: instruction ordering differs around rodata-constant setup (63.52% match) */
static void MB7_Create(void) {
    HuAudSndGrpSetSet(0x14);
    memset(mb7_Work, 0, sizeof(mb7_Work));
    mb7_SaveWorkP = (u8 *) GwSystem.mbSaveWork;
    MBMasuCreate(0xD70000);
    mb7_MainMdlId = (s16) MBModelCreate(0xD70001, NULL, FALSE);
    MBModelPosSet(mb7_MainMdlId, 0.0f, 0.0f, 0.0f);
    MBModelAttrSet(mb7_MainMdlId, HU3D_MOTATTR_LOOP);
    MBScrollInit(0xD70002U, 21000.0f, -2500.0f);
    MBOpeningViewSet((HuVecF *) &mb7_OpeningRot, (HuVecF *) &mb7_OpeningPos, 20000.0f);
    MBMapViewCameraSet((HuVecF *) &mb7_MapViewRot, (HuVecF *) &mb7_MapViewPos, 21000.0f);
    MBLightHookSet(MB7_LightSet, MB7_LightReset);
    MBCapsuleColCreate(0xD70003);
    MBBranchFlagSet(0x40U);
    MBBranchFlagSet(0x20U);
    MBMasuPathCheckHookSet(MB7_MasuPathCheck);
    if (GwSystem.last5Effect == 3) {
        MBMasuTypeChange(2U, 3U);
    }
    MB7Ev_MasuRedInit();
    MB7_ObjectCreate();
    HuDataDirClose(0xD70000);
    MBMasuWalkPreHookSet(MB7Ev_MasuWalkPre);
    MBMasuWalkPostHookSet((MASUPLAYERHOOK) MB7Ev_MasuWalkPost);
    MBMasuHatenaHookSet(MB7Ev_MasuHatena);
    mb7_MainObj = omAddObjEx(mbObjMan, -0x8000, 0, 0, -1, MB7_MainObjUpdate);
}

/* NONMATCHING: instruction ordering differs around mb7_Work byte-offset
 * address computation and the zero-Point3d local (75.84% match) */
static void MB7_ObjectCreate(void) {
    s32 i;

    for (i = 0; i < MB7_OBJ_MAX; i++) {
        mb7_Work[i] = -1;
    }
    for (i = 0; i < MB7_BACKOBJ_MAX; i++) {
        MB7W_S32(0x2C + (i * 4)) = -1;
    }
    for (i = 0; i < 4; i++) {
        MB7W_S16(0x94 + (i * 2)) = -1;
    }

    for (i = 0; i < MB7_OBJ_MAX; i++) {
        if (mb7_ObjFileTbl[i] != 0U) {
            s32 modelId = MBModelCreate((s32) mb7_ObjFileTbl[i], NULL, FALSE);
            mb7_Work[i] = modelId;
            MBModelPosSet((s16) modelId, 0.0f, 0.0f, 0.0f);
            MBMotionSpeedSet((s16) modelId, 0.0f);
            MBMotionShapeSpeedSet((s16) modelId, 0.0f);
        }
    }
    for (i = 0; i < MB7_BACKOBJ_MAX; i++) {
        s32 modelId = MBModelCreate(mb7_BackObjFileTbl[i], NULL, TRUE);
        MB7W_S32(0x2C + (i * 4)) = modelId;
        MBModelPosSetV((s16) modelId, (Point3d *) &mb7_BackObjPosTbl[i]);
        MBModelRotSet((s16) modelId, 0.0f, mb7_BackObjRotYTbl[i], 0.0f);
    }

    if (mb7_Work[0] >= 0) {
        MBModelAttrSet((s16) mb7_Work[0], HU3D_MOTATTR_LOOP);
        MBModelAttrSet((s16) mb7_Work[0], 0x40000040U);
        MBMotionSpeedSet((s16) mb7_Work[0], 0.5f);
        MBMotionShapeSpeedSet((s16) mb7_Work[0], 0.5f);
    }
    if (mb7_Work[2] >= 0) {
        MBModelPosSet((s16) mb7_Work[2], 0.0f, 1495.0f, -4645.0f);
    }
    for (i = 4; i <= 0xA; i++) {
        if (mb7_Work[i] >= 0) {
            MBModelDispSet((s16) mb7_Work[i], FALSE);
        }
    }
    Hu3DModelLayerSet(MBModelIdGet((s16) mb7_Work[0xA]), 2);
    for (i = 0x10; i <= 0x18; i++) {
        s32 modelId = MB7W_S32(0x2C + (i * 4));
        if (modelId >= 0) {
            MBModelAttrSet((s16) modelId, HU3D_MOTATTR_LOOP);
            MBMotionSpeedSet((s16) modelId, 1.0f);
        }
    }
    MB7W_S32(0x8C) = (s32) HuSprAnimRead(HuDataReadNum(0xD70013, 0x10000000));
    MB7W_S32(0x90) = (s32) HuSprAnimRead(HuDataReadNum(0xD70014, 0x10000000));
    MB7W_S16(0x94) = (s16) MBEffCreate((void *) MB7W_S32(0x8C), 1);
    MB7W_S16(0x96) = (s16) MBEffCreate((void *) MB7W_S32(0x90), 1);
}

/* NONMATCHING: mb7_Work byte-offset address add operand order differs
 * (82.41% match); logic identical */
static void MB7_Kill(void) {
    s32 i;

    if (mb7_MainMdlId >= 0) {
        MBModelKill(mb7_MainMdlId);
        mb7_MainMdlId = -1;
    }
    MB7Ev_MasuRedClose();
    MBCameraModelViewSet(-1, NULL, NULL, -1.0f, 45.0f, 0x3F);
    for (i = 0; i < MB7_OBJ_MAX; i++) {
        if (mb7_Work[i] >= 0) {
            MBModelKill((s16) mb7_Work[i]);
        }
        mb7_Work[i] = -1;
    }
    for (i = 0; i < MB7_BACKOBJ_MAX; i++) {
        if (MB7W_S32(0x2C + (i * 4)) >= 0) {
            MBModelKill((s16) MB7W_S32(0x2C + (i * 4)));
        }
        MB7W_S32(0x2C + (i * 4)) = -1;
    }
    for (i = 0; i < 4; i++) {
        if (MB7W_S16(0x94 + (i * 2)) >= 0) {
            MBEffKill(MB7W_S16(0x94 + (i * 2)));
        }
        MB7W_S16(0x94 + (i * 2)) = -1;
    }
}

static void MB7_MainObjUpdate(OMOBJ *obj) {
    MBCAMERA *cam;

    if (MBKillCheck()) {
        if (mb7_MainObj != NULL) {
            omDelObjEx(mbObjMan, mb7_MainObj);
        }
        return;
    }
    cam = MBCameraGet();
}

static void MB7_LightSet(void) {
    Hu3DShadowCreate(45.0f, 1000.0f, 250000.0f);
    Hu3DShadowTPLvlSet(0.3f);
    Hu3DBGColorSet(0U, 0U, 0U);
    Hu3DModelLightInfoSet(MBModelIdGet(mb7_MainMdlId), 1);
    MBPreTurnHookSet(MB7Ev_RedResetHook);
}

/* ---------------------------- masu event hooks --------------------------- */

/* NONMATCHING: stack frame layout differs (75.44% match) */
static int MB7Ev_MasuHatena(int playerNo) {
    MASU *masuP = MBMasuGet(0, (s32) GwPlayer[playerNo].masuId);
    u32 flag = masuP->flag;

    if (flag & 0x8000) {
        MB7Ev_KoopaWalk(playerNo);
        return 0;
    }
    if (flag & 0x2800) {
        MB7Ev_Dokan(playerNo);
        return 0;
    }
    if (flag & 0x1400) {
        MB7Ev_DokanIn(playerNo);
        return 0;
    }
    if (flag & 0x300) {
        MB7Ev_KoopaCube(playerNo);
        return 0;
    }
    if (flag & 0x80) {
        MB7Ev_Hou(playerNo);
    }
    return 0;
}

/* NONMATCHING: stack frame layout differs (69.23% match) */
static int MB7Ev_MasuWalkPost(int playerNo) {
    s16 masuId = GwPlayer[playerNo].masuId;
    s32 flag = (u16) MBMasuGet(0, masuId)->flag;
    return 0;
}

/* NONMATCHING: register allocation order differs */
/* NONMATCHING: register allocation differs, real code uses _savegpr_27 (80.00% match) */
static int MB7_MasuPathCheck(s16 id, u32 flag, s16 *linkTbl, BOOL endF) {
    s32 num;
    s32 i;
    s32 count;

    count = 0;
    if (flag & 0x1400) {
        num = MBMasuNumGet(0);
        for (i = 1; i <= num; i++) {
            MASU *masuP = MBMasuGet(0, i);
            if (masuP->flag & 0x2800) {
                linkTbl[count] = (s16) i;
                count++;
            }
        }
    }
    if (count <= 0) {
        count = -1;
    }
    return count;
}

/* --------------------------- Koopa walk state ---------------------------- */

/* Koopa "walk out" cutscene/state region, mb7_Work + 0xA0, 0x9C bytes
 * (byte offsets below are relative to that +0xA0 base, matching
 * MB7Ev_KoopaInit's own addressing - confirmed 92.86% match). Extended
 * as more of the region's users are decompiled:
 *   +0x8 (s32)                      coin-lose cutscene lock (0/1),
 *     set/cleared around MBCoinAddDispExec in MB7Ev_KoopaWalkCoinLose
 *   +0x34/+0x38/+0x3C/+0x40 (s32[4]) = -1 after memset - up to 4 koopa
 *     model ids (confirmed by MB7Ev_KoopaClose, which kills all 4);
 *     only index [0] (+0x34) is read by the motion helpers so far
 *   +0x44..+0x64 (s32[9]) = -1      motion ids, one per
 *     mb7ev_KoopaMotFileTbl entry (MB7Ev_KoopaMotionCreate/NoSet/
 *     ShiftSet always address index 0, i.e. the fixed +0x34 koopa)
 *   +0x68 (s16)                     dust-puff MBEFFECT model id
 *     (MB7Ev_KoopaWalkDustCreate), created in MB7Ev_KoopaWalk (TBD)
 *   +0x6C (s32)                     MBDataDirReadAsync() handle
 *   +0x70 (s32) = -1, +0x74 (s32) = 0   last motion index / step timer
 *   +0x78 (Point3d)                 foot position, used both as the
 *     footstep SFX emitter (MB7Ev_KoopaWalkFxPlay) and the dust-puff
 *     spawn position (MB7Ev_KoopaWalkDustCreate)
 */
/* NONMATCHING: redundant li r0,-1 reload before consecutive stores
 * (92.86% match); logic identical */
static void MB7Ev_KoopaInit(u16 arg0) {
    u8 *base = (u8 *) mb7_Work + 0xA0;
    s32 i;

    memset(base, 0, 0x9C);
    *(s32 *) (base + 0x70) = -1;
    *(s32 *) (base + 0x74) = 0;
    *(s32 *) (base + 0x40) = -1;
    *(s32 *) (base + 0x3C) = -1;
    *(s32 *) (base + 0x38) = -1;
    *(s32 *) (base + 0x34) = -1;
    for (i = 0; i < 9; i++) {
        *(s32 *) (base + 0x44 + (i * 4)) = -1;
    }
    *(s32 *) (base + 0x6C) = MBDataDirReadAsync(0x100000);
}

/* motion index -> motion id, base+0x44..+0x64 (s32[9]); base+0x34 is the
 * Koopa model id created by MB7Ev_KoopaCreate */
/* NONMATCHING: register allocation order differs (64.71% match) */
static void MB7Ev_KoopaMotionCreate(s32 motionIdx) {
    u8 *base = (u8 *) mb7_Work + 0xA0;

    if (*(s32 *) (base + 0x44 + (motionIdx * 4)) < 0) {
        s16 koopaMdlId = (s16) *(s32 *) (base + 0x34);
        *(s32 *) (base + 0x44 + (motionIdx * 4)) = MBMotionCreate(koopaMdlId, mb7ev_KoopaMotFileTbl[motionIdx]);
    }
}

/* NONMATCHING: compiler inlines the MB7Ev_KoopaMotionCreate(0) call here
 * (constant arg) where the original keeps it out-of-line; extra stack
 * slots as a result (42.86% match), logic identical */
static void MB7Ev_KoopaCreate(void) {
    u8 *base = (u8 *) mb7_Work + 0xA0;
    s16 koopaMdlId;

    MBDataAsyncWait(*(s32 *) (base + 0x6C));
    koopaMdlId = (s16) MBModelCreate(0x100000, NULL, TRUE);
    *(s32 *) (base + 0x34) = koopaMdlId;
    MBModelDispSet((s16) *(s32 *) (base + 0x34), FALSE);
    HuPrcVSleep();
    MB7Ev_KoopaMotionCreate(0);
    MB7Ev_KoopaMotionNoSet(0, 1);
    HuPrcVSleep();
}

static void MB7Ev_KoopaDataClose(void) {
    u8 *base = (u8 *) mb7_Work + 0xA0;
    HuDataDirClose(*(s32 *) (base + 0x6C));
}

/* NONMATCHING: register allocation order differs */
/* NONMATCHING: register allocation order differs (80.49% match) */
static void MB7Ev_KoopaMotionNoSet(s32 motionIdx, s32 loop) {
    u8 *base = (u8 *) mb7_Work + 0xA0;
    s32 attr = 0;
    s16 koopaMdlId;

    if (loop) {
        attr = 0x40000001;
        if (motionIdx != *(s32 *) (base + 0x70)) {
            *(s32 *) (base + 0x74) = 0;
        }
    }
    koopaMdlId = (s16) *(s32 *) (base + 0x34);
    MBMotionNoSet(koopaMdlId, (s16) *(s32 *) (base + 0x44 + (motionIdx * 4)), attr);
    *(s32 *) (base + 0x70) = motionIdx;
}

/* NONMATCHING: register allocation order differs */
/* NONMATCHING: register allocation order differs (70.21% match) */
static void MB7Ev_KoopaMotionShiftSet(s32 motionIdx, s32 loop) {
    u8 *base = (u8 *) mb7_Work + 0xA0;
    s32 attr = 0;
    s16 koopaMdlId;

    if (loop) {
        attr = 0x40000001;
        if (motionIdx != *(s32 *) (base + 0x70)) {
            *(s32 *) (base + 0x74) = 0;
        }
    }
    koopaMdlId = (s16) *(s32 *) (base + 0x34);
    MBMotionShiftSet(koopaMdlId, (s16) *(s32 *) (base + 0x44 + (motionIdx * 4)), 0.0f, 8.0f, attr);
    *(s32 *) (base + 0x70) = motionIdx;
}

static int MB7Ev_KoopaMotionCheck(void) {
    u8 *base = (u8 *) mb7_Work + 0xA0;

    if (MBMotionShiftIDGet((s16) *(s32 *) (base + 0x34)) >= 0) {
        return 0;
    }
    return MBMotionEndCheck((s16) *(s32 *) (base + 0x34));
}

/* NONMATCHING: register allocation order differs (69.70% match) */
static void MB7Ev_KoopaClose(void) {
    u8 *base = (u8 *) mb7_Work + 0xA0;
    s32 i;

    for (i = 0; i < 4; i++) {
        if (*(s32 *) (base + 0x34 + (i * 4)) >= 0) {
            MBModelKill((s16) *(s32 *) (base + 0x34 + (i * 4)));
        }
        *(s32 *) (base + 0x34 + (i * 4)) = -1;
    }
}

/* NONMATCHING: cror-fused compare not reproduced */
static void MB7Ev_KoopaWalkFxPlay(void) {
    u8 *base = (u8 *) mb7_Work + 0xA0;
    s16 koopaMdlId;
    f32 time;

    if (*(s32 *) (base + 0x70) != 1) {
        return;
    }
    koopaMdlId = (s16) *(s32 *) (base + 0x34);
    time = MBMotionTimeGet(koopaMdlId);
    if (*(s32 *) (base + 0x74) == 0) {
        if (time >= 15.0f) {
            if (time < 25.0f) {
                HuAudFXEmiterPlay(0x2C7, (Point3d *) (base + 0x78));
            }
            *(s32 *) (base + 0x74) = 1;
        }
    } else if (*(s32 *) (base + 0x74) == 1) {
        if (time >= 65.0f) {
            if (time < 75.0f) {
                HuAudFXEmiterPlay(0x2C7, (Point3d *) (base + 0x78));
            }
            *(s32 *) (base + 0x74) = 2;
        }
    } else if (time < 65.0f) {
        *(s32 *) (base + 0x74) = 0;
    }
}

/* ------------------------------ effect hooks ----------------------------- */

/* Base grey/blue/dark-blue/yellow colors for the dust and fire particle
 * hooks below - dumped from the shipped REL's .data (offset 0x274, 0x30
 * bytes = 4 HuVecF). Only entry [0] is used by MB7_EffDustHook; entries
 * [1..3] are read by the not-yet-decompiled MB7Ev_KoopaFire hooks. */
static const HuVecF mb7ev_DustColTbl[4] = {
    { 128.0f, 128.0f, 128.0f },
    { 64.0f, 64.0f, 64.0f },
    { 64.0f, 64.0f, 192.0f },
    { 131.0f, 131.0f, 0.0f },
};

/* base/variation color pairs for MB7Ev_KoopaCubeDustEffHook, indexed by
 * [effP->mode*2] (base) / [effP->mode*2+1] (variation) */
static const HuVecF mb7ev_KoopaCubeDustColTbl[4] = {
    { 192.0f, 64.0f, 64.0f },
    { 0.0f, 131.0f, 131.0f },
    { 64.0f, 64.0f, 192.0f },
    { 131.0f, 131.0f, 0.0f },
};

/* Standard MBEFFECT/MBEFFECTDATA particle hook (same shape as w03Dll's
 * MB3_ImpactEffHook), abusing effP->mode as a "spawn N new particles with
 * this parManId-1 recipe" trigger and effP->count as a one-shot "particle
 * array not yet cleared" flag - both are otherwise-unused MBEFFECT fields
 * for this non-Hu3D-driven hook. */
/* NONMATCHING: stack frame/register allocation differs slightly (extra
 * callee-save float regs for the per-iteration locals) (50.15% match,
 * only 55 of 648 rows are real differences - the rest are rodata label
 * churn); logic instruction-for-instruction identical to the original */
static void MB7_EffDustHook(HU3DMODEL *modelP, MBEFFECT *effP, Mtx matrix) {
    MBEFFECTDATA *p;
    s32 i;

    if (effP->count == 0) {
        p = effP->data;
        for (i = 0; i < effP->num; i++, p++) {
            p->time = 0;
            p->scale = 0.0f;
            p->color.a = 0;
            p->parManId = 0;
        }
        effP->mode = 0;
        effP->count = 1;
        return;
    }

    if (effP->mode != 0) {
        p = effP->data;
        for (i = 0; i < effP->num; i++, p++) {
            f32 angle;
            Point3d dir;
            f32 t;

            p->parManId = effP->mode - 1;
            p->time = (s16) (60.0f * (0.7f + (0.4f * MBEffRandF())));
            angle = 360.0f * MBEffRandF();
            dir.x = (f32) sin((3.141592653589793 * (f64) angle) / 180.0);
            dir.z = (f32) cos((3.141592653589793 * (f64) angle) / 180.0);
            dir.y = 0.0f;
            if (p->parManId == 0) {
                PSVECScale(&dir, &p->pos, 100.0f * (0.7f * (0.5f + (0.5f * MBEffRandF()))));
                p->pos.y = 20.0f + (20.0f * MBEffRandF());
                PSVECScale(&dir, &p->vel, 1.6666666f * (15.0f * (0.7f + (0.3f * MBEffRandF()))));
                p->vel.y = 1.6666666f * (0.7f * (0.4f + (0.6f * MBEffRandF())));
                p->scale = 30.0f + (20.0f * MBEffRandF());
                p->color.a = (u8) (s32) (50.0f + (60.0f * MBEffRandF()));
            } else if (p->parManId == 1) {
                PSVECScale(&dir, &p->pos, 100.0f * (0.7f * (0.5f + (0.5f * MBEffRandF()))));
                p->pos.y = 20.0f + (20.0f * MBEffRandF());
                PSVECScale(&dir, &p->vel, 1.6666666f * (9.0f * (0.7f + (0.3f * MBEffRandF()))));
                p->vel.y = 1.6666666f * (1.2f * (0.4f + (0.6f * MBEffRandF())));
                p->scale = 30.0f + (20.0f * MBEffRandF());
                p->color.a = (u8) (s32) (40.0f + (30.0f * MBEffRandF()));
            }
            p->rot.z = 360.0f * MBEffRandF();
            t = 0.3f + (0.7f * MBEffRandF());
            p->color.r = (u8) (s32) (mb7ev_DustColTbl[0].x + (mb7ev_DustColTbl[0].x * t));
            p->color.g = (u8) (s32) (mb7ev_DustColTbl[0].y + (mb7ev_DustColTbl[0].y * t));
            p->color.b = (u8) (s32) (mb7ev_DustColTbl[0].z + (mb7ev_DustColTbl[0].z * t));
        }
        effP->mode = 0;
    }

    p = effP->data;
    for (i = 0; i < effP->num; i++, p++) {
        if (p->time != 0) {
            VECAdd(&p->pos, &p->vel, &p->pos);
            if (p->parManId == 0) {
                p->vel.x *= 0.84f;
                p->vel.z *= 0.84f;
            } else if (p->parManId == 1) {
                p->vel.x *= 0.88f;
                p->vel.z *= 0.88f;
            }
            p->time -= 1;
            if (p->time <= 0) {
                p->time = 0;
                p->scale = 0.0f;
                p->color.a = 0;
            } else if (p->time < 0x10 && p->color.a >= 5U) {
                p->color.a -= 5;
            }
            if (p->color.a >= 2U) {
                p->color.a -= 2;
            }
            p->scale += 3.0f;
        }
    }
}

/* Cube-hit dust burst: creates a one-shot MBEFFECT, tags it with `mode`
 * (0/1, selects color pair and scatter recipe) stored through
 * Hu3DData[].hookData the same way w03Dll reads it back as MBEFFECT*. */
/* NONMATCHING: register allocation order differs (50.00% match) */
static s16 MB7Ev_KoopaCubeDustCreate(Point3d *pos, s32 mode) {
    s16 modelId = MBEffCreate((ANIMDATA *) mb7_Work[0x24], 0x40);
    MBEffHookSet(modelId, MB7Ev_KoopaCubeDustEffHook);
    Hu3DModelLayerSet(modelId, 2);
    Hu3DModelPosSetV(modelId, pos);
    *(s16 *) Hu3DData[modelId].hookData = (s16) mode;
    return modelId;
}

/* NONMATCHING: stack frame/register allocation differs slightly, same
 * shape as MB7_EffDustHook (46.14% match, 71 of 470 rows are real
 * differences); logic identical */
static void MB7Ev_KoopaCubeDustEffHook(HU3DMODEL *modelP, MBEFFECT *effP, Mtx matrix) {
    MBEFFECTDATA *p;
    s32 i;

    if (effP->count == 0) {
        p = effP->data;
        for (i = 0; i < effP->num; i++, p++) {
            f32 angle;
            Point3d dir;
            f32 spread;
            f32 t;

            p->time = 1;
            p->parManId = (s16) (60.0f * (0.4f + (0.3f * MBEffRandF())));
            angle = 360.0f * MBEffSRandF();
            dir.x = (f32) sin((3.141592653589793 * (f64) angle) / 180.0);
            dir.z = (f32) cos((3.141592653589793 * (f64) angle) / 180.0);
            dir.y = 0.0f;
            spread = (f32) (1.0 + sin((3.141592653589793 * (f64) (360.0f * MBEffRandF())) / 180.0));
            if (spread > 1.0f) {
                spread -= 2.0f;
            }
            PSVECScale(&dir, &p->pos, 100.0f * (0.5f * (0.5f + (0.5f * MBEffRandF()))));
            PSVECScale(&dir, &p->vel, 1.6666666f * (1.5f * (0.7f + (0.3f * MBEffRandF()))));
            p->rot.z = 360.0f * MBEffRandF();
            p->scale = 60.0f + (60.0f * MBEffRandF());
            p->color.a = (u8) (s32) (50.0f + (80.0f * MBEffRandF()));
            t = (0.7f * ((0.5f + (0.5f * spread)) * 0.7f)) + (0.3f * MBEffRandF());
            p->color.r = (u8) (s32) (mb7ev_KoopaCubeDustColTbl[effP->mode * 2].x + (t * mb7ev_KoopaCubeDustColTbl[(effP->mode * 2) + 1].x));
            p->color.g = (u8) (s32) (mb7ev_KoopaCubeDustColTbl[effP->mode * 2].y + (t * mb7ev_KoopaCubeDustColTbl[(effP->mode * 2) + 1].y));
            p->color.b = (u8) (s32) (mb7ev_KoopaCubeDustColTbl[effP->mode * 2].z + (t * mb7ev_KoopaCubeDustColTbl[(effP->mode * 2) + 1].z));
        }
        effP->count = 1;
    }

    p = effP->data;
    for (i = 0; i < effP->num; i++, p++) {
        if (p->time != 0) {
            VECAdd(&p->pos, &p->vel, &p->pos);
            p->vel.y += 0.27222225f;
            p->parManId -= 1;
            if (p->parManId <= 0) {
                p->time = 0;
                p->scale = 0.0f;
                p->color.a = 0;
            } else if (p->parManId < 0xA && p->color.a >= 0xAU) {
                p->color.a -= 0xA;
            }
            if (p->color.a >= 0xAU) {
                p->color.a -= 2;
            }
            p->scale += 4.0f;
        }
    }
}

/* ------------------------------- red masu -------------------------------- */

/* mb7_Work[0xCC] (s32, masu count + 1) and mb7_Work[0xCD] (a u8 * cast
 * through s32, one countdown byte per masu id) sit at the very end of
 * mb7_Work (words 0xCC/0xCD out of 0xCE total). mb7_SaveWorkP - despite
 * being typed u32 * from GwSystem.mbSaveWork - is addressed byte-wise
 * here (lbzx/stbx with no scale): mb7_SaveWorkP[0]/[1] hold one pending
 * "turn a masu red for N turns" countdown per player (0/1), confirmed
 * against build/GP5E01_00/w07Dll/asm/REL/w07Dll/world07.s. */

/* NONMATCHING: register allocation order differs (77.05% match) */
static void MB7Ev_MasuRedInit(void) {
    s32 playerIdx;

    mb7_Work[0xCC] = MBMasuNumGet(0) + 1;
    mb7_Work[0xCD] = (s32) (uintptr_t) HuMemDirectMallocNum(HEAP_HEAP, mb7_Work[0xCC], 0x10000000U);
    memset((void *) (uintptr_t) mb7_Work[0xCD], 0, mb7_Work[0xCC]);

    for (playerIdx = 0; playerIdx < 2; playerIdx++) {
        if (mb7_SaveWorkP[playerIdx] != 0) {
            u8 countdown = mb7_SaveWorkP[playerIdx];
            s32 masuId = 1;
            MASU *masuP = NULL;

            for (; masuId < mb7_Work[0xCC]; masuId++) {
                masuP = MBMasuGet(0, masuId);
                if (masuP->type == 4 && (masuP->flag & 0x8000)) {
                    s32 side = (masuP->pos.x == 0.0f) ? 0 : 1;
                    if (playerIdx == side) {
                        break;
                    }
                }
            }
            if (masuId < mb7_Work[0xCC]) {
                do {
                    s32 linkIdx = 0;
                    u16 linkMasuId = 0;
                    MASU *linkMasuP = masuP;

                    for (; linkIdx < (s32) masuP->linkNum; linkIdx++) {
                        linkMasuId = masuP->linkTbl[linkIdx];
                        linkMasuP = MBMasuGet(0, (s32) linkMasuId);
                        if (linkMasuP->flag & 0x4000) {
                            break;
                        }
                    }
                    masuP = linkMasuP;
                    if (masuP->type == 1) {
                        MBMasuTypeSet(0, (s32) linkMasuId, 2);
                        ((u8 *) (uintptr_t) mb7_Work[0xCD])[linkMasuId] = countdown;
                    }
                } while (!(masuP->flag & 0x8000));
            }
        }
    }
}

static void MB7Ev_MasuRedClose(void) {
    if ((u32) mb7_Work[0xCD] != 0U) {
        HuMemDirectFree((void *) (uintptr_t) mb7_Work[0xCD]);
    }
    mb7_Work[0xCD] = 0;
    mb7_Work[0xCC] = 0;
    MBPreTurnHookSet(NULL);
}

/* NONMATCHING: register allocation order differs (75.00% match) */
static void MB7Ev_MasuRedSet(s32 masuId) {
    if ((u32) mb7_Work[0xCD] != 0U) {
        ((u8 *) (uintptr_t) mb7_Work[0xCD])[masuId] = 2;
    }
}

/* NONMATCHING: register allocation order differs (76.19% match) */
static int MB7Ev_RedResetWin(void) {
    MBWinPlayerDisable(MBWinCreate(2, 0x33000BU, -1), -1);
    MBTopWinWait();
    MBTopWinKill();
    return 1;
}

/* NONMATCHING: extra local (redTimer) not cached by the original,
 * causing repeated mb7_Work[0xCD] reloads there instead (57.52% match) */
static void MB7Ev_RedResetHook(void) {
    s32 anyExpired;
    s32 playerIdx;
    s32 i;
    u8 *redTimer;

    if ((u32) mb7_Work[0xCD] == 0U) {
        return;
    }
    redTimer = (u8 *) (uintptr_t) mb7_Work[0xCD];
    anyExpired = 0;
    for (playerIdx = 0; playerIdx < 2; playerIdx++) {
        if (mb7_SaveWorkP[playerIdx] != 0) {
            mb7_SaveWorkP[playerIdx] -= 1;
            if (mb7_SaveWorkP[playerIdx] == 0) {
                anyExpired = 1;
            }
        }
    }
    if ((GwSystem.turnMax - GwSystem.turnNo) == 4 && (mb7_SaveWorkP[0] + mb7_SaveWorkP[1]) != 0) {
        mb7_SaveWorkP[1] = 0;
        mb7_SaveWorkP[0] = 0;
        anyExpired = 1;
        for (i = 1; i < mb7_Work[0xCC]; i++) {
            if (redTimer[i] != 0) {
                redTimer[i] = 1;
            }
        }
    }
    if (anyExpired != 0) {
        for (i = 1; i < mb7_Work[0xCC]; i++) {
            if (redTimer[i] != 0) {
                redTimer[i] -= 1;
                if (redTimer[i] == 0 && MBMasuTypeGet(0, i) == 2) {
                    MBMasuTypeSet(0, i, 1);
                }
            }
        }
        MBPlayerPreTurnHookSet(0, MB7Ev_RedResetWin);
        return;
    }
    for (i = 1; i < mb7_Work[0xCC]; i++) {
        if (redTimer[i] != 0) {
            redTimer[i] -= 1;
        }
    }
}

/* ---------------------------- fire breath hook --------------------------- */

/* PowerPC fast reciprocal-sqrt estimate refined by 3 Newton-Raphson
 * iterations (not 4, unlike the system sqrt() in
 * PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/math.h) - used once per
 * spawned fire particle in MB7Ev_KoopaFire1Hook to normalize a 2D
 * (x,z) scatter vector. */
static f32 MB7_FastSqrt(f32 x) {
    f64 guess = __frsqrte((f64) x);
    guess = 0.5 * guess * (3.0 - ((f64) x * guess * guess));
    guess = 0.5 * guess * (3.0 - ((f64) x * guess * guess));
    guess = 0.5 * guess * (3.0 - ((f64) x * guess * guess));
    return (f32) ((f64) x * guess);
}

/* NONMATCHING: extra live Point3d locals cause more callee-saved float
 * registers than the original (65.87% objdiff fuzzy match); logic
 * identical (uses the mb7_Work[0x4F..0x5B] fire-breath state documented
 * above: +0x53 breath angle, +0x55 sweep magnitude, +0x56 active flag,
 * +0x5F..0x61 the "fire_pos" bone world position as spawn origin) */
static void MB7Ev_KoopaFire1Hook(HU3DMODEL *modelP, MBEFFECT *effP, Mtx matrix) {
    MBEFFECTDATA *p;
    s32 i;
    Point3d dir;
    Point3d origin;
    Point3d diff;
    f32 sweep;

    if (effP->count == 0) {
        p = effP->data;
        for (i = 0; i < effP->num; i++, p++) {
            p->scale = 0.0f;
            p->color.a = 0;
            p->time = (s16) -(i >> 3);
        }
        effP->count = 1;
    }

    dir.x = (f32) sin((3.141592653589793 * (f64) MB7W_F32(0x14C)) / 180.0);
    dir.y = -0.15f;
    dir.z = (f32) cos((3.141592653589793 * (f64) MB7W_F32(0x14C)) / 180.0);
    PSVECScale(&dir, &origin, 250.0f * MB7W_F32(0x154));
    PSVECAdd(&origin, (Point3d *) ((u8 *) mb7_Work + 0x17C), &origin);
    sweep = 0.3f + (0.7f * MB7W_F32(0x154));

    p = effP->data;
    for (i = 0; i < effP->num; i++, p++) {
        if (p->time < 0) {
            if (MB7W_S32(0x158) != 0) {
                p->time += 1;
            }
            continue;
        }
        if (p->time == 0) {
            if (MB7W_S32(0x158) != 0) {
                f32 angle2;
                f32 vy;
                f32 t;
                f32 horizMag;
                f32 speed;
                f32 angle3;
                f32 spread;
                u8 grey;

                p->time += 1;
                p->rot.z = 360.0f * MBEffRandF();
                grey = (u8) (s32) (160.0f + (95.0f * MBEffRandF()));
                p->color.b = grey;
                p->color.g = grey;
                p->color.r = grey;
                angle2 = 360.0f * MBEffRandF();
                p->pos.x = (f32) sin((3.141592653589793 * (f64) angle2) / 180.0);
                p->pos.z = (f32) cos((3.141592653589793 * (f64) angle2) / 180.0);
                vy = (f32) (1.0 - sin((3.141592653589793 * (f64) (90.0f * MBEffRandF())) / 180.0));
                if (MBEffSRandF() < 0.0f) {
                    vy *= -1.0f;
                }
                p->pos.y = vy;
                t = 1.0f - (vy * vy);
                if (t == 0.0f) {
                    horizMag = 0.0f;
                } else {
                    horizMag = MB7_FastSqrt(1.0f + (t * t));
                }
                p->pos.x *= horizMag;
                p->pos.z *= horizMag;
                speed = sweep * (2.5f + (1.6666666f * (1.5f * MBEffRandF())));
                p->vel.x = p->pos.x * speed;
                p->vel.z = p->pos.z * speed;
                p->vel.y = p->pos.y * speed;
                p->parManId = 0x14;
                PSVECScale(&p->pos, &p->pos, 30.000002f * sweep);
                if (p->pos.y > 0.0f) {
                    p->pos.y *= 1.0f + (3.0f * MBEffRandF());
                }
                PSVECAdd(&origin, &p->pos, &p->pos);
                p->accel.x = 100.0f + (80.0f * MBEffRandF());
                p->accel.y = sweep;
                p->color.a = 0;
                p->scale = sweep * (30.0f + (30.0f * MBEffRandF()));
                p->animBank = (s16) ((s32) (1000.0f * MBEffRandF()) & 3);
                angle3 = 360.0f * MBEffRandF();
                spread = sweep * (3.3333333f + (1.6666666f * (2.0f * MBEffRandF())));
                p->speedDecay = (f32) ((f64) p->speedDecay + ((f64) spread * sin((3.141592653589793 * (f64) angle3) / 180.0)));
                p->scaleBase = (f32) ((f64) p->scaleBase + ((f64) spread * cos((3.141592653589793 * (f64) angle3) / 180.0)));
            } else {
                continue;
            }
        }

        PSVECAdd(&p->pos, &p->vel, &p->pos);
        p->vel.y += 0.8166668f * p->accel.y;
        p->vel.x *= 0.95f;
        p->vel.z *= 0.95f;
        if (p->pos.y > (origin.y + (50.0f * p->accel.y))) {
            PSVECSubtract(&origin, &p->pos, &diff);
            p->vel.x += 0.7f * (0.016666668f * diff.x);
            p->vel.z += 0.7f * (0.016666668f * diff.z);
            if (p->pos.y > (origin.y + (90.0f * p->accel.y))) {
                p->vel.x += 1.3f * (0.016666668f * p->speedDecay);
                p->vel.z += 1.3f * (0.016666668f * p->scaleBase);
            }
        }
        p->parManId -= 1;
        if (p->parManId <= 0) {
            p->time = 0;
            p->scale = 0.0f;
            p->color.a = 0;
        } else if (p->parManId < 8) {
            if (p->color.a >= 0x1EU) {
                p->color.a -= 0x1E;
            }
            p->scale += 5.0f;
        } else {
            p->color.a = (u8) (s32) ((f32) p->color.a + (0.3f * (p->accel.x - (f32) p->color.a)));
            p->scale += 2.0f;
        }
    }
}

/* NONMATCHING: extra live Point3d locals cause more callee-saved float
 * registers than the original, same shape as MB7Ev_KoopaFire1Hook; the
 * differences from Fire1 (spawn cadence -(i>>2), blendMode=1, white/red
 * ember color, no chimney-pull once particles rise above +90*accel.y)
 * are transcribed straight from the asm, not guessed. */
static void MB7Ev_KoopaFire2Hook(HU3DMODEL *modelP, MBEFFECT *effP, Mtx matrix) {
    MBEFFECTDATA *p;
    s32 i;
    Point3d dir;
    Point3d origin;
    Point3d diff;
    f32 sweep;

    if (effP->count == 0) {
        p = effP->data;
        for (i = 0; i < effP->num; i++, p++) {
            p->scale = 0.0f;
            p->color.a = 0;
            p->time = (s16) -(i >> 2);
        }
        effP->count = 1;
        effP->blendMode = 1;
    }

    dir.x = (f32) sin((3.141592653589793 * (f64) MB7W_F32(0x14C)) / 180.0);
    dir.y = -0.15f;
    dir.z = (f32) cos((3.141592653589793 * (f64) MB7W_F32(0x14C)) / 180.0);
    PSVECScale(&dir, &origin, 250.0f * MB7W_F32(0x154));
    PSVECAdd(&origin, (Point3d *) ((u8 *) mb7_Work + 0x17C), &origin);
    sweep = 0.3f + (0.7f * MB7W_F32(0x154));

    p = effP->data;
    for (i = 0; i < effP->num; i++, p++) {
        if (p->time < 0) {
            if (MB7W_S32(0x158) != 0) {
                p->time += 1;
            }
            continue;
        }
        if (p->time == 0) {
            if (MB7W_S32(0x158) != 0) {
                f32 angle2;
                f32 vy;
                f32 t;
                f32 horizMag;
                f32 speed;

                p->time += 1;
                p->rot.z = 360.0f * MBEffRandF();
                frand();
                p->color.b = 0xFF;
                p->color.g = 0xFF;
                p->color.r = 0xFF;
                p->color.r = (u8) (s32) (128.0f + (127.0f * MBEffRandF()));
                angle2 = 360.0f * MBEffRandF();
                p->pos.x = (f32) sin((3.141592653589793 * (f64) angle2) / 180.0);
                p->pos.z = (f32) cos((3.141592653589793 * (f64) angle2) / 180.0);
                vy = (f32) (1.0 - sin((3.141592653589793 * (f64) (90.0f * MBEffRandF())) / 180.0));
                if (MBEffSRandF() < 0.0f) {
                    vy *= -1.0f;
                }
                p->pos.y = vy;
                t = 1.0f - (vy * vy);
                if (t == 0.0f) {
                    horizMag = 0.0f;
                } else {
                    horizMag = MB7_FastSqrt(1.0f + (t * t));
                }
                p->pos.x *= horizMag;
                p->pos.z *= horizMag;
                speed = sweep * (1.3333334f + (1.6666666f * (1.2f * MBEffRandF())));
                p->vel.x = p->pos.x * speed;
                p->vel.z = p->pos.z * speed;
                p->vel.y = p->pos.y * speed;
                p->parManId = 0x10;
                PSVECScale(&p->pos, &p->pos, 30.000002f * sweep);
                if (p->pos.y > 0.0f) {
                    p->pos.y *= 1.0f + (2.0f * MBEffRandF());
                }
                PSVECAdd(&origin, &p->pos, &p->pos);
                p->accel.x = sweep * (50.0f + (70.0f * MBEffRandF()));
                p->accel.y = sweep;
                p->color.a = 0;
                p->scale = sweep * (40.0f + (60.0f * MBEffRandF()));
                p->animBank = (s16) ((s32) (1000.0f * MBEffRandF()) & 3);
            } else {
                continue;
            }
        }

        PSVECAdd(&p->pos, &p->vel, &p->pos);
        p->vel.y += 1.088889f * p->accel.y;
        p->vel.x *= 0.95f;
        p->vel.z *= 0.95f;
        if (p->pos.y > (origin.y + (50.0f * p->accel.y))) {
            PSVECSubtract(&origin, &p->pos, &diff);
            p->vel.x += 0.7f * (0.016666668f * diff.x);
            p->vel.z += 0.7f * (0.016666668f * diff.z);
        }
        p->parManId -= 1;
        if (p->parManId <= 0) {
            p->time = 0;
            p->scale = 0.0f;
            p->color.a = 0;
        } else if (p->parManId < 8) {
            if (p->color.a >= 0x1EU) {
                p->color.a -= 0x1E;
            }
            p->scale += 5.0f;
        } else {
            p->color.a = (u8) (s32) ((f32) p->color.a + (0.3f * (p->accel.x - (f32) p->color.a)));
            p->scale += 2.0f;
        }
    }
}

/* NONMATCHING: extra live Point3d locals, same shape as Fire1/Fire2 -
 * this one additionally biases scatter vs. breath-direction by a
 * `sin(90*rand())` cone factor, and mixes in a second breath-direction
 * push (into a reused scratch Point3d) on top of the plain outward
 * velocity, before offsetting by the copied "fire_pos" bone position. */
static void MB7Ev_KoopaFire3Hook(HU3DMODEL *modelP, MBEFFECT *effP, Mtx matrix) {
    MBEFFECTDATA *p;
    s32 i;
    Point3d dir;
    Point3d origin;
    Point3d firePos;
    Point3d tmp;
    Point3d diff;
    f32 sweep;

    if (effP->count == 0) {
        p = effP->data;
        for (i = 0; i < effP->num; i++, p++) {
            p->scale = 0.0f;
            p->color.a = 0;
            p->time = (s16) -(i >> 2);
        }
        effP->count = 1;
    }

    dir.x = (f32) sin((3.141592653589793 * (f64) MB7W_F32(0x14C)) / 180.0);
    dir.y = 0.0f;
    dir.z = (f32) cos((3.141592653589793 * (f64) MB7W_F32(0x14C)) / 180.0);
    PSVECScale(&dir, &origin, 250.0f * MB7W_F32(0x154));
    PSVECAdd(&origin, (Point3d *) ((u8 *) mb7_Work + 0x17C), &origin);
    firePos = *(Point3d *) ((u8 *) mb7_Work + 0x17C);
    sweep = 0.3f + (0.7f * MB7W_F32(0x154));

    p = effP->data;
    for (i = 0; i < effP->num; i++, p++) {
        if (p->time < 0) {
            if (MB7W_S32(0x158) != 0) {
                p->time += 1;
            }
            continue;
        }
        if (p->time == 0) {
            if (MB7W_S32(0x158) != 0) {
                f32 angle2;
                f32 vy;
                f32 t;
                f32 horizMag;
                f32 coneFactor;
                f32 speed;
                f32 angle3;
                f32 spread;
                u8 grey;

                p->time += 1;
                p->rot.z = 360.0f * MBEffRandF();
                grey = (u8) (s32) (160.0f + (95.0f * MBEffRandF()));
                p->color.b = grey;
                p->color.g = grey;
                p->color.r = grey;
                angle2 = 360.0f * MBEffRandF();
                p->pos.x = (f32) sin((3.141592653589793 * (f64) angle2) / 180.0);
                p->pos.z = (f32) cos((3.141592653589793 * (f64) angle2) / 180.0);
                vy = (f32) (1.0 - sin((3.141592653589793 * (f64) (90.0f * MBEffRandF())) / 180.0));
                if (MBEffSRandF() < 0.0f) {
                    vy *= -1.0f;
                }
                p->pos.y = vy;
                t = 1.0f - (vy * vy);
                if (t == 0.0f) {
                    horizMag = 0.0f;
                } else {
                    horizMag = MB7_FastSqrt(1.0f + (t * t));
                }
                p->pos.x *= horizMag;
                p->pos.z *= horizMag;
                coneFactor = (f32) sin((3.141592653589793 * (f64) (90.0f * MBEffRandF())) / 180.0);
                speed = (0.7f + (0.3f * coneFactor)) * (sweep * (2.5f + (1.6666666f * (1.5f * MBEffRandF()))));
                p->vel.x = p->pos.x * speed;
                p->vel.z = p->pos.z * speed;
                p->vel.y = p->pos.y * speed;
                p->vel.y += sweep * (-2.5f - (1.6666666f * MBEffRandF()));
                PSVECScale(&dir, &tmp, 1.6666666f * (7.0f * ((1.0f - (0.7f * coneFactor)) * sweep)));
                PSVECAdd(&tmp, &p->vel, &p->vel);
                p->parManId = 0x10;
                PSVECScale(&p->pos, &p->pos, 30.000002f * sweep * coneFactor);
                PSVECScale(&dir, &tmp, 100.0f * (1.8f * (coneFactor * MB7W_F32(0x154))));
                PSVECAdd(&firePos, &p->pos, &p->pos);
                PSVECAdd(&tmp, &p->pos, &p->pos);
                p->accel.x = 80.0f + (80.0f * MBEffRandF());
                p->accel.y = sweep;
                p->color.a = 0;
                p->scale = sweep * (30.0f + (30.0f * MBEffRandF()));
                p->animBank = (s16) ((s32) (1000.0f * MBEffRandF()) & 3);
                angle3 = 360.0f * MBEffRandF();
                spread = sweep * (3.3333333f + (1.6666666f * (2.0f * MBEffRandF())));
                p->speedDecay = (f32) ((f64) p->speedDecay + ((f64) spread * sin((3.141592653589793 * (f64) angle3) / 180.0)));
                p->scaleBase = (f32) ((f64) p->scaleBase + ((f64) spread * cos((3.141592653589793 * (f64) angle3) / 180.0)));
            } else {
                continue;
            }
        }

        PSVECAdd(&p->pos, &p->vel, &p->pos);
        p->vel.y += 0.8166668f * p->accel.y;
        p->vel.x *= 0.98f;
        p->vel.z *= 0.98f;
        if (p->pos.y > (origin.y + (30.000002f * p->accel.y))) {
            PSVECSubtract(&origin, &p->pos, &diff);
            p->vel.x += 0.7f * (0.016666668f * diff.x);
            p->vel.z += 0.7f * (0.016666668f * diff.z);
            if (p->pos.y > (origin.y + (50.0f * p->accel.y))) {
                p->vel.x += 1.3f * (0.016666668f * p->speedDecay);
                p->vel.z += 1.3f * (0.016666668f * p->scaleBase);
            }
        }
        p->parManId -= 1;
        if (p->parManId <= 0) {
            p->time = 0;
            p->scale = 0.0f;
            p->color.a = 0;
        } else if (p->parManId < 8) {
            if (p->color.a >= 0x1EU) {
                p->color.a -= 0x1E;
            }
            p->scale += 5.0f;
        } else {
            p->color.a = (u8) (s32) ((f32) p->color.a + (0.3f * (p->accel.x - (f32) p->color.a)));
            p->scale += 2.0f;
        }
    }
}

/* NONMATCHING: same shape as Fire3Hook, with effP->blendMode set to
 * additive (1) and different constants (accel.x/scale scaled by sweep
 * instead of fixed, weaker downward vel.y bias). */
static void MB7Ev_KoopaFire4Hook(HU3DMODEL *modelP, MBEFFECT *effP, Mtx matrix) {
    MBEFFECTDATA *p;
    s32 i;
    Point3d dir;
    Point3d origin;
    Point3d firePos;
    Point3d tmp;
    Point3d diff;
    f32 sweep;

    if (effP->count == 0) {
        p = effP->data;
        for (i = 0; i < effP->num; i++, p++) {
            p->scale = 0.0f;
            p->color.a = 0;
            p->time = (s16) -(i >> 2);
        }
        effP->count = 1;
        effP->blendMode = 1;
    }

    dir.x = (f32) sin((3.141592653589793 * (f64) MB7W_F32(0x14C)) / 180.0);
    dir.y = 0.0f;
    dir.z = (f32) cos((3.141592653589793 * (f64) MB7W_F32(0x14C)) / 180.0);
    PSVECScale(&dir, &origin, 250.0f * MB7W_F32(0x154));
    PSVECAdd(&origin, (Point3d *) ((u8 *) mb7_Work + 0x17C), &origin);
    firePos = *(Point3d *) ((u8 *) mb7_Work + 0x17C);
    sweep = 0.3f + (0.7f * MB7W_F32(0x154));

    p = effP->data;
    for (i = 0; i < effP->num; i++, p++) {
        if (p->time < 0) {
            if (MB7W_S32(0x158) != 0) {
                p->time += 1;
            }
            continue;
        }
        if (p->time == 0) {
            if (MB7W_S32(0x158) != 0) {
                f32 angle2;
                f32 vy;
                f32 t;
                f32 horizMag;
                f32 coneFactor;
                f32 speed;
                f32 angle3;
                f32 spread;
                u8 grey;

                p->time += 1;
                p->rot.z = 360.0f * MBEffRandF();
                grey = (u8) (s32) (160.0f + (95.0f * MBEffRandF()));
                p->color.b = grey;
                p->color.g = grey;
                p->color.r = grey;
                angle2 = 360.0f * MBEffRandF();
                p->pos.x = (f32) sin((3.141592653589793 * (f64) angle2) / 180.0);
                p->pos.z = (f32) cos((3.141592653589793 * (f64) angle2) / 180.0);
                vy = (f32) (1.0 - sin((3.141592653589793 * (f64) (90.0f * MBEffRandF())) / 180.0));
                if (MBEffSRandF() < 0.0f) {
                    vy *= -1.0f;
                }
                p->pos.y = vy;
                t = 1.0f - (vy * vy);
                if (t == 0.0f) {
                    horizMag = 0.0f;
                } else {
                    horizMag = MB7_FastSqrt(1.0f + (t * t));
                }
                p->pos.x *= horizMag;
                p->pos.z *= horizMag;
                coneFactor = (f32) sin((3.141592653589793 * (f64) (90.0f * MBEffRandF())) / 180.0);
                speed = (0.7f + (0.3f * coneFactor)) * (sweep * (2.5f + (1.6666666f * (1.5f * MBEffRandF()))));
                p->vel.x = p->pos.x * speed;
                p->vel.z = p->pos.z * speed;
                p->vel.y = p->pos.y * speed;
                p->vel.y += sweep * (-2.1666665f - (1.6666666f * MBEffRandF()));
                PSVECScale(&dir, &tmp, 1.6666666f * (7.0f * ((1.0f - (0.7f * coneFactor)) * sweep)));
                PSVECAdd(&tmp, &p->vel, &p->vel);
                p->parManId = 0x10;
                PSVECScale(&p->pos, &p->pos, 30.000002f * sweep * coneFactor);
                PSVECScale(&dir, &tmp, 100.0f * (1.8f * (coneFactor * MB7W_F32(0x154))));
                PSVECAdd(&firePos, &p->pos, &p->pos);
                PSVECAdd(&tmp, &p->pos, &p->pos);
                p->accel.x = sweep * (50.0f + (60.0f * MBEffRandF()));
                p->accel.y = sweep;
                p->color.a = 0;
                p->scale = sweep * (40.0f + (40.0f * MBEffRandF()));
                p->animBank = (s16) ((s32) (1000.0f * MBEffRandF()) & 3);
                angle3 = 360.0f * MBEffRandF();
                spread = sweep * (3.3333333f + (1.6666666f * (2.0f * MBEffRandF())));
                p->speedDecay = (f32) ((f64) p->speedDecay + ((f64) spread * sin((3.141592653589793 * (f64) angle3) / 180.0)));
                p->scaleBase = (f32) ((f64) p->scaleBase + ((f64) spread * cos((3.141592653589793 * (f64) angle3) / 180.0)));
            } else {
                continue;
            }
        }

        PSVECAdd(&p->pos, &p->vel, &p->pos);
        p->vel.y += 0.8166668f * p->accel.y;
        p->vel.x *= 0.98f;
        p->vel.z *= 0.98f;
        if (p->pos.y > (origin.y + (30.000002f * p->accel.y))) {
            PSVECSubtract(&origin, &p->pos, &diff);
            p->vel.x += 0.7f * (0.016666668f * diff.x);
            p->vel.z += 0.7f * (0.016666668f * diff.z);
            if (p->pos.y > (origin.y + (50.0f * p->accel.y))) {
                p->vel.x += 1.3f * (0.016666668f * p->speedDecay);
                p->vel.z += 1.3f * (0.016666668f * p->scaleBase);
            }
        }
        p->parManId -= 1;
        if (p->parManId <= 0) {
            p->time = 0;
            p->scale = 0.0f;
            p->color.a = 0;
        } else if (p->parManId < 8) {
            if (p->color.a >= 0x1EU) {
                p->color.a -= 0x1E;
            }
            p->scale += 5.0f;
        } else {
            p->color.a = (u8) (s32) ((f32) p->color.a + (0.3f * (p->accel.x - (f32) p->color.a)));
            p->scale += 2.0f;
        }
    }
}

/* -------------------------- Koopa walk cutscene --------------------------- */

/* NONMATCHING: register allocation order differs (77.78% match) */
static void MB7Ev_KoopaWalkCoinLose(void) {
    HUPROCESS *proc = HuPrcCurrentGet();
    u8 *base = (u8 *) mb7_Work + 0xA0;

    *(s32 *) (base + 0x8) = 1;
    MBCoinAddDispExec((s32) proc->property, -0xA, TRUE);
    proc->property = NULL;
    *(s32 *) (base + 0x8) = 0;
    HuPrcEnd();
}

/* dust puff at the koopa's foot position (mb7_Work+0xA0+0x78, the same
 * Point3d used as the footstep SFX emitter in MB7Ev_KoopaWalkFxPlay);
 * mb7_Work+0xA0+0x68 (s16) is the dust MBEFFECT model id, created
 * elsewhere (MB7Ev_KoopaWalk); its hookData mode is set to dispF+1.
 * NONMATCHING: register allocation order differs (48.28% match) */
static void MB7Ev_KoopaWalkDustCreate(s32 dispF) {
    u8 *base = (u8 *) mb7_Work + 0xA0;

    Hu3DModelPosSetV(*(s16 *) (base + 0x68), (Point3d *) (base + 0x78));
    *(s16 *) Hu3DData[*(s16 *) (base + 0x68)].hookData = (s16) (dispF + 1);
}

/* Asiba (scaffold platform prop, mb7_Work[7] - one of the 11 "front"
 * objects hidden by default) tilt/drift update, reusing the koopa
 * region's +0x78 pos / +0x84 rot / +0x90 velocity as a scratch Point3d
 * triple (only ever used by this cutscene, not concurrently with the
 * koopa's own pos/rot which live at different offsets); +0x30 (f32) is
 * the target tilt angle. */
static void MB7Ev_KoopaAsibaUpdate(void) {
    s32 asibaMdlId = mb7_Work[7];
    u8 *base = (u8 *) mb7_Work + 0xA0;

    MBModelDispSet((s16) asibaMdlId, TRUE);
    MBAngleMoveTo((f32 *) (base + 0x88), *(f32 *) (base + 0x30), 0.15f);
    PSVECAdd((Point3d *) (base + 0x78), (Point3d *) (base + 0x90), (Point3d *) (base + 0x78));
    MBModelPosSetV((s16) asibaMdlId, (Point3d *) (base + 0x78));
    MBModelRotSetV((s16) asibaMdlId, (Point3d *) (base + 0x84));
    MBModelPosSet((s16) *(s32 *) (base + 0x34), 0.0f, 0.0f, 0.0f);
    MBModelRotSet((s16) *(s32 *) (base + 0x34), 0.0f, 0.0f, 0.0f);
}

/* per-player array, mb7_Work + 0x1C0 (word 0x70), 4 entries stride
 * 0x5C: +0x4 (s32) unused marker, +0x8 (s32) state, +0xC (s32) masuId,
 * +0x10 (s32) sub-state, +0x20 (s16) motion-shift flag, +0x2C (Point3d)
 * saved pos, +0x38 (Point3d) saved rot, +0x3C (f32) unused after reset. */
/* NONMATCHING: register allocation order differs */
static void MB7Ev_KoopaWalkPlayerUpdate(void) {
    u8 *koopaBase = (u8 *) mb7_Work + 0xA0;
    s32 playerNo;

    for (playerNo = 0; playerNo < 4; playerNo++) {
        u8 *e = (u8 *) mb7_Work + 0x1C0 + (playerNo * 0x5C);

        switch (*(s32 *) (e + 0x8)) {
        case 0:
            *(s32 *) (e + 0xC) = GwPlayer[playerNo].masuId;
            MBPlayerPosGet(playerNo, (Point3d *) (e + 0x2C));
            MBPlayerRotGet(playerNo, (Point3d *) (e + 0x38));
            *(s32 *) (e + 0x4) = -1;
            *(s32 *) (e + 0x8) += 1;
            break;
        case 1:
            if (*(s32 *) (koopaBase + 0x14) == *(s32 *) (e + 0xC) && *(s32 *) (koopaBase + 0x1C) != 0) {
                *(s32 *) (e + 0x10) = 0;
                *(s32 *) (e + 0x8) += 1;
            }
            break;
        case 2:
            if (*(s32 *) (e + 0x10) == 0) {
                omVibrate((s16) playerNo, 0xC, 4, 2);
                MBPlayerMotionNoSet(playerNo, 0xA, 0U);
                *(s32 *) (e + 0x10) += 1;
            } else if (*(s32 *) (e + 0x10) == 1 && MBPlayerMotionTimeGet(playerNo) > 30.0f) {
                MBPlayerMotionSpeedSet(playerNo, 0.0f);
                *(s32 *) (e + 0x8) += 1;
                *(s32 *) (e + 0x10) = 0;
            }
            break;
        case 3:
            if (*(s32 *) (e + 0x10) == 0) {
                Point3d diff;

                PSVECSubtract((Point3d *) (koopaBase + 0x78), (Point3d *) (e + 0x2C), &diff);
                diff.y = 0.01f;
                if (PSVECMag(&diff) > 150.0f) {
                    MBPlayerMotionSpeedSet(playerNo, 1.0f);
                    HuPrcChildCreate(MB7Ev_KoopaWalkCoinLose, 0x200D, 0x4000, 0, mbMainProcess)->property = (void *) (uintptr_t) playerNo;
                    *(s32 *) (e + 0x10) += 1;
                }
            } else if (*(s32 *) (e + 0x10) == 1 && MBPlayerMotionEndCheck(playerNo) != 0) {
                *(s32 *) (e + 0x8) += 1;
                MBPlayerMotionNoShiftSet(playerNo, 6, 0.0f, 8.0f, 0x40000001U);
            }
            break;
        case -1:
            MBPlayerMotionNoSet(playerNo, 1, 0x40000001U);
            *(s16 *) (e + 0x20) = -1;
            *(f32 *) (e + 0x3C) = 0.0f;
            break;
        }
        MBPlayerPosSetV(playerNo, (Point3d *) (e + 0x2C));
        MBPlayerRotSetV(playerNo, (Point3d *) (e + 0x38));
    }
}
