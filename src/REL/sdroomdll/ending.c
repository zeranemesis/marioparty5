#include "REL/sdroomdll.h"
#include "game/window.h"
#include "game/sprite.h"
#include "game/frand.h"
#include "game/memory.h"
#include "game/mgdata.h"
#include "stdio.h"
#include "dolphin/gx/GXEnum.h"

/* camera views (centre, rotation, zoom) */
static OMCAMERAVIEW lbl_1_data_1DE0 = { { 37.0f, -32.0f, -3090.0f }, { -26.0f, 50.0f, 0.0f }, 815.0f };
static OMCAMERAVIEW lbl_1_data_1DFC = { { 0.0f, 140.0f, -40.0f }, { -10.0f, 0.0f, 0.0f }, 1200.0f };
static OMCAMERAVIEW lbl_1_data_1E18 = { { 36.0f, -10.0f, -95.0f }, { -41.0f, 154.0f, 0.0f }, 5815.0f };
static OMCAMERAVIEW lbl_1_data_1E34 = { { 0.0f, 80.0f, -3100.0f }, { -15.0f, 0.0f, 0.0f }, 1610.0f };
static OMCAMERAVIEW lbl_1_data_1E50[3] = {
    { { -56.0f, 188.0f, 110.0f }, { 10.0f, -29.0f, 0.0f }, 786.0f },
    { { 42.0f, 171.0f, -15.0f }, { -15.0f, -100.0f, 0.0f }, 2122.0f },
    { { 42.0f, 171.0f, -15.0f }, { -15.0f, 20.0f, 0.0f }, 2122.0f },
};
static OMCAMERAVIEW lbl_1_data_1EA4 = { { 240.0f, 77.0f, 350.0f }, { -15.0f, -90.0f, 0.0f }, 1100.0f };
static OMCAMERAVIEW lbl_1_data_1EC0 = { { 0.0f, 160.0f, -48.0f }, { -5.0f, -90.0f, 0.0f }, 1780.0f };
static OMCAMERAVIEW lbl_1_data_1EDC = { { 0.0f, 160.0f, -48.0f }, { -10.0f, 0.0f, 0.0f }, 1780.0f };
static OMCAMERAVIEW lbl_1_data_1EF8 = { { -15.0f, 100.0f, -74.0f }, { -3.0f, -68.0f, 0.0f }, 3456.0f };
static OMCAMERAVIEW lbl_1_data_1F14 = { { -7.0f, 192.0f, -104.0f }, { -3.0f, 312.0f, 0.0f }, 1300.0f };
static OMCAMERAVIEW lbl_1_data_1F30 = { { 8.0f, 160.0f, -48.0f }, { -15.0f, 0.0f, 0.0f }, 3600.0f };
static OMCAMERAVIEW lbl_1_data_1F4C = { { 8.0f, 160.0f, -48.0f }, { -10.0f, 0.0f, 0.0f }, 1360.0f };
static OMCAMERAVIEW lbl_1_data_1F68 = { { 92.0f, 100.0f, -154.0f }, { -10.0f, 44.0f, 0.0f }, 700.0f };
static OMCAMERAVIEW lbl_1_data_1F84 = { { 0.0f, 140.0f, -40.0f }, { 0.0f, 180.0f, 0.0f }, 3246.0f };
static OMCAMERAVIEW lbl_1_data_1FA0 = { { 8.0f, 110.0f, -40.0f }, { -10.0f, 0.0f, 0.0f }, 1070.0f };

/* part table of the extended characters: head, body, tail, item */
static s16 lbl_1_data_1FBC[12][4] = {
    { 8, 8, 8, 10 }, { 2, 4, 6, 11 }, { 6, 9, 1, 1 }, { 3, 1, 9, 0 },
    { 9, 7, 3, 2 }, { 2, 6, 5, 4 }, { 7, 5, 7, 8 }, { 4, 1, 4, 3 },
    { 5, 1, 2, 6 }, { 1, 2, 3, 5 }, { 0, 0, 0, 7 }, { 1, 2, 1, 9 },
};

/* sparkle / confetti particle parameters */
static HU3DPARMANPARAM lbl_1_data_201C = {
    0x14,
    0,
    10.0f,
    30.0f,
    15.0f,
    { 0.0f, -0.8f, 0.0f },
    70.0f,
    0.95f,
    40.0f,
    0.97f,
    2,
    {
        { 0xFF, 0xFF, 0xFF, 0xFF },
        { 0xFF, 0xFF, 0x40, 0xFF },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
    },
    {
        { 0xFF, 0x80, 0x80, 0 },
        { 0xFF, 0x40, 0x20, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
    },
};
static HU3DPARMANPARAM lbl_1_data_206C = {
    0x14,
    0,
    2.0f,
    30.0f,
    15.0f,
    { 0.0f, 0.5f, 0.0f },
    20.0f,
    0.97f,
    50.0f,
    1.05f,
    2,
    {
        { 0xFF, 0xFF, 0xFF, 0xFF },
        { 0xC0, 0xC0, 0xE0, 0xFF },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
    },
    {
        { 0xA0, 0xA0, 0xA0, 0 },
        { 0xA0, 0xA0, 0xA0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
    },
};
static HU3DPARMANPARAM lbl_1_data_20BC = {
    0x19,
    0,
    4.0f,
    50.0f,
    5.0f,
    { 0.0f, -1.0f, 0.0f },
    0.0f,
    1.0f,
    40.0f,
    0.98f,
    2,
    {
        { 0xFF, 0xFF, 0, 0xFF },
        { 0xFF, 0x80, 0x80, 0xFF },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
    },
    {
        { 0xFF, 0xFF, 0, 0 },
        { 0xFF, 0x80, 0x80, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
    },
};

static s16 lbl_1_data_2148[12] = { 0xA, 0xB, 1, 0, 2, 4, 8, 3, 6, 5, 7, 9 };
static u8 lbl_1_data_216E[4][4] = {
    { 0xFF, 0x40, 0x40, 0xFF }, { 0xFF, 0xFF, 0x00, 0xFF }, { 0x00, 0x80, 0xFF, 0xFF }, { 0x40, 0xFF, 0x40, 0xFF },
};

/* falling confetti particle */
typedef struct EndingParticle_s {
    s32 active; //0x00
    s32 landed; //0x04
    HuVecF pos; //0x08
    HuVecF vel; //0x14
    HuVecF rot; //0x20
    HuVecF rotVel; //0x2C
    u8 color[4]; //0x38
    s32 landTime; //0x3C
} ENDING_PARTICLE;

typedef struct EndingConfetti_s {
    s16 modelId; //0x00
    s16 hookId; //0x02
    float floorY; //0x04
    s32 count; //0x08
} ENDING_CONFETTI;

HUPROCESS *lbl_1_bss_C38;
HUPROCESS *lbl_1_bss_C3C;
ENDING_CONFETTI lbl_1_bss_C40;
ENDING_PARTICLE lbl_1_bss_C4C[50];
ANIMDATA *lbl_1_bss_18CC[2];
s16 lbl_1_bss_18D4;
s32 lbl_1_bss_18D8;
s16 lbl_1_bss_18DC;
HUPROCESS *lbl_1_bss_18E0;
HUPROCESS *lbl_1_bss_18E4;
ANIMDATA *lbl_1_bss_18EC;
void *lbl_1_bss_18F0;
SDMACHINE_DISP lbl_1_bss_18F4[2];
s16 lbl_1_bss_194C;
s16 lbl_1_bss_194E[5];
s16 lbl_1_bss_1958[3];
s16 lbl_1_bss_195E[17];

/* from tournament.c / garage.c / picture.c */
extern s32 lbl_1_data_1DAC[12];
extern s16 lbl_1_bss_568[2];
extern void fn_1_1C13C(void);

static void fn_1_21E04(void);
static void fn_1_22EE8(s32 on);
static void fn_1_22FB0(s32 on);
static void fn_1_230A0(void);
static void fn_1_23578(void);
static void fn_1_23ABC(s16 tie);
static void fn_1_245F0(void);
static void fn_1_247F0(void);
static void fn_1_248F4(void);
static void fn_1_24B24(SDMACHINE_DISP *disp, float dt);
static void fn_1_24D6C(void);
static void fn_1_24DC0(HU3DMODEL *model, Mtx *mtx);
static void fn_1_24F14(void);
static void fn_1_25040(void);
void fn_1_2515C(void);
static void fn_1_25198(HuVecF *pos, s32 num);
void fn_1_22C44(void);
s32 fn_1_24D20(s16 charNo);
void fn_1_24758(s16 time);
float fn_1_22E40(float z);

#define PI_D 3.141592653589793
#define MESS_END(n) (0x1C0000 | (n))
#define DISP(i) (&lbl_1_bss_18F4[i])
#define MDL(i) (lbl_1_bss_195E[i])

// NONMATCHING: register allocation / float scheduling differ (logic reconstructed from asm)
void fn_1_20870(void)
{
    HuVecF pos;
    HuVec2f sz;
    u8 empty = 0;
    u32 mess1, mess2, mess3;
    float t, a, b, ang, scale;
    int sfx = -1;
    s16 i, j, win, charNo;
    float wx, wy;

    fn_1_21E04();
    omCameraViewSetMulti(4, &lbl_1_data_1E18);
    HuAudBGMPlay(0x25);
    WipeCreate(1, 0, 60);
    HuPrcSleep(10);
    omCameraViewMoveMulti(4, &lbl_1_data_1DFC, 100, 2);
    while (WipeCheckEnd() != 0) {
        HuPrcVSleep();
    }
    HuPrcSleep(90);
    for (i = 0; i < 2; i++) {
        if (i == 1) {
            omCameraViewSetMulti(4, &lbl_1_data_1EA4);
        }
        charNo = GwSdBattle.machine[i].charNo;
        mess1 = fn_1_24D20(charNo);
        mess2 = (i == 0) ? MESS_END(2) : MESS_END(4);
        HuWinInsertMesSizeGet(mess1, 0);
        HuWinMesMaxSizeGet(1, &sz, mess2);
        win = HuWinExCreate(-10000.0f, 300.0f, (s32)sz.x, (s32)sz.y, -1);
        HuWinExOpen(win);
        HuWinInsertMesSet(win, mess1, 0);
        fn_1_24758(60);
        HuWinMesSet(win, mess2);
        HuAudFXPlay(0x2EE);
        HuWinMesWait(win);
        HuAudSStreamAllFadeOut(500);
        HuWinExClose(win);
        HuWinKill(win);
        if (i == 1) {
            fn_1_4534(DISP(1), 400.0f, 0.0f, 310.0f);
            fn_1_4590(DISP(1), 0.0f, 180.0 + 180.0 * (atan2(310.0, 400.0) / PI_D), 0.0f);
        }
        if (i == 0) {
            HuAudJinglePlay(0x2B);
        } else {
            HuAudBGMPlay(0x4A);
        }
        Hu3DModelAttrSet(MDL(3), 1);
        fn_1_22EE8(1);
        fn_1_22FB0(1);
        Hu3DGLightColorSet(lbl_1_bss_18DC, 0, 0, 0, 0xFF);
        Hu3DModelAttrSet(MDL(5), 1);
        Hu3DModelAttrSet(MDL(4), 1);
        Hu3DModelAttrSet(MDL(6), 1);
        Hu3DMotionTimeSet(MDL(2), 0.0f);
        Hu3DModelAttrSet(MDL(2), 0x40000002);
        omCameraViewSetMulti(4, &lbl_1_data_1E34);
        omCameraViewSetMulti(1, &lbl_1_data_1DE0);
        HuPrcSleep(30);
        Hu3DMotionShapeTimeSet(MDL(6), 0.0f);
        Hu3DModelAttrReset(MDL(6), 1);
        Hu3DModelAttrReset(MDL(6), 0x40000080);
        Hu3DModelAttrReset(MDL(2), 0x40000002);
        Hu3DMotionCalc(MDL(2));
        HuAudFXPlay(0x515);
        while (Hu3DMotionEndCheck(MDL(2)) == 0) {
            Hu3DModelObjPosGet(MDL(2), "daidai01", &pos);
            fn_1_4534(DISP(i), pos.x, pos.y, pos.z);
            fn_1_46C4(DISP(i), 1);
            if (Hu3DMotionTimeGet(MDL(2)) == 90.0f) {
                sfx = HuAudFXPlay(0x516);
            }
            if (Hu3DMotionTimeGet(MDL(2)) == 120.0f) {
                HuAudFXPlay(0x517);
                fn_1_230A0();
            }
            HuPrcVSleep();
        }
        if (sfx != -1) {
            HuAudFXStop(sfx);
        }
        HuAudFXPlay(0x51C);
        Hu3DModelObjPosGet(MDL(2), "daidai01", &pos);
        fn_1_4534(DISP(i), pos.x, pos.y, pos.z);
        HuPrcSleep(30);
        charNo = GwSdBattle.machine[i].charNo;
        if (charNo < 0x20) {
            mess1 = MESSNUM_PTR(&empty);
        } else {
            mess1 = charNo + 0x1CFFED;
        }
        mess2 = fn_1_24D20(charNo);
        if (charNo < 0x20) {
            mess3 = MESSNUM_PTR(GwSdBattle.machine[i].name);
        } else {
            mess3 = charNo + 0x1CFFE1;
        }
        HuWinInsertMesSizeGet(mess1, 0);
        HuWinInsertMesSizeGet(mess2, 1);
        HuWinInsertMesSizeGet(mess3, 2);
        HuWinMesMaxSizeGet(1, &sz, MESS_END(2));
        win = HuWinCreate(-10000.0f, 364.0f, (s32)sz.x, (s32)sz.y, 0);
        HuWinInsertMesSet(win, mess1, 0);
        HuWinInsertMesSet(win, mess2, 1);
        HuWinInsertMesSet(win, mess3, 2);
        HuWinMesSpeedSet(win, 0);
        HuWinMesSet(win, MESS_END(3));
        wx = winData[win].pos.x;
        wy = winData[win].pos.y;
        HuWinPosSet(win, -1000.0f, -1000.0f);
        for (j = 1; j <= 300; j++) {
            t = j / 300.0;
            a = fn_1_22E40(pos.z + 2000.0f * t - 50.0f);
            b = fn_1_22E40(50.0f + (pos.z + 2000.0f * t));
            ang = 180.0 * (atan2(a - b, 100.0) / PI_D);
            fn_1_4534(DISP(i), pos.x, (a + b) * 0.5f, pos.z + 2000.0f * t);
            fn_1_4590(DISP(i), ang, 0.0f, 0.0f);
            CenterM[2].z = -3100.0f + 2000.0f * t;
            CenterM[0].z = CenterM[2].z;
            CenterM[0].y = 148.0f + (a + b) * 0.5f;
            CenterM[2].y = 260.0f + (a + b) * 0.5f;
            if (j > 60 && j <= 80) {
                scale = 1.0 - (j - 60) / 20.0;
                HuWinPosSet(win, wx + 640.0 * scale, wy);
            }
            fn_1_24B24(DISP(i), 1.0f);
            HuPrcVSleep();
        }
        fn_1_22FB0(0);
        HuWinKill(win);
        Hu3DModelAttrReset(MDL(3), 1);
        fn_1_22EE8(0);
        Hu3DGLightColorSet(lbl_1_bss_18DC, 0xC0, 0xC0, 0xC0, 0xFF);
        omCameraViewSetMulti(4, &lbl_1_data_1EC0);
        omCameraViewSetMulti(1, &lbl_1_data_1E50[0]);
        Hu3DModelRotSet(MDL(8), 0.0f, -90.0f, 0.0f);
        Hu3DModelPosSet(MDL(8), 0.0f, 150.0f, 350.0f);
        for (j = 1; j <= 60; j++) {
            t = 1.0 - sin(PI_D * (90.0 * (j / 60.0)) / 180.0);
            fn_1_4534(DISP(i), 0.0f, 0.0f, -500.0f * t);
            fn_1_4590(DISP(i), 0.0f, 0.0f, 0.0f);
            fn_1_24B24(DISP(i), 0.3 + 0.7 * t);
            HuPrcVSleep();
        }
        Hu3DModelAttrReset(MDL(16), 1);
        Hu3DModelAttrReset(MDL(16), 0x40000002);
        Hu3DMotionSpeedSet(MDL(16), 3.0f);
        Hu3DMotionTimeSet(MDL(16), 0.0f);
        Hu3DModelScaleSet(MDL(16), 1.0f, 1.0f, 1.0f);
        HuAudFXPlay(0x51D);
        for (j = 1; j <= 50; j++) {
            t = sin(PI_D * (90.0 * (j / 50.0)) / 180.0);
            fn_1_4590(DISP(i), 0.0f, 720.0f * t, 0.0f);
            scale = 1.0 + 0.2 * t;
            Hu3DModelScaleSet(MDL(16), scale, 1.0f, scale);
            HuPrcVSleep();
        }
        HuPrcSleep(30);
        Hu3DModelAttrSet(MDL(16), 1);
        while (omCameraViewCheck(4) == 0) {
            HuPrcVSleep();
        }
    }
    HuAudSStreamAllFadeOut(500);
    HuAudSStreamVolSet(2, 0, 0);
    HuAudJinglePlay(0x25);
    WipeCreate(2, 0x81, 10);
    HuPrcVSleep();
    HuAudSStreamVolSet(2, 0x7F, 500);
    while (WipeCheckEnd() != 0) {
        HuPrcVSleep();
    }
    omCameraViewSetMulti(1, &lbl_1_data_1EF8);
    omCameraViewSetMulti(4, &lbl_1_data_1EDC);
    fn_1_4534(DISP(1), -400.0f, 0.0f, -310.0f);
    fn_1_4590(DISP(1), 0.0f, 180.0 + 180.0 * (atan2(-310.0, -400.0) / PI_D), 0.0f);
    Hu3DGLightColorSet(lbl_1_bss_18DC, 0xC0, 0xC0, 0xC0, 0xFF);
    Hu3DModelPosSet(MDL(8), 0.0f, 400.0f, 0.0f);
    Hu3DModelRotSet(MDL(8), 0.0f, 0.0f, 0.0f);
    fn_1_22FB0(0);
    WipeCreate(1, 0x81, 60);
    while (WipeCheckEnd() != 0) {
        HuPrcVSleep();
    }
    HuPrcSleep(30);
    Hu3DModelPosSet(MDL(7), 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(MDL(7), 0.0f, 0.0f, 0.0f);
    Hu3DParManAttrReset(lbl_1_bss_194E[4], 1);
    for (j = 1; j <= 50; j++) {
        t = j / 50.0;
        Hu3DModelRotSet(MDL(8), 0.0f, 720.0 * (1.0 - cos(PI_D * (180.0f * t) / 180.0)), 0.0f);
        if (j > 30) {
            t = (j - 30) / 20.0;
            Hu3DModelScaleSet(MDL(7), t, 1.0f, t);
        }
        HuPrcVSleep();
    }
    Hu3DParManAttrSet(lbl_1_bss_194E[4], 1);
    HuPrcSleep(30);
    HuWinMesMaxSizeGet(1, &sz, MESS_END(5));
    HuAudFXPlay(0x2EF);
    win = HuWinExCreate(-10000.0f, 364.0f, (s32)sz.x, (s32)sz.y, -1);
    HuWinExOpen(win);
    fn_1_24758(60);
    HuWinMesSet(win, MESS_END(5));
    HuWinMesWait(win);
    HuWinExClose(win);
    HuWinKill(win);
    lbl_1_bss_18D8 = 0;
    HuAudFXPlay(0x518);
    for (j = 1; j <= 30; j++) {
        t = j / 30.0;
        fn_1_4534(DISP(0), 400.0 * (1.0 - t), 300.0 * sin(PI_D * (180.0f * t) / 180.0), 310.0 * (1.0 - t));
        scale = 0.5 + 0.5 * (1.0 - t);
        fn_1_45E4(DISP(0), scale, scale, scale);
        HuPrcVSleep();
    }
    fn_1_46C4(DISP(0), 0);
    HuAudFXPlay(0x518);
    for (j = 1; j <= 30; j++) {
        t = j / 30.0;
        fn_1_4534(DISP(1), -400.0 * (1.0 - t), 300.0 * sin(PI_D * (180.0f * t) / 180.0), -310.0 * (1.0 - t));
        scale = 0.5 + 0.5 * (1.0 - t);
        fn_1_45E4(DISP(1), scale, scale, scale);
        HuPrcVSleep();
    }
    fn_1_46C4(DISP(1), 0);
    WipeCreate(2, 0, 60);
    HuAudSStreamAllFadeOut(1000);
    while (WipeCheckEnd() != 0) {
        HuPrcVSleep();
    }
}

/* creates every model / sprite / particle emitter of the ending scene */
// NONMATCHING: register allocation / float scheduling differ (logic reconstructed from asm)
static void fn_1_21E04(void)
{
    s16 i, id;
    ANIMDATA *anim;
    HU3DMODELID m;

    m = Hu3DModelCreate(HuDataSelHeapReadNum(0xBD0000, 0x10000000, HEAP_MODEL));
    MDL(0) = m;
    Hu3DModelAttrSet(m, 0x4000);
    Hu3DModelAttrSet(m, 0x40000001);
    Hu3DModelCameraSet(m, 5);
    m = Hu3DModelCreate(HuDataSelHeapReadNum(0xBD0001, 0x10000000, HEAP_MODEL));
    MDL(1) = m;
    Hu3DModelAttrSet(m, 0x4000);
    Hu3DModelAttrSet(m, 0x40000001);
    Hu3DModelCameraSet(m, 5);
    m = Hu3DModelCreate(HuDataSelHeapReadNum(0xBD0002, 0x10000000, HEAP_MODEL));
    MDL(2) = m;
    Hu3DModelAttrSet(m, 0x4000);
    Hu3DModelAttrSet(m, 0x40000002);
    Hu3DModelCameraSet(m, 5);
    m = Hu3DModelCreate(HuDataSelHeapReadNum(0xBD0003, 0x10000000, HEAP_MODEL));
    MDL(3) = m;
    Hu3DModelAttrSet(m, 0x4000);
    Hu3DModelAttrSet(m, 0x40000001);
    Hu3DModelCameraSet(m, 4);
    m = Hu3DModelCreate(HuDataSelHeapReadNum(0xBD0005, 0x10000000, HEAP_MODEL));
    MDL(4) = m;
    Hu3DModelAttrSet(m, 0x4000);
    Hu3DModelAttrSet(m, 0x40000001);
    Hu3DModelCameraSet(m, 5);
    m = Hu3DModelCreate(HuDataSelHeapReadNum(0xBD0006, 0x10000000, HEAP_MODEL));
    MDL(5) = m;
    Hu3DModelAttrSet(m, 0x4000);
    Hu3DModelAttrSet(m, 0x40000001);
    Hu3DModelCameraSet(m, 5);
    Hu3DModelPosSet(m, 0.0f, 1.0f, 0.0f);
    Hu3DModelScaleSet(m, 1.0f, 3.0f, 1.0f);
    Hu3DModelLayerSet(m, 2);
    m = Hu3DModelCreate(HuDataSelHeapReadNum(0xBD0007, 0x10000000, HEAP_MODEL));
    MDL(6) = m;
    Hu3DModelAttrSet(m, 0x4000);
    Hu3DModelAttrSet(m, 1);
    Hu3DModelAttrSet(m, 0x40000080);
    Hu3DModelCameraSet(m, 5);
    m = Hu3DModelCreate(HuDataSelHeapReadNum(0xBD0004, 0x10000000, HEAP_MODEL));
    MDL(7) = m;
    Hu3DModelAttrSet(m, 0x4000);
    Hu3DModelAttrSet(m, 0x40000001);
    Hu3DModelCameraSet(m, 4);
    Hu3DModelPosSet(m, 0.0f, -1000.0f, 0.0f);
    m = Hu3DModelCreate(HuDataSelHeapReadNum(0xBE0011, 0x10000000, HEAP_MODEL));
    MDL(8) = m;
    Hu3DModelAttrSet(m, 0x4000);
    Hu3DModelAttrSet(m, 0x40000001);
    Hu3DModelCameraSet(m, 5);
    Hu3DModelPosSet(m, 0.0f, 200.0f, 0.0f);
    Hu3DModelScaleSet(m, 1.5f, 1.5f, 1.5f);
    m = Hu3DModelCreate(HuDataSelHeapReadNum(0xBD0000 + sdWorkInt[3] * 3 + sdWorkInt[6] + 0xA, 0x10000000, HEAP_MODEL));
    MDL(9) = m;
    Hu3DModelReflectTypeSet(m, 2);
    Hu3DModelAttrSet(m, 0x4001);
    Hu3DModelCameraSet(m, 5);
    m = Hu3DModelCreate(HuDataSelHeapReadNum(0xBD0008, 0x10000000, HEAP_MODEL));
    MDL(10) = m;
    Hu3DModelAttrSet(m, 0x40000001);
    Hu3DModelCameraSet(m, 4);
    m = Hu3DModelCreate(HuDataSelHeapReadNum(0xBD0009, 0x10000000, HEAP_MODEL));
    MDL(11) = m;
    Hu3DModelAttrSet(m, 1);
    Hu3DModelAttrSet(m, 0x40000001);
    Hu3DModelCameraSet(m, 5);
    fn_1_22FB0(0);
    m = Hu3DModelCreate(HuDataSelHeapReadNum(0xBD0014, 0x10000000, HEAP_MODEL));
    MDL(12) = m;
    Hu3DModelCameraSet(m, 4);
    Hu3DModelScaleSet(m, 1.5f, 1.5f, 1.5f);
    for (i = 0; i < 3; i++) {
        m = Hu3DModelCreate(HuDataSelHeapReadNum(0xBD0013, 0x10000000, HEAP_MODEL));
        MDL(13 + i) = m;
        Hu3DModelAttrSet(m, 1);
        Hu3DModelCameraSet(m, 4);
        Hu3DModelScaleSet(m, 4.0f, 0.0f, 4.0f);
        Hu3DModelLayerSet(m, 1);
    }
    m = Hu3DModelCreate(HuDataSelHeapReadNum(0xBD0016, 0x10000000, HEAP_MODEL));
    MDL(16) = m;
    Hu3DModelAttrSet(m, 1);
    Hu3DModelCameraSet(m, 4);
    lbl_1_bss_1958[0] = Hu3DJointMotion(MDL(8), HuDataSelHeapReadNum(0xBE0012, 0x10000000, HEAP_MODEL));
    lbl_1_bss_1958[1] = Hu3DJointMotion(MDL(8), HuDataSelHeapReadNum(0xBE0013, 0x10000000, HEAP_MODEL));
    lbl_1_bss_1958[2] = Hu3DJointMotion(MDL(8), HuDataSelHeapReadNum(0xBE0014, 0x10000000, HEAP_MODEL));
    Hu3DMotionSet(MDL(8), lbl_1_bss_1958[0]);
    Hu3DModelAttrSet(MDL(8), 0x40000001);
    id = HuSprGrpCreate(1);
    lbl_1_bss_194C = id;
    anim = HuSprAnimRead(HuDataSelHeapReadNum(0xBD001F, 0x10000000, HEAP_MODEL));
    HuSprGrpMemberSet(id, 0, HuSprCreate(anim, 100, 0));
    HuSprAttrSet(id, 0, 4);
    HuSprPriSet(id, 0, 100);
    HuDataDirClose(0xBE0000);
    HuDataDirClose(0xBD0000);
    for (i = 0; i < 2; i++) {
        s8 charNo;
        fn_1_480C(DISP(i));
        charNo = GwSdBattle.machine[i].charNo;
        if (charNo < 0x20) {
            fn_1_42E0(DISP(i), GwSdBattle.machine[i].partNo[0], GwSdBattle.machine[i].partNo[1], GwSdBattle.machine[i].partNo[3], charNo);
        } else {
            s16 k = charNo - 0x20;
            fn_1_42E0(DISP(i), lbl_1_data_1FBC[k][0], lbl_1_data_1FBC[k][1], lbl_1_data_1FBC[k][2], lbl_1_data_1FBC[k][3]);
        }
        fn_1_4534(DISP(i), i * 400 - 200, 0.0f, 0.0f);
        fn_1_4638(DISP(i), 5);
        fn_1_46C4(DISP(i), 0);
        Hu3DModelAttrReset(DISP(i)->headMdl, 0x40000082);
        Hu3DModelAttrSet(DISP(i)->headMdl, 0x40000041);
    }
    HuDataDirClose(0xBC0000);
    Hu3DCameraPerspectiveSet(1, 40.0f, 30.0f, 15000.0f, 1.2f);
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, 256.0f, 192.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(1, 0, 0, 0x100, 0xC0);
    lbl_1_bss_18F0 = HuMemDirectMallocNum(HEAP_HEAP, GXGetTexBufferSize(0x100, 0xC0, 5, 0, 0), 0x10000000);
    Hu3DLayerHookSet(9, (HU3DLAYERHOOK)fn_1_22C44);
    lbl_1_bss_18EC = HuSprAnimMake(0x100, 0xC0, 2);
    lbl_1_bss_18EC->bmp->data = lbl_1_bss_18F0;
    lbl_1_bss_18D4 = Hu3DAnimCreate(lbl_1_bss_18EC, MDL(0), "dummy");
    anim = HuSprAnimRead(HuDataSelHeapReadNum(0xBD001C, 0x10000000, HEAP_MODEL));
    for (i = 0; i < 2; i++) {
        id = Hu3DParManCreate(anim, 200, &lbl_1_data_201C);
        lbl_1_bss_194E[i] = id;
        Hu3DParManAttrSet(id, 0x69);
        Hu3DParticleBlendModeSet(Hu3DParManModelIDGet(id), 1);
        Hu3DModelLayerSet(Hu3DParManModelIDGet(id), 1);
        Hu3DModelCameraSet(Hu3DParManModelIDGet(id), 4);
    }
    id = Hu3DParManCreate(anim, 100, &lbl_1_data_20BC);
    lbl_1_bss_194E[4] = id;
    Hu3DParManAttrSet(id, 0x61);
    Hu3DParticleBlendModeSet(Hu3DParManModelIDGet(id), 1);
    Hu3DModelLayerSet(Hu3DParManModelIDGet(id), 1);
    Hu3DModelCameraSet(Hu3DParManModelIDGet(id), 4);
    Hu3DParManPosSet(lbl_1_bss_194E[4], 0.0f, 400.0f, -50.0f);
    Hu3DParManVecSet(lbl_1_bss_194E[4], 0.0f, -1.0f, 0.0f);
    anim = HuSprAnimRead(HuDataSelHeapReadNum(0xBD001D, 0x10000000, HEAP_MODEL));
    for (i = 2; i < 4; i++) {
        id = Hu3DParManCreate(anim, 40, &lbl_1_data_206C);
        lbl_1_bss_194E[i] = id;
        Hu3DParManAttrSet(id, 0x23);
        Hu3DParticleBlendModeSet(Hu3DParManModelIDGet(id), 1);
        Hu3DModelLayerSet(Hu3DParManModelIDGet(id), 1);
        Hu3DModelCameraSet(Hu3DParManModelIDGet(id), 5);
    }
    Hu3DGLightKill(lbl_1_bss_568[1]);
    lbl_1_bss_18DC = Hu3DGLightCreate(0.0f, 100.0f, 1000.0f, 0.0f, -0.1f, -1.0f, 0x80, 0x80, 0x80);
    Hu3DGLightInfinitytSet(lbl_1_bss_18DC);
    lbl_1_bss_18E4 = HuPrcChildCreate(fn_1_248F4, 1000, 0x3000, 0, HuPrcCurrentGet());
    lbl_1_bss_18D8 = 1;
    fn_1_25040();
}

void fn_1_22C44(void)
{
    if (Hu3DCameraNo != 0) {
        return;
    }
    Hu3DFbCopyExec(0, 0, 0x100, 0xC0, GX_TF_RGB5A3, 0, lbl_1_bss_18F0);
    Hu3DZClear();
}

/* debug: move the left machine with the second pad and print its position */
// NONMATCHING: register allocation / float scheduling differ (logic reconstructed from asm)
void fn_1_22CA0(void)
{
    HuVecF pos;

    pos.x = 0.0f;
    pos.y = -190.0f;
    pos.z = -3100.0f;
    while (1) {
        pos.x += HuPadStkX[1] / 5;
        if (HuPadBtn[1] & 0x100) {
            pos.y += 1.0f;
        }
        if (HuPadBtn[1] & 0x200) {
            pos.y -= 1.0f;
        }
        pos.z += HuPadStkY[1] / 5;
        fn_1_4534(DISP(0), pos.x, pos.y, pos.z);
        if (HuPadBtnDown[1] & 0x400) {
            OSReport("%f,%f,%f\n", pos.x, pos.y, pos.z);
        }
        HuPrcVSleep();
    }
}

// NONMATCHING: floating-point instruction selection and register allocation differ
float fn_1_22E40(float z)
{
    if (z < -2700.0f) {
        return -180.0f;
    }
    if (z > -765.0f) {
        return 0.0f;
    }
    return -180.0f * (1.0 - (z - -2700.0f) / 1935.0f);
}

// NONMATCHING: register allocation / float scheduling differ (logic reconstructed from asm)
static void fn_1_22EE8(s32 on)
{
    if (on != 0) {
        Hu3DModelAttrReset(MDL(10), 1);
        return;
    }
    Hu3DModelAttrSet(MDL(10), 1);
    Hu3DGLightColorSet(Hu3DModelLightIdGet(MDL(10), "d1_p-inf_light3"), 0, 0, 0, 0xFF);
    Hu3DGLightColorSet(Hu3DModelLightIdGet(MDL(10), "d1_p-inf_light4"), 0, 0, 0, 0xFF);
}

static void fn_1_22FB0(s32 on)
{
    if (on != 0) {
        Hu3DMotionTimeSet(MDL(1), Hu3DMotionTimeGet(MDL(11)));
        Hu3DModelAttrReset(MDL(1), 1);
        Hu3DModelAttrSet(MDL(1), 0x40000001);
        Hu3DModelAttrSet(MDL(11), 1);
        return;
    }
    Hu3DMotionTimeSet(MDL(11), Hu3DMotionTimeGet(MDL(1)));
    Hu3DModelAttrReset(MDL(11), 1);
    Hu3DModelAttrSet(MDL(11), 0x40000001);
    Hu3DModelAttrSet(MDL(1), 1);
}

static void fn_1_230A0(void)
{
    s16 i;

    for (i = 0; i < 2; i++) {
        if (i & 1) {
            Hu3DParManPosSet(lbl_1_bss_194E[i], 670.0f, -175.0f, -2840.0f);
        } else {
            Hu3DParManPosSet(lbl_1_bss_194E[i], -670.0f, -175.0f, -2840.0f);
        }
        Hu3DParManTimeLimitSet(lbl_1_bss_194E[i], 120);
        Hu3DParManAttrReset(lbl_1_bss_194E[i], 1);
    }
    Hu3DParManVecSet(lbl_1_bss_194E[0], 0.8f, 1.0f, 0.0f);
    Hu3DParManVecSet(lbl_1_bss_194E[1], -0.8f, 1.0f, 0.0f);
    Hu3DParManVecSet(lbl_1_bss_194E[2], 0.8f, 1.0f, 0.0f);
    Hu3DParManVecSet(lbl_1_bss_194E[3], -0.8f, 1.0f, 0.0f);
}

/* ending scene process */
// NONMATCHING: register allocation / float scheduling differ (logic reconstructed from asm)
void fn_1_23280(void)
{
    s16 tie = 0;
    s16 i;

    fn_1_21E04();
    Hu3DModelAttrSet(MDL(10), 1);
    Hu3DGLightColorSet(Hu3DModelLightIdGet(MDL(10), "d1_p-inf_light3"), 0, 0, 0, 0xFF);
    Hu3DGLightColorSet(Hu3DModelLightIdGet(MDL(10), "d1_p-inf_light4"), 0, 0, 0, 0xFF);
    Hu3DMotionTimeSet(MDL(11), Hu3DMotionTimeGet(MDL(1)));
    Hu3DModelAttrReset(MDL(11), 1);
    Hu3DModelAttrSet(MDL(11), 0x40000001);
    Hu3DModelAttrSet(MDL(1), 1);
    Hu3DModelAttrSet(MDL(6), 1);
    Hu3DModelCameraSet(MDL(0), 4);
    fn_1_4638(DISP(0), 4);
    fn_1_4638(DISP(1), 4);
    lbl_1_bss_18E0 = HuPrcChildCreate(fn_1_245F0, 1000, 0x3000, 0, HuPrcCurrentGet());
    if (GwSdBattle.winnerNo == -1) {
        fn_1_23578();
        tie = 1;
    }
    HuAudJinglePlay(0x47);
    fn_1_23ABC(tie);
    WipeCreate(2, 0, 60);
    HuAudSStreamAllFadeOut(1000);
    while (WipeCheckEnd() != 0) {
        HuPrcVSleep();
    }
    if (lbl_1_bss_C3C != NULL) {
        HuPrcKill(lbl_1_bss_C3C);
    }
    HuPrcKill(lbl_1_bss_18E4);
    Hu3DLayerHookReset(9);
    if (lbl_1_bss_C38 != NULL) {
        HuPrcKill(lbl_1_bss_C38);
    }
    for (i = 0; i < 17; i++) {
        Hu3DModelKill(MDL(i));
    }
    for (i = 0; i < 3; i++) {
        Hu3DMotionKill(lbl_1_bss_1958[i]);
    }
    fn_1_4724(DISP(0));
    fn_1_4724(DISP(1));
    HuSprGrpKill(lbl_1_bss_194C);
    fn_1_2515C();
    HuPrcKill(lbl_1_bss_18E0);
}

/* tie breaker: the machine with the higher (randomised on a tie) score wins */
// NONMATCHING: register allocation / float scheduling differ (logic reconstructed from asm)
static void fn_1_23578(void)
{
    char buf0[8];
    char buf1[8];
    HuVec2f sz;
    s16 score[2];
    s16 i;
    s16 win;
    s16 winner;
    s32 charNo;
    u32 nameMess;
    HUWIN *w;

    HuAudBGMPlay(0x25);
    omCameraViewSetMulti(1, &lbl_1_data_1F14);
    omCameraViewSetMulti(4, &lbl_1_data_1F30);
    for (i = 0; i < 2; i++) {
        fn_1_4534(DISP(i), i * 600 - 300, 0.0f, 0.0f);
        fn_1_46C4(DISP(i), 1);
    }
    Hu3DModelPosSet(MDL(8), 0.0f, 200.0f, 0.0f);
    Hu3DModelAttrSet(MDL(5), 1);
    Hu3DModelAttrSet(MDL(4), 1);
    WipeCreate(1, 0, 60);
    HuPrcSleep(10);
    omCameraViewMoveMulti(4, &lbl_1_data_1F4C, 90, 2);
    while (WipeCheckEnd() != 0) {
        HuPrcVSleep();
    }
    HuPrcSleep(30);
    score[0] = sdHealthLoss[0];
    score[1] = sdHealthLoss[1];
    if (score[0] == score[1]) {
        if (frandmod(0) != 0) {
            score[0]++;
        } else {
            score[1]++;
        }
    }
    if (sdWorkInt[3] == 0) {
        score[0] = 50 - score[0];
        score[1] = 50 - score[1];
    } else {
        score[0] += 50;
        score[1] += 50;
    }
    if (score[0] > score[1]) {
        winner = 0;
    } else {
        winner = 1;
    }
    sprintf(buf0, "%d", score[0]);
    sprintf(buf1, "%d", score[1]);
    charNo = GwSdBattle.machine[winner].charNo;
    nameMess = fn_1_24D20(charNo);
    HuWinInsertMesSizeGet(MESSNUM_PTR(buf0), 0);
    HuWinInsertMesSizeGet(MESSNUM_PTR(buf1), 1);
    HuWinInsertMesSizeGet(MESS_END(0xE) + winner, 2);
    HuWinInsertMesSizeGet(nameMess, 3);
    HuWinMesMaxSizeBetGet(&sz, MESS_END(6), MESS_END(7));
    win = HuWinExCreate(-10000.0f, 364.0f, (s32)sz.x, (s32)sz.y, -1);
    HuWinInsertMesSet(win, MESSNUM_PTR(buf0), 0);
    HuWinInsertMesSet(win, MESSNUM_PTR(buf1), 1);
    HuWinInsertMesSet(win, MESS_END(0xE) + winner, 2);
    HuWinInsertMesSet(win, nameMess, 3);
    HuWinExOpen(win);
    fn_1_24758(60);
    HuWinMesSet(win, MESS_END(6));
    HuAudFXPlay(0x2EF);
    HuWinMesWait(win);
    fn_1_24758(60);
    HuWinMesSet(win, MESS_END(7));
    w = &winData[win];
    while (w->stat != 2) {
        HuPrcVSleep();
    }
    while (w->stat != 1) {
        HuPrcVSleep();
    }
    HuAudFXPlay(0x2EE);
    HuWinMesWait(win);
    GwSdBattle.winnerNo = winner;
    Hu3DModelAttrReset(MDL(5), 1);
    Hu3DModelScaleSet(MDL(5), 1.0f, 2.0f, 1.0f);
    Hu3DModelPosSet(MDL(5), winner * 600 - 300, 5.0f, 0.0f);
    if (winner == 0) {
        HuAudFXPlay(0x51F);
    } else {
        HuAudFXPlay(0x520);
    }
    HuWinExClose(win);
    HuWinKill(win);
    HuPrcSleep(60);
    HuAudSStreamAllFadeOut(1000);
    WipeCreate(2, 0, 60);
    while (WipeCheckEnd() != 0) {
        HuPrcVSleep();
    }
}

static void fn_1_23ABC_saveMachine(GWSDMACHINE *dst, GWSDMACHINE *src)
{
    *dst = *src;
}

/* result screen; offers to save the winning machine as a picture */
// NONMATCHING: register allocation / float scheduling differ (logic reconstructed from asm)
static void fn_1_23ABC(s16 tie)
{
    char buf[8];
    s16 wins[3];
    s16 winner;
    s16 charNo;
    s16 i;
    s16 win;
    s16 choice;
    u32 nameMess;
    HuVec2f sz;

    omCameraViewSetMulti(1, &lbl_1_data_1F68);
    if (tie != 0) {
        omCameraViewSetMulti(4, &lbl_1_data_1FA0);
    } else {
        omCameraViewSetMulti(4, &lbl_1_data_1F84);
    }
    winner = GwSdBattle.winnerNo;
    charNo = GwSdBattle.machine[winner].charNo;
    fn_1_4534(DISP(winner), 200.0f, 0.0f, 0.0f);
    fn_1_4590(DISP(winner), 0.0f, -15.0f, 0.0f);
    fn_1_46C4(DISP(winner), 1);
    fn_1_46C4(DISP(winner ^ 1), 0);
    Hu3DModelPosSet(MDL(9), -200.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(MDL(9), 0.0f, 15.0f, 0.0f);
    Hu3DModelScaleSet(MDL(9), 3.0f, 3.0f, 3.0f);
    Hu3DModelPosSet(MDL(8), 0.0f, 200.0f, 0.0f);
    Hu3DMotionSet(MDL(8), lbl_1_bss_1958[2]);
    Hu3DModelAttrSet(MDL(8), 0x40000001);
    Hu3DModelAttrReset(MDL(5), 1);
    Hu3DModelScaleSet(MDL(5), 1.5f, 2.0f, 1.5f);
    Hu3DModelPosSet(MDL(5), 0.0f, 5.0f, 0.0f);
    Hu3DModelAttrSet(MDL(4), 1);
    Hu3DModelAttrReset(MDL(9), 1);
    WipeCreate(1, 0, 60);
    if (tie == 0) {
        HuPrcSleep(10);
        omCameraViewMoveMulti(4, &lbl_1_data_1FA0, 80, 2);
        while (WipeCheckEnd() != 0) {
            HuPrcVSleep();
        }
    }
    while (WipeCheckEnd() != 0) {
        HuPrcVSleep();
    }
    HuPrcSleep(30);
    Hu3DModelAttrSet(MDL(3), 1);
    sprintf(buf, "%d", GwSdCommon.picNo + 1);
    if (charNo < 0x20) {
        nameMess = MESSNUM_PTR(GwSdBattle.machine[winner].name);
    } else {
        nameMess = charNo + 0x1CFFED;
    }
    HuSprPosSet(lbl_1_bss_194C, 0, 288.0f, 380.0f);
    HuSprAttrReset(lbl_1_bss_194C, 0, 4);
    for (i = 1; i <= 20; i++) {
        HuSprTPLvlSet(lbl_1_bss_194C, 0, i / 20.0);
        HuPrcVSleep();
    }
    win = HuWinCreate(-10000.0f, 332.0f, 0x200, 0x2A, 0);
    wins[0] = win;
    HuWinBGTPLvlSet(win, 0.0f);
    HuWinAttrSet(win, 0x800);
    HuWinMesSpeedSet(win, 0);
    HuWinPriSet(win, 50);
    HuWinInsertMesSet(win, MESSNUM_PTR(buf), 0);
    HuWinMesSet(win, MESS_END(8));
    HuPrcSleep(5);
    win = HuWinCreate(-10000.0f, 362.0f, 0x200, 0x2A, 0);
    wins[1] = win;
    HuWinBGTPLvlSet(win, 0.0f);
    HuWinAttrSet(win, 0x800);
    HuWinPriSet(win, 50);
    HuWinMesSpeedSet(win, 0);
    HuWinInsertMesSet(win, 0x190035 + sdWorkInt[3], 1);
    HuWinInsertMesSet(win, 0x190038 + sdWorkInt[6], 2);
    HuWinMesSet(win, MESS_END(9));
    HuPrcSleep(5);
    win = HuWinCreate(-10000.0f, 392.0f, 0x200, 0x2A, 0);
    wins[2] = win;
    HuWinBGTPLvlSet(win, 0.0f);
    HuWinAttrSet(win, 0x800);
    HuWinMesSpeedSet(win, 0);
    HuWinPriSet(win, 50);
    HuWinInsertMesSet(win, nameMess, 3);
    HuWinMesSet(win, MESS_END(0xA));
    lbl_1_bss_C3C = HuPrcChildCreate(fn_1_24D6C, 1000, 0x3000, 0, HuPrcCurrentGet());
    while ((fn_1_4FD0(3) & 0x100) == 0) {
        HuPrcVSleep();
    }
    for (i = 0; i < 3; i++) {
        HuWinKill(wins[i]);
    }
    for (i = 1; i <= 10; i++) {
        HuSprTPLvlSet(lbl_1_bss_194C, 0, 1.0 - i / 10.0);
        HuPrcVSleep();
    }
    if (winner != 0) {
        return;
    }
    HuWinMesMaxSizeGet(2, &sz, MESS_END(0x10), MESS_END(0x11));
    win = HuWinExCreate(-10000.0f, 364.0f, (s32)sz.x, (s32)sz.y, -1);
    HuWinExOpen(win);
    fn_1_24758(60);
    HuWinMesSet(win, MESS_END(0x10));
    HuWinMesWait(win);
    choice = HuWinChoiceGet(win, -1);
    if (choice == 0) {
        for (i = 0; i < 10; i++) {
            if (GwSdCommon.picture[i].number == 0xFFFF) {
                HuAudFXPlay(0x519);
                fn_1_23ABC_saveMachine(&GwSdCommon.picture[i].machine, &GwSdBattle.machine[winner]);
                GwSdCommon.picture[i].type = sdWorkInt[3];
                GwSdCommon.picture[i].diff = sdWorkInt[6];
                GwSdCommon.picture[i].number = GwSdCommon.picNo + 1;
                break;
            }
        }
        if (i == 10) {
            HuWinMesSet(win, MESS_END(0x11));
            fn_1_24758(60);
            HuWinMesWait(win);
            choice = HuWinChoiceGet(win, -1);
            if (choice == 0) {
                GwSdCommon.picture[0].number = 0xFFFF;
                fn_1_1C13C();
                fn_1_23ABC_saveMachine(&GwSdCommon.picture[9].machine, &GwSdBattle.machine[winner]);
                GwSdCommon.picture[9].type = sdWorkInt[3];
                GwSdCommon.picture[9].diff = sdWorkInt[6];
                GwSdCommon.picture[9].number = GwSdCommon.picNo + 1;
                HuAudFXPlay(0x519);
            }
        }
    }
    HuWinExClose(win);
    HuWinKill(win);
}

/* texture animation of the result sprite: alternates / selects the two machine pictures */
// NONMATCHING: register allocation / float scheduling differ (logic reconstructed from asm)
static void fn_1_245F0(void)
{
    HU3DTEXANIM *tex;
    s8 charNo;
    s16 n;

    charNo = GwSdBattle.machine[0].charNo;
    if (charNo >= 0x20) {
        charNo = lbl_1_data_2148[charNo - 0x20];
    }
    lbl_1_bss_18CC[0] = HuSprAnimRead(HuDataSelHeapReadNum(charNo + 0xBD0020, 0x10000000, HEAP_MODEL));
    charNo = GwSdBattle.machine[1].charNo;
    if (charNo >= 0x20) {
        charNo = lbl_1_data_2148[charNo - 0x20];
    }
    lbl_1_bss_18CC[1] = HuSprAnimRead(HuDataSelHeapReadNum(charNo + 0xBD0020, 0x10000000, HEAP_MODEL));
    tex = &Hu3DTexAnimData[lbl_1_bss_18D4];
    n = 0;
    while (1) {
        if (GwSdBattle.winnerNo == -1) {
            tex->anim = lbl_1_bss_18CC[n & 1];
        } else {
            tex->anim = lbl_1_bss_18CC[GwSdBattle.winnerNo & 1];
        }
        n++;
        HuPrcSleep(60);
    }
}

/* toggles the "waiting for a button" animation process of the message windows */
// NONMATCHING: register allocation / float scheduling differ (logic reconstructed from asm)
void fn_1_24758(s16 time)
{
    if (lbl_1_bss_C38 != NULL) {
        HuPrcKill(lbl_1_bss_C38);
        lbl_1_bss_C38 = NULL;
        return;
    }
    lbl_1_bss_C38 = HuPrcChildCreate(fn_1_247F0, 1000, 0x3000, 0, HuPrcCurrentGet());
    lbl_1_bss_C38->property = (void *)(size_t)time;
}

// NONMATCHING: register allocation / float scheduling differ (logic reconstructed from asm)
static void fn_1_247F0(void)
{
    s16 time;

    time = (s16)(u16)(size_t)HuPrcCurrentGet()->property - 10;
    if (time < 0) {
        time = 10;
    }
    if (lbl_1_bss_1958[1] != Hu3DMotionIDGet(MDL(8))) {
        Hu3DMotionShiftSet(MDL(8), lbl_1_bss_1958[1], 0.0f, 10.0f, 0x40000001);
    }
    HuPrcSleep(time - 10);
    Hu3DMotionShiftSet(MDL(8), lbl_1_bss_1958[0], 0.0f, 10.0f, 0x40000001);
    lbl_1_bss_C38 = NULL;
    HuPrcEnd();
    while (1) {
        HuPrcVSleep();
    }
}

/* keeps the shadow models under the machines / podium */
// NONMATCHING: register allocation / float scheduling differ (logic reconstructed from asm)
static void fn_1_248F4(void)
{
    HU3DMODEL *m;
    float y;

    while (1) {
        Hu3DModelPosSet(MDL(12), Hu3DData[8].pos.x, 5.0f, Hu3DData[8].pos.z);
        m = &Hu3DData[DISP(0)->headMdl];
        if ((m->attr & 1) == 0) {
            if (lbl_1_bss_18D8 != 0) {
                y = 15.0f + (m->pos.y - DISP(0)->yOfs);
            } else {
                y = 15.0f;
            }
            Hu3DModelPosSet(MDL(13), m->pos.x, y, m->pos.z);
            Hu3DModelRotSetV(MDL(13), &m->rot);
            Hu3DModelAttrReset(MDL(13), 1);
        } else {
            Hu3DModelAttrSet(MDL(13), 1);
        }
        m = &Hu3DData[DISP(1)->headMdl];
        if ((m->attr & 1) == 0) {
            if (lbl_1_bss_18D8 != 0) {
                y = 15.0f + (m->pos.y - DISP(0)->yOfs);
            } else {
                y = 15.0f;
            }
            Hu3DModelPosSet(MDL(14), m->pos.x, y, m->pos.z);
            Hu3DModelRotSetV(MDL(14), &m->rot);
            Hu3DModelAttrReset(MDL(14), 1);
        } else {
            Hu3DModelAttrSet(MDL(14), 1);
        }
        m = &Hu3DData[9];
        if ((m->attr & 1) == 0) {
            Hu3DModelPosSet(MDL(15), m->pos.x, 5.0f, m->pos.z);
        }
        HuPrcVSleep();
    }
}

/* per frame animation of the machine's body parts (wheels spin / motions advance) */
// NONMATCHING: register allocation / float scheduling differ (logic reconstructed from asm)
static void fn_1_24B24(SDMACHINE_DISP *disp, float dt)
{
    HU3DMODEL *m;
    HSFOBJECT *obj;
    s16 i;

    for (i = 0; i < 4; i++) {
        if (disp->bodyMdl[i] == -1) {
            continue;
        }
        m = &Hu3DData[disp->bodyMdl[i]];
        if (disp->partNo[1] == 2 || disp->partNo[1] == 5 || disp->partNo[1] == 6 || disp->partNo[1] == 7
            || disp->partNo[1] == 9) {
            m->motWork.time += 2.0f * dt;
            if (m->motWork.time >= Hu3DMotionMaxTimeGet(disp->bodyMdl[i])) {
                m->motWork.time = 0.0f;
            }
            /* NONMATCHING: the original passes lbl_1_bss_195E[bodyMdl] here, which indexes the scene model
               table with a model id; the model id itself is clearly what is meant */
            Hu3DMotionCalc(disp->bodyMdl[i]);
        } else {
            if (i & 1) {
                obj = Hu3DModelObjPtrGet(disp->bodyMdl[i], "chn_tr");
            } else {
                obj = Hu3DModelObjPtrGet(disp->bodyMdl[i], "chn_tl");
            }
            obj->mesh.base.rot.x += 4.0f * dt;
            if (obj->mesh.base.rot.x > 360.0f) {
                obj->mesh.base.rot.x -= 360.0f;
            }
        }
    }
}

/* message id of a character name (characters 10..31 share one message) */
s32 fn_1_24D20(s16 charNo)
{
    if (charNo < 0x20) {
        if (charNo > 9) {
            return 0x190060;
        }
        return charNo;
    }
    return lbl_1_data_1DAC[charNo - 0x20];
}

static void fn_1_24D6C(void)
{
    HuVecF pos;

    pos.x = 200.0f;
    pos.y = 500.0f;
    pos.z = 0.0f;
    while (1) {
        fn_1_25198(&pos, 1);
        fn_1_24F14();
        HuPrcVSleep();
    }
}

/* draws the confetti pieces */
// NONMATCHING: register allocation / float scheduling differ (logic reconstructed from asm)
static void fn_1_24DC0(HU3DMODEL *model, Mtx *mtx)
{
    Mtx m;
    HSFDATA *hsf;
    HSFOBJECT *obj;
    HSFMATERIAL *mat;
    ENDING_PARTICLE *p;
    s32 i, j;

    Hu3DModelObjDrawInit();
    hsf = Hu3DData[lbl_1_bss_C40.modelId].hsf;
    obj = Hu3DModelObjPtrGet(lbl_1_bss_C40.modelId, "m569_ef2");
    for (i = 0; i < 50; i++) {
        p = &lbl_1_bss_C4C[i];
        if (p->active == 0) {
            continue;
        }
        mtxRot(m, p->rot.x, p->rot.y, p->rot.z);
        mtxTransCat(m, p->pos.x, p->pos.y, p->pos.z);
        mtxScaleCat(m, 1.5f, 1.5f, 1.5f);
        PSMTXConcat(*mtx, m, m);
        Hu3DModelObjPtrDraw(lbl_1_bss_C40.modelId, obj, m);
        mat = hsf->material;
        for (j = 0; j < hsf->materialNum; j++, mat++) {
            mat->color[0] = p->color[0];
            mat->color[1] = p->color[1];
            mat->color[2] = p->color[2];
        }
    }
}

// NONMATCHING: register allocation / float scheduling differ (logic reconstructed from asm)
static void fn_1_24F14(void)
{
    ENDING_PARTICLE *p;
    s32 i;

    if (lbl_1_bss_C40.count == 0) {
        return;
    }
    for (i = 0; i < 50; i++) {
        p = &lbl_1_bss_C4C[i];
        if (p->active == 0) {
            continue;
        }
        if (p->landed != 0) {
            p->landTime++;
            if (p->landTime > 3) {
                p->active = 0;
                p->landed = 0;
                lbl_1_bss_C40.count--;
            }
        } else {
            PSVECAdd(&p->pos, &p->vel, &p->pos);
            PSVECAdd(&p->rot, &p->rotVel, &p->rot);
            if (p->pos.y <= lbl_1_bss_C40.floorY) {
                p->pos.y = lbl_1_bss_C40.floorY;
                p->rot.x = -90.0f;
                p->rot.z = 0.0f;
                p->landed = 1;
            }
        }
    }
}

static void fn_1_25040(void)
{
    s32 i;

    lbl_1_bss_C40.modelId = Hu3DModelCreate(HuDataSelHeapReadNum(0xBD001E, 0x10000000, HEAP_MODEL));
    Hu3DModelCameraSet(lbl_1_bss_C40.modelId, 1);
    Hu3DModelAttrSet(lbl_1_bss_C40.modelId, 1);
    lbl_1_bss_C40.hookId = Hu3DHookFuncCreate(fn_1_24DC0);
    Hu3DModelCameraSet(lbl_1_bss_C40.hookId, 5);
    Hu3DModelLayerSet(lbl_1_bss_C40.hookId, 1);
    lbl_1_bss_C40.floorY = 0.1f;
    lbl_1_bss_C40.count = 0;
    for (i = 0; i < 50; i++) {
        lbl_1_bss_C4C[i].active = 0;
        lbl_1_bss_C4C[i].landed = 0;
    }
}

void fn_1_2515C(void)
{
    Hu3DModelKill(lbl_1_bss_C40.modelId);
    Hu3DModelKill(lbl_1_bss_C40.hookId);
}

/* spawns up to num confetti pieces around pos */
// NONMATCHING: register allocation / float scheduling differ (logic reconstructed from asm)
static void fn_1_25198(HuVecF *pos, s32 num)
{
    ENDING_PARTICLE *p;
    s32 spawned = 0;
    s32 i;
    float r, azim, elev;

    for (i = 0; i < 50 && spawned != num; i++) {
        p = &lbl_1_bss_C4C[i];
        if (p->active != 0) {
            continue;
        }
        p->active = 1;
        spawned++;
        lbl_1_bss_C40.count++;
        r = (float)frandmod(15000) / 100.0f;
        elev = (float)frandmod(360);
        azim = (float)frandmod(360);
        p->pos.x = pos->x + r * (sin(PI_D * azim / 180.0) * cos(PI_D * elev / 180.0));
        p->pos.y = pos->y + r * -sin(PI_D * elev / 180.0);
        p->pos.z = pos->z + r * (cos(PI_D * azim / 180.0) * cos(PI_D * elev / 180.0));
        p->vel.x = 0.8 * (((float)frandmod(201) - 100.0) / 100.0);
        p->vel.y = -6.5f * ((float)frandmod(100) / 100.0f);
        if (p->vel.y > -2.6000001f) {
            p->vel.y = -2.6000001f;
        }
        p->vel.z = 0.5 * (((float)frandmod(201) - 100.0) / 100.0);
        p->rot.x = 0.0f;
        p->rot.y = 0.0f;
        p->rot.z = 0.0f;
        p->rotVel.x = 14.0 * (((float)frandmod(201) - 100.0) / 100.0);
        p->rotVel.y = 14.0 * (((float)frandmod(201) - 100.0) / 100.0);
        p->rotVel.z = 14.0 * (((float)frandmod(201) - 100.0) / 100.0);
        {
            u32 c = frandmod(4);
            p->color[0] = lbl_1_data_216E[c][0];
            p->color[1] = lbl_1_data_216E[c][1];
            p->color[2] = lbl_1_data_216E[c][2];
            p->color[3] = lbl_1_data_216E[c][3];
        }
        p->landTime = 0;
    }
}
