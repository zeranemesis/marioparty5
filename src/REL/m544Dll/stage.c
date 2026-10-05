#include "types.h"

#pragma pack(push, 2)

typedef struct {
    u8 pad0[0x3B8];
    s16 charNo;
} StageModel;

typedef struct {
    s16 pad0[3];
    s16 charNo;
    s16 pad1[10];
} StageChar;

typedef struct {
    s16 flag;
    s16 pad0[2];
    f32 scale;
    s16 pad1[37];
} StageEntry;

typedef struct {
    StageChar charTbl[2];
    u8 pad1[0x20];
    StageModel *mdlTab;
    s32 *charMotTbl[2];
    s32 *motNumTbl;
    s16 *motIdTbl[2];
    s16 *stageMotId;
    u8 pad2[2];
    StageEntry entryTbl[3];
    u8 pad3[0x5A];
    s32 field_1CC;
} StageData;

#pragma pack(pop)

extern StageData *lbl_1_bss_918;

s16 CharMotionCreate(s16 charNo, s32 dataNum);
s16 Hu3DJointMotion(s16 modelId, void *motionData);
void *HuDataSelHeapReadNum(s32 dataNum, s32 memNum, s32 heapId);

void fn_1_A3F0(s16 charNo, s32 *motData)
{
    lbl_1_bss_918->charMotTbl[charNo] = motData;
}

void fn_1_A410(s16 charNo, s16 *motId)
{
    lbl_1_bss_918->motIdTbl[charNo] = motId;
}

void fn_1_ADBC(void)
{
    s16 i;
    s16 j;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 7; j++) {
            lbl_1_bss_918->motIdTbl[i][j] =
                CharMotionCreate(lbl_1_bss_918->charTbl[i].charNo, lbl_1_bss_918->charMotTbl[i][j]);
        }
    }

    for (i = 0; i < 4; i++) {
        lbl_1_bss_918->stageMotId[i] =
            Hu3DJointMotion(lbl_1_bss_918->mdlTab->charNo,
                            HuDataSelHeapReadNum(lbl_1_bss_918->motNumTbl[i], 0x10000000, 2));
    }

    for (i = 0; i < 3; i++) {
        lbl_1_bss_918->entryTbl[i].flag = -1;
        lbl_1_bss_918->entryTbl[i].scale = -1.0f;
    }
}

void fn_1_B384(void);
void fn_1_B990(void);
void fn_1_BEDC(void);
void fn_1_C7C0(void);
void fn_1_D1CC(void);

void (*stageInitTbl[5])(void) = {
    fn_1_B384,
    fn_1_B990,
    fn_1_BEDC,
    fn_1_C7C0,
    fn_1_D1CC,
};

void fn_1_B318(void)
{
    s32 i;

    for (i = 0; i < 5; i++) {
        stageInitTbl[i]();
    }
}

void fn_1_B36C(void)
{
    lbl_1_bss_918->field_1CC = 0;
}