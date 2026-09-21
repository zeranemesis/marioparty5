#include "REL/partyresultdll.h"
#include "game/saveload.h"
#include "game/flag.h"

#include "math.h"
#include "humath.h"

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

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

static OMOBJ *outview;
OMOBJMAN *objman;
u8 PResLanguageNo;
HuVecF lbl_1_bss_45C[GW_PLAYER_MAX];
HU3DMODELID PResPlayerModelId[8];
HU3DMOTID PResPlayerMotion[GW_PLAYER_MAX][PRES_PLAYER_MOT_MAX];
PRES_PLAYER PResPlayer[GW_PLAYER_MAX];
s16 PResPlayerOrder[GW_PLAYER_MAX];
s16 PResCharNo[GW_PLAYER_MAX];
BOOL PResTeamF;
s32 PResLightId;
s32 lbl_1_bss_8;
BOOL CameraDebugF;
s32 lbl_1_bss_0;

GXColor ToueiFogColor = { 20, 0, 0, 0 };

static OMCAMERAVIEW StageCameraView = {
    { 0, 250, 0 },
    { 0, 0, 0 },
    7000
};

static OMCAMERAVIEW MainCameraView = {
    { 0, 0, 0 },
    { 0, 0, 0 },
    1600
};

static OMCAMERAVIEW GuideCameraView = {
    { 0, 150, 0 },
    { 0, 0, 0 },
    140
};

static unsigned int BoardDataDir[] = {
    DATA_w01,
    DATA_w02,
    DATA_w03,
    DATA_w04,
    DATA_w07,
    0
};

static void PResPlayerCreate(void);

void ObjectSetup(void)
{
    s16 i;
    OSReport("******* PRES ObjectSetup *********\n");
    objman = omInitObjMan(50, 8192);
    for(i=0; BoardDataDir[i] != 0; i++) {
        HuDataDirClose(BoardDataDir[i]);
    }
    omSysPauseEnable(FALSE);
    PResLanguageNo = GWLanguageGet();
    Hu3DCameraCreate(HU3D_CAM0|HU3D_CAM1|HU3D_CAM2);
    Hu3DCameraPerspectiveSet(HU3D_CAM0, 5, 20, 15000, 1.2f);
    Hu3DCameraViewportSet(HU3D_CAM0, 0, 0, 640, 480, 0, 1);
    Hu3DCameraPerspectiveSet(HU3D_CAM1|HU3D_CAM2, 30, 20, 15000, 1.2f);
    Hu3DCameraViewportSet(HU3D_CAM1|HU3D_CAM2, 0, 0, 640, 480, 0, 1);
    omCameraViewSetMulti(HU3D_CAM0, &StageCameraView);
    omCameraViewSetMulti(HU3D_CAM1, &MainCameraView);
    omCameraViewSetMulti(HU3D_CAM2, &MainCameraView);
    PResLightId = Hu3DGLightCreate(0, 1000, 1000, 0, -1, -1, 255, 255, 255);
    Hu3DGLightInfinitytSet(PResLightId);
    outview = omAddObj(objman, 32730, 0, 0, omOutViewMulti);
    outview->work[0] = 3;
    omAddObj(objman, 0, 32, 32, CameraMove);
    Hu3DBGColorSet(0, 0, 0);
    HuWinInit(1);
    HuPrcChildCreate(PResMain, 1000, 12288, 0, objman);
}

void PResMain(void)
{
    PResPlayerCreate();
    PResResultCreate();
    PResResultMain();
    PResStatCreate();
    PResStatMain();
    _ClearFlag(FLAG_BOARD_SAVEINIT);
    SLSaveBoardEndExec();
    omOvlReturn(1);
    while(1) {
        HuPrcVSleep();
    }
}

void PResExit(void)
{
    while(!omSysExitReq) {
        HuPrcVSleep();
    }
    WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 60);
    HuAudSStreamAllFadeOut(1000);
    WipeWait();
    omOvlReturn(1);
    while(1) {
        HuPrcVSleep();
    }
}

static void PResPlayerCreate(void)
{
    s16 i;
    s16 j;
    GWPLAYER *gwPlayer;
    PRES_PLAYER *player;
    s16 charNo;
    HU3DMODELID mdlId;

    PResTeamF = GwSystem.teamF;
    if(PResTeamF) {
        for(i=j=0; i<GW_PLAYER_MAX; i++) {
            if(GwPlayerConf[i].grp == 0) {
                PResPlayerOrder[j++] = i;
            }
        }
        for(i=0; i<GW_PLAYER_MAX; i++) {
            if(GwPlayerConf[i].grp != 0) {
                PResPlayerOrder[j++] = i;
            }
        }
    } else {
        for(i=0; i<GW_PLAYER_MAX; i++) {
            PResPlayerOrder[i] = i;
        }
    }
    for(i=0; i<GW_PLAYER_MAX; i++) {
        gwPlayer = &GwPlayer[PResPlayerOrder[i]];
        player = &PResPlayer[i];
        player->charNo = gwPlayer->charNo;
        player->playerNo = PResPlayerOrder[i];
        player->star = gwPlayer->star;
        player->coin = gwPlayer->coin;
        player->turnMax = GwSystem.turnMax;
        player->coinTotalMg = gwPlayer->coinTotalMg;
        player->coinMax = gwPlayer->coinMax;
        player->hatenaMasuNum = gwPlayer->hatenaMasuNum;
        player->starStat[0] = gwPlayer->handicap;
        player->coinStat[0] = 10;
        for(j=1; j<player->turnMax; j++) {
            player->starStat[j] = gwPlayer->starStat[j];
            player->coinStat[j] = gwPlayer->coinStat[j];
        }
        player->starStat[j] = gwPlayer->star;
        player->coinStat[j] = gwPlayer->coin;
        player->statStar = gwPlayer->star;
        player->statCoin = gwPlayer->coin;
        player->statCoinTotalMg = gwPlayer->coinTotalMg;
        player->statCoinMax = gwPlayer->coinMax;
        player->statBlueMasuNum = gwPlayer->blueMasuNum;
        player->statRedMasuNum = gwPlayer->redMasuNum;
        player->statCapsuleMasuNum = gwPlayer->capsuleMasuNum;
        player->statHatenaMasuNum = gwPlayer->hatenaMasuNum;
        player->statKupaMasuNum = gwPlayer->kupaMasuNum;
        player->statDonkeyMasuNum = gwPlayer->donkeyMasuNum;
    }
    for(i=0; i<GW_PLAYER_MAX; i++) {
        PResPlayer[i].bonusStar = 0;
    }
    for(i=0; i<GW_PLAYER_MAX; i++) {
        charNo = PResPlayer[i].charNo;
        PResCharNo[i] = charNo;
        mdlId = CharModelCreate(charNo, CHAR_MODEL1);
        PResPlayerModelId[i] = mdlId;
        PResPlayerMotion[i][0] = CharMotionCreate(charNo, CHARMOT_HSF_c000m1_480);
        PResPlayerMotion[i][1] = CharMotionCreate(charNo, CHARMOT_HSF_c000m1_303);
        PResPlayerMotion[i][2] = CharMotionCreate(charNo, CHAR_HSF_c000m1_526);
        PResPlayerMotion[i][3] = CharMotionCreate(charNo, CHARMOT_HSF_c000m1_487);
        PResPlayerMotion[i][4] = CharMotionCreate(charNo, CHARMOT_HSF_c000m1_306);
        PResPlayerMotion[i][5] = CharMotionCreate(charNo, CHARMOT_HSF_c000m1_483);
        PResPlayerMotion[i][6] = CharMotionCreate(charNo, CHARMOT_HSF_c000m1_354);
        Hu3DMotionAttrSet(PResPlayerMotion[i][2], HU3D_ATTR_MOT_RESET_LOCK);
        CharMotionSet(charNo, PResPlayerMotion[i][0]);
        Hu3DModelAttrSet(mdlId, HU3D_MOTATTR_LOOP);
    }
}

void CameraMove(OMOBJ *obj)
{
    HuVecF pos;
    HuVecF offset;
    HuVecF dir;
    HuVecF yOfs;

    float rotZ;
    s8 stickPos;
    if(CameraDebugF) {
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
