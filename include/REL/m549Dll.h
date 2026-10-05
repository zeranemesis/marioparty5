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

/*
 * m549 is a board-game module: two characters walk on a track made of
 * "stepping stones" while rotating the board under them, trying to keep
 * the sky.  m549.c owns the flow / board / stone bookkeeping in its own
 * .bss; this header only describes the shapes game.c and its siblings
 * agree on, plus the stage object created by m549.c.
 */

/* One entry of the model table (stride 0x1C) at stage+0x58. */
typedef struct M549Model_s {
    HU3DMODELID modelId; /* 0x00 */
    u8 pad02[2];        /* 0x02 */
    HuVecF pos;          /* 0x04 */
    HuVecF rot;          /* 0x10 */
} M549_MODEL;

/* Motion work of one player (stride 0x54), at stage+0x74. */
typedef struct M549ModelWork_s {
    s16 mode;          /* 0x00 */
    u8 pad02[2];       /* 0x02 */
    float speed;       /* 0x04 */
    float speedOld;    /* 0x08 */
    float time;        /* 0x0C */
    float timeOld;     /* 0x10 */
    float maxTime;     /* 0x14 */
    u32 attrReset;     /* 0x18 */
    u32 attrOld;       /* 0x1C */
    u32 attr;          /* 0x20 */
    s32 endCheck;      /* 0x24 */
    s16 nextF;         /* 0x28 */
    s16 nextMotNo;     /* 0x2A */
    float nextTime;    /* 0x2C */
    u32 nextAttr;      /* 0x30 */
    u32 nextAttrReset; /* 0x34 */
    s16 shiftMode;     /* 0x38 */
    s16 shiftMotNo;    /* 0x3A */
    u32 shiftAttr;     /* 0x3C */
    float shiftSpeed;  /* 0x40 */
    float shiftTime;   /* 0x44 */
    float shiftTimeNow; /* 0x48 */
    float shiftTimeMax; /* 0x4C */
    u8 pad50[4];       /* 0x50 */
} M549_MODELWORK;

/* Per-player work of game.c (lbl_1_bss_3B0, stride 0xE8, 2 entries). */
typedef struct M549PlayerWork_s {
    s16 mode;        /* 0x00 */
    s16 padState;    /* 0x02 - second state machine (fn_1_9FF4 / fn_1_D0C4) */
    s16 unk4;        /* 0x04 */
    s16 attachMode;  /* 0x06 */
    s16 pad08;    /* 0x08 */
    s16 winMode;     /* 0x0A */
    s16 hitMode;     /* 0x0C */
    s16 slideMode;   /* 0x0E */
    s16 fallMode;    /* 0x10 */
    s16 timer;       /* 0x12 */
    s16 fired;       /* 0x14 */
    s16 pad16;    /* 0x16 */
    float stkX;      /* 0x18 */
    float stkY;      /* 0x1C */
    float scale;     /* 0x20 */
    char *hookName;  /* 0x24 */
    s16 winF;        /* 0x28 */
    s16 pad2A;    /* 0x2A */
    HuVecF pos;      /* 0x2C */
    HuVecF angle;    /* 0x38 */
    HuVecF vel;      /* 0x44 */
    s32 velXi;       /* 0x50 */
    s32 velYi;       /* 0x54 */
    s32 velZi;       /* 0x58 */
    s16 count;       /* 0x5C */
    s16 stepLevel;   /* 0x60 */
    s16 unk62;       /* 0x62 */
    float targetAng; /* 0x64 */
    s16 targetType;  /* 0x68 */
    s16 targetOn;    /* 0x6A */
    s16 targetType2; /* 0x6C */
    s16 grounded;    /* 0x6E */
    s16 charNo;      /* 0x70 */
    s16 unk72;       /* 0x72 */
    s16 unk74;       /* 0x74 */
    s16 unk76;       /* 0x76 */
    s16 unk78;       /* 0x78 */
    s16 hitCount;    /* 0x7A */
    s16 comMode;     /* 0x7C */
    s16 unk7E;       /* 0x7E */
    s16 unk80;       /* 0x80 */
    s16 unk82;       /* 0x82 */
    s16 stepCount;   /* 0x84 */
    HuVecF target;   /* 0x88 */
    HuVecF oldTarget;/* 0x94 */
    float chaseSpeed;/* 0xA0 */
    float chaseSpeedScale; /* 0xA4 */
    s16 stepMode;    /* 0xA8 */
    s16 stepWait;    /* 0xAA */
    s16 motionLock;  /* 0xAC */
    s16 inputLock;   /* 0xAE */
    s16 moveTime;    /* 0xB0 */
    s16 state;       /* 0xB2 */
    float distToTarget; /* 0xB4 */
    float distToCenter; /* 0xB8 */
    float faceAng;   /* 0xBC */
    float sideSpeed; /* 0xC0 */
    float speed;     /* 0xC4 */
    s16 chgSpeedPct[4]; /* 0xC8 */
    s16 chgSpeedMax[4]; /* 0xD0 */
    u32 btnStk;      /* 0xD8 */
    u32 btnStk2;     /* 0xDC */
    s32 padX;        /* 0xE0 */
    s32 padY;        /* 0xE4 */
} M549_PLAYERWORK;

/* One "cloud" of the rain system (lbl_1_bss_158, stride 0x3C, 10 entries). */
typedef struct M549CloudWork_s {
    s16 active;      /* 0x00 */
    s16 mode;        /* 0x02 */
    s16 phase;       /* 0x04 */
    s16 fired;       /* 0x06 */
    s16 playerNo;    /* 0x08 */
    s16 hitF;        /* 0x0A */
    float ang;       /* 0x0C */
    s16 stepLevel;   /* 0x10 */
    s16 pad12;    /* 0x12 */
    float radius;    /* 0x14 */
    float growTime;  /* 0x18 */
    float scaleZ;    /* 0x1C */
    float growSpeed; /* 0x20 */
    HuVecF pos;      /* 0x24 */
    HuVecF dir;      /* 0x30 */
} M549_CLOUDWORK;

/* One "block" / hole of the stage (lbl_1_bss_18, stride 0x20, 10 entries). */
typedef struct M549BlockWork_s {
    s16 active;    /* 0x00 */
    s16 unk2;      /* 0x02 */
    s16 unk4;      /* 0x04 */
    s16 unk6;      /* 0x06 */
    s16 unk8;      /* 0x08 */
    s16 unkA;      /* 0x0A */
    float unkC;    /* 0x0C */
    float unk10;   /* 0x10 */
    HuVecF pos;    /* 0x14 */
} M549_BLOCKWORK;

/* Flow / mode controller (lbl_1_bss_580, 0x80 bytes). */
typedef struct M549Flow_s {
    s16 mode;      /* 0x00 */
    s16 unk2;      /* 0x02 */
    s16 unk4;      /* 0x04 */
    s16 timer;     /* 0x06 */
    float ang;     /* 0x08 */
    u8 unk0C[0x14];/* 0x0C */
    s16 stopMode;  /* 0x20 */
    s16 stopF;     /* 0x22 */
    s16 winner;    /* 0x24 */
    u8 unk26[2];   /* 0x26 */
    s32 actorId;   /* 0x28 */
    u8 unk2C[0x3C];/* 0x2C */
    s16 moveMode[11]; /* 0x68 */
} M549_FLOW;

/* Module-wide work (lbl_1_bss_0, 0x0C bytes). */
typedef struct M549Work_s {
    HU3DMODELID colMdlId; /* 0x00 */
    MGPLAYER *player[2];  /* 0x04 */
} M549_WORK;

/* Per-player entry of the stage object (stride 0x1C, at stage+0x04). */
typedef struct M549PlayerEntry_s {
    s16 playerNo;   /* 0x00 */
    s16 charNo;     /* 0x02 */
    u8 pad04[4];    /* 0x04 */
    s32 pad08;      /* 0x08 */
    s32 pad0C;      /* 0x0C */
    u8 pad10[0x0C]; /* 0x10 */
} M549_PLAYERENTRY;

/* The stage object, shared with m549.c / stage.c. */
typedef struct M549Stage_s {
    s16 unk0;             /* 0x00 */
    s16 pad02;            /* 0x02 */
    M549_PLAYERENTRY player[2]; /* 0x04, stride 0x1C */
    u8 unk3C[0x1C];       /* 0x3C */
    M549_MODEL *model;    /* 0x58 */
    u8 unk5C[0x18];       /* 0x5C */
    M549_MODELWORK work[2]; /* 0x74, stride 0x54 */
    u8 unk11C[0x54];      /* 0x11C */
    s32 *dataIds;         /* 0x170, array of s32 model data ids */
    u8 unk174[4];         /* 0x174 */
    s32 unk178;           /* 0x178 */
} M549_STAGE;

/* Camera state shared with m549.c (lbl_1_bss_14 points at one of these). */
typedef struct M549Camera_s {
    u8 unk00[4];      /* 0x00 */
    HuVecF pos;        /* 0x04 */
    u8 unk10[0x0C];   /* 0x10 */
    HuVecF look;       /* 0x1C */
    u8 unk28[0x0C];   /* 0x28 */
    HuVecF target;     /* 0x34 */
    u8 unk40[0x2C];   /* 0x40 */
    HuVecF altTarget;  /* 0x6C */
    u8 unk78[0x0C];   /* 0x78 */
    HuVecF farTarget;  /* 0x84 */
} M549_CAMERA;

/* Look-at targets table owned by m549.c (lbl_1_data_288, 0xD0 bytes). */
extern M549_CAMERA *lbl_1_bss_14;
extern M549_WORK lbl_1_bss_0;
extern M549_FLOW lbl_1_bss_580;
extern M549_PLAYERWORK lbl_1_bss_3B0[2];
extern M549_CLOUDWORK lbl_1_bss_158[10];
extern M549_BLOCKWORK lbl_1_bss_18[10];
extern float lbl_1_data_288[];

#endif
