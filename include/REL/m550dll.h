#ifndef _M550DLL_H
#define _M550DLL_H

#include "game/gamework.h"
#include "game/object.h"
#include "game/hu3d.h"

#include "datanum/m550.h"

/* One 0x1C slot of lbl_1_bss_A8: a model id plus its cached pos/rot. */
typedef struct M550Model_s {
    /* 0x00 */ s16 id;
    /* 0x04 */ HuVecF pos;
    /* 0x10 */ HuVecF rot;
} M550MODEL;

typedef struct M550Player_s {
    /* 0x00 */ OMOBJMAN *objman; /* only slot 0 is used */
    /* 0x04 */ s16 pno;
    /* 0x06 */ s16 charNo;
    /* 0x08 */ s16 cpuLevel;
    /* 0x0A */ s16 padNo;
    /* 0x0C */ s32 btn;
    /* 0x10 */ s32 btnDown;
    /* 0x14 */ s8 stkX;
    /* 0x15 */ s8 stkY;
    /* 0x16 */ u16 btnRep;
    /* 0x18 */ s8 subStkX;
    /* 0x19 */ s8 subStkY;
    /* 0x1A */ u8 trigL;
    /* 0x1B */ u8 trigR;
#ifdef TARGET_PC
    /* the GC code writes dstk/dstkRep at +0x1C/+0x1D, i.e. into the next slot */
    u8 dstk;
    u8 dstkRep;
#endif
} M550PLAYER;

#ifdef TARGET_PC
#define M550_DSTK(p) ((p)->dstk)
#define M550_DSTKREP(p) ((p)->dstkRep)
#else
#define M550_DSTK(p) (*((u8 *)(p) + 0x1C))
#define M550_DSTKREP(p) (*((u8 *)(p) + 0x1D))
#endif

/* Per-player motion request block (0x54 bytes). */
typedef struct M550Motion_s {
    /* 0x00 */ s16 motNo;
    /* 0x02 */ s16 prevMotNo;
    /* 0x04 */ f32 speed;
    /* 0x08 */ f32 prevSpeed;
    /* 0x0C */ f32 time;
    /* 0x10 */ f32 prevTime;
    /* 0x14 */ f32 maxTime;
    /* 0x18 */ u32 attrSet;
    /* 0x1C */ u32 prevAttrSet;
    /* 0x20 */ u32 attrReset;
    /* 0x24 */ s32 endF;
    /* 0x28 */ s16 chainF;
    /* 0x2A */ s16 chainMotNo;
    /* 0x2C */ f32 chainTime;
    /* 0x30 */ u32 chainAttrSet;
    /* 0x34 */ u32 chainAttrReset;
    /* 0x38 */ s16 shiftState;
    /* 0x3A */ s16 shiftMotNo;
    /* 0x3C */ u32 shiftAttr;
    /* 0x40 */ f32 shiftStart;
    /* 0x44 */ f32 shiftEnd;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ f32 shiftMaxTime;
    /* 0x50 */ s32 unk50;
} M550MOTION;

/* Decoration/effect work slot (0x4C bytes). */
typedef struct M550Fx_s {
    /* 0x00 */ s32 state;
    /* 0x04 */ s32 cnt[3];
    /* 0x10 */ s32 work10;
    /* 0x14 */ s32 work14;
    /* 0x18 */ s32 work18;
    /* 0x1C */ s32 work1C;
    /* 0x20 */ s32 work20;
    /* 0x24 */ s32 work24;
    /* 0x28 */ s32 work28;
    /* 0x2C */ f32 f2C;
    /* 0x30 */ f32 f30;
    /* 0x34 */ f32 f34;
    /* 0x38 */ f32 f38;
    /* 0x3C */ f32 f3C;
    /* 0x40 */ f32 f40;
    /* 0x44 */ f32 f44;
    /* 0x48 */ s32 work48;
} M550FX;

typedef struct M550MotionSet_s {
    s32 id[5];
} M550MOTIONSET;

/* The model file id table (lbl_1_data_8); stage setup reaches some entries
   as named member arrays, so mirror that layout. */
typedef struct M550ModelIds_s {
    /* 0x00 */ s32 id[0x33];
    /* 0xCC */ s32 cc[2];
    /* 0xD4 */ s32 d4[2];
    /* 0xDC */ s32 dc[2];
    /* 0xE4 */ s32 e4;
    /* 0xE8 */ s32 e8[2];
} M550MODELIDS;

/* lbl_1_bss_14AC: the module-wide game state block. */
typedef struct M550State_s {
    /* 0x000 */ M550PLAYER player[2];
    /* 0x038 */ s32 unk38;
    /* 0x03C */ M550MODEL *model;
    /* 0x040 */ M550MOTIONSET *motionSet[2];
    /* 0x048 */ s16 *motionId[2];
    /* 0x050 */ M550MOTION motion[2];
    /* 0x0F8 */ M550MODELIDS *modelData;
    /* 0x0FC */ s16 stageNo;
    /* 0x100 */ s32 modes[3]; /* [0] game mode, [1 + side] per-player state */
    /* 0x10C */ s32 unk10C;
    /* 0x110 */ s32 timer;
    /* 0x114 */ s32 unk114;
    /* 0x118 */ s32 timer2;
    /* 0x11C */ s32 unk11C;
    /* 0x120 */ s32 score[4];
    /* 0x130 */ s32 winner;
    /* 0x134 */ s32 winStep;
    /* 0x138 */ f32 scale[2];
    /* 0x140 */ s32 unk140[2];
    /* 0x148 */ HuVecF fxVec[4];
    /* 0x178 */ M550FX fx[17];
    /* 0x684 */ u8 unk684[0xA14 - 0x684];
} M550STATE;

/* lbl_1_bss_4: per-player CPU control block (0x10 bytes). */
typedef struct M550Cpu_s {
    /* 0x00 */ s16 level;
    /* 0x04 */ s32 unk04;
    /* 0x08 */ s32 wait;
    /* 0x0C */ s32 count;
} M550CPU;

/* Three-entry index tables kept in game.c's .rodata (see fn_1_50D8). */
typedef struct M550Idx3_s {
    s32 v[3];
} M550IDX3;

void M550StageInit(OMOBJMAN *om);
void M550StageClose(void);

#endif
