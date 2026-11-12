#ifndef _MDCARD_LOCAL_H
#define _MDCARD_LOCAL_H

/* Local header shared by the mdcardDll sources (the original
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
    MD_DEBUGFONT_CTRL_SHADOW, //3
    MD_DEBUGFONT_CTRL_MODEL, //4
    MD_DEBUGFONT_CTRL_MAX //5
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
typedef struct MdShadow_s {
    int bit; //0x00
    float tpLvl; //0x04
    HuVecF hvr; //0x08
    HuVecF center; //0x14
    HuVecF pos; //0x20
    HuVecF target; //0x2C
    HuVecF cross; //0x38
    HuVecF up; //0x44
    HuVecF dir; //0x50
} MDSHADOW;

typedef struct MdShadowWork_s {
    BOOL createF; //0x00
    int shadowSel; //0x04
    MDDEBUGFONT_WORK *debugFont; //0x08
    MDSHADOW shadow[1]; //0x0C
} MDSHADOW_WORK;

#define MDMESS_INSERT_MAX 16
#define MDMESS_WIN_NUM 3

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

typedef struct MdMessMisc_s {
    int state; //0x00
    int step; //0x04
    float t; //0x08
    float unkC; //0x0C
    float sinT; //0x10
    float unk14; //0x14
    float startPosY[2]; //0x18
    float startScaleY; //0x20
} MDMESS_MISC;

typedef struct MdMess_s {
    BOOL createF; //0x00
    u32 mode; //0x04
    HUWINID dlgWinId; //0x08
    s16 dlgType; //0x0A
    s16 dlgChoice; //0x0C
    u32 dlgMesNo; //0x10
    MDMESS_INSERT insert; //0x14
    MDMESS_WIN win[MDMESS_WIN_NUM]; //0x78
    MDMESS_MISC misc; //0xA8
} MDMESS_WORK;

/* Model slots of the card overlay (sys->model, 0x2A0 bytes on GC). */
#define MDMODEL_MAX 39

typedef struct MdModelWork_s {
    HU3DMODELID mdlId[MDMODEL_MAX]; //0x00 [0]=star [1]=cloth [2]=table [3..23]=pins [24..27]=man A [28..31]=man B [32]=? [33..37]=cards [38]=hook
    HU3DMOTID motId[44];            //0x4E
    HU3DANIMID animId[1];           //0xA6
    HuVecF *posP[MDMODEL_MAX];      //0xA8
    HuVecF *rotP[MDMODEL_MAX];      //0x144
    HuVecF *scaleP[MDMODEL_MAX];    //0x1E0
    float ambScale;                 //0x27C
    HU3DMODELID ambId[16];          //0x280
} MDMODEL_WORK;

#define MDSPR_ANIM_MAX 6
#define MDSPR_MAX 13
#define MDSPR_GRP_MAX 4

typedef struct MdSpriteWork_s {
    ANIMDATA *animP[MDSPR_ANIM_MAX]; //0x00
    HUSPRID sprId[MDSPR_MAX]; //0x18
    HUSPRGRPID grpId[MDSPR_GRP_MAX]; //0x32
    float fadeAlpha; //0x3C
} MDSPRITE_WORK;

/* Offsets recovered from the original overlay's sys-> accesses (GC layout;
 * pointer members are real pointers so offsets after 0x44 differ on PC). */
enum {
    MDCARD_OBJ_DEBUGFONT,
    MDCARD_OBJ_PAD,
    MDCARD_OBJ_CAMERA,
    MDCARD_OBJ_LIGHT,
    MDCARD_OBJ_SHADOW,
    MDCARD_OBJ_MESS,
    MDCARD_OBJ_MAX
};

typedef struct MdCardMsg_s {
    s16 mode;   //0x00
    s16 choice; //0x02
} MDCARD_MSG;

typedef struct MdCardSlot_s {
    s32 unk0; //0x00
    s32 unk4; //0x04
    u8 pad8[0x60 - 0x8];
} MDCARD_SLOT;

typedef struct MdCardCell_s {
    s32 unk0; //0x00
    u8 pad4[0x14 - 0x4];
} MDCARD_CELL;

typedef struct Sys_s {
    s32 step;                  //0x00
    s32 subStep;               //0x04
    s32 unk8;                  //0x08
    s32 counter;               //0x0C
    s32 flowMode;              //0x10
    s32 ovlMode;               //0x14 0 = goto, 1 = call, 2 = return
    s32 ovlNo;                 //0x18
    s8 exitF;                  //0x1C
    s8 skipWipeF;              //0x1D
    s8 unk1E;                  //0x1E
    s8 unk1F;                  //0x1F
    u8 pad20[0x3B - 0x20];     //0x20
    s8 unk3B[4];               //0x3B
    u8 pad3F;                  //0x3F
    s16 loadState;             //0x40
    s16 loadAsyncStat;         //0x42
    s32 loadDirNo;             //0x44
    OMOBJMAN *objman;          //0x48
    HUPROCESS *mainProc;       //0x4C
    HUPROCESS *unk50;          //0x50
    HUPROCESS *loadProc;       //0x54
    HUPROCESS *unk58;          //0x58
    HUPROCESS *unk5C;          //0x5C
    OMOBJ *obj[MDCARD_OBJ_MAX]; //0x60
    MDDEBUGFONT_WORK *debugFont; //0x78
    MDPAD_WORK *padWork;       //0x7C
    MDCAMERA_WORK *camera;     //0x80
    MDLIGHT_WORK *light;       //0x84
    MDSHADOW_WORK *shadow;     //0x88
    MDMESS_WORK *mess;         //0x8C
    MDSPRITE_WORK *sprite;     //0x90
    MDMODEL_WORK *model;       //0x94
    MDCARD_MSG msg;            //0x98
    MDCARD_SLOT unk9C[7];      //0x9C (msg lives at 0x98; slots follow at 0x9C)
    MDCARD_CELL unk33C[4];     //0x33C
    s32 unk38C;                //0x38C
    s32 unk390;                //0x390
    u8 pad394[0x3D4 - 0x394];  //0x394
} SYS;

extern SYS *sys;
extern SYS *sysP;

#endif
