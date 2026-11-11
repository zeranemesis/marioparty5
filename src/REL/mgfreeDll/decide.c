#include "game/animdata.h"
#include "game/audio.h"
#include "game/data.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/mgdata.h"
#include "game/memory.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/process.h"
#include "game/sprite.h"
#include "game/window.h"

#include "REL/mgfreeDll.h"

#include <math.h>
#include <stdio.h>

typedef struct DecideWinParam_s {
    f32 x;
    f32 y;
    f32 w;
    f32 h;
    s32 prio;
    u32 attr;
} DecideWinParam;

static DecideWinParam lbl_1_data_330[26] = {
    { 342.0f, 191.0f, 120.0f, 32.0f, 0x96, 0x800 },
    { 150.0f, 255.0f, 260.0f, 32.0f, 0x96, 0x0 },
    { 416.0f, 255.0f, 260.0f, 32.0f, 0x96, 0x0 },
    { 150.0f, 287.0f, 260.0f, 32.0f, 0x96, 0x0 },
    { 416.0f, 287.0f, 260.0f, 32.0f, 0x96, 0x0 },
    { 150.0f, 319.0f, 260.0f, 32.0f, 0x96, 0x0 },
    { 416.0f, 319.0f, 260.0f, 32.0f, 0x96, 0x0 },
    { 150.0f, 351.0f, 260.0f, 32.0f, 0x96, 0x0 },
    { 416.0f, 351.0f, 260.0f, 32.0f, 0x96, 0x0 },
    { 150.0f, 255.0f, 260.0f, 32.0f, 0xBE, 0x0 },
    { 416.0f, 255.0f, 260.0f, 32.0f, 0xBE, 0x0 },
    { 150.0f, 287.0f, 260.0f, 32.0f, 0xBE, 0x0 },
    { 416.0f, 287.0f, 260.0f, 32.0f, 0xBE, 0x0 },
    { 150.0f, 319.0f, 260.0f, 32.0f, 0xBE, 0x0 },
    { 416.0f, 319.0f, 260.0f, 32.0f, 0xBE, 0x0 },
    { 150.0f, 351.0f, 260.0f, 32.0f, 0xBE, 0x0 },
    { 416.0f, 351.0f, 260.0f, 32.0f, 0xBE, 0x0 },
    { 470.0f, 397.0f, 200.0f, 32.0f, 0xBE, 0x0 },
    { 288.0f, 420.0f, 500.0f, 35.0f, 0x96, 0x800 },
    { 288.0f, 314.0f, 460.0f, 40.0f, 0x6E, 0x800 },
    { 288.0f, 140.0f, 300.0f, 32.0f, 0x5A, 0x800 },
    { 288.0f, 240.0f, 300.0f, 32.0f, 0x5A, 0x800 },
    { 288.0f, 340.0f, 300.0f, 32.0f, 0x5A, 0x800 },
    { 288.0f, 145.0f, 300.0f, 64.0f, 0x5A, 0x800 },
    { 288.0f, 245.0f, 300.0f, 64.0f, 0x5A, 0x800 },
    { 288.0f, 345.0f, 300.0f, 64.0f, 0x5A, 0x800 },
};

static u32 lbl_1_data_5A0[10] = { 0x97002E, 0x97002F, 0x970030, 0x970031, 0x970032, 0x970033, 0x970034, 0x970035, 0x970036, 0x970037 };

static HuVec2f lbl_1_data_5C8[4] = {
    { -35.0f, -41.0f },
    { 35.0f, -41.0f },
    { -35.0f, 41.0f },
    { 35.0f, 41.0f },
};

static f32 lbl_1_data_5E8[4] = { 288.0f, 255.0f, 288.0f, 840.0f };

static f32 lbl_1_data_5F8[4] = { 100.0f, 125.0f, -300.0f, 125.0f };

static f32 lbl_1_data_608[4] = { 288.0f, 240.0f, 288.0f, 2000.0f };

static f32 lbl_1_data_618[4] = { 288.0f, 240.0f, 288.0f, 2000.0f };

static f32 lbl_1_data_628[4] = { 288.0f, 240.0f, 288.0f, 2000.0f };

static HuVec2f lbl_1_data_638[2] = {
    { 0.0f, -7.0f },
    { 0.0f, 151.0f },
};

static HuVec2f lbl_1_data_648[8] = {
    { -135.0f, 5.0f },
    { 132.0f, 5.0f },
    { -135.0f, 37.0f },
    { 132.0f, 37.0f },
    { -135.0f, 69.0f },
    { 132.0f, 69.0f },
    { -135.0f, 101.0f },
    { 132.0f, 101.0f },
};

static HuVec2f lbl_1_data_688[8] = {
    { -190.0f, 10.0f },
    { 190.0f, 10.0f },
    { -190.0f, 10.0f },
    { 190.0f, 10.0f },
    { -140.0f, 10.0f },
    { 140.0f, 10.0f },
    { 55.0f, 43.0f },
    { 145.0f, 43.0f },
};

static HuVec2f lbl_1_data_6C8[16] = {
    { -132.0f, 10.0f },
    { -62.0f, 10.0f },
    { 62.0f, 10.0f },
    { 132.0f, 10.0f },
    { -132.0f, 10.0f },
    { -8.0f, 10.0f },
    { 62.0f, 10.0f },
    { 132.0f, 10.0f },
    { -80.0f, 10.0f },
    { 80.0f, 10.0f },
    { 0.0f, 10.0f },
    { 0.0f, 10.0f },
    { -100.0f, 10.0f },
    { 100.0f, 10.0f },
    { 0.0f, 10.0f },
    { 0.0f, 10.0f },
};

static HuVec2f lbl_1_data_748[4] = {
    { 0.0f, 10.0f },
    { -70.0f, 10.0f },
    { 0.0f, 10.0f },
    { 0.0f, 10.0f },
};

static HuVec2f lbl_1_data_768[3] = {
    { 0.0f, -90.0f },
    { 0.0f, 10.0f },
    { 0.0f, 110.0f },
};

static s32 lbl_1_data_780[13][4] = {
    { 0, 1, 2, 3 },
    { 1, 0, 2, 3 },
    { 2, 0, 1, 3 },
    { 3, 0, 1, 2 },
    { 0, 1, 2, 3 },
    { 0, 2, 1, 3 },
    { 0, 3, 1, 2 },
    { 0, 1, 2, 3 },
    { 0, 2, 1, 3 },
    { 0, 3, 1, 2 },
    { 1, 2, 0, 3 },
    { 1, 3, 0, 2 },
    { 2, 3, 0, 1 },
};

static MgFreeDecide lbl_1_bss_978;
MgFreeDecide *lbl_1_bss_974;
static char lbl_1_bss_966[0xE];
static char lbl_1_bss_948[0x1E];

void fn_1_46DC(s32 player, s16 grp, s32 member, s16 grp2, s32 prio, s16 member2)
{
    s32 listA[4];
    s32 listB[4];
    ANIMDATA *anim;

    anim = HuSprAnimRead(HuDataSelHeapReadNum(lbl_1_data_5A0[GwPlayerConf[player].charNo], HU_MEMNUM_OVL, HEAP_MODEL));
    HuSprGrpMemberSet(grp, member, HuSprCreate(anim, prio, 0));
    switch (GwCommon.languageNo) {
        case 0:
            listA[0] = 0x97001D;
            listA[1] = 0x97001E;
            listA[2] = 0x97001F;
            listA[3] = 0x970020;
            listB[0] = 0x970021;
            listB[1] = 0x970022;
            listB[2] = 0x970023;
            listB[3] = 0x970024;
            break;
        case 1:
            listA[0] = 0x97003A;
            listA[1] = 0x97003B;
            listA[2] = 0x97003C;
            listA[3] = 0x97003D;
            listB[0] = 0x97003E;
            listB[1] = 0x97003F;
            listB[2] = 0x970040;
            listB[3] = 0x970041;
            break;
    }
    if (GwPlayerConf[player].type != 0) {
        switch (GwPlayerConf[player].dif) {
            case 0:
                anim = HuSprAnimRead(HuDataSelHeapReadNum(listA[0], HU_MEMNUM_OVL, HEAP_MODEL));
                break;
            case 1:
                anim = HuSprAnimRead(HuDataSelHeapReadNum(listA[1], HU_MEMNUM_OVL, HEAP_MODEL));
                break;
            case 2:
                anim = HuSprAnimRead(HuDataSelHeapReadNum(listA[2], HU_MEMNUM_OVL, HEAP_MODEL));
                break;
            case 3:
                anim = HuSprAnimRead(HuDataSelHeapReadNum(listA[3], HU_MEMNUM_OVL, HEAP_MODEL));
                break;
        }
        HuSprGrpMemberSet(grp, grp2, HuSprCreate(anim, member2, 0));
        return;
    }
    switch (player) {
        case 0:
            anim = HuSprAnimRead(HuDataSelHeapReadNum(listB[0], HU_MEMNUM_OVL, HEAP_MODEL));
            break;
        case 1:
            anim = HuSprAnimRead(HuDataSelHeapReadNum(listB[1], HU_MEMNUM_OVL, HEAP_MODEL));
            break;
        case 2:
            anim = HuSprAnimRead(HuDataSelHeapReadNum(listB[2], HU_MEMNUM_OVL, HEAP_MODEL));
            break;
        case 3:
            anim = HuSprAnimRead(HuDataSelHeapReadNum(listB[3], HU_MEMNUM_OVL, HEAP_MODEL));
            break;
    }
    HuSprGrpMemberSet(grp, grp2, HuSprCreate(anim, member2, 0));
}

void fn_1_4A08(void)
{
    s32 n;
    s32 i;
    s32 j;

    n = lbl_1_bss_974->count - lbl_1_bss_974->scroll;
    HuSprPosSet(lbl_1_bss_974->id[27], 8, lbl_1_data_648[(s8)lbl_1_bss_974->col].x, lbl_1_data_648[(s8)lbl_1_bss_974->col].y);
    if (n <= 8) {
        i = n;
        while (i--) {
            HuWinDispOn(lbl_1_bss_974->id[1 + i]);
            if (lbl_1_bss_974->entry[lbl_1_bss_974->scroll + i].enable != 0) {
                HuWinMesSet(lbl_1_bss_974->id[1 + i], mgDataTbl[lbl_1_bss_974->entry[lbl_1_bss_974->scroll + i].mgNo].nameMes);
            } else {
                HuWinMesSet(lbl_1_bss_974->id[1 + i], 0x1F004F);
            }
        }
        j = 7;
        while (j >= n) {
            HuWinDispOff(lbl_1_bss_974->id[j + 1]);
            j--;
        }
        return;
    }
    i = 8;
    while (i--) {
        HuWinDispOn(lbl_1_bss_974->id[1 + i]);
        if (lbl_1_bss_974->entry[lbl_1_bss_974->scroll + i].enable != 0) {
            HuWinMesSet(lbl_1_bss_974->id[1 + i], mgDataTbl[lbl_1_bss_974->entry[lbl_1_bss_974->scroll + i].mgNo].nameMes);
        } else {
            HuWinMesSet(lbl_1_bss_974->id[1 + i], 0x1F004F);
        }
    }
}

void fn_1_4D20(s32 val)
{
    lbl_1_bss_974->state = val;
    lbl_1_bss_974->timer = 0;
}

u8 fn_1_4D4C(void)
{
    return lbl_1_bss_974->sel;
}

s32 fn_1_4D60(s32 idx)
{
    return lbl_1_bss_974->list[idx];
}

/* Shared body of the "select a tab" screens: tab_sprite is the sprite group id, win0 the first window id. */
#define DECIDE_TAB_SETUP(spr, win0)                                                                \
    do {                                                                                           \
        i = 3;                                                                                     \
        while (i--) {                                                                              \
            HuSprTPLvlSet(lbl_1_bss_974->id[spr], i, 0.0f);                                        \
            HuSprTPLvlSet(lbl_1_bss_974->id[spr], i + 3, 1.0f);                                    \
            HuSprScaleSet(lbl_1_bss_974->id[spr], i, 1.0f, 1.0f);                                  \
            HuSprScaleSet(lbl_1_bss_974->id[spr], i + 3, 0.9f, 0.9f);                              \
            if (i == (s8)lbl_1_bss_974->sel) {                                                     \
                HuWinMesPalSet(lbl_1_bss_974->id[win0 + i], 7, 255, 255, 255);                     \
            } else {                                                                               \
                HuWinMesPalSet(lbl_1_bss_974->id[win0 + i], 7, 160, 160, 160);                     \
            }                                                                                      \
        }                                                                                          \
    } while (0)

#define DECIDE_TAB_PULSE(spr)                                                                      \
    do {                                                                                           \
        HuSprTPLvlSet(lbl_1_bss_974->id[spr], (s8)lbl_1_bss_974->sel, 1.0f);                       \
        HuSprTPLvlSet(lbl_1_bss_974->id[spr], (s8)lbl_1_bss_974->sel + 3, 0.0f);                   \
        k = 0;                                                                                     \
        while (k <= 10) {                                                                          \
            t = (f32)k / 10.0f;                                                                                t = sin(3.141592653589793 * t * 0.5);                                                  \
            t = t * 0.1;                                                                           \
            t = t + 0.9;                                                                           \
            HuSprScaleSet(lbl_1_bss_974->id[spr], (s8)lbl_1_bss_974->sel, t, t);                   \
            HuPrcVSleep();                                                                         \
            k++;                                                                                   \
        }                                                                                          \
    } while (0)

void fn_1_4D7C(void)
{
    f32 t;
    s32 i;
    s32 k;

    DECIDE_TAB_SETUP(30, 20);
    fn_1_15B8();
    DECIDE_TAB_PULSE(30);
}

void fn_1_5058(void)
{
    f32 t;
    s32 i;
    s32 k;

    DECIDE_TAB_SETUP(31, 23);
    DECIDE_TAB_PULSE(31);
}

void fn_1_5330(void)
{
    f32 t;
    s32 i;
    s32 k;

    OSReport("519 Decide\n");
    i = 3;
    while (i--) {
        HuWinDispOn(lbl_1_bss_974->id[20 + i]);
    }
    lbl_1_bss_974->sel = 0;
    HuSprGrpPosSet(lbl_1_bss_974->id[30], lbl_1_data_628[0], lbl_1_data_628[1]);
    DECIDE_TAB_SETUP(30, 20);
    fn_1_15B8();
    DECIDE_TAB_PULSE(30);
}

void fn_1_56A0(void)
{
    f32 t;
    s32 i;
    s32 k;

    OSReport("552 Decide\n");
    i = 3;
    while (i--) {
        HuWinDispOn(lbl_1_bss_974->id[23 + i]);
    }
    lbl_1_bss_974->sel = 0;
    HuSprGrpPosSet(lbl_1_bss_974->id[31], lbl_1_data_628[0], lbl_1_data_628[1]);
    DECIDE_TAB_SETUP(31, 23);
    DECIDE_TAB_PULSE(31);
}

/* Layout of a team/duel screen: posIdx picks the cursor start, t688/t6C8 the sprite position tables. */
#define DECIDE_TEAM_SETUP(posIdx, t688, t6C8, scale)                                               \
    do {                                                                                           \
        pos = lbl_1_data_748[posIdx];                                                              \
        lbl_1_bss_974->unk148 = 0;                                                                 \
        c = 4;                                                                                     \
        while (c--) {                                                                              \
            lbl_1_bss_974->list[c] = c;                                                            \
        }                                                                                          \
        HuSprAttrReset(lbl_1_bss_974->id[28], 1, 4);                                               \
        HuSprPosSet(lbl_1_bss_974->id[28], 1, pos.x, pos.y);                                       \
        j = 2;                                                                                     \
        while (j--) {                                                                              \
            HuSprTPLvlSet(lbl_1_bss_974->id[29], j, 0.0f);                                         \
            HuSprTPLvlSet(lbl_1_bss_974->id[29], j + 2, 1.0f);                                     \
            HuSprPosSet(lbl_1_bss_974->id[29], j + 2, (t688)[j].x, (t688)[j].y);                   \
            lbl_1_bss_974->dstPos[j + 2] = (t688)[j];                                              \
            HuSprScaleSet(lbl_1_bss_974->id[29], j, scale, scale);                                 \
            HuSprScaleSet(lbl_1_bss_974->id[29], j + 2, scale, scale);                             \
            HuSprScaleSet(lbl_1_bss_974->id[29], j + 4, scale, scale);                             \
            HuSprScaleSet(lbl_1_bss_974->id[29], j + 6, scale, scale);                             \
        }                                                                                          \
        HuSprAttrSet(lbl_1_bss_974->id[28], 6, 4);                                                 \
        k = 0;                                                                                     \
        while (k < 4) {                                                                            \
            HuSprAttrSet(lbl_1_bss_974->id[28], k + 0xB, 4);                                       \
            k++;                                                                                   \
        }                                                                                          \
        k = 0;                                                                                     \
        while (k < 4) {                                                                            \
            HuSprAttrReset(lbl_1_bss_974->id[28], k + 2, 4);                                       \
            HuSprAttrReset(lbl_1_bss_974->id[28], k + 7, 4);                                       \
            HuSprPosSet(lbl_1_bss_974->id[28], k + 2, (t6C8)[k].x, (t6C8)[k].y);                   \
            HuSprPosSet(lbl_1_bss_974->id[28], k + 7, (t6C8)[k].x, 33.0f + (t6C8)[k].y);           \
            k++;                                                                                   \
        }                                                                                          \
        HuSprGrpPosSet(lbl_1_bss_974->id[28], lbl_1_data_608[0], lbl_1_data_608[1]);               \
    } while (0)

void fn_1_5A0C(void)
{
    HuVec2f pos;
    s32 j;
    s32 k;
    s8 c;

    OSReport("2 vs 2 Decide\n");
    DECIDE_TEAM_SETUP(0, lbl_1_data_688, lbl_1_data_6C8, 1.0f);
}

void fn_1_5E04(void)
{
    HuVec2f pos;
    s32 j;
    s32 k;
    s8 c;

    OSReport("1 vs 3 Decide\n");
    DECIDE_TEAM_SETUP(0, &lbl_1_data_688[2], &lbl_1_data_6C8[4], 1.0f);
}

void fn_1_6204(void)
{
    HuVec2f pos;
    s32 j;
    s32 k;
    s8 c;

    OSReport("Duel Decide\n");
    pos = lbl_1_data_748[2];
    lbl_1_bss_974->unk148 = 0;
    c = 4;
    while (c--) {
        lbl_1_bss_974->list[c] = c;
    }
    HuSprAttrReset(lbl_1_bss_974->id[28], 1, 4);
    HuSprPosSet(lbl_1_bss_974->id[28], 1, pos.x, pos.y);
    j = 2;
    while (j--) {
        HuSprTPLvlSet(lbl_1_bss_974->id[29], j, 0.0f);
        HuSprTPLvlSet(lbl_1_bss_974->id[29], j + 2, 1.0f);
        HuSprPosSet(lbl_1_bss_974->id[29], j + 2, lbl_1_data_688[4 + j].x, lbl_1_data_688[4 + j].y);
        lbl_1_bss_974->dstPos[j + 2] = lbl_1_data_688[4 + j];
        HuSprScaleSet(lbl_1_bss_974->id[29], j, 1.0f, 1.0f);
        HuSprScaleSet(lbl_1_bss_974->id[29], j + 2, 1.0f, 1.0f);
        HuSprScaleSet(lbl_1_bss_974->id[29], j + 4, 1.0f, 1.0f);
        HuSprScaleSet(lbl_1_bss_974->id[29], j + 6, 1.0f, 1.0f);
    }
    HuSprAttrSet(lbl_1_bss_974->id[28], 6, 4);
    k = 0;
    while (k < 4) {
        HuSprAttrSet(lbl_1_bss_974->id[28], k + 0xB, 4);
        k++;
    }
    k = 0;
    while (k < 2) {
        HuSprAttrReset(lbl_1_bss_974->id[28], k + 2, 4);
        HuSprAttrReset(lbl_1_bss_974->id[28], k + 7, 4);
        HuSprPosSet(lbl_1_bss_974->id[28], k + 2, lbl_1_data_6C8[8 + k].x, lbl_1_data_6C8[8 + k].y);
        HuSprPosSet(lbl_1_bss_974->id[28], k + 7, lbl_1_data_6C8[8 + k].x, 33.0f + lbl_1_data_6C8[8 + k].y);
        k++;
    }
    k = 2;
    while (k < 4) {
        HuSprAttrSet(lbl_1_bss_974->id[28], k + 2, 4);
        HuSprAttrSet(lbl_1_bss_974->id[28], k + 7, 4);
        k++;
    }
    HuSprGrpPosSet(lbl_1_bss_974->id[28], lbl_1_data_608[0], lbl_1_data_608[1]);
}

void fn_1_6660(void)
{
    HuVec2f pos;
    s32 j;
    s32 k;

    OSReport("Story Decide\n");
    pos = lbl_1_data_748[3];
    lbl_1_bss_974->sel = 1;
    HuSprAttrReset(lbl_1_bss_974->id[28], 1, 4);
    HuSprPosSet(lbl_1_bss_974->id[28], 1, pos.x, pos.y);
    j = 2;
    while (j--) {
        HuSprTPLvlSet(lbl_1_bss_974->id[29], j, 0.0f);
        HuSprTPLvlSet(lbl_1_bss_974->id[29], j + 2, 1.0f);
        HuSprPosSet(lbl_1_bss_974->id[29], j + 2, lbl_1_data_688[6 + j].x, lbl_1_data_688[6 + j].y);
        lbl_1_bss_974->dstPos[j + 2] = lbl_1_data_688[6 + j];
        HuSprScaleSet(lbl_1_bss_974->id[29], j, 0.7f, 0.7f);
        HuSprScaleSet(lbl_1_bss_974->id[29], j + 2, 0.7f, 0.7f);
        HuSprScaleSet(lbl_1_bss_974->id[29], j + 4, 0.7f, 0.7f);
        HuSprScaleSet(lbl_1_bss_974->id[29], j + 6, 0.7f, 0.7f);
    }
    HuSprAttrReset(lbl_1_bss_974->id[28], 2, 4);
    HuSprAttrReset(lbl_1_bss_974->id[28], 7, 4);
    HuSprPosSet(lbl_1_bss_974->id[28], 2, lbl_1_data_6C8[12].x, lbl_1_data_6C8[12].y);
    HuSprPosSet(lbl_1_bss_974->id[28], 7, lbl_1_data_6C8[12].x, 33.0f + lbl_1_data_6C8[12].y);
    k = 0;
    while (k < 4) {
        HuSprAttrSet(lbl_1_bss_974->id[28], k + 0xB, 4);
        k++;
    }
    HuSprAttrReset(lbl_1_bss_974->id[28], 6, 4);
    HuSprAttrReset(lbl_1_bss_974->id[28], (s8)lbl_1_bss_974->sel + 0xB, 4);
    HuSprPosSet(lbl_1_bss_974->id[28], 6, lbl_1_data_6C8[13].x, lbl_1_data_6C8[13].y);
    HuSprPosSet(lbl_1_bss_974->id[28], 0xB, lbl_1_data_6C8[13].x, 33.0f + lbl_1_data_6C8[13].y);
    HuSprPosSet(lbl_1_bss_974->id[28], 0xC, lbl_1_data_6C8[13].x, 33.0f + lbl_1_data_6C8[13].y);
    HuSprPosSet(lbl_1_bss_974->id[28], 0xD, lbl_1_data_6C8[13].x, 33.0f + lbl_1_data_6C8[13].y);
    HuSprPosSet(lbl_1_bss_974->id[28], 0xE, lbl_1_data_6C8[13].x, 33.0f + lbl_1_data_6C8[13].y);
    k = 1;
    while (k < 4) {
        HuSprAttrSet(lbl_1_bss_974->id[28], k + 2, 4);
        HuSprAttrSet(lbl_1_bss_974->id[28], k + 7, 4);
        k++;
    }
    HuSprGrpPosSet(lbl_1_bss_974->id[28], lbl_1_data_608[0], lbl_1_data_608[1]);
}

void fn_1_6B80(s32 *out1, s32 *out2, s32 *out3)
{
    *out1 = lbl_1_bss_974->idx;
    *out2 = lbl_1_bss_974->col;
    *out3 = lbl_1_bss_974->scroll;
}

void fn_1_91A8(void);
void fn_1_A570(void);

void fn_1_6BC4(s32 mode, s32 selIdx, s32 selCol, s32 selScroll)
{
    HuVec2f pos;
    ANIMDATA *anim;
    s16 grp;
    s16 win;
    s16 m;
    s32 list4[4];
    s32 listA[4];
    s32 listB[4];
    s32 i;
    s16 j;
    s16 k;

    lbl_1_bss_974 = &lbl_1_bss_978;
    lbl_1_bss_974->state = 0;
    lbl_1_bss_974->timer = 0;
    lbl_1_bss_974->idx = selIdx;
    lbl_1_bss_974->unk102 = 0;
    lbl_1_bss_974->col = selCol;
    lbl_1_bss_974->scroll = selScroll;
    lbl_1_bss_974->sel = 0;
    lbl_1_bss_974->count = 0;
    lbl_1_bss_974->unk148 = 0;
    i = 0;
    while (i < 26) {
        win = HuWinExCreateFrame(lbl_1_data_330[i].x - (lbl_1_data_330[i].w * 0.5f), lbl_1_data_330[i].y - (lbl_1_data_330[i].h * 0.5f), lbl_1_data_330[i].w, lbl_1_data_330[i].h, -1, 0);
        lbl_1_bss_974->id[i] = win;
        HuWinMesSpeedSet(win, 0);
        HuWinBGTPLvlSet(win, 0.0f);
        HuWinPriSet(win, lbl_1_data_330[i].prio);
        if (lbl_1_data_330[i].attr != 0) {
            HuWinAttrSet(win, lbl_1_data_330[i].attr);
        }
        HuWinDispOff(win);
        i++;
    }
    HuWinCenterPosSet(lbl_1_bss_974->id[0], lbl_1_data_330[0].w * 0.5f, lbl_1_data_330[0].h * 0.5f);
    HuWinCenterPosSet(lbl_1_bss_974->id[0x11], lbl_1_data_330[0x11].w * 0.5f, lbl_1_data_330[0x11].h * 0.5f);
    HuWinMesSet(lbl_1_bss_974->id[0x12], 0x20022);
    HuWinMesSet(lbl_1_bss_974->id[0x13], 0x20022);
    HuWinMesSet(lbl_1_bss_974->id[0x14], 0xE0002);
    HuWinMesSet(lbl_1_bss_974->id[0x15], 0xE0001);
    HuWinMesSet(lbl_1_bss_974->id[0x16], 0xE0003);
    HuWinMesSet(lbl_1_bss_974->id[0x17], 0xE0004);
    HuWinMesSet(lbl_1_bss_974->id[0x18], 0xE0005);
    HuWinMesSet(lbl_1_bss_974->id[0x19], 0xE0006);
    sprintf(lbl_1_bss_948, "%5d", GwCommon.mgRecord[9]);
    sprintf(lbl_1_bss_948 + 0xA, "%5d", GwCommon.mgRecord[11]);
    sprintf(lbl_1_bss_948 + 0x14, "%5d", GwCommon.charPlayNum[0][0]);
    HuWinInsertMesSet(lbl_1_bss_974->id[0x17], MESSNUM_PTR(lbl_1_bss_948), 0);
    HuWinInsertMesSet(lbl_1_bss_974->id[0x18], MESSNUM_PTR(lbl_1_bss_948 + 0xA), 1);
    HuWinInsertMesSet(lbl_1_bss_974->id[0x19], MESSNUM_PTR(lbl_1_bss_948 + 0x14), 2);
    HuWinDispOn(lbl_1_bss_974->id[0x12]);
    grp = HuSprGrpCreate(8);
    lbl_1_bss_974->id[26] = grp;
    i = 0;
    while (i < 4) {
        fn_1_46DC(i, grp, i, i + 4, 0x190, 0x186);
        HuSprPosSet(grp, i, lbl_1_data_5C8[i].x, lbl_1_data_5C8[i].y);
        HuSprPosSet(grp, i + 4, lbl_1_data_5C8[i].x, 33.0f + lbl_1_data_5C8[i].y);
        i++;
    }
    if (mode != 0) {
        HuSprGrpPosSet(grp, lbl_1_data_5F8[2], lbl_1_data_5F8[3]);
    } else {
        HuSprGrpPosSet(grp, lbl_1_data_5F8[0], lbl_1_data_5F8[1]);
    }
    {
        s32 list8[8] = { 0x97000F, 0x970010, 0x970011, 0x970012, 0x970013, 0x970014, 0x970015, 0x970016 };

        grp = HuSprGrpCreate(9);
        lbl_1_bss_974->id[27] = grp;
        i = 0;
        while (i < 8) {
            anim = HuSprAnimRead(HuDataSelHeapReadNum(list8[i], HU_MEMNUM_OVL, HEAP_MODEL));
            HuSprGrpMemberSet(grp, i, HuSprCreate(anim, 200, 0));
            HuSprPosSet(grp, i, 0.0f, 60.0f);
            HuSprAttrSet(grp, i, 4);
            i++;
        }
    }
    anim = HuSprAnimRead(HuDataSelHeapReadNum(0x970017, HU_MEMNUM_OVL, HEAP_MODEL));
    HuSprGrpMemberSet(grp, 8, HuSprCreate(anim, 0xB4, 0));
    HuSprPosSet(grp, 8, lbl_1_data_648[2].x, lbl_1_data_648[2].y);
    if (mode != 0) {
        HuSprGrpPosSet(grp, lbl_1_data_5E8[2], lbl_1_data_5E8[3]);
    } else {
        HuSprGrpPosSet(grp, lbl_1_data_5E8[0], lbl_1_data_5E8[1]);
    }
    HuSprGrpDrawNoSet(grp, 0x40);
    pos = lbl_1_data_748[0];
    grp = HuSprGrpCreate(0xF);
    lbl_1_bss_974->id[28] = grp;
    anim = HuSprAnimRead(HuDataSelHeapReadNum(0x970018, HU_MEMNUM_OVL, HEAP_MODEL));
    HuSprGrpMemberSet(grp, 0, HuSprCreate(anim, 0x8C, 0));
    HuSprPosSet(grp, 0, 0.0f, 20.0f);
    switch (GwCommon.languageNo) {
        case 0:
            anim = HuSprAnimRead(HuDataSelHeapReadNum(0x970019, HU_MEMNUM_OVL, HEAP_MODEL));
            break;
        case 1:
            anim = HuSprAnimRead(HuDataSelHeapReadNum(0x970043, HU_MEMNUM_OVL, HEAP_MODEL));
            break;
    }
    HuSprGrpMemberSet(grp, 1, HuSprCreate(anim, 0x73, 0));
    HuSprPosSet(grp, 1, pos.x, pos.y);
    i = 0;
    while (i < 4) {
        fn_1_46DC(i, grp, i + 2, i + 7, 0x82, 0x78);
        HuSprPosSet(grp, i + 2, lbl_1_data_6C8[i].x, lbl_1_data_6C8[i].y);
        HuSprPosSet(grp, i + 7, lbl_1_data_6C8[i].x, 33.0f + lbl_1_data_6C8[i].y);
        i++;
    }
    switch (GwCommon.languageNo) {
        case 0:
            list4[0] = 0x97001D;
            list4[1] = 0x97001E;
            list4[2] = 0x97001F;
            list4[3] = 0x970020;
            break;
        case 1:
            list4[0] = 0x97003A;
            list4[1] = 0x97003B;
            list4[2] = 0x97003C;
            list4[3] = 0x97003D;
            break;
    }
    anim = HuSprAnimRead(HuDataSelHeapReadNum(0x970038, HU_MEMNUM_OVL, HEAP_MODEL));
    HuSprGrpMemberSet(grp, 6, HuSprCreate(anim, 0x82, 0));
    anim = HuSprAnimRead(HuDataSelHeapReadNum(list4[0], HU_MEMNUM_OVL, HEAP_MODEL));
    HuSprGrpMemberSet(grp, 0xB, HuSprCreate(anim, 0x78, 0));
    anim = HuSprAnimRead(HuDataSelHeapReadNum(list4[1], HU_MEMNUM_OVL, HEAP_MODEL));
    HuSprGrpMemberSet(grp, 0xC, HuSprCreate(anim, 0x78, 0));
    anim = HuSprAnimRead(HuDataSelHeapReadNum(list4[2], HU_MEMNUM_OVL, HEAP_MODEL));
    HuSprGrpMemberSet(grp, 0xD, HuSprCreate(anim, 0x78, 0));
    anim = HuSprAnimRead(HuDataSelHeapReadNum(list4[3], HU_MEMNUM_OVL, HEAP_MODEL));
    HuSprGrpMemberSet(grp, 0xE, HuSprCreate(anim, 0x78, 0));
    HuSprAttrSet(grp, 6, 4);
    i = 0;
    while (i < 4) {
        HuSprAttrSet(grp, i + 0xB, 4);
        i++;
    }
    HuSprGrpPosSet(grp, lbl_1_data_608[2], lbl_1_data_608[3]);
    HuWinDispOff(lbl_1_bss_974->id[0x13]);
    HuSprGrpDrawNoSet(grp, 0);
    grp = HuSprGrpCreate(6);
    lbl_1_bss_974->id[30] = grp;
    i = 3;
    while (i--) {
        anim = HuSprAnimRead(HuDataSelHeapReadNum(0x97001A, HU_MEMNUM_OVL, HEAP_MODEL));
        HuSprGrpMemberSet(grp, i, HuSprCreate(anim, 100, 0));
        HuSprPosSet(grp, i, lbl_1_data_768[i].x, lbl_1_data_768[i].y);
    }
    i = 3;
    while (i--) {
        m = i + 3;
        anim = HuSprAnimRead(HuDataSelHeapReadNum(0x97001B, HU_MEMNUM_OVL, HEAP_MODEL));
        HuSprGrpMemberSet(grp, m, HuSprCreate(anim, 100, 0));
        HuSprPosSet(grp, m, lbl_1_data_768[i].x, lbl_1_data_768[i].y);
        HuSprTPLvlSet(grp, m, 0.0f);
    }
    HuSprGrpPosSet(grp, lbl_1_data_628[2], lbl_1_data_628[3]);
    HuSprGrpDrawNoSet(grp, 0);
    grp = HuSprGrpCreate(6);
    lbl_1_bss_974->id[31] = grp;
    i = 3;
    while (i--) {
        anim = HuSprAnimRead(HuDataSelHeapReadNum(0x97001A, HU_MEMNUM_OVL, HEAP_MODEL));
        HuSprGrpMemberSet(grp, i, HuSprCreate(anim, 100, 0));
        HuSprPosSet(grp, i, lbl_1_data_768[i].x, lbl_1_data_768[i].y);
    }
    i = 3;
    while (i--) {
        m = i + 3;
        anim = HuSprAnimRead(HuDataSelHeapReadNum(0x97001B, HU_MEMNUM_OVL, HEAP_MODEL));
        HuSprGrpMemberSet(grp, m, HuSprCreate(anim, 100, 0));
        HuSprPosSet(grp, m, lbl_1_data_768[i].x, lbl_1_data_768[i].y);
        HuSprTPLvlSet(grp, m, 0.0f);
    }
    HuSprGrpPosSet(grp, lbl_1_data_628[2], lbl_1_data_628[3]);
    HuSprGrpDrawNoSet(grp, 0);
    listA[0] = 0x970028;
    listA[1] = 0x970029;
    listA[2] = 0x970027;
    listA[3] = 0x970026;
    listB[0] = 0x97002C;
    listB[1] = 0x97002D;
    listB[2] = 0x97002B;
    listB[3] = 0x97002A;
    grp = HuSprGrpCreate(8);
    lbl_1_bss_974->id[29] = grp;
    i = 4;
    while (i--) {
        lbl_1_bss_974->unk108[i] = 0;
        lbl_1_bss_974->proc[i] = NULL;
    }
    j = 2;
    while (j--) {
        anim = HuSprAnimRead(HuDataSelHeapReadNum(listA[j], HU_MEMNUM_OVL, HEAP_MODEL));
        HuSprGrpMemberSet(grp, j, HuSprCreate(anim, 0xA0, 0));
        HuSprPosSet(grp, j, lbl_1_data_638[j].x, lbl_1_data_638[j].y);
        m = j + 4;
        anim = HuSprAnimRead(HuDataSelHeapReadNum(listB[j], HU_MEMNUM_OVL, HEAP_MODEL));
        HuSprGrpMemberSet(grp, m, HuSprCreate(anim, 0xA0, 0));
        HuSprPosSet(grp, m, lbl_1_data_638[j].x, lbl_1_data_638[j].y);
        HuSprTPLvlSet(grp, m, 0.0f);
        lbl_1_bss_974->dstPos[j] = lbl_1_data_638[j];
    }
    k = 2;
    while (k--) {
        m = k + 2;
        anim = HuSprAnimRead(HuDataSelHeapReadNum(listA[k + 2], HU_MEMNUM_OVL, HEAP_MODEL));
        HuSprGrpMemberSet(grp, m, HuSprCreate(anim, 0x6E, 0));
        HuSprPosSet(grp, m, lbl_1_data_688[k].x, lbl_1_data_688[k].y);
        HuSprTPLvlSet(grp, m, 0.0f);
        m = k + 6;
        anim = HuSprAnimRead(HuDataSelHeapReadNum(listB[k + 2], HU_MEMNUM_OVL, HEAP_MODEL));
        HuSprGrpMemberSet(grp, m, HuSprCreate(anim, 0x6E, 0));
        HuSprPosSet(grp, m, lbl_1_data_688[k].x, lbl_1_data_688[k].y);
        HuSprTPLvlSet(grp, m, 0.0f);
        lbl_1_bss_974->dstPos[k + 2] = lbl_1_data_688[k];
    }
    if (mode != 0) {
        HuSprGrpPosSet(grp, lbl_1_data_618[2], lbl_1_data_618[3]);
    } else {
        HuSprGrpPosSet(grp, lbl_1_data_618[0], lbl_1_data_618[1]);
    }
    HuSprGrpDrawNoSet(grp, 0);
    HuSprExecLayerSet(0x40, 1);
    fn_1_91A8();
    if (mode != 0) {
        HuWinDispOff(lbl_1_bss_974->id[0]);
        HuWinDispOff(lbl_1_bss_974->id[0x11]);
        HuWinDispOff(lbl_1_bss_974->id[0x12]);
        HuWinDispOff(lbl_1_bss_974->id[1]);
        HuWinDispOff(lbl_1_bss_974->id[2]);
        HuWinDispOff(lbl_1_bss_974->id[3]);
        HuWinDispOff(lbl_1_bss_974->id[4]);
        HuWinDispOff(lbl_1_bss_974->id[5]);
        HuWinDispOff(lbl_1_bss_974->id[6]);
        HuWinDispOff(lbl_1_bss_974->id[7]);
        HuWinDispOff(lbl_1_bss_974->id[8]);
    }
    HuPrcChildCreate(fn_1_A570, 200, 0x2000, 0, HuPrcCurrentGet());
    fn_1_15B8();
}

/* Bounce animation of one arrow: slot picks the timer, mem/mem+4 the sprites. */
#define DECIDE_BOUNCE(slot, mem, dstIdx, sa, sb, POSX, POSY)                                           do {                                                                                                   HuVec2f *dst = &lbl_1_bss_974->dstPos[dstIdx];                                                     while (lbl_1_bss_974->unk108[slot]-- != 0) {                                                           a = (f32)lbl_1_bss_974->unk108[slot] * 0.125f * 2.0f;                                              if (a >= 1.0f) {                                                                                       a = 2.0f - a;                                                                                  }                                                                                                  t = sin(3.141592653589793 * a * 0.5);                                                              s = (sa * t) + sb;                                                                                 HuSprScaleSet(lbl_1_bss_974->id[29], (mem) + 4, s, s);                                             HuSprPosSet(lbl_1_bss_974->id[29], (mem) + 4, POSX, POSY);                                         HuPrcVSleep();                                                                                 }                                                                                              } while (0)

#define DECIDE_BOUNCE_START(mem, show, hide)                                                           HuSprTPLvlSet(lbl_1_bss_974->id[29], mem, hide);                                                   HuSprTPLvlSet(lbl_1_bss_974->id[29], (mem) + 4, show)

void fn_1_7FA0(void)
{
    f32 t;
    f32 s;
    f32 a;

    DECIDE_BOUNCE_START(0, 1.0f, 0.0f);
    DECIDE_BOUNCE(0, 0, 0, 0.2f, 1.0f, dst->x, dst->y - (16.0f * t));
    HuSprTPLvlSet(lbl_1_bss_974->id[29], 4, 0.0f);
    if (lbl_1_bss_974->idx - 1 <= 0) {
        HuSprTPLvlSet(lbl_1_bss_974->id[29], 0, 0.0f);
    } else {
        HuSprTPLvlSet(lbl_1_bss_974->id[29], 0, 1.0f);
    }
    lbl_1_bss_974->proc[0] = NULL;
    HuPrcEnd();
}

void fn_1_823C(void)
{
    lbl_1_bss_974->unk108[0] = 8;
    if (lbl_1_bss_974->proc[0] == 0) {
        lbl_1_bss_974->proc[0] = HuPrcChildCreate(fn_1_7FA0, 200, 0x1000, 0, HuPrcCurrentGet());
    }
}

void fn_1_82B4(void)
{
    f32 t;
    f32 s;
    f32 a;

    DECIDE_BOUNCE_START(1, 1.0f, 0.0f);
    DECIDE_BOUNCE(1, 1, 1, 0.2f, 1.0f, dst->x, dst->y + (16.0f * t));
    HuSprTPLvlSet(lbl_1_bss_974->id[29], 5, 0.0f);
    if (lbl_1_bss_974->count & 1) {
        if (lbl_1_bss_974->idx == lbl_1_bss_974->count - 1) {
            HuSprTPLvlSet(lbl_1_bss_974->id[29], 1, 0.0f);
        } else {
            HuSprTPLvlSet(lbl_1_bss_974->id[29], 1, 1.0f);
        }
    } else if (lbl_1_bss_974->count <= lbl_1_bss_974->idx + 2) {
        HuSprTPLvlSet(lbl_1_bss_974->id[29], 1, 0.0f);
    } else {
        HuSprTPLvlSet(lbl_1_bss_974->id[29], 1, 1.0f);
    }
    lbl_1_bss_974->proc[1] = NULL;
    HuPrcEnd();
}

void fn_1_8600(void)
{
    lbl_1_bss_974->unk108[1] = 8;
    if (lbl_1_bss_974->proc[1] == 0) {
        lbl_1_bss_974->proc[1] = HuPrcChildCreate(fn_1_82B4, 200, 0x1000, 0, HuPrcCurrentGet());
    }
}

void fn_1_8678(void)
{
    f32 t;
    f32 s;
    f32 a;

    DECIDE_BOUNCE_START(2, 1.0f, 0.0f);
    DECIDE_BOUNCE(2, 2, 2, 0.2f, 1.0f, dst->x - (16.0f * t), dst->y);
    HuSprTPLvlSet(lbl_1_bss_974->id[29], 2, 1.0f);
    HuSprTPLvlSet(lbl_1_bss_974->id[29], 6, 0.0f);
    lbl_1_bss_974->proc[2] = NULL;
    HuPrcEnd();
}

void fn_1_88CC(void)
{
    lbl_1_bss_974->unk108[2] = 8;
    if (lbl_1_bss_974->proc[2] == 0) {
        lbl_1_bss_974->proc[2] = HuPrcChildCreate(fn_1_8678, 200, 0x1000, 0, HuPrcCurrentGet());
    }
}

void fn_1_8944(void)
{
    f32 t;
    f32 s;
    f32 a;

    DECIDE_BOUNCE_START(3, 1.0f, 0.0f);
    DECIDE_BOUNCE(3, 3, 3, 0.2f, 1.0f, dst->x + (16.0f * t), dst->y);
    HuSprTPLvlSet(lbl_1_bss_974->id[29], 3, 1.0f);
    HuSprTPLvlSet(lbl_1_bss_974->id[29], 7, 0.0f);
    lbl_1_bss_974->proc[3] = NULL;
    HuPrcEnd();
}

void fn_1_8B98(void)
{
    lbl_1_bss_974->unk108[3] = 8;
    if (lbl_1_bss_974->proc[3] == 0) {
        lbl_1_bss_974->proc[3] = HuPrcChildCreate(fn_1_8944, 200, 0x1000, 0, HuPrcCurrentGet());
    }
}

void fn_1_8C10(void)
{
    f32 t;
    f32 s;
    f32 a;

    DECIDE_BOUNCE_START(2, 1.0f, 0.0f);
    DECIDE_BOUNCE(2, 2, 2, 0.3f, 0.7f, dst->x - (16.0f * t), dst->y);
    HuSprTPLvlSet(lbl_1_bss_974->id[29], 2, 1.0f);
    HuSprTPLvlSet(lbl_1_bss_974->id[29], 6, 0.0f);
    lbl_1_bss_974->proc[2] = NULL;
    HuPrcEnd();
}

void fn_1_8E64(void)
{
    lbl_1_bss_974->unk108[2] = 8;
    if (lbl_1_bss_974->proc[2] == 0) {
        lbl_1_bss_974->proc[2] = HuPrcChildCreate(fn_1_8C10, 200, 0x1000, 0, HuPrcCurrentGet());
    }
}

void fn_1_8EDC(void)
{
    f32 t;
    f32 s;
    f32 a;

    DECIDE_BOUNCE_START(3, 1.0f, 0.0f);
    DECIDE_BOUNCE(3, 3, 3, 0.3f, 0.7f, dst->x + (16.0f * t), dst->y);
    HuSprTPLvlSet(lbl_1_bss_974->id[29], 3, 1.0f);
    HuSprTPLvlSet(lbl_1_bss_974->id[29], 7, 0.0f);
    lbl_1_bss_974->proc[3] = NULL;
    HuPrcEnd();
}

void fn_1_9130(void)
{
    lbl_1_bss_974->unk108[3] = 8;
    if (lbl_1_bss_974->proc[3] == 0) {
        lbl_1_bss_974->proc[3] = HuPrcChildCreate(fn_1_8EDC, 200, 0x1000, 0, HuPrcCurrentGet());
    }
}

void fn_1_91A8(void)
{
    s32 unlocked;
    s32 i;
    s16 j;
    s16 k;
    s32 n;

    lbl_1_bss_974->unk102 = lbl_1_bss_974->idx;
    j = 0;
    lbl_1_bss_974->count = 0;
    unlocked = 0;
    while (mgDataTbl[j].ovl != 0xFFFF) {
        if (mgDataTbl[j].type == fn_1_218() && (mgDataTbl[j].flag & MG_FLAG_SPECIAL) == 0) {
            lbl_1_bss_974->entry[lbl_1_bss_974->count].mgNo = j;
            lbl_1_bss_974->entry[lbl_1_bss_974->count].enable = GWMgUnlockGet(j + 0x1F5);
            if (lbl_1_bss_974->entry[lbl_1_bss_974->count].enable != 0) {
                unlocked++;
            }
            lbl_1_bss_974->count++;
        }
        j++;
    }
    OSReport("Type = %d\n", fn_1_218());
    OSReport("\203~\203j\203Q\201[\203\200\220\224 %d\n", lbl_1_bss_974->count);
    HuWinDispOn(lbl_1_bss_974->id[0]);
    HuWinMesSet(lbl_1_bss_974->id[0], fn_1_244());
    sprintf(lbl_1_bss_966, "%2d", unlocked);
    sprintf(lbl_1_bss_966 + 6, "%2d", lbl_1_bss_974->count);
    HuWinScaleSet(lbl_1_bss_974->id[0x11], 0.8f, 0.8f);
    HuWinDispOn(lbl_1_bss_974->id[0x11]);
    HuWinMesSet(lbl_1_bss_974->id[0x11], 0xE000F);
    HuWinInsertMesSet(lbl_1_bss_974->id[0x11], MESSNUM_PTR(lbl_1_bss_966), 0);
    HuWinInsertMesSet(lbl_1_bss_974->id[0x11], MESSNUM_PTR(lbl_1_bss_966 + 6), 1);
    fn_1_4A08();
    k = 0;
    while (k < 8) {
        HuSprAttrSet(lbl_1_bss_974->id[27], k, 4);
        k++;
    }
    switch (fn_1_218()) {
        case 0:
            HuSprAttrReset(lbl_1_bss_974->id[27], 0, 4);
            break;
        case 1:
            HuSprAttrReset(lbl_1_bss_974->id[27], 1, 4);
            break;
        case 2:
            HuSprAttrReset(lbl_1_bss_974->id[27], 2, 4);
            break;
        case 3:
            HuSprAttrReset(lbl_1_bss_974->id[27], 3, 4);
            break;
        case 6:
            HuSprAttrReset(lbl_1_bss_974->id[27], 4, 4);
            break;
        case 4:
            HuSprAttrReset(lbl_1_bss_974->id[27], 5, 4);
            break;
        case 8:
            HuSprAttrReset(lbl_1_bss_974->id[27], 6, 4);
            break;
        case 5:
            HuSprAttrReset(lbl_1_bss_974->id[27], 7, 4);
            break;
    }
    if (lbl_1_bss_974->count <= 8) {
        HuSprTPLvlSet(lbl_1_bss_974->id[29], 0, 0.0f);
        HuSprTPLvlSet(lbl_1_bss_974->id[29], 1, 0.0f);
    } else {
        if (lbl_1_bss_974->scroll - 1 <= 0) {
            HuSprTPLvlSet(lbl_1_bss_974->id[29], 0, 0.0f);
        } else {
            HuSprTPLvlSet(lbl_1_bss_974->id[29], 0, 1.0f);
        }
        if (lbl_1_bss_974->count & 1) {
            if (lbl_1_bss_974->scroll >= lbl_1_bss_974->count - 7) {
                HuSprTPLvlSet(lbl_1_bss_974->id[29], 1, 0.0f);
            } else {
                HuSprTPLvlSet(lbl_1_bss_974->id[29], 1, 1.0f);
            }
        } else if (lbl_1_bss_974->scroll >= lbl_1_bss_974->count - 8) {
            HuSprTPLvlSet(lbl_1_bss_974->id[29], 1, 0.0f);
        } else {
            HuSprTPLvlSet(lbl_1_bss_974->id[29], 1, 1.0f);
        }
    }
    HuSprPosSet(lbl_1_bss_974->id[27], 8, lbl_1_data_648[(s8)lbl_1_bss_974->col].x, lbl_1_data_648[(s8)lbl_1_bss_974->col].y);
}

void fn_1_9B60(void)
{
    s32 i;

    HuWinDispOn(lbl_1_bss_974->id[0]);
    i = 0;
    while (i < 5) {
        HuWinScaleSet(lbl_1_bss_974->id[0], fn_1_2B4(1.0f, 0.0f, (f32)i / 5.0f), fn_1_2B4(1.0f, 0.0f, (f32)i / 5.0f));
        HuPrcVSleep();
        i++;
    }
    HuWinScaleSet(lbl_1_bss_974->id[0], 1.0f, 1.0f);
    HuPrcEnd();
}

void fn_1_9CF4(void)
{
    s32 i;

    HuSprGrpPosSet(lbl_1_bss_974->id[26], lbl_1_data_5F8[2], lbl_1_data_5F8[3]);
    i = 60;
    while (i--) {
        HuSprGrpPosSet(lbl_1_bss_974->id[26], lbl_1_data_5F8[0] + (((f32)i / 60.0f) * (lbl_1_data_5F8[2] - lbl_1_data_5F8[0])), lbl_1_data_5F8[1]);
        HuPrcVSleep();
    }
    HuPrcEnd();
}

void fn_1_9E0C(void)
{
    s32 list[10] = { 0x12, 0x11, 1, 2, 3, 4, 5, 6, 7, 8 };
    DecideWinParam *p;
    f32 f;
    s32 n;
    s32 m;

    m = 10;
    while (m--) {
        HuWinDispOn(lbl_1_bss_974->id[list[m]]);
    }
    n = 60;
    while (n--) {
        f = ((f32)n / 60.0f) * (lbl_1_data_5E8[3] - lbl_1_data_5E8[1]);
        HuSprGrpPosSet(lbl_1_bss_974->id[27], lbl_1_data_5E8[0], lbl_1_data_5E8[1] + f);
        HuSprGrpPosSet(lbl_1_bss_974->id[29], lbl_1_data_618[0], lbl_1_data_618[1] + f);
        m = 10;
        while (m--) {
            p = &lbl_1_data_330[list[m]];
            HuWinPosSet(lbl_1_bss_974->id[list[m]], p->x - (p->w * 0.5f), f + (p->y - (p->h * 0.5f)));
        }
        HuPrcVSleep();
    }
    HuPrcEnd();
}

void fn_1_A068(void)
{
    HuPrcChildCreate(fn_1_9B60, 200, 0x1000, 0, HuPrcCurrentGet());
    HuPrcChildCreate(fn_1_9CF4, 200, 0x1000, 0, HuPrcCurrentGet());
    HuPrcChildCreate(fn_1_9E0C, 200, 0x1000, 0, HuPrcCurrentGet());
}

void fn_1_A0E4(void)
{
    f32 zero;
    s32 i;
    s32 n;

    zero = 0.0f;
    i = 0;
    while (i < 5) {
        HuWinScaleSet(lbl_1_bss_974->id[0], fn_1_2B4(0.0f, 1.0f, (f32)i / 5.0f), fn_1_2B4(0.0f, 1.0f, (f32)i / 5.0f));
        HuPrcVSleep();
        i++;
    }
    HuWinScaleSet(lbl_1_bss_974->id[0], 0.0f, 0.0f);
    HuWinDispOff(lbl_1_bss_974->id[0]);
    n = 15;
    while (n--) {
        HuPrcVSleep();
    }
    lbl_1_bss_974->idx = 0;
    lbl_1_bss_974->unk102 = 0;
    lbl_1_bss_974->col = 0;
    lbl_1_bss_974->scroll = 0;
    fn_1_91A8();
    fn_1_15B8();
    HuWinDispOn(lbl_1_bss_974->id[0]);
    HuAudFXPlay(0x4F4);
    i = 0;
    while (i < 5) {
        HuWinScaleSet(lbl_1_bss_974->id[0], fn_1_2B4(1.0f, 0.0f, (f32)i / 5.0f), fn_1_2B4(1.0f, 0.0f, (f32)i / 5.0f));
        HuPrcVSleep();
        i++;
    }
    HuWinScaleSet(lbl_1_bss_974->id[0], 1.0f, 1.0f);
    HuPrcEnd();
}

void fn_1_A454(void)
{
    s32 n;

    HuPrcChildCreate(fn_1_A0E4, 200, 0x2000, 0, HuPrcCurrentGet());
    n = 5;
    while (n--) {
        HuPrcVSleep();
    }
}

s16 fn_1_A4B0(void)
{
    return lbl_1_bss_974->entry[lbl_1_bss_974->idx].mgNo;
}

s32 fn_1_A4DC(void)
{
    if (lbl_1_bss_974->entry[lbl_1_bss_974->idx].enable == 0) {
        return -1;
    }
    return lbl_1_bss_974->entry[lbl_1_bss_974->idx].mgNo;
}

s32 fn_1_A544(void)
{
    return lbl_1_bss_974->state == 1;
}

/* Re-lay the four team sprites after the arrow selection changed; tbl is the position table, row the permutation row. */
#define DECIDE_TEAM_SHUFFLE(tbl, row)                                                              \
    do {                                                                                           \
        for (i = 0; i < 4; i++) {                                                                  \
            lbl_1_bss_974->list[i] = lbl_1_data_780[row][i];                                       \
            m = lbl_1_bss_974->list[i];                                                            \
            HuSprPosSet(lbl_1_bss_974->id[28], m + 2, (tbl)[i].x, (tbl)[i].y);                     \
            HuSprPosSet(lbl_1_bss_974->id[28], m + 7, (tbl)[i].x, 33.0f + (tbl)[i].y);             \
        }                                                                                          \
    } while (0)

#define DECIDE_WAIT10()                                                                            \
    do {                                                                                           \
        wait = 10;                                                                                 \
        while (wait-- != 0) {                                                                      \
            HuPrcVSleep();                                                                         \
        }                                                                                          \
    } while (0)

#define DECIDE_BACK_TO_IDLE()                                                                      \
    do {                                                                                           \
        lbl_1_bss_974->state = 1;                                                                  \
        lbl_1_bss_974->timer = 0;                                                                  \
    } while (0)

void fn_1_A570(void)
{
    f32 t;
    s32 dir;
    s32 wait;
    s32 i;
    s32 k;
    s32 m;
    s32 n;
    s32 mgIdx;
    MGDATA *mg;
    s32 mgNo;
    HuVec2f *tbl;

    while (1) {
        if ((u32)lbl_1_bss_974->state <= 0x16) {
            switch (lbl_1_bss_974->state) {
                case 0:
                    lbl_1_bss_974->state = 1;
                    lbl_1_bss_974->timer = 0;
                    break;
                case 2:
                    if (lbl_1_bss_974->idx >= 2) {
                        lbl_1_bss_974->idx -= 2;
                        lbl_1_bss_974->state = 6;
                        lbl_1_bss_974->timer = 0;
                    } else {
                        DECIDE_BACK_TO_IDLE();
                    }
                    break;
                case 3:
                    if (lbl_1_bss_974->count & 1) {
                        lbl_1_bss_974->idx += 2;
                        if (lbl_1_bss_974->idx > lbl_1_bss_974->count - 1) {
                            lbl_1_bss_974->idx = lbl_1_bss_974->count - 1;
                        }
                    } else if (lbl_1_bss_974->count > lbl_1_bss_974->idx + 2) {
                        lbl_1_bss_974->idx += 2;
                    }
                    if (lbl_1_bss_974->idx != lbl_1_bss_974->unk102) {
                        lbl_1_bss_974->state = 6;
                        lbl_1_bss_974->timer = 0;
                    } else {
                        DECIDE_BACK_TO_IDLE();
                    }
                    break;
                case 4:
                    if (lbl_1_bss_974->idx >= 1 && (lbl_1_bss_974->idx & 1) != 0) {
                        lbl_1_bss_974->idx -= 1;
                        lbl_1_bss_974->state = 6;
                        lbl_1_bss_974->timer = 0;
                    } else {
                        DECIDE_BACK_TO_IDLE();
                    }
                    break;
                case 5:
                    if (lbl_1_bss_974->count > lbl_1_bss_974->idx + 1 && (lbl_1_bss_974->idx & 1) == 0) {
                        lbl_1_bss_974->idx += 1;
                        lbl_1_bss_974->state = 6;
                        lbl_1_bss_974->timer = 0;
                    } else {
                        DECIDE_BACK_TO_IDLE();
                    }
                    break;
                case 6:
                    switch (lbl_1_bss_974->timer) {
                        case 0:
                            n = lbl_1_bss_974->idx - lbl_1_bss_974->unk102;
                            dir = 0;
                            switch ((s8)lbl_1_bss_974->col) {
                                case 0:
                                    if (n < 0) {
                                        if (n == -1) {
                                            lbl_1_bss_974->col = 1;
                                            dir = -1;
                                        } else {
                                            dir = -1;
                                        }
                                    } else {
                                        lbl_1_bss_974->col = lbl_1_bss_974->col + n;
                                    }
                                    break;
                                case 1:
                                    if (n < 0) {
                                        if (n == -1) {
                                            lbl_1_bss_974->col = 0;
                                        } else {
                                            dir = -1;
                                        }
                                    } else {
                                        lbl_1_bss_974->col = lbl_1_bss_974->col + n;
                                    }
                                    break;
                                case 6:
                                    if (n > 0) {
                                        if (n == 1) {
                                            lbl_1_bss_974->col = 7;
                                        } else {
                                            dir = 1;
                                        }
                                    } else {
                                        lbl_1_bss_974->col = lbl_1_bss_974->col + n;
                                    }
                                    break;
                                case 7:
                                    if (n > 0) {
                                        if (n == 1) {
                                            lbl_1_bss_974->col = 6;
                                            dir = 1;
                                        } else {
                                            dir = 1;
                                        }
                                    } else {
                                        lbl_1_bss_974->col = lbl_1_bss_974->col + n;
                                    }
                                    break;
                                default:
                                    lbl_1_bss_974->col = lbl_1_bss_974->col + n;
                                    break;
                            }
                            fn_1_15B8();
                            HuAudFXPlay(0);
                            lbl_1_bss_974->timer = 1;
                            break;
                        case 1:
                            switch (dir) {
                                case -1:
                                    HuSprTPLvlSet(lbl_1_bss_974->id[29], 1, 1.0f);
                                    fn_1_823C();
                                    lbl_1_bss_974->scroll -= 2;
                                    fn_1_4A08();
                                    break;
                                case 1:
                                    HuSprTPLvlSet(lbl_1_bss_974->id[29], 0, 1.0f);
                                    fn_1_8600();
                                    lbl_1_bss_974->scroll += 2;
                                    fn_1_4A08();
                                    break;
                                case 0:
                                    HuSprPosSet(lbl_1_bss_974->id[27], 8, lbl_1_data_648[(s8)lbl_1_bss_974->col].x, lbl_1_data_648[(s8)lbl_1_bss_974->col].y);
                                    break;
                            }
                            wait = 5;
                            lbl_1_bss_974->timer = 2;
                            break;
                        case 2:
                            wait--;
                            if (wait == 0) {
                                lbl_1_bss_974->unk102 = lbl_1_bss_974->idx;
                                DECIDE_BACK_TO_IDLE();
                            }
                            break;
                    }
                    break;
                case 7:
                    HuAudFXPlay(0);
                    lbl_1_bss_974->unk148--;
                    if (lbl_1_bss_974->unk148 < 0) {
                        lbl_1_bss_974->unk148 = 2;
                    }
                    tbl = lbl_1_data_6C8;
                    DECIDE_TEAM_SHUFFLE(tbl, (s8)lbl_1_bss_974->unk148 + 4);
                    fn_1_88CC();
                    DECIDE_WAIT10();
                    DECIDE_BACK_TO_IDLE();
                    break;
                case 8:
                    HuAudFXPlay(0);
                    lbl_1_bss_974->unk148++;
                    if (lbl_1_bss_974->unk148 >= 3) {
                        lbl_1_bss_974->unk148 = 0;
                    }
                    tbl = lbl_1_data_6C8;
                    DECIDE_TEAM_SHUFFLE(tbl, (s8)lbl_1_bss_974->unk148 + 4);
                    fn_1_8B98();
                    DECIDE_WAIT10();
                    DECIDE_BACK_TO_IDLE();
                    break;
                case 9:
                    HuAudFXPlay(0);
                    lbl_1_bss_974->unk148--;
                    if (lbl_1_bss_974->unk148 < 0) {
                        lbl_1_bss_974->unk148 = 3;
                    }
                    tbl = &lbl_1_data_6C8[4];
                    DECIDE_TEAM_SHUFFLE(tbl, (s8)lbl_1_bss_974->unk148);
                    fn_1_88CC();
                    DECIDE_WAIT10();
                    DECIDE_BACK_TO_IDLE();
                    break;
                case 10:
                    HuAudFXPlay(0);
                    lbl_1_bss_974->unk148++;
                    if (lbl_1_bss_974->unk148 >= 4) {
                        lbl_1_bss_974->unk148 = 0;
                    }
                    tbl = &lbl_1_data_6C8[4];
                    DECIDE_TEAM_SHUFFLE(tbl, (s8)lbl_1_bss_974->unk148);
                    fn_1_8B98();
                    DECIDE_WAIT10();
                    DECIDE_BACK_TO_IDLE();
                    break;
                case 11:
                    lbl_1_bss_974->unk148--;
                    if (lbl_1_bss_974->unk148 < 0) {
                        lbl_1_bss_974->unk148 = 5;
                    }
                    HuAudFXPlay(0);
                    for (i = 0; i < 4; i++) {
                        lbl_1_bss_974->list[i] = lbl_1_data_780[(s8)lbl_1_bss_974->unk148 + 7][i];
                    }
                    tbl = &lbl_1_data_6C8[8];
                    for (k = 0; k < 2; k++) {
                        m = lbl_1_bss_974->list[k];
                        HuSprAttrReset(lbl_1_bss_974->id[28], m + 2, 4);
                        HuSprAttrReset(lbl_1_bss_974->id[28], m + 7, 4);
                        HuSprPosSet(lbl_1_bss_974->id[28], m + 2, tbl[k].x, tbl[k].y);
                        HuSprPosSet(lbl_1_bss_974->id[28], m + 7, tbl[k].x, 33.0f + tbl[k].y);
                    }
                    for (k = 2; k < 4; k++) {
                        m = lbl_1_bss_974->list[k];
                        HuSprAttrSet(lbl_1_bss_974->id[28], m + 2, 4);
                        HuSprAttrSet(lbl_1_bss_974->id[28], m + 7, 4);
                    }
                    fn_1_88CC();
                    DECIDE_WAIT10();
                    DECIDE_BACK_TO_IDLE();
                    break;
                case 12:
                    HuAudFXPlay(0);
                    lbl_1_bss_974->unk148++;
                    if (lbl_1_bss_974->unk148 >= 6) {
                        lbl_1_bss_974->unk148 = 0;
                    }
                    for (i = 0; i < 4; i++) {
                        lbl_1_bss_974->list[i] = lbl_1_data_780[(s8)lbl_1_bss_974->unk148 + 7][i];
                    }
                    tbl = &lbl_1_data_6C8[8];
                    for (k = 0; k < 2; k++) {
                        m = lbl_1_bss_974->list[k];
                        HuSprAttrReset(lbl_1_bss_974->id[28], m + 2, 4);
                        HuSprAttrReset(lbl_1_bss_974->id[28], m + 7, 4);
                        HuSprPosSet(lbl_1_bss_974->id[28], m + 2, tbl[k].x, tbl[k].y);
                        HuSprPosSet(lbl_1_bss_974->id[28], m + 7, tbl[k].x, 33.0f + tbl[k].y);
                    }
                    for (k = 2; k < 4; k++) {
                        m = lbl_1_bss_974->list[k];
                        HuSprAttrSet(lbl_1_bss_974->id[28], m + 2, 4);
                        HuSprAttrSet(lbl_1_bss_974->id[28], m + 7, 4);
                    }
                    fn_1_8B98();
                    DECIDE_WAIT10();
                    DECIDE_BACK_TO_IDLE();
                    break;
                case 13:
                    lbl_1_bss_974->sel--;
                    if (GwCommon.m562VeryHardUnlock != 0) {
                        if ((s8)lbl_1_bss_974->sel < 0) {
                            lbl_1_bss_974->sel = 3;
                        }
                    } else if ((s8)lbl_1_bss_974->sel < 0) {
                        lbl_1_bss_974->sel = 2;
                    }
                    HuAudFXPlay(0);
                    for (k = 0; k < 4; k++) {
                        HuSprAttrSet(lbl_1_bss_974->id[28], k + 0xB, 4);
                    }
                    HuSprAttrReset(lbl_1_bss_974->id[28], (s8)lbl_1_bss_974->sel + 0xB, 4);
                    fn_1_8E64();
                    DECIDE_WAIT10();
                    DECIDE_BACK_TO_IDLE();
                    break;
                case 14:
                    lbl_1_bss_974->sel++;
                    if (GwCommon.m562VeryHardUnlock != 0) {
                        if ((s8)lbl_1_bss_974->sel >= 4) {
                            lbl_1_bss_974->sel = 0;
                        }
                    } else if ((s8)lbl_1_bss_974->sel >= 3) {
                        lbl_1_bss_974->sel = 0;
                    }
                    HuAudFXPlay(0);
                    for (k = 0; k < 4; k++) {
                        HuSprAttrSet(lbl_1_bss_974->id[28], k + 0xB, 4);
                    }
                    HuSprAttrReset(lbl_1_bss_974->id[28], (s8)lbl_1_bss_974->sel + 0xB, 4);
                    fn_1_9130();
                    DECIDE_WAIT10();
                    DECIDE_BACK_TO_IDLE();
                    break;
                case 15:
                    if ((s8)lbl_1_bss_974->sel > 0) {
                        HuAudFXPlay(0);
                        lbl_1_bss_974->sel--;
                        DECIDE_TAB_SETUP(30, 20);
                        fn_1_15B8();
                        DECIDE_TAB_PULSE(30);
                    }
                    DECIDE_BACK_TO_IDLE();
                    break;
                case 16:
                    if ((s8)lbl_1_bss_974->sel < 2) {
                        HuAudFXPlay(0);
                        lbl_1_bss_974->sel++;
                        DECIDE_TAB_SETUP(30, 20);
                        fn_1_15B8();
                        DECIDE_TAB_PULSE(30);
                    }
                    DECIDE_BACK_TO_IDLE();
                    break;
                case 17:
                    if ((s8)lbl_1_bss_974->sel > 0) {
                        HuAudFXPlay(0);
                        lbl_1_bss_974->sel--;
                        DECIDE_TAB_SETUP(31, 23);
                        DECIDE_TAB_PULSE(31);
                    }
                    DECIDE_BACK_TO_IDLE();
                    break;
                case 18:
                    if ((s8)lbl_1_bss_974->sel < 2) {
                        HuAudFXPlay(0);
                        lbl_1_bss_974->sel++;
                        DECIDE_TAB_SETUP(31, 23);
                        DECIDE_TAB_PULSE(31);
                    }
                    DECIDE_BACK_TO_IDLE();
                    break;
                case 19:
                    mgIdx = lbl_1_bss_974->entry[lbl_1_bss_974->idx].mgNo;
                    mg = &mgDataTbl[mgIdx];
                    mgNo = lbl_1_bss_974->entry[lbl_1_bss_974->idx].mgNo + 0x1F5;
                    switch (mg->type) {
                        case 0:
                        case 3:
                        case 4:
                        case 7:
                        case 8:
                            break;
                        case 2:
                            fn_1_5A0C();
                            break;
                        case 1:
                            fn_1_5E04();
                            break;
                        case 6:
                            fn_1_6204();
                            break;
                        case 5:
                            fn_1_6660();
                            break;
                    }
                    DECIDE_BACK_TO_IDLE();
                    break;
                case 20:
                    if (lbl_1_bss_974->entry[lbl_1_bss_974->idx].enable == 0) {
                        mgIdx = -1;
                    } else {
                        mgIdx = lbl_1_bss_974->entry[lbl_1_bss_974->idx].mgNo;
                    }
                    if (mgIdx + 0x1F5 == 0x207) {
                        fn_1_5330();
                    } else {
                        if (lbl_1_bss_974->entry[lbl_1_bss_974->idx].enable == 0) {
                            mgIdx = -1;
                        } else {
                            mgIdx = lbl_1_bss_974->entry[lbl_1_bss_974->idx].mgNo;
                        }
                        if (mgIdx + 0x1F5 == 0x228) {
                            fn_1_56A0();
                        }
                    }
                    DECIDE_BACK_TO_IDLE();
                    break;
                case 21:
                    HuSprGrpPosSet(lbl_1_bss_974->id[28], lbl_1_data_608[2], lbl_1_data_608[3]);
                    HuSprTPLvlSet(lbl_1_bss_974->id[29], 0, 1.0f);
                    HuSprTPLvlSet(lbl_1_bss_974->id[29], 1, 1.0f);
                    HuSprTPLvlSet(lbl_1_bss_974->id[29], 2, 0.0f);
                    HuSprTPLvlSet(lbl_1_bss_974->id[29], 3, 0.0f);
                    HuWinDispOff(lbl_1_bss_974->id[19]);
                    DECIDE_BACK_TO_IDLE();
                    break;
                case 22:
                    i = 3;
                    while (i--) {
                        HuWinDispOff(lbl_1_bss_974->id[20 + i]);
                    }
                    HuSprGrpPosSet(lbl_1_bss_974->id[30], lbl_1_data_628[2], lbl_1_data_628[3]);
                    i = 3;
                    while (i--) {
                        HuWinDispOff(lbl_1_bss_974->id[23 + i]);
                    }
                    HuSprGrpPosSet(lbl_1_bss_974->id[31], lbl_1_data_628[2], lbl_1_data_628[3]);
                    DECIDE_BACK_TO_IDLE();
                    break;
            }
        }
        HuPrcVSleep();
    }
}
