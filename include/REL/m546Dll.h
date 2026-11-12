#ifndef _M546DLL_H
#define _M546DLL_H

#include "types.h"
#include "game/hu3d.h"
#include "game/mg/timer.h"

/* per-character input slot, 0x1C bytes; objman is not stored in front of chars[0] here */
typedef struct M546CharSlot_s {
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
} M546CharSlot;

/* one entry (0x1C bytes) of the stage model table (lbl_1_bss_254) */
typedef struct M546StageModel_s {
    s16 id;
    s16 pad_02;
    f32 x;
    f32 y;
    f32 z;
    f32 rotX;
    f32 rotY;
    f32 rotZ;
} M546StageModel;

/* per-character animation slot, 0x44 bytes */
typedef struct M546StageSlot_s {
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
    s16 shiftState;
    s16 shiftMotion;
    u32 shiftAttr;
    f32 shiftStart;
    f32 shiftEnd;
    f32 shiftCur;
    f32 shiftMax;
    u8 pad_40[4];
} M546StageSlot;

/* one 0x30 block of animation state per scripted prop (stage.c) */
typedef struct M546AnimBlock_s {
    s32 hold;
    s32 timer;
    s32 timer2;
    s32 state;
    u8 pad_10[0x10];
    f32 f20;
    f32 f24;
    f32 f28;
    u8 pad_2C[4];
} M546AnimBlock;

/* module state block (lbl_1_bss_67C, 0x250 bytes); m546.c, stage.c and game.c share it */
typedef struct M546State_s {
    u8 pad_00[4];
    M546CharSlot chars[2];
    M546StageModel *models;
    s32 *motionData[2];
    s16 *motionIds[2];
    M546StageSlot slots[2];
    s32 *dataNums;
    s16 field_DC;
    u8 pad_DE[2];
    s32 phase;
    s32 flagE4;
    s32 stepE8;
    s32 stepEC;
    s32 timerF0;
    s32 stepF4;
    u8 pad_F8[0x38];
    M546AnimBlock anim[5];
    u8 pad_220[0x30];
} M546State;

typedef struct M546Vec3_s {
    f32 x;
    f32 y;
    f32 z;
} M546Vec3;

/* camera block, 0x50 bytes (lbl_1_bss_8 points at an array of these, 0x148 bytes allocated) */
typedef struct M546Cam_s {
    s32 idx;
    M546Vec3 pos;
    f32 rotX;
    f32 rotY;
    f32 rotZ;
    M546Vec3 center;
    M546Vec3 up;
    f32 zoom;
    f32 aspect;
    f32 vpX;
    f32 vpY;
    f32 vpW;
    f32 vpH;
    f32 fov;
} M546Cam;

/* initial camera values, 0x34 bytes (lbl_1_data_178) */
typedef struct M546CamInit_s {
    M546Vec3 center;
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
} M546CamInit;

/* light block, 0x3C bytes (lbl_1_bss_4) */
typedef struct M546Light_s {
    s16 id;
    s16 pad_02;
    M546Vec3 pos;
    f32 rotX;
    f32 rotY;
    f32 rotZ;
    M546Vec3 target;
    M546Vec3 dir;
    f32 dist;
    GXColor color;
} M546Light;

/* shadow block, 0x50 bytes (lbl_1_bss_0) */
typedef struct M546Shadow_s {
    M546Vec3 pos;
    f32 rotX;
    f32 rotY;
    f32 rotZ;
    M546Vec3 target;
    u8 pad_24[0xC];
    M546Vec3 up;
    u8 pad_3C[0xC];
    f32 dist;
    f32 tpLvl;
} M546Shadow;

/* particle template / live work entry, 0x38 bytes (lbl_1_data_340) */
typedef struct M546PtclWork_s {
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
} M546PtclWork;

/* debug light, 0x40 bytes (lbl_1_bss_4) */
typedef struct M546DbgLight_s {
    s16 id;
    s16 enable;
    M546Vec3 pos;
    f32 rotX;
    f32 rotY;
    f32 rotZ;
    M546Vec3 target;
    M546Vec3 dir;
    f32 dist;
    f32 f38;
    GXColor color;
} M546DbgLight;

/* one paper scrap, 0x54 bytes (lbl_1_bss_1F0 / lbl_1_bss_4C) */
typedef struct M546Scrap_s {
    s16 active;
    s16 state;
    s16 life;
    s16 colorIdx;
    M546Vec3 pos;
    M546Vec3 vel;
    M546Vec3 rot;
    M546Vec3 rotVel;
    f32 alpha;
    M546Vec3 color;
    M546Vec3 color2;
} M546Scrap;

/* paper-scrap emitter, 0xA8 bytes (lbl_1_bss_14A10 / lbl_1_bss_14AB8) */
typedef struct M546Kami_s {
    s16 active;
    s16 pad_02;
    M546Vec3 pos;
    f32 floorY;
    s16 spawnTimer;
    s16 spawnMax;
    s16 spawnCnt;
    s16 total;
    s16 pad_1C;
    s16 model[4];
    char name[4][0x20];
    s16 hook;
} M546Kami;

/* per-player block, 0x70 bytes (lbl_1_bss_14B60) */
typedef struct M546Plr_s {
    s16 state;
    s16 sub;
    u8 pad_04[4];
    char *hook;
    s16 timer;
    s16 pad_0E;
    f32 f10;
    s16 col;
    s16 pad_16;
    M546Vec3 scale;
    M546Vec3 from;
    M546Vec3 to;
    M546Vec3 rot;
    u8 pad_48[4];
    f32 f4C;
    u8 pad_50[4];
    s16 flag54;
    s16 flag56;
    f32 f58;
    f32 f5C;
    f32 f60;
    f32 f64;
    s16 cnt68;
    s16 prob6A;
    s16 state6C;
    s16 pad_6E;
} M546Plr;

/* game-flow block (lbl_1_bss_14C40), 0x98 bytes */
typedef struct M546Game_s {
    s16 phase;
    s16 flag2;
    u8 pad_04[0x1C];
    s16 cnt20;
    s16 order[3];
    s16 diff28;
    s16 pad_2A;
    f32 fallVel;
    s32 mask30;
    f32 tp34[3];
    s16 cur;
    s16 win;
    s16 lose;
    s16 timer;
    f32 tp48;
    f32 tp4C[3];
    MGTIMER *timerId;
    s16 bgmFlag;
    s32 bgmId;
    s32 jingleId;
    s16 fx68;
    s32 fxId[7];
    s16 fxFlag[7];
} M546Game;

#endif
