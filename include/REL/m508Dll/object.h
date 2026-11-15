#ifndef _M508DLL_OBJECT_H
#define _M508DLL_OBJECT_H

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

#include "humath.h"

#include "stddef.h"

/* ---- 2D collision helpers (fn_1_A194..fn_1_B514) ---- */

typedef struct M508Vec2_s {
    float x; /* 0x0 */
    float y; /* 0x4 */
} M508_VEC2;

typedef struct M508Circle_s {
    float x; /* 0x0 */
    float y; /* 0x4 */
    float r; /* 0x8 */
} M508_CIRCLE;

typedef struct M508Rect_s { /* also a line segment: (x1,y1)-(x2,y2) */
    float x1; /* 0x0 */
    float y1; /* 0x4 */
    float x2; /* 0x8 */
    float y2; /* 0xC */
} M508_RECT;

typedef struct M508Poly_s {
    int num;          /* 0x0 */
    M508_VEC2 *point; /* 0x4 */
} M508_POLY;

typedef struct M508Sphere_s {
    float x; /* 0x0 */
    float y; /* 0x4 */
    float z; /* 0x8 */
    float r; /* 0xC */
} M508_SPHERE;

/* ---- Game object: a process driven by a table of mode functions ---- */
/*
 * The generic part (0x000-0x017) is followed by a work area that each kind
 * of object uses for its own structure, then by the model part (only
 * allocated for model and character objects) and the player part (only
 * allocated for character objects).
 * On PC the work area is widened so that pointer-bearing work structs fit;
 * code in m508.c must reach it through M508_OBJWORK, never by GC offsets.
 */

#ifdef TARGET_PC
typedef uintptr_t M508_WORKWORD;
#define M508_WORK_SIZE 0x300
#else
typedef u32 M508_WORKWORD;
#define M508_WORK_SIZE 0x140
#endif

typedef struct M508Obj_s M508_OBJ;
typedef void (*M508_OBJFUNC)(M508_OBJ *obj);

struct M508Obj_s {
    HUPROCESS *process;                                         /* 0x000 */
    int mode;                                                   /* 0x004 */
    int prevMode;                                               /* 0x008 */
    int unkC;                                                   /* 0x00C */
    M508_OBJFUNC *modeFunc;                                     /* 0x010 */
    int modeNum;                                                /* 0x014 */
    M508_WORKWORD work[M508_WORK_SIZE / sizeof(M508_WORKWORD)]; /* 0x018 */
    BOOL dispF;                                                 /* 0x158 */
    int unk15C;                                                 /* 0x15C */
    HuVecF pos;                                                 /* 0x160 */
    HuVecF rot;                                                 /* 0x16C */
    HuVecF scale;                                               /* 0x178 */
    HU3DMODELID modelId;                                        /* 0x184 */
    int motNo;                                                  /* 0x188 */
    int motNum;                                                 /* 0x18C */
    HU3DMOTID *motId;                                           /* 0x190 */
    u8 unk194[0x1C];                                            /* 0x194 */
    float tpLvl;                                                /* 0x1B0 */
    float motSpeed;                                             /* 0x1B4 */
    BOOL comF;                                                  /* 0x1B8 */
    int charNo;                                                 /* 0x1BC */
    int padNo;                                                  /* 0x1C0 */
    int dif;                                                    /* 0x1C4 */
    int playerNo;                                               /* 0x1C8 */
};

#define M508_OBJ_SIZE offsetof(M508_OBJ, dispF)
#define M508_MODELOBJ_SIZE offsetof(M508_OBJ, comF)
#define M508_CHAROBJ_SIZE sizeof(M508_OBJ)

#define M508_OBJWORK(type, obj) ((type *)(obj)->work)

/* ---- Camera ---- */

typedef struct M508CameraView_s {
    HuVecF pos;    /* 0x00 */
    HuVecF up;     /* 0x0C */
    HuVecF center; /* 0x18 */
    HuVecF rot;    /* 0x24 */
    float zoom;    /* 0x30 */
} M508_CAMERAVIEW;

typedef struct M508Camera_s {
    int mode;                /* 0x00 */
    int time;                /* 0x04 */
    s16 cameraBit;           /* 0x08 */
    HUPROCESS *process;      /* 0x0C */
    M508_CAMERAVIEW view[3]; /* 0x10 */
    BOOL debugF;             /* 0xAC */
} M508_CAMERA;

typedef void (*M508_CAMERAFUNC)(M508_CAMERA *camera);

/* ---- Window ---- */

typedef struct M508Win_s {
    HUWINID winId;      /* 0x00 */
    float x;            /* 0x04 */
    float y;            /* 0x08 */
    BOOL dispF;         /* 0x0C */
    HUPROCESS *process; /* 0x10 */
} M508_WIN;

#ifdef __cplusplus
extern "C" {
#endif

float fn_1_A194(M508_VEC2 *vec);
void fn_1_A314(M508_VEC2 *dst, M508_VEC2 *src);
BOOL fn_1_A4DC(M508_VEC2 *point, M508_CIRCLE *circle);
BOOL fn_1_A564(M508_CIRCLE *a, M508_CIRCLE *b);
BOOL fn_1_A5F4(M508_RECT *a, M508_RECT *b);
BOOL fn_1_A65C(M508_VEC2 *point, M508_POLY *poly);
BOOL fn_1_A74C(M508_CIRCLE *circle, M508_RECT *line);
BOOL fn_1_AD98(M508_CIRCLE *circle, M508_POLY *poly);
BOOL fn_1_AF28(M508_RECT *rect, M508_CIRCLE *circle);
BOOL fn_1_B1C0(M508_SPHERE *a, M508_SPHERE *b);
BOOL fn_1_B274(M508_CIRCLE *push, M508_CIRCLE *fixed);
BOOL fn_1_B514(M508_CIRCLE *push, M508_RECT *line);

M508_OBJ *fn_1_BC7C(int prio, u32 stackSize, int funcNum, M508_OBJFUNC *funcTbl);
void fn_1_BDB0(M508_OBJ *obj);
void fn_1_BE58(M508_OBJ *obj);
M508_OBJ *fn_1_BE70(int prio, u32 stackSize, u32 dataNum, int motNum, u32 *motTbl, int funcNum, M508_OBJFUNC *funcTbl);
void fn_1_C0F4(M508_OBJ *obj);
void fn_1_C194(M508_OBJ *obj, float x, float y, float z);
void fn_1_C1E8(M508_OBJ *obj, float *x, float *y, float *z);
void fn_1_C228(M508_OBJ *obj, float x, float y, float z);
void fn_1_C27C(M508_OBJ *obj, float *x, float *y, float *z);
void fn_1_C2BC(M508_OBJ *obj, float x, float y, float z);
void fn_1_C310(M508_OBJ *obj, float *x, float *y, float *z);
void fn_1_C350(M508_OBJ *obj, float tpLvl);
float fn_1_C38C(M508_OBJ *obj);
void fn_1_C394(M508_OBJ *obj);
void fn_1_C3D0(M508_OBJ *obj);
BOOL fn_1_C40C(M508_OBJ *obj);
void fn_1_C414(M508_OBJ *obj, int time);
void fn_1_C470(M508_OBJ *obj, BOOL loopF);
void fn_1_C4E8(M508_OBJ *obj);
BOOL fn_1_C520(M508_OBJ *obj);
void fn_1_C570(M508_OBJ *obj, int motNo, float start, float end, u32 attr);
int fn_1_C5EC(M508_OBJ *obj);
void fn_1_C5F4(M508_OBJ *obj, float speed);
void fn_1_C630(M508_OBJ *obj);
void fn_1_C660(M508_OBJ *obj);
void fn_1_C690(M508_OBJ *obj);
void fn_1_C6C0(M508_OBJ *obj);
void fn_1_C6C4(M508_OBJ *obj, char *objName, M508_OBJ *hookObj);
void fn_1_C70C(M508_OBJ *obj);
M508_OBJ *fn_1_C73C(int prio, u32 stackSize, int playerNo, int motNum, u32 *motTbl, int funcNum, M508_OBJFUNC *funcTbl, s16 modelNo);
void fn_1_CA2C(M508_OBJ *obj);
void fn_1_CACC(M508_OBJ *obj, char *objName);

M508_CAMERA *fn_1_CB24(s16 cameraBit);
void fn_1_CD14(M508_CAMERA *camera);
void fn_1_CDB0(M508_CAMERA *camera);
void fn_1_D028(M508_CAMERA *camera);
void fn_1_D098(M508_CAMERA *camera);
void fn_1_D0A4(M508_CAMERA *camera);
void fn_1_D158(M508_CAMERA *camera);
void fn_1_D164(M508_CAMERA *camera);
BOOL fn_1_D22C(M508_CAMERA *camera);
void fn_1_D25C(M508_CAMERA *camera, HuVecF *center, HuVecF *rot, float *zoom, int time);
void fn_1_D6A0(M508_CAMERA *camera);
void fn_1_D9AC(M508_CAMERA *camera, float vpX, float vpY, float vpW, float vpH, float vpNearZ, float vpFarZ);
void fn_1_DA0C(M508_CAMERA *camera, float fov, float near, float far, float aspect);
void fn_1_DA5C(M508_CAMERA *camera, float scissorX, float scissorY, float scissorW, float scissorH);
void fn_1_DAF0(M508_CAMERA *camera);
void fn_1_DF6C(M508_CAMERA *camera);

void fn_1_DF70(int time, BOOL waitF);
void fn_1_DFCC(int time, BOOL waitF);
void fn_1_E028(int time);
void fn_1_E0B4(int time);
float fn_1_E140(float start, float end, float t);

M508_WIN *fn_1_E244(int w, int h);
void fn_1_E2EC(M508_WIN *win);
void fn_1_E340(M508_WIN *win, float x, float y);
void fn_1_E388(M508_WIN *win, float *x, float *y);
void fn_1_E39C(M508_WIN *win, u32 mess);
void fn_1_E3D4(M508_WIN *win);
void fn_1_E40C(M508_WIN *win);
void fn_1_E444(M508_WIN *win, int color);
void fn_1_E480(M508_WIN *win, s16 speed);
void fn_1_E4B8(M508_WIN *win);
void fn_1_E4E8(M508_WIN *win);
void fn_1_E518(M508_WIN *win);
void fn_1_E548(M508_WIN *win, float scaleX, float scaleY);

#ifdef __cplusplus
}
#endif

#endif
