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
static u32 *mb7_SaveWorkP;
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
static void MB7Ev_KoopaWalk(s32 playerNo);
static void MB7Ev_Dokan(s32 playerNo);
static void MB7Ev_DokanIn(s32 playerNo);
static void MB7Ev_KoopaCube(s32 playerNo);
static void MB7Ev_Hou(s32 playerNo);

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
    mb7_SaveWorkP = GwSystem.mbSaveWork;
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
 * MB7Ev_KoopaInit's own addressing - confirmed 92.86% match):
 *   +0x34/+0x38/+0x3C/+0x40 (s32) = -1 after memset (revisited once
 *     MB7Ev_KoopaMotionCreate/MB7Ev_KoopaMotionShiftSet are decompiled -
 *     m2c's draft calls these mb7_Work[0x2E..0x30] etc in word units)
 *   +0x44..+0x64 (s32[9]) = -1      per-something free-slot markers
 *   +0x6C (s32)                     MBDataDirReadAsync() handle
 *   +0x70 (s32) = -1, +0x74 (s32) = 0   init markers
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
