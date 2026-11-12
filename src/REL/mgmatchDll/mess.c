#include "REL/mgmatchDll.h"

#ifdef __MWERKS__
extern int sprintf();
#else
#include "stdio.h"
#endif

extern s16 fn_1_458(void);
extern void fn_1_46C(void);
extern s32 fn_1_510(void);
extern u32 fn_1_528(void);
extern s8 fn_1_588(void);
extern s8 fn_1_59C(void);
extern s8 fn_1_5B0(s32 no);
extern s8 fn_1_624(void);
extern s32 fn_1_638(void);
extern void fn_1_64C(s32 *a, s32 *b, s32 c);
extern void fn_1_87C(u32 arg0);
extern void fn_1_898(u32 arg0);
extern void fn_1_924(u32 arg0);
extern void fn_1_940(u32 arg0);
extern int HuAudFXPlayPan(int seId, s16 pan);
extern int HuAudFXPlay(int seId);

s8 lbl_1_data_E40[14] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 9, 9, 9, 9 };
char lbl_1_data_E4E[3] = "%d";
char lbl_1_data_E51[7] = "second";
char lbl_1_data_E58[6] = "third";

void fn_1_15F24(void)
{
    s16 win = fn_1_458();

    fn_1_940(1);
    fn_1_46C();
    HuAudFXPlayPan(0x2F5, 0x40);
    HuWinMesSet(win, 0xF0000);
    HuWinMesWait(win);
    fn_1_924(1);
    HuPrcEnd();
}

void fn_1_15F88(void)
{
    char buf[8];
    s16 win = fn_1_458();

    fn_1_940(2);
    fn_1_46C();
    HuAudFXPlayPan(0x2F5, 0x28);
    sprintf(buf, "%d", fn_1_510());
    HuWinMesSet(win, 0xF0001);
    HuWinInsertMesSet(win, MESSNUM_PTR(buf), 0);
    HuWinMesWait(win);
    fn_1_924(2);
    HuPrcEnd();
}

void fn_1_16018(void)
{
    s16 win = fn_1_458();

    fn_1_940(4);
    fn_1_46C();
    HuAudFXPlayPan(0x2F5, 0x28);
    HuWinMesSet(win, 0xF0005);
    HuWinMesWait(win);
    fn_1_924(4);
    HuPrcEnd();
}

void fn_1_16080(void)
{
    s16 win = fn_1_458();

    fn_1_940(8);
    fn_1_46C();
    HuWinMesSet(win, 0xF0006);
    HuWinInsertMesSet(win, fn_1_528(), 0);
    HuWinMesWait(win);
    fn_1_924(8);
    HuPrcEnd();
}

void fn_1_160F0(void)
{
    s16 win = fn_1_458();
    s32 choice;
    s32 retry;

    fn_1_940(0x10);
    fn_1_898(6);
    fn_1_46C();
    do {
        retry = 0;
        HuWinMesSet(win, 0xF0007);
        HuWinMesWait(win);
        choice = HuWinChoiceGet(win, -1);
        if (choice < 0) {
            choice = 1;
        }
        if (choice == 1) {
            HuWinMesSet(win, 0xF0008);
            HuWinMesWait(win);
            if (HuWinChoiceGet(win, -1) == 0) {
                fn_1_87C(2);
            } else {
                retry = 1;
            }
        } else if (choice == 2) {
            fn_1_87C(4);
        }
    } while (retry != 0);
    fn_1_924(0x10);
    HuPrcEnd();
}

void fn_1_161E8(void)
{
    s16 win = fn_1_458();

    fn_1_940(0x20);
    fn_1_46C();
    HuAudFXPlayPan(0x2F6, 0x28);
    HuWinMesSet(win, 0xF0009);
    HuWinMesWait(win);
    fn_1_924(0x20);
    HuPrcEnd();
}

void fn_1_16250(void)
{
    s16 win = fn_1_458();

    fn_1_940(0x40);
    fn_1_46C();
    switch (fn_1_59C()) {
        case 0:
            HuWinMesSet(win, 0xF000A);
            HuWinInsertMesSet(win, lbl_1_data_E40[GwPlayerConf[fn_1_588()].charNo], 0);
            break;
        case 2:
            HuWinMesSet(win, 0xF000B);
            if (fn_1_588() == 0) {
                HuWinInsertMesSet(win, lbl_1_data_E40[GwPlayerConf[fn_1_5B0(0)].charNo], 0);
                HuWinInsertMesSet(win, lbl_1_data_E40[GwPlayerConf[fn_1_5B0(1)].charNo], 1);
            } else {
                HuWinInsertMesSet(win, lbl_1_data_E40[GwPlayerConf[fn_1_5B0(2)].charNo], 0);
                HuWinInsertMesSet(win, lbl_1_data_E40[GwPlayerConf[fn_1_5B0(3)].charNo], 1);
            }
            break;
        case 1:
            if (fn_1_588() == 0) {
                HuWinMesSet(win, 0xF000A);
                HuWinInsertMesSet(win, lbl_1_data_E40[GwPlayerConf[fn_1_5B0(0)].charNo], 0);
            } else {
                HuWinMesSet(win, 0xF000C);
                HuWinInsertMesSet(win, lbl_1_data_E40[GwPlayerConf[fn_1_5B0(1)].charNo], 0);
                HuWinInsertMesSet(win, lbl_1_data_E40[GwPlayerConf[fn_1_5B0(2)].charNo], 1);
                HuWinInsertMesSet(win, lbl_1_data_E40[GwPlayerConf[fn_1_5B0(3)].charNo], 2);
            }
            break;
    }
    HuWinMesWait(win);
    fn_1_924(0x40);
    HuPrcEnd();
}

void fn_1_16574(void)
{
    s16 win = fn_1_458();

    fn_1_940(0x80);
    fn_1_46C();
    HuAudFXPlayPan(0x2F4, 0x28);
    switch (fn_1_59C()) {
        case 0:
            HuWinMesSet(win, 0xF000D);
            HuWinInsertMesSet(win, lbl_1_data_E40[GwPlayerConf[fn_1_624()].charNo], 0);
            break;
        case 2:
            HuWinMesSet(win, 0xF000E);
            if (fn_1_588() == 0) {
                HuWinInsertMesSet(win, lbl_1_data_E40[GwPlayerConf[fn_1_5B0(0)].charNo], 0);
                HuWinInsertMesSet(win, lbl_1_data_E40[GwPlayerConf[fn_1_5B0(1)].charNo], 1);
            } else {
                HuWinInsertMesSet(win, lbl_1_data_E40[GwPlayerConf[fn_1_5B0(2)].charNo], 0);
                HuWinInsertMesSet(win, lbl_1_data_E40[GwPlayerConf[fn_1_5B0(3)].charNo], 1);
            }
            break;
        case 1:
            if (fn_1_588() == 0) {
                HuWinMesSet(win, 0xF000D);
                HuWinInsertMesSet(win, lbl_1_data_E40[GwPlayerConf[fn_1_5B0(0)].charNo], 0);
            } else {
                HuWinMesSet(win, 0xF000F);
                HuWinInsertMesSet(win, lbl_1_data_E40[GwPlayerConf[fn_1_5B0(1)].charNo], 0);
                HuWinInsertMesSet(win, lbl_1_data_E40[GwPlayerConf[fn_1_5B0(2)].charNo], 1);
                HuWinInsertMesSet(win, lbl_1_data_E40[GwPlayerConf[fn_1_5B0(3)].charNo], 2);
            }
            break;
    }
    HuWinMesWait(win);
    fn_1_924(0x80);
    HuPrcEnd();
}

void fn_1_168A4(void)
{
    char buf[48];
    s32 place;
    s32 kind;
    s16 win = fn_1_458();

    fn_1_940(0x100);
    fn_1_46C();
    fn_1_64C(&place, &kind, 0);
    HuAudFXPlayPan(0x2F4, 0x28);
    HuWinMesSet(win, 0xF0010);
#ifdef TARGET_PC
    buf[0] = 0;
#endif
    switch (GwCommon.languageNo) {
        case 0:
            sprintf(buf, lbl_1_data_E4E, place);
            break;
        case 1:
            switch (place) {
                case 2:
                    sprintf(buf, lbl_1_data_E51);
                    break;
                case 3:
                    sprintf(buf, lbl_1_data_E58);
                    break;
            }
            break;
    }
    HuWinInsertMesSet(win, MESSNUM_PTR(buf), 0);
    if (kind == 2) {
        HuWinInsertMesSet(win, 0xF0019, 1);
    } else {
        HuWinInsertMesSet(win, 0xF001A, 1);
    }
    HuWinMesWait(win);
    fn_1_924(0x100);
    HuPrcEnd();
}

void fn_1_169E0(void)
{
    s16 win = fn_1_458();

    fn_1_940(0x200);
    fn_1_46C();
    HuAudFXPlayPan(0x2F5, 0x28);
    HuWinMesSet(win, 0xF0011);
    HuWinMesWait(win);
    fn_1_924(0x200);
    HuPrcEnd();
}

void fn_1_16A48(void)
{
    s16 win = fn_1_458();

    fn_1_940(0x400);
    fn_1_46C();
    HuWinMesSet(win, 0xF0012);
    HuWinMesWait(win);
    if (HuWinChoiceGet(win, -1) == 0) {
        fn_1_87C(0x10);
    }
    fn_1_924(0x400);
    HuPrcEnd();
}

void fn_1_16AC4(void)
{
    s16 win = fn_1_458();

    fn_1_940(0x800);
    fn_1_46C();
    HuWinMesSet(win, 0xF0013);
    HuWinMesWait(win);
    fn_1_924(0x800);
    HuPrcEnd();
}

void fn_1_16B20(void)
{
    s16 win = fn_1_458();

    fn_1_940(0x800000);
    fn_1_46C();
    HuWinMesSet(win, 0xF0014);
    HuWinMesWait(win);
    fn_1_924(0x800000);
    HuPrcEnd();
}

void fn_1_16B7C(void)
{
    s16 win = fn_1_458();

    fn_1_940(0x1000000);
    fn_1_46C();
    HuWinMesSet(win, 0xF0015);
    HuWinInsertMesSet(win, lbl_1_data_E40[fn_1_638()], 0);
    HuWinMesWait(win);
    fn_1_924(0x1000000);
    HuPrcEnd();
}

void fn_1_16BFC(void)
{
    s16 win = fn_1_458();

    fn_1_940(0x2000000);
    fn_1_46C();
    HuWinMesSet(win, 0xF0016);
    HuWinInsertMesSet(win, lbl_1_data_E40[fn_1_638()], 0);
    HuWinMesWait(win);
    fn_1_924(0x2000000);
    HuPrcEnd();
}

void fn_1_16C7C(void)
{
    s16 win = fn_1_458();

    fn_1_940(0x4000000);
    fn_1_46C();
    HuWinMesSet(win, 0xF0017);
    HuWinMesWait(win);
    fn_1_924(0x4000000);
    HuPrcEnd();
}

void fn_1_16CD8(void)
{
    s16 win = fn_1_458();

    fn_1_940(0x8000000);
    fn_1_46C();
    HuWinMesSet(win, 0xF0018);
    HuWinMesWait(win);
    fn_1_924(0x8000000);
    HuPrcEnd();
}

void fn_1_16D34(void)
{
    HUWINID win;
    s32 humans;
    s32 i;

    fn_1_940(0x1000);
    humans = 0;
    for (i = 0; i < 4; i++) {
        if (GwPlayerConf[i].type != 0) {
            humans++;
        }
    }
    if (humans == 4) {
        fn_1_924(0x1000);
        HuPrcEnd();
        return;
    }
    win = HuWinCreate(-10000.0f, 380.0f, 0x190, 0x28, 0);
    HuWinMesSpeedSet(win, 0);
    HuWinBGTPLvlSet(win, 0.0f);
    HuWinPriSet(win, 5);
    HuWinAttrSet(win, 0x800);
    HuWinMesSet(win, 0x2000D);
    HuWinMesWait(win);
    do {
        humans = 0;
        for (i = 0; i < 4; i++) {
            if ((HuPadBtnDown[i] & 0x100) != 0) {
                HuAudFXPlay(0);
                humans = 4;
                break;
            }
        }
        if (humans != 4) {
            HuPrcVSleep();
        }
    } while (humans != 4);
    HuWinKill(win);
    fn_1_924(0x1000);
    HuPrcEnd();
}
