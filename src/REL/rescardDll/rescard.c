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
void ResCardBGMReset(void);
static RESCARD_EFFECT *ParticleGlowCreate(ANIMDATA *animP, s16 layer, s16 cameraBit);
static void ParticleGlowPosSet(RESCARD_EFFECT *effectP, HuVecF *pos);
void ResCardWinVoicePlay(void);

extern void MdLightSetupModel(MDLIGHT_WORK *lightWork);
extern void MdMathStdCurveInit(void);
extern void MdMathOscillate(int type, float *time, float *t, float step);
extern void MdMathLerp(float *out, float a, float b, float t);
extern void MdObjDebugFontCreate(OMOBJ *obj);
extern void MdObjPadMain(OMOBJ *obj);
extern void MdObjCameraCreate(OMOBJ *obj);
extern void MdObjLightCreate(OMOBJ *obj);
extern void MdObjMessCreate(OMOBJ *obj);
extern void MdMessKill(void);
extern void MdMessWinOpen(MDMESS_WORK *mess, int winNo, float posX, float posY, s16 w, s16 h, float bgTPLvl, s16 drawNo);
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
    sys->score->unkE0 = 0;
    sys->score->phase = 1;
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
    if(sys->score->mesId >= 0) {
        GameMesKill(sys->score->mesId);
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
            score->phase = 2;
            lbl_1_bss_4 = 0;
            sys->unk00 = 1;
            break;

        case 1:
            if(MdMessWinCheck(mess, 1)) {
                MdProcTbl[0]();
                ResCardAudExec(1);
                MdMessWinKill(mess, 1);
                score->phase = 3;
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
                if(score->rank[i] == score->rankNum) {
                    if(score->rank[i] != 1) {
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
            if(score->rankNum > 1 && score->rankMax > 1) {
                score->rankNum--;
            } else if(score->rankNum > 1 && score->rankMax == 1 && cnt == 0) {
                score->rankNum--;
            }
            if(cnt != 0) {
                sys->unk00 = 5;
            }
            break;

        case 5:
            if(score->player[lbl_1_bss_3F8].state == 7) {
                if(score->rankNum == 1) {
                    sys->unk00 = 6;
                } else {
                    sys->unk00 = 11;
                }
                if(score->rankNum > 1 && score->rankMax == 1) {
                    score->rankNum--;
                }
                if(score->rankNum == 1 && score->rankMax == 1 && sys->unk00 == 11) {
                    sys->unk00 = 12;
                }
            }
            break;

        case 6:
            if(score->rankMax > 1) {
                sys->unk00 = 0;
                sys->flow = 2;
                break;
            }
            for(i=0; i<4; i++) {
                if(score->rank[i] == 1) {
                    score->draw.sel = i;
                    break;
                }
            }
            MdMessWinOpen(mess, 1, 48, 376, 480, 64, 1.0f, 0);
            MdMessWinInsertMesSet(mess, 1, 0x150018, score->player[score->draw.sel].charNo);
            ResCardAudExec(27);
            score->phase = 2;
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
            MdMessWinInsertMesSet(mess, 1, 0x150014, score->rankMax+0x150017);
            ResCardAudExec(26);
            score->phase = 2;
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
                score->mesId = GameMesCreate(4, 3, score->player[draw->sel].charNo, -1, -1, -1);
                ResCardAudExec(4);
                score->unkEA = 1;
                sys->unk00 = 2;
            }
            break;

        case 2:
            if(GameMesStatGet(score->mesId) == 0) {
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
        score->rank[i] = 1;
        for(j=0; j<4; j++) {
            if((score->player[j].starNum-score->player[j].ztarNum) > (score->player[i].starNum-score->player[i].ztarNum)) {
                score->rank[i]++;
            }
        }
    }
    score->mesId = -1;
    score->rankMax = 0;
    score->rankNum = 1;
    for(i=0; i<4; i++) {
        if(score->rank[i] == 1) {
            score->rankMax++;
        }
        if(score->rankNum < score->rank[i]) {
            score->rankNum = score->rank[i];
        }
    }
    ResCardDrawStarCreate();
    score->unkE0 = 0;
    score->unkEA = 0;
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
                    if(score->rank[i] == 1) {
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
                    if(score->rank[j] == 1 && j != draw->sel) {
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
        if (score->rank[sel] == 1) {
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

/* kill the three particles of a "dot" effect record. */
static void ParticleDotKill(RESCARD_EFFECT *effectP)
{
    Hu3DParManKill(effectP->parManId[0]);
    Hu3DParManKill(effectP->parManId[1]);
    Hu3DParManKill(effectP->parManId[2]);
}

/* move a "glow" effect's single particle and restart its lifetime. */
static void ParticleGlowPosSet(RESCARD_EFFECT *effectP, HuVecF *pos)
{
    Hu3DParManPosSet(effectP->parManId[0], pos->x, pos->y, pos->z);
    Hu3DParManTimeLimitSet(effectP->parManId[0], 2);
    Hu3DParManAttrReset(effectP->parManId[0], HU3D_PARMAN_ATTR_TIMEUP);
}

/* kill a "glow" effect's single particle. */
static void ParticleGlowKill(RESCARD_EFFECT *effectP)
{
    Hu3DParManKill(effectP->parManId[0]);
}
