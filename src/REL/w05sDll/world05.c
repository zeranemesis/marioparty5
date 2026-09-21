#include "game/board/main.h"
#include "game/board/model.h"
#include "game/board/scroll.h"
#include "game/board/masu.h"
#include "game/board/branch.h"
#include "game/board/player.h"
#include "game/board/window.h"
#include "game/board/status.h"
#include "game/board/camera.h"
#include "game/board/star.h"
#include "game/board/capsule.h"

#include "game/wipe.h"

#include "datanum/effect.h"
#include "datanum/board.h"

#include "datanum/w05s.h"
#include "messnum/board_w05.h"

#include <math.h>

#define MB5EV_MASU_ROCKET MASU_FLAG_BIT_MULTI(0, 2)
#define MB5EV_MASU_WARP MASU_FLAG_BIT(2)
#define MB5EV_MASU_WARP_JUMP MASU_FLAG_BIT(3)

typedef struct MB5_Work_s {
    int bgMdlId; //0x00
    int symBaseMdlId; //0x04
    int bgPillarMdlId; //0x08
    int symMdlId; //0x0C
    int rocketPathMdlId[3]; //0x10
    int launchBMdlId[3]; //0x1C
    int rocketMdlId[3]; //0x28
    int warpMdlId[2]; //0x34
    int arrowMdlId[9]; //0x3C
    int warpEffMdlId[2]; //0x60
    int warpLayerMdlId[2]; //0x68
    int radarBodyMdlId[2]; //0x70
    int treeMdlId[2]; //0x78
    int rocketLightMdlId[2]; //0x80
} MB5_WORK; // size 0x88

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

extern void ObjectSetup(void);

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

static int numberFileTbl[] = {
    BOARD_HSF_num0,
    BOARD_HSF_num1,
    BOARD_HSF_num2,
    BOARD_HSF_num3,
    BOARD_HSF_num4,
    BOARD_HSF_num5,
    BOARD_HSF_num6,
    BOARD_HSF_num7,
    BOARD_HSF_num8,
    BOARD_HSF_num9
};

static HuVecF mb5ev_RocketPosTbl[3] = {
    { -1086, 750, -811 },
    { 0, 750, -1185 },
    { 1086, 750, -811 },
};

static HuVecF mb5ev_ArrowPosTbl[9] = {
    { 1000, 325, 2200 },
    { -1070, 490, 1100 },
    { 900, 490, 1265 },
    { -1500, 490, -55 },
    { -765, 540, -1025 },
    { 765, 540, -1025 },
    { 1500, 490, -55 },
    { -580, 490, 835 },
    { 900, 490, 340 },
};

static HuVecF mb5ev_ArrowRotTbl[9] = {
    { 0, 0, 0 },
    { 0, -45, 0 },
    { 0, 45, 0 },
    { 0, -115, 0 },
    { 0, -155, 0 },
    { 0, 155, 0 },
    { 0, 115, 0 },
    { 0, -135, 0 },
    { 0, 180, 0 },
};

static HuVecF mb5ev_WarpTogePosTbl[2] = {
    { -1860, 505, 550 },
    { 1860, 505, 550 }
};

static HuVecF mb5ev_RaderBdyPosTbl[2] = {
    { -1800, 320, 1390 },
    { 1800, 320, 1390 }
};

static HuVecF mb5ev_RaderBdyRotTbl[2] = {
    { 0, 45, 0 },
    { 0, -45, 0 }
};

static HuVecF mb5ev_Tree2PosTbl[2] = {
    { -1180, 320, 2000 },
    { 1180, 320, 2000 }
};

static HuVecF mb5ev_Tree2RotTbl[2] = {
    { 0, -45, 0 },
    { 0, 45, 0 }
};

static HuVecF mb5ev_RokPLPosTbl[2] = {
    { -1400, 615, -515 },
    { 1400, 615, -515 }
};

static HuVecF mb5ev_RokPLRotTbl[2] = {
    { 0, 45, 0 },
    { 0, -45, 0 }
};

static HuVecF mb5ev_WarpPosTbl[2] = {
    { -1860, 315, 550 },
    { 1860, 315, 550 }
};

static HuVecF mb5ev_LaunchBPosTbl[3] = {
    { -800, 700, -525 },
    { 0, 700, -780 },
    { 800, 700, -525 }
};

static HuVecF mb5ev_LaunchBRotTbl[3] = {
    { 0, 45, 0 },
    { 0, 0, 0 },
    { 0, -45, 0 },
};

static HuVecF rocketEndPos_200[3] = {
    { -1086, 750, -811 },
    { 0, 750, -1185 },
    { 1086, 750, -811 },
};

static char *rocketHookTbl_210[3] = {
    "rockRhook",
    "rockGhook",
    "rockBhook"
};

static HuVecF rocketPos_220[3] = {
    { -1086, 1180, -811 },
    { 0, 1180, -1185 },
    { 1086, 1180, -811 },
};

static s16 playerRotTbl_230[3] = { 45, 0, -45 };

static HU3DPARMANPARAM mb5ev_WarpEffParam = {
    70, 0,
    1.5f, 150.0f, 90.0f,
    { 0.0f, 0.15f, 0.0f },
    1.0f, 1.0f, 16.0f, 1.0f,
    1,
    { { 0xFF, 0xFF, 0xFF, 0xFF }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
    { { 0xFF, 0xFF, 0xFF, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
};

static HU3DPARMANPARAM mb5ev_RocketDustEffParam = {
    30, 0,
    1.0f, 180.0f, 0.0f,
    { 0.0f, -0.2f, 0.0f },
    10.0f, 0.8f, 200.0f, 1.0f,
    4,
    { { 0xFF, 0xFF, 0xFF, 0x50 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
    { { 0xFF, 0xFF, 0xFF, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
};

static MB5_WORK mb5_Work;
static s16 unkMdlId;
static s16 numberMdlId[5];
static HuVecF mb5ev_WarpEndSize;
static ANIMDATA *mb5ev_RocketDustAnim;
static ANIMDATA *mb5ev_WarpParmanAnim;
static HU3DPARMANID mb5ev_WarpParmanId;
static HU3DPARMANID mb5ev_RocketDustParmanId[13];
static OMOBJ *mb5_MainObj;

void MB5_Create(void);
void MB5_Kill(void);
void MB5_LightSet(void);
void MB5_LightReset(void);
int MB5Ev_MasuWalkPre(int playerNo);
int MB5Ev_MasuWalkPost(int playerNo);
int MB5Ev_MasuHatena(int playerNo);
int MB5_MasuPathCheck(s16 id, u32 flag, s16 *linkTbl, BOOL endF);
static void MB5_MainObjUpdate(OMOBJ *obj);
void MB5Ev_Warp(int playerNo);
void MB5Ev_Rocket(int playerNo);

void ObjectSetup(void)
{
    GWPartyFSet(FALSE);
    MBObjectSetup(MBNO_WORLD_5, MB5_Create, MB5_Kill);
}

void MB5_Create(void)
{
    HuVecF openingRot = { -33, 0, 0 };
    HuVecF openingPos = { 0, 1000, 1850 };
    HuVecF mapRot = { -78, 0, 0 };
    HuVecF mapPos = { 100, -150, 700 };
    int i;
    int mdlId;
    for(i=0; i<5; i++) {
        numberMdlId[i] = MBModelCreate(numberFileTbl[i], NULL, FALSE);
        MBModelDispSet(numberMdlId[i], FALSE);
    }
    MBMasuCreate(W05S_BIN_masuData);
    MBScrollInit(W05S_HSF_scrollCol, 20000, 0);
    MBLightHookSet(MB5_LightSet, MB5_LightReset);
    MBCapsuleColCreate(W05S_HSF_capsuleCol);
    MBMasuPathCheckHookSet(MB5_MasuPathCheck);
    mb5_Work.bgMdlId = MBModelCreate(W05S_HSF_bg, NULL, FALSE);
    mb5_Work.symBaseMdlId = mdlId = MBModelCreate(W05S_HSF_symBase, NULL, FALSE);
    MBModelAttrSet(mdlId, HU3D_MOTATTR_LOOP);
    mb5_Work.bgPillarMdlId = mdlId = MBModelCreate(W05S_HSF_bgPillar, NULL, FALSE);
    MBModelAttrSet(mdlId, HU3D_MOTATTR_LOOP);
    mb5_Work.symMdlId = mdlId = MBModelCreate(W05S_HSF_sym, NULL, FALSE);
    MBMotionSpeedSet(mdlId, 1);
    MBModelAttrSet(mdlId, HU3D_MOTATTR_LOOP);
    for(i=0; i<3; i++) {
        mb5_Work.launchBMdlId[i] = mdlId = MBModelCreate(W05S_HSF_launchB, NULL, TRUE);
        MBModelPosSetV(mdlId, &mb5ev_LaunchBPosTbl[i]);
        MBModelRotSetV(mdlId, &mb5ev_LaunchBRotTbl[i]);
        MBMotionSpeedSet(mdlId, 0);
    }
    for(i=0; i<3; i++) {
        mb5_Work.rocketPathMdlId[i] = mdlId = MBModelCreate(W05S_HSF_rocketPathR+i, NULL, FALSE);
        MBMotionSpeedSet(mdlId, 0);
    }
    for(i=0; i<3; i++) {
        mb5_Work.rocketMdlId[i] = mdlId = MBModelCreate(W05S_HSF_rocketR+i, NULL, FALSE);
        MBMotionSpeedSet(mdlId, 0);
        MBModelPosSetV(mdlId, &mb5ev_RocketPosTbl[i]);
        if(i != 1) {
            MBModelDispSet(mdlId, FALSE);
        }
    }
    for(i=0; i<2; i++) {
        mb5_Work.warpMdlId[i] = mdlId = MBModelCreate(W05S_HSF_warp, NULL, TRUE);
        MBModelPosSetV(mdlId, &mb5ev_WarpPosTbl[i]);
        MBMotionSpeedSet(mdlId, 1);
        MBModelAttrSet(mdlId, HU3D_MOTATTR_LOOP);
    }
    for(i=0; i<2; i++) {
        mb5_Work.warpEffMdlId[i] = mdlId = MBModelCreate(W05S_HSF_warpEff, NULL, TRUE);
        MBModelAttrSet(mdlId, HU3D_MOTATTR_LOOP);
        MBMotionSpeedSet(mdlId, 1);
        MBModelPosSetV(mdlId, &mb5ev_WarpTogePosTbl[i]);
        MBModelDispSet(mdlId, FALSE);
    }
    for(i=0; i<2; i++) {
        mb5_Work.warpLayerMdlId[i] = mdlId = MBModelCreate(W05S_HSF_warpLayer, NULL, TRUE);
        MBMotionSpeedSet(mdlId, 0);
        MBModelPosSetV(mdlId, &mb5ev_WarpTogePosTbl[i]);
        MBModelDispSet(mdlId, FALSE);
    }
    for(i=0; i<2; i++) {
        mb5_Work.radarBodyMdlId[i] = mdlId = MBModelCreate(W05S_HSF_radarBody, NULL, TRUE);
        MBModelRotSetV(mdlId, &mb5ev_RaderBdyRotTbl[i]);
        MBModelPosSetV(mdlId, &mb5ev_RaderBdyPosTbl[i]);
        MBMotionSpeedSet(mdlId, 1);
        MBModelAttrSet(mdlId, HU3D_MOTATTR_LOOP);
    }
    for(i=0; i<2; i++) {
        mb5_Work.treeMdlId[i] = mdlId = MBModelCreate(W05S_HSF_tree, NULL, TRUE);
        MBModelRotSetV(mdlId, &mb5ev_Tree2RotTbl[i]);
        MBModelPosSetV(mdlId, &mb5ev_Tree2PosTbl[i]);
        MBMotionSpeedSet(mdlId, 1);
        MBModelAttrSet(mdlId, HU3D_MOTATTR_LOOP);
    }
    for(i=0; i<2; i++) {
        mb5_Work.rocketLightMdlId[i] = mdlId = MBModelCreate(W05S_HSF_rocketLight, NULL, TRUE);
        MBModelRotSetV(mdlId, &mb5ev_RokPLRotTbl[i]);
        MBModelPosSetV(mdlId, &mb5ev_RokPLPosTbl[i]);
        MBMotionSpeedSet(mdlId, 1);
        MBModelAttrSet(mdlId, HU3D_MOTATTR_LOOP);
    }
    for(i=0; i<9; i++) {
        mb5_Work.arrowMdlId[i] = mdlId = MBModelCreate(W05S_HSF_arrow1, NULL, TRUE);
        MBModelPosSetV(mdlId, &mb5ev_ArrowPosTbl[i]);
        MBModelRotSetV(mdlId, &mb5ev_ArrowRotTbl[i]);
        MBMotionSpeedSet(mdlId, 1);
        MBModelAttrSet(mdlId, HU3D_MOTATTR_LOOP);
    }
    MBBranchFlagSet(MB5EV_MASU_WARP_JUMP);
    HuDataDirClose(W05S_BIN_masuData);
    MBMasuWalkPreHookSet(MB5Ev_MasuWalkPre);
    MBMasuWalkPostHookSet(MB5Ev_MasuWalkPost);
    MBMasuHatenaHookSet(MB5Ev_MasuHatena);
    MBOpeningViewSet(&openingRot, &openingPos, 9000);
    MBMapViewCameraSet(&mapRot, &mapPos, 12300);
    HuAudSndGrpSetSet(MSM_GRP_BD5);
    mb5_MainObj = MBAddObj(32768, 0, 0, MB5_MainObjUpdate);
}

void MB5_Kill(void)
{
    int i;
    if(unkMdlId >= 0) {
        MBModelKill(unkMdlId);
        unkMdlId = MB_MODEL_NONE;
    }
    for(i=0; i<9; i++) {
        if(mb5_Work.arrowMdlId[i] != MB_MODEL_NONE) {
            MBModelKill(mb5_Work.arrowMdlId[i]);
        }
    }
    for(i=0; i<2; i++) {
        if(mb5_Work.warpEffMdlId[i] != MB_MODEL_NONE) {
            MBModelKill(mb5_Work.warpEffMdlId[i]);
        }
        if(mb5_Work.warpLayerMdlId[i] != MB_MODEL_NONE) {
            MBModelKill(mb5_Work.warpLayerMdlId[i]);
        }
    }
    for(i=0; i<3; i++) {
        if(mb5_Work.launchBMdlId[i] != MB_MODEL_NONE) {
            MBModelKill(mb5_Work.launchBMdlId[i]);
        }
        if(mb5_Work.rocketPathMdlId[i] != MB_MODEL_NONE) {
            MBModelKill(mb5_Work.rocketPathMdlId[i]);
        }
        if(mb5_Work.rocketMdlId[i] != MB_MODEL_NONE) {
            MBModelKill(mb5_Work.rocketMdlId[i]);
        }
    }
    if(mb5_Work.symBaseMdlId != MB_MODEL_NONE) {
        MBModelKill(mb5_Work.symBaseMdlId);
    }
    for(i=0; i<2; i++) {
        if(mb5_Work.warpMdlId[i] != MB_MODEL_NONE) {
            MBModelKill(mb5_Work.warpMdlId[i]);
        }
        if(mb5_Work.radarBodyMdlId[i] != MB_MODEL_NONE) {
            MBModelKill(mb5_Work.radarBodyMdlId[i]);
        }
        if(mb5_Work.treeMdlId[i] != MB_MODEL_NONE) {
            MBModelKill(mb5_Work.treeMdlId[i]);
        }
        if(mb5_Work.rocketLightMdlId[i] != MB_MODEL_NONE) {
            MBModelKill(mb5_Work.rocketLightMdlId[i]);
        }
    }
    if(mb5_Work.bgMdlId != MB_MODEL_NONE) {
        MBModelKill(mb5_Work.bgMdlId);
    }
    if(mb5_Work.bgPillarMdlId != MB_MODEL_NONE) {
        MBModelKill(mb5_Work.bgPillarMdlId);
    }
    if(mb5_Work.symMdlId != MB_MODEL_NONE) {
        MBModelKill(mb5_Work.symMdlId);
    }
    mb5_MainObj = NULL;
    for(i=0; i<5; i++) {
        if(numberMdlId[i] >= 0) {
            MBModelKill(numberMdlId[i]);
            numberMdlId[i] = MB_MODEL_NONE;
        }
    }
}

static void MB5_MainObjUpdate(OMOBJ *obj)
{
    if(MBKillCheck() || mb5_MainObj == NULL) {
        omDelObj(mbObjMan, obj);
        mb5_MainObj = NULL;
    }
}

void MB5_LightSet(void)
{
    HuVecF from = { -2700, 4700, 2300 };
    HuVecF to = { 2700, -4700, -2300 };
    u8 r = 0xFF, g = 0xFF, b = 0xFF;
    Hu3DShadowCreate(45, 1000, 250000);
    Hu3DShadowTPLvlSet(0.3f);
    Hu3DBGColorSet(0xFF, 0xFF, 0xFF);
    Hu3DGLightInfinitytSet(Hu3DGLightCreate(from.x, from.y, from.z, to.x, to.y, to.z, r, g, b));
}

void MB5_LightReset(void)
{
}

int MB5Ev_MasuHatena(int playerNo)
{
    u32 flag = MBMasuGet(0, GwPlayer[playerNo].masuId)->flag;
    if(flag & MB5EV_MASU_WARP) {
        MB5Ev_Warp(playerNo);
    }
    if(flag & MB5EV_MASU_ROCKET) {
        if((flag & MB5EV_MASU_ROCKET) == 2) {
            MB5Ev_Rocket(playerNo);
        }
    }
    return 0;
}

int MB5_MasuStub(void)
{
    return 0;
}

int MB5Ev_MasuWalkPre(int playerNo)
{
    return 0;
}

int MB5Ev_MasuWalkPost(int playerNo)
{
    MASU *masuP = MBMasuGet(0, GwPlayer[playerNo].masuId);
    u32 flag = masuP->flag;
    int type = MBMasuTypeGet(0, GwPlayer[playerNo].masuId);
    return 0;
}

void MB5Ev_WarpParticleCreate(HuVecF pos);
void MB5Ev_RocketDustCreate(HuVecF pos);
void MB5Ev_WarpPlayerShrink(int playerNo);
void MB5Ev_WarpPlayerGrow(int playerNo);

void MB5Ev_Warp(int playerNo)
{
    HuVecF pos;
    HuVecF nextPos;
    HuVecF scale;
    float alpha;
    s16 masuId;
    s16 nextMasuId;
    s16 destMasuId;
    int i;
    int j;
    float speed;

    masuId = GwPlayer[playerNo].masuId;
    MBPlayerPosFixFlagSet(FALSE);
    MBPlayerMotionNoShiftSet(playerNo, MB_PLAYER_MOT_WALK, 0, 8, HU3D_MOTATTR_LOOP);
    HuPrcSleep(10);
    HuPrcSleep(10);
    nextMasuId = MBMasuFlagMatchFindLink(0, masuId, MB5EV_MASU_WARP);
    GwPlayer[playerNo].masuIdPrev = GwPlayer[playerNo].masuId;
    GwPlayer[playerNo].masuIdNext = nextMasuId;
    MBPlayerMasuWalk(playerNo, TRUE);
    GwPlayer[playerNo].masuId = nextMasuId;
    HuPrcSleep(10);
    MBPlayerRotateStart(playerNo, 0, 15);
    while(!MBPlayerRotateCheck(playerNo)) {
        HuPrcVSleep();
    }
    HuPrcSleep(5);
    MBCameraPlayerViewSet(playerNo, NULL, NULL, 3200, -1, 30);
    MBCameraMotionWait();
    HuPrcSleep(30);
    omVibrate(playerNo, 120, 6, 6);
    MBPlayerPosGet(playerNo, &pos);
    MB5Ev_WarpParticleCreate(pos);
    alpha = 0;
    scale.x = scale.y = 1;
    scale.z = 0;
    speed = 1;
    for(i=0; i<90; i++) {
        speed += 0.3f;
        alpha = alpha + 0.01f;
        if(alpha >= 1) {
            alpha = 1;
        }
        for(j=0; j<2; j++) {
            MBMotionSpeedSet(mb5_Work.warpEffMdlId[j], speed);
            MBModelScaleSetV(mb5_Work.warpEffMdlId[j], &scale);
        }
        if(i == 20) {
            HuAudFXPlay(MSM_SE_BD5_10);
        }
        HuPrcVSleep();
    }
    alpha = 1;
    for(i=0; i<2; i++) {
        MBModelDispSet(mb5_Work.warpEffMdlId[i], FALSE);
        MBModelDispSet(mb5_Work.warpLayerMdlId[i], TRUE);
        MBMotionSpeedSet(mb5_Work.warpLayerMdlId[i], 1.1f);
        MBMotionTimeSet(mb5_Work.warpLayerMdlId[i], 0);
    }
    MB5Ev_WarpPlayerShrink(playerNo);
    HuPrcSleep(90);
    Hu3DParManKill(mb5ev_WarpParmanId);
    HuSprAnimKill(mb5ev_WarpParmanAnim);
    mb5ev_WarpParmanAnim = NULL;
    destMasuId = MBMasuFlagMatchFindLink(0, nextMasuId, MB5EV_MASU_WARP);
    MBMasuPosGet(0, destMasuId, &nextPos);
    MBPlayerPosSetV(playerNo, &nextPos);
    MBCameraPlayerViewSet(playerNo, NULL, NULL, -1, -1, 60);
    MBCameraMotionWait();
    MBPlayerRotateStart(playerNo, 0, 1);
    while(!MBPlayerRotateCheck(playerNo)) {
        HuPrcVSleep();
    }
    MBPlayerMotionNoShiftSet(playerNo, MB_PLAYER_MOT_WALK, 0, 8, HU3D_MOTATTR_LOOP);
    HuPrcSleep(30);
    MB5Ev_WarpParticleCreate(nextPos);
    omVibrate(playerNo, 120, 6, 6);
    for(i=0; i<70; i++) {
        if(i == 40) {
            HuAudFXPlay(MSM_SE_BD5_11);
        }
        HuPrcVSleep();
    }
    HuPrcSleep(10);
    for(i=0; i<2; i++) {
        MBModelAttrSet(mb5_Work.warpLayerMdlId[i], HU3D_MOTATTR_REV);
        MBMotionSpeedSet(mb5_Work.warpLayerMdlId[i], 1.1f);
    }
    MB5Ev_WarpPlayerGrow(playerNo);
    HuPrcSleep(10);
    for(i=0; i<2; i++) {
        MBModelAttrReset(mb5_Work.warpLayerMdlId[i], HU3D_MOTATTR_REV);
        MBMotionSpeedSet(mb5_Work.warpLayerMdlId[i], 0);
        MBMotionTimeSet(mb5_Work.warpLayerMdlId[i], 0);
        MBModelDispSet(mb5_Work.warpLayerMdlId[i], FALSE);
        MBModelDispSet(mb5_Work.warpEffMdlId[i], TRUE);
        MBMotionSpeedSet(mb5_Work.warpEffMdlId[i], speed);
        MBModelAttrSet(mb5_Work.warpEffMdlId[i], HU3D_MOTATTR_REV);
    }
    alpha -= 0.05f;
    for(i=0; i<60; i++) {
        speed -= 0.2f;
        alpha = alpha - (2.0f*0.01f);
        if(alpha <= 0) {
            alpha = 0;
        }
        for(j=0; j<2; j++) {
            MBMotionSpeedSet(mb5_Work.warpEffMdlId[j], speed);
            MBModelScaleSetV(mb5_Work.warpEffMdlId[j], &scale);
        }
        HuPrcVSleep();
    }
    MBCameraPlayerViewSet(playerNo, NULL, NULL, -1, -1, 6);
    MBCameraMotionWait();
    nextMasuId = MBMasuFlagMatchFindLink(0, destMasuId, MASU_FLAG_JUMPTO);
    MBPlayerPosFixCurr(nextMasuId, FALSE);
    GwPlayer[playerNo].masuIdPrev = destMasuId;
    GwPlayer[playerNo].masuIdNext = nextMasuId;
    MBPlayerMasuWalk(playerNo, TRUE);
    GwPlayer[playerNo].masuId = nextMasuId;
    MBCameraSkipSet(TRUE);
    MBPlayerRotateStart(playerNo, 0, 15);
    while(!MBPlayerRotateCheck(playerNo)) {
        HuPrcVSleep();
    }
    Hu3DParManKill(mb5ev_WarpParmanId);
    HuSprAnimKill(mb5ev_WarpParmanAnim);
    mb5ev_WarpParmanAnim = NULL;
    for(i=0; i<2; i++) {
        MBModelAttrReset(mb5_Work.warpEffMdlId[i], HU3D_MOTATTR_REV);
    }
    MBPlayerPosFixFlagSet(TRUE);
}

void MB5Ev_Rocket(int playerNo)
{
    HuVecF overlapPos[GW_PLAYER_MAX];
    Mtx hookMtx;
    Mtx masuMtx;
    HuVecF playerPos;
    HuVecF pos;
    HuVecF rocketPos;
    HuVecF cameraRot;
    HuVecF cameraPos;
    HuVecF startPos;
    HuVecF rocketMasuPos;
    s16 overlapPlayer[GW_PLAYER_MAX];
    int rocketMdlId;
    float zoom;
    u32 flag;
    MASU *masuP;
    MBCAMERA *cameraP;
    s16 no;
    s16 i;
    s16 j;
    s16 countdownNum;
    s16 overlapNum;
    s16 masuId;
    MASU *rocketMasuP;
    int seNo1;
    int seNo2;
    s16 numTime;
    s16 numAlpha;
    int playerMdlId;
    int pathMdlId;
    s16 numAlphaSpeed;
    s16 rocketMasuId;
    float time;
    float numScale;
    float t;
    float startTime;
    float numScaleSpeed;

    cameraP = MBCameraGet();
    masuId = GwPlayer[playerNo].masuId;
    masuP = MBMasuGet(0, masuId);
    flag = masuP->flag & MB5EV_MASU_ROCKET;
    MBPlayerPosFixFlagSet(FALSE);
    countdownNum = 3;
    numAlphaSpeed = 4;
    switch(flag) {
        case 1:
            MBPlayerRotateStart(playerNo, 225, 15);
            while(!MBPlayerRotateCheck(playerNo)) {
                HuPrcVSleep();
            }
            break;
        case 2:
            MBPlayerRotateStart(playerNo, 180, 15);
            while(!MBPlayerRotateCheck(playerNo)) {
                HuPrcVSleep();
            }
            break;
        case 3:
            MBPlayerRotateStart(playerNo, 135, 15);
            while(!MBPlayerRotateCheck(playerNo)) {
                HuPrcVSleep();
            }
            break;
    }
    no = flag-1;
    MBPlayerPosGet(playerNo, &startPos);
    for(i=overlapNum=0; i<GW_PLAYER_MAX; i++) {
        if(i != playerNo && MBPlayerAliveCheck(i) && masuId == GwPlayer[i].masuId) {
            overlapPlayer[overlapNum] = i;
            MBPlayerPosGet(i, &overlapPos[overlapNum]);
            overlapNum++;
        }
    }
    MBWinCreate(MBWIN_TYPE_RESULT, MESS_BOARD_W05_ROCKET_START, HUWIN_SPEAKER_NONE);
    MBTopWinWait();
    MBTopWinKill();
    MBCameraPlayerViewSet(playerNo, NULL, NULL, 3200, -1, 30);
    MBCameraMotionWait();
    pathMdlId = MBModelIdGet(mb5_Work.rocketPathMdlId[no]);
    MBMotionSpeedSet(mb5_Work.rocketPathMdlId[no], 1);
    MBMotionTimeSet(mb5_Work.rocketPathMdlId[no], 1);
    MBMotionSpeedSet(mb5_Work.launchBMdlId[no], 1);
    MBMotionTimeSet(mb5_Work.launchBMdlId[no], 1);
    rocketMdlId = MBModelIdGet(mb5_Work.rocketMdlId[no]);
    MBMotionSpeedSet(mb5_Work.rocketMdlId[no], 1);
    MBMotionTimeSet(mb5_Work.rocketMdlId[no], 0);
    playerMdlId = MBPlayerModelGet(playerNo);
    seNo1 = HuAudFXPlay(MSM_SE_BD5_15);
    for(i=0; i<80; i++) {
        Hu3DModelObjMtxGet(pathMdlId, rocketHookTbl_210[no], hookMtx);
        MTXTransApply(hookMtx, masuMtx, 0, 3, 0);
        MBMasuMtxSet(masuId, masuMtx);
        MBPlayerPosSet(playerNo, hookMtx[0][3], hookMtx[1][3], hookMtx[2][3]);
        for(j=0; j<overlapNum; j++) {
            MBPlayerPosGet(overlapPlayer[j], &overlapPos[j]);
            overlapPos[j].x += hookMtx[0][3]-startPos.x;
            overlapPos[j].y += hookMtx[1][3]-startPos.y;
            overlapPos[j].z += hookMtx[2][3]-startPos.z;
            MBPlayerPosSetV(overlapPlayer[j], &overlapPos[j]);
        }
        startPos.x = hookMtx[0][3];
        startPos.y = hookMtx[1][3];
        startPos.z = hookMtx[2][3];
        HuPrcVSleep();
    }
    HuAudFXStop(seNo1);
    seNo1 = -1;
    HuAudFXPlay(MSM_SE_BD5_16);
    HuPrcSleep(10);
    MBPlayerPosGet(playerNo, &playerPos);
    rocketPos = rocketPos_220[no];
    HuAudFXPlay(MSM_SE_BD5_01);
    HuPrcSleep(30);
    MBPlayerMotionNoShiftSet(playerNo, MB_PLAYER_MOT_WALK, 0, 8, HU3D_MOTATTR_LOOP);
    for(i=0; i<60.0f; i++) {
        if(MBMotionTimeGet(mb5_Work.rocketMdlId[no]) == 150) {
            MBMotionSpeedSet(mb5_Work.rocketMdlId[no], 0);
        }
        t = i/60.0f;
        pos.x = playerPos.x+((rocketPos.x-playerPos.x)*t);
        pos.y = playerPos.y+((rocketPos.y-playerPos.y)*t);
        pos.z = playerPos.z+((rocketPos.z-playerPos.z)*t);
        MBPlayerPosSetV(playerNo, &pos);
        HuPrcVSleep();
    }
    MBPlayerRotateStart(playerNo, playerRotTbl_230[no], 15);
    while(!MBPlayerRotateCheck(playerNo)) {
        HuPrcVSleep();
    }
    MBPlayerMotionNoShiftSet(playerNo, MB_PLAYER_MOT_IDLE, 0, 8, HU3D_MOTATTR_LOOP);
    MBMotionSpeedSet(mb5_Work.rocketMdlId[no], 1);
    while(1) {
        time = MBMotionTimeGet(mb5_Work.rocketMdlId[no]);
        if(time == 200) {
            break;
        }
        if(time == 160) {
            HuAudFXPlay(MSM_SE_BD5_01);
        } else if(time == 190) {
            HuAudFXPlay(MSM_SE_BD5_14);
        }
        HuPrcVSleep();
    }
    MBMotionSpeedSet(mb5_Work.rocketMdlId[no], 0);
    MBModelAttrSet(mb5_Work.launchBMdlId[no], HU3D_MOTATTR_REV);
    MBMotionSpeedSet(mb5_Work.launchBMdlId[no], 1.3f);
    MBModelAttrSet(mb5_Work.rocketPathMdlId[no], HU3D_MOTATTR_REV);
    MBMotionSpeedSet(mb5_Work.rocketPathMdlId[no], 1.3f);
    while(1) {
        time = MBMotionTimeGet(mb5_Work.launchBMdlId[no]);
        if(time <= 0) {
            break;
        }
        Hu3DModelObjMtxGet(pathMdlId, rocketHookTbl_210[no], hookMtx);
        MTXTransApply(hookMtx, masuMtx, 0, -3, 0);
        MBMasuMtxSet(masuId, masuMtx);
        for(j=0; j<overlapNum; j++) {
            MBPlayerPosGet(overlapPlayer[j], &overlapPos[j]);
            overlapPos[j].x += hookMtx[0][3]-startPos.x;
            overlapPos[j].y += hookMtx[1][3]-startPos.y;
            overlapPos[j].z += hookMtx[2][3]-startPos.z;
            MBPlayerPosSetV(overlapPlayer[j], &overlapPos[j]);
        }
        startPos.x = hookMtx[0][3];
        startPos.y = hookMtx[1][3];
        startPos.z = hookMtx[2][3];
        if(time == 80 && seNo1 == -1) {
            seNo1 = HuAudFXPlay(MSM_SE_BD5_15);
        }
        HuPrcVSleep();
    }
    HuAudFXStop(seNo1);
    HuAudFXPlay(MSM_SE_BD5_16);
    HuPrcSleep(40);
    pos.y += 170;
    pos.z += 200;
    while(1) {
        HuAudFXPlay(MSM_SE_BD5_17);
        MBModelDispSet(numberMdlId[countdownNum], TRUE);
        MBModelRotSet(numberMdlId[countdownNum], -33, 0, 0);
        MBModelPosSetV(numberMdlId[countdownNum], &pos);
        MBModelScaleSet(numberMdlId[countdownNum], 1.5f, 1.5f, 1.5f);
        MBModelAlphaSet(numberMdlId[countdownNum], 255);
        numAlpha = 255;
        numScaleSpeed = 1.0f/60.0f;
        for(numTime=0, numScale=1.5f; numTime<=60; numTime++) {
            numScale += numScaleSpeed;
            numAlpha -= numAlphaSpeed;
            MBModelScaleSet(numberMdlId[countdownNum], numScale, numScale, numScale);
            MBModelAlphaSet(numberMdlId[countdownNum], numAlpha);
            HuPrcVSleep();
        }
        MBModelDispSet(numberMdlId[countdownNum], FALSE);
        countdownNum--;
        HuPrcSleep(5);
        if(countdownNum <= 0) {
            break;
        }
        HuPrcVSleep();
    }
    MBModelDispSet(playerMdlId, FALSE);
    MBMotionSpeedSet(mb5_Work.rocketMdlId[no], 1);
    seNo2 = HuAudFXPlay(MSM_SE_BD5_02);
    MBPlayerPosGet(playerNo, &startPos);
    startPos.y -= 400;
    MB5Ev_RocketDustCreate(startPos);
    omVibrate(playerNo, 160, 4, 2);
    HuPrcSleep(180);
    HuAudFXStop(seNo2);
    startTime = MBMotionTimeGet(mb5_Work.rocketMdlId[no]);
    MBMotionSpeedSet(mb5_Work.rocketMdlId[no], 0);
    MBModelDispSet(mb5_Work.rocketMdlId[no], FALSE);
    rocketMasuId = MBMasuFlagFind(0, MASU_FLAG_RAISECHANCE, MASU_FLAG_RAISECHANCE);
    MBMasuPosGet(0, rocketMasuId, &pos);
    zoom = MBCameraZoomGet();
    MBCameraSkipSet(FALSE);
    MBCameraViewNoSet(GwSystem.turnPlayerNo, MB_CAMERA_VIEW_WALK);
    MBCameraMotionWait();
    MBStarScrollCreate(&playerPos, &pos, 180);
    MBCameraSkipSet(FALSE);
    MBCameraMotionWait();
    MBCameraSkipSet(TRUE);
    MBStarScrollStart();
    while(!MBStarScrollDoneCheck()) {
        HuPrcVSleep();
    }
    for(i=0; i<13; i++) {
        Hu3DParManKill(mb5ev_RocketDustParmanId[i]);
        mb5ev_RocketDustParmanId[i] = -1;
    }
    mb5ev_RocketDustAnim = NULL;
    MBModelPosSet(playerMdlId, pos.x, 430+pos.y, pos.z);
    MBCameraRotGet(&cameraRot);
    cameraRot.x += 15;
    MBCameraPosTargetGet(&cameraPos);
    MBCameraPlayerViewSet(playerNo, &cameraRot, NULL, 3400, -1, 150);
    MBModelRotSet(mb5_Work.rocketMdlId[no], 0, -playerRotTbl_230[no], 0);
    MBModelDispSet(mb5_Work.rocketMdlId[no], TRUE);
    MBModelPosSetV(mb5_Work.rocketMdlId[no], &pos);
    MBModelAttrSet(mb5_Work.rocketMdlId[no], HU3D_MOTATTR_REV);
    MBMotionSpeedSet(mb5_Work.rocketMdlId[no], 1);
    seNo2 = HuAudFXPlay(MSM_SE_BD5_03);
    omVibrate(playerNo, 160, 4, 2);
    while(1) {
        time = MBMotionTimeGet(mb5_Work.rocketMdlId[no]);
        if(time == 210) {
            break;
        }
        if(time == 280) {
            HuAudFXFadeOut(seNo2, 2500);
        }
        if(fabs(startTime-time) == 60 && startTime != -1) {
            MB5Ev_RocketDustCreate(pos);
            startTime = -1;
        }
        cameraPos.y += 5;
        MBCameraPosSetV(&cameraPos);
        HuPrcVSleep();
    }
    MBMotionSpeedSet(mb5_Work.rocketMdlId[no], 0);
    MBWinCreate(MBWIN_TYPE_RESULT, MESS_BOARD_W05_ROCKET_END, HUWIN_SPEAKER_NONE);
    MBTopWinWait();
    MBTopWinKill();
    WipeColorSet(0, 0, 0);
    WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_CROSSCOPY, 1);
    while(WipeCheckEnd()) {
        HuPrcVSleep();
    }
    MBModelDispSet(mb5_Work.rocketMdlId[no], FALSE);
    MBStarScrollKill();
    MBPlayerRotateStart(playerNo, 0, 15);
    while(!MBPlayerRotateCheck(playerNo)) {
        HuPrcVSleep();
    }
    rocketMasuP = MBMasuGet(0, rocketMasuId);
    MBMasuPosGet(0, MBMasuGet(0, rocketMasuP->linkTbl[0])->linkTbl[0], &rocketMasuPos);
    MBPlayerPosSetV(playerNo, &rocketMasuPos);
    GwPlayer[playerNo].masuId = MBMasuGet(0, rocketMasuP->linkTbl[0])->linkTbl[0];
    MBPlayerPosFixCurr(MBMasuGet(0, rocketMasuP->linkTbl[0])->linkTbl[0], TRUE);
    MBPlayerRotateStart(playerNo, 0, 1);
    while(!MBPlayerRotateCheck(playerNo)) {
        HuPrcVSleep();
    }
    MBPlayerMotionNoShiftSet(playerNo, MB_PLAYER_MOT_IDLE, 0, 8, HU3D_MOTATTR_LOOP);
    MBCameraSkipSet(FALSE);
    MBCameraViewNoSet(GwSystem.turnPlayerNo, MB_CAMERA_VIEW_ZOOMOUT);
    MBCameraMotionWait();
    MBCameraSkipSet(TRUE);
    MBCameraZoomSet(2200);
    MBModelDispSet(playerMdlId, TRUE);
    WipeCreate(WIPE_MODE_IN, WIPE_TYPE_CROSSCOPY, 30);
    while(WipeCheckEnd()) {
        MBCameraSkipSet(FALSE);
        HuPrcVSleep();
    }
    HuPrcSleep(30);
    MBMasuMtxSet(masuId, NULL);
    MBModelAttrReset(mb5_Work.launchBMdlId[no], HU3D_MOTATTR_REV);
    MBMotionSpeedSet(mb5_Work.launchBMdlId[no], 0);
    MBMotionTimeSet(mb5_Work.launchBMdlId[no], 0);
    MBModelAttrReset(mb5_Work.rocketPathMdlId[no], HU3D_MOTATTR_REV);
    MBMotionSpeedSet(mb5_Work.rocketPathMdlId[no], 0);
    MBMotionTimeSet(mb5_Work.rocketPathMdlId[no], 0);
    MBMotionSpeedSet(mb5_Work.rocketMdlId[no], 0);
    MBMotionTimeSet(mb5_Work.rocketMdlId[no], 0);
    MBModelPosSetV(mb5_Work.rocketMdlId[no], &rocketEndPos_200[no]);
    MBModelRotSet(mb5_Work.rocketMdlId[no], 0, 0, 0);
    MBModelAttrReset(mb5_Work.rocketMdlId[no], HU3D_MOTATTR_REV);
    MBModelDispSet(mb5_Work.rocketMdlId[no], TRUE);
    for(i=0; i<13; i++) {
        Hu3DParManKill(mb5ev_RocketDustParmanId[i]);
        mb5ev_RocketDustParmanId[i] = -1;
    }
    mb5ev_RocketDustAnim = NULL;
    HuAudFXStop(seNo2);
    MBPlayerPosFixFlagSet(TRUE);
}

void MB5Ev_WarpParticleCreate(HuVecF pos)
{
    mb5ev_WarpParmanAnim = HuSprAnimRead(HuDataReadNum(EFFECT_ANM_circle, HU_MEMNUM_OVL));
    HuDataDirClose(DATA_effect);
    mb5ev_WarpParmanId = Hu3DParManCreate(mb5ev_WarpParmanAnim, 256, &mb5ev_WarpEffParam);
    Hu3DParManAttrSet(mb5ev_WarpParmanId, HU3D_PARMAN_ATTR_RANDSCALE70|HU3D_PARMAN_ATTR_RANDANGLE);
    Hu3DParManRotSet(mb5ev_WarpParmanId, 0, 90, 0);
    Hu3DParManPosSet(mb5ev_WarpParmanId, pos.x, pos.y, pos.z);
    Hu3DModelLayerSet(Hu3DParManModelIDGet(mb5ev_WarpParmanId), 5);
    Hu3DParManTimeLimitSet(mb5ev_WarpParmanId, 120);
}

void MB5Ev_RocketDustCreate(HuVecF pos)
{
    int i;
    mb5ev_RocketDustAnim = HuSprAnimRead(HuDataReadNum(EFFECT_ANM_landDust, HU_MEMNUM_OVL));
    HuDataDirClose(DATA_effect);
    mb5ev_RocketDustParmanId[0] = Hu3DParManCreate(mb5ev_RocketDustAnim, 128, &mb5ev_RocketDustEffParam);
    Hu3DParManRotSet(mb5ev_RocketDustParmanId[0], 0, 0, 0);
    Hu3DParManPosSet(mb5ev_RocketDustParmanId[0], pos.x, pos.y, pos.z);
    Hu3DModelLayerSet(Hu3DParManModelIDGet(mb5ev_RocketDustParmanId[0]), 5);
    Hu3DParManTimeLimitSet(mb5ev_RocketDustParmanId[0], 90);
    for(i=1; i<13; i++) {
        mb5ev_RocketDustParmanId[i] = Hu3DParManCreate(mb5ev_RocketDustAnim, 128, &mb5ev_RocketDustEffParam);
        Hu3DParManRotSet(mb5ev_RocketDustParmanId[i], 0, 30.0f*i, 0);
        Hu3DParManPosSet(mb5ev_RocketDustParmanId[i], pos.x, pos.y, pos.z);
        Hu3DModelLayerSet(Hu3DParManModelIDGet(mb5ev_RocketDustParmanId[i]), 5);
        Hu3DParManTimeLimitSet(mb5ev_RocketDustParmanId[i], 90);
    }
}

void MB5Ev_WarpPlayerShrink(int playerNo)
{
    HuVecF scale = { 1, 1, 1 };
    int playerMdlId = MBPlayerModelGet(playerNo);
    while(1) {
        scale.x -= 0.04f;
        scale.y += 0.04f;
        scale.z += 0.04f;
        if(scale.x < 0) {
            break;
        }
        MBModelScaleSet(playerMdlId, scale.x, scale.y, scale.z);
        HuPrcVSleep();
    }
    mb5ev_WarpEndSize = scale;
    MBModelScaleSet(playerMdlId, 0, 0, 0);
    MBModelDispSet(playerMdlId, FALSE);
}

void MB5Ev_WarpPlayerGrow(int playerNo)
{
    int playerMdlId = MBPlayerModelGet(playerNo);
    HuVecF scale = mb5ev_WarpEndSize;
    MBModelScaleSet(playerMdlId, mb5ev_WarpEndSize.x, mb5ev_WarpEndSize.y, mb5ev_WarpEndSize.z);
    MBModelDispSet(playerMdlId, TRUE);
    while(1) {
        scale.x += 0.04f;
        scale.y -= 0.04f;
        scale.z -= 0.04f;
        if(scale.x > 1) {
            break;
        }
        MBModelScaleSet(playerMdlId, scale.x, scale.y, scale.z);
        HuPrcVSleep();
    }
    MBModelScaleSet(playerMdlId, 1, 1, 1);
}

int MB5_MasuPathCheck(s16 id, u32 flag, s16 *linkTbl, BOOL endF)
{
    return -1;
}
