#include "REL/storyresultdll.h"

#include "math.h"
#include "humath.h"

typedef struct RankPos_s {
    f32 x;
    f32 y;
    f32 z;
} RANK_POS;

static void RankBackDraw(s16 layerNo);
static void RankObjRotExec(void);

static RANK_POS RankPosTbl[6] = {
    { 88.0f, 340.0f, 8400.0f },
    { 188.0f, 205.0f, 8400.0f },
    { 288.0f, 340.0f, 8400.0f },
    { 388.0f, 205.0f, 8400.0f },
    { 488.0f, 340.0f, 8400.0f },
    { 288.0f, 250.0f, 4400.0f },
};

static HU3DPARMANPARAM RankParmanParam = {
    10,
    0,
    50.0f,
    10.0f,
    90.0f,
    { 0.0f, -0.05f, 0.0f },
    15.0f,
    0.9f,
    8.0f,
    0.95f,
    2,
    {
        { 255, 255, 255, 255 },
        { 255, 255, 64, 255 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
    },
    {
        { 255, 128, 128, 0 },
        { 255, 64, 32, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
    },
};

static f64 BackScrollX;
static f64 BackScrollY;
static s16 RankParmanId;
static ANIMDATA *BackAnim;
static s16 RankEffModelId[4];
static s16 RankModelId[5];
static s16 RankBoardModelId[5];
static s16 RankHelpWin;
static s16 RankStoryWin;
static s16 RankBoardNameGrpId;
static s16 RankBarGrpId;
static GXColor RankBackColor = { 255, 255, 255, 255 };

// NONMATCHING: register allocation differs across this large setup function
void SRankCreate(void) {
    HuVecF pos2D;
    HuVecF pos3D;
    s16 i;
    s16 boardModelId;
    s16 modelId;
    u8 charNo;
    s16 lastNo = SRankWork.storyNo - 1;

    BackAnim = HuSprAnimRead(HuDataSelHeapReadNum(0xC20001, HU_MEMNUM_OVL, HEAP_MODEL));
    RankStoryWin = HuWinCreate(10.0f + (510.0f - 0x160), 44.0f, 0x160, 0x5E, 0);
    HuWinBGTPLvlSet(RankStoryWin, 0.0f);
    HuWinPriSet(RankStoryWin, 200);
    HuWinMesSpeedSet(RankStoryWin, 0);
    HuWinDrawNoSet(RankStoryWin, 0x40);
    HuWinInsertMesSet(RankStoryWin, GwPlayerConf->charNo, 0);
    HuWinInsertMesSet(RankStoryWin, SRankWork.dif + 0xB000C, 1);
    HuWinInsertMesSet(RankStoryWin, SRankWork.rank + 0xB0010, 2);
    HuWinMesSet(RankStoryWin, 0xB000B);
    RankBarGrpId = HuSprGrpCreate(1);
    HuSprGrpMemberSet(RankBarGrpId, 0, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(0xC20000, HU_MEMNUM_OVL, HEAP_MODEL)), 300, 0));
    HuSprGrpDrawNoSet(RankBarGrpId, 0x40);
    HuSprPosSet(RankBarGrpId, 0, 288.0f, 90.0f);
    Hu3DLayerHookSet(0, RankBackDraw);
    HuPrcVSleep();
    ToueiDispInit();

    pos2D.x = 88.0f;
    pos2D.y = 122.0f;
    Hu3D2Dto3D(&pos2D, HU3D_CAM2, &pos3D);
    Hu3DModelPosSetV(CapsuleMdlId, &pos3D);
    Hu3DModelLayerSet(CapsuleMdlId, 1);

    RankHelpWin = HuWinCreate(-10000.0f, 404.0f, 0x230, 0x29, 0);
    HuWinBGTPLvlSet(RankHelpWin, 0.0f);
    HuWinAttrSet(RankHelpWin, 0x800);
    HuWinMesSpeedSet(RankHelpWin, 0);
    HuWinDispOff(RankHelpWin);
    HuWinMesSet(RankHelpWin, 0x2000A);

    RankBoardNameGrpId = HuSprGrpCreate(5);
    for (i = 0; i < 5; i++) {
        charNo = GwCommon.storyBoardNo[i];
        if (SRankWork.storyNo > i) {
            boardModelId = Hu3DModelCreateData(0xC20011 + charNo);
        } else {
            boardModelId = Hu3DModelCreateData(0xC2000A);
        }
        RankBoardModelId[i] = boardModelId;
        Hu3D2Dto3D((HuVecF *)&RankPosTbl[i], HU3D_CAM2, &pos3D);
        Hu3DModelPosSetV(boardModelId, &pos3D);
        Hu3DModelLayerSet(boardModelId, 0);
        Hu3DModelCameraSet(boardModelId, HU3D_CAM2);
        Hu3DModelScaleSet(boardModelId, 0.9f, 0.9f, 0.9f);

        if (SRankWork.storyNo < i) {
            RankModelId[i] = -1;
        } else {
            modelId = Hu3DModelCreateData(0xC2000E - SRankWork.mapRank[i]);
            RankModelId[i] = modelId;
            Hu3D2Dto3D((HuVecF *)&RankPosTbl[i], HU3D_CAM2, &pos3D);
            Hu3DModelPosSetV(modelId, &pos3D);
            Hu3DModelLayerSet(modelId, 1);
            Hu3DModelAttrSet(modelId, 0x40000001U);
            Hu3DModelCameraSet(modelId, HU3D_CAM2);
            Hu3DModelScaleSet(modelId, 1.5f, 1.5f, 1.5f);
            if (lastNo <= i) {
                Hu3DModelAttrSet(modelId, HU3D_ATTR_DISPOFF);
            }
        }

        HuSprGrpMemberSet(RankBoardNameGrpId, i, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(0xC20002 + charNo, HU_MEMNUM_OVL, HEAP_MODEL)), 100, 0));
        HuSprPosSet(RankBoardNameGrpId, i, RankPosTbl[i].x, 50.0f + RankPosTbl[i].y);
        if (lastNo <= i) {
            HuSprScaleSet(RankBoardNameGrpId, i, 0.0f, 0.0f);
        }
    }
    HuSprGrpDrawNoSet(RankBoardNameGrpId, 0x40);

    boardModelId = RankBoardModelId[lastNo];
    Hu3D2Dto3D((HuVecF *)&RankPosTbl[5], HU3D_CAM2, &pos3D);
    Hu3DModelPosSetV(boardModelId, &pos3D);
    Hu3DModelLayerSet(boardModelId, 1);
    modelId = RankModelId[lastNo];
    Hu3DModelPosSetV(modelId, &pos3D);
    Hu3DModelLayerSet(modelId, 2);
    HuSprDrawNoSet(RankBoardNameGrpId, lastNo, 0);

    for (i = 1; i < 5; i++) {
        HuVecF a, b, mid;
        s16 ang;
        if (SRankWork.storyNo > i) {
            modelId = Hu3DModelCreateData(0xC20009);
            RankEffModelId[i - 1] = modelId;
            a.x = RankPosTbl[i - 1].x;
            a.y = RankPosTbl[i - 1].y;
            a.z = RankPosTbl[i - 1].z;
            VECSubtract((Vec *)&RankPosTbl[i], (Vec *)&a, (Vec *)&a);
            ang = (s16)(180.0 * (atan2(a.x, a.y) / 3.141592653589793));
            a.x = RankPosTbl[i - 1].x;
            a.y = RankPosTbl[i - 1].y;
            a.z = RankPosTbl[i - 1].z;
            VECAdd((Vec *)&a, (Vec *)&RankPosTbl[i], (Vec *)&a);
            VECScale((Vec *)&a, (Vec *)&a, 0.5f);
            a.z = 8510.0f;
            Hu3D2Dto3D(&a, HU3D_CAM2, &pos3D);
            Hu3DModelPosSetV(modelId, &pos3D);
            Hu3DModelLayerSet(modelId, 0);
            Hu3DModelCameraSet(modelId, HU3D_CAM2);
            Hu3DModelScaleSet(modelId, 0.8f, 1.1f, 1.0f);
            Hu3DModelRotSet(modelId, 0.0f, 0.0f, (f32)ang);
            if (lastNo == i) {
                Hu3DModelAttrSet(modelId, HU3D_ATTR_DISPOFF);
            }
        }
    }

    RankParmanId = Hu3DParManCreate(HuSprAnimRead(HuDataSelHeapReadNum(0x210005, HU_MEMNUM_OVL, HEAP_MODEL)), 200, &RankParmanParam);
    Hu3DParManAttrSet(RankParmanId, 0x125);
    Hu3DParticleBlendModeSet(Hu3DParManModelIDGet(RankParmanId), 1);
    Hu3DParManRotSet(RankParmanId, 0.0f, 0.0f, 0.0f);
    Hu3DModelLayerSet(Hu3DParManModelIDGet(RankParmanId), 3);
    Hu3DModelCameraSet(Hu3DParManModelIDGet(RankParmanId), HU3D_CAM2);
    Hu3D2Dto3D((HuVecF *)&RankPosTbl[5], HU3D_CAM2, &pos3D);
    Hu3DModelPosSetV(Hu3DParManModelIDGet(RankParmanId), &pos3D);
    Hu3DParManPosSet(RankParmanId, pos3D.x, pos3D.y, pos3D.z);
    HuSprExecLayerCameraSet(0x40, HU3D_CAM2, 1);
}

// NONMATCHING: register allocation differs across this large sequence function
void SRankExec(void) {
    Vec a, b, c;
    s16 lastNo = SRankWork.storyNo - 1;
    s16 rankMdlId = RankModelId[lastNo];
    s16 i;
    f32 scale;

    WipeCreate(1, 0, 60);
    while (WipeCheckEnd()) {
        HuPrcVSleep();
    }

    Hu3DModelAttrReset(rankMdlId, HU3D_ATTR_DISPOFF);
    Hu3DModelCameraSet(rankMdlId, HU3D_CAM1);
    HuAudFXPlay(0x57F);

    for (i = 1; i <= 30; i++) {
        scale = (f32)(1.5 + (7.0 * cos((3.141592653589793 * (90.0 * ((f64)i / 30.0))) / 180.0)));
        Hu3DModelScaleSet(rankMdlId, scale, scale, 1.5f);
        CapsuleGlowTPLvl = (f32)sin((3.141592653589793 * (90.0 * ((f64)i / 30.0))) / 180.0);
        if (i == 29) {
            Hu3DParManTimeLimitSet(RankParmanId, 5);
            Hu3DParManAttrReset(RankParmanId, 1);
        }
        HuPrcVSleep();
    }

    if (PResDiff == 3) {
        HuAudFXPlay(0x57D);
    } else {
        HuAudFXPlay(0x57E);
    }
    Hu3DModelCameraSet(rankMdlId, HU3D_CAM2);

    for (i = 1; i <= 60; i++) {
        CapsuleGlowTPLvl = (f32)cos((3.141592653589793 * (90.0 * ((f64)i / 60.0))) / 180.0);
        HuPrcVSleep();
    }
    HuPrcSleep(60);

    for (i = 1; i <= 30; i++) {
        f32 t = (f32)sin((3.141592653589793 * (90.0 * ((f64)i / 30.0))) / 180.0);
        s16 boardModelId = RankBoardModelId[lastNo];
        Hu3D2Dto3D((HuVecF *)&RankPosTbl[5], HU3D_CAM2, (HuVecF *)&a);
        Hu3D2Dto3D((HuVecF *)&RankPosTbl[lastNo], HU3D_CAM2, (HuVecF *)&b);
        VECScale(&a, &a, 1.0f - t);
        VECScale(&b, &b, t);
        VECAdd(&a, &b, &c);
        Hu3DModelPosSetV(boardModelId, (HuVecF *)&c);
        Hu3DModelPosSetV(RankModelId[lastNo], (HuVecF *)&c);
        HuPrcVSleep();
    }

    for (i = 1; i <= 30; i++) {
        f32 t = (f32)sin((3.141592653589793 * (90.0 * ((f64)i / 30.0))) / 180.0);
        HuSprScaleSet(RankBoardNameGrpId, lastNo, t, t);
        HuPrcVSleep();
    }
    HuPrcSleep(10);

    if (SRankWork.storyNo > 1) {
        s16 effModelId = RankEffModelId[lastNo - 1];
        Hu3DModelAttrReset(effModelId, HU3D_ATTR_DISPOFF);
        for (i = 1; i < 30; i++) {
            f32 t = (f32)(0.5 * ((f64)i / 30.0));
            a.x = RankPosTbl[lastNo - 1].x;
            a.y = RankPosTbl[lastNo - 1].y;
            a.z = RankPosTbl[lastNo - 1].z;
            b.x = RankPosTbl[lastNo].x;
            b.y = RankPosTbl[lastNo].y;
            b.z = RankPosTbl[lastNo].z;
            VECScale(&a, &a, 1.0f - t);
            VECScale(&b, &b, t);
            VECAdd(&a, &b, &a);
            a.z = 8510.0f;
            Hu3D2Dto3D((HuVecF *)&a, HU3D_CAM2, (HuVecF *)&c);
            Hu3DModelPosSetV(effModelId, (HuVecF *)&c);
            HuPrcVSleep();
        }
    }

    HuWinDispOn(RankHelpWin);
    HuPrcChildCreate(RankObjRotExec, 1000, 12288, 0, objman);

    while (!(HuPadBtnDown[0] & 0x100)) {
        HuPrcVSleep();
    }
    WipeCreate(2, 0, 60);
    HuAudSStreamAllFadeOut(1000);
    while (WipeCheckEnd()) {
        HuPrcVSleep();
    }
}

// NONMATCHING: minor register allocation difference in the sleep-duration expression
static void RankObjRotExec(void) {
    f32 spd[5];
    s16 i;
    s16 t;
    f32 angle;

    for (;;) {
        HuPrcSleep((s32)(s16)(frandmod(60) + 120));
        for (i = 0; i < 5; i++) {
            spd[i] = (f32)(frandmod(10) + 10);
        }
        for (t = 1; t <= 20; t++) {
            for (i = 0; i < 5; i++) {
                angle = (f32)(spd[i] + (360.0 * (1.0 - cos((3.141592653589793 * (90.0 * ((f64)t / 20.0))) / 180.0))));
                if (RankModelId[i] != -1) {
                    Hu3DModelRotSet(RankModelId[i], 0.0f, angle, 0.0f);
                }
            }
            HuPrcVSleep();
        }
        for (t = 1; t <= 5; t++) {
            for (i = 0; i < 5; i++) {
                angle = (f32)(spd[i] * (1.0 - sin((3.141592653589793 * (90.0 * ((f64)t / 5.0))) / 180.0)));
                if (RankModelId[i] != -1) {
                    Hu3DModelRotSet(RankModelId[i], 0.0f, angle, 0.0f);
                }
            }
            HuPrcVSleep();
        }
    }
}

static void RankBackDraw(s16 layerNo) {
    Mtx44 proj;
    Mtx modelview;
    GXColor color = RankBackColor;

    if (Hu3DCameraNo == 2) {
        DispZClear();
        MTXOrtho(proj, 0.0f, 480.0f, 0.0f, 640.0f, 0.0f, 10.0f);
        GXSetProjection(proj, GX_ORTHOGRAPHIC);
        MTXIdentity(modelview);
        GXLoadPosMtxImm(modelview, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXSetViewport(0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
        GXSetScissor(0, 0, 640, 480);
        GXSetCullMode(GX_CULL_NONE);
        GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
        GXSetAlphaUpdate(GX_FALSE);
        GXSetColorUpdate(GX_TRUE);
        GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_SPEC);
        GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_SPEC);
        GXSetCullMode(GX_CULL_NONE);
        HuSprTexLoad(BackAnim, 0, 0, 1, 1, 1);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_TEX_S, GX_RGBX8, 0);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_RGBA6, 0);
        GXSetNumTexGens(1);
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0);
        GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
        GXSetTevColor(GX_TEVREG0, color);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition2s16(0, 0);
        GXTexCoord2f32((f32)BackScrollX, (f32)BackScrollY);
        GXPosition2s16(640, 0);
        GXTexCoord2f32((f32)(5.0 + BackScrollX), (f32)BackScrollY);
        GXPosition2s16(640, 480);
        GXTexCoord2f32((f32)(5.0 + BackScrollX), (f32)(5.0 + BackScrollY));
        GXPosition2s16(0, 480);
        GXTexCoord2f32((f32)BackScrollX, (f32)(5.0 + BackScrollY));

        BackScrollX += 0.005;
        if (BackScrollX >= 1.0) {
            BackScrollX = 0.0;
        }
        BackScrollY += 0.005;
        if (BackScrollY >= 1.0) {
            BackScrollY = 0.0;
        }
    }
}
