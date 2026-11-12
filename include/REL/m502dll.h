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
#include "game/flag.h"
#include <stddef.h>
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
typedef struct M502Input_s { u8 raw[0x1C]; } M502INPUT;
typedef struct M502PlrSub_s {
    struct M502Player_s *player;
    u16 mode;       /* 0x04 */
    u16 phase;      /* 0x06 */
    u16 action;     /* 0x08 */
    s16 step;       /* 0x0A */
    s32 unkC;
    s32 playerNo;   /* 0x10 */
    s32 charNo;     /* 0x14 */
    s32 grp;        /* 0x18 */
    OMOBJ *obj;     /* 0x1C */
    s32 active;     /* 0x20 */
    M502INPUT input; /* 0x24 */
    s32 v40;
    s32 v44;
    s32 v48;
    u8 pad4C[0x14];
    s32 lane;       /* 0x60 */
    u8 pad64[8];
    f32 vel;        /* 0x6C */
    u8 pad70[4];
    s32 flag74;     /* 0x74 */
    u8 pad78[4];
} M502PLRSUB;
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
    s16 pad6;
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
    s32 unk5024;
} M502MAIN;

extern M502MAIN *lbl_1_data_0;
extern M502MAIN lbl_1_bss_0;
extern s32 lbl_1_bss_5028;
extern s16 lbl_1_bss_502C;
extern s32 lbl_1_bss_5030;

s32 rand8(void);
void ObjectSetup(void);
void fn_1_134(OMOBJ *obj);
int fn_1_198(int seId);
void fn_1_1C0(int seNo);
void fn_1_1E8(int seNo, s32 speed);
void fn_1_218(int seNo, s16 pan);
void fn_1_248(Vec *pos, Vec *heading, f32 sndDist, f32 sndSpeed, f32 startDis, f32 frontSurDis, f32 backSurDis);
int fn_1_2A0(s16 streamId);
void fn_1_2C8(int streamNo, s32 speed);
void fn_1_2F8(void);
int fn_1_318(s16 streamId);
int fn_1_340(s16 streamId);
int fn_1_368(s16 streamId, s16 chanNo);
void fn_1_398(M502MAIN *m, OMOBJMAN *objman, OMOBJ *obj);
void fn_1_504(M502MAIN *m);
s32 fn_1_5DC(M502MAIN *m);
void fn_1_FD8(M502MAIN *m);
void fn_1_1018(M502MAIN *m);
void fn_1_1104(M502MAIN *m);
void fn_1_1350(M502MAIN *m);
void fn_1_13A0(M502MAIN *m);
void fn_1_13F0(M502MODELS *models);
void fn_1_147C(M502MODELS *models);
void fn_1_14C0(M502MODELS *models);
void fn_1_1564(M502MODELS *models);
void fn_1_15C0(M502LIGHT *light, s32 unk);
void fn_1_1638(M502LIGHT *light);
void fn_1_168C(void);
void fn_1_1690(M502PLAYER *p, s32 no, M502MAIN *m);
void fn_1_1740(M502PLAYER *p);
s32 fn_1_17C4(M502PLAYER *p);

/* stage.c */
void fn_1_21B8(M502STAGE *st, s32 playerNo, M502PLAYER *player);
void fn_1_2704(M502STAGE *st);
void fn_1_27A8(M502STAGE *st);
void fn_1_27AC(void);
void fn_1_2878(HU3DDRAWOBJ *drawObj);
void fn_1_2E08(M502STAGE *st, f32 speed);
void fn_1_3298(M502STAGE *st);
void fn_1_3380(f32 v);
void fn_1_3390(HU3DDRAWOBJ *drawObj);

/* player.c */
void fn_1_38E0(M502PLRSUB *plr, s32 no, M502PLAYER *player);
void fn_1_3B88(M502PLRSUB *plr);
s32 fn_1_3BF4(M502PLRSUB *plr);

/* block.c */
void fn_1_6664(M502BLOCK *b, s32 no, M502PLAYER *player);
void fn_1_6778(M502BLOCK *b);
void fn_1_6808(M502BLOCK *b);
void fn_1_7290(M502BLOCK *dst, M502BLOCK *src);
void fn_1_735C(M502BLOCK *b);
void fn_1_8F04(M502BLOCK *b);

/* opening.c / ending.c / bat.c */
void fn_1_9468(M502OPENING *o, M502MAIN *m);
void fn_1_95B4(M502OPENING *o);
void fn_1_9604(M502OPENING *o);
s32 fn_1_9644(M502MAIN *m);
s32 fn_1_A914(M502MAIN *m);
void fn_1_C468(M502BAT *b, M502PLAYER *player);
void fn_1_C4F0(M502BAT *b);
void fn_1_C554(M502BAT *b);
void fn_1_CAC8(M502BAT *b);
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
