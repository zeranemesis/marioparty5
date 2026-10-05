#include "types.h"

extern s16 Hu3DCameraBit;

void MgSeqModeDelaySet(s16 delay, s32 mode);
void MgSeqModeNext(void);
void Hu3DZClear(void);
s32 WipeCheckEnd(void);

void fn_1_2AA0(void);

void fn_1_664(volatile s16 arg0)
{
    MgSeqModeDelaySet((s16)(arg0 + 1), -1);
    fn_1_2AA0();
}

void fn_1_6E0(void)
{
    MgSeqModeNext();
}

void fn_1_700(void)
{
    if ((u8)WipeCheckEnd() == 0) {
        MgSeqModeNext();
    }
}

void fn_1_730(void)
{
}

void fn_1_AE8(void)
{
    if ((Hu3DCameraBit & 4) != 0) {
        Hu3DZClear();
    }
}
