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

#include "messnum/teamname.h"

extern HU3DMODELID PResPlayerModelId[8];
extern s16 PResCharNo[GW_PLAYER_MAX];
extern s32 PResLightId;

void ToueiDispInit(HU3DMODELID *mdlId);

u32 PResTeamNameMesGet(s16 charNo1, s16 charNo2);
void PResBtnAWait(void);
void PResComInputWait(void);

#endif
