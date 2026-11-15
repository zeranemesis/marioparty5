#ifndef _M532DLL_SCORE_H
#define _M532DLL_SCORE_H

#include "dolphin/types.h"
#include "game/animdata.h"

// Intrusive list node (the object base class threads every object on a list).
typedef struct M532ScoreNode_s {
    struct M532ScoreNode_s *next;
    struct M532ScoreNode_s *prev;
} M532ScoreNode;

// Linear fade: moves val from `from` to `to` in total+1 steps.
typedef struct M532ScoreFade_s {
    s32 total;
    s32 cur;
    f32 val;
    f32 from;
    f32 to;
} M532ScoreFade;

// One textured quad of a score box.
typedef struct M532ScoreQuad_s {
    ANIMDATA *tex;
    f32 x;
    f32 y;
    f32 halfW;
    f32 halfH;
    f32 scale;
    f32 alpha;
} M532ScoreQuad;

// Per-player box: a score window and three small + three big digit quads.
typedef struct M532ScoreBox_s {
    s16 win;
    M532ScoreQuad small[3];
    M532ScoreQuad big[3];
    s32 count;
    s32 maxCount;
    M532ScoreFade fade;
} M532ScoreBox;

// The score object (two boxes). Allocate with sizeof(M532Score).
typedef struct M532Score_s {
    M532ScoreNode link;
    void **vtbl;
    s32 mask;
    u8 ready;
    M532ScoreFade fade;
    ANIMDATA *anim[3];
    M532ScoreBox box[2];
} M532Score;

#ifdef __cplusplus
extern "C" {
#endif
M532Score *fn_1_CB30(M532Score *self);
void *fn_1_CDE8(M532Score *self, s16 flag);
s32 fn_1_CF64(M532Score *self, s32 round);
void fn_1_CEB4(M532Score *self, s32 idx);
s32 fn_1_CEF0(M532Score *self, s32 idx);
void fn_1_CF2C(M532Score *self);
#ifdef __cplusplus
}
#endif

#endif
