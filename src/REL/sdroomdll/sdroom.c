#include "Runtime.PPCEABI.H/NMWException.h"
#include <stddef.h>
#include "REL/sdroomdll/sdroom.h"
#include "game/window.h"
#include "game/sprite.h"
#include "game/saveload.h"
#include "game/armem.h"
#include "game/charman.h"
#include "game/mgdata.h"
#include "game/memory.h"
#include "game/init.h"
#include "humath.h"
#include <math.h>

extern void (*_ctors[])(void);
extern void (*_dtors[])(void);

/* garage.c / garagesave.c / picture.c / tournament.c / desk.c / filename.c */
extern void fn_1_7298(void);
extern void fn_1_7320(void);
extern void fn_1_556C(void);
extern void fn_1_5854(s16 no);
extern void fn_1_13B1C(void);
extern void fn_1_13D08(void);
extern void fn_1_16024(void);
extern void fn_1_16064(void);
extern void fn_1_19324(void);
extern void fn_1_193C0(void);
extern void fn_1_1C25C(void);
extern void fn_1_1C260(void);
extern void fn_1_256B4(void);
extern void NameEnterInit(void);

static void fn_1_154C(void);
static void fn_1_17B0(void);
static void fn_1_18FC(void);
static void fn_1_1F40(OMOBJ *obj);
static void fn_1_35E8(void);
static void fn_1_49C(void);

s16 lbl_1_bss_0;
s32 lbl_1_bss_4;
s32 lbl_1_bss_8;
GWCOMMON lbl_1_bss_10;

static OMCAMERAVIEW lbl_1_data_0 = { { 0.0f, 250.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, 7000.0f };
static OMCAMERAVIEW lbl_1_data_1C = { { 0.0f, 165.0f, -891.0f }, { 0.0f, 0.0f, 0.0f }, 3267.0f };
static OMCAMERAVIEW lbl_1_data_38 = { { 0.0f, 215.0f, -896.0f }, { -3.8f, 0.0f, 0.0f }, 642.0f };
static OMCAMERAVIEW lbl_1_data_54 = { { 0.0f, 200.0f, -500.0f }, { 0.0f, 0.0f, 0.0f }, 500.0f };
static OMCAMERAVIEW lbl_1_data_70 = { { -433.0f, 200.0f, -250.0f }, { 0.0f, 60.0f, 0.0f }, 500.0f };
static OMCAMERAVIEW lbl_1_data_8C = { { -433.0f, 200.0f, 250.0f }, { 0.0f, 120.0f, 0.0f }, 500.0f };
static OMCAMERAVIEW lbl_1_data_A8 = { { 0.0f, 200.0f, -500.0f }, { 0.0f, 0.0f, 0.0f }, 500.0f };
static OMCAMERAVIEW lbl_1_data_C4 = { { 0.0f, 200.0f, -500.0f }, { 0.0f, 0.0f, 0.0f }, 500.0f };
static OMCAMERAVIEW lbl_1_data_E0 = { { 0.0f, 200.0f, 1961.0f }, { 0.0f, 180.0f, 0.0f }, 140.0f };
static u32 lbl_1_data_120[6] = { 0x190031, 0x19002D, 0x19002E, 0x190032, 0x190030, 0x19002F };

int _prolog(void)
{
    void (**ctor)(void) = _ctors;
    while (*ctor != NULL) {
        (*ctor)();
        ctor++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    void (**dtor)(void) = _dtors;
    while (*dtor != NULL) {
        (*dtor)();
        dtor++;
    }
}

void ObjectSetup(void)
{
    OSReport("******* SDR ObjectSetup *********\n");
    lbl_1_bss_5AC = omInitObjMan(0x32, 0x2000);
    lbl_1_bss_0 = 0;
    if (omovlevtno == 0) {
        if (SLSaveFlagGet() != 0) {
            SLSdLoad(0, SLCurBoxNoGet(), &GwSdCommon);
        }
        sdWorkInt[0] = 0;
        GwSdBackupNo = -1;
    }
    if (omovlevtno == 4) {
        HuAudAUXSet(4, 1);
    } else {
        HuAudAUXSet(7, 1);
    }
    omSysPauseEnable(0);
    Hu3DCameraCreate(0x3F);
    Hu3DCameraPerspectiveSet(0x3B, 5.0f, 20.0f, 15000.0f, 1.2f);
    Hu3DCameraViewportSet(0x3B, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraPerspectiveSet(4, 40.0f, 30.0f, 15000.0f, 1.2f);
    Hu3DCameraViewportSet(4, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    omCameraViewSetMulti(3, &lbl_1_data_0);
    omCameraViewSetMulti(4, &lbl_1_data_1C);
    lbl_1_bss_568[0] = Hu3DGLightCreate(1000.0f, 1000.0f, 1000.0f, -1.0f, -1.0f, -1.0f, 0xFF, 0xFF, 0xFF);
    Hu3DGLightInfinitytSet(lbl_1_bss_568[0]);
    lbl_1_bss_568[1] = Hu3DGLightCreate(100.0f, 100.0f, 100.0f, 1.0f, -1.0f, -1.0f, 0x18, 0x18, 0x18);
    Hu3DGLightInfinitytSet(lbl_1_bss_568[1]);
    omAddObjEx(lbl_1_bss_5AC, 0, 0x20, 0x20, -1, fn_1_1F40);
    Hu3DBGColorSet(0, 0, 0);
    HuWinInit(1);
    HuPrcChildCreate(fn_1_49C, 0x3E8, 0x3000, 0, lbl_1_bss_5AC);
    HuPrcChildCreate(fn_1_35E8, 0xA, 0x3000, 0, lbl_1_bss_5AC);
    Hu3DNoSyncSet(1);
    SdMachineFadeAlpha = 0.0f;
    SdMachineFadeCount = 0;
    /* the 11th item part is only available once unlocked */
    if (GwSdCommon.extCharSelF) {
        SdMachinePartNum[4] = 11;
    } else {
        SdMachinePartNum[4] = 10;
    }
}

/* Main garage-room process: stat update after a duel, then the room menu. */
static void fn_1_49C(void)
{
    OMCAMERAVIEW view;
    s16 sel;
    s16 pending;
    s16 i;
    s32 exitF;
    s32 reopen;
    GWSDBACKUP *backup;
    u16 *cnt;

    view = lbl_1_data_54;
    exitF = 0;
    reopen = 0;
    if (omovlevtno == 4) {
        fn_1_1C260();
    }
    if (omovlevtno == 2) {
        if (GwSdBattle.winnerNo == 0) {
            if (sdWorkInt[4] == 0) {
                cnt = &GwCommon.sdVsWinCnt[sdWorkInt[3]][sdWorkInt[7]];
                (*cnt)++;
                if (*cnt > 999) {
                    *cnt = 999;
                }
            }
        } else if (GwSdBattle.winnerNo == 1) {
            if (sdWorkInt[2] == 1) {
                s16 other = sdWorkInt[7] ^ 1;
                if (sdWorkInt[4] == 0) {
                    cnt = &GwCommon.sdVsWinCnt[sdWorkInt[3]][other];
                    (*cnt)++;
                    if (*cnt > 999) {
                        *cnt = 999;
                    }
                }
            } else {
                backup = &GwSdBackup[GwSdBackupNo];
                SLCommonLoadCopy(&lbl_1_bss_10, backup->slotNo, backup->boxNo);
                if (sdWorkInt[4] == 0) {
                    cnt = &lbl_1_bss_10.sdVsWinCnt[sdWorkInt[3]][backup->garageNo];
                    (*cnt)++;
                    if (*cnt > 999) {
                        *cnt = 999;
                    }
                }
                SLCommonSaveCopy(&lbl_1_bss_10, backup->slotNo, backup->boxNo);
            }
        }
        GwCommon.sdVsPlayCnt++;
        if (GwCommon.sdVsPlayCnt > 999) {
            GwCommon.sdVsPlayCnt = 999;
        }
        if (sdWorkInt[2] == 2) {
            backup = &GwSdBackup[GwSdBackupNo];
            SLCommonLoadCopy(&lbl_1_bss_10, backup->slotNo, backup->boxNo);
            lbl_1_bss_10.sdVsPlayCnt++;
            if (lbl_1_bss_10.sdVsPlayCnt > 999) {
                lbl_1_bss_10.sdVsPlayCnt = 999;
            }
            SLCommonSaveCopy(&lbl_1_bss_10, backup->slotNo, backup->boxNo);
            if (backup->slotNo == 1) {
                SLSaveSdExec();
            }
        }
    }
    fn_1_18FC();
    pending = 0;
    if (omovlevtno == 0) {
        HuAudBGMPlay(0x29);
        WipeCreate(1, 6, 0x3C);
        HuPrcSleep(0x1E);
        Hu3DModelAttrSet(lbl_1_bss_58C[4], 1);
        Hu3DModelAttrSet(lbl_1_bss_58C[6], 1);
        omCameraViewMoveMulti(4, &lbl_1_data_38, 0x5A, 0);
        while (WipeCheckEnd()) {
            HuPrcVSleep();
        }
        while (omCameraViewCheck(4) == 0) {
            HuPrcVSleep();
        }
        HuPrcChildCreate(fn_1_154C, 0x3E8, 0x3000, 0, lbl_1_bss_5AC);
        HuWinExOpen(lbl_1_bss_570);
        HuWinMesSet(lbl_1_bss_570, 0x190001);
        HuWinMesWait(lbl_1_bss_570);
        if (!GwSdCommon.unkFlag) {
            HuWinMesSet(lbl_1_bss_570, 0x190002);
            HuAudFXPlay(0x2EF);
            HuWinMesWait(lbl_1_bss_570);
            GwSdCommon.unkFlag = 1;
            GwSdCommon.prevPoint = 0;
            for (i = 0; i < 4; i++) {
                GwSdCommon.prevPoint += GwCommon.sdPoint[i];
            }
        }
        lbl_1_bss_8 = 0;
        HuWinExClose(lbl_1_bss_570);
        omCameraViewMoveMulti(4, &lbl_1_data_54, 0x3C, 0);
        HuPrcSleep(0x28);
        HuWinExOpen(lbl_1_bss_570);
        lbl_1_bss_56C = 0;
        view = lbl_1_data_54;
        HuWinMesSet(lbl_1_bss_570, lbl_1_data_120[lbl_1_bss_56C]);
        HuWinMesWait(lbl_1_bss_570);
        HuWinDispOn(lbl_1_bss_56E);
        HuWinMesSet(lbl_1_bss_56E, 0x20004);
    } else if (omovlevtno == 1) {
        HuAudBGMPlay(0x29);
        lbl_1_bss_56C = 0;
        view = lbl_1_data_54;
        omCameraViewSetMulti(4, &lbl_1_data_38);
        WipeCreate(1, 0, 0x3C);
        while (WipeCheckEnd()) {
            HuPrcVSleep();
        }
        fn_1_256B4();
        reopen = 1;
    } else if (omovlevtno == 2 || omovlevtno == 3) {
        HuAudBGMPlay(0x29);
        if (sdWorkInt[7] == 1) {
            lbl_1_bss_56C = 2;
            view = lbl_1_data_8C;
            fn_1_5854(1);
        } else if (sdWorkInt[7] == 0) {
            lbl_1_bss_56C = 1;
            view = lbl_1_data_70;
            fn_1_5854(0);
        } else {
            lbl_1_bss_56C = 5;
            view = lbl_1_data_A8;
            fn_1_5854(0);
        }
        reopen = 1;
    } else if (omovlevtno == 4) {
        fn_1_1C260();
        HuAudBGMPlay(0x29);
    }

    while (1) {
        if (!reopen) {
            sel = 0;
            if (fn_1_507C(7) & 1) {
                sel = 1;
            } else if (fn_1_507C(7) & 2) {
                sel = -1;
            } else if (fn_1_4FD0(7) & 0x200) {
                sel = 3 - lbl_1_bss_56C;
            }
            if (pending != 0) {
                sel = pending;
                pending = 0;
            }
            if (sel != 0) {
                lbl_1_bss_56C += sel;
                if (lbl_1_bss_56C < 0) {
                    lbl_1_bss_56C = 5;
                }
                if (lbl_1_bss_56C > 5) {
                    lbl_1_bss_56C = 0;
                }
                view.center.x = 500.0 * -HuSin(lbl_1_bss_56C * 60);
                view.center.z = 500.0 * -HuCos(lbl_1_bss_56C * 60);
                view.rot.y = lbl_1_bss_56C * 60;
                if (CRotM[2].y < 0.0f) {
                    CRotM[2].y += 360.0f;
                }
                if (CRotM[2].y >= 360.0f) {
                    CRotM[2].y -= 360.0f;
                }
                omCameraViewMoveMulti(4, &view, 0x3C, 0);
                HuAudFXPlay(0);
                for (i = 0; i < 30; i++) {
                    fn_1_17B0();
                    if (fn_1_4FD0(7) & 1) {
                        pending = 1;
                    } else if (fn_1_4FD0(7) & 2) {
                        pending = -1;
                    }
                    HuPrcVSleep();
                }
                HuWinMesSet(lbl_1_bss_570, lbl_1_data_120[lbl_1_bss_56C]);
                HuWinMesWait(lbl_1_bss_570);
            }
            if (!(fn_1_4FD0(7) & 0x100)) {
                HuPrcVSleep();
                continue;
            }
            if (lbl_1_bss_56C == 0 || lbl_1_bss_56C == 4) {
                if ((fn_1_4FD0(2) & 0x100) || (fn_1_4FD0(4) & 0x100)) {
                    HuPrcVSleep();
                    continue;
                }
            }
            HuAudFXPlay(1);
            HuWinDispOff(lbl_1_bss_56E);
            switch (lbl_1_bss_56C) {
                case 0:
                    HuWinExClose(lbl_1_bss_570);
                    omCameraViewMoveMulti(4, &lbl_1_data_38, 0x3C, 0);
                    HuPrcSleep(0x3C);
                    fn_1_256B4();
                    break;
                case 1:
                    if (!(SdMachinePadBtn[0] & 0x100)) {
                        HuWinMesSet(lbl_1_bss_570, 0x190063);
                        HuWinMesWait(lbl_1_bss_570);
                    } else {
                        HuWinExClose(lbl_1_bss_570);
                        fn_1_5854(0);
                    }
                    break;
                case 2:
                    if (!(SdMachinePadBtn[1] & 0x100)) {
                        HuWinMesSet(lbl_1_bss_570, 0x190064);
                        HuWinMesWait(lbl_1_bss_570);
                    } else {
                        HuWinExClose(lbl_1_bss_570);
                        fn_1_5854(1);
                    }
                    break;
                case 3:
                    HuWinMesSet(lbl_1_bss_570, 0x190005);
                    HuWinMesWait(lbl_1_bss_570);
                    if (HuWinChoiceGet(lbl_1_bss_570, -1) == 0) {
                        HuWinExClose(lbl_1_bss_570);
                        omCameraViewMoveMulti(4, &lbl_1_data_E0, 0x3C, 0);
                        HuPrcSleep(0x3C);
                        exitF = 1;
                    }
                    break;
                case 4:
                    HuWinExClose(lbl_1_bss_570);
                    fn_1_193C0();
                    break;
                case 5:
                    if (!(SdMachinePadBtn[2] & 0x100)) {
                        HuWinMesSet(lbl_1_bss_570, 0x190065);
                        HuWinMesWait(lbl_1_bss_570);
                    } else if (SLSaveFlagGet() == 0) {
                        HuWinMesSet(lbl_1_bss_570, 0x19005C);
                        HuWinMesWait(lbl_1_bss_570);
                    } else {
                        HuWinExClose(lbl_1_bss_570);
                        fn_1_13D08();
                    }
                    break;
            }
        }
        reopen = 0;
        if (exitF != 0) {
            break;
        }
        omCameraViewMoveMulti(4, &view, 0x3C, 0);
        HuPrcSleep(0x1E);
        HuWinExOpen(lbl_1_bss_570);
        HuWinMesSet(lbl_1_bss_570, lbl_1_data_120[lbl_1_bss_56C]);
        HuWinMesWait(lbl_1_bss_570);
        HuWinMesSet(lbl_1_bss_56E, 0x20004);
        HuWinDispOn(lbl_1_bss_56E);
        fn_1_17B0();
        HuPrcVSleep();
    }
    if (SLSaveFlagGet() != 0) {
        SLSdSave(0, SLCurBoxNoGet(), &GwSdCommon);
    }
    HuARDirFree(0xBC0000);
    HuARDirFree(0xBE0000);
    WipeCreate(2, 6, 0x3C);
    HuAudSStreamAllFadeOut(1000);
    while (WipeCheckEnd()) {
        HuPrcVSleep();
    }
    if (RenderMode->aa) {
        GXSetPixelFmt(GX_PF_RGB565_Z16, GX_ZC_LINEAR);
    } else {
        GXSetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);
    }
    omOvlReturnEx(1, 1);
    while (1) {
        HuPrcVSleep();
    }
}

static void fn_1_154C(void)
{
    HUSPRGRPID grpId;
    HUSPRID sprId;
    s16 i;

    lbl_1_bss_8 = 1;
    grpId = HuSprGrpCreate(1);
    sprId = HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(0xBE002E, HU_MEMNUM_OVL, HEAP_MODEL)), 100, 0);
    HuSprGrpMemberSet(grpId, 0, sprId);
    HuSprGrpPosSet(grpId, 0.0f, 0.0f);

    for (i = 1; i <= 10; i++) {
        HuSprPosSet(grpId, 0, 288.0f, 64.0 - 120.0 * (1.0 - HuSin(90.0 * ((f64)i / 10.0))));
        HuPrcVSleep();
    }

    while (lbl_1_bss_8 != 0) {
        HuPrcVSleep();
    }

    for (i = 1; i <= 10; i++) {
        HuSprPosSet(grpId, 0, 288.0f, 64.0 - 120.0 * (1.0 - HuCos(90.0 * ((f64)i / 10.0))));
        HuPrcVSleep();
    }

    HuSprGrpKill(grpId);
    HuPrcEnd();

    while (1) {
        HuPrcVSleep();
    }
}

static void fn_1_17B0(void)
{
    f32 angle;

    angle = CRotM[2].y;
    if (angle < 0.0f) {
        angle += 360.0f;
    }
    if (angle >= 360.0f) {
        angle -= 360.0f;
    }
    if (angle < 180.0f) {
        Hu3DModelAttrReset(lbl_1_bss_58C[3], 1);
        Hu3DModelAttrReset(lbl_1_bss_58C[4], 1);
        Hu3DModelAttrSet(lbl_1_bss_58C[2], 1);
        Hu3DModelAttrSet(lbl_1_bss_58C[6], 1);
        fn_1_7298();
        fn_1_16064();
    } else {
        Hu3DModelAttrSet(lbl_1_bss_58C[3], 1);
        Hu3DModelAttrSet(lbl_1_bss_58C[4], 1);
        Hu3DModelAttrReset(lbl_1_bss_58C[2], 1);
        Hu3DModelAttrReset(lbl_1_bss_58C[6], 1);
        fn_1_7320();
        fn_1_16024();
    }
}

/* Loads the room models and creates the message windows. */
static void fn_1_18FC(void)
{
    s16 mdl;

    if (omovlevtno == 0) {
        CharDataClose(-1);
        HuAR_DVDtoARAM(0xBE0000);
        HuAR_DVDtoARAM(0xBC0000);
        HuAMemDump();
    }
    mdl = Hu3DModelCreate(HuDataSelHeapReadNum(0xBE0000, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_58C[0] = mdl;
    Hu3DModelCameraSet(mdl, 4);
    mdl = Hu3DModelCreate(HuDataSelHeapReadNum(0xBE0001, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_58C[1] = mdl;
    Hu3DModelCameraSet(mdl, 4);
    Hu3DModelAttrSet(mdl, 0x40000001);
    Hu3DModelAttrSet(mdl, 0x40000040);
    Hu3DModelAttrSet(mdl, 0x4000);
    mdl = Hu3DModelCreate(HuDataSelHeapReadNum(0xBE0002, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_58C[2] = mdl;
    Hu3DModelRotSet(mdl, 0.0f, -60.0f, 0.0f);
    Hu3DModelCameraSet(mdl, 4);
    Hu3DModelAttrSet(mdl, 0x40000002);
    Hu3DAnimCreate(HuDataSelHeapReadNum(0xBE0018, HU_MEMNUM_OVL, HEAP_MODEL), mdl, "door_a1");
    Hu3DAnimCreate(HuDataSelHeapReadNum(0xBE0016, HU_MEMNUM_OVL, HEAP_MODEL), mdl, "garage_a1");
    Hu3DModelAttrSet(mdl, 0x4000);
    mdl = Hu3DModelCreate(HuDataSelHeapReadNum(0xBE0002, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_58C[3] = mdl;
    Hu3DModelRotSet(mdl, 0.0f, 60.0f, 0.0f);
    Hu3DModelCameraSet(mdl, 4);
    Hu3DModelAttrSet(mdl, 0x40000002);
    Hu3DModelAttrSet(mdl, 0x4000);
    mdl = Hu3DModelCreate(HuDataSelHeapReadNum(0xBE0002, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_58C[4] = mdl;
    Hu3DModelRotSet(mdl, 0.0f, 120.0f, 0.0f);
    Hu3DModelCameraSet(mdl, 4);
    Hu3DModelAttrSet(mdl, 0x40000002);
    Hu3DAnimCreate(HuDataSelHeapReadNum(0xBE0017, HU_MEMNUM_OVL, HEAP_MODEL), mdl, "door_a1");
    Hu3DAnimCreate(HuDataSelHeapReadNum(0xBE0015, HU_MEMNUM_OVL, HEAP_MODEL), mdl, "garage_a1");
    Hu3DModelAttrSet(mdl, 0x4000);
    mdl = Hu3DModelCreate(HuDataSelHeapReadNum(0xBE0003, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_58C[5] = mdl;
    Hu3DModelCameraSet(mdl, 4);
    Hu3DMotionTimeSet(mdl, 60.0f);
    Hu3DModelAttrSet(mdl, 0x40000002);
    Hu3DModelAttrSet(mdl, 0x4000);
    mdl = Hu3DModelCreate(HuDataSelHeapReadNum(0xBE0004, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_58C[6] = mdl;
    Hu3DModelCameraSet(mdl, 4);
    Hu3DModelAttrSet(mdl, 0x40000002);
    Hu3DModelAttrSet(mdl, 0x4000);
    mdl = Hu3DModelCreate(HuDataSelHeapReadNum(0xBE0011, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_58A = mdl;
    Hu3DModelPosSet(mdl, 0.0f, 195.0f, -1155.0f);
    Hu3DModelScaleSet(mdl, 1.8f, 1.8f, 1.8f);
    Hu3DModelCameraSet(mdl, 4);
    lbl_1_bss_584[0] = Hu3DJointMotion(mdl, HuDataSelHeapReadNum(0xBE0012, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_584[1] = Hu3DJointMotion(mdl, HuDataSelHeapReadNum(0xBE0013, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_584[2] = Hu3DJointMotion(mdl, HuDataSelHeapReadNum(0xBE0014, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DMotionSet(mdl, lbl_1_bss_584[0]);
    Hu3DModelAttrSet(mdl, 0x40000001);
    HuDataDirClose(0xBE0000);
    lbl_1_bss_570 = HuWinExCreateFrame(16.0f, 360.0f, 0x220, 0x44, -1, 1);
    HuWinMesPalSet(lbl_1_bss_570, 7, 0, 0, 0);
    HuWinMesSpeedSet(lbl_1_bss_570, 0);
    HuWinPriSet(lbl_1_bss_570, 0x32);
    winData[lbl_1_bss_570].padMask = 7;
    lbl_1_bss_56E = HuWinCreate(-10000.0f, 322.0f, 0x230, 0x2A, 0);
    HuWinBGTPLvlSet(lbl_1_bss_56E, 0.0f);
    HuWinAttrSet(lbl_1_bss_56E, 0x800);
    HuWinMesSpeedSet(lbl_1_bss_56E, 0);
    HuWinDispOff(lbl_1_bss_56E);
    HuWinPriSet(lbl_1_bss_56E, 0x32);
    HuSprExecLayerCameraSet(0x40, 4, 6);
    fn_1_556C();
    fn_1_13B1C();
    NameEnterInit();
    fn_1_19324();
    fn_1_1C25C();
}

/* Free-fly debug camera for the two outer views (cameras 2 and 3), driven by pads 0 and 1. */
static void DebugCamMove(int cam, int pad, f32 minZoom)
{
    Vec pos;
    Vec dir;
    Vec rotDir;
    Vec up;
    s8 t;
    f32 rotX;
    f32 rotY;
    f32 rotZ;

    CRotM[cam].y += 0.1f * HuPadStkX[pad];
    CRotM[cam].x += 0.1f * HuPadStkY[pad];
    CZoomM[cam] += HuPadTrigL[pad] / 2;
    CZoomM[cam] -= HuPadTrigR[pad] / 2;
    if (CZoomM[cam] < minZoom) {
        CZoomM[cam] = minZoom;
    }
    rotX = CRotM[cam].x;
    rotY = CRotM[cam].y;
    rotZ = CRotM[cam].z;
    pos.x = CenterM[cam].x + (CZoomM[cam] * (HuSin(rotY) * HuCos(rotX)));
    pos.y = CenterM[cam].y + (CZoomM[cam] * -HuSin(rotX));
    pos.z = CenterM[cam].z + (CZoomM[cam] * (HuCos(rotY) * HuCos(rotX)));
    dir.x = CenterM[cam].x - pos.x;
    dir.y = CenterM[cam].y - pos.y;
    dir.z = CenterM[cam].z - pos.z;
    rotDir.x = HuSin(rotY) * HuSin(rotX);
    rotDir.y = HuCos(rotX);
    rotDir.z = HuCos(rotY) * HuSin(rotX);
    up.x = (rotDir.x * ((dir.x * dir.x) + ((1.0f - (dir.x * dir.x)) * HuCos(rotZ))))
        + (rotDir.y * (((dir.x * dir.y) * (1.0 - HuCos(rotZ))) - (dir.z * HuSin(rotZ))))
        + (rotDir.z * (((dir.x * dir.z) * (1.0 - HuCos(rotZ))) + (dir.y * HuSin(rotZ))));
    up.y = (rotDir.y * ((dir.y * dir.y) + ((1.0f - (dir.y * dir.y)) * HuCos(rotZ))))
        + (rotDir.x * (((dir.x * dir.y) * (1.0 - HuCos(rotZ))) + (dir.z * HuSin(rotZ))))
        + (rotDir.z * (((dir.y * dir.z) * (1.0 - HuCos(rotZ))) - (dir.x * HuSin(rotZ))));
    up.z = (rotDir.z * ((dir.z * dir.z) + ((1.0f - (dir.z * dir.z)) * HuCos(rotZ))))
        + (rotDir.x * (((dir.x * dir.z) * (1.0 - HuCos(rotZ))) - (dir.y * HuSin(rotZ))))
        + (rotDir.y * (((dir.y * dir.z) * (1.0 - HuCos(rotZ))) + (dir.x * HuSin(rotZ))));
    PSVECCrossProduct(&rotDir, &dir, &dir);
    PSVECNormalize(&dir, &dir);
    t = HuPadSubStkX[pad] & 0xF8;
    if (t != 0) {
        CenterM[cam].x += 0.05f * (dir.x * t);
        CenterM[cam].y += 0.05f * (dir.y * t);
        CenterM[cam].z += 0.05f * (dir.z * t);
    }
    PSVECNormalize(&up, &dir);
    t = -(HuPadSubStkY[pad] & 0xF8);
    if (t != 0) {
        CenterM[cam].x += 0.05f * (dir.x * t);
        CenterM[cam].y += 0.05f * (dir.y * t);
        CenterM[cam].z += 0.05f * (dir.z * t);
    }
}

static void fn_1_1F40(OMOBJ *obj)
{
    if (lbl_1_bss_4 != 0) {
        DebugCamMove(2, 0, 50.0f);
        DebugCamMove(3, 1, 100.0f);
    }
}

/* Same camera update as omOutViewMulti, for the 6 cameras. */
static void fn_1_35E8(void)
{
    Vec pos;
    Vec target;
    Vec up;
    Vec rotDir;
    Vec dir;
    u8 i;
    f32 rotX;
    f32 rotY;
    f32 rotZ;

    while (1) {
        for (i = 0; i < 6; i++) {
            rotX = CRotM[i].x;
            rotY = CRotM[i].y;
            rotZ = CRotM[i].z;
            pos.x = CenterM[i].x + (CZoomM[i] * (HuSin(rotY) * HuCos(rotX)));
            pos.y = CenterM[i].y + (CZoomM[i] * -HuSin(rotX));
            pos.z = CenterM[i].z + (CZoomM[i] * (HuCos(rotY) * HuCos(rotX)));
            target.x = CenterM[i].x;
            target.y = CenterM[i].y;
            target.z = CenterM[i].z;
            rotDir.x = HuSin(rotY) * HuSin(rotX);
            rotDir.y = HuCos(rotX);
            rotDir.z = HuCos(rotY) * HuSin(rotX);
            PSVECSubtract(&pos, &target, &dir);
            PSVECNormalize(&dir, &dir);
            up.x = (rotDir.x * ((dir.x * dir.x) + ((1.0f - (dir.x * dir.x)) * HuCos(rotZ))))
                + (rotDir.y * (((dir.x * dir.y) * (1.0 - HuCos(rotZ))) - (dir.z * HuSin(rotZ))))
                + (rotDir.z * (((dir.x * dir.z) * (1.0 - HuCos(rotZ))) + (dir.y * HuSin(rotZ))));
            up.y = (rotDir.y * ((dir.y * dir.y) + ((1.0f - (dir.y * dir.y)) * HuCos(rotZ))))
                + (rotDir.x * (((dir.x * dir.y) * (1.0 - HuCos(rotZ))) + (dir.z * HuSin(rotZ))))
                + (rotDir.z * (((dir.y * dir.z) * (1.0 - HuCos(rotZ))) - (dir.x * HuSin(rotZ))));
            up.z = (rotDir.z * ((dir.z * dir.z) + ((1.0f - (dir.z * dir.z)) * HuCos(rotZ))))
                + (rotDir.x * (((dir.x * dir.z) * (1.0 - HuCos(rotZ))) - (dir.y * HuSin(rotZ))))
                + (rotDir.y * (((dir.y * dir.z) * (1.0 - HuCos(rotZ))) + (dir.x * HuSin(rotZ))));
            PSVECNormalize(&up, &up);
            Hu3DCameraPosSetV(1 << i, &pos, &up, &target);
        }
        HuPrcVSleep();
    }
}
