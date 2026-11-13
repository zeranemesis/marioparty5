#ifndef _M549DLL_H
#define _M549DLL_H

#include "game/object.h"
#include "game/process.h"
#include "game/memory.h"
#include "game/hu3d.h"
#include "game/data.h"
#include "game/wipe.h"
#include "game/gamemes.h"
#include "game/gamework.h"
#include "game/charman.h"
#include "game/pad.h"
#include "game/audio.h"
#include "game/frand.h"
#include "game/sprite.h"
#include "game/flag.h"
#include "game/mg/actman.h"
#include "game/mg/seqman.h"
#include "game/mg/timer.h"

#include "humath.h"

#include "stddef.h"

/* Shared declarations for m549.c / game.c / cloud.c / stage.c go below.
 * Rules: add with small Edits, re-read the file first, never delete another agent's entries. */

/* the original takes (f32)sqrt() (libc/math.h double sqrt, 4 Newton steps) of the squared XZ length */
#define M549_Mag2D(x, z) ((f32)sqrt(HuMag2Point2D(x, z)))

/* ---- game.c shared types (G4 started these; other parts: replace pad_XX with real fields via small Edits) ---- */

/* lbl_1_bss_0, 0xC bytes on GC */
typedef struct M549Globals_s {
    s16 unk0;
    MGPLAYER *player[2];
} M549Globals;

/* per-character game state (lbl_1_bss_3B0[2]), 0xE8 bytes on GC */
typedef struct M549Char_s {
    s16 mode;       /* 0x0 */
    s16 sub;        /* 0x2 */
    u8 pad_04[2];
    s16 s06;        /* 0x6 */
    s16 s08;        /* 0x8 */
    s16 s0A;        /* 0xA */
    s16 state;      /* 0xC */
    s16 state2;     /* 0xE (was isCom) */
    s16 state3;     /* 0x10 */
    s16 s12;        /* 0x12 */
    s16 s14;        /* 0x14 */
    u8 pad_16[2];
    f32 velX;       /* 0x18 */
    f32 velZ;       /* 0x1C */
    f32 scale;      /* 0x20 */
    char *hook;     /* 0x24, model hook object name */
    s16 s28;        /* 0x28 */
    HuVecF pos;     /* 0x2C */
    HuVecF prevPos; /* 0x38 */
    f32 f44;        /* 0x44 */
    f32 f48;
    f32 f4C;        /* 0x4C */
    f32 f50;
    f32 f54;
    f32 f58;
    s16 s5C;        /* 0x5C */
    s16 s60;        /* 0x60 */
    s16 s62;        /* 0x62 */
    f32 f64;        /* 0x64 */
    s16 s68;        /* 0x68 */
    s16 s6A;        /* 0x6A */
    s16 s6C;        /* 0x6C */
    s16 s6E;        /* 0x6E */
    s16 s70;        /* 0x70 */
    u8 pad_72[6];
    s16 s78;        /* 0x78 */
    s16 s7A;        /* 0x7A */
    s16 s7C;        /* 0x7C */
    s16 s7E;
    s16 s80;        /* 0x80 */
    s16 s82;        /* 0x82 */
    s16 s84;        /* 0x84 */
    s16 s86;
    HuVecF target;  /* 0x88 */
    HuVecF targetSave; /* 0x94 */
    f32 chaseRadius; /* 0xA0 */
    f32 chaseSpeed;  /* 0xA4 */
    s16 sA8;        /* 0xA8 */
    s16 sAA;        /* 0xAA */
    s16 sAC;        /* 0xAC */
    s16 sAE;        /* 0xAE */
    s16 sB0;        /* 0xB0 */
    s16 sB2;        /* 0xB2 */
    f32 fB4;        /* 0xB4 */
    f32 fB8;        /* 0xB8 */
    f32 fBC;        /* 0xBC */
    f32 fC0;        /* 0xC0 */
    f32 fC4;        /* 0xC4 */
    s16 sC8;        /* 0xC8 */
    s16 sCA;        /* 0xCA */
    s16 sCC;        /* 0xCC */
    s16 sCE;        /* 0xCE */
    s16 sD0;        /* 0xD0 */
    s16 sD2;        /* 0xD2 */
    s16 sD4;        /* 0xD4 */
    s16 sD6;
    u32 btnA;       /* 0xD8 */
    u32 btnB;       /* 0xDC */
    u32 btnC;       /* 0xE0 */
    u32 btnD;       /* 0xE4 */
} M549Char;

/* object table entry (lbl_1_bss_158[10]), 0x3C bytes on GC */
typedef struct M549Obj_s {
    s16 active;     /* 0x0 */
    s16 s02;
    s16 s04;
    s16 s06;
    s16 side;       /* 0x8 */
    s16 s0A;
    f32 angle;      /* 0xC */
    s16 level;      /* 0x10 */
    s16 s12;
    f32 radius;     /* 0x14 */
    f32 size;       /* 0x18 */
    f32 f1C;
    f32 f20;
    HuVecF pos;     /* 0x24 */
    HuVecF dir;     /* 0x30 */
} M549Obj;

/* impact effect table entry (lbl_1_bss_18[10]), 0x20 bytes on GC */
typedef struct M549Impact_s {
    s16 active;     /* 0x0 */
    s16 s02;
    s16 s04;
    s16 s06;
    s16 s08;
    s16 type;       /* 0xA */
    f32 fx0C;
    f32 fx10;
    HuVecF pos;     /* 0x14 */
} M549Impact;

/* game control block (lbl_1_bss_580), 0x80 bytes on GC */
typedef struct M549Ctrl_s {
    s16 mode;       /* 0x0 */
    s16 s02;
    s16 s04;
    s16 timer;      /* 0x6 */
    f32 f08;        /* 0x8 */
    u8 pad_0C[0x14];
    s16 s20;        /* 0x20 */
    s16 s22;        /* 0x22 */
    s16 s24;        /* 0x24 */
    u8 pad_26[2];
    MGTIMER *mgTimer; /* 0x28 */
    u8 pad_2C[4];
    s32 bgmHandle;  /* 0x30 */
    s32 jingleHandle; /* 0x34 */
    u8 pad_38[4];
    s32 sfxHandle[11]; /* 0x3C */
    s16 sfxFlag[11];   /* 0x68 */
    u8 pad_7E[2];
} M549Ctrl;

/* camera block, 0x50 bytes (lbl_1_bss_14 points at an array of these) */
typedef struct M549Cam_s {
    s32 idx;
    HuVecF pos;
    f32 rotX;
    f32 rotY;
    f32 rotZ;
    HuVecF center;
    HuVecF up;
    f32 zoom;       /* 0x34 */
    f32 aspect;
    f32 vpX;
    f32 vpY;
    f32 vpW;
    f32 vpH;
    f32 fov;
} M549Cam;

/* initial camera values, 0x34 bytes (lbl_1_data_288[4]) */
typedef struct M549CamInit_s {
    HuVecF center;
    f32 rotX;
    f32 rotY;
    f32 rotZ;
    f32 zoom;
    f32 aspect;
    f32 vpX;
    f32 vpY;
    f32 vpW;
    f32 vpH;
    f32 fov;
} M549CamInit;

extern M549Cam *lbl_1_bss_14;
extern M549CamInit lbl_1_data_288[4];

/* functions of m549.c: see the m549.c block below (authoritative, owner part M) */

/* game.c part G2 */
s32 fn_1_762C(s16 player);
void fn_1_7CC0(s16 player);
void fn_1_8570(void);
void fn_1_9324(HuVecF *pos, s16 type);
void fn_1_98F0(void);
s32 fn_1_99E8(s16 player);
void fn_1_9DBC(void);
s32 fn_1_9FF4(s16 player, s16 frames);
s32 fn_1_A18C(void);

/* ---- m549.c shared types / externs (owner: part M) ---- */

typedef struct M549Vec3_s {
    f32 x;
    f32 y;
    f32 z;
} M549Vec3;

/* light block, 0x3C bytes (lbl_1_bss_10) */
typedef struct M549Light_s {
    s16 id;
    s16 pad_02;
    M549Vec3 pos;
    f32 rotX;
    f32 rotY;
    f32 rotZ;
    M549Vec3 target;
    M549Vec3 dir;
    f32 dist;
    GXColor color;
} M549Light;

/* shadow block, 0x50 bytes (lbl_1_bss_C) */
typedef struct M549Shadow_s {
    M549Vec3 pos;
    f32 rotX;
    f32 rotY;
    f32 rotZ;
    M549Vec3 target;
    u8 pad_24[0xC];
    M549Vec3 up;
    u8 pad_3C[0xC];
    f32 dist;
    f32 tpLvl;
} M549Shadow;

/* particle template / live work entry, 0x38 bytes (lbl_1_data_3F0) */
typedef struct M549PtclWork_s {
    s16 unk0;
    s16 pad_02;
    u8 colStart[4];
    u8 colEnd[4];
    f32 velX;
    f32 velY;
    f32 velZ;
    f32 mulX;
    f32 mulY;
    f32 mulZ;
    f32 accelY;
    f32 pad_28;
    f32 scaleStep;
    f32 alphaStep;
    f32 colorStep;
} M549PtclWork;

extern M549Shadow *lbl_1_bss_C;
extern M549Light *lbl_1_bss_10;
extern M549Cam *lbl_1_bss_14;
extern OMOBJMAN *lbl_1_bss_15D4;
extern MGSEQ_PARAM lbl_1_data_0;
extern s32 lbl_1_data_28[13][10];
extern s32 lbl_1_data_230[22];
extern M549CamInit lbl_1_data_288[4];
extern s16 lbl_1_data_358[16];
extern M549PtclWork lbl_1_data_3F0;
extern s16 lbl_1_data_428[12];

/* game.c part G3 */
s32 fn_1_A898(void);
int fn_1_AD78(COL_NARROW_PARAM *a, COL_NARROW_PARAM *b);
void fn_1_ADC8(MGACTOR *actor, int param);
void fn_1_ADDC(void);
void fn_1_BF34(s16 player);
s32 fn_1_C360(s16 player);
s32 fn_1_C418(s16 player);
void fn_1_C64C(s16 player);
void fn_1_CA38(s16 player, s16 objNo);
s32 fn_1_CDD8(s16 player);

/* game.c part G4 */
s32 fn_1_D0C4(s16 player, s16 limit);
s32 fn_1_D9E0(s16 player);
s32 fn_1_E160(s16 player, s16 objNo);
void fn_1_E880(s16 player);
s32 fn_1_E90C(s16 player, s16 objNo);
void fn_1_F0C8(s16 player);
s32 fn_1_F4A0(s16 player, s16 objNo);
s32 fn_1_FDB0(s16 player);

extern M549Impact lbl_1_bss_18[10];
extern M549Ctrl lbl_1_bss_580;
extern M549Globals lbl_1_bss_0;
extern M549Obj lbl_1_bss_158[10];
extern M549Char lbl_1_bss_3B0[2];

/* ---- stage.c (owner: part D) ---- */

/* per-character input slot, 0x1C bytes */
typedef struct M549CharSlot_s {
    s16 playerNo;
    s16 charNo;
    s16 cpuLevel;
    s16 padNo;
    s32 btn;
    s32 btnDown;
    s8 stkX;
    u16 btnRep;
    s8 stkY;
    s8 subStkX;
    s8 subStkY;
    u8 trigL;
    u8 trigR;
    u8 dstk;
    u8 dstkRep;
} M549CharSlot;

/* one entry (0x1C bytes) of the stage model table */
typedef struct M549StageModel_s {
    s16 id;
    s16 pad_02;
    f32 x;
    f32 y;
    f32 z;
    f32 rotX;
    f32 rotY;
    f32 rotZ;
} M549StageModel;

/* per-character animation slot, 0x54 bytes */
typedef struct M549StageSlot_s {
    s16 motion;
    s16 id;
    f32 speed;
    f32 curSpeed;
    f32 time;
    f32 curTime;
    f32 maxTime;
    u32 attrSet;
    u32 attrCur;
    u32 attrReset;
    s32 endFlag;
    s16 endReq;
    s16 retMotion;
    f32 retTime;
    u32 retAttrSet;
    u32 retAttrReset;
    s16 shiftState;
    s16 shiftMotion;
    u32 shiftAttr;
    f32 shiftStart;
    f32 shiftEnd;
    f32 shiftCur;
    f32 shiftMax;
    u8 pad_50[4];
} M549StageSlot;

/* one 0x4C block of animation state per scripted prop */
typedef struct M549AnimBlock_s {
    s32 init;
    s32 t[3];
    s32 s10;
    s32 s14;
    s32 state;
    s32 s1C;
    u8 pad_20[0xC];
    f32 f2C;
    f32 f30;
    f32 f34;
    f32 f38;
    f32 f3C;
    f32 f40;
    f32 f44;
    u8 pad_48[4];
} M549AnimBlock;

/* stage state (lbl_1_bss_1608), 0x804 bytes on GC */
typedef struct M549State_s {
    u8 pad_00[4];
    M549CharSlot chars[3];
    M549StageModel *models;
    s32 *motionData[2];
    u8 pad_64[4];
    s16 *motionIds[2];
    u8 pad_70[4];
    M549StageSlot slots[2];
    u8 pad_11C[0x54];
    s32 *dataNums;
    u8 pad_174[4];
    s32 doneFlag;   /* 0x178 */
    u8 pad_17C[0x4C];
    M549AnimBlock anim[21];
} M549State;

extern M549State *lbl_1_bss_1608;

/* game.c part G1 */
void fn_1_3B74(M549State *state);
s32 fn_1_4EE0(M549State *state);
s32 fn_1_4FFC(void);
s32 fn_1_50A4(void);
s32 fn_1_5184(void);
s32 fn_1_569C(void);
s32 fn_1_5788(void);
s32 fn_1_57D8(void);
s32 fn_1_5A00(void);
s32 fn_1_5A40(void);
void fn_1_5A98(void);
void fn_1_5CD4(void);
void fn_1_115A8(M549State *state); /* cloud.c */
void MgActorPosGet(MGACTOR *actorP, HuVecF *pos);

/* ---- m549.c functions (owner: part M) ---- */
void ObjectSetup(void);
void fn_1_320(s16 mode, s16 frameNo);
void fn_1_354(s16 mode, s16 frameNo);
void fn_1_380(s16 mode, s16 frameNo);
void fn_1_3AC(s16 mode, s16 frameNo);
void fn_1_3D8(s16 mode, s16 frameNo);
void fn_1_404(s16 mode, s16 frameNo);
void fn_1_430(s16 mode, s16 frameNo);
void fn_1_45C(s16 mode, s16 frameNo);
void fn_1_47C(s16 mode, s16 frameNo);
void fn_1_4A8(OMOBJ *obj);
void fn_1_4AC(s16 camIdx);
void fn_1_97C(s16 camIdx);
void fn_1_EF4(void);
void fn_1_1124(void);
void fn_1_13FC(void);
void fn_1_1524(void);
s32 fn_1_18B0(f32 *value, f32 steps, f32 start, f32 target);
s32 fn_1_19D0(f32 *value, f32 steps, f32 start, f32 target);
MGTIMER *fn_1_1AEC(s32 type, s32 maxSec, s32 endSec, f32 x, f32 y);
void fn_1_1B60(MGTIMER *timer);
BOOL fn_1_1B8C(MGTIMER *timer);
void fn_1_1BB4(MGTIMER *timer);
s32 fn_1_1BEC(M549State *state, s16 pad, s16 idx);
void fn_1_1F40(void);
void fn_1_1F44(s16 pad);
void fn_1_23FC(void);
void fn_1_2550(M549Vec3 *pos);
void fn_1_28CC(M549Vec3 *pos, s16 mode);
s16 fn_1_2F70(M549PtclWork *work, f32 x, f32 y, f32 z, f32 scale);
void fn_1_3144(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);
void fn_1_357C(void);
void fn_1_35AC(void);
void fn_1_35DC(s16 isBgm, s16 speed);
void fn_1_3644(s16 isBgm);
void fn_1_36A4(s16 id);
void fn_1_3754(s16 id, s16 vol, s16 pan);
void fn_1_3814(s16 id, s16 pan);
void fn_1_38A4(s16 id, s16 pitch);
void fn_1_3934(s16 id, s16 vol);
void fn_1_39C4(s16 id);
void fn_1_3A8C(s16 id);
void fn_1_3B54(void);

void fn_1_11BC0(M549State *stage, M549StageModel *models, s32 *dataNums);
void fn_1_11E0C(void);
void fn_1_12450(s16 idx, s32 *data);
void fn_1_12470(s16 idx, s16 *ids);
void fn_1_12490(void);
void fn_1_12494(void);
void fn_1_128AC(void);
void fn_1_12A60(void);
void fn_1_12E18(s32 modelId, f32 alpha);
void fn_1_12E80(void);
void fn_1_12ED4(void);
void fn_1_1328C(void);
void fn_1_134AC(void);
void fn_1_134C4(void);
void fn_1_139F4(void);
void fn_1_1448C(void);
void fn_1_144D8(void);
void fn_1_14720(void);
void fn_1_1476C(void);

#endif
