#include "REL/resultDll.h"

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

#ifdef __MWERKS__
#define RESULT_RODATA __declspec(section ".rodata")
#else
#define RESULT_RODATA
#endif

void ObjectSetup(void);
void Result_Main(void);
void Result_CoinMdlMain(void);
void Result_PartyCreate(void);
void Result_StoryCreate(void);
void Result_CoinCount(void);
void Result_UpdateRank(void);
void Result_AddBonusCoin(void);
void Result_GetCoin(s16 *coin);
void Result_Debug(void);
void Result_BgRead(void);
void Result_SetupPlayer(void);
void Result_SetScoreSprNum(HUSPRGRPID grpId, s16 coinNum);
void Result_SetBonusSprNum(HUSPRGRPID grpId, s16 coinNum);
void Result_BoardSave(void);
s16 Result_CharMotEnd(s16 playerNo);
void Result_BattleMain(void);
void Result_TeamMain(void);

s32 Result_ReqSort;
s32 Result_AllComF;
static s16 ToueiPlayerOrder[4];
static s16 PlayerRankEnd[4];
static s16 PlayerRankStart[4];
static s16 Result_CharNo[4];
static float CoinRotSpeed[4];
static float StarRotSpeed[4];
static s16 BonusSprId[4][5];
static s16 BonusSprGrpId[4];
s16 lbl_1_bss_78;
static s16 PlayerRank[4];
static s16 PlayerOrder[4];
static s16 RankSprGrpId;
static s16 CrownSprGrpId;
static s16 ScoreSprId[8][4];
static s16 ScoreSprGrpId[8];
static ANIMDATA *CrownAnimP;
u8 lbl_1_bss_E4[0x9c];
static ANIMDATA *RankAnimP;
s16 Result_RandomCoinPlayer;
s32 Result_FastF;
s32 Result_BgReadEndF;
s16 Result_MgNo;
OMOBJMAN *objman;
static OMOBJ *outview;

static OMCAMERAVIEW ToueiCameraView = { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, 1600.0f };
static OMCAMERAVIEW MainCameraView = { { 0.0f, 200.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, 1600.0f };
static OMCAMERAVIEW FGCameraView = { { 0.0f, 200.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, 1600.0f };
STAGE_MODEL Result_PartyModel[0xB] = {
    {
        0xB90016,
        0x40000001,
        0,
        -1,
        -1,
        1,
        2,
        { 250.0f, 150.0f, 1600.0f },
        { 0.0f, 0.0f, 0.0f },
        { 0.6f, 0.6f, 0.6f },
    },
    {
        0xB90016,
        0x40000001,
        2,
        0,
        -1,
        1,
        2,
        { 250.0f, 225.0f, 1600.0f },
        { 0.0f, 0.0f, 0.0f },
        { 0.6f, 0.6f, 0.6f },
    },
    {
        0xB90016,
        0x40000001,
        2,
        0,
        -1,
        1,
        2,
        { 250.0f, 300.0f, 1600.0f },
        { 0.0f, 0.0f, 0.0f },
        { 0.6f, 0.6f, 0.6f },
    },
    {
        0xB90016,
        0x40000001,
        2,
        0,
        -1,
        1,
        2,
        { 250.0f, 375.0f, 1600.0f },
        { 0.0f, 0.0f, 0.0f },
        { 0.6f, 0.6f, 0.6f },
    },
    {
        0xB90017,
        0x40000001,
        0,
        -1,
        -1,
        1,
        2,
        { 420.0f, 150.0f, 1600.0f },
        { 0.0f, 0.0f, 0.0f },
        { 0.53f, 0.53f, 0.53f },
    },
    {
        0xB90017,
        0x40000001,
        2,
        4,
        -1,
        1,
        2,
        { 420.0f, 225.0f, 1600.0f },
        { 0.0f, 0.0f, 0.0f },
        { 0.53f, 0.53f, 0.53f },
    },
    {
        0xB90017,
        0x40000001,
        2,
        4,
        -1,
        1,
        2,
        { 420.0f, 300.0f, 1600.0f },
        { 0.0f, 0.0f, 0.0f },
        { 0.53f, 0.53f, 0.53f },
    },
    {
        0xB90017,
        0x40000001,
        2,
        4,
        -1,
        1,
        2,
        { 420.0f, 375.0f, 1600.0f },
        { 0.0f, 0.0f, 0.0f },
        { 0.53f, 0.53f, 0.53f },
    },
    {
        0xB90018,
        0x40000001,
        0,
        -1,
        -1,
        0,
        2,
        { 288.0f, 240.0f, 2500.0f },
        { 0.0f, 0.0f, 0.0f },
        { 1.0f, 1.0f, 1.0f },
    },
    {
        0xB9001F,
        0x40000001,
        0,
        -1,
        -1,
        0,
        1,
        { 300.0f, 800.0f, 1600.0f },
        { 0.0f, 0.0f, 0.0f },
        { 1.0f, 2.0f, 2.0f },
    },
    {
        0xFFFFFFFF,
        0,
        0,
        -1,
        -1,
        -1,
        0,
        { 0.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f },
        { 1.0f, 1.0f, 1.0f },
    },
};
STAGE_MODEL Result_StoryModel[7] = {
    {
        0xB90017,
        0x40000001,
        0,
        -1,
        -1,
        1,
        2,
        { 340.0f, 150.0f, 1600.0f },
        { 0.0f, 0.0f, 0.0f },
        { 0.53f, 0.53f, 0.53f },
    },
    {
        0xB90017,
        0x40000001,
        2,
        0,
        -1,
        1,
        2,
        { 340.0f, 225.0f, 1600.0f },
        { 0.0f, 0.0f, 0.0f },
        { 0.53f, 0.53f, 0.53f },
    },
    {
        0xB90017,
        0x40000001,
        2,
        0,
        -1,
        1,
        2,
        { 340.0f, 300.0f, 1600.0f },
        { 0.0f, 0.0f, 0.0f },
        { 0.53f, 0.53f, 0.53f },
    },
    {
        0xB90017,
        0x40000001,
        2,
        0,
        -1,
        1,
        2,
        { 340.0f, 375.0f, 1600.0f },
        { 0.0f, 0.0f, 0.0f },
        { 0.53f, 0.53f, 0.53f },
    },
    {
        0xB9001C,
        0x40000001,
        0,
        -1,
        -1,
        0,
        2,
        { 288.0f, 240.0f, 2500.0f },
        { 0.0f, 0.0f, 0.0f },
        { 1.0f, 1.0f, 1.0f },
    },
    {
        0xB9001F,
        0x40000001,
        0,
        -1,
        -1,
        0,
        1,
        { 300.0f, 800.0f, 1600.0f },
        { 0.0f, 0.0f, 0.0f },
        { 1.0f, 2.0f, 2.0f },
    },
    {
        0xFFFFFFFF,
        0,
        0,
        -1,
        -1,
        -1,
        0,
        { 0.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f },
        { 1.0f, 1.0f, 1.0f },
    },
};
STAGE_SPRITE Result_Sprite[6] = {
    { 0xB90006, 0, 0x3C, 288.0f, 70.0f, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { 0xB9000D, 0, 0x14, 288.0f, 150.0f, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { 0xB9000D, 0, 0x14, 288.0f, 225.0f, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { 0xB9000D, 0, 0x14, 288.0f, 300.0f, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { 0xB9000D, 0, 0x14, 288.0f, 375.0f, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { 0, 0, 0, 0.0f, 0.0f, { 0, 0, 0, 0 } },
};

s32 _prolog(void)
{
    const VoidFunc *ctor;
    for(ctor = _ctors; *ctor != 0; ctor++) {
        (*ctor)();
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const VoidFunc *dtor;
    for(dtor = _dtors; *dtor != 0; dtor++) {
        (*dtor)();
    }
}


void ObjectSetup(void)
{
    u8 unused[4] = { 20, 0, 0, 0 };
    s32 i;
    s16 light;
    s32 partyF;

    OSReport("*******  RESULT ObjectSetup *********\n");
    objman = omInitObjMan(50, 0x2000);
    Result_MgNo = GwSystem.mgNo;
    for(i=0; i<4; i++) {
        GwPlayerConf[i].grp = GwPlayer[i].grp;
    }
    if (_CheckFlag(0x10010)) {
        omOvlReturnEx(1, 1);
        return;
    }
    partyF = GwSystem.partyF;
    if (partyF == 0) {
        omOvlReturnEx(1, 1);
        return;
    }
    switch (mgDataTbl[GwSystem.mgNo].type) {
        case MG_TYPE_KUPA:
        case MG_TYPE_KETTOU:
        case MG_TYPE_DONKEY:
            omOvlReturnEx(1, 1);
            return;
    }
    if (Result_MgNo <= -1) {
        Result_MgNo = 0;
    }
    HuDataDirClose(mgDataTbl[Result_MgNo].dataDir);
    Hu3DCameraCreate(7);
    Hu3DCameraPerspectiveSet(1, 20.0f, 20.0f, 15000.0f, 1.2f);
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(1, 0, 0, 640, 480);
    Hu3DCameraPerspectiveSet(2, 20.0f, 20.0f, 15000.0f, 1.2f);
    Hu3DCameraViewportSet(2, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(2, 0, 0, 640, 480);
    Hu3DCameraPerspectiveSet(4, 20.0f, 20.0f, 15000.0f, 1.2f);
    Hu3DCameraViewportSet(4, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(4, 0, 0, 640, 480);
    omCameraViewSetMulti(1, &ToueiCameraView);
    omCameraViewSetMulti(2, &MainCameraView);
    omCameraViewSetMulti(4, &FGCameraView);
    outview = omAddObjEx(objman, 0x7FDA, 100, 50, -1, omOutViewMulti);
    outview->work[0] = 3;
    light = Hu3DGLightCreate(0.0f, 100.0f, 1000.0f, 0.0f, -0.5f, -1.0f, 255, 255, 255);
    Hu3DGLightInfinitytSet(light);
    Result_FastF = 0;
    if (mgDataTbl[GwSystem.mgNo].type != MG_TYPE_BATTLE) {
        if ((s32)GwSystem.teamF != 0) {
            HuPrcChildCreate(Result_TeamMain, 100, 0x3000, 0, objman);
        } else {
            HuPrcChildCreate(Result_Main, 100, 0x3000, 0, objman);
        }
    } else {
        HuPrcChildCreate(Result_BattleMain, 100, 0x3000, 0, objman);
    }
    HuWinInit(1);
    Result_RandomCoinPlayer = frandmod(4);
}

static u32 MgTypeBackFileTbl[9] = { 0xB90018, 0xB9001B, 0xB90019, 0xB90018, 0xB9001C, 0xB9001C, 0xB9001A, 0xB90018, 0xB90018 };
static s16 ScorePosTbl[16] = { 0x154, 0x96, 0x154, 0xE1, 0x154, 0x12C, 0x154, 0x177, 0x1FE, 0x96, 0x1FE, 0xE1, 0x1FE, 0x12C, 0x1FE, 0x177 };
static s16 RankPosTbl[8] = { 0x3C, 0x96, 0x3C, 0xE1, 0x3C, 0x12C, 0x3C, 0x177 };
static s16 CrownPosTbl[8] = { 0x86, 0x6E, 0x86, 0xB9, 0x86, 0x104, 0x86, 0x14F };
static s16 BonusPosTbl[8] = { 0xAA, 0x96, 0xAA, 0xE1, 0xAA, 0x12C, 0xAA, 0x177 };
static HuVecF ToueiPosTbl[4] = {
    { 133.0f, 150.0f, 1600.0f },
    { 133.0f, 225.0f, 1600.0f },
    { 133.0f, 300.0f, 1600.0f },
    { 133.0f, 375.0f, 1600.0f },
};
static HuVecF ToueiCharPosTbl[4] = {
    { 240.0f, 250.0f, 1700.0f },
    { 240.0f, 400.0f, 1700.0f },
    { 320.0f, 250.0f, 1700.0f },
    { 320.0f, 400.0f, 1700.0f },
};
static HuVecF ToueiCharScaleTbl[13] = {
    { 0.7f, 0.7f, 0.7f }, { 0.7f, 0.7f, 0.7f }, { 0.7f, 0.7f, 0.7f }, { 0.7f, 0.7f, 0.7f },
    { 0.6f, 0.6f, 0.6f }, { 0.7f, 0.7f, 0.7f }, { 0.7f, 0.7f, 0.7f }, { 0.7f, 0.7f, 0.7f },
    { 0.5f, 0.5f, 0.5f }, { 0.6f, 0.6f, 0.6f }, { 0.6f, 0.6f, 0.6f }, { 0.6f, 0.6f, 0.6f },
    { 0.6f, 0.6f, 0.6f },
};

void Result_Main(void)
{
    HUPROCESS *prcP;
    s16 cnt;
    s32 exitF;
    HUWINID winId;
    s16 i;
    s32 partyF;

    exitF = 0;
    prcP = HuPrcCurrentGet();
    Result_Debug();
    cnt = 0;
    for(i=cnt; i<4; i++) {
        if (GwPlayerConf[i].type != 0) {
            cnt++;
        }
    }
    if (cnt == 4) {
        Result_AllComF = 1;
    } else {
        Result_AllComF = 0;
    }
    for(i=0; i<4; i++) {
        StarRotSpeed[i] = 1.0f;
        CoinRotSpeed[i] = 1.0f;
    }
    partyF = GwSystem.partyF;
    if (partyF == 0) {
        Result_StoryCreate();
    } else {
        Result_PartyCreate();
    }
    HuPrcChildCreate(Result_CoinMdlMain, 100, 0x2000, 0, objman);
    WipeCreate(1, 0, 20);
    HuAudSStreamPlay(0x31);
    while (WipeCheckEnd()) {
        HuPrcVSleep();
    }
    HuDataDirClose(0xB90000);
    Result_BgReadEndF = 0;
    HuPrcChildCreate(Result_BgRead, 100, 0x3000, 0, objman);
    HuPrcSleep(10);
    if (Result_FastF == 0) {
        HuPrcSleep(20);
    }
    Result_CoinCount();
    Result_AddBonusCoin();
    Result_UpdateRank();
    winId = HuWinCreate(-10000.0f, 400.0f, 320, 40, 0);
    HuWinMesSpeedSet(winId, 0);
    HuWinBGTPLvlSet(winId, 0.0f);
    HuWinPriSet(winId, 5);
    HuWinAttrSet(winId, 0x800);
    if (Result_AllComF == 0) {
        HuWinMesSet(winId, 0x2000D);
    }
    HuPrcSleep(4);
    while (1) {
        if (Result_AllComF != 0) {
            exitF = 1;
        } else {
            for(i=0; i<4; i++) {
                if (HuPadStatGet(i) == 0) {
                    if (HuPadBtnDown[i] & 0x100) {
                        exitF = 1;
                        break;
                    }
                }
            }
        }
        if (exitF != 0) {
            break;
        }
        HuPrcVSleep();
    }
    HuWinKill(winId);
    WipeCreate(2, 0, 60);
    HuAudSStreamAllFadeOut(1000);
    while (WipeCheckEnd()) {
        HuPrcVSleep();
    }
    Result_BoardSave();
    ToueiDispClose();
    omSysPauseEnable(TRUE);
    HuDataDirClose(0xB90000);
    omOvlReturnEx(1, 1);
    HuPrcEnd();
    while (1) {
        HuPrcVSleep();
    }
}

void Result_CoinMdlMain(void)
{
    float coinRot[4];
    float starRot[4];
    float unused;
    s16 i;
    s32 partyF;

    unused = 0.0f;
    Result_ReqSort = 0;
    for(i=0; i<4; i++) {
        coinRot[i] = 0.0f;
        starRot[i] = 0.0f;
    }
    while (1) {
        if (Result_AllComF != 0) {
            Result_FastF = 1;
        } else {
            for(i=0; i<4; i++) {
                if (HuPadStatGet(i) == 0) {
                    if (HuPadBtn[i] & 0x100) {
                        Result_FastF = 1;
                    } else {
                        Result_FastF = 0;
                    }
                }
            }
        }
        for(i=0; i<4; i++) {
            coinRot[i] += 1.2f * CoinRotSpeed[i];
            if (coinRot[i] >= 360.0f) {
                coinRot[i] -= 360.0f;
            }
            starRot[i] += StarRotSpeed[i];
            if (starRot[i] >= 360.0f) {
                starRot[i] -= 360.0f;
            }
        }
        partyF = GwSystem.partyF;
        if (partyF == 0) {
            Hu3DModelRotSet(StageMdlId[0], 0.0f, starRot[0], 0.0f);
            Hu3DModelRotSet(StageMdlId[1], 0.0f, starRot[1], 0.0f);
            Hu3DModelRotSet(StageMdlId[2], 0.0f, starRot[2], 0.0f);
            Hu3DModelRotSet(StageMdlId[3], 0.0f, starRot[3], 0.0f);
        } else {
            Hu3DModelRotSet(StageMdlId[0], 0.0f, coinRot[0], 0.0f);
            Hu3DModelRotSet(StageMdlId[1], 0.0f, coinRot[1], 0.0f);
            Hu3DModelRotSet(StageMdlId[2], 0.0f, coinRot[2], 0.0f);
            Hu3DModelRotSet(StageMdlId[3], 0.0f, coinRot[3], 0.0f);
            Hu3DModelRotSet(StageMdlId[4], 0.0f, starRot[0], 0.0f);
            Hu3DModelRotSet(StageMdlId[5], 0.0f, starRot[1], 0.0f);
            Hu3DModelRotSet(StageMdlId[6], 0.0f, starRot[2], 0.0f);
            Hu3DModelRotSet(StageMdlId[7], 0.0f, starRot[3], 0.0f);
        }
        HuPrcVSleep();
    }
}

void Result_PartyCreate(void)
{
    HuVecF pos;
    s16 coin[5];
    ANIMDATA *scoreAnim;
    ANIMDATA *bonusNumAnim;
    ANIMDATA *bonusSmallAnim;
    ANIMDATA *bonusBigAnim;
    s16 i;

    Result_PartyModel[8].dataNum = MgTypeBackFileTbl[mgDataTbl[Result_MgNo].type];
    StageSpriteCreate(Result_Sprite);
    StageModelCreate(Result_PartyModel);
    Hu3DMotionSpeedSet(StageMdlId[8], 0.5f);
    scoreAnim = HuSprAnimDataRead(0xB90001);
    for(i=0; i<8; i++) {
        ScoreSprGrpId[i] = HuSprGrpCreate(4);
        ScoreSprId[i][0] = HuSprCreate(scoreAnim, 0, 0);
        HuSprGrpMemberSet(ScoreSprGrpId[i], 0, ScoreSprId[i][0]);
        HuSprPosSet(ScoreSprGrpId[i], 0, -52.0f, 0.0f);
        ScoreSprId[i][1] = HuSprCreate(scoreAnim, 0, 0);
        HuSprGrpMemberSet(ScoreSprGrpId[i], 1, ScoreSprId[i][1]);
        HuSprPosSet(ScoreSprGrpId[i], 1, -26.0f, 0.0f);
        ScoreSprId[i][2] = HuSprCreate(scoreAnim, 0, 0);
        HuSprGrpMemberSet(ScoreSprGrpId[i], 2, ScoreSprId[i][2]);
        HuSprPosSet(ScoreSprGrpId[i], 2, 0.0f, 0.0f);
        ScoreSprId[i][3] = HuSprCreate(scoreAnim, 0, 0);
        HuSprGrpMemberSet(ScoreSprGrpId[i], 3, ScoreSprId[i][3]);
        HuSprPosSet(ScoreSprGrpId[i], 3, 26.0f, 0.0f);
        HuSprGrpPosSet(ScoreSprGrpId[i], ScorePosTbl[i*2], ScorePosTbl[i*2+1]);
        Result_SetScoreSprNum(ScoreSprGrpId[i], 0);
    }
    Result_SetupPlayer();
    Result_GetCoin(coin);
    bonusNumAnim = HuSprAnimDataRead(0xB90003);
    bonusSmallAnim = HuSprAnimDataRead(0xB90007);
    bonusBigAnim = HuSprAnimDataRead(0xB90008);
    for(i=0; i<4; i++) {
        BonusSprGrpId[i] = HuSprGrpCreate(5);
        if (coin[PlayerOrder[i]] < 100) {
            BonusSprId[i][0] = HuSprCreate(bonusSmallAnim, 10, 0);
            HuSprGrpMemberSet(BonusSprGrpId[i], 0, BonusSprId[i][0]);
            HuSprPosSet(BonusSprGrpId[i], 0, 30.0f, -22.0f);
        } else {
            BonusSprId[i][0] = HuSprCreate(bonusBigAnim, 10, 0);
            HuSprGrpMemberSet(BonusSprGrpId[i], 0, BonusSprId[i][0]);
            HuSprPosSet(BonusSprGrpId[i], 0, 36.0f, -22.0f);
        }
        BonusSprId[i][1] = HuSprCreate(bonusNumAnim, 0, 0);
        HuSprGrpMemberSet(BonusSprGrpId[i], 1, BonusSprId[i][1]);
        HuSprPosSet(BonusSprGrpId[i], 1, 12.0f, -25.0f);
        HuSprBankSet(BonusSprGrpId[i], 1, 10);
        BonusSprId[i][2] = HuSprCreate(bonusNumAnim, 0, 0);
        HuSprGrpMemberSet(BonusSprGrpId[i], 2, BonusSprId[i][2]);
        HuSprPosSet(BonusSprGrpId[i], 2, 28.0f, -25.0f);
        BonusSprId[i][3] = HuSprCreate(bonusNumAnim, 0, 0);
        HuSprGrpMemberSet(BonusSprGrpId[i], 3, BonusSprId[i][3]);
        HuSprPosSet(BonusSprGrpId[i], 3, 44.0f, -25.0f);
        BonusSprId[i][4] = HuSprCreate(bonusNumAnim, 0, 0);
        HuSprGrpMemberSet(BonusSprGrpId[i], 4, BonusSprId[i][4]);
        HuSprPosSet(BonusSprGrpId[i], 4, 60.0f, -25.0f);
        HuSprGrpPosSet(BonusSprGrpId[i], BonusPosTbl[i*2], BonusPosTbl[i*2+1]);
        HuSprGrpScaleSet(BonusSprGrpId[i], 0.0f, 0.0f);
    }
    CrownAnimP = HuSprAnimDataRead(0xB90009);
    CrownSprGrpId = HuSprGrpCreate(4);
    for(i=0; i<4; i++) {
        HuSprGrpMemberSet(CrownSprGrpId, i, HuSprCreate(CrownAnimP, i, 0));
        HuSprScaleSet(CrownSprGrpId, i, 0.0f, 0.0f);
        HuSprPosSet(CrownSprGrpId, i, CrownPosTbl[i*2], CrownPosTbl[i*2+1]);
        HuSprAttrSet(CrownSprGrpId, i, 4);
    }
    RankAnimP = HuSprAnimDataRead(0xB90005);
    RankSprGrpId = HuSprGrpCreate(4);
    for(i=0; i<4; i++) {
        HuSprGrpMemberSet(RankSprGrpId, i, HuSprCreate(RankAnimP, i, 0));
        HuSprPosSet(RankSprGrpId, i, RankPosTbl[i*2], RankPosTbl[i*2+1]);
    }
    for(i=0; i<4; i++) {
        s16 charNo;
        if (i == 0) {
            StageToueiMdlId[0] = Hu3DModelCreateData(0xB9001E);
        } else {
            StageToueiMdlId[i] = Hu3DModelLink(StageToueiMdlId[0]);
        }
        Hu3D2Dto3D(&ToueiPosTbl[i], 2, &pos);
        Hu3DModelScaleSet(StageToueiMdlId[i], 0.37f, 0.37f, 0.37f);
        Hu3DModelLayerSet(StageToueiMdlId[i], 1);
        Hu3DModelPosSetV(StageToueiMdlId[i], &pos);
        Hu3DModelCameraSet(StageToueiMdlId[i], 2);
        Result_CharNo[i] = GwPlayerConf[PlayerOrder[i]].charNo;
        ToueiPlayerOrder[PlayerOrder[i]] = i;
        charNo = Result_CharNo[i]*4;
        StageCharMdlId[i] = Hu3DModelCreateData(charNo+0xB90020);
        StageCharMotId[i][0] = Hu3DJointMotionData(StageCharMdlId[i], charNo+0xB90021);
        StageCharMotId[i][1] = Hu3DJointMotionData(StageCharMdlId[i], charNo+0xB90022);
        StageCharMotId[i][2] = Hu3DJointMotionData(StageCharMdlId[i], charNo+0xB90023);
        Hu3D2Dto3D(&ToueiCharPosTbl[i], 1, &pos);
        Hu3DMotionSet(StageCharMdlId[i], StageCharMotId[i][0]);
        Hu3DModelScaleSet(StageCharMdlId[i], ToueiCharScaleTbl[Result_CharNo[i]].x, ToueiCharScaleTbl[Result_CharNo[i]].y, ToueiCharScaleTbl[Result_CharNo[i]].z);
        Hu3DModelLayerSet(StageCharMdlId[i], 0);
        Hu3DModelPosSetV(StageCharMdlId[i], &pos);
        Hu3DModelAttrSet(StageCharMdlId[i], 0x40000001);
        Hu3DModelCameraSet(StageCharMdlId[i], 1);
    }
    ToueiDispInit(Result_CharNo);
    for(i=0; i<4; i++) {
        Result_SetScoreSprNum(ScoreSprGrpId[i], MBPlayerStarGet(PlayerOrder[i]));
        Result_SetScoreSprNum(ScoreSprGrpId[i+4], MBPlayerCoinGet(PlayerOrder[i]));
        PlayerRankStart[i] = MBPlayerRankGet(i);
    }
    for(i=0; i<4; i++) {
        HuSprBankSet(RankSprGrpId, i, PlayerRankStart[PlayerOrder[i]]);
    }
}

void Result_StoryCreate(void)
{
    HuVecF pos;
    HuVecF touei;
    s16 coin[5];
    ANIMDATA *scoreAnim;
    ANIMDATA *bonusNumAnim;
    ANIMDATA *bonusSmallAnim;
    ANIMDATA *bonusBigAnim;
    s16 i;

    for(i=0; i<4; i++) {
        Result_Sprite[i+1].dataNum = 0xB9000E;
    }
    Result_StoryModel[4].dataNum = MgTypeBackFileTbl[mgDataTbl[Result_MgNo].type];
    StageSpriteCreate(Result_Sprite);
    StageModelCreate(Result_StoryModel);
    Hu3DMotionSpeedSet(StageMdlId[4], 0.5f);
    scoreAnim = HuSprAnimDataRead(0xB90001);
    for(i=4; i<8; i++) {
        ScoreSprGrpId[i] = HuSprGrpCreate(4);
        ScoreSprId[i][0] = HuSprCreate(scoreAnim, 0, 0);
        HuSprGrpMemberSet(ScoreSprGrpId[i], 0, ScoreSprId[i][0]);
        HuSprPosSet(ScoreSprGrpId[i], 0, -52.0f, 0.0f);
        ScoreSprId[i][1] = HuSprCreate(scoreAnim, 0, 0);
        HuSprGrpMemberSet(ScoreSprGrpId[i], 1, ScoreSprId[i][1]);
        HuSprPosSet(ScoreSprGrpId[i], 1, -26.0f, 0.0f);
        ScoreSprId[i][2] = HuSprCreate(scoreAnim, 0, 0);
        HuSprGrpMemberSet(ScoreSprGrpId[i], 2, ScoreSprId[i][2]);
        HuSprPosSet(ScoreSprGrpId[i], 2, 0.0f, 0.0f);
        ScoreSprId[i][3] = HuSprCreate(scoreAnim, 0, 0);
        HuSprGrpMemberSet(ScoreSprGrpId[i], 3, ScoreSprId[i][3]);
        HuSprPosSet(ScoreSprGrpId[i], 3, 26.0f, 0.0f);
        HuSprGrpPosSet(ScoreSprGrpId[i], ScorePosTbl[i*2]-75, ScorePosTbl[i*2+1]);
        Result_SetScoreSprNum(ScoreSprGrpId[i], 0);
    }
    Result_SetupPlayer();
    Result_GetCoin(coin);
    bonusNumAnim = HuSprAnimDataRead(0xB90003);
    bonusSmallAnim = HuSprAnimDataRead(0xB90007);
    bonusBigAnim = HuSprAnimDataRead(0xB90008);
    for(i=0; i<4; i++) {
        BonusSprGrpId[i] = HuSprGrpCreate(5);
        if (coin[PlayerOrder[i]] < 100) {
            BonusSprId[i][0] = HuSprCreate(bonusSmallAnim, 10, 0);
            HuSprGrpMemberSet(BonusSprGrpId[i], 0, BonusSprId[i][0]);
            HuSprPosSet(BonusSprGrpId[i], 0, 30.0f, -22.0f);
        } else {
            BonusSprId[i][0] = HuSprCreate(bonusBigAnim, 10, 0);
            HuSprGrpMemberSet(BonusSprGrpId[i], 0, BonusSprId[i][0]);
            HuSprPosSet(BonusSprGrpId[i], 0, 36.0f, -22.0f);
        }
        BonusSprId[i][1] = HuSprCreate(bonusNumAnim, 0, 0);
        HuSprGrpMemberSet(BonusSprGrpId[i], 1, BonusSprId[i][1]);
        HuSprPosSet(BonusSprGrpId[i], 1, 12.0f, -25.0f);
        BonusSprId[i][2] = HuSprCreate(bonusNumAnim, 0, 0);
        HuSprGrpMemberSet(BonusSprGrpId[i], 2, BonusSprId[i][2]);
        HuSprPosSet(BonusSprGrpId[i], 2, 28.0f, -25.0f);
        BonusSprId[i][3] = HuSprCreate(bonusNumAnim, 0, 0);
        HuSprGrpMemberSet(BonusSprGrpId[i], 3, BonusSprId[i][3]);
        HuSprPosSet(BonusSprGrpId[i], 3, 44.0f, -25.0f);
        BonusSprId[i][4] = HuSprCreate(bonusNumAnim, 0, 0);
        HuSprGrpMemberSet(BonusSprGrpId[i], 4, BonusSprId[i][4]);
        HuSprPosSet(BonusSprGrpId[i], 4, 60.0f, -25.0f);
        HuSprGrpPosSet(BonusSprGrpId[i], BonusPosTbl[i*2]+75, BonusPosTbl[i*2+1]);
        HuSprGrpScaleSet(BonusSprGrpId[i], 0.0f, 0.0f);
    }
    CrownAnimP = HuSprAnimDataRead(0xB90009);
    CrownSprGrpId = HuSprGrpCreate(4);
    for(i=0; i<4; i++) {
        HuSprGrpMemberSet(CrownSprGrpId, i, HuSprCreate(CrownAnimP, i, 0));
        HuSprScaleSet(CrownSprGrpId, i, 0.0f, 0.0f);
        HuSprPosSet(CrownSprGrpId, i, CrownPosTbl[i*2]+75, CrownPosTbl[i*2+1]);
        HuSprAttrSet(CrownSprGrpId, i, 4);
    }
    RankAnimP = HuSprAnimDataRead(0xB90005);
    RankSprGrpId = HuSprGrpCreate(4);
    for(i=0; i<4; i++) {
        HuSprGrpMemberSet(RankSprGrpId, i, HuSprCreate(RankAnimP, i, 0));
        HuSprPosSet(RankSprGrpId, i, RankPosTbl[i*2]+70, RankPosTbl[i*2+1]);
    }
    for(i=0; i<4; i++) {
        s16 charNo;
        if (i == 0) {
            StageToueiMdlId[0] = Hu3DModelCreateData(0xB9001E);
        } else {
            StageToueiMdlId[i] = Hu3DModelLink(StageToueiMdlId[0]);
        }
        touei.x = 75.0f+ToueiPosTbl[i].x;
        touei.y = ToueiPosTbl[i].y;
        touei.z = ToueiPosTbl[i].z;
        Hu3D2Dto3D(&touei, 2, &pos);
        Hu3DModelScaleSet(StageToueiMdlId[i], 0.37f, 0.37f, 0.37f);
        Hu3DModelLayerSet(StageToueiMdlId[i], 1);
        Hu3DModelPosSetV(StageToueiMdlId[i], &pos);
        Hu3DModelCameraSet(StageToueiMdlId[i], 2);
        Result_CharNo[i] = GwPlayerConf[PlayerOrder[i]].charNo;
        ToueiPlayerOrder[PlayerOrder[i]] = i;
        charNo = Result_CharNo[i]*4;
        StageCharMdlId[i] = Hu3DModelCreateData(charNo+0xB90020);
        StageCharMotId[i][0] = Hu3DJointMotionData(StageCharMdlId[i], charNo+0xB90021);
        StageCharMotId[i][1] = Hu3DJointMotionData(StageCharMdlId[i], charNo+0xB90022);
        StageCharMotId[i][2] = Hu3DJointMotionData(StageCharMdlId[i], charNo+0xB90023);
        Hu3D2Dto3D(&ToueiCharPosTbl[i], 1, &pos);
        Hu3DMotionSet(StageCharMdlId[i], StageCharMotId[i][0]);
        Hu3DModelScaleSet(StageCharMdlId[i], ToueiCharScaleTbl[Result_CharNo[i]].x, ToueiCharScaleTbl[Result_CharNo[i]].y, ToueiCharScaleTbl[Result_CharNo[i]].z);
        Hu3DModelLayerSet(StageCharMdlId[i], 0);
        Hu3DModelPosSetV(StageCharMdlId[i], &pos);
        Hu3DModelAttrSet(StageCharMdlId[i], 0x40000001);
        Hu3DModelCameraSet(StageCharMdlId[i], 1);
    }
    ToueiDispInit(Result_CharNo);
    for(i=0; i<4; i++) {
        Result_SetScoreSprNum(ScoreSprGrpId[i+4], MBPlayerCoinGet(PlayerOrder[i]));
        PlayerRankStart[i] = MBPlayerRankGet(i);
    }
    for(i=0; i<4; i++) {
        HuSprBankSet(RankSprGrpId, i, PlayerRankStart[PlayerOrder[i]]);
    }
}

void Result_CoinCount(void)
{
    s16 seCnt = 0;
    s32 seF[4] = { 0, 0, 0, 0 };
    float seTimer[4];
    s16 coin[5];
    float scale;
    float scale2;
    float count;
    float seStep;
    s16 i;
    s16 cnt;
    s16 n;
    s16 frame;
    s16 j;

    Result_GetCoin(coin);
    for(i=0; i<4; i++) {
        Result_SetBonusSprNum(BonusSprGrpId[i], coin[PlayerOrder[i]]);
        if (coin[PlayerOrder[i]] != 0) {
            seF[i] = 1;
        }
        seTimer[i] = 0.0f;
    }
    n = Result_FastF ? 3 : 10;
    for(frame=0; frame<=n; frame++) {
        scale = sin(3.141592653589793 * (90.0 / n * frame) / 180.0);
        for(i=0; i<4; i++) {
            HuSprGrpScaleSet(BonusSprGrpId[i], scale, scale);
        }
        HuPrcVSleep();
    }
    for(i=0; i<4; i++) {
        HuSprGrpScaleSet(BonusSprGrpId[i], 1.0f, 1.0f);
    }
    for(frame=0; frame<=14; ) {
        for(i=0; i<4; i++) {
            if (GwPlayer[PlayerOrder[i]].mgCoinBonus >= 10) {
                HuSprAttrReset(CrownSprGrpId, i, 4);
            }
            HuSprScaleSet(CrownSprGrpId, i,
                0.1f + sin(3.141592653589793 * (8.571428f * frame) / 180.0) * (0.8 * (1.0 / sin(2.0943951023931953))),
                0.1f + 0.8 * (sin(3.141592653589793 * (8.571428f * frame) / 180.0) * (1.0 / sin(2.0943951023931953))));
        }
        frame += Result_FastF ? 2 : 1;
        HuPrcVSleep();
    }
    if (Result_FastF == 0) {
        HuPrcSleep(15);
    }
    cnt = 0;
    for(i=cnt; i<4; i++) {
        cnt += coin[i];
    }
    cnt = 0;
    count = cnt;
    cnt = 0;
    while (1) {
        count += Result_FastF ? 1.0f : 0.2f;
        seStep = Result_FastF ? 1.0f : 0.2f;
        cnt = count;
        n = 0;
        for(i=n; i<4; i++) {
            seTimer[i] += seStep;
            if (coin[PlayerOrder[i]] - cnt < 0) {
                StarRotSpeed[i] = 1.0f;
                seF[i] = 0;
                n++;
                Result_SetBonusSprNum(BonusSprGrpId[i], 0);
                Result_SetScoreSprNum(ScoreSprGrpId[i+4], coin[PlayerOrder[i]] + MBPlayerCoinGet(PlayerOrder[i]));
            } else {
                if (seF[i] != 0) {
                    if (seTimer[i] >= 1.0f) {
                        HuAudFXPlay(7);
                        seTimer[i] = 0.0f;
                        seCnt++;
                    }
                }
                Result_SetBonusSprNum(BonusSprGrpId[i], coin[PlayerOrder[i]] - cnt);
                Result_SetScoreSprNum(ScoreSprGrpId[i+4], cnt + MBPlayerCoinGet(PlayerOrder[i]));
                StarRotSpeed[i] = 3.0f;
            }
        }
        if (n == 4) {
            break;
        }
        HuPrcVSleep();
    }
    if (n < 30) {
        HuPrcSleep(30 - n);
    }
    n = Result_FastF ? 3 : 10;
    for(frame=0; frame<=n; frame++) {
        scale2 = cos(3.141592653589793 * (90.0 / n * frame) / 180.0);
        for(i=0; i<4; i++) {
            HuSprGrpScaleSet(BonusSprGrpId[i], scale2, scale2);
            HuSprScaleSet(CrownSprGrpId, i, scale2, scale2);
        }
        HuPrcVSleep();
    }
    for(i=0; i<4; i++) {
        for(j=0; j<5; j++) {
            HuSprAttrSet(BonusSprGrpId[i], j, 4);
        }
        HuSprAttrSet(CrownSprGrpId, i, 4);
    }
}

#define PARTY_TEST (GwSystem.partyF == 1)
void Result_UpdateRank(void)
{
    float scale[4];
    HuVecF pos;
    HuVecF touei;
    s32 digit[3];
    s32 unused[4] = { -1, -1, -1, -1 };
    s32 delay[4] = { 120, 90, 60, 30 };
    s32 shrinkF[4] = { 1, 1, 1, 1 };
    s32 doneF[4];
    s32 motF[4];
    s16 modelId[4];
    s16 endF[4];
    s16 order[4];
    s16 coin[4];
    float speed;
    s16 delayStep;
    s16 i;
    s16 sum;
    s16 j;
    s16 frame;
    s32 rankDone;
    s16 rankFlag;
    s32 coinNum;

    rankDone = 0;
    for(i=0; i<4; i++) {
        PlayerRankEnd[i] = MBPlayerRankGet(i);
        order[i] = PlayerOrder[i];
        coin[i] = MBPlayerCoinGet(PlayerOrder[i]);
        scale[i] = 0.37f;
        modelId[i] = StageToueiMdlId[i];
        endF[i] = 0;
        motF[i] = 0;
    }
    Result_SetupPlayer();
    for(i=0; i<4; i++) {
        if (order[i] == PlayerOrder[i]) {
            doneF[i] = 1;
        } else {
            doneF[i] = 0;
        }
    }
    if (i == -1) {
        Result_ReqSort = 1;
        return;
    }
    sum = 0;
    rankFlag = 0;
    frame = 0;
    while (1) {
        speed = Result_FastF ? 0.0296f : 0.0148f;
        delayStep = Result_FastF ? 2 : 1;
        if (Result_FastF != 0) {
            frame += 3;
        } else {
            frame += 1;
        }
        for(i=3; i>-1; i--) {
            if (rankFlag == 0) {
                rankFlag = 1;
            }
            if (doneF[i] == 0) {
                if ((frame % 2) != 0) {
                    for(j=0; j<3; j++) {
                        digit[j] = frandmod(10);
                        if (digit[j] == 0) {
                            digit[j] = frandmod(9) + 1;
                        }
                    }
                    coinNum = digit[0]*100 + (digit[1]*10 + digit[2]);
                    if (PARTY_TEST) {
                        Result_SetScoreSprNum(ScoreSprGrpId[i], coinNum);
                    }
                    Result_SetScoreSprNum(ScoreSprGrpId[i+4], coinNum);
                    StarRotSpeed[i] = 4.0f;
                    CoinRotSpeed[i] = 4.0f;
                }
                if (shrinkF[i] != 0) {
                    if ((scale[i] -= speed) <= 0.0f) {
                        scale[i] = 0.001f;
                        StageToueiMdlId[i] = modelId[ToueiPlayerOrder[PlayerOrder[i]]];
                        if (!PARTY_TEST) {
                            touei.x = 75.0f + ToueiPosTbl[i].x;
                            touei.y = ToueiPosTbl[i].y;
                            touei.z = ToueiPosTbl[i].z;
                            Hu3D2Dto3D(&touei, 2, &pos);
                        } else {
                            Hu3D2Dto3D(&ToueiPosTbl[i], 2, &pos);
                        }
                        Hu3DModelScaleSet(StageToueiMdlId[i], scale[i], scale[i], scale[i]);
                        Hu3DModelPosSetV(StageToueiMdlId[i], &pos);
                        shrinkF[i] = 0;
                    }
                } else {
                    if ((delay[i] -= delayStep) <= 0) {
                        if ((scale[i] += speed) >= 0.37f) {
                            0.37f == scale[i];
                            if (PARTY_TEST) {
                                Result_SetScoreSprNum(ScoreSprGrpId[i], MBPlayerStarGet(PlayerOrder[i]));
                            }
                            Result_SetScoreSprNum(ScoreSprGrpId[i+4], MBPlayerCoinGet(PlayerOrder[i]));
                            StarRotSpeed[i] = 1.0f;
                            CoinRotSpeed[i] = 1.0f;
                            doneF[i] = 1;
                        }
                    }
                }
                Hu3DModelScaleSet(StageToueiMdlId[i], scale[i], scale[i], scale[i]);
            } else {
                endF[i] = 1;
            }
        }
        sum = 0;
        for(i=0; i<4; i++) {
            sum += endF[i];
        }
        if (sum >= 4 && rankDone == 0) {
            for(i=0; i<4; i++) {
                if (PlayerRankStart[i] > PlayerRankEnd[i]) {
                    Hu3DMotionShiftSet(StageCharMdlId[ToueiPlayerOrder[i]], StageCharMotId[ToueiPlayerOrder[i]][1], 0.0f, 8.0f, 0);
                    PlayerFXPlay(i, 0x1F8);
                    motF[ToueiPlayerOrder[i]] = 1;
                } else if (PlayerRankStart[i] < PlayerRankEnd[i]) {
                    Hu3DMotionShiftSet(StageCharMdlId[ToueiPlayerOrder[i]], StageCharMotId[ToueiPlayerOrder[i]][2], 0.0f, 8.0f, 0);
                    motF[ToueiPlayerOrder[i]] = 1;
                }
                HuSprBankSet(RankSprGrpId, i, PlayerRankEnd[PlayerOrder[i]]);
            }
            rankDone = 1;
        }
        sum = 0;
        for(i=sum; i<4; i++) {
            if (rankDone != 0 && motF[i] != 0 && Result_CharMotEnd(i) != 0) {
                Hu3DMotionShiftSet(StageCharMdlId[i], StageCharMotId[i][0], 0.0f, 8.0f, 0x40000001);
                motF[i] = 0;
            }
            sum += motF[i];
        }
        if (rankDone != 0) {
            if (sum <= 0) {
                break;
            }
        }
        HuPrcVSleep();
    }
    Result_ReqSort = 1;
}

#undef PARTY_TEST

void Result_SetScoreSprNum(HUSPRGRPID grpId, s16 coinNum)
{
    s16 num;
    s16 i;

    if (coinNum > 999) {
        coinNum = 999;
    }
    i = 0;
    HuSprBankSet(grpId, i, 10);
    HuSprAttrReset(grpId, i, 4);
    i++;
    num = coinNum / 100;
    if (num != 0) {
        HuSprBankSet(grpId, i, num);
        HuSprAttrReset(grpId, i, 4);
        i++;
    }
    coinNum -= num * 100;
    num = coinNum / 10;
    if (num != 0 || i == 2) {
        HuSprBankSet(grpId, i, num);
        HuSprAttrReset(grpId, i, 4);
        i++;
    }
    coinNum -= num * 10;
    HuSprBankSet(grpId, i, coinNum);
    HuSprAttrReset(grpId, i, 4);
    i++;
    num = i;
    while (num < 4) {
        HuSprAttrSet(grpId, num, 4);
        num++;
    }
}

void Result_SetBonusSprNum(HUSPRGRPID grpId, s16 coinNum)
{
    s16 num;
    s16 i;

    if (coinNum > 999) {
        coinNum = 999;
    }
    i = 1;
    HuSprBankSet(grpId, i, 10);
    HuSprAttrReset(grpId, i, 4);
    i++;
    num = coinNum / 100;
    if (num != 0) {
        HuSprBankSet(grpId, i, num);
        HuSprAttrReset(grpId, i, 4);
        i++;
    }
    coinNum -= num * 100;
    num = coinNum / 10;
    if (num != 0 || i == 3) {
        HuSprBankSet(grpId, i, num);
        HuSprAttrReset(grpId, i, 4);
        i++;
    }
    coinNum -= num * 10;
    HuSprBankSet(grpId, i, coinNum);
    HuSprAttrReset(grpId, i, 4);
    i++;
    num = i;
    while (num < 5) {
        HuSprAttrSet(grpId, num, 4);
        num++;
    }
}

void Result_SetupPlayer(void)
{
    s16 tmp;
    s16 i2;
    s16 i;

    for(i=0; i<4; i++) {
        PlayerRank[i] = MBPlayerRankGet(i);
        PlayerOrder[i] = i;
    }
    for(i2=1; i2<4; i2++) {
        for(i=0; i<4-i2; i++) {
            if (PlayerRank[i] > PlayerRank[i+1]) {
                tmp = PlayerRank[i];
                PlayerRank[i] = PlayerRank[i+1];
                PlayerRank[i+1] = tmp;
                tmp = PlayerOrder[i];
                PlayerOrder[i] = PlayerOrder[i+1];
                PlayerOrder[i+1] = tmp;
            }
        }
    }
}

static s32 MBDataDir[3] = { 0xCB0000, 0xCD0000, 0xCF0000 };

void Result_BgRead(void)
{
    s32 statId;
    if (_CheckFlag(0x10000) == 0) {
        Result_BgReadEndF = 1;
        HuPrcEnd();
        while (1) {
            HuPrcVSleep();
        }
    }
    statId = HuDataDirReadAsync(MBDataDir[GwSystem.boardNo]);
    if (statId != -1) {
        while (HuDataGetAsyncStat(statId) == 0) {
            HuPrcVSleep();
        }
    }
    Result_BgReadEndF = 1;
    HuPrcEnd();
    while (1) {
        HuPrcVSleep();
    }
}

void Result_AddBonusCoin(void)
{
    s16 i;
    s16 j;
    s16 coinAdd = 0;
    s16 cnt;
    s16 k;
    s16 tmp;
#ifdef TARGET_PC
    s16 randPlayer = -1; // uninitialised on GC when no player won the random bonus
#else
    s16 randPlayer;
#endif
    s16 val2;
    s16 val1;
    s16 teamIdx[2] = { -1, -1 };
    s16 teamCoin[2] = { 0, 0 };
    s16 coin[5];
    s16 rankSort[4];
    s16 rank[4];
    s16 order[4];

    for(i=0; i<5; i++) {
        coin[i] = 0;
    }
    if (mgDataTbl[GwSystem.mgNo].type == MG_TYPE_BATTLE) {
        for(i=0; i<4; i++) {
            cnt = 0;
            for(j=cnt; j<4; j++) {
                if (i != j) {
                    val2 = GwPlayer[j].mgCoinBonus;
                    val1 = GwPlayer[i].mgCoinBonus;
                    if (val1 > val2) {
                        cnt++;
                    }
                }
            }
            rankSort[i] = cnt;
            rank[i] = cnt;
            order[i] = i;
        }
        for(k=1; k<4; k++) {
            for(i=0; i<4-k; i++) {
                if (rank[i] > rank[i+1]) {
                    tmp = rank[i];
                    rank[i] = rank[i+1];
                    rank[i+1] = tmp;
                    tmp = order[i];
                    order[i] = order[i+1];
                    order[i+1] = tmp;
                }
            }
        }
        for(i=0; i<4; i++) {
            if (Result_RandomCoinPlayer == i) {
                randPlayer = order[i];
            }
        }
        Result_GetCoin(coin);
        if ((s32)GwSystem.teamF != 0) {
            for(i=0; i<4; i++) {
                coinAdd = coin[rankSort[i]];
                if (randPlayer == i) {
                    coinAdd += coin[4];
                }
                teamCoin[GwPlayer[i].grp] += coinAdd;
            }
            for(i=0; i<2; i++) {
                if (teamIdx[i] == -1) {
                    for(j=0; j<4; j++) {
                        if (i == GwPlayer[j].grp) {
                            teamIdx[i] = j;
                            break;
                        }
                    }
                }
            }
            for(i=0; i<2; i++) {
                GWPlayerCoinAdd(teamIdx[i], teamCoin[i]);
                GwPlayer[teamIdx[i]].coinTotalMg += teamCoin[i];
                if (GwPlayer[teamIdx[i]].coinTotalMg > 9999) {
                    GwPlayer[teamIdx[i]].coinTotalMg = 9999;
                }
            }
            for(i=0; i<4; i++) {
                if (_CheckFlag(0x1000F) == 0) {
                    GwPlayer[i].mgCoinBonus = 0;
                }
                GwPlayer[i].mgCoin = 0;
            }
            return;
        }
        for(i=0; i<4; i++) {
            coinAdd = coin[rankSort[i]];
            if (randPlayer == i) {
                coinAdd += coin[4];
            }
            GWPlayerCoinAdd(i, coinAdd);
            if (_CheckFlag(0x1000F) == 0) {
                GwPlayer[i].mgCoinBonus = 0;
            }
            GwPlayer[i].mgCoin = 0;
            GwPlayer[i].coinTotalMg += coinAdd;
            if (GwPlayer[i].coinTotalMg > 9999) {
                GwPlayer[i].coinTotalMg = 9999;
            }
        }
        return;
    }
    Result_GetCoin(coin);
    if ((s32)GwSystem.teamF != 0) {
        for(i=0; i<4; i++) {
            teamCoin[GwPlayer[i].grp] += coin[i];
        }
        for(i=0; i<2; i++) {
            if (teamIdx[i] == -1) {
                for(j=0; j<4; j++) {
                    if (i == GwPlayer[j].grp) {
                        teamIdx[i] = j;
                        break;
                    }
                }
            }
        }
        for(i=0; i<2; i++) {
            GWPlayerCoinAdd(teamIdx[i], teamCoin[i]);
            GwPlayer[teamIdx[i]].coinTotalMg += teamCoin[i];
            if (GwPlayer[teamIdx[i]].coinTotalMg > 9999) {
                GwPlayer[teamIdx[i]].coinTotalMg = 9999;
            }
        }
        for(i=0; i<4; i++) {
            if (_CheckFlag(0x1000F) == 0) {
                GwPlayer[i].mgCoinBonus = 0;
            }
            GwPlayer[i].mgCoin = 0;
        }
        return;
    }
    for(i=0; i<4; i++) {
        GWPlayerCoinAdd(i, coin[i]);
        if (_CheckFlag(0x1000F) == 0) {
            GwPlayer[i].mgCoinBonus = 0;
        }
        GwPlayer[i].mgCoin = 0;
        GwPlayer[i].coinTotalMg += coin[i];
        if (GwPlayer[i].coinTotalMg > 9999) {
            GwPlayer[i].coinTotalMg = 9999;
        }
    }
}

void Result_GetCoin(s16 *coin)
{
    float ratio[4];
    s16 sorted[4];
    s16 rank[4];
    s16 i;
    s16 j;
    u32 total;
    s16 tmp;
    u32 given;
    s16 type;

    type = mgDataTbl[GwSystem.mgNo].type;
    switch (type) {
    case MG_TYPE_BATTLE:
        total = 0;
        for(i=total; i<4; i++) {
            total += GwPlayer[i].coinBattle;
        }
        sorted[0] = sorted[1] = sorted[2] = sorted[3] = 0;
        rank[0] = rank[1] = rank[2] = rank[3] = 0;
        for(i=0; i<4; i++) {
            sorted[i] = GwPlayer[i].mgCoinBonus;
        }
        for(i=0; i<4; i++) {
            for(j=i+1; j<4; j++) {
                if (sorted[i] > sorted[j]) {
                    tmp = sorted[i];
                    sorted[i] = sorted[j];
                    sorted[j] = tmp;
                }
            }
        }
        rank[0] = 0;
        tmp = 0;
        for(i=1; i<4; i++) {
            if (sorted[i] != sorted[i-1]) {
                tmp++;
            }
            rank[i] = tmp;
        }
        ratio[0] = ratio[1] = ratio[2] = ratio[3] = 0.0f;
        if (rank[0] == 0 && rank[1] == 1 && rank[2] == 2 && rank[3] == 3) {
            ratio[0] = 0.7f;
            ratio[1] = 0.3f;
        } else if (rank[0] == 0 && rank[1] == 0 && rank[2] == 1 && rank[3] == 2) {
            ratio[0] = 0.5f;
            ratio[1] = 0.5f;
        } else if (rank[0] == 0 && rank[1] == 0 && rank[2] == 1 && rank[3] == 1) {
            ratio[0] = 0.5f;
            ratio[1] = 0.5f;
        } else if (rank[0] == 0 && rank[1] == 0 && rank[2] == 0 && rank[3] == 1) {
            ratio[0] = 0.3f;
            ratio[1] = 0.3f;
            ratio[2] = 0.3f;
        } else if (rank[0] == 0 && rank[1] == 1 && rank[2] == 1 && rank[3] == 2) {
            ratio[0] = 0.6f;
            ratio[1] = 0.2f;
            ratio[2] = 0.2f;
        } else if (rank[0] == 0 && rank[1] == 1 && rank[2] == 1 && rank[3] == 1) {
            ratio[0] = 0.7f;
            ratio[1] = 0.1f;
            ratio[2] = 0.1f;
            ratio[3] = 0.1f;
        } else if (rank[0] == 0 && rank[1] == 1 && rank[2] == 2 && rank[3] == 2) {
            ratio[0] = 0.7f;
            ratio[1] = 0.3f;
        } else if (rank[0] == 0 && rank[1] == 0 && rank[2] == 0 && rank[3] == 0) {
            ratio[0] = 0.25f;
            ratio[1] = 0.25f;
            ratio[2] = 0.25f;
            ratio[3] = 0.25f;
        }
        given = 0;
        for(i=given; i<4; i++) {
            u32 add = (u32)(total * ratio[i]);
            coin[i] = add;
            given += add;
        }
        if (given < total) {
            coin[4] = total - given;
        } else {
            coin[4] = 0;
        }
        break;
    default:
        for(i=0; i<4; i++) {
            coin[i] = GwPlayer[i].mgCoinBonus + GwPlayer[i].mgCoin;
        }
        break;
    }
}

s16 Result_CharMotEnd(s16 playerNo)
{
    if (Hu3DMotionShiftIDGet(StageCharMdlId[playerNo]) == -1) {
        if (Hu3DMotionEndCheck(StageCharMdlId[playerNo]) != 0) {
            return 1;
        }
    }
    return 0;
}

void Result_BoardSave(void)
{
    s16 turnNo;
    s16 i;

    turnNo = GwSystem.turnNo;
    for(i=0; i<4; i++) {
        GwPlayer[i].starStat[turnNo-1] = GwPlayer[i].star;
        GwPlayer[i].coinStat[turnNo-1] = GwPlayer[i].coin;
    }
    SLSaveBoardTurnExec();
    if (SLSaveFlagGet() == 0) {
        GwSystem.saveMode = 1;
    }
}

void Result_Debug(void)
{
    s16 coin[5];
    s16 rank[4];
    s16 total;
    s32 i;

    if (mgDataTbl[GwSystem.mgNo].type != MG_TYPE_BATTLE) {
        OSReport("\n");
        OSReport("--------------Result BonusCoin-------------- \n");
        for(i=0; i<4; i++) {
            coin[i] = GwPlayer[i].mgCoinBonus + GwPlayer[i].mgCoin;
            OSReport("Player=%d  BonusCoin=%d \n", i, coin[i]);
        }
        OSReport("-------------------------------------------- \n");
        OSReport("\n");
        return;
    }
    OSReport("\n");
    OSReport("--------------BATTLE_MINIGAME------------- \n");
    OSReport("--------------Result GetCoin-------------- \n");
    Result_GetCoin(coin);
    total = 0;
    for(i=total; i<5; i++) {
        total += coin[i];
    }
    OSReport("TotalCoin=%d \n", total);
    for(i=0; i<4; i++) {
        rank[i] = GwPlayer[i].mgCoinBonus;
        OSReport("Player=%d  Rank=%d  GetCoin=%d \n", i, rank[i], coin[rank[i]]);
    }
    OSReport("RandomCoin=%d \n", coin[4]);
    OSReport("-------------------------------------------- \n");
    OSReport("\n");
}
