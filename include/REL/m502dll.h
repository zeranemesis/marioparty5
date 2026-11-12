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
