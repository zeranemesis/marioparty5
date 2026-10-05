#ifndef _MDSTORYDLL_H
#define _MDSTORYDLL_H

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

#define MDSTORY_RING_MAX 1
#define MDSTORY_STAGE_MAX 7
#define MDSTORY_CHR_MAX 7
#define MDSTORY_PACK_MAX 5
/* Named _NUM, not _MAX: MDSTORY_PLAYERSPR_MAX / MDSTORY_MODELWORK_MAX are also
   enumerators of the enums at lines 132 and 143 below. The #define used to
   expand those enumerator names into the literals 1 and 7, which MWCC rejects
   ("identifier expected"), so nothing including this header compiled. The
   values themselves are correct: playerSpr 0xA0->0xB4 = 1 x 0x14, and
   modelWork 0xB4->mess 0x290 = 7 x 0x44. */
#define MDSTORY_PLAYERSPR_NUM 1
#define MDSTORY_MODELWORK_NUM 7
#define MDSTORY_TEXBUF_MAX 11

#define MDSTORY_OSC_MAX 7

#define MdProcCall(idx, arg) MdProcTbl[idx](arg)
#define MdProcWait(idx, arg) do { \
    while(!MdProcCall(idx, arg)) { \
        HuPrcVSleep(); \
    } \
} while(0)

enum {
    MD_LOAD_STATE_NONE, //0
    MD_LOAD_STATE_BOARD_LOAD, //1
    MD_LOAD_STATE_STORYMODE_LOAD, //2
    MD_LOAD_STATE_CHAR_WAIT, //3
    MD_LOAD_STATE_CHAR_LOAD, //4
    MD_LOAD_STATE_DONE //5
};

enum {
    MDSTORY_FLOW_INIT, //0
    MDSTORY_FLOW_START, //1
    MDSTORY_FLOW_CHR_SEL, //2
    MDSTORY_FLOW_STAGE_SEL, //3
    MDSTORY_FLOW_STORY_SEL, //4
    MDSTORY_FLOW_MAX //5
};

enum {
    MDSTORY_PROC_RESET, //0
    MDSTORY_PROC_CAMERA_LERP, //1
    MDSTORY_PROC_STAGE_INIT, //2
    MDSTORY_PROC_STAGE_MOVE, //3
    MDSTORY_PROC_STORY_INIT, //4
    MDSTORY_PROC_STORY_MOVE, //5
    MDSTORY_PROC_CHR_POS, //6
    MDSTORY_PROC_STORY_POS, //7
    MDSTORY_PROC_FADE, //8
    MDSTORY_PROC_FINGER, //9
    MDSTORY_PROC_MAX //10
};

enum {
    MDCAMERA_BG, //0
    MDCAMERA_MAIN, //1
    MDCAMERA_FG, //2
    MDCAMERA_MAX //3
};

enum {
    MDMESS_WIN_MAIN, //0
    MDMESS_WIN_SUB, //1
    MDMESS_WIN_MAX //2
};

enum {
    MD_DEBUGFONT_CTRL_NOP, //0
    MD_DEBUGFONT_CTRL_CAMERA, //1
    MD_DEBUGFONT_CTRL_LIGHT, //2
    MD_DEBUGFONT_CTRL_MODEL, //3
    MD_DEBUGFONT_CTRL_MAX //4
};

enum {
    MDSTORY_OBJ_DEBUGFONT, // 0
    MDSTORY_OBJ_PAD, //1
    MDSTORY_OBJ_CAMERA, //2
    MDSTORY_OBJ_LIGHT, //3
    MDSTORY_OBJ_MAX //4
};

enum {
    MDSPR_GRP_FINGER = 38,
    MDSPR_GRP_RING = 39,
    MDSPR_GRP_MAX = 40
};

enum {
    MDSTORY_STAR_NONE, //0
    MDSTORY_STAR_MOVE_START, //1
    MDSTORY_STAR_MOVE, //2
    MDSTORY_STAR_MOVE_END, //3
    MDSTORY_STAR_TALK, //4
    MDSTORY_STAR_TALK_END, //5
    MDSTORY_STAR_BLINK_START, //6
    MDSTORY_STAR_MAX //7
};

enum {
    MDSTORY_PLAYERSPR_NONE, //0
    MDSTORY_PLAYERSPR_START, //1
    MDSTORY_PLAYERSPR_END, //2
    MDSTORY_PLAYERSPR_MOVE, //3
    MDSTORY_PLAYERSPR_MOVE_END, //4
    MDSTORY_PLAYERSPR_TALK, //5
    MDSTORY_PLAYERSPR_TALK_END, //6
    MDSTORY_PLAYERSPR_MAX //7
};

enum {
    MDSTORY_MODELWORK_NONE, //0
    MDSTORY_MODELWORK_INIT, //1
    MDSTORY_MODELWORK_MOVE, //2
    MDSTORY_MODELWORK_MOVE_END, //3
    MDSTORY_MODELWORK_END, //4
    MDSTORY_MODELWORK_MAX //5
};

enum {
    MDSTORY_AUD_ALL_STOP, //0
    MDSTORY_AUD_BGM_PLAY, //1
    MDSTORY_AUD_BGM_STOP, //2
    MDSTORY_AUD_FX, //3
    MDSTORY_AUD_FINGER, //4
    MDSTORY_AUD_FINGER_STOP, //5
    MDSTORY_AUD_FX_CMN, //6
    MDSTORY_AUD_STAGE_START, //7
    MDSTORY_AUD_STAGE_MOVE, //8
    MDSTORY_AUD_STAGE_MOVE_STOP, //9
    MDSTORY_AUD_STAGE_SEL1, //10
    MDSTORY_AUD_STAGE_SEL2, //11
    MDSTORY_AUD_STAGE_SEL3, //12
    MDSTORY_AUD_STAGE_SEL4, //13
    MDSTORY_AUD_STAGE_SEL5, //14
    MDSTORY_AUD_STAGE_SEL6, //15
    MDSTORY_AUD_STAGE_SEL7, //16
    MDSTORY_AUD_MAX //18
};

enum {
    MDMODEL_STAR, //0
    MDMODEL_STAGE, //1
    MDMODEL_STAGE_CHR, //2

    MDMODEL_MARIO, //3
    MDMODEL_LUIGI, //4
    MDMODEL_PEACH, //5
    MDMODEL_YOSHI, //6
    MDMODEL_WARIO, //7
    MDMODEL_DAISY, //8
    MDMODEL_WALUIGI, //9

    MDMODEL_CAP_CHR_00, //10
    MDMODEL_CAP_CHR_01, //11
    MDMODEL_CAP_CHR_02, //12
    MDMODEL_CAP_CHR_03, //13
    MDMODEL_CAP_CHR_04, //14
    MDMODEL_CAP_CHR_05, //15
    MDMODEL_CAP_CHR_06, //16

    MDMODEL_CAP_PACK0, //17
    MDMODEL_CAP_PACK1, //18
    MDMODEL_CAP_PACK2, //19
    MDMODEL_CAP_PACK3, //20
    MDMODEL_CAP_PACK4, //21

    MDMODEL_CAP_LV, //22

    MDMODEL_FINGER_1P, //23
    MDMODEL_RING_1P, //24
    MDMODEL_FONTBGFILTER, //25

    MDMODEL_MAX //26
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
    HUSPRGRPID grpId[MDSPR_GRP_MAX]; //0x00
} MDSPRITE_WORK;

typedef struct MdModelDebugInfo_s {
    char *name; //0x00
    int cameraNo; //0x04
} MDMODEL_DEBUG_INFO;

typedef struct MdModelWork_s {
    HU3DMODELID mdlId[MDMODEL_MAX]; //0x00
    HU3DANIMID animId[MDMODEL_MAX]; //0x34
    HuVecF *posP[MDMODEL_MAX]; //0x68
    HuVecF *rotP[MDMODEL_MAX]; //0xD0
    HuVecF *scaleP[MDMODEL_MAX]; //0x138
} MDMODEL_WORK;

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
    MDTEXBUF_UNIT unit[MDSTORY_TEXBUF_MAX]; //0x00
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
    MDRING ring[MDSTORY_RING_MAX]; //0x00
    float scale; //0x44
} MDRING_WORK;

typedef struct MdStoryStar_s {
    s16 unk0; //0x00
    s16 mode; //0x02
    s16 motNext; //0x04
} MDSTORY_STAR;

typedef struct MdStoryPlayersPr_s {
    int mode; //0x00
    float time1; //0x04
    float time2; //0x08
    float t1; //0x0C
    float t2; //0x10
} MDSTORY_PLAYERSPR;

typedef struct MdStoryModelWork_s {
    int mode; //0x00
    float time; //0x04
    float ofsTime; //0x08
    float t; //0x0C
    float ofsT; //0x10
    int unk14;
    float unk18;
    float unk1C;
    float unk20;
    float unk24;
    float unk28;
    float unk2C;
    float unk30;
    float unk34;
    float unk38;
    float unk3C;
    float unk40;
} MDSTORY_MODEL_WORK;

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

typedef struct MdDotWork_s {
    s32 man[3]; /* 4-byte slots holding sign-extended s16 ids, so 0xC bytes, not
                   0x6: HuMemDirectMallocNum(0, 0xc, 0x1000) with extsh'd values
                   stored by stw at 0/4/8. Matches mdbeachDll.h. */
} MDDOT_WORK;

typedef struct Sys_s {
    int subMode; //0x00
    int unk4; //0x04
    int flowTime; //0x08
    int flowMode; //0x0C
    int ovlCallMode; //0x10
    OMOVL nextDll; //0x14
    s8 exitF; //0x18
    s8 skipWipeF; //0x19
    s8 padConnectNum; //0x1A
    s8 chrSel[4]; //0x1B
    s8 chrSelNo[4]; //0x1F
    s8 chrSelEnd[4]; //0x23
    s8 selNo[4]; //0x27
    s8 padNo[4]; //0x2B
    s8 selPackNo; //0x2F
    s8 selStoryNo; //0x30
    s8 unk31[3]; //0x31
    OMOBJMAN *objman; //0x34
    HUPROCESS *mainProc; //0x38
    HUPROCESS *loadProc; //0x3C
    HUPROCESS *toueiProc; //0x40
    HUPROCESS *stageProc; //0x44
    HUPROCESS *chrProc; //0x48
    HUPROCESS *ringProc; //0x4C
    HUPROCESS *mainWinProc; //0x50
    HUPROCESS *subWinProc; //0x54
    OMOBJ *obj[MDSTORY_OBJ_MAX]; //0x58
    OMOVLHIS *ovlHis; //0x68
    s32 evtNo; //0x6C
    s16 loadState; //0x70
    s16 loadAsyncStat; //0x72
    int loadDir; //0x74
    s16 loadProcReady; //0x78
    MDDEBUGFONT_WORK *debugFont; //0x7C
    MDPAD_WORK *pad; //0x80
    MDCAMERA_WORK *camera; //0x84
    MDLIGHT_WORK *light; //0x88
    MDSPRITE_WORK *sprite; //0x8C
    MDMODEL_WORK *model; //0x90
    MDTEXBUF_WORK *texbuf; //0x94
    MDSTORY_STAR *star; //0x98
    MDRING_WORK *ring; //0x9C
    MDSTORY_PLAYERSPR playerSpr[MDSTORY_PLAYERSPR_NUM]; //0xA0
    MDSTORY_MODEL_WORK modelWork[MDSTORY_MODELWORK_NUM]; //0xB4
    MDMESS *mess; //0x290
    int procInitF[6]; //0x294
    int chrPosInitF; //0x2AC
    int unk2B0; //0x2B0
    float oscTime[MDSTORY_OSC_MAX]; //0x2B4
    float oscT[MDSTORY_OSC_MAX]; //0x2D0
    float curveTime[MDMATH_CURVE_SAMPLE_MAX]; //0x2EC
    float curveSlope[25]; //0x30C
} SYS;

extern SYS *sys;
extern SYS *sysP;
extern SYS sysWork;
extern MD_FUNC MdFlowTbl[MDSTORY_FLOW_MAX];
extern MD_PROC MdProcTbl[MDSTORY_PROC_MAX];

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

void MdMathMatrixDir(Mtx *matrix, HuVecF *dir);
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

void MdSpriteCreate(MDSPRITE_WORK *sprite);
void MdSpriteDispSet(MDSPRITE_WORK *sprite, int grpNo, BOOL dispF);
HuVec2f *MdSpritePosGet(MDSPRITE_WORK *sprite, int grpNo, int memberNo);
HuVec2f *MdSpriteScaleGet(MDSPRITE_WORK *sprite, int grpNo, int memberNo);
float *MdSpriteAlphaGet(MDSPRITE_WORK *sprite, int grpNo, int memberNo);
HuVec2f *MdSpriteGrpPosGet(MDSPRITE_WORK *sprite, int grpNo);
HuVec2f *MdSpriteGrpScaleGet(MDSPRITE_WORK *sprite, int grpNo);

void MdModelCreate(MDMODEL_WORK *model, MDCAMERA_WORK *camera);
void MdTexBufInit(void);
void MdTexBufObjPosSet(MDTEXBUF_UNIT *unit, HU3DMODELID modelId, char *objName, u16 cameraBit, HuVecF *ofsP);
void MdTexBufPosSet(MDTEXBUF_UNIT *unit, HuVecF *posP, u16 cameraBit);
void MdToueiMdlInit(void);
void MdToueiTexBufInit(void);

void MdRingCreate(void);
void MdRingMain(void);
void MdRingDispSet(int no, BOOL dispF);
void MdRingBaseOfsSet(int no, float x, float y, float z);
void MdRingModeSet(int no, int mode);
void MdRingBaseMdlSet(int no, int mdlNo);
void MdRingOfsSet(int no, float x, float y, float z);
void MdRingMdlSet(int no, int mdlNo);
void MdRingScaleSet(int no, float scale);
void MdRingColorReset(int no);

void MdPosAdjustMode(void);

#endif