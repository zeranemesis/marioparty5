#ifndef _M544DLL_H
#define _M544DLL_H

#include "types.h"
#include "game/hu3d.h"
#include "game/mg/timer.h"

typedef struct M544Vec3_s {
    f32 x;
    f32 y;
    f32 z;
} M544Vec3;

/* per-player input slot, 0x1C bytes */
typedef struct M544CharSlot_s {
    s16 playerNo;
    s16 charNo;
    s16 cpuLevel; /* 0 = human */
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
} M544CharSlot;

/* model table entry, 0x1C bytes (lbl_1_bss_15C, 35 entries) */
typedef struct M544Mdl_s {
    s16 id;
    s16 pad_02;
    HuVecF pos;
    HuVecF rot;
} M544Mdl;

/* character/prop motion controller, 0x54 bytes */
typedef struct M544Slot_s {
    s16 motion;
    s16 curMotion;
    f32 speed;
    f32 curSpeed;
    f32 time;
    f32 curTime;
    f32 maxTime;
    u32 attrSet;
    u32 attrCur;
    u32 attrReset;
    s32 endFlag;
    s16 chain;
    s16 chainMotion;
    f32 chainTime;
    u32 chainAttr;
    u32 chainReset;
    s16 shiftState;
    s16 shiftMotion;
    u32 shiftAttr;
    f32 shiftStart;
    f32 shiftEnd;
    f32 shiftCur;
    f32 shiftMax;
    u8 pad_50[4];
} M544Slot;

/* one 0x30 block of stage-prop animation state */
typedef struct M544AnimBlock_s {
    s32 hold;
    s32 timer;
    s32 timer2;
    s32 state;
    u8 pad_10[0x10];
    f32 f20;
    f32 f24;
    f32 f28;
    u8 pad_2C[4];
} M544AnimBlock;

/* module state (lbl_1_bss_530, 0x2E8 bytes on GC) shared by every unit */
typedef struct M544State_s {
    u8 pad_00[4];
    M544CharSlot chars[2]; /* 0x04 */
    u8 pad_3C[0x1C];
    M544Mdl *models; /* 0x58 */
    s32 *motionData[3]; /* 0x5C, [2] is the stage motion list */
    s16 *motionIds[3]; /* 0x68, [2] is the stage motion ids */
    M544Slot slots[3]; /* 0x74 */
    s32 *dataNums; /* 0x170 */
    s16 stage; /* 0x174 */
    s16 pad_176;
    s32 field_178;
    u8 pad_17C[0x4C];
    M544AnimBlock anim[5]; /* 0x1C8 */
    u8 pad_2B8[0x30];
} M544State;

/* camera block, 0x50 bytes */
typedef struct M544Cam_s {
    s32 idx;
    M544Vec3 pos;
    f32 rotX;
    f32 rotY;
    f32 rotZ;
    M544Vec3 center;
    M544Vec3 up;
    f32 zoom;
    f32 aspect;
    f32 vpX;
    f32 vpY;
    f32 vpW;
    f32 vpH;
    f32 fov;
} M544Cam;

typedef struct M544CamInit_s {
    M544Vec3 center;
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
} M544CamInit;

/* light block, 0x3C bytes */
typedef struct M544Light_s {
    s16 id;
    s16 pad_02;
    M544Vec3 pos;
    f32 rotX;
    f32 rotY;
    f32 rotZ;
    M544Vec3 target;
    M544Vec3 dir;
    f32 dist;
    GXColor color;
} M544Light;

/* shadow block, 0x50 bytes */
typedef struct M544Shadow_s {
    M544Vec3 pos;
    f32 rotX;
    f32 rotY;
    f32 rotZ;
    M544Vec3 target;
    u8 pad_24[0xC];
    M544Vec3 up;
    u8 pad_3C[0xC];
    f32 dist;
    f32 tpLvl;
} M544Shadow;

/* particle template / live work entry, 0x38 bytes */
typedef struct M544PtclWork_s {
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
} M544PtclWork;

/* game-flow block (lbl_1_bss_D0, 0x60 bytes on GC) */
typedef struct M544Game_s {
    s16 phase; /* 0x00 */
    s16 phase2; /* 0x02 */
    s16 unk4;
    s16 timer6; /* 0x06 */
    s16 pad_08[12];
    s16 plrMask; /* 0x20 */
    s16 pad_22;
    MGTIMER *timer; /* 0x24 */
    s16 flag28;
    s16 pad_2A;
    s16 flag2C;
    s16 flag2E;
    s32 bgmId; /* 0x30 */
    s32 jingleId; /* 0x34 */
    s16 flag38;
    s16 pad_3A;
    s32 fxId[6]; /* 0x3C */
    s16 fxFlag[6]; /* 0x54 */
} M544Game;

/* stage-object block (lbl_1_bss_C, 0x74 bytes on GC) */
typedef struct M544Stage_s {
    s16 phase; /* 0x00 */
    s16 phase2; /* 0x02 */
    s16 mode; /* 0x04 */
    s16 pad_06;
    f32 tpLvl; /* 0x08 */
    s16 timerC; /* 0x0C */
    s16 timerE; /* 0x0E */
    s16 pad_10;
    s16 idx; /* 0x12 */
    s16 pad_14;
    s16 flag16; /* 0x16 */
    char name[0x2C]; /* 0x18 */
    Mtx mtx; /* 0x44 */
} M544Stage;

/* per-player object (lbl_1_bss_80, 2 entries of 0x28 bytes on GC) */
typedef struct M544Obj_s {
    s16 f0;
    s16 f2;
    s16 state; /* 0x04 */
    s16 stage; /* 0x06 */
    s16 timer; /* 0x08 */
    s16 flagA; /* 0x0A */
    s16 mode; /* 0x0C */
    s16 count; /* 0x0E */
    HuVecF scale; /* 0x10 */
    char *hook; /* 0x1C */
    s16 pad_20;
    s16 delay; /* 0x22 */
    s16 aiMode; /* 0x24 */
    s16 target; /* 0x26 */
} M544Obj;

/* flying item (lbl_1_bss_828, 4 entries of 0x2C bytes on GC) */
typedef struct M544Item_s {
    HuVecF a; /* 0x00 */
    HuVecF b; /* 0x0C */
    s32 timer; /* 0x18 */
    s32 state; /* 0x1C */
    s32 active; /* 0x20 */
    s32 owner; /* 0x24 */
    f32 alpha; /* 0x28 */
} M544Item;

/* random round table (lbl_1_bss_8D8, 10 entries of 6 bytes) */
typedef struct M544Rnd_s {
    s16 a;
    s16 b;
    s16 c;
} M544Rnd;

extern M544Cam *lbl_1_bss_8;
extern M544Light *lbl_1_bss_4;
extern M544Shadow *lbl_1_bss_0;
extern M544Stage lbl_1_bss_C;
extern M544Obj lbl_1_bss_80[2];
extern M544Game lbl_1_bss_D0;

#endif
