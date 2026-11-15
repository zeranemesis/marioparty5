#include "REL/sdroomdll/desk.h"
#include "REL/sdroomdll/sdroom.h"
#include "game/window.h"
#include "game/sprite.h"
#include "game/armem.h"
#include "game/memory.h"
#include "game/frand.h"
#include "game/mgdata.h"
#include "humath.h"
#include <stdio.h>


static void fn_1_258C8(void);
static void fn_1_25B58(void);
static void fn_1_26174(void);
static void fn_1_264C0(void);
static void fn_1_266A4(void);
static void fn_1_26DAC(void);
static void fn_1_26F0C(void);
static void fn_1_27004(s16 no);
static void fn_1_2709C(s16 winId, s16 picNo);
static void fn_1_271EC(s16 cat, s16 top);
static void fn_1_27730(f32 x, f32 y);
static void fn_1_279F8(s16 cat, s16 no);
static void fn_1_27D80(s16 cat, s16 no);
static void fn_1_28104(void);

/* desk bss (0x1980..0x1A0C) */
static s16 lbl_1_bss_1980; /* selected part */
static HUPROCESS *lbl_1_bss_1984; /* model spin process */
static s16 lbl_1_bss_1988; /* sprite group */
static s16 lbl_1_bss_198C; /* list window y */
static s16 lbl_1_bss_198E; /* list window x */
static char lbl_1_bss_1990[10][5]; /* part cost strings */
static char lbl_1_bss_19C2[6]; /* "nn" "nn" owned/total */
static char lbl_1_bss_19C8[6]; /* point string */
static s16 lbl_1_bss_19CE[2]; /* part preview models */
static s16 lbl_1_bss_19D2; /* list scroll */
static s16 lbl_1_bss_19D4; /* category */
static s16 lbl_1_bss_19D6[10]; /* cost windows */
static s16 lbl_1_bss_19EA[10]; /* name windows */
static s16 lbl_1_bss_19FE[7]; /* title, points, help, preview, info, count, desc */

static OMCAMERAVIEW lbl_1_data_2188 = { { 0.0f, 70.0f, 0.0f }, { -30.0f, 0.0f, 0.0f }, 5300.0f };
static u32 lbl_1_data_21A4[4] = { 0x190022, 0x190024, 0x190023, 0x190025 };

/* Desk main menu: look at the shop / start a duel. */
// NONMATCHING: restructured from the decompiled flow; same behaviour, different codegen
void fn_1_256B4(void)
{
    s16 choice;

    HuWinExOpen(lbl_1_bss_570);
    while (1) {
        if (omovlevtno != 1) {
            fn_1_28104();
        }
        HuWinMesSet(lbl_1_bss_570, 0x190006);
        HuWinMesWait(lbl_1_bss_570);
        if (omovlevtno != 1) {
            choice = HuWinChoiceGet(lbl_1_bss_570, -1);
        } else {
            choice = HuWinChoiceGet(lbl_1_bss_570, 1);
        }
        omovlevtno = 0;
        if (choice == 2 || choice == -1) {
            break;
        }
        if (choice == 0) {
            HuWinExClose(lbl_1_bss_570);
            fn_1_25B58();
            HuWinExOpen(lbl_1_bss_570);
        } else if (choice == 1) {
            if (GwSdCommon.partFlag[0] == 0 || GwSdCommon.partFlag[1] == 0 || GwSdCommon.partFlag[2] == 0
                || GwSdCommon.partFlag[3] == 0) {
                HuWinMesSet(lbl_1_bss_570, 0x190075);
                HuWinMesWait(lbl_1_bss_570);
            } else {
                HuWinMesSet(lbl_1_bss_570, 0x190007);
                HuWinMesWait(lbl_1_bss_570);
                if (HuWinChoiceGet(lbl_1_bss_570, -1) == 0) {
                    HuPrcSleep(0x1E);
                    fn_1_258C8();
                    fn_1_4238(1);
                }
            }
        }
    }
    HuWinExClose(lbl_1_bss_570);
}

/* Sets up a duel against a random machine on a random stage. */
// NONMATCHING: restructured from the decompiled flow; same behaviour, different codegen
static void fn_1_258C8(void)
{
    s16 list[16];
    s16 cnt;
    s16 i;
    s16 j;
    s16 k;
    s16 other;

    GwSdBattle.gameType = 0;
    GwSdBattle.stageNo = 0;
    GwPlayerConf[0].type = 0;
    GwSdBattle.stageType1 = 0;
    GwSdBattle.machine[0].charNo = frandmod(7);
    if (HuPadErr[1] == 0) {
        GwPlayerConf[1].type = 0;
        GwSdBattle.stageType2 = 0;
        for (j = 0; j < 100; j++) {
            other = frandmod(7);
            if (other != GwSdBattle.machine[0].charNo) {
                break;
            }
        }
        if (j == 100) {
            other = GwSdBattle.machine[0].charNo + 1;
        }
        if (other >= 7) {
            other = 0;
        }
        GwSdBattle.machine[1].charNo = other;
    } else {
        GwPlayerConf[1].type = 1;
        GwSdBattle.stageType2 = 1;
        GwPlayerConf[1].dif = 0;
        GwSdBattle.machine[1].charNo = 9;
    }
    for (i = 0; i < 2; i++) {
        GwSdBattle.padNo[i] = i;
        GwPlayerConf[i].padNo = i;
        for (j = 0; j < 4; j++) {
            cnt = 0;
            for (k = 0; k < SdMachinePartNum[j]; k++) {
                if (GwSdCommon.partFlag[j] & (1 << k)) {
                    list[cnt] = k;
                    cnt++;
                }
            }
            GwSdBattle.machine[i].partNo[j] = list[frandmod(cnt)];
        }
    }
}

/* The parts shop: browse the four categories, buy with sdPoint. */
// NONMATCHING: restructured from the decompiled flow; same behaviour, different codegen
static void fn_1_25B58(void)
{
    s16 i;
    s16 row;
    s16 move;

    SdMachineFadeAlpha = 0.0f;
    Hu3DLayerHookSet(0xD, fn_1_484C);
    for (i = 1; i <= 10; i++) {
        SdMachineFadeAlpha = 0.5 * (i / 10.0);
        HuPrcVSleep();
    }
    lbl_1_bss_19D4 = 0;
    lbl_1_bss_1980 = 0;
    fn_1_266A4();
    fn_1_26174();
    row = 0;
    lbl_1_bss_19D2 = 0;
    while (1) {
        move = 0;
        if (HuPadDStkRep[0] & 8) {
            row--;
            move = -1;
            if (row < 0) {
                if (lbl_1_bss_19D2 != 0) {
                    lbl_1_bss_19D2--;
                } else {
                    move = 0;
                }
                row = 0;
            }
        } else if (HuPadDStkRep[0] & 4) {
            row++;
            move = 1;
            if (row >= 5) {
                if (lbl_1_bss_19D2 + 5 < SdMachinePartNum[lbl_1_bss_19D4]) {
                    lbl_1_bss_19D2++;
                } else {
                    move = 0;
                }
                row = 4;
            }
        } else if (HuPadDStkRep[0] & 1) {
            lbl_1_bss_19D4--;
            if (lbl_1_bss_19D4 < 0) {
                lbl_1_bss_19D4 = 3;
            }
            row = 0;
            lbl_1_bss_19D2 = 0;
            move = -10;
        } else if (HuPadDStkRep[0] & 2) {
            lbl_1_bss_19D4++;
            if (lbl_1_bss_19D4 >= 4) {
                lbl_1_bss_19D4 = 0;
            }
            row = 0;
            lbl_1_bss_19D2 = 0;
            move = 10;
        }
        if (move != 0) {
            HuAudFXPlay(0);
            lbl_1_bss_1980 = lbl_1_bss_19D2 + row;
            fn_1_271EC(lbl_1_bss_19D4, lbl_1_bss_19D2);
            fn_1_279F8(lbl_1_bss_19D4, lbl_1_bss_1980);
            fn_1_27004(lbl_1_bss_19D4);
            fn_1_2709C(lbl_1_bss_19FE[5], lbl_1_bss_19D4);
        }
        CZoomM[3] += HuPadTrigL[0] / 2;
        CZoomM[3] -= HuPadTrigR[0] / 2;
        if (CZoomM[3] < 3000.0f) {
            CZoomM[3] = 3000.0f;
        }
        if (CZoomM[3] > 7000.0f) {
            CZoomM[3] = 7000.0f;
        }
        if (HuPadBtnDown[0] & 0x100) {
            if (fn_1_414C(lbl_1_bss_19D4, lbl_1_bss_1980) == 0) {
                HuAudFXPlay(1);
                fn_1_27D80(lbl_1_bss_19D4, lbl_1_bss_1980);
            } else {
                HuAudFXPlay(4);
            }
        }
        if (HuPadBtnDown[0] & 0x200) {
            break;
        }
        HuPrcVSleep();
    }
    HuAudFXPlay(3);
    fn_1_264C0();
    fn_1_26DAC();
    for (i = 1; i <= 10; i++) {
        SdMachineFadeAlpha = 0.5 * (1.0 - (i / 10.0));
        HuPrcVSleep();
    }
    Hu3DLayerHookReset(0xD);
    Hu3DLayerHookReset(5);
    GwSdCommon.prevPoint = 0;
    for (i = 0; i < 4; i++) {
        GwSdCommon.prevPoint += GwCommon.sdPoint[i];
    }
}

/* Opens the shop windows and grows the part preview. */
static void fn_1_26174(void)
{
    f32 scale;
    s16 i;

    HuSprGrpPosSet(lbl_1_bss_1988, 0.0f, -1000.0f);
    HuWinDispOff(lbl_1_bss_19FE[0]);
    HuWinDispOff(lbl_1_bss_19FE[1]);
    HuWinDispOff(lbl_1_bss_19FE[2]);
    HuWinDispOff(lbl_1_bss_19FE[5]);
    for (i = 0; i < 10; i++) {
        HuWinDispOff(lbl_1_bss_19EA[i]);
        HuWinDispOff(lbl_1_bss_19D6[i]);
    }
    if (lbl_1_bss_19CE[0] != -1) {
        Hu3DModelScaleSet(lbl_1_bss_19CE[0], 0.0f, 0.0f, 0.0f);
    }
    if (lbl_1_bss_19CE[1] != -1) {
        Hu3DModelScaleSet(lbl_1_bss_19CE[1], 0.0f, 0.0f, 0.0f);
    }
    HuWinExOpen(lbl_1_bss_19FE[3]);
    HuWinExOpen(lbl_1_bss_19FE[4]);
    HuWinExOpen(lbl_1_bss_19FE[6]);
    HuWinDispOn(lbl_1_bss_19FE[0]);
    HuWinDispOn(lbl_1_bss_19FE[1]);
    HuWinDispOn(lbl_1_bss_19FE[2]);
    HuWinDispOn(lbl_1_bss_19FE[5]);
    HuSprGrpPosSet(lbl_1_bss_1988, 0.0f, 0.0f);
    fn_1_271EC(lbl_1_bss_19D4, 0);
    fn_1_27730(210.0f, 72.0f);
    for (i = 0; i < 10; i++) {
        HuWinDispOn(lbl_1_bss_19EA[i]);
        HuWinDispOn(lbl_1_bss_19D6[i]);
    }
    for (i = 1; i <= 10; i++) {
        scale = (f64)i / 10.0;
        if (lbl_1_bss_19CE[0] != -1) {
            Hu3DModelScaleSet(lbl_1_bss_19CE[0], scale, scale, scale);
        }
        if (lbl_1_bss_19CE[1] != -1) {
            Hu3DModelScaleSet(lbl_1_bss_19CE[1], scale, scale, scale);
        }
        HuPrcVSleep();
    }
}

static void fn_1_264C0(void)
{
    f32 scale;
    s16 i;

    for (i = 1; i <= 10; i++) {
        scale = 1.0 - ((f64)i / 10.0);
        if (lbl_1_bss_19CE[0] != -1) {
            Hu3DModelScaleSet(lbl_1_bss_19CE[0], scale, scale, scale);
        }
        if (lbl_1_bss_19CE[1] != -1) {
            Hu3DModelScaleSet(lbl_1_bss_19CE[1], scale, scale, scale);
        }
        HuPrcVSleep();
    }
    HuSprGrpPosSet(lbl_1_bss_1988, 0.0f, -1000.0f);
    for (i = 0; i < 10; i++) {
        HuWinDispOff(lbl_1_bss_19EA[i]);
        HuWinDispOff(lbl_1_bss_19D6[i]);
    }
    HuWinDispOff(lbl_1_bss_19FE[0]);
    HuWinDispOff(lbl_1_bss_19FE[1]);
    HuWinDispOff(lbl_1_bss_19FE[2]);
    HuWinDispOff(lbl_1_bss_19FE[5]);
    HuWinExClose(lbl_1_bss_19FE[3]);
    HuWinExClose(lbl_1_bss_19FE[4]);
    HuWinExClose(lbl_1_bss_19FE[6]);
}

/* Creates the shop windows, sprites, preview camera and the spin process. */
// NONMATCHING: restructured from the decompiled flow; same behaviour, different codegen
static void fn_1_266A4(void)
{
    s16 win;
    s16 i;
    ANIMDATA *anim;
    HUSPRGRPID grp;

    winPrio = 0x3E8;
    fn_1_3F00();
    win = HuWinCreate(48.0f, 32.0f, 0x140, 0x28, 0);
    lbl_1_bss_19FE[0] = win;
    HuWinBGTPLvlSet(win, 0.0f);
    HuWinMesSpeedSet(win, 0);
    HuWinMesSet(win, 0x190026);
    win = HuWinCreate(296.0f, 32.0f, 0x102, 0x28, 0);
    lbl_1_bss_19FE[1] = win;
    HuWinBGTPLvlSet(win, 0.0f);
    HuWinMesSpeedSet(win, 0);
    fn_1_27004(lbl_1_bss_19D4);
    win = HuWinCreate(16.0f, 354.0f, 0x140, 0x58, 0);
    lbl_1_bss_19FE[2] = win;
    HuWinBGTPLvlSet(win, 0.0f);
    HuWinMesSpeedSet(win, 0);
    HuWinMesSet(win, 0x20018);
    win = HuWinExCreate(16.0f, 72.0f, 0xC0, 0xC0, -1);
    lbl_1_bss_19FE[3] = win;
    HuWinDrawNoSet(win, 0x40);
    HuWinMesSpeedSet(win, 0);
    win = HuWinExCreate(210.0f, 72.0f, 0x15A, 0xD0, -1);
    lbl_1_bss_19FE[4] = win;
    HuWinMesSpeedSet(win, 0);
    win = HuWinCreate(464.0f, 240.0f, 0x7E, 0x30, 0);
    lbl_1_bss_19FE[5] = win;
    HuWinBGTPLvlSet(win, 0.0f);
    HuWinMesSpeedSet(win, 0);
    fn_1_2709C(win, lbl_1_bss_19D4);
    win = HuWinExCreate(210.0f, 280.0f, 0x15A, 0x92, -1);
    lbl_1_bss_19FE[6] = win;
    HuWinMesSpeedSet(win, 0);
    for (i = 0; i < 10; i++) {
        win = HuWinCreate(0.0f, 0.0f, 0x196, 0x28, 0);
        lbl_1_bss_19EA[i] = win;
        HuWinBGTPLvlSet(win, 0.0f);
        HuWinMesSpeedSet(win, 0);
        win = HuWinCreate(0.0f, 0.0f, 0x5E, 0x28, 0);
        lbl_1_bss_19D6[i] = win;
        HuWinBGTPLvlSet(win, 0.0f);
        HuWinMesSpeedSet(win, 0);
    }
    lbl_1_bss_19CE[1] = -1;
    lbl_1_bss_19CE[0] = -1;
    lbl_1_bss_1984 = HuPrcChildCreate(fn_1_26F0C, 0x3E8, 0x1000, 0, HuPrcCurrentGet());
    fn_1_279F8(0, 0);
    Hu3DModelAttrSet(lbl_1_bss_58C[3], 1);
    Hu3DModelAttrSet(lbl_1_bss_58C[4], 1);
    Hu3DModelAttrSet(lbl_1_bss_58C[5], 1);
    Hu3DModelAttrSet(lbl_1_bss_58C[2], 1);
    anim = HuSprAnimRead(HuDataSelHeapReadNum(0xE70002, HU_MEMNUM_OVL, HEAP_MODEL));
    grp = HuSprGrpCreate(5);
    lbl_1_bss_1988 = grp;
    HuSprGrpMemberSet(grp, 0, HuSprCreate(anim, 100, 0x13));
    HuSprPosSet(grp, 0, 320.0f, 240.0f);
    HuSprScaleSet(grp, 0, 1.0f, -1.0f);
    HuSprGrpMemberSet(grp, 1, HuSprCreate(anim, 100, 0x13));
    HuSprPosSet(grp, 1, 320.0f, 240.0f);
    HuSprGrpMemberSet(grp, 2, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(0xE70001, HU_MEMNUM_OVL, HEAP_MODEL)), 100, 0));
    HuSprPosSet(grp, 2, 320.0f, 240.0f);
    HuSprGrpMemberSet(grp, 3, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(0xBE0032, HU_MEMNUM_OVL, HEAP_MODEL)), 20, 0));
    HuSprPosSet(grp, 3, 32.0f, 54.0f);
    HuSprGrpMemberSet(grp, 4, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(0xBE0033, HU_MEMNUM_OVL, HEAP_MODEL)), 20, 0));
    HuSprPosSet(grp, 4, 224.0f, 54.0f);
    HuSprGrpDrawNoSet(grp, 0);
    Hu3DCameraPerspectiveSet(8, 5.0f, 20.0f, 15000.0f, 1.2f);
    Hu3DCameraViewportSet(8, -3.5555496f, 72.0f, 256.0f, 192.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(8, 0x16, 0x4C, 0xCC, 0xB8);
    omCameraViewSetMulti(8, &lbl_1_data_2188);
    HuDataDirClose(0xBE0000);
}

static void fn_1_26DAC(void)
{
    s16 i;

    for (i = 0; i < 7; i++) {
        HuWinKill(lbl_1_bss_19FE[i]);
    }
    for (i = 0; i < 10; i++) {
        HuWinKill(lbl_1_bss_19EA[i]);
        HuWinKill(lbl_1_bss_19D6[i]);
    }
    HuPrcKill(lbl_1_bss_1984);
    Hu3DModelAttrReset(lbl_1_bss_58C[3], 1);
    Hu3DModelAttrReset(lbl_1_bss_58C[4], 1);
    Hu3DModelAttrReset(lbl_1_bss_58C[5], 1);
    Hu3DModelAttrReset(lbl_1_bss_58C[2], 1);
    HuSprGrpKill(lbl_1_bss_1988);
    if (lbl_1_bss_19CE[0] != -1) {
        Hu3DModelKill(lbl_1_bss_19CE[0]);
    }
    if (lbl_1_bss_19CE[1] != -1) {
        Hu3DModelKill(lbl_1_bss_19CE[1]);
    }
}

/* Spins the part preview models. */
static void fn_1_26F0C(void)
{
    s16 rot = 0;

    while (1) {
        if (lbl_1_bss_19CE[0] >= 0) {
            Hu3DModelRotSet(lbl_1_bss_19CE[0], 0.0f, rot, 0.0f);
            if (lbl_1_bss_19CE[1] >= 0) {
                Hu3DModelRotSet(lbl_1_bss_19CE[1], 0.0f, rot, 0.0f);
            }
            rot += 2;
            if (rot > 360) {
                rot -= 360;
            }
        }
        HuPrcVSleep();
    }
}

// NONMATCHING: restructured from the decompiled flow; same behaviour, different codegen
static void fn_1_27004(s16 no)
{
    sprintf(lbl_1_bss_19C8, "%03d", GwCommon.sdPoint[no]);
    HuWinInsertMesSet(lbl_1_bss_19FE[1], MESSNUM_PTR(lbl_1_bss_19C8), 0);
    HuWinMesSet(lbl_1_bss_19FE[1], lbl_1_data_21A4[no]);
}

/* "owned / total" counter for a category. */
// NONMATCHING: restructured from the decompiled flow; same behaviour, different codegen
static void fn_1_2709C(s16 winId, s16 picNo)
{
    s16 count;
    s16 i;

    count = 0;
    for (i = 0; i < SdMachinePartNum[picNo]; i++) {
        if (GwSdCommon.partFlag[picNo] & (1 << i)) {
            count++;
        }
    }
    sprintf(lbl_1_bss_19C2, "%02d", count);
    sprintf(&lbl_1_bss_19C2[3], "%02d", SdMachinePartNum[picNo]);
    HuWinInsertMesSet(winId, MESSNUM_PTR(lbl_1_bss_19C2), 0);
    HuWinInsertMesSet(winId, MESSNUM_PTR(&lbl_1_bss_19C2[3]), 1);
    HuWinMesSet(winId, 0x19002C);
    HuWinScaleSet(winId, 0.8f, 0.8f);
}

/* Fills the five visible list rows starting at part `top` of category `cat`. */
// NONMATCHING: restructured from the decompiled flow; same behaviour, different codegen
static void fn_1_271EC(s16 cat, s16 top)
{
    SDMACHINE_PART *part;
    s16 i;
    s16 stat;

    part = SdMachinePartTbl[cat] + top;
    for (i = 0; i < 5; i++) {
        if (i + top > SdMachinePartNum[cat]) {
            HuWinHomeClear(lbl_1_bss_19EA[i]);
            HuWinHomeClear(lbl_1_bss_19D6[i]);
            continue;
        }
        HuWinMesPalSet(lbl_1_bss_19EA[i], 7, 0xFF, 0xFF, 0xFF);
        HuWinMesPalSet(lbl_1_bss_19D6[i], 7, 0xFF, 0xFF, 0xFF);
        stat = fn_1_414C(cat, top + i);
        if (stat == 1) {
            HuWinMesSet(lbl_1_bss_19EA[i], 0x450007);
            HuWinHomeClear(lbl_1_bss_19D6[i]);
        } else {
            if (stat == 2) {
                HuWinMesPalSet(lbl_1_bss_19EA[i], 7, 0, 0, 0);
                HuWinMesPalSet(lbl_1_bss_19D6[i], 7, 0, 0, 0);
            } else if (stat == 3) {
                HuWinMesPalSet(lbl_1_bss_19EA[i], 7, 0xC0, 0x20, 0x20);
                HuWinMesPalSet(lbl_1_bss_19D6[i], 7, 0xC0, 0x20, 0x20);
            }
            HuWinMesSet(lbl_1_bss_19EA[i], part->nameMes);
            sprintf(lbl_1_bss_1990[i], "%02d", part->cost);
            HuWinMesSet(lbl_1_bss_19D6[i], MESSNUM_PTR(lbl_1_bss_1990[i]));
        }
        part++;
    }
    HuWinMesSet(lbl_1_bss_19FE[0], cat + 0x190027);
    if (top + 5 < SdMachinePartNum[cat]) {
        HuSprAttrReset(lbl_1_bss_1988, 1, 4);
    } else {
        HuSprAttrSet(lbl_1_bss_1988, 1, 4);
    }
    if (top > 0) {
        HuSprAttrReset(lbl_1_bss_1988, 0, 4);
    } else {
        HuSprAttrSet(lbl_1_bss_1988, 0, 4);
    }
    if (lbl_1_bss_1980 >= 0) {
        HuSprAttrReset(lbl_1_bss_1988, 2, 4);
    } else {
        HuSprAttrSet(lbl_1_bss_1988, 2, 4);
    }
    HuSprPosSet(lbl_1_bss_1988, 2, lbl_1_bss_198E + 10, 8.0f + (32.0f + (lbl_1_bss_198C + (32.0f * (lbl_1_bss_1980 - top)))));
    if (fn_1_414C(cat, lbl_1_bss_1980) == 1) {
        HuWinHomeClear(lbl_1_bss_19FE[6]);
        return;
    }
    HuWinMesSet(lbl_1_bss_19FE[6], SdMachinePartTbl[cat][lbl_1_bss_1980].descMes);
}

/* Moves the shop windows and sprites to (x, y). */
static void fn_1_27730(f32 x, f32 y)
{
    s16 i;

    for (i = 0; i < 5; i++) {
        HuWinPosSet(lbl_1_bss_19EA[i], 16.0f + x, 16.0f + (y + (f32)(i << 5)));
        HuWinPosSet(lbl_1_bss_19D6[i], 288.0f + x, 16.0f + (y + (f32)(i << 5)));
    }
    HuWinPosSet(lbl_1_bss_19FE[4], x, y);
    HuWinPosSet(lbl_1_bss_19FE[5], 8.0f + (240.0f + x), 16.0f + (160.0f + y));
    HuSprPosSet(lbl_1_bss_1988, 0, 151.0f + x, 16.0f + y);
    HuSprPosSet(lbl_1_bss_1988, 1, 151.0f + x, 32.0f + (160.0f + y));
    HuSprPosSet(lbl_1_bss_1988, 2, 10.0f + x, 8.0f + (32.0f + (y + (f32)(lbl_1_bss_1980 << 5))));
    lbl_1_bss_198E = x;
    lbl_1_bss_198C = y;
}

/* (Re)builds the preview model for part `no` of category `cat`. */
// NONMATCHING: restructured from the decompiled flow; same behaviour, different codegen
static void fn_1_279F8(s16 cat, s16 no)
{
    SDMACHINE_PART *part;
    void *data;
    s16 mdl;

    part = SdMachinePartTbl[cat] + no;
    omCameraViewSetMulti(8, &lbl_1_data_2188);
    if (lbl_1_bss_19CE[0] != -1) {
        Hu3DModelHookReset(lbl_1_bss_19CE[0]);
        Hu3DModelKill(lbl_1_bss_19CE[0]);
    }
    if (lbl_1_bss_19CE[1] != -1) {
        Hu3DModelKill(lbl_1_bss_19CE[1]);
    }
    lbl_1_bss_19CE[1] = -1;
    lbl_1_bss_19CE[0] = -1;
    if (fn_1_414C(cat, no) == 1) {
        return;
    }
    if (HuARDirCheck(part->modelFile) != 0) {
        data = HuAR_ARAMtoMRAMFileRead(part->modelFile, HU_MEMNUM_OVL, HEAP_MODEL);
    } else {
        data = HuDataSelHeapReadNum(part->modelFile, HU_MEMNUM_OVL, HEAP_MODEL);
    }
    mdl = Hu3DModelCreate(data);
    lbl_1_bss_19CE[0] = mdl;
    Hu3DModelCameraSet(mdl, 8);
    if (lbl_1_bss_19D4 == 2) {
        Hu3DModelScaleSet(mdl, 1.5f, 1.5f, 1.5f);
    }
    Hu3DModelAttrSet(mdl, 0x40000001);
    Hu3DModelAttrSet(mdl, 0x40000040);
    if (lbl_1_bss_19D4 == 2 && no == 10) {
        mdl = Hu3DModelCreate(HuDataSelHeapReadNum(0xBE0011, HU_MEMNUM_OVL, HEAP_MODEL));
        lbl_1_bss_19CE[1] = mdl;
        Hu3DModelCameraSet(mdl, 8);
        Hu3DModelPosSet(mdl, 0.0f, 75.0f, 0.0f);
        Hu3DModelScaleSet(mdl, 0.45f, 0.45f, 0.45f);
        Hu3DModelLayerSet(mdl, 0);
        Hu3DModelLayerSet(lbl_1_bss_19CE[0], 1);
    }
    if (lbl_1_bss_19D4 == 1 && no != 7 && no != 9) {
        Hu3DModelPosSet(lbl_1_bss_19CE[0], 0.0f, 50.0f, 0.0f);
        mdl = Hu3DModelLink(lbl_1_bss_19CE[0]);
        lbl_1_bss_19CE[1] = mdl;
        Hu3DModelCameraSet(mdl, 8);
        Hu3DModelScaleSet(mdl, -1.0f, 1.0f, 1.0f);
        Hu3DModelPosSet(mdl, 0.0f, 50.0f, 0.0f);
        Hu3DModelAttrSet(mdl, 0x800000);
        Hu3DModelAttrSet(mdl, 0x40000001);
        Hu3DModelAttrSet(mdl, 0x40000040);
    }
}

/* Buy part `no` of category `cat` (confirmation window, point deduction, unlock). */
// NONMATCHING: restructured from the decompiled flow; same behaviour, different codegen
static void fn_1_27D80(s16 cat, s16 no)
{
    SDMACHINE_PART *part;
    HUWIN *win;
    s16 choice;

    part = SdMachinePartTbl[cat] + no;
    HuWinExOpen(lbl_1_bss_570);
    win = &winData[lbl_1_bss_570];
    win->choiceEndSe = -1;
    win->padMask = 1;
    HuWinInsertMesSet(lbl_1_bss_570, part->nameMes, 0);
    HuWinMesSet(lbl_1_bss_570, 0x190008);
    HuWinMesWait(lbl_1_bss_570);
    choice = HuWinChoiceGet(lbl_1_bss_570, -1);
    if (choice == 0) {
        HuAudFXPlay(0x508);
        GwCommon.sdPoint[cat] -= part->cost;
        fn_1_27004(cat);
        fn_1_3E8C(cat, no);
        fn_1_2709C(lbl_1_bss_19FE[5], lbl_1_bss_19D4);
        if (fn_1_414C(3, 0xB) == 1 && (GwSdCommon.partFlag[0] & 0x7FF) == 0x7FF && (GwSdCommon.partFlag[1] & 0x7FF) == 0x7FF
            && (GwSdCommon.partFlag[2] & 0x7FF) == 0x7FF && (GwSdCommon.partFlag[3] & 0x7FF) == 0x7FF) {
            GwSdCommon.newPart3F = 1;
        }
        fn_1_3F00();
        fn_1_271EC(cat, lbl_1_bss_19D2);
        HuPrcSleep(0x14);
    } else if (choice == 1) {
        HuAudFXPlay(2);
    }
    win->choiceEndSe = 2;
    HuWinExClose(lbl_1_bss_570);
}

/* Announces newly earned points / newly unlocked parts. */
// NONMATCHING: restructured from the decompiled flow; same behaviour, different codegen
static void fn_1_28104(void)
{
    char buf[8];
    s16 total;
    s16 i;

    total = 0;
    for (i = 0; i < 4; i++) {
        total += GwCommon.sdPoint[i];
    }
    if (total > GwSdCommon.prevPoint) {
        sprintf(buf, "%d", total - GwSdCommon.prevPoint);
        HuWinInsertMesSet(lbl_1_bss_570, MESSNUM_PTR(buf), 0);
        HuWinMesSet(lbl_1_bss_570, 0x190003);
        HuAudFXPlay(0x2EF);
        HuWinMesWait(lbl_1_bss_570);
        GwSdCommon.prevPoint = total;
    }
    if (GwSdCommon.newPart1F || GwSdCommon.newPart2F || ((GwSdCommon.partFlag[3] & 0x1800) == 0 && GwSdCommon.newPart3F)) {
        HuWinMesSet(lbl_1_bss_570, 0x190004);
        HuAudFXPlay(0x2EF);
        HuWinMesWait(lbl_1_bss_570);
        GwSdCommon.newPart3F = 0;
        GwSdCommon.newPart2F = GwSdCommon.newPart3F;
        GwSdCommon.newPart1F = GwSdCommon.newPart2F;
    }
}
