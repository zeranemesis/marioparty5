#include "REL/m504dll.h"
#include "string.h"

typedef struct InputWork_s {
    struct InputMain_s *main;
    s32 unk4;
    s32 unk8;
    void *dataTable;
    s32 unk10;
    s16 charNo;
    s16 type;
} InputWork;

typedef struct InputMain_s {
    GWPLAYERCONF *playerConf;
    InputWork work;
    s32 pad1[3];
    s32 padId;
    u16 btn;
    u16 btnDown;
    s8 stkX;
    s8 stkY;
    u16 btnRep;
    s8 subStkX;
    s8 subStkY;
    u8 trigL;
    u8 trigR;
    u8 dStk;
    u8 dStkRep;
    s8 pad2[6];
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

extern void *lbl_1_data_2E14;
extern void *lbl_1_data_2E38;
extern void *lbl_1_data_2E74;
extern void *lbl_1_data_2EB0;
extern void **lbl_1_data_4134;

extern void fn_1_BC54(InputWork *work);
extern void fn_1_BC84(InputWork *work);
void fn_1_BAC4(InputWork *work, InputMain *main);

void fn_1_B828(InputMain *main, s32 padNo, GWPLAYERCONF *playerConf)
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
    s32 padId;
    s8 stkX;
    s8 stkY;
    s8 absStkX;
    s8 absStkY;

    if (main->work.unk4 != 0) {
        fn_1_BC84(&main->work);
        return;
    }

    padId = main->padId;
    main->btn = HuPadBtn[padId];
    main->btnDown = HuPadBtnDown[padId];
    main->btnRep = HuPadBtnRep[padId];

    stkX = HuPadStkX[padId];
    if (stkX < 0) {
        absStkX = -stkX;
    } else {
        absStkX = stkX;
    }
    if (absStkX >= 10) {
        main->stkX = stkX;
    } else {
        main->stkX = 0;
    }

    stkY = HuPadStkY[padId];
    if (stkY < 0) {
        absStkY = -stkY;
    } else {
        absStkY = stkY;
    }
    if (absStkY >= 10) {
        main->stkY = stkY;
    } else {
        main->stkY = 0;
    }

    main->subStkX = HuPadSubStkX[padId];
    main->subStkY = HuPadSubStkY[padId];
    main->trigL = HuPadTrigL[padId];
    main->trigR = HuPadTrigR[padId];
    main->dStk = HuPadDStk[padId];
    main->dStkRep = HuPadDStkRep[padId];
}

void fn_1_BAC4(InputWork *work, InputMain *main)
{
    GWPLAYERCONF *playerConf;
    s32 playerIdx;
    GWPLAYERCONF *conf;
    s16 type;
    s16 workType;
    void **ptr;
    u16 *target;

    memset(work, 0, 0x24);
    work->main = main;

    playerConf = main->playerConf;
    playerIdx = playerConf->dif;
    conf = &GwPlayerConf[playerIdx];

    work->charNo = conf->charNo;

    type = conf->dif;
    if (type == 3) {
        workType = 0;
    } else if (type == 2) {
        workType = 1;
    } else if (type == 1) {
        workType = 2;
    } else {
        workType = 3;
    }
    work->type = workType;

    switch (workType) {
        case 0:
            work->dataTable = &lbl_1_data_2E14;
            break;
        case 1:
            work->dataTable = &lbl_1_data_2E38;
            break;
        case 2:
            work->dataTable = &lbl_1_data_2E74;
            break;
        case 3:
            work->dataTable = &lbl_1_data_2EB0;
            break;
    }

    ptr = lbl_1_data_4134;
    while (*ptr != NULL) {
        target = (u16 *)*ptr + 5;
        *target = 0;
        ptr++;
    }
}