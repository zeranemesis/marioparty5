#include "game/board/main.h"
#include "game/board/effect.h"
#include "game/board/audio.h"
#include "game/board/scroll.h"
#include "game/board/model.h"
#include "game/board/masu.h"
#include "game/board/opening.h"
#include "game/board/capsule.h"
#include "game/board/branch.h"
#include "game/board/camera.h"
#include "game/board/player.h"
#include "game/board/guide.h"
#include "game/board/window.h"
#include "game/board/coin.h"
#include "game/board/roulette.h"
#include "game/board/star.h"
#include "game/board/capsule.h"
#include "game/board/status.h"
#include "game/board/com.h"

#include "game/sprite.h"
#include "game/wipe.h"
#include "game/gamemes.h"
#include "game/pad.h"
#include "game/main.h"

#include "datanum/capsule.h"
#include "datanum/capsulechar0.h"

#include "datanum/w01s.h"
#include "messnum/board_w01.h"
#include "game/gxarray.h"

/*
 * w01sDll - Story Mode variant of the "Toy Dream" board (w01Dll).
 *
 * Same file layout and helpers as w01Dll (train path, particle effects,
 * coin train, hasi bridges), with the Story-mode differences:
 *  - no Koopa balloon (hikosen) emitter, no haunted-house / car sound
 *    effects and no hhouse turn bookkeeping: MB1_MainObjUpdate only
 *    handles its own teardown;
 *  - the train is announced by a Terl guide model created here (instead
 *    of MBGuideCreateFlag) that walks to the platform and turns around;
 *  - the train never gets a looping audio emitter (trainSeNo1/2 are gone);
 *  - MB1Ev_BKoopa is a short cutscene (no hook/explosion sequence), and
 *    the explosion burst is spawned directly from MB1_TumikiArmyObjUpdate.
 */

#define MB1EV_MASU_TRAIN1 MASU_FLAG_BIT(15)
#define MB1EV_MASU_HAKO MASU_FLAG_BIT(14)
#define MB1EV_MASU_TRAIN2 MASU_FLAG_BIT(13)
#define MB1EV_MASU_HASI1 MASU_FLAG_BIT(12)
#define MB1EV_MASU_BKOOPA MASU_FLAG_BIT(11)
#define MB1EV_MASU_HASI2 MASU_FLAG_BIT(10)
#define MB1EV_MASU_TUMIKI MASU_FLAG_BIT(9)
#define MB1EV_MASU_HASI3 MASU_FLAG_BIT(8)
#define MB1EV_MASU_TUMIKI_FALL MASU_FLAG_BIT(7)
#define MB1EV_MASU_HASI4 MASU_FLAG_BIT(6)
#define MB1EV_MASU_HHOUSE1 MASU_FLAG_BIT(5)
#define MB1EV_MASU_CAR MASU_FLAG_BIT(4)
#define MB1EV_MASU_HHOUSE2 MASU_FLAG_BIT(3)
#define MB1EV_MASU_HHOUSE3 MASU_FLAG_BIT(1)

typedef struct MB1_TrainPoint_s {
    int idx; //0x00
    int nextIdx; //0x04
    float endTime; //0x08
    float startTime; //0x0C
    HuVecF pos; //0x10
    HuVecF rot; //0x1C
} MB1_TRAIN_POINT;

typedef struct MB1_Work_s {
    int bkoopaMdlId; //0x00
    int tumikiMdlId; //0x04
    int trainMdlId[3]; //0x08
    int hakoMdlId; //0x14
    int hhouseMdlId[3]; //0x18
    int trainSenroMdlId; //0x24 (never created in Story mode, still killed)
    int hasiMdlId[2]; //0x28
    int carMdlId; //0x30
    int capsuleColMdlId; //0x34
    int coinMdlId; //0x38
    int guideMdlId; //0x3C
    int hasiMode[2]; //0x40
    BOOL hasiSeF[2]; //0x48
    int hasiMasu[2][2]; //0x50
    int carMasu; //0x60
    int trainDelay; //0x64
    int trainMode; //0x68
    int trainPointNum; //0x6C
    float trainMaxTime; //0x70
    float trainTime; //0x74
    float trainSpeed; //0x78
    float trainMaxSpeed; //0x7C
    float coinSpeed; //0x80
    HuVecF trainRot[3]; //0x84
    MB1_TRAIN_POINT *trainPoint; //0xA8
    HSFOBJECT *trainObj[7]; //0xAC
} MB1_WORK; // size 0xC8

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

static MB1_WORK mb1_Work;
static MBMODELID mb1_MainMdlId;
static s16 mb1_StarPathMasuNo;
static s16 mb1_StarPathLen;
static s16 mb1_StarPathValidF;
static int mb1ev_TumikiArmyTimer;
static OMOBJ *mb1_CoinEffObj;
static OMOBJ *mb1_ExplodeEffObj;
static OMOBJ *mb1_CoinObj;
static OMOBJ *mb1_MainObj;

static HuVecF mb1ev_TrainPath[] = {
    { -1.915, 469.464, 2751.58 },
    { -580.25, 471.633, 2641.25 },
    { -865.361, 468.953, 2628.77 },
    { -1138.152, 466.271, 2618.68 },
    { -1412.862, 460.911, 2705.01 },
    { -1662.488, 418.361, 2805.36 },
    { -1846.859, 375.812, 3001.11 },
    { -2020.446, 277.578, 3193.85 },
    { -2204.011, 179.343, 3393.24 },
    { -2432.948, 109.719, 3506.7 },
    { -2694.274, 93.488, 3556.32 },
    { -2974.024, 89.162, 3501.62 },
    { -3245.914, 84.836, 3394.85 },
    { -3676.448, 168.933, 3072.13 },
    { -3985.845, 271.324, 2627.57 },
    { -4181.565, 490.99, 2129.7 },
    { -4314.454, 685.88, 1650.22 },
    { -4435.715, 921.132, 1124.52 },
    { -4499.265, 1132.735, 610.37 },
    { -4493.411, 1315.934, 78.89 },
    { -4458.717, 1496.253, -464.13 },
    { -4319.93, 1670.751, -966.72 },
    { -4077.319, 1889.773, -1451.99 },
    { -3776.86, 2099.411, -1850.6 },
    { -3384.136, 2342.109, -2145.22 },
    { -2927.919, 2537.273, -2387.85 },
    { -2425.529, 2708.696, -2578.49 },
    { -1893.839, 2804.936, -2705.58 },
    { -1322.038, 2860.622, -2803.79 },
    { -784.705, 2890.001, -2861.56 },
    { -223.365, 2890, -2899.83 },
    { 344.425, 2890.002, -2894.47 },
    { 907.041, 2888.546, -2856.97 },
    { 1461.76, 2852.534, -2784.63 },
    { 2008.285, 2787.41, -2680.14 },
    { 2534.18, 2663.868, -2540.82 },
    { 3026.259, 2506.325, -2350.6 },
    { 3486.987, 2253.289, -2090.72 },
    { 3851.548, 2015.844, -1734.38 },
    { 4143.508, 1797.28, -1319.11 },
    { 4341.732, 1612.933, -823.46 },
    { 4451.56, 1437.517, -303.69 },
    { 4489.071, 1261.281, 234.82 },
    { 4464.979, 1068.554, 757.26 },
    { 4392.669, 854.23, 1266.31 },
    { 4282.809, 644.18, 1764.64 },
    { 4120.136, 417.804, 2265.65 },
    { 3891.377, 228.86, 2761.3 },
    { 3549.03, 124.486, 3167.9 },
    { 3088.843, 91.631, 3441 },
    { 2827.196, 91.214, 3533.01 },
    { 2555.644, 102.471, 3544.42 },
    { 2287.966, 157.495, 3471.38 },
    { 2109.762, 224.851, 3289.3 },
    { 1923.183, 295.461, 3084.75 },
    { 1742.887, 372.159, 2936.54 },
    { 1513.289, 435.319, 2824.36 },
    { 1218.543, 455.064, 2790.22 },
    { 948.879, 469.157, 2796.39 },
    { 419.908, 470.16, 2787.43 }
};

#define MB1EV_TRAINPATH_LEN (sizeof(mb1ev_TrainPath)/sizeof(HuVecF))

void MB1_Create(void);
void MB1_Kill(void);
void MB1_LightSet(void);
void MB1_LightReset(void);
int MB1Ev_MasuWalkPre(int playerNo);
int MB1Ev_MasuWalkPost(int playerNo);
int MB1Ev_MasuHatena(int playerNo);
int MB1_MasuPathCheck(s16 id, u32 flag, s16 *linkTbl, BOOL endF);
HSFOBJECT *MB1_ModelObjPtrGet(HU3DMODEL *modelP, char *name);

void MB1Ev_Tumiki(int playerNo);
void MB1Ev_BKoopa(int playerNo);
void MB1Ev_Hako(int playerNo);
void MB1Ev_Train(int playerNo);

void MB1_PlayerMtxSet(s16 playerNo, Mtx *mtx);

float MB1_AngleDiff(float a, float b);
float MB1_AngleStep(float a, float b, float t);
float MB1_AngleLerp(float t, float a, float b);

void MB1_BezierCalcV(float t, float *a, float *b, float *c, float *out);

float MB1_PathTimeGetEnd(float t, HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *d);
float MB1_PathTimeGet(float t, HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *d);

void MB1_TrainInit(void);
void MB1_TrainUpdate(void);
void MB1_ExplodeEffInit(void);

void MB1_TrainClose(void);
void MB1_TrainTimeSet(float time);
float MB1_TrainTimeGet(void);
void MB1_TrainSpeedSet(float speed);
void MB1_TrainCoinSpeedSet(float speed);

void MB1_TrainCoinInit(void);
int MB1_TrainCoinSearch(HuVecF *pos, HuVecF *out);
int MB1_TrainCoinCreate(Mtx *srcMtx, HuVecF *srcPos, Mtx *dstMtx, HuVecF *dstPos, float gravity, int maxTime);

void MB1_TrainCoinClose(void);
void MB1_ExplodeEffClose(void);
void MB1_CoinEffInit(void);

void MB1_CoinEffClose(void);
int MB1_CoinEffCreate(HuVecF pos, HuVecF vel, float scale, float speed, float gravity, GXColor color);

s16 MB1_StarPathLenGet(s16 masuId, s16 len);

void MB1_HasiOpen(int no);
void MB1_HasiClose(int no);

void ObjectSetup(void)
{
    GWPartyFSet(FALSE);
    MBObjectSetup(MBNO_WORLD_1, MB1_Create, MB1_Kill);
}

static void MB1_MainObjUpdate(OMOBJ *obj);

void MB1_Create(void)
{
    int boardNo = MBBoardNoGet();
    int id;
    int masuNum;

    HuAudSndGrpSetSet(MSM_GRP_BD1);
    MBMasuCreate(W01S_BIN_masuData);
    mb1_MainMdlId = MBModelCreate(W01S_HSF_map, NULL, FALSE);
    MBModelPosSet(mb1_MainMdlId, 0, 0, 0);
    MBModelAttrSet(mb1_MainMdlId, HU3D_MOTATTR_LOOP);
    MBModelUnkSpeedSet(mb1_MainMdlId, -1);
    MBScrollInit(W01S_HSF_scrollCol, 20000, 0);
    MBLightHookSet(MB1_LightSet, MB1_LightReset);
    mb1_Work.bkoopaMdlId = id = MBModelCreate(W01S_HSF_bkoopa, NULL, FALSE);
    MBMotionSpeedSet(id, 0);
    mb1_Work.tumikiMdlId = id = MBModelCreate(W01S_HSF_tumiki, NULL, FALSE);
    MBMotionSpeedSet(id, 0);
    mb1_Work.trainMdlId[0] = id = MBModelCreate(W01S_HSF_trainA, NULL, FALSE);
    mb1_Work.trainMdlId[1] = id = MBModelCreate(W01S_HSF_trainB, NULL, FALSE);
    mb1_Work.trainMdlId[2] = id = MBModelCreate(W01S_HSF_trainC, NULL, FALSE);
    mb1_Work.hakoMdlId = id = MBModelCreate(W01S_HSF_hako, NULL, FALSE);
    MBMotionSpeedSet(id, 0);
    mb1_Work.hhouseMdlId[0] = id = MBModelCreate(W01S_HSF_hhouse1, NULL, FALSE);
    MBMotionSpeedSet(id, 0);
    mb1_Work.hhouseMdlId[1] = id = MBModelCreate(W01S_HSF_hhouse2, NULL, FALSE);
    MBMotionSpeedSet(id, 0);
    mb1_Work.hhouseMdlId[2] = id = MBModelCreate(W01S_HSF_hhouse3, NULL, FALSE);
    MBMotionSpeedSet(id, 0);
    mb1_Work.hasiMdlId[0] = id = MBModelCreate(W01S_HSF_hasi01, NULL, FALSE);
    MBMotionSpeedSet(id, 0);
    mb1_Work.hasiMdlId[1] = id = MBModelCreate(W01S_HSF_hasi02, NULL, FALSE);
    MBMotionSpeedSet(id, 0);
    mb1_Work.carMdlId = id = MBModelCreate(W01S_HSF_car, NULL, FALSE);
    MBMotionTimeSet(id, MBMotionMaxTimeGet(id));
    mb1_Work.coinMdlId = MB_MODEL_NONE;
    mb1_Work.guideMdlId = MB_MODEL_NONE;
    mb1_Work.capsuleColMdlId = MBCapsuleColCreate(W01S_HSF_capsuleCol);
    mb1_Work.hasiMode[0] = 0;
    mb1_Work.hasiMode[1] = 0;
    mb1_Work.hasiSeF[0] = FALSE;
    mb1_Work.hasiSeF[1] = FALSE;
    masuNum = MBMasuNumGet(MASU_LAYER_DEFAULT);
    for(id=1; id<=masuNum; id++) {
        if(MBMasuFlagGet(MASU_LAYER_DEFAULT, id) & MB1EV_MASU_HASI1) {
            break;
        }
    }
    mb1_Work.hasiMasu[0][0] = id;
    for(id=1; id<=masuNum; id++) {
        if(MBMasuFlagGet(MASU_LAYER_DEFAULT, id) & MB1EV_MASU_HASI2) {
            break;
        }
    }
    mb1_Work.hasiMasu[0][1] = id;
    for(id=1; id<=masuNum; id++) {
        if(MBMasuFlagGet(MASU_LAYER_DEFAULT, id) & MB1EV_MASU_HASI3) {
            break;
        }
    }
    mb1_Work.hasiMasu[1][0] = id;
    for(id=1; id<=masuNum; id++) {
        if(MBMasuFlagGet(MASU_LAYER_DEFAULT, id) & MB1EV_MASU_HASI4) {
            break;
        }
    }
    mb1_Work.hasiMasu[1][1] = id;
    for(id=1; id<=masuNum; id++) {
        if(MBMasuFlagGet(MASU_LAYER_DEFAULT, id) & MB1EV_MASU_CAR) {
            break;
        }
    }
    mb1_Work.carMasu = id;
    MBBranchFlagSet(MB1EV_MASU_TUMIKI_FALL);
    HuDataDirClose(DATA_w01s);
    MBMasuWalkPreHookSet(MB1Ev_MasuWalkPre);
    MBMasuWalkPostHookSet(MB1Ev_MasuWalkPost);
    MBMasuHatenaHookSet(MB1Ev_MasuHatena);
    MBMasuPathCheckHookSet(MB1_MasuPathCheck);
    mb1_MainObj = MBAddObj(32768, 0, 0, MB1_MainObjUpdate);
    MB1_TrainInit();
    MB1_ExplodeEffInit();
    {
        static HuVecF openingRot = { -33, 0, 0 };
        static HuVecF openingOfs = { 305, 894, 4257 };
        static float openingZoom = 5471;
        static HuVecF mapViewRot = { -78, -17, 0 };
        static HuVecF mapViewPos = { 369, -556, 2798 };
        static float mapViewZoom = 8330;
        MBOpeningViewSet(&openingRot, &openingOfs, openingZoom);
        MBMapViewCameraSet(&mapViewRot, &mapViewPos, mapViewZoom);
    }
}

void MB1_Kill(void)
{
    if(mb1_MainMdlId >= 0) {
        MBModelKill(mb1_MainMdlId);
        mb1_MainMdlId = MB_MODEL_NONE;
    }
    if(mb1_Work.bkoopaMdlId != MB_MODEL_NONE) {
        MBModelKill(mb1_Work.bkoopaMdlId);
    }
    if(mb1_Work.tumikiMdlId != MB_MODEL_NONE) {
        MBModelKill(mb1_Work.tumikiMdlId);
    }
    if(mb1_Work.trainMdlId[0] != MB_MODEL_NONE) {
        MBModelKill(mb1_Work.trainMdlId[0]);
    }
    if(mb1_Work.trainMdlId[1] != MB_MODEL_NONE) {
        MBModelKill(mb1_Work.trainMdlId[1]);
    }
    if(mb1_Work.trainMdlId[2] != MB_MODEL_NONE) {
        MBModelKill(mb1_Work.trainMdlId[2]);
    }
    if(mb1_Work.hakoMdlId != MB_MODEL_NONE) {
        MBModelKill(mb1_Work.hakoMdlId);
    }
    if(mb1_Work.hhouseMdlId[0] != MB_MODEL_NONE) {
        MBModelKill(mb1_Work.hhouseMdlId[0]);
    }
    if(mb1_Work.hhouseMdlId[1] != MB_MODEL_NONE) {
        MBModelKill(mb1_Work.hhouseMdlId[1]);
    }
    if(mb1_Work.hhouseMdlId[2] != MB_MODEL_NONE) {
        MBModelKill(mb1_Work.hhouseMdlId[2]);
    }
    if(mb1_Work.trainSenroMdlId != MB_MODEL_NONE) {
        MBModelKill(mb1_Work.trainSenroMdlId);
    }
    if(mb1_Work.hasiMdlId[0] != MB_MODEL_NONE) {
        MBModelKill(mb1_Work.hasiMdlId[0]);
    }
    if(mb1_Work.hasiMdlId[1] != MB_MODEL_NONE) {
        MBModelKill(mb1_Work.hasiMdlId[1]);
    }
    if(mb1_Work.carMdlId != MB_MODEL_NONE) {
        MBModelKill(mb1_Work.carMdlId);
    }
    if(mb1_Work.capsuleColMdlId != MB_MODEL_NONE) {
        MBModelKill(mb1_Work.capsuleColMdlId);
    }
    MBCapsuleColCreate(HU_DATANUM_NONE);
    if(mb1_Work.coinMdlId != MB_MODEL_NONE) {
        MBModelKill(mb1_Work.coinMdlId);
    }
    if(mb1_Work.guideMdlId != MB_MODEL_NONE) {
        MBModelKill(mb1_Work.guideMdlId);
    }
    mb1_MainObj = NULL;
    MB1_TrainClose();
    MB1_ExplodeEffClose();
    MB1_CoinEffClose();
    MB1_TrainCoinClose();
}

static void MB1_MainObjUpdate(OMOBJ *obj)
{
    int i; //r31
    int j; //r30
    int hhouseMdlId; //r29
    int modelId; //r28

    static char *hasiName[2][2] = {
        { "hasi1", "hasi2" },
        { "hasi3", "hasi4" }
    };
    if(MBKillCheck() || mb1_MainObj == NULL) {
        omDelObj(mbObjMan, obj);
        mb1_MainObj = NULL;
        return;
    }
}

void MB1_LightSet(void)
{
    Hu3DBGColorSet(255, 255, 255);
    Hu3DModelLightInfoSet(MBModelIdGet(mb1_MainMdlId), TRUE);

}

void MB1_LightReset(void)
{

}

int MB1Ev_MasuHatena(int playerNo)
{
    s16 masuId = GwPlayer[playerNo].masuId;
    MASU *masuP = MBMasuGet(MASU_LAYER_DEFAULT, masuId);
    if(masuP->flag & MB1EV_MASU_TUMIKI) {
        MB1Ev_Tumiki(playerNo);
    }
    if(masuP->flag & MB1EV_MASU_BKOOPA) {
        MB1Ev_BKoopa(playerNo);
    }
    if(masuP->flag & MB1EV_MASU_HAKO) {
        MB1Ev_Hako(playerNo);
    }
    if(masuP->flag & (MB1EV_MASU_TRAIN1|MB1EV_MASU_TRAIN2)) {
        MBPlayerPosFixFlagSet(TRUE);
        MB1Ev_Train(playerNo);
        MBPlayerPosFixFlagSet(FALSE);
    }
    HuDataDirClose(DATA_guide);
    HuDataDirClose(DATA_capsulechar0);
}

void MB1_MasuGet(void)
{
    int playerNo = GwSystem.turnPlayerNo;
    s16 masuId = GwPlayer[playerNo].masuId;
    MASU *masuP = MBMasuGet(MASU_LAYER_DEFAULT, masuId);
}

int MB1Ev_MasuWalkPre(int playerNo)
{

}

int MB1Ev_MasuWalkPost(int playerNo)
{
    s16 masuId = GwPlayer[playerNo].masuId;
    MASU *masuP = MBMasuGet(MASU_LAYER_DEFAULT, masuId);
    MBMODELID mdlId;

    if(masuP->flag & MB1EV_MASU_HHOUSE3) {
        mdlId = mb1_Work.hhouseMdlId[2];
        MBMotionSpeedSet(mdlId, 1);
        MBMotionTimeSet(mdlId, 0);
    }
    if(masuP->flag & MB1EV_MASU_HHOUSE2) {
        mdlId = mb1_Work.hhouseMdlId[1];
        MBMotionSpeedSet(mdlId, 1);
        MBMotionTimeSet(mdlId, 0);
    }
    if(masuP->flag & MB1EV_MASU_HHOUSE1) {
        mdlId = mb1_Work.hhouseMdlId[0];
        MBMotionSpeedSet(mdlId, 1);
        MBMotionTimeSet(mdlId, 0);
    }
    if(masuP->flag & MB1EV_MASU_CAR) {
        mdlId = mb1_Work.carMdlId;
        MBMotionSpeedSet(mdlId, 1);
        MBMotionTimeSet(mdlId, 0);
    }
    return FALSE;
}

int MB1_MasuPathCheck(s16 id, u32 flag, s16 *linkTbl, BOOL endF)
{
    return -1;
}

static void MB1_TumikiArmyObjUpdate(OMOBJ *obj);
static void MB1_TumikiArmyPosFix(OMOBJ *obj);
static void MB1_TrainObjUpdate(OMOBJ *obj);

static void MB1_TumikiArmyPosFix(OMOBJ *obj)
{
    HSFOBJECT *hsfObj; //r31
    HU3DMODEL *modelP; //r29
    int modelId; //r28
    HSFDATA *hsf; //sp+0x8
    if(MBKillCheck()) {
        obj->work[0] = 1;
    }
    switch(obj->work[0]) {
        case 0:
            modelId = MBModelIdGet(obj->work[1]);
            modelP = &Hu3DData[modelId];
            hsf = modelP->hsf;
            hsfObj = MB1_ModelObjPtrGet(modelP, "army_b4");
            if(hsfObj != NULL) {
                if(++hsfObj->mesh.base.rot.z >= 360) {
                    hsfObj->mesh.base.rot.x -= 360;
                }
            }
            hsfObj = MB1_ModelObjPtrGet(modelP, "army_a4");
            if(hsfObj != NULL) {
                if(++hsfObj->mesh.base.rot.z >= 360) {
                    hsfObj->mesh.base.rot.x -= 360;
                }
            }
            break;

        case 1:
            omDelObj(mbObjMan, obj);
            break;
    }

}

float MB1_AngleDiff(float a, float b)
{
    float delta;
    if(a >= 360.0f) {
        a -= 360.0f;
    } else if(a < 0.0f) {
        a += 360.0f;
    }
    if(b >= 360.0f) {
        b -= 360.0f;
    } else if(b < 0.0f) {
        b += 360.0f;
    }
    delta = a-b;
    if(delta <= -180.0f) {
        delta += 360.0f;
    } else if(delta >= 180.0f) {
        delta -= 360.0f;
    }
    return delta;
}

float MB1_AngleStep(float a, float b, float step)
{
    float ret;
    float delta;

    if(a >= 360.0) {
        a -= 360.0;
    } else if(a < 0.0) {
        a += 360.0;
    }
    if(b >= 360.0) {
        b -= 360.0;
    } else if(b < 0.0) {
        b += 360.0;
    }
    delta = 360.0+(a-b);
    if(fabs(delta) >= 360.0) {
        delta = fmod(delta, 360);
    }
    if(delta < 180.0) {
        if(delta <= step) {
            ret = delta;
        } else {
            ret = step;
        }
    } else if(360.0-delta <= step) {
        ret = -(360.0-delta);
    } else {
        ret = -step;
    }
    ret += b;
    if(ret >= 360.0) {
        ret -= 360.0;
    } else if(ret < 0.0) {
        ret += 360.0;
    }
    return ret;
}

float MB1_AngleLerp(float t, float a, float b)
{
    float delta = MB1_AngleDiff(b, a);
    return MB1_AngleStep(b, a, fabs(delta*t));
}

void MB1_ModelMtxSet(MBMODELID mdlId, Mtx *mtx)
{
    HU3DMODEL *modelP;
    int id;
    id = MBModelIdGet(mdlId);
    modelP = &Hu3DData[id];
    MTXCopy(*mtx, modelP->mtx);

}

void MB1_PlayerMtxSet(s16 playerNo, Mtx *mtx)
{
    MB1_ModelMtxSet(MBPlayerModelGet(playerNo), mtx);
}

float MB1_BezierCalc(float t, float a, float b, float c)
{
    float invT = 1.0-t;
    float ret = (a*(invT*invT))+(2.0*(b*(invT*t)))+(c*(t*t));
    return ret;
}

void MB1_BezierCalcV(float t, float *a, float *b, float *c, float *out)
{
    int i;
    for(i=0; i<3;i++) {
        *out++ = MB1_BezierCalc(t, *a++, *b++, *c++);
    }
}

void MB1_HermiteCoefGet(float t, float *p0Coef, float *p1Coef, float *m0Coef, float *m1Coef)
{
    float t2 = t*t;
    float t3 = t*t2;
    float coef = ((3.0*t2)-t3)-t3;
    *p0Coef = 1.0-coef;
    *p1Coef = coef;
    *m0Coef = t+((t3-t2)-t2);
    *m1Coef = t3-t2;
}

float MB1_HermiteEval(float t, float p0, float p1, float p2, float p3, float d)
{
    float pDelta;

    float p0Coef;
    float p1Coef;
    float m0Coef;
    float m1Coef;
    float ret;
    float d1;
    float d2;
    float m0;
    float m1;
    int pDeltai;

    pDelta = p2-p1;
    pDeltai = p2-p1;
    if(p1 == p2) {
        m0 = pDelta;
    } else {
        d1 = d;
        m0 = d1*(pDelta+(p1-p0));
    }
    if(p1 == p3) {
        m1 = pDelta;
    } else {
        d2 = d;
        m1 = d2*(pDelta+(p3-p2));
    }
    MB1_HermiteCoefGet(t, &p0Coef, &p1Coef, &m0Coef, &m1Coef);
    return ret = (p0Coef*p1)+(p1Coef*p2)+(m0Coef*m0)+(m1Coef*m1);
}

void MB1_HermiteCalcV(float t, HuVecF *p0, HuVecF *p1, HuVecF *p2, HuVecF *p3, HuVecF *out)
{
    float d = 0.5f;
    out->x = MB1_HermiteEval(t, p0->x, p1->x, p2->x, p3->x, d);
    out->y = MB1_HermiteEval(t, p0->y, p1->y, p2->y, p3->y, d);
    out->z = MB1_HermiteEval(t, p0->z, p1->z, p2->z, p3->z, d);
}

float MB1_HermiteSlopeCalc(float t, float p0, float p1, float p2, float p3)
{
    float t2 = t*t;
    float p0Coef = (6.0f*t2)-(6.0f*t);
    float p1Coef = (-6.0f*t2)-(6.0f*t);
    float m0Coef = 1+((3.0f*t2)-(4.0f*t));
    float m1Coef = (3.0f*t2)-(2.0f*t);
    return (p0Coef*p0)+(p1Coef*p1)+(m0Coef*p2)+(m1Coef*p3);
}

float MB1_PathTimeGetEnd(float t, HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *d)
{
    return MB1_PathTimeGet(1, a, b, c, d);
}

float MB1_QuadCalc(float t, float p0, float p1, float p2, float p3)
{
    float mid1 = 0.5f*((p2+(p1-p0))-p1);
    float mid2 = 0.5f*((p2+(p1-p0))-p1);
    float a = mid1+((2.0f*p1)-(2.0f*p2))+mid2;
    float b = (((-3.0f*p1)+(3.0f*p2))-(2.0f*mid1))-mid2;
    float c = mid1;
    return (t*((3.0f*a)*t))+(2.0f*b*t)+c;
}

float MB1_QuadCalcMag(float t, HuVecF *p0, HuVecF *p1, HuVecF *p2, HuVecF *p3)
{
    float x = MB1_QuadCalc(t, p0->x, p1->x, p2->x, p3->x);
    float y = MB1_QuadCalc(t, p0->y, p1->y, p2->y, p3->y);
    float z = MB1_QuadCalc(t, p0->z, p1->z, p2->z, p3->z);
    return sqrtf((x*x)+(y*y)+(z*z));
}

float MB1_PathTimeGet(float time, HuVecF *p0, HuVecF *p1, HuVecF *p2, HuVecF *p3)
{
    float min = 0;
    float max = time;
    int numStep = 16;
    float stepHalfW = (max-min)/numStep;
    float stepSize = stepHalfW+stepHalfW;
    float totalLen = MB1_QuadCalcMag(min, p0, p1, p2, p3)+MB1_QuadCalcMag(max, p0, p1, p2, p3);
    int i;
    float t;
    for(i=1, t=min+stepHalfW; i<numStep; i += 2, t += stepSize) {
        totalLen += MB1_QuadCalcMag(t, p0, p1, p2, p3)*4;
    }
    for(i=2, t=min+stepSize; i<numStep; i += 2, t += stepSize) {
        totalLen += MB1_QuadCalcMag(t, p0, p1, p2, p3)*2;
    }
    return (stepHalfW/3)*totalLen;
}

void MB1_QuadCalcRot(float t, HuVecF *p0, HuVecF *p1, HuVecF *p2, HuVecF *p3, HuVecF *out)
{
    HuVecF dir;
    dir.x = MB1_QuadCalc(t, p0->x, p1->x, p2->x, p3->x);
    dir.y = MB1_QuadCalc(t, p0->y, p1->y, p2->y, p3->y);
    dir.z = MB1_QuadCalc(t, p0->z, p1->z, p2->z, p3->z);

    VECNormalize(&dir, &dir);
    out->x = HuAtan(-dir.y, HuMagXZVecF(&dir));
    out->y = HuAtan(dir.x, dir.z);
    out->z = 0;
}

static HSFOBJECT *objCall(HU3DMODEL *modelP, char *name, HSFOBJECT *obj);

HSFOBJECT *MB1_ModelObjPtrGet(HU3DMODEL *modelP, char *name)
{
    HSFDATA *hsf =  modelP->hsf;
    return objCall(modelP, name, hsf->root);
}

static HSFOBJECT *objCall(HU3DMODEL *modelP, char *name, HSFOBJECT *obj)
{
    int i;
    HSFOBJECT *meshObj;
    HSFOBJECT *replicaObj;
    HSFOBJECT *ret;

    switch(obj->type) {
        case HSF_OBJ_NULL1:
        case HSF_OBJ_MESH:
        case HSF_OBJ_ROOT:
        case HSF_OBJ_JOINT:
        case HSF_OBJ_NULL2:
        case HSF_OBJ_NULL3:
        case HSF_OBJ_MAP:
        {
            meshObj = obj;
            if(!strcmp(name, meshObj->name)) {
                return meshObj;
            }
            for(i=0; i<meshObj->mesh.childNum; i++) {
                ret = objCall(modelP, name, meshObj->mesh.child[i]);
                if(ret) {
                    return ret;
                }
            }
        }
            break;

        case HSF_OBJ_REPLICA:
        {
            replicaObj = obj;
            if(!strcmp(name, replicaObj->name)) {
                return replicaObj;
            }
            for(i=0; i<replicaObj->mesh.childNum; i++) {
                ret = objCall(modelP, name, replicaObj->mesh.child[i]);
                if(ret) {
                    return ret;
                }
            }
        }
        break;
    }
    return NULL;
}

static s16 PathStarCheck(s16 masuId, s16 len);

s16 MB1_StarPathLenGet(s16 masuId, s16 len)
{
    mb1_StarPathMasuNo = len;
    mb1_StarPathLen = len;
    mb1_StarPathValidF = FALSE;
    PathStarCheck(masuId, len);
    if(mb1_StarPathValidF) {
        return mb1_StarPathLen;
    } else {
        return -1;
    }
}

static s16 PathStarCheck(s16 masuId, s16 len)
{
    s16 newLen;
    s16 result = FALSE;
    MASU *masuP = MBMasuGet(MASU_LAYER_DEFAULT, masuId);
    s16 i;
    if(MBMasuTypeGet(MASU_LAYER_DEFAULT, masuId) == MASU_TYPE_STAR) {
        newLen = mb1_StarPathMasuNo-len;
        if(newLen < mb1_StarPathLen) {
            mb1_StarPathLen = newLen;
        }
        mb1_StarPathValidF = TRUE;
        return TRUE;
    } else {
        if(MBMasuTypeGet(MASU_LAYER_DEFAULT, masuId) != MASU_TYPE_NONE &&MBMasuTypeGet(MASU_LAYER_DEFAULT, masuId) != MASU_TYPE_STAR) {
            len--;
        }
        if(len >= 0) {
            for(i=0; i<masuP->linkNum; i++) {
                if(!(MBMasuFlagGet(MASU_LAYER_DEFAULT, masuP->linkTbl[i]) & (MBBranchFlagGet()|(MASU_FLAG_BLOCKL|MASU_FLAG_BLOCKR)))) {
                    if(PathStarCheck(masuP->linkTbl[i], len)) {
                        result = TRUE;
                    }
                }
            }
        }

        if(result) {
            return TRUE;
        } else{
            return FALSE;
        }
    }
}

void MB1_TrainCoinUpdate(OMOBJ *obj);

#define MB1_TRAIN_COIN_MAX 32

typedef struct MB1_TrainCoin_s {
    int mdlId; //0x00
    BOOL enableF; //0x04
    int mode; //0x08
    float gravity; //0x0C
    int maxTime;  //0x10
    int time; //0x14
    HuVecF pos; //0x18
    HuVecF rot; //0x24
    HuVecF scale; //0x30
    HuVecF srcPos; //0x3C
    HuVecF dstPos; //0x48
    HuVecF vel; //0x54
    Mtx *srcMtx; //0x60
    Mtx *dstMtx; //0x64
} MB1_TRAIN_COIN;

void MB1_TrainCoinInit(void)
{
    MB1_TRAIN_COIN *coin; //r31
    int i; //r30
    OMOBJ *obj; //r29
    obj = mb1_CoinObj = MBAddObj(32768, 0, 0, MB1_TrainCoinUpdate);
    coin = obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MB1_TRAIN_COIN)*MB1_TRAIN_COIN_MAX, HU_MEMNUM_OVL);
    memset(coin, 0, sizeof(MB1_TRAIN_COIN)*MB1_TRAIN_COIN_MAX);
    for(i=0; i<MB1_TRAIN_COIN_MAX;i++, coin++) {
        coin->mdlId = Hu3DModelCreateData(BOARD_HSF_coin);
        Hu3DModelCameraSet(coin->mdlId, HU3D_CAM0);
        Hu3DModelDispOff(coin->mdlId);
        coin->enableF = FALSE;
        coin->mode = 0;
        coin->gravity = 0;
        coin->maxTime = coin->time = 0;
        coin->srcMtx = coin->dstMtx = NULL;
    }
}

void MB1_TrainCoinUpdate(OMOBJ *obj)
{
    MB1_TRAIN_COIN *coin; //r31
    int j; //r30
    int i; //r29
    BOOL coinGrabF; //r28
    BOOL coinDisableF; //r27

    float angle; //f30
    float t; //f29
    float intensity; //f27
    float angleX; //f26

    HuVecF effectPos; //sp+0x68
    HuVecF srcPos; //sp+0x5C
    HuVecF dstPos; //sp+0x50
    HuVecF pos; //sp+0x44
    HuVecF playerPos; //sp+0x38
    HuVecF effectVel; //sp+0x2C
    GXColor color; //sp+0x10

    coin = obj->data;
    if(MBKillCheck() || mb1_CoinObj == NULL) {
        for(i=0; i<MB1_TRAIN_COIN_MAX; i++, coin++) {
            Hu3DModelKill(coin->mdlId);
            coin->mdlId = HU3D_MODELID_NONE;
        }
        omDelObj(mbObjMan, obj);
        mb1_CoinObj = NULL;
        return;
    }
    for(i=0; i<MB1_TRAIN_COIN_MAX; i++, coin++) {
        if(!coin->enableF) {
            continue;
        }
        coinDisableF = FALSE;
        coinGrabF = FALSE;
        switch(coin->mode) {
            case 0:
                t = (++coin->time/(float)coin->maxTime);
                MTXMultVec(*coin->srcMtx, &coin->srcPos, &srcPos);
                MTXMultVec(*coin->dstMtx, &coin->dstPos, &dstPos);
                VECSubtract(&dstPos, &srcPos, &pos);
                VECScale(&pos, &pos, t);
                VECAdd(&pos, &srcPos, &pos);
                pos.y += coin->gravity*HuSin(t*180);
                coin->pos = pos;
                coin->rot.y += 10;
                if(t >= 1) {
                    coin->mode++;
                    coin->time = 0;
                }
                break;

            case 1:
                MTXMultVec(*coin->dstMtx, &coin->dstPos, &coin->pos);
                coin->rot.y += 10;
                MTXMultVec(*coin->srcMtx, &coin->srcPos, &srcPos);
                MTXMultVec(*coin->dstMtx, &coin->dstPos, &dstPos);
                VECSubtract(&dstPos, &srcPos, &pos);
                VECNormalize(&pos, &pos);
                angle = HuAtan(pos.x, pos.z);
                if(frand() & 0x1) {
                    angle += MBCapsuleEffRandF()*30;
                } else {
                    angle -= MBCapsuleEffRandF()*30;
                }
                coin->vel.x = (HuSin(angle)*100)*0.2f;
                coin->vel.y = 20;
                coin->vel.z = (HuCos(angle)*100)*0.2f;
                coin->mode++;
                coin->time = 0;
                break;

            case 2:
                VECAdd(&coin->pos, &coin->vel, &coin->pos);
                coin->vel.y -= 3.266666715343794;
                coin->rot.y += 10;
                t = (++coin->time/30.0f);
                Hu3DModelTPLvlSet(coin->mdlId, 1-t);
                if(++coin->time >= 30.0f) {
                    coin->mode = 0;
                    coin->time = 0;
                    coinDisableF = TRUE;
                }
                break;
        }
        if(coin->mode <= 2) {
            for(j=0; j<GW_PLAYER_MAX; j++) {
                if(!MBPlayerAliveCheck(j)) {
                    continue;
                }
                MBPlayerPosGet(j, &playerPos);
                VECSubtract(&coin->pos, &playerPos, &pos);
                if(coin->pos.y >= playerPos.y && coin->pos.y <= playerPos.y+150 && HuMagXZVecF(&pos) < 100.0f) {
                    MBPlayerCoinAdd(j, 1);
                    MBAudFXPlay(MSM_SE_CMN_08);
                    coinGrabF = TRUE;
                }
            }
        }
        Hu3DModelPosSet(coin->mdlId, coin->pos.x, coin->pos.y, coin->pos.z);
        Hu3DModelRotSet(coin->mdlId, coin->rot.x, coin->rot.y, coin->rot.z);
        Hu3DModelScaleSet(coin->mdlId, coin->scale.x, coin->scale.y, coin->scale.z);
        if(coinGrabF || coinDisableF) {
            coin->enableF = FALSE;
            Hu3DModelDispOff(coin->mdlId);
            if(coinGrabF) {
                for(j=0, angle=0; j<32; j++) {
                    angle += (MBCapsuleEffRandF()+1)*22.5f;
                    effectPos = coin->pos;
                    t = (MBCapsuleEffRandF()+1)*2.5f;
                    angleX = 45-(MBCapsuleEffRandF()*90);
                    effectVel.x = HuSin(angle)*HuCos(angleX)*t;
                    effectVel.y = HuCos(angle)*HuCos(angleX)*t;
                    effectVel.z = t*HuSin(angleX);
                    intensity = MBCapsuleEffRandF();
                    color.r = 192+(intensity*63);
                    color.g = 192+(intensity*63);
                    color.b = 64+(intensity*63);
                    color.a = 192+(MBCapsuleEffRandF()*63);
                    MB1_CoinEffCreate(effectPos, effectVel,
                        ((MBCapsuleEffRandF()*0.1f)+0.5f)*100,
                        (MBCapsuleEffRandF()+1.0f)*(1.0f/60.0f),
                        0,
                        color);
                }
                break;
            }
        }
    }
}

void MB1_TrainCoinClose(void)
{
    mb1_CoinObj = NULL;
}

int MB1_TrainCoinNumGet(void)
{
    MB1_TRAIN_COIN *coin;
    int i;
    int num;
    OMOBJ *obj;
    obj = mb1_CoinObj;
    num = 0;
    if(mb1_CoinObj == NULL) {
        return 0;
    }
    for(coin=obj->data, i=0; i<MB1_TRAIN_COIN_MAX; i++, coin++) {
        if(coin->enableF) {
            num++;
        }
    }
    return num;
}

int MB1_TrainCoinCreate(Mtx *srcMtx, HuVecF *srcPos, Mtx *dstMtx, HuVecF *dstPos, float gravity, int maxTime)
{
    MB1_TRAIN_COIN *coin; //r31
    int i; //r30
    OMOBJ *obj; //r29
    int temp; //sp+0x18
    obj = mb1_CoinObj;
    temp =0;
    if(mb1_CoinObj == NULL) {
        return -1;
    }
    for(coin=obj->data, i=0; i<MB1_TRAIN_COIN_MAX; i++, coin++) {
        if(!coin->enableF) {
            break;
        }
    }
    if(i >= MB1_TRAIN_COIN_MAX) {
        return -1;
    }
    coin->enableF  = TRUE;
    coin->mode = 0;
    coin->gravity = gravity;
    coin->maxTime = maxTime;
    coin->time = 0;
    coin->srcPos = *srcPos;
    coin->dstPos = *dstPos;
    coin->srcMtx = srcMtx;
    coin->dstMtx = dstMtx;
    MTXMultVec(*coin->srcMtx, &coin->srcPos, &coin->pos);
    coin->rot.x = coin->rot.y = coin->rot.z = 0;
    coin->scale.x = coin->scale.y = coin->scale.z = 1;
    Hu3DModelPosSet(coin->mdlId, coin->pos.x, coin->pos.y, coin->pos.z);
    Hu3DModelRotSet(coin->mdlId, coin->rot.x, coin->rot.y, coin->rot.z);
    Hu3DModelScaleSet(coin->mdlId, coin->scale.x, coin->scale.y, coin->scale.z);
    Hu3DModelTPLvlSet(coin->mdlId, 1);
    Hu3DModelDispOn(coin->mdlId);
    return i;
}

int MB1_TrainCoinSearch(HuVecF *pos, HuVecF *out)
{
    MB1_TRAIN_COIN *coin; //r31
    int i; //r30
    OMOBJ *obj; //r29
    int bestCoin; //r28
    float dist; //f31
    float bestDist; //f30
    HuVecF dir;

    obj = mb1_CoinObj;
    if(mb1_CoinObj == NULL) {
        return -1;
    }
    for(coin=obj->data, bestCoin=-1, bestDist=10000, i=0; i<MB1_TRAIN_COIN_MAX; i++, coin++) {
        if(!coin->enableF) {
            continue;
        }
        VECSubtract(&coin->pos, pos, &dir);
        dist = VECMag(&dir);
        if(dist < bestDist) {
            bestCoin = i;
            bestDist = dist;
            *out = coin->pos;
        }
    }
    return bestCoin;
}

#define MB1_PARTICLE_ATTR_NONE 0
#define MB1_PARTICLE_ATTR_COUNTER_RESET (1 << 0)
#define MB1_PARTICLE_ATTR_COUNTER_UPDATE (1 << 1)

#define MB1_PARTICLE_DISPATTR_NONE 0
#define MB1_PARTICLE_DISPATTR_ZBUF_OFF (1 << 0)
#define MB1_PARTICLE_DISPATTR_NOANIM (1 << 1)
#define MB1_PARTICLE_DISPATTR_CAMERA_ROT (1 << 2)
#define MB1_PARTICLE_DISPATTR_ROT3D (1 << 3)
#define MB1_PARTICLE_DISPATTR_ALL 15

typedef struct MB1Particle_s MB1_PARTICLE;
typedef void (*MB1_PARTICLE_HOOK)(HU3DMODEL *modelP, MB1_PARTICLE *particleP, Mtx *matrix);

typedef struct MB1ParticleData_s {
    s16 time; //0x00
    s16 work; //0x02
    s16 mode; //0x04
    s16 cameraBit; //0x06
    HuVecF vel; //0x08
    float baseScale; //0x14
    float scaleFactor; //0x18
    float speed; //0x1C
    float unk20; //0x20
    float gravity; //0x24
    float unk28; //0x28
    float animTime; //0x2C
    float animSpeed; //0x30
    float scale; //0x34
    HuVecF rot; //0x38
    HuVecF pos; //0x44
    GXColor color; //0x50
    int animNo; //0x54
} MB1_PARTICLE_DATA;

struct MB1Particle_s {
    s16 mode; //0x00
    s16 time; //0x02
    HuVecF vel; //0x04
    s16 work[8]; //0x10
    u8 blendMode; //0x20
    u8 attr; //0x21
    u8 dispAttr; //0x22
    u8 unk23;
    HU3DMODELID modelId; //0x24
    s16 num; //0x26
    u32 count; //0x28
    u32 prevCounter; //0x2C
    u32 prevCount; //0x30
    u32 dlSize; //0x34
    ANIMDATA *anim; //0x38
    MB1_PARTICLE_DATA *data; //0x3C
    HuVecF *vertex; //0x40
    HuVec2f *st; //0x44
    void *dl; //0x48
    MB1_PARTICLE_HOOK hook; //0x4C
    HU3DMODEL *hookMdlP; //0x50
};

HU3DMODELID MB1_ParticleCreate(ANIMDATA *animP, s16 num);

void MB1_ExplodeEffUpdate(OMOBJ *obj);

typedef struct MB1ExplodeEffWork_s {
    int mdlId;
    int num;
    ANIMDATA *animP;
} MB1_EXPLODE_EFF_WORK;

void MB1_ExplodeEffInit(void)
{
    MB1_EXPLODE_EFF_WORK *work; //r31
    OMOBJ *obj; //r30
    MB1_PARTICLE *particleP; //r29
    HU3DMODEL *modelP; //r28
    ANIMDATA *animP; //r27
    int mdlId;

    obj = mb1_ExplodeEffObj = MBAddObj(32768, 0, 0, MB1_ExplodeEffUpdate);
    work = obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MB1_EXPLODE_EFF_WORK), HU_MEMNUM_OVL);
    memset(work, 0, sizeof(MB1_EXPLODE_EFF_WORK));
    work->animP = animP = HuSprAnimRead(HuDataReadNum(CAPSULE_ANM_effExplode, HU_MEMNUM_OVL));
    work->mdlId = mdlId = MB1_ParticleCreate(animP, 256);
    Hu3DModelLayerSet(work->mdlId, 3);
    work->num = 0;
    modelP = &Hu3DData[work->mdlId];
    particleP = modelP->hookData;
    particleP->blendMode = HU3D_PARTICLE_BLEND_NORMAL;
}

void MB1_ExplodeEffUpdate(OMOBJ *obj)
{
    MB1_PARTICLE_DATA *particleDataP; //r31
    MB1_EXPLODE_EFF_WORK *work; //r30
    MB1_PARTICLE *particleP; //r29
    int i; //r28
    HU3DMODEL *modelP; //r27
    work = obj->data;

    if(MBKillCheck() || mb1_ExplodeEffObj == NULL) {
        Hu3DModelKill(work->mdlId);
        work->mdlId = HU3D_MODELID_NONE;
        HuSprAnimKill(work->animP);
        work->animP = NULL;
        omDelObj(mbObjMan, obj);
        mb1_ExplodeEffObj = NULL;
        return;
    }
    if(work->num <= 0) {
        Hu3DModelDispOff(work->mdlId);
        return;
    }
    Hu3DModelDispOn(work->mdlId);
    modelP = &Hu3DData[work->mdlId];
    particleP = modelP->hookData;
    particleDataP = particleP->data;
    particleP->unk23 = 0;
    for(i=0; i<particleP->num; i++, particleDataP++) {
        if(particleDataP->scale <= 0.0f) {
            continue;
        }
        particleDataP->pos.x += particleDataP->vel.x;
        particleDataP->pos.y += particleDataP->vel.y;
        particleDataP->pos.z += particleDataP->vel.z;
        particleDataP->rot.z += particleDataP->speed;
        if(particleDataP->rot.z >= 360.0f) {
            particleDataP->rot.z -= 360.0f;
        }
        particleDataP->animNo = particleDataP->animTime;
        particleDataP->animTime += particleDataP->animSpeed;
        if(particleDataP->animNo >= 16) {
            particleDataP->animNo = 0;
            particleDataP->time = 0;
            particleDataP->scale = 0;
            work->num--;
        }
    }
}

void MB1_ExplodeEffClose(void)
{
    mb1_ExplodeEffObj = NULL;
}

int MB1_ExplodeEffNumGet(void)
{
    OMOBJ *obj;
    MB1_EXPLODE_EFF_WORK *work;
    obj = mb1_ExplodeEffObj;
    if(mb1_ExplodeEffObj == NULL) {
        return 0;
    } else {
        work = obj->data;
        return work->num;
    }
}

static int MB1_ExplodeEffCreate(HuVecF pos, HuVecF vel, float scale, float speed, float animSpeed, GXColor color)
{
    MB1_PARTICLE_DATA *particleDataP; //r31
    MB1_PARTICLE *particleP; //r30
    MB1_EXPLODE_EFF_WORK *work; //r28
    int i; //r29

    OMOBJ *obj; //r27
    HU3DMODEL *modelP; //r26

    obj = mb1_ExplodeEffObj;
    work = obj->data;
    modelP = &Hu3DData[work->mdlId];
    particleP = modelP->hookData;
    for(particleDataP=particleP->data, i=0; i<particleP->num; i++, particleDataP++) {
        if(particleDataP->scale <= 0.0f) {
            break;
        }
    }
    if(i >= particleP->num) {
        return -1;
    }
    particleDataP->time = particleDataP->work = 0;
    particleDataP->pos.x = pos.x;
    particleDataP->pos.y = pos.y;
    particleDataP->pos.z = pos.z;
    particleDataP->vel.x = vel.x;
    particleDataP->vel.y = vel.y;
    particleDataP->vel.z = vel.z;
    particleDataP->speed = speed;
    particleDataP->scale = scale;
    particleDataP->color = color;
    particleDataP->rot.z = 0;
    particleDataP->animNo = 0;
    particleDataP->time = 0;
    particleDataP->animTime = 0;
    particleDataP->animSpeed = animSpeed;
    work->num++;
    return i;
}

typedef struct MB1CoinEffWork_s {
    int mdlId;
    int num;
    ANIMDATA *animP;
} MB1_COIN_EFF_WORK;

void MB1_CoinEffUpdate(OMOBJ *obj);

void MB1_CoinEffInit(void)
{
    MB1_COIN_EFF_WORK *work; //r31
    OMOBJ *obj; //r30
    MB1_PARTICLE *particleP; //r29
    HU3DMODEL *modelP; //r28
    ANIMDATA *animP; //r27
    int mdlId;

    obj = mb1_CoinEffObj = MBAddObj(32768, 0, 0, MB1_CoinEffUpdate);
    work = obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MB1_COIN_EFF_WORK), HU_MEMNUM_OVL);
    memset(work, 0, sizeof(MB1_COIN_EFF_WORK));
    work->animP = animP = HuSprAnimRead(HuDataReadNum(CAPSULE_ANM_effCoin, HU_MEMNUM_OVL));
    work->mdlId = mdlId = MB1_ParticleCreate(animP, 256);
    Hu3DModelLayerSet(work->mdlId, 3);
    work->num = 0;
    modelP = &Hu3DData[work->mdlId];
    particleP = modelP->hookData;
    particleP->blendMode = HU3D_PARTICLE_BLEND_ADDCOL;
    particleP->dispAttr = MB1_PARTICLE_DISPATTR_NOANIM;
}

void MB1_CoinEffUpdate(OMOBJ *obj)
{
    MB1_PARTICLE_DATA *particleDataP; //r31
    MB1_COIN_EFF_WORK *work; //r30
    MB1_PARTICLE *particleP; //r29
    int i; //r28
    HU3DMODEL *modelP; //r27
    work = obj->data;

    if(MBKillCheck() || mb1_CoinEffObj == NULL) {
        Hu3DModelKill(work->mdlId);
        work->mdlId = HU3D_MODELID_NONE;
        HuSprAnimKill(work->animP);
        work->animP = NULL;
        omDelObj(mbObjMan, obj);
        mb1_CoinEffObj = NULL;
        return;
    }
    if(work->num <= 0) {
        Hu3DModelDispOff(work->mdlId);
        return;
    }
    Hu3DModelDispOn(work->mdlId);
    modelP = &Hu3DData[work->mdlId];
    particleP = modelP->hookData;
    particleDataP = particleP->data;
    particleP->unk23 = 0;
    for(i=0; i<particleP->num; i++, particleDataP++) {
        if(particleDataP->scale <= 0.0f) {
            continue;
        }
        particleDataP->pos.x += particleDataP->vel.x;
        particleDataP->pos.y += particleDataP->vel.y;
        particleDataP->pos.z += particleDataP->vel.z;
        if(particleDataP->gravity) {
            particleDataP->vel.y -= particleDataP->gravity;
        }
        particleDataP->scale = particleDataP->baseScale*particleDataP->scaleFactor;
        particleDataP->scaleFactor -= particleDataP->speed;
        if(particleDataP->scaleFactor <= 0.0f) {
            particleDataP->animNo = 0;
            particleDataP->time = 0;
            particleDataP->scale = 0;
            work->num--;
        }
    }
}

void MB1_CoinEffClose(void)
{
    mb1_CoinEffObj = NULL;
}

int MB1_CoinEffNumGet(void)
{
    OMOBJ *obj;
    MB1_COIN_EFF_WORK *work;
    obj = mb1_CoinEffObj;
    if(mb1_CoinEffObj == NULL) {
        return 0;
    } else {
        work = obj->data;
        return work->num;
    }
}

void MB1_CoinEffBlendModeSet(int blendMode)
{
    OMOBJ *obj;
    MB1_COIN_EFF_WORK *work;
    MB1_PARTICLE *particleP;
    HU3DMODEL *modelP;
    obj = mb1_CoinEffObj;
    if(mb1_CoinEffObj == NULL) {
        return;
    } else {
        work = obj->data;
        modelP = &Hu3DData[work->mdlId];
        particleP = modelP->hookData;
        particleP->blendMode = blendMode;
    }
}

int MB1_CoinEffCreate(HuVecF pos, HuVecF vel, float scale, float speed, float gravity, GXColor color)
{
    MB1_PARTICLE_DATA *particleDataP; //r31
    MB1_PARTICLE *particleP; //r30
    int i; //r29
    MB1_COIN_EFF_WORK *work; //r26
    OMOBJ *obj; //r25
    HU3DMODEL *modelP; //r24
    if(mb1_CoinEffObj == NULL){
        return -1;
    }
    obj = mb1_CoinEffObj;
    work = obj->data;
    modelP = &Hu3DData[work->mdlId];
    particleP = modelP->hookData;
    for(particleDataP=particleP->data, i=0; i<particleP->num; i++, particleDataP++) {
        if(particleDataP->scale <= 0.0f) {
            break;
        }
    }
    if(i >= particleP->num) {
        return -1;
    }
    particleDataP->time = particleDataP->work = 0;
    particleDataP->pos.x = pos.x;
    particleDataP->pos.y = pos.y;
    particleDataP->pos.z = pos.z;
    particleDataP->vel.x = vel.x;
    particleDataP->vel.y = vel.y;
    particleDataP->vel.z = vel.z;

    particleDataP->baseScale = scale;
    particleDataP->scaleFactor = 1;
    particleDataP->speed = speed;
    particleDataP->gravity = gravity;
    particleDataP->scale = scale;
    particleDataP->color = color;
    particleDataP->rot.z = MBCapsuleEffRandF()*360;
    particleDataP->animNo = 0;
    particleDataP->time = 0;

    work->num++;
    return i;
}

static void ParticleDraw(HU3DMODEL *modelP, Mtx *mtx);

HU3DMODELID MB1_ParticleCreate(ANIMDATA *animP, s16 num)
{
    MB1_PARTICLE *effP;
    MB1_PARTICLE_DATA *effDataP;
    s16 i;
    HuVec2f *st;
    HU3DMODEL *modelP;
    HuVecF *vtx;
    HU3DMODELID modelId;
    void *dlBuf;
    void *dlBegin;
    modelId = Hu3DHookFuncCreate(ParticleDraw);
    Hu3DModelCameraSet(modelId, HU3D_CAM0);
    modelP = &Hu3DData[modelId];
    modelP->hookData = effP = HuMemDirectMallocNum(HEAP_MODEL, sizeof(MB1_PARTICLE), modelP->mallocNo);
    effP->anim = animP;
    effP->num = num;
    effP->blendMode = HU3D_PARTICLE_BLEND_NORMAL;
    effP->dispAttr = MB1_PARTICLE_DISPATTR_NONE;
    effP->hook = NULL;
    effP->hookMdlP = NULL;
    effP->count = 0;
    effP->attr = MB1_PARTICLE_ATTR_NONE;
    effP->unk23 = 0;
    effP->prevCount = 0;
    effP->mode = effP->time = 0;
    effP->data = effDataP = HuMemDirectMallocNum(HEAP_MODEL, num*sizeof(MB1_PARTICLE_DATA), modelP->mallocNo);
    memset(effDataP, 0, num*sizeof(MB1_PARTICLE_DATA));
    for(i=0; i<num; i++, effDataP++) {
        effDataP->scale = 0;
        effDataP->rot.x = effDataP->rot.y = effDataP->rot.z = 0;
        effDataP->animTime = 0;
        effDataP->animSpeed = 1;
        effDataP->pos.x = ((frand() & 0x7F)-64)*20;
        effDataP->pos.y = ((frand() & 0x7F)-64)*30;
        effDataP->pos.z = ((frand() & 0x7F)-64)*20;
        effDataP->color.r = effDataP->color.g = effDataP->color.b = effDataP->color.a = 255;
        effDataP->animNo = 0;
    }
    effP->vertex = vtx = HuMemDirectMallocNum(HEAP_MODEL, num*sizeof(HuVecF)*4, modelP->mallocNo);
    for(i=0; i<num*4; i++, vtx++) {
        vtx->x = vtx->y = vtx->z = 0;
    }
    effP->st = st = HuMemDirectMallocNum(HEAP_MODEL, num*sizeof(HuVec2f)*4, modelP->mallocNo);
    for(i=0; i<num; i++) {
        st->x = 0;
        st->y = 0;
        st++;

        st->x = 1;
        st->y = 0;
        st++;

        st->x = 1;
        st->y = 1;
        st++;

        st->x = 0;
        st->y = 1;
        st++;
    }
    dlBegin = dlBuf = HuMemDirectMallocNum(HEAP_MODEL, 0x10000, modelP->mallocNo);
    DCFlushRange(dlBuf, 0x10000);
    GXBeginDisplayList(dlBegin, 0x10000);
    GXBegin(GX_QUADS, GX_VTXFMT0, num*4);
    for(i=0; i<num; i++) {
        GXPosition1x16(i*4);
        GXColor1x16(i);
        GXTexCoord1x16(i*4);

        GXPosition1x16((i*4)+1);
        GXColor1x16(i);
        GXTexCoord1x16((i*4)+1);

        GXPosition1x16((i*4)+2);
        GXColor1x16(i);
        GXTexCoord1x16((i*4)+2);

        GXPosition1x16((i*4)+3);
        GXColor1x16(i);
        GXTexCoord1x16((i*4)+3);
    }
    GXEnd();
    effP->dlSize = GXEndDisplayList();
    effP->dl = HuMemDirectMallocNum(HEAP_MODEL, effP->dlSize, modelP->mallocNo);
    memcpy(effP->dl, dlBuf, effP->dlSize);
    DCFlushRange(effP->dl, effP->dlSize);
    HuMemDirectFree(dlBuf);
    return modelId;
}

HU3DMODELID MB1_ParticleCopy(HU3DMODELID oldMdlId)
{
    MB1_PARTICLE *newParticleP; //r31
    MB1_PARTICLE_DATA *particleDataP; //r30
    HuVec2f *st; //r29
    s16 i; //r28
    HU3DMODEL *newModelP; //r27
    MB1_PARTICLE *oldParticleP; //r26
    HuVecF *pos; //r25
    int num; //r24
    HU3DMODEL *oldModelP; //r23
    HU3DMODELID newMdlId; //r22

    oldModelP = &Hu3DData[oldMdlId];
    oldParticleP = oldModelP->hookData;
    newMdlId = Hu3DHookFuncCreate(oldModelP->hookFunc);
    Hu3DModelCameraSet(newMdlId, HU3D_CAM0);
    newModelP = &Hu3DData[newMdlId];
    newModelP->hookData = newParticleP = HuMemDirectMallocNum(HEAP_MODEL, sizeof(MB1_PARTICLE), newModelP->mallocNo);
    newParticleP->anim = oldParticleP->anim;
    newParticleP->num = oldParticleP->num;
    num = newParticleP->num;
    newParticleP->blendMode = HU3D_PARTICLE_BLEND_NORMAL;
    newParticleP->dispAttr = oldParticleP->dispAttr;
    newParticleP->hook = NULL;
    newParticleP->hookMdlP = NULL;
    newParticleP->count = 0;
    newParticleP->attr = MB1_PARTICLE_ATTR_NONE;
    newParticleP->prevCount = 0;
    newParticleP->mode = newParticleP->time = 0;
    newParticleP->data = particleDataP = HuMemDirectMallocNum(HEAP_MODEL, sizeof(MB1_PARTICLE_DATA)*num, newModelP->mallocNo);
    for(i=0; i<num; i++, particleDataP++) {
        particleDataP->scale = 0;
        particleDataP->rot.x = particleDataP->rot.y = particleDataP->rot.z = 0;
        particleDataP->animTime = 0;
        particleDataP->animSpeed = 1;
        particleDataP->pos.x = ((frand()&0x7F)-64)*20;
        particleDataP->pos.y = ((frand()&0x7F)-64)*30;
        particleDataP->pos.z = ((frand()&0x7F)-64)*20;
        particleDataP->color.r = particleDataP->color.g = particleDataP->color.b = particleDataP->color.a = 255;
        particleDataP->animNo = 0;
    }
    newParticleP->vertex = pos = HuMemDirectMallocNum(HEAP_MODEL, sizeof(HuVecF)*num*4, newModelP->mallocNo);
    for(i=0; i<num*4; i++, pos++) {
        pos->x = pos->y = pos->z = 0;
    }
    newParticleP->st = st = HuMemDirectMallocNum(HEAP_MODEL, sizeof(HuVec2f)*num*4, newModelP->mallocNo);
    for(i=0; i<num; i++) {
        st->x = 0;
        st->y = 0;
        st++;
        st->x = 1;
        st->y = 0;
        st++;
        st->x = 1;
        st->y = 1;
        st++;
        st->x = 0;
        st->y = 1;
        st++;
    }
    newParticleP->dlSize = oldParticleP->dlSize;
    newParticleP->dl = oldParticleP->dl;
    return newMdlId;
}

static void ParticleDraw(HU3DMODEL *modelP, Mtx *mtx)
{
    MB1_PARTICLE *effP;
    MB1_PARTICLE_DATA *effDataP;
    HuVecF *vtx;
    HuVec2f *st;
    HuVecF *scaleVtxP;
    s16 i;
    s16 j;
    HuVecF *initVtxP;
    s16 bmpFmt;
    s16 row;
    s16 col;
    MB1_PARTICLE_HOOK hook;

    Mtx mtxInv;
    Mtx mtxPos;
    Mtx mtxRotZ;
    HuVecF scaleVtx[4];
    HuVecF finalVtx[4];
    HuVecF initVtx[4];
    ROMtx basePosMtx;

    static HuVecF vtxTbl[] = {
        { -0.5f,  0.5f, 0.0f },
        {  0.5f,  0.5f, 0.0f },
        {  0.5f, -0.5f, 0.0f },
        { -0.5f, -0.5f, 0.0f }
    };

    static HuVec2f uvTbl[] = {
        { 0.0f, 0.0f },
        { 0.25f, 0.0f },
        { 0.25f, 0.25f },
        { 0.0f, 0.25f },
    };

    effP = modelP->hookData;
    if(!shadowModelDrawF) {
        if(effP->hookMdlP && effP->hookMdlP != modelP){
            ParticleDraw(effP->hookMdlP, mtx);
        }
        GXLoadPosMtxImm(*mtx, GX_PNMTX0);
        GXSetNumTevStages(1);
        GXSetNumTexGens(1);
        GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        if(shadowModelDrawF) {
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ONE, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
            GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
        } else {
            bmpFmt = (effP->anim->bmp->dataFmt & 0xF);
            if(bmpFmt == ANIM_BMP_I8 || bmpFmt == ANIM_BMP_I4) {
                GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ONE, GX_CC_RASC, GX_CC_ZERO);
            } else {
                GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC, GX_CC_ZERO);
            }
            if(effP->dispAttr & MB1_PARTICLE_DISPATTR_ZBUF_OFF) {
                GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
            } else if(modelP->attr & HU3D_ATTR_ZWRITE_OFF) {
                GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
            } else {
                GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
            }
        }
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
        HuSprTexLoad(effP->anim, 0, GX_TEXMAP0, GX_REPEAT, GX_REPEAT, GX_LINEAR);
        GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
        GXSetZCompLoc(GX_FALSE);
        switch (effP->blendMode) {
            case HU3D_PARTICLE_BLEND_NORMAL:
                GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
                break;
            case HU3D_PARTICLE_BLEND_ADDCOL:
                GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_NOOP);
                break;
            case HU3D_PARTICLE_BLEND_INVCOL:
                GXSetBlendMode(GX_BM_BLEND, GX_BL_ZERO, GX_BL_INVSRCALPHA, GX_LO_NOOP);
                break;
        }
        if(HmfInverseMtxF3X3(*mtx, mtxInv) == FALSE) {
            PSMTXIdentity(mtxInv);
        }
        PSMTXReorder(mtxInv, basePosMtx);
        if(effP->hook) {
            hook = effP->hook;
            hook(modelP, effP, mtx);
        }
        effDataP = effP->data;
        vtx = effP->vertex;
        st = effP->st;
        if(effP->dispAttr & MB1_PARTICLE_DISPATTR_CAMERA_ROT) {
            MTXIdentity(mtxInv);
            MTXIdentity(*(Mtx *)(&basePosMtx));
            initVtx[0] = vtxTbl[0];
            initVtx[1] = vtxTbl[1];
            initVtx[2] = vtxTbl[2];
            initVtx[3] = vtxTbl[3];
        } else {
            PSMTXROMultVecArray(basePosMtx, &vtxTbl[0], initVtx, 4);
        }
        for(i=0; i<effP->num; i++, effDataP++) {
            if(!effDataP->scale) {
                vtx->x = vtx->y = vtx->z = 0;
                vtx++;
                vtx->x = vtx->y = vtx->z = 0;
                vtx++;
                vtx->x = vtx->y = vtx->z = 0;
                vtx++;
                vtx->x = vtx->y = vtx->z = 0;
                vtx++;
            } else if(effP->dispAttr & MB1_PARTICLE_DISPATTR_ROT3D) {
                VECScale(&vtxTbl[0], &scaleVtx[0], effDataP->scale);
                VECScale(&vtxTbl[1], &scaleVtx[1], effDataP->scale);
                VECScale(&vtxTbl[2], &scaleVtx[2], effDataP->scale);
                VECScale(&vtxTbl[3], &scaleVtx[3], effDataP->scale);
                mtxRot(mtxPos, effDataP->rot.x, effDataP->rot.y, effDataP->rot.z);
                PSMTXMultVecArray(mtxPos, scaleVtx, finalVtx, 4);
                VECAdd(&finalVtx[0], &effDataP->pos, vtx++);
                VECAdd(&finalVtx[1], &effDataP->pos, vtx++);
                VECAdd(&finalVtx[2], &effDataP->pos, vtx++);
                VECAdd(&finalVtx[3], &effDataP->pos, vtx++);
            } else if(!effDataP->rot.z) {
                scaleVtxP = scaleVtx;
                initVtxP = initVtx;
                VECScale(initVtxP++, scaleVtxP, effDataP->scale);
                VECAdd(scaleVtxP++, &effDataP->pos, vtx++);
                VECScale(initVtxP++, scaleVtxP, effDataP->scale);
                VECAdd(scaleVtxP++, &effDataP->pos, vtx++);
                VECScale(initVtxP++, scaleVtxP, effDataP->scale);
                VECAdd(scaleVtxP++, &effDataP->pos, vtx++);
                VECScale(initVtxP++, scaleVtxP, effDataP->scale);
                VECAdd(scaleVtxP++, &effDataP->pos, vtx++);
            } else {
                VECScale(&vtxTbl[0], &scaleVtx[0], effDataP->scale);
                VECScale(&vtxTbl[1], &scaleVtx[1], effDataP->scale);
                VECScale(&vtxTbl[2], &scaleVtx[2], effDataP->scale);
                VECScale(&vtxTbl[3], &scaleVtx[3], effDataP->scale);
                MTXRotRad(mtxRotZ, 'Z', effDataP->rot.z);
                PSMTXConcat(mtxInv, mtxRotZ, mtxPos);
                PSMTXMultVecArray(mtxPos, scaleVtx, finalVtx, 4);
                VECAdd(&finalVtx[0], &effDataP->pos, vtx++);
                VECAdd(&finalVtx[1], &effDataP->pos, vtx++);
                VECAdd(&finalVtx[2], &effDataP->pos, vtx++);
                VECAdd(&finalVtx[3], &effDataP->pos, vtx++);

            }
        }
        effDataP = effP->data;
        st = effP->st;
        if(!(effP->dispAttr & MB1_PARTICLE_DISPATTR_NOANIM)) {
            for(i=0; i<effP->num; i++, effDataP++) {
                row = effDataP->animNo & 0x3;
                col = (effDataP->animNo >> 2) & 0x3;
                for(j=0; j<4; j++, st++) {
                    st->x = (0.25f*row)+uvTbl[j].x;
                    st->y = (0.25f*col)+uvTbl[j].y;
                }
            }
        } else {
            for(i=0; i<effP->num; i++, effDataP++) {
                for(j=0; j<4; j++, st++) {
                    st->x = 4*uvTbl[j].x;
                    st->y = 4*uvTbl[j].y;
                }
            }
        }
        DCFlushRangeNoSync(effP->vertex, effP->num*sizeof(HuVecF)*4);
        DCFlushRangeNoSync(effP->st, effP->num*sizeof(HuVec2f)*4);
        DCFlushRangeNoSync(effP->data, effP->num*sizeof(MB1_PARTICLE_DATA));
        PPCSync();
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXSETARRAY(GX_VA_POS, effP->vertex, effP->num*sizeof(HuVecF)*4, sizeof(HuVecF), TRUE);
        GXSetVtxDesc(GX_VA_CLR0, GX_INDEX16);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
        GXSETARRAY(GX_VA_CLR0, &effP->data->color, effP->num*sizeof(MB1_PARTICLE_DATA), sizeof(MB1_PARTICLE_DATA), TRUE);
        GXSetVtxDesc(GX_VA_TEX0, GX_INDEX16);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
        GXSETARRAY(GX_VA_TEX0, effP->st, effP->num*sizeof(HuVec2f)*4, sizeof(HuVec2f), TRUE);
        GXCallDisplayList(effP->dl, effP->dlSize);
        if(shadowModelDrawF == FALSE) {
            if(!(effP->attr & MB1_PARTICLE_ATTR_COUNTER_UPDATE)) {
                effP->count++;
            }
            if(effP->prevCount != 0 && effP->prevCount <= effP->count) {
                if(effP->attr & MB1_PARTICLE_ATTR_COUNTER_RESET) {
                    effP->count = 0;
                }
                effP->count = effP->prevCount;
            }
        }
    }
}

typedef struct MB1TrainInfo_s {
    int no;
    char *objName;
} MB1_TRAIN_INFO;

void MB1_TrainInit(void)
{
    int i;//r31
    MB1_TRAIN_POINT *trainPoint; //r30
    int indexA; //r29
    int indexD; //r28
    HuVecF *vtxBuf; //r27
    int indexC; //r26
    int j; //r25
    HU3DMODEL *modelP; //r24
    HuVecF *vtx; //r23
    int indexB; //r22
    int mdlId; //r21

    float t; //f28

    HuVecF pointA; //sp+0x40
    HuVecF pointB; //sp+0x34
    HuVecF pointC; //sp+0x28
    HuVecF pointD; //sp+0x1C
    HuVecF temp; //sp+0x10
    HSFDATA *hsf; //sp+0xC

    static MB1_TRAIN_INFO trainObjTbl[7] = {
        { 0, "train_a2" },
        { 0, "train_a3" },
        { 0, "train_a4" },
        { 1, "train_b2" },
        { 1, "train_b3" },
        { 2, "train_c2" },
        { 2, "train_c3" },
    };
    mb1_Work.trainPointNum = MB1EV_TRAINPATH_LEN*20;
    mb1_Work.trainMaxTime = 0;
    trainPoint = mb1_Work.trainPoint = HuMemDirectMallocNum(HEAP_HEAP, mb1_Work.trainPointNum*sizeof(MB1_TRAIN_POINT), HU_MEMNUM_OVL);
    memset(trainPoint, 0, mb1_Work.trainPointNum*sizeof(MB1_TRAIN_POINT));
    vtx = vtxBuf = HuMemDirectMallocNum(HEAP_HEAP, mb1_Work.trainPointNum*sizeof(HuVecF), HU_MEMNUM_OVL);
    memset(vtxBuf, 0, mb1_Work.trainPointNum*sizeof(HuVecF));
    for(i=0; i<MB1EV_TRAINPATH_LEN; i++) {
        indexA = i-1;
        if(indexA < 0) {
            indexA += MB1EV_TRAINPATH_LEN;
        }
        indexB = i;
        indexC = i+1;
        if(indexC >= MB1EV_TRAINPATH_LEN) {
            indexC -= MB1EV_TRAINPATH_LEN;
        }
        indexD = i+2;
        if(indexD >= MB1EV_TRAINPATH_LEN) {
            indexD -= MB1EV_TRAINPATH_LEN;
        }
        pointA.x = mb1ev_TrainPath[indexA].x;
        pointA.y = mb1ev_TrainPath[indexA].y;
        pointA.z = mb1ev_TrainPath[indexA].z;
        pointB.x = mb1ev_TrainPath[indexB].x;
        pointB.y = mb1ev_TrainPath[indexB].y;
        pointB.z = mb1ev_TrainPath[indexB].z;
        pointC.x = mb1ev_TrainPath[indexC].x;
        pointC.y = mb1ev_TrainPath[indexC].y;
        pointC.z = mb1ev_TrainPath[indexC].z;
        pointD.x = mb1ev_TrainPath[indexD].x;
        pointD.y = mb1ev_TrainPath[indexD].y;
        pointD.z = mb1ev_TrainPath[indexD].z;
        for(j=0; j<20; vtx++, j++) {
            t = j/20.0f;
            MB1_HermiteCalcV(t, &pointA, &pointB, &pointC, &pointD, vtx);
        }
    }
    for(i=0; i<mb1_Work.trainPointNum; i++, trainPoint++) {
        if(i < mb1_Work.trainPointNum-1) {
            indexC = i+1;
        } else {
            indexC = 0;
        }
        pointB = vtxBuf[i];
        pointC = vtxBuf[indexC];
        VECSubtract(&pointC, &pointB, &temp);
        indexA = i-1;
        if(indexA < 0) {
            indexA += mb1_Work.trainPointNum;
        }
        pointA = vtxBuf[indexA];
        indexD = i+2;
        if(indexD >= mb1_Work.trainPointNum) {
            indexD -= mb1_Work.trainPointNum;
        }
        pointD = vtxBuf[indexD];
        trainPoint->idx = i;
        trainPoint->nextIdx = indexC;
        trainPoint->endTime = MB1_PathTimeGetEnd(1, &pointA, &pointB, &pointC, &pointD);
        trainPoint->startTime = mb1_Work.trainMaxTime;
        trainPoint->pos = pointA;
        trainPoint->rot.x = HuAtan(-temp.y, HuMagXZVecF(&temp));
        trainPoint->rot.y  = HuAtan(temp.x, temp.z);
        mb1_Work.trainMaxTime += trainPoint->endTime;
    }
    HuMemDirectFree(vtxBuf);
    mb1_Work.trainDelay = ((MBCapsuleEffRandF()*0.4f)+1)*60;
    mb1_Work.trainMode = 1;
    mb1_Work.trainTime = 0;
    mb1_Work.trainSpeed = mb1_Work.trainMaxTime/1800;
    mb1_Work.trainMaxSpeed = mb1_Work.trainSpeed;
    mb1_Work.coinSpeed = 7.5f;
    for(i=0; i<7;i++) {
        mdlId = MBModelIdGet(mb1_Work.trainMdlId[trainObjTbl[i].no]);
        modelP = &Hu3DData[mdlId];
        hsf = modelP->hsf;
        mb1_Work.trainObj[i] = MB1_ModelObjPtrGet(modelP, trainObjTbl[i].objName);
    }
}

int MB1_TrainPosGet(float time, HuVecF *pos, HuVecF *rot);

void MB1_TrainUpdate(void)
{
    int i; //r30
    HSFOBJECT *obj; //r28

    float intensity; //f31
    float t; //f30
    float angle; //f29

    float cRotX; //f28
    float sRotX; //f27
    float sRotY; //f26
    float cRotY; //f25

    Mtx trainMtx; //sp+0x5C
    HuVecF trainPos; //sp+0x50
    HuVecF trainRot; //sp+0x44
    HuVecF pos; //sp+0x38
    HuVecF vel; //sp+0x2C
    GXColor color; //sp+0x10
    int posIdx; //sp+0xC

    mb1_Work.trainTime += mb1_Work.trainSpeed;
    if(mb1_Work.trainTime > mb1_Work.trainMaxTime) {
        mb1_Work.trainTime -= mb1_Work.trainMaxTime;
    }
    posIdx = MB1_TrainPosGet(mb1_Work.trainTime, &trainPos, &trainRot);
    MBModelPosSetV(mb1_Work.trainMdlId[0], &trainPos);
    MBModelRotSetV(mb1_Work.trainMdlId[0], &trainRot);
    mb1_Work.trainRot[0] = trainRot;
    MB1_TrainPosGet(mb1_Work.trainTime-350, &trainPos, &trainRot);
    MBModelPosSetV(mb1_Work.trainMdlId[1], &trainPos);
    MBModelRotSetV(mb1_Work.trainMdlId[1], &trainRot);
    mb1_Work.trainRot[1] = trainRot;
    MB1_TrainPosGet(mb1_Work.trainTime-650, &trainPos, &trainRot);
    MBModelPosSetV(mb1_Work.trainMdlId[2], &trainPos);
    MBModelRotSetV(mb1_Work.trainMdlId[2], &trainRot);
    mb1_Work.trainRot[2] = trainRot;
    for(i=0; i<7; i++) {
        obj = mb1_Work.trainObj[i];
        if(obj != NULL) {
            if((obj->mesh.base.rot.x += mb1_Work.coinSpeed) >= 360) {
                obj->mesh.base.rot.x -= 360;
            }
        }
    }
    if(--mb1_Work.trainDelay <= 0 && mb1_Work.trainMode != 0) {
        Hu3DModelObjMtxGet(MBModelIdGet(mb1_Work.trainMdlId[0]), "coin_pos", trainMtx);
        switch(mb1_Work.trainMode) {
            case 1:
                mb1_Work.trainDelay = ((MBCapsuleEffRandF()*0.4f)+1.0f)*60;
                break;

            case 2:
                mb1_Work.trainDelay = ((MBCapsuleEffRandF()*0.2f)+0.5f)*60;
                break;
        }
        pos.x = trainMtx[0][3];
        pos.y = trainMtx[1][3];
        pos.z = trainMtx[2][3];
        trainMtx[0][3] = 0;
        trainMtx[1][3] = 0;
        trainMtx[2][3] = 0;
        sRotX = HuSin(-mb1_Work.trainRot[0].x);
        cRotX = HuCos(-mb1_Work.trainRot[0].x);
        sRotY = HuSin(mb1_Work.trainRot[0].y);
        cRotY = HuCos(mb1_Work.trainRot[0].y);
        t = mb1_Work.trainSpeed/mb1_Work.trainMaxSpeed;
        for(i=0; i<16; i++) {
            trainPos.x = pos.x;
            trainPos.y = pos.y;
            trainPos.z = pos.z;
            angle = i*22.5f;
            vel.x = ((HuSin(angle)*100)*0.022f)*((MBCapsuleEffRandF()*0.1f)+0.9f);
            vel.y = 7.5000005f*((MBCapsuleEffRandF()*0.1f)+0.9f);
            vel.z = ((HuCos(angle)*100)*0.022f)*((MBCapsuleEffRandF()*0.1f)+0.9f);
            MTXMultVec(trainMtx, &vel, &vel);
            vel.x += t*(10*(sRotY*cRotX));
            vel.y += (sRotX*10)*t;
            vel.z += t*(10*(cRotY*cRotX));
            intensity = MBCapsuleEffRandF()*63;
            color.r = intensity+192.0f;
            color.g = intensity+192.0f;
            color.b = intensity+192.0f;
            color.a = (MBCapsuleEffRandF()*63)+192.0f;
            MB1_ExplodeEffCreate(trainPos, vel, 100, 0.1f, 0.33f, color);
        }
    }
}

void MB1_TrainClose(void)
{
    if(mb1_Work.trainPoint != NULL) {
        HuMemDirectFree(mb1_Work.trainPoint);
    }
    mb1_Work.trainPoint = NULL;
}

int MB1_TrainPosGet(float time, HuVecF *pos, HuVecF *rot)
{
    MB1_TRAIN_POINT *point; //r31
    int i; //r30
    MB1_TRAIN_POINT *nextPoint; //r28
    int idxPrev; //r27
    MB1_TRAIN_POINT *nextPoint2; //r26
    MB1_TRAIN_POINT *prevPoint; //r25
    int idxNext2; //r24


    float t; //f27

    HuVecF posCur; //sp+0x24
    HuVecF posNext; //sp+0x18
    HuVecF facePos; //sp+0xC

    point = mb1_Work.trainPoint;
    if(fabs(time) >= mb1_Work.trainMaxTime) {
        time = fmod(time, mb1_Work.trainMaxTime);
    }
    if(time < 0) {
        time += mb1_Work.trainMaxTime;
    }
    for(i=0; i<mb1_Work.trainPointNum; i++, point++) {
        if(time >= point->startTime && time < point->startTime+point->endTime) {
            break;
        }
    }
    if(i >= mb1_Work.trainPointNum) {
        point = mb1_Work.trainPoint;
        time = 0;
    }
    nextPoint = &mb1_Work.trainPoint[point->nextIdx];
    t = (time-point->startTime)/point->endTime;
    posCur = point->pos;
    posNext = nextPoint->pos;
    idxNext2 = nextPoint->nextIdx;
    if(i <= 0) {
        idxPrev = mb1_Work.trainPointNum-1;
    } else {
        idxPrev = i-1;
    }
    nextPoint2 = &mb1_Work.trainPoint[idxNext2];
    prevPoint = &mb1_Work.trainPoint[idxPrev];
    MB1_HermiteCalcV(t, &prevPoint->pos, &point->pos, &nextPoint->pos, &nextPoint2->pos, pos);
    if(rot != NULL) {
        MB1_TrainPosGet(time+0.1f, &facePos, NULL);
        VECSubtract(&facePos, pos, &facePos);
        rot->x = HuAtan(-facePos.y, HuMagXZVecF(&facePos));
        rot->y = HuAtan(facePos.x, facePos.z);
        rot->z = 0;
    }
    return i;
}

void MB1_TrainTimeSet(float time)
{
    mb1_Work.trainTime = mb1_Work.trainMaxTime*time;
}

float MB1_TrainTimeGet(void)
{
    return mb1_Work.trainTime/mb1_Work.trainMaxTime;
}

void MB1_TrainSpeedSet(float speed)
{
    mb1_Work.trainSpeed = mb1_Work.trainMaxSpeed*speed;
}

void MB1_TrainCoinSpeedSet(float speed)
{
    mb1_Work.coinSpeed = 7.5f*speed;
}

void MB1_HasiOpen(int no)
{
    if(mb1_Work.hasiMode[no] == 0) {
        MBMotionSpeedSet(mb1_Work.hasiMdlId[no], 1);
        mb1_Work.hasiMode[no]++;
    }
}

void MB1_HasiClose(int no)
{
    if(mb1_Work.hasiMode[no] == 2) {
        MBMotionSpeedSet(mb1_Work.hasiMdlId[no], 1);
        mb1_Work.hasiMode[no]++;
    } else {
        mb1_Work.hasiSeF[no] = TRUE;
    }
}
