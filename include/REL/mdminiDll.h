#ifndef _MDMINIDLL_H
#define _MDMINIDLL_H

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

#define MDMINI_FLOW_MODE_MAX 7
#define MDMINI_EFFECT_MAX 15
#define MDMINI_MODEL_MAX 10
#define MDMINI_MOTION_MAX 3

#define MDMESS_WIN_MAX 2

enum {
    MDMATH_CURVE_STD_BOUNCE, //0
    MDMATH_CURVE_STD_LINE, //1
    MDMATH_CURVE_STD_MAX // 2
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
    MDMINI_FLOW_INIT, //0
    MDMINI_FLOW_PLAYER_SEL, //1
    MDMINI_FLOW_SEL_TYPE, //2
    MDMINI_FLOW_SEL_DIF, //3
    MDMINI_FLOW_PLAYER_ANIM, //4
    MDMINI_FLOW_STAGE_MODEL, //5
    MDMINI_FLOW_MODEL_MOVE, //6
    MDMINI_FLOW_MAX //7
};

enum {
    MDMINI_OBJ_DEBUGFONT, // 0
    MDMINI_OBJ_PAD, //1
    MDMINI_OBJ_CAMERA, //2
    MDMINI_OBJ_LIGHT, //3
    MDMINI_OBJ_MAX //4
};

enum {
    MDMINI_ANIM_WIN_FRAME_TOP, //0
    MDMINI_ANIM_WIN_BG, //1
    MDMINI_ANIM_CURSOR, //2
    MDMINI_ANIM_CURSOR_GLOW, //3
    MDMINI_ANIM_EFF_GLOW, //4
    MDMINI_ANIM_MDMINI, //5
    MDMINI_ANIM_PARTICLE_0, //6
    MDMINI_ANIM_PARTICLE_1, //7
    MDMINI_ANIM_PARTICLE_2, //8
    MDMINI_ANIM_PARTICLE_DOT, //9
    MDMINI_ANIM_PARTICLE_GLOW, //10
    MDMINI_ANIM_MAX //11
};

enum {
    MDSPR_WIN_FRAME_TOP, //0
    MDSPR_WIN_BG, //1
    MDSPR_CURSOR_LR_R, //2
    MDSPR_CURSOR_LR_L, //3
    MDSPR_CURSOR_UDLR_R, //4
    MDSPR_CURSOR_UDLR_L, //5
    MDSPR_CURSOR_UDLR_U, //6
    MDSPR_CURSOR_UDLR_D, //7
    MDSPR_CURSOR_GLOW_UDLR_R, //8
    MDSPR_CURSOR_GLOW_UDLR_L, //9
    MDSPR_CURSOR_GLOW_UDLR_U, //10
    MDSPR_CURSOR_GLOW_UDLR_D, //11
    MDSPR_MDMINI, //12
    MDSPR_PARTICLE_0, //13
    MDSPR_PARTICLE_1, //14
    MDSPR_PARTICLE_2, //15
    MDSPR_PARTICLE_DOT, //16
    MDSPR_PARTICLE_GLOW, //17
    MDSPR_PARTICLE_0_2, //18
    MDSPR_PARTICLE_1_2, //19
    MDSPR_PARTICLE_2_2, //20
    MDSPR_PARTICLE_DOT_2, //21
    MDSPR_PARTICLE_GLOW_2, //22
    MDSPR_MAX //23
};

enum {
    MDSPR_GRP_WIN, //0
    MDSPR_GRP_CURSOR_LR, //1
    MDSPR_GRP_CURSOR_UDLR, //2
    MDSPR_GRP_MODE, //3
    MDSPR_GRP_MEMBER, //4
    MDSPR_GRP_MEMBER2, //5
    MDSPR_GRP_MAX //6
};

enum {
    MDMODEL_STAGE, //0

    MDMODEL_CAP_START,
    MDMODEL_CAP_PARTICLE = MDMODEL_CAP_START, //1
    MDMODEL_CAP_CHORU, //2
    MDMODEL_CAP_NERU, //3
    MDMODEL_CAP_NIRU, //4
    MDMODEL_CAP_HARU, //5
    MDMODEL_CAP_MARU, //6
    MDMODEL_CAP_DARU, //7
    MDMODEL_CAP_END,
    MDMODEL_CAP_NUM = MDMODEL_CAP_END-MDMODEL_CAP_START,

    MDMODEL_MODE_START = MDMODEL_CAP_END,
    MDMODEL_MODE_00 = MDMODEL_MODE_START, //8
    MDMODEL_MODE_01, //9
    MDMODEL_MODE_02, //10
    MDMODEL_MODE_03, //11
    MDMODEL_MODE_04, //12
    MDMODEL_MODE_05, //13
    MDMODEL_MODE_06, //14
    MDMODEL_MODE_07, //15
    MDMODEL_MODE_08, //16
    MDMODEL_MODE_09, //17
    MDMODEL_MODE_END,
    MDMODEL_MODE_NUM = MDMODEL_MODE_END-MDMODEL_MODE_START,

    MDMODEL_CHAR_START = MDMODEL_MODE_END,
    MDMODEL_CHAR_00 = MDMODEL_CHAR_START, //18
    MDMODEL_CHAR_01, //19
    MDMODEL_CHAR_02, //20
    MDMODEL_CHAR_03, //21
    MDMODEL_CHAR_04, //22
    MDMODEL_CHAR_05, //23
    MDMODEL_CHAR_06, //24
    MDMODEL_CHAR_07, //25
    MDMODEL_CHAR_08, //26
    MDMODEL_CHAR_09, //27
    MDMODEL_CHAR_END,
    MDMODEL_CHAR_NUM = MDMODEL_CHAR_END-MDMODEL_CHAR_START,

    MDMODEL_CHARGE_START = MDMODEL_CHAR_END,
    MDMODEL_CHARGE_00 = MDMODEL_CHARGE_START, //28
    MDMODEL_CHARGE_01, //29
    MDMODEL_CHARGE_02, //30
    MDMODEL_CHARGE_03, //31
    MDMODEL_CHARGE_04, //32
    MDMODEL_CHARGE_05, //33
    MDMODEL_CHARGE_END,
    MDMODEL_CHARGE_NUM = MDMODEL_CHARGE_END-MDMODEL_CHARGE_START,

    MDMODEL_TOU1 = MDMODEL_CHARGE_END, //34
    MDMODEL_TOU2, //35
    MDMODEL_TOU3, //36
    MDMODEL_TOU4, //37
    MDMODEL_TOU5, //38
    MDMODEL_TOU6, //39
    MDMODEL_TOU7, //40
    MDMODEL_TOU8, //41
    MDMODEL_TOU9, //42
    MDMODEL_TOU10, //43
    MDMODEL_STAR, //44
    MDMODEL_WIN, //45
    MDMODEL_PARTICLE, //46
    MDMODEL_FONTBG, //47
    MDMODEL_FONTBG_FILTER, //48
    MDMODEL_MAX //49
};

enum {
    MDMINI_RING_MODE_NONE,
    MDMINI_RING_MODE_SET,
    MDMINI_RING_MODE_MOVE,
    MDMINI_RING_MODE_FREE,
    MDMINI_RING_MODE_ERASE,
    MDMINI_RING_MODE_WAIT
};

enum {
    MDMATH_CURVE_SAMPLE_NUM_BOUNCE,
    MDMATH_CURVE_SAMPLE_NUM_LINE
};

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
    MDCAMERA camera[2]; //0x08
    s16 selCamera; //0x118
    u8 pad[0x8A];
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
    ANIMDATA *animP[MDMINI_ANIM_MAX]; //0x00
    HUSPRID sprId[MDSPR_MAX]; //0x2C
    HUSPRGRPID grpId[MDSPR_GRP_MAX]; //0x5C
} MDSPRITE_WORK;

typedef struct MdModelWork_s {
    HU3DMODELID mdlId[MDMODEL_MAX]; //0x00
    HU3DMOTID motId[29]; //0x60
    HuVecF *posP[MDMODEL_MAX]; //0x9C
    HuVecF *rotP[MDMODEL_MAX]; //0x160
    HuVecF *scaleP[MDMODEL_MAX]; //0x224
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

typedef struct MdTexBufWork_s {
    u8 pad[0x1E4];
} MDTEXBUF_WORK;

typedef struct MdRingWork_s {
    u8 pad[0x114];
} MDRING_WORK;

typedef struct MdMiniStar_s {
    s8 unk0;
    s8 unk1;
    s16 unk2;
    s16 unk4;
} MDMINI_STAR;

typedef struct MdMiniPlayerAnim_s {
    int state; //0x00
    float t1; //0x04
    float t2; //0x08
    float u1; //0x0C
    float u2; //0x10
} MDMINI_PLAYERANIM;

typedef struct MdMiniModelAnim_s {
    int state; //0x00
    float t1; //0x04
    float t2; //0x08
    float u1; //0x0C
    float u2; //0x10
    HuVecF pos1; //0x14
    HuVecF pos2; //0x20
    HuVecF rot1; //0x2C
    HuVecF rot2; //0x38
} MDMINI_MODELANIM;

typedef struct MdParticleWork_s {
    s32 manId[3];
} MDPARTICLE_DOT;

typedef struct MDParticleDotGlow_s {
    s32 manId;
} MDPARTICLE_GLOW;

typedef s32 (*MDMINI_EFFECT_FUNC)(s32 param);

typedef struct Sys_s {
    int mode; //0x00
    int unk4; //0x04
    int flowTime; //0x08
    int flowMode; //0x0C
    int ovlCallMode; //0x10
    OMOVL nextDll; //0x14
    s8 exitF; //0x18
    s8 skipWipeF; //0x19
    s8 selMode; //0x1A
    s8 prevSelMode; //0x1B
    s8 playerNum; //0x1C
    s8 padConnectF[GW_PLAYER_MAX]; //0x1D
    s8 inUseF[GW_PLAYER_MAX]; //0x21
    s8 padNo[GW_PLAYER_MAX]; //0x25
    s8 padNoPrev[GW_PLAYER_MAX]; //0x29
    s8 charNo[GW_PLAYER_MAX]; //0x2D
    s8 anmNo[GW_PLAYER_MAX]; //0x31
    s8 difNo[GW_PLAYER_MAX]; //0x35
    s8 selSub[MDMINI_MOTION_MAX]; //0x39
    s8 selNo; //0x3C
    s8 selNoNow; //0x3D
    s8 selNoPrev; //0x3E
    s8 mgTypeF[6]; //0x3F
    s8 scoreNum; //0x45
    s8 gameType; //0x46
    s8 subMode; //0x47
    s16 mdlA[2]; //0x48
    s8 mdlB[GW_PLAYER_MAX]; //0x4C
    s8 mdlBPrev[GW_PLAYER_MAX]; //0x50
    s8 mdlC[2]; //0x54
    s8 mdlD[3]; //0x56
    s8 subMode2; //0x59
    s8 mdlE[GW_PLAYER_MAX]; //0x5A
    s8 mdlF[GW_PLAYER_MAX]; //0x5E
    s8 mdlG[3]; //0x62
    OMOBJMAN *objman; //0x68
    HUPROCESS *mainProc; //0x6C
    HUPROCESS *loadProc; //0x70
    HUPROCESS *procA; //0x74
    HUPROCESS *procB; //0x78
    HUPROCESS *procC; //0x7C
    HUPROCESS *ringProc; //0x80
    HUPROCESS *messProc[2]; //0x84
    OMOBJ *obj[MDMINI_OBJ_MAX]; //0x8C
    OMOVLHIS *ovlHis; //0x9C
    int evtNo; //0xA0
    s16 loadState; //0xA4
    s16 loadAsyncStat; //0xA6
    unsigned int loadDir; //0xA8
    MDDEBUGFONT_WORK *debugFont; //0xAC
    MDPAD_WORK *pad; //0xB0
    MDCAMERA_WORK *camera; //0xB4
    MDLIGHT_WORK *light; //0xB8
    MDSPRITE_WORK *sprite; //0xBC
    MDMODEL_WORK *model; //0xC0
    MDTEXBUF_WORK *texbuf; //0xC4
    MDMINI_STAR *star; //0xC8
    MDMINI_PLAYERANIM playerAnim[GW_PLAYER_MAX]; //0xCC
    MDMINI_MODELANIM modelAnim[MDMINI_MODEL_MAX]; //0x11C
    MDMESS *mess; //0x3C4
    MDRING_WORK *ring; //0x3C8
    int initF[7]; //0x3CC
    int dirF; //0x3E4
    int unk3E8; //0x3E8
    float oscT[7]; //0x3EC
    float oscOut[7]; //0x408
    float curveT[5]; //0x424
    float curveSlope[5]; //0x444
    float pad4A0[0x4];
} SYS;

extern SYS *sys;
extern SYS *sysP;
extern SYS sysWork;

/* --- object.c ------------------------------------------------------------ */

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

void MdMathMatrixDir(HuVecF *start, HuVecF *end, Mtx *matrix);
void MdMathMatrixRot(Mtx *matrix, HuVecF *rot);

void MdMathClamp(float *val, float min, float max);
void MdMathAngleClamp(float *angle);
void MdMathOscillate(int type, float *t, float *out, float speed);
void MdMathStdCurveGet(int type, float *in, float *out, float t);
void MdMathLerp(float *out, float a, float b, float t);
void MdMathLerpOfs(float *out, float a, float b, float t, float ofs);
void MdMathLerpColor(u8 *out, float a, float b, float t);
void MdMathLerpScale(HuVecF *out, float a, float b, float t);
void MdMathLerpScaleOfs(HuVecF *out, float a, float b, float t, float ofs);
void MdMathCurveInit(int sampleNum, float *sampleT, float *sample, float *sampleSlope);
void MdMathCurveGet(float *out, int sampleNum, float *sampleT, float *sample, float *sampleSlope,
                   float *t, float time);
void MdMathStdCurveInit(void);

void MdMessCreate(MDMESS *mess);
void MdMessKill(MDMESS *mess);
void MdMessMain(MDMESS *mess, int winNo);
void MdMessWinOpen(MDMESS *mess, int winNo, s16 type, s16 frame, s16 drawNo, s16 pri,
                   float posX, float posY, s16 w, s16 h, float bgTPLvl);
void MdMessWinClose(MDMESS *mess, int winNo);
void MdMessWinKill(MDMESS *mess, int winNo);
void MdMessWinPosSet(MDMESS *mess, int winNo, float posX, float posY);
void MdMessWinSpeedSet(MDMESS *mess, int winNo, s16 speed);
void MdMessWinPad1Set(MDMESS *mess, int winNo);
void MdMessWinHomeClear(MDMESS *mess, int winNo);
void MdMessWinMesSet(MDMESS *mess, int winNo, BOOL choice, ...);
BOOL MdMessWinCheck(MDMESS *mess, int winNo);
s16 MdMessWinChoiceNowGet(MDMESS *mess, int winNo);
void MdMessWinChoiceSet(MDMESS *mess, int winNo, s16 choice);
int MdMessWinChoiceGet(MDMESS *mess, int winNo);
void MdMessWinAttrSet(MDMESS *mess, int winNo, u32 attr);
void MdMessWinAttrReset(MDMESS *mess, int winNo, u32 attr);

void MdSpriteCreate(MDSPRITE_WORK *sprite);
void MdSpriteDispSet(MDSPRITE_WORK *sprite, int grpNo, BOOL dispF);
HuVec2f *MdSpriteGrpPosGet(MDSPRITE_WORK *sprite, int grpNo);
HuVec2f *MdSpriteGrpScaleGet(MDSPRITE_WORK *sprite, int grpNo);
HuVec2f *MdSpritePosGet(MDSPRITE_WORK *sprite, int grpNo, int memberNo);
HuVec2f *MdSpriteScaleGet(MDSPRITE_WORK *sprite, int grpNo, int memberNo);

void MdModelCreate(MDMODEL_WORK *model, MDCAMERA_WORK *camera);

/* --- mdmini.c ------------------------------------------------------------ */

void ObjectSetup(void);
void MdMain(void);
void MdPosAdjustMode(void);

/* --- stage.c ------------------------------------------------------------- */

void FontBGFilter(void);
void LayerPostHook(void);
void Layer2Hook(void);
void Layer1Hook(void);
void MdToueiLayerHook(s16 layerNo);
void TexBufSet(void);
void Touei1MatHook(void);
void Touei2MatHook(void);
void Touei3MatHook(void);
void Touei4MatHook(void);
void Touei5MatHook(void);
void Touei6MatHook(void);
void Touei7MatHook(void);
void Touei8MatHook(void);
void Touei9MatHook(void);
void Touei10MatHook(void);
void MdToueiMdlInit(void);
void MdToueiTexBufInit(void);
void TexBufFBCopy(void);
void MdTexBufObjPosSet(void);
void MdTexBufPosSet(void);
void ToueiZBufClear(void);

void MdRingCreate(void);
void MdRingMain(void);
void MdRingModeSet(s32 ringNo, s16 mode);
void MdRingDispSet(s32 ringNo, BOOL dispF);
void MdRingScaleSet(s32 ringNo, double scale);
void MdRingMdlSet(s32 ringNo, HU3DMODELID mdlId);
void MdRingOfsSet(s32 ringNo, float x, float y, float z);
void MdRingColorReset(s32 ringNo);
void RingDraw(void);
void Ring1MatHook(void);
void Ring2MatHook(void);
void Ring3MatHook(void);
void Ring4MatHook(void);

#endif
