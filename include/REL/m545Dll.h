#ifndef _M545DLL_H
#define _M545DLL_H

#include "types.h"
#include "game/hu3d.h"

/* per-character input slot, 0x1C bytes; objman is not stored in front of chars[0] here */
typedef struct M545CharSlot_s {
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
} M545CharSlot;

/* one entry (0x1C bytes) of the stage model table (lbl_1_bss_254) */
typedef struct M545StageModel_s {
    s16 id;
    s16 pad_02;
    f32 x;
    f32 y;
    f32 z;
    f32 rotX;
    f32 rotY;
    f32 rotZ;
} M545StageModel;

/* per-character animation slot, 0x44 bytes */
typedef struct M545StageSlot_s {
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
} M545StageSlot;

/* one 0x30 block of animation state per scripted prop (stage.c) */
typedef struct M545AnimBlock_s {
    s32 hold;
    s32 timer;
    s32 timer2;
    s32 state;
    u8 pad_10[0x10];
    f32 f20;
    f32 f24;
    f32 f28;
    u8 pad_2C[4];
} M545AnimBlock;

/* module state block (lbl_1_bss_67C, 0x250 bytes); m545.c, stage.c and game.c share it */
typedef struct M545State_s {
    u8 pad_00[4];
    M545CharSlot chars[2];
    M545StageModel *models;
    s32 *motionData[2];
    s16 *motionIds[2];
    M545StageSlot slots[2];
    s32 *dataNums;
    s16 field_DC;
    u8 pad_DE[2];
    s32 phase;
    s32 flagE4;
    s32 stepE8;
    u8 pad_EC[4];
    s32 timerF0;
    s32 stepF4;
    u8 pad_F8[0x38];
    M545AnimBlock anim[5];
    u8 pad_220[0x30];
} M545State;

typedef struct M545Vec3_s {
    f32 x;
    f32 y;
    f32 z;
} M545Vec3;

/* camera block, 0x50 bytes (lbl_1_bss_8 points at an array of these, 0x148 bytes allocated) */
typedef struct M545Cam_s {
    s32 idx;
    M545Vec3 pos;
    f32 rotX;
    f32 rotY;
    f32 rotZ;
    M545Vec3 center;
    M545Vec3 up;
    f32 zoom;
    f32 aspect;
    f32 vpX;
    f32 vpY;
    f32 vpW;
    f32 vpH;
    f32 fov;
} M545Cam;

/* initial camera values, 0x34 bytes (lbl_1_data_178) */
typedef struct M545CamInit_s {
    M545Vec3 center;
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
} M545CamInit;

/* light block, 0x3C bytes (lbl_1_bss_4) */
typedef struct M545Light_s {
    s16 id;
    s16 pad_02;
    M545Vec3 pos;
    f32 rotX;
    f32 rotY;
    f32 rotZ;
    M545Vec3 target;
    M545Vec3 dir;
    f32 dist;
    GXColor color;
} M545Light;

/* shadow block, 0x50 bytes (lbl_1_bss_0) */
typedef struct M545Shadow_s {
    M545Vec3 pos;
    f32 rotX;
    f32 rotY;
    f32 rotZ;
    M545Vec3 target;
    u8 pad_24[0xC];
    M545Vec3 up;
    u8 pad_3C[0xC];
    f32 dist;
    f32 tpLvl;
} M545Shadow;

/* particle template / live work entry, 0x38 bytes (lbl_1_data_268, particle work buffer) */
typedef struct M545PtclWork_s {
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
} M545PtclWork;

#endif
