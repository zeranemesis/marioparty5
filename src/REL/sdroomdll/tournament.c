#include "REL/sdroomdll/tournament.h"
#include "REL/sdroomdll/sdroom.h"
#include "game/window.h"
#include "game/sprite.h"
#include "game/memory.h"
#include "game/frand.h"
#include "game/mgdata.h"
#include "game/hsfex.h"
#include "game/init.h"
#include "humath.h"
#include <stdio.h>

/* mgModeWork is used as a byte table: [0..7] entrant machine ids (0x2B = none),
 * [8..15] tournament round reached by each entrant. */
#define TNM_ENTRY(i) (((u8 *)mgModeWork)[(i)])
#define TNM_STAGE(i) (((u8 *)mgModeWork)[8 + (i)])

/* HUPROCESS.property doubles as a small integer mailbox between the processes */
#define TNM_PROP_SET(proc, v) ((proc)->property = (void *)(uintptr_t)(u32)(v))
#define TNM_PROP_GET(proc) ((u32)(uintptr_t)(proc)->property)

/* other units */
extern void fn_1_20870(void);
extern void fn_1_23280(void);

extern u16 HuSprAttrGet(HUSPRGRPID grpId, s16 memberNo);

static void fn_1_1D1B4(void);
static void fn_1_1DFA8(void);
static void fn_1_1E090(s16 layerNo);
static void fn_1_1E0EC(void);
static void fn_1_1E154(void);
static void fn_1_1E2E0(void);
static s16 fn_1_1E51C(s16 *out);
static s16 fn_1_1E6A8(void);
static void fn_1_1E880(s16 winner, s16 loser, s16 spr);
static void fn_1_1F21C(void);
static void fn_1_1F78C(s16 *arr, s16 count);
static s16 fn_1_1F82C(s16 id);
static void fn_1_1F960(s16 winner);
static s16 fn_1_20038(s16 *pair);
static void fn_1_20300(void);
static void fn_1_20760(void);

/* tournament bss (0xC00..0xC38) */
static s16 lbl_1_bss_C00; /* cursor moving */
static s16 lbl_1_bss_C02; /* cursor entrant */
static f32 lbl_1_bss_C04; /* cursor pulse scale */
static ANIMDATA *lbl_1_bss_C08; /* capture texture */
static void *lbl_1_bss_C0C; /* capture buffer */
static s32 lbl_1_bss_C10; /* pause marker blink */
static HUPROCESS *lbl_1_bss_C14; /* camera sway process */
static HUPROCESS *lbl_1_bss_C18; /* trophy spin process */
static HUPROCESS *lbl_1_bss_C1C; /* marker blink process */
static s16 lbl_1_bss_C20; /* trophy model */
static s16 lbl_1_bss_C22[6]; /* stage models */
static s16 lbl_1_bss_C2E[2]; /* windows */
static s16 lbl_1_bss_C32; /* sprite group */

static OMCAMERAVIEW lbl_1_data_1B60 = { { 0.0f, 100.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, 500.0f };

/* bracket sprite offsets: x, y, x scale */
static struct {
    f32 x;
    f32 y;
    f32 sx;
} lbl_1_data_1B7C[14] = {
    { -66.0f, -26.0f, 1.0f },
    { 65.0f, -26.0f, -1.0f },
    { -133.0f, -15.0f, 1.0f },
    { -138.0f, -15.0f, -1.0f },
    { 137.0f, -15.0f, 1.0f },
    { 132.0f, -15.0f, -1.0f },
    { -168.0f, 26.0f, 1.0f },
    { -237.0f, 26.0f, -1.0f },
    { -36.0f, 26.0f, 1.0f },
    { -101.0f, 26.0f, -1.0f },
    { 101.0f, 26.0f, 1.0f },
    { 35.0f, 26.0f, -1.0f },
    { 235.0f, 26.0f, 1.0f },
    { 169.0f, 26.0f, -1.0f },
};

/* bracket slot positions: 0-7 start, 8-15 first win line, 16-19 / 20-23 round 1,
 * 24-25 / 26-27 round 2, 28 / 29 final */
static struct {
    f32 x;
    f32 y;
} lbl_1_data_1C24[30] = {
    { 51.0f, 331.0f }, { 119.0f, 331.0f }, { 185.0f, 331.0f }, { 253.0f, 331.0f },
    { 321.0f, 331.0f }, { 389.0f, 331.0f }, { 455.0f, 331.0f }, { 523.0f, 331.0f },
    { 51.0f, 269.0f }, { 119.0f, 269.0f }, { 185.0f, 269.0f }, { 253.0f, 269.0f },
    { 321.0f, 269.0f }, { 389.0f, 269.0f }, { 455.0f, 269.0f }, { 523.0f, 269.0f },
    { 85.0f, 269.0f }, { 219.0f, 269.0f }, { 355.0f, 269.0f }, { 489.0f, 269.0f },
    { 85.0f, 228.0f }, { 219.0f, 228.0f }, { 355.0f, 228.0f }, { 489.0f, 228.0f },
    { 152.0f, 228.0f }, { 422.0f, 228.0f }, { 152.0f, 187.0f }, { 422.0f, 187.0f },
    { 287.0f, 187.0f }, { 287.0f, 172.0f },
};

/* machine id (index 0x20..0x2B) -> sprite bank */
static s16 lbl_1_data_1D14[12] = { 0xA, 0xB, 1, 0, 2, 4, 8, 3, 6, 5, 7, 9 };

/* Opponent pools per game type. The pools are edited while a bracket is built, so the
 * pristine values are kept to restore them (the module is not reloaded on PC). */
static s16 lbl_1_data_1D2C[3] = { 2, 4, 5 };
static s16 lbl_1_data_1D32[2] = { 0x20, 0x21 };
static s16 lbl_1_data_1D36[4] = { 0x22, 0x23, 0x24, 0x25 };
static s16 lbl_1_data_1D3E[5] = { 0x26, 0x27, 0x28, 0x29, 0x2A };
static s16 lbl_1_data_1D48[3] = { 3, 3, 5 };
static s16 lbl_1_data_1D4E[3] = { 0x22, 0x23, 0x24 };
static s16 lbl_1_data_1D54[3] = { 0x20, 0x21, 0x26 };
static s16 lbl_1_data_1D5A[5] = { 0x25, 0x27, 0x28, 0x29, 0x2A };
static s16 lbl_1_data_1D64[3] = { 2, 4, 5 };
static s16 lbl_1_data_1D6A[2] = { 0x23, 0x24 };
static s16 lbl_1_data_1D6E[4] = { 0x20, 0x21, 0x22, 0x26 };
static s16 lbl_1_data_1D76[5] = { 0x25, 0x27, 0x28, 0x29, 0x2A };

static OMCAMERAVIEW lbl_1_data_1D80 = { { 0.0f, 140.0f, -40.0f }, { -23.0f, -69.0f, 0.0f }, 6856.0f };

/* slot of entrant p once it reached the given round */
static int TnmSlot(int p, int stage)
{
    if (stage == 0) {
        return p;
    } else if (stage == 1) {
        return (p / 2) + 0x10;
    }
    return (p / 4) + 0x18;
}

static void TnmSprPosSet(s16 p, s16 pStage, f32 yOfs)
{
    int slot = TnmSlot(p, pStage);

    HuSprPosSet(lbl_1_bss_C32, p + 0xF, lbl_1_data_1C24[slot].x, lbl_1_data_1C24[slot].y - yOfs);
}

static void TnmPoolReset(void)
{
    static const s16 d2C[3] = { 2, 4, 5 };
    static const s16 d48[3] = { 3, 3, 5 };
    static const s16 d32[2] = { 0x20, 0x21 };
    static const s16 d36[4] = { 0x22, 0x23, 0x24, 0x25 };
    static const s16 d3E[5] = { 0x26, 0x27, 0x28, 0x29, 0x2A };
    static const s16 d4E[3] = { 0x22, 0x23, 0x24 };
    static const s16 d54[3] = { 0x20, 0x21, 0x26 };
    static const s16 d5A[5] = { 0x25, 0x27, 0x28, 0x29, 0x2A };
    static const s16 d6A[2] = { 0x23, 0x24 };
    static const s16 d6E[4] = { 0x20, 0x21, 0x22, 0x26 };
    static const s16 d76[5] = { 0x25, 0x27, 0x28, 0x29, 0x2A };
    s32 i;

    for (i = 0; i < 3; i++) {
        lbl_1_data_1D2C[i] = d2C[i];
        lbl_1_data_1D48[i] = d48[i];
        lbl_1_data_1D64[i] = d2C[i];
        lbl_1_data_1D4E[i] = d4E[i];
        lbl_1_data_1D54[i] = d54[i];
    }
    for (i = 0; i < 2; i++) {
        lbl_1_data_1D32[i] = d32[i];
        lbl_1_data_1D6A[i] = d6A[i];
    }
    for (i = 0; i < 4; i++) {
        lbl_1_data_1D36[i] = d36[i];
        lbl_1_data_1D6E[i] = d6E[i];
    }
    for (i = 0; i < 5; i++) {
        lbl_1_data_1D3E[i] = d3E[i];
        lbl_1_data_1D5A[i] = d5A[i];
        lbl_1_data_1D76[i] = d76[i];
    }
}

/* selects the pools (A, B, C and their counts) of the current game type */
static void TnmPoolGet(s16 **a, s16 **b, s16 **c, s16 **cnt)
{
    if (sdWorkInt[3] == 0) {
        *a = lbl_1_data_1D32;
        *b = lbl_1_data_1D36;
        *c = lbl_1_data_1D3E;
        *cnt = lbl_1_data_1D2C;
    } else if (sdWorkInt[3] == 1) {
        *a = lbl_1_data_1D4E;
        *b = lbl_1_data_1D54;
        *c = lbl_1_data_1D5A;
        *cnt = lbl_1_data_1D48;
    } else {
        *a = lbl_1_data_1D6A;
        *b = lbl_1_data_1D6E;
        *c = lbl_1_data_1D76;
        *cnt = lbl_1_data_1D64;
    }
}

void fn_1_1C25C(void)
{
}

/* Tournament bracket screen. */
void fn_1_1C260(void)
{
    s16 pair[4];
    HuVec2f size;
    s16 i;
    s16 win;
    s16 nextRound;
    s16 spr;
    s16 w;
    s16 l;
    u32 msg;
    s8 padNo;
    s8 winner;
    f32 off;
    OMOVLHIS *his;
    s16 mask;

    padNo = GwSdBattle.padNo[0];
    lbl_1_bss_C10 = 0;
    omCameraViewSetMulti(4, &lbl_1_data_1B60);
    HuPrcVSleep();
    if (omovlevtno != 4) {
        for (i = 0; i < 8; i++) {
            TNM_STAGE(i) = 0;
        }
        fn_1_1F21C();
        winner = -1;
        lbl_1_bss_C1C = NULL;
        fn_1_1D1B4();
        fn_1_1E51C(pair);
        lbl_1_bss_C1C = HuPrcChildCreate(fn_1_1E154, 0x3E8, 0x3000, 0, HuPrcCurrentGet());
        TNM_PROP_SET(lbl_1_bss_C1C, (pair[0] << 8) | pair[1]);
    } else {
        winner = GwSdBattle.winnerNo;
        fn_1_1E51C(pair);
        if (pair[0] == 0 && TNM_STAGE(pair[0]) == 2 && (winner == 0 || winner == -1)) {
            fn_1_23280();
            winner = GwSdBattle.winnerNo;
        }
        if (winner == 0 && pair[0] == 0 && GwSdBattle.machine[1].charNo == 0x20) {
            if (!GwSdCommon.extCharSelF) {
                GwSdCommon.newPart1F = 1;
            }
            GwSdCommon.extCharSelF = 1;
        }
        lbl_1_bss_C1C = HuPrcChildCreate(fn_1_1E154, 0x3E8, 0x3000, 0, HuPrcCurrentGet());
        TNM_PROP_SET(lbl_1_bss_C1C, (pair[0] << 8) | pair[1]);
        fn_1_1D1B4();
        TnmSprPosSet(pair[0], TNM_STAGE(pair[0]), 24.0f);
        TnmSprPosSet(pair[1], TNM_STAGE(pair[0]), 24.0f);
    }
    HuAudSStreamPlay(0x25);
    WipeCreate(1, 0, 0x3C);
    while (WipeCheckEnd() != 0) {
        HuPrcVSleep();
    }
    while (1) {
        fn_1_1E51C(pair);
        if (winner != -1) {
            w = pair[winner];
            l = pair[winner ^ 1];
            TNM_STAGE(w)++;
            nextRound = fn_1_1E51C(pair);
            spr = fn_1_1E6A8();
            if (spr != -1) {
                fn_1_1E880(w, l, spr);
                if (nextRound == -1) {
                    fn_1_1F960(w);
                    his = omOvlHisGet(0);
                    omOvlHisChg(0, his->ovl, 3, his->stat);
                    WipeCreate(2, 0, 0x3C);
                    HuAudSStreamAllFadeOut(1000);
                    while (WipeCheckEnd() != 0) {
                        HuPrcVSleep();
                    }
                    fn_1_1DFA8();
                    omOvlReturnEx(0, 1);
                    while (1) {
                        HuPrcVSleep();
                    }
                }
            }
            if (lbl_1_bss_C1C != NULL) {
                TNM_PROP_SET(lbl_1_bss_C1C, 1);
            }
            lbl_1_bss_C1C = HuPrcChildCreate(fn_1_1E154, 0x3E8, 0x3000, 0, HuPrcCurrentGet());
            TNM_PROP_SET(lbl_1_bss_C1C, (pair[0] << 8) | pair[1]);
        }
        if (winner != -1 || omovlevtno != 4) {
            for (i = 1; i <= 10; i++) {
                off = 24.0 * HuSin(90.0 * ((f64)i / 10.0));
                TnmSprPosSet(pair[0], TNM_STAGE(pair[0]), off);
                TnmSprPosSet(pair[1], TNM_STAGE(pair[0]), off);
                HuPrcVSleep();
            }
        }
        if (pair[0] == 0) {
            msg = 0x1C000B;
        } else {
            msg = 0x1C000C;
        }
        HuWinMesSet(lbl_1_bss_C2E[1], msg);
        HuWinDispOn(lbl_1_bss_C2E[1]);
        while (1) {
            mask = 1 << padNo;
            if (fn_1_4FD0(mask) & 0x1000) {
                if (pair[0] != 0) {
                    winner = fn_1_20038(pair);
                    break;
                }
                WipeCreate(2, 0, 0x3C);
                HuAudSStreamAllFadeOut(1000);
                while (WipeCheckEnd() != 0) {
                    HuPrcVSleep();
                }
                lbl_1_bss_C10 = 1;
                HuPrcSleep(2);
                fn_1_1DFA8();
                if (pair[0] == 0) {
                    GwPlayerConf[0].type = 0;
                    GwSdBattle.stageType1 = -1;
                    GwSdBattle.machine[0] = GwSdCommon.machine[sdWorkInt[7]];
                } else {
                    GwPlayerConf[0].type = 1;
                    GwSdBattle.stageType1 = fn_1_1F82C(TNM_ENTRY(pair[0]));
                    GwPlayerConf[0].dif = 1;
                    GwSdBattle.machine[0].charNo = TNM_ENTRY(pair[0]);
                }
                GwPlayerConf[1].type = 1;
                GwSdBattle.stageType2 = fn_1_1F82C(TNM_ENTRY(pair[1]));
                GwPlayerConf[1].dif = 1;
                GwSdBattle.machine[1].charNo = TNM_ENTRY(pair[1]);
                if (TNM_STAGE(pair[0]) == 2) {
                    fn_1_20870();
                }
                fn_1_4238(4);
                while (1) {
                    HuPrcVSleep();
                }
            }
            if (fn_1_4FD0(mask) & 0x100) {
                HuAudFXPlay(1);
                lbl_1_bss_C10 = 1;
                HuPrcVSleep();
                fn_1_20300();
                lbl_1_bss_C1C = HuPrcChildCreate(fn_1_1E154, 0x3E8, 0x3000, 0, HuPrcCurrentGet());
                TNM_PROP_SET(lbl_1_bss_C1C, (pair[0] << 8) | pair[1]);
                HuWinMesSet(lbl_1_bss_C2E[1], msg);
                lbl_1_bss_C10 = 0;
                HuPrcVSleep();
            }
            if (fn_1_4FD0(mask) & 0x200) {
                HuAudFXPlay(3);
                HuWinMesMaxSizeGet(1, &size, 0x19007C);
                win = HuWinExCreate(-10000.0f, 200.0f, (s32)size.x, (s32)size.y, -1);
                HuWinPriSet(win, 0xA);
                HuWinExOpen(win);
                HuWinMesSet(win, 0x19007C);
                HuWinMesWait(win);
                winData[win].padMask = 1 << sdWorkInt[7];
                if (HuWinChoiceGet(win, -1) == 0) {
                    WipeCreate(2, 0, 0x3C);
                    HuAudSStreamAllFadeOut(1000);
                    while (WipeCheckEnd() != 0) {
                        HuPrcVSleep();
                    }
                    his = omOvlHisGet(0);
                    omOvlHisChg(0, his->ovl, 3, his->stat);
                    fn_1_1DFA8();
                    omOvlReturnEx(0, 1);
                    while (1) {
                        HuPrcVSleep();
                    }
                }
                HuWinExClose(win);
                HuWinExKill(win);
            }
            HuPrcVSleep();
        }
    }
}

/* Creates the bracket windows, sprites, trophy and the 3D backdrop. */
static void fn_1_1D1B4(void)
{
    HuVecF src;
    HuVecF dst;
    ANIMDATA *anim;
    HUSPRGRPID grp;
    s16 pair[4];
    s16 i;
    s16 win;
    s16 mdl;
    s16 slot;
    s16 bank;

    win = HuWinCreate(-10000.0f, 32.0f, 0x240, 0x2A, 0);
    lbl_1_bss_C2E[0] = win;
    HuWinBGTPLvlSet(win, 0.0f);
    HuWinMesSpeedSet(win, 0);
    HuWinAttrSet(win, 0x800);
    HuWinInsertMesSet(win, sdWorkInt[3] + 0x190035, 1);
    HuWinInsertMesSet(win, sdWorkInt[6] + 0x190038, 2);
    HuWinMesSet(win, 0x190034);
    win = HuWinCreate(-10000.0f, 406.0f, 0x240, 0x2A, 0);
    lbl_1_bss_C2E[1] = win;
    HuWinBGTPLvlSet(win, 0.0f);
    HuWinMesSpeedSet(win, 0);
    HuWinAttrSet(win, 0x800);
    HuWinMesSet(win, 0x1C000B);
    HuWinDispOff(win);
    grp = HuSprGrpCreate(0x19);
    lbl_1_bss_C32 = grp;
    HuSprGrpMemberSet(grp, 0, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(0xBD0017, HU_MEMNUM_OVL, HEAP_MODEL)), 0x14, 0));
    HuSprPosSet(grp, 0, 288.0f, 240.0f);
    anim = HuSprAnimRead(HuDataSelHeapReadNum(0xBD0018, HU_MEMNUM_OVL, HEAP_MODEL));
    for (i = 0; i < 2; i++) {
        HuSprGrpMemberSet(grp, i + 1, HuSprCreate(anim, 0x14, 0));
        HuSprPosSet(grp, i + 1, 288.0f + lbl_1_data_1B7C[i].x, 240.0f + lbl_1_data_1B7C[i].y);
        HuSprScaleSet(grp, i + 1, lbl_1_data_1B7C[i].sx, 1.0f);
        HuSprAttrSet(grp, i + 1, 0x84);
        HuSprColorSet(grp, i + 1, 0xFF, 0xC, 0x20);
    }
    anim = HuSprAnimRead(HuDataSelHeapReadNum(0xBD0019, HU_MEMNUM_OVL, HEAP_MODEL));
    for (i = 0; i < 4; i++) {
        HuSprGrpMemberSet(grp, i + 3, HuSprCreate(anim, 0x14, 0));
        HuSprPosSet(grp, i + 3, 288.0f + lbl_1_data_1B7C[i + 2].x, 240.0f + lbl_1_data_1B7C[i + 2].y);
        HuSprScaleSet(grp, i + 3, lbl_1_data_1B7C[i + 2].sx, 1.0f);
        HuSprAttrSet(grp, i + 3, 0x84);
        HuSprColorSet(grp, i + 3, 0xFF, 0xC, 0x20);
    }
    anim = HuSprAnimRead(HuDataSelHeapReadNum(0xBD001A, HU_MEMNUM_OVL, HEAP_MODEL));
    for (i = 0; i < 8; i++) {
        HuSprGrpMemberSet(grp, i + 7, HuSprCreate(anim, 0x14, 0));
        HuSprPosSet(grp, i + 7, 288.0f + lbl_1_data_1B7C[i + 6].x, 240.0f + lbl_1_data_1B7C[i + 6].y);
        HuSprScaleSet(grp, i + 7, lbl_1_data_1B7C[i + 6].sx, 1.0f);
        HuSprAttrSet(grp, i + 7, 0x84);
        HuSprColorSet(grp, i + 7, 0xFF, 0xC, 0x20);
    }
    fn_1_1E6A8();
    fn_1_1E51C(pair);
    anim = HuSprAnimRead(HuDataSelHeapReadNum(0xBD001B, HU_MEMNUM_OVL, HEAP_MODEL));
    for (i = 0; i < 8; i++) {
        HuSprGrpMemberSet(grp, i + 0xF, HuSprCreate(anim, 0x14, 0));
        slot = TnmSlot(i, TNM_STAGE(i));
        HuSprPosSet(grp, i + 0xF, lbl_1_data_1C24[slot].x, lbl_1_data_1C24[slot].y);
        HuSprAttrSet(grp, i + 0xF, 8);
        if (i == 0) {
            HuSprBankSet(grp, i + 0xF, TNM_ENTRY(i));
        } else {
            bank = TNM_ENTRY(i) - 0x20;
            HuSprBankSet(grp, i + 0xF, (u8)lbl_1_data_1D14[bank]);
        }
    }
    mdl = Hu3DModelCreate(HuDataSelHeapReadNum(0xBD0000 + sdWorkInt[3] * 3 + sdWorkInt[6] + 0xA, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_C20 = mdl;
    Hu3DModelReflectTypeSet(mdl, 2);
    Hu3DModelCameraSet(mdl, 4);
    src.x = lbl_1_data_1C24[0x1D].x;
    src.y = lbl_1_data_1C24[0x1D].y - 48.0f;
    src.z = 700.0f;
    Hu3D2Dto3D(&src, 4, &dst);
    Hu3DModelPosSetV(mdl, &dst);
    PSMTXTrans(Hu3DData[mdl].mtx, 0.0f, -50.0f, 0.0f);
    mtxRotCat(Hu3DData[mdl].mtx, -5.0f, 0.0f, 5.0f);
    mdl = Hu3DModelCreate(HuDataSelHeapReadNum(0xBD0000, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_C22[0] = mdl;
    Hu3DModelAttrSet(mdl, 0x4000);
    Hu3DModelAttrSet(mdl, 0x40000001);
    Hu3DModelCameraSet(mdl, 1);
    mdl = Hu3DModelCreate(HuDataSelHeapReadNum(0xBD0001, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_C22[1] = mdl;
    Hu3DModelAttrSet(mdl, 0x4000);
    Hu3DModelAttrSet(mdl, 0x40000001);
    Hu3DModelCameraSet(mdl, 1);
    mdl = Hu3DModelCreate(HuDataSelHeapReadNum(0xBD0002, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_C22[2] = mdl;
    Hu3DModelAttrSet(mdl, 0x4000);
    Hu3DModelAttrSet(mdl, 0x40000002);
    Hu3DModelCameraSet(mdl, 1);
    mdl = Hu3DModelCreate(HuDataSelHeapReadNum(0xBD0003, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_C22[3] = mdl;
    Hu3DModelAttrSet(mdl, 0x4000);
    Hu3DModelAttrSet(mdl, 0x40000001);
    Hu3DModelCameraSet(mdl, 1);
    mdl = Hu3DModelCreate(HuDataSelHeapReadNum(0xBD0005, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_C22[4] = mdl;
    Hu3DModelAttrSet(mdl, 0x4000);
    Hu3DModelAttrSet(mdl, 0x40000001);
    Hu3DModelCameraSet(mdl, 1);
    mdl = Hu3DModelCreate(HuDataSelHeapReadNum(0xBD0015, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_C22[5] = mdl;
    Hu3DModelAttrSet(mdl, 1);
    Hu3DModelCameraSet(mdl, 4);
    lbl_1_bss_C0C = HuMemDirectMallocNum(HEAP_HEAP, GXGetTexBufferSize(0x1E0, 0x168, GX_TF_RGB5A3, 0, 0), HU_MEMNUM_OVL);
    Hu3DLayerHookSet(2, fn_1_1E090);
    lbl_1_bss_C08 = HuSprAnimMake(0x1E0, 0x168, 2);
    lbl_1_bss_C08->bmp->data = lbl_1_bss_C0C;
    HuSprExecLayerCameraSet(0x4A, 1, 0xA);
    HuSprGrpMemberSet(grp, 0x17, HuSprCreate(lbl_1_bss_C08, 0x14, 0));
    HuSprAttrSet(grp, 0x17, 8);
    HuSprPosSet(grp, 0x17, 288.0f, 240.0f);
    HuSprScaleSet(grp, 0x17, 1.3333334f, 1.2f);
    HuSprColorSet(grp, 0x17, 0x60, 0x60, 0x60);
    HuSprDrawNoSet(grp, 0x17, 0x4A);
    HuSprGrpMemberSet(grp, 0x18, HuSprCreate(lbl_1_bss_C08, 0x13, 0));
    HuSprAttrSet(grp, 0x18, 8);
    HuSprPosSet(grp, 0x18, 216.0f, 180.0f);
    HuSprScaleSet(grp, 0x18, 0.927f, 1.03f);
    HuSprExecLayerCameraSet(0x4B, 1, 8);
    HuSprDrawNoSet(grp, 0x18, 0x4B);
    HuSprTPLvlSet(grp, 0x18, 0.7f);
    Hu3DAnimCreate(lbl_1_bss_C08, lbl_1_bss_C22[0], "dummy");
    Hu3DCameraPerspectiveSet(1, 40.0f, 30.0f, 15000.0f, 1.2f);
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, 480.0f, 360.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(1, 0, 0, 0x1E0, 0x168);
    omCameraViewSetMulti(1, &lbl_1_data_1D80);
    lbl_1_bss_C14 = HuPrcChildCreate(fn_1_1E2E0, 0x3E8, 0x3000, 0, HuPrcCurrentGet());
    lbl_1_bss_C18 = HuPrcChildCreate(fn_1_1E0EC, 0x3E8, 0x3000, 0, HuPrcCurrentGet());
}

static void fn_1_1DFA8(void)
{
    s16 i;

    HuSprGrpKill(lbl_1_bss_C32);
    HuWinKill(lbl_1_bss_C2E[0]);
    HuWinKill(lbl_1_bss_C2E[1]);
    Hu3DModelKill(lbl_1_bss_C20);
    HuPrcKill(lbl_1_bss_C18);
    for (i = 0; i < 6; i++) {
        Hu3DModelKill(lbl_1_bss_C22[i]);
    }
    HuSprAnimKill(lbl_1_bss_C08);
    HuMemDirectFree(lbl_1_bss_C0C);
    HuPrcKill(lbl_1_bss_C14);
    Hu3DLayerHookReset(2);
    Hu3DLayerHookReset(0xA);
}

static void fn_1_1E090(s16 layerNo)
{
    if (Hu3DCameraNo == 0) {
        Hu3DFbCopyExec(0, 0, 0x1E0, 0x168, GX_TF_RGB5A3, 0, lbl_1_bss_C0C);
        Hu3DZClear();
    }
}

/* Spins the trophy. */
static void fn_1_1E0EC(void)
{
    f32 rot = 0.0f;

    while (1) {
        Hu3DModelRotSet(lbl_1_bss_C20, 0.0f, rot, 0.0f);
        rot += 1.0f;
        HuPrcVSleep();
    }
}

/* Pulses the two marker sprites of the current match until told to stop. */
static void fn_1_1E154(void)
{
    HUPROCESS *self;
    u32 v;
    u16 a;
    u16 b;
    u16 angle;
    f32 scale;

    self = HuPrcCurrentGet();
    v = TNM_PROP_GET(self);
    a = (u16)v >> 8;
    b = (u8)(u16)v;
    angle = 0;
    TNM_PROP_SET(self, 0);
    while (1) {
        if (lbl_1_bss_C10 == 0) {
            scale = 0.9 + (0.1 * HuCos(angle));
            HuSprScaleSet(lbl_1_bss_C32, a + 0xF, scale, scale);
            HuSprScaleSet(lbl_1_bss_C32, b + 0xF, scale, scale);
            if (angle + 2 >= 0x168 && TNM_PROP_GET(self) != 0) {
                break;
            }
        } else if (TNM_PROP_GET(self) != 0) {
            break;
        }
        angle += 2;
        if (angle > 0x168) {
            angle -= 0x168;
        }
        HuPrcVSleep();
    }
    HuPrcEnd();
    HuPrcVSleep();
}

/* Sways the backdrop camera. */
static void fn_1_1E2E0(void)
{
    f32 baseX;
    f32 baseY;
    f32 baseZoom;
    f32 a;
    f32 b;
    f32 c;

    baseX = lbl_1_data_1D80.rot.x;
    baseY = lbl_1_data_1D80.rot.y;
    baseZoom = lbl_1_data_1D80.zoom;
    a = frandmod(0x168);
    b = frandmod(0x168);
    c = frandmod(0x168);
    while (1) {
        CRotM[0].x = baseX + (10.0 * HuSin(a));
        CRotM[0].y = baseY + b;
        CZoomM[0] = (0.7 * baseZoom) + (0.3 * (baseZoom * HuSin(c)));
        a = a + 0.1;
        if (a > 360.0f) {
            a -= 360.0f;
        }
        b = b + 0.05;
        if (b > 360.0f) {
            b -= 360.0f;
        }
        c = c + 0.1;
        if (c > 360.0f) {
            c -= 360.0f;
        }
        HuPrcVSleep();
    }
}

/* Finds the next match. Returns the round (0..2) and fills out[] with the two entrants, or -1. */
static s16 fn_1_1E51C(s16 *out)
{
    s16 i;
    s16 j;
    s16 cnt;

    for (i = 0; i < 8; i += 2) {
        if (TNM_STAGE(i) == 0 && TNM_STAGE(i + 1) == 0) {
            out[0] = i;
            out[1] = i + 1;
            break;
        }
    }
    if (i != 8) {
        return 0;
    }
    for (i = 0; i < 8; i += 4) {
        cnt = 0;
        for (j = 0; j < 4; j++) {
            if (TNM_STAGE(i + j) == 1) {
                out[cnt] = i + j;
                cnt++;
            }
        }
        if (cnt >= 2) {
            break;
        }
    }
    if (i != 8) {
        return 1;
    }
    cnt = 0;
    for (i = 0; i < 8; i++) {
        if (TNM_STAGE(i) == 2) {
            out[cnt] = i;
            cnt++;
        }
    }
    if (cnt == 2) {
        return 2;
    }
    return -1;
}

/* Reveals the winner line sprites reached by the entrants; returns the last one that was hidden. */
static s16 fn_1_1E6A8(void)
{
    s16 ret;
    s16 i;
    s16 spr;

    ret = -1;
    for (i = 0; i < 8; i++) {
        if (TNM_STAGE(i) != 0) {
            if (HuSprAttrGet(lbl_1_bss_C32, i + 7) & 4) {
                ret = i + 7;
            }
            HuSprAttrReset(lbl_1_bss_C32, i + 7, 4);
        }
        if (TNM_STAGE(i) > 1) {
            spr = (i / 2) + 3;
            if (HuSprAttrGet(lbl_1_bss_C32, spr) & 4) {
                ret = spr;
            }
            HuSprAttrReset(lbl_1_bss_C32, spr, 4);
        }
        if (TNM_STAGE(i) > 2) {
            spr = (i / 4) + 1;
            if (HuSprAttrGet(lbl_1_bss_C32, spr) & 4) {
                ret = spr;
            }
            HuSprAttrReset(lbl_1_bss_C32, spr, 4);
        }
    }
    return ret;
}

/* Animates the result of a match: line fades in, loser drops, winner climbs the bracket. */
static void fn_1_1E880(s16 winner, s16 loser, s16 spr)
{
    f32 t;
    f32 x0;
    f32 y0;
    f32 x1;
    f32 y1;
    s16 i;
    s16 round;
    s32 se;
    s32 from;
    s32 to;

    HuAudFXPlay(0x51A);
    for (i = 1; i <= 20; i++) {
        HuSprTPLvlSet(lbl_1_bss_C32, spr, HuSin(90.0 * ((f64)i / 20.0)));
        HuPrcVSleep();
    }
    for (i = 1; i <= 15; i++) {
        t = 24.0 * (1.0 - ((f64)i / 15.0));
        TnmSprPosSet(loser, TNM_STAGE(loser), t);
        HuPrcVSleep();
    }
    se = HuAudFXPlay(0x51B);
    round = TNM_STAGE(winner) - 1;
    for (i = 1; i <= 15; i++) {
        t = (f64)i / 15.0;
        if (round == 0) {
            from = winner;
            to = winner + 8;
        } else if (round == 1) {
            from = (winner / 2) + 0x10;
            to = (winner / 2) + 0x14;
        } else {
            from = (winner / 4) + 0x18;
            to = (winner / 4) + 0x1A;
        }
        x0 = lbl_1_data_1C24[from].x;
        y0 = lbl_1_data_1C24[from].y - 16.0f;
        x1 = lbl_1_data_1C24[to].x;
        y1 = lbl_1_data_1C24[to].y;
        HuSprPosSet(lbl_1_bss_C32, winner + 0xF, x0 + (t * (x1 - x0)), y0 + (t * (y1 - y0)));
        HuPrcVSleep();
    }
    for (i = 1; i <= 15; i++) {
        t = (f64)i / 15.0;
        if (round == 0) {
            from = winner + 8;
            to = (winner / 2) + 0x10;
        } else if (round == 1) {
            from = (winner / 2) + 0x14;
            to = (winner / 4) + 0x18;
        } else {
            from = (winner / 4) + 0x1A;
            to = 0x1C;
        }
        x0 = lbl_1_data_1C24[from].x;
        y0 = lbl_1_data_1C24[from].y;
        x1 = lbl_1_data_1C24[to].x;
        y1 = lbl_1_data_1C24[to].y;
        HuSprPosSet(lbl_1_bss_C32, winner + 0xF, x0 + (t * (x1 - x0)), y0 + (t * (y1 - y0)));
        HuPrcVSleep();
    }
    if (round == 2) {
        for (i = 1; i <= 5; i++) {
            t = (f64)i / 5.0;
            x0 = lbl_1_data_1C24[(winner / 4) + 0x1C].x;
            y0 = lbl_1_data_1C24[(winner / 4) + 0x1C].y;
            HuSprPosSet(lbl_1_bss_C32, winner + 0xF, x0 + (t * (lbl_1_data_1C24[0x1D].x - x0)),
                y0 + (t * (lbl_1_data_1C24[0x1D].y - y0)));
            HuPrcVSleep();
        }
    }
    HuAudFXStop(se);
}

/* Builds the 8 entrants: the player in slot 0, opponents drawn from the per-difficulty pools. */
static void fn_1_1F21C(void)
{
    s16 tmp[6];
    s16 *a;
    s16 *b;
    s16 *c;
    s16 *cnt;
    s16 player;
    s16 i;
    s16 j;
    s16 *pool[3];

    TnmPoolReset();
    player = GwSdCommon.machine[sdWorkInt[7]].charNo;
    TNM_ENTRY(0) = player;
    for (i = 1; i < 8; i++) {
        TNM_ENTRY(i) = 0x2B;
    }
    TnmPoolGet(&a, &b, &c, &cnt);
    pool[0] = a;
    pool[1] = b;
    pool[2] = c;
    /* the player's own machine can not be an opponent */
    for (j = 0; j < 3; j++) {
        for (i = 0; i < cnt[j]; i++) {
            if (player == lbl_1_data_1D14[pool[j][i] - 0x20]) {
                s16 k;

                for (k = i; k < cnt[j]; k++) {
                    pool[j][k] = pool[j][k + 1];
                }
                cnt[j]--;
                break;
            }
        }
    }
    if (sdWorkInt[6] == 0) {
        fn_1_1F78C(c, cnt[2]);
        TNM_ENTRY(4 + frandmod(2)) = c[0];
        TNM_ENTRY(6 + frandmod(2)) = c[1];
    } else if (sdWorkInt[6] == 1) {
        fn_1_1F78C(b, cnt[1]);
        fn_1_1F78C(c, cnt[2]);
        if (b[0] == 0x20 && !GwSdCommon.extCharSelF) {
            tmp[0] = b[1];
        } else {
            tmp[0] = b[0];
        }
        tmp[1] = c[0];
        tmp[2] = c[1];
        fn_1_1F78C(tmp, 3);
        TNM_ENTRY(2 + frandmod(2)) = tmp[0];
        TNM_ENTRY(4 + frandmod(2)) = tmp[1];
        TNM_ENTRY(6 + frandmod(2)) = tmp[2];
    } else {
        fn_1_1F78C(a, cnt[0]);
        fn_1_1F78C(b, cnt[1]);
        fn_1_1F78C(c, cnt[2]);
        TNM_ENTRY(1) = c[0];
        tmp[0] = a[0];
        tmp[1] = b[0];
        tmp[2] = b[1];
        if (cnt[0] >= 2 && cnt[1] >= 3) {
            if (frandmod(2) == 0) {
                tmp[3] = a[1];
            } else {
                tmp[3] = b[2];
            }
        } else if (cnt[0] >= 2) {
            tmp[3] = a[1];
        } else {
            tmp[3] = b[2];
        }
        tmp[4] = c[1];
        tmp[5] = c[2];
        fn_1_1F78C(tmp, 6);
        for (i = 0; i < 6; i++) {
            TNM_ENTRY(2 + i) = tmp[i];
        }
    }
    for (i = 0; i < 8; i++) {
        OSReport("%d:%d\n", i, TNM_ENTRY(i));
    }
}

/* Shuffles arr[0..count). */
static void fn_1_1F78C(s16 *arr, s16 count)
{
    s16 i;
    s16 idx1;
    s16 idx2;
    s16 tmp;

    if (count == 1) {
        return;
    }
    for (i = 0; i < 100; i++) {
        idx1 = frandmod(count);
        idx2 = frandmod(count);
        tmp = arr[idx1];
        arr[idx1] = arr[idx2];
        arr[idx2] = tmp;
    }
}

/* Strength class of an entrant machine: 3 pool A, 2 pool B, 1 other, 0 none. */
static s16 fn_1_1F82C(s16 id)
{
    s16 *a;
    s16 *b;
    s16 *c;
    s16 *cnt;
    s16 i;

    if (id == 0x2B) {
        return 0;
    }
    TnmPoolGet(&a, &b, &c, &cnt);
    for (i = 0; i < cnt[0]; i++) {
        if (id == a[i]) {
            return 3;
        }
    }
    for (i = 0; i < cnt[1]; i++) {
        if (id == b[i]) {
            return 2;
        }
    }
    return 1;
}

/* Final: trophy animation, record the win and the unlock flags. */
static void fn_1_1F960(s16 winner)
{
    HU3DMODEL *trophy;
    f32 s;
    s16 i;
    s8 padNo;
    s32 allBefore;

    trophy = &Hu3DData[lbl_1_bss_C20];
    padNo = GwSdBattle.padNo[0];
    Hu3DModelAttrReset(lbl_1_bss_C22[5], 1);
    Hu3DModelPosSet(lbl_1_bss_C22[5], trophy->pos.x, trophy->pos.y, trophy->pos.z - 50.0f);
    if (lbl_1_bss_C1C != NULL) {
        TNM_PROP_SET(lbl_1_bss_C1C, 1);
    }
    HuAudFXPlay(0x51E);
    for (i = 1; i <= 10; i++) {
        s = 4.0 * HuSin(90.0 * ((f64)i / 10.0));
        Hu3DModelScaleSet(lbl_1_bss_C22[5], s, s, s);
        HuPrcVSleep();
    }
    for (i = 1; i <= 5; i++) {
        s = 2.0 + (2.0 * HuCos(90.0 * ((f64)i / 5.0)));
        Hu3DModelScaleSet(lbl_1_bss_C22[5], s, s, s);
        HuPrcVSleep();
    }
    if (winner == 0) {
        allBefore = GwSdCommon.battleEasyWinF && GwSdCommon.battleNormalWinF && GwSdCommon.battleHardWinF
            && GwSdCommon.flagEasyWinF && GwSdCommon.flagNormalWinF && GwSdCommon.flagHardWinF
            && GwSdCommon.rabbitEasyWinF && GwSdCommon.rabbitNormalWinF && GwSdCommon.rabbitHardWinF;
        if (sdWorkInt[3] == 0 && sdWorkInt[6] == 0) {
            GwSdCommon.battleEasyWinF = 1;
        }
        if (sdWorkInt[3] == 0 && sdWorkInt[6] == 1) {
            GwSdCommon.battleNormalWinF = 1;
        }
        if (sdWorkInt[3] == 0 && sdWorkInt[6] == 2) {
            GwSdCommon.battleHardWinF = 1;
        }
        if (sdWorkInt[3] == 1 && sdWorkInt[6] == 0) {
            GwSdCommon.flagEasyWinF = 1;
        }
        if (sdWorkInt[3] == 1 && sdWorkInt[6] == 1) {
            GwSdCommon.flagNormalWinF = 1;
        }
        if (sdWorkInt[3] == 1 && sdWorkInt[6] == 2) {
            GwSdCommon.flagHardWinF = 1;
        }
        if (sdWorkInt[3] == 2 && sdWorkInt[6] == 0) {
            GwSdCommon.rabbitEasyWinF = 1;
        }
        if (sdWorkInt[3] == 2 && sdWorkInt[6] == 1) {
            GwSdCommon.rabbitNormalWinF = 1;
        }
        if (sdWorkInt[3] == 2 && sdWorkInt[6] == 2) {
            GwSdCommon.rabbitHardWinF = 1;
        }
        if (GwSdCommon.battleHardWinF) {
            GwCommon.unkSdFlag = 1;
        }
        if (GwSdCommon.flagHardWinF) {
            GwCommon.unkSdFlag2 = 1;
        }
        if (GwSdCommon.battleEasyWinF && GwSdCommon.battleNormalWinF && GwSdCommon.battleHardWinF
            && GwSdCommon.flagEasyWinF && GwSdCommon.flagNormalWinF && GwSdCommon.flagHardWinF
            && GwSdCommon.rabbitEasyWinF && GwSdCommon.rabbitNormalWinF && GwSdCommon.rabbitHardWinF && !allBefore) {
            GwSdCommon.newPart2F = 1;
        }
    }
    HuWinMesSet(lbl_1_bss_C2E[1], 0x2000B);
    HuWinDispOn(lbl_1_bss_C2E[1]);
    GwSdCommon.picNo++;
    if (GwSdCommon.picNo > 999) {
        GwSdCommon.picNo = 0;
    }
    while (!(fn_1_4FD0(1 << padNo) & 0x200)) {
        HuPrcVSleep();
    }
}

/* Decides a match between two CPU entrants by their strength classes. */
static s16 fn_1_20038(s16 *pair)
{
    s16 stat[2];
    s16 k;
    s16 r;

    for (k = 0; k < 2; k++) {
        stat[k] = fn_1_1F82C(TNM_ENTRY(pair[k]));
    }
    switch (stat[0] - stat[1]) {
        case 0:
            return frandmod(2);
        case 1:
            r = (frandmod(10) < 7) ? 0 : 1;
            return r;
        case 2:
            r = (frandmod(10) < 9) ? 0 : 1;
            return r;
        case 3:
            r = (frandmod(100) < 95) ? 0 : 1;
            return r;
        case -1:
            r = (frandmod(10) < 7) ? 1 : 0;
            return r;
        case -2:
            r = (frandmod(10) < 9) ? 1 : 0;
            return r;
        case -3:
            r = (frandmod(100) < 95) ? 1 : 0;
            return r;
        default:
            return -1;
    }
}

/* Entrant viewer: left/right moves the cursor, A shows the machine info. */
static void fn_1_20300(void)
{
    HUPROCESS *pulse;
    s8 padNo;
    s16 i;
    s16 move;
    s16 win;
    f32 t;
    f32 s;

    padNo = GwSdBattle.padNo[0];
    lbl_1_bss_C02 = 1;
    for (i = 0; i < 8; i++) {
        HuSprScaleSet(lbl_1_bss_C32, i + 0xF, 0.8f, 0.8f);
    }
    lbl_1_bss_C04 = 1.0f;
    pulse = HuPrcChildCreate(fn_1_20760, 0x5DC, 0x2000, 0, HuPrcCurrentGet());
    lbl_1_bss_C00 = 0;
    HuWinMesSet(lbl_1_bss_C2E[1], 0x20004);
    while (1) {
        move = 0;
        if ((fn_1_507C(1 << padNo) & 1) && lbl_1_bss_C02 > 1) {
            move = -1;
        } else if ((fn_1_507C(1 << padNo) & 2) && lbl_1_bss_C02 < 7) {
            move = 1;
        }
        if (move != 0) {
            HuAudFXPlay(0);
            lbl_1_bss_C00 = 1;
            for (i = 1; i <= 10; i++) {
                t = lbl_1_bss_C04 - 0.8;
                s = 0.8 + (t * (1.0 - ((f64)i / 10.0)));
                HuSprScaleSet(lbl_1_bss_C32, lbl_1_bss_C02 + 0xF, s, s);
                s = 0.8 + (0.2 * ((f64)i / 10.0));
                HuSprScaleSet(lbl_1_bss_C32, lbl_1_bss_C02 + move + 0xF, s, s);
                HuPrcVSleep();
            }
            lbl_1_bss_C02 += move;
            lbl_1_bss_C00 = 0;
            HuPrcVSleep();
            continue;
        }
        if (fn_1_4FD0(1 << padNo) & 0x100) {
            HuAudFXPlay(1);
            win = HuWinCreate(-10000.0f, 320.0f, 0x200, 0x5E, 0);
            HuWinMesSpeedSet(win, 0);
            HuWinPriSet(win, 0xA);
            HuWinMesSet(win, TNM_ENTRY(lbl_1_bss_C02) + 0x1DFFE0);
            HuWinMesWait(win);
            HuWinKill(win);
            HuPrcVSleep();
            continue;
        }
        if (!(fn_1_4FD0(1 << padNo) & 0x200)) {
            HuPrcVSleep();
            continue;
        }
        break;
    }
    HuAudFXPlay(3);
    HuPrcKill(pulse);
    for (i = 0; i < 8; i++) {
        HuSprScaleSet(lbl_1_bss_C32, i + 0xF, 1.0f, 1.0f);
    }
}

/* Pulses the cursor sprite. */
static void fn_1_20760(void)
{
    f32 angle = 0.0f;

    while (1) {
        if (lbl_1_bss_C00 == 0) {
            lbl_1_bss_C04 = 0.8 + (0.2 * HuCos(angle));
            HuSprScaleSet(lbl_1_bss_C32, lbl_1_bss_C02 + 0xF, lbl_1_bss_C04, lbl_1_bss_C04);
            angle += 10.0f;
            if (angle > 360.0f) {
                angle -= 360.0f;
            }
        } else {
            angle = 0.0f;
        }
        HuPrcVSleep();
    }
}
