#include "REL/m510dll.h"

s32 fn_1_5B4(s32 seId)
{
    return HuAudFXPlay(seId);
}

void fn_1_5DC(s32 seNo)
{
    HuAudFXStop(seNo);
}

void fn_1_714(void)
{
    HuAudAllStop();
}