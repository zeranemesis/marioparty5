#include "game/card.h"
#include "game/saveload.h"
#include "game/hu3d.h"
#include "game/window.h"
#include "game/sprite.h"

extern u8 lbl_1_bss_770[0x2C];
extern void fn_1_46C4(void*, int);
extern s32 fn_1_160A4(s32);
extern s16 lbl_1_bss_58C[0x10];
extern s16 lbl_1_bss_75E;
extern s16 lbl_1_bss_978;
extern void fn_1_7298(void);
extern s16 lbl_1_bss_754[5];
extern char lbl_1_bss_762[6];
extern s16 lbl_1_bss_76C;

#define MESS_ICE_HOCKEY_44 0x192C

void fn_1_14A60(void);
void fn_1_15F58(s16 arg0);
void fn_1_16024(void);
void fn_1_16064(void);
s32 fn_1_16AB0(s32 length, void* addr);
s32 fn_1_16B20(void);

void fn_1_14A60(void)
{
    Hu3DModelAttrReset(lbl_1_bss_58C[1], 1);
    Hu3DModelAttrReset(lbl_1_bss_58C[5], 1);
    Hu3DModelAttrReset(lbl_1_bss_58C[3], 1);
    Hu3DModelAttrReset(lbl_1_bss_58C[4], 1);
    Hu3DModelAttrReset(lbl_1_bss_58C[6], 1);
    fn_1_7298();
    HuWinKill(lbl_1_bss_75E);
    HuSprGrpKill(lbl_1_bss_978);
}

void fn_1_15F58(s16 arg0)
{
    s16 winId = lbl_1_bss_754[4];
    sprintf(lbl_1_bss_762, "%02d", arg0);
    sprintf(&lbl_1_bss_762[3], "%02d", lbl_1_bss_76C);
    HuWinInsertMesSet(winId, (u32)lbl_1_bss_762, 0);
    HuWinInsertMesSet(winId, (u32)&lbl_1_bss_762[3], 1);
    HuWinMesSet(winId, (u32)(0x19 << 16 | 0x2C));
    HuWinScaleSet(winId, 0.8f, 0.8f);
}

void fn_1_16024(void)
{
    if (*(s16*)&lbl_1_bss_770[0x1A] != -1) {
        fn_1_46C4(lbl_1_bss_770, 1);
    }
}

void fn_1_16064(void)
{
    if (*(s16*)&lbl_1_bss_770[0x1A] != -1) {
        fn_1_46C4(lbl_1_bss_770, 0);
    }
}

s32 fn_1_16AB0(s32 length, void* addr)
{
    s32 result = HuCardRead(&curFileInfo, addr, length, 0);
    if (result == -3) {
        fn_1_160A4(0);
    } else if (result < 0) {
        fn_1_160A4(1);
    }
    return result;
}

s32 fn_1_16B20(void)
{
    s32 result = HuCardClose(&curFileInfo);
    return result;
}