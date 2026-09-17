#include "REL/partyresultdll.h"

#define TOUEI_FB_W 80
#define TOUEI_FB_H 80

#define TOUEI_MAX GW_PLAYER_MAX

static void *ToueiFbCopyBuf[TOUEI_MAX];
static HuVecF ToueiFbCopyPos[TOUEI_MAX];
static s16 ToueiNo;

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
    15,
    18,
    12,
    20,
    25,
    6,
    15,
    20,
    10,
    15,
    15,
    15,
    15
};

static void ToueiFBCopy(s16 layerNo);
static void ToueiZClear(void);
static void ToueiMatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material);

void ToueiDispInit(HU3DMODELID *mdlId)
{
    s16 i;
    HuVecF pos3D;
    HuVecF pos2D;
    s16 posX, posY;
    for(i=0; i<TOUEI_MAX; i++) {
        ToueiFbCopyBuf[i] = HuMemDirectMallocNum(HEAP_MODEL, GXGetTexBufferSize(TOUEI_FB_W, TOUEI_FB_H, GX_TF_RGB565, GX_FALSE, 0), HU_MEMNUM_OVL);
        Hu3DModelObjPosGet(PResPlayerModelId[i], ToueiCenterObj[PResCharNo[i]], &pos3D);
        pos3D.y += ToueiCenterOfsY[PResCharNo[i]];
        Hu3D3Dto2D(&pos3D, HU3D_CAM0, &pos2D);
        posX = pos2D.x*(640.0/576.0);
        posY = pos2D.y;
        posX &= 0xFFFE;
        posY &= 0xFFFE;
        ToueiFbCopyPos[i].x = posX;
        ToueiFbCopyPos[i].y = posY;
        Hu3DModelMatHookSet(mdlId[i], ToueiMatHook);
    }
    Hu3DLayerHookSet(8, ToueiFBCopy);
}

static void ToueiFBCopy(s16 layerNo)
{
    if(Hu3DCameraNo != 0) {
        Hu3DGLightPosSet(PResLightId, 1000, 1000, 1000, -1, -1, -1);
    } else {
        s16 i;
        for(i=0; i<TOUEI_MAX; i++) {
            Hu3DFbCopyExec(ToueiFbCopyPos[i].x-(TOUEI_FB_W/2), ToueiFbCopyPos[i].y-(TOUEI_FB_H/2), TOUEI_FB_W, TOUEI_FB_H, GX_TF_RGB565, GX_FALSE, ToueiFbCopyBuf[i]);
        }
        ToueiZClear();
        ToueiNo = 0;
    }
}

static void ToueiZClear(void)
{
    HU3DCAMERA *cameraP = &Hu3DCamera[0];
    Mtx44 proj;
    Mtx modelview;
    Vec pos;
    Vec target;
    Vec up;
    GXColor color;
    float z;
    
    MTXPerspective(proj, cameraP->fov, cameraP->aspect, cameraP->near, cameraP->far);
    GXSetProjection(proj, GX_PERSPECTIVE);
    GXSetViewport(0, 0, 640, 480, 0, 1);
    GXSetScissor(0, 0, 640, 480);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    color.r = color.g = color.b = 0;
    GXSetTevColor(GX_TEVREG0, color);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetCullMode(GX_CULL_NONE);
    GXSetAlphaCompare(GX_GEQUAL, 0, GX_AOP_AND, GX_GEQUAL, 0);
    GXSetZMode(GX_TRUE, GX_ALWAYS, GX_TRUE);

    pos.x = pos.y = pos.z = 0;
    target.x = target.y = 0;
    target.z = -100;
    up.x = up.z = 0;
    up.y = 1;
    MTXLookAt(modelview, &pos, &up, &target);
    GXLoadPosMtxImm(modelview, GX_PNMTX0);
    pos.x = (10000*-sin(cameraP->fov/2))*1.2f;
    pos.y = 10000*-sin(cameraP->fov/2)*(1/1.2f);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(-pos.x, -pos.y, -10000);
    GXPosition3f32(pos.x, -pos.y, -10000);
    GXPosition3f32(pos.x, pos.y, -10000);
    GXPosition3f32(-pos.x, pos.y, -10000);
}

static void ToueiMatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    GXTexObj texObj;
    GXColor color = { 32, 32, 32, 128 };
    HU3DCAMERA *cameraP = &Hu3DCamera[Hu3DCameraNo];
    GXInitTexObj(&texObj, ToueiFbCopyBuf[ToueiNo], TOUEI_FB_W, TOUEI_FB_H, GX_TF_RGB565, GX_CLAMP, GX_CLAMP, GX_FALSE);
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
