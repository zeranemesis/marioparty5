#ifndef _M538DLL_H
#define _M538DLL_H

#include "game/object.h"
#include "game/process.h"
#include "game/memory.h"
#include "game/hu3d.h"
#include "game/data.h"
#include "game/charman.h"
#include "game/pad.h"
#include "game/audio.h"
#include "game/frand.h"
#include "game/flag.h"
#include "game/gamework.h"
#include "game/hsfex.h"
#include "game/mg/seqman.h"

#include "humath.h"
#include "dolphin.h"

#include <math.h>
#include <stddef.h>

typedef struct M538_s M538;
typedef struct M538Player_s M538_PLAYER;
typedef struct M538Node_s M538_NODE;
typedef struct M538Anim_s M538_ANIM;
typedef struct M538CamWork_s M538_CAMWORK;
typedef struct M538FwWork_s M538_FW;
typedef struct M538Node0_s M538_NODE0;

/* shared firework work record: model, orbit, and a small state machine.
 * the same shape is embedded inside a player (0x48) and instantiated
 * four times inside the module controller. */
struct M538FwWork_s {
    float x;       /* 0x00 */
    float y;       /* 0x04 */
    float z;       /* 0x08 */
    float rotX;    /* 0x0C */
    float radius2; /* 0x10 second orbit radius */
    float radius3; /* 0x14 third orbit radius */
    float angle;   /* 0x18 */
    float radius;  /* 0x1C */
    float dist;    /* 0x20 live orbit distance */
    float pad24;   /* 0x24 */
    s16 model;     /* 0x28 */
    s16 state;     /* 0x2A */
    s16 timer;     /* 0x2C */
    s16 pad2E;     /* 0x2E */
}; /* 0x30 */

typedef s32 (*M538_NODE0FUNC)(M538_NODE0 *node);

/* first, lighter pool node: just prev/next/func/user */
struct M538Node0_s {
    M538_NODE0 *prev;        /* 0x00 */
    M538_NODE0 *next;        /* 0x04 */
    M538_NODE0FUNC func;     /* 0x08 */
    void *work;              /* 0x0C */
}; /* 0x10 */

typedef struct M538Pool_s {
    M538_NODE *head; /* 0x00 active list head */
    M538_NODE *free; /* 0x04 free list head */
} M538_POOL;

typedef s32 (*M538_NODEFUNC)(M538_NODE *node);

/* effect pool node used for smoke, footstep and impact marks */
struct M538Node_s {
    M538_NODE *prev;     /* 0x00 */
    M538_NODE *next;     /* 0x04 */
    M538_POOL *pool;     /* 0x08 */
    M538_NODEFUNC func;  /* 0x0C */
    float x;             /* 0x10 */
    float y;             /* 0x14 */
    float z;             /* 0x18 */
    float rotX;          /* 0x1C */
    float rotY;          /* 0x20 */
    float rotZ;          /* 0x24 */
    float angle;         /* 0x28 */
    float unk2C;         /* 0x2C */
    s16 timer;           /* 0x30 */
    s16 pad32;           /* 0x32 */
    s16 model;           /* 0x34 */
    s16 pad36;           /* 0x36 */
}; /* 0x38 */

/* camera orbit state: yaw/pitch around a focus point plus a radius */
struct M538CamWork_s {
    float x;       /* 0x00 focus */
    float y;       /* 0x04 */
    float z;       /* 0x08 */
    float theta;   /* 0x0C */
    float phi;     /* 0x10 */
    float radius;  /* 0x14 */
    float from[6]; /* 0x18..0x2C interpolation start values */
    s16 state;     /* 0x30 */
    s16 counter;   /* 0x32 */
}; /* 0x34 */

typedef s32 (*M538_ANIMFUNC)(M538_ANIM *anim);

struct M538Anim_s {
    float scale;         /* 0x00 */
    s32 flag;            /* 0x04 */
    M538_ANIMFUNC func;  /* 0x08 */
    s16 state;           /* 0x0C */
    s16 counter;         /* 0x0E */
    s16 mode;            /* 0x10 */
    s16 model[3];        /* 0x12 0x14 0x16 */
}; /* 0x18 */

/* small hook-driven camera swing block at 0x294 */
typedef struct M538CamSwing_s {
    float pad00[6];  /* 0x00..0x17 */
    float radius;    /* 0x18 */
    float pad1C;     /* 0x1C */
    float unk20;     /* 0x20 */
    s16 state;       /* 0x24 */
    s16 counter;     /* 0x26 */
    s16 model[4];    /* 0x28 0x2A 0x2C 0x2E */
} M538_CAMSWING; /* 0x30 */

typedef s32 (*M538_PLAYERFUNC)(M538_PLAYER *p);
typedef s32 (*M538_WORKFUNC)(void *work);

struct M538Player_s {
    float x;       /* 0x00 */
    float y;       /* 0x04 */
    float z;       /* 0x08 */
    float rotX;    /* 0x0C */
    float rotY;    /* 0x10 */
    float rotZ;    /* 0x14 */
    float velX;    /* 0x18 */
    float velY;    /* 0x1C */
    float velZ;    /* 0x20 */
    float accX;    /* 0x24 */
    float accY;    /* 0x28 */
    float accZ;    /* 0x2C */
    float hookX;   /* 0x30 raw word in the original; keep float, fn_1_7FAC needs lfs */
    float hookY;   /* 0x34 */
    float hookZ;   /* 0x38 */
    float hookRotX;/* 0x3C */
    float hookRotY;/* 0x40 */
    float hookRotZ;/* 0x44 */
    M538_FW fw;    /* 0x48 embedded firework work */
    s16 itemModel; /* 0x78 */
    s16 pad7A;     /* 0x7A */
    s16 fwModel;   /* 0x7C */
    s16 pad7E;     /* 0x7E */
    M538_WORKFUNC voiceHook; /* 0x80 */
    M538_PLAYERFUNC func;    /* 0x84 */
    s32 state;               /* 0x88 */
    u8 pad8C[0x18];          /* 0x8C..0xA3 */
    float aimRot;            /* 0xA4 */
    float aimDist;           /* 0xA8 */
    u8 padAC[0x10];          /* 0xAC..0xBB */
    float targetRot;         /* 0xBC */
    float rotSpeed;          /* 0xC0 */
    OMOBJ *obj;              /* 0xC4 */
    M538_POOL *pool1;        /* 0xC8 */
    M538_POOL *pool2;        /* 0xCC */
    M538_PLAYER *aiTarget;   /* 0xD0 */
    s16 aiState;             /* 0xD4 */
    s16 aiTimer;             /* 0xD6 */
    s16 aiMaxTimer;          /* 0xD8 */
    s16 aiFlag;              /* 0xDA */
    s32 sfxId;               /* 0xDC */
    s16 voicePan;            /* 0xE0 */
    s16 voiceVol;            /* 0xE2 */
    s16 counter;             /* 0xE4 */
    s16 flag;                /* 0xE6 */
    s16 timer;               /* 0xE8 */
    s16 place;               /* 0xEA */
    s16 active;              /* 0xEC */
    s16 modelId;             /* 0xEE */
    s16 playerNo;            /* 0xF0 */
    s16 charNo;              /* 0xF2 */
    s16 grp;                 /* 0xF4 */
    s16 padNo;               /* 0xF6 */
    s16 padF8;               /* 0xF8 */
    s16 padFA;               /* 0xFA */
    s16 type;                /* 0xFC */
    s16 dif;                 /* 0xFE */
    s16 counter2;            /* 0x100 */
    s16 counter3;            /* 0x102 */
}; /* 0x104 */

typedef struct M538Ctrl_s {
    u8 pad00[0x10];  /* 0x00 */
    M538_FW work[4]; /* 0x10 0x40 0x70 0xA0 */
    s16 state;       /* 0xD0 */
    s16 timer;       /* 0xD2 */
} M538_CTRL;

/* camera dolly state machine work used by the fn_1_A424 hook */
typedef struct M538Dolly_s {
    s16 state;   /* 0x00 */
    s16 counter; /* 0x02 */
} M538_DOLLY;

/* first-fall/work used by the fn_1_B0D0 hook */
typedef struct M538Fall_s {
    float unk0; /* 0x00 */
    s16 model;  /* 0x04 */
    s16 state;  /* 0x06 */
    s16 timer;  /* 0x08 */
    s16 pad0A;  /* 0x0A */
    s16 pad0C;  /* 0x0C */
    s16 pad0E;  /* 0x0E */
} M538_FALL; /* 0x10 */

struct M538_s {
    M538_PLAYER *player[4];   /* 0x000 */
    M538_NODE0 *node0Head;    /* 0x010 */
    M538_NODE0 *node0Free;    /* 0x014 */
    M538_NODE0 node0[0x20];   /* 0x018 */

    M538_CAMWORK cam;         /* 0x218 */

    M538_ANIM anim[3];        /* 0x24C 0x264 0x27C */
    M538_CAMSWING swing;      /* 0x294 */

    M538_POOL pool1;          /* 0x2C4 */
    M538_POOL pool2;          /* 0x2CC */
    M538_POOL pool3;          /* 0x2D4 */
    M538_NODE node1[0x10];    /* 0x2DC */
    M538_NODE node2[0x10];    /* 0x65C */
    M538_NODE node3[0x40];    /* 0x9DC */

    M538_DOLLY dolly;         /* 0x17DC */
    M538_FALL fall;           /* 0x17E0 */
    M538_WORKFUNC dollyHook;  /* 0x17F0 */
    M538_CTRL ctrl;           /* 0x17F4 */

    s16 lightId;              /* 0x18C8 */
    s16 padCA;                /* 0x18CA */
    float lightRadius;        /* 0x18CC */
    s16 fireCount;            /* 0x18D0 */
    s16 loseF;                /* 0x18D2 */
    s16 tick;                 /* 0x18D4 */
    s16 state;                /* 0x18D6 */
    s16 timer;                /* 0x18D8 */
    s16 state2;               /* 0x18DA */
    s16 activeF;              /* 0x18DC */
    s16 models[0x1D];         /* 0x18DE */
    s16 camModels[3];         /* 0x1918 */
    s16 roundEndF;            /* 0x191E */
}; /* 0x1920 */

#define M538_SIZE sizeof(M538)

extern OMOBJ *lbl_1_bss_C;

#endif