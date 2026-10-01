#ifndef _M575DLL_H
#define _M575DLL_H

#include "types.h"

typedef struct M575Score_s {
    s32 unk0;
    u16 side;
    s16 winId;
    struct MgScore_s *score;
    s16 espA;
    s16 espB;
    s16 value;
} M575Score;

void fn_1_3A7C(M575Score *self, u16 side, s32 arg2);
void fn_1_3CD8(M575Score *self);
void fn_1_3D28(M575Score *self);
void fn_1_3D5C(M575Score *self, s32 disp);

#endif
