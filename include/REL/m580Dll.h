#ifndef REL_M580DLL_H
#define REL_M580DLL_H

#include "dolphin/types.h"

typedef struct M580StageModels_s M580_STAGEMODELS;
typedef struct M580StageAnimObj_s M580_STAGEANIMOBJ;
typedef struct M580StageAnim_s M580_STAGEANIM;
typedef struct M580Stage_s M580_STAGE;
typedef struct M580StageGlobal_s M580_STAGEGLOBAL;

/* The three floats of a HuVecF are contiguous in the original layout, but
   humath.h typedefs HuVecF to Vec (16 bytes), so the offsets are re-derived
   here instead of reusing HSFOBJECT. */
typedef struct M580Vec3_s {
    f32 x;
    f32 y;
    f32 z;
} M580_VEC3;

typedef struct M580HsfObject_s {
    /* 0x00 */ char *name;
    /* 0x04 */ u32 type;
    /* 0x08 */ void *constData;
    /* 0x0C */ u32 flags;
    /* 0x10 */ void *parent;
    /* 0x14 */ u32 childNum;
    /* 0x18 */ void **child;
    /* 0x1C */ M580_VEC3 basePos;
    /* 0x28 */ M580_VEC3 baseRot;
    /* 0x34 */ M580_VEC3 baseScale;
    /* 0x40 */ M580_VEC3 currPos;
    /* 0x4C */ M580_VEC3 currRot;
    /* 0x58 */ M580_VEC3 currScale;
} M580_HSFOBJECT;

struct M580StageModels_s {
    /* 0x00 */ s16 mdl0;   // DATANUM(DATA_m580, 0x00)
    /* 0x02 */ s16 mdl1;   // DATANUM(DATA_m580, 0x01)
    /* 0x04 */ s16 mdl2;   // DATANUM(DATA_m580, 0x02)
    /* 0x06 */ s16 mdl3;   // DATANUM(DATA_m580, 0x03)
    /* 0x08 */ s16 mdl4;   // DATANUM(DATA_m580, 0x04)
    /* 0x0A */ s16 mdl5;   // DATANUM(DATA_m580, 0x05)
    /* 0x0C */ s16 mdl6;   // DATANUM(DATA_m580, 0x06)
    /* 0x0E */ s16 mdl7;   // DATANUM(DATA_m580, 0x08)
    /* 0x10 */ s16 mdl8;
    /* 0x12 */ s16 mdlObj; // DATANUM(DATA_m580, 0x10)
};

struct M580StageAnimObj_s {
    /* 0x00 */ s32 state;
    /* 0x04 */ s32 frames;
    /* 0x08 */ s32 param;
    /* 0x0C */ s32 index;
    /* 0x10 */ s32 kind;
    /* 0x14 */ M580_STAGE *stage;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ u8 pad20[0x20];
};

struct M580StageAnim_s {
    /* 0x00 */ s32 mode;
    /* 0x04 */ s32 count;
    /* 0x08 */ s32 prevMode;
    /* 0x0C */ M580_STAGEANIMOBJ objs[5];
};

struct M580Stage_s {
    /* 0x00 */ u8 pad00[0x14];
    /* 0x14 */ void (*unk14)(M580_STAGE *);
    /* 0x18 */ u8 pad18[0x28];
    /* 0x40 */ M580_STAGEMODELS *mdls;
    /* 0x44 */ u8 pad44[8];
    /* 0x4C */ s32 unk4C;
    /* 0x50 */ u8 pad50[0xC];
    /* 0x5C */ M580_STAGEANIM *anim;
};

/* 8-byte module global: only the first word is known from the map. */
struct M580StageGlobal_s {
    /* 0x0 */ M580_STAGE *stage;
    /* 0x4 */ s32 unk4;
};

#endif