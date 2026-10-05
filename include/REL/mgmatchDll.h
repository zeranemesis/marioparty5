#ifndef _MGMATCHDLL_H
#define _MGMATCHDLL_H

#include "game/charman.h"
#include "game/data.h"
#include "game/esprite.h"
#include "game/flag.h"
#include "game/frand.h"
#include "game/gamemes.h"
#include "game/gamework.h"
#include "game/hsfex.h"
#include "game/hu3d.h"
#include "game/mg/timer.h"
#include "game/mgdata.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/process.h"
#include "game/sprite.h"
#include "game/thpmain.h"
#include "game/window.h"
#include "game/wipe.h"

#ifndef __MWERKS__
#include "game/audio.h"
#endif

#define MGMATCH_PLAYER_MAX 4
#define MGMATCH_MG_MAX 21
#define MGMATCH_MGTBL_MAX 43
#define MGMATCH_SE_MAX 10
#define MGMATCH_ITEM_MAX 7
#define MGMATCH_MDL_MAX 61
#define MGMATCH_PARMAN_MAX 7
#define MGMATCH_CAMPATH_MAX 50

/* One of the four character slots. Stride 0x34. */
typedef struct M505Player_s {
    HU3DMODELID modelId;   //0x00
    HU3DMOTID mot0;        //0x02
    HU3DMOTID mot1;        //0x04
    HU3DMOTID mot2;        //0x06
    HU3DMOTID mot3;        //0x08
    HU3DMOTID mot4;        //0x0A
    HU3DMOTID mot5;        //0x0C
    HU3DMOTID mot6;        //0x0E
    HU3DMOTID mot7;        //0x10
    HU3DMOTID mot8;        //0x12
    s16 pad1;              //0x14
    s16 pad2;              //0x16
    s16 playerNo;          //0x18
    s16 charNo;            //0x1A
    s16 padNo;             //0x1C
    s8 unk1E;              //0x1E
    s8 motion;             //0x1F
    f32 unk20;             //0x20
    f32 unk24;             //0x24
    Vec pos;               //0x28
} M505_PLAYER;             //0x34

/* Flying item used by the minigame. Stride 0x28. */
typedef struct M505Item_s {
    HU3DMODELID modelId;   //0x00
    s16 state;             //0x02
    s32 timer;             //0x04
    f32 radX;              //0x08
    f32 radY;              //0x0C
    Vec pos;               //0x10
    Vec scale;             //0x1C
} M505_ITEM;               //0x28

/* Per-slot particle manager state. Stride 0x30. */
typedef struct M505ParMan_s {
    HU3DPARMANID id;       //0x00
    s16 pad1;              //0x02
    Vec pos;               //0x04
    Vec offset;            //0x10
    Vec scale;             //0x1C
    s16 playerNo;          //0x28
    s16 pad2;              //0x2A
    HU3DMOTID motA;        //0x2A
    HU3DMOTID motB;        //0x2C
} M505_PARMAN;            //0x30

/* The module work structure (lbl_1_bss_8). */
typedef struct M505Flow_s {
    OMOBJMAN *objman;      //0x00
    s16 camNo;             //0x04
    s16 winId;             //0x06
    s8 winner;             //0x08
    u8 unk9;               //0x09
    u8 unkA;               //0x0A
    u8 unkB;               //0x0B
    s32 flag;              //0x0C
    s32 flag2;             //0x10
    M505_PLAYER player[MGMATCH_PLAYER_MAX]; //0x14
    HU3DMODELID mdlPodium; //0xE4
    s16 camMoveSpeed;      //0xE6
    Vec posFrom;           //0xE8
    Vec posTo;             //0xF4
    Vec pos;               //0x100
    Vec unused10C;         //0x10C
    HU3DMODELID mdlItem;   //0x118
    Vec itemPos0;          //0x11C
    Vec itemPos1;          //0x128
    Vec itemPos2;          //0x134
    M505_ITEM item[MGMATCH_ITEM_MAX]; //0x140
    HU3DMODELID mdl[MGMATCH_MDL_MAX]; //0x258
    HU3DMODELID mdlCam0;   //0x2D2
    HU3DMODELID mdlCam1;   //0x2D4
    CAMMOTIONWORK camPath[MGMATCH_CAMPATH_MAX]; //0x2D8
    s32 camPathNum;        //0xC38
    s32 itemAnimState;     //0xC3C
    ANIMDATA *mgAnim[MGMATCH_MG_MAX]; //0xC40
    s32 parManId[8];      //0xC94 (32-bit slots, only the low half is used)
    s16 wheelAngle[4];     //0xCA4
    s16 parManMaxTime;     //0xCAC
    M505_PARMAN parMan[MGMATCH_PARMAN_MAX]; //0xCB0
    s32 unknownE00;        //0xE00
    s32 keyWaitMask;       //0xE04
    HU3DANIMID animId;     //0xE08
    s16 movieNo;           //0xE0A
    s32 seId;              //0xE0C
    s32 bgmId;             //0xE10
    s32 loadState;         //0xE14
    s32 mgNo;              //0xE18
    s16 camMotionType;     //0xE1C
    f32 rotSpeed;          //0xE20
    s16 glightId;          //0xE24
    s16 camMotionNo;       //0xE26
    f32 camMoveRate;       //0xE28
    s16 padE2A;            //0xE2A
    s16 timerType;         //0xE2C
} M505_FLOW;               //0xE2E

/* One entry of the minigame table. Stride 6. */
typedef struct M505MgEntry_s {
    u16 ovlNo;               //0x00 overlay number, matched against mgDataTbl[].ovl
    s16 mgNo;                //0x02 GW minigame number (GW_MGNO_BASE + index)
    s8 mgIdx;                //0x04 index into mgDataTbl, resolved at startup
    s8 is3D;                 //0x05 mgDataTbl[mgIdx].type == MG_TYPE_3D
} M505_MGENTRY;               //0x06

/* Module-local copy of the same table; type is shared with mgModeWork's. */
typedef M505_MGENTRY M505_MGINFO;

/* mgModeWork. */
typedef struct M505ModeWork_s {
    s8 entryNo;            //0x00
    s8 orderNo;            //0x01
    u8 pad[0x14];          //0x02
    u8 flag;               //0x16
    s8 score[MGMATCH_PLAYER_MAX]; //0x17
    u8 gameType;           //0x1B
    u8 teamCount;          //0x1C
    s8 maxScore;           //0x1D
    s8 winner;             //0x1E
    u8 pad2;               //0x1F
    s32 rank[MGMATCH_PLAYER_MAX]; //0x20
    u8 mgCount;            //0x30
    u8 pad4;               //0x31
    M505_MGENTRY mgTbl[MGMATCH_MG_MAX]; //0x32
    Vec camPos;            //0xB0
    Vec camUp;             //0xBC
    Vec camTarget;         //0xC8
    f32 fov;               //0xD4
} M505_MODEWORK;           //0xD8

typedef void (*M505Proc)(void);

extern M505_FLOW *lbl_1_bss_0;
extern M505_MODEWORK *lbl_1_bss_4;

extern const M505Proc _ctors[];
extern const M505Proc _dtors[];

/* score.c */
s32 fn_1_12404(s8 gameType, s8 maxScore, M505_PLAYER *player, s8 *score,
               HU3DMODELID *mdl, M505_FLOW *flow);
void fn_1_12934(void);
void fn_1_12AE4(s32 no);
s32 fn_1_12B08(void);
s32 fn_1_12B4C(void);
void fn_1_138B8(Vec *pos, Vec *scale, s8 winTeam, s8 playerNo, s8 gameType,
               Vec *standPos, f32 yOffset, f32 zOffset);
void fn_1_13C1C(s8 winTeam, s8 gameType, s8 maxScore, u8 *order, s32 *rank);

/* mess.c */
void fn_1_15BC8(void);
void fn_1_15C74(M505_PLAYER *player);
void fn_1_15DB8(M505_PLAYER *player, void *msg, s32 size);
void fn_1_15F24(void);
void fn_1_15F88(void);
void fn_1_16018(void);
void fn_1_16080(void);
void fn_1_160F0(void);
void fn_1_161E8(void);
void fn_1_16250(void);
void fn_1_16574(void);
void fn_1_168A4(void);
void fn_1_169E0(void);
void fn_1_16A48(void);
void fn_1_16AC4(void);
void fn_1_16B20(void);
void fn_1_16B7C(void);
void fn_1_16BFC(void);
void fn_1_16C7C(void);
void fn_1_16CD8(void);
void fn_1_16D34(void);

#endif