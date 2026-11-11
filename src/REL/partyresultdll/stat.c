#include "REL/partyresultdll.h"
#include "game/hsfex.h"
#include "game/main.h"
#include "stdio.h"
#include "math.h"

typedef struct {
    f32 x, y;
} StatLinePt;

#define STAT_DATA(n) (0xB10000 + (n))

extern void HuSprAnimNoSetPause(HUSPRGRPID grpId, s16 memberNo, s16 animNo);
extern s16 PResResultOrderGet(s16 (*)[4], s16 (*)[4], s16 (*)[4]);
extern s16 PResResultTeamOrderGet(s16 *);

static void StatBackDraw(s16 layerNo);
static void StatModelUpdate(void);
static void StatScorePosSet(s16 n, f32 x, f32 y);
static void StatDetailPosSet(f32 x, f32 y);
static void StatFingerPosSet(volatile s16 n);
static s32 StatLinePosGet(StatLinePt *p, StatLinePt *dir, StatLinePt *b, StatLinePt *c, StatLinePt *out);
static void StatLineNorm(StatLinePt *src, StatLinePt *dst);
static void StatLineScale(StatLinePt *src, StatLinePt *dst, f32 scale);
static void StatLineTrans(StatLinePt *a, StatLinePt *b, StatLinePt *out);
static void StatGraphDraw(HUSPRITE *spr);
extern void OSReport(const char *msg, ...);

static OMCAMERAVIEW StatView = { { 0.0f, 110.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, 6500.0f };

static f64 BackScrollX;
static f64 BackScrollY;
static HUSPRITE *GraphSprP[4];
static HUPROCESS *StatModelProc;
static s16 PlayerSel;
static s16 StatGraphMax[3];
static ANIMDATA *GraphAnimP;
static s16 FingerMdlId;
static f32 DetailGrpIdxOfsX;
static f32 DetailGrpOfsX;
static f32 DetailSelIdx;
static f32 DetailCursorPosX;
static s16 DetailGrpId;
static s16 DetailOverGrpId;
static s16 TeamNameWinId[2];
static ANIMDATA *BackAnimP;
static s16 MessWinId;
static s16 HelpWinId;
static s16 ScoreGrpId[4];
static s16 HeaderGrpId;
static s16 StageMdlId;
static s16 StarMdlId[4];
static s16 CoinMdlId[4];
static s16 ToueiMdlId[4];

void PResStatDetailCreate(void);
void PResStatRankCreate(void);
void PResStatRankTeamCreate(void);

void PResStatCreate(void)
{
    s16 i;
    s16 boardNo;
    s16 turnIdx;
    s16 grpId;
    s16 mdlId;
    s16 charNo;

    boardNo = GwSystem.boardNo;
    if (boardNo < 0 || boardNo >= 7) {
        boardNo = 0;
    }
    Hu3DCameraPerspectiveSet(5, 5.0f, 20.0f, 15000.0f, 1.2f);
    omCameraViewSetMulti(5, &StatView);
    HuPrcVSleep();
    for (i = 0; i < 4; i++) {
        charNo = PResCharNo[i];
        mdlId = PResPlayerModelId[i];
        Hu3DModelPosSet(mdlId, i * 150 - 225, 0.0f, 0.0f);
        Hu3DModelRotSet(mdlId, 0.0f, 0.0f, 0.0f);
        CharMotionSet(charNo, PResPlayerMotion[i][0]);
        Hu3DModelAttrSet(mdlId, 0x40000001);
        Hu3DModelAttrReset(mdlId, 1);
        Hu3DModelCameraSet(mdlId, 1);
        if (charNo == 8) {
            Hu3DModelScaleSet(mdlId, 0.8f, 0.8f, 0.8f);
        } else if (charNo == 9) {
            Hu3DModelScaleSet(mdlId, 0.8f, 0.8f, 0.8f);
        } else {
            Hu3DModelScaleSet(mdlId, 1.0f, 1.0f, 1.0f);
        }
        Hu3DModelLayerSet(mdlId, 0);
    }
    mdlId = Hu3DModelCreate(HuDataSelHeapReadNum(STAT_DATA(0x31), HU_MEMNUM_OVL, HEAP_MODEL));
    StageMdlId = mdlId;
    Hu3DModelAttrSet(mdlId, 0x40000001);
    Hu3DModelCameraSet(mdlId, 1);
    grpId = HuSprGrpCreate(4);
    HeaderGrpId = grpId;
    HuSprGrpMemberSet(grpId, 0, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(STAT_DATA(0x2B), HU_MEMNUM_OVL, HEAP_MODEL)), 0x5F, 0));
    HuSprPosSet(grpId, 0, 288.0f, 65.0f);
    HuSprGrpMemberSet(grpId, 1, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(STAT_DATA(0x21) + boardNo, HU_MEMNUM_OVL, HEAP_MODEL)), 0x64, 0));
    HuSprPosSet(grpId, 1, 90.0f, 65.0f);
    HuSprGrpMemberSet(grpId, 2, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(STAT_DATA(0x2A), HU_MEMNUM_OVL, HEAP_MODEL)), 0x64, 0));
    HuSprPosSet(grpId, 2, 490.0f, 65.0f);
    turnIdx = (GwSystem.turnMax - 10) / 5;
    if (turnIdx < 0 || turnIdx > 8) {
        turnIdx = 0;
    }
    HuSprGrpMemberSet(grpId, 3, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(STAT_DATA(0x2C), HU_MEMNUM_OVL, HEAP_MODEL)), 0x5F, turnIdx));
    HuSprPosSet(grpId, 3, 465.0f, 65.0f);
    if (PResTeamF == 0) {
        PResStatRankCreate();
    } else {
        PResStatRankTeamCreate();
    }
    PResStatDetailCreate();
    BackAnimP = HuSprAnimRead(HuDataSelHeapReadNum(STAT_DATA(0x1A) + boardNo, HU_MEMNUM_OVL, HEAP_MODEL));
    HelpWinId = HuWinCreate(-10000.0f, 1000.0f, 0x230, 0x2A, 0);
    HuWinBGTPLvlSet(HelpWinId, 0.0f);
    HuWinAttrSet(HelpWinId, 0x800);
    HuWinMesSpeedSet(HelpWinId, 0);
    HuWinMesSet(HelpWinId, 0x2000A);
    MessWinId = HuWinExCreateFrame(16.0f, 376.0f, 0x220, 0x40, -1, 0);
    HuWinMesSpeedSet(MessWinId, 0);
    HuSprExecLayerCameraSet(0x40, 4, 0);
    Hu3DLayerHookSet(1, StatBackDraw);
    HuPrcVSleep();
    ToueiDispInit(ToueiMdlId);
    StatModelProc = HuPrcChildCreate(StatModelUpdate, 0x3E8, 0x2000, 0, objman);
    Hu3DGLightPosSet(PResLightId, 1000.0f, 1000.0f, 1000.0f, -1.0f, -1.0f, -1.0f);
}

void PResStatRankCreate(void)
{
    HuVecF out;
    HuVecF in;
    char str[8];
    s16 rank[4];
    s16 idx[4];
    s16 pos[4];
    ANIMDATA *numAnim;
    s16 i, j;
    s16 mdlId;
    s16 grpId;

    PResResultOrderGet(&pos, &idx, &rank);
    for (i = 0; i < 4; i++) {
        mdlId = Hu3DModelCreate(HuDataSelHeapReadNum(STAT_DATA(0x30), HU_MEMNUM_OVL, HEAP_MODEL));
        ToueiMdlId[i] = mdlId;
        Hu3DModelCameraSet(mdlId, 4);
        Hu3DModelScaleSet(mdlId, 0.4f, 0.4f, 0.2f);
        in.x = 140.0f;
        in.y = rank[i] * 75 + 150;
        in.z = 6500.0f;
        Hu3D2Dto3D(&in, 4, &out);
        Hu3DModelPosSetV(mdlId, &out);
    }
    for (i = 0; i < 4; i++) {
        mdlId = Hu3DModelCreate(HuDataSelHeapReadNum(STAT_DATA(4), HU_MEMNUM_OVL, HEAP_MODEL));
        StarMdlId[i] = mdlId;
        Hu3DModelCameraSet(mdlId, 4);
        Hu3DModelScaleSet(mdlId, 0.6f, 0.6f, 0.6f);
        in.x = 210.0f;
        in.y = rank[i] * 75 + 150;
        in.z = 6500.0f;
        Hu3D2Dto3D(&in, 4, &out);
        Hu3DModelPosSetV(mdlId, &out);
        mdlId = Hu3DModelCreate(HuDataSelHeapReadNum(STAT_DATA(6), HU_MEMNUM_OVL, HEAP_MODEL));
        CoinMdlId[i] = mdlId;
        Hu3DModelCameraSet(mdlId, 4);
        Hu3DModelScaleSet(mdlId, 0.45f, 0.45f, 0.45f);
        in.x = 380.0f;
        in.y = rank[i] * 75 + 150;
        in.z = 6500.0f;
        Hu3D2Dto3D(&in, 4, &out);
        Hu3DModelPosSetV(mdlId, &out);
    }
    for (i = 0; i < 4; i++) {
        grpId = HuSprGrpCreate(13);
        ScoreGrpId[i] = grpId;
        HuSprGrpMemberSet(grpId, 0, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(STAT_DATA(0x32) + idx[i], HU_MEMNUM_OVL, HEAP_MODEL)), 0x64, 0));
        HuSprPosSet(grpId, 0, 288.0f, rank[i] * 75 + 150);
        HuSprDrawNoSet(grpId, 0, 0x40);
        HuSprGrpMemberSet(grpId, 1, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(STAT_DATA(0x2E), HU_MEMNUM_OVL, HEAP_MODEL)), 0x5F, 0));
        HuSprPosSet(grpId, 1, 70.0f, rank[i] * 75 + 150);
        HuSprAnimNoSetPause(grpId, 1, idx[i]);
        if (rank[i] == 0) {
            HuSprGrpMemberSet(grpId, 2, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(STAT_DATA(0x2D), HU_MEMNUM_OVL, HEAP_MODEL)), 0x5A, 0));
            HuSprPosSet(grpId, 2, 140.0f, rank[i] * 75 + 105);
        }
        sprintf(str, "%0d   ", PResPlayer[i].star);
        numAnim = HuSprAnimRead(HuDataSelHeapReadNum(STAT_DATA(0x2F), HU_MEMNUM_OVL, HEAP_MODEL));
        for (j = 0; j < 4; j++) {
            if (j != 0 && str[j - 1] == ' ') {
                break;
            }
            HuSprGrpMemberSet(grpId, j + 3, HuSprCreate(numAnim, 0x5F, 0));
            HuSprPosSet(grpId, j + 3, j * 24 + 250, rank[i] * 75 + 150);
            if (j == 0) {
                HuSprBankSet(grpId, 3, 10);
            } else {
                HuSprBankSet(grpId, j + 3, str[j - 1] & 0xF);
            }
        }
        sprintf(str, "%0d    ", PResPlayer[i].coin);
        for (j = 0; j < 5; j++) {
            if (j != 0 && str[j - 1] == ' ') {
                break;
            }
            HuSprGrpMemberSet(grpId, j + 8, HuSprCreate(numAnim, 0x5F, 0));
            HuSprPosSet(grpId, j + 8, j * 24 + 420, rank[i] * 75 + 150);
            if (j == 0) {
                HuSprBankSet(grpId, 8, 10);
            } else {
                HuSprBankSet(grpId, j + 8, str[j - 1] & 0xF);
            }
        }
    }
}

void PResStatRankTeamCreate(void)
{
    HuVecF out;
    HuVecF in;
    char str[8];
    s16 order[4];
    s16 teamOrd[2];
    ANIMDATA *numAnim;
    s16 i, j;
    s16 mdlId;
    s16 grpId;
    s16 team;
    s32 ofs;

    PResResultTeamOrderGet(teamOrd);
    if (teamOrd[0] == 0) {
        for (i = 0; i < 4; i++) {
            order[i] = i;
        }
    } else {
        for (i = 0; i < 2; i++) {
            order[i] = i + 2;
        }
        for (i = 0; i < 2; i++) {
            order[i + 2] = i;
        }
    }
    for (i = 0; i < 4; i++) {
        mdlId = Hu3DModelCreate(HuDataSelHeapReadNum(STAT_DATA(0x30), HU_MEMNUM_OVL, HEAP_MODEL));
        ToueiMdlId[i] = mdlId;
        Hu3DModelCameraSet(mdlId, 4);
        Hu3DModelScaleSet(mdlId, 0.4f, 0.4f, 0.2f);
        in.x = 140.0f;
        if (order[i] & 1) {
            ofs = 36;
        } else {
            ofs = -36;
        }
        in.y = (order[i] / 2) * 150 + ofs + 180;
        in.z = 6500.0f;
        Hu3D2Dto3D(&in, 4, &out);
        Hu3DModelPosSetV(mdlId, &out);
    }
    for (i = 0; i < 2; i++) {
        mdlId = Hu3DModelCreate(HuDataSelHeapReadNum(STAT_DATA(4), HU_MEMNUM_OVL, HEAP_MODEL));
        StarMdlId[i] = mdlId;
        Hu3DModelCameraSet(mdlId, 4);
        Hu3DModelScaleSet(mdlId, 0.6f, 0.6f, 0.6f);
        in.x = 210.0f;
        in.y = i * 150 + 210;
        in.z = 6500.0f;
        Hu3D2Dto3D(&in, 4, &out);
        Hu3DModelPosSetV(mdlId, &out);
        mdlId = Hu3DModelCreate(HuDataSelHeapReadNum(STAT_DATA(6), HU_MEMNUM_OVL, HEAP_MODEL));
        CoinMdlId[i] = mdlId;
        Hu3DModelCameraSet(mdlId, 4);
        Hu3DModelScaleSet(mdlId, 0.45f, 0.45f, 0.45f);
        in.x = 380.0f;
        in.y = i * 150 + 210;
        in.z = 6500.0f;
        Hu3D2Dto3D(&in, 4, &out);
        Hu3DModelPosSetV(mdlId, &out);
    }
    for (i = 0; i < 2; i++) {
        team = teamOrd[i];
        grpId = HuSprGrpCreate(13);
        ScoreGrpId[i] = grpId;
        HuSprGrpMemberSet(grpId, 0, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(STAT_DATA(0x36) + team, HU_MEMNUM_OVL, HEAP_MODEL)), 0x64, 0));
        HuSprPosSet(grpId, 0, 288.0f, i * 150 + 180);
        HuSprDrawNoSet(grpId, 0, 0x40);
        HuSprGrpMemberSet(grpId, 1, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(STAT_DATA(0x2E), HU_MEMNUM_OVL, HEAP_MODEL)), 0x5F, 0));
        HuSprPosSet(grpId, 1, 70.0f, i * 150 + 180);
        HuSprAnimNoSetPause(grpId, 1, i);
        if (i == 0) {
            HuSprGrpMemberSet(grpId, 2, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(STAT_DATA(0x2D), HU_MEMNUM_OVL, HEAP_MODEL)), 0x5A, 0));
            HuSprPosSet(grpId, 2, 70.0f, 135.0f);
        }
        sprintf(str, "%0d    ", PResPlayer[team * 2].star + PResPlayer[team * 2 + 1].star);
        numAnim = HuSprAnimRead(HuDataSelHeapReadNum(STAT_DATA(0x2F), HU_MEMNUM_OVL, HEAP_MODEL));
        for (j = 0; j < 5; j++) {
            if (j != 0 && str[j - 1] == ' ') {
                break;
            }
            HuSprGrpMemberSet(grpId, j + 3, HuSprCreate(numAnim, 0x5F, 0));
            HuSprPosSet(grpId, j + 3, j * 24 + 250, i * 150 + 210);
            if (j == 0) {
                HuSprBankSet(grpId, 3, 10);
            } else {
                HuSprBankSet(grpId, j + 3, str[j - 1] & 0xF);
            }
        }
        sprintf(str, "%0d    ", PResPlayer[team * 2].coin + PResPlayer[team * 2 + 1].coin);
        for (j = 0; j < 5; j++) {
            if (j != 0 && str[j - 1] == ' ') {
                break;
            }
            HuSprGrpMemberSet(grpId, j + 8, HuSprCreate(numAnim, 0x5F, 0));
            HuSprPosSet(grpId, j + 8, j * 24 + 420, i * 150 + 210);
            if (j == 0) {
                HuSprBankSet(grpId, 8, 10);
            } else {
                HuSprBankSet(grpId, j + 8, str[j - 1] & 0xF);
            }
        }
    }
    for (i = 0; i < 2; i++) {
        team = teamOrd[i];
        TeamNameWinId[i] = HuWinCreate(360.0f, i * 150 + 190, 0xEC, 0x2A, 0);
        HuWinBGTPLvlSet(TeamNameWinId[i], 0.0f);
        HuWinPriSet(TeamNameWinId[i], 500);
        HuWinAttrSet(TeamNameWinId[i], 0x800);
        HuWinMesSpeedSet(TeamNameWinId[i], 0);
        HuWinSprSet(TeamNameWinId[i], HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(STAT_DATA(0x28) + team, HU_MEMNUM_OVL, HEAP_MODEL)), 0x258, 0), 118.0f, 4.0f);
        HuWinMesSet(TeamNameWinId[i], PResTeamNameMesGet(PResCharNo[team * 2], PResCharNo[team * 2 + 1]));
        HuWinCenterPosSet(TeamNameWinId[i], 123.0f, 40.0f);
    }
}

void PResStatDetailCreate(void)
{
    char str[8];
    s16 i, j, k;
    s16 grpId;
    s16 sprId;
    s16 len;
    s16 step;
    s16 member;
    s16 cnt;
    s16 val;
    s16 teamNum;
    ANIMDATA *anim;
    ANIMDATA *numAnim;
    HUSPRITE *spr;
    s16 *p;

    for (i = 0; i < 4; i++) {
        PResPlayer[i].statStar = PResPlayer[i].star;
        PResPlayer[i].statCoin = PResPlayer[i].coin;
    }
    grpId = HuSprGrpCreate(21);
    DetailOverGrpId = grpId;
    HuSprGrpMemberSet(grpId, 0, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(STAT_DATA(0x38), HU_MEMNUM_OVL, HEAP_MODEL)), 0x96, 0));
    HuSprPosSet(grpId, 0, 288.0f, 220.0f);
    HuSprGrpMemberSet(grpId, 1, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(STAT_DATA(0x39), HU_MEMNUM_OVL, HEAP_MODEL)), 0x5A, 0));
    HuSprPosSet(grpId, 1, 318.0f, 220.0f);
    HuSprGrpMemberSet(grpId, 2, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(STAT_DATA(0x3C), HU_MEMNUM_OVL, HEAP_MODEL)), 0x50, 0));
    HuSprPosSet(grpId, 2, 132.0f, 220.0f);
    HuSprColorSet(grpId, 2, 0xFF, 0xF7, 0x7F);
    if (PResTeamF != 0) {
        HuSprGrpMemberSet(grpId, 20, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(STAT_DATA(0x42), HU_MEMNUM_OVL, HEAP_MODEL)), 0x64, 0));
        HuSprPosSet(grpId, 20, 58.0f, 224.0f);
    }
    HuSprGrpMemberSet(grpId, 3, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(STAT_DATA(0x3E), HU_MEMNUM_OVL, HEAP_MODEL)), 0x6E, 0));
    HuSprPosSet(grpId, 3, 318.0f, 220.0f);
    HuSprGrpMemberSet(grpId, 4, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(STAT_DATA(0x3D), HU_MEMNUM_OVL, HEAP_MODEL)), 0x6E, 0));
    HuSprPosSet(grpId, 4, 318.0f, 220.0f);
    if (PResTeamF != 0) {
        teamNum = 2;
    } else {
        teamNum = 4;
    }
    for (i = 0; i < teamNum; i++) {
        sprId = HuSprFuncCreate(StatGraphDraw, i + 0x69);
        HuSprGrpMemberSet(grpId, i + 5, sprId);
        HuSprPosSet(grpId, i + 5, 318.0f, 220.0f);
        spr = &HuSprData[sprId];
        GraphSprP[i] = spr;
        spr->work[0] = i;
        spr->work[1] = 0;
    }
    anim = HuSprAnimRead(HuDataSelHeapReadNum(STAT_DATA(0x41), HU_MEMNUM_OVL, HEAP_MODEL));
    if (PResPlayer[0].turnMax <= 20) {
        step = 5;
    } else {
        step = 10;
    }
    len = PResPlayer[0].turnMax + 1;
    for (i = 0; i < len / step + 1; i++) {
        HuSprGrpMemberSet(grpId, i + 9, HuSprCreate(anim, 0x6B, 0));
        HuSprPosSet(grpId, i + 9, 5.0 + (99.0 + (438.0 * ((f64)(f32)(i * step) / ((f64)(f32)len - 1.0)))), 220.0f);
    }
    StatGraphMax[1] = 0;
    StatGraphMax[0] = 0;
    for (i = 0; i < 4; i++) {
        len = PResPlayer[i].turnMax + 2;
        PResPlayer[i].starStat[len - 1] = PResPlayer[i].star;
        PResPlayer[i].coinStat[len - 1] = PResPlayer[i].coin;
    }
    if (PResTeamF != 0) {
        for (j = 0; j < len; j++) {
            for (i = 0; i < 4; i += 2) {
                PResPlayer[i].starStat[j] += PResPlayer[i + 1].starStat[j];
                PResPlayer[i].coinStat[j] += PResPlayer[i + 1].coinStat[j];
            }
        }
    }
    for (i = 0; i < 4; i++) {
        p = PResPlayer[i].starStat;
        for (j = 0; j < len; j++, p++) {
            if (StatGraphMax[0] < *p) {
                StatGraphMax[0] = *p;
            }
        }
        p = PResPlayer[i].coinStat;
        for (j = 0; j < len; j++, p++) {
            if (StatGraphMax[1] < *p) {
                StatGraphMax[1] = *p;
            }
        }
    }
    OSReport("Max %d:%d\n", StatGraphMax[0], StatGraphMax[1]);
    GraphAnimP = HuSprAnimRead(HuDataSelHeapReadNum(STAT_DATA(0x3F), HU_MEMNUM_OVL, HEAP_MODEL));
    HuSprGrpDrawNoSet(grpId, 0x40);
    DetailGrpIdxOfsX = 0.0f;
    DetailGrpOfsX = 0.0f;
    DetailSelIdx = 0.0f;
    DetailCursorPosX = 0.0f;
    grpId = HuSprGrpCreate(161);
    DetailGrpId = grpId;
    if (PResTeamF == 0) {
        anim = HuSprAnimRead(HuDataSelHeapReadNum(STAT_DATA(0x3A), HU_MEMNUM_OVL, HEAP_MODEL));
    } else {
        anim = HuSprAnimRead(HuDataSelHeapReadNum(STAT_DATA(0x3B), HU_MEMNUM_OVL, HEAP_MODEL));
    }
    HuSprGrpMemberSet(grpId, 0, HuSprCreate(anim, 0x64, 0));
    HuSprPosSet(grpId, 0, 318.0f, 220.0f);
    numAnim = HuSprAnimRead(HuDataSelHeapReadNum(STAT_DATA(0x2F), HU_MEMNUM_OVL, HEAP_MODEL));
    member = 1;
    for (k = 0; k < 10; k++) {
        if (PResTeamF == 0) {
            for (i = 0; i < 4; i++) {
                val = (&PResPlayer[i].statStar)[k];
                if (val < 0) {
                    val = 0;
                }
                sprintf(str, "%03d   ", val);
                cnt = 0;
                for (j = 0; j < 3; j++) {
                    if (str[j] == ' ') {
                        break;
                    }
                    if (str[j] != '0' || cnt != 0 || j == 2) {
                        cnt++;
                        HuSprGrpMemberSet(grpId, member, HuSprCreate(numAnim, 0x5F - j, 0));
                        HuSprPosSet(grpId, member, -57.0f + (k * 74 + (j * 20 + 22)), i * 53 + 156);
                        HuSprScaleSet(grpId, member, 0.95f, 1.0f);
                        HuSprBankSet(grpId, member, str[j] & 0xF);
                        HuSprColorSet(grpId, member, 0xFA, 0xDC, 0xA3);
                        HuSprAttrSet(grpId, member, 8);
                        member++;
                    }
                }
            }
        } else {
            for (i = 0; i < 2; i++) {
                val = (&PResPlayer[i * 2].statStar)[k] + (&PResPlayer[i * 2 + 1].statStar)[k];
                if (val < 0) {
                    val = 0;
                }
                if (val < 1000) {
                    sprintf(str, "%03d   ", val);
                    cnt = 0;
                    for (j = 0; j < 3; j++) {
                        if (str[j] == ' ') {
                            break;
                        }
                        if (str[j] != '0' || cnt != 0 || j == 2) {
                            cnt++;
                            HuSprGrpMemberSet(grpId, member, HuSprCreate(numAnim, 0x5F - j, 0));
                            HuSprPosSet(grpId, member, -57.0f + (k * 74 + (j * 20 + 22)), i * 106 + 182);
                            HuSprScaleSet(grpId, member, 0.95f, 1.0f);
                            HuSprBankSet(grpId, member, str[j] & 0xF);
                            HuSprColorSet(grpId, member, 0xFA, 0xDC, 0xA3);
                            HuSprAttrSet(grpId, member, 8);
                            member++;
                        }
                    }
                } else {
                    sprintf(str, "%04d    ", val);
                    cnt = 0;
                    for (j = 0; j < 4; j++) {
                        if (str[j] == ' ') {
                            break;
                        }
                        if (str[j] != '0' || cnt != 0 || j == 2) {
                            cnt++;
                            HuSprGrpMemberSet(grpId, member, HuSprCreate(numAnim, 0x5F - j, 0));
                            HuSprPosSet(grpId, member, -57.0f + (k * 74 + (j * 15 + 18)), i * 106 + 182);
                            HuSprScaleSet(grpId, member, 0.7125f, 1.0f);
                            HuSprBankSet(grpId, member, str[j] & 0xF);
                            HuSprColorSet(grpId, member, 0xFA, 0xDC, 0xA3);
                            HuSprAttrSet(grpId, member, 8);
                            member++;
                        }
                    }
                }
            }
        }
    }
    HuSprGrpDrawNoSet(grpId, 0x40);
    HuSprGrpPosSet(grpId, 149.0f, 0.0f);
    HuSprGrpScissorSet(grpId, 0x5F, 0x5B, 0x204, 0x102);
    FingerMdlId = Hu3DModelCreate(HuDataSelHeapReadNum(STAT_DATA(0x40), HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelCameraSet(FingerMdlId, 4);
    Hu3DModelAttrSet(FingerMdlId, 1);
    Hu3DModelAttrSet(FingerMdlId, 0x40000001);
}

void PResStatMain(void)
{
    f32 t;
    s16 teamNum;
    s16 i, j;
    s16 autoPlay;
    s16 page;
    s16 mesNo;

    t = 0.0f;
    if (PResTeamF != 0) {
        teamNum = 2;
    } else {
        teamNum = 4;
    }
    for (i = 0; i < 4; i++) {
        StatScorePosSet(i, 640.0f, 0.0f);
    }
    StatDetailPosSet(640.0f, 0.0f);
    WipeCreate(1, 0, 60);
    while (WipeCheckEnd() != 0) {
        HuPrcVSleep();
    }
    for (j = 1; j < 25; j++) {
        for (i = 0; i < 4; i++) {
            if (j - i >= 0 && j - i <= 20) {
                t = (j - i) / 20.0;
                StatScorePosSet(i, 640.0 * (1.0 - HuSin(90.0f * t)), 0.0f);
            }
        }
        HuWinPosSet(HelpWinId, 8.0f, 406.0 + 100.0 * HuCos(90.0f * t));
        HuPrcVSleep();
    }
    while (1) {
        for (i = 0; i < 4; i++) {
            if (GwPlayerConf[i].type == 0) {
                break;
            }
        }
        if (i == 4) {
            autoPlay = 1;
        } else {
            autoPlay = 0;
        }
        PResBtnAWait();
        HuAudFXPlay(1);
        for (j = 1; j <= 20; j++) {
            t = j / 20.0;
            for (i = 0; i < 4; i++) {
                StatScorePosSet(i, 0.0f, 640.0 * (1.0 - HuCos(90.0f * t)));
            }
            HuWinPosSet(HelpWinId, 8.0f, 406.0 + 100.0 * HuSin(90.0f * t));
            HuPrcVSleep();
        }
        HuWinMesSet(HelpWinId, 0x2001B);
        for (j = 1; j <= 20; j++) {
            t = j / 20.0;
            StatDetailPosSet(660.0 * (1.0 - HuSin(90.0f * t)), 0.0f);
            HuWinPosSet(HelpWinId, 8.0f, 340.0 + 150.0 * HuCos(90.0f * t));
            HuPrcVSleep();
        }
        HuWinHomeClear(MessWinId);
        HuWinExOpen(MessWinId);
        while (1) {
            HuWinMesSet(MessWinId, 655360.0f + DetailSelIdx + DetailGrpIdxOfsX);
            if (autoPlay != 0) {
                HuPrcSleep(30);
            }
            while (!(HuPadBtnDown[0] & 0x100) && autoPlay == 0) {
                if (HuPadBtnDown[0] & 0x200) {
                    HuAudFXPlay(3);
                    HuWinExClose(MessWinId);
                    for (j = 1; j <= 20; j++) {
                        t = j / 20.0;
                        StatDetailPosSet(660.0 * (1.0 - HuCos(90.0f * t)), 0.0f);
                        HuWinPosSet(HelpWinId, 8.0f, 340.0 + 150.0 * HuSin(90.0f * t));
                        HuPrcVSleep();
                    }
                    HuWinMesSet(HelpWinId, 0x2000A);
                    for (j = 1; j <= 20; j++) {
                        t = j / 20.0;
                        for (i = 0; i < 4; i++) {
                            StatScorePosSet(i, 640.0 * (1.0 - HuSin(90.0f * t)), 0.0f);
                        }
                        HuWinPosSet(HelpWinId, 8.0f, 406.0 + 100.0 * HuCos(90.0f * t));
                        HuPrcVSleep();
                    }
                    goto top;
                }
                if (HuPadDStk[0] & 1) {
                    if (DetailSelIdx == 0.0f) {
                        if (DetailGrpIdxOfsX > 0.0f) {
                            HuAudFXPlay(0);
                            for (j = 1; j <= 10; j++) {
                                DetailGrpOfsX = 74.0f * -DetailGrpIdxOfsX + 74.0 * HuSin(90.0f * (f32)(j / 10.0));
                                StatDetailPosSet(0.0f, 0.0f);
                                HuPrcVSleep();
                            }
                            DetailGrpIdxOfsX -= 1.0f;
                            DetailGrpOfsX = 74.0f * -DetailGrpIdxOfsX;
                            HuWinMesSet(MessWinId, 655360.0f + DetailSelIdx + DetailGrpIdxOfsX);
                        }
                    } else {
                        HuAudFXPlay(0);
                        for (j = 1; j <= 10; j++) {
                            DetailCursorPosX = 74.0f * DetailSelIdx - 74.0 * HuSin(90.0f * (f32)(j / 10.0));
                            StatDetailPosSet(0.0f, 0.0f);
                            HuPrcVSleep();
                        }
                        DetailSelIdx -= 1.0f;
                        DetailCursorPosX = 74.0f * DetailSelIdx;
                        HuWinMesSet(MessWinId, 655360.0f + DetailSelIdx + DetailGrpIdxOfsX);
                    }
                } else if (HuPadDStk[0] & 2) {
                    if (DetailSelIdx >= 5.0f) {
                        if (DetailGrpIdxOfsX <= 3.0f) {
                            HuAudFXPlay(0);
                            for (j = 1; j <= 10; j++) {
                                DetailGrpOfsX = 74.0f * -DetailGrpIdxOfsX - 74.0 * HuSin(90.0f * (f32)(j / 10.0));
                                StatDetailPosSet(0.0f, 0.0f);
                                HuPrcVSleep();
                            }
                            DetailGrpIdxOfsX += 1.0f;
                            DetailGrpOfsX = 74.0f * -DetailGrpIdxOfsX;
                            HuWinMesSet(MessWinId, 655360.0f + DetailSelIdx + DetailGrpIdxOfsX);
                        }
                    } else {
                        HuAudFXPlay(0);
                        for (j = 1; j <= 10; j++) {
                            DetailCursorPosX = 74.0f * DetailSelIdx + 74.0 * HuSin(90.0f * (f32)(j / 10.0));
                            StatDetailPosSet(0.0f, 0.0f);
                            HuPrcVSleep();
                        }
                        DetailSelIdx += 1.0f;
                        DetailCursorPosX = 74.0f * DetailSelIdx;
                        HuWinMesSet(MessWinId, 655360.0f + DetailSelIdx + DetailGrpIdxOfsX);
                    }
                }
                HuPrcVSleep();
            }
            HuAudFXPlay(1);
            page = 0;
            PlayerSel = 0;
            HuWinHomeClear(MessWinId);
            for (i = 0; i < teamNum; i++) {
                GraphSprP[i]->work[1] = 0;
            }
            HuWinMesSet(HelpWinId, 0x2001C);
            HuSprTPLvlSet(DetailOverGrpId, 3, 1.0f);
            HuSprTPLvlSet(DetailOverGrpId, 4, 0.0f);
            Hu3DModelAttrReset(FingerMdlId, 1);
            for (j = 0; j < teamNum; j++) {
                HuSprPriSet(DetailOverGrpId, j + 5, 0x6A);
            }
            HuSprPriSet(DetailOverGrpId, PlayerSel / 2 + 5, 0x69);
            StatFingerPosSet(PlayerSel);
            for (i = 1; i <= 20; i++) {
                t = i / 20.0;
                HuSprGrpTPLvlSet(DetailGrpId, 1.0 - t);
                HuSprTPLvlSet(DetailOverGrpId, 2, 1.0 - t);
                Hu3DModelScaleSet(FingerMdlId, 1.5 * t, 1.5 * t, 1.5 * t);
                HuPrcVSleep();
            }
            if (PResTeamF == 0) {
                mesNo = GwPlayerConf[PlayerSel].charNo;
            } else {
                mesNo = PResTeamNameMesGet(PResCharNo[PlayerSel & 2], PResCharNo[(PlayerSel & 2) + 1]);
            }
            HuWinInsertMesSet(MessWinId, mesNo, 0);
            HuWinMesSet(MessWinId, 0xA000A);
            if (autoPlay != 0) {
                HuPrcSleep(30);
            }
            while (!(HuPadBtnDown[0] & 0x100) && autoPlay == 0) {
                if (HuPadBtnDown[0] & 0x200) {
                    HuAudFXPlay(3);
                    HuWinHomeClear(MessWinId);
                    for (i = 1; i <= 20; i++) {
                        t = i / 20.0;
                        HuSprGrpTPLvlSet(DetailGrpId, t);
                        HuSprTPLvlSet(DetailOverGrpId, 2, t);
                        t = 1.0 - t;
                        Hu3DModelScaleSet(FingerMdlId, 1.5 * t, 1.5 * t, 1.5 * t);
                        HuPrcVSleep();
                    }
                    HuWinMesSet(HelpWinId, 0x2001B);
                    goto detail;
                }
                if (HuPadBtnDown[0] & 0x20) {
                    HuAudFXPlay(0);
                    if (page == 0) {
                        HuSprPriSet(DetailOverGrpId, 3, 0x6E);
                        HuSprPriSet(DetailOverGrpId, 4, 0x6F);
                        HuSprTPLvlSet(DetailOverGrpId, 4, 1.0f);
                    } else {
                        HuSprPriSet(DetailOverGrpId, 3, 0x6F);
                        HuSprPriSet(DetailOverGrpId, 4, 0x6E);
                        HuSprTPLvlSet(DetailOverGrpId, 3, 1.0f);
                    }
                    for (i = 1; i <= 20; i++) {
                        t = i / 20.0;
                        if (page == 0) {
                            HuSprTPLvlSet(DetailOverGrpId, 3, 1.0 - t);
                        } else {
                            HuSprTPLvlSet(DetailOverGrpId, 4, 1.0 - t);
                        }
                        for (j = 0; j < teamNum; j++) {
                            HuSprTPLvlSet(DetailOverGrpId, j + 5, 1.0 - t);
                        }
                        HuPrcVSleep();
                    }
                    page ^= 1;
                    for (i = 0; i < teamNum; i++) {
                        GraphSprP[i]->work[1] = page;
                    }
                    if (PResTeamF == 0) {
                        mesNo = GwPlayerConf[PlayerSel].charNo;
                    } else {
                        mesNo = PResTeamNameMesGet(PResCharNo[PlayerSel & 2], PResCharNo[(PlayerSel & 2) + 1]);
                    }
                    HuWinInsertMesSet(MessWinId, mesNo, 0);
                    HuWinMesSet(MessWinId, page + 0xA000A);
                    for (i = 1; i <= 10; i++) {
                        for (j = 0; j < teamNum; j++) {
                            HuSprTPLvlSet(DetailOverGrpId, j + 5, i / 10.0);
                        }
                        HuPrcVSleep();
                    }
                }
                if (HuPadDStkRep[0] & 8) {
                    HuAudFXPlay(0);
                    PlayerSel--;
                    if (PlayerSel < 0) {
                        PlayerSel = 3;
                    }
                    if (PResTeamF == 0) {
                        for (j = 0; j < 4; j++) {
                            HuSprPriSet(DetailOverGrpId, j + 5, 0x6A);
                        }
                        HuSprPriSet(DetailOverGrpId, PlayerSel + 5, 0x69);
                        mesNo = GwPlayerConf[PlayerSel].charNo;
                    } else {
                        HuSprPriSet(DetailOverGrpId, (PlayerSel / 2 ^ 1) + 5, 0x6A);
                        HuSprPriSet(DetailOverGrpId, PlayerSel / 2 + 5, 0x69);
                        mesNo = PResTeamNameMesGet(PResCharNo[PlayerSel & 2], PResCharNo[(PlayerSel & 2) + 1]);
                    }
                    StatFingerPosSet(PlayerSel);
                    HuWinInsertMesSet(MessWinId, mesNo, 0);
                    HuWinMesSet(MessWinId, page + 0xA000A);
                } else if (HuPadDStkRep[0] & 4) {
                    HuAudFXPlay(0);
                    PlayerSel++;
                    if (PlayerSel >= 4) {
                        PlayerSel = 0;
                    }
                    if (PResTeamF == 0) {
                        for (j = 0; j < 4; j++) {
                            HuSprPriSet(DetailOverGrpId, j + 5, 0x6A);
                        }
                        HuSprPriSet(DetailOverGrpId, PlayerSel + 5, 0x69);
                        mesNo = GwPlayerConf[PlayerSel].charNo;
                    } else {
                        HuSprPriSet(DetailOverGrpId, (PlayerSel / 2 ^ 1) + 5, 0x6A);
                        HuSprPriSet(DetailOverGrpId, PlayerSel / 2 + 5, 0x69);
                        mesNo = PResTeamNameMesGet(PResCharNo[PlayerSel & 2], PResCharNo[(PlayerSel & 2) + 1]);
                    }
                    StatFingerPosSet(PlayerSel);
                    HuWinInsertMesSet(MessWinId, mesNo, 0);
                    HuWinMesSet(MessWinId, page + 0xA000A);
                }
                HuPrcVSleep();
            }
            break;
        detail:;
        }
        break;
    top:;
    }
    HuAudFXPlay(1);
    WipeCreate(2, 0, 60);
    HuAudSStreamAllFadeOut(1000);
    while (WipeCheckEnd() != 0) {
        HuPrcVSleep();
    }
}

static void StatFingerPosSet(volatile s16 n)
{
    Vec in;
    Vec out;

    in.x = 27.0f;
    in.y = (f32)(n * 53 + 118);
    in.z = 6500.0f;
    Hu3D2Dto3D(&in, 4, &out);
    Hu3DModelPosSetV(FingerMdlId, &out);
}

static void StatScorePosSet(s16 n, f32 x, f32 y)
{
    HuVecF out;
    HuVecF in;
    s16 order[4];
    s16 team[2];
    s16 idx[4];
    s16 rank[4];
    s16 i;
    s32 ofs;

    if (PResTeamF == 0) {
        PResResultOrderGet(&order, &idx, &rank);
        in.x = 140.0f + x;
        in.y = y + (n * 75 + 150);
        in.z = 6500.0f;
        Hu3D2Dto3D(&in, 4, &out);
        Hu3DModelPosSetV(ToueiMdlId[order[n]], &out);
        Hu3DModelScaleSet(ToueiMdlId[order[n]], 0.4f, 0.4f, 0.2f);
        in.x = 210.0f + x;
        in.y = y + (n * 75 + 150);
        in.z = 6500.0f;
        Hu3D2Dto3D(&in, 4, &out);
        Hu3DModelPosSetV(StarMdlId[order[n]], &out);
        in.x = 380.0f + x;
        in.y = y + (n * 75 + 150);
        in.z = 6500.0f;
        Hu3D2Dto3D(&in, 4, &out);
        Hu3DModelPosSetV(CoinMdlId[order[n]], &out);
        HuSprGrpPosSet(ScoreGrpId[order[n]], x, y);
        return;
    }
    if (n < 2) {
        PResResultTeamOrderGet(team);
        if (team[0] == 0) {
            for (i = 0; i < 4; i++) {
                order[i] = i;
            }
        } else {
            for (i = 0; i < 2; i++) {
                order[i] = i + 2;
            }
            for (i = 0; i < 2; i++) {
                order[i + 2] = i;
            }
        }
        for (i = 0; i < 2; i++) {
            in.x = 140.0f + x;
            if (order[i] & 1) {
                ofs = 36;
            } else {
                ofs = -36;
            }
            in.y = y + (n * 150 + ofs + 180);
            in.z = 6500.0f;
            Hu3D2Dto3D(&in, 4, &out);
            Hu3DModelPosSetV(ToueiMdlId[order[i + n * 2]], &out);
            Hu3DModelScaleSet(ToueiMdlId[order[i + n * 2]], 0.4f, 0.4f, 0.2f);
        }
        in.x = 210.0f + x;
        in.y = y + (n * 150 + 210);
        in.z = 6500.0f;
        Hu3D2Dto3D(&in, 4, &out);
        Hu3DModelPosSetV(StarMdlId[n], &out);
        in.x = 380.0f + x;
        in.y = y + (n * 150 + 210);
        in.z = 6500.0f;
        Hu3D2Dto3D(&in, 4, &out);
        Hu3DModelPosSetV(CoinMdlId[n], &out);
        HuSprGrpPosSet(ScoreGrpId[n], x, y);
        HuWinPosSet(TeamNameWinId[n], 360.0f + x, y + (n * 150 + 190));
    }
}

static void StatDetailPosSet(f32 x, f32 y)
{
    HuVecF out;
    HuVecF in;
    f32 left, top, w, h;
    s16 i;

    for (i = 0; i < 4; i++) {
        in.x = 52.0f + x;
        in.y = y + (i * 53 + 143);
        in.z = 6500.0f;
        Hu3D2Dto3D(&in, 4, &out);
        Hu3DModelPosSetV(ToueiMdlId[i], &out);
        Hu3DModelScaleSet(ToueiMdlId[i], 0.3f, 0.3f, 0.3f);
    }
    HuSprGrpPosSet(DetailOverGrpId, x, y);
    HuSprPosSet(DetailOverGrpId, 2, 134.0f + DetailCursorPosX, 220.0f);
    HuSprGrpPosSet(DetailGrpId, 149.0f + x + DetailGrpOfsX, y);
    left = 95.33333333333334 + x;
    top = 91.0f + y;
    w = 515.55554f;
    h = 258.0f;
    if (left < 0.0f) {
        w = 515.55554f + left;
        left = 0.0f;
    } else if (left > 640.0f) {
        HuSprGrpPosSet(DetailGrpId, 1500.0f, 0.0f);
        return;
    }
    if (left + w > 640.0f) {
        w = 640.0f - left;
    }
    if (top < 0.0f) {
        h = 258.0f + top;
        top = 0.0f;
    } else if (top > 480.0f) {
        return;
    }
    if (top + h > 480.0f) {
        h = 480.0f - top;
    }
    if (w <= 0.0f || h <= 0.0f) {
        HuSprGrpPosSet(DetailGrpId, 1500.0f, 0.0f);
        return;
    }
    HuSprGrpScissorSet(DetailGrpId, left, top, w, h);
}

static void StatModelUpdate(void)
{
    f32 coinRot;
    f32 starRot;
    s16 i;
    s16 num;

    coinRot = 0.0f;
    starRot = 0.0f;
    while (1) {
        if (PResTeamF != 0) {
            num = 2;
        } else {
            num = 4;
        }
        for (i = 0; i < num; i++) {
            Hu3DModelRotSet(StarMdlId[i], 0.0f, starRot, 0.0f);
            Hu3DModelRotSet(CoinMdlId[i], 0.0f, coinRot, 0.0f);
        }
        coinRot += 1.0f;
        if (coinRot > 360.0f) {
            coinRot -= 360.0f;
        }
        starRot += 1.2;
        if (starRot > 360.0f) {
            starRot -= 360.0f;
        }
        Hu3DGLightPosSet(PResLightId, 0.0f, 1000.0f, 1000.0f, 0.0f, -1.0f, -1.0f);
        HuPrcVSleep();
    }
}

static void StatBackDraw(s16 layerNo)
{
    Mtx44 proj;
    Mtx mtx;
    GXColor color = { 0xFF, 0xFF, 0xFF, 0xFF };

    if (Hu3DCameraNo != 1) {
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
    HuSprTexLoad(BackAnimP, 0, 0, GX_REPEAT, GX_REPEAT, GX_LINEAR);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_S16, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetTevColor(GX_TEVREG0, color);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition2s16(0, 0);
    GXTexCoord2f32(BackScrollX, BackScrollY);
    GXPosition2s16(640, 0);
    GXTexCoord2f32(5.0 + BackScrollX, BackScrollY);
    GXPosition2s16(640, 480);
    GXTexCoord2f32(5.0 + BackScrollX, 5.0 + BackScrollY);
    GXPosition2s16(0, 480);
    GXTexCoord2f32(BackScrollX, 5.0 + BackScrollY);
    GXEnd();
    BackScrollX += 0.005;
    if (BackScrollX >= 1.0f) {
        BackScrollX = 0.0;
    }
    BackScrollY += 0.005;
    if (BackScrollY >= 1.0f) {
        BackScrollY = 0.0;
    }
}

static GXColor GraphColorTbl[6] = {
    { 0xDA, 0x32, 0x80, 0xFF },
    { 0x1E, 0xD7, 0x4B, 0xFF },
    { 0xF5, 0x91, 0x0F, 0xFF },
    { 0x0C, 0xC1, 0xF2, 0xFF },
    { 0xFF, 0x0C, 0x0C, 0xFF },
    { 0x0C, 0x0C, 0xFF, 0xFF },
};

static void StatGraphDraw(HUSPRITE *spr)
{
    StatLinePt P[4];
    StatLinePt d[3];
    StatLinePt n[3];
    StatLinePt p[3];
    StatLinePt A1, A2, B1, B2;
    Mtx mtx;
    GXColor amb = { 0xFF, 0xFF, 0xFF, 0xFF };
    GXColor mat = { 0xFF, 0xFF, 0xFF, 0xFF };
    GXColor col;
    f32 alphaScale;
    f32 scale;
    s16 *stat;
    s16 plNo;
    s16 page;
    s16 len;
    s16 i;

    plNo = spr->work[0];
    page = spr->work[1];
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetChanAmbColor(GX_COLOR0A0, amb);
    GXSetChanMatColor(GX_COLOR0A0, mat);
    if (PResTeamF == 0) {
        if (PlayerSel == plNo) {
            alphaScale = ((GlobalCounter & 7) < 6) ? 1 : 0;
        } else {
            alphaScale = 1.0f;
        }
        col = GraphColorTbl[plNo];
    } else {
        if (PlayerSel / 2 == plNo) {
            alphaScale = ((GlobalCounter & 7) < 6) ? 1 : 0;
        } else {
            alphaScale = 1.0f;
        }
        col = GraphColorTbl[plNo + 4];
    }
    col.a = alphaScale * spr->a;
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColor(GX_TEVREG0, col);
    GXSetNumTevStages(1);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_TRUE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT0, GX_DF_CLAMP, GX_AF_SPOT);
    if (spr->attr & 0x80) {
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_NOOP);
    } else if (spr->attr & 0x100) {
        GXSetBlendMode(GX_BM_BLEND, GX_BL_ZERO, GX_BL_INVDSTCLR, GX_LO_NOOP);
    } else {
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    }
    GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
    GXSetZCompLoc(GX_FALSE);
    PSMTXScale(mtx, spr->scale.x, spr->scale.y, 1.0f);
    mtxTransCat(mtx, spr->pos.x - 219.0, 126.0 + spr->pos.y, 0.0f);
    PSMTXConcat(*spr->groupMtx, mtx, mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    HuSprTexLoad(GraphAnimP, 0, 0, GX_CLAMP, GX_CLAMP, GX_LINEAR);
    if (PResTeamF == 0) {
        if (page == 0) {
            stat = PResPlayer[plNo].starStat;
        } else {
            stat = PResPlayer[plNo].coinStat;
        }
        len = PResPlayer[plNo].turnMax + 2;
    } else {
        if (page == 0) {
            stat = PResPlayer[plNo * 2].starStat;
        } else {
            stat = PResPlayer[plNo * 2].coinStat;
        }
        len = PResPlayer[plNo * 2].turnMax + 2;
    }
    for (i = 0; i < len - 1; i++, stat++) {
        scale = 3.0f + 1.1f * StatGraphMax[page];
        P[1].x = 5.0 + 438.0 * ((f64)(f32)i / ((f64)(f32)len - 1.0));
        P[1].y = 252.0 * -(stat[0] / scale) - 5.0;
        if (i != 0) {
            P[0].x = 5.0 + 438.0 * ((f64)(f32)(i - 1) / ((f64)(f32)len - 1.0));
            P[0].y = 252.0 * -(stat[-1] / (3.0f + 1.1f * StatGraphMax[page])) - 5.0;
        } else {
            P[0].x = P[1].x - 2.0f;
            P[0].y = P[1].y;
        }
        if (i + 1 < len) {
            P[2].x = 5.0 + 438.0 * ((f64)(f32)(i + 1) / ((f64)(f32)len - 1.0));
            P[2].y = 252.0 * -(stat[1] / (3.0f + 1.1f * StatGraphMax[page])) - 5.0;
        } else {
            P[2].x = 2.0f + P[1].x;
            P[2].y = P[1].y;
        }
        if (i + 2 < len) {
            P[3].x = 5.0 + 438.0 * ((f64)(f32)(i + 2) / ((f64)(f32)len - 1.0));
            P[3].y = 252.0 * -(stat[2] / (3.0f + 1.1f * StatGraphMax[page])) - 5.0;
        } else {
            P[3].x = 2.0f + P[2].x;
            P[3].y = P[2].y;
        }
        d[0].x = P[1].x - P[0].x;
        d[0].y = P[1].y - P[0].y;
        n[0].x = -d[0].y;
        n[0].y = d[0].x;
        d[1].x = P[2].x - P[1].x;
        d[1].y = P[2].y - P[1].y;
        n[1].x = -d[1].y;
        n[1].y = d[1].x;
        d[2].x = P[3].x - P[2].x;
        d[2].y = P[3].y - P[2].y;
        n[2].x = -d[2].y;
        n[2].y = d[2].x;
        StatLineNorm(&n[0], &n[0]);
        StatLineScale(&n[0], &n[0], 2.0f);
        StatLineTrans(&P[0], &n[0], &p[0]);
        StatLineNorm(&n[1], &n[1]);
        StatLineScale(&n[1], &n[1], 2.0f);
        StatLineTrans(&P[1], &n[1], &p[1]);
        StatLineNorm(&n[2], &n[2]);
        StatLineScale(&n[2], &n[2], 2.0f);
        StatLineTrans(&P[2], &n[2], &p[2]);
        if (StatLinePosGet(&p[0], &d[0], &p[1], &d[1], &A1) == 0) {
            A1 = p[1];
        }
        if (StatLinePosGet(&p[1], &d[1], &p[2], &d[2], &A2) == 0) {
            A2 = p[2];
        }
        StatLineScale(&n[0], &n[0], -1.0f);
        StatLineTrans(&P[0], &n[0], &p[0]);
        StatLineScale(&n[1], &n[1], -1.0f);
        StatLineTrans(&P[1], &n[1], &p[1]);
        StatLineScale(&n[2], &n[2], -1.0f);
        StatLineTrans(&P[2], &n[2], &p[2]);
        if (StatLinePosGet(&p[0], &d[0], &p[1], &d[1], &B1) == 0) {
            B1 = p[1];
        }
        if (StatLinePosGet(&p[1], &d[1], &p[2], &d[2], &B2) == 0) {
            B2 = p[2];
        }
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(A1.x, A1.y, 0.0f);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(A2.x, A2.y, 0.0f);
        GXTexCoord2f32(1.0f, 0.0f);
        GXPosition3f32(B2.x, B2.y, 0.0f);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3f32(B1.x, B1.y, 0.0f);
        GXTexCoord2f32(0.0f, 1.0f);
        GXEnd();
    }
}

static s32 StatLinePosGet(StatLinePt *p, StatLinePt *dir, StatLinePt *b, StatLinePt *c, StatLinePt *out)
{
    volatile s32 ret = 1;
    f32 t, t2, den, ad;

    t = dir->x * (b->y - p->y) - dir->y * (b->x - p->x);
    den = c->x * dir->y - c->y * dir->x;
    if (den < 0.0f) {
        ad = -den;
    } else {
        ad = den;
    }
    if (ad > 0.001f) {
        t = t / (c->x * dir->y - c->y * dir->x);
    } else {
        return 0;
    }
    if (dir->x != 0.0) {
        t2 = (b->x + t * c->x - p->x) / dir->x;
    } else {
        t2 = (b->y + t * c->y - p->y) / dir->y;
    }
    out->x = p->x + t2 * dir->x;
    out->y = p->y + t2 * dir->y;
    return 1;
}

// NONMATCHING: mwcceppc 1.3.2 expands 1.0/sqrt() into a guarded 4-iteration rsqrt plus a
// 1.0/x divide (it pulls in __float_huge/__float_nan); the original is a bare 3-iteration
// frsqrte rsqrt, and none of the 11 C spellings tried drops that guard.
static void StatLineNorm(StatLinePt *src, StatLinePt *dst)
{
    f64 e;
    f32 len, s, d;

    len = src->x * src->x + src->y * src->y;
    if (len > 0.0f) {
        e = 1.0 / sqrt(len);
        d = len * e;
    } else {
        d = len;
    }
    s = d;
    dst->x = src->x / s;
    dst->y = src->y / s;
}

static void StatLineScale(StatLinePt *src, StatLinePt *dst, f32 scale)
{
    dst->x = src->x * scale;
    dst->y = src->y * scale;
}

static void StatLineTrans(StatLinePt *a, StatLinePt *b, StatLinePt *out)
{
    out->x = a->x + b->x;
    out->y = a->y + b->y;
}
