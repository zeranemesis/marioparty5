#include "REL/mdstoryDll.h"
#include "datadir_enum.h"
#include "game/armem.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/frand.h"
#include "game/flag.h"
#include "game/gamework.h"
#include "game/hsfex.h"
#include "game/memory.h"
#include "game/saveload.h"
#include "game/wipe.h"

/*************************************************************
 * mdstory.c
 *
 * "mdstory" overlay.  The overlay shipped no symbol table for
 * its process entry points, so those carry objdiff's placeholder
 * names (fn_1_XXXX) keyed to their .text offset.
 *************************************************************/

//MDSTORY_DATA_BEGIN
typedef struct MdStoryPosTbl_s {
    HuVecF from;
    HuVecF to;
} MDSTORY_POSTBL;

static MDSTORY_POSTBL lbl_1_data_180[7] = {
    { { -250, 0, 0 }, { -250, 0, 0 } },
    { { -125, 0, 0 }, { -125, 0, 0 } },
    { { 0, 0, 0 }, { 0, 0, 0 } },
    { { 125, 0, 0 }, { 139, 17.5f, -500 } },
    { { 275, 9, -300 }, { 266, 2, -200 } },
    { { -250, -250, 0 }, { -250, -250, 0 } },
    { { -125, -250, 0 }, { -140.5f, -270, -400 } },
};

static MDSTORY_POSTBL lbl_1_data_228[7] = {
    { { 0, 0, 0 }, { 0, 0, 0 } },
    { { 0, 0, 0 }, { 0, 0, 0 } },
    { { 0, 0, 0 }, { 0, 0, 0 } },
    { { 10, -2, 0 }, { 10, -4, 0 } },
    { { 0, -3, 0 }, { 0, -6, 0 } },
    { { 0, 0, 0 }, { 0, 0, 0 } },
    { { 0, 0, 0 }, { 0, 0, 0 } },
};

static unsigned int lbl_1_data_30C[7] = {
    DATA_w01s,
    DATA_w02s,
    DATA_w03s,
    DATA_w04s,
    DATA_w05s,
    DATA_w06s,
    DATA_w07s
};

static float lbl_1_data_47C[8] = { 0, 16, 64, 32, -65, 220, 292, 1000 };

static float lbl_1_bss_D0;
static float lbl_1_bss_D4;
static float lbl_1_bss_D8;
static float lbl_1_bss_DC;
static float lbl_1_bss_E0;
static float lbl_1_bss_E4;
static HuVecF lbl_1_bss_E8[2];
static HuVecF lbl_1_bss_100[2];
static HU3DPARMANPARAM param1 = {
    0x1E,
    0,
    50.0f,
    10.0f,
    180.0f,
    { 0.0f, -0.05f, 0.0f },
    20.0f,
    0.8f,
    10.0f,
    1.0f,
    2,
    {
        { 0xFF, 0xFF, 0xFF, 0xFF },
        { 0xFF, 0xFF, 0x40, 0xFF },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
    },
    {
        { 0xFF, 0x80, 0x80, 0 },
        { 0xFF, 0x40, 0x20, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
    },
};
static HU3DPARMANPARAM param2 = {
    0x28,
    0,
    50.0f,
    0.0f,
    90.0f,
    { 0.0f, -0.05f, 0.0f },
    35.0f,
    0.88f,
    25.0f,
    0.99f,
    1,
    { { 0xFF, 0xFF, 0xFF, 0xFF }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
    { { 0xFF, 0x80, 0x80, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
};
static HU3DPARMANPARAM param3 = {
    0x1E,
    0,
    30.0f,
    60.0f,
    180.0f,
    { 0.0f, 0.0f, 0.0f },
    0.2f,
    1.0f,
    100.0f,
    1.0f,
    2,
    { { 0xFF, 0xFF, 0xFF, 0x20 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
    { { 0x40, 0x20, 0x20, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
};
static HU3DPARMANPARAM glowParam = {
    0x32,
    0,
    2.0f,
    30.0f,
    180.0f,
    { 0.0f, -0.2f, 0.0f },
    1.0f,
    0.8f,
    30.0f,
    0.95f,
    3,
    {
        { 0x80, 0xC0, 0xFF, 0xFF },
        { 0xFF, 0xFF, 0x40, 0xFF },
        { 0xFF, 0x60, 0x80, 0xFF },
        { 0, 0, 0, 0 },
    },
    {
        { 0x80, 0xC0, 0xFF, 0 },
        { 0xFF, 0x40, 0x20, 0 },
        { 0x80, 0x10, 0x10, 0 },
        { 0, 0, 0, 0 },
    },
};

typedef struct MdStoryCamLerp_s {
    HuVecF hvrFrom; //0x00
    HuVecF centerFrom; //0x0C
    HuVecF hvrTo; //0x18
    HuVecF centerTo; //0x24
    int oscType; //0x30
    float time; //0x34
} MDSTORY_CAMLERP;

static MDSTORY_CAMLERP lbl_1_data_544[1] = {
    {
        { -90, -12, 2500 },
        { 0, 400, 1750 },
        { -90, 0, 2500 },
        { 0, 200, -250 },
        MDMATH_OSC_90_REV,
        30
    },
};

static HuVecF lbl_1_data_57C = { 0, 0, -1000 };

static HuVecF lbl_1_data_588[7] = {
    { -330, 400, -600 },
    { -110, 400, -600 },
    { 110, 400, -600 },
    { 330, 400, -600 },
    { -220, 130, -600 },
    { 0, 130, -600 },
    { 220, 130, -600 },
};

static HuVecF lbl_1_data_5DC[4] = {
    { 120, -160, 750 },
    { 120, -160, 750 },
    { 120, -160, 750 },
    { 120, -160, 750 },
};

static HuVecF lbl_1_bss_58[2];
static HuVecF lbl_1_bss_70[2];
static MDDOT_WORK *lbl_1_bss_88[2];
static float lbl_1_bss_90;
static float lbl_1_bss_94;
static float lbl_1_bss_98;
static int lbl_1_bss_9C;
static HuVecF lbl_1_bss_A0[4];

static HuVecF lbl_1_data_60C[2] = {
    { 288, -128, 0 },
    { 288, 64, 0 },
};

static s8 lbl_1_data_624[4][7] = {
    { 0, 6, 5, 4, 3, 2, 1 },
    { 0, 1, 2, 3, 4, 5, 6 },
    { 0, 3, 6, 2, 5, 1, 4 },
    { 0, 4, 1, 5, 2, 6, 3 },
};

static int lbl_1_data_640[7] = { 513, 493, 613, 693, 653, 453, 673 };

static int lbl_1_bss_20;
static int lbl_1_bss_24;
static HuVecF lbl_1_bss_28[2];
static HuVecF lbl_1_bss_40[2];
//MDSTORY_DATA_END

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

extern void ObjectSetup(void);
extern void MdToueiLayerHook(s16 layerNo);

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

void ObjectSetup(void)
{
    int i;
    sys = &sysWork;
    sysP = sys;
    sys->subMode = 0;
    sys->unk4 = 0;
    sys->flowTime = 0;
    sys->flowMode = MDSTORY_FLOW_INIT;
    sys->ovlCallMode = MD_OVLCALL_GOTO;
    sys->nextDll = DLL_selmenuDLL;
    sys->exitF = FALSE;
    sys->skipWipeF = FALSE;
    _ClearFlag(FLAGNUM(FLAG_GROUP_COMMON, 14));
    GwSystem.partyF = FALSE;
    for(i=0; i<7; i++) {
        HuDataDirClose(lbl_1_data_30C[i]);
    }
    sys->objman = omInitObjMan(512, 8192);
    sys->ovlHis = omOvlHisGet(0);
    sys->evtNo = sys->ovlHis->evtno;
    omGameSysInit(sys->objman);
    sys->obj[MDSTORY_OBJ_DEBUGFONT] = omAddObjEx(sys->objman, 100, 0, 0, -1, MdObjDebugFontCreate);
    sys->obj[MDSTORY_OBJ_DEBUGFONT]->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDDEBUGFONT_WORK), HU_MEMNUM_OVL);
    if(!sys->obj[MDSTORY_OBJ_DEBUGFONT]->data) {
        OSReport("sys->obj[MDSTORY_OBJ_DEBUGFONT]->data = NULL...\n");
    }
    sys->debugFont = sys->obj[MDSTORY_OBJ_DEBUGFONT]->data;
    sys->debugFont->createF = FALSE;
    sys->obj[MDSTORY_OBJ_PAD] = omAddObjEx(sys->objman, 101, 0, 0, -1, MdObjPadMain);
    sys->obj[MDSTORY_OBJ_PAD]->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDPAD_WORK), HU_MEMNUM_OVL);
    if(!sys->obj[MDSTORY_OBJ_PAD]->data) {
        OSReport("sys->obj[MDSTORY_OBJ_PAD]->data = NULL...\n");
    }
    sys->pad = sys->obj[MDSTORY_OBJ_PAD]->data;
    sys->obj[MDSTORY_OBJ_CAMERA] = omAddObjEx(sys->objman, 102, 0, 0, -1, MdObjCameraCreate);
    sys->obj[MDSTORY_OBJ_CAMERA]->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDCAMERA_WORK), HU_MEMNUM_OVL);
    if(!sys->obj[MDSTORY_OBJ_CAMERA]->data) {
        OSReport("sys->obj[MDSTORY_OBJ_CAMERA]->data = NULL...\n");
    }
    sys->camera = sys->obj[MDSTORY_OBJ_CAMERA]->data;
    sys->camera->createF = FALSE;
    sys->camera->debugFont = sys->debugFont;
    sys->obj[MDSTORY_OBJ_LIGHT] = omAddObjEx(sys->objman, 103, 0, 0, -1, MdObjLightCreate);
    sys->obj[MDSTORY_OBJ_LIGHT]->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDLIGHT_WORK), HU_MEMNUM_OVL);
    if(!sys->obj[MDSTORY_OBJ_LIGHT]->data) {
        OSReport("sys->obj[MDSTORY_OBJ_LIGHT]->data = NULL...\n");
    }
    sys->light = sys->obj[MDSTORY_OBJ_LIGHT]->data;
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
    sys->star = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDSTORY_STAR), HU_MEMNUM_OVL);
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

void MdMain(void)
{
    if(sys->evtNo == 1) {
        omOvlReturn(1);
        HuPrcEnd();
        while(1) {
            HuPrcVSleep();
        }
    }
    MdInit();
    while(!omSysExitReq && !sys->exitF) {
        MdFlowTbl[sys->flowMode]();
        if(++sys->flowTime > 36000) {
            sys->flowTime = 0;
        }
        HuPrcVSleep();
    }
    MdClose();
}

static void LoadProc(void)
{
    int i;
    OSTick tick;
    s16 chrNo;
    while(1) {
        switch(sys->loadState) {
            case MD_LOAD_STATE_NONE:
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
                while(HuARDMACheck() != 0) {
                }
                HuDataDirClose(DATA_board);
                sys->loadState = MD_LOAD_STATE_STORYMODE_LOAD;
                break;

            case MD_LOAD_STATE_STORYMODE_LOAD:
                tick = OSGetTick();
                sys->loadAsyncStat = HuDataDirReadAsync(DATA_storymode);
                while(!HuDataGetAsyncStat(sys->loadAsyncStat)) {
                    HuPrcVSleep();
                }
                OSReport("Load Time:%d\n", OSTicksToMilliseconds(OSGetTick()-tick));
                OSReport("Finished!\n");
                sys->loadProcReady = FALSE;
                sys->loadState = MD_LOAD_STATE_CHAR_WAIT;
                break;

            case MD_LOAD_STATE_CHAR_WAIT:
                if(sys->loadProcReady == TRUE) {
                    sys->loadState = MD_LOAD_STATE_CHAR_LOAD;
                }
                break;

            case MD_LOAD_STATE_CHAR_LOAD:
                for(i=0; i<4; i++) {
                    if(i == 0) {
                        chrNo = GwPlayerConf[0].charNo;
                    } else {
                        chrNo = i+9;
                    }
                    tick = OSGetTick();
                    sys->loadAsyncStat = HuDataDirReadAsync(charDataDirTbl[chrNo][4]);
                    while(!HuDataGetAsyncStat(sys->loadAsyncStat)) {
                        HuPrcVSleep();
                    }
                    OSReport("Load Time:%d\n", OSTicksToMilliseconds(OSGetTick()-tick));
                    OSReport("Finished!\n");
                    CharMotionInit(chrNo);
                }
                sys->loadState = MD_LOAD_STATE_DONE;
                break;

            case MD_LOAD_STATE_DONE:
                HuPrcEnd();
                break;
        }
        HuPrcVSleep();
    }
}

static void fn_1_8B94(int mdlNo, int cameraNo);
static s16 fn_1_9524(int mdlNo, int cameraNo);
static void fn_1_9384(int cmd);
static BOOL fn_1_8948(int padNo);
static void fn_1_87F4(void);
static void fn_1_8C84(void);

/* 0xC24 - star (cursor model) position/animation driver */
static void fn_1_C24(void)
{
    MDSTORY_STAR *star;
    MDMODEL_WORK *model;
    HuVecF *pos;
    MDCAMERA_WORK *cameraWork;
    int i;
    HuVecF *rot;
    MDMESS *mess;
    HuVecF *scale;
    model = sys->model;
    cameraWork = sys->camera;
    mess = sys->mess;
    star = sys->star;
    pos = model->posP[MDMODEL_STAR];
    rot = model->rotP[MDMODEL_STAR];
    scale = model->scaleP[MDMODEL_STAR];
    star->mode = 0;
    star->motNext = 1;
    pos->x = 0;
    pos->y = 0;
    pos->z = 1800;
    fn_1_8B94(0, 2);
    while(1) {
        model = sys->model;
        cameraWork = sys->camera;
        mess = sys->mess;
        star = sys->star;
        pos = model->posP[MDMODEL_STAR];
        rot = model->rotP[MDMODEL_STAR];
        scale = model->scaleP[MDMODEL_STAR];
        switch(star->mode) {
            case 1:
                lbl_1_bss_E4 = 0;
                lbl_1_bss_E0 = 0;
                lbl_1_bss_DC = 0;
                lbl_1_bss_D8 = 0;
                lbl_1_bss_D4 = 0;
                lbl_1_bss_D0 = 0;
                MdMathCurveInit(5, sys->curveTime, lbl_1_data_47C, sys->curveSlope);
                star->mode = 2;
                break;

            case 2:
                MdMathOscillate(MDMATH_OSC_90_REV, &lbl_1_bss_E4, &lbl_1_bss_D8, 60);
                MdMathOscillate(MDMATH_OSC_180, &lbl_1_bss_E0, &lbl_1_bss_D4, 60);
                MdMathOscillate(MDMATH_OSC_0, &lbl_1_bss_DC, &lbl_1_bss_D0, 60);
                MdMathLerp(&pos->x, 0, 45, lbl_1_bss_D4);
                MdMathCurveGet(&pos->y, 5, sys->curveTime, lbl_1_data_47C, sys->curveSlope, lbl_1_bss_D0);
                MdMathLerp(&pos->z, 1800, 1300, lbl_1_bss_D8);
                MdMathLerp(&rot->y, 0, 360, lbl_1_bss_D8);
                if(lbl_1_bss_E4 >= 1) {
                    star->mode = 3;
                }
                break;

            case 4:
                lbl_1_bss_E4 = 0;
                lbl_1_bss_E0 = 0;
                lbl_1_bss_D8 = 0;
                lbl_1_bss_D4 = 0;
                lbl_1_bss_100[0].x = 64;
                lbl_1_bss_100[0].y = 320;
                lbl_1_bss_100[0].z = 1000;
                lbl_1_bss_100[1].x = 288;
                lbl_1_bss_100[1].y = 240;
                lbl_1_bss_100[1].z = 1000;
                for(i=0; i<2; i++) {
                    Hu3D2Dto3D(&lbl_1_bss_100[i], cameraWork->camera[MDCAMERA_FG].bit, &lbl_1_bss_E8[i]);
                }
                star->mode = 5;
                break;

            case 5:
                MdMathOscillate(MDMATH_OSC_90, &lbl_1_bss_E4, &lbl_1_bss_D8, 30);
                MdMathLerp(&pos->x, lbl_1_bss_E8[0].x, lbl_1_bss_E8[1].x, lbl_1_bss_D8);
                MdMathLerp(&pos->y, lbl_1_bss_E8[0].y, lbl_1_bss_E8[1].y, lbl_1_bss_D8);
                MdMathLerp(&pos->z, lbl_1_bss_E8[0].z, lbl_1_bss_E8[1].z, lbl_1_bss_D8);
                if(lbl_1_bss_E4 >= 1) {
                    star->mode = 6;
                }
                break;

            case 7:
                lbl_1_bss_E4 = 0;
                lbl_1_bss_E0 = 0;
                lbl_1_bss_DC = 0;
                lbl_1_bss_D8 = 0;
                lbl_1_bss_D4 = 0;
                lbl_1_bss_D0 = 0;
                lbl_1_bss_E8[0].x = 0;
                lbl_1_bss_E8[0].y = -65;
                lbl_1_bss_E8[0].z = 1300;
                lbl_1_bss_100[1].x = 220;
                lbl_1_bss_100[1].y = 292;
                lbl_1_bss_100[1].z = 1000;
                Hu3D2Dto3D(&lbl_1_bss_100[1], cameraWork->camera[MDCAMERA_FG].bit, &lbl_1_bss_E8[1]);
                star->mode = 8;
                break;

            case 8:
                MdMathOscillate(MDMATH_OSC_90, &lbl_1_bss_E4, &lbl_1_bss_D8, 30);
                MdMathOscillate(MDMATH_OSC_180, &lbl_1_bss_E0, &lbl_1_bss_D4, 30);
                MdMathLerp(&pos->x, lbl_1_bss_E8[0].x, lbl_1_bss_E8[1].x, lbl_1_bss_D8);
                MdMathLerp(&pos->y, lbl_1_bss_E8[0].y, lbl_1_bss_E8[1].y, lbl_1_bss_D8);
                MdMathLerpOfs(&pos->z, lbl_1_bss_E8[0].z, lbl_1_bss_E8[1].z, lbl_1_bss_D8, lbl_1_bss_D4);
                if(lbl_1_bss_E4 >= 1) {
                    star->mode = 9;
                }
                fn_1_8B94(0, 2);
                break;

            case 10:
                lbl_1_bss_E4 = 0;
                lbl_1_bss_E0 = 0;
                lbl_1_bss_DC = 0;
                lbl_1_bss_D8 = 0;
                lbl_1_bss_D4 = 0;
                lbl_1_bss_D0 = 0;
                lbl_1_bss_E8[0].x = 0;
                lbl_1_bss_E8[0].y = -65;
                lbl_1_bss_E8[0].z = 1300;
                lbl_1_bss_100[1].x = 220;
                lbl_1_bss_100[1].y = 292;
                lbl_1_bss_100[1].z = 1000;
                Hu3D2Dto3D(&lbl_1_bss_100[1], cameraWork->camera[MDCAMERA_FG].bit, &lbl_1_bss_E8[1]);
                star->mode = 11;
                break;

            case 11:
                MdMathOscillate(MDMATH_OSC_90_REV, &lbl_1_bss_E4, &lbl_1_bss_D8, 30);
                MdMathOscillate(MDMATH_OSC_180, &lbl_1_bss_E0, &lbl_1_bss_D4, 30);
                MdMathLerp(&pos->x, lbl_1_bss_E8[1].x, lbl_1_bss_E8[0].x, lbl_1_bss_D8);
                MdMathLerp(&pos->y, lbl_1_bss_E8[1].y, lbl_1_bss_E8[0].y, lbl_1_bss_D8);
                MdMathLerpOfs(&pos->z, lbl_1_bss_E8[1].z, lbl_1_bss_E8[0].z, lbl_1_bss_D8, -1*lbl_1_bss_D4);
                if(lbl_1_bss_E4 >= 1) {
                    star->mode = 12;
                }
                fn_1_8B94(0, 2);
                break;

            case 12:
                break;
        }
        switch(star->motNext) {
            case 0:
                break;

            case 1:
                Hu3DModelAttrSet(model->mdlId[MDMODEL_STAR], HU3D_MOTATTR_LOOP);
                Hu3DModelAttrReset(model->mdlId[MDMODEL_STAR], HU3D_MOTATTR_PAUSE);
                Hu3DMotionSet(model->mdlId[MDMODEL_STAR], model->animId[0]);
                star->motNext = 0;
                break;

            case 2:
                Hu3DMotionShiftSet(model->mdlId[MDMODEL_STAR], model->animId[1], 0, 32, HU3D_MOTATTR_LOOP);
                star->motNext = 0;
                break;

            case 3:
                Hu3DMotionShiftSet(model->mdlId[MDMODEL_STAR], model->animId[0], 0, 32, HU3D_MOTATTR_LOOP);
                star->motNext = 0;
                break;
        }
        HuPrcVSleep();
    }
}

/* 0x1728 - player-marker sprite scale driver */
static void fn_1_1728(void)
{
    MDSTORY_PLAYERSPR *playerSpr;
    HuVec2f *scale;
    int i;
    MDSPRITE_WORK *sprite;
    HUSPRGRPID grpId;
    MDCAMERA_WORK *camera;
    MDMODEL_WORK *model;
    camera = sys->camera;
    model = sys->model;
    sprite = sys->sprite;
    grpId = sprite->grpId[3];
    playerSpr = sys->playerSpr;
    for(i=0; i<MDSTORY_PLAYERSPR_NUM; i++, playerSpr++) {
        playerSpr->mode = MDSTORY_PLAYERSPR_NONE;
    }
    while(1) {
        camera = sys->camera;
        model = sys->model;
        sprite = sys->sprite;
        grpId = sprite->grpId[3];
        HuSprGrpPosSet(grpId, 0, 0);
        playerSpr = sys->playerSpr;
        for(i=0; i<MDSTORY_PLAYERSPR_NUM; i++, playerSpr++) {
            scale = MdSpriteScaleGet(sprite, 3, i);
            switch(playerSpr->mode) {
                case MDSTORY_PLAYERSPR_NONE:
                case MDSTORY_PLAYERSPR_MOVE:
                case MDSTORY_PLAYERSPR_TALK_END:
                    break;

                case MDSTORY_PLAYERSPR_START:
                    HuSprDispOn(grpId, i);
                    scale->x = 0;
                    scale->y = 0;
                    playerSpr->time1 = 0;
                    playerSpr->time2 = 0;
                    playerSpr->t1 = 0;
                    playerSpr->t2 = 0;
                    playerSpr->mode = MDSTORY_PLAYERSPR_END;
                    break;

                case MDSTORY_PLAYERSPR_END:
                    MdMathOscillate(MDMATH_OSC_90, &playerSpr->time1, &playerSpr->t1, 15);
                    MdMathOscillate(MDMATH_OSC_180, &playerSpr->time2, &playerSpr->t2, 15);
                    MdMathLerpOfs(&scale->x, 0, 1, playerSpr->t1, 0.5f*playerSpr->t2);
                    scale->y = scale->x;
                    if(playerSpr->time1 >= 1) {
                        playerSpr->mode = MDSTORY_PLAYERSPR_MOVE;
                    }
                    break;

                case MDSTORY_PLAYERSPR_MOVE_END:
                    playerSpr->time1 = 0;
                    playerSpr->time2 = 0;
                    playerSpr->t1 = 0;
                    playerSpr->t2 = 0;
                    playerSpr->mode = MDSTORY_PLAYERSPR_TALK;
                    break;

                case MDSTORY_PLAYERSPR_TALK:
                    MdMathOscillate(MDMATH_OSC_90, &playerSpr->time1, &playerSpr->t1, 15);
                    MdMathLerp(&scale->x, 1, 0, playerSpr->t1);
                    scale->y = scale->x;
                    if(playerSpr->time1 >= 1) {
                        HuSprDispOff(grpId, i);
                        playerSpr->mode = MDSTORY_PLAYERSPR_TALK_END;
                    }
                    break;
            }
        }
        HuPrcVSleep();
    }
}

/* 0x1A40 - character model position driver */
static void fn_1_1A40(void)
{
    MDSTORY_MODEL_WORK *modelWork;
    int i;
    HuVecF *scale;
    MDMODEL_WORK *model;
    HuVecF *pos;
    HuVecF *rot;
    MDCAMERA_WORK *cameraWork;
    MDSPRITE_WORK *sprite;
    cameraWork = sys->camera;
    model = sys->model;
    sprite = sys->sprite;
    modelWork = sys->modelWork;
    for(i=0; i<MDSTORY_MODELWORK_NUM; i++, modelWork++) {
        modelWork->mode = MDSTORY_MODELWORK_NONE;
    }
    while(1) {
        cameraWork = sys->camera;
        model = sys->model;
        sprite = sys->sprite;
        modelWork = sys->modelWork;
        for(i=0; i<MDSTORY_MODELWORK_NUM; i++, modelWork++) {
            if(i != 0 && i != 1 && i != 2 && i != 5) {
                pos = model->posP[i+MDMODEL_MARIO];
                rot = model->rotP[i+MDMODEL_MARIO];
                scale = model->scaleP[i+MDMODEL_MARIO];
                switch(modelWork->mode) {
                    case 0:
                        break;

                    case 1:
                        *pos = lbl_1_data_180[i].from;
                        *rot = lbl_1_data_228[i].from;
                        modelWork->mode = MDSTORY_MODELWORK_NONE;
                        break;

                    case 2:
                    case 3:
                        modelWork->posFrom.x = pos->x;
                        modelWork->posFrom.y = pos->y;
                        modelWork->posFrom.z = pos->z;
                        modelWork->rotFrom.x = rot->x;
                        modelWork->rotFrom.y = rot->y;
                        modelWork->rotFrom.z = rot->z;
                        if(modelWork->mode == 2) {
                            modelWork->posTo.x = lbl_1_data_180[i].to.x;
                            modelWork->posTo.y = lbl_1_data_180[i].to.y;
                            modelWork->posTo.z = lbl_1_data_180[i].to.z;
                            modelWork->rotTo.x = lbl_1_data_228[i].to.x;
                            modelWork->rotTo.y = lbl_1_data_228[i].to.y;
                            modelWork->rotTo.z = lbl_1_data_228[i].to.z;
                        } else if(modelWork->mode == 3) {
                            modelWork->posTo.x = lbl_1_data_180[i].from.x;
                            modelWork->posTo.y = lbl_1_data_180[i].from.y;
                            modelWork->posTo.z = lbl_1_data_180[i].from.z;
                            modelWork->rotTo.x = lbl_1_data_228[i].from.x;
                            modelWork->rotTo.y = lbl_1_data_228[i].from.y;
                            modelWork->rotTo.z = lbl_1_data_228[i].from.z;
                        }
                        modelWork->time = 0;
                        modelWork->ofsTime = 0;
                        modelWork->t = 0;
                        modelWork->ofsT = 0;
                        if(modelWork->mode == 2) {
                            modelWork->mode = 4;
                        } else if(modelWork->mode == 3) {
                            modelWork->mode = 5;
                        }
                        break;

                    case 4:
                    case 5:
                        if(modelWork->mode == 4) {
                            MdMathOscillate(MDMATH_OSC_0, &modelWork->time, &modelWork->t, 15);
                        } else if(modelWork->mode == 5) {
                            MdMathOscillate(MDMATH_OSC_0, &modelWork->time, &modelWork->t, 15);
                        }
                        MdMathLerp(&pos->x, modelWork->posFrom.x, modelWork->posTo.x, modelWork->t);
                        MdMathLerp(&pos->y, modelWork->posFrom.y, modelWork->posTo.y, modelWork->t);
                        MdMathLerp(&pos->z, modelWork->posFrom.z, modelWork->posTo.z, modelWork->t);
                        MdMathLerp(&rot->x, modelWork->rotFrom.x, modelWork->rotTo.x, modelWork->t);
                        MdMathLerp(&rot->y, modelWork->rotFrom.y, modelWork->rotTo.y, modelWork->t);
                        MdMathLerp(&rot->z, modelWork->rotFrom.z, modelWork->rotTo.z, modelWork->t);
                        if(modelWork->time >= 1) {
                            modelWork->mode = MDSTORY_MODELWORK_NONE;
                        }
                        break;
                }
            }
        }
        HuPrcVSleep();
    }
}

/* 0x1E3C - RingUpdate: per-frame ring animation driver */
static void fn_1_1E3C(void)
{
    while(1) {
        MdRingMain();
        HuPrcVSleep();
    }
}

/* 0x1E54 - MainWinUpdate: per-frame main message window driver */
static void fn_1_1E54(void)
{
    MDMESS *mess;
    mess = sys->mess;
    while(1) {
        mess = sys->mess;
        MdMessMain(mess, MDMESS_WIN_MAIN);
        HuPrcVSleep();
    }
}

/* 0x1E98 - SubWinUpdate: per-frame help/team text window driver */
static void fn_1_1E98(void)
{
    MDMESS *mess;
    mess = sys->mess;
    while(1) {
        mess = sys->mess;
        MdMessMain(mess, MDMESS_WIN_SUB);
        HuPrcVSleep();
    }
}

void MdInit(void)
{
    MdSpriteCreate(sys->sprite);
    HuPrcVSleep();
    MdRingCreate();
    MdModelCreate(sys->model, sys->camera);
    MdToueiTexBufInit();
    HuPrcVSleep();
    HuDataDirClose(DATA_mdstory);
    HuPrcVSleep();
    MdToueiMdlInit();
    sys->toueiProc = HuPrcChildCreate(fn_1_C24, 107, 12288, 0, sys->objman);
    sys->stageProc = HuPrcChildCreate(fn_1_1728, 108, 12288, 0, sys->objman);
    sys->chrProc = HuPrcChildCreate(fn_1_1A40, 109, 12288, 0, sys->objman);
    sys->mainWinProc = HuPrcChildCreate(fn_1_1E54, 110, 4096, 0, sys->objman);
    sys->subWinProc = HuPrcChildCreate(fn_1_1E98, 111, 4096, 0, sys->objman);
    sys->loadState = MD_LOAD_STATE_NONE;
    sys->loadAsyncStat = 0;
    sys->loadProcReady = FALSE;
    sys->loadDir = DATA_selmenu;
    sys->loadProc = HuPrcChildCreate(LoadProc, 106, 12288, 0, sys->objman);
    HuPrcVSleep();
}

void MdClose(void)
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

/* flow 0 - screen initialisation */
static void fn_1_2204(void)
{
    MDCAMERA_WORK *camera;
    MDMESS *mess;
    MDMODEL_WORK *model;
    MDSPRITE_WORK *sprite;
    MDSTORY_MODEL_WORK *modelWork;
    int i;
    camera = sys->camera;
    sprite = sys->sprite;
    model = sys->model;
    mess = sys->mess;
    modelWork = sys->modelWork;
    MdSpriteDispSet(sprite, 0, FALSE);
    MdSpriteDispSet(sprite, 1, FALSE);
    MdSpriteDispSet(sprite, 2, FALSE);
    MdSpriteDispSet(sprite, 3, FALSE);
    MdSpriteDispSet(sprite, 4, FALSE);
    Hu3DModelAttrReset(model->mdlId[MDMODEL_STAR], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrReset(model->mdlId[MDMODEL_STAGE], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(model->mdlId[MDMODEL_STAGE_CHR], HU3D_ATTR_DISPOFF);
    for(i=0; i<MDSTORY_CHR_MAX; i++) {
        Hu3DModelAttrSet(model->mdlId[i+MDMODEL_MARIO], HU3D_ATTR_DISPOFF);
    }
    for(i=0; i<MDSTORY_CHR_MAX; i++) {
        Hu3DModelAttrSet(model->mdlId[i+MDMODEL_CAP_CHR_00], HU3D_ATTR_DISPOFF);
    }
    for(i=0; i<MDSTORY_PACK_MAX; i++) {
        Hu3DModelAttrSet(model->mdlId[i+MDMODEL_CAP_PACK0], HU3D_ATTR_DISPOFF);
    }
    Hu3DModelAttrSet(model->mdlId[MDMODEL_CAP_LV], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(model->mdlId[MDMODEL_FINGER_1P], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(model->mdlId[MDMODEL_RING_1P], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrReset(model->mdlId[MDMODEL_FONTBGFILTER], HU3D_ATTR_DISPOFF);
    CharMotionVoiceOnSet(0, 47, FALSE);
    CharMotionVoiceOnSet(1, 6, FALSE);
    CharMotionVoiceOnSet(2, 6, FALSE);
    CharMotionVoiceOnSet(3, 6, FALSE);
    CharMotionVoiceOnSet(4, 6, FALSE);
    CharMotionVoiceOnSet(5, 6, FALSE);
    CharMotionVoiceOnSet(6, 6, FALSE);
    for(i=0; i<MDSTORY_RING_MAX; i++) {
        MdRingDispSet(i, FALSE);
    }
    sys->ringProc = HuPrcChildCreate(fn_1_1E3C, 104, 12288, 0, sys->objman);
    for(i=0; i<MDSTORY_MODELWORK_NUM; i++) {
        modelWork[i].mode = MDSTORY_MODELWORK_INIT;
    }
    HuPrcSleep(2);
    MdProcCall(MDSTORY_PROC_RESET, 0);
    HuPrcVSleep();
    sys->loadState = MD_LOAD_STATE_BOARD_LOAD;
    sys->subMode = 0;
    sys->flowMode = MDSTORY_FLOW_START;
}

/* flow 1 - title / continue menu */
static void fn_1_24FC(void)
{
    MDMODEL_WORK *model;
    MDSTORY_PLAYERSPR *playerSpr;
    MDSTORY_MODEL_WORK *modelWork;
    MDMESS *mess;
    MDSTORY_STAR *star;
    int choice;
    model = sys->model;
    mess = sys->mess;
    star = sys->star;
    playerSpr = sys->playerSpr;
    modelWork = sys->modelWork;
    switch(sys->subMode) {
        case 0:
            MdProcCall(MDSTORY_PROC_RESET, 0);
            sys->subMode = 1;
            return;

        case 1:
            WipeColorSet(255, 255, 255);
            WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, 30);
            fn_1_9384(0);
            WipeWait();
            MdProcWait(MDSTORY_PROC_FINGER, 0);
            MdProcCall(MDSTORY_PROC_RESET, 0);
            MdProcWait(MDSTORY_PROC_STAGE_MOVE, 0);
            MdProcCall(MDSTORY_PROC_RESET, 0);
            MdMessWinOpen(mess, MDMESS_WIN_MAIN, 1, 0, 0, 0, 32, 372, 512, 64, 0);
            MdMessWinOpen(mess, MDMESS_WIN_MAIN, 1, 0, 0, 0, 56, 376, 464, 64, 0);
            MdMessWinOpen(mess, MDMESS_WIN_SUB, 1, 0, 0, 1, 64, 340, 448, 32, 0);
            MdMessWinSpeedSet(mess, MDMESS_WIN_SUB, 0);
            MdMessWinAttrSet(mess, MDMESS_WIN_MAIN, HUWIN_ATTR_ALIGN_CENTER);
            MdMessWinAttrSet(mess, MDMESS_WIN_SUB, HUWIN_ATTR_ALIGN_CENTER);
            MdMessWinPad1Set(mess, MDMESS_WIN_MAIN);
            star->motNext = 2;
            MdMessWinMesSet(mess, MDMESS_WIN_MAIN, FALSE, 0xB0000, MDMESS_LIST_END);
            fn_1_9384(8);
            while(!MdMessWinCheck(mess, MDMESS_WIN_MAIN)) {
                HuPrcVSleep();
            }
        askContinue:
            if(GwCommon.storyContinue != TRUE) {
                goto flowEnd;
            }
            MdMessWinMesSet(mess, MDMESS_WIN_MAIN, TRUE, 0xB0001, MDMESS_LIST_END);
            while(!MdMessWinCheck(mess, MDMESS_WIN_MAIN)) {
                HuPrcVSleep();
            }
            choice = MdMessWinChoiceGet(mess, MDMESS_WIN_MAIN);
            if(choice != -1) {
                goto choiceDone;
            }
        askBack:
            MdMessWinMesSet(mess, MDMESS_WIN_MAIN, TRUE, 0xB0003, MDMESS_LIST_END);
            while(!MdMessWinCheck(mess, MDMESS_WIN_MAIN)) {
                HuPrcVSleep();
            }
            choice = MdMessWinChoiceGet(mess, MDMESS_WIN_MAIN);
            if(choice == -1) {
                goto askContinue;
            }
            if(choice == 1) {
                goto askContinue;
            }
            if(choice != 0) {
                goto flowEnd;
            }
            MdMessWinHomeClear(mess, MDMESS_WIN_MAIN);
            MdProcWait(MDSTORY_PROC_STAGE_MOVE, 1);
            MdProcCall(MDSTORY_PROC_RESET, 0);
            WipeColorSet(255, 255, 255);
            WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 30);
            fn_1_9384(1);
            WipeWait();
            while(sys->loadState != MD_LOAD_STATE_CHAR_WAIT) {
                HuPrcVSleep();
            }
            omOvlHisChg(0, sys->ovlHis->ovl, 0, sys->ovlHis->stat);
            sys->ovlCallMode = MD_OVLCALL_RETURN;
            sys->nextDll = DLL_mdseldll;
            sys->skipWipeF = TRUE;
            sys->exitF = TRUE;
            goto flowEnd;
        choiceDone:
            if(choice != 0) {
                goto choiceNo;
            }
            SLBoardLoadStory();
            sys->loadProcReady = TRUE;
            MdMessWinHomeClear(mess, MDMESS_WIN_MAIN);
            MdProcWait(MDSTORY_PROC_FINGER, 1);
            MdProcCall(MDSTORY_PROC_RESET, 0);
            MdProcWait(MDSTORY_PROC_STAGE_MOVE, 1);
            MdProcCall(MDSTORY_PROC_RESET, 0);
            WipeColorSet(255, 255, 255);
            WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 30);
            fn_1_9384(1);
            WipeWait();
            while(sys->loadState != MD_LOAD_STATE_DONE) {
                HuPrcVSleep();
            }
            omOvlHisChg(0, sys->ovlHis->ovl, 1, sys->ovlHis->stat);
            sys->ovlCallMode = MD_OVLCALL_CALL;
            sys->nextDll = DLL_storymodedll;
            sys->skipWipeF = TRUE;
            sys->exitF = TRUE;
            goto flowEnd;
        choiceNo:
            if(choice != 1) {
                goto flowEnd;
            }
            MdMessWinMesSet(mess, MDMESS_WIN_MAIN, FALSE, 0xB0002, MDMESS_LIST_END);
            while(!MdMessWinCheck(mess, MDMESS_WIN_MAIN)) {
                HuPrcVSleep();
            }
        flowEnd:
            sys->subMode = 2;
            return;

        case 2:
            MdMessWinMesSet(mess, MDMESS_WIN_MAIN, TRUE, 0xB0004, MDMESS_LIST_END);
            while(!MdMessWinCheck(mess, MDMESS_WIN_MAIN)) {
                HuPrcVSleep();
            }
            choice = MdMessWinChoiceGet(mess, MDMESS_WIN_MAIN);
            if(choice == -1) {
                sys->subMode = 1;
                goto askBack;
            }
            if(choice == 0) {
                MdMessWinMesSet(mess, MDMESS_WIN_MAIN, FALSE, 0xB001A, MDMESS_LIST_END);
                while(!MdMessWinCheck(mess, MDMESS_WIN_MAIN)) {
                    HuPrcVSleep();
                }
                sys->subMode = 3;
                return;
            }
            if(choice == 1) {
                sys->subMode = 3;
                return;
            }
            return;

        case 3:
            MdMessWinHomeClear(mess, MDMESS_WIN_MAIN);
            MdProcWait(MDSTORY_PROC_FINGER, 1);
            MdProcCall(MDSTORY_PROC_RESET, 0);
            star->mode = 1;
            star->motNext = 3;
            while(star->mode != 3) {
                HuPrcVSleep();
            }
            sys->subMode = 0;
            sys->flowMode = MDSTORY_FLOW_CHR_SEL;
            break;
    }
}

/* flow 2 - character selection */
static void fn_1_2D88(void)
{
    int i;
    MDMESS *mess;
    MDMODEL_WORK *model;
    int res;
    MDSTORY_STAR *star;
    MDSTORY_PLAYERSPR *playerSpr;
    MDSTORY_MODEL_WORK *modelWork;
    model = sys->model;
    mess = sys->mess;
    star = sys->star;
    playerSpr = sys->playerSpr;
    modelWork = sys->modelWork;
    switch(sys->subMode) {
        case 0:
            MdProcCall(MDSTORY_PROC_RESET, 0);
            for(i=0; i<4; i++) {
                if(sys->chrSelEnd[i] == -1) {
                    sys->selNo[i] = 0;
                } else {
                    sys->selNo[i] = sys->chrSelEnd[i];
                }
                sys->padNo[i] = 0;
            }
            sys->subMode = 1;
            return;

        case 1:
            res = MdProcCall(4, 0);
            if(res == TRUE) {
                MdProcCall(MDSTORY_PROC_RESET, 0);
                MdMessWinMesSet(mess, MDMESS_WIN_MAIN, FALSE, 0xB0005, MDMESS_LIST_END);
                MdMessWinMesSet(mess, MDMESS_WIN_SUB, FALSE, 0x20004, MDMESS_LIST_END);
                star->motNext = 2;
                sys->subMode = 2;
                return;
            }
            return;

        case 2:
            res = MdProcCall(8, 0);
            if(res == TRUE) {
                MdProcCall(MDSTORY_PROC_RESET, 0);
                sys->subMode = 3;
                return;
            }
            break;

        case 3:
            if(fn_1_8948(0) == TRUE) {
                fn_1_9384(2);
                fn_1_87F4();
                return;
            }
            if(HuPadBtnDown[0] & PAD_BUTTON_B) {
                fn_1_9384(4);
                MdMessWinMesSet(mess, MDMESS_WIN_MAIN, TRUE, 0xB0003, MDMESS_LIST_END);
                MdMessWinHomeClear(mess, MDMESS_WIN_SUB);
                sys->subMode = 4;
                return;
            }
            if(HuPadBtnDown[0] & PAD_BUTTON_A) {
                fn_1_9384(3);
                fn_1_9384(sys->selNo[0]+10);
                star->motNext = 3;
                MdMessWinHomeClear(mess, MDMESS_WIN_MAIN);
                MdMessWinHomeClear(mess, MDMESS_WIN_SUB);
                sys->padNo[0] = 1;
                Hu3DModelAttrSet(model->mdlId[MDMODEL_FINGER_1P], HU3D_ATTR_DISPOFF);
                playerSpr->mode = MDSTORY_PLAYERSPR_START;
                Hu3DMotionShiftSet(model->mdlId[sys->selNo[0]+MDMODEL_MARIO], model->animId[sys->selNo[0]*3+4], 0, 8, 0);
                modelWork[sys->selNo[0]].mode = MDSTORY_MODELWORK_MOVE;
                MdRingModeSet(0, 1);
                MdRingOfsSet(0, 0, 0, 0);
                MdRingMdlSet(0, sys->selNo[0]+10);
                MdRingScaleSet(0, 0.92);
                MdProcWait(4, 2);
                MdProcCall(MDSTORY_PROC_RESET, 0);
                sys->subMode = 0;
                sys->flowMode = MDSTORY_FLOW_STAGE_SEL;
                return;
            }
            break;

        case 4:
            res = MdMessWinCheck(mess, MDMESS_WIN_MAIN);
            if(res == TRUE) {
                res = MdMessWinChoiceGet(mess, MDMESS_WIN_MAIN);
                if(res == -1 || res == 1) {
                    MdMessWinMesSet(mess, MDMESS_WIN_MAIN, FALSE, 0xB0005, MDMESS_LIST_END);
                    MdMessWinMesSet(mess, MDMESS_WIN_SUB, FALSE, 0x20004, MDMESS_LIST_END);
                    sys->subMode = 3;
                    return;
                } else if(res == 0) {
                    star->motNext = 2;
                    Hu3DModelAttrSet(model->mdlId[MDMODEL_FINGER_1P], HU3D_ATTR_DISPOFF);
                    MdMessWinHomeClear(mess, MDMESS_WIN_MAIN);
                    MdProcWait(3, 1);
                    MdProcCall(MDSTORY_PROC_RESET, 0);
                    WipeColorSet(255, 255, 255);
                    WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 30);
                    fn_1_9384(1);
                    WipeWait();
                    while(sys->loadState != MD_LOAD_STATE_CHAR_WAIT) {
                        HuPrcVSleep();
                    }
                    omOvlHisChg(0, sys->ovlHis->ovl, 0, sys->ovlHis->stat);
                    sys->ovlCallMode = MD_OVLCALL_RETURN;
                    sys->nextDll = DLL_mdseldll;
                    sys->skipWipeF = TRUE;
                    sys->exitF = TRUE;
                    break;
                }
            }
            break;
    }
}

/* flow 3 - pack (stage) selection */
static void fn_1_3438(void)
{
    MDMODEL_WORK *model;
    MDMESS *mess;
    MDSTORY_STAR *star;
    int res;
    MDSTORY_PLAYERSPR *playerSpr;
    MDSTORY_MODEL_WORK *modelWork;
    model = sys->model;
    mess = sys->mess;
    star = sys->star;
    playerSpr = sys->playerSpr;
    modelWork = sys->modelWork;
    switch(sys->subMode) {
        case 0:
            MdProcCall(MDSTORY_PROC_RESET, 0);
            sys->selPackNo = 0;
            Hu3DAnmNoSet(model->toueiAnimId[1], sys->selPackNo);
            sys->subMode = 1;
            return;

        case 1:
            res = MdProcCall(5, 0);
            if(res == TRUE) {
                MdProcCall(MDSTORY_PROC_RESET, 0);
                sys->subMode = 2;
                return;
            }
            break;

        case 2:
            star->motNext = 2;
            MdMessWinMesSet(mess, MDMESS_WIN_MAIN, FALSE, 0xB0007, MDMESS_LIST_END);
            MdMessWinMesSet(mess, MDMESS_WIN_SUB, FALSE, 0x20004, MDMESS_LIST_END);
            MdProcWait(2, 0);
            MdProcCall(MDSTORY_PROC_RESET, 0);
            sys->subMode = 3;
            return;

        case 3:
            if((sys->pad->DStkDown[0] & PAD_BUTTON_LEFT) || (sys->pad->DStkDown[0] & PAD_BUTTON_RIGHT)) {
                if(sys->pad->DStkDown[0] & PAD_BUTTON_LEFT) {
                    if(--sys->selPackNo == -1) {
                        sys->selPackNo = 3;
                    }
                    if(!GwCommon.veryHardUnlock && sys->selPackNo == 2) {
                        sys->selPackNo = 1;
                    }
                    sys->chrPosInitF = FALSE;
                } else if(sys->pad->DStkDown[0] & PAD_BUTTON_RIGHT) {
                    sys->selPackNo = ++sys->selPackNo%4;
                    if(!GwCommon.veryHardUnlock && sys->selPackNo == 2) {
                        sys->selPackNo = 3;
                    }
                    sys->chrPosInitF = TRUE;
                }
                fn_1_9384(2);
                Hu3DAnmNoSet(model->toueiAnimId[1], sys->selPackNo);
                MdProcWait(5, 1);
                MdProcCall(MDSTORY_PROC_RESET, 0);
                return;
            }
            if(HuPadBtnDown[0] & PAD_BUTTON_B) {
                fn_1_9384(4);
                star->motNext = 3;
                MdMessWinHomeClear(mess, MDMESS_WIN_MAIN);
                MdMessWinHomeClear(mess, MDMESS_WIN_SUB);
                sys->subMode = 5;
                return;
            }
            if(HuPadBtnDown[0] & PAD_BUTTON_A) {
                fn_1_9384(3);
                star->motNext = 3;
                MdMessWinHomeClear(mess, MDMESS_WIN_MAIN);
                MdMessWinHomeClear(mess, MDMESS_WIN_SUB);
                sys->subMode = 8;
                return;
            }
            break;

        case 4:
            res = MdProcCall(5, 1);
            if(res == TRUE) {
                MdProcCall(MDSTORY_PROC_RESET, 0);
                sys->subMode = 3;
                return;
            }
            break;

        case 5:
            res = MdProcCall(2, 1);
            if(res == TRUE) {
                MdProcCall(MDSTORY_PROC_RESET, 0);
                sys->subMode = 6;
                return;
            }
            break;

        case 6:
            res = MdProcCall(5, 2);
            if(res == TRUE) {
                MdProcCall(MDSTORY_PROC_RESET, 0);
                sys->subMode = 7;
                return;
            }
            break;

        case 7:
            res = MdProcCall(4, 3);
            if(res == TRUE) {
                MdProcCall(MDSTORY_PROC_RESET, 0);
                sys->padNo[0] = 0;
                playerSpr->mode = MDSTORY_PLAYERSPR_MOVE_END;
                Hu3DMotionShiftSet(model->mdlId[sys->selNo[0]+MDMODEL_MARIO], model->animId[sys->selNo[0]*3+3], 0, 8, 0);
                modelWork[sys->selNo[0]].mode = MDSTORY_MODELWORK_MOVE_END;
                MdRingModeSet(0, 4);
                star->motNext = 2;
                MdMessWinMesSet(mess, MDMESS_WIN_MAIN, FALSE, 0xB0005, MDMESS_LIST_END);
                MdMessWinMesSet(mess, MDMESS_WIN_SUB, FALSE, 0x20004, MDMESS_LIST_END);
                sys->subMode = 2;
                sys->flowMode = MDSTORY_FLOW_CHR_SEL;
                return;
            }
            break;

        case 8:
            res = MdProcCall(2, 1);
            if(res == TRUE) {
                MdProcCall(MDSTORY_PROC_RESET, 0);
                sys->subMode = 9;
                return;
            }
            break;

        case 9:
            res = MdProcCall(5, 3);
            if(res == TRUE) {
                MdProcCall(MDSTORY_PROC_RESET, 0);
                sys->subMode = 0;
                sys->flowMode = MDSTORY_FLOW_STORY_SEL;
            }
            break;
    }
}

/* flow 4 - story selection */
static void fn_1_3C2C(void)
{
    int res;
    MDMESS *mess;
    MDSTORY_STAR *star;
    MDMODEL_WORK *model;
    int i;
    MDSTORY_PLAYERSPR *playerSpr;
    MDSTORY_MODEL_WORK *modelWork;
    model = sys->model;
    mess = sys->mess;
    star = sys->star;
    playerSpr = sys->playerSpr;
    modelWork = sys->modelWork;
    switch(sys->subMode) {
        case 0:
            MdProcCall(MDSTORY_PROC_RESET, 0);
            sys->selStoryNo = 0;
            sys->subMode = 1;
            return;

        case 1:
            res = MdProcCall(6, 0);
            if(res == TRUE) {
                MdProcCall(MDSTORY_PROC_RESET, 0);
                sys->subMode = 2;
                return;
            }
            break;

        case 2:
            star->motNext = 2;
            MdMessWinMesSet(mess, MDMESS_WIN_MAIN, FALSE, sys->selStoryNo+0xB0015, MDMESS_LIST_END);
            MdMessWinMesSet(mess, MDMESS_WIN_SUB, FALSE, 0x20004, MDMESS_LIST_END);
            MdProcWait(2, 0);
            MdProcCall(MDSTORY_PROC_RESET, 0);
            sys->subMode = 3;
            return;

        case 3:
            if((sys->pad->DStkDown[0] & PAD_BUTTON_LEFT) || (sys->pad->DStkDown[0] & PAD_BUTTON_RIGHT)) {
                if(sys->pad->DStkDown[0] & PAD_BUTTON_LEFT) {
                    if(--sys->selStoryNo == -1) {
                        sys->selStoryNo = 4;
                    }
                    sys->chrPosInitF = FALSE;
                } else if(sys->pad->DStkDown[0] & PAD_BUTTON_RIGHT) {
                    sys->selStoryNo = ++sys->selStoryNo%5;
                    sys->chrPosInitF = TRUE;
                }
                fn_1_9384(2);
                MdMessWinMesSet(mess, MDMESS_WIN_MAIN, FALSE, sys->selStoryNo+0xB0015, MDMESS_LIST_END);
                for(i=0; i<5; i++) {
                    if(i == sys->selStoryNo) {
                        Hu3DModelAttrReset(model->mdlId[i+MDMODEL_CAP_PACK0], HU3D_ATTR_DISPOFF);
                    } else {
                        Hu3DModelAttrSet(model->mdlId[i+MDMODEL_CAP_PACK0], HU3D_ATTR_DISPOFF);
                    }
                }
                sys->subMode = 4;
                return;
            }
            if(HuPadBtnDown[0] & PAD_BUTTON_B) {
                fn_1_9384(4);
                star->motNext = 3;
                MdMessWinHomeClear(mess, MDMESS_WIN_MAIN);
                MdMessWinHomeClear(mess, MDMESS_WIN_SUB);
                sys->subMode = 5;
                return;
            }
            if(HuPadBtnDown[0] & PAD_BUTTON_A) {
                fn_1_9384(3);
                star->motNext = 3;
                MdMessWinHomeClear(mess, MDMESS_WIN_MAIN);
                MdMessWinHomeClear(mess, MDMESS_WIN_SUB);
                sys->subMode = 8;
                return;
            }
            break;

        case 4:
            res = MdProcCall(6, 1);
            if(res == TRUE) {
                MdProcCall(MDSTORY_PROC_RESET, 0);
                sys->subMode = 3;
                return;
            }
            break;

        case 5:
            res = MdProcCall(2, 1);
            if(res == TRUE) {
                MdProcCall(MDSTORY_PROC_RESET, 0);
                sys->subMode = 6;
                return;
            }
            break;

        case 6:
            res = MdProcCall(6, 2);
            if(res == TRUE) {
                MdProcCall(MDSTORY_PROC_RESET, 0);
                sys->subMode = 7;
                return;
            }
            break;

        case 7:
            res = MdProcCall(5, 4);
            if(res == TRUE) {
                MdProcCall(MDSTORY_PROC_RESET, 0);
                sys->subMode = 2;
                sys->flowMode = MDSTORY_FLOW_STAGE_SEL;
                return;
            }
            break;

        case 8:
            res = MdProcCall(2, 1);
            if(res == TRUE) {
                MdProcCall(MDSTORY_PROC_RESET, 0);
                sys->subMode = 9;
                return;
            }
            break;

        case 9:
            res = MdProcCall(6, 3);
            if(res == TRUE) {
                MdProcCall(MDSTORY_PROC_RESET, 0);
                star->motNext = 2;
                MdMessWinMesSet(mess, MDMESS_WIN_MAIN, TRUE, 0xB0008, MDMESS_LIST_END);
                fn_1_9384(9);
                sys->subMode = 10;
                return;
            }
            break;

        case 10:
            res = MdMessWinCheck(mess, MDMESS_WIN_MAIN);
            if(res == TRUE) {
                res = MdMessWinChoiceGet(mess, MDMESS_WIN_MAIN);
                if(res == -1 || res == 1) {
                    MdMessWinMesSet(mess, MDMESS_WIN_MAIN, TRUE, 0xB0009, MDMESS_LIST_END);
                    sys->subMode = 11;
                    return;
                } else if(res == 0) {
                    MdProcCall(MDSTORY_PROC_RESET, 0);
                    fn_1_8C84();
                    sys->loadProcReady = TRUE;
                    MdMessWinMesSet(mess, MDMESS_WIN_MAIN, FALSE, 0xB000A, MDMESS_LIST_END);
                    while(!MdMessWinCheck(mess, MDMESS_WIN_MAIN)) {
                        HuPrcVSleep();
                    }
                    star->motNext = 3;
                    MdMessWinHomeClear(mess, MDMESS_WIN_MAIN);
                    WipeColorSet(255, 255, 255);
                    WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 60);
                    fn_1_9384(1);
                    fn_1_9384(7);
                    while(WipeCheckEnd()) {
                        MdProcCall(4, 4);
                        HuPrcVSleep();
                    }
                    while(sys->loadState != MD_LOAD_STATE_DONE) {
                        HuPrcVSleep();
                    }
                    omOvlHisChg(0, sys->ovlHis->ovl, 1, sys->ovlHis->stat);
                    sys->ovlCallMode = MD_OVLCALL_CALL;
                    sys->nextDll = DLL_storymodedll;
                    sys->skipWipeF = TRUE;
                    sys->exitF = TRUE;
                    return;
                }
            }
            break;

        case 11:
            res = MdMessWinCheck(mess, MDMESS_WIN_MAIN);
            if(res == TRUE) {
                res = MdMessWinChoiceGet(mess, MDMESS_WIN_MAIN);
                if(res == -1 || res == 1) {
                    star->motNext = 3;
                    MdMessWinHomeClear(mess, MDMESS_WIN_MAIN);
                    MdProcWait(6, 4);
                    MdProcCall(MDSTORY_PROC_RESET, 0);
                    sys->subMode = 2;
                    return;
                } else if(res == 0) {
                    star->motNext = 3;
                    MdMessWinHomeClear(mess, MDMESS_WIN_MAIN);
                    sys->padNo[0] = 0;
                    playerSpr->mode = MDSTORY_PLAYERSPR_MOVE_END;
                    Hu3DMotionShiftSet(model->mdlId[sys->selNo[0]+MDMODEL_MARIO], model->animId[sys->selNo[0]*3+3], 0, 8, 0);
                    modelWork[sys->selNo[0]].mode = MDSTORY_MODELWORK_MOVE_END;
                    MdRingModeSet(0, 4);
                    Hu3DModelAttrSet(model->mdlId[MDMODEL_RING_1P], HU3D_ATTR_DISPOFF);
                    MdProcWait(7, 0);
                    MdProcCall(MDSTORY_PROC_RESET, 0);
                    sys->subMode = 1;
                    sys->flowMode = MDSTORY_FLOW_CHR_SEL;
                    break;
                }
            }
            break;
    }
}

/* proc 0 - reset */
static BOOL fn_1_46E4(int param)
{
    sys->procInitF[0] = 0;
    sys->procInitF[1] = 0;
    sys->procInitF[2] = 0;
    sys->procInitF[3] = 0;
    sys->procInitF[4] = 0;
    sys->procInitF[5] = 0;
    sys->oscTime[0] = 0;
    sys->oscTime[1] = 0;
    sys->oscTime[2] = 0;
    sys->oscTime[3] = 0;
    sys->oscTime[4] = 0;
    sys->oscTime[5] = 0;
    sys->oscTime[6] = 0;
    sys->oscT[0] = 0;
    sys->oscT[1] = 0;
    sys->oscT[2] = 0;
    sys->oscT[3] = 0;
    sys->oscT[4] = 0;
    sys->oscT[5] = 0;
    sys->oscT[6] = 0;
    return FALSE;
}

/* proc 1 - camera lerp */
static BOOL fn_1_48EC(int param)
{
    MDCAMERA *camera = &sys->camera->camera[MDCAMERA_MAIN];
    MDSTORY_CAMLERP *lerp = &lbl_1_data_544[param];
    MdMathOscillate(lerp->oscType, &sys->oscTime[0], &sys->oscT[0], lerp->time);
    MdMathOscillate(MDMATH_OSC_180, &sys->oscTime[1], &sys->oscT[1], lerp->time);
    MdMathLerp(&camera->hvr.x, lerp->hvrFrom.x, lerp->hvrTo.x, sys->oscT[0]);
    MdMathLerp(&camera->hvr.y, lerp->hvrFrom.y, lerp->hvrTo.y, sys->oscT[0]);
    MdMathLerp(&camera->hvr.z, lerp->hvrFrom.z, lerp->hvrTo.z, sys->oscT[0]);
    MdMathLerp(&camera->center.x, lerp->centerFrom.x, lerp->centerTo.x, sys->oscT[0]);
    MdMathLerp(&camera->center.y, lerp->centerFrom.y, lerp->centerTo.y, sys->oscT[0]);
    MdMathLerp(&camera->center.z, lerp->centerFrom.z, lerp->centerTo.z, sys->oscT[0]);
    if(sys->oscTime[0] >= 1) {
        return TRUE;
    }
    return FALSE;
}

/* proc 2 - window frame slide */
static BOOL fn_1_4A98(int param)
{
    MDSPRITE_WORK *sprite = sys->sprite;
    HUSPRGRPID grpId = sprite->grpId[2];
    HuVecF pos3D;
    HuVecF pos2D;
    HuVec2f *pos;
    HuVec2f *scale;
    if(sys->procInitF[2] == FALSE) {
        pos3D.x = 0;
        pos3D.y = 145;
        pos3D.z = 400;
        Hu3D3Dto2D(&pos3D, sys->camera->camera[MDCAMERA_MAIN].bit, &pos2D);
        lbl_1_bss_A0[0].x = -64;
        lbl_1_bss_A0[0].y = pos2D.y;
        lbl_1_bss_A0[0].z = 0;
        lbl_1_bss_A0[1].x = pos2D.x-80;
        lbl_1_bss_A0[1].y = pos2D.y;
        lbl_1_bss_A0[1].z = 0;
        lbl_1_bss_A0[2].x = 640;
        lbl_1_bss_A0[2].y = pos2D.y;
        lbl_1_bss_A0[2].z = 0;
        lbl_1_bss_A0[3].x = 80+pos2D.x;
        lbl_1_bss_A0[3].y = pos2D.y;
        lbl_1_bss_A0[3].z = 0;
        switch(param) {
            case 0:
                HuSprGrpPosSet(grpId, 0, 0);
                pos = MdSpritePosGet(sprite, 2, 0);
                pos->x = lbl_1_bss_A0[0].x;
                pos->y = lbl_1_bss_A0[0].y;
                HuSprDispOn(grpId, 0);
                pos = MdSpritePosGet(sprite, 2, 1);
                pos->x = lbl_1_bss_A0[2].x;
                pos->y = lbl_1_bss_A0[2].y;
                HuSprDispOn(grpId, 1);
                break;

            case 2:
                pos = MdSpritePosGet(sprite, 2, 4);
                pos->x = lbl_1_bss_A0[1].x;
                pos->y = lbl_1_bss_A0[1].y;
                HuSprTPLvlSet(grpId, 4, 0);
                HuSprDispOn(grpId, 4);
                break;

            case 3:
                pos = MdSpritePosGet(sprite, 2, 5);
                pos->x = lbl_1_bss_A0[3].x;
                pos->y = lbl_1_bss_A0[3].y;
                HuSprTPLvlSet(grpId, 5, 0);
                HuSprDispOn(grpId, 5);
                break;
        }
        sys->procInitF[2] = TRUE;
    }
    switch(param) {
        case 0:
            MdMathOscillate(MDMATH_OSC_90, &sys->oscTime[4], &sys->oscT[4], 15);
            MdMathLerp(&MdSpritePosGet(sprite, 2, 0)->x, lbl_1_bss_A0[0].x, lbl_1_bss_A0[1].x, sys->oscT[4]);
            MdMathLerp(&MdSpritePosGet(sprite, 2, 1)->x, lbl_1_bss_A0[2].x, lbl_1_bss_A0[3].x, sys->oscT[4]);
            break;

        case 1:
            MdMathOscillate(MDMATH_OSC_90, &sys->oscTime[4], &sys->oscT[4], 15);
            MdMathLerp(&MdSpritePosGet(sprite, 2, 0)->x, lbl_1_bss_A0[1].x, lbl_1_bss_A0[0].x, sys->oscT[4]);
            MdMathLerp(&MdSpritePosGet(sprite, 2, 1)->x, lbl_1_bss_A0[3].x, lbl_1_bss_A0[2].x, sys->oscT[4]);
            break;

        case 2:
            MdMathOscillate(MDMATH_OSC_180, &sys->oscTime[4], &sys->oscT[4], 15);
            pos = MdSpritePosGet(sprite, 2, 0);
            scale = MdSpriteScaleGet(sprite, 2, 0);
            MdMathLerp(&pos->x, lbl_1_bss_A0[1].x, lbl_1_bss_A0[1].x-24, sys->oscT[4]);
            MdMathLerp(&scale->x, 1, 1.5f, sys->oscT[4]);
            MdMathLerp(&scale->y, 1, 1.5f, sys->oscT[4]);
            pos = MdSpritePosGet(sprite, 2, 4);
            scale = MdSpriteScaleGet(sprite, 2, 4);
            MdMathLerp(&pos->x, lbl_1_bss_A0[1].x, lbl_1_bss_A0[1].x-24, sys->oscT[4]);
            MdMathLerp(&scale->x, 1, 1.5f, sys->oscT[4]);
            MdMathLerp(&scale->y, 1, 1.5f, sys->oscT[4]);
            HuSprTPLvlSet(grpId, 4, sys->oscT[4]);
            break;

        case 3:
            MdMathOscillate(MDMATH_OSC_180, &sys->oscTime[4], &sys->oscT[4], 15);
            pos = MdSpritePosGet(sprite, 2, 1);
            scale = MdSpriteScaleGet(sprite, 2, 1);
            MdMathLerp(&pos->x, lbl_1_bss_A0[3].x, 24+lbl_1_bss_A0[3].x, sys->oscT[4]);
            MdMathLerp(&scale->x, 1, 1.5f, sys->oscT[4]);
            MdMathLerp(&scale->y, 1, 1.5f, sys->oscT[4]);
            pos = MdSpritePosGet(sprite, 2, 5);
            scale = MdSpriteScaleGet(sprite, 2, 5);
            MdMathLerp(&pos->x, lbl_1_bss_A0[3].x, 24+lbl_1_bss_A0[3].x, sys->oscT[4]);
            MdMathLerp(&scale->x, 1, 1.5f, sys->oscT[4]);
            MdMathLerp(&scale->y, 1, 1.5f, sys->oscT[4]);
            HuSprTPLvlSet(grpId, 5, sys->oscT[4]);
            break;
    }
    if(sys->oscTime[4] >= 1) {
        switch(param) {
            case 1:
                MdSpriteDispSet(sprite, 2, FALSE);
                break;

            case 2:
                HuSprDispOff(grpId, 4);
                break;

            case 3:
                HuSprDispOff(grpId, 5);
                break;
        }
        return TRUE;
    }
    return FALSE;
}

/* proc 3 - top/bottom bars */
static BOOL fn_1_53D4(int param)
{
    MDSPRITE_WORK *sprite = sys->sprite;
    HUSPRGRPID grpId = sprite->grpId[0];
    HuVec2f *pos0 = MdSpritePosGet(sprite, 0, 0);
    HuVec2f *pos1 = MdSpritePosGet(sprite, 0, 1);
    HuVec2f *pos2 = MdSpritePosGet(sprite, 0, 2);
    HuVec2f *scale0 = MdSpriteScaleGet(sprite, 0, 0);
    HuVec2f *scale1 = MdSpriteScaleGet(sprite, 0, 1);
    HuVec2f *scale2 = MdSpriteScaleGet(sprite, 0, 2);
    if(sys->procInitF[3] == FALSE) {
        lbl_1_bss_9C = 0;
        lbl_1_bss_98 = pos0->y;
        lbl_1_bss_94 = pos1->y;
        lbl_1_bss_90 = scale2->y;
        if(param == 0) {
            pos0->y = 0;
            pos1->y = 0;
            scale0->x = 0;
            scale1->x = 0;
            scale2->x = 0;
            scale2->y = 0;
            MdSpriteDispSet(sprite, 0, TRUE);
        }
        sys->procInitF[3] = TRUE;
    }
    if(param == 0) {
        if(lbl_1_bss_9C == 0) {
            MdMathOscillate(MDMATH_OSC_90, &sys->oscTime[5], &sys->oscT[5], 12);
            MdMathLerp(&pos0->y, 0, -2, sys->oscT[5]);
            MdMathLerp(&pos1->y, 0, 2, sys->oscT[5]);
            MdMathLerp(&scale0->x, 0, 72, sys->oscT[5]);
            MdMathLerp(&scale1->x, 0, 72, sys->oscT[5]);
            MdMathLerp(&scale2->x, 0, 1, sys->oscT[5]);
            if(sys->oscTime[5] >= 1) {
                sys->oscTime[5] = 0;
                lbl_1_bss_9C = 1;
            }
        } else if(lbl_1_bss_9C == 1) {
            MdMathOscillate(MDMATH_OSC_90, &sys->oscTime[5], &sys->oscT[5], 15);
            MdMathLerp(&pos0->y, -2, -28, sys->oscT[5]);
            MdMathLerp(&pos1->y, 2, 32, sys->oscT[5]);
            MdMathLerp(&scale2->y, 0, 8, sys->oscT[5]);
            if(sys->oscTime[5] >= 1) {
                lbl_1_bss_9C = 2;
            }
        }
    } else if(param == 1) {
        if(lbl_1_bss_9C == 0) {
            MdMathOscillate(MDMATH_OSC_90, &sys->oscTime[5], &sys->oscT[5], 15);
            MdMathLerp(&pos0->y, lbl_1_bss_98, -2, sys->oscT[5]);
            MdMathLerp(&pos1->y, lbl_1_bss_94, 2, sys->oscT[5]);
            MdMathLerp(&scale2->y, lbl_1_bss_90, 0, sys->oscT[5]);
            if(sys->oscTime[5] >= 1) {
                sys->oscTime[5] = 0;
                lbl_1_bss_9C = 1;
            }
        } else if(lbl_1_bss_9C == 1) {
            MdMathOscillate(MDMATH_OSC_90, &sys->oscTime[5], &sys->oscT[5], 12);
            MdMathLerp(&pos0->y, -2, 0, sys->oscT[5]);
            MdMathLerp(&pos1->y, 2, 0, sys->oscT[5]);
            MdMathLerp(&scale0->x, 72, 0, sys->oscT[5]);
            MdMathLerp(&scale1->x, 72, 0, sys->oscT[5]);
            MdMathLerp(&scale2->x, 1, 0, sys->oscT[5]);
            if(sys->oscTime[5] >= 1) {
                lbl_1_bss_9C = 2;
            }
        }
    }
    if(lbl_1_bss_9C == 2) {
        if(param == 1) {
            MdSpriteDispSet(sprite, 0, FALSE);
        }
        return TRUE;
    }
    return FALSE;
}

/* proc 4 - character / selection effect */
static BOOL fn_1_5B08(int param)
{
    MDCAMERA_WORK *cameraWork = sys->camera;
    MDMODEL_WORK *model = sys->model;
    MDSPRITE_WORK *sprite = sys->sprite;
    MDSTORY_STAR *star = sys->star;
    HU3DPARMANID parManId;
    ANIMDATA *dotAnim;
    ANIMDATA *glowAnim;
    MDDOT_WORK *dotWork;
    MDDOT_WORK *glowWork;
    MDDOT_WORK *effect;
    s16 cameraBit;
    HuVecF *pos;
    HuVecF *scale;
    HuVecF *dstPos;
    int i;
    int randVal;
    if(sys->procInitF[0] == FALSE) {
        switch(param) {
            case 0:
                for(i=0; i<MDSTORY_CHR_MAX; i++) {
                    scale = model->scaleP[i+MDMODEL_CAP_CHR_00];
                    *model->posP[i+MDMODEL_CAP_CHR_00] = lbl_1_data_57C;
                    scale->x = 0;
                    scale->y = 0;
                    scale->z = 0;
                    Hu3DModelAttrReset(model->mdlId[i+MDMODEL_CAP_CHR_00], HU3D_ATTR_DISPOFF);
                    Hu3DModelAttrReset(model->mdlId[i+MDMODEL_MARIO], HU3D_ATTR_DISPOFF);
                }
                Hu3DModelAttrReset(model->mdlId[MDMODEL_STAGE_CHR], HU3D_ATTR_DISPOFF);
                Hu3DLayerHookSet(1+HU3D_LAYER_HOOK_POST, MdToueiLayerHook);
                fn_1_9384(6);
                break;

            case 2:
                star->mode = 7;
                fn_1_9384(6);
                break;

            case 3:
                star->mode = 10;
                fn_1_9384(6);
                break;

            case 4:
                cameraBit = cameraWork->camera[MDCAMERA_MAIN].bit;
                dotAnim = sprite->animP[7];
                dotWork = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDDOT_WORK), HU_MEMNUM_OVL);
                parManId = Hu3DParManCreate(dotAnim, 100, &param1);
                dotWork->man[0] = parManId;
                Hu3DParManAttrSet(parManId, 0x65);
                Hu3DParticleBlendModeSet(Hu3DParManModelIDGet(parManId), 1);
                Hu3DParManRotSet(parManId, 90, 0, 0);
                Hu3DModelLayerSet(Hu3DParManModelIDGet(parManId), 4);
                Hu3DModelCameraSet(Hu3DParManModelIDGet(parManId), cameraBit);
                parManId = Hu3DParManCreate(dotAnim, 100, &param2);
                dotWork->man[1] = parManId;
                Hu3DParManAttrSet(parManId, 0x163);
                Hu3DParticleBlendModeSet(Hu3DParManModelIDGet(parManId), 1);
                randVal = frandmod(80)-40;
                Hu3DParManRotSet(parManId, frandmod(40)+70, 0, randVal);
                Hu3DModelLayerSet(Hu3DParManModelIDGet(parManId), 4);
                Hu3DModelCameraSet(Hu3DParManModelIDGet(parManId), cameraBit);
                parManId = Hu3DParManCreate(dotAnim, 100, &param3);
                dotWork->man[2] = parManId;
                Hu3DParManAttrSet(parManId, 0x21);
                Hu3DParticleBlendModeSet(Hu3DParManModelIDGet(parManId), 1);
                Hu3DParManRotSet(parManId, 0, 0, 0);
                Hu3DModelLayerSet(Hu3DParManModelIDGet(parManId), 4);
                Hu3DModelCameraSet(Hu3DParManModelIDGet(parManId), cameraBit);
                lbl_1_bss_88[0] = dotWork;
                cameraBit = cameraWork->camera[MDCAMERA_MAIN].bit;
                glowAnim = sprite->animP[8];
                glowWork = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDDOT_WORK), HU_MEMNUM_OVL);
                parManId = Hu3DParManCreate(glowAnim, 100, &glowParam);
                glowWork->man[0] = parManId;
                Hu3DParManAttrSet(parManId, 0x65);
                Hu3DParticleBlendModeSet(Hu3DParManModelIDGet(parManId), 1);
                Hu3DParManRotSet(parManId, 90, 0, 0);
                Hu3DModelLayerSet(Hu3DParManModelIDGet(parManId), 4);
                Hu3DModelCameraSet(Hu3DParManModelIDGet(parManId), cameraBit);
                lbl_1_bss_88[1] = glowWork;
                pos = model->posP[sys->selNo[0]+MDMODEL_CAP_CHR_00];
                effect = lbl_1_bss_88[0];
                Hu3DParManPosSet(effect->man[0], pos->x, pos->y, pos->z);
                Hu3DParManTimeLimitSet(effect->man[0], 2);
                Hu3DParManAttrReset(effect->man[0], 1);
                Hu3DParManPosSet(effect->man[1], pos->x, pos->y, pos->z);
                Hu3DParManTimeLimitSet(effect->man[1], 2);
                Hu3DParManAttrReset(effect->man[1], 1);
                Hu3DParManPosSet(effect->man[2], pos->x, pos->y, pos->z);
                Hu3DParManTimeLimitSet(effect->man[2], 5);
                Hu3DParManAttrReset(effect->man[2], 1);
                for(i=0; i<MDSTORY_CHR_MAX; i++) {
                    Hu3DModelAttrSet(model->mdlId[i+MDMODEL_CAP_CHR_00], HU3D_ATTR_DISPOFF);
                    Hu3DModelAttrSet(model->mdlId[i+MDMODEL_MARIO], HU3D_ATTR_DISPOFF);
                }
                Hu3DModelAttrSet(model->mdlId[MDMODEL_STAGE_CHR], HU3D_ATTR_DISPOFF);
                Hu3DModelAttrSet(model->mdlId[MDMODEL_RING_1P], HU3D_ATTR_DISPOFF);
                Hu3DModelAttrReset(model->mdlId[sys->selNo[0]+MDMODEL_MARIO], HU3D_ATTR_DISPOFF);
                Hu3DLayerHookSet(1+HU3D_LAYER_HOOK_POST, NULL);
                Hu3DModelLayerSet(model->mdlId[sys->selNo[0]+MDMODEL_MARIO], 1);
                Hu3DModelCameraSet(model->mdlId[sys->selNo[0]+MDMODEL_MARIO], cameraWork->camera[MDCAMERA_MAIN].bit);
                lbl_1_bss_70[1].x = 352;
                lbl_1_bss_70[1].y = 224;
                lbl_1_bss_70[1].z = 1500;
                Hu3D2Dto3D(&lbl_1_bss_70[1], cameraWork->camera[MDCAMERA_MAIN].bit, &lbl_1_bss_58[1]);
                *model->posP[sys->selNo[0]+MDMODEL_MARIO] = *model->posP[sys->selNo[0]+MDMODEL_CAP_CHR_00];
                dstPos = model->posP[sys->selNo[0]+MDMODEL_MARIO];
                dstPos->y -= 200;
                lbl_1_bss_58[0] = *model->posP[sys->selNo[0]+MDMODEL_MARIO];
                model->rotP[sys->selNo[0]+MDMODEL_MARIO]->x = 0;
                model->rotP[sys->selNo[0]+MDMODEL_MARIO]->y = 0;
                model->rotP[sys->selNo[0]+MDMODEL_MARIO]->z = 0;
                model->scaleP[sys->selNo[0]+MDMODEL_MARIO]->x = 2;
                model->scaleP[sys->selNo[0]+MDMODEL_MARIO]->y = 2;
                model->scaleP[sys->selNo[0]+MDMODEL_MARIO]->z = 2;
                HuSprDispOff(sprite->grpId[3], 0);
                Hu3DMotionSet(model->mdlId[sys->selNo[0]+MDMODEL_MARIO], model->animId[sys->selNo[0]*3+5]);
                Hu3DModelAttrReset(model->mdlId[sys->selNo[0]+MDMODEL_MARIO], HU3D_MOTATTR_LOOP);
                Hu3DModelAttrReset(model->mdlId[sys->selNo[0]+MDMODEL_MARIO], HU3D_MOTATTR_PAUSE);
                fn_1_9384(5);
                break;
        }
        sys->procInitF[0] = TRUE;
    }
    switch(param) {
        case 0:
            MdMathOscillate(MDMATH_OSC_90, &sys->oscTime[0], &sys->oscT[0], 30);
            for(i=0; i<MDSTORY_CHR_MAX; i++) {
                pos = model->posP[i+MDMODEL_CAP_CHR_00];
                scale = model->scaleP[i+MDMODEL_CAP_CHR_00];
                MdMathLerp(&pos->x, lbl_1_data_57C.x, lbl_1_data_588[i].x, sys->oscT[0]);
                MdMathLerp(&pos->y, lbl_1_data_57C.y, lbl_1_data_588[i].y, sys->oscT[0]);
                MdMathLerp(&pos->z, lbl_1_data_57C.z, lbl_1_data_588[i].z, sys->oscT[0]);
                MdMathLerp(&scale->x, 0, 1, sys->oscT[0]);
                MdMathLerp(&scale->y, 0, 1, sys->oscT[0]);
                MdMathLerp(&scale->z, 0, 0.1f, sys->oscT[0]);
            }
            break;

        case 1:
            MdMathOscillate(MDMATH_OSC_90_REV, &sys->oscTime[0], &sys->oscT[0], 15);
            for(i=0; i<MDSTORY_CHR_MAX; i++) {
                scale = model->scaleP[i+MDMODEL_CAP_CHR_00];
                MdMathLerp(&scale->x, 1, 0, sys->oscT[0]);
                MdMathLerp(&scale->y, 1, 0, sys->oscT[0]);
                MdMathLerp(&scale->z, 0.1f, 0, sys->oscT[0]);
            }
            break;

        case 2:
            MdMathOscillate(MDMATH_OSC_90_REV, &sys->oscTime[0], &sys->oscT[0], 30);
            MdMathOscillate(MDMATH_OSC_180, &sys->oscTime[1], &sys->oscT[1], 30);
            for(i=0; i<1; i++) {
                pos = model->posP[sys->selNo[i]+MDMODEL_CAP_CHR_00];
                MdMathLerp(&pos->x, lbl_1_data_588[sys->selNo[i]].x, lbl_1_data_5DC[i].x, sys->oscT[0]);
                MdMathLerp(&pos->y, lbl_1_data_588[sys->selNo[i]].y, lbl_1_data_5DC[i].y, sys->oscT[0]);
                MdMathLerp(&pos->z, lbl_1_data_588[sys->selNo[i]].z, lbl_1_data_5DC[i].z, sys->oscT[1]);
            }
            for(i=0; i<MDSTORY_CHR_MAX; i++) {
                scale = model->scaleP[i+MDMODEL_CAP_CHR_00];
                if(i != sys->selNo[0]) {
                    MdMathLerp(&scale->x, 1, 0, sys->oscT[0]);
                    MdMathLerp(&scale->y, 1, 0, sys->oscT[0]);
                    MdMathLerp(&scale->z, 0.1f, 0, sys->oscT[0]);
                }
            }
            fn_1_87F4();
            break;

        case 3:
            MdMathOscillate(MDMATH_OSC_90_REV, &sys->oscTime[0], &sys->oscT[0], 30);
            MdMathOscillate(MDMATH_OSC_180, &sys->oscTime[1], &sys->oscT[1], 30);
            for(i=0; i<1; i++) {
                pos = model->posP[sys->selNo[i]+MDMODEL_CAP_CHR_00];
                MdMathLerp(&pos->x, lbl_1_data_5DC[i].x, lbl_1_data_588[sys->selNo[i]].x, sys->oscT[0]);
                MdMathLerp(&pos->y, lbl_1_data_5DC[i].y, lbl_1_data_588[sys->selNo[i]].y, sys->oscT[0]);
                MdMathLerp(&pos->z, lbl_1_data_588[sys->selNo[i]].z, lbl_1_data_5DC[i].z, sys->oscT[1]);
            }
            for(i=0; i<MDSTORY_CHR_MAX; i++) {
                scale = model->scaleP[i+MDMODEL_CAP_CHR_00];
                if(i != sys->selNo[0]) {
                    MdMathLerp(&scale->x, 0, 1, sys->oscT[0]);
                    MdMathLerp(&scale->y, 0, 1, sys->oscT[0]);
                    MdMathLerp(&scale->z, 0, 0.1f, sys->oscT[0]);
                }
            }
            fn_1_87F4();
            break;

        case 4:
            dstPos = model->posP[sys->selNo[0]+MDMODEL_MARIO];
            MdMathOscillate(MDMATH_OSC_90_REV, &sys->oscTime[0], &sys->oscT[0], 60);
            MdMathOscillate(MDMATH_OSC_180, &sys->oscTime[1], &sys->oscT[1], 60);
            MdMathOscillate(MDMATH_OSC_180, &sys->oscTime[2], &sys->oscT[2], 60);
            MdMathLerp(&dstPos->x, lbl_1_bss_58[0].x, lbl_1_bss_58[1].x, sys->oscT[0]);
            MdMathLerp(&dstPos->y, lbl_1_bss_58[0].y, lbl_1_bss_58[1].y, sys->oscT[2]);
            MdMathLerpOfs(&dstPos->z, lbl_1_bss_58[0].z, lbl_1_bss_58[1].z, sys->oscT[0], 0.2f*sys->oscT[1]);
            pos = model->posP[sys->selNo[0]+MDMODEL_MARIO];
            glowWork = lbl_1_bss_88[1];
            Hu3DParManPosSet(glowWork->man[0], pos->x, pos->y, pos->z);
            Hu3DParManTimeLimitSet(glowWork->man[0], 2);
            Hu3DParManAttrReset(glowWork->man[0], 1);
            MdProcCall(3, 1);
            break;
    }
    if(sys->oscTime[0] >= 1) {
        return TRUE;
    }
    return FALSE;
}

/* proc 5 - level cap model */
static BOOL fn_1_709C(int param)
{
    MDCAMERA_WORK *cameraWork = sys->camera;
    MDMODEL_WORK *model = sys->model;
    HuVecF *pos = model->posP[MDMODEL_CAP_LV];
    HuVecF *rot = model->rotP[MDMODEL_CAP_LV];
    HuVecF *scale = model->scaleP[MDMODEL_CAP_LV];
    if(sys->procInitF[0] == FALSE) {
        switch(param) {
            case 1:
            case 2:
                break;

            case 0:
                pos->x = 0;
                pos->y = 145;
                pos->z = 400;
                scale->x = 0;
                scale->y = 0;
                scale->z = 0;
                Hu3DModelAttrReset(model->mdlId[MDMODEL_CAP_LV], HU3D_ATTR_DISPOFF);
                break;

            case 3:
                lbl_1_bss_40[0] = *pos;
                lbl_1_bss_40[1].x = 525;
                lbl_1_bss_40[1].y = 365;
                lbl_1_bss_40[1].z = -50;
                fn_1_9384(6);
                break;

            case 4:
                fn_1_9384(6);
                break;
        }
        sys->procInitF[0] = TRUE;
    }
    switch(param) {
        case 0:
            MdMathOscillate(MDMATH_OSC_90_REV, &sys->oscTime[0], &sys->oscT[0], 15);
            MdMathLerpScale(scale, 0, 1, sys->oscT[0]);
            break;

        case 1:
            MdMathOscillate(MDMATH_OSC_90_REV, &sys->oscTime[0], &sys->oscT[0], 15);
            MdProcCall(2, sys->chrPosInitF+2);
            break;

        case 2:
            MdMathOscillate(MDMATH_OSC_90, &sys->oscTime[0], &sys->oscT[0], 15);
            MdMathLerpScale(scale, 1, 0, sys->oscT[0]);
            break;

        case 3:
            MdMathOscillate(MDMATH_OSC_90_REV, &sys->oscTime[0], &sys->oscT[0], 15);
            MdMathOscillate(MDMATH_OSC_180, &sys->oscTime[1], &sys->oscT[1], 15);
            MdMathLerp(&pos->x, lbl_1_bss_40[0].x, lbl_1_bss_40[1].x, sys->oscT[0]);
            MdMathLerp(&pos->y, lbl_1_bss_40[0].y, lbl_1_bss_40[1].y, sys->oscT[0]);
            MdMathLerpOfs(&pos->z, lbl_1_bss_40[0].z, lbl_1_bss_40[1].z, sys->oscT[0], -1*sys->oscT[1]);
            MdMathLerpScale(scale, 1, 0.8f, sys->oscT[0]);
            break;

        case 4:
            MdMathOscillate(MDMATH_OSC_90, &sys->oscTime[0], &sys->oscT[0], 15);
            MdMathOscillate(MDMATH_OSC_180, &sys->oscTime[1], &sys->oscT[1], 15);
            MdMathLerp(&pos->x, lbl_1_bss_40[1].x, lbl_1_bss_40[0].x, sys->oscT[0]);
            MdMathLerp(&pos->y, lbl_1_bss_40[1].y, lbl_1_bss_40[0].y, sys->oscT[0]);
            MdMathLerpOfs(&pos->z, lbl_1_bss_40[1].z, lbl_1_bss_40[0].z, sys->oscT[0], sys->oscT[1]);
            MdMathLerpScale(scale, 0.8f, 1, sys->oscT[0]);
            break;
    }
    fn_1_8B94(MDMODEL_CAP_LV, 1);
    if(sys->oscTime[0] >= 1) {
        return TRUE;
    }
    return FALSE;
}

/* proc 6 - pack caps */
static BOOL fn_1_766C(int param)
{
    MDCAMERA_WORK *cameraWork = sys->camera;
    MDMODEL_WORK *model = sys->model;
    HuVecF *posP[MDSTORY_PACK_MAX];
    HuVecF *rotP[MDSTORY_PACK_MAX];
    HuVecF *scaleP[MDSTORY_PACK_MAX];
    int i;
    for(i=0; i<MDSTORY_PACK_MAX; i++) {
        posP[i] = model->posP[i+MDMODEL_CAP_PACK0];
        rotP[i] = model->rotP[i+MDMODEL_CAP_PACK0];
        scaleP[i] = model->scaleP[i+MDMODEL_CAP_PACK0];
    }
    if(sys->procInitF[0] == FALSE) {
        switch(param) {
            case 1:
            case 2:
                break;

            case 0:
                for(i=0; i<MDSTORY_PACK_MAX; i++) {
                    posP[i]->x = 0;
                    posP[i]->y = 145;
                    posP[i]->z = 400;
                    scaleP[i]->x = 0;
                    scaleP[i]->y = 0;
                    scaleP[i]->z = 0;
                }
                for(i=0; i<MDSTORY_PACK_MAX; i++) {
                    if(i == sys->selStoryNo) {
                        Hu3DModelAttrReset(model->mdlId[i+MDMODEL_CAP_PACK0], HU3D_ATTR_DISPOFF);
                    } else {
                        Hu3DModelAttrSet(model->mdlId[i+MDMODEL_CAP_PACK0], HU3D_ATTR_DISPOFF);
                    }
                }
                break;

            case 3:
                lbl_1_bss_28[0] = *posP[sys->selStoryNo];
                lbl_1_bss_28[1].x = -525;
                lbl_1_bss_28[1].y = 365;
                lbl_1_bss_28[1].z = -50;
                fn_1_9384(6);
                break;

            case 4:
                fn_1_9384(6);
                break;
        }
        sys->procInitF[0] = TRUE;
    }
    switch(param) {
        case 0:
            MdMathOscillate(MDMATH_OSC_90_REV, &sys->oscTime[0], &sys->oscT[0], 15);
            for(i=0; i<MDSTORY_PACK_MAX; i++) {
                MdMathLerpScale(scaleP[i], 0, 1, sys->oscT[0]);
            }
            break;

        case 1:
            MdMathOscillate(MDMATH_OSC_90_REV, &sys->oscTime[0], &sys->oscT[0], 15);
            MdProcCall(2, sys->chrPosInitF+2);
            break;

        case 2:
            MdMathOscillate(MDMATH_OSC_90, &sys->oscTime[0], &sys->oscT[0], 15);
            for(i=0; i<MDSTORY_PACK_MAX; i++) {
                MdMathLerpScale(scaleP[i], 1, 0, sys->oscT[0]);
            }
            break;

        case 3:
            MdMathOscillate(MDMATH_OSC_90_REV, &sys->oscTime[0], &sys->oscT[0], 15);
            MdMathOscillate(MDMATH_OSC_180, &sys->oscTime[1], &sys->oscT[1], 15);
            MdMathLerp(&posP[0]->x, lbl_1_bss_28[0].x, lbl_1_bss_28[1].x, sys->oscT[0]);
            MdMathLerp(&posP[0]->y, lbl_1_bss_28[0].y, lbl_1_bss_28[1].y, sys->oscT[0]);
            MdMathLerpOfs(&posP[0]->z, lbl_1_bss_28[0].z, lbl_1_bss_28[1].z, sys->oscT[0], -1*sys->oscT[1]);
            *posP[1] = *posP[0];
            *posP[2] = *posP[0];
            *posP[3] = *posP[0];
            *posP[4] = *posP[0];
            MdMathLerpScale(scaleP[0], 1, 0.8f, sys->oscT[0]);
            *scaleP[1] = *scaleP[0];
            *scaleP[2] = *scaleP[0];
            *scaleP[3] = *scaleP[0];
            *scaleP[4] = *scaleP[0];
            break;

        case 4:
            MdMathOscillate(MDMATH_OSC_90, &sys->oscTime[0], &sys->oscT[0], 15);
            MdMathOscillate(MDMATH_OSC_180, &sys->oscTime[1], &sys->oscT[1], 15);
            MdMathLerp(&posP[0]->x, lbl_1_bss_28[1].x, lbl_1_bss_28[0].x, sys->oscT[0]);
            MdMathLerp(&posP[0]->y, lbl_1_bss_28[1].y, lbl_1_bss_28[0].y, sys->oscT[0]);
            MdMathLerpOfs(&posP[0]->z, lbl_1_bss_28[1].z, lbl_1_bss_28[0].z, sys->oscT[0], sys->oscT[1]);
            *posP[1] = *posP[0];
            *posP[2] = *posP[0];
            *posP[3] = *posP[0];
            *posP[4] = *posP[0];
            MdMathLerpScale(scaleP[0], 0.8f, 1, sys->oscT[0]);
            *scaleP[1] = *scaleP[0];
            *scaleP[2] = *scaleP[0];
            *scaleP[3] = *scaleP[0];
            *scaleP[4] = *scaleP[0];
            break;
    }
    for(i=0; i<MDSTORY_PACK_MAX; i++) {
        fn_1_8B94(i+MDMODEL_CAP_PACK0, 1);
    }
    if(sys->oscTime[0] >= 1) {
        return TRUE;
    }
    return FALSE;
}

/* proc 7 - character cap shrink */
static BOOL fn_1_7FB4(int param)
{
    MDMODEL_WORK *model = sys->model;
    HuVecF *scale;
    int i;
    if(sys->procInitF[0] == FALSE) {
        sys->star->mode = 10;
        sys->procInitF[0] = TRUE;
    }
    if(param == 0) {
        MdMathOscillate(MDMATH_OSC_90, &sys->oscTime[0], &sys->oscT[0], 30);
        scale = model->scaleP[sys->selNo[0]+MDMODEL_CAP_CHR_00];
        MdMathLerp(&scale->x, 1, 0, sys->oscT[0]);
        MdMathLerp(&scale->y, 1, 0, sys->oscT[0]);
        MdMathLerp(&scale->z, 0.1f, 0, sys->oscT[0]);
        MdMathLerpScale(model->scaleP[MDMODEL_CAP_LV], 0.8f, 0, sys->oscT[0]);
        for(i=0; i<MDSTORY_PACK_MAX; i++) {
            MdMathLerpScale(model->scaleP[i+MDMODEL_CAP_PACK0], 0.8f, 0, sys->oscT[0]);
        }
    }
    if(sys->oscTime[0] >= 1) {
        return TRUE;
    }
    return FALSE;
}

/* proc 8 - finger model */
static void fn_1_81F8(int param)
{
    MDCAMERA_WORK *cameraWork = sys->camera;
    MDMODEL_WORK *model = sys->model;
    HuVecF *pos = model->posP[MDMODEL_FINGER_1P];
    HuVecF *rot = model->rotP[MDMODEL_FINGER_1P];
    HuVecF *scale = model->scaleP[MDMODEL_FINGER_1P];
    if(sys->procInitF[0] == FALSE) {
        if(param == 0) {
            scale->x = 0;
            scale->y = 0;
            scale->z = 0;
            Hu3DModelAttrReset(model->mdlId[MDMODEL_FINGER_1P], HU3D_ATTR_DISPOFF);
            fn_1_87F4();
        }
        sys->procInitF[0] = TRUE;
    }
    if(param == 0) {
        MdMathOscillate(MDMATH_OSC_90_REV, &sys->oscTime[0], &sys->oscT[0], 15);
        MdMathLerpScale(scale, 0, 4, sys->oscT[0]);
    }
    if(param == 0) {
        if(sys->oscTime[0] >= 1) {
        }
    }
}

/* proc 9 - cursor sprite */
static BOOL fn_1_8388(int param)
{
    MDSPRITE_WORK *sprite = sys->sprite;
    HUSPRGRPID grpId = sprite->grpId[4];
    HuVec2f *pos;
    if(sys->procInitF[4] == FALSE) {
        if(param == 0) {
            HuSprGrpPosSet(grpId, 0, 0);
            HuSprPosSet(grpId, 0, lbl_1_data_60C[1].x, lbl_1_data_60C[1].y);
            HuSprDispOn(grpId, 0);
        }
        sys->procInitF[4] = TRUE;
    }
    if(param == 0) {
        MdMathOscillate(MDMATH_OSC_90, &sys->oscTime[6], &sys->oscT[6], 15);
        pos = MdSpritePosGet(sprite, 4, 0);
        MdMathLerp(&pos->x, lbl_1_data_60C[0].x, lbl_1_data_60C[1].x, sys->oscT[6]);
        MdMathLerp(&pos->y, lbl_1_data_60C[0].y, lbl_1_data_60C[1].y, sys->oscT[6]);
    } else if(param == 1) {
        MdMathOscillate(MDMATH_OSC_90_REV, &sys->oscTime[6], &sys->oscT[6], 15);
        pos = MdSpritePosGet(sprite, 4, 0);
        MdMathLerp(&pos->x, lbl_1_data_60C[1].x, lbl_1_data_60C[0].x, sys->oscT[6]);
        MdMathLerp(&pos->y, lbl_1_data_60C[1].y, lbl_1_data_60C[0].y, sys->oscT[6]);
    }
    if(sys->oscTime[6] >= 1) {
        return TRUE;
    }
    return FALSE;
}

static void fn_1_8610(void)
{
    s8 padTbl[4] = { 0, -1, -1, -1 };
    int i;
    s8 j;
    sys->padConnectNum = 0;
    for(i=0; i<4; i++) {
        sys->chrSelNo[i] = -1;
    }
    for(j=0; j<4; j++) {
        if(padTbl[j] == 0) {
            sys->chrSelNo[sys->padConnectNum] = j;
            sys->chrSel[j] = TRUE;
            sys->padConnectNum++;
        } else {
            sys->chrSel[j] = FALSE;
        }
    }
}

static void fn_1_872C(void)
{
    MDMODEL_WORK *model;
    HUSPRGRPID grpId;
    int unused;
    u8 colorTbl[4] = { 255, 255, 255, 255 };
    int i;
    model = sys->model;
    grpId = sys->sprite->grpId[3];
    unused = 0;
    for(i=0; i<4; i++) {
        if(i == 0) {
            sys->chrSelEnd[i] = 0;
        } else {
            sys->chrSelEnd[i] = -1;
        }
    }
}

static void fn_1_87F4(void)
{
    MDCAMERA_WORK *cameraWork;
    MDMODEL_WORK *model;
    HuVecF *pos;
    HuVecF *selPos;
    HUSPRGRPID grpId;
    HuVecF pos3D;
    HuVecF pos2D;
    cameraWork = sys->camera;
    model = sys->model;
    pos = model->posP[MDMODEL_FINGER_1P];
    selPos = model->posP[sys->selNo[0]+MDMODEL_CAP_CHR_00];
    grpId = sys->sprite->grpId[3];
    HuSprGrpPosSet(grpId, 0, 0);
    *pos = *selPos;
    pos->x -= 60;
    pos->y += 105;
    pos->z += 30;
    pos3D.x = selPos->x;
    pos3D.y = selPos->y;
    pos3D.z = selPos->z;
    Hu3D3Dto2D(&pos3D, cameraWork->camera[MDCAMERA_MAIN].bit, &pos2D);
    HuSprPosSet(grpId, 0, pos2D.x, pos2D.y-18);
}

static BOOL fn_1_8948(int padNo)
{
    int dir;
    int i;
    if(sys->padNo[padNo] == 1) {
        return FALSE;
    }
    if((sys->pad->DStkDown[0] & PAD_BUTTON_LEFT) || (sys->pad->DStkDown[0] & PAD_BUTTON_RIGHT) || (sys->pad->DStkDown[0] & PAD_BUTTON_UP) || (sys->pad->DStkDown[0] & PAD_BUTTON_DOWN)) {
        if(sys->pad->DStkDown[0] & PAD_BUTTON_LEFT) {
            dir = 0;
        } else if(sys->pad->DStkDown[0] & PAD_BUTTON_RIGHT) {
            dir = 1;
        } else if(sys->pad->DStkDown[0] & PAD_BUTTON_UP) {
            dir = 2;
        } else if(sys->pad->DStkDown[0] & PAD_BUTTON_DOWN) {
            dir = 3;
        }
        for(i=0; i<7; i++) {
            if(sys->selNo[padNo] == lbl_1_data_624[dir][i]) {
                sys->selNo[padNo] = lbl_1_data_624[dir][(i+1)%7];
                break;
            }
        }
        return TRUE;
    }
    return FALSE;
}

static void fn_1_8B94(int mdlNo, int cameraNo)
{
    MDCAMERA *camera;
    MDMODEL_WORK *model;
    HuVecF *pos;
    HuVecF *rot;
    float rotY;
    HuVecF dir;
    camera = &sys->camera->camera[cameraNo];
    model = sys->model;
    pos = model->posP[mdlNo];
    rot = model->rotP[mdlNo];
    dir.x = camera->pos.x-pos->x;
    dir.y = camera->pos.y-pos->y;
    dir.z = camera->pos.z-pos->z;
    rotY = HuAtan(dir.x, dir.z);
    rot->y = rotY;
}

/* sets up the story-mode game work (difficulty, players, boards) from the menu choices */
static void fn_1_8C84(void)
{
    int used[6] = { 0, 0, 0, 0, 0, 0 };
    int i;
    int j;
    s8 playerNo;
    s8 boardNo;
    if(sys->selStoryNo == 0) {
        GwSystem.mgPack = 0;
    } else if(sys->selStoryNo == 1) {
        GwSystem.mgPack = 1;
    } else if(sys->selStoryNo == 2) {
        GwSystem.mgPack = 2;
    } else if(sys->selStoryNo == 3) {
        GwSystem.mgPack = 3;
    } else if(sys->selStoryNo == 4) {
        GwSystem.mgPack = 4;
    }
    GwCommon.storyMgPack = GwSystem.mgPack;
    GwSystem.boardNo = 0;
    GwSystem.turnNo = 1;
    GwSystem.bonusStarF = TRUE;
    GwSystem.teamF = FALSE;
    if(sys->selPackNo == 0) {
        GwPlayer[0].dif = GW_DIF_NORMAL;
        for(i=1; i<4; i++) {
            GwPlayer[i].dif = GW_DIF_NORMAL;
        }
    } else if(sys->selPackNo == 1) {
        GwPlayer[0].dif = GW_DIF_HARD;
        for(i=1; i<4; i++) {
            GwPlayer[i].dif = GW_DIF_HARD;
        }
    } else if(sys->selPackNo == 2) {
        GwPlayer[0].dif = GW_DIF_VERYHARD;
        for(i=1; i<4; i++) {
            GwPlayer[i].dif = GW_DIF_VERYHARD;
        }
    } else if(sys->selPackNo == 3) {
        GwPlayer[0].dif = GW_DIF_EASY;
        for(i=1; i<4; i++) {
            GwPlayer[i].dif = GW_DIF_EASY;
        }
    }
    GwPlayer[0].charNo = sys->selNo[0];
    GwPlayer[1].charNo = 10;
    GwPlayer[2].charNo = 11;
    GwPlayer[3].charNo = 12;
    GwPlayer[0].comF = FALSE;
    GwPlayer[1].comF = TRUE;
    GwPlayer[2].comF = TRUE;
    GwPlayer[3].comF = TRUE;
    for(playerNo=0; playerNo<4; playerNo++) {
        GwPlayer[playerNo].padNo = playerNo;
        GwPlayer[playerNo].grp = playerNo;
        GwPlayer[playerNo].handicap = 0;
        GwPlayerConf[playerNo].charNo = GwPlayer[playerNo].charNo;
        GwPlayerConf[playerNo].padNo = GwPlayer[playerNo].padNo;
        GwPlayerConf[playerNo].dif = GwPlayer[playerNo].dif;
        GwPlayerConf[playerNo].type = GwPlayer[playerNo].comF;
        GwPlayerConf[playerNo].grp = GwPlayer[playerNo].grp;
    }
    for(i=0; i<4; i++) {
        boardNo = frandmod(6);
        if(used[boardNo] == 0) {
            used[boardNo] = 1;
            GwCommon.storyBoardNo[i] = boardNo;
        } else if(used[boardNo] == 1) {
            for(j=0; j<5; j++) {
                boardNo = (boardNo+1)%6;
                if(used[boardNo] == 0) {
                    used[boardNo] = 1;
                    GwCommon.storyBoardNo[i] = boardNo;
                    break;
                }
            }
        }
    }
    GwCommon.storyBoardNo[4] = 6;
    GwCommon.storyNo = -2;
}

/* audio command dispatcher */
static void fn_1_9384(int cmd)
{
    s16 pan;
    switch(cmd) {
        case 0:
            lbl_1_bss_24 = HuAudBGMPlay(4);
            break;

        case 1:
            HuAudSStreamFadeOut(lbl_1_bss_24, 1000);
            break;

        case 2:
        case 3:
        case 4:
        case 6:
            if(cmd == 2) {
                HuAudFXPlay(0);
            } else if(cmd == 3) {
                HuAudFXPlay(2);
            } else if(cmd == 4) {
                HuAudFXPlay(3);
            } else if(cmd == 6) {
                HuAudFXPlay(993);
            }
            break;

        case 5:
            pan = fn_1_9524(sys->selNo[0]+MDMODEL_MARIO, 1);
            if(cmd == 5) {
                lbl_1_bss_20 = HuAudFXPlayPan(994, pan);
            }
            break;

        case 8:
        case 9:
            pan = fn_1_9524(0, 2);
            if(cmd == 8) {
                HuAudFXPlayPan(753, pan);
            } else if(cmd == 9) {
                HuAudFXPlayPan(753, pan);
            }
            break;

        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
            HuAudFXPlayPan(lbl_1_data_640[cmd-10], fn_1_9524(cmd, 1));
            break;

        case 17:
            HuAudAllStop();
            break;
    }
}

static s16 fn_1_9524(int mdlNo, int cameraNo)
{
    MDCAMERA *camera;
    MDMODEL_WORK *model;
    MDCAMERA_WORK *cameraWork;
    HuVecF *pos3D;
    s16 result;
    HuVecF pos2D;
    cameraWork = sys->camera;
    camera = &cameraWork->camera[cameraNo];
    model = sys->model;
    pos3D = model->posP[mdlNo];
    Hu3D3Dto2D(pos3D, camera->bit, &pos2D);
    result = MSM_PAN_LEFT+((MSM_PAN_WIDTH/HU_DISP_WIDTH)*pos2D.x);
    return result;
}
