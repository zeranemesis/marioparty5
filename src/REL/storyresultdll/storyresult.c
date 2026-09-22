#include "REL/storyresultdll.h"

#include "math.h"
#include "humath.h"

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

s32 _prolog(void) {
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

static OMCAMERAVIEW CameraView1 = { { 0.0f, 250.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, 7000.0f };
static OMCAMERAVIEW CameraView2 = { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, 8400.0f };

s32 debugCamOnF;
s32 PResGLight;
SRANK_WORK SRankWork;
s16 CapsuleMdlId;
s16 CharMotId;
s16 CharMdlId;
s16 PResDiff;
s8 LanguageNo;
OMOBJMAN *objman;
static OMOBJ *outViewObj;

// NONMATCHING: register allocation differs in a couple spots (LanguageNo load, epilogue restore order)
void ObjectSetup(void) {
    u8 languageNo;
    OSReport("******* PRES ObjectSetup *********\n");
    objman = omInitObjMan(50, 8192);
    omSysPauseEnable(FALSE);
    languageNo = GwCommon.languageNo;
    LanguageNo = languageNo;
    if (GwCommon.storyNo != -1 && GWPlayerCoinGet(0) <= 0) {
        omOvlGotoEx(DLL_storymodedll, TRUE, 0, 0);
        return;
    }
    Hu3DCameraCreate(HU3D_CAM0 | HU3D_CAM1 | HU3D_CAM2);
    Hu3DCameraPerspectiveSet(HU3D_CAM0, 5.0f, 20.0f, 15000.0f, 1.2f);
    Hu3DCameraViewportSet(HU3D_CAM0, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraPerspectiveSet(HU3D_CAM1 | HU3D_CAM2, 5.0f, 100.0f, 30000.0f, 1.2f);
    Hu3DCameraViewportSet(HU3D_CAM1 | HU3D_CAM2, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    omCameraViewSetMulti(HU3D_CAM0, &CameraView1);
    omCameraViewSetMulti(HU3D_CAM1 | HU3D_CAM2, &CameraView2);
    PResGLight = Hu3DGLightCreate(0.0f, 100.0f, 1000.0f, 0.0f, -0.5f, -1.0f, 255, 255, 255);
    Hu3DGLightInfinitytSet(PResGLight);
    outViewObj = omAddObjEx(objman, 32730, 0, 0, OM_GRP_NONE, omOutViewMulti);
    outViewObj->work[0] = 3;
    omAddObjEx(objman, 0, 32, 32, OM_GRP_NONE, DebugCamUpdate);
    HuAudSStreamPlay(0x1B);
    Hu3DBGColorSet(0, 0, 0);
    HuWinInit(1);
    HuPrcChildCreate(PResExec, 1000, 12288, 0, objman);
}

void PResExec(void) {
    GXSetPixelFmt(GX_PF_RGBA6_Z24, GX_ZC_LINEAR);
    PResCreate();
    SRankCreate();
    SRankExec();
    if (RenderMode->aa) {
        GXSetPixelFmt(GX_PF_RGB565_Z16, GX_ZC_LINEAR);
    } else {
        GXSetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);
    }
    if (!_CheckFlag(FLAG_MG_PRACTICE)) {
        GwPlayer->mgCoinBonus = 0;
    }
    _ClearFlag(FLAG_BOARD_SAVEINIT);
    SLSaveStoryExec();
    if (GwCommon.storyNo == 0) {
        omOvlReturnEx(1, TRUE);
    } else {
        omOvlGotoEx(DLL_storymodedll, TRUE, 0, 0);
    }
    while (TRUE) {
        HuPrcVSleep();
    }
}

// NONMATCHING: stack frame/register allocation differs (extra locals vs original temp reuse)
void PResCreate(void) {
    s16 charNo = GwPlayerConf->charNo;
    s16 i;
    s16 boardNo;
    s16 sumRank;
    s16 diff;
    HU3DMODELID mdlId;

    if (GwCommon.storyNo == -1) {
        SRankWork.dif = 0;
        SRankWork.rank = 4;
        SRankWork.storyNo = 1;
        for (i = 0; i < 5; i++) {
            SRankWork.mapRank[i] = (i > 3) ? 3 : i;
            GwCommon.storyBoardNo[i] = (s8)i;
        }
    } else {
        OSReport("%d\n", GwSystem.turnNo);
        if (GwPlayerConf->dif == 0) {
            if (GwSystem.turnNo <= 2) {
                diff = 3;
            } else if (GwSystem.turnNo <= 5) {
                diff = 2;
            } else if (GwSystem.turnNo <= 10) {
                diff = 1;
            } else {
                diff = 0;
            }
        } else if (GwPlayerConf->dif == 1) {
            if (GwSystem.turnNo <= 3) {
                diff = 3;
            } else if (GwSystem.turnNo <= 6) {
                diff = 2;
            } else if (GwSystem.turnNo <= 12) {
                diff = 1;
            } else {
                diff = 0;
            }
        } else if (GwPlayerConf->dif == 2) {
            if (GwSystem.turnNo <= 4) {
                diff = 3;
            } else if (GwSystem.turnNo <= 8) {
                diff = 2;
            } else if (GwSystem.turnNo <= 13) {
                diff = 1;
            } else {
                diff = 0;
            }
        } else if (GwPlayerConf->dif == 3) {
            if (GwSystem.turnNo <= 5) {
                diff = 3;
            } else if (GwSystem.turnNo <= 9) {
                diff = 2;
            } else if (GwSystem.turnNo <= 14) {
                diff = 1;
            } else {
                diff = 0;
            }
        }
        PResDiff = diff;

        boardNo = GwCommon.storyBoardNo[GwCommon.storyNo];
        if (GwCommon.charStoryRank[boardNo][charNo] < PResDiff) {
            GwCommon.charStoryRank[boardNo][charNo] = (s8)PResDiff;
        }
        GwCommon.storyRank[GwCommon.storyNo] = (s8)PResDiff;
        GwCommon.charStoryPlayNum[boardNo][charNo]++;
        if (GwCommon.charStoryPlayNum[boardNo][charNo] > 999) {
            GwCommon.charStoryPlayNum[boardNo][charNo] = 999;
        }
        GwCommon.storyNo++;

        SRankWork.dif = GwPlayerConf->dif;
        SRankWork.storyNo = GwCommon.storyNo;
        for (i = 0; i < 5; i++) {
            SRankWork.mapRank[i] = 0;
        }
        sumRank = 0;
        for (i = 0; i < GwCommon.storyNo; i++) {
            SRankWork.mapRank[i] = GwCommon.storyRank[i];
            sumRank += GwCommon.storyRank[i];
            OSReport("Rank %d\n", sumRank, GwCommon.storyRank[i]);
        }
        if (sumRank <= 4) {
            SRankWork.rank = 0;
        } else if (sumRank <= 9) {
            SRankWork.rank = 1;
        } else if (sumRank <= 12) {
            SRankWork.rank = 2;
        } else if (sumRank <= 14) {
            SRankWork.rank = 3;
        } else {
            SRankWork.rank = 4;
        }
        for (i = 0; i < 7; i++) {
            OSReport("Rank Map%d:%d\n", i, GwCommon.charStoryRank[i][charNo]);
        }
        OSReport("Total Rank %d\n", SRankWork.rank);
    }

    CharMdlId = CharModelCreate(charNo, 2);
    CharMotId = CharMotionCreate(charNo, 0x8C0000);
    CharMotionSet(charNo, CharMotId);
    Hu3DModelAttrSet(CharMdlId, 0x40000001U);
    Hu3DModelCameraSet(CharMdlId, HU3D_CAM0);
    mdlId = Hu3DModelCreateData(0xC2000F);
    CapsuleMdlId = mdlId;
    Hu3DModelCameraSet(mdlId, HU3D_CAM2);
    Hu3DModelScaleSet(mdlId, 0.6f, 0.6f, 0.2f);
    mdlId = Hu3DModelCreateData(0xC20010);
    Hu3DModelCameraSet(mdlId, HU3D_CAM0);
    Hu3DModelAttrSet(mdlId, 0x40000001U);
    Hu3DReflectMapSet(HuDataSelHeapReadNum(0xC20018, HU_MEMNUM_OVL, HEAP_MODEL));
}

void DebugCamUpdate(OMOBJ *obj) {
    Vec pos;
    Vec offset;
    Vec dir;
    Vec yOfs;
    f32 rotZ;
    s8 stickPos;

    if (debugCamOnF) {
        CRotM[2].y += 0.1f * HuPadStkX[0];
        CRotM[2].x += 0.1f * HuPadStkY[0];
        CZoomM[2] += HuPadTrigL[0] / 2;
        CZoomM[2] -= HuPadTrigR[0] / 2;
        if (CZoomM[2] < 100.0f) {
            CZoomM[2] = 100.0f;
        }
        pos.x = CenterM[2].x + (CZoomM[2] * (HuSin(CRotM[2].y) * HuCos(CRotM[2].x)));
        pos.y = (CenterM[2].y + (CZoomM[2] * -HuSin(CRotM[2].x)));
        pos.z = (CenterM[2].z + (CZoomM[2] * (HuCos(CRotM[2].y) * HuCos(CRotM[2].x))));
        offset.x = CenterM[2].x - pos.x;
        offset.y = CenterM[2].y - pos.y;
        offset.z = CenterM[2].z - pos.z;
        dir.x = (HuSin(CRotM[2].y) * HuSin(CRotM[2].x));
        dir.y = HuCos(CRotM[2].x);
        dir.z = (HuCos(CRotM[2].y) * HuSin(CRotM[2].x));
        rotZ = CRotM[2].z;
        yOfs.x = dir.x * (offset.x * offset.x + (1.0f - offset.x * offset.x) * HuCos(rotZ))
            + dir.y * (offset.x * offset.y * (1.0f - HuCos(rotZ)) - offset.z * HuSin(rotZ))
            + dir.z * (offset.x * offset.z * (1.0f - HuCos(rotZ)) + offset.y * HuSin(rotZ));

        yOfs.y = dir.y * (offset.y * offset.y + (1.0f - offset.y * offset.y) * HuCos(rotZ))
            + dir.x * (offset.x * offset.y * (1.0f - HuCos(rotZ)) + offset.z * HuSin(rotZ))
            + dir.z * (offset.y * offset.z * (1.0f - HuCos(rotZ)) - offset.x * HuSin(rotZ));

        yOfs.z = dir.z * (offset.z * offset.z + (1.0f - offset.z * offset.z) * HuCos(rotZ))
            + (dir.x * (offset.x * offset.z * (1.0 - HuCos(rotZ)) - offset.y * HuSin(rotZ))
                + dir.y * (offset.y * offset.z * (1.0 - HuCos(rotZ)) + offset.x * HuSin(rotZ)));

        VECCrossProduct(&dir, &offset, &offset);
        VECNormalize(&offset, &offset);
        stickPos = (HuPadSubStkX[0] & 0xF8);
        if (stickPos != 0) {
            CenterM[2].x += 0.05f * (offset.x * stickPos);
            CenterM[2].y += 0.05f * (offset.y * stickPos);
            CenterM[2].z += 0.05f * (offset.z * stickPos);
        }
        VECNormalize(&yOfs, &offset);
        stickPos = -(HuPadSubStkY[0] & 0xF8);
        if (stickPos != 0) {
            CenterM[2].x += 0.05f * (offset.x * stickPos);
            CenterM[2].y += 0.05f * (offset.y * stickPos);
            CenterM[2].z += 0.05f * (offset.z * stickPos);
        }
    }
}
