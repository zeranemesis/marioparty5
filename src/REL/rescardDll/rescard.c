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
void ResCardBGMReset(void);
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

/* kill the three particles of a "dot" effect record. */
static void ParticleDotKill(RESCARD_EFFECT *effectP)
{
    Hu3DParManKill(effectP->parManId[0]);
    Hu3DParManKill(effectP->parManId[1]);
    Hu3DParManKill(effectP->parManId[2]);
}

static RESCARD_EFFECT *ParticleGlowCreate(ANIMDATA *animP, s16 layer, s16 cameraBit);

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
