#ifndef _M543DLL_H
#define _M543DLL_H

#include "game/object.h"
#include "game/audio.h"
#include "game/hu3d.h"
#include "game/mg/timer.h"
#include "game/mg/seqman.h"
#include "game/mg/actman.h"

#include "datanum/m543.h"

typedef struct M543Vec3_s {
    f32 x;
    f32 y;
    f32 z;
} M543Vec3;

/* per-character input slot, 0x1C bytes */
typedef struct M543CharSlot_s {
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
} M543CharSlot;

/* one entry (0x1C bytes) of the stage model table (lbl_1_bss_C24) */
typedef struct M543StageModel_s {
    s16 id;
    s16 pad_02;
    f32 x;
    f32 y;
    f32 z;
    f32 rotX;
    f32 rotY;
    f32 rotZ;
} M543StageModel;

/* per-character animation slot, 0x54 bytes */
typedef struct M543StageSlot_s {
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
    s16 endMotion;
    f32 endTime;
    u32 endAttrSet;
    u32 endAttrReset;
    s16 shiftState;
    s16 shiftMotion;
    u32 shiftAttr;
    f32 shiftStart;
    f32 shiftEnd;
    f32 shiftCur;
    f32 shiftMax;
    u8 pad_50[4];
} M543StageSlot;

/* module state block (lbl_1_bss_12EC, 0x2E8 bytes on GC) */
typedef struct M543State_s {
    u8 pad_00[4];
    M543CharSlot chars[2];
    u8 pad_3C[0x1C];
    M543StageModel *models;
    s32 *motionData[2];
    u8 pad_64[4];
    s16 *motionIds[2];
    u8 pad_70[4];
    M543StageSlot slots[2];
    u8 pad_11C[0x54];
    s32 *dataNums;
    s16 field_174;
    u8 pad_176[2];
    s32 field_178;
    u8 pad_17C[0x16C];
} M543State;

/* camera block, 0x50 bytes (lbl_1_bss_18 points at an array of these, 0x148 bytes allocated) */
typedef struct M543Cam_s {
    s32 idx;
    M543Vec3 pos;
    f32 rotX;
    f32 rotY;
    f32 rotZ;
    M543Vec3 center;
    M543Vec3 up;
    f32 zoom;
    f32 aspect;
    f32 vpX;
    f32 vpY;
    f32 vpW;
    f32 vpH;
    f32 fov;
} M543Cam;

/* initial camera values, 0x34 bytes (lbl_1_data_1D8) */
typedef struct M543CamInit_s {
    M543Vec3 center;
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
} M543CamInit;

/* light block, 0x3C bytes (lbl_1_bss_14) */
typedef struct M543Light_s {
    s16 id;
    s16 pad_02;
    M543Vec3 pos;
    f32 rotX;
    f32 rotY;
    f32 rotZ;
    M543Vec3 target;
    M543Vec3 dir;
    f32 dist;
    GXColor color;
} M543Light;

/* shadow block, 0x50 bytes (lbl_1_bss_10) */
typedef struct M543Shadow_s {
    M543Vec3 pos;
    f32 rotX;
    f32 rotY;
    f32 rotZ;
    M543Vec3 target;
    u8 pad_24[0xC];
    M543Vec3 up;
    u8 pad_3C[0xC];
    f32 dist;
    f32 tpLvl;
} M543Shadow;

/* particle template / live work entry, 0x38 bytes (lbl_1_data_340) */
typedef struct M543PtclWork_s {
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
} M543PtclWork;

/* player actors (lbl_1_bss_0, 0x10 bytes on GC) */
typedef struct M543Mg_s {
    s16 colMdl[3];
    s16 pad_06;
    MGPLAYER *player[2];
} M543Mg;

/* per-player saved material colours (lbl_1_bss_8DC, 0x15C bytes on GC) */
typedef struct M543Actor_s {
    u8 pad_00[0xC];
    f32 colorScale;
    u8 *matColors;
    u8 pad_14[0x148];
} M543Actor;

/* game state block (lbl_1_bss_B94, 0x74 bytes on GC) */
typedef struct M543Game_s {
    u8 pad_00[0x50];
    s32 bgmStream;
    s32 jingleStream;
    s32 pad_58;
    s32 fxId[4];
    s16 fxOn[4];
} M543Game;

/* board tile (0x20 bytes, lbl_1_bss_33C) and path node (0x14 bytes, lbl_1_bss_1C) */
typedef struct M543Tile_s {
    u8 d[0x20];
} M543Tile;

typedef struct M543Node_s {
    u8 d[0x14];
} M543Node;

void fn_1_3FC(void);
void fn_1_3CCC(void);
BOOL fn_1_1B2C(MGTIMER* timer);
void fn_1_2F4(void);
void fn_1_320(void);
void fn_1_34C(void);

extern void ObjectSetup(void);

#endif
