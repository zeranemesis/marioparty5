#include "REL/m504dll.h"
#include "string.h"

typedef struct InputWork_s {
    struct InputMain_s *main;
    s32 unk4;
    void **dataTable;
    s32 unkC;
    s32 type;
    u16 dif;
    u8 unk18[14];
} InputWork;

typedef struct PadOwner_s {
    struct PadOwner_s *unk0;
    s32 playerNo;
} PadOwner;

typedef struct InputMain_s {
    PadOwner *playerConf;
    InputWork work;
    s32 padId;
    s32 btn;
    s32 btnDown;
    s8 stkX;
    s8 stkY;
    u16 btnRep;
    s8 subStkX;
    s8 subStkY;
    u8 trigL;
    u8 trigR;
    u8 dStk;
    u8 dStkRep;
} InputMain;

extern GWPLAYERCONF GwPlayerConf[];
extern u16 HuPadBtn[];
extern u16 HuPadBtnDown[];
extern u16 HuPadBtnRep[];
extern s8 HuPadStkX[];
extern s8 HuPadStkY[];
extern s8 HuPadSubStkX[];
extern s8 HuPadSubStkY[];
extern u8 HuPadTrigL[];
extern u8 HuPadTrigR[];
extern u8 HuPadDStk[];
extern u8 HuPadDStkRep[];

extern void *lbl_1_data_2E14[];
extern void *lbl_1_data_2E38[];
extern void *lbl_1_data_2E74[];
extern void *lbl_1_data_2EB0[];
extern u8 *lbl_1_data_4134[];

extern void fn_1_BC54(InputWork *work);
extern void fn_1_BC84(InputWork *work);
void fn_1_BAC4(InputWork *work, InputMain *main);

void fn_1_B828(InputMain *main, s32 padNo, PadOwner *playerConf)
{
    memset(main, 0, sizeof(InputMain));
    main->playerConf = playerConf;
    main->padId = GwPlayerConf[padNo].padNo;
    fn_1_BAC4(&main->work, main);
}

void fn_1_B898(InputMain *main)
{
    fn_1_BC54(&main->work);
    memset(main, 0, sizeof(InputMain));
}

void fn_1_B8D8(InputMain *main)
{
    register s32 absStkX;
    register s32 stkXv;
    register s32 absStkY;
    register s32 stkYv;

    if (main->work.type != 0) {
        fn_1_BC84(&main->work);
        return;
    }

    main->btn = HuPadBtn[main->padId];
    main->btnDown = HuPadBtnDown[main->padId];
    main->btnRep = HuPadBtnRep[main->padId];

    main->stkX = HuPadStkX[main->padId];
    if (main->stkX >= 0) {
        absStkX = main->stkX;
    } else {
        absStkX = -main->stkX;
    }
    if (absStkX < 10) {
        stkXv = 0;
    } else {
        stkXv = main->stkX;
    }
    main->stkX = stkXv;

    main->stkY = HuPadStkY[main->padId];
    if (main->stkY >= 0) {
        absStkY = main->stkY;
    } else {
        absStkY = -main->stkY;
    }
    if (absStkY < 10) {
        stkYv = 0;
    } else {
        stkYv = main->stkY;
    }
    main->stkY = stkYv;

    main->subStkX = HuPadSubStkX[main->padId];
    main->subStkY = HuPadSubStkY[main->padId];
    main->trigL = HuPadTrigL[main->padId];
    main->trigR = HuPadTrigR[main->padId];
    main->dStk = HuPadDStk[main->padId];
    main->dStkRep = HuPadDStkRep[main->padId];
}

void fn_1_BAC4(InputWork *work, InputMain *main)
{
    s32 playerNo;
    s32 dif;
    s32 i;
    void **table;

    playerNo = main->playerConf->unk0->playerNo;

    memset(work, 0, 0x24);
    work->main = main;

    work->type = GwPlayerConf[playerNo].type;

    if (GwPlayerConf[playerNo].dif == 3) {
        dif = 0;
    } else if (GwPlayerConf[playerNo].dif == 2) {
        dif = 1;
    } else if (GwPlayerConf[playerNo].dif == 1) {
        dif = 2;
    } else {
        dif = 3;
    }
    work->dif = dif;

    switch (work->dif) {
        case 0:
            table = lbl_1_data_2E14;
            break;
        case 1:
            table = lbl_1_data_2E38;
            break;
        case 2:
            table = lbl_1_data_2E74;
            break;
        case 3:
            table = lbl_1_data_2EB0;
            break;
    }
    work->dataTable = table;

    for (i = 0; lbl_1_data_4134[i] != NULL; i++) {
        *(u16 *)(lbl_1_data_4134[i] + 0xA) = 0;
    }
}