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

/* ---- effect.c ---- */
typedef struct EffectEntry {
    s16 type;
    u32 flag;       /* 0 = movable, nonzero = fixed */
    void *user;
    Vec pos;
    f32 radius;
} EffectEntry;

typedef void (*EffectHitFunc)(EffectEntry *a, EffectEntry *b);

typedef struct EffectList {
    u16 count;
    EffectEntry *entries;
    EffectHitFunc hitFunc;
    s16 max;
} EffectList;

extern void fn_1_2CD6C(EffectList *list, s16 max);
extern void fn_1_2CDE8(EffectList *list);
extern void fn_1_2CE3C(EffectList *list, EffectHitFunc func);
extern void fn_1_2CE44(EffectList *list);
extern void fn_1_2CE8C(EffectList *list, s16 type, u32 flag, Vec *pos, void *user, f32 radius);
extern void fn_1_2CEF4(EffectList *list, EffectList *arr, u16 axis, u16 a, u16 b);
extern void fn_1_2DB24(void);
extern void fn_1_2DB74(void);
extern void fn_1_2DC08(s16 camMask, Vec *pos, s16 kind);
extern s16 fn_1_2E600(s16 camMask, void *param, f32 x, f32 y, f32 z, f32 scale);
extern void fn_1_2E8A4(void);
extern void fn_1_2EB08(void);

/* ---- evt_opening / evt_fire / evt_fight / evt_fall / evt_winner ---- */
/* The event units' view of the boss-battle work object (0x28B4 bytes on GC).
   Only the members the event scripts touch are named; padding is the rest of
   the work object, which other units own. Sub-objects are embedded at the
   original offsets and are not accessed through raw byte offsets. */
typedef struct M562EvtInput {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ s16 btnA;
    /* 0x0A */ u8 pad0A[2];
    /* 0x0C */ s16 btnB;
    /* 0x0E */ u8 pad0E[2];
    /* 0x10 */ s8 stickX;
    /* 0x11 */ s8 stickY;
} M562EvtInput;

typedef struct M562EvtPlayer { /* 0x28 */
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ u8 pad8[0xC];
    /* 0x14 */ s16 charIdx;
    /* 0x16 */ u8 pad16[2];
    /* 0x18 */ s16 charNo;
    /* 0x1A */ u8 pad1A[0xA];
    /* 0x24 */ s16 motion;
    /* 0x26 */ u8 pad26[0x46];
    /* 0x6C */ M562EvtInput input;
    /* 0x7E */ u8 pad7E[0x12];
    /* 0x90 */ f32 rotGoal;
    /* 0x94 */ f32 rotSpeed;
    /* 0x98 */ u8 pad98[2];
    /* 0x9A */ s16 animNo;
    /* 0x9C */ s16 animLoop;
    /* 0x9E */ u8 pad9E[2];
    /* 0xA0 */ Vec pos;
    /* 0xAC */ u8 padAC[4];
    /* 0xB0 */ f32 rotY;
    /* 0xB4 */ u8 padB4[0x10];
    /* 0xC4 */ f32 velY;
    /* 0xC8 */ u8 padC8[0x138 - 0xC8];
} M562EvtPlayer;

typedef struct M562EvtStage { /* 0x160 */
    /* 0x00 */ u8 pad0[0x1A];
    /* 0x1A */ s16 model1A;
    /* 0x1C */ s16 model1C;
    /* 0x1E */ u8 pad1E[2];
    /* 0x20 */ s16 model20;
    /* 0x22 */ u8 pad22[2];
    /* 0x24 */ s16 unk24;
    /* 0x26 */ u8 pad26[2];
    /* 0x28 */ s16 unk28;
    /* 0x2A */ u8 pad2A[6];
    /* 0x30 */ s32 unk30;
    /* 0x34 */ u8 pad34[4];
    /* 0x38 */ f32 unk38;
    /* 0x3C */ u8 pad3C[4];
    /* 0x40 */ s16 motion40;
    /* 0x42 */ u8 pad42[0x5C - 0x42];
} M562EvtStage;

typedef struct M562EvtCamera { /* 0x1BC, 0x258 */
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ u16 camMask;
    /* 0x06 */ u8 pad6[4];
    /* 0x0A */ s16 unkA;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ u8 padE[2];
    /* 0x10 */ Vec pos;
    /* 0x1C */ Vec target;
    /* 0x28 */ u8 pad28[0xC];
    /* 0x34 */ Vec posGoal;
    /* 0x40 */ Vec targetGoal;
    /* 0x4C */ u8 pad4C[0x30];
    /* 0x7C */ f32 posSpeed;
    /* 0x80 */ f32 targetSpeed;
    /* 0x84 */ u8 pad84[0x9C - 0x84];
} M562EvtCamera;

typedef struct M562EvtBoss { /* 0x30C */
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ u8 pad8[0xC];
    /* 0x14 */ s32 unk14;
    /* 0x18 */ u8 pad18[4];
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s16 model;
    /* 0x22 */ s16 motionTbl[0x13];
    /* 0x48 */ s16 unk48;
    /* 0x4A */ s16 motionNo;
    /* 0x4C */ s16 motionLoop;
    /* 0x4E */ u8 pad4E[2];
    /* 0x50 */ s16 unk50;
    /* 0x52 */ s16 unk52;
    /* 0x54 */ Vec pos;
    /* 0x60 */ u8 pad60[4];
    /* 0x64 */ f32 rotCopy;
    /* 0x68 */ u8 pad68[0x78 - 0x68];
    /* 0x78 */ f32 speed;
    /* 0x7C */ u8 pad7C[0xB8 - 0x7C];
} M562EvtBoss;

typedef struct M562EvtFireEntry {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ Vec pos;
    /* 0x28 */ u8 pad28[0x34 - 0x28];
} M562EvtFireEntry;

typedef struct M562EvtFire { /* 0x7BC */
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s16 unk10;
    /* 0x12 */ u8 pad12[6];
    /* 0x18 */ M562EvtFireEntry entry[3];
    /* 0xB4 */ u8 padB4[0xCC - 0xB4];
    /* 0xCC */ s32 sndHandle;
} M562EvtFire;

typedef struct M562EvtTile { /* 0x7C */
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 state;
    /* 0x12 */ u8 pad12[0xA];
    /* 0x1C */ s16 model;
    /* 0x1E */ u8 pad1E[0x10];
    /* 0x2E */ s16 model2;
    /* 0x30 */ u8 pad30[0x14];
    /* 0x44 */ f32 x;
    /* 0x48 */ u8 pad48[4];
    /* 0x4C */ f32 z;
    /* 0x50 */ u8 pad50[0x7C - 0x50];
} M562EvtTile;

typedef struct M562EvtTiles { /* 0x88C */
    /* 0x000 */ u8 pad0[0xC];
    /* 0x00C */ M562EvtTile tile[0x23];
    /* 0x1100 */ s32 count;
    /* 0x1104 */ u8 pad1104[0x1B90 - 0x88C - 0x1104];
} M562EvtTiles;

typedef struct M562EvtCell { /* 0x48 */
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s16 state;
    /* 0x06 */ u8 pad6[0xA];
    /* 0x10 */ Vec pos;
    /* 0x1C */ u8 pad1C[0x18];
    /* 0x34 */ s32 flag;
    /* 0x38 */ u8 pad38[0x10];
} M562EvtCell;

typedef struct M562EvtCells { /* 0x227C */
    /* 0x000 */ u8 pad0[8];
    /* 0x008 */ M562EvtCell cell[20];
    /* 0x5A8 */ u8 pad5A8[6];
    /* 0x5AE */ s16 lightModel;
    /* 0x5B0 */ u8 pad5B0[0x18];
    /* 0x5C8 */ s32 hitFlag;
    /* 0x5CC */ Vec hitPos;
    /* 0x5D8 */ u8 pad5D8[4];
} M562EvtCells;

typedef struct M562EvtModelTriple { /* 0x2858 */
    /* 0x0 */ u8 pad0[4];
    /* 0x4 */ s16 model;
    /* 0x6 */ s16 motion;
    /* 0x8 */ s16 model2;
    /* 0xA */ u8 padA[2];
} M562EvtModelTriple;

typedef struct M562EvtWork {
    /* 0x0000 */ u8 pad0[0xE];
    /* 0x000E */ u16 mode;
    /* 0x0010 */ u16 state;
    /* 0x0012 */ u16 phase;
    /* 0x0014 */ u8 pad14[8];
    /* 0x001C */ s32 timer;
    /* 0x0020 */ u8 pad20[2];
    /* 0x0022 */ s16 winner;
    /* 0x0024 */ u8 pad24[4];
    /* 0x0028 */ M562EvtPlayer player;
    /* 0x0160 */ M562EvtStage stage;
    /* 0x01BC */ M562EvtCamera camera;
    /* 0x0258 */ M562EvtCamera camera2;
    /* 0x02F4 */ u8 light[0x18];
    /* 0x030C */ M562EvtBoss boss;
    /* 0x03C4 */ u8 pad3C4[0x7BC - 0x3C4];
    /* 0x07BC */ M562EvtFire fire;
    /* 0x088C */ M562EvtTiles tiles;
    /* 0x1B90 */ u8 pad1B90[0x2220 - 0x1B90];
    /* 0x2220 */ s32 flag2220;
    /* 0x2224 */ u8 pad2224[0x227C - 0x2224];
    /* 0x227C */ M562EvtCells cells;
    /* 0x2858 */ M562EvtModelTriple modelTriple;
    /* 0x2864 */ u8 pad2864[0x28A4 - 0x2864];
    /* 0x28A4 */ s16 winId;
    /* 0x28A6 */ u8 pad28A6[2];
    /* 0x28A8 */ s32 sndA;
    /* 0x28AC */ s32 sndB;
    /* 0x28B0 */ s32 sndC;
} M562EvtWork;

extern s32 fn_1_2446C(M562EvtWork *w);
extern s32 fn_1_252B4(M562EvtWork *w);
extern s32 fn_1_260B8(M562EvtWork *w);
extern s32 fn_1_27950(M562EvtWork *w);
extern s32 fn_1_2C118(M562EvtWork *w);

/* ---- stage.c ---- */
/* The stage unit's view of the main work (owned by main.c): the fields it
   reads, at the original's offsets. */
typedef struct M562StageCam {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ u32 cameraBit;
} M562StageCam;

typedef struct M562StageAmb {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ f32 r;
    /* 0x10 */ f32 g;
    /* 0x14 */ f32 b;
} M562StageAmb;

typedef struct M562StageParent {
    /* 0x000 */ u8 pad0[0xE];
    /* 0x00E */ u16 phase;
    /* 0x010 */ u8 pad10[0x1BC - 0x10];
    /* 0x1BC */ M562StageCam cam;
    /* 0x1C4 */ u8 pad1C4[0x2F4 - 0x1C4];
    /* 0x2F4 */ M562StageAmb amb;
} M562StageParent;

struct M562StageField;

/* one floor tile (0x48 bytes on GC) */
typedef struct M562StageTile {
    /* 0x00 */ struct M562StageField *field;
    /* 0x04 */ u16 state;
    /* 0x06 */ u16 pad6;
    /* 0x08 */ s16 modelId;
    /* 0x0A */ s16 padA;
    /* 0x0C */ s16 x;
    /* 0x0E */ s16 z;
    /* 0x10 */ Vec pos;
    /* 0x1C */ Vec rot;
    /* 0x28 */ Vec rotVel;
    /* 0x34 */ s32 flag;
    /* 0x38 */ f32 vy;
    /* 0x3C */ Vec vel;
} M562StageTile;

/* the stage floor (0x5DC bytes on GC) */
typedef struct M562StageField {
    /* 0x000 */ M562StageParent *parent;
    /* 0x004 */ u16 state;
    /* 0x006 */ u16 pad6;
    /* 0x008 */ M562StageTile tile[20];
    /* 0x5A8 */ s16 modelA;
    /* 0x5AA */ s16 modelB;
    /* 0x5AC */ s16 modelC;
    /* 0x5AE */ s16 modelD;
    /* 0x5B0 */ s16 modelE;
    /* 0x5B2 */ s16 motionE;
    /* 0x5B4 */ s16 modelF;
    /* 0x5B6 */ s16 modelG;
    /* 0x5B8 */ s16 motionG;
    /* 0x5BA */ s16 pad5BA;
    /* 0x5BC */ s32 gActive;
    /* 0x5C0 */ s16 modelH;
    /* 0x5C2 */ s16 motionH;
    /* 0x5C4 */ s32 hActive;
    /* 0x5C8 */ s16 quakeTimer;
    /* 0x5CA */ s16 pad5CA;
    /* 0x5CC */ Vec quakePos;
    /* 0x5D8 */ s32 soundId;
} M562StageField;

extern s32 fn_1_814(s32 seId);
extern void fn_1_83C(s32 seNo);
extern f32 lbl_1_data_C8[16];
extern Vec lbl_1_data_108;
void fn_1_5688(M562StageField *field, M562StageParent *parent);
void fn_1_5BE4(M562StageField *field);
void fn_1_5CAC(M562StageField *field);
void fn_1_60F8(M562StageTile *tile, s16 x, s16 z, M562StageField *field);
void fn_1_62A8(M562StageTile *tile);
void fn_1_62E8(M562StageTile *tile);

#endif
