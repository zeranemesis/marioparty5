#ifndef _MDBEACHDLL_H
#define _MDBEACHDLL_H

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

#define MD_OVLCALL_GOTO 0
#define MD_OVLCALL_CALL 1
#define MD_OVLCALL_RETURN 2

#define MDMESS_WIN_SPEAKER 0
#define MDMESS_WIN_NOSPEAKER 1
#define MDMESS_LIST_END -1

#define MDMATH_CURVE_SAMPLE_MAX 8

#define MDLIGHT_MAX 1

#define MdProcCall(idx, arg) MdProcTbl[idx](arg)
#define MdProcWait(idx, arg) do { \
    while(!MdProcCall(idx, arg)) { \
        HuPrcVSleep(); \
    } \
} while(0)

enum {
    MDBEACH_FLOW_INIT, //0
    MDBEACH_FLOW_CHAR_SEL, //1
    MDBEACH_FLOW_GAME_SEL, //2
    MDBEACH_FLOW_ORDER_SEL, //3
    MDBEACH_FLOW_VS_GAME, //4
    MDBEACH_FLOW_GAME, //5
    MDBEACH_FLOW_MAX //6
};

enum {
    MDBEACH_PROC_RESET, //0
    MDBEACH_PROC_CAMERA_LERP, //1
    MDBEACH_PROC_CHR_SEL, //2
    MDBEACH_PROC_CHAR_MOTION, //3
    MDBEACH_PROC_GAME_INIT, //4
    MDBEACH_PROC_GAME_SEL, //5
    MDBEACH_PROC_ORDER_SEL, //6
    MDBEACH_PROC_CHR_POS, //7
    MDBEACH_PROC_CHR_GRP_POS, //8
    MDBEACH_PROC_MODEL_POS, //9
    MDBEACH_PROC_MODEL_SCALE, //10
    MDBEACH_PROC_CHAR_SCALE, //11
    MDBEACH_PROC_CAPMAN_SCALE, //12
    MDBEACH_PROC_MAX //13
};

enum {
    MDCAMERA_MAIN, //0
    MDCAMERA_GUIDE, //1
    MDCAMERA_MAX //2
};

enum {
    MD_LOAD_STATE_NONE, //0
    MD_LOAD_STATE_LOAD, //1
    MD_LOAD_STATE_WAIT, //2
    MD_LOAD_STATE_MOT, //3
    MD_LOAD_STATE_END //4
};

enum {
    MDMESS_WIN_MAIN, //0
    MDMESS_WIN_SUB, //1
    MDMESS_WIN_SELECT, //2
    MDMESS_WIN_HELP, //3
    MDMESS_WIN_INFO, //4
    MDMESS_WIN_MAX //5
};

enum {
    MDBEACH_OBJ_DEBUGFONT, //0
    MDBEACH_OBJ_PAD, //1
    MDBEACH_OBJ_CAMERA, //2
    MDBEACH_OBJ_LIGHT, //3
    MDBEACH_OBJ_MAX //4
};

enum {
    MDSPR_ANIM_WIN_FRAME, //0
    MDSPR_ANIM_WIN_BG, //1
    MDSPR_ANIM_CURSOR, //2
    MDSPR_ANIM_CURSOR_GLOW, //3
    MDSPR_ANIM_MODE, //4
    MDSPR_ANIM_MAX = 18 //18
};

enum {
    MDSPR_WIN_FRAME_TOP, //0
    MDSPR_WIN_FRAME_BOTTOM, //1
    MDSPR_WIN_BG, //2
    MDSPR_CURSOR_LR_R, //3
    MDSPR_CURSOR_LR_L, //4
    MDSPR_CURSOR_UDLR_R, //5
    MDSPR_CURSOR_UDLR_L, //6
    MDSPR_CURSOR_UDLR_U, //7
    MDSPR_CURSOR_UDLR_D, //8
    MDSPR_CURSOR_GLOW_UDLR_R, //9
    MDSPR_CURSOR_GLOW_UDLR_L, //10
    MDSPR_CURSOR_GLOW_UDLR_U, //11
    MDSPR_CURSOR_GLOW_UDLR_D, //12
    MDSPR_MODE0, //13
    MDSPR_MODE1, //14
    MDSPR_MODE2, //15
    MDSPR_MODE3, //16
    MDSPR_MODE4, //17
    MDSPR_MODE5, //18
    MDSPR_MODE6, //19
    MDSPR_MODE7, //20
    MDSPR_MODE8, //21
    MDSPR_MODE9, //22
    MDSPR_MODE10, //23
    MDSPR_MODE11, //24
    MDSPR_MODE12, //25
    MDSPR_MODE13, //26
    MDSPR_MODE14, //27
    MDSPR_MODE15, //28
    MDSPR_MODE16, //29
    MDSPR_MAX //30
};

enum {
    MDSPR_GRP_WIN, //0
    MDSPR_GRP_CURSOR_LR, //1
    MDSPR_GRP_CURSOR_UDLR, //2
    MDSPR_GRP_CHAR, //3
    MDSPR_GRP_CAPMAN, //4
    MDSPR_GRP_MODEL, //5
    MDSPR_GRP_MODE0, //6
    MDSPR_GRP_MODE1, //7
    MDSPR_GRP_MODE2, //8
    MDSPR_GRP_MAX //9
};

enum {
    MDMODEL_STAGE, //0

    MDMODEL_CHAR0, //1
    MDMODEL_CHAR1, //2
    MDMODEL_CHAR2, //3
    MDMODEL_CHAR3, //4
    MDMODEL_CHAR4, //5
    MDMODEL_CHAR5, //6
    MDMODEL_CHAR6, //7
    MDMODEL_CHAR7, //8
    MDMODEL_CHAR8, //9
    MDMODEL_CHAR9, //10

    MDMODEL_VOLLEY0, //11
    MDMODEL_VOLLEY1, //12
    MDMODEL_VOLLEY2, //13
    MDMODEL_VOLLEY3, //14
    MDMODEL_VOLLEY4, //15
    MDMODEL_VOLLEY5, //16
    MDMODEL_VOLLEY6, //17
    MDMODEL_VOLLEY7, //18
    MDMODEL_VOLLEY8, //19
    MDMODEL_VOLLEY9, //20

    MDMODEL_HOCKEY0, //21
    MDMODEL_HOCKEY1, //22
    MDMODEL_HOCKEY2, //23
    MDMODEL_HOCKEY3, //24
    MDMODEL_HOCKEY4, //25
    MDMODEL_HOCKEY5, //26
    MDMODEL_HOCKEY6, //27
    MDMODEL_HOCKEY7, //28
    MDMODEL_HOCKEY8, //29
    MDMODEL_HOCKEY9, //30

    MDMODEL_CAPMAN1P, //31
    MDMODEL_CAPMAN2P, //32
    MDMODEL_CAPMAN3P, //33
    MDMODEL_CAPMAN4P, //34

    MDMODEL_CAPCOM1P, //35
    MDMODEL_CAPCOM2P, //36
    MDMODEL_CAPCOM3P, //37
    MDMODEL_CAPCOM4P, //38

    MDMODEL_FINGER1P, //39
    MDMODEL_FINGER2P, //40
    MDMODEL_FINGER3P, //41
    MDMODEL_FINGER4P, //42

    MDMODEL_FLASH1P, //43
    MDMODEL_FLASH2P, //44
    MDMODEL_FLASH3P, //45
    MDMODEL_FLASH4P, //46

    MDMODEL_VS_VOLLEY, //47
    MDMODEL_VS_HOCKEY, //48

    MDMODEL_BALL_NORMAL, //49
    MDMODEL_BALL_BOMB, //50
    MDMODEL_BALL_DICE, //51

    MDMODEL_FONTBGFILTER, //52
    MDMODEL_MAX, //53

    MDMODEL_CHAR_START = MDMODEL_CHAR0,
    MDMODEL_CHAR_END = MDMODEL_CHAR9 + 1,
    MDMODEL_CHAR_NUM = MDMODEL_CHAR_END - MDMODEL_CHAR_START, //10

    MDMODEL_VOLLEY_START = MDMODEL_VOLLEY0,
    MDMODEL_VOLLEY_END = MDMODEL_VOLLEY9 + 1,
    MDMODEL_VOLLEY_NUM = MDMODEL_VOLLEY_END - MDMODEL_VOLLEY_START, //10

    MDMODEL_HOCKEY_START = MDMODEL_HOCKEY0,
    MDMODEL_HOCKEY_END = MDMODEL_HOCKEY9 + 1,
    MDMODEL_HOCKEY_NUM = MDMODEL_HOCKEY_END - MDMODEL_HOCKEY_START, //10

    MDMODEL_CAPMAN_START = MDMODEL_CAPMAN1P,
    MDMODEL_CAPMAN_END = MDMODEL_CAPMAN4P + 1,
    MDMODEL_CAPMAN_NUM = MDMODEL_CAPMAN_END - MDMODEL_CAPMAN_START, //4

    MDMODEL_CAPCOM_START = MDMODEL_CAPCOM1P,
    MDMODEL_CAPCOM_END = MDMODEL_CAPCOM4P + 1,
    MDMODEL_CAPCOM_NUM = MDMODEL_CAPCOM_END - MDMODEL_CAPCOM_START, //4

    MDMODEL_FINGER_START = MDMODEL_FINGER1P,
    MDMODEL_FINGER_END = MDMODEL_FINGER4P + 1,
    MDMODEL_FINGER_NUM = MDMODEL_FINGER_END - MDMODEL_FINGER_START, //4

    MDMODEL_FLASH_START = MDMODEL_FLASH1P,
    MDMODEL_FLASH_END = MDMODEL_FLASH4P + 1,
    MDMODEL_FLASH_NUM = MDMODEL_FLASH_END - MDMODEL_FLASH_START, //4

    MDMODEL_BALL_START = MDMODEL_BALL_NORMAL,
    MDMODEL_BALL_END = MDMODEL_BALL_DICE + 1,
    MDMODEL_BALL_NUM = MDMODEL_BALL_END - MDMODEL_BALL_START //3
};

enum {
    MDMODEL_MOT_CHAR0, //0
    MDMODEL_MOT_CHAR1, //1
    MDMODEL_MOT_CHAR2, //2
    MDMODEL_MOT_CHAR3, //3
    MDMODEL_MOT_CHAR4, //4
    MDMODEL_MOT_CHAR5, //5
    MDMODEL_MOT_CHAR6, //6
    MDMODEL_MOT_CHAR7, //7
    MDMODEL_MOT_CHAR8, //8
    MDMODEL_MOT_CHAR9, //9
    MDMODEL_MOT_CAPMAN, //10
    MDMODEL_MOT_CAPCOM, //11
    MDMODEL_MOT_FINGER, //12
    MDMODEL_MOT_FLASH, //13
    MDMODEL_MOT_BALL, //14
    MDMODEL_MOT_CAP_LR, //15
    MDMODEL_MOT_CAP_UD, //16
    MDMODEL_MOT_MAX = 33 //33
};

typedef void (*MD_FUNC)(void);
typedef BOOL (*MD_PROC)(int param);

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

typedef struct MdCamera_s {
    u16 bit; //0x00
    int focusType; //0x04
    HuVecF pos; //0x08
    HuVecF target; //0x14
    HuVecF cross; //0x20
    HuVecF up; //0x2C
    HuVecF dir; //0x38
    HuVecF hvr; //0x44
    float unk50; //0x50
    HuVecF center; //0x54
    HuVecF unk60[3]; //0x60
    float fovy; //0x84
} MDCAMERA;

typedef struct MdCameraWork_s {
    BOOL createF; //0x00
    MDDEBUGFONT_WORK *debugFont; //0x04
    MDCAMERA camera[MDCAMERA_MAX]; //0x08
    int selCamera; //0x118
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
    HUSPRID sprId[MDSPR_MAX]; //0x48
    HUSPRGRPID grpId[MDSPR_GRP_MAX]; //0x84
} MDSPRITE_WORK;

typedef struct MdModelDebugInfo_s {
    char *name; //0x00
    int cameraNo; //0x04
} MDMODEL_DEBUG_INFO;

typedef struct MdModelWork_s {
    HU3DMODELID mdlId[MDMODEL_MAX]; //0x00
    HU3DMOTID motId[MDMODEL_MOT_MAX]; //0x6A
    HU3DANIMID toueiAnmId[4]; //0xAC
    HuVecF *posP[MDMODEL_MAX]; //0xB4
    HuVecF *rotP[MDMODEL_MAX]; //0x188
    HuVecF *scaleP[MDMODEL_MAX]; //0x25C
} MDMODEL_WORK;

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

typedef struct MdStar_s {
    s16 unk0; //0x00
    s16 mode; //0x02
    s16 motNext; //0x04
    HuVecF pos; //0x08
} MDBEACH_STAR;

typedef struct MdBeachPosWork_s {
    s32 mode; //0x00
    f32 in[4]; //0x04
    f32 ofs; //0x14
    f32 unk18;
    f32 unk1C;
} MDBEACH_POS_WORK;

typedef struct MdBeachStateWork_s {
    s32 mode; //0x00
    f32 in0; //0x04
    f32 in1; //0x08
    f32 t; //0x0C
    f32 ofs; //0x10
} MDBEACH_STATE_WORK;

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
    s8 unk1B; //0x1B
    s8 unk1C; //0x1C
    s8 unk1D; //0x1D
    s8 unk1E[4]; //0x1E
    s8 padEnable[4]; //0x22
    s8 chrSel[4]; //0x26
    s8 chrSelEnd[4]; //0x2A
    s8 chrNo[4]; //0x2E
    s8 unk32[4]; //0x32
    s8 unk36[4]; //0x36
    s8 comPlayerNo[4]; //0x3A
    s8 unk3E[4]; //0x3E
    s8 teamPlayer[3]; //0x42
    s8 selPlayerNo; //0x45
    s8 selMapNo; //0x46
    s8 chrSel2[2][2]; //0x47
    s8 chrGrpNo[4]; //0x4B
    s8 unk4F[4]; //0x4F
    s8 unk53[2]; //0x53
    s8 unk55[3]; //0x55
    s8 unk58; //0x58
    s8 unk59; //0x59
    s8 unk5A; //0x5A
    s8 unk5B; //0x5B
    OMOBJMAN *objman; //0x5C
    HUPROCESS *mainProc; //0x60
    HUPROCESS *loadProc; //0x64
    HUPROCESS *starProc; //0x68
    HUPROCESS *gameSelProc; //0x6C
    HUPROCESS *gameInitProc; //0x70
    HUPROCESS *cursorProc; //0x74
    HUPROCESS *mainWinProc; //0x78
    HUPROCESS *helpWinProc; //0x7C
    HUPROCESS *subWinProc; //0x80
    OMOBJ *obj[MDBEACH_OBJ_MAX]; //0x84
    OMOVLHIS *ovlHis; //0x94
    s32 evtNo; //0x98
    s16 loadState; //0x9C
    s16 loadAsyncStat; //0x9E
    unsigned int loadDir; //0xA0
    s16 loadProcReady; //0xA4
    MDDEBUGFONT_WORK *debugFont; //0xA8
    MDPAD_WORK *pad; //0xAC
    MDCAMERA_WORK *camera; //0xB0
    MDLIGHT_WORK *light; //0xB4
    MDSPRITE_WORK *sprite; //0xB8
    MDMODEL_WORK *model; //0xBC
    MDBEACH_STAR *star; //0xC0
    MDBEACH_POS_WORK posWork[MDMODEL_CHAR_NUM]; //0xC4
    MDBEACH_STATE_WORK chrWork[4]; //0x204
    MDBEACH_STATE_WORK capWork[4]; //0x254
    MDMESS *mess; //0x2A4
    s32 unk2A8; //0x2A8
    s32 unk2AC; //0x2AC
    s32 unk2B0; //0x2B0
    s32 unk2B4; //0x2B4
    s32 unk2B8; //0x2B8
    s32 unk2BC; //0x2BC
    s32 unk2C0; //0x2C0
    s32 unk2C4; //0x2C4
    s32 unk2C8; //0x2C8
    float oscIn[5]; //0x2CC
    float oscOut[5]; //0x2E0
} SYS;

typedef struct MdSpriteInfo_s {
    s16 animNo; //0x00
    s16 pri; //0x02
    HuVec2f pos; //0x04
    HuVec2f scale; //0x0C
    float tpLvl; //0x14
} MDSPR_INFO;

typedef struct MdSpriteGrpInfo_s {
    s16 start; //0x00
    s16 end; //0x02
    float posX; //0x04
    float posY; //0x08
} MDSPR_GRP_INFO;

typedef struct MdDotWork_s {
    s32 man[3];
} MDDOT_WORK;

extern SYS *sys;
extern SYS *sysP;
extern SYS sysWork;
extern MD_FUNC MdFlowTbl[MDBEACH_FLOW_MAX];
extern MD_PROC MdProcTbl[MDBEACH_PROC_MAX];

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
void MdMessWinScaleSet(MDMESS *mess, int winNo, float scaleX, float scaleY);
void MdMessWinCenterPosSet(MDMESS *mess, int winNo, float posX, float posY);

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
void MdMathCurveGet(float *out, int sampleNum, float *sampleT, float *sample, float *sampleSlope, float t);
void MdMathStdCurveInit(void);

void MdSpriteCreate(MDSPRITE_WORK *sprite);
void MdSpriteDispSet(MDSPRITE_WORK *sprite, int grpNo, BOOL dispF);
HuVec2f *MdSpritePosGet(MDSPRITE_WORK *sprite, int grpNo, int memberNo);
HuVec2f *MdSpriteScaleGet(MDSPRITE_WORK *sprite, int grpNo, int memberNo);
float *MdSpriteAlphaGet(MDSPRITE_WORK *sprite, int grpNo, int memberNo);
HuVec2f *MdSpriteGrpPosGet(MDSPRITE_WORK *sprite, int grpNo);
HuVec2f *MdSpriteGrpScaleGet(MDSPRITE_WORK *sprite, int grpNo);

void MdModelCreate(MDMODEL_WORK *model, MDCAMERA_WORK *camera);
void MdToueiMdlInit(void);

#endif