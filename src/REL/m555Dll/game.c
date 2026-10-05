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

extern G555_OBJ *lbl_1_bss_3030;

const f32 lbl_1_rodata_68 = 0.0f;
const f32 lbl_1_rodata_6C = 1.0f;
const f32 lbl_1_rodata_B0 = 38.0f;
const f32 lbl_1_rodata_B4 = 10.0f;
const f32 lbl_1_rodata_100 = 0.05818182f;
const f32 lbl_1_rodata_104 = 64.0f;
const f32 lbl_1_rodata_D8 = 2.5f;
const f32 lbl_1_rodata_268 = 3.0f;
const f32 lbl_1_rodata_27C = -4.0f;
const f32 lbl_1_rodata_280 = -0.15f;
const f32 lbl_1_rodata_284 = 0.1f;
const f32 lbl_1_rodata_288 = -1000.0f;

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