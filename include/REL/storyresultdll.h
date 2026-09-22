#ifndef _STORYRESULTDLL_H
#define _STORYRESULTDLL_H

#include "game/object.h"
#include "game/hu3d.h"
#include "game/hsfex.h"
#include "game/sprite.h"
#include "game/gamework.h"
#include "game/mgdata.h"
#include "game/audio.h"
#include "game/window.h"
#include "game/wipe.h"
#include "game/pad.h"
#include "game/data.h"
#include "game/memory.h"
#include "game/process.h"
#include "game/frand.h"
#include "game/flag.h"
#include "game/saveload.h"
#include "game/charman.h"
#include "game/init.h"

typedef struct SRankWork_s {
    s16 dif;
    s16 rank;
    s16 storyNo;
    s16 mapRank[5];
} SRANK_WORK;

extern s32 debugCamOnF;
extern s32 PResGLight;
extern SRANK_WORK SRankWork;
extern s16 CapsuleMdlId;
extern s16 CharMotId;
extern s16 CharMdlId;
extern s16 PResDiff;
extern u8 LanguageNo;
extern OMOBJMAN *objman;

extern f32 CapsuleGlowTPLvl;

void ObjectSetup(void);
void PResExec(void);
void PResCreate(void);
void DebugCamUpdate(OMOBJ *obj);

void SRankCreate(void);
void SRankExec(void);
void RankObjRotExec(void);
void RankBackDraw(s16 layerNo);

void ToueiDispInit(void);
void DispZClear(void);

#endif
