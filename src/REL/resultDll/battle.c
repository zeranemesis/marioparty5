#include "REL/resultDll.h"
double sin(double x);
double cos(double x);

#ifdef TARGET_PC
#include "game/esprite.h"
#else
/* the original calls these without prototypes (float arguments promoted to double) */
void espTPLvlSet();
void espScaleSet();
int espGrpIDGet();
#endif

extern s32 Result_FastF;
extern s32 Result_BgReadEndF;
extern s16 Result_RandomCoinPlayer;

void Result_Debug(void);
void Result_BgRead(void);
void Result_BattleCoinMdlMain(void);
void Result_BattleCreate(void);
void Result_BattleStart(void);
void Result_BattleAddCoin(void);
void Result_BattleAddRandomCoin(void);
void Result_BattleShowBonus(void);
void Result_BattleSetupPlayer(void);
void Result_AddBonusCoin(void);
void Result_BoardSave(void);
void Result_GetCoin(s16 *coin);
void Result_SetBonusSprNum(HUSPRGRPID grpId, s16 coinNum);
s16 Result_CharMotEnd(s16 playerNo);

STAGE_MODEL Result_BattleModel[3] = {
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

STAGE_SPRITE Result_BattleSprite[6] = {
    { 0xB90006, 0, 0xA, 288.0f, 70.0f, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { 0xB9000C, 0, 0x14, 108.0f, -1050.0f, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { 0xB9000C, 0, 0x14, 228.0f, -850.0f, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { 0xB9000C, 0, 0x14, 348.0f, -550.0f, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { 0xB9000C, 0, 0x14, 468.0f, -250.0f, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { 0, 0, 0, 0.0f, 0.0f, { 0, 0, 0, 0 } },
};

static s16 RankPosTbl[8] = { 0x6C, -0x492, 0xE4, -0x3CA, 0x15C, -0x29E, 0x1D4, -0x172 };
static s16 BonusPosTbl[8] = { 0x5D, 0x12C, 0xD5, 0x12C, 0x14D, 0x12C, 0x1C5, 0x12C };
static HuVecF ToueiPosTbl[4] = {
    { 105.0f, -1110.0f, 1600.0f },
    { 227.0f, -910.0f, 1600.0f },
    { 347.0f, -610.0f, 1600.0f },
    { 467.0f, -310.0f, 1600.0f },
};
static HuVecF ToueiRotTbl[4] = {
    { 0.0f, 5.0f, 0.0f },
    { 0.0f, 1.0f, 0.0f },
    { 0.0f, -5.0f, 0.0f },
    { 0.0f, -10.0f, 0.0f },
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
static HuVecF CoinBagPosTbl[4] = {
    { 108.0f, 220.0f, 1600.0f },
    { 228.0f, 220.0f, 1600.0f },
    { 348.0f, 220.0f, 1600.0f },
    { 468.0f, 220.0f, 1600.0f },
};
static HuVecF GuideEndPos = { 926.0f, 220.0f, 1600.0f };
static HuVecF CoinBagScaleTbl[4] = {
    { 0.8f, 0.8f, 0.8f },
    { 0.6f, 0.6f, 0.6f },
    { 0.5f, 0.5f, 0.5f },
    { 0.5f, 0.5f, 0.5f },
};
static float CoinMaxPosYTbl[4] = { 310.0f, 330.0f, 360.0f, 360.0f };
static HuVecF BonusSprPosTbl[4] = {
    { 12.0f, -25.0f, 0.0f },
    { 28.0f, -25.0f, 0.0f },
    { 44.0f, -25.0f, 0.0f },
    { 60.0f, -25.0f, 0.0f },
};
static float CoinDropPosXTbl[4] = { 108.0f, 228.0f, 348.0f, 468.0f };
static HuVecF CoinStartPosTbl[4] = {
    { 108.0f, -100.0f, 1600.0f },
    { 228.0f, -100.0f, 1600.0f },
    { 348.0f, -100.0f, 1600.0f },
    { 468.0f, -100.0f, 1600.0f },
};
static float CoinRandomPosXTbl[4] = { 108.0f, 228.0f, 348.0f, 468.0f };

static s32 BattleSkipF;
static float GuideSpeed;
s16 lbl_1_bss_520;
static s16 GuideMdlId;
static float CoinYAccel[256];
static float CoinYSpeed[256];
static float CoinRotYSpeed[4][256];
static s16 CoinMdlId[256];
static s16 CoinBagFlatMdlId[4];
static s16 CoinBagMdlId[6];
static s16 BattleCoin[5];
static s16 BattleCharNo[4];
static s16 BonusSprId[4][5];
static s16 BonusSprGrpId[4];
static s16 lbl_1_bss_1F7A;
static s16 BattleRank[4];
static s16 BattlePlayerNo[4];
static s16 RankSprGrpId;
static ANIMDATA *RankAnimP;

void Result_BattleMain(void)
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
        BattleSkipF = 1;
    }
    else {
        BattleSkipF = 0;
    }
    Result_BattleCreate();
    HuPrcChildCreate(Result_BattleCoinMdlMain, 0x64, 0x2000, 0, prcP);
    WipeCreate(1, 0, 0x14);
    HuAudSStreamPlay(0x5B);
    while (WipeCheckEnd()) {
        HuPrcVSleep();
    }
    HuDataDirClose(0xB90000);
    Result_BgReadEndF = 0;
    HuPrcChildCreate(Result_BgRead, 0x64, 0x3000, 0, prcP);
    HuPrcSleep(0xA);
    if (Result_FastF == 0) {
        HuPrcSleep(0x14);
    }
    Result_BattleStart();
    Result_BattleAddCoin();
    Result_BattleAddRandomCoin();
    Result_BattleShowBonus();
    Result_AddBonusCoin();
    winId = HuWinCreate(-10000.0f, 400.0f, 0x140, 0x28, 0);
    HuWinMesSpeedSet(winId, 0);
    HuWinBGTPLvlSet(winId, 0.0f);
    HuWinPriSet(winId, 5);
    HuWinAttrSet(winId, 0x800);
    if (BattleSkipF == 0) {
        HuWinMesSet(winId, 0x2000D);
    }
    HuPrcSleep(4);
    while (1) {
        if (BattleSkipF != 0) {
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
    WipeCreate(2, 0, 0x3C);
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

void Result_BattleCoinMdlMain(void)
{
    float f1;
    float f2;
    float f3;
    s16 i;

    f1 = 0.0f;
    f2 = 0.0f;
    f3 = 0.0f;

    while (1) {
        if (BattleSkipF != 0) {
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
        HuPrcVSleep();
    }
}

void Result_BattleCreate(void)
{
    HuVecF pos;
    HuVecF flatPos;
    ANIMDATA *numAnim;
    ANIMDATA *bonusAnim;
    s16 charNo;
    s16 i;
    s16 j;

    StageSpriteCreate(Result_BattleSprite);
    StageModelCreate(Result_BattleModel);
    Hu3DMotionSpeedSet(StageMdlId[0], 0.5f);
    espTPLvlSet(StageSprId[0], 0.3f);
    for(i=1; i<5; i++) {
        espScaleSet(StageSprId[i], 0.9f, 0.9f);
    }
    for(i=0; i<4; i++) {
        if (i == 0) {
            CoinBagMdlId[i] = Hu3DModelCreateData(0xB90012);
        } else if (i == 3 || i == 2) {
            CoinBagMdlId[i] = Hu3DModelCreateData(0xB90012);
        } else {
            CoinBagMdlId[i] = Hu3DModelLink(CoinBagMdlId[0]);
        }
        Hu3D2Dto3D(&CoinBagPosTbl[i], 4, &pos);
        Hu3DModelScaleSet(CoinBagMdlId[i], 0.0f, 0.0f, 0.0f);
        Hu3DModelLayerSet(CoinBagMdlId[i], 2);
        Hu3DModelPosSetV(CoinBagMdlId[i], &pos);
        Hu3DModelCameraSet(CoinBagMdlId[i], 4);
        Hu3DModelAttrSet(CoinBagMdlId[i], 0x40000082);
    }
    CoinBagFlatMdlId[0] = CoinBagFlatMdlId[1] = -1;
    for(i=2; i<4; i++) {
        CoinBagFlatMdlId[i] = Hu3DModelCreateData(0xB90015);
        flatPos = CoinBagPosTbl[i];
        flatPos.y = 390.0f;
        Hu3D2Dto3D(&flatPos, 4, &pos);
        Hu3DModelPosSetV(CoinBagFlatMdlId[i], &pos);
        Hu3DModelScaleSet(CoinBagFlatMdlId[i], CoinBagScaleTbl[i].x, CoinBagScaleTbl[i].y, CoinBagScaleTbl[i].z);
        Hu3DModelLayerSet(CoinBagFlatMdlId[i], 2);
        Hu3DModelCameraSet(CoinBagFlatMdlId[i], 4);
        Hu3DModelAttrSet(CoinBagFlatMdlId[i], 1);
        Hu3DModelAttrSet(CoinBagFlatMdlId[i], 0x40000082);
    }
    numAnim = HuSprAnimDataRead(0xB90003);
    bonusAnim = HuSprAnimDataRead(0xB90008);
    for(i=0; i<4; i++) {
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
    Result_BattleSetupPlayer();
    RankAnimP = HuSprAnimDataRead(0xB90005);
    RankSprGrpId = HuSprGrpCreate(4);
    for(i=0; i<4; i++) {
        HuSprGrpMemberSet(RankSprGrpId, i, HuSprCreate(RankAnimP, i, 0));
        HuSprPosSet(RankSprGrpId, i, RankPosTbl[i*2], RankPosTbl[i*2+1]);
        HuSprPriSet(RankSprGrpId, i, 15);
        HuSprBankSet(RankSprGrpId, i, BattleRank[i]);
    }
    HuSprGrpDrawNoSet(RankSprGrpId, 0x40);
    HuSprExecLayerCameraSet(0x40, 2, 1);
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
        Hu3DModelRotSetV(StageToueiMdlId[i], &ToueiRotTbl[i]);
        BattleCharNo[i] = GwPlayerConf[BattlePlayerNo[i]].charNo;
        charNo = BattleCharNo[i]*4;
        StageCharMdlId[i] = Hu3DModelCreateData(charNo+0xB90020);
        StageCharMotId[i][0] = Hu3DJointMotionData(StageCharMdlId[i], charNo+0xB90021);
        StageCharMotId[i][1] = Hu3DJointMotionData(StageCharMdlId[i], charNo+0xB90022);
        StageCharMotId[i][2] = Hu3DJointMotionData(StageCharMdlId[i], charNo+0xB90023);
        Hu3D2Dto3D(&ToueiCharPosTbl[i], 1, &pos);
        Hu3DMotionSet(StageCharMdlId[i], StageCharMotId[i][0]);
        Hu3DModelScaleSet(StageCharMdlId[i], ToueiCharScaleTbl[BattleCharNo[i]].x, ToueiCharScaleTbl[BattleCharNo[i]].y, ToueiCharScaleTbl[BattleCharNo[i]].z);
        Hu3DModelLayerSet(StageCharMdlId[i], 0);
        Hu3DModelPosSetV(StageCharMdlId[i], &pos);
        Hu3DModelAttrSet(StageCharMdlId[i], 0x40000001);
        Hu3DModelCameraSet(StageCharMdlId[i], 1);
    }
    ToueiDispInit(BattleCharNo);
    for(i=0; i<256; i++) {
        if (i == 0) {
            CoinMdlId[i] = Hu3DModelCreateData(0xB90017);
        } else {
            CoinMdlId[i] = Hu3DModelLink(CoinMdlId[0]);
        }
        Hu3DModelAttrSet(CoinMdlId[i], 1);
        Hu3DModelCameraSet(CoinMdlId[i], 4);
        Hu3DModelLayerSet(CoinMdlId[i], 1);
        CoinYAccel[i] = 0.001f;
        Hu3DModelScaleSet(CoinMdlId[i], 0.5f, 0.5f, 0.5f);
        CoinYSpeed[i] = 0.0f;
    }
    GuideMdlId = Hu3DModelCreateData(0xB90055);
    Hu3DModelAttrSet(GuideMdlId, 0x40000001);
    Hu3DModelCameraSet(GuideMdlId, 4);
    Hu3D2Dto3D(&GuideEndPos, 4, &pos);
    Hu3DModelPosSetV(GuideMdlId, &pos);
    Hu3DModelLayerSet(GuideMdlId, 1);
    Hu3DModelScaleSet(GuideMdlId, 0.8f, 0.8f, 0.8f);
    Result_GetCoin(BattleCoin);
}

void Result_BattleStart(void)
{
    HuVecF pos;
    float scale[4];
    s32 attrF[4];
    s32 dropF[4];
    HU3DMODEL *mdlP;
    HUSPRITE *sprP;
    float tpLvl;
    float speed;
    s32 endF;
    s16 grpId;
    s16 i;

    tpLvl = 0.3f;
    endF = 0;
    for(i=0; i<4; i++) {
        dropF[i] = 0;
        scale[i] = 0.0f;
        attrF[i] = 0;
    }
    while (1) {
        speed = Result_FastF ? 1.5 : 1.0;
        GuideSpeed = 10.0f * speed;
        mdlP = &Hu3DData[GuideMdlId];
        Hu3D3Dto2D(&mdlP->pos, 4, &pos);
        pos.x = pos.x - 0.7f * GuideSpeed;
        for(i=3; i>-1; i--) {
            if (dropF[i] == 0) {
                if (pos.x <= CoinDropPosXTbl[i]) {
                    dropF[i] = 1;
                }
            }
        }
        if (pos.x <= -200.0f) {
            endF = 1;
        }
        pos.z = 1600.0f;
        Hu3D2Dto3D(&pos, 4, &mdlP->pos);
        for(i=0; i<4; i++) {
            if (dropF[i] != 0) {
                mdlP = &Hu3DData[CoinBagMdlId[i]];
                scale[i] += 0.05f;
                if (scale[i] >= CoinBagScaleTbl[BattleRank[i]].x) {
                    scale[i] = CoinBagScaleTbl[BattleRank[i]].x;
                }
                Hu3DModelScaleSet(CoinBagMdlId[i], scale[i], scale[i], scale[i]);
                Hu3D3Dto2D(&mdlP->pos, 4, &pos);
                pos.y += 0.5f * GuideSpeed;
                if (pos.y > 390.0f) {
                    pos.y = 390.0f;
                    if ((BattleRank[i] == 2 || BattleRank[i] == 3) && attrF[i] == 0) {
                        Hu3DModelAttrReset(CoinBagMdlId[i], 0x40000082);
                        attrF[i] = 1;
                    }
                }
                pos.z = 1600.0f;
                Hu3D2Dto3D(&pos, 4, &mdlP->pos);
            }
        }
        for(i=0; i<4; i++) {
            mdlP = &Hu3DData[StageToueiMdlId[i]];
            Hu3D3Dto2D(&mdlP->pos, 2, &pos);
            pos.y += GuideSpeed;
            if (pos.y > 190.0f) {
                pos.y = 190.0f;
            }
            pos.z = 1600.0f;
            Hu3D2Dto3D(&pos, 2, &mdlP->pos);
        }
        for(i=0; i<4; i++) {
            sprP = &HuSprData[HuSprGrpData[RankSprGrpId].sprId[i]];
            sprP->pos.y += GuideSpeed;
            if (sprP->pos.y > 130.0f) {
                sprP->pos.y = 130.0f;
            }
        }
        grpId = espGrpIDGet();
        for(i=1; i<5; i++) {
            sprP = &HuSprData[HuSprGrpData[grpId].sprId[i]];
            sprP->pos.y += GuideSpeed;
            if (sprP->pos.y > 250.0f) {
                sprP->pos.y = 250.0f;
            }
        }
        if (endF != 0) {
            tpLvl += 0.03f;
            if (tpLvl >= 1.0f) {
                espTPLvlSet(StageSprId[0], 1.0f);
                break;
            }
            espTPLvlSet(StageSprId[0], tpLvl);
        }
        HuPrcVSleep();
    }
}

void Result_BattleAddCoin(void)
{
    s16 wordB[4][256];
    s16 wordA[4][256];
    s32 owner[256];
    float rotSpeed[256];
    HuVecF pos;
    HuVecF startPos;
    s32 spawned[4];
    s32 unusedArr[4];
    s32 delay[4];
    s16 landed[4];
    s32 u1 = 0;
    s32 u2 = 0;
    s32 u3 = 0;
    HU3DMODEL *mdlP;
    float speed;
    s32 total;
    s32 i;
    s32 j;
    s32 k;

    total = 0;
    for(i=0; i<4; i++) {
        total += BattleCoin[i];
        spawned[i] = 0;
        unusedArr[i] = 0;
        delay[i] = 0;
        landed[i] = 0;
        for(j=0; j<256; j++) {
            wordB[i][j] = 0;
            wordA[i][j] = 0;
            CoinRotYSpeed[i][j] = 0.0f;
        }
    }
    while (1) {
        speed = Result_FastF ? 1.5 : 1.0;
        for(i=0; i<4; i++) {
            if (--delay[i] <= 0 && BattleCoin[i] > 0 && spawned[i] < BattleCoin[i]) {
                for(j=0; j<256; j++) {
                    if (Hu3DModelAttrGet(CoinMdlId[j]) & 1) {
                        Hu3DModelAttrReset(CoinMdlId[j], 1);
                        Hu3D2Dto3D(&CoinStartPosTbl[i], 2, &startPos);
                        Hu3DModelPosSetV(CoinMdlId[j], &startPos);
                        rotSpeed[j] = 5.0f;
                        CoinYSpeed[j] = 10.0f;
                        delay[i] = 6;
                        spawned[i]++;
                        owner[j] = i;
                        break;
                    }
                }
            }
        }
        for(k=0; k<256; k++) {
            if ((Hu3DModelAttrGet(CoinMdlId[k]) & 1) == 0) {
                mdlP = &Hu3DData[CoinMdlId[k]];
                Hu3D3Dto2D(&mdlP->pos, 2, &pos);
                pos.y += speed * CoinYSpeed[k];
                CoinYSpeed[k] += 0.002f * speed;
                pos.z = 1600.0f;
                Hu3D2Dto3D(&pos, 2, &mdlP->pos);
                mdlP->rot.y += rotSpeed[k];
                if (pos.y >= CoinMaxPosYTbl[BattleRank[owner[k]]]) {
                    HuAudFXPlay(7);
                    if (BattleRank[owner[k]] == 3 && landed[owner[k]] == 0) {
                        Hu3DModelAttrSet(CoinBagMdlId[owner[k]], 1);
                        Hu3DModelAttrReset(CoinBagFlatMdlId[owner[k]], 1);
                        Hu3DModelAttrReset(CoinBagFlatMdlId[owner[k]], 0x40000082);
                    }
                    if (BattleRank[owner[k]] == 2 && landed[owner[k]] == 0) {
                        Hu3DModelAttrSet(CoinBagMdlId[owner[k]], 1);
                        Hu3DModelAttrReset(CoinBagFlatMdlId[owner[k]], 1);
                        Hu3DModelAttrReset(CoinBagFlatMdlId[owner[k]], 0x40000082);
                    }
                    CharEffectCoinGlowCreate(4, &mdlP->pos);
                    Hu3DModelAttrSet(CoinMdlId[k], 1);
                    landed[owner[k]]++;
                    total--;
                }
            }
        }
        if (total <= 0) {
            break;
        }
        HuPrcVSleep();
    }
    for(i=0; i<256; i++) {
        CoinYAccel[i] = 0.005f;
        Hu3DModelScaleSet(CoinMdlId[i], 0.5f, 0.5f, 0.5f);
        Hu3DModelAttrSet(CoinMdlId[i], 1);
        CoinYSpeed[i] = 0.0f;
    }
    HuPrcSleep(20);
}

void Result_BattleAddRandomCoin(void)
{
    s16 delay[256];
    HuVecF pos;
    HuVecF coinPos;
    HU3DMODEL *guideP;
    HU3DMODEL *mdlP;
    float speed;
    s32 coinNum;
    s16 landed;
    s32 spawnF;
    s32 endF;
    s32 i;
    s32 j;
    s32 k;

    coinNum = 0;
    landed = 0;
    spawnF = 0;
    endF = 0;
    for(i=0; i<256; i++) {
        delay[i] = 8;
    }
    while (1) {
        speed = Result_FastF ? 1.5 : 1.0;
        GuideSpeed = 10.0f * speed;
        guideP = &Hu3DData[GuideMdlId];
        Hu3D3Dto2D(&guideP->pos, 2, &pos);
        pos.x = pos.x + 0.7f * GuideSpeed;
        pos.z = 1600.0f;
        Hu3D2Dto3D(&pos, 2, &guideP->pos);
        if (pos.x >= CoinRandomPosXTbl[Result_RandomCoinPlayer] - 0.07f * GuideSpeed && spawnF == 0) {
            for(j=0; j<BattleCoin[4]; j++) {
                for(k=0; k<256; k++) {
                    if (Hu3DModelAttrGet(CoinMdlId[k]) & 1) {
                        Hu3DModelAttrReset(CoinMdlId[k], 1);
                        coinPos = guideP->pos;
                        coinPos.x -= 2.0f;
                        Hu3DModelPosSetV(CoinMdlId[k], &coinPos);
                        CoinRotYSpeed[Result_RandomCoinPlayer][k] = 5.0f;
                        CoinYSpeed[k] = 2.0f;
                        coinNum++;
                        break;
                    }
                }
            }
            spawnF = 1;
        }
        if (spawnF != 0) {
            for(i=0; i<256; i++) {
                if ((Hu3DModelAttrGet(CoinMdlId[i]) & 1) == 0 && (i == 0 || (i != 0 && delay[i-1] <= 0))) {
                    mdlP = &Hu3DData[CoinMdlId[i]];
                    Hu3D3Dto2D(&mdlP->pos, 2, &pos);
                    pos.y += CoinYSpeed[i];
                    CoinYSpeed[i] += 0.3f;
                    pos.z = 1600.0f;
                    Hu3D2Dto3D(&pos, 2, &mdlP->pos);
                    mdlP->rot.y += CoinRotYSpeed[Result_RandomCoinPlayer][i];
                    if (pos.y >= CoinMaxPosYTbl[BattleRank[Result_RandomCoinPlayer]]) {
                        HuAudFXPlay(7);
                        if (BattleRank[Result_RandomCoinPlayer] == 3 && landed == 0) {
                            Hu3DModelAttrSet(CoinBagMdlId[Result_RandomCoinPlayer], 1);
                            Hu3DModelAttrReset(CoinBagFlatMdlId[Result_RandomCoinPlayer], 1);
                            Hu3DModelAttrReset(CoinBagFlatMdlId[Result_RandomCoinPlayer], 0x40000082);
                        }
                        if (BattleRank[Result_RandomCoinPlayer] == 2 && landed == 0) {
                            Hu3DModelAttrSet(CoinBagMdlId[Result_RandomCoinPlayer], 1);
                            Hu3DModelAttrReset(CoinBagFlatMdlId[Result_RandomCoinPlayer], 1);
                            Hu3DModelAttrReset(CoinBagFlatMdlId[Result_RandomCoinPlayer], 0x40000082);
                        }
                        landed++;
                        CharEffectCoinGlowCreate(4, &mdlP->pos);
                        Hu3DModelAttrSet(CoinMdlId[i], 1);
                        coinNum--;
                    }
                    delay[i]--;
                }
            }
            if (coinNum == 0 && pos.x > 776.0f) {
                endF = 1;
            }
        }
        if (endF != 0) {
            break;
        }
        HuPrcVSleep();
    }
    HuPrcSleep(10);
}

void Result_BattleShowBonus(void)
{
    s32 flag[4];
    float scale;
    s16 n;
    s16 cnt;
    s16 coin;
    s16 frame;
    s16 i;
    s16 j;

    for(i=0; i<4; i++) {
        if (Result_RandomCoinPlayer == i) {
            Result_SetBonusSprNum(BonusSprGrpId[i], BattleCoin[4] + BattleCoin[i]);
        } else {
            Result_SetBonusSprNum(BonusSprGrpId[i], BattleCoin[i]);
        }
        flag[i] = 0;
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
    if (Result_FastF == 0) {
        HuPrcSleep(15);
    }
    for(i=0; i<4; i++) {
        if (Result_RandomCoinPlayer == i) {
            coin = BattleCoin[4] + BattleCoin[i];
        } else {
            coin = BattleCoin[i];
        }
        if (coin != 0) {
            Hu3DMotionShiftSet(StageCharMdlId[i], StageCharMotId[i][1], 0.0f, 8.0f, 0);
            PlayerFXPlay(BattlePlayerNo[i], 0x1F8);
        } else {
            Hu3DMotionShiftSet(StageCharMdlId[i], StageCharMotId[i][2], 0.0f, 8.0f, 0);
        }
    }
    do {
        cnt = 0;
        for(i=0; i<4; i++) {
            if (Result_CharMotEnd(i) != 0) {
                cnt++;
            }
        }
        if (cnt < 4) {
            HuPrcVSleep();
        }
    } while (cnt < 4);
    for(i=0; i<4; i++) {
        Hu3DMotionShiftSet(StageCharMdlId[i], StageCharMotId[i][0], 0.0f, 8.0f, 0x40000001);
    }
    n = Result_FastF ? 3 : 10;
    for(frame=0; frame<=n; frame++) {
        scale = cos(3.141592653589793 * (90.0 / n * frame) / 180.0);
        for(i=0; i<4; i++) {
            HuSprGrpScaleSet(BonusSprGrpId[i], scale, scale);
        }
        HuPrcVSleep();
    }
    for(i=0; i<4; i++) {
        for(j=0; j<5; j++) {
            HuSprAttrSet(BonusSprGrpId[i], j, 4);
        }
    }
}

// NONMATCHING: rank/tmp get r28/r27 where the original keeps rank in r27
void Result_BattleSetupPlayer(void)
{
    s16 val2;
    s16 val1;
    s16 rank;
    s16 tmp;
    s16 i2;
    s16 j1;
    s16 i1;

    for(i1=0; i1<4; i1++) {
        rank = 0;
        for(j1=0; j1<4; j1++) {
            if (i1 != j1) {
                val2 = GwPlayer[j1].mgCoinBonus;
                val1 = GwPlayer[i1].mgCoinBonus;
                if (val1 > val2) {
                    rank++;
                }
            }
        }
        BattleRank[i1] = rank;
        BattlePlayerNo[i1] = i1;
    }
    for(i2=1; i2<4; i2++) {
        for(i1=0; i1<4-i2; i1++) {
            if (BattleRank[i1] > BattleRank[i1+1]) {
                tmp = BattleRank[i1];
                BattleRank[i1] = BattleRank[i1+1];
                BattleRank[i1+1] = tmp;
                tmp = BattlePlayerNo[i1];
                BattlePlayerNo[i1] = BattlePlayerNo[i1+1];
                BattlePlayerNo[i1+1] = tmp;
            }
        }
    }
}
