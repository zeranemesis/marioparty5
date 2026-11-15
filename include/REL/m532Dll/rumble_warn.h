#ifndef _M532DLL_RUMBLE_WARN_H
#define _M532DLL_RUMBLE_WARN_H

// rumble_warn.cpp: the "rumble" warning sprite, an unk object drawn as a
// billboarded quad that grows in, holds and fades out.
#include "REL/m532Dll/unk.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct M532RumbleWarn_s {
    M532UnkObj base;     // 0x00
    M532Ptmf state;      // 0x14: current state function (returns 0 when finished)
    ANIMDATA *anim0;     // 0x20
    ANIMDATA *anim1;     // 0x24
    f32 sizeX;           // 0x28
    f32 sizeY;           // 0x2C
    Vec pos;             // 0x30
    f32 scale;           // 0x3C
    f32 alpha;           // 0x40
    s32 timer;           // 0x44
} M532RumbleWarn;

M532RumbleWarn *fn_1_8130(M532RumbleWarn *self, Vec *pos);

#ifdef __cplusplus
}
#endif

#endif
