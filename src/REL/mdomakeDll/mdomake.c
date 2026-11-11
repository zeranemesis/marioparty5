#include "REL/mdomakeDll.h"
#include "game/wipe.h"
#include "game/thpmain.h"
#include "game/mgdata.h"
#include "game/hsfex.h"
#include "game/frand.h"
#include "game/charman.h"
#include "game/armem.h"
#include "humath.h"
#include "datadir_enum.h"

#include "REL/mdomakeDll/effect.c"
#include "messnum/syshelp.h"

#define MDOMAKE_AUD_MUSSTART 0
#define MDOMAKE_AUD_MUSEND 1
#define MDOMAKE_AUD_SECMN_MOVE 2
#define MDOMAKE_AUD_SECMN_ACCEPT 3
#define MDOMAKE_AUD_SECMN_CANCEL 4
#define MDOMAKE_AUD_SE_ENTER 5
#define MDOMAKE_AUD_SE_2 6
#define MDOMAKE_AUD_SE_CAP_SPEAK 7
#define MDOMAKE_AUD_FX_STOP 8

#define MDOMAKE_STAR_NONE 0
#define MDOMAKE_STAR_RESET 1
#define MDOMAKE_STAR_ENTER 2
#define MDOMAKE_STAR_ENTER_IDLE 3
#define MDOMAKE_STAR_EXIT_START 4
#define MDOMAKE_STAR_EXIT 5
#define MDOMAKE_STAR_EXIT_IDLE 6
#define MDOMAKE_STAR_HIDE 7
#define MDOMAKE_STAR_STOP 8

#define MDOMAKE_MODE_MAX 3

#define MDOMAKE_MES(n) MESSNUM(MESS_EXTRA, (n))

#define MdOmakeReset() MdProcCall(MDOMAKE_PROC_RESET, 0)
#define MdOmakeCursorShow() MdProcWait(MDOMAKE_PROC_CURSOR_UPDATE, 0)
#define MdOmakeWinOpen() MdProcWait(MDOMAKE_PROC_WIN_UPDATE, 0)
#define MdOmakeWinClose() MdProcWait(MDOMAKE_PROC_WIN_UPDATE, 1)

typedef struct MdOmakeCamLerp_s {
    HuVecF hvrStart; //0x00
    HuVecF centerStart; //0x0C
    HuVecF hvrEnd; //0x18
    HuVecF centerEnd; //0x24
    int type; //0x30
    float time; //0x34
} MDOMAKE_CAMLERP;

typedef struct MdOmakeCap_s {
    s16 mode; //0x00
    s16 delay; //0x02
    float timeA; //0x04
    float timeB; //0x08
    float tA; //0x0C
    float tB; //0x10
    float radius; //0x14
    float yaw; //0x18
    float yawBase; //0x1C
    float radiusY; //0x20
    float pitch; //0x24
    float pitchBase; //0x28
    float scale; //0x2C
    HuVecF start; //0x30
    HuVecF end; //0x3C
} MDOMAKE_CAP;

void fn_1_5868(int cmd);
int fn_1_5988(int mdlNo, int cameraNo);
void fn_1_5778(int mdlNo, int cameraNo);

void fn_1_938(void);
void fn_1_1078(void);
void fn_1_1314(void);
void fn_1_1358(void);
void fn_1_1700(void);
void fn_1_192C(void);
BOOL fn_1_28BC(int param);
BOOL fn_1_2A78(int param);
BOOL fn_1_2BF8(int param);
BOOL fn_1_34F0(int param);
BOOL fn_1_3C24(int param);
BOOL fn_1_4F34(int param);
BOOL fn_1_54F0(int param);

char *lbl_1_data_144 = "26";

OMOVL lbl_1_data_148[3] = {
    DLL_mdcarddll,
    DLL_mdbeachdll,
    DLL_mdbeachdll
};

unsigned int lbl_1_data_154[3] = {
    DATA_mdcard,
    DATA_mdbeach,
    DATA_mdbeach
};

char *lbl_1_data_198[3] = {
    "movie/ys_cp_0.thp",
    "movie/ys578_0.thp",
    "movie/ys580_0.thp"
};

MD_FUNC MdFlowTbl[MDOMAKE_FLOW_MAX] = {
    fn_1_1700,
    fn_1_192C
};

MD_PROC MdProcTbl[MDOMAKE_PROC_MAX] = {
    fn_1_28BC,
    fn_1_2A78,
    fn_1_2BF8,
    fn_1_34F0,
    fn_1_3C24,
    fn_1_4F34,
    fn_1_54F0
};

s16 lbl_1_data_314[4] = { 1, 77, 79, 0 };

MDOMAKE_CAMLERP lbl_1_data_31C[1] = {
    {
        { 90, 0, 2000 },
        { 0, 0, 0 },
        { 90, 0, 1200 },
        { 0, 50, 0 },
        2,
        60
    }
};

float lbl_1_data_354[3] = { 90, 210, -30 };
float lbl_1_data_360[3] = { -90, -210, 30 };

HuVecF lbl_1_data_36C[3] = {
    { 288, 200, 1200 },
    { -64, -32, 4000 },
    { 640, -32, 4000 }
};

s16 lbl_1_data_390[4] = { 8, 0, 4, 0 };
float lbl_1_data_398[3] = { 270, 90, 180 };
float lbl_1_data_3A4[3] = { 630, -270, -180 };
HuVecF lbl_1_data_3B0[2] = {
    { 288, -128, 0 },
    { 288, 64, 0 }
};

int lbl_1_bss_20;
HuVecF lbl_1_bss_24[2];
HuVecF lbl_1_bss_3C[2];
HuVecF lbl_1_bss_54[2];
s16 lbl_1_bss_6C;
HuVecF lbl_1_bss_70[3];
HuVecF lbl_1_bss_94;
MDOMAKE_CAP lbl_1_bss_A0[3];
int lbl_1_bss_178;
float lbl_1_bss_17C;
float lbl_1_bss_180;
float lbl_1_bss_184;
int lbl_1_bss_188;
HuVecF lbl_1_bss_18C[4];
float lbl_1_bss_1BC;
float lbl_1_bss_1C0;
float lbl_1_bss_1C4;
float lbl_1_bss_1C8;
HuVecF lbl_1_bss_1CC[2];
HuVecF lbl_1_bss_1E4[2];
MDOMAKE_EFFECT *lbl_1_bss_1FC;

SYS *sysP;
SYS *sys;
SYS sysWork;

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

extern void ObjectSetup(void);

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
    sys = &sysWork;
    sysP = sys;
    sys->subMode = 0;
    sys->unk4 = 0;
    sys->flowTime = 0;
    sys->flowMode = MDOMAKE_FLOW_INIT;
    sys->ovlCallMode = MD_OVLCALL_RETURN;
    sys->nextDll = DLL_selmenuDLL;
    sys->exitF = FALSE;
    sys->skipWipeF = FALSE;
    sys->objman = omInitObjMan(512, 8192);
    sys->ovlHis = omOvlHisGet(0);
    sys->evtNo = sys->ovlHis->evtno;
    if(sys->evtNo == 2) {
        HuARDirFree(DATA_m433);
    } else if(sys->evtNo == 3) {
        HuARDirFree(DATA_m580);
    }
    CharDataClose(-1);
    omGameSysInit(sys->objman);
    sys->obj[MDOMAKE_OBJ_DEBUGFONT] = omAddObj(sys->objman, 100, 0, 0, MdObjDebugFontCreate);
    sys->obj[MDOMAKE_OBJ_DEBUGFONT]->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDDEBUGFONT_WORK), HU_MEMNUM_OVL);
    if(!sys->obj[MDOMAKE_OBJ_DEBUGFONT]->data) {
        OSReport("sys->obj[MDOMAKE_OBJ_DEBUGFONT]->data = NULL...\n");
    }
    sys->debugFont = sys->obj[MDOMAKE_OBJ_DEBUGFONT]->data;
    sys->debugFont->createF = FALSE;
    sys->obj[MDOMAKE_OBJ_PAD] = omAddObj(sys->objman, 101, 0, 0, MdObjPadMain);
    sys->obj[MDOMAKE_OBJ_PAD]->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDPAD_WORK), HU_MEMNUM_OVL);
    if(!sys->obj[MDOMAKE_OBJ_PAD]->data) {
        OSReport("sys->obj[MDOMAKE_OBJ_PAD]->data = NULL...\n");
    }
    sys->pad = sys->obj[MDOMAKE_OBJ_PAD]->data;
    sys->obj[MDOMAKE_OBJ_CAMERA] = omAddObj(sys->objman, 102, 0, 0, MdObjCameraCreate);
    sys->obj[MDOMAKE_OBJ_CAMERA]->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDCAMERA_WORK), HU_MEMNUM_OVL);
    if(!sys->obj[MDOMAKE_OBJ_CAMERA]->data) {
        OSReport("sys->obj[MDOMAKE_OBJ_CAMERA]->data = NULL...\n");
    }
    sys->camera = sys->obj[MDOMAKE_OBJ_CAMERA]->data;
    sys->camera->createF = FALSE;
    sys->camera->debugFont = sys->debugFont;
    sys->obj[MDOMAKE_OBJ_LIGHT] = omAddObj(sys->objman, 103, 0, 0, MdObjLightCreate);
    sys->obj[MDOMAKE_OBJ_LIGHT]->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDLIGHT_WORK), HU_MEMNUM_OVL);
    if(!sys->obj[MDOMAKE_OBJ_LIGHT]->data) {
        OSReport("sys->obj[MDOMAKE_OBJ_LIGHT]->data = NULL...\n");
    }
    sys->light = sys->obj[MDOMAKE_OBJ_LIGHT]->data;
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
    sys->star = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDOMAKE_STAR), HU_MEMNUM_OVL);
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
    sys->mainProc = HuPrcChildCreate(MdMain, 104, 12288, 0, sys->objman);
}

void MdMain(void)
{
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
    OSTick tick;
    while(1) {
        switch(sys->loadState) {
            case MD_LOAD_STATE_NONE:
                break;

            case MD_LOAD_STATE_LOADING:
                tick = OSGetTick();
                sys->loadAsyncStat = HuDataDirReadAsync(sys->loadDir);
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

void fn_1_938(void)
{
    MDOMAKE_STAR *star = sys->star;
    MDMODEL_WORK *model = sys->model;
    MDCAMERA_WORK *camera = sys->camera;
    HuVecF *pos = model->posP[MDMODEL_STAR];
    int i;
    star->mode = MDOMAKE_STAR_NONE;
    star->motNext = 1;
    lbl_1_bss_1E4[0].x = 288;
    lbl_1_bss_1E4[0].y = 240;
    lbl_1_bss_1E4[0].z = 750;
    Hu3D2Dto3D(&lbl_1_bss_1E4[0], camera->camera[MDCAMERA_GUIDE].bit, &lbl_1_bss_1CC[0]);
    *pos = lbl_1_bss_1CC[0];
    while(1) {
        model = sys->model;
        camera = sys->camera;
        star = sys->star;
        pos = model->posP[MDMODEL_STAR];
        switch(star->mode) {
            case MDOMAKE_STAR_RESET:
                lbl_1_bss_1C8 = 0;
                lbl_1_bss_1C4 = 0;
                lbl_1_bss_1C0 = 0;
                lbl_1_bss_1BC = 0;
                lbl_1_bss_1E4[0].x = 288;
                lbl_1_bss_1E4[0].y = 240;
                lbl_1_bss_1E4[0].z = 750;
                lbl_1_bss_1E4[1].x = 64;
                lbl_1_bss_1E4[1].y = 300;
                lbl_1_bss_1E4[1].z = 1000;
                for(i=0; i<2; i++) {
                    Hu3D2Dto3D(&lbl_1_bss_1E4[i], camera->camera[MDCAMERA_GUIDE].bit, &lbl_1_bss_1CC[i]);
                }
                star->mode = MDOMAKE_STAR_ENTER;
                break;

            case MDOMAKE_STAR_ENTER:
                MdMathOscillate(MDMATH_OSC_90, &lbl_1_bss_1C8, &lbl_1_bss_1C0, 30);
                MdMathOscillate(MDMATH_OSC_180, &lbl_1_bss_1C4, &lbl_1_bss_1BC, 30);
                MdMathLerp(&pos->x, lbl_1_bss_1CC[0].x, lbl_1_bss_1CC[1].x, lbl_1_bss_1C0);
                MdMathLerp(&pos->y, lbl_1_bss_1CC[0].y, lbl_1_bss_1CC[1].y, lbl_1_bss_1C0);
                MdMathLerpOfs(&pos->z, lbl_1_bss_1CC[0].z, lbl_1_bss_1CC[1].z, lbl_1_bss_1C0, -1.0f*lbl_1_bss_1BC);
                if(lbl_1_bss_1C8 >= 1) {
                    star->mode = MDOMAKE_STAR_ENTER_IDLE;
                }
                break;

            case MDOMAKE_STAR_EXIT_START:
                lbl_1_bss_1E4[1].x = 64;
                lbl_1_bss_1E4[1].y = 300;
                lbl_1_bss_1E4[1].z = 1000;
                Hu3D2Dto3D(&lbl_1_bss_1E4[1], camera->camera[MDCAMERA_GUIDE].bit, &lbl_1_bss_1CC[1]);
                pos->x = lbl_1_bss_1CC[1].x;
                pos->y = lbl_1_bss_1CC[1].y;
                pos->z = lbl_1_bss_1CC[1].z;
                star->mode = MDOMAKE_STAR_ENTER_IDLE;
                break;

            case MDOMAKE_STAR_EXIT:
                lbl_1_bss_1C8 = 0;
                lbl_1_bss_1C4 = 0;
                lbl_1_bss_1C0 = 0;
                lbl_1_bss_1BC = 0;
                lbl_1_bss_1E4[0].x = 64;
                lbl_1_bss_1E4[0].y = 320;
                lbl_1_bss_1E4[0].z = 1000;
                lbl_1_bss_1E4[1].x = 288;
                lbl_1_bss_1E4[1].y = 240;
                lbl_1_bss_1E4[1].z = 1000;
                for(i=0; i<2; i++) {
                    Hu3D2Dto3D(&lbl_1_bss_1E4[i], camera->camera[MDCAMERA_GUIDE].bit, &lbl_1_bss_1CC[i]);
                }
                star->mode = MDOMAKE_STAR_EXIT_IDLE;
                break;

            case MDOMAKE_STAR_EXIT_IDLE:
                MdMathOscillate(MDMATH_OSC_90, &lbl_1_bss_1C8, &lbl_1_bss_1C0, 30);
                MdMathLerp(&pos->x, lbl_1_bss_1CC[0].x, lbl_1_bss_1CC[1].x, lbl_1_bss_1C0);
                MdMathLerp(&pos->y, lbl_1_bss_1CC[0].y, lbl_1_bss_1CC[1].y, lbl_1_bss_1C0);
                MdMathLerp(&pos->z, lbl_1_bss_1CC[0].z, lbl_1_bss_1CC[1].z, lbl_1_bss_1C0);
                if(lbl_1_bss_1C8 >= 1) {
                    star->mode = MDOMAKE_STAR_HIDE;
                }
                break;
        }
        switch(star->motNext) {
            case 0:
                break;

            case 1:
                Hu3DModelAttrSet(model->mdlId[MDMODEL_STAR], HU3D_MOTATTR_LOOP);
                Hu3DModelAttrReset(model->mdlId[MDMODEL_STAR], HU3D_MOTATTR_PAUSE);
                Hu3DMotionSet(model->mdlId[MDMODEL_STAR], model->motId[0]);
                star->motNext = 0;
                break;

            case 2:
                Hu3DMotionShiftSet(model->mdlId[MDMODEL_STAR], model->motId[1], 0, 32, HU3D_MOTATTR_LOOP);
                star->motNext = 0;
                break;

            case 3:
                Hu3DMotionShiftSet(model->mdlId[MDMODEL_STAR], model->motId[0], 0, 32, HU3D_MOTATTR_LOOP);
                star->motNext = 0;
                break;
        }
        if(star->mode != MDOMAKE_STAR_STOP) {
            fn_1_5778(0, 1);
        }
        HuPrcVSleep();
    }
}

void fn_1_1078(void)
{
    MDMODEL_WORK *model;
    sys->thpState = 0;
    while(1) {
        model = sys->model;
        switch(sys->thpState) {
            case 1:
                if(sys->selMode == 0) {
                    HuTHPSprCreateVol(lbl_1_data_198[0], 1, 100, 0);
                } else {
                    HuTHPSprCreateVol(mgDataTbl[lbl_1_data_314[sys->selMode]].movie[0], 1, 100, 0);
                }
                while(HuTHPStartCheck()) {
                    HuPrcVSleep();
                }
                Hu3DModelMatHookSet(model->mdlId[MDMODEL_CAP_START+sys->selMode], HuTHPMatHook);
                sys->thpState = 0;
                break;

            case 2:
                HuTHPClose();
                while(HuTHPProcCheck()) {
                    HuPrcVSleep();
                }
                Hu3DModelMatHookSet(model->mdlId[MDMODEL_CAP_START+sys->prevSelMode], NULL);
                if(sys->selMode == 0) {
                    HuTHPSprCreateVol(lbl_1_data_198[0], 1, 100, 0);
                } else {
                    HuTHPSprCreateVol(mgDataTbl[lbl_1_data_314[sys->selMode]].movie[0], 1, 100, 0);
                }
                while(HuTHPStartCheck()) {
                    HuPrcVSleep();
                }
                Hu3DModelMatHookSet(model->mdlId[MDMODEL_CAP_START+sys->selMode], HuTHPMatHook);
                sys->thpState = 0;
                break;
        }
        HuPrcVSleep();
    }
}

void fn_1_1314(void)
{
    MDMESS *mess = sys->mess;
    while(1) {
        mess = sys->mess;
        MdMessMain(mess, 0);
        HuPrcVSleep();
    }
}

void fn_1_1358(void)
{
    MDMESS *mess = sys->mess;
    while(1) {
        mess = sys->mess;
        MdMessMain(mess, 1);
        HuPrcVSleep();
    }
}

void MdInit(void)
{
    MdSpriteCreate(sys->sprite);
    HuPrcVSleep();
    MdModelCreate(sys->model, sys->camera);
    HuPrcVSleep();
    HuDataDirClose(DATA_mdomake);
    HuPrcVSleep();
    MdToueiMdlInit();
    sys->starProc = HuPrcChildCreate(fn_1_938, 106, 12288, 0, sys->objman);
    sys->thpProc = HuPrcChildCreate(fn_1_1078, 107, 4096, 0, sys->objman);
    sys->mainWinProc = HuPrcChildCreate(fn_1_1314, 108, 4096, 0, sys->objman);
    sys->helpWinProc = HuPrcChildCreate(fn_1_1358, 109, 4096, 0, sys->objman);
    sys->loadState = MD_LOAD_STATE_NONE;
    sys->loadAsyncStat = 0;
    sys->loadDir = DATA_mdsel;
    sys->loadProc = HuPrcChildCreate(LoadProc, 105, 12288, 0, sys->objman);
    HuPrcVSleep();
}

void MdClose(void)
{
    if(!sys->skipWipeF) {
        WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 60);
        WipeWait();
    }
    HuTHPClose();
    while(HuTHPProcCheck()) {
        HuPrcVSleep();
    }
    HuAudAllStop();
    MdMessKill(sys->mess);
    if(sys->ovlCallMode == MD_OVLCALL_GOTO) {
        omOvlGotoEx(sys->nextDll, TRUE, 0, 0);
    } else if(sys->ovlCallMode == MD_OVLCALL_CALL) {
        omOvlCallEx(sys->nextDll, TRUE, 0, 0);
    } else if(sys->ovlCallMode == MD_OVLCALL_RETURN) {
        omOvlReturnEx(1, TRUE);
    } else if(sys->ovlCallMode == 3) {
        omOvlCallEx(sys->nextDll, TRUE, 0, 0);
    } else if(sys->ovlCallMode == 4) {
        omOvlCallEx(sys->nextDll, TRUE, 1, 0);
    }
    HuPrcEnd();
    while(1) {
        HuPrcVSleep();
    }
}

void fn_1_1700(void)
{
    MDCAMERA_WORK *camera = sys->camera;
    MDSPRITE_WORK *sprite = sys->sprite;
    MDMODEL_WORK *model = sys->model;
    MDMESS *mess = sys->mess;
    ANIMDATA *animP;
    MDOMAKE_EFFECT *effect;
    HU3DPARMANID parManId;
    s16 cameraBit;
    int i;
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
            { 0x80, 0xC0, 0xFF, 0xFF },
            { 0xFF, 0xFF, 0x40, 0xFF },
            { 0xFF, 0x60, 0x80, 0xFF },
        },
        {
            { 0x80, 0xC0, 0xFF, 0x00 },
            { 0xFF, 0x40, 0x20, 0x00 },
            { 0x80, 0x10, 0x10, 0x00 },
        }
    };
    MdSpriteDispSet(sprite, MDSPR_GRP_WIN, FALSE);
    MdSpriteDispSet(sprite, MDSPR_GRP_CURSOR_LR, FALSE);
    MdSpriteDispSet(sprite, MDSPR_GRP_CURSOR_UDLR, FALSE);
    MdSpriteDispSet(sprite, MDSPR_GRP_MODE, FALSE);
    Hu3DModelDispOn(model->mdlId[MDMODEL_STAR]);
    Hu3DModelDispOn(model->mdlId[MDMODEL_STAGE]);
    for(i=0; i<MDMODEL_CAP_NUM; i++) {
        Hu3DModelDispOff(model->mdlId[MDMODEL_CAP_START+i]);
    }
    Hu3DModelDispOn(model->mdlId[MDMODEL_FONTBGFILTER]);
    cameraBit = camera->camera[MDCAMERA_GUIDE].bit;
    animP = sprite->animP[4];
    effect = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDOMAKE_EFFECT), HU_MEMNUM_OVL);
    parManId = Hu3DParManCreate(animP, 100, &param);
    effect->parManId[0] = parManId;
    Hu3DParManAttrSet(parManId, 0x65);
    Hu3DParticleBlendModeSet(Hu3DParManModelIDGet(parManId), 1);
    Hu3DParManRotSet(parManId, 90, 0, 0);
    Hu3DModelLayerSet(Hu3DParManModelIDGet(parManId), 0);
    Hu3DModelCameraSet(Hu3DParManModelIDGet(parManId), cameraBit);
    lbl_1_bss_1FC = effect;
    MdOmakeReset();
    HuPrcVSleep();
    if(sys->evtNo == 0) {
        sys->subMode = 0;
    } else {
        sys->subMode = 10;
    }
    sys->flowMode = MDOMAKE_FLOW_MAIN;
}

#define MdOmakeWinOpenAll(mess) do { \
    MdMessWinOpen(mess, 0, 1, 0, 0, 0, 512, 64, 32, 372, 0); \
    MdMessWinOpen(mess, 0, 1, 0, 0, 0, 464, 64, 56, 376, 0); \
    MdMessWinOpen(mess, 1, 1, 0, 0, 1, 448, 32, 64, 340, 0); \
    MdMessWinSpeedSet(mess, 1, 0); \
    MdMessWinAttrSet(mess, 0, HUWIN_ATTR_ALIGN_CENTER); \
    MdMessWinAttrSet(mess, 1, HUWIN_ATTR_ALIGN_CENTER); \
    MdMessWinPad1Set(mess, 0); \
} while(0)

void fn_1_192C(void)
{
    MDMODEL_WORK *model = sys->model;
    MDMESS *mess = sys->mess;
    MDOMAKE_STAR *star = sys->star;
    int choice;
    switch(sys->subMode) {
        case 0:
            MdOmakeReset();
            sys->selMode = 0;
            sys->thpState = 1;
            sys->subMode = 1;
            return;

        case 1:
            WipeColorSet(255, 255, 255);
            WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, 30);
            fn_1_5868(MDOMAKE_AUD_MUSSTART);
            WipeWait();
            MdProcWait(MDOMAKE_PROC_TITLE_UPDATE, 0);
            MdOmakeReset();
            MdProcWait(MDOMAKE_PROC_WIN_UPDATE, 0);
            MdOmakeReset();
            star->motNext = 2;
            MdOmakeWinOpenAll(mess);
            MdMessWinMesSet(mess, 0, FALSE, MDOMAKE_MES(0), -1);
            fn_1_5868(MDOMAKE_AUD_SE_CAP_SPEAK);
            while(!MdMessWinCheck(mess, 0)) {
                HuPrcVSleep();
            }
            MdMessWinHomeClear(mess, 0);
            MdProcWait(MDOMAKE_PROC_TITLE_UPDATE, 1);
            MdOmakeReset();
            star->mode = MDOMAKE_STAR_RESET;
            star->motNext = 3;
            while(star->mode != MDOMAKE_STAR_ENTER_IDLE) {
                HuPrcVSleep();
            }
            MdProcWait(MDOMAKE_PROC_CAP_UPDATE, 0);
            MdOmakeReset();
            star->motNext = 3;
            MdMessWinMesSet(mess, 0, FALSE, MDOMAKE_MES(3)+sys->selMode, -1);
            MdMessWinMesSet(mess, 1, FALSE, MESSNUM(MESS_SYSHELP, 4), -1);
            MdProcWait(MDOMAKE_PROC_CURSOR_UPDATE, 0);
            MdOmakeReset();
            sys->subMode = 2;
            return;

        case 2:
            if((sys->pad->DStkDown[0] & PAD_BUTTON_LEFT) || (sys->pad->DStkDown[0] & PAD_BUTTON_RIGHT)) {
                sys->prevSelMode = sys->selMode;
                if(sys->pad->DStkDown[0] & PAD_BUTTON_RIGHT) {
                    sys->selMode--;
                    if(sys->selMode == -1) {
                        sys->selMode = MDOMAKE_MODE_MAX-1;
                    }
                    sys->turnDir = 0;
                } else if(sys->pad->DStkDown[0] & PAD_BUTTON_LEFT) {
                    sys->selMode = ++sys->selMode % MDOMAKE_MODE_MAX;
                    sys->turnDir = 1;
                }
                fn_1_5868(MDOMAKE_AUD_SECMN_MOVE);
                MdMessWinMesSet(mess, 0, FALSE, MDOMAKE_MES(3)+sys->selMode, -1);
                sys->thpState = 2;
                MdProcWait(MDOMAKE_PROC_CAP_UPDATE, 1);
                MdOmakeReset();
                return;
            }
            if(HuPadBtnDown[0] & PAD_BUTTON_B) {
                fn_1_5868(MDOMAKE_AUD_SECMN_CANCEL);
                MdMessWinMesSet(mess, 0, TRUE, MDOMAKE_MES(2), -1);
                MdMessWinHomeClear(mess, 1);
                while(!MdMessWinCheck(mess, 0)) {
                    HuPrcVSleep();
                }
                choice = MdMessWinChoiceGet(mess, 0);
                switch(choice) {
                    case -1:
                    case 1:
                        MdMessWinMesSet(mess, 0, FALSE, MDOMAKE_MES(3)+sys->selMode, -1);
                        MdMessWinMesSet(mess, 1, FALSE, MESSNUM(MESS_SYSHELP, 4), -1);
                        return;

                    case 0:
                        sys->loadState = MD_LOAD_STATE_LOADING;
                        sys->loadDir = DATA_mdsel;
                        star->motNext = 3;
                        MdMessWinHomeClear(mess, 0);
                        MdProcWait(MDOMAKE_PROC_CURSOR_UPDATE, 1);
                        MdOmakeReset();
                        MdProcWait(MDOMAKE_PROC_WIN_UPDATE, 1);
                        MdOmakeReset();
                        WipeColorSet(255, 255, 255);
                        WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 30);
                        fn_1_5868(MDOMAKE_AUD_MUSEND);
                        WipeWait();
                        while(sys->loadState == MD_LOAD_STATE_LOADING) {
                            HuPrcVSleep();
                        }
                        omOvlHisChg(0, sys->ovlHis->ovl, 0, sys->ovlHis->stat);
                        sys->ovlCallMode = MD_OVLCALL_RETURN;
                        sys->nextDll = DLL_mdseldll;
                        sys->skipWipeF = TRUE;
                        sys->exitF = TRUE;
                        return;
                }
            } else if(HuPadBtnDown[0] & PAD_BUTTON_A) {
                fn_1_5868(MDOMAKE_AUD_SECMN_ACCEPT);
                MdMessWinHomeClear(mess, 1);
                MdMessWinMesSet(mess, 0, FALSE, MDOMAKE_MES(6)+sys->selMode, -1);
                while(!MdMessWinCheck(mess, 0)) {
                    HuPrcVSleep();
                }
                sys->loadState = MD_LOAD_STATE_LOADING;
                sys->loadDir = lbl_1_data_154[sys->selMode];
                MdProcWait(MDOMAKE_PROC_CURSOR_UPDATE, 1);
                MdOmakeReset();
                MdProcWait(MDOMAKE_PROC_CAP_UPDATE, 2);
                MdOmakeReset();
                MdProcWait(MDOMAKE_PROC_SCENE_UPDATE, 0);
                WipeColorSet(255, 255, 255);
                WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 60);
                fn_1_5868(MDOMAKE_AUD_MUSEND);
                HuPrcVSleep();
                while(WipeCheckEnd()) {
                    MdProcCall(MDOMAKE_PROC_SCENE_UPDATE, 0);
                    HuPrcVSleep();
                }
                while(sys->loadState == MD_LOAD_STATE_LOADING) {
                    HuPrcVSleep();
                }
                if(sys->selMode == 0) {
                    omOvlHisChg(0, sys->ovlHis->ovl, 1, sys->ovlHis->stat);
                    sys->ovlCallMode = MD_OVLCALL_CALL;
                } else if(sys->selMode == 1) {
                    omOvlHisChg(0, sys->ovlHis->ovl, 2, sys->ovlHis->stat);
                    sys->ovlCallMode = 3;
                } else if(sys->selMode == 2) {
                    omOvlHisChg(0, sys->ovlHis->ovl, 3, sys->ovlHis->stat);
                    sys->ovlCallMode = 4;
                }
                sys->nextDll = lbl_1_data_148[sys->selMode];
                sys->skipWipeF = TRUE;
                sys->exitF = TRUE;
                return;
            }
            return;

        case 10:
            MdOmakeReset();
            sys->selMode = 0;
            star->mode = MDOMAKE_STAR_EXIT_START;
            while(star->mode != MDOMAKE_STAR_ENTER_IDLE) {
                HuPrcVSleep();
            }
            MdProcWait(MDOMAKE_PROC_CAP_UPDATE, 3);
            MdOmakeReset();
            sys->thpState = 1;
            WipeColorSet(255, 255, 255);
            WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, 30);
            fn_1_5868(MDOMAKE_AUD_MUSSTART);
            WipeWait();
            MdProcWait(MDOMAKE_PROC_WIN_UPDATE, 0);
            MdOmakeReset();
            MdOmakeWinOpenAll(mess);
            star->motNext = 3;
            MdMessWinMesSet(mess, 0, FALSE, MDOMAKE_MES(3)+sys->selMode, -1);
            MdMessWinMesSet(mess, 1, FALSE, MESSNUM(MESS_SYSHELP, 4), -1);
            MdProcWait(MDOMAKE_PROC_CURSOR_UPDATE, 0);
            MdOmakeReset();
            sys->subMode = 2;
            break;
    }
}

BOOL fn_1_28BC(int param)
{
    sys->procInitF[0] = 0;
    sys->procInitF[1] = 0;
    sys->procInitF[2] = 0;
    sys->procInitF[3] = 0;
    sys->procInitF[4] = 0;
    sys->time[0] = 0;
    sys->time[1] = 0;
    sys->time[2] = 0;
    sys->time[3] = 0;
    sys->time[4] = 0;
    sys->time[5] = 0;
    sys->t[0] = 0;
    sys->t[1] = 0;
    sys->t[2] = 0;
    sys->t[3] = 0;
    sys->t[4] = 0;
    sys->t[5] = 0;
    return FALSE;
}

BOOL fn_1_2A78(int param)
{
    MDCAMERA *camera = &sys->camera->camera[0];
    MDOMAKE_CAMLERP *lerp = &lbl_1_data_31C[param];
    MdMathOscillate(lerp->type, &sys->time[4], &sys->t[4], lerp->time);
    MdMathLerp(&camera->hvr.x, lerp->hvrStart.x, lerp->hvrEnd.x, sys->t[4]);
    MdMathLerp(&camera->hvr.y, lerp->hvrStart.y, lerp->hvrEnd.y, sys->t[4]);
    MdMathLerp(&camera->hvr.z, lerp->hvrStart.z, lerp->hvrEnd.z, sys->t[4]);
    MdMathLerp(&camera->center.x, lerp->centerStart.x, lerp->centerEnd.x, sys->t[4]);
    MdMathLerp(&camera->center.y, lerp->centerStart.y, lerp->centerEnd.y, sys->t[4]);
    MdMathLerp(&camera->center.z, lerp->centerStart.z, lerp->centerEnd.z, sys->t[4]);
    if(sys->time[4] >= 1) {
        return TRUE;
    }
    return FALSE;
}

BOOL fn_1_2BF8(int param)
{
    MDSPRITE_WORK *sprite = sys->sprite;
    HUSPRGRPID grpId = sprite->grpId[MDSPR_GRP_CURSOR_UDLR];
    HuVec2f *pos;
    HuVec2f *scale;
    if(sys->procInitF[2] == 0) {
        lbl_1_bss_18C[0].x = -64;
        lbl_1_bss_18C[0].y = 208;
        lbl_1_bss_18C[0].z = 0;
        lbl_1_bss_18C[1].x = 144;
        lbl_1_bss_18C[1].y = 208;
        lbl_1_bss_18C[1].z = 0;
        lbl_1_bss_18C[2].x = 640;
        lbl_1_bss_18C[2].y = 208;
        lbl_1_bss_18C[2].z = 0;
        lbl_1_bss_18C[3].x = 432;
        lbl_1_bss_18C[3].y = 208;
        lbl_1_bss_18C[3].z = 0;
        switch(param) {
            case 0:
                HuSprGrpPosSet(grpId, 0, 0);
                pos = MdSpritePosGet(sprite, MDSPR_GRP_CURSOR_UDLR, 0);
                pos->x = lbl_1_bss_18C[0].x;
                pos->y = lbl_1_bss_18C[0].y;
                HuSprAttrReset(grpId, 0, HUSPR_ATTR_DISPOFF);
                pos = MdSpritePosGet(sprite, MDSPR_GRP_CURSOR_UDLR, 1);
                pos->x = lbl_1_bss_18C[2].x;
                pos->y = lbl_1_bss_18C[2].y;
                HuSprAttrReset(grpId, 1, HUSPR_ATTR_DISPOFF);
                break;

            case 2:
                pos = MdSpritePosGet(sprite, MDSPR_GRP_CURSOR_UDLR, 4);
                pos->x = lbl_1_bss_18C[1].x;
                pos->y = lbl_1_bss_18C[1].y;
                HuSprTPLvlSet(grpId, 4, 0);
                HuSprAttrReset(grpId, 4, HUSPR_ATTR_DISPOFF);
                break;

            case 3:
                pos = MdSpritePosGet(sprite, MDSPR_GRP_CURSOR_UDLR, 5);
                pos->x = lbl_1_bss_18C[3].x;
                pos->y = lbl_1_bss_18C[3].y;
                HuSprTPLvlSet(grpId, 5, 0);
                HuSprAttrReset(grpId, 5, HUSPR_ATTR_DISPOFF);
                break;
        }
        sys->procInitF[2] = 1;
    }
    switch(param) {
        case 0:
            MdMathOscillate(MDMATH_OSC_90, &sys->time[4], &sys->t[4], 15);
            MdMathLerp(&MdSpritePosGet(sprite, MDSPR_GRP_CURSOR_UDLR, 0)->x, lbl_1_bss_18C[0].x, lbl_1_bss_18C[1].x, sys->t[4]);
            MdMathLerp(&MdSpritePosGet(sprite, MDSPR_GRP_CURSOR_UDLR, 1)->x, lbl_1_bss_18C[2].x, lbl_1_bss_18C[3].x, sys->t[4]);
            break;

        case 1:
            MdMathOscillate(MDMATH_OSC_90, &sys->time[4], &sys->t[4], 15);
            MdMathLerp(&MdSpritePosGet(sprite, MDSPR_GRP_CURSOR_UDLR, 0)->x, lbl_1_bss_18C[1].x, lbl_1_bss_18C[0].x, sys->t[4]);
            MdMathLerp(&MdSpritePosGet(sprite, MDSPR_GRP_CURSOR_UDLR, 1)->x, lbl_1_bss_18C[3].x, lbl_1_bss_18C[2].x, sys->t[4]);
            break;

        case 2:
            MdMathOscillate(MDMATH_OSC_180, &sys->time[4], &sys->t[4], 20);
            pos = MdSpritePosGet(sprite, MDSPR_GRP_CURSOR_UDLR, 0);
            scale = MdSpriteScaleGet(sprite, MDSPR_GRP_CURSOR_UDLR, 0);
            MdMathLerp(&pos->x, lbl_1_bss_18C[1].x, lbl_1_bss_18C[1].x-24, sys->t[4]);
            MdMathLerp(&scale->x, 1, 1.5f, sys->t[4]);
            MdMathLerp(&scale->y, 1, 1.5f, sys->t[4]);
            pos = MdSpritePosGet(sprite, MDSPR_GRP_CURSOR_UDLR, 4);
            scale = MdSpriteScaleGet(sprite, MDSPR_GRP_CURSOR_UDLR, 4);
            MdMathLerp(&pos->x, lbl_1_bss_18C[1].x, lbl_1_bss_18C[1].x-24, sys->t[4]);
            MdMathLerp(&scale->x, 1, 1.5f, sys->t[4]);
            MdMathLerp(&scale->y, 1, 1.5f, sys->t[4]);
            HuSprTPLvlSet(grpId, 4, sys->t[4]);
            break;

        case 3:
            MdMathOscillate(MDMATH_OSC_180, &sys->time[4], &sys->t[4], 20);
            pos = MdSpritePosGet(sprite, MDSPR_GRP_CURSOR_UDLR, 1);
            scale = MdSpriteScaleGet(sprite, MDSPR_GRP_CURSOR_UDLR, 1);
            MdMathLerp(&pos->x, lbl_1_bss_18C[3].x, 24+lbl_1_bss_18C[3].x, sys->t[4]);
            MdMathLerp(&scale->x, 1, 1.5f, sys->t[4]);
            MdMathLerp(&scale->y, 1, 1.5f, sys->t[4]);
            pos = MdSpritePosGet(sprite, MDSPR_GRP_CURSOR_UDLR, 5);
            scale = MdSpriteScaleGet(sprite, MDSPR_GRP_CURSOR_UDLR, 5);
            MdMathLerp(&pos->x, lbl_1_bss_18C[3].x, 24+lbl_1_bss_18C[3].x, sys->t[4]);
            MdMathLerp(&scale->x, 1, 1.5f, sys->t[4]);
            MdMathLerp(&scale->y, 1, 1.5f, sys->t[4]);
            HuSprTPLvlSet(grpId, 5, sys->t[4]);
            break;
    }
    if(sys->time[4] >= 1) {
        switch(param) {
            case 1:
                MdSpriteDispSet(sprite, MDSPR_GRP_CURSOR_UDLR, FALSE);
                break;

            case 2:
                HuSprAttrSet(grpId, 4, HUSPR_ATTR_DISPOFF);
                break;

            case 3:
                HuSprAttrSet(grpId, 5, HUSPR_ATTR_DISPOFF);
                break;
        }
        return TRUE;
    }
    return FALSE;
}

BOOL fn_1_34F0(int param)
{
    MDSPRITE_WORK *sprite = sys->sprite;
    HUSPRGRPID grpId = sprite->grpId[MDSPR_GRP_WIN];
    HuVec2f *pos0 = MdSpritePosGet(sprite, MDSPR_GRP_WIN, 0);
    HuVec2f *pos1 = MdSpritePosGet(sprite, MDSPR_GRP_WIN, 1);
    HuVec2f *pos2 = MdSpritePosGet(sprite, MDSPR_GRP_WIN, 2);
    HuVec2f *scale0 = MdSpriteScaleGet(sprite, MDSPR_GRP_WIN, 0);
    HuVec2f *scale1 = MdSpriteScaleGet(sprite, MDSPR_GRP_WIN, 1);
    HuVec2f *scale2 = MdSpriteScaleGet(sprite, MDSPR_GRP_WIN, 2);
    if(sys->procInitF[0] == 0) {
        lbl_1_bss_188 = 0;
        lbl_1_bss_184 = pos0->y;
        lbl_1_bss_180 = pos1->y;
        lbl_1_bss_17C = scale2->y;
        if(param == 0) {
            pos0->y = 0;
            pos1->y = 0;
            scale0->x = 0;
            scale1->x = 0;
            scale2->x = 0;
            scale2->y = 0;
            MdSpriteDispSet(sprite, MDSPR_GRP_WIN, TRUE);
        }
        sys->procInitF[0] = 1;
    }
    if(param == 0) {
        if(lbl_1_bss_188 == 0) {
            MdMathOscillate(MDMATH_OSC_90, &sys->time[3], &sys->t[3], 12);
            MdMathLerp(&pos0->y, 0, -2, sys->t[3]);
            MdMathLerp(&pos1->y, 0, 2, sys->t[3]);
            MdMathLerp(&scale0->x, 0, 72, sys->t[3]);
            MdMathLerp(&scale1->x, 0, 72, sys->t[3]);
            MdMathLerp(&scale2->x, 0, 1, sys->t[3]);
            if(sys->time[3] >= 1) {
                sys->time[3] = 0;
                lbl_1_bss_188 = 1;
            }
        } else if(lbl_1_bss_188 == 1) {
            MdMathOscillate(MDMATH_OSC_90, &sys->time[3], &sys->t[3], 15);
            MdMathLerp(&pos0->y, -2, -28, sys->t[3]);
            MdMathLerp(&pos1->y, 2, 32, sys->t[3]);
            MdMathLerp(&scale2->y, 0, 8, sys->t[3]);
            if(sys->time[3] >= 1) {
                lbl_1_bss_188 = 2;
            }
        }
    } else if(param == 1) {
        if(lbl_1_bss_188 == 0) {
            MdMathOscillate(MDMATH_OSC_90, &sys->time[3], &sys->t[3], 15);
            MdMathLerp(&pos0->y, lbl_1_bss_184, -2, sys->t[3]);
            MdMathLerp(&pos1->y, lbl_1_bss_180, 2, sys->t[3]);
            MdMathLerp(&scale2->y, lbl_1_bss_17C, 0, sys->t[3]);
            if(sys->time[3] >= 1) {
                sys->time[3] = 0;
                lbl_1_bss_188 = 1;
            }
        } else if(lbl_1_bss_188 == 1) {
            MdMathOscillate(MDMATH_OSC_90, &sys->time[3], &sys->t[3], 12);
            MdMathLerp(&pos0->y, -2, 0, sys->t[3]);
            MdMathLerp(&pos1->y, 2, 0, sys->t[3]);
            MdMathLerp(&scale0->x, 72, 0, sys->t[3]);
            MdMathLerp(&scale1->x, 72, 0, sys->t[3]);
            MdMathLerp(&scale2->x, 1, 0, sys->t[3]);
            if(sys->time[3] >= 1) {
                lbl_1_bss_188 = 2;
            }
        }
    }
    if(lbl_1_bss_188 == 2) {
        if(param == 1) {
            MdSpriteDispSet(sprite, MDSPR_GRP_WIN, FALSE);
        }
        return TRUE;
    }
    return FALSE;
}

BOOL fn_1_3C24(int param)
{
    MDCAMERA_WORK *camera = sys->camera;
    MDSPRITE_WORK *sprite = sys->sprite;
    MDMODEL_WORK *model = sys->model;
    HuVecF *pos;
    HuVecF *rot;
    HuVecF *scale;
    MDOMAKE_CAP *cap;
    int i;
    if(sys->procInitF[1] == 0) {
        switch(param) {
            case 0:
                lbl_1_bss_94.x = 0;
                lbl_1_bss_94.y = 225;
                lbl_1_bss_94.z = 0;
                cap = &lbl_1_bss_A0[0];
                for(i=0; i<MDMODEL_CAP_NUM; i++, cap++) {
                    pos = model->posP[MDMODEL_CAP_START+i];
                    rot = model->rotP[MDMODEL_CAP_START+i];
                    scale = model->scaleP[MDMODEL_CAP_START+i];
                    rot->x = 0;
                    rot->y = 0;
                    rot->z = 0;
                    scale->x = 0;
                    scale->y = 0;
                    scale->z = 0;
                    cap->mode = 0;
                    cap->delay = lbl_1_data_390[i];
                    cap->timeA = 0;
                    cap->timeB = 0;
                    cap->tA = 0;
                    cap->tB = 0;
                    cap->radius = 250;
                    cap->yaw = lbl_1_data_354[i];
                    cap->yawBase = 0;
                    cap->radiusY = 150;
                    cap->pitch = lbl_1_data_360[i];
                    cap->pitchBase = 0;
                    pos->x = lbl_1_bss_94.x+cap->radius*HuCos(cap->yaw);
                    pos->y = lbl_1_bss_94.y+cap->radiusY*HuSin(cap->pitch);
                    pos->z = lbl_1_bss_94.z+cap->radius*HuSin(cap->yaw);
                    cap->end.x = pos->x;
                    cap->end.y = pos->y;
                    cap->end.z = pos->z;
                    cap->radius = 1000;
                    cap->yaw = lbl_1_data_398[i];
                    cap->yawBase = lbl_1_data_398[i];
                    cap->pitchBase = lbl_1_data_3A4[i];
                    pos->x = cap->end.x+cap->radius*HuCos(cap->yaw);
                    pos->y = cap->end.y+cap->radius*HuSin(cap->yaw);
                    pos->z = cap->end.z;
                }
                fn_1_5868(MDOMAKE_AUD_SE_ENTER);
                break;

            case 1:
                cap = &lbl_1_bss_A0[0];
                for(i=0; i<MDMODEL_CAP_NUM; i++, cap++) {
                    cap->mode = 0;
                    cap->delay = 0;
                    cap->timeA = 0;
                    cap->timeB = 0;
                    cap->tA = 0;
                    cap->tB = 0;
                    cap->yawBase = cap->yaw;
                    cap->pitchBase = cap->pitch;
                }
                break;

            case 2:
                for(i=0; i<MDMODEL_CAP_NUM; i++) {
                    Hu3D2Dto3D(&lbl_1_data_36C[i], camera->camera[MDCAMERA_MAIN].bit, &lbl_1_bss_70[i]);
                }
                cap = &lbl_1_bss_A0[0];
                for(i=0; i<MDMODEL_CAP_NUM; i++, cap++) {
                    pos = model->posP[MDMODEL_CAP_START+i];
                    cap->mode = 0;
                    cap->delay = 0;
                    cap->timeA = 0;
                    cap->timeB = 0;
                    cap->tA = 0;
                    cap->tB = 0;
                    cap->start.x = pos->x;
                    cap->start.y = pos->y;
                    cap->start.z = pos->z;
                    if(cap->yaw >= 89 && cap->yaw <= 91) {
                        cap->end.x = lbl_1_bss_70[0].x;
                        cap->end.y = lbl_1_bss_70[0].y;
                        cap->end.z = lbl_1_bss_70[0].z;
                    } else if(cap->yaw <= -149 && cap->yaw >= -151) {
                        cap->end.x = lbl_1_bss_70[1].x;
                        cap->end.y = lbl_1_bss_70[1].y;
                        cap->end.z = lbl_1_bss_70[1].z;
                    } else if(cap->yaw <= -29 && cap->yaw >= -31) {
                        cap->end.x = lbl_1_bss_70[2].x;
                        cap->end.y = lbl_1_bss_70[2].y;
                        cap->end.z = lbl_1_bss_70[2].z;
                    }
                }
                fn_1_5868(MDOMAKE_AUD_SE_ENTER);
                break;

            case 3:
                lbl_1_bss_94.x = 0;
                lbl_1_bss_94.y = 225;
                lbl_1_bss_94.z = 0;
                cap = &lbl_1_bss_A0[0];
                for(i=0; i<MDMODEL_CAP_NUM; i++, cap++) {
                    pos = model->posP[MDMODEL_CAP_START+i];
                    rot = model->rotP[MDMODEL_CAP_START+i];
                    scale = model->scaleP[MDMODEL_CAP_START+i];
                    rot->x = 0;
                    rot->y = 0;
                    rot->z = 0;
                    if(i == sys->selMode) {
                        scale->x = 2;
                        scale->y = 2;
                        scale->z = 2;
                    } else {
                        scale->x = 0.75f;
                        scale->y = 0.75f;
                        scale->z = 0.75f;
                    }
                    Hu3DModelDispOn(model->mdlId[MDMODEL_CAP_START+i]);
                    cap->mode = 0;
                    cap->delay = lbl_1_data_390[i];
                    cap->timeA = 0;
                    cap->timeB = 0;
                    cap->tA = 0;
                    cap->tB = 0;
                    cap->radius = 250;
                    cap->yaw = lbl_1_data_354[i];
                    cap->yawBase = 0;
                    cap->radiusY = 150;
                    cap->pitch = lbl_1_data_360[i];
                    cap->pitchBase = 0;
                    MdMathAngleClamp(&cap->yaw);
                    MdMathAngleClamp(&cap->pitch);
                    pos->x = lbl_1_bss_94.x+cap->radius*HuCos(cap->yaw);
                    pos->y = lbl_1_bss_94.y+cap->radiusY*HuSin(cap->pitch);
                    pos->z = lbl_1_bss_94.z+cap->radius*HuSin(cap->yaw);
                    cap->end.x = pos->x;
                    cap->end.y = pos->y;
                    cap->end.z = pos->z;
                }
                break;
        }
        lbl_1_bss_178 = 0;
        sys->procInitF[1] = 1;
    }
    switch(param) {
        case 0:
            cap = &lbl_1_bss_A0[0];
            for(i=0; i<MDMODEL_CAP_NUM; i++, cap++) {
                pos = model->posP[MDMODEL_CAP_START+i];
                scale = model->scaleP[MDMODEL_CAP_START+i];
                switch(cap->mode) {
                    case 0:
                        if(--cap->delay <= 0) {
                            Hu3DModelDispOn(model->mdlId[MDMODEL_CAP_START+i]);
                            cap->mode++;
                        }
                        break;

                    case 1:
                        MdMathOscillate(MDMATH_OSC_90, &cap->timeA, &cap->tA, 60);
                        MdMathOscillate(MDMATH_OSC_180, &cap->timeB, &cap->tB, 60);
                        MdMathLerp(&cap->radius, 1000, 0, cap->tA);
                        MdMathLerp(&cap->yaw, cap->yawBase, cap->pitchBase, cap->tA);
                        pos->x = cap->end.x+cap->radius*HuCos(cap->yaw);
                        pos->y = cap->end.y+cap->radius*HuSin(cap->yaw);
                        MdMathLerp(&pos->z, -3000, cap->end.z, cap->tA);
                        if(i == sys->selMode) {
                            MdMathLerpScale(scale, 0, 2, cap->tA);
                        } else {
                            MdMathLerpScale(scale, 0, 0.75f, cap->tA);
                        }
                        if(cap->timeA >= 1) {
                            cap->radius = 250;
                            cap->radiusY = 150;
                            cap->yaw = lbl_1_data_354[i];
                            cap->pitch = lbl_1_data_360[i];
                            MdMathAngleClamp(&cap->yaw);
                            MdMathAngleClamp(&cap->pitch);
                            cap->mode++;
                        }
                        break;
                }
            }
            break;

        case 1:
            cap = &lbl_1_bss_A0[0];
            for(i=0; i<MDMODEL_CAP_NUM; i++, cap++) {
                pos = model->posP[MDMODEL_CAP_START+i];
                scale = model->scaleP[MDMODEL_CAP_START+i];
                switch(cap->mode) {
                    case 1:
                        break;

                    case 0:
                        MdMathOscillate(MDMATH_OSC_90, &cap->timeA, &cap->tA, 20);
                        MdMathOscillate(MDMATH_OSC_180, &cap->timeB, &cap->tB, 20);
                        if(sys->turnDir == 0) {
                            MdMathLerp(&cap->yaw, cap->yawBase, 120+cap->yawBase, cap->tA);
                            MdMathLerp(&cap->pitch, cap->pitchBase, cap->pitchBase-120, cap->tA);
                        } else if(sys->turnDir == 1) {
                            MdMathLerp(&cap->yaw, cap->yawBase, cap->yawBase-120, cap->tA);
                            MdMathLerp(&cap->pitch, cap->pitchBase, 120+cap->pitchBase, cap->tA);
                        }
                        pos->x = lbl_1_bss_94.x+cap->radius*HuCos(cap->yaw);
                        pos->y = lbl_1_bss_94.y+cap->radiusY*HuSin(cap->pitch);
                        pos->z = lbl_1_bss_94.z+cap->radius*HuSin(cap->yaw);
                        if(i == sys->selMode) {
                            MdMathLerpOfs(&cap->scale, 0.75f, 2, cap->tA, cap->tB);
                        } else if(i == sys->prevSelMode) {
                            MdMathLerp(&cap->scale, 2, 0.75f, cap->tA);
                        } else {
                            cap->scale = 0.75f;
                        }
                        scale->x = cap->scale;
                        scale->y = cap->scale;
                        scale->z = cap->scale;
                        if(cap->timeA >= 1) {
                            MdMathAngleClamp(&cap->yaw);
                            MdMathAngleClamp(&cap->pitch);
                            cap->mode++;
                        }
                        break;
                }
            }
            if(sys->turnDir == 0) {
                MdProcCall(MDOMAKE_PROC_CURSOR_UPDATE, 3);
            } else if(sys->turnDir == 1) {
                MdProcCall(MDOMAKE_PROC_CURSOR_UPDATE, 2);
            }
            break;

        case 2:
            cap = &lbl_1_bss_A0[0];
            for(i=0; i<MDMODEL_CAP_NUM; i++, cap++) {
                pos = model->posP[MDMODEL_CAP_START+i];
                switch(cap->mode) {
                    case 1:
                        break;

                    case 0:
                        if(i == sys->selMode) {
                            MdMathOscillate(MDMATH_OSC_90, &cap->timeA, &cap->tA, 30);
                            MdMathOscillate(MDMATH_OSC_180, &cap->timeB, &cap->tB, 30);
                            MdMathLerp(&pos->x, cap->start.x, cap->end.x, cap->tA);
                            MdMathLerpOfs(&pos->y, cap->start.y, cap->end.y, cap->tA, -10*cap->tB);
                            MdMathLerp(&pos->z, cap->start.z, cap->end.z, cap->tA);
                        } else {
                            MdMathOscillate(MDMATH_OSC_90_REV, &cap->timeA, &cap->tA, 30);
                            MdMathOscillate(MDMATH_OSC_180, &cap->timeB, &cap->tB, 30);
                            MdMathLerp(&pos->x, cap->start.x, cap->end.x, cap->tA);
                            MdMathLerp(&pos->y, cap->start.y, cap->end.y, cap->tA);
                            MdMathLerpOfs(&pos->z, cap->start.z, cap->end.z, cap->tA, cap->tB);
                        }
                        if(cap->timeA >= 1) {
                            if(i != sys->selMode) {
                                Hu3DModelDispOff(model->mdlId[MDMODEL_CAP_START+i]);
                            }
                            cap->mode++;
                        }
                        break;
                }
            }
            break;
    }
    switch(param) {
        case 0:
            if(lbl_1_bss_A0[0].mode == 2) {
                return TRUE;
            }
            return FALSE;

        case 1:
            if(lbl_1_bss_A0[0].mode == 1) {
                return TRUE;
            }
            return FALSE;

        case 2:
            if(lbl_1_bss_A0[0].mode == 1) {
                return TRUE;
            }
            return FALSE;

        case 3:
            return TRUE;

        default:
            return FALSE;
    }
}

BOOL fn_1_4F34(int param)
{
    MDCAMERA_WORK *camera = sys->camera;
    MDSPRITE_WORK *sprite = sys->sprite;
    MDMODEL_WORK *model = sys->model;
    MDMESS *mess = sys->mess;
    HuVecF *pos = model->posP[MDMODEL_STAR];
    HuVecF *rot = model->rotP[MDMODEL_STAR];
    HuVecF *scale = model->scaleP[MDMODEL_STAR];
    if(sys->procInitF[1] == 0) {
        lbl_1_bss_54[0].x = 64;
        lbl_1_bss_54[0].y = 300;
        lbl_1_bss_54[0].z = 1000;
        Hu3D2Dto3D(&lbl_1_bss_54[0], camera->camera[MDCAMERA_GUIDE].bit, &lbl_1_bss_3C[0]);
        *pos = lbl_1_bss_3C[0];
        lbl_1_bss_54[1].x = 288;
        lbl_1_bss_54[1].y = 200;
        lbl_1_bss_54[1].z = 1200;
        Hu3D2Dto3D(&lbl_1_bss_54[1], camera->camera[MDCAMERA_MAIN].bit, &lbl_1_bss_3C[1]);
        Hu3D3Dto2D(&lbl_1_bss_3C[1], camera->camera[MDCAMERA_GUIDE].bit, &lbl_1_bss_54[1]);
        lbl_1_bss_54[1].z = 2000;
        Hu3D2Dto3D(&lbl_1_bss_54[1], camera->camera[MDCAMERA_GUIDE].bit, &lbl_1_bss_3C[1]);
        fn_1_5778(0, 1);
        rot->z = 0;
        lbl_1_bss_24[0] = *rot;
        lbl_1_bss_24[1].x = 0;
        lbl_1_bss_24[1].y = -180;
        lbl_1_bss_24[1].z = 16;
        fn_1_5868(MDOMAKE_AUD_SE_2);
        lbl_1_bss_6C = 0;
        sys->procInitF[1] = 1;
    }
    switch(lbl_1_bss_6C) {
        case 0:
        case 1:
            MdMathOscillate(MDMATH_OSC_90_REV, &sys->time[0], &sys->t[0], 90);
            MdMathOscillate(MDMATH_OSC_180, &sys->time[1], &sys->t[1], 90);
            MdMathLerpOfs(&pos->x, lbl_1_bss_3C[0].x, lbl_1_bss_3C[1].x, sys->t[0], 1.5f*sys->t[1]);
            MdMathLerpOfs(&pos->y, lbl_1_bss_3C[0].y, lbl_1_bss_3C[1].y, sys->t[0], 1.5f*sys->t[1]);
            MdMathLerpOfs(&pos->z, lbl_1_bss_3C[0].z, lbl_1_bss_3C[1].z, sys->t[0], 0);
            MdMathLerp(&rot->x, lbl_1_bss_24[0].x, lbl_1_bss_24[1].x, sys->t[0]);
            MdMathLerp(&rot->y, lbl_1_bss_24[0].y, lbl_1_bss_24[1].y, sys->t[0]);
            MdMathLerpOfs(&rot->z, lbl_1_bss_24[0].z, lbl_1_bss_24[1].z, sys->t[0], 3*sys->t[1]);
            Hu3DParManPosSet(lbl_1_bss_1FC->parManId[0], pos->x, pos->y, pos->z);
            Hu3DParManTimeLimitSet(lbl_1_bss_1FC->parManId[0], 2);
            Hu3DParManAttrReset(lbl_1_bss_1FC->parManId[0], 1);
            if(sys->time[0] >= 0.3f) {
                MdProcCall(MDOMAKE_PROC_WIN_UPDATE, 1);
                MdProcCall(MDOMAKE_PROC_CAMERA_LERP, 0);
            }
            if(lbl_1_bss_6C == 0) {
                if(sys->time[0] >= 0.3f) {
                    MdMessWinHomeClear(mess, 0);
                    lbl_1_bss_6C++;
                }
            }
            break;
    }
    if(lbl_1_bss_6C == 1) {
        return TRUE;
    }
    return FALSE;
}

BOOL fn_1_54F0(int param)
{
    MDSPRITE_WORK *sprite = sys->sprite;
    HUSPRGRPID grpId = sprite->grpId[MDSPR_GRP_MODE];
    HuVec2f *pos;
    if(sys->procInitF[3] == 0) {
        if(param == 0) {
            HuSprGrpPosSet(grpId, 0, 0);
            HuSprPosSet(grpId, 0, lbl_1_data_3B0[1].x, lbl_1_data_3B0[1].y);
            HuSprAttrReset(grpId, 0, HUSPR_ATTR_DISPOFF);
        }
        sys->procInitF[3] = 1;
    }
    if(param == 0) {
        MdMathOscillate(MDMATH_OSC_90, &sys->time[5], &sys->t[5], 15);
        pos = MdSpritePosGet(sprite, MDSPR_GRP_MODE, 0);
        MdMathLerp(&pos->x, lbl_1_data_3B0[0].x, lbl_1_data_3B0[1].x, sys->t[5]);
        MdMathLerp(&pos->y, lbl_1_data_3B0[0].y, lbl_1_data_3B0[1].y, sys->t[5]);
    } else if(param == 1) {
        MdMathOscillate(MDMATH_OSC_90_REV, &sys->time[5], &sys->t[5], 15);
        pos = MdSpritePosGet(sprite, MDSPR_GRP_MODE, 0);
        MdMathLerp(&pos->x, lbl_1_data_3B0[1].x, lbl_1_data_3B0[0].x, sys->t[5]);
        MdMathLerp(&pos->y, lbl_1_data_3B0[1].y, lbl_1_data_3B0[0].y, sys->t[5]);
    }
    if(sys->time[5] >= 1) {
        return TRUE;
    }
    return FALSE;
}

void fn_1_5778(int mdlNo, int cameraNo)
{
    MDCAMERA *camera;
    HuVecF *pos;
    MDMODEL_WORK *model;
    HuVecF *rot;
    MDCAMERA_WORK *cameraWork;
    float rotY;
    HuVecF dir;
    cameraWork = sys->camera;
    camera = &cameraWork->camera[cameraNo];
    model = sys->model;
    pos = model->posP[mdlNo];
    rot = model->rotP[mdlNo];
    dir.x = camera->pos.x-pos->x;
    dir.y = camera->pos.y-pos->y;
    dir.z = camera->pos.z-pos->z;
    rotY = HuAtan(dir.x, dir.z);
    rot->y = rotY;
}

void fn_1_5868(int cmd)
{
    int pan;
    switch(cmd) {
        case MDOMAKE_AUD_MUSSTART:
            lbl_1_bss_20 = HuAudBGMPlay(MSM_STREAM_MODE);
            break;

        case MDOMAKE_AUD_MUSEND:
            HuAudSStreamFadeOut(lbl_1_bss_20, 1000);
            break;

        case MDOMAKE_AUD_SECMN_MOVE:
        case MDOMAKE_AUD_SECMN_ACCEPT:
        case MDOMAKE_AUD_SECMN_CANCEL:
        case MDOMAKE_AUD_SE_ENTER:
        case MDOMAKE_AUD_SE_2:
            if(cmd == MDOMAKE_AUD_SECMN_MOVE) {
                HuAudFXPlay(MSM_SE_CMN_01);
            } else if(cmd == MDOMAKE_AUD_SECMN_ACCEPT) {
                HuAudFXPlay(MSM_SE_CMN_03);
            } else if(cmd == MDOMAKE_AUD_SECMN_CANCEL) {
                HuAudFXPlay(MSM_SE_CMN_04);
            } else if(cmd == MDOMAKE_AUD_SE_ENTER) {
                HuAudFXPlay(MSM_SE_MENU_27);
            } else if(cmd == MDOMAKE_AUD_SE_2) {
                HuAudFXPlay(MSM_SE_MENU_29);
            }
            break;

        case MDOMAKE_AUD_SE_CAP_SPEAK:
            pan = fn_1_5988(MDMODEL_STAR, MDCAMERA_GUIDE);
            if(cmd == MDOMAKE_AUD_SE_CAP_SPEAK) {
                HuAudFXPlayPan(MSM_SE_GUIDE_53, pan);
            }
            break;

        case MDOMAKE_AUD_FX_STOP:
            HuAudAllStop();
            break;
    }
}

int fn_1_5988(int mdlNo, int cameraNo)
{
    MDCAMERA *camera;
    MDMODEL_WORK *model;
    MDCAMERA_WORK *cameraWork;
    HuVecF *pos3D;
    int result;
    HuVecF pos2D;
    cameraWork = sys->camera;
    camera = &cameraWork->camera[cameraNo];
    model = sys->model;
    pos3D = model->posP[mdlNo];
    Hu3D3Dto2D(pos3D, camera->bit, &pos2D);
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
        { "STAR", MDCAMERA_GUIDE },
        { "STAGE", MDCAMERA_MAIN },
        { "CAP_CARD", MDCAMERA_MAIN },
        { "CAP_VOLLEY", MDCAMERA_MAIN },
        { "CAP_HOCKEY", MDCAMERA_MAIN },
        { "FONTBGFILTER", MDCAMERA_GUIDE },
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