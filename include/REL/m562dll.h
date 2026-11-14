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
/* The event scripts work on the battle owner object itself (M562Work, defined
   in the main block below; this alias names it for the units that predate it).
   The views below are pointer-free overlays of its embedded sub-objects (cast
   from &work->member), holding only the members the scripts touch at the GC
   offsets of the original. The backdrop uses M562Back and the stage floor
   uses M562StageField. */
typedef struct M562Work M562EvtWork;

typedef struct M562EvtInput {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ s32 btnA;
    /* 0x0C */ s32 btnB;
    /* 0x10 */ s8 stickX;
    /* 0x11 */ s8 stickY;
} M562EvtInput;

typedef struct M562EvtPlayer { /* work + 0x28 */
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

typedef struct M562EvtCamera { /* work + 0x1BC, 0x258 */
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

typedef struct M562EvtBoss { /* work + 0x30C, Bowser */
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
    /* 0x7C */ u8 pad7C[0xB0 - 0x7C];
    /* 0xB0 */ struct M562StageTile *curCell;
    /* 0xB4 */ u8 padB4[4];
} M562EvtBoss;

typedef struct M562EvtFireEntry {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ Vec pos;
    /* 0x28 */ u8 pad28[0x34 - 0x28];
} M562EvtFireEntry;

typedef struct M562EvtFire { /* work + 0x7BC */
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 active;
    /* 0x14 */ u8 pad14[4];
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

typedef struct M562EvtTiles { /* work + 0x88C */
    /* 0x000 */ u8 pad0[0xC];
    /* 0x00C */ M562EvtTile tile[0x23];
    /* 0x1100 */ s16 count;
    /* 0x1102 */ u8 pad1102[0x1304 - 0x1102];
} M562EvtTiles;

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

/* ---- fire.c ---- */
#include "game/sprite.h"

typedef struct FireMgr_s FireMgr;
typedef struct Fire_s Fire;
typedef struct FireSlot_s FireSlot;

/* The boss object embedded in the battle owner at 0x28 (fields fire.c touches). */
typedef struct FireBoss_s {
    /* 0x000 */ u8 pad0[4];
    /* 0x004 */ u16 state;
    /* 0x006 */ s16 f6;
    /* 0x008 */ s16 f8;
    /* 0x00A */ s16 fA;
    /* 0x00C */ u8 padC[4];
    /* 0x010 */ s16 f10;
    /* 0x012 */ u8 pad12[2];
    /* 0x014 */ s16 player;
    /* 0x016 */ u8 pad16[0x68 - 0x16];
    /* 0x068 */ f32 height;
    /* 0x06C */ u8 pad6C[0x90 - 0x6C];
    /* 0x090 */ f32 rotY;
    /* 0x094 */ u8 pad94[0xA0 - 0x94];
    /* 0x0A0 */ Vec pos;
    /* 0x0AC */ u8 padAC[0xC4 - 0xAC];
    /* 0x0C4 */ f32 fC4;
    /* 0x0C8 */ u8 padC8[0x130 - 0xC8];
    /* 0x130 */ Fire *held;
    /* 0x134 */ void *target;
} FireBoss;

struct M562Work;
/* fire.c reaches the boss (M562Work.player) and the effect list (M562Work at 0x286C) through these views */
#define FIRE_BOSS(owner) ((FireBoss *)&(owner)->player)
#define FIRE_FX(owner) ((EffectList *)(owner)->pad286C)
#define FIRE_CAMBIT(owner) ((owner)->camera[0].camMask)

#define FIRE_MAX 35
#define FIRE_SLOT_MAX 6
#define FIRE_PARTICLE_MAX 12

/* One flame-breath emitter (a particle model plus 12 particle life counters). */
struct FireSlot_s {
    /* 0x00 */ FireMgr *mgr;
    /* 0x04 */ s16 pad4;
    /* 0x06 */ s16 timer;
    /* 0x08 */ s32 request;
    /* 0x0C */ s32 playing;
    /* 0x10 */ s32 suppress;
    /* 0x14 */ s32 run;
    /* 0x18 */ s32 enabled;
    /* 0x1C */ HU3DMODELID model;
    /* 0x1E */ s16 pad1E;
    /* 0x20 */ Vec pos;
    /* 0x2C */ f32 angle;
    /* 0x30 */ s16 life[FIRE_PARTICLE_MAX];
    /* 0x48 */ s32 pad48;
    /* 0x4C */ Fire *owner;
    /* 0x50 */ s32 sound;
};

/* One roaming fire creature. */
struct Fire_s {
    /* 0x00 */ FireMgr *mgr;
    /* 0x04 */ s16 index;
    /* 0x06 */ s16 pad6;
    /* 0x08 */ s32 run;
    /* 0x0C */ s32 shown;
    /* 0x10 */ u16 state;
    /* 0x12 */ u16 phase;
    /* 0x14 */ s16 timer;
    /* 0x16 */ s16 timer2;
    /* 0x18 */ u8 pad18[4];
    /* 0x1C */ HU3DMODELID model;
    /* 0x1E */ s16 motion[8];
    /* 0x2E */ HU3DMODELID shadow;
    /* 0x30 */ s32 aggressive;
    /* 0x34 */ f32 angle;
    /* 0x38 */ f32 targetAngle;
    /* 0x3C */ s16 curMotion;
    /* 0x3E */ s16 nextMotion;
    /* 0x40 */ s32 loopMotion;
    /* 0x44 */ Vec pos;
    /* 0x50 */ Vec rot;
    /* 0x5C */ u8 pad5C[8];
    /* 0x64 */ f32 vy;
    /* 0x68 */ FireSlot *flame;
    /* 0x6C */ s16 flameStage;
    /* 0x6E */ s16 flameTimer;
    /* 0x70 */ s16 flameWait;
    /* 0x72 */ s16 savedMotion;
    /* 0x74 */ HU3DMODELID deathModel;
    /* 0x76 */ s16 pad76;
    /* 0x78 */ s32 deathPlaying;
};

struct FireMgr_s {
    /* 0x0000 */ struct M562Work *owner;
    /* 0x0004 */ s32 run;
    /* 0x0008 */ s32 enable;
    /* 0x000C */ Fire fire[FIRE_MAX];
    /* 0x1100 */ s16 count;
    /* 0x1104 */ ANIMDATA *anim;
    /* 0x1108 */ FireSlot slot[FIRE_SLOT_MAX];
    /* 0x1300 */ HU3DMODELID deathModel;
    /* 0x1302 */ s16 deathMotion;
};

extern void fn_1_894(s32 seNo, s16 pan); /* HuAudFXPanning wrapper (m562.c) */
extern f32 fn_1_36EC(f32 target, f32 cur, f32 rate);
extern void fn_1_AD68(FireBoss *boss);

void fn_1_13CFC(FireMgr *mgr, struct M562Work *owner);
void fn_1_140E4(FireMgr *mgr);
void fn_1_14190(FireMgr *mgr);
void fn_1_14390(EffectEntry *a, EffectEntry *b);
void fn_1_14438(Fire *a, Fire *b);
void fn_1_14B68(FireBoss *boss, Fire *fire);
void fn_1_152C8(HU3DMODEL *model, HU3DPARTICLE *particle, Mtx mtx);
FireSlot *fn_1_15308(FireMgr *mgr, Fire *fire);
void fn_1_15390(void *unused, FireSlot *slot);
void fn_1_153A4(FireSlot *slot, FireMgr *mgr);
void fn_1_154FC(FireSlot *slot);
void fn_1_15554(FireSlot *slot);
void fn_1_15B78(FireSlot *slot);
void fn_1_15D40(Fire *fire, s16 index, FireMgr *mgr);
void fn_1_16218(Fire *fire);
void fn_1_162A0(Fire *fire);
void fn_1_16350(Fire *fire);
void fn_1_16AC0(Fire *fire);
void fn_1_16CF4(Fire *fire);
void fn_1_16D08(Fire *fire);
void fn_1_170E0(Fire *fire);
s32 fn_1_174C0(Fire *fire);
s32 fn_1_1776C(Fire *fire);
extern u32 lbl_1_data_998[8];
extern f32 lbl_1_data_9B8[6];
extern GXColor lbl_1_data_9D0;


/* ---- koopa.c / koopaboss.c ---- */
/* The koopa units' views of the battle work object ("host"); only members they
 * touch are named, at the GC offsets of the original. The pointer-bearing
 * sub-objects (Koopa, KoopaBoss) are embedded by type so their sizeof is right. */
typedef struct KpHost KpHost;

typedef struct KpPlayer { /* host + 0x28 */
    /* 0x00 */ KpHost *host;
    /* 0x04 */ u16 state;
    /* 0x06 */ s16 sub6;
    /* 0x08 */ s16 sub8;
    /* 0x0A */ s16 subA;
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ s16 invuln;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s16 charIdx;
    /* 0x16 */ u8 pad16[2];
    /* 0x18 */ s32 charNo;
    /* 0x1C */ u8 pad1C[0x68 - 0x1C];
    /* 0x68 */ f32 height;
    /* 0x6C */ u8 pad6C[0x8C - 0x6C];
    /* 0x8C */ s16 hp;
    /* 0x8E */ u8 pad8E[2];
    /* 0x90 */ f32 rotY;
    /* 0x94 */ u8 pad94[0xA0 - 0x94];
    /* 0xA0 */ Vec pos;
    /* 0xAC */ u8 padAC[0xC0 - 0xAC];
    /* 0xC0 */ f32 speed;
    /* 0xC4 */ u8 padC4[0x130 - 0xC4];
    /* 0x130 */ void *held;
    /* 0x134 */ void *target; /* the bomb slot being carried */
} KpPlayer;

typedef struct KpCamera { /* host + 0x1BC, 0x258 */
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ u32 camBit;
    /* 0x08 */ u8 pad8[0x9C - 8];
} KpCamera;

/* one floor tile of the stage table (0x48 bytes on GC) */
typedef struct KpCell {
    /* 0x00 */ void *field;
    /* 0x04 */ u16 state;
    /* 0x06 */ u8 pad6[0xA];
    /* 0x10 */ Vec pos;
    /* 0x1C */ u8 pad1C[0x34 - 0x1C];
    /* 0x34 */ s32 flag;
    /* 0x38 */ u8 pad38[0x48 - 0x38];
} KpCell;

typedef struct KpCells { /* host + 0x227C */
    /* 0x000 */ u8 pad0[8];
    /* 0x008 */ KpCell cell[20];
    /* 0x5A8 */ u8 pad5A8[0x5C8 - 0x5A8];
    /* 0x5C8 */ s16 quakeTimer;
    /* 0x5CA */ u8 pad5CA[2];
    /* 0x5CC */ Vec quakePos;
    /* 0x5D8 */ u8 pad5D8[4];
} KpCells;

/* mini mecha-koopa object returned by fn_1_181FC (only its position is read) */
typedef struct KbMini {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ Vec pos;
} KbMini;

typedef struct KbMiniMgr KbMiniMgr; /* embedded at host + 0x1B90, owned by mini_mechakoopa */

struct KbBombMgr;
struct KoopaBoss;

/* one slot of the bomb manager at KoopaBoss + 0x14C (0x78 bytes on GC);
 * the sunbomb unit owns the full layout */
typedef struct KbBombSlot {
    /* 0x00 */ struct KbBombMgr *mgr;
    /* 0x04 */ u16 state;
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ s16 unk8;
    /* 0x0A */ u8 padA[2];
    /* 0x0C */ s16 phase;
    /* 0x0E */ u8 padE[2];
    /* 0x10 */ s32 active;
    /* 0x14 */ u8 pad14[4];
    /* 0x18 */ s32 mode;
    /* 0x1C */ f32 radius;
    /* 0x20 */ s32 flag;
    /* 0x24 */ u8 pad24[0x38 - 0x24];
    /* 0x38 */ Vec pos;
    /* 0x44 */ Vec vel;
    /* 0x50 */ u8 pad50[0x5C - 0x50];
    /* 0x5C */ Vec spawn;
    /* 0x68 */ u8 pad68[0x78 - 0x68];
} KbBombSlot;

typedef struct KbBombMgr {
    /* 0x000 */ struct KoopaBoss *boss;
    /* 0x004 */ KbBombSlot slot[5];
    /* 0x25C */ s16 count;
    /* 0x25E */ u8 pad25E[0x294 - 0x25E];
    /* 0x294 */ s32 attract;
    /* 0x298 */ Vec attractPos;
    /* 0x2A4 */ f32 attractRange;
} KbBombMgr;

/* the Bowser actor (0xB8 bytes on GC) at host + 0x30C */
typedef struct Koopa {
    /* 0x00 */ KpHost *host;
    /* 0x04 */ u16 state;
    /* 0x06 */ u16 sub;
    /* 0x08 */ u16 step;
    /* 0x0A */ s16 timerA;
    /* 0x0C */ s16 timerC;
    /* 0x0E */ s16 timerE;
    /* 0x10 */ s16 timer10;
    /* 0x12 */ s16 pad12;
    /* 0x14 */ s32 visibleReq;
    /* 0x18 */ s32 visible;
    /* 0x1C */ s32 active;
    /* 0x20 */ s16 modelId;
    /* 0x22 */ s16 motionId[14];
    /* 0x3E */ u8 pad3E[2];
    /* 0x40 */ f32 rotY;
    /* 0x44 */ f32 goalRotY;
    /* 0x48 */ s16 curMotion;
    /* 0x4A */ s16 reqMotion;
    /* 0x4C */ s32 loopMotion;
    /* 0x50 */ s16 pendingDelay;
    /* 0x52 */ s16 shiftHold;
    /* 0x54 */ Vec pos;
    /* 0x60 */ Vec rot;
    /* 0x6C */ u8 pad6C[0x74 - 0x6C];
    /* 0x74 */ f32 velY;
    /* 0x78 */ f32 speed;
    /* 0x7C */ f32 baseSpeed;
    /* 0x80 */ Vec dir;
    /* 0x8C */ Vec target;
    /* 0x98 */ Vec cellPos;
    /* 0xA4 */ Vec jumpFrom;
    /* 0xB0 */ KpCell *cell;
    /* 0xB4 */ f32 prevTime;
} Koopa;

/* the fire-breathing boss (0x3F8 bytes on GC) at host + 0x3C4 */
typedef struct KoopaBoss {
    /* 0x000 */ KpHost *host;
    /* 0x004 */ u16 state;
    /* 0x006 */ u16 sub;
    /* 0x008 */ s16 timer;
    /* 0x00A */ s16 timerA;
    /* 0x00C */ u16 savedState;
    /* 0x00E */ s16 interval;
    /* 0x010 */ s32 visibleReq;
    /* 0x014 */ s32 visible;
    /* 0x018 */ s16 life;
    /* 0x01A */ u8 pad1A[2];
    /* 0x01C */ s32 active;
    /* 0x020 */ s16 modelId;
    /* 0x022 */ s16 motionId[19];
    /* 0x048 */ u8 pad48[0x50 - 0x48];
    /* 0x050 */ s16 curMotion;
    /* 0x052 */ s16 reqMotion;
    /* 0x054 */ s32 loopMotion;
    /* 0x058 */ Vec pos;
    /* 0x064 */ Vec rot;
    /* 0x070 */ u8 pad70[0x78 - 0x70];
    /* 0x078 */ f32 angle;
    /* 0x07C */ f32 goalAngle;
    /* 0x080 */ Mtx handR;
    /* 0x0B0 */ Mtx handL;
    /* 0x0E0 */ Mtx head;
    /* 0x110 */ s32 handRValid;
    /* 0x114 */ s32 handLValid;
    /* 0x118 */ s32 headValid;
    /* 0x11C */ s16 orbModel;
    /* 0x11E */ s16 orbMotion;
    /* 0x120 */ s16 ringModel;
    /* 0x122 */ s16 ringMotion;
    /* 0x124 */ s16 orbState;
    /* 0x126 */ s16 orbTimer;
    /* 0x128 */ f32 orbScale;
    /* 0x12C */ s32 orbRequest;
    /* 0x130 */ s16 shadowModel;
    /* 0x132 */ s16 chargeModel;
    /* 0x134 */ s16 chargeMotion;
    /* 0x136 */ s16 glowModel;
    /* 0x138 */ s16 glowMotion;
    /* 0x13A */ s16 hitModel;
    /* 0x13C */ s16 hitMotion;
    /* 0x13E */ u8 pad13E[2];
    /* 0x140 */ s32 hitPlaying;
    /* 0x144 */ s16 lightId;
    /* 0x146 */ u8 pad146[2];
    /* 0x148 */ f32 lightLevel;
    /* 0x14C */ KbBombMgr bombs;
    /* 0x3F4 */ s32 sndHandle;
} KoopaBoss;

struct KpHost {
    /* 0x000 */ u8 pad0[0xC];
    /* 0x00C */ u16 phase;
    /* 0x00E */ u8 padE[0x20 - 0xE];
    /* 0x020 */ s16 difficulty;
    /* 0x022 */ s16 flag22;
    /* 0x024 */ u8 pad24[0x28 - 0x24];
    /* 0x028 */ KpPlayer player;
    /* 0x160 */ u8 pad160[0x1BC - 0x160];
    /* 0x1BC */ KpCamera cam;
    /* 0x258 */ KpCamera cam2;
    /* 0x2F4 */ u8 pad2F4[0x30C - 0x2F4];
    /* 0x30C */ Koopa koopa;
    /* 0x3C4 */ KoopaBoss boss;
    /* 0x7BC */ u8 pad7BC[0x88C - 0x7BC];
    /* 0x88C */ u8 tiles[0x1100];
    /* 0x198C */ s16 tileCount;
    /* 0x198E */ u8 pad198E[0x1B90 - 0x198E];
    /* 0x1B90 */ u8 mini[0x227C - 0x1B90];
    /* 0x227C */ KpCells cells;
    /* 0x2854 */ u8 pad2854[0x287C - 0x2854];
    /* 0x287C */ EffectList hitA;
    /* 0x288C */ EffectList hitB;
};

extern void fn_1_B2E0(Koopa *self, KpHost *host);
extern void fn_1_B4F8(Koopa *self);
extern void fn_1_CE10(Koopa *self);
extern void fn_1_ED90(Koopa *self, Vec *in, Vec *out);
extern void fn_1_EEC4(EffectEntry *a, EffectEntry *b);
extern void fn_1_F014(KpPlayer *player, Koopa *koopa);
extern void fn_1_F784(KoopaBoss *self, KpHost *host);
extern void fn_1_10018(KoopaBoss *self);
extern Mtx *fn_1_1010C(KoopaBoss *self);
extern Mtx *fn_1_10160(KoopaBoss *self);
extern Mtx *fn_1_101B4(KoopaBoss *self);
extern void fn_1_10208(KoopaBoss *self);
extern void fn_1_10348(EffectEntry *a, EffectEntry *b);
extern void fn_1_11054(KoopaBoss *self);
extern s32 fn_1_934(s32 seNo, Vec *pos);
extern void fn_1_964(s32 handle, Vec *pos);
extern KbMini *fn_1_181FC(KbMiniMgr *mgr, Vec *pos, f32 angle);
extern void fn_1_1EE3C(KbBombMgr *mgr, KoopaBoss *boss);
extern void fn_1_1F138(KbBombMgr *mgr);
extern void fn_1_1F208(KbBombMgr *mgr);
extern KbBombSlot *fn_1_1F298(KbBombMgr *mgr, Vec *pos);

/* ---- evt_koopagrow / evt_ending ---- */
/* The ending scripts' view of the boss-battle work object (same object as
   M562EvtWork, 0x28B4 bytes on GC): only the members these two scripts touch
   are named, at the original's offsets; everything else is padding. Sub-objects
   are embedded and reached by name, never through raw byte offsets. */
typedef struct M562FinInput {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ s32 btnA;
    /* 0x0C */ s32 btnB;
    /* 0x10 */ s8 stickX;
    /* 0x11 */ s8 stickY;
} M562FinInput;

typedef struct M562FinPlayer { /* 0x28 */
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ u8 pad8[4];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 pad10[4];
    /* 0x14 */ s16 charIdx;
    /* 0x16 */ u8 pad16[0x6C - 0x16];
    /* 0x6C */ M562FinInput input;
    /* 0x7E */ u8 pad7E[0x90 - 0x7E];
    /* 0x90 */ f32 rotGoal;
    /* 0x94 */ f32 rotSpeed;
    /* 0x98 */ u8 pad98[2];
    /* 0x9A */ s16 animNo;
    /* 0x9C */ s32 animLoop;
    /* 0xA0 */ Vec pos;
    /* 0xAC */ u8 padAC[4];
    /* 0xB0 */ f32 rotY;
    /* 0xB4 */ u8 padB4[0x10];
    /* 0xC4 */ f32 velY;
    /* 0xC8 */ u8 padC8[0x138 - 0xC8];
} M562FinPlayer;

typedef struct M562FinStage { /* 0x160 */
    /* 0x00 */ u8 pad0[0x12];
    /* 0x12 */ s16 model12;
    /* 0x14 */ s16 model14;
    /* 0x16 */ u8 pad16[0x24 - 0x16];
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ u8 pad2C[0x40 - 0x2C];
    /* 0x40 */ s16 motion40;
    /* 0x42 */ u8 pad42[0x5C - 0x42];
} M562FinStage;

typedef struct M562FinCamera { /* 0x1BC */
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s32 camMask;
    /* 0x08 */ u8 pad8[2];
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
} M562FinCamera;

typedef struct M562FinKoopa { /* 0x3C4 */
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s16 unk4;
    /* 0x06 */ u8 pad6[0x1A];
    /* 0x20 */ s16 model;
    /* 0x22 */ u8 pad22[0x50 - 0x22];
    /* 0x50 */ s16 unk50;
    /* 0x52 */ s16 unk52;
    /* 0x54 */ s32 unk54;
    /* 0x58 */ Vec pos;
    /* 0x64 */ Vec rot;
    /* 0x70 */ u8 pad70[0x132 - 0x70];
    /* 0x132 */ s16 model2;
    /* 0x134 */ u8 pad134[0x3F8 - 0x134];
} M562FinKoopa;

typedef struct M562FinCells { /* 0x227C */
    /* 0x000 */ u8 pad0[0x5B0];
    /* 0x5B0 */ s16 model5B0;
    /* 0x5B2 */ u8 pad5B2[2];
    /* 0x5B4 */ s16 model5B4;
    /* 0x5B6 */ u8 pad5B6[0x5DC - 0x5B6];
} M562FinCells;

typedef struct M562FinWork {
    /* 0x0000 */ u8 pad0[0x10];
    /* 0x0010 */ u16 state;
    /* 0x0012 */ u8 pad12[0xA];
    /* 0x001C */ s32 timer;
    /* 0x0020 */ u8 pad20[8];
    /* 0x0028 */ M562FinPlayer player;
    /* 0x0160 */ M562FinStage stage;
    /* 0x01BC */ M562FinCamera camera;
    /* 0x0258 */ u8 pad258[0x2F4 - 0x258];
    /* 0x02F4 */ u8 light[0x18];
    /* 0x030C */ u8 pad30C[0x3C4 - 0x30C];
    /* 0x03C4 */ M562FinKoopa koopa;
    /* 0x07BC */ u8 pad7BC[0x1B94 - 0x7BC];
    /* 0x1B94 */ s32 unk1B94;
    /* 0x1B98 */ u8 pad1B98[0x227C - 0x1B98];
    /* 0x227C */ M562FinCells cells;
    /* 0x2858 */ u8 pad2858[8];
    /* 0x2860 */ s16 model2860;
    /* 0x2862 */ u8 pad2862[0x28A4 - 0x2862];
    /* 0x28A4 */ s16 winId;
    /* 0x28A6 */ u8 pad28A6[2];
    /* 0x28A8 */ s32 sndA;
    /* 0x28AC */ s32 sndB;
    /* 0x28B0 */ s32 sndC;
} M562FinWork;

extern s32 fn_1_280C8(M562EvtWork *w);
extern s32 fn_1_A44(s16 bgmNo);
extern void fn_1_9BC(s32 handle, u8 vol, s32 time);
extern s32 fn_1_29DF8(M562EvtWork *w);


/* ---- m562 / main / back ---- */
#include "game/mg/timer.h"
#include "game/gamemes.h"
#include "game/window.h"
#include "game/sprite.h"
#include "game/mg/seqman.h"
/* The battle owner object (0x28B4 bytes on GC) is allocated by m562.c and set up in main.c. Its pointer members
   are real pointers, so on PC its layout is not the GC byte layout; the sub-objects other units own are embedded
   as views holding only the members this module touches. */
typedef struct M562Work M562Work;

/* the backdrop (0x5C bytes on GC): models + motions around the arena, water reflection copy pass */
typedef struct M562Back {
    /* 0x00 */ M562Work *owner;
    /* 0x04 */ u8 pad4[8];
    /* 0x0C */ HU3DMODELID modelC;
    /* 0x0E */ HU3DMODELID modelE;
    /* 0x10 */ HU3DMODELID model10;
    /* 0x12 */ HU3DMODELID model12;
    /* 0x14 */ HU3DMODELID model14;
    /* 0x16 */ HU3DMOTID motion16;
    /* 0x18 */ HU3DMOTID motion18;
    /* 0x1A */ HU3DMODELID model1A;
    /* 0x1C */ HU3DMODELID model1C;
    /* 0x1E */ HU3DMOTID motion1E;
    /* 0x20 */ HU3DMODELID model20;
    /* 0x22 */ HU3DMOTID motion22;
    /* 0x24 */ s32 reflectOn;
    /* 0x28 */ s32 rippleOn;
    /* 0x2C */ s32 bobTimer;
    /* 0x30 */ s32 bobOn;
    /* 0x34 */ Vec bobPos;
    /* 0x40 */ HU3DMODELID model40;
    /* 0x42 */ HU3DMOTID motion42;
    /* 0x44 */ HU3DMODELID model44;
    /* 0x46 */ HU3DMODELID model46;
    /* 0x48 */ HU3DMOTID motion48;
    /* 0x4A */ HU3DMOTID motion4A;
    /* 0x4C */ s16 phase44;
    /* 0x4E */ s16 phase46;
    /* 0x50 */ s16 wait44;
    /* 0x52 */ s16 wait46;
    /* 0x54 */ f32 scale;
    /* 0x58 */ s16 waitMax;
    /* 0x5A */ u8 pad5A[2];
} M562Back;

/* the model/motion ids main.c tears down and zeroes (0xC bytes on GC) */
typedef struct M562MainTriple {
    /* 0x0 */ M562Work *owner;
    /* 0x4 */ HU3DMODELID model;
    /* 0x6 */ HU3DMOTID motion;
    /* 0x8 */ HU3DMODELID model2;
    /* 0xA */ u8 padA[2];
} M562MainTriple;

typedef struct M562MainPlayer { /* 0x28, 0x138 bytes */
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s16 mode;
    /* 0x06 */ u8 pad6[0x88 - 6];
    /* 0x88 */ s32 hitFlag;
    /* 0x8C */ s16 life;
    /* 0x8E */ u8 pad8E[0x138 - 0x8E];
} M562MainPlayer;

typedef struct M562MainCamera { /* 0x1BC, 0x258: 0x9C bytes */
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ u16 camMask;
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ s16 modeA;
    /* 0x0A */ s16 modeB;
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ Vec pos;
    /* 0x1C */ Vec target;
    /* 0x28 */ u8 pad28[0xC];
    /* 0x34 */ Vec posGoal;
    /* 0x40 */ Vec targetGoal;
    /* 0x4C */ u8 pad4C[0x9C - 0x4C];
} M562MainCamera;

typedef struct M562MainFireEntry { /* 0x34 bytes */
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ f32 value;
    /* 0x24 */ u8 pad24[0x34 - 0x24];
} M562MainFireEntry;

typedef struct M562MainFireRing { /* 0x7BC, 0xD0 bytes */
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ u16 state;
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ s16 timer;
    /* 0x0A */ u8 padA[6];
    /* 0x10 */ s32 active;
    /* 0x14 */ s32 moveOn;
    /* 0x18 */ M562MainFireEntry entry[3];
    /* 0xB4 */ u8 padB4[0xCC - 0xB4];
    /* 0xCC */ s32 sndHandle;
} M562MainFireRing;

typedef struct M562MainMecha { /* 0x1B90, 0x68C bytes */
    /* 0x000 */ u8 pad0[4];
    /* 0x004 */ s32 active;
    /* 0x008 */ u8 pad8[0x68C - 8];
} M562MainMecha;

typedef struct M562MainSprite { /* 0x221C, 0x224C: 0x30 bytes */
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s32 active;
    /* 0x08 */ u8 pad8[0x2C - 8];
    /* 0x2C */ s16 life;
    /* 0x2E */ u8 pad2E[2];
} M562MainSprite;

struct M562Work {
    /* 0x0000 */ OMOBJMAN *manager;
    /* 0x0004 */ u8 pad4[4];
    /* 0x0008 */ s32 frame;
    /* 0x000C */ u16 step;
    /* 0x000E */ u16 mode;
    /* 0x0010 */ u16 state;
    /* 0x0012 */ u16 phase;
    /* 0x0014 */ s32 timeLeft;
    /* 0x0018 */ u8 pad18[4];
    /* 0x001C */ s32 evtTimer;
    /* 0x0020 */ s16 diff;
    /* 0x0022 */ s16 winner;
    /* 0x0024 */ MGTIMER *timer;
    /* 0x0028 */ M562MainPlayer player;
    /* 0x0160 */ M562Back back;
    /* 0x01BC */ M562MainCamera camera[2];
    /* 0x02F4 */ LIGHT light;
    /* 0x030C */ Koopa koopa;
    /* 0x03C4 */ KoopaBoss boss;
    /* 0x07BC */ M562MainFireRing fireRing;
    /* 0x088C */ FireMgr fire;
    /* 0x1B90 */ M562MainMecha mecha;
    /* 0x221C */ M562MainSprite sprite[2];
    /* 0x227C */ M562StageField stage;
    /* 0x2858 */ M562MainTriple triple;
    /* 0x2864 */ void *fbTex;
    /* 0x2868 */ ANIMDATA *fbAnim;
    /* 0x286C */ u8 pad286C[0x289C - 0x286C];
    /* 0x289C */ s32 bossMode;
    /* 0x28A0 */ GAMEMESID mesId;
    /* 0x28A4 */ HUWINID winId;
    /* 0x28A6 */ u8 pad28A6[2];
    /* 0x28A8 */ s32 bgmId;
    /* 0x28AC */ s32 seId;
    /* 0x28B0 */ s32 seId2;
};

extern M562Work *lbl_1_bss_0;
extern OMOBJMAN *lbl_1_bss_4;
extern s32 lbl_1_data_0; /* own flag; the game-wide m562CtrlNo is in game/mgdata.h */
extern s32 lbl_1_data_4;

extern void fn_1_AC4(M562Work *work, OMOBJMAN *manager);
extern void fn_1_FB0(M562Work *work);
extern void fn_1_1CB0(M562Work *work, s16 mode, s32 on);
extern s32 fn_1_1F54(M562Work *work);
extern s32 fn_1_2034(M562Work *work);
extern void fn_1_2FF4(Vec *a, Vec *b, GXColor *color);
extern void fn_1_30CC(Vec *center, GXColor *color, f32 size);
extern void fn_1_323C(Vec *a, Vec *b, Vec *c, Vec *d, GXColor *color);
extern void fn_1_337C(Quaternion *q, Vec *from, Vec *to);
extern void fn_1_3808(M562Back *back, M562Work *work);
extern void fn_1_41B4(M562Back *back);
extern void fn_1_427C(M562Back *back);
extern void fn_1_508C(s16 layerNo);

/* ---- camera ---- */
/* Bowser, as far as the camera follows him: his position. */
typedef struct CAMKOOPA {
    /* 0x00 */ u8 pad00[0xA0];
    /* 0xA0 */ Vec pos;
} CAMKOOPA;

/* The stage object the camera belongs to; Bowser sits at 0x28. */
typedef struct CAMSTAGE {
    /* 0x00 */ u8 pad00[0x28];
    /* 0x28 */ CAMKOOPA koopa;
} CAMSTAGE;

/* 0x9C bytes. */
typedef struct CAMERA {
    /* 0x00 */ CAMSTAGE *owner;
    /* 0x04 */ s32 camId;
    /* 0x08 */ u16 state;
    /* 0x0A */ u16 mode;
    /* 0x0C */ u16 prevMode;
    /* 0x0E */ u8 pad0E[2];
    /* 0x10 */ Vec posGoal;
    /* 0x1C */ Vec tgtGoal;
    /* 0x28 */ Vec dirGoal;
    /* 0x34 */ Vec pos;
    /* 0x40 */ Vec tgt;
    /* 0x4C */ Vec dir;
    /* 0x58 */ Vec shakePos;
    /* 0x64 */ Vec shakeTgt;
    /* 0x70 */ Vec up;
    /* 0x7C */ f32 speed;
    /* 0x80 */ f32 slerpT;
    /* 0x84 */ u16 shakeMode;
    /* 0x86 */ u8 pad86[2];
    /* 0x88 */ s32 shakeFrames;
    /* 0x8C */ f32 shakeAmp;
    /* 0x90 */ f32 shakeDecay;
    /* 0x94 */ f32 shakeStep;
    /* 0x98 */ f32 shakePhase;
} CAMERA;

void fn_1_20CE0(CAMERA *cam, s32 camId, s32 flag, CAMSTAGE *owner);
void fn_1_20EF4(CAMERA *cam, s32 camId);
void fn_1_2102C(CAMERA *cam);
void fn_1_2105C(CAMERA *cam);
void fn_1_210F4(CAMERA *cam);
void fn_1_21280(CAMERA *cam);
void fn_1_21344(CAMERA *cam);
void fn_1_21700(CAMERA *cam);
void fn_1_2191C(CAMERA *cam);
void fn_1_21B68(CAMERA *cam, s16 mode, s32 frames, f32 amp, f32 decay, f32 step);
void fn_1_21BA8(CAMERA *cam);

/* kill the owner's countdown timer (inlined in the original at several places) */
static inline void M562TimerKill(M562Work *work)
{
    if (work->timer != NULL) {
        MgTimerKill(work->timer);
        if (work->timer->stopF == 0) {
            work->timer->stopF = TRUE;
            work->timer->mode = MGTIMER_MODE_ON;
        }
        work->timer = NULL;
    }
}

/* m562.c: thin audio wrappers */
extern void fn_1_8C4(Vec *pos, Vec *heading, f32 sndDist, f32 sndSpeed);
extern void fn_1_904(Vec *pos, Vec *heading);
extern s32 fn_1_994(s16 streamId);
extern void fn_1_A24(void);
extern s32 fn_1_A94(s16 streamId, s16 chanNo);

/* ---- evt_opening / evt_fire / evt_fight / evt_fall / evt_winner (entry points and callees owned elsewhere) ---- */
extern s32 fn_1_2446C(M562Work *work);
extern s32 fn_1_252B4(M562Work *work);
extern s32 fn_1_260B8(M562Work *work);
extern s32 fn_1_27950(M562Work *work);
extern s32 fn_1_2C118(M562Work *work);
extern void fn_1_1D9C(M562Work *work);
extern void fn_1_7D58(void *player, f32 speed);
extern s32 fn_1_AD98(void *player, Vec *dir, f32 speed, f32 rate);
extern void fn_1_B578(void *koopa);
extern void fn_1_864(s32 handle, s32 time);
extern void fn_1_9F4(s32 handle, s32 time);
extern void fn_1_A6C(s16 jingle);

#endif
