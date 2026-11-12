#include "types.h"
#include "string.h"
#include "game/audio.h"
#include "game/charman.h"
#include "game/esprite.h"
#include "game/flag.h"
#include "game/gamemes.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/object.h"
#include "game/wipe.h"
#include "game/mg/seqman.h"
#include "game/mg/score.h"
#include "game/mg/timer.h"
#include "REL/m552dll.h"

double sin(double x);
double cos(double x);

/* cube.c */
extern s32 lbl_1_data_410[22];
void fn_1_82BC(s32 arg0);
void fn_1_8644(s32 arg0, s32 arg1);
void fn_1_879C(s32 arg0, s32 arg1);
void fn_1_88EC(void);
s32 fn_1_8D4C(s8 color, s32 mode, f32 time);
void fn_1_8F2C(s32 arg0, s32 arg1);
/* com.c */
void fn_1_9238(s32 idx);
/* stage.c */
void fn_1_BADC(void);

/* m552.c */
extern s32 lbl_1_bss_0;
extern OMCAMERAVIEW lbl_1_bss_14[4];
extern M552Obj lbl_1_bss_710[70];
extern M552Obj lbl_1_bss_1200[8];
extern M552Obj lbl_1_bss_1340[0xD0];
extern s16 lbl_1_bss_33C0;
extern s16 lbl_1_bss_33C2[2][200];
extern s16 lbl_1_bss_36E2[2][7];
extern M552Field lbl_1_bss_3700;
extern M552State lbl_1_bss_4794;

void fn_1_3374(s32 arg0);
void fn_1_385C(s32 arg0, s32 arg1);
s32 fn_1_3B98(s32 arg0);
s32 fn_1_41C0(s32 arg0);
void fn_1_4450(s32 arg0, s32 arg1);
void fn_1_4534(s32 arg0);
void fn_1_4C50(s32 arg0);
void fn_1_4DA4(s32 arg0);
s32 fn_1_4E94(s32 arg0);
s32 fn_1_57D4(s32 arg0);
s32 fn_1_5A34(s32 arg0);
void fn_1_5EBC(s32 arg0);
void fn_1_63A0(s32 arg0);
s32 fn_1_6650(s32 arg0);
s32 fn_1_6758(s32 arg0, s32 arg1);
void fn_1_6834(s32 arg0);
s32 fn_1_6920(void);
void fn_1_6B58(s32 arg0, s32 arg1, s32 arg2);
s32 fn_1_6DE0(void);
s32 fn_1_7134(s32 arg0);
s32 fn_1_7400(s32 arg0);
s32 fn_1_7574(s32 arg0);
void fn_1_7800(s32 idx, s32 scrId);

/* unit-owned .bss */
s16 lbl_1_bss_4918;
MGSCORE *lbl_1_bss_4914;
s32 lbl_1_bss_4910;
MGTIMER *lbl_1_bss_490C;
s16 lbl_1_bss_4908;

/* unit-owned .data */
f32 lbl_1_data_360[22] = {
    0.0f, 0.0f, 0.0f, 0.25f, 0.0f, 0.5f, 0.0f, 0.75f, 0.25f, 0.0f, 0.25f,
    0.25f, 0.25f, 0.5f, 0.25f, 0.75f, 0.5f, 0.0f, 0.5f, 0.25f, 0.5f, 0.5f,
};
s32 lbl_1_data_3B8 = -1;
s32 lbl_1_data_3BC = -1;
s32 lbl_1_data_3C0[4] = { 0, 0, 0, 0 };
f32 lbl_1_data_3F4 = 5.0f;
f32 lbl_1_data_3F8[6] = { 0.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.0f };

void fn_1_1C40(void)
{
    s32 i;

    if (lbl_1_bss_4794.timer[0] == 0) {
        switch (lbl_1_bss_0) {
        case 0:
            lbl_1_bss_4910 = GWMgRecordGet(8);
            break;
        case 1:
            lbl_1_bss_4910 = GWMgRecordGet(0xA);
            break;
        case 2:
            lbl_1_bss_4910 = GWMgRecordGet(0xB);
            break;
        }
        lbl_1_bss_4918 = MgScoreWinCreate(0x88, 0x28);
        MgScoreWinPosSet(lbl_1_bss_4918, 288.0f, 420.0f);
        MgScoreWinTPLvlSet(lbl_1_bss_4918, 0.7f);
        lbl_1_bss_4914 = MgScoreInit(0x960030);
        lbl_1_bss_4914->dispLeadZeroF = 1;
        MgScoreMaxDigitSet(lbl_1_bss_4914, 6);
        MgScoreValueSet(lbl_1_bss_4914, lbl_1_bss_4910);
        MgScoreColorSet(lbl_1_bss_4914, 0x42, 0xFF, 0x7A);
        MgScorePosSet(lbl_1_bss_4914, 259.0f, 420.0f);
        espDispOn(lbl_1_bss_33C0);
        espPosSet(lbl_1_bss_33C0, 239.0f, 420.0f);
    }
    for (i = 0; i < 2; i++) {
        lbl_1_bss_4794.models[i].pos.x += 9.0 * cos((3.141592653589793 * (i * 30 + 75)) / 180.0);
        lbl_1_bss_4794.models[i].pos.z -= 9.0 * sin(1.3089969389957472);
    }
    if (lbl_1_bss_4794.timer[0] == 8) {
        omCameraViewMoveSimpleMulti(1, &lbl_1_bss_14[0], 0x98);
        omCameraViewMoveSimpleMulti(2, &lbl_1_bss_14[0], 0x98);
        omCameraViewMoveSimpleMulti(4, &lbl_1_bss_14[0], 0x98);
    }
    lbl_1_bss_4794.timer[0] += 1;
    if (lbl_1_bss_4794.timer[0] >= 0x41) {
        lbl_1_bss_4794.stat[0] += 1;
        lbl_1_bss_4794.stat[0] += 1;
        lbl_1_bss_4794.timer[0] = 0;
        WipeColorSet(0xFF, 0xFF, 0xFF);
        WipeCreate(2, 0, 0x37);
    }
    if (lbl_1_bss_4794.timer[0] == 0x1E) {
        lbl_1_data_3B8 = HuAudBGMPlay(0x3E);
    }
}

void fn_1_1FF4(void)
{
    s32 i;
    s32 t;

    for (i = 0; i < 2; i++) {
        if (lbl_1_bss_4794.models[i].pos.z > -380.0f) {
            lbl_1_bss_4794.models[i].pos.x += 8.0 * cos((3.141592653589793 * (i * 30 + 75)) / 180.0);
            lbl_1_bss_4794.models[i].pos.z -= 8.0 * sin(1.3089969389957472);
        }
    }
    t = lbl_1_bss_4794.timer[0];
    lbl_1_bss_4794.timer[0] = t + 1;
    if (t >= 0xA) {
        lbl_1_bss_4794.stat[0] += 1;
        WipeColorSet(0xFF, 0xFF, 0xFF);
        WipeCreate(2, 0, 0x3C);
        lbl_1_bss_4794.timer[0] = 0;
    }
}

void fn_1_2180(void)
{
    u8 i;

    for (i = 0; i < 2U; i++) {
        if (lbl_1_bss_4794.models[i].pos.z > -400.0f) {
            lbl_1_bss_4794.models[i].pos.x += 9.0 * cos((3.141592653589793 * (i * 30 + 75)) / 180.0);
            lbl_1_bss_4794.models[i].pos.z -= 9.0 * sin(1.3089969389957472);
        }
    }
    if (WipeCheckEnd() == 0) {
        lbl_1_bss_4794.stat[0] += 1;
        for (i = 0; i < 8U; i++) {
            Hu3DModelAttrSet(lbl_1_bss_4794.models[i].id, 1);
        }
        lbl_1_bss_4794.slots[0].motion = 0;
        lbl_1_bss_4794.slots[1].motion = 0;
        for (i = 0; i < 2U; i++) {
            Hu3DModelAttrReset(lbl_1_bss_4794.models[i + 35].id, 1);
        }
        for (i = 0; i < 0xBU; i++) {
            Hu3DModelAttrReset(lbl_1_bss_4794.models[i + 8].id, 1);
            if (i == 0 || i == 1) {
                Hu3DMotionSpeedSet(lbl_1_bss_4794.models[i + 8].id, 2.0f);
                Hu3DMotionTimeSet(lbl_1_bss_4794.models[i + 8].id, 0.0f);
                Hu3DModelAttrSet(lbl_1_bss_4794.models[i + 8].id, 0x40000001);
            }
            if (i == 8 || i == 9) {
                Hu3DMotionSpeedSet(lbl_1_bss_4794.models[i + 8].id, 1.0f);
                Hu3DMotionTimeSet(lbl_1_bss_4794.models[i + 8].id, 0.0f);
                Hu3DModelAttrSet(lbl_1_bss_4794.models[i + 8].id, 0x40000001);
            }
        }
        for (i = 0; i < 0xCU; i++) {
            Hu3DModelAttrReset(lbl_1_bss_4794.models[i + 19].id, 1);
        }
        for (i = 0; i < 4U; i++) {
            Hu3DModelAttrReset(lbl_1_bss_4794.models[i + 31].id, 1);
            Hu3DMotionSpeedSet(lbl_1_bss_4794.models[i + 31].id, 2.0f);
            Hu3DMotionTimeSet(lbl_1_bss_4794.models[i + 31].id, 0.0f);
            Hu3DModelAttrSet(lbl_1_bss_4794.models[i + 31].id, 0x40000001);
        }
        WipeColorSet(0xFF, 0xFF, 0xFF);
        WipeCreate(1, 0, 0x3C);
        omCameraViewMoveSimpleMulti(1, &lbl_1_bss_14[1], 1);
        omCameraViewMoveSimpleMulti(2, &lbl_1_bss_14[1], 1);
        omCameraViewMoveSimpleMulti(4, &lbl_1_bss_14[1], 1);
        lbl_1_bss_3700.plr[0].shown = -1;
        lbl_1_bss_3700.plr[1].shown = -1;
        fn_1_6920();
        Hu3DModelAttrSet(lbl_1_bss_4794.models[18].id, 0x40000001);
        Hu3DMotionSpeedSet(lbl_1_bss_4794.models[18].id, 2.16f);
        lbl_1_data_3BC = HuAudFXPlay(0x869);
    }
}

void fn_1_26BC(void)
{
    u8 end;

    end = WipeCheckEnd();
    if (end == 0) {
        lbl_1_bss_4794.stat[0] = 0;
        MgSeqModeNext();
        if (lbl_1_bss_0 != 0) {
            lbl_1_bss_490C = MgTimerCreate(3);
            if (lbl_1_bss_0 == 1) {
                MgTimerParamSet(lbl_1_bss_490C, 0x2A30, 0, 0);
            } else {
                MgTimerParamSet(lbl_1_bss_490C, 0x4650, 0, 0);
            }
            MgTimerPosSet(lbl_1_bss_490C, 294.0f, 64.0f);
            MgTimerModeOnSet(lbl_1_bss_490C, 0);
        }
    }
}

void fn_1_27B4(void)
{
    switch (lbl_1_bss_0) {
    case 0:
        lbl_1_bss_490C = MgTimerCreate(0);
        MgTimerParamSet(lbl_1_bss_490C, 0xE10, 0, 0);
        MgTimerPosSet(lbl_1_bss_490C, 288.0f, 64.0f);
        MgTimerModeOnSet(lbl_1_bss_490C, 1);
        break;
    case 1:
    case 2:
        MgTimerModeOnSet(lbl_1_bss_490C, 1);
        break;
    }
    lbl_1_bss_4794.stat[0] += 1;
    fn_1_88EC();
}

void fn_1_2890(void)
{
    s32 i;

    fn_1_BADC();
    for (i = 0; i < 2; i++) {
        if (lbl_1_bss_4794.chars[i].cpuLevel != 0) {
            fn_1_9238(i);
        }
        fn_1_3374(i);
    }
    fn_1_88EC();
    if (MgTimerDoneCheck(lbl_1_bss_490C) != 0) {
        lbl_1_bss_4794.stat[0] += 1;
    }
    fn_1_6920();
    fn_1_6DE0();
}

void fn_1_2930(void)
{
    s32 idle;
    s32 scoreDone;
    s32 popups;
    s32 i;

    idle = 0;
    for (i = 0; i < 2; i++) {
        if (lbl_1_bss_4794.stat[i + 2] == 4 || lbl_1_bss_4794.stat[i + 2] == 5 || lbl_1_bss_4794.stat[i + 2] == 6) {
            if (fn_1_5A34(i) != 0) {
                lbl_1_bss_4794.stat[i + 2] = 0;
            }
        } else {
            idle += 1;
        }
    }
    scoreDone = fn_1_6920();
    popups = fn_1_6DE0();
    if (scoreDone != 0 && popups == 0 && idle == 2) {
        lbl_1_bss_4794.stat[0] = 0;
        MgSeqModeNext();
        Hu3DMotionSpeedSet(lbl_1_bss_4794.models[18].id, 0.0f);
        HuAudSStreamFadeOut(lbl_1_data_3B8, 0x3E8);
        HuAudFXFadeOut(lbl_1_data_3BC, 0x14);
        return;
    }
    fn_1_88EC();
}

void fn_1_2AA0(void)
{
    lbl_1_bss_4794.stat[0] = 0;
    lbl_1_bss_4794.stat[1] = 0;
}

void fn_1_2AC4(void)
{
    if (lbl_1_data_3C0[2] == 0) {
        lbl_1_data_3C0[3] += 1;
        MgScoreDigitScaleSet(lbl_1_bss_4914, 1.0f + 0.02f * lbl_1_data_3C0[3], 1.0f + 0.02f * lbl_1_data_3C0[3]);
        espScaleSet(lbl_1_bss_33C0, 1.0f + 0.02f * lbl_1_data_3C0[3], 1.0f + 0.02f * lbl_1_data_3C0[3]);
        if (lbl_1_data_3C0[3] == 0xA) {
            lbl_1_data_3C0[2] = 1;
            lbl_1_data_3C0[3] = 0;
        }
    } else {
        lbl_1_data_3C0[3] += 1;
        MgScoreDigitScaleSet(lbl_1_bss_4914, 1.2f - 0.04f * lbl_1_data_3C0[3], 1.2f - 0.04f * lbl_1_data_3C0[3]);
        espScaleSet(lbl_1_bss_33C0, 1.2f - 0.04f * lbl_1_data_3C0[3], 1.2f - 0.04f * lbl_1_data_3C0[3]);
        if (lbl_1_data_3C0[3] == 5) {
            lbl_1_data_3C0[2] = 0;
            lbl_1_data_3C0[3] = 0;
        }
    }
}

void fn_1_2E48(void)
{
    s16 player;
    s32 win;

    switch (lbl_1_bss_4794.stat[1]) {
    case 0:
        espDispOn(lbl_1_bss_33C0);
        espTPLvlSet(lbl_1_bss_33C0, 1.0f);
        lbl_1_bss_4794.timer[0] = 0;
        if (lbl_1_bss_3700.plr[0].shown == lbl_1_bss_3700.plr[1].shown) {
            lbl_1_bss_4908 = GameMesMgDrawCreate();
            HuAudJinglePlay(0x7B);
            CharFXPlayPan(lbl_1_bss_4794.chars[0].charNo, 0x200, 0x30);
            CharFXPlayPan(lbl_1_bss_4794.chars[1].charNo, 0x200, 0x50);
            fn_1_7800(0, 2);
            fn_1_7800(1, 2);
        } else {
            win = 0;
            if (lbl_1_bss_3700.plr[0].shown < lbl_1_bss_3700.plr[1].shown) {
                win = 1;
            }
            player = lbl_1_bss_4794.chars[win].playerNo;
            if (_CheckFlag(0x1000F) == 0) {
                GwPlayer[player].mgCoinBonus = 0xA;
            }
            if (lbl_1_bss_3700.plr[win].shown > lbl_1_bss_4910 && lbl_1_bss_4794.chars[win].cpuLevel == 0) {
                lbl_1_bss_4908 = GameMesRecordCreate(lbl_1_bss_3700.plr[win].shown);
                switch (lbl_1_bss_0) {
                case 0:
                    GWMgRecordSet(8, lbl_1_bss_3700.plr[win].shown);
                    break;
                case 1:
                    GWMgRecordSet(0xA, lbl_1_bss_3700.plr[win].shown);
                    break;
                case 2:
                    GWMgRecordSet(0xB, lbl_1_bss_3700.plr[win].shown);
                    break;
                }
                MgScoreValueSet(lbl_1_bss_4914, lbl_1_bss_3700.plr[win].shown);
                lbl_1_bss_4794.stat[1] += 1;
                lbl_1_bss_4794.stat[2] = win;
                omAddObjEx(lbl_1_bss_4794.objman, 0x7FDA, 0, 0, -1, (OMOBJFUNC)fn_1_2AC4);
                if (_CheckFlag(0x1000F) == 0) {
                    HuAudJinglePlay(0x86);
                } else {
                    lbl_1_bss_4794.timer[0] = 0x96;
                }
            } else {
                lbl_1_bss_4908 = GameMesMgWinnerCreate(lbl_1_bss_4794.chars[win].charNo, -1, -1, -1);
                HuAudJinglePlay(0x79);
                CharFXPlayPan(lbl_1_bss_4794.chars[win].charNo, 0x1FA, (s16)((win << 5) + 0x30));
            }
            fn_1_7800(win, 1);
            fn_1_7800(win ^ 1, 2);
        }
        lbl_1_bss_4794.stat[1] += 1;
        return;
    case 1:
        lbl_1_bss_4794.timer[0] += 1;
        if (GameMesStatGet(lbl_1_bss_4908) == 0 && lbl_1_bss_4794.timer[0] >= 0xD0) {
            GameMesKill(lbl_1_bss_4908);
            MgSeqModeNext();
            return;
        }
        return;
    case 2:
        lbl_1_bss_4794.timer[0] += 1;
        if (GameMesStatGet(lbl_1_bss_4908) == 0 && lbl_1_bss_4794.timer[0] >= 0x96) {
            GameMesKill(lbl_1_bss_4908);
            lbl_1_bss_4908 = GameMesMgWinnerCreate(lbl_1_bss_4794.chars[lbl_1_bss_4794.stat[2]].charNo, -1, -1, -1);
            lbl_1_bss_4794.stat[1] -= 1;
            lbl_1_bss_4794.timer[0] = 0;
            HuAudJinglePlay(0x79);
            CharFXPlayPan(lbl_1_bss_4794.chars[lbl_1_bss_4794.stat[2]].charNo, 0x1FA,
                          (s16)((lbl_1_bss_4794.stat[2] << 5) + 0x30));
        }
        break;
    }
}

void fn_1_3374(s32 arg0)
{
    M552Plr *plr = &lbl_1_bss_3700.plr[arg0];

    if (lbl_1_bss_4794.timer[arg0 + 2] != 0) {
        fn_1_4534(arg0);
    }
    switch (lbl_1_bss_4794.stat[arg0 + 2]) {
    case 0:
        fn_1_385C(arg0, 0);
        if (fn_1_3B98(arg0) != 0) {
            lbl_1_bss_4794.stat[arg0 + 2] = 1;
        }
        plr->unk244 = 0;
        plr->unk248 = 0;
        break;
    case 1:
        fn_1_385C(arg0, 1);
        if (fn_1_41C0(arg0) != 0) {
            lbl_1_bss_4794.stat[arg0 + 2] = 2;
        }
        break;
    case 2:
        lbl_1_bss_3700.plr[arg0].timer += 1;
        fn_1_385C(arg0, 2);
        if (fn_1_4E94(arg0) != 0 && lbl_1_bss_4794.timer[arg0 + 2] == 0) {
            lbl_1_bss_4794.stat[arg0 + 2] = 3;
        }
        if (lbl_1_bss_4794.timer[arg0 + 2] == 0) {
            if (fn_1_7134(arg0) != 0) {
                lbl_1_bss_4794.stat[arg0 + 2] = 7;
            }
        } else {
            fn_1_4DA4(arg0);
        }
        if (lbl_1_bss_4794.stat[arg0 + 2] == 2) {
            fn_1_4C50(arg0);
            if ((lbl_1_bss_4794.chars[arg0].btnDown & 0x100) != 0) {
                if (arg0 != 0) {
                    HuAudFXPlay(0x86F);
                } else {
                    HuAudFXPlay(0x86E);
                }
            }
        } else {
            fn_1_8644(arg0, 1);
        }
        break;
    case 3:
        if (fn_1_57D4(arg0) != 0) {
            if (fn_1_6650(arg0) != 0) {
                lbl_1_bss_4794.stat[arg0 + 2] = 4;
            } else {
                lbl_1_bss_4794.stat[arg0 + 2] = 0;
            }
        }
        break;
    case 4:
        fn_1_385C(arg0, 0);
        if (fn_1_5A34(arg0) != 0) {
            lbl_1_bss_4794.stat[arg0 + 2] = 0;
        }
        if (fn_1_3B98(arg0) != 0) {
            if (lbl_1_bss_4794.stat[arg0 + 2] == 0) {
                lbl_1_bss_4794.stat[arg0 + 2] = 1;
            } else {
                lbl_1_bss_4794.stat[arg0 + 2] = 5;
            }
        }
        break;
    case 5:
        fn_1_385C(arg0, 1);
        if (fn_1_5A34(arg0) != 0) {
            lbl_1_bss_4794.stat[arg0 + 2] = 1;
        }
        if (fn_1_41C0(arg0) != 0) {
            if (lbl_1_bss_4794.stat[arg0 + 2] == 1) {
                lbl_1_bss_4794.stat[arg0 + 2] = 2;
            } else {
                lbl_1_bss_4794.stat[arg0 + 2] = 6;
            }
        }
        break;
    case 6:
        lbl_1_bss_3700.plr[arg0].timer += 1;
        fn_1_385C(arg0, 2);
        if (fn_1_5A34(arg0) != 0) {
            lbl_1_bss_4794.stat[arg0 + 2] = 2;
        }
        break;
    case 7:
        if (fn_1_7400(arg0) != 0) {
            lbl_1_bss_4794.stat[arg0 + 2] = 8;
        }
        break;
    case 8:
        fn_1_385C(arg0, 0);
        if (fn_1_7574(arg0) != 0) {
            lbl_1_bss_4794.stat[arg0 + 2] = 0;
        }
        break;
    }
    fn_1_6834(arg0);
}

void fn_1_385C(s32 arg0, s32 arg1)
{
    f32 speed[5] = { 11.5f, 10.5f, 11.5f, 12.8f, 13.5f };
    M552Plr *plr = &lbl_1_bss_3700.plr[arg0];

    if (lbl_1_bss_4794.chars[arg0].stkX > 0) {
        plr->x += speed[lbl_1_bss_4794.chars[arg0].cpuLevel];
    }
    if (lbl_1_bss_4794.chars[arg0].stkX < 0) {
        plr->x -= speed[lbl_1_bss_4794.chars[arg0].cpuLevel];
    }
    if (lbl_1_bss_4794.chars[arg0].stkY > 0) {
        plr->y += speed[lbl_1_bss_4794.chars[arg0].cpuLevel];
    }
    if (lbl_1_bss_4794.chars[arg0].stkY < 0) {
        plr->y -= speed[lbl_1_bss_4794.chars[arg0].cpuLevel];
    }
    if (plr->y > 300.0f) {
        plr->y = 300.0f;
    }
    if (plr->y < -525.0f) {
        plr->y = -525.0f;
    }
    if (arg0 != 0) {
        if (plr->x > 750.0f) {
            plr->x = 750.0f;
        }
        if (plr->x < -150.0f) {
            plr->x = -150.0f;
        }
    } else {
        if (plr->x > 150.0f) {
            plr->x = 150.0f;
        }
        if (plr->x < -750.0f) {
            plr->x = -750.0f;
        }
    }
    if (arg1 == 2) {
        plr->piece.x = plr->x;
        plr->piece.y = plr->y;
    }
    if (arg1 != 0) {
        if ((lbl_1_bss_4794.chars[arg0].btnDown & 0x40) != 0 && plr->piece.alive != 0) {
            fn_1_4450(arg0, 0);
        }
        if ((lbl_1_bss_4794.chars[arg0].btnDown & 0x20) != 0 && plr->piece.alive != 0) {
            fn_1_4450(arg0, 1);
        }
    }
}

s32 fn_1_3B98(s32 arg0)
{
    f32 hi;
    f32 lo;
    f32 time;
    f32 dist;
    s32 w;
    s32 h;
    s32 i;
    s32 k;

    time = -1.0f;
    if ((lbl_1_bss_4794.chars[arg0].btnDown & 0x100) != 0) {
        for (i = 0; i < 20; i++) {
            if (lbl_1_bss_3700.item[i].alive != 0) {
                if (lbl_1_bss_3700.item[i].rot == 0.0f || lbl_1_bss_3700.item[i].rot == 180.0f) {
                    w = lbl_1_data_410[lbl_1_bss_3700.item[i].shape * 2];
                    h = lbl_1_data_410[lbl_1_bss_3700.item[i].shape * 2 + 1];
                } else {
                    w = lbl_1_data_410[lbl_1_bss_3700.item[i].shape * 2 + 1];
                    h = lbl_1_data_410[lbl_1_bss_3700.item[i].shape * 2];
                }
                hi = 15.0f + (lbl_1_bss_3700.item[i].x + (100.0f * (f32)w * 0.5f));
                lo = (lbl_1_bss_3700.item[i].x - (100.0f * (f32)w * 0.5f)) - 15.0f;
                if (arg0 == 0) {
                    hi += 40.0f;
                } else {
                    lo -= 40.0f;
                }
                if (!(lbl_1_bss_3700.plr[arg0].x > hi) && !(lbl_1_bss_3700.plr[arg0].x < lo)) {
                    if ((lbl_1_bss_3700.item[i].y - lbl_1_bss_3700.plr[arg0].y) >= 0.0f) {
                        dist = lbl_1_bss_3700.item[i].y - lbl_1_bss_3700.plr[arg0].y;
                    } else {
                        dist = -(lbl_1_bss_3700.item[i].y - lbl_1_bss_3700.plr[arg0].y);
                    }
                    if (!(dist > (f32)((h * 100) / 2 + 0x19))) {
                        lbl_1_bss_4794.moveVec[arg0][0] = lbl_1_bss_3700.item[i].x - lbl_1_bss_3700.plr[arg0].x;
                        lbl_1_bss_4794.moveVec[arg0][1] = lbl_1_bss_3700.item[i].y - lbl_1_bss_3700.plr[arg0].y;
                        memcpy(&lbl_1_bss_3700.plr[arg0].piece, &lbl_1_bss_3700.item[i], sizeof(M552Piece));
                        for (k = 0; k < 25; k++) {
                            if (lbl_1_bss_3700.plr[arg0].piece.grid[k] != 0) {
                                if (time < 0.0f) {
                                    time = Hu3DMotionTimeGet((s16)lbl_1_bss_3700.item[i].cube[k]);
                                }
                                lbl_1_bss_3700.plr[arg0].piece.cube[k] =
                                    fn_1_8D4C(lbl_1_bss_3700.plr[arg0].piece.grid[k], 1, time);
                            }
                        }
                        fn_1_8F2C(arg0, 1);
                        for (k = 0; k < 25; k++) {
                            if (lbl_1_bss_3700.plr[arg0].piece.grid[k] != 0) {
                                M552Obj *obj = &lbl_1_bss_1340[lbl_1_bss_3700.plr[arg0].piece.cube[k]];

                                Hu3DModelPosSetV(obj->model, &obj->pos);
                            }
                        }
                        fn_1_879C(i, 0);
                        fn_1_82BC(arg0);
                        lbl_1_bss_4794.timer[arg0] = 8;
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

s32 fn_1_41C0(s32 arg0)
{
    lbl_1_bss_4794.timer[arg0] -= 1;
    lbl_1_bss_3700.plr[arg0].piece.x =
        lbl_1_bss_3700.plr[arg0].x + (lbl_1_bss_4794.moveVec[arg0][0] * (f32)lbl_1_bss_4794.timer[arg0] * 0.125f);
    lbl_1_bss_3700.plr[arg0].piece.y =
        lbl_1_bss_3700.plr[arg0].y + (lbl_1_bss_4794.moveVec[arg0][1] * (f32)lbl_1_bss_4794.timer[arg0] * 0.125f);
    if (arg0 == 0) {
        lbl_1_bss_3700.plr[arg0].piece.z += 15.0f;
    } else {
        lbl_1_bss_3700.plr[arg0].piece.z += 14.65;
    }
    Hu3DMotionTimeSet(lbl_1_bss_4794.models[arg0 + 12].id, (f32)((5 - lbl_1_bss_4794.timer[arg0]) * 10));
    if (lbl_1_bss_4794.timer[arg0] == 0) {
        lbl_1_bss_3700.plr[arg0].timer = 0;
        return 1;
    }
    if (lbl_1_bss_4794.timer[arg0] == 7) {
        if (arg0 != 0) {
            HuAudFXPlay(0x86B);
        } else {
            HuAudFXPlay(0x86A);
        }
    }
    return 0;
}

void fn_1_4450(s32 arg0, s32 arg1)
{
    if (lbl_1_bss_4794.timer[arg0 + 2] == 0) {
        lbl_1_bss_4794.timer[arg0 + 2] = 0xF;
        lbl_1_bss_4794.moveVec[arg0][2] = (0x5A - (arg1 * 0xB4)) / 15;
        if (arg0 != 0) {
            HuAudFXPlay(0x877);
            return;
        }
        HuAudFXPlay(0x876);
    }
}

void fn_1_4534(s32 arg0)
{
    lbl_1_bss_3700.plr[arg0].piece.rot += lbl_1_bss_4794.moveVec[arg0][2];
    lbl_1_bss_4794.timer[arg0 + 2] -= 1;
    if (lbl_1_bss_3700.plr[arg0].piece.rot >= 360.0f) {
        lbl_1_bss_3700.plr[arg0].piece.rot -= 360.0f;
    }
    if (lbl_1_bss_3700.plr[arg0].piece.rot < 0.0f) {
        lbl_1_bss_3700.plr[arg0].piece.rot += 360.0f;
    }
}

s32 fn_1_462C(s32 arg0, s32 arg1)
{
    f32 f;
    s32 col;
    s32 row;

    if (arg1 == 0) {
        if (lbl_1_bss_3700.plr[arg0].x < (-800.0f + (1100.0f * (f32)arg0))) {
            return -1;
        }
        if (lbl_1_bss_3700.plr[arg0].x > (-300.0f + (1100.0f * (f32)arg0))) {
            return -1;
        }
        if (lbl_1_bss_3700.plr[arg0].y < -400.0f) {
            return -1;
        }
        if (lbl_1_bss_3700.plr[arg0].y > 300.0f) {
            return -1;
        }
        f = -700.0f + (1100.0f * (f32)arg0);
        col = 0;
        while (col < 4) {
            if (lbl_1_bss_3700.plr[arg0].x < f) {
                break;
            }
            col += 1;
            f += 100.0f;
        }
        f = 200.0f;
        row = 0;
        while (row < 6) {
            if (lbl_1_bss_3700.plr[arg0].y > f) {
                break;
            }
            row += 1;
            f -= 100.0f;
        }
        return col + (row * 5);
    }
    if (lbl_1_bss_3700.plr[arg0].x < (-780.0f + (1100.0f * (f32)arg0))) {
        return -1;
    }
    if (lbl_1_bss_3700.plr[arg0].x > (-320.0f + (1100.0f * (f32)arg0))) {
        return -1;
    }
    if (lbl_1_bss_3700.plr[arg0].y < -390.0f) {
        return -1;
    }
    if (lbl_1_bss_3700.plr[arg0].y > 290.0f) {
        return -1;
    }
    f = -650.0f + (1100.0f * (f32)arg0);
    col = 0;
    while (col < 3) {
        if (lbl_1_bss_3700.plr[arg0].x < f) {
            break;
        }
        col += 1;
        f += 100.0f;
    }
    f = 150.0f;
    row = 0;
    while (row < 5) {
        if (lbl_1_bss_3700.plr[arg0].y > f) {
            break;
        }
        row += 1;
        f -= 100.0f;
    }
    return col + (row * 4);
}

s32 fn_1_4A78(s32 arg0, s32 arg1, s32 arg2)
{
    s32 lim[2] = { 4, 6 };
    s32 ret = -1;
    s32 lo;
    s32 hi;

    if (arg2 % 2 != 0) {
        if (arg0 != 0) {
            lo = (arg1 / 5) - (arg2 / 2);
            hi = (arg1 / 5) + (arg2 / 2);
        } else {
            lo = (arg1 % 5) - (arg2 / 2);
            hi = (arg1 % 5) + (arg2 / 2);
        }
    } else {
        if (arg1 < 0) {
            return -1;
        }
        if (arg0 != 0) {
            lo = ((arg1 / 4) - (arg2 / 2)) + 1;
            hi = (arg1 / 4) + (arg2 / 2);
        } else {
            lo = ((arg1 % 4) - (arg2 / 2)) + 1;
            hi = (arg1 % 4) + (arg2 / 2);
        }
    }
    if (lo >= 0 && lim[arg0] >= hi) {
        ret = lo;
    }
    return ret;
}

void fn_1_4C50(s32 arg0)
{
    M552Plr *plr = &lbl_1_bss_3700.plr[arg0];
    s32 state;
    s32 i;

    if (plr->unk244 != plr->unk248) {
        plr->unk248 = plr->unk244;
        plr->unk24C = 3;
        if (plr->unk244 == 1) {
            fn_1_8644(arg0, 0);
            state = 3;
        } else {
            fn_1_8644(arg0, 1);
            state = 1;
        }
        for (i = 0; i < 25; i++) {
            if (lbl_1_bss_3700.plr[arg0].piece.grid[i] != 0) {
                lbl_1_bss_1340[lbl_1_bss_3700.plr[arg0].piece.cube[i]].state = state;
            }
        }
        return;
    }
    if (plr->unk244 == 1) {
        plr->unk24C += 1;
        if (plr->unk24C >= 6) {
            plr->unk24C = 0;
            plr->unk250 = plr->unk250 ^ 1;
            fn_1_8644(arg0, plr->unk250);
        }
    }
}

void fn_1_4DA4(s32 arg0)
{
    s32 unused = 0;
    M552Plr *plr = &lbl_1_bss_3700.plr[arg0];

    if (lbl_1_bss_3700.plr[arg0].x < (f32)((arg0 * 0x4B0) - 0x320)
        || lbl_1_bss_3700.plr[arg0].x > (f32)((arg0 * 0x4B0) - 0x190)) {
        return;
    }
    if (plr->y < -625.0f || plr->y > -425.0f) {
        return;
    }
    lbl_1_bss_3700.plr[arg0].unk244 = 1;
}

s32 fn_1_4E94(s32 arg0)
{
    s32 cellA;
    s32 cellB;
    s32 w;
    s32 h;
    s32 x0;
    s32 y0;
    s32 xx;
    s32 yy;
    s32 cx;
    s32 cy;

    lbl_1_bss_3700.plr[arg0].unk244 = 0;
    cellA = fn_1_462C(arg0, 0);
    if (cellA < 0) {
        return 0;
    }
    cellB = fn_1_462C(arg0, 1);
    if (lbl_1_bss_3700.plr[arg0].piece.rot == 0.0f || lbl_1_bss_3700.plr[arg0].piece.rot == 180.0f) {
        w = lbl_1_data_410[lbl_1_bss_3700.plr[arg0].piece.shape * 2];
        h = lbl_1_data_410[lbl_1_bss_3700.plr[arg0].piece.shape * 2 + 1];
    } else {
        w = lbl_1_data_410[lbl_1_bss_3700.plr[arg0].piece.shape * 2 + 1];
        h = lbl_1_data_410[lbl_1_bss_3700.plr[arg0].piece.shape * 2];
    }
    if (w % 2 != 0) {
        x0 = fn_1_4A78(0, cellA, w);
    } else {
        x0 = fn_1_4A78(0, cellB, w);
    }
    if (x0 == -1) {
        return 0;
    }
    if (h % 2 != 0) {
        y0 = fn_1_4A78(1, cellA, h);
    } else {
        y0 = fn_1_4A78(1, cellB, h);
    }
    if (y0 == -1) {
        return 0;
    }
    for (yy = 0; yy < h; yy++) {
        for (xx = 0; xx < w; xx++) {
            if (lbl_1_bss_3700.plr[arg0].board[x0 + (xx + ((y0 + yy) * 5))] != 0) {
                return 0;
            }
        }
    }
    cx = x0 + (w / 2);
    cy = y0 + (h / 2);
    if (w % 2 != 0) {
        lbl_1_bss_4794.moveVec[arg0][0] = (f32)(((arg0 * 0x44C) + (cx * 0x64)) - 0x2EE) - lbl_1_bss_3700.plr[arg0].x;
    } else {
        lbl_1_bss_4794.moveVec[arg0][0] = (f32)(((arg0 * 0x44C) + (cx * 0x64)) - 0x320) - lbl_1_bss_3700.plr[arg0].x;
    }
    if (h % 2 != 0) {
        lbl_1_bss_4794.moveVec[arg0][1] = (f32)(0xFA - (cy * 0x64)) - lbl_1_bss_3700.plr[arg0].y;
    } else {
        lbl_1_bss_4794.moveVec[arg0][1] = (f32)(0x12C - (cy * 0x64)) - lbl_1_bss_3700.plr[arg0].y;
    }
    lbl_1_bss_4794.timer[arg0] = 5;
    lbl_1_bss_3700.plr[arg0].cellX = cellA;
    lbl_1_bss_3700.plr[arg0].cellY = cellB;
    lbl_1_bss_3700.plr[arg0].unk244 = 1;
    if ((lbl_1_bss_4794.chars[arg0].btnDown & 0x100) == 0) {
        return 0;
    }
    return 1;
}

s32 fn_1_57D4(s32 arg0)
{
    M552Plr *plr = &lbl_1_bss_3700.plr[arg0];
    s32 bonus;

    lbl_1_bss_3700.plr[arg0].x += (lbl_1_bss_4794.moveVec[arg0][0] / 5.0f);
    plr->y += (lbl_1_bss_4794.moveVec[arg0][1] / 5.0f);
    plr->piece.x = lbl_1_bss_3700.plr[arg0].x;
    plr->piece.y = plr->y;
    if (arg0 == 0) {
        plr->piece.z -= 24.0f;
    } else {
        plr->piece.z -= 23.65f;
    }
    lbl_1_bss_4794.timer[arg0] -= 1;
    Hu3DMotionTimeSet(lbl_1_bss_4794.models[arg0 + 12].id,
                      (f32)(((5 - lbl_1_bss_4794.timer[arg0]) * 10) + 0x32));
    if (lbl_1_bss_4794.timer[arg0] == 0) {
        fn_1_879C(arg0, 1);
        Hu3DMotionTimeSet(lbl_1_bss_4794.models[arg0 + 12].id, 0.0f);
        bonus = (plr->timer * 0x19) / 60;
        if (bonus > 0x64) {
            bonus = 0x64;
        }
        plr->score += (0x64 - bonus);
        fn_1_5EBC(arg0);
        memset(&plr->piece, 0, sizeof(M552Piece));
        return 1;
    }
    if (lbl_1_bss_4794.timer[arg0] == 4) {
        if (arg0 != 0) {
            HuAudFXPlay(0x86D);
        } else {
            HuAudFXPlay(0x86C);
        }
    }
    return 0;
}

s32 fn_1_5A34(s32 arg0)
{
    M552Plr *plr = &lbl_1_bss_3700.plr[arg0];
    s32 points;
    s32 i;

    switch (plr->stage) {
    case 0:
        plr->stage += 1;
        plr->timer = 0;
        plr->idx = 0;
    default:
        return 0;
    case 1:
        lbl_1_bss_710[plr->idx + (arg0 * 0x23)].state = 1;
        lbl_1_bss_710[plr->idx + (arg0 * 0x23)].model =
            Hu3DModelLink(lbl_1_bss_1200[arg0 + ((plr->board[plr->idx] - 1) * 2)].model);
        Hu3DModelAttrSet(lbl_1_bss_710[plr->idx + (arg0 * 0x23)].model, 1);
        Hu3DModelCameraSet(lbl_1_bss_710[plr->idx + (arg0 * 0x23)].model, 1);
        lbl_1_bss_710[plr->idx + (arg0 * 0x23)].pos.x = lbl_1_bss_1340[plr->boardCube[plr->idx]].pos.x;
        lbl_1_bss_710[plr->idx + (arg0 * 0x23)].pos.y = lbl_1_bss_1340[plr->boardCube[plr->idx]].pos.y;
        lbl_1_bss_710[plr->idx + (arg0 * 0x23)].pos.z = lbl_1_bss_1340[plr->boardCube[plr->idx]].pos.z;
        plr->idx += 1;
        if (plr->idx == 0x23) {
            plr->stage += 1;
            plr->timer = 0x14;
            plr->idx = 0;
            fn_1_7800(arg0, 1);
            CharFXPlayPan(lbl_1_bss_4794.chars[arg0].charNo, 0x1F8, (s16)((arg0 << 5) + 0x30));
        }
        return 0;
    case 2:
        i = plr->timer;
        plr->timer = i + 1;
        if (i > 0x14) {
            plr->stage += 1;
        }
        return 0;
    case 3:
        if (arg0 != 0) {
            HuAudFXPlay(0x875);
        } else {
            HuAudFXPlay(0x874);
        }
        for (i = 0; i < 0x23; i++) {
            if (plr->board[i] == plr->idx + 1) {
                M552Obj *obj = &lbl_1_bss_1340[plr->boardCube[i]];

                obj->state = 0;
                Hu3DModelAttrSet(obj->model, 1);
                obj->unk8 = 0;
                obj->count = 0;
                plr->board[i] = 0;
                Hu3DModelAttrReset(lbl_1_bss_710[i + (arg0 * 0x23)].model, 1);
                lbl_1_bss_710[i + (arg0 * 0x23)].state = 2;
                lbl_1_bss_710[i + (arg0 * 0x23)].count = 0;
            }
        }
        points = fn_1_6758(arg0, plr->idx);
        fn_1_6B58(arg0, plr->idx, points);
        plr->score += points;
        if (points != 0) {
            omVibrate(lbl_1_bss_4794.chars[arg0].playerNo, 0xF, 0xC, 0);
        }
        plr->idx += 1;
        if (plr->idx > 3) {
            plr->stage = 0;
            plr->timer = 0;
            plr->idx = 0;
            fn_1_7800(arg0, 0);
            return 1;
        }
        if (points != 0) {
            plr->timer = 0;
        } else {
            plr->timer = 0x14;
        }
        plr->stage -= 1;
        return 0;
    }
}

void fn_1_5EBC(s32 arg0)
{
    f32 time = -1.0f;
    M552Plr *plr = &lbl_1_bss_3700.plr[arg0];
    s32 cx;
    s32 cy;
    s32 w;
    s32 h;
    s32 col0;
    s32 row0;
    s32 yy;
    s32 xx;
    s32 idx;
    M552Obj *obj;

    if (plr->piece.rot != 0.0f) {
        fn_1_63A0(arg0);
    }
    cx = plr->cellX;
    cy = plr->cellY;
    if (plr->piece.rot == 0.0f || plr->piece.rot == 180.0f) {
        w = lbl_1_data_410[plr->piece.shape * 2];
        h = lbl_1_data_410[plr->piece.shape * 2 + 1];
    } else {
        w = lbl_1_data_410[plr->piece.shape * 2 + 1];
        h = lbl_1_data_410[plr->piece.shape * 2];
    }
    if (w % 2 != 0) {
        col0 = (cx % 5) - 2;
    } else {
        col0 = (cy % 4) - 2;
    }
    if (h % 2 != 0) {
        row0 = (cx / 5) - 2;
    } else {
        row0 = (cy / 4) - 1;
    }
    for (yy = 0; yy < 5; yy++) {
        if (yy + row0 >= 0) {
            for (xx = 0; xx < 5; xx++) {
                if (xx + col0 >= 0 && plr->piece.grid[xx + (yy * 5)] != 0) {
                    if (time < 0.0f) {
                        time = Hu3DMotionTimeGet((s16)plr->piece.cube[xx + (yy * 5)]);
                    }
                    idx = xx + (col0 + ((yy + row0) * 5));
                    plr->board[idx] = plr->piece.grid[xx + (yy * 5)];
                    plr->boardCube[idx] = fn_1_8D4C(plr->piece.grid[xx + (yy * 5)], 0, time);
                    obj = &lbl_1_bss_1340[plr->boardCube[idx]];
                    Hu3DModelPosSet(obj->model, 50.0f + (((1100.0f * (f32)arg0) + (100.0f * (f32)(xx + col0))) - 800.0f),
                                    250.0f - (100.0f * (f32)(yy + row0)), 0.0f);
                    obj->pos.x = 50.0f + (((1100.0f * (f32)arg0) + (100.0f * (f32)(xx + col0))) - 800.0f);
                    obj->pos.y = 250.0f - (100.0f * (f32)(yy + row0));
                    obj->pos.z = 0.0f;
                }
            }
        }
    }
}

void fn_1_63A0(s32 arg0)
{
    s8 tmp[25];
    s32 idx;
    s32 maxX;
    s32 maxY;
    s32 minX;
    s32 minY;
    s32 y;
    s32 x;

#ifdef TARGET_PC
    idx = 0;
#endif
    for (y = 0; y < 5; y++) {
        for (x = 0; x < 5; x++) {
            if (lbl_1_bss_3700.plr[arg0].piece.rot == 90.0f) {
                idx = (5 - (y + 1)) + (x * 5);
            }
            if (lbl_1_bss_3700.plr[arg0].piece.rot == 270.0f) {
                idx = y + (0x14 - (x * 5));
            }
            if (lbl_1_bss_3700.plr[arg0].piece.rot == 180.0f) {
                idx = 0x18 - (x + (y * 5));
            }
            tmp[x + (y * 5)] = lbl_1_bss_3700.plr[arg0].piece.grid[idx];
        }
    }
    minY = -1;
    minX = -1;
    maxY = 0;
    maxX = 0;
    for (y = 0; y < 5; y++) {
        for (x = 0; x < 5; x++) {
            if (tmp[x + (y * 5)] > 0) {
                if (minX == -1) {
                    minX = x;
                }
                if (minY == -1) {
                    minY = y;
                }
                if (maxX < x) {
                    maxX = x;
                }
                if (maxY < y) {
                    maxY = y;
                }
            }
        }
    }
    if ((2 - minX) > (maxX - 2)) {
        for (y = 0; y < 5; y++) {
            for (x = 0; x < 4; x++) {
                tmp[((y * 5) + 4) - x] = tmp[((y * 5) + 3) - x];
            }
            tmp[y * 5] = 0;
        }
    }
    if ((2 - minY) < (maxY - 2)) {
        for (y = 0; y < 4; y++) {
            for (x = 0; x < 5; x++) {
                tmp[x + (y * 5)] = tmp[x + ((y + 1) * 5)];
            }
        }
        for (x = 0; x < 5; x++) {
            tmp[20 + x] = 0;
        }
    }
    memcpy(lbl_1_bss_3700.plr[arg0].piece.grid, tmp, 0x19);
}

s32 fn_1_6650(s32 arg0)
{
    s32 filled = 0;
    s32 i;
    s32 y;
    s32 x;

    for (i = 0; i < 4; i++) {
        lbl_1_bss_3700.plr[arg0].count[i] = 0;
    }
    for (y = 0; y < 7; y++) {
        for (x = 0; x < 5; x++) {
            if (lbl_1_bss_3700.plr[arg0].board[x + (y * 5)] != 0) {
                lbl_1_bss_3700.plr[arg0].count[lbl_1_bss_3700.plr[arg0].board[x + (y * 5)] - 1] =
                    lbl_1_bss_3700.plr[arg0].count[lbl_1_bss_3700.plr[arg0].board[x + (y * 5)] - 1] + 1;
                filled += 1;
            }
        }
    }
    if (filled == 0x23) {
        return 1;
    }
    return 0;
}

s32 fn_1_6758(s32 arg0, s32 arg1)
{
    s32 ret;
    s8 max = 0;
    M552Plr *plr = &lbl_1_bss_3700.plr[arg0];
    s32 i;

    for (i = 0; i < 4; i++) {
        if (plr->count[i] > max) {
            max = plr->count[i];
        }
    }
    if (max == plr->count[arg1]) {
        ret = (plr->count[arg1] * 100) + ((plr->count[arg1] * 10000) / 35);
    } else {
        ret = plr->count[arg1] * 100;
    }
    return ret;
}

void fn_1_6834(s32 arg0)
{
    M552Plr *plr = &lbl_1_bss_3700.plr[arg0];
    M552Mdl *mdl = &lbl_1_bss_4794.models[arg0 + 12];
    s32 i;

    for (i = 0; i < 3; i++) {
        mdl->pos.x = lbl_1_bss_3700.plr[arg0].x;
        mdl->pos.y = plr->y;
        if (arg0 == 0) {
            mdl->pos.z = plr->z + lbl_1_data_3F4;
        } else {
            mdl->pos.z = plr->z - lbl_1_data_3F4;
        }
        mdl += 2;
    }
    if (plr->piece.alive != 0) {
        fn_1_8F2C(arg0, 1);
    }
}

s32 fn_1_6920(void)
{
    s32 digit[6];
    s32 done = 0;
    s32 i;
    s32 j;
    s32 div;
    s32 sum;
    s32 idx;
    M552Plr *plr;

    for (i = 0; i < 2; i++) {
        plr = &lbl_1_bss_3700.plr[i];
        if (plr->shown < plr->score) {
            if (plr->shown < plr->score - 0x64) {
                if (plr->shown < plr->score - 0x3E8) {
                    plr->shown += 0x14D;
                } else {
                    plr->shown += 0x21;
                }
            } else {
                plr->shown += 3;
            }
            if (plr->shown > plr->score) {
                plr->shown = plr->score;
            }
            div = 100000;
            for (j = 0; j < 6; j++) {
                digit[j] = (plr->shown % (div * 10)) / div;
                div = div / 10;
            }
            sum = 0;
            for (j = 0; j < 6; j++) {
                sum += digit[j];
                if (sum != 0 || j == 5) {
                    idx = (digit[j] * 2) + 1;
                    Hu3DTexScrollPosSet(lbl_1_bss_36E2[i][5 - j], lbl_1_data_360[digit[j] * 2], lbl_1_data_360[idx], 0.0f);
                } else {
                    Hu3DTexScrollPosSet(lbl_1_bss_36E2[i][5 - j], lbl_1_data_360[0x14], lbl_1_data_360[0x15], 0.0f);
                }
            }
        } else {
            done += 1;
        }
    }
    return done;
}

void fn_1_6B58(s32 arg0, s32 arg1, s32 arg2)
{
    M552Popup *pop = &lbl_1_bss_3700.plr[arg0].popup[arg1];
    s32 digit[5];
    s32 j;
    s32 div;
    s32 sum;

    if (pop->active == 0 || arg2 != 0) {
        pop->active = 1;
        pop->value = arg2;
        pop->unk14 = 0;
        pop->timer = 0;
        pop->stage = 0;
        pop->x = (f32)(((arg1 % 2) * 0x28) + (arg0 * 0x140) + 0x28);
        pop->y = (f32)((arg1 * 0x30) + 0xE4);
        div = 10000;
        for (j = 0; j < 5; j++) {
            digit[j] = (pop->value % (div * 10)) / div;
            div = div / 10;
        }
        sum = 0;
        for (j = 0; j < 5; j++) {
            sum += digit[j];
            if (sum != 0) {
                pop->spr[j] = (arg1 * 0x32) + ((j * 0xA) + digit[j]);
                espPosSet(lbl_1_bss_33C2[arg0][pop->spr[j]], pop->x + (f32)(j << 5), pop->y);
                espDispOn(lbl_1_bss_33C2[arg0][pop->spr[j]]);
                espTPLvlSet(lbl_1_bss_33C2[arg0][pop->spr[j]], 0.0f);
            } else {
                pop->spr[j] = -1;
            }
        }
    }
}

s32 fn_1_6DE0(void)
{
    f32 alpha;
    s32 count = 0;
    s32 i;
    s32 j;
    s32 k;
    M552Popup *pop;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 4; j++) {
            pop = &lbl_1_bss_3700.plr[i].popup[j];
            alpha = 1.0f;
            if (pop->active != 0) {
                count += 1;
                switch (pop->stage) {
                case 0:
                    pop->timer += 1;
                    alpha = (f32)pop->timer * 0.125f;
                    pop->y -= 1.5;
                    if (pop->timer == 8) {
                        pop->stage += 1;
                        pop->timer = 0;
                    }
                    break;
                case 1:
                    pop->timer += 1;
                    pop->y -= 1.5;
                    if (pop->timer == 0x20) {
                        pop->stage += 1;
                        pop->timer = 0;
                    }
                    break;
                case 2:
                    pop->timer += 1;
                    pop->y -= 1.5;
                    alpha = (f32)(0x10 - pop->timer) * 0.0625f;
                    if (pop->timer == 0x10) {
                        pop->stage += 1;
                        pop->timer = 0;
                    }
                    break;
                case 3:
                    alpha = 0.0f;
                    pop->active = 0;
                    for (k = 0; k < 5; k++) {
                        espDispOff(lbl_1_bss_33C2[i][pop->spr[k]]);
                        pop->spr[k] = -1;
                    }
                    espDispOn(lbl_1_bss_33C0);
                    break;
                }
                for (k = 0; k < 5; k++) {
                    if (pop->spr[k] >= 0) {
                        espPosSet(lbl_1_bss_33C2[i][pop->spr[k]], pop->x + (f32)(k << 5), pop->y);
                        espTPLvlSet(lbl_1_bss_33C2[i][pop->spr[k]], alpha);
                    }
                }
            }
        }
    }
    return count;
}

s32 fn_1_7134(s32 arg0)
{
    M552Plr *plr = &lbl_1_bss_3700.plr[arg0];

    if (lbl_1_bss_3700.plr[arg0].x < (f32)((arg0 * 0x4B0) - 0x320)
        || lbl_1_bss_3700.plr[arg0].x > (f32)((arg0 * 0x4B0) - 0x190)) {
        return 0;
    }
    if (plr->y < -625.0f || plr->y > -425.0f) {
        return 0;
    }
    lbl_1_bss_3700.plr[arg0].unk244 = 1;
    if ((lbl_1_bss_4794.chars[arg0].btnDown & 0x100) == 0) {
        return 0;
    }
    lbl_1_bss_4794.moveVec[arg0][0] = (f32)((arg0 * 0x4B0) - 0x258) - lbl_1_bss_3700.plr[arg0].x;
    lbl_1_bss_4794.moveVec[arg0][1] = -525.0f - plr->y;
    lbl_1_bss_4794.timer[arg0] = 0xF;
    if (plr->piece.shape >= 2 && plr->piece.shape != 4 && (plr->piece.rot == 0.0f || plr->piece.rot == 180.0f)
        && lbl_1_bss_4794.timer[arg0 + 2] == 0) {
        lbl_1_bss_4794.timer[arg0 + 2] = 0xF;
        lbl_1_bss_4794.moveVec[arg0][2] = (0x5A - (arg0 * 0xB4)) / 15;
        if (arg0 != 0) {
            HuAudFXPlay(0x877);
        } else {
            HuAudFXPlay(0x876);
        }
    }
    return 1;
}

s32 fn_1_7400(s32 arg0)
{
    M552Plr *plr = &lbl_1_bss_3700.plr[arg0];

    lbl_1_bss_3700.plr[arg0].x += (lbl_1_bss_4794.moveVec[arg0][0] / 15.0f);
    plr->y += (lbl_1_bss_4794.moveVec[arg0][1] / 15.0f);
    plr->piece.x = lbl_1_bss_3700.plr[arg0].x;
    plr->piece.y = plr->y;
    lbl_1_bss_4794.timer[arg0] -= 1;
    if (lbl_1_bss_4794.timer[arg0] == 0) {
        Hu3DMotionTimeSet(lbl_1_bss_4794.models[arg0 + 10].id, 0.0f);
        Hu3DMotionSpeedSet(lbl_1_bss_4794.models[arg0 + 10].id, 5.0f);
        plr->timer = 0;
        plr->stage = 0;
        if (arg0 != 0) {
            HuAudFXPlay(0x871);
        } else {
            HuAudFXPlay(0x870);
        }
        return 1;
    }
    return 0;
}

s32 fn_1_7574(s32 arg0)
{
    M552Plr *plr = &lbl_1_bss_3700.plr[arg0];
    s32 t;

    switch (plr->stage) {
    case 0:
        t = plr->timer;
        plr->timer = t + 1;
        if (t <= 5) {
            Hu3DMotionTimeSet(lbl_1_bss_4794.models[arg0 + 12].id, (f32)((plr->timer * 0xA) + 0x32));
        }
        if (plr->timer >= 0xA) {
            Hu3DMotionTimeSet(lbl_1_bss_4794.models[arg0 + 10].id, 50.0f);
            Hu3DMotionSpeedSet(lbl_1_bss_4794.models[arg0 + 10].id, 0.0f);
            plr->timer = 0;
            plr->stage += 1;
            Hu3DMotionTimeSet(lbl_1_bss_4794.models[arg0 + 12].id, 0.0f);
        }
    default:
        return 0;
    case 1:
        plr->z -= 50.0f;
        t = plr->timer;
        plr->timer = t + 1;
        if (t >= 0xA) {
            Hu3DMotionSpeedSet(lbl_1_bss_4794.models[arg0 + 10].id, 5.0f);
            plr->timer = 0;
            plr->stage += 1;
            if (arg0 != 0) {
                HuAudFXPlay(0x873);
            } else {
                HuAudFXPlay(0x872);
            }
        }
        return 0;
    case 2:
        t = plr->timer;
        plr->timer = t + 1;
        if (t >= 0xA) {
            Hu3DMotionTimeSet(lbl_1_bss_4794.models[arg0 + 10].id, 0.0f);
            Hu3DMotionSpeedSet(lbl_1_bss_4794.models[arg0 + 10].id, 0.0f);
            plr->timer = 0;
            plr->stage = 0;
            fn_1_879C(arg0, 1);
            memset(&plr->piece, 0, sizeof(M552Piece));
            return 1;
        }
        return 0;
    }
}

void fn_1_7800(s32 idx, s32 scrId)
{
    Hu3DTexScrollPosSet(lbl_1_bss_36E2[idx][6], lbl_1_data_3F8[scrId * 2], lbl_1_data_3F8[(scrId * 2) + 1], 0.0f);
}
