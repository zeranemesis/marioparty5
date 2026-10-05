#include "REL/m553Dll.h"

typedef struct StageObjWork_s {
    HU3DMODELID modelId;
    s32 unk04;
    s32 unk08;
    s32 unk0C;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    s32 unk54;
    void *unk58;
    s32 unk5C[0x2D8];
} StageObjWork;

typedef struct StageWork_s {
    s32 unk00[0x16];
    StageObjWork *objWork;
    s32 unk5C[0x17];
    s32 unkF0;
    s32 unkF4[0x1D];
    s32 unk168;
    s32 unk16C[0x182];
    s32 unk770;
    s32 unk774[0x12];
    s32 unk7B8;
} StageWork;

extern StageWork *lbl_1_bss_1F00;

extern void (*lbl_1_data_3C0[6])(void);

void fn_1_F290(void)
{
    lbl_1_bss_1F00->unk770 = -1;
}

void fn_1_E234(s16 index, s32 value)
{
    StageObjWork *objWork = lbl_1_bss_1F00->objWork;
    objWork->unk5C[index] = value;
}

void fn_1_E254(s16 index, s32 value)
{
    StageObjWork *objWork = lbl_1_bss_1F00->objWork;
    objWork->unk5C[index + 4] = value;
}

void fn_1_10270(void)
{
    StageWork *stageWork = lbl_1_bss_1F00;
    StageObjWork *objWork = stageWork->objWork;

    stageWork->unk770 = -1;

    Hu3DModelAttrSet(objWork->unk5C[0x309], 1);
}

void fn_1_10504(void)
{
    StageWork *stageWork = lbl_1_bss_1F00;
    StageObjWork *objWork = stageWork->objWork;

    Hu3DModelAttrSet(objWork->unk5C[0x310], 1);

    stageWork->unk7B8 = -1;
}

void fn_1_EC64(void)
{
    int i;
    for (i = 0; i < 6; i++) {
        lbl_1_data_3C0[i]();
    }
}