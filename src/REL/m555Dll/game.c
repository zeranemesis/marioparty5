#include "dolphin/types.h"

void fn_1_2FF0(void);
void fn_1_3010(void *arg);
s32 fn_1_130D8(void);
s32 fn_1_13430(void);
void fn_1_2A48(s32 arg0, s32 arg1);
void fn_1_A758(void);
void fn_1_90C4(void);
void fn_1_9B8C(void);
void fn_1_CFB8(void);
void fn_1_78DC(void);
void fn_1_14420(void);
void fn_1_12E04(void);
void fn_1_119EC(void);
void fn_1_103D8(s16 arg);
void fn_1_12CAC(s16 arg);
void fn_1_11DFC(s16 arg);
void fn_1_155DC(s16 arg);

s32 fn_1_FB2C(void);
s32 fn_1_8F4C(s16 arg);
void Hu3DFogSet(f32 near, f32 far, u8 r, u8 g, u8 b);
void Hu3DFogClear(void);
void MgSeqWinnerSet(s16 a, s32 b, s32 c, s32 d);
s32 _CheckFlag(s32 flag);
void CharFXPlay(s16 id, s32 arg);
void fn_1_2CB0(s16 a, s32 b);
void fn_1_2E60(s32 a, s32 b);
void Hu3DMotionSpeedSet(s16 motion, f32 speed);
void Hu3DModelHookReset(s16 model);
void Hu3DModelAttrSet(s16 model, s32 attr);
void fn_1_7D88(s16 arg);
void fn_1_8928(s16 arg);
void *memset(void *dst, s32 c, u32 n);

extern s16 lbl_1_bss_1894[0xE8 + 0xC];
extern u8 GwPlayer[];

typedef struct {
    u8 pad_0[2];
    s16 unk_2;
    u8 pad_4[0xA];
    s16 unk_E;
    u8 pad_10[4];
    s16 unk_14;
    u8 pad_16[0x3C];
    s16 unk_52;
    u8 pad_54[0x344 - 0x54];
} G555_SUB;

extern G555_SUB lbl_1_bss_120C[2];

typedef struct {
    s16 unk_0;
    s16 unk_2;
    u8 pad_4[4];
    s16 unk_8;
    u8 pad_A[0x12];
} G555_W;

typedef struct {
    f32 v[7];
} G555_F7;

typedef struct {
    s16 unk_0;
    u8 pad_2[0xE];
    f32 unk_10;
    f32 unk_14;
    u8 pad_18[4];
    f32 unk_1C;
    f32 unk_20;
} G555_11E4;

extern G555_11E4 lbl_1_bss_11E4;

#define G555_58_S16(off) (*(s16 *)((u8 *)lbl_1_bss_3030->unk_58 + (off)))

typedef struct {
    s16 unk_0;
    u8 pad_2[2];
    f32 unk_4;
    u8 pad_8[4];
    f32 unk_C;
    u8 pad_10[8];
    s32 unk_18;
    u8 pad_1C[4];
    s32 unk_20;
    u8 pad_24[0x30];
} G555_EL;

typedef struct {
    u8 pad_0[4];
    G555_W win[2];
    u8 pad_3C[0x1C];
    G555_F7 *unk_58;
    u8 pad_5C[0x18];
    G555_EL el[2];
    u8 pad_11C[0x5C];
    s32 unk_178;
} G555_OBJ;

extern s16 lbl_1_bss_B88[];
extern G555_OBJ *lbl_1_bss_3030;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} G555_VEC;

const f32 lbl_1_rodata_68 = 0.0f;
const f32 lbl_1_rodata_6C = 1.0f;
const f32 lbl_1_rodata_70 = -200.0f;
const f32 lbl_1_rodata_74 = 400.0f;
const f32 lbl_1_rodata_78 = -100.0f;
const f32 lbl_1_rodata_7C = 180.0f;
const f32 lbl_1_rodata_80 = 0.5;
const f32 lbl_1_rodata_84 = -10000.0f;
const f32 lbl_1_rodata_88 = 30.0f;
const f32 lbl_1_rodata_8C = -568.0f;
const f32 lbl_1_rodata_90 = 568.0f;
const f32 lbl_1_rodata_94 = -520.0f;
const f32 lbl_1_rodata_98 = 520.0f;
const f32 lbl_1_rodata_9C = -364.0f;
const f32 lbl_1_rodata_A0 = 10000.0f;
const f32 lbl_1_rodata_A4 = 23000.0f;
const f64 lbl_1_rodata_A8 = 4503601774854144.0;
const f32 lbl_1_rodata_B0 = 38.0f;
const f32 lbl_1_rodata_B4 = 10.0f;
const f32 lbl_1_rodata_B8 = 0.005;
const f32 lbl_1_rodata_BC = 15.0f;
const f32 lbl_1_rodata_C0 = 20.0f;
const f32 lbl_1_rodata_C4 = 6.0f;
const f32 lbl_1_rodata_C8 = 56.0f;
const f32 lbl_1_rodata_CC = 90.0f;
const f32 lbl_1_rodata_D0 = -90.0f;
const f32 lbl_1_rodata_D4 = -1.0f;
const f32 lbl_1_rodata_D8 = 2.5;
const f32 lbl_1_rodata_DC = -2.5;
const f64 lbl_1_rodata_E0 = 10.0;
const f64 lbl_1_rodata_E8 = 3.141592653589793;
const f64 lbl_1_rodata_F0 = 180.0;
const f64 lbl_1_rodata_F8 = 30.0;
const f32 lbl_1_rodata_100 = 0.05818182;
const f32 lbl_1_rodata_104 = 64.0f;
const f32 lbl_1_rodata_108 = 14.9;
const s16 lbl_1_rodata_10C[6] = {0x0003, 0x0003, 0x0003, 0x0003, 0x0003, 0x0003};
const s32 lbl_1_rodata_118[24] = {0xC4098000, 0xC3590000, 0xC3A80000, 0xC1A00000, 0xC4098000, 0xC3590000, 0x41A00000, 0x43A80000, 0xC3310000, 0x43310000, 0xC3A80000, 0xC1A00000, 0xC3310000, 0x43310000, 0x41A00000, 0x43A80000, 0x43590000, 0x44098000, 0xC3A80000, 0xC1A00000, 0x43590000, 0x44098000, 0x41A00000, 0x43A80000};
const f32 lbl_1_rodata_178 = 200.0f;
const f32 lbl_1_rodata_17C = -37000.0f;
const f32 lbl_1_rodata_180 = 0.05;
const f32 lbl_1_rodata_184 = 1500.0f;
const f32 lbl_1_rodata_188 = 100.0f;
const f32 lbl_1_rodata_18C = 0.3;
const f64 lbl_1_rodata_190 = 4503599627370496.0;
const f32 lbl_1_rodata_198 = 4.0f;
const f32 lbl_1_rodata_19C = 300.0f;
const f32 lbl_1_rodata_1A0 = -36000.0f;
const f32 lbl_1_rodata_1A4 = 12000.0f;
const f32 lbl_1_rodata_1A8 = 50.0f;
const f32 lbl_1_rodata_1AC = 1.5;
const f32 lbl_1_rodata_1B0 = 96.0f;
const f32 lbl_1_rodata_1B4 = 5000.0f;
const f32 lbl_1_rodata_1B8 = 37000.0f;
const f32 lbl_1_rodata_1BC = 49.0f;
const f32 lbl_1_rodata_1C0 = 97.0f;
const f32 lbl_1_rodata_1C4 = 2.0f;
const f32 lbl_1_rodata_1C8 = 0.7;
const f32 lbl_1_rodata_1CC = 98.0f;
const f32 lbl_1_rodata_1D0 = 8.0f;
const f32 lbl_1_rodata_1D4 = 12.5;
const f32 lbl_1_rodata_1D8 = 130.0f;
const f64 lbl_1_rodata_1E0 = 80.0;
const f32 lbl_1_rodata_1E8 = 94.0f;
const f32 lbl_1_rodata_1EC = 0.035;
const f32 lbl_1_rodata_1F0 = 148.0f;
const s16 lbl_1_rodata_1F4[6] = {0x0002, 0x0000, 0x0006, 0x0004, 0x000A, 0x0008};
const s16 lbl_1_rodata_200[16] = {0x0BB8, 0x03E8, 0x07D0, 0xFC18, 0x0BB8, 0xF060, 0x07D0, 0xFC18, 0x03E8, 0xFE0C, 0x01F4, 0x03E8, 0x03E8, 0xFE0C, 0x0064, 0xF830};
const f32 lbl_1_rodata_220 = 66.666664;
const f32 lbl_1_rodata_224 = 25.0f;
const f32 lbl_1_rodata_228 = 45.0f;
const f32 lbl_1_rodata_22C = 0.01;
const f32 lbl_1_rodata_230 = 750.0f;
const f32 lbl_1_rodata_234 = 9000.0f;
const f32 lbl_1_rodata_238 = 4000.0f;
const f32 lbl_1_rodata_23C = 16000.0f;
const f32 lbl_1_rodata_240 = 1000.0f;
const f32 lbl_1_rodata_244 = -30000.0f;
const f32 lbl_1_rodata_248 = 19.0f;
const f32 lbl_1_rodata_24C = 150.0f;
const f32 lbl_1_rodata_250 = 36000.0f;
const f32 lbl_1_rodata_254 = 600.0f;
const f32 lbl_1_rodata_258 = 700.0f;
const f32 lbl_1_rodata_25C = 500.0f;
const f32 lbl_1_rodata_260 = 225.0f;
const f32 lbl_1_rodata_264 = 135.0f;
const f32 lbl_1_rodata_268 = 3.0f;
const f64 lbl_1_rodata_270 = 50.0;
const f32 lbl_1_rodata_278 = 50000.0f;
const f32 lbl_1_rodata_27C = -4.0f;
const f32 lbl_1_rodata_280 = -0.15;
const f32 lbl_1_rodata_284 = 0.1;
const f32 lbl_1_rodata_288 = -1000.0f;
const f64 lbl_1_rodata_290 = 20.0;
const f32 lbl_1_rodata_298 = 1400.0f;
const s32 lbl_1_rodata_29C[16] = {0x40000000, 0x00000000, 0x40000000, 0x00000000, 0xC0000000, 0x00000000, 0xC0000000, 0x00000000, 0x00000000, 0x40000000, 0x00000000, 0x40000000, 0x00000000, 0xC0000000, 0x00000000, 0xC0000000};
const f32 lbl_1_rodata_2DC = -0.012;
const f32 lbl_1_rodata_2E0 = 0.015;
const f32 lbl_1_rodata_2E4 = -0.08;
const f32 lbl_1_rodata_2E8 = 0.08;
const f32 lbl_1_rodata_2EC = -6000.0f;
const f32 lbl_1_rodata_2F0 = 20.5;
const f32 lbl_1_rodata_2F4 = 112.0f;
const f32 lbl_1_rodata_2F8 = 5.0f;
const f64 lbl_1_rodata_300 = 0.0;
const f64 lbl_1_rodata_308 = 0.5;
const f64 lbl_1_rodata_310 = 3.0;
const f32 lbl_1_rodata_318 = -0.07;
const f32 lbl_1_rodata_31C = 60.0f;
const f32 lbl_1_rodata_320 = -900.0f;
const f32 lbl_1_rodata_324 = -400.0f;
const f32 lbl_1_rodata_328 = -750.0f;
const f32 lbl_1_rodata_32C = 800.0f;
const f32 lbl_1_rodata_330 = -2.0f;
const f32 lbl_1_rodata_334 = 900.0f;
const f32 lbl_1_rodata_338 = -150.0f;
const f32 lbl_1_rodata_33C = -0.5;
const f32 lbl_1_rodata_340 = 240.0f;
const f32 lbl_1_rodata_344 = -868.0f;
const f32 lbl_1_rodata_348 = 868.0f;
const f32 lbl_1_rodata_34C = -820.0f;
const f32 lbl_1_rodata_350 = 820.0f;
const f32 lbl_1_rodata_354 = 120.0f;
const f32 lbl_1_rodata_358 = 40.0f;
const f32 lbl_1_rodata_35C = -15.0f;
const f32 lbl_1_rodata_360 = 80.0f;
const f32 lbl_1_rodata_364 = 360.0f;
const f64 lbl_1_rodata_368 = -1.0;
const f32 lbl_1_rodata_370 = -121.0f;
const f32 lbl_1_rodata_374 = 121.0f;
const f32 lbl_1_rodata_378 = 530.0f;
const f32 lbl_1_rodata_37C = -530.0f;
const f32 lbl_1_rodata_380 = 316.0f;
const f32 lbl_1_rodata_384 = 286.0f;
const f32 lbl_1_rodata_388 = -316.0f;
const f32 lbl_1_rodata_38C = -346.0f;
const s32 lbl_1_rodata_390[4] = {0x41200000, 0x41C80000, 0x42200000, 0x42600000};
const s32 lbl_1_rodata_3A0[4] = {0x00000000, 0x41A00000, 0x42200000, 0x42600000};
const s32 lbl_1_rodata_3B0[4] = {0xC6EA6000, 0xC6EA6000, 0xC6EA6000, 0xC6EA6000};
const s32 lbl_1_rodata_3C0[4] = {0x41C80000, 0x41C80000, 0x40000000, 0x3F800000};
const s32 lbl_1_rodata_3D0[4] = {0x41F00000, 0x41A00000, 0x00000000, 0x00000000};
const s32 lbl_1_rodata_3E0[4] = {0x41F00000, 0x40A00000, 0x00000000, 0x00000000};
const f32 lbl_1_rodata_3F0 = 230.0f;
const s32 lbl_1_rodata_3F4[12] = {0x3F800000, 0x00000000, 0xC404C000, 0x3F800000, 0x00000000, 0x4404C000, 0x00000000, 0x3F800000, 0xC39E8000, 0x00000000, 0x3F800000, 0x439E8000};
const f32 lbl_1_rodata_424 = 2000.0f;
const f32 lbl_1_rodata_428 = -13000.0f;
const f32 lbl_1_rodata_42C = 270.0f;
const f64 lbl_1_rodata_430 = 200.0;
const f32 lbl_1_rodata_438 = 0.984;
const f32 lbl_1_rodata_43C = -0.984;
const f32 lbl_1_rodata_440 = -531.0f;
const f32 lbl_1_rodata_444 = 531.0f;
const f32 lbl_1_rodata_448 = -317.0f;
const f32 lbl_1_rodata_44C = 317.0f;
const f32 lbl_1_rodata_450 = -12000.0f;
const f32 lbl_1_rodata_454 = -16000.0f;

#define BSS1894_BYTE(n) (((u8 *)lbl_1_bss_1894)[n])

s32 fn_1_69C4(void *arg) {
    s32 ret = 1;

    fn_1_3010(arg);
    lbl_1_bss_1894[6] = 0;
    return ret;
}

s16 fn_1_6A0C(void) {
    s16 result = 0;

    result = fn_1_130D8();
    fn_1_A758();
    fn_1_9B8C();
    fn_1_CFB8();
    fn_1_78DC();
    lbl_1_bss_1894[6] = 1;
    return result;
}

s16 fn_1_6A60(void) {
    s16 result = 0;

    fn_1_FB2C();
    lbl_1_bss_1894[0] = 0;
    lbl_1_bss_1894[1] = 0;
    lbl_1_bss_1894[2] = 0;
    lbl_1_bss_1894[3] = 0;
    fn_1_A758();
    fn_1_9B8C();
    fn_1_CFB8();
    fn_1_78DC();
    lbl_1_bss_1894[6] = 2;
    return result;
}

s32 fn_1_6AF0(void) {
    s32 result = 0;
    s32 v = 0;
    s16 i;
    s32 pidx;

    *(s32 *)&lbl_1_bss_1894[4] += 1;

    switch (lbl_1_bss_1894[0]) {
    case 0:
        v = 1;
        v = fn_1_FB2C();
        if ((s16)v != 0) {
            lbl_1_bss_1894[0]++;
        }
        break;

    case 1:
        for (i = 0; i < 2; i++) {
            if (lbl_1_bss_120C[i].unk_52 != 0) {
                fn_1_155DC(i);
            }
        }
        for (i = 0; i < 2; i++) {
            fn_1_8F4C(i);
        }
        if (lbl_1_bss_1894[0x14] == 0 && lbl_1_bss_1894[0x15] == 0) {
            break;
        }
        if (lbl_1_bss_1894[0x14] == 0) {
            if (lbl_1_bss_120C[0].unk_14 > lbl_1_bss_120C[1].unk_14) {
                lbl_1_bss_1894[0x14] = 2;
            } else if (lbl_1_bss_120C[0].unk_14 < lbl_1_bss_120C[1].unk_14) {
                lbl_1_bss_1894[0x14] = 1;
            } else {
                lbl_1_bss_1894[0x14] = 0;
            }
            lbl_1_bss_1894[0x20] = 1;
        }
        lbl_1_bss_1894[0]++;
        break;

    case 2:
        if (lbl_1_bss_1894[0x14] != 0) {
            MgSeqWinnerSet(lbl_1_bss_3030->win[lbl_1_bss_1894[0x14] & 1].unk_2, -1, -1, -1);
            pidx = lbl_1_bss_3030->win[lbl_1_bss_1894[0x14] & 1].unk_0;
            if (!_CheckFlag(0x1000F)) {
                *(s16 *)(GwPlayer + pidx * 0x108 + 0x2C) = 10;
            }
        } else {
            MgSeqWinnerSet(-1, -1, -1, -1);
        }
        fn_1_2A48(1, 100);
        result = 1;
        break;
    }

    fn_1_A758();
    fn_1_90C4();
    fn_1_9B8C();
    fn_1_CFB8();
    fn_1_14420();
    fn_1_78DC();
    lbl_1_bss_1894[6] = 3;
    return result;
}

s16 fn_1_6DF0(void) {
    s16 result = 0;

    lbl_1_bss_1894[0] = 0;
    lbl_1_bss_1894[1] = 0;
    lbl_1_bss_1894[2] = 0;
    lbl_1_bss_1894[3] = 0;
    lbl_1_bss_3030->unk_178 = 0;
    lbl_1_bss_120C[0].unk_2 = 0;
    lbl_1_bss_120C[1].unk_2 = 0;
    fn_1_8F4C(0);
    fn_1_8F4C(1);
    fn_1_A758();
    fn_1_90C4();
    fn_1_9B8C();
    fn_1_CFB8();
    fn_1_14420();
    fn_1_12E04();
    fn_1_78DC();
    lbl_1_bss_1894[6] = 4;
    return result;
}

s16 fn_1_6ECC(void) {
    s16 result = 0;

    result = fn_1_13430();
    fn_1_8F4C(0);
    fn_1_8F4C(1);
    fn_1_A758();
    fn_1_90C4();
    fn_1_9B8C();
    fn_1_CFB8();
    fn_1_14420();
    fn_1_12E04();
    fn_1_78DC();
    lbl_1_bss_1894[6] = 5;
    return result;
}

s16 fn_1_6F3C(void) {
    G555_EL *p;
    s16 result = 0;

    if (lbl_1_bss_3030->unk_178 == 0) {
        if (lbl_1_bss_1894[0x14] != 0) {
            p = &lbl_1_bss_3030->el[lbl_1_bss_1894[0x14] & 1];
            p->unk_0 = 1;
            p->unk_4 = 1.0f;
            p->unk_C = 0.0f;
            p->unk_18 = 0;
            p->unk_20 = 0x40000001;
        }
        lbl_1_bss_3030->unk_178 = 1;
    } else {
        if (lbl_1_bss_1894[0x14] != 0) {
            if (lbl_1_bss_1894[0x16] == 0) {
                CharFXPlay(lbl_1_bss_3030->win[lbl_1_bss_1894[0x14] & 1].unk_2, 0x1FA);
                lbl_1_bss_1894[0x16] = 1;
            }
            p = &lbl_1_bss_3030->el[lbl_1_bss_1894[0x14] & 1];
            if (p->unk_C >= 38.0f) {
                p->unk_C = 10.0f;
            }
        }
    }

    if (lbl_1_bss_1894[0x14] == 0) {
        fn_1_119EC();
    }
    if (lbl_1_bss_1894[0x14] != 0) {
        fn_1_103D8(lbl_1_bss_1894[0x14] >> 1);
    }
    if (lbl_1_bss_1894[0x14] != 0) {
        fn_1_12CAC(lbl_1_bss_1894[0x14] & 1);
    }
    if (lbl_1_bss_1894[0x20] != 0 && lbl_1_bss_1894[0x14] != 0) {
        fn_1_11DFC(lbl_1_bss_1894[0x14] & 1);
    }

    fn_1_8F4C(0);
    fn_1_8F4C(1);
    fn_1_A758();
    fn_1_CFB8();
    fn_1_12E04();
    fn_1_78DC();
    lbl_1_bss_1894[6] = 6;
    return result;
}

s32 fn_1_71B4(void) {
    G555_EL *p;
    s32 result = 1;

    if (lbl_1_bss_1894[0x14] != 0) {
        p = &lbl_1_bss_3030->el[lbl_1_bss_1894[0x14] & 1];
        if (p->unk_C >= 38.0f) {
            p->unk_C = 10.0f;
        }
    }
    if (lbl_1_bss_3030->unk_178 == 1) {
        fn_1_2E60(0xB, 100);
        lbl_1_bss_3030->unk_178 = 2;
    }
    if (lbl_1_bss_1894[0x14] == 0) {
        fn_1_119EC();
    }
    if (lbl_1_bss_1894[0x14] != 0) {
        fn_1_12CAC(lbl_1_bss_1894[0x14] & 1);
    }
    if (lbl_1_bss_1894[0x14] != 0) {
        fn_1_103D8(lbl_1_bss_1894[0x14] >> 1);
    }
    if (lbl_1_bss_1894[0x20] != 0 && lbl_1_bss_1894[0x14] != 0) {
        fn_1_11DFC(lbl_1_bss_1894[0x14] & 1);
    }
    fn_1_8F4C(0);
    fn_1_8F4C(1);
    fn_1_A758();
    fn_1_CFB8();
    fn_1_78DC();
    lbl_1_bss_1894[6] = 7;
    return result;
}

s32 fn_1_7364(void) {
    s32 ret = 1;

    fn_1_2FF0();
    lbl_1_bss_1894[6] = 8;
    return ret;
}

void fn_1_7840(void) {
    if (lbl_1_bss_1894[0xC1] != 0) {
        if (lbl_1_bss_1894[0xC0] != 0) {
            Hu3DFogSet(*(f32 *)&lbl_1_bss_1894[0xC2], *(f32 *)&lbl_1_bss_1894[0xC4],
                BSS1894_BYTE(0x18C), BSS1894_BYTE(0x18D), BSS1894_BYTE(0x18E));
        } else {
            Hu3DFogClear();
        }
    }
    lbl_1_bss_1894[0xC1] = 0;
}

s32 fn_1_8F4C(s16 arg0) {
    s32 result = 0;
    s32 ival;
    f32 scale;

    if (lbl_1_bss_1894[0x14] != 0 || lbl_1_bss_1894[0x15] != 0) {
        if (lbl_1_bss_120C[arg0].unk_E == 0) {
            memset(&lbl_1_bss_3030->win[arg0].unk_8, 0, 0x14);
        }
    }
    fn_1_7D88(arg0);
    fn_1_8928(arg0);
    if (lbl_1_bss_120C[arg0].unk_14 > 0) {
        if (lbl_1_bss_1894[6] <= 3 && lbl_1_bss_1894[0x1B] == 0) {
            scale = lbl_1_rodata_100 * lbl_1_bss_3030->unk_58[arg0 + 0x21].v[1];
            ival = (s32)(lbl_1_rodata_104 + scale);
            fn_1_2CB0((s16)(arg0 + 2), ival);
        }
    }
    return result;
}

void fn_1_12CAC(s16 arg0)
{
    switch (*(s16 *)((u8 *)lbl_1_bss_1894 + 0x160)) {
    case 0:
        *(s16 *)((u8 *)lbl_1_bss_1894 + 0x160) = *(s16 *)((u8 *)lbl_1_bss_1894 + 0x160) + 1;
    case 1:
        *(f32 *)((u8 *)lbl_1_bss_1894 + 0x164) = *(f32 *)((u8 *)lbl_1_bss_1894 + 0x164) + *(volatile f32 *)&lbl_1_rodata_D8;
        *(f32 *)((u8 *)lbl_1_bss_3030->unk_58 + ((s16)(arg0 + 0x21)) * 0x1C + 8) =
            (f32)((f64)((f32 *)lbl_1_bss_1894)[0x56] + *(volatile f64 *)&lbl_1_rodata_E0 * sin(*(volatile f64 *)&lbl_1_rodata_E8 * ((f32 *)lbl_1_bss_1894)[0x59] / *(volatile f64 *)&lbl_1_rodata_F0));
        *(G555_VEC *)((u8 *)&lbl_1_bss_3030->unk_58[(s16)(arg0 + 0xA9)] + 4) = *(G555_VEC *)((u8 *)&lbl_1_bss_3030->unk_58[(s16)(arg0 + 0x21)] + 4);
        break;
    }
}

s32 fn_1_1A6F4(s32 arg0)
{
    s32 result;
    s32 i;

    result = 0;
    for (i = 0; i < 0x32; i++) {
        G555_VEC v = *(G555_VEC *)((u8 *)&lbl_1_bss_3030->unk_58[(s16)i + 0x23] + 4);

        if (((s16 (*)[16])lbl_1_bss_B88)[i][0] != 0
         && v.z < *(volatile f32 *)&lbl_1_rodata_84
         && v.z > *(volatile f32 *)&lbl_1_rodata_454
         && v.x <= *(volatile f32 *)&lbl_1_rodata_378
         && v.x >= *(volatile f32 *)&lbl_1_rodata_37C
         && *(volatile f32 *)&lbl_1_rodata_88 + v.y <= *(volatile f32 *)&lbl_1_rodata_388) {
            *(G555_VEC *)((u8 *)&lbl_1_bss_120C[arg0] + 0x308) = v;
            result = 1;
            break;
        }
    }
    return result;
}

s32 fn_1_1A880(s32 arg0, s32 arg1)
{
    s32 result;
    s16 idx;

    result = 0;
    idx = (s16)(arg0 + 0xA9);
    if ((s16)arg1 == 0) {
        if (lbl_1_bss_3030->unk_58[idx].v[1] <= *(volatile f32 *)&lbl_1_rodata_188
         && lbl_1_bss_3030->unk_58[idx].v[1] >= *(volatile f32 *)&lbl_1_rodata_78
         && lbl_1_bss_3030->unk_58[idx].v[2] <= *(volatile f32 *)&lbl_1_rodata_188
         && lbl_1_bss_3030->unk_58[idx].v[2] >= *(volatile f32 *)&lbl_1_rodata_338) {
            result = 1;
        }
    } else {
        result = 1;
    }
    return result;
}
s32 fn_1_FB2C(void) {
    f32 *p = (f32 *)((u8 *)lbl_1_bss_3030->unk_58 + 0xA68);
    s32 result = 0;

    switch (lbl_1_bss_11E4.unk_0) {
    case 0:
        lbl_1_bss_11E4.unk_10 = lbl_1_rodata_D8;
        lbl_1_bss_11E4.unk_14 = lbl_1_rodata_27C;
        lbl_1_bss_11E4.unk_1C = lbl_1_rodata_280;
        lbl_1_bss_11E4.unk_20 = lbl_1_rodata_284;
        Hu3DMotionSpeedSet(G555_58_S16(0x39C), lbl_1_rodata_268);
        Hu3DMotionSpeedSet(G555_58_S16(0x3B8), lbl_1_rodata_268);
        lbl_1_bss_11E4.unk_0++;
        /* fall through */
    case 1:
        p[0] = p[0] + lbl_1_bss_11E4.unk_10;
        p[1] = p[1] + lbl_1_bss_11E4.unk_14;
        lbl_1_bss_11E4.unk_10 += lbl_1_bss_11E4.unk_1C;
        lbl_1_bss_11E4.unk_14 += lbl_1_bss_11E4.unk_20;
        if (p[0] < lbl_1_rodata_288) {
            Hu3DModelHookReset(G555_58_S16(0xA64));
            Hu3DModelAttrSet(G555_58_S16(0xA48), 1);
            Hu3DModelAttrSet(G555_58_S16(0xA64), 1);
            lbl_1_bss_11E4.unk_0++;
        }
        break;

    case 2:
        result = 1;
        break;
    }

    return result;
}