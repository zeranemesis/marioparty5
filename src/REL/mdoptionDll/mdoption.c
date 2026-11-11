#include "REL/mdoptionDll/mdoption.h"
#include "game/audio.h"
#include "game/hsfex.h"
#include "game/gamework.h"
#include "msm.h"
#include "dolphin/os.h"
#include "math.h"

/*************************************************************
 * mdoption.c
 *
 * Record/option ("mdoption") overlay: module entry points
 * (_prolog/_epilog), the shared SYS work structure, the overlay's
 * startup/shutdown halves, the flow functions of the option menu
 * (MdFlowTbl) and the camera/model transition routines (MdProcTbl).
 *
 * Functions are declared in the order the original object emits
 * them, because MWCC inlines any callee whose body it has already
 * compiled. MdMain (0x84C) calls MdInit/MdClose, so it MUST be
 * inserted between ObjectSetup and LoadProc below; putting it after
 * MdClose silently inlines MdClose into MdMain.
 *
 * The original overlay shipped no header, so the shared SYS
 * layout (mdoption.h) was recovered from the sys-> field references
 * in this module's own object.
 *************************************************************/

/* sys->loadState states. The names are this decomp's; only the
 * ordering (0=none 1=read 2=done 3=idle) is fixed by the original's
 * switch dispatch. */
enum {
    MD_LOAD_STATE_NONE,      //0
    MD_LOAD_STATE_BOARD_LOAD,//1
    MD_LOAD_STATE_WAIT,      //2
    MD_LOAD_STATE_LOAD       //3
};

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

enum {
    MDOPTION_OBJ_DEBUGFONT, //0
    MDOPTION_OBJ_PAD, //1
    MDOPTION_OBJ_CAMERA, //2
    MDOPTION_OBJ_LIGHT, //3
    MDOPTION_OBJ_SHADOW, //4
    MDOPTION_OBJ_MESS, //5
    MDOPTION_OBJ_MAX //6
};

typedef struct MdShadowWork_s {
    BOOL createF; //0x00
    int unk4;
    MDDEBUGFONT_WORK *debugFont; //0x08
    u8 pad0C[0x68 - 0xC];
} MDSHADOW_WORK;

/* Routines that live in the other units of this overlay (object.c,
 * stage.c, record.c). */
extern void MdObjDebugFontCreate(OMOBJ *obj);
extern void MdObjPadMain(OMOBJ *obj);
extern void MdObjCameraCreate(OMOBJ *obj);
extern void MdObjLightCreate(OMOBJ *obj);
extern void MdObjShadowCreate(OMOBJ *obj);
extern void MdObjMessCreate(OMOBJ *obj);
extern void MdMathStdCurveInit(void);
extern void MdMathOscillate(int type, float *time, float *t, float step);
extern void MdMathLerp(float *out, float a, float b, float t);
extern void MdMathLerpOfs(float *out, float a, float b, float t, float ofs);
extern void MdMathLerpScale(HuVecF *out, float a, float b, float t);
extern void MdMathLerpScaleOfs(HuVecF *out, float a, float b, float t, float ofs);
extern void MdMathPolarCoord(HuVecF *out, HuVecF *center, float radX, float radZ, float scale, float angle);
extern void MdSpriteCreate(void *sprite);
extern HuVecF *MdSpriteGrpPosGet(void *sprite, int grpNo);
extern HuVecF *MdSpriteGrpScaleGet(void *sprite, int grpNo);
extern void MdSpriteFadeDispSet(int dispF, int mode);
extern void MdModelCreate(void *model, void *camera);
extern void MdModelSoundTypeSet(u8 mode);
extern void MdModelRumbleSet(int no);
extern void MdModelRumbleUpdate(void);
extern void MdMessKill(void *mess);
extern void MdMessWinOpen(void *mess, int winNo, float x, float y, int w, int h, float alpha, int type);
extern void MdMessWinKill(void *mess, int winNo);
extern void MdMessWinPad1Set(void *mess, int winNo);
extern void MdMessWinMesSet(void *mess, int winNo, int mesId);
extern void MdMessWinInsertMesSet(void *mess, int winNo, int mesId, int insertMesId);
extern void MdMessWinInsertMultiMesSet(void *mess, int winNo, int mesId, int *insertMesId);
extern void MdMessWinHomeClear(void *mess, int winNo);
extern s32 MdMessWinCheck(void *mess, int winNo);
extern s16 MdMessWinStatGet(void *mess, int winNo);
extern s16 MdMessWinChoiceGet(void *mess, int winNo);
extern void ObjSelectUpdate(void *work);
extern void MdOptionCursorUpdate(void);
extern void MdOptionCursorReset(void);
extern void MdOptionCursorSet(s32 *cursor, int dir, int menuNo, int listNo);
extern void MdOptionListCursorPosSet(int menuNo, int listNo, int cursorNo);
extern void MdOptionTrigSprSet(int dir);
extern void MdOptionTrigSprUpdate(void);
extern void MdOptionStoryRankUpdate(void);
extern void MdOptionStoryRankStarUpdate(void);
extern void MdOptionWaveEffectSet(int type, int mode);
extern void MdOptionWaveEffectUpdate(void);
extern void MdOptionCapsuleOrbitHideSet(void);
extern void MdOptionCapsuleOrbitOutSet(int modelNo, int mode);
extern void MdOptionCapsuleOrbitUpdate(void);
extern void MdOptionCapsuleOrbitOutUpdate(void);
extern void MdOptionSpriteTitleUpdate(void);
extern void MdOptionModeSpriteKill(void *sprite);
extern void MdOptionMgListUpdate(void *sprite);
extern void MdOptionMgListWinUpdate(void *sprite, int menuNo, int listNo, int cursorNo);
extern void MdOptionBoardRecordUpdate(void *sprite, int boardNo);
extern void MdOptionBoardTotalRecordUpdate(void *sprite);
extern void MdOptionStoryRecordUpdate(void *sprite);
extern void MdOptionStoryTotalRecordUpdate(void *sprite, int no);
extern void MdOptionDecaRecordUpdate(void *sprite);
extern void MdOptionDecaTotalRecordUpdate(void *sprite, int no);
extern void MdOptionSdRecordUpdate(void *sprite);
extern int MdOptionDecaGetBestRecord(int charNo);
extern void fn_1_1A278(void);
extern void fn_1_1A468(int dir);
extern void fn_1_1B26C(void);
extern void fn_1_1B294(void);

void ObjectSetup(void);
void MdMain(void);
void MdInit(void);
void MdClose(void);
void fn_1_B54(void);
void fn_1_CBC(void);
void fn_1_133C(void);
void fn_1_1BD8(void);
void fn_1_2160(void);
void fn_1_2D20(void);
s32 fn_1_4768(s32 mode);
s32 fn_1_4A88(s32 mode);
s32 fn_1_5320(s32 mode);
s32 fn_1_6328(s32 mode);
s32 fn_1_9FD8(s32 mode);
void fn_1_A324(void);
void fn_1_A514(int modelNo, int cameraNo);
void fn_1_A604(void);
s32 fn_1_A7F8(void);
void fn_1_A930(s32 cmd);

/* Flow functions, indexed by sys->flowMode. */
void (*MdFlowTbl[6])(void) = {
    fn_1_B54, fn_1_CBC, fn_1_133C, fn_1_1BD8, fn_1_2160, fn_1_2D20
};

/* Camera/model transition routines (the "proc" table). */
s32 (*MdProcTbl[5])(s32) = {
    fn_1_4768, fn_1_4A88, fn_1_5320, fn_1_6328, fn_1_9FD8
};

static HuVecF lbl_1_bss_4[2];
static HuVecF lbl_1_bss_1C[20];
static HuVecF lbl_1_bss_10C[4];
SYS *sysP;
SYS *sys;
static SYS sysWork;
static s16 lbl_1_bss_654;

/* 0x0 - module entry: run the static ctors, then bring the
 * overlay's objects up. Returns 0 to the loader. */
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

/* 0x54 - module teardown: run the static dtors. */
void _epilog(void)
{
    const VoidFunc *dtors = _dtors;
    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

/* 0xA0 - build the overlay's object manager, the generic Md* work
 * objects and the sprite/model work areas, then start MdMain. */
void ObjectSetup(void)
{
    sys = &sysWork;
    sysP = sys;
    memset(sysP, 0, sizeof(SYS));
    sys->modelScale = 0.9f;
    sys->flowMode = 0;
    sys->pad50 = 0x70;
    sys->objman = omInitObjMan(512, 8192);
    omGameSysInit(sys->objman);
    sys->obj[MDOPTION_OBJ_DEBUGFONT] = omAddObjEx(sys->objman, 100, 0, 0, -1, MdObjDebugFontCreate);
    sys->obj[MDOPTION_OBJ_DEBUGFONT]->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDDEBUGFONT_WORK), HU_MEMNUM_OVL);
    if(!sys->obj[MDOPTION_OBJ_DEBUGFONT]->data) {
        OSReport("sys->obj[MDOPTION_OBJ_DEBUGFONT]->data = NULL...\n");
    }
    sys->debugFont = sys->obj[MDOPTION_OBJ_DEBUGFONT]->data;
    sys->debugFont->createF = FALSE;
    sys->obj[MDOPTION_OBJ_PAD] = omAddObjEx(sys->objman, 101, 0, 0, -1, MdObjPadMain);
    sys->obj[MDOPTION_OBJ_PAD]->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDPAD_WORK), HU_MEMNUM_OVL);
    if(!sys->obj[MDOPTION_OBJ_PAD]->data) {
        OSReport("sys->obj[MDOPTION_OBJ_PAD]->data = NULL...\n");
    }
    sys->padWork = sys->obj[MDOPTION_OBJ_PAD]->data;
    sys->obj[MDOPTION_OBJ_CAMERA] = omAddObjEx(sys->objman, 102, 0, 0, -1, MdObjCameraCreate);
    sys->obj[MDOPTION_OBJ_CAMERA]->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDCAMERA_WORK), HU_MEMNUM_OVL);
    if(!sys->obj[MDOPTION_OBJ_CAMERA]->data) {
        OSReport("sys->obj[MDOPTION_OBJ_CAMERA]->data = NULL...\n");
    }
    sys->camera = sys->obj[MDOPTION_OBJ_CAMERA]->data;
    sys->camera->createF = FALSE;
    sys->camera->debugFont = sys->debugFont;
    sys->obj[MDOPTION_OBJ_LIGHT] = omAddObjEx(sys->objman, 103, 0, 0, -1, MdObjLightCreate);
    sys->obj[MDOPTION_OBJ_LIGHT]->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDLIGHT_WORK), HU_MEMNUM_OVL);
    if(!sys->obj[MDOPTION_OBJ_LIGHT]->data) {
        OSReport("sys->obj[MDOPTION_OBJ_LIGHT]->data = NULL...\n");
    }
    sys->light = sys->obj[MDOPTION_OBJ_LIGHT]->data;
    sys->light->createF = FALSE;
    sys->light->debugFont = sys->debugFont;
    sys->obj[MDOPTION_OBJ_SHADOW] = omAddObjEx(sys->objman, 104, 0, 0, -1, MdObjShadowCreate);
    sys->obj[MDOPTION_OBJ_SHADOW]->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDSHADOW_WORK), HU_MEMNUM_OVL);
    if(!sys->obj[MDOPTION_OBJ_SHADOW]->data) {
        OSReport("sys->obj[MDOPTION_OBJ_SHADOW]->data = NULL...\n");
    }
    sys->shadow = sys->obj[MDOPTION_OBJ_SHADOW]->data;
    ((MDSHADOW_WORK *)sys->shadow)->createF = FALSE;
    ((MDSHADOW_WORK *)sys->shadow)->debugFont = sys->debugFont;
    sys->obj[MDOPTION_OBJ_MESS] = omAddObjEx(sys->objman, 105, 0, 0, -1, MdObjMessCreate);
    sys->obj[MDOPTION_OBJ_MESS]->data = HuMemDirectMallocNum(HEAP_HEAP, 0x1BC, HU_MEMNUM_OVL);
    if(!sys->obj[MDOPTION_OBJ_MESS]->data) {
        OSReport("sys->obj[MDOPTION_OBJ_MESS]->data = NULL...\n");
    }
    sys->mess = sys->obj[MDOPTION_OBJ_MESS]->data;
    *(BOOL *)sys->mess = FALSE;
    sys->sprite = HuMemDirectMallocNum(HEAP_HEAP, 0x2A4, HU_MEMNUM_OVL);
    if(!sys->sprite) {
        OSReport("sys->sprite = NULL...\n");
    }
    memset(sys->sprite, 0, 0x2A4);
    sys->model = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDOPT_MODEL_WORK), HU_MEMNUM_OVL);
    if(!sys->model) {
        OSReport("sys->model = NULL...\n");
    }
    memset(sys->model, 0, sizeof(MDOPT_MODEL_WORK));
    MdMathStdCurveInit();
    Hu3DBGColorSet(0, 0, 0);
    while(1) {
        if(sys->debugFont->createF == TRUE && sys->camera->createF == TRUE && sys->light->createF == TRUE
            && ((MDSHADOW_WORK *)sys->shadow)->createF == TRUE && *(BOOL *)sys->mess == TRUE) {
            break;
        }
        HuPrcVSleep();
    }
    GwPlayerConf[0].padNo = 0;
    GwPlayerConf[0].type = 0;
    sys->mainProc = HuPrcChildCreate(MdMain, 107, 12288, 0, sys->objman);
}

/* 0x84C - overlay main loop: run the current flow function and the
 * per-frame effect updates until the overlay is asked to exit. */
void MdMain(void)
{
    u8 selWork[16];

    MdInit();
    while(!omSysExitReq && !sys->exitF) {
        MdFlowTbl[sys->flowMode]();
        ObjSelectUpdate(selWork);
        MdOptionCursorUpdate();
        MdOptionTrigSprUpdate();
        MdOptionStoryRankUpdate();
        MdOptionWaveEffectUpdate();
        MdOptionCapsuleOrbitUpdate();
        MdOptionCapsuleOrbitOutUpdate();
        MdModelRumbleUpdate();
        MdOptionStoryRankStarUpdate();
        MdOptionSpriteTitleUpdate();
        HuPrcVSleep();
    }
    MdClose();
}

/* 0x900 - background loader thread. Nothing starts it in this
 * overlay any more (MdInit spawns fn_1_A324 instead), so it is
 * kept only because the original still ships the body. */
static void LoadProc(void)
{
    OSTick tick;

    while(1) {
        switch(sys->loadState) {
            case MD_LOAD_STATE_NONE:
                break;

            case MD_LOAD_STATE_BOARD_LOAD:
                tick = OSGetTick();
                sys->loadAsyncStat = HuDataDirReadAsync(DATA_rescard);
                while(!HuDataGetAsyncStat(sys->loadAsyncStat)) {
                    HuPrcVSleep();
                }
                OSReport("Load Time:%d\n", OSTicksToMilliseconds(OSGetTick()-tick));
                OSReport("Finished!\n");
                sys->loadState = MD_LOAD_STATE_WAIT;
                break;

            case MD_LOAD_STATE_WAIT:
                HuPrcEnd();
                break;
        }
        HuPrcVSleep();
    }
}

/* 0x9F4 - overlay startup: build the sprite and model work areas,
 * release the overlay's own data directory and spawn the record
 * loader thread. */
void MdInit(void)
{
    MdSpriteCreate(sys->sprite);
    HuPrcVSleep();
    MdModelCreate(sys->model, sys->camera);
    HuPrcVSleep();
    HuDataDirClose(DATA_mdoption);
    HuPrcVSleep();
    sys->unk398 = 0;
    sys->unk39C = 0;
    sys->loadProc = HuPrcChildCreate(fn_1_A324, 0x6d, 0x1000, 0, sys->objman);
    HuPrcVSleep();
}

/* 0xAC0 - tear the option overlay down: wipe out unless the
 * caller already wiped, silence the audio hook, drop the message
 * windows and hand control back to the loader. */
void MdClose(void)
{
    if(!sys->skipWipeF) {
        WipeColorSet(255, 255, 255);
        WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 60);
        WipeWait();
    }
    fn_1_A930(0xb);
    MdMessKill(sys->mess);
    omOvlReturnEx(1, 1);
    HuPrcEnd();
    while(1) {
        HuPrcVSleep();
    }
}

/* 0xB54 - flow 0: hide the stage models, reset the camera proc,
 * start the BGM and fade the screen in. */
void fn_1_B54(void)
{
    MDSPRITE_WORK *sprite = sys->sprite;
    MDOPT_MODEL_WORK *model = sys->model;
    MDMESS_WORK *mess = sys->mess;
    int i;
    s16 *flag;

    for(i = 0; i < 4; i++) {
        sys->unk47[i] = 0;
    }
    Hu3DModelAttrReset(model->mdlId[0], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrReset(model->mdlId[1], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(model->mdlId[131], HU3D_ATTR_DISPOFF);
    MdProcTbl[0](0);
    flag = &sys->unkA4;
    *flag = 0;
    WipeColorSet(255, 255, 255);
    WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, 30);
    fn_1_A930(0);
    WipeWait();
    sys->step = 0;
    sys->unk1C = 0;
    sys->flowMode = 1;
}

/* 0xCBC - flow 1: top menu (story / party / ... selection). */
void fn_1_CBC(void)
{
    MDOPT_MODEL_WORK *model = sys->model;
    MDMESS_WORK *mess = sys->mess;
    int mesNo[4] = { 3, 0, 1, 2 };
    int ret;
    int menuPrev;
    int menuNext;

    switch(sys->step) {
        case 0:
            MdProcTbl[0](0);
            if(OSGetSoundMode() == 0) {
                if(GwCommon.soundOutputMode != 0) {
                    GwCommon.soundOutputMode = 0;
                }
            } else if(GwCommon.soundOutputMode == 0) {
                GwCommon.soundOutputMode = 1;
            }
            msmSysSetOutputMode(GwCommon.soundOutputMode);
            sys->unk4D0 = 1;
            sys->step = 10;
            break;

        case 10:
            ret = MdProcTbl[1](0);
            if(ret == 1) {
                MdProcTbl[0](0);
                sys->step = 12;
            }
            break;

        case 12:
            MdMessWinOpen(mess, 0, 95.0f, 378.0f, 450, 64, 0.75f, 0);
            MdMessWinPad1Set(mess, 0);
            MdMessWinMesSet(mess, 0, 0x1F0000);
            HuAudFXPlay(0x2EE);
            sys->step = 1;
            break;

        case 1:
            ret = MdMessWinCheck(mess, 0);
            if(ret) {
                MdMessWinHomeClear(mess, 0);
                MdMessWinMesSet(mess, 0, 0x1F0001);
                sys->step = 2;
            }
            break;

        case 2:
            ret = MdMessWinCheck(mess, 0);
            if(ret) {
                MdMessWinHomeClear(mess, 0);
                sys->unk4D0 = 2;
                sys->menu = 1;
                sys->step = 3;
            }
            break;

        case 3:
        case 7:
            ret = MdProcTbl[2](0);
            if(ret == 1) {
                MdProcTbl[0](0);
                MdMessWinHomeClear(mess, 0);
                MdMessWinMesSet(mess, 0, mesNo[sys->menu] + 0x1F0003);
                MdMessWinMesSet(mess, 1, 0x20004);
                sys->step = 4;
            }
            break;

        case 4:
            ret = fn_1_A7F8();
            model->selState[sys->menu + 3] = 1;
            if(ret == 0 || ret == 1) {
                if(ret == 0) {
                    if(sys->menu != 0) {
                        menuPrev = sys->menu - 1;
                    } else {
                        menuPrev = 3;
                    }
                    sys->menu = menuPrev;
                } else {
                    if(sys->menu < 3) {
                        menuNext = sys->menu + 1;
                    } else {
                        menuNext = 0;
                    }
                    sys->menu = menuNext;
                }
                MdMessWinHomeClear(mess, 0);
                MdMessWinMesSet(mess, 0, mesNo[sys->menu] + 0x1F0003);
                fn_1_A930(2);
                MdProcTbl[2](0x32);
            } else if(ret == 2) {
                MdMessWinHomeClear(mess, 1);
                MdMessWinHomeClear(mess, 0);
                fn_1_A930(4);
                sys->step = 5;
            } else if(ret == 3) {
                model->selState[sys->menu + 3] = 2;
                MdMessWinHomeClear(mess, 1);
                MdMessWinHomeClear(mess, 0);
                fn_1_A930(3);
                sys->step = 0;
                sys->item = 0;
                sys->flowMode = sys->menu + 2;
            }
            break;

        case 5:
            ret = MdProcTbl[2](0x65);
            if(ret == 1) {
                MdProcTbl[0](0);
                MdMessWinHomeClear(mess, 0);
                MdMessWinMesSet(mess, 0, 0x1F0002);
                sys->step = 6;
            }
            break;

        case 6:
            ret = MdMessWinChoiceGet(mess, 0);
            if(ret == -1 || ret == 1) {
                MdMessWinHomeClear(mess, 0);
                sys->step = 7;
            } else if(ret == 0) {
                MdMessWinKill(mess, 0);
                sys->step = 100;
            }
            break;

        default:
            fn_1_A930(1);
            sys->exitF = TRUE;
            break;
    }
}

/* 0x133C - flow 2: minigame list menu. */
void fn_1_133C(void)
{
    MDOPT_MODEL_WORK *model = sys->model;
    MDMESS_WORK *mess = sys->mess;
    int ret;
    int itemPrev;
    int itemNext;
    int subPrev;
    int subNext;

    switch(sys->step) {
        case 0:
            MdProcTbl[0](0);
            sys->step = 1;
            break;

        case 1:
            MdProcTbl[2](0x65);
            ret = MdProcTbl[3](0);
            if(ret == 1) {
                MdProcTbl[0](0);
                MdMessWinHomeClear(mess, 0);
                MdMessWinMesSet(mess, 0, 0x1F0033);
                sys->step = 2;
            }
            break;

        case 2:
            if(MdMessWinStatGet(mess, 0) == 0) {
                MdMessWinHomeClear(mess, 0);
                sys->step = 3;
            }
            break;

        case 3:
        case 4:
            ret = MdProcTbl[2](1);
            if(ret == 1) {
                MdMessWinMesSet(mess, 1, 0x20004);
                MdProcTbl[0](0);
                if(sys->step == 3) {
                    MdMessWinHomeClear(mess, 0);
                    MdMessWinMesSet(mess, 0, sys->item + 0x1F0034);
                }
                sys->step = 5;
            }
            break;

        case 5:
            ret = fn_1_A7F8();
            model->selState[sys->item + 8] = 1;
            if(ret == 0 || ret == 1) {
                if(ret == 0) {
                    if(sys->item != 0) {
                        itemPrev = sys->item - 1;
                    } else {
                        itemPrev = 4;
                    }
                    sys->item = itemPrev;
                } else {
                    if(sys->item < 4) {
                        itemNext = sys->item + 1;
                    } else {
                        itemNext = 0;
                    }
                    sys->item = itemNext;
                }
                fn_1_A930(2);
                MdProcTbl[2](0x33);
                MdMessWinHomeClear(mess, 0);
                MdMessWinMesSet(mess, 0, sys->item + 0x1F0034);
            } else if(ret == 2) {
                MdMessWinHomeClear(mess, 1);
                MdMessWinHomeClear(mess, 0);
                fn_1_A930(4);
                MdProcTbl[0](0);
                sys->step = 6;
            } else if(ret == 3) {
                model->selState[sys->item + 8] = 2;
                MdMessWinHomeClear(mess, 1);
                fn_1_A930(3);
                sys->step = 7;
            }
            break;

        case 6:
            MdProcTbl[2](0x64);
            ret = MdProcTbl[3](8);
            if(ret == 1) {
                MdProcTbl[0](0);
                sys->step = 3;
                sys->flowMode = 1;
            }
            break;

        case 7:
            MdProcTbl[2](0x64);
            ret = MdProcTbl[3](sys->item + 14);
            if(ret == 1) {
                MdMessWinMesSet(mess, 1, 0x20025);
                sys->sub = sys->sel = 0;
                MdProcTbl[0](0);
                sys->step = 8;
            }
            break;

        case 8:
            ret = fn_1_A7F8();
            if(ret == 4 || ret == 5) {
                if(ret == 4) {
                    if(sys->sub != 0) {
                        subPrev = sys->sub - 1;
                    } else {
                        subPrev = 4;
                    }
                    sys->sub = subPrev;
                } else {
                    if(sys->sub < 4) {
                        subNext = sys->sub + 1;
                    } else {
                        subNext = 0;
                    }
                    sys->sub = subNext;
                }
                MdOptionTrigSprSet(ret - 4);
                sys->sel = 0;
                MdOptionCursorReset();
                MdOptionListCursorPosSet(sys->item, sys->sub, sys->sel);
                fn_1_A930(8);
                MdProcTbl[2](0x33);
                MdOptionMgListWinUpdate(sys->sprite, sys->item, sys->sub, sys->sel);
                break;
            }
            if((HuPadDStkRep[0] & 8) || (HuPadDStkRep[0] & 4)) {
                if(HuPadDStkRep[0] & 8) {
                    MdOptionCursorSet(&sys->sel, 0, sys->item, sys->sub);
                } else {
                    MdOptionCursorSet(&sys->sel, 1, sys->item, sys->sub);
                }
                break;
            }
            if(ret == 2) {
                MdMessWinHomeClear(mess, 1);
                fn_1_A930(4);
                MdProcTbl[0](0);
                sys->step = 9;
            }
            break;

        case 9:
            ret = MdProcTbl[4](0x64);
            if(ret == 0) {
                MdOptionMgListWinUpdate(sys->sprite, sys->item, sys->sub, sys->sel);
                break;
            }
            MdProcTbl[0](0);
            sys->step = 10;
            break;

        case 10:
            ret = MdProcTbl[3](4);
            if(ret == 1) {
                MdProcTbl[0](0);
                sys->step = 4;
            }
            break;
    }
}

/* 0x1BD8 - flow 3: vibration setting. */
void fn_1_1BD8(void)
{
    MDOPT_MODEL_WORK *model = sys->model;
    MDMESS_WORK *mess = sys->mess;
    int ret;

    switch(sys->step) {
        case 0:
            MdProcTbl[0](0);
            sys->step = 1;
            sys->item = GwCommon.rumbleF ^ 1;
            break;

        case 1:
            MdProcTbl[2](0x65);
            ret = MdProcTbl[3](1);
            if(ret == 1) {
                MdProcTbl[0](0);
                MdMessWinHomeClear(mess, 0);
                if(GwCommon.rumbleF != 0) {
                    MdMessWinInsertMesSet(mess, 0, 0x1F0007, 0x1F0008);
                } else {
                    MdMessWinInsertMesSet(mess, 0, 0x1F0007, 0x1F0009);
                }
                sys->step = 2;
            }
            break;

        case 2:
            if(MdMessWinStatGet(mess, 0) == 0) {
                MdMessWinHomeClear(mess, 0);
                sys->step = 3;
            }
            break;

        case 3:
            ret = MdProcTbl[2](2);
            if(ret == 1) {
                MdProcTbl[0](0);
                MdMessWinHomeClear(mess, 0);
                MdMessWinMesSet(mess, 0, sys->item + 0x1F0046);
                MdMessWinMesSet(mess, 1, 0x20004);
                sys->step = 4;
            }
            break;

        case 4:
            ret = fn_1_A7F8();
            model->selState[sys->item + 13] = 1;
            if(ret == 0 || ret == 1) {
                sys->item ^= 1;
                fn_1_A930(2);
                MdProcTbl[2](0x34);
                MdMessWinHomeClear(mess, 0);
                MdMessWinMesSet(mess, 0, sys->item + 0x1F0046);
            } else if(ret == 2) {
                MdMessWinHomeClear(mess, 1);
                MdMessWinHomeClear(mess, 0);
                fn_1_A930(4);
                MdProcTbl[0](0);
                sys->step = 5;
            } else if(ret == 3) {
                model->selState[sys->item + 13] = 2;
                MdMessWinHomeClear(mess, 1);
                MdMessWinHomeClear(mess, 0);
                GwCommon.rumbleF = sys->item ^ 1;
                fn_1_A930(3);
                sys->step = 6;
            }
            break;

        case 5:
            MdProcTbl[2](0x64);
            ret = MdProcTbl[3](9);
            if(ret == 1) {
                MdOptionCapsuleOrbitHideSet();
                MdProcTbl[0](0);
                sys->step = 3;
                sys->flowMode = 1;
            }
            break;

        case 6:
            sys->rumbleTime++;
            if(sys->rumbleTime == 20 && sys->item == 0) {
                omVibrate(0, 15, 12, 0);
            }
            MdProcTbl[2](0x64);
            ret = MdProcTbl[3](sys->item + 19);
            if(ret == 1) {
                MdMessWinHomeClear(mess, 0);
                MdMessWinMesSet(mess, 0, sys->item + 0x1F000A);
                MdProcTbl[0](0);
                sys->step = 2;
            }
            break;
    }
}

/* 0x2160 - flow 4: sound setting (stereo/mono, test of the speakers). */
void fn_1_2160(void)
{
    MDOPT_MODEL_WORK *model = sys->model;
    MDMESS_WORK *mess = sys->mess;
    int ret;
    int subPrev;
    int subNext;

    switch(sys->step) {
        case 0:
            MdProcTbl[0](0);
            MdModelSoundTypeSet(GwCommon.soundOutputMode);
            sys->step = 1;
            break;

        case 1:
            MdProcTbl[2](0x65);
            ret = MdProcTbl[3](2);
            if(ret == 1) {
                MdProcTbl[0](0);
                MdMessWinHomeClear(mess, 0);
                MdMessWinMesSet(mess, 0, 0x1F000C);
                sys->step = 2;
            }
            break;

        case 2:
            if(MdMessWinStatGet(mess, 0) == 0) {
                MdMessWinHomeClear(mess, 0);
                sys->step = 3;
            }
            break;

        case 3:
        case 4:
            ret = MdProcTbl[2](3);
            if(ret == 1) {
                MdProcTbl[0](0);
                if(sys->item == 0) {
                    MdMessWinHomeClear(mess, 0);
                    MdMessWinMesSet(mess, 0, sys->item + 0x1F000D);
                }
                MdMessWinMesSet(mess, 1, 0x20004);
                sys->step = 5;
            }
            break;

        case 5:
            ret = fn_1_A7F8();
            model->selState[sys->item + 15] = 1;
            if(ret == 0 || ret == 1) {
                sys->item ^= 1;
                fn_1_A930(2);
                MdProcTbl[2](0x35);
                MdMessWinHomeClear(mess, 0);
                MdMessWinMesSet(mess, 0, sys->item + 0x1F000D);
            } else if(ret == 2) {
                MdMessWinHomeClear(mess, 1);
                MdMessWinHomeClear(mess, 0);
                fn_1_A930(4);
                MdProcTbl[0](0);
                sys->step = 6;
            } else if(ret == 3) {
                if(sys->item == 0) {
                    MdOptionCapsuleOrbitOutSet(GwCommon.soundOutputMode + 17, GwCommon.soundOutputMode);
                    MdMessWinHomeClear(mess, 0);
                } else {
                    fn_1_1A278();
                }
                model->selState[sys->item + 15] = 2;
                MdMessWinHomeClear(mess, 1);
                fn_1_A930(3);
                sys->step = 7;
            }
            break;

        case 6:
            MdProcTbl[2](0x64);
            ret = MdProcTbl[3](10);
            if(ret == 1) {
                MdProcTbl[0](0);
                sys->step = 3;
                sys->flowMode = 1;
            }
            break;

        case 7:
            MdProcTbl[2](0x64);
            ret = MdProcTbl[3](sys->item + 21);
            if(ret == 1) {
                MdProcTbl[0](0);
                sys->sub = sys->sel = sys->sel2 = 0;
                if(sys->item == 0) {
                    sys->sub = GwCommon.soundOutputMode;
                    MdMessWinHomeClear(mess, 0);
                    MdMessWinInsertMesSet(mess, 0, 0x1F000F, sys->sub + 0x1F0010);
                    sys->step = 8;
                    break;
                }
                sys->step = 16;
            }
            break;

        case 8:
            if(MdMessWinStatGet(mess, 0) == 0) {
                MdMessWinHomeClear(mess, 0);
                sys->step = 9;
            }
            break;

        case 9:
            ret = MdProcTbl[2](5);
            if(ret == 1) {
                MdProcTbl[0](0);
                MdMessWinHomeClear(mess, 0);
                MdMessWinMesSet(mess, 0, sys->sub + 0x1F0048);
                MdMessWinMesSet(mess, 1, 0x20004);
                sys->step = 10;
            }
            break;

        case 10:
            ret = fn_1_A7F8();
            model->selState[sys->sub + 17] = 1;
            if(ret == 0 || ret == 1) {
                if(ret == 0) {
                    if(sys->sub != 0) {
                        subPrev = sys->sub - 1;
                    } else {
                        subPrev = 2;
                    }
                    sys->sub = subPrev;
                } else {
                    if(sys->sub < 2) {
                        subNext = sys->sub + 1;
                    } else {
                        subNext = 0;
                    }
                    sys->sub = subNext;
                }
                fn_1_A930(2);
                MdProcTbl[2](0x37);
                MdMessWinHomeClear(mess, 0);
                MdMessWinMesSet(mess, 0, sys->sub + 0x1F0048);
            } else if(ret == 2) {
                MdMessWinHomeClear(mess, 1);
                MdMessWinHomeClear(mess, 0);
                fn_1_A930(4);
                MdProcTbl[0](0);
                sys->step = 11;
            } else if(ret == 3) {
                model->selState[sys->sub + 17] = 2;
                MdMessWinHomeClear(mess, 1);
                MdMessWinHomeClear(mess, 0);
                fn_1_A930(3);
                sys->step = 12;
            }
            break;

        case 11:
            MdProcTbl[2](0x64);
            ret = MdProcTbl[3](12);
            if(ret == 1) {
                MdOptionCapsuleOrbitHideSet();
                MdProcTbl[0](0);
                sys->step = 4;
            }
            break;

        case 12:
            MdProcTbl[2](0x64);
            ret = MdProcTbl[3](sys->sub + 27);
            if(ret == 1) {
                MdMessWinHomeClear(mess, 0);
                MdMessWinMesSet(mess, 0, sys->sub + 0x1F0013);
                MdProcTbl[0](0);
                sys->step = 8;
            }
            break;

        case 13:
            ret = fn_1_A7F8();
            if(HuPadDStkRep[0] & 1) {
                fn_1_1A468(2);
            } else if(HuPadDStkRep[0] & 2) {
                fn_1_1A468(3);
            } else if(HuPadDStkRep[0] & 8) {
                fn_1_1A468(0);
            } else if(HuPadDStkRep[0] & 4) {
                fn_1_1A468(1);
            } else {
                switch(ret) {
                    case 4:
                        fn_1_1A468(4);
                        MdOptionTrigSprSet(0);
                        break;

                    case 5:
                        fn_1_1A468(5);
                        MdOptionTrigSprSet(1);
                        break;

                    case 2:
                        MdMessWinHomeClear(mess, 1);
                        fn_1_A930(4);
                        MdProcTbl[0](0);
                        sys->step = 14;
                        break;

                    case 3:
                        fn_1_1B294();
                        break;
                }
            }
            MdOptionMgListWinUpdate(sys->sprite, 0, 0, 0);
            break;

        case 14:
            ret = MdProcTbl[4](0x64);
            if(ret == 0) {
                MdOptionMgListWinUpdate(sys->sprite, 0, 0, 0);
                break;
            }
            fn_1_1B26C();
            MdProcTbl[0](0);
            sys->step = 15;
            break;

        case 15:
            ret = MdProcTbl[3](5);
            if(ret == 1) {
                MdProcTbl[0](0);
                sys->step = 4;
            }
            break;

        case 16:
            if(HuAudSStreamStatGet(lbl_1_bss_654) == 0) {
                MdMessWinMesSet(mess, 1, 0x20020);
                sys->step = 13;
            }
            break;
    }
}

/* 0x2D20 - flow 5: records (board / story / decathlon / battle). */
void fn_1_2D20(void)
{
    MDOPT_MODEL_WORK *model = sys->model;
    MDMESS_WORK *mess = sys->mess;
    int ret;
    int itemPrev;
    int itemNext;
    int selPrev;
    int selNext;
    int listPrev;
    int listNext;
    int listMove;

    switch(sys->step) {
        case 0:
            MdProcTbl[0](0);
            sys->step = 1;
            break;

        case 1:
            MdProcTbl[2](0x65);
            ret = MdProcTbl[3](3);
            if(ret == 1) {
                MdProcTbl[0](0);
                MdMessWinHomeClear(mess, 0);
                MdMessWinMesSet(mess, 0, 0x1F0017);
                sys->step = 2;
            }
            break;

        case 2:
            if(MdMessWinStatGet(mess, 0) == 0) {
                MdMessWinHomeClear(mess, 0);
                sys->step = 3;
            }
            break;

        case 3:
        case 4:
            ret = MdProcTbl[2](4);
            if(ret == 1) {
                MdProcTbl[0](0);
                if(sys->step == 3) {
                    MdMessWinHomeClear(mess, 0);
                    MdMessWinMesSet(mess, 0, sys->item + 0x1F0018);
                }
                MdMessWinMesSet(mess, 1, 0x20004);
                sys->step = 5;
            }
            break;

        case 5:
            ret = fn_1_A7F8();
            model->selState[sys->item + 20] = 1;
            if(ret == 0 || ret == 1) {
                if(ret == 0) {
                    if(sys->item != 0) {
                        itemPrev = sys->item - 1;
                    } else {
                        itemPrev = 3;
                    }
                    sys->item = itemPrev;
                } else {
                    if(sys->item < 3) {
                        itemNext = sys->item + 1;
                    } else {
                        itemNext = 0;
                    }
                    sys->item = itemNext;
                }
                fn_1_A930(2);
                MdProcTbl[2](0x36);
                MdMessWinHomeClear(mess, 0);
                MdMessWinMesSet(mess, 0, sys->item + 0x1F0018);
            } else if(ret == 2) {
                MdMessWinHomeClear(mess, 1);
                MdMessWinHomeClear(mess, 0);
                fn_1_A930(4);
                MdProcTbl[0](0);
                sys->step = 6;
            } else if(ret == 3) {
                model->selState[sys->item + 20] = 2;
                if(sys->item != 0) {
                    MdMessWinHomeClear(mess, 0);
                }
                MdMessWinHomeClear(mess, 1);
                fn_1_A930(3);
                sys->step = 7;
            }
            break;

        case 6:
            MdProcTbl[2](0x64);
            ret = MdProcTbl[3](11);
            if(ret == 1) {
                MdProcTbl[0](0);
                sys->step = 7;
                sys->flowMode = 1;
            }
            break;

        case 7:
            MdProcTbl[2](0x64);
            ret = MdProcTbl[3](sys->item + 23);
            if(ret == 1) {
                MdProcTbl[0](0);
                sys->sub = 0;
                if(sys->item == 0) {
                    MdMessWinMesSet(mess, 1, 0x20024);
                    sys->step = 8;
                    break;
                }
                if(sys->item == 1) {
                    sys->step = 10;
                    break;
                }
                if(sys->item == 2) {
                    sys->step = 21;
                    MdMessWinHomeClear(mess, 0);
                    MdMessWinMesSet(mess, 0, 0x1F002E);
                    break;
                }
                MdMessWinMesSet(mess, 1, 0x2000B);
                sys->step = 26;
                MdMessWinHomeClear(mess, 0);
                MdMessWinMesSet(mess, 0, 0x1F0032);
            }
            break;

        case 8:
            ret = fn_1_A7F8();
            if((HuPadDStkRep[0] & 8) || (HuPadDStkRep[0] & 4)) {
                if(HuPadDStkRep[0] & 8) {
                    MdOptionCursorSet(&sys->sub, 0, 0, 0);
                } else {
                    MdOptionCursorSet(&sys->sub, 1, 0, 0);
                }
                break;
            }
            if(ret == 2) {
                MdMessWinHomeClear(mess, 1);
                fn_1_A930(4);
                sys->step = 9;
            }
            break;

        case 9:
            ret = MdProcTbl[4](0x64);
            if(ret == 0) {
                MdOptionMgListWinUpdate(sys->sprite, sys->sub, 0, 0);
                break;
            }
            MdProcTbl[0](0);
            sys->step = 29;
            break;

        case 10:
            ret = MdProcTbl[2](6);
            if(ret == 1) {
                MdMessWinMesSet(mess, 1, 0x20004);
                MdProcTbl[0](0);
                MdMessWinHomeClear(mess, 0);
                MdMessWinMesSet(mess, 0, sys->sub + 0x1F004B);
                sys->step = 11;
            }
            break;

        case 11:
            ret = fn_1_A7F8();
            model->selState[sys->sub + 24] = 1;
            if(ret == 0 || ret == 1) {
                sys->sub ^= 1;
                MdMessWinHomeClear(mess, 0);
                MdMessWinMesSet(mess, 0, sys->sub + 0x1F004B);
                fn_1_A930(2);
                MdProcTbl[2](0x38);
            } else if(ret == 2) {
                MdMessWinHomeClear(mess, 1);
                MdMessWinHomeClear(mess, 0);
                fn_1_A930(4);
                MdProcTbl[0](0);
                sys->step = 12;
            } else if(ret == 3) {
                model->selState[sys->sub + 24] = 2;
                MdMessWinHomeClear(mess, 1);
                MdMessWinHomeClear(mess, 0);
                fn_1_A930(3);
                MdProcTbl[0](0);
                sys->step = 13;
            }
            break;

        case 12:
            MdProcTbl[2](0x64);
            ret = MdProcTbl[3](13);
            if(ret == 1) {
                MdProcTbl[0](0);
                sys->step = 3;
            }
            break;

        case 13:
            MdProcTbl[2](0x64);
            ret = MdProcTbl[3](sys->sub + 30);
            if(ret == 1) {
                MdProcTbl[0](0);
                sys->sel = sys->sel2 = 0;
                if(sys->sub == 0) {
                    sys->step = 14;
                    MdMessWinMesSet(mess, 1, 0x2001F);
                    MdMessWinHomeClear(mess, 0);
                    MdMessWinInsertMesSet(mess, 0, 0x1F0026, sys->sel + 0x1F001D);
                    break;
                }
                sys->step = 16;
                MdMessWinMesSet(mess, 0, 0x1F002A);
            }
            break;

        case 14:
            ret = fn_1_A7F8();
            if(ret == 4 || ret == 5) {
                fn_1_A930(8);
                if(ret == 4) {
                    if(sys->sel != 0) {
                        selPrev = sys->sel - 1;
                    } else {
                        selPrev = 7;
                    }
                    sys->sel = selPrev;
                } else {
                    if(sys->sel < 7) {
                        selNext = sys->sel + 1;
                    } else {
                        selNext = 0;
                    }
                    sys->sel = selNext;
                }
                MdOptionTrigSprSet(ret - 4);
                MdMessWinHomeClear(mess, 0);
                if(sys->sel < 7) {
                    if(sys->sel == 6 && !GwCommon.w07Unlock) {
                        MdMessWinInsertMesSet(mess, 0, 0x1F0026, 0x1F0051);
                    } else {
                        MdMessWinInsertMesSet(mess, 0, 0x1F0026, sys->sel + 0x1F001D);
                    }
                    MdOptionBoardRecordUpdate(sys->sprite, sys->sel);
                    break;
                }
                MdMessWinMesSet(mess, 0, 0x1F0027);
                MdOptionBoardTotalRecordUpdate(sys->sprite);
            } else if(ret == 2) {
                MdMessWinHomeClear(mess, 1);
                MdMessWinHomeClear(mess, 0);
                fn_1_A930(4);
                sys->step = 15;
            }
            break;

        case 15:
            MdProcTbl[2](0x64);
            ret = MdProcTbl[4](0x64);
            if(ret == 0) {
                if(sys->sel < 7) {
                    MdOptionBoardRecordUpdate(sys->sprite, sys->sel);
                    break;
                }
                MdOptionBoardTotalRecordUpdate(sys->sprite);
                break;
            }
            MdProcTbl[0](0);
            sys->step = 30;
            break;

        case 16:
            ret = MdProcTbl[2](11);
            if(ret == 1) {
                MdMessWinMesSet(mess, 1, 0x20004);
                MdProcTbl[0](0);
                sys->step = 17;
            }
            break;

        case 17:
            ret = fn_1_A7F8();
            if(ret == 0) {
                sys->sel -= 1;
                if(sys->sel == -1) {
                    sys->sel = 3;
                } else if(sys->sel == 3) {
                    sys->sel = 6;
                }
                MdProcTbl[2](0x3D);
                fn_1_A930(2);
            } else if(ret == 1) {
                sys->sel += 1;
                if(sys->sel == 4) {
                    sys->sel = 0;
                } else if(sys->sel == 7) {
                    sys->sel = 4;
                }
                MdProcTbl[2](0x3D);
                fn_1_A930(2);
            } else if(ret == 6) {
                sys->sel -= 4;
                if(sys->sel == -4) {
                    sys->sel = 4;
                } else if(sys->sel < 0) {
                    sys->sel += 7;
                }
                MdProcTbl[2](0x3D);
                fn_1_A930(2);
            } else if(ret == 7) {
                sys->sel += 4;
                if(sys->sel == 7) {
                    sys->sel = 6;
                } else if(sys->sel > 6) {
                    sys->sel -= 7;
                }
                MdProcTbl[2](0x3D);
                fn_1_A930(2);
            } else if(ret == 2) {
                MdMessWinHomeClear(mess, 1);
                MdMessWinHomeClear(mess, 0);
                fn_1_A930(4);
                sys->step = 18;
            } else if(ret == 3) {
                MdMessWinHomeClear(mess, 1);
                MdMessWinHomeClear(mess, 0);
                MdProcTbl[0](0);
                fn_1_A930(3);
                sys->step = 19;
            }
            break;

        case 18:
            MdProcTbl[2](0x64);
            ret = MdProcTbl[4](0x64);
            if(ret == 0) {
                MdOptionStoryRecordUpdate(sys->sprite);
                break;
            }
            MdProcTbl[0](0);
            sys->step = 30;
            break;

        case 19:
            ret = MdProcTbl[2](0x64);
            if(ret == 1) {
                MdProcTbl[0](0);
                MdMessWinMesSet(mess, 1, 0x2000B);
                MdOptionStoryTotalRecordUpdate(sys->sprite, sys->sel);
                MdMessWinInsertMesSet(mess, 0, 0x1F002B, sys->sel);
                sys->step = 20;
            }
            break;

        case 20:
            ret = fn_1_A7F8();
            if(ret == 2) {
                MdMessWinHomeClear(mess, 1);
                fn_1_A930(4);
                MdOptionStoryRecordUpdate(sys->sprite);
                sys->step = 16;
                MdMessWinHomeClear(mess, 0);
                MdMessWinMesSet(mess, 0, 0x1F002A);
            }
            break;

        case 21:
            ret = MdProcTbl[2](10);
            if(ret == 1) {
                MdMessWinMesSet(mess, 1, 0x20004);
                MdProcTbl[0](0);
                sys->step = 22;
            }
            break;

        case 22:
            ret = fn_1_A7F8();
            if(ret == 0 || ret == 1 || ret == 6 || ret == 7) {
                fn_1_A930(2);
                if(ret == 0) {
                    if(sys->sub % 5 != 0) {
                        listPrev = sys->sub - 1;
                    } else {
                        listPrev = (sys->sub / 5) * 5 + 4;
                    }
                    sys->sub = listPrev;
                } else if(ret == 1) {
                    if(sys->sub % 5 < 4) {
                        listNext = sys->sub + 1;
                    } else {
                        listNext = (sys->sub / 5) * 5;
                    }
                    sys->sub = listNext;
                } else {
                    if(sys->sub < 5) {
                        listMove = sys->sub + 5;
                    } else {
                        listMove = sys->sub - 5;
                    }
                    sys->sub = listMove;
                }
                MdProcTbl[2](0x3C);
            } else if(ret == 2) {
                MdMessWinHomeClear(mess, 1);
                MdMessWinHomeClear(mess, 0);
                fn_1_A930(4);
                sys->step = 23;
            } else if(ret == 3) {
                if(GwCommon.decaScore[sys->sub].charNo == -1) {
                    fn_1_A930(5);
                    break;
                }
                MdMessWinHomeClear(mess, 1);
                MdMessWinHomeClear(mess, 0);
                MdProcTbl[0](0);
                fn_1_A930(3);
                sys->step = 24;
            }
            break;

        case 23:
            MdProcTbl[2](0x64);
            ret = MdProcTbl[4](0x64);
            if(ret == 0) {
                MdOptionDecaRecordUpdate(sys->sprite);
                break;
            }
            MdProcTbl[0](0);
            sys->step = 28;
            break;

        case 24:
            ret = MdProcTbl[2](0x64);
            if(ret == 1) {
                int mesInsert[8] = { -1, -1, 0x1F003F, -1, -1, -1, -1, -1 };

                mesInsert[0] = MdOptionDecaGetBestRecord(sys->sub) + 0x100026;
                mesInsert[1] = GwCommon.decaScore[sys->sub].charNo;
                MdMessWinMesSet(mess, 1, 0x2000B);
                MdMessWinHomeClear(mess, 0);
                MdMessWinInsertMultiMesSet(mess, 0, 0x1F002F, mesInsert);
                MdProcTbl[0](0);
                MdOptionDecaTotalRecordUpdate(sys->sprite, sys->sub);
                sys->step = 25;
            }
            break;

        case 25:
            ret = fn_1_A7F8();
            if(ret == 2) {
                MdMessWinHomeClear(mess, 1);
                MdMessWinHomeClear(mess, 0);
                MdMessWinMesSet(mess, 0, 0x1F002E);
                MdOptionDecaRecordUpdate(sys->sprite);
                fn_1_A930(4);
                sys->step = 21;
            }
            break;

        case 26:
            ret = fn_1_A7F8();
            if(ret == 2) {
                MdMessWinHomeClear(mess, 0);
                MdMessWinHomeClear(mess, 1);
                fn_1_A930(4);
                sys->step = 27;
            }
            break;

        case 27:
            ret = MdProcTbl[4](0x64);
            if(ret == 0) {
                MdOptionSdRecordUpdate(sys->sprite);
                break;
            }
            MdProcTbl[0](0);
            sys->step = 28;
            break;

        case 28:
        case 29:
            ret = MdProcTbl[3](6);
            if(ret == 1) {
                MdProcTbl[0](0);
                if(sys->step == 29) {
                    sys->step = 4;
                    break;
                }
                sys->step = 3;
            }
            break;

        case 30:
            ret = MdProcTbl[3](7);
            if(ret == 1) {
                MdProcTbl[0](0);
                sys->step = 10;
            }
            break;
    }
}

/* 0x4768 - proc 0: reset the transition work (timers, flags). */
s32 fn_1_4768(s32 mode)
{
    sys->camInitF = 0;
    sys->unk3E8 = 0;
    sys->orbitInitF = 0;
    sys->unk41C = 0;
    sys->rumbleTime = 0;
    sys->camTime[0] = 0.0f;
    sys->camTime[1] = 0.0f;
    sys->camTime[2] = 0.0f;
    sys->camTime[3] = 0.0f;
    sys->camT[0] = 0.0f;
    sys->camT[1] = 0.0f;
    sys->camT[2] = 0.0f;
    sys->camT[3] = 0.0f;
    sys->orbitTime[0] = 0.0f;
    sys->orbitTime[1] = 0.0f;
    sys->orbitTime[2] = 0.0f;
    sys->orbitTime[3] = 0.0f;
    sys->orbitTime[4] = 0.0f;
    sys->orbitTime[5] = 0.0f;
    sys->orbitT[0] = 0.0f;
    sys->orbitT[1] = 0.0f;
    sys->orbitT[2] = 0.0f;
    sys->orbitT[3] = 0.0f;
    sys->orbitT[4] = 0.0f;
    sys->orbitT[5] = 0.0f;
    sys->sprInitF = 0;
    sys->sprTime[0] = 0.0f;
    sys->sprTime[1] = 0.0f;
    sys->sprT[0] = 0.0f;
    sys->sprT[1] = 0.0f;
    return 0;
}

/*@@M2@@*/

/* 0x9FD8 - proc 4: slide the mode title sprite in (mode 0) or out. */
s32 fn_1_9FD8(s32 mode)
{
    float t;
    MDCAMERA_WORK *camera = sys->camera;
    MDOPT_MODEL_WORK *model = sys->model;
    HuVecF *pos;
    HuVecF *scale;

    pos = MdSpriteGrpPosGet(sys->sprite, 0);
    scale = MdSpriteGrpScaleGet(sys->sprite, 0);
    if(sys->sprInitF == 0) {
        if(mode < 50) {
            fn_1_A930(6);
            scale->x = 0.0f;
            scale->y = 0.0f;
            scale->z = 0.0f;
        }
        sys->sprInitF = 1;
    }
    MdMathOscillate(MDMATH_OSC_180, &sys->sprTime[1], &sys->sprT[1], 30.0f);
    if(mode == 0) {
        MdMathOscillate(MDMATH_OSC_90, &sys->sprTime[0], &sys->sprT[0], 30.0f);
        MdMathLerp(&t, 0.0f, 1.0f, sys->sprT[0]);
        scale->x = scale->y = t;
        pos->x = 288.0f - 100.0f * sys->sprT[1];
        pos->y = 192.0f + 50.0f * sys->sprT[1];
    } else {
        MdMathOscillate(MDMATH_OSC_90_REV, &sys->sprTime[0], &sys->sprT[0], 30.0f);
        MdMathLerp(&t, 1.0f, 0.0f, sys->sprT[0]);
        scale->x = scale->y = t;
        pos->x = 288.0f + 100.0f * sys->sprT[1];
        pos->y = 190.0f - 50.0f * sys->sprT[1];
    }
    if(sys->sprTime[0] >= 1.0f) {
        if(mode != 0) {
            MdOptionModeSpriteKill(sys->sprite);
            fn_1_A930(7);
        }
        return 1;
    }
    return 0;
}

/* 0xA324 - helper process: keeps the star model parked at its 2D
 * anchor, faces it to the camera and plays the requested motion. */
void fn_1_A324(void)
{
    static HuVecF pos2D[2] = {
        { 56.0f, 400.0f, 1280.0f },
        { 64.0f, -128.0f, 1280.0f }
    };
    MDOPT_MODEL_WORK *model = sys->model;
    MDCAMERA_WORK *camera = sys->camera;
    MDMESS_WORK *mess = sys->mess;
    s32 *stat = &sys->unk398;
    HuVecF *posP = model->posP[0];
    HuVecF *rotP = model->rotP[0];
    HuVecF *scaleP = model->scaleP[0];
    s16 winStat;

    while(1) {
        model = sys->model;
        camera = sys->camera;
        mess = sys->mess;
        stat = &sys->unk398;
        posP = model->posP[0];
        rotP = model->rotP[0];
        scaleP = model->scaleP[0];
        switch(stat[0]) {
            case 0:
                break;

            case 1:
                Hu3D2Dto3D(&pos2D[0], camera->camera[1].bit, &lbl_1_bss_4[0]);
                *posP = lbl_1_bss_4[0];
                break;
        }
        fn_1_A514(0, 1);
        winStat = MdMessWinStatGet(mess, 0);
        switch(stat[1]) {
            case 0:
                break;

            case 1:
                Hu3DModelAttrSet(model->mdlId[0], HU3D_MOTATTR_LOOP);
                Hu3DModelAttrReset(model->mdlId[0], HU3D_MOTATTR_PAUSE);
                Hu3DMotionSet(model->mdlId[0], model->motId[0]);
                stat[1] = 0;
                break;

            case 2:
                Hu3DMotionShiftSet(model->mdlId[0], model->motId[1], 0.0f, 32.0f, HU3D_MOTATTR_LOOP);
                stat[1] = 0;
                break;

            case 3:
                Hu3DMotionShiftSet(model->mdlId[0], model->motId[0], 0.0f, 32.0f, HU3D_MOTATTR_LOOP);
                stat[1] = 0;
                break;
        }
        HuPrcVSleep();
    }
}

/* 0xA514 - turn a model around Y so that it faces the given camera. */
void fn_1_A514(int modelNo, int cameraNo)
{
    HuVecF dir;
    MDCAMERA_WORK *camWork = sys->camera;
    MDCAMERA *camera;
    MDOPT_MODEL_WORK *model;
    HuVecF *posP;
    HuVecF *rotP;
    float yaw;

    camera = &camWork->camera[cameraNo];
    model = sys->model;
    posP = model->posP[modelNo];
    rotP = model->rotP[modelNo];
    dir.x = camera->pos.x - posP->x;
    dir.y = camera->pos.y - posP->y;
    dir.z = camera->pos.z - posP->z;
    yaw = 180.0 * (atan2(dir.x, dir.z) / 3.141592653589793);
    rotP->y = yaw;
}

/* 0xA604 - collect the connected controllers into the cursor list. */
void fn_1_A604(void)
{
    u8 unused[4] = { 0, 0, 0, 0 };
    int i;
    u16 stat;

    sys->cursorSelNo = 0;
    for(i = 0; i < 4; i++) {
        sys->cursorSel[i] = -1;
    }
    for(i = 0; i < 4; i++) {
        stat = HuPadStatGet(i);
        if(stat == 0) {
            sys->cursorSel[sys->cursorSelNo] = i;
            sys->cursorSelF[i] = 1;
            sys->cursorSelNo++;
        } else {
            sys->cursorSelF[i] = 0;
        }
    }
    sys->cursorSelNoPrev = sys->cursorSelNo - 1;
}

/*@@NEXT@@*//*@@NEXT@@*/

/* 0xA760 - latch the current per-entry cursor selections into
 * the display copy, blanking the entries past the last one. */
void fn_1_A760(void)
{
    void *sprite;
    int i;

    sprite = sys->sprite;
    for(i = 0; i < 4; i++) {
        sys->cursorSelPrev[i] = sys->cursorSel[i];
        if(i > sys->cursorSelNoPrev) {
            sys->cursorSelPrev[i] = -1;
        }
    }
}

/* 0xA7F8 - read this frame's menu command: d-stick repeat (0..1, 6..7)
 * or the A/B/Y/X buttons (2..5); -1 when nothing was pressed. */
s32 fn_1_A7F8(void)
{
    s32 cmd = -1;

    if(sys->padWork->DStkDown[0] & 1) {
        cmd = 0;
    } else if(sys->padWork->DStkDown[0] & 2) {
        cmd = 1;
    } else if(sys->padWork->DStkDown[0] & 8) {
        cmd = 6;
    } else if(sys->padWork->DStkDown[0] & 4) {
        cmd = 7;
    } else if(HuPadBtnDown[0] & 0x200) {
        cmd = 2;
    } else if(HuPadBtnDown[0] & 0x100) {
        cmd = 3;
    } else if(HuPadBtnDown[0] & 0x40) {
        cmd = 4;
    } else if(HuPadBtnDown[0] & 0x20) {
        cmd = 5;
    }
    return cmd;
}

/* 0xA930 - menu sound effects / BGM control. */
void fn_1_A930(s32 cmd)
{
    switch(cmd) {
        case 0:
            lbl_1_bss_654 = HuAudBGMPlay(4);
            break;

        case 1:
            HuAudSStreamFadeOut(lbl_1_bss_654, 1000);
            break;

        case 2:
            HuAudFXPlay(0);
            break;

        case 3:
            HuAudFXPlay(2);
            break;

        case 4:
            HuAudFXPlay(3);
            break;

        case 5:
            HuAudFXPlay(4);
            break;

        case 6:
            HuAudFXPlay(0x5A5);
            break;

        case 7:
            HuAudFXPlay(0x5A8);
            break;

        case 8:
            HuAudFXPlay(0x5A6);
            break;

        case 9:
            HuAudFXPlay(0x5A7);
            break;

        case 10:
            HuAudFXPlay(0x5A9);
            break;

        case 11:
            HuAudAllStop();
            break;
    }
}
