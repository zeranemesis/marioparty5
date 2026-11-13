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
    s16 unk0E;           /* 0x00E */
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
    HuVecF prevPos;      /* 0x044 */
    s16 idleCnt;         /* 0x050 */
    s16 wheel;           /* 0x052 */
    s16 camMode;         /* 0x054 */
    s16 wheelState;      /* 0x056 */
    s16 pad6[2];         /* 0x058 */
    s16 anim;            /* 0x05C */
    s16 anim2;           /* 0x05E */
    HuVecF edgePos[0x24]; /* 0x060 */
    f32 edgeDist[0x24];  /* 0x210 */
    s16 edgeKind[0x24];  /* 0x2A0 */
    s16 tallyA;          /* 0x2E8 */
    s16 tallyB;          /* 0x2EA */
    s16 pad7b[1];        /* 0x2EC */
    HuVecF aimPos;       /* 0x2F0 */
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
    HuVecF rot;          /* 0x10 */
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
    s32 pad5[5];         /* 0x24 */
    s16 on38;            /* 0x38 */
    s16 on3A;            /* 0x3A */
    s32 val3C;           /* 0x3C */
    f32 val40;           /* 0x40 */
    f32 val44;           /* 0x44 */
    s32 pad6[3];         /* 0x48 */
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
    s16 cnt;             /* 0x04 */
    s16 kind;            /* 0x06 */
    s16 mdlId;           /* 0x08 */
    s16 slot;            /* 0x0A */
    f32 angX;            /* 0x0C */
    f32 angY;            /* 0x10 */
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
    HuVecF camPos;       /* 0x168 */
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

/* 0x148-byte camera block; lbl_1_bss_8 points at it. */
typedef struct M555CamBlk_s {
    s32 pad0;            /* 0x00 */
    HuVecF pos;          /* 0x04 */
    f32 rotX;            /* 0x10 */
    f32 rotY;            /* 0x14 */
    f32 rotZ;            /* 0x18 */
    HuVecF tgt;          /* 0x1C */
    f32 pad28[3];        /* 0x28 */
    f32 zoom;            /* 0x34 */
    u8 pad38[0x148 - 0x38]; /* 0x38 */
} M555_CAMBLK;

extern M555_CAMBLK *lbl_1_bss_8;
extern M555_WORK lbl_1_bss_1894;
extern M555_CTRL *lbl_1_bss_3030;
extern M555_PLAYER lbl_1_bss_120C[2];
extern M555_ITEM lbl_1_bss_214[100];
extern M555_SPIN lbl_1_bss_11E4;
extern M555_MARK lbl_1_bss_B88[50];
extern s16 lbl_1_bss_B74[10];
extern M555_HOOK lbl_1_bss_11C8;



/* The 0x208-byte backdrop/scenery director at lbl_1_bss_C. */
typedef struct M555SceneRec_s {
    s16 active;          /* 0x00 */
    s16 state;           /* 0x02 */
    f32 f20;             /* 0x04 */
    f32 f24;             /* 0x08 */
} M555_SCENEREC;

typedef struct M555SceneRing_s {
    s16 on;              /* 0x0 */
    s16 hit;             /* 0x2 */
} M555_SCENERING;

typedef struct M555SceneBird_s {
    s16 on;              /* 0x00 */
    s16 state;           /* 0x02 */
    s16 pad4[2];         /* 0x04 */
    f32 f14C;            /* 0x08 */
    f32 f150;            /* 0x0C */
    f32 f154;            /* 0x10 */
    f32 f158;            /* 0x14 */
    f32 pad18[6];        /* 0x18 */
} M555_SCENEBIRD;

typedef struct M555Scene_s {
    s16 pad0[4];             /* 0x000 */
    s16 sw3;                 /* 0x008 */
    s16 timer3;              /* 0x00A */
    s16 used3[3];            /* 0x00C */
    s16 used4[4];            /* 0x012 */
    s16 pad1a;               /* 0x01A */
    M555_SCENEREC rec[12];   /* 0x01C */
    u8 padAC[0x60];          /* 0x0AC */
    s16 sw5;                 /* 0x10C */
    s16 cnt5;                /* 0x10E */
    s16 pad110[2];           /* 0x110 */
    M555_SCENERING ring[4];  /* 0x114 */
    s16 sw2;                 /* 0x124 */
    s16 pad126;              /* 0x126 */
    s16 sw6;                 /* 0x128 */
    s16 timer6;              /* 0x12A */
    u8 pad12C[0x10];         /* 0x12C */
    s16 used6[4];            /* 0x13C */
    M555_SCENEBIRD bird[4];  /* 0x144 */
    s16 sw1;                 /* 0x204 */
    s16 pad206;              /* 0x206 */
} M555_SCENE;

extern M555_SCENE lbl_1_bss_C;

/* ---- function prototypes (owner replaces its own `();` line with the real signature) ---- */
void fn_1_308();
void fn_1_33C();
void fn_1_368();
void fn_1_394();
void fn_1_3C0();
void fn_1_3EC();
void fn_1_418();
void fn_1_444();
void fn_1_464();
void fn_1_490();
void fn_1_494();
void fn_1_964();
void fn_1_EDC();
void fn_1_110C();
void fn_1_13E4();
void fn_1_150C();
s32 fn_1_1898(f32 *p, f32 frames, f32 from, f32 to);
void fn_1_19B8();
void fn_1_1AD4();
void fn_1_1B48();
void fn_1_1B74();
void fn_1_1B9C();
void fn_1_1BD4();
void fn_1_1FC0();
void fn_1_1FC4();
void fn_1_247C();
void fn_1_25D8();
void fn_1_2800();
void fn_1_28F0();
void fn_1_29E8();
void fn_1_2A18();
void fn_1_2A48();
void fn_1_2AB0();
void fn_1_2B10(s16 id);
void fn_1_2BD8(s16 a, s16 b, s16 c);
void fn_1_2CB0(s16 a, s16 b);
void fn_1_2D40(s16 idx, s16 pitch);
void fn_1_2DD0();
void fn_1_2E60(s16 a, s16 b);
void fn_1_2F28();
void fn_1_2FF0();
void fn_1_3010(M555_CTRL *ctrl);
s32 fn_1_69C4(M555_CTRL *ctrl);
s16 fn_1_6A0C(void);
s16 fn_1_6A60(void);
s32 fn_1_6AF0(void);
s16 fn_1_6DF0(void);
s16 fn_1_6ECC(void);
s16 fn_1_6F3C(void);
s32 fn_1_71B4(void);
s32 fn_1_7364(void);
void fn_1_73A4(void);
void fn_1_7840(void);
void fn_1_78DC(void);
void fn_1_7D88(s16 player);
void fn_1_8928(s16 player);
s32 fn_1_8F4C(s16 arg);
void fn_1_90C4();
void fn_1_9B8C();
void fn_1_A758(void);
void fn_1_CFB8(void);
s32 fn_1_FB2C(void);
s16 fn_1_FD64(void);
void fn_1_103D8(s16 player);
s16 fn_1_10E20(void);
void fn_1_119EC(void);
void fn_1_11DFC(s16 player);
void fn_1_12CAC(s16 player);
void fn_1_12E04(void);
s32 fn_1_130D8(void);
s32 fn_1_13430(void);
void fn_1_14420(void);
void fn_1_152D8(s16 arg);
void fn_1_155DC();
void fn_1_15CF4();
void fn_1_16104();
void fn_1_16958();
void fn_1_171C8();
void fn_1_17400();
void fn_1_176C4(s16 player);
void fn_1_19C0C();
void fn_1_1A4B0();
void fn_1_1A6F4();
void fn_1_1A880();
void fn_1_1A9A4();
void fn_1_1ABF0();
void fn_1_1AC04();
void fn_1_1AC24();
void fn_1_1AC44();
void fn_1_1AF38();
void fn_1_1B2F8();
void fn_1_1B4B0();
void fn_1_1B868();
void fn_1_1B86C();
void fn_1_1B870();
void fn_1_1B874();
void fn_1_1B878();
void fn_1_1B87C();
void fn_1_1B880();

#endif
