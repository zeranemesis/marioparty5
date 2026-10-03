#include "REL/resultDll.h"

s32 Result_FastF;
s32 Result_BgReadEndF;
static s32 BattleSkipF;

void Result_Debug(void);
void Result_BgRead(void);
void Result_BattleCoinMdlMain(void);
void Result_BattleCreate(void);
void Result_BattleStart(void);
void Result_BattleAddCoin(void);
void Result_BattleAddRandomCoin(void);
void Result_BattleShowBonus(void);
void Result_AddBonusCoin(void);
void Result_BoardSave(void);

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