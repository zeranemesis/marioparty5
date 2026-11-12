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
    s32 unkC;     /* 0x0C */
    s32 timer10;  /* 0x10 */
    s32 phase;    /* 0x14 */
    s32 timer18;  /* 0x18 */
    s32 unk1C;    /* 0x1C */
    s32 timer20;  /* 0x20 */
    s32 unk24[4]; /* 0x24 */
    s32 unk34[4]; /* 0x34 */
    s32 unk44[4]; /* 0x44 */
    s32 unk54;    /* 0x54 */
    u32 flags58;  /* 0x58 */
    u32 flags5C;  /* 0x5C */
} M534MAIN;

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
