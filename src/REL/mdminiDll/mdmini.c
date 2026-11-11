#include "REL/mdminiDll.h"
#include "game/memory.h"
#include "game/process.h"
#include "game/armem.h"
#include "game/data.h"
#include "game/flag.h"
#include "game/gamework.h"
#include "game/wipe.h"
#include "game/saveload.h"

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
BOOL fn_1_9714(int param);
BOOL fn_1_991C(int param);
BOOL fn_1_AF34(int param);
BOOL fn_1_B668(int param);
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
void fn_1_16308(void);
void fn_1_17008(int cmd);
int fn_1_171B4(int mdlNo, int cameraNo);
void fn_1_23D8(void);
void fn_1_27F8(void);
void fn_1_35A0(void);
void fn_1_5558(void);
void fn_1_6FB8(void);
void fn_1_7638(void);
void fn_1_81E0(void);
BOOL fn_1_9AC8(int param);
BOOL fn_1_C8B4(int param);
BOOL fn_1_E44C(int param);
BOOL fn_1_F6C4(int param);
BOOL fn_1_FC94(int param);
BOOL fn_1_10264(int param);
BOOL fn_1_106D4(int param);
BOOL fn_1_10B0C(int param);
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

BOOL fn_1_9714(int param)
{
    sys->dirtyF = FALSE;
    sys->teamInitF = FALSE;
    sys->winInitF = FALSE;
    sys->cursorInitF = FALSE;
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

/* 0x16308 - same latch for the four computer slots */
void fn_1_16308(void)
{
    int i;

    for(i = 0; i < 4; i++) {
        sys->mdlF[i] = sys->mdlE[i];
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

s8 capPlayerInitF;
