#ifndef _M555DLL_H
#define _M555DLL_H

#include "humath.h"

/* The 0x344-byte per-player record.  Only the fields the code touches are
   named; the rest is padding sized to keep the stride at 0x344. */
typedef struct M555Player_s {
    s16 pad0[1];         /* 0x000 */
    s16 motion;          /* 0x002 */
    s16 pad1[2];         /* 0x004 */
    s16 charNo;          /* 0x008 */
    s16 joinNo;          /* 0x00A */
    s16 state;           /* 0x00C */
    s16 pad2;            /* 0x00E */
    s16 step;            /* 0x010 */
    s16 subStep;         /* 0x012 */
    s16 stage;           /* 0x014 */
    s16 timer2;          /* 0x016 */
    s16 timer3;          /* 0x018 */
    s16 hookX;           /* 0x01A */
    s16 pad3c;           /* 0x01C */
    s16 hookOn;          /* 0x01E */
    f32 stickX;          /* 0x020 */
    f32 stickY;          /* 0x024 */
    s16 pad4[4];         /* 0x028 */
    f32 accX;            /* 0x030 */
    f32 accY;            /* 0x034 */
    s16 pad4b[2];        /* 0x038 */
    f32 velX;            /* 0x03C */
    f32 velY;            /* 0x040 */
    s16 pad5[7];         /* 0x044 */
    s16 wheel;           /* 0x052 */
    s16 camMode;         /* 0x054 */
    s16 wheelState;      /* 0x056 */
    s16 pad6[2];         /* 0x058 */
    s16 anim;            /* 0x05C */
    s16 anim2;           /* 0x05E */
    s16 pad7[0x144];     /* 0x060 */
    s16 tallyA;          /* 0x2E8 */
    s16 tallyB;          /* 0x2EA */
    s16 pad7b[1];        /* 0x2EC */
    f32 speedScale;      /* 0x2F0 */
    f32 speedScale2;     /* 0x2F4 */
    s16 pad8[2];         /* 0x2F8 */
    s16 flag0;           /* 0x2FC */
    s16 flag1;           /* 0x2FE */
    s16 pad9[1];         /* 0x300 */
    s16 flag2;           /* 0x302 */
    s16 flag3;           /* 0x304 */
    s16 pad9b[1];        /* 0x306 */
    HuVecF targetPos;    /* 0x308 */
    f32 carryX;          /* 0x314 */
    f32 carryY;          /* 0x318 */
    f32 carryZ;          /* 0x31C */
    f32 camX0;           /* 0x320 */
    f32 camY0;           /* 0x324 */
    f32 camZ0;           /* 0x328 */
    s16 camIdx0;         /* 0x32C */
    s16 camIdx1;         /* 0x32E */
    s16 camIdx2;         /* 0x330 */
    s16 pad10[1];        /* 0x332 */
    s32 pad11[2];        /* 0x334 */
    s32 wheelIdx0;       /* 0x33C */
    s32 wheelIdx1;       /* 0x340 */
} M555_PLAYER;

/* One 0x1C-byte model record: a model id then a position and a rotation. */
typedef struct M555ModelEntry_s {
    s16 mdlId;           /* 0x00 */
    HuVecF pos;          /* 0x04 */
    f32 rotX;            /* 0x10 */
    f32 rotY;            /* 0x14 */
    f32 rotZ;            /* 0x18 */
} M555_MODELENTRY;

/* One 0x1C-byte model record embedded in the control structure at offset 0.
   The trailing bytes are the two analogue-stick inputs for this player. */
typedef struct M555CtrlModel_s {
    s16 mdlId;           /* 0x00 */
    s16 pad0;            /* 0x02 */
    s16 playerNo;        /* 0x04 */
    s16 charNo;          /* 0x06 */
    s16 charSlot;        /* 0x08 */
    s16 pad0a;           /* 0x0A */
    f32 posX;            /* 0x0C */
    f32 posY;            /* 0x10 */
    s8 stickX;           /* 0x14 */
    s8 pad1[3];          /* 0x15 */
    s8 stickY;           /* 0x18 */
    s8 pad2[3];          /* 0x19 */
} M555_CTRLMODEL;

/* One 0x54-byte record embedded in the control structure at offset 0x74. */
typedef struct M555CtrlWin_s {
    s16 on;              /* 0x00 */
    s16 pad0;            /* 0x02 */
    f32 scale;           /* 0x04 */
    f32 pad1;            /* 0x08 */
    f32 value;           /* 0x0C */
    f32 angle;           /* 0x10 */
    s32 pad2[1];         /* 0x14 */
    s32 alpha;           /* 0x18 */
    s32 pad3;            /* 0x1C */
    s32 flags;           /* 0x20 */
    s32 pad5[0x0C];      /* 0x24 */
} M555_CTRLWIN;

/* Global state reached through the lbl_1_bss_3030 pointer.  It carries three
   independently-strided arrays: one at 0x00 (0x1C), one at 0x74 (0x54) and one
   behind the pointer at 0x58, plus scalars at 0x170/0x178. */
typedef struct M555Ctrl_s {
    M555_CTRLMODEL mdl0[2];  /* 0x000, 0x1C stride */
    s16 pad0[0x10];          /* 0x038 */
    M555_MODELENTRY *mdl;    /* 0x058 */
    s16 pad1[0x0C];          /* 0x05C */
    M555_CTRLWIN win[2];     /* 0x074, 0x54 stride */
    s32 pad2[0x15];          /* 0x11C */
    s32 *dataList;           /* 0x170 */
    s32 pad3;                /* 0x174 */
    s32 seqNo;               /* 0x178 */
} M555_CTRL;

/* One 0x18-byte item record in the 100-entry lbl_1_bss_214 table. */
typedef struct M555Item_s {
    s16 active;          /* 0x00 */
    s16 type;            /* 0x02 */
    s16 pad0;            /* 0x04 */
    s16 kind;            /* 0x06 */
    s16 mdlId;           /* 0x08 */
    s16 slot;            /* 0x0A */
    s16 angX;            /* 0x0C */
    s16 angY;            /* 0x10 */
    s16 pad1;            /* 0x12 */
    f32 fade;            /* 0x14 */
} M555_ITEM;



/* One 0x20-byte marker record in the 50-entry lbl_1_bss_B88 table.  A live
   record carries a per-frame step vector and a fade level that the director
   drives down to zero. */
typedef struct M555Mark_s {
    s16 active;         /* 0x00 */
    s16 type;           /* 0x02 */
    s16 pad0[4];        /* 0x04 */
    f32 stepX;          /* 0x0C */
    f32 stepY;          /* 0x10 */
    f32 stepZ;          /* 0x14 */
    s16 pad1[2];        /* 0x18 */
    f32 fade;           /* 0x1C */
} M555_MARK;

/* The 0x1A-byte spawn-director state block at lbl_1_bss_11C8.  Six named
   lanes each hold a live-marker count capped by a per-lane limit. */
typedef struct M555Hook_s {
    s16 state;          /* 0x00 */
    s16 timer;          /* 0x02 */
    s16 limit;          /* 0x04 */
    s16 timer2;         /* 0x06 */
    s16 limit2;         /* 0x08 */
    s16 idx;            /* 0x0A */
    s16 count[6];       /* 0x0C */
    s16 live;           /* 0x18 */
} M555_HOOK;

/* One 0x28-byte sub-record used by the spin/camera code. */
typedef struct M555Spin_s {
    s16 mode;            /* 0x00 */
    s16 pad0;            /* 0x02 */
    s16 timer;           /* 0x04 */
    s16 pad1[3];         /* 0x06 */
    f32 spinAngle;       /* 0x0C */
    f32 camX;            /* 0x10 */
    f32 camY;            /* 0x14 */
    s16 pad3[2];         /* 0x18 */
    f32 stepX;           /* 0x1C */
    f32 stepY;           /* 0x20 */
    s16 pad4;            /* 0x24 */
    s16 pad5;            /* 0x26 */
} M555_SPIN;

/* One 0x30-byte sub-record.  The array of four starts at offset 0x84 of the
   master work structure; 0x84 is the only base in (0x80, 0x86] that keeps
   every observed f32 access 4-byte aligned. */
typedef struct M555WorkItem_s {
    s16 pad0;            /* 0x00 */
    s16 state;           /* 0x02 */
    s16 pad1;            /* 0x04 */
    s16 timer;           /* 0x06 */
    f32 value;           /* 0x08 */
    f32 angle;           /* 0x0C */
    f32 pad2;            /* 0x10 */
    f32 pad3;            /* 0x14 */
    f32 posX;            /* 0x18 */
    f32 posY;            /* 0x1C */
    f32 posZ;            /* 0x20 */
    f32 stepX;           /* 0x24 */
    f32 stepY;           /* 0x28 */
    f32 stepZ;           /* 0x2C */
} M555_WORKITEM;

/* The 0x1E8-byte master work structure. */
typedef struct M555Work_s {
    s16 phase;           /* 0x000 */
    s16 phase2;          /* 0x002 */
    s16 subPhase;        /* 0x004 */
    s16 subPhase2;       /* 0x006 */
    s32 frameCnt;        /* 0x008 */
    s16 motion;          /* 0x00C */
    s16 pad0[8];         /* 0x00E */
    s16 unk1E;           /* 0x01E */
    s16 pad1[3];         /* 0x020 */
    s16 unk26;           /* 0x026 */
    s16 curPlayer;       /* 0x028 */
    s16 curPlayer2;      /* 0x02A */
    s16 playerNo;        /* 0x02C */
    s16 phase4;          /* 0x02E */
    s16 timer;           /* 0x030 */
    s16 pad3;            /* 0x032 */
    s16 count;           /* 0x034 */
    s16 resultF;         /* 0x036 */
    s16 resultOn;        /* 0x038 */
    s16 pad4;            /* 0x03A */
    f32 scale;           /* 0x03C */
    s16 winF;            /* 0x040 */
    s16 pad5[0x21];      /* 0x042 */
    M555_WORKITEM item[4];/* 0x084 */
    s16 posIdx;          /* 0x144 */
    s16 posIdx2;         /* 0x146 */
    s16 itemIdx;         /* 0x148 */
    s16 itemIdx2;        /* 0x14A */
    s16 pad6[4];         /* 0x14C */
    f32 basePosX;        /* 0x154 */
    f32 posX;            /* 0x158 */
    f32 posY;            /* 0x15C */
    s16 unk160;          /* 0x160 */
    s16 pad7;            /* 0x162 */
    f32 phaseAngle;      /* 0x164 */
    f32 pad8;            /* 0x168 */
    f32 pad9;            /* 0x16C */
    f32 pad10;           /* 0x170 */
    s16 pad11;           /* 0x174 */
    s16 unk178;          /* 0x178 */
    s16 pad12;           /* 0x17A */
    f32 fogScale;        /* 0x17C */
    s16 fogOn;           /* 0x180 */
    s16 fogSet;          /* 0x182 */
    f32 fogStart;        /* 0x184 */
    f32 fogEnd;          /* 0x188 */
    u8 fogR;             /* 0x18C */
    u8 fogG;             /* 0x18D */
    u8 fogB;             /* 0x18E */
    s16 pad13[0x1F];     /* 0x18F */
    s16 hookIdx[12];     /* 0x1D0 */
} M555_WORK;

extern M555_WORK lbl_1_bss_1894;
extern M555_CTRL *lbl_1_bss_3030;
extern M555_PLAYER lbl_1_bss_120C[2];
extern M555_ITEM lbl_1_bss_214[100];
extern M555_SPIN lbl_1_bss_11E4;

#endif
