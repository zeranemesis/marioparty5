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
extern void MdSpriteFadeDispSet(int dispF);
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
extern void MdOptionCapsuleOrbitOutSet(int modelNo);
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

/* MWCC emits .bss in reverse order of declaration. */
static s16 lbl_1_bss_654;
static SYS sysWork;
SYS *sys;
SYS *sysP;
static HuVecF lbl_1_bss_10C[4];
static HuVecF lbl_1_bss_1C[20];
static HuVecF lbl_1_bss_4[2];

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
    sys->sprite = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDSPRITE_WORK), HU_MEMNUM_OVL);
    if(!sys->sprite) {
        OSReport("sys->sprite = NULL...\n");
    }
    memset(sys->sprite, 0, sizeof(MDSPRITE_WORK));
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
                    MdOptionCapsuleOrbitOutSet(GwCommon.soundOutputMode + 17);
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

/* 0x4A88 - proc 1: intro. Lerps the camera and drops the star and the
 * four capsule models around it into place. */
s32 fn_1_4A88(s32 mode)
{
    HuVecF pos3D;
    HuVecF center0;
    HuVecF center1;
    HuVecF center2;
    HuVecF center3;
    HuVecF *centerP0;
    HuVecF *centerP1;
    HuVecF *centerP2;
    HuVecF *centerP3;
    float scale;
    float angle;
    MDCAMERA_WORK *camWork = sys->camera;
    MDCAMERA *camera = camWork->camera;
    MDOPT_MODEL_WORK *model = sysP->model;
    float modelScale = sys->modelScale;
    float cosV;
    float sinV;
    int i;

    if(sys->camInitF == 0) {
        lbl_1_bss_10C[0] = camera->center;
        lbl_1_bss_10C[1].x = camera->hvr.x;
        lbl_1_bss_10C[1].y = camera->hvr.y;
        lbl_1_bss_10C[1].z = 3375.0f;
        lbl_1_bss_10C[2].x = 0.0f;
        lbl_1_bss_10C[2].y = -50.0f;
        lbl_1_bss_10C[2].z = 0.0f;
        lbl_1_bss_10C[3].x = 90.0f;
        lbl_1_bss_10C[3].y = 25.0f;
        lbl_1_bss_10C[3].z = 2550.0f;
        sys->camInitF = 1;
    }
    MdMathOscillate(MDMATH_OSC_90, &sys->camTime[0], &sys->camT[0], 120.0f);
    MdMathLerp(&camera->center.x, lbl_1_bss_10C[0].x, lbl_1_bss_10C[2].x, sys->camT[0]);
    MdMathLerp(&camera->center.y, lbl_1_bss_10C[0].y, lbl_1_bss_10C[2].y, sys->camT[0]);
    MdMathLerp(&camera->center.z, lbl_1_bss_10C[0].z, lbl_1_bss_10C[2].z, sys->camT[0]);
    MdMathLerp(&camera->hvr.x, lbl_1_bss_10C[1].x, lbl_1_bss_10C[3].x, sys->camT[0]);
    MdMathLerp(&camera->hvr.y, lbl_1_bss_10C[1].y, lbl_1_bss_10C[3].y, sys->camT[0]);
    MdMathLerp(&camera->hvr.z, lbl_1_bss_10C[1].z, lbl_1_bss_10C[3].z, sys->camT[0]);
    MdMathLerp(&scale, 0.0f, 1.0f, sys->camT[0]);
    Hu3DModelScaleSet(model->mdlId[1], modelScale, modelScale, modelScale);
    Hu3DModelScaleSet(model->mdlId[2], scale * modelScale, scale * modelScale, scale * modelScale);
    MdMathLerp(&angle, 135.0f, 135.0f, sys->camT[0]);
    center0 = *model->posP[2];
    centerP0 = &center0;
    MdMathPolarCoord(&pos3D, centerP0, 700.0f, 700.0f, scale * modelScale, angle);
    Hu3DModelScaleSet(model->mdlId[3], scale, scale, scale);
    Hu3DModelPosSetV(model->mdlId[3], &pos3D);
    MdMathLerp(&angle, 110.0f, 110.0f, sys->camT[0]);
    center1 = *model->posP[2];
    centerP1 = &center1;
    MdMathPolarCoord(&pos3D, centerP1, 250.0f, 250.0f, scale * modelScale, angle);
    Hu3DModelScaleSet(model->mdlId[4], scale, scale, scale);
    Hu3DModelPosSetV(model->mdlId[4], &pos3D);
    MdMathLerp(&angle, 310.0f, 310.0f, sys->camT[0]);
    center2 = *model->posP[2];
    centerP2 = &center2;
    MdMathPolarCoord(&pos3D, centerP2, 400.0f, 400.0f, scale * modelScale, angle);
    Hu3DModelScaleSet(model->mdlId[5], scale, scale, scale);
    Hu3DModelPosSetV(model->mdlId[5], &pos3D);
    MdMathLerp(&angle, 260.0f, 260.0f, sys->camT[0]);
    center3 = *model->posP[2];
    centerP3 = &center3;
    MdMathPolarCoord(&pos3D, centerP3, 550.0f, 550.0f, scale * modelScale, angle);
    Hu3DModelScaleSet(model->mdlId[6], scale, scale, scale);
    Hu3DModelPosSetV(model->mdlId[6], &pos3D);
    MdMathOscillate(MDMATH_OSC_180, &sys->camTime[1], &sys->camT[1], 120.0f);
    cosV = cos(3.141592653589793 * (360.0f * sys->camT[0]) / 180.0);
    sinV = sin(3.141592653589793 * (360.0f * sys->camT[0]) / 180.0);
    {
        HuVecF pos2D = { 0.0f, 0.0f, 0.0f };

        pos2D.x = 288.0f - 232.0f * sys->camT[0] * cosV;
        pos2D.y = (230.0f - 100.0f * sys->camT[1]) + 170.0f * sys->camT[0] + 232.0f * sys->camT[0] * sinV;
        pos3D = pos2D;
    }
    Hu3D2Dto3D(&pos3D, camWork->camera[1].bit, &pos3D);
    Hu3DModelPosSetV(model->mdlId[0], &pos3D);
    Hu3DModelScaleSet(model->mdlId[0], sys->camT[0], sys->camT[0], sys->camT[0]);
    if(sys->camTime[0] >= 1.0f) {
        sys->unk3D8 = 1;
        sys->unk398 = 1;
        for(i = 0; i < 4; i++) {
            sys->orbitPos[i] = *model->posP[i + 3];
        }
        return 1;
    }
    return 0;
}

/* 0x5320 - proc 2: move the focus marker (and its scale) onto the
 * currently selected menu entry; modes 0..6 / 0x32..0x38 / 0xA / 0xB /
 * 0x3C / 0x3D pick the entry, 0x64/0x65 shrink it away. */
s32 fn_1_5320(s32 mode)
{
    HuVecF in;
    HuVecF out;
    MDCAMERA_WORK *camWork = sys->camera;
    MDOPT_MODEL_WORK *model = sys->model;
    HuVecF *focusP = model->posP[131];
    HuVecF *rotP = model->rotP[131];
    HuVecF *scaleP = model->scaleP[131];

    if(sys->camInitF == 0) {
        if(mode < 50) {
            if(mode == 0) {
                    *focusP = *model->posP[sys->menu + 3];
            } else if(mode == 1) {
                    *focusP = *model->posP[sys->item + 8];
            } else if(mode == 2) {
                    *focusP = *model->posP[sys->item + 13];
            } else if(mode == 3) {
                    *focusP = *model->posP[sys->item + 15];
            } else if(mode == 4) {
                    *focusP = *model->posP[sys->item + 20];
            } else if(mode == 5) {
                    *focusP = *model->posP[sys->sub + 17];
            } else if(mode == 6) {
                    *focusP = *model->posP[sys->sub + 24];
            }
            if(mode == 0) {
                if(sys->menu == 0) {
                    focusP->x += 100.0f;
                } else {
                    focusP->x -= 100.0f;
                }
                focusP->y += 100.0f;
            } else {
                focusP->x -= 50.0f;
                focusP->y += 50.0f;
            }
            rotP->x = 0.0f;
            rotP->y = 0.0f;
            rotP->z = 0.0f;
            scaleP->x = 0.0f;
            scaleP->y = 0.0f;
            scaleP->z = 0.0f;
            Hu3DModelAttrReset(model->mdlId[131], HU3D_ATTR_DISPOFF);
        }
        sys->camInitF = 1;
    }
    if(mode >= 100) {
        MdMathOscillate(MDMATH_OSC_90, &sys->camTime[0], &sys->camT[0], 15.0f);
        if(mode == 101) {
            if(sys->menu == 0) {
                MdMathLerp(&scaleP->x, -1.0f, 0.0f, sys->camT[0]);
                MdMathLerp(&scaleP->y, 1.0f, 0.0f, sys->camT[0]);
                MdMathLerp(&scaleP->z, 1.0f, 0.0f, sys->camT[0]);
            } else {
                MdMathLerpScale(scaleP, 1.0f, 0.0f, sys->camT[0]);
            }
        } else {
            MdMathLerpScale(scaleP, 0.4f, 0.0f, sys->camT[0]);
        }
    } else if(mode >= 50) {
        if(mode == 50) {
                *focusP = *model->posP[sys->menu + 3];
                if(sys->menu == 0) {
                    scaleP->x = -1.0f;
                    scaleP->y = 1.0f;
                    scaleP->z = 1.0f;
                } else {
                    scaleP->x = 1.0f;
                    scaleP->y = 1.0f;
                    scaleP->z = 1.0f;
                }
        } else if(mode == 51) {
                *focusP = *model->posP[sys->item + 8];
        } else if(mode == 52) {
                *focusP = *model->posP[sys->item + 13];
        } else if(mode == 53) {
                *focusP = *model->posP[sys->item + 15];
        } else if(mode == 54) {
                *focusP = *model->posP[sys->item + 20];
        } else if(mode == 55) {
                *focusP = *model->posP[sys->sub + 17];
        } else if(mode == 56) {
                *focusP = *model->posP[sys->sub + 24];
        } else if(mode == 60) {
                {
                    HuVecF pos2D = { 0.0f, 0.0f, 0.0f };

                    pos2D.x = 64.0f + 96.0f * (float)(sys->sub % 5);
                    pos2D.y = 128.0f + 126.0f * (float)(sys->sub / 5);
                    in = pos2D;
                }
                Hu3D2Dto3D(&in, camWork->camera[0].bit, &out);
                *focusP = out;
        } else if(mode == 61) {
                {
                    HuVecF pos2D = { 0.0f, 0.0f, 0.0f };

                    pos2D.x = 108.0f + 96.0f * (float)(sys->sel % 4) + 48.0f * (float)(sys->sel / 4);
                    pos2D.y = 144.0f + 126.0f * (float)(sys->sel / 4);
                    in = pos2D;
                }
                Hu3D2Dto3D(&in, camWork->camera[0].bit, &out);
                *focusP = out;
        }
        if(mode == 50) {
            if(sys->menu == 0) {
                focusP->x += 100.0f;
            } else {
                focusP->x -= 100.0f;
            }
            focusP->y += 100.0f;
        } else if(mode <= 56) {
            focusP->x -= 50.0f;
            focusP->y += 50.0f;
        }
    } else {
        if(mode == 10) {
            HuVecF pos2D = { 0.0f, 0.0f, 0.0f };

            pos2D.x = 64.0f + 96.0f * (float)(sys->sub % 5);
            pos2D.y = 128.0f + 126.0f * (float)(sys->sub / 5);
            in = pos2D;
            Hu3D2Dto3D(&in, camWork->camera[0].bit, &out);
            *focusP = out;
        } else if(mode == 11) {
            HuVecF pos2D = { 0.0f, 0.0f, 0.0f };

            pos2D.x = 108.0f + 96.0f * (float)(sys->sel % 4) + 48.0f * (float)(sys->sel / 4);
            pos2D.y = 144.0f + 126.0f * (float)(sys->sel / 4);
            in = pos2D;
            Hu3D2Dto3D(&in, camWork->camera[0].bit, &out);
            *focusP = out;
        }
        MdMathOscillate(MDMATH_OSC_90, &sys->camTime[0], &sys->camT[0], 15.0f);
        MdMathOscillate(MDMATH_OSC_180, &sys->camTime[1], &sys->camT[1], 15.0f);
        if(mode == 0) {
            if(sys->menu != 0) {
                MdMathLerpScaleOfs(scaleP, 0.0f, 1.0f, sys->camT[0], 2.0f * sys->camT[1]);
            } else {
                MdMathLerpOfs(&scaleP->x, 0.0f, -1.0f, sys->camT[0], 2.0f * sys->camT[1]);
                MdMathLerpOfs(&scaleP->y, 0.0f, 1.0f, sys->camT[0], 2.0f * sys->camT[1]);
                MdMathLerpOfs(&scaleP->z, 0.0f, 1.0f, sys->camT[0], 2.0f * sys->camT[1]);
            }
        } else {
            MdMathLerpScaleOfs(scaleP, 0.0f, 0.4f, sys->camT[0], 2.0f * sys->camT[1]);
        }
    }
    if(sys->camTime[0] >= 1.0f) {
        if(mode >= 100) {
            Hu3DModelAttrSet(model->mdlId[131], HU3D_ATTR_DISPOFF);
        }
        return 1;
    }
    return 0;
}

/* 0x6328 - proc 3: capsule orbit transition. Modes 0..3 / 8..11 bring
 * the main capsule in and out, 4..7 / 12..13 hold, 14..31 fly the
 * capsules of each sub menu around the focus; returns 1 when the
 * transition is finished. */
s32 fn_1_6328(s32 mode)
{
    HuVecF out;
    HuVecF dir;
    HuVecF center0;
    HuVecF center1;
    HuVecF center2;
    HuVecF center3;
    HuVecF center4;
    HuVecF center5;
    HuVecF center6;
    HuVecF center7;
    HuVecF center8;
    HuVecF center9;
    HuVecF center10;
    HuVecF center11;
    HuVecF center12;
    HuVecF center13;
    HuVecF center14;
    HuVecF center15;
    HuVecF center16;
    HuVecF center17;
    HuVecF center18;
    MDCAMERA_WORK *camera;
    HuVecF *rotP;
    HuVecF *scaleP;
    float angle;
    float scale;
    HuVecF *centerP0;
    HuVecF *centerP1;
    HuVecF *centerP2;
    HuVecF *centerP3;
    HuVecF *centerP4;
    HuVecF *centerP5;
    HuVecF *centerP6;
    HuVecF *centerP7;
    HuVecF *centerP8;
    HuVecF *centerP9;
    HuVecF *centerP10;
    HuVecF *centerP11;
    HuVecF *centerP12;
    HuVecF *centerP13;
    HuVecF *centerP14;
    HuVecF *centerP15;
    HuVecF *centerP16;
    HuVecF *centerP17;
    HuVecF *centerP18;
    int i;
    MDOPT_MODEL_WORK *model;
    HuVecF *posP;
    int idx;
    int result;
    int procRet;
    float yMul;
    float zMul;
    float zTimeMul;
    float yTimeMul;

    result = 0;
    procRet = 0;
    camera = sys->camera;
    model = sys->model;
    posP = model->posP[sys->menu + 3];
    rotP = model->rotP[sys->menu + 3];
    scaleP = model->scaleP[sys->menu + 3];
    if(sys->orbitInitF == 0) {
        switch(mode) {
            case 0:
            case 4:
                sys->unk3DC = 1;
                MdSpriteFadeDispSet(1);
                for(i = 0; i < 5; i++) {
                    Hu3DModelAttrReset(model->mdlId[i + 8], HU3D_ATTR_DISPOFF);
                    Hu3DModelPosSetV(model->mdlId[i + 8], posP);
                    Hu3DModelScaleSet(model->mdlId[i + 8], 0.5f, 0.5f, 0.5f);
                }
                break;

            case 1:
                MdOptionCapsuleOrbitOutSet(sys->item + 13);
                sys->unk3DC = 2;
                MdSpriteFadeDispSet(1);
                for(i = 0; i < 2; i++) {
                    Hu3DModelAttrReset(model->mdlId[i + 13], HU3D_ATTR_DISPOFF);
                    Hu3DModelPosSetV(model->mdlId[i + 13], posP);
                    Hu3DModelScaleSet(model->mdlId[i + 13], 0.5f, 0.5f, 0.5f);
                }
                break;

            case 2:
            case 5:
                sys->unk3DC = 3;
                MdSpriteFadeDispSet(1);
                for(i = 0; i < 2; i++) {
                    Hu3DModelAttrReset(model->mdlId[i + 15], HU3D_ATTR_DISPOFF);
                    Hu3DModelPosSetV(model->mdlId[i + 15], posP);
                    Hu3DModelScaleSet(model->mdlId[i + 15], 0.5f, 0.5f, 0.5f);
                }
                break;

            case 3:
            case 6:
                sys->unk3DC = 4;
                MdSpriteFadeDispSet(1);
                for(i = 0; i < 4; i++) {
                    Hu3DModelAttrReset(model->mdlId[i + 20], HU3D_ATTR_DISPOFF);
                    Hu3DModelPosSetV(model->mdlId[i + 20], posP);
                    Hu3DModelScaleSet(model->mdlId[i + 20], 0.5f, 0.5f, 0.5f);
                }
                break;

            case 7:
                MdSpriteFadeDispSet(1);
                for(i = 0; i < 2; i++) {
                    Hu3DModelAttrReset(model->mdlId[i + 24], HU3D_ATTR_DISPOFF);
                    Hu3DModelPosSetV(model->mdlId[i + 24], posP);
                    Hu3DModelScaleSet(model->mdlId[i + 24], 0.5f, 0.5f, 0.5f);
                }
                break;

            case 14:
            case 15:
            case 16:
            case 17:
            case 18:
                idx = mode - 14;
                lbl_1_bss_1C[0] = *model->posP[idx + 8];
                PSVECSubtract(model->posP[3], model->posP[idx + 8], &lbl_1_bss_1C[1]);
                fn_1_A930(10);
                break;

            case 19:
            case 20:
                idx = mode - 19;
                lbl_1_bss_1C[0] = *model->posP[idx + 13];
                PSVECSubtract(model->posP[4], model->posP[idx + 13], &lbl_1_bss_1C[1]);
                fn_1_A930(10);
                break;

            case 21:
            case 22:
                idx = mode - 21;
                lbl_1_bss_1C[0] = *model->posP[idx + 15];
                PSVECSubtract(model->posP[5], model->posP[idx + 15], &lbl_1_bss_1C[1]);
                fn_1_A930(10);
                break;

            case 27:
            case 28:
            case 29:
                idx = mode - 27;
                lbl_1_bss_1C[0] = *model->posP[idx + 17];
                PSVECSubtract(model->posP[5], model->posP[idx + 17], &lbl_1_bss_1C[1]);
                fn_1_A930(10);
                break;

            case 23:
            case 24:
            case 25:
            case 26:
                idx = mode - 23;
                lbl_1_bss_1C[0] = *model->posP[idx + 20];
                PSVECSubtract(model->posP[6], model->posP[idx + 20], &lbl_1_bss_1C[1]);
                fn_1_A930(10);
                break;

            case 30:
            case 31:
                idx = mode - 30;
                lbl_1_bss_1C[0] = *model->posP[idx + 24];
                PSVECSubtract(model->posP[6], model->posP[idx + 24], &lbl_1_bss_1C[1]);
                fn_1_A930(10);
                break;

            case 12:
                for(i = 0; i < 2; i++) {
                    Hu3DModelAttrReset(model->mdlId[i + 15], HU3D_ATTR_DISPOFF);
                }
                break;

            case 13:
                for(i = 0; i < 4; i++) {
                    Hu3DModelAttrReset(model->mdlId[i + 20], HU3D_ATTR_DISPOFF);
                }
                break;
        }
        sys->orbitInitF = 1;
    }
    if(mode >= 0 && mode <= 3) {
        MdMathOscillate(MDMATH_OSC_90, &sys->orbitTime[0], &sys->orbitT[0], 40.0f);
        PSVECSubtract(&Hu3DCamera->target, &Hu3DCamera->pos, &dir);
        PSVECNormalize(&dir, &dir);
        PSVECScale(&dir, &dir, 1000.0f);
        PSVECAdd(&dir, &Hu3DCamera->pos, &dir);
        dir.y += 50.0f;
        MdMathLerp(&posP->x, sys->orbitPos[mode].x, dir.x, sys->orbitT[0]);
        MdMathLerp(&posP->y, sys->orbitPos[mode].y, dir.y, sys->orbitT[0]);
        MdMathLerp(&posP->z, sys->orbitPos[mode].z, dir.z, sys->orbitT[0]);
    } else if(mode == 4 || mode == 5 || mode == 6 || mode == 7 || mode == 12 || mode == 13) {
        MdMathOscillate(MDMATH_OSC_90, &sys->orbitTime[0], &sys->orbitT[0], 40.0f);
    } else if(mode >= 8 && mode <= 11) {
        MdMathOscillate(MDMATH_OSC_90, &sys->orbitTime[0], &sys->orbitT[0], 40.0f);
        idx = mode - 8;
        PSVECSubtract(&Hu3DCamera->target, &Hu3DCamera->pos, &dir);
        PSVECNormalize(&dir, &dir);
        PSVECScale(&dir, &dir, 1000.0f);
        dir.y += 50.0f;
        PSVECAdd(&dir, &Hu3DCamera->pos, &dir);
        MdMathLerp(&posP->x, dir.x, sys->orbitPos[idx].x, sys->orbitT[0]);
        MdMathLerp(&posP->y, dir.y, sys->orbitPos[idx].y, sys->orbitT[0]);
        MdMathLerp(&posP->z, dir.z, sys->orbitPos[idx].z, sys->orbitT[0]);
    } else if(mode >= 14 && mode <= 18) {
        MdMathOscillate(MDMATH_OSC_90, &sys->orbitTime[0], &sys->orbitT[0], 40.0f);
        MdMathOscillate(MDMATH_OSC_180, &sys->orbitTime[1], &sys->orbitT[1], 40.0f);
        MdMathOscillate(MDMATH_OSC_360, &sys->orbitTime[2], &sys->orbitT[2], 40.0f);
        idx = mode - 14;
        for(i = 0; i < 5; i++) {
            if(i == idx) {
                switch(i) {
                    case 0:
                    case 4:
                        yMul = 200.0f;
                        zMul = 600.0f;
                        zTimeMul = 0.0f;
                        break;

                    case 1:
                    case 3:
                        yMul = 250.0f;
                        zMul = 450.0f;
                        zTimeMul = -400.0f;
                        break;

                    default:
                        yMul = 300.0f;
                        zMul = 400.0f;
                        zTimeMul = -500.0f;
                        break;
                }
                model->posP[i + 8]->x = lbl_1_bss_1C[0].x + lbl_1_bss_1C[1].x * sys->orbitT[1];
                model->posP[i + 8]->y = lbl_1_bss_1C[0].y + yMul * sys->orbitT[2];
                model->posP[i + 8]->z = lbl_1_bss_1C[0].z + zMul * sys->orbitT[2] + zTimeMul * sys->orbitTime[0];
                if(sys->orbitTime[0] > 0.5f && sys->waveActiveF == 0) {
                    if(i == 0 || i == 4) {
                        MdOptionWaveEffectSet(1, 0);
                    }
                    if(i == 1 || i == 3) {
                        MdOptionWaveEffectSet(1, 1);
                    } else {
                        MdOptionWaveEffectSet(1, 2);
                    }
                }
                if(sys->orbitTime[0] > 0.7f) {
                    Hu3DModelAttrSet(model->mdlId[i + 8], HU3D_ATTR_DISPOFF);
                    procRet = MdProcTbl[4](0);
                    if(sys->list == 0) {
                        MdOptionMgListUpdate(sys->sprite);
                    }
                    MdOptionMgListWinUpdate(sys->sprite, sys->item, 0, 0);
                }
            } else {
                if(i < idx) {
                    MdMathLerp(&angle, 90.0f + 45.0f * (float)i, 0.0f, sys->orbitT[0]);
                } else {
                    MdMathLerp(&angle, 90.0f + 45.0f * (float)i, 360.0f, sys->orbitT[0]);
                }
                MdMathLerp(&scale, 0.5f, 0.32f, sys->orbitTime[0]);
                Hu3DModelScaleSet(model->mdlId[i + 8], scale, scale, scale);
                MdMathLerp(&scale, 1.0f, 0.8f, sys->orbitTime[0]);
                center0.x = posP->x;
                center0.y = posP->y;
                center0.z = posP->z;
                centerP0 = &center0;
                MdMathPolarCoord(&out, centerP0, 180.0f, 180.0f, scale, angle);
                Hu3DModelPosSetV(model->mdlId[i + 8], &out);
            }
        }
    } else if(mode >= 19 && mode <= 20) {
        MdMathOscillate(MDMATH_OSC_90, &sys->orbitTime[0], &sys->orbitT[0], 40.0f);
        MdMathOscillate(MDMATH_OSC_180, &sys->orbitTime[1], &sys->orbitT[1], 40.0f);
        MdMathOscillate(MDMATH_OSC_360, &sys->orbitTime[2], &sys->orbitT[2], 40.0f);
        idx = mode - 19;
        model->posP[idx + 13]->x = lbl_1_bss_1C[0].x + lbl_1_bss_1C[1].x * sys->orbitT[1];
        model->posP[idx + 13]->y = lbl_1_bss_1C[0].y + 200.0f * sys->orbitT[2];
        model->posP[idx + 13]->z = lbl_1_bss_1C[0].z + 600.0f * sys->orbitT[2];
        if(sys->orbitTime[0] > 0.5f && sys->waveActiveF == 0) {
            MdModelRumbleSet(idx);
            MdOptionCapsuleOrbitOutSet(idx + 13);
            if(idx == 0) {
                Hu3DModelAttrReset(model->mdlId[4], HU3D_MOTATTR_PAUSE);
            } else {
                Hu3DModelAttrSet(model->mdlId[4], HU3D_MOTATTR_PAUSE);
            }
            MdOptionWaveEffectSet(2, 0);
        }
    } else if(mode >= 21 && mode <= 22) {
        MdMathOscillate(MDMATH_OSC_90, &sys->orbitTime[0], &sys->orbitT[0], 40.0f);
        MdMathOscillate(MDMATH_OSC_180, &sys->orbitTime[1], &sys->orbitT[1], 40.0f);
        MdMathOscillate(MDMATH_OSC_360, &sys->orbitTime[2], &sys->orbitT[2], 40.0f);
        idx = mode - 21;
        for(i = 0; i < 2; i++) {
            model->posP[idx + 15]->x = lbl_1_bss_1C[0].x + lbl_1_bss_1C[1].x * sys->orbitT[1];
            model->posP[idx + 15]->y = lbl_1_bss_1C[0].y + 200.0f * sys->orbitT[2];
            model->posP[idx + 15]->z = lbl_1_bss_1C[0].z + 600.0f * sys->orbitT[2];
            if(i == idx) {
                if(sys->orbitTime[0] > 0.5f && sys->waveActiveF == 0) {
                    MdOptionWaveEffectSet(3, 0);
                }
                if(sys->orbitTime[0] > 0.7f) {
                    Hu3DModelAttrSet(model->mdlId[i + 15], HU3D_ATTR_DISPOFF);
                    if(mode == 22) {
                        procRet = MdProcTbl[4](0);
                        if(idx == 1 && sys->list == 0) {
                            MdOptionMgListUpdate(sys->sprite);
                        }
                        MdOptionMgListWinUpdate(sys->sprite, 0, 0, 0);
                    }
                }
            } else {
                if(i < idx) {
                    MdMathLerp(&angle, 90.0f + 180.0f * (float)i, 0.0f, sys->orbitT[0]);
                } else {
                    MdMathLerp(&angle, 90.0f + 180.0f * (float)i, 360.0f, sys->orbitT[0]);
                }
                MdMathLerp(&scale, 1.0f, 0.8f, sys->orbitTime[0]);
                center1.x = posP->x;
                center1.y = posP->y;
                center1.z = posP->z;
                centerP1 = &center1;
                MdMathPolarCoord(&out, centerP1, 180.0f, 180.0f, scale, angle);
                Hu3DModelPosSetV(model->mdlId[i + 15], &out);
                MdMathLerp(&scale, 0.5f, 0.32f, sys->orbitTime[0]);
                Hu3DModelScaleSet(model->mdlId[i + 15], scale, scale, scale);
            }
        }
        if(mode == 21 && sys->orbitTime[0] > 0.5f) {
            MdMathOscillate(MDMATH_OSC_90, &sys->orbitTime[3], &sys->orbitT[3], 40.0f);
            for(i = 0; i < 3; i++) {
                MdMathLerp(&angle, 0.0f, 90.0f + 90.0f * (float)i, sys->orbitT[3]);
                MdMathLerp(&scale, 0.8f, 1.0f, sys->orbitT[3]);
                center2.x = posP->x;
                center2.y = posP->y;
                center2.z = posP->z;
                centerP2 = &center2;
                MdMathPolarCoord(&out, centerP2, 180.0f, 180.0f, scale, angle);
                Hu3DModelAttrReset(model->mdlId[i + 17], HU3D_ATTR_DISPOFF);
                Hu3DModelPosSetV(model->mdlId[i + 17], &out);
                MdMathLerp(&scale, 0.32f, 0.5f, sys->orbitT[3]);
                Hu3DModelScaleSet(model->mdlId[i + 17], scale, scale, scale);
            }
        }
    } else if(mode >= 27 && mode <= 29) {
        MdMathOscillate(MDMATH_OSC_90, &sys->orbitTime[0], &sys->orbitT[0], 40.0f);
        MdMathOscillate(MDMATH_OSC_180, &sys->orbitTime[1], &sys->orbitT[1], 40.0f);
        MdMathOscillate(MDMATH_OSC_360, &sys->orbitTime[2], &sys->orbitT[2], 40.0f);
        idx = mode - 27;
        if(idx != 1) {
            yMul = 200.0f;
            zMul = 600.0f;
            zTimeMul = 0.0f;
            yTimeMul = 0.0f;
        } else {
            yMul = 320.0f;
            zMul = 280.0f;
            zTimeMul = -370.0f;
            yTimeMul = -160.0f;
        }
        if(sys->waveActiveF == 0) {
            if(sys->orbitTime[0] > 0.5f && idx != 1) {
                GwCommon.soundOutputMode = sys->sub;
                MdModelSoundTypeSet(GwCommon.soundOutputMode);
                msmSysSetOutputMode(GwCommon.soundOutputMode);
                if(GwCommon.soundOutputMode == 0) {
                    OSSetSoundMode(0);
                } else {
                    OSSetSoundMode(1);
                }
                MdOptionCapsuleOrbitOutSet(idx + 17);
                MdOptionWaveEffectSet(3, 0);
            } else if(sys->orbitTime[0] > 0.4f && idx == 1) {
                GwCommon.soundOutputMode = sys->sub;
                MdModelSoundTypeSet(GwCommon.soundOutputMode);
                msmSysSetOutputMode(GwCommon.soundOutputMode);
                if(GwCommon.soundOutputMode == 0) {
                    OSSetSoundMode(0);
                } else {
                    OSSetSoundMode(1);
                }
                MdOptionCapsuleOrbitOutSet(idx + 17);
                MdOptionWaveEffectSet(3, 1);
            }
        }
        model->posP[idx + 17]->x = lbl_1_bss_1C[0].x + lbl_1_bss_1C[1].x * sys->orbitT[1];
        model->posP[idx + 17]->y = lbl_1_bss_1C[0].y + yMul * sys->orbitT[2] + yTimeMul * sys->orbitT[1];
        model->posP[idx + 17]->z = lbl_1_bss_1C[0].z + zMul * sys->orbitT[2] + zTimeMul * sys->orbitT[1];
        if(mode == 27) {
            model->posP[18]->x = 100.0f * sys->orbitT[1];
        }
        if(mode == 29) {
            model->posP[18]->x = -100.0f * sys->orbitT[1];
        }
    } else if(mode >= 23 && mode <= 26) {
        MdMathOscillate(MDMATH_OSC_90, &sys->orbitTime[0], &sys->orbitT[0], 40.0f);
        MdMathOscillate(MDMATH_OSC_180, &sys->orbitTime[1], &sys->orbitT[1], 40.0f);
        MdMathOscillate(MDMATH_OSC_360, &sys->orbitTime[2], &sys->orbitT[2], 40.0f);
        idx = mode - 23;
        for(i = 0; i < 4; i++) {
            if(i == idx) {
                if(i == 0 || i == 3) {
                    yMul = 200.0f;
                    zMul = 600.0f;
                    zTimeMul = 0.0f;
                } else {
                    yMul = 250.0f;
                    zMul = 450.0f;
                    zTimeMul = -400.0f;
                }
                model->posP[i + 20]->x = lbl_1_bss_1C[0].x + lbl_1_bss_1C[1].x * sys->orbitT[1];
                model->posP[i + 20]->y = lbl_1_bss_1C[0].y + yMul * sys->orbitT[2];
                model->posP[i + 20]->z = lbl_1_bss_1C[0].z + zTimeMul * sys->orbitTime[0] + zMul * sys->orbitT[2];
                if(sys->orbitTime[0] > 0.5f && sys->waveActiveF == 0) {
                    if(i == 0 || i == 3) {
                        MdOptionWaveEffectSet(4, 0);
                    } else {
                        MdOptionWaveEffectSet(4, 1);
                    }
                }
                if(sys->orbitTime[0] > 0.7f) {
                    if(idx != 1) {
                        procRet = MdProcTbl[4](0);
                        if(idx == 0) {
                            if(sys->list == 0) {
                                MdOptionMgListUpdate(sys->sprite);
                            }
                            MdOptionMgListWinUpdate(sys->sprite, 0, 0, 0);
                        }
                        if(idx == 2) {
                            MdOptionDecaRecordUpdate(sys->sprite);
                        }
                        if(idx == 3) {
                            MdOptionSdRecordUpdate(sys->sprite);
                        }
                    }
                    Hu3DModelAttrSet(model->mdlId[i + 20], HU3D_ATTR_DISPOFF);
                }
            } else {
                if(i < idx) {
                    MdMathLerp(&angle, 90.0f + 60.0f * (float)i, 0.0f, sys->orbitT[0]);
                } else {
                    MdMathLerp(&angle, 90.0f + 60.0f * (float)i, 360.0f, sys->orbitT[0]);
                }
                MdMathLerp(&scale, 1.0f, 0.8f, sys->orbitTime[0]);
                center3.x = posP->x;
                center3.y = posP->y;
                center3.z = posP->z;
                centerP3 = &center3;
                MdMathPolarCoord(&out, centerP3, 180.0f, 180.0f, scale, angle);
                Hu3DModelPosSetV(model->mdlId[i + 20], &out);
                MdMathLerp(&scale, 0.5f, 0.32f, sys->orbitTime[0]);
                Hu3DModelScaleSet(model->mdlId[i + 20], scale, scale, scale);
            }
        }
        if(mode == 24) {
            if(sys->orbitTime[0] > 0.5f && sys->waveActiveF == 0) {
                MdOptionWaveEffectSet(4, 0);
            }
            if(sys->orbitTime[0] > 0.7f) {
                MdMathOscillate(MDMATH_OSC_90, &sys->orbitTime[3], &sys->orbitT[3], 40.0f);
                for(i = 0; i < 2; i++) {
                    MdMathLerp(&angle, 0.0f, 90.0f + 180.0f * (float)i, sys->orbitT[3]);
                    MdMathLerp(&scale, 0.8f, 1.0f, sys->orbitT[3]);
                    center4.x = posP->x;
                    center4.y = posP->y;
                    center4.z = posP->z;
                    centerP4 = &center4;
                    MdMathPolarCoord(&out, centerP4, 180.0f, 180.0f, scale, angle);
                    Hu3DModelAttrReset(model->mdlId[i + 24], HU3D_ATTR_DISPOFF);
                    Hu3DModelPosSetV(model->mdlId[i + 24], &out);
                    MdMathLerp(&scale, 0.32f, 0.5f, sys->orbitT[3]);
                    Hu3DModelScaleSet(model->mdlId[i + 24], scale, scale, scale);
                }
            }
        }
    } else if(mode >= 30 && mode <= 31) {
        MdMathOscillate(MDMATH_OSC_90, &sys->orbitTime[0], &sys->orbitT[0], 40.0f);
        MdMathOscillate(MDMATH_OSC_180, &sys->orbitTime[1], &sys->orbitT[1], 40.0f);
        MdMathOscillate(MDMATH_OSC_360, &sys->orbitTime[2], &sys->orbitT[2], 40.0f);
        idx = mode - 30;
        for(i = 0; i < 2; i++) {
            if(i == idx) {
                model->posP[idx + 24]->x = lbl_1_bss_1C[0].x + lbl_1_bss_1C[1].x * sys->orbitT[1];
                model->posP[idx + 24]->y = lbl_1_bss_1C[0].y + 200.0f * sys->orbitT[2];
                model->posP[idx + 24]->z = lbl_1_bss_1C[0].z + 600.0f * sys->orbitT[2];
            } else {
                if(i < idx) {
                    MdMathLerp(&angle, 90.0f + 180.0f * (float)i, 0.0f, sys->orbitT[0]);
                } else {
                    MdMathLerp(&angle, 90.0f + 180.0f * (float)i, 360.0f, sys->orbitT[0]);
                }
                MdMathLerp(&scale, 1.0f, 0.8f, sys->orbitTime[0]);
                center5.x = posP->x;
                center5.y = posP->y;
                center5.z = posP->z;
                centerP5 = &center5;
                MdMathPolarCoord(&out, centerP5, 180.0f, 180.0f, scale, angle);
                Hu3DModelPosSetV(model->mdlId[i + 24], &out);
                MdMathLerp(&scale, 0.5f, 0.32f, sys->orbitTime[0]);
                Hu3DModelScaleSet(model->mdlId[i + 24], scale, scale, scale);
            }
        }
        if(sys->orbitTime[0] > 0.5f && sys->waveActiveF == 0) {
            MdOptionWaveEffectSet(4, 0);
        }
        if(sys->orbitTime[0] > 0.7f) {
            Hu3DModelAttrSet(model->mdlId[idx + 24], HU3D_ATTR_DISPOFF);
            procRet = MdProcTbl[4](0);
            if(idx == 0) {
                MdOptionBoardRecordUpdate(sys->sprite, 0);
            }
            if(idx == 1) {
                MdOptionStoryRecordUpdate(sys->sprite);
            }
        }
    }
    switch(mode) {
        case 0:
        case 4:
            for(i = 0; i < 5; i++) {
                if(mode == 0) {
                    MdMathLerp(&angle, 12.5f, 90.0f + 45.0f * (float)i, sys->orbitT[0]);
                } else {
                    MdMathLerp(&angle, 360.0f, 90.0f + 45.0f * (float)i, sys->orbitT[0]);
                }
                MdMathLerp(&scale, 0.8f, 1.0f, sys->orbitT[0]);
                center6.x = posP->x;
                center6.y = posP->y;
                center6.z = posP->z;
                centerP6 = &center6;
                MdMathPolarCoord(&out, centerP6, 180.0f, 180.0f, scale, angle);
                Hu3DModelPosSetV(model->mdlId[i + 8], &out);
                MdMathLerp(&scale, 0.32f, 0.5f, sys->orbitT[0]);
                Hu3DModelScaleSet(model->mdlId[i + 8], scale, scale, scale);
            }
            break;

        case 1:
            for(i = 0; i < 2; i++) {
                MdMathLerp(&angle, 5.0f, 90.0f + 180.0f * (float)i, sys->orbitT[0]);
                MdMathLerp(&scale, 0.8f, 1.0f, sys->orbitT[0]);
                center7.x = posP->x;
                center7.y = posP->y;
                center7.z = posP->z;
                centerP7 = &center7;
                MdMathPolarCoord(&out, centerP7, 180.0f, 180.0f, scale, angle);
                Hu3DModelPosSetV(model->mdlId[i + 13], &out);
                MdMathLerp(&scale, 0.32f, 0.5f, sys->orbitT[0]);
                Hu3DModelScaleSet(model->mdlId[i + 13], scale, scale, scale);
            }
            break;

        case 2:
        case 5:
            for(i = 0; i < 2; i++) {
                if(mode == 2) {
                    MdMathLerp(&angle, -5.0f, 90.0f + 180.0f * (float)i, sys->orbitT[0]);
                } else {
                    MdMathLerp(&angle, 360.0f, 90.0f + 180.0f * (float)i, sys->orbitT[0]);
                }
                MdMathLerp(&scale, 0.8f, 1.0f, sys->orbitT[0]);
                center8.x = posP->x;
                center8.y = posP->y;
                center8.z = posP->z;
                centerP8 = &center8;
                MdMathPolarCoord(&out, centerP8, 180.0f, 180.0f, scale, angle);
                Hu3DModelPosSetV(model->mdlId[i + 15], &out);
                MdMathLerp(&scale, 0.32f, 0.5f, sys->orbitT[0]);
                Hu3DModelScaleSet(model->mdlId[i + 15], scale, scale, scale);
            }
            break;

        case 3:
        case 6:
            for(i = 0; i < 4; i++) {
                if(mode == 3) {
                    MdMathLerp(&angle, -20.0f, 90.0f + 60.0f * (float)i, sys->orbitT[0]);
                } else {
                    MdMathLerp(&angle, 360.0f, 90.0f + 60.0f * (float)i, sys->orbitT[0]);
                }
                MdMathLerp(&scale, 0.8f, 1.0f, sys->orbitT[0]);
                center9.x = posP->x;
                center9.y = posP->y;
                center9.z = posP->z;
                centerP9 = &center9;
                MdMathPolarCoord(&out, centerP9, 180.0f, 180.0f, scale, angle);
                Hu3DModelPosSetV(model->mdlId[i + 20], &out);
                MdMathLerp(&scale, 0.32f, 0.5f, sys->orbitT[0]);
                Hu3DModelScaleSet(model->mdlId[i + 20], scale, scale, scale);
            }
            break;

        case 7:
            for(i = 0; i < 2; i++) {
                MdMathLerp(&angle, 360.0f, 90.0f + 180.0f * (float)i, sys->orbitT[0]);
                MdMathLerp(&scale, 0.8f, 1.0f, sys->orbitT[0]);
                center10.x = posP->x;
                center10.y = posP->y;
                center10.z = posP->z;
                centerP10 = &center10;
                MdMathPolarCoord(&out, centerP10, 180.0f, 180.0f, scale, angle);
                Hu3DModelPosSetV(model->mdlId[i + 24], &out);
                MdMathLerp(&scale, 0.32f, 0.5f, sys->orbitT[0]);
                Hu3DModelScaleSet(model->mdlId[i + 24], scale, scale, scale);
            }
            break;

        case 8:
            for(i = 0; i < 5; i++) {
                MdMathLerp(&angle, 90.0f + 45.0f * (float)i, 12.5f, sys->orbitT[0]);
                MdMathLerp(&scale, 1.0f, 0.6f, sys->orbitTime[0]);
                center11.x = posP->x;
                center11.y = posP->y;
                center11.z = posP->z;
                centerP11 = &center11;
                MdMathPolarCoord(&out, centerP11, 180.0f, 180.0f, scale, angle);
                Hu3DModelPosSetV(model->mdlId[i + 8], &out);
                MdMathLerp(&scale, 0.5f, 0.32f, sys->orbitTime[0]);
                Hu3DModelScaleSet(model->mdlId[i + 8], scale, scale, scale);
            }
            break;

        case 9:
            for(i = 0; i < 2; i++) {
                MdMathLerp(&angle, 90.0f + 180.0f * (float)i, 5.0f, sys->orbitT[0]);
                MdMathLerp(&scale, 1.0f, 0.6f, sys->orbitTime[0]);
                center12.x = posP->x;
                center12.y = posP->y;
                center12.z = posP->z;
                centerP12 = &center12;
                MdMathPolarCoord(&out, centerP12, 180.0f, 180.0f, scale, angle);
                Hu3DModelPosSetV(model->mdlId[i + 13], &out);
                MdMathLerp(&scale, 0.5f, 0.32f, sys->orbitTime[0]);
                Hu3DModelScaleSet(model->mdlId[i + 13], scale, scale, scale);
            }
            break;

        case 10:
            for(i = 0; i < 2; i++) {
                MdMathLerp(&angle, 90.0f + 180.0f * (float)i, -5.0f, sys->orbitT[0]);
                MdMathLerp(&scale, 1.0f, 0.6f, sys->orbitTime[0]);
                center13.x = posP->x;
                center13.y = posP->y;
                center13.z = posP->z;
                centerP13 = &center13;
                MdMathPolarCoord(&out, centerP13, 180.0f, 180.0f, scale, angle);
                Hu3DModelPosSetV(model->mdlId[i + 15], &out);
                MdMathLerp(&scale, 0.5f, 0.32f, sys->orbitTime[0]);
                Hu3DModelScaleSet(model->mdlId[i + 15], scale, scale, scale);
            }
            break;

        case 11:
            for(i = 0; i < 4; i++) {
                MdMathLerp(&angle, 90.0f + 60.0f * (float)i, -10.0f, sys->orbitT[0]);
                MdMathLerp(&scale, 1.0f, 0.6f, sys->orbitTime[0]);
                center14.x = posP->x;
                center14.y = posP->y;
                center14.z = posP->z;
                centerP14 = &center14;
                MdMathPolarCoord(&out, centerP14, 180.0f, 180.0f, scale, angle);
                Hu3DModelPosSetV(model->mdlId[i + 20], &out);
                MdMathLerp(&scale, 0.5f, 0.32f, sys->orbitTime[0]);
                Hu3DModelScaleSet(model->mdlId[i + 20], scale, scale, scale);
            }
            break;

        case 12:
            for(i = 0; i < 3; i++) {
                MdMathLerp(&angle, 90.0f + 90.0f * (float)i, 0.0f, sys->orbitT[0]);
                MdMathLerp(&scale, 1.0f, 0.8f, sys->orbitTime[0]);
                center15.x = posP->x;
                center15.y = posP->y;
                center15.z = posP->z;
                centerP15 = &center15;
                MdMathPolarCoord(&out, centerP15, 180.0f, 180.0f, scale, angle);
                Hu3DModelPosSetV(model->mdlId[i + 17], &out);
                MdMathLerp(&scale, 0.5f, 0.32f, sys->orbitTime[0]);
                Hu3DModelScaleSet(model->mdlId[i + 17], scale, scale, scale);
            }
            for(i = 0; i < 2; i++) {
                MdMathLerp(&angle, 360.0f, 90.0f + 180.0f * (float)i, sys->orbitT[0]);
                MdMathLerp(&scale, 0.8f, 1.0f, sys->orbitT[0]);
                center16.x = posP->x;
                center16.y = posP->y;
                center16.z = posP->z;
                centerP16 = &center16;
                MdMathPolarCoord(&out, centerP16, 180.0f, 180.0f, scale, angle);
                Hu3DModelPosSetV(model->mdlId[i + 15], &out);
                MdMathLerp(&scale, 0.32f, 0.5f, sys->orbitT[0]);
                Hu3DModelScaleSet(model->mdlId[i + 15], scale, scale, scale);
            }
            break;

        case 13:
            for(i = 0; i < 2; i++) {
                MdMathLerp(&angle, 90.0f + 180.0f * (float)i, 0.0f, sys->orbitT[0]);
                MdMathLerp(&scale, 1.0f, 0.8f, sys->orbitTime[0]);
                center17.x = posP->x;
                center17.y = posP->y;
                center17.z = posP->z;
                centerP17 = &center17;
                MdMathPolarCoord(&out, centerP17, 180.0f, 180.0f, scale, angle);
                Hu3DModelPosSetV(model->mdlId[i + 24], &out);
                MdMathLerp(&scale, 0.5f, 0.32f, sys->orbitTime[0]);
                Hu3DModelScaleSet(model->mdlId[i + 24], scale, scale, scale);
            }
            for(i = 0; i < 4; i++) {
                MdMathLerp(&angle, 360.0f, 90.0f + 60.0f * (float)i, sys->orbitT[0]);
                MdMathLerp(&scale, 0.8f, 1.0f, sys->orbitT[0]);
                center18.x = posP->x;
                center18.y = posP->y;
                center18.z = posP->z;
                centerP18 = &center18;
                MdMathPolarCoord(&out, centerP18, 180.0f, 180.0f, scale, angle);
                Hu3DModelPosSetV(model->mdlId[i + 20], &out);
                MdMathLerp(&scale, 0.32f, 0.5f, sys->orbitT[0]);
                Hu3DModelScaleSet(model->mdlId[i + 20], scale, scale, scale);
            }
            break;
    }
    if(sys->orbitTime[0] >= 1.0f) {
        if((mode >= 14 && mode <= 18) || mode == 22 || mode == 30 || mode == 31 || mode == 23 || mode == 25 || mode == 26) {
            if(procRet != 0) {
                if(mode == 22) {
                    HuAudSStreamAllFadeOut(100);
                }
                result = 1;
                sys->unk50C = 1;
            }
        } else if((mode >= 21 && mode <= 22) || mode == 24) {
            if(sys->orbitTime[3] >= 1.0f) {
                result = 1;
            }
        } else {
            result = 1;
        }
        switch(mode) {
            case 14:
            case 15:
            case 16:
            case 17:
            case 18:
                for(i = 0; i < 5; i++) {
                    Hu3DModelAttrSet(model->mdlId[i + 8], HU3D_ATTR_DISPOFF);
                }
                break;

            case 21:
            case 22:
                for(i = 0; i < 2; i++) {
                    Hu3DModelAttrSet(model->mdlId[i + 15], HU3D_ATTR_DISPOFF);
                }
                break;

            case 23:
            case 24:
            case 25:
            case 26:
                for(i = 0; i < 4; i++) {
                    Hu3DModelAttrSet(model->mdlId[i + 20], HU3D_ATTR_DISPOFF);
                }
                break;

            case 30:
            case 31:
                for(i = 0; i < 2; i++) {
                    Hu3DModelAttrSet(model->mdlId[i + 24], HU3D_ATTR_DISPOFF);
                }
                break;

            case 8:
                sys->unk3DC = 0;
                MdSpriteFadeDispSet(0);
                for(i = 0; i < 5; i++) {
                    Hu3DModelAttrSet(model->mdlId[i + 8], HU3D_ATTR_DISPOFF);
                }
                break;

            case 9:
                sys->unk3DC = 0;
                MdSpriteFadeDispSet(0);
                for(i = 0; i < 2; i++) {
                    Hu3DModelAttrSet(model->mdlId[i + 13], HU3D_ATTR_DISPOFF);
                }
                break;

            case 10:
                sys->unk3DC = 0;
                MdSpriteFadeDispSet(0);
                for(i = 0; i < 2; i++) {
                    Hu3DModelAttrSet(model->mdlId[i + 15], HU3D_ATTR_DISPOFF);
                }
                break;

            case 11:
                sys->unk3DC = 0;
                MdSpriteFadeDispSet(0);
                for(i = 0; i < 4; i++) {
                    Hu3DModelAttrSet(model->mdlId[i + 20], HU3D_ATTR_DISPOFF);
                }
                break;

            case 12:
                for(i = 0; i < 3; i++) {
                    Hu3DModelAttrSet(model->mdlId[i + 17], HU3D_ATTR_DISPOFF);
                }
                break;

            case 13:
                for(i = 0; i < 2; i++) {
                    Hu3DModelAttrSet(model->mdlId[i + 24], HU3D_ATTR_DISPOFF);
                }
                break;
        }
        if(result == 1) {
            sys->waveActiveF = 0;
        }
    }
    return result;
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

/* 2D anchors of the star model (screen position + depth). */
HuVecF lbl_1_data_3E0[2] = {
    { 56.0f, 400.0f, 1280.0f },
    { 64.0f, -128.0f, 1280.0f }
};

/* 0xA324 - helper process: keeps the star model parked at its 2D
 * anchor, faces it to the camera and plays the requested motion. */
void fn_1_A324(void)
{    MDOPT_MODEL_WORK *model = sys->model;
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
                Hu3D2Dto3D(&lbl_1_data_3E0[0], camera->camera[1].bit, &lbl_1_bss_4[0]);
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
    if(cmd == 0) {
        lbl_1_bss_654 = HuAudBGMPlay(4);
    } else if(cmd == 1) {
        HuAudSStreamFadeOut(lbl_1_bss_654, 1000);
    } else if(cmd == 2) {
        HuAudFXPlay(0);
    } else if(cmd == 3) {
        HuAudFXPlay(2);
    } else if(cmd == 4) {
        HuAudFXPlay(3);
    } else if(cmd == 5) {
        HuAudFXPlay(4);
    } else if(cmd == 6) {
        HuAudFXPlay(0x5A5);
    } else if(cmd == 7) {
        HuAudFXPlay(0x5A8);
    } else if(cmd == 8) {
        HuAudFXPlay(0x5A6);
    } else if(cmd == 9) {
        HuAudFXPlay(0x5A7);
    } else if(cmd == 10) {
        HuAudFXPlay(0x5A9);
    } else if(cmd == 11) {
        HuAudAllStop();
    }
}
