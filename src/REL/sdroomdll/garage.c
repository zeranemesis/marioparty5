#include "REL/sdroomdll/sdroom.h"
#include "REL/sdroomdll/tournament.h"
#include "game/hsfex.h"
#include "game/sprite.h"
#include "game/window.h"
#include "game/mgdata.h"
#include "game/memory.h"
#include "game/printfunc.h"
#include "game/saveload.h"
#include "game/frand.h"
#include "string.h"
#include "stdint.h"

extern void Hu3DLayerHookReset(s16 layerNo);
extern void HuSprTexLoad(ANIMDATA *anim, s16 bmpNo, s16 texMapId, GXTexWrapMode wrapS, GXTexWrapMode wrapT, GXTexFilter filter);

/* camera view of the machine for each of the two players */
static OMCAMERAVIEW lbl_1_data_10B0[2] = {
    { { -433.0f, 200.0f, -250.0f }, { 0.0f, 60.0f, 0.0f }, 50.0f },
    { { -433.0f, 200.0f, 250.0f }, { 0.0f, 120.0f, 0.0f }, 50.0f }
};
static OMCAMERAVIEW lbl_1_data_10E8 = { { 0.0f, 70.0f, 0.0f }, { -30.0f, 0.0f, 0.0f }, 5300.0f };
static OMCAMERAVIEW lbl_1_data_1104 = { { 0.0f, 120.0f, 0.0f }, { -30.0f, 0.0f, 0.0f }, 7500.0f };
static OMCAMERAVIEW lbl_1_data_1120 = { { 0.0f, 80.0f, 0.0f }, { -30.0f, 0.0f, 0.0f }, 6500.0f };
static OMCAMERAVIEW lbl_1_data_113C = { { 0.0f, 70.0f, 0.0f }, { -30.0f, 0.0f, 0.0f }, 6800.0f };

/* stat graph pentagon: corner offsets and per-corner label nudge */
static HuVec2f lbl_1_data_1174[5] = {
    { 0.0f, -92.0f }, { -88.0f, -28.0f }, { -55.0f, 75.0f }, { 54.0f, 75.0f }, { 88.0f, -28.0f },
};
static HuVec2f lbl_1_data_119C[5] = {
    { 0.0f, 0.0f }, { -12.0f, 0.0f }, { -18.0f, 0.0f }, { 20.0f, 0.0f }, { 10.0f, 0.0f },
};
static s16 lbl_1_data_11C4[10] = { 3, 12, -8, 20, -3, 8, -3, 13, 1, 9 };

/* viewport rects x, y, w, h */
static float lbl_1_data_11FC[8][4] = {
    { -33.777767f, 64.0f, 405.33334f, 304.0f },
    { 328.88892f, 40.0f, 284.44446f, 192.0f },
    { 328.88892f, 202.0f, 284.44446f, 192.0f },
    { -32.0f, 0.0f, 426.6667f, 288.0f },
    { 256.0f, 0.0f, 426.6667f, 288.0f },
    { 24.0f, 64.0f, 256.0f, 304.0f },
    { 296.0f, 64.0f, 256.0f, 144.0f },
    { 296.0f, 226.0f, 256.0f, 144.0f },
};
static HuVec2f lbl_1_data_127C[3] = { { 248.0f, 352.0f }, { 520.0f, 192.0f }, { 520.0f, 354.0f } };
static HuVec2f lbl_1_data_1294[2] = { { 64.0f, 200.0f }, { 512.0f, 200.0f } };
static u32 lbl_1_data_12A4[8] = {
    0x19003B, 0x19003C, 0x19003D, 0x19003E,
    0x19003F, 0x190040, 0x190041, 0x190042,
};
static s32 lbl_1_data_12C4[8] = { 1, 2, 3, 4, 5, 6, 7, 0 };

/* selectable-slot icon positions for the two layouts (x, y pairs) */
static float lbl_1_data_12EC[] = {
    -150.0f, -40.0f,
    -75.0f, -40.0f,
    0.0f, -40.0f,
    75.0f, -40.0f,
    150.0f, -40.0f,
    -150.0f, 40.0f,
    -75.0f, 40.0f,
    0.0f, 40.0f,
    75.0f, 40.0f,
    150.0f, 40.0f
};
static float lbl_1_data_133C[] = {
    -175.0f, -40.0f,
    -105.0f, -40.0f,
    -35.0f, -40.0f,
    35.0f, -40.0f,
    105.0f, -40.0f,
    175.0f, -40.0f,
    -140.0f, 40.0f,
    -70.0f, 40.0f,
    0.0f, 40.0f,
    70.0f, 40.0f,
    140.0f, 40.0f
};
static float *lbl_1_data_1394[2] = { lbl_1_data_12EC, lbl_1_data_133C };

/* cursor move table: [layout * 10 + slot][(dy + 1) * 3 + dx + 1] = slot delta */
static s16 lbl_1_data_139C[21][9] = {
    { 0, 0, 0, 0, 0, 1, 0, 5, 6 },
    { 0, 0, 0, -1, 0, 1, 4, 5, 6 },
    { 0, 0, 0, -1, 0, 1, 4, 5, 6 },
    { 0, 0, 0, -1, 0, 1, 4, 5, 6 },
    { 0, 0, 0, -1, 0, 0, 4, 5, 0 },
    { 0, -5, -4, 0, 0, 1, 0, 0, 0 },
    { -6, -5, -4, -1, 0, 1, 0, 0, 0 },
    { -6, -5, -4, -1, 0, 1, 0, 0, 0 },
    { -6, -5, -4, -1, 0, 1, 0, 0, 0 },
    { -6, -5, 0, -1, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 1, 0, 6, 6 },
    { 0, 0, 0, -1, 0, 1, 5, 6, 6 },
    { 0, 0, 0, -1, 0, 1, 5, 6, 6 },
    { 0, 0, 0, -1, 0, 1, 5, 6, 6 },
    { 0, 0, 0, -1, 0, 1, 5, 6, 6 },
    { 0, 0, 0, -1, 0, 0, 5, 5, 0 },
    { -6, -6, -5, 0, 0, 1, 0, 0, 0 },
    { -6, -6, -5, -1, 0, 1, 0, 0, 0 },
    { -6, -6, -5, -1, 0, 1, 0, 0, 0 },
    { -6, -6, -5, -1, 0, 1, 0, 0, 0 },
    { -6, -6, -5, -1, 0, 0, 0, 0, 0 }
};

s16 lbl_1_bss_568[2];
s16 lbl_1_bss_56C;
s16 lbl_1_bss_56E;
s16 lbl_1_bss_570;
s16 lbl_1_bss_572[9]; /* unused gap */
s16 lbl_1_bss_584[3];
s16 lbl_1_bss_58A;
s16 lbl_1_bss_58C[16];
OMOBJMAN *lbl_1_bss_5AC;
float SdMachineFadeAlpha; /* lbl_1_bss_5B8 */
s16 SdMachinePadBtn[4]; /* lbl_1_bss_5BC */
s16 SdMachineFadeLayer; /* lbl_1_bss_5C4 */
s16 SdMachineFadeCount; /* lbl_1_bss_5C6 */
s16 SdMachineMask[4]; /* lbl_1_bss_5C8 */
static s16 lbl_1_bss_5D0[3];
static s16 lbl_1_bss_5D6[3];
static ANIMDATA *lbl_1_bss_5DC;
static s16 lbl_1_bss_5E0[5];
static s16 lbl_1_bss_5EA[5];
static s16 lbl_1_bss_5F4;
static s16 lbl_1_bss_5F6[5];
static void *lbl_1_bss_600;
static s16 lbl_1_bss_604;
static s16 lbl_1_bss_606;
static GWSDMACHINE lbl_1_bss_608;
static HU3DCAMERA lbl_1_bss_620;
static HUPROCESS *lbl_1_bss_678;
static SDMACHINE_DISP lbl_1_bss_67C[2];
static s16 lbl_1_bss_6D4[5];
static s16 lbl_1_bss_6DE[16];
static s16 lbl_1_bss_6FE;
static float lbl_1_bss_700;
static float lbl_1_bss_704;
static s16 lbl_1_bss_708[3];
static s16 lbl_1_bss_70E[3];
static s16 lbl_1_bss_714;
static s16 lbl_1_bss_716;
static s16 lbl_1_bss_718;
static s16 lbl_1_bss_71A;
static char lbl_1_bss_71C[6];
static s16 lbl_1_bss_722;
static s16 lbl_1_bss_724;
static s16 lbl_1_bss_726;
static s16 lbl_1_bss_728[7];
static s16 lbl_1_bss_736[10];

/* prototypes */
void fn_1_556C(void);
void fn_1_5854(s16 arg0);
void fn_1_6264(void);
void fn_1_7190(void);
void fn_1_7298(void);
void fn_1_7320(void);
s32 fn_1_73A8(s16 arg0);
void fn_1_8704(s16 arg0);
void fn_1_88BC(void);
s32 fn_1_8954(s16 arg0, s16 arg1);
void fn_1_9FAC(s16 arg0);
void fn_1_A464(void);
void fn_1_A55C(void);
void fn_1_A63C(void);
void fn_1_A704(s16 win, s16 arg1);
void fn_1_A868(s16 arg0, s16 arg1);
void fn_1_AC08(float x, float y);
void fn_1_AE4C(s16 arg0);
s16 fn_1_AF04(s16 arg0, s16 arg1);
void fn_1_AFCC(SDMACHINE_DISP *disp, u32 mesNum);
void fn_1_B8E8(SDMACHINE_DISP *disp);
void fn_1_BAA0(HUSPRITE *sprite);
void fn_1_C360(s16 arg0, s16 arg1);
void fn_1_C384(s16 headNo, s16 bodyNo, s16 tailNo, s16 itemNo, s16 *out);
void fn_1_C538(s16 arg0);
void fn_1_DD3C(s16 *arg0, s16 arg1);
void fn_1_DF94(s16 arg0, u8 arg1);
void fn_1_113F8(s16 arg0);
s16 fn_1_12940(s16 arg0, s16 *arg1, void (*arg2)(s16 *, s16));
void fn_1_13688(void);
void fn_1_138D0(void);

void fn_1_556C(void) {
    GWSDMACHINE *temp_r30;
    f32 temp_f31;
    s16 var_r31;

    var_r31 = 0;
loop_4:
    if (var_r31 < 2) {
        temp_r30 = &GwSdCommon.machine[var_r31];
        if (temp_r30->charNo != -1) {
            Hu3DMotionTimeSet(lbl_1_bss_58C[var_r31 + 3], 61.0f);
            Hu3DMotionStartEndSet(lbl_1_bss_58C[var_r31 + 3], 61.0f, 240.0f);
            Hu3DModelAttrReset(lbl_1_bss_58C[var_r31 + 3], 0x40000002U);
            Hu3DModelAttrSet(lbl_1_bss_58C[var_r31 + 3], 0x40000001U);
            fn_1_44E4(&lbl_1_bss_67C[var_r31], (s16) temp_r30->partNo[0], (s16) temp_r30->partNo[1], (s16) temp_r30->partNo[3], 0x40);
            fn_1_4638(&lbl_1_bss_67C[var_r31], 4);
            temp_f31 = (f32) (1450.0 * -cos((3.141592653589793 * (f64) ((var_r31 * 0x3C) + 0x3C)) / 180.0));
            fn_1_4534(&lbl_1_bss_67C[var_r31], (f32) (1450.0 * -sin((3.141592653589793 * (f64) ((var_r31 * 0x3C) + 0x3C)) / 180.0)), 0.0f, temp_f31);
            fn_1_4590(&lbl_1_bss_67C[var_r31], 0.0f, (f32) (((var_r31 + 1) * 0x3C) + 0x3C), 0.0f);
        }
        var_r31 += 1;
        goto loop_4;
    }
    lbl_1_bss_608.charNo = -1;
}

void fn_1_5854(s16 arg0) {
    HuVecF sp8;
    GWSDMACHINE *temp_r28;
    f32 temp_f0;
    f32 temp_f0_2;
    s16 var_r30;
    s16 var_r30_2;
    s16 var_r30_3;

    /* Flowgraph is not reducible, falling back to gotos-only mode. */
    sdWorkInt[7] = (s32) arg0;
    fn_1_6264();
    winData[lbl_1_bss_606].padMask = (u8) (1 << arg0);
    winData[lbl_1_bss_570].padMask = (u8) (1 << arg0);
    Hu3DModelAttrSet(lbl_1_bss_58C[(arg0 ^ 1) + 3], 1U);
    temp_f0 = (f32) (1000.0 * -sin((3.141592653589793 * (f64) ((arg0 * 0x3C) + 0x3C)) / 180.0));
    sp8.x = temp_f0;
    sp8.y = 90.0f;
    temp_f0_2 = (f32) (1000.0 * -cos((3.141592653589793 * (f64) ((arg0 * 0x3C) + 0x3C)) / 180.0));
    sp8.z = temp_f0_2;
    sp8.x = (f32) ((f64) temp_f0 + (170.0 * -sin((3.141592653589793 * (f64) ((arg0 * 0x3C) + 0x96)) / 180.0)));
    sp8.z = (f32) ((f64) temp_f0_2 + (170.0 * -cos((3.141592653589793 * (f64) ((arg0 * 0x3C) + 0x96)) / 180.0)));
    Hu3DModelRotSet(lbl_1_bss_58A, 0.0f, (f32) ((arg0 * 0x3C) + 0x46), 0.0f);
    Hu3DModelScaleSet(lbl_1_bss_58A, 1.1f, 1.1f, 1.1f);
    temp_r28 = &GwSdCommon.machine[arg0];
    if ((s32) omovlevtno == 2) {
        goto block_2;
    }
    if ((s32) omovlevtno != 3) {
        goto block_3;
    }
block_2:
    Hu3DModelPosSetV(lbl_1_bss_58A, &sp8);
    omCameraViewSetMulti(4, &lbl_1_data_10B0[arg0]);
    goto block_22;
block_3:
    if (temp_r28->charNo != -1) {
        goto block_5;
    }
    HuAudFXPlay(0x509);
    Hu3DMotionTimeSet(lbl_1_bss_58C[arg0 + 3], 0.0f);
    Hu3DModelAttrReset(lbl_1_bss_58C[arg0 + 3], 0x40000002U);
    Hu3DModelAttrSet(lbl_1_bss_58C[arg0 + 3], 0x40000001U);
    HuPrcSleep(0x3C);
    Hu3DMotionStartEndSet(lbl_1_bss_58C[arg0 + 3], 61.0f, 240.0f);
block_5:
    omCameraViewMoveMulti(4U, &lbl_1_data_10B0[arg0], 0x3C, 0);
    HuPrcSleep(0x3C);
    var_r30 = 1;
    goto loop_7;
block_6:
    sp8.y = (f32) (90.0 + (500.0 * (1.0 - sin((3.141592653589793 * (90.0 * ((f64) var_r30 / 20.0))) / 180.0))));
    Hu3DModelPosSetV(lbl_1_bss_58A, &sp8);
    HuPrcVSleep();
    var_r30 += 1;
loop_7:
    if (var_r30 <= 0x14) {
        goto block_6;
    }
    HuWinExOpen(lbl_1_bss_606);
    if ((u16) GwSdCommon.partFlag[0] == 0) {
        goto block_12;
    }
    if ((u16) GwSdCommon.partFlag[1] == 0) {
        goto block_12;
    }
    if ((u16) GwSdCommon.partFlag[2] == 0) {
        goto block_12;
    }
    if ((u16) GwSdCommon.partFlag[3] != 0) {
        goto block_13;
    }
block_12:
    HuWinMesSet(lbl_1_bss_606, 0x190074U);
    HuWinMesWait(lbl_1_bss_606);
    HuWinExClose(lbl_1_bss_606);
    goto block_24;
block_13:
    HuWinMesSet(lbl_1_bss_606, arg0 + 0x190009);
    HuAudFXPlay(0x2EF);
    HuWinMesWait(lbl_1_bss_606);
block_14:
    if (temp_r28->charNo != -1) {
        goto block_22;
    }
    HuWinMesSet(lbl_1_bss_606, 0x19000DU);
    HuWinMesWait(lbl_1_bss_606);
    if (HuWinChoiceGet(lbl_1_bss_606, -1) == 0) {
        goto block_17;
    }
    HuWinExClose(lbl_1_bss_606);
    goto block_24;
block_17:
    HuWinExClose(lbl_1_bss_606);
    if (fn_1_8954(arg0, 0) == 0) {
        goto block_24;
    }
    HuAudJinglePlay(0x7E);
    HuWinExOpen(lbl_1_bss_606);
    HuWinInsertMesSet(lbl_1_bss_606, MESSNUM_PTR(lbl_1_bss_67C[arg0].name), 0);
    HuWinMesSet(lbl_1_bss_606, 0x19001EU);
    var_r30_2 = 0;
    goto loop_20;
block_19:
    HuPrcVSleep();
    var_r30_2 += 1;
loop_20:
    if (var_r30_2 < 0x9B) {
        goto block_19;
    }
    HuAudSStreamPauseFadeOut(0, 0, 0x3E8);
    HuWinMesWait(lbl_1_bss_606);
    fn_1_C360(arg0, lbl_1_bss_606);
block_22:
    if (fn_1_73A8(arg0) != 0) {
        goto block_14;
    }
    HuWinExClose(lbl_1_bss_606);
block_24:
    winData[lbl_1_bss_570].padMask = 7;
    var_r30_3 = 1;
    goto loop_26;
block_25:
    sp8.y = (f32) (90.0 + (500.0 * (1.0 - cos((3.141592653589793 * (90.0 * ((f64) var_r30_3 / 20.0))) / 180.0))));
    Hu3DModelPosSetV(lbl_1_bss_58A, &sp8);
    HuPrcVSleep();
    var_r30_3 += 1;
loop_26:
    if (var_r30_3 <= 0x14) {
        goto block_25;
    }
    if (temp_r28->charNo != -1) {
        goto block_29;
    }
    HuAudFXPlay(0x509);
    Hu3DMotionTimeSet(lbl_1_bss_58C[arg0 + 3], 601.0f);
    Hu3DModelAttrReset(lbl_1_bss_58C[arg0 + 3], 0x40000001U);
    Hu3DMotionStartEndSet(lbl_1_bss_58C[arg0 + 3], 0.0f, 640.0f);
block_29:
    Hu3DModelPosSet(lbl_1_bss_58A, 0.0f, 195.0f, -1155.0f);
    Hu3DModelRotSet(lbl_1_bss_58A, 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(lbl_1_bss_58A, 1.8f, 1.8f, 1.8f);
    fn_1_7190();
    Hu3DModelAttrReset(lbl_1_bss_58C[(arg0 ^ 1) + 3], 1U);
    return;
}

void fn_1_6264(void) {
    ANIMDATA *temp_r28;
    ANIMDATA *temp_r28_2;
    ANIMDATA *temp_r3_3;
    ANIMDATA *temp_r3_6;
    s16 temp_r3;
    s16 temp_r3_2;
    s16 temp_r3_4;
    s16 temp_r3_5;
    s16 temp_r3_7;
    s16 var_r29;
    s16 var_r29_2;
    s16 var_r29_3;
    s16 var_r29_4;
    s16 var_r29_5;

    Hu3DModelAttrSet(lbl_1_bss_58C[1], 1U);
    Hu3DModelAttrSet(lbl_1_bss_58C[5], 1U);
    Hu3DModelAttrSet(lbl_1_bss_58C[2], 1U);
    Hu3DModelAttrSet(lbl_1_bss_58C[6], 1U);
    lbl_1_bss_606 = HuWinExCreateFrame(156.0f, 344.0f, 0x190, 0x5E, -1, 1);
    HuWinMesPalSet(lbl_1_bss_606, 7U, 0U, 0U, 0U);
    HuWinMesSpeedSet(lbl_1_bss_606, 0);
    HuWinPriSet(lbl_1_bss_606, 0x32);
    temp_r3 = HuWinCreate(-10000.0f, 310.0f, 0x230, 0x2A, 0);
    lbl_1_bss_604 = temp_r3;
    HuWinBGTPLvlSet(temp_r3, 0.0f);
    HuWinAttrSet(temp_r3, 0x800U);
    HuWinMesSpeedSet(temp_r3, 0);
    HuWinMesSet(temp_r3, 0x20004U);
    HuWinDispOff(temp_r3);
    temp_r3_2 = HuSprGrpCreate(0x17);
    lbl_1_bss_718 = temp_r3_2;
    HuSprGrpMemberSet(temp_r3_2, 0, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(0xBE001D, 0x10000000, HEAP_MODEL)), 0x64, 0));
    HuSprPosSet(temp_r3_2, 0, 0.0f, -1000.0f);
    HuSprGrpMemberSet(temp_r3_2, 1, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(0xBE001E, 0x10000000, HEAP_MODEL)), 0x64, 0));
    HuSprPosSet(temp_r3_2, 1, 0.0f, -1000.0f);
    HuSprGrpMemberSet(temp_r3_2, 2, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(0xBE001F, 0x10000000, HEAP_MODEL)), 0x64, 0));
    HuSprPosSet(temp_r3_2, 2, 0.0f, -1000.0f);
    HuSprGrpMemberSet(temp_r3_2, 3, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(0xBE0020, 0x10000000, HEAP_MODEL)), 0x64, 0));
    HuSprPosSet(temp_r3_2, 3, 0.0f, -1000.0f);
    HuSprGrpMemberSet(temp_r3_2, 4, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(0xBE0026, 0x10000000, HEAP_MODEL)), 0x37, 0));
    HuSprPosSet(temp_r3_2, 4, 0.0f, -1000.0f);
    HuSprDrawNoSet(temp_r3_2, 4, 0x40);
    HuSprGrpMemberSet(temp_r3_2, 5, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(0xBE0024, 0x10000000, HEAP_MODEL)), 0x32, 0));
    HuSprPosSet(temp_r3_2, 5, 0.0f, -1000.0f);
    HuSprDrawNoSet(temp_r3_2, 5, 0x40);
    HuSprGrpMemberSet(temp_r3_2, 6, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(0xBE0021, 0x10000000, HEAP_MODEL)), 0x32, 0));
    HuSprPosSet(temp_r3_2, 6, 352.0f, -1000.0f);
    HuSprGrpMemberSet(temp_r3_2, 7, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(0xBE0022, 0x10000000, HEAP_MODEL)), 0x32, 0));
    HuSprPosSet(temp_r3_2, 7, 352.0f, -1000.0f);
    HuSprGrpMemberSet(temp_r3_2, 8, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(0xBE0023, 0x10000000, HEAP_MODEL)), 0x32, 0));
    HuSprPosSet(temp_r3_2, 8, 352.0f, -1000.0f);
    HuSprGrpMemberSet(temp_r3_2, 9, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(0xBE0027, 0x10000000, HEAP_MODEL)), 0x32, 0));
    HuSprPosSet(temp_r3_2, 9, 352.0f, -1000.0f);
    HuSprGrpMemberSet(temp_r3_2, 0xA, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(0xBE0028, 0x10000000, HEAP_MODEL)), 0x32, 0));
    HuSprPosSet(temp_r3_2, 0xA, 352.0f, -1000.0f);
    HuSprGrpMemberSet(temp_r3_2, 0xB, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(0xBE0029, 0x10000000, HEAP_MODEL)), 0x32, 0));
    HuSprPosSet(temp_r3_2, 0xB, 352.0f, -1000.0f);
    temp_r3_3 = HuSprAnimRead(HuDataSelHeapReadNum(0xBE002F, 0x10000000, HEAP_MODEL));
    HuSprGrpMemberSet(temp_r3_2, 0xC, HuSprCreate(temp_r3_3, 0x14, 0));
    HuSprPosSet(temp_r3_2, 0xC, 352.0f, -1000.0f);
    HuSprColorSet(temp_r3_2, 0xC, 0U, 0U, 0U);
    HuSprTPLvlSet(temp_r3_2, 0xC, 0.5f);
    HuSprGrpMemberSet(temp_r3_2, 0xD, HuSprCreate(temp_r3_3, 0x14, 0));
    HuSprPosSet(temp_r3_2, 0xD, 352.0f, -1000.0f);
    HuSprColorSet(temp_r3_2, 0xD, 0U, 0U, 0U);
    HuSprTPLvlSet(temp_r3_2, 0xD, 0.5f);
    HuSprGrpMemberSet(temp_r3_2, 0xE, HuSprCreate(temp_r3_3, 0x14, 0));
    HuSprPosSet(temp_r3_2, 0xE, 352.0f, -1000.0f);
    HuSprColorSet(temp_r3_2, 0xE, 0U, 0U, 0U);
    HuSprTPLvlSet(temp_r3_2, 0xE, 0.5f);
    HuSprGrpMemberSet(temp_r3_2, 0xF, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(0xBE0025, 0x10000000, HEAP_MODEL)), 0x32, 0));
    HuSprPosSet(temp_r3_2, 0xF, 352.0f, -1000.0f);
    HuSprGrpMemberSet(temp_r3_2, 0x10, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(0xBE0030, 0x10000000, HEAP_MODEL)), 0x32, 0));
    HuSprPosSet(temp_r3_2, 0x10, 352.0f, -1000.0f);
    HuSprGrpMemberSet(temp_r3_2, 0x11, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(0xBE0031, 0x10000000, HEAP_MODEL)), 0x32, 0));
    HuSprPosSet(temp_r3_2, 0x11, 352.0f, -1000.0f);
    HuSprGrpMemberSet(temp_r3_2, 0x12, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(0xBE0032, 0x10000000, HEAP_MODEL)), 0x14, 0));
    HuSprPosSet(temp_r3_2, 0x12, 288.0f, -1000.0f);
    HuSprGrpMemberSet(temp_r3_2, 0x13, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(0xBE0033, 0x10000000, HEAP_MODEL)), 0x14, 0));
    HuSprPosSet(temp_r3_2, 0x13, 288.0f, -1000.0f);
    HuSprGrpMemberSet(temp_r3_2, 0x14, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(0xBE002B, 0x10000000, HEAP_MODEL)), 0x32, 0));
    HuSprPosSet(temp_r3_2, 0x14, 288.0f, -1000.0f);
    HuSprGrpMemberSet(temp_r3_2, 0x15, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(0xBE002C, 0x10000000, HEAP_MODEL)), 0x32, 0));
    HuSprPosSet(temp_r3_2, 0x15, 288.0f, -1000.0f);
    HuSprGrpMemberSet(temp_r3_2, 0x16, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(0xBE002D, 0x10000000, HEAP_MODEL)), 0x32, 0));
    HuSprPosSet(temp_r3_2, 0x16, 288.0f, -1000.0f);
    temp_r3_4 = HuSprGrpCreate(9);
    lbl_1_bss_716 = temp_r3_4;
    var_r29 = 0;
loop_2:
    if (var_r29 < 8) {
        HuSprGrpMemberSet(temp_r3_4, var_r29, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(var_r29 + 0xBE0034, 0x10000000, HEAP_MODEL)), 0x1E, 0));
        HuSprAttrSet(temp_r3_4, var_r29, 4);
        HuSprPosSet(temp_r3_4, var_r29, 0.0f, -1000.0f);
        var_r29 += 1;
        goto loop_2;
    }
    temp_r3_5 = HuSprGrpCreate(0xF);
    lbl_1_bss_714 = temp_r3_5;
    HuSprGrpMemberSet(temp_r3_5, 0, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(0xBE003C, 0x10000000, HEAP_MODEL)), 0x64, 0));
    HuSprPosSet(temp_r3_5, 0, 0.0f, 0.0f);
    temp_r28 = HuSprAnimRead(HuDataSelHeapReadNum(0xBE003D, 0x10000000, HEAP_MODEL));
    var_r29_2 = 0;
loop_5:
    if (var_r29_2 < 0xB) {
        HuSprGrpMemberSet(temp_r3_5, (s16) (var_r29_2 + 1), HuSprCreate(temp_r28, 0x32, var_r29_2));
        HuSprPosSet(temp_r3_5, (s16) (var_r29_2 + 1), 0.0f, 0.0f);
        var_r29_2 += 1;
        goto loop_5;
    }
    temp_r28_2 = HuSprAnimRead(HuDataSelHeapReadNum(0xBE0040, 0x10000000, HEAP_MODEL));
    var_r29_3 = 0;
loop_8:
    if (var_r29_3 < 3) {
        HuSprGrpMemberSet(temp_r3_5, (s16) (var_r29_3 + 0xC), HuSprCreate(temp_r28_2, 0x28, var_r29_3));
        HuSprPosSet(temp_r3_5, (s16) (var_r29_3 + 0xC), 0.0f, 0.0f);
        var_r29_3 += 1;
        goto loop_8;
    }
    var_r29_4 = 0;
loop_11:
    if (var_r29_4 < 0xF) {
        HuSprAttrSet(temp_r3_5, var_r29_4, 4);
        var_r29_4 += 1;
        goto loop_11;
    }
    HuSprGrpDrawNoSet(temp_r3_5, 0x40);
    temp_r3_6 = HuSprAnimRead(HuDataSelHeapReadNum(0xBE003F, 0x10000000, HEAP_MODEL));
    lbl_1_bss_600 = temp_r3_6;
    var_r29_5 = 0;
loop_14:
    if (var_r29_5 < 3) {
        temp_r3_7 = Hu3DModelCreate(HuDataSelHeapReadNum(0xBE003E, 0x10000000, HEAP_MODEL));
        lbl_1_bss_70E[var_r29_5] = temp_r3_7;
        lbl_1_bss_708[var_r29_5] = Hu3DAnimCreate(temp_r3_6, temp_r3_7, "dummy");
        Hu3DModelAttrSet(temp_r3_7, 1U);
        Hu3DModelAttrSet(temp_r3_7, 0x40000001U);
        Hu3DModelCameraSet(temp_r3_7, 4U);
        Hu3DModelLayerSet(temp_r3_7, 6);
        Hu3DModelRotSet(temp_r3_7, 0.0f, (f32) ((sdWorkInt[7] + 1) * 0x3C), 0.0f);
        Hu3DAnimBankSet(lbl_1_bss_708[var_r29_5], 0);
        var_r29_5 += 1;
        goto loop_14;
    }
    Hu3DCameraPerspectiveSet(8, 5.0f, 20.0f, 15000.0f, 1.2f);
    Hu3DCameraViewportSet(8, -3.5555496f, 72.0f, 256.0f, 192.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(8, 0x16U, 0x4CU, 0xCCU, 0xB8U);
    omCameraViewSetMulti(8, &lbl_1_data_10E8);
    SdMachineFadeAlpha = 0.0f;
    SdMachineFadeCount = 0;
}

void fn_1_7190(void) {
    s16 var_r31;

    Hu3DModelAttrReset(lbl_1_bss_58C[1], 1U);
    Hu3DModelAttrReset(lbl_1_bss_58C[5], 1U);
    Hu3DModelAttrReset(lbl_1_bss_58C[2], 1U);
    Hu3DModelAttrReset(lbl_1_bss_58C[6], 1U);
    HuSprGrpKill(lbl_1_bss_716);
    HuSprGrpKill(lbl_1_bss_718);
    HuSprGrpKill(lbl_1_bss_714);
    HuWinKill(lbl_1_bss_606);
    HuWinKill(lbl_1_bss_604);
    var_r31 = 0;
loop_2:
    if (var_r31 < 3) {
        Hu3DModelKill(lbl_1_bss_70E[var_r31]);
        var_r31 += 1;
        goto loop_2;
    }
    HuMemDirectFree(lbl_1_bss_600);
}

void fn_1_7298(void) {
    s16 var_r31;

    var_r31 = 0;
loop_4:
    if (var_r31 < 2) {
        if (GwSdCommon.machine[var_r31].charNo != -1) {
            fn_1_46C4(&lbl_1_bss_67C[var_r31], 1);
        }
        var_r31 += 1;
        goto loop_4;
    }
}

void fn_1_7320(void) {
    s16 var_r31;

    var_r31 = 0;
loop_4:
    if (var_r31 < 2) {
        if (GwSdCommon.machine[var_r31].charNo != -1) {
            fn_1_46C4(&lbl_1_bss_67C[var_r31], 0);
        }
        var_r31 += 1;
        goto loop_4;
    }
}

s32 fn_1_73A8(s16 arg0) {
    SDMACHINE_DISP sp8;
    GWSDMACHINE *temp_r28;
    HUPROCESS *temp_r3;
    f32 temp_f31;
    f32 temp_f31_10;
    f32 temp_f31_11;
    f32 temp_f31_12;
    f32 temp_f31_2;
    f32 temp_f31_3;
    f32 temp_f31_4;
    f32 temp_f31_5;
    f32 temp_f31_6;
    f32 temp_f31_7;
    f32 temp_f31_8;
    f32 temp_f31_9;
    s16 var_r27;
    s16 var_r27_2;
    s16 var_r29;
    s16 var_r31;
    s16 var_r31_10;
    s16 var_r31_11;
    s16 var_r31_12;
    s16 var_r31_13;
    s16 var_r31_2;
    s16 var_r31_3;
    s16 var_r31_4;
    s16 var_r31_5;
    s16 var_r31_6;
    s16 var_r31_7;
    s16 var_r31_8;
    s16 var_r31_9;
    s32 var_r25;

    /* Flowgraph is not reducible, falling back to gotos-only mode. */
    var_r29 = 0;
    var_r25 = 0;
    var_r31 = 0;
    goto loop_2;
block_1:
    sp8.partNo[var_r31] = -1;
    var_r31 += 1;
loop_2:
    if (var_r31 < 4) {
        goto block_1;
    }
    temp_r28 = &GwSdCommon.machine[arg0];
    fn_1_4D0C(0xD);
    if ((s32) omovlevtno == 2) {
        goto block_5;
    }
    if ((s32) omovlevtno != 3) {
        goto block_9;
    }
block_5:
    HuSprPosSet(lbl_1_bss_718, 0, 96.0f, 128.0f);
    HuSprPosSet(lbl_1_bss_718, 1, 224.0f, 128.0f);
    HuSprPosSet(lbl_1_bss_718, 2, 352.0f, 128.0f);
    HuSprPosSet(lbl_1_bss_718, 3, 480.0f, 128.0f);
    var_r31_2 = 0;
    goto loop_7;
block_6:
    HuSprScaleSet(lbl_1_bss_718, var_r31_2, 0.0f, 0.0f);
    var_r31_2 += 1;
loop_7:
    if (var_r31_2 < 4) {
        goto block_6;
    }
    goto block_68;
block_9:
    HuWinMesSet(lbl_1_bss_606, 0x19000EU);
    HuWinMesWait(lbl_1_bss_606);
    var_r31_3 = 0;
    goto loop_11;
block_10:
    HuSprScaleSet(lbl_1_bss_718, var_r31_3, 0.8f, 0.8f);
    var_r31_3 += 1;
loop_11:
    if (var_r31_3 < 4) {
        goto block_10;
    }
    var_r31_4 = 1;
    goto loop_14;
block_13:
    temp_f31 = (f32) (128.0 - (500.0 * (1.0 - sin((3.141592653589793 * (90.0 * ((f64) var_r31_4 / 20.0))) / 180.0))));
    HuSprPosSet(lbl_1_bss_718, 0, 96.0f, temp_f31);
    HuSprPosSet(lbl_1_bss_718, 1, 224.0f, temp_f31);
    HuSprPosSet(lbl_1_bss_718, 2, 352.0f, temp_f31);
    HuSprPosSet(lbl_1_bss_718, 3, 480.0f, temp_f31);
    HuPrcVSleep();
    var_r31_4 += 1;
loop_14:
    if (var_r31_4 <= 0x14) {
        goto block_13;
    }
    var_r31_5 = 1;
    goto loop_17;
block_16:
    temp_f31_2 = (f32) (0.8 + (0.2 * sin((3.141592653589793 * (90.0 * ((f64) var_r31_5 / 10.0))) / 180.0)));
    HuSprScaleSet(lbl_1_bss_718, 0, temp_f31_2, temp_f31_2);
    HuPrcVSleep();
    var_r31_5 += 1;
loop_17:
    if (var_r31_5 <= 0xA) {
        goto block_16;
    }
    HuWinMesSet(lbl_1_bss_606, 0x19000FU);
    HuWinMesWait(lbl_1_bss_606);
    HuWinMesSet(lbl_1_bss_604, 0x20004U);
    HuWinDispOn(lbl_1_bss_604);
block_19:
    if ((s32) (fn_1_507C((u16) (1 << arg0)) & 1) == 0) {
        goto block_25;
    }
    if (var_r29 <= 0) {
        goto block_25;
    }
    HuAudFXPlay(0);
    var_r31_6 = 1;
    goto loop_23;
block_22:
    temp_f31_3 = (f32) (0.8 + (0.2 * cos((3.141592653589793 * (90.0 * ((f64) var_r31_6 / 10.0))) / 180.0)));
    HuSprScaleSet(lbl_1_bss_718, var_r29, temp_f31_3, temp_f31_3);
    temp_f31_4 = (f32) (0.8 + (0.2 * sin((3.141592653589793 * (90.0 * ((f64) var_r31_6 / 10.0))) / 180.0)));
    HuSprScaleSet(lbl_1_bss_718, (s16) (var_r29 - 1), temp_f31_4, temp_f31_4);
    HuPrcVSleep();
    var_r31_6 += 1;
loop_23:
    if (var_r31_6 <= 0xA) {
        goto block_22;
    }
    var_r29 -= 1;
    HuWinMesSet(lbl_1_bss_606, var_r29 + 0x19000F);
    HuWinMesWait(lbl_1_bss_606);
    goto block_81;
block_25:
    if ((s32) (fn_1_507C((u16) (1 << arg0)) & 2) == 0) {
        goto block_31;
    }
    if (var_r29 >= 3) {
        goto block_31;
    }
    HuAudFXPlay(0);
    var_r31_7 = 1;
    goto loop_29;
block_28:
    temp_f31_5 = (f32) (0.8 + (0.2 * cos((3.141592653589793 * (90.0 * ((f64) var_r31_7 / 10.0))) / 180.0)));
    HuSprScaleSet(lbl_1_bss_718, var_r29, temp_f31_5, temp_f31_5);
    temp_f31_6 = (f32) (0.8 + (0.2 * sin((3.141592653589793 * (90.0 * ((f64) var_r31_7 / 10.0))) / 180.0)));
    HuSprScaleSet(lbl_1_bss_718, (s16) (var_r29 + 1), temp_f31_6, temp_f31_6);
    HuPrcVSleep();
    var_r31_7 += 1;
loop_29:
    if (var_r31_7 <= 0xA) {
        goto block_28;
    }
    var_r29 += 1;
    HuWinMesSet(lbl_1_bss_606, var_r29 + 0x19000F);
    HuWinMesWait(lbl_1_bss_606);
    goto block_81;
block_31:
    if ((s32) (fn_1_4FD0((u16) (1 << arg0)) & 0x100) == 0) {
        goto block_39;
    }
    if (var_r29 != 3) {
        goto block_39;
    }
    HuAudFXPlay(1);
    HuWinDispOff(lbl_1_bss_604);
    HuWinMesSet(lbl_1_bss_606, 0x190014U);
    HuWinMesWait(lbl_1_bss_606);
    if (HuWinChoiceGet(lbl_1_bss_606, -1) != 0) {
        goto block_38;
    }
    HuAudFXPlay(0x50D);
    var_r31_8 = 1;
    goto loop_36;
block_35:
    temp_f31_7 = (f32) cos((3.141592653589793 * (90.0 * ((f64) var_r31_8 / 30.0))) / 180.0);
    Hu3DModelScaleSet(lbl_1_bss_67C[arg0].headMdl, temp_f31_7, 1.0f, temp_f31_7);
    Hu3DModelRotSet(lbl_1_bss_67C[arg0].headMdl, 0.0f, (f32) ((600.0 * (1.0 - (f64) temp_f31_7)) + (f64) (((arg0 + 1) * 0x3C) + 0x3C)), 0.0f);
    HuPrcVSleep();
    var_r31_8 += 1;
loop_36:
    if (var_r31_8 <= 0x1E) {
        goto block_35;
    }
    fn_1_4724(&lbl_1_bss_67C[arg0]);
    temp_r28->charNo = -1;
    var_r25 = 1;
    goto block_82;
block_38:
    HuWinMesSet(lbl_1_bss_606, var_r29 + 0x19000F);
    HuWinMesWait(lbl_1_bss_606);
    HuWinMesSet(lbl_1_bss_604, 0x20004U);
    HuWinDispOn(lbl_1_bss_604);
    goto block_81;
block_39:
    if ((s32) (fn_1_4FD0((u16) (1 << arg0)) & 0x100) == 0) {
        goto block_79;
    }
    HuAudFXPlay(1);
    HuWinDispOff(lbl_1_bss_604);
    var_r31_9 = 1;
    goto loop_48;
block_41:
    temp_f31_8 = (f32) (1.0 - ((f64) var_r31_9 / 10.0));
    var_r27 = 0;
    goto loop_46;
block_42:
    if (var_r29 != var_r27) {
        goto block_44;
    }
    HuSprScaleSet(lbl_1_bss_718, var_r27, temp_f31_8, temp_f31_8);
    goto block_45;
block_44:
    HuSprScaleSet(lbl_1_bss_718, var_r27, (f32) (0.8 * (f64) temp_f31_8), (f32) (0.8 * (f64) temp_f31_8));
block_45:
    var_r27 += 1;
loop_46:
    if (var_r27 < 4) {
        goto block_42;
    }
    HuPrcVSleep();
    var_r31_9 += 1;
loop_48:
    if (var_r31_9 <= 0xA) {
        goto block_41;
    }
    if (var_r29 == 1) {
        goto block_64;
    }
    if (var_r29 >= 1) {
        goto block_53;
    }
    if (var_r29 >= 0) {
        goto block_55;
    }
    goto block_69;
block_53:
    if (var_r29 >= 3) {
        goto block_69;
    }
    goto block_68;
block_55:
    HuWinExClose(lbl_1_bss_606);
    HuAudFXPlay(0x50D);
    var_r31_10 = 1;
    goto loop_57;
block_56:
    temp_f31_9 = (f32) cos((3.141592653589793 * (90.0 * ((f64) var_r31_10 / 30.0))) / 180.0);
    Hu3DModelScaleSet(lbl_1_bss_67C[arg0].headMdl, temp_f31_9, 1.0f, temp_f31_9);
    Hu3DModelRotSet(lbl_1_bss_67C[arg0].headMdl, 0.0f, (f32) ((600.0 * (1.0 - (f64) temp_f31_9)) + (f64) (((arg0 + 1) * 0x3C) + 0x3C)), 0.0f);
    HuPrcVSleep();
    var_r31_10 += 1;
loop_57:
    if (var_r31_10 <= 0x1E) {
        goto block_56;
    }
    fn_1_4724(&lbl_1_bss_67C[arg0]);
    if (fn_1_8954(arg0, 1) != 0) {
        goto block_63;
    }
    fn_1_44E4(&lbl_1_bss_67C[arg0], (s16) temp_r28->partNo[0], (s16) temp_r28->partNo[1], (s16) temp_r28->partNo[3], 0x40);
    fn_1_8704(arg0);
    var_r31_11 = 1;
    goto loop_61;
block_60:
    temp_f31_10 = (f32) sin((3.141592653589793 * (90.0 * ((f64) var_r31_11 / 30.0))) / 180.0);
    Hu3DModelScaleSet(lbl_1_bss_67C[arg0].headMdl, temp_f31_10, 1.0f, temp_f31_10);
    Hu3DModelRotSet(lbl_1_bss_67C[arg0].headMdl, 0.0f, (f32) ((600.0 * (1.0 - (f64) temp_f31_10)) + (f64) (((arg0 + 1) * 0x3C) + 0x3C)), 0.0f);
    HuPrcVSleep();
    var_r31_11 += 1;
loop_61:
    if (var_r31_11 <= 0x1E) {
        goto block_60;
    }
    HuWinExOpen(lbl_1_bss_606);
    goto block_69;
block_63:
    HuWinExOpen(lbl_1_bss_606);
    fn_1_C360(arg0, lbl_1_bss_606);
    goto block_69;
block_64:
    HuWinExClose(lbl_1_bss_606);
    lbl_1_bss_6D4[0] = (s16) temp_r28->partNo[0];
    lbl_1_bss_6D4[1] = (s16) temp_r28->partNo[1];
    lbl_1_bss_6D4[2] = (s16) temp_r28->partNo[2];
    lbl_1_bss_6D4[3] = (s16) temp_r28->partNo[3];
    lbl_1_bss_6D4[4] = (s16) temp_r28->charNo;
    strcpy(sp8.name, temp_r28->name);
    sp8.partNo[0] = -1;
    temp_r3 = HuPrcChildCreate(fn_1_88BC, 0x3E8U, 0x1000U, 0, HuPrcCurrentGet());
    temp_r3->property = &sp8;
    fn_1_AFCC(&sp8, 0x2000BU);
    goto loop_66;
block_65:
    HuPrcVSleep();
loop_66:
    if ((s32) (fn_1_4FD0((u16) (1 << arg0)) & 0x200) == 0) {
        goto block_65;
    }
    HuAudFXPlay(3);
    HuPrcKill(temp_r3);
    fn_1_B8E8(&sp8);
    fn_1_4724(&sp8);
    HuWinExOpen(lbl_1_bss_606);
    goto block_69;
block_68:
    fn_1_C538(arg0);
block_69:
    var_r31_12 = 1;
    goto loop_77;
block_70:
    temp_f31_11 = (f32) ((f64) var_r31_12 / 10.0);
    var_r27_2 = 0;
    goto loop_75;
block_71:
    if (var_r29 != var_r27_2) {
        goto block_73;
    }
    HuSprScaleSet(lbl_1_bss_718, var_r27_2, temp_f31_11, temp_f31_11);
    goto block_74;
block_73:
    HuSprScaleSet(lbl_1_bss_718, var_r27_2, (f32) (0.8 * (f64) temp_f31_11), (f32) (0.8 * (f64) temp_f31_11));
block_74:
    var_r27_2 += 1;
loop_75:
    if (var_r27_2 < 4) {
        goto block_71;
    }
    HuPrcVSleep();
    var_r31_12 += 1;
loop_77:
    if (var_r31_12 <= 0xA) {
        goto block_70;
    }
    HuWinMesSet(lbl_1_bss_606, var_r29 + 0x19000F);
    HuWinMesWait(lbl_1_bss_606);
    HuWinMesSet(lbl_1_bss_604, 0x20004U);
    HuWinDispOn(lbl_1_bss_604);
    goto block_81;
block_79:
    if ((s32) (fn_1_4FD0((u16) (1 << arg0)) & 0x200) == 0) {
        goto block_81;
    }
    HuAudFXPlay(3);
    HuWinDispOff(lbl_1_bss_604);
    goto block_82;
block_81:
    HuPrcVSleep();
    goto block_19;
block_82:
    var_r31_13 = 1;
    goto loop_84;
block_83:
    temp_f31_12 = (f32) (128.0 - (500.0 * (1.0 - cos((3.141592653589793 * (90.0 * ((f64) var_r31_13 / 20.0))) / 180.0))));
    HuSprPosSet(lbl_1_bss_718, 0, 96.0f, temp_f31_12);
    HuSprPosSet(lbl_1_bss_718, 1, 224.0f, temp_f31_12);
    HuSprPosSet(lbl_1_bss_718, 2, 352.0f, temp_f31_12);
    HuSprPosSet(lbl_1_bss_718, 3, 480.0f, temp_f31_12);
    HuPrcVSleep();
    var_r31_13 += 1;
loop_84:
    if (var_r31_13 <= 0x14) {
        goto block_83;
    }
    fn_1_4E14();
    return var_r25;
}

void fn_1_8704(s16 arg0) {
    f32 temp_f31;

    fn_1_4638(&lbl_1_bss_67C[arg0], 4);
    temp_f31 = (f32) (1450.0 * -cos((3.141592653589793 * (f64) ((arg0 * 0x3C) + 0x3C)) / 180.0));
    fn_1_4534(&lbl_1_bss_67C[arg0], (f32) (1450.0 * -sin((3.141592653589793 * (f64) ((arg0 * 0x3C) + 0x3C)) / 180.0)), 0.0f, temp_f31);
    fn_1_4590(&lbl_1_bss_67C[arg0], 0.0f, (f32) (((arg0 + 1) * 0x3C) + 0x3C), 0.0f);
}

void fn_1_88BC(void) {
    s16 var_r31;
    SDMACHINE_DISP *temp_r30;

    var_r31 = 0;
    temp_r30 = HuPrcCurrentGet()->property;
loop_1:
    if (temp_r30->partNo[0] >= 0) {
        Hu3DModelRotSet(temp_r30->headMdl, 0.0f, (f32) var_r31, 0.0f);
        var_r31 += 1;
        if (var_r31 > 0x168) {
            var_r31 -= 0x168;
        }
    }
    HuPrcVSleep();
    goto loop_1;
}

s32 fn_1_8954(s16 arg0, s16 arg1) {
    s16 sp8[5];
    GWSDMACHINE *temp_r29;
    GWSDMACHINE *temp_r29_2;
    f32 temp_f30;
    f32 temp_f31;
    f32 temp_f31_2;
    f32 temp_f31_3;
    f32 temp_f31_4;
    f32 temp_f31_5;
    f32 temp_f31_6;
    f32 temp_f31_7;
    s16 temp_r23;
    s16 temp_r23_2;
    s16 temp_r3;
    s16 temp_r3_2;
    s16 temp_r3_3;
    s16 temp_ret;
    s16 var_r18;
    s16 var_r19;
    s16 var_r20;
    s16 var_r21;
    s16 var_r22;
    s16 var_r24;
    s16 var_r27;
    s16 var_r28;
    s16 var_r31;
    s16 var_r31_10;
    s16 var_r31_11;
    s16 var_r31_12;
    s16 var_r31_13;
    s16 var_r31_2;
    s16 var_r31_3;
    s16 var_r31_4;
    s16 var_r31_5;
    s16 var_r31_6;
    s16 var_r31_7;
    s16 var_r31_8;
    s16 var_r31_9;
    s32 var_r25;
    u8 temp_r0;
    u8 temp_r5;

    Hu3DCameraPerspectiveSet(8, 5.0f, 20.0f, 15000.0f, 1.2f);
    Hu3DCameraViewportSet(8, -3.5555496f, 72.0f, 256.0f, 192.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(8, 0x16U, 0x4CU, 0xCCU, 0xB8U);
    omCameraViewSetMulti(8, &lbl_1_data_10E8);
    lbl_1_bss_724 = 0;
    fn_1_4D0C(0xD);
    lbl_1_bss_722 = 0;
    lbl_1_bss_726 = 0;
    temp_r29 = &GwSdCommon.machine[arg0];
    if (temp_r29->charNo != -1) {
        var_r31 = 0;
loop_3:
        if (var_r31 < 4) {
            sp8[var_r31] = (s16) temp_r29->partNo[var_r31];
            var_r31 += 1;
            goto loop_3;
        }
        sp8[var_r31] = (s16) temp_r29->charNo;
    } else {
        var_r31_2 = 0;
loop_7:
        if (var_r31_2 < 5) {
            sp8[var_r31_2] = -1;
            var_r31_2 += 1;
            goto loop_7;
        }
    }
    var_r31_3 = 0;
loop_10:
    if (var_r31_3 < 4) {
        lbl_1_bss_67C[arg0].partNo[var_r31_3] = -1;
        var_r31_3 += 1;
        goto loop_10;
    }
    if (arg1 == 0) {
        var_r31_4 = 0;
loop_14:
        if (var_r31_4 < 5) {
            lbl_1_bss_6D4[var_r31_4] = -1;
            var_r31_4 += 1;
            goto loop_14;
        }
    } else {
        lbl_1_bss_6D4[0] = (s16) temp_r29->partNo[0];
        lbl_1_bss_6D4[1] = (s16) temp_r29->partNo[1];
        lbl_1_bss_6D4[2] = (s16) temp_r29->partNo[2];
        lbl_1_bss_6D4[3] = (s16) temp_r29->partNo[3];
        lbl_1_bss_6D4[4] = (s16) temp_r29->charNo;
    }
    fn_1_9FAC(arg0);
    fn_1_A464();
    temp_ret = fn_1_AF04(lbl_1_bss_726, sp8[0]);
    lbl_1_bss_724 = temp_ret;
    if ((s16) lbl_1_bss_724 > 4) {
        var_r22 = lbl_1_bss_724 - 4;
    } else {
        var_r22 = 0;
    }
    lbl_1_bss_722 = var_r22;
    var_r28 = lbl_1_bss_724 - lbl_1_bss_722;
    OSReport("top:%d,idx:%d\n", lbl_1_bss_722, lbl_1_bss_724, &lbl_1_bss_722);
    fn_1_AC08(214.0f, 72.0f);
    fn_1_A868(lbl_1_bss_726, lbl_1_bss_722);
    if (arg1 == 1) {
        fn_1_AE4C(arg0);
        var_r31_5 = 1;
loop_23:
        if (var_r31_5 <= 0xA) {
            temp_f31 = (f32) sin((3.141592653589793 * (90.0 * ((f64) var_r31_5 / 10.0))) / 180.0);
            Hu3DModelScaleSet(lbl_1_bss_67C[arg0].headMdl, temp_f31, temp_f31, temp_f31);
            HuPrcVSleep();
            var_r31_5 += 1;
            goto loop_23;
        }
    }
    HuWinMesSet(lbl_1_bss_728[0], 0x190027U);
    HuWinDispOn(lbl_1_bss_728[0]);
loop_25:
    var_r27 = 0;
    if ((s32) (fn_1_507C((u16) (1 << arg0)) & 8) != 0) {
        var_r28 -= 1;
        var_r27 = -1;
        if (var_r28 < 0) {
            if ((s16) lbl_1_bss_722 != 0) {
                lbl_1_bss_722 -= 1;
            } else {
                var_r27 = 0;
            }
            var_r28 = 0;
        }
block_96:
        if (var_r27 != 0) {
            HuAudFXPlay(0);
            temp_r3 = lbl_1_bss_722 + var_r28;
            lbl_1_bss_724 = temp_r3;
            fn_1_A868(lbl_1_bss_726, lbl_1_bss_722);
            fn_1_A704(lbl_1_bss_728[5], lbl_1_bss_726);
            HuWinMesSet(lbl_1_bss_728[0], lbl_1_bss_726 + 0x190027);
            HuWinMesSet(lbl_1_bss_728[2], 0x20019U);
            HuWinPosSet(lbl_1_bss_728[2], 16.0f, 354.0f);
        }
        temp_r0 = HuPadTrigL[arg0];
        CZoomM[3] += (f32) (temp_r0 / 2);
        temp_r5 = HuPadTrigR[arg0];
        CZoomM[3] -= (f32) (temp_r5 / 2);
        if (CZoomM[3] < 3000.0f) {
            CZoomM[3] = 3000.0f;
        }
        if (CZoomM[3] > 7000.0f) {
            CZoomM[3] = 7000.0f;
        }
        HuPrcVSleep();
        goto loop_25;
    }
    if ((s32) (fn_1_507C((u16) (1 << arg0)) & 4) != 0) {
        var_r28 += 1;
        var_r27 = 1;
        if ((s32) (var_r28 + lbl_1_bss_722) < (s16) lbl_1_bss_6FE) {
            if (var_r28 >= 5) {
                if ((s32) (lbl_1_bss_722 + 5) < (s16) lbl_1_bss_6FE) {
                    lbl_1_bss_722 += 1;
                } else {
                    var_r27 = 0;
                }
                var_r28 = 4;
            }
        } else {
            var_r27 = 0;
            var_r28 -= 1;
        }
        goto block_96;
    }
    if ((s32) (fn_1_4FD0((u16) (1 << arg0)) & 0x200) != 0) {
        HuAudFXPlay(3);
        lbl_1_bss_726 -= 1;
        if ((s16) lbl_1_bss_726 >= 0) {
            fn_1_4724(&lbl_1_bss_67C[arg0]);
            if (arg1 == 0) {
                lbl_1_bss_6D4[lbl_1_bss_726] = -1;
            }
            if ((s16) lbl_1_bss_726 < 4) {
                lbl_1_bss_67C[arg0].partNo[lbl_1_bss_726] = -1;
            }
            fn_1_AE4C(arg0);
        }
        HuAudFXPlay(0x50C);
        lbl_1_bss_724 = fn_1_AF04(lbl_1_bss_726, sp8[lbl_1_bss_726]);
        if ((s16) lbl_1_bss_724 > 4) {
            var_r21 = lbl_1_bss_724 - 4;
        } else {
            var_r21 = 0;
        }
        lbl_1_bss_722 = var_r21;
        var_r28 = lbl_1_bss_724 - lbl_1_bss_722;
        if ((s16) lbl_1_bss_726 < 0) {
            if (arg1 == 1) {
                var_r31_6 = 1;
loop_53:
                if (var_r31_6 <= 0xA) {
                    temp_f31_2 = (f32) cos((3.141592653589793 * (90.0 * ((f64) var_r31_6 / 10.0))) / 180.0);
                    Hu3DModelScaleSet(lbl_1_bss_67C[arg0].headMdl, temp_f31_2, temp_f31_2, temp_f31_2);
                    HuPrcVSleep();
                    var_r31_6 += 1;
                    goto loop_53;
                }
            }
            fn_1_A55C();
            var_r25 = 1;
        } else {
            var_r27 = -0xA;
            goto block_96;
        }
    } else if ((s32) (fn_1_4FD0((u16) (1 << arg0)) & 0x100) != 0) {
        HuAudFXPlay(1);
        var_r24 = 0;
        var_r31_7 = 0;
loop_62:
        if (var_r31_7 < (s16) SdMachinePartNum[lbl_1_bss_726]) {
            if (fn_1_3EC0(lbl_1_bss_726, var_r31_7) != 0) {
                if (var_r24 != (s16) lbl_1_bss_724) {
                    var_r24 += 1;
                    goto block_61;
                }
            } else {
block_61:
                var_r31_7 += 1;
                goto loop_62;
            }
        }
        sp8[lbl_1_bss_726] = var_r31_7;
        lbl_1_bss_6D4[lbl_1_bss_726] = var_r31_7;
        HuAudFXPlay(0x50B);
        lbl_1_bss_6D4[4] = -1;
        fn_1_AE4C(arg0);
        lbl_1_bss_726 += 1;
        lbl_1_bss_724 = fn_1_AF04(lbl_1_bss_726, sp8[lbl_1_bss_726]);
        if ((s16) lbl_1_bss_724 > 4) {
            var_r20 = lbl_1_bss_724 - 4;
        } else {
            var_r20 = 0;
        }
        lbl_1_bss_722 = var_r20;
        var_r28 = lbl_1_bss_724 - lbl_1_bss_722;
        if ((s16) lbl_1_bss_726 >= 4) {
            if (arg1 == 0) {
                var_r31_8 = 1;
loop_70:
                if (var_r31_8 <= 0xA) {
                    temp_f31_3 = (f32) cos((3.141592653589793 * (90.0 * ((f64) var_r31_8 / 10.0))) / 180.0);
                    Hu3DModelScaleSet(lbl_1_bss_67C[arg0].headMdl, temp_f31_3, temp_f31_3, temp_f31_3);
                    HuPrcVSleep();
                    var_r31_8 += 1;
                    goto loop_70;
                }
                fn_1_A55C();
                omCameraViewSetMulti(8, &lbl_1_data_10E8);
loop_72:
                if (NameEnterMain(lbl_1_bss_67C[arg0].name, (s16) (1 << arg0)) != 0) {
                    fn_1_AFCC(&lbl_1_bss_67C[arg0], 0U);
                    HuWinExOpen(lbl_1_bss_570);
                    HuWinMesSet(lbl_1_bss_570, 0x19001DU);
                    HuWinMesWait(lbl_1_bss_570);
                    temp_r23 = HuWinChoiceGet(lbl_1_bss_570, -1);
                    HuWinExClose(lbl_1_bss_570);
                    fn_1_B8E8(&lbl_1_bss_67C[arg0]);
                    if (temp_r23 == 0) {
                        var_r25 = 0;
                    } else {
                        goto loop_72;
                    }
                } else {
                    lbl_1_bss_726 = 3;
                    lbl_1_bss_724 = fn_1_AF04(lbl_1_bss_726, sp8[lbl_1_bss_726]);
                    if ((s16) lbl_1_bss_724 > 4) {
                        var_r19 = lbl_1_bss_724 - 4;
                    } else {
                        var_r19 = 0;
                    }
                    lbl_1_bss_722 = var_r19;
                    temp_r3_2 = lbl_1_bss_724 - lbl_1_bss_722;
                    var_r28 = temp_r3_2;
                    fn_1_A464();
                    fn_1_A868(lbl_1_bss_726, lbl_1_bss_722);
                    fn_1_4724(&lbl_1_bss_67C[arg0]);
                    if (arg1 == 0) {
                        lbl_1_bss_6D4[lbl_1_bss_726] = -1;
                    }
                    fn_1_AE4C(arg0);
                    var_r31_9 = 1;
loop_82:
                    if (var_r31_9 <= 0xA) {
                        temp_f31_4 = (f32) sin((3.141592653589793 * (90.0 * ((f64) var_r31_9 / 10.0))) / 180.0);
                        Hu3DModelScaleSet(lbl_1_bss_67C[arg0].headMdl, temp_f31_4, temp_f31_4, temp_f31_4);
                        HuPrcVSleep();
                        var_r31_9 += 1;
                        goto loop_82;
                    }
                    goto block_95;
                }
            } else {
                strcpy(lbl_1_bss_67C[arg0].name, GwSdCommon.machine[arg0].name);
                var_r31_10 = 1;
loop_86:
                if (var_r31_10 <= 0xA) {
                    temp_f31_5 = (f32) cos((3.141592653589793 * (90.0 * ((f64) var_r31_10 / 10.0))) / 180.0);
                    Hu3DModelScaleSet(lbl_1_bss_67C[arg0].headMdl, temp_f31_5, temp_f31_5, temp_f31_5);
                    HuPrcVSleep();
                    var_r31_10 += 1;
                    goto loop_86;
                }
                fn_1_A55C();
                omCameraViewSetMulti(8, &lbl_1_data_10E8);
                fn_1_AFCC(&lbl_1_bss_67C[arg0], 0U);
                HuWinExOpen(lbl_1_bss_570);
                HuWinMesSet(lbl_1_bss_570, 0x19001DU);
                HuWinMesWait(lbl_1_bss_570);
                temp_r23_2 = HuWinChoiceGet(lbl_1_bss_570, -1);
                HuWinExClose(lbl_1_bss_570);
                fn_1_B8E8(&lbl_1_bss_67C[arg0]);
                if (temp_r23_2 == 0) {
                    var_r25 = 0;
                } else {
                    lbl_1_bss_726 = 3;
                    lbl_1_bss_724 = fn_1_AF04(lbl_1_bss_726, sp8[lbl_1_bss_726]);
                    if ((s16) lbl_1_bss_724 > 4) {
                        var_r18 = lbl_1_bss_724 - 4;
                    } else {
                        var_r18 = 0;
                    }
                    lbl_1_bss_722 = var_r18;
                    temp_r3_3 = lbl_1_bss_724 - lbl_1_bss_722;
                    var_r28 = temp_r3_3;
                    fn_1_A464();
                    fn_1_A868(lbl_1_bss_726, lbl_1_bss_722);
                    fn_1_4724(&lbl_1_bss_67C[arg0]);
                    fn_1_AE4C(arg0);
                    var_r31_11 = 1;
loop_94:
                    if (var_r31_11 <= 0xA) {
                        temp_f31_6 = (f32) sin((3.141592653589793 * (90.0 * ((f64) var_r31_11 / 10.0))) / 180.0);
                        Hu3DModelScaleSet(lbl_1_bss_67C[arg0].headMdl, temp_f31_6, temp_f31_6, temp_f31_6);
                        HuPrcVSleep();
                        var_r31_11 += 1;
                        goto loop_94;
                    }
                    goto block_95;
                }
            }
        } else {
block_95:
            var_r27 = 0xA;
            goto block_96;
        }
    } else {
        goto block_96;
    }
    fn_1_4724(&lbl_1_bss_67C[arg0]);
    HuPrcKill(lbl_1_bss_678);
    if (var_r25 == 0) {
        if (arg1 == 0) {
            HuAudSStreamPause(0, 1);
        }
        temp_r29_2 = &GwSdCommon.machine[arg0];
        var_r31_12 = 0;
loop_108:
        if (var_r31_12 < 4) {
            temp_r29_2->partNo[var_r31_12] = (u8) lbl_1_bss_6D4[var_r31_12];
            var_r31_12 += 1;
            goto loop_108;
        }
        temp_r29_2->charNo = 0x40;
        strcpy(temp_r29_2->name, lbl_1_bss_67C[arg0].name);
        fn_1_44E4(&lbl_1_bss_67C[arg0], lbl_1_bss_6D4[0], lbl_1_bss_6D4[1], lbl_1_bss_6D4[3], lbl_1_bss_6D4[4]);
        fn_1_4638(&lbl_1_bss_67C[arg0], 4);
        temp_f30 = (f32) (1450.0 * -cos((3.141592653589793 * (f64) ((arg0 * 0x3C) + 0x3C)) / 180.0));
        fn_1_4534(&lbl_1_bss_67C[arg0], (f32) (1450.0 * -sin((3.141592653589793 * (f64) ((arg0 * 0x3C) + 0x3C)) / 180.0)), 0.0f, temp_f30);
        HuAudFXPlay(0x50A);
        var_r31_13 = 1;
loop_111:
        if (var_r31_13 <= 0x1E) {
            temp_f31_7 = (f32) sin((3.141592653589793 * (90.0 * ((f64) var_r31_13 / 30.0))) / 180.0);
            Hu3DModelScaleSet(lbl_1_bss_67C[arg0].headMdl, temp_f31_7, 1.0f, temp_f31_7);
            Hu3DModelRotSet(lbl_1_bss_67C[arg0].headMdl, 0.0f, (f32) ((600.0 * (1.0 - (f64) temp_f31_7)) + (f64) (((arg0 + 1) * 0x3C) + 0x3C)), 0.0f);
            HuPrcVSleep();
            var_r31_13 += 1;
            goto loop_111;
        }
    }
    fn_1_4E14();
    fn_1_A63C();
    HuDataDirClose(0xBC0000);
    if (var_r25 != 0) {
        return 0;
    }
    return 1;
}

void fn_1_9FAC(s16 arg0) {
    s16 sp8;
    ANIMDATA *temp_r27;
    s16 temp_r3;
    s16 temp_r3_2;
    s16 temp_r3_3;
    s16 temp_r3_4;
    s16 temp_r3_5;
    s16 temp_r3_6;
    s16 temp_r3_7;
    s16 temp_r3_8;
    s16 var_r28;

    sp8 = arg0;
    winPrio = 0x3E8;
    temp_r3 = HuWinCreate(16.0f, 32.0f, 0x140, 0x28, 0);
    lbl_1_bss_728[0] = temp_r3;
    HuWinBGTPLvlSet(temp_r3, 0.0f);
    HuWinMesSpeedSet(temp_r3, 0);
    HuWinDispOff(temp_r3);
    temp_r3_2 = HuWinCreate(16.0f, 354.0f, 0x140, 0x58, 0);
    lbl_1_bss_728[2] = temp_r3_2;
    HuWinBGTPLvlSet(temp_r3_2, 0.0f);
    HuWinMesSpeedSet(temp_r3_2, 0);
    HuWinMesSet(temp_r3_2, 0x20019U);
    HuWinDispOff(lbl_1_bss_728[2]);
    temp_r3_3 = HuWinExCreate(16.0f, 72.0f, 0xC0, 0xC0, -1);
    lbl_1_bss_728[3] = temp_r3_3;
    HuWinDrawNoSet(temp_r3_3, 0x40);
    HuWinMesSpeedSet(temp_r3_3, 0);
    temp_r3_4 = HuWinExCreate(214.0f, 72.0f, 0x15A, 0xD0, -1);
    lbl_1_bss_728[4] = temp_r3_4;
    HuWinMesSpeedSet(temp_r3_4, 0);
    temp_r3_5 = HuWinCreate(332.0f, 240.0f, 0x7E, 0x30, 0);
    lbl_1_bss_728[5] = temp_r3_5;
    HuWinBGTPLvlSet(temp_r3_5, 0.0f);
    HuWinMesSpeedSet(temp_r3_5, 0);
    fn_1_A704(temp_r3_5, lbl_1_bss_726);
    HuWinDispOff(lbl_1_bss_728[5]);
    temp_r3_6 = HuWinExCreate(214.0f, 280.0f, 0x15A, 0x92, -1);
    lbl_1_bss_728[6] = temp_r3_6;
    HuWinMesSpeedSet(temp_r3_6, 0);
    var_r28 = 0;
loop_2:
    if (var_r28 < 0xA) {
        temp_r3_7 = HuWinCreate(0.0f, 0.0f, 0x196, 0x28, 0);
        lbl_1_bss_736[var_r28] = temp_r3_7;
        HuWinBGTPLvlSet(temp_r3_7, 0.0f);
        HuWinMesSpeedSet(temp_r3_7, 0);
        var_r28 += 1;
        goto loop_2;
    }
    temp_r27 = HuSprAnimRead(HuDataSelHeapReadNum(0xE70002, 0x10000000, HEAP_MODEL));
    temp_r3_8 = HuSprGrpCreate(3);
    lbl_1_bss_71A = temp_r3_8;
    HuSprGrpMemberSet(temp_r3_8, 0, HuSprCreate(temp_r27, 0x64, 0x13));
    HuSprPosSet(temp_r3_8, 0, 320.0f, 240.0f);
    HuSprScaleSet(temp_r3_8, 0, 1.0f, -1.0f);
    HuSprGrpMemberSet(temp_r3_8, 1, HuSprCreate(temp_r27, 0x64, 0x13));
    HuSprPosSet(temp_r3_8, 1, 320.0f, 240.0f);
    HuSprGrpMemberSet(temp_r3_8, 2, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(0xE70001, 0x10000000, HEAP_MODEL)), 0x64, 0));
    HuSprPosSet(temp_r3_8, 2, 320.0f, 240.0f);
    HuSprGrpDrawNoSet(temp_r3_8, 0);
    lbl_1_bss_678 = HuPrcChildCreate(fn_1_88BC, 0x3E8U, 0x1000U, 0, HuPrcCurrentGet());
    lbl_1_bss_678->property = &lbl_1_bss_67C[sp8];
    HuSprGrpPosSet(lbl_1_bss_71A, -1000.0f, 0.0f);
}

void fn_1_A464(void) {
    s16 var_r31;

    HuSprAttrSet(lbl_1_bss_71A, 2, 4);
    HuWinExOpen(lbl_1_bss_728[3]);
    HuWinExOpen(lbl_1_bss_728[4]);
    HuWinExOpen(lbl_1_bss_728[6]);
    HuWinDispOn(lbl_1_bss_728[0]);
    HuWinDispOn(lbl_1_bss_728[2]);
    HuWinDispOn(lbl_1_bss_728[5]);
    var_r31 = 0;
loop_2:
    if (var_r31 < 0xA) {
        HuWinDispOn(lbl_1_bss_736[var_r31]);
        var_r31 += 1;
        goto loop_2;
    }
    HuSprGrpPosSet(lbl_1_bss_71A, 0.0f, 0.0f);
}

void fn_1_A55C(void) {
    s16 var_r31;

    HuWinDispOff(lbl_1_bss_728[0]);
    HuWinDispOff(lbl_1_bss_728[2]);
    HuWinDispOff(lbl_1_bss_728[5]);
    var_r31 = 0;
loop_2:
    if (var_r31 < 0xA) {
        HuWinDispOff(lbl_1_bss_736[var_r31]);
        var_r31 += 1;
        goto loop_2;
    }
    HuSprGrpPosSet(lbl_1_bss_71A, -1000.0f, 0.0f);
    HuWinExClose(lbl_1_bss_728[3]);
    HuWinExClose(lbl_1_bss_728[4]);
    HuWinExClose(lbl_1_bss_728[6]);
}

void fn_1_A63C(void) {
    s16 var_r31;

    HuWinKill(lbl_1_bss_728[0]);
    HuWinKill(lbl_1_bss_728[2]);
    HuWinKill(lbl_1_bss_728[3]);
    HuWinKill(lbl_1_bss_728[4]);
    HuWinKill(lbl_1_bss_728[5]);
    HuWinKill(lbl_1_bss_728[6]);
    var_r31 = 0;
loop_2:
    if (var_r31 < 0xA) {
        HuWinKill(lbl_1_bss_736[var_r31]);
        var_r31 += 1;
        goto loop_2;
    }
    HuSprGrpKill(lbl_1_bss_71A);
}

void fn_1_A704(s16 arg0, s16 arg1) {
    s16 var_r30;
    s16 var_r31;

    if (arg1 != 4) {
        var_r31 = 0;
        var_r30 = 0;
loop_5:
        if (var_r30 < (s16) SdMachinePartNum[arg1]) {
            if ((s32) (GwSdCommon.partFlag[arg1] & (1 << var_r30)) != 0) {
                var_r31 += 1;
            }
            var_r30 += 1;
            goto loop_5;
        }
    } else {
        var_r31 = 0xB;
    }
    sprintf(lbl_1_bss_71C, "%02d", var_r31);
    sprintf(lbl_1_bss_71C + 3, "%02d", SdMachinePartNum[arg1]);
    HuWinInsertMesSet(arg0, MESSNUM_PTR(lbl_1_bss_71C), 0);
    HuWinInsertMesSet(arg0, MESSNUM_PTR(lbl_1_bss_71C + 3), 1);
    HuWinMesSet(arg0, 0x19002CU);
    HuWinScaleSet(arg0, 0.8f, 0.8f);
}

void fn_1_A868(s16 arg0, s16 arg1) {
    s16 var_r27;
    s16 var_r31;
    s16 var_r31_2;
    s16 var_r31_3;
    SDMACHINE_PART *temp_r30;

    temp_r30 = SdMachinePartTbl[arg0];
    var_r31 = 0;
    lbl_1_bss_6FE = 0;
loop_4:
    if (var_r31 < (s16) SdMachinePartNum[arg0]) {
        if (fn_1_3EC0(arg0, var_r31) != 0) {
            lbl_1_bss_6DE[lbl_1_bss_6FE] = var_r31;
            lbl_1_bss_6FE += 1;
        }
        var_r31 += 1;
        goto loop_4;
    }
    var_r31_2 = 0;
loop_10:
    if (var_r31_2 < 5) {
        if ((s32) (var_r31_2 + arg1) >= (s16) lbl_1_bss_6FE) {
            HuWinHomeClear(lbl_1_bss_736[var_r31_2]);
        } else {
            HuWinMesSet(lbl_1_bss_736[var_r31_2], temp_r30[lbl_1_bss_6DE[var_r31_2 + arg1]].nameMes);
        }
        var_r31_2 += 1;
        goto loop_10;
    }
    if ((s32) (arg1 + 5) < (s16) lbl_1_bss_6FE) {
        HuSprAttrReset(lbl_1_bss_71A, 1, 4);
    } else {
        HuSprAttrSet(lbl_1_bss_71A, 1, 4);
    }
    if (arg1 > 0) {
        HuSprAttrReset(lbl_1_bss_71A, 0, 4);
    } else {
        HuSprAttrSet(lbl_1_bss_71A, 0, 4);
    }
    if ((s16) lbl_1_bss_724 >= 0) {
        HuSprAttrReset(lbl_1_bss_71A, 2, 4);
    } else {
        HuSprAttrSet(lbl_1_bss_71A, 2, 4);
    }
    HuSprPosSet(lbl_1_bss_71A, 2, 10.0f + lbl_1_bss_704, 8.0f + (32.0f + (lbl_1_bss_700 + (32.0f * (f32) (lbl_1_bss_724 - arg1)))));
    var_r27 = 0;
    var_r31_3 = 0;
loop_25:
    if (var_r31_3 < (s16) SdMachinePartNum[arg0]) {
        if (fn_1_3EC0(arg0, var_r31_3) != 0) {
            if (var_r27 != (s16) lbl_1_bss_724) {
                var_r27 += 1;
                goto block_24;
            }
        } else {
block_24:
            var_r31_3 += 1;
            goto loop_25;
        }
    }
    HuWinMesSet(lbl_1_bss_728[6], SdMachinePartTbl[arg0][var_r31_3].descMes);
}

void fn_1_AC08(float farg0, float farg1) {
    s16 var_r31;

    var_r31 = 0;
loop_2:
    if (var_r31 < 5) {
        HuWinPosSet(lbl_1_bss_736[var_r31], 16.0f + farg0, 16.0f + (farg1 + (f32) (var_r31 << 5)));
        var_r31 += 1;
        goto loop_2;
    }
    HuWinPosSet(lbl_1_bss_728[4], farg0, farg1);
    HuWinPosSet(lbl_1_bss_728[5], 15.0f + (240.0f + farg0), 16.0f + (160.0f + farg1));
    HuSprPosSet(lbl_1_bss_71A, 0, 173.0f + farg0, 16.0f + farg1);
    HuSprPosSet(lbl_1_bss_71A, 1, 173.0f + farg0, 32.0f + (160.0f + farg1));
    HuSprPosSet(lbl_1_bss_71A, 2, 10.0f + farg0, 8.0f + (32.0f + (farg1 + (f32) (lbl_1_bss_724 << 5))));
    lbl_1_bss_704 = farg0;
    lbl_1_bss_700 = farg1;
}

void fn_1_AE4C(s16 arg0) {
    fn_1_4724(&lbl_1_bss_67C[arg0]);
    if ((s16) lbl_1_bss_6D4[0] != -1) {
        fn_1_42E0(&lbl_1_bss_67C[arg0], lbl_1_bss_6D4[0], lbl_1_bss_6D4[1], lbl_1_bss_6D4[3], lbl_1_bss_6D4[4]);
        fn_1_4638(&lbl_1_bss_67C[arg0], 8);
    }
}

s16 fn_1_AF04(s16 arg0, s16 arg1) {
    s16 var_r29;
    s16 var_r30;
    s16 var_r31;

    var_r29 = arg1;
    if (var_r29 < 0) {
        var_r29 = 0;
    }
    if (arg0 == 4) {
        return var_r29;
    }
    var_r30 = 0;
    var_r31 = 0;
loop_9:
    if (var_r31 < (s16) SdMachinePartNum[arg0]) {
        if (var_r31 != var_r29) {
            if (fn_1_3EC0(arg0, var_r31) != 0) {
                var_r30 += 1;
            }
            var_r31 += 1;
            goto loop_9;
        }
    }
    return var_r30;
}

void fn_1_AFCC(SDMACHINE_DISP *arg0, u32 arg1) {
    s16 pts[5];
    HuVecF sp14;
    HuVecF sp8;
    f32 temp_f31;
    f64 temp_f1_2;
    s16 temp_r3;
    s16 temp_r3_2;
    s16 temp_r3_3;
    s16 temp_r3_4;
    s16 temp_r3_5;
    s16 temp_r3_6;
    s16 var_r28;
    s16 var_r30;
    s16 var_r30_2;
    s32 temp_r5;

    temp_r3 = HuWinExCreateFrame(16.0f, 32.0f, 0x220, 0x136, -1, 0);
    lbl_1_bss_5F6[0] = temp_r3;
    HuWinMesSpeedSet(temp_r3, 0);
    HuWinPriSet(temp_r3, 0x32);
    HuWinDrawNoSet(temp_r3, 0x40);
    HuWinExOpen(temp_r3);
    temp_r3_2 = HuWinCreate(48.0f, 304.0f, 0xC0, 0x2A, 0);
    lbl_1_bss_5F6[1] = temp_r3_2;
    HuWinMesSpeedSet(temp_r3_2, 0);
    HuWinPriSet(temp_r3_2, 0x28);
    HuWinAttrSet(temp_r3_2, 0x800U);
    HuWinBGTPLvlSet(temp_r3_2, 0.0f);
    HuWinMesSet(temp_r3_2, MESSNUM_PTR(arg0->name));
    temp_r3_3 = HuWinCreate(280.0f, 230.0f, 0x118, 0x78, 0);
    lbl_1_bss_5F6[2] = temp_r3_3;
    HuWinMesSpeedSet(temp_r3_3, 0);
    HuWinPriSet(temp_r3_3, 0x28);
    HuWinBGTPLvlSet(temp_r3_3, 0.0f);
    HuWinInsertMesSet(temp_r3_3, lbl_1_bss_6D4[0] + 0x1A0017, 0);
    HuWinInsertMesSet(temp_r3_3, lbl_1_bss_6D4[1] + 0x1A0001, 1);
    HuWinInsertMesSet(temp_r3_3, lbl_1_bss_6D4[2] + 0x1A000C, 2);
    HuWinInsertMesSet(temp_r3_3, lbl_1_bss_6D4[3] + 0x1A0022, 3);
    HuWinMesSet(temp_r3_3, 0x450008U);
    temp_r3_4 = HuWinCreate(-10000.0f, 352.0f, 0x220, 0x1A, 0);
    lbl_1_bss_5F6[3] = temp_r3_4;
    HuWinMesSpeedSet(temp_r3_4, 0);
    HuWinPriSet(temp_r3_4, 0x28);
    HuWinAttrSet(temp_r3_4, 0x800U);
    HuWinBGTPLvlSet(temp_r3_4, 0.0f);
    if (arg1 != 0) {
        HuWinMesSet(temp_r3_4, arg1);
    }
    HuDataDirClose(0xBC0000);
    fn_1_C384(lbl_1_bss_6D4[0], lbl_1_bss_6D4[1], lbl_1_bss_6D4[2], lbl_1_bss_6D4[3], pts);
    lbl_1_bss_5E0[0] = pts[3];
    lbl_1_bss_5E0[1] = pts[1];
    temp_r5 = pts[2] - lbl_1_data_11C4[4];
    lbl_1_bss_5E0[2] = lbl_1_data_11C4[4] + ((lbl_1_data_11C4[5] - lbl_1_data_11C4[4]) - temp_r5);
    OSReport("Toruque:%d\n", lbl_1_bss_5E0[2]);
    lbl_1_bss_5E0[3] = lbl_1_data_11C4[6] + ((lbl_1_data_11C4[7] - lbl_1_data_11C4[6]) - (pts[0] - lbl_1_data_11C4[6]));
    lbl_1_bss_5E0[4] = pts[4];
    temp_r3_5 = HuSprGrpCreate(3);
    lbl_1_bss_5F4 = temp_r3_5;
    HuSprGrpMemberSet(temp_r3_5, 0, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(0xBE0041, 0x10000000, HEAP_MODEL)), 0x1E, 0));
    HuSprGrpMemberSet(temp_r3_5, 1, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(0xBE0042, 0x10000000, HEAP_MODEL)), 0x19, 0));
    HuSprAttrSet(temp_r3_5, 1, 8);
    if (((s16) lbl_1_bss_5E0[0] < -0x32) || ((s16) lbl_1_bss_5E0[1] < -0x32) || ((s16) lbl_1_bss_5E0[2] < -0x32) || ((s16) lbl_1_bss_5E0[3] < -0x32) || ((s16) lbl_1_bss_5E0[4] < -0x32)) {
        var_r28 = HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(0xBE0044, 0x10000000, HEAP_MODEL)), 0x18, 0);
    } else {
        var_r28 = HuSprFuncCreate(fn_1_BAA0, 0x1C);
    }
    HuSprGrpMemberSet(temp_r3_5, 2, var_r28);
    HuSprGrpPosSet(temp_r3_5, 424.0f, 151.0f);
    HuSprGrpScaleSet(temp_r3_5, 1.0f, 1.0f);
    HuSprGrpDrawNoSet(temp_r3_5, 0x40);
    lbl_1_bss_5DC = HuSprAnimRead(HuDataSelHeapReadNum(0xBE0043, 0x10000000, HEAP_MODEL));
    var_r30 = 0;
loop_11:
    if (var_r30 < 5) {
        temp_r3_6 = Hu3DModelCreate(HuDataSelHeapReadNum(var_r30 + 0xBE0045, 0x10000000, HEAP_MODEL));
        lbl_1_bss_5EA[var_r30] = temp_r3_6;
        Hu3DModelCameraSet(temp_r3_6, 4U);
        sp14.x = (f32) (424.0 + (f64) lbl_1_data_1174[var_r30].x + (f64) lbl_1_data_119C[var_r30].x);
        temp_f1_2 = 151.0 + (f64) lbl_1_data_1174[var_r30].y;
        sp14.y = (f32) (temp_f1_2 + (f64) lbl_1_data_119C[var_r30].y);
        sp14.z = 500.0f;
        Hu3D2Dto3D(&sp14, 4, &sp8);
        Hu3DModelLayerSet(temp_r3_6, 6);
        Hu3DModelPosSetV(temp_r3_6, &sp8);
        Hu3DModelRotSet(temp_r3_6, 0.0f, (f32) ((sdWorkInt[7] + 1) * 0x3C), 0.0f);
        Hu3DModelScaleSet(temp_r3_6, 1.3f, 1.3f, 1.3f);
        Hu3DModelAttrSet(temp_r3_6, 0x40000001U);
        var_r30 += 1;
        goto loop_11;
    }
    HuDataDirClose(0xBE0000);
    fn_1_4724(arg0);
    fn_1_44E4(arg0, lbl_1_bss_6D4[0], lbl_1_bss_6D4[1], lbl_1_bss_6D4[3], lbl_1_bss_6D4[4]);
    fn_1_4638(arg0, 8);
    fn_1_4590(arg0, 0.0f, 45.0f, 0.0f);
    lbl_1_bss_620 = Hu3DCamera[3];
    omCameraViewSetMulti(8, &lbl_1_data_1120);
    Hu3DCameraViewportSet(8, -42.222218f, 34.0f, 400.0f, 300.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(8, 0x16U, 0x22U, 0x137U, 0x110U);
    var_r30_2 = 1;
loop_14:
    if (var_r30_2 <= 0xA) {
        temp_f31 = (f32) sin((3.141592653589793 * (90.0 * ((f64) var_r30_2 / 10.0))) / 180.0);
        Hu3DModelScaleSet(arg0->headMdl, temp_f31, temp_f31, temp_f31);
        HuPrcVSleep();
        var_r30_2 += 1;
        goto loop_14;
    }
}

void fn_1_B8E8(SDMACHINE_DISP *arg0) {
    f32 temp_f31;
    s16 var_r31;
    s16 var_r31_2;

    var_r31 = 1;
loop_2:
    if (var_r31 <= 0xA) {
        temp_f31 = (f32) cos((3.141592653589793 * (90.0 * ((f64) var_r31 / 10.0))) / 180.0);
        Hu3DModelScaleSet(arg0->headMdl, temp_f31, temp_f31, temp_f31);
        HuPrcVSleep();
        var_r31 += 1;
        goto loop_2;
    }
    HuSprGrpKill(lbl_1_bss_5F4);
    var_r31_2 = 0;
loop_5:
    if (var_r31_2 < 5) {
        Hu3DModelKill(lbl_1_bss_5EA[var_r31_2]);
        var_r31_2 += 1;
        goto loop_5;
    }
    HuSprAnimKill(lbl_1_bss_5DC);
    HuWinKill(lbl_1_bss_5F6[1]);
    HuWinKill(lbl_1_bss_5F6[2]);
    HuWinKill(lbl_1_bss_5F6[3]);
    HuWinExClose(lbl_1_bss_5F6[0]);
    HuWinKill(lbl_1_bss_5F6[0]);
    Hu3DCamera[3] = lbl_1_bss_620;
}

/* sprite callback: draws the five-corner stat graph as a fan of triangles */
void fn_1_BAA0(HUSPRITE *arg0)
{
    GXColor amb;
    GXColor mat;
    GXColor tev;
    Mtx mtx;
    HuVec2f pt;
    f32 range;
    f32 ratio;
    s16 i;
    s16 next;

    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, GX_FALSE, 0x7D);
    mat.r = 0xFD;
    mat.g = 0xFF;
    mat.b = 0;
    mat.a = 0xFF;
    amb.r = 0xFD;
    amb.g = 0xFF;
    amb.b = 0;
    amb.a = 0xFF;
    GXSetChanAmbColor(GX_COLOR0A0, amb);
    GXSetChanMatColor(GX_COLOR0A0, mat);
    mat.a = (u8)(s32)arg0->a;
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    tev = mat;
    GXSetTevColor(GX_TEVREG0, tev);
    GXSetNumTevStages(1);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT0, GX_DF_CLAMP, GX_AF_SPOT);
    if (arg0->attr & HUSPR_ATTR_ADDCOL) {
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_NOOP);
    } else if (arg0->attr & HUSPR_ATTR_NOANIM) {
        GXSetBlendMode(GX_BM_BLEND, GX_BL_ZERO, GX_BL_INVDSTCLR, GX_LO_NOOP);
    } else {
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    }
    GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
    GXSetZCompLoc(GX_FALSE);
    PSMTXScale(mtx, arg0->scale.x, arg0->scale.y, 1.0f);
    mtxTransCat(mtx, arg0->pos.x, arg0->pos.y, 0.0f);
    PSMTXConcat(*arg0->groupMtx, mtx, mtx);
    GXLoadPosMtxImm(mtx, 0);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    HuSprTexLoad(lbl_1_bss_5DC, 0, GX_TEXMAP0, GX_CLAMP, GX_CLAMP, GX_LINEAR);
    for (i = 0; i < 5; i++) {
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 3);
        GXPosition3f32(0.0f, 8.0f, 0.0f);
        GXTexCoord2f32(0.0f, 0.0f);
        range = lbl_1_data_11C4[i * 2 + 1] - lbl_1_data_11C4[i * 2];
        ratio = lbl_1_bss_5E0[i] - lbl_1_data_11C4[i * 2];
        pt.x = (f32)((0.1 * lbl_1_data_1174[i].x) + (0.9 * lbl_1_data_1174[i].x * (f64)(ratio / range)));
        pt.y = (f32)((0.1 * lbl_1_data_1174[i].y) + (0.9 * lbl_1_data_1174[i].y * (f64)(ratio / range)));
        GXPosition3f32(pt.x, 8.0f + pt.y, 0.0f);
        GXTexCoord2f32(0.0f, 1.0f);
        next = (i < 4) ? i + 1 : 0;
        range = lbl_1_data_11C4[next * 2 + 1] - lbl_1_data_11C4[next * 2];
        ratio = lbl_1_bss_5E0[next] - lbl_1_data_11C4[next * 2];
        pt.x = (f32)((0.1 * lbl_1_data_1174[next].x) + (0.9 * lbl_1_data_1174[next].x * (f64)(ratio / range)));
        pt.y = (f32)((0.1 * lbl_1_data_1174[next].y) + (0.9 * lbl_1_data_1174[next].y * (f64)(ratio / range)));
        GXPosition3f32(pt.x, 8.0f + pt.y, 0.0f);
        GXTexCoord2f32(1.0f, 1.0f);
    }
}

/* No effect; the original keeps a frame with dead locals, hence the self-assignments. */
void fn_1_C360(s16 arg0, s16 arg1)
{
    s32 scratch0;
    s32 scratch1;
    f64 scratch2;

    scratch0 = scratch0;
    scratch1 = scratch1;
    scratch2 = scratch2;
}

/* sum the five stat points (out[0..4]) of a head/body/tail/item combination */
void fn_1_C384(s16 headNo, s16 bodyNo, s16 tailNo, s16 itemNo, s16 *out)
{
    SDMACHINE_PART *head = &SdMachinePartTbl[0][headNo];
    SDMACHINE_PART *body = &SdMachinePartTbl[1][bodyNo];
    SDMACHINE_PART *tail = &SdMachinePartTbl[2][tailNo];
    SDMACHINE_PART *item = &SdMachinePartTbl[3][itemNo];

    out[0] = body->unkD + tail->unkD + 5;
    out[1] = body->unkE + head->unkD + tail->unkE + item->unkE + 5;
    out[2] = body->unkF + tail->unkF + 5;
    out[3] = head->unkF + item->unkD + 4;
    out[4] = head->unkE + 5;
    OSReport("Point %d,%d,%d,%d,%d\n", out[0], out[1], out[2], out[3], out[4]);
}

void fn_1_C538(s16 arg0) {
    s16 sp8[3];
    GWSDMACHINE *temp_r27;
    f32 temp_f31;
    f32 temp_f31_10;
    f32 temp_f31_11;
    f32 temp_f31_12;
    f32 temp_f31_13;
    f32 temp_f31_14;
    f32 temp_f31_15;
    f32 temp_f31_16;
    f32 temp_f31_17;
    f32 temp_f31_2;
    f32 temp_f31_3;
    f32 temp_f31_4;
    f32 temp_f31_5;
    f32 temp_f31_6;
    f32 temp_f31_7;
    f32 temp_f31_8;
    f32 temp_f31_9;
    s16 var_r25;
    s16 var_r29;
    s16 var_r29_2;
    s16 var_r30;
    s16 var_r31;
    s16 var_r31_10;
    s16 var_r31_11;
    s16 var_r31_12;
    s16 var_r31_13;
    s16 var_r31_14;
    s16 var_r31_2;
    s16 var_r31_3;
    s16 var_r31_4;
    s16 var_r31_5;
    s16 var_r31_6;
    s16 var_r31_7;
    s16 var_r31_8;
    s16 var_r31_9;

    if (((s32) omovlevtno == 3) || ((s32) omovlevtno == 2)) {
        HuSprPosSet(lbl_1_bss_718, 4, 224.0f, 128.0f);
        HuSprScaleSet(lbl_1_bss_718, 4, 0.0f, 0.0f);
        HuSprPosSet(lbl_1_bss_718, 5, 352.0f, 128.0f);
        HuSprScaleSet(lbl_1_bss_718, 5, 0.0f, 0.0f);
        if ((s32) omovlevtno == 3) {
            var_r25 = 1;
        } else {
            var_r25 = 0;
        }
        var_r30 = var_r25;
        omovlevtno = 0;
        HuWinDispOn(lbl_1_bss_606);
        HuWinMesSet(lbl_1_bss_606, var_r30 + 0x190015);
        HuWinMesWait(lbl_1_bss_606);
        HuSprPosSet(lbl_1_bss_718, 4, 224.0f, 128.0f);
        HuSprPosSet(lbl_1_bss_718, 5, 352.0f, 128.0f);
        if (var_r30 == 0) {
            HuSprScaleSet(lbl_1_bss_718, 4, 1.0f, 1.0f);
            HuSprScaleSet(lbl_1_bss_718, 5, 0.8f, 0.8f);
        } else {
            HuSprScaleSet(lbl_1_bss_718, 4, 0.8f, 0.8f);
            HuSprScaleSet(lbl_1_bss_718, 5, 1.0f, 1.0f);
        }
        WipeCreate(1, 0, 0x3C);
loop_10:
        if (WipeCheckEnd() != 0) {
            HuPrcVSleep();
            goto loop_10;
        }
        var_r31 = 0;
loop_15:
        if (var_r31 < 2) {
            if (GwSdCommon.machine[var_r31].charNo != -1) {
                GwSdCommon.machine[var_r31].charNo = 0x40;
            }
            var_r31 += 1;
            goto loop_15;
        }
        if (GwSdBackup[GwSdBackupNo].machine.charNo != -1) {
            GwSdBackup[GwSdBackupNo].machine.charNo = 0x40;
        }
    } else {
        HuSprScaleSet(lbl_1_bss_718, 4, 0.8f, 0.8f);
        HuSprScaleSet(lbl_1_bss_718, 5, 0.8f, 0.8f);
        var_r31_2 = 1;
loop_20:
        if (var_r31_2 <= 0xA) {
            temp_f31 = (f32) (128.0 - (500.0 * (1.0 - sin((3.141592653589793 * (90.0 * ((f64) var_r31_2 / 10.0))) / 180.0))));
            HuSprPosSet(lbl_1_bss_718, 4, 224.0f, temp_f31);
            HuSprPosSet(lbl_1_bss_718, 5, 352.0f, temp_f31);
            HuPrcVSleep();
            var_r31_2 += 1;
            goto loop_20;
        }
        var_r30 = 0;
        var_r31_3 = 1;
loop_23:
        if (var_r31_3 <= 0xA) {
            temp_f31_2 = (f32) (0.8 + (0.2 * sin((3.141592653589793 * (90.0 * ((f64) var_r31_3 / 10.0))) / 180.0)));
            HuSprScaleSet(lbl_1_bss_718, 4, temp_f31_2, temp_f31_2);
            HuPrcVSleep();
            var_r31_3 += 1;
            goto loop_23;
        }
        HuWinMesSet(lbl_1_bss_606, 0x190015U);
        HuWinMesWait(lbl_1_bss_606);
    }
    HuWinMesSet(lbl_1_bss_604, 0x20004U);
    HuWinDispOn(lbl_1_bss_604);
loop_26:
    if (((s32) (fn_1_507C((u16) (1 << arg0)) & 1) != 0) && (var_r30 > 0)) {
        HuAudFXPlay(0);
        var_r31_4 = 1;
loop_30:
        if (var_r31_4 <= 0xA) {
            temp_f31_3 = (f32) (0.8 + (0.2 * cos((3.141592653589793 * (90.0 * ((f64) var_r31_4 / 10.0))) / 180.0)));
            HuSprScaleSet(lbl_1_bss_718, (s16) (var_r30 + 4), temp_f31_3, temp_f31_3);
            temp_f31_4 = (f32) (0.8 + (0.2 * sin((3.141592653589793 * (90.0 * ((f64) var_r31_4 / 10.0))) / 180.0)));
            HuSprScaleSet(lbl_1_bss_718, (s16) (var_r30 + 3), temp_f31_4, temp_f31_4);
            HuPrcVSleep();
            var_r31_4 += 1;
            goto loop_30;
        }
        var_r30 -= 1;
        HuWinMesSet(lbl_1_bss_606, var_r30 + 0x190015);
        HuWinMesWait(lbl_1_bss_606);
block_115:
        HuPrcVSleep();
        goto loop_26;
    }
    if (((s32) (fn_1_507C((u16) (1 << arg0)) & 2) != 0) && (var_r30 < 1)) {
        HuAudFXPlay(0);
        var_r31_5 = 1;
loop_36:
        if (var_r31_5 <= 0xA) {
            temp_f31_5 = (f32) (0.8 + (0.2 * cos((3.141592653589793 * (90.0 * ((f64) var_r31_5 / 10.0))) / 180.0)));
            HuSprScaleSet(lbl_1_bss_718, (s16) (var_r30 + 4), temp_f31_5, temp_f31_5);
            temp_f31_6 = (f32) (0.8 + (0.2 * sin((3.141592653589793 * (90.0 * ((f64) var_r31_5 / 10.0))) / 180.0)));
            HuSprScaleSet(lbl_1_bss_718, (s16) (var_r30 + 5), temp_f31_6, temp_f31_6);
            HuPrcVSleep();
            var_r31_5 += 1;
            goto loop_36;
        }
        var_r30 += 1;
        HuWinMesSet(lbl_1_bss_606, var_r30 + 0x190015);
        HuWinMesWait(lbl_1_bss_606);
        goto block_115;
    }
    if ((s32) (fn_1_4FD0((u16) (1 << arg0)) & 0x100) != 0) {
        HuWinDispOff(lbl_1_bss_604);
        HuAudFXPlay(1);
        if (var_r30 == 0) {
            var_r29 = 0;
            var_r31_6 = 0;
            if ((GwSdCommon.machine[0].charNo != -1) && (HuPadStatGet(0) != -1)) {
                var_r31_6 = 1;
                var_r29 = 1;
            }
            if ((GwSdCommon.machine[1].charNo != -1) && (HuPadStatGet(1) != -1)) {
                var_r31_6 += 1;
                var_r29 |= 2;
            }
            if (((s16) GwSdBackupNo != -1) && (HuPadStatGet(2) != -1)) {
                var_r31_6 += 1;
                var_r29 |= 4;
            }
            if (var_r31_6 >= 2) {
                var_r31_7 = 1;
loop_52:
                if (var_r31_7 <= 0xF) {
                    temp_f31_7 = (f32) cos((3.141592653589793 * (90.0 * ((f64) var_r31_7 / 15.0))) / 180.0);
                    HuSprScaleSet(lbl_1_bss_718, 5, (f32) (0.8 * (f64) temp_f31_7), (f32) (0.8 * (f64) temp_f31_7));
                    temp_f31_8 = (f32) sin((3.141592653589793 * (90.0 * ((f64) var_r31_7 / 15.0))) / 180.0);
                    HuSprPosSet(lbl_1_bss_718, 4, 224.0f + (64.0f * temp_f31_8), 128.0f + (-48.0f * temp_f31_8));
                    HuPrcVSleep();
                    var_r31_7 += 1;
                    goto loop_52;
                }
loop_53:
                HuWinMesSet(lbl_1_bss_606, 0x190073U);
                if (fn_1_12940(var_r29, sp8, fn_1_DD3C) != 0) {
                    HuWinExClose(lbl_1_bss_606);
                    var_r31_8 = 1;
loop_56:
                    if (var_r31_8 <= 0xA) {
                        temp_f31_9 = (f32) cos((3.141592653589793 * (90.0 * ((f64) var_r31_8 / 10.0))) / 180.0);
                        HuSprScaleSet(lbl_1_bss_718, 4, temp_f31_9, temp_f31_9);
                        HuPrcVSleep();
                        var_r31_8 += 1;
                        goto loop_56;
                    }
                    if (sp8[0] != -1) {
                        GwSdCommon.machine[0].charNo = (s8) sp8[0];
                    }
                    if (sp8[1] != -1) {
                        GwSdCommon.machine[1].charNo = (s8) sp8[1];
                    }
                    if (sp8[2] != -1) {
                        GwSdBackup[GwSdBackupNo].machine.charNo = (s8) sp8[2];
                    }
                    fn_1_DF94(arg0, (u8) var_r29);
                    if (sp8[0] != -1) {
                        GwSdCommon.machine[0].charNo = 0x40;
                    }
                    if (sp8[1] != -1) {
                        GwSdCommon.machine[1].charNo = 0x40;
                    }
                    if (sp8[2] != -1) {
                        GwSdBackup[GwSdBackupNo].machine.charNo = 0x40;
                    }
                    var_r31_9 = 1;
loop_71:
                    if (var_r31_9 <= 0xA) {
                        temp_f31_10 = (f32) sin((3.141592653589793 * (90.0 * ((f64) var_r31_9 / 10.0))) / 180.0);
                        HuSprScaleSet(lbl_1_bss_718, 4, temp_f31_10, temp_f31_10);
                        HuPrcVSleep();
                        var_r31_9 += 1;
                        goto loop_71;
                    }
                    HuWinExOpen(lbl_1_bss_606);
                    goto loop_53;
                }
                var_r31_10 = 1;
loop_75:
                if (var_r31_10 <= 0xA) {
                    temp_f31_11 = (f32) cos((3.141592653589793 * (90.0 * ((f64) var_r31_10 / 10.0))) / 180.0);
                    HuSprPosSet(lbl_1_bss_718, 4, 224.0f + (64.0f * temp_f31_11), 128.0f + (-48.0f * temp_f31_11));
                    temp_f31_12 = (f32) sin((3.141592653589793 * (90.0 * ((f64) var_r31_10 / 10.0))) / 180.0);
                    HuSprScaleSet(lbl_1_bss_718, 5, (f32) (0.8 * (f64) temp_f31_12), (f32) (0.8 * (f64) temp_f31_12));
                    HuPrcVSleep();
                    var_r31_10 += 1;
                    goto loop_75;
                }
                var_r30 = 0;
                HuWinMesSet(lbl_1_bss_606, 0x190015U);
                HuWinMesWait(lbl_1_bss_606);
            } else {
                var_r29_2 = 0;
                var_r31_11 = 0;
                if (GwSdCommon.machine[0].charNo != -1) {
                    var_r31_11 = 1;
                    var_r29_2 = 1;
                }
                if (GwSdCommon.machine[1].charNo != -1) {
                    var_r31_11 += 1;
                    var_r29_2 |= 2;
                }
                if ((s16) GwSdBackupNo != -1) {
                    var_r31_11 += 1;
                    var_r29_2 |= 4;
                }
                HuAudFXPlay(0x2F0);
                if (var_r31_11 <= 1) {
                    HuWinMesSet(lbl_1_bss_606, 0x190013U);
                } else {
                    if (arg0 == 0) {
                        if (var_r29_2 == 7) {
                            HuWinInsertMesSet(lbl_1_bss_606, 0x190083U, 0);
                        } else if (var_r29_2 == 3) {
                            HuWinInsertMesSet(lbl_1_bss_606, 0x190080U, 0);
                        } else {
                            HuWinInsertMesSet(lbl_1_bss_606, 0x190081U, 0);
                        }
                    } else if (var_r29_2 == 7) {
                        HuWinInsertMesSet(lbl_1_bss_606, 0x190082U, 0);
                    } else if (var_r29_2 == 3) {
                        HuWinInsertMesSet(lbl_1_bss_606, 0x19007FU, 0);
                    } else {
                        HuWinInsertMesSet(lbl_1_bss_606, 0x190081U, 0);
                    }
                    HuWinMesSet(lbl_1_bss_606, 0x19007EU);
                }
                HuWinMesWait(lbl_1_bss_606);
                HuWinMesSet(lbl_1_bss_606, var_r30 + 0x190015);
                HuWinMesWait(lbl_1_bss_606);
            }
        } else {
            var_r31_12 = 1;
loop_100:
            if (var_r31_12 <= 0xA) {
                temp_f31_13 = (f32) cos((3.141592653589793 * (90.0 * ((f64) var_r31_12 / 10.0))) / 180.0);
                HuSprScaleSet(lbl_1_bss_718, 4, (f32) (0.8 * (f64) temp_f31_13), (f32) (0.8 * (f64) temp_f31_13));
                temp_f31_14 = (f32) sin((3.141592653589793 * (90.0 * ((f64) var_r31_12 / 10.0))) / 180.0);
                HuSprPosSet(lbl_1_bss_718, 5, 352.0f + (-64.0f * temp_f31_14), 128.0f + (-48.0f * temp_f31_14));
                HuPrcVSleep();
                var_r31_12 += 1;
                goto loop_100;
            }
loop_101:
            temp_r27 = &GwSdCommon.machine[arg0];
            HuWinMesSet(lbl_1_bss_606, 0x190073U);
            if (fn_1_12940(1 << arg0, sp8, fn_1_DD3C) != 0) {
                if ((s16) sp8[arg0] != -1) {
                    temp_r27->charNo = (s8) sp8[arg0];
                }
                fn_1_113F8(arg0);
                goto loop_101;
            }
            var_r31_13 = 1;
loop_107:
            if (var_r31_13 <= 0xA) {
                temp_f31_15 = (f32) sin((3.141592653589793 * (90.0 * ((f64) var_r31_13 / 10.0))) / 180.0);
                HuSprScaleSet(lbl_1_bss_718, 4, (f32) (0.8 * (f64) temp_f31_15), (f32) (0.8 * (f64) temp_f31_15));
                temp_f31_16 = (f32) cos((3.141592653589793 * (90.0 * ((f64) var_r31_13 / 10.0))) / 180.0);
                HuSprPosSet(lbl_1_bss_718, 5, 352.0f + (-64.0f * temp_f31_16), 128.0f + (-48.0f * temp_f31_16));
                HuPrcVSleep();
                var_r31_13 += 1;
                goto loop_107;
            }
            temp_r27->charNo = 0x40;
            var_r30 = 1;
            HuWinMesSet(lbl_1_bss_606, 0x190016U);
            HuWinMesWait(lbl_1_bss_606);
        }
        HuWinMesSet(lbl_1_bss_604, 0x20004U);
        HuWinDispOn(lbl_1_bss_604);
        goto block_115;
    }
    if ((s32) (fn_1_4FD0((u16) (1 << arg0)) & 0x200) == 0) {
        goto block_115;
    }
    HuAudFXPlay(3);
    HuWinDispOff(lbl_1_bss_604);
    var_r31_14 = 1;
loop_113:
    if (var_r31_14 <= 0xA) {
        temp_f31_17 = (f32) (128.0 - (500.0 * (1.0 - cos((3.141592653589793 * (90.0 * ((f64) var_r31_14 / 10.0))) / 180.0))));
        HuSprPosSet(lbl_1_bss_718, 4, 224.0f, temp_f31_17);
        HuSprPosSet(lbl_1_bss_718, 5, 352.0f, temp_f31_17);
        HuPrcVSleep();
        var_r31_14 += 1;
        goto loop_113;
    }
}

void fn_1_DD3C(s16 *arg0, s16 arg1) {
    s16 sp8;
    GWSDMACHINE *temp_r30;
    f32 temp_f31;
    s16 temp_r31;
    s16 var_r29;

    sp8 = arg1;
    temp_r31 = (s16) sdWorkInt[7];
    temp_r30 = &GwSdCommon.machine[temp_r31];
    fn_1_4724(&lbl_1_bss_67C[temp_r31]);
    if (sp8 != 0) {
        var_r29 = *(arg0 + (temp_r31 * 2));
    } else {
        var_r29 = 0x40;
    }
    fn_1_44E4(&lbl_1_bss_67C[temp_r31], (s16) temp_r30->partNo[0], (s16) temp_r30->partNo[1], (s16) temp_r30->partNo[3], var_r29);
    fn_1_4638(&lbl_1_bss_67C[temp_r31], 4);
    temp_f31 = (f32) (1450.0 * -cos((3.141592653589793 * (f64) ((temp_r31 * 0x3C) + 0x3C)) / 180.0));
    fn_1_4534(&lbl_1_bss_67C[temp_r31], (f32) (1450.0 * -sin((3.141592653589793 * (f64) ((temp_r31 * 0x3C) + 0x3C)) / 180.0)), 0.0f, temp_f31);
    fn_1_4590(&lbl_1_bss_67C[temp_r31], 0.0f, (f32) (((temp_r31 + 1) * 0x3C) + 0x3C), 0.0f);
}

void fn_1_DF94(s16 arg0, u8 arg1) {
    SDMACHINE_DISP sp38[3];
    s16 sp28[7];
    GWSDMACHINE *sp1C[3];
    s16 sp14[3];
    s16 sp8;
    GWSDBACKUP *temp_r6_2;
    GWSDMACHINE *temp_r26;
    GWSDMACHINE *temp_r5;
    GWSDMACHINE *temp_r5_2;
    f32 temp_f29;
    f32 temp_f30;
    f32 temp_f30_2;
    f32 temp_f30_3;
    f32 temp_f31;
    f32 temp_f31_10;
    f32 temp_f31_11;
    f32 temp_f31_12;
    f32 temp_f31_13;
    f32 temp_f31_14;
    f32 temp_f31_15;
    f32 temp_f31_16;
    f32 temp_f31_17;
    f32 temp_f31_18;
    f32 temp_f31_19;
    f32 temp_f31_20;
    f32 temp_f31_2;
    f32 temp_f31_3;
    f32 temp_f31_4;
    f32 temp_f31_5;
    f32 temp_f31_6;
    f32 temp_f31_7;
    f32 temp_f31_8;
    f32 temp_f31_9;
    s16 temp_r29;
    s16 temp_r30;
    s16 temp_r3;
    s16 var_r28;
    s16 var_r28_2;
    s16 var_r28_3;
    s16 var_r28_4;
    s16 var_r29;
    s16 var_r29_2;
    s16 var_r29_3;
    s16 var_r29_4;
    s16 var_r30;
    s16 var_r30_10;
    s16 var_r30_11;
    s16 var_r30_12;
    s16 var_r30_13;
    s16 var_r30_14;
    s16 var_r30_15;
    s16 var_r30_16;
    s16 var_r30_17;
    s16 var_r30_18;
    s16 var_r30_2;
    s16 var_r30_3;
    s16 var_r30_4;
    s16 var_r30_5;
    s16 var_r30_6;
    s16 var_r30_7;
    s16 var_r30_8;
    s16 var_r30_9;
    s16 var_r31;
    s16 var_r31_10;
    s16 var_r31_11;
    s16 var_r31_12;
    s16 var_r31_13;
    s16 var_r31_14;
    s16 var_r31_15;
    s16 var_r31_16;
    s16 var_r31_17;
    s16 var_r31_18;
    s16 var_r31_19;
    s16 var_r31_20;
    s16 var_r31_21;
    s16 var_r31_22;
    s16 var_r31_23;
    s16 var_r31_24;
    s16 var_r31_25;
    s16 var_r31_26;
    s16 var_r31_27;
    s16 var_r31_28;
    s16 var_r31_29;
    s16 var_r31_2;
    s16 var_r31_30;
    s16 var_r31_31;
    s16 var_r31_32;
    s16 var_r31_33;
    s16 var_r31_3;
    s16 var_r31_4;
    s16 var_r31_5;
    s16 var_r31_6;
    s16 var_r31_7;
    s16 var_r31_8;
    s16 var_r31_9;
    s32 temp_r3_2;
    s32 temp_r6;
    s32 var_r22;
    s32 var_r23;

    /* Flowgraph is not reducible, falling back to gotos-only mode. */
    sp8 = 0;
    sdWorkInt[7] = (s32) arg0;
    var_r31 = 0;
    goto loop_5;
block_1:
    if (GwSdCommon.machine[var_r31].charNo == -1) {
        goto block_4;
    }
    if (arg0 == var_r31) {
        goto block_4;
    }
    fn_1_46C4(&lbl_1_bss_67C[var_r31], 0);
block_4:
    var_r31 += 1;
loop_5:
    if (var_r31 < 2) {
        goto block_1;
    }
    sp28[0] = HuWinCreate(24.0f, 64.0f, 0x100, 0x130, 0);
    HuWinMesSpeedSet(sp28[0], 0);
    HuWinDrawNoSet(sp28[0], 0x40);
    sp28[1] = HuWinCreate(296.0f, 64.0f, 0x100, 0x90, 0);
    HuWinMesSpeedSet(sp28[1], 0);
    HuWinDrawNoSet(sp28[1], 0x40);
    sp28[2] = HuWinCreate(296.0f, 226.0f, 0x100, 0x90, 0);
    HuWinMesSpeedSet(sp28[2], 0);
    HuWinDrawNoSet(sp28[2], 0x40);
    sp28[3] = HuWinCreate(-10000.0f, 380.0f, 0x230, 0x2A, 0);
    HuWinBGTPLvlSet(sp28[3], 0.0f);
    HuWinAttrSet(sp28[3], 0x800U);
    HuWinMesSpeedSet(sp28[3], 0);
    HuWinMesSet(sp28[3], 0x20004U);
    temp_r3 = HuWinCreate(24.0f, 64.0f, 0x100, 0x2A, 0);
    sp28[4] = temp_r3;
    HuWinMesSpeedSet(temp_r3, 0);
    HuWinBGTPLvlSet(temp_r3, 0.0f);
    HuWinAttrSet(temp_r3, 0x801U);
    sp28[5] = HuWinCreate(296.0f, 64.0f, 0x100, 0x2A, 0);
    HuWinMesSpeedSet(sp28[5], 0);
    HuWinBGTPLvlSet(sp28[5], 0.0f);
    HuWinAttrSet(sp28[5], 0x801U);
    sp28[6] = HuWinCreate(296.0f, 226.0f, 0x100, 0x2A, 0);
    HuWinMesSpeedSet(sp28[6], 0);
    HuWinBGTPLvlSet(sp28[6], 0.0f);
    HuWinAttrSet(sp28[6], 0x801U);
    HuSprPosSet(lbl_1_bss_718, 0xC, 152.0f, 216.0f);
    HuSprScaleSet(lbl_1_bss_718, 0xC, 32.0f, 38.0f);
    HuSprPosSet(lbl_1_bss_718, 0xD, 424.0f, 136.0f);
    HuSprScaleSet(lbl_1_bss_718, 0xD, 32.0f, 18.0f);
    HuSprPosSet(lbl_1_bss_718, 0xE, 424.0f, 298.0f);
    HuSprScaleSet(lbl_1_bss_718, 0xE, 32.0f, 18.0f);
    if (arg0 != 0) {
        goto block_8;
    }
    sp14[0] = 0;
    sp14[1] = 1;
    sp14[2] = 2;
    goto block_9;
block_8:
    sp14[0] = 1;
    sp14[1] = 0;
    sp14[2] = 2;
block_9:
    sp1C[0] = GwSdCommon.machine;
    sp1C[1] = &GwSdCommon.machine[1];
    if ((s16) GwSdBackupNo == -1) {
        goto block_11;
    }
    sp1C[2] = &GwSdBackup[GwSdBackupNo].machine;
    goto block_12;
block_11:
    sp1C[2] = &lbl_1_bss_608;
block_12:
    var_r31_2 = 0;
    goto loop_21;
block_13:
    temp_r26 = sp1C[sp14[var_r31_2]];
    if (temp_r26 == NULL) {
        goto block_19;
    }
    if ((s32) (arg1 & (1 << sp14[var_r31_2])) == 0) {
        goto block_19;
    }
    fn_1_44E4(&sp38[var_r31_2], (s16) temp_r26->partNo[0], (s16) temp_r26->partNo[1], (s16) temp_r26->partNo[3], (s16) temp_r26->charNo);
    fn_1_4638(&sp38[var_r31_2], (s16) (8 << var_r31_2));
    if (var_r31_2 <= 0) {
        goto block_17;
    }
    var_r23 = -1;
    goto block_18;
block_17:
    var_r23 = 1;
block_18:
    fn_1_4590(&sp38[var_r31_2], 0.0f, (f32) (var_r23 * 0x2D), 0.0f);
    fn_1_45E4(&sp38[var_r31_2], 0.9f, 0.9f, 0.9f);
    HuWinMesSet(sp28[var_r31_2 + 4], MESSNUM_PTR(temp_r26->name));
    goto block_20;
block_19:
    fn_1_480C(&sp38[var_r31_2]);
block_20:
    var_r31_2 += 1;
loop_21:
    if (var_r31_2 < 3) {
        goto block_13;
    }
    if ((s32) omovlevtno == 2) {
        goto loop_28;
    }
    sdWorkInt[1] = 0;
    if ((s32) (arg1 & (1 << sp14[1])) == 0) {
        goto block_25;
    }
    sdWorkInt[2] = 1;
block_25:
    if ((s32) (arg1 & (1 << sp14[1])) != 0) {
        goto block_27;
    }
    sdWorkInt[2] = 2;
block_27:
    sdWorkInt[3] = 0;
    sdWorkInt[4] = 0;
    sdWorkInt[5] = 0;
loop_28:
    Hu3DCameraPerspectiveSet(0x38, 5.0f, 20.0f, 15000.0f, 1.2f);
    omCameraViewSetMulti(8, &lbl_1_data_10E8);
    omCameraViewSetMulti(0x30, &lbl_1_data_1104);
    Hu3DCameraViewportSet(8, lbl_1_data_11FC[0][0], lbl_1_data_11FC[0][1], lbl_1_data_11FC[0][2], lbl_1_data_11FC[0][3], 0.0f, 1.0f);
    Hu3DCameraScissorSet(8, 0x1FU, 0x44U, 0x114U, 0x128U);
    Hu3DCameraViewportSet(0x10, lbl_1_data_11FC[1][0], lbl_1_data_11FC[1][1], lbl_1_data_11FC[1][2], lbl_1_data_11FC[1][3], 0.0f, 1.0f);
    Hu3DCameraScissorSet(0x10, 0x14AU, 0x42U, 0x11AU, 0x8AU);
    Hu3DCameraViewportSet(0x20, lbl_1_data_11FC[2][0], lbl_1_data_11FC[2][1], lbl_1_data_11FC[2][2], lbl_1_data_11FC[2][3], 0.0f, 1.0f);
    Hu3DCameraScissorSet(0x20, 0x14AU, 0xE4U, 0x11AU, 0x8AU);
    HuSprScaleSet(lbl_1_bss_718, 0xF, 1.0f, 1.0f);
    if ((s32) omovlevtno != 2) {
        goto block_51;
    }
    omovlevtno = 0;
    var_r30 = 0;
    goto loop_34;
block_30:
    HuSprAttrSet(lbl_1_bss_718, (s16) (var_r30 + 0xC), 4);
    HuWinBGTPLvlSet(sp28[var_r30], 0.0f);
    HuWinDispOff(sp28[var_r30 + 4]);
    if (var_r30 == (s32) sdWorkInt[1]) {
        goto block_33;
    }
    if (var_r30 == (s32) sdWorkInt[2]) {
        goto block_33;
    }
    fn_1_45E4(&sp38[var_r30], 0.0f, 0.0f, 0.0f);
    HuSprTPLvlSet(lbl_1_bss_718, (s16) (sp14[var_r30] + 9), 0.0f);
block_33:
    var_r30 += 1;
loop_34:
    if (var_r30 < 3) {
        goto block_30;
    }
    Hu3DCameraScissorSet(8, 0U, 0U, 0x280U, 0x1E0U);
    Hu3DCameraScissorSet(0x10, 0U, 0U, 0x280U, 0x1E0U);
    Hu3DCameraScissorSet(0x20, 0U, 0U, 0x280U, 0x1E0U);
    var_r30_2 = 0;
    goto loop_37;
block_36:
    temp_r3_2 = sdWorkInt[var_r30_2 + 1];
    Hu3DCameraViewportSet(8 << (s16) temp_r3_2, lbl_1_data_11FC[var_r30_2 + 3][0], lbl_1_data_11FC[var_r30_2 + 3][1], lbl_1_data_11FC[var_r30_2 + 3][2], lbl_1_data_11FC[var_r30_2 + 3][3], 0.0f, 1.0f);
    HuSprPosSet(lbl_1_bss_718, (s16) (sp14[(s16) temp_r3_2] + 9), lbl_1_data_1294[var_r30_2].x, lbl_1_data_1294[var_r30_2].y);
    var_r30_2 += 1;
loop_37:
    if (var_r30_2 < 2) {
        goto block_36;
    }
    HuSprPosSet(lbl_1_bss_718, 0xF, 288.0f, 180.0f);
    HuSprPosSet(lbl_1_bss_718, (s16) (sdWorkInt[3] + 6), 64.0f, 250.0f);
    HuSprPosSet(lbl_1_bss_718, (s16) (sdWorkInt[4] + 0x10), 130.0f, 250.0f);
    var_r31_3 = 0;
    goto loop_40;
block_39:
    HuSprPosSet(lbl_1_bss_716, var_r31_3, 288.0f, 250.0f);
    var_r31_3 += 1;
loop_40:
    if (var_r31_3 < 8) {
        goto block_39;
    }
    HuSprAttrReset(lbl_1_bss_716, (s16) sdWorkInt[5], 4);
    HuSprPosSet(lbl_1_bss_718, 0x12, 224.0f, 250.0f);
    HuSprPosSet(lbl_1_bss_718, 0x13, 352.0f, 250.0f);
    if ((s32) sdWorkInt[5] == 0) {
        goto block_43;
    }
    HuSprTPLvlSet(lbl_1_bss_718, 0x12, 1.0f);
    goto block_44;
block_43:
    HuSprTPLvlSet(lbl_1_bss_718, 0x12, 0.0f);
block_44:
    if ((s32) sdWorkInt[5] >= 7) {
        goto block_46;
    }
    HuSprTPLvlSet(lbl_1_bss_718, 0x13, 1.0f);
    goto block_47;
block_46:
    HuSprTPLvlSet(lbl_1_bss_718, 0x13, 0.0f);
block_47:
    HuWinPosSet(sp28[3], -10000.0f, 310.0f);
    HuWinDispOn(lbl_1_bss_606);
    WipeCreate(1, 0, 0x3C);
    goto loop_49;
block_48:
    HuPrcVSleep();
loop_49:
    if (WipeCheckEnd() != 0) {
        goto block_48;
    }
    goto block_224;
block_51:
    var_r31_4 = 0;
    goto loop_56;
block_52:
    if (var_r31_4 <= 0) {
        goto block_54;
    }
    var_r22 = -1;
    goto block_55;
block_54:
    var_r22 = 1;
block_55:
    fn_1_4590(&sp38[var_r31_4], 0.0f, (f32) (var_r22 * 0x2D), 0.0f);
    var_r31_4 += 1;
loop_56:
    if (var_r31_4 < 3) {
        goto block_52;
    }
    var_r30_3 = 0;
    goto loop_59;
block_58:
    HuWinDispOff(sp28[var_r30_3 + 4]);
    var_r30_3 += 1;
loop_59:
    if (var_r30_3 < 3) {
        goto block_58;
    }
    var_r31_5 = 1;
    goto loop_65;
block_61:
    temp_f31 = (f32) ((f64) var_r31_5 / 10.0);
    var_r30_4 = 0;
    goto loop_63;
block_62:
    HuWinBGTPLvlSet(sp28[var_r30_4], 1.0f);
    HuWinScaleSet(sp28[var_r30_4], 1.0f, temp_f31);
    HuSprPosSet(lbl_1_bss_718, (s16) (sp14[var_r30_4] + 9), lbl_1_data_127C[var_r30_4].x, lbl_1_data_127C[var_r30_4].y);
    HuSprScaleSet(lbl_1_bss_718, (s16) (sp14[var_r30_4] + 9), 1.0f, temp_f31);
    HuSprTPLvlSet(lbl_1_bss_718, (s16) (sp14[var_r30_4] + 9), 1.0f);
    fn_1_45E4(&sp38[var_r30_4], temp_f31, temp_f31, temp_f31);
    HuSprAttrSet(lbl_1_bss_718, (s16) (var_r30_4 + 0xC), 4);
    var_r30_4 += 1;
loop_63:
    if (var_r30_4 < 3) {
        goto block_62;
    }
    HuPrcVSleep();
    var_r31_5 += 1;
loop_65:
    if (var_r31_5 <= 0xA) {
        goto block_61;
    }
    var_r30_5 = 0;
    goto loop_68;
block_67:
    HuWinDispOn(sp28[var_r30_5 + 4]);
    var_r30_5 += 1;
loop_68:
    if (var_r30_5 < 3) {
        goto block_67;
    }
    var_r29 = (s16) sdWorkInt[2];
    var_r31_6 = 0;
    goto loop_71;
block_70:
    HuSprAttrReset(lbl_1_bss_718, (s16) (var_r31_6 + 0xC), 4);
    var_r31_6 += 1;
loop_71:
    if (var_r31_6 < 3) {
        goto block_70;
    }
    HuSprAttrSet(lbl_1_bss_718, 0xC, 4);
    HuSprAttrSet(lbl_1_bss_718, (s16) (var_r29 + 0xC), 4);
    goto loop_89;
block_73:
    if ((s32) (fn_1_4FD0((u16) (1 << arg0)) & 0x200) == 0) {
        goto block_75;
    }
    HuAudFXPlay(3);
    goto block_269;
block_75:
    var_r28 = -1;
    if ((s32) (fn_1_507C((u16) (1 << arg0)) & 8) == 0) {
        goto block_79;
    }
    if (var_r29 != 2) {
        goto block_79;
    }
    if ((s32) (arg1 & (1 << sp14[1])) == 0) {
        goto block_79;
    }
    var_r28 = 1;
    goto block_83;
block_79:
    if ((s32) (fn_1_507C((u16) (1 << arg0)) & 4) == 0) {
        goto block_83;
    }
    if (var_r29 != 1) {
        goto block_83;
    }
    if ((s32) (arg1 & (1 << sp14[2])) == 0) {
        goto block_83;
    }
    var_r28 = 2;
block_83:
    if (var_r28 == -1) {
        goto block_88;
    }
    HuAudFXPlay(0);
    var_r29 = var_r28;
    var_r31_7 = 0;
    goto loop_86;
block_85:
    HuSprAttrReset(lbl_1_bss_718, (s16) (var_r31_7 + 0xC), 4);
    var_r31_7 += 1;
loop_86:
    if (var_r31_7 < 3) {
        goto block_85;
    }
    HuSprAttrSet(lbl_1_bss_718, 0xC, 4);
    HuSprAttrSet(lbl_1_bss_718, (s16) (var_r29 + 0xC), 4);
block_88:
    HuPrcVSleep();
loop_89:
    if ((s32) (fn_1_4FD0((u16) (1 << arg0)) & 0x100) == 0) {
        goto block_73;
    }
    HuAudFXPlay(1);
    sdWorkInt[1] = 0;
    sdWorkInt[2] = (s32) var_r29;
    HuWinDispOff(sp28[3]);
    var_r30_6 = 0;
    goto loop_92;
block_91:
    HuSprAttrSet(lbl_1_bss_718, (s16) (var_r30_6 + 0xC), 4);
    HuWinDispOff(sp28[var_r30_6 + 4]);
    var_r30_6 += 1;
loop_92:
    if (var_r30_6 < 3) {
        goto block_91;
    }
    var_r31_8 = 1;
    goto loop_105;
block_94:
    temp_f31_2 = (f32) (1.0 - ((f64) var_r31_8 / 20.0));
    var_r30_7 = 0;
    goto loop_103;
block_95:
    HuWinBGTPLvlSet(sp28[var_r30_7], temp_f31_2);
    if (var_r30_7 == (s32) sdWorkInt[1]) {
        goto block_102;
    }
    if (var_r30_7 == (s32) sdWorkInt[2]) {
        goto block_102;
    }
    if (sp1C[var_r30_7]->charNo == -1) {
        goto block_101;
    }
    if (var_r30_7 != 0) {
        goto block_100;
    }
    fn_1_45E4(&sp38[var_r30_7], (f32) (0.9 * (f64) temp_f31_2), (f32) (0.9 * (f64) temp_f31_2), (f32) (0.9 * (f64) temp_f31_2));
    goto block_101;
block_100:
    fn_1_45E4(&sp38[var_r30_7], temp_f31_2, temp_f31_2, temp_f31_2);
block_101:
    HuSprTPLvlSet(lbl_1_bss_718, (s16) (sp14[var_r30_7] + 9), temp_f31_2);
block_102:
    var_r30_7 += 1;
loop_103:
    if (var_r30_7 < 3) {
        goto block_95;
    }
    HuPrcVSleep();
    var_r31_8 += 1;
loop_105:
    if (var_r31_8 <= 0x14) {
        goto block_94;
    }
    Hu3DCameraScissorSet(8, 0U, 0U, 0x280U, 0x1E0U);
    Hu3DCameraScissorSet(0x10, 0U, 0U, 0x280U, 0x1E0U);
    Hu3DCameraScissorSet(0x20, 0U, 0U, 0x280U, 0x1E0U);
    omCameraViewMoveMulti(0x38U, &lbl_1_data_113C, 0x14, 0);
    var_r31_9 = 1;
    goto loop_111;
block_107:
    temp_f31_3 = (f32) ((f64) var_r31_9 / 20.0);
    var_r30_8 = 0;
    goto loop_109;
block_108:
    temp_r6 = sdWorkInt[var_r30_8 + 1];
    Hu3DCameraViewportSet(8 << (s16) temp_r6, lbl_1_data_11FC[(s16) temp_r6][0] + (temp_f31_3 * (lbl_1_data_11FC[var_r30_8 + 3][0] - lbl_1_data_11FC[(s16) temp_r6][0])), lbl_1_data_11FC[(s16) temp_r6][1] + (temp_f31_3 * (lbl_1_data_11FC[var_r30_8 + 3][1] - lbl_1_data_11FC[(s16) temp_r6][1])), lbl_1_data_11FC[(s16) temp_r6][2] + (temp_f31_3 * (lbl_1_data_11FC[var_r30_8 + 3][2] - lbl_1_data_11FC[(s16) temp_r6][2])), lbl_1_data_11FC[(s16) temp_r6][3] + (temp_f31_3 * (lbl_1_data_11FC[var_r30_8 + 3][3] - lbl_1_data_11FC[(s16) temp_r6][3])), 0.0f, 1.0f);
    temp_f30 = lbl_1_data_127C[(s16) temp_r6].x;
    temp_f29 = lbl_1_data_127C[(s16) temp_r6].y;
    HuSprPosSet(lbl_1_bss_718, (s16) (sp14[(s16) temp_r6] + 9), temp_f30 + (temp_f31_3 * (lbl_1_data_1294[var_r30_8].x - temp_f30)), temp_f29 + (temp_f31_3 * (lbl_1_data_1294[var_r30_8].y - temp_f29)));
    var_r30_8 += 1;
loop_109:
    if (var_r30_8 < 2) {
        goto block_108;
    }
    HuSprPosSet(lbl_1_bss_718, 0xF, 288.0f, (f32) (180.0 - (300.0 * (1.0 - (f64) temp_f31_3))));
    HuPrcVSleep();
    var_r31_9 += 1;
loop_111:
    if (var_r31_9 <= 0x14) {
        goto block_107;
    }
    HuAudFXPlay(0x514);
    HuWinExOpen(lbl_1_bss_606);
block_113:
    var_r29_2 = (s16) sdWorkInt[3];
    var_r30_9 = 0;
    goto loop_115;
block_114:
    HuSprPosSet(lbl_1_bss_718, (s16) (var_r30_9 + 6), 188.0f + (f32) (var_r30_9 * 0x64), 250.0f);
    var_r30_9 += 1;
loop_115:
    if (var_r30_9 < 3) {
        goto block_114;
    }
    var_r31_10 = 1;
    goto loop_124;
block_117:
    temp_f31_4 = (f32) ((f64) var_r31_10 / 20.0);
    var_r30_10 = 0;
    goto loop_122;
block_118:
    if (var_r29_2 != var_r30_10) {
        goto block_120;
    }
    HuSprScaleSet(lbl_1_bss_718, (s16) (var_r30_10 + 6), temp_f31_4, temp_f31_4);
    goto block_121;
block_120:
    HuSprScaleSet(lbl_1_bss_718, (s16) (var_r30_10 + 6), (f32) (0.8 * (f64) temp_f31_4), (f32) (0.8 * (f64) temp_f31_4));
block_121:
    var_r30_10 += 1;
loop_122:
    if (var_r30_10 < 3) {
        goto block_118;
    }
    HuPrcVSleep();
    var_r31_10 += 1;
loop_124:
    if (var_r31_10 <= 0x14) {
        goto block_117;
    }
    HuWinPosSet(sp28[3], -10000.0f, 310.0f);
    HuWinDispOn(sp28[3]);
    HuWinMesSet(lbl_1_bss_606, var_r29_2 + 0x19001F);
    goto loop_158;
block_126:
    if ((s32) (fn_1_4FD0((u16) (1 << arg0)) & 0x200) == 0) {
        goto block_146;
    }
    HuAudFXPlay(3);
    HuWinExClose(lbl_1_bss_606);
    HuWinDispOff(sp28[3]);
    var_r31_11 = 1;
    goto loop_135;
block_128:
    temp_f31_5 = (f32) (1.0 - ((f64) var_r31_11 / 10.0));
    var_r30_11 = 0;
    goto loop_133;
block_129:
    if (var_r29_2 != var_r30_11) {
        goto block_131;
    }
    HuSprScaleSet(lbl_1_bss_718, (s16) (var_r30_11 + 6), temp_f31_5, temp_f31_5);
    goto block_132;
block_131:
    HuSprScaleSet(lbl_1_bss_718, (s16) (var_r30_11 + 6), (f32) (0.8 * (f64) temp_f31_5), (f32) (0.8 * (f64) temp_f31_5));
block_132:
    var_r30_11 += 1;
loop_133:
    if (var_r30_11 < 3) {
        goto block_129;
    }
    HuPrcVSleep();
    var_r31_11 += 1;
loop_135:
    if (var_r31_11 <= 0xA) {
        goto block_128;
    }
    var_r31_12 = 1;
    goto loop_144;
block_137:
    temp_f31_6 = (f32) (1.0 - ((f64) var_r31_12 / 10.0));
    HuSprScaleSet(lbl_1_bss_718, 0xF, temp_f31_6, temp_f31_6);
    var_r30_12 = 0;
    goto loop_142;
block_138:
    if (var_r30_12 == (s32) sdWorkInt[1]) {
        goto block_140;
    }
    if (var_r30_12 != (s32) sdWorkInt[2]) {
        goto block_141;
    }
block_140:
    HuSprScaleSet(lbl_1_bss_718, (s16) (sp14[var_r30_12] + 9), temp_f31_6, temp_f31_6);
    fn_1_45E4(&sp38[var_r30_12], temp_f31_6, temp_f31_6, temp_f31_6);
block_141:
    var_r30_12 += 1;
loop_142:
    if (var_r30_12 < 3) {
        goto block_138;
    }
    HuPrcVSleep();
    var_r31_12 += 1;
loop_144:
    if (var_r31_12 <= 0xA) {
        goto block_137;
    }
    HuSprPosSet(lbl_1_bss_718, 0xF, 0.0f, -1000.0f);
    HuWinPosSet(sp28[3], -10000.0f, 380.0f);
    HuWinDispOn(sp28[3]);
    goto loop_28;
block_146:
    var_r28_2 = 0;
    if ((s32) (fn_1_507C((u16) (1 << arg0)) & 1) == 0) {
        goto block_149;
    }
    if (var_r29_2 == 0) {
        goto block_149;
    }
    var_r28_2 = -1;
    goto block_152;
block_149:
    if ((s32) (fn_1_507C((u16) (1 << arg0)) & 2) == 0) {
        goto block_152;
    }
    if (var_r29_2 >= 2) {
        goto block_152;
    }
    var_r28_2 = 1;
block_152:
    if (var_r28_2 == 0) {
        goto block_157;
    }
    HuAudFXPlay(0);
    var_r31_13 = 1;
    goto loop_155;
block_154:
    temp_f31_7 = (f32) (1.0 - (0.2 * ((f64) var_r31_13 / 10.0)));
    HuSprScaleSet(lbl_1_bss_718, (s16) (var_r29_2 + 6), temp_f31_7, temp_f31_7);
    temp_f31_8 = (f32) (0.8 + (0.2 * ((f64) var_r31_13 / 10.0)));
    HuSprScaleSet(lbl_1_bss_718, (s16) (var_r29_2 + var_r28_2 + 6), temp_f31_8, temp_f31_8);
    HuPrcVSleep();
    var_r31_13 += 1;
loop_155:
    if (var_r31_13 <= 0xA) {
        goto block_154;
    }
    var_r29_2 += var_r28_2;
    HuWinMesSet(lbl_1_bss_606, var_r29_2 + 0x19001F);
block_157:
    HuPrcVSleep();
loop_158:
    if ((s32) (fn_1_4FD0((u16) (1 << arg0)) & 0x100) == 0) {
        goto block_126;
    }
    HuAudFXPlay(1);
    HuWinHomeClear(lbl_1_bss_606);
    sdWorkInt[3] = (s32) var_r29_2;
    var_r31_14 = 1;
    goto loop_167;
block_160:
    temp_f31_9 = (f32) ((f64) var_r31_14 / 10.0);
    var_r30_13 = 0;
    goto loop_165;
block_161:
    if (var_r29_2 == var_r30_13) {
        goto block_163;
    }
    HuSprScaleSet(lbl_1_bss_718, (s16) (var_r30_13 + 6), (f32) (0.8 * (1.0 - (f64) temp_f31_9)), (f32) (0.8 * (1.0 - (f64) temp_f31_9)));
    goto block_164;
block_163:
    temp_f30_2 = 188.0f + (f32) (var_r30_13 * 0x64);
    HuSprPosSet(lbl_1_bss_718, (s16) (var_r30_13 + 6), temp_f30_2 + ((64.0f - temp_f30_2) * temp_f31_9), 250.0f);
block_164:
    var_r30_13 += 1;
loop_165:
    if (var_r30_13 < 3) {
        goto block_161;
    }
    HuPrcVSleep();
    var_r31_14 += 1;
loop_167:
    if (var_r31_14 <= 0xA) {
        goto block_160;
    }
block_168:
    HuSprPosSet(lbl_1_bss_718, 0x10, 238.0f, 250.0f);
    HuSprPosSet(lbl_1_bss_718, 0x11, 338.0f, 250.0f);
    var_r29_3 = (s16) sdWorkInt[4];
    var_r31_15 = 1;
    goto loop_176;
block_169:
    temp_f31_10 = (f32) ((f64) var_r31_15 / 20.0);
    var_r30_14 = 0;
    goto loop_174;
block_170:
    if (var_r29_3 != var_r30_14) {
        goto block_172;
    }
    HuSprScaleSet(lbl_1_bss_718, (s16) (var_r30_14 + 0x10), temp_f31_10, temp_f31_10);
    goto block_173;
block_172:
    HuSprScaleSet(lbl_1_bss_718, (s16) (var_r30_14 + 0x10), (f32) (0.8 * (f64) temp_f31_10), (f32) (0.8 * (f64) temp_f31_10));
block_173:
    var_r30_14 += 1;
loop_174:
    if (var_r30_14 < 2) {
        goto block_170;
    }
    HuPrcVSleep();
    var_r31_15 += 1;
loop_176:
    if (var_r31_15 <= 0x14) {
        goto block_169;
    }
    HuWinMesSet(lbl_1_bss_606, var_r29_3 + 0x190044);
    goto loop_204;
block_178:
    if ((s32) (fn_1_4FD0((u16) (1 << arg0)) & 0x200) == 0) {
        goto block_192;
    }
    HuAudFXPlay(3);
    HuWinHomeClear(lbl_1_bss_606);
    var_r31_16 = 1;
    goto loop_187;
block_180:
    temp_f31_11 = (f32) (1.0 - ((f64) var_r31_16 / 10.0));
    var_r30_15 = 0;
    goto loop_185;
block_181:
    if (var_r29_3 != var_r30_15) {
        goto block_183;
    }
    HuSprScaleSet(lbl_1_bss_718, (s16) (var_r30_15 + 0x10), temp_f31_11, temp_f31_11);
    goto block_184;
block_183:
    HuSprScaleSet(lbl_1_bss_718, (s16) (var_r30_15 + 0x10), (f32) (0.8 * (f64) temp_f31_11), (f32) (0.8 * (f64) temp_f31_11));
block_184:
    var_r30_15 += 1;
loop_185:
    if (var_r30_15 < 2) {
        goto block_181;
    }
    HuPrcVSleep();
    var_r31_16 += 1;
loop_187:
    if (var_r31_16 <= 0xA) {
        goto block_180;
    }
    var_r31_17 = 1;
    goto loop_190;
block_189:
    temp_f31_12 = (f32) (1.0 - ((f64) var_r31_17 / 10.0));
    HuSprScaleSet(lbl_1_bss_718, (s16) (sdWorkInt[3] + 6), temp_f31_12, temp_f31_12);
    HuPrcVSleep();
    var_r31_17 += 1;
loop_190:
    if (var_r31_17 <= 0xA) {
        goto block_189;
    }
    goto block_113;
block_192:
    var_r28_3 = 0;
    if ((s32) (fn_1_507C((u16) (1 << arg0)) & 1) == 0) {
        goto block_195;
    }
    if (var_r29_3 == 0) {
        goto block_195;
    }
    var_r28_3 = -1;
    goto block_198;
block_195:
    if ((s32) (fn_1_507C((u16) (1 << arg0)) & 2) == 0) {
        goto block_198;
    }
    if (var_r29_3 >= 1) {
        goto block_198;
    }
    var_r28_3 = 1;
block_198:
    if (var_r28_3 == 0) {
        goto block_203;
    }
    HuAudFXPlay(0);
    var_r31_18 = 1;
    goto loop_201;
block_200:
    temp_f31_13 = (f32) (1.0 - (0.2 * ((f64) var_r31_18 / 10.0)));
    HuSprScaleSet(lbl_1_bss_718, (s16) (var_r29_3 + 0x10), temp_f31_13, temp_f31_13);
    temp_f31_14 = (f32) (0.8 + (0.2 * ((f64) var_r31_18 / 10.0)));
    HuSprScaleSet(lbl_1_bss_718, (s16) (var_r29_3 + var_r28_3 + 0x10), temp_f31_14, temp_f31_14);
    HuPrcVSleep();
    var_r31_18 += 1;
loop_201:
    if (var_r31_18 <= 0xA) {
        goto block_200;
    }
    var_r29_3 += var_r28_3;
    HuWinMesSet(lbl_1_bss_606, var_r29_3 + 0x190044);
block_203:
    HuPrcVSleep();
loop_204:
    if ((s32) (fn_1_4FD0((u16) (1 << arg0)) & 0x100) == 0) {
        goto block_178;
    }
    HuAudFXPlay(1);
    HuWinHomeClear(lbl_1_bss_606);
    sdWorkInt[4] = (s32) var_r29_3;
    var_r31_19 = 1;
    goto loop_213;
block_206:
    temp_f31_15 = (f32) ((f64) var_r31_19 / 10.0);
    var_r30_16 = 0;
    goto loop_211;
block_207:
    if (var_r29_3 == var_r30_16) {
        goto block_209;
    }
    HuSprScaleSet(lbl_1_bss_718, (s16) (var_r30_16 + 0x10), (f32) (0.8 * (1.0 - (f64) temp_f31_15)), (f32) (0.8 * (1.0 - (f64) temp_f31_15)));
    goto block_210;
block_209:
    temp_f30_3 = 238.0f + (f32) (var_r30_16 * 0x64);
    HuSprPosSet(lbl_1_bss_718, (s16) (var_r30_16 + 0x10), temp_f30_3 + ((130.0f - temp_f30_3) * temp_f31_15), 250.0f);
block_210:
    var_r30_16 += 1;
loop_211:
    if (var_r30_16 < 2) {
        goto block_207;
    }
    HuPrcVSleep();
    var_r31_19 += 1;
loop_213:
    if (var_r31_19 <= 0xA) {
        goto block_206;
    }
    temp_r29 = (s16) sdWorkInt[5];
    var_r31_20 = 0;
    goto loop_216;
block_215:
    HuSprPosSet(lbl_1_bss_716, var_r31_20, 288.0f, 250.0f);
    var_r31_20 += 1;
loop_216:
    if (var_r31_20 < 8) {
        goto block_215;
    }
    HuSprPosSet(lbl_1_bss_718, 0x12, 224.0f, 250.0f);
    HuSprPosSet(lbl_1_bss_718, 0x13, 352.0f, 250.0f);
    HuSprTPLvlSet(lbl_1_bss_718, 0x12, 0.0f);
    HuSprTPLvlSet(lbl_1_bss_718, 0x13, 0.0f);
    HuSprAttrReset(lbl_1_bss_716, temp_r29, 4);
    HuSprTPLvlSet(lbl_1_bss_716, temp_r29, 1.0f);
    var_r31_21 = 1;
    goto loop_223;
block_218:
    temp_f31_16 = (f32) ((f64) var_r31_21 / 20.0);
    HuSprScaleSet(lbl_1_bss_716, temp_r29, temp_f31_16, temp_f31_16);
    if (temp_r29 == 0) {
        goto block_220;
    }
    HuSprTPLvlSet(lbl_1_bss_718, 0x12, temp_f31_16);
block_220:
    if (temp_r29 >= 7) {
        goto block_222;
    }
    HuSprTPLvlSet(lbl_1_bss_718, 0x13, temp_f31_16);
block_222:
    HuPrcVSleep();
    var_r31_21 += 1;
loop_223:
    if (var_r31_21 <= 0x14) {
        goto block_218;
    }
block_224:
    var_r29_4 = (s16) sdWorkInt[5];
    HuWinMesSet(lbl_1_bss_606, lbl_1_data_12A4[var_r29_4]);
    goto loop_257;
block_225:
    if ((s32) (fn_1_4FD0((u16) (1 << arg0)) & 0x200) == 0) {
        goto block_237;
    }
    HuAudFXPlay(3);
    HuWinHomeClear(lbl_1_bss_606);
    var_r31_22 = 1;
    goto loop_232;
block_227:
    temp_f31_17 = (f32) (1.0 - ((f64) var_r31_22 / 10.0));
    HuSprTPLvlSet(lbl_1_bss_716, var_r29_4, temp_f31_17);
    if (var_r29_4 == 0) {
        goto block_229;
    }
    HuSprTPLvlSet(lbl_1_bss_718, 0x12, temp_f31_17);
block_229:
    if (var_r29_4 >= 7) {
        goto block_231;
    }
    HuSprTPLvlSet(lbl_1_bss_718, 0x13, temp_f31_17);
block_231:
    HuPrcVSleep();
    var_r31_22 += 1;
loop_232:
    if (var_r31_22 <= 0xA) {
        goto block_227;
    }
    var_r31_23 = 1;
    goto loop_235;
block_234:
    temp_f31_18 = (f32) (1.0 - ((f64) var_r31_23 / 10.0));
    HuSprScaleSet(lbl_1_bss_718, (s16) (sdWorkInt[4] + 0x10), temp_f31_18, temp_f31_18);
    HuPrcVSleep();
    var_r31_23 += 1;
loop_235:
    if (var_r31_23 <= 0xA) {
        goto block_234;
    }
    goto block_168;
block_237:
    var_r28_4 = 0;
    if ((s32) (fn_1_507C((u16) (1 << arg0)) & 1) == 0) {
        goto block_240;
    }
    if (var_r29_4 == 0) {
        goto block_240;
    }
    var_r28_4 = -1;
    goto block_243;
block_240:
    if ((s32) (fn_1_507C((u16) (1 << arg0)) & 2) == 0) {
        goto block_243;
    }
    if (var_r29_4 >= 7) {
        goto block_243;
    }
    var_r28_4 = 1;
block_243:
    if (var_r28_4 == 0) {
        goto block_256;
    }
    HuAudFXPlay(0);
    HuSprAttrReset(lbl_1_bss_716, var_r29_4, 4);
    HuSprAttrReset(lbl_1_bss_716, (s16) (var_r29_4 + var_r28_4), 4);
    HuSprPriSet(lbl_1_bss_716, var_r29_4, 0x23);
    HuSprPriSet(lbl_1_bss_716, (s16) (var_r29_4 + var_r28_4), 0x1E);
    var_r31_24 = 1;
    goto loop_254;
block_245:
    temp_f31_19 = (f32) ((f64) var_r31_24 / 10.0);
    HuSprTPLvlSet(lbl_1_bss_716, (s16) (var_r29_4 + var_r28_4), temp_f31_19);
    if ((s32) (var_r29_4 + var_r28_4) != 0) {
        goto block_247;
    }
    HuSprTPLvlSet(lbl_1_bss_718, 0x12, (f32) (1.0 - (f64) temp_f31_19));
    goto block_249;
block_247:
    if (var_r29_4 != 0) {
        goto block_249;
    }
    HuSprTPLvlSet(lbl_1_bss_718, 0x12, temp_f31_19);
block_249:
    if ((s32) (var_r29_4 + var_r28_4) < 7) {
        goto block_251;
    }
    HuSprTPLvlSet(lbl_1_bss_718, 0x13, (f32) (1.0 - (f64) temp_f31_19));
    goto block_253;
block_251:
    if (var_r29_4 != 7) {
        goto block_253;
    }
    HuSprTPLvlSet(lbl_1_bss_718, 0x13, temp_f31_19);
block_253:
    HuPrcVSleep();
    var_r31_24 += 1;
loop_254:
    if (var_r31_24 <= 0xA) {
        goto block_245;
    }
    HuSprAttrSet(lbl_1_bss_716, var_r29_4, 4);
    var_r29_4 += var_r28_4;
    HuWinMesSet(lbl_1_bss_606, lbl_1_data_12A4[var_r29_4]);
block_256:
    HuPrcVSleep();
loop_257:
    if ((s32) (fn_1_4FD0((u16) (1 << arg0)) & 0x100) == 0) {
        goto block_225;
    }
    HuAudFXPlay(2);
    HuPrcSleep(0x3C);
    sdWorkInt[5] = (s32) var_r29_4;
    GwSdBattle.gameType = (s8) sdWorkInt[3];
    GwSdBattle.stageNo = (s8) lbl_1_data_12C4[sdWorkInt[5]];
    if ((s32) sdWorkInt[4] != 0) {
        goto block_260;
    }
    GwPlayerConf[1].type = 0;
    GwPlayerConf->type = 0;
    GwSdBattle.stageType2 = -1;
    GwSdBattle.stageType1 = -1;
    goto block_261;
block_260:
    GwPlayerConf[1].type = 1;
    GwPlayerConf->type = 1;
    GwPlayerConf[1].dif = 1;
    GwPlayerConf->dif = 1;
    GwSdBattle.stageType2 = 1;
    GwSdBattle.stageType1 = 1;
block_261:
    var_r31_25 = 0;
    goto loop_266;
block_262:
    temp_r30 = sp14[sdWorkInt[var_r31_25 + 1]];
    if (temp_r30 >= 2) {
        goto block_264;
    }
    temp_r5 = &GwSdBattle.machine[var_r31_25];
    *temp_r5 = GwSdCommon.machine[temp_r30];
    goto block_265;
block_264:
    temp_r6_2 = &GwSdBackup[GwSdBackupNo];
    temp_r5_2 = &GwSdBattle.machine[var_r31_25];
    *temp_r5_2 = temp_r6_2->machine;
block_265:
    GwSdBattle.padNo[var_r31_25] = (s8) temp_r30;
    GwPlayerConf[var_r31_25].padNo = temp_r30;
    var_r31_25 += 1;
loop_266:
    if (var_r31_25 < 2) {
        goto block_262;
    }
    fn_1_4238(2);
loop_268:
    HuPrcVSleep();
    goto loop_268;
block_269:
    var_r30_17 = 0;
    goto loop_271;
block_270:
    HuWinDispOff(sp28[var_r30_17 + 4]);
    var_r30_17 += 1;
loop_271:
    if (var_r30_17 < 3) {
        goto block_270;
    }
    var_r31_26 = 1;
    goto loop_277;
block_273:
    temp_f31_20 = (f32) (1.0 - ((f64) var_r31_26 / 10.0));
    var_r30_18 = 0;
    goto loop_275;
block_274:
    HuWinScaleSet(sp28[var_r30_18], 1.0f, temp_f31_20);
    HuSprPosSet(lbl_1_bss_718, (s16) (sp14[var_r30_18] + 9), lbl_1_data_127C[var_r30_18].x, lbl_1_data_127C[var_r30_18].y);
    HuSprScaleSet(lbl_1_bss_718, (s16) (sp14[var_r30_18] + 9), 1.0f, temp_f31_20);
    fn_1_45E4(&sp38[var_r30_18], temp_f31_20, temp_f31_20, temp_f31_20);
    var_r30_18 += 1;
loop_275:
    if (var_r30_18 < 3) {
        goto block_274;
    }
    HuPrcVSleep();
    var_r31_26 += 1;
loop_277:
    if (var_r31_26 <= 0xA) {
        goto block_273;
    }
    var_r31_27 = 0;
    goto loop_280;
block_279:
    HuWinKill(sp28[var_r31_27]);
    var_r31_27 += 1;
loop_280:
    if (var_r31_27 < 7) {
        goto block_279;
    }
    var_r31_28 = 0;
    goto loop_283;
block_282:
    HuSprPosSet(lbl_1_bss_718, (s16) (var_r31_28 + 9), 0.0f, -1000.0f);
    var_r31_28 += 1;
loop_283:
    if (var_r31_28 < 3) {
        goto block_282;
    }
    var_r31_29 = 0;
    goto loop_286;
block_285:
    HuSprAttrSet(lbl_1_bss_718, (s16) (var_r31_29 + 0xC), 4);
    var_r31_29 += 1;
loop_286:
    if (var_r31_29 < 3) {
        goto block_285;
    }
    HuSprPosSet(lbl_1_bss_718, 0xF, 0.0f, -1000.0f);
    var_r31_30 = 0;
    goto loop_289;
block_288:
    HuSprPosSet(lbl_1_bss_718, (s16) (var_r31_30 + 6), 0.0f, -1000.0f);
    var_r31_30 += 1;
loop_289:
    if (var_r31_30 < 3) {
        goto block_288;
    }
    var_r31_31 = 0;
    goto loop_292;
block_291:
    HuSprPosSet(lbl_1_bss_718, (s16) (var_r31_31 + 0x10), 0.0f, -1000.0f);
    var_r31_31 += 1;
loop_292:
    if (var_r31_31 < 2) {
        goto block_291;
    }
    var_r31_32 = 0;
    goto loop_295;
block_294:
    HuSprPosSet(lbl_1_bss_716, var_r31_32, 0.0f, -1000.0f);
    var_r31_32 += 1;
loop_295:
    if (var_r31_32 < 8) {
        goto block_294;
    }
    fn_1_4724(&sp38[0]);
    fn_1_4724(&sp38[1]);
    fn_1_4724(&sp38[2]);
    var_r31_33 = 0;
    goto loop_300;
block_297:
    if (GwSdCommon.machine[var_r31_33].charNo == -1) {
        goto block_299;
    }
    fn_1_46C4(&lbl_1_bss_67C[var_r31_33], 1);
block_299:
    var_r31_33 += 1;
loop_300:
    if (var_r31_33 < 2) {
        goto block_297;
    }
    return;
}

void fn_1_113F8(s16 arg0) {
    Point3d sp14;
    Point3d sp8;
    f32 temp_f28;
    f32 temp_f30;
    f32 temp_f30_2;
    f32 temp_f31;
    f32 temp_f31_10;
    f32 temp_f31_11;
    f32 temp_f31_2;
    f32 temp_f31_3;
    f32 temp_f31_4;
    f32 temp_f31_5;
    f32 temp_f31_6;
    f32 temp_f31_7;
    f32 temp_f31_8;
    f32 temp_f31_9;
    s16 temp_r24;
    s16 temp_r3;
    s16 var_r25;
    s16 var_r26;
    s16 var_r27;
    s16 var_r27_2;
    s16 var_r29;
    s16 var_r29_2;
    s16 var_r30;
    s16 var_r30_2;
    s16 var_r30_3;
    s16 var_r30_4;
    s16 var_r30_5;
    s16 var_r30_6;
    s16 var_r30_7;
    s16 var_r30_8;
    s16 var_r30_9;
    s16 var_r31;
    s16 var_r31_2;
    s16 var_r31_3;
    s16 var_r31_4;
    s16 var_r31_5;
    s16 var_r31_6;
    s16 var_r31_7;
    s16 var_r31_8;
    s32 temp_r0;
    s32 temp_r0_2;
    s32 temp_r3_2;
    s32 temp_r5;

    sdWorkInt[7] = (s32) arg0;
    if ((s32) omovlevtno != 3) {
        sdWorkInt[6] = 0;
        sdWorkInt[3] = 0;
    }
    temp_r3 = HuWinCreate(-10000.0f, 310.0f, 0x230, 0x2A, 0);
    HuWinBGTPLvlSet(temp_r3, 0.0f);
    HuWinAttrSet(temp_r3, 0x800U);
    HuWinMesSpeedSet(temp_r3, 0);
    HuWinMesSet(temp_r3, 0x20004U);
loop_3:
    var_r29 = (s16) sdWorkInt[3];
    if (GwSdCommon.flagHardWinF != 0) {
        var_r25 = 3;
    } else if (GwSdCommon.battleHardWinF != 0) {
        var_r25 = 2;
    } else {
        var_r25 = 1;
    }
    var_r31 = 0;
loop_10:
    if (var_r31 < var_r25) {
        temp_r0 = var_r25 * 0x64;
        HuSprPosSet(lbl_1_bss_718, (s16) (var_r31 + 6), (288.0f - (f32) ((temp_r0 / 2) - 0x32)) + (f32) (var_r31 * 0x64), 270.0f);
        var_r31 += 1;
        goto loop_10;
    }
    var_r30 = 1;
loop_19:
    if (var_r30 <= 0x14) {
        temp_f31 = (f32) ((f64) var_r30 / 20.0);
        var_r31_2 = 0;
loop_17:
        if (var_r31_2 < var_r25) {
            if (var_r29 == var_r31_2) {
                HuSprScaleSet(lbl_1_bss_718, (s16) (var_r31_2 + 6), temp_f31, temp_f31);
            } else {
                HuSprScaleSet(lbl_1_bss_718, (s16) (var_r31_2 + 6), (f32) (0.8 * (f64) temp_f31), (f32) (0.8 * (f64) temp_f31));
            }
            var_r31_2 += 1;
            goto loop_17;
        }
        HuPrcVSleep();
        var_r30 += 1;
        goto loop_19;
    }
    HuWinMesSet(lbl_1_bss_606, var_r29 + 0x19001F);
loop_37:
    if ((s32) (fn_1_4FD0((u16) (1 << arg0)) & 0x100) != 0) {
        HuAudFXPlay(1);
        sdWorkInt[3] = (s32) var_r29;
        var_r30_2 = 1;
loop_46:
        if (var_r30_2 <= 0xA) {
            temp_f31_2 = (f32) ((f64) var_r30_2 / 10.0);
            var_r31_3 = 0;
loop_44:
            if (var_r31_3 < var_r25) {
                if (var_r29 == var_r31_3) {
                    temp_r0_2 = var_r25 * 0x64;
                    temp_f30 = (288.0f - (f32) ((temp_r0_2 / 2) - 0x32)) + (f32) (var_r31_3 * 0x64);
                    HuSprPosSet(lbl_1_bss_718, (s16) (var_r31_3 + 6), temp_f30 + ((288.0f - temp_f30) * temp_f31_2), 270.0f + (-68.0f * temp_f31_2));
                } else {
                    HuSprScaleSet(lbl_1_bss_718, (s16) (var_r31_3 + 6), (f32) (0.8 * (1.0 - (f64) temp_f31_2)), (f32) (0.8 * (1.0 - (f64) temp_f31_2)));
                }
                var_r31_3 += 1;
                goto loop_44;
            }
            HuPrcVSleep();
            var_r30_2 += 1;
            goto loop_46;
        }
        sdWorkInt[6] = 0;
        var_r26 = 2;
        if (((s32) sdWorkInt[3] == 0) && (GwSdCommon.battleNormalWinF != 0)) {
            var_r26 = 3;
        }
        if (((s32) sdWorkInt[3] == 1) && (GwSdCommon.flagNormalWinF != 0)) {
            var_r26 = 3;
        }
        if (((s32) sdWorkInt[3] == 2) && (GwSdCommon.rabbitNormalWinF != 0)) {
            var_r26 = 3;
        }
        var_r31_4 = 0;
loop_58:
        if (var_r31_4 < var_r26) {
            temp_r5 = var_r26 * 0x64;
            HuSprPosSet(lbl_1_bss_718, (s16) (var_r31_4 + 0x14), (288.0f - (f32) ((temp_r5 / 2) - 0x32)) + (f32) (var_r31_4 * 0x64), 270.0f);
            var_r31_4 += 1;
            goto loop_58;
        }
        var_r30_3 = 1;
loop_67:
        if (var_r30_3 <= 0x14) {
            temp_f31_3 = (f32) ((f64) var_r30_3 / 20.0);
            var_r31_5 = 0;
loop_65:
            if (var_r31_5 < var_r26) {
                if (var_r31_5 == 0) {
                    HuSprScaleSet(lbl_1_bss_718, (s16) (var_r31_5 + 0x14), temp_f31_3, temp_f31_3);
                } else {
                    HuSprScaleSet(lbl_1_bss_718, (s16) (var_r31_5 + 0x14), (f32) (0.8 * (f64) temp_f31_3), (f32) (0.8 * (f64) temp_f31_3));
                }
                var_r31_5 += 1;
                goto loop_65;
            }
            HuPrcVSleep();
            var_r30_3 += 1;
            goto loop_67;
        }
        var_r29_2 = (s16) sdWorkInt[6];
        HuWinMesSet(lbl_1_bss_606, var_r29_2 + 0x190046);
loop_97:
        if ((s32) (fn_1_4FD0((u16) (1 << arg0)) & 0x100) != 0) {
            HuAudFXPlay(1);
            sdWorkInt[6] = (s32) var_r29_2;
            var_r30_4 = 1;
loop_106:
            if (var_r30_4 <= 0xA) {
                temp_f31_4 = (f32) ((f64) var_r30_4 / 10.0);
                var_r31_6 = 0;
loop_104:
                if (var_r31_6 < var_r26) {
                    if (var_r29_2 == var_r31_6) {
                        temp_r3_2 = var_r26 * 0x64;
                        temp_f30_2 = (288.0f - (f32) ((temp_r3_2 / 2) - 0x32)) + (f32) (var_r31_6 * 0x64);
                        HuSprPosSet(lbl_1_bss_718, (s16) (var_r31_6 + 0x14), temp_f30_2 + ((288.0f - temp_f30_2) * temp_f31_4), 270.0f + (-2.0f * temp_f31_4));
                    } else {
                        HuSprScaleSet(lbl_1_bss_718, (s16) (var_r31_6 + 0x14), (f32) (0.8 * (1.0 - (f64) temp_f31_4)), (f32) (0.8 * (1.0 - (f64) temp_f31_4)));
                    }
                    var_r31_6 += 1;
                    goto loop_104;
                }
                HuPrcVSleep();
                var_r30_4 += 1;
                goto loop_106;
            }
            HuAudFXPlay(0x50E);
            temp_r24 = lbl_1_bss_58C[arg0 + 3];
            Hu3DModelAttrReset(temp_r24, 1U);
            Hu3DMotionStartEndSet(temp_r24, 61.0f, 600.0f);
            Hu3DMotionTimeSet(temp_r24, 361.0f);
            Hu3DModelObjPosGet(temp_r24, "cyl61", &sp14);
loop_111:
            if (Hu3DMotionTimeGet(temp_r24) < 599.0f) {
                Hu3DModelObjPosGet(temp_r24, "cyl61", &sp8);
                temp_f28 = (f32) (1450.0 * -cos((3.141592653589793 * (f64) ((arg0 * 0x3C) + 0x3C)) / 180.0));
                fn_1_4534(&lbl_1_bss_67C[arg0], (f32) (1450.0 * -sin((3.141592653589793 * (f64) ((arg0 * 0x3C) + 0x3C)) / 180.0)), sp8.y - sp14.y, temp_f28);
                if (Hu3DMotionTimeGet(temp_r24) == 540.0f) {
                    WipeCreate(2, 6, 0x3C);
                    HuAudFadeOut(0x3E8);
                }
                HuPrcVSleep();
                goto loop_111;
            }
            GwSdBattle.gameType = (s8) sdWorkInt[3];
            GwSdBattle.stageNo = 0;
            GwPlayerConf->type = 1;
            GwPlayerConf[1].type = 0;
            GwSdBattle.stageType1 = -1;
            GwSdBattle.stageType2 = 1;
            GwPlayerConf[1].dif = 1;
            GwSdBattle.machine[0] = GwSdCommon.machine[arg0];
            GwSdBattle.machine[1] = GwSdCommon.machine[arg0];
            GwSdBattle.padNo[0] = (s8) arg0;
            GwPlayerConf->padNo = arg0;
            GwSdBattle.padNo[1] = 3;
loop_114:
            if (WipeCheckEnd() != 0) {
                HuPrcVSleep();
                goto loop_114;
            }
            Hu3DModelKill(lbl_1_bss_58C[0]);
            Hu3DModelKill(lbl_1_bss_58C[3]);
            Hu3DModelKill(lbl_1_bss_58C[4]);
            Hu3DModelKill(lbl_1_bss_58A);
            fn_1_4724(lbl_1_bss_67C);
            fn_1_4724(&lbl_1_bss_67C[1]);
            HuWinKill(temp_r3);
            HuSprGrpKill(lbl_1_bss_718);
            HuWinKill(lbl_1_bss_606);
            SdMachineFadeAlpha = 0.0f;
            Hu3DLayerHookReset(0xD);
            fn_1_1C260();
loop_116:
            HuPrcVSleep();
            goto loop_116;
        }
        if ((s32) (fn_1_4FD0((u16) (1 << arg0)) & 0x200) != 0) {
            HuAudFXPlay(3);
            var_r30_5 = 1;
loop_78:
            if (var_r30_5 <= 0xA) {
                temp_f31_5 = (f32) (1.0 - ((f64) var_r30_5 / 10.0));
                var_r31_7 = 0;
loop_76:
                if (var_r31_7 < var_r26) {
                    if (var_r29_2 == var_r31_7) {
                        HuSprScaleSet(lbl_1_bss_718, (s16) (var_r31_7 + 0x14), temp_f31_5, temp_f31_5);
                    } else {
                        HuSprScaleSet(lbl_1_bss_718, (s16) (var_r31_7 + 0x14), (f32) (0.8 * (f64) temp_f31_5), (f32) (0.8 * (f64) temp_f31_5));
                    }
                    var_r31_7 += 1;
                    goto loop_76;
                }
                HuPrcVSleep();
                var_r30_5 += 1;
                goto loop_78;
            }
            var_r30_6 = 1;
loop_81:
            if (var_r30_6 <= 0xA) {
                temp_f31_6 = (f32) (1.0 - ((f64) var_r30_6 / 10.0));
                HuSprScaleSet(lbl_1_bss_718, (s16) (sdWorkInt[3] + 6), temp_f31_6, temp_f31_6);
                HuPrcVSleep();
                var_r30_6 += 1;
                goto loop_81;
            }
            goto loop_3;
        }
        var_r27 = 0;
        if (((s32) (fn_1_507C((u16) (1 << arg0)) & 1) != 0) && (var_r29_2 != 0)) {
            var_r27 = -1;
        } else if (((s32) (fn_1_507C((u16) (1 << arg0)) & 2) != 0) && (var_r29_2 < (s32) (var_r26 - 1))) {
            var_r27 = 1;
        }
        if (var_r27 != 0) {
            HuAudFXPlay(0);
            var_r30_7 = 1;
loop_94:
            if (var_r30_7 <= 0xA) {
                temp_f31_7 = (f32) (1.0 - (0.2 * ((f64) var_r30_7 / 10.0)));
                HuSprScaleSet(lbl_1_bss_718, (s16) (var_r29_2 + 0x14), temp_f31_7, temp_f31_7);
                temp_f31_8 = (f32) (0.8 + (0.2 * ((f64) var_r30_7 / 10.0)));
                HuSprScaleSet(lbl_1_bss_718, (s16) (var_r29_2 + var_r27 + 0x14), temp_f31_8, temp_f31_8);
                if (var_r30_7 == 5) {
                    HuWinMesSet(lbl_1_bss_606, var_r29_2 + 0x190000 + var_r27 + 0x46);
                }
                HuPrcVSleep();
                var_r30_7 += 1;
                goto loop_94;
            }
            var_r29_2 += var_r27;
        }
        HuPrcVSleep();
        goto loop_97;
    }
    if ((s32) (fn_1_4FD0((u16) (1 << arg0)) & 0x200) != 0) {
        HuAudFXPlay(3);
        HuWinKill(temp_r3);
        var_r30_8 = 1;
loop_125:
        if (var_r30_8 <= 0xA) {
            temp_f31_9 = (f32) (1.0 - ((f64) var_r30_8 / 10.0));
            var_r31_8 = 0;
loop_123:
            if (var_r31_8 < 3) {
                if ((s32) sdWorkInt[3] == var_r31_8) {
                    HuSprScaleSet(lbl_1_bss_718, (s16) (var_r31_8 + 6), temp_f31_9, temp_f31_9);
                } else {
                    HuSprScaleSet(lbl_1_bss_718, (s16) (var_r31_8 + 6), (f32) (0.8 * (f64) temp_f31_9), (f32) (0.8 * (f64) temp_f31_9));
                }
                var_r31_8 += 1;
                goto loop_123;
            }
            HuPrcVSleep();
            var_r30_8 += 1;
            goto loop_125;
        }
        return;
    }
    var_r27_2 = 0;
    if (((s32) (fn_1_507C((u16) (1 << arg0)) & 1) != 0) && (var_r29 != 0)) {
        var_r27_2 = -1;
    } else if (((s32) (fn_1_507C((u16) (1 << arg0)) & 2) != 0) && (var_r29 < (s32) (var_r25 - 1))) {
        var_r27_2 = 1;
    }
    if (var_r27_2 != 0) {
        HuAudFXPlay(0);
        var_r30_9 = 1;
loop_34:
        if (var_r30_9 <= 0xA) {
            temp_f31_10 = (f32) (1.0 - (0.2 * ((f64) var_r30_9 / 10.0)));
            HuSprScaleSet(lbl_1_bss_718, (s16) (var_r29 + 6), temp_f31_10, temp_f31_10);
            temp_f31_11 = (f32) (0.8 + (0.2 * ((f64) var_r30_9 / 10.0)));
            HuSprScaleSet(lbl_1_bss_718, (s16) (var_r29 + var_r27_2 + 6), temp_f31_11, temp_f31_11);
            if (var_r30_9 == 5) {
                HuWinMesSet(lbl_1_bss_606, var_r29 + 0x190000 + var_r27_2 + 0x1F);
            }
            HuPrcVSleep();
            var_r30_9 += 1;
            goto loop_34;
        }
        var_r29 += var_r27_2;
    }
    HuPrcVSleep();
    goto loop_37;
}

s16 fn_1_12940(s16 arg0, s16 *arg1, void (*arg2)(s16 *, s16)) {
    HuVecF sp30;
    HuVecF sp24;
    s16 sp1C[3];
    s32 sp18;
    s32 sp14;
    s16 sp10;
    void (*spC)(s16 *, s16);
    s16 sp8;
    s16 temp_r0;
    s16 temp_r3;
    s16 var_r18;
    s16 var_r21;
    s16 var_r23;
    s16 var_r25;
    s16 var_r26;
    s16 var_r27;
    s16 var_r29;
    s16 var_r29_2;
    s16 var_r29_3;
    s16 var_r30;
    s16 var_r31;
    s16 var_r31_10;
    s16 var_r31_11;
    s16 var_r31_2;
    s16 var_r31_3;
    s16 var_r31_4;
    s16 var_r31_5;
    s16 var_r31_6;
    s16 var_r31_7;
    s16 var_r31_8;
    s16 var_r31_9;

    sp8 = arg0;
    spC = arg2;
    if (GwSdCommon.extCharSelF != 0) {
        sp18 = 1;
    } else {
        sp18 = 0;
    }
    sp10 = sp18 + 0xA;
    if (sp18 == 0) {
        HuSprAttrSet(lbl_1_bss_714, 0xB, 4);
    }
    HuSprAttrReset(lbl_1_bss_714, 0, 4);
    var_r31 = 0;
loop_7:
    if (var_r31 < 3) {
        HuSprAttrSet(lbl_1_bss_714, (s16) (var_r31 + 0xC), 4);
        var_r31 += 1;
        goto loop_7;
    }
    var_r31_2 = 0;
loop_10:
    if (var_r31_2 < sp10) {
        HuSprPosSet(lbl_1_bss_714, (s16) (var_r31_2 + 1), lbl_1_data_1394[sp18][var_r31_2 * 2], lbl_1_data_1394[sp18][var_r31_2 * 2 + 1]);
        HuSprAttrReset(lbl_1_bss_714, (s16) (var_r31_2 + 1), 4);
        var_r31_2 += 1;
        goto loop_10;
    }
    var_r31_3 = 1;
loop_13:
    if (var_r31_3 <= 0x14) {
        HuSprGrpPosSet(lbl_1_bss_714, (f32) (288.0 + (640.0 * (1.0 - sin((3.141592653589793 * (f64) (90.0f * (f32) ((f64) var_r31_3 / 20.0))) / 180.0)))), 210.0f);
        HuPrcVSleep();
        var_r31_3 += 1;
        goto loop_13;
    }
    var_r18 = 0;
    var_r31_4 = 0;
loop_19:
    if (var_r31_4 < 3) {
        if ((s32) (sp8 & (1 << var_r31_4)) != 0) {
            lbl_1_bss_5D0[var_r31_4] = var_r31_4;
            var_r18 += 1;
        } else {
            lbl_1_bss_5D0[var_r31_4] = -1;
        }
        sp1C[var_r31_4] = var_r31_4;
        lbl_1_bss_5D6[var_r31_4] = -1;
        var_r31_4 += 1;
        goto loop_19;
    }
    var_r31_5 = 0;
loop_24:
    if (var_r31_5 < 3) {
        if ((s16) lbl_1_bss_5D0[var_r31_5] != -1) {
            sp30.x = (288.0f + lbl_1_data_1394[sp18][var_r31_5 * 2]) - 32.0f;
            sp30.y = (210.0f + lbl_1_data_1394[sp18][var_r31_5 * 2 + 1]) - 32.0f;
            sp30.z = 500.0f;
            Hu3D2Dto3D(&sp30, 4, &sp24);
            Hu3DModelPosSetV(lbl_1_bss_70E[var_r31_5], &sp24);
            Hu3DAnimAttrSet(lbl_1_bss_708[var_r31_5], 1U);
            Hu3DModelAttrReset(lbl_1_bss_70E[var_r31_5], 1U);
            Hu3DAnmNoSet(lbl_1_bss_708[var_r31_5], (u16) lbl_1_bss_5D0[var_r31_5]);
        }
        var_r31_5 += 1;
        goto loop_24;
    }
    var_r23 = 1;
    temp_r3 = HuWinCreate(-10000.0f, 310.0f, 0x230, 0x28, 0);
    HuWinBGTPLvlSet(temp_r3, 0.0f);
    HuWinAttrSet(temp_r3, 0x800U);
    HuWinMesSpeedSet(temp_r3, 0);
    HuWinMesSet(temp_r3, 0x20004U);
loop_26:
    var_r30 = 0;
loop_73:
    if (var_r30 < 3) {
        if (((s16) lbl_1_bss_5D0[var_r30] != -1) && (temp_r0 = 1 << lbl_1_bss_5D0[var_r30], (((s16) lbl_1_bss_5D6[var_r30] == -2) == 0))) {
            if ((s16) lbl_1_bss_5D6[var_r30] != -1) {
                if ((s32) (fn_1_4FD0((u16) temp_r0) & 0x200) != 0) {
                    HuAudFXPlay(3);
                    HuPrcChildCreate(fn_1_138D0, 0x3E8U, 0x1000U, 0, HuPrcCurrentGet())->property = (void *)(uintptr_t)((var_r30 << 8) | lbl_1_bss_5D6[var_r30]);
                    Hu3DModelAttrReset(lbl_1_bss_70E[var_r30], 1U);
                    lbl_1_bss_5D6[var_r30] = -2;
                }
                goto block_72;
            }
            var_r27 = sp1C[var_r30];
            var_r21 = 0;
            var_r26 = 0;
            if ((s32) (fn_1_507C((u16) temp_r0) & 1) != 0) {
                var_r26 = -1;
            } else if ((s32) (fn_1_507C((u16) temp_r0) & 2) != 0) {
                var_r26 = 1;
            }
            if ((s32) (fn_1_507C((u16) temp_r0) & 8) != 0) {
                var_r21 = -1;
            } else if ((s32) (fn_1_507C((u16) temp_r0) & 4) != 0) {
                var_r21 = 1;
            }
            if ((var_r26 != 0) || (var_r21 != 0)) {
                var_r31_6 = lbl_1_data_139C[var_r27 + sp18 * 10][(var_r21 * 3) + var_r26 + 4];
                var_r29 = 0;
loop_47:
                if (var_r29 < 3) {
                    if ((var_r29 == var_r30) || ((s16) lbl_1_bss_5D0[var_r29] == -1) || ((s32) (var_r27 + var_r31_6) != (s16) sp1C[var_r29])) {
                        var_r29 += 1;
                        goto loop_47;
                    }
                }
                if (var_r29 != 3) {
                    if (var_r21 == 0) {
                        var_r29_2 = 1;
loop_62:
                        if (var_r29_2 >= 5) {

                        } else {
                            var_r25 = 0;
loop_56:
                            if (var_r25 < 3) {
                                if ((var_r25 == var_r30) || ((s16) lbl_1_bss_5D0[var_r25] == -1) || ((s16) sp1C[var_r25] != (s32) (var_r27 + (var_r29_2 * var_r26)))) {
                                    var_r25 += 1;
                                    goto loop_56;
                                }
                            }
                            if (var_r25 == 3) {
                                var_r31_6 = var_r26 * var_r29_2;
                            } else if ((s16) lbl_1_data_139C[var_r27 + (sp18 * 10 + var_r26 * var_r29_2)][var_r26 + 4] == 0) {
                                var_r31_6 = 0;
                            } else {
                                var_r29_2 += 1;
                                goto loop_62;
                            }
                        }
                    } else {
                        var_r31_6 = 0;
                    }
                }
                if (var_r31_6 != 0) {
                    HuAudFXPlay(0);
                    var_r27 += var_r31_6;
                    sp1C[var_r30] = var_r27;
                    sp30.x = (288.0f + lbl_1_data_1394[sp18][var_r27 * 2]) - 32.0f;
                    sp30.y = (210.0f + lbl_1_data_1394[sp18][var_r27 * 2 + 1]) - 32.0f;
                    sp30.z = 500.0f;
                    Hu3D2Dto3D(&sp30, 4, &sp24);
                    Hu3DModelPosSetV(lbl_1_bss_70E[var_r30], &sp24);
                }
            }
            if ((s32) (fn_1_4FD0((u16) temp_r0) & 0x100) != 0) {
                HuAudFXPlay(1);
                HuPrcChildCreate(fn_1_13688, 0x3E7U, 0x1000U, 0, HuPrcCurrentGet())->property = (void *)(uintptr_t)((var_r30 << 8) | var_r27);
                Hu3DModelAttrSet(lbl_1_bss_70E[var_r30], 1U);
                lbl_1_bss_5D6[var_r30] = -2;
                goto block_72;
            }
            if ((var_r30 == (s32) sdWorkInt[7]) && ((s32) (fn_1_4FD0((u16) temp_r0) & 0x200) != 0)) {
                HuAudFXPlay(3);
                Hu3DModelAttrSet(lbl_1_bss_70E[0], 1U);
                Hu3DModelAttrSet(lbl_1_bss_70E[1], 1U);
                var_r23 = 0;
            } else {
                goto block_72;
            }
        } else {
block_72:
            var_r30 += 1;
            goto loop_73;
        }
    }
    if (var_r23 != 0) {
        var_r29_3 = 0;
        var_r31_7 = 0;
loop_79:
        if (var_r31_7 < 3) {
            if ((s16) lbl_1_bss_5D6[var_r31_7] >= 0) {
                var_r29_3 += 1;
            }
            var_r31_7 += 1;
            goto loop_79;
        }
        if (var_r29_3 == var_r18) {
            var_r23 = 1;
        } else {
            HuPrcVSleep();
            goto loop_26;
        }
    }
    HuWinKill(temp_r3);
    var_r31_8 = 0;
loop_88:
    if (var_r31_8 < 3) {
        if ((s16) lbl_1_bss_5D0[var_r31_8] == -1) {
            *(arg1 + (var_r31_8 * 2)) = -1;
        } else {
            *(arg1 + (var_r31_8 * 2)) = lbl_1_bss_5D6[var_r31_8];
        }
        var_r31_8 += 1;
        goto loop_88;
    }
    var_r31_9 = 0;
loop_91:
    if (var_r31_9 < 3) {
        Hu3DModelAttrSet(lbl_1_bss_70E[var_r31_9], 1U);
        var_r31_9 += 1;
        goto loop_91;
    }
    if (spC != NULL) {
        spC(arg1, var_r23);
    }
    var_r31_10 = 1;
loop_99:
    if (var_r31_10 <= 0x14) {
        if (var_r23 != 0) {
            sp14 = -1;
        } else {
            sp14 = 1;
        }
        HuSprGrpPosSet(lbl_1_bss_714, (f32) (288.0 + ((f64) sp14 * (640.0 * (1.0 - cos((3.141592653589793 * (f64) (90.0f * (f32) ((f64) var_r31_10 / 20.0))) / 180.0))))), 210.0f);
        HuPrcVSleep();
        var_r31_10 += 1;
        goto loop_99;
    }
    var_r31_11 = 0;
loop_102:
    if (var_r31_11 < 0xF) {
        HuSprAttrSet(lbl_1_bss_714, var_r31_11, 4);
        var_r31_11 += 1;
        goto loop_102;
    }
    return var_r23;
}

void fn_1_13688(void) {
    f32 temp_f31;
    f32 temp_f31_2;
    f64 temp_f30;
    s16 temp_r29;
    s16 temp_r31;
    s16 var_r25;
    s16 var_r30;
    u32 temp_r26;

    temp_r26 = (u32)(uintptr_t)HuPrcCurrentGet()->property;
    temp_r31 = (s16) (u8) (temp_r26 >> 8U);
    temp_r29 = (s16) (u8) temp_r26;
    if (GwSdCommon.extCharSelF != 0) {
        var_r25 = 1;
    } else {
        var_r25 = 0;
    }
    HuSprAttrReset(lbl_1_bss_714, (s16) (temp_r31 + 0xC), 4);
    HuSprPosSet(lbl_1_bss_714, (s16) (temp_r31 + 0xC), lbl_1_data_1394[var_r25][temp_r29 * 2], lbl_1_data_1394[var_r25][temp_r29 * 2 + 1] - 16.0f);
    HuSprBankSet(lbl_1_bss_714, (s16) (temp_r31 + 0xC), lbl_1_bss_5D0[temp_r31]);
    var_r30 = 1;
loop_5:
    if (var_r30 <= 0xA) {
        temp_f31 = (f32) ((f64) var_r30 / 10.0);
        temp_f30 = 0.2 * sin((3.141592653589793 * (f64) (180.0f * temp_f31)) / 180.0);
        temp_f31_2 = (f32) (sin((3.141592653589793 * (f64) (90.0f * temp_f31)) / 180.0) + temp_f30);
        HuSprScaleSet(lbl_1_bss_714, (s16) (temp_r31 + 0xC), temp_f31_2, temp_f31_2);
        HuPrcVSleep();
        var_r30 += 1;
        goto loop_5;
    }
    lbl_1_bss_5D6[temp_r31] = temp_r29;
    HuPrcEnd();
loop_7:
    HuPrcVSleep();
    goto loop_7;
}

void fn_1_138D0(void) {
    f32 temp_f31;
    f32 temp_f31_2;
    f64 temp_f30;
    s16 temp_r26;
    s16 temp_r31;
    s16 var_r25;
    s16 var_r30;
    u32 temp_r27;

    temp_r27 = (u32)(uintptr_t)HuPrcCurrentGet()->property;
    temp_r31 = (s16) (u8) (temp_r27 >> 8U);
    temp_r26 = (s16) (u8) temp_r27;
    if (GwSdCommon.extCharSelF != 0) {
        var_r25 = 1;
    } else {
        var_r25 = 0;
    }
    HuSprPosSet(lbl_1_bss_714, (s16) (temp_r31 + 0xC), lbl_1_data_1394[var_r25][temp_r26 * 2], lbl_1_data_1394[var_r25][temp_r26 * 2 + 1] - 16.0f);
    HuSprBankSet(lbl_1_bss_714, (s16) (temp_r31 + 0xC), lbl_1_bss_5D0[temp_r31]);
    var_r30 = 1;
loop_5:
    if (var_r30 <= 5) {
        temp_f31 = (f32) ((f64) var_r30 / 5.0);
        temp_f30 = 0.2 * sin((3.141592653589793 * (f64) (180.0f * temp_f31)) / 180.0);
        temp_f31_2 = (f32) (cos((3.141592653589793 * (f64) (90.0f * temp_f31)) / 180.0) + temp_f30);
        HuSprScaleSet(lbl_1_bss_714, (s16) (temp_r31 + 0xC), temp_f31_2, temp_f31_2);
        HuPrcVSleep();
        var_r30 += 1;
        goto loop_5;
    }
    HuSprAttrSet(lbl_1_bss_714, (s16) (temp_r31 + 0xC), 4);
    lbl_1_bss_5D6[temp_r31] = -1;
    HuPrcEnd();
loop_7:
    HuPrcVSleep();
    goto loop_7;
}
