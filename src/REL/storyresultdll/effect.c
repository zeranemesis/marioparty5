#include "REL/storyresultdll.h"

#include "math.h"
#include "humath.h"

#define TOUEI_FB_W 80
#define TOUEI_FB_H 80

static void ToueiFBCopy(s16 layerNo);
static void RankGlowHook(s16 layerNo);
static void ToueiMatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material);

static char *ToueiCenterObj[15] = {
    "root_head",
    "root_head",
    "ske_head",
    "ske_head",
    "ske_head",
    "ske_head",
    "ske_head",
    "ske_head",
    "ske_head",
    "ske_head",
    "ske_head",
    "ske_head",
    "ske_head",
    "ske_head",
    "ske_head",
};

static float ToueiCenterOfsY[13] = {
    15.0f,
    18.0f,
    12.0f,
    20.0f,
    15.0f,
    6.0f,
    15.0f,
    20.0f,
    10.0f,
    15.0f,
    15.0f,
    15.0f,
    15.0f,
};

f32 CapsuleGlowTPLvl;
static s16 ToueiNo;
static HuVecF ToueiFbCopyPos;
static void *RankGlowCopyBuf;
static void *ToueiFbCopyBuf;

// NONMATCHING: register allocation differs in the framebuffer-copy position computation
void ToueiDispInit(void) {
    HuVecF pos3D;
    HuVecF pos2D;
    s16 charNo = GwPlayerConf->charNo;

    ToueiFbCopyBuf = HuMemDirectMallocNum(HEAP_MODEL, GXGetTexBufferSize(TOUEI_FB_W, TOUEI_FB_H, GX_TF_RGB565, GX_FALSE, 0), HU_MEMNUM_OVL);
    Hu3DModelObjPosGet(CharMdlId, ToueiCenterObj[charNo], &pos3D);
    pos3D.y += ToueiCenterOfsY[charNo];
    Hu3D3Dto2D(&pos3D, HU3D_CAM0, &pos2D);
    ToueiFbCopyPos.x = (s16)((s32)(1.1111111111111112 * (f64)pos2D.x) & 0xFFFE);
    ToueiFbCopyPos.y = (s16)((s32)pos2D.y & 0xFFFE);
    Hu3DModelMatHookSet(CapsuleMdlId, ToueiMatHook);
    RankGlowCopyBuf = HuMemDirectMallocNum(HEAP_MODEL, GXGetTexBufferSize(640, 480, GX_TF_RGBA8, GX_FALSE, 0), HU_MEMNUM_OVL);
    Hu3DLayerHookSet(8, ToueiFBCopy);
    CapsuleGlowTPLvl = 0.0f;
    Hu3DLayerHookSet(10, RankGlowHook);
}

static void ToueiFBCopy(s16 layerNo) {
    if (Hu3DCameraNo == 0) {
        Hu3DFbCopyExec(ToueiFbCopyPos.x - 40, ToueiFbCopyPos.y - 40, TOUEI_FB_W, TOUEI_FB_H, GX_TF_RGB565, GX_FALSE, ToueiFbCopyBuf);
        DispZClear();
        ToueiNo = 0;
    }
}

// NONMATCHING: register allocation and rodata color reconstruction differ
static void RankGlowHook(s16 layerNo) {
    Mtx44 proj;
    Mtx modelview;
    GXColor color;

    switch (Hu3DCameraNo) {
    case 1:
        Hu3DFbCopyExec(0, 0, 640, 480, GX_TF_RGBA8, GX_FALSE, RankGlowCopyBuf);
        DispZClear();
        break;

    case 2:
        MTXOrtho(proj, 0.0f, 480.0f, 0.0f, 640.0f, 0.0f, 10.0f);
        GXSetProjection(proj, GX_ORTHOGRAPHIC);
        GXSetViewport(0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
        GXSetScissor(0, 0, 640, 480);
        GXSetNumTexGens(1);
        GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
        color.r = color.g = color.b = 255;
        color.a = (u8)(255.0f * CapsuleGlowTPLvl);
        GXSetTevColor(GX_TEVREG0, color);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetNumTevStages(1);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_SPOT);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_NOOP);
        GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
        GXSetZCompLoc(GX_FALSE);
        MTXIdentity(modelview);
        GXLoadPosMtxImm(modelview, GX_PNMTX0);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        Hu3DTexLoad(RankGlowCopyBuf, 640, 480, GX_TF_RGBA8, GX_CLAMP, GX_CLAMP, GX_FALSE, GX_TEXMAP0);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(0.0f, 0.0f, 0.0f);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(640.0f, 0.0f, 0.0f);
        GXTexCoord2f32(1.0f, 0.0f);
        GXPosition3f32(640.0f, 480.0f, 0.0f);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3f32(0.0f, 480.0f, 0.0f);
        GXTexCoord2f32(0.0f, 1.0f);
        break;
    }
}

// NONMATCHING: register allocation differs in the perspective/quad setup
void DispZClear(void) {
    Mtx44 proj;
    Mtx modelview;
    Vec pos;
    Vec target;
    Vec up;
    GXColor color;
    HU3DCAMERA *cameraP = Hu3DCamera;
    float x, y;

    MTXPerspective(proj, cameraP->fov, cameraP->aspect, cameraP->near, cameraP->far);
    GXSetProjection(proj, GX_PERSPECTIVE);
    GXSetViewport(0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    GXSetScissor(0, 0, 640, 480);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_TEX_ST, GX_RGBA6, 0);
    color.r = color.g = color.b = 0;
    color.a = 0;
    GXSetTevColor(GX_TEVREG0, color);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetCullMode(GX_CULL_NONE);
    GXSetAlphaCompare(GX_GEQUAL, 0, GX_AOP_AND, GX_GEQUAL, 0);
    GXSetZMode(GX_TRUE, GX_ALWAYS, GX_TRUE);
    GXSetBlendMode(GX_BM_NONE, GX_BL_ZERO, GX_BL_ZERO, GX_LO_CLEAR);

    pos.x = pos.y = pos.z = 0.0f;
    target.x = target.y = 0.0f;
    target.z = -100.0f;
    up.x = up.z = 0.0f;
    up.y = 1.0f;
    MTXLookAt(modelview, &pos, &up, &target);
    GXLoadPosMtxImm(modelview, GX_PNMTX0);
    x = (10000.0f * -sin(cameraP->fov * 0.5f)) * 1.2f;
    y = 10000.0f * -sin(cameraP->fov * 0.5f) * (1.0f / 1.2f);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(-x, -y, -10000.0f);
    GXPosition3f32(x, -y, -10000.0f);
    GXPosition3f32(x, y, -10000.0f);
    GXPosition3f32(-x, y, -10000.0f);
}

// NONMATCHING: register allocation differs
static void ToueiMatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material) {
    GXTexObj texObj;
    GXColor color = { 32, 32, 32, 128 };

    GXInitTexObj(&texObj, ToueiFbCopyBuf, TOUEI_FB_W, TOUEI_FB_H, GX_TF_RGB565, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXInitTexObjLOD(&texObj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    GXSetTevColor(GX_TEVREG0, color);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    ToueiNo++;
}
