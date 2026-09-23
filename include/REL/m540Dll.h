#ifndef _M540DLL_H
#define _M540DLL_H

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
#include "game/mg/seqman.h"
#include "game/mg/timer.h"

#include "humath.h"

#include "datanum/m506.h"
#include "datanum/mgconst.h"

#include "stddef.h"

/*
 * Every game object of m540 is a process driven by a table of mode
 * functions. The generic part (0x000-0x017) is followed by a work area that
 * each kind of object uses for its own structure, then by the model part
 * (only allocated for model and character objects) and the player part (only
 * allocated for character objects).
 */

#ifdef TARGET_PC
typedef uintptr_t M540_WORKWORD;
#define M540_WORK_SIZE 0x300
#else
typedef u32 M540_WORKWORD;
#define M540_WORK_SIZE 0x140
#endif

typedef struct M540Obj_s M540_OBJ;
typedef void (*M540_OBJFUNC)(M540_OBJ *obj);

struct M540Obj_s {
    HUPROCESS *process;                                     /* 0x000 */
    int mode;                                               /* 0x004 */
    int prevMode;                                           /* 0x008 */
    int unkC;                                               /* 0x00C */
    M540_OBJFUNC *modeFunc;                                 /* 0x010 */
    int modeNum;                                            /* 0x014 */
    M540_WORKWORD work[M540_WORK_SIZE / sizeof(M540_WORKWORD)]; /* 0x018 */
    BOOL dispF;                                             /* 0x158 */
    int unk15C;                                             /* 0x15C */
    HuVecF pos;                                             /* 0x160 */
    HuVecF rot;                                             /* 0x16C */
    HuVecF scale;                                           /* 0x178 */
    HU3DMODELID modelId;                                    /* 0x184 */
    int motNo;                                              /* 0x188 */
    int motNum;                                             /* 0x18C */
    HU3DMOTID *motId;                                       /* 0x190 */
    u8 unk194[0x1C];                                        /* 0x194 */
    float tpLvl;                                            /* 0x1B0 */
    float motSpeed;                                         /* 0x1B4 */
    BOOL comF;                                              /* 0x1B8 */
    int charNo;                                             /* 0x1BC */
    int padNo;                                              /* 0x1C0 */
    int dif;                                                /* 0x1C4 */
    int playerNo;                                           /* 0x1C8 */
};

#define M540_OBJ_SIZE offsetof(M540_OBJ, dispF)
#define M540_MODELOBJ_SIZE offsetof(M540_OBJ, comF)
#define M540_CHAROBJ_SIZE sizeof(M540_OBJ)

#define M540_OBJWORK(type, obj) ((type *)(obj)->work)

typedef struct M540CameraView_s {
    HuVecF pos;    /* 0x00 */
    HuVecF up;     /* 0x0C */
    HuVecF center; /* 0x18 */
    HuVecF rot;    /* 0x24 */
    float zoom;    /* 0x30 */
} M540_CAMERAVIEW;

typedef struct M540Camera_s {
    int mode;                /* 0x00 */
    int time;                /* 0x04 */
    s16 cameraBit;           /* 0x08 */
    HUPROCESS *process;      /* 0x0C */
    M540_CAMERAVIEW view[3]; /* 0x10 */
    BOOL debugF;             /* 0xAC */
} M540_CAMERA;

typedef void (*M540_CAMERAFUNC)(M540_CAMERA *camera);

typedef struct M540Win_s {
    HUWINID winId;      /* 0x00 */
    float x;            /* 0x04 */
    float y;            /* 0x08 */
    BOOL dispF;         /* 0x0C */
    HUPROCESS *process; /* 0x10 */
} M540_WIN;

#endif
