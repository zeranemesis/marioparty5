#include "REL/mdoptionDll/mdoption.h"
#include "game/audio.h"
#include "game/gamework.h"
#include "string.h"

/*************************************************************
 * record.c
 *
 * Option mode "sound test" (record) menu: category/entry list
 * build, cursor movement, list window refresh and sound playback.
 *************************************************************/

#define MDREC_SND_BGM 0
#define MDREC_SND_FX 1
#define MDREC_SND_JINGLE 2

typedef struct MdRecSnd_s {
    s16 type; //0x00 MDREC_SND_*, -1 ends the table
    s16 id; //0x02 stream/sound id
    int mess; //0x04 name message id
} MDREC_SND;

typedef struct MdRecCategory_s {
    int cursor; //0x00 initial cursor
    int num; //0x04 entry count
    MDREC_SND *snd; //0x08
    int title; //0x0C title message
    int insert; //0x10 inserted message, -1 for none
    int enableF; //0x14
} MDREC_CATEGORY;

typedef struct MdRecWork_s {
    int category; //0x00
    int top; //0x04 first listed entry
    int cursor; //0x08
    int categoryNum; //0x0C
    int sndType; //0x10 type of the playing sound
    int fxId; //0x14
    int streamId; //0x18
} MDREC_WORK;

extern void MdMessHelpWinCreate(MDMESS_WORK *mess, int winNo, int msg, float x, float y, int w, int h, float z, int type);
extern void MdMessWinAttrSet(MDMESS_WORK *mess, int winNo, u32 attr);
extern void MdMessWinClose(MDMESS_WORK *mess, int winNo);
extern void MdMessWinHomeClear(MDMESS_WORK *mess, int winNo);
extern void MdMessWinInsertMesSet(MDMESS_WORK *mess, int winNo, int msg, int insert);
extern void MdMessWinInsertMultiMesSet(MDMESS_WORK *mess, int winNo, int msg, int *list);
extern void MdMessWinMesSet(MDMESS_WORK *mess, int winNo, int msg);
extern void MdMessWinPosSet(MDMESS_WORK *mess, int winNo, float x, float y);
extern void MdMessWinScaleSet(MDMESS_WORK *mess, int winNo, float x, float y);
extern void MdMessWinSpeedSet(MDMESS_WORK *mess, int winNo, s16 speed);
extern HuVecF *MdSpriteGrpPosGet(MDSPRITE_WORK *sprite, int grpNo);
extern HuVecF *MdSpriteGrpScaleGet(MDSPRITE_WORK *sprite, int grpNo);
extern void fn_1_A930(int cmd);
void fn_1_1B418(void);

MDREC_SND lbl_1_data_2AD0[5] = {
    { 0, 2, 0x21001D },
    { 0, 69, 0x210020 },
    { 0, 3, 0x21001E },
    { 0, 4, 0x21001F },
    { -1, -1, -1 },
};

MDREC_SND lbl_1_data_2AF8[15] = {
    { 0, 13, 0x210029 },
    { 0, 12, 0x210028 },
    { 0, 5, 0x210021 },
    { 0, 6, 0x210022 },
    { 0, 7, 0x210023 },
    { 0, 8, 0x210024 },
    { 0, 9, 0x210025 },
    { 0, 10, 0x210026 },
    { 0, 11, 0x210027 },
    { 0, 21, 0x210031 },
    { 0, 70, 0x210032 },
    { 0, 71, 0x210033 },
    { -1, -1, -1 },
    { -1, -1, -1 },
    { -1, -1, -1 },
};

MDREC_SND lbl_1_data_2B70[15] = {
    { 0, 14, 0x21002A },
    { 0, 15, 0x21002B },
    { 0, 16, 0x21002C },
    { 0, 17, 0x21002D },
    { 0, 18, 0x21002E },
    { 0, 19, 0x21002F },
    { 0, 76, 0x210034 },
    { 0, 20, 0x210030 },
    { 2, 127, 0x210036 },
    { 2, 128, 0x210037 },
    { 2, 132, 0x210038 },
    { 2, 138, 0x210039 },
    { 2, 139, 0x21003A },
    { -1, -1, -1 },
    { -1, -1, -1 },
};

MDREC_SND lbl_1_data_2BE8[15] = {
    { 0, 28, 0x210046 },
    { 0, 29, 0x210042 },
    { 0, 24, 0x21003D },
    { 0, 25, 0x21003E },
    { 0, 26, 0x21003F },
    { 2, 140, 0x210045 },
    { 0, 30, 0x210041 },
    { 0, 64, 0x21005B },
    { 0, 87, 0x210061 },
    { 0, 65, 0x21005C },
    { 2, 124, 0x210065 },
    { 0, 31, 0x210043 },
    { 0, 32, 0x210044 },
    { 0, 27, 0x210040 },
    { -1, -1, -1 },
};

MDREC_SND lbl_1_data_2C60[15] = {
    { 0, 48, 0x21004B },
    { 0, 49, 0x21004C },
    { 0, 50, 0x21004D },
    { 0, 51, 0x21004E },
    { 0, 52, 0x21004F },
    { 0, 53, 0x210050 },
    { 0, 54, 0x210051 },
    { 0, 55, 0x210052 },
    { 0, 60, 0x210057 },
    { 0, 57, 0x210054 },
    { 0, 58, 0x210055 },
    { 0, 62, 0x210059 },
    { 2, 131, 0x210066 },
    { 0, 59, 0x210056 },
    { -1, -1, -1 },
};

MDREC_SND lbl_1_data_2CD8[15] = {
    { 0, 63, 0x21005A },
    { 0, 61, 0x210058 },
    { 0, 66, 0x21005D },
    { 0, 67, 0x21005E },
    { 0, 68, 0x21005F },
    { 0, 75, 0x210060 },
    { 2, 121, 0x210062 },
    { 2, 122, 0x210063 },
    { 2, 123, 0x210064 },
    { 2, 134, 0x210068 },
    { -1, -1, -1 },
    { -1, -1, -1 },
    { -1, -1, -1 },
    { -1, -1, -1 },
    { -1, -1, -1 },
};

MDREC_SND lbl_1_data_2D50[15] = {
    { 0, 35, 0x210069 },
    { 0, 36, 0x21006A },
    { 0, 37, 0x21006B },
    { 0, 38, 0x21006C },
    { 0, 72, 0x21006D },
    { 2, 129, 0x21006E },
    { 2, 135, 0x21006F },
    { 2, 136, 0x210070 },
    { 0, 39, 0x210071 },
    { 0, 40, 0x210072 },
    { -1, -1, -1 },
    { -1, -1, -1 },
    { -1, -1, -1 },
    { -1, -1, -1 },
    { -1, -1, -1 },
};

MDREC_SND lbl_1_data_2DC8[15] = {
    { 0, 41, 0x210075 },
    { 2, 126, 0x21007C },
    { 2, 133, 0x210067 },
    { 0, 44, 0x210077 },
    { 0, 45, 0x210078 },
    { 0, 46, 0x210079 },
    { 2, 137, 0x21007B },
    { 0, 43, 0x210076 },
    { 0, 74, 0x21007A },
    { -1, -1, -1 },
    { -1, -1, -1 },
    { -1, -1, -1 },
    { -1, -1, -1 },
    { -1, -1, -1 },
    { -1, -1, -1 },
};

MDREC_SND lbl_1_data_2E40[15] = {
    { 1, 502, 0x210083 },
    { 1, 503, 0x210084 },
    { 1, 504, 0x210085 },
    { 1, 505, 0x210086 },
    { 1, 506, 0x210087 },
    { 1, 507, 0x210088 },
    { 1, 508, 0x210089 },
    { 1, 509, 0x21008A },
    { 1, 511, 0x21008B },
    { 1, 512, 0x21008C },
    { 1, 513, 0x21008D },
    { 1, 514, 0x21008E },
    { -1, -1, -1 },
    { -1, -1, -1 },
    { -1, -1, -1 },
};

MDREC_SND lbl_1_data_2EB8[15] = {
    { 1, 713, 0x2100BB },
    { 1, 714, 0x2100BC },
    { 1, 715, 0x2100BD },
    { 1, 716, 0x2100BE },
    { -1, -1, -1 },
    { -1, -1, -1 },
    { -1, -1, -1 },
    { -1, -1, -1 },
    { -1, -1, -1 },
    { -1, -1, -1 },
    { -1, -1, -1 },
    { -1, -1, -1 },
    { -1, -1, -1 },
    { -1, -1, -1 },
    { -1, -1, -1 },
};

MDREC_SND lbl_1_data_2F30[15] = {
    { 1, 731, 0x210091 },
    { 1, 732, 0x210092 },
    { 1, 733, 0x210093 },
    { 1, 719, 0x210094 },
    { 1, 720, 0x210095 },
    { 1, 721, 0x210096 },
    { 1, 724, 0x210097 },
    { 1, 725, 0x210098 },
    { 1, 726, 0x210099 },
    { 1, 708, 0x21009A },
    { 1, 709, 0x21009B },
    { 1, 710, 0x21009C },
    { -1, -1, -1 },
    { -1, -1, -1 },
    { -1, -1, -1 },
};

MDREC_SND lbl_1_data_2FA8[15] = {
    { 1, 740, 0x21009F },
    { 1, 741, 0x2100A0 },
    { 1, 742, 0x2100A1 },
    { 1, 743, 0x2100A2 },
    { 1, 744, 0x2100A3 },
    { 1, 745, 0x2100A4 },
    { 1, 746, 0x2100A5 },
    { 1, 747, 0x2100A6 },
    { 1, 748, 0x2100A7 },
    { 1, 749, 0x2100A8 },
    { -1, -1, -1 },
    { -1, -1, -1 },
    { -1, -1, -1 },
    { -1, -1, -1 },
    { -1, -1, -1 },
};

MDREC_SND lbl_1_data_3020[15] = {
    { 1, 703, 0x2100AD },
    { 1, 705, 0x2100AE },
    { 1, 727, 0x2100AF },
    { 1, 728, 0x2100B0 },
    { 1, 799, 0x2100B1 },
    { 1, 775, 0x2100B2 },
    { -1, -1, -1 },
    { -1, -1, -1 },
    { -1, -1, -1 },
    { -1, -1, -1 },
    { -1, -1, -1 },
    { -1, -1, -1 },
    { -1, -1, -1 },
    { -1, -1, -1 },
    { -1, -1, -1 },
};

MDREC_SND lbl_1_data_3098[15] = {
    { 1, 25, 0x2100C9 },
    { 1, 26, 0x2100CA },
    { 1, 27, 0x2100CB },
    { 1, 28, 0x2100CC },
    { -1, -1, -1 },
    { -1, -1, -1 },
    { -1, -1, -1 },
    { -1, -1, -1 },
    { -1, -1, -1 },
    { -1, -1, -1 },
    { -1, -1, -1 },
    { -1, -1, -1 },
    { -1, -1, -1 },
    { -1, -1, -1 },
    { -1, -1, -1 },
};

MDREC_CATEGORY lbl_1_data_3110[24] = {    { 0, 0, lbl_1_data_2AD0, 0x210001,-1, 1 },
    { 0, 0, lbl_1_data_2AF8, 0x210002,0x210007, 1 },
    { 0, 0, lbl_1_data_2B70, 0x210002,0x210008, 1 },
    { 0, 0, lbl_1_data_2BE8, 0x210003,-1, 1 },
    { 0, 0, lbl_1_data_2C60, 0x210004,0x210007, 1 },
    { 0, 0, lbl_1_data_2CD8, 0x210004,0x210008, 1 },
    { 0, 0, lbl_1_data_2D50, 0x210004,0x210009, 1 },
    { 0, 0, lbl_1_data_2DC8, 0x210005,-1, 1 },
    { 0, 0, lbl_1_data_2E40, 0x210006,0x21000C, 1 },
    { 0, 0, lbl_1_data_2E40, 0x210006,0x21000D, 1 },
    { 0, 0, lbl_1_data_2E40, 0x210006,0x21000E, 1 },
    { 0, 0, lbl_1_data_2E40, 0x210006,0x21000F, 1 },
    { 0, 0, lbl_1_data_2E40, 0x210006,0x210010, 1 },
    { 0, 0, lbl_1_data_2E40, 0x210006,0x210011, 1 },
    { 0, 0, lbl_1_data_2E40, 0x210006,0x210012, 1 },
    { 0, 0, lbl_1_data_2E40, 0x210006,0x210013, 1 },
    { 0, 0, lbl_1_data_2E40, 0x210006,0x210014, 1 },
    { 0, 0, lbl_1_data_2E40, 0x210006,0x210015, 1 },
    { 0, 0, lbl_1_data_2EB8, 0x210006,0x210017, 1 },
    { 0, 0, lbl_1_data_2F30, 0x210006,0x210018, 1 },
    { 0, 0, lbl_1_data_2FA8, 0x210006,0x210019, 1 },
    { 0, 0, lbl_1_data_3020, 0x210006,0x21001A, 1 },
    { 0, 0, lbl_1_data_3098, 0x210006,0x21001B, 1 },
    { -1, 0, NULL, -1, -1, 0 },
};

s16 lbl_1_data_3350[10] = { 0x1F6, 0x1E2, 0x25A, 0x2AA, 0x282, 0x1BA, 0x296, 0x1CE, 0x26E, 0x232 };

MDREC_WORK lbl_1_bss_7EC;
MDREC_WORK *lbl_1_bss_7E8;

void fn_1_1A278(void)
{
    int i;
    int j;
    int sum;
    int cnt;
    MDREC_CATEGORY *category;

    memset(&lbl_1_bss_7EC, 0, sizeof(MDREC_WORK));
    lbl_1_bss_7E8 = &lbl_1_bss_7EC;
    for (i = 0; lbl_1_data_3110[i].cursor != -1; i++) {
        category = &lbl_1_data_3110[i];
        for (j = 0; category->snd[j].type != -1; j++) {
        }
        category->num = j;
    }
    lbl_1_bss_7E8->categoryNum = i;
    sum = i = 0;
    for (; i < 7; i++) {
        sum += GwCommon.storyPlayNum[i];
    }
    lbl_1_data_3110[3].enableF = (sum != 0) ? 1 : 0;
    i = 0;
    cnt = 0;
    for (; i < 7; i++) {
        if (GwCommon.playNum[i] != 0) {
            cnt++;
        }
    }
    lbl_1_data_3110[1].enableF = (cnt == 7) ? 1 : 0;
    lbl_1_data_3110[2].enableF = (cnt == 7) ? 1 : 0;
    lbl_1_data_3110[7].enableF = (GwSdCommon.battleEasyWinF + GwSdCommon.battleNormalWinF != 0) ? 1 : 0;
    lbl_1_bss_7E8->fxId = lbl_1_bss_7E8->streamId = -1;
}

void fn_1_1A468(int cmd)
{
    MDREC_CATEGORY *category = &lbl_1_data_3110[lbl_1_bss_7E8->category];
    int num = category->num;
    int prevCat;
    int nextCat;
    int prevCursor;

    /* shortcut: cmd/prevCursor register order differs from the original (r26/r27 swapped) */
    prevCursor = lbl_1_bss_7E8->cursor;
    switch (cmd) {
        case 0:
            if (lbl_1_bss_7E8->cursor >= 2) {
                lbl_1_bss_7E8->cursor -= 2;
            }
            break;
        case 1:
            if (lbl_1_bss_7E8->cursor < num - 2) {
                lbl_1_bss_7E8->cursor += 2;
            }
            break;
        case 2:
            if (lbl_1_bss_7E8->cursor % 2 != 0) {
                lbl_1_bss_7E8->cursor--;
            }
            break;
        case 3:
            if (lbl_1_bss_7E8->cursor % 2 == 0 && lbl_1_bss_7E8->cursor < num - 1) {
                lbl_1_bss_7E8->cursor++;
            }
            break;
        case 4:
            if (lbl_1_bss_7E8->category != 0) {
                prevCat = lbl_1_bss_7E8->category - 1;
            } else {
                prevCat = lbl_1_bss_7E8->categoryNum - 1;
            }
            lbl_1_bss_7E8->category = prevCat;
            lbl_1_bss_7E8->top = lbl_1_bss_7E8->cursor = lbl_1_data_3110[lbl_1_bss_7E8->category].cursor;
            break;
        case 5:
            if (lbl_1_bss_7E8->category < lbl_1_bss_7E8->categoryNum - 1) {
                nextCat = lbl_1_bss_7E8->category + 1;
            } else {
                nextCat = 0;
            }
            lbl_1_bss_7E8->category = nextCat;
            lbl_1_bss_7E8->top = lbl_1_bss_7E8->cursor = lbl_1_data_3110[lbl_1_bss_7E8->category].cursor;
            break;
    }
    if (lbl_1_bss_7E8->cursor < lbl_1_bss_7E8->top) {
        lbl_1_bss_7E8->top -= 2;
    }
    if (lbl_1_bss_7E8->cursor > lbl_1_bss_7E8->top + 13) {
        lbl_1_bss_7E8->top += 2;
    }
    if (cmd >= 4) {
        fn_1_A930(8);
    } else if (prevCursor != lbl_1_bss_7E8->cursor) {
        fn_1_A930(2);
    }
}
void fn_1_1A7C0(void)
{
    HuVecF *scale;
    MDMESS_WORK *mess;
    MDREC_CATEGORY *category;
    int i;
    s16 grp;
    HuVecF *pos;
    SYS *sys;
    MDREC_SND *snd;
    int d;
    MDSPRITE_WORK *sprite;

    sys = sysP;
    sprite = sys->sprite;
    grp = sprite->grpId[0];
    mess = sys->mess;
    pos = MdSpriteGrpPosGet(sys->sprite, 0);
    scale = MdSpriteGrpScaleGet(sysP->sprite, 0);
    category = &lbl_1_data_3110[lbl_1_bss_7E8->category];
    {
        int listL[8] = { -1, -1, -1, -1, -1, -1, -1, -1 };
        int listR[8] = { -1, -1, -1, -1, -1, -1, -1, -1 };
        float baseX = -116.0f;
        float baseY = -59.0f;
    HuSprAttrReset(grp, 3, 4);
    HuSprAttrReset(grp, 4, 4);
    HuSprAttrSet(grp, 5, 4);
    HuSprAttrSet(grp, 6, 4);
    HuSprAttrSet(grp, 7, 4);
    HuSprAttrSet(grp, 8, 4);
    d = lbl_1_bss_7E8->cursor - lbl_1_bss_7E8->top;
    HuSprAttrReset(grp, 2, 4);
    HuSprScaleSet(grp, 2, 1.0f, 1.0f);
    HuSprTPLvlSet(grp, 2, 0.75f);
    HuSprPosSet(grp, 2, baseX + 235.0f * (d % 2), baseY + 26.0f * (d / 2));
    MdMessWinAttrSet(mess, 2, 0x800);
    if ((u32)(category->insert + 0x10000) == 0xFFFF) {
        MdMessWinMesSet(mess, 2, category->title);
    } else {
        MdMessWinInsertMesSet(mess, 2, category->title, category->insert);
    }
    for (i = 0; i < 14; i++) {
        snd = &category->snd[lbl_1_bss_7E8->top + i];
        if (snd->type == -1) {
            break;
        }
        if (category->enableF != 0) {
            if (i % 2 == 0) {
                listL[i / 2] = snd->mess;
            } else {
                listR[i / 2] = snd->mess;
            }
        } else {
            if (i % 2 == 0) {
                listL[i / 2] = 0x1F0050;
            } else {
                listR[i / 2] = 0x1F0050;
            }
        }
    }
    MdMessWinClose(mess, 4);
    MdMessWinClose(mess, 5);
    MdMessHelpWinCreate(mess, 4, 0x1F003E, 58.0f, 116.0f, 0x104, 0xB4, 0.0f, 0);
    MdMessWinScaleSet(mess, 4, 0.85f * scale->x, scale->y);
    MdMessHelpWinCreate(mess, 5, 0x1F003E, 293.0f, 116.0f, 0x104, 0xB4, 0.0f, 0);
    MdMessWinScaleSet(mess, 5, 0.85f * scale->x, scale->y);
    MdMessWinInsertMultiMesSet(mess, 4, 0x1F003E, listL);
    MdMessWinSpeedSet(mess, 4, 0);
    MdMessWinPosSet(mess, 4, -230.0f * scale->x + pos->x, -80.0f * scale->y + pos->y);
    MdMessWinInsertMultiMesSet(mess, 5, 0x1F003E, listR);
    MdMessWinSpeedSet(mess, 5, 0);
    MdMessWinPosSet(mess, 5, 5.0f * scale->x + pos->x, -80.0f * scale->y + pos->y);
    }
}

void fn_1_1AD88(void)
{
    HuVecF *scale;
    MDMESS_WORK *mess;
    MDREC_CATEGORY *category;
    int i;
    s16 grp;
    HuVecF *pos;
    SYS *sys;
    MDREC_SND *snd;
    int d;
    MDSPRITE_WORK *sprite;

    sys = sysP;
    sprite = sys->sprite;
    grp = sprite->grpId[0];
    mess = sys->mess;
    pos = MdSpriteGrpPosGet(sys->sprite, 0);
    scale = MdSpriteGrpScaleGet(sysP->sprite, 0);
    category = &lbl_1_data_3110[lbl_1_bss_7E8->category];
    {
        int listL[8] = { -1, -1, -1, -1, -1, -1, -1, -1 };
        int listR[8] = { -1, -1, -1, -1, -1, -1, -1, -1 };
        float baseX = -116.0f;
        float baseY = -59.0f;
    HuSprAttrReset(grp, 3, 4);
    HuSprAttrReset(grp, 4, 4);
    HuSprAttrSet(grp, 5, 4);
    HuSprAttrSet(grp, 6, 4);
    HuSprAttrSet(grp, 7, 4);
    HuSprAttrSet(grp, 8, 4);
    d = lbl_1_bss_7E8->cursor - lbl_1_bss_7E8->top;
    HuSprPosSet(grp, 2, baseX + 235.0f * (d % 2), baseY + 26.0f * (d / 2));
    if ((u32)(category->insert + 0x10000) == 0xFFFF) {
        MdMessWinMesSet(mess, 2, category->title);
    } else {
        MdMessWinInsertMesSet(mess, 2, category->title, category->insert);
    }
    for (i = 0; i < 14; i++) {
        snd = &category->snd[lbl_1_bss_7E8->top + i];
        if (snd->type == -1) {
            break;
        }
        if (category->enableF != 0) {
            if (i % 2 == 0) {
                listL[i / 2] = snd->mess;
            } else {
                listR[i / 2] = snd->mess;
            }
        } else {
            if (i % 2 == 0) {
                listL[i / 2] = 0x1F0050;
            } else {
                listR[i / 2] = 0x1F0050;
            }
        }
    }
    MdMessWinHomeClear(mess, 4);
    MdMessWinInsertMultiMesSet(mess, 4, 0x1F003E, listL);
    MdMessWinPosSet(mess, 4, -230.0f * scale->x + pos->x, -80.0f * scale->y + pos->y);
    MdMessWinSpeedSet(mess, 4, 0);
    MdMessWinHomeClear(mess, 5);
    MdMessWinInsertMultiMesSet(mess, 5, 0x1F003E, listR);
    MdMessWinPosSet(mess, 5, 5.0f * scale->x + pos->x, -80.0f * scale->y + pos->y);
    MdMessWinSpeedSet(mess, 5, 0);
    MdMessWinScaleSet(mess, 4, 0.85f * scale->x, scale->y);
    MdMessWinScaleSet(mess, 5, 0.85f * scale->x, scale->y);
    }
}

void fn_1_1B26C(void)
{
    fn_1_1B418();
    fn_1_A930(0);
}

void fn_1_1B294(void)
{
    MDREC_CATEGORY *category = &lbl_1_data_3110[lbl_1_bss_7E8->category];
    MDREC_SND *snd = &category->snd[lbl_1_bss_7E8->cursor];
    s16 id;

    fn_1_1B418();
    if (category->enableF == 0) {
        HuAudFXPlay(4);
        return;
    }
    lbl_1_bss_7E8->sndType = snd->type;
    if (snd->type == MDREC_SND_BGM) {
        lbl_1_bss_7E8->streamId = HuAudBGMPlay(snd->id);
    } else if (snd->type == MDREC_SND_FX) {
        if (lbl_1_bss_7E8->category >= 8 && lbl_1_bss_7E8->category < 18) {
            id = snd->id + lbl_1_data_3350[lbl_1_bss_7E8->category - 8] - 0x1F6;
        } else {
            id = snd->id;
        }
        lbl_1_bss_7E8->fxId = HuAudFXPlay(id);
    } else if (snd->type == MDREC_SND_JINGLE) {
        lbl_1_bss_7E8->streamId = HuAudJinglePlay(snd->id);
    }
}

void fn_1_1B418(void)
{
    int i;

    if (lbl_1_bss_7E8->sndType == MDREC_SND_BGM || lbl_1_bss_7E8->sndType == MDREC_SND_JINGLE) {
        i = 0;
        HuAudSStreamAllFadeOut(100);
        if (lbl_1_bss_7E8->streamId != -1) {
            while (HuAudSStreamStatGet(lbl_1_bss_7E8->streamId) != 0) {
                if (i >= 30) {
                    HuAudSStreamAllStop();
                    break;
                }
                i++;
                HuPrcVSleep();
            }
        }
    } else if (lbl_1_bss_7E8->sndType == MDREC_SND_FX) {
        HuAudFXFadeOut(lbl_1_bss_7E8->fxId, 100);
        for (i = 0; i < 7; i++) {
            HuPrcVSleep();
        }
    }
    HuPrcVSleep();
}
