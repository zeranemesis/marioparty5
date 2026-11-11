#include "REL/resultDll.h"

double sin(double x);
double cos(double x);

#ifdef TARGET_PC
#include "game/esprite.h"
#else
/* the original calls these without prototypes (float arguments promoted to double) */
void espTPLvlSet();
void espPosSet();
int espGrpIDGet();
#endif

extern s32 Result_FastF;
extern s32 Result_BgReadEndF;

void Result_Debug(void);
void Result_BgRead(void);
void Result_TeamCoinMdlMain(void);
void Result_TeamCreate(void);
void Result_TeamCoinCount(void);
void Result_TeamUpdateRank(void);
void Result_TeamUpdateOrder(void);
void Result_TeamSetTPLvl(s16 level, float scale);
u32 Result_TeamNameMessGet(s16 charNo1, s16 charNo2);
void Result_AddBonusCoin(void);
void Result_BoardSave(void);
void Result_GetCoin(s16 *coin);
void Result_SetScoreSprNum(HUSPRGRPID grpId, s16 coinNum);
void Result_SetBonusSprNum(HUSPRGRPID grpId, s16 coinNum);
s16 Result_CharMotEnd(s16 playerNo);

STAGE_MODEL Result_TeamModel[7] = {
    {
        0xB90016,
        0x40000001,
        0,
        -1,
        -1,
        1,
        2,
        { 240.0f, 210.0f, 1600.0f },
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
        { 240.0f, 360.0f, 1600.0f },
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
        { 410.0f, 210.0f, 1600.0f },
        { 0.0f, 0.0f, 0.0f },
        { 0.53f, 0.53f, 0.53f },
    },
    {
        0xB90017,
        0x40000001,
        2,
        2,
        -1,
        1,
        2,
        { 410.0f, 360.0f, 1600.0f },
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

STAGE_SPRITE Result_TeamSprite[4] = {
    { 0xB90006, 0, 0xA, 288.0f, 70.0f, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { 0xB9000A, 0, 0x14, 288.0f, 180.0f, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { 0xB9000B, 0, 0x14, 288.0f, 330.0f, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { 0, 0, 0, 0.0f, 0.0f, { 0, 0, 0, 0 } },
};

static u32 MgTypeBackFileTbl[9] = { 0xB90018, 0xB9001B, 0xB90019, 0xB90018, 0xB9001C, 0xB9001C, 0xB9001A, 0xB90018, 0xB90018 };
static s16 RankPosTbl[4] = { 0x3C, 0xB4, 0x3C, 0x14A };
static s16 BonusPosTbl[4] = { 0xA0, 0xB4, 0xA0, 0x14A };
static s16 ScorePosTbl[8] = { 0x14A, 0xD2, 0x14A, 0x168, 0x1F4, 0xD2, 0x1F4, 0x168 };
static s16 CrownPosTbl[4] = { 0x10C, 0xA0, 0x10C, 0x136 };
static HuVecF ToueiPosTbl[4] = {
    { 120.0f, 145.0f, 1600.0f },
    { 120.0f, 215.0f, 1600.0f },
    { 120.0f, 295.0f, 1600.0f },
    { 120.0f, 365.0f, 1600.0f },
};
static HuVecF ToueiCharPosTbl[4] = {
    { 240.0f, 250.0f, 1700.0f },
    { 240.0f, 400.0f, 1700.0f },
    { 320.0f, 250.0f, 1700.0f },
    { 320.0f, 400.0f, 1700.0f },
};
static s16 TeamBoxPosTbl[4] = { 0x1A4, 0x96, 0x1A4, 0x12C };
static HuVecF ToueiCharScaleTbl[13] = {
    { 0.7f, 0.7f, 0.7f }, { 0.7f, 0.7f, 0.7f }, { 0.7f, 0.7f, 0.7f }, { 0.7f, 0.7f, 0.7f },
    { 0.6f, 0.6f, 0.6f }, { 0.7f, 0.7f, 0.7f }, { 0.7f, 0.7f, 0.7f }, { 0.7f, 0.7f, 0.7f },
    { 0.5f, 0.5f, 0.5f }, { 0.6f, 0.6f, 0.6f }, { 0.6f, 0.6f, 0.6f }, { 0.6f, 0.6f, 0.6f },
    { 0.6f, 0.6f, 0.6f },
};
static HuVecF ScoreSprPosTbl[4] = {
    { -52.0f, 0.0f, 0.0f },
    { -26.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f },
    { 26.0f, 0.0f, 0.0f },
};
static HuVecF BonusSprPosTbl[4] = {
    { 12.0f, -25.0f, 0.0f },
    { 28.0f, -25.0f, 0.0f },
    { 44.0f, -25.0f, 0.0f },
    { 60.0f, -25.0f, 0.0f },
};
static u32 TeamFrameFileTbl[2] = { 0xB9000A, 0xB9000B };
static s16 TopWinMotTbl[4] = { 2, 2, 1, 1 };
static s16 BottomWinMotTbl[4] = { 1, 1, 2, 2 };
static u8 TeamNoTbl[90] = {
    0xFF, 0, 1, 2, 3, 4, 5, 6, 7,
    8, 0xFF, 0xFF, 9, 10, 11, 12, 13, 14,
    15, 16, 0xFF, 0xFF, 0xFF, 17, 18, 19, 20,
    21, 22, 23, 0xFF, 0xFF, 0xFF, 0xFF, 24, 25,
    26, 27, 28, 29, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    30, 31, 32, 33, 34, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 35, 36, 37, 38, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 39, 40, 41, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 42, 43, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 44,
};

static s32 TeamFastF;
static s16 TeamCharNo[4];
HuVecF lbl_1_bss_1FA4[2];
static HuVecF TeamNamePos[2];
float lbl_1_bss_1FD4[2];
static s32 TeamNameWinId[2];
static s16 TeamGrpNo[4];
static s16 TeamPlayerNo[4];
static s16 TeamOrigCharNo[4];
static s16 TeamOrder[2];
static s16 TeamReqSort;
static s16 TeamPlayerIdx[2];
static s16 TeamCoinNum[2];
static s16 TeamStarNum[2];
static s16 TeamCoinAddNum[2];
static s16 ToueiCharNo[4];
static float StarRotSpeed[2];
static float CoinRotSpeed[2];
u8 lbl_1_bss_202C[0x10];
static s16 TeamBoxSprGrpId;
static s16 CrownSprGrpId;
static s16 BonusSprId[2][5];
static s16 BonusSprGrpId[2];
static s16 ScoreSprId[4][4];
static s16 ScoreSprGrpId[4];
static s16 RankSprGrpId;
static ANIMDATA *TeamBoxAnimP[2];
static ANIMDATA *CrownAnimP;
static ANIMDATA *RankAnimP;

void Result_TeamMain(void)
{
    HUPROCESS *prcP;
    s16 cnt;
    s32 exitF;
    HUWINID winId;
    s16 i;

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
        TeamFastF = 1;
    } else {
        TeamFastF = 0;
    }
    for(i=0; i<2; i++) {
        StarRotSpeed[i] = 1.0f;
        CoinRotSpeed[i] = 1.0f;
    }
    Result_TeamCreate();
    HuPrcChildCreate(Result_TeamCoinMdlMain, 100, 0x2000, 0, prcP);
    WipeCreate(1, 0, 20);
    HuAudSStreamPlay(0x31);
    while (WipeCheckEnd()) {
        HuPrcVSleep();
    }
    HuDataDirClose(0xB90000);
    Result_BgReadEndF = 0;
    HuPrcChildCreate(Result_BgRead, 100, 0x3000, 0, prcP);
    HuPrcSleep(10);
    if (Result_FastF == 0) {
        HuPrcSleep(20);
    }
    Result_TeamCoinCount();
    Result_TeamUpdateRank();
    Result_AddBonusCoin();
    winId = HuWinCreate(-10000.0f, 400.0f, 320, 40, 0);
    HuWinMesSpeedSet(winId, 0);
    HuWinBGTPLvlSet(winId, 0.0f);
    HuWinPriSet(winId, 5);
    HuWinAttrSet(winId, 0x800);
    if (TeamFastF == 0) {
        HuWinMesSet(winId, 0x2000D);
    }
    HuPrcSleep(4);
    while (1) {
        if (TeamFastF != 0) {
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

void Result_TeamCoinMdlMain(void)
{
    float starRot[2];
    float coinRot[2];
    float f1;
    float f2;
    float f3;
    s16 i;

    f1 = 0.0f;
    f2 = 0.0f;
    f3 = 0.0f;
    for(i=0; i<2; i++) {
        starRot[i] = 0.0f;
        coinRot[i] = 0.0f;
    }
    while (1) {
        if (TeamFastF != 0) {
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
        for(i=0; i<2; i++) {
            starRot[i] += 1.2f * StarRotSpeed[i];
            if (starRot[i] >= 360.0f) {
                starRot[i] -= 360.0f;
            }
            coinRot[i] += CoinRotSpeed[i];
            if (coinRot[i] >= 360.0f) {
                coinRot[i] -= 360.0f;
            }
        }
        Hu3DModelRotSet(StageMdlId[0], 0.0f, starRot[0], 0.0f);
        Hu3DModelRotSet(StageMdlId[1], 0.0f, starRot[1], 0.0f);
        Hu3DModelRotSet(StageMdlId[2], 0.0f, coinRot[0], 0.0f);
        Hu3DModelRotSet(StageMdlId[3], 0.0f, coinRot[1], 0.0f);
        HuPrcVSleep();
    }
}

void Result_TeamCreate(void)
{
    HuVecF pos;
    s16 bank[2];
    s16 teamCnt[2];
    ANIMDATA *scoreAnim;
    ANIMDATA *numAnim;
    ANIMDATA *bonusAnim;
    s16 charNo;
    s16 idx1;
    s16 idx2;
    s16 coinTmp;
    s16 i;
    s16 j;

    TeamCoinAddNum[0] = TeamCoinAddNum[1] = 0;
    TeamCoinNum[0] = TeamCoinNum[1] = 0;
    TeamPlayerIdx[0] = 0;
    TeamPlayerIdx[1] = 2;
    teamCnt[0] = teamCnt[1] = 0;
    for(i=0; i<4; i++) {
        TeamGrpNo[i] = GwPlayer[i].grp;
        if (teamCnt[TeamGrpNo[i]] == 0) {
            TeamCoinAddNum[TeamGrpNo[i]] += MBPlayerCoinGet(i);
            coinTmp = MBPlayerCoinGet(i);
            TeamCoinNum[TeamGrpNo[i]] += MBPlayerStarGet(i);
            teamCnt[TeamGrpNo[i]]++;
        }
        TeamPlayerNo[TeamPlayerIdx[TeamGrpNo[i]]] = i;
        TeamOrigCharNo[TeamPlayerIdx[TeamGrpNo[i]]++] = GwPlayerConf[i].charNo;
    }
    for(i=0; i<4; i++) {
        OSReport("PlayerNo=%d  CharNo=%d  teamNo=%d  Star=%d  Coin=%d \n", i, GwPlayerConf[i].charNo, GwPlayer[i].grp, MBPlayerStarGet(i), MBPlayerCoinGet(i));
    }
    TeamOrder[0] = 0;
    TeamOrder[1] = 1;
    bank[0] = 0;
    bank[1] = 1;
    if (TeamCoinNum[0] > TeamCoinNum[1]) {
        TeamReqSort = 0;
    } else if (TeamCoinNum[0] == TeamCoinNum[1]) {
        if (TeamCoinAddNum[0] > TeamCoinAddNum[1]) {
            TeamReqSort = 0;
        } else if (TeamCoinAddNum[0] == TeamCoinAddNum[1]) {
            bank[1] = 0;
            bank[0] = 0;
            TeamReqSort = 0;
        } else {
            TeamOrder[0] = 1;
            TeamOrder[1] = 0;
            TeamReqSort = 1;
        }
    } else {
        TeamOrder[0] = 1;
        TeamOrder[1] = 0;
        TeamReqSort = 1;
    }
    if (TeamReqSort == 1) {
        Result_TeamSprite[1].dataNum = TeamFrameFileTbl[1];
        Result_TeamSprite[2].dataNum = TeamFrameFileTbl[0];
    }
    Result_TeamModel[4].dataNum = MgTypeBackFileTbl[mgDataTbl[GwSystem.mgNo].type];
    StageSpriteCreate(Result_TeamSprite);
    StageModelCreate(Result_TeamModel);
    Hu3DMotionSpeedSet(StageMdlId[4], 0.5f);
    scoreAnim = HuSprAnimDataRead(0xB90001);
    for(i=0; i<4; i++) {
        ScoreSprGrpId[i] = HuSprGrpCreate(4);
        for(j=0; j<4; j++) {
            ScoreSprId[i][j] = HuSprCreate(scoreAnim, 0, 0);
            HuSprGrpMemberSet(ScoreSprGrpId[i], j, ScoreSprId[i][j]);
            HuSprPosSet(ScoreSprGrpId[i], j, ScoreSprPosTbl[j].x, ScoreSprPosTbl[j].y);
        }
        HuSprGrpPosSet(ScoreSprGrpId[i], ScorePosTbl[i*2], ScorePosTbl[i*2+1]);
    }
    numAnim = HuSprAnimDataRead(0xB90003);
    bonusAnim = HuSprAnimDataRead(0xB90008);
    for(i=0; i<2; i++) {
        BonusSprGrpId[i] = HuSprGrpCreate(5);
        BonusSprId[i][0] = HuSprCreate(bonusAnim, 10, 0);
        HuSprGrpMemberSet(BonusSprGrpId[i], 0, BonusSprId[i][0]);
        HuSprPosSet(BonusSprGrpId[i], 0, 36.0f, -22.0f);
        for(j=1; j<5; j++) {
            BonusSprId[i][j] = HuSprCreate(numAnim, 0, 0);
            HuSprGrpMemberSet(BonusSprGrpId[i], j, BonusSprId[i][j]);
            HuSprPosSet(BonusSprGrpId[i], j, BonusSprPosTbl[j-1].x, BonusSprPosTbl[j-1].y);
        }
        HuSprBankSet(BonusSprGrpId[i], 1, 10);
        HuSprGrpPosSet(BonusSprGrpId[i], BonusPosTbl[i*2], BonusPosTbl[i*2+1]);
        HuSprGrpScaleSet(BonusSprGrpId[i], 0.0f, 0.0f);
    }
    CrownAnimP = HuSprAnimDataRead(0xB90009);
    CrownSprGrpId = HuSprGrpCreate(2);
    for(i=0; i<2; i++) {
        HuSprGrpMemberSet(CrownSprGrpId, i, HuSprCreate(CrownAnimP, i, 0));
        HuSprScaleSet(CrownSprGrpId, i, 0.0f, 0.0f);
        HuSprPosSet(CrownSprGrpId, i, CrownPosTbl[i*2], CrownPosTbl[i*2+1]);
        HuSprAttrSet(CrownSprGrpId, i, 4);
    }
    if (TeamReqSort == 1) {
        ToueiCharNo[0] = TeamOrigCharNo[2];
        ToueiCharNo[1] = TeamOrigCharNo[3];
        ToueiCharNo[2] = TeamOrigCharNo[0];
        ToueiCharNo[3] = TeamOrigCharNo[1];
    } else {
        ToueiCharNo[0] = TeamOrigCharNo[0];
        ToueiCharNo[1] = TeamOrigCharNo[1];
        ToueiCharNo[2] = TeamOrigCharNo[2];
        ToueiCharNo[3] = TeamOrigCharNo[3];
    }
    for(i=0; i<4; i++) {
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
        charNo = ToueiCharNo[i]*4;
        StageCharMdlId[i] = Hu3DModelCreateData(charNo+0xB90020);
        StageCharMotId[i][0] = Hu3DJointMotionData(StageCharMdlId[i], charNo+0xB90021);
        StageCharMotId[i][1] = Hu3DJointMotionData(StageCharMdlId[i], charNo+0xB90022);
        StageCharMotId[i][2] = Hu3DJointMotionData(StageCharMdlId[i], charNo+0xB90023);
        Hu3D2Dto3D(&ToueiCharPosTbl[i], 1, &pos);
        Hu3DMotionSet(StageCharMdlId[i], StageCharMotId[i][0]);
        Hu3DModelScaleSet(StageCharMdlId[i], ToueiCharScaleTbl[ToueiCharNo[i]].x, ToueiCharScaleTbl[ToueiCharNo[i]].y, ToueiCharScaleTbl[ToueiCharNo[i]].z);
        Hu3DModelLayerSet(StageCharMdlId[i], 0);
        Hu3DModelPosSetV(StageCharMdlId[i], &pos);
        Hu3DModelAttrSet(StageCharMdlId[i], 0x40000001);
        Hu3DModelCameraSet(StageCharMdlId[i], 1);
    }
    ToueiDispInit(ToueiCharNo);
    RankAnimP = HuSprAnimDataRead(0xB90005);
    RankSprGrpId = HuSprGrpCreate(2);
    for(i=0; i<2; i++) {
        HuSprGrpMemberSet(RankSprGrpId, i, HuSprCreate(RankAnimP, i, 0));
        HuSprPosSet(RankSprGrpId, i, RankPosTbl[i*2], RankPosTbl[i*2+1]);
        HuSprBankSet(RankSprGrpId, i, bank[i]);
    }
    if (TeamReqSort == 0) {
        idx2 = idx1 = 0;
        for(i=idx2; i<4; i++) {
            if (TeamGrpNo[i] == 0) {
                TeamCharNo[idx2] = GwPlayerConf[i].charNo;
                idx2++;
            } else if (TeamGrpNo[i] == 1) {
                TeamCharNo[2+idx1] = GwPlayerConf[i].charNo;
                idx1++;
            }
        }
    } else if (TeamReqSort == 1) {
        idx2 = idx1 = 0;
        for(i=idx2; i<4; i++) {
            if (TeamGrpNo[i] == 0) {
                TeamCharNo[2+idx2] = GwPlayerConf[i].charNo;
                idx2++;
            } else if (TeamGrpNo[i] == 1) {
                TeamCharNo[idx1] = GwPlayerConf[i].charNo;
                idx1++;
            }
        }
    }
    if (TeamReqSort == 1) {
        TeamBoxAnimP[0] = HuSprAnimDataRead(0xB90010);
        TeamBoxAnimP[1] = HuSprAnimDataRead(0xB9000F);
    } else {
        TeamBoxAnimP[0] = HuSprAnimDataRead(0xB9000F);
        TeamBoxAnimP[1] = HuSprAnimDataRead(0xB90010);
    }
    TeamBoxSprGrpId = HuSprGrpCreate(2);
    for(i=0; i<2; i++) {
        HuSprGrpMemberSet(TeamBoxSprGrpId, i, HuSprCreate(TeamBoxAnimP[i], i, 0));
        HuSprPosSet(TeamBoxSprGrpId, i, TeamBoxPosTbl[i*2], TeamBoxPosTbl[i*2+1]);
        TeamNamePos[i].x = 300.0f;
        TeamNamePos[i].y = TeamBoxPosTbl[i*2+1] - 5;
        TeamNameWinId[i] = HuWinCreate(TeamNamePos[i].x, TeamNamePos[i].y, 236, 35, 0);
        HuWinBGTPLvlSet(TeamNameWinId[i], 255.0f);
        HuWinPriSet(TeamNameWinId[i], 0);
        HuWinAttrSet(TeamNameWinId[i], 0x800);
        HuWinMesSpeedSet(TeamNameWinId[i], 0);
        HuWinMesSet(TeamNameWinId[i], Result_TeamNameMessGet(TeamCharNo[i*2], TeamCharNo[i*2+1]));
        lbl_1_bss_1FD4[i] = 0.0f;
        lbl_1_bss_1FA4[i].z = 1.0f;
        lbl_1_bss_1FA4[i].y = 1.0f;
        lbl_1_bss_1FA4[i].x = 1.0f;
    }
    Result_SetScoreSprNum(ScoreSprGrpId[0], TeamCoinNum[TeamOrder[0]]);
    Result_SetScoreSprNum(ScoreSprGrpId[1], TeamCoinNum[TeamOrder[1]]);
    Result_SetScoreSprNum(ScoreSprGrpId[2], TeamCoinAddNum[TeamOrder[0]]);
    Result_SetScoreSprNum(ScoreSprGrpId[3], TeamCoinAddNum[TeamOrder[1]]);
}

void Result_TeamCoinCount(void)
{
    s16 coin[5];
    s32 seF[2] = { 0, 0 };
    float seTimer[2];
    s16 teamCoin[2];
    s16 holdCoin[2];
    float scale;
    float scale2;
    float count;
    float seStep;
    s16 i;
    s16 cnt;
    s16 n;
    s16 frame;
    s16 j;

    teamCoin[0] = teamCoin[1] = 0;
    for(i=0; i<4; i++) {
        coin[i] = 0;
    }
    Result_GetCoin(coin);
    for(i=0; i<4; i++) {
        teamCoin[TeamGrpNo[i]] += coin[i];
    }
    for(i=0; i<2; i++) {
        Result_SetBonusSprNum(BonusSprGrpId[i], teamCoin[TeamOrder[i]]);
        if (teamCoin[TeamOrder[i]] > 0) {
            seF[i] = 1;
        }
        seTimer[i] = 0.0f;
    }
    n = Result_FastF ? 3 : 10;
    for(frame=0; frame<=n; frame++) {
        scale = sin(3.141592653589793 * (90.0 / n * frame) / 180.0);
        for(i=0; i<2; i++) {
            HuSprGrpScaleSet(BonusSprGrpId[i], scale, scale);
        }
        HuPrcVSleep();
    }
    for(i=0; i<2; i++) {
        HuSprGrpScaleSet(BonusSprGrpId[i], 1.0f, 1.0f);
    }
    for(frame=0; frame<=14; ) {
        for(i=0; i<2; i++) {
            if (teamCoin[TeamOrder[i]] >= 10) {
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
    for(i=cnt; i<2; i++) {
        holdCoin[i] = teamCoin[i];
        cnt += teamCoin[i];
    }
    cnt = 0;
    count = cnt;
    cnt = 0;
    while (1) {
        count += Result_FastF ? 1.0f : 0.2f;
        seStep = Result_FastF ? 1.0f : 0.2f;
        cnt = count;
        n = 0;
        for(i=0; i<2; i++) {
            seTimer[i] += seStep;
            if (holdCoin[TeamOrder[i]] - cnt <= 0) {
                CoinRotSpeed[i] = 1.0f;
                seF[i] = 0;
                n++;
                Result_SetBonusSprNum(BonusSprGrpId[i], 0);
                TeamStarNum[TeamOrder[i]] = teamCoin[TeamOrder[i]] + TeamCoinAddNum[TeamOrder[i]];
                Result_SetScoreSprNum(ScoreSprGrpId[i+2], TeamStarNum[TeamOrder[i]]);
            } else {
                if (seF[i] != 0) {
                    if (seTimer[i] >= 1.0f) {
                        HuAudFXPlay(7);
                        seTimer[i] = 0.0f;
                    }
                }
                Result_SetBonusSprNum(BonusSprGrpId[i], holdCoin[TeamOrder[i]] - cnt);
                Result_SetScoreSprNum(ScoreSprGrpId[i+2], cnt + TeamCoinAddNum[TeamOrder[i]]);
                CoinRotSpeed[i] = 3.0f;
            }
        }
        if (n == 2) {
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
        for(i=0; i<2; i++) {
            HuSprGrpScaleSet(BonusSprGrpId[i], scale2, scale2);
            HuSprScaleSet(CrownSprGrpId, i, scale2, scale2);
        }
        HuPrcVSleep();
    }
    for(i=0; i<2; i++) {
        for(j=0; j<5; j++) {
            HuSprAttrSet(BonusSprGrpId[i], j, 4);
        }
        HuSprAttrSet(CrownSprGrpId, i, 4);
    }
}

void Result_TeamUpdateRank(void)
{
    s32 digit[3];
    s32 delay[4] = { 60, 30, 0, 0 };
    s32 shrinkF[2] = { 1, 1 };
    s32 motF[4];
    float scale[2];
    float boxScale[2];
    float tpLvl[2];
    HUSPRITE *sprP;
    HUSPRITE *boxP;
    s16 sprGrp;
    float speed;
    s16 delayStep;
    s16 frame;
    s16 i;
    s16 j;
    s16 sum;
    s16 other;
    s16 winTeam;
    s32 startF;
    s32 doneF;
    s32 coinNum;
    s16 tmp;

    startF = 0;
    doneF = 0;
    sprGrp = espGrpIDGet();
    if (TeamReqSort == 0) {
        winTeam = 1;
    } else if (TeamReqSort == 1) {
        winTeam = 0;
    }
    for(i=0; i<2; i++) {
        scale[i] = 0.37f;
        boxScale[i] = 1.0f;
        tpLvl[i] = 1.0f;
        sprP = &HuSprData[HuSprGrpData[sprGrp].sprId[StageSprId[i]]];
    }
    for(i=0; i<4; i++) {
        motF[i] = 0;
    }
    if (TeamStarNum[TeamOrder[1]] == TeamStarNum[TeamOrder[0]] && TeamCoinNum[1] == TeamCoinNum[0]) {
        HuSprBankSet(RankSprGrpId, 0, 0);
        HuSprBankSet(RankSprGrpId, 1, 0);
        if (TeamCoinAddNum[0] != TeamCoinAddNum[1]) {
            for(i=2; i<4; i++) {
                Hu3DMotionShiftSet(StageCharMdlId[i], StageCharMotId[i][TopWinMotTbl[i]], 0.0f, 8.0f, 0);
                motF[i] = 1;
            }
            for(i=0; i<4; i++) {
                if (winTeam == GwPlayer[i].grp) {
                    PlayerFXPlay(i, 0x1F8);
                }
            }
            startF = 1;
            while (1) {
                sum = 0;
                for(i=2; i<4; i++) {
                    if (startF != 0 && motF[i] != 0 && Result_CharMotEnd(i) != 0) {
                        motF[i] = 0;
                    }
                    sum += motF[i];
                }
                if (startF != 0 && sum <= 0) {
                    for(i=0; i<4; i++) {
                        Hu3DMotionShiftSet(StageCharMdlId[i], StageCharMotId[i][0], 0.0f, 8.0f, 0x40000001);
                    }
                    break;
                }
                HuPrcVSleep();
            }
        }
        return;
    }
    if (TeamCoinNum[TeamOrder[0]] > TeamCoinNum[TeamOrder[1]] || (TeamCoinNum[TeamOrder[0]] == TeamCoinNum[TeamOrder[1]] && TeamStarNum[TeamOrder[0]] > TeamStarNum[TeamOrder[1]])) {
        HuSprBankSet(RankSprGrpId, 0, 0);
        HuSprBankSet(RankSprGrpId, 1, 1);
        if (TeamOrder[0] == 0 && TeamOrder[1] == 1 && TeamCoinAddNum[0] == TeamCoinAddNum[1] && TeamCoinNum[TeamOrder[0]] == TeamCoinNum[TeamOrder[1]]) {
            for(i=2; i<4; i++) {
                Hu3DMotionShiftSet(StageCharMdlId[i], StageCharMotId[i][BottomWinMotTbl[i]], 0.0f, 8.0f, 0);
                motF[i] = 1;
            }
            startF = 1;
            while (1) {
                sum = 0;
                for(i=0; i<4; i++) {
                    if (startF != 0 && motF[i] != 0 && Result_CharMotEnd(i) != 0) {
                        motF[i] = 0;
                    }
                    sum += motF[i];
                }
                if (startF != 0 && sum <= 0) {
                    for(i=0; i<4; i++) {
                        Hu3DMotionShiftSet(StageCharMdlId[i], StageCharMotId[i][0], 0.0f, 8.0f, 0x40000001);
                    }
                    break;
                }
                HuPrcVSleep();
            }
        }
        return;
    }
    tmp = TeamOrder[0];
    TeamOrder[0] = TeamOrder[1];
    TeamOrder[1] = tmp;
    sum = 0;
    frame = 0;
    while (1) {
        speed = Result_FastF ? 0.0296f : 0.0148f;
        delayStep = Result_FastF ? 2 : 1;
        if (Result_FastF != 0) {
            frame += 3;
        } else {
            frame += 1;
        }
        if (doneF == 0) {
            for(i=1; i>-1; i--) {
                if ((frame % 2) != 0) {
                    for(j=0; j<3; j++) {
                        digit[j] = frandmod(10);
                        if (digit[j] == 0) {
                            digit[j] = frandmod(9) + 1;
                        }
                    }
                    coinNum = digit[0]*100 + (digit[1]*10 + digit[2]);
                    Result_SetScoreSprNum(ScoreSprGrpId[i], coinNum);
                    Result_SetScoreSprNum(ScoreSprGrpId[i+2], coinNum);
                    CoinRotSpeed[i] = 4.0f;
                    StarRotSpeed[i] = 4.0f;
                }
                if (shrinkF[i] != 0) {
                    if ((scale[i] -= speed) <= 0.0f) {
                        scale[i] = 0.001f;
                        shrinkF[i] = 0;
                        if (i != 0) {
                            Result_TeamUpdateOrder();
                        }
                        Result_TeamSetTPLvl(i, scale[i]);
                    }
                    if ((boxScale[i] -= 3.0f * speed) <= 0.0f) {
                        boxScale[i] = 0.0f;
                    }
                    HuSprScaleSet(TeamBoxSprGrpId, i, boxScale[i], 1.0f);
                    boxP = &HuSprData[HuSprGrpData[TeamBoxSprGrpId].sprId[i]];
                    HuWinPosSet(TeamNameWinId[i], boxP->pos.x - 118.0f * boxP->scale.x, TeamNamePos[i].y);
                    HuWinScaleSet(TeamNameWinId[i], boxScale[i], 1.0f);
                    if ((tpLvl[i] -= 3.0f * speed) <= 0.1f) {
                        tpLvl[i] = 0.1f;
                    }
                    espTPLvlSet(StageSprId[i+1], tpLvl[i]);
                } else {
                    if ((delay[i] -= delayStep) <= 0) {
                        if ((scale[i] += speed) >= 0.37f) {
                            scale[i] = 0.37f;
                            Result_SetScoreSprNum(ScoreSprGrpId[i], TeamCoinNum[TeamOrder[i]]);
                            Result_SetScoreSprNum(ScoreSprGrpId[i+2], TeamStarNum[TeamOrder[i]]);
                            CoinRotSpeed[i] = 1.0f;
                            StarRotSpeed[i] = 1.0f;
                            Result_TeamSetTPLvl(i, scale[i]);
                            if (i == 0) {
                                doneF = 1;
                            }
                        }
                        if (i == 0) {
                            other = 1;
                        } else if (i != 0) {
                            other = 0;
                        }
                        if ((boxScale[other] += 3.0f * speed) >= 1.0f) {
                            boxScale[other] = 1.0f;
                        }
                        HuSprScaleSet(TeamBoxSprGrpId, other, boxScale[other], 1.0f);
                        boxP = &HuSprData[HuSprGrpData[TeamBoxSprGrpId].sprId[other]];
                        HuWinPosSet(TeamNameWinId[other], boxP->pos.x - 118.0f * boxP->scale.x, TeamNamePos[other].y);
                        HuWinScaleSet(TeamNameWinId[other], boxScale[other], 1.0f);
                        if ((tpLvl[other] += 2.0f * speed) >= 1.0f) {
                            tpLvl[other] = 1.0f;
                        }
                        espTPLvlSet(StageSprId[other+1], tpLvl[other]);
                    }
                }
                Result_TeamSetTPLvl(i, scale[i]);
            }
        } else if (startF == 0) {
            HuSprBankSet(RankSprGrpId, 0, 0);
            HuSprBankSet(RankSprGrpId, 1, 1);
            for(i=0; i<4; i++) {
                Hu3DMotionShiftSet(StageCharMdlId[i], StageCharMotId[i][TopWinMotTbl[i]], 0.0f, 8.0f, 0);
                if (winTeam == GwPlayer[i].grp) {
                    PlayerFXPlay(i, 0x1F8);
                }
                motF[i] = 1;
            }
            startF = 1;
        }
        sum = 0;
        for(i=sum; i<4; i++) {
            if (startF != 0 && motF[i] != 0 && Result_CharMotEnd(i) != 0) {
                motF[i] = 0;
            }
            sum += motF[i];
        }
        if (startF != 0 && sum <= 0) {
            for(i=0; i<4; i++) {
                Hu3DMotionShiftSet(StageCharMdlId[i], StageCharMotId[i][0], 0.0f, 8.0f, 0x40000001);
            }
            break;
        }
        HuPrcVSleep();
    }
}

void Result_TeamSetTPLvl(s16 level, float scale)
{
    if (level != 0) {
        Hu3DModelScaleSet(StageToueiMdlId[3], scale, scale, scale);
        Hu3DModelScaleSet(StageToueiMdlId[2], scale, scale, scale);
    } else {
        Hu3DModelScaleSet(StageToueiMdlId[1], scale, scale, scale);
        Hu3DModelScaleSet(StageToueiMdlId[0], scale, scale, scale);
    }
}

void Result_TeamUpdateOrder(void)
{
    HuVecF pos2;
    HuVecF pos1;
    HuVecF tmp;
    s16 mdl[4];
    s32 i;

    for(i=0; i<4; i++) {
        mdl[i] = StageToueiMdlId[i];
    }
    StageToueiMdlId[0] = mdl[2];
    StageToueiMdlId[1] = mdl[3];
    StageToueiMdlId[2] = mdl[0];
    StageToueiMdlId[3] = mdl[1];
    Hu3D2Dto3D(&ToueiPosTbl[0], 2, &pos1);
    Hu3D2Dto3D(&ToueiPosTbl[1], 2, &pos2);
    Hu3DModelPosSetV(StageToueiMdlId[0], &pos1);
    Hu3DModelPosSetV(StageToueiMdlId[1], &pos2);
    Hu3D2Dto3D(&ToueiPosTbl[2], 2, &pos1);
    Hu3D2Dto3D(&ToueiPosTbl[3], 2, &pos2);
    Hu3DModelPosSetV(StageToueiMdlId[2], &pos1);
    Hu3DModelPosSetV(StageToueiMdlId[3], &pos2);
    HuSprPosSet(TeamBoxSprGrpId, 0, TeamBoxPosTbl[2], TeamBoxPosTbl[3]);
    HuSprPosSet(TeamBoxSprGrpId, 1, TeamBoxPosTbl[0], TeamBoxPosTbl[1]);
    tmp = TeamNamePos[0];
    TeamNamePos[0] = TeamNamePos[1];
    TeamNamePos[1] = tmp;
    for(i=0; i<2; i++) {
        HuWinPosSet(TeamNameWinId[i], TeamNamePos[i].x, TeamNamePos[i].y);
    }
    espPosSet(StageSprId[2], 288.0f, 180.0f);
    espPosSet(StageSprId[1], 288.0f, 330.0f);
}

u32 Result_TeamNameMessGet(s16 charNo1, s16 charNo2)
{
    s32 teamNo;

    if (charNo1 > 9 || charNo2 > 9) {
        return 0x8002D;
    }
    if (charNo1 < charNo2) {
        teamNo = (s8)TeamNoTbl[charNo2 + charNo1*10];
    } else {
        teamNo = (s8)TeamNoTbl[charNo1 + charNo2*10];
    }
    OSReport("%d:%d->%d\n", charNo1, charNo2, teamNo);
    if (teamNo == -1) {
        return 0x8002D;
    }
    return teamNo + 0x80000;
}
