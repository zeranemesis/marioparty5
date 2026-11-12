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

/* camera.c */
void (*fn_1_19E8(HUPROCESS *proc))(OMOBJ *);
void (*fn_1_1DF4(HUPROCESS *proc, OMOBJ *obj))(OMOBJ *);

/* player.c */
void fn_1_2290(HUPROCESS *proc);
void fn_1_2438(void);

/* stage.c */
void fn_1_3360(HUPROCESS *proc);
void fn_1_33E8(void);
s32 fn_1_4624(void);
BOOL fn_1_49BC(void);
BOOL fn_1_49FC(void);

/* effect.c */
void fn_1_6314(HUPROCESS *proc);
void fn_1_6338(void);

/* score.c */
void fn_1_6BDC(HUPROCESS *proc, OMOBJ **mainObj);
void fn_1_6C74(void);

/* m534.c */
void fn_1_444(OMOBJ *obj);
void fn_1_518(OMOBJ *obj);
s32 fn_1_1694(void);
s32 fn_1_16AC(void);
s32 fn_1_16C4(void);
s32 fn_1_16DC(void);
void fn_1_16F4(s32 arg);
s32 fn_1_170C(s32 no);
void fn_1_173C(s32 no);
void fn_1_176C(s32 no);
s32 fn_1_179C(void);
s32 fn_1_17C8(void);
void fn_1_1800(s32 no, s32 a, s32 b);

#endif
