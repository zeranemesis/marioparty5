#ifndef _MDOPTION_LOCAL_H
#define _MDOPTION_LOCAL_H

/* Local header shared by the four mdoptionDll sources (the original
 * overlay shipped none). The generic Md* library part (debug font,
 * camera, light, message windows) is the same code as in mdpartyDll;
 * SYS is extended as functions are recovered. Add fields ONLY by
 * keeping every offset pinned (use explicit pad arrays). */
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

#define MDLIGHT_MAX 1

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
} MDMESS_WORK;

/* Offsets recovered from the original object's sys-> accesses. */
typedef struct Sys_s {
    s32 pad00[10];             //0x00
    s32 flowMode;              //0x28 MdFlowTbl index
    s8 exitF;                  //0x2C
    s8 skipWipeF;              //0x2D
    s8 pad2E;                  //0x2E
    s8 cursorSelNoPrev;        //0x2F
    s8 pad30;                  //0x30
    s8 cursorSelNo;            //0x31 write cursor into cursorSel[]
    s8 cursorSelF[4];          //0x32
    s8 cursorSel[4];           //0x36
    s8 cursorSelPrev[4];       //0x3A
    s8 pad3E[0x4C - 0x3E];     //0x3E
    s16 loadState;             //0x4C
    s16 loadAsyncStat;         //0x4E
    s32 pad50;                 //0x50
    OMOBJMAN *objman;          //0x54
    HUPROCESS *mainProc;       //0x58
    void *unk5C[3];            //0x5C
    HUPROCESS *loadProc;       //0x68
    OMOBJ *obj[6];             //0x6C debugfont, pad, camera, light, shadow, mess
    void *debugFont;           //0x84
    void *padWork;             //0x88
    void *camera;              //0x8C
    void *light;               //0x90
    void *shadow;              //0x94
    void *mess;                //0x98
    void *sprite;              //0x9C
    void *model;               //0xA0
    u8 padA4[0x398 - 0xA4];    //0xA4
    s32 unk398;                //0x398
    s32 unk39C;                //0x39C
    u8 pad3A0[0x510 - 0x3A0];  //0x3A0
} SYS;

extern SYS *sys;
extern SYS *sysP;

#endif
