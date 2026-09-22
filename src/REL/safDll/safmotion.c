#include "REL/safDll.h"

#include "game/data.h"
#include "game/pad.h"
#include "game/printfunc.h"

#include <math.h>

/*
 * safDll/safmotion.c - unused/debug camera rehearsal for the motion test,
 * exactly the same pattern as safanim.c and DebugCamUpdate in motchkDll.
 * SafMotionObjectSetup is never called from ObjectSetup(); it and its
 * children are present in the shipped binary but unreachable.
 */

static HUPROCESS *objman;
static u8 lbl_1_bss_4[0x40]; /* unused padding, not referenced by any function in this file */
static OMOBJ *outview;
static OMOBJ *cameraObj;

void SafMotionObjectSetup(void) {
    HU3DLIGHTID lightId;

    OSReport("******* SAFObjectSetup *********\n");
    objman = omInitObjMan(0x32, 0x2000);
    CRot.x = -20.0f;
    CRot.y = 0.0f;
    CRot.z = 0.0f;
    Center.x = 0.0f;
    Center.y = 0.0f;
    Center.z = 0.0f;
    CZoom = 500.0f;
    Hu3DCameraCreate(1);
    Hu3DCameraPerspectiveSet(1, 45.0f, 20.0f, 15000.0f, 1.2f);
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    outview = omAddObjEx(objman, 0x7FDA, 0, 0, -1, omOutView);
    cameraObj = omAddObjEx(objman, 0, 0x40, 0, -1, SafMotionCameraUpdate);
    HuPrcChildCreate(SafMotionMain, 0x64, 0x3000, 0, objman);
    lightId = Hu3DGLightCreate(0.0f, 1000.0f, 1000.0f, 0.0f, -1.0f, -1.0f, 0xFF, 0xFF, 0xFF);
    Hu3DGLightInfinitytSet(lightId);
}

void SafMotionMain(void) {
    HU3DMODELID mdlId;
    f32 waterTransX;

    mdlId = Hu3DModelCreateData(0xBA000A);
    Hu3DModelAttrSet(mdlId, HU3D_MOTATTR_LOOP);

    WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, 0x14);
    while (WipeCheckEnd()) {
        HuPrcVSleep();
    }

    waterTransX = 0.0f;
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
// and SafAnimCameraUpdate/SafCameraUpdate; MWCC's scheduling of the
// repeated trig calls under -O0,p does not come back the same from a
// straightforward re-expansion of the paired-single rotation math.
void SafMotionCameraUpdate(OMOBJ *obj) {
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
