#include "types.h"

void MgTimerDoneCheck(s32 timer);
void fn_1_4E3C(void);
void fn_1_5098(void);
void fn_1_5514(void);
void HuAudAllStop(void);

void fn_1_1ABC(s32 timer)
{
    MgTimerDoneCheck(timer);
}

void fn_1_3DC(void)
{
    fn_1_4E3C();
}

void fn_1_428(void)
{
    fn_1_5098();
}

void fn_1_4A0(void)
{
    fn_1_5514();
}

void fn_1_4308(void)
{
    HuAudAllStop();
}