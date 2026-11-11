#ifndef _MDOMAKEDLL_H
#define _MDOMAKEDLL_H

#include "msm_se.h"
#include "msm_stream.h"
#include "msm.h"
#include "game/audio.h"

#include "game/object.h"
#include "game/printfunc.h"
#include "game/hu3d.h"
#include "game/sprite.h"
#include "game/window.h"
#include "game/pad.h"
#include "game/disp.h"

#define MDMATH_OSC_0 0
#define MDMATH_OSC_90 1
#define MDMATH_OSC_90_REV 2
#define MDMATH_OSC_180 3
#define MDMATH_OSC_360 4
#define MDMATH_OSC_540 5
#define MDMATH_OSC_720 6

#define MD_LOAD_STATE_NONE 0
#define MD_LOAD_STATE_LOADING 1
#define MD_LOAD_STATE_DONE 2

#define MD_OVLCALL_GOTO 0
#define MD_OVLCALL_CALL 1
#define MD_OVLCALL_RETURN 2

#define MDMESS_WIN_SPEAKER 0
#define MDMESS_WIN_NOSPEAKER 1
#define MDMESS_LIST_END -1

#define MDMATH_CURVE_SAMPLE_MAX 8

#define MDLIGHT_MAX 1
#define MDPARTICLE_MAX 768

#define MdProcCall(idx, arg) MdProcTbl[idx](arg)
#define MdProcWait(idx, arg) do { \
    while(!MdProcCall(idx, arg)) { \
        HuPrcVSleep(); \
    } \
} while(0)

#define MDSPR_GRP_MEMBERNO(grp, spr) ((spr)-(grp##_START))

#define MDSPR_GRP_INFO_DEFINE(grp, posX, posY) { grp##_START, grp##_END, posX, posY }
enum {
    MDMATH_CURVE_STD_BOUNCE, //0
    MDMATH_CURVE_STD_LINE, //1
    MDMATH_CURVE_STD_MAX // 2
};

enum {
    MD_DEBUGFONT_CTRL_NOP, //0
    MD_DEBUGFONT_CTRL_CAMERA, //1
    MD_DEBUGFONT_CTRL_LIGHT, //2
    MD_DEBUGFONT_CTRL_MODEL, //3
    MD_DEBUGFONT_CTRL_MAX //4
};

enum {
    MDCAMERA_MAIN, //0
    MDCAMERA_GUIDE, //1
    MDCAMERA_UNUSED, //2
    MDCAMERA_MAX //3
};

enum {
    MDCAMERA_FOCUS_TARGET, // 0
    MDCAMERA_FOCUS_POS, //1
    MDCAMERA_FOCUS_MAX //2
};

enum {
    MDMESS_WIN_MAIN, //0
    MDMESS_WIN_HELP, //1
    MDMESS_WIN_MAX //2
};

enum {
    MDLIGHT_TYPE_NOP, //0
    MDLIGHT_TYPE_G_DIRECT, //1
    MDLIGHT_TYPE_L_DIRECT, //2
    MDLIGHT_TYPE_G_POINT, //3
    MDLIGHT_TYPE_L_POINT, //4
    MDLIGHT_TYPE_G_SPOT, //5
    MDLIGHT_TYPE_L_SPOT, //6
    MDLIGHT_TYPE_MAX
};

typedef void (*MD_FUNC)(void);
typedef BOOL (*MD_PROC)(int param);


enum {
    MDOMAKE_FLOW_INIT, //0
    MDOMAKE_FLOW_MAIN, //1
    MDOMAKE_FLOW_MAX //2
};

enum {
    MDOMAKE_PROC_RESET, //0
    MDOMAKE_PROC_CAMERA_LERP, //1
    MDOMAKE_PROC_CURSOR_UPDATE, //2
    MDOMAKE_PROC_WIN_UPDATE, //3
    MDOMAKE_PROC_CAP_UPDATE, //4
    MDOMAKE_PROC_SCENE_UPDATE, //5
    MDOMAKE_PROC_TITLE_UPDATE, //6
    MDOMAKE_PROC_MAX //7
};

#define MDSPR_ANIM_MAX 9
#define MDSPR_MAX 14
#define MDSPR_GRP_MAX 4

enum {
    MDSPR_GRP_WIN, //0
    MDSPR_GRP_CURSOR_LR, //1
    MDSPR_GRP_CURSOR_UDLR, //2
    MDSPR_GRP_MODE //3
};

enum {
    MDOMAKE_OBJ_DEBUGFONT, // 0
    MDOMAKE_OBJ_PAD, //1
    MDOMAKE_OBJ_CAMERA, //2
    MDOMAKE_OBJ_LIGHT, //3
    MDOMAKE_OBJ_MAX //4
};

enum {
    MDMODEL_STAR, //0
    MDMODEL_STAGE, //1
    MDMODEL_CAP_START, //2
    MDMODEL_CAP_CARD = MDMODEL_CAP_START, //2
    MDMODEL_CAP_VOLLEY, //3
    MDMODEL_CAP_HOCKEY, //4
    MDMODEL_CAP_END, //5
    MDMODEL_CAP_NUM = MDMODEL_CAP_END-MDMODEL_CAP_START,
    MDMODEL_FONTBGFILTER = MDMODEL_CAP_END, //5
    MDMODEL_MAX //6
};

#define MDMODEL_NUM MDMODEL_MAX
#define MDMODEL_MOT_NUM 3
#define MDMODEL_TOUEI_NUM 3

typedef struct MdDebugFontWork_s {
    BOOL createF; //0x00
    int ctrl; //0x04
    BOOL printF; //0x08
    int color; //0x0C
    s16 posX; //0x10
    s16 posY; //0x12
    s16 ofsY; //0x14
    s16 rowOfsY; //0x16
    s16 rowH; //0x18
    s16 maxLen; //0x1A
    float size; //0x1C
} MDDEBUGFONT_WORK;

typedef struct MdPadWork_s {
    u16 DStkDown[4];
    u16 DStk[4];
} MDPAD_WORK;

typedef struct MdCameraTypeInfo_s {
    int bit;
    int focusType;
} MDCAMERA_TYPE_INFO;

typedef struct MdCameraRect_s {
    float x;
    float y;
    float w;
    float h;
} MDCAMERA_RECT;

typedef struct MdCameraParam_s {
    HuVecF hvr;
    HuVecF center;
} MDCAMERA_PARAM;

typedef struct MdCamera_s {
    u16 bit; //0x00
    int focusType; //0x04
    HuVecF pos; //0x08
    HuVecF target; //0x14
    HuVecF cross; //0x20
    HuVecF up; //0x2C
    HuVecF dir; //0x38
    HuVecF hvr; //0x44
    float unk50;
    HuVecF center; //0x54
    HuVecF unk60[3];
    float fovy; //0x84
} MDCAMERA;

typedef struct MdCameraWork_s {
    BOOL createF; //0x00
    MDDEBUGFONT_WORK *debugFont; //0x04
    MDCAMERA camera[MDCAMERA_MAX]; //0x08
    int selCamera;
} MDCAMERA_WORK;

typedef struct MdLight_s {
    s16 type; //0x00
    HU3DLIGHTID gLightId; //0x02
    HU3DLLIGHTID lLightId; //0x04
    HuVecF hvr; //0x08
    HuVecF center; //0x14
    HuVecF pos; //0x20
    HuVecF target; //0x2C
    HuVecF lightPos; //0x38
    HuVecF lightDir; //0x44
    GXColor rgb; //0x50
} MDLIGHT;

typedef struct MdLightWork_s {
    BOOL createF; //0x00
    int lightSel; //0x04
    MDDEBUGFONT_WORK *debugFont; //0x08
    MDLIGHT light[MDLIGHT_MAX]; //0x0C
} MDLIGHT_WORK;

typedef struct MdSpriteWork_s {
    ANIMDATA *animP[MDSPR_ANIM_MAX]; //0x00
    HUSPRID sprId[MDSPR_MAX]; //0x24
    HUSPRGRPID grpId[MDSPR_GRP_MAX]; //0x40
} MDSPRITE_WORK;

typedef struct MdModelDebugInfo_s {
    char *name; //0x00
    int cameraNo; //0x04
} MDMODEL_DEBUG_INFO;

typedef struct MdModelWork_s {
    HU3DMODELID mdlId[MDMODEL_NUM]; //0x00
    HU3DMOTID motId[MDMODEL_MOT_NUM]; //0x0C
    HU3DANIMID toueiAnimId[MDMODEL_TOUEI_NUM]; //0x12
    HuVecF *posP[MDMODEL_NUM]; //0x18
    HuVecF *rotP[MDMODEL_NUM]; //0x30
    HuVecF *scaleP[MDMODEL_NUM]; //0x48
} MDMODEL_WORK;

typedef struct MdOmakeStar_s {
    s16 unk0;
    s16 mode; //0x02
    s16 motNext; //0x04
} MDOMAKE_STAR;

typedef struct MdMessWin_s {
    s16 mode;
    HUWINID winId; //0x02
    s16 choice; //0x04
    s16 unk6;
    s16 type; //0x08
    s16 unkA;
} MDMESS_WIN;

typedef struct MdMess_s {
    MDMESS_WIN win[MDMESS_WIN_MAX];
} MDMESS;

typedef struct MdSprInfo_s {
    s16 animNo; //0x00
    s16 pri; //0x02
    HuVec2f pos; //0x04
    HuVec2f scale; //0xC
    float tpLvl; //0x14
} MDSPR_INFO;

typedef struct MdSprGrpInfo_s {
    s16 start; //0x00
    s16 end; //0x02
    float posX; //0x04
    float posY; //0x08
} MDSPR_GRP_INFO;

typedef struct MdParticleWork_s MDPARTICLE_WORK;
typedef struct MdParticleBuffer_s MDPARTICLE_BUFFER;
typedef void (*MDPARTICLE_UPDATE_HOOK)(MDPARTICLE_BUFFER *spawner);


typedef struct MdParticle_s {
    int mode; //0x00
    int delay; //0x04
    float angle; //0x08
    float radius; //0x0C
    float tpLvl; //0x10
    float radiusSpeed; //0x14
    float angleSpeed; //0x18
    float velY; //0x1C
    float scale; //0x20
    float zRot; //0x24
    HuVecF pos; //0x28
    GXColor color; //0x34
} MDPARTICLE;

typedef struct MdParticleParam_s {
    float baseAlpha; //0x00
    float fadeInSpeed2; //0x04
    float fadeOutSpeed1; //0x08
    float fadeInSpeed1; //0x0C
    float fadeOutSpeed2; //0x10
    float unk14;
    float scaleSpeed1; //0x18
    float scaleSpeed2; //0x1C
    float unk20;
    GXColor color; //0x24
} MDPARTICLE_PARAM;

struct MdParticleBuffer_s {
    BOOL pauseF; //0x00
    ANIMDATA *animP; //0x04
    HU3DMODELID mdlId; //0x08
    s16 num; //0x0A
    MDPARTICLE *particle; //0x0C
    MDPARTICLE_PARAM *param; //0x10
    HU3DPARTICLE *hookWork; //0x14
    HU3DPARTICLEDATA *dataBuf; //0x18
    HU3DPARTICLEHOOK hook; //0x1C
    MDPARTICLE_UPDATE_HOOK updateHook; //0x20
};

struct MdParticleWork_s {
    MDPARTICLE data[MDPARTICLE_MAX]; //0x0
    MDPARTICLE_BUFFER buffer[2]; //0xA800
    MDPARTICLE_PARAM param; //0xA848
};

typedef struct Sys_s {
    int subMode; //0x00
    int unk4; //0x04
    int flowTime; //0x08
    int flowMode; //0x0C
    int ovlCallMode; //0x10
    OMOVL nextDll; //0x14
    s8 exitF; //0x18
    s8 skipWipeF; //0x19
    s8 selMode; //0x1A
    s8 prevSelMode; //0x1B
    OMOBJMAN *objman; //0x1C
    HUPROCESS *mainProc; //0x20
    HUPROCESS *loadProc; //0x24
    HUPROCESS *starProc; //0x28
    HUPROCESS *thpProc; //0x2C
    HUPROCESS *mainWinProc; //0x30
    HUPROCESS *helpWinProc; //0x34
    OMOBJ *obj[MDOMAKE_OBJ_MAX]; //0x38
    OMOVLHIS *ovlHis; //0x48
    s32 evtNo; //0x4C
    s16 thpState; //0x50
    s16 loadState; //0x52
    s32 loadAsyncStat; //0x54
    unsigned int loadDir; //0x58
    MDDEBUGFONT_WORK *debugFont; //0x5C
    MDPAD_WORK *pad; //0x60
    MDCAMERA_WORK *camera; //0x64
    MDLIGHT_WORK *light; //0x68
    MDSPRITE_WORK *sprite; //0x6C
    MDMODEL_WORK *model; //0x70
    MDOMAKE_STAR *star; //0x74
    MDMESS *mess; //0x78
    int procInitF[5]; //0x7C
    int turnDir; //0x90
    int unk94;
    float time[6]; //0x98
    float t[6]; //0xB0
} SYS;

extern SYS *sys;
extern SYS *sysP;
extern SYS sysWork;
extern MD_FUNC MdFlowTbl[MDOMAKE_FLOW_MAX];
extern MD_PROC MdProcTbl[MDOMAKE_PROC_MAX];

void MdMain(void);
void MdInit(void);
void MdClose(void);

void MdObjDebugFontCreate(OMOBJ *obj);
void MdObjDebugFontMain(OMOBJ *obj);
void MdObjPadMain(OMOBJ *obj);
void MdObjCameraCreate(OMOBJ *obj);
void MdObjCameraMain(OMOBJ *obj);
void MdObjLightCreate(OMOBJ *obj);
void MdObjLightMain(OMOBJ *obj);

void MdDebugFontCreate(MDDEBUGFONT_WORK *debugFont);
void MdDebugFontUpdate(MDDEBUGFONT_WORK *debugFont);
void MdDebugFontPrint(MDDEBUGFONT_WORK *debugFont, const char *str, ...);

void MdCameraCreate(MDCAMERA_WORK *cameraWork);
void MdCameraDebug(MDCAMERA_WORK *cameraWork);
void MdCameraUpdate(MDCAMERA_WORK *cameraWork);
void MdCameraPrint(MDCAMERA_WORK *cameraWork);
void MdCameraPosSet(MDCAMERA *camera, float h, float v, float r, float cx, float cy, float cz);

void MdLightCreate(MDLIGHT_WORK *lightWork);
void MdLightUpdate(MDLIGHT_WORK *lightWork);
void MdLightUpdateData(MDLIGHT *light);
void MdLightDebug(MDLIGHT_WORK *lightWork);
void MdLightPrint(MDLIGHT_WORK *lightWork);

void MdMessCreate(MDMESS *mess);
void MdMessKill(MDMESS *mess);
void MdMessMain(MDMESS *mess, int winNo);
void MdMessWinOpen(MDMESS *mess, int winNo, s16 type, s16 frame, s16 drawNo, s16 pri, float posX, float posY, s16 w, s16 h, float bgTPLvl);
void MdMessWinClose(MDMESS *mess, int winNo);
void MdMessWinKill(MDMESS *mess, int winNo);
void MdMessWinPosSet(MDMESS *mess, int winNo, float posX, float posY);

void MdMessWinSpeedSet(MDMESS *mess, int winNo, s16 speed);
void MdMessWinAttrSet(MDMESS *mess, int winNo, u32 attr);
void MdMessWinAttrReset(MDMESS *mess, int winNo, u32 attr);
void MdMessWinPad1Set(MDMESS *mess, int winNo);
void MdMessWinMesSet(MDMESS *mess, int winNo, BOOL choice, ...);
BOOL MdMessWinCheck(MDMESS *mess, int winNo);
s16 MdMessWinChoiceNowGet(MDMESS *mess, int winNo);
void MdMessWinChoiceSet(MDMESS *mess, int winNo, s16 choice);
int MdMessWinChoiceGet(MDMESS *mess, int winNo);
void MdMessWinHomeClear(MDMESS *mess, int winNo);
s16 MdMessWinStatGet(MDMESS *mess, int winNo);

Mtx *MdMathMatrixDir(HuVecF *start, HuVecF *end);
void MdMathMatrixRot(Mtx *matrix, HuVecF *rot);

void MdMathClamp(float *val, float min, float max);

void MdMathAngleClamp(float *angle);


void MdMathOscillate(int type, float *in, float *out, float t);
void MdMathLerp(float *out, float a, float b, float t);
void MdMathLerpOfs(float *out, float a, float b, float t, float ofs);
void MdMathLerpColor(u8 *out, float a, float b, float t);
void MdMathLerpScale(HuVecF *out, float a, float b, float t);
void MdMathLerpScaleOfs(HuVecF *out, float a, float b, float t, float ofs);
void MdMathStdCurveGet(int type, float *in, float *out, float t);

void MdMathCurveInit(int sampleNum, float *sampleT, float *sample, float *sampleSlope);
void MdMathCurveSample(float *out, int sampleNum, float *sampleT, float *sample, float *sampleSlope, float t);
void MdMathStdCurveInit(void);

void MdSpriteCreate(MDSPRITE_WORK *sprite);
HuVec2f *MdSpritePosGet(MDSPRITE_WORK *sprite, int grpNo, int memberNo);
HuVec2f *MdSpriteScaleGet(MDSPRITE_WORK *sprite, int grpNo, int memberNo);
float *MdSpriteAlphaGet(MDSPRITE_WORK *sprite, int grpNo, int memberNo);

void MdSpriteDispSet(MDSPRITE_WORK *sprite, int grpNo, BOOL dispF);
HuVec2f *MdSpriteGrpPosGet(MDSPRITE_WORK *sprite, int grpNo);
HuVec2f *MdSpriteGrpScaleGet(MDSPRITE_WORK *sprite, int grpNo);


void MdModelCreate(MDMODEL_WORK *model, MDCAMERA_WORK *camera);
void MdParticleCreate(MDPARTICLE_WORK *particle);
void MdParticleDataUpdate(int bufIdx);

#endif