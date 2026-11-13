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

#include "humath.h"

#include "stddef.h"

/* Shared declarations for m549.c / game.c / cloud.c / stage.c go below.
 * Rules: add with small Edits, re-read the file first, never delete another agent's entries. */

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
    u8 pad_174[0x54];
    M549AnimBlock anim[21];
} M549State;

extern M549State *lbl_1_bss_1608;

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
