#include "REL/mgtourDll.h"

extern void fn_1_708(void);
extern void fn_1_7AC(u32 mask);
extern void fn_1_7C8(u32 mask);
extern void fn_1_854(u32 mask);
extern void fn_1_870(u32 mask);
extern s32 fn_1_8A4(void);
#ifdef TARGET_PC
extern s16 fn_1_904(s32 arg);
#else
extern s32 fn_1_904(s32 arg);
#endif
extern s32 fn_1_96C(void);
extern s32 fn_1_984(void);
extern HUWINID fn_1_660(void);

s8 lbl_1_data_C18[14] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 9, 9, 9, 9 };
#define charMessTbl lbl_1_data_C18

void fn_1_FBD0(void)
{
    HUWINID winId = fn_1_660();

    fn_1_870(0x1);
    fn_1_708();
    HuAudFXPlayPan(757, 0x28);
    HuWinMesSet(winId, 0x120000);
    HuWinMesWait(winId);
    fn_1_854(0x1);
    HuPrcEnd();
}

void fn_1_FC34(void)
{
    HUWINID winId = fn_1_660();

    fn_1_870(0x2);
    fn_1_708();
    HuWinMesSet(winId, 0x120001);
    HuWinMesWait(winId);
    fn_1_854(0x2);
    HuPrcEnd();
}

void fn_1_FC90(void)
{
    HUWINID winId = fn_1_660();

    fn_1_870(0x4);
    fn_1_708();
    HuWinMesSet(winId, 0x120002);
    HuWinInsertMesSet(winId, charMessTbl[fn_1_904(0)], 0);
    HuWinInsertMesSet(winId, charMessTbl[fn_1_904(1)], 1);
    HuWinMesWait(winId);
    fn_1_854(0x4);
    HuPrcEnd();
}

void fn_1_FD3C(void)
{
    HUWINID winId = fn_1_660();

    fn_1_870(0x8);
    fn_1_708();
    HuWinMesSet(winId, 0x120003);
    HuWinInsertMesSet(winId, charMessTbl[fn_1_904(0)], 0);
    HuWinInsertMesSet(winId, charMessTbl[fn_1_904(1)], 1);
    HuWinMesWait(winId);
    fn_1_854(0x8);
    HuPrcEnd();
}

void fn_1_FDE8(void)
{
    HUWINID winId = fn_1_660();

    fn_1_870(0x10);
    fn_1_708();
    HuWinMesSet(winId, 0x120004);
    HuWinInsertMesSet(winId, charMessTbl[fn_1_904(0)], 0);
    HuWinInsertMesSet(winId, charMessTbl[fn_1_904(1)], 1);
    HuWinMesWait(winId);
    fn_1_854(0x10);
    HuPrcEnd();
}

void fn_1_FE94(void)
{
    HUWINID winId = fn_1_660();

    fn_1_870(0x20);
    fn_1_708();
    HuWinMesSet(winId, 0x120005);
    HuWinInsertMesSet(winId, charMessTbl[fn_1_904(0)], 0);
    HuWinInsertMesSet(winId, charMessTbl[fn_1_904(1)], 1);
    HuWinMesWait(winId);
    fn_1_854(0x20);
    HuPrcEnd();
}

void fn_1_FF40(void)
{
    HUWINID winId = fn_1_660();

    fn_1_870(0x40);
    fn_1_708();
    HuWinMesSet(winId, 0x120006);
    HuWinMesWait(winId);
    fn_1_854(0x40);
    HuPrcEnd();
}

void fn_1_FF9C(void)
{
    HUWINID winId = fn_1_660();

    fn_1_870(0x80);
    fn_1_708();
    HuAudFXPlayPan(756, 0x40);
    HuWinMesSet(winId, 0x120007);
    HuWinInsertMesSet(winId, fn_1_8A4(), 0);
    HuWinMesWait(winId);
    fn_1_854(0x80);
    HuPrcEnd();
}

void fn_1_10018(void)
{
    s32 mode;
    s32 choice;
    HUWINID winId = fn_1_660();

    fn_1_870(0x100);
    fn_1_7C8(6);
    fn_1_708();
    do {
        mode = 0;
        HuWinMesSet(winId, 0x120008);
        HuWinMesWait(winId);
        choice = HuWinChoiceGet(winId, -1);
        if (choice < 0) {
            choice = 1;
        }
        if (choice == 1) {
            HuWinMesSet(winId, 0x120009);
            HuWinMesWait(winId);
            if (HuWinChoiceGet(winId, -1) == 0) {
                fn_1_7AC(2);
            } else {
                mode = 1;
            }
        } else if (choice == 2) {
            fn_1_7AC(4);
        }
    } while (mode != 0);
    fn_1_854(0x100);
    HuPrcEnd();
}

void fn_1_10110(void)
{
    HUWINID winId = fn_1_660();

    fn_1_870(0x200);
    fn_1_708();
    HuAudFXPlayPan(758, 0x28);
    HuWinMesSet(winId, 0x12000a);
    HuWinMesWait(winId);
    fn_1_854(0x200);
    HuPrcEnd();
}

void fn_1_10178(void)
{
    HUWINID winId = fn_1_660();

    fn_1_870(0x400);
    fn_1_708();
    HuWinMesSet(winId, 0x12000b);
    HuWinInsertMesSet(winId, charMessTbl[fn_1_904(fn_1_96C())], 0);
    HuWinMesWait(winId);
    fn_1_854(0x400);
    HuPrcEnd();
}

void fn_1_101FC(void)
{
    HUWINID winId = fn_1_660();

    fn_1_870(0x800);
    fn_1_708();
    HuAudFXPlayPan(756, 0x28);
    HuWinMesSet(winId, 0x12000c);
    HuWinInsertMesSet(winId, charMessTbl[fn_1_984()], 0);
    HuWinMesWait(winId);
    fn_1_854(0x800);
    HuPrcEnd();
}

void fn_1_10288(void)
{
    HUWINID winId = fn_1_660();

    fn_1_870(0x1000);
    fn_1_708();
    HuWinMesSet(winId, 0x12000d);
    HuWinMesWait(winId);
    fn_1_854(0x1000);
    HuPrcEnd();
}

void fn_1_102E4(void)
{
    HUWINID winId = fn_1_660();

    fn_1_870(0x2000);
    fn_1_708();
    HuWinMesSet(winId, 0x12000e);
    HuWinMesWait(winId);
    if (HuWinChoiceGet(winId, -1) == 0) {
        fn_1_7AC(0x10);
    }
    fn_1_854(0x2000);
    HuPrcEnd();
}
