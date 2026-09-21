#ifndef _RESULTDLL_H
#define _RESULTDLL_H

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
#include "game/board/player.h"

#define STAGE_MDL_MAX 128
#define STAGE_MOT_MAX 128
#define STAGE_SPR_MAX 128

#define STAGE_MDL_TYPE_MODEL 0
#define STAGE_MDL_TYPE_MOTION 1
#define STAGE_MDL_TYPE_LINK 2

typedef struct StageModel_s {
    unsigned int dataNum;
    u32 attr;
    s16 type;
    s16 linkMdl;
    s16 motMdl;
    s16 layer;
    u16 cameraBit;
    HuVecF pos;
    HuVecF rot;
    HuVecF scale;
} STAGE_MODEL;

typedef struct StageSprite_s {
    unsigned int dataNum;
    s16 attr;
    s16 pri;
    float posX;
    float posY;
    GXColor color;
} STAGE_SPRITE;

extern HU3DMODELID StageToueiMdlId[GW_PLAYER_MAX];
extern HU3DMOTID StageCharMotId[GW_PLAYER_MAX][3];
extern HU3DMODELID StageCharMdlId[GW_PLAYER_MAX];
extern HUSPRID StageSprId[STAGE_SPR_MAX];
extern HU3DMOTID StageMotId[STAGE_MOT_MAX];
extern HU3DMODELID StageMdlId[STAGE_MDL_MAX];

void StageModelCreate(STAGE_MODEL *model);
void StageSpriteCreate(STAGE_SPRITE *sprite);
void StageSpriteKill(void);

void ToueiDispInit(s16 *charNo);
void ToueiDispClose(void);

#endif
