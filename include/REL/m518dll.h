#ifndef _M518DLL_H
#define _M518DLL_H

#include "dolphin.h"
#include "game/hu3d.h"
#include "game/process.h"
#include "humath.h"

/* Every work structure run by a process starts with this header. */
typedef struct M518ObjHead_s {
    s16 mode;
    s16 prevMode;
} M518_OBJHEAD;

/* A cell of the 12x12 grid. */
typedef struct M518Cell_s {
    s8 x;
    s8 z;
} M518_CELL;

/* Fixed size FIFO of directions, used by the path search of the COM. */
typedef struct M518Queue_s {
    s32 *data;
    s32 size;
    s32 tail;
    s32 head;
} M518_QUEUE;

/* The rolling block of a player (Thwomp for the solo player, block for the team). */
typedef struct M518Player_s {
    /* 0x00 */ s16 mode;
    /* 0x02 */ s16 prevMode;
    /* 0x04 */ s16 no;
    /* 0x06 */ s16 playerNo;
    /* 0x08 */ s16 type;
    /* 0x0A */ s16 charNo;
    /* 0x0C */ s16 modelId[2];
    /* 0x10 */ u8 pad10[2];
    /* 0x12 */ s16 kind;
    /* 0x14 */ s32 dir;
    /* 0x18 */ s16 step;
    /* 0x1A */ s16 stepMax;
    /* 0x1C */ s16 stepWait;
    /* 0x1E */ u8 pad1E[2];
    /* 0x20 */ HuVecF pos;
    /* 0x2C */ HuVecF rot;
    /* 0x38 */ HuVecF scale;
    /* 0x44 */ u8 pad44[0x18];
    /* 0x5C */ HuVecF moveDir;
    /* 0x68 */ HuVecF rotAxis;
    /* 0x74 */ HuVecF prevPos;
    /* 0x80 */ HuVecF prevRot;
    /* 0x8C */ HuVecF prevScale;
    /* 0x98 */ float tpLvl;
    /* 0x9C */ M518_CELL cell;
    /* 0x9E */ M518_CELL prevCell;
    /* 0xA0 */ s16 size;
    /* 0xA2 */ u8 padA2[2];
    /* 0xA4 */ s32 crushF;
    /* 0xA8 */ s16 time;
    /* 0xAA */ s16 delay;
    /* 0xAC */ s16 moveNum;
    /* 0xAE */ u8 padAE[2];
    /* 0xB0 */ s32 bounceF;
    /* 0xB4 */ s32 comF;
    /* 0xB8 */ s32 unkB8;
    /* 0xBC */ s32 unkBC;
    /* 0xC0 */ s16 dif;
    /* 0xC2 */ s16 unkC2;
    /* 0xC4 */ s16 comDir;
    /* 0xC6 */ s16 comWait;
    /* 0xC8 */ s16 comAxis;
    /* 0xCA */ s16 unkCA;
    /* 0xCC */ M518_CELL comTarget;
    /* 0xCE */ s16 padNo;
    /* 0xD0 */ s16 stkX;
    /* 0xD2 */ s16 stkY;
    /* 0xD4 */ s16 btnDown;
    /* 0xD6 */ u8 padD6[2];
    /* 0xD8 */ M518_QUEUE *path;
    /* 0xDC */ M518_QUEUE *pathTry[4];
    /* 0xEC */ HUPROCESS *process;
} M518_PLAYER;

/* The floor state: one word per cell (owner, flags, distance). */
typedef struct M518Map_s {
    /* 0x000 */ s16 mode;
    /* 0x002 */ s16 prevMode;
    /* 0x004 */ s16 modelId[20];
    /* 0x02C */ u8 pad2C[0x24];
    /* 0x050 */ u16 grid[12 * 12];
    /* 0x170 */ u16 gridSave[12 * 12];
    /* 0x290 */ u16 weight[12 * 12];
    /* 0x3B0 */ s16 unk3B0;
    /* 0x3B2 */ s16 maxDist;
    /* 0x3B4 */ HUPROCESS *process;
} M518_MAP;

/* A character model standing beside the field. */
typedef struct M518Char_s {
    /* 0x00 */ s16 mode;
    /* 0x02 */ s16 prevMode;
    /* 0x04 */ s16 playerNo;
    /* 0x06 */ s16 modelId;
    /* 0x08 */ s16 motId[7];
    /* 0x16 */ u8 pad16[2];
    /* 0x18 */ HuVecF pos;
    /* 0x24 */ HuVecF rot;
    /* 0x30 */ HuVecF target;
    /* 0x3C */ s32 comF;
    /* 0x40 */ s16 dif;
    /* 0x42 */ s16 charNo;
    /* 0x44 */ s16 time;
    /* 0x46 */ s16 delay;
    /* 0x48 */ HUPROCESS *process;
} M518_CHAR;

/* The winner podium. */
typedef struct M518Podium_s {
    /* 0x00 */ s16 mode;
    /* 0x02 */ s16 prevMode;
    /* 0x04 */ s16 modelId[3];
    /* 0x0A */ u8 padA[2];
    /* 0x0C */ HuVecF pos;
    /* 0x18 */ HUPROCESS *process;
} M518_PODIUM;

/* A member of the audience. */
typedef struct M518Crowd_s {
    /* 0x00 */ s16 mode;
    /* 0x02 */ s16 prevMode;
    /* 0x04 */ s16 no;
    /* 0x06 */ s16 type;
    /* 0x08 */ s16 modelId;
    /* 0x0A */ s16 motId;
    /* 0x0C */ HuVecF pos;
    /* 0x18 */ HuVecF rot;
    /* 0x24 */ HuVecF target;
    /* 0x30 */ HuVecF scale;
    /* 0x3C */ s16 time;
    /* 0x3E */ s16 delay;
    /* 0x40 */ HUPROCESS *process;
} M518_CROWD;

/* A floor tile. */
typedef struct M518Block_s {
    /* 0x00 */ s16 mode;
    /* 0x02 */ s16 prevMode;
    /* 0x04 */ s16 no;
    /* 0x06 */ s16 type;
    /* 0x08 */ s16 modelId;
    /* 0x0A */ s16 time;
    /* 0x0C */ s16 delay;
    /* 0x0E */ s16 playerNo;
    /* 0x10 */ M518_CELL cell;
    /* 0x12 */ u8 pad12[2];
    /* 0x14 */ HuVecF pos;
    /* 0x20 */ HuVecF rot;
    /* 0x2C */ float depth;
    /* 0x30 */ HUPROCESS *process;
} M518_BLOCK;

/* A dust ring particle effect. */
typedef struct M518Effect_s {
    /* 0x00 */ s32 activeF;
    /* 0x04 */ HU3DMODELID modelId;
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ ANIMDATA *anim;
    /* 0x0C */ HuVecF pos;
    /* 0x18 */ float radius;
} M518_EFFECT;

#endif
