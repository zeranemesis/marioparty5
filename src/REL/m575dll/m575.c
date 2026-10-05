#include "types.h"

// REL/m575dll/m575.c thin audio/utility forwarders. The Hu* entry points live
// in another object, so they are declared here rather than pulled in from
// game/audio.h, which does not survive MWCC's header handling.
int HuAudFXPlay(int seId);
void HuAudFXStop(int seNo);
int HuAudSStreamPlay(s16 streamId);
void HuAudAllStop(void);
void fn_1_E424(s32 arg);

int fn_1_530(s32 seId)
{
    return HuAudFXPlay(seId);
}

void fn_1_558(s32 seNo)
{
    HuAudFXStop(seNo);
}

int fn_1_638(s16 streamId)
{
    return HuAudSStreamPlay(streamId);
}

void fn_1_690(void)
{
    HuAudAllStop();
}

void fn_1_1C00(s32 arg)
{
    fn_1_E424(arg);
}