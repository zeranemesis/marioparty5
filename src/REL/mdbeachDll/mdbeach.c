#include "REL/mdbeachDll.h"
#include "game/wipe.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/armem.h"
#include "game/hsfex.h"
#include "game/mgdata.h"
#include <math.h>
#include "game/frand.h"
#include "game/memory.h"
#include "game/gamework.h"

typedef struct MdBeachEffect_s {
    int parManId[3];
} MDBEACH_EFFECT;

/* prototypes */
extern void fn_1_A30(void);
extern void fn_1_15AC(void);
extern void fn_1_19AC(void);
extern void fn_1_2554(void);
extern void fn_1_28C8(void);
extern void fn_1_290C(void);
extern void fn_1_2968(void);
extern void fn_1_2D3C(void);
extern void fn_1_31A8(void);
extern void fn_1_4138(void);
extern void fn_1_57A8(void);
extern void fn_1_6E9C(void);
extern void fn_1_79EC(void);
extern BOOL fn_1_9A14(int param);
extern BOOL fn_1_9BAC(int param);
extern BOOL fn_1_9D58(int param);
extern BOOL fn_1_B1E8(int param);
extern BOOL fn_1_B91C(int param);
extern BOOL fn_1_D45C(int param);
extern BOOL fn_1_EC34(int param);
extern BOOL fn_1_118D0(int param);
extern BOOL fn_1_12314(int param);
extern BOOL fn_1_12894(int param);
extern BOOL fn_1_12E6C(int param);
extern BOOL fn_1_13850(int param);
extern BOOL fn_1_13A2C(int param);
extern void fn_1_13CAC(void);
extern void fn_1_13EA4(void);
extern void fn_1_14050(void);
extern void fn_1_14354(void);
extern void fn_1_146D0(void);
extern int fn_1_14A68(s8 padNo, int mode);
extern void fn_1_14F44(void);
extern void fn_1_15048(void);
extern void fn_1_153C8(void);
extern void fn_1_15418(int modelNo, int cameraNo);
extern void fn_1_15508(void);
extern void fn_1_15A40(int type);
extern s16 fn_1_15BC8(int modelNo, int cameraNo);
extern void MdPosAdjustMode(void);
extern void fn_1_16DE8(void);
static MDBEACH_EFFECT *ParticleDotCreate(ANIMDATA *animP, s16 layer, s16 cameraBit);
static void ParticleDotPosSet(MDBEACH_EFFECT *effectP, HuVecF *pos);
static void ParticleDotKill(MDBEACH_EFFECT *effectP);
static MDBEACH_EFFECT *ParticleGlowCreate(ANIMDATA *animP, s16 layer, s16 cameraBit);
static void ParticleGlowPosSet(MDBEACH_EFFECT *effectP, HuVecF *pos);
static void ParticleGlowKill(MDBEACH_EFFECT *effectP);

/* ---- part A ---- */
/* partA: ObjectSetup, MdMain, LoadProc, fn_1_A30, fn_1_15AC, fn_1_19AC, fn_1_2554, fn_1_28C8, fn_1_290C, fn_1_2968,
   MdInit, MdClose, fn_1_2D3C  (+ globals sys, sysP, sysWork, MdFlowTbl, MdProcTbl)

   MdFlowTbl[MDBEACH_FLOW_*]:
     0 INIT      -> fn_1_2D3C
     1 CHAR_SEL  -> fn_1_31A8
     2 GAME_SEL  -> fn_1_4138
     3 ORDER_SEL -> fn_1_57A8
     4 VS_GAME   -> fn_1_6E9C
     5 GAME      -> fn_1_79EC
   MdProcTbl[MDBEACH_PROC_*]:
     0  RESET        -> fn_1_9A14
     1  CAMERA_LERP  -> fn_1_9BAC
     2  CHR_SEL      -> fn_1_9D58
     3  CHAR_MOTION  -> fn_1_B1E8
     4  GAME_INIT    -> fn_1_B91C
     5  GAME_SEL     -> fn_1_D45C
     6  ORDER_SEL    -> fn_1_EC34
     7  CHR_POS      -> fn_1_118D0
     8  CHR_GRP_POS  -> fn_1_12314
     9  MODEL_POS    -> fn_1_12894
     10 MODEL_SCALE  -> fn_1_12E6C
     11 CHAR_SCALE   -> fn_1_13A2C
     12 CAPMAN_SCALE -> fn_1_13850
*/

/* ---------------- PROTOS ---------------- */

void ObjectSetup(void);

/* ---------------- DATA ---------------- */
SYS sysWork;
SYS *sys;
SYS *sysP;

MD_FUNC MdFlowTbl[MDBEACH_FLOW_MAX] = {
    fn_1_2D3C,
    fn_1_31A8,
    fn_1_4138,
    fn_1_57A8,
    fn_1_6E9C,
    fn_1_79EC,
};

MD_PROC MdProcTbl[MDBEACH_PROC_MAX] = {
    fn_1_9A14,
    fn_1_9BAC,
    fn_1_9D58,
    fn_1_B1E8,
    fn_1_B91C,
    fn_1_D45C,
    fn_1_EC34,
    fn_1_118D0,
    fn_1_12314,
    fn_1_12894,
    fn_1_12E6C,
    fn_1_13A2C,
    fn_1_13850,
};

/* loadDir per selMode */
static unsigned int loadDirTbl[2] = { 0x370000, 0x860000 };

/* camera focus used by the star model when moving to the selection screen */
static HuVecF starTargetScr = { 288.0f, 310.0f, 1250.0f };

/* chrWork[].mode -> data_764 flag: set once the motion shift to idle has been issued */
static int chrIdleShiftF[4] = { 0, 0, 0, 0 };

/* star model working data (screen positions, world positions, oscillation in/out) */
static HuVecF starScr[2];
static HuVecF starPos[2];
static float starOscIn[2];
static float starOscOut[2];

/* per-slot model/pos/rot/scale of the two character model kinds (index 0: char, 1: volley/hockey) */
static HU3DMODELID chrMdlId[MDMODEL_CHAR_NUM][2];
static HuVecF *chrPosP[MDMODEL_CHAR_NUM][2];
static HuVecF *chrRotP[MDMODEL_CHAR_NUM][2];
static HuVecF *chrScaleP[MDMODEL_CHAR_NUM][2];
static HuVecF chrScr;
static HuVecF chrWorld;

/* screen offsets [selMode][chrNo]: char model position relative to the volley/hockey model; flash model relative to the char model */
static HuVecF chrPosOfsTbl[2][MDMODEL_CHAR_NUM] = {
    {
        { 0.0f, -30.0f, 1500.0f },
        { 0.0f, -30.0f, 1500.0f },
        { -8.0f, -17.0f, 1500.0f },
        { -24.0f, -2.0f, 1500.0f },
        { -8.0f, -26.0f, 1500.0f },
        { -1.0f, -16.0f, 1500.0f },
        { 0.0f, -16.0f, 1500.0f },
        { 0.0f, -30.0f, 1500.0f },
        { 0.0f, -24.0f, 1500.0f },
        { 7.0f, -39.0f, 1500.0f },
    },
    {
        { 0.0f, -30.0f, 1500.0f },
        { 4.0f, -30.0f, 1500.0f },
        { -3.0f, -17.0f, 1500.0f },
        { 2.0f, -27.0f, 1500.0f },
        { 3.0f, -29.0f, 1500.0f },
        { -1.0f, -16.0f, 1500.0f },
        { 0.0f, -16.0f, 1500.0f },
        { 0.0f, -30.0f, 1500.0f },
        { 0.0f, -29.0f, 1500.0f },
        { -1.0f, -39.0f, 1500.0f },
    },
};

static HuVecF flashPosOfsTbl[2][MDMODEL_CHAR_NUM] = {
    {
        { 0.0f, -60.0f, 1440.0f },
        { 0.0f, -65.0f, 1440.0f },
        { 0.0f, -70.0f, 1480.0f },
        { 0.0f, -60.0f, 1440.0f },
        { 0.0f, -50.0f, 1480.0f },
        { 0.0f, -65.0f, 1470.0f },
        { 0.0f, -65.0f, 1460.0f },
        { 2.0f, -50.0f, 1440.0f },
        { 30.0f, -55.0f, 1450.0f },
        { 10.0f, -45.0f, 1490.0f },
    },
    {
        { 0.0f, -60.0f, 1440.0f },
        { 0.0f, -65.0f, 1440.0f },
        { 0.0f, -70.0f, 1480.0f },
        { 0.0f, -60.0f, 1440.0f },
        { 0.0f, -50.0f, 1480.0f },
        { 0.0f, -65.0f, 1470.0f },
        { 0.0f, -65.0f, 1460.0f },
        { 2.0f, -50.0f, 1440.0f },
        { 30.0f, -55.0f, 1450.0f },
        { 10.0f, -45.0f, 1490.0f },
    },
};

/* ---------------- CODE ---------------- */
void ObjectSetup(void)
{
    sys = &sysWork;
    sysP = sys;
    sys->subMode = 0;
    sys->unk4 = 0;
    sys->flowTime = 0;
    sys->flowMode = MDBEACH_FLOW_INIT;
    sys->ovlCallMode = MD_OVLCALL_GOTO;
    sys->nextDll = DLL_selmenuDLL;
    sys->exitF = FALSE;
    sys->skipWipeF = FALSE;
    mgPauseExitF = TRUE;
    sys->objman = omInitObjMan(512, 8192);
    sys->ovlHis = omOvlHisGet(0);
    sys->evtNo = sys->ovlHis->evtno;
    if (sys->evtNo == 0 || sys->evtNo == 2) {
        sys->selMode = 0;
    } else {
        sys->selMode = 1;
    }
    omGameSysInit(sys->objman);
    sys->obj[MDBEACH_OBJ_DEBUGFONT] = omAddObj(sys->objman, 100, 0, 0, MdObjDebugFontCreate);
    sys->obj[MDBEACH_OBJ_DEBUGFONT]->data = HuMemDirectMallocNum(HEAP_MODEL, sizeof(MDDEBUGFONT_WORK), HU_MEMNUM_OVL);
    if (!sys->obj[MDBEACH_OBJ_DEBUGFONT]->data) {
        OSReport("sys->obj[MDBEACH_OBJ_DEBUGFONT]->data = NULL...\n");
    }
    sys->debugFont = sys->obj[MDBEACH_OBJ_DEBUGFONT]->data;
    sys->debugFont->createF = FALSE;
    sys->obj[MDBEACH_OBJ_PAD] = omAddObj(sys->objman, 101, 0, 0, MdObjPadMain);
    sys->obj[MDBEACH_OBJ_PAD]->data = HuMemDirectMallocNum(HEAP_MODEL, sizeof(MDPAD_WORK), HU_MEMNUM_OVL);
    if (!sys->obj[MDBEACH_OBJ_PAD]->data) {
        OSReport("sys->obj[MDBEACH_OBJ_PAD]->data = NULL...\n");
    }
    sys->pad = sys->obj[MDBEACH_OBJ_PAD]->data;
    sys->obj[MDBEACH_OBJ_CAMERA] = omAddObj(sys->objman, 102, 0, 0, MdObjCameraCreate);
    sys->obj[MDBEACH_OBJ_CAMERA]->data = HuMemDirectMallocNum(HEAP_MODEL, sizeof(MDCAMERA_WORK), HU_MEMNUM_OVL);
    if (!sys->obj[MDBEACH_OBJ_CAMERA]->data) {
        OSReport("sys->obj[MDBEACH_OBJ_CAMERA]->data = NULL...\n");
    }
    sys->camera = sys->obj[MDBEACH_OBJ_CAMERA]->data;
    sys->camera->createF = FALSE;
    sys->camera->debugFont = sys->debugFont;
    sys->obj[MDBEACH_OBJ_LIGHT] = omAddObj(sys->objman, 103, 0, 0, MdObjLightCreate);
    sys->obj[MDBEACH_OBJ_LIGHT]->data = HuMemDirectMallocNum(HEAP_MODEL, sizeof(MDLIGHT_WORK), HU_MEMNUM_OVL);
    if (!sys->obj[MDBEACH_OBJ_LIGHT]->data) {
        OSReport("sys->obj[MDBEACH_OBJ_LIGHT]->data = NULL...\n");
    }
    sys->light = sys->obj[MDBEACH_OBJ_LIGHT]->data;
    sys->light->createF = FALSE;
    sys->light->debugFont = sys->debugFont;
    sys->sprite = HuMemDirectMallocNum(HEAP_MODEL, sizeof(MDSPRITE_WORK), HU_MEMNUM_OVL);
    if (!sys->sprite) {
        OSReport("sys->sprite = NULL...\n");
    }
    sys->model = HuMemDirectMallocNum(HEAP_MODEL, sizeof(MDMODEL_WORK), HU_MEMNUM_OVL);
    if (!sys->model) {
        OSReport("sys->model = NULL...\n");
    }
    sys->star = HuMemDirectMallocNum(HEAP_MODEL, sizeof(MDBEACH_STAR), HU_MEMNUM_OVL);
    if (!sys->star) {
        OSReport("sys->star = NULL...\n");
    }
    sys->mess = HuMemDirectMallocNum(HEAP_MODEL, sizeof(MDMESS), HU_MEMNUM_OVL);
    if (!sys->mess) {
        OSReport("sys->mess = NULL...\n");
    }
    MdMessCreate(sys->mess);
    MdMathStdCurveInit();
    Hu3DBGColorSet(0, 0, 0);
    Hu3DNoSyncSet(FALSE);
    while (1) {
        if (sys->debugFont->createF == TRUE && sys->camera->createF == TRUE && sys->light->createF == TRUE) {
            break;
        }
        HuPrcVSleep();
    }
    sys->mainProc = HuPrcChildCreate(MdMain, 104, 12288, 0, sys->objman);
}

void MdMain(void)
{
    MdInit();
    while (!omSysExitReq && !sys->exitF) {
        MdFlowTbl[sys->flowMode]();
        if (++sys->flowTime > 36000) {
            sys->flowTime = 0;
        }
        HuPrcVSleep();
    }
    MdClose();
}

static void LoadProc(void)
{
    int i;
    u32 tick;

    while (1) {
        switch (sys->loadState) {
            case MD_LOAD_STATE_NONE:
                break;

            case MD_LOAD_STATE_LOAD:
                CharDataClose(-1);
                tick = OSGetTick();
                sys->loadAsyncStat = HuDataDirReadAsync(sys->loadDir);
                while (!HuDataGetAsyncStat(sys->loadAsyncStat)) {
                    HuPrcVSleep();
                }
                OSReport("Load Time:%d\n", OSTicksToMilliseconds(OSGetTick() - tick));
                OSReport("Finished!\n");
                HuAR_MRAMtoARAM(sys->loadDir);
                while (HuARDMACheck()) { }
                HuDataDirClose(sys->loadDir);
                sys->loadProcReady = FALSE;
                sys->loadState = MD_LOAD_STATE_WAIT;
                break;

            case MD_LOAD_STATE_WAIT:
                if (sys->loadProcReady == TRUE) {
                    sys->loadState = MD_LOAD_STATE_MOT;
                }
                break;

            case MD_LOAD_STATE_MOT:
                for (i = 0; i < 4; i++) {
                    CharMotionInitAsync(sys->chrNo[i]);
                }
                sys->loadState = MD_LOAD_STATE_END;
                break;

            case MD_LOAD_STATE_END:
                HuPrcEnd();
                break;
        }
        HuPrcVSleep();
    }
}

/* star model: camera path / motion state machine */
void fn_1_A30(void)
{
    int i;
    MDBEACH_STAR *star = sys->star;
    MDCAMERA_WORK *camera = sys->camera;
    MDMODEL_WORK *model = sys->model;
    HuVecF *pos = model->posP[MDMODEL_STAGE];

    star->mode = 0;
    star->motNext = 1;
    starScr[0].x = 288.0f;
    starScr[0].y = 240.0f;
    starScr[0].z = 750.0f;
    Hu3D2Dto3D(&starScr[0], camera->camera[MDCAMERA_GUIDE].bit, &starPos[0]);
    *pos = starPos[0];
    while (1) {
        camera = sys->camera;
        model = sys->model;
        star = sys->star;
        pos = model->posP[MDMODEL_STAGE];
        switch (star->mode) {
            case 1:
                starOscIn[1] = 0.0f;
                starOscIn[0] = 0.0f;
                starOscOut[1] = 0.0f;
                starOscOut[0] = 0.0f;
                starScr[0].x = 288.0f;
                starScr[0].y = 240.0f;
                starScr[0].z = 750.0f;
                starScr[1].x = 64.0f;
                starScr[1].y = 300.0f;
                starScr[1].z = 1000.0f;
                for (i = 0; i < 2; i++) {
                    Hu3D2Dto3D(&starScr[i], camera->camera[MDCAMERA_GUIDE].bit, &starPos[i]);
                }
                star->mode = 2;
                break;

            case 2:
                MdMathOscillate(MDMATH_OSC_90, &starOscIn[0], &starOscOut[0], 30.0f);
                MdMathOscillate(MDMATH_OSC_180, &starOscIn[1], &starOscOut[1], 30.0f);
                MdMathLerp(&pos->x, starPos[0].x, starPos[1].x, starOscOut[0]);
                MdMathLerp(&pos->y, starPos[0].y, starPos[1].y, starOscOut[0]);
                MdMathLerpOfs(&pos->z, starPos[0].z, starPos[1].z, starOscOut[0], -1.0f * starOscOut[1]);
                if (starOscIn[0] >= 1.0f) {
                    star->mode = 3;
                }
                break;

            case 4:
                starOscIn[1] = 0.0f;
                starOscIn[0] = 0.0f;
                starOscOut[1] = 0.0f;
                starOscOut[0] = 0.0f;
                starScr[0].x = 64.0f;
                starScr[0].y = 320.0f;
                starScr[0].z = 1000.0f;
                starScr[1].x = 288.0f;
                starScr[1].y = 240.0f;
                starScr[1].z = 1000.0f;
                for (i = 0; i < 2; i++) {
                    Hu3D2Dto3D(&starScr[i], camera->camera[MDCAMERA_GUIDE].bit, &starPos[i]);
                }
                star->mode = 5;
                break;

            case 5:
                MdMathOscillate(MDMATH_OSC_90, &starOscIn[0], &starOscOut[0], 30.0f);
                MdMathLerp(&pos->x, starPos[0].x, starPos[1].x, starOscOut[0]);
                MdMathLerp(&pos->y, starPos[0].y, starPos[1].y, starOscOut[0]);
                MdMathLerp(&pos->z, starPos[0].z, starPos[1].z, starOscOut[0]);
                if (starOscIn[0] >= 1.0f) {
                    star->mode = 6;
                }
                break;

            case 7:
                starOscIn[1] = 0.0f;
                starOscIn[0] = 0.0f;
                starOscOut[1] = 0.0f;
                starOscOut[0] = 0.0f;
                starScr[0].x = 288.0f;
                starScr[0].y = 240.0f;
                starScr[0].z = 750.0f;
                starScr[1] = starTargetScr;
                for (i = 0; i < 2; i++) {
                    Hu3D2Dto3D(&starScr[i], camera->camera[MDCAMERA_GUIDE].bit, &starPos[i]);
                }
                star->mode = 8;
                break;

            case 8:
                MdMathOscillate(MDMATH_OSC_90, &starOscIn[0], &starOscOut[0], 30.0f);
                MdMathOscillate(MDMATH_OSC_180, &starOscIn[1], &starOscOut[1], 30.0f);
                MdMathLerp(&pos->x, starPos[0].x, starPos[1].x, starOscOut[0]);
                MdMathLerp(&pos->y, starPos[0].y, starPos[1].y, starOscOut[0]);
                MdMathLerpOfs(&pos->z, starPos[0].z, starPos[1].z, starOscOut[0], -1.0f * starOscOut[1]);
                if (starOscIn[0] >= 1.0f) {
                    star->mode = 9;
                }
                break;

            case 10:
                starScr[1] = starTargetScr;
                Hu3D2Dto3D(&starScr[1], camera->camera[MDCAMERA_GUIDE].bit, &starPos[1]);
                *pos = starPos[1];
                star->mode = 9;
                break;

            case 11:
                starOscIn[1] = 0.0f;
                starOscIn[0] = 0.0f;
                starOscOut[1] = 0.0f;
                starOscOut[0] = 0.0f;
                starScr[0].x = 288.0f;
                starScr[0].y = 280.0f;
                starScr[0].z = 800.0f;
                starScr[1] = starTargetScr;
                for (i = 0; i < 2; i++) {
                    Hu3D2Dto3D(&starScr[i], camera->camera[MDCAMERA_GUIDE].bit, &starPos[i]);
                }
                star->mode = 12;
                break;

            case 12:
                MdMathOscillate(MDMATH_OSC_90, &starOscIn[0], &starOscOut[0], 30.0f);
                MdMathOscillate(MDMATH_OSC_180, &starOscIn[1], &starOscOut[1], 30.0f);
                MdMathLerp(&pos->x, starPos[0].x, starPos[1].x, starOscOut[0]);
                MdMathLerp(&pos->y, starPos[0].y, starPos[1].y, starOscOut[0]);
                MdMathLerpOfs(&pos->z, starPos[0].z, starPos[1].z, starOscOut[0], -1.0f * starOscOut[1]);
                if (starOscIn[0] >= 1.0f) {
                    star->mode = 13;
                }
                break;
        }
        switch (star->motNext) {
            case 0:
                break;

            case 1:
                Hu3DModelAttrSet(model->mdlId[MDMODEL_STAGE], HU3D_MOTATTR_LOOP);
                Hu3DModelAttrReset(model->mdlId[MDMODEL_STAGE], HU3D_MOTATTR_PAUSE);
                Hu3DMotionSet(model->mdlId[MDMODEL_STAGE], model->motId[0]);
                star->motNext = 0;
                break;

            case 2:
                Hu3DMotionShiftSet(model->mdlId[MDMODEL_STAGE], model->motId[1], 0.0f, 32.0f, HU3D_MOTATTR_LOOP);
                star->motNext = 0;
                break;

            case 3:
                Hu3DMotionShiftSet(model->mdlId[MDMODEL_STAGE], model->motId[0], 0.0f, 32.0f, HU3D_MOTATTR_LOOP);
                star->motNext = 0;
                break;
        }
        fn_1_15418(0, 1);
        HuPrcVSleep();
    }
}

/* char/capman sprite scale-in / scale-out animation per player slot (capWork) */
void fn_1_15AC(void)
{
    int i;
    MDSPRITE_WORK *sprite;
    HUSPRGRPID grpChar;
    HUSPRGRPID grpCapman;
    HuVec2f *charScale;
    HuVec2f *capmanScale;
    MDBEACH_STATE_WORK *cap;

    for (i = 0; i < 4; i++) {
        sys->capWork[i].mode = 0;
    }
    while (1) {
        sprite = sys->sprite;
        grpChar = sprite->grpId[MDSPR_GRP_CHAR];
        grpCapman = sprite->grpId[MDSPR_GRP_CAPMAN];
        HuSprGrpPosSet(grpChar, 0.0f, 0.0f);
        HuSprGrpPosSet(grpCapman, 0.0f, 0.0f);
        for (i = 0, cap = sys->capWork; i < 4; i++, cap++) {
            charScale = MdSpriteScaleGet(sprite, MDSPR_GRP_CHAR, i);
            capmanScale = MdSpriteScaleGet(sprite, MDSPR_GRP_CAPMAN, i);
            switch (cap->mode) {
                case 1:
                    HuSprAttrReset(grpChar, i, HUSPR_ATTR_DISPOFF);
                    HuSprAttrReset(grpCapman, i, HUSPR_ATTR_DISPOFF);
                    charScale->x = 0.0f;
                    charScale->y = 0.0f;
                    capmanScale->x = 0.0f;
                    capmanScale->y = 0.0f;
                    cap->in0 = 0.0f;
                    cap->in1 = 0.0f;
                    cap->t = 0.0f;
                    cap->ofs = 0.0f;
                    cap->mode = 2;
                    break;

                case 2:
                    MdMathOscillate(MDMATH_OSC_90, &cap->in0, &cap->t, 15.0f);
                    MdMathOscillate(MDMATH_OSC_180, &cap->in1, &cap->ofs, 15.0f);
                    MdMathLerpOfs(&charScale->x, 0.0f, 1.0f, cap->t, 0.5f * cap->ofs);
                    charScale->y = charScale->x;
                    MdMathLerpOfs(&capmanScale->x, 0.0f, 1.0f, cap->t, 0.5f * cap->ofs);
                    capmanScale->y = capmanScale->x;
                    if (cap->in0 >= 1.0f) {
                        cap->mode = 3;
                    }
                    break;

                case 4:
                    cap->in0 = 0.0f;
                    cap->in1 = 0.0f;
                    cap->t = 0.0f;
                    cap->ofs = 0.0f;
                    cap->mode = 5;
                    break;

                case 5:
                    MdMathOscillate(MDMATH_OSC_90, &cap->in0, &cap->t, 15.0f);
                    MdMathLerp(&charScale->x, 1.0f, 0.0f, cap->t);
                    charScale->y = charScale->x;
                    MdMathLerp(&capmanScale->x, 1.0f, 0.0f, cap->t);
                    capmanScale->y = capmanScale->x;
                    if (cap->in0 >= 1.0f) {
                        HuSprAttrSet(grpChar, i, HUSPR_ATTR_DISPOFF);
                        HuSprAttrSet(grpCapman, i, HUSPR_ATTR_DISPOFF);
                        cap->mode = 6;
                    }
                    break;
            }
        }
        HuPrcVSleep();
    }
}

/* player character model slot state machine (chrWork) */
void fn_1_19AC(void)
{
    int i;
    int isEnd;
    MDCAMERA_WORK *camera;
    MDMODEL_WORK *model;
    MDSPRITE_WORK *sprite;
    MDBEACH_STATE_WORK *cap;
    MDBEACH_STATE_WORK *chrW;

    model = sys->model;
    for (i = 0; i < 4; i++) {
        sys->chrWork[i].mode = 0;
    }
    for (i = 0; i < MDMODEL_CHAR_NUM; i++) {
        chrMdlId[i][0] = model->mdlId[MDMODEL_CHAR_START + i];
        chrPosP[i][0] = model->posP[MDMODEL_CHAR_START + i];
        chrRotP[i][0] = model->rotP[MDMODEL_CHAR_START + i];
        chrScaleP[i][0] = model->scaleP[MDMODEL_CHAR_START + i];
        if (sys->selMode == 0) {
            chrMdlId[i][1] = model->mdlId[MDMODEL_VOLLEY_START + i];
            chrPosP[i][1] = model->posP[MDMODEL_VOLLEY_START + i];
            chrRotP[i][1] = model->rotP[MDMODEL_VOLLEY_START + i];
            chrScaleP[i][1] = model->scaleP[MDMODEL_VOLLEY_START + i];
        } else if (sys->selMode == 1) {
            chrMdlId[i][1] = model->mdlId[MDMODEL_HOCKEY_START + i];
            chrPosP[i][1] = model->posP[MDMODEL_HOCKEY_START + i];
            chrRotP[i][1] = model->rotP[MDMODEL_HOCKEY_START + i];
            chrScaleP[i][1] = model->scaleP[MDMODEL_HOCKEY_START + i];
        }
    }
    while (1) {
        camera = sys->camera;
        model = sys->model;
        sprite = sys->sprite;
        cap = sys->capWork;
        chrW = sys->chrWork;
        for (i = 0; i < 4; i++) {
            switch (chrW[i].mode) {
                case 1:
                    Hu3DModelAttrSet(chrMdlId[sys->chrNo[i]][1], HU3D_ATTR_DISPOFF);
                    Hu3DModelAttrReset(chrMdlId[sys->chrNo[i]][0], HU3D_ATTR_DISPOFF);
                    if (sys->chrNo[i] == 8) {
                        Hu3DModelLayerSet(chrMdlId[sys->chrNo[i]][0], 2);
                    } else {
                        Hu3DModelLayerSet(chrMdlId[sys->chrNo[i]][0], 1);
                    }
                    Hu3D3Dto2D(chrPosP[sys->chrNo[i]][1], camera->camera[MDCAMERA_MAIN].bit, &chrScr);
                    chrScr.x = chrScr.x + chrPosOfsTbl[sys->selMode][sys->chrNo[i]].x;
                    chrScr.y = chrScr.y + chrPosOfsTbl[sys->selMode][sys->chrNo[i]].y;
                    chrScr.z = chrPosOfsTbl[sys->selMode][sys->chrNo[i]].z;
                    Hu3D2Dto3D(&chrScr, camera->camera[MDCAMERA_MAIN].bit, &chrWorld);
                    *chrPosP[sys->chrNo[i]][0] = chrWorld;
                    fn_1_15418(sys->chrNo[i] + 1, MDCAMERA_MAIN);
                    fn_1_15A40(sys->chrNo[i] + 18);
                    Hu3D3Dto2D(chrPosP[sys->chrNo[i]][0], camera->camera[MDCAMERA_MAIN].bit, &chrScr);
                    chrScr.x = chrScr.x + flashPosOfsTbl[sys->selMode][sys->chrNo[i]].x;
                    chrScr.y = chrScr.y + flashPosOfsTbl[sys->selMode][sys->chrNo[i]].y;
                    chrScr.z = flashPosOfsTbl[sys->selMode][sys->chrNo[i]].z;
                    Hu3D2Dto3D(&chrScr, camera->camera[MDCAMERA_MAIN].bit, &chrWorld);
                    Hu3DModelAttrReset(model->mdlId[MDMODEL_FLASH_START + i], HU3D_ATTR_DISPOFF);
                    *model->posP[MDMODEL_FLASH_START + i] = chrWorld;
                    Hu3DModelAttrReset(model->mdlId[MDMODEL_FLASH_START + i], HU3D_MOTATTR_LOOP);
                    Hu3DModelAttrReset(model->mdlId[MDMODEL_FLASH_START + i], HU3D_MOTATTR_PAUSE);
                    Hu3DMotionSpeedSet(model->mdlId[MDMODEL_FLASH_START + i], 5.0f);
                    Hu3DMotionTimeSet(model->mdlId[MDMODEL_FLASH_START + i], 0.0f);
                    if (sys->chrNo[i] == 8) {
                        Hu3DModelLayerSet(model->mdlId[MDMODEL_FLASH_START + i], 2);
                    } else {
                        Hu3DModelLayerSet(model->mdlId[MDMODEL_FLASH_START + i], 1);
                    }
                    fn_1_146D0();
                    Hu3DMotionSet(chrMdlId[sys->chrNo[i]][0], model->motId[sys->chrNo[i] * 3 + 4]);
                    Hu3DModelAttrReset(chrMdlId[sys->chrNo[i]][0], HU3D_MOTATTR_LOOP);
                    Hu3DModelAttrReset(chrMdlId[sys->chrNo[i]][0], HU3D_MOTATTR_PAUSE);
                    chrIdleShiftF[i] = 0;
                    cap[i].mode = 1;
                    chrW[i].mode = 2;
                    break;

                case 2:
                    if (cap[i].mode == 3) {
                        chrW[i].mode = 3;
                    }
                    break;

                case 3:
                    isEnd = 0;
                    if (chrIdleShiftF[i] == 0) {
                        isEnd = Hu3DMotionEndCheck(chrMdlId[sys->chrNo[i]][0]);
                        if (isEnd == 1) {
                            Hu3DMotionShiftSet(chrMdlId[sys->chrNo[i]][0], model->motId[sys->chrNo[i] * 3 + 3], 0.0f, 32.0f, HU3D_MOTATTR_LOOP);
                            chrIdleShiftF[i] = 1;
                        }
                    }
                    break;

                case 4:
                    chrIdleShiftF[i] = 1;
                    chrW[i].mode = 3;
                    break;

                case 5:
                    Hu3DModelAttrSet(chrMdlId[sys->chrNo[i]][0], HU3D_ATTR_DISPOFF);
                    Hu3DModelAttrReset(chrMdlId[sys->chrNo[i]][1], HU3D_ATTR_DISPOFF);
                    if (sys->unk36[i] != 0) {
                        if (sys->unk36[i] == 1) {
                            sys->unk36[i] = 0;
                        }
                    }
                    cap[i].mode = 0;
                    HuSprAttrSet(sprite->grpId[MDSPR_GRP_CHAR], i, HUSPR_ATTR_DISPOFF);
                    HuSprAttrSet(sprite->grpId[MDSPR_GRP_CAPMAN], i, HUSPR_ATTR_DISPOFF);
                    Hu3DModelAttrSet(model->mdlId[MDMODEL_FLASH_START + i], HU3D_ATTR_DISPOFF);
                    chrW[i].mode = 6;
                    break;

                case 6:
                    chrW[i].mode = 7;
                    break;
            }
        }
        HuPrcVSleep();
    }
}

/* char model spin state machine (posWork) */
void fn_1_2554(void)
{
    int i;
    MDMODEL_WORK *model;
    MDBEACH_POS_WORK *work;
    HuVecF *rot;

    for (i = 0, work = sys->posWork; i < MDMODEL_CHAR_NUM; i++, work++) {
        work->ofs = 0.0f;
        work->mode = 0;
    }
    while (1) {
        model = sys->model;
        for (i = 0, work = sys->posWork; i < MDMODEL_CHAR_NUM; i++, work++) {
            if (sys->selMode == 0) {
                rot = model->rotP[MDMODEL_VOLLEY_START + i];
            } else if (sys->selMode == 1) {
                rot = model->rotP[MDMODEL_HOCKEY_START + i];
            }
            switch (work->mode) {
                case 1:
                    work->in[0] = 0.0f;
                    work->in[1] = 0.0f;
                    work->in[2] = 0.0f;
                    work->in[3] = 0.0f;
                    work->unk18 = work->ofs;
                    work->mode = 2;
                    break;

                case 2:
                    MdMathOscillate(MDMATH_OSC_90_REV, &work->in[0], &work->in[2], 30.0f);
                    MdMathLerp(&work->ofs, work->unk18, 8.0f, work->in[2]);
                    rot->y = rot->y + work->ofs;
                    MdMathAngleClamp(&rot->y);
                    if (work->in[0] >= 1.0f) {
                        work->mode = 3;
                    }
                    break;

                case 3:
                    rot->y = rot->y + work->ofs;
                    MdMathAngleClamp(&rot->y);
                    break;

                case 4:
                    work->in[0] = 0.0f;
                    work->in[1] = 0.0f;
                    work->in[2] = 0.0f;
                    work->in[3] = 0.0f;
                    work->unk1C = rot->y;
                    work->mode = 5;
                    break;

                case 5:
                    MdMathOscillate(MDMATH_OSC_90, &work->in[0], &work->in[2], 60.0f);
                    MdMathOscillate(MDMATH_OSC_180, &work->in[1], &work->in[3], 60.0f);
                    MdMathLerpOfs(&rot->y, work->unk1C, 360.0f, work->in[2], 0.2f * work->in[3]);
                    if (work->in[0] >= 1.0f) {
                        MdMathAngleClamp(&rot->y);
                        work->mode = 6;
                    }
                    break;
            }
        }
        HuPrcVSleep();
    }
}

void fn_1_28C8(void)
{
    MDMESS *mess;

    while (1) {
        mess = sys->mess;
        MdMessMain(mess, MDMESS_WIN_MAIN);
        HuPrcVSleep();
    }
}

void fn_1_290C(void)
{
    MDMESS *mess;

    while (1) {
        mess = sys->mess;
        MdMessMain(mess, MDMESS_WIN_SUB);
        MdMessMain(mess, MDMESS_WIN_HELP);
        MdMessMain(mess, MDMESS_WIN_INFO);
        HuPrcVSleep();
    }
}

void fn_1_2968(void)
{
    MDMESS *mess;

    while (1) {
        mess = sys->mess;
        MdMessMain(mess, MDMESS_WIN_SELECT);
        HuPrcVSleep();
    }
}

void MdInit(void)
{
    MdSpriteCreate(sys->sprite);
    HuPrcVSleep();
    MdModelCreate(sys->model, sys->camera);
    HuPrcVSleep();
    HuDataDirClose(0x8E0000);
    HuPrcVSleep();
    MdToueiMdlInit();
    sys->starProc = HuPrcChildCreate(fn_1_A30, 106, 12288, 0, sys->objman);
    sys->gameSelProc = HuPrcChildCreate(fn_1_2554, 107, 12288, 0, sys->objman);
    sys->gameInitProc = HuPrcChildCreate(fn_1_19AC, 108, 12288, 0, sys->objman);
    sys->cursorProc = HuPrcChildCreate(fn_1_15AC, 109, 12288, 0, sys->objman);
    sys->mainWinProc = HuPrcChildCreate(fn_1_28C8, 110, 4096, 0, sys->objman);
    sys->helpWinProc = HuPrcChildCreate(fn_1_290C, 111, 4096, 0, sys->objman);
    sys->subWinProc = HuPrcChildCreate(fn_1_2968, 112, 4096, 0, sys->objman);
    sys->loadState = MD_LOAD_STATE_NONE;
    sys->loadAsyncStat = 0;
    sys->loadProcReady = FALSE;
    sys->loadDir = 0x910000;
    sys->loadProc = HuPrcChildCreate(LoadProc, 105, 12288, 0, sys->objman);
    HuPrcVSleep();
}

void MdClose(void)
{
    if (sys->skipWipeF == FALSE) {
        WipeCreate(2, 0, 60);
        while (WipeCheckEnd()) {
            HuPrcVSleep();
        }
    }
    HuAudAllStop();
    MdMessKill(sys->mess);
    if (sys->ovlCallMode == MD_OVLCALL_GOTO) {
        omOvlGotoEx(sys->nextDll, TRUE, 0, 0);
    } else if (sys->ovlCallMode == MD_OVLCALL_CALL) {
        omOvlCallEx(sys->nextDll, TRUE, 0, 0);
    } else if (sys->ovlCallMode == MD_OVLCALL_RETURN) {
        omOvlReturnEx(1, TRUE);
    }
    HuPrcEnd();
    while (1) {
        HuPrcVSleep();
    }
}

void fn_1_2D3C(void)
{
    int i;
    MDSPRITE_WORK *sprite = sys->sprite;
    MDMODEL_WORK *model = sys->model;

    for (i = 0; i < MDSPR_GRP_MAX; i++) {
        MdSpriteDispSet(sprite, i, FALSE);
    }
    HuSprAttrReset(sprite->grpId[MDSPR_GRP_MODEL], sys->selMode, HUSPR_ATTR_DISPOFF);
    HuSprAttrReset(sprite->grpId[MDSPR_GRP_MODEL], 2, HUSPR_ATTR_DISPOFF);
    Hu3DModelAttrReset(model->mdlId[MDMODEL_STAGE], HU3D_ATTR_DISPOFF);
    for (i = 0; i < MDMODEL_CHAR_NUM; i++) {
        Hu3DModelAttrSet(model->mdlId[MDMODEL_CHAR_START + i], HU3D_ATTR_DISPOFF);
    }
    for (i = 0; i < MDMODEL_VOLLEY_NUM; i++) {
        Hu3DModelAttrSet(model->mdlId[MDMODEL_VOLLEY_START + i], HU3D_ATTR_DISPOFF);
    }
    for (i = 0; i < MDMODEL_HOCKEY_NUM; i++) {
        Hu3DModelAttrSet(model->mdlId[MDMODEL_HOCKEY_START + i], HU3D_ATTR_DISPOFF);
    }
    for (i = 0; i < MDMODEL_CAPMAN_NUM + MDMODEL_CAPCOM_NUM; i++) {
        Hu3DModelAttrSet(model->mdlId[MDMODEL_CAPMAN_START + i], HU3D_ATTR_DISPOFF);
    }
    for (i = 0; i < MDMODEL_FINGER_NUM; i++) {
        Hu3DModelAttrSet(model->mdlId[MDMODEL_FINGER_START + i], HU3D_ATTR_DISPOFF);
    }
    for (i = 0; i < MDMODEL_FLASH_NUM; i++) {
        Hu3DModelAttrSet(model->mdlId[MDMODEL_FLASH_START + i], HU3D_ATTR_DISPOFF);
    }
    for (i = 0; i < 2; i++) {
        Hu3DModelAttrSet(model->mdlId[MDMODEL_VS_VOLLEY + i], HU3D_ATTR_DISPOFF);
    }
    for (i = 0; i < MDMODEL_BALL_NUM; i++) {
        Hu3DModelAttrSet(model->mdlId[MDMODEL_BALL_START + i], HU3D_ATTR_DISPOFF);
    }
    Hu3DModelAttrReset(model->mdlId[MDMODEL_FONTBGFILTER], HU3D_ATTR_DISPOFF);
    CharMotionVoiceOnSet(0, 47, FALSE);
    for (i = 1; i < 10; i++) {
        CharMotionVoiceOnSet(i, 6, FALSE);
    }
    MdProcCall(MDBEACH_PROC_RESET, 0);
    HuPrcVSleep();
    if (sys->evtNo == 0 || sys->evtNo == 1) {
        sys->loadState = MD_LOAD_STATE_LOAD;
        sys->loadDir = loadDirTbl[sys->selMode];
    } else {
        CharDataClose(-1);
        sys->loadState = MD_LOAD_STATE_WAIT;
        sys->loadProcReady = FALSE;
    }
    if (sys->evtNo <= 1) {
        sys->subMode = 0;
        sys->flowMode = MDBEACH_FLOW_CHAR_SEL;
    } else {
        sys->subMode = 10;
        sys->flowMode = MDBEACH_FLOW_GAME;
    }
}

/* ---- part B ---- */
/* fn_1_31A8 fn_1_4138 */

/* ---- PROTOS ---- */

/* ---- DATA ---- */
/* original: lbl_1_bss_0 (set to 1 when the player accepts the intro choice and the intro is skipped) */
static s32 MdBeachIntroSkipF;

/* ---- FUNCTIONS ---- */

/* flow: game select (intro, game/character group choice) */
void fn_1_31A8(void)
{
    MDMODEL_WORK *model = sys->model;
    MDMESS *mess = sys->mess;
    MDBEACH_STAR *star = sys->star;
    int choice;

    switch(sys->subMode) {
        case 0:
            MdProcCall(MDBEACH_PROC_RESET, 0);
            MdBeachIntroSkipF = 0;
            sys->subMode = 1;
            break;

        case 1:
            WipeColorSet(255, 255, 255);
            WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, 30);
            fn_1_15A40(0);
            while(WipeCheckEnd()) {
                HuPrcVSleep();
            }
            MdMessWinOpen(mess, 0, 0, 1, 0, 0, 56.0f, 376.0f, 0x1D0, 0x40, 1.0f);
            MdMessWinPad1Set(mess, 0);
            MdMessWinOpen(mess, 1, 1, 0, 0, 1, 32.0f, 340.0f, 0x200, 0x20, 0.0f);
            MdMessWinOpen(mess, 3, 1, 0, 0, 3, 288.0f, -64.0f, 0xE0, 0x20, 0.0f);
            MdMessWinOpen(mess, 4, 1, 0, 0, 4, 288.0f, -64.0f, 0xE0, 0x20, 0.0f);
            MdMessWinSpeedSet(mess, 1, 0);
            MdMessWinSpeedSet(mess, 3, 0);
            MdMessWinSpeedSet(mess, 4, 0);
            MdMessWinAttrSet(mess, 1, 0x800);
            MdMessWinAttrSet(mess, 3, 0x800);
            MdMessWinAttrSet(mess, 4, 0x800);
            while(!MdMessWinCheck(mess, 0)) {
                HuPrcVSleep();
            }
            star->motNext = 2;
            fn_1_15A40(0x1C);
            if(sys->selMode == 0) {
                MdMessWinMesSet(mess, 0, 0, 0x170000, -1);
            } else if(sys->selMode == 1) {
                MdMessWinMesSet(mess, 0, 0, 0x180000, -1);
            }
            while(!MdMessWinCheck(mess, 0)) {
                HuPrcVSleep();
            }
            MdMessWinMesSet(mess, 0, 1, 0x170001, -1);
            while(!MdMessWinCheck(mess, 0)) {
                HuPrcVSleep();
            }
            choice = MdMessWinChoiceGet(mess, 0);
            switch(choice) {
                case -1:
                case 1:
                    MdBeachIntroSkipF = 0;
                    break;

                case 0:
                    MdBeachIntroSkipF = 1;
                    MdMessWinClose(mess, 0);
                    while(!MdMessWinCheck(mess, 0)) {
                        HuPrcVSleep();
                    }
                    MdProcWait(MDBEACH_PROC_CAPMAN_SCALE, 0);
                    MdProcCall(MDBEACH_PROC_RESET, 0);
                    star->motNext = 3;
                    star->mode = 7;
                    while(star->mode != 9) {
                        HuPrcVSleep();
                    }
                    if(sys->selMode == 0) {
                        MdMessWinOpen(mess, 2, 0, 0, 0, 2, 196.0f, 120.0f, 0xB8, 0x60, 1.0f);
                    } else if(sys->selMode == 1) {
                        MdMessWinOpen(mess, 2, 0, 0, 0, 2, 112.0f, 120.0f, 0x160, 0x80, 1.0f);
                    }
                    MdMessWinSpeedSet(mess, 2, 0);
                    while(!MdMessWinCheck(mess, 2)) {
                        HuPrcVSleep();
                    }
                    if(sys->selMode == 0) {
                        MdMessWinMesSet(mess, 2, 0, 0x3F0179, -1);
                    } else if(sys->selMode == 1) {
                        MdMessWinMesSet(mess, 2, 0, 0x3F017E, -1);
                    }
                    star->motNext = 2;
                    MdMessWinOpen(mess, 0, 0, 1, 0, 0, 56.0f, 376.0f, 0x1D0, 0x40, 1.0f);
                    MdMessWinPad1Set(mess, 0);
                    while(!MdMessWinCheck(mess, 0)) {
                        HuPrcVSleep();
                    }
                    if(sys->selMode == 0) {
                        MdMessWinMesSet(mess, 0, 0, 0x170012, -1);
                    } else if(sys->selMode == 1) {
                        MdMessWinMesSet(mess, 0, 0, 0x18000E, -1);
                    }
                    while(!MdMessWinCheck(mess, 0)) {
                        HuPrcVSleep();
                    }
                    MdMessWinHomeClear(mess, 0);
                    MdMessWinClose(mess, 2);
                    while(!MdMessWinCheck(mess, 2)) {
                        HuPrcVSleep();
                    }
                    break;
            }
            if(MdBeachIntroSkipF == 0) {
                star->motNext = 3;
            }
            MdMessWinClose(mess, 0);
            while(!MdMessWinCheck(mess, 0)) {
                HuPrcVSleep();
            }
            if(MdBeachIntroSkipF == 0) {
                MdProcWait(MDBEACH_PROC_CAPMAN_SCALE, 0);
                MdProcCall(MDBEACH_PROC_RESET, 0);
            }
            fn_1_13CAC();
            sys->unk1C = sys->unk1B;
            if(MdBeachIntroSkipF == 0) {
                star->mode = 7;
                while(star->mode != 9) {
                    HuPrcVSleep();
                }
            }
            MdProcWait(MDBEACH_PROC_GAME_INIT, 0);
            MdProcCall(MDBEACH_PROC_RESET, 0);
            MdMessWinOpen(mess, 0, 0, 1, 0, 0, 56.0f, 376.0f, 0x1D0, 0x40, 1.0f);
            MdMessWinPad1Set(mess, 0);
            while(!MdMessWinCheck(mess, 0)) {
                HuPrcVSleep();
            }
            star->motNext = 2;
            MdMessWinMesSet(mess, 0, 0, sys->unk1B + 0x170004, -1);
            MdMessWinMesSet(mess, 1, 0, 0x20004, -1);
            if(sys->unk1D > 1) {
                sys->unk2C4 = 0;
                MdProcWait(MDBEACH_PROC_CHR_SEL, 0);
                MdProcCall(MDBEACH_PROC_RESET, 0);
            }
            sys->subMode = 2;
            break;

        case 2:
            if(sys->unk1D > 1 && ((sys->pad->DStkDown[0] & PAD_BUTTON_LEFT) || (sys->pad->DStkDown[0] & PAD_BUTTON_RIGHT))) {
                if(sys->pad->DStkDown[0] & PAD_BUTTON_LEFT) {
                    sys->unk1C = sys->unk1B;
                    sys->unk1B--;
                    if(sys->unk1B == -1) {
                        sys->unk1B = sys->unk1D - 1;
                    }
                    sys->unk2C0 = 0;
                } else if(sys->pad->DStkDown[0] & PAD_BUTTON_RIGHT) {
                    sys->unk1C = sys->unk1B;
                    sys->unk1B++;
                    sys->unk1B = sys->unk1B % sys->unk1D;
                    sys->unk2C0 = 1;
                }
                MdMessWinMesSet(mess, 0, 0, sys->unk1B + 0x170004, -1);
                fn_1_15A40(2);
                MdProcWait(MDBEACH_PROC_GAME_INIT, sys->unk2C0 + 2);
                MdProcCall(MDBEACH_PROC_RESET, 0);
                break;
            }
            if(HuPadBtnDown[0] & PAD_BUTTON_B) {
                fn_1_15A40(5);
                MdMessWinHomeClear(mess, 1);
                if(sys->selMode == 0) {
                    MdMessWinMesSet(mess, 0, 1, 0x170003, -1);
                } else if(sys->selMode == 1) {
                    MdMessWinMesSet(mess, 0, 1, 0x180003, -1);
                }
                while(!MdMessWinCheck(mess, 0)) {
                    HuPrcVSleep();
                }
                choice = MdMessWinChoiceGet(mess, 0);
                switch(choice) {
                    case -1:
                    case 1:
                        MdMessWinMesSet(mess, 0, 0, sys->unk1B + 0x170004, -1);
                        MdMessWinMesSet(mess, 1, 0, 0x20004, -1);
                        break;

                    case 0:
                        MdMessWinClose(mess, 0);
                        while(!MdMessWinCheck(mess, 0)) {
                            HuPrcVSleep();
                        }
                        WipeColorSet(255, 255, 255);
                        WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 30);
                        fn_1_15A40(1);
                        while(WipeCheckEnd()) {
                            HuPrcVSleep();
                        }
                        if(sys->evtNo == 0 || sys->evtNo == 1) {
                            while(sys->loadState != MD_LOAD_STATE_WAIT) {
                                HuPrcVSleep();
                            }
                        }
                        mgPauseExitF = FALSE;
                        sys->ovlCallMode = MD_OVLCALL_RETURN;
                        sys->nextDll = (OMOVL)0x5E;
                        sys->skipWipeF = TRUE;
                        sys->exitF = TRUE;
                        break;
                }
            } else if(HuPadBtnDown[0] & PAD_BUTTON_A) {
                fn_1_14050();
                star->motNext = 3;
                MdMessWinHomeClear(mess, 1);
                MdMessWinClose(mess, 0);
                while(!MdMessWinCheck(mess, 0)) {
                    HuPrcVSleep();
                }
                fn_1_15A40(3);
                if(sys->unk1D > 1) {
                    sys->unk2C4 = 0;
                    MdProcWait(MDBEACH_PROC_CHR_SEL, 1);
                    MdProcCall(MDBEACH_PROC_RESET, 0);
                }
                MdProcWait(MDBEACH_PROC_GAME_INIT, 1);
                MdProcCall(MDBEACH_PROC_RESET, 0);
                sys->subMode = 0;
                sys->flowMode = MDBEACH_FLOW_ORDER_SEL;
            }
            break;
    }
}

/* flow: order select (players pick their order/team slots with the finger cursors) */
void fn_1_4138(void)
{
    MDMODEL_WORK *model = sys->model;
    MDSPRITE_WORK *sprite = sys->sprite;
    MDMESS *mess = sys->mess;
    MDBEACH_STAR *star = sys->star;
    MDBEACH_STATE_WORK *chrWork = sys->chrWork;
    MDBEACH_POS_WORK *posWork = sys->posWork;
    int i;

    switch(sys->subMode) {
        case 0:
            MdProcCall(MDBEACH_PROC_RESET, 0);
            for(i = 0; i < 4; i++) {
                if(sys->padEnable[i] == 0) {
                    sys->chrNo[i] = 0;
                } else {
                    sys->chrNo[i] = i;
                }
                sys->comPlayerNo[i] = 0;
                sys->unk32[i] = sys->chrNo[i];
                sys->unk36[i] = 0;
            }
            sys->subMode = 1;
            break;

        case 1:
            MdMessWinOpen(mess, 0, 0, 1, 0, 0, 56.0f, 376.0f, 0x1D0, 0x40, 1.0f);
            MdMessWinPad1Set(mess, 0);
            while(!MdMessWinCheck(mess, 0)) {
                HuPrcVSleep();
            }
            star->motNext = 2;
            MdMessWinMesSet(mess, 0, 0, 0x170008, -1);
            MdProcWait(MDBEACH_PROC_GAME_SEL, 0);
            MdProcCall(MDBEACH_PROC_RESET, 0);
            MdMessWinMesSet(mess, 1, 0, 0x20004, -1);
            MdProcWait(MDBEACH_PROC_CHAR_SCALE, 0);
            MdProcCall(MDBEACH_PROC_RESET, 0);
            for(i = 0; i < 4; i++) {
                if(sys->padEnable[i] != 0) {
                    posWork[sys->chrNo[i]].mode = 1;
                }
            }
            sys->subMode = 2;
            break;

        case 2:
            if(sys->comPlayerNo[0] == 0 && (chrWork[0].mode == 0 || chrWork[0].mode == 3 || chrWork[0].mode == 7)
                && sys->comPlayerNo[1] == 0 && (chrWork[1].mode == 0 || chrWork[1].mode == 3 || chrWork[1].mode == 7)
                && sys->comPlayerNo[2] == 0 && (chrWork[2].mode == 0 || chrWork[2].mode == 3 || chrWork[2].mode == 7)
                && sys->comPlayerNo[3] == 0 && (chrWork[3].mode == 0 || chrWork[3].mode == 3 || chrWork[3].mode == 7)
                && (HuPadBtnDown[0] & PAD_BUTTON_B)) {
                fn_1_15A40(5);
                for(i = 0; i < 4; i++) {
                    Hu3DModelAttrSet(model->mdlId[MDMODEL_FINGER_START + i], HU3D_ATTR_DISPOFF);
                }
                for(i = 0; i < 4; i++) {
                    if(sys->padEnable[i] == 1) {
                        posWork[sys->chrNo[i]].mode = 4;
                    }
                }
                MdSpriteDispSet(sprite, MDSPR_GRP_CHAR, FALSE);
                star->motNext = 3;
                MdMessWinClose(mess, 0);
                while(!MdMessWinCheck(mess, 0)) {
                    HuPrcVSleep();
                }
                MdMessWinHomeClear(mess, 1);
                MdProcWait(MDBEACH_PROC_GAME_SEL, 1);
                MdProcCall(MDBEACH_PROC_RESET, 0);
                fn_1_13CAC();
                sys->unk1C = sys->unk1B;
                MdProcWait(MDBEACH_PROC_GAME_INIT, 4);
                MdProcCall(MDBEACH_PROC_RESET, 0);
                MdMessWinOpen(mess, 0, 0, 1, 0, 0, 56.0f, 376.0f, 0x1D0, 0x40, 1.0f);
                MdMessWinPad1Set(mess, 0);
                while(!MdMessWinCheck(mess, 0)) {
                    HuPrcVSleep();
                }
                star->motNext = 2;
                MdMessWinMesSet(mess, 0, 0, sys->unk1B + 0x170004, -1);
                MdMessWinMesSet(mess, 1, 0, 0x20004, -1);
                if(sys->unk1D > 1) {
                    sys->unk2C4 = 0;
                    MdProcWait(MDBEACH_PROC_CHR_SEL, 0);
                    MdProcCall(MDBEACH_PROC_RESET, 0);
                }
                sys->subMode = 2;
                sys->flowMode = MDBEACH_FLOW_CHAR_SEL;
                break;
            }
            for(i = 0; i < 4; i++) {
                if(sys->padEnable[i] != 0 && (chrWork[i].mode == 0 || chrWork[i].mode == 3 || chrWork[i].mode == 7)) {
                    if(fn_1_14A68(i, 0) == 1) {
                        fn_1_15A40(2);
                        posWork[sys->chrNo[i]].mode = 1;
                        posWork[sys->unk32[i]].mode = 4;
                        fn_1_146D0();
                    } else if((HuPadBtnDown[i] & PAD_BUTTON_B) && sys->comPlayerNo[i] == 1) {
                        sys->comPlayerNo[i] = 0;
                        Hu3DModelAttrReset(model->mdlId[MDMODEL_FINGER_START + i], HU3D_ATTR_DISPOFF);
                        chrWork[i].mode = 5;
                        fn_1_15A40(5);
                    } else if((HuPadBtnDown[i] & PAD_BUTTON_A) && sys->comPlayerNo[i] == 0) {
                        sys->comPlayerNo[i] = 1;
                        Hu3DModelAttrSet(model->mdlId[MDMODEL_FINGER_START + i], HU3D_ATTR_DISPOFF);
                        chrWork[i].mode = 1;
                        fn_1_15A40(4);
                        fn_1_15A40(sys->chrNo[i] + 7);
                    }
                }
            }
            if(((sys->comPlayerNo[0] == 1 && chrWork[0].mode == 3) || sys->padEnable[0] == 0)
                && ((sys->comPlayerNo[1] == 1 && chrWork[1].mode == 3) || sys->padEnable[1] == 0)
                && ((sys->comPlayerNo[2] == 1 && chrWork[2].mode == 3) || sys->padEnable[2] == 0)
                && ((sys->comPlayerNo[3] == 1 && chrWork[3].mode == 3) || sys->padEnable[3] == 0)) {
                if(sys->unk1B == 3) {
                    sys->subMode = 4;
                    break;
                }
                sys->selPlayerNo = 0;
                fn_1_14F44();
                fn_1_146D0();
                Hu3DModelAttrReset(model->mdlId[MDMODEL_FINGER_START + sys->teamPlayer[sys->selPlayerNo]], HU3D_ATTR_DISPOFF);
                model->scaleP[MDMODEL_FINGER_START + sys->teamPlayer[sys->selPlayerNo]]->x = 4.0f;
                model->scaleP[MDMODEL_FINGER_START + sys->teamPlayer[sys->selPlayerNo]]->y = 4.0f;
                model->scaleP[MDMODEL_FINGER_START + sys->teamPlayer[sys->selPlayerNo]]->z = 4.0f;
                posWork[sys->chrNo[sys->teamPlayer[sys->selPlayerNo]]].mode = 1;
                MdMessWinMesSet(mess, 0, 0, 0x170009, -1);
                sys->subMode = 3;
            }
            break;

        case 3:
            if(sys->selPlayerNo == 0 && (HuPadBtnDown[0] & PAD_BUTTON_B)) {
                for(i = 0; i < 4; i++) {
                    if(sys->comPlayerNo[i] == 1) {
                        chrWork[i].mode = 5;
                        sys->comPlayerNo[i] = 0;
                    }
                }
                posWork[sys->chrNo[sys->teamPlayer[sys->selPlayerNo]]].mode = 4;
                Hu3DModelAttrSet(model->mdlId[MDMODEL_FINGER_START + sys->teamPlayer[sys->selPlayerNo]], HU3D_ATTR_DISPOFF);
                MdMessWinMesSet(mess, 0, 0, 0x170008, -1);
                fn_1_15A40(5);
                while(chrWork[0].mode != 7) {
                    HuPrcVSleep();
                }
                MdProcWait(MDBEACH_PROC_CHAR_SCALE, 0);
                MdProcCall(MDBEACH_PROC_RESET, 0);
                for(i = 0; i < 4; i++) {
                    if(sys->padEnable[i] != 0) {
                        posWork[sys->chrNo[i]].mode = 1;
                    }
                }
                sys->subMode = 2;
                break;
            }
            if(fn_1_14A68(sys->teamPlayer[sys->selPlayerNo], 1) == 1) {
                fn_1_15A40(2);
                posWork[sys->chrNo[sys->teamPlayer[sys->selPlayerNo]]].mode = 1;
                posWork[sys->unk32[sys->teamPlayer[sys->selPlayerNo]]].mode = 4;
                fn_1_146D0();
                break;
            }
            if((HuPadBtnDown[0] & PAD_BUTTON_B) && sys->selPlayerNo > 0) {
                Hu3DModelAttrSet(model->mdlId[MDMODEL_FINGER_START + sys->teamPlayer[sys->selPlayerNo]], HU3D_ATTR_DISPOFF);
                posWork[sys->chrNo[sys->teamPlayer[sys->selPlayerNo]]].mode = 4;
                sys->selPlayerNo--;
                fn_1_15A40(5);
                chrWork[sys->teamPlayer[sys->selPlayerNo]].mode = 5;
                while(chrWork[sys->teamPlayer[sys->selPlayerNo]].mode != 7) {
                    HuPrcVSleep();
                }
                Hu3DModelAttrReset(model->mdlId[MDMODEL_FINGER_START + sys->teamPlayer[sys->selPlayerNo]], HU3D_ATTR_DISPOFF);
                posWork[sys->chrNo[sys->teamPlayer[sys->selPlayerNo]]].mode = 1;
                sys->comPlayerNo[sys->teamPlayer[sys->selPlayerNo]] = 0;
                fn_1_146D0();
                break;
            }
            if(HuPadBtnDown[0] & PAD_BUTTON_A) {
                chrWork[sys->teamPlayer[sys->selPlayerNo]].mode = 1;
                Hu3DModelAttrSet(model->mdlId[MDMODEL_FINGER_START + sys->teamPlayer[sys->selPlayerNo]], HU3D_ATTR_DISPOFF);
                if(sys->selPlayerNo + 1 < 3 - sys->unk1B) {
                    fn_1_15A40(4);
                } else {
                    fn_1_15A40(3);
                }
                fn_1_15A40(sys->chrNo[sys->teamPlayer[sys->selPlayerNo]] + 7);
                while(chrWork[sys->teamPlayer[sys->selPlayerNo]].mode != 3) {
                    HuPrcVSleep();
                }
                sys->comPlayerNo[sys->teamPlayer[sys->selPlayerNo]] = 1;
                sys->selPlayerNo++;
                if(sys->selPlayerNo < 3 - sys->unk1B) {
                    fn_1_14F44();
                    fn_1_146D0();
                    posWork[sys->chrNo[sys->teamPlayer[sys->selPlayerNo]]].mode = 1;
                    Hu3DModelAttrReset(model->mdlId[MDMODEL_FINGER_START + sys->teamPlayer[sys->selPlayerNo]], HU3D_ATTR_DISPOFF);
                    model->scaleP[MDMODEL_FINGER_START + sys->teamPlayer[sys->selPlayerNo]]->x = 4.0f;
                    model->scaleP[MDMODEL_FINGER_START + sys->teamPlayer[sys->selPlayerNo]]->y = 4.0f;
                    model->scaleP[MDMODEL_FINGER_START + sys->teamPlayer[sys->selPlayerNo]]->z = 4.0f;
                    break;
                }
                if(sys->selPlayerNo >= 3 - sys->unk1B) {
                    sys->subMode = 4;
                }
            }
            break;

        case 4:
            star->motNext = 3;
            MdMessWinClose(mess, 0);
            while(!MdMessWinCheck(mess, 0)) {
                HuPrcVSleep();
            }
            MdMessWinHomeClear(mess, 1);
            MdProcWait(MDBEACH_PROC_GAME_SEL, 2);
            MdProcCall(MDBEACH_PROC_RESET, 0);
            MdProcWait(MDBEACH_PROC_ORDER_SEL, 0);
            MdProcCall(MDBEACH_PROC_RESET, 0);
            sys->subMode = 0;
            sys->flowMode = MDBEACH_FLOW_VS_GAME;
            break;
    }
}

/* ---- part C ---- */
// fn_1_57A8, fn_1_6E9C

/* ---- PROTOS ---- */

/* ---- DATA ---- */
/* all constants (56.0f, 376.0f, 1.0f, 4.0f) are inline literals */

#define MD_PAD_LEFT 0x1
#define MD_PAD_RIGHT 0x2
#define MD_PAD_A 0x100
#define MD_PAD_B 0x200

#define MD_WIN_OPEN_MAIN(mess) do { \
    MdMessWinOpen(mess, 0, 0, 1, 0, 0, 0x1D0, 0x40, 56.0f, 376.0f, 1.0f); \
    MdMessWinPad1Set(mess, 0); \
    while(!MdMessWinCheck(mess, 0)) { \
        HuPrcVSleep(); \
    } \
} while(0)

#define MD_WIN_CLOSE_MAIN(mess) do { \
    MdMessWinClose(mess, 0); \
    while(!MdMessWinCheck(mess, 0)) { \
        HuPrcVSleep(); \
    } \
    MdMessWinHomeClear(mess, 1); \
} while(0)

/* character/team select flow (flowMode 3) */
void fn_1_57A8(void)
{
    MDMODEL_WORK *model = sys->model;
    MDSPRITE_WORK *sprite = sys->sprite;
    MDMESS *mess = sys->mess;
    MDBEACH_STAR *star = sys->star;
    MDBEACH_STATE_WORK *chrWork = sys->chrWork;
    MDBEACH_POS_WORK *posWork = sys->posWork;
    int i;
    int num;
    switch(sys->subMode) {
        case 0:
            MdProcCall(MDBEACH_PROC_RESET, 0);
            sys->selMapNo = 0;
            fn_1_15048();
            fn_1_153C8();
            sys->subMode = 1;
            break;

        case 1:
            MdMessWinMesSet(mess, 3, 0, 0x8002D, -1);
            MdMessWinMesSet(mess, 4, 0, 0x8002D, -1);
            MD_WIN_OPEN_MAIN(mess);
            star->motNext = 2;
            MdMessWinMesSet(mess, 0, 0, 0x17000A, -1);
            MdProcWait(MDBEACH_PROC_CHR_POS, 0);
            MdProcCall(MDBEACH_PROC_RESET, 0);
            MdMessWinMesSet(mess, 1, 0, 0x20004, -1);
            sys->unk2C4 = 1;
            MdProcWait(MDBEACH_PROC_CHR_SEL, 0);
            MdProcCall(MDBEACH_PROC_RESET, 0);
            sys->subMode = 2;
            break;

        case 5:
            MdProcCall(MDBEACH_PROC_GAME_SEL, 0x61);
            break;

        case 2:
            if((sys->pad->DStkDown[0] & MD_PAD_LEFT) || (sys->pad->DStkDown[0] & MD_PAD_RIGHT)) {
                if(sys->pad->DStkDown[0] & MD_PAD_LEFT) {
                    sys->unk2C0 = 0;
                    sys->selMapNo--;
                    if(sys->selMapNo == -1) {
                        sys->selMapNo = 2;
                    }
                } else if(sys->pad->DStkDown[0] & MD_PAD_RIGHT) {
                    sys->unk2C0 = 1;
                    sys->selMapNo = (s8)((sys->selMapNo + 1) % 3);
                }
                fn_1_15A40(2);
                fn_1_153C8();
                fn_1_15048();
                MdProcWait(MDBEACH_PROC_ORDER_SEL, 2);
                MdProcCall(MDBEACH_PROC_RESET, 0);
                break;
            }
            if(HuPadBtnDown[0] & MD_PAD_B) {
                fn_1_15A40(5);
                for(i = 0; i < 4; i++) {
                    posWork[sys->chrNo[i]].mode = 4;
                }
                star->motNext = 3;
                MdMessWinClose(mess, 0);
                while(!MdMessWinCheck(mess, 0)) {
                    HuPrcVSleep();
                }
                MdMessWinHomeClear(mess, 1);
                sys->unk2C4 = 1;
                MdProcWait(MDBEACH_PROC_CHR_SEL, 1);
                MdProcCall(MDBEACH_PROC_RESET, 0);
                MdProcWait(MDBEACH_PROC_CHR_POS, 1);
                MdProcCall(MDBEACH_PROC_RESET, 0);
                MdProcWait(MDBEACH_PROC_ORDER_SEL, 1);
                MdProcCall(MDBEACH_PROC_RESET, 0);
                MdProcWait(MDBEACH_PROC_GAME_SEL, 3);
                MdProcCall(MDBEACH_PROC_RESET, 0);
                if(sys->unk1B == 3) {
                    MD_WIN_OPEN_MAIN(mess);
                    star->motNext = 2;
                    MdMessWinMesSet(mess, 0, 0, 0x170008, -1);
                    while(chrWork[1].mode != 7 || chrWork[2].mode != 7 || chrWork[3].mode != 7) {
                        HuPrcVSleep();
                    }
                    MdMessWinMesSet(mess, 1, 0, 0x20004, -1);
                    MdProcWait(MDBEACH_PROC_CHAR_SCALE, 0);
                    MdProcCall(MDBEACH_PROC_RESET, 0);
                    for(i = 0; i < 4; i++) {
                        if(sys->chrSelEnd[i] != -1) {
                            posWork[sys->chrNo[i]].mode = 1;
                        }
                    }
                    sys->subMode = 2;
                    sys->flowMode = 2;
                    break;
                }
                MD_WIN_OPEN_MAIN(mess);
                star->motNext = 2;
                MdMessWinMesSet(mess, 0, 0, 0x170009, -1);
                while(!((sys->padEnable[1] == 1 || (sys->padEnable[1] == 0 && chrWork[1].mode == 7))
                    && (sys->padEnable[2] == 1 || (sys->padEnable[2] == 0 && chrWork[2].mode == 7))
                    && (sys->padEnable[3] == 1 || (sys->padEnable[3] == 0 && chrWork[3].mode == 7)))) {
                    HuPrcVSleep();
                }
                sys->selPlayerNo = 0;
                fn_1_14F44();
                fn_1_146D0();
                MdMessWinMesSet(mess, 1, 0, 0x20004, -1);
                Hu3DModelAttrReset(model->mdlId[MDMODEL_FINGER_START + sys->teamPlayer[sys->selPlayerNo]], 1);
                model->scaleP[MDMODEL_FINGER_START + sys->teamPlayer[sys->selPlayerNo]]->x = 4.0f;
                model->scaleP[MDMODEL_FINGER_START + sys->teamPlayer[sys->selPlayerNo]]->y = 4.0f;
                model->scaleP[MDMODEL_FINGER_START + sys->teamPlayer[sys->selPlayerNo]]->z = 4.0f;
                posWork[sys->chrNo[sys->teamPlayer[sys->selPlayerNo]]].mode = 1;
                sys->subMode = 3;
                sys->flowMode = 2;
                break;
            }
            if(HuPadBtnDown[0] & MD_PAD_A) {
                fn_1_15A40(3);
                MdMessWinMesSet(mess, 3, 0, sys->unk53[0] + 0x80000, -1);
                MdMessWinMesSet(mess, 4, 0, sys->unk53[1] + 0x80000, -1);
                sys->unk2C4 = 1;
                MdProcWait(MDBEACH_PROC_CHR_SEL, 1);
                MdProcCall(MDBEACH_PROC_RESET, 0);
                if(sys->unk1B == 3) {
                    sys->subMode = 4;
                    break;
                }
                MdMessWinMesSet(mess, 0, 0, 0x170013, -1);
                for(i = 0; i < 4; i++) {
                    sys->unk3E[i] = 1;
                    HuSprBankSet(sprite->grpId[MDSPR_GRP_CAPMAN], i, sys->unk3E[i]);
                }
                sys->selPlayerNo = 0;
                sys->unk2C4 = 2;
                MdProcWait(MDBEACH_PROC_CHR_SEL, 0);
                MdProcCall(MDBEACH_PROC_RESET, 0);
                HuSprAttrSet(sprite->grpId[MDSPR_GRP_CHAR], sys->unk55[sys->selPlayerNo], 4);
                HuSprAttrReset(sprite->grpId[MDSPR_GRP_CAPMAN], sys->unk55[sys->selPlayerNo], 4);
                fn_1_146D0();
                sys->subMode = 3;
            }
            break;

        case 3:
            if(GwCommon.veryHardUnlock == 1) {
                num = 4;
            } else {
                num = 3;
            }
            if((sys->pad->DStkDown[0] & MD_PAD_LEFT) || (sys->pad->DStkDown[0] & MD_PAD_RIGHT)) {
                if(sys->pad->DStkDown[0] & MD_PAD_LEFT) {
                    sys->unk3E[sys->unk55[sys->selPlayerNo]]--;
                    if(sys->unk3E[sys->unk55[sys->selPlayerNo]] == -1) {
                        sys->unk3E[sys->unk55[sys->selPlayerNo]] = num - 1;
                    }
                    sys->unk2C0 = 0;
                } else if(sys->pad->DStkDown[0] & MD_PAD_RIGHT) {
                    sys->unk3E[sys->unk55[sys->selPlayerNo]]++;
                    sys->unk3E[sys->unk55[sys->selPlayerNo]] = sys->unk3E[sys->unk55[sys->selPlayerNo]] % num;
                    sys->unk2C0 = 1;
                }
                fn_1_15A40(2);
                fn_1_146D0();
                HuSprBankSet(sprite->grpId[MDSPR_GRP_CAPMAN], sys->unk55[sys->selPlayerNo], sys->unk3E[sys->unk55[sys->selPlayerNo]]);
                sys->unk2C4 = 2;
                MdProcWait(MDBEACH_PROC_CHR_SEL, sys->unk2C0 + 2);
                MdProcCall(MDBEACH_PROC_RESET, 0);
                break;
            }
            if(HuPadBtnDown[0] & MD_PAD_B) {
                fn_1_15A40(5);
                if(sys->selPlayerNo == 0) {
                    HuSprAttrReset(sprite->grpId[MDSPR_GRP_CHAR], sys->unk55[sys->selPlayerNo], 4);
                    HuSprAttrSet(sprite->grpId[MDSPR_GRP_CAPMAN], sys->unk55[sys->selPlayerNo], 4);
                    sys->unk2C4 = 2;
                    MdProcWait(MDBEACH_PROC_CHR_SEL, 1);
                    MdProcCall(MDBEACH_PROC_RESET, 0);
                    sys->unk2C4 = 1;
                    MdProcWait(MDBEACH_PROC_CHR_SEL, 0);
                    MdProcCall(MDBEACH_PROC_RESET, 0);
                    MdMessWinMesSet(mess, 0, 0, 0x17000A, -1);
                    MdMessWinMesSet(mess, 3, 0, 0x8002D, -1);
                    MdMessWinMesSet(mess, 4, 0, 0x8002D, -1);
                    sys->subMode = 2;
                    break;
                }
                HuSprAttrReset(sprite->grpId[MDSPR_GRP_CHAR], sys->unk55[sys->selPlayerNo], 4);
                HuSprAttrSet(sprite->grpId[MDSPR_GRP_CAPMAN], sys->unk55[sys->selPlayerNo], 4);
                sys->unk2C4 = 2;
                MdProcWait(MDBEACH_PROC_CHR_SEL, 1);
                MdProcCall(MDBEACH_PROC_RESET, 0);
                sys->selPlayerNo--;
                sys->unk2C4 = 2;
                MdProcWait(MDBEACH_PROC_CHR_SEL, 0);
                MdProcCall(MDBEACH_PROC_RESET, 0);
                HuSprAttrSet(sprite->grpId[MDSPR_GRP_CHAR], sys->unk55[sys->selPlayerNo], 4);
                HuSprAttrReset(sprite->grpId[MDSPR_GRP_CAPMAN], sys->unk55[sys->selPlayerNo], 4);
                fn_1_146D0();
                break;
            }
            if(HuPadBtnDown[0] & MD_PAD_A) {
                if(sys->selPlayerNo + 1 >= 3 - sys->unk1B) {
                    fn_1_15A40(3);
                } else {
                    fn_1_15A40(4);
                }
                sys->unk2C4 = 2;
                MdProcWait(MDBEACH_PROC_CHR_SEL, 1);
                MdProcCall(MDBEACH_PROC_RESET, 0);
                sys->selPlayerNo++;
                if(sys->selPlayerNo >= 3 - sys->unk1B) {
                    sys->subMode = 4;
                    break;
                }
                sys->unk2C4 = 2;
                MdProcWait(MDBEACH_PROC_CHR_SEL, 0);
                MdProcCall(MDBEACH_PROC_RESET, 0);
                HuSprAttrSet(sprite->grpId[MDSPR_GRP_CHAR], sys->unk55[sys->selPlayerNo], 4);
                HuSprAttrReset(sprite->grpId[MDSPR_GRP_CAPMAN], sys->unk55[sys->selPlayerNo], 4);
                fn_1_146D0();
            }
            break;

        case 4:
            star->motNext = 3;
            MD_WIN_CLOSE_MAIN(mess);
            sys->subMode = 0;
            if(sys->selMode == 0) {
                sys->flowMode = 4;
            } else if(sys->selMode == 1) {
                sys->flowMode = 5;
            }
            break;
    }
}

/* VS game (team) select flow (flowMode 4) */
void fn_1_6E9C(void)
{
    MDSPRITE_WORK *sprite = sys->sprite;
    MDMESS *mess = sys->mess;
    MDBEACH_STAR *star = sys->star;
    int i;
    switch(sys->subMode) {
        case 0:
            MdProcCall(MDBEACH_PROC_RESET, 0);
            sys->unk58 = 0;
            sys->subMode = 1;
            break;

        case 1:
            MdProcWait(MDBEACH_PROC_MODEL_POS, 1);
            MdProcCall(MDBEACH_PROC_RESET, 0);
            MdProcWait(MDBEACH_PROC_MODEL_SCALE, 0);
            MdProcCall(MDBEACH_PROC_RESET, 0);
            MD_WIN_OPEN_MAIN(mess);
            star->motNext = 2;
            MdMessWinMesSet(mess, 0, 0, sys->unk58 + 0x17000C, -1);
            MdMessWinMesSet(mess, 1, 0, 0x20004, -1);
            sys->unk2C4 = 3;
            MdProcWait(MDBEACH_PROC_CHR_SEL, 0);
            MdProcCall(MDBEACH_PROC_RESET, 0);
            sys->subMode = 2;
            break;

        case 2:
            if((sys->pad->DStkDown[0] & MD_PAD_LEFT) || (sys->pad->DStkDown[0] & MD_PAD_RIGHT)) {
                sys->unk59 = sys->unk58;
                if(sys->pad->DStkDown[0] & MD_PAD_LEFT) {
                    sys->unk58--;
                    if(sys->unk58 == -1) {
                        sys->unk58 = 2;
                    }
                    sys->unk2C0 = 0;
                } else if(sys->pad->DStkDown[0] & MD_PAD_RIGHT) {
                    sys->unk58 = (s8)((sys->unk58 + 1) % 3);
                    sys->unk2C0 = 1;
                }
                MdMessWinMesSet(mess, 0, 0, sys->unk58 + 0x17000C, -1);
                fn_1_15A40(2);
                MdProcWait(MDBEACH_PROC_MODEL_SCALE, 2);
                MdProcCall(MDBEACH_PROC_RESET, 0);
                break;
            }
            if(HuPadBtnDown[0] & MD_PAD_B) {
                fn_1_15A40(5);
                star->motNext = 3;
                MD_WIN_CLOSE_MAIN(mess);
                sys->unk2C4 = 3;
                MdProcWait(MDBEACH_PROC_CHR_SEL, 1);
                MdProcCall(MDBEACH_PROC_RESET, 0);
                MdProcWait(MDBEACH_PROC_MODEL_SCALE, 1);
                MdProcCall(MDBEACH_PROC_RESET, 0);
                MdProcWait(MDBEACH_PROC_MODEL_POS, 0);
                MdProcCall(MDBEACH_PROC_RESET, 0);
                if(sys->unk1B == 3) {
                    MD_WIN_OPEN_MAIN(mess);
                    star->motNext = 2;
                    MdMessWinMesSet(mess, 0, 0, 0x17000A, -1);
                    for(i = 0; i < 3; i++) {
                        if(sys->teamPlayer[i] != -1) {
                            HuSprAttrReset(sprite->grpId[MDSPR_GRP_CHAR], sys->teamPlayer[i], 4);
                            HuSprAttrSet(sprite->grpId[MDSPR_GRP_CAPMAN], sys->teamPlayer[i], 4);
                        }
                    }
                    MdMessWinMesSet(mess, 1, 0, 0x20004, -1);
                    MdMessWinMesSet(mess, 3, 0, 0x8002D, -1);
                    MdMessWinMesSet(mess, 4, 0, 0x8002D, -1);
                    sys->unk2C4 = 1;
                    MdProcWait(MDBEACH_PROC_CHR_SEL, 0);
                    MdProcCall(MDBEACH_PROC_RESET, 0);
                    sys->subMode = 2;
                    sys->flowMode = 3;
                    break;
                }
                MD_WIN_OPEN_MAIN(mess);
                star->motNext = 2;
                MdMessWinMesSet(mess, 0, 0, 0x170013, -1);
                for(i = 0; i < 4; i++) {
                    HuSprBankSet(sprite->grpId[MDSPR_GRP_CAPMAN], i, sys->unk3E[i]);
                }
                sys->selPlayerNo = 0;
                for(i = 0; i < 3; i++) {
                    if(sys->teamPlayer[i] != -1) {
                        HuSprAttrReset(sprite->grpId[MDSPR_GRP_CHAR], sys->teamPlayer[i], 4);
                        HuSprAttrSet(sprite->grpId[MDSPR_GRP_CAPMAN], sys->teamPlayer[i], 4);
                    }
                }
                MdMessWinMesSet(mess, 1, 0, 0x20004, -1);
                sys->unk2C4 = 2;
                MdProcWait(MDBEACH_PROC_CHR_SEL, 0);
                MdProcCall(MDBEACH_PROC_RESET, 0);
                HuSprAttrSet(sprite->grpId[MDSPR_GRP_CHAR], sys->unk55[sys->selPlayerNo], 4);
                HuSprAttrReset(sprite->grpId[MDSPR_GRP_CAPMAN], sys->unk55[sys->selPlayerNo], 4);
                fn_1_146D0();
                sys->subMode = 3;
                sys->flowMode = 3;
                break;
            }
            if(HuPadBtnDown[0] & MD_PAD_A) {
                fn_1_15A40(3);
                star->motNext = 3;
                MD_WIN_CLOSE_MAIN(mess);
                sys->unk2C4 = 3;
                MdProcWait(MDBEACH_PROC_CHR_SEL, 1);
                MdProcCall(MDBEACH_PROC_RESET, 0);
                sys->subMode = 0;
                sys->flowMode = 5;
            }
            break;
    }
}

/* ---- part D ---- */
/* partD: fn_1_79EC, fn_1_9A14, fn_1_9BAC */


/* ---------------- TYPES ---------------- */
typedef struct MdBeachCamLerpInfo_s {
    float start[6]; //0x00: hvr(3), center(3)
    float end[6]; //0x18
    int oscType; //0x30
    float oscSpeed; //0x34
} MDBEACH_CAM_LERP_INFO;

/* ---------------- PROTOS ---------------- */

/* ---------------- DATA ---------------- */
/* next DLL per selMode (lbl_1_data_148) */
static OMOVL MdBeachNextDll[2] = { DLL_m433dll, DLL_m580dll };

/* lbl_1_data_7B0 */
static MDBEACH_CAM_LERP_INFO MdBeachCamLerpTbl[1] = {
    { { -90.0f, -12.0f, 2500.0f, 0.0f, 400.0f, 1750.0f },
      { -90.0f, 0.0f, 2500.0f, 0.0f, 200.0f, -250.0f },
      MDMATH_OSC_90_REV, 30.0f }
};

#define MD_WAIT_WIN(win) while(!MdMessWinCheck(mess, (win))) HuPrcVSleep()

#define MD_MAIN_WIN_OPEN() do { \
    MdMessWinOpen(mess, 0, 0, 1, 0, 0, 56.0f, 376.0f, 0x1D0, 0x40, 1.0f); \
    MdMessWinPad1Set(mess, 0); \
    MD_WAIT_WIN(0); \
} while(0)

/* default main window text for the current select mode */
#define MD_MAIN_MES_DEFAULT() do { \
    if(sys->selMode == 0) { \
        MdMessWinMesSet(mess, 0, 0, 0x17000F, -1); \
    } else if(sys->selMode == 1) { \
        MdMessWinMesSet(mess, 0, 0, 0x18000B, -1); \
    } \
} while(0)

#define MD_SPR_BANK_UPDATE() do { \
    if(sys->selMode == 0) { \
        HuSprBankSet(sprite->grpId[MDSPR_GRP_MODE1], 1, sys->unk5A); \
    } else if(sys->selMode == 1) { \
        HuSprBankSet(sprite->grpId[MDSPR_GRP_MODE2], 1, sys->unk5A); \
    } \
} while(0)

void fn_1_79EC(void)
{
    MDMODEL_WORK *model = sys->model;
    MDSPRITE_WORK *sprite = sys->sprite;
    MDMESS *mess = sys->mess;
    MDBEACH_STAR *star = sys->star;
    MDBEACH_STATE_WORK *capWork = sys->capWork;
    MDBEACH_POS_WORK *posWork = sys->posWork;
    int i;
    int choice;

    switch(sys->subMode) {
        case 0:
            MdProcCall(MDBEACH_PROC_RESET, 0);
            if(sys->selMode == 0) {
                sys->unk5A = 0;
            } else if(sys->selMode == 1) {
                sys->unk5A = 0;
            }
            MD_SPR_BANK_UPDATE();
            sys->subMode = 1;
            break;

        case 1:
            MD_MAIN_WIN_OPEN();
            star->motNext = 2;
            MD_MAIN_MES_DEFAULT();
            MdProcWait(MDBEACH_PROC_CHR_GRP_POS, 0);
            MdProcCall(MDBEACH_PROC_RESET, 0);
            MdMessWinMesSet(mess, 1, 0, 0x20021, -1);
            sys->unk2C4 = 4;
            MdProcWait(MDBEACH_PROC_CHR_SEL, 0);
            MdProcCall(MDBEACH_PROC_RESET, 0);
            sys->subMode = 2;
            break;

        case 2:
            if((sys->pad->DStkDown[0] & 1) || (sys->pad->DStkDown[0] & 2)) {
                if(sys->pad->DStkDown[0] & 1) {
                    if(sys->selMode == 0) {
                        sys->unk5A--;
                        if(sys->unk5A == -1) {
                            sys->unk5A = 2;
                        }
                    } else if(sys->selMode == 1) {
                        sys->unk5A--;
                        if(sys->unk5A == -1) {
                            sys->unk5A = 2;
                        }
                    }
                    sys->unk2C0 = 0;
                } else if(sys->pad->DStkDown[0] & 2) {
                    if(sys->selMode == 0) {
                        sys->unk5A++;
                        sys->unk5A = sys->unk5A % 3;
                    } else if(sys->selMode == 1) {
                        sys->unk5A++;
                        sys->unk5A = sys->unk5A % 3;
                    }
                    sys->unk2C0 = 1;
                }
                fn_1_15A40(2);
                MD_SPR_BANK_UPDATE();
                sys->unk2C4 = 4;
                MdProcWait(MDBEACH_PROC_CHR_SEL, sys->unk2C0 + 2);
                MdProcCall(MDBEACH_PROC_RESET, 0);
                break;
            }
            if(HuPadBtnDown[0] & PAD_BUTTON_X) {
                fn_1_15A40(5);
                MdMessWinHomeClear(mess, 1);
                if(sys->selMode == 0) {
                    MdMessWinMesSet(mess, 0, 1, 0x170003, -1);
                } else if(sys->selMode == 1) {
                    MdMessWinMesSet(mess, 0, 1, 0x180003, -1);
                }
                MD_WAIT_WIN(0);
                choice = MdMessWinChoiceGet(mess, 0);
                if(choice == -1 || choice == 1) {
                    MD_MAIN_MES_DEFAULT();
                    MdMessWinMesSet(mess, 1, 0, 0x20021, -1);
                } else if(choice == 0) {
                    star->motNext = 3;
                    MdMessWinHomeClear(mess, 0);
                    sys->unk2C4 = 4;
                    MdProcWait(MDBEACH_PROC_CHR_SEL, 1);
                    MdProcCall(MDBEACH_PROC_RESET, 0);
                    MdMessWinClose(mess, 0);
                    MD_WAIT_WIN(0);
                    WipeColorSet(0xFF, 0xFF, 0xFF);
                    WipeCreate(2, 0, 30);
                    fn_1_15A40(1);
                    WipeWait();
                    if(sys->evtNo == 0 || sys->evtNo == 1) {
                        while(sys->loadState != MD_LOAD_STATE_WAIT) {
                            HuPrcVSleep();
                        }
                    }
                    mgPauseExitF = 0;
                    sys->ovlCallMode = MD_OVLCALL_RETURN;
                    sys->nextDll = DLL_mdomakedll; /* 0x5E */
                    sys->skipWipeF = TRUE;
                    sys->exitF = TRUE;
                }
            } else if(HuPadBtnDown[0] & PAD_BUTTON_B) {
                fn_1_15A40(5);
                star->motNext = 3;
                MdMessWinClose(mess, 0);
                MD_WAIT_WIN(0);
                MdMessWinHomeClear(mess, 1);
                sys->unk2C4 = 4;
                MdProcWait(MDBEACH_PROC_CHR_SEL, 1);
                MdProcCall(MDBEACH_PROC_RESET, 0);
                MdProcWait(MDBEACH_PROC_CHR_GRP_POS, 1);
                MdProcCall(MDBEACH_PROC_RESET, 0);
                if(sys->selMode == 0) {
                    MD_MAIN_WIN_OPEN();
                    star->motNext = 2;
                    MdMessWinMesSet(mess, 0, 0, 0x17000C + sys->unk58, -1);
                    MdMessWinMesSet(mess, 1, 0, 0x20004, -1);
                    sys->unk2C4 = 3;
                    MdProcWait(MDBEACH_PROC_CHR_SEL, 0);
                    MdProcCall(MDBEACH_PROC_RESET, 0);
                    sys->subMode = 2;
                    sys->flowMode = 4;
                } else if(sys->selMode == 1) {
                    if(sys->unk1B == 3) {
                        MD_MAIN_WIN_OPEN();
                        star->motNext = 2;
                        MdMessWinMesSet(mess, 0, 0, 0x17000A, -1);
                        for(i = 0; i < 3; i++) {
                            if(sys->teamPlayer[i] != -1) {
                                HuSprAttrReset(sprite->grpId[MDSPR_GRP_CHAR], sys->teamPlayer[i], 4);
                                HuSprAttrSet(sprite->grpId[MDSPR_GRP_CAPMAN], sys->teamPlayer[i], 4);
                            }
                        }
                        MdMessWinMesSet(mess, 1, 0, 0x20004, -1);
                        MdMessWinMesSet(mess, 3, 0, 0x8002D, -1);
                        MdMessWinMesSet(mess, 4, 0, 0x8002D, -1);
                        sys->unk2C4 = 1;
                        MdProcWait(MDBEACH_PROC_CHR_SEL, 0);
                        MdProcCall(MDBEACH_PROC_RESET, 0);
                        sys->subMode = 2;
                        sys->flowMode = 3;
                    } else {
                        MD_MAIN_WIN_OPEN();
                        star->motNext = 2;
                        MdMessWinMesSet(mess, 0, 0, 0x170013, -1);
                        for(i = 0; i < 4; i++) {
                            HuSprBankSet(sprite->grpId[MDSPR_GRP_CAPMAN], i, sys->unk3E[i]);
                        }
                        sys->selPlayerNo = 0;
                        for(i = 0; i < 3; i++) {
                            if(sys->teamPlayer[i] != -1) {
                                HuSprAttrReset(sprite->grpId[MDSPR_GRP_CHAR], sys->teamPlayer[i], 4);
                                HuSprAttrSet(sprite->grpId[MDSPR_GRP_CAPMAN], sys->teamPlayer[i], 4);
                            }
                        }
                        MdMessWinMesSet(mess, 1, 0, 0x20004, -1);
                        sys->unk2C4 = 2;
                        MdProcWait(MDBEACH_PROC_CHR_SEL, 0);
                        MdProcCall(MDBEACH_PROC_RESET, 0);
                        HuSprAttrSet(sprite->grpId[MDSPR_GRP_CHAR], sys->unk55[sys->selPlayerNo], 4);
                        HuSprAttrReset(sprite->grpId[MDSPR_GRP_CAPMAN], sys->unk55[sys->selPlayerNo], 4);
                        fn_1_146D0();
                        sys->subMode = 3;
                        sys->flowMode = 3;
                    }
                }
            } else if(HuPadBtnDown[0] & PAD_BUTTON_A) {
                MdMessWinHomeClear(mess, 1);
                fn_1_15A40(3);
                sys->unk2C4 = 4;
                MdProcWait(MDBEACH_PROC_CHR_SEL, 1);
                MdProcCall(MDBEACH_PROC_RESET, 0);
                MdMessWinMesSet(mess, 0, 1, 0x170010, -1);
                MD_WAIT_WIN(0);
                choice = MdMessWinChoiceGet(mess, 0);
                if(choice == -1 || choice == 1) {
                    MdMessWinMesSet(mess, 0, 1, 0x170011, -1);
                    MD_WAIT_WIN(0);
                    choice = MdMessWinChoiceGet(mess, 0);
                    if(choice == -1 || choice == 1) {
                        MD_MAIN_MES_DEFAULT();
                        MdMessWinMesSet(mess, 1, 0, 0x20021, -1);
                        sys->unk2C4 = 4;
                        MdProcWait(MDBEACH_PROC_CHR_SEL, 0);
                        MdProcCall(MDBEACH_PROC_RESET, 0);
                        sys->subMode = 2;
                    } else if(choice == 0) {
                        star->motNext = 3;
                        MdMessWinClose(mess, 0);
                        MD_WAIT_WIN(0);
                        for(i = 0; i < 4; i++) {
                            posWork[sys->chrNo[i]].mode = 4;
                        }
                        WipeColorSet(0, 0, 0);
                        WipeCreate(2, 0, 30);
                        WipeWait();
                        MdSpriteDispSet(sprite, MDSPR_GRP_CHAR, FALSE);
                        MdSpriteDispSet(sprite, MDSPR_GRP_CAPMAN, FALSE);
                        MdSpriteDispSet(sprite, MDSPR_GRP_MODE0, FALSE);
                        MdSpriteDispSet(sprite, MDSPR_GRP_MODE1, FALSE);
                        MdSpriteDispSet(sprite, MDSPR_GRP_MODE2, FALSE);
                        for(i = 0; i < MDMODEL_CHAR_NUM; i++) {
                            Hu3DModelAttrSet(model->mdlId[MDMODEL_CHAR_START + i], HU3D_ATTR_DISPOFF);
                        }
                        for(i = 0; i < MDMODEL_VOLLEY_NUM; i++) {
                            Hu3DModelAttrSet(model->mdlId[MDMODEL_VOLLEY_START + i], HU3D_ATTR_DISPOFF);
                        }
                        for(i = 0; i < MDMODEL_HOCKEY_NUM; i++) {
                            Hu3DModelAttrSet(model->mdlId[MDMODEL_HOCKEY_START + i], HU3D_ATTR_DISPOFF);
                        }
                        for(i = 0; i < 2; i++) {
                            Hu3DModelAttrSet(model->mdlId[MDMODEL_VS_VOLLEY + i], HU3D_ATTR_DISPOFF);
                        }
                        for(i = 0; i < MDMODEL_BALL_NUM; i++) {
                            Hu3DModelAttrSet(model->mdlId[MDMODEL_BALL_START + i], HU3D_ATTR_DISPOFF);
                        }
                        MdMessWinPosSet(mess, 3, 288.0f, -64.0f);
                        MdMessWinPosSet(mess, 4, 288.0f, -64.0f);
                        MdMessWinHomeClear(mess, 3);
                        MdMessWinHomeClear(mess, 4);
                        WipeColorSet(0, 0, 0);
                        WipeCreate(1, 0, 30);
                        WipeWait();
                        fn_1_13CAC();
                        sys->unk1C = sys->unk1B;
                        MdProcWait(MDBEACH_PROC_GAME_INIT, 0);
                        MdProcCall(MDBEACH_PROC_RESET, 0);
                        MD_MAIN_WIN_OPEN();
                        star->motNext = 2;
                        MdMessWinMesSet(mess, 0, 0, 0x170004 + sys->unk1B, -1);
                        MdMessWinMesSet(mess, 1, 0, 0x20004, -1);
                        if(sys->unk1D > 1) {
                            sys->unk2C4 = 0;
                            MdProcWait(MDBEACH_PROC_CHR_SEL, 0);
                        }
                        sys->subMode = 2;
                        sys->flowMode = 1;
                    }
                } else if(choice == 0) {
                    fn_1_15508();
                    sys->loadProcReady = 1;
                    star->motNext = 3;
                    MdMessWinClose(mess, 0);
                    MD_WAIT_WIN(0);
                    WipeColorSet(0xFF, 0xFF, 0xFF);
                    WipeCreate(2, 0, 60);
                    fn_1_15A40(1);
                    while(WipeCheckEnd()) {
                        MdProcCall(MDBEACH_PROC_ORDER_SEL, 5);
                        HuPrcVSleep();
                    }
                    while(sys->loadState != MD_LOAD_STATE_END) {
                        HuPrcVSleep();
                    }
                    omOvlHisChg(0, sys->ovlHis->ovl, sys->selMode + 2, sys->ovlHis->stat);
                    sys->ovlCallMode = MD_OVLCALL_GOTO;
                    sys->nextDll = MdBeachNextDll[sys->selMode];
                    sys->skipWipeF = TRUE;
                    sys->exitF = TRUE;
                }
            }
            break;

        case 10:
            fn_1_13EA4();
            fn_1_14050();
            fn_1_14354();
            sys->unk1C = sys->unk1B;
            MdMessWinOpen(mess, 3, 1, 0, 0, 3, 288.0f, -64.0f, 0xE0, 0x20, 0.0f);
            MdMessWinOpen(mess, 4, 1, 0, 0, 4, 288.0f, -64.0f, 0xE0, 0x20, 0.0f);
            MdMessWinSpeedSet(mess, 3, 0);
            MdMessWinSpeedSet(mess, 4, 0);
            MdMessWinAttrSet(mess, 3, HUWIN_ATTR_ALIGN_CENTER);
            MdMessWinAttrSet(mess, 4, HUWIN_ATTR_ALIGN_CENTER);
            MdMessWinMesSet(mess, 3, 0, 0x80000 + sys->unk53[0], -1);
            MdMessWinMesSet(mess, 4, 0, 0x80000 + sys->unk53[1], -1);
            star->mode = 10;
            star->motNext = 1;
            MdProcCall(MDBEACH_PROC_CHR_POS, 4);
            MdProcCall(MDBEACH_PROC_RESET, 0);
            MdProcCall(MDBEACH_PROC_CHR_GRP_POS, 3);
            MdProcCall(MDBEACH_PROC_RESET, 0);
            MD_SPR_BANK_UPDATE();
            if(sys->selMode == 0) {
                MdProcCall(MDBEACH_PROC_MODEL_SCALE, 3);
                MdProcCall(MDBEACH_PROC_RESET, 0);
            } else if(sys->selMode == 1) {
                MdProcCall(MDBEACH_PROC_MODEL_POS, 2);
                MdProcCall(MDBEACH_PROC_RESET, 0);
            }
            MdProcCall(MDBEACH_PROC_GAME_SEL, 4);
            MdProcCall(MDBEACH_PROC_RESET, 0);
            MdProcCall(MDBEACH_PROC_ORDER_SEL, 6);
            MdProcCall(MDBEACH_PROC_RESET, 0);
            for(i = 0; i < 4; i++) {
                capWork[i].mode = 3;
                if(sys->padEnable[i] != 0) {
                    HuSprAttrReset(sprite->grpId[MDSPR_GRP_CHAR], i, 4);
                }
            }
            for(i = 0; i < 3; i++) {
                if(sys->unk55[i] != -1) {
                    HuSprAttrReset(sprite->grpId[MDSPR_GRP_CAPMAN], sys->unk55[i], 4);
                    HuSprBankSet(sprite->grpId[MDSPR_GRP_CAPMAN], sys->unk55[i], sys->unk3E[sys->unk55[i]]);
                }
            }
            HuPrcVSleep();
            WipeColorSet(0xFF, 0xFF, 0xFF);
            WipeCreate(1, 5, 30);
            fn_1_15A40(0);
            WipeWait();
            MdProcWait(MDBEACH_PROC_CAPMAN_SCALE, 0);
            MdProcCall(MDBEACH_PROC_RESET, 0);
            MdMessWinOpen(mess, 0, 0, 1, 0, 0, 56.0f, 376.0f, 0x1D0, 0x40, 1.0f);
            MdMessWinPad1Set(mess, 0);
            MdMessWinOpen(mess, 1, 1, 0, 0, 1, 32.0f, 340.0f, 0x200, 0x20, 0.0f);
            MdMessWinAttrSet(mess, 1, HUWIN_ATTR_ALIGN_CENTER);
            MdMessWinSpeedSet(mess, 1, 0);
            MD_WAIT_WIN(0);
            star->motNext = 2;
            MD_MAIN_MES_DEFAULT();
            MdMessWinMesSet(mess, 1, 0, 0x20021, -1);
            sys->unk2C4 = 4;
            MdProcWait(MDBEACH_PROC_CHR_SEL, 0);
            MdProcCall(MDBEACH_PROC_RESET, 0);
            sys->subMode = 2;
            break;
    }
}

/* reset camera-lerp oscillator state */
BOOL fn_1_9A14(int param)
{
    sys->unk2A8 = 0;
    sys->unk2AC = 0;
    sys->unk2B0 = 0;
    sys->unk2B4 = 0;
    sys->unk2B8 = 0;
    sys->unk2BC = 0;
    sys->oscIn[0] = 0.0f;
    sys->oscIn[1] = 0.0f;
    sys->oscIn[2] = 0.0f;
    sys->oscIn[3] = 0.0f;
    sys->oscIn[4] = 0.0f;
    sys->oscOut[0] = 0.0f;
    sys->oscOut[1] = 0.0f;
    sys->oscOut[2] = 0.0f;
    sys->oscOut[3] = 0.0f;
    sys->oscOut[4] = 0.0f;
    return FALSE;
}

/* lerp main camera between two preset hvr/center values; returns TRUE when done */
BOOL fn_1_9BAC(int param)
{
    MDCAMERA_WORK *cameraWork = sys->camera;
    MDCAMERA *camera = &cameraWork->camera[0];
    MDBEACH_CAM_LERP_INFO *info = &MdBeachCamLerpTbl[param];

    MdMathOscillate(info->oscType, &sys->oscIn[0], &sys->oscOut[0], info->oscSpeed);
    MdMathOscillate(MDMATH_OSC_180, &sys->oscIn[1], &sys->oscOut[1], info->oscSpeed);
    MdMathLerp(&camera->hvr.x, info->start[0], info->end[0], sys->oscOut[0]);
    MdMathLerp(&camera->hvr.y, info->start[1], info->end[1], sys->oscOut[0]);
    MdMathLerp(&camera->hvr.z, info->start[2], info->end[2], sys->oscOut[0]);
    MdMathLerp(&camera->center.x, info->start[3], info->end[3], sys->oscOut[0]);
    MdMathLerp(&camera->center.y, info->start[4], info->end[4], sys->oscOut[0]);
    MdMathLerp(&camera->center.z, info->start[5], info->end[5], sys->oscOut[0]);
    if(sys->oscIn[0] >= 1.0f) {
        return TRUE;
    }
    return FALSE;
}

/* ---- part E ---- */
// fn_1_9D58 fn_1_B1E8

/* PROTOS */

/* DATA */
static HuVecF cursorPos[4]; // lbl_1_bss_660
static s32 winState; // lbl_1_bss_65C
static float winPosY0; // lbl_1_bss_658
static float winPosY1; // lbl_1_bss_654
static float winScaleY; // lbl_1_bss_650

static void CursorPosSet(float y, float x1, float x3)
{
    cursorPos[0].x = -64.0f;
    cursorPos[0].y = y;
    cursorPos[0].z = 0.0f;
    cursorPos[1].x = x1;
    cursorPos[1].y = y;
    cursorPos[1].z = 0.0f;
    cursorPos[2].x = 640.0f;
    cursorPos[2].y = y;
    cursorPos[2].z = 0.0f;
    cursorPos[3].x = x3;
    cursorPos[3].y = y;
    cursorPos[3].z = 0.0f;
}

static void CursorScaleSet(HuVec2f *scale, float v)
{
    scale->y = v;
    scale->x = v;
}

static void CursorScaleLerp(HuVec2f *scale, float t)
{
    if (sys->unk2C4 == 2) {
        MdMathLerp(&scale->x, 0.7f, 1.0f, t);
        MdMathLerp(&scale->y, 0.7f, 1.0f, t);
    } else {
        MdMathLerp(&scale->x, 1.0f, 1.5f, t);
        MdMathLerp(&scale->y, 1.0f, 1.5f, t);
    }
}

// Cursor (left/right arrows) slide animation: param 0 = in, 1 = out, 2 = left bounce, 3 = right bounce
BOOL fn_1_9D58(int param)
{
    MDSPRITE_WORK *sprite = sys->sprite;
    MDCAMERA_WORK *camera = sys->camera;
    HUSPRGRPID grpId = sprite->grpId[MDSPR_GRP_CURSOR_LR];
    float scaleInit;
    HuVec2f *posP, *scaleP;
    HuVecF pos3D, pos2D;
    HuVec2f *p;

    if (sys->unk2B8 == 0) {
        switch (sys->unk2C4) {
            case 0:
                pos3D.x = 0.0f;
                pos3D.y = 176.8f;
                pos3D.z = -580.0f;
                Hu3D3Dto2D(&pos3D, camera->camera[0].bit, &pos2D);
                CursorPosSet(pos2D.y, pos2D.x - 192.0f, 192.0f + pos2D.x);
                break;
            case 1:
                CursorPosSet(192.0f, 48.0f, 528.0f);
                break;
            case 2:
                p = MdSpritePosGet(sprite, MDSPR_GRP_CHAR, sys->unk55[sys->selPlayerNo]);
                pos2D.x = p->x;
                pos2D.y = p->y;
                pos2D.y -= 26.0f;
                CursorPosSet(pos2D.y, pos2D.x - 46.0f, (46.0f + pos2D.x) - 3.0f);
                break;
            case 3:
                CursorPosSet(188.0f, 208.0f, 368.0f);
                break;
            case 4:
                if (sys->selMode == 0) {
                    CursorPosSet(88.0f, 224.0f, 352.0f);
                } else if (sys->selMode == 1) {
                    CursorPosSet(90.0f, 232.0f, 341.0f);
                }
                break;
            default:
                CursorPosSet(240.0f, 128.0f, 448.0f);
                break;
        }
        scaleInit = (sys->unk2C4 == 2) ? 0.7f : 1.0f;
        switch (param) {
            case 0:
                HuSprGrpPosSet(grpId, 0.0f, 0.0f);
                posP = MdSpritePosGet(sprite, MDSPR_GRP_CURSOR_UDLR, 0);
                scaleP = MdSpriteScaleGet(sprite, MDSPR_GRP_CURSOR_UDLR, 0);
                posP->x = cursorPos[0].x;
                posP->y = cursorPos[0].y;
                CursorScaleSet(scaleP, scaleInit);
                HuSprAttrReset(grpId, 0, 4);
                posP = MdSpritePosGet(sprite, MDSPR_GRP_CURSOR_UDLR, 1);
                scaleP = MdSpriteScaleGet(sprite, MDSPR_GRP_CURSOR_UDLR, 1);
                posP->x = cursorPos[2].x;
                posP->y = cursorPos[2].y;
                CursorScaleSet(scaleP, scaleInit);
                HuSprAttrReset(grpId, 1, 4);
                break;
            case 2:
                posP = MdSpritePosGet(sprite, MDSPR_GRP_CURSOR_UDLR, 4);
                scaleP = MdSpriteScaleGet(sprite, MDSPR_GRP_CURSOR_UDLR, 0); // sic (original uses member 0)
                posP->x = cursorPos[1].x;
                posP->y = cursorPos[1].y;
                CursorScaleSet(scaleP, scaleInit);
                HuSprTPLvlSet(grpId, 4, 0.0f);
                HuSprAttrReset(grpId, 4, 4);
                break;
            case 3:
                posP = MdSpritePosGet(sprite, MDSPR_GRP_CURSOR_UDLR, 5);
                scaleP = MdSpriteScaleGet(sprite, MDSPR_GRP_CURSOR_UDLR, 1); // sic (original uses member 1)
                posP->x = cursorPos[3].x;
                posP->y = cursorPos[3].y;
                CursorScaleSet(scaleP, scaleInit);
                HuSprTPLvlSet(grpId, 5, 0.0f);
                HuSprAttrReset(grpId, 5, 4);
                break;
        }
        sys->unk2B8 = 1;
    }
    switch (param) {
        case 0:
            MdMathOscillate(MDMATH_OSC_90, &sys->oscIn[4], &sys->oscOut[4], 15.0f);
            MdMathLerp(&MdSpritePosGet(sprite, MDSPR_GRP_CURSOR_UDLR, 0)->x, cursorPos[0].x, cursorPos[1].x, sys->oscOut[4]);
            MdMathLerp(&MdSpritePosGet(sprite, MDSPR_GRP_CURSOR_UDLR, 1)->x, cursorPos[2].x, cursorPos[3].x, sys->oscOut[4]);
            break;
        case 1:
            MdMathOscillate(MDMATH_OSC_90, &sys->oscIn[4], &sys->oscOut[4], 15.0f);
            MdMathLerp(&MdSpritePosGet(sprite, MDSPR_GRP_CURSOR_UDLR, 0)->x, cursorPos[1].x, cursorPos[0].x, sys->oscOut[4]);
            MdMathLerp(&MdSpritePosGet(sprite, MDSPR_GRP_CURSOR_UDLR, 1)->x, cursorPos[3].x, cursorPos[2].x, sys->oscOut[4]);
            break;
        case 2:
            MdMathOscillate(MDMATH_OSC_180, &sys->oscIn[4], &sys->oscOut[4], 15.0f);
            posP = MdSpritePosGet(sprite, MDSPR_GRP_CURSOR_UDLR, 0);
            scaleP = MdSpriteScaleGet(sprite, MDSPR_GRP_CURSOR_UDLR, 0);
            MdMathLerp(&posP->x, cursorPos[1].x, cursorPos[1].x - 24.0f, sys->oscOut[4]);
            CursorScaleLerp(scaleP, sys->oscOut[4]);
            posP = MdSpritePosGet(sprite, MDSPR_GRP_CURSOR_UDLR, 4);
            scaleP = MdSpriteScaleGet(sprite, MDSPR_GRP_CURSOR_UDLR, 4);
            MdMathLerp(&posP->x, cursorPos[1].x, cursorPos[1].x - 24.0f, sys->oscOut[4]);
            CursorScaleLerp(scaleP, sys->oscOut[4]);
            HuSprTPLvlSet(grpId, 4, sys->oscOut[4]);
            break;
        case 3:
            MdMathOscillate(MDMATH_OSC_180, &sys->oscIn[4], &sys->oscOut[4], 15.0f);
            posP = MdSpritePosGet(sprite, MDSPR_GRP_CURSOR_UDLR, 1);
            scaleP = MdSpriteScaleGet(sprite, MDSPR_GRP_CURSOR_UDLR, 1);
            MdMathLerp(&posP->x, cursorPos[3].x, 24.0f + cursorPos[3].x, sys->oscOut[4]);
            CursorScaleLerp(scaleP, sys->oscOut[4]);
            posP = MdSpritePosGet(sprite, MDSPR_GRP_CURSOR_UDLR, 5);
            scaleP = MdSpriteScaleGet(sprite, MDSPR_GRP_CURSOR_UDLR, 5);
            MdMathLerp(&posP->x, cursorPos[3].x, 24.0f + cursorPos[3].x, sys->oscOut[4]);
            CursorScaleLerp(scaleP, sys->oscOut[4]);
            HuSprTPLvlSet(grpId, 5, sys->oscOut[4]);
            break;
    }
    if (sys->oscIn[4] >= 1.0f) {
        switch (param) {
            case 1:
                MdSpriteDispSet(sprite, MDSPR_GRP_CURSOR_UDLR, FALSE);
                break;
            case 2:
                HuSprAttrSet(grpId, 4, 4);
                break;
            case 3:
                HuSprAttrSet(grpId, 5, 4);
                break;
        }
        return TRUE;
    }
    return FALSE;
}

// Window frame open (param 0) / close (param 1) animation
BOOL fn_1_B1E8(int param)
{
    MDSPRITE_WORK *sprite = sys->sprite;
    HuVec2f *pos0 = MdSpritePosGet(sprite, MDSPR_GRP_WIN, 0);
    HuVec2f *pos1 = MdSpritePosGet(sprite, MDSPR_GRP_WIN, 1);
    HuVec2f *scale0 = MdSpriteScaleGet(sprite, MDSPR_GRP_WIN, 0);
    HuVec2f *scale1 = MdSpriteScaleGet(sprite, MDSPR_GRP_WIN, 1);
    HuVec2f *scale2 = MdSpriteScaleGet(sprite, MDSPR_GRP_WIN, 2);

    if (sys->unk2A8 == 0) {
        winState = 0;
        winPosY0 = pos0->y;
        winPosY1 = pos1->y;
        winScaleY = scale2->y;
        if (param == 0) {
            pos0->y = 0.0f;
            pos1->y = 0.0f;
            scale0->x = 0.0f;
            scale1->x = 0.0f;
            scale2->x = 0.0f;
            scale2->y = 0.0f;
            MdSpriteDispSet(sprite, MDSPR_GRP_WIN, TRUE);
        }
        sys->unk2A8 = 1;
    }
    if (param == 0) {
        if (winState == 0) {
            MdMathOscillate(MDMATH_OSC_90, &sys->oscIn[0], &sys->oscOut[0], 12.0f);
            MdMathLerp(&pos0->y, 0.0f, -2.0f, sys->oscOut[0]);
            MdMathLerp(&pos1->y, 0.0f, 2.0f, sys->oscOut[0]);
            MdMathLerp(&scale0->x, 0.0f, 72.0f, sys->oscOut[0]);
            MdMathLerp(&scale1->x, 0.0f, 72.0f, sys->oscOut[0]);
            MdMathLerp(&scale2->x, 0.0f, 1.0f, sys->oscOut[0]);
            if (sys->oscIn[0] >= 1.0f) {
                sys->oscIn[0] = 0.0f;
                winState = 1;
            }
        } else if (winState == 1) {
            MdMathOscillate(MDMATH_OSC_90, &sys->oscIn[0], &sys->oscOut[0], 15.0f);
            MdMathLerp(&pos0->y, -2.0f, -28.0f, sys->oscOut[0]);
            MdMathLerp(&pos1->y, 2.0f, 32.0f, sys->oscOut[0]);
            MdMathLerp(&scale2->y, 0.0f, 8.0f, sys->oscOut[0]);
            if (sys->oscIn[0] >= 1.0f) {
                winState = 2;
            }
        }
    } else if (param == 1) {
        if (winState == 0) {
            MdMathOscillate(MDMATH_OSC_90, &sys->oscIn[0], &sys->oscOut[0], 15.0f);
            MdMathLerp(&pos0->y, winPosY0, -2.0f, sys->oscOut[0]);
            MdMathLerp(&pos1->y, winPosY1, 2.0f, sys->oscOut[0]);
            MdMathLerp(&scale2->y, winScaleY, 0.0f, sys->oscOut[0]);
            if (sys->oscIn[0] >= 1.0f) {
                sys->oscIn[0] = 0.0f;
                winState = 1;
            }
        } else if (winState == 1) {
            MdMathOscillate(MDMATH_OSC_90, &sys->oscIn[0], &sys->oscOut[0], 12.0f);
            MdMathLerp(&pos0->y, -2.0f, 0.0f, sys->oscOut[0]);
            MdMathLerp(&pos1->y, 2.0f, 0.0f, sys->oscOut[0]);
            MdMathLerp(&scale0->x, 72.0f, 0.0f, sys->oscOut[0]);
            MdMathLerp(&scale1->x, 72.0f, 0.0f, sys->oscOut[0]);
            MdMathLerp(&scale2->x, 1.0f, 0.0f, sys->oscOut[0]);
            if (sys->oscIn[0] >= 1.0f) {
                winState = 2;
            }
        }
    }
    if (winState == 2) {
        if (param == 1) {
            MdSpriteDispSet(sprite, MDSPR_GRP_WIN, FALSE);
        }
        return TRUE;
    }
    return FALSE;
}

/* ---- part F ---- */
// fn_1_B91C fn_1_D45C

/* ---------------------------------------------------------------- TYPES */

typedef struct MdBeachSlideWork_s {
    s16 mode; //0x00 0: wait, 1: moving, 2: done
    s16 timer; //0x02 start delay
    f32 in; //0x04 oscillation input
    f32 out; //0x08 oscillation output
} MDBEACH_SLIDE_WORK;

typedef struct MdBeachPosPair_s {
    HuVecF a; //0x00
    HuVecF b; //0x0C
} MDBEACH_POS_PAIR;

/* --------------------------------------------------------------- PROTOS */


/* ----------------------------------------------------------------- DATA */

// fn_1_B91C
static HuVecF capScreenBase; // lbl_1_data_7E8
static HuVecF capScreenIn[4]; // lbl_1_data_7F4
static u8 capInitF; // lbl_1_bss_4
static MDBEACH_SLIDE_WORK capSlide[4]; // lbl_1_bss_574
static HuVecF capOrbitBase[4]; // lbl_1_bss_5A4
static f32 capOrbitAngle; // lbl_1_bss_5D4
static f32 capOrbitRadius; // lbl_1_bss_5D8
static s8 capSel[4][2]; // lbl_1_bss_5DC
static HuVecF capSide3D[2]; // lbl_1_bss_5E4
static HuVecF capIn3D[4]; // lbl_1_bss_5FC
static HuVecF capBase3D; // lbl_1_bss_62C
static HuVecF capSide2D[2]; // lbl_1_bss_638

// fn_1_D45C
static s16 charOrder[10] = { 5, 9, 4, 8, 3, 2, 1, 7, 0, 6 }; // lbl_1_data_824
static HuVecF charScreenPos[2][10] = { // lbl_1_data_158
    {
        { 70.0f, 204.0f, 450.0f },
        { 150.0f, 240.0f, 450.0f },
        { 229.0f, 224.0f, 450.0f },
        { 328.0f, 179.0f, 450.0f },
        { 413.0f, 245.0f, 450.0f },
        { 514.0f, 215.0f, 450.0f },
        { 80.0f, 350.0f, 450.0f },
        { 190.0f, 359.0f, 450.0f },
        { 393.0f, 370.0f, 450.0f },
        { 508.0f, 353.0f, 450.0f },
    },
    {
        { 76.0f, 194.0f, 450.0f },
        { 138.0f, 265.0f, 450.0f },
        { 234.0f, 216.0f, 450.0f },
        { 322.0f, 194.0f, 450.0f },
        { 400.0f, 255.0f, 450.0f },
        { 499.0f, 211.0f, 450.0f },
        { 66.0f, 354.0f, 450.0f },
        { 183.0f, 365.0f, 450.0f },
        { 392.0f, 374.0f, 450.0f },
        { 497.0f, 358.0f, 450.0f },
    },
};
static MDBEACH_SLIDE_WORK charSlide[10]; // lbl_1_bss_318
static s16 charLastNo; // lbl_1_bss_390
static MDBEACH_POS_PAIR charPos3D[10]; // lbl_1_bss_394
static MDBEACH_POS_PAIR charPos2D[10]; // lbl_1_bss_484

/* ------------------------------------------------------------ FUNCTIONS */

// capman/capcom (character group) slide-in/out and orbit animation
BOOL fn_1_B91C(int param)
{
    MDMODEL_WORK *model = sys->model;
    MDCAMERA_WORK *camera = sys->camera;
    HuVecF *posP[4][2];
    HuVecF *rotP[4][2];
    HuVecF *scaleP[4][2];
    int i;

    if (capInitF == 0) {
        capScreenBase.x = 288.0f;
        capScreenBase.y = 176.64f;
        capScreenBase.z = 4000.0f;
        for (i = 0; i < 4; i++) {
            capScreenIn[i].x = 168.0f + 80.0f * i;
            capScreenIn[i].y = capScreenBase.y;
            capScreenIn[i].z = 2500.0f;
        }
        capSide2D[0].x = -64.0f;
        capSide2D[0].y = capScreenBase.y;
        capSide2D[0].z = 2500.0f;
        capSide2D[1].x = 704.0f;
        capSide2D[1].y = capScreenBase.y;
        capSide2D[1].z = 2500.0f;
        capInitF = 1;
    }
    for (i = 0; i < 4; i++) {
        posP[i][0] = model->posP[MDMODEL_CAPMAN_START + i];
        posP[i][1] = model->posP[MDMODEL_CAPCOM_START + i];
        rotP[i][0] = model->rotP[MDMODEL_CAPMAN_START + i];
        rotP[i][1] = model->rotP[MDMODEL_CAPCOM_START + i];
        scaleP[i][0] = model->scaleP[MDMODEL_CAPMAN_START + i];
        scaleP[i][1] = model->scaleP[MDMODEL_CAPCOM_START + i];
    }
    if (sys->unk2A8 == 0) {
        for (i = 0; i < 4; i++) {
            capSel[i][0] = (i < sys->unk1B + 1) ? 0 : 1;
            capSel[i][1] = (i < sys->unk1C + 1) ? 0 : 1;
        }
        for (i = 0; i < 4; i++) {
            capSlide[i].mode = 0;
            capSlide[i].timer = 0;
            capSlide[i].in = 0.0f;
            capSlide[i].out = 0.0f;
        }
        switch (param) {
            case 0:
                capSlide[0].timer = 12;
                capSlide[1].timer = 8;
                capSlide[2].timer = 4;
                capSlide[3].timer = 0;
                Hu3D2Dto3D(&capScreenBase, camera->camera[0].bit, &capBase3D);
                for (i = 0; i < 4; i++) {
                    Hu3D2Dto3D(&capScreenIn[i], camera->camera[0].bit, &capIn3D[i]);
                }
                for (i = 0; i < 2; i++) {
                    Hu3D2Dto3D(&capSide2D[i], camera->camera[0].bit, &capSide3D[i]);
                }
                for (i = 0; i < 4; i++) {
                    *posP[i][capSel[i][0]] = capBase3D;
                    rotP[i][capSel[i][0]]->x = 0.0f;
                    rotP[i][capSel[i][0]]->y = 0.0f;
                    rotP[i][capSel[i][0]]->z = 0.0f;
                    scaleP[i][capSel[i][0]]->x = 0.0f;
                    scaleP[i][capSel[i][0]]->y = 0.0f;
                    scaleP[i][capSel[i][0]]->z = 0.0f;
                    if (capSel[i][0] == 0) {
                        Hu3DModelAttrReset(model->mdlId[MDMODEL_CAPMAN_START + i], 1);
                        Hu3DModelAttrSet(model->mdlId[MDMODEL_CAPCOM_START + i], 1);
                    } else if (capSel[i][0] == 1) {
                        Hu3DModelAttrSet(model->mdlId[MDMODEL_CAPMAN_START + i], 1);
                        Hu3DModelAttrReset(model->mdlId[MDMODEL_CAPCOM_START + i], 1);
                    }
                }
                fn_1_15A40(6);
                break;
            case 1:
                capSlide[0].timer = 12;
                capSlide[1].timer = 8;
                capSlide[2].timer = 4;
                capSlide[3].timer = 0;
                fn_1_15A40(6);
                break;
            case 2:
            case 3:
                for (i = 0; i < 4; i++) {
                    if (capSel[i][0] != capSel[i][1]) {
                        *posP[i][capSel[i][1]] = capBase3D;
                        rotP[i][capSel[i][1]]->x = 0.0f;
                        rotP[i][capSel[i][1]]->y = 0.0f;
                        rotP[i][capSel[i][1]]->z = 0.0f;
                        scaleP[i][capSel[i][1]]->x = 0.0f;
                        scaleP[i][capSel[i][1]]->y = 0.0f;
                        scaleP[i][capSel[i][1]]->z = 0.0f;
                        Hu3DModelAttrReset(model->mdlId[MDMODEL_CAPMAN_START + i + capSel[i][0] * 4], 1);
                    }
                }
                capOrbitRadius = 200.0f;
                capOrbitAngle = 90.0f;
                for (i = 0; i < 4; i++) {
                    capOrbitBase[i] = capIn3D[i];
                    capOrbitBase[i].z -= capOrbitRadius;
                }
                break;
            case 4:
                capSlide[0].timer = 0;
                capSlide[1].timer = 4;
                capSlide[2].timer = 8;
                capSlide[3].timer = 12;
                Hu3D2Dto3D(&capScreenBase, camera->camera[0].bit, &capBase3D);
                for (i = 0; i < 4; i++) {
                    Hu3D2Dto3D(&capScreenIn[i], camera->camera[0].bit, &capIn3D[i]);
                }
                for (i = 0; i < 2; i++) {
                    Hu3D2Dto3D(&capSide2D[i], camera->camera[0].bit, &capSide3D[i]);
                }
                for (i = 0; i < 4; i++) {
                    scaleP[i][capSel[i][0]]->x = 1.0f;
                    scaleP[i][capSel[i][0]]->y = 1.0f;
                    scaleP[i][capSel[i][0]]->z = 1.0f;
                    if (capSel[i][0] == 0) {
                        Hu3DModelAttrReset(model->mdlId[MDMODEL_CAPMAN_START + i], 1);
                        Hu3DModelAttrSet(model->mdlId[MDMODEL_CAPCOM_START + i], 1);
                    } else if (capSel[i][0] == 1) {
                        Hu3DModelAttrSet(model->mdlId[MDMODEL_CAPMAN_START + i], 1);
                        Hu3DModelAttrReset(model->mdlId[MDMODEL_CAPCOM_START + i], 1);
                    }
                    posP[i][capSel[i][0]]->x = capSide3D[1].x;
                    posP[i][capSel[i][0]]->y = capSide3D[1].y;
                    posP[i][capSel[i][0]]->z = capSide3D[1].z;
                }
                fn_1_15A40(6);
                break;
            case 5:
                for (i = 0; i < 4; i++) {
                    scaleP[i][capSel[i][0]]->x = 1.0f;
                    scaleP[i][capSel[i][0]]->y = 1.0f;
                    scaleP[i][capSel[i][0]]->z = 1.0f;
                    posP[i][capSel[i][0]]->x = capSide3D[1].x;
                    posP[i][capSel[i][0]]->y = capSide3D[1].y;
                    posP[i][capSel[i][0]]->z = capSide3D[1].z;
                }
                break;
        }
        sys->unk2A8 = 1;
    }
    switch (param) {
        case 0:
            for (i = 0; i < 4; i++) {
                MDBEACH_SLIDE_WORK *slide = &capSlide[i];
                switch (slide->mode) {
                    case 0:
                        if (--slide->timer < 0) {
                            slide->mode++;
                        }
                        break;
                    case 1:
                        MdMathOscillate(1, &slide->in, &slide->out, 30.0f);
                        MdMathLerp(&posP[i][capSel[i][0]]->x, capSide3D[0].x, capIn3D[i].x, slide->out);
                        MdMathLerp(&posP[i][capSel[i][0]]->y, capSide3D[0].y, capIn3D[i].y, slide->out);
                        MdMathLerp(&posP[i][capSel[i][0]]->z, capSide3D[0].z, capIn3D[i].z, slide->out);
                        MdMathLerpScale(scaleP[i][capSel[i][0]], 0.0f, 1.0f, slide->out);
                        if (slide->in >= 1.0f) {
                            slide->mode++;
                        }
                        break;
                }
            }
            break;
        case 1:
            for (i = 0; i < 4; i++) {
                MDBEACH_SLIDE_WORK *slide = &capSlide[i];
                switch (slide->mode) {
                    case 0:
                        if (--slide->timer < 0) {
                            slide->mode++;
                        }
                        break;
                    case 1:
                        MdMathOscillate(2, &slide->in, &slide->out, 30.0f);
                        MdMathLerp(&posP[i][capSel[i][0]]->x, capIn3D[i].x, capSide3D[1].x, slide->out);
                        MdMathLerp(&posP[i][capSel[i][0]]->y, capIn3D[i].y, capSide3D[1].y, slide->out);
                        MdMathLerp(&posP[i][capSel[i][0]]->z, capIn3D[i].z, capSide3D[1].z, slide->out);
                        if (slide->in >= 1.0f) {
                            slide->mode++;
                        }
                        break;
                }
            }
            break;
        case 2:
        case 3:
            MdMathOscillate(2, &sys->oscIn[0], &sys->oscOut[0], 20.0f);
            if (param == 2) {
                MdMathLerp(&capOrbitAngle, 90.0f, 270.0f, sys->oscOut[0]);
            } else if (param == 3) {
                MdMathLerp(&capOrbitAngle, 90.0f, -90.0f, sys->oscOut[0]);
            }
            for (i = 0; i < 4; i++) {
                if (capSel[i][0] != capSel[i][1]) {
                    MdMathLerpScale(scaleP[i][capSel[i][1]], 1.0f, 0.0f, sys->oscOut[0]);
                    posP[i][capSel[i][1]]->x = capOrbitBase[i].x + capOrbitRadius * cos(3.141592653589793 * capOrbitAngle / 180.0);
                    posP[i][capSel[i][1]]->y = capOrbitBase[i].y;
                    posP[i][capSel[i][1]]->z = capOrbitBase[i].z + capOrbitRadius * sin(3.141592653589793 * capOrbitAngle / 180.0);
                    MdMathLerpScale(scaleP[i][capSel[i][0]], 0.0f, 1.0f, sys->oscOut[0]);
                    posP[i][capSel[i][0]]->x = capOrbitBase[i].x + capOrbitRadius * cos(3.141592653589793 * (180.0f + capOrbitAngle) / 180.0);
                    posP[i][capSel[i][0]]->y = capOrbitBase[i].y;
                    posP[i][capSel[i][0]]->z = capOrbitBase[i].z + capOrbitRadius * sin(3.141592653589793 * (180.0f + capOrbitAngle) / 180.0);
                }
            }
            if (sys->unk1D > 1) {
                sys->unk2C4 = 0;
                MdProcTbl[MDBEACH_PROC_CHR_SEL](param);
            }
            break;
        case 4:
            for (i = 0; i < 4; i++) {
                MDBEACH_SLIDE_WORK *slide = &capSlide[i];
                switch (slide->mode) {
                    case 0:
                        if (--slide->timer < 0) {
                            slide->mode++;
                        }
                        break;
                    case 1:
                        MdMathOscillate(1, &slide->in, &slide->out, 30.0f);
                        MdMathLerp(&posP[i][capSel[i][0]]->x, capSide3D[1].x, capIn3D[i].x, slide->out);
                        MdMathLerp(&posP[i][capSel[i][0]]->y, capSide3D[1].y, capIn3D[i].y, slide->out);
                        MdMathLerp(&posP[i][capSel[i][0]]->z, capSide3D[1].z, capIn3D[i].z, slide->out);
                        if (slide->in >= 1.0f) {
                            slide->mode++;
                        }
                        break;
                }
            }
            break;
    }
    for (i = 0; i < 4; i++) {
        fn_1_15418(MDMODEL_CAPMAN_START + i, 1);
        fn_1_15418(MDMODEL_CAPCOM_START + i, 1);
    }
    switch (param) {
        case 0:
            if (capSlide[0].mode == 2) {
                return TRUE;
            }
            break;
        case 1:
            if (capSlide[0].mode == 2) {
                for (i = 0; i < 4; i++) {
                    Hu3DModelAttrSet(model->mdlId[MDMODEL_CAPMAN_START + i + capSel[i][0] * 4], 1);
                }
                return TRUE;
            }
            break;
        case 2:
        case 3:
            if (sys->oscIn[0] >= 1.0f) {
                for (i = 0; i < 4; i++) {
                    if (capSel[i][0] != capSel[i][1]) {
                        Hu3DModelAttrSet(model->mdlId[MDMODEL_CAPMAN_START + i + capSel[i][1] * 4], 1);
                    }
                }
                return TRUE;
            }
            break;
        case 4:
            if (capSlide[3].mode == 2) {
                return TRUE;
            }
            break;
        case 5:
            return TRUE;
    }
    return FALSE;
}

// selectable character sliding in/out along a fixed order
BOOL fn_1_D45C(int param)
{
    MDCAMERA_WORK *camera = sys->camera;
    MDMODEL_WORK *model = sys->model;
    HuVecF *posP[10];
    HuVecF *rotP[10];
    HuVecF *scaleP[10];
    int i;
    int slot;
    int idx;

    for (i = 0; i < 10; i++) {
        if (sys->selMode == 0) {
            posP[i] = model->posP[MDMODEL_VOLLEY_START + i];
            rotP[i] = model->rotP[MDMODEL_VOLLEY_START + i];
            scaleP[i] = model->scaleP[MDMODEL_VOLLEY_START + i];
        } else if (sys->selMode == 1) {
            posP[i] = model->posP[MDMODEL_HOCKEY_START + i];
            rotP[i] = model->rotP[MDMODEL_HOCKEY_START + i];
            scaleP[i] = model->scaleP[MDMODEL_HOCKEY_START + i];
        }
    }
    if (sys->unk2A8 == 0) {
        switch (param) {
            case 0:
                for (i = 0; i < 10; i++) {
                    charSlide[i].mode = 0;
                    charSlide[i].in = 0.0f;
                    charSlide[i].out = 0.0f;
                }
                slot = 0;
                for (i = 0; i < 10; i++) {
                    charSlide[charOrder[i]].timer = slot * 4;
                    slot++;
                }
                for (i = 0; i < 10; i++) {
                    charPos2D[i].b = charScreenPos[sys->selMode][i];
                    charPos2D[i].a = charPos2D[i].b;
                    charPos2D[i].a.x = -80.0f;
                    Hu3D2Dto3D(&charPos2D[i].a, camera->camera[0].bit, &charPos3D[i].a);
                    Hu3D2Dto3D(&charPos2D[i].b, camera->camera[0].bit, &charPos3D[i].b);
                }
                for (i = 0; i < 10; i++) {
                    if (sys->selMode == 0) {
                        Hu3DModelAttrReset(model->mdlId[MDMODEL_VOLLEY_START + i], 1);
                    } else if (sys->selMode == 1) {
                        Hu3DModelAttrReset(model->mdlId[MDMODEL_HOCKEY_START + i], 1);
                    }
                    *posP[i] = charPos3D[i].a;
                    rotP[i]->x = 0.0f;
                    rotP[i]->y = 0.0f;
                    rotP[i]->z = 0.0f;
                    scaleP[i]->x = 1.0f;
                    scaleP[i]->y = 1.0f;
                    scaleP[i]->z = 1.0f;
                }
                fn_1_15A40(6);
                break;
            case 1:
                for (i = 0; i < 10; i++) {
                    charSlide[i].mode = 0;
                    charSlide[i].in = 0.0f;
                    charSlide[i].out = 0.0f;
                }
                slot = 0;
                for (i = 9; i >= 0; i--) {
                    charSlide[charOrder[i]].timer = slot * 4;
                    slot++;
                }
                for (i = 0; i < 10; i++) {
                    charPos2D[i].b = charScreenPos[sys->selMode][i];
                    charPos2D[i].a = charPos2D[i].b;
                    charPos2D[i].a.x = -80.0f;
                    Hu3D2Dto3D(&charPos2D[i].a, camera->camera[0].bit, &charPos3D[i].a);
                    Hu3D2Dto3D(&charPos2D[i].b, camera->camera[0].bit, &charPos3D[i].b);
                }
                fn_1_15A40(6);
                break;
            case 2:
                for (i = 0; i < 10; i++) {
                    charSlide[i].mode = 0;
                    charSlide[i].in = 0.0f;
                    charSlide[i].out = 0.0f;
                }
                idx = 0;
                slot = 0;
                for (i = 0; i < 10; i++) {
                    if (sys->chrNo[0] == charOrder[idx] || sys->chrNo[1] == charOrder[idx]
                        || sys->chrNo[2] == charOrder[idx] || sys->chrNo[3] == charOrder[idx]) {
                        idx++;
                    } else {
                        charSlide[charOrder[idx]].timer = slot * 4;
                        charLastNo = charOrder[idx];
                        idx++;
                        slot++;
                    }
                }
                for (i = 0; i < 10; i++) {
                    charPos2D[i].b = charScreenPos[sys->selMode][i];
                    charPos2D[i].a = charPos2D[i].b;
                    charPos2D[i].a.x = 640.0f;
                    Hu3D2Dto3D(&charPos2D[i].a, camera->camera[0].bit, &charPos3D[i].a);
                    Hu3D2Dto3D(&charPos2D[i].b, camera->camera[0].bit, &charPos3D[i].b);
                }
                fn_1_15A40(6);
                break;
            case 3:
                for (i = 0; i < 10; i++) {
                    charSlide[i].mode = 0;
                    charSlide[i].in = 0.0f;
                    charSlide[i].out = 0.0f;
                }
                idx = 9;
                slot = 0;
                for (i = 0; i < 10; i++) {
                    if ((sys->chrNo[0] == charOrder[idx] && sys->comPlayerNo[0] == 1)
                        || (sys->chrNo[1] == charOrder[idx] && sys->comPlayerNo[1] == 1)
                        || (sys->chrNo[2] == charOrder[idx] && sys->comPlayerNo[2] == 1)
                        || (sys->chrNo[3] == charOrder[idx] && sys->comPlayerNo[3] == 1)) {
                        idx--;
                    } else {
                        charSlide[charOrder[idx]].timer = slot * 4;
                        charLastNo = charOrder[idx];
                        idx--;
                        slot++;
                    }
                }
                for (i = 0; i < 10; i++) {
                    charPos2D[i].b = charScreenPos[sys->selMode][i];
                    charPos2D[i].a = charPos2D[i].b;
                    charPos2D[i].a.x = 640.0f;
                    Hu3D2Dto3D(&charPos2D[i].a, camera->camera[0].bit, &charPos3D[i].a);
                    Hu3D2Dto3D(&charPos2D[i].b, camera->camera[0].bit, &charPos3D[i].b);
                }
                fn_1_15A40(6);
                break;
            case 4:
                for (i = 0; i < 10; i++) {
                    if (i != sys->chrNo[0] && i != sys->chrNo[1] && i != sys->chrNo[2] && i != sys->chrNo[3]) {
                        charPos2D[i].b = charScreenPos[sys->selMode][i];
                        charPos2D[i].b.x = 640.0f;
                        Hu3D2Dto3D(&charPos2D[i].b, camera->camera[0].bit, &charPos3D[i].b);
                        *posP[i] = charPos3D[i].b;
                        if (sys->selMode == 0) {
                            Hu3DModelAttrReset(model->mdlId[MDMODEL_VOLLEY_START + i], 1);
                        } else if (sys->selMode == 1) {
                            Hu3DModelAttrReset(model->mdlId[MDMODEL_HOCKEY_START + i], 1);
                        }
                    }
                }
                for (i = 0; i < 10; i++) {
                    if (i == sys->chrNo[0] || i == sys->chrNo[1] || i == sys->chrNo[2] || i == sys->chrNo[3]) {
                        charPos2D[i].b = charScreenPos[sys->selMode][i];
                        Hu3D2Dto3D(&charPos2D[i].b, camera->camera[0].bit, &charPos3D[i].b);
                        *posP[i] = charPos3D[i].b;
                    }
                }
                break;
        }
        sys->unk2A8 = 1;
    }
    switch (param) {
        case 0:
            for (i = 0; i < 10; i++) {
                MDBEACH_SLIDE_WORK *slide = &charSlide[i];
                switch (slide->mode) {
                    case 0:
                        if (--slide->timer < 0) {
                            slide->mode++;
                        }
                        break;
                    case 1:
                        MdMathOscillate(1, &slide->in, &slide->out, 30.0f);
                        MdMathLerp(&posP[i]->x, charPos3D[i].a.x, charPos3D[i].b.x, slide->out);
                        MdMathLerp(&posP[i]->y, charPos3D[i].a.y, charPos3D[i].b.y, slide->out);
                        MdMathLerp(&posP[i]->z, charPos3D[i].a.z, charPos3D[i].b.z, slide->out);
                        if (slide->in >= 1.0f) {
                            slide->mode++;
                        }
                        break;
                }
            }
            break;
        case 1:
            for (i = 0; i < 10; i++) {
                MDBEACH_SLIDE_WORK *slide = &charSlide[i];
                switch (slide->mode) {
                    case 0:
                        if (--slide->timer < 0) {
                            slide->mode++;
                        }
                        break;
                    case 1:
                        MdMathOscillate(2, &slide->in, &slide->out, 30.0f);
                        MdMathLerp(&posP[i]->x, charPos3D[i].b.x, charPos3D[i].a.x, slide->out);
                        MdMathLerp(&posP[i]->y, charPos3D[i].b.y, charPos3D[i].a.y, slide->out);
                        MdMathLerp(&posP[i]->z, charPos3D[i].b.z, charPos3D[i].a.z, slide->out);
                        if (slide->in >= 1.0f) {
                            slide->mode++;
                        }
                        break;
                }
            }
            break;
        case 2:
            for (i = 0; i < 10; i++) {
                MDBEACH_SLIDE_WORK *slide = &charSlide[i];
                if (i == sys->chrNo[0] || i == sys->chrNo[1] || i == sys->chrNo[2] || i == sys->chrNo[3]) {
                    continue;
                }
                switch (slide->mode) {
                    case 0:
                        if (--slide->timer < 0) {
                            slide->mode++;
                        }
                        break;
                    case 1:
                        MdMathOscillate(2, &slide->in, &slide->out, 30.0f);
                        MdMathLerp(&posP[i]->x, charPos3D[i].b.x, charPos3D[i].a.x, slide->out);
                        MdMathLerp(&posP[i]->y, charPos3D[i].b.y, charPos3D[i].a.y, slide->out);
                        MdMathLerp(&posP[i]->z, charPos3D[i].b.z, charPos3D[i].a.z, slide->out);
                        if (slide->in >= 1.0f) {
                            slide->mode++;
                        }
                        break;
                }
            }
            break;
        case 3:
            for (i = 0; i < 10; i++) {
                MDBEACH_SLIDE_WORK *slide = &charSlide[i];
                if ((i == sys->chrNo[0] && sys->comPlayerNo[0] == 1)
                    || (i == sys->chrNo[1] && sys->comPlayerNo[1] == 1)
                    || (i == sys->chrNo[2] && sys->comPlayerNo[2] == 1)
                    || (i == sys->chrNo[3] && sys->comPlayerNo[3] == 1)) {
                    continue;
                }
                switch (slide->mode) {
                    case 0:
                        if (--slide->timer < 0) {
                            slide->mode++;
                        }
                        break;
                    case 1:
                        MdMathOscillate(2, &slide->in, &slide->out, 30.0f);
                        MdMathLerp(&posP[i]->x, charPos3D[i].a.x, charPos3D[i].b.x, slide->out);
                        MdMathLerp(&posP[i]->y, charPos3D[i].a.y, charPos3D[i].b.y, slide->out);
                        MdMathLerp(&posP[i]->z, charPos3D[i].a.z, charPos3D[i].b.z, slide->out);
                        if (slide->in >= 1.0f) {
                            slide->mode++;
                        }
                        break;
                }
            }
            break;
    }
    switch (param) {
        case 0:
            if (charSlide[charOrder[9]].mode == 2) {
                return TRUE;
            }
            break;
        case 1:
            if (charSlide[charOrder[0]].mode == 2) {
                return TRUE;
            }
            break;
        case 2:
        case 3:
            if (charSlide[charLastNo].mode == 2) {
                return TRUE;
            }
            break;
        case 4:
            return TRUE;
    }
    return FALSE;
}

/* ---- part G ---- */
/* fn_1_EC34 */

/* ---- TYPES ---- */
typedef struct MdBeachEnterAnim_s {
    s16 mode; //0x00
    s16 timer; //0x02
    float in; //0x04
    float out; //0x08
} MDBEACH_ENTER_ANIM;

typedef struct MdBeachEnterMax_s {
    s8 idx; //0x00 (character with the longest start delay)
    s8 val; //0x01
} MDBEACH_ENTER_MAX;

/* ---- PROTOS ---- */

/* ---- DATA ---- */
static HU3DPARMANPARAM sparkParManParam = {
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

/* lbl_1_data_158: screen positions (x, y, z) for the game-select character walk-in */
static HuVecF gameSelChrScrPos[2][10] = {
    {
        { 70.0f, 204.0f, 450.0f }, { 150.0f, 240.0f, 450.0f }, { 229.0f, 224.0f, 450.0f }, { 328.0f, 179.0f, 450.0f }, { 413.0f, 245.0f, 450.0f },
        { 514.0f, 215.0f, 450.0f }, { 80.0f, 350.0f, 450.0f }, { 190.0f, 359.0f, 450.0f }, { 393.0f, 370.0f, 450.0f }, { 508.0f, 353.0f, 450.0f },
    },
    {
        { 76.0f, 194.0f, 450.0f }, { 138.0f, 265.0f, 450.0f }, { 234.0f, 216.0f, 450.0f }, { 322.0f, 194.0f, 450.0f }, { 400.0f, 255.0f, 450.0f },
        { 499.0f, 211.0f, 450.0f }, { 66.0f, 354.0f, 450.0f }, { 183.0f, 365.0f, 450.0f }, { 392.0f, 374.0f, 450.0f }, { 497.0f, 358.0f, 450.0f },
    },
};

/* lbl_1_data_338: screen position offsets (x, y, z) for the game-select exit */
static HuVecF gameSelChrScrOfs[2][10] = {
    {
        { 0.0f, -30.0f, 1500.0f }, { 0.0f, -30.0f, 1500.0f }, { -8.0f, -17.0f, 1500.0f }, { -24.0f, -2.0f, 1500.0f }, { -8.0f, -26.0f, 1500.0f },
        { -1.0f, -16.0f, 1500.0f }, { 0.0f, -16.0f, 1500.0f }, { 0.0f, -30.0f, 1500.0f }, { 0.0f, -24.0f, 1500.0f }, { 7.0f, -39.0f, 1500.0f },
    },
    {
        { 0.0f, -30.0f, 1500.0f }, { 4.0f, -30.0f, 1500.0f }, { -3.0f, -17.0f, 1500.0f }, { 2.0f, -27.0f, 1500.0f }, { 3.0f, -29.0f, 1500.0f },
        { -1.0f, -16.0f, 1500.0f }, { 0.0f, -16.0f, 1500.0f }, { 0.0f, -30.0f, 1500.0f }, { 0.0f, -29.0f, 1500.0f }, { -1.0f, -39.0f, 1500.0f },
    },
};

/* lbl_1_data_838 */
static HuVecF chrSlotScrPos[4] = {
    { 96.0f, 224.0f, 1500.0f },
    { 192.0f, 224.0f, 1500.0f },
    { 384.0f, 224.0f, 1500.0f },
    { 480.0f, 224.0f, 1500.0f },
};

/* lbl_1_data_868 */
static float chrSlotScrX[4] = { 176.0f, 232.0f, 344.0f, 400.0f };

/* bss */
static s32 *chrSparkP[4]; //lbl_1_bss_D4
static MDBEACH_ENTER_MAX enterMax; //lbl_1_bss_E4
static MDBEACH_ENTER_ANIM enterAnim[4]; //lbl_1_bss_E8
static float chrRotRange[4][2]; //lbl_1_bss_118
static HuVecF chrOrgPos[4][2]; //lbl_1_bss_138
static HuVecF chrPosPath[4][4]; //lbl_1_bss_198
static HuVecF chrScrPos[4][4]; //lbl_1_bss_258

/* ---- CODE ---- */
static void Lerp3(HuVecF *out, HuVecF *a, HuVecF *b, float t)
{
    MdMathLerp(&out->x, a->x, b->x, t);
    MdMathLerp(&out->y, a->y, b->y, t);
    MdMathLerp(&out->z, a->z, b->z, t);
}

static void EnterAnimInit(void)
{
    int i;
    for (i = 0; i < 4; i++) {
        enterAnim[i].mode = 0;
        enterAnim[i].timer = 0;
        enterAnim[i].in = 0.0f;
        enterAnim[i].out = 0.0f;
    }
}

static void EnterAnimInitRand(int range)
{
    int i;
    enterMax.val = 0;
    enterMax.idx = 0;
    for (i = 0; i < 4; i++) {
        enterAnim[i].mode = 0;
        enterAnim[i].timer = frandmod(range);
        if (enterAnim[i].timer > enterMax.val) {
            enterMax.idx = i;
            enterMax.val = enterAnim[i].timer;
        }
        enterAnim[i].in = 0.0f;
        enterAnim[i].out = 0.0f;
    }
}

BOOL fn_1_EC34(int param)
{
    MDMODEL_WORK *model = sys->model;
    MDCAMERA_WORK *camera = sys->camera;
    MDSPRITE_WORK *sprite = sys->sprite;
    MDBEACH_STATE_WORK *chrWork = sys->chrWork;
    HuVecF scr;
    HuVecF out3d;
    HuVecF *tgt = NULL;
    HuVecF *pos;
    HuVecF *rot;
    HU3DMODELID mdl;
    int i;

    scr.x = scr.y = scr.z = 0.0f;
    if (sys->unk2A8 == 0) {
        switch (param) {
            case 0:
                EnterAnimInitRand(8);
                for (i = 0; i < 4; i++) {
                    chrPosPath[i][0] = *model->posP[sys->chrNo[i] + MDMODEL_CHAR_START];
                    chrOrgPos[i][0] = chrPosPath[i][0];
                    chrScrPos[i][1] = chrSlotScrPos[i];
                    chrScrPos[i][1].y = -8.0f;
                    Hu3D2Dto3D(&chrScrPos[i][1], camera->camera[MDCAMERA_MAIN].bit, &chrPosPath[i][1]);
                    chrPosPath[i][1].x = chrPosPath[i][0].x;
                    chrOrgPos[i][1] = chrPosPath[i][1];
                    chrScrPos[i][2] = chrSlotScrPos[i];
                    chrScrPos[i][2].y = -8.0f;
                    Hu3D2Dto3D(&chrScrPos[i][2], camera->camera[MDCAMERA_MAIN].bit, &chrPosPath[i][2]);
                    chrScrPos[i][3] = chrSlotScrPos[i];
                    Hu3D2Dto3D(&chrScrPos[i][3], camera->camera[MDCAMERA_MAIN].bit, &chrPosPath[i][3]);
                }
                break;

            case 1:
                EnterAnimInitRand(8);
                for (i = 0; i < 4; i++) {
                    chrPosPath[i][0] = *model->posP[sys->chrNo[i] + MDMODEL_CHAR_START];
                    chrScrPos[i][1] = chrSlotScrPos[sys->chrGrpNo[i]];
                    chrScrPos[i][1].y = -8.0f;
                    Hu3D2Dto3D(&chrScrPos[i][1], camera->camera[MDCAMERA_MAIN].bit, &chrPosPath[i][1]);
                    chrPosPath[i][1].x = chrPosPath[i][0].x;
                    chrPosPath[i][2] = chrOrgPos[i][1];
                    chrPosPath[i][3] = chrOrgPos[i][0];
                }
                break;

            case 2:
                EnterAnimInitRand(8);
                for (i = 0; i < 4; i++) {
                    chrPosPath[i][0] = *model->posP[sys->chrNo[i] + MDMODEL_CHAR_START];
                    chrScrPos[i][1] = chrSlotScrPos[sys->unk4F[i]];
                    chrScrPos[i][1].x = chrSlotScrX[sys->unk4F[i]];
                    chrScrPos[i][1].y = -8.0f;
                    Hu3D2Dto3D(&chrScrPos[i][1], camera->camera[MDCAMERA_MAIN].bit, &chrPosPath[i][1]);
                    chrScrPos[i][2] = chrSlotScrPos[sys->chrGrpNo[i]];
                    chrScrPos[i][2].x = chrSlotScrX[sys->chrGrpNo[i]];
                    chrScrPos[i][2].y = -8.0f;
                    Hu3D2Dto3D(&chrScrPos[i][2], camera->camera[MDCAMERA_MAIN].bit, &chrPosPath[i][2]);
                    chrScrPos[i][3] = chrSlotScrPos[sys->chrGrpNo[i]];
                    Hu3D2Dto3D(&chrScrPos[i][3], camera->camera[MDCAMERA_MAIN].bit, &chrPosPath[i][3]);
                }
                break;

            case 3:
            case 4:
                EnterAnimInit();
                for (i = 0; i < 4; i++) {
                    chrScrPos[i][0] = chrSlotScrPos[sys->chrGrpNo[i]];
                    Hu3D2Dto3D(&chrScrPos[i][0], camera->camera[MDCAMERA_MAIN].bit, &chrPosPath[i][0]);
                    chrScrPos[i][1] = chrScrPos[i][0];
                    if (sys->chrGrpNo[i] < 2) {
                        chrScrPos[i][1].x -= 32.0f;
                    } else {
                        chrScrPos[i][1].x += 32.0f;
                    }
                    Hu3D2Dto3D(&chrScrPos[i][1], camera->camera[MDCAMERA_MAIN].bit, &chrPosPath[i][1]);
                }
                break;

            case 5:
                EnterAnimInitRand(12);
                for (i = 0; i < 4; i++) {
                    chrPosPath[i][0] = *model->posP[sys->chrNo[i] + MDMODEL_CHAR_START];
                    chrScrPos[i][1] = chrSlotScrPos[sys->chrGrpNo[i]];
                    chrScrPos[i][1].x = chrSlotScrX[sys->chrGrpNo[i]];
                    chrScrPos[i][1].y = -8.0f;
                    Hu3D2Dto3D(&chrScrPos[i][1], camera->camera[MDCAMERA_MAIN].bit, &chrPosPath[i][1]);
                    chrScrPos[i][2] = chrSlotScrPos[sys->chrGrpNo[i]];
                    chrScrPos[i][2].x = chrSlotScrX[sys->chrGrpNo[i]];
                    chrScrPos[i][2].y = -8.0f;
                    Hu3D2Dto3D(&chrScrPos[i][2], camera->camera[MDCAMERA_MAIN].bit, &chrPosPath[i][2]);
                    chrScrPos[i][3] = chrSlotScrPos[sys->chrGrpNo[i]];
                    Hu3D2Dto3D(&chrScrPos[i][3], camera->camera[MDCAMERA_MAIN].bit, &chrPosPath[i][3]);
                    chrRotRange[i][0] = model->rotP[sys->chrNo[i] + MDMODEL_CHAR_START]->y;
                    if (sys->chrGrpNo[i] < 2) {
                        chrRotRange[i][1] = 360.0f;
                    } else {
                        chrRotRange[i][1] = -360.0f;
                    }
                }
                for (i = 0; i < 4; i++) {
                    s16 camBit = camera->camera[MDCAMERA_MAIN].bit;
                    ANIMDATA *anim = sprite->animP[8];
                    s32 *spark = HuMemDirectMallocNum(HEAP_HEAP, 0xC, HU_MEMNUM_OVL);
                    HU3DPARMANID parManId = Hu3DParManCreate(anim, 100, &sparkParManParam);
                    *spark = parManId;
                    Hu3DParManAttrSet(parManId, 0x65);
                    Hu3DParticleBlendModeSet(Hu3DParManModelIDGet(parManId), 1);
                    Hu3DParManRotSet(parManId, 90.0f, 0.0f, 0.0f);
                    Hu3DModelLayerSet(Hu3DParManModelIDGet(parManId), 0);
                    Hu3DModelCameraSet(Hu3DParManModelIDGet(parManId), (u16)camBit);
                    chrSparkP[i] = spark;
                }
                fn_1_15A40(0x11);
                break;

            case 6:
                for (i = 0; i < 4; i++) {
                    if (sys->selMode == 0) {
                        Hu3D3Dto2D(model->posP[sys->chrNo[i] + MDMODEL_VOLLEY_START], camera->camera[MDCAMERA_MAIN].bit, &scr);
                    } else if (sys->selMode == 1) {
                        Hu3D3Dto2D(model->posP[sys->chrNo[i] + MDMODEL_HOCKEY_START], camera->camera[MDCAMERA_MAIN].bit, &scr);
                    }
                    scr.x += gameSelChrScrOfs[sys->selMode][sys->chrNo[i]].x;
                    scr.y += gameSelChrScrOfs[sys->selMode][sys->chrNo[i]].y;
                    scr.z = gameSelChrScrOfs[sys->selMode][sys->chrNo[i]].z;
                    Hu3D2Dto3D(&scr, camera->camera[MDCAMERA_MAIN].bit, &out3d);
                    chrOrgPos[i][0] = out3d;
                    scr = chrSlotScrPos[i];
                    scr.y = -8.0f;
                    Hu3D2Dto3D(&scr, camera->camera[MDCAMERA_MAIN].bit, &out3d);
                    out3d.x = chrOrgPos[i][0].x;
                    chrOrgPos[i][1] = out3d;
                }
                for (i = 0; i < 4; i++) {
                    mdl = sys->chrNo[i] + MDMODEL_CHAR_START;
                    chrScrPos[i][3] = chrSlotScrPos[sys->chrGrpNo[i]];
                    Hu3D2Dto3D(&chrScrPos[i][3], camera->camera[MDCAMERA_MAIN].bit, &chrPosPath[i][3]);
                    *model->posP[mdl] = chrPosPath[i][3];
                    model->rotP[mdl]->x = 0.0f;
                    model->rotP[mdl]->y = 0.0f;
                    model->rotP[mdl]->z = 0.0f;
                    model->scaleP[mdl]->x = 1.0f;
                    model->scaleP[mdl]->y = 1.0f;
                    model->scaleP[mdl]->z = 1.0f;
                    Hu3DModelAttrReset(model->mdlId[mdl], 1);
                    fn_1_15418(mdl, 0);
                    Hu3DMotionSet(model->mdlId[mdl], model->motId[sys->chrNo[i] * 3 + 3]);
                    Hu3DModelAttrSet(model->mdlId[mdl], 0x40000001);
                    Hu3DModelAttrReset(model->mdlId[mdl], 0x40000002);
                    chrWork[i].mode = 4;
                }
                break;
        }
        sys->unk2A8 = 1;
    }

    switch (param) {
        case 0:
        case 1:
        case 2: {
            /* per-case tuning: first motion shift length, oscillation length, second motion shift length */
            float shiftIn = (param == 0) ? 4.0f : 2.0f;
            float oscLen = (param == 2) ? 20.0f : 30.0f;
            float shiftOut = (param == 2) ? 20.0f : 30.0f;
            for (i = 0; i < 4; i++) {
                MDBEACH_ENTER_ANIM *anim = &enterAnim[i];
                mdl = sys->chrNo[i] + MDMODEL_CHAR_START;
                pos = model->posP[mdl];
                switch (anim->mode) {
                    case 0:
                        if (--anim->timer < 0) {
                            Hu3DMotionShiftSet(model->mdlId[mdl], model->motId[sys->chrNo[i] * 3 + 5], 0.0f, shiftIn, 0);
                            anim->mode++;
                        }
                        break;
                    case 1:
                        MdMathOscillate(MDMATH_OSC_90, &anim->in, &anim->out, oscLen);
                        Lerp3(pos, &chrPosPath[i][0], &chrPosPath[i][1], anim->out);
                        if (anim->in >= 1.0f) {
                            Hu3DMotionShiftSet(model->mdlId[mdl], model->motId[sys->chrNo[i] * 3 + 3], 0.0f, shiftOut, 0x40000001);
                            anim->in = 0.0f;
                            anim->mode++;
                            if (param == 1) {
                                if (sys->unk1B == 3 || sys->padEnable[i] == 0) {
                                    chrWork[i].mode = 5;
                                    sys->unk36[i] = 1;
                                    sys->comPlayerNo[i] = 0;
                                    Hu3DModelAttrSet(model->mdlId[mdl], 1);
                                    if (sys->selMode == 0) {
                                        tgt = model->posP[sys->chrNo[i] + MDMODEL_VOLLEY_START];
                                    } else if (sys->selMode == 1) {
                                        tgt = model->posP[sys->chrNo[i] + MDMODEL_HOCKEY_START];
                                    }
                                    scr = gameSelChrScrPos[sys->selMode][sys->chrNo[i]];
                                    scr.x = 640.0f;
                                    if (tgt != NULL) {
                                        Hu3D2Dto3D(&scr, camera->camera[MDCAMERA_MAIN].bit, tgt);
                                    }
                                }
                            }
                        }
                        break;
                    case 2:
                        MdMathOscillate(MDMATH_OSC_90_REV, &anim->in, &anim->out, oscLen);
                        Lerp3(pos, &chrPosPath[i][2], &chrPosPath[i][3], anim->out);
                        if (anim->in >= 1.0f) {
                            anim->mode++;
                        }
                        break;
                }
                fn_1_15418(sys->chrNo[i] + MDMODEL_CHAR_START, 0);
            }
            fn_1_146D0();
            if (param == 2) {
                sys->unk2C4 = 1;
                MdProcCall(MDBEACH_PROC_CHR_SEL, param + sys->unk2C0);
            }
            break;
        }

        case 3:
        case 4:
            for (i = 0; i < 4; i++) {
                MDBEACH_ENTER_ANIM *anim = &enterAnim[i];
                mdl = sys->chrNo[i] + MDMODEL_CHAR_START;
                pos = model->posP[mdl];
                switch (anim->mode) {
                    case 0:
                        if (--anim->timer < 0) {
                            anim->mode++;
                        }
                        break;
                    case 1:
                        MdMathOscillate(MDMATH_OSC_90, &anim->in, &anim->out, 15.0f);
                        if (param == 3) {
                            Lerp3(pos, &chrPosPath[i][0], &chrPosPath[i][1], anim->out);
                        } else if (param == 4) {
                            Lerp3(pos, &chrPosPath[i][1], &chrPosPath[i][0], anim->out);
                        }
                        if (anim->in >= 1.0f) {
                            anim->mode++;
                        }
                        break;
                }
                fn_1_15418(sys->chrNo[i] + MDMODEL_CHAR_START, 0);
            }
            fn_1_146D0();
            if (param == 3) {
                MdProcCall(MDBEACH_PROC_CHR_POS, 2);
            } else if (param == 4) {
                MdProcCall(MDBEACH_PROC_CHR_POS, 3);
            }
            break;

        case 5:
            for (i = 0; i < 4; i++) {
                MDBEACH_ENTER_ANIM *anim = &enterAnim[i];
                s32 *spark;
                mdl = sys->chrNo[i] + MDMODEL_CHAR_START;
                pos = model->posP[mdl];
                rot = model->rotP[mdl];
                switch (anim->mode) {
                    case 0:
                        if (--anim->timer < 0) {
                            Hu3DMotionShiftSet(model->mdlId[mdl], model->motId[sys->chrNo[i] * 3 + 5], 0.0f, 2.0f, 0);
                            anim->mode++;
                        }
                        break;
                    case 1:
                        MdMathOscillate(MDMATH_OSC_90_REV, &anim->in, &anim->out, 30.0f);
                        Lerp3(pos, &chrPosPath[i][0], &chrPosPath[i][1], anim->out);
                        MdMathLerp(&rot->y, chrRotRange[i][0], chrRotRange[i][1], anim->out);
                        if (anim->in >= 1.0f) {
                            anim->mode = 3;
                        }
                        spark = chrSparkP[i];
                        Hu3DParManPosSet((s16)*spark, pos->x, pos->y, pos->z);
                        Hu3DParManTimeLimitSet((s16)*spark, 2);
                        Hu3DParManAttrReset((s16)*spark, 1);
                        break;
                }
            }
            fn_1_146D0();
            break;

        case 6:
            /* original used the stale loop counter here (4 after init); intent is every character */
            for (i = 0; i < 4; i++) {
                fn_1_15418(sys->chrNo[i] + MDMODEL_CHAR_START, 0);
            }
            fn_1_146D0();
            break;
    }

    switch (param) {
        case 0:
        case 1:
        case 2:
        case 5:
            if (enterAnim[enterMax.idx].mode == 3) {
                return TRUE;
            }
            return FALSE;
        case 3:
        case 4:
            if (enterAnim[2].mode == 2) {
                return TRUE;
            }
            return FALSE;
        case 6:
            return TRUE;
        default:
            return FALSE;
    }
}

/* ---- part H ---- */
// fn_1_118D0, fn_1_12314, fn_1_12894, fn_1_12E6C, fn_1_13850, fn_1_13A2C

/* PROTOS */

/* DATA */
static HuVecF helpWinOfs = { 114.0f, 8.0f, 0.0f };

static HuVecF vsMdlPosTbl[3] = {
    { -12.0f, 320.0f, 500.0f },
    { -12.0f, 120.0f, 0.0f },
    { -12.0f, 320.0f, -500.0f },
};

static HuVecF vsMdlRotTbl[3] = {
    { 60.0f, 0.0f, 270.0f },
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, -270.0f, 0.0f },
};

static HuVecF ballPos2DTbl[3] = {
    { 288.0f, 188.0f, 1500.0f },
    { 288.0f, 188.0f, 2000.0f },
    { 288.0f, 188.0f, 5000.0f },
};

static HuVecF ballRotTbl[3] = {
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f },
    { 40.0f, 0.0f, 0.0f },
};

/* BSS */
static float winSprPos[12];
static float modeSprPos[6];
static HuVecF ballPos2D[4];
static HuVecF ballPos3D[4];

/* fn_1_118D0: help/info window slide in/out (param 0..4) */
BOOL fn_1_118D0(int param)
{
    MDSPRITE_WORK *sprite = sys->sprite;
    MDMESS *mess = sys->mess;
    HUSPRGRPID grpId = sprite->grpId[MDSPR_GRP_MODE0];
    HuVec2f *sprPos;
    if(sys->unk2AC == 0) {
        switch(param) {
            case 0:
            case 1:
            case 4:
                winSprPos[0] = -128.0f;
                winSprPos[1] = 252.0f;
                winSprPos[2] = 0.0f;
                winSprPos[3] = 140.0f;
                winSprPos[4] = 252.0f;
                winSprPos[5] = 0.0f;
                winSprPos[6] = 704.0f;
                winSprPos[7] = 252.0f;
                winSprPos[8] = 0.0f;
                winSprPos[9] = 436.0f;
                winSprPos[10] = 252.0f;
                winSprPos[11] = 0.0f;
                break;

            case 2:
            case 3:
                winSprPos[0] = 140.0f;
                winSprPos[1] = 252.0f;
                winSprPos[2] = 0.0f;
                winSprPos[3] = 108.0f;
                winSprPos[4] = 252.0f;
                winSprPos[5] = 0.0f;
                winSprPos[6] = 436.0f;
                winSprPos[7] = 252.0f;
                winSprPos[8] = 0.0f;
                winSprPos[9] = 468.0f;
                winSprPos[10] = 252.0f;
                winSprPos[11] = 0.0f;
                break;
        }
        switch(param) {
            case 0:
                HuSprGrpPosSet(grpId, 0.0f, 0.0f);
                sprPos = MdSpritePosGet(sprite, MDSPR_GRP_MODE0, 0);
                sprPos->x = winSprPos[0];
                sprPos->y = winSprPos[1];
                HuSprAttrReset(grpId, 0, HUSPR_ATTR_DISPOFF);
                sprPos = MdSpritePosGet(sprite, MDSPR_GRP_MODE0, 1);
                sprPos->x = winSprPos[6];
                sprPos->y = winSprPos[7];
                HuSprAttrReset(grpId, 1, HUSPR_ATTR_DISPOFF);
                fn_1_15A40(6);
                break;

            case 1:
                fn_1_15A40(6);
                break;

            case 4:
                HuSprGrpPosSet(grpId, 0.0f, 0.0f);
                sprPos = MdSpritePosGet(sprite, MDSPR_GRP_MODE0, 0);
                sprPos->x = winSprPos[3];
                sprPos->y = winSprPos[4];
                HuSprAttrReset(grpId, 0, HUSPR_ATTR_DISPOFF);
                sprPos = MdSpritePosGet(sprite, MDSPR_GRP_MODE0, 1);
                sprPos->x = winSprPos[9];
                sprPos->y = winSprPos[10];
                HuSprAttrReset(grpId, 1, HUSPR_ATTR_DISPOFF);
                break;
        }
        sys->unk2AC = 1;
    }
    switch(param) {
        case 0:
            MdMathOscillate(MDMATH_OSC_90, &sys->oscIn[0], &sys->oscOut[0], 30);
            sprPos = MdSpritePosGet(sprite, MDSPR_GRP_MODE0, 0);
            MdMathLerp(&sprPos->x, winSprPos[0], winSprPos[3], sys->oscOut[0]);
            MdMessWinPosSet(mess, MDMESS_WIN_HELP, sprPos->x-helpWinOfs.x, sprPos->y-helpWinOfs.y);
            sprPos = MdSpritePosGet(sprite, MDSPR_GRP_MODE0, 1);
            MdMathLerp(&sprPos->x, winSprPos[6], winSprPos[9], sys->oscOut[0]);
            MdMessWinPosSet(mess, MDMESS_WIN_INFO, sprPos->x-helpWinOfs.x, sprPos->y-helpWinOfs.y);
            MdProcCall(MDBEACH_PROC_MODEL_POS, 0);
            break;

        case 1:
            MdMathOscillate(MDMATH_OSC_90, &sys->oscIn[0], &sys->oscOut[0], 30);
            sprPos = MdSpritePosGet(sprite, MDSPR_GRP_MODE0, 0);
            MdMathLerp(&sprPos->x, winSprPos[3], winSprPos[0], sys->oscOut[0]);
            MdMessWinPosSet(mess, MDMESS_WIN_HELP, sprPos->x-helpWinOfs.x, sprPos->y-helpWinOfs.y);
            sprPos = MdSpritePosGet(sprite, MDSPR_GRP_MODE0, 1);
            MdMathLerp(&sprPos->x, winSprPos[9], winSprPos[6], sys->oscOut[0]);
            MdMessWinPosSet(mess, MDMESS_WIN_INFO, sprPos->x-helpWinOfs.x, sprPos->y-helpWinOfs.y);
            MdProcCall(MDBEACH_PROC_MODEL_POS, 1);
            break;

        case 2:
            MdMathOscillate(MDMATH_OSC_90, &sys->oscIn[0], &sys->oscOut[0], 15);
            sprPos = MdSpritePosGet(sprite, MDSPR_GRP_MODE0, 0);
            MdMathLerp(&sprPos->x, winSprPos[0], winSprPos[3], sys->oscOut[0]);
            MdMessWinPosSet(mess, MDMESS_WIN_HELP, sprPos->x-helpWinOfs.x, sprPos->y-helpWinOfs.y);
            sprPos = MdSpritePosGet(sprite, MDSPR_GRP_MODE0, 1);
            MdMathLerp(&sprPos->x, winSprPos[6], winSprPos[9], sys->oscOut[0]);
            MdMessWinPosSet(mess, MDMESS_WIN_INFO, sprPos->x-helpWinOfs.x, sprPos->y-helpWinOfs.y);
            break;

        case 3:
            MdMathOscillate(MDMATH_OSC_90, &sys->oscIn[0], &sys->oscOut[0], 15);
            sprPos = MdSpritePosGet(sprite, MDSPR_GRP_MODE0, 0);
            MdMathLerp(&sprPos->x, winSprPos[3], winSprPos[0], sys->oscOut[0]);
            MdMessWinPosSet(mess, MDMESS_WIN_HELP, sprPos->x-helpWinOfs.x, sprPos->y-helpWinOfs.y);
            sprPos = MdSpritePosGet(sprite, MDSPR_GRP_MODE0, 1);
            MdMathLerp(&sprPos->x, winSprPos[9], winSprPos[6], sys->oscOut[0]);
            MdMessWinPosSet(mess, MDMESS_WIN_INFO, sprPos->x-helpWinOfs.x, sprPos->y-helpWinOfs.y);
            break;

        case 4:
            sprPos = MdSpritePosGet(sprite, MDSPR_GRP_MODE0, 0);
            MdMessWinPosSet(mess, MDMESS_WIN_HELP, sprPos->x-helpWinOfs.x, sprPos->y-helpWinOfs.y);
            sprPos = MdSpritePosGet(sprite, MDSPR_GRP_MODE0, 1);
            MdMessWinPosSet(mess, MDMESS_WIN_INFO, sprPos->x-helpWinOfs.x, sprPos->y-helpWinOfs.y);
            sys->oscIn[0] = 1;
            break;
    }
    return sys->oscIn[0] >= 1;
}

/* fn_1_12314: mode sprite group slide (param 0..3) */
BOOL fn_1_12314(int param)
{
    float x;
    float y;
    MDSPRITE_WORK *sprite = sys->sprite;
    HUSPRGRPID grpId = 0;
    if(sys->selMode == 0) {
        grpId = sprite->grpId[MDSPR_GRP_MODE1];
    } else if(sys->selMode == 1) {
        grpId = sprite->grpId[MDSPR_GRP_MODE2];
    }
    if(sys->unk2B4 == 0) {
        switch(param) {
            case 0:
            case 1:
            case 3:
                modeSprPos[0] = 288.0f;
                modeSprPos[1] = -96.0f;
                modeSprPos[2] = 0.0f;
                modeSprPos[3] = 288.0f;
                modeSprPos[4] = 88.0f;
                modeSprPos[5] = 0.0f;
                break;

            case 2:
                modeSprPos[0] = 288.0f;
                modeSprPos[1] = 88.0f;
                modeSprPos[2] = 0.0f;
                modeSprPos[3] = 288.0f;
                modeSprPos[4] = 128.0f;
                modeSprPos[5] = 0.0f;
                break;
        }
        switch(param) {
            case 1:
            case 2:
                break;

            case 0:
                HuSprGrpPosSet(grpId, modeSprPos[0], modeSprPos[1]);
                if(sys->selMode == 0) {
                    MdSpriteDispSet(sprite, MDSPR_GRP_MODE1, TRUE);
                } else if(sys->selMode == 1) {
                    MdSpriteDispSet(sprite, MDSPR_GRP_MODE2, TRUE);
                }
                break;

            case 3:
                HuSprGrpPosSet(grpId, modeSprPos[3], modeSprPos[4]);
                if(sys->selMode == 0) {
                    MdSpriteDispSet(sprite, MDSPR_GRP_MODE1, TRUE);
                } else if(sys->selMode == 1) {
                    MdSpriteDispSet(sprite, MDSPR_GRP_MODE2, TRUE);
                }
                break;
        }
        sys->unk2B4 = 1;
    }
    switch(param) {
        case 0:
            MdMathOscillate(MDMATH_OSC_90, &sys->oscIn[0], &sys->oscOut[0], 30);
            MdMathLerp(&x, modeSprPos[0], modeSprPos[3], sys->oscOut[0]);
            MdMathLerp(&y, modeSprPos[1], modeSprPos[4], sys->oscOut[0]);
            HuSprGrpPosSet(grpId, x, y);
            break;

        case 1:
            MdMathOscillate(MDMATH_OSC_90_REV, &sys->oscIn[0], &sys->oscOut[0], 30);
            MdMathLerp(&x, modeSprPos[3], modeSprPos[0], sys->oscOut[0]);
            MdMathLerp(&y, modeSprPos[4], modeSprPos[1], sys->oscOut[0]);
            HuSprGrpPosSet(grpId, x, y);
            break;

        case 2:
            MdMathOscillate(MDMATH_OSC_90, &sys->oscIn[0], &sys->oscOut[0], 15);
            MdMathLerp(&x, modeSprPos[0], modeSprPos[3], sys->oscOut[0]);
            MdMathLerp(&y, modeSprPos[1], modeSprPos[4], sys->oscOut[0]);
            HuSprGrpPosSet(grpId, x, y);
            break;

        case 3:
            sys->oscIn[0] = 1;
            break;
    }
    return sys->oscIn[0] >= 1;
}

/* fn_1_12894: VS game model slide/scale in and out (param 0..2) */
BOOL fn_1_12894(int param)
{
    MDMODEL_WORK *model = sys->model;
    int no = MDMODEL_VS_VOLLEY+sys->selMode;
    HuVecF *posP = model->posP[no];
    HuVecF *rotP = model->rotP[no];
    HuVecF *scaleP = model->scaleP[no];
    if(sys->unk2B0 == 0) {
        if(param == 0) {
            *posP = vsMdlPosTbl[0];
            *rotP = vsMdlRotTbl[0];
            scaleP->x = 3.0f;
            scaleP->y = 3.0f;
            scaleP->z = 3.0f;
            Hu3DModelAttrReset(model->mdlId[no], HU3D_ATTR_DISPOFF);
        } else if(param == 2) {
            *posP = vsMdlPosTbl[1];
            *rotP = vsMdlRotTbl[1];
            scaleP->x = 1.0f;
            scaleP->y = 1.0f;
            scaleP->z = 1.0f;
            Hu3DModelAttrReset(model->mdlId[no], HU3D_ATTR_DISPOFF);
        }
        sys->unk2B0 = 1;
    }
    switch(param) {
        case 0:
            MdMathOscillate(MDMATH_OSC_90, &sys->oscIn[4], &sys->oscOut[4], 30);
            MdMathLerp(&posP->x, vsMdlPosTbl[0].x, vsMdlPosTbl[1].x, sys->oscOut[4]);
            MdMathLerp(&posP->y, vsMdlPosTbl[0].y, vsMdlPosTbl[1].y, sys->oscOut[4]);
            MdMathLerp(&posP->z, vsMdlPosTbl[0].z, vsMdlPosTbl[1].z, sys->oscOut[4]);
            MdMathLerp(&rotP->x, vsMdlRotTbl[0].x, vsMdlRotTbl[1].x, sys->oscOut[4]);
            MdMathLerp(&rotP->y, vsMdlRotTbl[0].y, vsMdlRotTbl[1].y, sys->oscOut[4]);
            MdMathLerp(&rotP->z, vsMdlRotTbl[0].z, vsMdlRotTbl[1].z, sys->oscOut[4]);
            MdMathLerpScale(scaleP, 3.0f, 1.0f, sys->oscOut[4]);
            break;

        case 1:
            MdMathOscillate(MDMATH_OSC_90, &sys->oscIn[4], &sys->oscOut[4], 30);
            MdMathLerp(&posP->x, vsMdlPosTbl[1].x, vsMdlPosTbl[2].x, sys->oscOut[4]);
            MdMathLerp(&posP->y, vsMdlPosTbl[1].y, vsMdlPosTbl[2].y, sys->oscOut[4]);
            MdMathLerp(&posP->z, vsMdlPosTbl[1].z, vsMdlPosTbl[2].z, sys->oscOut[4]);
            MdMathLerp(&rotP->x, vsMdlRotTbl[1].x, vsMdlRotTbl[2].x, sys->oscOut[4]);
            MdMathLerp(&rotP->y, vsMdlRotTbl[1].y, vsMdlRotTbl[2].y, sys->oscOut[4]);
            MdMathLerp(&rotP->z, vsMdlRotTbl[1].z, vsMdlRotTbl[2].z, sys->oscOut[4]);
            MdMathLerpScale(scaleP, 1.0f, 0.0f, sys->oscOut[4]);
            break;

        case 2:
            sys->oscIn[4] = 1;
            break;
    }
    if(sys->oscIn[4] >= 1) {
        if(param == 1) {
            Hu3DModelAttrSet(model->mdlId[no], HU3D_ATTR_DISPOFF);
        }
        return TRUE;
    }
    return FALSE;
}

/* fn_1_12E6C: ball model (BALL_NORMAL..) show/scale/move (param 0..3) */
BOOL fn_1_12E6C(int param)
{
    MDCAMERA_WORK *camera = sys->camera;
    MDMODEL_WORK *model = sys->model;
    int i;
    HuVecF *posP;
    if(sys->unk2A8 == 0) {
        switch(param) {
            case 1:
                break;

            case 0:
                ballPos2D[0] = ballPos2DTbl[1];
                Hu3D2Dto3D(&ballPos2D[0], camera->camera[MDCAMERA_MAIN].bit, &ballPos3D[0]);
                for(i=0; i<3; i++) {
                    *model->posP[MDMODEL_BALL_NORMAL+i] = ballPos3D[0];
                    *model->rotP[MDMODEL_BALL_NORMAL+i] = ballRotTbl[i];
                    model->scaleP[MDMODEL_BALL_NORMAL+i]->x = 0.0f;
                    model->scaleP[MDMODEL_BALL_NORMAL+i]->y = 0.0f;
                    model->scaleP[MDMODEL_BALL_NORMAL+i]->z = 0.0f;
                    Hu3DModelAttrReset(model->mdlId[MDMODEL_BALL_NORMAL+i], HU3D_ATTR_DISPOFF);
                }
                break;

            case 2:
                ballPos2D[0] = ballPos2DTbl[1];
                ballPos2D[1] = ballPos2DTbl[2];
                ballPos2D[2] = ballPos2DTbl[0];
                ballPos2D[3] = ballPos2DTbl[1];
                for(i=0; i<2; i++) {
                    Hu3D2Dto3D(&ballPos2D[i], camera->camera[MDCAMERA_MAIN].bit, &ballPos3D[i]);
                    Hu3D2Dto3D(&ballPos2D[i+2], camera->camera[MDCAMERA_MAIN].bit, &ballPos3D[i+2]);
                }
                break;

            case 3:
                ballPos2D[0] = ballPos2DTbl[1];
                Hu3D2Dto3D(&ballPos2D[0], camera->camera[MDCAMERA_MAIN].bit, &ballPos3D[0]);
                *model->posP[MDMODEL_BALL_NORMAL+sys->unk58] = ballPos3D[0];
                *model->rotP[MDMODEL_BALL_NORMAL+sys->unk58] = ballRotTbl[sys->unk58];
                model->scaleP[MDMODEL_BALL_NORMAL+sys->unk58]->x = 4.0f;
                model->scaleP[MDMODEL_BALL_NORMAL+sys->unk58]->y = 4.0f;
                model->scaleP[MDMODEL_BALL_NORMAL+sys->unk58]->z = 4.0f;
                Hu3DModelAttrReset(model->mdlId[MDMODEL_BALL_NORMAL+sys->unk58], HU3D_ATTR_DISPOFF);
                for(i=0; i<3; i++) {
                    if(i != sys->unk58) {
                        *model->posP[MDMODEL_BALL_NORMAL+i] = ballPos3D[0];
                        *model->rotP[MDMODEL_BALL_NORMAL+i] = ballRotTbl[i];
                        model->scaleP[MDMODEL_BALL_NORMAL+i]->x = 0.0f;
                        model->scaleP[MDMODEL_BALL_NORMAL+i]->y = 0.0f;
                        model->scaleP[MDMODEL_BALL_NORMAL+i]->z = 0.0f;
                        Hu3DModelAttrReset(model->mdlId[MDMODEL_BALL_NORMAL+i], HU3D_ATTR_DISPOFF);
                    }
                }
                break;
        }
        sys->unk2A8 = 1;
    }
    switch(param) {
        case 0:
            MdMathOscillate(MDMATH_OSC_90_REV, &sys->oscIn[0], &sys->oscOut[0], 15);
            MdMathOscillate(MDMATH_OSC_180, &sys->oscIn[1], &sys->oscOut[1], 15);
            MdMathLerpScaleOfs(model->scaleP[MDMODEL_BALL_NORMAL+sys->unk58], 0.0f, 4.0f, sys->oscOut[0], sys->oscOut[1]);
            break;

        case 1:
            MdMathOscillate(MDMATH_OSC_90_REV, &sys->oscIn[0], &sys->oscOut[0], 15);
            MdMathLerpScale(model->scaleP[MDMODEL_BALL_NORMAL+sys->unk58], 4.0f, 0.0f, sys->oscOut[0]);
            break;

        case 2:
            MdMathOscillate(MDMATH_OSC_90, &sys->oscIn[0], &sys->oscOut[0], 15);
            MdMathLerpScale(model->scaleP[MDMODEL_BALL_NORMAL+sys->unk59], 4.0f, 0.0f, sys->oscOut[0]);
            MdMathLerpScale(model->scaleP[MDMODEL_BALL_NORMAL+sys->unk58], 8.0f, 4.0f, sys->oscOut[0]);
            posP = model->posP[MDMODEL_BALL_NORMAL+sys->unk59];
            MdMathLerp(&posP->x, ballPos3D[0].x, ballPos3D[1].x, sys->oscOut[0]);
            MdMathLerp(&posP->y, ballPos3D[0].y, ballPos3D[1].y, sys->oscOut[0]);
            MdMathLerp(&posP->z, ballPos3D[0].z, ballPos3D[1].z, sys->oscOut[0]);
            posP = model->posP[MDMODEL_BALL_NORMAL+sys->unk58];
            MdMathLerp(&posP->x, ballPos3D[2].x, ballPos3D[3].x, sys->oscOut[0]);
            MdMathLerp(&posP->y, ballPos3D[2].y, ballPos3D[3].y, sys->oscOut[0]);
            MdMathLerp(&posP->z, ballPos3D[2].z, ballPos3D[3].z, sys->oscOut[0]);
            sys->unk2C4 = 3;
            MdProcCall(MDBEACH_PROC_CHR_SEL, sys->unk2C0+2);
            break;

        case 3:
            sys->oscIn[0] = 1;
            break;
    }
    return sys->oscIn[0] >= 1;
}

/* fn_1_13850: fade sprite (MODEL group member 2) in/out (param 0..1) */
BOOL fn_1_13850(int param)
{
    float tpLvl;
    HUSPRGRPID grpId = sys->sprite->grpId[MDSPR_GRP_MODEL];
    if(sys->unk2A8 == 0) {
        if(param == 0) {
            HuSprTPLvlSet(grpId, 2, 0.0f);
        } else if(param == 1) {
            HuSprTPLvlSet(grpId, 2, 0.5f);
        }
        sys->unk2A8 = 1;
    }
    if(param == 0) {
        MdMathOscillate(MDMATH_OSC_90, &sys->oscIn[0], &sys->oscOut[0], 15);
        MdMathLerp(&tpLvl, 0.0f, 0.5f, sys->oscOut[0]);
    } else if(param == 1) {
        MdMathOscillate(MDMATH_OSC_90_REV, &sys->oscIn[0], &sys->oscOut[0], 15);
        MdMathLerp(&tpLvl, 0.0f, 0.5f, sys->oscOut[0]);
    }
    HuSprTPLvlSet(grpId, 2, tpLvl);
    return sys->oscIn[0] >= 1;
}

/* fn_1_13A2C: scale up finger models of enabled players (param 0) */
BOOL fn_1_13A2C(int param)
{
    MDMODEL_WORK *model = sys->model;
    HuVecF *scaleP[4];
    int i;
    for(i=0; i<4; i++) {
        scaleP[i] = model->scaleP[MDMODEL_FINGER_START+i];
    }
    if(sys->unk2A8 == 0) {
        if(param == 0) {
            for(i=0; i<4; i++) {
                if(sys->padEnable[i]) {
                    scaleP[i]->x = 0.0f;
                    scaleP[i]->y = 0.0f;
                    scaleP[i]->z = 0.0f;
                    Hu3DModelAttrReset(model->mdlId[MDMODEL_FINGER_START+i], HU3D_ATTR_DISPOFF);
                }
            }
            fn_1_146D0();
        }
        sys->unk2A8 = 1;
    }
    if(param == 0) {
        MdMathOscillate(MDMATH_OSC_90_REV, &sys->oscIn[0], &sys->oscOut[0], 15);
        for(i=0; i<4; i++) {
            if(sys->padEnable[i]) {
                MdMathLerpScale(scaleP[i], 0.0f, 4.0f, sys->oscOut[0]);
            }
        }
    }
    if(param == 0) {
        return sys->oscIn[0] >= 1;
    }
    return param; /* original returns r3 untouched, i.e. param */
}

/* ---- part I ---- */


// fn_1_13CAC, fn_1_13EA4, fn_1_14050, fn_1_14354, fn_1_146D0, fn_1_14A68, fn_1_14F44, fn_1_15048,
// fn_1_153C8, fn_1_15418, fn_1_15508, fn_1_15A40, fn_1_15BC8

/* ---------------- PROTOS ---------------- */

/* ---------------- DATA ---------------- */
// finger (hand cursor) screen offsets, [selMode][chrNo]
static const HuVecF mdFingerOfs[2][MDMODEL_CHAR_NUM] = {
    {
        { -25.0f, -140.0f, 2500.0f }, { -25.0f, -140.0f, 2500.0f }, { -28.0f, -160.0f, 2500.0f },
        { -55.0f, -114.0f, 2500.0f }, { -33.0f, -147.0f, 2500.0f }, { -28.0f, -152.0f, 2500.0f },
        { -26.0f, -153.0f, 2500.0f }, { -31.0f, -116.0f, 2500.0f }, { -26.0f, -133.0f, 2500.0f },
        { -31.0f, -133.0f, 2500.0f },
    },
    {
        { -25.0f, -132.0f, 2500.0f }, { -25.0f, -143.0f, 2500.0f }, { -25.0f, -152.0f, 2500.0f },
        { -27.0f, -130.0f, 2500.0f }, { -31.0f, -131.0f, 2500.0f }, { -27.0f, -147.0f, 2500.0f },
        { -25.0f, -157.0f, 2500.0f }, { -24.0f, -126.0f, 2500.0f }, { -27.0f, -129.0f, 2500.0f },
        { -27.0f, -127.0f, 2500.0f },
    },
};

// character cursor sprite offsets, [chrNo]
static const HuVecF mdCursorOfs[MDMODEL_CHAR_NUM] = {
    { 0.0f, -85.0f, 0.0f },  { 0.0f, -90.0f, 0.0f },  { 0.0f, -102.0f, 0.0f }, { 0.0f, -84.0f, 0.0f },
    { 0.0f, -80.0f, 0.0f },  { 0.0f, -102.0f, 0.0f }, { 0.0f, -115.0f, 0.0f }, { 0.0f, -66.0f, 0.0f },
    { 0.0f, -80.0f, 0.0f },  { 0.0f, -60.0f, 0.0f },
};

// character select grid: [chrNo][0..3] = candidates on up, [4..7] = candidates on down
static const s8 mdChrGridMove[MDMODEL_CHAR_NUM][8] = {
    { 6, 9, 7, 8, 6, 7, 9, 8 }, { 7, 6, 8, 9, 7, 8, 6, 9 }, { 7, 8, 6, 9, 8, 7, 9, 6 },
    { 7, 8, 6, 9, 8, 7, 9, 6 }, { 8, 7, 9, 6, 8, 9, 7, 6 }, { 9, 8, 6, 7, 9, 6, 8, 7 },
    { 0, 5, 1, 4, 0, 1, 5, 2 }, { 1, 0, 2, 5, 1, 2, 0, 3 }, { 4, 3, 5, 2, 4, 5, 3, 0 },
    { 5, 4, 0, 3, 5, 0, 4, 1 },
};

// team layout per selMapNo: player order [0..3]
static const s8 mdTeamPlayerOrder[3][4] = {
    { 0, 1, 2, 3 },
    { 0, 2, 1, 3 },
    { 0, 3, 1, 2 },
};

// team group number per selMapNo
static const s8 mdTeamGrpNo[3][4] = {
    { 0, 1, 2, 3 },
    { 0, 2, 1, 3 },
    { 0, 2, 3, 1 },
};

// character pair -> pair index (original table is 0x5C bytes: 45 pairs + 2 bytes padding)
static const s8 mdChrPairTbl[45][2] = {
    { 0, 1 }, { 0, 2 }, { 0, 3 }, { 0, 4 }, { 0, 5 }, { 0, 6 }, { 0, 7 }, { 0, 8 }, { 0, 9 },
    { 1, 2 }, { 1, 3 }, { 1, 4 }, { 1, 5 }, { 1, 6 }, { 1, 7 }, { 1, 8 }, { 1, 9 },
    { 2, 3 }, { 2, 4 }, { 2, 5 }, { 2, 6 }, { 2, 7 }, { 2, 8 }, { 2, 9 },
    { 3, 4 }, { 3, 5 }, { 3, 6 }, { 3, 7 }, { 3, 8 }, { 3, 9 },
    { 4, 5 }, { 4, 6 }, { 4, 7 }, { 4, 8 }, { 4, 9 },
    { 5, 6 }, { 5, 7 }, { 5, 8 }, { 5, 9 },
    { 6, 7 }, { 6, 8 }, { 6, 9 },
    { 7, 8 }, { 7, 9 },
    { 8, 9 },
};

// SE ids for sound types 18..27
static const int mdSeTbl[10] = { 0x201, 0x1ED, 0x265, 0x2B5, 0x28D, 0x1C5, 0x2A1, 0x1D9, 0x279, 0x23D };

static int mdBgmStream;

/* ---------------- FUNCTIONS ---------------- */

// builds the list of connected pads (player -> pad)
void fn_1_13CAC(void)
{
    int i;
    s16 j;

    sys->unk1D = 0;
    for (i = 0; i < 4; i++) {
        sys->chrSel[i] = -1;
    }
    for (j = 0; j < 4; j++) {
        if (HuPadStatGet(j) == 0) {
            sys->chrSel[sys->unk1D] = j;
            sys->unk1E[j] = 1;
            sys->unk1D++;
        } else {
            sys->unk1E[j] = 0;
        }
    }
    if (sys->unk1E[0] == 0) {
        for (i = 0; i < 4; i++) {
            sys->chrSel[i] = -1;
            sys->unk1E[i] = 0;
        }
        sys->chrSel[0] = 0;
        sys->unk1E[0] = 1;
        sys->unk1D = 1;
    }
    sys->unk1B = sys->unk1D - 1;
}

// builds the list of human players from GwPlayerConf
void fn_1_13EA4(void)
{
    s8 humanF[4] = { 0, -1, -1, -1 };
    int i;
    s8 j;

    sys->unk1D = 0;
    for (i = 0; i < 4; i++) {
        sys->chrSel[i] = -1;
    }
    for (i = 0; i < 4; i++) {
        if (GwPlayerConf[i].type == 0) {
            humanF[i] = 0;
        } else if (GwPlayerConf[i].type == 1) {
            humanF[i] = -1;
        }
    }
    for (j = 0; j < 4; j++) {
        if (humanF[j] == 0) {
            sys->chrSel[sys->unk1D] = j;
            sys->unk1E[j] = 1;
            sys->unk1D++;
        } else {
            sys->unk1E[j] = 0;
        }
    }
    sys->unk1B = sys->unk1D - 1;
}

void fn_1_14050(void)
{
    s8 usedF[4] = { -1, -1, -1, -1 };
    MDMODEL_WORK *model = sys->model;
    HUSPRGRPID grpId = sys->sprite->grpId[MDSPR_GRP_CHAR];
    int teamNum = 0;
    int i;
    s8 j;

    for (i = 0; i < 4; i++) {
        sys->chrSelEnd[i] = sys->chrSel[i];
        if (i > sys->unk1B) {
            sys->chrSelEnd[i] = -1;
        }
    }
    for (i = 0; i < 3; i++) {
        sys->teamPlayer[i] = -1;
    }
    for (i = 0; i < 4; i++) {
        if (sys->chrSelEnd[i] != -1) {
            usedF[sys->chrSelEnd[i]] = 0;
        }
    }
    for (j = 0; j < 4; j++) {
        if (usedF[j] == -1) {
            sys->teamPlayer[teamNum] = j;
            teamNum++;
            if (teamNum >= 3) {
                break;
            }
        }
    }
    for (i = 0; i < 4; i++) {
        sys->padEnable[i] = 0;
    }
    for (i = 0; i < 4; i++) {
        if (sys->chrSelEnd[i] != -1) {
            sys->padEnable[sys->chrSelEnd[i]] = 1;
        }
    }
    for (i = 0; i < 4; i++) {
        if (sys->padEnable[i] == 1) {
            HuSprBankSet(grpId, i, i);
        } else {
            HuSprBankSet(grpId, i, 4);
        }
    }
    for (i = 0; i < 4; i++) {
        if (sys->padEnable[i] == 1) {
            Hu3DAnmNoSet(model->toueiAnmId[i], i);
        } else {
            Hu3DAnmNoSet(model->toueiAnmId[i], 4);
        }
    }
}

// loads state from GwPlayer/GwPlayerConf/mgMatch* into sys
void fn_1_14354(void)
{
    int i;

    for (i = 0; i < 4; i++) {
        sys->chrNo[i] = GwPlayerConf[i].charNo;
        sys->unk3E[i] = GwPlayerConf[i].dif;
    }
    if (GwPlayer[1].grp == 0) {
        sys->selMapNo = 0;
    } else if (GwPlayer[2].grp == 0) {
        sys->selMapNo = 1;
    } else if (GwPlayer[3].grp == 0) {
        sys->selMapNo = 2;
    }
    fn_1_15048();
    fn_1_153C8();
    if (sys->selMode == 0) {
        switch (mgMatchGameType) {
            case 0:
                sys->unk58 = 0;
                break;
            case 1:
                sys->unk58 = 1;
                break;
            case 2:
                sys->unk58 = 2;
                break;
        }
        if ((u8)mgMatchMaxScore == 10) {
            sys->unk5A = 0;
        } else if ((u8)mgMatchMaxScore == 15) {
            sys->unk5A = 1;
        } else if ((u8)mgMatchMaxScore == 20) {
            sys->unk5A = 2;
        } else {
            sys->unk5A = 0;
        }
    } else if (sys->selMode == 1) {
        switch (mgMatchMaxScore) {
            case 1:
                sys->unk5A = 0;
                break;
            case 2:
                sys->unk5A = 1;
                break;
            case 3:
                sys->unk5A = 2;
                break;
            default:
                sys->unk5A = 0;
                break;
        }
    }
    for (i = 0; i < 4; i++) {
        sys->comPlayerNo[i] = 1;
    }
    sys->selPlayerNo = 3 - sys->unk1B;
}

// places finger/cursor sprites over the characters
void fn_1_146D0(void)
{
    MDCAMERA_WORK *camera = sys->camera;
    MDMODEL_WORK *model = sys->model;
    MDSPRITE_WORK *sprite = sys->sprite;
    HUSPRGRPID charGrp = sprite->grpId[MDSPR_GRP_CHAR];
    HUSPRGRPID capGrp = sprite->grpId[MDSPR_GRP_CAPMAN];
    HuVecF pos3D;
    HuVecF pos2D;
    s8 i;

    HuSprGrpPosSet(charGrp, 0.0f, 0.0f);
    HuSprGrpPosSet(capGrp, 0.0f, 0.0f);
    for (i = 0; i < 4; i++) {
        HuVecF *fingerPos = model->posP[MDMODEL_FINGER_START + i];
        HuVecF *ballPos = model->posP[MDMODEL_VOLLEY_START + sys->chrNo[i]];
        HuVecF *charPos;
        HuVec2f *cursorPosR;
        HuVec2f *cursorPosL;
        const HuVecF *ofs;

        if (sys->selMode == 0) {
            ballPos = model->posP[MDMODEL_VOLLEY_START + sys->chrNo[i]];
        } else if (sys->selMode == 1) {
            ballPos = model->posP[MDMODEL_HOCKEY_START + sys->chrNo[i]];
        }
        charPos = model->posP[MDMODEL_CHAR_START + sys->chrNo[i]];
        pos3D = *ballPos;
        Hu3D3Dto2D(&pos3D, camera->camera[MDCAMERA_MAIN].bit, &pos2D);
        ofs = &mdFingerOfs[sys->selMode][sys->chrNo[i]];
        pos2D.x += ofs->x;
        pos2D.y += ofs->y;
        pos2D.z = ofs->z;
        Hu3D2Dto3D(&pos2D, camera->camera[MDCAMERA_MAIN].bit, &pos3D);
        *fingerPos = pos3D;
        pos3D = *charPos;
        Hu3D3Dto2D(&pos3D, camera->camera[MDCAMERA_MAIN].bit, &pos2D);
        cursorPosR = MdSpritePosGet(sprite, MDSPR_GRP_CHAR, i);
        cursorPosL = MdSpritePosGet(sprite, MDSPR_GRP_CAPMAN, i);
        cursorPosL->x = cursorPosR->x = pos2D.x + mdCursorOfs[sys->chrNo[i]].x;
        cursorPosL->y = cursorPosR->y = pos2D.y + mdCursorOfs[sys->chrNo[i]].y;
    }
}

// moves a character selection with the pad; returns 1 if the selection was moved
int fn_1_14A68(s8 padNo, int mode)
{
    s8 takenF[10];
    int i;
    int padIdx = 0;
    u16 stk;

    if (sys->comPlayerNo[padNo] == 1) {
        return 0;
    }
    if (mode == 0) {
        padIdx = padNo;
    } else if (mode == 1) {
        padIdx = 0;
    }
    stk = sys->pad->DStkDown[padIdx];
    if (!(stk & 1) && !(stk & 2) && !(stk & 8) && !(stk & 4)) {
        return 0;
    }
    for (i = 0; i < 10; i++) {
        takenF[i] = -1;
    }
    for (i = 0; i < 4; i++) {
        if (mode == 0) {
            if (i == padNo || sys->padEnable[i] == 0) {
                continue;
            }
        } else if (mode == 1) {
            if (i == padNo || sys->comPlayerNo[i] == 0) {
                continue;
            }
        }
        takenF[sys->chrNo[i]] = 0;
    }
    sys->unk32[padNo] = sys->chrNo[padNo];
    if (sys->pad->DStkDown[padIdx] & 1) {
        for (i = 0; i < 4; i++) {
            sys->chrNo[padNo]--;
            if (sys->chrNo[padNo] == -1) {
                sys->chrNo[padNo] = 9;
            }
            if (takenF[sys->chrNo[padNo]] == -1) {
                break;
            }
        }
    } else if (sys->pad->DStkDown[padIdx] & 2) {
        for (i = 0; i < 4; i++) {
            sys->chrNo[padNo]++;
            sys->chrNo[padNo] = sys->chrNo[padNo] % 10;
            if (takenF[sys->chrNo[padNo]] == -1) {
                break;
            }
        }
    } else if (sys->pad->DStkDown[padIdx] & 8) {
        for (i = 0; i < 4; i++) {
            if (takenF[mdChrGridMove[sys->chrNo[padNo]][i]] == -1) {
                sys->chrNo[padNo] = mdChrGridMove[sys->chrNo[padNo]][i];
                break;
            }
        }
    } else if (sys->pad->DStkDown[padIdx] & 4) {
        for (i = 0; i < 4; i++) {
            if (takenF[mdChrGridMove[sys->chrNo[padNo]][4 + i]] == -1) {
                sys->chrNo[padNo] = mdChrGridMove[sys->chrNo[padNo]][4 + i];
                break;
            }
        }
    }
    return 1;
}

// picks a free character for the current CPU team player
void fn_1_14F44(void)
{
    s8 takenF[10];
    int i;
    s8 j;

    for (i = 0; i < 10; i++) {
        takenF[i] = -1;
    }
    for (i = 0; i < 4; i++) {
        if (sys->comPlayerNo[i] == 1) {
            takenF[sys->chrNo[i]] = 0;
        }
    }
    for (j = 0; j < 4; j++) {
        if (takenF[j] == -1) {
            sys->chrNo[sys->teamPlayer[sys->selPlayerNo]] = j;
            return;
        }
    }
}

// sets up team layout (chrSel2, chrGrpNo, pair numbers, cpu team players)
void fn_1_15048(void)
{
    int cpuNum = 0;
    int i;
    int k;
    s8 j;

    for (i = 0; i < 2; i++) {
        sys->chrSel2[i][0] = mdTeamPlayerOrder[sys->selMapNo][i * 2];
        sys->chrSel2[i][1] = mdTeamPlayerOrder[sys->selMapNo][i * 2 + 1];
    }
    for (i = 0; i < 4; i++) {
        sys->chrGrpNo[i] = mdTeamGrpNo[sys->selMapNo][i];
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 45; j++) {
            s8 a = sys->chrNo[sys->chrSel2[i][0]];
            s8 b = sys->chrNo[sys->chrSel2[i][1]];
            if ((a == mdChrPairTbl[j][0] && b == mdChrPairTbl[j][1])
                || (b == mdChrPairTbl[j][0] && a == mdChrPairTbl[j][1])) {
                sys->unk53[i] = j;
                break;
            }
        }
    }
    for (i = 0; i < 3; i++) {
        sys->unk55[i] = -1;
    }
    for (k = 0; k < 4; k++) {
        if (sys->padEnable[mdTeamPlayerOrder[sys->selMapNo][k]] == 0) {
            sys->unk55[cpuNum] = mdTeamPlayerOrder[sys->selMapNo][k];
            cpuNum++;
            if (cpuNum >= 3) {
                break;
            }
        }
    }
}

void fn_1_153C8(void)
{
    int i;

    for (i = 0; i < 4; i++) {
        sys->unk4F[i] = sys->chrGrpNo[i];
    }
}

// turns a model around Y to face a camera
void fn_1_15418(int modelNo, int cameraNo)
{
    MDCAMERA *camera = &sys->camera->camera[cameraNo];
    MDMODEL_WORK *model = sys->model;
    HuVecF *pos = model->posP[modelNo];
    HuVecF *rot = model->rotP[modelNo];
    HuVecF diff;

    diff.x = camera->pos.x - pos->x;
    diff.y = camera->pos.y - pos->y;
    diff.z = camera->pos.z - pos->z;
    rot->y = 180.0 * (atan2(diff.x, diff.z) / 3.141592653589793);
}

// writes sys state back to mgMatch* / GwPlayer / GwPlayerConf
void fn_1_15508(void)
{
    s8 i;

    if (sys->selMode == 0) {
        if (sys->unk58 == 0) {
            mgMatchGameType = 0;
        } else if (sys->unk58 == 1) {
            mgMatchGameType = 1;
        } else if (sys->unk58 == 2) {
            mgMatchGameType = 2;
        }
        if (sys->unk58 == 2) {
            if (sys->unk5A == 0) {
                mgMatchMaxScore = 0x60A;
            } else if (sys->unk5A == 1) {
                mgMatchMaxScore = 0x60F;
            } else if (sys->unk5A == 2) {
                mgMatchMaxScore = 0x614;
            }
        } else if (sys->unk5A == 0) {
            mgMatchMaxScore = 10;
        } else if (sys->unk5A == 1) {
            mgMatchMaxScore = 15;
        } else if (sys->unk5A == 2) {
            mgMatchMaxScore = 20;
        }
    } else if (sys->selMode == 1) {
        if (sys->unk5A == 0) {
            mgMatchMaxScore = 1;
        } else if (sys->unk5A == 1) {
            mgMatchMaxScore = 2;
        } else if (sys->unk5A == 2) {
            mgMatchMaxScore = 3;
        }
    }
    for (i = 0; i < 4; i++) {
        if (sys->padEnable[i] == 0) {
            GwPlayer[i].comF = 1;
        } else {
            GwPlayer[i].comF = 0;
        }
        GwPlayer[i].charNo = sys->chrNo[i];
        GwPlayer[i].dif = sys->unk3E[i];
        if (sys->chrSelEnd[i] != -1) {
            GwPlayer[i].padNo = sys->chrSel[i];
        } else {
            GwPlayer[i].padNo = -1;
        }
        if (sys->chrGrpNo[i] <= 1) {
            GwPlayer[i].grp = 0;
        } else {
            GwPlayer[i].grp = 1;
        }
        GwPlayerConf[i].charNo = GwPlayer[i].charNo;
        GwPlayer[i].padNo = i;
        GwPlayerConf[i].padNo = i;
        GwPlayerConf[i].dif = GwPlayer[i].dif;
        GwPlayerConf[i].type = GwPlayer[i].comF;
        GwPlayerConf[i].grp = GwPlayer[i].grp;
    }
}

// plays BGM / sound effects by type
void fn_1_15A40(int type)
{
    s16 pan;

    switch (type) {
        case 0:
            mdBgmStream = HuAudBGMPlay(0x25);
            break;
        case 1:
            HuAudSStreamFadeOut(mdBgmStream, 1000);
            break;
        case 2:
            HuAudFXPlay(0);
            break;
        case 3:
            HuAudFXPlay(2);
            break;
        case 4:
            HuAudFXPlay(1);
            break;
        case 5:
            HuAudFXPlay(3);
            break;
        case 6:
            HuAudFXPlay(0x3E1);
            break;
        case 17:
            HuAudFXPlay(0x3E3);
            break;
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
            HuAudFXPlayPan(0x3E2, fn_1_15BC8(type - 6, MDCAMERA_MAIN));
            break;
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
        case 27:
            HuAudFXPlayPan(mdSeTbl[type - 18], fn_1_15BC8(type - 17, MDCAMERA_MAIN));
            break;
        case 28:
            pan = fn_1_15BC8(MDMODEL_STAGE, MDCAMERA_GUIDE);
            HuAudFXPlayPan(0x2F2, pan);
            break;
        case 29:
            HuAudAllStop();
            break;
    }
}

// returns the stereo pan for a model's screen position
s16 fn_1_15BC8(int modelNo, int cameraNo)
{
    MDCAMERA *camera = &sys->camera->camera[cameraNo];
    HuVecF *pos = sys->model->posP[modelNo];
    HuVecF pos2D;

    Hu3D3Dto2D(pos, camera->bit, &pos2D);
    return (s16)(32.0f + 0.11111111f * pos2D.x);
}

/* ---- part J ---- */
// MdPosAdjustMode, fn_1_16DE8, ParticleDotCreate, ParticleDotPosSet, ParticleDotKill, ParticleGlowCreate, ParticleGlowPosSet, ParticleGlowKill

/* TYPES */

/* PROTOS */

/* MdPosAdjustMode: debug model position/rotation/scale editor */
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
        { "MARIO_VOLLEY", MDCAMERA_MAIN },
        { "LUIGI_VOLLEY", MDCAMERA_MAIN },
        { "PEACH_VOLLEY", MDCAMERA_MAIN },
        { "YOSHI_VOLLEY", MDCAMERA_MAIN },
        { "WARIO_VOLLEY", MDCAMERA_MAIN },
        { "DAISY_VOLLEY", MDCAMERA_MAIN },
        { "WALUIGI_VOLLEY", MDCAMERA_MAIN },
        { "KINOPIO_VOLLEY", MDCAMERA_MAIN },
        { "TERESA_VOLLEY", MDCAMERA_MAIN },
        { "MINIKOOPA_VOLLEY", MDCAMERA_MAIN },
        { "MARIO_HOCKEY", MDCAMERA_MAIN },
        { "LUIGI_HOCKEY", MDCAMERA_MAIN },
        { "PEACH_HOCKEY", MDCAMERA_MAIN },
        { "YOSHI_HOCKEY", MDCAMERA_MAIN },
        { "WARIO_HOCKEY", MDCAMERA_MAIN },
        { "DAISY_HOCKEY", MDCAMERA_MAIN },
        { "WALUIGI_HOCKEY", MDCAMERA_MAIN },
        { "KINOPIO_HOCKEY", MDCAMERA_MAIN },
        { "TERESA_HOCKEY", MDCAMERA_MAIN },
        { "MINIKOOPA_HOCKEY", MDCAMERA_MAIN },
        { "CAP_MAN_1P", MDCAMERA_MAIN },
        { "CAP_MAN_2P", MDCAMERA_MAIN },
        { "CAP_MAN_3P", MDCAMERA_MAIN },
        { "CAP_MAN_4P", MDCAMERA_MAIN },
        { "CAP_COM_1P", MDCAMERA_MAIN },
        { "CAP_COM_2P", MDCAMERA_MAIN },
        { "CAP_COM_3P", MDCAMERA_MAIN },
        { "CAP_COM_4P", MDCAMERA_MAIN },
        { "FINGER_1P", MDCAMERA_MAIN },
        { "FINGER_2P", MDCAMERA_MAIN },
        { "FINGER_3P", MDCAMERA_MAIN },
        { "FINGER_4P", MDCAMERA_MAIN },
        { "FLASH_1P", MDCAMERA_MAIN },
        { "FLASH_2P", MDCAMERA_MAIN },
        { "FLASH_3P", MDCAMERA_MAIN },
        { "FLASH_4P", MDCAMERA_MAIN },
        { "VS_VOLLEY", MDCAMERA_MAIN },
        { "VS_HOCKEY", MDCAMERA_MAIN },
        { "BALL_NORMAL", MDCAMERA_MAIN },
        { "BALL_BOMB", MDCAMERA_MAIN },
        { "BALL_DICE", MDCAMERA_MAIN },
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

/* empty function in original */
void fn_1_16DE8(void)
{
}

static MDBEACH_EFFECT *ParticleDotCreate(ANIMDATA *animP, s16 layer, s16 cameraBit)
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
    MDBEACH_EFFECT *effectP;
    effectP = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDBEACH_EFFECT), HU_MEMNUM_OVL);
    
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

static void ParticleDotPosSet(MDBEACH_EFFECT *effectP, HuVecF *pos)
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

static void ParticleDotKill(MDBEACH_EFFECT *effectP)
{
    Hu3DParManKill(effectP->parManId[0]);
    Hu3DParManKill(effectP->parManId[1]);
    Hu3DParManKill(effectP->parManId[2]);
}

static MDBEACH_EFFECT *ParticleGlowCreate(ANIMDATA *animP, s16 layer, s16 cameraBit)
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
    MDBEACH_EFFECT *effectP;
    effectP = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDBEACH_EFFECT), HU_MEMNUM_OVL);
    effectP->parManId[0] = parManId = Hu3DParManCreate(animP, 100, &param);
    Hu3DParManAttrSet(parManId, HU3D_PARMAN_ATTR_SCALEJITTER|HU3D_PARMAN_ATTR_RANDSCALE70|HU3D_PARMAN_ATTR_RANDSPEED70|HU3D_PARMAN_ATTR_TIMEUP);
    Hu3DParticleBlendModeSet(Hu3DParManModelIDGet(parManId), HU3D_PARTICLE_BLEND_ADDCOL);
    Hu3DParManRotSet(parManId, 90, 0, 0);
    Hu3DModelLayerSet(Hu3DParManModelIDGet(parManId), layer);
    Hu3DModelCameraSet(Hu3DParManModelIDGet(parManId), cameraBit);
    return effectP;
}

static void ParticleGlowPosSet(MDBEACH_EFFECT *effectP, HuVecF *pos)
{
    Hu3DParManPosSet(effectP->parManId[0], pos->x, pos->y, pos->z);
    Hu3DParManTimeLimitSet(effectP->parManId[0], 2);
    Hu3DParManAttrReset(effectP->parManId[0], HU3D_PARMAN_ATTR_TIMEUP);
}

static void ParticleGlowKill(MDBEACH_EFFECT *effectP)
{
    Hu3DParManKill(effectP->parManId[0]);
}

