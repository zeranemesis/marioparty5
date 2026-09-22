#include "REL/safDll.h"

#include "game/charman.h"
#include "game/data.h"
#include "game/frand.h"
#include "game/memory.h"
#include "game/pad.h"

#include <math.h>

/*
 * safDll/safanim.c - unused reflective-ball / water rehearsal scene.
 *
 * Not reachable from ObjectSetup()'s normal flow other than through the
 * REL entry point itself: SafAnimMain/SafAnimCameraUpdate are debug-camera
 * test code left in the shipped binary (see SafCameraUpdate in saf.c and
 * SafMotionCameraUpdate in safmotion.c for the near-identical siblings).
 */

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

void ObjectSetup(void);
static void SafBallMatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material);

static f32 indTexMtx[2][3] = { { 0.5f, 0.0f, 0.0f }, { 0.0f, 0.5f, 0.0f } };
static const GXColor BallTevColor = { 0x20, 0xFF, 0xFF, 0x80 };

static s16 ballMdlId[32];
static u8 lbl_1_bss_40[0x1C4]; /* unused padding, not referenced by any function in this file */
static ANIMDATA *waterBumpAnim;
static void *waterFbCopyAnim;

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
    SafObjectSetup();
}

// NONMATCHING: long -O0,p rehearsal sequence; the original keeps far more
// of its local HuVecF/scalar temporaries live in named nonvolatile
// registers/stack slots than this straightforward re-expression needs
// (frame 0x3f0 vs 0xa0 here), and unwinding it into the original's exact
// variable set did not reliably come back closer without guessing dead
// register pressure rather than fixing an actual logic difference. Code
// and constants are otherwise faithful to the original control flow.
void SafAnimMain(void) {
    HU3DMODELID reflectMdlId;
    HU3DMODELID shadowMdlId;
    HU3DMODELID charMdlId;
    HUSPRGRPID sprGrpId;
    HU3DMOTID walkMotId;
    HU3DMOTID runMotId;
    HuVecF shadowPos;
    HuVecF shadowUp;
    HuVecF shadowTarget;
    HuVecF waterPosMin;
    HuVecF waterPosMax;
    s16 i;

    reflectMdlId = Hu3DModelCreateData(0xBA0005);
    Hu3DModelAttrSet(reflectMdlId, HU3D_MOTATTR_LOOP);
    Hu3DReflectModelSet(reflectMdlId);

    shadowMdlId = Hu3DModelCreateData(0xBA0003);
    Hu3DModelShadowMapSet(shadowMdlId);
    Hu3DModelScaleSet(shadowMdlId, 5.0f, 5.0f, 5.0f);
    Hu3DModelAttrSet(shadowMdlId, HU3D_MOTATTR_LOOP);

    Hu3DBGColorSet(0x20, 0, 0);
    Hu3DShadowCreate(30.0f, 20.0f, 2000.0f);
    shadowPos.x = 200.0f;
    shadowPos.y = 1000.0f;
    shadowPos.z = 200.0f;
    shadowUp.x = 0.0f;
    shadowUp.y = 1.0f;
    shadowUp.z = 0.0f;
    shadowTarget.x = 0.0f;
    shadowTarget.y = 0.0f;
    shadowTarget.z = 0.0f;
    Hu3DShadowPosSet(&shadowPos, &shadowUp, &shadowTarget);
    Hu3DShadowTPLvlSet(0.6f);

    sprGrpId = HuSprGrpCreate(1);
    HuSprGrpMemberSet(sprGrpId, 0, HuSprCreate(Hu3DReflectModelAnim, 1000, 0));
    HuSprGrpPosSet(sprGrpId, 64.0f, 64.0f);
    HuSprAttrSet(sprGrpId, 0, HUSPR_ATTR_DISPOFF);

    waterFbCopyAnim = HuMemDirectMallocNum(HEAP_MODEL, GXGetTexBufferSize(0x280, 0x1E0, GX_TF_RGB565, GX_FALSE, 0), HU_MEMNUM_OVL);
    waterBumpAnim = HuSprAnimDataRead(0x21000A);
    Hu3DFbCopyLayerSet(2, 0, 0, 0x280, 0x1E0, GX_TF_RGB565, 0, waterFbCopyAnim);

    for (i = 0; i < 32; i++) {
        if (i == 0) {
            ballMdlId[i] = Hu3DModelCreateData(0xBA0006);
        } else {
            ballMdlId[i] = Hu3DModelLink(ballMdlId[0]);
        }
        Hu3DModelPosSet(ballMdlId[i], (f32) (frandmod(400) - 200), (f32) (frandmod(100) + 200), (f32) (frandmod(400) - 200));
        Hu3DModelScaleSet(ballMdlId[i], 0.2f, 0.2f, 0.2f);
        Hu3DModelMatHookSet(ballMdlId[i], SafBallMatHook);
        Hu3DModelLayerSet(ballMdlId[i], 2);
    }

    charMdlId = CharModelCreate(0, 2);
    Hu3DModelAttrSet(charMdlId, HU3D_MOTATTR_LOOP);
    Hu3DModelPosSet(charMdlId, 200.0f, 0.0f, 0.0f);
    Hu3DModelLayerSet(charMdlId, 1);
    walkMotId = CharMotionCreate(0, 0x8C0000);
    CharMotionSet(0, walkMotId);
    runMotId = CharMotionCreate(0, 0x8C0001);
    CharEffectLayerSet(1);

    waterPosMin.x = -1000.0f;
    waterPosMin.y = 20.0f;
    waterPosMin.z = -1000.0f;
    waterPosMax.x = 1000.0f;
    waterPosMax.y = 20.0f;
    waterPosMax.z = 1000.0f;
    Hu3DWaterCreate(1, (void *) 1, (void *) 1, (void *) 1, TRUE, &waterPosMin, &waterPosMax);
    Hu3DWaterTexScaleSet(1.0f, 1.0f, 1.0f);
    Hu3DWaterHiliteSet(0.5f);

    WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, 0x14);
    while (WipeCheckEnd()) {
        HuPrcVSleep();
    }

    Hu3DModelCameraInfoSet(Hu3DModelCreateData(0xBA0008), 1);
    Hu3DCameraMotionStart(Hu3DModelCreateData(0xBA0008), 1);

    for (;;) {
        if (HuPadBtn[0] & PAD_BUTTON_A) {
            HuSprAttrReset(sprGrpId, 0, HUSPR_ATTR_DISPOFF);
        } else {
            HuSprAttrSet(sprGrpId, 0, HUSPR_ATTR_DISPOFF);
        }
        if (HuPadBtnDown[0] & PAD_BUTTON_X) {
            Hu3DModelAttrSet(charMdlId, HU3D_MOTATTR_PAUSE);
            CharMotionShiftSet(0, runMotId, 0.0f, 100.0f, HU3D_MOTATTR_LOOP);
        }
        if (HuPadBtnDown[0] & PAD_BUTTON_START) {
            break;
        }
        HuPrcVSleep();
    }

    WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 0x14);
    while (WipeCheckEnd()) {
        HuPrcVSleep();
    }
    omOvlReturnEx(1, TRUE);
    HuPrcEnd();
    for (;;) {
        HuPrcVSleep();
    }
}

void SafAnimExit(void) {
    for (;;) {
        if (HuPadBtnDown[0] & PAD_BUTTON_B) {
            WipeCreate(WIPE_MODE_OUT, 0x84, 0x3C);
            while (WipeCheckEnd()) {
                HuPrcVSleep();
            }
            HuPrcSleep(0x3C);
            WipeCreate(WIPE_MODE_IN, 0x84, 0x3C);
            while (WipeCheckEnd()) {
                HuPrcVSleep();
            }
        }
        HuPrcVSleep();
    }
}

// NONMATCHING: same debug-camera math as DebugCamUpdate (motchkDll/motchk.c)
// and SafCameraUpdate/SafMotionCameraUpdate; MWCC's scheduling of the
// repeated trig calls under -O0,p does not come back the same from a
// straightforward re-expansion of the paired-single rotation math.
void SafAnimCameraUpdate(void) {
    HuVecF eye;
    HuVecF dir;
    HuVecF axis;
    HuVecF tmp;
    f32 s;
    f32 rad;
    s8 stickX;
    s8 stickY;

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

// NONMATCHING: MWCC's stack-slot allocation for the reused Mtx scratch
// buffers comes back slightly differently under -O0,p; the code and
// constants are otherwise identical (81% instruction match).
static void SafBallMatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material) {
    GXTexObj texObj;
    HU3DCAMERA *cameraP;
    Mtx proj;
    Mtx camInv;
    Mtx modelView;
    Mtx tmp;
    Mtx texScale;
    GXColor tevColor;

    tevColor = BallTevColor;
    cameraP = &Hu3DCamera[Hu3DCameraNo];

    GXInitTexObj(&texObj, waterFbCopyAnim, 0x280, 0x1E0, GX_TF_RGB565, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXInitTexObjLOD(&texObj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    HuSprTexLoad(waterBumpAnim, 0, GX_TEXMAP1, GX_REPEAT, GX_REPEAT, GX_LINEAR);

    GXSetNumTexGens(2);
    GXSetNumTevStages(1);

    C_MTXLightPerspective(proj, cameraP->fov, 1.2f, 0.5f, -0.5f, 0.5f, 0.5f);
    PSMTXInverse(Hu3DCameraMtx, camInv);
    PSMTXConcat(camInv, drawObj->matrix, modelView);
    PSMTXConcat(proj, Hu3DCameraMtx, tmp);
    PSMTXConcat(tmp, modelView, tmp);
    GXLoadTexMtxImm(tmp, GX_TEXMTX1, GX_MTX3x4);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_POS, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);

    GXSetTevColor(GX_TEVREG0, tevColor);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);

    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR0);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_TEXC, GX_CC_A0, GX_CC_CPREV);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);

    GXSetNumIndStages(1);
    PSMTXTrans(modelView, 0.0f, 0.0f, 0.0f);
    PSMTXScale(texScale, 0.2f, 0.2f, 1.0f);
    PSMTXConcat(texScale, modelView, tmp);
    GXLoadTexMtxImm(tmp, GX_TEXMTX0, GX_MTX2x4);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD1, GX_TEXMAP1);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE0, GX_ITS_1, GX_ITS_1);
    GXSetTevIndWarp(GX_TEVSTAGE0, GX_INDTEXSTAGE0, GX_TRUE, GX_FALSE, GX_ITM_0);
    GXSetIndTexMtx(GX_ITM_0, indTexMtx, -1);

    Hu3DMatLightSet(drawObj->model, 0, material->hiliteScale);
}
