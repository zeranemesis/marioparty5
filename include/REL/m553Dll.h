#ifndef _M553DLL_H
#define _M553DLL_H

#include "game/audio.h"
#include "game/gamework.h"
#include "game/pad.h"
#include "game/object.h"
#include "game/hu3d.h"
#include "game/charman.h"
#include "game/sprite.h"

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