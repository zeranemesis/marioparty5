#include "REL/partyresultdll.h"
#include "game/hsfex.h"
#include "game/main.h"
#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "math.h"
#include "humath.h"

#ifdef __MWERKS__
#define RES_RODATA __declspec(section ".rodata")
#else
#define RES_RODATA
#endif

#define RES_DATA(n) (0xB10000 + (n))

extern s16 Hu3DCameraNo;
s16 PResResultOrderGet(s16 (*orderP)[4], s16 (*tieP)[4], s16 (*rankP)[4]);
s16 PResResultTeamOrderGet(s16 *order);

static void BGFilter(s16 layerNo);
static void BlockPlayerProc(void);
static void BlockProc(void);
static void BonusFadeCreate(s16 mdlId, s16 num, HuVecF *pos);
static void BonusFadeEffHook(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);
static void BonusFadeFbCopy(s16 layerNo);
static void DispStatTotal(void);
static void PResResultFinal(void);
static void PResResultFinalTeam(void);
static void PResResultKill(void);
static s16 PResResultTie(void);
static void PResResultWinnerExec(s16 charNo1, s16 charNo2, f32 height);
static void PlayerHover(void);
static void ResultBonusExec(s16 type);
static void ResultCharFXPlay(s16 charNo, s16 seNo);
static void ResultCharFXUpdate(s16 playerNo);
static void StatNumSet(s16 n, s16 val);
static void TeamNameMain(void);
static void TeamNamePosUpdate(void);

static OMCAMERAVIEW MainResultView = { { 0.0f, 120.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, 1240.0f };
static OMCAMERAVIEW FinalResultView = { { 45.0f, 100.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, 960.0f };
static HuVecF GuideCenterPos = { 0.0f, 120.0f, 700.0f };
static HuVecF GuideLeftPos = { -320.0f, 50.0f, 0.0f };
static HuVecF PlayerPosTbl[4] = {
    { -180.0f, -70.0f, 0.0f },
    { -30.0f, -70.0f, 0.0f },
    { 120.0f, -70.0f, 0.0f },
    { 270.0f, -70.0f, 0.0f },
};
static HU3DPARMANPARAM StarEffParManParam = {
    100,
    0,
    1.0f,
    25.0f,
    30.0f,
    { 0.0f, -0.03f, 0.0f },
    1.0f,
    1.0f,
    50.0f,
    0.98f,
    4,
    {
        { 0xFF, 0xFF, 0xFF, 0xFF },
        { 0xEB, 0x5A, 0x8C, 0xFF },
        { 0x8C, 0xB4, 0xFF, 0xFF },
        { 0x46, 0xF0, 0x9B, 0xFF },
    },
    {
        { 0xFF, 0xFF, 0xFF, 0 },
        { 0xEB, 0x5A, 0x8C, 0 },
        { 0x8C, 0xB4, 0xFF, 0 },
        { 0x46, 0xF0, 0x9B, 0 },
    },
};
static u32 BonusStarMessTbl[15] = {
    0x90004,
    0x9000B,
    0x9000E,
    0x90005,
    0x9000C,
    0x9000F,
    0x90006,
    0x9000D,
    0x90010,
    0x90007,
    0x90007,
    0x90007,
    0x9001B,
    0x9001C,
    0x9001D,
};
static HU3DPARMANPARAM BlockDustParManParam = {
    30,
    0,
    20.0f,
    10.0f,
    180.0f,
    { 0.0f, 0.0f, 0.0f },
    6.0f,
    0.97f,
    50.0f,
    1.03f,
    2,
    {
        { 0xFF, 0xFF, 0xFF, 0xFF },
        { 0xC0, 0xC0, 0xC0, 0xFF },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
    },
    {
        { 0xFF, 0xFF, 0xFF, 0 },
        { 0xC0, 0xC0, 0xC0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
    },
};
static s8 WinnerRotXTbl[13] = { 0x00, 0x0F, 0x00, 0x23, 0x19, 0x00, 0x00, 0x00, 0x00, 0x14, 0x14, 0x14, 0x14 };

static s16 BonusFadeMode;
static s16 BGFadeTime;
static u8 CharFXVol[13];
static u8 CharFXPan[13];
static s16 BonusFadeSize[4];
static void *BonusFadeAddr;
static s32 BlockSeNo[4];
static s32 BlockMode;
static s16 BlockPlayerMode[4];
static s16 BlockResult[10];
static s16 BlockNum;
static HUPROCESS *PlayerHoverProc;
static HUPROCESS *TeamNameProc;
static s32 TeamNameFadeF;
static ANIMDATA *CircleAnimP;
static ANIMDATA *DustAnimP;
static s16 TeamNameWinId[2];
static s16 StarEffAnimP[4];
static s16 WinnerSprGrpId;
static s16 MessWinId;
static s16 NumberMdlId[4][10];
static s16 BlockMdlId[4];
static s16 BonusStarMdlId[3];
static s16 StarMdlId;
static s16 PlayerCoinMdlId[4];
static s16 PlayerStarMdlId[4];
static s16 PlayerNumAnimId[4][4];
static s16 PlayerNumMdlId[4][4];
static s16 GuideMotId[10];
static s16 GuideMdlId;
static s16 StageMdlId;

void PResResultCreate(void)
{
    s16 i, j;
    s16 mdl;
    s16 grp;
    s16 par;
    f32 x;
    ANIMDATA *anim;

    mdl = Hu3DModelCreate(HuDataSelHeapReadNum(RES_DATA(3), HU_MEMNUM_OVL, HEAP_MODEL));
    StageMdlId = mdl;
    Hu3DModelScaleSet(mdl, 1.0f, 1.0f, 3.0f);
    Hu3DModelAttrSet(mdl, 0x40000001);
    Hu3DModelCameraSet(mdl, 4);
    Hu3DModelLayerSet(mdl, 0);
    GuideMdlId = Hu3DModelCreate(HuDataSelHeapReadNum(RES_DATA(0xB), HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelCameraSet(GuideMdlId, 4);
    Hu3DModelLayerSet(GuideMdlId, 1);
    GuideMotId[0] = Hu3DJointMotion(GuideMdlId, HuDataSelHeapReadNum(RES_DATA(0xC), HU_MEMNUM_OVL, HEAP_MODEL));
    GuideMotId[1] = Hu3DJointMotion(GuideMdlId, HuDataSelHeapReadNum(RES_DATA(0xD), HU_MEMNUM_OVL, HEAP_MODEL));
    GuideMotId[2] = Hu3DJointMotion(GuideMdlId, HuDataSelHeapReadNum(RES_DATA(0xE), HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DMotionSet(GuideMdlId, GuideMotId[0]);
    Hu3DModelAttrSet(GuideMdlId, 0x40000001);
    Hu3DModelPosSetV(GuideMdlId, &GuideCenterPos);
    if (PResTeamF != 0) {
        x = -180.0f;
        for (i = 0; i < 4; i++) {
            PlayerPosTbl[i].x = x;
            PlayerPosTbl[i].y = -70.0f;
            PlayerPosTbl[i].z = 0.0f;
            x += 130.0f;
            if (i == 1) {
                x += 80.0f;
            }
        }
    }
    for (i = 0; i < 4; i++) {
        mdl = PResPlayerModelId[i];
        CharMotionSet(PResCharNo[i], PResPlayerMotion[i][0]);
        Hu3DModelAttrSet(mdl, 0x40000001);
        Hu3DModelCameraSet(mdl, 4);
        Hu3DModelScaleSet(mdl, 0.0f, 0.0f, 0.0f);
        Hu3DModelLayerSet(mdl, 1);
        PSMTXTrans(Hu3DData[mdl].mtx, 0.0f, 70.0f, 0.0f);
    }
    for (i = 0; i < 4; i++) {
        if (i == 0) {
            mdl = Hu3DModelCreate(HuDataSelHeapReadNum(RES_DATA(4), HU_MEMNUM_OVL, HEAP_MODEL));
            PlayerStarMdlId[i] = mdl;
        } else {
            PlayerStarMdlId[i] = Hu3DModelLink(mdl);
        }
        Hu3DModelCameraSet(PlayerStarMdlId[i], 4);
        Hu3DModelAttrSet(PlayerStarMdlId[i], 1);
        Hu3DModelLayerSet(PlayerStarMdlId[i], 1);
    }
    StarMdlId = Hu3DModelCreate(HuDataSelHeapReadNum(RES_DATA(5), HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelAttrSet(StarMdlId, 1);
    Hu3DModelAttrSet(StarMdlId, 0x40000003);
    Hu3DMotionAttrSet(Hu3DMotionIDGet(StarMdlId), 1);
    Hu3DModelLayerSet(StarMdlId, 1);
    Hu3DModelCameraSet(StarMdlId, 4);
    for (i = 0; i < 4; i++) {
        if (i == 0) {
            mdl = Hu3DModelCreate(HuDataSelHeapReadNum(RES_DATA(6), HU_MEMNUM_OVL, HEAP_MODEL));
            PlayerCoinMdlId[i] = mdl;
        } else {
            PlayerCoinMdlId[i] = Hu3DModelLink(mdl);
        }
        Hu3DModelCameraSet(PlayerCoinMdlId[i], 4);
        Hu3DModelAttrSet(PlayerCoinMdlId[i], 1);
        Hu3DModelLayerSet(PlayerCoinMdlId[i], 1);
    }
    anim = HuSprAnimRead(HuDataSelHeapReadNum(RES_DATA(0), HU_MEMNUM_OVL, HEAP_MODEL));
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (i == 0 && j == 0) {
                mdl = Hu3DModelCreate(HuDataSelHeapReadNum(RES_DATA(0xA), HU_MEMNUM_OVL, HEAP_MODEL));
                PlayerNumMdlId[i][j] = mdl;
            } else {
                PlayerNumMdlId[i][j] = Hu3DModelLink(mdl);
            }
            PlayerNumAnimId[i][j] = Hu3DAnimCreate(anim, PlayerNumMdlId[i][j], "test");
            Hu3DModelCameraSet(PlayerNumMdlId[i][j], 4);
            Hu3DModelAttrSet(PlayerNumMdlId[i][j], 1);
            Hu3DModelLayerSet(PlayerNumMdlId[i][j], 1);
        }
    }
    for (i = 0; i < 3; i++) {
        BonusStarMdlId[i] = Hu3DModelCreate(HuDataSelHeapReadNum(RES_DATA(7) + i, HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelAttrSet(BonusStarMdlId[i], 1);
        Hu3DModelCameraSet(BonusStarMdlId[i], 6);
        Hu3DModelLayerSet(BonusStarMdlId[i], 1);
    }
    for (i = 0; i < 4; i++) {
        BlockMdlId[i] = Hu3DModelCreate(HuDataSelHeapReadNum(RES_DATA(0xF), HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelAttrSet(BlockMdlId[i], 1);
        Hu3DModelCameraSet(BlockMdlId[i], 4);
        Hu3DMotionSpeedSet(BlockMdlId[i], 0.8f);
        Hu3DModelLayerSet(BlockMdlId[i], 1);
        for (j = 0; j < 10; j++) {
            if (i == 0) {
                NumberMdlId[i][j] = Hu3DModelCreate(HuDataSelHeapReadNum(RES_DATA(0x10) + j, HU_MEMNUM_OVL, HEAP_MODEL));
            } else {
                NumberMdlId[i][j] = Hu3DModelLink(NumberMdlId[0][j]);
            }
            Hu3DModelAttrSet(NumberMdlId[i][j], 1);
            Hu3DModelCameraSet(NumberMdlId[i][j], 4);
            Hu3DModelAttrSet(NumberMdlId[i][j], 0x40000002);
            Hu3DModelLayerSet(NumberMdlId[i][j], 1);
        }
    }
    grp = HuSprGrpCreate(2);
    WinnerSprGrpId = grp;
    anim = HuSprAnimRead(HuDataSelHeapReadNum(RES_DATA(1), HU_MEMNUM_OVL, HEAP_MODEL));
    for (i = 0; i < 2; i++) {
        HuSprGrpMemberSet(grp, i, HuSprCreate(anim, 100 - i, 0));
        HuSprAttrSet(grp, i, 0xC);
    }
    HuSprAttrSet(grp, 1, 0x80);
    HuSprGrpPosSet(grp, 288.0f, 380.0f);
    DustAnimP = HuSprAnimRead(HuDataSelHeapReadNum(0x210004, HU_MEMNUM_OVL, HEAP_MODEL));
    anim = HuSprAnimRead(HuDataSelHeapReadNum(RES_DATA(2), HU_MEMNUM_OVL, HEAP_MODEL));
    for (i = 0; i < 4; i++) {
        par = Hu3DParManCreate(anim, 200, &StarEffParManParam);
        StarEffAnimP[i] = par;
        Hu3DParManAttrSet(par, 0x65);
        Hu3DParManRotSet(par, 90.0f, 0.0f, 0.0f);
        Hu3DParticleBlendModeSet(Hu3DParManModelIDGet(par), 1);
        Hu3DModelLayerSet(Hu3DParManModelIDGet(par), 2);
        Hu3DModelCameraSet(Hu3DParManModelIDGet(par), 4);
    }
    CircleAnimP = HuSprAnimRead(HuDataSelHeapReadNum(0x210005, HU_MEMNUM_OVL, HEAP_MODEL));
    HuSprAnimLock(CircleAnimP);
    if (PResTeamF != 0) {
        for (i = 0; i < 2; i++) {
            TeamNameWinId[i] = HuWinCreate(320.0f, i * 100 + 100, 0xEC, 0x2A, 0);
            HuWinBGTPLvlSet(TeamNameWinId[i], 0.0f);
            HuWinPriSet(TeamNameWinId[i], 500);
            HuWinAttrSet(TeamNameWinId[i], 0x800);
            HuWinMesSpeedSet(TeamNameWinId[i], 0);
            HuWinSprSet(TeamNameWinId[i], HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(RES_DATA(0x28) + i, HU_MEMNUM_OVL, HEAP_MODEL)), 0x258, 0), 118.0f, 4.0f);
            HuWinMesSet(TeamNameWinId[i], PResTeamNameMesGet(PResCharNo[i * 2], PResCharNo[i * 2 + 1]));
            HuWinCenterPosSet(TeamNameWinId[i], 123.0f, 40.0f);
            HuWinScaleSet(TeamNameWinId[i], 0.8f, 0.8f);
            HuWinDispOff(TeamNameWinId[i]);
        }
    }
    PlayerHoverProc = HuPrcChildCreate(PlayerHover, 1000, 0x2000, 0, objman);
    Hu3DFogSet(4700.0f, 5500.0f, 0xA0, 0xA0, 0x60);
    omCameraViewSetMulti(6, &MainResultView);
}

void PResResultMain(void)
{
    HuVecF pos;
    f32 t;
    s16 i, j;

    HuAudBGMPlay(0x15);
    WipeCreate(1, 0, 60);
    while (WipeCheckEnd() != 0) {
        HuPrcVSleep();
    }
    MessWinId = HuWinExCreateFrame(16.0f, 376.0f, 0x220, 0x40, -1, 0);
    winData[MessWinId].padMask = 1;
    HuWinExOpen(MessWinId);
    HuWinMesSet(MessWinId, 0x90001);
    HuAudFXPlay(0x2EF);
    PResComInputWait();
    HuWinMesWait(MessWinId);
    for (j = 1; j <= 20; j++) {
        PSVECSubtract(&GuideLeftPos, &GuideCenterPos, &pos);
        PSVECScale(&pos, &pos, HuSin(90.0 * (j / 20.0)));
        PSVECAdd(&GuideCenterPos, &pos, &pos);
        pos.y = pos.y + 50.0 * HuSin(180.0 * (j / 20.0));
        Hu3DModelPosSetV(GuideMdlId, &pos);
        Hu3DModelRotSet(GuideMdlId, 0.0f, 370.0 * (j / 20.0), 0.0f);
        HuPrcVSleep();
    }
    for (j = 1; j <= 45; j++) {
        for (i = 0; i < 4; i++) {
            if (j >= i * 3 && j <= i * 3 + 30) {
                t = (j - i * 3) / 30.0;
                Hu3DModelPosSet(PResPlayerModelId[i], PlayerPosTbl[i].x, PlayerPosTbl[i].y, PlayerPosTbl[i].z - 4000.0 * (1.0 - t));
                ResultCharFXUpdate(i);
                Hu3DModelRotSet(PResPlayerModelId[i], 40.0 * (1.0 - t), 0.0f, 0.0f);
                Hu3DModelScaleSet(PResPlayerModelId[i], 1.0f, 1.0f, 1.0f);
                Hu3DModelLayerSet(PResPlayerModelId[i], 0);
            }
        }
        if (j == 30 && PResTeamF != 0) {
            TeamNameProc = HuPrcChildCreate(TeamNameMain, 1000, 0x2000, 0, objman);
        }
        HuPrcVSleep();
    }
    for (i = 0; i < 4; i++) {
        Hu3DModelLayerSet(PResPlayerModelId[i], 1);
    }
    DispStatTotal();
    if (GwSystem.bonusStarF != 0) {
        ResultBonusExec(0);
        ResultBonusExec(1);
        ResultBonusExec(2);
    }
    if (PResTeamF == 0) {
        PResResultFinal();
    } else {
        PResResultFinalTeam();
    }
    PResBtnAWait();
    WipeCreate(2, 0, 60);
    while (WipeCheckEnd() != 0) {
        HuPrcVSleep();
    }
    PResResultKill();
    HuWinExKill(MessWinId);
    Hu3DFogClear();
}

static void PResResultKill(void)
{
    s16 i, j;

    Hu3DModelKill(StageMdlId);
    Hu3DModelKill(GuideMdlId);
    for (i = 0; i < 4; i++) {
        PSMTXIdentity(Hu3DData[PResPlayerModelId[i]].mtx);
    }
    for (i = 0; i < 4; i++) {
        Hu3DModelKill(PlayerStarMdlId[i]);
    }
    Hu3DModelKill(StarMdlId);
    for (i = 0; i < 4; i++) {
        Hu3DModelKill(PlayerCoinMdlId[i]);
    }
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            Hu3DModelKill(PlayerNumMdlId[i][j]);
        }
    }
    for (i = 0; i < 3; i++) {
        Hu3DModelKill(BonusStarMdlId[i]);
    }
    for (i = 0; i < 4; i++) {
        Hu3DModelKill(BlockMdlId[i]);
        for (j = 0; j < 10; j++) {
            Hu3DModelKill(NumberMdlId[i][j]);
        }
    }
    HuSprGrpKill(WinnerSprGrpId);
    for (i = 0; i < 4; i++) {
        Hu3DParManKill(StarEffAnimP[i]);
    }
    HuSprAnimKill(CircleAnimP);
    if (PResTeamF != 0) {
        for (i = 0; i < 2; i++) {
            HuWinKill(TeamNameWinId[i]);
        }
        HuPrcKill(TeamNameProc);
    }
    Hu3DLayerHookReset(1);
    HuPrcKill(PlayerHoverProc);
}

static void TeamNameMain(void)
{
    f32 t;
    s16 i, j;
    HUWIN *win;

    while (1) {
        TeamNameFadeF = 0;
        for (i = 0; i < 2; i++) {
            HuWinDispOn(TeamNameWinId[i]);
        }
        for (j = 1; j <= 20; j++) {
            t = j / 20.0;
            for (i = 0; i < 2; i++) {
                win = &winData[TeamNameWinId[i]];
                HuSprTPLvlSet(win->grpId, 2, t);
                HuSprTPLvlSet(win->grpId, 3, t);
            }
            TeamNamePosUpdate();
            HuPrcVSleep();
        }
        while (TeamNameFadeF == 0) {
            TeamNamePosUpdate();
            HuPrcVSleep();
        }
        for (j = 1; j <= 30; j++) {
            t = 1.0 - j / 30.0;
            for (i = 0; i < 2; i++) {
                win = &winData[TeamNameWinId[i]];
                HuSprTPLvlSet(win->grpId, 2, t);
                HuSprTPLvlSet(win->grpId, 3, t);
            }
            TeamNamePosUpdate();
            HuPrcVSleep();
        }
        for (i = 0; i < 2; i++) {
            HuWinDispOff(TeamNameWinId[i]);
        }
        while (TeamNameFadeF != 0) {
            HuPrcVSleep();
        }
    }
}

static void TeamNamePosUpdate(void)
{
    s16 i;
    HuVecF pos3D;
    HuVecF pos2D;

    for (i = 0; i < 2; i++) {
        PSVECAdd(&pos3D, &PlayerPosTbl[i * 2], &PlayerPosTbl[i * 2 + 1]);
        PSVECScale(&pos3D, &pos3D, 0.5f);
        Hu3D3Dto2D(&pos3D, 4, &pos2D);
        HuWinPosSet(TeamNameWinId[i], pos2D.x, pos2D.y - 10.0f);
    }
}

static void DispStatTotal(void)
{
    HuVecF pos[4];
    f32 t, s;
    s16 i, j, k;
    s16 num;
    s16 val;

    HuWinMesSet(MessWinId, 0x90002);
    PResComInputWait();
    HuWinMesWait(MessWinId);
    if (PResTeamF == 0) {
        num = 4;
        for (i = 0; i < 4; i++) {
            Hu3DParManColorSet(StarEffAnimP[i], 0);
            Hu3DParManAttrReset(StarEffAnimP[i], 1);
            pos[i] = PlayerPosTbl[i];
            pos[i].y += 70.0f;
        }
    } else {
        num = 2;
        for (i = 0; i < 2; i++) {
            Hu3DParManColorSet(StarEffAnimP[i], 0);
            Hu3DParManAttrReset(StarEffAnimP[i], 1);
            pos[i] = PlayerPosTbl[i];
            pos[i].x = PlayerPosTbl[i * 2].x + (PlayerPosTbl[i * 2 + 1].x - PlayerPosTbl[i * 2].x) * 0.5f;
            pos[i].y += 70.0f;
        }
    }
    for (j = 1; j <= 20; j++) {
        for (i = 0; i < num; i++) {
            Hu3DModelAttrReset(PlayerStarMdlId[i], 1);
            t = 0.1 + 0.9 * (j / 20.0);
            Hu3DModelScaleSet(PlayerStarMdlId[i], t, t, t);
            s = HuSin(90.0 * (j / 20.0));
            Hu3DModelPosSet(PlayerStarMdlId[i], pos[i].x, 200.0f * s + (70.0f + pos[i].y), pos[i].z);
            Hu3DParManPosSet(StarEffAnimP[i], pos[i].x, 200.0f * s + (70.0f + pos[i].y), pos[i].z - 20.0f);
        }
        HuPrcVSleep();
    }
    for (i = 0; i < num; i++) {
        if (PResTeamF == 0) {
            StatNumSet(i, PResPlayer[i].star);
        } else {
            StatNumSet(i, PResPlayer[i * 2].star + PResPlayer[i * 2 + 1].star);
        }
        for (k = 0; k < 4; k++) {
            Hu3DModelRotSet(PlayerNumMdlId[i][k], 0.0f, 180.0f, 0.0f);
        }
    }
    for (j = 1; j <= 20; j++) {
        t = 180.0 * (1.0 - HuSin(90.0 * (j / 20.0)));
        for (i = 0; i < num; i++) {
            for (k = 0; k < 4; k++) {
                Hu3DModelRotSet(PlayerNumMdlId[i][k], 0.0f, t, 0.0f);
            }
            Hu3DModelRotSet(PlayerStarMdlId[i], 0.0f, 180.0f + t, 0.0f);
        }
        if (j == 10) {
            HuAudFXPlay(0x3E4);
        }
        HuPrcVSleep();
    }
    HuWinKeyWaitEntry(MessWinId);
    PResComInputWait();
    HuWinMesWait(MessWinId);
    for (j = 1; j <= 20; j++) {
        t = 500.0 * (1.0 - HuCos(90.0 * (j / 20.0)));
        for (i = 0; i < num; i++) {
            for (k = 0; k < 4; k++) {
                Hu3DModelPosSet(PlayerNumMdlId[i][k], pos[i].x, t + (270.0f + pos[i].y), pos[i].z);
            }
            Hu3DModelPosSet(PlayerStarMdlId[i], pos[i].x, t + (270.0f + pos[i].y), pos[i].z);
            Hu3DParManPosSet(StarEffAnimP[i], pos[i].x, t + (270.0f + pos[i].y), pos[i].z - 20.0f);
        }
        HuPrcVSleep();
    }
    for (i = 0; i < num; i++) {
        Hu3DModelAttrSet(PlayerStarMdlId[i], 1);
    }
    HuWinMesSet(MessWinId, 0x90003);
    PResComInputWait();
    HuWinMesWait(MessWinId);
    for (j = 1; j <= 20; j++) {
        for (i = 0; i < num; i++) {
            Hu3DModelAttrReset(PlayerCoinMdlId[i], 1);
            t = 0.1 + 0.9 * (j / 20.0);
            Hu3DModelScaleSet(PlayerCoinMdlId[i], t, t, t);
            s = HuSin(90.0 * (j / 20.0));
            Hu3DModelPosSet(PlayerCoinMdlId[i], pos[i].x, 200.0f * s + (70.0f + pos[i].y), pos[i].z);
            Hu3DParManPosSet(StarEffAnimP[i], pos[i].x, 200.0f * s + (70.0f + pos[i].y), pos[i].z - 20.0f);
        }
        HuPrcVSleep();
    }
    for (i = 0; i < num; i++) {
        if (PResTeamF == 0) {
            StatNumSet(i, PResPlayer[i].coin);
        } else {
            val = PResPlayer[i * 2].coin;
            StatNumSet(i, val + PResPlayer[i * 2 + 1].coin);
        }
        for (k = 0; k < 4; k++) {
            Hu3DModelRotSet(PlayerNumMdlId[i][k], 0.0f, 180.0f, 0.0f);
        }
    }
    for (j = 1; j <= 20; j++) {
        t = 180.0 * (1.0 - HuSin(90.0 * (j / 20.0)));
        for (i = 0; i < num; i++) {
            for (k = 0; k < 4; k++) {
                Hu3DModelRotSet(PlayerNumMdlId[i][k], 0.0f, t, 0.0f);
            }
            Hu3DModelRotSet(PlayerCoinMdlId[i], 0.0f, 180.0f + t, 0.0f);
        }
        if (j == 10) {
            HuAudFXPlay(0x3E5);
        }
        HuPrcVSleep();
    }
    HuWinKeyWaitEntry(MessWinId);
    PResComInputWait();
    HuWinMesWait(MessWinId);
    for (j = 1; j <= 20; j++) {
        t = 500.0 * (1.0 - HuCos(90.0 * (j / 20.0)));
        for (i = 0; i < num; i++) {
            for (k = 0; k < 4; k++) {
                Hu3DModelPosSet(PlayerNumMdlId[i][k], pos[i].x, t + (270.0f + pos[i].y), pos[i].z);
            }
            Hu3DModelPosSet(PlayerCoinMdlId[i], pos[i].x, t + (270.0f + pos[i].y), pos[i].z);
            Hu3DParManPosSet(StarEffAnimP[i], pos[i].x, t + (270.0f + pos[i].y), pos[i].z - 20.0f);
        }
        HuPrcVSleep();
    }
    for (i = 0; i < num; i++) {
        Hu3DModelAttrSet(PlayerCoinMdlId[i], 1);
        Hu3DParManAttrSet(StarEffAnimP[i], 1);
    }
}

static void StatNumSet(s16 n, s16 val)
{
    char str[8];
    HU3DMODEL *mdl;
    f32 x;
    u8 len;
    s16 i;

    if (val < 0 || val > 9999) {
        val = 0;
    }
    sprintf(str, "%d", val);
    len = strlen(str);
    for (i = 0; i < 4; i++) {
        Hu3DModelAttrSet(PlayerNumMdlId[n][i], 1);
    }
    x = -(len * 30 / 2 - 15);
    for (i = 0; i < len; i++) {
        Hu3DAnimBankSet(PlayerNumAnimId[n][i], (u16)(str[i] & 0xF));
        Hu3DModelAttrReset(PlayerNumMdlId[n][i], 1);
        PSMTXTrans(Hu3DData[PlayerNumMdlId[n][i]].mtx, x, 0.0f, 25.0f);
        mdl = &Hu3DData[PlayerStarMdlId[n]];
        if (mdl->attr & 1) {
            mdl = &Hu3DData[PlayerCoinMdlId[n]];
        }
        Hu3DModelPosSetV(PlayerNumMdlId[n][i], &mdl->pos);
        Hu3DModelScaleSet(PlayerNumMdlId[n][i], 0.8f, 0.8f, 0.8f);
        x += 30.0f;
    }
}

static void ResultBonusExec(s16 type)
{
    HuVecF center;
    HuVecF fadePos[4];
    HuVecF pos[4];
    s16 idx[4];
    s16 got[4];
    s32 se;
    s32 max;
    s16 cnt;
    s16 i, j;
    s16 ofs;
    f32 t, ang, s;
    s16 *stat;

    Hu3DParManColorSet(StarEffAnimP[0], type + 1);
    if (PResTeamF == 0) {
        HuWinMesSet(MessWinId, BonusStarMessTbl[type]);
    } else {
        HuWinMesSet(MessWinId, type + 0x90022);
    }
    PResComInputWait();
    HuWinMesWait(MessWinId);
    for (i = 0; i < 4; i++) {
        pos[i] = PlayerPosTbl[i];
    }
    center.x = PlayerPosTbl[0].x + (PlayerPosTbl[3].x - PlayerPosTbl[0].x) * 0.5f;
    center.y = 70.0f + (270.0f + PlayerPosTbl[0].y);
    center.z = PlayerPosTbl[0].z;
    Hu3DModelPosSetV(BonusStarMdlId[type], &center);
    Hu3DParManPosSet(StarEffAnimP[0], center.x, center.y, center.z - 50.0f);
    se = HuAudFXPlay(0x3E6);
    HuAudFXPlay(0x1F);
    for (j = 1; j <= 20; j++) {
        Hu3DModelAttrReset(BonusStarMdlId[type], 1);
        t = 1.5 * (1.0 - HuCos(90.0 * (j / 20.0)));
        Hu3DModelScaleSet(BonusStarMdlId[type], t, t, t);
        if (j == 10) {
            Hu3DParManAttrReset(StarEffAnimP[0], 1);
        }
        HuPrcVSleep();
    }
    for (j = 1; j <= 120; j++) {
        if (j < 60) {
            ang = 180.0 * (1.0 - HuCos(90.0 * (j / 60.0)));
        } else {
            ang = 180.0 + 180.0 * HuSin(90.0 * ((j - 60) / 60.0));
        }
        Hu3DModelPosSet(BonusStarMdlId[type], center.x + 350.0 * HuSin(ang), center.y + 40.0 * HuSin(2.0f * ang), (center.z + 100.0 * HuCos(ang)) - 100.0);
        Hu3DParManPosSet(StarEffAnimP[0], center.x + 350.0 * HuSin(ang), center.y, ((center.z + 100.0 * HuCos(ang)) - 100.0) - 50.0);
        Hu3DModelRotSet(BonusStarMdlId[type], 0.0f, 25.0 * -HuSin(360.0 * (j / 120.0)), 0.0f);
        HuAudFXPanning(se, (s32)(32.0 * HuSin(ang)) + 0x40);
        HuPrcVSleep();
    }
    if (PResTeamF != 0) {
        for (i = 0; i < 4; i += 2) {
            (&PResPlayer[i].coinTotalMg)[type] += (&PResPlayer[i + 1].coinTotalMg)[type];
            (&PResPlayer[i + 1].coinTotalMg)[type] = 0;
        }
    }
    max = 0;
    for (i = 0; i < 4; i++) {
        if ((u32)(&PResPlayer[i].coinTotalMg)[type] >= max) {
            max = (&PResPlayer[i].coinTotalMg)[type];
        }
    }
    if (max == 0) {
        HuPrcSleep(60);
        if (PResTeamF == 0) {
            HuWinMesSet(MessWinId, BonusStarMessTbl[type + 12]);
        } else {
            HuWinMesSet(MessWinId, type + 0x9001F);
        }
        HuAudFXPlay(0x20);
        HuAudFXPlay(0x2F0);
        for (i = 0; i < 4; i++) {
            CharMotionShiftSet(PResCharNo[i], PResPlayerMotion[i][3], 0.0f, 8.0f, 0x40000001);
        }
        PResComInputWait();
        HuWinMesWait(MessWinId);
        for (j = 1; j <= 20; j++) {
            t = 1.5 * (1.0 - HuSin(90.0 * (j / 20.0)));
            Hu3DModelScaleSet(BonusStarMdlId[type], t, t, t);
            if (j == 5) {
                Hu3DParManAttrSet(StarEffAnimP[0], 1);
            }
            if (j == 10) {
                HuAudFXStop(se);
                for (i = 0; i < 4; i++) {
                    CharMotionShiftSet(PResCharNo[i], PResPlayerMotion[i][0], 0.0f, 8.0f, 0x40000001);
                }
            }
            HuPrcVSleep();
        }
        Hu3DModelAttrSet(BonusStarMdlId[type], 1);
        return;
    }
    cnt = 0;
    for (i = 0; i < 4; i++) {
        if (max == (&PResPlayer[i].coinTotalMg)[type]) {
            idx[cnt++] = i;
        }
    }
    HuPrcSleep(30);
    HuAudFXPlay(0x20);
    if (PResTeamF == 0) {
        switch (cnt) {
            case 4:
                HuWinMesSet(MessWinId, BonusStarMessTbl[type + 9]);
                HuAudFXPlay(0x2EF);
                for (i = 0; i < 4; i++) {
                    CharMotionShiftSet(PResCharNo[i], PResPlayerMotion[i][3], 0.0f, 8.0f, 0x40000001);
                }
                PResComInputWait();
                HuWinMesWait(MessWinId);
                for (j = 1; j <= 20; j++) {
                    t = 1.5 * (1.0 - HuSin(90.0 * (j / 20.0)));
                    Hu3DModelScaleSet(BonusStarMdlId[type], t, t, t);
                    if (j == 5) {
                        Hu3DParManAttrSet(StarEffAnimP[0], 1);
                    }
                    if (j == 10) {
                        HuAudFXStop(se);
                        for (i = 0; i < 4; i++) {
                            CharMotionShiftSet(PResCharNo[i], PResPlayerMotion[i][0], 0.0f, 8.0f, 0x40000001);
                        }
                    }
                    HuPrcVSleep();
                }
                Hu3DModelAttrSet(BonusStarMdlId[type], 1);
                return;
            case 3:
                HuWinMesSet(MessWinId, BonusStarMessTbl[type + 6]);
                HuAudFXPlay(0x2EF);
                PResComInputWait();
                HuWinMesWait(MessWinId);
                break;
            case 2:
                HuWinMesSet(MessWinId, BonusStarMessTbl[type + 3]);
                HuAudFXPlay(0x2EF);
                PResComInputWait();
                HuWinMesWait(MessWinId);
                break;
            default:
                break;
        }
        HuAudFXStop(se);
        for (i = 0; i < cnt; i++) {
            fadePos[i] = pos[idx[i]];
            fadePos[i].y += 340.0f;
        }
        BonusFadeCreate(BonusStarMdlId[type], cnt, fadePos);
        for (i = 0; i < cnt; i++) {
            HuWinInsertMesSet(MessWinId, PResCharNo[idx[i]], i);
        }
        switch (cnt) {
            case 1:
                HuWinMesSet(MessWinId, 0x90008);
                break;
            case 2:
                HuWinMesSet(MessWinId, 0x90009);
                break;
            case 3:
                HuWinMesSet(MessWinId, 0x9000A);
                break;
        }
        PResComInputWait();
        HuWinMesWait(MessWinId);
        HuPrcSleep(30);
        for (j = 1; j <= 30; j++) {
            for (i = 0; i < cnt; i++) {
                t = j / 30.0;
                Hu3DModelAttrReset(PlayerStarMdlId[i], 1);
                s = HuCos(90.0f * t);
                Hu3DModelPosSet(PlayerStarMdlId[i], pos[idx[i]].x, 70.0 + (100.0 + (pos[idx[i]].y + 170.0 * s)), pos[idx[i]].z);
                Hu3DParManPosSet(StarEffAnimP[i], pos[idx[i]].x, 70.0 + (100.0 + (pos[idx[i]].y + 170.0 * s)), pos[idx[i]].z - 20.0f);
                Hu3DModelRotSet(PlayerStarMdlId[i], 0.0f, 360.0f * t, 0.0f);
                t = 1.2 * HuCos(90.0f * t);
                Hu3DModelScaleSet(PlayerStarMdlId[i], t, t, t);
                if (j == 10) {
                    Hu3DParManAttrSet(StarEffAnimP[i], 1);
                }
            }
            if (j == 20) {
                got[3] = 0;
                got[2] = 0;
                got[1] = 0;
                got[0] = 0;
                for (i = 0; i < cnt; i++) {
                    ResultCharFXPlay(PResCharNo[idx[i]], 0x1F8);
                    CharMotionShiftSet(PResCharNo[idx[i]], PResPlayerMotion[idx[i]][2], 0.0f, 8.0f, 0x40000001);
                    got[idx[i]] = 1;
                }
                for (i = 0; i < 4; i++) {
                    if (got[i] == 0) {
                        CharMotionShiftSet(PResCharNo[i], PResPlayerMotion[i][3], 0.0f, 8.0f, 0x40000001);
                    }
                }
            }
            HuPrcVSleep();
        }
        HuPrcSleep(60);
        for (i = 0; i < 4; i++) {
            CharMotionShiftSet(PResCharNo[i], PResPlayerMotion[i][0], 0.0f, 8.0f, 0x40000001);
        }
        HuPrcSleep(8);
        for (i = 0; i < cnt; i++) {
            PResPlayer[idx[i]].star += 1;
        }
        return;
    }
    if (cnt > 1) {
        HuWinMesSet(MessWinId, 0x9001E);
        HuAudFXPlay(0x2EF);
        for (i = 0; i < 4; i++) {
            CharMotionShiftSet(PResCharNo[i], PResPlayerMotion[i][3], 0.0f, 8.0f, 0x40000001);
        }
        PResComInputWait();
        HuWinMesWait(MessWinId);
        for (j = 1; j <= 20; j++) {
            t = 1.5 * (1.0 - HuSin(90.0 * (j / 20.0)));
            Hu3DModelScaleSet(BonusStarMdlId[type], t, t, t);
            if (j == 5) {
                Hu3DParManAttrSet(StarEffAnimP[0], 1);
            }
            if (j == 10) {
                HuAudFXStop(se);
                for (i = 0; i < 4; i++) {
                    CharMotionShiftSet(PResCharNo[i], PResPlayerMotion[i][0], 0.0f, 8.0f, 0x40000001);
                }
            }
            HuPrcVSleep();
        }
        Hu3DModelAttrSet(BonusStarMdlId[type], 1);
        return;
    }
    HuAudFXStop(se);
    fadePos[0].x = PlayerPosTbl[idx[0]].x + (PlayerPosTbl[idx[0] + 1].x - PlayerPosTbl[idx[0]].x) * 0.5f;
    fadePos[0].y = 70.0f + (270.0f + pos[0].y);
    fadePos[0].z = pos[0].z;
    BonusFadeCreate(BonusStarMdlId[type], 1, fadePos);
    HuWinInsertMesSet(MessWinId, PResTeamNameMesGet(PResCharNo[idx[0]], PResCharNo[idx[0] + 1]), 0);
    HuWinMesSet(MessWinId, 0x90008);
    PResComInputWait();
    HuWinMesWait(MessWinId);
    HuPrcSleep(30);
    for (j = 1; j <= 30; j++) {
        t = j / 30.0;
        Hu3DModelAttrReset(PlayerStarMdlId[0], 1);
        Hu3DModelPosSet(PlayerStarMdlId[0], fadePos[0].x, (fadePos[0].y - 170.0f) + 170.0 * HuCos(90.0f * t), fadePos[0].z);
        Hu3DParManPosSet(StarEffAnimP[0], fadePos[0].x, (fadePos[0].y - 170.0f) + 170.0 * HuCos(90.0f * t), fadePos[0].z - 20.0f);
        Hu3DModelRotSet(PlayerStarMdlId[0], 0.0f, 360.0f * t, 0.0f);
        t = 1.2 * HuCos(90.0f * t);
        Hu3DModelScaleSet(PlayerStarMdlId[0], t, t, t);
        if (j == 10) {
            Hu3DParManAttrSet(StarEffAnimP[0], 1);
        }
        if (j == 20) {
            ofs = idx[0] & 2;
            for (i = 0; i < 2; i++) {
                CharMotionShiftSet(PResCharNo[ofs + i], PResPlayerMotion[ofs + i][2], 0.0f, 8.0f, 0x40000001);
                ResultCharFXPlay(PResCharNo[ofs + i], 0x1F8);
            }
            ofs = (idx[0] & 2) ^ 2;
            for (i = 0; i < 2; i++) {
                CharMotionShiftSet(PResCharNo[ofs + i], PResPlayerMotion[ofs + i][3], 0.0f, 8.0f, 0x40000001);
            }
        }
        HuPrcVSleep();
    }
    HuPrcSleep(60);
    for (i = 0; i < 4; i++) {
        CharMotionShiftSet(PResCharNo[i], PResPlayerMotion[i][0], 0.0f, 8.0f, 0x40000001);
    }
    HuPrcSleep(8);
    PResPlayer[idx[0]].star += 1;
}

static void PResResultFinal(void)
{
    HuVecF rot[4];
    HuVecF center;
    HuVecF lerp;
    s16 order[4];
    s16 idx[4];
    s16 rank[4];
    s16 winCnt;
    s16 winner;
    s32 se;
    s32 jingle;
    f32 speed;
    f32 t;
    s16 i, j;
    s16 n;

    se = 0;
    center.x = PlayerPosTbl[0].x + (PlayerPosTbl[3].x - PlayerPosTbl[0].x) * 0.5f;
    center.y = PlayerPosTbl[0].y;
    center.z = PlayerPosTbl[0].z;
    for (i = 0; i < 4; i++) {
        rot[i].x = frandmod(10) - 5;
        rot[i].y = frandmod(10) - 5;
        rot[i].z = frandmod(10) - 5;
    }
    winCnt = PResResultOrderGet(&order, &idx, &rank);
    HuWinMesSet(MessWinId, 0x90011);
    PResComInputWait();
    HuWinMesWait(MessWinId);
    HuAudSStreamPauseFadeOut(0, 1, 500);
    HuWinMesSet(MessWinId, 0x90012);
    jingle = HuAudJinglePlay(0x46);
    speed = 1.0f;
    for (i = 0; i < 60; i++) {
        Hu3DMotionSpeedSet(StageMdlId, speed);
        speed += 0.1;
        if (i == 20) {
            se = HuAudFXPlay(0x3E9);
            for (j = 0; j < 4; j++) {
                CharMotionShiftSet(PResCharNo[j], PResPlayerMotion[j][5], 0.0f, 8.0f, 0x40000001);
            }
        }
        HuPrcVSleep();
    }
    PResComInputWait();
    HuWinMesWait(MessWinId);
    HuWinExClose(MessWinId);
    omCameraViewMoveMulti(4, &FinalResultView, 10, 1);
    if (winCnt > 1) {
        HuPrcSleep(60);
        HuWinExOpen(MessWinId);
        HuWinMesSet(MessWinId, 0x90013);
        HuAudFXPlay(0x2EF);
        HuAudSStreamFadeOut(jingle, 1000);
        HuAudSStreamPause(0, 0);
        omCameraViewMoveMulti(4, &MainResultView, 30, 0);
        for (i = 0; i < 30; i++) {
            Hu3DMotionSpeedSet(StageMdlId, speed);
            if (i == 10) {
                for (j = 0; j < 4; j++) {
                    CharMotionShiftSet(PResCharNo[j], PResPlayerMotion[j][0], 0.0f, 8.0f, 0x40000001);
                }
                HuAudFXStop(se);
            }
            speed -= 0.2;
            HuPrcVSleep();
        }
        PResComInputWait();
        HuWinMesWait(MessWinId);
        HuWinMesSet(MessWinId, 0x90014);
        PResComInputWait();
        HuWinMesWait(MessWinId);
        order[0] = PResResultTie();
        PResPlayer[order[0]].bonusStar = 1;
        HuWinMesSet(MessWinId, 0x90016);
        PResComInputWait();
        HuWinMesWait(MessWinId);
        HuAudSStreamAllFadeOut(500);
        HuWinExClose(MessWinId);
        omCameraViewMoveMulti(4, &FinalResultView, 10, 1);
        HuAudJinglePlay(0x46);
        for (i = 0; i < 60; i++) {
            Hu3DMotionSpeedSet(StageMdlId, speed);
            speed += 0.1;
            if (i == 20) {
                se = HuAudFXPlay(0x3E9);
                for (j = 0; j < 4; j++) {
                    CharMotionShiftSet(PResCharNo[j], PResPlayerMotion[j][5], 0.0f, 8.0f, 0x40000001);
                }
            }
            HuPrcVSleep();
        }
        HuPrcSleep(60);
        for (j = 0; j < 60; j++) {
            for (i = 0; i < 4; i++) {
                if (j >= i * 5 && j <= i * 5 + 30 && i != order[0]) {
                    if (j == i * 5) {
                        CharMotionShiftSet(PResCharNo[i], PResPlayerMotion[i][6], 0.0f, 8.0f, 0x40000001);
                        HuAudFXPlay(0x3EA);
                    }
                    t = j - i * 5;
                    Hu3DModelPosSet(PResPlayerModelId[i], PlayerPosTbl[i].x, PlayerPosTbl[i].y, PlayerPosTbl[i].z - 150.0f * t);
                    Hu3DModelRotSet(PResPlayerModelId[i], t * rot[i].x, t * rot[i].y, t * rot[i].z);
                    ResultCharFXUpdate(i);
                    t = 0.3 + 0.7 * (1.0 - t / 30.0);
                    Hu3DModelScaleSet(PResPlayerModelId[i], t, t, t);
                    Hu3DModelLayerSet(PResPlayerModelId[i], 0);
                    if (j == i * 5 + 30) {
                        Hu3DModelAttrSet(PResPlayerModelId[i], 1);
                    }
                }
            }
            HuPrcVSleep();
        }
    } else {
        for (n = 3; n >= 1; n--) {
            HuPrcSleep((s16)(frandmod(60) + 60));
            winner = (&order[0])[n];
            CharMotionShiftSet(PResCharNo[winner], PResPlayerMotion[winner][6], 0.0f, 8.0f, 0x40000001);
            HuAudFXPlay(0x3EA);
            for (j = 1; j <= 30; j++) {
                Hu3DModelPosSet(PResPlayerModelId[winner], PlayerPosTbl[winner].x, PlayerPosTbl[winner].y, PlayerPosTbl[winner].z - j * 150);
                ResultCharFXUpdate(winner);
                t = 0.3 + 0.7 * (1.0 - j / 30.0);
                Hu3DModelScaleSet(PResPlayerModelId[winner], t, t, t);
                Hu3DModelRotSet(PResPlayerModelId[winner], (f32)j * rot[winner].x, (f32)j * rot[winner].y, (f32)j * rot[winner].z);
                Hu3DModelLayerSet(PResPlayerModelId[winner], 0);
                HuPrcVSleep();
            }
            Hu3DModelAttrSet(PResPlayerModelId[winner], 1);
        }
    }
    HuWinExOpen(MessWinId);
    HuWinInsertMesSet(MessWinId, PResCharNo[order[0]], 0);
    HuWinMesSet(MessWinId, 0x90017);
    for (i = 0; i < 4; i++) {
        CharMotionShiftSet(PResCharNo[i], PResPlayerMotion[i][0], 0.0f, 8.0f, 0x40000001);
    }
    for (j = 1; j <= 60; j++) {
        PSVECSubtract(&center, &PlayerPosTbl[order[0]], &lerp);
        PSVECScale(&lerp, &lerp, HuSin(90.0f * (f32)(j / 60.0)));
        PSVECAdd(&PlayerPosTbl[order[0]], &lerp, &lerp);
        Hu3DModelPosSetV(PResPlayerModelId[order[0]], &lerp);
        ResultCharFXUpdate(order[0]);
        speed -= 0.1;
        Hu3DMotionSpeedSet(StageMdlId, speed);
        if (j == 30) {
            HuAudFXStop(se);
        }
        HuPrcVSleep();
    }
    PResComInputWait();
    HuWinMesWait(MessWinId);
    HuAudSStreamAllFadeOut(500);
    HuAudBGMPlay(0x47);
    HuWinExClose(MessWinId);
    PResResultWinnerExec(order[0], -1, CharModelHeightGet(PResCharNo[order[0]]));
    PResBtnAWait();
}

static s16 PResResultTie(void)
{
    HuVecF center;
    HuVecF start;
    HuVecF tmp;
    s16 order[4];
    s16 idx[4];
    s16 rank[4];
    s16 cnt;
    s16 tries;
    s16 winner;
    s16 best;
    s16 a, b;
    s16 tmp2;
    s16 i, j;
    s16 p;
    s16 winId;

    center.x = PlayerPosTbl[0].x + (PlayerPosTbl[3].x - PlayerPosTbl[0].x) * 0.5f;
    center.y = 70.0f + (100.0f + PlayerPosTbl[0].y);
    center.z = PlayerPosTbl[0].z;
    tries = 0;
retry:
    for (i = 0; i < 10; i++) {
        BlockResult[i] = i + 1;
    }
    for (i = 0; i < 50; i++) {
        a = frandmod(10);
        b = frandmod(10);
        tmp2 = BlockResult[a];
        BlockResult[a] = BlockResult[b];
        BlockResult[b] = tmp2;
    }
    if (PResTeamF == 0) {
        cnt = PResResultOrderGet(&order, &idx, &rank);
    } else {
        cnt = 4;
        for (i = 0; i < 4; i++) {
            order[i] = i;
        }
        if (BlockResult[0] + BlockResult[1] == BlockResult[2] + BlockResult[3]) {
            tries++;
            if (tries >= 100) {
                if (frandmod(100) < 50) {
                    BlockResult[0] = 8;
                    BlockResult[1] = 9;
                    BlockResult[2] = 6;
                    BlockResult[3] = 5;
                } else {
                    BlockResult[0] = 5;
                    BlockResult[1] = 7;
                    BlockResult[2] = 6;
                    BlockResult[3] = 9;
                }
            } else {
                goto retry;
            }
        }
    }
    HuAudFXPlay(0x35C);
    HuAudFXPlay(0x2F9);
    for (j = 1; j <= 10; j++) {
        for (i = 0; i < cnt; i++) {
            p = order[i];
            Hu3DModelAttrReset(BlockMdlId[p], 1);
            Hu3DModelAttrSet(BlockMdlId[p], 0x40000001);
            start = PlayerPosTbl[p];
            start.y += 370.0f;
            PSVECSubtract(&start, &center, &tmp);
            PSVECScale(&tmp, &tmp, HuSin(90.0f * (f32)(j / 10.0)));
            PSVECAdd(&center, &tmp, &tmp);
            Hu3DModelPosSetV(BlockMdlId[p], &tmp);
        }
        HuPrcVSleep();
    }
    BlockNum = 0;
    BlockMode = 0;
    for (i = 0; i < cnt; i++) {
        HuPrcChildCreate(BlockPlayerProc, 1000, 0x2000, 0, objman)->property = (void *)(uintptr_t)order[i];
        BlockPlayerMode[i] = 0;
        BlockSeNo[order[i]] = HuAudFXPlay(0x2F7);
    }
    winId = HuWinCreate(-10000.0f, 330.0f, 0x230, 0x2A, 0);
    HuWinBGTPLvlSet(winId, 0.0f);
    HuWinPriSet(winId, 10);
    HuWinAttrSet(winId, 0x800);
    HuWinMesSpeedSet(winId, 0);
    if (PResTeamF != 0) {
        HuWinPosSet(winId, 40.0f, 300.0f);
    }
    HuWinMesSet(winId, 0x20009);
    while (BlockNum < cnt) {
        HuPrcVSleep();
    }
    HuWinKill(winId);
    if (PResTeamF == 0) {
        best = 0;
        for (i = 0; i < cnt; i++) {
            if (best < BlockResult[order[i]]) {
                winner = order[i];
                best = BlockResult[winner];
            }
        }
        HuWinInsertMesSet(MessWinId, PResCharNo[winner], 0);
        HuWinMesSet(MessWinId, 0x90015);
        ResultCharFXPlay(PResCharNo[winner], 0x1F8);
        CharMotionShiftSet(PResCharNo[winner], PResPlayerMotion[winner][2], 0.0f, 8.0f, 0x40000001);
        for (i = 0; i < 4; i++) {
            if (i != winner) {
                CharMotionShiftSet(PResCharNo[i], PResPlayerMotion[i][3], 0.0f, 16.0f, 0x40000001);
            }
        }
        HuPrcSleep(60);
        PResComInputWait();
        HuWinMesWait(MessWinId);
    } else {
        if (BlockResult[0] + BlockResult[1] > BlockResult[2] + BlockResult[3]) {
            winner = 0;
        } else {
            winner = 1;
        }
        HuWinInsertMesSet(MessWinId, PResCharNo[winner * 2], 0);
        HuWinInsertMesSet(MessWinId, PResCharNo[winner * 2 + 1], 1);
        HuWinInsertMesSet(MessWinId, PResTeamNameMesGet(PResCharNo[winner * 2], PResCharNo[winner * 2 + 1]), 2);
        HuWinMesSet(MessWinId, 0x90019);
        if (winner == 0) {
            ResultCharFXPlay(PResCharNo[0], 0x1F8);
            ResultCharFXPlay(PResCharNo[1], 0x1F8);
            CharMotionShiftSet(PResCharNo[0], PResPlayerMotion[0][2], 0.0f, 8.0f, 0x40000001);
            CharMotionShiftSet(PResCharNo[1], PResPlayerMotion[1][2], 0.0f, 8.0f, 0x40000001);
            CharMotionShiftSet(PResCharNo[2], PResPlayerMotion[2][3], 0.0f, 16.0f, 0x40000001);
            CharMotionShiftSet(PResCharNo[3], PResPlayerMotion[3][3], 0.0f, 16.0f, 0x40000001);
        } else {
            ResultCharFXPlay(PResCharNo[2], 0x1F8);
            ResultCharFXPlay(PResCharNo[3], 0x1F8);
            CharMotionShiftSet(PResCharNo[0], PResPlayerMotion[0][3], 0.0f, 16.0f, 0x40000001);
            CharMotionShiftSet(PResCharNo[1], PResPlayerMotion[1][3], 0.0f, 16.0f, 0x40000001);
            CharMotionShiftSet(PResCharNo[2], PResPlayerMotion[2][2], 0.0f, 8.0f, 0x40000001);
            CharMotionShiftSet(PResCharNo[3], PResPlayerMotion[3][2], 0.0f, 8.0f, 0x40000001);
        }
        HuPrcSleep(60);
        PResComInputWait();
        HuWinMesWait(MessWinId);
    }
    BlockMode = 1;
    for (i = 0; i < 4; i++) {
        CharMotionShiftSet(PResCharNo[i], PResPlayerMotion[i][0], 0.0f, 4.0f, 0x40000001);
    }
    return winner;
}

static void BlockPlayerProc(void)
{
    HuVecF pos;
    f32 h;
    s16 p;
    s16 j;

    p = (s16)(uintptr_t)HuPrcCurrentGet()->property;
    if (GwPlayerConf[PResPlayerOrder[p]].type == 0) {
        while (!(HuPadBtnDown[GwPlayerConf[PResPlayerOrder[p]].padNo] & 0x100)) {
            HuPrcVSleep();
        }
    } else {
        HuPrcSleep((s16)(frandmod(30) + 30));
    }
    CharMotionShiftSet(PResCharNo[p], PResPlayerMotion[p][1], 0.0f, 4.0f, 0);
    for (j = 1; j <= 30; j++) {
        pos = PlayerPosTbl[p];
        h = 300.0f - CharModelHeightGet(PResCharNo[p]);
        pos.y = pos.y + HuSin(180.0f * (f32)(j / 30.0)) * h;
        Hu3DModelPosSetV(PResPlayerModelId[p], &pos);
        ResultCharFXUpdate(p);
        if (j >= 12 && j <= 15) {
            pos = PlayerPosTbl[p];
            pos.y += (j - 7) * 5 + 370;
            Hu3DModelPosSetV(BlockMdlId[p], &pos);
            if (j == 15) {
                HuAudFXStop(BlockSeNo[p]);
                HuAudFXPlay(0x2FA);
                HuPrcChildCreate(BlockProc, 1000, 0x2000, 0, objman)->property = (void *)(uintptr_t)p;
            }
        }
        HuPrcVSleep();
    }
    CharMotionShiftSet(PResCharNo[p], PResPlayerMotion[p][0], 0.0f, 4.0f, 0x40000001);
    HuPrcEnd();
    while (1) {
        HuPrcVSleep();
    }
}

static void BlockProc(void)
{
    HuVecF dest;
    HuVecF from;
    HuVecF tmp;
    HU3DMODEL *mdl;
    s16 nums[2];
    s16 p, other;
    s16 par;
    s16 i, j;
    s16 sum;
    f32 t;
    f32 w;

    p = (s16)(uintptr_t)HuPrcCurrentGet()->property;
    mdl = &Hu3DData[BlockMdlId[p]];
    par = Hu3DParManCreate(DustAnimP, 50, &BlockDustParManParam);
    Hu3DParManAttrSet(par, 0x25);
    Hu3DParManRotSet(par, 0.0f, 0.0f, 0.0f);
    Hu3DModelLayerSet(Hu3DParManModelIDGet(par), 2);
    Hu3DModelCameraSet(Hu3DParManModelIDGet(par), 4);
    Hu3DParManPosSet(par, mdl->pos.x, mdl->pos.y, 50.0f + mdl->pos.z);
    Hu3DParManTimeLimitSet(par, 2);
    Hu3DParManAttrReset(par, 1);
    HuPrcSleep(3);
    Hu3DModelAttrSet(BlockMdlId[p], 1);
    HuPrcSleep(10);
    if (BlockResult[p] > 9) {
        nums[0] = NumberMdlId[p][BlockResult[p] / 10];
        Hu3DModelAttrReset(nums[0], 1);
        Hu3DModelPosSet(nums[0], mdl->pos.x - 40.0f, mdl->pos.y, mdl->pos.z);
        nums[1] = NumberMdlId[p][BlockResult[p] % 10];
        Hu3DModelAttrReset(nums[1], 1);
        Hu3DModelPosSet(nums[1], 40.0f + mdl->pos.x, mdl->pos.y, mdl->pos.z);
    } else {
        nums[0] = NumberMdlId[p][BlockResult[p]];
        Hu3DModelAttrReset(nums[0], 1);
        Hu3DModelPosSet(nums[0], mdl->pos.x, mdl->pos.y, mdl->pos.z);
        nums[1] = -1;
    }
    for (j = 1; j <= 20; j++) {
        t = j / 20.0;
        for (i = 0; i < 2; i++) {
            if (nums[i] != -1) {
                Hu3DModelRotSet(nums[i], 0.0f, 360.0f * t, 0.0f);
                Hu3DModelScaleSet(nums[i], 0.5 + 0.5 * t, 0.5 + 0.5 * t, 0.5 + 0.5 * t);
                if (nums[1] == -1) {
                    Hu3DModelPosSet(nums[i], mdl->pos.x, mdl->pos.y - 50.0 * (1.0 - HuCos(90.0f * t)), mdl->pos.z);
                } else {
                    Hu3DModelPosSet(nums[i], (mdl->pos.x - 40.0f) + i * 80, mdl->pos.y - 50.0 * (1.0 - HuCos(90.0f * t)), mdl->pos.z);
                }
            }
        }
        HuPrcVSleep();
    }
    if (PResTeamF != 0) {
        other = p ^ 1;
        BlockPlayerMode[p] = 2;
        while (BlockPlayerMode[other] != 2) {
            HuPrcVSleep();
        }
        HuPrcSleep(30);
        dest = PlayerPosTbl[p];
        dest.x = (PlayerPosTbl[p].x + PlayerPosTbl[other].x) * 0.5f;
        dest.y = mdl->pos.y - 50.0f;
        for (j = 0; j < 10; j++) {
            for (i = 0; i < 2; i++) {
                if (nums[i] != -1) {
                    from.x = mdl->pos.x;
                    from.y = mdl->pos.y;
                    from.z = mdl->pos.z;
                    from.y -= 50.0f;
                    if (nums[1] != -1) {
                        from.x += i * 80 - 40;
                    }
                    PSVECSubtract(&dest, &from, &tmp);
                    PSVECScale(&tmp, &tmp, HuSin(90.0f * (f32)(j / 10.0)));
                    PSVECAdd(&from, &tmp, &tmp);
                    Hu3DModelPosSetV(nums[i], &tmp);
                }
            }
            HuPrcVSleep();
        }
        Hu3DModelAttrSet(nums[0], 1);
        if (nums[1] != -1) {
            Hu3DModelAttrSet(nums[1], 1);
        }
        sum = BlockResult[p] + BlockResult[other];
        if (p & 1) {
            if (sum >= 10) {
                nums[0] = NumberMdlId[p][sum / 10];
                Hu3DModelAttrReset(nums[0], 1);
                tmp = dest;
                tmp.x = dest.x - 40.0f;
                Hu3DModelPosSetV(nums[0], &tmp);
            }
        } else {
            nums[0] = NumberMdlId[p][sum % 10];
            Hu3DModelAttrReset(nums[0], 1);
            tmp = dest;
            if (sum >= 10) {
                tmp.x += 40.0f;
            }
            Hu3DModelPosSetV(nums[0], &tmp);
        }
        HuAudFXPlay(0x3ED);
        for (j = 1; j <= 15; j++) {
            w = 0.2 * HuSin(180.0 + 360.0 * (j / 15.0));
            t = 0.3 + 0.7 * HuSin(90.0 * (j / 15.0)) + w;
            Hu3DModelScaleSet(nums[0], t, t, t);
            HuPrcVSleep();
        }
    }
    BlockNum++;
    while (BlockMode == 0) {
        HuPrcVSleep();
    }
    for (j = 1; j <= 10; j++) {
        t = HuCos(90.0 * (j / 10.0));
        for (i = 0; i < 10; i++) {
            Hu3DModelTPLvlSet(NumberMdlId[p][i], t);
            Hu3DModelScaleSet(NumberMdlId[p][i], 1.0 + 0.5 * (1.0 - t), 1.0 + 0.5 * (1.0 - t), 1.0 + 0.5 * (1.0 - t));
        }
        HuPrcVSleep();
    }
    Hu3DParManKill(par);
    HuPrcEnd();
    while (1) {
        HuPrcVSleep();
    }
}

static void PResResultWinnerExec(s16 charNo1, s16 charNo2, f32 height)
{
    HuVecF pos;
    HuVecF p0;
    HuVecF p1;
    HU3DMODEL *mdl;
    f32 zoom0, centerY0;
    f32 v, ang, t, r, rx;
    s32 se;
    s16 j, k, m;
    s16 charNo;
    s16 board;
    s16 sum;

    Hu3DParManColorSet(StarEffAnimP[0], 0);
    Hu3DParManAttrReset(StarEffAnimP[0], 1);
    zoom0 = CZoomM[2];
    centerY0 = CenterM[2].y;
    Hu3DModelAttrReset(StarMdlId, 1);
    Hu3DModelAttrSet(StarMdlId, 0x40000002);
    Hu3DModelScaleSet(StarMdlId, 0.6f, 0.6f, 0.6f);
    mdl = &Hu3DData[PResPlayerModelId[charNo1]];
    p0.x = mdl->pos.x;
    p0.y = mdl->pos.y;
    p0.z = mdl->pos.z;
    if (charNo2 != -1) {
        mdl = &Hu3DData[PResPlayerModelId[charNo2]];
        p1.x = mdl->pos.x;
        p1.y = mdl->pos.y;
        p1.z = mdl->pos.z;
    }
    se = HuAudFXPlay(0x3EB);
    for (j = 1; j <= 180; j++) {
        if (j < 90) {
            v = 0.5 * (1.0 - HuCos(90.0 * (j / 90.0)));
        } else {
            v = 0.5 + 0.5 * HuSin(90.0 * ((j - 90) / 90.0));
        }
        CRotM[2].x = 30.0 * -HuSin(330.0f * v);
        ang = 2.0f * (360.0f * v);
        t = j / 180.0;
        CZoomM[2] = zoom0 - 270.0f * t;
        CenterM[2].y = centerY0 + (height - 150.0f) * 0.5f * t;
        r = 350.0 * (1.0 - t);
        rx = HuCos(CRotM[2].x);
        pos.x = CenterM[2].x - r * (HuSin(ang) * rx);
        pos.y = CenterM[2].y - r * -HuSin(CRotM[2].x);
        pos.z = CenterM[2].z - r * (HuCos(ang) * HuCos(CRotM[2].x));
        pos.y += 150.0f * t;
        pos.z -= 100.0f * t;
        HuAudFXPanning(se, (s32)(f32)((r / 350.0) * (32.0 * (HuSin(ang) * HuCos(CRotM[2].x)))) + 0x40);
        Hu3DModelPosSetV(StarMdlId, &pos);
        Hu3DParManPosSet(StarEffAnimP[0], pos.x, pos.y, pos.z - 20.0f);
        if (j == 60) {
            CharMotionShiftSet(PResCharNo[charNo1], PResPlayerMotion[charNo1][4], 0.0f, 10.0f, 0);
            if (charNo2 != -1) {
                CharMotionShiftSet(PResCharNo[charNo2], PResPlayerMotion[charNo2][4], 0.0f, 10.0f, 0);
            }
        }
        if (j > 60) {
            t = (j - 60) / 120.0;
            charNo = PResCharNo[charNo1];
            Hu3DModelRotSet(PResPlayerModelId[charNo1], t * (f32)WinnerRotXTbl[charNo], 0.0f, 0.0f);
            mdl = &Hu3DData[PResPlayerModelId[charNo1]];
            if (charNo == 3) {
                mdl->pos.y = p0.y + 15.0f * t;
            } else if (charNo == 6) {
                mdl->pos.y = p0.y - 10.0f * t;
            }
            mdl->pos.z = p0.z - 70.0 * HuSin(t * (f32)WinnerRotXTbl[charNo]);
            if (charNo2 != -1) {
                charNo = PResCharNo[charNo2];
                Hu3DModelRotSet(PResPlayerModelId[charNo2], t * (f32)WinnerRotXTbl[charNo], 0.0f, 0.0f);
                mdl = &Hu3DData[PResPlayerModelId[charNo2]];
                if (charNo == 3) {
                    mdl->pos.y = p1.y + 15.0f * t;
                } else if (charNo == 6) {
                    mdl->pos.y = p1.y - 10.0f * t;
                }
                mdl->pos.z = p1.z - 70.0 * HuSin(t * (f32)WinnerRotXTbl[charNo]);
            }
        }
        HuPrcVSleep();
    }
    Hu3DLayerHookSet(1, BGFilter);
    Hu3DModelAttrReset(StarMdlId, 0x40000002);
    HuAudFXStop(se);
    HuAudFXPlay(0x3EC);
    HuPrcSleep(30);
    HuSprAttrReset(WinnerSprGrpId, 0, 4);
    HuSprAttrReset(WinnerSprGrpId, 1, 4);
    for (j = 1; j <= 40; j++) {
        HuSprTPLvlSet(WinnerSprGrpId, 0, j / 40.0);
        HuSprTPLvlSet(WinnerSprGrpId, 1, 1.0 - j / 40.0);
        HuPrcVSleep();
    }
    board = GwSystem.boardNo;
    GwCommon.playNum[board] += 1;
    if (GwCommon.playNum[board] > 9999) {
        GwCommon.playNum[board] = 9999;
    }
    charNo = PResCharNo[charNo1];
    if (GwPlayerConf[PResPlayerOrder[charNo1]].type == 0) {
        GwCommon.charPlayNum[board][charNo] += 1;
        if (GwCommon.charPlayNum[board][charNo] > 999) {
            GwCommon.charPlayNum[board][charNo] = 999;
        }
    }
    if (charNo2 != -1) {
        charNo = PResCharNo[charNo2];
        if (GwPlayerConf[PResPlayerOrder[charNo2]].type == 0) {
            GwCommon.charPlayNum[board][charNo] += 1;
            if (GwCommon.charPlayNum[board][charNo] > 999) {
                GwCommon.charPlayNum[board][charNo] = 999;
            }
        }
        for (k = 0; k < 4; k += 2) {
            sum = 0;
            for (m = k; m < k + 2; m++) {
                sum += PResPlayer[m].star;
            }
            for (m = k; m < k + 2; m++) {
                charNo = PResCharNo[m];
                if (GwPlayerConf[PResPlayerOrder[m]].type == 0) {
                    if (GwCommon.boardMaxStar[board][charNo] < sum) {
                        GwCommon.boardMaxStar[board][charNo] = sum;
                    }
                    if (GwCommon.boardMaxCoin[board][charNo] < GwPlayer[PResPlayerOrder[m]].coinMax) {
                        GwCommon.boardMaxCoin[board][charNo] = GwPlayer[PResPlayerOrder[m]].coinMax;
                    }
                }
            }
        }
        return;
    }
    for (k = 0; k < 4; k++) {
        charNo = GwPlayerConf[k].charNo;
        if (GwPlayerConf[k].type == 0) {
            if (GwCommon.boardMaxStar[board][charNo] < PResPlayer[k].star) {
                GwCommon.boardMaxStar[board][charNo] = PResPlayer[k].star;
            }
            if (GwCommon.boardMaxCoin[board][charNo] < GwPlayer[k].coinMax) {
                GwCommon.boardMaxCoin[board][charNo] = GwPlayer[k].coinMax;
            }
        }
    }
}

s16 PResResultOrderGet(s16 (*orderP)[4], s16 (*tieP)[4], s16 (*rankP)[4])
{
    s16 order[4];
    s32 score[4];
    s16 tie;
    s16 cnt;
    s16 tmp;
    s16 i, j;

    for (i = 0; i < 4; i++) {
        order[i] = i;
        score[i] = PResPlayer[i].star + PResPlayer[i].bonusStar;
        score[i] <<= 16;
        score[i] |= PResPlayer[i].coin;
    }
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3 - i; j++) {
            if (score[order[j]] < score[order[j + 1]]) {
                tmp = order[j];
                order[j] = order[j + 1];
                order[j + 1] = tmp;
            }
        }
    }
    for (i = 0; i < 4; i++) {
        (*orderP)[i] = order[i];
    }
    tie = 0;
    for (i = 0; i < 3; i++) {
        if (score[order[i]] != score[order[i + 1]]) {
            (*tieP)[order[i]] = tie;
            tie++;
        } else {
            (*tieP)[order[i]] = tie;
        }
    }
    (*tieP)[order[i]] = tie;
    tie = 0;
    for (i = 0; i < 4; i++) {
        (*rankP)[order[i]] = tie;
        tie++;
    }
    cnt = 0;
    for (i = 0; i < 4; i++) {
        if ((*tieP)[i] == 0) {
            cnt++;
        }
    }
    return cnt;
}

static void PResResultFinalTeam(void)
{
    HuVecF rot[4];
    HuVecF center;
    HuVecF pos;
    s16 teamOrd[2];
    s16 winCnt;
    s16 win;
    s32 se;
    s32 jingle;
    f32 speed;
    f32 t;
    s16 i, j, k;
    s16 p;
    s16 h1, h2, hMax;

    se = 0;
    center.x = PlayerPosTbl[0].x + (PlayerPosTbl[3].x - PlayerPosTbl[0].x) * 0.5f;
    center.y = PlayerPosTbl[0].y;
    center.z = PlayerPosTbl[0].z;
    for (i = 0; i < 4; i++) {
        rot[i].x = frandmod(10) - 5;
        rot[i].y = frandmod(10) - 5;
        rot[i].z = frandmod(10) - 5;
    }
    winCnt = PResResultTeamOrderGet(teamOrd);
    HuWinMesSet(MessWinId, 0x90011);
    PResComInputWait();
    HuWinMesWait(MessWinId);
    HuAudSStreamPauseFadeOut(0, 1, 500);
    HuWinMesSet(MessWinId, 0x90012);
    jingle = HuAudJinglePlay(0x46);
    speed = 1.0f;
    for (i = 0; i < 60; i++) {
        Hu3DMotionSpeedSet(StageMdlId, speed);
        speed += 0.1;
        if (i == 10) {
            se = HuAudFXPlay(0x3E9);
        }
        if (i == 20) {
            for (j = 0; j < 4; j++) {
                CharMotionShiftSet(PResCharNo[j], PResPlayerMotion[j][5], 0.0f, 8.0f, 0x40000001);
            }
        }
        HuPrcVSleep();
    }
    PResComInputWait();
    HuWinMesWait(MessWinId);
    TeamNameFadeF = 1;
    HuWinExClose(MessWinId);
    omCameraViewMoveMulti(4, &FinalResultView, 10, 1);
    if (winCnt > 1) {
        HuPrcSleep(60);
        HuWinExOpen(MessWinId);
        HuWinMesSet(MessWinId, 0x90018);
        HuAudFXPlay(0x2EF);
        HuAudSStreamFadeOut(jingle, 1000);
        HuAudSStreamPause(0, 0);
        TeamNameFadeF = 0;
        omCameraViewMoveMulti(4, &MainResultView, 30, 0);
        for (i = 0; i < 30; i++) {
            Hu3DMotionSpeedSet(StageMdlId, speed);
            speed -= 0.2;
            if (i == 20) {
                HuAudFXStop(se);
                for (j = 0; j < 4; j++) {
                    CharMotionShiftSet(PResCharNo[j], PResPlayerMotion[j][0], 0.0f, 8.0f, 0x40000001);
                }
            }
            HuPrcVSleep();
        }
        PResComInputWait();
        HuWinMesWait(MessWinId);
        HuWinMesSet(MessWinId, 0x90014);
        PResComInputWait();
        HuWinMesWait(MessWinId);
        teamOrd[0] = PResResultTie();
        PResPlayer[teamOrd[0] * 2].bonusStar = 1;
        PResPlayer[teamOrd[0] * 2 + 1].bonusStar = 1;
        HuWinMesSet(MessWinId, 0x90016);
        PResComInputWait();
        HuWinMesWait(MessWinId);
        HuAudSStreamAllFadeOut(500);
        TeamNameFadeF = 1;
        HuWinExClose(MessWinId);
        omCameraViewMoveMulti(4, &FinalResultView, 10, 1);
        HuAudJinglePlay(0x46);
        for (i = 0; i < 60; i++) {
            Hu3DMotionSpeedSet(StageMdlId, speed);
            speed += 0.1;
            if (i == 10) {
                se = HuAudFXPlay(0x3E9);
            }
            if (i == 20) {
                HuAudFXPlay(0x3EA);
                for (j = 0; j < 4; j++) {
                    CharMotionShiftSet(PResCharNo[j], PResPlayerMotion[j][5], 0.0f, 8.0f, 0x40000001);
                }
            }
            HuPrcVSleep();
        }
    }
    HuPrcSleep((s16)(frandmod(60) + 60));
    for (j = 0; j < 60; j++) {
        for (i = 0; i < 4; i++) {
            if (j >= i * 5 && j <= i * 5 + 30 && teamOrd[0] != GwPlayerConf[PResPlayerOrder[i]].grp) {
                if (j == i * 5) {
                    HuAudFXPlay(0x3EA);
                    CharMotionShiftSet(PResCharNo[i], PResPlayerMotion[i][6], 0.0f, 8.0f, 0x40000001);
                }
                t = j - i * 5;
                Hu3DModelPosSet(PResPlayerModelId[i], PlayerPosTbl[i].x, PlayerPosTbl[i].y, PlayerPosTbl[i].z - 150.0f * t);
                Hu3DModelRotSet(PResPlayerModelId[i], t * rot[i].x, t * rot[i].y, t * rot[i].z);
                ResultCharFXUpdate(i);
                t = 0.3 + 0.7 * (1.0 - t / 30.0);
                Hu3DModelScaleSet(PResPlayerModelId[i], t, t, t);
                Hu3DModelLayerSet(PResPlayerModelId[i], 0);
                if (j == i * 5 + 30) {
                    Hu3DModelAttrSet(PResPlayerModelId[i], 1);
                }
            }
        }
        if (j == 30) {
            HuAudFXStop(se);
        }
        HuPrcVSleep();
    }
    HuWinExOpen(MessWinId);
    HuWinInsertMesSet(MessWinId, PResCharNo[teamOrd[0] * 2], 0);
    HuWinInsertMesSet(MessWinId, PResCharNo[teamOrd[0] * 2 + 1], 1);
    HuWinInsertMesSet(MessWinId, PResTeamNameMesGet(PResCharNo[teamOrd[0] * 2], PResCharNo[teamOrd[0] * 2 + 1]), 2);
    HuWinMesSet(MessWinId, 0x9001A);
    for (j = 1; j <= 60; j++) {
        for (k = 0; k < 2; k++) {
            p = k + teamOrd[0] * 2;
            pos = center;
            pos.x += k * 140 - 70;
            PSVECSubtract(&pos, &PlayerPosTbl[p], &pos);
            PSVECScale(&pos, &pos, HuSin(90.0f * (f32)(j / 60.0)));
            PSVECAdd(&PlayerPosTbl[p], &pos, &pos);
            Hu3DModelPosSetV(PResPlayerModelId[p], &pos);
            ResultCharFXUpdate(p);
        }
        speed -= 0.1;
        Hu3DMotionSpeedSet(StageMdlId, speed);
        if (j == 50) {
            for (i = 0; i < 4; i++) {
                CharMotionShiftSet(PResCharNo[i], PResPlayerMotion[i][0], 0.0f, 8.0f, 0x40000001);
            }
        }
        HuPrcVSleep();
    }
    PResComInputWait();
    HuWinMesWait(MessWinId);
    HuAudSStreamAllFadeOut(500);
    HuAudBGMPlay(0x47);
    HuWinExClose(MessWinId);
    h1 = CharModelHeightGet(PResCharNo[teamOrd[0] * 2]);
    h2 = CharModelHeightGet(PResCharNo[teamOrd[0] * 2 + 1]);
    if (h1 > h2) {
        hMax = h1;
    } else {
        hMax = h2;
    }
    PResResultWinnerExec(teamOrd[0] * 2, teamOrd[0] * 2 + 1, hMax);
    PResBtnAWait();
}

s16 PResResultTeamOrderGet(s16 *order)
{
    s32 score[2];
    s16 i;

    for (i = 0; i < 2; i++) {
        score[i] = PResPlayer[i * 2].star + PResPlayer[i * 2 + 1].star;
        score[i] = PResPlayer[i * 2].bonusStar + (PResPlayer[i * 2 + 1].bonusStar + score[i]);
        score[i] <<= 16;
        score[i] |= PResPlayer[i * 2].coin + PResPlayer[i * 2 + 1].coin;
    }
    if (score[0] > score[1]) {
        order[0] = 0;
        order[1] = 1;
        return 1;
    }
    if (score[0] < score[1]) {
        order[0] = 1;
        order[1] = 0;
        return 1;
    }
    order[0] = 0;
    order[1] = 0;
    return 2;
}

static void BonusFadeCreate(s16 mdlId, s16 num, HuVecF *pos)
{
    HU3DMODEL *mdl;
    HU3DPARTICLE *part[4];
    s16 partMdl[4];
    HuVecF pos2D;
    s16 i, j;
    f32 t;

    mdl = &Hu3DData[mdlId];
    BonusFadeAddr = HuMemDirectMallocNum(HEAP_HEAP, GXGetTexBufferSize(192, 192, GX_TF_RGB565, GX_FALSE, 0), HU_MEMNUM_OVL);
    Hu3D3Dto2D(&mdl->pos, 2, &pos2D);
    pos2D.x = pos2D.x * 1.1111111111111112;
    BonusFadeSize[0] = pos2D.x - 96.0f;
    BonusFadeSize[1] = pos2D.y - 96.0f;
    BonusFadeSize[2] = 192;
    BonusFadeSize[3] = 192;
    Hu3DLayerHookSet(9, BonusFadeFbCopy);
    HuPrcVSleep();
    Hu3DLayerHookReset(9);
    for (i = 0; i < num; i++) {
        partMdl[i] = Hu3DParticleCreate(CircleAnimP, 1100);
        Hu3DModelLayerSet(partMdl[i], 2);
        Hu3DModelCameraSet(partMdl[i], 4);
        Hu3DParticleHookSet(partMdl[i], BonusFadeEffHook);
        part[i] = Hu3DData[partMdl[i]].hookData;
        part[i]->pos.x = mdl->pos.x;
        part[i]->pos.y = mdl->pos.y;
        part[i]->pos.z = mdl->pos.z;
        part[i]->unk_10 = pos[i];
    }
    BonusFadeMode = 0;
    HuPrcVSleep();
    BonusFadeMode = 1;
    HuPrcVSleep();
    BonusFadeMode = 2;
    HuAudFXPlay(0x3E7);
    for (i = 0; i < 15; i++) {
        Hu3DModelTPLvlSet(mdlId, 1.0 - i / 15.0);
        if (i == 10) {
            Hu3DParManAttrSet(StarEffAnimP[0], 1);
        }
        HuPrcVSleep();
    }
    Hu3DModelAttrSet(mdlId, 1);
    BonusFadeMode = 0;
    HuPrcSleep(30);
    BonusFadeMode = 3;
    for (i = 0; i < num; i++) {
        part[i]->dataCnt = 0;
    }
    HuPrcSleep(40);
    BonusFadeMode = 4;
    HuAudFXPlay(0x3E8);
    for (i = 0; i < num; i++) {
        Hu3DModelAttrReset(PlayerStarMdlId[i], 1);
        Hu3DModelPosSetV(PlayerStarMdlId[i], &pos[i]);
        Hu3DModelRotSet(PlayerStarMdlId[i], 0.0f, 0.0f, 0.0f);
        Hu3DModelScaleSet(PlayerStarMdlId[i], 1.2f, 1.2f, 1.2f);
        part[i]->dataCnt = 0;
    }
    for (j = 1; j <= 30; j++) {
        t = j / 30.0;
        for (i = 0; i < num; i++) {
            Hu3DModelScaleSet(PlayerStarMdlId[i], 1.2 * t, 1.2 * t, 1.2 * t);
            if (j == 15) {
                Hu3DParManAttrReset(StarEffAnimP[i], 1);
                Hu3DParManColorSet(StarEffAnimP[i], 0);
                Hu3DParManPosSet(StarEffAnimP[i], pos[i].x, pos[i].y, pos[i].z - 20.0f);
            }
        }
        HuPrcVSleep();
    }
    for (i = 0; i < num; i++) {
        Hu3DModelKill(partMdl[i]);
    }
    HuMemDirectFree(BonusFadeAddr);
}

static void BonusFadeFbCopy(s16 layerNo)
{
    if (Hu3DCameraNo != 1) {
        return;
    }

    Hu3DFbCopyExec(BonusFadeSize[0], BonusFadeSize[1], BonusFadeSize[2], BonusFadeSize[3], 0x4, 0, BonusFadeAddr);
}

static void BonusFadeEffHook(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx)
{
    HuVecF tmp;
    HU3DPARTICLEDATA *d;
    s16 i;
    s16 n;
    s32 x, y, xx, yy;
    s32 sumR, sumG, sumB, cnt;
    s32 sign;
    u8 *px;
    u16 pix;
    u8 r, g, b;
    s32 v;

    if (particleP->count == 0) {
        d = particleP->data;
        for (i = 0; i < particleP->maxCnt; i++, d++) {
            d->scale = 0.0f;
        }
    }
    switch (BonusFadeMode) {
        case 1:
            d = particleP->data;
            for (y = 0; y < 192; y += 6) {
                for (x = 0; x < 192; x += 6) {
                    if (y >= 48 && y <= 144 && x >= 48 && x <= 144) {
                        sumB = 0;
                        sumG = 0;
                        sumR = 0;
                        cnt = 0;
                        for (yy = y; yy < y + 6; yy++) {
                            for (xx = x; xx < x + 6; xx++) {
                                px = (u8 *)BonusFadeAddr + ((xx >> 2) << 5) + ((xx & 3) * 2) + ((yy >> 2) * 0x600) + ((yy & 3) * 4 * 2);
                                pix = (px[0] << 8) | px[1];
                                r = (pix >> 8) & 0xF8;
                                g = (pix >> 3) & 0xFC;
                                b = (pix * 8) & 0xF8;
                                if (!((f32)(0.114 * b + (0.299 * r + 0.587 * g)) < 65.0f)) {
                                    sumR += r;
                                    sumG += g;
                                    sumB += b;
                                    cnt++;
                                }
                            }
                        }
                        if (cnt != 0) {
                            d->color.r = sumR / cnt;
                            d->color.g = sumG / cnt;
                            d->color.b = sumB / cnt;
                            d->pos.x = particleP->pos.x;
                            d->pos.y = particleP->pos.y;
                            d->pos.z = particleP->pos.z;
                            d->pos.x = d->pos.x + 1.3 * (x - 96);
                            d->pos.y = d->pos.y + 1.3 * (-y + 96);
                            d->pos.z = d->pos.z + 30.0f;
                            d->accel.x = d->pos.x;
                            d->accel.y = d->pos.y;
                            d->accel.z = d->pos.z;
                            d->vel.x = d->accel.x;
                            d->vel.y = d->accel.y;
                            d->vel.z = d->accel.z;
                            d->scale = 0.1f;
                            d->speedDecay = frandmod(10) - 5;
                            if (d->speedDecay < 0.0f) {
                                sign = -5;
                            } else {
                                sign = 5;
                            }
                            d->speedDecay = d->speedDecay + sign;
                            d->colorIdx = frandmod(10) - 5;
                            if (d->colorIdx < 0.0f) {
                                sign = -5;
                            } else {
                                sign = 5;
                            }
                            d->colorIdx = d->colorIdx + sign;
                            d->scaleBase = frandmod(10);
                            d->time = 0;
                            d++;
                        }
                    }
                }
            }
            DCFlushRangeNoSync(particleP->data, particleP->maxCnt * sizeof(HU3DPARTICLEDATA));
            particleP->dataCnt = 0;
            break;
        case 2:
            d = particleP->data;
            for (i = 0; i < particleP->maxCnt; i++, d++) {
                if (d->scale > 0.0f && d->scale < 15.0f) {
                    d->scale += 1.0f;
                }
            }
            particleP->dataCnt++;
            break;
        case 3:
            if (particleP->dataCnt <= 40) {
                d = particleP->data;
                for (i = 0; i < particleP->maxCnt; i++, d++) {
                    if (d->scale > 0.0f && !((f32)particleP->dataCnt < d->scaleBase) && !((f32)particleP->dataCnt > 30.0f + d->scaleBase)) {
                        n = (f32)particleP->dataCnt - d->scaleBase;
                        PSVECSubtract(&particleP->unk_10, &particleP->pos, &tmp);
                        PSVECScale(&tmp, &tmp, HuSin(90.0 * (n / 30.0)));
                        PSVECAdd(&d->vel, &tmp, &d->accel);
                        v = d->color.r + 5;
                        if (v > 0xFF) {
                            d->color.r = 0xFF;
                        } else {
                            d->color.r = v;
                        }
                        v = d->color.g + 5;
                        if (v > 0xFF) {
                            d->color.g = 0xFF;
                        } else {
                            d->color.g = v;
                        }
                        v = d->color.b + 5;
                        if (v > 0xFF) {
                            d->color.b = 0xFF;
                        } else {
                            d->color.b = v;
                        }
                    }
                }
                DCFlushRangeNoSync(particleP->data, particleP->maxCnt * sizeof(HU3DPARTICLEDATA));
                particleP->dataCnt++;
            }
            break;
        case 4:
            if (particleP->dataCnt <= 30) {
                d = particleP->data;
                for (i = 0; i < particleP->maxCnt; i++, d++) {
                    if (d->scale > 0.0f) {
                        d->color.a = (s32)(255.0 * (1.0 - HuSin(90.0 * (particleP->dataCnt / 30.0))));
                        d->scale = d->scale * 1.05;
                    }
                }
                DCFlushRangeNoSync(particleP->data, particleP->maxCnt * sizeof(HU3DPARTICLEDATA));
                particleP->dataCnt++;
            }
            break;
    }
    d = particleP->data;
    for (i = 0; i < particleP->maxCnt; i++, d++) {
        if (d->scale > 0.0f) {
            d->pos.x = d->accel.x + 3.0 * HuSin((f32)d->time * d->speedDecay);
            d->pos.y = d->accel.y + 3.0 * HuCos((f32)d->time * d->colorIdx);
            d->pos.z = d->accel.z;
            d->time++;
        }
    }
}

static void BGFilter(s16 layerNo)
{
    Mtx44 proj;
    Mtx mtx;
    GXColor color = { 0, 0, 0, 0 };

    if (Hu3DCameraNo != 2) {
        return;
    }
    MTXOrtho(proj, 0.0f, 480.0f, 0.0f, 640.0f, 0.0f, 10.0f);
    GXSetProjection(proj, GX_ORTHOGRAPHIC);
    MTXIdentity(mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
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
    color.a = 160.0 * (BGFadeTime / 30.0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_S16, 0);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevColor(GX_TEVREG0, color);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition2s16(0, 0);
    GXPosition2s16(640, 0);
    GXPosition2s16(640, 480);
    GXPosition2s16(0, 480);
    GXEnd();
    BGFadeTime++;
    if (BGFadeTime > 30) {
        BGFadeTime = 30;
    }
}

static void PlayerHover(void)
{
    HU3DMODEL *mdl;
    s16 i;

    while (1) {
        for (i = 0; i < 4; i++) {
            mdl = &Hu3DData[PResPlayerModelId[i]];
            if (PResPlayerMotion[i][5] == Hu3DMotionIDGet(PResPlayerModelId[i])) {
                mdl->pos.y = (PlayerPosTbl[i].y + (f32)(((GlobalCounter + i) & 2) * 2)) - 2.0f;
            }
        }
        HuPrcVSleep();
    }
}

static void ResultCharFXUpdate(s16 playerNo)
{
    HU3DMODEL *mdl;
    HuVecF pos3D;
    HuVecF pos2D;
    f32 pan;

    mdl = &Hu3DData[PResPlayerModelId[playerNo]];
    pos3D.x = mdl->pos.x;
    pos3D.y = mdl->pos.y;
    pos3D.z = mdl->pos.z;
    Hu3D3Dto2D(&pos3D, 4, &pos2D);
    pan = 64.0 + 32.0 * ((pos2D.x - 288.0f) / 288.0f);
    if (pan < 32.0f) {
        pan = 32.0f;
    }
    if (pan > 96.0f) {
        pan = 96.0f;
    }
    CharFXPan[PResCharNo[playerNo]] = pan;
    CharFXVol[PResCharNo[playerNo]] = 0x7F;
}

static void ResultCharFXPlay(s16 charNo, s16 seNo)
{
    CharFXPlayVolPan(charNo, seNo, CharFXVol[charNo], CharFXPan[charNo]);
}
