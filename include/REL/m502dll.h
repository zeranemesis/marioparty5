#ifndef _M502DLL_H
#define _M502DLL_H

#include "dolphin.h"
#include "game/object.h"
#include "game/gamework.h"
#include "game/gamemes.h"
#include "game/wipe.h"
#include "game/audio.h"
#include "game/hu3d.h"
#include "game/charman.h"
#include "game/memory.h"
#include "game/pad.h"
#include "game/process.h"
#include "game/esprite.h"
#include "game/frand.h"
#include "game/data.h"
#include "game/sprite.h"
#include "game/mg/timer.h"
#include "math.h"
#include "humath.h"

typedef void (*M502_VOIDFUNC)(void);
extern const M502_VOIDFUNC _ctors[];
extern const M502_VOIDFUNC _dtors[];

/* camera.c: one camera pair per player (0x84 bytes on GC) */
typedef struct M502Camera_s {
    struct M502Player_s *player;
    s32 camBit1;
    s32 camBit2;
    s32 pad0C;
    Vec posGoal;    /* 0x10 */
    Vec tgtGoal;    /* 0x1C */
    Vec pos;        /* 0x28 */
    Vec tgt;        /* 0x34 */
    Vec up;         /* 0x40 */
    f32 rate;       /* 0x4C */
    f32 t;          /* 0x50 */
    Vec posA;       /* 0x54 */
    Vec tgtA;       /* 0x60 */
    Vec posB;       /* 0x6C */
    Vec tgtB;       /* 0x78 */
} M502CAMERA;

/* stage.c (0x74 bytes on GC) */
typedef struct M502Stage_s {
    struct M502Player_s *player;
    s16 wall[5];        /* 0x04 */
    s16 flame[3];       /* 0x0E; GC pads this to 0x14 */
    s16 water;          /* 0x14 */
    s16 pad16;
    s32 state;          /* 0x18 */
    s32 flameIdx;       /* 0x1C */
    s32 state2;         /* 0x20 */
    f32 offY;           /* 0x24 */
    f32 scaleX;         /* 0x28 */
    f32 scaleY;         /* 0x2C */
    f32 scaleY2;        /* 0x30 */
    f32 scroll;         /* 0x34 */
    f32 rate;           /* 0x38 */
    s16 model[4];       /* 0x3C */
    s16 modelNo[8];     /* 0x44 */
    char *name[8];      /* 0x54 */
} M502STAGE;

/* TEMP placeholders, expanded in the owning files */
typedef struct M502Block_s { u8 raw[0xF50]; } M502BLOCK;
typedef struct M502PlrSub_s { u8 raw0[0x60]; s32 lane; u8 raw[0x18]; } M502PLRSUB;
typedef struct M502Opening_s { u8 raw[0x24]; } M502OPENING;
typedef struct M502Bat_s { u8 raw[0x248]; } M502BAT;

/* models loaded once per game (0x35C bytes on GC) */
typedef struct M502Models_s {
    s16 model[0x8D];
    char *name[0x8D];
    void *texBuf;       /* 0x350 */
    ANIMDATA *anim1;    /* 0x354 */
    ANIMDATA *anim2;    /* 0x358 */
} M502MODELS;

typedef struct M502Light_s {
    s32 unk0;
    s16 id;
} M502LIGHT;

/* one player's state (0x1318 bytes on GC) */
typedef struct M502Player_s {
    struct M502Main_s *main;
    s32 no;
    M502BLOCK block;    /* 0x08 */
    M502PLRSUB plr;     /* 0xF58 */
    M502STAGE stage;    /* 0xFD4 */
    M502CAMERA camera;  /* 0x1048 */
    M502BAT bat;        /* 0x10CC */
    MGTIMER *timer;     /* 0x1314 */
} M502PLAYER;

/* the whole game (0x5028 bytes on GC) */
typedef struct M502Main_s {
    OMOBJMAN *objman;
    OMOBJ *obj;
    s32 state;          /* 0x08 */
    s32 unkC;
    s32 timer;          /* 0x10 */
    s32 vibTimer;       /* 0x14 */
    s32 mesId;          /* 0x18 */
    M502MODELS models;  /* 0x1C */
    M502LIGHT light;    /* 0x378 */
    M502OPENING opening;/* 0x380 */
    M502PLAYER player[4]; /* 0x3A4 */
    s16 firstPlayer;    /* 0x5004 */
    s32 unk5008;
    s32 gameTime;       /* 0x500C */
    MGTIMER *mgTimer;   /* 0x5010 */
    s32 versus;         /* 0x5014 */
    s32 bgmId;          /* 0x5018 */
    s32 seId;           /* 0x501C */
    s32 unk5020;
} M502MAIN;

extern M502MAIN *lbl_1_data_0;
extern s32 lbl_1_data_B60[8];


void fn_1_1830(M502CAMERA *cam, s32 playerNo, struct M502Player_s *player);
void fn_1_1A60(M502CAMERA *cam);
void fn_1_1A90(M502CAMERA *cam);
void fn_1_1B40(Vec *out, Vec *a, Vec *b, f32 t);
void fn_1_1CB4(M502CAMERA *cam, Vec *pos, Vec *up, Vec *tgt);
void fn_1_1D60(M502CAMERA *cam, Vec *pos, Vec *up, Vec *tgt, f32 *rate);
void fn_1_1DB4(M502CAMERA *cam, Vec *pos, Vec *up, Vec *tgt);
void fn_1_1E00(M502CAMERA *cam, Vec *pos, Vec *up, Vec *tgt);
void fn_1_1E5C(M502CAMERA *cam, f32 t);
s32 fn_1_1E64(M502CAMERA *cam, Vec *up, f32 time);

#endif
