#ifndef _M574DLL_H
#define _M574DLL_H

#include "game/charman.h"
#include "game/data.h"
#include "game/esprite.h"
#include "game/flag.h"
#include "game/gamemes.h"
#include "game/gamework.h"
#include "game/hsfformat.h"
#include "game/hsfex.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/printfunc.h"
#include "game/sprite.h"

#include "humath.h"

#define M574_PLAYER_MAX 4

/* Collision objects shared by collide.c, ball.c and stage.c. All pointers are
   typed so the layouts are also correct for 64-bit pointers. */
typedef struct M574Collider_s {
    /* 0x00 */ u32 flag;
    /* 0x04 */ s32 shape;
    /* 0x08 */ s32 id;
    /* 0x0C */ s32 kind;
    /* 0x10 */ void *user;
    /* 0x14 */ void (*hook)(void *);
    /* 0x18 */ HuVecF *pos;
    /* 0x1C */ HuVecF *vel;
    /* 0x20 */ HuVecF prevPos;
    /* 0x2C */ f32 radius;
} M574_COLLIDER;

typedef struct M574ColHead_s {
    /* 0x00 */ u32 flag;
    /* 0x04 */ s32 shape;
    /* 0x08 */ s32 id;
    /* 0x0C */ s32 kind;
} M574_COLHEAD;

typedef struct M574ColCircle_s {
    /* 0x00 */ u32 flag;
    /* 0x04 */ s32 shape;
    /* 0x08 */ s32 id;
    /* 0x0C */ s32 kind;
    /* 0x10 */ void *user;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 (*hook)(struct M574ColCircle_s *, M574_COLLIDER *);
    /* 0x1C */ HuVecF *pos;
    /* 0x20 */ f32 radius;
} M574_COL_CIRCLE;

typedef struct M574Edge_s {
    /* 0x00 */ HuVecF p0;
    /* 0x0C */ HuVecF p1;
    /* 0x18 */ Mtx *mtx;
} M574_EDGE;

typedef struct M574Plane_s {
    /* 0x00 */ HuVecF p0;
    /* 0x0C */ HuVecF p1;
    /* 0x18 */ HuVecF n;
    /* 0x24 */ f32 d;
} M574_PLANE;

typedef struct M574ColWall_s {
    /* 0x00 */ u32 flag;
    /* 0x04 */ s32 shape;
    /* 0x08 */ s32 id;
    /* 0x0C */ s32 kind;
    /* 0x10 */ void *user;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 (*hook)(struct M574ColWall_s *, M574_COLLIDER *);
    /* 0x1C */ s32 edgeNum;
    /* 0x20 */ M574_PLANE *planeA;
    /* 0x24 */ M574_PLANE *planeB;
    /* 0x28 */ M574_EDGE *edge;
    /* 0x2C */ s32 objNum;
    /* 0x30 */ HSFTRANSFORM **objTbl;
    /* 0x34 */ Mtx *mtxTbl;
    /* 0x38 */ M574_PLANE *lastPlane;
} M574_COL_WALL;

s32 fn_1_8AD0(M574_COLLIDER *col);
s32 fn_1_8B48(M574_COLHEAD *col);
void fn_1_8F54(M574_COL_WALL *wall, s16 model);
void fn_1_8FA8(M574_COL_WALL *wall, HSFOBJECT *obj);
void fn_1_8FDC(M574_COL_WALL *wall);
void *fn_1_70B0(s32 idx, HuVecF *pos, HuVecF *vel);
void fn_1_6C2C(HuVecF *pos, HuVecF *vel);

#endif
