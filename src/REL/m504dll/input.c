#define _MATH_H

#include "REL/m504dll.h"
#include "string.h"

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

extern void fn_1_BC54(INPUTWORK *work);
extern void fn_1_BC84(INPUTWORK *work);
void fn_1_BAC4(INPUTWORK *work, INPUTMAIN *main);

void fn_1_B828(INPUTMAIN *main, s32 padNo, PADOWNER *playerConf)
{
    memset(main, 0, sizeof(INPUTMAIN));
    main->playerConf = playerConf;
    main->padId = GwPlayerConf[padNo].padNo;
    fn_1_BAC4(&main->work, main);
}

void fn_1_B898(INPUTMAIN *main)
{
    fn_1_BC54(&main->work);
    memset(main, 0, sizeof(INPUTMAIN));
}

void fn_1_B8D8(INPUTMAIN *main)
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

void fn_1_BAC4(INPUTWORK *work, INPUTMAIN *main)
{
    register s32 i;
    register s32 playerNo;
    register s32 v1;
    register s32 v2;
    register s32 v3;
    register void **t1;
    register void **t2;
    register void **t3;

    playerNo = main->playerConf->unk0->playerNo;

    memset(work, 0, 0x24);
    work->main = main;

    work->type = GwPlayerConf[playerNo].type;

    if (GwPlayerConf[playerNo].dif == 3) {
        v1 = 0;
    } else {
        if (GwPlayerConf[playerNo].dif == 2) {
            v2 = 1;
        } else {
            if (GwPlayerConf[playerNo].dif == 1) {
                v3 = 2;
            } else {
                v3 = 3;
            }
            v2 = v3;
        }
        v1 = v2;
    }
    work->dif = v1;

    if (work->dif == 0) {
        t1 = lbl_1_data_2E14;
    } else {
        if (work->dif == 1) {
            t2 = lbl_1_data_2E38;
        } else {
            if (work->dif == 2) {
                t3 = lbl_1_data_2E74;
            } else {
                t3 = lbl_1_data_2EB0;
            }
            t2 = t3;
        }
        t1 = t2;
    }
    work->dataTable = t1;

    for (i = 0; lbl_1_data_4134[i] != NULL; i++) {
        *(u16 *)(lbl_1_data_4134[i] + 0xA) = 0;
    }
}
