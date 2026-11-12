#ifndef _RESCARD_LOCAL_H
#define _RESCARD_LOCAL_H

/* Local header shared by the three rescardDll sources (the original overlay
 * shipped none). The generic Md* library part (debug font, camera, light,
 * message windows) is the same code as in mdoptionDll/mdpartyDll, trimmed
 * to what this overlay uses. Offsets are pinned from the original's code. */
#include "game/object.h"
#include "game/printfunc.h"
#include "game/hu3d.h"
#include "game/sprite.h"
#include "game/window.h"
#include "game/pad.h"
#include "game/disp.h"
#include "game/process.h"
#include "game/data.h"
#include "game/wipe.h"
#include "game/mg/timer.h"

#define MDMATH_OSC_0 0
#define MDMATH_OSC_90 1
#define MDMATH_OSC_90_REV 2
#define MDMATH_OSC_180 3
#define MDMATH_OSC_360 4
#define MDMATH_OSC_540 5
#define MDMATH_OSC_720 6

#define MDMATH_CURVE_SAMPLE_MAX 8

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
    MDCAMERA_BG, //0
    MDCAMERA_MAIN, //1
    MDCAMERA_FG, //2
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
    MDMESS_WIN_TEAM1, //2
    MDMESS_WIN_TEAM2, //3
    MDMESS_WIN_MAX //4
};

#define MDLIGHT_MAX 2

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
    MDCAMERA camera[2]; //0x08 (this overlay only has two cameras: size 0x11C)
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


#define MDMESS_INSERT_MAX 16
#define MDMESS_WIN_NUM 2

typedef struct MdMessWin_s {
    HUWINID winId; //0x00
    float unk4; //0x04
    float unk8; //0x08
    float unkC; //0x0C
} MDMESS_WIN;

typedef struct MdMessInsert_s {
    int num; //0x00
    u32 mesNo[MDMESS_INSERT_MAX]; //0x04
    s16 insertNo[MDMESS_INSERT_MAX]; //0x44
} MDMESS_INSERT;

typedef struct MdMess_s {
    BOOL createF; //0x00
    u32 mode; //0x04
    HUWINID dlgWinId; //0x08
    s16 pad0A; //0x0A
    s16 dlgType; //0x0C
    s16 dlgChoice; //0x0E
    u32 dlgMesNo; //0x10
    MDMESS_INSERT insert; //0x14
    MDMESS_WIN win[MDMESS_WIN_NUM]; //0x78
} MDMESS_WORK;

#define MDSPR_ANIM_MAX 5
#define MDSPR_MAX 18
#define MDSPR_GRP_MAX 7

typedef struct MdSpriteWork_s {
    ANIMDATA *animP[MDSPR_ANIM_MAX]; //0x00
    HUSPRID sprId[MDSPR_MAX]; //0x14
    HUSPRGRPID grpId[MDSPR_GRP_MAX]; //0x38
    s16 pad46; //0x46
    float fadeAlpha; //0x48
} MDSPRITE_WORK;

#define MDMODEL_MAX 89
#define MDMODEL_MOT_MAX 14
#define MDMODEL_AMB_MAX 16

typedef struct MdModelWork_s {
    HU3DMODELID mdlId[MDMODEL_MAX]; //0x00
    HU3DMOTID motId[MDMODEL_MOT_MAX]; //0xB2
    s16 padCE; //0xCE
    HuVecF *posP[MDMODEL_MAX]; //0xD0
    HuVecF *rotP[MDMODEL_MAX]; //0x234
    HuVecF *scaleP[MDMODEL_MAX]; //0x398
    float ambScale; //0x4FC
    HU3DMODELID ambId[MDMODEL_AMB_MAX]; //0x500
} MDMODEL_WORK;

/* model slots (sys->model->mdlId[]), names from the original's mdlInfo table */
enum {
    MDMODEL_STAR = 0,
    MDMODEL_STAGE = 1,          //1..6 STAGE_BASE, STAGE_SIDE, STAGE_OBJ1-4
    MDMODEL_PIN = 7,            //7..10
    MDMODEL_CUR = 11,           //11..14
    MDMODEL_PEDESTAL = 15,      //15..18
    MDMODEL_POP_STAR = 19,
    MDMODEL_POP_ZTAR = 20,
    MDMODEL_WIN_STAR = 21,      //21..42
    MDMODEL_WIN_ZTAR = 43,      //43..53
    MDMODEL_CARD_STAR = 54,     //54..57
    MDMODEL_CARD_ZTAR = 58,     //58..61
    MDMODEL_CARD_SZTAR = 62,    //62..65
    MDMODEL_DRAW = 66,          //66 DRAW_STAR, 67..73 DRAW_ZTAR_0-6
    MDMODEL_EFF_STARFLASH = 74,
    MDMODEL_EFF_DICEFLASH = 75, //75..78
    MDMODEL_EFF_CARDFLASH = 79, //79..82
    MDMODEL_RING = 83,
    MDMODEL_SPOT = 84,          //84..87
    MDMODEL_FONTBGFILTER = 88
};

typedef struct RescardPlayer_s {
    s8 charNo; //0x00
    s8 padNo; //0x01 -1 for a COM player
    s8 starNum; //0x02
    s8 ztarNum; //0x03
    s8 type; //0x04
    s8 pedestalF; //0x05
    u8 pad06[2];
    s32 state; //0x08
    float oscT; //0x0C
    float oscOut; //0x10
    s32 unk14; //0x14
} RESCARD_PLAYER;

typedef struct RescardCard_s {
    s8 kind; //0x00 0 = zero card, 1 = star card
    s8 state; //0x01
    s8 id; //0x02 model slot (MDMODEL_DRAW + n)
    u8 pad03;
    float oscT; //0x04
    float oscOut; //0x08
} RESCARD_CARD;

typedef struct RescardDraw_s {
    u32 phase; //0x00
    s32 wait; //0x04
    s32 timeLimit; //0x08
    s32 sel; //0x0C
    s32 cursor; //0x10
    s32 remain; //0x14
    s32 revealIdx; //0x18
    s32 revealWait; //0x1C
    RESCARD_CARD card[8]; //0x20
} RESCARD_DRAW;

typedef struct RescardScore_s {
    RESCARD_PLAYER player[4]; //0x00
    RESCARD_DRAW draw; //0x60
    s32 unkE0; //0xE0
    s32 phase; //0xE4
    s16 mesId; //0xE8
    s8 unkEA; //0xEA
    s8 rankMax; //0xEB number of players on the top rank
    s8 rankNum; //0xEC
    s8 rank[4]; //0xED
    u8 padF1[3];
} RESCARD_SCORE;

typedef struct RescardPin_s {
    s32 state; //0x00
    u8 pad04[0x50 - 4];
} RESCARD_PIN;

typedef struct RescardFocus_s {
    HuVecF center; //0x00
    HuVecF hvr; //0x0C
    HuVecF centerEnd; //0x18
    HuVecF hvrEnd; //0x24
} RESCARD_FOCUS;

typedef struct Sys_s {
    s32 unk00; //0x00
    s32 unk04; //0x04
    s32 unk08; //0x08
    s32 frame; //0x0C
    s32 flow; //0x10 MdFlowTbl index
    s32 retMode; //0x14
    OMOVL ovl; //0x18
    s8 exitF; //0x1C
    s8 skipWipeF; //0x1D
    u8 pad1E[2];
    OMOBJMAN *objman; //0x20
    HUPROCESS *mainProc; //0x24
    HUPROCESS *starProc; //0x28
    HUPROCESS *pinProc; //0x2C
    HUPROCESS *bgmProc; //0x30
    HUPROCESS *voiceProc; //0x34
    HUPROCESS *loadProc; //0x38
    OMOBJ *obj[5]; //0x3C debugfont, pad, camera, light, mess
    s16 loadState; //0x50
    s16 loadAsyncStat; //0x52
    s32 dataDir; //0x54
    MDDEBUGFONT_WORK *debugFont; //0x58
    MDPAD_WORK *padWork; //0x5C
    MDCAMERA_WORK *camera; //0x60
    MDLIGHT_WORK *light; //0x64
    MDMESS_WORK *mess; //0x68
    MDSPRITE_WORK *sprite; //0x6C
    MDMODEL_WORK *model; //0x70
    RESCARD_SCORE *score; //0x74
    RESCARD_PIN pin[4]; //0x78
    s32 procState; //0x1B8
    s32 procState2; //0x1BC
    u8 pad1C0[8];
    float procT[2]; //0x1C8
    float procOut[2]; //0x1D0
    MGTIMER *timer; //0x1D8
    u8 pad1DC[4];
} SYS;

extern SYS *sys;
extern SYS *sysP;

#endif
