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
#include "humath.h"

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

struct M510Obj_s;
struct M510PlayerWork_s;
struct M510Stage_s;
struct M510BallWork_s;
struct M510Pad_s;
struct M510PlayerModelWork_s;

typedef HuVecF M510_VECF;

// CPU controller (0x68 bytes)
typedef struct M510Ai_s {
    struct M510Pad_s *owner;  //0x00
    u16 state;                //0x04
    s16 timer;                //0x06
    s32 type;                 //0x08
    s32 done;                 //0x0C
    u16 level;                //0x10
    u8 pad12[0x2C - 0x12];    //0x12
    s16 pattern[30];          //0x2C
} M510_AI;

// Pad work (0x84 bytes)
typedef struct M510Pad_s {
    struct M510PlayerModelWork_s *owner; //0x00
    M510_AI ai;                          //0x04
    s32 padNo;                           //0x6C
    s32 btn;                             //0x70
    s32 btnDown;                         //0x74
    s8 stkX;                             //0x78
    s8 stkY;                             //0x79
    u16 btnRep;                          //0x7A
    s8 subStkX;                          //0x7C
    s8 subStkY;                          //0x7D
    u8 trigL;                            //0x7E
    u8 trigR;                            //0x7F
    u8 dStk;                             //0x80
    u8 dStkRep;                          //0x81
    u8 pad82[2];                         //0x82
} M510_PAD;

// Player model work (0x110 bytes, M510_PLAYER_WORK::model)
typedef struct M510PlayerModelWork_s {
    struct M510PlayerWork_s *owner; //0x00
    u16 state;                      //0x04
    s16 timer;                      //0x06
    s16 playerNo;                   //0x08
    u8 pad0A[2];                    //0x0A
    s16 charNo;                     //0x0C
    u8 pad0E[2];                    //0x0E
    s32 grp;                        //0x10
    OMOBJ *obj;                     //0x14
    s16 modelId;                    //0x18
    u8 pad1A[0x2C - 0x1A];          //0x1A
    M510_PAD pad;                   //0x2C (+0x84)
    s16 motion;                     //0xB0
    s16 nextMotion;                 //0xB2
    s32 loopF;                      //0xB4
    M510_VECF camPos;               //0xB8 (absolute 0xC0)
    M510_VECF camRot;               //0xC4
    HuVecF *modelPos;               //0xD0
    HuVecF *modelRot;               //0xD4
    s16 itemModelId;                //0xD8
    float rotY;                     //0xDC
    float prevRotY;                 //0xE0
    float rotYGoal;                 //0xE4 (unused)
    float holdYRot;                 //0xE8 (unused)
    float f0EC;                     //0xEC (unused)
    M510_VECF itemPos;              //0xF0
    s32 padOn;                      //0xFC
    s32 flag;                       //0x100
    s32 winner;                     //0x104
    s16 effModelId;                 //0x108
    s16 effMotionId;                //0x10A
    s32 effOn;                      //0x10C
} M510_PLAYER_MODEL_WORK;

typedef struct M510PlayerWork_s M510_PLAYER_WORK;

// Pitcher work (0x3C bytes, M510_PLAYER_WORK::pitch)
typedef struct M510PitchWork_s {
    M510_PLAYER_WORK *owner;       //0x00
    u16 state;                     //0x04
    s16 timer;                     //0x06
    s16 modelId;                   //0x08
    s16 motionId;                  //0x0A
    s16 modelId2;                  //0x0C
    u8 pad0E[0x10 - 0xE];
    M510_VECF pos;                 //0x10 (absolute 0x128)
    float rot;                     //0x1C
    s16 index;                     //0x20
    u8 pad22[0x24 - 0x22];
    s32 active;                    //0x24
    M510_VECF ofs;                 //0x28
    float f34;                     //0x34
    s32 soundId;                   //0x38
} M510_PITCH_WORK;

// Ball path (0x1E4 bytes)
typedef struct M510Path_s {
    s16 count;                 //0x00
    u8 pad02[2];               //0x02
    M510_VECF pts[24];         //0x04
    s32 seg[24];               //0x124
    s32 sum[24];               //0x184
} M510_PATH;

// Spectator (0x20 bytes)
typedef struct M510Spectator_s {
    struct M510Stage_s *owner; //0x00
    u16 state;                 //0x04
    s16 timer;                 //0x06
    s16 type;                  //0x08
    s16 modelId;               //0x0A
    s16 motion[3];             //0x0C
    u8 pad12[2];               //0x12
    M510_VECF pos;             //0x14
} M510_SPECTATOR;

// Score digit board (0x24 bytes)
typedef struct M510Digit_s {
    struct M510Stage_s *owner; //0x00
    u8 pad04[4];               //0x04
    s16 modelId;               //0x08
    s16 motion[10];            //0x0A
    s16 cur;                   //0x1E
    s16 goal;                  //0x20
    u8 pad22[2];               //0x22
} M510_DIGIT;

// Stage work (0x258 bytes, M510_OBJ::stage)
typedef struct M510Stage_s {
    struct M510Obj_s *obj;     //0x00
    u8 pad04[4];               //0x04
    s16 modelId[3];            //0x08
    s16 motionId;              //0x0E
    M510_SPECTATOR spec[16];   //0x10
    M510_DIGIT digit[2];       //0x210
} M510_STAGE;

// One ball (0x250 bytes)
typedef struct M510Ball_s {
    struct M510BallWork_s *work; //0x00
    u16 state;                   //0x04
    s16 timer;                   //0x06
    s16 index;                   //0x08
    u8 pad0A[2];                 //0x0A
    s32 active;                  //0x0C
    M510_VECF pos;               //0x10
    M510_VECF prevPos;           //0x1C
    u8 pad28[0x58 - 0x28];       //0x28
    s16 type;                    //0x58
    s16 modelId0;                //0x5A
    s16 modelId1;                //0x5C
    u8 pad5E[2];                 //0x5E
    M510_PATH path;              //0x60
    s16 speed;                   //0x244
    s16 step;                    //0x246
    s16 hitTime;                 //0x248
    u8 pad24A[2];                //0x24A
    M510_SPECTATOR *target;      //0x24C
} M510_BALL;

// Ball work (0x6FC bytes, M510_PLAYER_WORK::ballWork)
typedef struct M510BallWork_s {
    M510_PLAYER_WORK *owner;   //0x00
    M510_BALL balls[3];        //0x04
    s16 liveCount;             //0x6F4
    s16 modelId0;              //0x6F6
    s16 modelId1;              //0x6F8
    s16 motionId;              //0x6FA
} M510_BALL_WORK;

// Player work (0x880 bytes)
typedef struct M510PlayerWork_s {
    struct M510Obj_s *obj;         //0x00
    s16 playerNo;                  //0x04
    u8 pad06[2];                   //0x06
    M510_PLAYER_MODEL_WORK model;  //0x08
    M510_PITCH_WORK pitch;         //0x118
    M510_BALL_WORK ballWork;       //0x154
    M510_SCORE_WORK score;         //0x850
    s16 modelId;                   //0x878
    s16 motionId;                  //0x87A
    s32 flag;                      //0x87C
} M510_PLAYER_WORK;

// Sub-struct used by the opening sequence
typedef struct M510OpenCam_s {
    struct M510Obj_s *obj;   //0x00
    u32 unk04;       //0x04
    u8 pad08[0x0C - 0x08];
    HuVecF tgt;      //0x0C
    HuVecF cur;      //0x18
    HuVecF ofs;      //0x24
    HuVecF dst;      //0x30
    HuVecF posOfs;   //0x3C
    HuVecF ofsOfs;   //0x48
    HuVecF up;       //0x54
    float speed;     //0x60
} M510_OPEN_CAM;

// Main object of m510dll
typedef struct M510Obj_s {
    OMOBJMAN *objman;        //0x00
    u8 pad04[4];
    s32 frame;               //0x08
    u16 mode;                //0x0C
    u16 state;               //0x0E
    s32 time;                //0x10
    s32 timer1;              //0x14
    s32 timer2;              //0x18
    M510_PLAYER_WORK players[4];//0x1C
    M510_STAGE stage;        //0x221C
    M510_OPEN_CAM cam;       //0x2474
    u8 openWork[8];          //0x24D8
    M510_SCORE_WORK score;   //0x24E0
    s16 remain;              //0x2508
    u8 pattern[30];          //0x250A
    s16 patIdx;              //0x2528
    u8 pad252A[2];
    u32 record;              //0x252C
    s32 mesId;               //0x2530
    s32 practice;            //0x2534
    s32 soundId;             //0x2538
    s32 unk253C;             //0x253C
} M510_OBJ;

void fn_1_3A64(M510_PLAYER_WORK *work, s16 playerNo, M510_OBJ *obj);
void fn_1_3CCC(M510_PLAYER_WORK *work);
s32 fn_1_3D34(M510_PLAYER_WORK *work);

void fn_1_3DC8(M510_PLAYER_MODEL_WORK *data, s16 playerNo, M510_PLAYER_WORK *owner);
void fn_1_4284(M510_PLAYER_MODEL_WORK *data);
s32 fn_1_4328(M510_PLAYER_MODEL_WORK *data);
void fn_1_4770(M510_PLAYER_MODEL_WORK *data);

void fn_1_47C4(M510_PITCH_WORK *work, M510_PLAYER_WORK *self);
void fn_1_49A4(M510_PITCH_WORK *work);
void fn_1_49F4(M510_PITCH_WORK *work);

void fn_1_509C(M510_BALL_WORK *data, M510_PLAYER_WORK *owner);
void fn_1_51B4(M510_BALL_WORK *data);
void fn_1_5230(M510_BALL_WORK *data);
void fn_1_52B0(M510_BALL_WORK *data, M510_VECF *pos, s16 type);
void fn_1_53E0(M510_BALL *ball, s16 index, M510_BALL_WORK *work);
void fn_1_557C(M510_BALL *ball);
void fn_1_55C4(M510_BALL *ball);
void fn_1_5E40(M510_BALL *ball);
void fn_1_64B0(M510_BALL *ball, M510_VECF *target);
s32 fn_1_7038(M510_BALL *ball, M510_VECF *out);
void fn_1_8060(M510_BALL_WORK *data, M510_BALL *ball, s16 type, s32 on);

void fn_1_8184(M510_PAD *data, s16 playerNo, M510_PLAYER_MODEL_WORK *owner);
void fn_1_81F4(M510_PAD *data);
void fn_1_8234(M510_PAD *data);
void fn_1_8420(M510_AI *data, M510_PAD *owner);
void fn_1_8778(M510_AI *data);
void fn_1_87A8(M510_AI *data);

void fn_1_89B8(M510_OPEN_CAM *data, M510_OBJ *obj);
void fn_1_8B1C(M510_OPEN_CAM *data, s32 camId);
void fn_1_8BEC(M510_OPEN_CAM *data);
void fn_1_8C1C(M510_OPEN_CAM *data);
void fn_1_8D48(M510_OPEN_CAM *data);

s32 fn_1_734(s32 arg0);

void fn_1_9B0C(M510_OBJ *data);
void fn_1_A454(M510_OBJ *data);
s32 fn_1_A4F4(M510_OBJ *data);
s32 fn_1_B350(M510_OBJ *data);
void fn_1_B4A4(M510_OBJ *data);
void fn_1_B8F4(M510_OBJ *data);
void fn_1_B958(void *data);
void fn_1_BE48(M510_OBJ *data, M510_VECF *src);
void fn_1_BEC4(void *data);

void fn_1_C590(M510_PATH *data);
void fn_1_C5C0(M510_PATH *data);
void fn_1_C5F0(M510_PATH *data);
void fn_1_C650(M510_PATH *data, M510_VECF *pt, s32 len);
void fn_1_C70C(M510_VECF *out, M510_PATH *path, s32 dist, f32 tension);
void fn_1_CAD0(M510_VECF *out, M510_PATH *path, s32 idx, f32 tension);
void fn_1_CC58(M510_VECF *out, M510_VECF *p0, M510_VECF *p1, M510_VECF *t0, M510_VECF *t1, f32 t);
f32 fn_1_CD18(f32 t, f32 p0, f32 p1, f32 t0, f32 t1);

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
s32 fn_1_5B4(s32 seId);
void fn_1_5DC(s32 seNo);
void fn_1_604(s32 seNo, s32 speed);
void fn_1_634(s32 seNo, s16 pan);
void fn_1_664(Vec *pos, Vec *heading, float sndDist, float sndSpeed, float startDis);
void fn_1_6BC(s16 streamId);
void fn_1_6E4(s32 streamNo, s32 speed);
void fn_1_714(void);
s32 fn_1_75C(s16 streamId);
void fn_1_784(s16 streamId, s16 chanNo);
void fn_1_7B4(M510_OBJ *obj, OMOBJMAN *objman);
void fn_1_9B8(M510_OBJ *obj, s16 mode);
s32 fn_1_C6C(M510_OBJ *obj, s16 idx, s16 val);
void fn_1_D84(M510_OBJ *obj);
s32 fn_1_E54(M510_OBJ *obj, s32 unused);
void fn_1_1074(M510_OBJ *obj);
s32 fn_1_1274(M510_OBJ *obj);
void fn_1_129C(M510_OBJ *obj);
s32 fn_1_1308(M510_OBJ *obj);
s32 fn_1_1310(M510_OBJ *obj);
void fn_1_1338(void *unused, Mtx mtx);
void fn_1_2148(M510_VECF *a, M510_VECF *b, GXColor *c);
void fn_1_2220(M510_VECF *pos, f32 size, GXColor *c);
void fn_1_2390(M510_VECF *a, M510_VECF *b, M510_VECF *c, M510_VECF *d, GXColor *col);
void fn_1_24D0(float quat[4], Vec *from, Vec *to);
f32 fn_1_2840(f32 a, f32 b, f32 t);
void fn_1_295C(M510_STAGE *stage, M510_OBJ *obj);
void fn_1_2F8C(M510_STAGE *stage);
void fn_1_3020(M510_STAGE *stage);
void fn_1_30D8(M510_SPECTATOR *spec, u16 type, M510_VECF *pos, M510_STAGE *stage, f32 scale);
void fn_1_33C0(M510_SPECTATOR *spec);
void fn_1_3418(M510_SPECTATOR *spec);
void fn_1_3598(M510_DIGIT *digit, s16 value, M510_VECF *pos, M510_STAGE *stage);
void fn_1_3834(M510_DIGIT *digit);
void fn_1_38A0(M510_DIGIT *digit);

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

// Declared here rather than with the other M510_OBJ prototypes only because
// REL/m510dll/opening.c defines it. It takes an M510_OBJ, and that spelling is
// the verified one: with it, opening.o is byte-identical to the target object
// (.text 1184, .rodata 24, .data 48, 47 relocation records matching record for
// record) and m510dll.rel hashes to its recorded SHA-1 with the unit linked as
// Matching.
//
// An earlier integration attempt declared it against M510_WORK instead, on the
// theory that M510_WORK and M510_OBJ describe the same 0x253C-byte object under
// two names -- phase at 0x0E, frame at 0x14, time at 0x18, four 0x880 slots from
// 0x1C, slide at 0x2474, fieldId at 0x2538. That theory is true and it was still
// wrong: it cost 4 bytes of .text and the object never matched. Note also that
// MWCC rejects two conflicting definitions of the same function, so whichever
// spelling is chosen here must be the one opening.c uses.
s32 fn_1_966C(M510_OBJ *obj);

#endif
