#ifndef _M551DLL_H
#define _M551DLL_H

#include "humath.h"
#include "game/mg/actman.h"

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
    s16 pad4[4];         /* 0x016 */
    s16 pad1C;           /* 0x01E */
    s16 phase;           /* 0x020 */
    s16 step;            /* 0x022 */
    s16 subPhase;        /* 0x024 */
    s16 timer;           /* 0x026 */
    s32 state;           /* 0x028 */
    s16 pad7[4];         /* 0x02C */
    s16 itemNo;          /* 0x034 */
    s16 itemIdx;         /* 0x036 */
    s16 pad8;            /* 0x038 */
    s16 pad9;            /* 0x03A */
    s16 canDrop;         /* 0x03C */
    s16 pad10;           /* 0x03E */
    f32 rotY2;           /* 0x040 */
    s32 pad11;           /* 0x044 */
    HuVecF pos;          /* 0x048 */
    HuVecF oldPos;       /* 0x054 */
    f32 velX;            /* 0x060 */
    f32 padVelY;         /* 0x064 */
    f32 velZ;            /* 0x068 */
    f32 angle;           /* 0x06C */
    f32 pad12;           /* 0x070 */
    s16 pad13;           /* 0x074 */
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
    s16 pad1[2];         /* 0x00C */
    s16 unk10;           /* 0x010 */
    s16 unk12;           /* 0x012 */
    s16 unk14;           /* 0x014 */
    s16 pad2;            /* 0x016 */
    s16 cpuPlayer;       /* 0x018 */
    s16 winCount[2];     /* 0x01A */
    s16 pad4[1];         /* 0x01E */
    s16 pad5[6];         /* 0x020 */
    s16 pad6[4];         /* 0x02C */
    s16 owner[12];       /* 0x034 */
    s16 carryIdx;        /* 0x04C */
    s16 pad7;            /* 0x04E */
    s32 objP;            /* 0x050 */
    s16 itemNo;          /* 0x054 */
    s16 pad8;            /* 0x056 */
    s32 pad9;            /* 0x058 */
    f32 unk60;           /* 0x05C */
    f32 unk64;           /* 0x060 */
    f32 pad10[0x0D];     /* 0x064 */
    s16 pad98[8];        /* 0x098 */
} M551_FLOW;

/* One 0x8-byte entry of the 12-slot per-item model index table. */
typedef struct M551ItemModel_s {
    s16 mdl[4];          /* 0x00 */
} M551_ITEMMODEL;

/* The object behind lbl_1_bss_34; only the four ambient bytes are read here. */
typedef struct M551Amb_s {
    s32 pad0[0xE];
    u8 unk38;
    u8 unk39;
    u8 unk3A;
    u8 unk3B;
} M551_AMB;

extern M551_AMB *lbl_1_bss_34;

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
extern s32 lbl_1_bss_30;

extern s8 lbl_1_bss_3C[0x16760];
extern s16 lbl_1_bss_1679C[0x54];
extern M551_ITEM lbl_1_bss_16844[0x12];
extern M551_PLAYERWORK lbl_1_bss_16A84[2];
extern M551_FLOW lbl_1_bss_16CB4;
extern s32 lbl_1_bss_16D5C[0xE];
extern f32 lbl_1_bss_16D94[0x245];
extern s16 lbl_1_bss_176A8[0x174];
extern M551_ITEMMODEL lbl_1_bss_179A0[0xC];
extern s32 lbl_1_bss_17998;
extern s16 lbl_1_bss_1799C;

#endif