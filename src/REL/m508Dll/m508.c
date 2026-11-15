#include "REL/m508Dll.h"

typedef void (*M508VoidFunc)(void);
extern const M508VoidFunc _ctors[];
extern const M508VoidFunc _dtors[];

/* ---- prototypes ---- */
void ObjectSetup(void);
static void fn_1_538(void);
M508_OBJ *fn_1_5B0(void);
void fn_1_6FC(M508_OBJ *self);
void fn_1_A3C(M508_OBJ *self);
void fn_1_10D4(M508_OBJ *self);
void fn_1_15B4(M508_OBJ *self);
void fn_1_1BB4(void);
M508_OBJ *fn_1_1D88(void);
void fn_1_1E58(M508_OBJ *obj);
M508_OBJ *fn_1_1E64(void);
M508_OBJ *fn_1_1F34(void);
void fn_1_1FD0(M508_OBJ *obj);
void fn_1_1FDC(M508_OBJ *self);
void fn_1_2084(M508_OBJ *obj, s32 seq);
void fn_1_20A8(M508_OBJ *self);
void fn_1_21C8(M508_OBJ *obj, s32 id);
M508_OBJ *fn_1_2260(void);
void fn_1_22B4(M508_OBJ *obj);
void fn_1_22C0(M508_OBJ *self);
void fn_1_2388(M508_OBJ *obj);
void fn_1_2394(M508_OBJ *self);
M508_OBJ *fn_1_24DC(void);
void fn_1_25A8(M508_OBJ *obj);
void fn_1_25B4(M508_OBJ *self);
void fn_1_265C(M508_OBJ *obj, s32 kind, f32 rotSpeed, f32 shiftTime, f32 motSpeed);
void fn_1_268C(M508_OBJ *self);
void fn_1_2838(M508_OBJ *obj);
void fn_1_2844(M508_OBJ *self);
void fn_1_2C7C(M508_OBJ *obj);
void fn_1_2C88(M508_OBJ *self);
void fn_1_2F80(M508_OBJ *obj);
f32 fn_1_2F8C(f32 a, f32 b, f32 t);
void fn_1_3090(M508_OBJ *self);
void fn_1_33D8(M508_OBJ *obj);
void fn_1_33E4(M508_OBJ *self);
M508_OBJ *fn_1_37E0(s32 index);
void fn_1_38CC(M508_OBJ *obj, f32 x, f32 y, f32 z);
M508_OBJ *fn_1_3958(s16 playerNo);
void fn_1_3A38(M508_OBJ *obj);
void fn_1_3A44(M508_OBJ *self);
void fn_1_3B8C(M508_OBJ *obj);
void fn_1_3B98(M508_OBJ *self);
void fn_1_4150(M508_OBJ *obj);
void fn_1_415C(M508_OBJ *self);
void fn_1_4374(M508_OBJ *obj);
void fn_1_4380(M508_OBJ *self);
s32 fn_1_4654(M508_OBJ *obj);
void fn_1_469C(M508_OBJ *obj);
void fn_1_46A8(M508_OBJ *self);
void fn_1_4790(M508_OBJ *obj);
void fn_1_479C(M508_OBJ *self);
void fn_1_4884(M508_OBJ *obj);
void fn_1_4890(M508_OBJ *self);
void fn_1_49EC(M508_OBJ *obj);
void fn_1_49F8(M508_OBJ *self);
M508_OBJ *fn_1_4B54(s32 playerNo);
void fn_1_4F48(M508_OBJ *obj, f32 x, f32 y);
void fn_1_4FA8(M508_OBJ *obj);
void fn_1_5010(M508_OBJ *obj);
void fn_1_5078(M508_OBJ *obj);
void fn_1_5084(M508_OBJ *self);
void fn_1_512C(M508_OBJ *obj);
void fn_1_5164(M508_OBJ *self);
M508_OBJ *fn_1_5394(s32 kind);
M508_OBJ *fn_1_5414(void);
void fn_1_5464(M508_OBJ *obj);
void fn_1_5470(M508_OBJ *self);
void fn_1_5518(M508_OBJ *obj);
void fn_1_5524(M508_OBJ *self);
M508_OBJ *fn_1_560C(void);
M508_OBJ *fn_1_5758(s32 kind);
void fn_1_57D4(M508_OBJ *obj);
void fn_1_57E0(M508_OBJ *self);
void fn_1_58B8(M508_OBJ *obj);
void fn_1_58C4(M508_OBJ *self);
void fn_1_5C7C(s16 playerNo, s32 kind, s16 time);
M508_OBJ *fn_1_7848(s32 prio, u32 stackSize, s32 funcNum, M508_OBJFUNC *funcTbl);
M508_OBJ *fn_1_7A3C(s32 prio, u32 stackSize, s32 dataNum, s32 motNum, u32 *motTbl, s32 funcNum, M508_OBJFUNC *funcTbl);
M508_OBJ *fn_1_8308(s32 prio, u32 stackSize, s32 playerNo, s32 motNum, u32 *motTbl, s32 funcNum, M508_OBJFUNC *funcTbl, s16 modelNo);
void fn_1_7D60(M508_OBJ *obj, f32 x, f32 y, f32 z);
void fn_1_7DB4(M508_OBJ *obj, f32 *x, f32 *y, f32 *z);
M508_CAMERA *fn_1_86F0(s16 cameraBit);
void fn_1_8918(void);
void fn_1_8BF4(M508_CAMERA *camera);
void fn_1_8C70(M508_CAMERA *camera);
void fn_1_8D30(M508_CAMERA *camera);
void fn_1_926C(M508_CAMERA *camera);
void fn_1_8E28(M508_CAMERA *camera, HuVecF *center, HuVecF *rot, float *zoom, int time);
void fn_1_95D8(M508_CAMERA *camera, float fov, float near, float far, float aspect);
void fn_1_9EF8(void);

/* ---- .data ---- */
static M508_CAMERAFUNC lbl_1_data_0[3] = { fn_1_8D30, fn_1_926C, fn_1_8C70 };
static s32 lbl_1_data_C = -1;
static M508_OBJFUNC lbl_1_data_10[5] = { NULL, fn_1_6FC, fn_1_A3C, fn_1_10D4, fn_1_15B4 };
static M508_OBJFUNC lbl_1_data_2C[2] = { fn_1_1FDC, fn_1_20A8 };
static M508_OBJFUNC lbl_1_data_34[2] = { fn_1_22C0, fn_1_2394 };
static M508_OBJFUNC lbl_1_data_3C[6] = { fn_1_25B4, fn_1_268C, fn_1_2844, fn_1_2C88, fn_1_3090, fn_1_33E4 };
static M508_OBJFUNC lbl_1_data_54[8] = { fn_1_3A44, fn_1_3B98, fn_1_415C, fn_1_4380, fn_1_46A8, fn_1_479C, fn_1_4890, fn_1_49F8 };
static char lbl_1_data_74[] = "test11_tex_we-itemhook-r";
static M508_OBJFUNC lbl_1_data_90[2] = { fn_1_5084, fn_1_5164 };
static M508_OBJFUNC lbl_1_data_98[2] = { fn_1_5470, fn_1_5524 };
static M508_OBJFUNC lbl_1_data_A0[1] = { NULL };
static M508_OBJFUNC lbl_1_data_A4[2] = { fn_1_57E0, fn_1_58C4 };

/* ---- .rodata ---- */
static s16 lbl_1_rodata_C4[4] = { 0x28, 0x38, 0x48, 0x58 };
static s32 lbl_1_rodata_CC[3] = { 0x43, 0x44, 0x42 };
static f32 lbl_1_rodata_DC[4][2] = { { 70.0f, 58.0f }, { 190.0f, 58.0f }, { 388.0f, 58.0f }, { 508.0f, 58.0f } };
static HuVecF lbl_1_rodata_110 = { 0.0f, 500.0f, 1000.0f };
static HuVecF lbl_1_rodata_11C = { 0.0f, 0.0f, -300.0f };
static HuVecF lbl_1_rodata_12C = { 0.0f, 579.0f, -1070.79f };
static HuVecF lbl_1_rodata_138 = { 11.6f, -0.1f, 7.0f };
static f32 lbl_1_rodata_144 = 2202.3f;
static s32 lbl_1_rodata_150[3][4] = { { 4, 6, 3, 6 }, { 1, 2, 3, 4 }, { 1, 5, 0, 4 } };
static s32 lbl_1_rodata_180[3][2] = { { 0, 2 }, { 3, 4 }, { 1, 5 } };
static f32 lbl_1_rodata_198[3][2] = { { 2.0f, -2.0f }, { 1.0f, -1.0f }, { 3.0f, -3.0f } };
static f32 lbl_1_rodata_1B0[3] = { 1.0f, 0.4f, 2.0f };
static HuVecF lbl_1_rodata_1BC = { 0.0f, 1000.0f, 800.0f };
static HuVecF lbl_1_rodata_1C8 = { 0.0f, 0.0f, -400.0f };
static GXColor lbl_1_rodata_1D4 = { 0xFF, 0xFF, 0xFF, 0xFF };
static HuVecF lbl_1_rodata_1D8 = { 0.0f, 1000.0f, 800.0f };
static HuVecF lbl_1_rodata_1E4 = { 0.0f, 0.0f, -600.0f };
static GXColor lbl_1_rodata_1F0 = { 0xFF, 0xFF, 0xFF, 0xFF };
static s32 lbl_1_rodata_210[6][10][2] = {
    { { 1, 10 }, { 2, 10 }, { 3, 10 }, { 4, 10 }, { 255, 10 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
    { { 1, 30 }, { 0, 4 }, { 2, 30 }, { 0, 4 }, { 255, 10 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
    { { 2, 60 }, { 0, 30 }, { 3, 60 }, { 0, 30 }, { 255, 10 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
    { { 1, 10 }, { 3, 10 }, { 1, 10 }, { 2, 10 }, { 255, 10 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
    { { 0, 15 }, { 1, 15 }, { 2, 15 }, { 3, 15 }, { 4, 15 }, { 255, 10 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
    { { 0, 90 }, { 255, 60 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
};
static f32 lbl_1_rodata_3F0[5][2] = { { 0.0f, 1.0f }, { 10.0f, 19.0f }, { 20.0f, 29.0f }, { 30.0f, 39.0f }, { 40.0f, 49.0f } };
static HuVecF lbl_1_rodata_418 = { 0.0f, 1000.0f, 800.0f };
static HuVecF lbl_1_rodata_424 = { 0.0f, 0.0f, -400.0f };
static GXColor lbl_1_rodata_430 = { 0xFF, 0xFF, 0xFF, 0xFF };
static u32 lbl_1_rodata_434[9] = { 0x420001, 0x420002, 0x420003, 0x420004, 0x420005, 0x420006, 0x420007, 0x420008, 0x420009 };
static s32 lbl_1_rodata_45C[7] = { 1, 2, 3, 4, 5, 6, 7 };
static HuVecF lbl_1_rodata_4CC = { 0.0f, 1000.0f, 800.0f };
static HuVecF lbl_1_rodata_4D8 = { 0.0f, 0.0f, -600.0f };
static GXColor lbl_1_rodata_4E4 = { 0xFF, 0xFF, 0xFF, 0xFF };
static s32 lbl_1_rodata_4E8[5] = { 0x420015, 0x420014, 0x420017, 0x420016, 0x420018 };
/* 13 characters x 9 motion data numbers */
static u32 lbl_1_rodata_4FC[13][9] = {
    { 0x8C0006, 0x8C0031, 0x8C003B, 0x8C003C, 0x8C003D, 0x8C0046, 0x8C0047, 0x8C0048, 0x8C0049 },
    { 0x330006, 0x330031, 0x33003B, 0x33003C, 0x33003D, 0x330046, 0x330047, 0x330048, 0x330049 },
    { 0xB70006, 0xB70031, 0xB7003B, 0xB7003C, 0xB7003D, 0xB70046, 0xB70047, 0xB70048, 0xB70049 },
    { 0xED0006, 0xED0031, 0xED003B, 0xED003C, 0xED003D, 0xED0046, 0xED0047, 0xED0048, 0xED0049 },
    { 0xE60006, 0xE60031, 0xE6003B, 0xE6003C, 0xE6003D, 0xE60046, 0xE60047, 0xE60048, 0xE60049 },
    { 0x170006, 0x170031, 0x17003B, 0x17003C, 0x17003D, 0x170046, 0x170047, 0x170048, 0x170049 },
    { 0xE00006, 0xE00031, 0xE0003B, 0xE0003C, 0xE0003D, 0xE00046, 0xE00047, 0xE00048, 0xE00049 },
    { 0x2D0006, 0x2D0031, 0x2D003B, 0x2D003C, 0x2D003D, 0x2D0046, 0x2D0047, 0x2D0048, 0x2D0049 },
    { 0xC90006, 0xC90031, 0xC9003B, 0xC9003C, 0xC9003D, 0xC90046, 0xC90047, 0xC90048, 0xC90049 },
    { 0xB00006, 0xB00031, 0xB0003B, 0xB0003C, 0xB0003D, 0xB00046, 0xB00047, 0xB00048, 0xB00049 },
    { 0xB00006, 0xB00031, 0xB0003B, 0xB0003C, 0xB0003D, 0xB00046, 0xB00047, 0xB00048, 0xB00049 },
    { 0xB00006, 0xB00031, 0xB0003B, 0xB0003C, 0xB0003D, 0xB00046, 0xB00047, 0xB00048, 0xB00049 },
    { 0xB00006, 0xB00031, 0xB0003B, 0xB0003C, 0xB0003D, 0xB00046, 0xB00047, 0xB00048, 0xB00049 },
};
static s32 lbl_1_rodata_6D0[4] = { 0xA, 0x28, 0x3C, 0x55 };
static s32 lbl_1_rodata_6F0[13] = { 0x960000, 0x960001, 0x960002, 0x960003, 0x960004, 0x960006, 0x960007, 0x960008, 0x960009, 0x96000A, 0x96000B, 0x96000C, 0x96000D };
static s32 lbl_1_rodata_738[7] = { 0x42001A, 0x420019, 0x42001C, 0x42001B, 0x42001D, 0x42001E, 0x42000A };
static s32 lbl_1_rodata_754[6] = { 0x42000D, 0x420011, 0x42000E, 0x42000F, 0x420010, 0x420012 };

/* ---- .bss ---- */
static s32 lbl_1_bss_0;
static s32 lbl_1_bss_4;
static M508_OBJ *lbl_1_bss_8[5];
static M508_OBJ *lbl_1_bss_1C[6];
static M508_OBJ *lbl_1_bss_34[4];
static M508_OBJ *lbl_1_bss_44[6][4];
static M508_OBJ *lbl_1_bss_A4[4];
static M508_OBJ *lbl_1_bss_B4[5];
static M508_OBJ *lbl_1_bss_C8[5];
static M508_OBJ *lbl_1_bss_DC;
static M508_OBJ *lbl_1_bss_E0;
static M508_OBJ *lbl_1_bss_E4;
static M508_OBJ *lbl_1_bss_E8;
static M508_OBJ *lbl_1_bss_EC;
static M508_CAMERA *lbl_1_bss_F0;
static s32 lbl_1_bss_F4[4];
static s32 lbl_1_bss_104;
static s32 lbl_1_bss_108;
static s32 lbl_1_bss_10C;
static s32 lbl_1_bss_110;
static M508_BOXSET lbl_1_bss_114[5];

/* One frame of a mode function: sleep, then report whether the mode changed meanwhile. */
static s32 M508Yield(void)
{
    M508_OBJ *self;
    s32 mode;

    HuPrcVSleep();
    self = HuPrcCurrentGet()->property;
    mode = self->mode;
    if (self->prevMode == mode) {
        return 0;
    }
    self->prevMode = mode;
    return 1;
}

int _prolog(void)
{
    const M508VoidFunc *ctors = _ctors;

    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const M508VoidFunc *dtors = _dtors;

    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

void ObjectSetup(void)
{
    s32 i;
    s32 j;

    omGameSysInit(omInitObjMan(1, 0x2000));
    lbl_1_bss_F0 = fn_1_86F0(1);
    lbl_1_bss_F0->mode = 2;
    fn_1_95D8(lbl_1_bss_F0, 40.0f, 10.0f, 8000.0f, 1.2f);
    lbl_1_bss_E8 = fn_1_5414();
    lbl_1_bss_EC = fn_1_5B0();
    lbl_1_bss_EC->mode = 1;
    for (i = 0; i < 4; i++) {
        lbl_1_bss_F4[i] = 0;
    }
    lbl_1_bss_E4 = fn_1_1D88();
    lbl_1_bss_E0 = fn_1_1F34();
    lbl_1_bss_DC = fn_1_1E64();
    lbl_1_bss_DC->dispF = 1;
    Hu3DModelAttrReset(lbl_1_bss_DC->modelId, 1);
    for (i = 0; i < 5; i++) {
        lbl_1_bss_C8[i] = fn_1_2260();
    }
    for (i = 0; i < 5; i++) {
        lbl_1_bss_B4[i] = fn_1_37E0(i);
    }
    for (i = 0; i < 4; i++) {
        lbl_1_bss_A4[i] = fn_1_3958(i);
    }
    for (i = 0; i < 5; i++) {
        lbl_1_bss_8[i] = fn_1_24DC();
    }
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 6; j++) {
            lbl_1_bss_44[j][i] = fn_1_5394(j);
        }
    }
    for (i = 0; i < 4; i++) {
        lbl_1_bss_34[i] = fn_1_4B54(i);
        fn_1_5010(lbl_1_bss_34[i]);
    }
    for (i = 0; i < 6; i++) {
        lbl_1_bss_1C[i] = fn_1_5758(i);
    }
    HuPrcChildCreate(fn_1_538, 0xFFF, 0x1000, 0, HuPrcCurrentGet());
}

static void fn_1_538(void)
{
    while (omSysExitReq == 0) {
        HuPrcVSleep();
    }
    HuAudFadeOut(1000);
    WipeCreate(2, 6, 60);
    while (WipeCheckEnd() != 0) {
        HuPrcVSleep();
    }
    omOvlReturnEx(1, 1);
    HuPrcEnd();
}

M508_OBJ *fn_1_5B0(void)
{
    return fn_1_7848(0x1000, 0x2000, 5, lbl_1_data_10);
}

/* controller mode 1: place everything for the intro */
void fn_1_6FC(M508_OBJ *self)
{
    s32 i;
    f32 x;

    fn_1_7D60(lbl_1_bss_E4, 0.0f, 0.0f, 0.0f);
    for (i = 0; i < 5; i++) {
        x = -500.0f + (250.0f * (f32)i);
        fn_1_7D60(lbl_1_bss_C8[i], x, 250.0f, -450.0f);
    }
    for (i = 0; i < 5; i++) {
        fn_1_38CC(lbl_1_bss_B4[i], -500.0f + (250.0f * (f32)i), 250.0f, -450.0f);
    }
    for (i = 0; i < 4; i++) {
        x = -300.0f + (200.0f * (f32)i);
        fn_1_7D60(lbl_1_bss_A4[i], x, 0.0f, 0.0f);
        fn_1_4150(lbl_1_bss_A4[i]);
    }
    for (i = 0; i < 5; i++) {
        x = -500.0f + (250.0f * (f32)i);
        fn_1_7D60(lbl_1_bss_8[i], x, 0.0f, -450.0f);
    }
    for (i = 0; i < 4; i++) {
        fn_1_4F48(lbl_1_bss_34[i], lbl_1_rodata_DC[i][0], lbl_1_rodata_DC[i][1]);
    }
    lbl_1_bss_EC->mode = 2;
}

/* controller mode 2: opening sequence */
void fn_1_A3C(M508_OBJ *self)
{
    s32 light;
    s32 i;

    light = Hu3DGLightCreate(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    Hu3DGLightPosAimSetV(light, &lbl_1_rodata_110, &lbl_1_rodata_11C);
    Hu3DGLightColorSet(light, 0xFF, 0xFF, 0xFF, 0xFF);
    Hu3DGLightStaticSet(light, 1);
    lbl_1_bss_F0->mode = 2;
    fn_1_5518(lbl_1_bss_E8);
    fn_1_58B8(lbl_1_bss_1C[0]);
    fn_1_2084(lbl_1_bss_E0, 0);
    lbl_1_data_C = HuAudFXPlay(0x39A);
    WipeCreate(1, 5, 60);
    while (WipeCheckEnd() != 0) {
        HuPrcVSleep();
    }
    HuPrcSleep(0x81);
    WipeCreate(2, 0x81, 60);
    fn_1_8E28(lbl_1_bss_F0, &lbl_1_rodata_12C, &lbl_1_rodata_138, &lbl_1_rodata_144, 0);
    HuPrcSleep(0);
    WipeCreate(1, 0x81, 0x1E);
    fn_1_2084(lbl_1_bss_E0, 5);
    lbl_1_bss_4 = HuAudSStreamPlay(lbl_1_rodata_CC[0]);
    GameMesCreate(2, 0);
    for (i = 0; i < 0x78; i++) {
        HuPrcVSleep();
    }
    fn_1_1FD0(lbl_1_bss_E0);
    for (i = 0; i < 5; i++) {
        fn_1_2388(lbl_1_bss_C8[i]);
    }
    HuPrcSleep(0x13);
    HuAudFXStop(lbl_1_data_C);
    self->mode = 4;
}

/* controller mode 3: results */
void fn_1_10D4(M508_OBJ *self)
{
    s32 winners[4];
    s32 id;
    s32 max;
    s32 cnt;
    s32 n;
    s32 flag;
    s32 i;
    s32 j;
    s32 k;

    fn_1_58B8(lbl_1_bss_1C[1]);
    fn_1_2084(lbl_1_bss_E0, 4);
    HuAudFXStop(lbl_1_data_C);
    id = GameMesCreate(2, 1);
    while (GameMesStatGet(id) != 0) {
        HuPrcVSleep();
    }
    max = 0;
    for (i = 0; i < 4; i++) {
        if (max < lbl_1_bss_F4[i]) {
            max = lbl_1_bss_F4[i];
        }
    }
    cnt = 0;
    for (i = 0; i < 4; i++) {
        if (max == lbl_1_bss_F4[i]) {
            cnt++;
        }
    }
    if (i == cnt && max == 0) {
        HuAudJinglePlay(0x7B);
        HuPrcSleep(0);
        GameMesCreate(2, 2);
        CharLoseVoicePlay(lbl_1_bss_A4[0]->charNo, lbl_1_bss_A4[1]->charNo, lbl_1_bss_A4[2]->charNo, lbl_1_bss_A4[3]->charNo);
        for (i = 0; i < 4; i++) {
            CharModelVoicePanSet(lbl_1_bss_A4[i]->charNo, lbl_1_rodata_C4[i]);
            fn_1_49EC(lbl_1_bss_A4[i]);
        }
        for (i = 0; i < 5; i++) {
            HuAudFXPlay(0x68E);
            fn_1_2C7C(lbl_1_bss_8[i]);
            HuPrcSleep(9);
        }
        HuPrcSleep(0xA9);
    } else {
        n = 0;
        for (i = 0; i < 4; i++) {
            if (max == lbl_1_bss_F4[i]) {
                winners[n++] = lbl_1_bss_A4[i]->charNo;
                GwPlayer[i].coinBattle = 10;
                fn_1_4884(lbl_1_bss_A4[i]);
            } else {
                fn_1_49EC(lbl_1_bss_A4[i]);
            }
        }
        while (n < i) {
            winners[n++] = -1;
        }
        HuAudJinglePlay(0x79);
        HuPrcSleep(0);
        GameMesCreate(4, 3, winners[0], winners[1], winners[2], winners[3]);
        lbl_1_data_C = HuAudFXPlay(0x39A);
        flag = 0;
        for (j = 0; j < 4; j++) {
            if (lbl_1_bss_F4[j] == 3) {
                cnt = 0;
                for (k = 0; k < 4; k++) {
                    if (k != j && lbl_1_bss_F4[k] == 0) {
                        cnt++;
                    }
                }
                if (cnt == 3) {
                    flag = 1;
                    break;
                }
            }
        }
        if (flag != 0) {
            for (i = 0; i < 5; i++) {
                fn_1_33D8(lbl_1_bss_8[i]);
                HuPrcSleep(9);
            }
            HuPrcSleep(0xA9);
        } else {
            for (i = 0; i < 5; i++) {
                fn_1_265C(lbl_1_bss_8[i], lbl_1_bss_114[lbl_1_bss_110].kind, 0.0f, 0.0f, 1.0f);
            }
            HuPrcSleep(0xD1);
        }
    }
    HuAudFadeOut(1000);
    WipeCreate(2, 6, 60);
    while (WipeCheckEnd() != 0) {
        HuPrcVSleep();
    }
    omOvlReturnEx(1, 1);
    HuPrcEnd();
}

/* controller mode 4: the three rounds */
void fn_1_15B4(M508_OBJ *self)
{
    MGTIMER *timer;
    M508_OBJ *box;
    HuVecF pos;
    s32 round;
    s32 i;

    timer = MgTimerCreate(0);
    MgTimerPosSet(timer, 288.0f, 64.0f);
    for (round = 0; round < 3;) {
        lbl_1_bss_104 = round;
        if (round > 0) {
            for (i = 0; i < 4; i++) {
                lbl_1_bss_A4[i]->mode = 2;
            }
        }
        fn_1_1BB4();
        HuAudFXPlay(0x688);
        for (i = 0; i < 5; i++) {
            fn_1_2838(lbl_1_bss_8[i]);
        }
        HuPrcSleep(0x2D);
        for (i = 0; i < 5; i++) {
            fn_1_265C(lbl_1_bss_8[i], lbl_1_bss_114[i].kind, lbl_1_bss_114[i].rotSpeed, lbl_1_bss_114[i].shiftTime, lbl_1_bss_114[i].motSpeed);
        }
        fn_1_2084(lbl_1_bss_E0, lbl_1_bss_104 + 1);
        fn_1_58B8(lbl_1_bss_1C[round + 2]);
        MgTimerParamSet(timer, 300, 0, 0);
        MgTimerModeOnSet(timer, 1);
        for (i = 0; i < 4; i++) {
            fn_1_4FA8(lbl_1_bss_34[i]);
        }
        for (i = 0; i < 4; i++) {
            lbl_1_bss_A4[i]->mode = 1;
        }
        lbl_1_bss_0 = 0;
        while (MgTimerDoneCheck(timer) == 0) {
            if (lbl_1_bss_0 >= 4) {
                if (timer->stopF == 0) {
                    timer->stopF = 1;
                    timer->mode = 1;
                }
                break;
            }
            HuPrcSleep(0);
        }
        for (i = 0; i < 4; i++) {
            lbl_1_bss_A4[i]->mode = 3;
        }
        HuPrcSleep(0x3B);
        for (i = 0; i < 5; i++) {
            fn_1_2F80(lbl_1_bss_8[i]);
        }
        HuAudSStreamFadeOut(lbl_1_bss_4, 2000);
        fn_1_2084(lbl_1_bss_E0, 5);
        HuPrcSleep(0x77);
        lbl_1_bss_1C[round + 2]->mode = 0;
        fn_1_1FD0(lbl_1_bss_E0);
        for (i = 0; i < 5; i++) {
            fn_1_265C(lbl_1_bss_8[i], lbl_1_bss_114[i].kind, 0.0f, 0.0f, 1.0f);
        }
        HuAudFXPlay(0x68D);
        box = lbl_1_bss_8[lbl_1_bss_110];
        fn_1_7DB4(box, &pos.x, &pos.y, &pos.z);
        fn_1_7D60(lbl_1_bss_1C[5], pos.x, pos.y, pos.z);
        fn_1_58B8(lbl_1_bss_1C[5]);
        lbl_1_data_C = HuAudFXPlay(0x39A);
        for (i = 0; i < 4; i++) {
            if (fn_1_4654(lbl_1_bss_A4[i]) != 0) {
                fn_1_469C(lbl_1_bss_A4[i]);
                fn_1_512C(lbl_1_bss_34[i]);
                lbl_1_bss_F4[i]++;
            } else {
                fn_1_4790(lbl_1_bss_A4[i]);
            }
        }
        HuPrcSleep(0x77);
        if (round == 2) {
            break;
        }
        HuAudFXStop(lbl_1_data_C);
        fn_1_2084(lbl_1_bss_E0, 5);
        fn_1_57D4(lbl_1_bss_1C[5]);
        HuAudFXPlay(0x68E);
        for (i = 0; i < 5; i++) {
            fn_1_2C7C(lbl_1_bss_8[i]);
        }
        round++;
        lbl_1_bss_4 = HuAudSStreamPlay(lbl_1_rodata_CC[round]);
        HuPrcSleep(0x77);
    }
    MgTimerKill(timer);
    self->mode = 3;
}

/* pick the box layout of the current round */
void fn_1_1BB4(void)
{
    s32 a;
    s32 b;
    s32 i;

    a = lbl_1_rodata_180[lbl_1_bss_104][frandmod(2)];
    do {
        b = lbl_1_rodata_150[lbl_1_bss_104][frandmod(4)];
    } while (a == b);
    lbl_1_bss_110 = frandmod(5);
    for (i = 0; i < 5; i++) {
        lbl_1_bss_114[i].kind = b;
        lbl_1_bss_114[i].rotSpeed = lbl_1_rodata_198[lbl_1_bss_104][frandmod(2)];
        lbl_1_bss_114[i].shiftTime = (f32)frandmod(30);
        lbl_1_bss_114[i].motSpeed = lbl_1_rodata_1B0[lbl_1_bss_104];
    }
    lbl_1_bss_114[lbl_1_bss_110].kind = a;
    lbl_1_bss_10C = a;
    lbl_1_bss_108 = b;
}

M508_OBJ *fn_1_1D88(void)
{
    M508_OBJ *obj;
    M508_LIGHTWORK *work;
    s16 model;

    obj = fn_1_7A3C(0x1000, 0x2000, 0x42000B, 0, NULL, 1, NULL);
    work = M508_OBJWORK(M508_LIGHTWORK, obj);
    work->light = Hu3DLLightCreateV(obj->modelId, &lbl_1_rodata_1BC, &lbl_1_rodata_1C8, &lbl_1_rodata_1D4);
    Hu3DLLightInfinitytSet(obj->modelId, work->light);
    obj->dispF = 1;
    Hu3DModelAttrReset(obj->modelId, 1);
    model = obj->modelId;
    Hu3DModelAttrReset(model, 0x40000002);
    Hu3DModelAttrSet(model, 0x40000001);
    return obj;
}

void fn_1_1E58(M508_OBJ *obj)
{
    obj->mode = 0;
}

M508_OBJ *fn_1_1E64(void)
{
    M508_OBJ *obj;
    M508_LIGHTWORK *work;
    s16 model;

    obj = fn_1_7A3C(0x1000, 0x2000, 0x42001F, 0, NULL, 1, NULL);
    work = M508_OBJWORK(M508_LIGHTWORK, obj);
    work->light = Hu3DLLightCreateV(obj->modelId, &lbl_1_rodata_1D8, &lbl_1_rodata_1E4, &lbl_1_rodata_1F0);
    Hu3DLLightInfinitytSet(obj->modelId, work->light);
    obj->dispF = 1;
    Hu3DModelAttrReset(obj->modelId, 1);
    model = obj->modelId;
    Hu3DModelAttrReset(model, 0x40000002);
    Hu3DModelAttrSet(model, 0x40000001);
    return obj;
}

M508_OBJ *fn_1_1F34(void)
{
    M508_OBJ *obj;
    s16 model;

    obj = fn_1_7A3C(0x1000, 0x2000, 0x42000C, 0, NULL, 2, lbl_1_data_2C);
    obj->dispF = 1;
    Hu3DModelAttrReset(obj->modelId, 1);
    model = obj->modelId;
    Hu3DModelAttrReset(model, 0x40000002);
    Hu3DModelAttrSet(model, 0x40000001);
    return obj;
}

void fn_1_1FD0(M508_OBJ *obj)
{
    obj->mode = 0;
}

void fn_1_1FDC(M508_OBJ *self)
{
    while (1) {
        if (M508Yield() != 0) {
            return;
        }
    }
}

void fn_1_2084(M508_OBJ *obj, s32 seq)
{
    M508_OBJWORK(M508_SEQWORK, obj)->seq = seq;
    obj->mode = 1;
}

void fn_1_20A8(M508_OBJ *self)
{
    M508_SEQWORK *work = M508_OBJWORK(M508_SEQWORK, self);
    s32 step = 0;
    s32 wait = 0;
    s32 id;

    while (1) {
        if (wait > 0) {
            wait--;
        } else {
            while (1) {
                id = lbl_1_rodata_210[work->seq][step][0];
                wait = lbl_1_rodata_210[work->seq][step][1];
                if (id == 0xFF) {
                    step = 0;
                    continue;
                }
                step++;
                fn_1_21C8(self, id);
                break;
            }
        }
        if (M508Yield() != 0) {
            return;
        }
    }
}

void fn_1_21C8(M508_OBJ *obj, s32 id)
{
    Hu3DModelAttrReset(obj->modelId, 0x40000002);
    Hu3DModelAttrReset(obj->modelId, 0x40000001);
    Hu3DMotionStartEndSet(obj->modelId, lbl_1_rodata_3F0[id][0], lbl_1_rodata_3F0[id][1]);
    Hu3DMotionTimeSet(obj->modelId, 0.0f);
}

M508_OBJ *fn_1_2260(void)
{
    return fn_1_7A3C(0x1000, 0x2000, 0x420020, 0, NULL, 2, lbl_1_data_34);
}

void fn_1_22B4(M508_OBJ *obj)
{
    obj->mode = 0;
}

void fn_1_22C0(M508_OBJ *self)
{
    self->dispF = 0;
    Hu3DModelAttrSet(self->modelId, 1);
    while (1) {
        if (M508Yield() != 0) {
            return;
        }
    }
}

void fn_1_2388(M508_OBJ *obj)
{
    obj->mode = 1;
}

void fn_1_2394(M508_OBJ *self)
{
    s16 model;
    s16 i;

    for (i = 0; i < 4; i++) {
        fn_1_5C7C(i, 2, 0x28);
    }
    HuAudFXPlay(0x687);
    self->dispF = 1;
    Hu3DModelAttrReset(self->modelId, 1);
    model = self->modelId;
    Hu3DModelAttrReset(model, 0x40000002);
    Hu3DModelAttrReset(model, 0x40000001);
    Hu3DMotionTimeSet(self->modelId, 0.0f);
    while (Hu3DMotionEndCheck(self->modelId) == 0) {
        if (M508Yield() != 0) {
            return;
        }
    }
    self->mode = 0;
}

M508_OBJ *fn_1_24DC(void)
{
    M508_OBJ *obj;
    M508_BOXWORK *work;

    obj = fn_1_7A3C(0x1000, 0x2000, 0x420000, 9, lbl_1_rodata_434, 6, lbl_1_data_3C);
    work = M508_OBJWORK(M508_BOXWORK, obj);
    work->kind = 0;
    work->light = Hu3DLLightCreateV(obj->modelId, &lbl_1_rodata_418, &lbl_1_rodata_424, &lbl_1_rodata_430);
    Hu3DLLightInfinitytSet(obj->modelId, work->light);
    obj->motNo = 0;
    Hu3DMotionShiftSet(obj->modelId, obj->motId[0], 0.0f, 8.0f, 0);
    return obj;
}

void fn_1_25A8(M508_OBJ *obj)
{
    obj->mode = 0;
}

void fn_1_25B4(M508_OBJ *self)
{
    while (1) {
        if (M508Yield() != 0) {
            return;
        }
    }
}

void fn_1_265C(M508_OBJ *obj, s32 kind, f32 rotSpeed, f32 shiftTime, f32 motSpeed)
{
    M508_BOXWORK *work = M508_OBJWORK(M508_BOXWORK, obj);

    work->kind = kind;
    work->rotSpeed = rotSpeed;
    work->shiftTime = shiftTime;
    work->motSpeed = motSpeed;
    obj->mode = 1;
}

void fn_1_268C(M508_OBJ *self)
{
    M508_BOXWORK *work = M508_OBJWORK(M508_BOXWORK, self);
    f32 rx;
    f32 ry;
    f32 rz;

    Hu3DMotionShiftSet(self->modelId, self->motId[lbl_1_rodata_45C[work->kind]], work->shiftTime, 16.0f, 0x40000008);
    Hu3DMotionShiftSpeedSet(self->modelId, work->motSpeed);
    rx = self->rot.x;
    ry = self->rot.y;
    rz = self->rot.z;
    while (1) {
        ry += work->rotSpeed;
        self->rot.x = rx;
        self->rot.y = ry;
        self->rot.z = rz;
        Hu3DModelRotSet(self->modelId, rx, ry, rz);
        if (M508Yield() != 0) {
            return;
        }
    }
}

void fn_1_2838(M508_OBJ *obj)
{
    obj->mode = 2;
}

/* box mode 2: pop up */
void fn_1_2844(M508_OBJ *self)
{
    f32 x;
    f32 z;
    f32 y;
    f32 base;
    f32 t;
    f32 s;
    s32 i;

    self->motNo = 8;
    Hu3DMotionShiftSet(self->modelId, self->motId[8], 0.0f, 8.0f, 0x40000020);
    Hu3DMotionShiftStartEndSet(self->modelId, 0.0f, 45.0f);
    self->rot.y = 45.0f * (f32)frandmod(8);
    Hu3DModelRotSet(self->modelId, self->rot.x, self->rot.y, self->rot.z);
    x = self->pos.x;
    y = self->pos.y;
    z = self->pos.z;
    base = (f32)(410.0 + (410.0 - (410.0 * sin(1.5707963267948966))));
    for (i = 0; i < 0x2D; i++) {
        t = 0.022222223f * (f32)(i + 1);
        s = (f32)HuSin(90.0f * t);
        y = base * (s * (s * s));
        self->pos.x = x;
        self->pos.y = y;
        self->pos.z = z;
        Hu3DModelPosSet(self->modelId, x, y, z);
        self->scale.x = t;
        self->scale.y = t;
        self->scale.z = t;
        Hu3DModelScaleSet(self->modelId, t, t, t);
        if (M508Yield() != 0) {
            return;
        }
    }
    self->scale.x = 1.0f;
    self->scale.y = 1.0f;
    self->scale.z = 1.0f;
    Hu3DModelScaleSet(self->modelId, 1.0f, 1.0f, 1.0f);
    self->pos.x = x;
    self->pos.y = 410.0f;
    self->pos.z = z;
    Hu3DModelPosSet(self->modelId, x, 410.0f, z);
    self->mode = 0;
}

void fn_1_2C7C(M508_OBJ *obj)
{
    obj->mode = 3;
}

/* box mode 3: sink */
void fn_1_2C88(M508_OBJ *self)
{
    f32 x;
    f32 z;
    f32 y;
    f32 t;
    f32 s;
    s32 i;

    Hu3DMotionSpeedSet(self->modelId, 1.0f);
    self->motNo = 8;
    Hu3DMotionShiftSet(self->modelId, self->motId[8], 0.0f, 16.0f, 0);
    x = self->pos.x;
    y = self->pos.y;
    z = self->pos.z;
    for (i = 0; i < 0x2D; i++) {
        t = 0.022222223f * (f32)(i + 1);
        s = (f32)HuSin(90.0f * t);
        y = 410.0f - (410.0f * (s * (s * s)));
        self->pos.x = x;
        self->pos.y = y;
        self->pos.z = z;
        Hu3DModelPosSet(self->modelId, x, y, z);
        self->scale.x = 1.0f - t;
        self->scale.y = 1.0f - t;
        self->scale.z = 1.0f - t;
        Hu3DModelScaleSet(self->modelId, self->scale.x, self->scale.y, self->scale.z);
        if (M508Yield() != 0) {
            return;
        }
    }
    self->pos.x = x;
    self->pos.y = 0.0f;
    self->pos.z = z;
    Hu3DModelPosSet(self->modelId, x, 0.0f, z);
    self->mode = 0;
}

void fn_1_2F80(M508_OBJ *obj)
{
    obj->mode = 4;
}

f32 fn_1_2F8C(f32 a, f32 b, f32 t)
{
    return fn_1_E140(a, b, t);
}

/* box mode 4: turn to face front */
void fn_1_3090(M508_OBJ *self)
{
    f32 rx;
    f32 ry;
    f32 rz;
    f32 d;

    self->motNo = 0;
    Hu3DMotionShiftSet(self->modelId, self->motId[0], 9.0f, 9.0f, 0x40000001);
    rx = self->rot.x;
    ry = self->rot.y;
    rz = self->rot.z;
    while (1) {
        d = fmod((f32)HuAtan(0.0, 2.0) - ry, 360.0);
        if (d < 0.0f) {
            d += 360.0f;
        }
        if (d > 180.0f) {
            d -= 360.0f;
        }
        ry = fmod(ry + (d * 0.1f), 360.0);
        if (ry < 0.0f) {
            ry += 360.0f;
        }
        self->rot.x = rx;
        self->rot.y = ry;
        self->rot.z = rz;
        Hu3DModelRotSet(self->modelId, rx, ry, rz);
        if (!(ry < 1.0f) || !(ry > -1.0f)) {
            if (M508Yield() != 0) {
                return;
            }
            continue;
        }
        self->rot.x = 0.0f;
        self->rot.y = 0.0f;
        self->rot.z = 0.0f;
        Hu3DModelRotSet(self->modelId, 0.0f, 0.0f, 0.0f);
        self->mode = 0;
        return;
    }
}

void fn_1_33D8(M508_OBJ *obj)
{
    obj->mode = 5;
}

/* box mode 5: bounce forever */
void fn_1_33E4(M508_OBJ *self)
{
    f32 x;
    f32 y;
    f32 z;
    s32 i;

    self->motNo = 0;
    Hu3DMotionShiftSet(self->modelId, self->motId[0], 0.0f, 8.0f, 0x40000001);
    while (1) {
        x = self->pos.x;
        y = self->pos.y;
        z = self->pos.z;
        for (i = 0; i < 10; i++) {
            y = (f32)(400.0 - (80.0 * HuSin(180.0f * (0.1f * (f32)(i + 1)))));
            self->pos.x = x;
            self->pos.y = y;
            self->pos.z = z;
            Hu3DModelPosSet(self->modelId, x, y, z);
            if (M508Yield() != 0) {
                return;
            }
        }
        for (i = 0; i < 0x1E; i++) {
            y = (f32)(400.0 + (200.0 * HuSin(180.0f * (0.033333335f * (f32)(i + 1)))));
            self->pos.x = x;
            self->pos.y = y;
            self->pos.z = z;
            Hu3DModelPosSet(self->modelId, x, y, z);
            if (M508Yield() != 0) {
                return;
            }
        }
        self->pos.x = x;
        self->pos.y = 400.0f;
        self->pos.z = z;
        Hu3DModelPosSet(self->modelId, x, 400.0f, z);
    }
}

M508_OBJ *fn_1_37E0(s32 index)
{
    M508_OBJ *obj;
    M508_HOLDERWORK *work;

    obj = fn_1_7A3C(0x1000, 0x2000, lbl_1_rodata_4E8[index], 0, NULL, 1, NULL);
    work = M508_OBJWORK(M508_HOLDERWORK, obj);
    work->model = Hu3DModelCreate(HuDataSelHeapReadNum(0x420013, 0x10000000, HEAP_MODEL));
    work->light0 = Hu3DLLightCreateV(obj->modelId, &lbl_1_rodata_4CC, &lbl_1_rodata_4D8, &lbl_1_rodata_4E4);
    Hu3DLLightInfinitytSet(obj->modelId, work->light0);
    work->light1 = Hu3DLLightCreateV(work->model, &lbl_1_rodata_4CC, &lbl_1_rodata_4D8, &lbl_1_rodata_4E4);
    Hu3DLLightInfinitytSet(work->model, work->light1);
    return obj;
}

void fn_1_38CC(M508_OBJ *obj, f32 x, f32 y, f32 z)
{
    M508_HOLDERWORK *work = M508_OBJWORK(M508_HOLDERWORK, obj);

    Hu3DModelPosSet(work->model, x, y, z);
    obj->pos.x = x;
    obj->pos.y = y;
    obj->pos.z = z;
    Hu3DModelPosSet(obj->modelId, x, y, z);
}

M508_OBJ *fn_1_3958(s16 playerNo)
{
    M508_OBJ *obj;
    M508_CHARWORK *work;

    obj = fn_1_8308(0x1000, 0x2800, playerNo, 9, (u32 *)lbl_1_rodata_4FC, 8, lbl_1_data_54, 2);
    obj->motNo = 5;
    Hu3DMotionShiftSet(obj->modelId, obj->motId[5], 0.0f, 0.0f, 0x40000001);
    CharModelVoicePanSet(playerNo, lbl_1_rodata_C4[playerNo]);
    work = M508_OBJWORK(M508_CHARWORK, obj);
    work->hasItem = 0;
    work->item = -1;
    obj->mode = 0;
    return obj;
}

void fn_1_3A38(M508_OBJ *obj)
{
    obj->mode = 0;
}

/* player mode 0: hold the empty hand item */
void fn_1_3A44(M508_OBJ *self)
{
    M508_OBJ *holder;

    self->motNo = 5;
    Hu3DMotionShiftSet(self->modelId, self->motId[5], 0.0f, 8.0f, 0x40000001);
    holder = lbl_1_bss_44[0][self->playerNo];
    Hu3DModelHookSet(self->modelId, CharModelItemHookGet(self->charNo, 2, 0), holder->modelId);
    holder = lbl_1_bss_44[0][self->playerNo];
    holder->dispF = 1;
    Hu3DModelAttrReset(holder->modelId, 1);
    while (1) {
        if (M508Yield() != 0) {
            return;
        }
    }
}

void fn_1_3B8C(M508_OBJ *obj)
{
    obj->mode = 1;
}

/* player mode 1: choose a box */
void fn_1_3B98(M508_OBJ *self)
{
    M508_CHARWORK *work = M508_OBJWORK(M508_CHARWORK, self);
    M508_OBJ *holder;
    s32 sel;
    s32 delay;
    s32 i;
    s16 pad;

    if (work->item != -1) {
        Hu3DModelHookReset(self->modelId);
        holder = lbl_1_bss_44[work->item][self->playerNo];
        holder->dispF = 0;
        Hu3DModelAttrSet(holder->modelId, 1);
    }
    holder = lbl_1_bss_44[5][self->playerNo];
    Hu3DModelHookSet(self->modelId, CharModelItemHookGet(self->charNo, 2, 0), holder->modelId);
    holder = lbl_1_bss_44[5][self->playerNo];
    holder->dispF = 1;
    Hu3DModelAttrReset(holder->modelId, 1);
    work->hasItem = 0;
    work->item = -1;
    sel = -1;
    if (self->comF != 0) {
        delay = frandmod(0x78) + 0x1E;
        for (i = 0; i < delay; i++) {
            if (M508Yield() != 0) {
                return;
            }
        }
        if (frandmod(100) < lbl_1_rodata_6D0[self->dif]) {
            sel = lbl_1_bss_110;
        } else {
            sel = frandmod(5);
        }
    } else {
        pad = self->padNo;
        while (sel == -1) {
            if ((HuPadBtnDown[pad] & 0x200) != 0) {
                sel = 0;
            }
            if ((HuPadBtnDown[pad] & 0x100) != 0) {
                sel = 1;
            }
            if ((HuPadBtnDown[pad] & 0x800) != 0) {
                sel = 2;
            }
            if ((HuPadBtnDown[pad] & 0x400) != 0) {
                sel = 3;
            }
            if ((HuPadBtnDown[pad] & 0x20) != 0) {
                sel = 4;
            }
            if (M508Yield() != 0) {
                return;
            }
        }
    }
    lbl_1_bss_0++;
    work->hasItem = 1;
    work->item = sel;
    holder = lbl_1_bss_44[5][self->playerNo];
    holder->dispF = 0;
    Hu3DModelAttrSet(holder->modelId, 1);
    Hu3DModelHookReset(self->modelId);
    Hu3DModelHookSet(self->modelId, lbl_1_data_74, lbl_1_bss_44[sel][self->playerNo]->modelId);
    holder = lbl_1_bss_44[sel][self->playerNo];
    holder->dispF = 1;
    Hu3DModelAttrReset(holder->modelId, 1);
    switch (self->playerNo) {
        case 0:
            HuAudFXPlay(0x689);
            break;
        case 1:
            HuAudFXPlay(0x68A);
            break;
        case 2:
            HuAudFXPlay(0x68B);
            break;
        case 3:
            HuAudFXPlay(0x68C);
            break;
    }
    self->motNo = 2;
    Hu3DMotionShiftSet(self->modelId, self->motId[2], 0.0f, 8.0f, 0);
    for (i = 0; i < 0x14; i++) {
        if (M508Yield() != 0) {
            return;
        }
    }
    self->motNo = 3;
    Hu3DMotionShiftSet(self->modelId, self->motId[3], 0.0f, 8.0f, 0x40000001);
    while (1) {
        if (M508Yield() != 0) {
            return;
        }
    }
}

void fn_1_4150(M508_OBJ *obj)
{
    obj->mode = 2;
}

/* player mode 2: turn around */
void fn_1_415C(M508_OBJ *self)
{
    f32 start;
    f32 ry;
    s32 i;

    self->motNo = 6;
    Hu3DMotionShiftSet(self->modelId, self->motId[6], 0.0f, 8.0f, 0);
    start = self->rot.y;
    for (i = 0; i < 0x2E; i++) {
        ry = start + ((-180.0f - start) * (0.02173913f * (f32)(i + 1)));
        self->rot.x = 0.0f;
        self->rot.y = ry;
        self->rot.z = 0.0f;
        Hu3DModelRotSet(self->modelId, 0.0f, ry, 0.0f);
        if (M508Yield() != 0) {
            return;
        }
    }
    self->rot.x = 0.0f;
    self->rot.y = -180.0f;
    self->rot.z = 0.0f;
    Hu3DModelRotSet(self->modelId, 0.0f, -180.0f, 0.0f);
    self->mode = 0;
}

void fn_1_4374(M508_OBJ *obj)
{
    obj->mode = 3;
}

/* player mode 3: spin to the front and react */
void fn_1_4380(M508_OBJ *self)
{
    M508_CHARWORK *work = M508_OBJWORK(M508_CHARWORK, self);
    f32 start;
    f32 ry;
    s32 i;

    HuPrcSleep(0x3B);
    if (work->hasItem != 0) {
        self->motNo = 4;
        Hu3DMotionShiftSet(self->modelId, self->motId[4], 0.0f, 8.0f, 0);
    } else {
        self->motNo = 6;
        Hu3DMotionShiftSet(self->modelId, self->motId[6], 0.0f, 8.0f, 0);
    }
    start = self->rot.y;
    for (i = 0; i < 0x3C; i++) {
        ry = start + ((-360.0f - start) * (0.016666668f * (f32)(i + 1)));
        self->rot.x = 0.0f;
        self->rot.y = ry;
        self->rot.z = 0.0f;
        Hu3DModelRotSet(self->modelId, 0.0f, ry, 0.0f);
        if (M508Yield() != 0) {
            return;
        }
    }
    self->rot.x = 0.0f;
    self->rot.y = 0.0f;
    self->rot.z = 0.0f;
    Hu3DModelRotSet(self->modelId, 0.0f, 0.0f, 0.0f);
    while (1) {
        if (M508Yield() != 0) {
            return;
        }
    }
}

s32 fn_1_4654(M508_OBJ *obj)
{
    return (lbl_1_bss_110 == M508_OBJWORK(M508_CHARWORK, obj)->item) ? 1 : 0;
}

void fn_1_469C(M508_OBJ *obj)
{
    obj->mode = 4;
}

/* player mode 4: happy */
void fn_1_46A8(M508_OBJ *self)
{
    self->motNo = 7;
    Hu3DMotionShiftSet(self->modelId, self->motId[7], 0.0f, 8.0f, 0);
    while (1) {
        if (M508Yield() != 0) {
            return;
        }
    }
}

void fn_1_4790(M508_OBJ *obj)
{
    obj->mode = 5;
}

/* player mode 5: sad */
void fn_1_479C(M508_OBJ *self)
{
    self->motNo = 8;
    Hu3DMotionShiftSet(self->modelId, self->motId[8], 0.0f, 8.0f, 0);
    while (1) {
        if (M508Yield() != 0) {
            return;
        }
    }
}

void fn_1_4884(M508_OBJ *obj)
{
    obj->mode = 6;
}

/* player mode 6: winner pose with dust and smoke at the item hook */
void fn_1_4890(M508_OBJ *self)
{
    HuVecF pos;
    Mtx mtx;

    CharModelHookDustCreate(self->charNo, CharModelItemHookGet(self->charNo, 2, 0));
    Hu3DModelObjMtxGet(self->modelId, CharModelItemHookGet(self->charNo, 2, 0), mtx);
    pos.x = mtx[0][3];
    pos.y = mtx[1][3];
    pos.z = mtx[2][3];
    CharEffectSmokeCreate(1, &pos);
    self->motNo = 0;
    Hu3DMotionShiftSet(self->modelId, self->motId[0], 0.0f, 8.0f, 0);
    while (1) {
        if (M508Yield() != 0) {
            return;
        }
    }
}

void fn_1_49EC(M508_OBJ *obj)
{
    obj->mode = 7;
}

/* player mode 7: loser pose with dust and smoke at the item hook */
void fn_1_49F8(M508_OBJ *self)
{
    HuVecF pos;
    Mtx mtx;

    CharModelHookDustCreate(self->charNo, CharModelItemHookGet(self->charNo, 2, 0));
    Hu3DModelObjMtxGet(self->modelId, CharModelItemHookGet(self->charNo, 2, 0), mtx);
    pos.x = mtx[0][3];
    pos.y = mtx[1][3];
    pos.z = mtx[2][3];
    CharEffectSmokeCreate(1, &pos);
    self->motNo = 1;
    Hu3DMotionShiftSet(self->modelId, self->motId[1], 0.0f, 8.0f, 0);
    while (1) {
        if (M508Yield() != 0) {
            return;
        }
    }
}

/* score display for one player */
M508_OBJ *fn_1_4B54(s32 playerNo)
{
    M508_OBJ *obj;
    M508_SCOREWORK *work;
    s16 i;
    s32 dataNum;

    obj = fn_1_7848(0x1000, 0x2000, 2, lbl_1_data_90);
    work = M508_OBJWORK(M508_SCOREWORK, obj);
    work->grp = HuSprGrpCreate(7);
    work->win = MgScoreWinCreate(0x70, 0x28);
    for (i = 0; i < 7; i++) {
        if (i < 4) {
            if (i != 0) {
                dataNum = 0x420030;
            } else {
                dataNum = lbl_1_rodata_6F0[GwPlayerConf[playerNo].charNo];
            }
        } else {
            dataNum = 0x420031;
        }
        work->spr[i] = HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(dataNum, 0x10000000, HEAP_MODEL)), 0, 0);
        HuSprGrpMemberSet(work->grp, i, work->spr[i]);
    }
    MgScoreWinPosSet(work->win, 0.0f, 0.0f);
    HuSprPosSet(work->grp, 0, -36.0f, 0.0f);
    HuSprPosSet(work->grp, 1, -8.0f, 0.0f);
    HuSprPosSet(work->grp, 2, 14.0f, 0.0f);
    HuSprPosSet(work->grp, 3, 36.0f, 0.0f);
    HuSprPosSet(work->grp, 4, -8.0f, 0.0f);
    HuSprPosSet(work->grp, 5, 14.0f, 0.0f);
    HuSprPosSet(work->grp, 6, 36.0f, 0.0f);
    HuSprPriSet(work->grp, 0, 0xB);
    HuSprPriSet(work->grp, 1, 0xB);
    HuSprPriSet(work->grp, 2, 0xB);
    HuSprPriSet(work->grp, 3, 0xB);
    HuSprPriSet(work->grp, 4, 0xA);
    HuSprPriSet(work->grp, 5, 0xA);
    HuSprPriSet(work->grp, 6, 0xA);
    MgScoreWinDispSet(work->win, 0);
    HuSprAttrSet(work->grp, 4, 4);
    HuSprAttrSet(work->grp, 5, 4);
    HuSprAttrSet(work->grp, 6, 4);
    work->count = 0;
    return obj;
}

void fn_1_4F48(M508_OBJ *obj, f32 x, f32 y)
{
    M508_SCOREWORK *work = M508_OBJWORK(M508_SCOREWORK, obj);

    work->x = x;
    work->y = y;
    HuSprGrpPosSet(work->grp, x, y);
    MgScoreWinPosSet(work->win, work->x, work->y);
}

void fn_1_4FA8(M508_OBJ *obj)
{
    M508_SCOREWORK *work = M508_OBJWORK(M508_SCOREWORK, obj);
    s16 i;

    for (i = 0; i < 4; i++) {
        HuSprAttrReset(work->grp, i, 4);
    }
    MgScoreWinDispSet(work->win, 1);
}

void fn_1_5010(M508_OBJ *obj)
{
    M508_SCOREWORK *work = M508_OBJWORK(M508_SCOREWORK, obj);
    s16 i;

    for (i = 0; i < 7; i++) {
        HuSprAttrSet(work->grp, i, 4);
    }
    MgScoreWinDispSet(work->win, 0);
}

void fn_1_5078(M508_OBJ *obj)
{
    obj->mode = 0;
}

void fn_1_5084(M508_OBJ *self)
{
    while (1) {
        if (M508Yield() != 0) {
            return;
        }
    }
}

void fn_1_512C(M508_OBJ *obj)
{
    M508_SCOREWORK *work = M508_OBJWORK(M508_SCOREWORK, obj);

    if (work->count < 3) {
        work->count++;
    }
    obj->mode = 1;
}

/* score mode 1: pop the newest digit in */
void fn_1_5164(M508_OBJ *self)
{
    M508_SCOREWORK *work = M508_OBJWORK(M508_SCOREWORK, self);
    s32 count = work->count;
    f32 s;
    s32 i;

    HuSprAttrReset(work->grp, (s16)(count + 3), 4);
    for (i = 0; i < 0x3C; i++) {
        s = (f32)HuSin(90.0f * (0.016666668f * (f32)(i + 1)));
        HuSprScaleSet(work->grp, (s16)(count + 3), 1.0f + (2.0f - (2.0f * s)), 1.0f + (2.0f - (2.0f * s)));
        HuSprTPLvlSet(work->grp, (s16)(count + 3), s);
        if (M508Yield() != 0) {
            return;
        }
    }
    HuSprScaleSet(work->grp, (s16)(count + 3), 1.0f, 1.0f);
    HuSprTPLvlSet(work->grp, (s16)(count + 3), 1.0f);
    self->mode = 0;
}

M508_OBJ *fn_1_5394(s32 kind)
{
    M508_OBJ *obj;

    obj = fn_1_7A3C(0x1000, 0x2000, lbl_1_rodata_738[kind], 0, NULL, 1, NULL);
    obj->dispF = 0;
    Hu3DModelAttrSet(obj->modelId, 1);
    return obj;
}

M508_OBJ *fn_1_5414(void)
{
    return fn_1_7A3C(0x1000, 0x2000, -1, 0, NULL, 2, lbl_1_data_98);
}

void fn_1_5464(M508_OBJ *obj)
{
    obj->mode = 0;
}

void fn_1_5470(M508_OBJ *self)
{
    while (1) {
        if (M508Yield() != 0) {
            return;
        }
    }
}

void fn_1_5518(M508_OBJ *obj)
{
    obj->mode = 1;
}

/* camera motion model: play the fly-by */
void fn_1_5524(M508_OBJ *self)
{
    s16 model;
    s32 i;

    model = Hu3DModelCameraCreate(Hu3DMotionCreate(HuDataSelHeapReadNum(0x42000A, 0x10000000, HEAP_MODEL)), 1);
    Hu3DCameraMotionStart(model, 1);
    for (i = 0; i < 0xB4; i++) {
        if (M508Yield() != 0) {
            return;
        }
    }
    Hu3DModelKill(model);
    self->mode = 0;
}

M508_OBJ *fn_1_560C(void)
{
    return fn_1_7848(0x1000, 0x2000, 1, lbl_1_data_A0);
}

M508_OBJ *fn_1_5758(s32 kind)
{
    M508_OBJ *obj;

    obj = fn_1_7A3C(0x1000, 0x2000, lbl_1_rodata_754[kind], 0, NULL, 2, lbl_1_data_A4);
    M508_OBJWORK(M508_KINDWORK, obj)->kind = kind;
    return obj;
}

void fn_1_57D4(M508_OBJ *obj)
{
    obj->mode = 0;
}

void fn_1_57E0(M508_OBJ *self)
{
    self->dispF = 0;
    Hu3DModelAttrSet(self->modelId, 1);
    Hu3DModelAttrSet(self->modelId, 0x40000002);
    while (1) {
        if (M508Yield() != 0) {
            return;
        }
    }
}

void fn_1_58B8(M508_OBJ *obj)
{
    obj->mode = 1;
}

/* sign mode 1: show and play, behaviour depends on the sign kind */
void fn_1_58C4(M508_OBJ *self)
{
    s16 model;
    s32 i;

    switch (M508_OBJWORK(M508_KINDWORK, self)->kind) {
        case 1:
            self->dispF = 1;
            Hu3DModelAttrReset(self->modelId, 1);
            model = self->modelId;
            Hu3DModelAttrReset(model, 0x40000002);
            Hu3DModelAttrSet(model, 0x40000001);
            Hu3DMotionTimeSet(self->modelId, 0.0f);
            while (Hu3DMotionEndCheck(self->modelId) == 0) {
                if (M508Yield() != 0) {
                    return;
                }
            }
            break;
        case 0:
        case 2:
        case 3:
        case 4:
            self->dispF = 1;
            Hu3DModelAttrReset(self->modelId, 1);
            model = self->modelId;
            Hu3DModelAttrReset(model, 0x40000002);
            Hu3DModelAttrReset(model, 0x40000001);
            Hu3DMotionTimeSet(self->modelId, 0.0f);
            while (Hu3DMotionEndCheck(self->modelId) == 0) {
                if (M508Yield() != 0) {
                    return;
                }
            }
            break;
        case 5:
            self->pos.y = -20.0f;
            Hu3DModelPosSet(self->modelId, self->pos.x, self->pos.y, self->pos.z);
            Hu3DModelAttrReset(self->modelId, 0x40000004);
            self->dispF = 1;
            Hu3DModelAttrReset(self->modelId, 1);
            model = self->modelId;
            Hu3DModelAttrReset(model, 0x40000002);
            Hu3DModelAttrReset(model, 0x40000001);
            Hu3DMotionTimeSet(self->modelId, 0.0f);
            for (i = 0; i < 0x6E; i++) {
                if (M508Yield() != 0) {
                    return;
                }
            }
            break;
    }
    self->mode = 0;
}

void fn_1_5C7C(s16 playerNo, s32 kind, s16 time)
{
    switch (kind) {
        case 0:
            omVibrate(playerNo, time, 6, 6);
            break;
        case 1:
            omVibrate(playerNo, time, 4, 2);
            break;
        case 2:
            omVibrate(playerNo, time, 0xC, 0);
            break;
    }
}

/* ---- the object library's own copies (forwarded to object.c) ---- */

float fn_1_5D20(M508_VEC2 *a, M508_VEC2 *b)
{
    return a->x * b->x + a->y * b->y;
}

float fn_1_5D40(M508_VEC2 *a, M508_VEC2 *b)
{
    return a->x * b->y - a->y * b->x;
}

float fn_1_5D60(M508_VEC2 *vec)
{
    return fn_1_A194(vec);
}

void fn_1_5EE0(M508_VEC2 *dst, M508_VEC2 *src)
{
    fn_1_A314(dst, src);
}

BOOL fn_1_60A8(M508_VEC2 *point, M508_CIRCLE *circle)
{
    return fn_1_A4DC(point, circle);
}

BOOL fn_1_6130(M508_CIRCLE *a, M508_CIRCLE *b)
{
    return fn_1_A564(a, b);
}

BOOL fn_1_61C0(M508_RECT *a, M508_RECT *b)
{
    return fn_1_A5F4(a, b);
}

BOOL fn_1_6228(M508_VEC2 *point, M508_POLY *poly)
{
    return fn_1_A65C(point, poly);
}

BOOL fn_1_6318(M508_CIRCLE *circle, M508_RECT *line)
{
    return fn_1_A74C(circle, line);
}

BOOL fn_1_6964(M508_CIRCLE *circle, M508_POLY *poly)
{
    return fn_1_AD98(circle, poly);
}

BOOL fn_1_6AF4(M508_RECT *rect, M508_CIRCLE *circle)
{
    return fn_1_AF28(rect, circle);
}

BOOL fn_1_6D8C(M508_SPHERE *a, M508_SPHERE *b)
{
    return fn_1_B1C0(a, b);
}

BOOL fn_1_6E40(M508_CIRCLE *push, M508_CIRCLE *fixed)
{
    return fn_1_B274(push, fixed);
}

BOOL fn_1_70E0(M508_CIRCLE *push, M508_RECT *line)
{
    return fn_1_B514(push, line);
}

M508_OBJ *fn_1_7848(s32 prio, u32 stackSize, s32 funcNum, M508_OBJFUNC *funcTbl)
{
    return fn_1_BC7C(prio, stackSize, funcNum, funcTbl);
}

void fn_1_797C(M508_OBJ *obj)
{
    fn_1_BDB0(obj);
}

void fn_1_79BC(void)
{
    M508_OBJ *obj = HuPrcCurrentGet()->property;

    while (obj->mode < obj->modeNum) {
        obj->modeFunc[obj->mode](obj);
        obj->prevMode = obj->mode;
    }
    HuPrcEnd();
}

void fn_1_7A24(M508_OBJ *obj)
{
    while (1) {
        HuPrcSleep(0);
    }
}

M508_OBJ *fn_1_7A3C(s32 prio, u32 stackSize, s32 dataNum, s32 motNum, u32 *motTbl, s32 funcNum, M508_OBJFUNC *funcTbl)
{
    return fn_1_BE70(prio, stackSize, dataNum, motNum, motTbl, funcNum, funcTbl);
}

void fn_1_7CC0(M508_OBJ *obj)
{
    fn_1_C0F4(obj);
}

void fn_1_7D60(M508_OBJ *obj, f32 x, f32 y, f32 z)
{
    fn_1_C194(obj, x, y, z);
}

void fn_1_7DB4(M508_OBJ *obj, f32 *x, f32 *y, f32 *z)
{
    fn_1_C1E8(obj, x, y, z);
}

void fn_1_7DF4(M508_OBJ *obj, f32 x, f32 y, f32 z)
{
    fn_1_C228(obj, x, y, z);
}

void fn_1_7E48(M508_OBJ *obj, f32 *x, f32 *y, f32 *z)
{
    fn_1_C27C(obj, x, y, z);
}

void fn_1_7E88(M508_OBJ *obj, f32 x, f32 y, f32 z)
{
    fn_1_C2BC(obj, x, y, z);
}

void fn_1_7EDC(M508_OBJ *obj, f32 *x, f32 *y, f32 *z)
{
    fn_1_C310(obj, x, y, z);
}

void fn_1_7F1C(M508_OBJ *obj, f32 tpLvl)
{
    fn_1_C350(obj, tpLvl);
}

f32 fn_1_7F58(M508_OBJ *obj)
{
    return fn_1_C38C(obj);
}

void fn_1_7F60(M508_OBJ *obj)
{
    fn_1_C394(obj);
}

void fn_1_7F9C(M508_OBJ *obj)
{
    fn_1_C3D0(obj);
}

BOOL fn_1_7FD8(M508_OBJ *obj)
{
    return fn_1_C40C(obj);
}

void fn_1_7FE0(M508_OBJ *obj, s32 time)
{
    fn_1_C414(obj, time);
}

void fn_1_803C(M508_OBJ *obj, BOOL loopF)
{
    fn_1_C470(obj, loopF);
}

void fn_1_80B4(M508_OBJ *obj)
{
    fn_1_C4E8(obj);
}

BOOL fn_1_80EC(M508_OBJ *obj)
{
    return fn_1_C520(obj);
}

void fn_1_813C(M508_OBJ *obj, s32 motNo, u32 attr, f32 start, f32 end)
{
    fn_1_C570(obj, motNo, start, end, attr);
}

s32 fn_1_81B8(M508_OBJ *obj)
{
    return fn_1_C5EC(obj);
}

void fn_1_81C0(M508_OBJ *obj, f32 speed)
{
    fn_1_C5F4(obj, speed);
}

void fn_1_81FC(M508_OBJ *obj)
{
    fn_1_C630(obj);
}

void fn_1_822C(M508_OBJ *obj)
{
    fn_1_C660(obj);
}

void fn_1_825C(M508_OBJ *obj)
{
    fn_1_C690(obj);
}

void fn_1_828C(M508_OBJ *obj)
{
    fn_1_C6C0(obj);
}

void fn_1_8290(M508_OBJ *obj, char *objName, M508_OBJ *hookObj)
{
    fn_1_C6C4(obj, objName, hookObj);
}

void fn_1_82D8(M508_OBJ *obj)
{
    fn_1_C70C(obj);
}

M508_OBJ *fn_1_8308(s32 prio, u32 stackSize, s32 playerNo, s32 motNum, u32 *motTbl, s32 funcNum, M508_OBJFUNC *funcTbl, s16 modelNo)
{
    return fn_1_C73C(prio, stackSize, playerNo, motNum, motTbl, funcNum, funcTbl, modelNo);
}

void fn_1_85F8(M508_OBJ *obj)
{
    fn_1_CA2C(obj);
}

void fn_1_8698(M508_OBJ *obj, char *objName)
{
    fn_1_CACC(obj, objName);
}

M508_CAMERA *fn_1_86F0(s16 cameraBit)
{
    return fn_1_CB24(cameraBit);
}

void fn_1_88E0(M508_CAMERA *camera)
{
    fn_1_CD14(camera);
}

void fn_1_8918(void)
{
    M508_CAMERA *camera = HuPrcCurrentGet()->property;

    while (camera->mode != 3) {
        lbl_1_data_0[camera->mode](camera);
    }
    HuPrcEnd();
}

void fn_1_897C(M508_CAMERA *camera)
{
    fn_1_CDB0(camera);
}

void fn_1_8BF4(M508_CAMERA *camera)
{
    fn_1_D028(camera);
}

void fn_1_8C64(M508_CAMERA *camera)
{
    fn_1_D098(camera);
}

void fn_1_8C70(M508_CAMERA *camera)
{
    fn_1_D0A4(camera);
}

void fn_1_8D24(M508_CAMERA *camera)
{
    fn_1_D158(camera);
}

void fn_1_8D30(M508_CAMERA *camera)
{
    fn_1_D164(camera);
}

BOOL fn_1_8DF8(M508_CAMERA *camera)
{
    return fn_1_D22C(camera);
}

void fn_1_8E28(M508_CAMERA *camera, HuVecF *center, HuVecF *rot, float *zoom, int time)
{
    fn_1_D25C(camera, center, rot, zoom, time);
}

void fn_1_926C(M508_CAMERA *camera)
{
    fn_1_D6A0(camera);
}

void fn_1_9578(M508_CAMERA *camera, float vpX, float vpY, float vpW, float vpH, float vpNearZ, float vpFarZ)
{
    fn_1_D9AC(camera, vpX, vpY, vpW, vpH, vpNearZ, vpFarZ);
}

void fn_1_95D8(M508_CAMERA *camera, float fov, float near, float far, float aspect)
{
    fn_1_DA0C(camera, fov, near, far, aspect);
}

void fn_1_9628(M508_CAMERA *camera, float scissorX, float scissorY, float scissorW, float scissorH)
{
    fn_1_DA5C(camera, scissorX, scissorY, scissorW, scissorH);
}

void fn_1_96BC(M508_CAMERA *camera)
{
    fn_1_DAF0(camera);
}

void fn_1_9B38(M508_CAMERA *camera)
{
    fn_1_DF6C(camera);
}

void fn_1_9B3C(int time, BOOL waitF)
{
    fn_1_DF70(time, waitF);
}

void fn_1_9B98(int time, BOOL waitF)
{
    fn_1_DFCC(time, waitF);
}

void fn_1_9BF4(int time)
{
    fn_1_E028(time);
}

void fn_1_9C80(int time)
{
    fn_1_E0B4(time);
}

float fn_1_9D0C(float start, float end, float t)
{
    return fn_1_E140(start, end, t);
}

M508_WIN *fn_1_9E10(int w, int h)
{
    return fn_1_E244(w, h);
}

void fn_1_9EB8(M508_WIN *win)
{
    fn_1_E2EC(win);
}

void fn_1_9EF8(void)
{
    while (1) {
        HuPrcVSleep();
    }
}

void fn_1_9F0C(M508_WIN *win, float x, float y)
{
    fn_1_E340(win, x, y);
}

void fn_1_9F54(M508_WIN *win, float *x, float *y)
{
    fn_1_E388(win, x, y);
}

void fn_1_9F68(M508_WIN *win, u32 mess)
{
    fn_1_E39C(win, mess);
}

void fn_1_9FA0(M508_WIN *win)
{
    fn_1_E3D4(win);
}

void fn_1_9FD8(M508_WIN *win)
{
    fn_1_E40C(win);
}

void fn_1_A010(M508_WIN *win, int color)
{
    fn_1_E444(win, color);
}

void fn_1_A04C(M508_WIN *win, s16 speed)
{
    fn_1_E480(win, speed);
}

void fn_1_A084(M508_WIN *win)
{
    fn_1_E4B8(win);
}

void fn_1_A0B4(M508_WIN *win)
{
    fn_1_E4E8(win);
}

void fn_1_A0E4(M508_WIN *win)
{
    fn_1_E518(win);
}

void fn_1_A114(M508_WIN *win, float scaleX, float scaleY)
{
    fn_1_E548(win, scaleX, scaleY);
}

float fn_1_A154(M508_VEC2 *a, M508_VEC2 *b)
{
    return a->x * b->x + a->y * b->y;
}

float fn_1_A174(M508_VEC2 *a, M508_VEC2 *b)
{
    return a->x * b->y - a->y * b->x;
}
