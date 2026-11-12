#ifndef _M576DLL_H
#define _M576DLL_H

#include "types.h"
#include "string.h"
#include "game/audio.h"
#include "game/data.h"
#include "game/frand.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/process.h"
#include "game/sprite.h"
#include "game/charman.h"
#include "game/wipe.h"
#include "game/mg/seqman.h"
#include "game/mg/actman.h"
#include "dolphin/mtx.h"

/* actman.c is plain C and the header does not declare it */
void MgActorPosGet(MGACTOR *actorP, HuVecF *pos);

#define M576_PLAYER_MAX 4
#define M576_COIN_MAX 5
#define M576_BOMB_MAX 4

/* m576.c */
extern OMOBJ *lbl_1_bss_0;
extern OMOBJMAN *lbl_1_bss_4;
extern s32 lbl_1_bss_8;
extern s32 lbl_1_bss_14;
extern s32 lbl_1_bss_2A0[M576_COIN_MAX];
extern MGPLAYER *lbl_1_bss_2FC[M576_COIN_MAX];

#endif
