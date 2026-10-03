#ifndef _M510DLL_H
#define _M510DLL_H

#include "dolphin.h"
#include "game/object.h"
#include "game/gamework.h"
#include "game/gamemes.h"
#include "game/mg/score.h"
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

// Data numbers used by this REL (include/datanum/m510.h does not exist yet)
#define M510_DATA_PLAYER_ESP   0x4533
#define M510_DATA_SCORE_NORMAL 0x9631
#define M510_DATA_SCORE_B      0x9630
#define M510_DATA_ESP_TPL1     0x9645
#define M510_DATA_ESP_TPL2     0x9646

// Score work, 0x28 bytes
typedef struct M510ScoreWork_s {
    u32 unk0;         //0x00
    u16 type;         //0x04
    s16 playerNo;     //0x06
    HUSPRGRPID winId; //0x08
    MGSCORE *score;   //0x0C
    s16 espId0;       //0x10
    s16 espId1;       //0x12
    s16 espId2;       //0x14
    float scale;      //0x18
    s32 dirFlag;      //0x1C
    s32 dispFlag;     //0x20
    s16 value;        //0x24
} M510_SCORE_WORK;

// Player work struct (0x880 bytes) used by player.c / playermodel.c / pitch.c
// / ball_path.c / score.c
struct M510Obj_s;

typedef struct M510VecF_s {
    float x;
    float y;
    float z;
} M510_VECF;

typedef struct M510PlayerModelWork_s { //0x08, 0x110 bytes
    u8 pad00[0xB8];
    M510_VECF camPos;              //0xB8 (absolute 0xC0)
    u8 padC4[0x100 - 0xC4];
    s32 flag;                      //0x100
    u8 pad104[0x110 - 0x104];
} M510_PLAYER_MODEL_WORK;

typedef struct M510PlayerWork_s M510_PLAYER_WORK;

typedef struct M510PitchWork_s {   //0x118, 0x3C bytes
    M510_PLAYER_WORK *owner;          //0x00
    u8 pad04[0x8 - 0x4];
    s16 modelId;                   //0x08
    s16 motionId;                  //0x0A
    s16 modelId2;                  //0x0C
    u8 pad0E[0x10 - 0xE];
    M510_VECF pos;                 //0x10 (absolute 0x128)
    u8 pad1C[0x20 - 0x1C];
    s16 state;                     //0x20
    u8 pad22[0x24 - 0x22];
    s32 unk24;                     //0x24
    u8 pad28[0x38 - 0x28];
    s32 unk38;                     //0x38
} M510_PITCH_WORK;

typedef struct M510PlayerWork_s {
    struct M510Obj_s *obj;         //0x00
    s16 playerNo;                  //0x04
    u8 pad06[2];                   //0x06
    M510_PLAYER_MODEL_WORK model;  //0x08
    M510_PITCH_WORK pitch;         //0x118
    u8 ballPathWork[0x850 - 0x154];//0x154
    M510_SCORE_WORK score;         //0x850
    s16 modelId;                   //0x878
    s16 motionId;                  //0x87A
    s32 flag;                      //0x87C
} M510_PLAYER_WORK;

// Sub-struct used by the opening sequence
typedef struct M510OpenCam_s {
    u8 pad00[4];
    u32 unk04;       //0x04
    u8 pad08[0x0C - 0x08];
    HuVecF tgt;      //0x0C
    HuVecF cur;      //0x18
    HuVecF ofs;      //0x24
    HuVecF dst;      //0x30
    u8 pad3C[0x60 - 0x3C];
    float speed;     //0x60
} M510_OPEN_CAM;

// Main object of m510dll
typedef struct M510Obj_s {
    u8 pad00[0x0E];
    u16 state;               //0x0E
    u8 pad10[0x14 - 0x10];
    s32 timer1;              //0x14
    s32 timer2;              //0x18
    M510_PLAYER_WORK players[4];//0x1C
    u8 pad221C[0x2474 - 0x221C];
    M510_OPEN_CAM cam;       //0x2474
    u8 pad24D8[0x2538 - 0x24D8];
    s32 soundId;             //0x2538
} M510_OBJ;

void fn_1_3A64(M510_PLAYER_WORK *work, s16 playerNo, M510_OBJ *obj);
void fn_1_3CCC(M510_PLAYER_WORK *work);
s32 fn_1_3D34(M510_PLAYER_WORK *work);

void fn_1_3DC8(void *data, s16 playerNo, void *obj);
void fn_1_4284(void *data);
void fn_1_4328(void *data);
void fn_1_4770(void *data);

void fn_1_47C4(M510_PITCH_WORK *work, M510_PLAYER_WORK *self);
void fn_1_49A4(M510_PITCH_WORK *work);
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

void fn_1_8184(void *data);
void fn_1_81F4(void *data);
void fn_1_8234(void *data);
void fn_1_8420(void *data);
void fn_1_8778(void *data);
void fn_1_87A8(void *data);

void fn_1_89B8(void *data);
void fn_1_8B1C(void *data);
void fn_1_8BEC(void *data);
void fn_1_8C1C(void *data);
void fn_1_8D48(void *data);

s32 fn_1_734(s32 dataNum);
s32 fn_1_966C(M510_OBJ *obj);

void fn_1_9B0C(void *data);
void fn_1_A454(void *data);
void fn_1_A4F4(void *data);
void fn_1_B350(void *data);
void fn_1_B4A4(void *data);
void fn_1_B8F4(void *data);
void fn_1_B958(void *data);
void fn_1_BE48(void *data);
void fn_1_BEC4(void *data);

void fn_1_C590(void *data);
void fn_1_C5C0(void *data);
void fn_1_C5F0(void *data);
void fn_1_C650(void *data);
void fn_1_C70C(void *data);
void fn_1_CAD0(void *data);
void fn_1_CC58(void *data);
void fn_1_CD18(void *data);

void fn_1_CE04(M510_SCORE_WORK *work, u16 type, s16 playerNo);
void fn_1_D1AC(M510_SCORE_WORK *work);
void fn_1_D234(M510_SCORE_WORK *work);
void fn_1_D354(M510_SCORE_WORK *work, s32 disp);

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
#include "game/hu3d.h"
#include "humath.h"

/* One of the view objects the opening drives: the camera and three model slots.
 * The opening moves each one, then raises ->showF when the sequence reaches the
 * frame that reveals it. */
typedef struct M510ViewObj_s {
    /* 0x000 */ u8 pad000[0xB8];
    /* 0x0B8 */ f32 offset;
    /* 0x0BC */ u8 pad0BC[0x44];
    /* 0x100 */ s32 showF;
} M510_VIEWOBJ;

/* A view slot: an eight byte header, then the object the opening animates. */
typedef struct M510View_s {
    /* 0x000 */ u8 pad000[0x08];
    /* 0x008 */ M510_VIEWOBJ obj;
    /* 0x10C */ u8 pad10C[0x774];
} M510_VIEW;

/* The block the opening slides across the screen. */
typedef struct M510Slide_s {
    /* 0x00 */ u8 pad00[0x0C];
    /* 0x0C */ Vec from;
    /* 0x18 */ f32 x;
    /* 0x1C */ f32 y;
    /* 0x20 */ f32 z;
    /* 0x24 */ Vec to;
    /* 0x30 */ f32 x2;
    /* 0x34 */ f32 y2;
    /* 0x38 */ f32 z2;
    /* 0x3C */ u8 pad3C[0x24];
    /* 0x60 */ f32 fade;
} M510_SLIDE;

/* A player slot, reset when the third phase ends. */
typedef struct M510Slot_s {
    /* 0x00 */ u8 pad00[0x20];
    /* 0x20 */ s16 state;
    /* 0x22 */ u8 pad22[0x02];
    /* 0x24 */ s32 activeF;
} M510_SLOT;

/* The module's work structure: the argument of the opening function. */
typedef struct M510Work_s {
    /* 0x000 */ u8 pad000[0x0E];
    /* 0x00E */ u16 phase;
    /* 0x010 */ u8 pad010[0x04];
    /* 0x014 */ s32 frame;
    /* 0x018 */ s32 time;
    /* 0x01C */ M510_VIEW view0;
    /* 0x89C */ M510_VIEW view1;
    /* 0x111C */ M510_VIEW view2;
    /* 0x199C */ M510_VIEW view3;
    /* 0x221C */ u8 pad221C[0x258];
    /* 0x2474 */ M510_SLIDE slide;
    /* 0x24D8 */ u8 pad24D8[0x60];
    /* 0x2538 */ s32 fieldId;
} M510_WORK;

s32 fn_1_734(s32 arg0);

#endif
