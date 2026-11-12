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

typedef struct Sys_s {
    u8 pad00[0x1C];
    s8 exitF; //0x1C
    s8 skipWipeF; //0x1D
    u8 pad1E[0x70 - 0x1E];
    MDMODEL_WORK *model; //0x70
    void *score; //0x74
    u8 pad78[0x1E0 - 0x78];
} SYS;

extern SYS *sys;
extern SYS *sysP;

#endif
