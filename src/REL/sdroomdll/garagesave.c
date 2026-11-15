#include "REL/sdroomdll/garagesave.h"
#include "game/card.h"
#include "game/saveload.h"
#include "game/window.h"
#include "game/sprite.h"
#include "game/memory.h"
#include "string.h"
#include "stdio.h"

/* camera views of the backup browser: centre, rotation, zoom */
static OMCAMERAVIEW lbl_1_data_1518 = { { 432.0f, 200.0f, -250.0f }, { 0.0f, 300.0f, 0.0f }, 50.0f };
static OMCAMERAVIEW lbl_1_data_1534 = { { 0.0f, 70.0f, 0.0f }, { -30.0f, 0.0f, 0.0f }, 5300.0f };
static OMCAMERAVIEW lbl_1_data_1550 = { { 0.0f, 80.0f, 0.0f }, { -30.0f, 0.0f, 0.0f }, 6500.0f };

s32 lbl_1_bss_750;
s16 lbl_1_bss_754[5];
s16 lbl_1_bss_75E;
static s16 lbl_1_bss_760;
static char lbl_1_bss_762[6];
static s16 lbl_1_bss_768;
static s16 lbl_1_bss_76A;
static s16 lbl_1_bss_76C;
SDMACHINE_DISP lbl_1_bss_770;
static GWSDCOMMON lbl_1_bss_79C;
static HU3DCAMERA lbl_1_bss_8E8;
static u8 lbl_1_bss_940[0x38];
static s16 lbl_1_bss_978[20];

/* from garage.c */
extern s16 lbl_1_bss_58A;
extern s16 lbl_1_bss_58C[0x10];
extern void fn_1_7298(void);
extern void fn_1_7320(void);

static void fn_1_147CC(void);
static void fn_1_14A60(void);
static s16 fn_1_14B04(void);
static void fn_1_15060(void);
static void fn_1_158E0(void);
static void fn_1_15BD0(GWSDMACHINE *machine, SDMACHINE_DISP *disp);
static void fn_1_15E00(SDMACHINE_DISP *disp);
static void fn_1_15F58(s16 no);
static s16 fn_1_1645C(s16 slot);
static s32 fn_1_165BC(s16 slot);
static s32 fn_1_1682C(u8 slot);
static s32 fn_1_16B58(u8 slot);
static s32 fn_1_171C4(u8 slot);
static s32 fn_1_17568(s16 slot);
static s32 fn_1_17800(s16 slot);
static s32 fn_1_17900(s16 slot);

#define ANG_M60 (-1.0471975511965976)
#define ANG_30 (0.5235987755982988)
#define PI_D 3.141592653589793
#define MESS_SD(n) (0x190000 | (n))

void fn_1_13B1C(void)
{
    f32 z;
    if (GwSdBackupNo == -1) {
        lbl_1_bss_770.headMdl = -1;
        Hu3DMotionTimeSet(lbl_1_bss_58C[2], 0.0f);
        Hu3DModelAttrSet(lbl_1_bss_58C[2], 0x40000002);
        return;
    }
    Hu3DMotionTimeSet(lbl_1_bss_58C[2], 61.0f);
    Hu3DModelAttrReset(lbl_1_bss_58C[2], 0x40000002);
    Hu3DModelAttrSet(lbl_1_bss_58C[2], 0x40000001);
    Hu3DMotionStartEndSet(lbl_1_bss_58C[2], 61.0f, 240.0f);
    {
        GWSDBACKUP *backup = &GwSdBackup[GwSdBackupNo];
        fn_1_44E4(&lbl_1_bss_770, backup->machine.partNo[0], backup->machine.partNo[1], backup->machine.partNo[3], 0x40);
    }
    fn_1_4638(&lbl_1_bss_770, 4);
    z = 1450.0 * -cos(ANG_M60);
    fn_1_4534(&lbl_1_bss_770, 1450.0 * -sin(ANG_M60), 0.0f, z);
    fn_1_4590(&lbl_1_bss_770, 0.0f, 0.0f, 0.0f);
}

// NONMATCHING: control flow reconstructed from m2c
void fn_1_13D08(void)
{
    HuVecF pos;
    s16 i;
    s16 choice;
    s16 res;
    s32 done;
    u32 mess;
    f32 scale;

    done = 0;
    fn_1_147CC();
    winData[lbl_1_bss_75E].padMask = 4;
    pos.x = 1000.0 * -sin(ANG_M60);
    pos.y = 90.0f;
    pos.z = 1000.0 * -cos(ANG_M60);
    pos.x = pos.x + 170.0 * -sin(ANG_30);
    pos.z = pos.z + 170.0 * -cos(ANG_30);
    Hu3DModelRotSet(lbl_1_bss_58A, 0.0f, -50.0f, 0.0f);
    Hu3DModelScaleSet(lbl_1_bss_58A, 1.1f, 1.1f, 1.1f);
    if (lbl_1_bss_770.headMdl == -1) {
        HuAudFXPlay(0x509);
        Hu3DMotionTimeSet(lbl_1_bss_58C[2], 0.0f);
        Hu3DModelAttrReset(lbl_1_bss_58C[2], 0x40000002);
        Hu3DModelAttrSet(lbl_1_bss_58C[2], 0x40000001);
        HuPrcSleep(60);
        Hu3DMotionStartEndSet(lbl_1_bss_58C[2], 61.0f, 240.0f);
    }
    omCameraViewMoveMulti(4, &lbl_1_data_1518, 60, 0);
    HuPrcSleep(60);
    for (i = 1; i <= 20; i++) {
        pos.y = 90.0 + 500.0 * (1.0 - sin(PI_D * (90.0 * ((f64)i / 20.0)) / 180.0));
        Hu3DModelPosSetV(lbl_1_bss_58A, &pos);
        HuPrcVSleep();
    }
    HuWinExOpen(lbl_1_bss_75E);
    HuWinMesSet(lbl_1_bss_75E, MESS_SD(0x0B));
    HuAudFXPlay(0x2EF);
    HuWinMesWait(lbl_1_bss_75E);
    if (lbl_1_bss_770.headMdl == -1) {
        HuWinMesSet(lbl_1_bss_75E, MESS_SD(0x19));
    } else {
        HuWinMesSet(lbl_1_bss_75E, MESS_SD(0x4F));
    }
    HuWinMesWait(lbl_1_bss_75E);
    choice = HuWinChoiceGet(lbl_1_bss_75E, -1);
    if (choice == 0) {
        /* load a machine from a memory card backup */
        while (1) {
            HuWinMesSet(lbl_1_bss_75E, MESS_SD(0x59));
            HuWinMesWait(lbl_1_bss_75E);
            if (HuWinChoiceGet(lbl_1_bss_75E, -1) != 0) {
                lbl_1_bss_750 = 0;
            } else {
                lbl_1_bss_750 = 1;
                HuWinMesSet(lbl_1_bss_75E, MESS_SD(0x5A));
                HuWinMesWait(lbl_1_bss_75E);
                while (1) {
                    if (fn_1_4FD0(4) & 0x100) {
                        HuAudFXPlay(1);
                        if ((s16)(HuCardCheck() & 2) == 2) {
                            HuWinInsertMesSet(lbl_1_bss_75E, MESS_SD(0x68), 0);
                            HuWinMesSet(lbl_1_bss_75E, MESS_SD(0x4E));
                            HuWinMesWait(lbl_1_bss_75E);
                            HuPrcSleep(60);
                            break;
                        }
                        HuWinMesSet(lbl_1_bss_75E, MESS_SD(0x7B));
                        HuWinMesWait(lbl_1_bss_75E);
                        goto retry;
                    } else if (fn_1_4FD0(4) & 0x200) {
                        HuAudFXPlay(3);
                        done = 1;
                        goto finish;
                    }
                    HuPrcVSleep();
                }
            }
            res = fn_1_14B04();
            if (res == -2) {
                HuWinMesSet(lbl_1_bss_75E, MESS_SD(0x7A));
                HuWinMesWait(lbl_1_bss_75E);
                continue;
            }
            if (res == -1) {
                break;
            }
            if (lbl_1_bss_76C == 0) {
                mess = MESS_SD(0x50);
                if (res == 3) {
                    HuWinInsertMesSet(lbl_1_bss_75E, MESS_SD(0x69), 0);
                } else if (res & 1) {
                    mess = MESS_SD(0x7D);
                } else if (res & 2) {
                    HuWinInsertMesSet(lbl_1_bss_75E, MESS_SD(0x68), 0);
                }
                HuWinMesSet(lbl_1_bss_75E, mess);
                HuWinMesWait(lbl_1_bss_75E);
                done = 1;
                break;
            }
            if (lbl_1_bss_76A == 0 && (res & 1)) {
                HuWinMesSet(lbl_1_bss_75E, MESS_SD(0x7D));
                HuWinMesWait(lbl_1_bss_75E);
            } else if (lbl_1_bss_768 == 0 && (res & 2)) {
                HuWinInsertMesSet(lbl_1_bss_75E, MESS_SD(0x68), 0);
                HuWinMesSet(lbl_1_bss_75E, MESS_SD(0x50));
                HuWinMesWait(lbl_1_bss_75E);
            }
            fn_1_15060();
            break;
        retry:
            continue;
        }
    }
finish:
    HuWinExClose(lbl_1_bss_75E);
    for (i = 1; i <= 20; i++) {
        pos.y = 90.0 + 500.0 * (1.0 - cos(PI_D * (90.0 * ((f64)i / 20.0)) / 180.0));
        Hu3DModelPosSetV(lbl_1_bss_58A, &pos);
        HuPrcVSleep();
    }
    Hu3DModelPosSet(lbl_1_bss_58A, 0.0f, 195.0f, -1155.0f);
    Hu3DModelRotSet(lbl_1_bss_58A, 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(lbl_1_bss_58A, 1.8f, 1.8f, 1.8f);
    if (lbl_1_bss_770.headMdl == -1 || done != 0) {
        if (done != 0 && lbl_1_bss_770.headMdl != -1) {
            HuAudFXPlay(0x50D);
            for (i = 1; i <= 30; i++) {
                scale = cos(PI_D * (90.0 * ((f64)i / 30.0)) / 180.0);
                Hu3DModelScaleSet(lbl_1_bss_770.headMdl, scale, 1.0f, scale);
                Hu3DModelRotSet(lbl_1_bss_770.headMdl, 0.0f, 600.0 * (1.0 - (f64)scale), 0.0f);
                HuPrcVSleep();
            }
            fn_1_4724(&lbl_1_bss_770);
            lbl_1_bss_770.headMdl = -1;
            GwSdBackupNo = -1;
        }
        HuAudFXPlay(0x509);
        Hu3DMotionTimeSet(lbl_1_bss_58C[2], 601.0f);
        Hu3DModelAttrReset(lbl_1_bss_58C[2], 0x40000001);
        Hu3DMotionStartEndSet(lbl_1_bss_58C[2], 0.0f, 640.0f);
    }
    fn_1_14A60();
}

static void fn_1_147CC(void)
{
    s16 grp;

    Hu3DModelAttrSet(lbl_1_bss_58C[1], 1);
    Hu3DModelAttrSet(lbl_1_bss_58C[5], 1);
    Hu3DModelAttrSet(lbl_1_bss_58C[3], 1);
    Hu3DModelAttrSet(lbl_1_bss_58C[4], 1);
    Hu3DModelAttrSet(lbl_1_bss_58C[6], 1);
    fn_1_7320();
    lbl_1_bss_75E = HuWinExCreateFrame(156.0f, 344.0f, 400, 94, -1, 1);
    HuWinMesPalSet(lbl_1_bss_75E, 7, 0, 0, 0);
    HuWinMesSpeedSet(lbl_1_bss_75E, 0);
    HuWinPriSet(lbl_1_bss_75E, 50);
    grp = HuSprGrpCreate(2);
    lbl_1_bss_978[0] = grp;
    HuSprGrpMemberSet(grp, 0, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(0xBE0032, 0x10000000, HEAP_MODEL)), 20, 0));
    HuSprPosSet(grp, 0, 288.0f, -1000.0f);
    HuSprGrpMemberSet(grp, 1, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(0xBE0033, 0x10000000, HEAP_MODEL)), 20, 0));
    HuSprPosSet(grp, 1, 288.0f, -1000.0f);
    Hu3DCameraPerspectiveSet(8, 5.0f, 20.0f, 15000.0f, 1.2f);
    Hu3DCameraViewportSet(8, -3.5555496f, 72.0f, 256.0f, 192.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(8, 0x16, 0x4C, 0xCC, 0xB8);
    omCameraViewSetMulti(8, &lbl_1_data_1534);
}

static void fn_1_14A60(void)
{
    Hu3DModelAttrReset(lbl_1_bss_58C[1], 1);
    Hu3DModelAttrReset(lbl_1_bss_58C[5], 1);
    Hu3DModelAttrReset(lbl_1_bss_58C[3], 1);
    Hu3DModelAttrReset(lbl_1_bss_58C[4], 1);
    Hu3DModelAttrReset(lbl_1_bss_58C[6], 1);
    fn_1_7298();
    HuWinKill(lbl_1_bss_75E);
    HuSprGrpKill(lbl_1_bss_978[0]);
}

/* collects the machines of the other save boxes / the second card into GwSdBackup.
   returns -2 (same card as slot 0), or a bitmask: 1 = slot 0 scanned, 2 = slot 1 scanned */
static s16 fn_1_14B04(void)
{
    u64 serial;
    s16 ret;
    s16 box;
    s16 garage;

    lbl_1_bss_768 = 0;
    lbl_1_bss_76A = 0;
    lbl_1_bss_76C = 0;
    if (!SLSaveFlagGet()) {
        return 0;
    }
    for (box = 0; box < 3; box++) {
        if (box != SLCurBoxNoGet() && strncmp((char *)saveBuf[0] + SLBoxDataOffsetGet(box), "SAVE", 4) == 0) {
            SLSdLoad(0, box, &lbl_1_bss_79C);
            for (garage = 0; garage < 2; garage++) {
                if (lbl_1_bss_79C.machine[garage].charNo != -1) {
                    GwSdBackup[lbl_1_bss_76C].machine = lbl_1_bss_79C.machine[garage];
                    GwSdBackup[lbl_1_bss_76C].slotNo = 0;
                    GwSdBackup[lbl_1_bss_76C].boxNo = box;
                    lbl_1_bss_76A++;
                    lbl_1_bss_76C++;
                }
            }
        }
    }
    ret = 1;
    lbl_1_bss_760 = 1;
    if (lbl_1_bss_750 != 0 && fn_1_165BC(1) == 0) {
        CARDGetSerialNo(1, &serial);
        if (SLSerialNo[0] == serial) {
            return -2;
        }
        if (fn_1_16B58(1) != 0) {
            for (box = 0; box < 3; box++) {
                if (strncmp((char *)saveBuf[1] + SLBoxDataOffsetGet(box), "SAVE", 4) == 0) {
                    SLSdLoad(1, box, &lbl_1_bss_79C);
                    for (garage = 0; garage < 2; garage++) {
                        if (lbl_1_bss_79C.machine[garage].charNo != -1) {
                            GwSdBackup[lbl_1_bss_76C].machine = lbl_1_bss_79C.machine[garage];
                            GwSdBackup[lbl_1_bss_76C].slotNo = 1;
                            GwSdBackup[lbl_1_bss_76C].boxNo = box;
                            GwSdBackup[lbl_1_bss_76C].garageNo = garage;
                            lbl_1_bss_768++;
                            lbl_1_bss_76C++;
                        }
                    }
                }
            }
        }
        ret = 1 | 2;
    } else {
        HuCardUnMount(1);
    }
    OSReport("==>%d\n", lbl_1_bss_76C);
    return ret;
}

/* machine picker: left/right browse GwSdBackup, A selects, B cancels */
static void fn_1_15060(void)
{
    SDMACHINE_DISP disp;
    GWSDBACKUP *backup;
    f32 t;
    f32 z;
    s16 dir;
    s16 sel;
    s16 i;

    fn_1_4D0C(0xD);
    fn_1_480C(&disp);
    sel = 0;
    fn_1_158E0();
    fn_1_15BD0(&GwSdBackup[0].machine, &disp);
    fn_1_15F58(1);
    HuWinMesSet(lbl_1_bss_75E, MESS_SD(0x5D));
    HuWinMesWait(lbl_1_bss_75E);
    HuSprPosSet(lbl_1_bss_978[0], 0, 32.0f, 200.0f);
    HuSprPosSet(lbl_1_bss_978[0], 1, 544.0f, 200.0f);
    HuSprTPLvlSet(lbl_1_bss_978[0], 0, 0.0f);
    HuSprTPLvlSet(lbl_1_bss_978[0], 1, 0.0f);
    for (i = 1; i <= 10; i++) {
        t = (f64)i / 10.0;
        if (sel != 0) {
            HuSprTPLvlSet(lbl_1_bss_978[0], 0, t);
        }
        if (lbl_1_bss_76C - 1 > 0) {
            HuSprTPLvlSet(lbl_1_bss_978[0], 1, t);
        }
        HuPrcVSleep();
    }
    while (1) {
        if (fn_1_4FD0(4) & 0x100) {
            HuAudFXPlay(1);
            GwSdBackupNo = sel;
            if (lbl_1_bss_770.headMdl != -1) {
                HuAudFXPlay(0x50D);
                for (i = 1; i <= 30; i++) {
                    t = cos(PI_D * (90.0 * ((f64)i / 30.0)) / 180.0);
                    Hu3DModelScaleSet(lbl_1_bss_770.headMdl, t, 1.0f, t);
                    Hu3DModelRotSet(lbl_1_bss_770.headMdl, 0.0f, 600.0 * (1.0 - (f64)t), 0.0f);
                    HuPrcVSleep();
                }
                fn_1_4724(&lbl_1_bss_770);
            }
            backup = &GwSdBackup[sel];
            fn_1_44E4(&lbl_1_bss_770, backup->machine.partNo[0], backup->machine.partNo[1], backup->machine.partNo[3], 0x40);
            fn_1_4638(&lbl_1_bss_770, 4);
            z = 1450.0 * -cos(ANG_M60);
            fn_1_4534(&lbl_1_bss_770, 1450.0 * -sin(ANG_M60), 0.0f, z);
            fn_1_4590(&lbl_1_bss_770, 0.0f, 0.0f, 0.0f);
            HuAudFXPlay(0x50A);
            for (i = 1; i <= 30; i++) {
                t = sin(PI_D * (90.0 * ((f64)i / 30.0)) / 180.0);
                Hu3DModelScaleSet(lbl_1_bss_770.headMdl, t, 1.0f, t);
                Hu3DModelRotSet(lbl_1_bss_770.headMdl, 0.0f, 600.0 * (1.0 - (f64)t), 0.0f);
                HuPrcVSleep();
            }
            break;
        } else if (fn_1_4FD0(4) & 0x200) {
            HuAudFXPlay(3);
            break;
        } else {
            dir = 0;
            if ((fn_1_507C(4) & 1) && sel != 0) {
                dir = -1;
            } else if ((fn_1_507C(4) & 2) && sel < lbl_1_bss_76C - 1) {
                dir = 1;
            }
            if (dir != 0) {
                if (sel + dir == 0 || sel == 0 || lbl_1_bss_76C - 1 == sel + dir || sel == lbl_1_bss_76C - 1) {
                    HuAudFXPlay(0);
                    for (i = 1; i <= 10; i++) {
                        t = (f64)i / 10.0;
                        if (sel + dir == 0) {
                            HuSprTPLvlSet(lbl_1_bss_978[0], 0, 1.0 - (f64)t);
                        }
                        if (sel == 0) {
                            HuSprTPLvlSet(lbl_1_bss_978[0], 0, t);
                        }
                        if (lbl_1_bss_76C - 1 == sel + dir) {
                            HuSprTPLvlSet(lbl_1_bss_978[0], 1, 1.0 - (f64)t);
                        }
                        if (sel == lbl_1_bss_76C - 1) {
                            HuSprTPLvlSet(lbl_1_bss_978[0], 1, t);
                        }
                        HuPrcVSleep();
                    }
                }
                sel += dir;
                fn_1_15BD0(&GwSdBackup[sel].machine, &disp);
                fn_1_15F58(sel + 1);
            }
            HuPrcVSleep();
        }
    }
    for (i = 1; i <= 10; i++) {
        t = 1.0 - ((f64)i / 10.0);
        if (sel != 0) {
            HuSprTPLvlSet(lbl_1_bss_978[0], 0, t);
        }
        if (sel < lbl_1_bss_76C - 1) {
            HuSprTPLvlSet(lbl_1_bss_978[0], 1, t);
        }
        HuPrcVSleep();
    }
    fn_1_15E00(&disp);
    fn_1_4E14();
}

static void fn_1_158E0(void)
{
    s16 win;

    win = HuWinExCreateFrame(32.0f, 32.0f, 512, 276, -1, 0);
    lbl_1_bss_754[0] = win;
    HuWinMesSpeedSet(win, 0);
    HuWinPriSet(win, 50);
    HuWinDrawNoSet(win, 0x40);
    HuWinExOpen(win);
    win = HuWinCreate(-10000.0f, 32.0f, 544, 26, 0);
    lbl_1_bss_754[1] = win;
    HuWinMesSpeedSet(win, 0);
    HuWinPriSet(win, 40);
    HuWinAttrSet(win, 0x800);
    HuWinBGTPLvlSet(win, 0.0f);
    win = HuWinCreate(280.0f, 70.0f, 280, 172, 0);
    lbl_1_bss_754[2] = win;
    HuWinMesSpeedSet(win, 0);
    HuWinPriSet(win, 40);
    HuWinBGTPLvlSet(win, 0.0f);
    win = HuWinCreate(-10000.0f, 275.0f, 544, 26, 0);
    lbl_1_bss_754[3] = win;
    HuWinMesSpeedSet(win, 0);
    HuWinPriSet(win, 40);
    HuWinAttrSet(win, 0x800);
    HuWinBGTPLvlSet(win, 0.0f);
    HuWinMesSet(win, 0x20004);
    win = HuWinCreate(446.0f, 32.0f, 120, 26, 0);
    lbl_1_bss_754[4] = win;
    HuWinMesSpeedSet(win, 0);
    HuWinPriSet(win, 40);
    HuWinBGTPLvlSet(win, 0.0f);
    lbl_1_bss_8E8 = Hu3DCamera[3];
    omCameraViewSetMulti(8, &lbl_1_data_1550);
    Hu3DCameraViewportSet(8, -24.444437f, 34.0f, 400.0f, 300.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(8, 0x25, 0x22, 0x137, 0x11A);
}

static void fn_1_15BD0(GWSDMACHINE *machine, SDMACHINE_DISP *disp)
{
    f32 t;
    s16 win;
    s16 i;

    HuWinMesSet(lbl_1_bss_754[1], MESSNUM_PTR(machine->name));
    if (disp->partNo[0] != -1) {
        for (i = 1; i <= 10; i++) {
            t = 1.0 - ((f64)i / 10.0);
            fn_1_45E4(disp, t, t, t);
            HuPrcVSleep();
        }
        fn_1_4724(disp);
    }
    win = lbl_1_bss_754[2];
    HuWinInsertMesSet(win, machine->partNo[0] + 0x1A0017, 0);
    HuWinInsertMesSet(win, machine->partNo[1] + 0x1A0001, 1);
    HuWinInsertMesSet(win, machine->partNo[2] + 0x1A000C, 2);
    HuWinInsertMesSet(win, machine->partNo[3] + 0x1A0022, 3);
    HuWinMesSet(win, 0x450008);
    fn_1_44E4(disp, machine->partNo[0], machine->partNo[1], machine->partNo[3], 0x40);
    fn_1_4638(disp, 8);
    fn_1_4590(disp, 0.0f, 45.0f, 0.0f);
    for (i = 1; i <= 10; i++) {
        t = (f64)i / 10.0;
        fn_1_45E4(disp, t, t, t);
        HuPrcVSleep();
    }
}

static void fn_1_15E00(SDMACHINE_DISP *disp)
{
    f32 t;
    s16 i;

    for (i = 1; i <= 10; i++) {
        t = 1.0 - ((f64)i / 10.0);
        fn_1_45E4(disp, t, t, t);
        HuPrcVSleep();
    }
    fn_1_4724(disp);
    HuWinKill(lbl_1_bss_754[1]);
    HuWinKill(lbl_1_bss_754[2]);
    HuWinKill(lbl_1_bss_754[3]);
    HuWinKill(lbl_1_bss_754[4]);
    Hu3DCamera[3] = lbl_1_bss_8E8;
    HuWinExClose(lbl_1_bss_754[0]);
    HuWinKill(lbl_1_bss_754[0]);
}

static void fn_1_15F58(s16 no)
{
    s16 win = lbl_1_bss_754[4];

    sprintf(lbl_1_bss_762, "%02d", no);
    sprintf(&lbl_1_bss_762[3], "%02d", lbl_1_bss_76C);
    HuWinInsertMesSet(win, MESSNUM_PTR(lbl_1_bss_762), 0);
    HuWinInsertMesSet(win, MESSNUM_PTR(&lbl_1_bss_762[3]), 1);
    HuWinMesSet(win, 0x19002C);
    HuWinScaleSet(win, 0.8f, 0.8f);
}

void fn_1_16024(void)
{
    if (lbl_1_bss_770.headMdl != -1) {
        fn_1_46C4(&lbl_1_bss_770, 1);
    }
}

void fn_1_16064(void)
{
    if (lbl_1_bss_770.headMdl != -1) {
        fn_1_46C4(&lbl_1_bss_770, 0);
    }
}

/* memory card error message; returns the yes/no choice for the questions (-1 otherwise) */
s16 fn_1_160A4(s16 errNo)
{
    s16 win;
    s16 choice = -1;
    u32 insert = 0;
    s32 ask = 0;
    u32 mess = 0;

    win = lbl_1_bss_75E;
    if ((u32)errNo <= 12) {
        switch (errNo) {
            case 0:
                mess = MESS_SD(0x7B);
                break;
            case 1:
                mess = MESS_SD(0x52);
                HuWinInsertMesSizeGet(lbl_1_bss_760 + 0x190067, 0);
                insert = lbl_1_bss_760 + 0x190067;
                break;
            case 2:
                HuWinInsertMesSizeGet(lbl_1_bss_760 + 0x190067, 0);
                insert = lbl_1_bss_760 + 0x190067;
                mess = 0x30005;
                break;
            case 3:
                mess = 0x30005;
                HuWinInsertMesSizeGet(lbl_1_bss_760 + 0x190067, 0);
                insert = lbl_1_bss_760 + 0x190067;
                break;
            case 4:
                mess = 0x30035;
                HuWinInsertMesSizeGet(lbl_1_bss_760 + 0x190067, 0);
                insert = lbl_1_bss_760 + 0x190067;
                break;
            case 5:
                HuWinInsertMesSizeGet(lbl_1_bss_760 + 0x190067, 0);
                insert = lbl_1_bss_760 + 0x190067;
                mess = MESS_SD(0x53);
                ask = 1;
                break;
            case 6:
                mess = MESS_SD(0x54);
                break;
            case 7:
                HuWinInsertMesSizeGet(lbl_1_bss_760 + 0x190067, 0);
                insert = lbl_1_bss_760 + 0x190067;
                mess = 0x30003;
                break;
            case 8:
                HuWinInsertMesSizeGet(lbl_1_bss_760 + 0x190067, 0);
                insert = lbl_1_bss_760 + 0x190067;
                mess = 0x30021;
                break;
            case 9:
                HuWinInsertMesSizeGet(lbl_1_bss_760 + 0x190067, 0);
                insert = lbl_1_bss_760 + 0x190067;
                mess = 0x30022;
                break;
            case 10:
                mess = 0x30018;
                ask = 1;
                break;
            case 11:
                HuWinInsertMesSizeGet(lbl_1_bss_760 + 0x190067, 0);
                insert = lbl_1_bss_760 + 0x190067;
                mess = 0x3001F;
                break;
            case 12:
                mess = 0x30019;
                HuWinInsertMesSizeGet(lbl_1_bss_760 + 0x190067, 0);
                insert = lbl_1_bss_760 + 0x190067;
                break;
        }
    }
    if (insert != 0) {
        HuWinInsertMesSet(win, insert, 0);
    }
    HuWinAttrSet(win, 0x10);
    HuWinMesSet(win, mess);
    HuWinMesWait(win);
    if (ask != 0) {
        if (errNo == 5) {
            HuWinInsertMesSet(win, lbl_1_bss_760 + 0x190067, 0);
            HuWinMesSet(win, MESS_SD(0x55));
            HuWinMesWait(win);
        }
        choice = HuWinChoiceGet(win, -1);
    }
    if (errNo == 11) {
        while (!(fn_1_4FD0(4) & 0x100)) {
            HuPrcVSleep();
        }
        HuAudFXPlay(1);
    }
    return choice;
}

/* format a card after the user agreed; returns the HuCardFormat result (0 when the card was removed) */
static s16 fn_1_1645C(s16 slot)
{
    s16 ret;

    HuWinInsertMesSet(lbl_1_bss_75E, slot + 0x190067, 0);
    HuWinMesSet(lbl_1_bss_75E, MESS_SD(0x76));
    HuPrcSleep(30);
    if (UnMountCnt & (1 << slot)) {
        fn_1_160A4(12);
        UnMountCnt = 0;
        return 0;
    }
    ret = HuCardFormat(slot);
    SLSerialNo[slot] = 0;
    switch (ret) {
        case -128:
            fn_1_160A4(6);
            fn_1_160A4(1);
            return -128;
        case -3:
            fn_1_160A4(0);
            return -3;
        case -2:
            fn_1_160A4(7);
            return ret;
        default:
            CARDGetSerialNo(slot, &SLSerialNo[slot]);
            return ret;
    }
}

static s32 fn_1_165BC(s16 slot)
{
    s32 ret;
    s32 size;

    ret = HuCardMount(slot);
    switch (ret) {
        case -2:
            fn_1_160A4(7);
            return ret;
        case -128:
            fn_1_160A4(1);
            return -128;
        case -3:
            fn_1_160A4(0);
            return -3;
        case -6:
            size = HuCardSectorSizeGet(slot);
            if (size > 0 && size != 0x2000) {
                fn_1_160A4(8);
                return -2;
            }
            UnMountCnt = 0;
            if (fn_1_160A4(5) == 0) {
                return fn_1_1645C(slot);
            }
            return -128;
        default:
            size = HuCardSectorSizeGet(slot);
            if (size < 0) {
                fn_1_160A4(1);
                return size;
            }
            if (size != 0x2000) {
                fn_1_160A4(8);
                return -2;
            }
            return 0;
    }
}

static s32 fn_1_1682C(u8 slot)
{
    s32 ret;
    s32 size;

    ret = fn_1_165BC(slot);
    if (ret < 0) {
        return ret;
    }
    ret = HuCardOpen(slot, SLSaveFileName, &curFileInfo);
    switch (ret) {
        case -4:
            return -4;
        case -2:
            fn_1_160A4(7);
            return -128;
        case -128:
            fn_1_160A4(1);
            return -128;
        case -3:
            fn_1_160A4(0);
            return -3;
        case -6:
            size = HuCardSectorSizeGet(slot);
            if (size > 0 && size != 0x2000) {
                fn_1_160A4(8);
                return -2;
            }
            UnMountCnt = 0;
            if (fn_1_160A4(5) == 0) {
                return fn_1_1645C(slot);
            }
            return -4;
        default:
            return 0;
    }
}

s32 fn_1_16AB0(s32 length, void *addr)
{
    s32 result = HuCardRead(&curFileInfo, addr, length, 0);
    if (result == -3) {
        fn_1_160A4(0);
    } else if (result < 0) {
        fn_1_160A4(1);
    }
    return result;
}

s32 fn_1_16B20(void)
{
    s32 result = HuCardClose(&curFileInfo);
    return result;
}

/* reads and verifies the save file of a card; offers repair/rewrite when boxes are damaged */
static s32 fn_1_16B58(u8 slot)
{
    OSTime time;
    char buf[8];
    s32 stat[3];
    s32 ret;
    s32 iconBroken;
    s16 win;
    s16 prevSlot;
    s16 i;

    if (fn_1_1682C(slot) < 0) {
        goto fail;
    }
    CARDGetSerialNo(slot, &SLSerialNo[slot]);
    ret = HuCardRead(&curFileInfo, saveBuf[slot], 0xA000, 0);
    if (ret == -3) {
        fn_1_160A4(0);
    } else if (ret < 0) {
        fn_1_160A4(1);
    }
    SLFileClose();
    if (ret < 0) {
        goto fail;
    }
    win = lbl_1_bss_75E;
    if (strncmp((char *)saveBuf[curSlotNo], "ERASE", 5) == 0) {
        time = OSGetTime();
        SLSaveDataMake(1, &time);
        for (i = 0; i < 3; i++) {
            SLSaveEmptyMark(slot, i);
        }
        goto ok;
    }
    iconBroken = 0;
    for (i = 0; i < 3; i++) {
        stat[i] = 0;
    }
    /* the checksum is stored big-endian in the buffer */
    if (((saveBuf[slot][SAVE_ICONBANNER_CHECKSUM_OFS] << 8) | saveBuf[slot][SAVE_ICONBANNER_CHECKSUM_OFS + 1])
        != SLCheckSumSlotGet(slot, 0, SAVE_ICONBANNER_SIZE)) {
        OSReport("IconBanner Area Broken!\n");
        time = OSGetTime();
        SLSaveDataSlotMake(slot, 0, &time);
        iconBroken = 1;
    }
    for (i = 0; i < 3; i++) {
        if (strncmp((char *)saveBuf[slot] + SLBoxDataOffsetGet(i), "SAVE", 4) == 0
            || strncmp((char *)saveBuf[slot] + SLBoxDataOffsetGet(i + 3), "SAVE", 4) == 0) {
            if (SLCheckSumBoxSlotCheck(slot, i) == 0) {
                OSReport("Box%d Broken!\n", i);
                SLBoxBackupSlotLoad(slot, i);
                if (SLCheckSumBoxSlotCheck(slot, i) == 0) {
                    SLSaveEmptyMark(slot, i);
                    stat[i] = 2;
                } else {
                    stat[i] = 1;
                }
            }
        } else if (strncmp((char *)saveBuf[slot] + SLBoxDataOffsetGet(i), "EMPT", 4) != 0
                   && strncmp((char *)saveBuf[slot] + SLBoxDataOffsetGet(i + 3), "EMPT", 4) != 0) {
            stat[i] = 3;
            SLSaveEmptyMark(slot, i);
        } else if (strncmp((char *)saveBuf[slot] + SLBoxDataOffsetGet(i), "EMPT", 4) == 0
                   || strncmp((char *)saveBuf[slot] + SLBoxDataOffsetGet(i + 3), "EMPT", 4) == 0) {
            SLSaveEmptyMark(slot, i);
        }
    }
    if (iconBroken != 0 || stat[0] != 0 || stat[1] != 0 || stat[2] != 0) {
        HuWinExOpen(win);
        if (stat[0] != 2 && stat[0] != 3 && stat[1] != 2 && stat[1] != 3 && stat[2] != 2 && stat[2] != 3) {
            HuWinInsertMesSet(win, slot + 0x190067, 0);
            HuWinMesSet(win, 0x30034);
            HuWinMesWait(win);
        } else {
            for (i = 0; i < 3; i++) {
                sprintf(buf, "%d", i + 1);
                if (stat[i] == 1) {
                    HuWinInsertMesSet(win, MESSNUM_PTR(buf), 0);
                    HuWinInsertMesSet(win, slot + 0x190067, 1);
                    HuWinMesSet(win, MESS_SD(0x58));
                    HuWinMesWait(win);
                } else if (stat[i] == 2) {
                    HuWinInsertMesSet(win, MESSNUM_PTR(buf), 0);
                    HuWinInsertMesSet(win, slot + 0x190067, 1);
                    HuWinMesSet(win, MESS_SD(0x57));
                    HuWinMesWait(win);
                } else if (stat[i] == 3) {
                    HuWinInsertMesSet(win, slot + 0x190067, 0);
                    HuWinInsertMesSet(win, MESSNUM_PTR(buf), 1);
                    HuWinMesSet(win, MESS_SD(0x79));
                    HuWinMesWait(win);
                }
            }
        }
        prevSlot = curSlotNo;
        curSlotNo = slot;
        ret = fn_1_171C4(slot);
        curSlotNo = prevSlot;
        if (ret == 0) {
            goto fail;
        }
    }
ok:
    HuCardUnMount(slot);
    return 1;
fail:
    HuCardUnMount(slot);
    return 0;
}

/* writes the (repaired) save buffer back to the card, creating the file when it does not exist.
   returns 1 on success */
static s32 fn_1_171C4(u8 slot)
{
    s16 win;
    s32 ret;
    s32 res;
    s32 size;

    SLCheckSumBoxAllSet();
    SLSaveBackup();
    while (1) {
        res = fn_1_1682C(slot);
        if (res == -4) {
            if (fn_1_17900(slot) == 0) {
                fn_1_160A4(9);
                goto error;
            }
            ret = fn_1_17568(slot);
            if (ret >= 0) {
                CARDGetSerialNo(slot, &SLSerialNo[slot]);
                goto close;
            }
            goto error;
        }
        if (res != -3 && res >= 0) {
            if (fn_1_17900(slot) == 0) {
                fn_1_160A4(9);
                goto error;
            }
            ret = fn_1_17800(slot);
            if (ret == -3) {
                fn_1_160A4(0);
            } else if (ret == -2) {
                fn_1_160A4(7);
            } else if (ret == -6) {
                size = HuCardSectorSizeGet(slot);
                if (size > 0 && size != 0x2000) {
                    fn_1_160A4(8);
                    goto error;
                }
                UnMountCnt = 0;
                if (fn_1_160A4(5) == 0) {
                    if (fn_1_1645C(slot) != 0) {
                        return 0;
                    }
                    continue;
                }
            } else if (ret < 0) {
                fn_1_160A4(1);
            }
        close:
            SLFileClose();
            if (ret >= 0) {
                HuCardUnMount(slot);
                return 1;
            }
        }
    error:
        win = lbl_1_bss_75E;
        HuWinAttrSet(win, 0x10);
        HuWinInsertMesSet(win, slot + 0x190067, 0);
        HuWinMesSet(win, MESS_SD(0x77));
        HuWinMesWait(win);
        if (HuWinChoiceGet(win, -1) == 0) {
            HuCardUnMount(slot);
            return 0;
        }
        HuWinInsertMesSet(win, slot + 0x190067, 0);
        HuWinMesSet(win, MESS_SD(0x5A));
        HuWinMesWait(win);
        while (!(HuPadBtnDown[2] & 0x100)) {
            HuPrcVSleep();
        }
        HuAudFXPlay(1);
    }
}

/* creates the save file (header "ERASE" + current save buffer) */
static s32 fn_1_17568(s16 slot)
{
    u32 freeBytes;
    u32 freeFiles;
    s32 ret;
    s32 size;
    void *buf;

    if (!SLSaveFlagGet()) {
        return 0;
    }
    ret = fn_1_165BC(slot);
    if (ret < 0) {
        return ret;
    }
    size = HuCardSectorSizeGet(slot);
    if (size < 0 && size != 0x2000) {
        fn_1_160A4(8);
        return -128;
    }
    HuCardFreeSpaceGet(slot, &freeBytes, &freeFiles);
    if (freeFiles == 0 && freeBytes < 0xA000) {
        fn_1_160A4(4);
        return -9;
    }
    if (freeFiles == 0) {
        fn_1_160A4(2);
        return -9;
    }
    if (freeBytes < 0xA000) {
        fn_1_160A4(3);
        return -9;
    }
    HuWinInsertMesSet(lbl_1_bss_75E, slot + 0x190067, 0);
    HuWinMesSet(lbl_1_bss_75E, MESS_SD(0x78));
    HuWinMesWait(lbl_1_bss_75E);
    ret = HuCardCreate(slot, SLSaveFileName, 0xA000, &curFileInfo);
    if (ret == -3) {
        fn_1_160A4(0);
        return -3;
    }
    if (ret < 0) {
        fn_1_160A4(1);
        return -128;
    }
    buf = HuMemDirectMalloc(HEAP_HEAP, 0xA000);
#ifdef TARGET_PC
    memset(buf, 0, 0xA000);
#else
    // the original passes (buf, 0xA000, 0): the buffer is never cleared
    memset(buf, 0xA000, 0);
#endif
    memcpy(buf, SLEraseStr, 6);
    ret = HuCardWrite(&curFileInfo, buf, 0xA000, 0);
    if (ret == 0) {
        ret = HuCardWrite(&curFileInfo, saveBuf[slot], 0xA000, 0);
    }
    HuMemDirectFree(buf);
    if (ret == -3) {
        fn_1_160A4(0);
        return -3;
    }
    if (ret < 0) {
        fn_1_160A4(1);
        return -128;
    }
    ret = SLStatSet(0);
    if (ret < 0) {
        return ret;
    }
    return 0;
}

/* rewrites the save data of an existing file */
static s32 fn_1_17800(s16 slot)
{
    s32 ret;

    if (!SLSaveFlagGet()) {
        return 0;
    }
    HuWinInsertMesSet(lbl_1_bss_75E, slot + 0x190067, 0);
    HuWinMesSet(lbl_1_bss_75E, MESS_SD(0x78));
    HuWinMesWait(lbl_1_bss_75E);
    HuPrcSleep(60);
    CARDGetSerialNo(slot, &SLSerialNo[slot]);
    ret = HuCardWrite(&curFileInfo, saveBuf[slot], 0xA000, 0);
    if (ret == 0) {
        ret = SLStatSet(0);
    }
    return ret;
}

/* 1 when the card still has the serial number recorded in SLSerialNo (or none was recorded) */
static s32 fn_1_17900(s16 slot)
{
    u64 serial;

    if (SLSerialNo[slot] == 0) {
        return 1;
    }
    if (CARDGetSerialNo(slot, &serial) < 0) {
        return 1;
    }
    if (serial != SLSerialNo[slot]) {
        return 0;
    }
    return 1;
}
