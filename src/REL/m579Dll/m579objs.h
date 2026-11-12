#ifndef M579OBJS_H
#define M579OBJS_H
/* Object types shared between the m579 units (needs dolphin.h and game/sprite.h from the caller). */
#include "REL/m579Dll/m579types.h"

typedef struct MGSCORE MGSCORE;

typedef struct M579Actor {
    M579Node node;
    Vec pos;
} M579Actor;

typedef struct M579Camera {
    M579Base base;
    M579Cam cam;
    s32 frame;
    M579Ptmf update;
    s16 model;
    M579Actor *target;
    u8 debugOn;
    u8 debugHold;
    M579Cam debugCam;
} M579Camera;

typedef struct M579EffMgr {
    M579Base base;
} M579EffMgr;

typedef struct M579FishMgr {
    M579Base base;
} M579FishMgr;

typedef struct M579ScoreEntry {
    s16 win;
    s16 grp;
    MGSCORE *score;
    f32 tpLvl;
    s32 type;
    s32 value;
    f32 x;
    f32 y;
} M579ScoreEntry;

typedef struct M579Score {
    void *prev;
    void *next;
    void **vtable;
    s32 flags;
    s32 active;
    M579ScoreEntry entry[4];
} M579Score;

/* Water singleton: spray/ripple sprites and the particle managers. */
typedef struct M579WaterMgr {
    M579Base base;
    ANIMDATA *anim[3];
} M579WaterMgr;

/* Byte vector holding the water reflection texture. */
typedef struct M579U8Vec {
    u32 cap;
    u32 size;
    u8 *data;
} M579U8Vec;

typedef struct M579WaterEff {
    M579Base base;
    s16 model;
    GXTexObj reflTex;
    GXTexObj waveTex;
    M579U8Vec buf;
    void *waveData;
    ANIMDATA *anim;
    Mtx mtx[4];
    f32 scrollX;
    f32 scrollY;
} M579WaterEff;

/* Small background fish. */
typedef struct M579Zako {
    M579Node node;
    s32 u08;
    M579Ptmf update;
    s32 anim;
    s32 u1C;
    Vec pos;
    s32 frame;
    f32 phase;
    s16 model;
    f32 yaw;
    f32 speed;
    f32 target;
    u8 type;
    s32 pad48;
    s32 timer;
    f32 roll;
} M579Zako;

/* Item (treasure) manager. */
typedef struct M579ItemMgr {
    M579Base base;
    M579S16Vec motion;
    M579Ptmf update;
    s32 anim;
    s16 model[3];
    u8 landed;
    u8 hidden;
    s32 frame;
    Vec vel;
    Vec pos;
    f32 f50;
    f32 f54;
    f32 f58;
    f32 f5C;
    s16 hook;
    u8 u62;
    f32 f64;
} M579ItemMgr;

/* Fisher player (one per controller). */
struct M579Player {
    M579Node node;
    Vec pos;
    M579Ptmf update;
    s32 idx;
    s32 charNo;
    s32 padNo;
    s32 anim;
    s32 dif;
    f32 yaw;
    f32 f38;
    f32 yawTarget;
    f32 stick;
    s32 state;
    f32 dist;
    s16 model;
    u8 loser;
    s32 frame;
    M579Fish *fish;
    M579Fish *target;
    s32 count;
    s32 score;
    u8 u64;
    s32 timer;
    s32 active;
    s32 u70;
    M579S16Vec motion;
};

typedef struct M579StageModel {
    s16 field0;
    s16 field2;
} M579StageModel;

typedef struct M579Stage {
    M579Base base;
    M579StageModel model;
} M579Stage;

#endif
