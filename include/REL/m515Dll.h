#ifndef _REL_M515DLL_H
#define _REL_M515DLL_H

#include "version.h"
#include "dolphin.h"

#include "game/hu3d.h"

struct M515Play_s;
struct M515CamHook_s;

/* Doubly-linked list node, 0x14 bytes: model animation queue. */
typedef struct M515Node_s {
    s16 modelId;
    HU3DMOTID mtnId;
    s16 step;
    s16 pad2;
    s32 (*func)(struct M515Node_s *node);
    struct M515Node_s *prev;
    struct M515Node_s *next;
} M515_NODE;

/* One square of the 5x5 board, 0x28 bytes. */
typedef struct M515Cell_s {
    struct M515Cell_s *nextZ;
    struct M515Cell_s *prevZ;
    struct M515Cell_s *nextX;
    struct M515Cell_s *prevX;
    M515_NODE *piece;
    HuVecF pos;
    u16 type;
    u16 pad;
    s32 pad1;
} M515_CELL;

/* One moving piece on the board, 0x2C bytes. */
typedef struct M515Piece_s {
    HU3DMODELID modelId;
    s16 unk2;
    HU3DMOTID mtnId;
    HuVecF startPos;
    HuVecF endPos;
    s16 count;
    s16 step;
    f32 angle;
    s32 pad;
} M515_PIECE;

/* Camera orbit state, 0x30 bytes. */
typedef struct M515CamPos_s {
    HuVecF pos;
    HuVecF rot;
    f32 radius;
    s16 state;
    s16 timer;
    s16 camNo;
    s16 pad0;
    s16 unk24;
    s16 unk26;
    s16 camBit;
    s16 pad2;
    s32 unk2C;
} M515_CAMPOS;

/* Pad snapshot, 0x10 bytes. */
typedef struct M515Input_s {
    s16 state;
    s16 timer;
    M515_CAMPOS *camPos;
    s32 playerNo;
    s32 unk8;
} M515_INPUT;

/* Two on-screen sprites, 0x10 bytes. */
typedef struct M515Sprt_s {
    s16 espId[2];
    f32 width;
    f32 height;
    f32 unkC;
} M515_SPRT;

/* Per-player state, 0x6A0 bytes. */
typedef struct M515Play_s {
    s16 playerNo;
    s16 charNo;
    s16 padNo;
    s16 unk6;
    s16 unk8;
    s16 type;
    s16 dif;
    s16 side;

    M515_CELL cell[5][5];

    M515_CELL *cur;

    M515_NODE node3FC;
    s32 pad410;

    HuVecF unk414;
    s16 unk420;
    s16 pad422;

    M515_NODE node424;
    s32 pad438;

    HuVecF unk43C;
    s16 unk448;
    s16 pad44A;

    HuVecF pos;
    HuVecF rot;

    HuVecF pos0;
    HuVecF pos1;

    s16 stepA;
    s16 stepB;
    s16 frame;
    s16 mode;
    s16 tween;
    s16 rotStep;

    M515_PIECE piece[8];

    s16 pieceNum;

    HU3DMODELID mdlId[3][0x19];

    u16 state;
    s16 timer;

    M515_INPUT *cmd;
    s32 (*updateFunc)(M515_INPUT *cmd, struct M515Play_s *play);

    OMOBJ *obj;

    struct M515CamHook_s *hook;

    s32 pad694[3];
} M515_PLAY;

/* Shared game state, 0xCB0 bytes. */
typedef struct M515Work_s {
    s16 unk0;
    s16 boardType[3];

    HU3DMODELID mdlIdA;
    HU3DMODELID mdlIdB;
    HU3DMODELID mdlIdC;

    /* Board and prop model ids, plus the motion ids at 0xF0. Everything from
       here to the end of this run is 16-bit. The code splits it into separate
       groups that are each indexed off a player/board number, so the groups
       are declared individually rather than as one flat array. */
    s16 mdlId0E[4];
    s16 mdlId16[4];
    s16 mdlId1E[4];
    s16 mdlId26[4];
    s16 mdlId2E[4];
    s16 mdlId36[4];
    s16 mdlId3E[4];
    s16 mdlId46[4];
    s16 mdlId4E[4];
    s16 mdlId56[4];
    s16 mdlId5E[4][2];
    s16 mdlId6E[4];
    s16 mdlId76[4][4];
    s16 mdlId96[4][2];
    s16 mdlIdA6[4];
    s16 mdlIdAE[4];
    s16 mdlIdB6[3][3];
    s16 padC8[3];
    s16 mdlIdCE[9];
    s16 mdlIdE0[22];

    s16 mdlId10C[0x11];

    s16 pad12E[0x51];

    s16 unk1D0;
    s16 frame;
    s16 unk1D4;
    s16 winner;
    s16 unk1D8;
    s16 mode;

    s16 pauseFlag[10];

    s16 pad1E8[2];

    s32 unk1F4;

    /* Three shuffled-order lists of 6 board slots, one row per player.
       permIdx[j] counts how many entries of row j have been handed out. */
    s16 perm[3][6];
    s16 permIdx[3];

    M515_PLAY *player[4];

    M515_CAMPOS camPos[4];

    M515_INPUT input[4];

    M515_SPRT sprt;

    M515_NODE *unk344;
    M515_NODE *unk348;
    M515_NODE node[0x78];

    s16 unkCAC;
} M515_WORK;

#endif
