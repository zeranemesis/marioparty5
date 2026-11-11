#include "game/audio.h"
#include "game/data.h"
#include "game/gamemes.h"
#include "game/gamework.h"
#include "game/hsfex.h"
#include "game/hu3d.h"
#include "game/mgdata.h"
#include "game/object.h"
#include "game/process.h"
#include "game/saveload.h"
#include "game/thpmain.h"
#include "game/wipe.h"
#include "game/board/main.h"

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

extern void fn_1_13E8(void);
extern s32 fn_1_2448(void);

void ObjectSetup(void);
static void fn_1_894(OMOBJ *obj);
static void fn_1_CB8(OMOBJ *obj);
static void fn_1_E90(OMOBJ *obj);
static void fn_1_1230(void);

#ifdef __MWERKS__
#define SM_RODATA __declspec(section ".rodata")
#else
#define SM_RODATA
#endif

static char lbl_1_data_0[] = "Load Time:%d\n";
static char lbl_1_data_E[] = "Finished!\n";
static s32 lbl_1_data_1C[7] = { 0xCC0000, 0xCE0000, 0xD00000, 0xD20000, 0xD40000, 0xD60000, 0xD80000 };

static s32 lbl_1_bss_0;
static s16 lbl_1_bss_4;
static s16 lbl_1_bss_6;
static s16 lbl_1_bss_8;
s32 lbl_1_bss_C;
s32 lbl_1_bss_10;
s32 lbl_1_bss_14[10];
s16 lbl_1_bss_3C;
s16 lbl_1_bss_3E;
s16 lbl_1_bss_40[2];
static OMOBJ *lbl_1_bss_44;
s32 lbl_1_bss_48;
static s32 lbl_1_bss_4C;
static OMOBJMAN *lbl_1_bss_50;

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

static void fn_1_A0(void)
{
    u32 start;

    lbl_1_bss_6 = 0;
    while (TRUE) {
        switch (lbl_1_bss_6) {
            case 0:
                break;
            case 1:
                start = OSGetTick();
                lbl_1_bss_4 = HuDataDirReadAsync(lbl_1_bss_0);
                while (HuDataGetAsyncStat(lbl_1_bss_4) == 0) {
                    HuPrcVSleep();
                }
                OSReport(lbl_1_data_0, (OSGetTick() - start) / (OS_TIMER_CLOCK / 1000));
                OSReport(lbl_1_data_E);
                lbl_1_bss_6 = 2;
                break;
            case 2:
                HuPrcEnd();
                break;
        }
        HuPrcVSleep();
    }
}

void fn_1_19C(s32 dirNum)
{
    lbl_1_bss_6 = 1;
    lbl_1_bss_0 = dirNum;
}

#ifdef TARGET_PC
// the original indexes storyBoardNo with storyNo even when it is -1
#define STORY_BOARD(i) GwCommon.storyBoardNo[((i) < 0) ? 0 : (i)]
#else
#define STORY_BOARD(i) GwCommon.storyBoardNo[i]
#endif

#define STORY_OVL_SELECT(ovl, board)                                                                              (ovl) = ((board) == 0)   ? DLL_w01sdll                                                                                 : ((board) == 1) ? DLL_w02sdll                                                                                 : ((board) == 2) ? DLL_w03sdll                                                                                 : ((board) == 3) ? DLL_w04sdll                                                                                 : ((board) == 4) ? DLL_w05sdll                                                                                 : ((board) == 5) ? DLL_w06sdll                                                                                                  : DLL_w07sdll

void ObjectSetup(void)
{
    Vec camPos = { 0.0f, 0.0f, 2000.0f };
    Vec camUp = { 0.0f, 1.0f, 0.0f };
    Vec camTarget = { 0.0f, 0.0f, 0.0f };
    Vec camPos2 = { 0.0f, 0.0f, 2000.0f };
    Vec lightPos = { 0.0f, 8000.0f, 500.0f };
    Vec lightDir = { 0.0f, -1.0f, 0.0f };
    GXColor lightColor = { 0xFF, 0xFF, 0xFF, 0xFF };
    Vec lightPos2 = { 0.0f, 0.0f, 3000.0f };
    Vec lightDir2 = { 0.0f, 0.0f, -1.0f };
    GXColor lightColor2 = { 0x80, 0x80, 0x80, 0xFF };
#ifdef TARGET_PC
    OMOVL ovl = DLL_w01sdll;
#else
    OMOVL ovl;
#endif
    s32 i;

    for (i = 0; i < 7; i++) {
        HuDataDirClose(lbl_1_data_1C[i]);
    }
    lbl_1_bss_50 = omInitObjMan(200, 0x2000);
    if (GwCommon.storyNo != -2 && GwCommon.storyNo < 5) {
        HuPrcVSleep();
        HuPrcChildCreate(fn_1_A0, 100, 0x3000, 0, lbl_1_bss_50);
    }
    omSystemKeyCheckSetup(lbl_1_bss_50);
    omSysPauseEnable(FALSE);
    if (mgPauseExitF != 0) {
        omSetStatBit(omAddObjEx(lbl_1_bss_50, 0x7FDC, 0, 0, -1, GameMesExitCheck), 0xA0);
    }
    if (_CheckFlag(0x10000)) {
        if (GwCommon.storyNo < 5) {
            omOvlHisChg(0, DLL_storyresultdll, 0, 0);
            STORY_OVL_SELECT(ovl, STORY_BOARD(GwCommon.storyNo));
        }
        omOvlCallEx(ovl, 1, 0, 0);
        return;
    }
    Hu3DCameraCreate(1);
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraPerspectiveSet(1, 20.0f, 20.0f, 30000.0f, 1.2f);
    Hu3DCameraPosSetV(1, &camPos, &camUp, &camTarget);
    Hu3DCameraCreate(2);
    Hu3DCameraViewportSet(2, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraPerspectiveSet(2, 20.0f, 20.0f, 30000.0f, 1.2f);
    Hu3DCameraPosSetV(2, &camPos2, &camUp, &camTarget);
    Hu3DCameraCreate(4);
    Hu3DCameraViewportSet(4, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraPerspectiveSet(4, 20.0f, 20.0f, 30000.0f, 1.2f);
    Hu3DCameraPosSetV(4, &camPos2, &camUp, &camTarget);
    lbl_1_bss_40[0] = Hu3DGLightCreateV(&lightPos, &lightDir, &lightColor);
    Hu3DGLightStaticSet(lbl_1_bss_40[0], 1);
    Hu3DGLightInfinitytSet(lbl_1_bss_40[0]);
    lbl_1_bss_40[1] = Hu3DGLightCreateV(&lightPos2, &lightDir2, &lightColor2);
    Hu3DGLightStaticSet(lbl_1_bss_40[1], 1);
    Hu3DGLightInfinitytSet(lbl_1_bss_40[1]);
    lbl_1_bss_44 = omAddObjEx(lbl_1_bss_50, 0, 0x6C, 0, -1, fn_1_894);
    lbl_1_bss_44->work[0] = 0;
}

static void fn_1_894(OMOBJ *obj)
{
    s32 state;
    s32 board;
    s8 boardSel;
    s32 mdlOfs;

    lbl_1_bss_48 = 0;
    if (GwCommon.storyNo == -2) {
        state = 0;
        lbl_1_bss_48 = -1;
    } else if (GwCommon.storyNo == -3) {
        state = 100;
    } else if (GwCommon.storyNo == 5) {
        state = 300;
        lbl_1_bss_48 = -2;
    } else if (GwCommon.storyNo < 6) {
        state = 200;
    } else if (GwPlayer->mgCoinBonus != 0) {
        state = 400;
        lbl_1_bss_48 = 1;
        if (GwCommon.storyPlayNum[GwPlayerConf->charNo] < 999) {
            GwCommon.storyPlayNum[GwPlayerConf->charNo]++;
        }
    } else {
        state = 300;
        lbl_1_bss_48 = -2;
    }
    if (state != 300) {
        if (GwCommon.storyNo < -1) {
            boardSel = GwCommon.storyBoardNo[0];
        } else {
            boardSel = STORY_BOARD(GwCommon.storyNo);
        }
        board = boardSel;
    } else {
        board = 7;
    }
    if (state == 0) {
        mdlOfs = 0x5B;
    } else {
        mdlOfs = 0;
    }
    lbl_1_bss_3E = Hu3DModelCreate(HuDataSelHeapReadNum(mdlOfs + 0xC10000, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelCameraSet(lbl_1_bss_3E, 1);
    Hu3DModelPosSet(lbl_1_bss_3E, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(lbl_1_bss_3E, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrSet(lbl_1_bss_3E, 0x40000001);
    Hu3DModelLayerSet(lbl_1_bss_3E, 1);
    if (state == 100 || state == 200 || state == 300) {
        lbl_1_bss_3C = Hu3DModelCreate(HuDataSelHeapReadNum(board + 0xC10001, HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelCameraSet(lbl_1_bss_3C, 1);
        Hu3DModelPosSet(lbl_1_bss_3C, 0.0f, 0.0f, 0.0f);
        Hu3DModelRotSet(lbl_1_bss_3C, 0.0f, 0.0f, 0.0f);
        Hu3DModelLayerSet(lbl_1_bss_3C, 1);
        if (state == 100 || state == 200) {
            Hu3DModelAttrSet(lbl_1_bss_3C, 1);
            Hu3DModelScaleSet(lbl_1_bss_3C, 0.0f, 0.0f, 0.0f);
        }
    }
    fn_1_13E8();
    fn_1_1230();
    HuDataDirClose(0xC10000);
    if (state == 0) {
        WipeColorSet(0, 0, 0);
    } else {
        WipeColorSet(0xFF, 0xFF, 0xFF);
    }
    WipeCreate(1, 0, 120);
    obj->objFunc = fn_1_CB8;
}

static void fn_1_CB8(OMOBJ *obj)
{
    s32 wipeTime;
    s32 next;
    s32 res;

    if (omSysExitReq != 0) {
        WipeCreate(2, 0, 60);
        obj->objFunc = fn_1_E90;
        lbl_1_bss_4C = -1;
    }
    switch (obj->work[0]) {
        case 0:
            if (lbl_1_bss_10 == -1) {
                lbl_1_bss_10 = HuAudBGMPlay(lbl_1_bss_C);
            }
            if (WipeCheckEnd() == 0) {
                obj->work[0]++;
            }
            break;
        case 1:
            res = fn_1_2448();
            if (res != 0) {
                obj->work[0]++;
                if (lbl_1_bss_48 > 0) {
                    next = -1;
                } else {
                    next = res;
                }
                lbl_1_bss_4C = next;
            }
            break;
        case 2:
            if (lbl_1_bss_48 == -1) {
                WipeColorSet(0, 0, 0);
                wipeTime = 1;
            } else {
                HuAudSStreamFadeOut(lbl_1_bss_10, 1000);
                WipeColorSet(0xFF, 0xFF, 0xFF);
                if (lbl_1_bss_48 == 1) {
                    wipeTime = 180;
                    HuAudFXPlay(0x583);
                } else {
                    wipeTime = 120;
                }
            }
            WipeCreate(2, 0, wipeTime);
            obj->objFunc = fn_1_E90;
            break;
    }
}

static void fn_1_E90(OMOBJ *obj)
{
    OMOVL ovl;
    s32 storyNo;
    s8 board;

    if (WipeCheckEnd() != 0) {
        return;
    }
    HuAudAllStop();
    Hu3DModelAllKill();
    Hu3DLightAllKill();
    if (lbl_1_bss_48 == -1) {
        omOvlReturnEx(0, 1);
        GwCommon.storyNo = -3;
        return;
    }
    if (lbl_1_bss_48 == -2) {
        HuTHPClose();
        while (HuTHPProcCheck() != 0) {
            HuPrcVSleep();
        }
    }
    if (lbl_1_bss_4C == -1) {
        if (lbl_1_bss_48 == 1) {
            omOvlGotoEx(DLL_staffdll, 1, 0, 0);
            return;
        }
        if (GwCommon.storyNo < 5) {
            SLSaveStoryExec();
        }
        if (GwCommon.storyNo == 6) {
            GwCommon.storyNo = 5;
        }
        omOvlReturnEx(1, 1);
        return;
    }
    if (GwCommon.storyNo < -1) {
        board = 0;
    } else {
        board = GwCommon.storyNo;
    }
    storyNo = board;
    if (storyNo < 5) {
        MBPartySaveInit(STORY_BOARD(storyNo));
        MBStorySaveInit(GwSystem.mgPack, 0);
        omOvlHisChg(0, DLL_storyresultdll, 0, 0);
        STORY_OVL_SELECT(ovl, STORY_BOARD(storyNo));
        if (GwCommon.storyNo != -2 && GwCommon.storyNo < 5) {
            while (lbl_1_bss_6 != 2) {
                HuPrcVSleep();
            }
        }
    } else {
        omOvlHisChg(0, DLL_storymodedll, 0, 0);
        if (storyNo == 5) {
            GwSystem.subGameNo = -1;
        }
        ovl = DLL_m562dll;
        GWMgUnlockSet(0x232);
    }
    omOvlCallEx(ovl, 1, 0, 0);
    if (GwCommon.storyNo < -1) {
        GwCommon.storyNo = 0;
        return;
    }
    if (GwCommon.storyNo == 5) {
        GwCommon.storyNo = 6;
    }
}

static void fn_1_1230(void)
{
    s32 i;

    for (i = 0; i < 10; i++) {
        lbl_1_bss_14[i] = 0;
    }
    lbl_1_bss_10 = -1;
}

s16 fn_1_1280(s16 sndId, s16 modelId, s16 camBit)
{
    Vec pos;
    HuVecF scr;
    s16 id;
    s32 pan;

    pos = Hu3DData[modelId].pos;
    Hu3D3Dto2D(&pos, camBit, &scr);
    if (scr.x < 0.0f || scr.x > 576.0f) {
        return -1;
    }
    if (scr.y < 0.0f || scr.y > 480.0f) {
        return -1;
    }
    pan = (s32)(scr.x / 9.0f) + 0x20;
    if ((sndId & 0x8000) == 0) {
        id = HuAudFXPlay(sndId & 0x7FF);
        if (sndId == 0x580) {
            lbl_1_bss_8 = id;
        }
    } else {
        id = lbl_1_bss_8;
    }
    HuAudFXPanning(id, pan);
    return id;
}
