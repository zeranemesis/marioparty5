#include "game/data.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/printfunc.h"
#include "game/process.h"
#include "game/wipe.h"
#include "math.h"

#include "REL/motchkDll.h"

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

void ObjectSetup(void);
void MotChkMain(void);
void MotChkModelInit(s16 charNo, s16 motNo, s16 lodNo);
void MotChkTimingHook(HU3DMODELID modelId, HU3DMOTID motId, BOOL lagF);
void MotChkMotionSet(s16 charNo, s16 motNo, s16 lodNo);
void MotChkMotionKill(s16 charNo);
void DebugCamUpdate(OMOBJ *obj);

static const char *motNameTbl[0xDC] = {
    "c000m1_300", "c000m1_301", "c000m1_302", "c000m1_303", "c000m1_304",
    "c000m1_305", "c000m1_306", "c000m1_307", "c000m1_308", "c000m1_309",
    "c000m1_310", "c000m1_311", "c000m1_312", "c000m1_313", "c000m1_314",
    "c000m1_315", "c000m1_316", "c000m1_317", "c000m1_318", "c000m1_319",
    "c000m1_320", "c000m1_321", "c000m1_322", "c000m1_323", "c000m1_324",
    "c000m1_325", "c000m1_326", "c000m1_327", "c000m1_328", "c000m1_329",
    "c000m1_332", "c000m1_333", "c000m1_336", "c000m1_338", "c000m1_339",
    "c000m1_340", "c000m1_341", "c000m1_343", "c000m1_344", "c000m1_345",
    "c000m1_346", "c000m1_347", "c000m1_348", "c000m1_349", "c000m1_350",
    "c000m1_351", "c000m1_352", "c000m1_353", "c000m1_354", "c000m1_355",
    "c000m1_356", "c000m1_357", "c000m1_358", "c000m1_359", "c000m1_360",
    "c000m1_361", "c000m1_362", "c000m1_363", "c000m1_364", "c000m1_365",
    "c000m1_366", "c000m1_367", "c000m1_368", "c000m1_369", "c000m1_370",
    "c000m1_371", "c000m1_372", "c000m1_373", "c000m1_374", "c000m1_375",
    "c000m1_376", "c000m1_377", "c000m1_378", "c000m1_379", "c000m1_380",
    "c000m1_381", "c000m1_382", "c000m1_383", "c000m1_384", "c000m1_385",
    "c000m1_386", "c000m1_387", "c000m1_388", "c000m1_389", "c000m1_390",
    "c000m1_391", "c000m1_392", "c000m1_393", "c000m1_394", "c000m1_395",
    "c000m1_396", "c000m1_397", "c000m1_398", "c000m1_399", "c000m1_400",
    "c000m1_401", "c000m1_402", "c000m1_403", "c000m1_404", "c000m1_405",
    "c000m1_406", "c000m1_407", "c000m1_408", "c000m1_409", "c000m1_410",
    "c000m1_411", "c000m1_412", "c000m1_413", "c000m1_414", "c000m1_415",
    "c000m1_416", "c000m1_417", "c000m1_418", "c000m1_419", "c000m1_420",
    "c000m1_421", "c000m1_422", "c000m1_423", "c000m1_424", "c000m1_425",
    "c000m1_426", "c000m1_427", "c000m1_428", "c000m1_429", "c000m1_430",
    "c000m1_431", "c000m1_432", "c000m1_433", "c000m1_434", "c000m1_435",
    "c000m1_436", "c000m1_437", "c000m1_438", "c000m1_439", "c000m1_440",
    "c000m1_441", "c000m1_442", "c000m1_443", "c000m1_444", "c000m1_445",
    "c000m1_446", "c000m1_447", "c000m1_448", "c000m1_449", "c000m1_450",
    "c000m1_451", "c000m1_452", "c000m1_453", "c000m1_454", "c000m1_455",
    "c000m1_456", "c000m1_457", "c000m1_458", "c000m1_459", "c000m1_460",
    "c000m1_461", "c000m1_462", "c000m1_463", "c000m1_464", "c000m1_465",
    "c000m1_466", "c000m1_467", "c000m1_468", "c000m1_469", "c000m1_470",
    "c000m1_471", "c000m1_472", "c000m1_473", "c000m1_474", "c000m1_475",
    "c000m1_476", "c000m1_477", "c000m1_478", "c000m1_479", "c000m1_480",
    "c000m1_481", "c000m1_482", "c000m1_483", "c000m1_484", "c000m1_485",
    "c000m1_486", "c000m1_487", "c000m1_488", "c000m1_489", "c000m1_490",
    "c000m1_491", "c000m1_493", "c000m1_494", "c000m1_495", "c000m1_496",
    "c000m1_497", "c000m1_498", "c000m1_499", "c000m1_500", "c000m1_501",
    "c000m1_502", "c000m1_503", "c000m1_504", "c000m1_505", "c000m1_506",
    "c000m1_507", "c000m1_508", "c000m1_509", "c000m1_510", "c000m1_511",
    "c000m1_512", "c000m1_513", "c000m1_514", "c000m1_515", "c000m1_516",
    "c000m1_517", "c000m1_518", "c000m1_519", "c000m1_520", "c000m1_521",
    "c000m1_522", "c000m1_523", "c000m1_524", "c000m1_526", "c000m1_eye",
};

static const s32 dataDirTbl[0xA] = {
    0x870000, 0x2E0000, 0xB20000, 0xE80000, 0xE10000,
    0x120000, 0xDB0000, 0x280000, 0xC40000, 0x9C0000,
};

static s32 lodNo;
static s32 motNo;
static s32 charNo;
static s32 motchkMotIdOld[4];
static s32 motchkMotId[4];
static s32 motchkMdlId[4];
static OMOBJMAN *objman;
static s32 motchkLoopMode;
static u8 dummyProc[0x20];
static OMOBJ *outViewObj;
static OMOBJ *debugCamUpdateObj;

s32 _prolog(void) {
    const VoidFunc *ctors = _ctors;

    while (*ctors) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void) {
    const VoidFunc *dtors = _dtors;

    while (*dtors) {
        (**dtors)();
        dtors++;
    }
}

void ObjectSetup(void) {
    HU3DLIGHTID lightId;

    OSReport("******* Motion Check ObjectSetup *********\n");
    objman = omInitObjMan(0x32, 0x2000);
    CRot.x = -20.0f;
    CRot.y = 0.0f;
    CRot.z = 0.0f;
    Center.x = 0.0f;
    Center.y = 100.0f;
    Center.z = 0.0f;
    CZoom = 400.0f;
    Hu3DCameraCreate(1);
    Hu3DCameraPerspectiveSet(1, 45.0f, 20.0f, 15000.0f, 1.2f);
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    outViewObj = omAddObjEx(objman, 0x7FDA, 0, 0, OM_GRP_NONE, omOutView);
    debugCamUpdateObj = omAddObjEx(objman, 0, 0x40, 0, OM_GRP_NONE, DebugCamUpdate);
    HuPrcChildCreate(MotChkMain, 0x64, 0x3000, 0, (HUPROCESS *) objman);
    lightId = Hu3DGLightCreate(0.0f, 1000.0f, 1000.0f, 0.0f, -1.0f, -1.0f, 0xFF, 0xFF, 0xFF);
    Hu3DGLightInfinitytSet(lightId);
}

// NONMATCHING: MWCC allocates one fewer callee-saved register (r29) than the
// original; the loaded rodata float values and control flow are identical.
void MotChkMain(void) {
    s16 unused;
    s16 i;
    HU3DMODELID modelId;
    f32 sp28;
    f32 sp24;
    f32 sp20;
    f32 sp1C;
    f32 sp18;
    f32 sp14;
    f32 sp10;
    f32 spC;
    f32 sp8;

    WipeCreate(1, 0, 0x14);
    while (WipeCheckEnd()) {
        HuPrcVSleep();
    }

    modelId = Hu3DModelCreate(HuDataSelHeapReadNum(0xBA0007, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelPosSet(modelId, 0.0f, 200.0f, 0.0f);
    Hu3DModelShadowMapSet(modelId);
    HuDataDirClose(0xBA0000);
    Hu3DShadowCreate(30.0f, 20.0f, 2000.0f);

    sp20 = 200.0f;
    sp24 = 1000.0f;
    sp28 = 200.0f;
    spC = 1.0f;
    sp10 = 0.0f;
    sp8 = 0.0f;
    sp1C = 0.0f;
    sp18 = 0.0f;
    sp14 = 0.0f;
    Hu3DShadowPosSet((HuVecF *) &sp20, (HuVecF *) &sp8, (HuVecF *) &sp14);
    Hu3DShadowTPLvlSet(0.6f);

    motNo = 0;
    charNo = GwPlayerConf[0].charNo;
    lodNo = 1;
    unused = 0;
    MotChkModelInit((s16) charNo, (s16) motNo, (s16) lodNo);

    for (;;) {
        if (HuPadBtnRep[0] & 0x100) {
            motNo += 1;
            if (motNo >= 0xDC) {
                motNo = 0;
            }
            MotChkMotionSet((s16) charNo, (s16) motNo, (s16) lodNo);
        } else if (HuPadBtnRep[0] & 0x200) {
            motNo -= 1;
            if (motNo < 0) {
                motNo = 0xDB;
            }
            MotChkMotionSet((s16) charNo, (s16) motNo, (s16) lodNo);
        } else if (HuPadBtnDown[0] & 0x10) {
            lodNo += 1;
            if (lodNo >= 4) {
                lodNo = 0;
            }
            for (i = 0; i < 4; i++) {
                Hu3DModelAttrSet((HU3DMODELID) motchkMdlId[i], HU3D_ATTR_DISPOFF);
            }
            Hu3DModelAttrReset((HU3DMODELID) motchkMdlId[lodNo], HU3D_ATTR_DISPOFF);
        } else if (HuPadBtnRep[0] & 0x400) {
            MotChkMotionKill((s16) charNo);
            charNo += 1;
            if (charNo >= 0xA) {
                charNo = 0;
            }
            MotChkModelInit((s16) charNo, (s16) motNo, (s16) lodNo);
        } else if (HuPadBtnRep[0] & 0x800) {
            MotChkMotionKill((s16) charNo);
            charNo -= 1;
            if (charNo < 0) {
                charNo = 9;
            }
            MotChkModelInit((s16) charNo, (s16) motNo, (s16) lodNo);
        }

        if (HuPadBtnDown[0] == 8) {
            unused = 1;
        }

        fontcolor = 0xF;
        print8(0x10, 0x18, 2.0f, "%03d:%s", motNo, motNameTbl[motNo]);
        print8(0x10, 0x28, 2.0f, "MODEL%d", lodNo);
        if (motchkLoopMode == 1) {
            print8(0x260, 0x1A0, 2.0f, "*");
        } else if (motchkLoopMode == 2) {
            print8(0x260, 0x1A0, 2.0f, "-");
        }
        motchkLoopMode = 0;
        HuPrcVSleep();
    }
}

// NONMATCHING: MWCC re-extends the motNo parameter (extsh) before each of
// the four dataDirTbl lookups; equivalent register scheduling otherwise.
void MotChkModelInit(s16 charNo, s16 motNo, s16 lodNo) {
    s16 i;

    motchkMdlId[0] = Hu3DModelCreateData(dataDirTbl[charNo] | 0);
    motchkMdlId[1] = Hu3DModelCreateData(dataDirTbl[charNo] | 1);
    motchkMdlId[2] = Hu3DModelCreateData(dataDirTbl[charNo] | 2);
    motchkMdlId[3] = Hu3DModelCreateData(dataDirTbl[charNo] | 3);
    motchkMotId[0] = Hu3DJointMotionData((HU3DMODELID) motchkMdlId[0], (u16) ((s32) motNo + 0x870004) | dataDirTbl[charNo]);
    motchkMotId[1] = Hu3DJointMotionData((HU3DMODELID) motchkMdlId[1], (u16) ((s32) motNo + 0x870004) | dataDirTbl[charNo]);
    motchkMotId[2] = Hu3DJointMotionData((HU3DMODELID) motchkMdlId[2], (u16) ((s32) motNo + 0x870004) | dataDirTbl[charNo]);
    motchkMotId[3] = Hu3DJointMotionData((HU3DMODELID) motchkMdlId[3], (u16) ((s32) motNo + 0x870004) | dataDirTbl[charNo]);
    motchkMotIdOld[0] = Hu3DJointMotionData((HU3DMODELID) motchkMdlId[0], dataDirTbl[charNo] | 4);
    motchkMotIdOld[1] = Hu3DJointMotionData((HU3DMODELID) motchkMdlId[1], dataDirTbl[charNo] | 4);
    motchkMotIdOld[2] = Hu3DJointMotionData((HU3DMODELID) motchkMdlId[2], dataDirTbl[charNo] | 4);
    motchkMotIdOld[3] = Hu3DJointMotionData((HU3DMODELID) motchkMdlId[3], dataDirTbl[charNo] | 4);
    Hu3DMotionSet((HU3DMODELID) motchkMdlId[0], (HU3DMOTID) motchkMotId[0]);
    Hu3DMotionSet((HU3DMODELID) motchkMdlId[1], (HU3DMOTID) motchkMotId[1]);
    Hu3DMotionSet((HU3DMODELID) motchkMdlId[2], (HU3DMOTID) motchkMotId[2]);
    Hu3DMotionSet((HU3DMODELID) motchkMdlId[3], (HU3DMOTID) motchkMotId[3]);
    for (i = 0; i < 4; i++) {
        Hu3DModelAttrSet((HU3DMODELID) motchkMdlId[i], HU3D_ATTR_DISPOFF);
        Hu3DModelAttrSet((HU3DMODELID) motchkMdlId[i], HU3D_MOTATTR | HU3D_ATTR_DISPOFF);
        Hu3DMotionAttrSet((HU3DMOTID) motchkMotId[i], HU3D_ATTR_DISPOFF);
        Hu3DModelShadowSet((HU3DMODELID) motchkMdlId[i]);
        Hu3DMotionTimingHookSet((HU3DMODELID) motchkMdlId[i], MotChkTimingHook);
    }
    Hu3DModelAttrReset((HU3DMODELID) motchkMdlId[lodNo], HU3D_ATTR_DISPOFF);
    motchkLoopMode = 0;
}

void MotChkTimingHook(HU3DMODELID modelId, HU3DMOTID motId, BOOL lagF) {
    motchkLoopMode = lagF ? 1 : 2;
}

// NONMATCHING: same minor register-scheduling difference as MotChkModelInit.
void MotChkMotionSet(s16 charNo, s16 motNo, s16 lodNo) {
    s16 i;

    for (i = 0; i < 4; i++) {
        Hu3DMotionSet((HU3DMODELID) motchkMdlId[i], (HU3DMOTID) motchkMotIdOld[i]);
    }
    for (i = 0; i < 4; i++) {
        Hu3DMotionKill((HU3DMOTID) motchkMotId[i]);
    }
    motchkMotId[0] = Hu3DJointMotionData((HU3DMODELID) motchkMdlId[0], (u16) (motNo + 0x870004) | dataDirTbl[charNo]);
    motchkMotId[1] = Hu3DJointMotionData((HU3DMODELID) motchkMdlId[1], (u16) (motNo + 0x870004) | dataDirTbl[charNo]);
    motchkMotId[2] = Hu3DJointMotionData((HU3DMODELID) motchkMdlId[2], (u16) (motNo + 0x870004) | dataDirTbl[charNo]);
    motchkMotId[3] = Hu3DJointMotionData((HU3DMODELID) motchkMdlId[3], (u16) (motNo + 0x870004) | dataDirTbl[charNo]);
    Hu3DMotionSet((HU3DMODELID) motchkMdlId[0], (HU3DMOTID) motchkMotId[0]);
    Hu3DMotionSet((HU3DMODELID) motchkMdlId[1], (HU3DMOTID) motchkMotId[1]);
    Hu3DMotionSet((HU3DMODELID) motchkMdlId[2], (HU3DMOTID) motchkMotId[2]);
    Hu3DMotionSet((HU3DMODELID) motchkMdlId[3], (HU3DMOTID) motchkMotId[3]);
    for (i = 0; i < 4; i++) {
        Hu3DModelAttrSet((HU3DMODELID) motchkMdlId[i], HU3D_ATTR_DISPOFF);
        Hu3DModelAttrSet((HU3DMODELID) motchkMdlId[i], HU3D_MOTATTR | HU3D_ATTR_DISPOFF);
        Hu3DMotionAttrSet((HU3DMOTID) motchkMotId[i], HU3D_ATTR_DISPOFF);
    }
    Hu3DModelAttrReset((HU3DMODELID) motchkMdlId[lodNo], HU3D_ATTR_DISPOFF);
}

void MotChkMotionKill(s16 charNo) {
    s16 i;

    HuDataDirClose(dataDirTbl[charNo]);
    for (i = 0; i < 4; i++) {
        Hu3DModelKill((HU3DMODELID) motchkMdlId[i]);
    }
    for (i = 0; i < 4; i++) {
        Hu3DMotionKill((HU3DMOTID) motchkMotId[i]);
        Hu3DMotionKill((HU3DMOTID) motchkMotIdOld[i]);
    }
}

// NONMATCHING: MWCC schedules the repeated trig calls differently than a
// straightforward re-expansion of the paired-single matrix rotation math;
// the values produced are identical but instruction order/regalloc differs.
void DebugCamUpdate(OMOBJ *obj) {
    HuVecF eye;
    HuVecF dir;
    HuVecF axis;
    HuVecF tmp;
    f32 s;
    f32 rad;
    s8 stickX;
    s8 stickY;

    if (HuPadBtnDown[0] & 0x1000) {
        if (&dummyProc != NULL) {
            HuPrcKill((HUPROCESS *) dummyProc);
        }
        omOvlReturnEx(1, TRUE);
        return;
    }

    CRot.x += (f32) (HuPadStkY[0] / 20);
    CRot.y += (f32) (HuPadStkX[0] / 20);
    CZoom += (f32) ((s32) (((u8) HuPadTrigL[0] >> 0x1F) + HuPadTrigL[0]) >> 1);
    CZoom -= (f32) ((s32) (((u8) HuPadTrigR[0] >> 0x1F) + HuPadTrigR[0]) >> 1);

    eye.x = (f32) ((f64) Center.x + ((f64) CZoom * (sin((3.141592653589793 * (f64) CRot.y) / 180.0) * cos((3.141592653589793 * (f64) CRot.x) / 180.0))));
    eye.y = (f32) ((f64) Center.y + ((f64) CZoom * -sin((3.141592653589793 * (f64) CRot.x) / 180.0)));
    eye.z = (f32) ((f64) Center.z + ((f64) CZoom * (cos((3.141592653589793 * (f64) CRot.y) / 180.0) * cos((3.141592653589793 * (f64) CRot.x) / 180.0))));

    dir.x = Center.x - eye.x;
    dir.y = Center.y - eye.y;
    dir.z = Center.z - eye.z;

    tmp.x = (f32) (sin((3.141592653589793 * (f64) CRot.y) / 180.0) * sin((3.141592653589793 * (f64) CRot.x) / 180.0));
    tmp.y = (f32) cos((3.141592653589793 * (f64) CRot.x) / 180.0);
    tmp.z = (f32) (cos((3.141592653589793 * (f64) CRot.y) / 180.0) * sin((3.141592653589793 * (f64) CRot.x) / 180.0));

    rad = CRot.z;
    s = (f32) (((f64) tmp.x * ((f64) (dir.x * dir.z) + ((f64) (1.0f - (dir.x * dir.z)) * cos((3.141592653589793 * (f64) rad) / 180.0)))) +
               ((f64) tmp.y * (((f64) (dir.x * dir.y) * (1.0 - cos((3.141592653589793 * (f64) rad) / 180.0))) - ((f64) dir.z * sin((3.141592653589793 * (f64) rad) / 180.0)))) +
               ((f64) tmp.z * (((f64) (dir.x * dir.z) * (1.0 - cos((3.141592653589793 * (f64) rad) / 180.0))) + ((f64) dir.y * sin((3.141592653589793 * (f64) rad) / 180.0)))));
    axis.x = s;

    s = (f32) (((f64) tmp.y * ((f64) (dir.y * dir.y) + ((f64) (1.0f - (dir.y * dir.y)) * cos((3.141592653589793 * (f64) rad) / 180.0)))) +
               ((f64) tmp.z * (((f64) (dir.x * dir.y) * (1.0 - cos((3.141592653589793 * (f64) rad) / 180.0))) + ((f64) dir.z * sin((3.141592653589793 * (f64) rad) / 180.0)))) +
               ((f64) tmp.x * (((f64) (dir.y * dir.z) * (1.0 - cos((3.141592653589793 * (f64) rad) / 180.0))) - ((f64) dir.x * sin((3.141592653589793 * (f64) rad) / 180.0)))));
    axis.y = s;

    s = (f32) (((f64) tmp.z * ((f64) (dir.z * dir.z) + ((f64) (1.0f - (dir.z * dir.z)) * cos((3.141592653589793 * (f64) rad) / 180.0)))) +
               ((f64) tmp.x * (((f64) (dir.x * dir.z) * (1.0 - cos((3.141592653589793 * (f64) rad) / 180.0))) + ((f64) dir.y * sin((3.141592653589793 * (f64) rad) / 180.0)))) +
               ((f64) tmp.y * (((f64) (dir.y * dir.z) * (1.0 - cos((3.141592653589793 * (f64) rad) / 180.0))) - ((f64) dir.x * sin((3.141592653589793 * (f64) rad) / 180.0)))));
    axis.z = s;

    PSVECCrossProduct(&tmp, &dir, &dir);
    PSVECNormalize(&dir, &dir);

    stickX = (u8) HuPadSubStkX[0] & 0xF8;
    if (stickX != 0) {
        Center.x += 0.05f * (dir.x * (f32) stickX);
        Center.y += 0.05f * (dir.y * (f32) stickX);
        Center.z += 0.05f * (dir.z * (f32) stickX);
    }

    PSVECNormalize(&axis, &dir);
    stickY = -((u8) HuPadSubStkY[0] & 0xF8);
    if (stickY != 0) {
        Center.x += 0.05f * (dir.x * (f32) stickY);
        Center.y += 0.05f * (dir.y * (f32) stickY);
        Center.z += 0.05f * (dir.z * (f32) stickY);
    }
}
