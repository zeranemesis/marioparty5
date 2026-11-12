#ifndef _M548DLL_H
#define _M548DLL_H

#include "types.h"

/* per-character slot, 0x1C bytes; the objman pointer of M548State sits in front of chars[0] */
typedef struct M548CharSlot_s {
    s16 playerNo;
    s16 charNo;
    s16 cpuLevel;
    s16 padNo;
    s32 btn;
    s32 btnDown;
    s8 stkX;
    s8 stkY;
    u16 btnRep;
    s8 subStkX;
    s8 subStkY;
    u8 trigL;
    u8 trigR;
    u8 dstk;
    u8 dstkRep;
    u8 pad_1A[2];
} M548CharSlot;

/* per-character animation slot, 0x24 bytes */
typedef struct M548StageSlot_s {
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
} M548StageSlot;

/* one entry (0x1C bytes) of the stage model table (lbl_1_bss_154) */
typedef struct M548StageModel_s {
    s16 id;
    s16 pad_02;
    f32 x;
    f32 y;
    f32 z;
    f32 rotX;
    f32 rotY;
    f32 rotZ;
} M548StageModel;

typedef struct M548Vec3_s {
    f32 x;
    f32 y;
    f32 z;
} M548Vec3;

/* one 0x30 block of animation state per scripted prop (stage.c) */
typedef struct M548AnimBlock_s {
    s32 hold;
    s32 timer;
    s32 timer2;
    s32 state;
    u8 pad_10[0x10];
    f32 f20;
    f32 f24;
    f32 f28;
    u8 pad_2C[4];
} M548AnimBlock;

/* per-character cry timer state (m548.c bss 0x0) */
typedef struct M548Cry_s {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
} M548Cry;

/* per-character floats (data 0x0 table, copied to bss 0x384) */
typedef struct M548CryPos_s {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
} M548CryPos;

/* motion data numbers of one character (data 0x184 table) */
typedef struct M548MotSet_s {
    u32 unk0;
    u32 unk4;
    u32 unk8;
    u32 unkC;
    u32 unk10;
} M548MotSet;

/* game-flow block (m548.c bss 0x3D0) */
typedef struct M548Game_s {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 pad_E;
    f32 unk10[2];
    f32 unk18[2];
} M548Game;

/* module state block (lbl_1_bss_3F0, 0x1E0 bytes); m548.c, stage.c and game.c share it */
typedef struct M548State_s {
    void *objman;
    M548CharSlot chars[2];
    union {
        M548StageModel *models;
        s16 (*modelIds)[14];
    };
    u32 *motionData[2];
    s16 *motionIds[2];
    M548StageSlot slots[2];
    s32 *dataNums;
    s16 field9C;
    u8 pad_9E[2];
    s32 stateA0;
    s32 stateA4;
    s32 fieldA8;
    s32 fieldAC;
    s32 counters[4];
    M548Vec3 pos[3];
    u8 pad_E4[0xC];
    M548AnimBlock anim[5];
} M548State;

#endif
