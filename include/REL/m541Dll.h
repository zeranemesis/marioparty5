#ifndef _M541DLL_H
#define _M541DLL_H

#include "game/object.h"
#include "game/process.h"
#include "game/memory.h"
#include "game/hu3d.h"
#include "game/data.h"
#include "game/window.h"
#include "game/wipe.h"
#include "game/gamemes.h"
#include "game/gamework.h"
#include "game/charman.h"
#include "game/pad.h"
#include "game/audio.h"
#include "game/frand.h"
#include "game/sprite.h"
#include "game/esprite.h"
#include "game/flag.h"
#include "game/mg/actman.h"
#include "game/mg/seqman.h"
#include "game/mg/timer.h"

#include "humath.h"

#include "stddef.h"

/*
 * m541 is the "Battle mini-game" board module: two characters move their
 * markers around a board of leaves while the other board game elements run
 * from m541.c.  The stage object (a plain 0x2E8 byte block created by
 * m541.c's fn_1_19AB0) carries the model table, the per-player motion
 * work areas and the pad state; everything else lives in the module's
 * own .bss, which is described by the M541_* structures below.
 */

/* One entry of the model table (stride 0x1C). */
typedef struct M541Model_s {
    HU3DMODELID modelId; /* 0x00 */
    float posX;          /* 0x04 */
    float posY;          /* 0x08 */
    float posZ;          /* 0x0C */
    float rotX;          /* 0x10 */
    float rotY;          /* 0x14 */
    float rotZ;          /* 0x18 */
} M541_MODEL;

/* Motion work of one model (stride 0x54), at stage+0x74. */
typedef struct M541ModelWork_s {
    s16 motNo;      /* 0x00 */
    s16 motNoPrev;  /* 0x02 */
    float speed;    /* 0x04 */
    float speedOld; /* 0x08 */
    float time;     /* 0x0C */
    float timeOld;  /* 0x10 */
    float maxTime;  /* 0x14 */
    u32 attrReset;  /* 0x18 */
    u32 attrOld;    /* 0x1C */
    u32 attr;       /* 0x20 */
    s32 endCheck;   /* 0x24 */
    s16 nextF;      /* 0x28 */
    s16 nextMotNo;  /* 0x2A */
    float nextTime; /* 0x2C */
    u32 nextAttr;   /* 0x30 */
    u32 nextAttrReset; /* 0x34 */
    s16 shiftMode;  /* 0x38 */
    s16 shiftMotNo; /* 0x3A */
    u32 shiftAttr;  /* 0x3C */
    float shiftSpeed; /* 0x40 */
    float shiftTime;  /* 0x44 */
    float shiftTimeNow; /* 0x48 */
    float shiftTimeMax; /* 0x4C */
    u8 pad50[4];     /* 0x50 */
} M541_MODELWORK;

/* Pad / per-player part of the stage object (stride 0x1C, at stage+0x04). */
typedef struct M541Player_s {
    s16 playerNo;   /* 0x04 */
    s16 charNo;     /* 0x06 */
    s16 dif;        /* 0x08 */
    s16 padNo;      /* 0x0A */
    u32 btn;        /* 0x0C */
    u32 btnDown;    /* 0x10 */
    s8 stkX;        /* 0x14 */
    u16 btnRep;     /* 0x16 */
    s8 stkY;        /* 0x18 */
    s8 subStkX;     /* 0x19 */
    s8 subStkY;     /* 0x1A */
    s8 trigL;       /* 0x1B */
    s8 trigR;       /* 0x1C */
    s8 dStk;        /* 0x1D */
    s8 dStkRep;     /* 0x1E */
} M541_PLAYER;

/* The stage object itself, 0x2E8 bytes, shared with stage.c. */
typedef struct M541Stage_s {
    s16 unk0;            /* 0x00 */
    s16 unk2;            /* 0x02 */
    M541_PLAYER player[2]; /* 0x04, stride 0x1C */
    u8 unk3C[0x1C];      /* 0x3C */
    M541_MODEL *model;   /* 0x58 */
    u8 unk5C[0x18];      /* 0x5C */
    M541_MODELWORK work[2]; /* 0x74, stride 0x54 */
    u8 unk11C[0x54];     /* 0x11C */
    s32 *dataIds;        /* 0x170, array of s32 model data ids */
    u8 unk174Pad[4];     /* 0x174 */
    s32 unk178;          /* 0x178 */
} M541_STAGE;

/* Per-player work of game.c (lbl_1_bss_2614, stride 0x90). */
typedef struct M541PlayerWork_s {
    s16 mode;        /* 0x00 */
    s16 sub;         /* 0x02 */
    s16 motion;      /* 0x04 */
    u8 pad06[2];     /* 0x06 */
    s16 tick;        /* 0x08 */
    s16 unk8;        /* 0x0A */
    void *hook;      /* 0x0C */
    s16 anim2;       /* 0x10 */
    s16 anim;        /* 0x12 */
    float tpLvl;     /* 0x14 */
    u8 pad18[4];     /* 0x18 */
    HuVecF pos;      /* 0x1C */
    float velX;      /* 0x28 */
    float velY;      /* 0x2C */
    float velZ;      /* 0x30 */
    s16 stunMode;    /* 0x34 */
    float stunTime;  /* 0x38 */
    s16 waitMot;     /* 0x3C */
    s16 waitCount;   /* 0x3E */
    s16 otherMode;   /* 0x40 */
    u8 pad42[4];     /* 0x42 */
    s16 otherSub;    /* 0x46 */
    s16 chaseMode;   /* 0x48 */
    s16 chaseSub;    /* 0x4A */
    u8 pad4C[4];     /* 0x4C */
    HuVecF chasePos; /* 0x50 */
    float chaseVel;  /* 0x5C */
    float chaseRadius; /* 0x60 */
    float timerScale; /* 0x64 */
    s16 targetMot;   /* 0x68 */
    s16 targetTime;  /* 0x6A */
    s16 targetPos;   /* 0x6C */
    s16 targetIdx;   /* 0x6E */
    HuVecF targetPosV; /* 0x70 */
    float targetDist; /* 0x7C */
    s32 padStkX;    /* 0x80 */
    s32 padStkY;    /* 0x84 */
    s32 padBtnDown; /* 0x88 */
    s32 padBtn;     /* 0x8C */
} M541_PLAYERWORK;

/* Leaf work (lbl_1_bss_20, stride 0x30). */
typedef struct M541LeafWork_s {
    s16 unk0;      /* 0x00 */
    s16 exist;     /* 0x02 */
    s16 state;     /* 0x04 */
    u8 pad06[6];   /* 0x06 */
    float tpLvl;   /* 0x0C */
    s16 hidden;    /* 0x10 */
    u8 pad12[2];   /* 0x12 */
    HuVecF vel;    /* 0x14 */
    float angle;   /* 0x2C */
} M541_LEAFWORK;

/* Board / path control (lbl_1_bss_2F0). */
typedef struct M541Board_s {
    s16 mode;      /* 0x00 */
    s16 unk2;      /* 0x02 */
    s16 ready;     /* 0x04 */
    s16 timer;     /* 0x06 */
    s16 count;     /* 0x08 */
    s16 nextLeaf[10]; /* 0x0A, stride 2 */
    u8 pad1E[2];   /* 0x1E */
    HuVecF unk20[10]; /* 0x20, stride 0xC */
    HuVecF scaleB[10]; /* 0x98, stride 0xC */
    float dist[10];  /* 0x110, stride 4 */
    float angle[10]; /* 0x138, stride 4 */
    float length;    /* 0x160 */
    s16 spawnMode;  /* 0x164 */
    s16 spawnTimer; /* 0x166 */
    s16 spawnCount; /* 0x168 */
    u8 pad16A[2];   /* 0x16A */
} M541_BOARD;

/* Shared flow / mode state (lbl_1_bss_2734). */
typedef struct M541Flow_s {
    s16 mode;      /* 0x00 */
    s16 unk2;      /* 0x02 */
    s16 unk4;      /* 0x04 */
    s16 unk6;      /* 0x06 */
    float unk8;    /* 0x08 */
    u8 unkC[0x10]; /* 0x0C */
    float unk1C;   /* 0x1C */
    s16 cpuPlayer; /* 0x20 */
    u8 pad22[2];   /* 0x22 */
    u32 timer;     /* 0x24 */
    float unk28;   /* 0x28 */
    float moveX;   /* 0x2C */
    u8 pad30[4];   /* 0x30 */
    float moveZ;   /* 0x34 */
    HuVecF goal;   /* 0x38 */
    float velX;    /* 0x44 */
    float velY;    /* 0x48 */
    float velZ;    /* 0x4C */
    float angVelX; /* 0x50 */
    float angVelY; /* 0x54 */
    float angVelZ; /* 0x58 */
    s16 angCount;  /* 0x5C */
    float angAcc;  /* 0x60 */
    u8 pad64[0x20]; /* 0x64 */
    s16 playerHook[4]; /* 0x84, stride 2 */
} M541_FLOW;

/* AI path table entry (lbl_1_bss_45C). */
typedef struct M541Path_s {
    s16 unk0;      /* 0x00 */
    s16 count;     /* 0x02 */
    s16 mode;      /* 0x04 */
    s16 num;       /* 0x06 */
    float radius;  /* 0x08 */
    float pad14;   /* 0x14 */
    float pad18;   /* 0x18 */
} M541_PATH;

#endif