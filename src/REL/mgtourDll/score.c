#include "REL/mgtourDll.h"

typedef struct MGTVec2_s {
    f32 x;
    f32 y;
} MGTVec2;

typedef struct MGTScorePt_s {
    f32 x;
    f32 y;
    f32 frames;
} MGTSCOREPT;

typedef struct MGTScorePath_s {
    s32 num;
    MGTSCOREPT pt[3];
} MGTSCOREPATH;

typedef struct MGTScoreWork_s {
    /* 0x00 */ s16 grpMain;
    /* 0x02 */ u8 pad_02[0xA];
    /* 0x0C */ s16 mode;
    /* 0x0E */ u8 pad_0E[2];
    /* 0x10 */ s16 grpPlate;
    /* 0x12 */ s16 grpPlateBg;
    /* 0x14 */ s32 sel[4];
    /* 0x24 */ s16 grpFrame;
    /* 0x26 */ s16 grpRing;
    /* 0x28 */ s16 *mdl;
    /* 0x2C */ s16 score[4];
    /* 0x34 */ ANIMDATA *anim[4];
    /* 0x44 */ MGT_PLAYER *player;
} MGTSCOREWORK;

f32 lbl_1_data_3F0[4] = { 288.0f, 800.0f, 288.0f, 240.0f };
f32 lbl_1_data_400[4] = { 288.0f, 800.0f, 288.0f, 150.0f };
f32 lbl_1_data_410[4] = { 288.0f, 800.0f, 288.0f, 240.0f };
s32 lbl_1_data_420[12] = {
    0x9A0012, 0x9A0013, 0x9A0014, 0x9A0015, 0x9A0016, 0x9A0017,
    0x9A0018, 0x9A0019, 0x9A001A, 0x9A001B, 0x9A001C, 0x9A001D,
};
MGTVec2 lbl_1_data_450 = { 0.0f, -20.0f };
MGTVec2 lbl_1_data_458[12] = {
    { -128.0f, -45.0f }, { -63.0f, -45.0f }, { -128.0f, 75.0f }, { -63.0f, 75.0f },
    { 126.0f, -45.0f }, { 189.0f, -45.0f }, { 126.0f, 75.0f }, { 189.0f, 75.0f },
    { -61.0f, 109.0f }, { 65.0f, 109.0f }, { -61.0f, -76.0f }, { 62.0f, -76.0f },
};
u8 lbl_1_data_4B8[12][4] = { { 0x2A, 0x94, 0x00, 0x00 }, { 0xAA, 0x0C, 0x20, 0x00 } };
s32 lbl_1_data_4E8[12] = { 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1 };
char *lbl_1_data_52C[15] = {
    "root_head", "root_head", "ske_head", "ske_head", "ske_head", "ske_head", "ske_head", "ske_head",
    "ske_head", "ske_head", "ske_head", "ske_head", "ske_head", "ske_head", "ske_head",
};
Vec lbl_1_data_568[13] = {
    { 0.0f, 110.0f, 0.0f },
    { 0.0f, 125.0f, 0.0f },
    { 0.0f, 140.0f, 0.0f },
    { 0.0f, 120.0f, 0.0f },
    { 0.0f, 100.0f, 0.0f },
    { 0.0f, 140.0f, 0.0f },
    { 0.0f, 160.0f, 0.0f },
    { 0.0f, 80.0f, 0.0f },
    { 0.0f, 90.0f, 0.0f },
    { 0.0f, 80.0f, 0.0f },
    { 0.0f, 80.0f, 0.0f },
    { 0.0f, 80.0f, 0.0f },
    { 0.0f, 80.0f, 0.0f }
};
Vec lbl_1_data_604[13] = {
    { -15.0f, 80.0f, 0.0f },
    { 0.0f, 75.0f, 0.0f },
    { 0.0f, 90.0f, 0.0f },
    { 7.0f, 72.0f, 0.0f },
    { 0.0f, 80.0f, 0.0f },
    { 0.0f, 90.0f, 0.0f },
    { -25.0f, 170.0f, 0.0f },
    { 4.0f, 70.0f, 0.0f },
    { 0.0f, 55.0f, 0.0f },
    { 60.0f, 45.0f, 0.0f },
    { 0.0f, 80.0f, 0.0f },
    { 0.0f, 80.0f, 0.0f },
    { 0.0f, 80.0f, 0.0f }
};
f32 lbl_1_data_6A0[130] = {
    22.0f, 97.0f, 0.0f, -15.0f, 120.0f, 0.0f,
    5.0f, 145.0f, 0.0f, 8.0f, 120.0f, 0.0f,
    3.0f, 105.0f, 0.0f, -4.0f, 140.0f, 0.0f,
    7.0f, 160.0f, 0.0f, 12.0f, 75.0f, 0.0f,
    0.0f, 77.0f, 0.0f, 8.0f, 80.0f, 0.0f,
    0.0f, 80.0f, 0.0f, 0.0f, 80.0f, 0.0f,
    0.0f, 80.0f, 0.0f, -10.0f, 5.0f, 0.0f,
    10.0f, 0.0f, -10.0f, 5.0f, -15.0f, 0.0f,
    -20.0f, 0.0f, 10.0f, -5.0f, -25.0f, 5.0f,
    -15.0f, -5.0f, 0.0f, 15.0f, 10.0f, 0.0f,
    -5.0f, 0.0f, -5.0f, 0.0f, -5.0f, 15.0f,
    25.0f, 15.0f, 30.0f, 30.0f, 10.0f, 20.0f,
    10.0f, 0.0f, 10.0f, 10.0f, 10.0f, 10.0f,
    0.0f, -5.0f, 0.0f, -20.0f, 0.0f, -10.0f,
    -10.0f, -25.0f, 0.0f, -20.0f, 0.0f, 0.0f,
    0.0f, -20.0f, 5.0f, -20.0f, 0.0f, 0.0f,
    0.0f, -15.0f, 0.0f, 10.0f, 0.0f, 10.0f,
    0.0f, 10.0f, -10.0f, 5.0f, 0.0f, 10.0f,
    0.0f, -10.0f, 5.0f, -15.0f, 0.0f, -20.0f,
    0.0f, 10.0f, -5.0f, -25.0f, 5.0f, -15.0f,
    -5.0f, 0.0f, 15.0f, 10.0f, 0.0f, -5.0f,
    0.0f, -5.0f, 0.0f, -5.0f
};
MGTVec2 lbl_1_data_8A8[14] = {
    { 99.0f, 259.0f }, { 226.0f, 259.0f }, { 352.0f, 259.0f }, { 479.0f, 259.0f },
    { 163.0f, 163.0f }, { 163.0f, 348.0f }, { 415.0f, 163.0f }, { 415.0f, 348.0f },
    { 289.0f, 600.0f }, { 289.0f, 132.0f }, { 188.0f, 140.0f }, { 388.0f, 140.0f },
    { -200.0f, 550.0f }, { 776.0f, 550.0f },
};
MGTVec2 lbl_1_data_918 = { 288.0f, 125.0f };
MGTSCOREPATH lbl_1_data_920[14] = {
    { 1, { { 0.0f, 0.0f, 20.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, -1.0f } } },
    { 1, { { 0.0f, 0.0f, 20.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, -1.0f } } },
    { 1, { { 0.0f, 0.0f, 20.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, -1.0f } } },
    { 1, { { 0.0f, 0.0f, 20.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, -1.0f } } },
    { 3, { { 0.0f, 0.0f, 20.0f }, { 0.0f, 0.0f, 20.0f }, { 0.0f, 0.0f, 20.0f } } },
    { 3, { { 0.0f, 0.0f, 20.0f }, { 0.0f, 0.0f, 20.0f }, { 0.0f, 0.0f, 20.0f } } },
    { 3, { { 0.0f, 0.0f, 20.0f }, { 0.0f, 0.0f, 20.0f }, { 0.0f, 0.0f, 20.0f } } },
    { 3, { { 0.0f, 0.0f, 20.0f }, { 0.0f, 0.0f, 20.0f }, { 0.0f, 0.0f, 20.0f } } },
    { 3, { { 0.0f, 0.0f, 40.0f }, { 0.0f, 0.0f, 40.0f }, { 0.0f, 0.0f, 15.0f } } },
    { 2, { { 0.0f, 0.0f, 40.0f }, { 0.0f, 0.0f, 20.0f }, { 0.0f, 0.0f, -1.0f } } },
    { 1, { { 0.0f, 0.0f, 60.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, -1.0f } } },
    { 1, { { 0.0f, 0.0f, 60.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, -1.0f } } },
    { 1, { { 0.0f, 0.0f, 120.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, -1.0f } } },
    { 1, { { 0.0f, 0.0f, 120.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, -1.0f } } },
};
MGTVec2 lbl_1_data_B50 = { 0.0f, -160.0f };
MGTVec2 lbl_1_data_B58[4] = {
    { -178.0f, 0.0f }, { -59.333332f, 0.0f }, { 59.333332f, 0.0f }, { 178.0f, 0.0f },
};
MGTVec2 lbl_1_data_B78[4] = {
    { -178.0f, 45.0f }, { -59.333332f, 45.0f }, { 59.333332f, 45.0f }, { 178.0f, 45.0f },
};
MGTVec2 lbl_1_data_B98[4] = {
    { -178.0f, -30.0f }, { -59.333332f, -30.0f }, { 59.333332f, -30.0f }, { 178.0f, -30.0f },
};
MGTVec2 lbl_1_data_BB8 = { -178.0f, -90.0f };

Vec lbl_1_bss_176E0[4];
Vec lbl_1_bss_17710[4];
Vec lbl_1_bss_17740[4];
Vec lbl_1_bss_17770[4];
MGTSCOREWORK lbl_1_bss_177A0;

const Vec lbl_1_rodata_1B0 = { 0.0f, 0.0f, -1.0f };

#define SCORE_PLAYER(i) (&lbl_1_bss_177A0.player[lbl_1_bss_177A0.score[i]])

extern void fn_1_7AC(u32 mask);
extern u32 fn_1_7E4(u32 mask);
extern s16 fn_1_938(s32 arg);
extern s32 fn_1_96C(void);
extern s8 fn_1_99C(void);
extern s32 fn_1_9D4(s32 arg);

extern void fn_1_D658(void);
extern void fn_1_DEFC(void);
extern void fn_1_F708(void);
extern void fn_1_F9D0(void);

void fn_1_B8C4(s16 layerNo)
{
    s32 i;

    if (Hu3DCameraBit == 2 && lbl_1_bss_177A0.player) {
        for (i = 0; i < 4; i++) {
            Hu3DFbCopyExec((s16)(s32)(lbl_1_bss_176E0[i].x - 40.0f), (s16)(s32)(lbl_1_bss_176E0[i].y - 40.0f), 0x50, 0x50, 5, 0,
                lbl_1_bss_177A0.anim[i]->bmp->data);
        }
        Hu3DZClear();
    }
}

void fn_1_B9C0(void)
{
    Vec pos;
    s32 i;
    s32 t;
    MGT_PLAYER *player;
    s32 end;
    f32 ratio;

    end = 0x78;
    for (t = 0; t <= end; t++) {
        ratio = (f32)t / (f32)end;
        for (i = 0; i < 4; i++) {
            player = SCORE_PLAYER(i);
            PSVECSubtract(&lbl_1_bss_17710[i], &lbl_1_bss_17740[i], &pos);
            PSVECScale(&pos, &pos, ratio);
            PSVECSubtract(&lbl_1_bss_17770[i], &pos, &pos);
            Hu3DModelPosSetV(player->modelId, &pos);
        }
        HuPrcVSleep();
    }
    HuPrcEnd();
}

void fn_1_BB14(s8 *str, s32 len)
{
    s8 tmp[0x10];
    s32 i;

    i = len;
    while (i--) {
        tmp[i] = str[i];
    }
    for (i = 1; i < len; i++) {
        str[i - 1] = tmp[i];
    }
    str[len - 1] = tmp[0];
}

void fn_1_BB78(s32 *order)
{
    s32 idx[4];

    switch (fn_1_9D4(0)) {
        case 0:
            order[0] = 4;
            order[1] = 5;
            idx[2] = 0;
            idx[0] = 1;
            break;
        case 1:
            order[0] = 5;
            order[1] = 4;
            idx[2] = 1;
            idx[0] = 0;
            break;
        case -1:
            order[0] = 0;
            order[1] = 1;
            order[2] = 2;
            order[3] = 3;
            break;
    }
    switch (fn_1_9D4(1)) {
        case 0:
            order[2] = 6;
            order[3] = 7;
            idx[3] = 2;
            idx[1] = 3;
            break;
        case 1:
            order[2] = 7;
            order[3] = 6;
            idx[3] = 3;
            idx[1] = 2;
            break;
        case -1:
            order[2] = 2;
            order[3] = 3;
            break;
    }
    switch (fn_1_9D4(2)) {
        case 0:
            order[idx[1]] = 8;
            break;
        case 1:
            order[idx[0]] = 8;
            break;
    }
    switch (fn_1_9D4(3)) {
        case 0:
            order[idx[2]] = 9;
            break;
        case 1:
            order[idx[3]] = 9;
            break;
    }
}

void fn_1_BD40(void)
{
    MGTSCOREPATH *path;
    s32 i;
    s32 j;

    i = 0xE;
    while (i--) {
        path = &lbl_1_data_920[i];
        j = 3;
        while (j--) {
            path->pt[j].x += lbl_1_data_450.x;
            path->pt[j].y += lbl_1_data_450.y;
        }
    }
    i = 0xE;
    while (i--) {
        lbl_1_data_8A8[i].x += lbl_1_data_450.x;
        lbl_1_data_8A8[i].y += lbl_1_data_450.y;
    }
    i = 0xC;
    while (i--) {
        lbl_1_data_458[i].x += lbl_1_data_450.x;
        lbl_1_data_458[i].y += lbl_1_data_450.y;
    }
    lbl_1_data_918.x += lbl_1_data_450.x;
    lbl_1_data_918.y += lbl_1_data_450.y;
    lbl_1_data_400[2] += lbl_1_data_450.x;
    lbl_1_data_400[3] += lbl_1_data_450.y;
}

void fn_1_BF2C(void)
{
    MGTSCOREPATH *p;

    p = &lbl_1_data_920[0];
    p->pt[0].x = lbl_1_data_8A8[0].x;
    p->pt[0].y = 196.0f;
    p = &lbl_1_data_920[1];
    p->pt[0].x = lbl_1_data_8A8[1].x;
    p->pt[0].y = 196.0f;
    p = &lbl_1_data_920[2];
    p->pt[0].x = lbl_1_data_8A8[2].x;
    p->pt[0].y = 196.0f;
    p = &lbl_1_data_920[3];
    p->pt[0].x = lbl_1_data_8A8[3].x;
    p->pt[0].y = 196.0f;
    p = &lbl_1_data_920[4];
    p->pt[0].x = lbl_1_data_8A8[0].x;
    p->pt[0].y = 196.0f;
    p->pt[1].x = lbl_1_data_8A8[4].x;
    p->pt[1].y = 196.0f;
    p->pt[2].x = lbl_1_data_8A8[4].x;
    p->pt[2].y = lbl_1_data_8A8[4].y;
    p = &lbl_1_data_920[5];
    p->pt[0].x = lbl_1_data_8A8[0].x;
    p->pt[0].y = 315.0f;
    p->pt[1].x = lbl_1_data_8A8[5].x;
    p->pt[1].y = 315.0f;
    p->pt[2].x = lbl_1_data_8A8[5].x;
    p->pt[2].y = lbl_1_data_8A8[5].y;
    p = &lbl_1_data_920[6];
    p->pt[0].x = lbl_1_data_8A8[2].x;
    p->pt[0].y = 196.0f;
    p->pt[1].x = lbl_1_data_8A8[6].x;
    p->pt[1].y = 196.0f;
    p->pt[2].x = lbl_1_data_8A8[6].x;
    p->pt[2].y = lbl_1_data_8A8[6].y;
    p = &lbl_1_data_920[7];
    p->pt[0].x = lbl_1_data_8A8[2].x;
    p->pt[0].y = 315.0f;
    p->pt[1].x = lbl_1_data_8A8[7].x;
    p->pt[1].y = 315.0f;
    p->pt[2].x = lbl_1_data_8A8[7].x;
    p->pt[2].y = lbl_1_data_8A8[7].y;
    p = &lbl_1_data_920[8];
    p->pt[0].x = lbl_1_data_8A8[8].x;
    p->pt[0].y = lbl_1_data_8A8[7].y;
    p->pt[1].x = lbl_1_data_8A8[8].x;
    p->pt[1].y = lbl_1_data_8A8[7].y;
    p->pt[2].x = lbl_1_data_8A8[8].x;
    p->pt[2].y = lbl_1_data_8A8[8].y;
    p = &lbl_1_data_920[9];
    p->pt[0].x = lbl_1_data_8A8[9].x;
    p->pt[0].y = lbl_1_data_8A8[4].y;
    p->pt[1].x = lbl_1_data_8A8[9].x;
    p->pt[1].y = lbl_1_data_8A8[9].y;
    p = &lbl_1_data_920[10];
    *(MGTVec2 *)&p->pt[0] = lbl_1_data_8A8[10];
    p = &lbl_1_data_920[11];
    *(MGTVec2 *)&p->pt[0] = lbl_1_data_8A8[11];
    p = &lbl_1_data_920[12];
    *(MGTVec2 *)&p->pt[0] = lbl_1_data_8A8[12];
    p = &lbl_1_data_920[13];
    *(MGTVec2 *)&p->pt[0] = lbl_1_data_8A8[13];
}

void fn_1_C2E4(void)
{
    Vec in;
    Vec out;
    s32 order[4];
    Vec dir;
    s32 mdl;
    s32 i;

    fn_1_BB78(order);
    i = 4;
    while (i--) {
        mdl = lbl_1_bss_177A0.mdl[i + 0xC];
        in.x = lbl_1_data_8A8[order[i]].x;
        in.y = lbl_1_data_8A8[order[i]].y;
        in.z = 1000.0f;
        Hu3D2Dto3D(&in, 4, &out);
        Hu3DModelPosSetV(mdl, &out);
    }
    dir = lbl_1_rodata_1B0;
    mdl = lbl_1_bss_177A0.mdl[4];
    in.x = lbl_1_data_918.x;
    in.y = lbl_1_data_918.y;
    in.z = 1200.0f;
    Hu3D2Dto3D(&in, 4, &out);
    Hu3DModelPosSetV(mdl, &out);
}

void fn_1_C434(s16 *mdl)
{
    s32 k;
    s32 i;
    s32 j;
    MGTSCOREPATH *path;

    lbl_1_bss_177A0.mdl = mdl;
    lbl_1_bss_177A0.grpMain = -1;
    lbl_1_bss_177A0.mode = 0;
    lbl_1_bss_177A0.grpPlate = -1;
    lbl_1_bss_177A0.grpPlateBg = -1;
    lbl_1_bss_177A0.grpRing = -1;
    i = 4;
    while (i--) {
        lbl_1_bss_177A0.anim[i] = HuSprAnimMake(0x50, 0x50, 2);
        lbl_1_bss_177A0.anim[i]->bmp->data
            = HuMemDirectMallocNum(HEAP_MODEL, lbl_1_bss_177A0.anim[i]->bmp->dataSize, HU_MEMNUM_OVL);
        memset(lbl_1_bss_177A0.anim[i]->bmp->data, 0, lbl_1_bss_177A0.anim[i]->bmp->dataSize);
    }
    Hu3DLayerHookSet(1, fn_1_B8C4);
    k = 0xE;
    while (k--) {
        path = &lbl_1_data_920[k];
        j = 3;
        while (j--) {
            path->pt[j].x += lbl_1_data_450.x;
            path->pt[j].y += lbl_1_data_450.y;
        }
    }
    k = 0xE;
    while (k--) {
        lbl_1_data_8A8[k].x += lbl_1_data_450.x;
        lbl_1_data_8A8[k].y += lbl_1_data_450.y;
    }
    k = 0xC;
    while (k--) {
        lbl_1_data_458[k].x += lbl_1_data_450.x;
        lbl_1_data_458[k].y += lbl_1_data_450.y;
    }
    lbl_1_data_918.x += lbl_1_data_450.x;
    lbl_1_data_918.y += lbl_1_data_450.y;
    lbl_1_data_400[2] += lbl_1_data_450.x;
    lbl_1_data_400[3] += lbl_1_data_450.y;
    fn_1_BF2C();
}

void fn_1_C76C(void)
{
    s32 i;

    i = 4;
    while (i--) {
        HuSprAnimKill(lbl_1_bss_177A0.anim[i]);
    }
}

void fn_1_C7BC(s32 *param)
{
    s32 i;

    i = 4;
    while (i--) {
        lbl_1_bss_177A0.score[i] = (s16)param[i];
    }
}

void fn_1_C804(void)
{
    MGTVec2 pos;
    ANIMDATA *anim;
    s16 grp;
    s32 member;
    s32 i;

    if (lbl_1_bss_177A0.grpMain < 0) {
        grp = HuSprGrpCreate(0xE);
        lbl_1_bss_177A0.grpMain = grp;
        anim = HuSprAnimRead(HuDataSelHeapReadNum(0x9A000E, HU_MEMNUM_OVL, HEAP_MODEL));
        HuSprGrpMemberSet(grp, 0, HuSprCreate(anim, 0x64, 0));
        HuSprScaleSet(grp, 0, 72.0f, 60.0f);
        HuSprTPLvlSet(grp, 0, 0.5f);
        anim = HuSprAnimRead(HuDataSelHeapReadNum(0x9A0011, HU_MEMNUM_OVL, HEAP_MODEL));
        HuSprGrpMemberSet(grp, 1, HuSprCreate(anim, 0x46, 0));
        HuSprPosSet(grp, 1, lbl_1_data_450.x, lbl_1_data_450.y);
        i = 0xC;
        while (i--) {
            pos = lbl_1_data_450;
            member = i + 2;
            anim = HuSprAnimRead(HuDataSelHeapReadNum(lbl_1_data_420[i], HU_MEMNUM_OVL, HEAP_MODEL));
            HuSprGrpMemberSet(grp, member, HuSprCreate(anim, 0x32, 0));
            HuSprPosSet(grp, member, pos.x, pos.y);
            HuSprColorSet(grp, member, lbl_1_data_4B8[lbl_1_data_4E8[i]][0], lbl_1_data_4B8[lbl_1_data_4E8[i]][1],
                lbl_1_data_4B8[lbl_1_data_4E8[i]][2]);
        }
        HuSprGrpDrawNoSet(grp, 0x40);
        HuSprGrpPosSet(grp, lbl_1_data_3F0[0], lbl_1_data_3F0[1]);
        grp = HuSprGrpCreate(1);
        lbl_1_bss_177A0.grpFrame = grp;
        switch (GwCommon.languageNo) {
            case 0:
                anim = HuSprAnimRead(HuDataSelHeapReadNum(0x9A0010, HU_MEMNUM_OVL, HEAP_MODEL));
                break;
            case 1:
                anim = HuSprAnimRead(HuDataSelHeapReadNum(0x9A0024, HU_MEMNUM_OVL, HEAP_MODEL));
                break;
#ifdef TARGET_PC
            default:
                anim = HuSprAnimRead(HuDataSelHeapReadNum(0x9A0010, HU_MEMNUM_OVL, HEAP_MODEL));
                break;
#endif
        }
        HuSprGrpMemberSet(grp, 0, HuSprCreate(anim, 0x64, 0));
        HuSprGrpDrawNoSet(grp, 0x40);
        HuSprGrpPosSet(grp, lbl_1_data_400[0], lbl_1_data_400[1]);
        HuSprExecLayerSet(0x40, 3);
    }
    fn_1_D658();
    if (fn_1_7E4(0x80000000) == 0) {
        fn_1_7AC(0x80000000);
        HuPrcChildCreate(fn_1_DEFC, 0x64, 0x1000, 0, HuPrcCurrentGet());
    }
}

void fn_1_CB8C(s32 *param)
{
    Vec in;
    s32 i;
    s16 grp;
    ANIMDATA *anim;
    s32 member;
    s32 mdl;

    if (lbl_1_bss_177A0.grpPlate < 0) {
        grp = HuSprGrpCreate(0xB);
        lbl_1_bss_177A0.grpPlateBg = grp;
        anim = HuSprAnimRead(HuDataSelHeapReadNum(0x9A000E, HU_MEMNUM_OVL, HEAP_MODEL));
        HuSprGrpMemberSet(grp, 0, HuSprCreate(anim, 0x64, 0));
        HuSprScaleSet(grp, 0, 72.0f, 60.0f);
        HuSprTPLvlSet(grp, 0, 0.7f);
        anim = HuSprAnimRead(HuDataSelHeapReadNum(0x9A0020, HU_MEMNUM_OVL, HEAP_MODEL));
        HuSprGrpMemberSet(grp, 1, HuSprCreate(anim, 0x28, 0));
        HuSprPosSet(grp, 1, lbl_1_data_B50.x, lbl_1_data_B50.y);
        HuSprGrpDrawNoSet(grp, 0x40);
        HuSprGrpPosSet(grp, lbl_1_data_410[0], lbl_1_data_410[1]);
        grp = HuSprGrpCreate(0xB);
        lbl_1_bss_177A0.grpPlate = grp;
        i = 4;
        while (i--) {
            member = i + 2;
            anim = HuSprAnimRead(HuDataSelHeapReadNum(0x9A0021, HU_MEMNUM_OVL, HEAP_MODEL));
            HuSprGrpMemberSet(grp, member, HuSprCreate(anim, 0x46, 0));
            HuSprPosSet(grp, member, lbl_1_data_B58[i].x, lbl_1_data_B58[i].y);
        }
        i = 4;
        while (i--) {
            member = i + 6;
            anim = HuSprAnimRead(HuDataSelHeapReadNum(0x9A001E, HU_MEMNUM_OVL, HEAP_MODEL));
            HuSprGrpMemberSet(grp, member, HuSprCreate(anim, 0x32, i));
            HuSprPosSet(grp, member, lbl_1_data_B78[i].x, lbl_1_data_B78[i].y);
        }
        anim = HuSprAnimRead(HuDataSelHeapReadNum(0x9A001F, HU_MEMNUM_OVL, HEAP_MODEL));
        HuSprGrpMemberSet(grp, 0xA, HuSprCreate(anim, 0x28, 0));
        HuSprPosSet(grp, 0xA, lbl_1_data_BB8.x, lbl_1_data_BB8.y);
        HuSprGrpDrawNoSet(grp, 0x40);
        HuSprGrpPosSet(grp, lbl_1_data_410[0], lbl_1_data_410[1]);
        HuSprExecLayerSet(0x40, 3);
    }
    i = 4;
    while (i--) {
        lbl_1_bss_177A0.sel[i] = param[i];
        lbl_1_data_B98[i].x += lbl_1_data_410[2];
        lbl_1_data_B98[i].y += lbl_1_data_410[3];
        in.x = lbl_1_data_B98[i].x;
        in.y = lbl_1_data_B98[i].y;
        in.z = 1000.0f;
        Hu3D2Dto3D(&in, 0x10, &in);
        mdl = lbl_1_bss_177A0.mdl[lbl_1_bss_177A0.sel[i] + 0xC];
        Hu3DModelPosSetV(mdl, &in);
    }
    if (fn_1_7E4(0x40000000) == 0) {
        fn_1_7AC(0x40000000);
        HuPrcChildCreate(fn_1_F708, 0x64, 0x1000, 0, HuPrcCurrentGet());
    }
}

void fn_1_CFEC(s32 flag)
{
    Vec dir;
    s32 order[4];
    Vec out;
    Vec in;
    s32 k;
    s32 i;
    s32 mdl2;
    s32 mdl;

    if (flag != 0) {
        fn_1_BB78(order);
        i = 4;
        while (i--) {
            mdl = lbl_1_bss_177A0.mdl[i + 0xC];
            in.x = lbl_1_data_8A8[order[i]].x;
            in.y = lbl_1_data_8A8[order[i]].y;
            in.z = 1000.0f;
            Hu3D2Dto3D(&in, 4, &out);
            Hu3DModelPosSetV(mdl, &out);
        }
        dir = lbl_1_rodata_1B0;
        mdl = lbl_1_bss_177A0.mdl[4];
        in.x = lbl_1_data_918.x;
        in.y = lbl_1_data_918.y;
        in.z = 1200.0f;
        Hu3D2Dto3D(&in, 4, &out);
        Hu3DModelPosSetV(mdl, &out);
        HuSprGrpPosSet(lbl_1_bss_177A0.grpMain, lbl_1_data_3F0[2], lbl_1_data_3F0[3]);
        HuSprGrpPosSet(lbl_1_bss_177A0.grpFrame, lbl_1_data_400[0], lbl_1_data_400[1]);
        k = 4;
        while (k--) {
            mdl2 = lbl_1_bss_177A0.mdl[k + 0xC];
            Hu3DModelAttrReset(mdl2, 1);
            Hu3DModelScaleSet(mdl2, 0.35f, 0.35f, 0.1f);
        }
        return;
    }
    HuSprGrpPosSet(lbl_1_bss_177A0.grpMain, lbl_1_data_3F0[0], lbl_1_data_3F0[1]);
    HuSprGrpPosSet(lbl_1_bss_177A0.grpFrame, lbl_1_data_400[0], lbl_1_data_400[1]);
    k = 4;
    while (k--) {
        mdl2 = lbl_1_bss_177A0.mdl[k + 0xC];
        Hu3DModelAttrSet(mdl2, 1);
    }
}

void fn_1_D29C(s32 flag)
{
    s32 i;
    s32 mdl;

    if (flag != 0) {
        HuSprGrpPosSet(lbl_1_bss_177A0.grpPlate, lbl_1_data_410[2], lbl_1_data_410[3]);
        HuSprGrpPosSet(lbl_1_bss_177A0.grpPlateBg, lbl_1_data_410[2], lbl_1_data_410[3]);
        i = 4;
        while (i--) {
            mdl = lbl_1_bss_177A0.mdl[i + 0xC];
            Hu3DModelAttrReset(mdl, 1);
            Hu3DModelScaleSet(mdl, 0.42f, 0.42f, 0.1f);
        }
        return;
    }
    HuSprGrpPosSet(lbl_1_bss_177A0.grpPlate, lbl_1_data_410[0], lbl_1_data_410[1]);
    HuSprGrpPosSet(lbl_1_bss_177A0.grpPlateBg, lbl_1_data_410[0], lbl_1_data_410[1]);
    i = 4;
    while (i--) {
        mdl = lbl_1_bss_177A0.mdl[i + 0xC];
        Hu3DModelAttrSet(mdl, 1);
    }
}

void fn_1_D418(s32 flag)
{
    Vec dir;
    s32 order[4];
    Vec out;
    Vec in;
    s32 i;
    s32 mdl;

    if (flag != 0) {
        fn_1_BB78(order);
        i = 4;
        while (i--) {
            mdl = lbl_1_bss_177A0.mdl[i + 0xC];
            in.x = lbl_1_data_8A8[order[i]].x;
            in.y = lbl_1_data_8A8[order[i]].y;
            in.z = 1000.0f;
            Hu3D2Dto3D(&in, 4, &out);
            Hu3DModelPosSetV(mdl, &out);
        }
        dir = lbl_1_rodata_1B0;
        mdl = lbl_1_bss_177A0.mdl[4];
        in.x = lbl_1_data_918.x;
        in.y = lbl_1_data_918.y;
        in.z = 1200.0f;
        Hu3D2Dto3D(&in, 4, &out);
        Hu3DModelPosSetV(mdl, &out);
        HuSprGrpPosSet(lbl_1_bss_177A0.grpMain, lbl_1_data_3F0[2], lbl_1_data_3F0[3]);
        HuSprGrpPosSet(lbl_1_bss_177A0.grpFrame, lbl_1_data_400[0], lbl_1_data_400[1]);
        return;
    }
    HuSprGrpPosSet(lbl_1_bss_177A0.grpMain, lbl_1_data_3F0[0], lbl_1_data_3F0[1]);
    HuSprGrpPosSet(lbl_1_bss_177A0.grpFrame, lbl_1_data_400[2], lbl_1_data_400[3]);
}

void fn_1_D61C(s32 val)
{
    lbl_1_bss_177A0.mode = (s16)val;
}

s32 fn_1_D630(void)
{
    return lbl_1_bss_177A0.mode == 1;
}

void fn_1_D658(void)
{
    s32 grp;
    s32 i;
    MGTSCOREWORK *work;

    grp = lbl_1_bss_177A0.grpMain;
    i = 0xC;
    while (i--) {
        HuSprAttrSet(grp, (s16)(i + 2), 4);
    }
    switch (fn_1_9D4(0)) {
        case 0:
            HuSprAttrReset(grp, 2, 4);
            HuSprAttrReset(grp, 5, 4);
            break;
        case 1:
            HuSprAttrReset(grp, 3, 4);
            HuSprAttrReset(grp, 4, 4);
            break;
    }
    switch (fn_1_9D4(1)) {
        case 0:
            HuSprAttrReset(grp, 6, 4);
            HuSprAttrReset(grp, 9, 4);
            break;
        case 1:
            HuSprAttrReset(grp, 7, 4);
            HuSprAttrReset(grp, 8, 4);
            break;
    }
    switch (fn_1_9D4(2)) {
        case 0:
            HuSprAttrReset(grp, 0xB, 4);
            break;
        case 1:
            HuSprAttrReset(grp, 0xA, 4);
            break;
    }
    switch (fn_1_9D4(3)) {
        case 0:
            HuSprAttrReset(grp, 0xC, 4);
            break;
        case 1:
            HuSprAttrReset(grp, 0xD, 4);
            break;
    }
    work = &lbl_1_bss_177A0;
    work->mode = 0;
}

void fn_1_D820(s32 flag, MGT_PLAYER *players)
{
    Vec pos;
    Vec scr;
    Vec dir;
    s32 order[4];
    Vec out;
    Vec in;
    MGT_PLAYER *player;
    s32 i;
    s16 sx;
    s16 sy;
    s32 k;
    s32 mdl;
    s16 am;

#ifdef TARGET_PC
    sx = 0;
    sy = 0;
#endif
    HuPrcVSleep();
    lbl_1_bss_177A0.player = players;
    for (i = 0; i < 4; i++) {
        player = &players[lbl_1_bss_177A0.score[i]];
        switch (player->state) {
            case 0:
                OSReport("stand\n");
                Hu3DModelObjPosGet(player->modelId, "root", &pos);
                PSVECAdd(&lbl_1_data_568[player->charNo], &pos, &pos);
                Hu3D3Dto2D(&pos, 2, &scr);
                sx = (s16)(1.1111111111111112 * scr.x);
                sy = (s16)scr.y;
                break;
            case 3:
                OSReport("lose\n");
                Hu3DModelObjPosGet(player->modelId, "root", &pos);
                PSVECAdd(&lbl_1_data_604[player->charNo], &pos, &pos);
                Hu3D3Dto2D(&pos, 2, &scr);
                sx = (s16)(1.1111111111111112 * scr.x);
                sy = (s16)scr.y;
                break;
            case 2:
                OSReport("win\n");
                Hu3DModelObjPosGet(player->modelId, "root", &pos);
                PSVECAdd(&((Vec *)lbl_1_data_6A0)[player->charNo], &pos, &pos);
                Hu3D3Dto2D(&pos, 2, &scr);
                sx = (s16)(1.1111111111111112 * scr.x);
                sy = (s16)scr.y;
                break;
        }
        sx &= 0xFFFE;
        sy &= 0xFFFE;
        lbl_1_bss_176E0[i].x = (f32)sx;
        lbl_1_bss_176E0[i].y = (f32)sy;
    }
    HuPrcVSleep();
    i = 4;
    while (i--) {
        am = lbl_1_bss_177A0.mdl[i + 0xC];
        Hu3DAnimCreate(lbl_1_bss_177A0.anim[i], am, "S3TC_touei");
    }
    if (flag != 0) {
        fn_1_BB78(order);
        k = 4;
        while (k--) {
            mdl = lbl_1_bss_177A0.mdl[k + 0xC];
            in.x = lbl_1_data_8A8[order[k]].x;
            in.y = lbl_1_data_8A8[order[k]].y;
            in.z = 1000.0f;
            Hu3D2Dto3D(&in, 4, &out);
            Hu3DModelPosSetV(mdl, &out);
        }
        dir = lbl_1_rodata_1B0;
        mdl = lbl_1_bss_177A0.mdl[4];
        in.x = lbl_1_data_918.x;
        in.y = lbl_1_data_918.y;
        in.z = 1200.0f;
        Hu3D2Dto3D(&in, 4, &out);
        Hu3DModelPosSetV(mdl, &out);
        return;
    }
    for (i = 0; i < 4; i++) {
        player = &players[lbl_1_bss_177A0.score[i]];
        Hu3DModelObjPosGet(player->modelId, "root", &pos);
        lbl_1_bss_17770[i] = pos;
        PSVECAdd(&lbl_1_data_568[player->charNo], &pos, &pos);
        Hu3D3Dto2D(&pos, 2, &scr);
        sx = (s16)(1.1111111111111112 * scr.x);
        sy = (s16)scr.y;
        sx &= 0xFFFE;
        sy &= 0xFFFE;
        lbl_1_bss_176E0[i].x = (f32)sx;
        lbl_1_bss_176E0[i].y = (f32)sy;
        lbl_1_bss_17740[i] = lbl_1_data_568[player->charNo];
        switch (player->state) {
            case 0:
                lbl_1_bss_17710[i] = lbl_1_data_568[player->charNo];
                break;
            case 3:
                lbl_1_bss_17710[i] = lbl_1_data_604[player->charNo];
                break;
            case 2:
                lbl_1_bss_17710[i] = ((Vec *)lbl_1_data_6A0)[player->charNo];
                break;
        }
    }
    HuPrcChildCreate(fn_1_B9C0, 0xC8, 0x1000, 0, HuPrcCurrentGet());
}

void fn_1_DEFC(void)
{
}

void fn_1_F708(void)
{
}

void fn_1_F9D0(void)
{
}
