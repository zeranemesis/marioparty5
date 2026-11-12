#ifndef _M553DLL_H
#define _M553DLL_H

#include "game/audio.h"
#include "game/gamework.h"
#include "game/pad.h"
#include "game/object.h"
#include "game/hu3d.h"
#include "game/charman.h"
#include "game/sprite.h"
#include "game/mg/timer.h"
#include "game/mg/actman.h"

#include "datanum/m553.h"

extern int gameMaxScore;
extern int gameBallNumMax;
extern int gameType;

typedef struct M553GameWork_s {
    unsigned int mode;
    int modeTime;
    BOOL modeActive;
    int cameraMode;
    int startTime;
    int gamemesMode;
    int gamemesTimer;
    unsigned int second;
    unsigned int frame;
    int winMode;
    unsigned int winTimer;
    int winPlayer[2];
    unsigned int winType;
    unsigned int winFlag;
    unsigned int winFlag2;
} M553_GAMEWORK;

typedef struct M553StModel_s {
    HU3DMODELID id;
    HuVecF pos;
    HuVecF rot;
} M553STMODEL;

typedef struct M553StPlayer_s {
    s16 idx;
    s16 charNo;
    s16 dif;
    s16 padNo;
    s32 btn;
    s32 btnDown;
    s8 stkX;
    u16 btnRep;
    s8 stkY;
    s8 subStkX;
    s8 subStkY;
    u8 trigL;
    u8 trigR;
    u8 dStk;
    u8 dStkRep;
} M553STPLAYER;

typedef struct M553StMotCtl_s {
    s16 motNo;
    s16 motNoOld;
    f32 speed;
    f32 speedOld;
    f32 time;
    f32 timeOld;
    f32 maxTime;
    u32 attr;
    u32 attrOld;
    u32 attrReset;
    s32 endFlag;
    s16 endReturn;
    s16 retMotNo;
    f32 retTime;
    u32 retAttr;
    u32 retAttrReset;
    s16 shiftMode;
    s16 shiftMotNo;
    u32 shiftAttr;
    f32 shiftStart;
    f32 shiftEnd;
    f32 shiftTime;
    f32 shiftMaxTime;
    u8 pad50[4];
} M553STMOTCTL;

typedef struct M553StFx_s {
    s32 active;
    s32 t[3];
    s32 i10;
    s32 i14;
    s32 i18;
    s32 i1C;
    u8 pad20[0xC];
    f32 f2C;
    f32 f30;
    f32 f34;
    f32 f38;
    f32 f3C;
    f32 f40;
    f32 f44;
    u8 pad48[4];
} M553STFX;

typedef struct M553StWork_s {
    u8 pad00[4];
    M553STPLAYER player[2];
    u8 pad3C[0x1C];
    M553STMODEL *model;
    s32 *motData[2];
    u8 pad64[4];
    s16 *motId[2];
    u8 pad70[4];
    M553STMOTCTL motCtl[2];
    u8 pad11C[0x54];
    s32 *data;
    u8 pad174[4];
    s32 endFlag;
    u8 pad17C[0x4C];
    M553STFX fx[21];
    u8 pad7BC[0x44];
} M553STWORK;


typedef struct M553Cam_s {
    s32 no;
    HuVecF pos;
    HuVecF rot;
    HuVecF target;
    HuVecF up;
    f32 zoom;
    f32 aspect;
    f32 vpX;
    f32 vpY;
    f32 vpW;
    f32 vpH;
    f32 fov;
} M553CAM;

typedef struct M553Shadow_s {
    HuVecF pos;
    HuVecF rot;
    HuVecF target;
    HuVecF dir;
    HuVecF up;
    HuVecF unk3C;
    f32 dist;
    f32 tpLvl;
} M553SHADOW;

typedef struct M553GCell_s {
    s16 active;
    s16 vis;
    s16 phase;
    s16 phase2;
    s16 unk08;
    s16 timer;
    s16 type;
    s16 mdl;
    s16 mdl2;
    s16 mdl3;
    s16 hit;
    s16 cell;
    s16 player;
    u8 pad1A[2];
    f32 tpLvl;
} M553GCELL;

typedef struct M553GPlayerState_s {
    s16 active;
    s16 phase;
    u8 pad04[8];
    f32 rate;
    s8 *color;
    s16 stop;
    s16 hitCnt;
    s16 delay;
    u8 pad1A[2];
    HuVecF pos;
    HuVecF oldPos;
    f32 moveX;
    u8 pad38[4];
    f32 moveZ;
    f32 rotY;
    s16 hitting;
    s16 itemCnt;
    s16 itemCnt2;
    s16 item[20];
    s16 comOn;
    s16 level;
    s16 comPhase;
    u8 pad78[4];
    s16 chasePhase;
    u8 pad7E[2];
    HuVecF target;
    u8 pad8C[0xC];
    f32 chaseRadius;
    f32 chaseVel;
    f32 speed;
    s16 comMode;
    s16 comWait;
    s16 comTimer;
    s16 cellFlag[64];
    s16 skillRate;
    s16 targetCell;
    s16 targetRate;
    s32 padBtn;
    s32 padBtnDown;
    s32 padStkX;
    s32 padStkY;
} M553GPLAYERSTATE;

typedef struct M553GState_s {
    s16 mode;
    s16 sub;
    s16 unk04;
    s16 timer;
    f32 tween[4];
    s16 round;
    s16 frame;
    s16 moveFlag[2];
    s16 winner;
    u8 pad22[2];
    s16 dirIdx;
    s16 dirTimer;
    u8 pad28[14];
    s16 chaseMode;
    s16 hitCnt;
    s16 hitFlag;
    f32 scaleA;
    f32 scaleB;
    MGTIMER *timerObj;
    s32 pad48;
    s32 bgm;
    s32 jingle;
    s16 sfxFlag;
    s16 pad56;
    s32 se[22];
    s16 seState[22];
} M553GSTATE;

typedef struct M553GPlayers_s {
    HU3DMODELID colMdl;
    MGPLAYER *player[2];
} M553GPLAYERS;

extern M553GPLAYERS lbl_1_bss_0;
extern M553CAM *lbl_1_bss_14;
extern M553SHADOW *lbl_1_bss_C;
extern M553GCELL lbl_1_bss_18[35];
extern s16 lbl_1_bss_478[10];
extern M553GPLAYERSTATE lbl_1_bss_48C[2];
extern M553GSTATE lbl_1_bss_70C;
extern M553STMODEL lbl_1_bss_7FC[0x88];

void M553StageInit(OMOBJMAN *om);
void M553StageClose(void);
OMOBJFUNC M553StartCameraCreate(OMOBJMAN *om, OMOBJ *obj);
int M553StageRandom(void);
void M553StageCameraFocusSet(float time, Vec *pos, Vec *rot, float zoom);
void M553StageCameraSpeedSet(float speed);
void M553StageCameraOfsSet(u8 no, u16 time, float radius, float speed, float rotSpeed);
void M553StageRotMtxCalc(Mtx matrix, float x, float y, float z);
void M553StageMtxRotGet(Mtx matrix, HuVecF *result);
void M553GuideModeSet(s16 mode);

void M553PlayerInit(OMOBJMAN *om);
void M553PlayerClose(void);
BOOL M553WinGroupCheck(int side);
unsigned int M553GameWinGet(int side);
unsigned int M553EventGet(int side);
BOOL M553ServeGroupCheck(int side);
unsigned int M553ServeCelGet(int side);
BOOL M553PointGroupCheck(int side);
float M553PlaneDistGet(HuVecF *pos1, HuVecF *point1, HuVecF *pos2, HuVecF *point2);
unsigned int M553GameEventGet(void);
unsigned int M553ServeSideGet(HuVecF *pos);
unsigned int M553ContactCountGet(void);
BOOL M553ContactCheck(unsigned int side, unsigned int cel);
BOOL M553ContactGroupCheck(unsigned int side);

BOOL M553BallCoordGet(HuVecF *pos);
unsigned int M553BallSideGet(HuVecF *pos);
float M553BallPointCheck(float radius, HuVecF *pos);
BOOL M553BallJumpCheck(HuVecF *pos);
unsigned int M553BallTimeGet(void);
unsigned int M553BallAirTimeGet(void);
void M553SandEffectAdd(HuVecF *pos, float speed);
void M553DustEffectAdd(HuVecF *pos);
float M553ServeTargetTimeGet(void);
void M553BallContactSet(HuVecF *pos, unsigned int type, unsigned int side, unsigned int cel);
void M553BallServeSet(float angle, float mag, unsigned int side, unsigned int scaleF, HuVecF *pos);
void M553BallTrailStart(void);
float M553BallTrailGet(void);
void M553StageCollide(HuVecF *pos, unsigned int side);
BOOL M553BallSlipCheck(float radius, HuVecF *pos);
void M553BallExplodeSet(void);

unsigned int M553GameTimerGet(void);

void M553ModeWatch(OMOBJ *obj);
void M553ModeUpdate(OMOBJ *obj);
OMOBJFUNC M553GameWinCreate(OMOBJMAN *om, OMOBJ *obj);
unsigned int M553GameModeGet(void);
unsigned int M553CameraModeGet(void);
unsigned int M553GamemesModeGet(void);
unsigned int M553GameWinModeGet(void);
void M553GameWinnerSet(int type, int player1, int player2);
int M553GameWinTypeGet(void);
void M553GameWinFlagSet(int playerNo);
void M553GameWinFlag2Set(int playerNo);
BOOL M553GameFinishEndCheck(void);
void M553CameraAimSetV(HuVecF *eye, HuVecF *pos);
void M553CameraAimSet(float eyeX, float eyeY, float eyeZ, float posX, float posY, float posZ);
float M553AngleLerp(float start, float end, float time);
float M553AngleSub(float start, float end);

#endif