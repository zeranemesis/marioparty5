#include "types.h"

extern void HuAudAllStop(void);
extern void MgSeqModeNext(void);
extern void MgTimerDoneCheck(s32 timer);
extern void MgTimerModeOnSet(s32 timer, s32 mode);
extern s32 fn_1_6170(void);
extern s32 fn_1_6BDC(void);

void fn_1_324(void)
{
    if (fn_1_6170() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_42C(void)
{
    fn_1_6BDC();
}

void fn_1_1B30(s32 timer)
{
    MgTimerModeOnSet(timer, 1);
}

void fn_1_1B5C(s32 timer)
{
    MgTimerDoneCheck(timer);
}

void fn_1_3AC4(void)
{
    HuAudAllStop();
}