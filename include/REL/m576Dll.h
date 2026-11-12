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

void OSReport(const char *msg, ...);

#define M576_PLAYER_MAX 4
#define M576_COIN_MAX 5
#define M576_BOMB_MAX 4

typedef struct M576Com_s {
    /* 0x00 */ MGPLAYER *player;
    /* 0x04 */ u8 no;
    /* 0x08 */ HuVecF *trail;
    /* 0x0C */ s16 trailPos;
    /* 0x0E */ s16 trailMax;
    /* 0x10 */ HuVecF targetPos;
    /* 0x1C */ HuVecF pos;
    /* 0x28 */ f32 reach;
    /* 0x2C */ s16 target;
    /* 0x2E */ s16 coinTarget;
    /* 0x30 */ s16 timer30;
    /* 0x32 */ s16 timer32;
    /* 0x34 */ s16 timer34;
    /* 0x36 */ s16 cooldown;
    /* 0x38 */ s8 level;
    /* 0x3C */ s32 mode;
    /* 0x40 */ s8 targetType;
    /* 0x41 */ s8 state;
} M576COM;

/* m576.c */
s32 fn_1_174(s32 idx, HuVecF *dest);
s32 fn_1_1BC(HuVecF *dest);
s32 fn_1_234(HuVecF *dest);
s8 fn_1_424(s32 idx);
s32 fn_1_59C(void);
s32 fn_1_5F4(s32 idx, HuVecF *pos);
s32 fn_1_668(MGPLAYER **player, HuVecF *outPos, f32 *outDist);
s32 fn_1_798(MGPLAYER **player);
s32 fn_1_854(s32 playerNo);
s32 fn_1_8AC(s32 *outPlayer, HuVecF *outPos);
void fn_1_9A4(s32 idx, HuVecF *pos);
s32 fn_1_9EC(s32 idx);
u32 fn_1_A54(s32 idx);
extern OMOBJ *lbl_1_bss_0;
extern OMOBJMAN *lbl_1_bss_4;
extern s32 lbl_1_bss_8;
extern s32 lbl_1_bss_14;
extern s32 lbl_1_bss_2A0[M576_COIN_MAX];
extern MGPLAYER *lbl_1_bss_2FC[M576_COIN_MAX];

#endif
