#include "REL/rescardDll/rescard.h"
#include "datadir_enum.h"
#include "game/audio.h"
#include "game/hsfex.h"
#include "game/gamework.h"
#include "game/gamemes.h"
#include "game/frand.h"
#include "game/memory.h"
#include "dolphin/os.h"
#include "math.h"

typedef void (*VoidFunc)(void);

extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

/* Particle helper block, shared by ParticleDotCreate and ParticleGlowCreate:
 * both HuMemDirectMallocNum a 0xC-byte record and store HU3DPARMANIDs in it.
 * The ids are held as s32 but every use goes through the s16 HU3DPARMANID
 * parameter, which is where the extsh after each lwz comes from. */
typedef struct RescardEffect_s {
    int parManId[3];
} RESCARD_EFFECT;

/* effect helpers (static, defined first so the callers inline them) */
static RESCARD_EFFECT *ParticleDotCreate(ANIMDATA *animP, s16 layer, s16 cameraBit)
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
            { 0xFF, 0xFF, 0xFF, 0x20 },
        },
        {
            { 0x40, 0x20, 0x20, 0x00 },
        }
    };
    HU3DPARMANID parManId;
    RESCARD_EFFECT *effectP;
    effectP = HuMemDirectMallocNum(HEAP_HEAP, sizeof(RESCARD_EFFECT), HU_MEMNUM_OVL);
    parManId = Hu3DParManCreate(animP, 100, &param1);
    effectP->parManId[0] = parManId;
    Hu3DParManAttrSet(parManId, HU3D_PARMAN_ATTR_SCALEJITTER|HU3D_PARMAN_ATTR_RANDSCALE70|HU3D_PARMAN_ATTR_RANDSPEED70|HU3D_PARMAN_ATTR_TIMEUP);
    Hu3DParticleBlendModeSet(Hu3DParManModelIDGet(parManId), HU3D_PARTICLE_BLEND_ADDCOL);
    Hu3DParManRotSet(parManId, 90, 0, 0);
    Hu3DModelLayerSet(Hu3DParManModelIDGet(parManId), layer);
    Hu3DModelCameraSet(Hu3DParManModelIDGet(parManId), cameraBit);
    parManId = Hu3DParManCreate(animP, 100, &param2);
    effectP->parManId[1] = parManId;
    Hu3DParManAttrSet(parManId, HU3D_PARMAN_ATTR_RANDANGLE|HU3D_PARMAN_ATTR_SCALEJITTER|HU3D_PARMAN_ATTR_RANDSCALE70|HU3D_PARMAN_ATTR_RANDSPEED90|HU3D_PARMAN_ATTR_TIMEUP);
    Hu3DParticleBlendModeSet(Hu3DParManModelIDGet(parManId), HU3D_PARTICLE_BLEND_ADDCOL);
    Hu3DParManRotSet(parManId, frandmod(40)+70, 0, frandmod(80)-40);
    Hu3DModelLayerSet(Hu3DParManModelIDGet(parManId), layer);
    Hu3DModelCameraSet(Hu3DParManModelIDGet(parManId), cameraBit);
    parManId = Hu3DParManCreate(animP, 100, &param3);
    effectP->parManId[2] = parManId;
    Hu3DParManAttrSet(parManId, HU3D_PARMAN_ATTR_RANDSCALE70|HU3D_PARMAN_ATTR_TIMEUP);
    Hu3DParticleBlendModeSet(Hu3DParManModelIDGet(parManId), HU3D_PARTICLE_BLEND_ADDCOL);
    Hu3DParManRotSet(parManId, 0, 0, 0);
    Hu3DModelLayerSet(Hu3DParManModelIDGet(parManId), layer);
    Hu3DModelCameraSet(Hu3DParManModelIDGet(parManId), cameraBit);
    return effectP;
}

static void ParticleDotPosSet(RESCARD_EFFECT *effectP, HuVecF *pos)
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

static void ParticleDotKill(RESCARD_EFFECT *effectP)
{
    Hu3DParManKill(effectP->parManId[0]);
    Hu3DParManKill(effectP->parManId[1]);
    Hu3DParManKill(effectP->parManId[2]);
}

static RESCARD_EFFECT *ParticleGlowCreate(ANIMDATA *animP, s16 layer, s16 cameraBit)
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
            { 128, 192, 255, 255 },
            { 255, 255, 64, 255 },
            { 255, 96, 128, 255 },
        },
        {
            { 128, 192, 255, 0 },
            { 255, 64, 32, 0 },
            { 128, 16, 16, 0 },
        }
    };
    HU3DPARMANID parManId;
    RESCARD_EFFECT *effectP;
    effectP = HuMemDirectMallocNum(HEAP_HEAP, sizeof(RESCARD_EFFECT), HU_MEMNUM_OVL);
    parManId = Hu3DParManCreate(animP, 100, &param);
    effectP->parManId[0] = parManId;
    Hu3DParManAttrSet(parManId, HU3D_PARMAN_ATTR_SCALEJITTER|HU3D_PARMAN_ATTR_RANDSCALE70|HU3D_PARMAN_ATTR_RANDSPEED70|HU3D_PARMAN_ATTR_TIMEUP);
    Hu3DParticleBlendModeSet(Hu3DParManModelIDGet(parManId), HU3D_PARTICLE_BLEND_ADDCOL);
    Hu3DParManRotSet(parManId, 90, 0, 0);
    Hu3DModelLayerSet(Hu3DParManModelIDGet(parManId), layer);
    Hu3DModelCameraSet(Hu3DParManModelIDGet(parManId), cameraBit);
    return effectP;
}

static void ParticleGlowPosSet(RESCARD_EFFECT *effectP, HuVecF *pos)
{
    Hu3DParManPosSet(effectP->parManId[0], pos->x, pos->y, pos->z);
    Hu3DParManTimeLimitSet(effectP->parManId[0], 2);
    Hu3DParManAttrReset(effectP->parManId[0], HU3D_PARMAN_ATTR_TIMEUP);
}

static void ParticleGlowKill(RESCARD_EFFECT *effectP)
{
    Hu3DParManKill(effectP->parManId[0]);
}


/* one star/zero-star model orbiting the winning pin */
typedef struct RescardOrbit_s {
    s8 unk0;
    s8 unk1;
    s16 mdl; //model slot, -1 = unused
    float angle; //0x04
    float scale; //0x08
} RESCARD_ORBIT;

enum {
    RESCARD_OBJ_DEBUGFONT,
    RESCARD_OBJ_PAD,
    RESCARD_OBJ_CAMERA,
    RESCARD_OBJ_LIGHT,
    RESCARD_OBJ_MESS
};

enum {
    RESCARD_LOAD_NONE,
    RESCARD_LOAD_READ,
    RESCARD_LOAD_DONE
};

static void ResCardFlowInit(void);
static void ResCardFlowScore(void);
static void ResCardFlowDraw(void);
static void ResCardFlowWin(void);
static s32 ResCardProcReset(void);
static s32 ResCardProcDarken(void);
static s32 ResCardProcPlayerFocus(void);
static s32 ResCardProcStageFocus(void);

void ObjectSetup(void);
void MdMain(void);
static void LoadProc(void);
void MdInit(void);
void MdClose(void);
void ResCardScoreCreate(void);
void ResCardScoreUpdate(void);
void ResCardDrawStarCreate(void);
void ResCardDrawUpdate(void);
void ResCardDrawCursorReset(s32 playerNo);
void ResCardSpotCreate(void);
void ResCardSpotUpdate(void);
void ResCardStarUpdate(void);
void ResCardPinUpdate(void);
void ResCardPinMotStart(s32 playerNo);
void ResCardAudExec(u32 no);
void ResCardDrawStarUpdate(void);
s32 ResCardDrawStkDirGet(void);
s32 ResCardDrawCursorModeSet(s32 mode);
void ResCardDrawTimerPosSet(void);
void ResCardDrawCursorMove(s32 dir);
void ResCardSpotScaleCalc(void);
void ResCardBillboardUpdate(s32 modelNo, s32 cameraNo);
s16 ResCardFXPanGet(s32 modelNo, s32 cameraNo);
void ResCardBGMReset(void);
void ResCardWinVoicePlay(void);

typedef struct MdModelDebugInfo_s {
    char *name; //0x00
    int cameraNo; //0x04
} MDMODEL_DEBUG_INFO;

extern void MdDebugFontPrint(MDDEBUGFONT_WORK *debugFont, const char *str, ...);
void MdPosAdjustMode(void);
extern void MdLightSetupModel(MDLIGHT_WORK *lightWork);
extern void MdMathStdCurveInit(void);
extern void MdMathOscillate(int type, float *time, float *t, float step);
extern void MdMathLerp(float *out, float a, float b, float t);
extern void MdMathLerpScale(HuVecF *out, float a, float b, float t);
extern void MdMathLerpScaleOfs(HuVecF *out, float a, float b, float t, float ofs);
extern void MdMathAngleClamp(float *angle);
extern Mtx *MdMathMatrixDir(HuVecF *start, HuVecF *end);
extern void MdObjDebugFontCreate(OMOBJ *obj);
extern void MdObjPadMain(OMOBJ *obj);
extern void MdObjCameraCreate(OMOBJ *obj);
extern void MdObjLightCreate(OMOBJ *obj);
extern void MdObjMessCreate(OMOBJ *obj);
extern void MdMessKill(void);
extern void MdMessWinOpen(MDMESS_WORK *mess, int winNo, float posX, float posY, s16 w, s16 h, float bgTPLvl, s16 drawNo);
extern void MdMessHelpWinCreate(MDMESS_WORK *mess, int winNo, u32 mesNo, float posX, float posY, s16 w, s16 h, float bgTPLvl, s16 drawNo);
extern void MdMessWinClose(MDMESS_WORK *mess, int winNo);
extern void MdMessWinKill(MDMESS_WORK *mess, int winNo);
extern void MdMessWinMesSet(MDMESS_WORK *mess, int winNo, u32 mesNo);
extern void MdMessWinInsertMesSet(MDMESS_WORK *mess, int winNo, u32 mesNo, u32 insertMesNo);
extern void MdMessWinHomeClear(MDMESS_WORK *mess, int winNo);
extern BOOL MdMessWinCheck(MDMESS_WORK *mess, int winNo);
extern s16 MdMessWinChoiceGet(MDMESS_WORK *mess, int winNo);
extern void MdSpriteCreate(MDSPRITE_WORK *sprite);
extern void MdSpriteDispSet(MDSPRITE_WORK *sprite, int grpNo, BOOL dispF);
extern void MdModelCreate(MDMODEL_WORK *model, MDCAMERA_WORK *camera);
extern void MdModelRemoveAmb(MDMODEL_WORK *model, HU3DMODELID mdlId);
extern void MdModelUpdateAmb(MDMODEL_WORK *model);

s8 winVoiceFlag[4];
SYS sysWork;
SYS *sys;
SYS *sysP;

/* bss shared between the functions */
s32 lbl_1_bss_0; //bgm-reset state machine
s32 lbl_1_bss_4; //flow wait counter
s32 lbl_1_bss_8;
s16 lbl_1_bss_C;
s32 lbl_1_bss_10;
s32 lbl_1_bss_40; //bgm stream id
f32 lbl_1_bss_3BC;
f32 lbl_1_bss_3C0;
f32 lbl_1_bss_3C4;
RESCARD_FOCUS lbl_1_bss_3C8;
s16 lbl_1_bss_34[6];
RESCARD_EFFECT *lbl_1_bss_44[33];
s8 lbl_1_bss_C8;
RESCARD_ORBIT lbl_1_bss_CC[33];
HuVecF lbl_1_bss_258;
HuVecF lbl_1_bss_35C[4];
f32 lbl_1_bss_264[2];
f32 lbl_1_bss_26C[2];
RESCARD_EFFECT *lbl_1_bss_274[7];
HuVecF lbl_1_bss_290;
HuVecF lbl_1_bss_29C[8][2];
HuVecF lbl_1_bss_38C[4];
s32 lbl_1_bss_3F8; //first top-rank player

static void (*MdFlowTbl[4])(void) = {
    ResCardFlowInit,
    ResCardFlowScore,
    ResCardFlowDraw,
    ResCardFlowWin
};

static s32 (*MdProcTbl[4])(void) = {
    ResCardProcReset,
    ResCardProcDarken,
    ResCardProcPlayerFocus,
    ResCardProcStageFocus
};

/* 0x0 - module init: walk the static-constructor table, then bring the
 * overlay's objects up. */
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

/* 0x54 - module fini: walk the static-destructor table. */
void _epilog(void)
{
    const VoidFunc *dtors = _dtors;

    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

/* 0xA0 - build the object manager, the generic Md* work objects and the
 * sprite/model work areas, then start MdMain and the loader. */
void ObjectSetup(void)
{
    int i;

    sys = &sysWork;
    sysP = sys;
    sys->unk00 = 0;
    sys->unk04 = 0;
    sys->unk08 = 0;
    sys->frame = 0;
    sys->flow = 0;
    sys->retMode = 0;
    sys->ovl = (OMOVL)0x70;
    sys->exitF = 0;
    sys->skipWipeF = 0;
    sys->score = HuMemDirectMallocNum(HEAP_HEAP, sizeof(RESCARD_SCORE), HU_MEMNUM_OVL);
    if(sys->score == NULL) {
        OSReport("sys->score = NULL...\n");
    }
    ResCardScoreCreate();
    sys->objman = omInitObjMan(512, 8192);
    omGameSysInit(sys->objman);
    sys->obj[RESCARD_OBJ_DEBUGFONT] = omAddObjEx(sys->objman, 100, 0, 0, -1, MdObjDebugFontCreate);
    sys->obj[RESCARD_OBJ_DEBUGFONT]->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDDEBUGFONT_WORK), HU_MEMNUM_OVL);
    if(sys->obj[RESCARD_OBJ_DEBUGFONT]->data == NULL) {
        OSReport("sys->obj[RESCARD_OBJ_DEBUGFONT]->data = NULL...\n");
    }
    sys->debugFont = sys->obj[RESCARD_OBJ_DEBUGFONT]->data;
    sys->debugFont->createF = FALSE;
    sys->obj[RESCARD_OBJ_PAD] = omAddObjEx(sys->objman, 101, 0, 0, -1, MdObjPadMain);
    sys->obj[RESCARD_OBJ_PAD]->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDPAD_WORK), HU_MEMNUM_OVL);
    if(sys->obj[RESCARD_OBJ_PAD]->data == NULL) {
        OSReport("sys->obj[RESCARD_OBJ_PAD]->data = NULL...\n");
    }
    sys->padWork = sys->obj[RESCARD_OBJ_PAD]->data;
    sys->obj[RESCARD_OBJ_CAMERA] = omAddObjEx(sys->objman, 102, 0, 0, -1, MdObjCameraCreate);
    sys->obj[RESCARD_OBJ_CAMERA]->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDCAMERA_WORK), HU_MEMNUM_OVL);
    if(sys->obj[RESCARD_OBJ_CAMERA]->data == NULL) {
        OSReport("sys->obj[RESCARD_OBJ_CAMERA]->data = NULL...\n");
    }
    sys->camera = sys->obj[RESCARD_OBJ_CAMERA]->data;
    sys->camera->createF = FALSE;
    sys->camera->debugFont = sys->debugFont;
    sys->obj[RESCARD_OBJ_LIGHT] = omAddObjEx(sys->objman, 103, 0, 0, -1, MdObjLightCreate);
    sys->obj[RESCARD_OBJ_LIGHT]->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDLIGHT_WORK), HU_MEMNUM_OVL);
    if(sys->obj[RESCARD_OBJ_LIGHT]->data == NULL) {
        OSReport("sys->obj[RESCARD_OBJ_LIGHT]->data = NULL...\n");
    }
    sys->light = sys->obj[RESCARD_OBJ_LIGHT]->data;
    sys->light->createF = FALSE;
    sys->light->debugFont = sys->debugFont;
    sys->obj[RESCARD_OBJ_MESS] = omAddObjEx(sys->objman, 104, 0, 0, -1, MdObjMessCreate);
    sys->obj[RESCARD_OBJ_MESS]->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDMESS_WORK), HU_MEMNUM_OVL);
    if(sys->obj[RESCARD_OBJ_MESS]->data == NULL) {
        OSReport("sys->obj[RESCARD_OBJ_MESS]->data = NULL...\n");
    }
    sys->mess = sys->obj[RESCARD_OBJ_MESS]->data;
    sys->mess->createF = FALSE;
    sys->sprite = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDSPRITE_WORK), HU_MEMNUM_OVL);
    if(sys->sprite == NULL) {
        OSReport("sys->sprite = NULL...\n");
    }
    sys->model = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDMODEL_WORK), HU_MEMNUM_OVL);
    if(sys->model == NULL) {
        OSReport("sys->model = NULL...\n");
    }
    for(i=0; i<MDMODEL_AMB_MAX; i++) {
        sys->model->ambId[i] = -1;
    }
    MdMathStdCurveInit();
    Hu3DBGColorSet(0, 0, 0);
    Hu3DNoSyncSet(FALSE);
    while(sys->debugFont->createF != TRUE || sys->camera->createF != TRUE || sys->mess->createF != TRUE) {
        HuPrcVSleep();
    }
    sys->mainProc = HuPrcChildCreate(MdMain, 105, 12288, 0, sys->objman);
    sys->loadState = RESCARD_LOAD_NONE;
    sys->loadAsyncStat = 0;
    sys->dataDir = 0x940000;
    sys->loadProc = HuPrcChildCreate(LoadProc, 110, 12288, 0, sys->objman);
}

/* overlay main loop: run the current flow function and the per-frame
 * updates until the overlay is asked to exit. */
void MdMain(void)
{
    MdInit();
    while(!omSysExitReq && !sys->exitF) {
        MdFlowTbl[sys->flow]();
        ResCardScoreUpdate();
        ResCardDrawUpdate();
        MdModelUpdateAmb(sys->model);
        ResCardSpotUpdate();
        if(++sys->frame > 36000) {
            sys->frame = 0;
        }
        HuPrcVSleep();
    }
    MdClose();
}

/* background loader: reads the next overlay's data directory. */
static void LoadProc(void)
{
    OSTick tick;

    while(1) {
        switch(sys->loadState) {
            case RESCARD_LOAD_READ:
                tick = OSGetTick();
                sys->loadAsyncStat = HuDataDirReadAsync(sys->dataDir);
                while(!HuDataGetAsyncStat(sys->loadAsyncStat)) {
                    HuPrcVSleep();
                }
                OSReport("Load Time:%d\n", OSTicksToMilliseconds(OSGetTick()-tick));
                OSReport("Finished!\n");
                sys->loadState = RESCARD_LOAD_DONE;
                break;

            case RESCARD_LOAD_DONE:
                HuPrcEnd();
                break;
        }
        HuPrcVSleep();
    }
}

/* create sprites, models, light and the helper processes. */
void MdInit(void)
{
    MdSpriteCreate(sys->sprite);
    HuPrcVSleep();
    MdModelCreate(sys->model, sys->camera);
    HuPrcVSleep();
    MdLightSetupModel(sys->light);
    HuPrcVSleep();
    sys->timer = MgTimerCreate(0);
    MgTimerDispOff(sys->timer);
    ResCardSpotCreate();
    HuPrcVSleep();
    HuDataDirClose(DATA_rescard);
    HuPrcVSleep();
    sys->score->info.unkE0 = 0;
    sys->score->info.phase = 1;
    sys->starProc = HuPrcChildCreate(ResCardStarUpdate, 106, 12288, 0, sys->objman);
    sys->pinProc = HuPrcChildCreate(ResCardPinUpdate, 107, 12288, 0, sys->objman);
    sys->bgmProc = HuPrcChildCreate(ResCardBGMReset, 108, 4096, 0, sys->objman);
    sys->voiceProc = HuPrcChildCreate(ResCardWinVoicePlay, 109, 4096, 0, sys->objman);
    HuPrcVSleep();
}

/* leave the overlay: wipe out, stop the audio and jump on. */
void MdClose(void)
{
    if(sys->skipWipeF == 0) {
        WipeCreate(2, 0, 60);
        while(WipeCheckEnd()) {
            HuPrcVSleep();
        }
    }
    ResCardAudExec(28);
    MdMessKill();
    if(sys->score->info.mesId >= 0) {
        GameMesKill(sys->score->info.mesId);
    }
    MgTimerKill(sys->timer);
    if(sys->retMode == 0) {
        omOvlGotoEx(sys->ovl, 1, 0, 0);
    } else if(sys->retMode == 1) {
        omOvlCallEx(sys->ovl, 1, 0, 0);
    } else if(sys->retMode == 2) {
        omOvlReturnEx(1, 1);
    }
    HuPrcEnd();
    while(1) {
        HuPrcVSleep();
    }
}

/* flow 0: hide everything, place the pins and pedestals, fade in. */
static void ResCardFlowInit(void)
{
    MDSPRITE_WORK *sprite = sys->sprite;
    MDMODEL_WORK *model = sys->model;
    int i;

    MdSpriteDispSet(sprite, 0, FALSE);
    MdSpriteDispSet(sprite, 1, FALSE);
    MdSpriteDispSet(sprite, 2, FALSE);
    MdSpriteDispSet(sprite, 3, FALSE);
    MdSpriteDispSet(sprite, 4, FALSE);
    MdSpriteDispSet(sprite, 5, FALSE);
    MdSpriteDispSet(sprite, 6, FALSE);
    Hu3DModelAttrReset(model->mdlId[MDMODEL_STAR], HU3D_ATTR_DISPOFF);
    for(i=0; i<6; i++) {
        Hu3DModelAttrReset(model->mdlId[MDMODEL_STAGE+i], HU3D_ATTR_DISPOFF);
    }
    for(i=0; i<4; i++) {
        Hu3DModelAttrReset(model->mdlId[MDMODEL_PIN+i], HU3D_ATTR_DISPOFF);
    }
    for(i=0; i<4; i++) {
        Hu3DModelAttrSet(model->mdlId[MDMODEL_CUR+i], HU3D_ATTR_DISPOFF);
    }
    for(i=0; i<4; i++) {
        Hu3DModelAttrReset(model->mdlId[MDMODEL_PEDESTAL+i], HU3D_ATTR_DISPOFF);
    }
    for(i=0; i<2; i++) {
        Hu3DModelAttrSet(model->mdlId[MDMODEL_POP_STAR+i], HU3D_ATTR_DISPOFF);
    }
    for(i=0; i<22; i++) {
        Hu3DModelAttrSet(model->mdlId[MDMODEL_WIN_STAR+i], HU3D_ATTR_DISPOFF);
    }
    for(i=0; i<11; i++) {
        Hu3DModelAttrSet(model->mdlId[MDMODEL_WIN_ZTAR+i], HU3D_ATTR_DISPOFF);
    }
    for(i=0; i<4; i++) {
        Hu3DModelAttrSet(model->mdlId[MDMODEL_CARD_STAR+i], HU3D_ATTR_DISPOFF);
    }
    for(i=0; i<4; i++) {
        Hu3DModelAttrSet(model->mdlId[MDMODEL_CARD_ZTAR+i], HU3D_ATTR_DISPOFF);
    }
    for(i=0; i<4; i++) {
        Hu3DModelAttrSet(model->mdlId[MDMODEL_CARD_SZTAR+i], HU3D_ATTR_DISPOFF);
    }
    for(i=0; i<8; i++) {
        Hu3DModelAttrSet(model->mdlId[MDMODEL_DRAW+i], HU3D_ATTR_DISPOFF);
    }
    Hu3DModelAttrSet(model->mdlId[MDMODEL_EFF_STARFLASH], HU3D_ATTR_DISPOFF);
    for(i=0; i<4; i++) {
        Hu3DModelAttrSet(model->mdlId[MDMODEL_EFF_DICEFLASH+i], HU3D_ATTR_DISPOFF);
    }
    for(i=0; i<4; i++) {
        Hu3DModelAttrSet(model->mdlId[MDMODEL_EFF_CARDFLASH+i], HU3D_ATTR_DISPOFF);
    }
    Hu3DModelAttrSet(model->mdlId[MDMODEL_RING], HU3D_ATTR_DISPOFF);
    for(i=0; i<4; i++) {
        Hu3DModelAttrSet(model->mdlId[MDMODEL_SPOT+i], HU3D_ATTR_DISPOFF);
    }
    for(i=0; i<4; i++) {
        model->posP[MDMODEL_PIN+i]->x = -450.0f+(300.0f*i);
        model->posP[MDMODEL_PIN+i]->y = 100;
        model->posP[MDMODEL_PIN+i]->z = 0;
    }
    for(i=0; i<4; i++) {
        model->posP[MDMODEL_PEDESTAL+i]->x = -450.0f+(300.0f*i);
        model->posP[MDMODEL_PEDESTAL+i]->y = 0;
        model->posP[MDMODEL_PEDESTAL+i]->z = 0;
    }
    MdProcTbl[0]();
    WipeColorSet(255, 255, 255);
    WipeCreate(1, 0, 30);
    ResCardAudExec(0);
    while(WipeCheckEnd()) {
        HuPrcVSleep();
    }
    sys->unk00 = 0;
    sys->unk04 = 0;
    sys->flow = 1;
}

/* flow 1: announce the star counts, fly the cards in, find the top rank. */
static void ResCardFlowScore(void)
{
    MDMESS_WORK *mess = sys->mess;
    RESCARD_SCORE *score = sys->score;
    int cnt;
    int i;

    switch(sys->unk00) {
        case 0:
            MdMessWinOpen(mess, 1, 48, 376, 480, 64, 1.0f, 0);
            MdMessWinMesSet(mess, 1, 0x150011);
            ResCardAudExec(25);
            score->info.phase = 2;
            lbl_1_bss_4 = 0;
            sys->unk00 = 1;
            break;

        case 1:
            if(MdMessWinCheck(mess, 1)) {
                MdProcTbl[0]();
                ResCardAudExec(1);
                MdMessWinKill(mess, 1);
                score->info.phase = 3;
                sys->unk00 = 8;
            }
            break;

        case 8:
            if(MdProcTbl[1]() == 1) {
                MdProcTbl[0]();
                for(i=0; i<4; i++) {
                    sys->pin[i].state = 1;
                }
                ResCardAudExec(5);
                sys->unk00 = 2;
            }
            break;

        case 2:
            for(i=0; i<4; i++) {
                score->player[i].state = 1;
            }
            sys->unk00 = 3;
            break;

        case 3:
            if(score->player[0].state == 3) {
                sys->unk00 = 11;
            }
            break;

        case 11:
            if(++lbl_1_bss_4 >= 120) {
                lbl_1_bss_4 = 0;
                sys->unk00 = 4;
            }
            break;

        case 12:
            if(++lbl_1_bss_4 >= 30) {
                lbl_1_bss_4 = 0;
                sys->unk00 = 4;
            }
            break;

        case 4:
            cnt = 0;
            lbl_1_bss_3F8 = -1;
            for(i=0; i<4; i++) {
                if(score->info.rank[i] == score->info.rankNum) {
                    if(score->info.rank[i] != 1) {
                        ResCardPinMotStart(i);
                        sys->pin[i].state = 7;
                    } else {
                        sys->pin[i].state = 4;
                    }
                    score->player[i].state = 4;
                    if(lbl_1_bss_3F8 == -1) {
                        lbl_1_bss_3F8 = i;
                    }
                    cnt++;
                }
            }
            if(score->info.rankNum > 1 && score->info.rankMax > 1) {
                score->info.rankNum--;
            } else if(score->info.rankNum > 1 && score->info.rankMax == 1 && cnt == 0) {
                score->info.rankNum--;
            }
            if(cnt != 0) {
                sys->unk00 = 5;
            }
            break;

        case 5:
            if(score->player[lbl_1_bss_3F8].state == 7) {
                if(score->info.rankNum == 1) {
                    sys->unk00 = 6;
                } else {
                    sys->unk00 = 11;
                }
                if(score->info.rankNum > 1 && score->info.rankMax == 1) {
                    score->info.rankNum--;
                }
                if(score->info.rankNum == 1 && score->info.rankMax == 1 && sys->unk00 == 11) {
                    sys->unk00 = 12;
                }
            }
            break;

        case 6:
            if(score->info.rankMax > 1) {
                sys->unk00 = 0;
                sys->flow = 2;
                break;
            }
            for(i=0; i<4; i++) {
                if(score->info.rank[i] == 1) {
                    score->draw.sel = i;
                    break;
                }
            }
            MdMessWinOpen(mess, 1, 48, 376, 480, 64, 1.0f, 0);
            MdMessWinInsertMesSet(mess, 1, 0x150018, score->player[score->draw.sel].charNo);
            ResCardAudExec(27);
            score->info.phase = 2;
            sys->unk00 = 7;
            break;

        case 7:
            if(MdMessWinCheck(mess, 1)) {
                for(i=0; i<4; i++) {
                    score->player[i].state = 8;
                }
                sys->unk00 = 0;
                sys->flow = 3;
            }
            break;
    }
}

/* flow 2: several players share the top rank, announce the card draw. */
static void ResCardFlowDraw(void)
{
    MDMESS_WORK *mess = sys->mess;
    RESCARD_SCORE *score = sys->score;
    int i;

    switch(sys->unk00) {
        case 0:
            sys->unk00 = 1;
            break;

        case 1:
            MdMessWinOpen(mess, 1, 48, 376, 480, 64, 1.0f, 0);
            MdMessWinInsertMesSet(mess, 1, 0x150014, score->info.rankMax+0x150017);
            ResCardAudExec(26);
            score->info.phase = 2;
            ResCardAudExec(2);
            sys->unk00 = 2;
            break;

        case 2:
            if(MdMessWinCheck(mess, 1)) {
                MdMessWinHomeClear(mess, 1);
                MdMessWinMesSet(mess, 1, 0x150015);
                sys->unk00 = 3;
            }
            break;

        case 3:
            if(MdMessWinCheck(mess, 1)) {
                for(i=0; i<4; i++) {
                    score->player[i].state = 8;
                }
                score->draw.phase = 1;
                sys->unk00 = 4;
            }
            break;

        case 4:
            if(score->draw.phase == 12) {
                sys->unk00 = 0;
                sys->flow = 3;
            }
            break;
    }
}

/* flow 3: the winner is shown, ask whether to play again. */
static void ResCardFlowWin(void)
{
    MDMODEL_WORK *model = sys->model;
    MDMESS_WORK *mess = sys->mess;
    RESCARD_SCORE *score = sys->score;
    RESCARD_DRAW *draw = &score->draw;
    s16 choice;
    int i;

    switch(sys->unk00) {
        case 0:
            MdProcTbl[0]();
            Hu3DMotionSet(model->mdlId[MDMODEL_PIN+draw->sel], model->motId[draw->sel+2]);
            Hu3DModelAttrSet(model->mdlId[MDMODEL_PIN+draw->sel], HU3D_MOTATTR_LOOP);
            Hu3DModelAttrReset(model->mdlId[MDMODEL_PIN+draw->sel], HU3D_MOTATTR_PAUSE);
            Hu3DMotionSpeedSet(model->mdlId[MDMODEL_PIN+draw->sel], 2.0f);
            sys->unk00 = 1;
            break;

        case 1:
            if(MdProcTbl[2]() == 1) {
                MdProcTbl[0]();
                score->info.mesId = GameMesCreate(4, 3, score->player[draw->sel].charNo, -1, -1, -1);
                ResCardAudExec(4);
                score->info.unkEA = 1;
                sys->unk00 = 2;
            }
            break;

        case 2:
            if(GameMesStatGet(score->info.mesId) == 0) {
                sys->unk00 = 3;
            }
            break;

        case 3:
            if(MdProcTbl[3]() == 1) {
                MdProcTbl[0]();
                MdMessWinHomeClear(mess, 1);
                MdMessWinMesSet(mess, 1, 0x150012);
                sys->unk00 = 4;
            }
            break;

        case 4:
            choice = MdMessWinChoiceGet(mess, 1);
            switch(choice) {
                case -1:
                case 1:
                    MdMessWinHomeClear(mess, 1);
                    MdMessWinMesSet(mess, 1, 0x150013);
                    sys->loadState = RESCARD_LOAD_READ;
                    sys->dataDir = 0x910000;
                    sys->unk00 = 5;
                    break;

                case 0:
                    sys->loadState = RESCARD_LOAD_READ;
                    sys->dataDir = 0x110000;
                    for(i=0; i<4; i++) {
                        GwCardCommon.starNum[i] = 0;
                        GwCardCommon.ztarNum[i] = 0;
                    }
                    WipeColorSet(255, 255, 255);
                    WipeCreate(2, 0, 30);
                    ResCardAudExec(3);
                    sys->unk00 = 8;
                    break;
            }
            break;

        case 5:
            if(MdMessWinCheck(mess, 1)) {
                WipeColorSet(0, 0, 0);
                WipeCreate(2, 0, 30);
                ResCardAudExec(3);
                sys->unk00 = 6;
            }
            break;

        case 6:
            if(WipeCheckEnd() == 0) {
                sys->unk00 = 7;
            }
            break;

        case 7:
            if(sys->loadState == RESCARD_LOAD_DONE) {
                sys->retMode = 2;
                sys->ovl = (OMOVL)0x5E;
                sys->skipWipeF = 1;
                sys->exitF = 1;
            }
            break;

        case 8:
            if(WipeCheckEnd() == 0) {
                sys->unk00 = 9;
            }
            break;

        case 9:
            if(sys->loadState == RESCARD_LOAD_DONE) {
                sys->retMode = 0;
                sys->ovl = (OMOVL)2;
                sys->skipWipeF = 1;
                sys->exitF = 1;
            }
            break;
    }
}

/* re-arm the flow procs: clear the two proc counters and zero the four
 * cached floats. */
static s32 ResCardProcReset(void)
{
    sys->procState = 0;
    sys->procState2 = 0;
    sys->procT[0] = 0;
    sys->procT[1] = 0;
    sys->procOut[0] = 0;
    sys->procOut[1] = 0;
    return 0;
}

/* proc 1: darken the ambient light of the whole scene. */
static s32 ResCardProcDarken(void)
{
    MDMODEL_WORK *model = sys->model;

    MdMathOscillate(1, &sys->procT[0], &sys->procOut[0], 60);
    MdMathLerp(&model->ambScale, 1.0f, 0.45f, sys->procOut[0]);
    if(sys->procT[0] >= 1.0f) {
        return 1;
    }
    return 0;
}

/* proc 2: fly the camera to the winner's pin and spot-light it. */
static s32 ResCardProcPlayerFocus(void)
{
    MDMODEL_WORK *model = sys->model;
    MDCAMERA *camera = &sys->camera->camera[0];
    RESCARD_SCORE *score = sys->score;
    HuVecF *pos;

    if(sys->procState == 0) {
        lbl_1_bss_3C8.center.x = camera->center.x;
        lbl_1_bss_3C8.center.y = camera->center.y;
        lbl_1_bss_3C8.center.z = camera->center.z;
        lbl_1_bss_3C8.hvr.x = camera->hvr.x;
        lbl_1_bss_3C8.hvr.y = camera->hvr.y;
        lbl_1_bss_3C8.hvr.z = camera->hvr.z;
        pos = model->posP[MDMODEL_PIN+score->draw.sel];
        lbl_1_bss_3C8.centerEnd.x = pos->x;
        lbl_1_bss_3C8.centerEnd.y = pos->y;
        lbl_1_bss_3C8.centerEnd.z = pos->z;
        lbl_1_bss_3C8.centerEnd.y += 70;
        lbl_1_bss_3C8.hvrEnd.x = camera->hvr.x;
        lbl_1_bss_3C8.hvrEnd.y = camera->hvr.y;
        lbl_1_bss_3C8.hvrEnd.z = 1800;
        lbl_1_bss_3C4 = lbl_1_bss_3C0 = model->ambScale;
        lbl_1_bss_3BC = 1.0f;
        MdModelRemoveAmb(model, model->mdlId[MDMODEL_PIN+score->draw.sel]);
        sys->procState = 1;
    }
    MdMathOscillate(1, &sys->procT[0], &sys->procOut[0], 60);
    MdMathLerp(&camera->center.x, lbl_1_bss_3C8.center.x, lbl_1_bss_3C8.centerEnd.x, sys->procOut[0]);
    MdMathLerp(&camera->center.y, lbl_1_bss_3C8.center.y, lbl_1_bss_3C8.centerEnd.y, sys->procOut[0]);
    MdMathLerp(&camera->center.z, lbl_1_bss_3C8.center.z, lbl_1_bss_3C8.centerEnd.z, sys->procOut[0]);
    MdMathLerp(&camera->hvr.x, lbl_1_bss_3C8.hvr.x, lbl_1_bss_3C8.hvrEnd.x, sys->procOut[0]);
    MdMathLerp(&camera->hvr.y, lbl_1_bss_3C8.hvr.y, lbl_1_bss_3C8.hvrEnd.y, sys->procOut[0]);
    MdMathLerp(&camera->hvr.z, lbl_1_bss_3C8.hvr.z, lbl_1_bss_3C8.hvrEnd.z, sys->procOut[0]);
    MdMathOscillate(1, &sys->procT[1], &sys->procOut[1], 30);
    MdMathLerp(&lbl_1_bss_3C0, lbl_1_bss_3C4, 0.75f, sys->procOut[1]);
    MdMathLerp(&lbl_1_bss_3BC, 1.0f, 0.3f, sys->procOut[1]);
    Hu3DModelAmbSet(model->mdlId[MDMODEL_PIN+score->draw.sel], lbl_1_bss_3C0, lbl_1_bss_3C0, lbl_1_bss_3C0);
    Hu3DModelTPLvlSet(model->mdlId[MDMODEL_SPOT+score->draw.sel], lbl_1_bss_3BC);
    if(sys->procT[0] >= 1.0f) {
        return 1;
    }
    return 0;
}

/* proc 3: fly the camera back to where it started. */
static s32 ResCardProcStageFocus(void)
{
    MDCAMERA *camera = &sys->camera->camera[0];

    MdMathOscillate(1, &sys->procT[0], &sys->procOut[0], 60);
    MdMathLerp(&camera->center.x, lbl_1_bss_3C8.centerEnd.x, lbl_1_bss_3C8.center.x, sys->procOut[0]);
    MdMathLerp(&camera->center.y, lbl_1_bss_3C8.centerEnd.y, lbl_1_bss_3C8.center.y, sys->procOut[0]);
    MdMathLerp(&camera->center.z, lbl_1_bss_3C8.centerEnd.z, lbl_1_bss_3C8.center.z, sys->procOut[0]);
    MdMathLerp(&camera->hvr.x, lbl_1_bss_3C8.hvrEnd.x, lbl_1_bss_3C8.hvr.x, sys->procOut[0]);
    MdMathLerp(&camera->hvr.y, lbl_1_bss_3C8.hvrEnd.y, lbl_1_bss_3C8.hvr.y, sys->procOut[0]);
    MdMathLerp(&camera->hvr.z, lbl_1_bss_3C8.hvrEnd.z, lbl_1_bss_3C8.hvr.z, sys->procOut[0]);
    if(sys->procT[0] >= 1.0f) {
        return 1;
    }
    return 0;
}

/* unreferenced byte table kept for the data layout */
static s8 lbl_1_data_2FC[0x18] = {
    1, 0, 2, 0, 0, 0, 3, -1, 2, 0, 1, 1, 0, -1, 2, 0, 1, 2, 4, -1, 2, 0, 1, 0
};

/* seed the per-player score data from the game work and rank the players. */
void ResCardScoreCreate(void)
{
    RESCARD_SCORE *score = sys->score;
    int i;
    int j;

    for(i=0; i<4; i++) {
        score->player[i].charNo = GwPlayerConf[i].charNo;
        score->player[i].starNum = GwCardCommon.starNum[i];
        score->player[i].ztarNum = GwCardCommon.ztarNum[i];
        score->player[i].type = GwPlayerConf[i].type;
        if(score->player[i].type == 1) {
            score->player[i].padNo = -1;
        } else {
            score->player[i].padNo = GwPlayerConf[i].padNo;
        }
        score->player[i].pedestalF = GwCardCommon.pedestalF[i];
        score->player[i].state = 0;
        score->player[i].unk14 = 0;
    }
    for(i=0; i<4; i++) {
        score->info.rank[i] = 1;
        for(j=0; j<4; j++) {
            if((score->player[j].starNum-score->player[j].ztarNum) > (score->player[i].starNum-score->player[i].ztarNum)) {
                score->info.rank[i]++;
            }
        }
    }
    score->info.mesId = -1;
    score->info.rankMax = 0;
    score->info.rankNum = 1;
    for(i=0; i<4; i++) {
        if(score->info.rank[i] == 1) {
            score->info.rankMax++;
        }
        if(score->info.rankNum < score->info.rank[i]) {
            score->info.rankNum = score->info.rank[i];
        }
    }
    ResCardDrawStarCreate();
    score->info.unkE0 = 0;
    score->info.unkEA = 0;
}

/* per-player star count cards: pop the card model out above the pin, flip
 * it while the dice-flash plays, then show the star count sprite. */
void ResCardScoreUpdate(void)
{
    MDCAMERA_WORK *camera = sys->camera;
    MDMODEL_WORK *model = sys->model;
    MDSPRITE_WORK *sprite = sys->sprite;
    RESCARD_SCORE *score = sys->score;
    RESCARD_PLAYER *player;
    HUSPRGRPID grpId[4];
    HuVecF *pinPos;
    HuVecF *cardPos;
    HuVecF *cardRot;
    HuVecF *cardScale;
    HuVecF *flashPos;
    HuVecF *flashRot;
    HuVecF *flashScale;
    int num;
    int i;

    for(i=0; i<4; i++) {
        grpId[i] = sprite->grpId[i+2];
    }
    player = &score->player[0];
    for(i=0; i<4; i++, player++) {
        pinPos = model->posP[MDMODEL_PIN+i];
        cardPos = model->posP[MDMODEL_CARD_SZTAR+i];
        cardRot = model->rotP[MDMODEL_CARD_SZTAR+i];
        cardScale = model->scaleP[MDMODEL_CARD_SZTAR+i];
        flashPos = model->posP[MDMODEL_EFF_DICEFLASH+i];
        flashRot = model->rotP[MDMODEL_EFF_DICEFLASH+i];
        flashScale = model->scaleP[MDMODEL_EFF_DICEFLASH+i];
        switch(player->state) {
            case 1:
                lbl_1_bss_35C[i] = *pinPos;
                lbl_1_bss_35C[i].y += 300;
                Hu3D3Dto2D(&lbl_1_bss_35C[i], camera->camera[0].bit, &lbl_1_bss_38C[i]);
                lbl_1_bss_38C[i].z = 2500;
                Hu3D2Dto3D(&lbl_1_bss_38C[i], camera->camera[1].bit, &lbl_1_bss_35C[i]);
                Hu3DModelAttrReset(model->mdlId[MDMODEL_CARD_SZTAR+i], HU3D_ATTR_DISPOFF);
                Hu3DModelAttrSet(model->mdlId[MDMODEL_EFF_DICEFLASH+i], HU3D_ATTR_DISPOFF);
                cardPos->x = lbl_1_bss_35C[i].x;
                cardPos->y = lbl_1_bss_35C[i].y;
                cardPos->z = lbl_1_bss_35C[i].z;
                flashPos->x = lbl_1_bss_35C[i].x;
                flashPos->y = lbl_1_bss_35C[i].y;
                flashPos->z = lbl_1_bss_35C[i].z;
                cardRot->x = 90;
                cardRot->y = frandmod(360)-180;
                cardRot->z = 0;
                flashRot->x = 0;
                flashRot->y = 0;
                flashRot->z = 0;
                cardScale->x = 0;
                cardScale->y = 0;
                cardScale->z = 0;
                flashScale->x = 1;
                flashScale->y = 1;
                flashScale->z = 1;
                Hu3DMotionTimeSet(model->mdlId[MDMODEL_EFF_DICEFLASH+i], 0);
                Hu3DModelAttrSet(model->mdlId[MDMODEL_EFF_DICEFLASH+i], HU3D_MOTATTR_PAUSE);
                MdSpriteDispSet(sprite, 2, FALSE);
                MdSpriteDispSet(sprite, 3, FALSE);
                MdSpriteDispSet(sprite, 4, FALSE);
                MdSpriteDispSet(sprite, 5, FALSE);
                player->oscT = 0;
                player->oscOut = 0;
                player->state = 2;
                break;

            case 2:
                cardRot->y += 16;
                MdMathAngleClamp(&cardRot->y);
                MdMathOscillate(1, &player->oscT, &player->oscOut, 60);
                MdMathLerpScale(cardScale, 0, 0.5f, player->oscOut);
                if(player->oscT >= 1.0f) {
                    player->state = 3;
                }
                break;

            case 3:
                cardRot->y += 16;
                MdMathAngleClamp(&cardRot->y);
                break;

            case 4:
                cardRot->y += 16;
                MdMathAngleClamp(&cardRot->y);
                Hu3DModelAttrReset(model->mdlId[MDMODEL_EFF_DICEFLASH+i], HU3D_ATTR_DISPOFF);
                Hu3DModelAttrReset(model->mdlId[MDMODEL_EFF_DICEFLASH+i], HU3D_MOTATTR_PAUSE);
                player->state = 5;
                break;

            case 5:
                cardRot->y += 16;
                MdMathAngleClamp(&cardRot->y);
                if(Hu3DMotionTimeGet(model->mdlId[MDMODEL_EFF_DICEFLASH+i]) >= 20.0f) {
                    Hu3DModelAttrSet(model->mdlId[MDMODEL_CARD_SZTAR+i], HU3D_ATTR_DISPOFF);
                    player->state = 6;
                }
                break;

            case 6:
                if(Hu3DMotionTimeGet(model->mdlId[MDMODEL_EFF_DICEFLASH+i]) >= 25.0f) {
                    num = player->starNum-player->ztarNum;
                    if(num < 0) {
                        HuSprBankSet(grpId[i], 0, 10);
                    }
                    if(num >= 10) {
                        HuSprBankSet(grpId[i], 1, (player->starNum-player->ztarNum)/10);
                    } else if(num <= -10) {
                        HuSprBankSet(grpId[i], 1, (num*-1)/10);
                    }
                    if(num == 0) {
                        HuSprBankSet(grpId[i], 2, 0);
                    } else if(num > 0) {
                        HuSprBankSet(grpId[i], 2, (player->starNum-player->ztarNum)%10);
                    } else if(num < 0) {
                        HuSprBankSet(grpId[i], 2, (num*-1)%10);
                    }
                    if(num < 0) {
                        HuSprAttrReset(grpId[i], 0, HUSPR_ATTR_DISPOFF);
                    }
                    if(num >= 10 || num <= -10) {
                        HuSprAttrReset(grpId[i], 1, HUSPR_ATTR_DISPOFF);
                    }
                    HuSprAttrReset(grpId[i], 2, HUSPR_ATTR_DISPOFF);
                    HuSprGrpPosSet(grpId[i], lbl_1_bss_38C[i].x, lbl_1_bss_38C[i].y);
                    if(num <= -10) {
                        HuSprPosSet(grpId[i], 0, -20, 0);
                        HuSprPosSet(grpId[i], 1, 0, 0);
                        HuSprPosSet(grpId[i], 2, 20, 0);
                    } else if(num <= -1) {
                        HuSprPosSet(grpId[i], 0, -10, 0);
                        HuSprPosSet(grpId[i], 2, 10, 0);
                    } else if(num >= 10) {
                        HuSprPosSet(grpId[i], 1, -10, 0);
                        HuSprPosSet(grpId[i], 2, 10, 0);
                    } else {
                        HuSprPosSet(grpId[i], 2, 0, 0);
                    }
                    if(score->info.rank[i] == 1) {
                        ResCardAudExec(7);
                    } else {
                        ResCardAudExec(6);
                    }
                    player->state = 7;
                }
                break;

            case 8:
                player->state = 9;
                break;

            case 9:
                MdSpriteDispSet(sprite, i+2, FALSE);
                player->state = 10;
                break;

            case 10:
                break;
        }
    }
}

/* start the card draw: cursor on the first top-rank player, one zero card. */
void ResCardDrawStarCreate(void)
{
    RESCARD_DRAW *draw = &sys->score->draw;
    int i;
    int k;

    draw->phase = 0;
    draw->timeLimit = 300;
    ResCardDrawCursorReset(0);
    draw->cursor = 0;
    draw->revealIdx = 0;
    draw->revealWait = 0;
    for(i=0; i<8; i++) {
        draw->card[i].kind = 1;
        draw->card[i].state = 0;
    }
    draw->card[frandmod(8)].kind = 0;
    k = 0;
    for(i=0; i<8; i++) {
        if(draw->card[i].kind == 0) {
            draw->card[i].id = 0x42;
        } else if(draw->card[i].kind == 1) {
            draw->card[i].id = k+0x43;
            k++;
        }
    }
}

/* card draw state machine: deal the cards, run the cursor, turn the picked
 * card over. */
void ResCardDrawUpdate(void)
{
    MDMESS_WORK *mess = sys->mess;
    MDMODEL_WORK *model = sys->model;
    MDSPRITE_WORK *sprite = sys->sprite;
    RESCARD_SCORE *score = sys->score;
    RESCARD_DRAW *draw = &score->draw;
    BOOL timerDone;
    s32 dir;
    int i;

    switch(draw->phase) {
        case 1:
            for(i=0; i<8; i++) {
                draw->card[i].state = 1;
            }
            draw->remain = 8;
            draw->revealIdx = 0;
            draw->revealWait = 0;
            draw->phase = 2;
            break;

        case 2:
            if(draw->revealIdx < 8) {
                draw->revealWait--;
                if(draw->revealWait < 0) {
                    draw->card[draw->revealIdx].state = 3;
                    draw->revealWait = 8;
                    draw->revealIdx++;
                    ResCardAudExec(11);
                }
            } else if(draw->card[7].state == 4) {
                draw->phase = 3;
            }
            break;

        case 3:
            MdMessWinHomeClear(mess, 1);
            MdMessWinMesSet(mess, 1, 0x150016);
            draw->phase = 4;
            break;

        case 4:
            if(MdMessWinCheck(mess, 1)) {
                MdMessHelpWinCreate(mess, 0, 0x20012, 144, 340, 0, 32, 0, 0);
                draw->phase = 5;
            }
            break;

        case 5:
            ResCardDrawCursorModeSet(0);
            ResCardDrawTimerPosSet();
            MgTimerParamSet(sys->timer, 300, 0, 0);
            MgTimerDispOn(sys->timer);
            MdMessWinHomeClear(mess, 1);
            MdMessWinInsertMesSet(mess, 1, 0x150017, score->player[draw->sel].charNo);
            draw->phase = 6;
            break;

        case 6:
            if(ResCardDrawCursorModeSet(1) == 1) {
                MgTimerModeOnSet(sys->timer, 1);
                draw->wait = 0;
                draw->phase = 7;
            }
            break;

        case 7:
            timerDone = MgTimerDoneCheck(sys->timer);
            dir = ResCardDrawStkDirGet();
            if(dir >= 0 && dir <= 3) {
                ResCardDrawCursorMove(dir);
                ResCardAudExec(8);
                ResCardDrawCursorModeSet(3);
            } else if(timerDone == 1) {
                ResCardAudExec(10);
                ResCardDrawCursorModeSet(4);
                draw->wait = 0;
                draw->phase = 8;
            } else if(dir == 4) {
                draw->remain--;
                ResCardDrawCursorModeSet(4);
                MgTimerModeOffSet(sys->timer);
                if(draw->card[draw->cursor].kind == 1) {
                    draw->card[draw->cursor].state = 5;
                    ResCardAudExec(9);
                    draw->phase = 9;
                } else if(draw->card[draw->cursor].kind == 0) {
                    draw->card[draw->cursor].state = 10;
                    ResCardAudExec(9);
                    draw->phase = 10;
                }
            }
            break;

        case 8:
            if(draw->wait == 0) {
                MgTimerDispOff(sys->timer);
                draw->wait = 1;
            }
            if(ResCardDrawCursorModeSet(5) == 1) {
                ResCardDrawCursorReset(1);
                draw->wait = 0;
                draw->phase = 11;
            }
            break;

        case 11:
            if(++draw->wait >= 60) {
                draw->phase = 5;
            }
            break;

        case 9:
            if(draw->card[draw->cursor].state == 9) {
                ResCardDrawCursorReset(1);
                ResCardDrawCursorMove(1);
                ResCardDrawCursorModeSet(3);
                draw->phase = 5;
            }
            break;

        case 10:
            if(draw->card[draw->cursor].state == 22) {
                draw->phase = 12;
            }
            break;

        case 12:
            break;
    }
    ResCardDrawStarUpdate();
}

/* pad (or COM) input for the cursor owner: 0-3 = direction, 4 = pick, -1 = none. */
s32 ResCardDrawStkDirGet(void)
{
    RESCARD_SCORE *score = sys->score;
    s8 padNo;
    s32 dir;

    if(score->player[score->draw.sel].type == 0) {
        padNo = score->player[score->draw.sel].padNo;
        if(sys->padWork->DStkDown[padNo] & 1) {
            return 0;
        }
        if(sys->padWork->DStkDown[padNo] & 2) {
            return 1;
        }
        if(sys->padWork->DStkDown[padNo] & 8) {
            return 2;
        }
        if(sys->padWork->DStkDown[padNo] & 4) {
            return 3;
        }
        if(HuPadBtnDown[padNo] & PAD_BUTTON_A) {
            return 4;
        }
    } else if(score->player[score->draw.sel].type == 1) {
        dir = -1;
        if(sys->frame % 15 == 0) {
            if(score->draw.remain == 1) {
                score->draw.wait++;
                score->draw.wait %= 2;
            } else {
                score->draw.wait++;
                score->draw.wait %= 4;
            }
            if(score->draw.wait == 0) {
                dir = 4;
            } else {
                dir = frandmod(4);
            }
        }
        return dir;
    }
    return -1;
}

/* card slot screen positions: [slot][0] = start, [slot][1] = fly-in origin
 * (filled in at run time). */
static HuVecF lbl_1_data_374[8][2] = {
    { { 396, 84, 0 }, { 0, 0, 0 } },
    { { 288, 84, 0 }, { 0, 0, 0 } },
    { { 180, 84, 0 }, { 0, 0, 0 } },
    { { 444, 180, 0 }, { 0, 0, 0 } },
    { { 132, 180, 0 }, { 0, 0, 0 } },
    { { 396, 276, 0 }, { 0, 0, 0 } },
    { { 288, 276, 0 }, { 0, 0, 0 } },
    { { 180, 276, 0 }, { 0, 0, 0 } }
};

/* per-card state machine: deal in, flip, win/lose handling, the star trip. */
void ResCardDrawStarUpdate(void)
{
    MDCAMERA_WORK *camera = sys->camera;
    MDMODEL_WORK *model = sys->model;
    MDSPRITE_WORK *sprite = sys->sprite;
    RESCARD_SCORE *score = sys->score;
    RESCARD_DRAW *draw = &score->draw;
    MDMESS_WORK *mess = sys->mess;
    RESCARD_CARD *card = draw->card;
    HuVecF *pos;
    HuVecF *rot;
    HuVecF *scale;
    HuVecF *scrPos;
    HuVecF *wldPos;
    HuVecF *pinPos;
    int i;
    int j;

    for(i=0; i<8; i++, card++) {
        pos = model->posP[card->id];
        rot = model->rotP[card->id];
        scale = model->scaleP[card->id];
        scrPos = lbl_1_data_374[i];
        wldPos = lbl_1_bss_29C[i];
        switch(card->state) {
            case 1:
                scrPos[0].z = 3000;
                scrPos[1].x = scrPos[0].x;
                scrPos[1].y = scrPos[0].y;
                scrPos[1].z = scrPos[0].z;
                scrPos[1].x = -128;
                for(j=0; j<2; j++) {
                    Hu3D2Dto3D(&scrPos[j], camera->camera[1].bit, &wldPos[j]);
                }
                pos->x = wldPos[1].x;
                pos->y = wldPos[1].y;
                pos->z = wldPos[1].z;
                rot->x = 90;
                rot->y = 180;
                rot->z = 0;
                scale->x = 1;
                scale->y = 1;
                scale->z = 1;
                Hu3DModelAttrReset(model->mdlId[card->id], HU3D_ATTR_DISPOFF);
                card->oscT = 0;
                card->oscOut = 0;
                card->state = 2;
                break;

            case 3:
                MdMathOscillate(1, &card->oscT, &card->oscOut, 30);
                MdMathLerp(&pos->x, wldPos[1].x, wldPos[0].x, card->oscOut);
                MdMathLerp(&pos->y, wldPos[1].y, wldPos[0].y, card->oscOut);
                MdMathLerp(&pos->z, wldPos[1].z, wldPos[0].z, card->oscOut);
                if(card->oscT >= 1.0f) {
                    card->state = 4;
                }
                break;

            case 5:
            case 6:
                card->oscT = 0;
                card->oscOut = 0;
                if(card->state == 5) {
                    MgTimerDispOff(sys->timer);
                }
                if(card->state == 5) {
                    card->state = 7;
                } else if(card->state == 6) {
                    card->state = 8;
                }
                break;

            case 7:
                ResCardDrawCursorModeSet(5);
                MdMathOscillate(0, &card->oscT, &card->oscOut, 60);
                MdMathLerp(&rot->y, 180, 0, card->oscOut);
                if(card->oscT >= 1.0f) {
                    card->oscT = 0;
                    card->oscOut = 0;
                    ResCardAudExec(13);
                    card->state = 8;
                }
                break;

            case 8:
                MdMathOscillate(2, &card->oscT, &card->oscOut, 30);
                MdMathLerp(&rot->z, 0, -512, card->oscOut);
                MdMathLerpScale(scale, 1, 0, card->oscOut);
                if(card->oscT >= 1.0f) {
                    card->state = 9;
                }
                break;

            case 10:
                MgTimerDispOff(sys->timer);
                card->oscT = 0;
                card->oscOut = 0;
                lbl_1_bss_8 = 0;
                lbl_1_bss_290.x = 0;
                lbl_1_bss_290.y = 0;
                lbl_1_bss_290.z = 0;
                score->player[draw->sel].starNum++;
                card->state = 11;
                break;

            case 11:
                ResCardDrawCursorModeSet(5);
                MdMathOscillate(0, &card->oscT, &card->oscOut, 60);
                MdMathLerp(&rot->y, 180, 0, card->oscOut);
                if(card->oscT >= 1.0f) {
                    card->oscT = 0;
                    card->oscOut = 0;
                    MdMessWinClose(mess, 0);
                    card->state = 12;
                }
                break;

            case 12:
                for(j=0; j<8; j++) {
                    if(draw->card[j].state == 4) {
                        draw->card[j].state = 6;
                    }
                }
                for(j=0; j<4; j++) {
                    if(score->info.rank[j] == 1 && j != draw->sel) {
                        ResCardPinMotStart(j);
                        sys->pin[j].state = 10;
                    }
                }
                model->posP[MDMODEL_EFF_STARFLASH]->x = pos->x;
                model->posP[MDMODEL_EFF_STARFLASH]->y = pos->y;
                model->posP[MDMODEL_EFF_STARFLASH]->z = pos->z+50;
                model->rotP[MDMODEL_EFF_STARFLASH]->x = 0;
                model->rotP[MDMODEL_EFF_STARFLASH]->y = 0;
                model->rotP[MDMODEL_EFF_STARFLASH]->z = 0;
                model->scaleP[MDMODEL_EFF_STARFLASH]->x = 2;
                model->scaleP[MDMODEL_EFF_STARFLASH]->y = 2;
                model->scaleP[MDMODEL_EFF_STARFLASH]->z = 2;
                Hu3DMotionSpeedSet(model->mdlId[MDMODEL_EFF_STARFLASH], 1.0f);
                Hu3DModelAttrSet(model->mdlId[MDMODEL_EFF_STARFLASH], HU3D_MOTATTR_LOOP);
                Hu3DModelAttrReset(model->mdlId[MDMODEL_EFF_STARFLASH], HU3D_MOTATTR_PAUSE);
                Hu3DModelAttrReset(model->mdlId[MDMODEL_EFF_STARFLASH], HU3D_ATTR_DISPOFF);
                ResCardAudExec(14);
                card->state = 13;
                break;

            case 13:
                if(Hu3DMotionTimeGet(model->mdlId[MDMODEL_EFF_STARFLASH]) >= 20.0f) {
                    model->posP[MDMODEL_POP_STAR]->x = pos->x;
                    model->posP[MDMODEL_POP_STAR]->y = pos->y;
                    model->posP[MDMODEL_POP_STAR]->z = pos->z;
                    lbl_1_bss_290.x = model->posP[MDMODEL_POP_STAR]->x;
                    lbl_1_bss_290.y = model->posP[MDMODEL_POP_STAR]->y;
                    lbl_1_bss_290.z = model->posP[MDMODEL_POP_STAR]->z;
                    model->rotP[MDMODEL_POP_STAR]->x = 0;
                    model->rotP[MDMODEL_POP_STAR]->y = 0;
                    model->rotP[MDMODEL_POP_STAR]->z = 0;
                    model->scaleP[MDMODEL_POP_STAR]->x = 1.3f;
                    model->scaleP[MDMODEL_POP_STAR]->y = 1.3f;
                    model->scaleP[MDMODEL_POP_STAR]->z = 1.3f;
                    Hu3DModelAttrSet(model->mdlId[MDMODEL_POP_STAR], HU3D_MOTATTR_LOOP);
                    Hu3DModelAttrReset(model->mdlId[MDMODEL_POP_STAR], HU3D_MOTATTR_PAUSE);
                    Hu3DModelAttrReset(model->mdlId[MDMODEL_POP_STAR], HU3D_ATTR_DISPOFF);
                    ResCardAudExec(15);
                    card->state = 14;
                }
                break;

            case 14:
                if(++lbl_1_bss_8 >= 60) {
                    Hu3DModelAttrReset(model->mdlId[MDMODEL_EFF_STARFLASH], HU3D_MOTATTR_LOOP);
                    card->state = 16;
                }
                break;

            case 16:
                if(Hu3DMotionEndCheck(model->mdlId[MDMODEL_EFF_STARFLASH]) == 1) {
                    Hu3DModelAttrSet(model->mdlId[MDMODEL_EFF_STARFLASH], HU3D_ATTR_DISPOFF);
                    card->state = 15;
                }
                break;

            case 15:
                MdMathOscillate(2, &card->oscT, &card->oscOut, 30);
                MdMathLerp(&rot->z, 0, -512, card->oscOut);
                MdMathLerpScale(scale, 1, 0, card->oscOut);
                if(card->oscT >= 1.0f) {
                    MdMessWinHomeClear(mess, 1);
                    MdMessWinInsertMesSet(mess, 1, 0x150018, score->player[draw->sel].charNo);
                    ResCardAudExec(27);
                    card->oscT = 0;
                    card->oscOut = 0;
                    card->state = 23;
                }
                break;

            case 23:
                if(MdMessWinCheck(mess, 1)) {
                    card->state = 17;
                }
                break;

            case 17:
                MdMathOscillate(2, &card->oscT, &card->oscOut, 30);
                MdMathLerp(&model->posP[MDMODEL_POP_STAR]->y, lbl_1_bss_290.y, 1000, card->oscOut);
                MdMathLerp(&model->rotP[MDMODEL_POP_STAR]->y, 0, 360, card->oscOut);
                if(card->oscT >= 1.0f) {
                    card->oscT = 0;
                    card->oscOut = 0;
                    Hu3DModelLayerSet(model->mdlId[MDMODEL_POP_STAR], 2);
                    Hu3DModelCameraSet(model->mdlId[MDMODEL_POP_STAR], camera->camera[0].bit);
                    pinPos = model->posP[MDMODEL_PIN+draw->sel];
                    lbl_1_bss_290.x = pinPos->x;
                    lbl_1_bss_290.y = pinPos->y;
                    lbl_1_bss_290.z = pinPos->z;
                    model->posP[MDMODEL_POP_STAR]->x = pinPos->x;
                    model->posP[MDMODEL_POP_STAR]->y = 1000;
                    model->posP[MDMODEL_POP_STAR]->z = pinPos->z;
                    model->rotP[MDMODEL_POP_STAR]->y = 0;
                    model->scaleP[MDMODEL_POP_STAR]->x = 1;
                    model->scaleP[MDMODEL_POP_STAR]->y = 1;
                    model->scaleP[MDMODEL_POP_STAR]->z = 1;
                    lbl_1_bss_274[0] = ParticleGlowCreate(sprite->animP[4], 1, camera->camera[0].bit);
                    card->state = 18;
                }
                break;

            case 18:
                MdMathOscillate(1, &card->oscT, &card->oscOut, 30);
                MdMathLerp(&model->posP[MDMODEL_POP_STAR]->y, 1000, 300+lbl_1_bss_290.y, card->oscOut);
                if(card->oscT >= 1.0f) {
                    card->oscT = 0;
                    card->oscOut = 0;
                    card->state = 19;
                }
                break;

            case 19:
                MdMathOscillate(2, &card->oscT, &card->oscOut, 120);
                MdMathLerp(&model->rotP[MDMODEL_POP_STAR]->y, 0, 1080, card->oscOut);
                ParticleGlowPosSet(lbl_1_bss_274[0], model->posP[MDMODEL_POP_STAR]);
                if(card->oscT >= 1.0f) {
                    card->oscT = 0;
                    card->oscOut = 0;
                    ResCardAudExec(1);
                    card->state = 20;
                }
                break;

            case 20:
                MdMathOscillate(2, &card->oscT, &card->oscOut, 30);
                MdMathLerp(&model->posP[MDMODEL_POP_STAR]->y, 300+lbl_1_bss_290.y, 150+lbl_1_bss_290.y, card->oscOut);
                MdMathLerpScale(model->scaleP[MDMODEL_POP_STAR], 1, 0, card->oscOut);
                ParticleGlowPosSet(lbl_1_bss_274[0], model->posP[MDMODEL_POP_STAR]);
                if(card->oscT >= 1.0f) {
                    ResCardAudExec(16);
                    card->oscT = 0;
                    card->oscOut = 0;
                    card->state = 22;
                }
                break;

            case 21:
                if(MdMessWinCheck(mess, 1)) {
                    card->state = 22;
                }
                break;
        }
    }
}

/* cursor model of the card owner: 0 = reset, 1 = grow in (returns 1 when
 * done), 3 = follow the card, 4 = reset timers, 5 = shrink out (returns 1
 * when done). */
s32 ResCardDrawCursorModeSet(s32 mode)
{
    MDMODEL_WORK *model = sys->model;
    MDSPRITE_WORK *sprite = sys->sprite;
    RESCARD_SCORE *score = sys->score;
    HuVecF ofs = { -160, -80, 80 };
    HuVecF *curPos = model->posP[MDMODEL_CUR+score->draw.sel];
    HuVecF *curRot = model->rotP[MDMODEL_CUR+score->draw.sel];
    HuVecF *curScale = model->scaleP[MDMODEL_CUR+score->draw.sel];
    HuVecF *cardPos = model->posP[score->draw.card[score->draw.cursor].id];
    int i;

    switch(mode) {
        case 0:
            curPos->x = cardPos->x+ofs.x;
            curPos->y = cardPos->y+ofs.y;
            curPos->z = cardPos->z+ofs.z;
            curRot->x = 90;
            curRot->y = 0;
            curRot->z = 0;
            curScale->x = 0;
            curScale->y = 0;
            curScale->z = 0;
            for(i=0; i<4; i++) {
                if(i == score->draw.sel) {
                    Hu3DModelAttrReset(model->mdlId[MDMODEL_CUR+score->draw.sel], HU3D_ATTR_DISPOFF);
                } else {
                    Hu3DModelAttrSet(model->mdlId[MDMODEL_CUR+i], HU3D_ATTR_DISPOFF);
                }
            }
            for(i=0; i<2; i++) {
                lbl_1_bss_26C[i] = 0;
                lbl_1_bss_264[i] = 0;
            }
            break;

        case 1:
            MdMathOscillate(1, &lbl_1_bss_26C[0], &lbl_1_bss_264[0], 15);
            MdMathOscillate(3, &lbl_1_bss_26C[1], &lbl_1_bss_264[1], 15);
            MdMathLerpScaleOfs(curScale, 0, 1, lbl_1_bss_264[0], 1.5f*lbl_1_bss_264[1]);
            if(lbl_1_bss_26C[0] >= 1.0f) {
                return 1;
            }
            break;

        case 3:
            curPos->x = cardPos->x+ofs.x;
            curPos->y = cardPos->y+ofs.y;
            curPos->z = cardPos->z+ofs.z;
            break;

        case 4:
            for(i=0; i<2; i++) {
                lbl_1_bss_26C[i] = 0;
                lbl_1_bss_264[i] = 0;
            }
            break;

        case 5:
            MdMathOscillate(2, &lbl_1_bss_26C[0], &lbl_1_bss_264[0], 15);
            MdMathLerpScale(curScale, 1, 0, lbl_1_bss_264[0]);
            if(lbl_1_bss_26C[0] >= 1.0f) {
                Hu3DModelAttrSet(model->mdlId[MDMODEL_CUR+score->draw.sel], HU3D_ATTR_DISPOFF);
                return 1;
            }
            break;
    }
    return 0;
}

/* put the timer at the average screen position of the eight cards. */
void ResCardDrawTimerPosSet(void)
{
    MDMODEL_WORK *model = sys->model;
    MDSPRITE_WORK *sprite = sys->sprite;
    RESCARD_DRAW *draw = &sys->score->draw;
    HuVecF scr;
    HuVecF sum;
    int i;

    sum.x = 0;
    sum.y = 0;
    sum.z = 0;
    for(i=0; i<8; i++) {
        Hu3D3Dto2D(model->posP[draw->card[i].id], sys->camera->camera[1].bit, &scr);
        sum.x += scr.x;
        sum.y += scr.y;
    }
    if(sum.x != 0) {
        sum.x *= 0.125f;
    }
    if(sum.y != 0) {
        sum.y *= 0.125f;
    }
    MgTimerPosSet(sys->timer, sum.x, sum.y);
}

/* pick the next top-rank player after the stored selection (player 1 keeps
 * its own start point) and make it the cursor owner. */
void ResCardDrawCursorReset(s32 playerNo)
{
    RESCARD_SCORE *score = sys->score;
    s32 sel = 3;
    s32 i;

    if (playerNo == 1) {
        sel = score->draw.sel;
    }
    for (i = 0; i < 3; i++) {
        sel = (sel + 1) % 4;
        if (score->info.rank[sel] == 1) {
            score->draw.sel = sel;
            break;
        }
    }
}

/* neighbour tables for the cursor: left/right per card, 1-based. */
static s8 lbl_1_data_494[8][2] = {
    { 2, 4 }, { 3, 1 }, { 5, 2 }, { 1, 6 },
    { 8, 3 }, { 4, 7 }, { 6, 8 }, { 7, 5 }
};

/* up/down candidates per card, 1-based, in order of preference. */
static s8 lbl_1_data_4A4[8][2][8] = {
    { { 6, 4, 7, 8, 5, 2, 3, 1 }, { 4, 6, 7, 8, 5, 2, 3, 1 } },
    { { 7, 6, 8, 4, 5, 1, 3, 2 }, { 7, 8, 6, 5, 4, 3, 1, 2 } },
    { { 8, 5, 7, 6, 4, 2, 1, 3 }, { 5, 8, 7, 6, 4, 2, 1, 3 } },
    { { 1, 2, 3, 6, 7, 8, 5, 4 }, { 6, 7, 8, 1, 2, 3, 5, 4 } },
    { { 3, 2, 1, 8, 7, 6, 4, 5 }, { 8, 7, 6, 3, 2, 1, 4, 5 } },
    { { 4, 1, 2, 3, 5, 7, 8, 6 }, { 1, 4, 2, 3, 5, 7, 8, 6 } },
    { { 2, 3, 1, 5, 4, 8, 6, 7 }, { 2, 1, 3, 4, 5, 6, 8, 7 } },
    { { 5, 3, 2, 1, 4, 7, 6, 8 }, { 3, 5, 2, 1, 4, 7, 6, 8 } }
};

/* move the cursor: 0/1 = left/right, 2/3 = up/down; skips cards already taken. */
void ResCardDrawCursorMove(s32 dir)
{
    RESCARD_SCORE *score = sys->score;
    RESCARD_DRAW *draw = &score->draw;
    s32 next;
    s32 i;
    s32 j;

    switch(dir) {
        case 0:
        case 1:
            next = draw->cursor;
            for(i=0; i<8; i++) {
                next = lbl_1_data_494[next][dir]-1;
                if(draw->card[next].state != 9) {
                    draw->cursor = next;
                    return;
                }
            }
            break;

        case 2:
        case 3:
            j = 0;
            for(i=0; i<8; i++) {
                if(draw->card[lbl_1_data_4A4[draw->cursor][dir-2][j]-1].state != 9) {
                    draw->cursor = lbl_1_data_4A4[draw->cursor][dir-2][j]-1;
                    return;
                }
                j++;
            }
            break;
    }
}

/* place the four spot lights (one per pin) and aim them. */
void ResCardSpotCreate(void)
{
    MDMODEL_WORK *model = sys->model;
    RESCARD_PIN *pin = sys->pin;
    int i;

    for(i=0; i<4; i++, pin++) {
        pin->state = 0;
        pin->pos.x = -450.0f+(300.0f*i);
        pin->pos.y = 750;
        pin->pos.z = 50;
        pin->center.x = pin->pos.x;
        pin->center.y = 0;
        pin->center.z = 0;
        pin->target.x = pin->center.x;
        pin->target.y = pin->center.y;
        pin->target.z = pin->center.z;
        pin->width = 1;
        pin->oscT = 0;
        pin->oscOut = 0;
        pin->radius = 0;
        pin->radiusPhase = frandmod(360);
        pin->angle = 0;
        pin->anglePhase = frandmod(360);
    }
    ResCardSpotScaleCalc();
}

/* aim every spot model from its light position at its target point and
 * stretch it to the distance. */
static inline void SpotScaleCalcInl(void)
{
    MDMODEL_WORK *model = sys->model;
    RESCARD_PIN *pin = sys->pin;
    HuVecF *pos;
    HuVecF *rot;
    HuVecF *scale;
    HuVecF rotNew;
    HuVecF diff;
    float dist;
    int i;

    for(i=0; i<4; i++, pin++) {
        pos = model->posP[MDMODEL_SPOT+i];
        rot = model->rotP[MDMODEL_SPOT+i];
        scale = model->scaleP[MDMODEL_SPOT+i];
        pin->target.x = pin->center.x+pin->radius*cos(3.141592653589793*pin->angle/180.0);
        pin->target.z = pin->center.z+pin->radius*sin(3.141592653589793*pin->angle/180.0);
        if(pin->target.x == pin->pos.x && pin->target.z == pin->pos.z) {
            pin->target.z -= 1;
        }
        pos->x = pin->pos.x;
        pos->y = pin->pos.y;
        pos->z = pin->pos.z;
        Hu3DMtxRotGet(*MdMathMatrixDir(&pin->pos, &pin->target), &rotNew);
        if(rotNew.y == -90.0f) {
            rotNew.x *= -1.0f;
        }
        rot->x = rotNew.x;
        rot->y = rotNew.y;
        rot->z = rotNew.z;
        diff.x = pin->pos.x-pin->target.x;
        diff.y = pin->pos.y-pin->target.y;
        diff.z = pin->pos.z-pin->target.z;
        dist = sqrtf(diff.z*diff.z+(diff.x*diff.x+diff.y*diff.y));
        scale->x = pin->width;
        scale->y = pin->width;
        scale->z = dist/630.0f;
    }
}

/* out-of-line copy (SpotUpdate uses the inlined body, as the original does) */
void ResCardSpotScaleCalc(void)
{
    SpotScaleCalcInl();
}

/* spot sway: {phase speed, radius amplitude, angle phase speed, angle amplitude} */
static f32 lbl_1_data_524[4] = { 2.5f, 400.0f, 0.3f, 180.0f };

/* advance (mode 0) or just re-evaluate (mode 1) the sway of one spot. */
static inline void SpotAngleUpdateInl(s32 playerNo, s32 mode)
{
    RESCARD_PIN *pin = &sys->pin[playerNo];

    switch(mode) {
        case 0:
            pin->radiusPhase += lbl_1_data_524[0];
            MdMathAngleClamp(&pin->radiusPhase);
            pin->radius = lbl_1_data_524[1]*sin(3.141592653589793*pin->radiusPhase/180.0);
            pin->anglePhase += lbl_1_data_524[2];
            MdMathAngleClamp(&pin->anglePhase);
            pin->angle = lbl_1_data_524[3]*sin(3.141592653589793*pin->anglePhase/180.0);
            MdMathAngleClamp(&pin->angle);
            return;

        case 1:
            MdMathAngleClamp(&pin->radiusPhase);
            pin->radius = lbl_1_data_524[1]*sin(3.141592653589793*pin->radiusPhase/180.0);
            MdMathAngleClamp(&pin->anglePhase);
            pin->angle = lbl_1_data_524[3]*sin(3.141592653589793*pin->anglePhase/180.0);
            MdMathAngleClamp(&pin->angle);
            return;
    }
}

void ResCardSpotAngleUpdate(s32 playerNo, s32 mode)
{
    SpotAngleUpdateInl(playerNo, mode);
}

static s32 lbl_1_data_534 = -1;

/* spot light state machine: fade the spot in, sway it, narrow it onto the
 * winner, fade it out. */
void ResCardSpotUpdate(void)
{
    MDMODEL_WORK *model = sys->model;
    RESCARD_PIN *pin = sys->pin;
    int i;

    for(i=0; i<4; i++, pin++) {
        switch(pin->state) {
            case 1:
                Hu3DModelAttrReset(model->mdlId[MDMODEL_SPOT+i], HU3D_ATTR_DISPOFF);
                pin->oscT = 0;
                pin->oscOut = 0;
                pin->width = 0;
                pin->state = 2;
                SpotScaleCalcInl();
                break;

            case 2:
                MdMathOscillate(2, &pin->oscT, &pin->oscOut, 30);
                MdMathLerp(&pin->width, 0, 1, pin->oscOut);
                if(pin->oscT >= 1.0f) {
                    pin->state = 3;
                }
                SpotAngleUpdateInl(i, 0);
                SpotScaleCalcInl();
                break;

            case 3:
                SpotAngleUpdateInl(i, 0);
                SpotScaleCalcInl();
                break;

            case 4:
                pin->oscT = 0;
                pin->oscOut = 0;
                pin->saveRadiusPhase = pin->radiusPhase;
                pin->saveAnglePhase = pin->anglePhase;
                pin->state = 5;
                SpotAngleUpdateInl(i, 1);
                SpotScaleCalcInl();
                break;

            case 5:
                MdMathOscillate(1, &pin->oscT, &pin->oscOut, 120);
                MdMathLerp(&pin->radiusPhase, pin->saveRadiusPhase, 0, pin->oscOut);
                if(pin->oscT >= 1.0f) {
                    pin->state = 6;
                }
                SpotAngleUpdateInl(i, 1);
                SpotScaleCalcInl();
                break;

            case 7:
                pin->oscT = 0;
                pin->oscOut = 0;
                pin->saveWidth = pin->width;
                pin->state = 8;
                SpotScaleCalcInl();
                break;

            case 8:
                MdMathOscillate(2, &pin->oscT, &pin->oscOut, 30);
                MdMathLerp(&pin->width, pin->saveWidth, 0, pin->oscOut);
                if(pin->oscT >= 1.0f) {
                    Hu3DModelAttrSet(model->mdlId[MDMODEL_SPOT+i], HU3D_ATTR_DISPOFF);
                    pin->state = 9;
                }
                SpotAngleUpdateInl(i, 0);
                SpotScaleCalcInl();
                break;

            case 10:
                pin->oscT = 0;
                pin->oscOut = 0;
                pin->saveWidth = pin->width;
                pin->state = 11;
                SpotScaleCalcInl();
                break;

            case 11:
                MdMathOscillate(2, &pin->oscT, &pin->oscOut, 30);
                MdMathLerp(&pin->width, pin->saveWidth, 0, pin->oscOut);
                if(pin->oscT >= 1.0f) {
                    Hu3DModelAttrSet(model->mdlId[MDMODEL_SPOT+i], HU3D_ATTR_DISPOFF);
                    pin->state = 12;
                }
                SpotAngleUpdateInl(i, 1);
                SpotScaleCalcInl();
                break;

            case 12:
                break;
        }
    }
}

/* star position on the 2D screen (x, y, depth) */
static HuVecF lbl_1_data_56C = { 64, 308, 1280 };

/* camera-facing star animation process. */
void ResCardStarUpdate(void)
{
    MDMODEL_WORK *model = sys->model;
    MDCAMERA_WORK *camera = sys->camera;
    MDMESS_WORK *mess = sys->mess;
    RESCARD_INFO *info = &sys->score->info;
    HuVecF *pos = model->posP[MDMODEL_STAR];
    HuVecF *rot = model->rotP[MDMODEL_STAR];
    HuVecF *scale = model->scaleP[MDMODEL_STAR];

    while(1) {
        model = sys->model;
        camera = sys->camera;
        mess = sys->mess;
        info = &sys->score->info;
        pos = model->posP[MDMODEL_STAR];
        rot = model->rotP[MDMODEL_STAR];
        scale = model->scaleP[MDMODEL_STAR];
        if(info->unkE0 == 0) {
            Hu3D2Dto3D(&lbl_1_data_56C, camera->camera[1].bit, &lbl_1_bss_258);
            *pos = lbl_1_bss_258;
            ResCardBillboardUpdate(0, 1);
        }
        switch(info->phase) {
            case 0:
                break;

            case 1:
                Hu3DModelAttrSet(model->mdlId[MDMODEL_STAR], HU3D_MOTATTR_LOOP);
                Hu3DModelAttrReset(model->mdlId[MDMODEL_STAR], HU3D_MOTATTR_PAUSE);
                Hu3DMotionSet(model->mdlId[MDMODEL_STAR], model->motId[0]);
                info->phase = 0;
                break;

            case 2:
                Hu3DMotionShiftSet(model->mdlId[MDMODEL_STAR], model->motId[1], 0, 32, HU3D_MOTATTR_LOOP);
                info->phase = 0;
                break;

            case 3:
                Hu3DMotionShiftSet(model->mdlId[MDMODEL_STAR], model->motId[0], 0, 32, HU3D_MOTATTR_LOOP);
                info->phase = 0;
                break;
        }
        HuPrcVSleep();
    }
}

/* pin process: when the winner is known, circle the star/zero-star models
 * around the winning pin. */
void ResCardPinUpdate(void)
{
    MDCAMERA_WORK *camera;
    MDMODEL_WORK *model;
    MDSPRITE_WORK *sprite;
    RESCARD_SCORE *score;
    RESCARD_PLAYER *player;
    RESCARD_ORBIT *orbit;
    HuVecF *pinPos;
    HuVecF *pos;
    HuVecF *rot;
    HuVecF *scale;
    float angleStep;
    int i;

    while(1) {
        camera = sys->camera;
        model = sys->model;
        sprite = sys->sprite;
        score = sys->score;
        player = &score->player[score->draw.sel];
        switch(score->info.unkEA) {
            case 1:
                for(i=0; i<33; i++) {
                    lbl_1_bss_44[i] = NULL;
                }
                pinPos = model->posP[MDMODEL_PIN+score->draw.sel];
                lbl_1_bss_C8 = 0;
                orbit = lbl_1_bss_CC;
                for(i=0; i<33; i++, orbit++) {
                    if(i < player->starNum) {
                        lbl_1_bss_44[lbl_1_bss_C8] = ParticleGlowCreate(sprite->animP[4], 0, camera->camera[0].bit);
                        orbit->mdl = MDMODEL_WIN_STAR+i;
                        lbl_1_bss_C8++;
                    } else if(i < player->starNum+player->ztarNum) {
                        orbit->mdl = MDMODEL_WIN_ZTAR+(i-player->starNum);
                        lbl_1_bss_C8++;
                    } else {
                        orbit->mdl = -1;
                    }
                }
                orbit = lbl_1_bss_CC;
                for(i=0; i<lbl_1_bss_C8; i++, orbit++) {
                    if(orbit->mdl != -1) {
                        orbit->unk0 = 0;
                        orbit->unk1 = 0;
                        angleStep = 360.0f/lbl_1_bss_C8;
                        orbit->angle = angleStep*i;
                        MdMathAngleClamp(&orbit->angle);
                        pos = model->posP[orbit->mdl];
                        rot = model->rotP[orbit->mdl];
                        scale = model->scaleP[orbit->mdl];
                        pos->x = pinPos->x+300.0f*cos(3.141592653589793*orbit->angle/180.0);
                        pos->y = 100.0+(pinPos->y+(0.1*(100.0*sin(3.141592653589793*(4.0f*orbit->angle)/180.0))));
                        pos->z = pinPos->z+300.0f*sin(3.141592653589793*orbit->angle/180.0);
                        rot->x = 0;
                        rot->y = 0;
                        rot->z = 0;
                        orbit->scale = 0;
                        scale->x = orbit->scale;
                        scale->y = orbit->scale;
                        scale->z = orbit->scale;
                        Hu3DMotionTimeSet(model->mdlId[MDMODEL_WIN_STAR], frandmod(60));
                        Hu3DModelAttrReset(model->mdlId[orbit->mdl], HU3D_ATTR_DISPOFF);
                    }
                }
                score->info.unkEA = 2;
                break;

            case 2:
                pinPos = model->posP[MDMODEL_PIN+score->draw.sel];
                orbit = lbl_1_bss_CC;
                for(i=0; i<lbl_1_bss_C8; i++, orbit++) {
                    if(lbl_1_bss_CC[0].mdl != -1) {
                        pos = model->posP[orbit->mdl];
                        scale = model->scaleP[orbit->mdl];
                        orbit->angle -= 2;
                        pos->x = pinPos->x+300.0f*cos(3.141592653589793*orbit->angle/180.0);
                        pos->y = 100.0+(pinPos->y+(0.1*(100.0*sin(3.141592653589793*(4.0f*orbit->angle)/180.0))));
                        pos->z = pinPos->z+300.0f*sin(3.141592653589793*orbit->angle/180.0);
                        if(orbit->scale < 0.5f) {
                            orbit->scale += 0.008333334f;
                            if(orbit->scale >= 0.5f) {
                                orbit->scale = 0.5f;
                            }
                            scale->x = orbit->scale;
                            scale->y = orbit->scale;
                            scale->z = orbit->scale;
                        }
                        if(lbl_1_bss_44[i] != NULL) {
                            ParticleGlowPosSet(lbl_1_bss_44[i], pos);
                        }
                    }
                }
                break;
        }
        HuPrcVSleep();
    }
}

/* start a random pin motion for one player and play its voice. */
void ResCardPinMotStart(s32 playerNo)
{
    MDMODEL_WORK *model = sys->model;
    u32 motSel;
    u32 speedAdd;

    motSel = frandmod(2);
    speedAdd = (frandmod(49)+1)/100;
    Hu3DMotionSet(model->mdlId[MDMODEL_PIN+playerNo], model->motId[(motSel*4)+playerNo+6]);
    Hu3DModelAttrReset(model->mdlId[MDMODEL_PIN+playerNo], HU3D_MOTATTR_LOOP);
    Hu3DModelAttrReset(model->mdlId[MDMODEL_PIN+playerNo], HU3D_MOTATTR_PAUSE);
    Hu3DMotionSpeedSet(model->mdlId[MDMODEL_PIN+playerNo], 0.5f+speedAdd);
    ResCardAudExec(playerNo+17);
}

/* turn a model about Y so it faces the given camera. */
void ResCardBillboardUpdate(s32 modelNo, s32 cameraNo)
{
    MDCAMERA *camera = &sys->camera->camera[cameraNo];
    MDMODEL_WORK *model = sys->model;
    HuVecF *pos = model->posP[modelNo];
    HuVecF *rot = model->rotP[modelNo];
    HuVecF diff;

    diff.x = camera->pos.x-pos->x;
    diff.y = camera->pos.y-pos->y;
    diff.z = camera->pos.z-pos->z;
    rot->y = 180.0*(atan2(diff.x, diff.z)/3.141592653589793);
}

/* sound effect / bgm dispatcher. */
void ResCardAudExec(u32 no)
{
    MDMODEL_WORK *model = sys->model;
    MDCAMERA_WORK *camera = sys->camera;
    HuVecF in;
    HuVecF out;
    s16 pan;

    switch(no) {
        case 0:
            lbl_1_bss_40 = HuAudBGMPlay(21);
            return;

        case 1:
        case 2:
            switch(no) {
                case 1:
                    HuAudSStreamPauseFadeOut(lbl_1_bss_40, 1, 1000);
                    return;

                case 2:
                    HuAudSStreamPauseFadeOut(lbl_1_bss_40, 0, 1000);
                    return;
            }
            break;

        case 3:
            HuAudSStreamFadeOut(lbl_1_bss_40, 1000);
            return;

        case 4:
            lbl_1_bss_0 = 1;
            HuAudJinglePlay(129);
            return;

        case 5:
        case 6:
        case 7:
            switch(no) {
                case 5:
                    lbl_1_bss_C = 0;
                    HuAudFXPlay(31);
                    return;

                case 6:
                    HuAudFXPlay(33);
                    return;

                case 7:
                    if(lbl_1_bss_C == 0) {
                        lbl_1_bss_C = 1;
                        HuAudFXPlay(32);
                        return;
                    }
                    break;
            }
            break;

        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
            if(no == 8) {
                HuAudFXPlay(0);
                return;
            }
            if(no == 9) {
                HuAudFXPlay(2);
                return;
            }
            if(no == 11) {
                HuAudFXPlay(1021);
                return;
            }
            if(no == 12) {
                HuAudFXPlay(4);
                return;
            }
            if(no == 13) {
                HuAudFXPlay(1039);
                return;
            }
            if(no == 14) {
                HuAudFXPlay(1100);
                return;
            }
            if(no == 15) {
                HuAudFXPlay(1139);
                return;
            }
            if(no == 16) {
                HuAudFXPlay(851);
                return;
            }
            break;

        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
            if(no <= 20) {
                in = *model->posP[no-10];
                Hu3D3Dto2D(&in, camera->camera[0].bit, &out);
                HuAudFXPlayPan(1140, 32.0f+(0.11111111f*out.x));
                winVoiceFlag[no-17] = 1;
                return;
            }
            if(no <= 24) {
                in = *model->posP[no-10];
                Hu3D3Dto2D(&in, camera->camera[0].bit, &out);
                HuAudFXPlayPan(1141, 32.0f+(0.11111111f*out.x));
                return;
            }
            break;

        case 25:
        case 26:
        case 27:
            pan = ResCardFXPanGet(0, 1);
            switch(no) {
                case 25:
                    HuAudFXPlayPan(754, pan);
                    return;

                case 26:
                    HuAudFXPlayPan(753, pan);
                    return;

                case 27:
                    HuAudFXPlayPan(753, pan);
                    return;
            }
            break;

        case 28:
            HuAudAllStop();
            break;
    }
}

/* stereo pan (0..64) of a model's screen position. */
s16 ResCardFXPanGet(s32 modelNo, s32 cameraNo)
{
    HuVecF scr;

    Hu3D3Dto2D(sys->model->posP[modelNo], sys->camera->camera[cameraNo].bit, &scr);
    return 32.0f+(0.11111111f*scr.x);
}

/* bgm process: five seconds after the jingle, resume the stream. */
void ResCardBGMReset(void)
{
    lbl_1_bss_0 = 0;
    lbl_1_bss_10 = 0;
    while(1) {
        switch(lbl_1_bss_0) {
            case 0:
                break;

            case 1:
                if(++lbl_1_bss_10 >= 300) {
                    ResCardAudExec(2);
                    lbl_1_bss_0++;
                }
                break;
        }
        HuPrcVSleep();
    }
}

/* delayed win voices: forty frames after a pin sound, play that player's voice. */
void ResCardWinVoicePlay(void)
{
    int i;

    for(i=0; i<4; i++) {
        winVoiceFlag[i] = 0;
        lbl_1_bss_34[i] = 0;
    }
    while(1) {
        for(i=0; i<4; i++) {
            switch(winVoiceFlag[i]) {
                case 0:
                    break;

                case 1:
                    if(++lbl_1_bss_34[i] >= 40) {
                        winVoiceFlag[i] = 0;
                        ResCardAudExec(i+21);
                    }
                    break;
            }
        }
        HuPrcVSleep();
    }
}

/* model position/rotation/scale debug editor (active in debug font mode 3). */
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
    
    static MDMODEL_DEBUG_INFO mdlInfo[89] = {
        { "STAR", 0 },
        { "STAGE_BASE", 0 },
        { "STAGE_SIDE", 0 },
        { "STAGE_OBJ1", 0 },
        { "STAGE_OBJ2", 0 },
        { "STAGE_OBJ3", 0 },
        { "STAGE_OBJ4", 0 },
        { "PIN_1P", 0 },
        { "PIN_2P", 0 },
        { "PIN_3P", 0 },
        { "PIN_4P", 0 },
        { "CUR_1P", 0 },
        { "CUR_2P", 0 },
        { "CUR_3P", 0 },
        { "CUR_4P", 0 },
        { "PEDESTAL_1P", 0 },
        { "PEDESTAL_2P", 0 },
        { "PEDESTAL_3P", 0 },
        { "PEDESTAL_4P", 0 },
        { "POP_STAR", 0 },
        { "POP_ZTAR", 0 },
        { "WIN_STAR_00", 0 },
        { "WIN_STAR_01", 0 },
        { "WIN_STAR_02", 0 },
        { "WIN_STAR_03", 0 },
        { "WIN_STAR_04", 0 },
        { "WIN_STAR_05", 0 },
        { "WIN_STAR_06", 0 },
        { "WIN_STAR_07", 0 },
        { "WIN_STAR_08", 0 },
        { "WIN_STAR_09", 0 },
        { "WIN_STAR_10", 0 },
        { "WIN_STAR_11", 0 },
        { "WIN_STAR_12", 0 },
        { "WIN_STAR_13", 0 },
        { "WIN_STAR_14", 0 },
        { "WIN_STAR_15", 0 },
        { "WIN_STAR_16", 0 },
        { "WIN_STAR_17", 0 },
        { "WIN_STAR_18", 0 },
        { "WIN_STAR_19", 0 },
        { "WIN_STAR_20", 0 },
        { "WIN_STAR_21", 0 },
        { "WIN_ZTAR_00", 0 },
        { "WIN_ZTAR_01", 0 },
        { "WIN_ZTAR_02", 0 },
        { "WIN_ZTAR_03", 0 },
        { "WIN_ZTAR_04", 0 },
        { "WIN_ZTAR_05", 0 },
        { "WIN_ZTAR_06", 0 },
        { "WIN_ZTAR_07", 0 },
        { "WIN_ZTAR_08", 0 },
        { "WIN_ZTAR_09", 0 },
        { "WIN_ZTAR_10", 0 },
        { "CARD_STAR_1P", 0 },
        { "CARD_STAR_2P", 0 },
        { "CARD_STAR_3P", 0 },
        { "CARD_STAR_4P", 0 },
        { "CARD_ZTAR_1P", 0 },
        { "CARD_ZTAR_2P", 0 },
        { "CARD_ZTAR_3P", 0 },
        { "CARD_ZTAR_4P", 0 },
        { "CARD_SZTAR_1P", 0 },
        { "CARD_SZTAR_2P", 0 },
        { "CARD_SZTAR_3P", 0 },
        { "CARD_SZTAR_4P", 0 },
        { "DRAW_STAR", 0 },
        { "DRAW_ZTAR_0", 0 },
        { "DRAW_ZTAR_1", 0 },
        { "DRAW_ZTAR_2", 0 },
        { "DRAW_ZTAR_3", 0 },
        { "DRAW_ZTAR_4", 0 },
        { "DRAW_ZTAR_5", 0 },
        { "DRAW_ZTAR_6", 0 },
        { "EFF_STARFLASH", 0 },
        { "EFF_DICEFLASH_1P", 0 },
        { "EFF_DICEFLASH_2P", 0 },
        { "EFF_DICEFLASH_3P", 0 },
        { "EFF_DICEFLASH_4P", 0 },
        { "EFF_CARDFLASH_1P", 0 },
        { "EFF_CARDFLASH_2P", 0 },
        { "EFF_CARDFLASH_3P", 0 },
        { "EFF_CARDFLASH_4P", 0 },
        { "RING", 0 },
        { "SPOT_0", 0 },
        { "SPOT_1", 0 },
        { "SPOT_2", 0 },
        { "SPOT_3", 0 },
        { "FONTBGFILTER", 0 }
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
    grpId = sprite->grpId[1];
    if(debugFont->ctrl == 3) {
        if(showF == FALSE) {
            if(HuPadBtn[0] & PAD_BUTTON_X) {
                if(++dispTimer >= 60) {
                    MdSpriteDispSet(sprite, 1, TRUE);
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
                        MdSpriteDispSet(sprite, 1, FALSE);
                        dispTimer = 0;
                        showF = FALSE;
                        break;
                    }
                } else {
                    dispTimer = 0;
                }
                if(debugFont->ctrl == 3) {
                    if(sys->padWork->DStkDown[0] & PAD_BUTTON_LEFT) {
                        for(i=0; i<89; i++) {
                            if(--mdlSel == -1) {
                                mdlSel = 89-1;
                            }
                            attr = Hu3DModelAttrGet(model->mdlId[mdlSel]);
                            if(!(attr & HU3D_ATTR_DISPOFF)) {
                                break;
                            }
                        }
                    } else if(sys->padWork->DStkDown[0] & PAD_BUTTON_RIGHT) {
                        for(i=0; i<89; i++) {
                            mdlSel = ++mdlSel % 89;
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
                HuSprPosSet(grpId, 0, 32, 0);
                HuSprPosSet(grpId, 1, -32, 0);
                sprRot += 2;
                MdMathAngleClamp(&sprRot);
                HuSprGrpZRotSet(grpId, sprRot);
                Hu3D3Dto2D(pos, camera->camera[mdlInfoP->cameraNo].bit, &pos2D);
                HuSprGrpPosSet(grpId, pos2D.x, pos2D.y);
                if(attr & HU3D_ATTR_DISPOFF) {
                    HuSprAttrSet(grpId, 0, HUSPR_ATTR_ADDCOL);
                    HuSprAttrSet(grpId, 1, HUSPR_ATTR_ADDCOL);
                    HuSprGrpTPLvlSet(grpId, 0.4f);
                } else {
                    HuSprAttrReset(grpId, 0, HUSPR_ATTR_ADDCOL);
                    HuSprAttrReset(grpId, 1, HUSPR_ATTR_ADDCOL);
                    HuSprGrpTPLvlSet(grpId, 1.0f);
                }
                MdDebugFontPrint(debugFont, "POS-ADJUST-MODE(%2d / %2d) -> %s", mdlSel, 89, mdlInfoP->name);
                if(debugFont->ctrl == 3) {
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
