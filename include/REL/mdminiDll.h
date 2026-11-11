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

#define MDSPR_GRP_MEMBERNO(grp, spr) ((spr)-(grp##_START))

enum {
    MDCAMERA_BG, //0
    MDCAMERA_MAIN, //1
    MDCAMERA_FG, //2
    MDCAMERA_MAX //3
};

#define MDMINI_FLOW_MODE_MAX 7
#define MDMINI_EFFECT_MAX 15
#define MDMINI_MODEL_MAX 10
#define MDMINI_MOTION_MAX 3

#define MDMESS_WIN_MAX 2

#define MDMINI_TEXBUF_MIRROR 0
#define MDMINI_TEXBUF_MAX 10
#define MDMINI_TEXBUF_W 64
#define MDMINI_TEXBUF_H 64

enum {
    MD_LOAD_STATE_NONE, //0
    MD_LOAD_STATE_SEL_LOAD, //1
    MD_LOAD_STATE_LOAD, //2
    MD_LOAD_STATE_BOARD_LOAD, //3
    MD_LOAD_STATE_DONE //4
};

enum {
    MD_DEBUGFONT_CTRL_NOP, //0
    MD_DEBUGFONT_CTRL_CAMERA, //1
    MD_DEBUGFONT_CTRL_LIGHT, //2
    MD_DEBUGFONT_CTRL_MODEL, //3
    MD_DEBUGFONT_CTRL_MAX //4
};

enum {
    MDCAMERA_FOCUS_TARGET, //0
    MDCAMERA_FOCUS_POS, //1
    MDCAMERA_FOCUS_MAX //2
};

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

#define MdProcCall(idx, arg) MdProcTbl[idx](arg)
#define MdProcWait(idx, arg) do {     while(!MdProcCall(idx, arg)) {         HuPrcVSleep();     } } while(0)

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
    MDMINI_ANIM_WIN_FRAME, //0
    MDMINI_ANIM_WIN_BG, //1
    MDMINI_ANIM_CURSOR, //2
    MDMINI_ANIM_CURSOR_GLOW, //3
    MDMINI_ANIM_PLAYERNO, //4
    MDMINI_ANIM_COMDIF, //5
    MDMINI_ANIM_6, //6
    MDMINI_ANIM_7, //7
    MDMINI_ANIM_8, //8
    MDMINI_ANIM_9, //9
    MDMINI_ANIM_10, //10
    MDMINI_ANIM_MODE_NAME, //11
    MDMINI_ANIM_MAX //12
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
    MDSPR_PLAYERNO_1P, //13
    MDSPR_PLAYERNO_2P, //14
    MDSPR_PLAYERNO_3P, //15
    MDSPR_PLAYERNO_4P, //16
    MDSPR_COMDIF_1P, //17
    MDSPR_COMDIF_2P, //18
    MDSPR_COMDIF_3P, //19
    MDSPR_COMDIF_4P, //20
    MDSPR_MODE_NAME, //21
    MDSPR_MAX //22
};

enum {
    MDSPR_GRP_WIN, //0
    MDSPR_GRP_CURSOR_LR, //1
    MDSPR_GRP_CURSOR_UDLR, //2
    MDSPR_GRP_PLAYERNO, //3
    MDSPR_GRP_COMDIF, //4
    MDSPR_GRP_NAME, //5
    MDSPR_GRP_MAX //6
};

enum {
    MDMODEL_STAR, //0
    MDMODEL_STAGE, //1
    MDMODEL_STAGE_CHR, //2
    MDMODEL_CHR_00, //3
    MDMODEL_CHR_01, //4
    MDMODEL_CHR_02, //5
    MDMODEL_CHR_03, //6
    MDMODEL_CHR_04, //7
    MDMODEL_CHR_05, //8
    MDMODEL_CHR_06, //9
    MDMODEL_CHR_07, //10
    MDMODEL_CHR_08, //11
    MDMODEL_CHR_09, //12
    MDMODEL_CAP_MAN_1P, //13
    MDMODEL_CAP_MAN_2P, //14
    MDMODEL_CAP_MAN_3P, //15
    MDMODEL_CAP_MAN_4P, //16
    MDMODEL_CAP_COM_1P, //17
    MDMODEL_CAP_COM_2P, //18
    MDMODEL_CAP_COM_3P, //19
    MDMODEL_CAP_COM_4P, //20
    MDMODEL_CAP_CHR_00, //21
    MDMODEL_CAP_CHR_01, //22
    MDMODEL_CAP_CHR_02, //23
    MDMODEL_CAP_CHR_03, //24
    MDMODEL_CAP_CHR_04, //25
    MDMODEL_CAP_CHR_05, //26
    MDMODEL_CAP_CHR_06, //27
    MDMODEL_CAP_CHR_07, //28
    MDMODEL_CAP_CHR_08, //29
    MDMODEL_CAP_CHR_09, //30
    MDMODEL_CAP_MODE_00, //31
    MDMODEL_CAP_MODE_01, //32
    MDMODEL_CAP_MODE_02, //33
    MDMODEL_CAP_MODE_03, //34
    MDMODEL_CAP_MODE_04, //35
    MDMODEL_CAP_MODE_05, //36
    MDMODEL_CAP_WIN, //37
    MDMODEL_CAP_GENRE, //38
    MDMODEL_FINGER_1P, //39
    MDMODEL_FINGER_2P, //40
    MDMODEL_FINGER_3P, //41
    MDMODEL_FINGER_4P, //42
    MDMODEL_RING_1P, //43
    MDMODEL_RING_2P, //44
    MDMODEL_RING_3P, //45
    MDMODEL_RING_4P, //46
    MDMODEL_VS, //47
    MDMODEL_FONTBGFILTER, //48
    MDMODEL_MAX //49
};

enum {
    MDMODEL_ANIM_FINGER_1P, //0
    MDMODEL_ANIM_FINGER_2P, //1
    MDMODEL_ANIM_FINGER_3P, //2
    MDMODEL_ANIM_FINGER_4P, //3
    MDMODEL_ANIM_CAP_TURN, //4
    MDMODEL_ANIM_CAP_BONUS, //5
    MDMODEL_ANIM_MAX //6
};

enum {
    MDMODEL_MOT_STAR_IDLE, //0
    MDMODEL_MOT_STAR_TALK, //1
    MDMODEL_MOT_STAR_TALK_FAST, //2
    
    MDMODEL_MOT_MARIO_IDLE,//3
    MDMODEL_MOT_MARIO_WIN, //4
    
    MDMODEL_MOT_LUIGI_IDLE, //5
    MDMODEL_MOT_LUIGI_WIN, //6
    
    MDMODEL_MOT_PEACH_IDLE, //7
    MDMODEL_MOT_PEACH_WIN, //8
    
    MDMODEL_MOT_YOSHI_IDLE, //9
    MDMODEL_MOT_YOSHI_WIN, //10
    
    MDMODEL_MOT_WARIO_IDLE, //11
    MDMODEL_MOT_WARIO_WIN, //12
    
    MDMODEL_MOT_DAISY_IDLE, //13
    MDMODEL_MOT_DAISY_WIN, //14
    
    MDMODEL_MOT_WALUIGI_IDLE, //15
    MDMODEL_MOT_WALUIGI_WIN, //16
    
    MDMODEL_MOT_KINOPIO_IDLE, //17
    MDMODEL_MOT_KINOPIO_WIN, //18
    
    MDMODEL_MOT_TERESA_IDLE, //19
    MDMODEL_MOT_TERESA_WIN, //20
    
    MDMODEL_MOT_MINIKOOPA_IDLE, //21
    MDMODEL_MOT_MINIKOOPA_WIN, //22
    
    MDMODEL_MOT_MAX //23
};

enum {
    MDSPR_GRP_WIN_START = MDSPR_WIN_FRAME_TOP,
    MDSPR_GRP_WIN_END = MDSPR_WIN_BG,
    MDSPR_GRP_CURSOR_LR_START = MDSPR_CURSOR_LR_R,
    MDSPR_GRP_CURSOR_LR_END = MDSPR_CURSOR_LR_L,
    MDSPR_GRP_CURSOR_UDLR_START = MDSPR_CURSOR_UDLR_R,
    MDSPR_GRP_CURSOR_UDLR_END = MDSPR_CURSOR_GLOW_UDLR_D,
    MDSPR_GRP_PLAYERNO_START = MDSPR_PLAYERNO_1P,
    MDSPR_GRP_PLAYERNO_END = MDSPR_PLAYERNO_4P,
    MDSPR_GRP_COMDIF_START = MDSPR_COMDIF_1P,
    MDSPR_GRP_COMDIF_END = MDSPR_COMDIF_4P,
    MDSPR_GRP_NAME_START = MDSPR_MODE_NAME,
    MDSPR_GRP_NAME_END = MDSPR_MODE_NAME
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
    int selCamera; //0x1A0
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
    HUSPRID sprId[MDSPR_MAX]; //0x30
    HUSPRGRPID grpId[MDSPR_GRP_MAX]; //0x5C
} MDSPRITE_WORK;

typedef struct MdModelWork_s {
    HU3DMODELID mdlId[MDMODEL_MAX]; //0x00
    HU3DMOTID motId[23]; //0x62
    HU3DANIMID animId[6]; //0x90
    HuVecF *posP[MDMODEL_MAX]; //0x9C
    HuVecF *rotP[MDMODEL_MAX]; //0x160
    HuVecF *scaleP[MDMODEL_MAX]; //0x224
} MDMODEL_WORK;

typedef struct MdSprInfo_s {
    s16 animNo; //0x00
    s16 pri; //0x02
    HuVec2f pos; //0x04
    HuVec2f scale; //0x0C
    float tpLvl; //0x14
} MDSPR_INFO;

typedef struct MdSprGrpInfo_s {
    s16 start; //0x00
    s16 end; //0x02
    float posX; //0x04
    float posY; //0x08
} MDSPR_GRP_INFO;

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

typedef struct MdModelDebugInfo_s {
    char *name; //0x00
    int cameraNo; //0x04
} MDMODEL_DEBUG_INFO;

typedef struct MdTexBufUnit_s {
    void *addr; //0x00
    GXTexObj texObj; //0x04
    s16 posX; //0x24
    s16 posY; //0x26
    u16 w; //0x28
    u16 h; //0x2A
    GXTexFmt texFmt; //0x2C
} MDTEXBUF_UNIT;

typedef struct MdTexBufWork_s {
    MDTEXBUF_UNIT unit[MDMINI_TEXBUF_MAX]; //0x00
    int unk1E0;
} MDTEXBUF_WORK;

typedef struct MdRing_s {
    BOOL disp; //0x00
    int mode; //0x04
    GXColor color; //0x08
    GXColor endColor; //0x0C
    int unk10;
    float time; //0x14
    float ofsTime; //0x18
    float t; //0x1C
    float ofsT; //0x20
    int mdlNo; //0x24
    HuVecF **posP; //0x28
    HuVecF **rotP; //0x2C
    HuVecF **scaleP; //0x30
    int baseMdlNo; //0x34
    HuVecF baseOfs; //0x38
} MDRING;

typedef struct MdRingWork_s {
    MDRING ring[4]; //0x00
    float scale; //0x110
} MDRING_WORK;

typedef struct MdMiniStar_s {
    s16 unk0;
    s16 mode; //0x02
    s16 motNext; //0x04
} MDMINI_STAR;

typedef struct MdMiniPlayerSpr_s {
    int mode; //0x00
    float time; //0x04
    float ofsTime; //0x08
    float t; //0x0C
    float ofsT; //0x10
} MDMINI_PLAYERSPR;

typedef struct MdMiniChar_s {
    int mode; //0x00
    float time; //0x04
    float ofsTime; //0x08
    float t; //0x0C
    float ofsT; //0x10
    HuVecF origPos; //0x14
    HuVecF pos; //0x20
    HuVecF origRot; //0x2C
    HuVecF rot; //0x38
} MDMINI_CHR;

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
    s8 playerNum; //0x1A
    s8 prevPlayerNum; //0x1B
    s8 maxPlayerNum; //0x1C
    s8 padConnect[GW_PLAYER_MAX]; //0x1D
    s8 padEnable[GW_PLAYER_MAX]; //0x21
    s8 padNoConnect[GW_PLAYER_MAX]; //0x25
    s8 padNoEnable[GW_PLAYER_MAX]; //0x29
    s8 chrSel[GW_PLAYER_MAX]; //0x2D
    s8 chrSelEnd[GW_PLAYER_MAX]; //0x31
    s8 comDif[GW_PLAYER_MAX]; //0x35
    s8 comPlayerNo[3]; //0x39
    s8 selComNo; //0x3C
    s8 selNoNow; //0x3D
    s8 selNoPrev; //0x3E
    s8 mgTypeF[6]; //0x3F
    s8 scoreNum; //0x45
    s8 gameType; //0x46
    s8 teamOrderNo; //0x47
    s8 teamPlayer[2][2]; //0x48
    s8 teamOrder[GW_PLAYER_MAX]; //0x4C
    s8 teamOrderBackup[GW_PLAYER_MAX]; //0x50
    s8 teamNo[2]; //0x54
    s8 teamComPlayerNo[3]; //0x56
    s8 subMode2; //0x59
    s8 mdlE[GW_PLAYER_MAX]; //0x5A
    s8 mdlF[GW_PLAYER_MAX]; //0x5E
    s8 mdlG[3]; //0x62
    u8 pad65[3]; //0x65
    OMOBJMAN *objman; //0x68
    HUPROCESS *mainProc; //0x6C
    HUPROCESS *loadProc; //0x70
    HUPROCESS *starProc; //0x74
    HUPROCESS *playerSprProc; //0x78
    HUPROCESS *chrProc; //0x7C
    HUPROCESS *ringProc; //0x80
    HUPROCESS *mainWinProc; //0x84
    HUPROCESS *textWinProc; //0x88
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
    MDMINI_PLAYERSPR playerSpr[GW_PLAYER_MAX]; //0xCC
    MDMINI_CHR chr[MDMINI_MODEL_MAX]; //0x11C
    MDMESS *mess; //0x3C4
    MDRING_WORK *ring; //0x3C8
    BOOL dirtyF; //0x3CC
    BOOL teamInitF; //0x3D0
    BOOL winInitF; //0x3D4
    BOOL cursorInitF; //0x3D8
    BOOL nameInitF; //0x3DC
    BOOL unk3E0;
    int cursorDir; //0x3E4
    int cursorMode; //0x3E8
    float procTime; //0x3EC
    float procOfsTime; //0x3F0
    float cameraTime; //0x3F4
    float cameraTotalTime; //0x3F8
    float sprTime; //0x3FC
    float unk400;
    float nameTime; //0x404
    float procT; //0x408
    float procOfsT; //0x40C
    float cameraT; //0x410
    float cameraTotalT; //0x414
    float sprT; //0x418
    float unk41C;
    float nameT; //0x420
    float curveTime[8]; //0x424
    float curveSlope[25]; //0x444
} SYS;

enum {
    MDMINI_PROC_RESET, //0
    MDMINI_PROC_CAMERA, //1
    MDMINI_PROC_CURSOR, //2
    MDMINI_PROC_WIN, //3
    MDMINI_PROC_CAP_PLAYER, //4
    MDMINI_PROC_CHR, //5
    MDMINI_PROC_6, //6
    MDMINI_PROC_CAP_TURN, //7
    MDMINI_PROC_CAP_BONUS, //8
    MDMINI_PROC_9, //9
    MDMINI_PROC_10, //10
    MDMINI_PROC_11, //11
    MDMINI_PROC_FINGER, //12
    MDMINI_PROC_13, //13
    MDMINI_PROC_NAME, //14
    MDMINI_PROC_MAX //15
};

extern SYS *sys;
extern SYS *sysP;
extern SYS sysWork;
extern MD_FUNC MdFlowTbl[MDMINI_FLOW_MAX];
extern MD_PROC MdProcTbl[MDMINI_PROC_MAX];

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

Mtx *MdMathMatrixDir(HuVecF *start, HuVecF *end);
void MdMathMatrixRot(Mtx *matrix, HuVecF *rot);

void MdMathClamp(float *val, float min, float max);
void MdMathAngleClamp(float *angle);
void MdMathOscillate(int type, float *in, float *out, float t);
void MdMathStdCurveGet(int type, float *in, float *out, float t);
void MdMathLerp(float *out, float a, float b, float t);
void MdMathLerpOfs(float *out, float a, float b, float t, float ofs);
void MdMathLerpColor(u8 *out, float a, float b, float t);
void MdMathLerpScale(HuVecF *out, float a, float b, float t);
void MdMathLerpScaleOfs(HuVecF *out, float a, float b, float t, float ofs);
void MdMathCurveInit(int sampleNum, float *sampleT, float *sample, float *sampleSlope);
void MdMathCurveGet(float *out, int sampleNum, float *sampleT, float *sample, float *sampleSlope, float t);
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

void MdSpriteCreate(MDSPRITE_WORK *sprite);
void MdSpriteDispSet(MDSPRITE_WORK *sprite, int grpNo, BOOL dispF);
HuVec2f *MdSpriteGrpPosGet(MDSPRITE_WORK *sprite, int grpNo);
HuVec2f *MdSpriteGrpScaleGet(MDSPRITE_WORK *sprite, int grpNo);
HuVec2f *MdSpritePosGet(MDSPRITE_WORK *sprite, int grpNo, int memberNo);
HuVec2f *MdSpriteScaleGet(MDSPRITE_WORK *sprite, int grpNo, int memberNo);
float *MdSpriteAlphaGet(MDSPRITE_WORK *sprite, int grpNo, int memberNo);

void MdModelCreate(MDMODEL_WORK *model, MDCAMERA_WORK *camera);
void MdToueiLayerHook(s16 layerNo);
void MdToueiMdlInit(void);
void MdToueiTexBufInit(void);
void MdTexBufObjPosSet(MDTEXBUF_UNIT *unit, HU3DMODELID modelId, char *objName, u16 cameraBit, HuVecF *ofsP);
void MdTexBufPosSet(MDTEXBUF_UNIT *unit, HuVecF *posP, u16 cameraBit);

void MdRingCreate(void);
void MdRingMain(void);
void MdRingModeSet(int no, int mode);
void MdRingDispSet(int no, BOOL dispF);
void MdRingScaleSet(int no, float scale);
void MdRingMdlSet(int no, int mdlNo);
void MdRingOfsSet(int no, float x, float y, float z);
void MdRingColorReset(int no);

/* --- mdmini.c ------------------------------------------------------------ */

void ObjectSetup(void);
void MdMain(void);
void MdPosAdjustMode(void);

#endif
