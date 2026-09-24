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
#include "game/board/com.h"

#include "game/sprite.h"
#include "game/wipe.h"
#include "game/gamemes.h"
#include "game/pad.h"
#include "game/main.h"
#include "game/object.h"
#include "game/charman.h"
#include "game/hsfex.h"
#include "game/gamework.h"

#include <math.h>

/*
 * w07sDll - Story Mode variant of the "Koopa's Tycoon Town" board (w07Dll).
 *
 * Same flat-scratch-buffer design as w07Dll's mb7_Work, but smaller: this
 * board has only 4 "front" objects (mb7_ObjFileTbl) and 12 "back" objects
 * (mb7_BackObjFileTbl/Pos/RotY), instead of w07Dll's 11/24, and drops the
 * red-masu, Dokan, Koopa-cube and Koopa-walk-out cutscenes entirely (Story
 * mode never uses those masu types on this board) - only the Koopa fire
 * breath ("Hou") event and its motion/model plumbing remain.
 *
 * Field offsets below were re-derived from
 * config/GP5E01_00/rels/w07sDll/symbols.txt (mb7_Work = .bss:0x4
 * size:0x2DC) and build/GP5E01_00/w07sDll/asm/REL/w07sDll/world07.s,
 * function by function - NOT assumed from w07Dll's layout, since the
 * smaller front/back object tables shift every later offset. The Koopa
 * fire-breath sub-state and per-player Hou state *are* laid out exactly
 * like w07Dll's (byte-for-byte identical relative to their own base
 * pointers - koopaBase/fireBase/houPlayerBase below), just at different
 * absolute mb7_Work offsets; this was confirmed by reading
 * MB7Ev_KoopaMotionNoSet/ShiftSet/Close/Init's asm directly.
 */

#define MB7_OBJ_MAX 4      /* mb7_ObjFileTbl entries / model ids: mb7_Work[0..3] */
#define MB7_BACKOBJ_MAX 0xC /* mb7_BackObjFileTbl / mb7_BackObjPosTbl / mb7_BackObjRotYTbl entries */

/* mb7_Work as a flat 32-bit-word scratch buffer (0x2DC bytes = 0xB7 words):
 *   word[0..3]      (s32[4])  the 4 "front" object model ids
 *   byte 0x10+i*4    (s32[0xC]) the 12 "back" object model ids, i=0..0xB
 *   byte 0x40/0x44  (s32)     dust sprite ANIMDATA* read by
 *                              HuSprAnimRead/HuDataReadNum (0xD8000C/D)
 *   byte 0x48+i*2   (s16[4])  dust MBEFFECT model ids (only 2 created,
 *                              at i=0/1 - confirmed by MB7_ObjectCreate,
 *                              which only ever stores to +0x48/+0x4A)
 *   byte 0x54       koopaBase - see MB7Ev_Koopa helpers / MB7Ev_Hou below
 *   byte 0xE0       fireBase - fire-breath sub-state, see MB7Ev_Hou
 *   byte 0x164      houPlayerBase - per-player Hou cutscene state
 */
static s32 mb7_Work[0xB7];
#define MB7W_S16(off) (*(s16 *) ((off) + (u8 *) mb7_Work))
#define MB7W_U16(off) (*(u16 *) ((off) + (u8 *) mb7_Work))
#define MB7W_S32(off) (*(s32 *) ((off) + (u8 *) mb7_Work))
#define MB7W_F32(off) (*(f32 *) ((off) + (u8 *) mb7_Work))

static OMOBJ *mb7_MainObj;
static u8 *mb7_SaveWorkP;
static s16 mb7_MainMdlId;

static u32 mb7_ObjFileTbl[MB7_OBJ_MAX] = {
    0xD80004U, 0xD80006U, 0xD8000AU, 0xD8000BU,
};

static s32 mb7_BackObjFileTbl[MB7_BACKOBJ_MAX] = {
    0xD80005, 0xD80005,
    0xD80007, 0xD80007, 0xD80007, 0xD80007,
    0xD80008, 0xD80008,
    0xD80009, 0xD80009, 0xD80009, 0xD80009,
};

static const HuVecF mb7_BackObjPosTbl[MB7_BACKOBJ_MAX] = {
    { -615.0f, -315.0f, 645.0f }, { 615.0f, -315.0f, 645.0f },
    { -715.0f, -300.0f, -1600.0f }, { 715.0f, -300.0f, -1600.0f },
    { -715.0f, -300.0f, -2435.0f }, { 715.0f, -300.0f, -2435.0f },
    { -2895.0f, 500.0f, -2030.0f }, { 2895.0f, 500.0f, -2030.0f },
    { -2385.0f, 500.0f, -2545.0f }, { 2385.0f, 500.0f, -2545.0f },
    { -1245.0f, -400.0f, -300.0f }, { 1245.0f, -400.0f, -300.0f },
};

static f32 mb7_BackObjRotYTbl[MB7_BACKOBJ_MAX] = {
    30.0f, -30.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    -45.0f, 45.0f, -45.0f, 45.0f, 45.0f, -45.0f,
};

static s32 mb7ev_KupaMotFileTbl[9] = {
    0x100001, 0x100002, 0x100003, 0x100004, 0x100005,
    0x100007, 0x100006, 0x10000D, 0x100010,
};

/* MB7_Create camera constants (extracted from the shipped REL's .rodata,
 * same way as w07Dll's - m2c has no context for anonymous data). */
static const HuVecF mb7_OpeningPos = { 0.0f, -380.0f, -1030.0f };
static const HuVecF mb7_OpeningRot = { -30.0f, 0.0f, 0.0f };
static const HuVecF mb7_MapViewPos = { 0.0f, 0.0f, -631.0f };
static const HuVecF mb7_MapViewRot = { -78.0f, 0.0f, 0.0f };

/* Hu3DModelObjPosGet bone-name strings (dumped from .data, same names as
 * w07Dll uses for the koopa "dream" display model). */
static char lbl_1_data_100[] = "kupa_dream_all-kupa_pos";
static char lbl_1_data_118[] = "kupa_dream_all-fire_pos";

/* forward declarations, in increasing order of risk (boilerplate -> leaf
 * helpers -> effect hooks -> setup/teardown -> masu event hook -> Koopa
 * motion plumbing -> the Hou cutscene state machine). */
static void MB7_Create(void);
static void MB7_Kill(void);
static void MB7_ObjectCreate(void);
static void MB7_MainObjUpdate(OMOBJ *obj);
static void MB7_LightSet(void);
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
static void MB7Ev_KoopaFire1Hook(HU3DMODEL *modelP, MBEFFECT *effP, Mtx matrix);
static void MB7Ev_KoopaFire2Hook(HU3DMODEL *modelP, MBEFFECT *effP, Mtx matrix);
static void MB7Ev_KoopaFire3Hook(HU3DMODEL *modelP, MBEFFECT *effP, Mtx matrix);
static void MB7Ev_KoopaFire4Hook(HU3DMODEL *modelP, MBEFFECT *effP, Mtx matrix);
static void MB7Ev_Hou(s32 playerNo);
static void MB7Ev_HouPlayerUpdate(void);
static f32 MB7_FastSqrt(f32 x);

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
    GWPartyFSet(FALSE);
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
 * reproduced; logic is otherwise identical instruction-for-instruction.
 * #pragma dont_inline: MWCC inlines this small function into some of
 * its callers (e.g. MB7Ev_HouPlayerUpdate) without it; the original
 * keeps every call site out-of-line (confirmed via `bl MB7_AngleDiff`
 * there). */
#pragma dont_inline on
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
#pragma dont_inline reset

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

/* the original never sets a return value here (just `blr`); the result
 * is genuinely unused by the caller for this particular hook */
static int MB7Ev_MasuWalkPre(int playerNo) {
}

/* -------------------------- setup / teardown ---------------------------- */

/* NONMATCHING: the boardNo read below is a genuine dead store in the
 * original (a leftover debug/assert snapshot - confirmed via
 * build/GP5E01_00/w07sDll/asm/REL/w07sDll/world07.s: `lbz`+`clrlwi ,27`
 * off GwSystem+0x8 masks exactly the 5-bit boardNo bitfield, stored to
 * the stack and never read again); the original keeps it in a
 * callee-saved register (r31, +0x10 stack frame) even though it's
 * dead, ours doesn't - a rare case of the register allocator not
 * matching without the dead code actually being reachable another way.
 * 62.68% match; all remaining rodata-symbol churn is cosmetic. */
static void MB7_Create(void) {
    s32 boardNo = GwSystem.boardNo;

    memset(mb7_Work, 0, sizeof(mb7_Work));
    HuAudSndGrpSetSet(0x14);
    mb7_SaveWorkP = (u8 *) GwSystem.mbSaveWork;
    MBMasuCreate(0xD80000);
    mb7_MainMdlId = (s16) MBModelCreate(0xD80001, NULL, FALSE);
    MBModelPosSet(mb7_MainMdlId, 0.0f, 0.0f, 0.0f);
    MBModelAttrSet(mb7_MainMdlId, HU3D_MOTATTR_LOOP);
    MBScrollInit(0xD80002U, 10690.0f, -631.0f);
    {
        Point3d openingPos = mb7_OpeningPos;
        Point3d openingRot = mb7_OpeningRot;
        Point3d mapViewPos = mb7_MapViewPos;
        Point3d mapViewRot = mb7_MapViewRot;

        MBOpeningViewSet((HuVecF *) &openingRot, (HuVecF *) &openingPos, 9950.0f);
        MBMapViewCameraSet((HuVecF *) &mapViewRot, (HuVecF *) &mapViewPos, 10690.0f);
    }
    MBLightHookSet(MB7_LightSet, MB7_LightReset);
    MBCapsuleColCreate(0xD80003);
    MB7_ObjectCreate();
    HuDataDirClose(0xD80000);
    MBMasuWalkPreHookSet(MB7Ev_MasuWalkPre);
    MBMasuWalkPostHookSet((MASUPLAYERHOOK) MB7Ev_MasuWalkPost);
    MBMasuHatenaHookSet((MASUPLAYERHOOK) MB7Ev_MasuHatena);
    mb7_MainObj = omAddObjEx(mbObjMan, -0x8000, 0, 0, -1, MB7_MainObjUpdate);
}

/* NONMATCHING: mb7_Work byte-offset address computation order differs
 * from the original's -1/-0x14 folded add operands; logic identical */
static void MB7_ObjectCreate(void) {
    s32 i;

    for (i = 0; i < MB7_OBJ_MAX; i++) {
        mb7_Work[i] = -1;
    }
    for (i = 0; i < MB7_BACKOBJ_MAX; i++) {
        MB7W_S32(0x10 + (i * 4)) = -1;
    }
    for (i = 0; i < 4; i++) {
        MB7W_S16(0x48 + (i * 2)) = -1;
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
        MB7W_S32(0x10 + (i * 4)) = modelId;
        MBModelPosSetV((s16) modelId, (Point3d *) &mb7_BackObjPosTbl[i]);
        MBModelRotSet((s16) modelId, 0.0f, mb7_BackObjRotYTbl[i], 0.0f);
    }

    if (mb7_Work[0] >= 0) {
        MBModelAttrSet((s16) mb7_Work[0], HU3D_MOTATTR_LOOP);
        MBModelAttrSet((s16) mb7_Work[0], 0x40000040U);
        MBMotionSpeedSet((s16) mb7_Work[0], 0.5f);
        MBMotionShapeSpeedSet((s16) mb7_Work[0], 0.5f);
    }
    if (mb7_Work[1] >= 0) {
        MBModelPosSet((s16) mb7_Work[1], 0.0f, 330.0f, -2030.0f);
    }
    for (i = 2; i <= 3; i++) {
        if (mb7_Work[i] >= 0) {
            MBModelDispSet((s16) mb7_Work[i], FALSE);
        }
    }
    Hu3DModelLayerSet(MBModelIdGet((s16) mb7_Work[3]), 2);
    for (i = 8; i <= 0xC; i++) {
        s32 modelId = MB7W_S32(0x10 + (i * 4));
        if (modelId >= 0) {
            MBModelAttrSet((s16) modelId, HU3D_MOTATTR_LOOP);
            MBMotionSpeedSet((s16) modelId, 1.0f);
        }
    }
    MB7W_S32(0x40) = (s32)(uintptr_t) HuSprAnimRead(HuDataReadNum(0xD8000C, 0x10000000));
    MB7W_S32(0x44) = (s32)(uintptr_t) HuSprAnimRead(HuDataReadNum(0xD8000D, 0x10000000));
    MB7W_S16(0x48) = (s16) MBEffCreate((void *)(uintptr_t) MB7W_S32(0x40), 1);
    MB7W_S16(0x4A) = (s16) MBEffCreate((void *)(uintptr_t) MB7W_S32(0x44), 1);
}

/* NONMATCHING: mb7_Work byte-offset address add operand order differs;
 * logic identical */
static void MB7_Kill(void) {
    s32 i;

    if (mb7_MainMdlId >= 0) {
        MBModelKill(mb7_MainMdlId);
        mb7_MainMdlId = -1;
    }
    MBCameraModelViewSet(-1, NULL, NULL, -1.0f, 45.0f, 0x3F);
    for (i = 0; i < MB7_OBJ_MAX; i++) {
        if (mb7_Work[i] >= 0) {
            MBModelKill((s16) mb7_Work[i]);
        }
        mb7_Work[i] = -1;
    }
    for (i = 0; i < MB7_BACKOBJ_MAX; i++) {
        if (MB7W_S32((i * 4) + 0x10) >= 0) {
            MBModelKill((s16) MB7W_S32((i * 4) + 0x10));
        }
        MB7W_S32((i * 4) + 0x10) = -1;
    }
    for (i = 0; i < 4; i++) {
        if (MB7W_S16((i * 2) + 0x48) >= 0) {
            MBEffKill(MB7W_S16((i * 2) + 0x48));
        }
        MB7W_S16((i * 2) + 0x48) = -1;
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
}

/* ---------------------------- masu event hook ---------------------------- */

/* NONMATCHING: cmplwi vs cmpwi on the flag-bit test, and a trailing
 * `li r3,0` the original schedules before the MBMasuGet call instead of
 * at the end (90% match); logic identical */
static void MB7Ev_MasuHatena(int playerNo) {
    MASU *masuP = MBMasuGet(0, (s32) GwPlayer[playerNo].masuId);
    u32 flag = (u16) masuP->flag;

    if ((flag & 0x80) != 0) {
        MB7Ev_Hou(playerNo);
    }
}

static int MB7Ev_MasuWalkPost(int playerNo) {
    s16 masuId = GwPlayer[playerNo].masuId;
    MASU *masuP = MBMasuGet(0, masuId);
    s32 flag = (u16) masuP->flag;
    return 0;
}

/* --------------------------- Koopa motion state -------------------------- */

/* koopaBase = mb7_Work + 0x54, 0x8C bytes (confirmed against
 * build/GP5E01_00/w07sDll/asm/REL/w07sDll/world07.s - every Koopa* helper
 * computes `addi r31, r3, 0x54` off mb7_Work):
 *   +0x28/+0x2C/+0x30/+0x34 (s32[4]) = -1 after memset - up to 4 Koopa
 *     model ids (MB7Ev_KoopaClose kills all 4); only +0x28 is ever
 *     written to (by MB7Ev_KoopaCreate) or read by the motion helpers.
 *   +0x38..+0x58 (s32[9])  = -1     motion ids, one per
 *     mb7ev_KupaMotFileTbl entry (MB7Ev_KoopaMotionCreate/NoSet/
 *     ShiftSet always address index 0, i.e. the fixed +0x28 koopa).
 *   +0x60 (s32)                     MBDataDirReadAsync() handle.
 *   +0x64 (s32) = -1                last motion index set via
 *     MotionNoSet/ShiftSet (used to skip a redundant re-set).
 */
/* NONMATCHING: redundant li r0,-1 reload before consecutive stores
 * (92.50% match), same known MWCC quirk as w07Dll's equivalent
 * function; logic identical */
#pragma dont_inline on
static void MB7Ev_KoopaInit(u16 arg0) {
    u8 *base = (u8 *) mb7_Work + 0x54;
    s32 i;

    memset(base, 0, 0x8C);
    *(s32 *) (base + 0x64) = -1;
    *(s32 *) (base + 0x28) = *(s32 *) (base + 0x2C) = *(s32 *) (base + 0x30) = *(s32 *) (base + 0x34) = -1;
    for (i = 0; i < 9; i++) {
        *(s32 *) (base + 0x38 + (i * 4)) = -1;
    }
    *(s32 *) (base + 0x60) = MBDataDirReadAsync(0x100000);
}
#pragma dont_inline reset

/* NONMATCHING: register allocation order differs. #pragma dont_inline:
 * the original keeps this out-of-line even at its `(0)` constant-arg
 * call site in MB7Ev_KoopaCreate (confirmed via `bl
 * MB7Ev_KoopaMotionCreate` there); without the pragma MWCC's -inline
 * auto inlines it since motionIdx is a compile-time constant. */
#pragma dont_inline on
static void MB7Ev_KoopaMotionCreate(s32 motionIdx) {
    u8 *base = (u8 *) mb7_Work + 0x54;

    if (*(s32 *) (base + 0x38 + (motionIdx * 4)) < 0) {
        *(s32 *) (base + 0x38 + (motionIdx * 4)) = MBMotionCreate((s16) *(s32 *) (base + 0x28), mb7ev_KupaMotFileTbl[motionIdx]);
    }
}
#pragma dont_inline reset

static void MB7Ev_KoopaCreate(void) {
    u8 *base = (u8 *) mb7_Work + 0x54;

    MBDataAsyncWait(*(s32 *) (base + 0x60));
    *(s32 *) (base + 0x28) = (s16) MBModelCreate(0x100000, NULL, TRUE);
    MBModelDispSet((s16) *(s32 *) (base + 0x28), FALSE);
    HuPrcVSleep();
    MB7Ev_KoopaMotionCreate(0);
    MB7Ev_KoopaMotionNoSet(0, 1);
    HuPrcVSleep();
}

static void MB7Ev_KoopaDataClose(void) {
    u8 *base = (u8 *) mb7_Work + 0x54;
    HuDataDirClose(*(s32 *) (base + 0x60));
}

/* NONMATCHING: branch-distance-only diff, code identical (94.29% match) */
static void MB7Ev_KoopaMotionNoSet(s32 motionIdx, s32 loop) {
    u8 *base = (u8 *) mb7_Work + 0x54;
    s32 attr = 0;

    if (loop != 0) {
        attr = 0x40000001;
        if (motionIdx == *(s32 *) (base + 0x64)) {
            return;
        }
    }
    MBMotionNoSet((s16) *(s32 *) (base + 0x28), *(s32 *) (base + 0x38 + (motionIdx * 4)), attr);
    *(s32 *) (base + 0x64) = motionIdx;
}

/* NONMATCHING: branch-distance-only diff, code identical (95.12% match) */
static void MB7Ev_KoopaMotionShiftSet(s32 motionIdx, s32 loop) {
    u8 *base = (u8 *) mb7_Work + 0x54;
    s32 attr = 0;

    if (loop != 0) {
        attr = 0x40000001;
        if (motionIdx == *(s32 *) (base + 0x64)) {
            return;
        }
    }
    MBMotionShiftSet((s16) *(s32 *) (base + 0x28), *(s32 *) (base + 0x38 + (motionIdx * 4)), 0.0f, 8.0f, attr);
    *(s32 *) (base + 0x64) = motionIdx;
}

static int MB7Ev_KoopaMotionCheck(void) {
    u8 *base = (u8 *) mb7_Work + 0x54;

    if (MBMotionShiftIDGet((s16) *(s32 *) (base + 0x28)) >= 0) {
        return 0;
    }
    return MBMotionEndCheck((s16) *(s32 *) (base + 0x28));
}

/* NONMATCHING: base-pointer and loop-index registers swapped (r30/r31)
 * (69.70% match); logic identical */
static void MB7Ev_KoopaClose(void) {
    s32 i;
    u8 *base = (u8 *) mb7_Work + 0x54;

    for (i = 0; i < 4; i++) {
        if (*(s32 *) (base + 0x28 + (i * 4)) >= 0) {
            MBModelKill((s16) *(s32 *) (base + 0x28 + (i * 4)));
        }
        *(s32 *) (base + 0x28 + (i * 4)) = -1;
    }
}

/* ------------------------------ effect hooks ----------------------------- */

/* Standard fire-particle MBEFFECT/MBEFFECTDATA hooks, byte-for-byte
 * identical to w07Dll's MB7Ev_KoopaFire1..4Hook (same particle recipe,
 * same fireBase-relative field offsets - only the absolute fireBase
 * address differs, see MB7Ev_Hou below). */

/* NONMATCHING: original uses _savegpr_27 + extra callee-saved
 * float registers (f24-f26/r27) that this port's register
 * allocation doesn't reproduce (61.73% match), same known MWCC
 * quirk as w07Dll's equivalent hook; logic instruction-for-
 * instruction identical. */
static void MB7Ev_KoopaFire1Hook(HU3DMODEL *modelP, MBEFFECT *effP, Mtx matrix) {
    MBEFFECTDATA *p;
    s32 i;
    Point3d dir;
    Point3d origin;
    Point3d diff;
    f32 sweep;
    u8 *fireBase = (u8 *) mb7_Work + 0xE0;

    if (effP->count == 0) {
        p = effP->data;
        for (i = 0; i < effP->num; i++, p++) {
            p->scale = 0.0f;
            p->color.a = 0;
            p->time = (s16) -(i >> 3);
        }
        effP->count = 1;
    }

    dir.x = (f32) sin((3.141592653589793 * (f64) *(f32 *) (fireBase + 0x10)) / 180.0);
    dir.y = -0.15f;
    dir.z = (f32) cos((3.141592653589793 * (f64) *(f32 *) (fireBase + 0x10)) / 180.0);
    PSVECScale(&dir, &origin, 250.0f * *(f32 *) (fireBase + 0x18));
    PSVECAdd(&origin, (Point3d *) (fireBase + 0x40), &origin);
    sweep = 0.3f + (0.7f * *(f32 *) (fireBase + 0x18));

    p = effP->data;
    for (i = 0; i < effP->num; i++, p++) {
        if (p->time < 0) {
            if (*(s32 *) (fireBase + 0x1C) != 0) {
                p->time += 1;
            }
            continue;
        }
        if (p->time == 0) {
            if (*(s32 *) (fireBase + 0x1C) != 0) {
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

/* NONMATCHING: original uses _savegpr_27 + extra callee-saved
 * float registers (f24-f26/r27) that this port's register
 * allocation doesn't reproduce (63.54% match), same known MWCC
 * quirk as w07Dll's equivalent hook; logic instruction-for-
 * instruction identical. */
static void MB7Ev_KoopaFire2Hook(HU3DMODEL *modelP, MBEFFECT *effP, Mtx matrix) {
    MBEFFECTDATA *p;
    s32 i;
    Point3d dir;
    Point3d origin;
    Point3d diff;
    f32 sweep;
    u8 *fireBase = (u8 *) mb7_Work + 0xE0;

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

    dir.x = (f32) sin((3.141592653589793 * (f64) *(f32 *) (fireBase + 0x10)) / 180.0);
    dir.y = -0.15f;
    dir.z = (f32) cos((3.141592653589793 * (f64) *(f32 *) (fireBase + 0x10)) / 180.0);
    PSVECScale(&dir, &origin, 250.0f * *(f32 *) (fireBase + 0x18));
    PSVECAdd(&origin, (Point3d *) (fireBase + 0x40), &origin);
    sweep = 0.3f + (0.7f * *(f32 *) (fireBase + 0x18));

    p = effP->data;
    for (i = 0; i < effP->num; i++, p++) {
        if (p->time < 0) {
            if (*(s32 *) (fireBase + 0x1C) != 0) {
                p->time += 1;
            }
            continue;
        }
        if (p->time == 0) {
            if (*(s32 *) (fireBase + 0x1C) != 0) {
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

/* NONMATCHING: original uses _savegpr_27 + extra callee-saved
 * float registers (f24-f26/r27) that this port's register
 * allocation doesn't reproduce (56.23% match), same known MWCC
 * quirk as w07Dll's equivalent hook; logic instruction-for-
 * instruction identical. */
static void MB7Ev_KoopaFire3Hook(HU3DMODEL *modelP, MBEFFECT *effP, Mtx matrix) {
    MBEFFECTDATA *p;
    s32 i;
    Point3d dir;
    Point3d origin;
    Point3d firePos;
    Point3d tmp;
    Point3d diff;
    f32 sweep;
    u8 *fireBase = (u8 *) mb7_Work + 0xE0;

    if (effP->count == 0) {
        p = effP->data;
        for (i = 0; i < effP->num; i++, p++) {
            p->scale = 0.0f;
            p->color.a = 0;
            p->time = (s16) -(i >> 2);
        }
        effP->count = 1;
    }

    dir.x = (f32) sin((3.141592653589793 * (f64) *(f32 *) (fireBase + 0x10)) / 180.0);
    dir.y = 0.0f;
    dir.z = (f32) cos((3.141592653589793 * (f64) *(f32 *) (fireBase + 0x10)) / 180.0);
    PSVECScale(&dir, &origin, 250.0f * *(f32 *) (fireBase + 0x18));
    PSVECAdd(&origin, (Point3d *) (fireBase + 0x40), &origin);
    firePos = *(Point3d *) (fireBase + 0x40);
    sweep = 0.3f + (0.7f * *(f32 *) (fireBase + 0x18));

    p = effP->data;
    for (i = 0; i < effP->num; i++, p++) {
        if (p->time < 0) {
            if (*(s32 *) (fireBase + 0x1C) != 0) {
                p->time += 1;
            }
            continue;
        }
        if (p->time == 0) {
            if (*(s32 *) (fireBase + 0x1C) != 0) {
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
                PSVECScale(&dir, &tmp, 100.0f * (1.8f * (coneFactor * *(f32 *) (fireBase + 0x18))));
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

/* NONMATCHING: original uses _savegpr_27 + extra callee-saved
 * float registers (f24-f26/r27) that this port's register
 * allocation doesn't reproduce (56.84% match), same known MWCC
 * quirk as w07Dll's equivalent hook; logic instruction-for-
 * instruction identical. */
static void MB7Ev_KoopaFire4Hook(HU3DMODEL *modelP, MBEFFECT *effP, Mtx matrix) {
    MBEFFECTDATA *p;
    s32 i;
    Point3d dir;
    Point3d origin;
    Point3d firePos;
    Point3d tmp;
    Point3d diff;
    f32 sweep;
    u8 *fireBase = (u8 *) mb7_Work + 0xE0;

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

    dir.x = (f32) sin((3.141592653589793 * (f64) *(f32 *) (fireBase + 0x10)) / 180.0);
    dir.y = 0.0f;
    dir.z = (f32) cos((3.141592653589793 * (f64) *(f32 *) (fireBase + 0x10)) / 180.0);
    PSVECScale(&dir, &origin, 250.0f * *(f32 *) (fireBase + 0x18));
    PSVECAdd(&origin, (Point3d *) (fireBase + 0x40), &origin);
    firePos = *(Point3d *) (fireBase + 0x40);
    sweep = 0.3f + (0.7f * *(f32 *) (fireBase + 0x18));

    p = effP->data;
    for (i = 0; i < effP->num; i++, p++) {
        if (p->time < 0) {
            if (*(s32 *) (fireBase + 0x1C) != 0) {
                p->time += 1;
            }
            continue;
        }
        if (p->time == 0) {
            if (*(s32 *) (fireBase + 0x1C) != 0) {
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
                PSVECScale(&dir, &tmp, 100.0f * (1.8f * (coneFactor * *(f32 *) (fireBase + 0x18))));
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

/* ---------------------------- fire breath hook --------------------------- */

/* PowerPC fast reciprocal-sqrt estimate refined by 3 Newton-Raphson
 * iterations, same as w07Dll's - used once per spawned fire particle to
 * normalize a 2D (x,z) scatter vector. */
static f32 MB7_FastSqrt(f32 x) {
#ifdef TARGET_PC
    /* __frsqrte is a PowerPC compiler intrinsic with no host equivalent;
     * seed the same Newton-Raphson refinement with an exact reciprocal
     * sqrt instead of a hardware estimate - the refinement below already
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

/* Per-player Hou state, mb7_Work + 0x164, 4 * 0x5C bytes (one entry per
 * player - confirmed against MB7Ev_Hou/HouPlayerUpdate's asm, same
 * layout as w07Dll's: +0x0 (s32) active gate, +0x8 (s32) state, +0xC
 * (s32) masuId, +0x10 (s32) bounce count, +0x20 (s16) motion id, +0x2C/
 * +0x38 (Point3d) pos/rot, +0x30 (f32) last ground height, +0x44
 * (Point3d) fall velocity, +0x48 (f32) fall speed, +0x3C (f32) reset
 * scratch. */
#define MB7EV_HOU_PLAYER_MAX 4
#define MB7EV_HOU_PLAYER_STRIDE 0x5C

/* Per-frame per-player Hou (fire breath) update: knocks any player
 * standing in the breath's landing path off their feet. */
/* NONMATCHING: register allocation order differs */
static void MB7Ev_HouPlayerUpdate(void) {
    u8 *fireBase = (u8 *) mb7_Work + 0xE0;
    u8 *e = (u8 *) mb7_Work + 0x164;
    s32 playerNo;

    for (playerNo = 0; playerNo < MB7EV_HOU_PLAYER_MAX; playerNo++, e += MB7EV_HOU_PLAYER_STRIDE) {
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

/* Hou (fire breath) cutscene: player stands in front of the koopa, gets
 * a warning turn, then the koopa breathes fire in a sweeping arc (4
 * layered MBEFFECT fire hooks) while MB7Ev_HouPlayerUpdate knocks out
 * any player standing in the breath's path; unlike w07Dll's normal-mode
 * version, Story mode also docks half the coin count of every affected
 * player afterward, with an extra dialogue window for it.
 * fireBase = mb7_Work+0xE0 (see MB7Ev_HouPlayerUpdate); mb7_Work[1] is
 * the koopa "dream" display model used for Hu3DModelObjPosGet bone
 * lookups, mb7_Work[2] is a small ember/nozzle prop shown only while the
 * koopa is mid-breath, mb7_Work[0x10] is the fire particle sprite data
 * read by MB7_ObjectCreate. koopaBase+0x24 (f32) is a decaying head-bob
 * factor for the koopa's aiming pose (mb7_Work+0x54+0x24, just below the
 * +0x28 koopa model id). */
/* NONMATCHING: register allocation order differs substantially across
 * this function's many sequential loops and its 5-phase inner state
 * machine; logic verified against the asm field-by-field */
static void MB7Ev_Hou(s32 playerNo) {
    u8 *koopaBase = (u8 *) mb7_Work + 0x54;
    u8 *fireBase = (u8 *) mb7_Work + 0xE0;
    s16 dreamMdlId;
    s32 seId = -1;
    Point3d kupaPos;
    Point3d firePos;
    s32 i;
    MASU *masuP = MBMasuGet(0, (s32) GwPlayer[playerNo].masuId);
    u32 flag = masuP->flag;

    MB7Ev_KoopaInit((u16) flag);
    MBCameraFocusPlayerSet(-1);
    MBCameraPlayerViewSet((s16) playerNo, NULL, NULL, 1500.0f, -1.0f, 0x1E);
    {
        Point3d rot;
        MBPlayerRotGet(playerNo, &rot);
        if (fabs(MB7_AngleDiff(rot.y, 0.0f)) > 10.0f) {
            MBPlayerMotionNoShiftSet(playerNo, 2, 0.0f, 8.0f, 0x40000001U);
        } else {
            MBPlayerMotIdleSet(playerNo);
        }
    }
    for (i = 0; (f32) i < 30.0f; i++) {
        Point3d rot;

        MBPlayerRotGet(playerNo, &rot);
        MBAngleMoveTo(&rot.y, 0.0f, 0.15f);
        MBPlayerRotSetV(playerNo, &rot);
        HuPrcVSleep();
    }

    memset((u8 *) mb7_Work + 0x164, 0, MB7EV_HOU_PLAYER_MAX * MB7EV_HOU_PLAYER_STRIDE);
    {
        u8 *e = (u8 *) mb7_Work + 0x164;

        for (i = 0; i < MB7EV_HOU_PLAYER_MAX; i++, e += MB7EV_HOU_PLAYER_STRIDE) {
            if (MBPlayerAliveCheck(i)) {
                s16 masuId = GwPlayer[i].masuId;

                if (MBMasuGet(0, (s32) masuId)->flag & 0x80) {
                    *(s32 *) (e + 0x0) = 1;
                    *(s32 *) (e + 0xC) = masuId;
                }
            }
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
    dreamMdlId = (s16) mb7_Work[1];
    Hu3DModelObjPosGet(MBModelIdGet(dreamMdlId), lbl_1_data_100, &kupaPos);
    Hu3DModelObjPosGet(MBModelIdGet(dreamMdlId), lbl_1_data_118, (Point3d *) (fireBase + 0x40));
    *(Point3d *) (fireBase + 0x64) = kupaPos;
    MBMotionSpeedSet((s16) mb7_Work[2], 1.0f);
    MBModelAttrSet((s16) mb7_Work[2], 0x40000001U);
    *(Point3d *) (koopaBase + 0x18) = kupaPos;
    MB7W_F32(0x54 + 0x18 + 0x8) -= 120.00001f;
    MBModelPosSetV((s16) *(s32 *) (koopaBase + 0x28), (Point3d *) (koopaBase + 0x18));
    MBModelDispSet((s16) *(s32 *) (koopaBase + 0x28), TRUE);
    MBCameraFocusPlayerSet(-1);
    {
        Point3d camPos = kupaPos;
        camPos.z += 500.0f;
        MBCameraPosViewSet(&camPos, NULL, NULL, 3000.0f, -1.0f, 0x3C);
    }
    MBAudFXPlay(0x2C9);
    MB7Ev_KoopaMotionShiftSet(3, 0);
    while (!MBMotionEndCheck((s16) *(s32 *) (koopaBase + 0x28))) {
        HuPrcVSleep();
    }

    MB7Ev_KoopaMotionShiftSet(0, 1);
    MBWinCreate(2, 0x330009U, 7);
    MBTopWinWait();
    MBTopWinKill();
    MB7Ev_KoopaMotionShiftSet(8, 0);
    *(s32 *) (fireBase + 0x30) = MBEffCreate((ANIMDATA *)(uintptr_t) mb7_Work[0x10], 0x100);
    MBEffHookSet((s16) *(s32 *) (fireBase + 0x30), MB7Ev_KoopaFire1Hook);
    Hu3DModelLayerSet((s16) *(s32 *) (fireBase + 0x30), 2);
    Hu3DModelPosSet((s16) *(s32 *) (fireBase + 0x30), 0.0f, 0.0f, 0.0f);
    *(s32 *) (fireBase + 0x34) = MBEffCreate((ANIMDATA *)(uintptr_t) mb7_Work[0x10], 0x40);
    MBEffHookSet((s16) *(s32 *) (fireBase + 0x34), MB7Ev_KoopaFire2Hook);
    Hu3DModelLayerSet((s16) *(s32 *) (fireBase + 0x34), 2);
    Hu3DModelPosSet((s16) *(s32 *) (fireBase + 0x34), 0.0f, 0.05f, 0.05f);
    *(s32 *) (fireBase + 0x38) = MBEffCreate((ANIMDATA *)(uintptr_t) mb7_Work[0x10], 0x40);
    MBEffHookSet((s16) *(s32 *) (fireBase + 0x38), MB7Ev_KoopaFire3Hook);
    Hu3DModelLayerSet((s16) *(s32 *) (fireBase + 0x38), 2);
    Hu3DModelPosSet((s16) *(s32 *) (fireBase + 0x38), 0.0f, -0.05f, -0.05f);
    *(s32 *) (fireBase + 0x3C) = MBEffCreate((ANIMDATA *)(uintptr_t) mb7_Work[0x10], 0x40);
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
            *(f32 *) (koopaBase + 0x24) = 1.0f;
            *(s32 *) (fireBase + 0x0) += 1;
            seId = HuAudFXEmiterPlay(0x61D, &kupaPos);
            /* fallthrough */
        case 1:
            *(f32 *) (fireBase + 0x10) += *(f32 *) (fireBase + 0x14);
            if (*(f32 *) (fireBase + 0x10) > -45.0f) {
                if (*(f32 *) (fireBase + 0x14) > -1.0f) {
                    *(f32 *) (fireBase + 0x14) += -0.02f;
                }
            } else if (*(f32 *) (fireBase + 0x10) < -90.0f && *(f32 *) (fireBase + 0x14) < -0.33333334f) {
                *(f32 *) (fireBase + 0x14) += 0.02f;
            }
            if (*(f32 *) (fireBase + 0x10) <= -110.0f) {
                *(s32 *) (fireBase + 0x1C) = 1;
                *(f32 *) (fireBase + 0x10) = -110.0f;
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
            } else if (*(f32 *) (fireBase + 0x10) > 95.0f && *(f32 *) (fireBase + 0x14) > 0.33333334f) {
                *(f32 *) (fireBase + 0x14) += -0.015000001f;
            }
            if (*(f32 *) (fireBase + 0x10) >= 110.0f) {
                *(f32 *) (fireBase + 0x10) = 110.0f;
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
            }
            break;
        default:
            done = TRUE;
            break;
        }

        if (!done) {
            *(f32 *) (fireBase + 0x50) = *(f32 *) (fireBase + 0x10);
            MBModelRotSetV(dreamMdlId, (Point3d *) (fireBase + 0x4C));
            Hu3DModelObjPosGet(MBModelIdGet(dreamMdlId), lbl_1_data_100, &kupaPos);
            Hu3DModelObjPosGet(MBModelIdGet(dreamMdlId), lbl_1_data_118, (Point3d *) (fireBase + 0x40));
            *(Point3d *) (fireBase + 0x64) = kupaPos;
            *(f32 *) (koopaBase + 0x24) *= 0.98f;
            kupaPos.x += ((-60.000004f * *(f32 *) (koopaBase + 0x24)) - 60.000004f) * (f32) sin((3.141592653589793 * (f64) *(f32 *) (fireBase + 0x50)) / 180.0);
            kupaPos.z += ((-60.000004f * *(f32 *) (koopaBase + 0x24)) - 60.000004f) * (f32) cos((3.141592653589793 * (f64) *(f32 *) (fireBase + 0x50)) / 180.0);
            MBModelPosSetV((s16) *(s32 *) (koopaBase + 0x28), &kupaPos);
            MBModelRotSetV((s16) *(s32 *) (koopaBase + 0x28), (Point3d *) (fireBase + 0x4C));

            if (*(f32 *) (fireBase + 0x18) == 0.0f || *(s32 *) (fireBase + 0x1C) == 0) {
                MBModelDispSet((s16) mb7_Work[2], FALSE);
            } else {
                MBModelDispSet((s16) mb7_Work[2], TRUE);
                MBModelPosSet((s16) mb7_Work[2], *(f32 *) (fireBase + 0x40), *(f32 *) (fireBase + 0x44) - 60.000004f, *(f32 *) (fireBase + 0x48));
                MBModelRotSetV((s16) mb7_Work[2], (Point3d *) (fireBase + 0x4C));
                MBModelScaleSet((s16) mb7_Work[2], *(f32 *) (fireBase + 0x18), *(f32 *) (fireBase + 0x18), *(f32 *) (fireBase + 0x18));
            }
            firePos.x = 250.0f * (*(f32 *) (fireBase + 0x18) * (f32) sin((3.141592653589793 * (f64) *(f32 *) (fireBase + 0x50)) / 180.0));
            firePos.z = 250.0f * (*(f32 *) (fireBase + 0x18) * (f32) cos((3.141592653589793 * (f64) *(f32 *) (fireBase + 0x50)) / 180.0));
            firePos.y = *(f32 *) (fireBase + 0x44);
            MB7Ev_HouPlayerUpdate();

            {
                BOOL anyBusy = FALSE;
                u8 *e = (u8 *) mb7_Work + 0x164;

                for (i = 0; i < MB7EV_HOU_PLAYER_MAX; i++, e += MB7EV_HOU_PLAYER_STRIDE) {
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

    /* Story-mode-only: dock half the coin count of every player who was
     * caught in the fire breath, with its own coin-loss window. */
    {
        s32 coinLoss[MB7EV_HOU_PLAYER_MAX];
        u8 *e = (u8 *) mb7_Work + 0x164;

        for (i = 0; i < MB7EV_HOU_PLAYER_MAX; i++, e += MB7EV_HOU_PLAYER_STRIDE) {
            coinLoss[i] = 0;
            if (*(s32 *) (e + 0x0) != 0) {
                s32 coins = MBPlayerCoinGet(i);
                if (coins > 1) {
                    coinLoss[i] = -(coins >> 1);
                }
            }
        }
        MBCoinAddAllDispExec(coinLoss[0], coinLoss[1], coinLoss[2], coinLoss[3], TRUE);
    }
    MBAudFXPlay(0x2C9);
    MBWinCreate(2, 0x33000AU, 7);
    MBTopWinWait();
    MBTopWinKill();
    MB7Ev_KoopaMotionShiftSet(2, 0);
    {
        Point3d pos = kupaPos;
        pos.z -= 80.0f;
        for (i = 0; i < 0x64; i++) {
            kupaPos.z += 0.15f * (pos.z - kupaPos.z);
            MBModelPosSetV((s16) *(s32 *) (koopaBase + 0x28), &kupaPos);
            if (i == 0x41) {
                MBCameraQuakeSet(0x12, 50.0f);
                MBAudFXPlay(0x2CC);
            }
            HuPrcVSleep();
        }
    }

    MBMotionSpeedSet((s16) *(s32 *) (koopaBase + 0x28), 0.0f);
    WipeColorSet(0, 0, 0);
    WipeCreate(2, 0x81, 1);
    while (WipeCheckEnd()) {
        HuPrcVSleep();
    }

    {
        u8 *e = (u8 *) mb7_Work + 0x164;

        for (i = 0; i < MB7EV_HOU_PLAYER_MAX; i++, e += MB7EV_HOU_PLAYER_STRIDE) {
            if (*(s32 *) (e + 0x0) != 0) {
                *(s32 *) (e + 0x8) = -1;
            }
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
