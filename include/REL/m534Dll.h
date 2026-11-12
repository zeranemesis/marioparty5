#ifndef _M534DLL_H
#define _M534DLL_H

#include "game/audio.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/flag.h"
#include "game/frand.h"
#include "game/gamemes.h"
#include "game/gamework.h"
#include "game/hsfex.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/process.h"
#include "game/sprite.h"
#include "game/wipe.h"
#include "game/mg/seqman.h"
#include "game/mg/timer.h"

#include "humath.h"
#include "dolphin.h"

#include <math.h>
#include <string.h>

/* module controller (OMOBJ data of fn_1_36C), 0x60 bytes */
typedef struct M534Main_s {
    s32 state;    /* 0x00 */
    s32 timer4;   /* 0x04 */
    s32 busy;     /* 0x08 */
    u32 camPhase; /* 0x0C */
    u32 camTimer; /* 0x10 */
    s32 phase;    /* 0x14 */
    s32 timer18;  /* 0x18 */
    s32 endPhase; /* 0x1C */
    u32 endTimer; /* 0x20 */
    u32 unk24[4]; /* 0x24 */
    s32 unk34[4]; /* 0x34 */
    s32 unk44[4]; /* 0x44 */
    u32 mode;     /* 0x54 */
    u32 flags58;  /* 0x58 */
    u32 flags5C;  /* 0x5C */
} M534MAIN;

/* player work (OMOBJ data of fn_1_245C), 0x60 bytes */
typedef struct M534Player_s {
    s32 no;        /* 0x00 */
    s32 team;      /* 0x04 */
    s32 slot;      /* 0x08 */
    s32 charNo;    /* 0x0C */
    s32 motion;    /* 0x10 */
    u32 btnDown;   /* 0x14 */
    u32 btn;       /* 0x18 */
    s32 cpu;       /* 0x1C */
    f32 speed;     /* 0x20 */
    f32 wait;      /* 0x24 */
    s32 state;     /* 0x28 */
    s32 timer;     /* 0x2C */
    s32 counter;   /* 0x30 */
    s32 flag34;    /* 0x34 */
    s32 frames;    /* 0x38 */
    f32 animSpeed; /* 0x3C */
    HuVecF pos;    /* 0x40 */
    f32 rotY;      /* 0x4C */
    f32 scaleY;    /* 0x50 */
    f32 scaleXZ;   /* 0x54 */
    s32 flag58;    /* 0x58 */
    s32 flag5C;    /* 0x5C */
} M534PLAYER;

/* camera.c */
s32 fn_1_1840(f32 x);
s32 fn_1_189C(HuVecF *pos);
void fn_1_1930(s32 seId, HuVecF *pos);
void (*fn_1_19E8(HUPROCESS *proc))(OMOBJ *);
void (*fn_1_1DF4(HUPROCESS *proc, OMOBJ *obj))(OMOBJ *);

/* player.c */
void fn_1_2290(HUPROCESS *proc);
void fn_1_2438(void);
s32 fn_1_318C(OMOBJ *obj, u32 motion);

/* stage.c */
void fn_1_3360(HUPROCESS *proc);
void fn_1_33E8(void);
s32 fn_1_4624(void);
BOOL fn_1_49BC(void);
BOOL fn_1_49FC(void);
void fn_1_4114(s32 slot);
void fn_1_43D4(s32 slot);
void fn_1_449C(s32 team);
f32 fn_1_4848(void);

/* effect.c */
void fn_1_6314(HUPROCESS *proc);
void fn_1_6338(void);

void fn_1_6458(HuVecF *pos);
void fn_1_6938(HuVecF *pos, HuVecF *vel);

/* score.c */
void fn_1_74B0(s32 team, s32 amount);
s32 fn_1_74E8(s32 team);
void fn_1_6BDC(HUPROCESS *proc, OMOBJ **mainObj);
void fn_1_6C74(void);

/* m534.c */
void fn_1_444(OMOBJ *obj);
void fn_1_518(OMOBJ *obj);
u32 fn_1_1694(void);
u32 fn_1_16AC(void);
u32 fn_1_16C4(void);
u32 fn_1_16DC(void);
void fn_1_16F4(s32 arg);
s32 fn_1_170C(s32 no);
void fn_1_173C(s32 no);
void fn_1_176C(s32 no);
s32 fn_1_179C(void);
s32 fn_1_17C8(void);
void fn_1_1800(s32 no, s32 a, s32 b);

#endif
