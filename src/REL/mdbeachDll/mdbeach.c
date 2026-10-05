#include "game/hu3d.h"
#include "game/object.h"
#include "game/process.h"
#include "game/sprite.h"
#include "game/window.h"

/* REL/mdbeachDll.h does not exist in this checkout yet; the pieces this unit
   needs are reproduced here so the file compiles on its own. */
typedef struct MdMessWin_s {
    s16 mode;
    HUWINID winId;
    s16 choice;
    s16 unk6;
    s16 type;
    s16 unkA;
} MDMESS_WIN;

typedef struct MdMess_s {
    MDMESS_WIN win[5];
} MDMESS;

typedef struct MdDebugFontWork_s {
    BOOL createF;
    int ctrl;
    BOOL printF;
    int color;
    s16 posX;
    s16 posY;
    s16 ofsY;
    s16 rowOfsY;
    s16 rowH;
    s16 maxLen;
    float size;
} MDDEBUGFONT_WORK;

typedef struct MdPadWork_s {
    u16 DStkDown[4];
    u16 DStk[4];
} MDPAD_WORK;

typedef struct MdCamera_s {
    u16 bit;
    int focusType;
    HuVecF pos;
    HuVecF target;
    HuVecF cross;
    HuVecF up;
    HuVecF dir;
    HuVecF hvr;
    float unk50;
    HuVecF center;
    HuVecF unk60[3];
    float fovy;
} MDCAMERA;

typedef struct MdCameraWork_s {
    BOOL createF;
    MDDEBUGFONT_WORK *debugFont;
    MDCAMERA camera[2];
    int selCamera;
} MDCAMERA_WORK;

typedef struct MdLight_s {
    s16 type;
    HU3DLIGHTID gLightId;
    HU3DLLIGHTID lLightId;
    HuVecF hvr;
    HuVecF center;
    HuVecF pos;
    HuVecF target;
    HuVecF lightPos;
    HuVecF lightDir;
    GXColor rgb;
} MDLIGHT;

typedef struct MdLightWork_s {
    BOOL createF;
    int lightSel;
    MDDEBUGFONT_WORK *debugFont;
    MDLIGHT light[1];
} MDLIGHT_WORK;

typedef struct MdSpriteWork_s {
    ANIMDATA *animP[18];
    HUSPRID sprId[30];
    HUSPRGRPID grpId[9];
} MDSPRITE_WORK;

typedef struct MdModelWork_s {
    HU3DMODELID mdlId[53];
    HU3DMOTID motId[33];
    HU3DANIMID toueiAnmId[4];
    HuVecF *posP[53];
    HuVecF *rotP[53];
    HuVecF *scaleP[53];
} MDMODEL_WORK;

typedef struct MdStar_s {
    s16 unk0;
    s16 mode;
    s16 motNext;
    HuVecF pos;
} MDBEACH_STAR;

typedef struct MdBeachPosWork_s {
    s32 mode;
    f32 in[4];
    f32 ofs;
    f32 unk18;
    f32 unk1C;
} MDBEACH_POS_WORK;

typedef struct MdBeachStateWork_s {
    s32 mode;
    f32 in0;
    f32 in1;
    f32 t;
    f32 ofs;
} MDBEACH_STATE_WORK;

typedef struct Sys_s {
    int subMode;
    int unk4;
    int flowTime;
    int flowMode;
    int ovlCallMode;
    OMOVL nextDll;
    s8 exitF;
    s8 skipWipeF;
    s8 selMode;
    s8 unk1B;
    s8 unk1C;
    s8 unk1D;
    s8 unk1E[4];
    s8 padEnable[4];
    s8 chrSel[4];
    s8 chrSelEnd[4];
    s8 chrNo[4];
    s8 unk32[4];
    s8 unk36[4];
    s8 comPlayerNo[4];
    s8 unk3E[4];
    s8 teamPlayer[3];
    s8 selPlayerNo;
    s8 selMapNo;
    s8 chrSel2[2][2];
    s8 chrGrpNo[4];
    s8 unk4F[4];
    s8 unk53[2];
    s8 unk55[3];
    s8 unk58;
    s8 unk59;
    s8 unk5A;
    s8 unk5B;
    OMOBJMAN *objman;
    HUPROCESS *mainProc;
    HUPROCESS *loadProc;
    HUPROCESS *starProc;
    HUPROCESS *gameSelProc;
    HUPROCESS *gameInitProc;
    HUPROCESS *cursorProc;
    HUPROCESS *mainWinProc;
    HUPROCESS *helpWinProc;
    HUPROCESS *subWinProc;
    OMOBJ *obj[4];
    OMOVLHIS *ovlHis;
    s32 evtNo;
    s16 loadState;
    s16 loadAsyncStat;
    unsigned int loadDir;
    s16 loadProcReady;
    MDDEBUGFONT_WORK *debugFont;
    MDPAD_WORK *pad;
    MDCAMERA_WORK *camera;
    MDLIGHT_WORK *light;
    MDSPRITE_WORK *sprite;
    MDMODEL_WORK *model;
    MDBEACH_STAR *star;
    MDBEACH_POS_WORK posWork[10];
    MDBEACH_STATE_WORK chrWork[4];
    MDBEACH_STATE_WORK capWork[4];
    MDMESS *mess;
} SYS;

typedef struct MdBeachEffect_s {
    int parManId[3];
} MDBEACH_EFFECT;

extern SYS *sys;

void MdMessMain(MDMESS *mess, int winNo);

#define MDMESS_WIN_MAIN 0

void fn_1_16DE8(void)
{
}

static void ParticleGlowKill(MDBEACH_EFFECT *effectP)
{
    Hu3DParManKill(effectP->parManId[0]);
}

void fn_1_28C8(void)
{
    MDMESS *mess = sys->mess;

    while (1) {
        mess = sys->mess;
        MdMessMain(mess, MDMESS_WIN_MAIN);
        HuPrcVSleep();
    }
}
