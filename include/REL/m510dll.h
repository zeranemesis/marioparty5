#ifndef _M510DLL_H
#define _M510DLL_H

#include "dolphin.h"
#include "game/object.h"
#include "game/gamework.h"
#include "game/gamemes.h"
#include "game/wipe.h"
#include "game/audio.h"
#include "game/hu3d.h"
#include "game/charman.h"
#include "game/memory.h"
#include "game/pad.h"
#include "game/process.h"
#include "game/esprite.h"
#include "game/frand.h"
#include "game/data.h"
#include "math.h"

#include "datanum/m510.h"

// Player work struct (0x880 bytes) used by player.c / playermodel.c / pitch.c / ball_path.c / score.c
typedef struct M510PlayerWork_s {
    void *obj;            //0x00  OMOBJ *
    s16 playerNo;         //0x04
    struct M510PlayerModelWork_s { //0x08, 0x110 bytes
        HU3DMODELID modelId;   //0x08
        HU3DMOTID motId;       //0x0a
        void *data;            //0x0c
        // ... 0x110 bytes total
    } model;              //0x08
    // pitch at 0x118 (0x3c bytes)
    // ball_path at 0x154 (0x1e4 bytes)
    // score at 0x850 (0x28 bytes)
    s16 modelId;          //0x878
    s16 motionId;         //0x87a
    u32 flag;             //0x87c
    u32 unkC0;            //0xc0
    u32 unkC4;            //0xc4
    u32 unkC8;            //0xc8
} M510_PLAYER_WORK;

void fn_1_3A64(void *data, s16 playerNo, OMOBJ *obj);
void fn_1_3CCC(void *data);
void fn_1_3D34(void *data);

void fn_1_3DC8(void *data, void *obj, s16 playerNo);
void fn_1_4284(void *data);
void fn_1_4328(void *data);
void fn_1_4770(void *data);

void fn_1_47C4(void *data, void *obj);
void fn_1_49A4(void *data);
void fn_1_49F4(void *data);

void fn_1_509C(void *data, void *obj);
void fn_1_51B4(void *data);
void fn_1_5230(void *data);
void fn_1_52B0(void *data);
void fn_1_53E0(void *data);
void fn_1_557C(void *data);
void fn_1_55C4(void *data);
void fn_1_5E40(void *data);
void fn_1_64B0(void *data);
void fn_1_7038(void *data);
void fn_1_8060(void *data);

void fn_1_8184(void);
void fn_1_81F4(void);
void fn_1_8234(void);
void fn_1_8420(void);
void fn_1_8778(void);
void fn_1_87A8(void);

void fn_1_89B8(void);
void fn_1_8B1C(void);
void fn_1_8BEC(void);
void fn_1_8C1C(void);
void fn_1_8D48(void);

void fn_1_966C(void);

void fn_1_9B0C(void);
void fn_1_A454(void);
void fn_1_A4F4(void);
void fn_1_B350(void);
void fn_1_B4A4(void);
void fn_1_B8F4(void);
void fn_1_B958(void);
void fn_1_BE48(void);
void fn_1_BEC4(void);

void fn_1_C590(void);
void fn_1_C5C0(void);
void fn_1_C5F0(void);
void fn_1_C650(void);
void fn_1_C70C(void);
void fn_1_CAD0(void);
void fn_1_CC58(void);
void fn_1_CD18(void);

void fn_1_CE04(void *data, s16 playerNo, void *obj);
void fn_1_D1AC(void *data);
void fn_1_D234(void *data);
void fn_1_D354(void *data);

void fn_1_E8(void);
void fn_1_164(void);
void fn_1_1C0(void);
void fn_1_21C(void);
void fn_1_260(void);
void fn_1_2BC(void);
void fn_1_30C(void);
void fn_1_404(void);
void fn_1_494(void);
void fn_1_568(void);
void fn_1_5B4(void);
void fn_1_5DC(void);
void fn_1_604(void);
void fn_1_634(void);
void fn_1_664(void);
void fn_1_6BC(void);
void fn_1_6E4(void);
void fn_1_714(void);
void fn_1_734(void);
void fn_1_75C(void);
void fn_1_784(void);
void fn_1_7B4(void);
void fn_1_9B8(void);
void fn_1_C6C(void);
void fn_1_D84(void);
void fn_1_E54(void);
void fn_1_1074(void);
void fn_1_1274(void);
void fn_1_129C(void);
void fn_1_1308(void);
void fn_1_1310(void);
void fn_1_1338(void);
void fn_1_2148(void);
void fn_1_2220(void);
void fn_1_2390(void);
void fn_1_24D0(void);
void fn_1_2840(void);
void fn_1_295C(void);
void fn_1_2F8C(void);
void fn_1_3020(void);
void fn_1_30D8(void);
void fn_1_33C0(void);
void fn_1_3418(void);
void fn_1_3598(void);
void fn_1_3834(void);
void fn_1_38A0(void);

void ObjectSetup(void);

#endif