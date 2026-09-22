#include "REL/safDll.h"

#include "game/data.h"
#include "game/memory.h"
#include "game/pad.h"
#include "game/printfunc.h"

#include <math.h>

/*
 * safDll/saf.c - "Safari"-ish water/reflection test stage, the module's
 * only reachable entry point (called from ObjectSetup() in safanim.c).
 * Sets up 8 split-screen cameras (SafObjectSetup), a single water plane
 * that reflects into an indirect-texture ripple (SafWaterMatHook), and a
 * debug camera identical in spirit to DebugCamUpdate in motchkDll.
 */

static void SafWaterFbCopy(s16 layerNo);
static void SafWaterMatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material);

static f32 indTexMtx[2][3] = { { 0.02f, 0.0f, 0.0f }, { 0.0f, 0.02f, 0.0f } };
static const GXColor WaterTevColor = { 0x20, 0xFF, 0xFF, 0x80 };

static f32 waterTransX;
static HUPROCESS *objman;
static ANIMDATA *waterSurfaceAnim;
static ANIMDATA *waterBumpAnim;
static void *waterFbCopyAnim;
static OMOBJ *outview;
static OMOBJ *cameraObj;

void SafObjectSetup(void) {
    s16 i;
    s16 col;

    OSReport("******* SAFObjectSetup *********\n");
    objman = omInitObjMan(0x32, 0x2000);
    Hu3DCameraCreate(0xFF);
    for (i = 0; i < 8; i++) {
        CRotM[i].x = -20.0f;
        CRotM[i].y = 0.0f;
        CRotM[i].z = 0.0f;
        CenterM[i].x = 0.0f;
        CenterM[i].y = 0.0f;
        CenterM[i].z = 0.0f;
        CZoomM[i] = 500.0f;
        col = i % 4;
        Hu3DCameraPerspectiveSet(1 << i, 50.0f, 20.0f, 2000.0f, 0.31666666f);
        Hu3DCameraViewportSet(1 << i, (f32) ((col * 0x98) + 0x10), 0.0f, 152.0f, 480.0f, 0.0f, 1.0f);
        Hu3DCameraScissorSet(1 << i, (col * 0x98) + 0x10, 0, 0x98, 0x1E0);
    }
    outview = omAddObjEx(objman, 0x7FDA, 0, 0, -1, omOutViewMulti);
    outview->work[0] = 8;
    cameraObj = omAddObjEx(objman, 0, 0x40, 0, -1, SafCameraUpdate);
    HuPrcChildCreate(SafMain, 0x64, 0x3000, 0, objman);
    Hu3DGLightCreate(0.0f, 1000.0f, 1000.0f, 0.0f, -1.0f, -1.0f, 0xFF, 0xFF, 0xFF);
}

void SafMain(void) {
    HU3DMODELID waterMdlId;

    Hu3DModelCameraSet(Hu3DModelCreateData(0xBA0003), 0x0F);

    waterFbCopyAnim = HuMemDirectMallocNum(HEAP_MODEL, GXGetTexBufferSize(0x140, 0xF0, GX_TF_RGB565, GX_FALSE, 0), HU_MEMNUM_OVL);
    waterBumpAnim = HuSprAnimDataRead(0x21000A);
    waterSurfaceAnim = HuSprAnimDataRead(0x21000C);

    waterMdlId = Hu3DModelCreateData(0xBA0009);
    Hu3DModelCameraSet(waterMdlId, 0xF0);
    Hu3DModelMatHookSet(waterMdlId, SafWaterMatHook);
    Hu3DLayerHookSet(8, SafWaterFbCopy);
    Hu3DModelPosSet(waterMdlId, 0.0f, 50.0f, 0.0f);
    Hu3DModelScaleSet(waterMdlId, 0.2f, 0.2f, 0.2f);

    WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, 0x14);
    while (WipeCheckEnd()) {
        HuPrcVSleep();
    }

    for (;;) {
        waterTransX += 0.002f;
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

// NONMATCHING: same debug-camera math as DebugCamUpdate (motchkDll/motchk.c)
// and SafAnimCameraUpdate/SafMotionCameraUpdate; MWCC's scheduling of the
// repeated trig calls under -O0,p does not come back the same from a
// straightforward re-expansion of the paired-single rotation math.
void SafCameraUpdate(OMOBJ *obj) {
    HuVecF eye;
    HuVecF dir;
    HuVecF axis;
    HuVecF tmp;
    f32 s;
    f32 rad;
    s8 stickX;
    s8 stickY;
    s16 i;

    CRotM->x += (f32) (HuPadStkY[0] / 20);
    CRotM->y += (f32) (HuPadStkX[0] / 20);
    *CZoomM += (f32) ((s32) (((u8) HuPadTrigL[0] >> 0x1F) + HuPadTrigL[0]) >> 1);
    *CZoomM -= (f32) ((s32) (((u8) HuPadTrigR[0] >> 0x1F) + HuPadTrigR[0]) >> 1);

    eye.x = (f32) ((f64) CenterM->x + ((f64) *CZoomM * (sin((3.141592653589793 * (f64) CRotM->y) / 180.0) * cos((3.141592653589793 * (f64) CRotM->x) / 180.0))));
    eye.y = (f32) ((f64) CenterM->y + ((f64) *CZoomM * -sin((3.141592653589793 * (f64) CRotM->x) / 180.0)));
    eye.z = (f32) ((f64) CenterM->z + ((f64) *CZoomM * (cos((3.141592653589793 * (f64) CRotM->y) / 180.0) * cos((3.141592653589793 * (f64) CRotM->x) / 180.0))));

    dir.x = CenterM->x - eye.x;
    dir.y = CenterM->y - eye.y;
    dir.z = CenterM->z - eye.z;

    tmp.x = (f32) (sin((3.141592653589793 * (f64) CRotM->y) / 180.0) * sin((3.141592653589793 * (f64) CRotM->x) / 180.0));
    tmp.y = (f32) cos((3.141592653589793 * (f64) CRotM->x) / 180.0);
    tmp.z = (f32) (cos((3.141592653589793 * (f64) CRotM->y) / 180.0) * sin((3.141592653589793 * (f64) CRotM->x) / 180.0));

    rad = CRotM[1].z;
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
        CenterM->x += 0.05f * (dir.x * (f32) stickX);
        CenterM->y += 0.05f * (dir.y * (f32) stickX);
        CenterM->z += 0.05f * (dir.z * (f32) stickX);
    }

    PSVECNormalize(&axis, &dir);
    stickY = -((u8) HuPadSubStkY[0] & 0xF8);
    if (stickY != 0) {
        CenterM->x += 0.05f * (dir.x * (f32) stickY);
        CenterM->y += 0.05f * (dir.y * (f32) stickY);
        CenterM->z += 0.05f * (dir.z * (f32) stickY);
    }

    /* replicate camera 0's view onto every other split-screen camera,
     * offsetting the yaw of each additional pair by 90 degrees */
    for (i = 0; i < 4; i++) {
        CRotM[i] = CRotM[0];
        CenterM[i] = CenterM[0];
        CZoomM[i] = CZoomM[0];
        CRotM[i + 4] = CRotM[0];
        CenterM[i + 4] = CenterM[0];
        CZoomM[i + 4] = CZoomM[0];
        CRotM[i].y += (f32) (i * 90);
        CRotM[i + 4].y += (f32) (i * 90);
    }
}

static void SafWaterFbCopy(s16 layerNo) {
    HU3DWATER *waterP = &Hu3DWaterData;

    if (Hu3DCameraBit & 8) {
        GXSetTexCopySrc(0, 0, 0x280, 0x1E0);
        GXSetTexCopyDst(0x140, 0xF0, GX_TF_RGB565, GX_TRUE);
        GXCopyTex(waterFbCopyAnim, GX_FALSE);
    }
    (void) waterP;
}

static void SafWaterMatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material) {
    GXTexObj texObj;
    HU3DCAMERA *cameraP;
    Mtx proj;
    Mtx camInv;
    Mtx modelView;
    Mtx tmp;
    Mtx texTrans;
    Mtx texScale;
    Mtx texMtx;
    GXColor tevColor;

    tevColor = WaterTevColor;
    cameraP = &Hu3DCamera[Hu3DCameraNo];

    GXInitTexObj(&texObj, waterFbCopyAnim, 0x140, 0xF0, GX_TF_RGB565, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXInitTexObjLOD(&texObj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&texObj, GX_TEXMAP1);
    HuSprTexLoad(waterBumpAnim, 0, GX_TEXMAP2, GX_REPEAT, GX_REPEAT, GX_LINEAR);
    HuSprTexLoad(waterSurfaceAnim, 0, GX_TEXMAP3, GX_REPEAT, GX_REPEAT, GX_LINEAR);

    GXSetNumTexGens(3);
    GXSetNumTevStages(3);

    C_MTXLightPerspective(proj, cameraP->fov, 1.3333334f, 0.5f, -0.5f, (f32) (0.14375 + (0.2375 * (f64) (Hu3DCameraNo - 4))), 0.5f);
    PSMTXInverse(Hu3DCameraMtx, camInv);
    PSMTXConcat(camInv, drawObj->matrix, modelView);
    PSMTXConcat(proj, Hu3DCameraMtx, tmp);
    PSMTXConcat(tmp, modelView, tmp);
    GXLoadTexMtxImm(tmp, GX_TEXMTX1, GX_MTX3x4);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_POS, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);

    PSMTXTrans(texTrans, waterTransX, 0.0f, 0.0f);
    PSMTXScale(texScale, 0.8f, 0.8f, 1.0f);
    PSMTXConcat(texScale, texTrans, texMtx);
    GXLoadTexMtxImm(texMtx, GX_TEXMTX0, GX_MTX2x4);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD2, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);

    GXSetTevColor(GX_TEVREG0, tevColor);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP1, GX_COLOR0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);

    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD2, GX_TEXMAP0, GX_COLOR0);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_TEXC, GX_CC_C0, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);

    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD1, GX_TEXMAP3, GX_COLOR0);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_TEXC, GX_CC_A0, GX_CC_CPREV);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);

    GXSetNumIndStages(1);
    GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD1, GX_TEXMAP2);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE0, GX_ITS_1, GX_ITS_1);
    GXSetTevIndWarp(GX_TEVSTAGE0, GX_INDTEXSTAGE0, GX_TRUE, GX_FALSE, GX_ITM_0);
    GXSetIndTexMtx(GX_ITM_0, indTexMtx, -1);
}
