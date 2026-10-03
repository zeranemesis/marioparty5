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

static HuVecF mb7_BackObjPosTbl[MB7_BACKOBJ_MAX] = {
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

static HuVecF mb7ev_KoopaAsibaPosTbl[2] = {
    { -3925.0f, 1350.0f, -6085.0f },
    { 3925.0f, 1350.0f, -6085.0f },
};

static HuVecF mb7ev_KoopaCubePosTbl[2][2] = {
    { { -3850.0f, 1150.0f, -5780.0f }, { -3635.0f, 1150.0f, -6000.0f } },
    { { 3850.0f, 1150.0f, -5780.0f }, { 3635.0f, 1150.0f, -6000.0f } },
};

static HuVecF mb7ev_KoopaWalkStartPosTbl[2] = {
    { 4580.0f, 495.0f, -4260.0f },
    { -4580.0f, 495.0f, -4260.0f },
};

static HuVecF mb7ev_DoorPosTbl[2] = {
    { 4400.0f, 495.0f, -4000.0f },
    { -4400.0f, 495.0f, -4000.0f },
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
static void MB7Ev_MasuHatena(int playerNo);
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
static void MB7Ev_KoopaWalkUpdate(void);
static void MB7Ev_DokanSet(s32 playerNo, s32 masuId, s32 goingIn);
static void MB7Ev_DokanPlayerSet(s32 playerNo, s32 masuId, s32 goingIn);
static void MB7Ev_KoopaWalk(s32 playerNo);
static void MB7Ev_Dokan(s32 playerNo);
static void MB7Ev_DokanIn(s32 playerNo);
static void MB7Ev_KoopaCube(s32 playerNo);
static void MB7Ev_Hou(s32 playerNo);
static void MB7Ev_HouPlayerUpdate(void);

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
 *   +0x64/+0x6C (f32 x/z, byte 0x1A0/0x1A8): a target x/z position
 *     (probably the splash/landing point in front of the koopa) that
 *     MB7Ev_HouPlayerUpdate compares the player's position against,
 *     via atan2 and MB7_AngleDiff against the +0x10 breath angle, to
 *     decide whether the player is standing in the fire's path.
 * The rest of MB7Ev_Hou (the full cutscene state machine around this
 * setup) is not yet decompiled. */

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

extern void ObjectSetup(void);

/* Every function in this unit is out of line in the original object (52
 * text symbols, and `bl <name>` call sites for all 14 static helpers).
 * MWCC -O0,p otherwise auto-inlines any function DEFINED IN THIS FILE at
 * every call site, which merged each helper's body into its callers: the
 * rebuilt .text was 7760 bytes in MB7Ev_KoopaCube alone against the
 * target's 5428, 54480 against 46268 for the unit, and the real MB*
 * library calls that live inside the helpers appeared as calls from the
 * big board-event functions. One file-scope pragma restores the original
 * shape. MB7_FastSqrt is the one exception and resets it below: the
 * original has no such symbol and no `bl` to it, so the four Fire hooks
 * carry its body inline. The original has no .ctors, so the __sinit this
 * suppresses for file-scope statics costs nothing. */
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

/* The three REL entry points above must be left alone: with the pragma
 * active MWCC pushes _prolog/_epilog out into their own .text sections,
 * which drops ObjectSetup from 100 % to 61.90 %. Everything from here on
 * is a static helper or a board-event function the original also keeps
 * out of line. */
#pragma dont_inline on

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

static s32 MB7Ev_BitGet(s32 flag, u32 mask) {
    s32 i;
    u32 inBit;
    s32 result;
    u32 outBit;

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
static u32 MB7Ev_MasuFlagGet(s32 bits, u32 mask) {
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

/* the original never sets a return value here (just `blr`); the result
 * is genuinely unused by the caller for this particular hook */
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
    MBMasuHatenaHookSet((MASUPLAYERHOOK) MB7Ev_MasuHatena);
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
    MB7W_S32(0x8C) = (s32)(uintptr_t) HuSprAnimRead(HuDataReadNum(0xD70013, 0x10000000));
    MB7W_S32(0x90) = (s32)(uintptr_t) HuSprAnimRead(HuDataReadNum(0xD70014, 0x10000000));
    MB7W_S16(0x94) = (s16) MBEffCreate((void *)(uintptr_t) MB7W_S32(0x8C), 1);
    MB7W_S16(0x96) = (s16) MBEffCreate((void *)(uintptr_t) MB7W_S32(0x90), 1);
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
static void MB7Ev_MasuHatena(int playerNo) {
    MASU *masuP = MBMasuGet(0, (s32) GwPlayer[playerNo].masuId);
    u32 flag = (u16) masuP->flag;

    if (flag & 0x8000) {
        MB7Ev_KoopaWalk(playerNo);
        return;
    }
    if (flag & 0x2800) {
        MB7Ev_Dokan(playerNo);
        return;
    }
    if (flag & 0x1400) {
        MB7Ev_DokanIn(playerNo);
        return;
    }
    if (flag & 0x300) {
        MB7Ev_KoopaCube(playerNo);
        return;
    }
    if (flag & 0x80) {
        MB7Ev_Hou(playerNo);
    }
}

/* NONMATCHING: stack frame layout differs (69.23% match) */
static int MB7Ev_MasuWalkPost(int playerNo) {
    s16 masuId = GwPlayer[playerNo].masuId;
    MASU *masu = MBMasuGet(0, masuId);
    s32 flag = (u16) masu->flag;
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
 * (92.86% match); logic identical. The original keeps this out-of-line
 * at every call site (confirmed via `bl MB7Ev_KoopaInit` in both
 * MB7Ev_KoopaWalk and MB7Ev_Hou); the file-scope dont_inline above
 * covers it, since arg0 is unused and -inline would otherwise fold it
 * in. */
static void MB7Ev_KoopaInit(u16 arg0) {
    u8 *base = (u8 *) mb7_Work + 0xA0;
    s32 i;

    memset(base, 0, 0x9C);
    *(s32 *) (base + 0x70) = -1;
    *(s32 *) (base + 0x74) = 0;
    *(s32 *) (base + 0x34) = *(s32 *) (base + 0x38) = *(s32 *) (base + 0x3C) = *(s32 *) (base + 0x40) = -1;
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

    if (loop) {
        attr = 0x40000001;
        if (motionIdx == *(s32 *) (base + 0x70)) {
            return;
        }
        *(s32 *) (base + 0x74) = 0;
    }
    MBMotionNoSet((s16) *(s32 *) (base + 0x34), *(s32 *) (base + 0x44 + (motionIdx * 4)), attr);
    *(s32 *) (base + 0x70) = motionIdx;
}

/* NONMATCHING: register allocation order differs */
/* NONMATCHING: register allocation order differs (70.21% match) */
static void MB7Ev_KoopaMotionShiftSet(s32 motionIdx, s32 loop) {
    u8 *base = (u8 *) mb7_Work + 0xA0;
    s32 attr = 0;

    if (loop) {
        attr = 0x40000001;
        if (motionIdx == *(s32 *) (base + 0x70)) {
            return;
        }
        *(s32 *) (base + 0x74) = 0;
    }
    MBMotionShiftSet((s16) *(s32 *) (base + 0x34), *(s32 *) (base + 0x44 + (motionIdx * 4)), 0.0f, 8.0f, attr);
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
    f32 time;

    if (*(s32 *) (base + 0x70) != 1) {
        return;
    }
    time = MBMotionTimeGet((s16) *(s32 *) (base + 0x34));
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
static HuVecF mb7ev_DustColTbl[4] = {
    { 128.0f, 128.0f, 128.0f },
    { 64.0f, 64.0f, 64.0f },
    { 64.0f, 64.0f, 192.0f },
    { 131.0f, 131.0f, 0.0f },
};

/* base/variation color pairs for MB7Ev_KoopaCubeDustEffHook, indexed by
 * [effP->mode*2] (base) / [effP->mode*2+1] (variation) */
static HuVecF mb7ev_KoopaCubeDustColTbl[4] = {
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
                p->color.a = (u8) (50.0f + (60.0f * MBEffRandF()));
            } else if (p->parManId == 1) {
                PSVECScale(&dir, &p->pos, 100.0f * (0.7f * (0.5f + (0.5f * MBEffRandF()))));
                p->pos.y = 20.0f + (20.0f * MBEffRandF());
                PSVECScale(&dir, &p->vel, 1.6666666f * (9.0f * (0.7f + (0.3f * MBEffRandF()))));
                p->vel.y = 1.6666666f * (1.2f * (0.4f + (0.6f * MBEffRandF())));
                p->scale = 30.0f + (20.0f * MBEffRandF());
                p->color.a = (u8) (40.0f + (30.0f * MBEffRandF()));
            }
            p->rot.z = 360.0f * MBEffRandF();
            t = 0.3f + (0.7f * MBEffRandF());
            p->color.r = (u8) (mb7ev_DustColTbl[0].x + (mb7ev_DustColTbl[0].x * t));
            p->color.g = (u8) (mb7ev_DustColTbl[0].y + (mb7ev_DustColTbl[0].y * t));
            p->color.b = (u8) (mb7ev_DustColTbl[0].z + (mb7ev_DustColTbl[0].z * t));
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
    s16 modelId = MBEffCreate((ANIMDATA *)(uintptr_t) mb7_Work[0x24], 0x40);
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

static void MB7Ev_MasuRedSet(s32 masuId) {
    if ((u8 *) mb7_Work[0xCD] != NULL) {
        ((u8 *) mb7_Work[0xCD])[masuId] = 2;
    }
}

/* NONMATCHING: register allocation order differs (76.19% match) */
static int MB7Ev_RedResetWin(void) {
    int winNo = MBWinCreate(2, 0x33000BU, -1);
    MBWinPlayerDisable(winNo, -1);
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
/* NONMATCHING: not a symbol in the original - it is inlined at its four
 * call sites in the MB7Ev_KoopaFire1..4Hook bodies, so it must stay
 * inlinable and is exempt from the file-scope dont_inline. */
#pragma dont_inline reset
static f32 MB7_FastSqrt(f32 x) {
#ifdef TARGET_PC
    /* __frsqrte is a PowerPC compiler intrinsic with no host equivalent;
     * seed the same Newton-Raphson refinement with an exact reciprocal
     * sqrt instead of a hardware estimate. The refinement below already
     * converges to full precision, so the seed only affects rounding in
     * the last bit or two. */
    f64 guess = 1.0 / sqrt((f64) x);
#else
    f64 guess = __frsqrte((f64) x);
#endif
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
    MBCoinAddDispExec((s32)(uintptr_t) proc->property, -0xA, TRUE);
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

/* NONMATCHING: register allocation order differs across this function's
 * 5 sequential loops (54.64% objdiff fuzzy match); calls
 * MB7Ev_KoopaWalkUpdate which is not decompiled yet (only declared) */
static void MB7Ev_KoopaWalk(s32 playerNo) {
    u8 *base = (u8 *) mb7_Work + 0xA0;
    MASU *masuP = MBMasuGet(0, (s32) GwPlayer[playerNo].masuId);
    u16 flag = (u16) masuP->flag;
    f32 savedZoom = MBCameraZoomGet();
    s32 side = 0;
    s32 i;

    if (masuP->pos.x < 0.0f) {
        side = 1;
    }
    MB7Ev_KoopaInit(flag);
    MBPlayerMotIdleSet(playerNo);
    MBMotionTimeSet(mb7_Work[1], 0.0f);
    MBMotionSpeedSet(mb7_Work[1], 1.0f);

    {
        s32 seId = -1;
        for (i = 0; i < 0x82; i++) {
            if (i == 0) {
                seId = HuAudFXEmiterPlay(0x616, (Point3d *) &mb7ev_DoorPosTbl[side]);
            } else if (i == 0x78) {
                HuAudFXStop(seId);
                HuAudFXEmiterPlay(0x617, (Point3d *) &mb7ev_DoorPosTbl[side]);
            }
            HuPrcVSleep();
        }
    }

    MBMotionSpeedSet(mb7_Work[1], 0.0f);
    MB7Ev_KoopaCreate();
    MB7Ev_KoopaMotionCreate(1);
    MB7Ev_KoopaMotionCreate(3);
    MB7Ev_KoopaMotionCreate(6);
    MB7Ev_KoopaMotionCreate(7);
    MB7Ev_KoopaDataClose();
    *(s32 *) (base + 0x68) = MBEffCreate((ANIMDATA *)(uintptr_t) mb7_Work[0x24], 0x80);
    MBEffHookSet((s16) *(s32 *) (base + 0x68), MB7_EffDustHook);
    Hu3DModelLayerSet((s16) *(s32 *) (base + 0x68), 2);
    *(s32 *) (base + 0x10) = -1;
    *(s32 *) (base + 0x14) = GwPlayer[playerNo].masuId;
    MB7Ev_KoopaWalkUpdate();
    memset((u8 *) mb7_Work + 0x1C0, 0, 0x170);
    MB7Ev_KoopaWalkPlayerUpdate();
    MBAudFXPlay(0x2C9);
    MBWinCreate(2, 0x330000U, 7);
    MBTopWinWait();
    MBTopWinKill();
    MBCameraPosViewSet(NULL, NULL, NULL, 2800.0f, -1.0f, 0x1E);
    while (*(s32 *) (base + 0xC) < 0xB) {
        MB7Ev_KoopaWalkUpdate();
        MB7Ev_KoopaWalkPlayerUpdate();
        HuPrcVSleep();
    }

    MBWinCreate(2, 0x330001U, 7);
    MBTopWinWait();
    MBTopWinKill();
    MBCameraFocusSet(-1);
    while (*(s32 *) (base + 0xC) != -1) {
        MB7Ev_KoopaWalkUpdate();
        MB7Ev_KoopaWalkPlayerUpdate();
        HuPrcVSleep();
    }

    MBMotionSpeedSet(mb7_Work[1], 1.0f);
    MBMotionTimeSet(mb7_Work[1], 240.0f);

    {
        s32 seId = -1;
        for (i = 0; i < 0x82; i++) {
            if (i == 0) {
                seId = HuAudFXEmiterPlay(0x616, (Point3d *) &mb7ev_DoorPosTbl[1 - side]);
            } else if (i == 0x78) {
                HuAudFXStop(seId);
                HuAudFXEmiterPlay(0x617, (Point3d *) &mb7ev_DoorPosTbl[1 - side]);
            }
            MB7Ev_KoopaWalkPlayerUpdate();
            HuPrcVSleep();
        }
    }

    MBMotionTimeSet(mb7_Work[1], 0.0f);
    MBMotionSpeedSet(mb7_Work[1], 0.0f);
    MBEffKill((s16) *(s32 *) (base + 0x68));
    *(s32 *) (base + 0x68) = -1;
    WipeColorSet(0, 0, 0);
    WipeCreate(2, 0x81, 1);
    while (WipeCheckEnd()) {
        HuPrcVSleep();
    }

    for (i = 0; i < 4; i++) {
        MB7W_S32(0x1C8 + (i * 0x5C)) = -1;
    }
    MB7Ev_KoopaWalkPlayerUpdate();
    MB7Ev_KoopaClose();
    MBCameraFocusPlayerSet(playerNo);
    MBCameraSkipSet(FALSE);
    MBCameraZoomSet(savedZoom);
    WipeCreate(1, 0x81, 0x1E);
    while (WipeCheckEnd()) {
        HuPrcVSleep();
    }
    for (i = 0; (f32) i < 30.0f; i++) {
        HuPrcVSleep();
    }
}

/* ------------------------------ dokan warp -------------------------------- */

/* NONMATCHING: register allocation order differs (57.76% match) */
static void MB7Ev_DokanSet(s32 playerNo, s32 masuId, s32 goingIn) {
    MASU *masuP = MBMasuGet(0, masuId);
    s32 pipeMdlId = mb7_Work[8];
    Point3d pos;
    f32 scaleTime = 30.0f;
    s32 i;

    MBModelDispSet((s16) pipeMdlId, TRUE);
    pos.x = masuP->pos.x;
    pos.y = masuP->pos.y;
    pos.z = masuP->pos.z;
    if (goingIn != 0) {
        pos.y -= 100.0f;
    }
    if (goingIn != 0) {
        scaleTime = 45.0f;
    }
    if (goingIn != 0) {
        MBAudFXPlay(0x618);
    }
    for (i = 0; (f32) i < scaleTime; i++) {
        f32 t = (0.4f * (f32) i) / scaleTime;
        f32 yStep = 100.0f / scaleTime;
        f32 scale;

        if (goingIn != 0) {
            pos.y += yStep;
            scale = t + 0.8f;
        } else {
            pos.y -= yStep;
            scale = 1.2f - t;
        }
        MBModelPosSetV((s16) pipeMdlId, &pos);
        MBModelScaleSet((s16) pipeMdlId, scale, 1.0f, scale);
        MB7W_F32(0x1F0) = pos.y + 100.0f;
        MBPlayerPosSetV(playerNo, (Point3d *) ((u8 *) mb7_Work + 0x1C0 + 0x2C));
        HuPrcVSleep();
    }

    if (goingIn == 0) {
        MBModelDispSet((s16) pipeMdlId, FALSE);
        MB7W_F32(0x1EC) = masuP->pos.x;
        MB7W_F32(0x1F0) = masuP->pos.y;
        MB7W_F32(0x1F4) = masuP->pos.z;
        MBPlayerPosSetV(playerNo, (Point3d *) ((u8 *) mb7_Work + 0x1C0 + 0x2C));
    }
}

/* NONMATCHING: register allocation order differs (63.79% match) */
static void MB7Ev_DokanPlayerSet(s32 playerNo, s32 masuId, s32 goingIn) {
    f32 startY = MBMasuGet(0, masuId)->pos.y;
    f32 pipeY = MB7W_F32(0x1F0);
    f32 shrinkTime = 45.0f;
    s32 i;

    if (goingIn != 0) {
        MBModelDispSet(MBPlayerModelGet(playerNo), TRUE);
        shrinkTime = 30.0f;
    } else {
        MBAudFXPlay(0x619);
    }
    for (i = 0; (f32) i < shrinkTime; i++) {
        f32 t = (f32) i * (1.0f / shrinkTime);
        f32 scale;

        if (goingIn == 0) {
            t = 1.0f - t;
        }
        scale = t * t;
        MB7W_F32(0x1F0) = startY + (t * (pipeY - startY));
        MBPlayerPosSetV(playerNo, (Point3d *) ((u8 *) mb7_Work + 0x1C0 + 0x2C));
        MBPlayerScaleSet(playerNo, scale, scale, scale);
        HuPrcVSleep();
    }

    if (goingIn == 0) {
        MBModelDispSet(MBPlayerModelGet(playerNo), FALSE);
    }
}

/* Dokan (green pipe warp) cutscene: player goes down into the pipe at
 * their masu, the koopa carries them (offscreen, via a straight fly-in
 * on the "guru_pos" bone) to the target masu found by MBMasuFlagFind,
 * then the player comes back out. mb7_Work+0x1C0+0x20 (s16) is reused
 * as a scratch player-motion id, mb7_Work+0xA0+0x88 (f32, part of the
 * rot Point3d) as the koopa's target facing angle. */
/* NONMATCHING: register allocation order differs substantially (29.70%
 * match) - many more live locals (extra callee-saved float registers)
 * than the original across this function's several sequential loops;
 * logic verified correct against the asm field-by-field, not guessed */
static void MB7Ev_Dokan(s32 playerNo) {
    u8 *koopaBase = (u8 *) mb7_Work + 0xA0;
    u8 *playerEntry = (u8 *) mb7_Work + 0x1C0;
    s16 startMasuId = GwPlayer[playerNo].masuId;
    u16 flag = (u16) MBMasuGet(0, (s32) startMasuId)->flag;
    u32 findFlag = 0x1000;
    s32 targetMasuId;
    Point3d guruPos;
    s32 i;

    MBPlayerMotIdleSet(playerNo);
    MB7Ev_KoopaInit(flag);
    if (flag & 0x800) {
        findFlag = 0x400;
    }
    targetMasuId = MBMasuFlagFind(0, findFlag, findFlag);
    Hu3DModelObjPosGet(MBModelIdGet(mb7_MainMdlId), "kupa_dream_all-guru_pos", &guruPos);
    memset(playerEntry, 0, 0x170);
    MBPlayerPosGet(playerNo, (Point3d *) (playerEntry + 0x2C));
    MBPlayerRotGet(playerNo, (Point3d *) (playerEntry + 0x38));
    *(s16 *) (playerEntry + 0x20) = (s16) MBPlayerMotionCreate(playerNo, 0x8C0061);
    MBCameraFocusPlayerSet(-1);
    MB7Ev_DokanSet(playerNo, startMasuId, 1);
    MBPlayerMotionNoShiftSet(playerNo, *(s16 *) (playerEntry + 0x20), 0.0f, 8.0f, 0);
    MB7Ev_DokanPlayerSet(playerNo, startMasuId, 0);
    MBPlayerMotionKill(playerNo, *(s16 *) (playerEntry + 0x20));
    MB7Ev_DokanSet(playerNo, startMasuId, 0);
    MB7Ev_KoopaCreate();
    MB7Ev_KoopaMotionCreate(3);
    MB7Ev_KoopaMotionCreate(2);
    MB7Ev_KoopaDataClose();
    *(s32 *) (koopaBase + 0x68) = MBEffCreate((ANIMDATA *)(uintptr_t) mb7_Work[0x24], 0x80);
    MBEffHookSet((s16) *(s32 *) (koopaBase + 0x68), MB7_EffDustHook);
    Hu3DModelLayerSet((s16) *(s32 *) (koopaBase + 0x68), 2);
    MBMasuGet(0, targetMasuId);
    GwPlayer[playerNo].masuId = (s16) targetMasuId;
    MBPlayerPosFixPlayer(playerNo, targetMasuId, TRUE);
    MBPlayerPosReset(playerNo);
    MB7W_F32(0x1FC) = 0.0f;
    MBPlayerPosGet(playerNo, (Point3d *) (playerEntry + 0x2C));
    MBPlayerRotSetV(playerNo, (Point3d *) (playerEntry + 0x38));
    MBPlayerMotIdleSet(playerNo);
    MBCameraFocusMasuSet(targetMasuId);
    MB7Ev_DokanSet(playerNo, (s16) targetMasuId, 1);
    MB7Ev_DokanPlayerSet(playerNo, (s16) targetMasuId, 1);
    MB7Ev_DokanSet(playerNo, (s16) targetMasuId, 0);

    *(Point3d *) (koopaBase + 0x78) = guruPos;
    *(f32 *) (koopaBase + 0x88) = (f32) (180.0 * (atan2((f64) (MB7W_F32(0x1EC) - guruPos.x), (f64) (MB7W_F32(0x1F4) - guruPos.z)) / 3.141592653589793));
    MBModelPosSetV((s16) *(s32 *) (koopaBase + 0x34), (Point3d *) (koopaBase + 0x78));
    MBModelRotSetV((s16) *(s32 *) (koopaBase + 0x34), (Point3d *) (koopaBase + 0x84));
    MBModelDispSet((s16) *(s32 *) (koopaBase + 0x34), TRUE);
    MB7Ev_KoopaMotionNoSet(2, 0);
    MBMotionTimeSet((s16) *(s32 *) (koopaBase + 0x34), 100.0f);
    *(f32 *) (koopaBase + 0x7C) += 1000.0f;
    MB7W_F32(0x94) = 0.0f;
    while (guruPos.y < *(f32 *) (koopaBase + 0x7C)) {
        MB7W_F32(0x94) += -0.5444445f;
        PSVECAdd((Point3d *) (koopaBase + 0x78), (Point3d *) (koopaBase + 0x90), (Point3d *) (koopaBase + 0x78));
        if (guruPos.y > *(f32 *) (koopaBase + 0x7C)) {
            *(f32 *) (koopaBase + 0x7C) = guruPos.y;
        }
        MBModelPosSetV((s16) *(s32 *) (koopaBase + 0x34), (Point3d *) (koopaBase + 0x78));
        HuPrcVSleep();
    }

    Hu3DModelPosSetV((s16) *(s32 *) (koopaBase + 0x68), (Point3d *) (koopaBase + 0x78));
    *(s16 *) Hu3DData[*(s32 *) (koopaBase + 0x68)].hookData = 1;
    MBPlayerMotionNoSet(playerNo, 9, 0);
    MB7W_F32(0x1DC) = (f32) (180.0 * (atan2((f64) (MB7W_F32(0x118) - MB7W_F32(0x1EC)), (f64) (MB7W_F32(0x120) - MB7W_F32(0x1F4))) / 3.141592653589793));
    MBCameraQuakeSet(0x12, 200.0f);
    MBAudFXPlay(0x369);
    while (!MBMotionEndCheck((s16) *(s32 *) (koopaBase + 0x34))) {
        MBAngleMoveTo((f32 *) (playerEntry + 0x3C), MB7W_F32(0x1DC), 0.15f);
        MBPlayerRotSetV(playerNo, (Point3d *) (playerEntry + 0x38));
        HuPrcVSleep();
    }

    MBPlayerMotIdleSet(playerNo);
    MBCameraFocusMasuSet(-1);
    {
        Point3d camPos;

        PSVECSubtract((Point3d *) (koopaBase + 0x78), (Point3d *) (playerEntry + 0x2C), &camPos);
        PSVECScale(&camPos, &camPos, 0.5f);
        PSVECAdd((Point3d *) (playerEntry + 0x2C), &camPos, &camPos);
        camPos.y += 100.0f;
        MBCameraPosViewSet(&camPos, NULL, NULL, 2500.0f, -1.0f, 0x3C);
    }
    MB7Ev_KoopaMotionShiftSet(3, 0);
    while (!MBMotionEndCheck((s16) *(s32 *) (koopaBase + 0x34))) {
        HuPrcVSleep();
    }

    MB7Ev_KoopaMotionShiftSet(0, 1);
    MBAudFXPlay(0x2C9);
    MBWinCreate(2, 0x330002U, 7);
    MBTopWinWait();
    MBTopWinKill();
    MB7Ev_KoopaMotionShiftSet(2, 0);
    MB7W_F32(0x94) = 16.666666f;
    for (i = 0; i < 0x64; i++) {
        if (i == 0x41) {
            MBCameraQuakeSet(0x12, 50.0f);
            Hu3DModelPosSetV((s16) *(s32 *) (koopaBase + 0x68), (Point3d *) (koopaBase + 0x78));
            *(s16 *) Hu3DData[*(s32 *) (koopaBase + 0x68)].hookData = 2;
            MBAudFXPlay(0x2CC);
        }
        if (i > 0x46) {
            MB7W_F32(0x94) += -0.27222225f;
            PSVECAdd((Point3d *) (koopaBase + 0x78), (Point3d *) (koopaBase + 0x90), (Point3d *) (koopaBase + 0x78));
            MBModelPosSetV((s16) *(s32 *) (koopaBase + 0x34), (Point3d *) (koopaBase + 0x78));
        }
        HuPrcVSleep();
    }

    MBMotionSpeedSet((s16) *(s32 *) (koopaBase + 0x34), 0.0f);
    MBEffKill((s16) *(s32 *) (koopaBase + 0x68));
    *(s32 *) (koopaBase + 0x68) = -1;
    MB7Ev_KoopaClose();
}

/* Dokan-in cutscene (player enters a pipe locally, koopa carries them
 * off-screen to the target masu, then they re-emerge there) - same
 * structure as MB7Ev_Dokan, confirmed via asm to call MB7Ev_DokanSet 4
 * times but MB7Ev_DokanPlayerSet never (its shrink/grow logic is
 * duplicated inline twice in the original source, not shared). */
/* NONMATCHING: register allocation order differs substantially, same
 * cause as MB7Ev_Dokan (more live locals than the original) */
static void MB7Ev_DokanIn(s32 playerNo) {
    u8 *koopaBase = (u8 *) mb7_Work + 0xA0;
    u8 *playerEntry = (u8 *) mb7_Work + 0x1C0;
    s16 startMasuId = GwPlayer[playerNo].masuId;
    u16 flag = (u16) MBMasuGet(0, (s32) startMasuId)->flag;
    u32 findFlag = 0x2000;
    s32 targetMasuId;
    Point3d guruPos;
    s32 i;

    MBPlayerMotIdleSet(playerNo);
    MB7Ev_KoopaInit(flag);
    if (flag & 0x400) {
        findFlag = 0x800;
    }
    targetMasuId = MBMasuFlagFind(0, findFlag, findFlag);
    MB7Ev_KoopaCreate();
    MB7Ev_KoopaMotionCreate(5);
    MB7Ev_KoopaMotionCreate(2);
    MB7Ev_KoopaDataClose();
    *(s32 *) (koopaBase + 0x68) = MBEffCreate((ANIMDATA *)(uintptr_t) mb7_Work[0x24], 0x80);
    MBEffHookSet((s16) *(s32 *) (koopaBase + 0x68), MB7_EffDustHook);
    Hu3DModelLayerSet((s16) *(s32 *) (koopaBase + 0x68), 2);
    Hu3DModelObjPosGet(MBModelIdGet(mb7_MainMdlId), "kupa_dream_all-guru_pos", &guruPos);
    memset(playerEntry, 0, 0x170);
    MBPlayerPosGet(playerNo, (Point3d *) (playerEntry + 0x2C));
    MBPlayerRotGet(playerNo, (Point3d *) (playerEntry + 0x38));
    *(s16 *) (playerEntry + 0x20) = (s16) MBPlayerMotionCreate(playerNo, 0x8C0061);

    *(Point3d *) (koopaBase + 0x78) = guruPos;
    *(f32 *) (koopaBase + 0x88) = (f32) (180.0 * (atan2((f64) (MB7W_F32(0x1EC) - guruPos.x), (f64) (MB7W_F32(0x1F4) - guruPos.z)) / 3.141592653589793));
    MBModelPosSetV((s16) *(s32 *) (koopaBase + 0x34), (Point3d *) (koopaBase + 0x78));
    MBModelRotSetV((s16) *(s32 *) (koopaBase + 0x34), (Point3d *) (koopaBase + 0x84));
    MBModelDispSet((s16) *(s32 *) (koopaBase + 0x34), TRUE);
    MB7Ev_KoopaMotionNoSet(2, 0);
    MBMotionTimeSet((s16) *(s32 *) (koopaBase + 0x34), 100.0f);
    *(f32 *) (koopaBase + 0x7C) += 1000.0f;
    MB7W_F32(0x94) = 0.0f;
    while (guruPos.y < *(f32 *) (koopaBase + 0x7C)) {
        MB7W_F32(0x94) += -0.5444445f;
        PSVECAdd((Point3d *) (koopaBase + 0x78), (Point3d *) (koopaBase + 0x90), (Point3d *) (koopaBase + 0x78));
        if (guruPos.y > *(f32 *) (koopaBase + 0x7C)) {
            *(f32 *) (koopaBase + 0x7C) = guruPos.y;
        }
        MBModelPosSetV((s16) *(s32 *) (koopaBase + 0x34), (Point3d *) (koopaBase + 0x78));
        HuPrcVSleep();
    }

    Hu3DModelPosSetV((s16) *(s32 *) (koopaBase + 0x68), (Point3d *) (koopaBase + 0x78));
    *(s16 *) Hu3DData[*(s32 *) (koopaBase + 0x68)].hookData = 1;
    MBPlayerMotionNoSet(playerNo, 9, 0);
    MB7W_F32(0x1DC) = (f32) (180.0 * (atan2((f64) (MB7W_F32(0x118) - MB7W_F32(0x1EC)), (f64) (MB7W_F32(0x120) - MB7W_F32(0x1F4))) / 3.141592653589793));
    MBCameraQuakeSet(0x12, 200.0f);
    MBAudFXPlay(0x369);
    while (!MBMotionEndCheck((s16) *(s32 *) (koopaBase + 0x34))) {
        MBAngleMoveTo((f32 *) (playerEntry + 0x3C), MB7W_F32(0x1DC), 0.15f);
        MBPlayerRotSetV(playerNo, (Point3d *) (playerEntry + 0x38));
        HuPrcVSleep();
    }

    MBPlayerMotIdleSet(playerNo);
    MBCameraFocusPlayerSet(-1);
    {
        Point3d camPos;

        PSVECSubtract((Point3d *) (koopaBase + 0x78), (Point3d *) (playerEntry + 0x2C), &camPos);
        PSVECScale(&camPos, &camPos, 0.5f);
        PSVECAdd((Point3d *) (playerEntry + 0x2C), &camPos, &camPos);
        camPos.y += 100.0f;
        MBCameraPosViewSet(&camPos, NULL, NULL, 2500.0f, -1.0f, 0x1E);
    }
    MB7Ev_KoopaMotionNoSet(5, 0);
    while (!MBMotionEndCheck((s16) *(s32 *) (koopaBase + 0x34))) {
        HuPrcVSleep();
    }

    MB7Ev_KoopaMotionShiftSet(0, 1);
    MBAudFXPlay(0x2CA);
    MBWinCreate(2, 0x330003U, 7);
    MBTopWinWait();
    MBTopWinKill();
    MB7Ev_KoopaMotionShiftSet(2, 0);
    MB7W_F32(0x94) = 16.666666f;
    for (i = 0; i < 0x64; i++) {
        if (i == 0x41) {
            MBCameraQuakeSet(0x12, 50.0f);
            Hu3DModelPosSetV((s16) *(s32 *) (koopaBase + 0x68), (Point3d *) (koopaBase + 0x78));
            *(s16 *) Hu3DData[*(s32 *) (koopaBase + 0x68)].hookData = 2;
            MBAudFXPlay(0x2CC);
        }
        if (i > 0x46) {
            MB7W_F32(0x94) += -0.27222225f;
            PSVECAdd((Point3d *) (koopaBase + 0x78), (Point3d *) (koopaBase + 0x90), (Point3d *) (koopaBase + 0x78));
            MBModelPosSetV((s16) *(s32 *) (koopaBase + 0x34), (Point3d *) (koopaBase + 0x78));
        }
        HuPrcVSleep();
    }

    MBMotionSpeedSet((s16) *(s32 *) (koopaBase + 0x34), 0.0f);
    MBCameraFocusPlayerSet(-1);
    MB7Ev_DokanSet(playerNo, startMasuId, 1);
    MBPlayerMotionNoShiftSet(playerNo, *(s16 *) (playerEntry + 0x20), 0.0f, 8.0f, 0);

    {
        MASU *startMasuP = MBMasuGet(0, (s32) startMasuId);
        f32 startY = startMasuP->pos.y;
        f32 pipeY = *(f32 *) (koopaBase + 0x7C);

        MBAudFXPlay(0x619);
        for (i = 0; i < 45; i++) {
            f32 t = 1.0f - ((f32) i * (1.0f / 45.0f));
            f32 scale = t * t;

            *(f32 *) (koopaBase + 0x7C) = startY + (t * (pipeY - startY));
            MBPlayerPosSetV(playerNo, (Point3d *) (playerEntry + 0x2C));
            MBPlayerScaleSet(playerNo, scale, scale, scale);
            HuPrcVSleep();
        }
    }

    MBModelDispSet(MBPlayerModelGet(playerNo), FALSE);
    MBPlayerMotionKill(playerNo, *(s16 *) (playerEntry + 0x20));
    MB7Ev_DokanSet(playerNo, startMasuId, 0);
    MBMasuGet(0, targetMasuId);
    GwPlayer[playerNo].masuId = (s16) targetMasuId;
    MBPlayerPosFixPlayer(playerNo, targetMasuId, TRUE);
    MBPlayerPosReset(playerNo);
    MB7W_F32(0x1FC) = 0.0f;
    MBPlayerPosGet(playerNo, (Point3d *) (playerEntry + 0x2C));
    MBPlayerRotSetV(playerNo, (Point3d *) (playerEntry + 0x38));
    MBPlayerMotIdleSet(playerNo);
    MBCameraFocusMasuSet(targetMasuId);
    MB7Ev_DokanSet(playerNo, (s16) targetMasuId, 1);

    {
        MASU *targetMasuP = MBMasuGet(0, targetMasuId);
        f32 startY = targetMasuP->pos.y;
        f32 pipeY = *(f32 *) (koopaBase + 0x7C);

        MBModelDispSet(MBPlayerModelGet(playerNo), TRUE);
        for (i = 0; i < 30; i++) {
            f32 t = (f32) i * (1.0f / 30.0f);
            f32 scale = t * t;

            *(f32 *) (koopaBase + 0x7C) = startY + (t * (pipeY - startY));
            MBPlayerPosSetV(playerNo, (Point3d *) (playerEntry + 0x2C));
            MBPlayerScaleSet(playerNo, scale, scale, scale);
            HuPrcVSleep();
        }
    }

    MB7Ev_DokanSet(playerNo, (s16) targetMasuId, 0);
    MBEffKill((s16) *(s32 *) (koopaBase + 0x68));
    *(s32 *) (koopaBase + 0x68) = -1;
    MB7Ev_KoopaClose();
}

/* per-player Hou state, reusing the same mb7_Work+0x1C0 array as
 * MB7Ev_KoopaWalkPlayerUpdate/Dokan* (only one of these cutscenes runs
 * at a time): +0x0 (s32) active gate, +0x8 (s32) state, +0xC (s32)
 * masuId, +0x10 (s32) bounce count, +0x20 (s16) motion id, +0x2C/+0x38
 * (Point3d) pos/rot, +0x30 (f32) last ground height, +0x44 (Point3d)
 * fall velocity, +0x48 (f32) fall speed, +0x3C (f32) reset scratch. */
/* NONMATCHING: register allocation order differs */
static void MB7Ev_HouPlayerUpdate(void) {
    u8 *fireBase = (u8 *) mb7_Work + 0x13C;
    s32 playerNo;

    for (playerNo = 0; playerNo < 4; playerNo++) {
        u8 *e = (u8 *) mb7_Work + 0x1C0 + (playerNo * 0x5C);

        if (*(s32 *) (e + 0x0) != 0) {
            MASU *masuP = MBMasuGet(0, *(s32 *) (e + 0xC));

            switch (*(s32 *) (e + 0x8)) {
            case 0:
                *(s16 *) (e + 0x20) = (s16) MBPlayerMotionCreate(playerNo, 0x8C0015);
                MBPlayerPosGet(playerNo, (Point3d *) (e + 0x2C));
                MBPlayerRotGet(playerNo, (Point3d *) (e + 0x38));
                *(s32 *) (e + 0x4) = -1;
                *(s32 *) (e + 0x8) += 1;
                break;
            case 1:
                if (*(f32 *) (fireBase + 0x18) >= 0.8f) {
                    f32 angle = (f32) (180.0 * (atan2((f64) (*(f32 *) (e + 0x2C) - *(f32 *) (fireBase + 0x64)), (f64) (*(f32 *) (e + 0x34) - *(f32 *) (fireBase + 0x6C))) / 3.141592653589793));

                    if (fabs(MB7_AngleDiff(angle, *(f32 *) (fireBase + 0x10))) <= 10.0f) {
                        s32 capsuleMax = MBPlayerCapsuleMaxGet();
                        s32 i;

                        for (i = 0; i < capsuleMax; i++) {
                            MBPlayerCapsuleRemove(playerNo, 0);
                        }
                        omVibrate((s16) playerNo, 0xC, 4, 2);
                        MBPlayerMotionNoShiftSet(playerNo, *(s16 *) (e + 0x20), 0.0f, 8.0f, 0x40000001U);
                        MBPlayerWinLoseVoicePlay(playerNo, 1, 0x1F9);
                        *(f32 *) (e + 0x48) = 32.666668f;
                        *(s32 *) (e + 0x10) = 0;
                        *(s32 *) (e + 0x8) += 1;
                    } else {
                        break;
                    }
                } else {
                    break;
                }
                /* fallthrough */
            case 2:
                *(f32 *) (e + 0x48) += -0.8166667f;
                PSVECAdd((Point3d *) (e + 0x2C), (Point3d *) (e + 0x44), (Point3d *) (e + 0x2C));
                if (*(f32 *) (e + 0x30) <= masuP->pos.y) {
                    *(f32 *) (e + 0x30) = masuP->pos.y;
                    *(f32 *) (e + 0x48) *= -0.7f;
                    *(s32 *) (e + 0x10) += 1;
                    omVibrate((s16) playerNo, 0xC, 4, 2);
                    if (*(s32 *) (e + 0x10) >= 2) {
                        *(f32 *) (e + 0x48) = 0.0f;
                        *(s32 *) (e + 0x8) += 1;
                        MBPlayerMotionNoShiftSet(playerNo, 6, 0.0f, 8.0f, 0x40000001U);
                    }
                }
                break;
            case -1:
                MBPlayerMotionNoSet(playerNo, 1, 0x40000001U);
                MBPlayerMotionKill(playerNo, *(s16 *) (e + 0x20));
                *(s16 *) (e + 0x20) = -1;
                *(f32 *) (e + 0x3C) = 0.0f;
                break;
            default:
                break;
            }
            MBPlayerPosSetV(playerNo, (Point3d *) (e + 0x2C));
            MBPlayerRotSetV(playerNo, (Point3d *) (e + 0x38));
        }
    }
}

/* Hou (fire breath) cutscene: player stands in front of the koopa,
 * gets a warning turn, then the koopa breathes fire in a sweeping arc
 * (4 layered MBEFFECT fire hooks) while MB7Ev_HouPlayerUpdate knocks
 * out any player standing in the breath path. See the mb7_Work+0x13C
 * "fire-breath" comment above MB7Ev_Hou's declaration for field
 * offsets; mb7_Work[2] is the koopa "dream" display model used for
 * Hu3DModelObjPosGet bone lookups, mb7_Work[9] is a small ember/nozzle
 * prop shown only while the koopa is mid-breath. koopaBase+0x28 (f32)
 * is a decaying head-bob factor for the koopa's aiming pose. */
/* NONMATCHING: register allocation order differs substantially across
 * this function's many sequential loops and its 5-phase inner state
 * machine; logic verified against the asm field-by-field */
static void MB7Ev_Hou(s32 playerNo) {
    u8 *koopaBase = (u8 *) mb7_Work + 0xA0;
    u8 *fireBase = (u8 *) mb7_Work + 0x13C;
    s16 dreamMdlId;
    s32 seId = -1;
    Point3d kupaPos;
    Point3d firePos;
    s32 i;

    MB7Ev_KoopaInit((u16) MBMasuGet(0, (s32) GwPlayer[playerNo].masuId)->flag);
    MBCameraFocusPlayerSet(-1);
    MBCameraPlayerViewSet((s16) playerNo, NULL, NULL, 1500.0f, -1.0f, 0x1E);
    MBPlayerMotionNoShiftSet(playerNo, 2, 0.0f, 8.0f, 0x40000001U);
    for (i = 0; (f32) i < 30.0f; i++) {
        Point3d rot;

        MBPlayerRotGet(playerNo, &rot);
        MBAngleMoveTo(&rot.y, 0.0f, 0.15f);
        MBPlayerRotSetV(playerNo, &rot);
        HuPrcVSleep();
    }

    memset((u8 *) mb7_Work + 0x1C0, 0, 0x170);
    for (i = 0; i < 4; i++) {
        u8 *e = (u8 *) mb7_Work + 0x1C0 + (i * 0x5C);
        s16 masuId = GwPlayer[i].masuId;

        if (MBMasuGet(0, (s32) masuId)->flag & 0x80) {
            *(s32 *) (e + 0x0) = 1;
            *(s32 *) (e + 0xC) = masuId;
        }
    }
    MB7Ev_HouPlayerUpdate();

    MB7Ev_KoopaCreate();
    MB7Ev_KoopaMotionCreate(3);
    MB7Ev_KoopaMotionCreate(2);
    MB7Ev_KoopaMotionCreate(8);
    MB7Ev_KoopaDataClose();
    MBPlayerMotionNoSet(playerNo, 9, 0);
    MBCameraQuakeSet(0x12, 200.0f);
    MBAudFXPlay(0x369);
    for (i = 0; (f32) i < 18.0f; i++) {
        HuPrcVSleep();
    }

    MBPlayerMotIdleSet(playerNo);
    memset(fireBase, 0, 0x84);
    dreamMdlId = (s16) mb7_Work[2];
    Hu3DModelObjPosGet(MBModelIdGet(dreamMdlId), "kupa_dream_all-kupa_pos", &kupaPos);
    Hu3DModelObjPosGet(MBModelIdGet(dreamMdlId), "kupa_dream_all-fire_pos", (Point3d *) (fireBase + 0x40));
    *(Point3d *) (fireBase + 0x64) = kupaPos;
    MBMotionSpeedSet((s16) mb7_Work[9], 1.0f);
    MBModelAttrSet((s16) mb7_Work[9], 0x40000001U);
    *(Point3d *) (koopaBase + 0x78) = kupaPos;
    MB7W_F32(0x118 + 0x8) -= 120.00001f;
    MBModelPosSetV((s16) *(s32 *) (koopaBase + 0x34), (Point3d *) (koopaBase + 0x78));
    MBModelDispSet((s16) *(s32 *) (koopaBase + 0x34), TRUE);
    MBCameraFocusPlayerSet(-1);
    {
        Point3d camPos = kupaPos;
        camPos.z += 500.0f;
        MBCameraPosViewSet(&camPos, NULL, NULL, 3000.0f, -1.0f, 0x3C);
    }
    MBAudFXPlay(0x2C9);
    MB7Ev_KoopaMotionShiftSet(3, 0);
    while (!MBMotionEndCheck((s16) *(s32 *) (koopaBase + 0x34))) {
        HuPrcVSleep();
    }

    MB7Ev_KoopaMotionShiftSet(0, 1);
    MBWinCreate(2, 0x330007U, 7);
    MBTopWinWait();
    MBTopWinKill();
    MB7Ev_KoopaMotionShiftSet(8, 0);
    *(s32 *) (fireBase + 0x30) = MBEffCreate((ANIMDATA *)(uintptr_t) mb7_Work[0x23], 0x100);
    MBEffHookSet((s16) *(s32 *) (fireBase + 0x30), MB7Ev_KoopaFire1Hook);
    Hu3DModelLayerSet((s16) *(s32 *) (fireBase + 0x30), 2);
    Hu3DModelPosSet((s16) *(s32 *) (fireBase + 0x30), 0.0f, 0.0f, 0.0f);
    *(s32 *) (fireBase + 0x34) = MBEffCreate((ANIMDATA *)(uintptr_t) mb7_Work[0x23], 0x40);
    MBEffHookSet((s16) *(s32 *) (fireBase + 0x34), MB7Ev_KoopaFire2Hook);
    Hu3DModelLayerSet((s16) *(s32 *) (fireBase + 0x34), 2);
    Hu3DModelPosSet((s16) *(s32 *) (fireBase + 0x34), 0.0f, 0.05f, 0.05f);
    *(s32 *) (fireBase + 0x38) = MBEffCreate((ANIMDATA *)(uintptr_t) mb7_Work[0x23], 0x40);
    MBEffHookSet((s16) *(s32 *) (fireBase + 0x38), MB7Ev_KoopaFire3Hook);
    Hu3DModelLayerSet((s16) *(s32 *) (fireBase + 0x38), 2);
    Hu3DModelPosSet((s16) *(s32 *) (fireBase + 0x38), 0.0f, -0.05f, -0.05f);
    *(s32 *) (fireBase + 0x3C) = MBEffCreate((ANIMDATA *)(uintptr_t) mb7_Work[0x23], 0x40);
    MBEffHookSet((s16) *(s32 *) (fireBase + 0x3C), MB7Ev_KoopaFire4Hook);
    Hu3DModelLayerSet((s16) *(s32 *) (fireBase + 0x3C), 2);
    Hu3DModelPosSet((s16) *(s32 *) (fireBase + 0x3C), 0.0f, 0.0f, -0.05f);

    {
    BOOL done = FALSE;
    while (!done) {
        switch (*(s32 *) (fireBase + 0x0)) {
        case 0:
            *(f32 *) (fireBase + 0x10) = 0.0f;
            *(f32 *) (fireBase + 0x14) = 0.0f;
            *(f32 *) (fireBase + 0x18) = 0.0f;
            *(s32 *) (fireBase + 0x1C) = 0;
            *(f32 *) (koopaBase + 0x28) = 1.0f;
            *(s32 *) (fireBase + 0x0) += 1;
            seId = HuAudFXEmiterPlay(0x61D, &kupaPos);
            /* fallthrough */
        case 1:
            *(f32 *) (fireBase + 0x10) += *(f32 *) (fireBase + 0x14);
            if (*(f32 *) (fireBase + 0x10) > -45.0f) {
                if (*(f32 *) (fireBase + 0x14) > -1.0f) {
                    *(f32 *) (fireBase + 0x14) += -0.02f;
                }
            } else if (*(f32 *) (fireBase + 0x10) < -80.0f && *(f32 *) (fireBase + 0x14) < -0.33333334f) {
                *(f32 *) (fireBase + 0x14) += 0.02f;
            }
            if (*(f32 *) (fireBase + 0x10) <= -100.0f) {
                *(s32 *) (fireBase + 0x1C) = 1;
                *(f32 *) (fireBase + 0x10) = -100.0f;
                *(f32 *) (fireBase + 0x14) = 0.0f;
                *(s32 *) (fireBase + 0x0) += 1;
                HuAudFXStop(seId);
                seId = HuAudFXEmiterPlay(0x61F, &firePos);
            }
            break;
        case 2:
            HuAudFXEmiterUpDate(seId, &firePos);
            *(f32 *) (fireBase + 0x18) += 0.05f * (1.0f - *(f32 *) (fireBase + 0x18));
            *(f32 *) (fireBase + 0x10) += *(f32 *) (fireBase + 0x14);
            if (*(f32 *) (fireBase + 0x10) < 0.0f) {
                if (*(f32 *) (fireBase + 0x14) < 0.5f) {
                    *(f32 *) (fireBase + 0x14) += 0.01f;
                }
            } else if (*(f32 *) (fireBase + 0x10) > 100.0f && *(f32 *) (fireBase + 0x14) > 0.33333334f) {
                *(f32 *) (fireBase + 0x14) += -0.015000001f;
            }
            if (*(f32 *) (fireBase + 0x10) >= 115.0f) {
                *(f32 *) (fireBase + 0x10) = 115.0f;
                *(f32 *) (fireBase + 0x14) = 0.0f;
                *(s32 *) (fireBase + 0x1C) = 0;
                HuAudFXStop(seId);
                *(s32 *) (fireBase + 0x0) += 1;
                seId = HuAudFXEmiterPlay(0x61D, &kupaPos);
            }
            break;
        case 3:
            *(f32 *) (fireBase + 0x18) *= 0.95f;
            *(f32 *) (fireBase + 0x10) += *(f32 *) (fireBase + 0x14);
            if (*(f32 *) (fireBase + 0x10) > 45.0f) {
                if (*(f32 *) (fireBase + 0x14) > -1.0f) {
                    *(f32 *) (fireBase + 0x14) += -0.02f;
                }
            } else if (*(f32 *) (fireBase + 0x10) < 20.0f && *(f32 *) (fireBase + 0x14) < -0.33333334f) {
                *(f32 *) (fireBase + 0x14) += 0.02f;
            }
            if (*(f32 *) (fireBase + 0x10) <= 0.0f) {
                *(f32 *) (fireBase + 0x10) = 0.0f;
                *(s32 *) (fireBase + 0x0) += 1;
                HuAudFXStop(seId);
                MBAudFXPlay(0x61E);
                done = TRUE;
            }
            break;
        default:
            done = TRUE;
            break;
        }

        *(f32 *) (fireBase + 0x50) = *(f32 *) (fireBase + 0x10);
        MBModelRotSetV(dreamMdlId, (Point3d *) (fireBase + 0x4C));
        Hu3DModelObjPosGet(MBModelIdGet(dreamMdlId), "kupa_dream_all-kupa_pos", &kupaPos);
        Hu3DModelObjPosGet(MBModelIdGet(dreamMdlId), "kupa_dream_all-fire_pos", (Point3d *) (fireBase + 0x40));
        *(Point3d *) (fireBase + 0x64) = kupaPos;
        *(f32 *) (koopaBase + 0x28) *= 0.93f;
        kupaPos.x += ((-50.0f * *(f32 *) (koopaBase + 0x28)) - 50.0f) * (f32) sin((3.141592653589793 * (f64) *(f32 *) (fireBase + 0x50)) / 180.0);
        kupaPos.z += ((-50.0f * *(f32 *) (koopaBase + 0x28)) - 50.0f) * (f32) cos((3.141592653589793 * (f64) *(f32 *) (fireBase + 0x50)) / 180.0);
        MBModelPosSetV((s16) *(s32 *) (koopaBase + 0x34), &kupaPos);
        MBModelRotSetV((s16) *(s32 *) (koopaBase + 0x34), (Point3d *) (fireBase + 0x4C));

        if (*(f32 *) (fireBase + 0x18) <= 0.0f || *(s32 *) (fireBase + 0x1C) == 0) {
            MBModelDispSet((s16) mb7_Work[9], FALSE);
        } else {
            MBModelDispSet((s16) mb7_Work[9], TRUE);
            MBModelPosSet((s16) mb7_Work[9], *(f32 *) (fireBase + 0x40), *(f32 *) (fireBase + 0x44) - 60.000004f, *(f32 *) (fireBase + 0x48));
            MBModelRotSetV((s16) mb7_Work[9], (Point3d *) (fireBase + 0x4C));
            MBModelScaleSet((s16) mb7_Work[9], *(f32 *) (fireBase + 0x18), *(f32 *) (fireBase + 0x18), *(f32 *) (fireBase + 0x18));
        }
        firePos.x = 250.0f * (*(f32 *) (fireBase + 0x18) * (f32) sin((3.141592653589793 * (f64) *(f32 *) (fireBase + 0x50)) / 180.0));
        firePos.z = 250.0f * (*(f32 *) (fireBase + 0x18) * (f32) cos((3.141592653589793 * (f64) *(f32 *) (fireBase + 0x50)) / 180.0));
        firePos.y = *(f32 *) (fireBase + 0x44);
        MB7Ev_HouPlayerUpdate();

        {
            BOOL anyBusy = FALSE;

            for (i = 0; i < 4; i++) {
                u8 *e = (u8 *) mb7_Work + 0x1C0 + (i * 0x5C);

                if (*(s32 *) (e + 0x0) != 0 && *(s32 *) (e + 0x8) < 3) {
                    anyBusy = TRUE;
                }
            }
            if (anyBusy) {
                done = FALSE;
            }
        }
        HuPrcVSleep();
    }
    }

    MBEffKill((s16) *(s32 *) (fireBase + 0x30));
    MBEffKill((s16) *(s32 *) (fireBase + 0x34));
    MBEffKill((s16) *(s32 *) (fireBase + 0x38));
    MBEffKill((s16) *(s32 *) (fireBase + 0x3C));
    *(s32 *) (fireBase + 0x3C) = -1;
    *(s32 *) (fireBase + 0x38) = -1;
    *(s32 *) (fireBase + 0x34) = -1;
    *(s32 *) (fireBase + 0x30) = -1;
    MB7Ev_KoopaMotionShiftSet(0, 1);
    MBAudFXPlay(0x2C9);
    MBWinCreate(2, 0x330008U, 7);
    MBTopWinWait();
    MBTopWinKill();
    MB7Ev_KoopaMotionShiftSet(2, 0);
    {
        Point3d pos = kupaPos;
        pos.z -= 80.0f;
        for (i = 0; i < 0x64; i++) {
            kupaPos.z += 0.15f * (pos.z - kupaPos.z);
            MBModelPosSetV((s16) *(s32 *) (koopaBase + 0x34), &kupaPos);
            if (i == 0x41) {
                MBCameraQuakeSet(0x12, 50.0f);
                MBAudFXPlay(0x2CC);
            }
            HuPrcVSleep();
        }
    }

    MBMotionSpeedSet((s16) *(s32 *) (koopaBase + 0x34), 0.0f);
    WipeColorSet(0, 0, 0);
    WipeCreate(2, 0x81, 1);
    while (WipeCheckEnd()) {
        HuPrcVSleep();
    }

    for (i = 0; i < 4; i++) {
        u8 *e = (u8 *) mb7_Work + 0x1C0 + (i * 0x5C);

        if (*(s32 *) (e + 0x0) != 0) {
            *(s32 *) (e + 0x8) = -1;
        }
    }
    MB7Ev_HouPlayerUpdate();
    MB7Ev_KoopaClose();
    MBCameraFocusPlayerSet(playerNo);
    MBCameraSkipSet(FALSE);
    MBCameraZoomSet(2000.0f);
    WipeCreate(1, 0x81, 0x1E);
    while (WipeCheckEnd()) {
        HuPrcVSleep();
    }
    for (i = 0; (f32) i < 30.0f; i++) {
        HuPrcVSleep();
    }
}

/* Per-frame Koopa path-walk state machine, called every frame from
 * MB7Ev_KoopaWalk's loops. Koopa base (koopaBase = mb7_Work+0xA0)
 * fields used here (word-index N in the m2c draft -> byte N*4 -> minus
 * 0xA0 for the relative offset used below - re-derived from the asm,
 * not trusted blindly from the draft, since m2c's word-vs-byte
 * addressing style differs function to function):
 *   +0x0 (s32) "arrived, waiting for next direction" flag
 *   +0x4 (s32) next-direction flag (copied to +0x0 once consumed)
 *   +0x8 (s32) coin-lose cutscene lock (shared with
 *     MB7Ev_KoopaWalkCoinLose/PlayerUpdate)
 *   +0xC (s32) step index (-1 = idle, 13 = finished)
 *   +0x10 (s32) current path masu id
 *   +0x14 (s32) target masu id (the player's masu)
 *   +0x18 (s32) "snap to facing" request flag
 *   +0x1C (s32) "just landed" flag
 *   +0x20 (s32) per-step sub-state
 *   +0x24 (f32) per-step timer
 *   +0x28 (f32) per-step duration/rate
 *   +0x2C (f32) precomputed jump gravity term
 *   +0x30 (f32) target facing angle
 *   +0x34 (s32) koopa model id
 *   +0x78/+0x7C/+0x80 (pos), +0x84/+0x88/+0x8C (rot, +0x88 = facing
 *     angle), +0x90/+0x94/+0x98 (velocity/direction scratch)
 * mb7_Work[0xA] is the "red flag" marker model (one of the 11 front
 * objects), shown when the koopa turns a masu red on arrival. */
/* NONMATCHING: register allocation order differs substantially across
 * this function's 13-step state machine; logic verified against the
 * asm field-by-field (word-index vs koopa-relative byte offsets are
 * easy to conflate here, so every offset below was re-derived, not
 * copied from the m2c draft's addressing) */
static void MB7Ev_KoopaWalkUpdate(void) {
    u8 *koopaBase = (u8 *) mb7_Work + 0xA0;
    MASU *curMasuP = NULL;
    MASU *targetMasuP = NULL;
    s16 koopaMdlId = (s16) *(s32 *) (koopaBase + 0x34);
    s32 arrivedFlag = 0;
    s32 landedFlag = 0;

    if (*(s32 *) (koopaBase + 0x10) > 0) {
        curMasuP = MBMasuGet(0, *(s32 *) (koopaBase + 0x10));
    }
    if (*(s32 *) (koopaBase + 0x14) > 0) {
        targetMasuP = MBMasuGet(0, *(s32 *) (koopaBase + 0x14));
    }
    MB7Ev_KoopaWalkFxPlay();

    if (*(s32 *) (koopaBase + 0x18) != 0) {
        if (*(s32 *) (koopaBase + 0x20) == 0) {
            f32 diff;

            *(f32 *) (koopaBase + 0x30) = (f32) (180.0 * (atan2((f64) (targetMasuP->pos.x - MB7W_F32(0x118)), (f64) (targetMasuP->pos.z - MB7W_F32(0x120))) / 3.141592653589793));
            diff = (f32) fabs(MB7_AngleDiff(MB7W_F32(0x128), *(f32 *) (koopaBase + 0x30)));
            if (diff > 20.0f) {
                *(f32 *) (koopaBase + 0x28) = 1.0f / (0.33333334f * diff);
                *(f32 *) (koopaBase + 0x24) = 0.0f;
                MB7Ev_KoopaMotionShiftSet(1, 1);
                MBMotionSpeedSet(koopaMdlId, 2.0f);
                *(s32 *) (koopaBase + 0x20) += 1;
            }
        }
        if (*(s32 *) (koopaBase + 0x20) > 0) {
            f32 t;

            *(f32 *) (koopaBase + 0x24) += 1.0f;
            t = *(f32 *) (koopaBase + 0x24) * *(f32 *) (koopaBase + 0x28);
            if (t > 1.0f) {
                t = 1.0f;
            }
            MBAngleMoveTo((f32 *) (koopaBase + 0x88), *(f32 *) (koopaBase + 0x30), 0.15f * t);
            if (fabs(MB7_AngleDiff(MB7W_F32(0x128), *(f32 *) (koopaBase + 0x30))) < 5.0f) {
                *(s32 *) (koopaBase + 0x18) = 0;
                *(s32 *) (koopaBase + 0x20) = 0;
            }
            MBModelPosSetV(koopaMdlId, (Point3d *) (koopaBase + 0x78));
            MBModelRotSetV(koopaMdlId, (Point3d *) (koopaBase + 0x84));
            return;
        }
    }

    switch (*(s32 *) (koopaBase + 0xC) + 1) {
    case 1: {
        s32 side = 0;

        if (targetMasuP->pos.x < 0.0f) {
            side = 1;
        }
        mb7_SaveWorkP[side] = 2;
        *(Point3d *) (koopaBase + 0x78) = mb7ev_KoopaWalkStartPosTbl[side];
        *(s32 *) (koopaBase + 0xC) = 1;
        break;
    }
    case 2:
        if (*(s32 *) (koopaBase + 0x20) == 0) {
            MBModelDispSet(koopaMdlId, TRUE);
            MB7Ev_KoopaMotionShiftSet(1, 1);
            MBMotionSpeedSet(koopaMdlId, 2.0f);
            PSVECSubtract(&targetMasuP->pos, (Point3d *) (koopaBase + 0x78), (Point3d *) (koopaBase + 0x90));
            *(f32 *) (koopaBase + 0x88) = (f32) (180.0 * (atan2((f64) MB7W_F32(0x130), (f64) MB7W_F32(0x138)) / 3.141592653589793));
            *(f32 *) (koopaBase + 0x30) = *(f32 *) (koopaBase + 0x88);
            *(f32 *) (koopaBase + 0x28) = 0.120000005f * PSVECMag((Point3d *) (koopaBase + 0x90));
            PSVECScale((Point3d *) (koopaBase + 0x90), (Point3d *) (koopaBase + 0x90), 1.0f / *(f32 *) (koopaBase + 0x28));
            *(f32 *) (koopaBase + 0x24) = 0.0f;
            *(s32 *) (koopaBase + 0x20) += 1;
        } else {
            *(f32 *) (koopaBase + 0x24) += 1.0f;
            MBAngleMoveTo((f32 *) (koopaBase + 0x88), *(f32 *) (koopaBase + 0x30), 0.15f);
            PSVECAdd((Point3d *) (koopaBase + 0x78), (Point3d *) (koopaBase + 0x90), (Point3d *) (koopaBase + 0x78));
            if (*(f32 *) (koopaBase + 0x24) >= 0.68f * *(f32 *) (koopaBase + 0x28)) {
                *(s32 *) (koopaBase + 0x4) = 1;
                *(s32 *) (koopaBase + 0xC) = 3;
                *(s32 *) (koopaBase + 0x20) = 0;
            }
        }
        break;
    case 3:
        if (*(s32 *) (koopaBase + 0x20) == 0) {
            MB7Ev_KoopaMotionShiftSet(1, 1);
            MBMotionSpeedSet(koopaMdlId, 2.0f);
            PSVECSubtract(&targetMasuP->pos, (Point3d *) (koopaBase + 0x78), (Point3d *) (koopaBase + 0x90));
            *(f32 *) (koopaBase + 0x30) = (f32) (180.0 * (atan2((f64) MB7W_F32(0x130), (f64) MB7W_F32(0x138)) / 3.141592653589793));
            *(f32 *) (koopaBase + 0x28) = 0.15f * PSVECMag((Point3d *) (koopaBase + 0x90));
            PSVECScale((Point3d *) (koopaBase + 0x90), (Point3d *) (koopaBase + 0x90), 1.0f / *(f32 *) (koopaBase + 0x28));
            *(f32 *) (koopaBase + 0x24) = 0.0f;
            *(s32 *) (koopaBase + 0x20) += 1;
        } else {
            f32 t = 0.06666667f * *(f32 *) (koopaBase + 0x24);

            *(f32 *) (koopaBase + 0x24) += 1.0f;
            if (t > 1.0f) {
                t = 1.0f;
            }
            MBAngleMoveTo((f32 *) (koopaBase + 0x88), *(f32 *) (koopaBase + 0x30), 0.15f * t);
            PSVECAdd((Point3d *) (koopaBase + 0x78), (Point3d *) (koopaBase + 0x90), (Point3d *) (koopaBase + 0x78));
            if ((*(f32 *) (koopaBase + 0x28) - *(f32 *) (koopaBase + 0x24)) < 15.0f) {
                arrivedFlag = 1;
            }
            if (*(f32 *) (koopaBase + 0x24) >= *(f32 *) (koopaBase + 0x28)) {
                landedFlag = 1;
            }
        }
        break;
    case 4:
        if (*(s32 *) (koopaBase + 0x20) == 0) {
            MB7Ev_KoopaMotionShiftSet(6, 0);
            *(s32 *) (koopaBase + 0x1C) = 0;
            *(f32 *) (koopaBase + 0x24) = 0.0f;
            *(s32 *) (koopaBase + 0x20) += 1;
        } else if (*(s32 *) (koopaBase + 0x20) == 1) {
            *(f32 *) (koopaBase + 0x24) += 1.0f;
            if (*(f32 *) (koopaBase + 0x24) >= 25.0f) {
                PSVECSubtract(&targetMasuP->pos, (Point3d *) (koopaBase + 0x78), (Point3d *) (koopaBase + 0x90));
                *(f32 *) (koopaBase + 0x30) = (f32) (180.0 * (atan2((f64) MB7W_F32(0x130), (f64) MB7W_F32(0x138)) / 3.141592653589793));
                MB7W_F32(0x130) /= 1.0f;
                MB7W_F32(0x138) /= 1.0f;
                MB7W_F32(0x134) = (MB7W_F32(0x134) / 1.0f) - (0.5f * 1.0f * -4900.0f);
                PSVECScale((Point3d *) (koopaBase + 0x90), (Point3d *) (koopaBase + 0x90), 0.016666668f);
                *(f32 *) (koopaBase + 0x28) = 60.0f * 1.0f;
                *(f32 *) (koopaBase + 0x2C) = 0.016666668f * (0.016666668f * -4900.0f);
                *(f32 *) (koopaBase + 0x24) = 0.0f;
                *(s32 *) (koopaBase + 0x20) += 1;
            }
        } else if (*(s32 *) (koopaBase + 0x20) == 2) {
            *(f32 *) (koopaBase + 0x24) += 1.0f;
            MBAngleMoveTo((f32 *) (koopaBase + 0x88), *(f32 *) (koopaBase + 0x30), 0.15f);
            MB7W_F32(0x134) += *(f32 *) (koopaBase + 0x2C);
            PSVECAdd((Point3d *) (koopaBase + 0x78), (Point3d *) (koopaBase + 0x90), (Point3d *) (koopaBase + 0x78));
            if (*(f32 *) (koopaBase + 0x24) > 3.0f) {
                MBMotionSpeedSet(koopaMdlId, 0.95f * MBMotionSpeedGet(koopaMdlId));
            }
            if (MBMotionTimeGet(koopaMdlId) > 40.0f) {
                MBMotionSpeedSet(koopaMdlId, 0.0f);
                *(s32 *) (koopaBase + 0x20) += 1;
            }
        } else if (*(s32 *) (koopaBase + 0x20) == 3) {
            *(f32 *) (koopaBase + 0x24) += 1.0f;
            MBAngleMoveTo((f32 *) (koopaBase + 0x88), *(f32 *) (koopaBase + 0x30), 0.15f);
            MB7W_F32(0x134) += *(f32 *) (koopaBase + 0x2C);
            PSVECAdd((Point3d *) (koopaBase + 0x78), (Point3d *) (koopaBase + 0x90), (Point3d *) (koopaBase + 0x78));
            if ((*(f32 *) (koopaBase + 0x28) - *(f32 *) (koopaBase + 0x24)) < 10.0f) {
                MBMotionSpeedSet(koopaMdlId, 1.0f);
                *(s32 *) (koopaBase + 0x1C) = 1;
            }
            if (MB7W_F32(0x134) < 0.0f) {
                if ((MB7W_F32(0x11C) - targetMasuP->pos.y) < 200.0f) {
                    *(s32 *) (koopaBase + 0x1C) = 1;
                }
                if (MB7W_F32(0x11C) <= targetMasuP->pos.y) {
                    MB7W_F32(0x11C) = targetMasuP->pos.y;
                    *(s32 *) (koopaBase + 0x20) += 1;
                    MBCameraQuakeSet(0xC, 50.0f);
                    MBAudFXPlay(0x369);
                    MB7Ev_KoopaWalkDustCreate(0);
                    arrivedFlag = 1;
                }
            }
        } else if (*(s32 *) (koopaBase + 0x20) == 4) {
            MBAngleMoveTo((f32 *) (koopaBase + 0x88), *(f32 *) (koopaBase + 0x30), 0.15f);
            if (MBMotionEndCheck(koopaMdlId) != 0) {
                *(s32 *) (koopaBase + 0x1C) = 0;
                landedFlag = 1;
            }
        }
        break;
    case 5:
        if (*(s32 *) (koopaBase + 0x20) == 0) {
            MB7Ev_KoopaMotionShiftSet(7, 0);
            *(f32 *) (koopaBase + 0x24) = 0.0f;
            *(s32 *) (koopaBase + 0x20) += 1;
        } else if (*(s32 *) (koopaBase + 0x20) == 1) {
            *(f32 *) (koopaBase + 0x24) += 1.0f;
            if (*(f32 *) (koopaBase + 0x24) >= 10.0f) {
                PSVECSubtract(&targetMasuP->pos, (Point3d *) (koopaBase + 0x78), (Point3d *) (koopaBase + 0x90));
                *(f32 *) (koopaBase + 0x30) = (f32) (180.0 * (atan2((f64) MB7W_F32(0x130), (f64) MB7W_F32(0x138)) / 3.141592653589793));
                MB7W_F32(0x130) /= 0.6666667f;
                MB7W_F32(0x138) /= 0.6666667f;
                MB7W_F32(0x134) = (MB7W_F32(0x134) / 0.6666667f) - (0.5f * 0.6666667f * -4900.0f);
                PSVECScale((Point3d *) (koopaBase + 0x90), (Point3d *) (koopaBase + 0x90), 0.016666668f);
                *(f32 *) (koopaBase + 0x28) = 60.0f * 0.6666667f;
                *(f32 *) (koopaBase + 0x2C) = 0.016666668f * (0.016666668f * -4900.0f);
                *(f32 *) (koopaBase + 0x24) = 0.0f;
                *(s32 *) (koopaBase + 0x20) += 1;
            }
        } else if (*(s32 *) (koopaBase + 0x20) == 2) {
            f32 t = 0.06666667f * *(f32 *) (koopaBase + 0x24);

            *(f32 *) (koopaBase + 0x24) += 1.0f;
            if (t > 1.0f) {
                t = 1.0f;
            }
            MBAngleMoveTo((f32 *) (koopaBase + 0x88), *(f32 *) (koopaBase + 0x30), 0.15f * t);
            MB7W_F32(0x134) += *(f32 *) (koopaBase + 0x2C);
            PSVECAdd((Point3d *) (koopaBase + 0x78), (Point3d *) (koopaBase + 0x90), (Point3d *) (koopaBase + 0x78));
            if (MB7W_F32(0x134) < 0.0f) {
                if (MB7W_F32(0x11C) <= targetMasuP->pos.y) {
                    MB7W_F32(0x11C) = targetMasuP->pos.y;
                    *(s32 *) (koopaBase + 0x20) += 1;
                    MBCameraQuakeSet(0xC, 30.000002f);
                    MBAudFXPlay(0x369);
                    MB7Ev_KoopaWalkDustCreate(1);
                    arrivedFlag = 1;
                }
            }
        } else if (*(s32 *) (koopaBase + 0x20) == 3) {
            MBAngleMoveTo((f32 *) (koopaBase + 0x88), *(f32 *) (koopaBase + 0x30), 0.15f);
            if (MB7Ev_KoopaMotionCheck() != 0) {
                landedFlag = 1;
            }
        }
        break;
    case 6:
        if (*(s32 *) (koopaBase + 0x20) == 0) {
            MB7Ev_KoopaMotionShiftSet(3, 0);
            MBAudFXPlay(0x2C9);
            *(f32 *) (koopaBase + 0x24) = 0.0f;
            *(s32 *) (koopaBase + 0x20) += 1;
        } else if (*(s32 *) (koopaBase + 0x20) == 1 && *(s32 *) (koopaBase + 0x8) == 0 && MB7Ev_KoopaMotionCheck() != 0) {
            landedFlag = 1;
        }
        break;
    case 12:
        if (*(s32 *) (koopaBase + 0x20) == 0) {
            s32 side = 0;

            MB7Ev_KoopaMotionShiftSet(1, 1);
            MBMotionSpeedSet(koopaMdlId, 2.0f);
            if (targetMasuP->pos.x < 0.0f) {
                side = 1;
            }
            PSVECSubtract((Point3d *) &mb7ev_KoopaWalkStartPosTbl[side], (Point3d *) (koopaBase + 0x78), (Point3d *) (koopaBase + 0x90));
            *(f32 *) (koopaBase + 0x30) = (f32) (180.0 * (atan2((f64) MB7W_F32(0x130), (f64) MB7W_F32(0x138)) / 3.141592653589793));
            *(f32 *) (koopaBase + 0x28) = 0.120000005f * PSVECMag((Point3d *) (koopaBase + 0x90));
            PSVECScale((Point3d *) (koopaBase + 0x90), (Point3d *) (koopaBase + 0x90), 1.0f / *(f32 *) (koopaBase + 0x28));
            *(f32 *) (koopaBase + 0x24) = 0.0f;
            *(s32 *) (koopaBase + 0x20) += 1;
        } else if (*(s32 *) (koopaBase + 0x20) == 1) {
            f32 t = 0.033333335f * *(f32 *) (koopaBase + 0x24);

            *(f32 *) (koopaBase + 0x24) += 1.0f;
            if (t > 1.0f) {
                t = 1.0f;
            }
            MBAngleMoveTo((f32 *) (koopaBase + 0x88), *(f32 *) (koopaBase + 0x30), 0.15f * t);
            if (fabs(MB7_AngleDiff(MB7W_F32(0x128), *(f32 *) (koopaBase + 0x30))) < 20.0f) {
                *(s32 *) (koopaBase + 0x20) += 1;
            }
        } else if (*(s32 *) (koopaBase + 0x20) == 2) {
            *(f32 *) (koopaBase + 0x24) += 1.0f;
            MBAngleMoveTo((f32 *) (koopaBase + 0x88), *(f32 *) (koopaBase + 0x30), 0.15f);
            PSVECAdd((Point3d *) (koopaBase + 0x78), (Point3d *) (koopaBase + 0x90), (Point3d *) (koopaBase + 0x78));
            if (*(f32 *) (koopaBase + 0x24) >= *(f32 *) (koopaBase + 0x28)) {
                *(s32 *) (koopaBase + 0xC) += 1;
                *(s32 *) (koopaBase + 0x20) = 0;
            }
        }
        break;
    case 13:
        *(s32 *) (koopaBase + 0xC) = -1;
        break;
    default:
        break;
    }

    if (arrivedFlag != 0 && MBMasuTypeGet(0, *(s32 *) (koopaBase + 0x14)) == 1) {
        s16 flagMdlId = (s16) mb7_Work[0xA];

        MBMasuTypeSet(0, *(s32 *) (koopaBase + 0x14), 2);
        MB7Ev_MasuRedSet(*(s32 *) (koopaBase + 0x14));
        MBAudFXPlay(0x34B);
        MBModelPosSetV(flagMdlId, &targetMasuP->pos);
        MBModelDispSet(flagMdlId, TRUE);
        MBMotionTimeSet(flagMdlId, 0.0f);
        MBMotionSpeedSet(flagMdlId, 1.0f);
    }

    MBModelPosSetV(koopaMdlId, (Point3d *) (koopaBase + 0x78));
    MBModelRotSetV(koopaMdlId, (Point3d *) (koopaBase + 0x84));

    if (landedFlag != 0 && targetMasuP != NULL) {
        s32 order;
        MASU *fromMasuP;

        if (*(s32 *) (koopaBase + 0x0) != 0) {
            *(s32 *) (koopaBase + 0xC) = 5;
            *(s32 *) (koopaBase + 0x20) = 0;
            *(s32 *) (koopaBase + 0x0) = 0;
            return;
        }
        *(s32 *) (koopaBase + 0x0) = *(s32 *) (koopaBase + 0x4);
        *(s32 *) (koopaBase + 0x4) = 0;
        if (*(s32 *) (koopaBase + 0x0) == 0) {
            MBCameraFocusMasuSet(-1);
            MBCameraFocusSet(koopaMdlId);
        } else {
            MBCameraFocusSet(-1);
            MBCameraFocusMasuSet(*(s32 *) (koopaBase + 0x14));
        }
        if (curMasuP != NULL && (targetMasuP->flag & 0x8000)) {
            *(s32 *) (koopaBase + 0xC) = 0xA;
            *(s32 *) (koopaBase + 0x20) = 0;
            return;
        }

        *(s32 *) (koopaBase + 0x10) = *(s32 *) (koopaBase + 0x14);
        fromMasuP = targetMasuP;
        order = 0;
        do {
            s32 linkIdx;
            MASU *scanFrom = targetMasuP;

            order += 1;
            for (linkIdx = 0; linkIdx < (s32) scanFrom->linkNum; linkIdx++) {
                *(s32 *) (koopaBase + 0x14) = scanFrom->linkTbl[linkIdx];
                targetMasuP = MBMasuGet(0, *(s32 *) (koopaBase + 0x14));
                if (targetMasuP->flag & 0x4000) {
                    break;
                }
            }
        } while ((targetMasuP->flag & 0x80000) || (targetMasuP->flag & 0x40000));

        {
            s32 i;
            for (i = 0; i < 4; i++) {
                if (*(s32 *) (koopaBase + 0x14) == GwPlayer[i].masuId) {
                    order = 2;
                    *(s32 *) (koopaBase + 0x4) = 1;
                }
            }
        }

        if (order > 1) {
            *(s32 *) (koopaBase + 0xC) = 3;
            *(s32 *) (koopaBase + 0x18) = 1;
        } else if ((fromMasuP->flag & 0x20000) && (targetMasuP->flag & 0x10000)) {
            *(s32 *) (koopaBase + 0xC) = 4;
            *(s32 *) (koopaBase + 0x18) = 1;
        } else if ((fromMasuP->flag & 0x80000) && (targetMasuP->flag & 0x40000)) {
            *(s32 *) (koopaBase + 0xC) = 3;
            *(s32 *) (koopaBase + 0x18) = 1;
        } else {
            *(s32 *) (koopaBase + 0xC) = 2;
        }
        *(s32 *) (koopaBase + 0x20) = 0;
    }
}

/* Koopa Cube event: the koopa flies the player to a masu with a
 * "choice" flag (0x60) on a linked masu, dangles them over 2 target
 * cubes, lets them (or a coin-flip for AI, GwPlayer[].comF) pick a
 * side, then either they win (bonus coins via the animated result
 * model at mb7_Work[6]) or lose (coins taken, tossed onto a random
 * masu with flag 0x80000000). Reuses the asiba fly/aim physics fields
 * already documented (koopaBase+0x28/0x30/0x88/0x94/0x78) via
 * MB7Ev_KoopaAsibaUpdate, and the player-array's +0x1C angle scratch
 * and a new +0x44 Point3d "fly velocity" (playerEntry+0x44/+0x48/+0x4C,
 * confirmed by mb7_Work[0x81..0x83] in the m2c draft matching those
 * exact byte offsets). mb7_Work[3..7] are 5 of the 11 "front" object
 * models reused here: [3] a floor/crane prop, [4]/[5] the two result
 * cubes, [6] the win-result model, [7] the koopa's carry/hook model
 * (attached to the koopa's "hikou_pos" bone via MBModelHookSet). */
/* NONMATCHING: register allocation order differs substantially across
 * this function's many sequential loops and its win/lose branches;
 * logic verified against the m2c draft field-by-field with the same
 * word-index -> byte-offset re-derivation used for the other Koopa
 * state machines in this file */
static void MB7Ev_KoopaCube(s32 playerNo) {
    u8 *koopaBase = (u8 *) mb7_Work + 0xA0;
    u8 *playerEntry = (u8 *) mb7_Work + 0x1C0;
    s16 startMasuId = GwPlayer[playerNo].masuId;
    MASU *startMasuP = MBMasuGet(0, (s32) startMasuId);
    s32 chosenMasuId = -1;
    MASU *chosenMasuP = NULL;
    s32 i;

    for (i = 0; i < (s32) startMasuP->linkNum; i++) {
        chosenMasuP = MBMasuGet(0, (s32) startMasuP->linkTbl[i]);
        if (chosenMasuP->flag & 0x60) {
            chosenMasuId = startMasuP->linkTbl[i];
            break;
        }
    }
    if (chosenMasuId < 0) {
        return;
    }

    {
        s32 side = (chosenMasuP->pos.x >= 0.0f) ? 1 : 0;
        s32 pickedCube;
        Point3d camRot;
        Point3d camTarget;
        Point3d resultPos;
        s16 flyMdlId;
        s16 cubeMdlId;

        MB7Ev_KoopaInit((u16) startMasuP->flag);
        memset(playerEntry, 0, 0x170);
        MBPlayerPosGet(playerNo, (Point3d *) (playerEntry + 0x2C));
        MBPlayerRotGet(playerNo, (Point3d *) (playerEntry + 0x38));
        MBPlayerMotionNoShiftSet(playerNo, 2, 0.0f, 8.0f, 0x40000001U);
        *(f32 *) (playerEntry + 0x1C) = (f32) (180.0 * (atan2((f64) (chosenMasuP->pos.x - MB7W_F32(0x1EC)), (f64) (chosenMasuP->pos.z - MB7W_F32(0x1F4))) / 3.141592653589793));
        for (i = 0; (f32) i < 30.0f; i++) {
            MBAngleMoveTo((f32 *) (playerEntry + 0x3C), *(f32 *) (playerEntry + 0x1C), 0.15f);
            MBPlayerRotSetV(playerNo, (Point3d *) (playerEntry + 0x38));
            HuPrcVSleep();
        }

        MB7Ev_KoopaCreate();
        MB7Ev_KoopaMotionCreate(3);
        MB7Ev_KoopaDataClose();
        PSVECSubtract(&chosenMasuP->pos, (Point3d *) (playerEntry + 0x2C), (Point3d *) (playerEntry + 0x44));
        *(f32 *) (playerEntry + 0x14) = 0.060000002f * PSVECMag((Point3d *) (playerEntry + 0x44));
        PSVECScale((Point3d *) (playerEntry + 0x44), (Point3d *) (playerEntry + 0x44), 1.0f / *(f32 *) (playerEntry + 0x14));
        *(f32 *) (playerEntry + 0x3C) = *(f32 *) (playerEntry + 0x1C);
        MBPlayerMotionNoShiftSet(playerNo, 3, 0.0f, 8.0f, 0x40000001U);
        MBPlayerPosFixPlayer(playerNo, chosenMasuId, FALSE);
        for (i = 0; (f32) i < *(f32 *) (playerEntry + 0x14); i++) {
            PSVECAdd((Point3d *) (playerEntry + 0x2C), (Point3d *) (playerEntry + 0x44), (Point3d *) (playerEntry + 0x2C));
            MBPlayerPosSetV(playerNo, (Point3d *) (playerEntry + 0x2C));
            MBPlayerRotSetV(playerNo, (Point3d *) (playerEntry + 0x38));
            HuPrcVSleep();
        }

        MBPlayerMotIdleSet(playerNo);
        flyMdlId = (s16) mb7_Work[7];
        MBMotionSpeedSet(flyMdlId, 1.0f);
        MBModelAttrSet(flyMdlId, 0x40000001U);
        MBModelDispSet(flyMdlId, TRUE);
        MBModelHookSet(flyMdlId, "hikou_pos", (s16) *(s32 *) (koopaBase + 0x34));

        *(Point3d *) (koopaBase + 0x78) = mb7ev_KoopaAsibaPosTbl[side];
        MB7W_F32(0x11C) += 500.0f;
        *(f32 *) (koopaBase + 0x28) = 120.0f;
        MB7W_F32(0x134) = -1000.0f / *(f32 *) (koopaBase + 0x28);
        {
            f32 angle = 180.0f + *(f32 *) (playerEntry + 0x1C);

            *(f32 *) (koopaBase + 0x30) = angle;
            *(f32 *) (koopaBase + 0x88) = angle;
        }
        MBModelDispSet((s16) *(s32 *) (koopaBase + 0x34), TRUE);
        MBCameraFocusPlayerSet(-1);
        {
            Point3d camPos;

            PSVECSubtract((Point3d *) &mb7ev_KoopaAsibaPosTbl[side], (Point3d *) (playerEntry + 0x2C), &camPos);
            PSVECScale(&camPos, &camPos, 0.75f);
            PSVECAdd((Point3d *) (playerEntry + 0x2C), &camPos, &camPos);
            camPos.y -= 350.0f;
            MBCameraPosViewSet(&camPos, NULL, NULL, 3000.0f, -1.0f, 0x78);
        }
        MBCameraRotGet(&camRot);

        for (i = 0; (f32) i < *(f32 *) (koopaBase + 0x28); i++) {
            if ((f32) i > (0.7f * *(f32 *) (koopaBase + 0x28))) {
                MB7W_F32(0x134) *= 0.9f;
            }
            MB7Ev_KoopaAsibaUpdate();
            HuPrcVSleep();
        }
        MB7W_F32(0x134) = 0.0f;
        MB7Ev_KoopaMotionShiftSet(3, 0);
        MBAudFXPlay(0x2C9);
        MBWinCreate(2, 0x330004U, 7);
        MBTopWinWait();
        MBTopWinKill();
        while (!MBMotionEndCheck((s16) *(s32 *) (koopaBase + 0x34))) {
            HuPrcVSleep();
        }
        MB7Ev_KoopaMotionShiftSet(0, 1);
        HuPrcVSleep();
        HuPrcVSleep();

        for (i = 0; i < 2; i++) {
            s16 mdlId = (s16) mb7_Work[i + 4];

            MBModelDispSet(mdlId, TRUE);
            MBModelPosSetV(mdlId, (Point3d *) &mb7ev_KoopaCubePosTbl[side][i]);
            MBModelRotSet(mdlId, 0.0f, *(f32 *) (playerEntry + 0x1C), 0.0f);
        }
        MBAudFXPlay(0x620);
        for (i = 0; i <= 0x10; i++) {
            f32 scale = (f32) sin((3.141592653589793 * (f64) (90.0f * (0.0625f * (f32) i))) / 180.0);
            s32 j;

            for (j = 0; j < 2; j++) {
                MBModelScaleSet((s16) mb7_Work[j + 4], scale, scale, scale);
            }
            HuPrcVSleep();
        }

        MBWinCreateChoice(2, 0x330005U, 7, 0);
        if (GwPlayer[playerNo].comF) {
            if (MBEffSRandF() >= 0.0f) {
                MBComChoiceSetUp();
            } else {
                MBComChoiceSetDown();
            }
        }
        MBTopWinWait();
        pickedCube = MBWinLastChoiceGet() & 1;
        MBTopWinKill();

        resultPos = *(Point3d *) (playerEntry + 0x2C);
        resultPos.y += 275.0f;
        {
            Point3d dir;

            PSVECSubtract(&resultPos, (Point3d *) &mb7ev_KoopaCubePosTbl[side][pickedCube], &dir);
            PSVECScale(&dir, &dir, 0.033333335f);
            resultPos = mb7ev_KoopaCubePosTbl[side][pickedCube];
            MBModelDispSet((s16) mb7_Work[5 - pickedCube], FALSE);
            cubeMdlId = MB7Ev_KoopaCubeDustCreate((Point3d *) &mb7ev_KoopaCubePosTbl[side][1 - pickedCube], 1 - pickedCube);
            MBAudFXPlay(0x621);
            for (i = 0; (f32) i < 30.0f; i++) {
                PSVECAdd(&resultPos, &dir, &resultPos);
                MBModelPosSetV((s16) mb7_Work[pickedCube + 4], &resultPos);
                HuPrcVSleep();
            }
        }
        MBEffKill(cubeMdlId);
        MBPlayerMotionNoShiftSet(playerNo, 2, 0.0f, 8.0f, 0x40000001U);
        for (i = 0; (f32) i < 30.0f; i++) {
            MBAngleMoveTo((f32 *) (playerEntry + 0x3C), camRot.y, 0.15f);
            MBPlayerRotSetV(playerNo, (Point3d *) (playerEntry + 0x38));
            MB7Ev_KoopaAsibaUpdate();
            HuPrcVSleep();
        }

        *(f32 *) (koopaBase + 0x30) = MB7W_F32(0x128);
        MBPlayerMotionNoSet(playerNo, 0xB, 0U);
        {
            s32 bobCount = 0;

            for (i = 0; i < 0x2A; i++) {
                if (i >= 0x1B) {
                    f32 t;

                    if (i == 0x1B) {
                        MBAudFXPlay(0x32E);
                    }
                    bobCount += 1;
                    t = (f32) sin((3.141592653589793 * (f64) (12.0f * (f32) bobCount)) / 180.0);
                    resultPos.y += 100.0f * t;
                    MBModelPosSetV((s16) mb7_Work[pickedCube + 4], &resultPos);
                    MBModelScaleSet((s16) mb7_Work[pickedCube + 4], 1.0f + (0.5f * t), 1.0f - (0.5f * t), 1.0f + (0.5f * t));
                }
                HuPrcVSleep();
            }
        }
        while (!MBPlayerMotionEndCheck(playerNo)) {
            HuPrcVSleep();
        }

        MBPlayerMotIdleSet(playerNo);
        for (i = 0; i < 2; i++) {
            MBModelDispSet((s16) mb7_Work[i + 4], FALSE);
        }
        cubeMdlId = MB7Ev_KoopaCubeDustCreate(&resultPos, pickedCube);
        MBCameraQuakeSet(0x3C, 50.0f);
        {
            s32 seId = MBAudFXPlay(0x61A);

            for (i = 0; i < 0x3C; i++) {
                if (i == 0x1E) {
                    HuAudFXStop(seId);
                }
                HuPrcVSleep();
            }
        }
        MBEffKill(cubeMdlId);

        /* the win/lose outcome is an independent 50/50 coin flip, not
         * tied to which cube the player picked (the disassembly's
         * cror-fused compare on SRandF() decodes to >= 0.0f, not the
         * m2c draft's literal == 0.0f) */
        if (MBEffSRandF() >= 0.0f) {
            s16 winMdlId = (s16) mb7_Work[6];

            MBModelPosSetV(winMdlId, (Point3d *) (playerEntry + 0x2C));
            MBMotionTimeSet(winMdlId, 0.0f);
            MBMotionSpeedSet(winMdlId, 1.0f);
            MBModelDispSet(winMdlId, TRUE);
            MBAudFXPlay(0x61C);
            MBPlayerMotionNoSet(playerNo, 9, 0U);
            {
                s32 frame = 0;

                while (!MBPlayerMotionEndCheck(playerNo)) {
                    frame += 1;
                    if (frame == 0xA) {
                        omVibrate((s16) playerNo, 0xC, 6, 6);
                        WipeColorSet(0xFF, 0xFF, 0xFF);
                        WipeCreate(1, 0, 0x36);
                    }
                    HuPrcVSleep();
                }
            }
            MBPlayerMotionNoShiftSet(playerNo, 6, 0.0f, 8.0f, 0x40000001U);
            while (!MBMotionEndCheck(winMdlId)) {
                HuPrcVSleep();
            }
            MBModelDispSet(winMdlId, FALSE);
            MBCoinAddDispExec(playerNo, -(MBPlayerCoinGet(playerNo) >> 1), TRUE);
        } else {
            s16 craneMdlId = (s16) mb7_Work[3];
            s32 targetMasuId;
            MASU *targetMasuP;

            MBMotionTimeSet(craneMdlId, 0.0f);
            MBMotionSpeedSet(craneMdlId, 1.0f);
            *(s16 *) (playerEntry + 0x20) = (s16) MBPlayerMotionCreate(playerNo, 0x8C003F);
            MBPlayerMotionNoSet(playerNo, 9, 0U);
            MB7W_F32(0x20C) = 0.0f;
            MB7W_F32(0x208) = 0.0f;
            MB7W_F32(0x204) = 0.0f;
            for (i = 0; i < 0xA; i++) {
                if ((i & 3) == 0) {
                    MBAudFXPlay(0x61B);
                }
                HuPrcVSleep();
            }
            for (i = 0; (f32) i < 60.0f; i++) {
                if (((i + 0xA) & 3) == 0 && i < 0x14) {
                    MBAudFXPlay(0x61B);
                }
                if (i == 0xF) {
                    MBPlayerMotionNoShiftSet(playerNo, *(s16 *) (playerEntry + 0x20), 0.0f, 8.0f, 0);
                } else if (i == 0x1E) {
                    WipeColorSet(0, 0, 0);
                    WipeCreate(2, 0, 0x1E);
                }
                if ((i & 0xF) == 0) {
                    omVibrate((s16) playerNo, 0xC, 6, 6);
                }
                MB7W_F32(0x208) -= 0.5444445f;
                PSVECAdd((Point3d *) (playerEntry + 0x2C), (Point3d *) (playerEntry + 0x44), (Point3d *) (playerEntry + 0x2C));
                MBPlayerPosSetV(playerNo, (Point3d *) (playerEntry + 0x2C));
                MBCameraPosTargetGet(&camTarget);
                camTarget.y += MB7W_F32(0x208);
                MBCameraPosTargetSetV(&camTarget);
                HuPrcVSleep();
            }
            while (WipeCheckEnd()) {
                HuPrcVSleep();
            }

            targetMasuId = MBMasuFlagFind(0, 0x80000000U, 0x80000000U);
            targetMasuP = MBMasuGet(0, targetMasuId);
            MBPlayerPosFixPlayer(playerNo, targetMasuId, TRUE);
            GwPlayer[playerNo].masuId = (s16) targetMasuId;
            MBPlayerPosReset(playerNo);
            MBPlayerMotionNoSet(playerNo, 6, 0x40000001U);
            MBPlayerRotSet(playerNo, 0.0f, 0.0f, 0.0f);
            MBPlayerPosGet(playerNo, (Point3d *) (playerEntry + 0x2C));
            for (i = 0; i < 4; i++) {
                if (i != playerNo) {
                    MBModelDispSet(MBPlayerModelGet(i), FALSE);
                }
            }
            MBCameraSkipSet(FALSE);
            MBCameraFocusPlayerSet(-1);
            {
                Point3d camPos;

                camPos.x = MB7W_F32(0x1EC);
                camPos.y = MB7W_F32(0x1F0) + 150.0f;
                camPos.z = MB7W_F32(0x1F4);
                MBCameraPosViewSet(&camPos, NULL, NULL, 2500.0f, -1.0f, 1);
            }
            for (i = 0; i < 0xA; i++) {
                HuPrcVSleep();
            }
            WipeCreate(1, 0, 0x1E);
            MBMotionTimeSet(craneMdlId, 0.0f);
            MBMotionSpeedSet(craneMdlId, 0.0f);
            MBPlayerMotionKill(playerNo, *(s16 *) (playerEntry + 0x20));
            *(s16 *) (playerEntry + 0x20) = -1;
            while (WipeCheckEnd()) {
                HuPrcVSleep();
            }

            *(Point3d *) (koopaBase + 0x78) = targetMasuP->pos;
            MB7W_F32(0x11C) += 1000.0f;
            MB7W_F32(0x120) -= 400.0f;
            *(f32 *) (koopaBase + 0x28) = 120.0f;
            MB7W_F32(0x134) = -1000.0f / *(f32 *) (koopaBase + 0x28);
            *(f32 *) (koopaBase + 0x30) = 0.0f;
            *(f32 *) (koopaBase + 0x88) = 0.0f;
            for (i = 0; (f32) i < *(f32 *) (koopaBase + 0x28); i++) {
                if ((f32) i > (0.7f * *(f32 *) (koopaBase + 0x28))) {
                    MB7W_F32(0x134) *= 0.9f;
                }
                MB7Ev_KoopaAsibaUpdate();
                HuPrcVSleep();
            }
            MB7W_F32(0x134) = 0.0f;
        }

        MB7Ev_KoopaMotionShiftSet(3, 0);
        MBAudFXPlay(0x2C9);
        MBWinCreate(2, 0x330006U, 7);
        MBTopWinWait();
        MBTopWinKill();
        while (!MBMotionEndCheck((s16) *(s32 *) (koopaBase + 0x34))) {
            HuPrcVSleep();
        }
        MB7Ev_KoopaMotionShiftSet(0, 1);
        *(f32 *) (koopaBase + 0x28) = 120.0f;
        MB7W_F32(0x134) = 0.0f;
        for (i = 0; (f32) i < *(f32 *) (koopaBase + 0x28); i++) {
            if (MB7W_F32(0x134) < 11.666666f) {
                MB7W_F32(0x134) += 0.2777778f;
            }
            MB7Ev_KoopaAsibaUpdate();
            HuPrcVSleep();
        }

        WipeColorSet(0, 0, 0);
        WipeCreate(2, 0x81, 1);
        while (WipeCheckEnd()) {
            HuPrcVSleep();
        }

        MBModelHookReset(flyMdlId);
        MB7Ev_KoopaClose();
        MBModelDispSet(flyMdlId, FALSE);
        for (i = 0; i < 4; i++) {
            MBModelDispSet(MBPlayerModelGet(i), TRUE);
        }
        MBPlayerPosFixPlayer(playerNo, chosenMasuId, TRUE);
        GwPlayer[playerNo].masuId = (s16) chosenMasuId;
        MBPlayerPosReset(playerNo);
        MBPlayerMotionNoSet(playerNo, 1, 0x40000001U);
        MBPlayerRotSet(playerNo, 0.0f, 0.0f, 0.0f);
        MBCameraFocusMasuSet(chosenMasuId);
        MBCameraSkipSet(FALSE);
        MBCameraZoomSet(2000.0f);
        WipeCreate(1, 0x81, 0x1E);
        while (WipeCheckEnd()) {
            HuPrcVSleep();
        }
        for (i = 0; (f32) i < 30.0f; i++) {
            HuPrcVSleep();
        }
    }
}
