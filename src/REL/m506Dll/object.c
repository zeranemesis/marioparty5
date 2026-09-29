#include "REL/m506Dll.h"
#include "game/esprite.h"
#include "game/mg/score.h"
#include "game/frand.h"
#include "game/charman.h"
#include "dolphin/mtx.h"

#define M506_ANM_cloud_num M506_ANM_cloud

typedef struct M506FlyObj_s {
    HuVecF pos;     //0x00
    HuVecF rot;     //0x0C
    f32 speed;      //0x18
    s32 state;      //0x1C
    s32 doneFlag;   //0x20
} M506_FLYOBJ; //size 0x24

typedef struct M506Bounce_s {
    s32 state;      //0x00
    f32 targetY;    //0x04
    f32 vel;        //0x08
    f32 accel;      //0x0C
    f32 damp;       //0x10
} M506_BOUNCE; //size 0x14

typedef struct M506Flow_s {
    M506_WORK *m506Work;                       //0x000
    M506_WORK *work;                           //0x004
    OMOBJ *playerObj[M506_PLAYER_MAX];         //0x008
    M506_PLAYER_WORK *player[M506_PLAYER_MAX]; //0x018
    s32 mode;                                  //0x028
    s32 unk2C;                                 //0x02C
    s32 unk30;                                 //0x030
    f32 timer;                                 //0x034
    s32 humanTiedFlag;                         //0x038
    s32 tieCount;                              //0x03C
    s32 recordBase;                            //0x040
    s32 newRecordHeight;                       //0x044
    s8 playerHeight[M506_PLAYER_MAX];          //0x048
    s8 playerNewHeight[M506_PLAYER_MAX];       //0x04C
    s8 playerRank[M506_PLAYER_MAX];            //0x050
    s8 playerBest[M506_PLAYER_MAX];            //0x054
    s8 playerMotSel[M506_PLAYER_MAX];          //0x058
    u8 p1SubState;                             //0x05C
    u8 unk5D[3];                               //0x05D
    M506_BOUNCE bounce[M506_PLAYER_MAX];       //0x060
    u8 unkB0[0x20];                            //0x0B0
    s8 stopFlag[M506_PLAYER_MAX];              //0x0D0
    u8 unkD4[8];                               //0x0D4
    s16 recordEspId;                           //0x0DC
    u8 unkDE[8];                               //0x0DE
    s16 recordWinId;                           //0x0E6
    MGSCORE *recordScore;                      //0x0E8
    MGSCORE *scoreArr[M506_PLAYER_MAX];        //0x0EC
    f32 unkFC;                                 //0x0FC
    f32 unk100;                                //0x100
    f32 unk104;                                //0x104
    f32 unk108;                                //0x108
    f32 unk10C;                                //0x10C
    f32 unk110;                                //0x110
    f32 unk114;                                //0x114
    f32 unk118;                                //0x118
    f32 unk11C;                                //0x11C
    f32 unk120;                                //0x120
    u8 unk124[4];                              //0x124
    s32 shadowScrollState;                     //0x128
    s32 rocketType;                            //0x12C
    M506_FLYOBJ rocket;                        //0x130
    s32 unk154;                                //0x154
    s32 unk158;                                //0x158
    M506_FLYOBJ starfighter[M506_PLAYER_MAX];  //0x15C
    s16 recordScrollY;                         //0x1EC
    u8 unk1EE[2];                              //0x1EE
    f32 shadowScrollY;                         //0x1F0
    HU3DMODELID particleMdlId;                 //0x1F4
    GAMEMESID gameMesId;                       //0x1F6
    s16 unk1F8[M506_PLAYER_MAX];               //0x1F8
    s32 unk200;                                //0x200
    f32 tmpX;                                  //0x204
    f32 tmpY;                                  //0x208
    f32 tmpZ;                                  //0x20C
    s32 readyFlag;                             //0x210
    HUPROCESS *seedProc;                       //0x214
} M506_FLOW_WORK; //size 0x218

void M506MainUpdate(OMOBJ *obj);
void M506ObjInit(M506_FLOW_WORK *flow);
BOOL M506FlowModeStart(M506_FLOW_WORK *flow);
void M506FlowModeStartSetup(M506_FLOW_WORK *flow);
void M506FlowMameSetup(M506_FLOW_WORK *flow);
BOOL M506ShadowScrollInit(M506_FLOW_WORK *flow);
void M506FlowP1Update(M506_FLOW_WORK *flow);
void M506FlowModeEnd(OMOBJ *obj);
void M506FlowModeEndSetup(M506_FLOW_WORK *flow);
void M506FlowWinStart(M506_FLOW_WORK *flow);
s32 M506FlowStopCheck(M506_FLOW_WORK *flow);
BOOL M506FlowResult(M506_FLOW_WORK *flow);
void M506ScoreCreate(M506_FLOW_WORK *flow);
void M506ScoreDispUpdate(M506_FLOW_WORK *flow);
void M506ScoreKill(M506_FLOW_WORK *flow);
void M506MameShadowOff(M506_FLOW_WORK *flow);
void M506MameShadowOn(M506_FLOW_WORK *flow);
void M506MameShadowScrollOn(M506_FLOW_WORK *flow);
void M506MameShadowScrollExec(M506_FLOW_WORK *flow);
void M506ParticleCreate(M506_FLOW_WORK *flow);
void M506ParticleUpdate(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);
void M506ParticleStart(M506_FLOW_WORK *flow);
void M506ParticleKill(M506_FLOW_WORK *flow);
void M506RocketInit(M506_FLOW_WORK *flow);
BOOL M506RocketModelUpdate(M506_FLOW_WORK *flow);
void M506RocketMove(M506_FLOW_WORK *flow);
void M506StarfighterStart(M506_FLOW_WORK *flow);
void M506StarfighterModelUpdate(M506_FLYOBJ *fo, HU3DMODELID mdlId);
void M506RecordCreate(M506_FLOW_WORK *flow);
void M506RecordSet(M506_FLOW_WORK *flow);
void M506RecordKill(M506_FLOW_WORK *flow);
void M506RecordBlink(M506_FLOW_WORK *flow);
void M506RecordBlinkInit(void);
void M506YCameraProjUpdate(void);
void M506YCameraViewUpdate(void);
void M506YCameraSplitCreate(void);
void M506YCameraSplitKill(void);
void M506YLightUpdate(void);
void M506YShadowPosUpdate(void);
void M506YCameraViewSet(s32 camNo, s32 viewNo);
BOOL M506YCameraViewLerp(s32 camNo, s32 viewNo);
s32 M506PointLerp(f32 *val, f32 cur, f32 target, f32 duration);
void M506MakeRotMtx(Mtx dst, HuVecF *rot);
void M506SeedUpdate(void);

static const u32 LookUpRotLockFlag[13] = {
    0x200, 0x200, 0x200, 0x200, 0x200, 0x200, 0x200,
    0x200, 0x200, 0x200, 0x200, 0x200, 0x200
};

static const struct { f32 up; f32 down; } LookUpRotTbl[13] = {
    { 180.0f, 135.0f },
    { 180.0f, 135.0f },
    { 100.0f, 85.0f },
    { 88.0f, 43.0f },
    { 84.0f, 52.0f },
    { 100.0f, 75.0f },
    { 72.0f, 27.0f },
    { 180.0f, 135.0f },
    { 180.0f, 135.0f },
    { 180.0f, 135.0f },
    { 180.0f, 135.0f },
    { 180.0f, 135.0f },
    { 180.0f, 135.0f }
};

typedef struct M506YCameraView_s {
    f32 duration;
    f32 hvr[3];
    f32 pos[3];
    f32 targetHvr[3];
    f32 targetPos[3];
} M506_YCAMERA_VIEW; //size 0x34

static const M506_YCAMERA_VIEW YCameraView[3] = {
    { 0.0f,   { 90.0f, 12.0f, 2150.0f }, { 0.0f, 300.0f, 0.0f }, { 90.0f, 12.0f, 2150.0f }, { 0.0f, 300.0f, 0.0f } },
    { 120.0f, { 73.0f, 0.0f, 300.0f },   { 0.0f, 60.0f, 0.0f },  { 90.0f, 0.0f, 1500.0f },  { 0.0f, 150.0f, 0.0f } },
    { 120.0f, { 90.0f, 0.0f, 1500.0f },  { 0.0f, 150.0f, 0.0f }, { 90.0f, 12.0f, 2150.0f }, { 0.0f, 300.0f, 0.0f } }
};

/* raw score box positions: pairs of {x, y} in s16, indexed by player*0xC */
static const u8 ScorePos[0x40] = {
    0x00, 0x3C, 0x00, 0xA0, 0x00, 0x4C, 0x00, 0xA0, 0x00, 0x5C, 0x00, 0xA0, 0x00, 0xC8, 0x00, 0xA0,
    0x00, 0xD8, 0x00, 0xA0, 0x00, 0xE8, 0x00, 0xA0, 0x01, 0x58, 0x00, 0xA0, 0x01, 0x68, 0x00, 0xA0,
    0x01, 0x78, 0x00, 0xA0, 0x01, 0xE8, 0x00, 0xA0, 0x01, 0xF8, 0x00, 0xA0, 0x02, 0x08, 0x00, 0xA0,
    0x00, 0x78, 0x00, 0xA0, 0x01, 0x04, 0x00, 0xA0, 0x01, 0x94, 0x00, 0xA0, 0x02, 0x24, 0x00, 0xA0
};

static char lbl_1_data_298[12] = "ske_head";

static const HuVecF rotTbl700 = { 0.0f, 90.0f, 15.0f };
static const f32 rotTbl700_extra[3] = { 30.0f, 10.0f, 45.0f };

static f32 recordScale725 = 1.0f;

static const HuVecF upVecA = { 0.0f, 1.0f, 0.0f };
static const HuVecF forwardVecA = { 0.0f, 0.0f, 1.0f };

static const HuVecF upVecB = { 0.0f, 1.0f, 0.0f };
static const HuVecF forwardVecB = { 0.0f, 0.0f, 1.0f };

static s32 lbl_1_bss_18;
static f32 ScrollHeight;
static f32 ScrollAngle;
static s32 MameShadowF;
static s8 lbl_1_bss_28;
static s8 RecordInitF;
static s32 SeedInitF;
static f32 LookUpCurrRot;
static s32 StartFXFlag;
static f32 StartFXTime;
static f32 FlowTime;

M506_YSHADOW_WORK *M506YShadow;
M506_YLIGHT *M506YLight;
M506_YCAMERA_WORK *M506YCamera;

static M506_FLOW_WORK M506Flow;

static u8 CharFallSeNo[16];
static u8 FallEndSeNo[16];
static u8 FallStartSeNo[16];

// NONMATCHING: faithful translation, not yet instruction-matched
void M506MainCreate(OMOBJ *obj)
{
    M506_WORK *work;
    work = obj->data;
    M506Create(work);
    obj->objFunc = M506MainUpdate;
}

// NONMATCHING: faithful translation, not yet instruction-matched
void M506FlowEndWatch(OMOBJ *obj)
{
    M506_WORK *work = obj->data;

    BOOL flowUpd;
    if (M506ExitCheck(obj)) {
        return;
    }
    flowUpd = M506FlowUpdate(work);
    if (flowUpd) {
        obj->objFunc = M506FlowModeEnd;
    }
}

// NONMATCHING: faithful translation, not yet instruction-matched
void M506ObjShadowMapSet(int no, int height)
{
    M506_WORK *work = M506MainObj->data;
    Hu3DModelShadowMapSet(work->mameMdlId[no][height - 1]);
}

// NONMATCHING: faithful translation, not yet instruction-matched
void M506ObjShadowMapReset(int no, int from, int to)
{
    M506_WORK *work = M506MainObj->data;
    int i;
    M506_WORK *work2 = work;
    for (i = from; i < to; i++) {
        Hu3DModelShadowMapObjReset(work2->mameMdlId[no][i]);
    }
}

// NONMATCHING: faithful translation, not yet instruction-matched
void M506MainUpdate(OMOBJ *obj)
{
    BOOL exitF = M506ExitCheck(obj);
    int i;
    M506_FLOW_WORK *flow = &M506Flow;

    switch (obj->work[2]) {
    case 0:
        Hu3DBGColorSet(0, 0, 0);
        flow->m506Work = obj->data;
        flow->work = flow->m506Work;
        for (i = 0; i < M506_PLAYER_MAX; i++) {
            flow->playerObj[i] = M506PlayerObj[i];
            flow->player[i] = flow->playerObj[i]->data;
        }
        flow->readyFlag = 0;
        flow->seedProc = HuPrcChildCreate(M506SeedUpdate, 100, 0x3000, 0, M506ObjMan);
        FlowTime = 0.0f;
        M506ObjInit(flow);
        M506RecordCreate(flow);
        M506RecordSet(flow);
        M506ModelUpdate(flow->work);
        WipeCreate(WIPE_MODE_IN, WIPE_TYPE_PREVTYPE, 60);
        StartFXFlag = 0;
        StartFXTime = 0.0f;
        flow->mode = 0;
        flow->unk2C = 0;
        flow->unk30 = 0;
        flow->timer = 0.0f;
        obj->work[2] = 1;
        return;
    case 1:
        if (!StartFXFlag) {
            StartFXTime += 1.0f;
            if (StartFXTime == 50.0f) {
                HuAudFXPlay(0x66B);
                StartFXFlag = 1;
            }
        }
        if (!WipeCheckEnd()) {
            flow->readyFlag = 1;
            obj->work[2] = 2;
        }
        return;
    case 2:
        {
            BOOL done = M506FlowModeStart(flow);
            M506FlowMameSetup(flow);
            M506ModelUpdate(flow->work);
            if (done) {
                obj->work[2] = 3;
                return;
            }
        }
        break;
    case 3:
        if (!exitF) {
            M506ParticleKill(flow);
            obj->work[2] = 0;
            obj->objFunc = M506FlowEndWatch;
        }
        break;
    }
}

// NONMATCHING: faithful translation, not yet instruction-matched
void M506ObjInit(M506_FLOW_WORK *flow)
{
    int i;

    flow->mode = 0;
    flow->unk120 = 0.0f;
    flow->work = 0;
    flow->shadowScrollState = 0;
    flow->rocketType = 0;
    flow->gameMesId = -1;
    M506YCamera->splitTime = 0.0f;
    M506YCameraProjUpdate();
    if (lbl_1_bss_18 == 0) {
        for (i = 0; i < M506_PLAYER_MAX; i++) {
            if (i == 0) {
                M506YCameraViewSet(i, 1);
            } else {
                M506YCameraViewSet(i, 0);
            }
        }
    }
    M506FlowModeStartSetup(flow);
    M506RocketInit(flow);
    flow->unk154 = 0;
    flow->unk158 = 0;
    M506StarfighterStart(flow);
    M506MameShadowOff(flow);
    M506ParticleCreate(flow);
    for (i = 0; i < M506_PLAYER_MAX; i++) {
        flow->unk1F8[i] = 0;
    }
    flow->unk200 = 0;
}

// NONMATCHING: faithful translation, not yet instruction-matched
BOOL M506FlowModeStart(M506_FLOW_WORK *flow)
{
    switch (flow->mode) {
    case 0:
        if (flow->readyFlag == 3) {
            flow->unk30 = 0;
            flow->timer = 0.0f;
            flow->mode = 1;
        }
        break;
    case 1:
        flow->timer += 1.0f;
        if (flow->timer >= 30.0f) {
            M506MameShadowOn(flow);
            LookUpCurrRot = LookUpRotTbl[flow->player[0]->charNo].up;
            Hu3DModelAttrReset(flow->work->moundMdlId, HU3D_MOTATTR_PAUSE);
            flow->unk30 = 0;
            flow->timer = 0.0f;
            flow->mode = 2;
        }
        break;
    case 2:
        M506YCameraViewLerp(0, 1);
        M506MameShadowScrollExec(flow);
        flow->unk10C += 3.0f;
        flow->unk110 += 0.008333334f;
        flow->unk108 += 5.4166665f;
        LookUpCurrRot -= (LookUpRotTbl[flow->player[0]->charNo].up - LookUpRotTbl[flow->player[0]->charNo].down) / 120.0f;
        if (flow->unk10C >= 360.0f) {
            flow->unk10C = 360.0f;
        }
        if (flow->unk110 >= 1.0f) {
            flow->unk110 = 1.0f;
        }
        if (flow->unk108 >= 0.0f) {
            flow->unk108 = 0.0f;
        }
        if (LookUpCurrRot <= LookUpRotTbl[flow->player[0]->charNo].down) {
            LookUpCurrRot = LookUpRotTbl[flow->player[0]->charNo].down;
        }
        if (flow->unk108 >= -108.333374f) {
            M506ShadowScrollInit(flow);
        }
        if (flow->unk108 == 0.0f) {
            flow->unk10C = 0.0f;
            flow->unk110 = 1.0f;
            flow->unk108 = 0.0f;
            LookUpCurrRot = LookUpRotTbl[flow->player[0]->charNo].down;
            flow->mode = 3;
        }
        if (flow->player[0]->charNo != 8) {
            Hu3DMotionForceSet(flow->playerObj[0]->mdlId[0], lbl_1_data_298, LookUpRotLockFlag[flow->player[0]->charNo], LookUpCurrRot);
        }
        break;
    case 3:
        M506ShadowScrollInit(flow);
        flow->timer += 1.0f;
        if (flow->timer >= 15.0f) {
            M506YCameraViewSet(0, 2);
            flow->unk30 = 0;
            flow->unk118 = 1.0f;
            flow->mode = 4;
        }
        break;
    case 4:
        M506ShadowScrollInit(flow);
        M506FlowP1Update(flow);
        M506YCameraViewLerp(0, 2);
        M506YCamera->splitTime += 0.008333334f;
        if (M506YCamera->splitTime >= 1.0f) {
            M506YCamera->splitTime = 1.0f;
        }
        M506YCameraProjUpdate();
        if (flow->player[0]->charNo != 8) {
            LookUpCurrRot += (LookUpRotTbl[flow->player[0]->charNo].up - LookUpRotTbl[flow->player[0]->charNo].down) / 60.0f;
            if (LookUpCurrRot >= LookUpRotTbl[flow->player[0]->charNo].up) {
                LookUpCurrRot = LookUpRotTbl[flow->player[0]->charNo].up;
            }
            Hu3DMotionForceSet(flow->playerObj[0]->mdlId[0], lbl_1_data_298, LookUpRotLockFlag[flow->player[0]->charNo], LookUpCurrRot);
        }
        if (M506YCamera->splitTime >= 1.0f) {
            int i;
            for (i = 0; i < 3; i++) {
                espDispOn(M506YCamera->splitSprId[i]);
            }
            Hu3DBGColorSet(0, 0x7F, 0xFF);
            if (flow->player[0]->charNo != 8) {
                Hu3DMotionNoMotReset(flow->playerObj[0]->mdlId[0], lbl_1_data_298, LookUpRotLockFlag[flow->player[0]->charNo]);
            }
            Hu3DModelAttrSet(flow->work->seedMdlId, HU3D_ATTR_DISPOFF);
            flow->mode = 5;
        }
        break;
    case 5:
        flow->mode = 0;
        return TRUE;
    }
    return FALSE;
}

// NONMATCHING: faithful translation, not yet instruction-matched
void M506FlowModeStartSetup(M506_FLOW_WORK *flow)
{
    int i;

    flow->unkFC = 200.0f;
    flow->unk100 = 0.0f;
    flow->unk104 = 10.0f;
    flow->unk108 = -650.0f;
    flow->unk10C = 0.0f;
    flow->unk110 = 0.0f;
    flow->unk114 = 0.0f;
    flow->unk118 = 0.0f;
    flow->unk11C = 120.0f;
    Hu3DModelAttrReset(flow->work->seedMdlId, HU3D_ATTR_DISPOFF);
    Hu3DModelRotSet(flow->work->moundMdlId, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrReset(flow->work->moundMdlId, HU3D_ATTR_DISPOFF);
    Hu3DModelAttrReset(flow->work->moundMdlId, HU3D_MOTATTR_LOOP);
    Hu3DMotionSpeedSet(flow->work->moundMdlId, 0.5f);
    Hu3DMotionTimeSet(flow->work->moundMdlId, 0.0f);
    Hu3DModelAttrSet(flow->work->moundMdlId, HU3D_MOTATTR_PAUSE);
    M506FlowMameSetup(flow);
    for (i = 0; i < M506_PLAYER_MAX; i++) {
        omSetTra(flow->playerObj[i], -64.5f, 0.0f, 179.5f);
        omSetRot(flow->playerObj[i], 0.0f, 0.0f, 0.0f);
    }
    omSetRot(flow->playerObj[0], 0.0f, flow->unk11C, 0.0f);
}

void M506FlowMameSetup(M506_FLOW_WORK *flow)
{
    int i;

    Hu3DModelPosSet(flow->work->seedMdlId, 0.0f, flow->unkFC, 0.0f);
    Hu3DModelRotSet(flow->work->seedMdlId, flow->unk100, flow->unk100, 0.0f);
    Hu3DModelPosSet(flow->work->moundMdlId, 0.0f, flow->unk104, 0.0f);
    for (i = 0; i < 2; i++) {
        Hu3DModelPosSet(flow->work->mameVineMdlId[0][i], 0.0f, flow->unk108 - (600.0f * i), 0.0f);
        Hu3DModelRotSet(flow->work->mameVineMdlId[0][i], 0.0f, flow->unk10C, 0.0f);
        Hu3DModelScaleSet(flow->work->mameVineMdlId[0][i], flow->unk110, 1.0f, flow->unk110);
    }
    Hu3DModelRotSet(flow->work->mameMdlId[0][0], flow->unk114, -20.0f + (40.0f * flow->work->mameSide[0][0]), 0.0f);
    Hu3DModelScaleSet(flow->work->mameMdlId[0][0], flow->unk118, flow->unk118, flow->unk118);
}

// NONMATCHING: faithful translation, not yet instruction-matched
BOOL M506ShadowScrollInit(M506_FLOW_WORK *flow)
{
    switch (flow->shadowScrollState) {
    case 0:
        flow->unk118 += 0.06666667f;
        if (flow->unk118 >= 1.0f) {
            ScrollHeight = 2.0f;
            ScrollAngle = 0.0f;
            flow->unk118 = 1.0f;
            flow->shadowScrollState++;
        }
        if (MameShadowF == 0) {
            M506MameShadowScrollOn(flow);
            MameShadowF = 1;
        }
        break;
    case 1:
        ScrollHeight -= 0.033333335f;
        if (ScrollHeight < 0.0f) {
            ScrollHeight = 0.0f;
            flow->shadowScrollState++;
            MameShadowF = 0;
            return TRUE;
        }
        ScrollAngle += 16.0f;
        flow->unk114 = ScrollHeight * HuSin(ScrollAngle);
        break;
    }
    return FALSE;
}

// NONMATCHING: faithful translation, not yet instruction-matched
void M506FlowP1Update(M506_FLOW_WORK *flow)
{
    OMOBJ *p0 = M506PlayerObj[0];

    switch (flow->p1SubState) {
    case 0:
        if (flow->player[0]->charNo != 8) {
            Hu3DMotionTimeSet(p0->mdlId[0], 0.0f);
            Hu3DMotionSpeedSet(p0->mdlId[0], 1.0f);
            Hu3DMotionShiftSet(p0->mdlId[0], p0->mtnId[6], 0.0f, 4.0f, 0);
        }
        flow->p1SubState++;
        break;
    case 1:
        if (flow->player[0]->charNo != 8) {
            if (Hu3DMotionTimeGet(p0->mdlId[0]) == 58.0f) {
                Hu3DMotionShiftSet(p0->mdlId[0], p0->mtnId[0], 0.0f, 4.0f, HU3D_MOTATTR_LOOP);
                flow->p1SubState++;
            }
        } else {
            flow->p1SubState++;
        }
        break;
    }
    flow->unk11C -= 2.0f;
    if (flow->unk11C < 0.0f) {
        flow->unk11C = 0.0f;
    }
    omSetRot(flow->playerObj[0], 0.0f, flow->unk11C, 0.0f);
}

// NONMATCHING: faithful translation, not yet instruction-matched
void M506FlowModeEnd(OMOBJ *obj)
{
    BOOL exitF = M506ExitCheck(obj);
    int i;
    M506_FLOW_WORK *flow = &M506Flow;

    switch (obj->work[2]) {
    case 0:
        flow->m506Work = obj->data;
        flow->work = flow->m506Work;
        for (i = 0; i < M506_PLAYER_MAX; i++) {
            flow->playerObj[i] = M506PlayerObj[i];
            flow->player[i] = flow->playerObj[i]->data;
        }
        flow->rocketType = 1;
        M506FlowModeEndSetup(flow);
        M506ModelUpdate(flow->work);
        M506ScoreCreate(flow);
        M506RecordBlinkInit();
        WipeColorSet(0xFF, 0xFF, 0xFF);
        WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, 60);
        obj->work[2] = 1;
        return;
    case 1:
        M506ModelUpdate(flow->work);
        if (!WipeCheckEnd()) {
            HuAudFXPlay(0x1D);
            obj->work[2] = 2;
            return;
        }
        return;
    default:
        return;
    case 2:
        {
            BOOL done = M506FlowStopCheck(flow);
            M506ModelUpdate(flow->work);
            if (done) {
                for (i = 0; i < M506_PLAYER_MAX; i++) {
                    flow->stopFlag[i] = 1;
                }
                flow->mode = 0;
                flow->unk30 = 0;
                flow->timer = 0.0f;
                if (_CheckFlag(FLAG_INST_DECA)) {
                    obj->work[2] = 6;
                } else {
                    obj->work[2] = 3;
                }
            }
            M506ScoreDispUpdate(flow);
        }
        return;
    case 3:
        M506ModelUpdate(flow->work);
        flow->timer += 1.0f;
        if (flow->timer == 30.0f) {
            flow->unk30 = 0;
            flow->timer = 0.0f;
            obj->work[2] = 4;
            return;
        }
        break;
    case 4:
        M506ModelUpdate(flow->work);
        {
            BOOL done = M506FlowResult(flow);
            M506RecordBlink(flow);
            if (done) {
                obj->work[2] = 5;
                return;
            }
        }
        break;
    case 5:
        M506RecordBlink(flow);
        FlowTime += 1.0f;
        if (FlowTime == 210.0f) {
            obj->work[2] = 6;
            return;
        }
        break;
    case 6:
        M506ModelUpdate(flow->work);
        if (!_CheckFlag(FLAG_INST_DECA)) {
            M506RecordBlink(flow);
            WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_WHITEFADE, 60);
            obj->work[2] = 7;
            return;
        }
        flow->timer += 1.0f;
        if (flow->timer == 60.0f) {
            for (i = 0; i < M506_PLAYER_MAX; i++) {
                GwPlayer[i].mgScore = flow->playerHeight[i] * 3;
            }
            WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_WHITEFADE, 60);
            flow->unk30 = 0;
            flow->timer = 0.0f;
            obj->work[2] = 7;
            return;
        }
        break;
    case 7:
        M506ModelUpdate(flow->work);
        if (!_CheckFlag(FLAG_INST_DECA)) {
            M506RecordBlink(flow);
        }
        if (!WipeCheckEnd()) {
            obj->work[2] = 8;
            return;
        }
        break;
    case 8:
        if (!exitF) {
            obj->work[2] = 0;
            HuAudFadeOut(1000);
            obj->objFunc = M506Exit;
        }
        break;
    }
}

// NONMATCHING: complex per-player initialisation with uncertain field layout for the winner
// side offset table and camera pos temporaries; behaviourally faithful but not instruction-matched.
// NONMATCHING: faithful translation, not yet instruction-matched
void M506FlowModeEndSetup(M506_FLOW_WORK *flow)
{
    s8 charNo[M506_PLAYER_MAX];
    s8 best[M506_PLAYER_MAX];
    static const f32 sideOfs[2] = { -64.5f, 64.5f };
    s32 bestHeight;
    s32 humanBestHeight;
    int i, j;

    for (i = 0; i < M506_PLAYER_MAX; i++) {
        charNo[i] = flow->player[i]->charNo;
    }
    flow->recordBase = flow->work->recordVal / 3;
    flow->tieCount = 0;
    for (i = 0; i < M506_PLAYER_MAX; i++) {
        flow->playerBest[i] = flow->player[i]->height;
        flow->playerHeight[i] = flow->player[i]->newHeight;
        flow->playerRank[i] = -1;
        best[i] = -1;
    }

    bestHeight = 0;
    for (i = 0; i < M506_PLAYER_MAX; i++) {
        if (flow->playerBest[i] > bestHeight) {
            bestHeight = flow->playerBest[i];
            flow->tieCount = 1;
            best[i] = 0;
            for (j = 0; j < i; j++) {
                best[j] = -1;
            }
        } else if (flow->playerBest[i] == bestHeight && bestHeight != 0) {
            flow->tieCount++;
            best[i] = 0;
        }
    }
    for (i = 0; i < M506_PLAYER_MAX; i++) {
        flow->playerRank[i] = best[i];
    }

    humanBestHeight = 0;
    for (i = 0; i < M506_PLAYER_MAX; i++) {
        if (flow->playerBest[i] > humanBestHeight && best[i] == 0 && GwPlayerConf[charNo[i]].type == 0) {
            humanBestHeight = flow->playerBest[i];
        }
    }
    flow->newRecordHeight = humanBestHeight;
    flow->humanTiedFlag = 0;
    for (i = 0; i < M506_PLAYER_MAX; i++) {
        if (best[i] == 0 && GwPlayerConf[charNo[i]].type == 0) {
            flow->humanTiedFlag = 1;
            break;
        }
    }

    for (i = 0; i < M506_PLAYER_MAX; i++) {
        flow->stopFlag[i] = 0;
        if (best[i] == -1) {
            flow->playerMotSel[i] = 5;
        } else if (best[i] == 0) {
            flow->playerMotSel[i] = 4;
        }
    }

    {
        int rankIdx = 0;
        for (i = 0; i < M506_PLAYER_MAX; i++) {
            if (best[i] != -1) {
                flow->playerRank[rankIdx] = charNo[i];
                rankIdx++;
            }
        }
    }

    for (i = 0; i < M506_PLAYER_MAX; i++) {
        M506_BOUNCE *b = &flow->bounce[i];
        if (flow->playerBest[i] < 1) {
            b->targetY = 300.0f;
        } else {
            b->targetY = 100.0f + (300.0f * flow->playerBest[i]);
        }
        b->state = 0;
        b->vel = 0.0f;
        b->accel = 0.4f;
        b->damp = 0.01f;
    }

    M506YCamera->splitTime = 1.0f;
    for (i = 0; i < M506_PLAYER_MAX; i++) {
        M506_YCAMERA *cam = &M506YCamera->camera[i];
        cam->hvr.x = 90.0f;
        cam->hvr.y = 12.0f;
        cam->hvr.z = 2150.0f;
        cam->pos.x = 0.0f;
        cam->pos.y = 300.0f;
        cam->pos.z = 0.0f;
    }
    M506YCameraUpdate();
    M506YCameraProjUpdate();

    for (i = 0; i < M506_PLAYER_MAX; i++) {
        int side = (flow->playerBest[i] == 0) ? 0 : flow->work->mameSide[flow->player[i]->no][flow->playerBest[i] - 1];
        flow->tmpX = sideOfs[side];
        flow->tmpY = 300.0f * flow->playerBest[i];
        flow->tmpZ = 179.5f;
        omSetTra(flow->playerObj[i], flow->tmpX, flow->tmpY, flow->tmpZ);
        omSetRot(flow->playerObj[i], 0.0f, 0.0f, 0.0f);
    }

    M506RocketInit(flow);
    for (i = 0; i < M506_PLAYER_MAX; i++) {
        Hu3DModelAttrSet(flow->work->starfighterMdlId[i], HU3D_MOTATTR_PAUSE);
    }
    M506StarfighterStart(flow);
}

// NONMATCHING: faithful translation, not yet instruction-matched
void M506FlowWinStart(M506_FLOW_WORK *flow)
{
    int i;
    for (i = 0; i < M506_PLAYER_MAX; i++) {
        OMOBJ *p = flow->playerObj[i];
        if (flow->stopFlag[i] != 1) {
            Hu3DMotionSpeedSet(p->mdlId[0], 1.0f);
            Hu3DMotionShiftSet(p->mdlId[0], p->mtnId[flow->playerMotSel[i]], 0.0f, 4.0f, 0);
            flow->stopFlag[i]++;
        }
    }
}

// NONMATCHING: faithful translation, not yet instruction-matched
s32 M506FlowStopCheck(M506_FLOW_WORK *flow)
{
    static const s16 panTbl[M506_PLAYER_MAX] = { 40, 56, 72, 88 };
    s32 stopCnt = 0;
    int i, j;

    for (i = 0; i < M506_PLAYER_MAX; i++) {
        M506_YCAMERA *cam = &M506YCamera->camera[i];
        M506_BOUNCE *b = &flow->bounce[i];
        if (cam->pos.y < b->targetY) {
            if (b->state == 0) {
                f32 sum;
                f32 v;
                b->vel += b->accel - (b->vel * b->damp);
                sum = 0.0f;
                v = b->vel;
                for (j = 0; j < 32; j++) {
                    sum += v;
                    v *= 0.9f;
                }
                if (sum > (b->targetY - cam->pos.y)) {
                    b->state = 1;
                }
            } else if (b->state == 1) {
                b->vel *= 0.9f;
            }
            cam->pos.y += b->vel;
            if (cam->pos.y > b->targetY) {
                cam->pos.y = b->targetY;
            }
        } else {
            if (flow->stopFlag[i] == 0) {
                HuAudFXPlayPan(0x21, panTbl[i]);
            }
            flow->stopFlag[i] = 1;
            stopCnt++;
        }
    }
    if (stopCnt == 4) {
        HuAudFXPlay(0x1E);
        return 1;
    }
    return 0;
}

// NONMATCHING: faithful translation, not yet instruction-matched
BOOL M506FlowResult(M506_FLOW_WORK *flow)
{
    int i;

    switch (flow->mode) {
    case 0:
        lbl_1_bss_28 = 0;
        flow->gameMesId = -1;
        if (flow->newRecordHeight > flow->recordBase) {
            flow->gameMesId = GameMesCreate(GAMEMES_MES_MG_RECORD, flow->newRecordHeight * 3);
            flow->mode = 1;
            flow->work->recordVal = flow->newRecordHeight * 3;
            M506RecordSet(flow);
            GWMgRecordSet(0, flow->newRecordHeight * 3);
            if (!_CheckFlag(FLAG_MG_PRACTICE)) {
                HuAudJinglePlay(0x86);
            }
        } else {
            flow->mode = 2;
        }
        break;
    default:
        return 0;
    case 1:
        if (GameMesStatGet(flow->gameMesId) == 0) {
            if (flow->gameMesId >= 0) {
                GameMesKill(flow->gameMesId);
            }
            flow->mode = 2;
        }
        break;
    case 2:
        M506FlowWinStart(flow);
        FlowTime += 1.0f;
        flow->gameMesId = -1;
        if (flow->tieCount == 0) {
            flow->gameMesId = GameMesCreate(GAMEMES_MES_MG, GAMEMES_MG_TYPE_DRAW, FlowTime);
            HuAudJinglePlay(0x7B);
            CharLoseVoicePlay(flow->player[0]->charNo, flow->player[1]->charNo, flow->player[2]->charNo, flow->player[3]->charNo);
        } else {
            flow->gameMesId = GameMesCreate(GAMEMES_MES_MG_WINNER, GAMEMES_MG_TYPE_WIN,
                flow->playerRank[0], flow->playerRank[1], flow->playerRank[2], flow->playerRank[3], FlowTime);
            if (flow->humanTiedFlag == 0 || flow->humanTiedFlag == 1) {
                HuAudJinglePlay(0x79);
            }
            for (i = 0; i < M506_PLAYER_MAX; i++) {
                if (flow->playerBest[i] == 0 && !_CheckFlag(FLAG_MG_PRACTICE)) {
                    GwPlayer[i].mgCoinBonus = 10;
                }
            }
        }
        flow->mode = 3;
        break;
    case 3:
        M506FlowWinStart(flow);
        FlowTime += 1.0f;
        if (GameMesStatGet(flow->gameMesId) == 0) {
            flow->mode = 0;
            return 1;
        }
        break;
    }
    return 0;
}

// NONMATCHING: faithful translation, not yet instruction-matched
void M506ScoreCreate(M506_FLOW_WORK *flow)
{
    int i;
    for (i = 0; i < M506_PLAYER_MAX; i++) {
        s16 x = ((const s16 *)ScorePos)[(i * 0xC) / 2];
        s16 y = ((const s16 *)ScorePos)[((i * 0xC) / 2) + 1];
        flow->stopFlag[i] = 0;
        flow->scoreArr[i] = MgScoreCreate(0x960032, 0x960036, 0);
        MgScoreMaxDigitSet(flow->scoreArr[i], 3);
        MgScoreScaleSet(flow->scoreArr[i], 1.0f, 1.0f);
        MgScoreValueSet(flow->scoreArr[i], 0);
        MgScorePosSet(flow->scoreArr[i], (x - 20.0f) - 8.0f, 12.0f + y);
        MgScoreZRotSet(flow->scoreArr[i], 0.0f);
        MgScoreTPLvlSet(flow->scoreArr[i], 1.0f);
        MgScoreColorSet(flow->scoreArr[i], 0xFF, 0xFF, 0xFF);
        MgScoreDigitWidthSet(flow->scoreArr[i], 18.0f);
        MgScorePriSet(flow->scoreArr[i], 0x100);
        MgScoreDispOff(flow->scoreArr[i]);
        HuSprBankSet(flow->scoreArr[i]->grpId, 5, 1);
    }
}

// NONMATCHING: faithful translation, not yet instruction-matched
void M506ScoreDispUpdate(M506_FLOW_WORK *flow)
{
    int i;
    for (i = 0; i < M506_PLAYER_MAX; i++) {
        if (flow->stopFlag[i] == 0) {
            MgScoreDispOff(flow->scoreArr[i]);
        } else if (flow->stopFlag[i] == 1) {
            MgScoreValueSet(flow->scoreArr[i], flow->playerHeight[i] * 3);
            MgScoreDispOn(flow->scoreArr[i]);
        }
    }
}

void M506ScoreKill(M506_FLOW_WORK *flow)
{
    int i;
    for (i = 0; i < M506_PLAYER_MAX; i++) {
        if (flow->scoreArr[i] != 0) {
            MgScoreKill(flow->scoreArr[i]);
        }
    }
}

// NONMATCHING: faithful translation, not yet instruction-matched
void M506MameShadowOff(M506_FLOW_WORK *flow)
{
    Hu3DModelAttrSet(flow->work->mameShadowMdlId[0], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(flow->work->mameShadowMdlId[0], HU3D_MOTATTR_PAUSE);
    Hu3DModelAttrReset(flow->work->mameShadowMdlId[0], HU3D_MOTATTR_LOOP);
    Hu3DMotionTimeSet(flow->work->mameShadowMdlId[0], 0.0f);
    Hu3DMotionSpeedSet(flow->work->mameShadowMdlId[0], 0.5f);
    flow->shadowScrollY = 0.0f;
    flow->recordScrollY = Hu3DTexScrollCreate(flow->work->mameShadowMdlId[0], "I8m506sdw01");
    Hu3DTexScrollPosSet(flow->recordScrollY, 0.0f, flow->shadowScrollY, 0.0f);
    Hu3DModelAttrSet(flow->work->glowShadowMdlId, HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(flow->work->glowShadowMdlId, HU3D_MOTATTR_PAUSE);
    Hu3DModelAttrReset(flow->work->glowShadowMdlId, HU3D_MOTATTR_LOOP);
    Hu3DMotionTimeSet(flow->work->glowShadowMdlId, 0.0f);
    Hu3DMotionSpeedSet(flow->work->glowShadowMdlId, 1.0f);
}

void M506MameShadowOn(M506_FLOW_WORK *flow)
{
    Hu3DModelAttrReset(flow->work->mameShadowMdlId[0], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrReset(flow->work->mameShadowMdlId[0], HU3D_MOTATTR_PAUSE);
}

void M506MameShadowScrollOn(M506_FLOW_WORK *flow)
{
    Hu3DModelAttrReset(flow->work->glowShadowMdlId, HU3D_ATTR_DISPOFF);
    Hu3DModelAttrReset(flow->work->glowShadowMdlId, HU3D_MOTATTR_PAUSE);
}

// NONMATCHING: faithful translation, not yet instruction-matched
void M506MameShadowScrollExec(M506_FLOW_WORK *flow)
{
    flow->shadowScrollY += 0.0033333334f;
    if (flow->shadowScrollY > 0.4f) {
        flow->shadowScrollY = 0.4f;
    }
    Hu3DTexScrollPosSet(flow->recordScrollY, 0.0f, flow->shadowScrollY, 0.0f);
}

// NONMATCHING: faithful translation, not yet instruction-matched
void M506ParticleCreate(M506_FLOW_WORK *flow)
{
    HU3DPARTICLE *particle;
    HU3DPARTICLEDATA *data;
    int i;

    flow->particleMdlId = Hu3DParticleCreate(HuSprAnimRead(HuDataSelHeapReadNum(M506_ANM_cloud, HU_MEMNUM_OVL, HEAP_MODEL)), 0x20);
    particle = Hu3DData[flow->particleMdlId].hookData;
    Hu3DParticleBlendModeSet(flow->particleMdlId, 1);
    Hu3DParticleHookSet(flow->particleMdlId, M506ParticleUpdate);
    Hu3DModelLayerSet(flow->particleMdlId, 6);
    Hu3DModelPosSet(flow->particleMdlId, 0.0f, 0.0f, 0.0f);
    Hu3DParticleColSet(flow->particleMdlId, 0x80, 0x40, 0x20);
    data = particle->data;
    for (i = 0; i < particle->maxCnt; i++) {
        data[i].time = 0;
        data[i].scale = 0.0f;
        data[i].color.a = 0;
    }
}

// NONMATCHING: faithful translation, not yet instruction-matched
void M506ParticleUpdate(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx)
{
    HU3DPARTICLEDATA *data = particleP->data;
    f64 angle = 0.0;
    int i;

    for (i = 0; i < particleP->maxCnt; i++) {
        HU3DPARTICLEDATA *d = &data[i];
        switch (d->time) {
        case 0:
            break;
        case 1:
            d->parManId = frandmod(16);
            d->accel.x = 0.0f;
            d->color.a = 0;
            d->color.r = 0xE0;
            d->color.g = 0x80;
            d->color.b = 0x40;
            d->scale = 0.0f;
            d->zRot = 0.0f;
            {
                f32 a = (frandmod(0x12C) + 1) / 100.0f;
                f32 b = (frandmod(0x12C) + 1) / 100.0f;
                d->pos.x = a - b;
            }
            d->pos.y = 10.0f;
            {
                f32 a = (frandmod(0x12C) + 1) / 100.0f;
                f32 b = (frandmod(0x12C) + 1) / 100.0f;
                d->pos.z = a - b;
            }
            {
                f32 a = (frandmod(0x14) + 1) / 100.0f;
                f32 b = (frandmod(0x14) + 1) / 100.0f;
                d->vel.x = (f32)(HuCos(angle) + b - a);
            }
            {
                f32 a = (frandmod(0xA) + 1) / 100.0f;
                d->vel.y = (0.3f + a) - ((frandmod(0xA) + 1) / 100.0f);
            }
            {
                f32 a = (frandmod(0x14) + 1) / 100.0f;
                f32 b = (frandmod(0x14) + 1) / 100.0f;
                d->vel.z = (f32)(HuSin(angle) + b - a);
            }
            angle = (f64)(f32)(angle + (360.0f / particleP->maxCnt));
            d->time++;
            /* fallthrough */
        case 2:
            d->parManId--;
            if (d->parManId < 0) {
                d->time++;
            }
            break;
        default:
            d->scale += 0.3f;
            d->pos.x += d->vel.x;
            d->pos.y += d->vel.y;
            d->pos.z += d->vel.z;
            if (d->time == 3) {
                d->accel.x += 0.125f;
                if (d->accel.x == 1.0f) {
                    d->time++;
                    d->accel.x = 1.0f;
                }
            } else if (d->time == 4) {
                d->accel.x -= 0.010416667f;
                if (d->accel.x == 0.0f) {
                    d->accel.x = 0.0f;
                }
            }
            d->color.a = (u8)(64.0f * d->accel.x);
            if (d->time == 4 && d->accel.x == 0.0f) {
                d->time = 0;
            }
            break;
        }
    }
}

// NONMATCHING: faithful translation, not yet instruction-matched
void M506ParticleStart(M506_FLOW_WORK *flow)
{
    HU3DPARTICLE *particle = Hu3DData[flow->particleMdlId].hookData;
    HU3DPARTICLEDATA *data = particle->data;
    int i;
    for (i = 0; i < particle->maxCnt; i++) {
        data[i].time = 1;
    }
}

void M506ParticleKill(M506_FLOW_WORK *flow)
{
    Hu3DModelKill(flow->particleMdlId);
}

// NONMATCHING: faithful translation, not yet instruction-matched
void M506RocketInit(M506_FLOW_WORK *flow)
{
    flow->rocket.pos.x = 600.0f;
    flow->rocket.pos.y = 11100.0f;
    flow->rocket.pos.z = -2500.0f;
    flow->rocket.rot.x = 0.0f;
    flow->rocket.rot.y = 0.0f;
    flow->rocket.rot.z = 90.0f;
    if (flow->rocketType == 0) {
        flow->rocket.speed = 5.0f;
    } else if (flow->rocketType == 1) {
        flow->rocket.speed = 7.5f;
    }
    flow->rocket.state = 0;
    flow->rocket.doneFlag = 0;
}

// NONMATCHING: faithful translation, not yet instruction-matched
void M506RocketUpdate(void)
{
    M506_FLOW_WORK *flow = &M506Flow;
    int i;

    if (flow->rocketType == 0 && flow->work->rocketCamBit == 0) {
        for (i = 0; i < M506_PLAYER_MAX; i++) {
            if (flow->player[i]->height == 30) {
                flow->work->rocketCamBit |= M506CameraBit[i];
            }
        }
    }
    if (flow->rocket.doneFlag == 0) {
        if (flow->rocketType == 0) {
            for (i = 0; i < M506_PLAYER_MAX; i++) {
                if (flow->player[i]->height == 0x23) {
                    flow->rocket.doneFlag = 1;
                    flow->rocket.state = 1;
                    break;
                }
            }
        } else if (flow->rocketType == 1 && flow->rocket.state == 1) {
            for (i = 0; i < M506_PLAYER_MAX; i++) {
                if (M506YCamera->camera[i].pos.y == 9900.0f) {
                    flow->rocket.doneFlag = 1;
                    break;
                }
            }
        }
    }
    switch (flow->rocket.state) {
    case 0:
        if (flow->rocket.doneFlag != 0) {
            flow->rocket.state++;
        }
        break;
    case 1:
        if (M506RocketModelUpdate(flow)) {
            flow->rocket.state++;
        }
        break;
    }
}

// NONMATCHING: faithful translation, not yet instruction-matched
BOOL M506RocketModelUpdate(M506_FLOW_WORK *flow)
{
    HuVecF ofs;
    Mtx m;

    M506RocketMove(flow);
    ofs.x = 0.0f;
    ofs.y = flow->rocket.speed;
    ofs.z = 0.0f;
    M506MakeRotMtx(m, &flow->rocket.rot);
    PSMTXMultVec(m, &ofs, &ofs);
    flow->rocket.pos.x += ofs.x;
    flow->rocket.pos.y += ofs.y;
    flow->rocket.pos.z += ofs.z;
    Hu3DModelPosSet(flow->work->rocketMdlId, flow->rocket.pos.x, flow->rocket.pos.y, flow->rocket.pos.z);
    Hu3DModelRotSet(flow->work->rocketMdlId, flow->rocket.rot.x, flow->rocket.rot.y, flow->rocket.rot.z);
    return FALSE;
}

// NONMATCHING: faithful translation, not yet instruction-matched
void M506RocketMove(M506_FLOW_WORK *flow)
{
    if (flow->rocketType == 0) {
        flow->rocket.rot.x += (rotTbl700.z - rotTbl700.x) / 200.0f;
        flow->rocket.rot.z += (rotTbl700_extra[0] - rotTbl700.y) / 200.0f;
        return;
    }
    if (flow->rocketType == 1) {
        flow->rocket.rot.x += (rotTbl700_extra[1] - rotTbl700.x) / 200.0f;
        flow->rocket.rot.z += (rotTbl700_extra[2] - rotTbl700.y) / 200.0f;
    }
}

// NONMATCHING: faithful translation, not yet instruction-matched
void M506StarfighterStart(M506_FLOW_WORK *flow)
{
    int i;
    for (i = 0; i < M506_PLAYER_MAX; i++) {
        M506_FLYOBJ *fo = &flow->starfighter[i];
        fo->pos.x = 700.0f;
        fo->pos.y = 15300.0f;
        fo->pos.z = -2500.0f;
        fo->rot.x = -45.0f;
        fo->rot.y = 0.0f;
        fo->rot.z = 135.0f;
        fo->speed = 16.0f;
        fo->state = 0;
        fo->doneFlag = 0;
        M506StarfighterModelUpdate(fo, flow->work->starfighterMdlId[i]);
        Hu3DModelAttrSet(flow->work->starfighterMdlId[i], HU3D_ATTR_DISPOFF);
    }
}

// NONMATCHING: faithful translation, not yet instruction-matched
void M506StarfighterUpdate(void)
{
    M506_FLOW_WORK *flow = &M506Flow;
    int i;

    for (i = 0; i < M506_PLAYER_MAX; i++) {
        M506_FLYOBJ *fo = &flow->starfighter[i];
        if (fo->doneFlag == 0 && flow->player[i]->newHeight == 0x32 && flow->player[i]->upJumpF == 0) {
            Hu3DModelAttrReset(flow->work->starfighterMdlId[i], 1);
            fo->doneFlag = 1;
        }
        switch (fo->state) {
        case 0:
            if (fo->doneFlag != 0) {
                fo->state++;
            }
            break;
        case 1:
            M506StarfighterModelUpdate(fo, flow->work->starfighterMdlId[i]);
            break;
        }
    }
}

void M506StarfighterModelUpdate(M506_FLYOBJ *fo, HU3DMODELID mdlId)
{
    HuVecF ofs;
    Mtx m;

    ofs.x = 0.0f;
    ofs.y = fo->speed;
    ofs.z = 0.0f;
    M506MakeRotMtx(m, &fo->rot);
    PSMTXMultVec(m, &ofs, &ofs);
    fo->pos.x += ofs.x;
    fo->pos.y += ofs.y;
    fo->pos.z += ofs.z;
    Hu3DModelPosSet(mdlId, fo->pos.x, fo->pos.y, fo->pos.z);
    Hu3DModelRotSet(mdlId, fo->rot.x, fo->rot.y, fo->rot.z);
}

void M506RecordCreate(M506_FLOW_WORK *flow)
{
    if (!_CheckFlag(FLAG_INST_DECA)) {
        flow->recordEspId = espEntry(0x40001E, 0, 0);
        espDispOff(flow->recordEspId);
        espPriSet(flow->recordEspId, 0x10);
        espPosSet(flow->recordEspId, 236.0f, 420.0f);
        espScaleSet(flow->recordEspId, 1.0f, 1.0f);
        espAttrSet(flow->recordEspId, 1);
        espDispOn(flow->recordEspId);
        flow->recordScore = MgScoreCreate(0x96003F, 0x960035, 0);
        MgScoreMaxDigitSet(flow->recordScore, 3);
        MgScoreScaleSet(flow->recordScore, 1.0f, 1.0f);
        MgScoreValueSet(flow->recordScore, 0);
        MgScorePosSet(flow->recordScore, 258.0f, 420.0f);
        MgScoreZRotSet(flow->recordScore, 0.0f);
        MgScoreTPLvlSet(flow->recordScore, 1.0f);
        MgScoreColorSet(flow->recordScore, 0x42, 0xFF, 0x7A);
        MgScoreDigitWidthSet(flow->recordScore, 16.0f);
        MgScoreDispOn(flow->recordScore);
        flow->recordWinId = MgScoreWinCreate(0x90, 0x28);
        MgScoreWinColorSet(flow->recordWinId, 0, 0, 0);
        MgScoreWinTPLvlSet(flow->recordWinId, 0.7f);
        MgScoreWinPosSet(flow->recordWinId, 288.0f, 420.0f);
        MgScoreWinDispSet(flow->recordWinId, 1);
    }
}

// NONMATCHING: faithful translation, not yet instruction-matched
void M506RecordSet(M506_FLOW_WORK *flow)
{
    if (!_CheckFlag(FLAG_INST_DECA)) {
        MgScoreValueSet(flow->recordScore, flow->m506Work->recordVal);
    }
}

void M506RecordKill(M506_FLOW_WORK *flow)
{
    if (!_CheckFlag(FLAG_INST_DECA)) {
        espKill(flow->recordEspId);
        if (flow->recordScore != 0) {
            MgScoreKill(flow->recordScore);
        }
    }
}

// NONMATCHING: faithful translation, not yet instruction-matched
void M506RecordBlink(M506_FLOW_WORK *flow)
{
    if (!_CheckFlag(FLAG_INST_DECA) && flow->newRecordHeight > flow->recordBase) {
        if (RecordInitF == 0) {
            recordScale725 += 0.02f;
            if (recordScale725 >= 1.2f) {
                recordScale725 = 1.2f;
                RecordInitF = 1;
            }
        } else if (RecordInitF == 1) {
            recordScale725 -= 0.04f;
            if (recordScale725 <= 1.0f) {
                recordScale725 = 1.0f;
                RecordInitF = 0;
            }
        }
        espScaleSet(flow->recordEspId, recordScale725, recordScale725);
        MgScoreDigitScaleSet(flow->recordScore, recordScale725, recordScale725);
    }
}

void M506RecordBlinkInit(void)
{
    if (!_CheckFlag(FLAG_INST_DECA)) {
        RecordInitF = 0;
        recordScale725 = 1.0f;
    }
}

void fn_1_9EC4(void)
{
}

void fn_1_9EC8(void)
{
}

void fn_1_9ECC(void)
{
}

// NONMATCHING: faithful translation, not yet instruction-matched
void M506Debug(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7,
    f64 farg0, f64 farg1, f64 farg2, f64 farg3, f64 farg4, f64 farg5, f64 farg6, f64 farg7)
{
    int sp8, spC, sp10, sp14, sp18, sp1C, sp20, sp24;
    f64 sp28, sp30, sp38, sp40, sp48, sp50, sp58, sp60;

    sp28 = farg0;
    sp30 = farg1;
    sp38 = farg2;
    sp40 = farg3;
    sp48 = farg4;
    sp50 = farg5;
    sp58 = farg6;
    sp60 = farg7;
    sp8 = arg0;
    spC = arg1;
    sp10 = arg2;
    sp14 = arg3;
    sp18 = arg4;
    sp1C = arg5;
    sp20 = arg6;
    sp24 = arg7;
}

void M506YCameraInit(void)
{
    int i;

    M506YCamera = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M506_YCAMERA_WORK), HU_MEMNUM_OVL);
    if (M506YCamera == NULL) {
        OSReport("M506YCameraInit Error...\n");
        return;
    }
    for (i = 0; i < M506_PLAYER_MAX; i++) {
        M506_YCAMERA *cam = &M506YCamera->camera[i];
        cam->bit = M506CameraBit[i];
        Hu3DCameraCreate(cam->bit);
        cam->center.x = 0.0f;
        cam->center.y = 0.0f;
        cam->center.z = 0.0f;
        cam->target.x = 0.0f;
        cam->target.y = 0.0f;
        cam->target.z = 0.0f;
        cam->hvr.x = 90.0f;
        cam->hvr.y = 0.0f;
        cam->hvr.z = 2000.0f;
        cam->unk4C = 0.0f;
        cam->pos.x = 0.0f;
        cam->pos.y = 300.0f;
        cam->pos.z = 0.0f;
        cam->fov = 30.0f;
    }
    M506YCamera->splitTime = 1.0f;
    M506YCamera->unk1D4 = 0;
    M506YCameraSplitCreate();
    M506YCameraUpdate();
    M506YCameraProjUpdate();
}

void M506YCameraClose(void)
{
    M506_FLOW_WORK *flow = &M506Flow;
    int i;

    HuMemDirectFree(M506YCamera);
    for (i = 0; i < M506_PLAYER_MAX; i++) {
        if (flow->scoreArr[i] != 0) {
            MgScoreKill(flow->scoreArr[i]);
        }
    }
    M506YCameraSplitKill();
    if (!_CheckFlag(FLAG_INST_DECA)) {
        espKill(flow->recordEspId);
        if (flow->recordScore != 0) {
            MgScoreKill(flow->recordScore);
        }
    }
    Hu3DModelKill(flow->particleMdlId);
    if (flow->gameMesId >= 0) {
        GameMesKill(flow->gameMesId);
    }
}

void fn_1_A1C4(void)
{
}

void M506YCameraUpdate(void)
{
    int i;
    for (i = 0; i < M506_PLAYER_MAX; i++) {
        M506_YCAMERA *cam = &M506YCamera->camera[i];
        cam->target.x = cam->pos.x;
        cam->target.y = cam->pos.y;
        cam->target.z = cam->pos.z;
        cam->center.x = cam->target.x + (cam->hvr.z * HuCos(cam->hvr.y) * HuCos(cam->hvr.x));
        cam->center.y = cam->target.y + (cam->hvr.z * HuSin(cam->hvr.y));
        cam->center.z = cam->target.z + (cam->hvr.z * HuCos(cam->hvr.y) * HuSin(cam->hvr.x));
        PSVECSubtract((Vec *)&cam->target, (Vec *)&cam->center, (Vec *)&cam->dir);
        PSVECNormalize((Vec *)&cam->dir, (Vec *)&cam->dir);
        PSVECCrossProduct((Vec *)&upVecA, (Vec *)&cam->dir, (Vec *)&cam->upDir);
        PSVECNormalize((Vec *)&cam->upDir, (Vec *)&cam->upDir);
        PSVECCrossProduct((Vec *)&cam->dir, (Vec *)&cam->upDir, (Vec *)&cam->up);
        PSVECNormalize((Vec *)&cam->up, (Vec *)&cam->up);
        Hu3DCameraPosSet(cam->bit, cam->center.x, cam->center.y, cam->center.z,
            cam->up.x, cam->up.y, cam->up.z, cam->target.x, cam->target.y, cam->target.z);
    }
}

void M506YCameraProjUpdate(void)
{
    int i;

    M506YCameraViewUpdate();
    for (i = 0; i < M506_PLAYER_MAX; i++) {
        M506_YCAMERA *cam = &M506YCamera->camera[i];
        Hu3DCameraPerspectiveSet(cam->bit, cam->fov, 20.0f, 30000.0f, cam->aspect);
        Hu3DCameraViewportSet(cam->bit, cam->viewportX, 0.0f, cam->viewportW, 480.0f, 0.0f, 1.0f);
        Hu3DCameraScissorSet(cam->bit, (u32)cam->scissorX, 0, (u32)cam->scissorW, 480);
    }
}

// NONMATCHING: faithful translation, not yet instruction-matched
void M506YCameraViewUpdate(void)
{
    f32 t = M506YCamera->splitTime;
    M506_YCAMERA *cam0 = &M506YCamera->camera[0];
    M506_YCAMERA *cam1 = &M506YCamera->camera[1];
    M506_YCAMERA *cam2 = &M506YCamera->camera[2];
    M506_YCAMERA *cam3 = &M506YCamera->camera[3];

    cam0->aspect = (151.0f + (425.0f * (1.0f - t))) / 480.0f;
    cam0->viewportX = 0.0f;
    cam0->viewportW = 160.0f + (480.0f * (1.0f - t));
    if (t < 0.33333334f) {
        cam0->scissorX = 0.0f;
        cam0->scissorW = (f32)(s32)cam0->viewportW - 2.0f;
    } else if (t < 1.0f) {
        cam0->scissorX = 0.0f;
        cam0->scissorW = cam0->viewportW - 2.0f;
    } else {
        cam0->scissorX = 0.0f;
        cam0->scissorW = cam0->viewportW;
    }

    cam1->aspect = 0.28541666f;
    cam1->viewportX = 160.0f;
    cam1->viewportW = 160.0f;
    if (t < 1.0f) {
        f32 shift = 160.0f * ((t - 0.6666667f) / 0.33333334f);
        cam1->scissorX = 160.0f + (160.0f - shift);
        cam1->scissorW = shift;
    } else {
        cam1->scissorX = 161.0f;
        cam1->scissorW = 158.0f;
    }

    cam2->aspect = 0.28541666f;
    cam2->viewportX = 320.0f;
    cam2->viewportW = 160.0f;
    if (t < 0.6666667f) {
        f32 shift = 160.0f * ((t - 0.33333334f) / 0.33333334f);
        cam2->scissorX = 320.0f + (160.0f - shift);
        cam2->scissorW = shift;
    } else {
        cam2->scissorX = 321.0f;
        cam2->scissorW = 158.0f;
    }

    cam3->aspect = 0.31458333f;
    cam3->viewportX = 480.0f;
    cam3->viewportW = 160.0f;
    if (t < 0.33333334f) {
        cam3->scissorX = (f32)(0x280 - (s32)(160.0f * (t / 0.33333334f)));
        cam3->scissorW = 640.0f - cam3->scissorX;
        return;
    }
    cam3->scissorX = 481.0f;
    cam3->scissorW = 159.0f;
}

// NONMATCHING: faithful translation, not yet instruction-matched
void M506YCameraSplitCreate(void)
{
    static const f32 splitX[3] = { 136.0f, 296.0f, 456.0f };
    int i;

    for (i = 0; i < 3; i++) {
        M506YCamera->splitSprId[i] = espEntry(0x40001D, 0, 0);
        espDispOff(M506YCamera->splitSprId[i]);
        espPriSet(M506YCamera->splitSprId[i], 0x102);
        espPosSet(M506YCamera->splitSprId[i], splitX[i], 240.0f);
        espScaleSet(M506YCamera->splitSprId[i], 0.6f, 1.0f);
        espAttrSet(M506YCamera->splitSprId[i], 1);
    }
}

void M506YCameraSplitKill(void)
{
    int i;
    for (i = 0; i < 3; i++) {
        espKill(M506YCamera->splitSprId[i]);
    }
}

void fn_1_AAE0(void)
{
}

// NONMATCHING: faithful translation, not yet instruction-matched
void M506YLightInit(void)
{
    Hu3DLighInit();
    M506YLight = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M506_YLIGHT), HU_MEMNUM_OVL);
    if (M506YLight == NULL) {
        OSReport("M506YLightInit Error...\n");
        return;
    }
    M506YLight->hvr.x = 60.0f;
    M506YLight->hvr.y = 50.0f;
    M506YLight->hvr.z = 1000.0f;
    M506YLight->unk10.x = 0.0f;
    M506YLight->unk10.y = 0.0f;
    M506YLight->unk10.z = 0.0f;
    M506YLight->color.r = 0xE8;
    M506YLight->color.g = 0xE8;
    M506YLight->color.b = 0xE8;
    M506YLight->color.a = 0xFF;
    M506YLight->target.x = 0.0f;
    M506YLight->target.y = 20000.0f;
    M506YLight->target.z = -1000.0f;
    M506YLight->center.x = 0.0f;
    M506YLight->center.y = 0.0f;
    M506YLight->center.z = 0.0f;
    M506YLight->pos = M506YLight->target;
    PSVECSubtract((Vec *)&M506YLight->center, (Vec *)&M506YLight->target, (Vec *)&M506YLight->dir);
    M506YLight->lightId = Hu3DGLightCreateV(&M506YLight->pos, &M506YLight->dir, &M506YLight->color);
    M506YLightUpdate();
    Hu3DGLightStaticSet(M506YLight->lightId, 1);
    Hu3DGLightInfinitytSet(M506YLight->lightId);
}

void M506YLightClose(void)
{
    HuMemDirectFree(M506YLight);
}

// NONMATCHING: faithful translation, not yet instruction-matched
void M506YLightUpdate(void)
{
    M506YLight->center = M506YLight->unk10;
    M506YLight->target.x = M506YLight->center.x + (M506YLight->hvr.z * HuCos(M506YLight->hvr.y) * HuCos(M506YLight->hvr.x));
    M506YLight->target.y = M506YLight->center.y + (M506YLight->hvr.z * HuSin(M506YLight->hvr.y));
    M506YLight->target.z = M506YLight->center.z + (M506YLight->hvr.z * HuCos(M506YLight->hvr.y) * HuSin(M506YLight->hvr.x));
    M506YLight->pos = M506YLight->target;
    PSVECSubtract((Vec *)&M506YLight->center, (Vec *)&M506YLight->target, (Vec *)&M506YLight->dir);
    PSVECNormalize((Vec *)&M506YLight->dir, (Vec *)&M506YLight->dir);
    Hu3DGLightPosSetV(M506YLight->lightId, &M506YLight->pos, &M506YLight->dir);
    Hu3DGLightColorSet(M506YLight->lightId, M506YLight->color.r, M506YLight->color.g, M506YLight->color.b, M506YLight->color.a);
}

void M506YShadowInit(void)
{
    int i;

    M506YShadow = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M506_YSHADOW_WORK), HU_MEMNUM_OVL);
    if (M506YShadow == NULL) {
        OSReport("M506YShadowInit Error...\n");
        return;
    }
    Hu3DShadowMultiCreate(30.0f, 20.0f, 30000.0f, M506CameraBit[0] | M506CameraBit[1] | M506CameraBit[2] | M506CameraBit[3]);
    for (i = 0; i < M506_PLAYER_MAX; i++) {
        M506_YSHADOW *sh = &M506YShadow->shadow[i];
        sh->bit = M506CameraBit[i];
        sh->tpLvl = 0.3f;
        sh->hvr.x = -70.0f;
        sh->hvr.y = 50.0f;
        sh->hvr.z = 2000.0f;
        sh->center.x = 0.0f;
        sh->center.y = 0.0f;
        sh->center.z = 0.0f;
    }
    M506YShadowPosUpdate();
}

void M506YShadowClose(void)
{
    HuMemDirectFree(M506YShadow);
}

static void M506YShadowCalcPos(M506_YSHADOW *sh)
{
    sh->center = sh->pos;
    sh->pos.x = sh->center.x + (sh->tpLvl * HuCos(sh->hvr.y) * HuCos(sh->hvr.x));
    sh->pos.y = sh->center.y + (sh->tpLvl * HuSin(sh->hvr.y));
    sh->pos.z = sh->center.z + (sh->tpLvl * HuCos(sh->hvr.y) * HuSin(sh->hvr.x));
    PSVECSubtract((Vec *)&sh->center, (Vec *)&sh->pos, (Vec *)&sh->forward);
    PSVECNormalize((Vec *)&sh->forward, (Vec *)&sh->forward);
    PSVECCrossProduct((Vec *)&upVecB, (Vec *)&sh->forward, (Vec *)&sh->upDir);
    PSVECNormalize((Vec *)&sh->upDir, (Vec *)&sh->upDir);
    PSVECCrossProduct((Vec *)&sh->forward, (Vec *)&sh->upDir, (Vec *)&sh->up);
    PSVECNormalize((Vec *)&sh->up, (Vec *)&sh->up);
    Hu3DShadowMultiTPLvlSet(sh->tpLvl, (s16)sh->bit);
    Hu3DShadowMultiPosSet(&sh->pos, &sh->up, &sh->center, (s16)sh->bit);
}

// NONMATCHING: faithful translation, not yet instruction-matched
void M506YShadowPosUpdate(void)
{
    int i;
    for (i = 0; i < M506_PLAYER_MAX; i++) {
        M506YShadowCalcPos(&M506YShadow->shadow[i]);
    }
}

// NONMATCHING: faithful translation, not yet instruction-matched
void M506YShadowUpdate(void)
{
    int i;

    for (i = 0; i < M506_PLAYER_MAX; i++) {
        M506_YSHADOW *sh = &M506YShadow->shadow[i];
        sh->hvr = M506PlayerObj[i]->trans;
    }
    for (i = 0; i < M506_PLAYER_MAX; i++) {
        M506YShadowCalcPos(&M506YShadow->shadow[i]);
    }
}

// NONMATCHING: faithful translation, not yet instruction-matched
void M506YCameraViewSet(s32 camNo, s32 viewNo)
{
    const M506_YCAMERA_VIEW *view = &YCameraView[viewNo];
    M506_YCAMERA *cam = &M506YCamera->camera[camNo];
    int i;

    cam->hvr.x = view->hvr[0];
    cam->hvr.y = view->hvr[1];
    cam->hvr.z = view->hvr[2];
    cam->pos.x = view->pos[0];
    cam->pos.y = view->pos[1];
    cam->pos.z = view->pos[2];
    for (i = 0; i < M506_PLAYER_MAX; i++) {
        M506_YCAMERA *c = &M506YCamera->camera[i];
        c->target.x = c->pos.x;
        c->target.y = c->pos.y;
        c->target.z = c->pos.z;
        c->center.x = c->target.x + (c->hvr.z * HuCos(c->hvr.y) * HuCos(c->hvr.x));
        c->center.y = c->target.y + (c->hvr.z * HuSin(c->hvr.y));
        c->center.z = c->target.z + (c->hvr.z * HuCos(c->hvr.y) * HuSin(c->hvr.x));
        PSVECSubtract((Vec *)&c->target, (Vec *)&c->center, (Vec *)&c->dir);
        PSVECNormalize((Vec *)&c->dir, (Vec *)&c->dir);
        PSVECCrossProduct((Vec *)&upVecA, (Vec *)&c->dir, (Vec *)&c->upDir);
        PSVECNormalize((Vec *)&c->upDir, (Vec *)&c->upDir);
        PSVECCrossProduct((Vec *)&c->dir, (Vec *)&c->upDir, (Vec *)&c->up);
        PSVECNormalize((Vec *)&c->up, (Vec *)&c->up);
        Hu3DCameraPosSet(c->bit, c->center.x, c->center.y, c->center.z,
            c->up.x, c->up.y, c->up.z, c->target.x, c->target.y, c->target.z);
    }
}

// NONMATCHING: faithful translation, not yet instruction-matched
BOOL M506YCameraViewLerp(s32 camNo, s32 viewNo)
{
    const M506_YCAMERA_VIEW *view = &YCameraView[viewNo];
    M506_YCAMERA *cam = &M506YCamera->camera[camNo];
    f32 duration = view->duration;
    int result;
    int i;

    result = M506PointLerp(&cam->hvr.x, cam->hvr.x, view->targetHvr[0], duration);
    result += M506PointLerp(&cam->hvr.y, cam->hvr.y, view->targetHvr[1], duration);
    result += M506PointLerp(&cam->hvr.z, cam->hvr.z, view->targetHvr[2], duration);
    result += M506PointLerp(&cam->pos.x, cam->pos.x, view->targetPos[0], duration);
    result += M506PointLerp(&cam->pos.y, cam->pos.y, view->targetPos[1], duration);
    result += M506PointLerp(&cam->pos.z, cam->pos.z, view->targetPos[2], duration);
    for (i = 0; i < M506_PLAYER_MAX; i++) {
        M506_YCAMERA *c = &M506YCamera->camera[i];
        c->target.x = c->pos.x;
        c->target.y = c->pos.y;
        c->target.z = c->pos.z;
        c->center.x = c->target.x + (c->hvr.z * HuCos(c->hvr.y) * HuCos(c->hvr.x));
        c->center.y = c->target.y + (c->hvr.z * HuSin(c->hvr.y));
        c->center.z = c->target.z + (c->hvr.z * HuCos(c->hvr.y) * HuSin(c->hvr.x));
        PSVECSubtract((Vec *)&c->target, (Vec *)&c->center, (Vec *)&c->dir);
        PSVECNormalize((Vec *)&c->dir, (Vec *)&c->dir);
        PSVECCrossProduct((Vec *)&upVecA, (Vec *)&c->dir, (Vec *)&c->upDir);
        PSVECNormalize((Vec *)&c->upDir, (Vec *)&c->upDir);
        PSVECCrossProduct((Vec *)&c->dir, (Vec *)&c->upDir, (Vec *)&c->up);
        PSVECNormalize((Vec *)&c->up, (Vec *)&c->up);
        Hu3DCameraPosSet(c->bit, c->center.x, c->center.y, c->center.z,
            c->up.x, c->up.y, c->up.z, c->target.x, c->target.y, c->target.z);
    }
    return result;
}

// NONMATCHING: faithful translation, not yet instruction-matched
s32 M506PointLerp(f32 *val, f32 cur, f32 target, f32 duration)
{
    if (*val != target) {
        *val += (target - cur) / duration;
        if ((cur > target && *val < target) || (cur < target && *val > target)) {
            *val = target;
            return 0;
        }
        return 1;
    }
    return 0;
}

void M506MakeRotMtx(Mtx dst, HuVecF *rot)
{
    Mtx mtxX, mtxY, mtxZ;
    PSMTXRotRad(mtxX, 'X', 0.017453292f * rot->x);
    PSMTXRotRad(mtxY, 'Y', 0.017453292f * rot->y);
    PSMTXRotRad(mtxZ, 'Z', 0.017453292f * rot->z);
    PSMTXConcat(mtxZ, mtxX, dst);
    PSMTXConcat(dst, mtxY, dst);
}

// NONMATCHING: faithful translation, not yet instruction-matched
void M506SeedUpdate(void)
{
    HU3DPARTICLE *particle;
    HU3DPARTICLEDATA *data;
    M506_FLOW_WORK *flow = &M506Flow;
    int i;

    for (;;) {
        switch (flow->readyFlag) {
        case 0:
            break;
        case 1:
            SeedInitF = 0;
            flow->readyFlag = 2;
            break;
        case 2:
            flow->unk120 += 0.016666668f;
            flow->unkFC -= 3.0f * flow->unk120;
            flow->unk100 += 2.1f;
            if (SeedInitF == 0 && flow->unkFC < 93.5f) {
                particle = Hu3DData[flow->particleMdlId].hookData;
                data = particle->data;
                for (i = 0; i < particle->maxCnt; i++) {
                    data[i].time = 1;
                }
                SeedInitF = 1;
            }
            if (flow->unkFC < 13.5f) {
                flow->unkFC = 13.5f;
                SeedInitF = 0;
                flow->readyFlag = 3;
            }
            Hu3DModelPosSet(flow->work->seedMdlId, 0.0f, flow->unkFC, 0.0f);
            Hu3DModelRotSet(flow->work->seedMdlId, flow->unk100, flow->unk100, 0.0f);
            break;
        }
        HuPrcVSleep();
    }
}
