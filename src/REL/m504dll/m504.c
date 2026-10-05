#include "REL/m504dll.h"
#include "game/audio.h"

void fn_1_5EC(s32 seId)
{
    HuAudFXPlay(seId);
}

void fn_1_614(s32 seNo)
{
    HuAudFXStop(seNo);
}

void fn_1_6F4(s16 streamId)
{
    HuAudSStreamPlay(streamId);
}

void fn_1_74C(void)
{
    HuAudAllStop();
}

void fn_1_76C(s16 streamId)
{
    HuAudBGMPlay(streamId);
}