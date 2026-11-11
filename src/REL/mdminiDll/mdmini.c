#include "REL/mdminiDll.h"
#include "game/memory.h"
#include "game/process.h"
#include "game/armem.h"
#include "game/data.h"
#include "game/flag.h"
#include "game/gamework.h"
#include "game/wipe.h"
#include "game/saveload.h"
#include "game/mgdata.h"

#include "messnum/partymode_sys.h"
#include "messnum/mapname.h"
#include "messnum/teamname.h"
#include "messnum/syshelp.h"
#include "messnum/mgmode_sys.h"

/* not game/board/main.h: it pulls in game/frand.h */
extern void MBPartySaveInit(int boardNo);

/* frandmod is deliberately left undeclared (no game/frand.h): the original
 * called it with an implicit int return, which is what makes MWCC emit the
 * signed int -> float "magic 2^52 double" sequence (xoris r0,r0,0x8000). */

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

extern void ObjectSetup(void);
extern void MdMain(void);
extern void fn_1_20A0(void);
extern void fn_1_22C8(void);

/* per-frame process bodies created by fn_1_20A0 */
extern void fn_1_8CC(void);
extern void fn_1_C4C(void);
extern void fn_1_1804(void);
extern void fn_1_1C04(void);
extern void fn_1_2018(void);
extern void fn_1_205C(void);

/* flow state machine / proc table entries live in the not-yet-recovered
 * helpers below; declared so MdMain's dispatch table is complete. */
extern void fn_1_23D8(void);
extern void fn_1_27F8(void);
extern void fn_1_35A0(void);
extern void fn_1_5558(void);
extern void fn_1_6FB8(void);
extern void fn_1_7638(void);
extern void fn_1_81E0(void);

#define MDMINI_STAR_NONE 0
#define MDMINI_STAR_BACK_START 1
#define MDMINI_STAR_BACK 2
#define MDMINI_STAR_BACK_END 3
#define MDMINI_STAR_DECENTER_START 4
#define MDMINI_STAR_DECENTER 5
#define MDMINI_STAR_DECENTER_END 6
#define MDMINI_STAR_MOVE_LEFT_START 7
#define MDMINI_STAR_MOVE_LEFT 8
#define MDMINI_STAR_MOVE_LEFT_END 9
#define MDMINI_STAR_SNAP_LEFT 10
#define MDMINI_STAR_CENTER_START 11
#define MDMINI_STAR_CENTER 12
#define MDMINI_STAR_CENTER_END 13

#define MDMINI_STAR_MOT_NONE 0
#define MDMINI_STAR_MOT_RESET 1
#define MDMINI_STAR_MOT_TALK 2
#define MDMINI_STAR_MOT_IDLE 3

#define MDMINI_PLAYERSPR_NONE 0
#define MDMINI_PLAYERSPR_GROW_START 1
#define MDMINI_PLAYERSPR_GROW 2
#define MDMINI_PLAYERSPR_GROW_END 3
#define MDMINI_PLAYERSPR_SHRINK_START 4
#define MDMINI_PLAYERSPR_SHRINK 5
#define MDMINI_PLAYERSPR_SHRINK_END 6

#define MDMINI_CHR_NONE 0
#define MDMINI_CHR_INIT 1
#define MDMINI_CHR_SELECT 2
#define MDMINI_CHR_SELECT_WAIT 3
#define MDMINI_CHR_SELECT_MOVE 4
#define MDMINI_CHR_SELECT_MOVE_WAIT 5

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

char *lbl_1_data_144 = "26";

static OMOVL NextDllTbl[6] = {
    DLL_mgfreedll,
    DLL_mgmatchdll,
    DLL_w20dll,
    DLL_decathlondll,
    DLL_mgwarsdll,
    DLL_mgtourdll,
};

static unsigned int LoadDirTbl[6] = {
    DATA_mgfree,
    DATA_mgmatch,
    DATA_w20,
    DATA_decathlon,
    DATA_mgwars,
    DATA_mgtour,
};

static HuVecF CharPos[10][2] = {
	{ { -250, 0, 0 }, { -250, 0, 0 } },
	{ { -125, 0, 0 }, { -125, 0, 0 } },
	{ { 0, 0, 0 },  { 0, 0, 0 } },
	{ { 125, 0, 0 }, { 139, 17.5, -500 } },
	{ { 275, 9, -300 }, { 266, 2, -200 } },
	{ { -250, -250, 0 }, { -250, -250, 0 } },
	{ { -125, -250, 0 }, { -140.5, -270, -400 } },
	{ { 0, -270, -300 }, { -8.5, -290.5, -629 } },
	{ { 125, -250, -1800 }, { 122, -261, -1800 } },
	{ { 250, -250, -1000 }, { 245.5, -254, -1000 } }
};

static HuVecF CharRot[10][2] = {
	{ { 0, 0, 0 }, { 0, 0, 0 } },
    { { 0, 0, 0 }, { 0, 0, 0 } },
    { { 0, 0, 0 }, { 0, 0, 0 } },
    { { 10, -2, 0 }, { 10, -4, 0 } },
    { { 0, -3, 0 }, { 0, -6, 0 } },
    { { 0, 0, 0 }, { 0, 0, 0 } },
    { { 0, 0, 0 }, { 0, 0, 0 } },
    { { 0, 0, 0 }, { 0, 0, 0 } },
    { { 0, 0, 0 }, { 0, 0, 0 } },
    { { 0, -4, 0 }, { 0, -2, 0 } }
};

#define MDMINI_AUD_MUS_START 0
#define MDMINI_AUD_MUS_STOP 1
#define MDMINI_AUD_SE_CMN_MOVE 2
#define MDMINI_AUD_SE_CMN_CONFIRM 3
#define MDMINI_AUD_SE_CMN_CHRSEL 4
#define MDMINI_AUD_SE_CMN_CANCEL 5
#define MDMINI_AUD_SE_CMN_SELECT 6
#define MDMINI_AUD_SE_MENU_MOVE 7
#define MDMINI_AUD_SE_BOARD_START 8
#define MDMINI_AUD_SE_BOARD_ENTER1 9
#define MDMINI_AUD_SE_STAR_ENTER 11
#define MDMINI_AUD_SE_STAR_CONFIRM 12
#define MDMINI_AUD_SE_CHR_MARIO 13
#define MDMINI_AUD_SE_CHR_LUIGI 14
#define MDMINI_AUD_SE_CHR_PEACH 15
#define MDMINI_AUD_SE_CHR_YOSHI 16
#define MDMINI_AUD_SE_CHR_WARIO 17
#define MDMINI_AUD_SE_CHR_DAISY 18
#define MDMINI_AUD_SE_CHR_WALUIGI 19
#define MDMINI_AUD_SE_CHR_KINOPIO 20
#define MDMINI_AUD_SE_CHR_TERESA 21
#define MDMINI_AUD_SE_CHR_MINIKOOPA 22
#define MDMINI_AUD_SE_ALL_STOP 23

extern s8 capPlayerInitF;

#define MDMINI_CURSOR_START 0
#define MDMINI_CURSOR_PLAYERSEL 1
#define MDMINI_CURSOR_CHR_DIF_SEL 2
#define MDMINI_CURSOR_TEAM_DIF_SEL 3
#define MDMINI_CURSOR_TEAM_SEL 4
#define MDMINI_CURSOR_MAP_SEL 5
#define MDMINI_CURSOR_TURN_SEL 6
#define MDMINI_CURSOR_PACK_SEL 7
#define MDMINI_CURSOR_BONUS_SEL 8
#define MDMINI_CURSOR_HANDICAP 9
#define MDMINI_CURSOR_HANDICAP_TEAM 10

/* @protos */
void fn_1_8CC(void);
void fn_1_C4C(void);
void fn_1_1804(void);
void fn_1_1C04(void);
void fn_1_2000(void);
void fn_1_2018(void);
void fn_1_205C(void);
void fn_1_20A0(void);
void fn_1_22C8(void);
void fn_1_23D8(void);
void fn_1_6FB8(void);
void fn_1_7638(void);
BOOL fn_1_9714(int param);
BOOL fn_1_991C(int param);
BOOL fn_1_AF34(int param);
BOOL fn_1_B668(int param);
BOOL fn_1_F6C4(int param);
BOOL fn_1_FC94(int param);
BOOL fn_1_10264(int param);
BOOL fn_1_106D4(int param);
BOOL fn_1_10B0C(int param);
BOOL fn_1_113D8(int param);
BOOL fn_1_149A4(int param);
void fn_1_14C2C(void);
void fn_1_14E24(void);
void fn_1_14FD0(void);
void fn_1_152D4(void);
void fn_1_153D0(void);
BOOL fn_1_15590(int playerNo, BOOL comSelF);
void fn_1_15BF4(void);
void fn_1_15CF8(int mdlNo, int cameraNo);
void fn_1_15DE8(void);
void fn_1_16168(void);
void fn_1_161B8(void);
void fn_1_16308(void);
void fn_1_16358(void);
void fn_1_16978(void);
void fn_1_17008(int cmd);
int fn_1_171B4(int mdlNo, int cameraNo);
void fn_1_27F8(void);
void fn_1_35A0(void);
void fn_1_5558(void);
void fn_1_81E0(void);
BOOL fn_1_9AC8(int param);
BOOL fn_1_C8B4(int param);
BOOL fn_1_E44C(int param);
BOOL fn_1_11658(int param);
/* @end */

MD_FUNC MdFlowTbl[MDMINI_FLOW_MAX] = {
    fn_1_23D8,
    fn_1_27F8,
    fn_1_35A0,
    fn_1_5558,
    fn_1_6FB8,
    fn_1_7638,
    fn_1_81E0,
};

MD_PROC MdProcTbl[MDMINI_PROC_MAX] = {
    fn_1_9714,
    fn_1_991C,
    fn_1_9AC8,
    fn_1_AF34,
    fn_1_B668,
    fn_1_C8B4,
    fn_1_E44C,
    fn_1_F6C4,
    fn_1_FC94,
    fn_1_10264,
    fn_1_106D4,
    fn_1_10B0C,
    fn_1_113D8,
    fn_1_11658,
    fn_1_149A4,
};

SYS sysWork;
SYS *sys;
SYS *sysP;

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
    sys->mode = 0;
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
    sys->obj[MDMINI_OBJ_DEBUGFONT]->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDDEBUGFONT_WORK), HU_MEMNUM_OVL);
    if(!sys->obj[MDMINI_OBJ_DEBUGFONT]->data) {
        OSReport("sys->obj[MDMINI_OBJ_DEBUGFONT]->data = NULL...\n");
    }
    sys->debugFont = sys->obj[MDMINI_OBJ_DEBUGFONT]->data;
    sys->debugFont->createF = FALSE;
    sys->obj[MDMINI_OBJ_PAD] = omAddObj(sys->objman, 101, 0, 0, MdObjPadMain);
    sys->obj[MDMINI_OBJ_PAD]->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDPAD_WORK), HU_MEMNUM_OVL);
    if(!sys->obj[MDMINI_OBJ_PAD]->data) {
        OSReport("sys->obj[MDMINI_OBJ_PAD]->data = NULL...\n");
    }
    sys->pad = sys->obj[MDMINI_OBJ_PAD]->data;
    sys->obj[MDMINI_OBJ_CAMERA] = omAddObj(sys->objman, 102, 0, 0, MdObjCameraCreate);
    sys->obj[MDMINI_OBJ_CAMERA]->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDCAMERA_WORK), HU_MEMNUM_OVL);
    if(!sys->obj[MDMINI_OBJ_CAMERA]->data) {
        OSReport("sys->obj[MDMINI_OBJ_CAMERA]->data = NULL...\n");
    }
    sys->camera = sys->obj[MDMINI_OBJ_CAMERA]->data;
    sys->camera->createF = FALSE;
    sys->camera->debugFont = sys->debugFont;
    sys->obj[MDMINI_OBJ_LIGHT] = omAddObj(sys->objman, 103, 0, 0, MdObjLightCreate);
    sys->obj[MDMINI_OBJ_LIGHT]->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDLIGHT_WORK), HU_MEMNUM_OVL);
    if(!sys->obj[MDMINI_OBJ_LIGHT]->data) {
        OSReport("sys->obj[MDMINI_OBJ_LIGHT]->data = NULL...\n");
    }
    sys->light = sys->obj[MDMINI_OBJ_LIGHT]->data;
    sys->light->createF = FALSE;
    sys->light->debugFont = sys->debugFont;
    sys->sprite = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDSPRITE_WORK), HU_MEMNUM_OVL);
    if(!sys->sprite) {
        OSReport("sys->sprite = NULL...\n");
    }
    sys->model = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDMODEL_WORK), HU_MEMNUM_OVL);
    if(!sys->model) {
        OSReport("sys->model = NULL...\n");
    }
    sys->texbuf = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDTEXBUF_WORK), HU_MEMNUM_OVL);
    if(!sys->texbuf) {
        OSReport("sys->texbuf = NULL...\n");
    }
    sys->ring = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDRING_WORK), HU_MEMNUM_OVL);
    if(!sys->ring) {
        OSReport("sys->ring = NULL...\n");
    }
    sys->star = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDMINI_STAR), HU_MEMNUM_OVL);
    if(!sys->star) {
        OSReport("sys->star = NULL...\n");
    }
    sys->mess = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDMESS), HU_MEMNUM_OVL);
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

/* 0x8CC - LoadProc: asynchronous data-directory loader driven by sys->loadState */
void fn_1_8CC(void)
{
    OSTick tick;
    int i;
    int charNo;

    while(1) {
        switch(sys->loadState) {
            case MD_LOAD_STATE_NONE:
                break;

            case MD_LOAD_STATE_SEL_LOAD:
                tick = OSGetTick();
                sys->loadAsyncStat = HuDataDirReadAsync(DATA_mdsel);
                while(!HuDataGetAsyncStat(sys->loadAsyncStat)) {
                    HuPrcVSleep();
                }
                OSReport("Load Time:%d\n", OSTicksToMilliseconds(OSGetTick()-tick));
                OSReport("Finished!\n");
                sys->loadState = MD_LOAD_STATE_DONE;
                break;

            case MD_LOAD_STATE_LOAD:
                CharDataClose(-1);
                for(i=0; i<GW_PLAYER_MAX; i++) {
                    charNo = GwPlayerConf[i].charNo;
                    CharMotionInitAsync(charNo);
                }
                tick = OSGetTick();
                sys->loadAsyncStat = HuDataDirReadAsync(sys->loadDir);
                while(!HuDataGetAsyncStat(sys->loadAsyncStat)) {
                    HuPrcVSleep();
                }
                OSReport("Load Time:%d\n", OSTicksToMilliseconds(OSGetTick()-tick));
                OSReport("Finished!\n");
                sys->loadState = MD_LOAD_STATE_DONE;
                break;

            case MD_LOAD_STATE_BOARD_LOAD:
                CharDataClose(-1);
                tick = OSGetTick();
                sys->loadAsyncStat = HuDataDirReadAsync(DATA_board);
                while(!HuDataGetAsyncStat(sys->loadAsyncStat)) {
                    HuPrcVSleep();
                }
                OSReport("Load Time:%d\n", OSTicksToMilliseconds(OSGetTick()-tick));
                OSReport("Finished!\n");
                HuAR_MRAMtoARAM(DATA_board);
                while(HuARDMACheck());
                HuDataDirClose(DATA_board);
                for(i=0; i<GW_PLAYER_MAX; i++) {
                    charNo = GwPlayerConf[i].charNo;
                    CharMotionInitAsync(charNo);
                }
                tick = OSGetTick();
                sys->loadAsyncStat = HuDataDirReadAsync(DATA_w20);
                while(!HuDataGetAsyncStat(sys->loadAsyncStat)) {
                    HuPrcVSleep();
                }
                OSReport("Load Time:%d\n", OSTicksToMilliseconds(OSGetTick()-tick));
                OSReport("Finished!\n");
                sys->loadState = MD_LOAD_STATE_DONE;
                break;

            case MD_LOAD_STATE_DONE:
                HuPrcEnd();
                break;
        }
        HuPrcVSleep();
    }
}

void fn_1_C4C(void)
{
    MDMINI_STAR *star; //r31
    MDMODEL_WORK *model; //r30
    HuVecF *posP; //r29
    MDCAMERA_WORK *camera; //r28
    int i; //r27
    HuVecF *rotP; //r26
    MDMESS *mess; //r25
    HuVecF *scaleP; //r24
    
    static float curveData[5] = {
        0,
        16,
        64,
        32,
        -65
    };
    
    static HuVecF pos2D[2];
    static HuVecF pos3D[2];
    static float lerpTime;
    static float lerpOfsTime;
    static float curveTime;
    static float lerpT;
    static float lerpOfsT;
    static float curveT;

    model = sys->model;
    camera = sys->camera;
    mess = sys->mess;
    star = sys->star;
    posP = model->posP[MDMODEL_STAR];
    rotP = model->rotP[MDMODEL_STAR];
    scaleP = model->scaleP[MDMODEL_STAR];
    star->mode = MDMINI_STAR_NONE;
    star->motNext = 1;
    
    posP->x = 0;
    posP->y = 0;
    posP->z = 1800;
    fn_1_15CF8(MDMODEL_STAR, MDCAMERA_FG);
    while(1) {
        model = sys->model;
        camera = sys->camera;
        mess = sys->mess;
        star = sys->star;
        posP = model->posP[MDMODEL_STAR];
        rotP = model->rotP[MDMODEL_STAR];
        scaleP = model->scaleP[MDMODEL_STAR];
        
        switch(star->mode) {
            case MDMINI_STAR_NONE:
                break;

            case MDMINI_STAR_BACK_START:
                lerpTime = 0;
                lerpOfsTime = 0;
                curveTime = 0;
                lerpT = 0;
                lerpOfsT = 0;
                curveT = 0;
                MdMathCurveInit(5, sys->curveTime, curveData, sys->curveSlope);
                star->mode = MDMINI_STAR_BACK;
                break;
            
            case MDMINI_STAR_BACK:
                MdMathOscillate(MDMATH_OSC_90_REV, &lerpTime, &lerpT, 60);
                MdMathOscillate(MDMATH_OSC_180, &lerpOfsTime, &lerpOfsT, 60);
                MdMathOscillate(MDMATH_OSC_0, &curveTime, &curveT, 60);
                MdMathLerp(&posP->x, 0, 45, lerpOfsT);
                MdMathCurveGet(&posP->y, 5, sys->curveTime, curveData, sys->curveSlope, curveT);
                MdMathLerp(&posP->z, 1800, 1300, lerpT);
                MdMathLerp(&rotP->y, 0, 360, lerpT);
                if(lerpTime >= 1) {
                    star->mode = MDMINI_STAR_BACK_END;
                }
                break;
            
            case MDMINI_STAR_BACK_END:
                break;

            case MDMINI_STAR_DECENTER_START:
                lerpTime = 0;
                lerpOfsTime = 0;
                lerpT = 0;
                lerpOfsT = 0;
                pos2D[0].x = 64;
                pos2D[0].y = 320;
                pos2D[0].z = 1000;
                pos2D[1].x = 288;
                pos2D[1].y = 240;
                pos2D[1].z = 1000;
                for(i=0; i<2; i++) {
                    Hu3D2Dto3D(&pos2D[i], camera->camera[MDCAMERA_FG].bit, &pos3D[i]);
                }
                star->mode = MDMINI_STAR_DECENTER;
                break;
            
            case MDMINI_STAR_DECENTER:
                MdMathOscillate(MDMATH_OSC_90, &lerpTime, &lerpT, 30);
                MdMathLerp(&posP->x, pos3D[0].x, pos3D[1].x, lerpT);
                MdMathLerp(&posP->y, pos3D[0].y, pos3D[1].y, lerpT);
                MdMathLerp(&posP->z, pos3D[0].z, pos3D[1].z, lerpT);
                if(lerpTime >= 1) {
                    star->mode = MDMINI_STAR_DECENTER_END;
                }
                break;
            
            case MDMINI_STAR_DECENTER_END:
                break;

            case MDMINI_STAR_MOVE_LEFT_START:
                lerpTime = 0;
                lerpOfsTime = 0;
                curveTime = 0;
                lerpT = 0;
                lerpOfsT = 0;
                curveT = 0;
                pos3D[0].x = 0;
                pos3D[0].y = -65;
                pos3D[0].z = 1300;
                pos2D[1].x = 54;
                pos2D[1].y = 312;
                pos2D[1].z = 1200;
                Hu3D2Dto3D(&pos2D[1], camera->camera[MDCAMERA_FG].bit, &pos3D[1]);
                star->mode = MDMINI_STAR_MOVE_LEFT;
                break;
            
            case MDMINI_STAR_MOVE_LEFT:
                MdMathOscillate(MDMATH_OSC_90, &lerpTime, &lerpT, 30);
                MdMathOscillate(MDMATH_OSC_180, &lerpOfsTime, &lerpOfsT, 30);
                MdMathLerp(&posP->x, pos3D[0].x, pos3D[1].x, lerpT);
                MdMathLerp(&posP->y, pos3D[0].y, pos3D[1].y, lerpT);
                MdMathLerpOfs(&posP->z, pos3D[0].z, pos3D[1].z, lerpT, -1.0f*lerpOfsT);
                if(lerpTime >= 1) {
                    star->mode = MDMINI_STAR_MOVE_LEFT_END;
                }
                fn_1_15CF8(MDMODEL_STAR, MDCAMERA_FG);
                break;
            
            case MDMINI_STAR_SNAP_LEFT:
                pos2D[1].x = 64;
                pos2D[1].y = 300;
                pos2D[1].z = 1000;
                Hu3D2Dto3D(&pos2D[1], camera->camera[MDCAMERA_FG].bit, &pos3D[1]);
                *posP = pos3D[1];
                fn_1_15CF8(MDMODEL_STAR, MDCAMERA_FG);
                star->mode = MDMINI_STAR_MOVE_LEFT_END;
                break;
            
            case MDMINI_STAR_CENTER_START:
                lerpTime = 0;
                lerpOfsTime = 0;
                curveTime = 0;
                lerpT = 0;
                lerpOfsT = 0;
                curveT = 0;
                pos3D[0].x = 0;
                pos3D[0].y = -65;
                pos3D[0].z = 1300;
                pos2D[1].x = 54;
                pos2D[1].y = 312;
                pos2D[1].z = 1200;
                Hu3D2Dto3D(&pos2D[1], camera->camera[MDCAMERA_FG].bit, &pos3D[1]);
                star->mode = MDMINI_STAR_CENTER;
                break;
            
            case MDMINI_STAR_CENTER:
                MdMathOscillate(MDMATH_OSC_90_REV, &lerpTime, &lerpT, 30);
                MdMathOscillate(MDMATH_OSC_180, &lerpOfsTime, &lerpOfsT, 30);
                MdMathLerp(&posP->x, pos3D[1].x, pos3D[0].x, lerpT);
                MdMathLerp(&posP->y, pos3D[1].y, pos3D[0].y, lerpT);
                MdMathLerpOfs(&posP->z, pos3D[1].z, pos3D[0].z, lerpT, -1.0f*lerpOfsT);
                if(lerpTime >= 1) {
                    star->mode = MDMINI_STAR_CENTER_END;
                }
                fn_1_15CF8(MDMODEL_STAR, MDCAMERA_FG);
                break;
                
            
            case MDMINI_STAR_CENTER_END:
                break;
        }
        switch(star->motNext) {
            case MDMINI_STAR_MOT_NONE:
                break;
                
            case MDMINI_STAR_MOT_RESET:
                Hu3DModelAttrSet(model->mdlId[MDMODEL_STAR], HU3D_MOTATTR_LOOP);
                Hu3DModelAttrReset(model->mdlId[MDMODEL_STAR], HU3D_MOTATTR_PAUSE);
                Hu3DMotionSet(model->mdlId[MDMODEL_STAR], model->motId[MDMODEL_MOT_STAR_IDLE]);
                star->motNext = MDMINI_STAR_MOT_NONE;
                break;
            
            case MDMINI_STAR_MOT_TALK:
                Hu3DMotionShiftSet(model->mdlId[MDMODEL_STAR], model->motId[MDMODEL_MOT_STAR_TALK], 0, 32, HU3D_MOTATTR_LOOP);
                star->motNext = MDMINI_STAR_MOT_NONE;
                break;
            
            case MDMINI_STAR_MOT_IDLE:
                Hu3DMotionShiftSet(model->mdlId[MDMODEL_STAR], model->motId[MDMODEL_MOT_STAR_IDLE], 0, 32, HU3D_MOTATTR_LOOP);
                star->motNext = MDMINI_STAR_MOT_NONE;
                break;
        }
        HuPrcVSleep();
    }
}

void fn_1_1804(void)
{
    MDMINI_PLAYERSPR *playerSpr; //r31
    HuVec2f *playerNoScale; //r30
    HuVec2f *comDifScale; //r29
    int i; //r28
    MDSPRITE_WORK *sprite; //r27
    HUSPRGRPID playerNoGrpId; //r26
    HUSPRGRPID comDifGrpId; //r25
    MDCAMERA_WORK *camera; //r24
    MDMODEL_WORK *model; //r23
    
    camera = sys->camera;
    model = sys->model;
    sprite = sys->sprite;
    playerNoGrpId = sprite->grpId[MDSPR_GRP_PLAYERNO];
    comDifGrpId = sprite->grpId[MDSPR_GRP_COMDIF];
    for(playerSpr=&sys->playerSpr[0], i=0; i<4; i++, playerSpr++) {
        playerSpr->mode = MDMINI_PLAYERSPR_NONE;
    }
    while(1) {
        camera = sys->camera;
        model = sys->model;
        sprite = sys->sprite;
        playerNoGrpId = sprite->grpId[MDSPR_GRP_PLAYERNO];
        comDifGrpId = sprite->grpId[MDSPR_GRP_COMDIF];
        HuSprGrpPosSet(playerNoGrpId, 0, 0);
        HuSprGrpPosSet(comDifGrpId, 0, 0);
        for(playerSpr=&sys->playerSpr[0], i=0; i<4; i++, playerSpr++) {
            playerNoScale = MdSpriteScaleGet(sprite, MDSPR_GRP_PLAYERNO, i);
            comDifScale = MdSpriteScaleGet(sprite, MDSPR_GRP_COMDIF, i);
            switch(playerSpr->mode) {
                case MDMINI_PLAYERSPR_NONE:
                    break;
                
                case MDMINI_PLAYERSPR_GROW_START:
                    HuSprDispOn(playerNoGrpId, i);
                    HuSprDispOn(comDifGrpId, i);
                    playerNoScale->x = 0;
                    playerNoScale->y = 0;
                    comDifScale->x = 0;
                    comDifScale->y = 0;
                    playerSpr->time = 0;
                    playerSpr->ofsTime = 0;
                    playerSpr->t = 0;
                    playerSpr->ofsT = 0;
                    playerSpr->mode = MDMINI_PLAYERSPR_GROW;
                    break;
                
                case MDMINI_PLAYERSPR_GROW:
                    MdMathOscillate(MDMATH_OSC_90, &playerSpr->time, &playerSpr->t, 15);
                    MdMathOscillate(MDMATH_OSC_180, &playerSpr->ofsTime, &playerSpr->ofsT, 15);
                    MdMathLerpOfs(&playerNoScale->x, 0, 1, playerSpr->t, 0.5f*playerSpr->ofsT);
                    playerNoScale->y = playerNoScale->x;
                    MdMathLerpOfs(&comDifScale->x, 0, 1, playerSpr->t, 0.5f*playerSpr->ofsT);
                    comDifScale->y = comDifScale->x;
                    if(playerSpr->time >= 1) {
                        playerSpr->mode = MDMINI_PLAYERSPR_GROW_END;
                    }
                    break;
                
                case MDMINI_PLAYERSPR_GROW_END:
                    break;
                
                case MDMINI_PLAYERSPR_SHRINK_START:
                    playerSpr->time = 0;
                    playerSpr->ofsTime = 0;
                    playerSpr->t = 0;
                    playerSpr->ofsT = 0;
                    playerSpr->mode = MDMINI_PLAYERSPR_SHRINK;
                    break;
                
                case MDMINI_PLAYERSPR_SHRINK:
                    MdMathOscillate(MDMATH_OSC_90, &playerSpr->time, &playerSpr->t, 15);
                    MdMathLerp(&playerNoScale->x, 1, 0, playerSpr->t);
                    playerNoScale->y = playerNoScale->x;
                    MdMathLerp(&comDifScale->x, 1, 0, playerSpr->t);
                    comDifScale->y = comDifScale->x;
                    if(playerSpr->time >= 1) {
                        HuSprDispOff(playerNoGrpId, i);
                        HuSprDispOff(comDifGrpId, i);
                        playerSpr->mode = MDMINI_PLAYERSPR_SHRINK_END;
                    }
                    break;
                
                case MDMINI_PLAYERSPR_SHRINK_END:
                    break;
            }
        }
        HuPrcVSleep();
    }   
}

void fn_1_1C04(void)
{
    MDMINI_CHR *chr; //r31
    int i; //r30
    MDMODEL_WORK *model; //r29
    HuVecF *posP; //r28
    HuVecF *rotP; //r27
    MDCAMERA_WORK *camera; //r26
    MDSPRITE_WORK *sprite; //r25
    
    HuVecF *scaleP; //sp+0x8
    camera = sys->camera;
    model = sys->model;
    sprite = sys->sprite;

    for(chr=&sys->chr[0], i=0; i<10; i++, chr++) {
        chr->mode = MDMINI_CHR_NONE;
    }
    while(1) {
        camera = sys->camera;
        model = sys->model;
        sprite = sys->sprite;
        for(chr=&sys->chr[0], i=0; i<10; i++, chr++) {
            if(i != 0 && i != 1 && i != 2 && i != 5) {
                posP = model->posP[i+MDMODEL_CHR_00];
                rotP = model->rotP[i+MDMODEL_CHR_00];
                scaleP = model->scaleP[i+MDMODEL_CHR_00];
                switch(chr->mode) {
                    case MDMINI_CHR_NONE:
                        break;
                    
                    case MDMINI_CHR_INIT:
                        *posP = CharPos[i][0];
                        *rotP = CharRot[i][0];
                        chr->mode = MDMINI_CHR_NONE;
                        break;
                        
                    case MDMINI_CHR_SELECT:
                    case MDMINI_CHR_SELECT_WAIT:
                        chr->origPos = *posP;
                        chr->origRot = *rotP;
                        if(chr->mode == MDMINI_CHR_SELECT) {
                            chr->pos = CharPos[i][1];
                            chr->rot = CharRot[i][1];
                        } else if(chr->mode == MDMINI_CHR_SELECT_WAIT) {
                            chr->pos = CharPos[i][0];
                            chr->rot = CharRot[i][0];
                        }
                        chr->time = 0;
                        chr->ofsTime = 0;
                        chr->t = 0;
                        chr->ofsT = 0;
                        if(chr->mode == MDMINI_CHR_SELECT) {
                            chr->mode = MDMINI_CHR_SELECT_MOVE;
                        } else if(chr->mode == MDMINI_CHR_SELECT_WAIT) {
                            chr->mode = MDMINI_CHR_SELECT_MOVE_WAIT;
                        }
                        break;
                    
                    case MDMINI_CHR_SELECT_MOVE:
                    case MDMINI_CHR_SELECT_MOVE_WAIT:
                        if(chr->mode == MDMINI_CHR_SELECT_MOVE) {
                            MdMathOscillate(MDMATH_OSC_0, &chr->time, &chr->t, 15);
                        } else if(chr->mode == MDMINI_CHR_SELECT_MOVE_WAIT) {
                            MdMathOscillate(MDMATH_OSC_0, &chr->time, &chr->t, 15);
                        }
                        MdMathLerp(&posP->x, chr->origPos.x, chr->pos.x, chr->t);
                        MdMathLerp(&posP->y, chr->origPos.y, chr->pos.y, chr->t);
                        MdMathLerp(&posP->z, chr->origPos.z, chr->pos.z, chr->t);
                        MdMathLerp(&rotP->x, chr->origRot.x, chr->rot.x, chr->t);
                        MdMathLerp(&rotP->y, chr->origRot.y, chr->rot.y, chr->t);
                        MdMathLerp(&rotP->z, chr->origRot.z, chr->rot.z, chr->t);
                        if(chr->time >= 1) {
                            chr->mode = MDMINI_CHR_NONE;
                        }
                        break;
                }
            }
        }
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
    MDMESS *mess;

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
    MDMESS *mess;

    mess = sys->mess;
    while (1) {
        mess = sys->mess;
        MdMessMain(mess, 1);
        HuPrcVSleep();
    }
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

void fn_1_23D8(void)
{
    MDCAMERA_WORK *camera;
    MDMESS *mess;
    MDMINI_CHR *chr;
    MDMODEL_WORK *model;
    MDSPRITE_WORK *sprite;
    int i;

    camera = sys->camera;
    sprite = sys->sprite;
    model = sys->model;
    mess = sys->mess;
    chr = &sys->chr[0];
    MdSpriteDispSet(sprite, MDSPR_GRP_WIN, FALSE);
    MdSpriteDispSet(sprite, MDSPR_GRP_CURSOR_LR, FALSE);
    MdSpriteDispSet(sprite, MDSPR_GRP_CURSOR_UDLR, FALSE);
    MdSpriteDispSet(sprite, MDSPR_GRP_PLAYERNO, FALSE);
    MdSpriteDispSet(sprite, MDSPR_GRP_COMDIF, FALSE);
    MdSpriteDispSet(sprite, MDSPR_GRP_NAME, FALSE);
    Hu3DModelDispOn(model->mdlId[MDMODEL_STAR]);
    Hu3DModelDispOn(model->mdlId[MDMODEL_STAGE]);
    Hu3DModelDispOff(model->mdlId[MDMODEL_STAGE_CHR]);
    for(i=0; i<10; i++) {
        Hu3DModelDispOff(model->mdlId[i+MDMODEL_CHR_00]);
    }
    for(i=0; i<4; i++) {
        Hu3DModelDispOff(model->mdlId[i+MDMODEL_CAP_MAN_1P]);
    }
    for(i=0; i<4; i++) {
        Hu3DModelDispOff(model->mdlId[i+MDMODEL_CAP_COM_1P]);
    }
    for(i=0; i<10; i++) {
        Hu3DModelDispOff(model->mdlId[i+MDMODEL_CAP_CHR_00]);
    }
    for(i=0; i<6; i++) {
        Hu3DModelDispOff(model->mdlId[i+MDMODEL_CAP_MODE_00]);
    }
    Hu3DModelDispOff(model->mdlId[MDMODEL_CAP_WIN]);
    Hu3DModelDispOff(model->mdlId[MDMODEL_CAP_GENRE]);
    Hu3DModelDispOff(model->mdlId[MDMODEL_VS]);
    for(i=0; i<4; i++) {
        Hu3DModelDispOff(model->mdlId[i+MDMODEL_FINGER_1P]);
    }
    for(i=0; i<4; i++) {
        Hu3DModelDispOff(model->mdlId[i+MDMODEL_RING_1P]);
    }
    Hu3DModelDispOn(model->mdlId[MDMODEL_FONTBGFILTER]);
    CharMotionVoiceOnSet(0, 47, 0);
    CharMotionVoiceOnSet(1, 6, 0);
    CharMotionVoiceOnSet(2, 6, 0);
    CharMotionVoiceOnSet(3, 6, 0);
    CharMotionVoiceOnSet(4, 6, 0);
    CharMotionVoiceOnSet(5, 6, 0);
    CharMotionVoiceOnSet(6, 6, 0);
    CharMotionVoiceOnSet(7, 6, 0);
    CharMotionVoiceOnSet(8, 6, 0);
    CharMotionVoiceOnSet(9, 6, 0);
    for(i=0; i<4; i++) {
        MdRingDispSet(i, FALSE);
    }
    sys->ringProc = HuPrcChildCreate(fn_1_2000, 104, 12288, 0, sys->objman);
    for(i=0; i<10; i++) {
        chr[i].mode = MDMINI_CHR_INIT;
    }
    HuPrcSleep(2);
    MdProcCall(MDMINI_PROC_RESET, 0);
    HuPrcVSleep();
    if(sys->evtNo == 0) {
        sys->mode = 0;
        sys->flowMode = MDMINI_FLOW_PLAYER_NUM_SEL;
    } else if(sys->evtNo == 1) {
        sys->mode = 50;
        sys->flowMode = MDMINI_FLOW_MAP_SEL;
    }
}

void fn_1_27F8(void)
{
    BOOL ret;
    MDMESS *mess;
    MDMINI_STAR *star;
    MDMODEL_WORK *model;
    MDMINI_CHR *chr;

    model = sys->model;
    mess = sys->mess;
    star = sys->star;
    chr = &sys->chr[0];

    switch(sys->mode) {
        case 0:
            MdProcCall(MDMINI_PROC_RESET, 0);
            fn_1_14C2C();
            sys->prevPlayerNum = sys->playerNum;
            fn_1_17008(MDMINI_AUD_MUS_START);
            WipeColorSet(255, 255, 255);
            WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, 30);
            sys->mode = 1;
            break;

        case 1:
            if((ret = WipeCheckEnd()) == FALSE) {
                MdProcWait(MDMINI_PROC_NAME, 0);
                MdProcCall(MDMINI_PROC_RESET, 0);
                sys->mode = 2;
            }
            break;

        case 2:
            if((ret = MdProcCall(MDMINI_PROC_WIN, 0)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                MdMessWinOpen(mess, MDMESS_WIN_MAIN, MDMESS_WIN_NOSPEAKER, 0, 0, 0, 32, 372, 512, 64, 0);
                MdMessWinOpen(mess, MDMESS_WIN_HELP, MDMESS_WIN_NOSPEAKER, 0, 0, 1, 32, 340, 512, 32, 0);
                MdMessWinSpeedSet(mess, MDMESS_WIN_HELP, 0);
                MdMessWinAttrSet(mess, MDMESS_WIN_MAIN, 0x800);
                MdMessWinAttrSet(mess, MDMESS_WIN_HELP, 0x800);
                MdMessWinPad1Set(mess, MDMESS_WIN_MAIN);
                MdMessWinMesSet(mess, MDMESS_WIN_MAIN, FALSE, MESS_MGMODE_SYS_ENTER, MDMESS_LIST_END);
                fn_1_17008(MDMINI_AUD_SE_STAR_ENTER);
                sys->mode = 3;
            }
            break;

        case 3:
            if((ret = MdMessWinCheck(mess, MDMESS_WIN_MAIN)) == TRUE) {
                MdMessWinMesSet(mess, MDMESS_WIN_MAIN, FALSE, MESS_MGMODE_SYS_PLAYER_NUM_START, MDMESS_LIST_END);
                while(!MdMessWinCheck(mess, MDMESS_WIN_MAIN)) {
                    HuPrcVSleep();
                }
                MdProcWait(MDMINI_PROC_NAME, 1);
                MdProcCall(MDMINI_PROC_RESET, 0);
                star->mode = MDMINI_STAR_BACK_START;
                star->motNext = MDMINI_STAR_MOT_IDLE;
                MdMessWinHomeClear(mess, MDMESS_WIN_MAIN);
                sys->mode = 4;
            }
            break;

        case 4:
            if(star->mode == MDMINI_STAR_BACK_END) {
                sys->mode = 6;
            }
            break;

        case 5:
            if((ret = MdMessWinCheck(mess, MDMESS_WIN_MAIN)) == TRUE) {
                MdMessWinHomeClear(mess, MDMESS_WIN_MAIN);
                star->motNext = MDMINI_STAR_MOT_IDLE;
                sys->mode = 6;
            }
            break;

        case 6:
            if((ret = MdProcCall(MDMINI_PROC_CAP_PLAYER, 0)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                star->motNext = MDMINI_STAR_MOT_TALK;
                if(sys->playerNum == 3) {
                    MdMessWinMesSet(mess, MDMESS_WIN_MAIN, FALSE, MESS_MGMODE_SYS_PLAYER_NUM_4, MDMESS_LIST_END);
                } else {
                    MdMessWinMesSet(mess, MDMESS_WIN_MAIN, FALSE,
                        MESS_MGMODE_SYS_PLAYER_NUM,
                        MESS_MGMODE_SYS_PLAYER_NUM_1+sys->playerNum,
                        (MESS_MGMODE_SYS_PLAYER_NUM_1+2)-sys->playerNum,
                        MDMESS_LIST_END);
                }
                MdMessWinMesSet(mess, MDMESS_WIN_HELP, FALSE, MESS_SYSHELP_MENU_SEL, MDMESS_LIST_END);
                star->motNext = MDMINI_STAR_MOT_TALK;
                if(sys->maxPlayerNum == 1) {
                    sys->mode = 8;
                } else {
                    sys->mode = 7;
                }
            }
            break;

        case 7:
            if((ret = MdProcCall(MDMINI_PROC_CURSOR, 0)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                sys->mode = 8;
            }
            break;

        case 8:
            if(sys->maxPlayerNum > 1 && ((sys->pad->DStkDown[0] & PAD_BUTTON_LEFT) || (sys->pad->DStkDown[0] & PAD_BUTTON_RIGHT))) {
                if(sys->pad->DStkDown[0] & PAD_BUTTON_LEFT) {
                    sys->prevPlayerNum = sys->playerNum;
                    if(--sys->playerNum == -1) {
                        sys->playerNum = sys->maxPlayerNum-1;
                    }
                    sys->cursorDir = 0;
                } else if(sys->pad->DStkDown[0] & PAD_BUTTON_RIGHT) {
                    sys->prevPlayerNum = sys->playerNum;
                    sys->playerNum = (++sys->playerNum)%sys->maxPlayerNum;
                    sys->cursorDir = 1;
                }
                if(sys->playerNum == 3) {
                    MdMessWinMesSet(mess, MDMESS_WIN_MAIN, FALSE, MESS_MGMODE_SYS_PLAYER_NUM_4, MDMESS_LIST_END);
                } else {
                    MdMessWinMesSet(mess, MDMESS_WIN_MAIN, FALSE,
                        MESS_MGMODE_SYS_PLAYER_NUM,
                        MESS_MGMODE_SYS_PLAYER_NUM_1+sys->playerNum,
                        (MESS_MGMODE_SYS_PLAYER_NUM_1+2)-sys->playerNum,
                        MDMESS_LIST_END);
                }
                fn_1_17008(MDMINI_AUD_SE_CMN_MOVE);
                sys->mode = 9;
            } else if(HuPadBtnDown[0] & PAD_BUTTON_B) {
                MdMessWinMesSet(mess, MDMESS_WIN_MAIN, TRUE, MESS_MGMODE_SYS_QUIT, MDMESS_LIST_END);
                MdMessWinHomeClear(mess, MDMESS_WIN_HELP);
                fn_1_17008(MDMINI_AUD_SE_CMN_CANCEL);
                sys->mode = 10;
            } else if(HuPadBtnDown[0] & PAD_BUTTON_A) {
                fn_1_14FD0();
                MdMessWinHomeClear(mess, MDMESS_WIN_MAIN);
                MdMessWinHomeClear(mess, MDMESS_WIN_HELP);
                star->motNext = MDMINI_STAR_MOT_IDLE;
                fn_1_17008(MDMINI_AUD_SE_CMN_CONFIRM);
                if(sys->maxPlayerNum == 1) {
                    sys->mode = 15;
                } else {
                    sys->mode = 14;
                }
            }
            break;

        case 9:
            if((ret = MdProcCall(MDMINI_PROC_CAP_PLAYER, sys->cursorDir+2)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                sys->mode = 8;
            }
            break;

        case 10:
            if((ret = MdMessWinCheck(mess, MDMESS_WIN_MAIN)) == TRUE) {
                ret = MdMessWinChoiceGet(mess, MDMESS_WIN_MAIN);
                if(ret == -1 || ret == 1) {
                    if(sys->playerNum == 3) {
                        MdMessWinMesSet(mess, MDMESS_WIN_MAIN, FALSE, MESS_MGMODE_SYS_PLAYER_NUM_4, MDMESS_LIST_END);
                    } else {
                        MdMessWinMesSet(mess, MDMESS_WIN_MAIN, FALSE,
                            MESS_MGMODE_SYS_PLAYER_NUM,
                            MESS_MGMODE_SYS_PLAYER_NUM_1+sys->playerNum,
                            (MESS_MGMODE_SYS_PLAYER_NUM_1+2)-sys->playerNum,
                            MDMESS_LIST_END);
                    }
                    MdMessWinMesSet(mess, MDMESS_WIN_HELP, FALSE, MESS_SYSHELP_MENU_SEL, MDMESS_LIST_END);
                    sys->mode = 8;
                } else if(ret == 0) {
                    star->motNext = MDMINI_STAR_MOT_IDLE;
                    MdMessWinHomeClear(mess, MDMESS_WIN_MAIN);
                    MdMessWinHomeClear(mess, MDMESS_WIN_HELP);
                    sys->mode = 11;
                }
            }
            break;

        case 11:
            if((ret = MdProcCall(MDMINI_PROC_WIN, 1)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                if(sys->maxPlayerNum == 1) {
                    sys->mode = 13;
                } else {
                    sys->mode = 12;
                }
            }
            break;

        case 12:
            if((ret = MdProcCall(MDMINI_PROC_CURSOR, 1)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                sys->mode = 13;
            }
            break;

        case 13:
            if((ret = MdProcCall(MDMINI_PROC_CAP_PLAYER, 1)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                sys->loadState = MD_LOAD_STATE_SEL_LOAD;
                sys->loadDir = DATA_mdsel;
                WipeColorSet(255, 255, 255);
                WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 30);
                fn_1_17008(MDMINI_AUD_MUS_STOP);
                while(WipeCheckEnd()) {
                    HuPrcVSleep();
                }
                while(sys->loadState != MD_LOAD_STATE_DONE) {
                    HuPrcVSleep();
                }
                omOvlHisChg(0, sys->ovlHis->ovl, 0, sys->ovlHis->stat);
                sys->ovlCallMode = MD_OVLCALL_RETURN;
                sys->nextDll = DLL_mdseldll;
                sys->skipWipeF = TRUE;
                sys->exitF = TRUE;
            }
            break;

        case 14:
            if((ret = MdProcCall(MDMINI_PROC_CURSOR, 1)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                sys->mode = 15;
            }
            break;

        case 15:
            if((ret = MdProcCall(MDMINI_PROC_CAP_PLAYER, 1)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                sys->mode = 0;
                sys->flowMode = MDMINI_FLOW_CHR_SEL;
            }
            break;
    }
}

void fn_1_35A0(void)
{
    int i;
    MDMODEL_WORK *model;
    MDMINI_PLAYERSPR *playerSpr;
    MDMESS *mess;
    MDMINI_STAR *star;
    MDMINI_CHR *chr;
    MDSPRITE_WORK *sprite;
    int difMax;
    BOOL result;

    model = sys->model;
    sprite = sys->sprite;
    mess = sys->mess;
    star = sys->star;
    playerSpr = &sys->playerSpr[0];
    chr = &sys->chr[0];

    switch(sys->mode) {
        case 0:
            MdProcCall(MDMINI_PROC_RESET, 0);
            for(i=0; i<4; i++) {
                if(sys->padEnable[i] == FALSE) {
                    sys->chrSel[i] = 0;
                } else {
                    sys->chrSel[i] = i;
                }
                sys->chrSelEnd[i] = FALSE;
                sys->comDif[i] = 1;
                HuSprBankSet(sprite->grpId[MDSPR_GRP_COMDIF], (int)i, sys->comDif[i]);
            }
            sys->mode = 1;
            break;

        case 1:
            if((result = MdProcCall(MDMINI_PROC_CHR, 0)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                star->motNext = MDMINI_STAR_MOT_TALK;
                MdMessWinMesSet(mess, MDMESS_WIN_MAIN, FALSE, MESS_MGMODE_SYS_CHR_SEL_MAN, MDMESS_LIST_END);
                MdMessWinMesSet(mess, MDMESS_WIN_HELP, FALSE, MESS_SYSHELP_MENU_SEL, MDMESS_LIST_END);
                sys->mode = 2;
            }
            break;

        case 2:
            if((result = MdProcCall(MDMINI_PROC_FINGER, 0)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                sys->mode = 3;
            }
            break;

        case 3:
            if(sys->chrSelEnd[0] == FALSE &&
                (playerSpr[0].mode == MDMINI_PLAYERSPR_NONE || playerSpr[0].mode == MDMINI_PLAYERSPR_SHRINK_END) &&
                (HuPadBtnDown[0] & PAD_BUTTON_B)) {
                fn_1_17008(MDMINI_AUD_SE_CMN_CANCEL);
                star->motNext = MDMINI_STAR_MOT_IDLE;
                MdMessWinHomeClear(mess, MDMESS_WIN_MAIN);
                MdMessWinHomeClear(mess, MDMESS_WIN_HELP);
                for(i=0; i<4; i++) {
                    Hu3DModelDispOff(model->mdlId[i+MDMODEL_FINGER_1P]);
                }
                for(i=0; i<4; i++) {
                    playerSpr[i].mode = MDMINI_PLAYERSPR_NONE;
                }
                MdSpriteDispSet(sprite, MDSPR_GRP_PLAYERNO, FALSE);
                MdSpriteDispSet(sprite, MDSPR_GRP_COMDIF, FALSE);
                for(i=0; i<4; i++) {
                    if(sys->chrSelEnd[i] == TRUE) {
                        Hu3DMotionShiftSet(model->mdlId[sys->chrSel[i]+MDMODEL_CHR_00], model->motId[(sys->chrSel[i]*2)+MDMODEL_MOT_MARIO_IDLE], 0, 16, HU3D_MOTATTR_LOOP);
                        chr[sys->chrSel[i]].mode = MDMINI_CHR_SELECT_WAIT;
                    }
                }
                for(i=0; i<4; i++) {
                    MdRingDispSet(i, FALSE);
                }
                sys->mode = 4;
            } else {
                for(i=0; i<4; i++) {
                    if(sys->padEnable[i] != FALSE && (playerSpr[i].mode == MDMINI_PLAYERSPR_NONE ||
                        playerSpr[i].mode == MDMINI_PLAYERSPR_GROW_END ||
                        playerSpr[i].mode == MDMINI_PLAYERSPR_SHRINK_END)) {
                        result = fn_1_15590(i, FALSE);
                        if(result == TRUE) {
                            fn_1_17008(MDMINI_AUD_SE_CMN_MOVE);
                            fn_1_153D0();
                        } else if((HuPadBtnDown[i] & PAD_BUTTON_B) && sys->chrSelEnd[i] == TRUE) {
                            sys->chrSelEnd[i] = FALSE;
                            Hu3DModelDispOn(model->mdlId[i+MDMODEL_FINGER_1P]);
                            playerSpr[i].mode = MDMINI_PLAYERSPR_SHRINK_START;
                            Hu3DMotionShiftSet(model->mdlId[sys->chrSel[i]+MDMODEL_CHR_00], model->motId[(sys->chrSel[i]*2)+MDMODEL_MOT_MARIO_IDLE], 0, 16, HU3D_MOTATTR_LOOP);
                            chr[sys->chrSel[i]].mode = MDMINI_CHR_SELECT_WAIT;
                            fn_1_17008(MDMINI_AUD_SE_CMN_CANCEL);
                            MdRingModeSet(i, MDMINI_RING_MODE_ERASE);
                        } else if((HuPadBtnDown[i] & PAD_BUTTON_A) && sys->chrSelEnd[i] == FALSE) {
                            sys->chrSelEnd[i] = TRUE;
                            Hu3DModelDispOff(model->mdlId[i+MDMODEL_FINGER_1P]);
                            playerSpr[i].mode = MDMINI_PLAYERSPR_GROW_START;
                            Hu3DMotionShiftSet(model->mdlId[sys->chrSel[i]+MDMODEL_CHR_00], model->motId[(sys->chrSel[i]*2)+MDMODEL_MOT_MARIO_WIN], 0, 8, HU3D_MOTATTR_NONE);
                            chr[sys->chrSel[i]].mode = MDMINI_CHR_SELECT;
                            fn_1_17008(MDMINI_AUD_SE_CMN_CHRSEL);
                            fn_1_17008(sys->chrSel[i]+MDMINI_AUD_SE_CHR_MARIO);
                            MdRingModeSet(i, MDMINI_RING_MODE_SET);
                            MdRingOfsSet(i, 0, 0, 0);
                            MdRingMdlSet(i, sys->chrSel[i]+MDMODEL_CAP_CHR_00);
                            MdRingScaleSet(i, 1.0);
                        }
                    }
                }
                if(((sys->chrSelEnd[0] == TRUE && playerSpr[0].mode == MDMINI_PLAYERSPR_GROW_END) || sys->padEnable[0] == FALSE) &&
                    ((sys->chrSelEnd[1] == TRUE && playerSpr[1].mode == MDMINI_PLAYERSPR_GROW_END) || sys->padEnable[1] == FALSE) &&
                    ((sys->chrSelEnd[2] == TRUE && playerSpr[2].mode == MDMINI_PLAYERSPR_GROW_END) || sys->padEnable[2] == FALSE) &&
                    ((sys->chrSelEnd[3] == TRUE && playerSpr[3].mode == MDMINI_PLAYERSPR_GROW_END) || sys->padEnable[3] == FALSE)) {
                    if(sys->playerNum == 3) {
                        star->motNext = MDMINI_STAR_MOT_IDLE;
                        MdMessWinHomeClear(mess, MDMESS_WIN_MAIN);
                        MdMessWinHomeClear(mess, MDMESS_WIN_HELP);
                        sys->mode = 14;
                    } else {
                        sys->selComNo = 0;
                        fn_1_15BF4();
                        fn_1_153D0();
                        Hu3DModelDispOn(model->mdlId[sys->comPlayerNo[sys->selComNo]+MDMODEL_FINGER_1P]);
                        model->scaleP[sys->comPlayerNo[sys->selComNo]+MDMODEL_FINGER_1P]->x = 4;
                        model->scaleP[sys->comPlayerNo[sys->selComNo]+MDMODEL_FINGER_1P]->y = 4;
                        model->scaleP[sys->comPlayerNo[sys->selComNo]+MDMODEL_FINGER_1P]->z = 4;
                        MdMessWinMesSet(mess, MDMESS_WIN_MAIN, FALSE,
                            MESS_MGMODE_SYS_CHR_SEL_COM,
                            (MESS_MGMODE_SYS_PLAYER_NUM_1+2)-sys->playerNum+sys->selComNo,
                            MDMESS_LIST_END);
                        sys->mode = 6;
                    }
                }
            }
            break;

        case 4:
            for(i=0; i<4; i++) {
                Hu3DModelDispOff(model->mdlId[i+MDMODEL_FINGER_1P]);
            }
            MdSpriteDispSet(sprite, MDSPR_GRP_PLAYERNO, FALSE);
            sys->mode = 5;
            break;

        case 5:
            if((result = MdProcCall(MDMINI_PROC_CHR, 1)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                fn_1_14C2C();
                sys->prevPlayerNum = sys->playerNum;
                sys->mode = 6;
                sys->flowMode = MDMINI_FLOW_PLAYER_NUM_SEL;
            }
            break;

        case 6:
            if(sys->selComNo == 0 && (HuPadBtnDown[0] & PAD_BUTTON_B)) {
                for(i=0; i<4; i++) {
                    if(sys->chrSelEnd[i] == TRUE) {
                        Hu3DMotionShiftSet(model->mdlId[sys->chrSel[i]+MDMODEL_CHR_00], model->motId[(sys->chrSel[i]*2)+MDMODEL_MOT_MARIO_IDLE], 0, 16, HU3D_MOTATTR_LOOP);
                        chr[sys->chrSel[i]].mode = MDMINI_CHR_SELECT_WAIT;
                        MdRingModeSet(i, MDMINI_RING_MODE_ERASE);
                        playerSpr[i].mode = MDMINI_PLAYERSPR_SHRINK_START;
                        sys->chrSelEnd[i] = FALSE;
                    }
                }
                Hu3DModelDispOff(model->mdlId[sys->comPlayerNo[sys->selComNo]+MDMODEL_FINGER_1P]);
                MdMessWinMesSet(mess, MDMESS_WIN_MAIN, FALSE, MESS_MGMODE_SYS_CHR_SEL_MAN, MDMESS_LIST_END);
                fn_1_17008(MDMINI_AUD_SE_CMN_CANCEL);
                sys->mode = 7;
            } else {
                result = fn_1_15590(sys->comPlayerNo[sys->selComNo], TRUE);
                if(result == TRUE) {
                    fn_1_17008(MDMINI_AUD_SE_CMN_MOVE);
                    fn_1_153D0();
                } else if((HuPadBtnDown[0] & PAD_BUTTON_B) && sys->selComNo > 0) {
                    Hu3DModelDispOff(model->mdlId[sys->comPlayerNo[sys->selComNo]+MDMODEL_FINGER_1P]);
                    sys->selComNo--;
                    Hu3DMotionShiftSet(model->mdlId[sys->chrSel[sys->comPlayerNo[sys->selComNo]]+MDMODEL_CHR_00], model->motId[(sys->chrSel[sys->comPlayerNo[sys->selComNo]]*2)+MDMODEL_MOT_MARIO_IDLE], 0, 16, HU3D_MOTATTR_LOOP);
                    chr[sys->chrSel[sys->comPlayerNo[sys->selComNo]]].mode = MDMINI_CHR_SELECT_WAIT;
                    fn_1_17008(MDMINI_AUD_SE_CMN_CANCEL);
                    MdRingModeSet(sys->comPlayerNo[sys->selComNo], MDMINI_RING_MODE_ERASE);
                    playerSpr[sys->comPlayerNo[sys->selComNo]].mode = MDMINI_PLAYERSPR_SHRINK_START;
                    MdMessWinMesSet(mess, MDMESS_WIN_MAIN, FALSE,
                        MESS_MGMODE_SYS_CHR_SEL_COM,
                        (MESS_MGMODE_SYS_PLAYER_NUM_1+2)-sys->playerNum-sys->selComNo,
                        MDMESS_LIST_END);
                    sys->mode = 15;
                } else if(HuPadBtnDown[0] & PAD_BUTTON_A) {
                    playerSpr[sys->comPlayerNo[sys->selComNo]].mode = MDMINI_PLAYERSPR_GROW_START;
                    MdMessWinMesSet(mess, MDMESS_WIN_MAIN, FALSE, MESS_MGMODE_SYS_DIF_SEL, MDMESS_LIST_END);
                    Hu3DModelDispOff(model->mdlId[sys->comPlayerNo[sys->selComNo]+MDMODEL_FINGER_1P]);
                    Hu3DMotionShiftSet(model->mdlId[sys->chrSel[sys->comPlayerNo[sys->selComNo]]+MDMODEL_CHR_00], model->motId[(sys->chrSel[sys->comPlayerNo[sys->selComNo]]*2)+MDMODEL_MOT_MARIO_WIN], 0, 8, HU3D_MOTATTR_NONE);
                    chr[sys->chrSel[sys->comPlayerNo[sys->selComNo]]].mode = MDMINI_CHR_SELECT;
                    MdRingModeSet(sys->comPlayerNo[sys->selComNo], MDMINI_RING_MODE_SET);
                    MdRingOfsSet(sys->comPlayerNo[sys->selComNo], 0, 0, 0);
                    MdRingMdlSet(sys->comPlayerNo[sys->selComNo], sys->chrSel[sys->comPlayerNo[sys->selComNo]]+MDMODEL_CAP_CHR_00);
                    MdRingScaleSet(sys->comPlayerNo[sys->selComNo], 1.0);
                    fn_1_17008(MDMINI_AUD_SE_CMN_CHRSEL);
                    fn_1_17008(sys->chrSel[sys->comPlayerNo[sys->selComNo]]+MDMINI_AUD_SE_CHR_MARIO);
                    sys->mode = 8;
                }
            }
            break;

        case 7:
            if(playerSpr[0].mode == MDMINI_PLAYERSPR_SHRINK_END) {
                sys->mode = 2;
            }
            break;

        case 8:
            if(playerSpr[sys->comPlayerNo[sys->selComNo]].mode == MDMINI_PLAYERSPR_GROW_END) {
                sys->mode = 9;
            }
            break;

        case 9:
            if((result = MdProcCall(MDMINI_PROC_CURSOR, 4)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                HuSprDispOff(sprite->grpId[MDSPR_GRP_PLAYERNO], (int)sys->comPlayerNo[sys->selComNo]);
                HuSprDispOn(sprite->grpId[MDSPR_GRP_COMDIF], (int)sys->comPlayerNo[sys->selComNo]);
                fn_1_153D0();
                sys->mode = 10;
            }
            break;

        case 10:
            if(GwCommon.veryHardUnlock == TRUE) {
                difMax = 4;
            } else {
                difMax = 3;
            }
            if((sys->pad->DStkDown[0] & PAD_BUTTON_LEFT) || (sys->pad->DStkDown[0] & PAD_BUTTON_RIGHT)) {
                if(sys->pad->DStkDown[0] & PAD_BUTTON_LEFT) {
                    if(--sys->comDif[sys->comPlayerNo[sys->selComNo]] == -1) {
                        sys->comDif[sys->comPlayerNo[sys->selComNo]] = difMax-1;
                    }
                    sys->cursorDir = 0;
                } else if(sys->pad->DStkDown[0] & PAD_BUTTON_RIGHT) {
                    sys->comDif[sys->comPlayerNo[sys->selComNo]] = ++sys->comDif[sys->comPlayerNo[sys->selComNo]]%difMax;
                    sys->cursorDir = 1;
                }
                fn_1_17008(MDMINI_AUD_SE_CMN_MOVE);
                fn_1_153D0();
                HuSprBankSet(sprite->grpId[MDSPR_GRP_COMDIF], (int)sys->comPlayerNo[sys->selComNo], sys->comDif[sys->comPlayerNo[sys->selComNo]]);
                sys->mode = 11;
            } else if(HuPadBtnDown[0] & PAD_BUTTON_B) {
                playerSpr[sys->comPlayerNo[sys->selComNo]].mode = MDMINI_PLAYERSPR_SHRINK_START;
                Hu3DModelDispOn(model->mdlId[sys->comPlayerNo[sys->selComNo]+MDMODEL_FINGER_1P]);
                Hu3DMotionShiftSet(model->mdlId[sys->chrSel[sys->comPlayerNo[sys->selComNo]]+MDMODEL_CHR_00], model->motId[(sys->chrSel[sys->comPlayerNo[sys->selComNo]]*2)+MDMODEL_MOT_MARIO_IDLE], 0, 16, HU3D_MOTATTR_LOOP);
                chr[sys->chrSel[sys->comPlayerNo[sys->selComNo]]].mode = MDMINI_CHR_SELECT_WAIT;
                MdRingModeSet(sys->comPlayerNo[sys->selComNo], MDMINI_RING_MODE_ERASE);
                MdMessWinMesSet(mess, MDMESS_WIN_MAIN, FALSE,
                    MESS_MGMODE_SYS_CHR_SEL_COM,
                    (MESS_MGMODE_SYS_PLAYER_NUM_1+2)-sys->playerNum-sys->selComNo,
                    MDMESS_LIST_END);
                fn_1_17008(MDMINI_AUD_SE_CMN_CANCEL);
                sys->mode = 12;
            } else if(HuPadBtnDown[0] & PAD_BUTTON_A) {
                sys->chrSelEnd[sys->comPlayerNo[sys->selComNo]] = TRUE;
                if(sys->selComNo+1 < 3-sys->playerNum) {
                    MdMessWinMesSet(mess, MDMESS_WIN_MAIN, FALSE,
                        MESS_MGMODE_SYS_CHR_SEL_COM,
                        (MESS_MGMODE_SYS_PLAYER_NUM_1+1)-sys->playerNum-sys->selComNo,
                        MDMESS_LIST_END);
                    fn_1_17008(MDMINI_AUD_SE_CMN_CHRSEL);
                } else {
                    fn_1_17008(MDMINI_AUD_SE_CMN_CONFIRM);
                    MdMessWinHomeClear(mess, MDMESS_WIN_MAIN);
                    MdMessWinHomeClear(mess, MDMESS_WIN_HELP);
                    star->motNext = MDMINI_STAR_MOT_IDLE;
                }
                sys->mode = 13;
            }
            break;

        case 11:
            if((result = MdProcCall(MDMINI_PROC_CURSOR, sys->cursorDir+6)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                sys->mode = 10;
            }
            break;

        case 12:
            if((result = MdProcCall(MDMINI_PROC_CURSOR, 5)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                Hu3DModelDispOn(model->mdlId[sys->comPlayerNo[sys->selComNo]+MDMODEL_FINGER_1P]);
                sys->mode = 6;
            }
            break;

        case 13:
            if((result = MdProcCall(MDMINI_PROC_CURSOR, 5)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                sys->selComNo++;
                if(sys->selComNo < 3-sys->playerNum) {
                    fn_1_15BF4();
                    fn_1_153D0();
                    Hu3DModelDispOn(model->mdlId[sys->comPlayerNo[sys->selComNo]+MDMODEL_FINGER_1P]);
                    model->scaleP[sys->comPlayerNo[sys->selComNo]+MDMODEL_FINGER_1P]->x = 4;
                    model->scaleP[sys->comPlayerNo[sys->selComNo]+MDMODEL_FINGER_1P]->y = 4;
                    model->scaleP[sys->comPlayerNo[sys->selComNo]+MDMODEL_FINGER_1P]->z = 4;
                    sys->mode = 6;
                } else if(sys->selComNo >= 3-sys->playerNum) {
                    sys->mode = 14;
                }
            }
            break;

        case 14:
            if((result = MdProcCall(MDMINI_PROC_CHR, 2)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                sys->mode = 0;
                sys->flowMode = MDMINI_FLOW_MAP_SEL;
            }
            break;

        case 15:
            if(playerSpr[sys->comPlayerNo[sys->selComNo]].mode == MDMINI_PLAYERSPR_SHRINK_END) {
                Hu3DModelDispOn(model->mdlId[sys->comPlayerNo[sys->selComNo]+MDMODEL_FINGER_1P]);
                sys->chrSelEnd[sys->comPlayerNo[sys->selComNo]] = FALSE;
                fn_1_153D0();
                sys->mode = 6;
            }
            break;
    }
}

void fn_1_5558(void)
{
    int i;
    MDMESS *mess;
    MDMODEL_WORK *model;
    BOOL ret;
    MDMINI_STAR *star;
    MDSPRITE_WORK *sprite;
    HuVec2f *scaleP;
    MDMINI_PLAYERSPR *playerSpr;
    MDMINI_CHR *chr;
    float maxTime;

    model = sys->model;
    sprite = sys->sprite;
    mess = sys->mess;
    star = sys->star;
    playerSpr = &sys->playerSpr[0];
    chr = &sys->chr[0];

    switch(sys->mode) {
        case 0:
            MdProcCall(MDMINI_PROC_RESET, 0);
            sys->selNoNow = 0;
            fn_1_16358();
            sys->mode = 1;
            break;

        case 1:
            if((ret = MdProcCall(MDMINI_PROC_6, 0)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                sys->mode = 2;
            }
            break;

        case 2:
            if((ret = MdProcCall(MDMINI_PROC_CURSOR, 8)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                star->motNext = MDMINI_STAR_MOT_TALK;
                MdMessWinMesSet(mess, MDMESS_WIN_MAIN, FALSE, MESS_MGMODE_SYS_MGFREE+sys->selNoNow, MDMESS_LIST_END);
                MdMessWinMesSet(mess, MDMESS_WIN_HELP, FALSE, MESS_SYSHELP_OPTION_ENTER, MDMESS_LIST_END);
                sys->mode = 3;
            }
            break;

        case 3:
            if((sys->pad->DStkDown[0] & PAD_BUTTON_LEFT) || (sys->pad->DStkDown[0] & PAD_BUTTON_RIGHT)) {
                sys->selNoPrev = sys->selNoNow;
                if(sys->pad->DStkDown[0] & PAD_BUTTON_RIGHT) {
                    sys->selNoNow = ++sys->selNoNow%6;
                    sys->cursorDir = 0;
                } else if(sys->pad->DStkDown[0] & PAD_BUTTON_LEFT) {
                    if(--sys->selNoNow == -1) {
                        sys->selNoNow = 5;
                    }
                    sys->cursorDir = 1;
                }
                MdMessWinMesSet(mess, MDMESS_WIN_MAIN, FALSE, MESS_MGMODE_SYS_MGFREE+sys->selNoNow, MDMESS_LIST_END);
                fn_1_17008(MDMINI_AUD_SE_CMN_MOVE);
                sys->mode = 4;
            } else if(HuPadBtnDown[0] & PAD_BUTTON_B) {
                star->motNext = MDMINI_STAR_MOT_IDLE;
                MdMessWinHomeClear(mess, MDMESS_WIN_MAIN);
                MdMessWinHomeClear(mess, MDMESS_WIN_HELP);
                fn_1_17008(MDMINI_AUD_SE_CMN_CANCEL);
                sys->mode = 5;
            } else if(HuPadBtnDown[0] & PAD_BUTTON_A) {
                if(sys->mgTypeF[sys->selNoNow] == TRUE) {
                    star->motNext = MDMINI_STAR_MOT_IDLE;
                    MdMessWinHomeClear(mess, MDMESS_WIN_MAIN);
                    MdMessWinHomeClear(mess, MDMESS_WIN_HELP);
                    fn_1_17008(MDMINI_AUD_SE_CMN_CONFIRM);
                    sys->mode = 8;
                } else {
                    fn_1_17008(MDMINI_AUD_SE_CMN_SELECT);
                }
            } else if(HuPadBtnDown[0] & PAD_BUTTON_X) {
                fn_1_17008(MDMINI_AUD_SE_CMN_CANCEL);
                MdMessWinHomeClear(mess, MDMESS_WIN_HELP);
                MdMessWinMesSet(mess, MDMESS_WIN_MAIN, TRUE, MESS_MGMODE_SYS_QUIT, MDMESS_LIST_END);
                while(!MdMessWinCheck(mess, MDMESS_WIN_MAIN)) {
                    HuPrcVSleep();
                }
                ret = MdMessWinChoiceGet(mess, MDMESS_WIN_MAIN);
                if(ret == -1 || ret == 1) {
                    MdMessWinHomeClear(mess, MDMESS_WIN_MAIN);
                    MdMessWinMesSet(mess, MDMESS_WIN_MAIN, FALSE, MESS_MGMODE_SYS_MGFREE+sys->selNoNow, MDMESS_LIST_END);
                    MdMessWinMesSet(mess, MDMESS_WIN_HELP, FALSE, MESS_SYSHELP_OPTION_ENTER, MDMESS_LIST_END);
                } else if(ret == 0) {
                    star->motNext = MDMINI_STAR_MOT_IDLE;
                    MdMessWinHomeClear(mess, MDMESS_WIN_MAIN);
                    MdMessWinHomeClear(mess, MDMESS_WIN_HELP);
                    MdProcWait(MDMINI_PROC_CURSOR, 9);
                    MdProcCall(MDMINI_PROC_RESET, 0);
                    while(MdProcCall(MDMINI_PROC_WIN, 1) == FALSE) {
                        HuPrcVSleep();
                    }
                    MdProcCall(MDMINI_PROC_RESET, 0);
                    sys->loadState = MD_LOAD_STATE_SEL_LOAD;
                    sys->loadDir = DATA_mdsel;
                    WipeColorSet(255, 255, 255);
                    WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 30);
                    fn_1_17008(MDMINI_AUD_MUS_STOP);
                    while(WipeCheckEnd()) {
                        HuPrcVSleep();
                    }
                    while(sys->loadState != MD_LOAD_STATE_DONE) {
                        HuPrcVSleep();
                    }
                    omOvlHisChg(0, sys->ovlHis->ovl, 0, sys->ovlHis->stat);
                    sys->ovlCallMode = MD_OVLCALL_RETURN;
                    sys->nextDll = DLL_mdseldll;
                    sys->skipWipeF = TRUE;
                    sys->exitF = TRUE;
                }
            }
            break;

        case 4:
            if((ret = MdProcCall(MDMINI_PROC_6, 1)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                sys->mode = 3;
            }
            break;

        case 5:
            if((ret = MdProcCall(MDMINI_PROC_CURSOR, 9)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                sys->mode = 6;
            }
            break;

        case 6:
            if((ret = MdProcCall(MDMINI_PROC_6, 2)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                sys->mode = 7;
            }
            break;

        case 7:
            if((ret = MdProcCall(MDMINI_PROC_CHR, 3)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                if(sys->playerNum == 3) {
                    sys->selComNo = 0;
                    for(i=0; i<4; i++) {
                        sys->chrSelEnd[i] = FALSE;
                        playerSpr[i].mode = MDMINI_PLAYERSPR_SHRINK_START;
                        Hu3DMotionShiftSet(model->mdlId[sys->chrSel[i]+MDMODEL_CHR_00], model->motId[(sys->chrSel[i]*2)+MDMODEL_MOT_MARIO_IDLE], 0, 16, HU3D_MOTATTR_LOOP);
                        chr[sys->chrSel[i]].mode = MDMINI_CHR_SELECT_WAIT;
                        MdRingModeSet(i, MDMINI_RING_MODE_ERASE);
                    }
                    MdMessWinMesSet(mess, MDMESS_WIN_MAIN, FALSE, MESS_MGMODE_SYS_CHR_SEL_MAN, MDMESS_LIST_END);
                    MdMessWinMesSet(mess, MDMESS_WIN_HELP, FALSE, MESS_SYSHELP_MENU_SEL, MDMESS_LIST_END);
                    sys->mode = 2;
                    sys->flowMode = MDMINI_FLOW_CHR_SEL;
                } else {
                    sys->selComNo = 0;
                    for(i=0; i<3; i++) {
                        if(sys->comPlayerNo[i] != -1) {
                            sys->chrSelEnd[sys->comPlayerNo[i]] = FALSE;
                            playerSpr[sys->comPlayerNo[i]].mode = MDMINI_PLAYERSPR_SHRINK_START;
                            Hu3DMotionShiftSet(model->mdlId[sys->chrSel[sys->comPlayerNo[i]]+MDMODEL_CHR_00], model->motId[(sys->chrSel[sys->comPlayerNo[i]]*2)+MDMODEL_MOT_MARIO_IDLE], 0, 16, HU3D_MOTATTR_LOOP);
                            chr[sys->chrSel[sys->comPlayerNo[i]]].mode = MDMINI_CHR_SELECT_WAIT;
                            MdRingModeSet(sys->comPlayerNo[i], MDMINI_RING_MODE_ERASE);
                        }
                    }
                    fn_1_15BF4();
                    fn_1_153D0();
                    Hu3DModelDispOn(model->mdlId[sys->comPlayerNo[sys->selComNo]+MDMODEL_FINGER_1P]);
                    model->scaleP[sys->comPlayerNo[sys->selComNo]+MDMODEL_FINGER_1P]->x = 4;
                    model->scaleP[sys->comPlayerNo[sys->selComNo]+MDMODEL_FINGER_1P]->y = 4;
                    model->scaleP[sys->comPlayerNo[sys->selComNo]+MDMODEL_FINGER_1P]->z = 4;
                    MdMessWinMesSet(mess, MDMESS_WIN_MAIN, FALSE, MESS_MGMODE_SYS_CHR_SEL_COM, (MESS_MGMODE_SYS_PLAYER_NUM_1+2)-sys->playerNum+sys->selComNo, MDMESS_LIST_END);
                    MdMessWinMesSet(mess, MDMESS_WIN_HELP, FALSE, MESS_SYSHELP_MENU_SEL, MDMESS_LIST_END);
                    sys->mode = 6;
                    sys->flowMode = MDMINI_FLOW_CHR_SEL;
                }
            }
            break;

        case 8:
            if((ret = MdProcCall(MDMINI_PROC_CURSOR, 9)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                sys->mode = 9;
            }
            break;

        case 9:
            if((ret = MdProcCall(MDMINI_PROC_6, 3)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                if(sys->selNoNow == 1) {
                    sys->mode = 0;
                    sys->flowMode = MDMINI_FLOW_TURN_SEL;
                } else {
                    MdMessWinMesSet(mess, MDMESS_WIN_MAIN, TRUE, MESS_MGMODE_SYS_CONFIRM, MDMESS_LIST_END);
                    fn_1_17008(MDMINI_AUD_SE_STAR_CONFIRM);
                    sys->mode = 10;
                }
            }
            break;

        case 10:
            if(MdMessWinCheck(mess, MDMESS_WIN_MAIN) == TRUE) {
                ret = MdMessWinChoiceGet(mess, MDMESS_WIN_MAIN);
                if(ret == -1 || ret == 1) {
                    MdMessWinMesSet(mess, MDMESS_WIN_MAIN, TRUE, MESS_MGMODE_SYS_RESET, MDMESS_LIST_END);
                    sys->mode = 11;
                } else if(ret == 0) {
                    MdProcCall(MDMINI_PROC_RESET, 0);
                    fn_1_16978();
                    if(sys->selNoNow == 2) {
                        _SetFlag(FLAG_MG_CIRCUIT);
                        MBPartySaveInit(8);
                    }
                    if(sys->selNoNow == 2) {
                        sys->loadState = MD_LOAD_STATE_BOARD_LOAD;
                    } else {
                        sys->loadState = MD_LOAD_STATE_LOAD;
                        sys->loadDir = LoadDirTbl[sys->selNoNow];
                    }
                    MdMessWinMesSet(mess, MDMESS_WIN_MAIN, FALSE, MESS_MGMODE_SYS_MG_START, MDMESS_LIST_END);
                    while(!MdMessWinCheck(mess, MDMESS_WIN_MAIN)) {
                        HuPrcVSleep();
                    }
                    MdProcWait(MDMINI_PROC_13, 1);
                    WipeColorSet(255, 255, 255);
                    WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 55);
                    fn_1_17008(MDMINI_AUD_MUS_STOP);
                    fn_1_17008(10);
                    while(WipeCheckEnd()) {
                        MdProcCall(MDMINI_PROC_13, 1);
                        HuPrcVSleep();
                    }
                    while(sys->loadState != MD_LOAD_STATE_DONE) {
                        HuPrcVSleep();
                    }
                    omOvlHisChg(0, sys->ovlHis->ovl, 1, sys->ovlHis->stat);
                    sys->ovlCallMode = MD_OVLCALL_CALL;
                    sys->nextDll = NextDllTbl[sys->selNoNow];
                    sys->skipWipeF = TRUE;
                    sys->exitF = TRUE;
                }
            }
            break;

        case 11:
            if(MdMessWinCheck(mess, MDMESS_WIN_MAIN) == TRUE) {
                ret = MdMessWinChoiceGet(mess, MDMESS_WIN_MAIN);
                if(ret == -1 || ret == 1) {
                    MdMessWinHomeClear(mess, MDMESS_WIN_MAIN);
                    star->motNext = MDMINI_STAR_MOT_IDLE;
                    sys->mode = 12;
                } else if(ret == 0) {
                    MdMessWinHomeClear(mess, MDMESS_WIN_MAIN);
                    star->mode = MDMINI_STAR_CENTER_START;
                    star->motNext = MDMINI_STAR_MOT_IDLE;
                    for(i=0; i<4; i++) {
                        sys->chrSelEnd[i] = FALSE;
                        playerSpr[i].mode = MDMINI_PLAYERSPR_SHRINK_START;
                        Hu3DMotionShiftSet(model->mdlId[sys->chrSel[i]+MDMODEL_CHR_00], model->motId[(sys->chrSel[i]*2)+MDMODEL_MOT_MARIO_IDLE], 0, 16, HU3D_MOTATTR_LOOP);
                        chr[sys->chrSel[i]].mode = MDMINI_CHR_SELECT_WAIT;
                        MdRingModeSet(i, MDMINI_RING_MODE_ERASE);
                    }
                    sys->mode = 13;
                }
            }
            break;

        case 12:
            if((ret = MdProcCall(MDMINI_PROC_6, 4)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                sys->mode = 2;
            }
            break;

        case 13:
            if((ret = MdProcCall(MDMINI_PROC_10, 0)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                fn_1_14C2C();
                sys->prevPlayerNum = sys->playerNum;
                sys->mode = 6;
                sys->flowMode = MDMINI_FLOW_PLAYER_NUM_SEL;
            }
            break;

        case 50:
            MdProcCall(MDMINI_PROC_RESET, 0);
            fn_1_14E24();
            fn_1_14FD0();
            fn_1_152D4();
            MdProcCall(MDMINI_PROC_CHR, 4);
            MdProcCall(MDMINI_PROC_RESET, 0);
            for(i=0; i<4; i++) {
                maxTime = Hu3DMotionMotionMaxTimeGet(model->motId[(sys->chrSel[i]*2)+MDMODEL_MOT_MARIO_WIN]);
                Hu3DModelAttrReset(model->mdlId[sys->chrSel[i]+MDMODEL_CHR_00], HU3D_MOTATTR_LOOP);
                Hu3DModelAttrSet(model->mdlId[sys->chrSel[i]+MDMODEL_CHR_00], HU3D_MOTATTR_PAUSE);
                Hu3DMotionSpeedSet(model->mdlId[sys->chrSel[i]+MDMODEL_CHR_00], 1);
                Hu3DMotionSet(model->mdlId[sys->chrSel[i]+MDMODEL_CHR_00], model->motId[(sys->chrSel[i]*2)+MDMODEL_MOT_MARIO_WIN]);
                Hu3DMotionTimeSet(model->mdlId[sys->chrSel[i]+MDMODEL_CHR_00], maxTime);
                chr[sys->chrSel[i]].mode = MDMINI_CHR_SELECT;
            }
            for(i=0; i<4; i++) {
                MdRingModeSet(i, MDMINI_RING_MODE_SET);
                MdRingOfsSet(i, 0, 0, 0);
                MdRingMdlSet(i, sys->chrSel[i]+MDMODEL_CAP_CHR_00);
                MdRingScaleSet(i, 1.0);
            }
            for(i=0; i<4; i++) {
                playerSpr[i].mode = MDMINI_PLAYERSPR_GROW_END;
                HuSprDispOn(sprite->grpId[MDSPR_GRP_PLAYERNO], (s16)i);
                scaleP = MdSpriteScaleGet(sprite, MDSPR_GRP_PLAYERNO, i);
                scaleP->x = scaleP->y = 1;
            }
            for(i=0; i<3; i++) {
                if(sys->comPlayerNo[i] != -1) {
                    HuSprDispOff(sprite->grpId[MDSPR_GRP_PLAYERNO], (int)sys->comPlayerNo[i]);
                    HuSprDispOn(sprite->grpId[MDSPR_GRP_COMDIF], (int)sys->comPlayerNo[i]);
                    scaleP = MdSpriteScaleGet(sprite, MDSPR_GRP_COMDIF, sys->comPlayerNo[i]);
                    scaleP->x = scaleP->y = 1;
                    HuSprBankSet(sprite->grpId[MDSPR_GRP_COMDIF], (int)sys->comPlayerNo[i], sys->comDif[sys->comPlayerNo[i]]);
                }
            }
            fn_1_153D0();
            star->mode = MDMINI_STAR_SNAP_LEFT;
            star->motNext = MDMINI_STAR_MOT_RESET;
            fn_1_17008(MDMINI_AUD_MUS_START);
            WipeColorSet(255, 255, 255);
            WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, 30);
            sys->mode = 51;
            break;

        case 51:
            if((ret = WipeCheckEnd()) == FALSE) {
                sys->mode = 52;
            }
            break;

        case 52:
            if((ret = MdProcCall(MDMINI_PROC_WIN, 0)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                MdMessWinOpen(mess, MDMESS_WIN_MAIN, MDMESS_WIN_NOSPEAKER, 0, 0, 0, 32, 372, 512, 64, 0);
                MdMessWinOpen(mess, MDMESS_WIN_HELP, MDMESS_WIN_NOSPEAKER, 0, 0, 1, 32, 340, 512, 32, 0);
                MdMessWinSpeedSet(mess, MDMESS_WIN_HELP, 0);
                MdMessWinAttrSet(mess, MDMESS_WIN_MAIN, 0x800);
                MdMessWinAttrSet(mess, MDMESS_WIN_HELP, 0x800);
                MdMessWinPad1Set(mess, MDMESS_WIN_MAIN);
                sys->selNoNow = 0;
                fn_1_16358();
                sys->mode = 53;
            }
            break;

        case 53:
            if((ret = MdProcCall(MDMINI_PROC_6, 0)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                sys->mode = 54;
            }
            break;

        case 54:
            if((ret = MdProcCall(MDMINI_PROC_CURSOR, 8)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                MdMessWinMesSet(mess, MDMESS_WIN_MAIN, FALSE, MESS_MGMODE_SYS_MGFREE+sys->selNoNow, MDMESS_LIST_END);
                MdMessWinMesSet(mess, MDMESS_WIN_HELP, FALSE, MESS_SYSHELP_OPTION_ENTER, MDMESS_LIST_END);
                star->motNext = MDMINI_STAR_MOT_TALK;
                sys->mode = 3;
            }
            break;
    }
}

void fn_1_6FB8(void)
{
    MDMINI_STAR *star;
    MDMODEL_WORK *model;
    BOOL ret;
    MDMESS *mess;

    MDMINI_CHR *chr;

    model = sys->model;
    mess = sys->mess;
    star = sys->star;
    chr = &sys->chr[0];

    switch(sys->mode) {
        case 0:
            MdProcCall(MDMINI_PROC_RESET, 0);
            sys->scoreNum = 0;
            Hu3DAnmNoSet(model->animId[MDMODEL_ANIM_CAP_TURN], sys->scoreNum);
            sys->mode = 1;
            break;

        case 1:
            if((ret = MdProcCall(MDMINI_PROC_CAP_WIN, 0)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                sys->mode = 2;
            }
            break;

        case 2:
            if((ret = MdProcCall(MDMINI_PROC_CURSOR, 12)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                star->motNext = MDMINI_STAR_MOT_TALK;
                MdMessWinMesSet(mess, MDMESS_WIN_MAIN, FALSE, MESS_MGMODE_SYS_MGMATCH_MODE_3+sys->scoreNum, MDMESS_LIST_END);
                MdMessWinMesSet(mess, MDMESS_WIN_HELP, FALSE, MESS_SYSHELP_MENU_SEL, MDMESS_LIST_END);
                sys->mode = 3;
            }
            break;

        case 3:
            if((sys->pad->DStkDown[0] & PAD_BUTTON_LEFT) || (sys->pad->DStkDown[0] & PAD_BUTTON_RIGHT)) {
                if(sys->pad->DStkDown[0] & PAD_BUTTON_LEFT) {
                    if(--sys->scoreNum == -1) {
                        sys->scoreNum = 2;
                    }
                    sys->cursorDir = 0;
                } else if(sys->pad->DStkDown[0] & PAD_BUTTON_RIGHT) {
                    sys->scoreNum = ++sys->scoreNum%3;
                    sys->cursorDir = 1;
                }
                Hu3DAnmNoSet(model->animId[MDMODEL_ANIM_CAP_TURN], sys->scoreNum);
                MdMessWinMesSet(mess, MDMESS_WIN_MAIN, FALSE, MESS_MGMODE_SYS_MGMATCH_MODE_3+sys->scoreNum, MDMESS_LIST_END);
                fn_1_17008(MDMINI_AUD_SE_CMN_MOVE);
                sys->mode = 4;
            } else if(HuPadBtnDown[0] & PAD_BUTTON_B) {
                star->motNext = MDMINI_STAR_MOT_IDLE;
                MdMessWinHomeClear(mess, MDMESS_WIN_MAIN);
                MdMessWinHomeClear(mess, MDMESS_WIN_HELP);
                fn_1_17008(MDMINI_AUD_SE_CMN_CANCEL);
                sys->mode = 5;
            } else if(HuPadBtnDown[0] & PAD_BUTTON_A) {
                star->motNext = MDMINI_STAR_MOT_IDLE;
                MdMessWinHomeClear(mess, MDMESS_WIN_MAIN);
                MdMessWinHomeClear(mess, MDMESS_WIN_HELP);
                fn_1_17008(MDMINI_AUD_SE_CMN_CONFIRM);
                sys->mode = 8;
            }
            break;

        case 4:
            if((ret = MdProcCall(MDMINI_PROC_CAP_WIN, 1)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                sys->mode = 3;
            }
            break;

        case 5:
            if((ret = MdProcCall(MDMINI_PROC_CURSOR, 13)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                sys->mode = 6;
            }
            break;

        case 6:
            if((ret = MdProcCall(MDMINI_PROC_CAP_WIN, 2)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                sys->mode = 7;
            }
            break;

        case 7:
            if((ret = MdProcCall(MDMINI_PROC_6, 4)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                sys->mode = 2;
                sys->flowMode = MDMINI_FLOW_MAP_SEL;
            }
            break;

        case 8:
            if((ret = MdProcCall(MDMINI_PROC_CURSOR, 13)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                sys->mode = 9;
            }
            break;

        case 9:
            if((ret = MdProcCall(MDMINI_PROC_CAP_WIN, 3)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                sys->mode = 0;
                sys->flowMode = MDMINI_FLOW_BONUS_SEL;
            }
            break;
    }
}

void fn_1_7638(void)
{
    MDMESS *mess;
    MDMINI_STAR *star;
    MDMODEL_WORK *model;
    MDMINI_PLAYERSPR *playerSpr;
    MDMINI_CHR *chr;
    int i;
    BOOL ret;

    model = sys->model;
    mess = sys->mess;
    star = sys->star;
    playerSpr = &sys->playerSpr[0];
    chr = &sys->chr[0];

    switch(sys->mode) {
        case 0:
            MdProcCall(MDMINI_PROC_RESET, 0);
            sys->gameType = 0;
            Hu3DAnmNoSet(model->animId[MDMODEL_ANIM_CAP_BONUS], sys->gameType);
            sys->mode = 1;
            break;

        case 1:
            if((ret = MdProcCall(MDMINI_PROC_CAP_GENRE, 0)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                sys->mode = 2;
            }
            break;

        case 2:
            if((ret = MdProcCall(MDMINI_PROC_CURSOR, 16)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                star->motNext = MDMINI_STAR_MOT_TALK;
                MdMessWinMesSet(mess, MDMESS_WIN_MAIN, FALSE, MESS_MGMODE_SYS_GENRE_SEL, MDMESS_LIST_END);
                MdMessWinMesSet(mess, MDMESS_WIN_HELP, FALSE, MESS_SYSHELP_MENU_SEL, MDMESS_LIST_END);
                sys->mode = 3;
            }
            break;

        case 3:
            if((sys->pad->DStkDown[0] & PAD_BUTTON_LEFT) || (sys->pad->DStkDown[0] & PAD_BUTTON_RIGHT)) {
                if(sys->pad->DStkDown[0] & PAD_BUTTON_LEFT) {
                    if(--sys->gameType == -1) {
                        sys->gameType = 2;
                    }
                    sys->cursorDir = 0;
                } else if(sys->pad->DStkDown[0] & PAD_BUTTON_RIGHT) {
                    sys->gameType = ++sys->gameType%3;
                    sys->cursorDir = 1;
                }
                Hu3DAnmNoSet(model->animId[MDMODEL_ANIM_CAP_BONUS], sys->gameType);
                fn_1_17008(MDMINI_AUD_SE_CMN_MOVE);
                sys->mode = 4;
            } else if(HuPadBtnDown[0] & PAD_BUTTON_B) {
                star->motNext = MDMINI_STAR_MOT_IDLE;
                MdMessWinHomeClear(mess, MDMESS_WIN_MAIN);
                MdMessWinHomeClear(mess, MDMESS_WIN_HELP);
                fn_1_17008(MDMINI_AUD_SE_CMN_CANCEL);
                sys->mode = 5;
            } else if(HuPadBtnDown[0] & PAD_BUTTON_A) {
                fn_1_17008(MDMINI_AUD_SE_CMN_CONFIRM);
                MdMessWinHomeClear(mess, MDMESS_WIN_MAIN);
                MdMessWinHomeClear(mess, MDMESS_WIN_HELP);
                star->motNext = MDMINI_STAR_MOT_TALK;
                MdProcWait(MDMINI_PROC_CURSOR, 17);
                MdProcCall(MDMINI_PROC_RESET, 0);
                MdProcWait(MDMINI_PROC_CAP_GENRE, 3);
                MdProcCall(MDMINI_PROC_RESET, 0);
                sys->mode = 0;
                sys->flowMode = MDMINI_FLOW_HANDICAP;
            }
            break;

        case 4:
            if((ret = MdProcCall(MDMINI_PROC_CAP_GENRE, 1)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                sys->mode = 3;
            }
            break;

        case 5:
            if((ret = MdProcCall(MDMINI_PROC_CURSOR, 17)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                sys->mode = 6;
            }
            break;

        case 6:
            if((ret = MdProcCall(MDMINI_PROC_CAP_GENRE, 2)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                sys->mode = 7;
            }
            break;

        case 7:
            if((ret = MdProcCall(MDMINI_PROC_CAP_WIN, 4)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                sys->mode = 2;
                sys->flowMode = MDMINI_FLOW_TURN_SEL;
            }
            break;

        case 8:
            if((ret = MdProcCall(MDMINI_PROC_CURSOR, 17)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                sys->mode = 9;
            }
            break;

        case 9:
            if((ret = MdProcCall(MDMINI_PROC_CAP_GENRE, 3)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                star->motNext = MDMINI_STAR_MOT_TALK;
                MdMessWinMesSet(mess, MDMESS_WIN_MAIN, TRUE, MESS_MGMODE_SYS_CONFIRM, MDMESS_LIST_END);
                fn_1_17008(MDMINI_AUD_SE_STAR_CONFIRM);
                sys->mode = 10;
            }
            break;

        case 10:
            if(MdMessWinCheck(mess, MDMESS_WIN_MAIN) == TRUE) {
                switch(MdMessWinChoiceGet(mess, MDMESS_WIN_MAIN)) {
                    case -1:
                    case 1:
                        MdMessWinMesSet(mess, MDMESS_WIN_MAIN, TRUE, MESS_MGMODE_SYS_RESET, MDMESS_LIST_END);
                        sys->mode = 11;
                        break;

                    case 0:
                        sys->loadState = MD_LOAD_STATE_LOAD;
                        sys->loadDir = LoadDirTbl[sys->selNoNow];
                        fn_1_16978();
                        MdMessWinMesSet(mess, MDMESS_WIN_MAIN, FALSE, MESS_MGMODE_SYS_MG_START, MDMESS_LIST_END);
                        while(!MdMessWinCheck(mess, MDMESS_WIN_MAIN)) {
                            HuPrcVSleep();
                        }
                        MdProcWait(MDMINI_PROC_13, 0);
                        WipeColorSet(255, 255, 255);
                        WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 55);
                        fn_1_17008(MDMINI_AUD_MUS_STOP);
                        fn_1_17008(10);
                        HuPrcVSleep();
                        while(WipeCheckEnd()) {
                            MdProcCall(MDMINI_PROC_13, 0);
                            HuPrcVSleep();
                        }
                        while(sys->loadState != MD_LOAD_STATE_DONE) {
                            HuPrcVSleep();
                        }
                        omOvlHisChg(0, sys->ovlHis->ovl, 1, sys->ovlHis->stat);
                        sys->ovlCallMode = MD_OVLCALL_CALL;
                        sys->nextDll = NextDllTbl[sys->selNoNow];
                        sys->skipWipeF = TRUE;
                        sys->exitF = TRUE;
                        break;
                }
            }
            break;

        case 11:
            if(MdMessWinCheck(mess, MDMESS_WIN_MAIN) == TRUE) {
                switch(MdMessWinChoiceGet(mess, MDMESS_WIN_MAIN)) {
                    case -1:
                    case 1:
                        MdMessWinHomeClear(mess, MDMESS_WIN_MAIN);
                        star->motNext = MDMINI_STAR_MOT_IDLE;
                        sys->mode = 13;
                        break;

                    case 0:
                        MdMessWinHomeClear(mess, MDMESS_WIN_MAIN);
                        star->mode = MDMINI_STAR_CENTER_START;
                        star->motNext = MDMINI_STAR_MOT_TALK;
                        for(i=0; i<4; i++) {
                            sys->padNoEnable[i] = 0;
                            playerSpr[i].mode = MDMINI_PLAYERSPR_SHRINK_START;
                            Hu3DMotionShiftSet(model->mdlId[sys->chrSel[i]+MDMODEL_CHR_00], model->motId[(sys->chrSel[i]*2)+3], 0, 16, HU3D_MOTATTR_LOOP);
                            chr[sys->chrSel[i]].mode = MDMINI_CHR_SELECT_WAIT;
                            MdRingModeSet(i, MDMINI_RING_MODE_ERASE);
                        }
                        sys->mode = 12;
                        break;
                }
            }
            break;

        case 12:
            if((ret = MdProcCall(MDMINI_PROC_10, 1)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                fn_1_14C2C();
                sys->prevPlayerNum = sys->playerNum;
                sys->mode = 6;
                sys->flowMode = MDMINI_FLOW_PLAYER_NUM_SEL;
            }
            break;

        case 13:
            if((ret = MdProcCall(MDMINI_PROC_CAP_GENRE, 4)) == TRUE) {
                MdProcCall(MDMINI_PROC_RESET, 0);
                sys->mode = 2;
            }
            break;
    }
}

void fn_1_81E0(void)
{
    int i;
    MDMESS *mess;
    MDMINI_STAR *star;
    MDMODEL_WORK *model;
    int ret;
    MDMINI_PLAYERSPR *playerSpr;
    MDMINI_CHR *chr;

    model = sys->model;
    mess = sys->mess;
    star = sys->star;
    playerSpr = &sys->playerSpr[0];
    chr = &sys->chr[0];

    switch(sys->mode) {
        case 0:
            MdProcCall(MDMINI_PROC_RESET, 0);
            if(sys->gameType == 0) {
                MdProcWait(MDMINI_PROC_11, 0);
                MdProcCall(MDMINI_PROC_RESET, 0);
                sys->mode = 3;
            } else if(sys->gameType == 2) {
                sys->teamOrderNo = 0;
                fn_1_15DE8();
                fn_1_16168();
                star->motNext = MDMINI_STAR_MOT_TALK;
                MdMessWinMesSet(mess, MDMESS_WIN_MAIN, FALSE, MESS_MGMODE_SYS_TEAM_SEL, MDMESS_LIST_END);
                for(i=0; i<4; i++) {
                    if(sys->teamOrder[i] < 2) {
                        MdRingModeSet(i, 9);
                    } else {
                        MdRingModeSet(i, 11);
                    }
                }
                MdProcWait(MDMINI_PROC_CHR, 5);
                MdProcCall(MDMINI_PROC_RESET, 0);
                MdMessWinMesSet(mess, MDMESS_WIN_HELP, FALSE, MESS_SYSHELP_MENU_SEL, MDMESS_LIST_END);
                MdProcWait(MDMINI_PROC_CURSOR, 20);
                MdProcCall(MDMINI_PROC_RESET, 0);
                sys->mode = 1;
            } else if(sys->gameType == 1) {
                sys->handicapNo = 0;
                fn_1_161B8();
                fn_1_16308();
                star->motNext = MDMINI_STAR_MOT_TALK;
                MdMessWinMesSet(mess, MDMESS_WIN_MAIN, FALSE, MESS_MGMODE_SYS_TEAM_SEL, MDMESS_LIST_END);
                for(i=0; i<4; i++) {
                    if(sys->handicapOrder[i] < 1) {
                        MdRingModeSet(i, 9);
                    } else {
                        MdRingModeSet(i, 11);
                    }
                }
                MdProcWait(MDMINI_PROC_CHR, 8);
                MdProcCall(MDMINI_PROC_RESET, 0);
                MdMessWinMesSet(mess, MDMESS_WIN_HELP, FALSE, MESS_SYSHELP_MENU_SEL, MDMESS_LIST_END);
                MdProcWait(MDMINI_PROC_CURSOR, 20);
                MdProcCall(MDMINI_PROC_RESET, 0);
                sys->mode = 2;
            }
            break;

        case 1:
            if((sys->pad->DStkDown[0] & PAD_BUTTON_LEFT) || (sys->pad->DStkDown[0] & PAD_BUTTON_RIGHT)) {
                if(sys->pad->DStkDown[0] & PAD_BUTTON_LEFT) {
                    if(--sys->teamOrderNo == -1) {
                        sys->teamOrderNo = 2;
                    }
                    sys->cursorDir = 0;
                } else if(sys->pad->DStkDown[0] & PAD_BUTTON_RIGHT) {
                    sys->teamOrderNo = (++sys->teamOrderNo)%3;
                    sys->cursorDir = 1;
                }
                fn_1_17008(MDMINI_AUD_SE_CMN_MOVE);
                fn_1_16168();
                fn_1_15DE8();
                for(i=0; i<4; i++) {
                    if(sys->teamOrder[i] < 2) {
                        MdRingModeSet(i, 9);
                    } else {
                        MdRingModeSet(i, 11);
                    }
                }
                MdProcWait(MDMINI_PROC_CHR, 7);
                MdProcCall(MDMINI_PROC_RESET, 0);
            } else if(HuPadBtnDown[0] & PAD_BUTTON_B) {
                fn_1_17008(MDMINI_AUD_SE_CMN_CANCEL);
                MdMessWinHomeClear(mess, MDMESS_WIN_MAIN);
                MdMessWinHomeClear(mess, MDMESS_WIN_HELP);
                star->motNext = MDMINI_STAR_MOT_IDLE;
                MdProcWait(MDMINI_PROC_CURSOR, 21);
                MdProcCall(MDMINI_PROC_RESET, 0);
                for(i=0; i<4; i++) {
                    MdRingModeSet(i, 13);
                }
                MdProcWait(MDMINI_PROC_CHR, 6);
                MdProcCall(MDMINI_PROC_RESET, 0);
                Hu3DAnmNoSet(model->animId[MDMODEL_ANIM_CAP_BONUS], sys->gameType);
                MdProcWait(MDMINI_PROC_CAP_GENRE, 4);
                MdProcCall(MDMINI_PROC_RESET, 0);
                MdMessWinMesSet(mess, MDMESS_WIN_MAIN, FALSE, MESS_MGMODE_SYS_GENRE_SEL, MDMESS_LIST_END);
                star->motNext = MDMINI_STAR_MOT_TALK;
                MdProcWait(MDMINI_PROC_CURSOR, 16);
                MdProcCall(MDMINI_PROC_RESET, 0);
                MdMessWinMesSet(mess, MDMESS_WIN_HELP, FALSE, MESS_SYSHELP_MENU_SEL, MDMESS_LIST_END);
                sys->mode = 3;
                sys->flowMode = MDMINI_FLOW_BONUS_SEL;
            } else if(HuPadBtnDown[0] & PAD_BUTTON_A) {
                fn_1_17008(MDMINI_AUD_SE_CMN_CONFIRM);
                MdMessWinHomeClear(mess, MDMESS_WIN_MAIN);
                MdMessWinHomeClear(mess, MDMESS_WIN_HELP);
                star->motNext = MDMINI_STAR_MOT_IDLE;
                MdProcWait(MDMINI_PROC_CURSOR, 21);
                MdProcCall(MDMINI_PROC_RESET, 0);
                MdProcWait(MDMINI_PROC_11, 0);
                MdProcCall(MDMINI_PROC_RESET, 0);
                sys->mode = 3;
            }
            break;

        case 2:
            if((sys->pad->DStkDown[0] & PAD_BUTTON_LEFT) || (sys->pad->DStkDown[0] & PAD_BUTTON_RIGHT)) {
                if(sys->pad->DStkDown[0] & PAD_BUTTON_LEFT) {
                    if(--sys->handicapNo == -1) {
                        sys->handicapNo = 3;
                    }
                    sys->cursorDir = 0;
                } else if(sys->pad->DStkDown[0] & PAD_BUTTON_RIGHT) {
                    sys->handicapNo = (++sys->handicapNo)%4;
                    sys->cursorDir = 1;
                }
                fn_1_17008(MDMINI_AUD_SE_CMN_MOVE);
                fn_1_16308();
                fn_1_161B8();
                for(i=0; i<4; i++) {
                    if(sys->handicapOrder[i] < 1) {
                        MdRingModeSet(i, 9);
                    } else {
                        MdRingModeSet(i, 11);
                    }
                }
                MdProcWait(MDMINI_PROC_CHR, 10);
                MdProcCall(MDMINI_PROC_RESET, 0);
            } else if(HuPadBtnDown[0] & PAD_BUTTON_B) {
                fn_1_17008(MDMINI_AUD_SE_CMN_CANCEL);
                MdMessWinHomeClear(mess, MDMESS_WIN_MAIN);
                MdMessWinHomeClear(mess, MDMESS_WIN_HELP);
                star->motNext = MDMINI_STAR_MOT_IDLE;
                MdProcWait(MDMINI_PROC_CURSOR, 21);
                MdProcCall(MDMINI_PROC_RESET, 0);
                for(i=0; i<4; i++) {
                    MdRingModeSet(i, 13);
                }
                MdProcWait(MDMINI_PROC_CHR, 9);
                MdProcCall(MDMINI_PROC_RESET, 0);
                Hu3DAnmNoSet(model->animId[MDMODEL_ANIM_CAP_BONUS], sys->gameType);
                MdProcWait(MDMINI_PROC_CAP_GENRE, 4);
                MdProcCall(MDMINI_PROC_RESET, 0);
                MdMessWinMesSet(mess, MDMESS_WIN_MAIN, FALSE, MESS_MGMODE_SYS_GENRE_SEL, MDMESS_LIST_END);
                star->motNext = MDMINI_STAR_MOT_TALK;
                MdProcWait(MDMINI_PROC_CURSOR, 16);
                MdProcCall(MDMINI_PROC_RESET, 0);
                MdMessWinMesSet(mess, MDMESS_WIN_HELP, FALSE, MESS_SYSHELP_MENU_SEL, MDMESS_LIST_END);
                sys->mode = 3;
                sys->flowMode = MDMINI_FLOW_BONUS_SEL;
            } else if(HuPadBtnDown[0] & PAD_BUTTON_A) {
                fn_1_17008(MDMINI_AUD_SE_CMN_CONFIRM);
                MdMessWinHomeClear(mess, MDMESS_WIN_MAIN);
                MdMessWinHomeClear(mess, MDMESS_WIN_HELP);
                star->motNext = MDMINI_STAR_MOT_IDLE;
                MdProcWait(MDMINI_PROC_CURSOR, 21);
                MdProcCall(MDMINI_PROC_RESET, 0);
                MdProcWait(MDMINI_PROC_11, 0);
                MdProcCall(MDMINI_PROC_RESET, 0);
                sys->mode = 3;
            }
            break;

        case 3:
            star->motNext = MDMINI_STAR_MOT_TALK;
            MdMessWinMesSet(mess, MDMESS_WIN_MAIN, TRUE, MESS_MGMODE_SYS_CONFIRM, MDMESS_LIST_END);
            fn_1_17008(MDMINI_AUD_SE_STAR_CONFIRM);
            while(!MdMessWinCheck(mess, MDMESS_WIN_MAIN)) {
                HuPrcVSleep();
            }
            ret = MdMessWinChoiceGet(mess, MDMESS_WIN_MAIN);
            if(ret == -1 || ret == 1) {
                MdMessWinMesSet(mess, MDMESS_WIN_MAIN, TRUE, MESS_MGMODE_SYS_RESET, MDMESS_LIST_END);
                while(!MdMessWinCheck(mess, MDMESS_WIN_MAIN)) {
                    HuPrcVSleep();
                }
                ret = MdMessWinChoiceGet(mess, MDMESS_WIN_MAIN);
                if(ret == -1 || ret == 1) {
                    star->motNext = MDMINI_STAR_MOT_IDLE;
                    MdMessWinHomeClear(mess, MDMESS_WIN_MAIN);
                    if(sys->gameType == 0) {
                        MdProcWait(MDMINI_PROC_11, 1);
                        MdProcCall(MDMINI_PROC_RESET, 0);
                        MdProcWait(MDMINI_PROC_CAP_GENRE, 4);
                        MdProcCall(MDMINI_PROC_RESET, 0);
                        MdMessWinMesSet(mess, MDMESS_WIN_MAIN, FALSE, MESS_MGMODE_SYS_GENRE_SEL, MDMESS_LIST_END);
                        star->motNext = MDMINI_STAR_MOT_TALK;
                        MdProcWait(MDMINI_PROC_CURSOR, 16);
                        MdProcCall(MDMINI_PROC_RESET, 0);
                        MdMessWinMesSet(mess, MDMESS_WIN_HELP, FALSE, MESS_SYSHELP_MENU_SEL, MDMESS_LIST_END);
                        sys->mode = 3;
                        sys->flowMode = MDMINI_FLOW_BONUS_SEL;
                    } else if(sys->gameType == 2) {
                        MdProcWait(MDMINI_PROC_11, 1);
                        MdProcCall(MDMINI_PROC_RESET, 0);
                        star->motNext = MDMINI_STAR_MOT_TALK;
                        MdMessWinMesSet(mess, MDMESS_WIN_MAIN, FALSE, MESS_MGMODE_SYS_TEAM_SEL, MDMESS_LIST_END);
                        MdMessWinMesSet(mess, MDMESS_WIN_HELP, FALSE, MESS_SYSHELP_MENU_SEL, MDMESS_LIST_END);
                        MdProcWait(MDMINI_PROC_CURSOR, 20);
                        MdProcCall(MDMINI_PROC_RESET, 0);
                        sys->mode = 1;
                    } else if(sys->gameType == 1) {
                        MdProcWait(MDMINI_PROC_11, 1);
                        MdProcCall(MDMINI_PROC_RESET, 0);
                        star->motNext = MDMINI_STAR_MOT_TALK;
                        MdMessWinMesSet(mess, MDMESS_WIN_MAIN, FALSE, MESS_MGMODE_SYS_TEAM_SEL, MDMESS_LIST_END);
                        MdMessWinMesSet(mess, MDMESS_WIN_HELP, FALSE, MESS_SYSHELP_MENU_SEL, MDMESS_LIST_END);
                        MdProcWait(MDMINI_PROC_CURSOR, 20);
                        MdProcCall(MDMINI_PROC_RESET, 0);
                        sys->mode = 2;
                    }
                } else if(ret == 0) {
                    MdMessWinHomeClear(mess, MDMESS_WIN_MAIN);
                    star->mode = MDMINI_STAR_CENTER_START;
                    star->motNext = MDMINI_STAR_MOT_TALK;
                    for(i=0; i<4; i++) {
                        sys->chrSelEnd[i] = FALSE;
                        playerSpr[i].mode = MDMINI_PLAYERSPR_SHRINK_START;
                        Hu3DMotionShiftSet(model->mdlId[sys->chrSel[i]+MDMODEL_CHR_00], model->motId[(sys->chrSel[i]*2)+MDMODEL_MOT_MARIO_IDLE], 0, 16, HU3D_MOTATTR_LOOP);
                        chr[sys->chrSel[i]].mode = MDMINI_CHR_SELECT_WAIT;
                        MdRingModeSet(i, MDMINI_RING_MODE_ERASE);
                    }
                    MdProcWait(MDMINI_PROC_10, 1);
                    MdProcCall(MDMINI_PROC_RESET, 0);
                    fn_1_14C2C();
                    sys->prevPlayerNum = sys->playerNum;
                    sys->mode = 6;
                    sys->flowMode = MDMINI_FLOW_PLAYER_NUM_SEL;
                }
            } else if(ret == 0) {
                sys->loadState = MD_LOAD_STATE_LOAD;
                sys->loadDir = LoadDirTbl[sys->selNoNow];
                fn_1_16978();
                MdMessWinMesSet(mess, MDMESS_WIN_MAIN, FALSE, MESS_MGMODE_SYS_MG_START, MDMESS_LIST_END);
                while(!MdMessWinCheck(mess, MDMESS_WIN_MAIN)) {
                    HuPrcVSleep();
                }
                MdProcWait(MDMINI_PROC_13, 0);
                WipeColorSet(255, 255, 255);
                WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 55);
                fn_1_17008(MDMINI_AUD_MUS_STOP);
                fn_1_17008(10);
                HuPrcVSleep();
                while(WipeCheckEnd()) {
                    MdProcCall(MDMINI_PROC_13, 0);
                    HuPrcVSleep();
                }
                while(sys->loadState != MD_LOAD_STATE_DONE) {
                    HuPrcVSleep();
                }
                omOvlHisChg(0, sys->ovlHis->ovl, 1, sys->ovlHis->stat);
                sys->ovlCallMode = MD_OVLCALL_CALL;
                sys->nextDll = NextDllTbl[sys->selNoNow];
                sys->skipWipeF = TRUE;
                sys->exitF = TRUE;
            }
            break;
    }
}

BOOL fn_1_9714(int param)
{
    sys->dirtyF = FALSE;
    sys->cursorInitF = FALSE;
    sys->winInitF = FALSE;
    sys->vsInitF = FALSE;
    sys->nameInitF = FALSE;
    sys->unk3E0 = FALSE;
    sys->procTime = 0.0f;
    sys->procOfsTime = 0.0f;
    sys->cameraTime = 0.0f;
    sys->cameraTotalTime = 0.0f;
    sys->sprTime = 0.0f;
    sys->unk400 = 0.0f;
    sys->nameTime = 0.0f;
    sys->procT = 0.0f;
    sys->procOfsT = 0.0f;
    sys->cameraT = 0.0f;
    sys->cameraTotalT = 0.0f;
    sys->sprT = 0.0f;
    sys->unk41C = 0.0f;
    sys->nameT = 0.0f;
    return FALSE;
}

typedef struct MdPartyLerpData_s {
    HuVecF hvrA;
    HuVecF centerA;
    HuVecF hvrB;
    HuVecF centerB;
    int type;
    float step;
} MDMINI_LERP_DATA;

BOOL fn_1_991C(int param)
{
    MDMINI_LERP_DATA *lerpP;
    MDCAMERA_WORK *cameraWork;
    MDCAMERA *camera;

    static MDMINI_LERP_DATA lerpData[1] = {
        {
            { 90, 0, 1920 },
            { 0, 0, 0 },
            { 90, 0, 500 },
            { 0, 150, 0 },
            MDMATH_OSC_90,
            60
        }
    };
    
    cameraWork = sys->camera;
    camera = &cameraWork->camera[MDCAMERA_MAIN];
    lerpP = &lerpData[param];
    MdMathOscillate(lerpP->type, &sys->cameraTime, &sys->cameraT, lerpP->step);
    MdMathOscillate(MDMATH_OSC_180, &sys->cameraTotalTime, &sys->cameraTotalT, lerpP->step);
    MdMathLerp(&camera->hvr.x, lerpP->hvrA.x, lerpP->hvrB.x, sys->cameraT);
    MdMathLerp(&camera->hvr.y, lerpP->hvrA.y, lerpP->hvrB.y, sys->cameraT);
    MdMathLerp(&camera->hvr.z, lerpP->hvrA.z, lerpP->hvrB.z, sys->cameraT);
    MdMathLerp(&camera->center.x, lerpP->centerA.x, lerpP->centerB.x, sys->cameraT);
    MdMathLerp(&camera->center.y, lerpP->centerA.y, lerpP->centerB.y, sys->cameraT);
    MdMathLerp(&camera->center.z, lerpP->centerA.z, lerpP->centerB.z, sys->cameraT);
    if(sys->procTime >= 1.0f) {
        return TRUE;
    } else {
        return FALSE;
    }
}

#define MDMINI_CURSOR_ISIN(p) ((p) == 0 || (p) == 4 || (p) == 8 || (p) == 12 || (p) == 16 || (p) == 20)
#define MDMINI_CURSOR_ISOUT(p) ((p) == 1 || (p) == 5 || (p) == 9 || (p) == 13 || (p) == 17 || (p) == 21)
#define MDMINI_CURSOR_ISLEFT(p) ((p) == 2 || (p) == 6 || (p) == 10 || (p) == 14 || (p) == 18 || (p) == 22)
#define MDMINI_CURSOR_ISRIGHT(p) ((p) == 3 || (p) == 7 || (p) == 11 || (p) == 15 || (p) == 19 || (p) == 23)

BOOL fn_1_9AC8(int param)
{
    HuVec2f *sprPos;
    HuVec2f *sprScale;
    HUSPRGRPID grpId;
    MDSPRITE_WORK *sprite;
    MDCAMERA_WORK *camera;
    MDMODEL_WORK *model;

    static HuVecF pos[4];

    HuVecF pos2D;
    HuVecF pos3D;

    camera = sys->camera;
    model = sys->model;
    sprite = sys->sprite;
    grpId = sprite->grpId[MDSPR_GRP_CURSOR_UDLR];
    if(!sys->cursorInitF) {
        if(param >= 0 && param <= 3) {
            pos3D.x = 0;
            pos3D.y = 176.8f;
            pos3D.z = -580;
            Hu3D3Dto2D(&pos3D, camera->camera[MDCAMERA_MAIN].bit, &pos2D);
            pos[0].x = -64;
            pos[0].y = pos2D.y;
            pos[0].z = 0;
            pos[1].x = pos2D.x-192;
            pos[1].y = pos2D.y;
            pos[1].z = 0;
            pos[2].x = 640;
            pos[2].y = pos2D.y;
            pos[2].z = 0;
            pos[3].x = 192+pos2D.x;
            pos[3].y = pos2D.y;
            pos[3].z = 0;
        } else if(param >= 4 && param <= 7) {
            pos3D = *model->posP[sys->chrSel[sys->comPlayerNo[sys->selComNo]]+MDMODEL_CAP_CHR_00];
            Hu3D3Dto2D(&pos3D, camera->camera[MDCAMERA_MAIN].bit, &pos2D);
            pos2D.y -= 44;
            pos[0].x = -64;
            pos[0].y = pos2D.y;
            pos[0].z = 0;
            pos[1].x = pos2D.x-46;
            pos[1].y = pos2D.y;
            pos[1].z = 0;
            pos[2].x = 640;
            pos[2].y = pos2D.y;
            pos[2].z = 0;
            pos[3].x = (46+pos2D.x)-3;
            pos[3].y = pos2D.y;
            pos[3].z = 0;
        } else if(param >= 8 && param <= 11) {
            pos3D.x = 0;
            pos3D.y = 180;
            pos3D.z = -200;
            Hu3D3Dto2D(&pos3D, camera->camera[MDCAMERA_MAIN].bit, &pos2D);
            pos[0].x = -64;
            pos[0].y = pos2D.y;
            pos[0].z = 0;
            pos[1].x = pos2D.x-96;
            pos[1].y = pos2D.y;
            pos[1].z = 0;
            pos[2].x = 640;
            pos[2].y = pos2D.y;
            pos[2].z = 0;
            pos[3].x = 96+pos2D.x;
            pos[3].y = pos2D.y;
            pos[3].z = 0;
        } else if(param >= 12 && param <= 15) {
            pos3D = *model->posP[MDMODEL_CAP_WIN];
            Hu3D3Dto2D(&pos3D, camera->camera[MDCAMERA_MAIN].bit, &pos2D);
            pos[0].x = -64;
            pos[0].y = pos2D.y;
            pos[0].z = 0;
            pos[1].x = pos2D.x-80;
            pos[1].y = pos2D.y;
            pos[1].z = 0;
            pos[2].x = 640;
            pos[2].y = pos2D.y;
            pos[2].z = 0;
            pos[3].x = 80+pos2D.x;
            pos[3].y = pos2D.y;
            pos[3].z = 0;
        } else if(param >= 16 && param <= 19) {
            pos3D = *model->posP[MDMODEL_CAP_GENRE];
            Hu3D3Dto2D(&pos3D, camera->camera[MDCAMERA_MAIN].bit, &pos2D);
            pos[0].x = -64;
            pos[0].y = pos2D.y;
            pos[0].z = 0;
            pos[1].x = pos2D.x-80;
            pos[1].y = pos2D.y;
            pos[1].z = 0;
            pos[2].x = 640;
            pos[2].y = pos2D.y;
            pos[2].z = 0;
            pos[3].x = 80+pos2D.x;
            pos[3].y = pos2D.y;
            pos[3].z = 0;
        } else if(param >= 20 && param <= 23) {
            pos3D.x = -460;
            pos3D.y = 120;
            pos3D.z = -600;
            Hu3D3Dto2D(&pos3D, camera->camera[MDCAMERA_MAIN].bit, &pos2D);
            pos[0].x = -64;
            pos[0].y = pos2D.y;
            pos[0].z = 0;
            pos[1].x = pos2D.x-64;
            pos[1].y = pos2D.y;
            pos[1].z = 0;
            pos3D.x = 460;
            pos3D.y = 120;
            pos3D.z = -600;
            Hu3D3Dto2D(&pos3D, camera->camera[MDCAMERA_MAIN].bit, &pos2D);
            pos[2].x = 640;
            pos[2].y = pos2D.y;
            pos[2].z = 0;
            pos[3].x = 64+pos2D.x;
            pos[3].y = pos2D.y;
            pos[3].z = 0;
        }
        if(MDMINI_CURSOR_ISIN(param)) {
        HuSprGrpPosSet(grpId, 0, 0);
        sprPos = MdSpritePosGet(sprite, MDSPR_GRP_CURSOR_UDLR, 0);
        sprScale = MdSpriteScaleGet(sprite, MDSPR_GRP_CURSOR_UDLR, 0);
        sprPos->x = pos[0].x;
        sprPos->y = pos[0].y;
        if(param == 4) {
            sprScale->x = sprScale->y = 0.7f;
        } else {
            sprScale->x = sprScale->y = 1;
        }
        HuSprDispOn(grpId, 0);
        sprPos = MdSpritePosGet(sprite, MDSPR_GRP_CURSOR_UDLR, 1);
        sprScale = MdSpriteScaleGet(sprite, MDSPR_GRP_CURSOR_UDLR, 1);
        sprPos->x = pos[2].x;
        sprPos->y = pos[2].y;
        if(param == 4) {
            sprScale->x = sprScale->y = 0.7f;
        } else {
            sprScale->x = sprScale->y = 1;
        }
        HuSprDispOn(grpId, 1);
        } else if(MDMINI_CURSOR_ISLEFT(param)) {
        sprPos = MdSpritePosGet(sprite, MDSPR_GRP_CURSOR_UDLR, 4);
        sprScale = MdSpriteScaleGet(sprite, MDSPR_GRP_CURSOR_UDLR, 4);
        sprPos->x = pos[1].x;
        sprPos->y = pos[1].y;
        if(param == 6) {
            sprScale->x = sprScale->y = 0.7f;
        } else {
            sprScale->x = sprScale->y = 1;
        }
        HuSprTPLvlSet(grpId, 4, 0);
        HuSprDispOn(grpId, 4);
        } else if(MDMINI_CURSOR_ISRIGHT(param)) {
        sprPos = MdSpritePosGet(sprite, MDSPR_GRP_CURSOR_UDLR, 5);
        sprScale = MdSpriteScaleGet(sprite, MDSPR_GRP_CURSOR_UDLR, 5);
        sprPos->x = pos[3].x;
        sprPos->y = pos[3].y;
        if(param == 7) {
            sprScale->x = sprScale->y = 0.7f;
        } else {
            sprScale->x = sprScale->y = 1;
        }
        HuSprTPLvlSet(grpId, 5, 0);
        HuSprDispOn(grpId, 5);
        }
        sys->cursorInitF = TRUE;
    }
    if(MDMINI_CURSOR_ISIN(param)) {
    MdMathOscillate(MDMATH_OSC_90, &sys->sprTime, &sys->sprT, 15);
    sprPos = MdSpritePosGet(sprite, MDSPR_GRP_CURSOR_UDLR, 0);
    MdMathLerp(&sprPos->x, pos[0].x, pos[1].x, sys->sprT);
    sprPos = MdSpritePosGet(sprite, MDSPR_GRP_CURSOR_UDLR, 1);
    MdMathLerp(&sprPos->x, pos[2].x, pos[3].x, sys->sprT);
    } else if(MDMINI_CURSOR_ISOUT(param)) {
    MdMathOscillate(MDMATH_OSC_90, &sys->sprTime, &sys->sprT, 15);
    sprPos = MdSpritePosGet(sprite, MDSPR_GRP_CURSOR_UDLR, 0);
    MdMathLerp(&sprPos->x, pos[1].x, pos[0].x, sys->sprT);
    sprPos = MdSpritePosGet(sprite, MDSPR_GRP_CURSOR_UDLR, 1);
    MdMathLerp(&sprPos->x, pos[3].x, pos[2].x, sys->sprT);
    } else if(MDMINI_CURSOR_ISLEFT(param)) {
    MdMathOscillate(MDMATH_OSC_180, &sys->sprTime, &sys->sprT, 15);
    sprPos = MdSpritePosGet(sprite, MDSPR_GRP_CURSOR_UDLR, 0);
    sprScale = MdSpriteScaleGet(sprite, MDSPR_GRP_CURSOR_UDLR, 0);
    MdMathLerp(&sprPos->x, pos[1].x, pos[1].x-24, sys->sprT);
    if(param == 6) {
        MdMathLerp(&sprScale->x, 0.7f, 1, sys->sprT);
        MdMathLerp(&sprScale->y, 0.7f, 1, sys->sprT);
    } else {
        MdMathLerp(&sprScale->x, 1, 1.5f, sys->sprT);
        MdMathLerp(&sprScale->y, 1, 1.5f, sys->sprT);
    }
    sprPos = MdSpritePosGet(sprite, MDSPR_GRP_CURSOR_UDLR, 4);
    sprScale = MdSpriteScaleGet(sprite, MDSPR_GRP_CURSOR_UDLR, 4);
    MdMathLerp(&sprPos->x, pos[1].x, pos[1].x-24, sys->sprT);
    MdMathLerp(&sprScale->x, 1, 1.5f, sys->sprT);
    MdMathLerp(&sprScale->y, 1, 1.5f, sys->sprT);
    HuSprTPLvlSet(grpId, 4, sys->sprT);
    } else if(MDMINI_CURSOR_ISRIGHT(param)) {
    MdMathOscillate(MDMATH_OSC_180, &sys->sprTime, &sys->sprT, 15);
    sprPos = MdSpritePosGet(sprite, MDSPR_GRP_CURSOR_UDLR, 1);
    sprScale = MdSpriteScaleGet(sprite, MDSPR_GRP_CURSOR_UDLR, 1);
    MdMathLerp(&sprPos->x, pos[3].x, 24+pos[3].x, sys->sprT);
    if(param == 7) {
        MdMathLerp(&sprScale->x, 0.7f, 1, sys->sprT);
        MdMathLerp(&sprScale->y, 0.7f, 1, sys->sprT);
    } else {
        MdMathLerp(&sprScale->x, 1, 1.5f, sys->sprT);
        MdMathLerp(&sprScale->y, 1, 1.5f, sys->sprT);
    }
    sprPos = MdSpritePosGet(sprite, MDSPR_GRP_CURSOR_UDLR, 5);
    sprScale = MdSpriteScaleGet(sprite, MDSPR_GRP_CURSOR_UDLR, 5);
    MdMathLerp(&sprPos->x, pos[3].x, 24+pos[3].x, sys->sprT);
    MdMathLerp(&sprScale->x, 1, 1.5f, sys->sprT);
    MdMathLerp(&sprScale->y, 1, 1.5f, sys->sprT);
    HuSprTPLvlSet(grpId, 5, sys->sprT);
    }
    if(sys->sprTime >= 1) {
        if(MDMINI_CURSOR_ISOUT(param)) {
        MdSpriteDispSet(sprite, MDSPR_GRP_CURSOR_UDLR, FALSE);
        } else if(MDMINI_CURSOR_ISLEFT(param)) {
        HuSprDispOff(grpId, 4);
        } else if(MDMINI_CURSOR_ISRIGHT(param)) {
        HuSprDispOff(grpId, 5);
        }
        return TRUE;
    } else {
        return FALSE;
    }
}

BOOL fn_1_AF34(int param)
{
    MDSPRITE_WORK *sprite; //r31
    HuVec2f *bgScale; //r30
    HuVec2f *topPos; //r29
    HuVec2f *bottomPos; //r28
    HuVec2f *topScale; //r27
    HuVec2f *bottomScale; //r26
    
    HuVec2f *bgPos; //sp+0xC
    HUSPRGRPID grpId; //sp+0x8
    
    static int updateMode; //bss+0x248
    static float openY; //bss+0x244
    static float closeY; //bss+0x240
    static float bgScaleY; //bss+0x23C
    
    sprite = sys->sprite;
    grpId = sprite->grpId[MDSPR_GRP_WIN];
    topPos = MdSpritePosGet(sprite, MDSPR_GRP_WIN, MDSPR_GRP_MEMBERNO(MDSPR_GRP_WIN, MDSPR_WIN_FRAME_TOP));
    bottomPos = MdSpritePosGet(sprite, MDSPR_GRP_WIN, MDSPR_GRP_MEMBERNO(MDSPR_GRP_WIN, MDSPR_WIN_FRAME_BOTTOM));
    bgPos = MdSpritePosGet(sprite, MDSPR_GRP_WIN, MDSPR_GRP_MEMBERNO(MDSPR_GRP_WIN, MDSPR_WIN_BG));
    topScale = MdSpriteScaleGet(sprite, MDSPR_GRP_WIN, MDSPR_GRP_MEMBERNO(MDSPR_GRP_WIN, MDSPR_WIN_FRAME_TOP));
    bottomScale = MdSpriteScaleGet(sprite, MDSPR_GRP_WIN, MDSPR_GRP_MEMBERNO(MDSPR_GRP_WIN, MDSPR_WIN_FRAME_BOTTOM));
    bgScale = MdSpriteScaleGet(sprite, MDSPR_GRP_WIN, MDSPR_GRP_MEMBERNO(MDSPR_GRP_WIN, MDSPR_WIN_BG));
    if(!sys->winInitF) {
        updateMode = 0;
        openY = topPos->y;
        closeY = bottomPos->y;
        bgScaleY = bgScale->y;
        if(param == FALSE) {
            topPos->y = 0;
            bottomPos->y = 0;
            topScale->x = 0;
            bottomScale->x = 0;
            bgScale->x = 0;
            bgScale->y = 0;
            MdSpriteDispSet(sprite, MDSPR_GRP_WIN, TRUE);
        }
        sys->winInitF = TRUE;
    }
    if(param == FALSE) {
        if(updateMode == 0) {
            MdMathOscillate(MDMATH_OSC_90, &sys->sprTime, &sys->sprT, 12);
            MdMathLerp(&topPos->y, 0, -2, sys->sprT);
            MdMathLerp(&bottomPos->y, 0, 2, sys->sprT);
            MdMathLerp(&topScale->x, 0, 72, sys->sprT);
            MdMathLerp(&bottomScale->x, 0, 72, sys->sprT);
            MdMathLerp(&bgScale->x, 0, 1, sys->sprT);
            if(sys->sprTime >= 1) {
                sys->sprTime = 0;
                updateMode = 1;
            }
        } else if(updateMode == 1) {
            MdMathOscillate(MDMATH_OSC_90, &sys->sprTime, &sys->sprT, 15);
            MdMathLerp(&topPos->y, -2, -28, sys->sprT);
            MdMathLerp(&bottomPos->y, 2, 32, sys->sprT);
            MdMathLerp(&bgScale->y, 0, 8, sys->sprT);
            if(sys->sprTime >= 1) {
                updateMode = 2;
            }
        }
    } else if(param == TRUE) {
        if(updateMode == 0) {
            MdMathOscillate(MDMATH_OSC_90, &sys->sprTime, &sys->sprT, 15);
            MdMathLerp(&topPos->y, openY, -2, sys->sprT);
            MdMathLerp(&bottomPos->y, closeY, 2, sys->sprT);
            MdMathLerp(&bgScale->y, bgScaleY, 0, sys->sprT);
            if(sys->sprTime >= 1) {
                sys->sprTime = 0;
                updateMode = 1;
            }
            
        } else if(updateMode == 1) {
            MdMathOscillate(MDMATH_OSC_90, &sys->sprTime, &sys->sprT, 12);
            MdMathLerp(&topPos->y, -2, 0, sys->sprT);
            MdMathLerp(&bottomPos->y, 2, 0, sys->sprT);
            MdMathLerp(&topScale->x, 72, 0, sys->sprT);
            MdMathLerp(&bottomScale->x, 72, 0, sys->sprT);
            MdMathLerp(&bgScale->x, 1, 0, sys->sprT);
            if(sys->sprTime >= 1) {
                updateMode = 2;
            }
        }
    }
    if(updateMode == 2) {
        if(param == TRUE) {
            MdSpriteDispSet(sprite, MDSPR_GRP_WIN, FALSE);
        }
        return TRUE;
    } else {
        return FALSE;
    }
}

BOOL fn_1_B668(int param)
{
    int i; //r31
    MDMODEL_WORK *model; //r30
    MDCAMERA_WORK *camera; //r28
    
    HuVecF *posP[4][2]; //sp+0x48
    HuVecF *rotP[4][2]; //sp+0x28
    HuVecF *scaleP[4][2]; //sp+0x8
    
    static HuVecF centerPos = { 0, 0, 0 }; //data+0x638
    static HuVecF playerPos[4] = { }; //data+0x644
    
    static HuVecF centerPos3D; //bss+0x230
    static HuVecF playerPos3D[4]; //bss+0x200
    static s8 playerType[4][2]; //bss+0x1F8
    static float radius; //bss+0x1F4
    static float angle; //bss+0x1F0
    static HuVecF oldPlayerPos3D[4]; //bss+0x1C0
    
    model = sys->model;
    
    camera = sys->camera;
    if(!capPlayerInitF) {
        centerPos.x = 288;
        centerPos.y = 176.64f;
        centerPos.z = 4000.0f;
        for(i=0; i<4; i++) {
            playerPos[i].x = 168.0f+(i*80.0f);
            playerPos[i].y = centerPos.y;
            playerPos[i].z = 2500;
        }
        capPlayerInitF = TRUE;
    }
    for(i=0; i<4; i++) {
        posP[i][0] = model->posP[i+MDMODEL_CAP_MAN_1P];
        posP[i][1] = model->posP[i+MDMODEL_CAP_COM_1P];
        
        rotP[i][0] = model->rotP[i+MDMODEL_CAP_MAN_1P];
        rotP[i][1] = model->rotP[i+MDMODEL_CAP_COM_1P];
        
        scaleP[i][0] = model->scaleP[i+MDMODEL_CAP_MAN_1P];
        scaleP[i][1] = model->scaleP[i+MDMODEL_CAP_COM_1P];
    }
    if(!sys->dirtyF) {
        for(i=0; i<4; i++) {
            if(i < sys->playerNum+1) {
                playerType[i][0] = 0;
            } else {
                playerType[i][0] = 1;
            }
            if(i < sys->prevPlayerNum+1) {
                playerType[i][1] = 0;
            } else {
                playerType[i][1] = 1;
            }
        }
        if(param == 0) {
            Hu3D2Dto3D(&centerPos, camera->camera[MDCAMERA_MAIN].bit, &centerPos3D);
            for(i=0; i<4; i++) {
                Hu3D2Dto3D(&playerPos[i], camera->camera[MDCAMERA_MAIN].bit, &playerPos3D[i]);
            }
            for(i=0; i<4; i++) {
                posP[i][playerType[i][0]]->x = centerPos3D.x;
                posP[i][playerType[i][0]]->y = centerPos3D.y;
                posP[i][playerType[i][0]]->z = centerPos3D.z;
                rotP[i][playerType[i][0]]->x = 0;
                rotP[i][playerType[i][0]]->y = 0;
                rotP[i][playerType[i][0]]->z = 0;
                scaleP[i][playerType[i][0]]->x = 0;
                scaleP[i][playerType[i][0]]->y = 0;
                scaleP[i][playerType[i][0]]->z = 0;
                if(playerType[i][0] == 0) {
                    Hu3DModelDispOn(model->mdlId[i+MDMODEL_CAP_MAN_1P]);
                    Hu3DModelDispOff(model->mdlId[i+MDMODEL_CAP_COM_1P]);
                } else if(playerType[i][0] == 1) {
                    Hu3DModelDispOff(model->mdlId[i+MDMODEL_CAP_MAN_1P]);
                    Hu3DModelDispOn(model->mdlId[i+MDMODEL_CAP_COM_1P]);
                }
            }
        } else if(param != 1 && (param == 2 || param == 3)) {
            for(i=0; i<4; i++) {
                if(playerType[i][0] != playerType[i][1]) {
                    posP[i][playerType[i][1]]->x = centerPos3D.x;
                    posP[i][playerType[i][1]]->y = centerPos3D.y;
                    posP[i][playerType[i][1]]->z = centerPos3D.z;
                    rotP[i][playerType[i][1]]->x = 0;
                    rotP[i][playerType[i][1]]->y = 0;
                    rotP[i][playerType[i][1]]->z = 0;
                    scaleP[i][playerType[i][1]]->x = 0;
                    scaleP[i][playerType[i][1]]->y = 0;
                    scaleP[i][playerType[i][1]]->z = 0;
                    Hu3DModelDispOn(model->mdlId[i+MDMODEL_CAP_MAN_1P+(playerType[i][0]*4)]);
                }
            }
            radius = 200;
            angle = 90;
            for(i=0; i<4; i++) {
                oldPlayerPos3D[i] = playerPos3D[i];
                oldPlayerPos3D[i].z -= radius;
            }
        }
        sys->dirtyF = TRUE;
    }
    if(param == 0) {
        MdMathOscillate(MDMATH_OSC_90, &sys->procTime, &sys->procT, 30);
        for(i=0; i<4; i++) {
            MdMathLerp(&posP[i][playerType[i][0]]->x, centerPos3D.x, playerPos3D[i].x, sys->procT);
            MdMathLerp(&posP[i][playerType[i][0]]->y, centerPos3D.y, playerPos3D[i].y, sys->procT);
            MdMathLerp(&posP[i][playerType[i][0]]->z, centerPos3D.z, playerPos3D[i].z, sys->procT);
            MdMathLerpScale(scaleP[i][playerType[i][0]], 0, 1, sys->procT);
        }
    } else if(param == 1) {
        MdMathOscillate(MDMATH_OSC_90_REV, &sys->procTime, &sys->procT, 15);
        for(i=0; i<4; i++) {
            MdMathLerpScale(scaleP[i][playerType[i][0]], 1, 0, sys->procT);
        }
    } else if(param == 2 || param == 3) {
        MdMathOscillate(MDMATH_OSC_90_REV, &sys->procTime, &sys->procT, 20);
        if(param == 2) {
            MdMathLerp(&angle, 90, 270, sys->procT);
        } else if(param == 3) {
            MdMathLerp(&angle, 90, -90, sys->procT);
        }
        for(i=0; i<4; i++) {
            if(playerType[i][0] != playerType[i][1]) {
                MdMathLerpScale(scaleP[i][playerType[i][1]], 1, 0, sys->procT);
                posP[i][playerType[i][1]]->x = oldPlayerPos3D[i].x+(radius*HuCos(angle));
                posP[i][playerType[i][1]]->y = oldPlayerPos3D[i].y;
                posP[i][playerType[i][1]]->z = oldPlayerPos3D[i].z+(radius*HuSin(angle));
                MdMathLerpScale(scaleP[i][playerType[i][0]], 0, 1, sys->procT);
                posP[i][playerType[i][0]]->x = oldPlayerPos3D[i].x+(radius*HuCos(180.0f+angle));
                posP[i][playerType[i][0]]->y = oldPlayerPos3D[i].y;
                posP[i][playerType[i][0]]->z = oldPlayerPos3D[i].z+(radius*HuSin(180.0f+angle));
            }
        }
        if(sys->maxPlayerNum > 1) {
            sys->cursorMode = MDMINI_CURSOR_PLAYERSEL;
            MdProcCall(MDMINI_PROC_CURSOR, sys->cursorDir+2);
        }
    }
    for(i=0; i<4; i++) {
        fn_1_15CF8(i+MDMODEL_CAP_MAN_1P, MDCAMERA_MAIN);
        fn_1_15CF8(i+MDMODEL_CAP_COM_1P, MDCAMERA_MAIN);
    }
    if(sys->procTime >= 1) {
        if(param != 0) {
            if(param == 1) {
                for(i=0; i<4; i++) {
                    Hu3DModelDispOff(model->mdlId[i+MDMODEL_CAP_MAN_1P+(playerType[i][0]*4)]);
                }
            } else if(param == 2 || param == 3) {
                for(i=0; i<4; i++) {
                    if(playerType[i][0] != playerType[i][1]) {
                        Hu3DModelDispOff(model->mdlId[i+MDMODEL_CAP_MAN_1P+(playerType[i][1]*4)]);
                    }
                }
            }
        }
        return TRUE;
    } else {
        return FALSE;
    }
}

BOOL fn_1_C8B4(int param)
{
    int i;
    MDMODEL_WORK *model;
    HuVecF *posP;
    HuVecF *rotP;
    HuVecF *scaleP;
    MDMINI_STAR *star;
    MDCAMERA_WORK *camera;

    static HuVecF centerPos = { 0, 0, -1000 };
    static HuVecF chrPos[10] = {
        { -440, 400, -600 },
        { -220, 400, -600 },
        { 0, 400, -600 },
        { 220, 400, -600 },
        { 440, 400, -600 },
        { -440, 130, -600 },
        { -220, 130, -600 },
        { 0, 130, -600 },
        { 220, 130, -600 },
        { 440, 130, -600 }
    };
    static HuVecF playerPos[4] = {
        { -360, -160, 250 },
        { -120, -160, 500 },
        { 120, -160, 750 },
        { 360, -160, 1000 }
    };
    static HuVecF teamPlayerPos[4] = {
        { -460, 120, -600 },
        { -220, 120, -600 },
        { 220, 120, -600 },
        { 460, 120, -600 }
    };
    static HuVec2f cursorTeamPos[4] = {
        { -130, -900 },
        { 260, -1100 },
        { -30, 900 },
        { 80, 1100 }
    };
    static HuVecF handicapPos[4] = {
        { -460, 120, -600 },
        { -20, 120, -600 },
        { 220, 120, -600 },
        { 460, 120, -600 }
    };

    camera = sys->camera;
    model = sys->model;
    star = sys->star;
    if(!sys->dirtyF) {
        if(param == 0) {
            for(i=0; i<10; i++) {
                posP = model->posP[i+MDMODEL_CAP_CHR_00];
                rotP = model->rotP[i+MDMODEL_CAP_CHR_00];
                scaleP = model->scaleP[i+MDMODEL_CAP_CHR_00];
                *posP = centerPos;
                scaleP->x = 0;
                scaleP->y = 0;
                scaleP->z = 0;
                Hu3DModelDispOn(model->mdlId[i+MDMODEL_CAP_CHR_00]);
                Hu3DModelDispOn(model->mdlId[i+MDMODEL_CHR_00]);
            }
            Hu3DModelDispOn(model->mdlId[MDMODEL_STAGE_CHR]);
            Hu3DLayerHookSet(1+HU3D_LAYER_HOOK_POST, MdToueiLayerHook);
            fn_1_17008(MDMINI_AUD_SE_MENU_MOVE);
        } else if(param != 1) {
            if(param == 2) {
                star->mode = MDMINI_STAR_MOVE_LEFT_START;
                fn_1_17008(MDMINI_AUD_SE_MENU_MOVE);
            } else if(param == 3) {
                star->mode = MDMINI_STAR_CENTER_START;
                fn_1_17008(MDMINI_AUD_SE_MENU_MOVE);
            } else if(param == 4) {
                for(i=0; i<4; i++) {
                    posP = model->posP[sys->chrSel[i]+MDMODEL_CAP_CHR_00];
                    rotP = model->rotP[sys->chrSel[i]+MDMODEL_CAP_CHR_00];
                    scaleP = model->scaleP[sys->chrSel[i]+MDMODEL_CAP_CHR_00];
                    *posP = playerPos[i];
                    posP->z = chrPos[sys->chrSel[i]].z;
                    scaleP->x = 1;
                    scaleP->y = 1;
                    scaleP->z = 0.1f;
                    Hu3DModelDispOn(model->mdlId[sys->chrSel[i]+MDMODEL_CAP_CHR_00]);
                    Hu3DModelDispOn(model->mdlId[sys->chrSel[i]+MDMODEL_CHR_00]);
                }
                for(i=0; i<10; i++) {
                    posP = model->posP[i+MDMODEL_CAP_CHR_00];
                    rotP = model->rotP[i+MDMODEL_CAP_CHR_00];
                    scaleP = model->scaleP[i+MDMODEL_CAP_CHR_00];
                    if(i != sys->chrSel[0] && i != sys->chrSel[1] && i != sys->chrSel[2] && i != sys->chrSel[3]) {
                        *posP = chrPos[i];
                        scaleP->x = 0;
                        scaleP->y = 0;
                        scaleP->z = 0;
                        Hu3DModelDispOn(model->mdlId[i+MDMODEL_CAP_CHR_00]);
                        Hu3DModelDispOn(model->mdlId[i+MDMODEL_CHR_00]);
                    }
                }
                Hu3DModelDispOn(model->mdlId[MDMODEL_STAGE_CHR]);
                Hu3DLayerHookSet(1+HU3D_LAYER_HOOK_POST, MdToueiLayerHook);
                sys->procTime = 1;
            } else if(param == 5) {

            } else if(param == 6) {
                fn_1_17008(MDMINI_AUD_SE_MENU_MOVE);
            } else if(param == 7) {
                fn_1_17008(MDMINI_AUD_SE_MENU_MOVE);
            } else if(param == 8) {

            } else if(param == 9) {
                fn_1_17008(MDMINI_AUD_SE_MENU_MOVE);
            } else if(param == 10) {
                fn_1_17008(MDMINI_AUD_SE_MENU_MOVE);
            }
        }
        sys->dirtyF = TRUE;
    }
    if(param == 0) {
        MdMathOscillate(MDMATH_OSC_90, &sys->procTime, &sys->procT, 30);
        for(i=0; i<10; i++) {
            posP = model->posP[i+MDMODEL_CAP_CHR_00];
            rotP = model->rotP[i+MDMODEL_CAP_CHR_00];
            scaleP = model->scaleP[i+MDMODEL_CAP_CHR_00];
            MdMathLerp(&posP->x, centerPos.x, chrPos[i].x, sys->procT);
            MdMathLerp(&posP->y, centerPos.y, chrPos[i].y, sys->procT);
            MdMathLerp(&posP->z, centerPos.z, chrPos[i].z, sys->procT);
            MdMathLerp(&scaleP->x, 0, 1, sys->procT);
            MdMathLerp(&scaleP->y, 0, 1, sys->procT);
            MdMathLerp(&scaleP->z, 0, 0.1f, sys->procT);
        }
    } else if(param == 1) {
        MdMathOscillate(MDMATH_OSC_90_REV, &sys->procTime, &sys->procT, 15);
        for(i=0; i<10; i++) {
            posP = model->posP[i+MDMODEL_CAP_CHR_00];
            rotP = model->rotP[i+MDMODEL_CAP_CHR_00];
            scaleP = model->scaleP[i+MDMODEL_CAP_CHR_00];
            MdMathLerp(&scaleP->x, 1, 0, sys->procT);
            MdMathLerp(&scaleP->y, 1, 0, sys->procT);
            MdMathLerp(&scaleP->z, 0.1f, 0, sys->procT);
        }
    } else if(param == 2) {
        MdMathOscillate(MDMATH_OSC_90_REV, &sys->procTime, &sys->procT, 30);
        MdMathOscillate(MDMATH_OSC_180, &sys->procOfsTime, &sys->procOfsT, 30);
        for(i=0; i<4; i++) {
            posP = model->posP[sys->chrSel[i]+MDMODEL_CAP_CHR_00];
            rotP = model->rotP[sys->chrSel[i]+MDMODEL_CAP_CHR_00];
            scaleP = model->scaleP[sys->chrSel[i]+MDMODEL_CAP_CHR_00];
            MdMathLerp(&posP->x, chrPos[sys->chrSel[i]].x, playerPos[i].x, sys->procT);
            MdMathLerp(&posP->y, chrPos[sys->chrSel[i]].y, playerPos[i].y, sys->procT);
            MdMathLerp(&posP->z, chrPos[sys->chrSel[i]].z, playerPos[i].z, sys->procOfsT);
        }
        for(i=0; i<10; i++) {
            posP = model->posP[i+MDMODEL_CAP_CHR_00];
            rotP = model->rotP[i+MDMODEL_CAP_CHR_00];
            scaleP = model->scaleP[i+MDMODEL_CAP_CHR_00];
            if(i != sys->chrSel[0] && i != sys->chrSel[1] && i != sys->chrSel[2] && i != sys->chrSel[3]) {
                MdMathLerp(&scaleP->x, 1, 0, sys->procT);
                MdMathLerp(&scaleP->y, 1, 0, sys->procT);
                MdMathLerp(&scaleP->z, 0.1f, 0, sys->procT);
            }
        }
        fn_1_153D0();
    } else if(param == 3) {
        MdMathOscillate(MDMATH_OSC_90_REV, &sys->procTime, &sys->procT, 30);
        MdMathOscillate(MDMATH_OSC_180, &sys->procOfsTime, &sys->procOfsT, 30);
        for(i=0; i<4; i++) {
            posP = model->posP[sys->chrSel[i]+MDMODEL_CAP_CHR_00];
            rotP = model->rotP[sys->chrSel[i]+MDMODEL_CAP_CHR_00];
            scaleP = model->scaleP[sys->chrSel[i]+MDMODEL_CAP_CHR_00];
            MdMathLerp(&posP->x, playerPos[i].x, chrPos[sys->chrSel[i]].x, sys->procT);
            MdMathLerp(&posP->y, playerPos[i].y, chrPos[sys->chrSel[i]].y, sys->procT);
            MdMathLerp(&posP->z, chrPos[sys->chrSel[i]].z, playerPos[i].z, sys->procOfsT);
        }
        for(i=0; i<10; i++) {
            posP = model->posP[i+MDMODEL_CAP_CHR_00];
            rotP = model->rotP[i+MDMODEL_CAP_CHR_00];
            scaleP = model->scaleP[i+MDMODEL_CAP_CHR_00];
            if(i != sys->chrSel[0] && i != sys->chrSel[1] && i != sys->chrSel[2] && i != sys->chrSel[3]) {
                MdMathLerp(&scaleP->x, 0, 1, sys->procT);
                MdMathLerp(&scaleP->y, 0, 1, sys->procT);
                MdMathLerp(&scaleP->z, 0, 0.1f, sys->procT);
            }
        }
        fn_1_153D0();
    } else if(param == 4) {

    } else if(param == 5) {
        MdMathOscillate(MDMATH_OSC_90_REV, &sys->procTime, &sys->procT, 30);
        MdMathOscillate(MDMATH_OSC_180, &sys->procOfsTime, &sys->procOfsT, 30);
        for(i=0; i<4; i++) {
            posP = model->posP[sys->chrSel[i]+MDMODEL_CAP_CHR_00];
            rotP = model->rotP[sys->chrSel[i]+MDMODEL_CAP_CHR_00];
            scaleP = model->scaleP[sys->chrSel[i]+MDMODEL_CAP_CHR_00];
            MdMathLerp(&posP->x, playerPos[i].x, teamPlayerPos[i].x, sys->procT);
            MdMathLerp(&posP->y, playerPos[i].y, teamPlayerPos[i].y, sys->procT);
            MdMathLerp(&posP->z, teamPlayerPos[i].z, playerPos[i].z, sys->procOfsT);
        }
        MdProcCall(MDMINI_PROC_9, 0);
        fn_1_153D0();
    } else if(param == 6) {
        MdMathOscillate(MDMATH_OSC_90_REV, &sys->procTime, &sys->procT, 30);
        MdMathOscillate(MDMATH_OSC_180, &sys->procOfsTime, &sys->procOfsT, 30);
        for(i=0; i<4; i++) {
            posP = model->posP[sys->chrSel[i]+MDMODEL_CAP_CHR_00];
            rotP = model->rotP[sys->chrSel[i]+MDMODEL_CAP_CHR_00];
            scaleP = model->scaleP[sys->chrSel[i]+MDMODEL_CAP_CHR_00];
            MdMathLerp(&posP->x, teamPlayerPos[sys->teamOrder[i]].x, playerPos[i].x, sys->procT);
            MdMathLerp(&posP->y, teamPlayerPos[sys->teamOrder[i]].y, playerPos[i].y, sys->procT);
            MdMathLerp(&posP->z, teamPlayerPos[sys->teamOrder[i]].z, playerPos[i].z, sys->procOfsT);
        }
        MdProcCall(MDMINI_PROC_9, 1);
        fn_1_153D0();
    } else if(param == 7) {
        MdMathOscillate(MDMATH_OSC_90_REV, &sys->procTime, &sys->procT, 15);
        MdMathOscillate(MDMATH_OSC_180, &sys->procOfsTime, &sys->procOfsT, 15);
        for(i=0; i<4; i++) {
            if(sys->teamOrder[i] != sys->teamOrderBackup[i]) {
                posP = model->posP[sys->chrSel[i]+MDMODEL_CAP_CHR_00];
                scaleP = model->scaleP[sys->chrSel[i]+MDMODEL_CAP_CHR_00];
                MdMathLerp(&posP->x, teamPlayerPos[sys->teamOrderBackup[i]].x, teamPlayerPos[sys->teamOrder[i]].x, sys->procT);
                MdMathLerp(&posP->y, teamPlayerPos[sys->teamOrderBackup[i]].y, cursorTeamPos[sys->teamOrder[i]].x, sys->procOfsT);
                MdMathLerp(&posP->z, -600, cursorTeamPos[sys->teamOrder[i]].y, sys->procOfsT);
            }
        }
        fn_1_153D0();
        MdProcCall(MDMINI_PROC_CURSOR, sys->cursorDir+22);
    } else if(param == 8) {
        MdMathOscillate(MDMATH_OSC_90_REV, &sys->procTime, &sys->procT, 30);
        MdMathOscillate(MDMATH_OSC_180, &sys->procOfsTime, &sys->procOfsT, 30);
        for(i=0; i<4; i++) {
            posP = model->posP[sys->chrSel[i]+MDMODEL_CAP_CHR_00];
            rotP = model->rotP[sys->chrSel[i]+MDMODEL_CAP_CHR_00];
            scaleP = model->scaleP[sys->chrSel[i]+MDMODEL_CAP_CHR_00];
            MdMathLerp(&posP->x, playerPos[i].x, handicapPos[i].x, sys->procT);
            MdMathLerp(&posP->y, playerPos[i].y, handicapPos[i].y, sys->procT);
            MdMathLerp(&posP->z, handicapPos[i].z, playerPos[i].z, sys->procOfsT);
        }
        MdProcCall(MDMINI_PROC_9, 0);
        fn_1_153D0();
    } else if(param == 9) {
        MdMathOscillate(MDMATH_OSC_90_REV, &sys->procTime, &sys->procT, 30);
        MdMathOscillate(MDMATH_OSC_180, &sys->procOfsTime, &sys->procOfsT, 30);
        for(i=0; i<4; i++) {
            posP = model->posP[sys->chrSel[i]+MDMODEL_CAP_CHR_00];
            rotP = model->rotP[sys->chrSel[i]+MDMODEL_CAP_CHR_00];
            scaleP = model->scaleP[sys->chrSel[i]+MDMODEL_CAP_CHR_00];
            MdMathLerp(&posP->x, handicapPos[sys->handicapOrder[i]].x, playerPos[i].x, sys->procT);
            MdMathLerp(&posP->y, handicapPos[sys->handicapOrder[i]].y, playerPos[i].y, sys->procT);
            MdMathLerp(&posP->z, handicapPos[sys->handicapOrder[i]].z, playerPos[i].z, sys->procOfsT);
        }
        MdProcCall(MDMINI_PROC_9, 1);
        fn_1_153D0();
    } else if(param == 10) {
        MdMathOscillate(MDMATH_OSC_90_REV, &sys->procTime, &sys->procT, 15);
        MdMathOscillate(MDMATH_OSC_180, &sys->procOfsTime, &sys->procOfsT, 15);
        for(i=0; i<4; i++) {
            if(sys->handicapOrder[i] != sys->handicapOrderBackup[i]) {
                posP = model->posP[sys->chrSel[i]+MDMODEL_CAP_CHR_00];
                scaleP = model->scaleP[sys->chrSel[i]+MDMODEL_CAP_CHR_00];
                MdMathLerp(&posP->x, handicapPos[sys->handicapOrderBackup[i]].x, handicapPos[sys->handicapOrder[i]].x, sys->procT);
                MdMathLerp(&posP->y, handicapPos[sys->handicapOrderBackup[i]].y, cursorTeamPos[sys->handicapOrder[i]].x, sys->procOfsT);
                MdMathLerp(&posP->z, -600, cursorTeamPos[sys->handicapOrder[i]].y, sys->procOfsT);
            }
        }
        fn_1_153D0();
        MdProcCall(MDMINI_PROC_CURSOR, sys->cursorDir+22);
    }
    if(sys->procTime >= 1) {
        return TRUE;
    } else {
        return FALSE;
    }
}

typedef struct MdMiniModeAnim_s {
    s16 mode; //0x00
    s16 wait; //0x02
    float radius; //0x04
    float angle; //0x08
    float angleStart; //0x0C
    float angleEnd; //0x10
    float time[4]; //0x14
    float t[4]; //0x24
} MDMINI_MODEANIM;

BOOL fn_1_E44C(int param)
{
    MDMINI_MODEANIM *anim;
    int i;
    MDMODEL_WORK *model;
    HuVecF *posP;
    HuVecF *scaleP;
    HuVecF *rotP;
    MDCAMERA_WORK *camera;

    static s16 waitTbl[6] = { 30, 40, 50, 0, 10, 20 };
    static float angleTbl[6] = { 90, 30, -30, 270, 210, 150 };
    static float speedTbl[6] = { 45, 40, 45, 60, 55, 50 };

    static MDMINI_MODEANIM modeAnim[6];
    static HuVecF center;
    static HuVecF centerPos[2];
    static HuVecF selPos[2];

    camera = sys->camera;
    model = sys->model;
    if(!sys->dirtyF) {
        if(param == 0) {
            centerPos[0].x = 0;
            centerPos[0].y = 0;
            centerPos[0].z = -2000;
            centerPos[1].x = 0;
            centerPos[1].y = 180;
            centerPos[1].z = -200;
            center = centerPos[0];
            for(anim=&modeAnim[0], i=0; i<6; i++, anim++) {
                posP = model->posP[i+MDMODEL_CAP_MODE_00];
                rotP = model->rotP[i+MDMODEL_CAP_MODE_00];
                scaleP = model->scaleP[i+MDMODEL_CAP_MODE_00];
                anim->mode = 0;
                anim->wait = waitTbl[i];
                anim->radius = 0;
                anim->angle = -90;
                anim->angleStart = anim->angle;
                anim->angleEnd = angleTbl[i];
                anim->time[0] = 0;
                anim->time[1] = 0;
                anim->time[2] = 0;
                anim->time[3] = 0;
                anim->t[0] = 0;
                anim->t[1] = 0;
                anim->t[2] = 0;
                anim->t[3] = 0;
                posP->x = center.x+anim->radius*cos(3.141592653589793*anim->angle/180.0);
                posP->y = center.y;
                posP->z = center.z+anim->radius*sin(3.141592653589793*anim->angle/180.0);
                rotP->x = 0;
                rotP->y = 0;
                rotP->z = 0;
                scaleP->x = 0;
                scaleP->y = 0;
                scaleP->z = 0;
                Hu3DModelDispOn(model->mdlId[i+MDMODEL_CAP_MODE_00]);
            }
            fn_1_17008(MDMINI_AUD_SE_MENU_MOVE);
        } else if(param == 1) {
            for(anim=&modeAnim[0], i=0; i<6; i++, anim++) {
                anim->mode = 0;
                anim->angleStart = anim->angle;
                if(sys->cursorDir == 0) {
                    anim->angleEnd = 60+anim->angle;
                } else if(sys->cursorDir == 1) {
                    anim->angleEnd = anim->angle-60;
                }
                anim->time[1] = 0;
                anim->t[1] = 0;
            }
        } else if(param == 2) {
            for(anim=&modeAnim[0], i=0; i<6; i++, anim++) {
                anim->mode = 0;
                anim->time[0] = 0;
                anim->time[1] = 0;
                anim->time[2] = 0;
                anim->time[3] = 0;
                anim->t[0] = 0;
                anim->t[1] = 0;
                anim->t[2] = 0;
                anim->t[3] = 0;
            }
        } else if(param == 3) {
            for(anim=&modeAnim[0], i=0; i<6; i++, anim++) {
                anim->mode = 0;
                anim->time[0] = 0;
                anim->time[1] = 0;
                anim->time[2] = 0;
                anim->time[3] = 0;
                anim->t[0] = 0;
                anim->t[1] = 0;
                anim->t[2] = 0;
                anim->t[3] = 0;
            }
            selPos[0] = *model->posP[sys->selNoNow+MDMODEL_CAP_MODE_00];
            selPos[1].x = 0;
            selPos[1].y = -50;
            selPos[1].z = 500;
        } else if(param == 4) {
            for(anim=&modeAnim[0], i=0; i<6; i++, anim++) {
                anim->mode = 0;
                anim->time[0] = 0;
                anim->time[1] = 0;
                anim->time[2] = 0;
                anim->time[3] = 0;
                anim->t[0] = 0;
                anim->t[1] = 0;
                anim->t[2] = 0;
                anim->t[3] = 0;
                if(i != sys->selNoNow) {
                    Hu3DModelDispOn(model->mdlId[i+MDMODEL_CAP_MODE_00]);
                }
            }
        }
        sys->dirtyF = TRUE;
    }
    if(param == 0) {
        MdMathOscillate(MDMATH_OSC_90, &sys->procTime, &sys->procT, 30);
        MdMathLerp(&center.x, centerPos[0].x, centerPos[1].x, sys->procT);
        MdMathLerp(&center.y, centerPos[0].y, centerPos[1].y, sys->procT);
        MdMathLerp(&center.z, centerPos[0].z, centerPos[1].z, sys->procT);
        for(anim=&modeAnim[0], i=0; i<6; i++, anim++) {
            posP = model->posP[i+MDMODEL_CAP_MODE_00];
            rotP = model->rotP[i+MDMODEL_CAP_MODE_00];
            scaleP = model->scaleP[i+MDMODEL_CAP_MODE_00];
            switch(anim->mode) {
                case 0:
                    if(--anim->wait <= 0) {
                        anim->wait = 0;
                        anim->mode = 1;
                    }
                    break;

                case 1:
                    MdMathOscillate(MDMATH_OSC_90, &anim->time[0], &anim->t[0], speedTbl[i]);
                    MdMathLerp(&anim->radius, 0, 300, anim->t[0]);
                    MdMathOscillate(MDMATH_OSC_90, &anim->time[1], &anim->t[1], speedTbl[i]);
                    MdMathLerp(&anim->angle, anim->angleStart, anim->angleEnd, anim->t[1]);
                    posP->x = center.x+anim->radius*cos(3.141592653589793*anim->angle/180.0);
                    posP->y = center.y;
                    posP->z = center.z+anim->radius*sin(3.141592653589793*anim->angle/180.0);
                    MdMathOscillate(MDMATH_OSC_90, &anim->time[3], &anim->t[3], speedTbl[i]);
                    if(i == sys->selNoNow) {
                        MdMathLerpScale(scaleP, 0, 1.5f, anim->t[3]);
                    } else {
                        MdMathLerpScale(scaleP, 0, 0.5f, anim->t[3]);
                    }
                    if(anim->time[1] >= 1) {
                        MdMathAngleClamp(&anim->angle);
                        anim->mode++;
                    }
                    break;

                case 2:
                    break;
            }
        }
    } else if(param == 1) {
        for(anim=&modeAnim[0], i=0; i<6; i++, anim++) {
            posP = model->posP[i+MDMODEL_CAP_MODE_00];
            rotP = model->rotP[i+MDMODEL_CAP_MODE_00];
            scaleP = model->scaleP[i+MDMODEL_CAP_MODE_00];
            switch(anim->mode) {
                case 1:
                    break;

                case 0:
                    MdMathOscillate(MDMATH_OSC_90, &anim->time[1], &anim->t[1], 15);
                    MdMathLerp(&anim->angle, anim->angleStart, anim->angleEnd, anim->t[1]);
                    posP->x = center.x+anim->radius*cos(3.141592653589793*anim->angle/180.0);
                    posP->y = center.y;
                    posP->z = center.z+anim->radius*sin(3.141592653589793*anim->angle/180.0);
                    if(i == sys->selNoNow) {
                        MdMathLerpScale(scaleP, 0.5f, 1.5f, anim->t[1]);
                    } else if(i == sys->selNoPrev) {
                        MdMathLerpScale(scaleP, 1.5f, 0.5f, anim->t[1]);
                    }
                    if(anim->time[1] >= 1) {
                        MdMathAngleClamp(&anim->angle);
                        anim->mode++;
                    }
                    break;
            }
        }
        if(sys->cursorDir == 0) {
            MdProcCall(MDMINI_PROC_CURSOR, 11);
        } else if(sys->cursorDir == 1) {
            MdProcCall(MDMINI_PROC_CURSOR, 10);
        }
    } else if(param == 2) {
        for(anim=&modeAnim[0], i=0; i<6; i++, anim++) {
            posP = model->posP[i+MDMODEL_CAP_MODE_00];
            rotP = model->rotP[i+MDMODEL_CAP_MODE_00];
            scaleP = model->scaleP[i+MDMODEL_CAP_MODE_00];
            switch(anim->mode) {
                case 1:
                    break;

                case 0:
                    MdMathOscillate(MDMATH_OSC_90, &anim->time[3], &anim->t[3], 15);
                    MdMathLerpScale(scaleP, 1, 0, anim->t[3]);
                    if(anim->time[3] >= 1) {
                        anim->mode++;
                    }
                    break;
            }
        }
    } else if(param == 3) {
        for(anim=&modeAnim[0], i=0; i<6; i++, anim++) {
            posP = model->posP[i+MDMODEL_CAP_MODE_00];
            rotP = model->rotP[i+MDMODEL_CAP_MODE_00];
            scaleP = model->scaleP[i+MDMODEL_CAP_MODE_00];
            if(i == sys->selNoNow) {
                switch(anim->mode) {
                    case 1:
                        break;

                    case 0:
                        MdMathOscillate(MDMATH_OSC_90, &anim->time[0], &anim->t[0], 15);
                        MdMathOscillate(MDMATH_OSC_180, &anim->time[1], &anim->t[1], 15);
                        MdMathLerpScaleOfs(scaleP, 1.5f, 2, anim->t[0], 2*anim->t[1]);
                        if(anim->time[0] >= 1) {
                            anim->mode++;
                        }
                        break;
                }
            } else {
                switch(anim->mode) {
                    case 1:
                        break;

                    case 0:
                        MdMathOscillate(MDMATH_OSC_90, &anim->time[0], &anim->t[0], 15);
                        MdMathLerpScale(scaleP, 0.5f, 0, anim->t[0]);
                        if(anim->time[0] >= 1) {
                            anim->mode++;
                        }
                        break;
                }
            }
        }
    } else if(param == 4) {
        for(anim=&modeAnim[0], i=0; i<6; i++, anim++) {
            posP = model->posP[i+MDMODEL_CAP_MODE_00];
            rotP = model->rotP[i+MDMODEL_CAP_MODE_00];
            scaleP = model->scaleP[i+MDMODEL_CAP_MODE_00];
            if(i == sys->selNoNow) {
                switch(anim->mode) {
                    case 1:
                        break;

                    case 0:
                        MdMathOscillate(MDMATH_OSC_90, &anim->time[0], &anim->t[0], 15);
                        MdMathLerpScale(scaleP, 2, 1.5f, anim->t[0]);
                        if(anim->time[0] >= 1) {
                            anim->mode++;
                        }
                        break;
                }
            } else {
                switch(anim->mode) {
                    case 1:
                        break;

                    case 0:
                        MdMathOscillate(MDMATH_OSC_90, &anim->time[0], &anim->t[0], 15);
                        MdMathLerpScale(scaleP, 0, 0.5f, anim->t[0]);
                        if(anim->time[0] >= 1) {
                            anim->mode++;
                        }
                        break;
                }
            }
        }
    }
    if(param == 0) {
        if(modeAnim[2].mode == 2) {
            return TRUE;
        } else {
            return FALSE;
        }
    } else if(param == 1) {
        if(modeAnim[5].mode == 1) {
            return TRUE;
        } else {
            return FALSE;
        }
    } else if(param == 2) {
        if(modeAnim[5].mode == 1) {
            return TRUE;
        } else {
            return FALSE;
        }
    } else if(param == 3) {
        if(modeAnim[sys->selNoNow].mode == 1) {
            for(i=0; i<6; i++) {
                if(i != sys->selNoNow) {
                    Hu3DModelDispOff(model->mdlId[i+MDMODEL_CAP_MODE_00]);
                }
            }
            return TRUE;
        } else {
            return FALSE;
        }
    } else if(param == 4) {
        if(modeAnim[sys->selNoNow].mode == 1) {
            return TRUE;
        } else {
            return FALSE;
        }
    }
#ifdef TARGET_PC
    return FALSE;
#endif
}

BOOL fn_1_F6C4(int param)
{
    HuVecF *posP;
    MDMODEL_WORK *model;
    HuVecF *scaleP;

    MDCAMERA_WORK *camera;
    HuVecF *rotP;

    static HuVecF posTbl[2];

    camera = sys->camera;
    model = sys->model;
    posP = model->posP[MDMODEL_CAP_WIN];
    rotP = model->rotP[MDMODEL_CAP_WIN];
    scaleP = model->scaleP[MDMODEL_CAP_WIN];

    if(!sys->dirtyF) {
        if(param == 0) {
            posP->x = 0;
            posP->y = 145;
            posP->z = 400;

            scaleP->x = 0;
            scaleP->y = 0;
            scaleP->z = 0;
            Hu3DModelDispOn(model->mdlId[MDMODEL_CAP_WIN]);
        } else if(param != 1) {
            if(param == 2) {
            } else if(param == 3) {
                posTbl[0] = *posP;
                posTbl[1].x = -525;
                posTbl[1].y = 365;
                posTbl[1].z = -50;
                fn_1_17008(MDMINI_AUD_SE_MENU_MOVE);
            } else if(param == 4) {
                fn_1_17008(MDMINI_AUD_SE_MENU_MOVE);
            }
        }
        sys->dirtyF = TRUE;
    }
    if(param == 0) {
        MdMathOscillate(MDMATH_OSC_90_REV, &sys->procTime, &sys->procT, 15.0f);
        MdMathLerpScale(scaleP, 0, 1, sys->procT);
    } else if(param == 1) {
        MdMathOscillate(MDMATH_OSC_90_REV, &sys->procTime, &sys->procT, 15.0f);
        MdProcCall(MDMINI_PROC_CURSOR, sys->cursorDir+14);
    } else if(param == 2) {
        MdMathOscillate(MDMATH_OSC_90, &sys->procTime, &sys->procT, 15.0f);
        MdMathLerpScale(scaleP, 1, 0, sys->procT);
    } else if(param == 3) {
        MdMathOscillate(MDMATH_OSC_90_REV, &sys->procTime, &sys->procT, 15.0f);
        MdMathOscillate(MDMATH_OSC_180, &sys->procOfsTime, &sys->procOfsT, 15.0f);
        MdMathLerp(&posP->x, posTbl[0].x, posTbl[1].x, sys->procT);
        MdMathLerp(&posP->y, posTbl[0].y, posTbl[1].y, sys->procT);
        MdMathLerpOfs(&posP->z, posTbl[0].z, posTbl[1].z, sys->procT, -1.0f*sys->procOfsT);
        MdMathLerpScale(scaleP, 1, 0.8f, sys->procT);
    } else if(param == 4) {
        MdMathOscillate(MDMATH_OSC_90, &sys->procTime, &sys->procT, 15.0f);
        MdMathOscillate(MDMATH_OSC_180, &sys->procOfsTime, &sys->procOfsT, 15.0f);
        MdMathLerp(&posP->x, posTbl[1].x, posTbl[0].x, sys->procT);
        MdMathLerp(&posP->y, posTbl[1].y, posTbl[0].y, sys->procT);
        MdMathLerpOfs(&posP->z, posTbl[1].z, posTbl[0].z, sys->procT, sys->procOfsT);
        MdMathLerpScale(scaleP, 0.8f, 1, sys->procT);
    }
    fn_1_15CF8(MDMODEL_CAP_WIN, MDCAMERA_MAIN);
    if(sys->procTime >= 1) {
        return TRUE;
    } else {
        return FALSE;
    }
}

BOOL fn_1_FC94(int param)
{
    HuVecF *posP;
    MDMODEL_WORK *model;
    HuVecF *scaleP;

    MDCAMERA_WORK *camera;
    HuVecF *rotP;

    static HuVecF posTbl[2];

    camera = sys->camera;
    model = sys->model;
    posP = model->posP[MDMODEL_CAP_GENRE];
    rotP = model->rotP[MDMODEL_CAP_GENRE];
    scaleP = model->scaleP[MDMODEL_CAP_GENRE];

    if(!sys->dirtyF) {
        if(param == 0) {
            posP->x = 0;
            posP->y = 145;
            posP->z = 400;

            scaleP->x = 0;
            scaleP->y = 0;
            scaleP->z = 0;
            Hu3DModelDispOn(model->mdlId[MDMODEL_CAP_GENRE]);
        } else if(param != 1) {
            if(param == 2) {
            } else if(param == 3) {
                posTbl[0] = *posP;
                posTbl[1].x = 525;
                posTbl[1].y = 365;
                posTbl[1].z = -50;
                fn_1_17008(MDMINI_AUD_SE_MENU_MOVE);
            } else if(param == 4) {
                fn_1_17008(MDMINI_AUD_SE_MENU_MOVE);
            }
        }
        sys->dirtyF = TRUE;
    }
    if(param == 0) {
        MdMathOscillate(MDMATH_OSC_90_REV, &sys->procTime, &sys->procT, 15.0f);
        MdMathLerpScale(scaleP, 0, 1, sys->procT);
    } else if(param == 1) {
        MdMathOscillate(MDMATH_OSC_90_REV, &sys->procTime, &sys->procT, 15.0f);
        MdProcCall(MDMINI_PROC_CURSOR, sys->cursorDir+18);
    } else if(param == 2) {
        MdMathOscillate(MDMATH_OSC_90, &sys->procTime, &sys->procT, 15.0f);
        MdMathLerpScale(scaleP, 1, 0, sys->procT);
    } else if(param == 3) {
        MdMathOscillate(MDMATH_OSC_90_REV, &sys->procTime, &sys->procT, 15.0f);
        MdMathOscillate(MDMATH_OSC_180, &sys->procOfsTime, &sys->procOfsT, 15.0f);
        MdMathLerp(&posP->x, posTbl[0].x, posTbl[1].x, sys->procT);
        MdMathLerp(&posP->y, posTbl[0].y, posTbl[1].y, sys->procT);
        MdMathLerpOfs(&posP->z, posTbl[0].z, posTbl[1].z, sys->procT, -1.0f*sys->procOfsT);
        MdMathLerpScale(scaleP, 1, 0.8f, sys->procT);
    } else if(param == 4) {
        MdMathOscillate(MDMATH_OSC_90, &sys->procTime, &sys->procT, 15.0f);
        MdMathOscillate(MDMATH_OSC_180, &sys->procOfsTime, &sys->procOfsT, 15.0f);
        MdMathLerp(&posP->x, posTbl[1].x, posTbl[0].x, sys->procT);
        MdMathLerp(&posP->y, posTbl[1].y, posTbl[0].y, sys->procT);
        MdMathLerpOfs(&posP->z, posTbl[1].z, posTbl[0].z, sys->procT, sys->procOfsT);
        MdMathLerpScale(scaleP, 0.8f, 1, sys->procT);
    }
    fn_1_15CF8(MDMODEL_CAP_GENRE, MDCAMERA_MAIN);
    if(sys->procTime >= 1) {
        return TRUE;
    } else {
        return FALSE;
    }
}

BOOL fn_1_10264(int param)
{
    HuVecF *posP;
    MDMODEL_WORK *model;
    HuVecF *rotP;
    HuVecF *scaleP;

    MDCAMERA_WORK *camera;

    static HuVecF mdlPos[5] = {
        { 0, 150, 500 },
        { 0, 95, 0 },
        { 0, 150, -500 },
        { 0, -120, 0 },
        { 0, 75, 0 }
    };
    static HuVecF mdlRot[3] = {
        { 60, 0, 270 },
        { 0, 0, 0 },
        { 0, -270, 0 }
    };

    camera = sys->camera;
    model = sys->model;
    posP = model->posP[MDMODEL_VS];
    rotP = model->rotP[MDMODEL_VS];
    scaleP = model->scaleP[MDMODEL_VS];
    if(!sys->vsInitF) {
        if(param == 0) {
            *posP = mdlPos[0];
            if(sys->gameType == 2) {
                posP->x = -15;
            } else if(sys->gameType == 1) {
                posP->x = -200;
            }
            *rotP = mdlRot[0];
            scaleP->x = 3;
            scaleP->y = 3;
            scaleP->z = 3;
            Hu3DModelDispOn(model->mdlId[MDMODEL_VS]);
        } else {
            param == 1;
        }
        sys->vsInitF = TRUE;
    }
    if(param == 0) {
        MdMathOscillate(MDMATH_OSC_90, &sys->unk400, &sys->unk41C, 30);
        MdMathLerp(&posP->y, mdlPos[0].y, mdlPos[1].y, sys->unk41C);
        MdMathLerp(&posP->z, mdlPos[0].z, mdlPos[1].z, sys->unk41C);
        MdMathLerp(&rotP->x, mdlRot[0].x, mdlRot[1].x, sys->unk41C);
        MdMathLerp(&rotP->y, mdlRot[0].y, mdlRot[1].y, sys->unk41C);
        MdMathLerp(&rotP->z, mdlRot[0].z, mdlRot[1].z, sys->unk41C);
        MdMathLerpScale(scaleP, 3, 1, sys->unk41C);
    } else if(param == 1) {
        MdMathOscillate(MDMATH_OSC_90, &sys->unk400, &sys->unk41C, 30);
        MdMathLerp(&posP->y, mdlPos[1].y, mdlPos[2].y, sys->unk41C);
        MdMathLerp(&posP->z, mdlPos[1].z, mdlPos[2].z, sys->unk41C);
        MdMathLerp(&rotP->x, mdlRot[1].x, mdlRot[2].x, sys->unk41C);
        MdMathLerp(&rotP->y, mdlRot[1].y, mdlRot[2].y, sys->unk41C);
        MdMathLerp(&rotP->z, mdlRot[1].z, mdlRot[2].z, sys->unk41C);
        MdMathLerpScale(scaleP, 1, 0, sys->unk41C);
    }
    if(sys->unk400 >= 1) {
        return TRUE;
    } else {
        return FALSE;
    }
}

BOOL fn_1_106D4(int param)
{
    MDCAMERA_WORK *camera;
    MDMODEL_WORK *model;
    HuVecF *scaleP;
    int i;

    camera = sys->camera;
    model = sys->model;
    if(!sys->dirtyF) {
        sys->dirtyF = TRUE;
    }
    if(param == 0) {
        MdMathOscillate(MDMATH_OSC_90, &sys->procTime, &sys->procT, 30);
        scaleP = model->scaleP[sys->selNoNow+MDMODEL_CAP_MODE_00];
        MdMathLerpScale(scaleP, 2, 0, sys->procT);
        for(i=0; i<4; i++) {
            scaleP = model->scaleP[sys->chrSel[i]+MDMODEL_CAP_CHR_00];
            MdMathLerp(&scaleP->x, 1, 0, sys->procT);
            MdMathLerp(&scaleP->y, 1, 0, sys->procT);
            MdMathLerp(&scaleP->z, 0.1f, 0, sys->procT);
        }
    } else if(param == 1) {
        MdMathOscillate(MDMATH_OSC_90, &sys->procTime, &sys->procT, 30);
        MdMathLerpScale(model->scaleP[MDMODEL_CAP_WIN], 1, 0, sys->procT);
        MdMathLerpScale(model->scaleP[MDMODEL_CAP_GENRE], 1, 0, sys->procT);
        scaleP = model->scaleP[sys->selNoNow+MDMODEL_CAP_MODE_00];
        MdMathLerpScale(scaleP, 2, 0, sys->procT);
        for(i=0; i<4; i++) {
            scaleP = model->scaleP[sys->chrSel[i]+MDMODEL_CAP_CHR_00];
            MdMathLerp(&scaleP->x, 1, 0, sys->procT);
            MdMathLerp(&scaleP->y, 1, 0, sys->procT);
            MdMathLerp(&scaleP->z, 0.1f, 0, sys->procT);
        }
        if(sys->gameType != 0) {
            MdMathLerpScale(model->scaleP[MDMODEL_VS], 1, 0, sys->procT);
        }
    }
    if(sys->procTime >= 1) {
        return TRUE;
    } else {
        return FALSE;
    }
}

BOOL fn_1_10B0C(int param)
{
    MDCAMERA_WORK *camera;
    MDSPRITE_WORK *sprite;
    MDMODEL_WORK *model;
    HuVecF *posP;
    HuVecF *scaleP;
    int i;

    static int frame;
    static s8 capDispF[2];
    static float lerpT[2];
    static float ofsTime[2];
    static float time[2];

    camera = sys->camera;
    sprite = sys->sprite;
    model = sys->model;
    if(!sys->dirtyF) {
        if(param == 0 || param == 1) {
            for(i=0; i<2; i++) {
                time[i] = 0;
                ofsTime[i] = 0;
                capDispF[i] = FALSE;
            }
            frame = 0;
        }
        sys->dirtyF = TRUE;
    }
    if(param == 0) {
        MdMathOscillate(MDMATH_OSC_90, &sys->procTime, &sys->procT, 30);
        if(sys->gameType == 0) {
            for(i=0; i<4; i++) {
                MdMathLerp(&model->posP[sys->chrSel[i]+MDMODEL_CAP_CHR_00]->y, -160, 120, sys->procT);
            }
            fn_1_153D0();
        }
        frame++;
        if(frame >= 4) {
            capDispF[0] = TRUE;
        }
        if(frame >= 6) {
            capDispF[1] = TRUE;
        }
        if(capDispF[0]) {
            MdMathOscillate(MDMATH_OSC_90, &time[0], &lerpT[0], 20);
            MdMathOscillate(MDMATH_OSC_180, &ofsTime[0], &lerpT[1], 20);
            posP = model->posP[MDMODEL_CAP_WIN];
            scaleP = model->scaleP[MDMODEL_CAP_WIN];
            MdMathLerp(&posP->x, -525, -380, lerpT[0]);
            MdMathLerp(&posP->y, 365, 340, lerpT[0]);
            MdMathLerpOfs(&posP->z, -50, 0, lerpT[0], 2.0f*lerpT[1]);
            MdMathLerpScale(scaleP, 0.8f, 1, lerpT[0]);
        }
        if(capDispF[1]) {
            MdMathOscillate(MDMATH_OSC_90, &time[1], &lerpT[0], 20);
            MdMathOscillate(MDMATH_OSC_180, &ofsTime[1], &lerpT[1], 20);
            posP = model->posP[MDMODEL_CAP_GENRE];
            scaleP = model->scaleP[MDMODEL_CAP_GENRE];
            MdMathLerp(&posP->x, 525, 380, lerpT[0]);
            MdMathLerp(&posP->y, 365, 340, lerpT[0]);
            MdMathLerpOfs(&posP->z, -50, 0, lerpT[0], 2.0f*lerpT[1]);
            MdMathLerpScale(scaleP, 0.8f, 1, lerpT[0]);
        }
        fn_1_15CF8(MDMODEL_CAP_WIN, MDCAMERA_MAIN);
        fn_1_15CF8(MDMODEL_CAP_GENRE, MDCAMERA_MAIN);
    } else if(param == 1) {
        MdMathOscillate(MDMATH_OSC_90, &sys->procTime, &sys->procT, 30);
        if(sys->gameType == 0) {
            for(i=0; i<4; i++) {
                MdMathLerp(&model->posP[sys->chrSel[i]+MDMODEL_CAP_CHR_00]->y, 120, -160, sys->procT);
            }
            fn_1_153D0();
        }
        frame++;
        if(frame >= 6) {
            capDispF[0] = TRUE;
        }
        if(frame >= 4) {
            capDispF[1] = TRUE;
        }
        if(capDispF[0]) {
            MdMathOscillate(MDMATH_OSC_90, &time[0], &lerpT[0], 20);
            MdMathOscillate(MDMATH_OSC_180, &ofsTime[0], &lerpT[1], 20);
            posP = model->posP[MDMODEL_CAP_WIN];
            scaleP = model->scaleP[MDMODEL_CAP_WIN];
            MdMathLerp(&posP->x, -380, -525, lerpT[0]);
            MdMathLerp(&posP->y, 340, 365, lerpT[0]);
            MdMathLerpOfs(&posP->z, 0, -50, lerpT[0], 2.0f*lerpT[1]);
            MdMathLerpScale(scaleP, 1, 0.8f, lerpT[0]);
        }
        if(capDispF[1]) {
            MdMathOscillate(MDMATH_OSC_90, &time[1], &lerpT[0], 20);
            MdMathOscillate(MDMATH_OSC_180, &ofsTime[1], &lerpT[1], 20);
            posP = model->posP[MDMODEL_CAP_GENRE];
            scaleP = model->scaleP[MDMODEL_CAP_GENRE];
            MdMathLerp(&posP->x, 380, 525, lerpT[0]);
            MdMathLerp(&posP->y, 340, 365, lerpT[0]);
            MdMathLerpOfs(&posP->z, 0, -50, lerpT[0], 2.0f*lerpT[1]);
            MdMathLerpScale(scaleP, 1, 0.8f, lerpT[0]);
        }
        fn_1_15CF8(MDMODEL_CAP_WIN, MDCAMERA_MAIN);
        fn_1_15CF8(MDMODEL_CAP_GENRE, MDCAMERA_MAIN);
    }
    if(sys->procTime >= 1) {
        return TRUE;
    } else {
        return FALSE;
    }
}

BOOL fn_1_113D8(int param)
{
    int i; //r31
    MDMODEL_WORK *model; //r30
    
    HuVecF *posP[4]; //sp+0x2C
    HuVecF *rotP[4]; //sp+0x1C
    HuVecF *scaleP[4]; //sp+0xC
    MDCAMERA_WORK *camera; //sp+0x8
    
    camera = sys->camera;
    model = sys->model;
    for(i=0; i<4; i++) {
        posP[i] = model->posP[i+MDMODEL_FINGER_1P];
        rotP[i] = model->rotP[i+MDMODEL_FINGER_1P];
        scaleP[i] = model->scaleP[i+MDMODEL_FINGER_1P];
    }
    if(!sys->dirtyF) {
        if(param == 0) {
            for(i=0; i<4; i++) {
                if(sys->padEnable[i]) {
                    scaleP[i]->x = 0;
                    scaleP[i]->y = 0;
                    scaleP[i]->z = 0;
                    Hu3DModelDispOn(model->mdlId[i+MDMODEL_FINGER_1P]);
                }
            }
            fn_1_153D0();
        }
        sys->dirtyF = TRUE;
    }
    if(param == 0){ 
        MdMathOscillate(MDMATH_OSC_90_REV, &sys->procTime, &sys->procT, 15.0f);
        for(i=0; i<4; i++) {
            if(sys->padEnable[i]) {
                MdMathLerpScale(scaleP[i], 0, 4, sys->procT);
            }
        }
    }
    if(param == 0) {
        if(sys->procTime >= 1) {
            return TRUE;
        } else {
            return FALSE;
        }
    }
}

BOOL fn_1_11658(int param)
{
    int i;
    MDMODEL_WORK *model;
    HuVecF *posP;
    HuVecF *rotP;
    HuVecF *scaleP;
    MDSPRITE_WORK *sprite;
    MDCAMERA_WORK *camera;
    MDMINI_STAR *star;
    MDMESS *mess;

    static int mode;
    static MDMINI_EFFECT *effectP[4][2];
    static HuVecF effectPos[4][2];
    static HuVecF effectRot[4][2];
    static int enterTime;
    static float vsTime;
    static float vsT;

    camera = sys->camera;
    model = sys->model;
    sprite = sys->sprite;
    star = sys->star;
    mess = sys->mess;
    if(!sys->dirtyF) {
        if(param == 0 || param == 1) {
        }
        vsTime = 0;
        vsT = 0;
        mode = 0;
        sys->dirtyF = TRUE;
    }
    if(param == 0 || param == 1) {
        switch(mode) {
            case 0:
                for(i=0; i<4; i++) {
                    HuSprDispOff(sprite->grpId[MDSPR_GRP_PLAYERNO], i);
                    HuSprDispOff(sprite->grpId[MDSPR_GRP_COMDIF], i);
                }
                for(i=0; i<4; i++) {
                    MdRingModeSet(i, MDMINI_RING_MODE_ERASE);
                }
                for(i=0; i<4; i++) {
                    Hu3DModelDispOff(model->mdlId[sys->chrSel[i]+MDMODEL_CAP_CHR_00]);
                }
                Hu3DModelDispOff(model->mdlId[MDMODEL_STAGE_CHR]);
                for(i=0; i<4; i++) {
                    Hu3DModelLayerSet(model->mdlId[sys->chrSel[i]+MDMODEL_CHR_00], 3);
                    Hu3DModelCameraSet(model->mdlId[sys->chrSel[i]+MDMODEL_CHR_00], camera->camera[MDCAMERA_MAIN].bit);
                    *model->posP[sys->chrSel[i]+MDMODEL_CHR_00] = *model->posP[sys->chrSel[i]+MDMODEL_CAP_CHR_00];
                    model->posP[sys->chrSel[i]+MDMODEL_CHR_00]->z = 0;
                }
                for(i=0; i<4; i++) {
                    effectP[i][0] = ParticleDotCreate(sprite->animP[MDMINI_ANIM_9], 4, camera->camera[MDCAMERA_MAIN].bit);
                    ParticleDotPosSet(effectP[i][0], model->posP[sys->chrSel[i]+MDMODEL_CAP_CHR_00]);
                    effectP[i][1] = ParticleGlowCreate(sprite->animP[MDMINI_ANIM_10], 4, camera->camera[MDCAMERA_MAIN].bit);
                    ParticleGlowPosSet(effectP[i][1], model->posP[sys->chrSel[i]+MDMODEL_CHR_00]);
                }
                if(param == 0) {
                    if(sys->gameType == 0) {
                        for(i=0; i<4; i++) {
                            effectPos[i][0] = *model->posP[sys->chrSel[i]+MDMODEL_CHR_00];
                            effectRot[i][0] = *model->rotP[sys->chrSel[i]+MDMODEL_CHR_00];
                        }
                    } else if(sys->gameType == 2) {
                        for(i=0; i<4; i++) {
                            effectPos[sys->teamOrder[i]][0] = *model->posP[sys->chrSel[i]+MDMODEL_CHR_00];
                            effectRot[sys->teamOrder[i]][0] = *model->rotP[sys->chrSel[i]+MDMODEL_CHR_00];
                        }
                    } else if(sys->gameType == 1) {
                        for(i=0; i<4; i++) {
                            effectPos[sys->handicapOrder[i]][0] = *model->posP[sys->chrSel[i]+MDMODEL_CHR_00];
                            effectRot[sys->handicapOrder[i]][0] = *model->rotP[sys->chrSel[i]+MDMODEL_CHR_00];
                        }
                    }
                } else if(param == 1) {
                    for(i=0; i<4; i++) {
                        effectPos[i][0] = *model->posP[sys->chrSel[i]+MDMODEL_CHR_00];
                        effectRot[i][0] = *model->rotP[sys->chrSel[i]+MDMODEL_CHR_00];
                    }
                }
                effectPos[0][1].x = -300;
                effectPos[0][1].y = 50+effectPos[0][0].y;
                effectPos[0][1].z = 200+effectPos[0][0].z;
                effectPos[1][1].x = -100;
                effectPos[1][1].y = 50+effectPos[1][0].y;
                effectPos[1][1].z = 200+effectPos[1][0].z;
                effectPos[2][1].x = 100;
                effectPos[2][1].y = 50+effectPos[2][0].y;
                effectPos[2][1].z = 200+effectPos[2][0].z;
                effectPos[3][1].x = 300;
                effectPos[3][1].y = 50+effectPos[3][0].y;
                effectPos[3][1].z = 200+effectPos[3][0].z;

                effectRot[0][1].x = 0;
                effectRot[0][1].y = 540;
                effectRot[0][1].z = 0;
                effectRot[1][1].x = 0;
                effectRot[1][1].y = 540;
                effectRot[1][1].z = 0;
                effectRot[2][1].x = 0;
                effectRot[2][1].y = -540;
                effectRot[2][1].z = 0;
                effectRot[3][1].x = 0;
                effectRot[3][1].y = -540;
                effectRot[3][1].z = 0;
                fn_1_17008(MDMINI_AUD_SE_BOARD_START);
                mode++;
                break;

            case 1:
                MdMathOscillate(MDMATH_OSC_90, &sys->procTime, &sys->procT, 45);
                MdMathOscillate(MDMATH_OSC_180, &sys->procOfsTime, &sys->procOfsT, 45);
                if(param == 0 && sys->gameType != 0) {
                    MdMathOscillate(MDMATH_OSC_90, &vsTime, &vsT, 15);
                    MdMathLerpScale(model->scaleP[MDMODEL_VS], 1, 0, vsT);
                }
                if(param == 0) {
                    if(sys->gameType == 0) {
                        float ofsTbl[4] = { 4, 2, 1.5f, 3 };
                        for(i=0; i<4; i++) {
                            posP = model->posP[sys->chrSel[i]+MDMODEL_CHR_00];
                            rotP = model->rotP[sys->chrSel[i]+MDMODEL_CHR_00];
                            scaleP = model->scaleP[sys->chrSel[i]+MDMODEL_CHR_00];
                            MdMathLerpOfs(&posP->x, effectPos[i][0].x, effectPos[i][1].x, sys->procT, 0.2f*(sys->procOfsT*ofsTbl[i]));
                            MdMathLerpOfs(&posP->y, effectPos[i][0].y, effectPos[i][1].y, sys->procT, 0.1f*(sys->procOfsT*ofsTbl[i]));
                            MdMathLerpOfs(&posP->z, effectPos[i][0].z, effectPos[i][1].z, sys->procT, 0.5f*(sys->procOfsT*ofsTbl[i]));
                            MdMathLerp(&rotP->y, effectRot[i][0].y, effectRot[i][1].y, sys->procT);
                            ParticleGlowPosSet(effectP[i][1], posP);
                        }
                    } else if(sys->gameType == 2) {
                        float ofsTbl[4] = { 4, 2, 1.5f, 3 };
                        for(i=0; i<4; i++) {
                            posP = model->posP[sys->chrSel[i]+MDMODEL_CHR_00];
                            rotP = model->rotP[sys->chrSel[i]+MDMODEL_CHR_00];
                            scaleP = model->scaleP[sys->chrSel[i]+MDMODEL_CHR_00];
                            MdMathLerpOfs(&posP->x, effectPos[sys->teamOrder[i]][0].x, effectPos[sys->teamOrder[i]][1].x, sys->procT, 0.2f*(sys->procOfsT*ofsTbl[sys->teamOrder[i]]));
                            MdMathLerpOfs(&posP->y, effectPos[sys->teamOrder[i]][0].y, effectPos[sys->teamOrder[i]][1].y, sys->procT, 0.2f*(sys->procOfsT*ofsTbl[sys->teamOrder[i]]));
                            MdMathLerpOfs(&posP->z, effectPos[sys->teamOrder[i]][0].z, effectPos[sys->teamOrder[i]][1].z, sys->procT, 0.5f*(sys->procOfsT*ofsTbl[sys->teamOrder[i]]));
                            MdMathLerp(&rotP->y, effectRot[sys->teamOrder[i]][0].y, effectRot[sys->teamOrder[i]][1].y, sys->procT);
                            ParticleGlowPosSet(effectP[i][1], posP);
                        }
                    } else if(sys->gameType == 1) {
                        float ofsTbl[4] = { 4, 2, 1.5f, 3 };
                        for(i=0; i<4; i++) {
                            posP = model->posP[sys->chrSel[i]+MDMODEL_CHR_00];
                            rotP = model->rotP[sys->chrSel[i]+MDMODEL_CHR_00];
                            scaleP = model->scaleP[sys->chrSel[i]+MDMODEL_CHR_00];
                            MdMathLerpOfs(&posP->x, effectPos[sys->handicapOrder[i]][0].x, effectPos[sys->handicapOrder[i]][1].x, sys->procT, 0.2f*(sys->procOfsT*ofsTbl[sys->handicapOrder[i]]));
                            MdMathLerpOfs(&posP->y, effectPos[sys->handicapOrder[i]][0].y, effectPos[sys->handicapOrder[i]][1].y, sys->procT, 0.2f*(sys->procOfsT*ofsTbl[sys->handicapOrder[i]]));
                            MdMathLerpOfs(&posP->z, effectPos[sys->handicapOrder[i]][0].z, effectPos[sys->handicapOrder[i]][1].z, sys->procT, 0.5f*(sys->procOfsT*ofsTbl[sys->handicapOrder[i]]));
                            MdMathLerp(&rotP->y, effectRot[sys->handicapOrder[i]][0].y, effectRot[sys->handicapOrder[i]][1].y, sys->procT);
                            ParticleGlowPosSet(effectP[i][1], posP);
                        }
                    }
                } else if(param == 1) {
                    float ofsTbl[4] = { 8, 4, 3, 6 };
                    for(i=0; i<4; i++) {
                        posP = model->posP[sys->chrSel[i]+MDMODEL_CHR_00];
                        rotP = model->rotP[sys->chrSel[i]+MDMODEL_CHR_00];
                        scaleP = model->scaleP[sys->chrSel[i]+MDMODEL_CHR_00];
                        MdMathLerpOfs(&posP->x, effectPos[i][0].x, effectPos[i][1].x, sys->procT, 0.2f*(sys->procOfsT*ofsTbl[i]));
                        MdMathLerpOfs(&posP->y, effectPos[i][0].y, effectPos[i][1].y, sys->procT, 0.2f*(sys->procOfsT*ofsTbl[i]));
                        MdMathLerpOfs(&posP->z, effectPos[i][0].z, effectPos[i][1].z, sys->procT, 0.5f*(sys->procOfsT*ofsTbl[i]));
                        MdMathLerp(&rotP->y, effectRot[i][0].y, effectRot[i][1].y, sys->procT);
                        ParticleGlowPosSet(effectP[i][1], posP);
                    }
                }
                if(sys->procTime >= 1) {
                    sys->procTime = 0;
                    sys->procOfsTime = 0;
                    sys->procT = 0;
                    sys->procOfsT = 0;
                    if(param == 0) {
                        if(sys->gameType == 0) {
                            for(i=0; i<4; i++) {
                                effectPos[i][0] = *model->posP[sys->chrSel[i]+MDMODEL_CHR_00];
                                MdMathAngleClamp(&model->rotP[sys->chrSel[i]+MDMODEL_CHR_00]->x);
                                MdMathAngleClamp(&model->rotP[sys->chrSel[i]+MDMODEL_CHR_00]->y);
                                MdMathAngleClamp(&model->rotP[sys->chrSel[i]+MDMODEL_CHR_00]->z);
                                effectRot[i][0] = *model->rotP[sys->chrSel[i]+MDMODEL_CHR_00];
                            }
                        } else if(sys->gameType == 2) {
                            for(i=0; i<4; i++) {
                                effectPos[sys->teamOrder[i]][0] = *model->posP[sys->chrSel[i]+MDMODEL_CHR_00];
                                MdMathAngleClamp(&model->rotP[sys->chrSel[i]+MDMODEL_CHR_00]->x);
                                MdMathAngleClamp(&model->rotP[sys->chrSel[i]+MDMODEL_CHR_00]->y);
                                MdMathAngleClamp(&model->rotP[sys->chrSel[i]+MDMODEL_CHR_00]->z);
                                effectRot[sys->teamOrder[i]][0] = *model->rotP[sys->chrSel[i]+MDMODEL_CHR_00];
                            }
                        } else if(sys->gameType == 1) {
                            for(i=0; i<4; i++) {
                                effectPos[sys->handicapOrder[i]][0] = *model->posP[sys->chrSel[i]+MDMODEL_CHR_00];
                                MdMathAngleClamp(&model->rotP[sys->chrSel[i]+MDMODEL_CHR_00]->x);
                                MdMathAngleClamp(&model->rotP[sys->chrSel[i]+MDMODEL_CHR_00]->y);
                                MdMathAngleClamp(&model->rotP[sys->chrSel[i]+MDMODEL_CHR_00]->z);
                                effectRot[sys->handicapOrder[i]][0] = *model->rotP[sys->chrSel[i]+MDMODEL_CHR_00];
                            }
                        }
                    } else if(param == 1) {
                        for(i=0; i<4; i++) {
                            effectPos[i][0] = *model->posP[sys->chrSel[i]+MDMODEL_CHR_00];
                            MdMathAngleClamp(&model->rotP[sys->chrSel[i]+MDMODEL_CHR_00]->x);
                            MdMathAngleClamp(&model->rotP[sys->chrSel[i]+MDMODEL_CHR_00]->y);
                            MdMathAngleClamp(&model->rotP[sys->chrSel[i]+MDMODEL_CHR_00]->z);
                            effectRot[i][0] = *model->rotP[sys->chrSel[i]+MDMODEL_CHR_00];
                        }
                    }
                    effectPos[0][1].x = -200;
                    effectPos[0][1].y = 250;
                    effectPos[0][1].z = -1000;
                    effectPos[1][1].x = -75;
                    effectPos[1][1].y = 250;
                    effectPos[1][1].z = -1000;
                    effectPos[2][1].x = 75;
                    effectPos[2][1].y = 250;
                    effectPos[2][1].z = -1000;
                    effectPos[3][1].x = 200;
                    effectPos[3][1].y = 250;
                    effectPos[3][1].z = -1000;

                    effectRot[0][1].x = 90;
                    effectRot[0][1].y = 160;
                    effectRot[0][1].z = -10;
                    effectRot[1][1].x = 90;
                    effectRot[1][1].y = 175;
                    effectRot[1][1].z = -10;
                    effectRot[2][1].x = 90;
                    effectRot[2][1].y = -175;
                    effectRot[2][1].z = 10;
                    effectRot[3][1].x = 90;
                    effectRot[3][1].y = -160;
                    effectRot[3][1].z = 10;
                    MdMessWinHomeClear(mess, MDMESS_WIN_MAIN);
                    star->motNext = MDMINI_STAR_MOT_IDLE;
                    enterTime = 0;
                    mode++;
                }
                break;

            case 2:
                if(++enterTime >= 0) {
                    fn_1_17008(MDMINI_AUD_SE_BOARD_ENTER1);
                    mode++;
                }
                break;

            case 3:
                MdProcCall(MDMINI_PROC_CAMERA, 0);
                MdMathOscillate(MDMATH_OSC_90, &sys->procTime, &sys->procT, 60);
                if(param == 0) {
                    if(sys->gameType == 0) {
                        for(i=0; i<4; i++) {
                            posP = model->posP[sys->chrSel[i]+MDMODEL_CHR_00];
                            rotP = model->rotP[sys->chrSel[i]+MDMODEL_CHR_00];
                            scaleP = model->scaleP[sys->chrSel[i]+MDMODEL_CHR_00];
                            MdMathLerp(&posP->x, effectPos[i][0].x, effectPos[i][1].x, sys->procT);
                            MdMathLerp(&posP->y, effectPos[i][0].y, effectPos[i][1].y, sys->procT);
                            MdMathLerp(&posP->z, effectPos[i][0].z, effectPos[i][1].z, sys->procT);
                            MdMathLerp(&rotP->x, effectRot[i][0].x, effectRot[i][1].x, sys->procT);
                            MdMathLerp(&rotP->y, effectRot[i][0].y, effectRot[i][1].y, sys->procT);
                            MdMathLerp(&rotP->z, effectRot[i][0].z, effectRot[i][1].z, sys->procT);
                            ParticleGlowPosSet(effectP[i][1], posP);
                        }
                    } else if(sys->gameType == 2) {
                        for(i=0; i<4; i++) {
                            posP = model->posP[sys->chrSel[i]+MDMODEL_CHR_00];
                            rotP = model->rotP[sys->chrSel[i]+MDMODEL_CHR_00];
                            scaleP = model->scaleP[sys->chrSel[i]+MDMODEL_CHR_00];
                            MdMathLerp(&posP->x, effectPos[sys->teamOrder[i]][0].x, effectPos[sys->teamOrder[i]][1].x, sys->procT);
                            MdMathLerp(&posP->y, effectPos[sys->teamOrder[i]][0].y, effectPos[sys->teamOrder[i]][1].y, sys->procT);
                            MdMathLerp(&posP->z, effectPos[sys->teamOrder[i]][0].z, effectPos[sys->teamOrder[i]][1].z, sys->procT);
                            MdMathLerp(&rotP->x, effectRot[sys->teamOrder[i]][0].x, effectRot[sys->teamOrder[i]][1].x, sys->procT);
                            MdMathLerp(&rotP->y, effectRot[sys->teamOrder[i]][0].y, effectRot[sys->teamOrder[i]][1].y, sys->procT);
                            MdMathLerp(&rotP->z, effectRot[sys->teamOrder[i]][0].z, effectRot[sys->teamOrder[i]][1].z, sys->procT);
                            ParticleGlowPosSet(effectP[i][1], posP);
                        }
                    } else if(sys->gameType == 1) {
                        for(i=0; i<4; i++) {
                            posP = model->posP[sys->chrSel[i]+MDMODEL_CHR_00];
                            rotP = model->rotP[sys->chrSel[i]+MDMODEL_CHR_00];
                            scaleP = model->scaleP[sys->chrSel[i]+MDMODEL_CHR_00];
                            MdMathLerp(&posP->x, effectPos[sys->handicapOrder[i]][0].x, effectPos[sys->handicapOrder[i]][1].x, sys->procT);
                            MdMathLerp(&posP->y, effectPos[sys->handicapOrder[i]][0].y, effectPos[sys->handicapOrder[i]][1].y, sys->procT);
                            MdMathLerp(&posP->z, effectPos[sys->handicapOrder[i]][0].z, effectPos[sys->handicapOrder[i]][1].z, sys->procT);
                            MdMathLerp(&rotP->x, effectRot[sys->handicapOrder[i]][0].x, effectRot[sys->handicapOrder[i]][1].x, sys->procT);
                            MdMathLerp(&rotP->y, effectRot[sys->handicapOrder[i]][0].y, effectRot[sys->handicapOrder[i]][1].y, sys->procT);
                            MdMathLerp(&rotP->z, effectRot[sys->handicapOrder[i]][0].z, effectRot[sys->handicapOrder[i]][1].z, sys->procT);
                            ParticleGlowPosSet(effectP[i][1], posP);
                        }
                    }
                } else if(param == 1) {
                    for(i=0; i<4; i++) {
                        posP = model->posP[sys->chrSel[i]+MDMODEL_CHR_00];
                        rotP = model->rotP[sys->chrSel[i]+MDMODEL_CHR_00];
                        scaleP = model->scaleP[sys->chrSel[i]+MDMODEL_CHR_00];
                        MdMathLerp(&posP->x, effectPos[i][0].x, effectPos[i][1].x, sys->procT);
                        MdMathLerp(&posP->y, effectPos[i][0].y, effectPos[i][1].y, sys->procT);
                        MdMathLerp(&posP->z, effectPos[i][0].z, effectPos[i][1].z, sys->procT);
                        MdMathLerp(&rotP->x, effectRot[i][0].x, effectRot[i][1].x, sys->procT);
                        MdMathLerp(&rotP->y, effectRot[i][0].y, effectRot[i][1].y, sys->procT);
                        MdMathLerp(&rotP->z, effectRot[i][0].z, effectRot[i][1].z, sys->procT);
                        ParticleGlowPosSet(effectP[i][1], posP);
                    }
                }
                if(sys->procTime >= 1) {
                    sys->procTime = 0;
                    sys->procOfsTime = 0;
                    sys->procT = 0;
                    sys->procOfsT = 0;
                    mode++;
                }
                break;
        }
        if(mode >= 2) {
            MdProcCall(MDMINI_PROC_WIN, 1);
        }
    }
    if(param == 0 || param == 1) {
        if(mode == 3) {
            return TRUE;
        } else {
            return FALSE;
        }
    }
#ifdef TARGET_PC
    return FALSE;
#endif
}

BOOL fn_1_149A4(int param)
{
    HuVec2f *sprPos; //r31
    MDSPRITE_WORK *sprite; //r30
    HUSPRGRPID grpId; //r29
    
    static HuVecF pos[2] = { { 288, -128, 0 }, { 288, 64, 0 } };
    
    sprite = sys->sprite;
    grpId = sprite->grpId[MDSPR_GRP_NAME];
    if(!sys->nameInitF) {
        if(param == 0) {
            HuSprGrpPosSet(grpId, 0, 0);
            HuSprPosSet(grpId, 0, pos[1].x, pos[1].y);
            HuSprDispOn(grpId, 0);
        } else if(param == 1) {
            
        }
        sys->nameInitF = TRUE;
    }
    if(param == 0) {
        MdMathOscillate(MDMATH_OSC_90, &sys->nameTime, &sys->nameT, 15);
        sprPos = MdSpritePosGet(sprite, MDSPR_GRP_NAME, 0);
        MdMathLerp(&sprPos->x, pos[0].x, pos[1].x, sys->nameT);
        MdMathLerp(&sprPos->y, pos[0].y, pos[1].y, sys->nameT);
    } else if(param == 1) {
        MdMathOscillate(MDMATH_OSC_90_REV, &sys->nameTime, &sys->nameT, 15);
        sprPos = MdSpritePosGet(sprite, MDSPR_GRP_NAME, 0);
        MdMathLerp(&sprPos->x, pos[1].x, pos[0].x, sys->nameT);
        MdMathLerp(&sprPos->y, pos[1].y, pos[0].y, sys->nameT);
    }
    if(sys->nameTime >= 1) {
        return TRUE;
    } else {
        return FALSE;
    }
}

void fn_1_14C2C(void)
{
    int i; //r31
    u16 stat; //r30
    s8 padNo[4] = { 0, 0, 0, 0 };
    sys->maxPlayerNum = 0;
    for(i=0; i<4; i++) {
        sys->padNoConnect[i] = -1;
    }
    for(i=0; i<4; i++) {
        stat = HuPadStatGet(i);
        if(stat == 0) {
            sys->padNoConnect[sys->maxPlayerNum] = i;
            sys->padConnect[i] = TRUE;
            sys->maxPlayerNum++;
        } else {
            sys->padConnect[i] = FALSE;
        }
    }
    if(sys->padConnect[0] == FALSE) {
        for(i=0; i<4; i++) {
            sys->padNoConnect[i] = -1;
            sys->padConnect[i] = FALSE;
        }
        sys->padNoConnect[0] = 0;
        sys->padConnect[0] = TRUE;
        sys->maxPlayerNum = 1;
    }
    sys->playerNum = sys->maxPlayerNum-1;
}

void fn_1_14E24(void)
{
    int i; //r31
    s8 padNo[4] = { 0, -1, -1, -1 };
    sys->maxPlayerNum = 0;
    for(i=0; i<4; i++) {
        sys->padNoConnect[i] = -1;
    }
    for(i=0; i<4; i++) {
        if(GwPlayerConf[i].type == GW_TYPE_MAN) {
            padNo[i] = 0;
        } else if(GwPlayerConf[i].type == GW_TYPE_COM) {
            padNo[i] = -1;
        }
    }
    for(i=0; i<4; i++) {
        if(padNo[i] == 0) {
            sys->padNoConnect[sys->maxPlayerNum] = i;
            sys->padConnect[i] = TRUE;
            sys->maxPlayerNum++;
        } else {
            sys->padConnect[i] = FALSE;
        }
    }
    sys->playerNum = sys->maxPlayerNum-1;
}

void fn_1_14FD0(void)
{
    MDSPRITE_WORK *sprite = sys->sprite; //r28
    MDMODEL_WORK *model = sys->model; //r29
    HUSPRGRPID grpId = sprite->grpId[MDSPR_GRP_PLAYERNO];
    int comNum = 0; //r30
    s8 padNo[4] = { -1, -1, -1, -1 }; //sp+0x8
    int i; //r31
    for(i=0; i<4; i++) {
        sys->padNoEnable[i] = sys->padNoConnect[i];
        if(i > sys->playerNum) {
            sys->padNoEnable[i] = -1;
        }
    }
    for(i=0; i<3; i++) {
        sys->comPlayerNo[i] = -1;
    }
    for(i=0; i<4; i++) {
        if(sys->padNoEnable[i] != -1) {
            padNo[sys->padNoEnable[i]] = 0;
        }
    }
    for(i=0; i<4; i++) {
        if(padNo[i] == -1) {
            sys->comPlayerNo[comNum] = i;
            comNum++;
            if(comNum >= 3) {
                break;
            }
        }
    }
    for(i=0; i<4; i++) {
        sys->padEnable[i] = FALSE;
    }
    for(i=0; i<4; i++) {
        if(sys->padNoEnable[i] != -1) {
            sys->padEnable[sys->padNoEnable[i]] = TRUE;
        }
    }
    for(i=0; i<4; i++) {
        if(sys->padEnable[i] == TRUE) {
            HuSprBankSet(grpId, (int)i, i);
        } else {
            HuSprBankSet(grpId, (int)i, 4);
        }
    }
    for(i=0; i<4; i++) {
        if(sys->padEnable[i] == TRUE) {
            Hu3DAnmNoSet(model->animId[MDMODEL_ANIM_FINGER_1P+i], i);
        } else {
            Hu3DAnmNoSet(model->animId[MDMODEL_ANIM_FINGER_1P+i], 4);
        }
    }
}

void fn_1_152D4(void)
{
    int i;
    for(i=0; i<4; i++) {
        sys->chrSel[i] = GwPlayerConf[i].charNo;
        sys->comDif[i] = GwPlayerConf[i].dif;
    }
    sys->selComNo = 3-sys->playerNum;
    for(i=0; i<4; i++) {
        sys->chrSelEnd[i] = TRUE;
    }
    sys->selComNo = 3-sys->playerNum;
}

void fn_1_153D0(void)
{
    int i; //r31
    HuVecF *fingerPosP; //r30
    MDMODEL_WORK *model; //r29
    MDSPRITE_WORK *sprite; //r28
    HuVecF *chrPosP; //r27
    MDCAMERA_WORK *camera; //r26
    HUSPRGRPID playerNoGrpId; //r25
    HUSPRGRPID comDifGrpId; //r24
    
    HuVecF pos3D; //sp+0x14
    HuVecF pos2D; //sp+0x8
    
    camera = sys->camera;
    model = sys->model;
    sprite = sys->sprite;
    playerNoGrpId = sprite->grpId[MDSPR_GRP_PLAYERNO];
    comDifGrpId = sprite->grpId[MDSPR_GRP_COMDIF];
    HuSprGrpPosSet(playerNoGrpId, 0, 0);
    HuSprGrpPosSet(comDifGrpId, 0, 0);
    for(i=0; i<4; i++) {
        fingerPosP = model->posP[i+MDMODEL_FINGER_1P];
        chrPosP = model->posP[sys->chrSel[i]+MDMODEL_CAP_CHR_00];
        *fingerPosP = *chrPosP;
        fingerPosP->x -= 60;
        fingerPosP->y += 105;
        fingerPosP->z += 30;
        pos3D = *chrPosP;
        Hu3D3Dto2D(&pos3D, camera->camera[MDCAMERA_MAIN].bit, &pos2D);
        HuSprPosSet(playerNoGrpId, (int)i, pos2D.x, pos2D.y-18);
        HuSprPosSet(comDifGrpId, (int)i, pos2D.x, pos2D.y-18);
    }
}

BOOL fn_1_15590(int playerNo, BOOL comSelF)
{
    int i; //r31
    int padNo; //r30

    s8 selFlag[10]; //sp+0x8
    if(sys->chrSelEnd[playerNo] == TRUE) {
        return FALSE;
    }
    if(comSelF == FALSE) {
        padNo = playerNo;
    } else if(comSelF == TRUE) {
        padNo = 0;
    }
    if((sys->pad->DStkDown[padNo] & PAD_BUTTON_LEFT) ||
        (sys->pad->DStkDown[padNo] & PAD_BUTTON_RIGHT) ||
        (sys->pad->DStkDown[padNo] & PAD_BUTTON_UP) ||
        (sys->pad->DStkDown[padNo] & PAD_BUTTON_DOWN)) {
            for(i=0; i<10; i++) {
                selFlag[i] = -1;
            }
            for(i=0; i<4; i++) {
                if(comSelF == FALSE) {
                    if(i == playerNo) {
                        continue;
                    }
                    if(sys->padEnable[i] == FALSE) {
                        continue;
                    }
                } else if(comSelF == TRUE) {
                    if(i == playerNo) {
                        continue;
                    }
                    if(sys->chrSelEnd[i] == FALSE) {
                        continue;
                    }
                }
                selFlag[sys->chrSel[i]] = 0;
            }
            if(sys->pad->DStkDown[padNo] & PAD_BUTTON_LEFT) {
                for(i=0; i<4; i++) {
                    if(--sys->chrSel[playerNo] == -1) {
                        sys->chrSel[playerNo] = 9;
                    }
                    if(selFlag[sys->chrSel[playerNo]] == -1) {
                        break;
                    }
                }
            } else if(sys->pad->DStkDown[padNo] & PAD_BUTTON_RIGHT) {
                for(i=0; i<4; i++) {
                    sys->chrSel[playerNo] = ++sys->chrSel[playerNo] %10;
                    if(selFlag[sys->chrSel[playerNo]] == -1) {
                        break;
                    }
                }
            } else if(sys->pad->DStkDown[padNo] & PAD_BUTTON_UP) {
                static s8 fixPos[5][4] = {
                    { 0, 4, 1, 3 },
                    { 1, 0, 2, 4 },
                    { 2, 1, 3, 0 },
                    { 3, 2, 4, 1 },
                    { 4, 3, 0, 2 },
                };
                if(sys->chrSel[playerNo] < 5) {
                    for(i=0; i<4; i++) {
                        if(selFlag[fixPos[sys->chrSel[playerNo]][i]+5] == -1) {
                            sys->chrSel[playerNo] = fixPos[sys->chrSel[playerNo]][i]+5;
                            break;
                        }
                    }
                } else {
                    for(i=0; i<4; i++) {
                        if(selFlag[fixPos[sys->chrSel[playerNo]-5][i]] == -1) {
                            sys->chrSel[playerNo] = fixPos[sys->chrSel[playerNo]-5][i];
                            break;
                        }
                    }
                }
            } else if(sys->pad->DStkDown[padNo] & PAD_BUTTON_DOWN) {
                static s8 fixPos[5][4] = {
                    { 0, 1, 4, 2 },
                    { 1, 2, 0, 3 },
                    { 2, 3, 1, 4 },
                    { 3, 4, 2, 0 },
                    { 4, 0, 3, 1 },
                };
                if(sys->chrSel[playerNo] < 5) {
                    for(i=0; i<4; i++) {
                        if(selFlag[fixPos[sys->chrSel[playerNo]][i]+5] == -1) {
                            sys->chrSel[playerNo] = fixPos[sys->chrSel[playerNo]][i]+5;
                            break;
                        }
                    }
                } else {
                    for(i=0; i<4; i++) {
                        if(selFlag[fixPos[sys->chrSel[playerNo]-5][i]] == -1) {
                            sys->chrSel[playerNo] = fixPos[sys->chrSel[playerNo]-5][i];
                            break;
                        }
                    }
                }
            }
            return TRUE;
        } else {
            return FALSE;
        }
        
}

void fn_1_15BF4(void)
{
    int i; //r31
    s8 selFlag[10];
    int temp = 0;
    for(i=0; i<10; i++) {
        selFlag[i] = -1;
    }
    for(i=0; i<4; i++) {
        if(sys->chrSelEnd[i] == TRUE) {
            selFlag[sys->chrSel[i]] = 0;
        }
    }
    for(i=0; i<4; i++) {
        if(selFlag[i] == -1) {
            sys->chrSel[sys->comPlayerNo[sys->selComNo]] = i;
            break;
        }
    }
}

void fn_1_15CF8(int mdlNo, int cameraNo)
{
    MDCAMERA *camera; //r31
    HuVecF *posP; //r30
    MDMODEL_WORK *model; //r29
    HuVecF *rotP; //r28
    MDCAMERA_WORK *cameraWork; //r27
    
    float rotY;
    HuVecF dir;
    
    cameraWork = sys->camera;
    camera = &cameraWork->camera[cameraNo];
    model = sys->model;
    posP = model->posP[mdlNo];
    rotP = model->rotP[mdlNo];
    dir.x = camera->pos.x-posP->x;
    dir.y = camera->pos.y-posP->y;
    dir.z = camera->pos.z-posP->z;
    rotY = HuAtan(dir.x, dir.z);
    rotP->y = rotY;
}

void fn_1_15DE8(void)
{
    int i; //r31
    int j; //r30
    int comNum; //r29
    
    static s8 teamPlayer[3][4] = {
        { 0, 1, 2, 3 },
        { 0, 2, 1, 3 },
        { 0, 3, 1, 2 },
    };
    static s8 teamOrder[3][4] = {
        { 0, 1, 2, 3 },
        { 0, 2, 1, 3 },
        { 0, 2, 3, 1 },
    };
    static s8 teamChr[45][2] = {
        { 0, 1 },
        { 0, 2 },
        { 0, 3 },
        { 0, 4 },
        { 0, 5 },
        { 0, 6 },
        { 0, 7 },
        { 0, 8 },
        { 0, 9 },
        
        { 1, 2 },
        { 1, 3 },
        { 1, 4 },
        { 1, 5 },
        { 1, 6 },
        { 1, 7 },
        { 1, 8 },
        { 1, 9 },
        
        { 2, 3 },
        { 2, 4 },
        { 2, 5 },
        { 2, 6 },
        { 2, 7 },
        { 2, 8 },
        { 2, 9 },
        
        { 3, 4 },
        { 3, 5 },
        { 3, 6 },
        { 3, 7 },
        { 3, 8 },
        { 3, 9 },
        
        { 4, 5 },
        { 4, 6 },
        { 4, 7 },
        { 4, 8 },
        { 4, 9 },
        
        { 5, 6 },
        { 5, 7 },
        { 5, 8 },
        { 5, 9 },
        
        { 6, 7 },
        { 6, 8 },
        { 6, 9 },
        
        { 7, 8 },
        { 7, 9 },
        
        { 8, 9 },
    };
    
    comNum = 0;
    for(i=0; i<2; i++) {
        sys->teamPlayer[0][i] = teamPlayer[sys->teamOrderNo][i];
        sys->teamPlayer[1][i] = teamPlayer[sys->teamOrderNo][i+2];
    }
    for(i=0; i<4; i++) {
        sys->teamOrder[i] = teamOrder[sys->teamOrderNo][i];
    }
    for(i=0; i<2; i++) {
        for(j=0; j<45; j++) {
            if((sys->chrSel[sys->teamPlayer[i][0]] == teamChr[j][0] &&
                sys->chrSel[sys->teamPlayer[i][1]] == teamChr[j][1]) ||
                (sys->chrSel[sys->teamPlayer[i][1]] == teamChr[j][0] &&
                sys->chrSel[sys->teamPlayer[i][0]] == teamChr[j][1])) {
                    sys->teamNo[i] = j;
                    break;
                }
        }
    }
    
    for(i=0; i<3; i++) {
        sys->teamComPlayerNo[i] = -1;
    }
    for(i=0; i<4; i++) {
        if(!sys->padEnable[teamPlayer[sys->teamOrderNo][i]]) {
            sys->teamComPlayerNo[comNum] = teamPlayer[sys->teamOrderNo][i];
            comNum++;
            if(comNum >= 3) {
                break;
            }
        }
    }
}

void fn_1_16168(void)
{
    int i;
    for(i=0; i<4; i++) {
        sys->teamOrderBackup[i] = sys->teamOrder[i];
    }
}

void fn_1_161B8(void)
{
    int i;
    int comNum;

    static s8 teamPlayer[4][4] = {
        { 0, 1, 2, 3 },
        { 1, 0, 2, 3 },
        { 2, 0, 1, 3 },
        { 3, 0, 1, 2 },
    };
    static s8 teamOrder[4][4] = {
        { 0, 1, 2, 3 },
        { 1, 0, 2, 3 },
        { 1, 2, 0, 3 },
        { 1, 2, 3, 0 },
    };

    comNum = 0;
    for(i=0; i<4; i++) {
        sys->handicapOrder[i] = teamOrder[sys->handicapNo][i];
    }
    for(i=0; i<3; i++) {
        sys->handicapComPlayerNo[i] = -1;
    }
    for(i=0; i<4; i++) {
        if(!sys->padEnable[teamPlayer[sys->handicapNo][i]]) {
            sys->handicapComPlayerNo[comNum] = teamPlayer[sys->handicapNo][i];
            comNum++;
            if(comNum >= 3) {
                break;
            }
        }
    }
}

/* 0x16308 - same latch for the four computer slots */
void fn_1_16308(void)
{
    int i;

    for(i = 0; i < 4; i++) {
        sys->handicapOrderBackup[i] = sys->handicapOrder[i];
    }
}

void fn_1_16358(void)
{
    int i;
    int j;
    MDMODEL_WORK *model;
    MGDATA *mgData;
    BOOL unlockF;

    model = sys->model;
    for(i=0; i<6; i++) {
        sys->mgTypeF[i] = 0;
    }
    sys->mgTypeF[0] = 1;
    {
        s8 type3[3] = { 0, 0, 0 };
    j = 0;
    mgData = &mgDataTbl[j];
    do {
        if(type3[0] == 0 && mgData->type == MG_TYPE_4P && (mgData->flag & MG_FLAG_NOSTORY) == 0) {
            if((unlockF = GWMgUnlockGet(j+0x1F5)) == 1) {
                type3[0] = 1;
            }
        } else if(type3[1] == 0 && mgData->type == MG_TYPE_1VS3 && (mgData->flag & MG_FLAG_NOSTORY) == 0) {
            if((unlockF = GWMgUnlockGet(j+0x1F5)) == 1) {
                type3[1] = 1;
            }
        } else if(type3[2] == 0 && mgData->type == MG_TYPE_2VS2 && (mgData->flag & MG_FLAG_NOSTORY) == 0 && (unlockF = GWMgUnlockGet(j+0x1F5)) == 1) {
            type3[2] = 1;
        }
        j++;
        mgData++;
    } while(mgData->ovl != 0xFFFF);
    if(type3[0] == 1 && type3[1] == 1 && type3[2] == 1) {
        sys->mgTypeF[1] = 1;
    }
    }
    {
        s8 type5[5] = { 0, 0, 0, 0, 0 };
    j = 0;
    mgData = &mgDataTbl[j];
    do {
        if(type5[0] == 0 && mgData->type == MG_TYPE_4P && (mgData->flag & MG_FLAG_NOSTORY) == 0) {
            if((unlockF = GWMgUnlockGet(j+0x1F5)) == 1) {
                type5[0] = 1;
            }
        } else if(type5[1] == 0 && mgData->type == MG_TYPE_1VS3 && (mgData->flag & MG_FLAG_NOSTORY) == 0) {
            if((unlockF = GWMgUnlockGet(j+0x1F5)) == 1) {
                type5[1] = 1;
            }
        } else if(type5[2] == 0 && mgData->type == MG_TYPE_2VS2 && (mgData->flag & MG_FLAG_NOSTORY) == 0) {
            if((unlockF = GWMgUnlockGet(j+0x1F5)) == 1) {
                type5[2] = 1;
            }
        } else if(type5[3] == 0 && mgData->type == MG_TYPE_KUPA) {
            if((unlockF = GWMgUnlockGet(j+0x1F5)) == 1) {
                type5[3] = 1;
            }
        } else if(type5[4] == 0 && mgData->type == MG_TYPE_KETTOU && (unlockF = GWMgUnlockGet(j+0x1F5)) == 1) {
            type5[4] = 1;
        }
        j++;
        mgData++;
    } while(mgData->ovl != 0xFFFF);
    if(type5[0] == 1 && type5[1] == 1 && type5[2] == 1 && type5[3] == 1 && type5[4] == 1) {
        sys->mgTypeF[2] = 1;
    }
    }
    {
        s16 unlockTbl[11] = { 502, 504, 506, 507, 510, 511, 512, 513, 514, 563, -1 };
    j = 0;
    while(1) {
        if((unlockF = GWMgUnlockGet(unlockTbl[j])) == 0) {
            sys->mgTypeF[3] = 0;
            break;
        }
        j++;
        if(unlockTbl[j] == -1) {
            sys->mgTypeF[3] = 1;
            break;
        }
    }
    }
    j = 0;
    mgData = &mgDataTbl[j];
    do {
        if(mgData->type == MG_TYPE_4P && (mgData->flag & MG_FLAG_NOSTORY) == 0 && (unlockF = GWMgUnlockGet(j+0x1F5)) == 1) {
            sys->mgTypeF[4] = 1;
            break;
        }
        j++;
        mgData++;
    } while(mgData->ovl != 0xFFFF);
    j = 0;
    mgData = &mgDataTbl[j];
    do {
        if(mgData->type == MG_TYPE_KETTOU && (unlockF = GWMgUnlockGet(j+0x1F5)) == 1) {
            sys->mgTypeF[5] = 1;
            break;
        }
        j++;
        mgData++;
    } while(mgData->ovl != 0xFFFF);
    for(i=0; i<6; i++) {
        if(sys->mgTypeF[i] == 0) {
            Hu3DModelTPLvlSet(model->mdlId[i+MDMODEL_CAP_MODE_00], 0.2f);
        }
    }
}

void fn_1_16978(void)
{
    int i;

    for(i=0; i<4; i++) {
        sdHealthLoss[i] = 0;
    }
    if(sys->scoreNum == 0) {
        mgMatchMaxScore = 3;
    } else if(sys->scoreNum == 1) {
        mgMatchMaxScore = 5;
    } else if(sys->scoreNum == 2) {
        mgMatchMaxScore = 7;
    }
    if(sys->gameType == 0) {
        mgMatchGameType = 0;
    } else if(sys->gameType == 1) {
        mgMatchGameType = 1;
    } else if(sys->gameType == 2) {
        mgMatchGameType = 2;
    }
    if(sys->selNoNow == 1) {
        GwPlayer[0].grp = 0;
        if(sys->gameType == 0) {
            GwPlayer[0].grp = 0;
            GwPlayer[1].grp = 1;
            GwPlayer[2].grp = 2;
            GwPlayer[3].grp = 3;
        } else if(sys->gameType == 1) {
            GwPlayer[0].grp = 0;
            GwPlayer[1].grp = 1;
            GwPlayer[2].grp = 1;
            GwPlayer[3].grp = 1;
            for(i=0; i<4; i++) {
                if(sys->handicapOrder[i] < 1) {
                    GwPlayer[i].grp = 0;
                } else {
                    GwPlayer[i].grp = 1;
                }
            }
        } else if(sys->gameType == 2) {
            GwPlayer[0].grp = 0;
            GwPlayer[1].grp = 0;
            GwPlayer[2].grp = 1;
            GwPlayer[3].grp = 1;
            for(i=0; i<4; i++) {
                if(sys->teamOrder[i] < 2) {
                    GwPlayer[i].grp = 0;
                } else {
                    GwPlayer[i].grp = 1;
                }
            }
        }
    } else {
        GwPlayer[0].grp = 0;
        GwPlayer[1].grp = 1;
        GwPlayer[2].grp = 2;
        GwPlayer[3].grp = 3;
    }
    for(i=0; i<4; i++) {
        if(sys->padEnable[i] == 0) {
            GwPlayer[i].comF = TRUE;
        } else {
            GwPlayer[i].comF = FALSE;
        }
        GwPlayer[i].charNo = sys->chrSel[i];
        GwPlayer[i].dif = sys->comDif[i];
        if(sys->padNoEnable[i] != -1) {
            GwPlayer[i].padNo = sys->padNoConnect[i];
        } else {
            GwPlayer[i].padNo = -1;
        }
        GwPlayerConf[i].charNo = GwPlayer[i].charNo;
        GwPlayerConf[i].padNo = GwPlayer[i].padNo = i;
        GwPlayerConf[i].dif = GwPlayer[i].dif;
        GwPlayerConf[i].type = GwPlayer[i].comF;
        GwPlayerConf[i].grp = GwPlayer[i].grp;
    }
}

void fn_1_17008(int cmd)
{
    int pan; //r30

    static int streamNo; //bss+0x28
    static int seNo; //bss+0x24
    switch(cmd) {
        case MDMINI_AUD_MUS_START:
            streamNo = HuAudBGMPlay(MSM_STREAM_MODE);
            break;

        case MDMINI_AUD_MUS_STOP:
            HuAudSStreamFadeOut(streamNo, 1000);
            break;

        case MDMINI_AUD_SE_CMN_MOVE:
        case MDMINI_AUD_SE_CMN_CONFIRM:
        case MDMINI_AUD_SE_CMN_CHRSEL:
        case MDMINI_AUD_SE_CMN_CANCEL:
        case MDMINI_AUD_SE_CMN_SELECT:
        case MDMINI_AUD_SE_MENU_MOVE:
        case MDMINI_AUD_SE_BOARD_START:
        case MDMINI_AUD_SE_BOARD_ENTER1:
            if(cmd == MDMINI_AUD_SE_CMN_MOVE) {
                HuAudFXPlay(MSM_SE_CMN_01);
            } else if(cmd == MDMINI_AUD_SE_CMN_CONFIRM) {
                HuAudFXPlay(MSM_SE_CMN_03);
            } else if(cmd == MDMINI_AUD_SE_CMN_CHRSEL) {
                HuAudFXPlay(MSM_SE_CMN_02);
            } else if(cmd == MDMINI_AUD_SE_CMN_CANCEL) {
                HuAudFXPlay(MSM_SE_CMN_04);
            } else if(cmd == MDMINI_AUD_SE_CMN_SELECT) {
                HuAudFXPlay(MSM_SE_CMN_05);
            } else if(cmd == MDMINI_AUD_SE_MENU_MOVE) {
                HuAudFXPlay(MSM_SE_MENU_27);
            } else if(cmd == MDMINI_AUD_SE_BOARD_START) {
                HuAudFXPlay(MSM_SE_MENU_28);
            } else if(cmd == MDMINI_AUD_SE_BOARD_ENTER1) {
                seNo = HuAudFXPlay(MSM_SE_MENU_29);
            }
            break;

        case MDMINI_AUD_SE_STAR_ENTER:
        case MDMINI_AUD_SE_STAR_CONFIRM:
        {
            int pan = fn_1_171B4(MDMODEL_STAR, MDCAMERA_FG);
            if(cmd == MDMINI_AUD_SE_STAR_ENTER) {
                HuAudFXPlayPan(MSM_SE_GUIDE_56, pan);
            } else if(cmd == MDMINI_AUD_SE_STAR_CONFIRM) {
                HuAudFXPlayPan(MSM_SE_GUIDE_56, pan);
            }
        }
            break;

        case MDMINI_AUD_SE_CHR_MARIO:
        case MDMINI_AUD_SE_CHR_LUIGI:
        case MDMINI_AUD_SE_CHR_PEACH:
        case MDMINI_AUD_SE_CHR_YOSHI:
        case MDMINI_AUD_SE_CHR_WARIO:
        case MDMINI_AUD_SE_CHR_DAISY:
        case MDMINI_AUD_SE_CHR_WALUIGI:
        case MDMINI_AUD_SE_CHR_KINOPIO:
        case MDMINI_AUD_SE_CHR_TERESA:
        case MDMINI_AUD_SE_CHR_MINIKOOPA:
        {
            static int charSe[10] = {
                MSM_SE_VOICE_MARIO+11,
                MSM_SE_VOICE_LUIGI+11,
                MSM_SE_VOICE_PEACH+11,
                MSM_SE_VOICE_YOSHI+11,
                MSM_SE_VOICE_WARIO+11,
                MSM_SE_VOICE_DAISY+11,
                MSM_SE_VOICE_WALUIGI+11,
                MSM_SE_VOICE_KINOPIO+11,
                MSM_SE_VOICE_TERESA+11,
                MSM_SE_VOICE_MINIKOOPAR+11
            };
            int pan = fn_1_171B4((cmd-MDMINI_AUD_SE_CHR_MARIO)+MDMODEL_CAP_CHR_00, MDCAMERA_MAIN);
            HuAudFXPlayPan(charSe[cmd-MDMINI_AUD_SE_CHR_MARIO], pan);
        }
            break;

        case MDMINI_AUD_SE_ALL_STOP:
            HuAudAllStop();
            break;
    }
}

int fn_1_171B4(int mdlNo, int cameraNo)
{
    MDCAMERA *camera; //r31
    MDMODEL_WORK *model; //r30
    
    MDCAMERA_WORK *cameraWork; //r29
    HuVecF *posP; //r28
    int result; //r27
    
    HuVecF pos2D;
    
    cameraWork = sys->camera;
    camera = &cameraWork->camera[cameraNo];
    model = sys->model;
    posP = model->posP[mdlNo];
    Hu3D3Dto2D(posP, camera->bit, &pos2D);
    result = MSM_PAN_LEFT+((MSM_PAN_WIDTH/HU_DISP_WIDTH)*pos2D.x);
    return result;
}

void MdPosAdjustMode(void)
{
    HuVecF *pos; //r31
    HuVecF *rot; //r30
    HuVecF *scale; //r29
    MDDEBUGFONT_WORK *debugFont; //r28
    MDMODEL_WORK *model; //r27
    HUSPRGRPID grpId; //r26
    int i; //r25
    u32 attr; //r24
    MDSPRITE_WORK *sprite; //r23
    MDCAMERA_WORK *camera; //r22
    
    HuVecF pos2D; //sp+0x8
    
    static MDMODEL_DEBUG_INFO mdlInfo[MDMODEL_MAX] = {
        { "STAR", MDCAMERA_FG },
        { "STAGE", MDCAMERA_MAIN },
        { "STAGE_CHR", MDCAMERA_MAIN },
        { "MARIO", MDCAMERA_MAIN },
        { "LUIGI", MDCAMERA_MAIN },
        { "PEACH", MDCAMERA_MAIN },
        { "YOSHI", MDCAMERA_MAIN },
        { "WARIO", MDCAMERA_MAIN },
        { "DAISY", MDCAMERA_MAIN },
        { "WALUIGI", MDCAMERA_MAIN },
        { "KINOPIO", MDCAMERA_MAIN },
        { "TERESA", MDCAMERA_MAIN },
        { "MINIKOOPA", MDCAMERA_MAIN },
        { "CAP_MAN_1P", MDCAMERA_MAIN },
        { "CAP_MAN_2P", MDCAMERA_MAIN },
        { "CAP_MAN_3P", MDCAMERA_MAIN },
        { "CAP_MAN_4P", MDCAMERA_MAIN },
        { "CAP_COM_1P", MDCAMERA_MAIN },
        { "CAP_COM_2P", MDCAMERA_MAIN },
        { "CAP_COM_3P", MDCAMERA_MAIN },
        { "CAP_COM_4P", MDCAMERA_MAIN },
        { "CAP_CHR_00", MDCAMERA_MAIN },
        { "CAP_CHR_01", MDCAMERA_MAIN },
        { "CAP_CHR_02", MDCAMERA_MAIN },
        { "CAP_CHR_03", MDCAMERA_MAIN },
        { "CAP_CHR_04", MDCAMERA_MAIN },
        { "CAP_CHR_05", MDCAMERA_MAIN },
        { "CAP_CHR_06", MDCAMERA_MAIN },
        { "CAP_CHR_07", MDCAMERA_MAIN },
        { "CAP_CHR_08", MDCAMERA_MAIN },
        { "CAP_CHR_09", MDCAMERA_MAIN },
        { "CAP_MODE_00", MDCAMERA_MAIN },
        { "CAP_MODE_01", MDCAMERA_MAIN },
        { "CAP_MODE_02", MDCAMERA_MAIN },
        { "CAP_MODE_03", MDCAMERA_MAIN },
        { "CAP_MODE_04", MDCAMERA_MAIN },
        { "CAP_MODE_05", MDCAMERA_MAIN },
        { "CAP_WIN", MDCAMERA_MAIN },
        { "CAP_GENRE", MDCAMERA_MAIN },
        { "FINGER_1P", MDCAMERA_MAIN },
        { "FINGER_2P", MDCAMERA_MAIN },
        { "FINGER_3P", MDCAMERA_MAIN },
        { "FINGER_4P", MDCAMERA_MAIN },
        { "RING_1P", MDCAMERA_MAIN },
        { "RING_2P", MDCAMERA_MAIN },
        { "RING_3P", MDCAMERA_MAIN },
        { "RING_4P", MDCAMERA_MAIN },
        { "VS", MDCAMERA_MAIN },
        { "FONTBGFILTER", MDCAMERA_FG },
    };
    
    static MDMODEL_DEBUG_INFO *mdlInfoP;
    static HuVec2f sprPos;
    static float sprRot;
    static int menuSel;
    static int mdlSel;
    static int dispTimer;
    static BOOL showF;
    
    debugFont = sys->debugFont;
    camera = sys->camera;
    sprite = sys->sprite;
    model = sys->model;
    grpId = sprite->grpId[MDSPR_GRP_CURSOR_LR];
    if(debugFont->ctrl == MD_DEBUGFONT_CTRL_MODEL) {
        if(showF == FALSE) {
            if(HuPadBtn[0] & PAD_BUTTON_X) {
                if(++dispTimer >= 60) {
                    MdSpriteDispSet(sprite, MDSPR_GRP_CURSOR_LR, TRUE);
                    dispTimer = 0;
                    showF = TRUE;
                }
            } else {
                dispTimer = 0;
            }
        } else if(showF == TRUE) {
            while(1) {
                if(HuPadBtn[0] & PAD_BUTTON_X) {
                    if(++dispTimer >= 60) {
                        MdSpriteDispSet(sprite, MDSPR_GRP_CURSOR_LR, FALSE);
                        dispTimer = 0;
                        showF = FALSE;
                        break;
                    }
                } else {
                    dispTimer = 0;
                }
                if(debugFont->ctrl == MD_DEBUGFONT_CTRL_MODEL) {
                    if(sys->pad->DStkDown[0] & PAD_BUTTON_LEFT) {
                        for(i=0; i<MDMODEL_MAX; i++) {
                            if(--mdlSel == -1) {
                                mdlSel = MDMODEL_MAX-1;
                            }
                            attr = Hu3DModelAttrGet(model->mdlId[mdlSel]);
                            if(!(attr & HU3D_ATTR_DISPOFF)) {
                                break;
                            }
                        }
                    } else if(sys->pad->DStkDown[0] & PAD_BUTTON_RIGHT) {
                        for(i=0; i<MDMODEL_MAX; i++) {
                            mdlSel = ++mdlSel % MDMODEL_MAX;
                            attr = Hu3DModelAttrGet(model->mdlId[mdlSel]);
                            if(!(attr & HU3D_ATTR_DISPOFF)) {
                                break;
                            }
                        }
                    }
                    if(HuPadBtnDown[0] & PAD_BUTTON_DOWN) {
                        menuSel = ++menuSel % 3;
                    }
                    mdlInfoP = &mdlInfo[mdlSel];
                    pos = model->posP[mdlSel];
                    rot = model->rotP[mdlSel];
                    scale = model->scaleP[mdlSel];
                    attr = Hu3DModelAttrGet(model->mdlId[mdlSel]);
                    if(menuSel == 0) {
                        if(HuPadTrigL[0] <= 75) {
                            if(HuPadSubStkX[0] <= -29) {
                                pos->x -= 5;
                            } else if(HuPadSubStkX[0] >= 29) {
                                pos->x += 5;
                            }
                            if(HuPadStkY[0] <= -36) {
                                pos->y -= 5;
                            } else if(HuPadStkY[0] >= 36) {
                                pos->y += 5;
                            }
                            if(HuPadSubStkY[0] <= -29) {
                                pos->z += 5;
                            } else if(HuPadSubStkY[0] >= 29) {
                                pos->z -= 5;
                            }
                        } else if(HuPadBtn[0] & PAD_TRIGGER_L) {
                            if(HuPadSubStkX[0] <= -29) {
                                pos->x -= 0.5f;
                            } else if(HuPadSubStkX[0] >= 29) {
                                pos->x += 0.5f;
                            }
                            if(HuPadStkY[0] <= -36) {
                                pos->y -= 0.5f;
                            } else if(HuPadStkY[0] >= 36) {
                                pos->y += 0.5f;
                            }
                            if(HuPadSubStkY[0] <= -29) {
                                pos->z += 0.5f;
                            } else if(HuPadSubStkY[0] >= 29) {
                                pos->z -= 0.5f;
                            }
                        } else {
                            if(HuPadSubStkX[0] <= -29) {
                                pos->x -= 1.0f;
                            } else if(HuPadSubStkX[0] >= 29) {
                                pos->x += 1.0f;
                            }
                            if(HuPadStkY[0] <= -36) {
                                pos->y -= 1.0f;
                            } else if(HuPadStkY[0] >= 36) {
                                pos->y += 1.0f;
                            }
                            if(HuPadSubStkY[0] <= -29) {
                                pos->z += 1.0f;
                            } else if(HuPadSubStkY[0] >= 29) {
                                pos->z -= 1.0f;
                            }
                        }
                    } else if(menuSel == 1) {
                        if(HuPadTrigL[0] <= 75) {
                            if(HuPadSubStkX[0] <= -29) {
                                rot->x -= 10;
                            } else if(HuPadSubStkX[0] >= 29) {
                                rot->x += 10;
                            }
                            if(HuPadStkY[0] <= -36) {
                                rot->y -= 10;
                            } else if(HuPadStkY[0] >= 36) {
                                rot->y += 10;
                            }
                            if(HuPadSubStkY[0] <= -29) {
                                rot->z += 10;
                            } else if(HuPadSubStkY[0] >= 29) {
                                rot->z -= 10;
                            }
                        } else if(HuPadBtn[0] & PAD_TRIGGER_L) {
                            if(HuPadSubStkX[0] <= -29) {
                                rot->x -= 1;
                            } else if(HuPadSubStkX[0] >= 29) {
                                rot->x += 1;
                            }
                            if(HuPadStkY[0] <= -36) {
                                rot->y -= 1;
                            } else if(HuPadStkY[0] >= 36) {
                                rot->y += 1;
                            }
                            if(HuPadSubStkY[0] <= -29) {
                                rot->z += 1;
                            } else if(HuPadSubStkY[0] >= 29) {
                                rot->z -= 1;
                            }
                        } else {
                            if(HuPadSubStkX[0] <= -29) {
                                rot->x -= 5;
                            } else if(HuPadSubStkX[0] >= 29) {
                                rot->x += 5;
                            }
                            if(HuPadStkY[0] <= -36) {
                                rot->y -= 5;
                            } else if(HuPadStkY[0] >= 36) {
                                rot->y += 5;
                            }
                            if(HuPadSubStkY[0] <= -29) {
                                rot->z += 5;
                            } else if(HuPadSubStkY[0] >= 29) {
                                rot->z -= 5;
                            }
                        }
                    } else if(menuSel == 2) {
                        if(HuPadTrigL[0] <= 75) {
                            if(HuPadSubStkX[0] <= -29) {
                                scale->x -= 0.05f;
                                scale->y -= 0.05f;
                                scale->z -= 0.05f;
                            } else if(HuPadSubStkX[0] >= 29) {
                                scale->x += 0.05f;
                                scale->y += 0.05f;
                                scale->z += 0.05f;
                            }
                        } else if(HuPadBtn[0] & PAD_TRIGGER_L) {
                            if(HuPadSubStkX[0] <= -29) {
                                scale->x -= 0.01f;
                            } else if(HuPadSubStkX[0] >= 29) {
                                scale->x += 0.01f;
                            }
                            if(HuPadStkY[0] <= -36) {
                                scale->y -= 0.01f;
                            } else if(HuPadStkY[0] >= 36) {
                                scale->y += 0.01f;
                            }
                            if(HuPadSubStkY[0] <= -29) {
                                scale->z += 0.01f;
                            } else if(HuPadSubStkY[0] >= 29) {
                                scale->z -= 0.01f;
                            }
                        } else {
                            if(HuPadSubStkX[0] <= -29) {
                                scale->x -= 0.1f;
                            } else if(HuPadSubStkX[0] >= 29) {
                                scale->x += 0.1f;
                            }
                            if(HuPadStkY[0] <= -36) {
                                scale->y -= 0.1f;
                            } else if(HuPadStkY[0] >= 36) {
                                scale->y += 0.1f;
                            }
                            if(HuPadSubStkY[0] <= -29) {
                                scale->z += 0.1f;
                            } else if(HuPadSubStkY[0] >= 29) {
                                scale->z -= 0.1f;
                            }
                        }
                    }
                }
                HuSprPosSet(grpId, MDSPR_GRP_MEMBERNO(MDSPR_GRP_CURSOR_LR, MDSPR_CURSOR_LR_R), 32, 0);
                HuSprPosSet(grpId, MDSPR_GRP_MEMBERNO(MDSPR_GRP_CURSOR_LR, MDSPR_CURSOR_LR_L), -32, 0);
                sprRot += 2;
                MdMathAngleClamp(&sprRot);
                HuSprGrpZRotSet(grpId, sprRot);
                Hu3D3Dto2D(pos, camera->camera[mdlInfoP->cameraNo].bit, &pos2D);
                HuSprGrpPosSet(grpId, pos2D.x, pos2D.y);
                if(attr & HU3D_ATTR_DISPOFF) {
                    HuSprAttrSet(grpId, MDSPR_GRP_MEMBERNO(MDSPR_GRP_CURSOR_LR, MDSPR_CURSOR_LR_R), HUSPR_ATTR_ADDCOL);
                    HuSprAttrSet(grpId, MDSPR_GRP_MEMBERNO(MDSPR_GRP_CURSOR_LR, MDSPR_CURSOR_LR_L), HUSPR_ATTR_ADDCOL);
                    HuSprGrpTPLvlSet(grpId, 0.4f);
                } else {
                    HuSprAttrReset(grpId, MDSPR_GRP_MEMBERNO(MDSPR_GRP_CURSOR_LR, MDSPR_CURSOR_LR_R), HUSPR_ATTR_ADDCOL);
                    HuSprAttrReset(grpId, MDSPR_GRP_MEMBERNO(MDSPR_GRP_CURSOR_LR, MDSPR_CURSOR_LR_L), HUSPR_ATTR_ADDCOL);
                    HuSprGrpTPLvlSet(grpId, 1.0f);
                }
                MdDebugFontPrint(debugFont, "POS-ADJUST-MODE(%2d / %2d) -> %s", mdlSel, MDMODEL_MAX, mdlInfoP->name);
                if(debugFont->ctrl == MD_DEBUGFONT_CTRL_MODEL) {
                    if(menuSel == 0) {
                        MdDebugFontPrint(debugFont, "*  POS(%.1f / %.1f / %.1f)", pos->x, pos->y, pos->z);
                    } else {
                        MdDebugFontPrint(debugFont, "   POS(%.1f / %.1f / %.1f)", pos->x, pos->y, pos->z);
                    }
                    if(menuSel == 1) {
                        MdDebugFontPrint(debugFont, "*  ROT(%.1f / %.1f / %.1f)", rot->x, rot->y, rot->z);
                    } else {
                        MdDebugFontPrint(debugFont, "   ROT(%.1f / %.1f / %.1f)", rot->x, rot->y, rot->z);
                    }
                    if(menuSel == 2) {
                        MdDebugFontPrint(debugFont, "*SCALE(%.2f / %.2f / %.2f)", scale->x, scale->y, scale->z);
                    } else {
                        MdDebugFontPrint(debugFont, " SCALE(%.2f / %.2f / %.2f)", scale->x, scale->y, scale->z);
                    }
                }
                HuPrcVSleep();
            }
        }
    }
}

s8 capPlayerInitF;
