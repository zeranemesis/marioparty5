#ifndef _M551DLL_H
#define _M551DLL_H

#include "humath.h"
#include "game/object.h"
#include "game/audio.h"
#include "game/hu3d.h"
#include "game/mg/timer.h"
#include "game/mg/seqman.h"
#include "game/mg/actman.h"

typedef struct M551Vec3_s {
    f32 x;
    f32 y;
    f32 z;
} M551Vec3;

/* camera block, 0x50 bytes (lbl_1_bss_38 points at an array of these, 0x148 bytes allocated) */
typedef struct M551Cam_s {
    s32 idx;
    M551Vec3 pos;
    f32 rotX;
    f32 rotY;
    f32 rotZ;
    M551Vec3 center;
    M551Vec3 up;
    f32 zoom;
    f32 aspect;
    f32 vpX;
    f32 vpY;
    f32 vpW;
    f32 vpH;
    f32 fov;
} M551Cam;

/* initial camera values, 0x34 bytes (lbl_1_data_368) */
typedef struct M551CamInit_s {
    M551Vec3 center;
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
} M551CamInit;

/* light block, 0x3C bytes (lbl_1_bss_34) */
typedef struct M551Light_s {
    s16 id;
    s16 pad_02;
    M551Vec3 pos;
    f32 rotX;
    f32 rotY;
    f32 rotZ;
    M551Vec3 target;
    M551Vec3 dir;
    f32 dist;
    GXColor color;
} M551Light;

/* shadow block, 0x50 bytes (lbl_1_bss_30) */
typedef struct M551Shadow_s {
    M551Vec3 pos;
    f32 rotX;
    f32 rotY;
    f32 rotZ;
    M551Vec3 target;
    u8 pad_24[0xC];
    M551Vec3 up;
    u8 pad_3C[0xC];
    f32 dist;
    f32 tpLvl;
} M551Shadow;

/* particle template / live work entry, 0x38 bytes (lbl_1_data_4D0) */
typedef struct M551PtclWork_s {
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
} M551PtclWork;

/* per-character input slot, 0x1C bytes */
typedef struct M551CharSlot_s {
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
} M551CharSlot;

/* one entry (0x1C bytes) of the stage model table (lbl_1_bss_16D94) */
typedef struct M551StageModel_s {
    s16 id;
    s16 pad_02;
    f32 x;
    f32 y;
    f32 z;
    f32 rotX;
    f32 rotY;
    f32 rotZ;
} M551StageModel;

/* per-character animation slot, 0x54 bytes */
typedef struct M551StageSlot_s {
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
} M551StageSlot;

/* stage state block (lbl_1_bss_176A8, 0x2E8 bytes on GC) */
typedef struct M551State_s {
    u8 pad_00[4];
    M551CharSlot chars[2];
    u8 pad_3C[0x1C];
    M551StageModel *models;
    s32 *motionData[2];
    u8 pad_64[4];
    s16 *motionIds[2];
    u8 pad_70[4];
    M551StageSlot slots[2];
    u8 pad_11C[0x54];
    s32 *dataNums;
    s16 field_174;
    u8 pad_176[2];
    s32 field_178;
    u8 pad_17C[0x16C];
} M551State;

/* one falling paper strip, 0x5C bytes (lbl_1_bss_3C[1000]) */
typedef struct M551Paper_s {
    s16 active;          /* 0x00 */
    s16 state;           /* 0x02 */
    s16 life;            /* 0x04 */
    s16 type;            /* 0x06 */
    s16 moving;          /* 0x08 */
    s16 pad_0A;          /* 0x0A */
    HuVecF pos;          /* 0x0C */
    HuVecF vel;          /* 0x18 */
    HuVecF rot;          /* 0x24 */
    HuVecF rotVel;       /* 0x30 */
    f32 yOfs;            /* 0x3C */
    f32 tpLvl;           /* 0x40 */
    HuVecF colA;         /* 0x44 */
    HuVecF colB;         /* 0x50 */
} M551Paper;

/* paper-strip emitter, 0xA8 bytes (lbl_1_bss_1679C) */
typedef struct M551PaperMgr_s {
    s16 active;          /* 0x00 */
    s16 pad_02;          /* 0x02 */
    HuVecF pos;          /* 0x04 */
    f32 yOfs;            /* 0x10 */
    s16 interval;        /* 0x14 */
    s16 burstMax;        /* 0x16 */
    s16 burstCnt;        /* 0x18 */
    s16 total;           /* 0x1A */
    s16 pad_1C[1];       /* 0x1C */
    s16 mdl[4];          /* 0x1E */
    char name[4][0x20];  /* 0x26 */
    s16 hookId;          /* 0xA6 */
} M551PaperMgr;

/* One 0x30-byte item slot in lbl_1_bss_16844. */
typedef struct M551Item_s {
    s16 state;           /* 0x00 */
    s16 oldState;        /* 0x02 */
    s16 timer;           /* 0x04 */
    s16 pad0;            /* 0x06 */
    s16 kind;            /* 0x08 */
    s16 flag;            /* 0x0A */
    s16 pad1[2];         /* 0x0C */
    f32 baseY;           /* 0x10 */
    f32 speed;           /* 0x14 */
    s32 pad2[6];         /* 0x18 */
} M551_ITEM;

/* The 0x118-byte per-player work record.  Offsets come from a full scan of
   every lhz/lha/sth/lwz/stw/lfs/stfs against lbl_1_bss_16A84 in the original
   disassembly; gaps are spelled out as padding so the stride stays exact. */
typedef struct M551PlayerWork_s {
    s16 mode;            /* 0x000 */
    s16 charNo;          /* 0x002 */
    s16 pad0;            /* 0x004 */
    s16 pad1;            /* 0x006 */
    s32 pad2;            /* 0x008 */
    f32 height;          /* 0x00C */
    char *itemHookName;  /* 0x010 */
    s16 carriedBy;       /* 0x014 */
    s16 pad16a;          /* 0x016 */
    f32 unk18;           /* 0x018 */
    f32 gravity;         /* 0x01C */
    s16 phase;           /* 0x020 */
    s16 step;            /* 0x022 */
    s16 subPhase;        /* 0x024 */
    s16 timer;           /* 0x026 */
    s16 flag28;          /* 0x028 */
    s16 pad2A;           /* 0x02A */
    f32 unk2C;           /* 0x02C */
    f32 unk30;           /* 0x030 */
    s16 itemNo;          /* 0x034 */
    s16 itemIdx;         /* 0x036 */
    s16 pad8;            /* 0x038 */
    s16 pad9;            /* 0x03A */
    s16 canDrop;         /* 0x03C */
    s16 pad10;           /* 0x03E */
    f32 colorScale;      /* 0x040 */
    u8 *matColors;       /* 0x044 */
    HuVecF pos;          /* 0x048 */
    HuVecF oldPos;       /* 0x054 */
    f32 velX;            /* 0x060 */
    f32 padVelY;         /* 0x064 */
    f32 velZ;            /* 0x068 */
    f32 angle;           /* 0x06C */
    f32 pad12;           /* 0x070 */
    s16 comOn;           /* 0x074 */
    s16 carryState;      /* 0x076 */
    s16 pad15;           /* 0x078 */
    s16 pad16[3];        /* 0x07A */
    s16 pad17;           /* 0x080 */
    s16 owner[12];       /* 0x082 */
    s16 hookNo;          /* 0x09A */
    s16 hookNo2;         /* 0x09C */
    s16 pad18;           /* 0x09E */
    s16 subState;        /* 0x0A0 */
    s16 pad19[1];        /* 0x0A2 */
    s16 subSub;          /* 0x0A4 */
    s16 pad20;           /* 0x0A6 */
    s16 pushState;       /* 0x0A8 */
    s16 pad21;           /* 0x0AA */
    HuVecF target;       /* 0x0AC */
    f32 radius;          /* 0x0B8 */
    f32 radius2;         /* 0x0BC */
    s16 aimState;        /* 0x0C0 */
    s16 pad22;           /* 0x0C2 */
    HuVecF aim;          /* 0x0C4 */
    s16 aimTimer;        /* 0x0D0 */
    s16 pad23;           /* 0x0D2 */
    s32 pad24[2];        /* 0x0D4 */
    s16 pad25;           /* 0x0DC */
    s16 pad26;           /* 0x0DE */
    s16 pad27;           /* 0x0E0 */
    s16 pad28;           /* 0x0E2 */
    s16 pad29[1];        /* 0x0E4 */
    s16 aimSlot;         /* 0x0E6 */
    s16 aimSlotPrev;     /* 0x0E8 */
    s16 pad30[1];        /* 0x0EA */
    s16 snapState;       /* 0x0EC */
    s16 pad31;           /* 0x0EE */
    s16 pad32;           /* 0x0F0 */
    s16 pad33;           /* 0x0F2 */
    HuVecF snap;         /* 0x0F4 */
    s16 pad34;           /* 0x100 */
    s16 pad35;           /* 0x102 */
    s16 chaseNo;         /* 0x104 */
    s16 chaseSub;        /* 0x106 */
    s32 stkX;            /* 0x108 */
    s32 stkY;            /* 0x10C */
    s32 btnDown;         /* 0x110 */
    s32 btn;             /* 0x114 */
} M551_PLAYERWORK;

/* The 0xA8-byte flow/phase descriptor shared by the mode functions.  Offsets
   come from a full scan of every access against lbl_1_bss_16CB4. */
typedef struct M551Flow_s {
    s16 mode;            /* 0x000 */
    s16 subMode;         /* 0x002 */
    s16 pad0;            /* 0x004 */
    s16 timer;           /* 0x006 */
    f32 camY;            /* 0x008 */
    s16 pad0C;           /* 0x00C */
    s16 pad0E;           /* 0x00E */
    s16 unk10;           /* 0x010 */
    s16 unk12;           /* 0x012 */
    s16 unk14;           /* 0x014 */
    s16 pad16;           /* 0x016 */
    s16 cpuPlayer;       /* 0x018 */
    s16 winCount[2];     /* 0x01A */
    s16 pad1E;           /* 0x01E */
    f32 unk20;           /* 0x020 */
    s16 pad24[2];        /* 0x024 */
    s16 unk28;           /* 0x028 */
    s16 pad2A;           /* 0x02A */
    s16 unk2C;           /* 0x02C */
    s16 unk2E;           /* 0x02E */
    s16 unk30;           /* 0x030 */
    s16 unk32;           /* 0x032 */
    s16 owner[12];       /* 0x034 */
    s16 carryIdx;        /* 0x04C */
    s16 pad4E;           /* 0x04E */
    MGTIMER *timerObj;   /* 0x050 */
    s16 pad54;           /* 0x054 */
    s16 itemNo;          /* 0x056 */
    s16 unk58;           /* 0x058 */
    s16 pad5A;           /* 0x05A */
    f32 fadeTp;          /* 0x05C */
    f32 ambScale;        /* 0x060 */
    f32 ambLevel;        /* 0x064 */
    f32 pad68;           /* 0x068 */
    s32 bgmStream;       /* 0x06C */
    s32 jingleStream;    /* 0x070 */
    s32 pad74;           /* 0x074 */
    s32 fxId[8];         /* 0x078 */
    s16 fxOn[8];         /* 0x098 */
} M551_FLOW;

/* One 0x8-byte entry of the 12-slot per-item model index table. */
typedef struct M551ItemModel_s {
    s16 mdl[4];          /* 0x00 */
} M551_ITEMMODEL;

/* The record whose s32 at 0x08 fn_1_117B0 range-checks. */
typedef struct M551RangeChk_s {
    s32 pad0[2];         /* 0x00 */
    s32 unk8;            /* 0x08 */
} M551_RANGECHK;

/* The 0x30-byte master work structure at lbl_1_bss_0. */
typedef struct M551Work_s {
    s16 colMdl[13];      /* 0x000 */
    s16 pad0[7];         /* 0x01A */
    MGPLAYER *player[2]; /* 0x028 */
} M551_WORK;

extern M551_WORK lbl_1_bss_0;
extern M551_ITEMMODEL lbl_1_bss_179A0[12];
extern M551_FLOW lbl_1_bss_16CB4;
extern M551_PLAYERWORK lbl_1_bss_16A84[2];
extern M551_ITEM lbl_1_bss_16844[12];
extern M551Paper lbl_1_bss_3C[1000];
extern M551PaperMgr lbl_1_bss_1679C;
extern M551State *lbl_1_bss_17A00;
extern M551PtclWork lbl_1_data_4D0;
extern s16 lbl_1_data_438[16];

/* m551.c */
void fn_1_91C(s16 idx);
void fn_1_10C4(void);
void fn_1_14C4(void);
s32 fn_1_1850(f32 *arg0, f32 steps, f32 start, f32 target);
s32 fn_1_1970(s32 *arg0, s32 steps, s32 start, s32 target);
MGTIMER *fn_1_1A44(s32 type, s32 maxSec, s32 endSec, f32 x, f32 y);
void fn_1_1AB8(MGTIMER *timer);
BOOL fn_1_1AE4(MGTIMER *timer);
void fn_1_1B0C(MGTIMER *timer);
void fn_1_2354(void);
void fn_1_2610(M551Vec3 *pos);
void fn_1_29DC(M551Vec3 *pos);
void fn_1_3270(M551Vec3 *pos);
void fn_1_41C8(M551State *state, s16 idx, HuVecF *pos);
void fn_1_43CC(void);
void fn_1_4CB0(void);
void fn_1_5860(s16 arg0);
s32 fn_1_5958(s16 arg0, f32 scale);
void fn_1_5AC8(s16 arg0);
void fn_1_5BC4(void);
void fn_1_5BF4(void);
void fn_1_5C24(s16 isBgm, s16 speed);
void fn_1_5C8C(s16 isBgm);
void fn_1_5CEC(s16 arg0);
void fn_1_5D64(s16 arg0, s16 vol, s16 pan);
void fn_1_5DEC(s16 arg0, s16 pan);
void fn_1_5E7C(s16 arg0);
void fn_1_5F28(s16 arg0);
void fn_1_5FD4(void);

/* stage.c */
void fn_1_170B0(void);

/* game.c */
s32 fn_1_805C(M551State *state);
s32 fn_1_81F4(void);
s32 fn_1_822C(void);
s32 fn_1_828C(void);
s32 fn_1_89B0(void);
s32 fn_1_8CBC(void);
s32 fn_1_8D20(void);
s16 fn_1_8FF4(void);
s32 fn_1_9024(void);

#endif
