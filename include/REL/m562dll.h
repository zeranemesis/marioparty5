#ifndef _M562DLL_H
#define _M562DLL_H

#include "dolphin.h"
#include "game/hu3d.h"
#include "game/data.h"
#include "game/object.h"
#include "game/audio.h"
#include "game/memory.h"

/* Shared declarations for the m562dll units (Bowser boss battle).
 * Append-only: each unit adds its own exported types/functions/data in a block
 * headed by a comment with the unit name. Never reformat other blocks. */

/* ---- light ---- */
/* Three floats copied as a unit ' the target moves them word by word. */
typedef struct lightVec_s {
    f32 x;
    f32 y;
    f32 z;
} lightVec;

/* The lights' view of the stage object: only the model ids it broadcasts the
   ambient colour to, at the original's offsets (padding is the rest of the
   stage, which other units own). The LIGHT block lives inside the stage. */
typedef struct light_s LIGHT;

typedef struct LightStage_s {
    /* 0x000 */ u8 pad0[0x4C];
    /* 0x04C */ s16 ambModelA;
    /* 0x04E */ u8 pad4E[0x16C - 0x4E];
    /* 0x16C */ s16 bodyModel[2];
    /* 0x170 */ s16 bodyModelC;
    /* 0x172 */ u8 pad172[0x17A - 0x172];
    /* 0x17A */ s16 bodyModelD[2];
    /* 0x17E */ u8 pad17E[0x180 - 0x17E];
    /* 0x180 */ s16 bodyModelE;
    /* 0x182 */ u8 pad182[0x2F4 - 0x182];
    /* 0x2F4 */ u8 lightBlock[0x18];
    /* 0x30C */ u8 pad30C[4];
    /* 0x310 */ u16 kind;
    /* 0x312 */ u8 pad312[0x32C - 0x312];
    /* 0x32C */ s16 kindModel;
    /* 0x32E */ u8 pad32E[0x3E4 - 0x32E];
    /* 0x3E4 */ s16 ambModelB;
    /* 0x3E6 */ u8 pad3E6[0x2284 - 0x3E6];
    /* 0x2284 */ struct { u8 pad0[8]; s16 model; u8 pad0A[0x3E]; } wheel[20];
    /* 0x2824 */ s16 wheelModel[5];
    /* 0x282E */ u8 pad282E[0x2832 - 0x282E];
    /* 0x2832 */ s16 wheelModelA;
    /* 0x2834 */ u8 pad2834[0x283C - 0x2834];
    /* 0x283C */ s16 wheelModelB;
} LightStage;

/* 0x18 bytes: an owner word, the ambient light id, the spot light id. */
struct light_s {
    /* 0x00 */ LightStage *owner;
    /* 0x04 */ HU3DLIGHTID ambientId;
    /* 0x06 */ HU3DLIGHTID spotId;
    /* 0x08 */ s32 spotIsLocal;
    /* 0x0C */ lightVec amb;
};

void fn_1_23CB8(LightStage *self, const lightVec *col);

void fn_1_2253C(LIGHT *self, LightStage *stage);
void fn_1_22620(LIGHT *self);
void fn_1_22690(void);
void fn_1_22694(LIGHT *self, s16 mode, f32 t);
void fn_1_23CB8(LightStage *self, const lightVec *col);

#endif
