#ifndef _M508DLL_H
#define _M508DLL_H

#include "REL/m508Dll/object.h"
#include "game/sprite.h"
#include "game/audio.h"
#include "game/frand.h"
#include "game/mg/timer.h"

/* m508.c's own objects are state machines built on the object library (REL/m508Dll/object.h):
 * a process runs modeFunc[mode](obj) and every mode function polls obj->mode once a frame.
 * Per-class data lives in the object work area, reached through M508_OBJWORK. */

typedef struct M508LightWork_s { /* DC, E4 */
    s16 light;
} M508_LIGHTWORK;

typedef struct M508SeqWork_s { /* E0: animation sequencer */
    s32 unk0;
    s32 seq;
} M508_SEQWORK;

typedef struct M508BoxSet_s { /* one entry of the per-round box layout (bss_114) */
    s32 kind;
    f32 rotSpeed;
    f32 shiftTime;
    f32 motSpeed;
} M508_BOXSET;

typedef struct M508BoxWork_s { /* bss_8: the five boxes */
    s32 kind;
    f32 rotSpeed;
    f32 shiftTime;
    f32 motSpeed;
    s16 light;
} M508_BOXWORK;

typedef struct M508CharWork_s { /* bss_A4: the four players */
    s32 hasItem;
    s32 item;
} M508_CHARWORK;

typedef struct M508HolderWork_s { /* bss_B4: item holders */
    s16 model;
    s16 light0;
    s16 light1;
} M508_HOLDERWORK;

typedef struct M508ScoreWork_s { /* bss_34: score displays */
    s16 grp;
    s16 spr[7];
    f32 x;
    f32 y;
    s32 count;
    s16 win;
} M508_SCOREWORK;

typedef struct M508KindWork_s { /* bss_1C: signs */
    s32 kind;
} M508_KINDWORK;

#endif
