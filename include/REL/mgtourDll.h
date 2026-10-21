#ifndef _MGTOURDLL_H
#define _MGTOURDLL_H

#include "game/charman.h"
#include "game/data.h"
#include "game/esprite.h"
#include "game/flag.h"
#include "game/frand.h"
#include "game/gamemes.h"
#include "game/gamework.h"
#include "game/hsfex.h"
#include "game/hu3d.h"
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

#define MGTOUR_PLAYER_MAX 4
#define MGTOUR_MGTBL_MAX 15
#define MGTOUR_MDL_MAX 19
#define MGTOUR_ANIM_MAX 15
#define MGTOUR_PARMAN_MAX 24
#define MGTOUR_CAMPATH_MAX 66
#define MGTOUR_PAPER_MAX 1000

/* One of the four character slots. Stride 0x30. */
typedef struct MGTPlayer_s {
    HU3DMODELID modelId;   //0x00 returned by CharModelMotListCreate
    HU3DMOTID motId[7];    //0x02 filled by CharModelMotListCreate
    s16 unk10;             //0x10
    s16 unk12;             //0x12
    s16 playerNo;          //0x14
    s16 charNo;            //0x16
    s16 charNo2;           //0x18 GwPlayerConf[].padNo
    s8 state;              //0x1A
    s8 pad1B;              //0x1B
    f32 rotX;              //0x1C
    f32 rotY;              //0x20
    Vec pos;               //0x24
} MGT_PLAYER;              //0x30

/* One entry of the minigame table. Stride 6. */
typedef struct MGTMgEntry_s {
    u16 mes1;              //0x00
    u16 mes2;              //0x02
    u8 mgIdx;              //0x04 index into mgDataTbl, resolved at startup
    u8 pad5;               //0x05
} MGT_MGENTRY;             //0x06

/* The module work structure (lbl_1_bss_16820). */
typedef struct MGTFlow_s {
    MGT_PLAYER player[MGTOUR_PLAYER_MAX]; //0x000
    HU3DMODELID mdlShake;  //0x0C0
    s16 padC2;             //0x0C2
    u8 padC4[0x18];        //0x0C4
    Vec shakePos;          //0x0DC
    HU3DMODELID mdlPaper;  //0x0E8 same value as mdlReflect
    s16 padEA[7];          //0x0EA
    s32 unkF8;             //0x0F8
    u8 padFC[0x14];        //0x0FC
    OMOBJMAN *objman;      //0x110
    ANIMDATA *texId;       //0x114
    f32 scrollX;           //0x118
    f32 scrollY;           //0x11C
    s16 winId2;            //0x120
    HUWINID winId;         //0x122
    u32 flag;              //0x124
    u32 stat;              //0x128
    u32 keyMask;           //0x12C
    s32 bgmId;             //0x130
    s32 seId;              //0x134
    MGT_MGENTRY mgTbl[MGTOUR_MGTBL_MAX]; //0x138
    HU3DMODELID mdl[MGTOUR_MDL_MAX];     //0x192
    ANIMDATA *thpAnim[MGTOUR_ANIM_MAX];  //0x1B8
    s32 mgTblNum;          //0x1F4
    HU3DPARMANID parManId[MGTOUR_PARMAN_MAX]; //0x1F8
    HU3DANIMID animId;     //0x228
    s8 player1No;          //0x22A
    s8 player2No;          //0x22B
    s8 turnNo;             //0x22C
    s8 charNo;             //0x22D
    s8 pad22E[2];          //0x22E
    s32 loadState;         //0x230
    s32 mgNo;              //0x234
    s8 mode;               //0x238
    u8 pad239[3];          //0x239
    f32 rotSpeed;          //0x23C
    Vec camPos;            //0x240
    Vec camUp;             //0x24C
    CAMMOTIONWORK camPath[MGTOUR_CAMPATH_MAX]; //0x258
    s32 camPathNum;        //0xEB8
} MGT_FLOW;                //0xEC0

/* mgModeWork as this module sees it. */
typedef struct MGTModeWork_s {
    s8 cur;                //0x00 current entry index
    s8 order[0x0B];        //0x01 shuffled order of mgTbl entries
    s16 unk6;              //0x0C
    s16 unk8;              //0x0E
    u8 flag;               //0x10
    s8 mgNum;              //0x11
    s8 playerNo[4];        //0x12
    s8 cur2;               //0x16
    s8 cur3;               //0x17
    s8 cur4;               //0x18
    s8 cur5;               //0x19
    s8 pad1A[2];           //0x1A
    Vec camPos;            //0x1C
    Vec camUp;             //0x28
    Vec camTarget;         //0x34
    f32 aspect;            //0x40
} MGT_MODEWORK;            //0x44

/* One flying paper sheet. Stride 0x5C. */
typedef struct MGTPaper_s {
    s16 alive;             //0x00
    s16 pad2;              //0x02
    s16 kind;              //0x04
    s16 color;             //0x06
    s16 moving;            //0x08
    s16 padA;              //0x0A
    f32 posX;              //0x0C
    f32 posY;              //0x10
    f32 posZ;              //0x14
    f32 velX;              //0x18
    f32 velY;              //0x1C
    f32 velZ;              //0x20
    f32 rotX;              //0x24
    f32 rotY;              //0x28
    f32 rotZ;              //0x2C
    f32 rotVelX;           //0x30
    f32 rotVelY;           //0x34
    f32 rotVelZ;           //0x38
    f32 life;              //0x3C
    f32 alpha;             //0x40
    Vec tint;              //0x44
    Vec scale;             //0x50
} MGT_PAPER;               //0x5C

/* The confetti effect work (lbl_1_bss_16760). */
typedef struct MGTKamiWork_s {
    s16 active;            //0x00
    s16 stopF;             //0x02
    s16 curIdx;            //0x04
    s16 mdlType;           //0x06
    Vec pos;               //0x08
    Vec pos2;              //0x14
    f32 time;              //0x20
    s16 timer;             //0x24
    s16 timerMax;          //0x26
    s16 count;             //0x28
    s16 total;             //0x2A
    s16 pad2C[1];          //0x2C
    HU3DMODELID modelId[4]; //0x2E
    char name[4][0x20];    //0x36
    HU3DMODELID hookMdlId; //0xB6
} MGT_KAMIWORK;            //0xB8

typedef void (*MGTProc)(void);

extern MGT_FLOW *lbl_1_bss_16818;
extern MGT_MODEWORK *lbl_1_bss_1681C;
extern MGT_KAMIWORK *lbl_1_bss_16760;
extern MGT_PAPER lbl_1_bss_0[MGTOUR_PAPER_MAX];

extern const MGTProc _ctors[];
extern const MGTProc _dtors[];

#endif
