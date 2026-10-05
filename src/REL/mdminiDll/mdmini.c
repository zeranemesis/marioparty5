#include "game/object.h"
#include "game/hu3d.h"
#include "game/pad.h"
#include "game/memory.h"
#include "game/process.h"
#include "game/printfunc.h"
#include "game/armem.h"
#include "game/data.h"
#include "game/flag.h"
#include "game/gamework.h"
#include "game/window.h"
#include "game/wipe.h"
#include "game/audio.h"
#include "game/saveload.h"

/* frandmod is deliberately left undeclared (no game/frand.h): the original
 * called it with an implicit int return, which is what makes MWCC emit the
 * signed int -> float "magic 2^52 double" sequence (xoris r0,r0,0x8000). */

/*************************************************************
 * mdmini.c
 *
 * Mini-game ("mdmini") overlay: common MD runtime (objects, debug
 * font, camera, light, sprite, model, texbuf, ring, mess windows,
 * math) plus the per-mini-game flow/pos-adjust/particle helpers.
 *
 * The original overlay shipped no symbol table for most of its
 * internal helpers, so those carry objdiff's placeholder names
 * (fn_1_XXXX) keyed to their .text offset. Descriptive comments
 * below give the recovered meaning.
 *************************************************************/

/* ---- object ids registered by ObjectSetup (MDMINI_OBJ_*) ---- */
enum {
    MDMINI_OBJ_DEBUGFONT,
    MDMINI_OBJ_PAD,
    MDMINI_OBJ_CAMERA,
    MDMINI_OBJ_LIGHT,
    MDMINI_OBJ_MAX
};

/* MdMain dispatches through MdFlowTbl[]; ObjectSetup selects entry 0
 * (fn_1_23D8), which is the overlay's init flow. */
enum {
    MDMINI_FLOW_INIT,
    MDMINI_FLOW_MAX
};

/* overlay-call modes, same encoding as the other MD overlays */
#define MD_OVLCALL_GOTO 0
#define MD_OVLCALL_CALL 1
#define MD_OVLCALL_RETURN 2

/* async data-load state machine driven by fn_1_8CC */
enum {
    MD_LOAD_STATE_NONE
};

#define MDMINI_CAMERA_MAX 3
#define MDMINI_LIGHT_MAX 1
#define MDMINI_MESS_WIN_MAX 2

typedef struct MdDebugFontWork_s {
    BOOL createF; //0x00
    int ctrl;    //0x04
    BOOL printF; //0x08
    int color;   //0x0C
    s16 posX;    //0x10
    s16 posY;    //0x12
    s16 ofsY;    //0x14
    s16 rowOfsY; //0x16
    s16 rowH;    //0x18
    s16 maxLen;  //0x1A
    float size;  //0x1C
} MDMINI_DEBUGFONT_WORK;

typedef struct MdPadWork_s {
    u16 DStkDown[4]; //0x00
    u16 DStk[4];     //0x08
} MDMINI_PAD_WORK;

typedef struct MdMiniCamera_s {
    u16 bit;       //0x00
    int focusType; //0x04
    HuVecF pos;    //0x08
    HuVecF target; //0x14
    HuVecF cross;  //0x20
    HuVecF up;     //0x2C
    HuVecF dir;    //0x38
    HuVecF hvr;    //0x44
    float unk50;   //0x50
    HuVecF center; //0x54
    HuVecF unk60[3];
    float fovy;    //0x84
} MDMINI_CAMERA;

typedef struct MdMiniCameraWork_s {
    BOOL createF;                       //0x00
    MDMINI_DEBUGFONT_WORK *debugFont;   //0x04
    MDMINI_CAMERA camera[MDMINI_CAMERA_MAX]; //0x08
    int selCamera;                      //0x1A0
} MDMINI_CAMERA_WORK;

typedef struct MdMiniLight_s {
    s16 type;         //0x00
    HU3DLIGHTID gLightId; //0x02
    HU3DLLIGHTID lLightId; //0x04
    HuVecF hvr;       //0x08
    HuVecF center;    //0x14
    HuVecF pos;       //0x20
    HuVecF target;    //0x2C
    HuVecF lightPos;  //0x38
    HuVecF lightDir;  //0x44
    GXColor rgb;      //0x50
} MDMINI_LIGHT;

typedef struct MdMiniLightWork_s {
    BOOL createF;                     //0x00
    int lightSel;                     //0x04
    MDMINI_DEBUGFONT_WORK *debugFont; //0x08
    MDMINI_LIGHT light[MDMINI_LIGHT_MAX]; //0x0C
} MDMINI_LIGHT_WORK;

/* mdmini allocates 0x68 bytes for the sprite work; mdmini.c never reads
 * its fields, so only the total size is known. */
typedef struct MdMiniSpriteWork_s {
    u8 opaque[0x68];
} MDMINI_SPRITE_WORK;

/* likewise, mdmini allocates 0x2E8 bytes for the model work. */
typedef struct MdMiniModelWork_s {
    u8 opaque[0x2E8];
} MDMINI_MODEL_WORK;

typedef struct MdTexBufUnit_s {
    void *addr;      //0x00
    GXTexObj texObj; //0x04
    s16 posX;        //0x24
    s16 posY;        //0x26
    u16 w;           //0x28
    u16 h;           //0x2A
    GXTexFmt texFmt; //0x2C
} MDMINI_TEXBUF_UNIT;

typedef struct MdMiniTexBufWork_s {
    MDMINI_TEXBUF_UNIT unit[10]; //0x00
    int unk1E0;                  //0x1E0
} MDMINI_TEXBUF_WORK;

typedef struct MdMiniStar_s {
    s16 unk0;     //0x00
    s16 mode;     //0x02
    s16 motNext;  //0x04
} MDMINI_STAR;

typedef struct MdMiniMessWin_s {
    s16 mode;     //0x00
    HUWINID winId;//0x02
    s16 choice;   //0x04
    s16 unk6;     //0x06
    s16 type;     //0x08
    s16 unkA;     //0x0A
} MDMINI_MESS_WIN;

typedef struct MdMiniMessWork_s {
    MDMINI_MESS_WIN win[MDMINI_MESS_WIN_MAX]; //0x00
} MDMINI_MESS_WORK;

typedef struct MdMiniRing_s {
    BOOL disp;      //0x00
    int mode;       //0x04
    GXColor color;  //0x08
    GXColor endColor; //0x0C
    int unk10;      //0x10
    float time;     //0x14
    float ofsTime;  //0x18
    float t;        //0x1C
    float ofsT;     //0x20
    int mdlNo;      //0x24
    HuVecF **posP;  //0x28
    HuVecF **rotP;  //0x2C
    HuVecF **scaleP; //0x30
    int baseMdlNo;  //0x34
    HuVecF baseOfs; //0x38
} MDMINI_RING;

typedef struct MdMiniRingWork_s {
    MDMINI_RING ring[4]; //0x00
    float scale;         //0x110
} MDMINI_RING_WORK;

/* Overlay state. Offsets are recovered from mdmini's code; regions mdmini.c
 * does not touch yet are held as reserved blocks so later functions can fill
 * them in without disturbing the layout. */
typedef struct MdMiniSys_s {
    int subMode;                    //0x00
    int unk4;                       //0x04
    int flowTime;                   //0x08
    int flowMode;                   //0x0C
    int ovlCallMode;                //0x10
    OMOVL nextDll;                  //0x14
    s8 exitF;                       //0x18
    s8 skipWipeF;                   //0x19
    u8 pad1A[0x32];                 //0x1A
    s8 playerState[4];              //0x4C
    s8 playerStatePrev[4];          //0x50
    u8 pad54[0x6];                  //0x54
    s8 comState[4];                 //0x5A
    s8 comStatePrev[4];             //0x5E
    u8 pad62[0x6];                  //0x62
    OMOBJMAN *objman;               //0x68
    HUPROCESS *mainProc;            //0x6C
    HUPROCESS *loadProc;            //0x70
    HUPROCESS *starProc;            //0x74
    HUPROCESS *playerSprProc;       //0x78
    HUPROCESS *chrProc;             //0x7C
    HUPROCESS *ringProc;            //0x80
    HUPROCESS *mainWinProc;         //0x84
    HUPROCESS *textWinProc;         //0x88
    OMOBJ *obj[MDMINI_OBJ_MAX];     //0x8C
    OMOVLHIS *ovlHis;               //0x9C
    int evtNo;                      //0xA0
    s16 loadState;                  //0xA4
    s16 loadAsyncStat;              //0xA6
    unsigned int loadDir;           //0xA8
    MDMINI_DEBUGFONT_WORK *debugFont; //0xAC
    MDMINI_PAD_WORK *pad;           //0xB0
    MDMINI_CAMERA_WORK *camera;     //0xB4
    MDMINI_LIGHT_WORK *light;       //0xB8
    MDMINI_SPRITE_WORK *sprite;     //0xBC
    MDMINI_MODEL_WORK *model;       //0xC0
    MDMINI_TEXBUF_WORK *texbuf;     //0xC4
    MDMINI_STAR *star;              //0xC8
    u8 padCC[0x2F8];                //0xCC
    MDMINI_MESS_WORK *mess;         //0x3C4
    MDMINI_RING_WORK *ring;         //0x3C8
} MDMINI_SYS;

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

extern void ObjectSetup(void);
extern void MdMain(void);
extern void fn_1_20A0(void);
extern void fn_1_22C8(void);
extern void MdRingMain(void);
extern void MdMathStdCurveInit(void);
extern void MdMessCreate(MDMINI_MESS_WORK *mess);
extern void MdMessKill(MDMINI_MESS_WORK *mess);
extern void MdMessMain(void *mess, int winNo);
extern void MdSpriteCreate(MDMINI_SPRITE_WORK *sprite);
extern void MdRingCreate(void);
extern void MdModelCreate(MDMINI_MODEL_WORK *model, MDMINI_CAMERA_WORK *camera);
extern void MdToueiTexBufInit(void);
extern void MdToueiMdlInit(void);

/* per-frame process bodies created by fn_1_20A0 */
extern void fn_1_8CC(void);
extern void fn_1_C4C(void);
extern void fn_1_1804(void);
extern void fn_1_1C04(void);
extern void fn_1_2018(void);
extern void fn_1_205C(void);

/* object procs supplied by mdminiDll's object code */
extern void MdObjDebugFontCreate(OMOBJ *obj);
extern void MdObjPadMain(OMOBJ *obj);
extern void MdObjCameraCreate(OMOBJ *obj);
extern void MdObjLightCreate(OMOBJ *obj);

typedef void (*MD_FUNC)(void);

/* flow state machine / proc table entries live in the not-yet-recovered
 * helpers below; declared so MdMain's dispatch table is complete. */
extern void fn_1_23D8(void);
extern void fn_1_27F8(void);
extern void fn_1_35A0(void);
extern void fn_1_5558(void);
extern void fn_1_6FB8(void);
extern void fn_1_7638(void);
extern void fn_1_81E0(void);

static MD_FUNC MdFlowTbl[7] = {
    fn_1_23D8,
    fn_1_27F8,
    fn_1_35A0,
    fn_1_5558,
    fn_1_6FB8,
    fn_1_7638,
    fn_1_81E0,
};

MDMINI_SYS sysWork;
MDMINI_SYS *sys;
MDMINI_SYS *sysP;

int _prolog(void)
{
    const VoidFunc *ctors = _ctors;
    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const VoidFunc *dtors = _dtors;
    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

/* 0xA0 - ObjectSetup: brings up the overlay's object manager, its four
 * fixed objects and the shared sprite/model/texbuf/ring/star/mess work
 * areas, then hands control to MdMain's process. */
void ObjectSetup(void)
{
    sys = &sysWork;
    sysP = sys;
    sys->subMode = 0;
    sys->unk4 = 0;
    sys->flowTime = 0;
    sys->flowMode = MDMINI_FLOW_INIT;
    sys->ovlCallMode = MD_OVLCALL_GOTO;
    sys->nextDll = DLL_selmenuDLL;
    sys->exitF = FALSE;
    sys->skipWipeF = FALSE;
    GwSystem.subGameNo = 0;
    _SetFlag(FLAG_INST_MGMODE);
    mgPauseExitF = FALSE;
    HuDataDirClose(DATA_board);
    HuARDirFree(DATA_board);
    HuDataDirClose(DATA_w20);
    HuARDirFree(DATA_w20);
    sys->objman = omInitObjMan(512, 8192);
    sys->ovlHis = omOvlHisGet(0);
    sys->evtNo = sys->ovlHis->evtno;
    omGameSysInit(sys->objman);
    sys->obj[MDMINI_OBJ_DEBUGFONT] = omAddObj(sys->objman, 100, 0, 0, MdObjDebugFontCreate);
    sys->obj[MDMINI_OBJ_DEBUGFONT]->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDMINI_DEBUGFONT_WORK), HU_MEMNUM_OVL);
    if(!sys->obj[MDMINI_OBJ_DEBUGFONT]->data) {
        OSReport("sys->obj[MDMINI_OBJ_DEBUGFONT]->data = NULL...\n");
    }
    sys->debugFont = sys->obj[MDMINI_OBJ_DEBUGFONT]->data;
    sys->debugFont->createF = FALSE;
    sys->obj[MDMINI_OBJ_PAD] = omAddObj(sys->objman, 101, 0, 0, MdObjPadMain);
    sys->obj[MDMINI_OBJ_PAD]->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDMINI_PAD_WORK), HU_MEMNUM_OVL);
    if(!sys->obj[MDMINI_OBJ_PAD]->data) {
        OSReport("sys->obj[MDMINI_OBJ_PAD]->data = NULL...\n");
    }
    sys->pad = sys->obj[MDMINI_OBJ_PAD]->data;
    sys->obj[MDMINI_OBJ_CAMERA] = omAddObj(sys->objman, 102, 0, 0, MdObjCameraCreate);
    sys->obj[MDMINI_OBJ_CAMERA]->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDMINI_CAMERA_WORK), HU_MEMNUM_OVL);
    if(!sys->obj[MDMINI_OBJ_CAMERA]->data) {
        OSReport("sys->obj[MDMINI_OBJ_CAMERA]->data = NULL...\n");
    }
    sys->camera = sys->obj[MDMINI_OBJ_CAMERA]->data;
    sys->camera->createF = FALSE;
    sys->camera->debugFont = sys->debugFont;
    sys->obj[MDMINI_OBJ_LIGHT] = omAddObj(sys->objman, 103, 0, 0, MdObjLightCreate);
    sys->obj[MDMINI_OBJ_LIGHT]->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDMINI_LIGHT_WORK), HU_MEMNUM_OVL);
    if(!sys->obj[MDMINI_OBJ_LIGHT]->data) {
        OSReport("sys->obj[MDMINI_OBJ_LIGHT]->data = NULL...\n");
    }
    sys->light = sys->obj[MDMINI_OBJ_LIGHT]->data;
    sys->light->createF = FALSE;
    sys->light->debugFont = sys->debugFont;
    sys->sprite = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDMINI_SPRITE_WORK), HU_MEMNUM_OVL);
    if(!sys->sprite) {
        OSReport("sys->sprite = NULL...\n");
    }
    sys->model = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDMINI_MODEL_WORK), HU_MEMNUM_OVL);
    if(!sys->model) {
        OSReport("sys->model = NULL...\n");
    }
    sys->texbuf = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDMINI_TEXBUF_WORK), HU_MEMNUM_OVL);
    if(!sys->texbuf) {
        OSReport("sys->texbuf = NULL...\n");
    }
    sys->ring = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDMINI_RING_WORK), HU_MEMNUM_OVL);
    if(!sys->ring) {
        OSReport("sys->ring = NULL...\n");
    }
    sys->star = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDMINI_STAR), HU_MEMNUM_OVL);
    if(!sys->star) {
        OSReport("sys->star = NULL...\n");
    }
    sys->mess = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDMINI_MESS_WORK), HU_MEMNUM_OVL);
    if(!sys->mess) {
        OSReport("sys->mess = NULL...\n");
    }
    MdMessCreate(sys->mess);
    MdMathStdCurveInit();
    Hu3DBGColorSet(0, 0, 0);
    Hu3DNoSyncSet(FALSE);
    while(1) {
        if(sys->debugFont->createF == TRUE && sys->camera->createF == TRUE && sys->light->createF == TRUE) {
            break;
        }
        HuPrcVSleep();
    }
    sys->mainProc = HuPrcChildCreate(MdMain, 105, 12288, 0, sys->objman);
}

/* 0x808 - main overlay thread body. Kept ahead of its MdInit/MdClose
 * helpers: mdmini's build inlines any helper whose body it can already see,
 * and the overlay relies on the real calls. */
void MdMain(void)
{
    fn_1_20A0();
    while(!omSysExitReq && !sys->exitF) {
        MdFlowTbl[sys->flowMode]();
        if(++sys->flowTime > 36000) {
            sys->flowTime = 0;
        }
        HuPrcVSleep();
    }
    fn_1_22C8();
}

/* 0x20A0 - MdInit: brings up the shared subsystems and spawns the per-frame
 * processes the flow handlers rely on. */
void fn_1_20A0(void)
{
    MdSpriteCreate(sys->sprite);
    HuPrcVSleep();
    MdRingCreate();
    MdModelCreate(sys->model, sys->camera);
    MdToueiTexBufInit();
    HuPrcVSleep();
    HuDataDirClose(DATA_mdmini);
    HuPrcVSleep();
    if(sys->evtNo == 1) {
        SLSaveModeExec(0);
    }
    HuPrcVSleep();
    MdToueiMdlInit();
    sys->starProc = HuPrcChildCreate(fn_1_C4C, 107, 12288, 0, sys->objman);
    sys->playerSprProc = HuPrcChildCreate(fn_1_1804, 108, 12288, 0, sys->objman);
    sys->chrProc = HuPrcChildCreate(fn_1_1C04, 109, 12288, 0, sys->objman);
    sys->mainWinProc = HuPrcChildCreate(fn_1_2018, 110, 4096, 0, sys->objman);
    sys->textWinProc = HuPrcChildCreate(fn_1_205C, 111, 4096, 0, sys->objman);
    sys->loadState = MD_LOAD_STATE_NONE;
    sys->loadAsyncStat = 0;
    sys->loadDir = DATA_rescard;
    sys->loadProc = HuPrcChildCreate(fn_1_8CC, 106, 12288, 0, sys->objman);
    HuPrcVSleep();
}

/* 0x22C8 - MdClose: wipe out, release the message windows and hand control
 * to whichever overlay MdMain was launched from. */
void fn_1_22C8(void)
{
    if(!sys->skipWipeF) {
        WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 60);
        WipeWait();
    }
    HuAudAllStop();
    MdMessKill(sys->mess);
    if(sys->ovlCallMode == MD_OVLCALL_GOTO) {
        omOvlGoto(sys->nextDll, 0, 0);
    } else if(sys->ovlCallMode == MD_OVLCALL_CALL) {
        omOvlCall(sys->nextDll, 0, 0);
    } else if(sys->ovlCallMode == MD_OVLCALL_RETURN) {
        omOvlReturn(1);
    }
HuPrcEnd();
    while(1) {
        HuPrcVSleep();
    }
}

/* 0x2000 - RingUpdate: per-frame ring animation driver */
void fn_1_2000(void)
{
    while (1) {
        MdRingMain();
        HuPrcVSleep();
    }
}

/* 0x2018 - MainWinUpdate: per-frame main message window driver */
void fn_1_2018(void)
{
    MDMINI_MESS_WORK *mess;

    mess = sys->mess;
    while (1) {
        mess = sys->mess;
        MdMessMain(mess, 0);
        HuPrcVSleep();
    }
}

/* 0x205C - TextWinUpdate: per-frame help/team text window driver */
void fn_1_205C(void)
{
    MDMINI_MESS_WORK *mess;

    mess = sys->mess;
    while (1) {
        mess = sys->mess;
        MdMessMain(mess, 1);
        HuPrcVSleep();
    }
}


/* 0x16168 - latch the four per-player state bytes into their previous-value
 * copies (done once when a state change is committed). */
void fn_1_16168(void)
{
    int i;

    for(i = 0; i < 4; i++) {
        sys->playerStatePrev[i] = sys->playerState[i];
    }
}

/* 0x16308 - same latch for the four computer slots */
void fn_1_16308(void)
{
    int i;

    for(i = 0; i < 4; i++) {
        sys->comStatePrev[i] = sys->comState[i];
    }
}

/* ------------------------------------------------------------
 * Particle effects (particle "dot" triple + single "glow").
 * Shared shape with mdpartyDll/effect.c; mdmini uses its own
 * HU3DPARMANPARAM tables for the last two entries.
 * ---------------------------------------------------------- */

typedef struct MdMiniEffect_s {
    int parManId[3];
} MDMINI_EFFECT;

static MDMINI_EFFECT *ParticleDotCreate(ANIMDATA *animP, s16 layer, s16 cameraBit)
{
    static HU3DPARMANPARAM param1 = {
        30,
        0,
        50,
        10,
        180,
        { 0, -0.05f, 0 },
        20,
        0.8f,
        10,
        1,
        2,
        {
            { 0xFF, 0xFF, 0xFF, 0xFF },
            { 0xFF, 0xFF, 0x40, 0xFF },
        },
        {
            { 0xFF, 0x80, 0x80, 0x00 },
            { 0xFF, 0x40, 0x20, 0x00 },
        }
    };
    static HU3DPARMANPARAM param2 = {
        40,
        0,
        50,
        0,
        90,
        { 0, -0.05f, 0 },
        35,
        0.88f,
        25,
        0.99f,
        1,
        {
            { 0xFF, 0xFF, 0xFF, 0xFF },
        },
        {
            { 0xFF, 0x80, 0x80, 0x00 },
        }
    };
    static HU3DPARMANPARAM param3 = {
        30,
        0,
        30,
        60,
        180,
        { 0, 0, 0 },
        0.2f,
        1.0f,
        100,
        1.0f,
        2,
        {
            { 0xFF, 0x20, 0x00, 0x00 },
            { 0x00, 0x00, 0x00, 0x00 },
            { 0x00, 0x00, 0x00, 0x00 },
            { 0x40, 0x20, 0x00, 0x00 },
        },
        {
            { 0x20, 0x00, 0x00, 0x00 },
            { 0x00, 0x00, 0x00, 0x00 },
            { 0x00, 0x00, 0x00, 0x00 },
            { 0x00, 0x00, 0x00, 0x00 },
        }
    };
    HU3DPARMANID parManId;
    MDMINI_EFFECT *effectP;
    effectP = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDMINI_EFFECT), HU_MEMNUM_OVL);

    effectP->parManId[0] = parManId = Hu3DParManCreate(animP, 100, &param1);
    Hu3DParManAttrSet(parManId, HU3D_PARMAN_ATTR_SCALEJITTER|HU3D_PARMAN_ATTR_RANDSCALE70|HU3D_PARMAN_ATTR_RANDSPEED70|HU3D_PARMAN_ATTR_TIMEUP);
    Hu3DParticleBlendModeSet(Hu3DParManModelIDGet(parManId), HU3D_PARTICLE_BLEND_ADDCOL);
    Hu3DParManRotSet(parManId, 90, 0, 0);
    Hu3DModelLayerSet(Hu3DParManModelIDGet(parManId), layer);
    Hu3DModelCameraSet(Hu3DParManModelIDGet(parManId), cameraBit);
    effectP->parManId[1] = parManId = Hu3DParManCreate(animP, 100, &param2);
    Hu3DParManAttrSet(parManId, HU3D_PARMAN_ATTR_RANDANGLE|HU3D_PARMAN_ATTR_SCALEJITTER|HU3D_PARMAN_ATTR_RANDSCALE70|HU3D_PARMAN_ATTR_RANDSPEED90|HU3D_PARMAN_ATTR_TIMEUP);
    Hu3DParticleBlendModeSet(Hu3DParManModelIDGet(parManId), HU3D_PARTICLE_BLEND_ADDCOL);
    Hu3DParManRotSet(parManId, frandmod(40)+70, 0, frandmod(80)-40);
    Hu3DModelLayerSet(Hu3DParManModelIDGet(parManId), layer);
    Hu3DModelCameraSet(Hu3DParManModelIDGet(parManId), cameraBit);
    effectP->parManId[2] = parManId = Hu3DParManCreate(animP, 100, &param3);
    Hu3DParManAttrSet(parManId, HU3D_PARMAN_ATTR_RANDSCALE70|HU3D_PARMAN_ATTR_TIMEUP);
    Hu3DParticleBlendModeSet(Hu3DParManModelIDGet(parManId), HU3D_PARTICLE_BLEND_ADDCOL);
    Hu3DParManRotSet(parManId, 0, 0, 0);
    Hu3DModelLayerSet(Hu3DParManModelIDGet(parManId), layer);
    Hu3DModelCameraSet(Hu3DParManModelIDGet(parManId), cameraBit);
    return effectP;
}

static void ParticleDotPosSet(MDMINI_EFFECT *effectP, HuVecF *pos)
{
    Hu3DParManPosSet(effectP->parManId[0], pos->x, pos->y, pos->z);
    Hu3DParManTimeLimitSet(effectP->parManId[0], 2);
    Hu3DParManAttrReset(effectP->parManId[0], HU3D_PARMAN_ATTR_TIMEUP);
    Hu3DParManPosSet(effectP->parManId[1], pos->x, pos->y, pos->z);
    Hu3DParManTimeLimitSet(effectP->parManId[1], 2);
    Hu3DParManAttrReset(effectP->parManId[1], HU3D_PARMAN_ATTR_TIMEUP);
    Hu3DParManPosSet(effectP->parManId[2], pos->x, pos->y, pos->z);
    Hu3DParManTimeLimitSet(effectP->parManId[2], 5);
    Hu3DParManAttrReset(effectP->parManId[2], HU3D_PARMAN_ATTR_TIMEUP);
}

static void ParticleDotKill(MDMINI_EFFECT *effectP)
{
    Hu3DParManKill(effectP->parManId[0]);
    Hu3DParManKill(effectP->parManId[1]);
    Hu3DParManKill(effectP->parManId[2]);
}

static MDMINI_EFFECT *ParticleGlowCreate(ANIMDATA *animP, s16 layer, s16 cameraBit)
{
    static HU3DPARMANPARAM param = {
        50,
        0,
        2,
        30,
        180,
        { 0, -0.2f, 0 },
        1,
        0.8f,
        30,
        0.95f,
        3,
        {
            { 0xFF, 0xFF, 0xFF, 0xFF },
            { 0x40, 0xFF, 0x00, 0x00 },
            { 0x00, 0x00, 0x00, 0x00 },
            { 0x00, 0x00, 0xFF, 0x80 },
        },
        {
            { 0x80, 0xFF, 0x00, 0x00 },
            { 0x00, 0x00, 0x80, 0xC0 },
            { 0xFF, 0x00, 0xFF, 0x40 },
            { 0x20, 0x00, 0x80, 0x10 },
        }
    };
    HU3DPARMANID parManId;
    MDMINI_EFFECT *effectP;
    effectP = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDMINI_EFFECT), HU_MEMNUM_OVL);
    effectP->parManId[0] = parManId = Hu3DParManCreate(animP, 100, &param);
    Hu3DParManAttrSet(parManId, HU3D_PARMAN_ATTR_SCALEJITTER|HU3D_PARMAN_ATTR_RANDSCALE70|HU3D_PARMAN_ATTR_RANDSPEED70|HU3D_PARMAN_ATTR_TIMEUP);
    Hu3DParticleBlendModeSet(Hu3DParManModelIDGet(parManId), HU3D_PARTICLE_BLEND_ADDCOL);
    Hu3DParManRotSet(parManId, 90, 0, 0);
    Hu3DModelLayerSet(Hu3DParManModelIDGet(parManId), layer);
    Hu3DModelCameraSet(Hu3DParManModelIDGet(parManId), cameraBit);
    return effectP;
}

static void ParticleGlowPosSet(MDMINI_EFFECT *effectP, HuVecF *pos)
{
    Hu3DParManPosSet(effectP->parManId[0], pos->x, pos->y, pos->z);
    Hu3DParManTimeLimitSet(effectP->parManId[0], 2);
    Hu3DParManAttrReset(effectP->parManId[0], HU3D_PARMAN_ATTR_TIMEUP);
}

static void ParticleGlowKill(MDMINI_EFFECT *effectP)
{
    Hu3DParManKill(effectP->parManId[0]);
}

