#ifndef _M552DLL_H
#define _M552DLL_H

#include "types.h"
#include "game/hu3d.h"
#include "game/object.h"

/* per-player input slot, 0x1C bytes (inside M552State, starts at +4) */
typedef struct M552CharSlot_s {
    s16 playerNo; /* index into GwPlayerConf */
    s16 charNo;
    s16 cpuLevel; /* 0 = human */
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
} M552CharSlot;

/* entry of the stage model table (lbl_1_bss_84, 0x1C bytes) */
typedef struct M552Mdl_s {
    s16 id;
    s16 pad_02;
    HuVecF pos;
    HuVecF rot;
} M552Mdl;

/* generic scene object (0x28 bytes): lbl_1_bss_490/530/710/1200/1340 */
typedef struct M552Obj_s {
    s32 state;
    s32 count;
    s32 unk8;
    s16 model;
    s16 pad_0E;
    HuVecF pos;
    HuVecF rot;
} M552Obj;

/* character motion controller, 0x54 bytes */
typedef struct M552Slot_s {
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
} M552Slot;

/* module state (lbl_1_bss_4794) shared by every unit of the REL */
typedef struct M552State_s {
    OMOBJMAN *objman; /* 0x00 */
    M552CharSlot chars[2]; /* 0x04 */
    M552Mdl *models; /* 0x3C */
    s32 *motionData[2]; /* 0x40 */
    s16 *motionIds[2]; /* 0x48 */
    M552Slot slots[2]; /* 0x50 */
    s32 *dataNums; /* 0xF8 */
    s16 stage; /* 0xFC */
    s16 pad_FE;
    s32 stat[4]; /* 0x100: [0] mode, [1] sub, [2 + player] puzzle state ([2] doubles as result winner) */
    s32 timer[4]; /* 0x110: [player] move timer (sequence frame counter in [0]), [2 + player] rotate timer */
    u8 pad_120[0x20];
    f32 moveVec[2][3]; /* 0x140: x, y, rotate step */
    u8 pad_158[0x1C];
} M552State;

/* falling piece / held piece, 0x98 bytes */
typedef struct M552Piece_s {
    s32 alive;
    s32 shape;
    f32 x;
    f32 y;
    f32 z;
    f32 rot;
    s8 grid[25];
    u8 pad_31[3];
    s32 cube[25]; /* index into lbl_1_bss_1340 */
} M552Piece;

/* score popup, 0x34 bytes */
typedef struct M552Popup_s {
    s32 active;
    s32 value;
    f32 x;
    f32 y;
    s32 unk10;
    s32 unk14;
    s32 timer;
    s32 stage;
    s32 spr[5];
} M552Popup;

/* per-player play field, 0x254 bytes (starts at lbl_1_bss_3700 + 0xBEC) */
typedef struct M552Plr_s {
    f32 x; /* 0x000 */
    f32 y;
    f32 z;
    M552Piece piece; /* 0x00C */
    s32 cellX; /* 0x0A4 */
    s32 cellY; /* 0x0A8 */
    s8 board[35]; /* 0x0AC */
    s32 boardCube[35]; /* 0x0D0 */
    s8 count[4]; /* 0x15C */
    s32 timer; /* 0x160 */
    s32 idx; /* 0x164 */
    s32 stage; /* 0x168 */
    s32 shown; /* 0x16C */
    s32 score; /* 0x170 */
    M552Popup popup[4]; /* 0x174 */
    s32 unk244;
    s32 unk248;
    s32 unk24C;
    s32 unk250;
} M552Plr;

/* lbl_1_bss_3700 */
typedef struct M552Field_s {
    s32 unk0;
    s32 head; /* next item slot */
    s32 unk8;
    M552Piece item[20];
    M552Plr plr[2];
} M552Field;

#endif
