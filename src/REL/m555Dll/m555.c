// m555Dll/REL/m555Dll/m555 - decompilation of the m555Dll/m555 unit
// (12304 B of text, 44 functions). Real code is added function by function,
// smallest first, each verified with tools/fndiff.py before the next one.

#include "types.h"
#include "game/audio.h"

extern s32 fn_1_6A0C(void);
extern s32 fn_1_6A60(void);
extern void fn_1_71B4(void);
extern void MgSeqModeNext(void);
extern void MgTimerDoneCheck(s32 timer);

void fn_1_33C(void)
{
    if (fn_1_6A0C() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_368(void)
{
    if (fn_1_6A60() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_444(void)
{
    fn_1_71B4();
}

void fn_1_1B74(volatile s32 timer)
{
    MgTimerDoneCheck(timer);
}

void fn_1_2FF0(void)
{
    HuAudAllStop();
}