#include "types.h"

#include "game/charman.h"

typedef struct M542Chr_s {
    s16 modelId;         /* 0x00 */
    u8 pad2[0x4];
    s16 charNo;          /* 0x06 */
    u8 pad8[0x14];
} M542Chr;

typedef struct M542StageChr_s {
    s16 unk0;
    s16 unk2;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    u32 unk18;
    u32 unk1C;
    u32 unk20;
} M542StageChr;

typedef struct M542StageWork_s {
    M542Chr chr[2];              /* 0x00 */
    u32 unk38;
    M542Chr *chrTbl;
    u32 *dataNum[2];             /* 0x40 */
    s16 *motId[2];               /* 0x48 */
    M542StageChr stageChr[2];    /* 0x50 */
    u8 pad98[0xBC];
} M542StageWork;

M542StageWork *lbl_1_bss_1B38;

float Hu3DMotionMaxTimeGet(HU3DMODELID modelId);

void fn_1_89DC(void *arg0, void *arg1)
{
    lbl_1_bss_1B38->dataNum[0] = arg0;
    lbl_1_bss_1B38->dataNum[1] = arg1;
}

void fn_1_8A00(void *arg0, void *arg1)
{
    lbl_1_bss_1B38->motId[0] = arg0;
    lbl_1_bss_1B38->motId[1] = arg1;
}

void fn_1_9150(void)
{
    s16 i;
    s16 j;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 9; j++) {
            lbl_1_bss_1B38->motId[i][j] = CharMotionCreate(lbl_1_bss_1B38->chr[i].charNo, lbl_1_bss_1B38->dataNum[i][j]);
        }
    }
    for (i = 0; i < 2; i++) {
        lbl_1_bss_1B38->stageChr[i].unk2 = -1;
        lbl_1_bss_1B38->stageChr[i].unk4 = 1.0f;
        lbl_1_bss_1B38->stageChr[i].unk8 = 1.0f;
    }
}

void fn_1_92A4(void)
{
    s16 i;
    M542Chr *chrTbl;
    M542StageChr *chr;
    s16 idx;

    for (i = 0; i < 2; i++) {
        chr = &lbl_1_bss_1B38->stageChr[i];
        idx = i + 0x20;
        chrTbl = lbl_1_bss_1B38->chrTbl;

        if (chr->unk0 != chr->unk2) {
            Hu3DMotionSet(chrTbl[idx].modelId, lbl_1_bss_1B38->motId[i][chr->unk0]);
            chr->unk2 = chr->unk0;
            chr->unk14 = Hu3DMotionMaxTimeGet(chrTbl[idx].modelId);
        }
        if (chr->unk4 != chr->unk8) {
            Hu3DMotionSpeedSet(chrTbl[idx].modelId, chr->unk4);
            chr->unk8 = chr->unk4;
        }
        if (chr->unk18 != chr->unk1C) {
            Hu3DModelAttrSet(chrTbl[idx].modelId, chr->unk18);
            chr->unk1C = chr->unk18;
        } else if (chr->unk20 != 0) {            Hu3DModelAttrReset(chrTbl[idx].modelId, chr->unk20);
            chr->unk20 = 0;
            chr->unk18 = 0;
            chr->unk1C = 0;
        }
        if (chr->unkC != chr->unk10) {
            Hu3DMotionTimeSet(chrTbl[idx].modelId, chr->unkC);
        }
        chr->unkC = Hu3DMotionTimeGet(chrTbl[idx].modelId);
        chr->unk10 = chr->unkC;
    }
}

void fn_1_94B4(void)
{
}