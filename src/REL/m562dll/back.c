/* m562dll/back: the arena backdrop - banner/pillar/lava models, the two bobbing swarm models and a screen-copy
   based water reflection (copy pass in a layer hook, indirect texture draw in a material hook). */

#include <math.h>
#include <string.h>

#include "game/frand.h"
#include "game/hu3d.h"
#include "game/object.h"
#include "game/sprite.h"

#include "REL/m562dll.h"

void HuSprTexLoad(ANIMDATA *anim, s16 bmpNo, s16 texMapId, int wrapS, int wrapT, int filter);
float Hu3DMotionMaxTimeGet(HU3DMODELID modelId);
void fn_1_4404(M562Back *self, s16 model, s16 *phase, s16 *wait);
void fn_1_51C4(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material);

/* a pose of the pillar group: position and scale */
typedef struct M562BackPose {
    Vec pos;
    f32 scale;
} M562BackPose;

static M562BackPose lbl_1_data_60[5] = {
    { { 0.0f, -4000.0f, -26000.0f }, 25.0f },
    { { 0.0f, -2600.0f, -26000.0f }, 40.0f },
    { { 0.0f, -3300.0f, -26000.0f }, 50.0f },
    { { 0.0f, -3300.0f, -26000.0f }, 60.0f },
    { { 0.0f, -3300.0f, -26000.0f }, 80.0f },
};

/* indirect texture matrix: only the y scale (element [1][1]) is rewritten per frame */
static f32 lbl_1_data_B0[2][3] = {
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.01f, 0.0f },
};

static f32 lbl_1_bss_7810;

static const Vec lbl_1_rodata_C8 = { 0.0f, -50.0f, -1700.0f };
static const GXColor lbl_1_rodata_16C = { 0, 0, 0, 0 };
static const GXColor lbl_1_rodata_170 = { 0x40, 0x50, 0x50, 0 };

void fn_1_3808(M562Back *self, M562Work *owner)
{
    CAMERA *cam = &owner->camera[0];
    Vec bobPos;

    memset(self, 0, sizeof(M562Back));
    self->owner = owner;

    self->modelC = Hu3DModelCreateData(0x750008);
    Hu3DModelPosSet(self->modelC, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(self->modelC, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(self->modelC, (u16)cam->camMask);
    Hu3DModelLayerSet(self->modelC, 0);
    Hu3DModelAttrReset(self->modelC, HU3D_ATTR_DISPOFF);

    self->modelE = Hu3DModelCreateData(0x750009);
    Hu3DModelPosSet(self->modelE, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(self->modelE, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(self->modelE, (u16)cam->camMask);
    Hu3DModelLayerSet(self->modelE, 0);
    Hu3DModelAttrReset(self->modelE, HU3D_ATTR_DISPOFF);

    self->model10 = Hu3DModelCreateData(0x75000A);
    Hu3DModelPosSet(self->model10, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(self->model10, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(self->model10, (u16)cam->camMask);
    Hu3DModelLayerSet(self->model10, 0);
    Hu3DModelAttrSet(self->model10, HU3D_ATTR_DISPOFF);

    self->model12 = Hu3DModelCreateData(0x75000B);
    Hu3DModelPosSet(self->model12, -680.0f, 370.0f, -2100.0f);
    Hu3DModelRotSet(self->model12, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(self->model12, (u16)cam->camMask);
    Hu3DModelLayerSet(self->model12, 0);
    Hu3DModelAttrSet(self->model12, HU3D_ATTR_DISPOFF);
    self->motion16 = Hu3DJointMotionData(self->model12, 0x75002B);
    Hu3DMotionSet(self->model12, self->motion16);
    Hu3DMotionSpeedSet(self->model12, 0.0f);
    Hu3DModelAttrReset(self->model12, HU3D_MOTATTR_LOOP);

    self->model14 = Hu3DModelCreateData(0x75000C);
    Hu3DModelPosSet(self->model14, 680.0f, 370.0f, -2100.0f);
    Hu3DModelRotSet(self->model14, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(self->model14, (u16)cam->camMask);
    Hu3DModelLayerSet(self->model14, 0);
    Hu3DModelAttrSet(self->model14, HU3D_ATTR_DISPOFF);
    self->motion18 = Hu3DJointMotionData(self->model14, 0x75002C);
    Hu3DMotionSet(self->model14, self->motion18);
    Hu3DMotionSpeedSet(self->model14, 0.0f);
    Hu3DModelAttrReset(self->model14, HU3D_MOTATTR_LOOP);

    bobPos = lbl_1_rodata_C8;
    self->bobPos = bobPos;
    self->bobTimer = 0;
    self->bobOn = 1;

    self->model1A = Hu3DModelCreateData(0x75000D);
    Hu3DModelPosSet(self->model1A, self->bobPos.x, self->bobPos.y, self->bobPos.z);
    Hu3DModelRotSet(self->model1A, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(self->model1A, (u16)cam->camMask);
    Hu3DModelLayerSet(self->model1A, 0);
    Hu3DModelAttrReset(self->model1A, HU3D_ATTR_DISPOFF);
    Hu3DModelMatHookSet(self->model1A, fn_1_51C4);

    self->model1C = Hu3DModelCreateData(0x75000E);
    Hu3DModelPosSet(self->model1C, self->bobPos.x, self->bobPos.y, self->bobPos.z);
    Hu3DModelRotSet(self->model1C, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(self->model1C, (u16)cam->camMask);
    Hu3DMotionSpeedSet(self->model1C, 0.25f);
    Hu3DModelLayerSet(self->model1C, 0);
    Hu3DModelAttrReset(self->model1C, HU3D_ATTR_DISPOFF);
    self->motion1E = Hu3DJointMotionData(self->model1C, 0x75005F);
    Hu3DMotionSet(self->model1C, self->motion1E);
    Hu3DModelAttrSet(self->model1C, HU3D_MOTATTR_LOOP);

    self->model20 = Hu3DModelCreateData(0x75000F);
    Hu3DModelPosSet(self->model20, self->bobPos.x, self->bobPos.y, self->bobPos.z);
    Hu3DModelRotSet(self->model20, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(self->model20, (u16)cam->camMask);
    Hu3DMotionSpeedSet(self->model20, 0.0f);
    Hu3DModelLayerSet(self->model20, 0);
    Hu3DModelAttrSet(self->model20, HU3D_ATTR_DISPOFF);
    self->motion22 = Hu3DJointMotionData(self->model20, 0x750060);
    Hu3DMotionSet(self->model20, self->motion22);
    Hu3DModelAttrReset(self->model20, HU3D_MOTATTR_LOOP);

    self->model40 = Hu3DModelCreateData(0x750010);
    Hu3DModelPosSet(self->model40, lbl_1_data_60[0].pos.x, lbl_1_data_60[0].pos.y, lbl_1_data_60[0].pos.z);
    Hu3DModelRotSet(self->model40, 37.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(self->model40, 25.0f, 25.0f, 25.0f);
    Hu3DModelCameraSet(self->model40, (u16)cam->camMask);
    Hu3DModelLayerSet(self->model40, 0);
    Hu3DModelAttrReset(self->model40, HU3D_ATTR_DISPOFF);
    self->motion42 = Hu3DJointMotionData(self->model40, 0x75005C);
    Hu3DMotionSet(self->model40, self->motion42);
    Hu3DMotionSpeedSet(self->model40, 0.25f);
    Hu3DModelAttrSet(self->model40, HU3D_MOTATTR_LOOP);

    self->model44 = Hu3DModelCreateData(0x750011);
    Hu3DModelPosSet(self->model44, lbl_1_data_60[0].pos.x, lbl_1_data_60[0].pos.y, lbl_1_data_60[0].pos.z);
    Hu3DModelRotSet(self->model44, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(self->model44, (u16)cam->camMask);
    Hu3DModelLayerSet(self->model44, 0);
    Hu3DModelAttrSet(self->model44, HU3D_ATTR_DISPOFF);
    self->motion48 = Hu3DJointMotionData(self->model44, 0x75005D);
    Hu3DMotionSet(self->model44, self->motion48);
    Hu3DModelAttrReset(self->model44, HU3D_MOTATTR_LOOP);

    self->model46 = Hu3DModelCreateData(0x750012);
    Hu3DModelPosSet(self->model46, lbl_1_data_60[0].pos.x, lbl_1_data_60[0].pos.y, lbl_1_data_60[0].pos.z);
    Hu3DModelRotSet(self->model46, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(self->model46, (u16)cam->camMask);
    Hu3DModelLayerSet(self->model46, 0);
    Hu3DModelAttrSet(self->model46, HU3D_ATTR_DISPOFF);
    self->motion4A = Hu3DJointMotionData(self->model46, 0x75005E);
    Hu3DMotionSet(self->model46, self->motion4A);
    Hu3DModelAttrReset(self->model46, HU3D_MOTATTR_LOOP);

    self->scale = 40.0f;
    self->waitMax = 0x3C;
}

void fn_1_41B4(M562Back *self)
{
    Hu3DMotionKill(self->motion4A);
    Hu3DModelKill(self->model46);
    Hu3DMotionKill(self->motion48);
    Hu3DModelKill(self->model44);
    Hu3DMotionKill(self->motion42);
    Hu3DModelKill(self->model40);
    Hu3DMotionKill(self->motion22);
    Hu3DModelKill(self->model20);
    Hu3DMotionKill(self->motion1E);
    Hu3DModelKill(self->model1C);
    Hu3DModelKill(self->model1A);
    Hu3DMotionKill(self->motion18);
    Hu3DModelKill(self->model14);
    Hu3DMotionKill(self->motion16);
    Hu3DModelKill(self->model12);
    Hu3DModelKill(self->model10);
    Hu3DModelKill(self->modelE);
    Hu3DModelKill(self->modelC);
    memset(self, 0, sizeof(M562Back));
}

void fn_1_427C(M562Back *self)
{
    M562Work *owner = self->owner;
    Vec pos;

    fn_1_4404(self, self->model44, &self->phase44, &self->wait44);
    fn_1_4404(self, self->model46, &self->phase46, &self->wait46);
    if (owner->mode != 4) {
        pos.x = self->bobPos.x;
        pos.y = (f32)((f64)self->bobPos.y
            + 30.0 * sin(3.141592653589793 * (f64)(360.0f * ((f32)(self->bobTimer % 400) / 400.0f)) / 180.0));
        pos.z = self->bobPos.z;
        Hu3DModelPosSet(self->model1A, pos.x, pos.y, pos.z);
        Hu3DModelPosSet(self->model1C, pos.x, pos.y, pos.z);
        Hu3DModelPosSet(self->model20, pos.x, pos.y, pos.z);
        if (self->bobOn != 0) {
            self->bobTimer++;
        }
    }
}

/* one of the two drifting swarm models: wait, then restart its motion with a random orientation and scale */
void fn_1_4404(M562Back *self, s16 model, s16 *phase, s16 *wait)
{
    Mtx m;
    Mtx rot;
    f32 f;
    f32 s;

    switch (*phase) {
    case 0:
        *wait = (s16)(self->waitMax / 2 + frand() % self->waitMax);
        *phase = 1;
        break;
    case 1:
        if ((*wait)-- <= 0) {
            f = (f32)(frand() % 10000) / 10000.0f;
            PSMTXScale(m, 1.0f, 1.0f, 1.0f);
            PSMTXTrans(m, 0.0f, 0.0f, 0.0f);
            PSMTXRotRad(rot, 'X', 0.7853982f);
            PSMTXConcat(rot, m, m);
            PSMTXRotRad(rot, 'Y', 0.017453292f * (360.0f * f - 180.0f));
            PSMTXConcat(rot, m, m);
            PSMTXRotRad(rot, 'X', 0.6457718f);
            PSMTXConcat(rot, m, m);
            PSMTXCopy(m, Hu3DData[model].mtx);
            s = 0.5f + 0.5f * ((f32)(frand() % 10000) / 10000.0f);
            Hu3DModelScaleSet(model, s * self->scale, s * self->scale, s * self->scale);
            Hu3DMotionTimeSet(model, 0.0f);
            Hu3DModelAttrReset(model, HU3D_ATTR_DISPOFF);
            *wait = 0;
            *phase = 2;
        }
        break;
    case 2:
        if (Hu3DMotionTimeGet(model) >= Hu3DMotionMaxTimeGet(model)) {
            Hu3DModelAttrSet(model, HU3D_ATTR_DISPOFF);
            *wait = 0;
            *phase = 0;
        }
        break;
    }
}

/* blend the pillar group between two poses (`t` = 1 is the new pose) for battle part `mode` */
void fn_1_4748(M562Back *self, s16 mode, f32 t)
{
    Vec pos;
    f32 scale;

    switch (mode) {
    case 0:
        pos.x = lbl_1_data_60[0].pos.x * t + lbl_1_data_60[3].pos.x * (1.0f - t);
        pos.y = lbl_1_data_60[0].pos.y * t + lbl_1_data_60[3].pos.y * (1.0f - t);
        pos.z = lbl_1_data_60[0].pos.z * t + lbl_1_data_60[3].pos.z * (1.0f - t);
        scale = lbl_1_data_60[0].scale * t + lbl_1_data_60[3].scale * (1.0f - t);
        Hu3DModelScaleSet(self->model40, scale, scale, scale);
        Hu3DModelPosSet(self->model40, pos.x, pos.y, pos.z);
        Hu3DModelPosSet(self->model44, pos.x, pos.y, pos.z);
        Hu3DModelPosSet(self->model46, pos.x, pos.y, pos.z);
        self->scale = 40.0f;
        self->waitMax = 0x3C;
        Hu3DMotionSpeedSet(self->model44, 1.0f);
        Hu3DMotionSpeedSet(self->model46, 1.0f);
        Hu3DMotionSpeedSet(self->model40, 0.25f);
        break;
    case 1:
        pos.x = lbl_1_data_60[1].pos.x * t + lbl_1_data_60[0].pos.x * (1.0f - t);
        pos.y = lbl_1_data_60[1].pos.y * t + lbl_1_data_60[0].pos.y * (1.0f - t);
        pos.z = lbl_1_data_60[1].pos.z * t + lbl_1_data_60[0].pos.z * (1.0f - t);
        scale = lbl_1_data_60[1].scale * t + lbl_1_data_60[0].scale * (1.0f - t);
        Hu3DModelScaleSet(self->model40, scale, scale, scale);
        Hu3DModelPosSet(self->model40, pos.x, pos.y, pos.z);
        Hu3DModelPosSet(self->model44, pos.x, pos.y, pos.z);
        Hu3DModelPosSet(self->model46, pos.x, pos.y, pos.z);
        self->scale = 45.0f * t + 40.0f * (1.0f - t);
        self->waitMax = 0x3C;
        Hu3DMotionSpeedSet(self->model44, 1.0f);
        Hu3DMotionSpeedSet(self->model46, 1.0f);
        Hu3DMotionSpeedSet(self->model40, 0.25f);
        break;
    case 2:
        pos.x = lbl_1_data_60[2].pos.x * t + lbl_1_data_60[1].pos.x * (1.0f - t);
        pos.y = lbl_1_data_60[2].pos.y * t + lbl_1_data_60[1].pos.y * (1.0f - t);
        pos.z = lbl_1_data_60[2].pos.z * t + lbl_1_data_60[1].pos.z * (1.0f - t);
        scale = lbl_1_data_60[2].scale * t + lbl_1_data_60[1].scale * (1.0f - t);
        Hu3DModelScaleSet(self->model40, scale, scale, scale);
        Hu3DModelPosSet(self->model40, pos.x, pos.y, pos.z);
        Hu3DModelPosSet(self->model44, pos.x, pos.y, pos.z);
        Hu3DModelPosSet(self->model46, pos.x, pos.y, pos.z);
        self->scale = 50.0f * t + 45.0f * (1.0f - t);
        self->waitMax = 0x32;
        Hu3DMotionSpeedSet(self->model44, 1.0f);
        Hu3DMotionSpeedSet(self->model46, 1.0f);
        Hu3DMotionSpeedSet(self->model40, 0.25f);
        break;
    case 3:
        pos.x = lbl_1_data_60[3].pos.x * t + lbl_1_data_60[2].pos.x * (1.0f - t);
        pos.y = lbl_1_data_60[3].pos.y * t + lbl_1_data_60[2].pos.y * (1.0f - t);
        pos.z = lbl_1_data_60[3].pos.z * t + lbl_1_data_60[2].pos.z * (1.0f - t);
        scale = lbl_1_data_60[3].scale * t + lbl_1_data_60[2].scale * (1.0f - t);
        Hu3DModelScaleSet(self->model40, scale, scale, scale);
        Hu3DModelPosSet(self->model40, pos.x, pos.y, pos.z);
        Hu3DModelPosSet(self->model44, pos.x, pos.y, pos.z);
        Hu3DModelPosSet(self->model46, pos.x, pos.y, pos.z);
        self->scale = 55.0f * t + 50.0f * (1.0f - t);
        self->waitMax = 0x28;
        Hu3DMotionSpeedSet(self->model44, 1.0f);
        Hu3DMotionSpeedSet(self->model46, 1.0f);
        Hu3DMotionSpeedSet(self->model40, 0.25f);
        break;
    case 4:
        pos.x = lbl_1_data_60[4].pos.x * t + lbl_1_data_60[3].pos.x * (1.0f - t);
        pos.y = lbl_1_data_60[4].pos.y * t + lbl_1_data_60[3].pos.y * (1.0f - t);
        pos.z = lbl_1_data_60[4].pos.z * t + lbl_1_data_60[3].pos.z * (1.0f - t);
        scale = lbl_1_data_60[4].scale * t + lbl_1_data_60[3].scale * (1.0f - t);
        Hu3DModelScaleSet(self->model40, scale, scale, scale);
        Hu3DModelPosSet(self->model40, pos.x, pos.y, pos.z);
        Hu3DModelPosSet(self->model44, pos.x, pos.y, pos.z);
        Hu3DModelPosSet(self->model46, pos.x, pos.y, pos.z);
        self->scale = 110.0f * t + 55.0f * (1.0f - t);
        self->waitMax = (s16)(20.0f * t + 40.0f * (1.0f - t));
        Hu3DMotionSpeedSet(self->model44, 5.0f * t + (1.0f - t));
        Hu3DMotionSpeedSet(self->model46, 5.0f * t + (1.0f - t));
        Hu3DMotionSpeedSet(self->model40, 2.0f * t + 0.25f * (1.0f - t));
        break;
    }
}

/* layer hook: copy the rendered frame into the reflection texture */
void fn_1_508C(s16 layerNo)
{
    HU3DWATER *water = &Hu3DWaterData;
    GXColor clear = lbl_1_rodata_16C;
    GXColor copyClear;

    if (lbl_1_bss_0->fbTex != NULL && lbl_1_bss_0->mode != 4 && (Hu3DCameraBit & 1) != 0) {
        GXSetTexCopySrc(0, 0, 0x280, 0x1E0);
        GXSetTexCopyDst(0x140, 0xF0, GX_TF_RGB565, GX_TRUE);
        copyClear = clear;
        GXSetCopyClear(copyClear, 0xFFFFFF);
        GXCopyTex(lbl_1_bss_0->fbTex, GX_TRUE);
        if (omUPauseFlag + omPauseChk() == 0) {
        }
    }
}

/* material hook of the water plane: draw the reflection copy through an indirect (ripple) texture */
void fn_1_51C4(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    GXTexObj texObj;
    Mtx reflMtx;
    Mtx lightMtx;
    Mtx invCam;
    Mtx catMtx;
    Mtx scaleMtx;
    GXColor color = lbl_1_rodata_170;

    if (lbl_1_bss_0->fbTex != NULL) {
        if (lbl_1_bss_0->back.rippleOn == 0) {
            lbl_1_data_B0[1][1] = 0.01f;
        } else {
            lbl_1_data_B0[1][1] = 1.0f;
        }
        GXSetNumTexGens(2);
        GXSetNumTevStages(1);
        GXInitTexObj(&texObj, lbl_1_bss_0->fbTex, 0x140, 0xF0, GX_TF_RGB565, GX_CLAMP, GX_CLAMP, GX_FALSE);
        GXInitTexObjLOD(&texObj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
        GXLoadTexObj(&texObj, GX_TEXMAP0);
        HuSprTexLoad(lbl_1_bss_0->fbAnim, 0, 1, 1, 1, 1);
        C_MTXLightPerspective(lightMtx, 45.0f, 1.2f, 0.5f, -0.5f, 0.5f, 0.5f);
        PSMTXIdentity(reflMtx);
        PSMTXTrans(reflMtx, 0.0f, -51.0f, -874.0f);
        PSMTXInverse(Hu3DCameraMtx, invCam);
        PSMTXConcat(invCam, reflMtx, reflMtx);
        PSMTXConcat(lightMtx, Hu3DCameraMtx, catMtx);
        PSMTXConcat(catMtx, reflMtx, catMtx);
        GXLoadTexMtxImm(catMtx, GX_TEXMTX0, GX_MTX3x4);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_POS, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
#ifdef TARGET_PC
        if(!HuInterpDrawF)
#endif
        if (lbl_1_bss_0->back.reflectOn != 0) {
            lbl_1_bss_7810 += 0.05f;
        }
        PSMTXTrans(reflMtx, 0.0f, lbl_1_bss_7810, 0.0f);
        PSMTXScale(scaleMtx, 120.0f, 120.0f, 120.0f);
        PSMTXConcat(scaleMtx, reflMtx, catMtx);
        GXLoadTexMtxImm(catMtx, GX_TEXMTX1, GX_MTX2x4);
        GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);
        if (lbl_1_bss_0->back.reflectOn != 0) {
            GXSetTevColor(GX_TEVREG0, color);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        } else {
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        }
        GXSetNumIndStages(1);
        GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD1, GX_TEXMAP1);
        GXSetIndTexCoordScale(GX_INDTEXSTAGE0, GX_ITS_1, GX_ITS_1);
        GXSetTevIndWarp(GX_TEVSTAGE0, GX_INDTEXSTAGE0, GX_TRUE, GX_FALSE, GX_ITM_0);
        GXSetIndTexMtx(GX_ITM_0, lbl_1_data_B0, -1);
    }
}
