#ifndef _PARTYRESULTDLL_H
#define _PARTYRESULTDLL_H

#include "game/object.h"
#include "game/hu3d.h"
#include "game/sprite.h"
#include "game/gamework.h"
#include "game/audio.h"
#include "game/window.h"
#include "game/wipe.h"
#include "game/pad.h"
#include "game/data.h"
#include "game/charman.h"
#include "game/process.h"
#include "game/memory.h"

#include "datanum/char.h"
#include "datanum/charmot.h"
#include "messnum/teamname.h"

#define PRES_PLAYER_MOT_MAX 10

typedef struct PResPlayer_s {
    s16 charNo;
    s16 playerNo;
    s16 star;
    s16 coin;
    s16 bonusStar;
    s16 coinTotalMg;
    s16 coinMax;
    s16 hatenaMasuNum;
    s16 statStar;
    s16 statCoin;
    s16 statCoinTotalMg;
    s16 statCoinMax;
    s16 statBlueMasuNum;
    s16 statRedMasuNum;
    s16 statCapsuleMasuNum;
    s16 statHatenaMasuNum;
    s16 statKupaMasuNum;
    s16 statDonkeyMasuNum;
    s16 turnMax;
    s16 starStat[GW_STAT_LEN];
    s16 coinStat[GW_STAT_LEN];
} PRES_PLAYER;

extern OMOBJMAN *objman;
extern s32 PResLightId;
extern BOOL PResTeamF;
extern s16 PResCharNo[GW_PLAYER_MAX];
extern s16 PResPlayerOrder[GW_PLAYER_MAX];
extern PRES_PLAYER PResPlayer[GW_PLAYER_MAX];
extern HU3DMOTID PResPlayerMotion[GW_PLAYER_MAX][PRES_PLAYER_MOT_MAX];
extern HU3DMODELID PResPlayerModelId[8];

void ObjectSetup(void);
void PResMain(void);
void PResExit(void);
void CameraMove(OMOBJ *obj);

void PResResultCreate(void);
void PResResultMain(void);

void PResStatCreate(void);
void PResStatMain(void);

void ToueiDispInit(HU3DMODELID *mdlId);

u32 PResTeamNameMesGet(s16 charNo1, s16 charNo2);
void PResBtnAWait(void);
void PResComInputWait(void);

#endif
