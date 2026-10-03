#include "REL/resultDll.h"

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];
extern s32 MBDataDir[];

void ObjectSetup(void);

s32 Result_FastF;
s32 Result_BgReadEndF;

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

static s16 PlayerRank[4];
static s16 PlayerOrder[4];

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

s16 Result_CharMotEnd(s16 playerNo)
{
    if (Hu3DMotionShiftIDGet(StageCharMdlId[playerNo]) == -1) {
        if (Hu3DMotionEndCheck(StageCharMdlId[playerNo]) != 0) {
            return 1;
        }
    }
    return 0;
}
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
