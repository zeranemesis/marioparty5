#include "dolphin/types.h"
#include "game/gamework.h"

extern "C" {

extern s32 lbl_1_bss_A80;
extern s32 lbl_1_bss_A84;
extern s32 lbl_1_bss_A88;

BOOL fn_1_5634(void *self, void *arg1);
void fn_1_11268(void *arg);
void fn_1_11380(void *arg);

// fn_1_FD14 - Returns 1 if GwPlayerConf[playerNo].grp == 0, else 0
// NONMATCHING: original uses cntlzw/extsh trick to implement zero-check without branches
s32 fn_1_FD14(s32 playerNo)
{
    return (GwPlayerConf[playerNo].grp == 0) ? 1 : 0;
}

// fn_1_11F38 - Initializes three BSS variables
void fn_1_11F38(void)
{
    lbl_1_bss_A80 = 0x78;
    lbl_1_bss_A84 = 0xD2;
    lbl_1_bss_A88 = 0xD2;
}

// fn_1_11230 - Calls fn_1_11268 and fn_1_11380
void fn_1_11230(void *arg)
{
    fn_1_11268(arg);
    fn_1_11380(arg);
}

// fn_1_FD38 - If fn_1_5634(arg1, arg2) != 0, increment *arg0
void fn_1_FD38(s32 *arg0, void *arg1, void *arg2)
{
    if ((u8)fn_1_5634(arg1, arg2) != 0) {
        (*arg0)++;
    }
}

}